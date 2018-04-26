/* Produced by CVXGEN, 2018-02-12 23:01:27 -0500.  */
/* CVXGEN is Copyright (C) 2006-2017 Jacob Mattingley, jem@cvxgen.com. */
/* The code in this file is Copyright (C) 2006-2017 Jacob Mattingley. */
/* CVXGEN, or solvers produced by CVXGEN, cannot be used for commercial */
/* applications without prior written permission from Jacob Mattingley. */

/* Filename: solver.h. */
/* Description: Header file with relevant definitions. */
#ifndef LKSOLVER_H
#define LKSOLVER_H
/* Uncomment the next line to remove all library dependencies. */
/*#define ZERO_LIBRARY_MODE */
#ifdef MATLAB_MEX_FILE
/* Matlab functions. MATLAB_MEX_FILE will be defined by the mex compiler. */
/* If you are not using the mex compiler, this functionality will not intrude, */
/* as it will be completely disabled at compile-time. */
#include "mex.h"
#else
#ifndef ZERO_LIBRARY_MODE
#include <stdio.h>
#endif
#endif
/* Space must be allocated somewhere (testsolver.c, csolve.c or your own */
/* program) for the global variables vars, params, work and settings. */
/* At the bottom of this file, they are externed. */
#ifndef ZERO_LIBRARY_MODE
#include <math.h>
#define pm(A, m, n) printmatrix(#A, A, m, n, 1)
#endif
typedef struct Params_t {
  double Wy[1];
  double Wv[1];
  double Wp[1];
  double rd_1[1];
  double Wr[1];
  double s_0[1];
  double Ws[1];
  double rd_2[1];
  double rd_3[1];
  double rd_4[1];
  double rd_5[1];
  double rd_6[1];
  double rd_7[1];
  double rd_8[1];
  double rd_9[1];
  double rd_10[1];
  double rd_11[1];
  double rd_12[1];
  double rd_13[1];
  double rd_14[1];
  double rd_15[1];
  double rd_16[1];
  double rd_17[1];
  double rd_18[1];
  double rd_19[1];
  double rd_20[1];
  double A[16];
  double x_0[4];
  double B[4];
  double E[4];
  double rd_0[1];
  double y_max[1];
  double y_min[1];
  double v_max[1];
  double p_max[1];
  double r_max[1];
  double s_max[1];
  double *rd[21];
  double *s[1];
  double *x[1];
} Params;
typedef struct Vars_t {
  double *t_01; /* 1 rows. */
  double *t_02; /* 1 rows. */
  double *t_03; /* 1 rows. */
  double *t_04; /* 1 rows. */
  double *s_1; /* 1 rows. */
  double *t_05; /* 1 rows. */
  double *t_06; /* 1 rows. */
  double *t_07; /* 1 rows. */
  double *t_08; /* 1 rows. */
  double *t_09; /* 1 rows. */
  double *t_10; /* 1 rows. */
  double *t_11; /* 1 rows. */
  double *t_12; /* 1 rows. */
  double *t_13; /* 1 rows. */
  double *t_14; /* 1 rows. */
  double *t_15; /* 1 rows. */
  double *t_16; /* 1 rows. */
  double *t_17; /* 1 rows. */
  double *t_18; /* 1 rows. */
  double *t_19; /* 1 rows. */
  double *t_20; /* 1 rows. */
  double *t_21; /* 1 rows. */
  double *t_22; /* 1 rows. */
  double *t_23; /* 1 rows. */
  double *t_24; /* 1 rows. */
  double *t_25; /* 1 rows. */
  double *t_26; /* 1 rows. */
  double *t_27; /* 1 rows. */
  double *t_28; /* 1 rows. */
  double *t_29; /* 1 rows. */
  double *t_30; /* 1 rows. */
  double *t_31; /* 1 rows. */
  double *t_32; /* 1 rows. */
  double *t_33; /* 1 rows. */
  double *t_34; /* 1 rows. */
  double *t_35; /* 1 rows. */
  double *t_36; /* 1 rows. */
  double *t_37; /* 1 rows. */
  double *t_38; /* 1 rows. */
  double *t_39; /* 1 rows. */
  double *t_40; /* 1 rows. */
  double *t_41; /* 1 rows. */
  double *t_42; /* 1 rows. */
  double *t_43; /* 1 rows. */
  double *t_44; /* 1 rows. */
  double *t_45; /* 1 rows. */
  double *t_46; /* 1 rows. */
  double *t_47; /* 1 rows. */
  double *t_48; /* 1 rows. */
  double *t_49; /* 1 rows. */
  double *t_50; /* 1 rows. */
  double *t_51; /* 1 rows. */
  double *t_52; /* 1 rows. */
  double *t_53; /* 1 rows. */
  double *t_54; /* 1 rows. */
  double *t_55; /* 1 rows. */
  double *t_56; /* 1 rows. */
  double *t_57; /* 1 rows. */
  double *t_58; /* 1 rows. */
  double *t_59; /* 1 rows. */
  double *t_60; /* 1 rows. */
  double *t_61; /* 1 rows. */
  double *t_62; /* 1 rows. */
  double *t_63; /* 1 rows. */
  double *t_64; /* 1 rows. */
  double *t_65; /* 1 rows. */
  double *t_66; /* 1 rows. */
  double *t_67; /* 1 rows. */
  double *t_68; /* 1 rows. */
  double *t_69; /* 1 rows. */
  double *t_70; /* 1 rows. */
  double *t_71; /* 1 rows. */
  double *t_72; /* 1 rows. */
  double *t_73; /* 1 rows. */
  double *t_74; /* 1 rows. */
  double *t_75; /* 1 rows. */
  double *t_76; /* 1 rows. */
  double *t_77; /* 1 rows. */
  double *t_78; /* 1 rows. */
  double *t_79; /* 1 rows. */
  double *t_80; /* 1 rows. */
  double *t_81; /* 1 rows. */
  double *t_82; /* 1 rows. */
  double *t_83; /* 1 rows. */
  double *t_84; /* 1 rows. */
  double *t_85; /* 1 rows. */
  double *t_86; /* 1 rows. */
  double *t_87; /* 1 rows. */
  double *t_88; /* 1 rows. */
  double *t_89; /* 1 rows. */
  double *t_90; /* 1 rows. */
  double *t_91; /* 1 rows. */
  double *t_92; /* 1 rows. */
  double *t_93; /* 1 rows. */
  double *t_94; /* 1 rows. */
  double *t_95; /* 1 rows. */
  double *t_96; /* 1 rows. */
  double *t_97; /* 1 rows. */
  double *t_98; /* 1 rows. */
  double *t_99; /* 1 rows. */
  double *x_1; /* 4 rows. */
  double *x_2; /* 4 rows. */
  double *x_3; /* 4 rows. */
  double *x_4; /* 4 rows. */
  double *x_5; /* 4 rows. */
  double *x_6; /* 4 rows. */
  double *x_7; /* 4 rows. */
  double *x_8; /* 4 rows. */
  double *x_9; /* 4 rows. */
  double *x_10; /* 4 rows. */
  double *x_11; /* 4 rows. */
  double *x_12; /* 4 rows. */
  double *x_13; /* 4 rows. */
  double *x_14; /* 4 rows. */
  double *x_15; /* 4 rows. */
  double *x_16; /* 4 rows. */
  double *x_17; /* 4 rows. */
  double *x_18; /* 4 rows. */
  double *x_19; /* 4 rows. */
  double *x_20; /* 4 rows. */
  double *t_100; /* 1 rows. */
  double *t_101; /* 1 rows. */
  double *t_102; /* 1 rows. */
  double *t_103; /* 1 rows. */
  double *t_104; /* 1 rows. */
  double *t_105; /* 1 rows. */
  double *t_106; /* 1 rows. */
  double *t_107; /* 1 rows. */
  double *t_108; /* 1 rows. */
  double *t_109; /* 1 rows. */
  double *t_110; /* 1 rows. */
  double *t_111; /* 1 rows. */
  double *t_112; /* 1 rows. */
  double *t_113; /* 1 rows. */
  double *t_114; /* 1 rows. */
  double *t_115; /* 1 rows. */
  double *t_116; /* 1 rows. */
  double *t_117; /* 1 rows. */
  double *t_118; /* 1 rows. */
  double *t_119; /* 1 rows. */
  double *t_120; /* 1 rows. */
  double *t_121; /* 1 rows. */
  double *t_122; /* 1 rows. */
  double *t_123; /* 1 rows. */
  double *t_124; /* 1 rows. */
  double *t_125; /* 1 rows. */
  double *t_126; /* 1 rows. */
  double *t_127; /* 1 rows. */
  double *t_128; /* 1 rows. */
  double *t_129; /* 1 rows. */
  double *t_130; /* 1 rows. */
  double *t_131; /* 1 rows. */
  double *t_132; /* 1 rows. */
  double *t_133; /* 1 rows. */
  double *t_134; /* 1 rows. */
  double *t_135; /* 1 rows. */
  double *t_136; /* 1 rows. */
  double *t_137; /* 1 rows. */
  double *t_138; /* 1 rows. */
  double *t_139; /* 1 rows. */
  double *t_140; /* 1 rows. */
  double *t_141; /* 1 rows. */
  double *t_142; /* 1 rows. */
  double *t_143; /* 1 rows. */
  double *t_144; /* 1 rows. */
  double *t_145; /* 1 rows. */
  double *t_146; /* 1 rows. */
  double *t_147; /* 1 rows. */
  double *t_148; /* 1 rows. */
  double *t_149; /* 1 rows. */
  double *t_150; /* 1 rows. */
  double *t_151; /* 1 rows. */
  double *t_152; /* 1 rows. */
  double *t_153; /* 1 rows. */
  double *t_154; /* 1 rows. */
  double *t_155; /* 1 rows. */
  double *t_156; /* 1 rows. */
  double *t_157; /* 1 rows. */
  double *t_158; /* 1 rows. */
  double *t_159; /* 1 rows. */
  double *t_160; /* 1 rows. */
  double *t_161; /* 1 rows. */
  double *s_2; /* 1 rows. */
  double *t_162; /* 1 rows. */
  double *s_3; /* 1 rows. */
  double *t_163; /* 1 rows. */
  double *s_4; /* 1 rows. */
  double *t_164; /* 1 rows. */
  double *s_5; /* 1 rows. */
  double *t_165; /* 1 rows. */
  double *s_6; /* 1 rows. */
  double *t_166; /* 1 rows. */
  double *s_7; /* 1 rows. */
  double *t_167; /* 1 rows. */
  double *s_8; /* 1 rows. */
  double *t_168; /* 1 rows. */
  double *s_9; /* 1 rows. */
  double *t_169; /* 1 rows. */
  double *s_10; /* 1 rows. */
  double *t_170; /* 1 rows. */
  double *s_11; /* 1 rows. */
  double *t_171; /* 1 rows. */
  double *s_12; /* 1 rows. */
  double *t_172; /* 1 rows. */
  double *s_13; /* 1 rows. */
  double *t_173; /* 1 rows. */
  double *s_14; /* 1 rows. */
  double *t_174; /* 1 rows. */
  double *s_15; /* 1 rows. */
  double *t_175; /* 1 rows. */
  double *s_16; /* 1 rows. */
  double *t_176; /* 1 rows. */
  double *s_17; /* 1 rows. */
  double *t_177; /* 1 rows. */
  double *s_18; /* 1 rows. */
  double *t_178; /* 1 rows. */
  double *s_19; /* 1 rows. */
  double *t_179; /* 1 rows. */
  double *s_20; /* 1 rows. */
  double *x[21];
  double *s[21];
} Vars;
typedef struct Workspace_t {
  double h[280];
  double s_inv[280];
  double s_inv_z[280];
  double b[179];
  double q[279];
  double rhs[1018];
  double x[1018];
  double *s;
  double *z;
  double *y;
  double lhs_aff[1018];
  double lhs_cc[1018];
  double buffer[1018];
  double buffer2[1018];
  double KKT[2061];
  double L[2054];
  double d[1018];
  double v[1018];
  double d_inv[1018];
  double gap;
  double optval;
  double ineq_resid_squared;
  double eq_resid_squared;
  double block_33[1];
  /* Pre-op symbols. */
  double quad_998056431616[1];
  int converged;
} Workspace;
typedef struct Settings_t {
  double resid_tol;
  double eps;
  int max_iters;
  int refine_steps;
  int better_start;
  /* Better start obviates the need for s_init and z_init. */
  double s_init;
  double z_init;
  int verbose;
  /* Show extra details of the iterative refinement steps. */
  int verbose_refinement;
  int debug;
  /* For regularization. Minimum value of abs(D_ii) in the kkt D factor. */
  double kkt_reg;
} Settings;
extern Vars vars;
extern Params params;
extern Workspace work;
extern Settings settings;
/* Function definitions in ldl.c: */
void ldl_solve(double *target, double *var);
void ldl_factor(void);
double check_factorization(void);
void matrix_multiply(double *result, double *source);
double check_residual(double *target, double *multiplicand);
void fill_KKT(void);

