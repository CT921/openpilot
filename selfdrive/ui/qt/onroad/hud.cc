#include "selfdrive/ui/qt/onroad/hud.h"

#include <cmath>
#include <QElapsedTimer>
#include <QPainterPath>
#include "selfdrive/ui/qt/util.h"

constexpr int SET_SPEED_NA = 255;

HudRenderer::HudRenderer() {
  plus_arrow_up_img = loadPixmap("../../top/selfdrive/assets/img_plus_arrow_up", {105, 105});
  minus_arrow_down_img = loadPixmap("../../top/selfdrive/assets/img_minus_arrow_down", {105, 105});

  profile_data = {
    {QPixmap("../assets/aggressive.png"), "Aggressive"},
    {QPixmap("../assets/standard.png"), "Standard"},
    {QPixmap("../assets/relaxed.png"), "Relaxed"}
  };

  const QStringList imagePaths = {
    "../assets/images/tim_turn_signal_1.png",
    "../assets/images/tim_turn_signal_2.png"
  };
  signalImgVector.reserve(2 * imagePaths.size() + 1);
  for (int i = 0; i < 2; ++i) {
    for (const QString& path : imagePaths) {
      signalImgVector.push_back(QPixmap(path));
    }
  }
  signalImgVector.push_back(QPixmap("../assets/images/tim_turn_signal_1_red.png"));
  animation_timer = new QTimer(this);
  connect(animation_timer, &QTimer::timeout, this, [this] {
    animationFrameIndex = (animationFrameIndex + 1) % totalFrames;
  });
  animation_timer->start(totalFrames * 100);

}

