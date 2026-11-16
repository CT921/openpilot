import numpy as np
from cereal import car
from openpilot.common.realtime import DT_CTRL
from openpilot.selfdrive.controls.lib.drive_helpers import CONTROL_N
from openpilot.common.pid import PIDController
from openpilot.selfdrive.modeld.constants import ModelConstants
from openpilot.common.params import Params
from openpilot.selfdrive.controls.lib.smooth_stops import SmoothStopController, PARAMS_UPDATE_PERIOD

CONTROL_N_T_IDX = ModelConstants.T_IDXS[:CONTROL_N]

LongCtrlState = car.CarControl.Actuators.LongControlState


def long_control_state_trans(CP, active, long_control_state, v_ego,
                             should_stop, brake_pressed, cruise_standstill):
  stopping_condition = should_stop
  starting_condition = (not should_stop and
                        not cruise_standstill and
                        not brake_pressed)
  started_condition = v_ego > CP.vEgoStarting

  if not active:
    long_control_state = LongCtrlState.off

  else:
    if long_control_state == LongCtrlState.off:
      if not starting_condition:
        long_control_state = LongCtrlState.stopping
      else:
        if starting_condition and CP.startingState:
          long_control_state = LongCtrlState.starting
        else:
          long_control_state = LongCtrlState.pid

    elif long_control_state == LongCtrlState.stopping:
      if starting_condition and CP.startingState:
        long_control_state = LongCtrlState.starting
      elif starting_condition:
        long_control_state = LongCtrlState.pid

    elif long_control_state in [LongCtrlState.starting, LongCtrlState.pid]:
      if stopping_condition:
        long_control_state = LongCtrlState.stopping
      elif started_condition:
        long_control_state = LongCtrlState.pid
  return long_control_state

def apply_deadzone(error, deadzone):
  if error > deadzone:
    error -= deadzone
  elif error < - deadzone:
    error += deadzone
  else:
    error = 0.
  return error

class LongControl:
  def __init__(self, CP):
    self.CP = CP
    self.long_control_state = LongCtrlState.off
    self.pid = PIDController((CP.longitudinalTuning.kpBP, CP.longitudinalTuning.kpV),
                             (CP.longitudinalTuning.kiBP, CP.longitudinalTuning.kiV),
                             rate=1 / DT_CTRL)
    self.last_output_accel = 0.0

    # TOP: poll params instead of reading them twice per 100 Hz frame
    self.params = Params()
    self.frame = 0
    self.use_deadzone = self._read_deadzone_params()
    self.smooth = SmoothStopController(self.params)

  def _read_deadzone_params(self) -> bool:
    return bool(self.params.get_bool("ExperimentalMode") or self.params.get_bool("ToyotaTune"))

  def reset(self):
    self.pid.reset()

  def update(self, active, CS, a_target, should_stop, accel_limits, lead_distance=0.0, has_lead=False, lead_speed=0.0):
    """Update longitudinal control. This updates the state machine and runs a PID loop"""
    self.pid.neg_limit = accel_limits[0]
    self.pid.pos_limit = accel_limits[1]

    if self.frame % int(PARAMS_UPDATE_PERIOD / DT_CTRL) == 0:
      self.use_deadzone = self._read_deadzone_params()
    self.frame += 1

    # Smooth Stops toggle flips only take effect between stops (never mid-settle / mid-hold)
    self.smooth.update_params(can_apply=(self.long_control_state != LongCtrlState.stopping and not self.smooth.settling))

    # Smooth Stops owns the final approach: while the plan wants to stop but the car is
    # still rolling, defer the hold clamp and settle in the pid branch below. The clamp
    # (stopping state) only arms once we're actually stopped, so it never headbangs.
    # starting / off are exempt and take the stock edges into stopping.
    # Toggle off = fully stock: raw should_stop everywhere, no settle.
    if self.smooth.enabled and active and self.long_control_state == LongCtrlState.pid:
      stop_now = self.smooth.want_hold(should_stop, CS.vEgo, CS.standstill)
    elif self.smooth.enabled and active and self.long_control_state == LongCtrlState.stopping:
      # debounced hold exit: a one-frame should_stop flicker must not blip the brake at standstill
      stop_now = not self.smooth.hold_release(should_stop)
    else:
      stop_now = should_stop

    prev_state = self.long_control_state
    self.long_control_state = long_control_state_trans(self.CP, active, self.long_control_state, CS.vEgo,
                                                       stop_now, CS.brakePressed,
                                                       CS.cruiseState.standstill)
    if self.long_control_state == LongCtrlState.stopping and prev_state != LongCtrlState.stopping:
      # every entry into the hold gets a fresh release debounce (incl. stock off/starting edges)
      self.smooth.arm_hold()

    if self.long_control_state == LongCtrlState.off:
      self.reset()
      self.smooth.reset()
      output_accel = 0.

    elif self.long_control_state == LongCtrlState.stopping:
      output_accel = self.last_output_accel
      if output_accel > self.CP.stopAccel:
        output_accel = min(output_accel, 0.0)
        output_accel -= self.CP.stoppingDecelRate * DT_CTRL
      self.reset()
      self.smooth.reset()

    elif self.long_control_state == LongCtrlState.starting:
      output_accel = self.CP.startAccel
      self.reset()
      self.smooth.reset()

    else:  # LongCtrlState.pid
      if self.smooth.enabled and active and self.smooth.want_settle(should_stop):
        # SETTLE: feather to a true standstill instead of clamping while still rolling.
        # Open-loop accel command (like stopping/starting), so keep the PID reset.
        output_accel = self.smooth.settle(a_target, CS.vEgo, lead_distance, has_lead, self.last_output_accel, lead_speed)
        self.reset()
      else:
        error = a_target - CS.aEgo
        # TOP apply deadzone to experimental mode
        if self.use_deadzone:
          error = apply_deadzone(error, np.interp(CS.vEgo, [0., 6., 7., 20., 30.], [0., .001, .003, .1, .15]))
        output_accel = self.pid.update(error, speed=CS.vEgo,
                                       feedforward=a_target)
        self.smooth.reset()

    self.last_output_accel = float(np.clip(output_accel, accel_limits[0], accel_limits[1]))
    return self.last_output_accel
