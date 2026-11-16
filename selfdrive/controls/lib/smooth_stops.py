"""
Smooth Stops -- ported to TOP from sunnypilot (MIT License).
Original concept and implementation by SpysyWeeb (github.com/SpysyWeeb).

The smooth landing is produced entirely in longcontrol, at control rate, by feathering
the brake all the way down to a *true* standstill and only then handing off to the
standstill hold clamp. Gated behind the SmoothStops toggle: disabled means stock
longcontrol behavior (and stock radard lead selection).

TOP port fixes vs. upstream:
  1. Settle exit debounce: a one-frame should_stop flicker while still rolling used to
     drop to the raw PID for a frame (unfeathered jump) and reset the entry-anchored
     taper + anti-creep latch. Settle now needs SETTLE_RELEASE_FRAMES of should_stop=False
     to exit.
  2. Toggle changes are deferred until the car is between stops (never mid-settle or
     mid-hold), so flipping the toggle can't abruptly switch control laws.
"""
from opendbc.car.interfaces import ACCEL_MIN
from openpilot.common.params import Params
from openpilot.common.realtime import DT_CTRL

PARAMS_UPDATE_PERIOD = 1.0     # s, how often the toggle is re-read

# --- handoff to the hold clamp ---
STANDSTILL_SPEED = 0.05        # m/s, arm the stopping/hold clamp once the car is essentially stopped
STANDSTILL_HOLD_SPEED = 0.15   # m/s, ceiling for trusting CS.standstill -- never arm the hold above this

# --- the settle feather: a controlled deceleration to a true stop ---
STOP_KISS_DECEL = 0.12    # m/s^2, residual decel right at the stop
MIN_ENTRY_SPEED = 0.1     # m/s, floor on the latched entry speed (guards the taper division)
STOP_GAP_MARGIN = 2.5     # m, settle brakes to stop at least this far behind the lead
MIN_GAP_BUDGET = 0.15     # m, floor on the gap budget -- only guards the division
PROGRESS_EPS = 0.02       # m/s, speed drop (vs the running minimum) that counts as "still slowing"
ANTI_CREEP_RATE = 0.50    # m/s^2 added per second the car is NOT slowing (kills creep / held creep)
LEAD_CREEP_SPEED = 0.3    # m/s, a lead moving faster than this sets the pace -- ratchet stands down
URGENT_GAP = 2.0          # m, inside this gap budget the ratchet escalates faster
URGENT_RATE_MULT = 4.0    # ratchet-rate multiplier at gap -> 0, ramped in linearly from 1x at URGENT_GAP
SETTLE_JERK = 2.5         # m/s^3, smoothness of the settle pressure itself (plan braking is never feathered)
HOLD_RELEASE_FRAMES = 10  # frames of should_stop=False required to release the hold (0.1 s)
SETTLE_RELEASE_FRAMES = 5 # frames of should_stop=False required to leave settle while rolling (0.05 s)


def read_smooth_stops_params(params: Params) -> bool:
  return bool(params.get_bool("SmoothStops"))


