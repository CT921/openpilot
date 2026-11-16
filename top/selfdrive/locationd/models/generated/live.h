#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void live_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_9(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_12(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_35(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_32(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_33(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_H(double *in_vec, double *out_8407973768981585941);
void live_err_fun(double *nom_x, double *delta_x, double *out_3809538825091607003);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_2234796900995103970);
void live_H_mod_fun(double *state, double *out_5916247595490968350);
void live_f_fun(double *state, double dt, double *out_2494489570055502755);
void live_F_fun(double *state, double dt, double *out_3987660329736070162);
void live_h_4(double *state, double *unused, double *out_2733065062335381572);
void live_H_4(double *state, double *unused, double *out_1581870475671522522);
void live_h_9(double *state, double *unused, double *out_7239910944337976037);
void live_H_9(double *state, double *unused, double *out_1823060122301113167);
void live_h_10(double *state, double *unused, double *out_1368765428601632870);
void live_H_10(double *state, double *unused, double *out_5453242189124688570);
void live_h_12(double *state, double *unused, double *out_1181802930096048610);
void live_H_12(double *state, double *unused, double *out_6601326883703484317);
void live_h_35(double *state, double *unused, double *out_4946987363960806421);
void live_H_35(double *state, double *unused, double *out_4948532533044129898);
void live_h_32(double *state, double *unused, double *out_2051563379975280761);
void live_H_32(double *state, double *unused, double *out_7696907168457809160);
void live_h_13(double *state, double *unused, double *out_6596564125924537673);
void live_H_13(double *state, double *unused, double *out_3330157645099586772);
void live_h_14(double *state, double *unused, double *out_7239910944337976037);
void live_H_14(double *state, double *unused, double *out_1823060122301113167);
void live_h_33(double *state, double *unused, double *out_2317934239341456921);
void live_H_33(double *state, double *unused, double *out_8099089537682987502);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}