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
void car_err_fun(double *nom_x, double *delta_x, double *out_4624415640507330488);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_5296127735945035033);
void car_H_mod_fun(double *state, double *out_1380935324346926016);
void car_f_fun(double *state, double dt, double *out_961222297369104125);
void car_F_fun(double *state, double dt, double *out_2870206942681480216);
void car_h_25(double *state, double *unused, double *out_3496292854128936459);
void car_H_25(double *state, double *unused, double *out_4903915974336417693);
void car_h_24(double *state, double *unused, double *out_857869959165743186);
void car_H_24(double *state, double *unused, double *out_83594469680429430);
void car_h_30(double *state, double *unused, double *out_4384979796625112726);
void car_H_30(double *state, double *unused, double *out_4774577027193177623);
void car_h_26(double *state, double *unused, double *out_5929008340093117738);
void car_H_26(double *state, double *unused, double *out_1162412655462361469);
void car_h_27(double *state, double *unused, double *out_2892976969452458942);
void car_H_27(double *state, double *unused, double *out_2599813715392752712);
void car_h_29(double *state, double *unused, double *out_2617782907167953053);
void car_H_29(double *state, double *unused, double *out_5284808371507569807);
void car_h_28(double *state, double *unused, double *out_3209542466280735448);
void car_H_28(double *state, double *unused, double *out_7248438643072896058);
void car_h_31(double *state, double *unused, double *out_7133742382382958809);
void car_H_31(double *state, double *unused, double *out_4934561936213378121);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}