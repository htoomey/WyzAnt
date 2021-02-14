/******************************************************************************
	       Steffensen's Method for Systems - Algorithm 10.1A
*******************************************************************************

To approximate the solution of the nonlinear system F(X) = 0 after being put
into the form X = G(X) and given an initial approximation X:

INPUT number n of equations and unknowns; initial approximation
X = (x1,...,xn)t, tolerance TOL; maximum number of iterations N, equations
for X = G(X) in function g():

++++++++++++++++++++++++++++++ RUN-TIME ADVICE ++++++++++++++++++++++++++++++++
WARNING: If you select a "bad" form for X = G(X) you will get all sorts of
math errors, such as DOMAIN ERROR with sqrt() and pow(), stack overflow,
etc.  If this occurs, don't panic, just try another form of G(X).
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

OUTPUT approximate solution X=(x1,...,xn)t or a message that the number of
iterations was exceeded.

NOTE: This algorithm was included as a "Professor's Favorite, Must-Have"
algorithm.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */
#include "gaussj.c"		/* Guass-Jordan method (inverts a matrix)   */

char *outfile = "101a.out";	/* Customized default output file name.     */
int n;				/* Dimension of n x n function matrix.      */


/*****************************************************************************/
/* g(i,X) - Function vector, g(i,X) to evaluate.  Index i selects function   */
/*          number.  Needs updating $.                                       */
/*****************************************************************************/
double g(i, X)
int i;
double *X;
{
  switch (i) {
    case 1: return ((1.0/3.0)*cos(X[2]*X[3]) + (1.0/6.0));
    case 2: return (sqrt(X[1]*X[1] + sin(X[3]) + 1.06) / 9.0 - 0.1);
    case 3: return (-exp(-X[1]*X[2]) / 20.0 - ((10*PI-3) / 60.0));
    default: naaerror("Oops!  You forgot to modify g().");
  }
}
/*****************************************************************************/


