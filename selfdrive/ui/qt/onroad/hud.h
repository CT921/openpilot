#pragma once

#include <QPainter>
#include "selfdrive/ui/ui.h"
#include "selfdrive/ui/qt/offroad/speed_limit/helpers.h"

constexpr int SPEED_LIMIT_AHEAD_VALID_FRAME_THRESHOLD = 5;

class HudRenderer : public QObject {
  Q_OBJECT

public:
  HudRenderer();
  void updateState(const UIState &s);
  void draw(QPainter &p, const QRect &surface_rect);

private:
  void drawSetSpeed(QPainter &p, const QRect &surface_rect);
  void drawCurrentSpeed(QPainter &p, const QRect &surface_rect);
  void drawText(QPainter &p, int x, int y, const QString &text, int alpha = 255);

  void drawIcon(QPainter &p, QPoint pos, const QPixmap &img, QColor bg_color = QColor(0,0,0,0), qreal opacity = 1.0);
  void drawDrivingPersonalities(QPainter &p, const QRect &rect);
  void drawTimSignals(QPainter &p, const QRect &rect);
  void drawSpeedLimitSigns(QPainter &p, QRect &sign_rect);
  void drawUpcomingSpeedLimit(QPainter &p);
  void drawSpeedLimitPreActiveArrow(QPainter &p, QRect &sign_rect);

  bool pulseElement(int frame);
  void drawSmartCruiseControlOnroadIcon(QPainter &p, const QRect &surface_rect, int x_offset, int y_offset, std::string name);

  QString road_name;
  void drawRoadName(QPainter &p, const QRect &surface_rect);

  static constexpr int btn_size = 250;
  static constexpr int UI_BORDER_SIZE = 15;
  static constexpr int bdr_s = 30;
  static constexpr int footer_h = 60;

  float speed = 0;
  float set_speed = 0;
  bool is_cruise_set = false;
  bool is_cruise_available = true;
  bool is_metric = false;
  bool v_ego_cluster_seen = false;
  bool brakeLights = false;
  int status = STATUS_DISENGAGED;

  bool longOverride;
  bool smartCruiseControlVisionEnabled;
  bool smartCruiseControlVisionActive;
  int smartCruiseControlVisionFrame;
  bool smartCruiseControlMapEnabled;
  bool smartCruiseControlMapActive;
  int smartCruiseControlMapFrame;

  float speedLimit;
  float speedLimitLast;
  float speedLimitOffset;
  bool speedLimitValid;
  bool speedLimitLastValid;
  float speedLimitFinalLast;
  cereal::LongitudinalPlanTOP::SpeedLimit::Source speedLimitSource;
  bool speedLimitAheadValid;
  float speedLimitAhead;
  float speedLimitAheadDistance;
  float speedLimitAheadDistancePrev;
  int speedLimitAheadValidFrame;
  SpeedLimitMode speedLimitMode = SpeedLimitMode::OFF;
  cereal::LongitudinalPlanTOP::SpeedLimit::AssistState speedLimitAssistState;
  bool speedLimitAssistActive;
  int speedLimitAssistFrame;
  QPixmap plus_arrow_up_img;
  QPixmap minus_arrow_down_img;

  bool blindSpotLeft = false;
  bool blindSpotRight = false;
  bool drivingPersonalitiesUIWheel = false;
  bool timSignals = false;
  bool hideBottomIcons = false;
  bool turnSignalLeft = false;
  bool turnSignalRight = false;
  bool rightHandDM = false;
  int personalityProfile = 0;
  int animationFrameIndex = 0;
  static constexpr int totalFrames = 4;
  QVector<std::pair<QPixmap, QString>> profile_data;
  std::vector<QPixmap> signalImgVector;

  QTimer *animation_timer = nullptr;

  inline QColor redColor(int alpha = 255) { return QColor(201, 34, 49, alpha); }
  inline QColor whiteColor(int alpha = 255) { return QColor(255, 255, 255, alpha); }
  inline QColor blackColor(int alpha = 255) { return QColor(0, 0, 0, alpha); }
};
