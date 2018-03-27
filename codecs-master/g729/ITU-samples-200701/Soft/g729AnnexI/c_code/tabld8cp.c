/* ITU-T G.729 Software Package Release 2 (November 2006) */
/*
    ITU-T G.729 Annex I  - Reference C code for fixed point
                         implementation of G.729 Annex I
                         Version 1.1 of October 1999
*/
/*
 File : tabld8cp.c
 */

/* from tab_ld8e.c G.729 Annex E Version 1.2  Last modified: May 1998 */
/* from tabld8kd.c G.729 Annex D Version 1.2  Last modified: May 1998 */
/* from tab_ld8k.c G.729 Annex B Version 1.3  Last modified: August 1997 */

#include <stdio.h>
#include <stdlib.h>

#include "typedef.h"
#include "ld8k.h"
#include "ld8cp.h"
#include "tabld8cp.h"

/*
   This file contains the tables used by the coder
*/

/*-----------------------------------------------------*
 | Table of lag_window for autocorrelation.            |
 | noise floor = 1.0001   = (0.9999  on r[1] ..r[10])  |
 | Bandwidth expansion = 60 Hz                         |
 |                                                     |
 | Special double precision format. See "oper_32b.c"   |
 |                                                     |
 | lag_wind[0] =  1.00000000    (not stored)           |
 | lag_wind[1] =  0.99879038                           |
 | lag_wind[2] =  0.99546897                           |
 | lag_wind[3] =  0.98995781                           |
 | lag_wind[4] =  0.98229337                           |
 | lag_wind[5] =  0.97252619                           |
 | lag_wind[6] =  0.96072036                           |
 | lag_wind[7] =  0.94695264                           |
 | lag_wind[8] =  0.93131179                           |
 | lag_wind[9] =  0.91389757                           |
 | lag_wind[10]=  0.89481968                           |
 | lag_wind[11]=  0.87419660                           |
 | lag_wind[12]=  0.85215437                           |
 -----------------------------------------------------*/

Word16 lag_h_B[M+2] = {
    32728,
    32619,
    32438,
    32187,
    31867,
    31480,
    31029,
    30517,
    29946,
    29321,
    28645,
    27923};

Word16 lag_l_B[M+2] = {
    11904,
    17280,
    30720,
    25856,
    24192,
    28992,
    24384,
     7360,
    19520,
    14784,
    22092,
    12924};


Word16 freq_prev_reset[M] = { /* Q13 */
2339, 4679, 7018, 9358, 11698, 14037, 16377, 18717, 21056, 23396
};     /* PI*(float)(j+1)/(float)(M+1) */
/*--------------------------------------------------------------------------*
 * tables specific to G729E                                                 *
 *--------------------------------------------------------------------------*/
Word16 lag_h_bwd[M_BWD] = {
    32764,
    32763,
    32762,
    32760,
    32758,
    32755,
    32752,
    32748,
    32744,
    32739,
    32734,
    32728,
    32722,
    32715,
    32707,
    32700,
    32691,
    32682,
    32673,
    32663,
    32653,
    32642,
    32631,
    32619,
    32607,
    32594,
    32581,
    32567,
    32552,
    32538 };

Word16 lag_l_bwd[M_BWD] = {
    15424,
    23360,
    14784,
    22336,
    13376,
    20672,
    11328,
    18304,
    8704,
    15424,
    5504,
    11904,
    1856,
    8000,
    30464,
    3712,
    25984,
    31872,
    21248,
    27072,
    16384,
    22144,
    11456,
    17280,
    6784,
    12608,
    2240,
    8320,
    30912,
    4480};
Word16 hw[NRP+L_FRAME+M_BWD] = {       /* Buffer for window */
  1565,  3127,  4681,  6225, 7755,
  9266, 10757, 12223, 13661,15068,
 16441, 17776, 19071, 20322,21526,
 22682, 23786, 24835, 25828,26761,
 27634, 28444, 29188, 29866,30476,
 31016, 31486, 31884, 32208,32460,
 32637, 32739, 32767, 32721,32599,
 32403, 32170, 31940, 31711,31484,
 31258, 31034, 30812, 30591,30372,
 30154, 29938, 29723, 29510,29299,
 29089, 28880, 28673, 28468,28264,
 28061, 27860, 27661, 27462,27266,
 27070, 26876, 26684, 26492,26303,
 26114, 25927, 25741, 25557,25373,
 25192, 25011, 24832, 24654,24477,
 24302, 24128, 23955, 23783,23613,
 23443, 23275, 23109, 22943,22779,
 22615, 22453, 22292, 22133,21974,
 21817, 21660, 21505, 21351,21198,
 21046, 20895, 20745, 20597,20449,
 20303, 20157, 20013, 19869,19727,
 19585, 19445, 19306, 19167,19030,
 18894, 18758, 18624, 18490,18358,
 18226, 18096, 17966, 17837,17709,
 17582, 17456, 17331, 17207,17084,
 16961, 16840, 16719, 16599,16480,
 16362, 16245, 16129, 16013,15898,
 15784, 15671, 15559, 15447,15337,
 15227, 15118, 15009, 14902,14795
};
/*-----------------------------------------------------*
 | Tables for routine bits().                          |
 -----------------------------------------------------*/
 Word16 bitsno_B[PRM_SIZE_SID] = {1, /* SID Lsp : MA  */
                     5, /* SID Lsp : 1st stage */
                     4, /* SID Lsp : 2nd stage */
                     5 };  /* SID gain */
