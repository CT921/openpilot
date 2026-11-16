/*
 * Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.
 *
 * This file is part of sunnypilot and is licensed under the MIT License.
 * See the LICENSE.md file in the root directory for more details.
 */

#pragma once

#include "common/params.h"
#include "selfdrive/ui/qt/offroad/speed_limit/helpers.h"
#include "selfdrive/ui/qt/offroad/speed_limit/speed_limit_policy.h"
#include "selfdrive/ui/qt/widgets/controls.h"
#include "selfdrive/ui/qt/widgets/scrollview.h"
#include <QStackedWidget>
#include <QFrame>

class SpeedLimitSettings : public QStackedWidget {
  Q_OBJECT

public:
  SpeedLimitSettings(QWidget *parent = nullptr);
  void refresh();
  void showEvent(QShowEvent *event) override;

signals:
  void backPress();

private:
  Params params;
  ScrollView *speedLimitScroller;
  QFrame *subPanelFrame;
  ButtonParamControl *speed_limit_mode_settings;
  PushButton *speedLimitSource;
  SpeedLimitPolicy *speedLimitPolicyScreen;
  ButtonParamControl *speed_limit_offset_settings;
  OptionControl *speed_limit_offset;

  static QString offsetDescription(SpeedLimitOffsetType type = SpeedLimitOffsetType::NONE) {
    switch (type) {
      case SpeedLimitOffsetType::FIXED:
        return tr("Fixed: Adds a fixed offset [Speed Limit + Offset]");
      case SpeedLimitOffsetType::PERCENT:
        return tr("Percent: Adds a percent offset [Speed Limit + (Offset % Speed Limit)]");
      default:
        return tr("None: No Offset");
    }
  }

  static QString modeDescription(SpeedLimitMode mode = SpeedLimitMode::OFF) {
    switch (mode) {
      case SpeedLimitMode::INFO:
        return tr("Information: Displays the current road's speed limit.");
      case SpeedLimitMode::WARNING:
        return tr("Warning: Provides a warning when exceeding the current road's speed limit.");
      case SpeedLimitMode::ASSIST:
        return tr("Assist: Adjusts the vehicle's cruise speed based on the current road's speed limit when operating the +/- buttons.");
      default:
        return tr("Off: Disables the Speed Limit functions.");
    }
  }
};
