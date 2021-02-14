/******************************************************************************
		  Symmetric Inverse Power Method - Algorithm 9.3B
*******************************************************************************

To approximate the dominant eigenvalue and an associated eigenvector of the
n x n symmetric matrix A, given a nonzero vector X:

INPUT dimension n; matrix A; vector X; tolerance TOL; maximum number of
iterations N.

OUTPUT approximate eigenvalue æ; approximate eigenvector X (with
euclidean_norm(X) = 1) or a message that the maximum number of iterations was
exceeded.

NOTE: This algorithm was included as a "Homework Helper."  See p. 506,
Exercise Set 9.2, Problem 4.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 1Q 1992, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */
#include "gaussj.c"		/* Gauss-Jordan Method (for inverses)       */

char *outfile = "093b.out";	/* Customized default output file name.     */
int n;				/* Number of equations and unknowns.        */


main()
{
  double **A, **ATEMP, **XTEMP, *X, *Y, *TEMP;
  double ERR, mu, norm, a, b, q, TOL, euclidean_norm();
  int i, j, k, N;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Symmetric Inverse Power Method - Algorithm 9.3B\n\n");

  printf("Enter maximum number of iterations, N: ");
  scanf("%d", &N);
  fprintf(file_id, "Maximum number of iterations: %d\n", N);

  printf("Enter the tolerance, TOL (1.0e-4): ");
  scanf("%lf", &TOL);
  fprintf(file_id, "Tolerance = %lg\n", TOL);

  do {
    printf("Enter the dimension, n, of the matrix A: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greter than zero.\n");
  } while (n <= 0);
  fprintf(file_id, "System has %d equations and %d unknowns.\n\n", n, n);

  /* Dynamically allocate memory for the needed arrays. */
  A     = dmatrix(1,n,1,n);	/* Matrix A                   */
  ATEMP = dmatrix(1,n,1,n);	/* Temporary matrix A         */
  XTEMP = dmatrix(1,n,1,1);	/* Temporary matrix X         */
  X     = dvector(1,n);		/* solution vector            */
  Y     = dvector(1,n);		/* Matrix Y                   */
  TEMP  = dvector(1,n);		/* Temporary matrix for error */

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

  printf("Enter initial Eigenvector, X:\n");	/* Get X. */
  for (i=1;i<=n;i++) {
    printf("\tX[%d] = ", i);
    scanf("%lf", &X[i]);
  }

  fprintf(file_id, "X = [  ");	/* Print Vector X to file. */
  for (i=1;i<=n;i++)
    fprintf(file_id, "% 3lg  ", X[i]);
  fprintf(file_id, "]t\n\n");

  printf2("\n k\t æ = mu\t\t");	/* Print table header. */
  for (i=1;i<=n;i++)
    printf2(" X[%d]\t\t", i);
  printf2("\n----------");
  for (i=1;i<=n+1;i++)
    printf2("---------------");
  printf2("\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  for (i=1;i<=n;i++) {
    Y[i] = 0.0;
    for (j=1;j<=n;j++)
      Y[i] += A[i][j] * X[j];
  }
  a = 0.0;
  b = 0.0;
  for (i=1;i<=n;i++) {
    a += X[i] * Y[i];
    b += X[i] * X[i];
  }
  q = a/b;

  /* STEP #2 */
  k = 1;

  /* STEP #3 */
  /* CHANGE #1 - Use the Euclidean Norm */
  norm = euclidean_norm(X);

  /* STEP #4 */
  for (i=1;i<=n;i++)
    X[i] /= norm;

  /* STEP #5 */
  while (k <= N) {

    /* STEP #6 */
    for (i=1;i<=n;i++) {	/* Sets up (A - qI). */
      XTEMP[i][1] = X[i];
      for (j=1;j<=n;j++) {
        ATEMP[i][j] = A[i][j];
        if (i==j)
          ATEMP[i][i] -= q;
      }
    }

    gaussj(ATEMP,n,XTEMP,1);	/* Solves the linear system (A - qI)Y = X. */

    /*
    **  If the system does not have a unique solution, then
    **  printf2("%.11lg is an eigenvalue.\n", q);
    **  STOP.
    **  Note: Never really gets to this step.  Will exit with an error
    **        message first.
    */

    for (i=1;i<=n;i++)
      Y[i] = XTEMP[i][1];

    /* STEPS #7 */
    /* CHANGE #2 - Use  mu = Xt*Y  to exploit the symmetries. */
    mu = 0.0;
    for (i=1;i<=n;i++)
      mu += X[i] * Y[i];

    /* STEPS #8 */
    /* CHANGE #3 - Use the Euclidean Norm */
    norm = euclidean_norm(Y);

    /* STEP #9 */
    /* CHANGE #4 - Use the Euclidean Norm */
    for (i=1;i<=n;i++)
      TEMP[i] = X[i] - Y[i]/norm;
    ERR = fabs(euclidean_norm(TEMP));
    for (i=1;i<=n;i++)
      X[i] = Y[i] / norm;

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #10 */
    mu = (1.0/mu) + q;
    printf2("% d\t% .9lf", k, mu);
    for (i=1;i<=n;i++)
      printf2("\t% .9lf", X[i]);
    printf2("\n");

    if (ERR < TOL) {
      /* Free the memory that was dynamically allocated for the arrays. */
      free_dvector(TEMP,1,n);
      free_dvector(Y,1,n);
      free_dvector(X,1,n);
      free_dmatrix(XTEMP,1,n,1,1);
      free_dmatrix(ATEMP,1,n,1,n);
      free_dmatrix(A,1,n,1,n);
      NAA_do_last(outfile);	/* NAA finish-up procedure. */
      exit (1);			/* STOP - Procedure completed successfully. */
    }

    /* STEP #11 */
    k++;			/* k = k + 1. */

  }

  /* STEP #12 */		/* Procudure completed unseccessfully. */
  printf2("\nMaximum number of iterations (%d) exceeded.\n", N);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(TEMP,1,n);
  free_dvector(Y,1,n);
  free_dvector(X,1,n);
  free_dmatrix(XTEMP,1,n,1,1);
  free_dmatrix(ATEMP,1,n,1,n);
  free_dmatrix(A,1,n,1,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/****************************************************************************/
/* euclidean_norm() - Computes the euclidean norm of a vector.              */
/****************************************************************************/
double euclidean_norm(X)
double *X;
{
  double sum_of_sqrs = 0.0;
  int i;

  for (i=1;i<=n;i++)
    sum_of_sqrs += pow(X[i], 2.0);

  return (sqrt(sum_of_sqrs));
}

/*****************************************************************************/
/*	Copyright (C) 1988-1992, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
