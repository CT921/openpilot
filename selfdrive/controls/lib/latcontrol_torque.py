import math
import numpy as np
from collections import deque

from cereal import log
from opendbc.car.lateral import FRICTION_THRESHOLD, get_friction
from openpilot.common.constants import ACCELERATION_DUE_TO_GRAVITY
from openpilot.common.filter_simple import FirstOrderFilter
from openpilot.common.params import Params
from openpilot.selfdrive.controls.lib.drive_helpers import CONTROL_N
from openpilot.selfdrive.controls.lib.latcontrol import LatControl
from openpilot.common.pid import PIDController
from openpilot.selfdrive.modeld.constants import ModelConstants

# At higher speeds (25+mph) we can assume:
# Lateral acceleration achieved by a specific car correlates to
# torque applied to the steering rack. It does not correlate to
# wheel slip, or to speed.

# This controller applies torque to achieve desired lateral
# accelerations. To compensate for the low speed effects the
# proportional gain is increased at low speeds by the PID controller.
# Additionally, there is friction in the steering wheel that needs
# to be overcome to move it at all, this is compensated for too.

KP = 0.8
KI = 0.15
INTERP_SPEEDS = [1, 1.5, 2.0, 3.0, 5, 7.5, 10, 15, 30]
KP_INTERP = [250, 120, 65, 30, 11.5, 5.5, 3.5, 2.0, KP]

LP_FILTER_CUTOFF_HZ = 1.2
JERK_LOOKAHEAD_SECONDS = 0.19
JERK_GAIN = 0.3
LAT_ACCEL_REQUEST_BUFFER_SECONDS = 1.0
VERSION = 1

# NNFF specific parameters
LOW_SPEED_X = [0, 10, 20, 30]
LOW_SPEED_Y_NN = [12, 3, 1, 0]
LAT_PLAN_MIN_IDX = 5
LATERAL_LAG_MOD = 0.0

def get_predicted_lateral_jerk(lat_accels, t_diffs):
  # compute finite difference between subsequent model_data.acceleration.y values
  # this is just two calls of np.diff followed by an element-wise division
  lat_accel_diffs = np.diff(lat_accels)
  lat_jerk = lat_accel_diffs / t_diffs
  # return as python list
  return lat_jerk.tolist()

def sign(x):
  return 1.0 if x > 0.0 else (-1.0 if x < 0.0 else 0.0)

def get_lookahead_value(future_vals, current_val):
  if len(future_vals) == 0:
    return current_val

  same_sign_vals = [v for v in future_vals if sign(v) == sign(current_val)]

  # if any future val has opposite sign of current val, return 0
  if len(same_sign_vals) < len(future_vals):
    return 0.0

  # otherwise return the value with minimum absolute value
  min_val = min(same_sign_vals + [current_val], key=lambda x: abs(x))
  return min_val

# At a given roll, if pitch magnitude increases, the
# gravitational acceleration component starts pointing
# in the longitudinal direction, decreasing the lateral
# acceleration component. Here we do the same thing
# to the roll value itself, then passed to nnff.
def roll_pitch_adjust(roll, pitch):
  return roll * math.cos(pitch)