void HudRenderer::updateState(const UIState &s) {
  is_metric = s.scene.is_metric;
  status = s.status;
  hideBottomIcons = s.sm->rcv_frame("selfdriveState") > s.scene.started_frame && 
                   (*s.sm)["selfdriveState"].getSelfdriveState().getAlertSize() != cereal::SelfdriveState::AlertSize::NONE;

  blindSpotLeft = s.scene.blind_spot_left;
  blindSpotRight = s.scene.blind_spot_right;
  drivingPersonalitiesUIWheel = s.scene.driving_personalities_ui_wheel;
  timSignals = s.scene.tim_signals;
  personalityProfile = s.scene.personality_profile;
  turnSignalLeft = s.scene.turn_signal_left;
  turnSignalRight = s.scene.turn_signal_right;

  const SubMaster &sm = *(s.sm);
  if (sm.rcv_frame("carState") < s.scene.started_frame) {
    is_cruise_set = false;
    set_speed = SET_SPEED_NA;
    speed = 0.0;
    return;
  }

  const auto car_control = sm["carControl"].getCarControl();
  const auto &controls_state = sm["controlsState"].getControlsState();
  const auto &car_state = sm["carState"].getCarState();
  const auto &drivermonitor_state = sm["driverMonitoringState"].getDriverMonitoringState();
  const auto lp_top = sm["longitudinalPlanTOP"].getLongitudinalPlanTOP();
  const auto lmd = sm["liveMapDataTOP"].getLiveMapDataTOP();

  float speedConv = is_metric ? MS_TO_KPH : MS_TO_MPH;
  speedLimit = lp_top.getSpeedLimit().getResolver().getSpeedLimit() * speedConv;
  speedLimitLast = lp_top.getSpeedLimit().getResolver().getSpeedLimitLast() * speedConv;
  speedLimitOffset = lp_top.getSpeedLimit().getResolver().getSpeedLimitOffset() * speedConv;
  speedLimitValid = lp_top.getSpeedLimit().getResolver().getSpeedLimitValid();
  speedLimitLastValid = lp_top.getSpeedLimit().getResolver().getSpeedLimitLastValid();
  speedLimitFinalLast = lp_top.getSpeedLimit().getResolver().getSpeedLimitFinalLast() * speedConv;
  speedLimitSource = lp_top.getSpeedLimit().getResolver().getSource();
  speedLimitMode = static_cast<SpeedLimitMode>(s.scene.speed_limit_mode);
  speedLimitAssistState = lp_top.getSpeedLimit().getAssist().getState();
  speedLimitAssistActive = lp_top.getSpeedLimit().getAssist().getActive();
  if (sm.updated("liveMapDataTOP")) {
    speedLimitAheadValid = lmd.getSpeedLimitAheadValid();
    speedLimitAhead = lmd.getSpeedLimitAhead() * speedConv;
    speedLimitAheadDistance = lmd.getSpeedLimitAheadDistance();
    if (speedLimitAheadDistance < speedLimitAheadDistancePrev && speedLimitAheadValidFrame < SPEED_LIMIT_AHEAD_VALID_FRAME_THRESHOLD) {
      speedLimitAheadValidFrame++;
    } else if (speedLimitAheadDistance > speedLimitAheadDistancePrev && speedLimitAheadValidFrame > 0) {
      speedLimitAheadValidFrame--;
    }
  }
  speedLimitAheadDistancePrev = speedLimitAheadDistance;

  // Handle older routes where vCruiseCluster is not set
  set_speed = car_state.getVCruiseCluster() == 0.0 ? controls_state.getVCruiseDEPRECATED() : car_state.getVCruiseCluster();
  is_cruise_set = set_speed > 0 && set_speed != SET_SPEED_NA;
  is_cruise_available = set_speed != -1;
  brakeLights = car_state.getBrakeLights();
  rightHandDM = drivermonitor_state.getIsRHD();
  longOverride = car_control.getCruiseControl().getOverride();
  smartCruiseControlVisionEnabled = lp_top.getSmartCruiseControl().getVision().getEnabled();
  smartCruiseControlVisionActive = lp_top.getSmartCruiseControl().getVision().getActive();
  smartCruiseControlMapEnabled = lp_top.getSmartCruiseControl().getMap().getEnabled();
  smartCruiseControlMapActive = lp_top.getSmartCruiseControl().getMap().getActive();

  if (is_cruise_set && !is_metric) {
    set_speed *= KM_TO_MILE;
  }

  // Handle older routes where vEgoCluster is not set
  v_ego_cluster_seen = v_ego_cluster_seen || car_state.getVEgoCluster() != 0.0;
  float v_ego = v_ego_cluster_seen ? car_state.getVEgoCluster() : car_state.getVEgo();
  speed = std::max<float>(0.0f, v_ego * (is_metric ? MS_TO_KPH : MS_TO_MPH));

  if (sm.alive("liveMapDataTOP") && sm.rcv_frame("liveMapDataTOP") > 0)
  {
    const auto live_map_data = sm["liveMapDataTOP"].getLiveMapDataTOP();
    road_name = QString::fromStdString(live_map_data.getRoadName());
  }
}

