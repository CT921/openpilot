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
void live_H(double *in_vec, double *out_6239140479139828737);
void live_err_fun(double *nom_x, double *delta_x, double *out_2081205929068557440);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_5629961785760558667);
void live_H_mod_fun(double *state, double *out_4883138952962026137);
void live_f_fun(double *state, double dt, double *out_3148196119809648138);
void live_F_fun(double *state, double dt, double *out_2447472429641852050);
void live_h_4(double *state, double *unused, double *out_3871374593867914418);
void live_H_4(double *state, double *unused, double *out_8270029089612570260);
void live_h_9(double *state, double *unused, double *out_7022257705660296594);
void live_H_9(double *state, double *unused, double *out_2889496048832533886);
void live_h_10(double *state, double *unused, double *out_203094704844003668);
void live_H_10(double *state, double *unused, double *out_7891453104249136367);
void live_h_12(double *state, double *unused, double *out_1227514842485447151);
void live_H_12(double *state, double *unused, double *out_1888770712569837264);
void live_h_35(double *state, double *unused, double *out_3359118208195020868);
void live_H_35(double *state, double *unused, double *out_235976361910482845);
void live_h_32(double *state, double *unused, double *out_2285162697753726843);
void live_H_32(double *state, double *unused, double *out_8454892436435273582);
void live_h_13(double *state, double *unused, double *out_6886689405908088075);
void live_H_13(double *state, double *unused, double *out_1456090936481578628);
void live_h_14(double *state, double *unused, double *out_7022257705660296594);
void live_H_14(double *state, double *unused, double *out_2889496048832533886);
void live_h_33(double *state, double *unused, double *out_4383520330870992836);
void live_H_33(double *state, double *unused, double *out_3386533366549340449);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}