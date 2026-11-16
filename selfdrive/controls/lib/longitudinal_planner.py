#!/usr/bin/env python3
import math
import numpy as np
import cereal.messaging as messaging
from openpilot.common.params import Params
from opendbc.car.interfaces import ACCEL_MIN, ACCEL_MAX
from openpilot.common.constants import CV
from openpilot.common.filter_simple import FirstOrderFilter
from openpilot.common.realtime import DT_MDL
from openpilot.selfdrive.modeld.constants import ModelConstants
from openpilot.selfdrive.controls.lib.longcontrol import LongCtrlState
from openpilot.selfdrive.controls.lib.longitudinal_mpc_lib.long_mpc import LongitudinalMpc, LongitudinalPlanSource
from openpilot.selfdrive.controls.lib.longitudinal_mpc_lib.long_mpc import T_IDXS as T_IDXS_MPC
from openpilot.selfdrive.controls.lib.drive_helpers import CONTROL_N, get_accel_from_plan
from openpilot.selfdrive.car.cruise import V_CRUISE_MAX, V_CRUISE_UNSET
from openpilot.common.swaglog import cloudlog
from openpilot.top.selfdrive.controls.lib.longitudinal_planner import LongitudinalPlannerTOP
from openpilot.top.selfdrive.controls.lib.longitudinal_planner import LongitudinalPlanSource as TopLongitudinalPlanSource

A_CRUISE_MAX_VALS = [1.6, 1.2, 0.8, 0.6]
A_CRUISE_MAX_BP = [0., 10.0, 25., 40.]
# A_CRUISE_MAX_VALS_TOYOTA = [2.0, 1.7, 1.3,  .95,  .75, .70, .65, .45, .32, .20, .085]  # Sets the limits of the planner accel, PID may exceed
A_CRUISE_MAX_VALS_TOYOTA =   [2.0, 1.9, 1.7, 1.42, .95, .83, .61, .54, .45, .38, .15]  # Sets the limits of the planner accel, PID may exceed
# CRUISE_MAX_BP in kmh =     [0.,  3,   10,  20,    30,  40,  53,  72,  90,  107, 150]
A_CRUISE_MAX_BP_TOYOTA =     [0.,  1,   3.,  6.,    8.,  11., 16., 20., 25., 30., 55.]
CONTROL_N_T_IDX = ModelConstants.T_IDXS[:CONTROL_N]
ALLOW_THROTTLE_THRESHOLD = 0.4
MIN_ALLOW_THROTTLE_SPEED = 2.5

# Lookup table for turns
_A_TOTAL_MAX_V = [1.7, 3.2]
_A_TOTAL_MAX_BP = [20., 40.]

RED_LIGHT_DISTANCE_THRESHOLD = 25.0
GREEN_LIGHT_DISTANCE_THRESHOLD = 35.0
FILTER_TIME_CONSTANT = 0.5
FILTER_THRESHOLD = 0.6

def get_max_accel(v_ego):
  return np.interp(v_ego, A_CRUISE_MAX_BP, A_CRUISE_MAX_VALS)

def get_max_accel_toyota(v_ego):
  return np.interp(v_ego, A_CRUISE_MAX_BP_TOYOTA, A_CRUISE_MAX_VALS_TOYOTA)

def get_coast_accel(pitch):
  return np.sin(pitch) * -5.65 - 0.3  # fitted from data using xx/projects/allow_throttle/compute_coast_accel.py

def limit_accel_in_turns(v_ego, angle_steers, a_target, CP):
  """
  This function returns a limited long acceleration allowed, depending on the existing lateral acceleration
  this should avoid accelerating when losing the target in turns
  """
  # FIXME: This function to calculate lateral accel is incorrect and should use the VehicleModel
  # The lookup table for turns should also be updated if we do this
  a_total_max = np.interp(v_ego, _A_TOTAL_MAX_BP, _A_TOTAL_MAX_V)
  a_y = v_ego ** 2 * angle_steers * CV.DEG_TO_RAD / (CP.steerRatio * CP.wheelbase)
  a_x_allowed = math.sqrt(max(a_total_max ** 2 - a_y ** 2, 0.))

  return [a_target[0], min(a_target[1], a_x_allowed)]


