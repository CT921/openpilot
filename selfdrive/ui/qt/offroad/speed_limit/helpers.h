/*
 * Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.
 *
 * This file is part of sunnypilot and is licensed under the MIT License.
 * See the LICENSE.md file in the root directory for more details.
 */
#pragma once

#include <QObject>
#include <QString>
#include <QCoreApplication>

enum class SpeedLimitOffsetType {
  NONE,
  FIXED,
  PERCENT,
};

inline QString getSpeedLimitOffsetTypeText(SpeedLimitOffsetType type) {
  switch(type) {
    case SpeedLimitOffsetType::NONE: 
      return QCoreApplication::translate("QObject", "None");
    case SpeedLimitOffsetType::FIXED: 
      return QCoreApplication::translate("QObject", "Fixed");
    case SpeedLimitOffsetType::PERCENT: 
      return QCoreApplication::translate("QObject", "Percent");
    default: return "";
  }
}

enum class SpeedLimitSourcePolicy {
  CAR_ONLY,
  MAP_ONLY,
  CAR_FIRST,
  MAP_FIRST,
  COMBINED,
};

inline QString getSpeedLimitSourcePolicyText(SpeedLimitSourcePolicy policy) {
  switch(policy) {
    case SpeedLimitSourcePolicy::CAR_ONLY: 
      return QCoreApplication::translate("QObject", "Car\nOnly");
    case SpeedLimitSourcePolicy::MAP_ONLY: 
      return QCoreApplication::translate("QObject", "Map\nOnly");
    case SpeedLimitSourcePolicy::CAR_FIRST: 
      return QCoreApplication::translate("QObject", "Car\nFirst");
    case SpeedLimitSourcePolicy::MAP_FIRST: 
      return QCoreApplication::translate("QObject", "Map\nFirst");
    case SpeedLimitSourcePolicy::COMBINED: 
      return QCoreApplication::translate("QObject", "Both\nData");
    default: return "";
  }
}

enum class SpeedLimitMode {
  OFF,
  INFO,
  WARNING,
  ASSIST,
};

inline QString getSpeedLimitModeText(SpeedLimitMode mode) {
  switch(mode) {
    case SpeedLimitMode::OFF: 
      return QCoreApplication::translate("QObject", "Off");
    case SpeedLimitMode::INFO: 
      return QCoreApplication::translate("QObject", "Info.");
    case SpeedLimitMode::WARNING: 
      return QCoreApplication::translate("QObject", "Warning");
    case SpeedLimitMode::ASSIST: 
      return QCoreApplication::translate("QObject", "Assist");
    default: return "";
  }
}