void HudRenderer::draw(QPainter &p, const QRect &surface_rect) {
  p.save();

  // Draw header gradient
  QLinearGradient bg(0, UI_HEADER_HEIGHT - (UI_HEADER_HEIGHT / 2.5), 0, UI_HEADER_HEIGHT);
  bg.setColorAt(0, brakeLights ? QColor::fromRgbF(1.0, 0.48, 0.5, 0.45) : QColor::fromRgbF(0, 0, 0, 0.45));
  bg.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));
  p.fillRect(0, 0, surface_rect.width(), UI_HEADER_HEIGHT, bg);
  drawRoadName(p, surface_rect);

  if (is_cruise_available) {
    drawSetSpeed(p, surface_rect);

    // Smart Cruise Control
    int x_offset = -260;
    int y1_offset = -70;
    int y2_offset = -150;

    int y_scc_v = 0, y_scc_m = 0;
    const int orders[2] = {y1_offset, y2_offset};
    int i = 0;
    // SCC-V takes first order
    if (smartCruiseControlVisionEnabled) y_scc_v = orders[i++];
    if (smartCruiseControlMapEnabled) y_scc_m = orders[i++];

    // Smart Cruise Control - Vision
    bool scc_vision_active_pulse = pulseElement(smartCruiseControlVisionFrame);
    if ((smartCruiseControlVisionEnabled && !smartCruiseControlVisionActive) || (smartCruiseControlVisionActive && scc_vision_active_pulse)) {
      drawSmartCruiseControlOnroadIcon(p, surface_rect, x_offset, y_scc_v, "V-TSC");
    }
    smartCruiseControlVisionFrame = smartCruiseControlVisionActive ? (smartCruiseControlVisionFrame + 1) : 0;

    // Smart Cruise Control - Map
    bool scc_map_active_pulse = pulseElement(smartCruiseControlMapFrame);
    if ((smartCruiseControlMapEnabled && !smartCruiseControlMapActive) || (smartCruiseControlMapActive && scc_map_active_pulse)) {
      drawSmartCruiseControlOnroadIcon(p, surface_rect, x_offset, y_scc_m, "M-TSC");
    }
    smartCruiseControlMapFrame = smartCruiseControlMapActive ? (smartCruiseControlMapFrame + 1) : 0;

    // Speed Limit
    bool showSpeedLimit;
    bool speed_limit_assist_pre_active_pulse = pulseElement(speedLimitAssistFrame);

    // Position speed limit sign next to set speed box
    const int sign_width = is_metric ? 200 : 172;
    const int sign_x = is_metric ? 280 : 272;
    const int sign_y = 45;
    const int sign_height = 204;
    QRect sign_rect(sign_x, sign_y, sign_width, sign_height);

    if (speedLimitAssistState == cereal::LongitudinalPlanTOP::SpeedLimit::AssistState::PRE_ACTIVE) {
      speedLimitAssistFrame++;
      showSpeedLimit = speed_limit_assist_pre_active_pulse;
      drawSpeedLimitPreActiveArrow(p, sign_rect);
    } else {
      speedLimitAssistFrame = 0;
      showSpeedLimit = speedLimitMode != SpeedLimitMode::OFF;
    }

    if (showSpeedLimit) {
      drawSpeedLimitSigns(p, sign_rect);

      // do not show during SLA's preActive state
      if (speedLimitAssistState != cereal::LongitudinalPlanTOP::SpeedLimit::AssistState::PRE_ACTIVE) {
        drawUpcomingSpeedLimit(p);
      }
    }
  }
  drawCurrentSpeed(p, surface_rect);

  if (drivingPersonalitiesUIWheel && !hideBottomIcons) {
    drawDrivingPersonalities(p, surface_rect);
  }

  if (timSignals && (turnSignalLeft || turnSignalRight)) {
    drawTimSignals(p, surface_rect);
  }

  p.restore();
}

void HudRenderer::drawSetSpeed(QPainter &p, const QRect &surface_rect) {
  // Draw outer box + border to contain set speed
  const QSize default_size = {172, 204};
  QSize set_speed_size = is_metric ? QSize(200, 204) : default_size;
  QRect set_speed_rect(QPoint(60 + (default_size.width() - set_speed_size.width()) / 2, 45), set_speed_size);

  // Draw set speed box
  p.setPen(QPen(QColor(255, 255, 255, 75), 6));
  p.setBrush(QColor(0, 0, 0, 166));
  p.drawRoundedRect(set_speed_rect, 32, 32);

  // Colors based on status
  QColor max_color = QColor(0xa6, 0xa6, 0xa6, 0xff);
  QColor set_speed_color = QColor(0x72, 0x72, 0x72, 0xff);
  if (is_cruise_set) {
    set_speed_color = QColor(255, 255, 255);
    if (speedLimitAssistActive) {
      set_speed_color = longOverride ? QColor(0x91, 0x9b, 0x95, 0xff) : QColor(0, 0xff, 0, 0xff);
      max_color = longOverride ? QColor(0x91, 0x9b, 0x95, 0xff) : QColor(0x80, 0xd8, 0xa6, 0xff);
    } else {
      if (status == STATUS_DISENGAGED) {
        max_color = QColor(255, 255, 255);
      } else if (status == STATUS_OVERRIDE) {
        max_color = QColor(0x91, 0x9b, 0x95, 0xff);
      } else {
        max_color = QColor(0x80, 0xd8, 0xa6, 0xff);
      }
    }
  }

  // Draw "MAX" text
  p.setFont(InterFont(40, QFont::DemiBold));
  p.setPen(max_color);
  p.drawText(set_speed_rect.adjusted(0, 27, 0, 0), Qt::AlignTop | Qt::AlignHCenter, tr("MAX"));

  // Draw set speed
  QString setSpeedStr = is_cruise_set ? QString::number(std::nearbyint(set_speed)) : "–";
  p.setFont(InterFont(90, QFont::Bold));
  p.setPen(set_speed_color);
  p.drawText(set_speed_rect.adjusted(0, 77, 0, 0), Qt::AlignTop | Qt::AlignHCenter, setSpeedStr);
}

