#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_5761959574788611112);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_7722816966219680250);
void car_H_mod_fun(double *state, double *out_6261109591928099587);
void car_f_fun(double *state, double dt, double *out_3430613697628368432);
void car_F_fun(double *state, double dt, double *out_329430980246932027);
void car_h_25(double *state, double *unused, double *out_6962537965763093599);
void car_H_25(double *state, double *unused, double *out_9091616208989055521);
void car_h_24(double *state, double *unused, double *out_9086466335703166215);
void car_H_24(double *state, double *unused, double *out_7182478265714996529);
void car_h_30(double *state, double *unused, double *out_1647929917074108392);
void car_H_30(double *state, double *unused, double *out_6573283250481806894);
void car_h_26(double *state, double *unused, double *out_7290659788155065227);
void car_H_26(double *state, double *unused, double *out_5613624545846439871);
void car_h_27(double *state, double *unused, double *out_586412670239069213);
void car_H_27(double *state, double *unused, double *out_4349689179297863677);
void car_h_29(double *state, double *unused, double *out_2030493016390104616);
void car_H_29(double *state, double *unused, double *out_6063051906167414710);
void car_h_28(double *state, double *unused, double *out_2774700312940059471);
void car_H_28(double *state, double *unused, double *out_7301293150472606332);
void car_h_31(double *state, double *unused, double *out_2175179318920757583);
void car_H_31(double *state, double *unused, double *out_4987416443613088395);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}