class LatControlTorque(LatControl):
  def __init__(self, CP, CI, dt):
    super().__init__(CP, CI, dt)
    self.steerActuatorDelay = CP.steerActuatorDelay
    self.torque_params = CP.lateralTuning.torque.as_builder()
    self.torque_from_lateral_accel = CI.torque_from_lateral_accel()
    self.lateral_accel_from_torque = CI.lateral_accel_from_torque()
    self.steering_angle_deadzone_deg = self.torque_params.steeringAngleDeadzoneDeg

    self.pid_official = PIDController([INTERP_SPEEDS, KP_INTERP], KI, rate=1/self.dt)
    # Twilsonco's Lateral Neural Network Feedforward
    self.use_nn = CI.has_lateral_torque_nn if hasattr(CI, 'has_lateral_torque_nn') else False
    if self.use_nn:
      self.pid_nnff = PIDController(self.torque_params.kp, self.torque_params.ki, rate=1/self.dt)

    self.pid = self.pid_official
    self.update_limits()

    # Official new jerk lookahead (for standard mode)
    self.lat_accel_request_buffer_len = int(LAT_ACCEL_REQUEST_BUFFER_SECONDS / self.dt)
    self.lat_accel_request_buffer = deque([0.] * self.lat_accel_request_buffer_len, maxlen=self.lat_accel_request_buffer_len)
    self.lookahead_frames = int(JERK_LOOKAHEAD_SECONDS / self.dt)
    # Official new jerk filter (for standard mode)
    self.jerk_filter = FirstOrderFilter(0.0, 1 / (2 * np.pi * LP_FILTER_CUTOFF_HZ), self.dt)

    # NNFF: friction factor initialisation
    if self.use_nn:
      self.lat_jerk_friction_factor = 0.4
      self.lat_accel_friction_factor_default = 0.7
      self.lat_accel_friction_factor = self.lat_accel_friction_factor_default

      # Lookahead configuration (speed-dependent)
      self.friction_look_ahead_v = [1.4, 2.0]   # seconds
      self.friction_look_ahead_bp = [9.0, 30.0]  # m/s

      # Precompute time differences for jerk calculation
      self.t_diffs = np.diff(ModelConstants.T_IDXS)
      self.desired_lat_jerk_time = self.steerActuatorDelay + LATERAL_LAG_MOD
      self.pitch = FirstOrderFilter(0.0, 0.5, 0.01)
      self.pitch_last = 0.0
      # NN model takes current v_ego, lateral_accel, lat accel/jerk error, roll, and past/future/planned data
      # of lat accel and roll
      # Past value is computed using previous desired lat accel and observed roll
      self.torque_from_nn = CI.get_ff_nn
      self.nn_friction_override = CI.lat_torque_nn_model.friction_override

      # setup future time offsets
      self.future_times = [0.3, 0.6, 1.0, 1.5]  # seconds in the future
      self.nn_future_times = [t + self.desired_lat_jerk_time for t in self.future_times]
      self.nn_future_times_np = np.array(self.nn_future_times)

      # setup past time offsets
      self.past_times = [-0.3, -0.2, -0.1]
      history_check_frames = [int(abs(t) * 100) for t in self.past_times]
      self.history_frame_offsets = [history_check_frames[0] - f for f in history_check_frames]
      self.lateral_accel_desired_deque = deque(maxlen=history_check_frames[0])
      self.roll_deque = deque(maxlen=history_check_frames[0])
      self.past_future_len = len(self.past_times) + len(self.nn_future_times)

  def update_lateral_lag(self, lag):
    if Params().get_bool("DisableLagLearning"):
      self.desired_lat_jerk_time = self.steerActuatorDelay + 0.3

      if self.use_nn:
        nn_time_offset = self.steerActuatorDelay + 0.2
        self.nn_future_times = [t + nn_time_offset for t in self.future_times]
        self.nn_future_times_np = np.array(self.nn_future_times)
    else:
      self.desired_lat_jerk_time = max(0.01, lag) + LATERAL_LAG_MOD

      if self.use_nn:
        self.nn_future_times = [t + self.desired_lat_jerk_time for t in self.future_times]
        self.nn_future_times_np = np.array(self.nn_future_times)

  def update_live_torque_params(self, latAccelFactor, latAccelOffset, friction):
    self.torque_params.latAccelFactor = latAccelFactor
    self.torque_params.latAccelOffset = latAccelOffset
    self.torque_params.friction = friction
    self.update_limits()

  def update_limits(self):
    limit_pos = self.lateral_accel_from_torque(self.steer_max, self.torque_params)
    limit_neg = self.lateral_accel_from_torque(-self.steer_max, self.torque_params)
    self.pid_official.set_limits(limit_pos, limit_neg)
    if self.use_nn:
      self.pid_nnff.set_limits(limit_pos, limit_neg)

  def update(self, active, CS, VM, params, steer_limited_by_safety, desired_curvature, calibrated_pose, curvature_limited, lat_delay, model_data=None):
    pid_log = log.ControlsState.LateralTorqueState.new_message()
    pid_log.version = VERSION
    measured_curvature = -VM.calc_curvature(math.radians(CS.steeringAngleDeg - params.angleOffsetDeg), CS.vEgo, params.roll)
    actual_lateral_accel = measured_curvature * CS.vEgo ** 2   # pure lateral accel, no low-speed factor
    roll_compensation = params.roll * ACCELERATION_DUE_TO_GRAVITY
    curvature_deadzone = abs(VM.calc_curvature(math.radians(self.steering_angle_deadzone_deg), CS.vEgo, 0.0))
    lateral_accel_deadzone = curvature_deadzone * CS.vEgo ** 2

    model_good = (model_data is not None and hasattr(model_data, 'orientation') and
                  hasattr(model_data.orientation, 'x') and len(list(model_data.orientation.x)) >= CONTROL_N)

    disable_lag = Params().get_bool("DisableLagLearning")

    if disable_lag and self.use_nn and model_good:
      self.pid = self.pid_nnff

      desired_lateral_accel = desired_curvature * CS.vEgo ** 2
      low_speed_factor = float(np.interp(CS.vEgo, LOW_SPEED_X, LOW_SPEED_Y_NN)) ** 2
      setpoint = desired_lateral_accel + low_speed_factor * desired_curvature
      measurement = actual_lateral_accel + low_speed_factor * measured_curvature
      actual_curvature_rate = -VM.calc_curvature(math.radians(CS.steeringRateDeg), CS.vEgo, 0.0)
      actual_lateral_jerk = actual_curvature_rate * CS.vEgo ** 2
      predicted_lateral_jerk = get_predicted_lateral_jerk(model_data.acceleration.y, self.t_diffs)
      lookahead = np.interp(CS.vEgo, self.friction_look_ahead_bp, self.friction_look_ahead_v)
      friction_upper_idx = next((i for i, val in enumerate(ModelConstants.T_IDXS) if val > lookahead), 16)
      desired_lateral_jerk_model = (np.interp(self.desired_lat_jerk_time, ModelConstants.T_IDXS, model_data.acceleration.y) - desired_lateral_accel) / self.desired_lat_jerk_time
      lookahead_lateral_jerk = get_lookahead_value(predicted_lateral_jerk[LAT_PLAN_MIN_IDX:friction_upper_idx], desired_lateral_jerk_model)

      if lookahead_lateral_jerk == 0.0:
        actual_lateral_jerk = 0.0
        self.lat_accel_friction_factor = 1.0
      else:
        self.lat_accel_friction_factor = self.lat_accel_friction_factor_default

      lateral_jerk_setpoint = self.lat_jerk_friction_factor * lookahead_lateral_jerk
      lateral_jerk_measurement = self.lat_jerk_friction_factor * actual_lateral_jerk

    # FF and error calculation
      pitch = self.pitch.update(calibrated_pose.orientation.pitch) if calibrated_pose is not None else 0.0
      roll = roll_pitch_adjust(params.roll, pitch)
      self.pitch_last = pitch
      self.roll_deque.append(roll)
      self.lateral_accel_desired_deque.append(desired_lateral_accel)

      if not active:
        output_torque = 0.0
        pid_log.active = False
      else:
        past_rolls = [self.roll_deque[min(len(self.roll_deque)-1, i)] for i in self.history_frame_offsets]
        past_lateral_accels_desired = [self.lateral_accel_desired_deque[min(len(self.lateral_accel_desired_deque)-1, i)] for i in self.history_frame_offsets]
        adjusted_future_times = [t + 0.5 * CS.aEgo * (t / max(CS.vEgo, 1.0)) for t in self.nn_future_times]

        future_rolls = [roll_pitch_adjust(np.interp(t, ModelConstants.T_IDXS, model_data.orientation.x) + roll,
                        np.interp(t, ModelConstants.T_IDXS, model_data.orientation.y) + self.pitch_last) for t in adjusted_future_times]
        future_planned_lateral_accels = [np.interp(t, ModelConstants.T_IDXS, model_data.acceleration.y) for t in adjusted_future_times]

        friction_input = self.lat_accel_friction_factor * (setpoint - measurement) + lateral_jerk_setpoint

        nnff_setpoint_input = [CS.vEgo, setpoint, lateral_jerk_setpoint, roll] + [setpoint] * self.past_future_len + past_rolls + future_rolls
        nnff_measurement_input = [CS.vEgo, measurement, lateral_jerk_measurement, roll] + [measurement] * self.past_future_len + past_rolls + future_rolls

        torque_from_setpoint = self.torque_from_nn(nnff_setpoint_input)
        torque_from_measurement = self.torque_from_nn(nnff_measurement_input)
        pid_log.error = torque_from_setpoint - torque_from_measurement

        error_blend_factor = float(np.interp(abs(desired_lateral_accel), [1.0, 2.0], [0.0, 1.0]))
        if error_blend_factor > 0.0:
          nnff_error_input = [CS.vEgo, setpoint - measurement, lateral_jerk_setpoint - lateral_jerk_measurement, 0.0]
          torque_from_error = self.torque_from_nn(nnff_error_input)
          if sign(pid_log.error) == sign(torque_from_error) and abs(pid_log.error) < abs(torque_from_error):
            pid_log.error = pid_log.error * (1.0 - error_blend_factor) + torque_from_error * error_blend_factor

        nn_input = [CS.vEgo, desired_lateral_accel, friction_input, roll] + past_lateral_accels_desired + future_planned_lateral_accels + past_rolls + future_rolls
        nn_torque = self.torque_from_nn(nn_input)
        scaled_nn_torque = nn_torque * self.torque_params.kf

        if self.nn_friction_override:
          pid_log.error += get_friction(friction_input, lateral_accel_deadzone, FRICTION_THRESHOLD, self.torque_params)

        self.pid.set_limits(self.steer_max, -self.steer_max)
        freeze_integrator = steer_limited_by_safety or CS.steeringPressed or CS.vEgo < 5
        output_torque = self.pid.update(pid_log.error, speed=CS.vEgo, feedforward=scaled_nn_torque, freeze_integrator=freeze_integrator)

        pid_log.active = True
        pid_log.p, pid_log.i, pid_log.d, pid_log.f = float(self.pid.p), float(self.pid.i), float(self.pid.d), float(self.pid.f)
        pid_log.output = float(-output_torque)
        pid_log.actualLateralAccel = float(actual_lateral_accel)
        pid_log.desiredLateralAccel = float(desired_lateral_accel)
        pid_log.saturated = bool(self._check_saturation(self.steer_max - abs(output_torque) < 1e-3, CS, steer_limited_by_safety, curvature_limited))

    else:
      self.pid = self.pid_official

      future_desired_lateral_accel = desired_curvature * CS.vEgo ** 2
      self.lat_accel_request_buffer.append(future_desired_lateral_accel)

      delay_frames = int(np.clip(lat_delay / self.dt + 1, 1, self.lat_accel_request_buffer_len))
      setpoint = self.lat_accel_request_buffer[-delay_frames]
      error = setpoint - actual_lateral_accel

      lookahead_idx = int(np.clip(-delay_frames + self.lookahead_frames, -self.lat_accel_request_buffer_len+1, -2))
      raw_lateral_jerk = (self.lat_accel_request_buffer[lookahead_idx+1] - self.lat_accel_request_buffer[lookahead_idx-1]) / (2 * self.dt)
      desired_lateral_jerk = self.jerk_filter.update(raw_lateral_jerk)

      ff = (future_desired_lateral_accel - roll_compensation) - self.torque_params.latAccelOffset
      ff += get_friction(error + JERK_GAIN * desired_lateral_jerk, lateral_accel_deadzone, FRICTION_THRESHOLD, self.torque_params)

      if not active:
        output_torque = 0.0
        pid_log.active = False
      else:
        pid_log.error = float(error)
        freeze_integrator = steer_limited_by_safety or CS.steeringPressed or CS.vEgo < 5
        output_lataccel = self.pid.update(pid_log.error, speed=CS.vEgo, feedforward=ff, freeze_integrator=freeze_integrator)
        output_torque = self.torque_from_lateral_accel(output_lataccel, self.torque_params)

        pid_log.active = True
        pid_log.p, pid_log.i, pid_log.d, pid_log.f = float(self.pid.p), float(self.pid.i), float(self.pid.d), float(self.pid.f)
        pid_log.output = float(-output_torque)
        pid_log.actualLateralAccel = float(actual_lateral_accel)
        pid_log.desiredLateralAccel = float(setpoint)
        pid_log.desiredLateralJerk = float(desired_lateral_jerk)
        pid_log.saturated = bool(self._check_saturation(self.steer_max - abs(output_torque) < 1e-3, CS, steer_limited_by_safety, curvature_limited))

    # TODO left is positive in this convention
    return -output_torque, 0.0, pid_log