void HudRenderer::drawCurrentSpeed(QPainter &p, const QRect &surface_rect) {
  QString speedStr = QString::number(std::nearbyint(speed));

  p.setFont(InterFont(176, QFont::Bold));
  drawText(p, surface_rect.center().x(), 210, speedStr);

  p.setFont(InterFont(66));
  drawText(p, surface_rect.center().x(), 290, is_metric ? tr("km/h") : tr("mph"), 200);
}

void HudRenderer::drawText(QPainter &p, int x, int y, const QString &text, int alpha) {
  QRect real_rect = p.fontMetrics().boundingRect(text);
  real_rect.moveCenter({x, y - real_rect.height() / 2});

  p.setPen(QColor(0xff, 0xff, 0xff, alpha));
  p.drawText(real_rect.x(), real_rect.bottom(), text);
}

bool HudRenderer::pulseElement(int frame) {
  if (frame % UI_FREQ < (UI_FREQ / 2.5)) {
    return false;
  }

  return true;
}

void HudRenderer::drawSmartCruiseControlOnroadIcon(QPainter &p, const QRect &surface_rect, int x_offset, int y_offset, std::string name) {
  int x = surface_rect.center().x();
  int y = surface_rect.height() / 4;

  QString text = QString::fromStdString(name);
  QFont font = InterFont(42, QFont::Bold);
  p.setFont(font);

  QFontMetrics fm(font);

  int padding_v = 5;
  int box_width = 160;
  int box_height = fm.height() + padding_v * 2;

  QRectF bg_rect(x - (box_width / 2) + x_offset,
                 y - (box_height / 2) + y_offset,
                 box_width, box_height);

  QPainterPath boxPath;
  boxPath.addRoundedRect(bg_rect, 10, 10);

  int text_w = fm.horizontalAdvance(text);
  qreal baseline_y = bg_rect.top() + padding_v + fm.ascent();
  qreal text_x = bg_rect.center().x() - (text_w / 2.0);

  QPainterPath textPath;
  textPath.addText(QPointF(text_x, baseline_y), font, text);
  boxPath = boxPath.subtracted(textPath);

  p.setPen(Qt::NoPen);
  p.setBrush(longOverride ? QColor(0x91, 0x9b, 0x95, 0xf1) : QColor(0, 0xff, 0, 0xff));
  p.drawPath(boxPath);
}

void HudRenderer::drawIcon(QPainter &p, QPoint pos, const QPixmap &img, 
                         QColor bg_color, qreal opacity) {
  p.setOpacity(opacity);
  
  // Draw background circle
  if (bg_color.alpha() > 0) {
    p.setPen(Qt::NoPen);
    p.setBrush(bg_color);
    p.drawEllipse(pos, btn_size/2, btn_size/2);
  }

  p.drawPixmap(pos.x() - img.width()/2, pos.y() - img.height()/2, img);
  p.setOpacity(1.0);
}

