import math
import numpy as np
from opendbc.car import Bus, make_tester_present_msg, rate_limit, structs, ACCELERATION_DUE_TO_GRAVITY, DT_CTRL
from opendbc.car.lateral import apply_meas_steer_torque_limits, apply_std_steer_angle_limits, common_fault_avoidance
from opendbc.car.can_definitions import CanData
from opendbc.car.carlog import carlog
from opendbc.car.common.filter_simple import FirstOrderFilter, HighPassFilter
from opendbc.car.common.pid import PIDController
from opendbc.car.secoc import add_mac, build_sync_mac
from opendbc.car.interfaces import CarControllerBase
from opendbc.car.toyota import toyotacan
from opendbc.car.toyota.values import CAR, STATIC_DSU_MSGS, NO_STOP_TIMER_CAR, TSS2_CAR, \
                                        CarControllerParams, ToyotaFlags, \
                                        UNSUPPORTED_DSU_CAR, RADAR_ACC_CAR
from opendbc.car.common.conversions import Conversions as CV
from opendbc.can import CANPacker

Ecu = structs.CarParams.Ecu
LongCtrlState = structs.CarControl.Actuators.LongControlState
SteerControlType = structs.CarParams.SteerControlType
VisualAlert = structs.CarControl.HUDControl.VisualAlert

# The up limit allows the brakes/gas to unwind quickly leaving a stop,
# the down limit roughly matches the rate of ACCEL_NET, reducing PCM compensation windup
ACCEL_WINDUP_LIMIT = 4.0 * DT_CTRL * 3  # m/s^2 / frame
ACCEL_WINDDOWN_LIMIT = -4.0 * DT_CTRL * 3  # m/s^2 / frame
ACCEL_PID_UNWIND = 0.03 * DT_CTRL * 3  # m/s^2 / frame

MAX_PITCH_COMPENSATION = 1.5  # m/s^2

# LKA limits
# EPS faults if you apply torque while the steering rate is above 100 deg/s for too long
MAX_STEER_RATE = 100  # deg/s
MAX_STEER_RATE_FRAMES = 18  # tx control frames needed before torque can be cut

# EPS allows user torque above threshold for 50 frames before permanently faulting
MAX_USER_TORQUE = 500


# PCM compensatory force calculation threshold interpolation values
COMPENSATORY_CALCULATION_THRESHOLD_V = [-0.2, -0.2, -0.05]  # m/s^2
COMPENSATORY_CALCULATION_THRESHOLD_BP = [0., 20., 32.]  # m/s

# resume, lead, and lane lines hysteresis
UI_HYSTERESIS_TIME = 1.  # seconds

GearShifter = structs.CarState.GearShifter
UNLOCK_CMD = b'\x40\x05\x30\x11\x00\x40\x00\x00'
LOCK_CMD = b'\x40\x05\x30\x11\x00\x80\x00\x00'
LOCK_AT_SPEED = 10 * CV.KPH_TO_MS

# Blindspot codes
LEFT_BLINDSPOT = b'\x41'
RIGHT_BLINDSPOT = b'\x42'


def set_blindspot_debug_mode(lr,enable):
  if enable:
    m = lr + b'\x02\x10\x60\x00\x00\x00\x00'
  else:
    m = lr + b'\x02\x10\x01\x00\x00\x00\x00'
  return CanData(0x750, m, 0)


def poll_blindspot_status(lr):
  m = lr + b'\x02\x21\x69\x00\x00\x00\x00'
  return CanData(0x750, m, 0)


def get_long_tune(CP, params):
  if CP.flags & ToyotaFlags.TSSP_TUNE.value:
    kiBP = [0., 5., 35.]
    kiV = [3.6, 2.4, 1.5]
    if CP.carFingerprint in TSS2_CAR:
      kiBP = [2., 5.]
      kiV = [0.5, 0.25]
  else:
    kiBP = [2.,  4.,   8.,  12.,  27.]
    kiV = [.45, .385, .32, .235,  .11]

  return PIDController(0.0, (kiBP, kiV), k_f=1.0,
                       pos_limit=params.ACCEL_MAX, neg_limit=params.ACCEL_MIN,
                       rate=1 / (DT_CTRL * 3))


