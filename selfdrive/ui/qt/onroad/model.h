#pragma once

#include <QPainter>
#include <QPolygonF>

#include "selfdrive/ui/ui.h"

struct LeadcarLockon {
  float x, y, d, a, lxt, lxf, lockOK;
};
#define LeadcarLockon_MAX 2

class ModelRenderer {
public:
  ModelRenderer();
  void setTransform(const Eigen::Matrix3f &transform) { car_space_transform = transform; }
  void draw(QPainter &painter, const QRect &rect_param);
  void drawLockon(QPainter &painter, const cereal::ModelDataV2::LeadDataV3::Reader &lead_data, const QPointF &vd, int num);

private:
  bool mapToScreen(float in_x, float in_y, float in_z, QPointF *out);
  void update_leads(const cereal::RadarState::Reader &radar_state, const cereal::XYZTData::Reader &line);
  void update_model(const cereal::ModelDataV2::Reader &model, const cereal::RadarState::LeadData::Reader &lead);
  void drawLaneLines(QPainter &painter);
  void drawPath(QPainter &painter, const cereal::ModelDataV2::Reader &model, int height);
  void updatePathGradient(QLinearGradient &bg);
  QColor blendColors(const QColor &start, const QColor &end, float t);
  void drawLead(QPainter &painter, const cereal::RadarState::LeadData::Reader &lead_data, const QPointF &vd, int num);
  void mapLineToPolygon(const cereal::XYZTData::Reader &line, float y_off, float z_off,
                        QPolygonF *pvd, int max_idx, bool allow_invert = true);

  bool longitudinal_control = false;
  bool experimental_mode = false;
  float blend_factor = 1.0f;
  bool prev_allow_throttle = true;
  float lane_line_probs[4] = {};
  float road_edge_stds[2] = {};
  float path_offset_z = 1.22f;
  QPolygonF track_vertices;
  QPolygonF lane_line_vertices[4] = {};
  QPolygonF road_edge_vertices[2] = {};
  QPointF lead_vertices[2] = {};
  Eigen::Matrix3f car_space_transform = Eigen::Matrix3f::Zero();
  QRectF clip_region;
  QRect surface_rect;
  LeadcarLockon leadcar_lockon[LeadcarLockon_MAX];
  float vc_speed; 
  static float global_a_rel;
  static float global_a_rel_col;

};
