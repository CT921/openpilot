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
void err_fun(double *nom_x, double *delta_x, double *out_5761959574788611112) {
   out_5761959574788611112[0] = delta_x[0] + nom_x[0];
   out_5761959574788611112[1] = delta_x[1] + nom_x[1];
   out_5761959574788611112[2] = delta_x[2] + nom_x[2];
   out_5761959574788611112[3] = delta_x[3] + nom_x[3];
   out_5761959574788611112[4] = delta_x[4] + nom_x[4];
   out_5761959574788611112[5] = delta_x[5] + nom_x[5];
   out_5761959574788611112[6] = delta_x[6] + nom_x[6];
   out_5761959574788611112[7] = delta_x[7] + nom_x[7];
   out_5761959574788611112[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_7722816966219680250) {
   out_7722816966219680250[0] = -nom_x[0] + true_x[0];
   out_7722816966219680250[1] = -nom_x[1] + true_x[1];
   out_7722816966219680250[2] = -nom_x[2] + true_x[2];
   out_7722816966219680250[3] = -nom_x[3] + true_x[3];
   out_7722816966219680250[4] = -nom_x[4] + true_x[4];
   out_7722816966219680250[5] = -nom_x[5] + true_x[5];
   out_7722816966219680250[6] = -nom_x[6] + true_x[6];
   out_7722816966219680250[7] = -nom_x[7] + true_x[7];
   out_7722816966219680250[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_6261109591928099587) {
   out_6261109591928099587[0] = 1.0;
   out_6261109591928099587[1] = 0.0;
   out_6261109591928099587[2] = 0.0;
   out_6261109591928099587[3] = 0.0;
   out_6261109591928099587[4] = 0.0;
   out_6261109591928099587[5] = 0.0;
   out_6261109591928099587[6] = 0.0;
   out_6261109591928099587[7] = 0.0;
   out_6261109591928099587[8] = 0.0;
   out_6261109591928099587[9] = 0.0;
   out_6261109591928099587[10] = 1.0;
   out_6261109591928099587[11] = 0.0;
   out_6261109591928099587[12] = 0.0;
   out_6261109591928099587[13] = 0.0;
   out_6261109591928099587[14] = 0.0;
   out_6261109591928099587[15] = 0.0;
   out_6261109591928099587[16] = 0.0;
   out_6261109591928099587[17] = 0.0;
   out_6261109591928099587[18] = 0.0;
   out_6261109591928099587[19] = 0.0;
   out_6261109591928099587[20] = 1.0;
   out_6261109591928099587[21] = 0.0;
   out_6261109591928099587[22] = 0.0;
   out_6261109591928099587[23] = 0.0;
   out_6261109591928099587[24] = 0.0;
   out_6261109591928099587[25] = 0.0;
   out_6261109591928099587[26] = 0.0;
   out_6261109591928099587[27] = 0.0;
   out_6261109591928099587[28] = 0.0;
   out_6261109591928099587[29] = 0.0;
   out_6261109591928099587[30] = 1.0;
   out_6261109591928099587[31] = 0.0;
   out_6261109591928099587[32] = 0.0;
   out_6261109591928099587[33] = 0.0;
   out_6261109591928099587[34] = 0.0;
   out_6261109591928099587[35] = 0.0;
   out_6261109591928099587[36] = 0.0;
   out_6261109591928099587[37] = 0.0;
   out_6261109591928099587[38] = 0.0;
   out_6261109591928099587[39] = 0.0;
   out_6261109591928099587[40] = 1.0;
   out_6261109591928099587[41] = 0.0;
   out_6261109591928099587[42] = 0.0;
   out_6261109591928099587[43] = 0.0;
   out_6261109591928099587[44] = 0.0;
   out_6261109591928099587[45] = 0.0;
   out_6261109591928099587[46] = 0.0;
   out_6261109591928099587[47] = 0.0;
   out_6261109591928099587[48] = 0.0;
   out_6261109591928099587[49] = 0.0;
   out_6261109591928099587[50] = 1.0;
   out_6261109591928099587[51] = 0.0;
   out_6261109591928099587[52] = 0.0;
   out_6261109591928099587[53] = 0.0;
   out_6261109591928099587[54] = 0.0;
   out_6261109591928099587[55] = 0.0;
   out_6261109591928099587[56] = 0.0;
   out_6261109591928099587[57] = 0.0;
   out_6261109591928099587[58] = 0.0;
   out_6261109591928099587[59] = 0.0;
   out_6261109591928099587[60] = 1.0;
   out_6261109591928099587[61] = 0.0;
   out_6261109591928099587[62] = 0.0;
   out_6261109591928099587[63] = 0.0;
   out_6261109591928099587[64] = 0.0;
   out_6261109591928099587[65] = 0.0;
   out_6261109591928099587[66] = 0.0;
   out_6261109591928099587[67] = 0.0;
   out_6261109591928099587[68] = 0.0;
   out_6261109591928099587[69] = 0.0;
   out_6261109591928099587[70] = 1.0;
   out_6261109591928099587[71] = 0.0;
   out_6261109591928099587[72] = 0.0;
   out_6261109591928099587[73] = 0.0;
   out_6261109591928099587[74] = 0.0;
   out_6261109591928099587[75] = 0.0;
   out_6261109591928099587[76] = 0.0;
   out_6261109591928099587[77] = 0.0;
   out_6261109591928099587[78] = 0.0;
   out_6261109591928099587[79] = 0.0;
   out_6261109591928099587[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_3430613697628368432) {
   out_3430613697628368432[0] = state[0];
   out_3430613697628368432[1] = state[1];
   out_3430613697628368432[2] = state[2];
   out_3430613697628368432[3] = state[3];
   out_3430613697628368432[4] = state[4];
   out_3430613697628368432[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_3430613697628368432[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_3430613697628368432[7] = state[7];
   out_3430613697628368432[8] = state[8];
}
void F_fun(double *state, double dt, double *out_329430980246932027) {
   out_329430980246932027[0] = 1;
   out_329430980246932027[1] = 0;
   out_329430980246932027[2] = 0;
   out_329430980246932027[3] = 0;
   out_329430980246932027[4] = 0;
   out_329430980246932027[5] = 0;
   out_329430980246932027[6] = 0;
   out_329430980246932027[7] = 0;
   out_329430980246932027[8] = 0;
   out_329430980246932027[9] = 0;
   out_329430980246932027[10] = 1;
   out_329430980246932027[11] = 0;
   out_329430980246932027[12] = 0;
   out_329430980246932027[13] = 0;
   out_329430980246932027[14] = 0;
   out_329430980246932027[15] = 0;
   out_329430980246932027[16] = 0;
   out_329430980246932027[17] = 0;
   out_329430980246932027[18] = 0;
   out_329430980246932027[19] = 0;
   out_329430980246932027[20] = 1;
   out_329430980246932027[21] = 0;
   out_329430980246932027[22] = 0;
   out_329430980246932027[23] = 0;
   out_329430980246932027[24] = 0;
   out_329430980246932027[25] = 0;
   out_329430980246932027[26] = 0;
   out_329430980246932027[27] = 0;
   out_329430980246932027[28] = 0;
   out_329430980246932027[29] = 0;
   out_329430980246932027[30] = 1;
   out_329430980246932027[31] = 0;
   out_329430980246932027[32] = 0;
   out_329430980246932027[33] = 0;
   out_329430980246932027[34] = 0;
   out_329430980246932027[35] = 0;
   out_329430980246932027[36] = 0;
   out_329430980246932027[37] = 0;
   out_329430980246932027[38] = 0;
   out_329430980246932027[39] = 0;
   out_329430980246932027[40] = 1;
   out_329430980246932027[41] = 0;
   out_329430980246932027[42] = 0;
   out_329430980246932027[43] = 0;
   out_329430980246932027[44] = 0;
   out_329430980246932027[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_329430980246932027[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_329430980246932027[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_329430980246932027[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_329430980246932027[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_329430980246932027[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_329430980246932027[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_329430980246932027[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_329430980246932027[53] = -9.8100000000000005*dt;
   out_329430980246932027[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_329430980246932027[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_329430980246932027[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_329430980246932027[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_329430980246932027[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_329430980246932027[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_329430980246932027[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_329430980246932027[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_329430980246932027[62] = 0;
   out_329430980246932027[63] = 0;
   out_329430980246932027[64] = 0;
   out_329430980246932027[65] = 0;
   out_329430980246932027[66] = 0;
   out_329430980246932027[67] = 0;
   out_329430980246932027[68] = 0;
   out_329430980246932027[69] = 0;
   out_329430980246932027[70] = 1;
   out_329430980246932027[71] = 0;
   out_329430980246932027[72] = 0;
   out_329430980246932027[73] = 0;
   out_329430980246932027[74] = 0;
   out_329430980246932027[75] = 0;
   out_329430980246932027[76] = 0;
   out_329430980246932027[77] = 0;
   out_329430980246932027[78] = 0;
   out_329430980246932027[79] = 0;
   out_329430980246932027[80] = 1;
}
void h_25(double *state, double *unused, double *out_6962537965763093599) {
   out_6962537965763093599[0] = state[6];
}
void H_25(double *state, double *unused, double *out_9091616208989055521) {
   out_9091616208989055521[0] = 0;
   out_9091616208989055521[1] = 0;
   out_9091616208989055521[2] = 0;
   out_9091616208989055521[3] = 0;
   out_9091616208989055521[4] = 0;
   out_9091616208989055521[5] = 0;
   out_9091616208989055521[6] = 1;
   out_9091616208989055521[7] = 0;
   out_9091616208989055521[8] = 0;
}
void h_24(double *state, double *unused, double *out_9086466335703166215) {
   out_9086466335703166215[0] = state[4];
   out_9086466335703166215[1] = state[5];
}
void H_24(double *state, double *unused, double *out_7182478265714996529) {
   out_7182478265714996529[0] = 0;
   out_7182478265714996529[1] = 0;
   out_7182478265714996529[2] = 0;
   out_7182478265714996529[3] = 0;
   out_7182478265714996529[4] = 1;
   out_7182478265714996529[5] = 0;
   out_7182478265714996529[6] = 0;
   out_7182478265714996529[7] = 0;
   out_7182478265714996529[8] = 0;
   out_7182478265714996529[9] = 0;
   out_7182478265714996529[10] = 0;
   out_7182478265714996529[11] = 0;
   out_7182478265714996529[12] = 0;
   out_7182478265714996529[13] = 0;
   out_7182478265714996529[14] = 1;
   out_7182478265714996529[15] = 0;
   out_7182478265714996529[16] = 0;
   out_7182478265714996529[17] = 0;
}
void h_30(double *state, double *unused, double *out_1647929917074108392) {
   out_1647929917074108392[0] = state[4];
}
void H_30(double *state, double *unused, double *out_6573283250481806894) {
   out_6573283250481806894[0] = 0;
   out_6573283250481806894[1] = 0;
   out_6573283250481806894[2] = 0;
   out_6573283250481806894[3] = 0;
   out_6573283250481806894[4] = 1;
   out_6573283250481806894[5] = 0;
   out_6573283250481806894[6] = 0;
   out_6573283250481806894[7] = 0;
   out_6573283250481806894[8] = 0;
}
void h_26(double *state, double *unused, double *out_7290659788155065227) {
   out_7290659788155065227[0] = state[7];
}
void H_26(double *state, double *unused, double *out_5613624545846439871) {
   out_5613624545846439871[0] = 0;
   out_5613624545846439871[1] = 0;
   out_5613624545846439871[2] = 0;
   out_5613624545846439871[3] = 0;
   out_5613624545846439871[4] = 0;
   out_5613624545846439871[5] = 0;
   out_5613624545846439871[6] = 0;
   out_5613624545846439871[7] = 1;
   out_5613624545846439871[8] = 0;
}
void h_27(double *state, double *unused, double *out_586412670239069213) {
   out_586412670239069213[0] = state[3];
}
void H_27(double *state, double *unused, double *out_4349689179297863677) {
   out_4349689179297863677[0] = 0;
   out_4349689179297863677[1] = 0;
   out_4349689179297863677[2] = 0;
   out_4349689179297863677[3] = 1;
   out_4349689179297863677[4] = 0;
   out_4349689179297863677[5] = 0;
   out_4349689179297863677[6] = 0;
   out_4349689179297863677[7] = 0;
   out_4349689179297863677[8] = 0;
}
void h_29(double *state, double *unused, double *out_2030493016390104616) {
   out_2030493016390104616[0] = state[1];
}
void H_29(double *state, double *unused, double *out_6063051906167414710) {
   out_6063051906167414710[0] = 0;
   out_6063051906167414710[1] = 1;
   out_6063051906167414710[2] = 0;
   out_6063051906167414710[3] = 0;
   out_6063051906167414710[4] = 0;
   out_6063051906167414710[5] = 0;
   out_6063051906167414710[6] = 0;
   out_6063051906167414710[7] = 0;
   out_6063051906167414710[8] = 0;
}
void h_28(double *state, double *unused, double *out_2774700312940059471) {
   out_2774700312940059471[0] = state[0];
}
void H_28(double *state, double *unused, double *out_7301293150472606332) {
   out_7301293150472606332[0] = 1;
   out_7301293150472606332[1] = 0;
   out_7301293150472606332[2] = 0;
   out_7301293150472606332[3] = 0;
   out_7301293150472606332[4] = 0;
   out_7301293150472606332[5] = 0;
   out_7301293150472606332[6] = 0;
   out_7301293150472606332[7] = 0;
   out_7301293150472606332[8] = 0;
}
void h_31(double *state, double *unused, double *out_2175179318920757583) {
   out_2175179318920757583[0] = state[8];
}
void H_31(double *state, double *unused, double *out_4987416443613088395) {
   out_4987416443613088395[0] = 0;
   out_4987416443613088395[1] = 0;
   out_4987416443613088395[2] = 0;
   out_4987416443613088395[3] = 0;
   out_4987416443613088395[4] = 0;
   out_4987416443613088395[5] = 0;
   out_4987416443613088395[6] = 0;
   out_4987416443613088395[7] = 0;
   out_4987416443613088395[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_5761959574788611112) {
  err_fun(nom_x, delta_x, out_5761959574788611112);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_7722816966219680250) {
  inv_err_fun(nom_x, true_x, out_7722816966219680250);
}
void car_H_mod_fun(double *state, double *out_6261109591928099587) {
  H_mod_fun(state, out_6261109591928099587);
}
void car_f_fun(double *state, double dt, double *out_3430613697628368432) {
  f_fun(state,  dt, out_3430613697628368432);
}
void car_F_fun(double *state, double dt, double *out_329430980246932027) {
  F_fun(state,  dt, out_329430980246932027);
}
void car_h_25(double *state, double *unused, double *out_6962537965763093599) {
  h_25(state, unused, out_6962537965763093599);
}
void car_H_25(double *state, double *unused, double *out_9091616208989055521) {
  H_25(state, unused, out_9091616208989055521);
}
void car_h_24(double *state, double *unused, double *out_9086466335703166215) {
  h_24(state, unused, out_9086466335703166215);
}
void car_H_24(double *state, double *unused, double *out_7182478265714996529) {
  H_24(state, unused, out_7182478265714996529);
}
void car_h_30(double *state, double *unused, double *out_1647929917074108392) {
  h_30(state, unused, out_1647929917074108392);
}
void car_H_30(double *state, double *unused, double *out_6573283250481806894) {
  H_30(state, unused, out_6573283250481806894);
}
void car_h_26(double *state, double *unused, double *out_7290659788155065227) {
  h_26(state, unused, out_7290659788155065227);
}
void car_H_26(double *state, double *unused, double *out_5613624545846439871) {
  H_26(state, unused, out_5613624545846439871);
}
void car_h_27(double *state, double *unused, double *out_586412670239069213) {
  h_27(state, unused, out_586412670239069213);
}
void car_H_27(double *state, double *unused, double *out_4349689179297863677) {
  H_27(state, unused, out_4349689179297863677);
}
void car_h_29(double *state, double *unused, double *out_2030493016390104616) {
  h_29(state, unused, out_2030493016390104616);
}
void car_H_29(double *state, double *unused, double *out_6063051906167414710) {
  H_29(state, unused, out_6063051906167414710);
}
void car_h_28(double *state, double *unused, double *out_2774700312940059471) {
  h_28(state, unused, out_2774700312940059471);
}
void car_H_28(double *state, double *unused, double *out_7301293150472606332) {
  H_28(state, unused, out_7301293150472606332);
}
void car_h_31(double *state, double *unused, double *out_2175179318920757583) {
  h_31(state, unused, out_2175179318920757583);
}
void car_H_31(double *state, double *unused, double *out_4987416443613088395) {
  H_31(state, unused, out_4987416443613088395);
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