void HudRenderer::drawDrivingPersonalities(QPainter &p, const QRect &rect) {
  // Declare the variables
  static QElapsedTimer timer;
  static bool displayText = false;
  static int lastProfile = 4;
  constexpr int fadeDuration = 1000; // 1 second
  constexpr int textDuration = 3000; // 3 seconds

  // Set the x and y coordinates
  int x = rightHandDM ? rect.right() - (btn_size - 24) / 2 - (bdr_s * 2) - 250 : rect.right() - (btn_size - 96) / 2 - (bdr_s * 2);
  const int y = rect.bottom() - footer_h / 2 - 80;

  // Enable Antialiasing
  p.setRenderHint(QPainter::Antialiasing);
  p.setRenderHint(QPainter::TextAntialiasing);

  // Select the appropriate profile image/text
  int index = qBound(0, personalityProfile, 2);
  QPixmap &profile_image = profile_data[index].first;
  QString profile_text = profile_data[index].second;

  // Display the profile text when the user changes profiles
  if (lastProfile != personalityProfile) {
    displayText = true;
    lastProfile = personalityProfile;
    timer.restart();
  }

  // Set the text display
  displayText = !timer.hasExpired(textDuration);

  // Set the elapsed time since the profile switch
  int elapsed = timer.elapsed();

  // Calculate the opacity for the text and image based on the elapsed time
  qreal textOpacity = qBound(0.0, (1.0 - static_cast<qreal>(elapsed - textDuration) / fadeDuration), 1.0);
  qreal imageOpacity = qBound(0.0, (static_cast<qreal>(elapsed - textDuration) / fadeDuration), 1.0);

  // Draw the profile text with the calculated opacity
  if (displayText && textOpacity > 0.0) {
    p.setFont(InterFont(40, QFont::Bold));
    p.setPen(QColor(255, 255, 255));
    // Calculate the center position for text
    QFontMetrics fontMetrics(p.font());
    int textWidth = fontMetrics.horizontalAdvance(profile_text);
    // Apply opacity to the text
    p.setOpacity(textOpacity);
    p.drawText(x - textWidth / 2, y + fontMetrics.height() / 2, profile_text);
  }

  // Draw the profile image with the calculated opacity
  if (imageOpacity > 0.0) {
    drawIcon(p, QPoint(x, y), profile_image, blackColor(0), imageOpacity);
  }
}

void HudRenderer::drawTimSignals(QPainter &p, const QRect &rect) {
  // Declare the turn signal size
  constexpr int signalHeight = 142;
  constexpr int signalWidth = 142;

  // Calculate the vertical position for the turn signals
  const int baseYPosition = (blindSpotLeft || blindSpotRight ? 
                           (rect.height() - signalHeight) / 2 : 350);
                           
  // Calculate the x-coordinates for the turn signals
  int leftSignalXPosition = rect.width() / 2 - 50 - 360 * (blindSpotLeft ? 2 : 0);
  int rightSignalXPosition = rect.width() / 2 - 50 + 360 * (blindSpotRight ? 2 : 0);

  // Enable Antialiasing
  p.setRenderHint(QPainter::Antialiasing);

  // Draw the turn signals
  if (animationFrameIndex < static_cast<int>(signalImgVector.size())) {
    const auto drawSignal = [&](const bool signalActivated, const int xPosition, 
                               const bool flip, const bool blindspot) {
      if (signalActivated) {
        // Get the appropriate image from the signalImgVector
        QPixmap signal = signalImgVector[
          (blindspot ? signalImgVector.size()-1 : animationFrameIndex % totalFrames)
        ].transformed(QTransform().scale(flip ? -1 : 1, 1));
        
        // Draw the image
        p.drawPixmap(xPosition, baseYPosition, signalWidth, signalHeight, signal);
      }
    };

    // Display the animation based on which signal is activated
    drawSignal(turnSignalLeft, leftSignalXPosition, false, blindSpotLeft);
    drawSignal(turnSignalRight, rightSignalXPosition, true, blindSpotRight);
  }
}

