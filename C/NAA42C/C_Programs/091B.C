/******************************************************************************
	  Power Method with Aitken's Delta^2 Method - Algorithm 9.1B
*******************************************************************************

To approximate the dominant eigenvalue and associated eigenvector of the n x n
matrix A given a nonzero vector X.  The eigenvalue will also be refined using
Aitken's Interpolation:

INPUT dimension n; matrix A; vector X; tolerance TOL; maximum number of
iterations N.

OUTPUT approximate eigenvalue æ; Aitken's interpolated values of æ;
approximate eigenvector X (with infinite_norm(X) = 1) or a message that the
maximum number of iterations was exceeded.

NOTE: This algorithm was included as a "Homework Helper."  See p. 506,
Exercise Set 9.2, Problem *** 6 ***.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "091b.out";	/* Customized default output file name.     */
int n;				/* Number of equations and unknowns.        */


main()
{
  double **A, *X, *Y, *TEMP, TOL;
  double ERR, mu, xp, yp, infinite_norm(), mu0, mu1, mutilda;
  int i, j, k, N;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Power Method with Aitken's Delta^2 Method - Algorithm 9.1B\n\n");

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
  A    = dmatrix(1,n,1,n);	/* Matrix A                   */
  X    = dvector(1,n);		/* solution vector            */
  Y    = dvector(1,n);		/* Vector Y                   */
  TEMP = dvector(1,n);		/* Temporary matrix for error */

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
  printf2(" æ~ (Aitken's)");
  printf2("\n----------");
  for (i=1;i<=n+2;i++)
    printf2("---------------");
  printf2("\n");

  printf2("% d\t\t\t", 0);	/* Print first row of output table. */
  for (i=1;i<=n;i++)
    printf2("% .9lf\t", X[i]);
  printf2("\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  k   = 1;
  mu0 = 0.0;
  mu1 = 0.0;

  /* STEP #2 */
  xp = infinite_norm(X);

  /* STEP #3 */
  for (i=1;i<=n;i++)
    X[i] /= xp;

  /* STEP #4 */
  while (k <= N) {		/* Do Steps 5-11. */

    /* STEP #5 */
    for (i=1;i<=n;i++) {
      Y[i] = 0.0;
      for (j=1;j<=n;j++)
        Y[i] += A[i][j] * X[j];
    }

    /* STEPS #6 & #7 */		/* Text error: Swap STEP #7 with STEP #6. */
    yp = infinite_norm(Y);
    mu = yp;
    mutilda = mu0 - (pow(mu1 - mu0, 2.0)) / (mu - 2.0*mu1 + mu0);

    /* STEP #8 */
    if (yp == 0.0) {
      printf2("\n\neigenvector is:\nX = [");
      for (i=1;i<=n;i++)
        printf2("\t %.11lf", X[i]);
      printf2(" ]\n\nA has eigenvalue 0, select new vector X and restart.\n");
      /* Free the memory that was dynamically allocated for the arrays. */
      free_dvector(TEMP,1,n);
      free_dvector(Y,1,n);
      free_dvector(X,1,n);
      free_dmatrix(A,1,n,1,n);
      NAA_do_last(outfile);	/* NAA finish-up procedure. */
      exit (-1);		/* STOP */
    }

    /* STEP #9 */
    for (i=1;i<=n;i++)
      TEMP[i] = X[i] - Y[i] / yp;
    ERR = fabs(infinite_norm(TEMP));
    for (i=1;i<=n;i++)
      X[i] = Y[i] / yp;

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #10 */
    printf2("% d\t", k);
    printf2("% .9lf\t", mu);
    for (i=1;i<=n;i++)
      printf2("% .9lf\t", X[i]);
    printf2("% .9lf\t", mutilda);
    printf2("\n");

    if ((ERR < TOL) && (k >= 4)) {
      /* Free the memory that was dynamically allocated for the arrays. */
      free_dvector(TEMP,1,n);
      free_dvector(Y,1,n);
      free_dvector(X,1,n);
      free_dmatrix(A,1,n,1,n);
      NAA_do_last(outfile);	/* NAA finish-up procedure. */
      exit (1);			/* STOP - Procedure completed successfully. */
    }

    /* STEP #11 */
    k++;			/* k = k + 1. */
    mu0 = mu1;
    mu1 = mu;

  }

  /* STEP #12 */		/* Procudure completed unseccessfully. */
  printf2("\nMaximum number of iterations (%d) exceeded.\n", N);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(TEMP,1,n);
  free_dvector(Y,1,n);
  free_dvector(X,1,n);
  free_dmatrix(A,1,n,1,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/****************************************************************************/
/* infinite_norm() - Computes the infinite norm of a vector.                */
/****************************************************************************/
double infinite_norm(X)
double *X;
{
  double max = 0.0;
  int i;

  for (i=1;i<=n;i++)
    if (fabs(X[i]) > fabs(max))
      max = X[i];

  return (max);
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
