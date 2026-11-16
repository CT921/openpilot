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
void err_fun(double *nom_x, double *delta_x, double *out_1941320000308867080) {
   out_1941320000308867080[0] = delta_x[0] + nom_x[0];
   out_1941320000308867080[1] = delta_x[1] + nom_x[1];
   out_1941320000308867080[2] = delta_x[2] + nom_x[2];
   out_1941320000308867080[3] = delta_x[3] + nom_x[3];
   out_1941320000308867080[4] = delta_x[4] + nom_x[4];
   out_1941320000308867080[5] = delta_x[5] + nom_x[5];
   out_1941320000308867080[6] = delta_x[6] + nom_x[6];
   out_1941320000308867080[7] = delta_x[7] + nom_x[7];
   out_1941320000308867080[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_8564590531858914382) {
   out_8564590531858914382[0] = -nom_x[0] + true_x[0];
   out_8564590531858914382[1] = -nom_x[1] + true_x[1];
   out_8564590531858914382[2] = -nom_x[2] + true_x[2];
   out_8564590531858914382[3] = -nom_x[3] + true_x[3];
   out_8564590531858914382[4] = -nom_x[4] + true_x[4];
   out_8564590531858914382[5] = -nom_x[5] + true_x[5];
   out_8564590531858914382[6] = -nom_x[6] + true_x[6];
   out_8564590531858914382[7] = -nom_x[7] + true_x[7];
   out_8564590531858914382[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_1707414104968884861) {
   out_1707414104968884861[0] = 1.0;
   out_1707414104968884861[1] = 0.0;
   out_1707414104968884861[2] = 0.0;
   out_1707414104968884861[3] = 0.0;
   out_1707414104968884861[4] = 0.0;
   out_1707414104968884861[5] = 0.0;
   out_1707414104968884861[6] = 0.0;
   out_1707414104968884861[7] = 0.0;
   out_1707414104968884861[8] = 0.0;
   out_1707414104968884861[9] = 0.0;
   out_1707414104968884861[10] = 1.0;
   out_1707414104968884861[11] = 0.0;
   out_1707414104968884861[12] = 0.0;
   out_1707414104968884861[13] = 0.0;
   out_1707414104968884861[14] = 0.0;
   out_1707414104968884861[15] = 0.0;
   out_1707414104968884861[16] = 0.0;
   out_1707414104968884861[17] = 0.0;
   out_1707414104968884861[18] = 0.0;
   out_1707414104968884861[19] = 0.0;
   out_1707414104968884861[20] = 1.0;
   out_1707414104968884861[21] = 0.0;
   out_1707414104968884861[22] = 0.0;
   out_1707414104968884861[23] = 0.0;
   out_1707414104968884861[24] = 0.0;
   out_1707414104968884861[25] = 0.0;
   out_1707414104968884861[26] = 0.0;
   out_1707414104968884861[27] = 0.0;
   out_1707414104968884861[28] = 0.0;
   out_1707414104968884861[29] = 0.0;
   out_1707414104968884861[30] = 1.0;
   out_1707414104968884861[31] = 0.0;
   out_1707414104968884861[32] = 0.0;
   out_1707414104968884861[33] = 0.0;
   out_1707414104968884861[34] = 0.0;
   out_1707414104968884861[35] = 0.0;
   out_1707414104968884861[36] = 0.0;
   out_1707414104968884861[37] = 0.0;
   out_1707414104968884861[38] = 0.0;
   out_1707414104968884861[39] = 0.0;
   out_1707414104968884861[40] = 1.0;
   out_1707414104968884861[41] = 0.0;
   out_1707414104968884861[42] = 0.0;
   out_1707414104968884861[43] = 0.0;
   out_1707414104968884861[44] = 0.0;
   out_1707414104968884861[45] = 0.0;
   out_1707414104968884861[46] = 0.0;
   out_1707414104968884861[47] = 0.0;
   out_1707414104968884861[48] = 0.0;
   out_1707414104968884861[49] = 0.0;
   out_1707414104968884861[50] = 1.0;
   out_1707414104968884861[51] = 0.0;
   out_1707414104968884861[52] = 0.0;
   out_1707414104968884861[53] = 0.0;
   out_1707414104968884861[54] = 0.0;
   out_1707414104968884861[55] = 0.0;
   out_1707414104968884861[56] = 0.0;
   out_1707414104968884861[57] = 0.0;
   out_1707414104968884861[58] = 0.0;
   out_1707414104968884861[59] = 0.0;
   out_1707414104968884861[60] = 1.0;
   out_1707414104968884861[61] = 0.0;
   out_1707414104968884861[62] = 0.0;
   out_1707414104968884861[63] = 0.0;
   out_1707414104968884861[64] = 0.0;
   out_1707414104968884861[65] = 0.0;
   out_1707414104968884861[66] = 0.0;
   out_1707414104968884861[67] = 0.0;
   out_1707414104968884861[68] = 0.0;
   out_1707414104968884861[69] = 0.0;
   out_1707414104968884861[70] = 1.0;
   out_1707414104968884861[71] = 0.0;
   out_1707414104968884861[72] = 0.0;
   out_1707414104968884861[73] = 0.0;
   out_1707414104968884861[74] = 0.0;
   out_1707414104968884861[75] = 0.0;
   out_1707414104968884861[76] = 0.0;
   out_1707414104968884861[77] = 0.0;
   out_1707414104968884861[78] = 0.0;
   out_1707414104968884861[79] = 0.0;
   out_1707414104968884861[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_2427056617940528332) {
   out_2427056617940528332[0] = state[0];
   out_2427056617940528332[1] = state[1];
   out_2427056617940528332[2] = state[2];
   out_2427056617940528332[3] = state[3];
   out_2427056617940528332[4] = state[4];
   out_2427056617940528332[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_2427056617940528332[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_2427056617940528332[7] = state[7];
   out_2427056617940528332[8] = state[8];
}
void F_fun(double *state, double dt, double *out_1481622780511648799) {
   out_1481622780511648799[0] = 1;
   out_1481622780511648799[1] = 0;
   out_1481622780511648799[2] = 0;
   out_1481622780511648799[3] = 0;
   out_1481622780511648799[4] = 0;
   out_1481622780511648799[5] = 0;
   out_1481622780511648799[6] = 0;
   out_1481622780511648799[7] = 0;
   out_1481622780511648799[8] = 0;
   out_1481622780511648799[9] = 0;
   out_1481622780511648799[10] = 1;
   out_1481622780511648799[11] = 0;
   out_1481622780511648799[12] = 0;
   out_1481622780511648799[13] = 0;
   out_1481622780511648799[14] = 0;
   out_1481622780511648799[15] = 0;
   out_1481622780511648799[16] = 0;
   out_1481622780511648799[17] = 0;
   out_1481622780511648799[18] = 0;
   out_1481622780511648799[19] = 0;
   out_1481622780511648799[20] = 1;
   out_1481622780511648799[21] = 0;
   out_1481622780511648799[22] = 0;
   out_1481622780511648799[23] = 0;
   out_1481622780511648799[24] = 0;
   out_1481622780511648799[25] = 0;
   out_1481622780511648799[26] = 0;
   out_1481622780511648799[27] = 0;
   out_1481622780511648799[28] = 0;
   out_1481622780511648799[29] = 0;
   out_1481622780511648799[30] = 1;
   out_1481622780511648799[31] = 0;
   out_1481622780511648799[32] = 0;
   out_1481622780511648799[33] = 0;
   out_1481622780511648799[34] = 0;
   out_1481622780511648799[35] = 0;
   out_1481622780511648799[36] = 0;
   out_1481622780511648799[37] = 0;
   out_1481622780511648799[38] = 0;
   out_1481622780511648799[39] = 0;
   out_1481622780511648799[40] = 1;
   out_1481622780511648799[41] = 0;
   out_1481622780511648799[42] = 0;
   out_1481622780511648799[43] = 0;
   out_1481622780511648799[44] = 0;
   out_1481622780511648799[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_1481622780511648799[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_1481622780511648799[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_1481622780511648799[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_1481622780511648799[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_1481622780511648799[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_1481622780511648799[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_1481622780511648799[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_1481622780511648799[53] = -9.8100000000000005*dt;
   out_1481622780511648799[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_1481622780511648799[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_1481622780511648799[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1481622780511648799[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1481622780511648799[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_1481622780511648799[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_1481622780511648799[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_1481622780511648799[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1481622780511648799[62] = 0;
   out_1481622780511648799[63] = 0;
   out_1481622780511648799[64] = 0;
   out_1481622780511648799[65] = 0;
   out_1481622780511648799[66] = 0;
   out_1481622780511648799[67] = 0;
   out_1481622780511648799[68] = 0;
   out_1481622780511648799[69] = 0;
   out_1481622780511648799[70] = 1;
   out_1481622780511648799[71] = 0;
   out_1481622780511648799[72] = 0;
   out_1481622780511648799[73] = 0;
   out_1481622780511648799[74] = 0;
   out_1481622780511648799[75] = 0;
   out_1481622780511648799[76] = 0;
   out_1481622780511648799[77] = 0;
   out_1481622780511648799[78] = 0;
   out_1481622780511648799[79] = 0;
   out_1481622780511648799[80] = 1;
}
void h_25(double *state, double *unused, double *out_6533420610061618523) {
   out_6533420610061618523[0] = state[6];
}
void H_25(double *state, double *unused, double *out_5230394754958376538) {
   out_5230394754958376538[0] = 0;
   out_5230394754958376538[1] = 0;
   out_5230394754958376538[2] = 0;
   out_5230394754958376538[3] = 0;
   out_5230394754958376538[4] = 0;
   out_5230394754958376538[5] = 0;
   out_5230394754958376538[6] = 1;
   out_5230394754958376538[7] = 0;
   out_5230394754958376538[8] = 0;
}
void h_24(double *state, double *unused, double *out_7497968432055013731) {
   out_7497968432055013731[0] = state[4];
   out_7497968432055013731[1] = state[5];
}
void H_24(double *state, double *unused, double *out_6562143860642989321) {
   out_6562143860642989321[0] = 0;
   out_6562143860642989321[1] = 0;
   out_6562143860642989321[2] = 0;
   out_6562143860642989321[3] = 0;
   out_6562143860642989321[4] = 1;
   out_6562143860642989321[5] = 0;
   out_6562143860642989321[6] = 0;
   out_6562143860642989321[7] = 0;
   out_6562143860642989321[8] = 0;
   out_6562143860642989321[9] = 0;
   out_6562143860642989321[10] = 0;
   out_6562143860642989321[11] = 0;
   out_6562143860642989321[12] = 0;
   out_6562143860642989321[13] = 0;
   out_6562143860642989321[14] = 1;
   out_6562143860642989321[15] = 0;
   out_6562143860642989321[16] = 0;
   out_6562143860642989321[17] = 0;
}
void h_30(double *state, double *unused, double *out_377973414220958394) {
   out_377973414220958394[0] = state[4];
}
void H_30(double *state, double *unused, double *out_5101055807815136468) {
   out_5101055807815136468[0] = 0;
   out_5101055807815136468[1] = 0;
   out_5101055807815136468[2] = 0;
   out_5101055807815136468[3] = 0;
   out_5101055807815136468[4] = 1;
   out_5101055807815136468[5] = 0;
   out_5101055807815136468[6] = 0;
   out_5101055807815136468[7] = 0;
   out_5101055807815136468[8] = 0;
}
void h_26(double *state, double *unused, double *out_981289298897435911) {
   out_981289298897435911[0] = state[7];
}
void H_26(double *state, double *unused, double *out_1488891436084320314) {
   out_1488891436084320314[0] = 0;
   out_1488891436084320314[1] = 0;
   out_1488891436084320314[2] = 0;
   out_1488891436084320314[3] = 0;
   out_1488891436084320314[4] = 0;
   out_1488891436084320314[5] = 0;
   out_1488891436084320314[6] = 0;
   out_1488891436084320314[7] = 1;
   out_1488891436084320314[8] = 0;
}
void h_27(double *state, double *unused, double *out_9202755917876670014) {
   out_9202755917876670014[0] = state[3];
}
void H_27(double *state, double *unused, double *out_2926292496014711557) {
   out_2926292496014711557[0] = 0;
   out_2926292496014711557[1] = 0;
   out_2926292496014711557[2] = 0;
   out_2926292496014711557[3] = 1;
   out_2926292496014711557[4] = 0;
   out_2926292496014711557[5] = 0;
   out_2926292496014711557[6] = 0;
   out_2926292496014711557[7] = 0;
   out_2926292496014711557[8] = 0;
}
void h_29(double *state, double *unused, double *out_1321483267122620829) {
   out_1321483267122620829[0] = state[1];
}
void H_29(double *state, double *unused, double *out_1212929769145160524) {
   out_1212929769145160524[0] = 0;
   out_1212929769145160524[1] = 1;
   out_1212929769145160524[2] = 0;
   out_1212929769145160524[3] = 0;
   out_1212929769145160524[4] = 0;
   out_1212929769145160524[5] = 0;
   out_1212929769145160524[6] = 0;
   out_1212929769145160524[7] = 0;
   out_1212929769145160524[8] = 0;
}
void h_28(double *state, double *unused, double *out_4201082650215824369) {
   out_4201082650215824369[0] = state[0];
}
void H_28(double *state, double *unused, double *out_3869469247924370050) {
   out_3869469247924370050[0] = 1;
   out_3869469247924370050[1] = 0;
   out_3869469247924370050[2] = 0;
   out_3869469247924370050[3] = 0;
   out_3869469247924370050[4] = 0;
   out_3869469247924370050[5] = 0;
   out_3869469247924370050[6] = 0;
   out_3869469247924370050[7] = 0;
   out_3869469247924370050[8] = 0;
}
void h_31(double *state, double *unused, double *out_5692946280800919124) {
   out_5692946280800919124[0] = state[8];
}
void H_31(double *state, double *unused, double *out_5261040716835336966) {
   out_5261040716835336966[0] = 0;
   out_5261040716835336966[1] = 0;
   out_5261040716835336966[2] = 0;
   out_5261040716835336966[3] = 0;
   out_5261040716835336966[4] = 0;
   out_5261040716835336966[5] = 0;
   out_5261040716835336966[6] = 0;
   out_5261040716835336966[7] = 0;
   out_5261040716835336966[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_1941320000308867080) {
  err_fun(nom_x, delta_x, out_1941320000308867080);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_8564590531858914382) {
  inv_err_fun(nom_x, true_x, out_8564590531858914382);
}
void car_H_mod_fun(double *state, double *out_1707414104968884861) {
  H_mod_fun(state, out_1707414104968884861);
}
void car_f_fun(double *state, double dt, double *out_2427056617940528332) {
  f_fun(state,  dt, out_2427056617940528332);
}
void car_F_fun(double *state, double dt, double *out_1481622780511648799) {
  F_fun(state,  dt, out_1481622780511648799);
}
void car_h_25(double *state, double *unused, double *out_6533420610061618523) {
  h_25(state, unused, out_6533420610061618523);
}
void car_H_25(double *state, double *unused, double *out_5230394754958376538) {
  H_25(state, unused, out_5230394754958376538);
}
void car_h_24(double *state, double *unused, double *out_7497968432055013731) {
  h_24(state, unused, out_7497968432055013731);
}
void car_H_24(double *state, double *unused, double *out_6562143860642989321) {
  H_24(state, unused, out_6562143860642989321);
}
void car_h_30(double *state, double *unused, double *out_377973414220958394) {
  h_30(state, unused, out_377973414220958394);
}
void car_H_30(double *state, double *unused, double *out_5101055807815136468) {
  H_30(state, unused, out_5101055807815136468);
}
void car_h_26(double *state, double *unused, double *out_981289298897435911) {
  h_26(state, unused, out_981289298897435911);
}
void car_H_26(double *state, double *unused, double *out_1488891436084320314) {
  H_26(state, unused, out_1488891436084320314);
}
void car_h_27(double *state, double *unused, double *out_9202755917876670014) {
  h_27(state, unused, out_9202755917876670014);
}
void car_H_27(double *state, double *unused, double *out_2926292496014711557) {
  H_27(state, unused, out_2926292496014711557);
}
void car_h_29(double *state, double *unused, double *out_1321483267122620829) {
  h_29(state, unused, out_1321483267122620829);
}
void car_H_29(double *state, double *unused, double *out_1212929769145160524) {
  H_29(state, unused, out_1212929769145160524);
}
void car_h_28(double *state, double *unused, double *out_4201082650215824369) {
  h_28(state, unused, out_4201082650215824369);
}
void car_H_28(double *state, double *unused, double *out_3869469247924370050) {
  H_28(state, unused, out_3869469247924370050);
}
void car_h_31(double *state, double *unused, double *out_5692946280800919124) {
  h_31(state, unused, out_5692946280800919124);
}
void car_H_31(double *state, double *unused, double *out_5261040716835336966) {
  H_31(state, unused, out_5261040716835336966);
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
