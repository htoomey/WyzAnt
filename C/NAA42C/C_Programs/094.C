/******************************************************************************
		  Weilandt's Deflation - Algorithm 9.4
*******************************************************************************

To approximate the second most dominant eigenvalue and an associated
eigenvector of the n x n matrix A given an approximation lambda to the
dominant eigenvalue, an approximation V to a corresponding eigenvector and a
vector X î R(n-1):

INPUT dimension n; matrix A; approximate eigenvalue lambda; approximate
eigenvector V î R(n); approximate second eigenvector X î R(n-1).

OUTPUT approximate eigenvalue æ; approximate eigenvector U or a message that
the method fails.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "094.out";	/* Customized default output file name.     */
int n;				/* Number of equations and unknowns.        */


main()
{
  double **A, **B, *U, *V, *W, *X, *WPRIME;
  double lambda, mu, sum, power();
  int i, j, k, int_of_max();

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Weilandt's Deflation - Algorithm 9.4\n\n");

  do {
    printf("Enter the dimension, n, of the matrix A: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greter than zero.\n");
  } while (n <= 0);
  fprintf(file_id, "System has %d equations and %d unknowns.\n\n", n, n);

  /* Dynamically allocate memory for the needed arrays. */
  A      = dmatrix(1,n,1,n);	/* Matrix A (n x n)               */
  B      = dmatrix(1,n-1,1,n-1);/* Matrix B (n-1 x n-1)           */
  U      = dvector(1,n);	/* Second approximate eigenvector */
  V      = dvector(1,n);	/* First approximate eigenvector  */
  W      = dvector(1,n);	/* Vector W (n x 1)               */
  X      = dvector(1,n-1);	/* Vector X (n-1 x 1)             */
  WPRIME = dvector(1,n-1);	/* Vector WPRIME (n-1 x 1)        */

  printf("Enter the coefficients for matrix A:\n");	/* Get A. */
  for (i=1;i<=n;i++)
    for (j=1;j<=n;j++) {
      printf("\tA[%d][%d] = ", i, j);
      scanf("%lf", &A[i][j]);
    }

  fprintf(file_id, "A = ");	/* Print Matrix A to file. */
  for (i=1;i<=n;i++) {
    fprintf(file_id, "[ ");
    for (j=1;j<=n;j++)
      fprintf(file_id, "% 3lg  ", A[i][j]);
    fprintf(file_id, "]\n    ");
  }
  fprintf(file_id, "\n");

  printf("Enter approximate first Eigenvalue, lambda: ");
  scanf("%lf", &lambda);
  fprintf(file_id,
    "Approximate first Eigenvalue, lambda = %.11lg\n\n", lambda);

  printf("Enter first %d x 1 approximated Eigenvector, V:\n", n);
  for (i=1;i<=n;i++) {				/* Get V. */
    printf("\tV[%d] = ", i);
    scanf("%lf", &V[i]);
  }

  fprintf(file_id, "Approximate first Eigenvector, V:\n");
  fprintf(file_id, "V = [ ");			/* Print V. */
  for (i=1;i<=n;i++)
    fprintf(file_id, "% 3lg ", V[i]);
  fprintf(file_id, "]t\n\n");

  printf("Enter second %d x 1 approximate Eigenvector, X:\n", n-1);
  for (i=1;i<n;i++) {				/* Get X. */
    printf("\tX[%d] = ", i);
    scanf("%lf", &X[i]);
  }

  fprintf(file_id, "Second approximate Eigenvector R(n-1), X:\n");
  fprintf(file_id, "X = [ ");	 		/* Print X. */
  for (i=1;i<n;i++)
    fprintf(file_id, "% 3lg ", X[i]);
  fprintf(file_id, "]t\n\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  i = int_of_max(V);

  /* STEP #2 */
  if (i != 1)
    for (k=1;k<i;k++)
      for (j=1;j<i;j++)
        B[k][j] = A[k][j] - V[k] * A[i][j] / V[i];

  /* STEP #3 */
  if ((i != 1) && (i != n))
    for (k=i;k<n;k++)
      for (j=1;j<i;j++) {
        B[k][j] = A[k+1][j] - V[k+1] * A[i][j] / V[i];
        B[j][k] = A[j][k+1] - V[j] * A[i][k+1] / V[i];
      }

  /* STEP #4 */
  if (i != n)
    for (k=i;k<n;k++)
      for (j=i;j<n;j++)
        B[k][j] = A[k+1][j+1] - V[k+1] * A[i][j+1] / V[i];

  /* STEP #5 */

  /*
  **  Perform the power method on the (n-1) x (n-1) matrix B' = (b(k,j))
  **  with X as initial approximation.
  */

  for (j=1;j<n;j++)		/* Copy vector X into WPRIME. */
    WPRIME[j] = X[j];
  mu = power(B, WPRIME, n-1);
  if (mu == 0.0) {
    printf2("Method fails.\n");
    /* Free the memory that was dynamically allocated for the arrays. */
    free_dvector(WPRIME,1,n-1);
    free_dvector(X,1,n-1);
    free_dvector(W,1,n);
    free_dvector(V,1,n);
    free_dvector(U,1,n);
    free_dmatrix(B,1,n-1,1,n-1);
    free_dmatrix(A,1,n,1,n);
    NAA_do_last(outfile);	/* NAA finish-up procedure. */
    exit (-1);
  }

  /* STEP #6 */
  if (i != 1)
    for (k=1;k<i;k++)
      W[k] = WPRIME[k];

  /* STEP #7 */
  W[i] = 0.0;

  /* STEP #8 */
  if (i != n)
    for (k=i+1;k<=n;k++)
      W[k] = WPRIME[k-1];

  /* STEP #9 */
  sum = 0.0;
  for (j=1;j<=n;j++)
    sum += A[i][j]*W[j];
  for (k=1;k<=n;k++)
    U[k] = (mu - lambda) * W[k] + sum * V[k] / V[i];
  /* Compute eigenvector using Eq. (9.21). */

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #10 */
  printf2("Approximate second Eigenvalue, mu = %.11lg\n\n", mu);
  printf2("Approximate second Eigenvector, U:\n");
  printf2("U = [ ");
  for (i=1;i<=n;i++)		/* Procedure completed successfully. */
    printf2("% 3lg ", U[i]);
  printf2("]t\n");

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(WPRIME,1,n-1);
  free_dvector(X,1,n-1);
  free_dvector(W,1,n);
  free_dvector(V,1,n);
  free_dvector(U,1,n);
  free_dmatrix(B,1,n-1,1,n-1);
  free_dmatrix(A,1,n,1,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* int_of_max() - Returns the index of the largest magnitude element of the  */
/*                vector X.                                                  */
/*****************************************************************************/
int int_of_max(X)
double *X;
{
  double max=0.0;
  int j, digit=0;

  for (j=n;j>=1;j--)
    if (fabs(X[j]) >= fabs(max)) {
      max = X[j];
      digit = j;
    }

  return (digit);
}

/*****************************************************************************/
/* power() - Power method - Algorithm 9.1   Solves for the dominant          */
/*           eigenvalue and its associated eigenvector.                      */
/*****************************************************************************/
double power(A, X, nn)
double **A;			/* nn x nn matrix A      */
double *X;			/* Initial eigenvector X */
int nn;
{
  double *Y, *TEMP;
  double ERR, xp, yp, mu;
  double infinite_norm(), TOL = 1.0e-4;
  int N = 25;			/* Maximum number of iterations */
  int i, j, k;

  /* Dynamically allocate memory for the needed arrays. */
  Y    = dvector(1,nn);		/* Matrix Y                   */
  TEMP = dvector(1,nn);		/* Temporary matrix for error */

  /* STEP #1 */
  k = 1;

  /* STEP #2 */
  xp = infinite_norm(X);

  /* STEP #3 */
  for (i=1;i<=nn;i++)
    X[i] /= xp;

  /* STEP #4 */
  while (k <= N) {		/* Do Steps 5-11. */

    /* STEP #5 */
    for (i=1;i<=nn;i++) {
      Y[i] = 0.0;
      for (j=1;j<=nn;j++)
        Y[i] += A[i][j] * X[j];
    }

    /* STEPS #6 & #7 */		/* Text error: Swap STEP #7 with STEP #6. */
    yp = infinite_norm(Y,nn);
    mu = yp;

    /* STEP #8 */
    if (yp == 0.0)
      return (0.0);

    /* STEP #9 */
    for (i=1;i<=nn;i++)
      TEMP[i] = X[i] - Y[i] / yp;
    ERR = fabs(infinite_norm(TEMP,nn));
    for (i=1;i<=nn;i++)
      X[i] = Y[i] / yp;

    /* STEP #10 */
    if (ERR < TOL)
      return (mu);		/* STOP - Procedure completed successfully. */

    /* STEP #11 */
    k++;			/* k = k + 1. */

  }

  /* STEP #12 */		/* Procudure completed unseccessfully. */
  printf2("\nMaximum number of iterations (%d) exceeded in power().\n", N);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(Y,1,nn);
  free_dvector(TEMP,1,nn);

}				/* STOP */

/****************************************************************************/
/* infinite_norm() - Computes the infinite norm of a vector.                */
/****************************************************************************/
double infinite_norm(X,nn)
double *X;
int nn;
{
  double max = 0.0;
  int i;

  for (i=1;i<=nn-1;i++)
    if (fabs(X[i]) > fabs(max))
      max = X[i];

  return (max);
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
