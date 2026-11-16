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
void err_fun(double *nom_x, double *delta_x, double *out_4389229945795658064) {
   out_4389229945795658064[0] = delta_x[0] + nom_x[0];
   out_4389229945795658064[1] = delta_x[1] + nom_x[1];
   out_4389229945795658064[2] = delta_x[2] + nom_x[2];
   out_4389229945795658064[3] = delta_x[3] + nom_x[3];
   out_4389229945795658064[4] = delta_x[4] + nom_x[4];
   out_4389229945795658064[5] = delta_x[5] + nom_x[5];
   out_4389229945795658064[6] = delta_x[6] + nom_x[6];
   out_4389229945795658064[7] = delta_x[7] + nom_x[7];
   out_4389229945795658064[8] = delta_x[8] + nom_x[8];
   out_4389229945795658064[9] = delta_x[9] + nom_x[9];
   out_4389229945795658064[10] = delta_x[10] + nom_x[10];
   out_4389229945795658064[11] = delta_x[11] + nom_x[11];
   out_4389229945795658064[12] = delta_x[12] + nom_x[12];
   out_4389229945795658064[13] = delta_x[13] + nom_x[13];
   out_4389229945795658064[14] = delta_x[14] + nom_x[14];
   out_4389229945795658064[15] = delta_x[15] + nom_x[15];
   out_4389229945795658064[16] = delta_x[16] + nom_x[16];
   out_4389229945795658064[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_635646804366371573) {
   out_635646804366371573[0] = -nom_x[0] + true_x[0];
   out_635646804366371573[1] = -nom_x[1] + true_x[1];
   out_635646804366371573[2] = -nom_x[2] + true_x[2];
   out_635646804366371573[3] = -nom_x[3] + true_x[3];
   out_635646804366371573[4] = -nom_x[4] + true_x[4];
   out_635646804366371573[5] = -nom_x[5] + true_x[5];
   out_635646804366371573[6] = -nom_x[6] + true_x[6];
   out_635646804366371573[7] = -nom_x[7] + true_x[7];
   out_635646804366371573[8] = -nom_x[8] + true_x[8];
   out_635646804366371573[9] = -nom_x[9] + true_x[9];
   out_635646804366371573[10] = -nom_x[10] + true_x[10];
   out_635646804366371573[11] = -nom_x[11] + true_x[11];
   out_635646804366371573[12] = -nom_x[12] + true_x[12];
   out_635646804366371573[13] = -nom_x[13] + true_x[13];
   out_635646804366371573[14] = -nom_x[14] + true_x[14];
   out_635646804366371573[15] = -nom_x[15] + true_x[15];
   out_635646804366371573[16] = -nom_x[16] + true_x[16];
   out_635646804366371573[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_1592105825884835830) {
   out_1592105825884835830[0] = 1.0;
   out_1592105825884835830[1] = 0.0;
   out_1592105825884835830[2] = 0.0;
   out_1592105825884835830[3] = 0.0;
   out_1592105825884835830[4] = 0.0;
   out_1592105825884835830[5] = 0.0;
   out_1592105825884835830[6] = 0.0;
   out_1592105825884835830[7] = 0.0;
   out_1592105825884835830[8] = 0.0;
   out_1592105825884835830[9] = 0.0;
   out_1592105825884835830[10] = 0.0;
   out_1592105825884835830[11] = 0.0;
   out_1592105825884835830[12] = 0.0;
   out_1592105825884835830[13] = 0.0;
   out_1592105825884835830[14] = 0.0;
   out_1592105825884835830[15] = 0.0;
   out_1592105825884835830[16] = 0.0;
   out_1592105825884835830[17] = 0.0;
   out_1592105825884835830[18] = 0.0;
   out_1592105825884835830[19] = 1.0;
   out_1592105825884835830[20] = 0.0;
   out_1592105825884835830[21] = 0.0;
   out_1592105825884835830[22] = 0.0;
   out_1592105825884835830[23] = 0.0;
   out_1592105825884835830[24] = 0.0;
   out_1592105825884835830[25] = 0.0;
   out_1592105825884835830[26] = 0.0;
   out_1592105825884835830[27] = 0.0;
   out_1592105825884835830[28] = 0.0;
   out_1592105825884835830[29] = 0.0;
   out_1592105825884835830[30] = 0.0;
   out_1592105825884835830[31] = 0.0;
   out_1592105825884835830[32] = 0.0;
   out_1592105825884835830[33] = 0.0;
   out_1592105825884835830[34] = 0.0;
   out_1592105825884835830[35] = 0.0;
   out_1592105825884835830[36] = 0.0;
   out_1592105825884835830[37] = 0.0;
   out_1592105825884835830[38] = 1.0;
   out_1592105825884835830[39] = 0.0;
   out_1592105825884835830[40] = 0.0;
   out_1592105825884835830[41] = 0.0;
   out_1592105825884835830[42] = 0.0;
   out_1592105825884835830[43] = 0.0;
   out_1592105825884835830[44] = 0.0;
   out_1592105825884835830[45] = 0.0;
   out_1592105825884835830[46] = 0.0;
   out_1592105825884835830[47] = 0.0;
   out_1592105825884835830[48] = 0.0;
   out_1592105825884835830[49] = 0.0;
   out_1592105825884835830[50] = 0.0;
   out_1592105825884835830[51] = 0.0;
   out_1592105825884835830[52] = 0.0;
   out_1592105825884835830[53] = 0.0;
   out_1592105825884835830[54] = 0.0;
   out_1592105825884835830[55] = 0.0;
   out_1592105825884835830[56] = 0.0;
   out_1592105825884835830[57] = 1.0;
   out_1592105825884835830[58] = 0.0;
   out_1592105825884835830[59] = 0.0;
   out_1592105825884835830[60] = 0.0;
   out_1592105825884835830[61] = 0.0;
   out_1592105825884835830[62] = 0.0;
   out_1592105825884835830[63] = 0.0;
   out_1592105825884835830[64] = 0.0;
   out_1592105825884835830[65] = 0.0;
   out_1592105825884835830[66] = 0.0;
   out_1592105825884835830[67] = 0.0;
   out_1592105825884835830[68] = 0.0;
   out_1592105825884835830[69] = 0.0;
   out_1592105825884835830[70] = 0.0;
   out_1592105825884835830[71] = 0.0;
   out_1592105825884835830[72] = 0.0;
   out_1592105825884835830[73] = 0.0;
   out_1592105825884835830[74] = 0.0;
   out_1592105825884835830[75] = 0.0;
   out_1592105825884835830[76] = 1.0;
   out_1592105825884835830[77] = 0.0;
   out_1592105825884835830[78] = 0.0;
   out_1592105825884835830[79] = 0.0;
   out_1592105825884835830[80] = 0.0;
   out_1592105825884835830[81] = 0.0;
   out_1592105825884835830[82] = 0.0;
   out_1592105825884835830[83] = 0.0;
   out_1592105825884835830[84] = 0.0;
   out_1592105825884835830[85] = 0.0;
   out_1592105825884835830[86] = 0.0;
   out_1592105825884835830[87] = 0.0;
   out_1592105825884835830[88] = 0.0;
   out_1592105825884835830[89] = 0.0;
   out_1592105825884835830[90] = 0.0;
   out_1592105825884835830[91] = 0.0;
   out_1592105825884835830[92] = 0.0;
   out_1592105825884835830[93] = 0.0;
   out_1592105825884835830[94] = 0.0;
   out_1592105825884835830[95] = 1.0;
   out_1592105825884835830[96] = 0.0;
   out_1592105825884835830[97] = 0.0;
   out_1592105825884835830[98] = 0.0;
   out_1592105825884835830[99] = 0.0;
   out_1592105825884835830[100] = 0.0;
   out_1592105825884835830[101] = 0.0;
   out_1592105825884835830[102] = 0.0;
   out_1592105825884835830[103] = 0.0;
   out_1592105825884835830[104] = 0.0;
   out_1592105825884835830[105] = 0.0;
   out_1592105825884835830[106] = 0.0;
   out_1592105825884835830[107] = 0.0;
   out_1592105825884835830[108] = 0.0;
   out_1592105825884835830[109] = 0.0;
   out_1592105825884835830[110] = 0.0;
   out_1592105825884835830[111] = 0.0;
   out_1592105825884835830[112] = 0.0;
   out_1592105825884835830[113] = 0.0;
   out_1592105825884835830[114] = 1.0;
   out_1592105825884835830[115] = 0.0;
   out_1592105825884835830[116] = 0.0;
   out_1592105825884835830[117] = 0.0;
   out_1592105825884835830[118] = 0.0;
   out_1592105825884835830[119] = 0.0;
   out_1592105825884835830[120] = 0.0;
   out_1592105825884835830[121] = 0.0;
   out_1592105825884835830[122] = 0.0;
   out_1592105825884835830[123] = 0.0;
   out_1592105825884835830[124] = 0.0;
   out_1592105825884835830[125] = 0.0;
   out_1592105825884835830[126] = 0.0;
   out_1592105825884835830[127] = 0.0;
   out_1592105825884835830[128] = 0.0;
   out_1592105825884835830[129] = 0.0;
   out_1592105825884835830[130] = 0.0;
   out_1592105825884835830[131] = 0.0;
   out_1592105825884835830[132] = 0.0;
   out_1592105825884835830[133] = 1.0;
   out_1592105825884835830[134] = 0.0;
   out_1592105825884835830[135] = 0.0;
   out_1592105825884835830[136] = 0.0;
   out_1592105825884835830[137] = 0.0;
   out_1592105825884835830[138] = 0.0;
   out_1592105825884835830[139] = 0.0;
   out_1592105825884835830[140] = 0.0;
   out_1592105825884835830[141] = 0.0;
   out_1592105825884835830[142] = 0.0;
   out_1592105825884835830[143] = 0.0;
   out_1592105825884835830[144] = 0.0;
   out_1592105825884835830[145] = 0.0;
   out_1592105825884835830[146] = 0.0;
   out_1592105825884835830[147] = 0.0;
   out_1592105825884835830[148] = 0.0;
   out_1592105825884835830[149] = 0.0;
   out_1592105825884835830[150] = 0.0;
   out_1592105825884835830[151] = 0.0;
   out_1592105825884835830[152] = 1.0;
   out_1592105825884835830[153] = 0.0;
   out_1592105825884835830[154] = 0.0;
   out_1592105825884835830[155] = 0.0;
   out_1592105825884835830[156] = 0.0;
   out_1592105825884835830[157] = 0.0;
   out_1592105825884835830[158] = 0.0;
   out_1592105825884835830[159] = 0.0;
   out_1592105825884835830[160] = 0.0;
   out_1592105825884835830[161] = 0.0;
   out_1592105825884835830[162] = 0.0;
   out_1592105825884835830[163] = 0.0;
   out_1592105825884835830[164] = 0.0;
   out_1592105825884835830[165] = 0.0;
   out_1592105825884835830[166] = 0.0;
   out_1592105825884835830[167] = 0.0;
   out_1592105825884835830[168] = 0.0;
   out_1592105825884835830[169] = 0.0;
   out_1592105825884835830[170] = 0.0;
   out_1592105825884835830[171] = 1.0;
   out_1592105825884835830[172] = 0.0;
   out_1592105825884835830[173] = 0.0;
   out_1592105825884835830[174] = 0.0;
   out_1592105825884835830[175] = 0.0;
   out_1592105825884835830[176] = 0.0;
   out_1592105825884835830[177] = 0.0;
   out_1592105825884835830[178] = 0.0;
   out_1592105825884835830[179] = 0.0;
   out_1592105825884835830[180] = 0.0;
   out_1592105825884835830[181] = 0.0;
   out_1592105825884835830[182] = 0.0;
   out_1592105825884835830[183] = 0.0;
   out_1592105825884835830[184] = 0.0;
   out_1592105825884835830[185] = 0.0;
   out_1592105825884835830[186] = 0.0;
   out_1592105825884835830[187] = 0.0;
   out_1592105825884835830[188] = 0.0;
   out_1592105825884835830[189] = 0.0;
   out_1592105825884835830[190] = 1.0;
   out_1592105825884835830[191] = 0.0;
   out_1592105825884835830[192] = 0.0;
   out_1592105825884835830[193] = 0.0;
   out_1592105825884835830[194] = 0.0;
   out_1592105825884835830[195] = 0.0;
   out_1592105825884835830[196] = 0.0;
   out_1592105825884835830[197] = 0.0;
   out_1592105825884835830[198] = 0.0;
   out_1592105825884835830[199] = 0.0;
   out_1592105825884835830[200] = 0.0;
   out_1592105825884835830[201] = 0.0;
   out_1592105825884835830[202] = 0.0;
   out_1592105825884835830[203] = 0.0;
   out_1592105825884835830[204] = 0.0;
   out_1592105825884835830[205] = 0.0;
   out_1592105825884835830[206] = 0.0;
   out_1592105825884835830[207] = 0.0;
   out_1592105825884835830[208] = 0.0;
   out_1592105825884835830[209] = 1.0;
   out_1592105825884835830[210] = 0.0;
   out_1592105825884835830[211] = 0.0;
   out_1592105825884835830[212] = 0.0;
   out_1592105825884835830[213] = 0.0;
   out_1592105825884835830[214] = 0.0;
   out_1592105825884835830[215] = 0.0;
   out_1592105825884835830[216] = 0.0;
   out_1592105825884835830[217] = 0.0;
   out_1592105825884835830[218] = 0.0;
   out_1592105825884835830[219] = 0.0;
   out_1592105825884835830[220] = 0.0;
   out_1592105825884835830[221] = 0.0;
   out_1592105825884835830[222] = 0.0;
   out_1592105825884835830[223] = 0.0;
   out_1592105825884835830[224] = 0.0;
   out_1592105825884835830[225] = 0.0;
   out_1592105825884835830[226] = 0.0;
   out_1592105825884835830[227] = 0.0;
   out_1592105825884835830[228] = 1.0;
   out_1592105825884835830[229] = 0.0;
   out_1592105825884835830[230] = 0.0;
   out_1592105825884835830[231] = 0.0;
   out_1592105825884835830[232] = 0.0;
   out_1592105825884835830[233] = 0.0;
   out_1592105825884835830[234] = 0.0;
   out_1592105825884835830[235] = 0.0;
   out_1592105825884835830[236] = 0.0;
   out_1592105825884835830[237] = 0.0;
   out_1592105825884835830[238] = 0.0;
   out_1592105825884835830[239] = 0.0;
   out_1592105825884835830[240] = 0.0;
   out_1592105825884835830[241] = 0.0;
   out_1592105825884835830[242] = 0.0;
   out_1592105825884835830[243] = 0.0;
   out_1592105825884835830[244] = 0.0;
   out_1592105825884835830[245] = 0.0;
   out_1592105825884835830[246] = 0.0;
   out_1592105825884835830[247] = 1.0;
   out_1592105825884835830[248] = 0.0;
   out_1592105825884835830[249] = 0.0;
   out_1592105825884835830[250] = 0.0;
   out_1592105825884835830[251] = 0.0;
   out_1592105825884835830[252] = 0.0;
   out_1592105825884835830[253] = 0.0;
   out_1592105825884835830[254] = 0.0;
   out_1592105825884835830[255] = 0.0;
   out_1592105825884835830[256] = 0.0;
   out_1592105825884835830[257] = 0.0;
   out_1592105825884835830[258] = 0.0;
   out_1592105825884835830[259] = 0.0;
   out_1592105825884835830[260] = 0.0;
   out_1592105825884835830[261] = 0.0;
   out_1592105825884835830[262] = 0.0;
   out_1592105825884835830[263] = 0.0;
   out_1592105825884835830[264] = 0.0;
   out_1592105825884835830[265] = 0.0;
   out_1592105825884835830[266] = 1.0;
   out_1592105825884835830[267] = 0.0;
   out_1592105825884835830[268] = 0.0;
   out_1592105825884835830[269] = 0.0;
   out_1592105825884835830[270] = 0.0;
   out_1592105825884835830[271] = 0.0;
   out_1592105825884835830[272] = 0.0;
   out_1592105825884835830[273] = 0.0;
   out_1592105825884835830[274] = 0.0;
   out_1592105825884835830[275] = 0.0;
   out_1592105825884835830[276] = 0.0;
   out_1592105825884835830[277] = 0.0;
   out_1592105825884835830[278] = 0.0;
   out_1592105825884835830[279] = 0.0;
   out_1592105825884835830[280] = 0.0;
   out_1592105825884835830[281] = 0.0;
   out_1592105825884835830[282] = 0.0;
   out_1592105825884835830[283] = 0.0;
   out_1592105825884835830[284] = 0.0;
   out_1592105825884835830[285] = 1.0;
   out_1592105825884835830[286] = 0.0;
   out_1592105825884835830[287] = 0.0;
   out_1592105825884835830[288] = 0.0;
   out_1592105825884835830[289] = 0.0;
   out_1592105825884835830[290] = 0.0;
   out_1592105825884835830[291] = 0.0;
   out_1592105825884835830[292] = 0.0;
   out_1592105825884835830[293] = 0.0;
   out_1592105825884835830[294] = 0.0;
   out_1592105825884835830[295] = 0.0;
   out_1592105825884835830[296] = 0.0;
   out_1592105825884835830[297] = 0.0;
   out_1592105825884835830[298] = 0.0;
   out_1592105825884835830[299] = 0.0;
   out_1592105825884835830[300] = 0.0;
   out_1592105825884835830[301] = 0.0;
   out_1592105825884835830[302] = 0.0;
   out_1592105825884835830[303] = 0.0;
   out_1592105825884835830[304] = 1.0;
   out_1592105825884835830[305] = 0.0;
   out_1592105825884835830[306] = 0.0;
   out_1592105825884835830[307] = 0.0;
   out_1592105825884835830[308] = 0.0;
   out_1592105825884835830[309] = 0.0;
   out_1592105825884835830[310] = 0.0;
   out_1592105825884835830[311] = 0.0;
   out_1592105825884835830[312] = 0.0;
   out_1592105825884835830[313] = 0.0;
   out_1592105825884835830[314] = 0.0;
   out_1592105825884835830[315] = 0.0;
   out_1592105825884835830[316] = 0.0;
   out_1592105825884835830[317] = 0.0;
   out_1592105825884835830[318] = 0.0;
   out_1592105825884835830[319] = 0.0;
   out_1592105825884835830[320] = 0.0;
   out_1592105825884835830[321] = 0.0;
   out_1592105825884835830[322] = 0.0;
   out_1592105825884835830[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_5328593654681060988) {
   out_5328593654681060988[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_5328593654681060988[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_5328593654681060988[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_5328593654681060988[3] = dt*state[12] + state[3];
   out_5328593654681060988[4] = dt*state[13] + state[4];
   out_5328593654681060988[5] = dt*state[14] + state[5];
   out_5328593654681060988[6] = state[6];
   out_5328593654681060988[7] = state[7];
   out_5328593654681060988[8] = state[8];
   out_5328593654681060988[9] = state[9];
   out_5328593654681060988[10] = state[10];
   out_5328593654681060988[11] = state[11];
   out_5328593654681060988[12] = state[12];
   out_5328593654681060988[13] = state[13];
   out_5328593654681060988[14] = state[14];
   out_5328593654681060988[15] = state[15];
   out_5328593654681060988[16] = state[16];
   out_5328593654681060988[17] = state[17];
}
void F_fun(double *state, double dt, double *out_1459903262957102476) {
   out_1459903262957102476[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1459903262957102476[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1459903262957102476[2] = 0;
   out_1459903262957102476[3] = 0;
   out_1459903262957102476[4] = 0;
   out_1459903262957102476[5] = 0;
   out_1459903262957102476[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1459903262957102476[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1459903262957102476[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1459903262957102476[9] = 0;
   out_1459903262957102476[10] = 0;
   out_1459903262957102476[11] = 0;
   out_1459903262957102476[12] = 0;
   out_1459903262957102476[13] = 0;
   out_1459903262957102476[14] = 0;
   out_1459903262957102476[15] = 0;
   out_1459903262957102476[16] = 0;
   out_1459903262957102476[17] = 0;
   out_1459903262957102476[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1459903262957102476[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1459903262957102476[20] = 0;
   out_1459903262957102476[21] = 0;
   out_1459903262957102476[22] = 0;
   out_1459903262957102476[23] = 0;
   out_1459903262957102476[24] = 0;
   out_1459903262957102476[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1459903262957102476[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1459903262957102476[27] = 0;
   out_1459903262957102476[28] = 0;
   out_1459903262957102476[29] = 0;
   out_1459903262957102476[30] = 0;
   out_1459903262957102476[31] = 0;
   out_1459903262957102476[32] = 0;
   out_1459903262957102476[33] = 0;
   out_1459903262957102476[34] = 0;
   out_1459903262957102476[35] = 0;
   out_1459903262957102476[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1459903262957102476[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1459903262957102476[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1459903262957102476[39] = 0;
   out_1459903262957102476[40] = 0;
   out_1459903262957102476[41] = 0;
   out_1459903262957102476[42] = 0;
   out_1459903262957102476[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1459903262957102476[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1459903262957102476[45] = 0;
   out_1459903262957102476[46] = 0;
   out_1459903262957102476[47] = 0;
   out_1459903262957102476[48] = 0;
   out_1459903262957102476[49] = 0;
   out_1459903262957102476[50] = 0;
   out_1459903262957102476[51] = 0;
   out_1459903262957102476[52] = 0;
   out_1459903262957102476[53] = 0;
   out_1459903262957102476[54] = 0;
   out_1459903262957102476[55] = 0;
   out_1459903262957102476[56] = 0;
   out_1459903262957102476[57] = 1;
   out_1459903262957102476[58] = 0;
   out_1459903262957102476[59] = 0;
   out_1459903262957102476[60] = 0;
   out_1459903262957102476[61] = 0;
   out_1459903262957102476[62] = 0;
   out_1459903262957102476[63] = 0;
   out_1459903262957102476[64] = 0;
   out_1459903262957102476[65] = 0;
   out_1459903262957102476[66] = dt;
   out_1459903262957102476[67] = 0;
   out_1459903262957102476[68] = 0;
   out_1459903262957102476[69] = 0;
   out_1459903262957102476[70] = 0;
   out_1459903262957102476[71] = 0;
   out_1459903262957102476[72] = 0;
   out_1459903262957102476[73] = 0;
   out_1459903262957102476[74] = 0;
   out_1459903262957102476[75] = 0;
   out_1459903262957102476[76] = 1;
   out_1459903262957102476[77] = 0;
   out_1459903262957102476[78] = 0;
   out_1459903262957102476[79] = 0;
   out_1459903262957102476[80] = 0;
   out_1459903262957102476[81] = 0;
   out_1459903262957102476[82] = 0;
   out_1459903262957102476[83] = 0;
   out_1459903262957102476[84] = 0;
   out_1459903262957102476[85] = dt;
   out_1459903262957102476[86] = 0;
   out_1459903262957102476[87] = 0;
   out_1459903262957102476[88] = 0;
   out_1459903262957102476[89] = 0;
   out_1459903262957102476[90] = 0;
   out_1459903262957102476[91] = 0;
   out_1459903262957102476[92] = 0;
   out_1459903262957102476[93] = 0;
   out_1459903262957102476[94] = 0;
   out_1459903262957102476[95] = 1;
   out_1459903262957102476[96] = 0;
   out_1459903262957102476[97] = 0;
   out_1459903262957102476[98] = 0;
   out_1459903262957102476[99] = 0;
   out_1459903262957102476[100] = 0;
   out_1459903262957102476[101] = 0;
   out_1459903262957102476[102] = 0;
   out_1459903262957102476[103] = 0;
   out_1459903262957102476[104] = dt;
   out_1459903262957102476[105] = 0;
   out_1459903262957102476[106] = 0;
   out_1459903262957102476[107] = 0;
   out_1459903262957102476[108] = 0;
   out_1459903262957102476[109] = 0;
   out_1459903262957102476[110] = 0;
   out_1459903262957102476[111] = 0;
   out_1459903262957102476[112] = 0;
   out_1459903262957102476[113] = 0;
   out_1459903262957102476[114] = 1;
   out_1459903262957102476[115] = 0;
   out_1459903262957102476[116] = 0;
   out_1459903262957102476[117] = 0;
   out_1459903262957102476[118] = 0;
   out_1459903262957102476[119] = 0;
   out_1459903262957102476[120] = 0;
   out_1459903262957102476[121] = 0;
   out_1459903262957102476[122] = 0;
   out_1459903262957102476[123] = 0;
   out_1459903262957102476[124] = 0;
   out_1459903262957102476[125] = 0;
   out_1459903262957102476[126] = 0;
   out_1459903262957102476[127] = 0;
   out_1459903262957102476[128] = 0;
   out_1459903262957102476[129] = 0;
   out_1459903262957102476[130] = 0;
   out_1459903262957102476[131] = 0;
   out_1459903262957102476[132] = 0;
   out_1459903262957102476[133] = 1;
   out_1459903262957102476[134] = 0;
   out_1459903262957102476[135] = 0;
   out_1459903262957102476[136] = 0;
   out_1459903262957102476[137] = 0;
   out_1459903262957102476[138] = 0;
   out_1459903262957102476[139] = 0;
   out_1459903262957102476[140] = 0;
   out_1459903262957102476[141] = 0;
   out_1459903262957102476[142] = 0;
   out_1459903262957102476[143] = 0;
   out_1459903262957102476[144] = 0;
   out_1459903262957102476[145] = 0;
   out_1459903262957102476[146] = 0;
   out_1459903262957102476[147] = 0;
   out_1459903262957102476[148] = 0;
   out_1459903262957102476[149] = 0;
   out_1459903262957102476[150] = 0;
   out_1459903262957102476[151] = 0;
   out_1459903262957102476[152] = 1;
   out_1459903262957102476[153] = 0;
   out_1459903262957102476[154] = 0;
   out_1459903262957102476[155] = 0;
   out_1459903262957102476[156] = 0;
   out_1459903262957102476[157] = 0;
   out_1459903262957102476[158] = 0;
   out_1459903262957102476[159] = 0;
   out_1459903262957102476[160] = 0;
   out_1459903262957102476[161] = 0;
   out_1459903262957102476[162] = 0;
   out_1459903262957102476[163] = 0;
   out_1459903262957102476[164] = 0;
   out_1459903262957102476[165] = 0;
   out_1459903262957102476[166] = 0;
   out_1459903262957102476[167] = 0;
   out_1459903262957102476[168] = 0;
   out_1459903262957102476[169] = 0;
   out_1459903262957102476[170] = 0;
   out_1459903262957102476[171] = 1;
   out_1459903262957102476[172] = 0;
   out_1459903262957102476[173] = 0;
   out_1459903262957102476[174] = 0;
   out_1459903262957102476[175] = 0;
   out_1459903262957102476[176] = 0;
   out_1459903262957102476[177] = 0;
   out_1459903262957102476[178] = 0;
   out_1459903262957102476[179] = 0;
   out_1459903262957102476[180] = 0;
   out_1459903262957102476[181] = 0;
   out_1459903262957102476[182] = 0;
   out_1459903262957102476[183] = 0;
   out_1459903262957102476[184] = 0;
   out_1459903262957102476[185] = 0;
   out_1459903262957102476[186] = 0;
   out_1459903262957102476[187] = 0;
   out_1459903262957102476[188] = 0;
   out_1459903262957102476[189] = 0;
   out_1459903262957102476[190] = 1;
   out_1459903262957102476[191] = 0;
   out_1459903262957102476[192] = 0;
   out_1459903262957102476[193] = 0;
   out_1459903262957102476[194] = 0;
   out_1459903262957102476[195] = 0;
   out_1459903262957102476[196] = 0;
   out_1459903262957102476[197] = 0;
   out_1459903262957102476[198] = 0;
   out_1459903262957102476[199] = 0;
   out_1459903262957102476[200] = 0;
   out_1459903262957102476[201] = 0;
   out_1459903262957102476[202] = 0;
   out_1459903262957102476[203] = 0;
   out_1459903262957102476[204] = 0;
   out_1459903262957102476[205] = 0;
   out_1459903262957102476[206] = 0;
   out_1459903262957102476[207] = 0;
   out_1459903262957102476[208] = 0;
   out_1459903262957102476[209] = 1;
   out_1459903262957102476[210] = 0;
   out_1459903262957102476[211] = 0;
   out_1459903262957102476[212] = 0;
   out_1459903262957102476[213] = 0;
   out_1459903262957102476[214] = 0;
   out_1459903262957102476[215] = 0;
   out_1459903262957102476[216] = 0;
   out_1459903262957102476[217] = 0;
   out_1459903262957102476[218] = 0;
   out_1459903262957102476[219] = 0;
   out_1459903262957102476[220] = 0;
   out_1459903262957102476[221] = 0;
   out_1459903262957102476[222] = 0;
   out_1459903262957102476[223] = 0;
   out_1459903262957102476[224] = 0;
   out_1459903262957102476[225] = 0;
   out_1459903262957102476[226] = 0;
   out_1459903262957102476[227] = 0;
   out_1459903262957102476[228] = 1;
   out_1459903262957102476[229] = 0;
   out_1459903262957102476[230] = 0;
   out_1459903262957102476[231] = 0;
   out_1459903262957102476[232] = 0;
   out_1459903262957102476[233] = 0;
   out_1459903262957102476[234] = 0;
   out_1459903262957102476[235] = 0;
   out_1459903262957102476[236] = 0;
   out_1459903262957102476[237] = 0;
   out_1459903262957102476[238] = 0;
   out_1459903262957102476[239] = 0;
   out_1459903262957102476[240] = 0;
   out_1459903262957102476[241] = 0;
   out_1459903262957102476[242] = 0;
   out_1459903262957102476[243] = 0;
   out_1459903262957102476[244] = 0;
   out_1459903262957102476[245] = 0;
   out_1459903262957102476[246] = 0;
   out_1459903262957102476[247] = 1;
   out_1459903262957102476[248] = 0;
   out_1459903262957102476[249] = 0;
   out_1459903262957102476[250] = 0;
   out_1459903262957102476[251] = 0;
   out_1459903262957102476[252] = 0;
   out_1459903262957102476[253] = 0;
   out_1459903262957102476[254] = 0;
   out_1459903262957102476[255] = 0;
   out_1459903262957102476[256] = 0;
   out_1459903262957102476[257] = 0;
   out_1459903262957102476[258] = 0;
   out_1459903262957102476[259] = 0;
   out_1459903262957102476[260] = 0;
   out_1459903262957102476[261] = 0;
   out_1459903262957102476[262] = 0;
   out_1459903262957102476[263] = 0;
   out_1459903262957102476[264] = 0;
   out_1459903262957102476[265] = 0;
   out_1459903262957102476[266] = 1;
   out_1459903262957102476[267] = 0;
   out_1459903262957102476[268] = 0;
   out_1459903262957102476[269] = 0;
   out_1459903262957102476[270] = 0;
   out_1459903262957102476[271] = 0;
   out_1459903262957102476[272] = 0;
   out_1459903262957102476[273] = 0;
   out_1459903262957102476[274] = 0;
   out_1459903262957102476[275] = 0;
   out_1459903262957102476[276] = 0;
   out_1459903262957102476[277] = 0;
   out_1459903262957102476[278] = 0;
   out_1459903262957102476[279] = 0;
   out_1459903262957102476[280] = 0;
   out_1459903262957102476[281] = 0;
   out_1459903262957102476[282] = 0;
   out_1459903262957102476[283] = 0;
   out_1459903262957102476[284] = 0;
   out_1459903262957102476[285] = 1;
   out_1459903262957102476[286] = 0;
   out_1459903262957102476[287] = 0;
   out_1459903262957102476[288] = 0;
   out_1459903262957102476[289] = 0;
   out_1459903262957102476[290] = 0;
   out_1459903262957102476[291] = 0;
   out_1459903262957102476[292] = 0;
   out_1459903262957102476[293] = 0;
   out_1459903262957102476[294] = 0;
   out_1459903262957102476[295] = 0;
   out_1459903262957102476[296] = 0;
   out_1459903262957102476[297] = 0;
   out_1459903262957102476[298] = 0;
   out_1459903262957102476[299] = 0;
   out_1459903262957102476[300] = 0;
   out_1459903262957102476[301] = 0;
   out_1459903262957102476[302] = 0;
   out_1459903262957102476[303] = 0;
   out_1459903262957102476[304] = 1;
   out_1459903262957102476[305] = 0;
   out_1459903262957102476[306] = 0;
   out_1459903262957102476[307] = 0;
   out_1459903262957102476[308] = 0;
   out_1459903262957102476[309] = 0;
   out_1459903262957102476[310] = 0;
   out_1459903262957102476[311] = 0;
   out_1459903262957102476[312] = 0;
   out_1459903262957102476[313] = 0;
   out_1459903262957102476[314] = 0;
   out_1459903262957102476[315] = 0;
   out_1459903262957102476[316] = 0;
   out_1459903262957102476[317] = 0;
   out_1459903262957102476[318] = 0;
   out_1459903262957102476[319] = 0;
   out_1459903262957102476[320] = 0;
   out_1459903262957102476[321] = 0;
   out_1459903262957102476[322] = 0;
   out_1459903262957102476[323] = 1;
}
void h_4(double *state, double *unused, double *out_1573274557216908386) {
   out_1573274557216908386[0] = state[6] + state[9];
   out_1573274557216908386[1] = state[7] + state[10];
   out_1573274557216908386[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_7440187632636794551) {
   out_7440187632636794551[0] = 0;
   out_7440187632636794551[1] = 0;
   out_7440187632636794551[2] = 0;
   out_7440187632636794551[3] = 0;
   out_7440187632636794551[4] = 0;
   out_7440187632636794551[5] = 0;
   out_7440187632636794551[6] = 1;
   out_7440187632636794551[7] = 0;
   out_7440187632636794551[8] = 0;
   out_7440187632636794551[9] = 1;
   out_7440187632636794551[10] = 0;
   out_7440187632636794551[11] = 0;
   out_7440187632636794551[12] = 0;
   out_7440187632636794551[13] = 0;
   out_7440187632636794551[14] = 0;
   out_7440187632636794551[15] = 0;
   out_7440187632636794551[16] = 0;
   out_7440187632636794551[17] = 0;
   out_7440187632636794551[18] = 0;
   out_7440187632636794551[19] = 0;
   out_7440187632636794551[20] = 0;
   out_7440187632636794551[21] = 0;
   out_7440187632636794551[22] = 0;
   out_7440187632636794551[23] = 0;
   out_7440187632636794551[24] = 0;
   out_7440187632636794551[25] = 1;
   out_7440187632636794551[26] = 0;
   out_7440187632636794551[27] = 0;
   out_7440187632636794551[28] = 1;
   out_7440187632636794551[29] = 0;
   out_7440187632636794551[30] = 0;
   out_7440187632636794551[31] = 0;
   out_7440187632636794551[32] = 0;
   out_7440187632636794551[33] = 0;
   out_7440187632636794551[34] = 0;
   out_7440187632636794551[35] = 0;
   out_7440187632636794551[36] = 0;
   out_7440187632636794551[37] = 0;
   out_7440187632636794551[38] = 0;
   out_7440187632636794551[39] = 0;
   out_7440187632636794551[40] = 0;
   out_7440187632636794551[41] = 0;
   out_7440187632636794551[42] = 0;
   out_7440187632636794551[43] = 0;
   out_7440187632636794551[44] = 1;
   out_7440187632636794551[45] = 0;
   out_7440187632636794551[46] = 0;
   out_7440187632636794551[47] = 1;
   out_7440187632636794551[48] = 0;
   out_7440187632636794551[49] = 0;
   out_7440187632636794551[50] = 0;
   out_7440187632636794551[51] = 0;
   out_7440187632636794551[52] = 0;
   out_7440187632636794551[53] = 0;
}
void h_10(double *state, double *unused, double *out_469758277648148749) {
   out_469758277648148749[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_469758277648148749[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_469758277648148749[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_5199566927053226908) {
   out_5199566927053226908[0] = 0;
   out_5199566927053226908[1] = 9.8100000000000005*cos(state[1]);
   out_5199566927053226908[2] = 0;
   out_5199566927053226908[3] = 0;
   out_5199566927053226908[4] = -state[8];
   out_5199566927053226908[5] = state[7];
   out_5199566927053226908[6] = 0;
   out_5199566927053226908[7] = state[5];
   out_5199566927053226908[8] = -state[4];
   out_5199566927053226908[9] = 0;
   out_5199566927053226908[10] = 0;
   out_5199566927053226908[11] = 0;
   out_5199566927053226908[12] = 1;
   out_5199566927053226908[13] = 0;
   out_5199566927053226908[14] = 0;
   out_5199566927053226908[15] = 1;
   out_5199566927053226908[16] = 0;
   out_5199566927053226908[17] = 0;
   out_5199566927053226908[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_5199566927053226908[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_5199566927053226908[20] = 0;
   out_5199566927053226908[21] = state[8];
   out_5199566927053226908[22] = 0;
   out_5199566927053226908[23] = -state[6];
   out_5199566927053226908[24] = -state[5];
   out_5199566927053226908[25] = 0;
   out_5199566927053226908[26] = state[3];
   out_5199566927053226908[27] = 0;
   out_5199566927053226908[28] = 0;
   out_5199566927053226908[29] = 0;
   out_5199566927053226908[30] = 0;
   out_5199566927053226908[31] = 1;
   out_5199566927053226908[32] = 0;
   out_5199566927053226908[33] = 0;
   out_5199566927053226908[34] = 1;
   out_5199566927053226908[35] = 0;
   out_5199566927053226908[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_5199566927053226908[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_5199566927053226908[38] = 0;
   out_5199566927053226908[39] = -state[7];
   out_5199566927053226908[40] = state[6];
   out_5199566927053226908[41] = 0;
   out_5199566927053226908[42] = state[4];
   out_5199566927053226908[43] = -state[3];
   out_5199566927053226908[44] = 0;
   out_5199566927053226908[45] = 0;
   out_5199566927053226908[46] = 0;
   out_5199566927053226908[47] = 0;
   out_5199566927053226908[48] = 0;
   out_5199566927053226908[49] = 0;
   out_5199566927053226908[50] = 1;
   out_5199566927053226908[51] = 0;
   out_5199566927053226908[52] = 0;
   out_5199566927053226908[53] = 1;
}
void h_13(double *state, double *unused, double *out_29074149891970277) {
   out_29074149891970277[0] = state[3];
   out_29074149891970277[1] = state[4];
   out_29074149891970277[2] = state[5];
}
void H_13(double *state, double *unused, double *out_4227913807304461750) {
   out_4227913807304461750[0] = 0;
   out_4227913807304461750[1] = 0;
   out_4227913807304461750[2] = 0;
   out_4227913807304461750[3] = 1;
   out_4227913807304461750[4] = 0;
   out_4227913807304461750[5] = 0;
   out_4227913807304461750[6] = 0;
   out_4227913807304461750[7] = 0;
   out_4227913807304461750[8] = 0;
   out_4227913807304461750[9] = 0;
   out_4227913807304461750[10] = 0;
   out_4227913807304461750[11] = 0;
   out_4227913807304461750[12] = 0;
   out_4227913807304461750[13] = 0;
   out_4227913807304461750[14] = 0;
   out_4227913807304461750[15] = 0;
   out_4227913807304461750[16] = 0;
   out_4227913807304461750[17] = 0;
   out_4227913807304461750[18] = 0;
   out_4227913807304461750[19] = 0;
   out_4227913807304461750[20] = 0;
   out_4227913807304461750[21] = 0;
   out_4227913807304461750[22] = 1;
   out_4227913807304461750[23] = 0;
   out_4227913807304461750[24] = 0;
   out_4227913807304461750[25] = 0;
   out_4227913807304461750[26] = 0;
   out_4227913807304461750[27] = 0;
   out_4227913807304461750[28] = 0;
   out_4227913807304461750[29] = 0;
   out_4227913807304461750[30] = 0;
   out_4227913807304461750[31] = 0;
   out_4227913807304461750[32] = 0;
   out_4227913807304461750[33] = 0;
   out_4227913807304461750[34] = 0;
   out_4227913807304461750[35] = 0;
   out_4227913807304461750[36] = 0;
   out_4227913807304461750[37] = 0;
   out_4227913807304461750[38] = 0;
   out_4227913807304461750[39] = 0;
   out_4227913807304461750[40] = 0;
   out_4227913807304461750[41] = 1;
   out_4227913807304461750[42] = 0;
   out_4227913807304461750[43] = 0;
   out_4227913807304461750[44] = 0;
   out_4227913807304461750[45] = 0;
   out_4227913807304461750[46] = 0;
   out_4227913807304461750[47] = 0;
   out_4227913807304461750[48] = 0;
   out_4227913807304461750[49] = 0;
   out_4227913807304461750[50] = 0;
   out_4227913807304461750[51] = 0;
   out_4227913807304461750[52] = 0;
   out_4227913807304461750[53] = 0;
}
void h_14(double *state, double *unused, double *out_3272782566418947268) {
   out_3272782566418947268[0] = state[6];
   out_3272782566418947268[1] = state[7];
   out_3272782566418947268[2] = state[8];
}
void H_14(double *state, double *unused, double *out_3476946776297310022) {
   out_3476946776297310022[0] = 0;
   out_3476946776297310022[1] = 0;
   out_3476946776297310022[2] = 0;
   out_3476946776297310022[3] = 0;
   out_3476946776297310022[4] = 0;
   out_3476946776297310022[5] = 0;
   out_3476946776297310022[6] = 1;
   out_3476946776297310022[7] = 0;
   out_3476946776297310022[8] = 0;
   out_3476946776297310022[9] = 0;
   out_3476946776297310022[10] = 0;
   out_3476946776297310022[11] = 0;
   out_3476946776297310022[12] = 0;
   out_3476946776297310022[13] = 0;
   out_3476946776297310022[14] = 0;
   out_3476946776297310022[15] = 0;
   out_3476946776297310022[16] = 0;
   out_3476946776297310022[17] = 0;
   out_3476946776297310022[18] = 0;
   out_3476946776297310022[19] = 0;
   out_3476946776297310022[20] = 0;
   out_3476946776297310022[21] = 0;
   out_3476946776297310022[22] = 0;
   out_3476946776297310022[23] = 0;
   out_3476946776297310022[24] = 0;
   out_3476946776297310022[25] = 1;
   out_3476946776297310022[26] = 0;
   out_3476946776297310022[27] = 0;
   out_3476946776297310022[28] = 0;
   out_3476946776297310022[29] = 0;
   out_3476946776297310022[30] = 0;
   out_3476946776297310022[31] = 0;
   out_3476946776297310022[32] = 0;
   out_3476946776297310022[33] = 0;
   out_3476946776297310022[34] = 0;
   out_3476946776297310022[35] = 0;
   out_3476946776297310022[36] = 0;
   out_3476946776297310022[37] = 0;
   out_3476946776297310022[38] = 0;
   out_3476946776297310022[39] = 0;
   out_3476946776297310022[40] = 0;
   out_3476946776297310022[41] = 0;
   out_3476946776297310022[42] = 0;
   out_3476946776297310022[43] = 0;
   out_3476946776297310022[44] = 1;
   out_3476946776297310022[45] = 0;
   out_3476946776297310022[46] = 0;
   out_3476946776297310022[47] = 0;
   out_3476946776297310022[48] = 0;
   out_3476946776297310022[49] = 0;
   out_3476946776297310022[50] = 0;
   out_3476946776297310022[51] = 0;
   out_3476946776297310022[52] = 0;
   out_3476946776297310022[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_4389229945795658064) {
  err_fun(nom_x, delta_x, out_4389229945795658064);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_635646804366371573) {
  inv_err_fun(nom_x, true_x, out_635646804366371573);
}
void pose_H_mod_fun(double *state, double *out_1592105825884835830) {
  H_mod_fun(state, out_1592105825884835830);
}
void pose_f_fun(double *state, double dt, double *out_5328593654681060988) {
  f_fun(state,  dt, out_5328593654681060988);
}
void pose_F_fun(double *state, double dt, double *out_1459903262957102476) {
  F_fun(state,  dt, out_1459903262957102476);
}
void pose_h_4(double *state, double *unused, double *out_1573274557216908386) {
  h_4(state, unused, out_1573274557216908386);
}
void pose_H_4(double *state, double *unused, double *out_7440187632636794551) {
  H_4(state, unused, out_7440187632636794551);
}
void pose_h_10(double *state, double *unused, double *out_469758277648148749) {
  h_10(state, unused, out_469758277648148749);
}
void pose_H_10(double *state, double *unused, double *out_5199566927053226908) {
  H_10(state, unused, out_5199566927053226908);
}
void pose_h_13(double *state, double *unused, double *out_29074149891970277) {
  h_13(state, unused, out_29074149891970277);
}
void pose_H_13(double *state, double *unused, double *out_4227913807304461750) {
  H_13(state, unused, out_4227913807304461750);
}
void pose_h_14(double *state, double *unused, double *out_3272782566418947268) {
  h_14(state, unused, out_3272782566418947268);
}
void pose_H_14(double *state, double *unused, double *out_3476946776297310022) {
  H_14(state, unused, out_3476946776297310022);
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