main()
{
  double **X, **XTEMP, **Y0, **Y1, **Y2, TOL, g(), infinite_norm();
  double **DELTA_Y0, **DELTA_Y1, **DELTA2_Y0, *DELTA_X0, **TEMP;
  int i, j, k, m, N;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Steffensen's Method for Systems - Algorithm 10.1A\n\n");

  printf("Enter maximum number of iterations, N: ");
  scanf("%d", &N);
  fprintf(file_id, "Maximum number of iterations: %d\n", N);

  printf("Enter Tolerance TOL (1.0e-5)?: ");
  scanf("%lf", &TOL);
  fprintf(file_id, "Tolerance: %lg\n", TOL);

  do {
    printf("Enter number of equations, n: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greter than zero.\n");
  } while (n <= 0);
  fprintf(file_id, "System has %d equations and %d unknowns.\n\n", n, n);

  /* Dynamically allocate memory for the needed arrays. */
  X         = dmatrix(0,n+1,1,n);/* solution vectors ie- X[1][i] = G(X[0][i] */
  Y0        = dmatrix(1,n,1,n);	/* for [X1  X0] */
  Y1        = dmatrix(1,n,1,n);	/* for [X2  X1] */
  Y2        = dmatrix(1,n,1,n);	/* for [X3  X2] */
  DELTA_X0  = dvector(1,n);
  DELTA_Y0  = dmatrix(1,n,1,n);
  DELTA_Y1  = dmatrix(1,n,1,n);
  DELTA2_Y0 = dmatrix(1,n,1,n);
  TEMP      = dmatrix(1,n,1,n);
  XTEMP     = dmatrix(1,n,1,1);

  printf("Enter initial approximations to X:\n");
  for (i=1;i<=n;i++) {
    printf("X0[%d] =  ", i);
    scanf("%lf", &X[0][i]);
  }

  printf2("\n k\t");		/* Print table header. */
  for (i=1;i<=n;i++)
    printf2("  X[%d]\t\t", i);
  printf2("\n---------");
  for (i=1;i<=n;i++)
    printf2("---------------");
  printf2("\n");

  k = 0;
  printf2("% d\t", k);		/* Output initial approximation to X. */
  for (i=1;i<=n;i++)
    printf2("% .9lf\t", X[0][i]);
  printf2("\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  k = 1;

  /* STEP #2 */			/* Heart of algorithm */
  while (k <= N) {		/* Do Steps 3-10      */

    /* STEP #3 */
    for (i=1;i<=n;i++)		/* Load X with first 3 solutions using G(X). */
      X[1][i] = g(i, &X[0][0]);
    for (i=1;i<=n;i++)
      X[2][i] = g(i, &X[1][0]);
    for (i=1;i<=n;i++)
      X[3][i] = g(i, &X[2][0]);

    /* STEP #4 */
    for (i=1;i<=n;i++)		/* Forms Y0, Y1, & Y2 */
      for (j=1;j<=n;j++) {
        Y0[j][i] = X[i-1][j];
        Y1[j][i] = X[i][j];
        Y2[j][i] = X[i+1][j];
      }

    /* STEP #5 */
    for (i=1;i<=n;i++) {	/* Compute Delta values. */
      DELTA_X0[i] = X[1][i] - X[0][i];
      for (j=1;j<=n;j++) {
        DELTA_Y0[i][j] = Y1[i][j] - Y0[i][j];
        DELTA_Y1[i][j] = Y2[i][j] - Y1[i][j];
        DELTA2_Y0[i][j] = DELTA_Y1[i][j] - DELTA_Y0[i][j];
      }
    }

    for (i=1;i<=n;i++)	/* initialize TEMP[][] to the identity matrix. */
      TEMP[i][i] = 1.0;

    gaussj(DELTA2_Y0,n,TEMP,1);	/* Computes inverse of DELTA2_Y0 */
				/* and stores in into DELTA2_Y0. */

    for (i=1;i<=n;i++)		/* Perform n x n matrix multiplication. */
      for (j=1;j<=n;j++) {
        TEMP[i][j] = 0.0;
        for (m=1;m<=n;m++)
          TEMP[i][j] += DELTA_Y0[i][m] * DELTA2_Y0[m][j];
      }
        
    for (i=1;i<=n;i++) {	/* Perform n x 1 matrix multiplication. */
      XTEMP[i][1] = 0.0;
      for (j=1;j<=n;j++)
        XTEMP[i][1] += TEMP[i][j] * DELTA_X0[j];
    }
        
    /* STEPS #6 */
    for (i=1;i<=n;i++)		/* X[1][] acts as ^X[0][] */
      X[1][i] = X[0][i] - XTEMP[i][1];

    /* STEP #7 */
    for (i=1;i<=n;i++)
      X[0][i] = X[1][i];

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #8 */
    printf2("% d\t", k);
    for (i=1;i<=n;i++)
      printf2("% .9lf\t", X[1][i]);
    printf2("\n");

    /* STEP #9 */
    if (infinite_norm(XTEMP) < TOL) {
      /* Free the memory that was dynamically allocated for the arrays. */
      free_dmatrix(XTEMP,1,n,1,1);
      free_dmatrix(TEMP,1,n,1,n);
      free_dmatrix(DELTA2_Y0,1,n,1,n);
      free_dmatrix(DELTA_Y1,1,n,1,n);
      free_dmatrix(DELTA_Y0,1,n,1,n);
      free_dvector(DELTA_X0,1,n);
      free_dmatrix(Y2,1,n,1,n);
      free_dmatrix(Y1,1,n,1,n);
      free_dmatrix(Y0,1,n,1,n);
      free_dmatrix(X,0,n+1,1,n);

      NAA_do_last(outfile);	/* NAA finish-up procedure. */
      exit (1);			/* Procedure completed successfully */
    }

    /* STEP #10 */
    k++;			/* k = k + 1. */

  }

  /* STEP #11 */		/* Procedure completed unsuccessfully */
  printf2("\nMaximum number of iterations (%d) exceeded.\n", N);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dmatrix(XTEMP,1,n,1,1);
  free_dmatrix(TEMP,1,n,1,n);
  free_dmatrix(DELTA2_Y0,1,n,1,n);
  free_dmatrix(DELTA_Y1,1,n,1,n);
  free_dmatrix(DELTA_Y0,1,n,1,n);
  free_dvector(DELTA_X0,1,n);
  free_dmatrix(Y2,1,n,1,n);
  free_dmatrix(Y1,1,n,1,n);
  free_dmatrix(Y0,1,n,1,n);
  free_dmatrix(X,0,n+1,1,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* infinite_norm() - Computes the infinite norm of a vector.                 */
/*****************************************************************************/
double infinite_norm(X)
double **X;
{
  double max = 0.0;
  int i;

  for (i=1;i<=n;i++)
    if (fabs(X[i][1]) > max)
      max = fabs(X[i][1]);

  return (max);
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
