#include "car.h"

namespace {
#define DIM 9
#define EDIM 9
#define MEDIM 9
typedef void (*Hfun)(double *, double *, double *);

double mass;

void set_mass(double x){ mass = x;}

double rotational_inertia;

void set_rotational_inertia(double x){ rotational_inertia = x;}

double center_to_front;

void set_center_to_front(double x){ center_to_front = x;}

double center_to_rear;

void set_center_to_rear(double x){ center_to_rear = x;}

double stiffness_front;

void set_stiffness_front(double x){ stiffness_front = x;}

double stiffness_rear;

void set_stiffness_rear(double x){ stiffness_rear = x;}
const static double MAHA_THRESH_25 = 3.8414588206941227;
const static double MAHA_THRESH_24 = 5.991464547107981;
const static double MAHA_THRESH_30 = 3.8414588206941227;
const static double MAHA_THRESH_26 = 3.8414588206941227;
const static double MAHA_THRESH_27 = 3.8414588206941227;
const static double MAHA_THRESH_29 = 3.8414588206941227;
const static double MAHA_THRESH_28 = 3.8414588206941227;
const static double MAHA_THRESH_31 = 3.8414588206941227;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_1891774851159901983) {
   out_1891774851159901983[0] = delta_x[0] + nom_x[0];
   out_1891774851159901983[1] = delta_x[1] + nom_x[1];
   out_1891774851159901983[2] = delta_x[2] + nom_x[2];
   out_1891774851159901983[3] = delta_x[3] + nom_x[3];
   out_1891774851159901983[4] = delta_x[4] + nom_x[4];
   out_1891774851159901983[5] = delta_x[5] + nom_x[5];
   out_1891774851159901983[6] = delta_x[6] + nom_x[6];
   out_1891774851159901983[7] = delta_x[7] + nom_x[7];
   out_1891774851159901983[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_5837655934759956642) {
   out_5837655934759956642[0] = -nom_x[0] + true_x[0];
   out_5837655934759956642[1] = -nom_x[1] + true_x[1];
   out_5837655934759956642[2] = -nom_x[2] + true_x[2];
   out_5837655934759956642[3] = -nom_x[3] + true_x[3];
   out_5837655934759956642[4] = -nom_x[4] + true_x[4];
   out_5837655934759956642[5] = -nom_x[5] + true_x[5];
   out_5837655934759956642[6] = -nom_x[6] + true_x[6];
   out_5837655934759956642[7] = -nom_x[7] + true_x[7];
   out_5837655934759956642[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_7894264172705417836) {
   out_7894264172705417836[0] = 1.0;
   out_7894264172705417836[1] = 0.0;
   out_7894264172705417836[2] = 0.0;
   out_7894264172705417836[3] = 0.0;
   out_7894264172705417836[4] = 0.0;
   out_7894264172705417836[5] = 0.0;
   out_7894264172705417836[6] = 0.0;
   out_7894264172705417836[7] = 0.0;
   out_7894264172705417836[8] = 0.0;
   out_7894264172705417836[9] = 0.0;
   out_7894264172705417836[10] = 1.0;
   out_7894264172705417836[11] = 0.0;
   out_7894264172705417836[12] = 0.0;
   out_7894264172705417836[13] = 0.0;
   out_7894264172705417836[14] = 0.0;
   out_7894264172705417836[15] = 0.0;
   out_7894264172705417836[16] = 0.0;
   out_7894264172705417836[17] = 0.0;
   out_7894264172705417836[18] = 0.0;
   out_7894264172705417836[19] = 0.0;
   out_7894264172705417836[20] = 1.0;
   out_7894264172705417836[21] = 0.0;
   out_7894264172705417836[22] = 0.0;
   out_7894264172705417836[23] = 0.0;
   out_7894264172705417836[24] = 0.0;
   out_7894264172705417836[25] = 0.0;
   out_7894264172705417836[26] = 0.0;
   out_7894264172705417836[27] = 0.0;
   out_7894264172705417836[28] = 0.0;
   out_7894264172705417836[29] = 0.0;
   out_7894264172705417836[30] = 1.0;
   out_7894264172705417836[31] = 0.0;
   out_7894264172705417836[32] = 0.0;
   out_7894264172705417836[33] = 0.0;
   out_7894264172705417836[34] = 0.0;
   out_7894264172705417836[35] = 0.0;
   out_7894264172705417836[36] = 0.0;
   out_7894264172705417836[37] = 0.0;
   out_7894264172705417836[38] = 0.0;
   out_7894264172705417836[39] = 0.0;
   out_7894264172705417836[40] = 1.0;
   out_7894264172705417836[41] = 0.0;
   out_7894264172705417836[42] = 0.0;
   out_7894264172705417836[43] = 0.0;
   out_7894264172705417836[44] = 0.0;
   out_7894264172705417836[45] = 0.0;
   out_7894264172705417836[46] = 0.0;
   out_7894264172705417836[47] = 0.0;
   out_7894264172705417836[48] = 0.0;
   out_7894264172705417836[49] = 0.0;
   out_7894264172705417836[50] = 1.0;
   out_7894264172705417836[51] = 0.0;
   out_7894264172705417836[52] = 0.0;
   out_7894264172705417836[53] = 0.0;
   out_7894264172705417836[54] = 0.0;
   out_7894264172705417836[55] = 0.0;
   out_7894264172705417836[56] = 0.0;
   out_7894264172705417836[57] = 0.0;
   out_7894264172705417836[58] = 0.0;
   out_7894264172705417836[59] = 0.0;
   out_7894264172705417836[60] = 1.0;
   out_7894264172705417836[61] = 0.0;
   out_7894264172705417836[62] = 0.0;
   out_7894264172705417836[63] = 0.0;
   out_7894264172705417836[64] = 0.0;
   out_7894264172705417836[65] = 0.0;
   out_7894264172705417836[66] = 0.0;
   out_7894264172705417836[67] = 0.0;
   out_7894264172705417836[68] = 0.0;
   out_7894264172705417836[69] = 0.0;
   out_7894264172705417836[70] = 1.0;
   out_7894264172705417836[71] = 0.0;
   out_7894264172705417836[72] = 0.0;
   out_7894264172705417836[73] = 0.0;
   out_7894264172705417836[74] = 0.0;
   out_7894264172705417836[75] = 0.0;
   out_7894264172705417836[76] = 0.0;
   out_7894264172705417836[77] = 0.0;
   out_7894264172705417836[78] = 0.0;
   out_7894264172705417836[79] = 0.0;
   out_7894264172705417836[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_8559630220606310989) {
   out_8559630220606310989[0] = state[0];
   out_8559630220606310989[1] = state[1];
   out_8559630220606310989[2] = state[2];
   out_8559630220606310989[3] = state[3];
   out_8559630220606310989[4] = state[4];
   out_8559630220606310989[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_8559630220606310989[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_8559630220606310989[7] = state[7];
   out_8559630220606310989[8] = state[8];
}
void F_fun(double *state, double dt, double *out_47845048400386193) {
   out_47845048400386193[0] = 1;
   out_47845048400386193[1] = 0;
   out_47845048400386193[2] = 0;
   out_47845048400386193[3] = 0;
   out_47845048400386193[4] = 0;
   out_47845048400386193[5] = 0;
   out_47845048400386193[6] = 0;
   out_47845048400386193[7] = 0;
   out_47845048400386193[8] = 0;
   out_47845048400386193[9] = 0;
   out_47845048400386193[10] = 1;
   out_47845048400386193[11] = 0;
   out_47845048400386193[12] = 0;
   out_47845048400386193[13] = 0;
   out_47845048400386193[14] = 0;
   out_47845048400386193[15] = 0;
   out_47845048400386193[16] = 0;
   out_47845048400386193[17] = 0;
   out_47845048400386193[18] = 0;
   out_47845048400386193[19] = 0;
   out_47845048400386193[20] = 1;
   out_47845048400386193[21] = 0;
   out_47845048400386193[22] = 0;
   out_47845048400386193[23] = 0;
   out_47845048400386193[24] = 0;
   out_47845048400386193[25] = 0;
   out_47845048400386193[26] = 0;
   out_47845048400386193[27] = 0;
   out_47845048400386193[28] = 0;
   out_47845048400386193[29] = 0;
   out_47845048400386193[30] = 1;
   out_47845048400386193[31] = 0;
   out_47845048400386193[32] = 0;
   out_47845048400386193[33] = 0;
   out_47845048400386193[34] = 0;
   out_47845048400386193[35] = 0;
   out_47845048400386193[36] = 0;
   out_47845048400386193[37] = 0;
   out_47845048400386193[38] = 0;
   out_47845048400386193[39] = 0;
   out_47845048400386193[40] = 1;
   out_47845048400386193[41] = 0;
   out_47845048400386193[42] = 0;
   out_47845048400386193[43] = 0;
   out_47845048400386193[44] = 0;
   out_47845048400386193[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_47845048400386193[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_47845048400386193[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_47845048400386193[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_47845048400386193[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_47845048400386193[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_47845048400386193[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_47845048400386193[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_47845048400386193[53] = -9.8100000000000005*dt;
   out_47845048400386193[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_47845048400386193[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_47845048400386193[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_47845048400386193[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_47845048400386193[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_47845048400386193[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_47845048400386193[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_47845048400386193[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_47845048400386193[62] = 0;
   out_47845048400386193[63] = 0;
   out_47845048400386193[64] = 0;
   out_47845048400386193[65] = 0;
   out_47845048400386193[66] = 0;
   out_47845048400386193[67] = 0;
   out_47845048400386193[68] = 0;
   out_47845048400386193[69] = 0;
   out_47845048400386193[70] = 1;
   out_47845048400386193[71] = 0;
   out_47845048400386193[72] = 0;
   out_47845048400386193[73] = 0;
   out_47845048400386193[74] = 0;
   out_47845048400386193[75] = 0;
   out_47845048400386193[76] = 0;
   out_47845048400386193[77] = 0;
   out_47845048400386193[78] = 0;
   out_47845048400386193[79] = 0;
   out_47845048400386193[80] = 1;
}
void h_25(double *state, double *unused, double *out_8892095267414260739) {
   out_8892095267414260739[0] = state[6];
}
void H_25(double *state, double *unused, double *out_7029499251014642103) {
   out_7029499251014642103[0] = 0;
   out_7029499251014642103[1] = 0;
   out_7029499251014642103[2] = 0;
   out_7029499251014642103[3] = 0;
   out_7029499251014642103[4] = 0;
   out_7029499251014642103[5] = 0;
   out_7029499251014642103[6] = 1;
   out_7029499251014642103[7] = 0;
   out_7029499251014642103[8] = 0;
}
void h_24(double *state, double *unused, double *out_9191269563965419038) {
   out_9191269563965419038[0] = state[4];
   out_9191269563965419038[1] = state[5];
}
void H_24(double *state, double *unused, double *out_3471573652212318967) {
   out_3471573652212318967[0] = 0;
   out_3471573652212318967[1] = 0;
   out_3471573652212318967[2] = 0;
   out_3471573652212318967[3] = 0;
   out_3471573652212318967[4] = 1;
   out_3471573652212318967[5] = 0;
   out_3471573652212318967[6] = 0;
   out_3471573652212318967[7] = 0;
   out_3471573652212318967[8] = 0;
   out_3471573652212318967[9] = 0;
   out_3471573652212318967[10] = 0;
   out_3471573652212318967[11] = 0;
   out_3471573652212318967[12] = 0;
   out_3471573652212318967[13] = 0;
   out_3471573652212318967[14] = 1;
   out_3471573652212318967[15] = 0;
   out_3471573652212318967[16] = 0;
   out_3471573652212318967[17] = 0;
}
void h_30(double *state, double *unused, double *out_5944369277058775778) {
   out_5944369277058775778[0] = state[4];
}
void H_30(double *state, double *unused, double *out_7158838198157882173) {
   out_7158838198157882173[0] = 0;
   out_7158838198157882173[1] = 0;
   out_7158838198157882173[2] = 0;
   out_7158838198157882173[3] = 0;
   out_7158838198157882173[4] = 1;
   out_7158838198157882173[5] = 0;
   out_7158838198157882173[6] = 0;
   out_7158838198157882173[7] = 0;
   out_7158838198157882173[8] = 0;
}
void h_26(double *state, double *unused, double *out_7500701528989930193) {
   out_7500701528989930193[0] = state[7];
}
void H_26(double *state, double *unused, double *out_7675741503820853289) {
   out_7675741503820853289[0] = 0;
   out_7675741503820853289[1] = 0;
   out_7675741503820853289[2] = 0;
   out_7675741503820853289[3] = 0;
   out_7675741503820853289[4] = 0;
   out_7675741503820853289[5] = 0;
   out_7675741503820853289[6] = 0;
   out_7675741503820853289[7] = 1;
   out_7675741503820853289[8] = 0;
}
void h_27(double *state, double *unused, double *out_1088215833618585104) {
   out_1088215833618585104[0] = state[3];
}
void H_27(double *state, double *unused, double *out_9113142563751244532) {
   out_9113142563751244532[0] = 0;
   out_9113142563751244532[1] = 0;
   out_9113142563751244532[2] = 0;
   out_9113142563751244532[3] = 1;
   out_9113142563751244532[4] = 0;
   out_9113142563751244532[5] = 0;
   out_9113142563751244532[6] = 0;
   out_9113142563751244532[7] = 0;
   out_9113142563751244532[8] = 0;
}
void h_29(double *state, double *unused, double *out_942696009397930133) {
   out_942696009397930133[0] = state[1];
}
void H_29(double *state, double *unused, double *out_6648606853843489989) {
   out_6648606853843489989[0] = 0;
   out_6648606853843489989[1] = 1;
   out_6648606853843489989[2] = 0;
   out_6648606853843489989[3] = 0;
   out_6648606853843489989[4] = 0;
   out_6648606853843489989[5] = 0;
   out_6648606853843489989[6] = 0;
   out_6648606853843489989[7] = 0;
   out_6648606853843489989[8] = 0;
}
void h_28(double *state, double *unused, double *out_2031725686520247539) {
   out_2031725686520247539[0] = state[0];
}
void H_28(double *state, double *unused, double *out_4684976582278163738) {
   out_4684976582278163738[0] = 1;
   out_4684976582278163738[1] = 0;
   out_4684976582278163738[2] = 0;
   out_4684976582278163738[3] = 0;
   out_4684976582278163738[4] = 0;
   out_4684976582278163738[5] = 0;
   out_4684976582278163738[6] = 0;
   out_4684976582278163738[7] = 0;
   out_4684976582278163738[8] = 0;
}
void h_31(double *state, double *unused, double *out_1673376155541241692) {
   out_1673376155541241692[0] = state[8];
}
void H_31(double *state, double *unused, double *out_6998853289137681675) {
   out_6998853289137681675[0] = 0;
   out_6998853289137681675[1] = 0;
   out_6998853289137681675[2] = 0;
   out_6998853289137681675[3] = 0;
   out_6998853289137681675[4] = 0;
   out_6998853289137681675[5] = 0;
   out_6998853289137681675[6] = 0;
   out_6998853289137681675[7] = 0;
   out_6998853289137681675[8] = 1;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_25, H_25, NULL, in_z, in_R, in_ea, MAHA_THRESH_25);
}
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_24, H_24, NULL, in_z, in_R, in_ea, MAHA_THRESH_24);
}
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_30, H_30, NULL, in_z, in_R, in_ea, MAHA_THRESH_30);
}
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_26, H_26, NULL, in_z, in_R, in_ea, MAHA_THRESH_26);
}
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_27, H_27, NULL, in_z, in_R, in_ea, MAHA_THRESH_27);
}
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_29, H_29, NULL, in_z, in_R, in_ea, MAHA_THRESH_29);
}
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_31, H_31, NULL, in_z, in_R, in_ea, MAHA_THRESH_31);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_1891774851159901983) {
  err_fun(nom_x, delta_x, out_1891774851159901983);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_5837655934759956642) {
  inv_err_fun(nom_x, true_x, out_5837655934759956642);
}
void car_H_mod_fun(double *state, double *out_7894264172705417836) {
  H_mod_fun(state, out_7894264172705417836);
}
void car_f_fun(double *state, double dt, double *out_8559630220606310989) {
  f_fun(state,  dt, out_8559630220606310989);
}
void car_F_fun(double *state, double dt, double *out_47845048400386193) {
  F_fun(state,  dt, out_47845048400386193);
}
void car_h_25(double *state, double *unused, double *out_8892095267414260739) {
  h_25(state, unused, out_8892095267414260739);
}
void car_H_25(double *state, double *unused, double *out_7029499251014642103) {
  H_25(state, unused, out_7029499251014642103);
}
void car_h_24(double *state, double *unused, double *out_9191269563965419038) {
  h_24(state, unused, out_9191269563965419038);
}
void car_H_24(double *state, double *unused, double *out_3471573652212318967) {
  H_24(state, unused, out_3471573652212318967);
}
void car_h_30(double *state, double *unused, double *out_5944369277058775778) {
  h_30(state, unused, out_5944369277058775778);
}
void car_H_30(double *state, double *unused, double *out_7158838198157882173) {
  H_30(state, unused, out_7158838198157882173);
}
void car_h_26(double *state, double *unused, double *out_7500701528989930193) {
  h_26(state, unused, out_7500701528989930193);
}
void car_H_26(double *state, double *unused, double *out_7675741503820853289) {
  H_26(state, unused, out_7675741503820853289);
}
void car_h_27(double *state, double *unused, double *out_1088215833618585104) {
  h_27(state, unused, out_1088215833618585104);
}
void car_H_27(double *state, double *unused, double *out_9113142563751244532) {
  H_27(state, unused, out_9113142563751244532);
}
void car_h_29(double *state, double *unused, double *out_942696009397930133) {
  h_29(state, unused, out_942696009397930133);
}
void car_H_29(double *state, double *unused, double *out_6648606853843489989) {
  H_29(state, unused, out_6648606853843489989);
}
void car_h_28(double *state, double *unused, double *out_2031725686520247539) {
  h_28(state, unused, out_2031725686520247539);
}
void car_H_28(double *state, double *unused, double *out_4684976582278163738) {
  H_28(state, unused, out_4684976582278163738);
}
void car_h_31(double *state, double *unused, double *out_1673376155541241692) {
  h_31(state, unused, out_1673376155541241692);
}
void car_H_31(double *state, double *unused, double *out_6998853289137681675) {
  H_31(state, unused, out_6998853289137681675);
}
void car_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
void car_set_mass(double x) {
  set_mass(x);
}
void car_set_rotational_inertia(double x) {
  set_rotational_inertia(x);
}
void car_set_center_to_front(double x) {
  set_center_to_front(x);
}
void car_set_center_to_rear(double x) {
  set_center_to_rear(x);
}
void car_set_stiffness_front(double x) {
  set_stiffness_front(x);
}
void car_set_stiffness_rear(double x) {
  set_stiffness_rear(x);
}
}

const EKF car = {
  .name = "car",
  .kinds = { 25, 24, 30, 26, 27, 29, 28, 31 },
  .feature_kinds = {  },
  .f_fun = car_f_fun,
  .F_fun = car_F_fun,
  .err_fun = car_err_fun,
  .inv_err_fun = car_inv_err_fun,
  .H_mod_fun = car_H_mod_fun,
  .predict = car_predict,
  .hs = {
    { 25, car_h_25 },
    { 24, car_h_24 },
    { 30, car_h_30 },
    { 26, car_h_26 },
    { 27, car_h_27 },
    { 29, car_h_29 },
    { 28, car_h_28 },
    { 31, car_h_31 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
    { 31, car_H_31 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
    { 31, car_update_31 },
  },
  .Hes = {
  },
  .sets = {
    { "mass", car_set_mass },
    { "rotational_inertia", car_set_rotational_inertia },
    { "center_to_front", car_set_center_to_front },
    { "center_to_rear", car_set_center_to_rear },
    { "stiffness_front", car_set_stiffness_front },
    { "stiffness_rear", car_set_stiffness_rear },
  },
  .extra_routines = {
  },
};

ekf_lib_init(car)
