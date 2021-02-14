/******************************************************************************
		Newton's Method for Systems - Algorithm 10.1		
*******************************************************************************

To approximate the solution of the nonlinear system F(X)=0 given an initial
approximation X:

INPUT number n of equations and unknowns; initial approximation
X = (x1,...,xn)t, tolerance TOL; maximum number of iterations N; equations
for F(X) = 0 in function f() and for J(X) in jacobian():

OUTPUT approximate solution X = (x1,...,xn)t or a message that the number
of iterations was exceeded.

NOTE: Listed as Newton's for Systems in the 3rd edition of the text.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */
#include "gaussj.c"		/* Guass-Jordan method (inverts a matrix)   */

char *outfile = "101.out";	/* Customized default output file name.     */
int n;				/* dimension of n x n matrix                */


/*****************************************************************************/
/* f(i,X) - Function vector, f(i,X) to evaluate.  Index i selects function   */
/*          number.  Needs updating $.                                       */
/*****************************************************************************/
double f(i, X)
int i;
double *X;
{
  switch (i) {
    case 1: return (3.0*X[1] - cos(X[2]*X[3]) - 0.5);
    case 2: return (X[1]*X[1] - 81.0*(X[2]+0.1)*(X[2]+0.1) + sin(X[3]) + 1.06);
    case 3: return (exp(-X[1]*X[2]) + 20.0*X[3] + (10.0*PI - 3.0)/3.0);
    default: naaerror("Oops!  You forgot to modify f().");
  }
}

/*****************************************************************************/
/* jacobian(i,j,X) - Jacobian matrix, jacobian(i,j,X) to evaluate.  Index i  */
/*                   and j selects function number.  Needs updating $.       */
/*****************************************************************************/
double jacobian(i, j, X)
int i, j;
double *X;
{
  switch (j) {
    case 1:
      switch (i) {
        case 1: return (3.0);
        case 2: return (2.0*X[1]);
        case 3: return (-X[2]*exp(-X[1]*X[2]));
        default: naaerror("Oops!  You forgot to modify jacobian().");
      }

    case 2:
      switch (i) {
        case 1: return (X[3]*sin(X[2]*X[3]));
        case 2: return (-162.0*(X[2]+0.1));
        case 3: return (-X[1]*exp(-X[1]*X[2]));
      }

    case 3:
      switch (i) {
        case 1: return (X[2]*sin(X[2]*X[3]));
        case 2: return (cos(X[3]));
        case 3: return (20.0);
      }
  }
}
/*****************************************************************************/


main()
{
  double **J, **JINV, **F, **Y, *X, TOL;
  double f(), jacobian(), infinite_norm();
  int i, j, k, N;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Newton's Method for Systems - Algorithm 10.1\n\n");

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
  J    = dmatrix(1,n,1,n);	/* Jacobian Matrix     */
  JINV = dmatrix(1,n,1,n);	/* Inverse of J        */
  F    = dmatrix(1,n,1,1);	/* Function Matrix     */
  Y    = dmatrix(1,n,1,1);	/* for -Jinv[][]*F[][] */
  X    = dvector(1,n);		/* solution vector     */

  printf("Enter initial approximations to X:\n");	/* Get X. */
  for (i=1;i<=n;i++) {
    printf("X[%d] =  ", i);
    scanf("%lf", &X[i]);
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
    printf2("% .9lf\t", X[i]);
  printf2("\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  k = 1;

  /* STEP #2 */
  while (k <= N) {		/* Do Steps 3-7. */

    /* STEP #3 */
    for (i=1;i<=n;i++) {	/* Calculate F(X) and J(X), where J(X)ij = */
      F[i][1] = f(i, X);	/* (ëfi(X)/ëxj) for 1 ó i,j ó n.           */
      for (j=1;j<=n;j++)
        J[i][j] = jacobian(i, j, X);
    }

    /* STEP #4 */
    /* Solve the n x n linear system J(X)*Y = -F(X). */
    for (i=1;i<=n;i++) {
      Y[i][1] = -F[i][1];	/* Y = -F                   */
      for (j=1;j<=n;j++)
        JINV[i][j] = J[i][j];	/* Copy J into JINV before passing to gaussj */
    }

    gaussj(JINV,n,Y,1);		/* Solves for Y of J*Y = -F. */

    /* STEP #5 */
    for (i=1;i<=n;i++)		/* set X = X + Y.            */
      X[i] += Y[i][1];

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #6 */
    printf2("% d\t", k);	/* print out solution to kth iteration. */
    for (i=1;i<=n;i++)
      printf2("% .9lf\t", X[i]);
    printf2("\n");
    if (infinite_norm(Y) < TOL) { /* Check for requested accuracy.      */
      /* Free the memory that was dynamically allocated for the arrays. */
      free_dvector(X,1,n);
      free_dmatrix(Y,1,n,1,1);
      free_dmatrix(F,1,n,1,1);
      free_dmatrix(JINV,1,n,1,n);
      free_dmatrix(J,1,n,1,n);

      NAA_do_last(outfile);	/* NAA finish-up procedure.          */
      exit (1);			/* Procedure completed successfully. */
    }

    /* STEP #7 */
    k++;			/* k = k + 1. */

  }

  /* STEP #8 */			/* Procedure completed unsuccessfully */
  printf2("\nMaximum number of iterations (%d) exceeded.\n", N);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(X,1,n);
  free_dmatrix(Y,1,n,1,1);
  free_dmatrix(F,1,n,1,1);
  free_dmatrix(JINV,1,n,1,n);
  free_dmatrix(J,1,n,1,n);

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