class LongitudinalPlanner(LongitudinalPlannerTOP):
  def __init__(self, CP, init_v=0.0, init_a=0.0, dt=DT_MDL):
    self.CP = CP
    self.mpc = LongitudinalMpc(CP, dt=dt)
    LongitudinalPlannerTOP.__init__(self, self.CP)
    self.fcw = False
    self.dt = dt
    self.allow_throttle = True

    self.a_desired = init_a
    self.v_desired_filter = FirstOrderFilter(init_v, 2.0, self.dt)
    self.prev_accel_clip = [ACCEL_MIN, ACCEL_MAX]
    self.output_a_target = 0.0
    self.output_should_stop = False

    self.v_desired_trajectory = np.zeros(CONTROL_N)
    self.a_desired_trajectory = np.zeros(CONTROL_N)
    self.j_desired_trajectory = np.zeros(CONTROL_N)
    self.params = Params()
    self.dynamic_follow = False
    self.dynamic_follow = self.params.get_bool("Dynamic_Follow")

    # StandstillMode
    self.sng_e2e = self.params.get_bool("sng_e2e")
    if self.sng_e2e:
      self.standstill_prev = False
      self.standstill_current = False
      self.standstill_transit_counter = 0
      self.STANDSTILL_TRANSIT_FRAMES = 10
      self.experimental_mode_active_by_standstill = False
      self.experimental_mode_active_by_traffic_light = False
      self.LEAD_DISTANCE_THRESHOLD = 5.0
      self.LEAD_SPEED_THRESHOLD = 3.0 * CV.KPH_TO_MS

      # Red light detection
      self.red_light_filter = FirstOrderFilter(0, FILTER_TIME_CONSTANT, self.dt)
      self.red_light_detected = False
      self.green_light_detected = False
      self.model_length = 0
      self.tracking_lead = False
      self.lead_distance = float('inf')
      self.lead_moving = False
      self.lead_velocity = 0.0
      self.update_counter = 0
      self.consecutive_red_detections = 0
      self.consecutive_green_detections = 0
      self.CONSECUTIVE_THRESHOLD = 3

  def detect_traffic_light(self, sm, v_ego):
    model_x = sm['modelV2'].position.x
    if len(model_x) == 0:
      return self.red_light_detected

    self.model_length = model_x[-1]

    # Get lead vehicle info
    lead_one = sm['radarState'].leadOne
    lead_two = sm['radarState'].leadTwo
    has_lead_primary = lead_one.status
    has_lead_secondary = lead_two.status
    self.tracking_lead = has_lead_primary or has_lead_secondary
    active_lead = lead_one if has_lead_primary else (lead_two if has_lead_secondary else None)

    if active_lead is not None:
      self.lead_distance = active_lead.dRel
      self.lead_moving = active_lead.vRel > -0.5  # Consider vehicle moving if relative velocity > -0.5 m/s
      self.lead_velocity = active_lead.vLead
    else:
      self.lead_distance = float('inf')
      self.lead_moving = False
      self.lead_velocity = 0.0

    is_standstill = sm['carState'].standstill
    gas_pressed = sm['carState'].gasPressed

    prev_red_light_detected = self.red_light_detected

    consider_traffic_light = not self.tracking_lead or self.lead_distance > self.model_length

    if consider_traffic_light and is_standstill and not gas_pressed:
      # Red light detection: short model path suggests stopped traffic/red light
      if self.model_length < RED_LIGHT_DISTANCE_THRESHOLD:
        self.consecutive_red_detections += 1
        self.consecutive_green_detections = 0
      # Green light detection: long model path suggests clear road/green light
      elif self.model_length > GREEN_LIGHT_DISTANCE_THRESHOLD:
        self.consecutive_green_detections += 1
        self.consecutive_red_detections = 0
      else:
        # In between - maintain current state but reduce confidence
        self.consecutive_red_detections = max(0, self.consecutive_red_detections - 1)
        self.consecutive_green_detections = max(0, self.consecutive_green_detections - 1)
    else:
      # Not in standstill or other conditions not met - reset
      self.consecutive_red_detections = 0
      self.consecutive_green_detections = 0
      self.red_light_detected = False
      self.green_light_detected = False
      self.red_light_filter.x = 0
      return False

    # Update states based on consecutive detections
    if self.consecutive_red_detections >= self.CONSECUTIVE_THRESHOLD:
      self.red_light_detected = True
      self.green_light_detected = False
      self.red_light_filter.update(1.0)
    elif self.consecutive_green_detections >= self.CONSECUTIVE_THRESHOLD:
      self.red_light_detected = False
      self.green_light_detected = True
      self.red_light_filter.update(0.0)

    # Apply filter threshold for final decision
    if self.red_light_filter.x >= FILTER_THRESHOLD and not self.green_light_detected:
      self.red_light_detected = True
    elif self.red_light_filter.x < (FILTER_THRESHOLD * 0.5):  # Hysteresis
      self.red_light_detected = False

    # Logging for state changes
    if self.red_light_detected != prev_red_light_detected:
      light_status = "RED LIGHT" if self.red_light_detected else "GREEN LIGHT"
      print(f"Traffic light status changed: {light_status}, filter: {self.red_light_filter.x:.2f}, model length: {self.model_length:.2f}m, consecutive R/G: {self.consecutive_red_detections}/{self.consecutive_green_detections}")
    # Periodic status logging
    self.update_counter += 1
    if self.update_counter >= 20:  # Every 1 second at 20Hz
      light_status = "RED" if self.red_light_detected else ("GREEN" if self.green_light_detected else "UNKNOWN")
      lead_info = f"(dist: {self.lead_distance:.1f}m, v: {self.lead_velocity:.1f}m/s)" if self.tracking_lead else "(no lead)"
      print(f"Traffic status: {light_status}, filter: {self.red_light_filter.x:.2f}, model: {self.model_length:.2f}m, lead: {lead_info}, consecutive R/G: {self.consecutive_red_detections}/{self.consecutive_green_detections}")
      self.update_counter = 0

    return self.red_light_detected

  @staticmethod
  def parse_model(model_msg):
    if (len(model_msg.position.x) == ModelConstants.IDX_N and
      len(model_msg.velocity.x) == ModelConstants.IDX_N and
      len(model_msg.acceleration.x) == ModelConstants.IDX_N):
      x = np.interp(T_IDXS_MPC, ModelConstants.T_IDXS, model_msg.position.x)
      v = np.interp(T_IDXS_MPC, ModelConstants.T_IDXS, model_msg.velocity.x)
      a = np.interp(T_IDXS_MPC, ModelConstants.T_IDXS, model_msg.acceleration.x)
      j = np.zeros(len(T_IDXS_MPC))
    else:
      x = np.zeros(len(T_IDXS_MPC))
      v = np.zeros(len(T_IDXS_MPC))
      a = np.zeros(len(T_IDXS_MPC))
      j = np.zeros(len(T_IDXS_MPC))
    if len(model_msg.meta.disengagePredictions.gasPressProbs) > 1:
      throttle_prob = model_msg.meta.disengagePredictions.gasPressProbs[1]
    else:
      throttle_prob = 1.0
    return x, v, a, j, throttle_prob

  def update(self, sm):
    # standstill e2e
    v_ego = sm['carState'].vEgo
    red_light_detected = self.detect_traffic_light(sm, v_ego) if self.sng_e2e else False

    if self.sng_e2e:
      self.standstill_current = sm['carState'].standstill

      lead_one = sm['radarState'].leadOne
      has_lead = lead_one.status
      lead_dist = lead_one.dRel if has_lead else float('inf')
      lead_speed = lead_one.vRel + sm['carState'].vEgo if has_lead else 0.0
      lead_moving_away = has_lead and lead_dist > self.LEAD_DISTANCE_THRESHOLD and lead_speed > self.LEAD_SPEED_THRESHOLD

      # Enhanced exit conditions
      if lead_moving_away and self.experimental_mode_active_by_standstill:
        self.params.put_bool_nonblocking("ExperimentalMode", False)
        self.experimental_mode_active_by_standstill = False
        print(f"Lead vehicle moving away: dist={lead_dist:.1f}m, speed={lead_speed*3.6:.1f}km/h, disabling ExperimentalMode")

      # Use green light detection for more reliable exit
      elif (self.green_light_detected or not red_light_detected) and self.experimental_mode_active_by_traffic_light:
        self.params.put_bool_nonblocking("ExperimentalMode", False)
        self.experimental_mode_active_by_traffic_light = False
        print("Traffic light turned green or red light no longer detected: disabling ExperimentalMode")

      if self.standstill_current != self.standstill_prev:
        self.standstill_transit_counter = self.STANDSTILL_TRANSIT_FRAMES
        print(f"Standstill state change: {self.standstill_prev} -> {self.standstill_current}")

      if self.standstill_transit_counter > 0:
        self.standstill_transit_counter -= 1

        if self.standstill_transit_counter == 0:
          if self.standstill_current:
            should_enable_experimental = False

            if has_lead:
              should_enable_experimental = True
              self.experimental_mode_active_by_standstill = True
              self.experimental_mode_active_by_traffic_light = False
              print("Entering standstill with lead vehicle: Enabling ExperimentalMode")
            elif red_light_detected:
              should_enable_experimental = True
              self.experimental_mode_active_by_standstill = False
              self.experimental_mode_active_by_traffic_light = True
              print("Entering standstill at red light: Enabling ExperimentalMode")

            if should_enable_experimental:
              self.params.put_bool_nonblocking("ExperimentalMode", True)
          else:
            if self.experimental_mode_active_by_standstill or self.experimental_mode_active_by_traffic_light:
              self.params.put_bool_nonblocking("ExperimentalMode", False)
              self.experimental_mode_active_by_standstill = False
              self.experimental_mode_active_by_traffic_light = False
              print("Leaving standstill: Disabling ExperimentalMode")

      self.standstill_prev = self.standstill_current

    if len(sm['carControl'].orientationNED) == 3:
      accel_coast = get_coast_accel(sm['carControl'].orientationNED[1])
      pitch_rad = sm['carControl'].orientationNED[1]
    else:
      accel_coast = ACCEL_MAX
      pitch_rad = 0.0

    v_ego = sm['carState'].vEgo
    v_cruise_kph = min(sm['carState'].vCruise, V_CRUISE_MAX)
    v_cruise = v_cruise_kph * CV.KPH_TO_MS
    v_cruise_initialized = sm['carState'].vCruise != V_CRUISE_UNSET

    long_control_off = sm['controlsState'].longControlState == LongCtrlState.off
    force_slow_decel = sm['controlsState'].forceDecel

    # Reset current state when not engaged, or user is controlling the speed
    reset_state = long_control_off if self.CP.openpilotLongitudinalControl else not sm['selfdriveState'].enabled
    # PCM cruise speed may be updated a few cycles later, check if initialized
    reset_state = reset_state or not v_cruise_initialized

    # No change cost when user is controlling the speed, or when standstill
    prev_accel_constraint = not (reset_state or sm['carState'].standstill)

    if self.CP.brand == "toyota":
      accel_clip = [ACCEL_MIN, get_max_accel_toyota(v_ego)]
    else:
      accel_clip = [ACCEL_MIN, get_max_accel(v_ego)]
    steer_angle_without_offset = sm['carState'].steeringAngleDeg - sm['liveParameters'].angleOffsetDeg
    accel_clip = limit_accel_in_turns(v_ego, steer_angle_without_offset, accel_clip, self.CP)

    if self.accel_controller.is_personality_enabled():
      max_limit = self.accel_controller._get_max_accel_for_speed(v_ego)
      # Use the accel controller limits directly
      accel_clip = [ACCEL_MIN, max_limit]
      # Recalculate limit turn according to the new max limit
      steer_angle_without_offset = sm['carState'].steeringAngleDeg - sm['liveParameters'].angleOffsetDeg
      accel_clip = limit_accel_in_turns(v_ego, steer_angle_without_offset, accel_clip, self.CP)

    if reset_state:
      self.v_desired_filter.x = v_ego
      # Clip aEgo to cruise limits to prevent large accelerations when becoming active
      self.a_desired = np.clip(sm['carState'].aEgo, accel_clip[0], accel_clip[1])

    # Prevent divergence, smooth in current v_ego
    self.v_desired_filter.x = max(0.0, self.v_desired_filter.update(v_ego))
    _, _, _, _, throttle_prob = self.parse_model(sm['modelV2'])
    # Don't clip at low speeds since throttle_prob doesn't account for creep
    self.allow_throttle = throttle_prob > ALLOW_THROTTLE_THRESHOLD or v_ego <= MIN_ALLOW_THROTTLE_SPEED

    if not self.allow_throttle:
      clipped_accel_coast = max(accel_coast, accel_clip[0])
      clipped_accel_coast_interp = np.interp(v_ego, [MIN_ALLOW_THROTTLE_SPEED, MIN_ALLOW_THROTTLE_SPEED*2], [accel_clip[1], clipped_accel_coast])
      accel_clip[1] = min(accel_clip[1], clipped_accel_coast_interp)

    # Get new v_cruise and a_desired from Smart Cruise Control and Speed Limit Assist.
    # LongitudinalPlannerTOP.update() handles: events.clear(), accel_controller.update(), and all target calculations.
    v_cruise, a_target_top = LongitudinalPlannerTOP.update(self, sm, self.v_desired_filter.x, self.a_desired, v_cruise)

    if self.source != TopLongitudinalPlanSource.cruise:
      self.a_desired = a_target_top

    if force_slow_decel:
      v_cruise = 0.0

    lead_xv_0 = self.mpc.process_lead(sm['radarState'].leadOne)
    lead_xv_1 = self.mpc.process_lead(sm['radarState'].leadTwo)
    v_lead0 = lead_xv_0[0,1]
    v_lead1 = lead_xv_1[0,1]
    self.mpc.set_weights(prev_accel_constraint, personality=sm['selfdriveState'].personality, v_lead0=v_lead0, v_lead1=v_lead1)
    self.mpc.set_cur_state(self.v_desired_filter.x, self.a_desired)
    self.mpc.update(sm['radarState'], v_cruise, personality=sm['selfdriveState'].personality, dynamic_follow=self.dynamic_follow, pitch_rad=pitch_rad)

    self.v_desired_trajectory = np.interp(CONTROL_N_T_IDX, T_IDXS_MPC, self.mpc.v_solution)
    self.a_desired_trajectory = np.interp(CONTROL_N_T_IDX, T_IDXS_MPC, self.mpc.a_solution)
    self.j_desired_trajectory = np.interp(CONTROL_N_T_IDX, T_IDXS_MPC[:-1], self.mpc.j_solution)

    # TODO counter is only needed because radar is glitchy, remove once radar is gone
    self.fcw = self.mpc.crash_cnt > 2 and not sm['carState'].standstill
    if self.fcw:
      cloudlog.info("FCW triggered")

    # Interpolate 0.05 seconds and save as starting point for next iteration
    a_prev = self.a_desired
    self.a_desired = float(np.interp(self.dt, CONTROL_N_T_IDX, self.a_desired_trajectory))
    self.v_desired_filter.x = self.v_desired_filter.x + self.dt * (self.a_desired + a_prev) / 2.0

    action_t = self.CP.longitudinalActuatorDelay + DT_MDL
    output_a_target_mpc, output_should_stop_mpc = get_accel_from_plan(self.v_desired_trajectory, self.a_desired_trajectory, CONTROL_N_T_IDX,
                                                                        action_t=action_t, vEgoStopping=self.CP.vEgoStopping)
    output_a_target_e2e = sm['modelV2'].action.desiredAcceleration
    output_should_stop_e2e = sm['modelV2'].action.shouldStop

    if sm['selfdriveState'].experimentalMode:
      output_a_target = min(output_a_target_e2e, output_a_target_mpc)
      self.output_should_stop = output_should_stop_e2e or output_should_stop_mpc
      if output_a_target < output_a_target_mpc:
        self.mpc.source = LongitudinalPlanSource.e2e
    else:
      output_a_target = output_a_target_mpc
      self.output_should_stop = output_should_stop_mpc

    for idx in range(2):
      accel_clip[idx] = np.clip(accel_clip[idx], self.prev_accel_clip[idx] - 0.05, self.prev_accel_clip[idx] + 0.05)
    self.output_a_target = np.clip(output_a_target, accel_clip[0], accel_clip[1])
    self.prev_accel_clip = accel_clip

  def publish(self, sm, pm):
    plan_send = messaging.new_message('longitudinalPlan')

    plan_send.valid = sm.all_checks(service_list=['carState', 'controlsState', 'selfdriveState', 'radarState'])

    longitudinalPlan = plan_send.longitudinalPlan
    longitudinalPlan.modelMonoTime = sm.logMonoTime['modelV2']
    longitudinalPlan.processingDelay = (plan_send.logMonoTime / 1e9) - sm.logMonoTime['modelV2']
    longitudinalPlan.solverExecutionTime = self.mpc.solve_time

    longitudinalPlan.speeds = self.v_desired_trajectory.tolist()
    longitudinalPlan.accels = self.a_desired_trajectory.tolist()
    longitudinalPlan.jerks = self.j_desired_trajectory.tolist()

    longitudinalPlan.hasLead = sm['radarState'].leadOne.status
    longitudinalPlan.longitudinalPlanSource = self.mpc.source
    longitudinalPlan.fcw = self.fcw

    longitudinalPlan.aTarget = float(self.output_a_target)
    longitudinalPlan.shouldStop = bool(self.output_should_stop)
    longitudinalPlan.allowBrake = True
    longitudinalPlan.allowThrottle = bool(self.allow_throttle)

    pm.send('longitudinalPlan', plan_send)
    self.publish_longitudinal_plan_top(sm, pm)