class CarController(CarControllerBase):
  def __init__(self, dbc_names, CP):
    super().__init__(dbc_names, CP)
    self.params = CarControllerParams(self.CP)
    self.last_torque = 0
    self.last_angle = 0
    self.alert_active = False
    # self.last_standstill = False
    self.standstill_req = False
    self.permit_braking = True
    self.steer_rate_counter = 0
    self.distance_button = 0

    self._resume_false_frame = None
    self.lead = False
    self._standstill_req = False
    self.prohibit_neg_calculation = True

    # *** start long control state ***
    self.long_pid = get_long_tune(self.CP, self.params)
    self.aego = FirstOrderFilter(0.0, 0.25, DT_CTRL * 3)
    self.pitch = FirstOrderFilter(0, 0.5, DT_CTRL)
    self.pitch_hp = HighPassFilter(0.0, 0.25, 1.5, DT_CTRL)

    self.accel = 0
    self.prev_accel = 0
    # *** end long control state ***

    self.packer = CANPacker(dbc_names[Bus.pt])

    self.secoc_lka_message_counter = 0
    self.secoc_lta_message_counter = 0
    self.secoc_acc_message_counter = 0
    self.secoc_prev_reset_counter = 0

    self.toyotaautolock = self.CP.flags & ToyotaFlags.AUTO_LOCK.value
    self.toyotaautounlock = self.CP.flags & ToyotaFlags.AUTO_UNLOCK.value
    self.last_gear = GearShifter.park
    self.lock_once = False
    self.ToyotaTune = self.CP.flags & ToyotaFlags.TSSP_TUNE.value
    self._reverse_acc_change = self.CP.flags & ToyotaFlags.REVERSE_ACC_CHANGE.value
    self.topsng = self.CP.flags & ToyotaFlags.TSSP_SNG.value
    self.toyota_bsm = self.CP.flags & ToyotaFlags.BSM.value
    self.blindspot_debug_enabled_left = False
    self.blindspot_debug_enabled_right = False
    self.blindspot_frame = 0
    self.blindspot_rate = 20
    self.blindspot_always_on = True

  def update(self, CC, CS, now_nanos):
    actuators = CC.actuators
    stopping = actuators.longControlState == LongCtrlState.stopping
    hud_control = CC.hudControl
    pcm_cancel_cmd = CC.cruiseControl.cancel
    lat_active = CC.latActive and abs(CS.out.steeringTorque) < MAX_USER_TORQUE

    if len(CC.orientationNED) == 3:
      self.pitch.update(CC.orientationNED[1])
      self.pitch_hp.update(CC.orientationNED[1])

    # *** control msgs ***
    can_sends = []

    # dp - door auto lock / unlock logic
    # thanks to AlexandreSato & cydia2020
    # https://github.com/AlexandreSato/openpilot/blob/personal/doors.py
    if self.toyotaautolock or self.toyotaautounlock:
      gear = CS.out.gearShifter
      if self.last_gear != gear and gear == GearShifter.park:
        if self.toyotaautounlock:
          can_sends.append(CanData(0x750, UNLOCK_CMD, 0))
        if self.toyotaautolock:
          self.lock_once = False
      elif self.toyotaautolock and not CS.out.doorOpen and gear == GearShifter.drive and not self.lock_once and CS.out.vEgo >= LOCK_AT_SPEED:
        can_sends.append(CanData(0x750, LOCK_CMD, 0))
        self.lock_once = True
      self.last_gear = gear

    # Enable blindspot debug mode once (@arne182)
    # let's keep all the commented out code for easy debug purpose for future.
    if self.toyota_bsm:
      if self.frame > 200:
        # left bsm
        if not self.blindspot_debug_enabled_left:
          if (self.blindspot_always_on or CS.out.vEgo > 6): # eagle eye camera will stop working if right bsm is switched on under 6m/s
            can_sends.append(set_blindspot_debug_mode(LEFT_BLINDSPOT, True))
            self.blindspot_debug_enabled_left = True
            # print("bsm debug left, on")
        else:
          if not self.blindspot_always_on and self.frame - self.blindspot_frame > 50:
            can_sends.append(set_blindspot_debug_mode(LEFT_BLINDSPOT, False))
            self.blindspot_debug_enabled_left = False
            # print("bsm debug left, off")
          if self.frame % self.blindspot_rate == 0:
            can_sends.append(poll_blindspot_status(LEFT_BLINDSPOT))
            # if CS.out.leftBlinker:
            self.blindspot_frame = self.frame
            # print(self.blindspot_frame)
            # print("bsm poll left")
        # right bsm
        if not self.blindspot_debug_enabled_right:
          if (self.blindspot_always_on or CS.out.vEgo > 6): # eagle eye camera will stop working if right bsm is switched on under 6m/s
            can_sends.append(set_blindspot_debug_mode(RIGHT_BLINDSPOT, True))
            self.blindspot_debug_enabled_right = True
            # print("bsm debug right, on")
        else:
          if not self.blindspot_always_on and self.frame - self.blindspot_frame > 50:
            can_sends.append(set_blindspot_debug_mode(RIGHT_BLINDSPOT, False))
            self.blindspot_debug_enabled_right = False
            # print("bsm debug right, off")
          if self.frame % self.blindspot_rate == self.blindspot_rate/2:
            can_sends.append(poll_blindspot_status(RIGHT_BLINDSPOT))
            # if CS.out.rightBlinker:
            self.blindspot_frame = self.frame
            # print(self.blindspot_frame)
            # print("bsm poll right")

    # *** handle secoc reset counter increase ***
    if self.CP.flags & ToyotaFlags.SECOC.value:
      if CS.secoc_synchronization['RESET_CNT'] != self.secoc_prev_reset_counter:
        self.secoc_lka_message_counter = 0
        self.secoc_lta_message_counter = 0
        self.secoc_acc_message_counter = 0
        self.secoc_prev_reset_counter = CS.secoc_synchronization['RESET_CNT']

        expected_mac = build_sync_mac(self.secoc_key, int(CS.secoc_synchronization['TRIP_CNT']), int(CS.secoc_synchronization['RESET_CNT']))
        if int(CS.secoc_synchronization['AUTHENTICATOR']) != expected_mac:
          carlog.error("SecOC synchronization MAC mismatch, wrong key?")

    # *** steer torque ***
    new_torque = int(round(actuators.torque * self.params.STEER_MAX))
    apply_torque = apply_meas_steer_torque_limits(new_torque, self.last_torque, CS.out.steeringTorqueEps, self.params)

    # >100 degree/sec steering fault prevention
    self.steer_rate_counter, apply_steer_req = common_fault_avoidance(abs(CS.out.steeringRateDeg) >= MAX_STEER_RATE, lat_active,
                                                                      self.steer_rate_counter, MAX_STEER_RATE_FRAMES)

    if not lat_active:
      apply_torque = 0

    # *** steer angle ***
    if self.CP.steerControlType == SteerControlType.angle:
      # If using LTA control, disable LKA and set steering angle command
      apply_torque = 0
      apply_steer_req = False
      if self.frame % 2 == 0:
        # EPS uses the torque sensor angle to control with, offset to compensate
        apply_angle = actuators.steeringAngleDeg + CS.out.steeringAngleOffsetDeg

        # Angular rate limit based on speed
        self.last_angle = apply_std_steer_angle_limits(apply_angle, self.last_angle, CS.out.vEgoRaw,
                                                       CS.out.steeringAngleDeg + CS.out.steeringAngleOffsetDeg,
                                                       CC.latActive, self.params.ANGLE_LIMITS)

    self.last_torque = apply_torque

    # toyota can trace shows STEERING_LKA at 42Hz, with counter adding alternatively 1 and 2;
    # sending it at 100Hz seem to allow a higher rate limit, as the rate limit seems imposed
    # on consecutive messages
    steer_command = toyotacan.create_steer_command(self.packer, apply_torque, apply_steer_req)
    if self.CP.flags & ToyotaFlags.SECOC.value:
      # TODO: check if this slow and needs to be done by the CANPacker
      steer_command = add_mac(self.secoc_key,
                              int(CS.secoc_synchronization['TRIP_CNT']),
                              int(CS.secoc_synchronization['RESET_CNT']),
                              self.secoc_lka_message_counter,
                              steer_command)
      self.secoc_lka_message_counter += 1
    can_sends.append(steer_command)

    # STEERING_LTA does not seem to allow more rate by sending faster, and may wind up easier
    if self.frame % 2 == 0 and self.CP.carFingerprint in TSS2_CAR:
      lta_active = lat_active and self.CP.steerControlType == SteerControlType.angle
      # cut steering torque with TORQUE_WIND_DOWN when either EPS torque or driver torque is above
      # the threshold, to limit max lateral acceleration and for driver torque blending respectively.
      full_torque_condition = (abs(CS.out.steeringTorqueEps) < self.params.STEER_MAX and
                               abs(CS.out.steeringTorque) < self.params.MAX_LTA_DRIVER_TORQUE_ALLOWANCE)

      # TORQUE_WIND_DOWN at 0 ramps down torque at roughly the max down rate of 1500 units/sec
      torque_wind_down = 100 if lta_active and full_torque_condition else 0
      can_sends.append(toyotacan.create_lta_steer_command(self.packer, self.CP.steerControlType, self.last_angle,
                                                          lta_active, self.frame // 2, torque_wind_down))

      if self.CP.flags & ToyotaFlags.SECOC.value:
        lta_steer_2 = toyotacan.create_lta_steer_command_2(self.packer, self.frame // 2)
        lta_steer_2 = add_mac(self.secoc_key,
                              int(CS.secoc_synchronization['TRIP_CNT']),
                              int(CS.secoc_synchronization['RESET_CNT']),
                              self.secoc_lta_message_counter,
                              lta_steer_2)
        self.secoc_lta_message_counter += 1
        can_sends.append(lta_steer_2)

    # *** gas and brake ***

    # resume requested, clear pending delay and set standstill_req to low
    if CC.cruiseControl.resume:
        self._resume_false_frame = None
        self._standstill_req = False
    # resume not pressed
    else:
        # start delay timer if needed
        if self._resume_false_frame is None:
            self._resume_false_frame = self.frame

        # only set standstill_req after waiting 1 s
        if (self.frame - self._resume_false_frame) >= 3. / DT_CTRL:
          self._standstill_req = True
        else:
          self._standstill_req = False

    self.standstill_req = self._standstill_req and self.CP.carFingerprint not in NO_STOP_TIMER_CAR and not self.topsng

    # AleSato's Automatic Brake Hold
    if self.CP.flags & ToyotaFlags.AUTO_BRAKE_HOLD.value and self.CP.carFingerprint in (TSS2_CAR - RADAR_ACC_CAR) \
       and not (self.CP.flags & ToyotaFlags.SECOC.value) and (self.CP.flags & ToyotaFlags.HYBRID.value) and self.frame % 2 == 0:
      if CS.out.brakeholdGovernor:
        can_sends.append(toyotacan.create_brakehold_command(self.packer, {}, True if self.frame % 730 < 727 else False))
      else:
        can_sends.append(toyotacan.create_brakehold_command(self.packer, CS.stock_aeb, False))

    # handle UI messages
    fcw_alert = hud_control.visualAlert == VisualAlert.fcw
    steer_alert = hud_control.visualAlert in (VisualAlert.steerRequired, VisualAlert.ldw)
    # lead = hud_control.leadVisible or CS.out.vEgo < 12.  # at low speed we always assume the lead is present so ACC can be engaged

    # *** ui hysteresis ***
    if self.frame % (UI_HYSTERESIS_TIME / DT_CTRL) == 0:
      self.lead = hud_control.leadVisible
    reverse_acc = 2 if self._reverse_acc_change else 1

    if self.CP.openpilotLongitudinalControl:
      if self.frame % 3 == 0:
        # Press distance button until we are at the correct bar length. Only change while enabled to avoid skipping startup popup
        if self.frame % 6 == 0 and self.CP.openpilotLongitudinalControl:
          desired_distance = 4 - hud_control.leadDistanceBars
          if CS.out.cruiseState.enabled and CS.pcm_follow_distance != desired_distance:
            self.distance_button = not self.distance_button
          else:
            self.distance_button = 0

        if self.ToyotaTune:
          # Set thresholds for compensatory force calculations
          comp_thresh = np.interp(CS.out.vEgo, COMPENSATORY_CALCULATION_THRESHOLD_BP, COMPENSATORY_CALCULATION_THRESHOLD_V)
          if not CC.longActive:
            self.prohibit_neg_calculation = True
          if CS.pcm_accel_net > comp_thresh:
            self.prohibit_neg_calculation = False
          # Calculate acceleration offset only when allowed
          self.pcm_accel_compensation = CS.pcm_accel_net if CC.longActive and not self.prohibit_neg_calculation else 0.0
          # Compute PCM acceleration command only if long control is active
          pcm_accel_cmd = float(np.clip(actuators.accel + self.pcm_accel_compensation, self.params.ACCEL_MIN, self.params.ACCEL_MAX)) if CC.longActive and not \
             CS.out.cruiseState.standstill else 0.0
          self.permit_braking = CC.longActive
        else:
          # internal PCM gas command can get stuck unwinding from negative accel so we apply a generous rate limit
          pcm_accel_cmd = actuators.accel
          if CC.longActive:
            pcm_accel_cmd = rate_limit(pcm_accel_cmd, self.prev_accel, ACCEL_WINDDOWN_LIMIT, ACCEL_WINDUP_LIMIT)
          self.prev_accel = pcm_accel_cmd

          # calculate amount of acceleration PCM should apply to reach target, given pitch.
          # clipped to only include downhill angles, avoids erroneously unsetting PERMIT_BRAKING when stopping on uphills
          accel_due_to_pitch = math.sin(min(self.pitch.x, 0.0)) * ACCELERATION_DUE_TO_GRAVITY
          # TODO: on uphills this sometimes sets PERMIT_BRAKING low not considering the creep force
          net_acceleration_request = pcm_accel_cmd + accel_due_to_pitch

          if self.CP.carFingerprint in TSS2_CAR:
            # GVC does not overshoot ego acceleration when starting from stop, but still has a similar delay
            if not self.CP.flags & ToyotaFlags.SECOC.value:
              a_ego_blended = float(np.interp(CS.out.vEgo, [1.0, 2.0], [CS.gvc, CS.out.aEgo]))
            else:
              a_ego_blended = CS.out.aEgo

            # wind down integral when approaching target for step changes and smooth ramps to reduce overshoot
            prev_aego = self.aego.x
            self.aego.update(a_ego_blended)
            j_ego = (self.aego.x - prev_aego) / (DT_CTRL * 3)

            future_t = float(np.interp(CS.out.vEgo, [2., 5.], [0.25, 0.5]))
            a_ego_future = a_ego_blended + j_ego * future_t

            if CC.longActive:
              # constantly slowly unwind integral to recover from large temporary errors
              self.long_pid.i -= ACCEL_PID_UNWIND * float(np.sign(self.long_pid.i))

              error_future = pcm_accel_cmd - a_ego_future

              if not stopping:
                # Toyota's PCM slowly responds to changes in pitch. On change, we amplify our
                # acceleration request to compensate for the undershoot and following overshoot
                pitch_compensation = float(np.clip(math.sin(self.pitch_hp.x) * ACCELERATION_DUE_TO_GRAVITY,
                                                   -MAX_PITCH_COMPENSATION, MAX_PITCH_COMPENSATION))
                pcm_accel_cmd += pitch_compensation

              pcm_accel_cmd = self.long_pid.update(error_future,
                                                   speed=CS.out.vEgo,
                                                   feedforward=pcm_accel_cmd,
                                                   freeze_integrator=actuators.longControlState != LongCtrlState.pid)
            else:
              self.long_pid.reset()

          # Along with rate limiting positive jerk above, this greatly improves gas response time
          # Consider the net acceleration request that the PCM should be applying (pitch included)
          net_acceleration_request_min = min(actuators.accel + accel_due_to_pitch, net_acceleration_request)
          if net_acceleration_request_min < 0.2 or stopping or not CC.longActive:
            self.permit_braking = True
          elif net_acceleration_request_min > 0.3:
            self.permit_braking = False

          pcm_accel_cmd = float(np.clip(pcm_accel_cmd, self.params.ACCEL_MIN, self.params.ACCEL_MAX))

        if self.CP.flags & ToyotaFlags.AUTO_BRAKE_HOLD.value and self.CP.flags & ToyotaFlags.HYBRID.value:
          if CS.out.brakeholdGovernor and CS.out.standstill and stopping:
            pcm_accel_cmd = 0.0
            self.standstill_req = False

        main_accel_cmd = 0. if self.CP.flags & ToyotaFlags.SECOC.value else pcm_accel_cmd
        can_sends.append(toyotacan.create_accel_command(self.packer,  main_accel_cmd, actuators.accel, pcm_cancel_cmd, self.permit_braking, self.standstill_req,
                                                        self.lead or CS.out.vEgo < 12., CS.acc_type, fcw_alert, self.distance_button, reverse_acc))
        if self.CP.flags & ToyotaFlags.SECOC.value:
          acc_cmd_2 = toyotacan.create_accel_command_2(self.packer, pcm_accel_cmd)
          acc_cmd_2 = add_mac(self.secoc_key,
                              int(CS.secoc_synchronization['TRIP_CNT']),
                              int(CS.secoc_synchronization['RESET_CNT']),
                              self.secoc_acc_message_counter,
                              acc_cmd_2)
          self.secoc_acc_message_counter += 1
          can_sends.append(acc_cmd_2)

        self.accel = pcm_accel_cmd

    else:
      # we can spam can to cancel the system even if we are using lat only control
      if pcm_cancel_cmd:
        if self.CP.carFingerprint in UNSUPPORTED_DSU_CAR:
          can_sends.append(toyotacan.create_acc_cancel_command(self.packer))
        else:
          can_sends.append(toyotacan.create_accel_command(self.packer, 0, 0, pcm_cancel_cmd, True, False, self.lead or CS.out.vEgo < 12., CS.acc_type, False,
                                                          self.distance_button, reverse_acc))

    # *** hud ui ***
    if self.CP.carFingerprint != CAR.TOYOTA_PRIUS_V:
      # ui mesg is at 1Hz but we send asap if:
      # - there is something to display
      # - there is something to stop displaying
      send_ui = False
      if ((fcw_alert or steer_alert) and not self.alert_active) or \
         (not (fcw_alert or steer_alert) and self.alert_active):
        send_ui = True
        self.alert_active = not self.alert_active
      elif pcm_cancel_cmd:
        # forcing the pcm to disengage causes a bad fault sound so play a good sound instead
        send_ui = True

      if self.frame % 20 == 0 or send_ui:
        can_sends.append(toyotacan.create_ui_command(self.packer, steer_alert, pcm_cancel_cmd, hud_control.leftLaneVisible,
                                                     hud_control.rightLaneVisible, hud_control.leftLaneDepart,
                                                     hud_control.rightLaneDepart, lat_active, CS.lkas_hud))

      if not self.CP.flags & ToyotaFlags.RADAR_FILTER.value and (self.frame % 100 == 0 or send_ui) \
         and (self.CP.enableDsu or self.CP.flags & ToyotaFlags.DISABLE_RADAR.value):
        can_sends.append(toyotacan.create_fcw_command(self.packer, fcw_alert))

    # *** static msgs ***
    if self.CP.enableDsu:
      for addr, cars, bus, fr_step, vl in STATIC_DSU_MSGS:
        if self.frame % fr_step == 0 and self.CP.carFingerprint in cars:
          can_sends.append(CanData(addr, vl, bus))

    # keep radar disabled
    if not self.CP.flags & ToyotaFlags.RADAR_FILTER.value and self.frame % 20 == 0 and self.CP.flags & ToyotaFlags.DISABLE_RADAR.value:
      can_sends.append(make_tester_present_msg(0x750, 0, 0xF))

    new_actuators = actuators.as_builder()
    new_actuators.torque = apply_torque / self.params.STEER_MAX
    new_actuators.torqueOutputCan = apply_torque
    new_actuators.steeringAngleDeg = self.last_angle
    new_actuators.accel = self.accel

    self.frame += 1
    return new_actuators, can_sends
