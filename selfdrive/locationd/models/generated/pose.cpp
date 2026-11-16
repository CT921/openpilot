#include "pose.h"

namespace {
#define DIM 18
#define EDIM 18
#define MEDIM 18
typedef void (*Hfun)(double *, double *, double *);
const static double MAHA_THRESH_4 = 7.814727903251177;
const static double MAHA_THRESH_10 = 7.814727903251177;
const static double MAHA_THRESH_13 = 7.814727903251177;
const static double MAHA_THRESH_14 = 7.814727903251177;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_2330460961423581716) {
   out_2330460961423581716[0] = delta_x[0] + nom_x[0];
   out_2330460961423581716[1] = delta_x[1] + nom_x[1];
   out_2330460961423581716[2] = delta_x[2] + nom_x[2];
   out_2330460961423581716[3] = delta_x[3] + nom_x[3];
   out_2330460961423581716[4] = delta_x[4] + nom_x[4];
   out_2330460961423581716[5] = delta_x[5] + nom_x[5];
   out_2330460961423581716[6] = delta_x[6] + nom_x[6];
   out_2330460961423581716[7] = delta_x[7] + nom_x[7];
   out_2330460961423581716[8] = delta_x[8] + nom_x[8];
   out_2330460961423581716[9] = delta_x[9] + nom_x[9];
   out_2330460961423581716[10] = delta_x[10] + nom_x[10];
   out_2330460961423581716[11] = delta_x[11] + nom_x[11];
   out_2330460961423581716[12] = delta_x[12] + nom_x[12];
   out_2330460961423581716[13] = delta_x[13] + nom_x[13];
   out_2330460961423581716[14] = delta_x[14] + nom_x[14];
   out_2330460961423581716[15] = delta_x[15] + nom_x[15];
   out_2330460961423581716[16] = delta_x[16] + nom_x[16];
   out_2330460961423581716[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_6665394572644059121) {
   out_6665394572644059121[0] = -nom_x[0] + true_x[0];
   out_6665394572644059121[1] = -nom_x[1] + true_x[1];
   out_6665394572644059121[2] = -nom_x[2] + true_x[2];
   out_6665394572644059121[3] = -nom_x[3] + true_x[3];
   out_6665394572644059121[4] = -nom_x[4] + true_x[4];
   out_6665394572644059121[5] = -nom_x[5] + true_x[5];
   out_6665394572644059121[6] = -nom_x[6] + true_x[6];
   out_6665394572644059121[7] = -nom_x[7] + true_x[7];
   out_6665394572644059121[8] = -nom_x[8] + true_x[8];
   out_6665394572644059121[9] = -nom_x[9] + true_x[9];
   out_6665394572644059121[10] = -nom_x[10] + true_x[10];
   out_6665394572644059121[11] = -nom_x[11] + true_x[11];
   out_6665394572644059121[12] = -nom_x[12] + true_x[12];
   out_6665394572644059121[13] = -nom_x[13] + true_x[13];
   out_6665394572644059121[14] = -nom_x[14] + true_x[14];
   out_6665394572644059121[15] = -nom_x[15] + true_x[15];
   out_6665394572644059121[16] = -nom_x[16] + true_x[16];
   out_6665394572644059121[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_132265286258857251) {
   out_132265286258857251[0] = 1.0;
   out_132265286258857251[1] = 0.0;
   out_132265286258857251[2] = 0.0;
   out_132265286258857251[3] = 0.0;
   out_132265286258857251[4] = 0.0;
   out_132265286258857251[5] = 0.0;
   out_132265286258857251[6] = 0.0;
   out_132265286258857251[7] = 0.0;
   out_132265286258857251[8] = 0.0;
   out_132265286258857251[9] = 0.0;
   out_132265286258857251[10] = 0.0;
   out_132265286258857251[11] = 0.0;
   out_132265286258857251[12] = 0.0;
   out_132265286258857251[13] = 0.0;
   out_132265286258857251[14] = 0.0;
   out_132265286258857251[15] = 0.0;
   out_132265286258857251[16] = 0.0;
   out_132265286258857251[17] = 0.0;
   out_132265286258857251[18] = 0.0;
   out_132265286258857251[19] = 1.0;
   out_132265286258857251[20] = 0.0;
   out_132265286258857251[21] = 0.0;
   out_132265286258857251[22] = 0.0;
   out_132265286258857251[23] = 0.0;
   out_132265286258857251[24] = 0.0;
   out_132265286258857251[25] = 0.0;
   out_132265286258857251[26] = 0.0;
   out_132265286258857251[27] = 0.0;
   out_132265286258857251[28] = 0.0;
   out_132265286258857251[29] = 0.0;
   out_132265286258857251[30] = 0.0;
   out_132265286258857251[31] = 0.0;
   out_132265286258857251[32] = 0.0;
   out_132265286258857251[33] = 0.0;
   out_132265286258857251[34] = 0.0;
   out_132265286258857251[35] = 0.0;
   out_132265286258857251[36] = 0.0;
   out_132265286258857251[37] = 0.0;
   out_132265286258857251[38] = 1.0;
   out_132265286258857251[39] = 0.0;
   out_132265286258857251[40] = 0.0;
   out_132265286258857251[41] = 0.0;
   out_132265286258857251[42] = 0.0;
   out_132265286258857251[43] = 0.0;
   out_132265286258857251[44] = 0.0;
   out_132265286258857251[45] = 0.0;
   out_132265286258857251[46] = 0.0;
   out_132265286258857251[47] = 0.0;
   out_132265286258857251[48] = 0.0;
   out_132265286258857251[49] = 0.0;
   out_132265286258857251[50] = 0.0;
   out_132265286258857251[51] = 0.0;
   out_132265286258857251[52] = 0.0;
   out_132265286258857251[53] = 0.0;
   out_132265286258857251[54] = 0.0;
   out_132265286258857251[55] = 0.0;
   out_132265286258857251[56] = 0.0;
   out_132265286258857251[57] = 1.0;
   out_132265286258857251[58] = 0.0;
   out_132265286258857251[59] = 0.0;
   out_132265286258857251[60] = 0.0;
   out_132265286258857251[61] = 0.0;
   out_132265286258857251[62] = 0.0;
   out_132265286258857251[63] = 0.0;
   out_132265286258857251[64] = 0.0;
   out_132265286258857251[65] = 0.0;
   out_132265286258857251[66] = 0.0;
   out_132265286258857251[67] = 0.0;
   out_132265286258857251[68] = 0.0;
   out_132265286258857251[69] = 0.0;
   out_132265286258857251[70] = 0.0;
   out_132265286258857251[71] = 0.0;
   out_132265286258857251[72] = 0.0;
   out_132265286258857251[73] = 0.0;
   out_132265286258857251[74] = 0.0;
   out_132265286258857251[75] = 0.0;
   out_132265286258857251[76] = 1.0;
   out_132265286258857251[77] = 0.0;
   out_132265286258857251[78] = 0.0;
   out_132265286258857251[79] = 0.0;
   out_132265286258857251[80] = 0.0;
   out_132265286258857251[81] = 0.0;
   out_132265286258857251[82] = 0.0;
   out_132265286258857251[83] = 0.0;
   out_132265286258857251[84] = 0.0;
   out_132265286258857251[85] = 0.0;
   out_132265286258857251[86] = 0.0;
   out_132265286258857251[87] = 0.0;
   out_132265286258857251[88] = 0.0;
   out_132265286258857251[89] = 0.0;
   out_132265286258857251[90] = 0.0;
   out_132265286258857251[91] = 0.0;
   out_132265286258857251[92] = 0.0;
   out_132265286258857251[93] = 0.0;
   out_132265286258857251[94] = 0.0;
   out_132265286258857251[95] = 1.0;
   out_132265286258857251[96] = 0.0;
   out_132265286258857251[97] = 0.0;
   out_132265286258857251[98] = 0.0;
   out_132265286258857251[99] = 0.0;
   out_132265286258857251[100] = 0.0;
   out_132265286258857251[101] = 0.0;
   out_132265286258857251[102] = 0.0;
   out_132265286258857251[103] = 0.0;
   out_132265286258857251[104] = 0.0;
   out_132265286258857251[105] = 0.0;
   out_132265286258857251[106] = 0.0;
   out_132265286258857251[107] = 0.0;
   out_132265286258857251[108] = 0.0;
   out_132265286258857251[109] = 0.0;
   out_132265286258857251[110] = 0.0;
   out_132265286258857251[111] = 0.0;
   out_132265286258857251[112] = 0.0;
   out_132265286258857251[113] = 0.0;
   out_132265286258857251[114] = 1.0;
   out_132265286258857251[115] = 0.0;
   out_132265286258857251[116] = 0.0;
   out_132265286258857251[117] = 0.0;
   out_132265286258857251[118] = 0.0;
   out_132265286258857251[119] = 0.0;
   out_132265286258857251[120] = 0.0;
   out_132265286258857251[121] = 0.0;
   out_132265286258857251[122] = 0.0;
   out_132265286258857251[123] = 0.0;
   out_132265286258857251[124] = 0.0;
   out_132265286258857251[125] = 0.0;
   out_132265286258857251[126] = 0.0;
   out_132265286258857251[127] = 0.0;
   out_132265286258857251[128] = 0.0;
   out_132265286258857251[129] = 0.0;
   out_132265286258857251[130] = 0.0;
   out_132265286258857251[131] = 0.0;
   out_132265286258857251[132] = 0.0;
   out_132265286258857251[133] = 1.0;
   out_132265286258857251[134] = 0.0;
   out_132265286258857251[135] = 0.0;
   out_132265286258857251[136] = 0.0;
   out_132265286258857251[137] = 0.0;
   out_132265286258857251[138] = 0.0;
   out_132265286258857251[139] = 0.0;
   out_132265286258857251[140] = 0.0;
   out_132265286258857251[141] = 0.0;
   out_132265286258857251[142] = 0.0;
   out_132265286258857251[143] = 0.0;
   out_132265286258857251[144] = 0.0;
   out_132265286258857251[145] = 0.0;
   out_132265286258857251[146] = 0.0;
   out_132265286258857251[147] = 0.0;
   out_132265286258857251[148] = 0.0;
   out_132265286258857251[149] = 0.0;
   out_132265286258857251[150] = 0.0;
   out_132265286258857251[151] = 0.0;
   out_132265286258857251[152] = 1.0;
   out_132265286258857251[153] = 0.0;
   out_132265286258857251[154] = 0.0;
   out_132265286258857251[155] = 0.0;
   out_132265286258857251[156] = 0.0;
   out_132265286258857251[157] = 0.0;
   out_132265286258857251[158] = 0.0;
   out_132265286258857251[159] = 0.0;
   out_132265286258857251[160] = 0.0;
   out_132265286258857251[161] = 0.0;
   out_132265286258857251[162] = 0.0;
   out_132265286258857251[163] = 0.0;
   out_132265286258857251[164] = 0.0;
   out_132265286258857251[165] = 0.0;
   out_132265286258857251[166] = 0.0;
   out_132265286258857251[167] = 0.0;
   out_132265286258857251[168] = 0.0;
   out_132265286258857251[169] = 0.0;
   out_132265286258857251[170] = 0.0;
   out_132265286258857251[171] = 1.0;
   out_132265286258857251[172] = 0.0;
   out_132265286258857251[173] = 0.0;
   out_132265286258857251[174] = 0.0;
   out_132265286258857251[175] = 0.0;
   out_132265286258857251[176] = 0.0;
   out_132265286258857251[177] = 0.0;
   out_132265286258857251[178] = 0.0;
   out_132265286258857251[179] = 0.0;
   out_132265286258857251[180] = 0.0;
   out_132265286258857251[181] = 0.0;
   out_132265286258857251[182] = 0.0;
   out_132265286258857251[183] = 0.0;
   out_132265286258857251[184] = 0.0;
   out_132265286258857251[185] = 0.0;
   out_132265286258857251[186] = 0.0;
   out_132265286258857251[187] = 0.0;
   out_132265286258857251[188] = 0.0;
   out_132265286258857251[189] = 0.0;
   out_132265286258857251[190] = 1.0;
   out_132265286258857251[191] = 0.0;
   out_132265286258857251[192] = 0.0;
   out_132265286258857251[193] = 0.0;
   out_132265286258857251[194] = 0.0;
   out_132265286258857251[195] = 0.0;
   out_132265286258857251[196] = 0.0;
   out_132265286258857251[197] = 0.0;
   out_132265286258857251[198] = 0.0;
   out_132265286258857251[199] = 0.0;
   out_132265286258857251[200] = 0.0;
   out_132265286258857251[201] = 0.0;
   out_132265286258857251[202] = 0.0;
   out_132265286258857251[203] = 0.0;
   out_132265286258857251[204] = 0.0;
   out_132265286258857251[205] = 0.0;
   out_132265286258857251[206] = 0.0;
   out_132265286258857251[207] = 0.0;
   out_132265286258857251[208] = 0.0;
   out_132265286258857251[209] = 1.0;
   out_132265286258857251[210] = 0.0;
   out_132265286258857251[211] = 0.0;
   out_132265286258857251[212] = 0.0;
   out_132265286258857251[213] = 0.0;
   out_132265286258857251[214] = 0.0;
   out_132265286258857251[215] = 0.0;
   out_132265286258857251[216] = 0.0;
   out_132265286258857251[217] = 0.0;
   out_132265286258857251[218] = 0.0;
   out_132265286258857251[219] = 0.0;
   out_132265286258857251[220] = 0.0;
   out_132265286258857251[221] = 0.0;
   out_132265286258857251[222] = 0.0;
   out_132265286258857251[223] = 0.0;
   out_132265286258857251[224] = 0.0;
   out_132265286258857251[225] = 0.0;
   out_132265286258857251[226] = 0.0;
   out_132265286258857251[227] = 0.0;
   out_132265286258857251[228] = 1.0;
   out_132265286258857251[229] = 0.0;
   out_132265286258857251[230] = 0.0;
   out_132265286258857251[231] = 0.0;
   out_132265286258857251[232] = 0.0;
   out_132265286258857251[233] = 0.0;
   out_132265286258857251[234] = 0.0;
   out_132265286258857251[235] = 0.0;
   out_132265286258857251[236] = 0.0;
   out_132265286258857251[237] = 0.0;
   out_132265286258857251[238] = 0.0;
   out_132265286258857251[239] = 0.0;
   out_132265286258857251[240] = 0.0;
   out_132265286258857251[241] = 0.0;
   out_132265286258857251[242] = 0.0;
   out_132265286258857251[243] = 0.0;
   out_132265286258857251[244] = 0.0;
   out_132265286258857251[245] = 0.0;
   out_132265286258857251[246] = 0.0;
   out_132265286258857251[247] = 1.0;
   out_132265286258857251[248] = 0.0;
   out_132265286258857251[249] = 0.0;
   out_132265286258857251[250] = 0.0;
   out_132265286258857251[251] = 0.0;
   out_132265286258857251[252] = 0.0;
   out_132265286258857251[253] = 0.0;
   out_132265286258857251[254] = 0.0;
   out_132265286258857251[255] = 0.0;
   out_132265286258857251[256] = 0.0;
   out_132265286258857251[257] = 0.0;
   out_132265286258857251[258] = 0.0;
   out_132265286258857251[259] = 0.0;
   out_132265286258857251[260] = 0.0;
   out_132265286258857251[261] = 0.0;
   out_132265286258857251[262] = 0.0;
   out_132265286258857251[263] = 0.0;
   out_132265286258857251[264] = 0.0;
   out_132265286258857251[265] = 0.0;
   out_132265286258857251[266] = 1.0;
   out_132265286258857251[267] = 0.0;
   out_132265286258857251[268] = 0.0;
   out_132265286258857251[269] = 0.0;
   out_132265286258857251[270] = 0.0;
   out_132265286258857251[271] = 0.0;
   out_132265286258857251[272] = 0.0;
   out_132265286258857251[273] = 0.0;
   out_132265286258857251[274] = 0.0;
   out_132265286258857251[275] = 0.0;
   out_132265286258857251[276] = 0.0;
   out_132265286258857251[277] = 0.0;
   out_132265286258857251[278] = 0.0;
   out_132265286258857251[279] = 0.0;
   out_132265286258857251[280] = 0.0;
   out_132265286258857251[281] = 0.0;
   out_132265286258857251[282] = 0.0;
   out_132265286258857251[283] = 0.0;
   out_132265286258857251[284] = 0.0;
   out_132265286258857251[285] = 1.0;
   out_132265286258857251[286] = 0.0;
   out_132265286258857251[287] = 0.0;
   out_132265286258857251[288] = 0.0;
   out_132265286258857251[289] = 0.0;
   out_132265286258857251[290] = 0.0;
   out_132265286258857251[291] = 0.0;
   out_132265286258857251[292] = 0.0;
   out_132265286258857251[293] = 0.0;
   out_132265286258857251[294] = 0.0;
   out_132265286258857251[295] = 0.0;
   out_132265286258857251[296] = 0.0;
   out_132265286258857251[297] = 0.0;
   out_132265286258857251[298] = 0.0;
   out_132265286258857251[299] = 0.0;
   out_132265286258857251[300] = 0.0;
   out_132265286258857251[301] = 0.0;
   out_132265286258857251[302] = 0.0;
   out_132265286258857251[303] = 0.0;
   out_132265286258857251[304] = 1.0;
   out_132265286258857251[305] = 0.0;
   out_132265286258857251[306] = 0.0;
   out_132265286258857251[307] = 0.0;
   out_132265286258857251[308] = 0.0;
   out_132265286258857251[309] = 0.0;
   out_132265286258857251[310] = 0.0;
   out_132265286258857251[311] = 0.0;
   out_132265286258857251[312] = 0.0;
   out_132265286258857251[313] = 0.0;
   out_132265286258857251[314] = 0.0;
   out_132265286258857251[315] = 0.0;
   out_132265286258857251[316] = 0.0;
   out_132265286258857251[317] = 0.0;
   out_132265286258857251[318] = 0.0;
   out_132265286258857251[319] = 0.0;
   out_132265286258857251[320] = 0.0;
   out_132265286258857251[321] = 0.0;
   out_132265286258857251[322] = 0.0;
   out_132265286258857251[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_8603722381005829308) {
   out_8603722381005829308[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_8603722381005829308[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_8603722381005829308[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_8603722381005829308[3] = dt*state[12] + state[3];
   out_8603722381005829308[4] = dt*state[13] + state[4];
   out_8603722381005829308[5] = dt*state[14] + state[5];
   out_8603722381005829308[6] = state[6];
   out_8603722381005829308[7] = state[7];
   out_8603722381005829308[8] = state[8];
   out_8603722381005829308[9] = state[9];
   out_8603722381005829308[10] = state[10];
   out_8603722381005829308[11] = state[11];
   out_8603722381005829308[12] = state[12];
   out_8603722381005829308[13] = state[13];
   out_8603722381005829308[14] = state[14];
   out_8603722381005829308[15] = state[15];
   out_8603722381005829308[16] = state[16];
   out_8603722381005829308[17] = state[17];
}
void F_fun(double *state, double dt, double *out_2009381636234601862) {
   out_2009381636234601862[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2009381636234601862[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2009381636234601862[2] = 0;
   out_2009381636234601862[3] = 0;
   out_2009381636234601862[4] = 0;
   out_2009381636234601862[5] = 0;
   out_2009381636234601862[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2009381636234601862[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2009381636234601862[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2009381636234601862[9] = 0;
   out_2009381636234601862[10] = 0;
   out_2009381636234601862[11] = 0;
   out_2009381636234601862[12] = 0;
   out_2009381636234601862[13] = 0;
   out_2009381636234601862[14] = 0;
   out_2009381636234601862[15] = 0;
   out_2009381636234601862[16] = 0;
   out_2009381636234601862[17] = 0;
   out_2009381636234601862[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2009381636234601862[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2009381636234601862[20] = 0;
   out_2009381636234601862[21] = 0;
   out_2009381636234601862[22] = 0;
   out_2009381636234601862[23] = 0;
   out_2009381636234601862[24] = 0;
   out_2009381636234601862[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2009381636234601862[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2009381636234601862[27] = 0;
   out_2009381636234601862[28] = 0;
   out_2009381636234601862[29] = 0;
   out_2009381636234601862[30] = 0;
   out_2009381636234601862[31] = 0;
   out_2009381636234601862[32] = 0;
   out_2009381636234601862[33] = 0;
   out_2009381636234601862[34] = 0;
   out_2009381636234601862[35] = 0;
   out_2009381636234601862[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2009381636234601862[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2009381636234601862[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2009381636234601862[39] = 0;
   out_2009381636234601862[40] = 0;
   out_2009381636234601862[41] = 0;
   out_2009381636234601862[42] = 0;
   out_2009381636234601862[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2009381636234601862[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2009381636234601862[45] = 0;
   out_2009381636234601862[46] = 0;
   out_2009381636234601862[47] = 0;
   out_2009381636234601862[48] = 0;
   out_2009381636234601862[49] = 0;
   out_2009381636234601862[50] = 0;
   out_2009381636234601862[51] = 0;
   out_2009381636234601862[52] = 0;
   out_2009381636234601862[53] = 0;
   out_2009381636234601862[54] = 0;
   out_2009381636234601862[55] = 0;
   out_2009381636234601862[56] = 0;
   out_2009381636234601862[57] = 1;
   out_2009381636234601862[58] = 0;
   out_2009381636234601862[59] = 0;
   out_2009381636234601862[60] = 0;
   out_2009381636234601862[61] = 0;
   out_2009381636234601862[62] = 0;
   out_2009381636234601862[63] = 0;
   out_2009381636234601862[64] = 0;
   out_2009381636234601862[65] = 0;
   out_2009381636234601862[66] = dt;
   out_2009381636234601862[67] = 0;
   out_2009381636234601862[68] = 0;
   out_2009381636234601862[69] = 0;
   out_2009381636234601862[70] = 0;
   out_2009381636234601862[71] = 0;
   out_2009381636234601862[72] = 0;
   out_2009381636234601862[73] = 0;
   out_2009381636234601862[74] = 0;
   out_2009381636234601862[75] = 0;
   out_2009381636234601862[76] = 1;
   out_2009381636234601862[77] = 0;
   out_2009381636234601862[78] = 0;
   out_2009381636234601862[79] = 0;
   out_2009381636234601862[80] = 0;
   out_2009381636234601862[81] = 0;
   out_2009381636234601862[82] = 0;
   out_2009381636234601862[83] = 0;
   out_2009381636234601862[84] = 0;
   out_2009381636234601862[85] = dt;
   out_2009381636234601862[86] = 0;
   out_2009381636234601862[87] = 0;
   out_2009381636234601862[88] = 0;
   out_2009381636234601862[89] = 0;
   out_2009381636234601862[90] = 0;
   out_2009381636234601862[91] = 0;
   out_2009381636234601862[92] = 0;
   out_2009381636234601862[93] = 0;
   out_2009381636234601862[94] = 0;
   out_2009381636234601862[95] = 1;
   out_2009381636234601862[96] = 0;
   out_2009381636234601862[97] = 0;
   out_2009381636234601862[98] = 0;
   out_2009381636234601862[99] = 0;
   out_2009381636234601862[100] = 0;
   out_2009381636234601862[101] = 0;
   out_2009381636234601862[102] = 0;
   out_2009381636234601862[103] = 0;
   out_2009381636234601862[104] = dt;
   out_2009381636234601862[105] = 0;
   out_2009381636234601862[106] = 0;
   out_2009381636234601862[107] = 0;
   out_2009381636234601862[108] = 0;
   out_2009381636234601862[109] = 0;
   out_2009381636234601862[110] = 0;
   out_2009381636234601862[111] = 0;
   out_2009381636234601862[112] = 0;
   out_2009381636234601862[113] = 0;
   out_2009381636234601862[114] = 1;
   out_2009381636234601862[115] = 0;
   out_2009381636234601862[116] = 0;
   out_2009381636234601862[117] = 0;
   out_2009381636234601862[118] = 0;
   out_2009381636234601862[119] = 0;
   out_2009381636234601862[120] = 0;
   out_2009381636234601862[121] = 0;
   out_2009381636234601862[122] = 0;
   out_2009381636234601862[123] = 0;
   out_2009381636234601862[124] = 0;
   out_2009381636234601862[125] = 0;
   out_2009381636234601862[126] = 0;
   out_2009381636234601862[127] = 0;
   out_2009381636234601862[128] = 0;
   out_2009381636234601862[129] = 0;
   out_2009381636234601862[130] = 0;
   out_2009381636234601862[131] = 0;
   out_2009381636234601862[132] = 0;
   out_2009381636234601862[133] = 1;
   out_2009381636234601862[134] = 0;
   out_2009381636234601862[135] = 0;
   out_2009381636234601862[136] = 0;
   out_2009381636234601862[137] = 0;
   out_2009381636234601862[138] = 0;
   out_2009381636234601862[139] = 0;
   out_2009381636234601862[140] = 0;
   out_2009381636234601862[141] = 0;
   out_2009381636234601862[142] = 0;
   out_2009381636234601862[143] = 0;
   out_2009381636234601862[144] = 0;
   out_2009381636234601862[145] = 0;
   out_2009381636234601862[146] = 0;
   out_2009381636234601862[147] = 0;
   out_2009381636234601862[148] = 0;
   out_2009381636234601862[149] = 0;
   out_2009381636234601862[150] = 0;
   out_2009381636234601862[151] = 0;
   out_2009381636234601862[152] = 1;
   out_2009381636234601862[153] = 0;
   out_2009381636234601862[154] = 0;
   out_2009381636234601862[155] = 0;
   out_2009381636234601862[156] = 0;
   out_2009381636234601862[157] = 0;
   out_2009381636234601862[158] = 0;
   out_2009381636234601862[159] = 0;
   out_2009381636234601862[160] = 0;
   out_2009381636234601862[161] = 0;
   out_2009381636234601862[162] = 0;
   out_2009381636234601862[163] = 0;
   out_2009381636234601862[164] = 0;
   out_2009381636234601862[165] = 0;
   out_2009381636234601862[166] = 0;
   out_2009381636234601862[167] = 0;
   out_2009381636234601862[168] = 0;
   out_2009381636234601862[169] = 0;
   out_2009381636234601862[170] = 0;
   out_2009381636234601862[171] = 1;
   out_2009381636234601862[172] = 0;
   out_2009381636234601862[173] = 0;
   out_2009381636234601862[174] = 0;
   out_2009381636234601862[175] = 0;
   out_2009381636234601862[176] = 0;
   out_2009381636234601862[177] = 0;
   out_2009381636234601862[178] = 0;
   out_2009381636234601862[179] = 0;
   out_2009381636234601862[180] = 0;
   out_2009381636234601862[181] = 0;
   out_2009381636234601862[182] = 0;
   out_2009381636234601862[183] = 0;
   out_2009381636234601862[184] = 0;
   out_2009381636234601862[185] = 0;
   out_2009381636234601862[186] = 0;
   out_2009381636234601862[187] = 0;
   out_2009381636234601862[188] = 0;
   out_2009381636234601862[189] = 0;
   out_2009381636234601862[190] = 1;
   out_2009381636234601862[191] = 0;
   out_2009381636234601862[192] = 0;
   out_2009381636234601862[193] = 0;
   out_2009381636234601862[194] = 0;
   out_2009381636234601862[195] = 0;
   out_2009381636234601862[196] = 0;
   out_2009381636234601862[197] = 0;
   out_2009381636234601862[198] = 0;
   out_2009381636234601862[199] = 0;
   out_2009381636234601862[200] = 0;
   out_2009381636234601862[201] = 0;
   out_2009381636234601862[202] = 0;
   out_2009381636234601862[203] = 0;
   out_2009381636234601862[204] = 0;
   out_2009381636234601862[205] = 0;
   out_2009381636234601862[206] = 0;
   out_2009381636234601862[207] = 0;
   out_2009381636234601862[208] = 0;
   out_2009381636234601862[209] = 1;
   out_2009381636234601862[210] = 0;
   out_2009381636234601862[211] = 0;
   out_2009381636234601862[212] = 0;
   out_2009381636234601862[213] = 0;
   out_2009381636234601862[214] = 0;
   out_2009381636234601862[215] = 0;
   out_2009381636234601862[216] = 0;
   out_2009381636234601862[217] = 0;
   out_2009381636234601862[218] = 0;
   out_2009381636234601862[219] = 0;
   out_2009381636234601862[220] = 0;
   out_2009381636234601862[221] = 0;
   out_2009381636234601862[222] = 0;
   out_2009381636234601862[223] = 0;
   out_2009381636234601862[224] = 0;
   out_2009381636234601862[225] = 0;
   out_2009381636234601862[226] = 0;
   out_2009381636234601862[227] = 0;
   out_2009381636234601862[228] = 1;
   out_2009381636234601862[229] = 0;
   out_2009381636234601862[230] = 0;
   out_2009381636234601862[231] = 0;
   out_2009381636234601862[232] = 0;
   out_2009381636234601862[233] = 0;
   out_2009381636234601862[234] = 0;
   out_2009381636234601862[235] = 0;
   out_2009381636234601862[236] = 0;
   out_2009381636234601862[237] = 0;
   out_2009381636234601862[238] = 0;
   out_2009381636234601862[239] = 0;
   out_2009381636234601862[240] = 0;
   out_2009381636234601862[241] = 0;
   out_2009381636234601862[242] = 0;
   out_2009381636234601862[243] = 0;
   out_2009381636234601862[244] = 0;
   out_2009381636234601862[245] = 0;
   out_2009381636234601862[246] = 0;
   out_2009381636234601862[247] = 1;
   out_2009381636234601862[248] = 0;
   out_2009381636234601862[249] = 0;
   out_2009381636234601862[250] = 0;
   out_2009381636234601862[251] = 0;
   out_2009381636234601862[252] = 0;
   out_2009381636234601862[253] = 0;
   out_2009381636234601862[254] = 0;
   out_2009381636234601862[255] = 0;
   out_2009381636234601862[256] = 0;
   out_2009381636234601862[257] = 0;
   out_2009381636234601862[258] = 0;
   out_2009381636234601862[259] = 0;
   out_2009381636234601862[260] = 0;
   out_2009381636234601862[261] = 0;
   out_2009381636234601862[262] = 0;
   out_2009381636234601862[263] = 0;
   out_2009381636234601862[264] = 0;
   out_2009381636234601862[265] = 0;
   out_2009381636234601862[266] = 1;
   out_2009381636234601862[267] = 0;
   out_2009381636234601862[268] = 0;
   out_2009381636234601862[269] = 0;
   out_2009381636234601862[270] = 0;
   out_2009381636234601862[271] = 0;
   out_2009381636234601862[272] = 0;
   out_2009381636234601862[273] = 0;
   out_2009381636234601862[274] = 0;
   out_2009381636234601862[275] = 0;
   out_2009381636234601862[276] = 0;
   out_2009381636234601862[277] = 0;
   out_2009381636234601862[278] = 0;
   out_2009381636234601862[279] = 0;
   out_2009381636234601862[280] = 0;
   out_2009381636234601862[281] = 0;
   out_2009381636234601862[282] = 0;
   out_2009381636234601862[283] = 0;
   out_2009381636234601862[284] = 0;
   out_2009381636234601862[285] = 1;
   out_2009381636234601862[286] = 0;
   out_2009381636234601862[287] = 0;
   out_2009381636234601862[288] = 0;
   out_2009381636234601862[289] = 0;
   out_2009381636234601862[290] = 0;
   out_2009381636234601862[291] = 0;
   out_2009381636234601862[292] = 0;
   out_2009381636234601862[293] = 0;
   out_2009381636234601862[294] = 0;
   out_2009381636234601862[295] = 0;
   out_2009381636234601862[296] = 0;
   out_2009381636234601862[297] = 0;
   out_2009381636234601862[298] = 0;
   out_2009381636234601862[299] = 0;
   out_2009381636234601862[300] = 0;
   out_2009381636234601862[301] = 0;
   out_2009381636234601862[302] = 0;
   out_2009381636234601862[303] = 0;
   out_2009381636234601862[304] = 1;
   out_2009381636234601862[305] = 0;
   out_2009381636234601862[306] = 0;
   out_2009381636234601862[307] = 0;
   out_2009381636234601862[308] = 0;
   out_2009381636234601862[309] = 0;
   out_2009381636234601862[310] = 0;
   out_2009381636234601862[311] = 0;
   out_2009381636234601862[312] = 0;
   out_2009381636234601862[313] = 0;
   out_2009381636234601862[314] = 0;
   out_2009381636234601862[315] = 0;
   out_2009381636234601862[316] = 0;
   out_2009381636234601862[317] = 0;
   out_2009381636234601862[318] = 0;
   out_2009381636234601862[319] = 0;
   out_2009381636234601862[320] = 0;
   out_2009381636234601862[321] = 0;
   out_2009381636234601862[322] = 0;
   out_2009381636234601862[323] = 1;
}
void h_4(double *state, double *unused, double *out_6414518568457466023) {
   out_6414518568457466023[0] = state[6] + state[9];
   out_6414518568457466023[1] = state[7] + state[10];
   out_6414518568457466023[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_8381063530603800607) {
   out_8381063530603800607[0] = 0;
   out_8381063530603800607[1] = 0;
   out_8381063530603800607[2] = 0;
   out_8381063530603800607[3] = 0;
   out_8381063530603800607[4] = 0;
   out_8381063530603800607[5] = 0;
   out_8381063530603800607[6] = 1;
   out_8381063530603800607[7] = 0;
   out_8381063530603800607[8] = 0;
   out_8381063530603800607[9] = 1;
   out_8381063530603800607[10] = 0;
   out_8381063530603800607[11] = 0;
   out_8381063530603800607[12] = 0;
   out_8381063530603800607[13] = 0;
   out_8381063530603800607[14] = 0;
   out_8381063530603800607[15] = 0;
   out_8381063530603800607[16] = 0;
   out_8381063530603800607[17] = 0;
   out_8381063530603800607[18] = 0;
   out_8381063530603800607[19] = 0;
   out_8381063530603800607[20] = 0;
   out_8381063530603800607[21] = 0;
   out_8381063530603800607[22] = 0;
   out_8381063530603800607[23] = 0;
   out_8381063530603800607[24] = 0;
   out_8381063530603800607[25] = 1;
   out_8381063530603800607[26] = 0;
   out_8381063530603800607[27] = 0;
   out_8381063530603800607[28] = 1;
   out_8381063530603800607[29] = 0;
   out_8381063530603800607[30] = 0;
   out_8381063530603800607[31] = 0;
   out_8381063530603800607[32] = 0;
   out_8381063530603800607[33] = 0;
   out_8381063530603800607[34] = 0;
   out_8381063530603800607[35] = 0;
   out_8381063530603800607[36] = 0;
   out_8381063530603800607[37] = 0;
   out_8381063530603800607[38] = 0;
   out_8381063530603800607[39] = 0;
   out_8381063530603800607[40] = 0;
   out_8381063530603800607[41] = 0;
   out_8381063530603800607[42] = 0;
   out_8381063530603800607[43] = 0;
   out_8381063530603800607[44] = 1;
   out_8381063530603800607[45] = 0;
   out_8381063530603800607[46] = 0;
   out_8381063530603800607[47] = 1;
   out_8381063530603800607[48] = 0;
   out_8381063530603800607[49] = 0;
   out_8381063530603800607[50] = 0;
   out_8381063530603800607[51] = 0;
   out_8381063530603800607[52] = 0;
   out_8381063530603800607[53] = 0;
}
void h_10(double *state, double *unused, double *out_1500850275664504396) {
   out_1500850275664504396[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_1500850275664504396[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_1500850275664504396[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_4230206924188573634) {
   out_4230206924188573634[0] = 0;
   out_4230206924188573634[1] = 9.8100000000000005*cos(state[1]);
   out_4230206924188573634[2] = 0;
   out_4230206924188573634[3] = 0;
   out_4230206924188573634[4] = -state[8];
   out_4230206924188573634[5] = state[7];
   out_4230206924188573634[6] = 0;
   out_4230206924188573634[7] = state[5];
   out_4230206924188573634[8] = -state[4];
   out_4230206924188573634[9] = 0;
   out_4230206924188573634[10] = 0;
   out_4230206924188573634[11] = 0;
   out_4230206924188573634[12] = 1;
   out_4230206924188573634[13] = 0;
   out_4230206924188573634[14] = 0;
   out_4230206924188573634[15] = 1;
   out_4230206924188573634[16] = 0;
   out_4230206924188573634[17] = 0;
   out_4230206924188573634[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_4230206924188573634[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_4230206924188573634[20] = 0;
   out_4230206924188573634[21] = state[8];
   out_4230206924188573634[22] = 0;
   out_4230206924188573634[23] = -state[6];
   out_4230206924188573634[24] = -state[5];
   out_4230206924188573634[25] = 0;
   out_4230206924188573634[26] = state[3];
   out_4230206924188573634[27] = 0;
   out_4230206924188573634[28] = 0;
   out_4230206924188573634[29] = 0;
   out_4230206924188573634[30] = 0;
   out_4230206924188573634[31] = 1;
   out_4230206924188573634[32] = 0;
   out_4230206924188573634[33] = 0;
   out_4230206924188573634[34] = 1;
   out_4230206924188573634[35] = 0;
   out_4230206924188573634[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_4230206924188573634[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_4230206924188573634[38] = 0;
   out_4230206924188573634[39] = -state[7];
   out_4230206924188573634[40] = state[6];
   out_4230206924188573634[41] = 0;
   out_4230206924188573634[42] = state[4];
   out_4230206924188573634[43] = -state[3];
   out_4230206924188573634[44] = 0;
   out_4230206924188573634[45] = 0;
   out_4230206924188573634[46] = 0;
   out_4230206924188573634[47] = 0;
   out_4230206924188573634[48] = 0;
   out_4230206924188573634[49] = 0;
   out_4230206924188573634[50] = 1;
   out_4230206924188573634[51] = 0;
   out_4230206924188573634[52] = 0;
   out_4230206924188573634[53] = 1;
}
void h_13(double *state, double *unused, double *out_2580468438894152282) {
   out_2580468438894152282[0] = state[3];
   out_2580468438894152282[1] = state[4];
   out_2580468438894152282[2] = state[5];
}
void H_13(double *state, double *unused, double *out_6853406717773418208) {
   out_6853406717773418208[0] = 0;
   out_6853406717773418208[1] = 0;
   out_6853406717773418208[2] = 0;
   out_6853406717773418208[3] = 1;
   out_6853406717773418208[4] = 0;
   out_6853406717773418208[5] = 0;
   out_6853406717773418208[6] = 0;
   out_6853406717773418208[7] = 0;
   out_6853406717773418208[8] = 0;
   out_6853406717773418208[9] = 0;
   out_6853406717773418208[10] = 0;
   out_6853406717773418208[11] = 0;
   out_6853406717773418208[12] = 0;
   out_6853406717773418208[13] = 0;
   out_6853406717773418208[14] = 0;
   out_6853406717773418208[15] = 0;
   out_6853406717773418208[16] = 0;
   out_6853406717773418208[17] = 0;
   out_6853406717773418208[18] = 0;
   out_6853406717773418208[19] = 0;
   out_6853406717773418208[20] = 0;
   out_6853406717773418208[21] = 0;
   out_6853406717773418208[22] = 1;
   out_6853406717773418208[23] = 0;
   out_6853406717773418208[24] = 0;
   out_6853406717773418208[25] = 0;
   out_6853406717773418208[26] = 0;
   out_6853406717773418208[27] = 0;
   out_6853406717773418208[28] = 0;
   out_6853406717773418208[29] = 0;
   out_6853406717773418208[30] = 0;
   out_6853406717773418208[31] = 0;
   out_6853406717773418208[32] = 0;
   out_6853406717773418208[33] = 0;
   out_6853406717773418208[34] = 0;
   out_6853406717773418208[35] = 0;
   out_6853406717773418208[36] = 0;
   out_6853406717773418208[37] = 0;
   out_6853406717773418208[38] = 0;
   out_6853406717773418208[39] = 0;
   out_6853406717773418208[40] = 0;
   out_6853406717773418208[41] = 1;
   out_6853406717773418208[42] = 0;
   out_6853406717773418208[43] = 0;
   out_6853406717773418208[44] = 0;
   out_6853406717773418208[45] = 0;
   out_6853406717773418208[46] = 0;
   out_6853406717773418208[47] = 0;
   out_6853406717773418208[48] = 0;
   out_6853406717773418208[49] = 0;
   out_6853406717773418208[50] = 0;
   out_6853406717773418208[51] = 0;
   out_6853406717773418208[52] = 0;
   out_6853406717773418208[53] = 0;
}
void h_14(double *state, double *unused, double *out_952452624527645277) {
   out_952452624527645277[0] = state[6];
   out_952452624527645277[1] = state[7];
   out_952452624527645277[2] = state[8];
}
void H_14(double *state, double *unused, double *out_6102439686766266480) {
   out_6102439686766266480[0] = 0;
   out_6102439686766266480[1] = 0;
   out_6102439686766266480[2] = 0;
   out_6102439686766266480[3] = 0;
   out_6102439686766266480[4] = 0;
   out_6102439686766266480[5] = 0;
   out_6102439686766266480[6] = 1;
   out_6102439686766266480[7] = 0;
   out_6102439686766266480[8] = 0;
   out_6102439686766266480[9] = 0;
   out_6102439686766266480[10] = 0;
   out_6102439686766266480[11] = 0;
   out_6102439686766266480[12] = 0;
   out_6102439686766266480[13] = 0;
   out_6102439686766266480[14] = 0;
   out_6102439686766266480[15] = 0;
   out_6102439686766266480[16] = 0;
   out_6102439686766266480[17] = 0;
   out_6102439686766266480[18] = 0;
   out_6102439686766266480[19] = 0;
   out_6102439686766266480[20] = 0;
   out_6102439686766266480[21] = 0;
   out_6102439686766266480[22] = 0;
   out_6102439686766266480[23] = 0;
   out_6102439686766266480[24] = 0;
   out_6102439686766266480[25] = 1;
   out_6102439686766266480[26] = 0;
   out_6102439686766266480[27] = 0;
   out_6102439686766266480[28] = 0;
   out_6102439686766266480[29] = 0;
   out_6102439686766266480[30] = 0;
   out_6102439686766266480[31] = 0;
   out_6102439686766266480[32] = 0;
   out_6102439686766266480[33] = 0;
   out_6102439686766266480[34] = 0;
   out_6102439686766266480[35] = 0;
   out_6102439686766266480[36] = 0;
   out_6102439686766266480[37] = 0;
   out_6102439686766266480[38] = 0;
   out_6102439686766266480[39] = 0;
   out_6102439686766266480[40] = 0;
   out_6102439686766266480[41] = 0;
   out_6102439686766266480[42] = 0;
   out_6102439686766266480[43] = 0;
   out_6102439686766266480[44] = 1;
   out_6102439686766266480[45] = 0;
   out_6102439686766266480[46] = 0;
   out_6102439686766266480[47] = 0;
   out_6102439686766266480[48] = 0;
   out_6102439686766266480[49] = 0;
   out_6102439686766266480[50] = 0;
   out_6102439686766266480[51] = 0;
   out_6102439686766266480[52] = 0;
   out_6102439686766266480[53] = 0;
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

void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_4, H_4, NULL, in_z, in_R, in_ea, MAHA_THRESH_4);
}
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_10, H_10, NULL, in_z, in_R, in_ea, MAHA_THRESH_10);
}
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_13, H_13, NULL, in_z, in_R, in_ea, MAHA_THRESH_13);
}
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_14, H_14, NULL, in_z, in_R, in_ea, MAHA_THRESH_14);
}
void pose_err_fun(double *nom_x, double *delta_x, double *out_2330460961423581716) {
  err_fun(nom_x, delta_x, out_2330460961423581716);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_6665394572644059121) {
  inv_err_fun(nom_x, true_x, out_6665394572644059121);
}
void pose_H_mod_fun(double *state, double *out_132265286258857251) {
  H_mod_fun(state, out_132265286258857251);
}
void pose_f_fun(double *state, double dt, double *out_8603722381005829308) {
  f_fun(state,  dt, out_8603722381005829308);
}
void pose_F_fun(double *state, double dt, double *out_2009381636234601862) {
  F_fun(state,  dt, out_2009381636234601862);
}
void pose_h_4(double *state, double *unused, double *out_6414518568457466023) {
  h_4(state, unused, out_6414518568457466023);
}
void pose_H_4(double *state, double *unused, double *out_8381063530603800607) {
  H_4(state, unused, out_8381063530603800607);
}
void pose_h_10(double *state, double *unused, double *out_1500850275664504396) {
  h_10(state, unused, out_1500850275664504396);
}
void pose_H_10(double *state, double *unused, double *out_4230206924188573634) {
  H_10(state, unused, out_4230206924188573634);
}
void pose_h_13(double *state, double *unused, double *out_2580468438894152282) {
  h_13(state, unused, out_2580468438894152282);
}
void pose_H_13(double *state, double *unused, double *out_6853406717773418208) {
  H_13(state, unused, out_6853406717773418208);
}
void pose_h_14(double *state, double *unused, double *out_952452624527645277) {
  h_14(state, unused, out_952452624527645277);
}
void pose_H_14(double *state, double *unused, double *out_6102439686766266480) {
  H_14(state, unused, out_6102439686766266480);
}
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
}

const EKF pose = {
  .name = "pose",
  .kinds = { 4, 10, 13, 14 },
  .feature_kinds = {  },
  .f_fun = pose_f_fun,
  .F_fun = pose_F_fun,
  .err_fun = pose_err_fun,
  .inv_err_fun = pose_inv_err_fun,
  .H_mod_fun = pose_H_mod_fun,
  .predict = pose_predict,
  .hs = {
    { 4, pose_h_4 },
    { 10, pose_h_10 },
    { 13, pose_h_13 },
    { 14, pose_h_14 },
  },
  .Hs = {
    { 4, pose_H_4 },
    { 10, pose_H_10 },
    { 13, pose_H_13 },
    { 14, pose_H_14 },
  },
  .updates = {
    { 4, pose_update_4 },
    { 10, pose_update_10 },
    { 13, pose_update_13 },
    { 14, pose_update_14 },
  },
  .Hes = {
  },
  .sets = {
  },
  .extra_routines = {
  },
};

ekf_lib_init(pose)
