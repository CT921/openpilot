/*
 * Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.
 *
 * This file is part of sunnypilot and is licensed under the MIT License.
 * See the LICENSE.md file in the root directory for more details.
 */

#include "selfdrive/ui/qt/offroad/speed_limit/speed_limit_settings.h"

SpeedLimitSettings::SpeedLimitSettings(QWidget *parent) : QStackedWidget(parent) {
  subPanelFrame = new QFrame();
  QVBoxLayout *subPanelLayout = new QVBoxLayout(subPanelFrame);
  subPanelLayout->setContentsMargins(0, 0, 0, 0);
  subPanelLayout->setSpacing(0);

  // Back button
  PanelBackButton *back = new PanelBackButton(tr("Back"));
  connect(back, &QPushButton::clicked, [=]() { emit backPress(); });
  subPanelLayout->addWidget(back, 0, Qt::AlignLeft);

  subPanelLayout->addSpacing(20);

  ListWidget *list = new ListWidget(this);

  speedLimitPolicyScreen = new SpeedLimitPolicy(this);

  std::vector<QString> speed_limit_mode_texts{
    getSpeedLimitModeText(SpeedLimitMode::OFF),
    getSpeedLimitModeText(SpeedLimitMode::INFO),
    getSpeedLimitModeText(SpeedLimitMode::WARNING),
    getSpeedLimitModeText(SpeedLimitMode::ASSIST),
  };
  speed_limit_mode_settings = new ButtonParamControl(
    "SpeedLimitMode",
    tr("Speed Limit Mode"),
    "",
    "",
    speed_limit_mode_texts,
    220);
  list->addItem(speed_limit_mode_settings);

  list->addItem(horizontal_line());
  list->addItem(vertical_space());

  speedLimitSource = new PushButton(tr("Customize Source"));
  connect(speedLimitSource, &QPushButton::clicked, [&]() {
    setCurrentWidget(speedLimitPolicyScreen);
  });
  connect(speedLimitPolicyScreen, &SpeedLimitPolicy::backPress, [&]() {
    setCurrentWidget(subPanelFrame);
    showEvent(new QShowEvent());
  });

  speedLimitSource->setMaximumWidth(600);
  speedLimitSource->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  list->addItem(speedLimitSource);

  list->addItem(vertical_space(0));
  list->addItem(horizontal_line());

  std::vector<QString> speed_limit_offset_texts{
    getSpeedLimitOffsetTypeText(SpeedLimitOffsetType::NONE),
    getSpeedLimitOffsetTypeText(SpeedLimitOffsetType::FIXED),
    getSpeedLimitOffsetTypeText(SpeedLimitOffsetType::PERCENT)
  };
  speed_limit_offset_settings = new ButtonParamControl(
    "SpeedLimitOffsetType",
    tr("Speed Limit Offset"),
    "",
    "",
    speed_limit_offset_texts,
    240);

  list->addItem(speed_limit_offset_settings);

  speed_limit_offset = new OptionControl(
    "SpeedLimitValueOffset",
    "",
    "",
    "",
    {-30, 30}
    );
  list->addItem(speed_limit_offset);

  connect(speed_limit_mode_settings, &ButtonParamControl::buttonClicked, this, &SpeedLimitSettings::refresh);
  connect(speed_limit_offset_settings, &ButtonParamControl::buttonClicked, this, &SpeedLimitSettings::refresh);

  refresh();
  speedLimitScroller = new ScrollView(list, this);
  speedLimitScroller->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  subPanelLayout->addWidget(speedLimitScroller, 1);

  addWidget(subPanelFrame);
  addWidget(speedLimitPolicyScreen);
  setCurrentWidget(subPanelFrame);
}

void SpeedLimitSettings::refresh() {
  bool is_metric_param = params.getBool("IsMetric");
  SpeedLimitMode speed_limit_mode_param = static_cast<SpeedLimitMode>(std::atoi(params.get("SpeedLimitMode").c_str()));
  SpeedLimitOffsetType offset_type_param = static_cast<SpeedLimitOffsetType>(std::atoi(params.get("SpeedLimitOffsetType").c_str()));

  speed_limit_mode_settings->setDescription(modeDescription(speed_limit_mode_param));
  speed_limit_mode_settings->showDescription();
  speed_limit_offset_settings->setDescription(offsetDescription(offset_type_param));
  speed_limit_offset_settings->showDescription();
  speed_limit_offset->setDescription(offsetDescription(offset_type_param));

  QString suffix = "";
  if (offset_type_param == SpeedLimitOffsetType::PERCENT) {
    suffix = "%";
  } else if (offset_type_param == SpeedLimitOffsetType::FIXED) {
    suffix = is_metric_param ? " km/h" : " mph";
  }

  if (offset_type_param == SpeedLimitOffsetType::NONE) {
    speed_limit_offset->setVisible(false);
  } else {
    speed_limit_offset->setVisible(true);
    speed_limit_offset->setLabel(suffix);
    speed_limit_offset->showDescription();
  }
}

void SpeedLimitSettings::showEvent(QShowEvent *event) {
  refresh();
}
