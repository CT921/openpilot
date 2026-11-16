#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_2330460961423581716);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_6665394572644059121);
void pose_H_mod_fun(double *state, double *out_132265286258857251);
void pose_f_fun(double *state, double dt, double *out_8603722381005829308);
void pose_F_fun(double *state, double dt, double *out_2009381636234601862);
void pose_h_4(double *state, double *unused, double *out_6414518568457466023);
void pose_H_4(double *state, double *unused, double *out_8381063530603800607);
void pose_h_10(double *state, double *unused, double *out_1500850275664504396);
void pose_H_10(double *state, double *unused, double *out_4230206924188573634);
void pose_h_13(double *state, double *unused, double *out_2580468438894152282);
void pose_H_13(double *state, double *unused, double *out_6853406717773418208);
void pose_h_14(double *state, double *unused, double *out_952452624527645277);
void pose_H_14(double *state, double *unused, double *out_6102439686766266480);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}