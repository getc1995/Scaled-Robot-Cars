/* Produced by CVXGEN, 2018-02-12 21:04:55 -0500.  */
/* CVXGEN is Copyright (C) 2006-2017 Jacob Mattingley, jem@cvxgen.com. */
/* The code in this file is Copyright (C) 2006-2017 Jacob Mattingley. */
/* CVXGEN, or solvers produced by CVXGEN, cannot be used for commercial */
/* applications without prior written permission from Jacob Mattingley. */

/* Filename: solver.h. */
/* Description: Header file with relevant definitions. */
#ifndef ACCSOLVER_H
#define ACCSOLVER_H
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
  double Q[4];
  double u_0[1];
  double R[1];
  double A[4];
  double x_0[2];
  double B[2];
  double E[2];
  double vl[1];
  double F[2];
  double h_stop[1];
  double T_gap[1];
  double v_max[1];
  double v_min[1];
  double h_min[1];
  double u_max[1];
  double u_min[1];
  double *u[1];
  double *x[1];
} Params;
typedef struct Vars_t {
  double *t_01; /* 2 rows. */
  double *t_02; /* 2 rows. */
  double *t_03; /* 2 rows. */
  double *t_04; /* 2 rows. */
  double *t_05; /* 2 rows. */
  double *t_06; /* 2 rows. */
  double *t_07; /* 2 rows. */
  double *t_08; /* 2 rows. */
  double *t_09; /* 2 rows. */
  double *t_10; /* 2 rows. */
  double *t_11; /* 2 rows. */
  double *t_12; /* 2 rows. */
  double *t_13; /* 2 rows. */
  double *t_14; /* 2 rows. */
  double *t_15; /* 2 rows. */
  double *t_16; /* 2 rows. */
  double *t_17; /* 2 rows. */
  double *t_18; /* 2 rows. */
  double *t_19; /* 2 rows. */
  double *t_20; /* 2 rows. */
  double *u_1; /* 1 rows. */
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
  double *x_1; /* 2 rows. */
  double *x_2; /* 2 rows. */
  double *x_3; /* 2 rows. */
  double *x_4; /* 2 rows. */
  double *x_5; /* 2 rows. */
  double *x_6; /* 2 rows. */
  double *x_7; /* 2 rows. */
  double *x_8; /* 2 rows. */
  double *x_9; /* 2 rows. */
  double *x_10; /* 2 rows. */
  double *x_11; /* 2 rows. */
  double *x_12; /* 2 rows. */
  double *x_13; /* 2 rows. */
  double *x_14; /* 2 rows. */
  double *x_15; /* 2 rows. */
  double *x_16; /* 2 rows. */
  double *x_17; /* 2 rows. */
  double *x_18; /* 2 rows. */
  double *x_19; /* 2 rows. */
  double *x_20; /* 2 rows. */
  double *u_2; /* 1 rows. */
  double *u_3; /* 1 rows. */
  double *u_4; /* 1 rows. */
  double *u_5; /* 1 rows. */
  double *u_6; /* 1 rows. */
  double *u_7; /* 1 rows. */
  double *u_8; /* 1 rows. */
  double *u_9; /* 1 rows. */
  double *u_10; /* 1 rows. */
  double *u_11; /* 1 rows. */
  double *u_12; /* 1 rows. */
  double *u_13; /* 1 rows. */
  double *u_14; /* 1 rows. */
  double *u_15; /* 1 rows. */
  double *u_16; /* 1 rows. */
  double *u_17; /* 1 rows. */
  double *u_18; /* 1 rows. */
  double *u_19; /* 1 rows. */
  double *u_20; /* 1 rows. */
  double *xr_1; /* 2 rows. */
  double *xr_2; /* 2 rows. */
  double *xr_3; /* 2 rows. */
  double *xr_4; /* 2 rows. */
  double *xr_5; /* 2 rows. */
  double *xr_6; /* 2 rows. */
  double *xr_7; /* 2 rows. */
  double *xr_8; /* 2 rows. */
  double *xr_9; /* 2 rows. */
  double *xr_10; /* 2 rows. */
  double *xr_11; /* 2 rows. */
  double *xr_12; /* 2 rows. */
  double *xr_13; /* 2 rows. */
  double *xr_14; /* 2 rows. */
  double *xr_15; /* 2 rows. */
  double *xr_16; /* 2 rows. */
  double *xr_17; /* 2 rows. */
  double *xr_18; /* 2 rows. */
  double *xr_19; /* 2 rows. */
  double *xr_20; /* 2 rows. */
  double *x[21];
  double *xr[21];
  double *u[21];
} Vars;
typedef struct Workspace_t {
  double h[100];
  double s_inv[100];
  double s_inv_z[100];
  double b[139];
  double q[159];
  double rhs[498];
  double x[498];
  double *s;
  double *z;
  double *y;
  double lhs_aff[498];
  double lhs_cc[498];
  double buffer[498];
  double buffer2[498];
  double KKT[873];
  double L[866];
  double d[498];
  double v[498];
  double d_inv[498];
  double gap;
  double optval;
  double ineq_resid_squared;
  double eq_resid_squared;
  double block_33[1];
  /* Pre-op symbols. */
  double quad_397717204992[1];
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
