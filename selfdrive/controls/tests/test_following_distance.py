import pytest
import itertools
from parameterized import parameterized_class

from cereal import log

from openpilot.common.params import Params
from openpilot.selfdrive.controls.lib.longitudinal_mpc_lib.long_mpc import get_safe_obstacle_distance, get_stopped_equivalence_factor, get_T_FOLLOW, get_STOP_DISTANCE
from openpilot.selfdrive.test.longitudinal_maneuvers.maneuver import Maneuver


def desired_follow_distance(v_ego, v_lead, t_follow=None, stop_distance=None):
  if t_follow is None:
    t_follow = get_T_FOLLOW()
  if stop_distance is None:
    stop_distance = get_STOP_DISTANCE()
  return get_safe_obstacle_distance(v_ego, t_follow, stop_distance) - get_stopped_equivalence_factor(v_lead, v_ego)

def run_following_distance_simulation(v_lead, t_end=100.0, e2e=False, personality=0):
  man = Maneuver(
    '',
    duration=t_end,
    initial_speed=float(v_lead),
    lead_relevancy=True,
    initial_distance_lead=100,
    speed_lead_values=[v_lead],
    breakpoints=[0.],
    e2e=e2e,
    personality=personality,
  )
  valid, output = man.evaluate()
  assert valid
  return output[-1,2] - output[-1,1]


@parameterized_class(("e2e", "personality", "speed"), itertools.product(
                      [True, False], # e2e
                      [log.LongitudinalPersonality.relaxed, # personality
                       log.LongitudinalPersonality.standard,
                       log.LongitudinalPersonality.aggressive],
                      [0,10,35])) # speed
class TestFollowingDistance:
  def setup_method(self):
    # Ensure ToyotaTune is OFF so stop_distance in MPC matches get_STOP_DISTANCE(personality)
    # without the +0.5 offset, keeping simulation and expected value consistent.
    self._params = Params()
    self._toyota_tune_prev = self._params.get("ToyotaTune")
    self._params.put_bool("ToyotaTune", False)

  def teardown_method(self):
    # Restore ToyotaTune to its original value after each test
    if self._toyota_tune_prev is None:
      self._params.remove("ToyotaTune")
    else:
      self._params.put("ToyotaTune", self._toyota_tune_prev)

  def test_following_distance(self):
    v_lead = float(self.speed)
    simulation_steady_state = run_following_distance_simulation(v_lead, e2e=self.e2e, personality=self.personality)
    correct_steady_state = desired_follow_distance(v_lead, v_lead, get_T_FOLLOW(self.personality), get_STOP_DISTANCE(self.personality))
    err_ratio = 0.2 if self.e2e else 0.1
    abs_err_margin = 0.5 if v_lead > 0.0 else 1.15
    assert simulation_steady_state == pytest.approx(correct_steady_state, abs=err_ratio * correct_steady_state + abs_err_margin)