Word16 bitsno_D[PRM_SIZE_D] = {1+NC0_B, /* MA + 1st stage */
                                 NC1_B*2, /* 2nd stage */
                                 8,  9,2, 6, /* first subframe  */
                                 4,  9,2, 6}; /* second subframe */


Word16 bitsno_E_fwd[PRM_SIZE_E_fwd] =
    {1+NC0_B,             /* MA + 1st stage */
     NC1_B*2,             /* 2nd stage */
     8,1, 7,7,7,7,7, 7,   /* first subframe  */
     5,   7,7,7,7,7, 7};  /* second subframe */
Word16 bitsno_E_bwd[PRM_SIZE_E_bwd] =
    {
     8,1, 13,10,7,7,7, 7,   /* first subframe  */
     5, 13,10,7,7,7, 7};  /* second subframe */

Word16 bitrates[2] = {80, 118};
Word16 tab_log[17] = { /* Table for log calculation */
    2466, 2520, 2571, 2619, 2665, 2708, 2749, 2789,
    2827, 2863, 2898, 2931, 2964, 2995, 3025, 3054, 3083};
Word16 ipos[16]={0,1,2,3,4,0,1,2,3,4,0,1,2,3,4,0};

Word16 lspSid_reset[M] = {
    31441,  27566,  21458,  13612,   4663,
    -4663, -13612, -21458, -27566, -31441};

/*-----------------------------------------------------*
 | Tables for gain related routines . (6.4kbps)        |
 -----------------------------------------------------*/
 Word16 gbk1_6k[NCODE1_6K][2] = {
 /*  Q14    Q14 */
  { 5849,     0 },
  { 2929,  1078 },
  { 9425,  2731 },
  { 6068,  6093 },
  { 3617,  6747 },
  { 3171,  9280 },
  { 3915, 12872 },
  { 4987, 22294 }
 };

Word16 gbk2_6k[NCODE2_6K][2] = {
 /*  Q14    Q14 */
  {    0,  4175 },
  { 3988,     0 },
  { 4478,  7324 },
  { 7876,  7821 },
  {10291, 11385 },
  {10828, 27602 },
  {11956, 10735 },
  {16423, 15724 }
 };

/* {36.632507,  2.514171},  {0.399259,  0.073709} */
/*  [0][0]      [0][1]       [1][0]     [1][1]    */
/*  Q9          Q13          Q16        Q18       */
Word16 coef_6k[2][2] = {
   { 18756, 20596},
   { 26166, 19322}
};
/*  [0][0]      [0][1]       [1][0]     [1][1]    */
/*  Q25         Q29          Q32        Q34       */
Word32 L_coef_6k[2][2] = {
   { 1229182965L, 1349785278L },
   { 1714804347L, 1266310978L }
};

Word16 thr1_6k[NCODE1_6K-NCAN1_6K] = { /* Q13 */
   9919,  /* 1.210869 */
  19675   /* 2.401702 */
};

Word16 thr2_6k[NCODE2_6K-NCAN2_6K] = { /* Q15 */
  17233,  /* 0.525915 */
  25144   /* 0.767320 */
};

Word16 map1_6k[NCODE1_6K] = { 0, 4, 6, 5, 2, 1, 7, 3 };

Word16 imap1_6k[NCODE1_6K] = { 0, 5, 4, 7, 1, 3, 2, 6 };

Word16 map2_6k[NCODE2_6K] = { 0, 4, 3, 7, 5, 1, 6, 2 };

Word16 imap2_6k[NCODE2_6K] = { 0, 5, 7, 2, 1, 4, 6, 3 };


/*-----------------------------------------------------*
 | Tables for algebraic codebook routines. (6.4kbps)   |
 -----------------------------------------------------*/
Word16 trackTable0[] = {
 1, 3, 6, 8, 11, 13, 16, 18, 21, 23, 26, 28, 31, 33, 36, 38};

Word16 trackTable1[] = {
0, 2, 4, 5, 7, 9, 10, 12, 14, 15, 17, 19, 20, 22, 24, 25,
27, 29, 30, 32, 34, 35, 37, 39, 1, 6, 11, 16, 21, 26, 31, 36};

Word16 grayEncode[] = {
0, 1, 3, 2, 6, 7, 5, 4, 12, 13, 15, 14, 10, 11, 9, 8,
24, 25, 27, 26, 30, 31, 29, 28, 20, 21, 23, 22, 18, 19, 17, 16};

Word16 grayDecode[] = {
0, 1, 3, 2, 7, 6, 4, 5, 15, 14, 12, 13, 8, 9, 11, 10,
31, 30, 28, 29, 24, 25, 27, 26, 16, 17, 19, 18, 23, 22, 20, 21};

/*-----------------------------------------------------*
 | Tables for the adaptive phase dispersion. (6.4kbps) |
 -----------------------------------------------------*/
/* post-processing in Q15 */
Word16 ph_imp_low[] = {
14690, 11518, 1268, -2762, -5672, 7514, -36, -2808, -3041, 4823,
2952, -8425, 3785, 1455, 2179, -8638, 8051, -2104, -1455, 777,
1108, -2386, 2254, -364, -675, -2104, 6046, -5682, 1072, 3123,
-5059, 5312, -2330, -3729, 6924, -3890, 675, -1776, 29, 10145};
   
Word16 ph_imp_mid[] = {
30274, 3831, -4037, 2972, -1049, -1003, 2477, -3044, 2815, -2232,
1753, -1612, 1714, -1776, 1543, -1009, 429, -170, 472, -1265,
2176, -2707, 2523, -1622, 344, 826, -1530, 1724, -1658, 1701,
-2064, 2644, -3061, 2897, -1979, 557, 780, -1370, 842, 655};
 