/* Function definitions in matrix_support.c: */
void multbymA(double *lhs, double *rhs);
void multbymAT(double *lhs, double *rhs);
void multbymG(double *lhs, double *rhs);
void multbymGT(double *lhs, double *rhs);
void multbyP(double *lhs, double *rhs);
void fillq(void);
void fillh(void);
void fillb(void);
void pre_ops(void);

/* Function definitions in solver.c: */
double eval_gap(void);
void set_defaults(void);
void setup_pointers(void);
void setup_indexed_params(void);
void setup_indexed_optvars(void);
void setup_indexing(void);
void set_start(void);
double eval_objv(void);
void fillrhs_aff(void);
void fillrhs_cc(void);
void refine(double *target, double *var);
double calc_ineq_resid_squared(void);
double calc_eq_resid_squared(void);
void better_start(void);
void fillrhs_start(void);
long solve(void);

/* Function definitions in testsolver.c: */
int main(int argc, char **argv);
void load_default_data(void);

/* Function definitions in util.c: */
void tic(void);
float toc(void);
float tocq(void);
void printmatrix(char *name, double *A, int m, int n, int sparse);
double unif(double lower, double upper);
float ran1(long*idum, int reset);
float randn_internal(long *idum, int reset);
double randn(void);
void reset_rand(void);

#endif
