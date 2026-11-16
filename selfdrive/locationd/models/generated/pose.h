#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_4389229945795658064);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_635646804366371573);
void pose_H_mod_fun(double *state, double *out_1592105825884835830);
void pose_f_fun(double *state, double dt, double *out_5328593654681060988);
void pose_F_fun(double *state, double dt, double *out_1459903262957102476);
void pose_h_4(double *state, double *unused, double *out_1573274557216908386);
void pose_H_4(double *state, double *unused, double *out_7440187632636794551);
void pose_h_10(double *state, double *unused, double *out_469758277648148749);
void pose_H_10(double *state, double *unused, double *out_5199566927053226908);
void pose_h_13(double *state, double *unused, double *out_29074149891970277);
void pose_H_13(double *state, double *unused, double *out_4227913807304461750);
void pose_h_14(double *state, double *unused, double *out_3272782566418947268);
void pose_H_14(double *state, double *unused, double *out_3476946776297310022);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}