void HudRenderer::drawRoadName(QPainter &p, const QRect &surface_rect)
{
  if (road_name.isEmpty() || road_name.isNull()) {
       return;
  }

  // Set font first to measure text
  p.setFont(InterFont(48, QFont::DemiBold));
  QFontMetrics fm(p.font());

  // Calculate required width based on text + padding
  int text_width = fm.horizontalAdvance(road_name);
  int padding = 40;
  int rect_width = text_width + padding;

  // Set minimum and maximum widths
  int min_width = 200;
  int max_width = surface_rect.width() - 40;
  rect_width = std::max(min_width, std::min(rect_width, max_width));

  // Position road name at the bottom center
  QRect road_rect(surface_rect.width() / 2 - rect_width / 2, -4, rect_width, 60);

  p.setPen(Qt::NoPen);
  p.setBrush(QColor(0, 0, 0, 120));
  p.drawRoundedRect(road_rect, 12, 12);

  // Truncate long road names if they still don't fit
  p.setPen(QColor(255, 215, 0, 255));
  QString truncated = fm.elidedText(road_name, Qt::ElideRight, road_rect.width() - 20);
  p.drawText(road_rect, Qt::AlignCenter, truncated);
}

void HudRenderer::drawSpeedLimitSigns(QPainter &p, QRect &sign_rect) {
  bool speedLimitWarningEnabled = speedLimitMode >= SpeedLimitMode::WARNING;  // TODO-TOP: update to include SpeedLimitMode::ASSIST
  bool hasSpeedLimit = speedLimitValid || speedLimitLastValid;
  bool overspeed = hasSpeedLimit && std::nearbyint(speedLimitFinalLast) < std::nearbyint(speed);
  QString speedLimitStr = hasSpeedLimit ? QString::number(std::nearbyint(speedLimitLast)) : "---";

  // Offset display text
  QString speedLimitSubText = "";
  if (speedLimitOffset != 0) {
    speedLimitSubText = (speedLimitOffset > 0 ? "" : "-") + QString::number(std::nearbyint(speedLimitOffset));
  }

  float speedLimitSubTextFactor = is_metric ? 0.5 : 0.6;
  if (speedLimitSubText.size() >= 3) {
    speedLimitSubTextFactor = 0.475;
  }

  int alpha = 255;
  QColor red_color = QColor(255, 0, 0, alpha);
  QColor speed_color = (speedLimitWarningEnabled && overspeed) ? red_color :
                       (!speedLimitValid && speedLimitLastValid ? QColor(0x91, 0x9b, 0x95, 0xf1) : QColor(0, 0, 0, alpha));

  if (is_metric) {
    // EU Vienna Convention style circular sign
    QRect vienna_rect = sign_rect;
    int circle_size = std::min(vienna_rect.width(), vienna_rect.height());
    QRect circle_rect(vienna_rect.x(), vienna_rect.y(), circle_size, circle_size);

    if (vienna_rect.width() > vienna_rect.height()) {
      circle_rect.moveLeft(vienna_rect.x() + (vienna_rect.width() - circle_size) / 2);
    } else if (vienna_rect.height() > vienna_rect.width()) {
      circle_rect.moveTop(vienna_rect.y() + (vienna_rect.height() - circle_size) / 2);
    }

    // White background circle
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(255, 255, 255, alpha));
    p.drawEllipse(circle_rect);

    // Red border ring with color coding
    QRect red_ring = circle_rect;

    p.setBrush(red_color);
    p.drawEllipse(red_ring);

    // Center white circle for text
    int ring_size = circle_size * 0.12;
    QRect center_circle = red_ring.adjusted(ring_size, ring_size, -ring_size, -ring_size);
    p.setBrush(QColor(255, 255, 255, alpha));
    p.drawEllipse(center_circle);

    // Speed value, smaller font for 3+ digits
    int font_size = (speedLimitStr.size() >= 3) ? 70 : 85;
    p.setFont(InterFont(font_size, QFont::Bold));

    p.setPen(speed_color);
    p.drawText(center_circle, Qt::AlignCenter, speedLimitStr);

    // Offset value in small circular box
    if (!speedLimitSubText.isEmpty() && hasSpeedLimit) {
      int offset_circle_size = circle_size * 0.4;
      int overlap = offset_circle_size * 0.25;
      QRect offset_circle_rect(
        circle_rect.right() - offset_circle_size/1.25 + overlap,
        circle_rect.top() - offset_circle_size/1.75 + overlap,
        offset_circle_size,
        offset_circle_size
      );

      p.setPen(QPen(QColor(77, 77, 77, 255), 6));
      p.setBrush(QColor(0, 0, 0, alpha));
      p.drawEllipse(offset_circle_rect);

      p.setFont(InterFont(offset_circle_size * speedLimitSubTextFactor, QFont::Bold));
      p.setPen(QColor(255, 255, 255, alpha));
      p.drawText(offset_circle_rect, Qt::AlignCenter, speedLimitSubText);
    }
  } else {
    // US/Canada MUTCD style sign
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(255, 255, 255, alpha));
    p.drawRoundedRect(sign_rect, 32, 32);

    // Inner border with violation color coding
    QRect inner_rect = sign_rect.adjusted(10, 10, -10, -10);
    QColor border_color = QColor(0, 0, 0, alpha);

    p.setPen(QPen(border_color, 4));
    p.setBrush(QColor(255, 255, 255, alpha));
    p.drawRoundedRect(inner_rect, 22, 22);

    // "SPEED LIMIT" text
    p.setFont(InterFont(40, QFont::DemiBold));
    p.setPen(QColor(0, 0, 0, alpha));
    p.drawText(inner_rect.adjusted(0, 10, 0, 0), Qt::AlignTop | Qt::AlignHCenter, tr("SPEED"));
    p.drawText(inner_rect.adjusted(0, 50, 0, 0), Qt::AlignTop | Qt::AlignHCenter, tr("LIMIT"));

    // Speed value with color coding
    p.setFont(InterFont(90, QFont::Bold));

    p.setPen(speed_color);
    p.drawText(inner_rect.adjusted(0, 80, 0, 0), Qt::AlignTop | Qt::AlignHCenter, speedLimitStr);

    // Offset value in small box
    if (!speedLimitSubText.isEmpty() && hasSpeedLimit) {
      int offset_box_size = sign_rect.width() * 0.4;
      int overlap = offset_box_size * 0.25;
      QRect offset_box_rect(
        sign_rect.right() - offset_box_size/1.5 + overlap,
        sign_rect.top() - offset_box_size/1.25 + overlap,
        offset_box_size,
        offset_box_size
      );

      int corner_radius = offset_box_size * 0.2;
      p.setPen(QPen(QColor(77, 77, 77, 255), 6));
      p.setBrush(QColor(0, 0, 0, alpha));
      p.drawRoundedRect(offset_box_rect, corner_radius, corner_radius);

      p.setFont(InterFont(offset_box_size * speedLimitSubTextFactor, QFont::Bold));
      p.setPen(QColor(255, 255, 255, alpha));
      p.drawText(offset_box_rect, Qt::AlignCenter, speedLimitSubText);
    }
  }
}

