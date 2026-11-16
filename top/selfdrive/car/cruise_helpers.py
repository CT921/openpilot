"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""

from cereal import car, log
from opendbc.car import structs
from openpilot.common.params import Params

ButtonType = car.CarState.ButtonEvent.Type
EventName = log.OnroadEvent.EventName

DISTANCE_LONG_PRESS = 150
DISTANCE_SHORT_PRESS = 50


class CruiseHelper:
  def __init__(self, CP: structs.CarParams):
    self.CP = CP
    self.params = Params()

    self.button_frame_counts = {ButtonType.gapAdjustCruise: 0}
    self._experimental_mode = False
    self.experimental_mode_switched = False

    self.experimental_mode_via_wheel = self.CP.experimentalModeViaWheel
    self.ispressed_prev = False
    self.distance_button_hold = False
    self.gap_button_counter = 0
    self.short_press_button_counter = 0

    if self.params.get("UserExperimentalMode") is None:
      user_exp_mode = self.params.get_bool("ExperimentalMode")
      self.params.put_bool("UserExperimentalMode", user_exp_mode)
    self.manual_exp_mode_change = False
    self.last_exp_mode_state = self.params.get_bool("ExperimentalMode")

    self.sng_e2e_enabled = self.params.get_bool("sng_e2e")
    self.other_systems_active = self.sng_e2e_enabled

  def update(self, CS, events, experimental_mode) -> None:
    if self.CP.openpilotLongitudinalControl:
      if CS.cruiseState.available:
        self.update_button_frame_counts(CS)

        current_exp_mode = self.params.get_bool("ExperimentalMode")
        if current_exp_mode != self.last_exp_mode_state and not self.manual_exp_mode_change:
          self._reset_button_counters()
          print("CruiseHelper: Detected external experimental mode change, resetting counters")
        distance_button_pressed = self._get_distance_button_state(CS)

        if not self._should_defer_to_other_systems():
          self._update_distance_button_logic(distance_button_pressed, experimental_mode)
        else:
          if distance_button_pressed:
            self._reset_button_counters()

        # toggle experimental mode once on distance button hold
        self.update_experimental_mode(events, experimental_mode)
        self.last_exp_mode_state = current_exp_mode
        self.manual_exp_mode_change = False

  def update_button_frame_counts(self, CS) -> None:
    for button in self.button_frame_counts:
      if self.button_frame_counts[button] > 0:
        self.button_frame_counts[button] += 1

    for button_event in CS.buttonEvents:
      button = button_event.type.raw
      if button in self.button_frame_counts:
        self.button_frame_counts[button] = int(button_event.pressed)

  def update_experimental_mode(self, events, experimental_mode) -> None:
    if self.button_frame_counts[ButtonType.gapAdjustCruise] >= DISTANCE_LONG_PRESS and not self.experimental_mode_switched:
      self._experimental_mode = not experimental_mode
      self.params.put_bool_nonblocking("ExperimentalMode", self._experimental_mode)
      events.add(EventName.experimentalModeSwitched)
      self.experimental_mode_switched = True

  def _get_distance_button_state(self, CS) -> bool:
    for button_event in CS.buttonEvents:
      if button_event.type == ButtonType.gapAdjustCruise and button_event.pressed:
        return True
    return False

  def _should_defer_to_other_systems(self) -> bool:
    if self.sng_e2e_enabled:
      return True
    return False

  def _reset_button_counters(self) -> None:
    self.gap_button_counter = 0
    self.short_press_button_counter = 0
    self.distance_button_hold = False

  def _update_distance_button_logic(self, distance_button_pressed: bool, experimental_mode: bool) -> None:
    # change experimental/chill mode on fly with long press
    if distance_button_pressed:
      self.short_press_button_counter += 1
      if not self.distance_button_hold:
        self.gap_button_counter += 1
        if self.gap_button_counter > DISTANCE_LONG_PRESS:
          if not self._should_defer_to_other_systems():
            new_exp_mode = not self.params.get_bool("ExperimentalMode")
            self.params.put_bool_nonblocking('ExperimentalMode', new_exp_mode)
            self.params.put_bool_nonblocking('UserExperimentalMode', new_exp_mode)
            self.manual_exp_mode_change = True
            print(f"CruiseHelper: Manual experimental mode switch to {new_exp_mode}")
          else:
            print("CruiseHelper: Experimental mode switch deferred to other systems")
          self.gap_button_counter = 0

    if not distance_button_pressed and self.ispressed_prev and self.short_press_button_counter < DISTANCE_SHORT_PRESS:
      self.distance_button_hold = True

    if not self.ispressed_prev and not distance_button_pressed:
      self.distance_button_hold = False
      self.short_press_button_counter = 0

    self.ispressed_prev = distance_button_pressed

  def reset_experimental_mode_switch(self) -> None:
    self.experimental_mode_switched = False

  def get_distance_button_hold_state(self) -> bool:
    return self.distance_button_hold

  def get_gap_button_counter(self) -> int:
    return self.gap_button_counter