class SmoothStopController:
  """
  Owns the final approach to a stop, inside longcontrol (control rate, true v_ego).

  Stock: the moment should_stop goes true the state machine jumps to `stopping` and ramps
  the command toward stopAccel while the car is usually still rolling -> headbang.
  Here the car stays in a closed-loop settle feather until it's actually stopped:

    want_settle()  -- debounced "keep settling" (should_stop with exit hysteresis).
    settle()       -- entry-anchored taper down to the kiss, lead-aware floor, latched
                      anti-creep ratchet. Never softer than the plan's a_target.
    want_hold()    -- request the hold clamp only once v_ego <= STANDSTILL_SPEED.
    arm_hold()     -- fresh release debounce on every entry into `stopping`.
    hold_release() -- debounced hold exit.
  """
  def __init__(self, params: Params | None = None):
    self.params = params if params is not None else Params()
    self.frame = 0
    self.enabled = read_smooth_stops_params(self.params)
    self._requested = self.enabled
    self._no_stop_frames = 0
    self._settling = False
    self._no_settle_frames = 0
    self.reset()

  @property
  def settling(self) -> bool:
    return self._settling

  def update_params(self, can_apply: bool = True) -> None:
    if self.frame % int(PARAMS_UPDATE_PERIOD / DT_CTRL) == 0:
      self._requested = read_smooth_stops_params(self.params)
    self.frame += 1
    # only switch control laws between stops (fix #2)
    if can_apply and self.enabled != self._requested:
      self.enabled = self._requested
      self.reset()

  def reset(self) -> None:
    self._v_min = float("inf")
    self._stall_s = 0.0
    self._creep_decel = 0.0
    self._entry_v = 0.0
    self._entry_decel = 0.0
    self._settling = False
    self._no_settle_frames = 0

  def want_settle(self, should_stop: bool) -> bool:
    # fix #1: debounce the settle exit while rolling
    if should_stop:
      self._settling = True
      self._no_settle_frames = 0
    elif self._settling:
      self._no_settle_frames += 1
      if self._no_settle_frames >= SETTLE_RELEASE_FRAMES:
        self._settling = False
    return self._settling

  def want_hold(self, should_stop: bool, v_ego: float, standstill: bool) -> bool:
    return bool(should_stop and (v_ego <= STANDSTILL_SPEED or (standstill and v_ego <= STANDSTILL_HOLD_SPEED)))

  def arm_hold(self) -> None:
    self._no_stop_frames = 0

  def hold_release(self, should_stop: bool) -> bool:
    # NOT cleared by reset(): reset() runs every frame while holding
    if should_stop:
      self._no_stop_frames = 0
    else:
      self._no_stop_frames += 1
    return self._no_stop_frames >= HOLD_RELEASE_FRAMES

  def settle(self, a_target: float, v_ego: float, lead_distance: float, has_lead: bool, last_output: float,
             lead_speed: float = 0.0) -> float:
    # entry-anchored taper: fade the braking level we entered with down to the kiss as v -> 0
    if self._entry_v <= 0.0:
      self._entry_v = max(v_ego, MIN_ENTRY_SPEED)
      self._entry_decel = max(-last_output, STOP_KISS_DECEL)
    landing = STOP_KISS_DECEL + (self._entry_decel - STOP_KISS_DECEL) * min(v_ego / self._entry_v, 1.0)
    a_settle = -landing

    # lead-aware floor: brake hard enough to stop STOP_GAP_MARGIN short of the lead
    creep_rate = ANTI_CREEP_RATE
    if has_lead and lead_distance > 0.0:
      gap = max(lead_distance - STOP_GAP_MARGIN, MIN_GAP_BUDGET)
      a_settle = min(a_settle, -(v_ego * v_ego) / (2.0 * gap))
      urgency = max(0.0, min(1.0, 1.0 - gap / URGENT_GAP))
      creep_rate = ANTI_CREEP_RATE * (1.0 + urgency * (URGENT_RATE_MULT - 1.0))

    # anti-creep: latched ratchet while the car is NOT slowing. Latched (never released until
  # reset()) -- releasing on each sliver of progress limit-cycles against creep torque.
    if v_ego < self._v_min - PROGRESS_EPS:
      self._v_min = v_ego
      self._stall_s = 0.0
    elif not (has_lead and lead_speed > LEAD_CREEP_SPEED):
      self._stall_s += DT_CTRL
    self._creep_decel = max(self._creep_decel, creep_rate * self._stall_s)
    a_settle -= self._creep_decel

    a_settle = max(a_settle, ACCEL_MIN)

    # feather only the settle pressure
    step = SETTLE_JERK * DT_CTRL
    a_settle = min(max(a_settle, last_output - step), last_output + step)

    # never brake softer than the plan -- it owns collision avoidance
    return min(a_settle, a_target)