void HudRenderer::drawUpcomingSpeedLimit(QPainter &p) {
  bool speed_limit_ahead = speedLimitAheadValid && speedLimitAhead > 0 && speedLimitAhead != speedLimit && speedLimitAheadValidFrame > 0 &&
                           speedLimitSource == cereal::LongitudinalPlanTOP::SpeedLimit::Source::MAP;
  if (!speed_limit_ahead) {
    return;
  }

  auto roundToInterval = [&](float distance, int interval, int threshold) {
    int base = static_cast<int>(distance / interval) * interval;
    return (distance - base >= threshold) ? base + interval : base;
  };

  auto outputDistance = [&] {
    if (is_metric) {
      if (speedLimitAheadDistance < 50) return tr("Near");
      if (speedLimitAheadDistance >= 1000) return QString::number(speedLimitAheadDistance * METER_TO_KM, 'f', 1) + tr("km");

      int rounded = (speedLimitAheadDistance < 200) ? std::max(10, roundToInterval(speedLimitAheadDistance, 10, 5)) : roundToInterval(speedLimitAheadDistance, 100, 50);
      return QString::number(rounded) + tr("m");
    } else {
      float distance_ft = speedLimitAheadDistance * METER_TO_FOOT;
      if (distance_ft < 100) return tr("Near");
      if (distance_ft >= 900) return QString::number(speedLimitAheadDistance * METER_TO_MILE, 'f', 1) + tr("mi");

      int rounded = (distance_ft < 500) ? std::max(50, roundToInterval(distance_ft, 50, 25)) : roundToInterval(distance_ft, 100, 50);
      return QString::number(rounded) + tr("ft");
    }
  };

  QString speedStr = QString::number(std::nearbyint(speedLimitAhead));
  QString distanceStr = outputDistance();

  // Position below current speed limit sign
  const int sign_width = is_metric ? 200 : 172;
  const int sign_x = is_metric ? 280 : 272;
  const int sign_y = 45;
  const int sign_height = 204;

  const int ahead_width = 170;
  const int ahead_height = 160;
  const int ahead_x = sign_x + (sign_width - ahead_width) / 2;
  const int ahead_y = sign_y + sign_height + 10;

  QRect ahead_rect(ahead_x, ahead_y, ahead_width, ahead_height);
  p.setPen(QPen(QColor(255, 255, 255, 100), 3));
  p.setBrush(QColor(0, 0, 0, 180));
  p.drawRoundedRect(ahead_rect, 16, 16);

  // "AHEAD" label
  p.setFont(InterFont(40, QFont::DemiBold));
  p.setPen(QColor(200, 200, 200, 255));
  p.drawText(ahead_rect.adjusted(0, 4, 0, 0), Qt::AlignTop | Qt::AlignHCenter, tr("AHEAD"));

  // Speed value
  p.setFont(InterFont(70, QFont::Bold));
  p.setPen(QColor(255, 255, 255, 255));
  p.drawText(ahead_rect.adjusted(0, 38, 0, 0), Qt::AlignTop | Qt::AlignHCenter, speedStr);

  // Distance
  p.setFont(InterFont(40, QFont::Normal));
  p.setPen(QColor(180, 180, 180, 255));
  p.drawText(ahead_rect.adjusted(0, 110, 0, 0), Qt::AlignTop | Qt::AlignHCenter, distanceStr);
}


void HudRenderer::drawSpeedLimitPreActiveArrow(QPainter &p, QRect &sign_rect) {
  const int sign_margin = 12;
  const int arrow_spacing = sign_margin * 3;
  int arrow_x = sign_rect.right() + arrow_spacing;

  int _set_speed = std::nearbyint(set_speed);
  int _speed_limit_final_last = std::nearbyint(speedLimitFinalLast);

  // Calculate the vertical offset using a sinusoidal function for smooth bouncing
  double bounce_frequency = 2.0 * M_PI / UI_FREQ;  // 20 frames for one full oscillation
  int bounce_offset = 20 * sin(speedLimitAssistFrame * bounce_frequency);  // Adjust the amplitude (20 pixels) as needed

  if (_set_speed < _speed_limit_final_last) {
    QPoint iconPosition(arrow_x, sign_rect.center().y() - plus_arrow_up_img.height() / 2 + bounce_offset);
    p.drawPixmap(iconPosition, plus_arrow_up_img);
  } else if (_set_speed > _speed_limit_final_last) {
    QPoint iconPosition(arrow_x, sign_rect.center().y() - minus_arrow_down_img.height() / 2 - bounce_offset);
    p.drawPixmap(iconPosition, minus_arrow_down_img);
  }
}
