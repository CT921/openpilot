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
void err_fun(double *nom_x, double *delta_x, double *out_4624415640507330488) {
   out_4624415640507330488[0] = delta_x[0] + nom_x[0];
   out_4624415640507330488[1] = delta_x[1] + nom_x[1];
   out_4624415640507330488[2] = delta_x[2] + nom_x[2];
   out_4624415640507330488[3] = delta_x[3] + nom_x[3];
   out_4624415640507330488[4] = delta_x[4] + nom_x[4];
   out_4624415640507330488[5] = delta_x[5] + nom_x[5];
   out_4624415640507330488[6] = delta_x[6] + nom_x[6];
   out_4624415640507330488[7] = delta_x[7] + nom_x[7];
   out_4624415640507330488[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_5296127735945035033) {
   out_5296127735945035033[0] = -nom_x[0] + true_x[0];
   out_5296127735945035033[1] = -nom_x[1] + true_x[1];
   out_5296127735945035033[2] = -nom_x[2] + true_x[2];
   out_5296127735945035033[3] = -nom_x[3] + true_x[3];
   out_5296127735945035033[4] = -nom_x[4] + true_x[4];
   out_5296127735945035033[5] = -nom_x[5] + true_x[5];
   out_5296127735945035033[6] = -nom_x[6] + true_x[6];
   out_5296127735945035033[7] = -nom_x[7] + true_x[7];
   out_5296127735945035033[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_1380935324346926016) {
   out_1380935324346926016[0] = 1.0;
   out_1380935324346926016[1] = 0.0;
   out_1380935324346926016[2] = 0.0;
   out_1380935324346926016[3] = 0.0;
   out_1380935324346926016[4] = 0.0;
   out_1380935324346926016[5] = 0.0;
   out_1380935324346926016[6] = 0.0;
   out_1380935324346926016[7] = 0.0;
   out_1380935324346926016[8] = 0.0;
   out_1380935324346926016[9] = 0.0;
   out_1380935324346926016[10] = 1.0;
   out_1380935324346926016[11] = 0.0;
   out_1380935324346926016[12] = 0.0;
   out_1380935324346926016[13] = 0.0;
   out_1380935324346926016[14] = 0.0;
   out_1380935324346926016[15] = 0.0;
   out_1380935324346926016[16] = 0.0;
   out_1380935324346926016[17] = 0.0;
   out_1380935324346926016[18] = 0.0;
   out_1380935324346926016[19] = 0.0;
   out_1380935324346926016[20] = 1.0;
   out_1380935324346926016[21] = 0.0;
   out_1380935324346926016[22] = 0.0;
   out_1380935324346926016[23] = 0.0;
   out_1380935324346926016[24] = 0.0;
   out_1380935324346926016[25] = 0.0;
   out_1380935324346926016[26] = 0.0;
   out_1380935324346926016[27] = 0.0;
   out_1380935324346926016[28] = 0.0;
   out_1380935324346926016[29] = 0.0;
   out_1380935324346926016[30] = 1.0;
   out_1380935324346926016[31] = 0.0;
   out_1380935324346926016[32] = 0.0;
   out_1380935324346926016[33] = 0.0;
   out_1380935324346926016[34] = 0.0;
   out_1380935324346926016[35] = 0.0;
   out_1380935324346926016[36] = 0.0;
   out_1380935324346926016[37] = 0.0;
   out_1380935324346926016[38] = 0.0;
   out_1380935324346926016[39] = 0.0;
   out_1380935324346926016[40] = 1.0;
   out_1380935324346926016[41] = 0.0;
   out_1380935324346926016[42] = 0.0;
   out_1380935324346926016[43] = 0.0;
   out_1380935324346926016[44] = 0.0;
   out_1380935324346926016[45] = 0.0;
   out_1380935324346926016[46] = 0.0;
   out_1380935324346926016[47] = 0.0;
   out_1380935324346926016[48] = 0.0;
   out_1380935324346926016[49] = 0.0;
   out_1380935324346926016[50] = 1.0;
   out_1380935324346926016[51] = 0.0;
   out_1380935324346926016[52] = 0.0;
   out_1380935324346926016[53] = 0.0;
   out_1380935324346926016[54] = 0.0;
   out_1380935324346926016[55] = 0.0;
   out_1380935324346926016[56] = 0.0;
   out_1380935324346926016[57] = 0.0;
   out_1380935324346926016[58] = 0.0;
   out_1380935324346926016[59] = 0.0;
   out_1380935324346926016[60] = 1.0;
   out_1380935324346926016[61] = 0.0;
   out_1380935324346926016[62] = 0.0;
   out_1380935324346926016[63] = 0.0;
   out_1380935324346926016[64] = 0.0;
   out_1380935324346926016[65] = 0.0;
   out_1380935324346926016[66] = 0.0;
   out_1380935324346926016[67] = 0.0;
   out_1380935324346926016[68] = 0.0;
   out_1380935324346926016[69] = 0.0;
   out_1380935324346926016[70] = 1.0;
   out_1380935324346926016[71] = 0.0;
   out_1380935324346926016[72] = 0.0;
   out_1380935324346926016[73] = 0.0;
   out_1380935324346926016[74] = 0.0;
   out_1380935324346926016[75] = 0.0;
   out_1380935324346926016[76] = 0.0;
   out_1380935324346926016[77] = 0.0;
   out_1380935324346926016[78] = 0.0;
   out_1380935324346926016[79] = 0.0;
   out_1380935324346926016[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_961222297369104125) {
   out_961222297369104125[0] = state[0];
   out_961222297369104125[1] = state[1];
   out_961222297369104125[2] = state[2];
   out_961222297369104125[3] = state[3];
   out_961222297369104125[4] = state[4];
   out_961222297369104125[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_961222297369104125[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_961222297369104125[7] = state[7];
   out_961222297369104125[8] = state[8];
}
void F_fun(double *state, double dt, double *out_2870206942681480216) {
   out_2870206942681480216[0] = 1;
   out_2870206942681480216[1] = 0;
   out_2870206942681480216[2] = 0;
   out_2870206942681480216[3] = 0;
   out_2870206942681480216[4] = 0;
   out_2870206942681480216[5] = 0;
   out_2870206942681480216[6] = 0;
   out_2870206942681480216[7] = 0;
   out_2870206942681480216[8] = 0;
   out_2870206942681480216[9] = 0;
   out_2870206942681480216[10] = 1;
   out_2870206942681480216[11] = 0;
   out_2870206942681480216[12] = 0;
   out_2870206942681480216[13] = 0;
   out_2870206942681480216[14] = 0;
   out_2870206942681480216[15] = 0;
   out_2870206942681480216[16] = 0;
   out_2870206942681480216[17] = 0;
   out_2870206942681480216[18] = 0;
   out_2870206942681480216[19] = 0;
   out_2870206942681480216[20] = 1;
   out_2870206942681480216[21] = 0;
   out_2870206942681480216[22] = 0;
   out_2870206942681480216[23] = 0;
   out_2870206942681480216[24] = 0;
   out_2870206942681480216[25] = 0;
   out_2870206942681480216[26] = 0;
   out_2870206942681480216[27] = 0;
   out_2870206942681480216[28] = 0;
   out_2870206942681480216[29] = 0;
   out_2870206942681480216[30] = 1;
   out_2870206942681480216[31] = 0;
   out_2870206942681480216[32] = 0;
   out_2870206942681480216[33] = 0;
   out_2870206942681480216[34] = 0;
   out_2870206942681480216[35] = 0;
   out_2870206942681480216[36] = 0;
   out_2870206942681480216[37] = 0;
   out_2870206942681480216[38] = 0;
   out_2870206942681480216[39] = 0;
   out_2870206942681480216[40] = 1;
   out_2870206942681480216[41] = 0;
   out_2870206942681480216[42] = 0;
   out_2870206942681480216[43] = 0;
   out_2870206942681480216[44] = 0;
   out_2870206942681480216[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_2870206942681480216[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_2870206942681480216[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_2870206942681480216[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_2870206942681480216[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_2870206942681480216[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_2870206942681480216[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_2870206942681480216[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_2870206942681480216[53] = -9.8100000000000005*dt;
   out_2870206942681480216[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_2870206942681480216[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_2870206942681480216[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2870206942681480216[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2870206942681480216[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_2870206942681480216[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_2870206942681480216[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_2870206942681480216[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2870206942681480216[62] = 0;
   out_2870206942681480216[63] = 0;
   out_2870206942681480216[64] = 0;
   out_2870206942681480216[65] = 0;
   out_2870206942681480216[66] = 0;
   out_2870206942681480216[67] = 0;
   out_2870206942681480216[68] = 0;
   out_2870206942681480216[69] = 0;
   out_2870206942681480216[70] = 1;
   out_2870206942681480216[71] = 0;
   out_2870206942681480216[72] = 0;
   out_2870206942681480216[73] = 0;
   out_2870206942681480216[74] = 0;
   out_2870206942681480216[75] = 0;
   out_2870206942681480216[76] = 0;
   out_2870206942681480216[77] = 0;
   out_2870206942681480216[78] = 0;
   out_2870206942681480216[79] = 0;
   out_2870206942681480216[80] = 1;
}
void h_25(double *state, double *unused, double *out_3496292854128936459) {
   out_3496292854128936459[0] = state[6];
}
void H_25(double *state, double *unused, double *out_4903915974336417693) {
   out_4903915974336417693[0] = 0;
   out_4903915974336417693[1] = 0;
   out_4903915974336417693[2] = 0;
   out_4903915974336417693[3] = 0;
   out_4903915974336417693[4] = 0;
   out_4903915974336417693[5] = 0;
   out_4903915974336417693[6] = 1;
   out_4903915974336417693[7] = 0;
   out_4903915974336417693[8] = 0;
}
void h_24(double *state, double *unused, double *out_857869959165743186) {
   out_857869959165743186[0] = state[4];
   out_857869959165743186[1] = state[5];
}
void H_24(double *state, double *unused, double *out_83594469680429430) {
   out_83594469680429430[0] = 0;
   out_83594469680429430[1] = 0;
   out_83594469680429430[2] = 0;
   out_83594469680429430[3] = 0;
   out_83594469680429430[4] = 1;
   out_83594469680429430[5] = 0;
   out_83594469680429430[6] = 0;
   out_83594469680429430[7] = 0;
   out_83594469680429430[8] = 0;
   out_83594469680429430[9] = 0;
   out_83594469680429430[10] = 0;
   out_83594469680429430[11] = 0;
   out_83594469680429430[12] = 0;
   out_83594469680429430[13] = 0;
   out_83594469680429430[14] = 1;
   out_83594469680429430[15] = 0;
   out_83594469680429430[16] = 0;
   out_83594469680429430[17] = 0;
}
void h_30(double *state, double *unused, double *out_4384979796625112726) {
   out_4384979796625112726[0] = state[4];
}
void H_30(double *state, double *unused, double *out_4774577027193177623) {
   out_4774577027193177623[0] = 0;
   out_4774577027193177623[1] = 0;
   out_4774577027193177623[2] = 0;
   out_4774577027193177623[3] = 0;
   out_4774577027193177623[4] = 1;
   out_4774577027193177623[5] = 0;
   out_4774577027193177623[6] = 0;
   out_4774577027193177623[7] = 0;
   out_4774577027193177623[8] = 0;
}
void h_26(double *state, double *unused, double *out_5929008340093117738) {
   out_5929008340093117738[0] = state[7];
}
void H_26(double *state, double *unused, double *out_1162412655462361469) {
   out_1162412655462361469[0] = 0;
   out_1162412655462361469[1] = 0;
   out_1162412655462361469[2] = 0;
   out_1162412655462361469[3] = 0;
   out_1162412655462361469[4] = 0;
   out_1162412655462361469[5] = 0;
   out_1162412655462361469[6] = 0;
   out_1162412655462361469[7] = 1;
   out_1162412655462361469[8] = 0;
}
void h_27(double *state, double *unused, double *out_2892976969452458942) {
   out_2892976969452458942[0] = state[3];
}
void H_27(double *state, double *unused, double *out_2599813715392752712) {
   out_2599813715392752712[0] = 0;
   out_2599813715392752712[1] = 0;
   out_2599813715392752712[2] = 0;
   out_2599813715392752712[3] = 1;
   out_2599813715392752712[4] = 0;
   out_2599813715392752712[5] = 0;
   out_2599813715392752712[6] = 0;
   out_2599813715392752712[7] = 0;
   out_2599813715392752712[8] = 0;
}
void h_29(double *state, double *unused, double *out_2617782907167953053) {
   out_2617782907167953053[0] = state[1];
}
void H_29(double *state, double *unused, double *out_5284808371507569807) {
   out_5284808371507569807[0] = 0;
   out_5284808371507569807[1] = 1;
   out_5284808371507569807[2] = 0;
   out_5284808371507569807[3] = 0;
   out_5284808371507569807[4] = 0;
   out_5284808371507569807[5] = 0;
   out_5284808371507569807[6] = 0;
   out_5284808371507569807[7] = 0;
   out_5284808371507569807[8] = 0;
}
void h_28(double *state, double *unused, double *out_3209542466280735448) {
   out_3209542466280735448[0] = state[0];
}
void H_28(double *state, double *unused, double *out_7248438643072896058) {
   out_7248438643072896058[0] = 1;
   out_7248438643072896058[1] = 0;
   out_7248438643072896058[2] = 0;
   out_7248438643072896058[3] = 0;
   out_7248438643072896058[4] = 0;
   out_7248438643072896058[5] = 0;
   out_7248438643072896058[6] = 0;
   out_7248438643072896058[7] = 0;
   out_7248438643072896058[8] = 0;
}
void h_31(double *state, double *unused, double *out_7133742382382958809) {
   out_7133742382382958809[0] = state[8];
}
void H_31(double *state, double *unused, double *out_4934561936213378121) {
   out_4934561936213378121[0] = 0;
   out_4934561936213378121[1] = 0;
   out_4934561936213378121[2] = 0;
   out_4934561936213378121[3] = 0;
   out_4934561936213378121[4] = 0;
   out_4934561936213378121[5] = 0;
   out_4934561936213378121[6] = 0;
   out_4934561936213378121[7] = 0;
   out_4934561936213378121[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_4624415640507330488) {
  err_fun(nom_x, delta_x, out_4624415640507330488);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_5296127735945035033) {
  inv_err_fun(nom_x, true_x, out_5296127735945035033);
}
void car_H_mod_fun(double *state, double *out_1380935324346926016) {
  H_mod_fun(state, out_1380935324346926016);
}
void car_f_fun(double *state, double dt, double *out_961222297369104125) {
  f_fun(state,  dt, out_961222297369104125);
}
void car_F_fun(double *state, double dt, double *out_2870206942681480216) {
  F_fun(state,  dt, out_2870206942681480216);
}
void car_h_25(double *state, double *unused, double *out_3496292854128936459) {
  h_25(state, unused, out_3496292854128936459);
}
void car_H_25(double *state, double *unused, double *out_4903915974336417693) {
  H_25(state, unused, out_4903915974336417693);
}
void car_h_24(double *state, double *unused, double *out_857869959165743186) {
  h_24(state, unused, out_857869959165743186);
}
void car_H_24(double *state, double *unused, double *out_83594469680429430) {
  H_24(state, unused, out_83594469680429430);
}
void car_h_30(double *state, double *unused, double *out_4384979796625112726) {
  h_30(state, unused, out_4384979796625112726);
}
void car_H_30(double *state, double *unused, double *out_4774577027193177623) {
  H_30(state, unused, out_4774577027193177623);
}
void car_h_26(double *state, double *unused, double *out_5929008340093117738) {
  h_26(state, unused, out_5929008340093117738);
}
void car_H_26(double *state, double *unused, double *out_1162412655462361469) {
  H_26(state, unused, out_1162412655462361469);
}
void car_h_27(double *state, double *unused, double *out_2892976969452458942) {
  h_27(state, unused, out_2892976969452458942);
}
void car_H_27(double *state, double *unused, double *out_2599813715392752712) {
  H_27(state, unused, out_2599813715392752712);
}
void car_h_29(double *state, double *unused, double *out_2617782907167953053) {
  h_29(state, unused, out_2617782907167953053);
}
void car_H_29(double *state, double *unused, double *out_5284808371507569807) {
  H_29(state, unused, out_5284808371507569807);
}
void car_h_28(double *state, double *unused, double *out_3209542466280735448) {
  h_28(state, unused, out_3209542466280735448);
}
void car_H_28(double *state, double *unused, double *out_7248438643072896058) {
  H_28(state, unused, out_7248438643072896058);
}
void car_h_31(double *state, double *unused, double *out_7133742382382958809) {
  h_31(state, unused, out_7133742382382958809);
}
void car_H_31(double *state, double *unused, double *out_4934561936213378121) {
  H_31(state, unused, out_4934561936213378121);
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
