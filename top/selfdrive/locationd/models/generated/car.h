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
void car_err_fun(double *nom_x, double *delta_x, double *out_1891774851159901983);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_5837655934759956642);
void car_H_mod_fun(double *state, double *out_7894264172705417836);
void car_f_fun(double *state, double dt, double *out_8559630220606310989);
void car_F_fun(double *state, double dt, double *out_47845048400386193);
void car_h_25(double *state, double *unused, double *out_8892095267414260739);
void car_H_25(double *state, double *unused, double *out_7029499251014642103);
void car_h_24(double *state, double *unused, double *out_9191269563965419038);
void car_H_24(double *state, double *unused, double *out_3471573652212318967);
void car_h_30(double *state, double *unused, double *out_5944369277058775778);
void car_H_30(double *state, double *unused, double *out_7158838198157882173);
void car_h_26(double *state, double *unused, double *out_7500701528989930193);
void car_H_26(double *state, double *unused, double *out_7675741503820853289);
void car_h_27(double *state, double *unused, double *out_1088215833618585104);
void car_H_27(double *state, double *unused, double *out_9113142563751244532);
void car_h_29(double *state, double *unused, double *out_942696009397930133);
void car_H_29(double *state, double *unused, double *out_6648606853843489989);
void car_h_28(double *state, double *unused, double *out_2031725686520247539);
void car_H_28(double *state, double *unused, double *out_4684976582278163738);
void car_h_31(double *state, double *unused, double *out_1673376155541241692);
void car_H_31(double *state, double *unused, double *out_6998853289137681675);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}