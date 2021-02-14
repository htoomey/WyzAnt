/******************************************************************************
		   Symmetric Power Method - Algorithm 9.2
*******************************************************************************

To approximate the dominant eigenvalue and associated eigenvector of the n x n
symmetric matrix A given a nonzero vector X:

INPUT dimension n; matrix A; vector X; tolerance TOL; maximum number of
iterations N.

OUTPUT approximate eigenvalue æ; approximate eigenvector X (with
euclidean_norm(X) = 1) or a message that the maximum number of iterations was
exceeded.

NOTE: Listed as Power Method for Symmetric Matrices in the 3rd edition of the
text.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "092.out";	/* Customized default output file name.     */
int n;				/* Number of equations and unknowns.        */


main()
{
  double **A, *X, *Y, *TEMP, ERR, mu, norm, TOL, euclidean_norm();
  int i, j, k, N;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Symmetric Power Method - Algorithm 9.2\n\n");

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
  Y    = dvector(1,n);		/* Matrix Y                   */
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
  printf2("\n----------");
  for (i=1;i<=n+1;i++)
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
  k    = 1;
  norm = euclidean_norm(X);
  for (i=1;i<=n;i++)
    X[i] /= norm;

  /* STEP #2 */
  while (k <= N) {		/* Do Steps 3-8. */

    /* STEP #3 */
    for (i=1;i<=n;i++) {
      Y[i] = 0.0;
      for (j=1;j<=n;j++)
        Y[i] += A[i][j] * X[j];
    }

    /* STEP #4 */
    mu = 0.0;
    for (i=1;i<=n;i++)
      mu += X[i] * Y[i];

    /* STEP #5 */
    norm = euclidean_norm(Y);
    if (norm == 0.0) {
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

    /* STEP #6 */
    for (i=1;i<=n;i++)
      TEMP[i] = X[i] - Y[i] / norm;
    ERR = euclidean_norm(TEMP);
    for (i=1;i<=n;i++)
      X[i] = Y[i] / norm;

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #7 */
    printf2("% d\t", k);
    printf2("% .9lf\t", mu);
    for (i=1;i<=n;i++)
      printf2("% .9lf\t", X[i]);
    printf2("\n");

    if (ERR < TOL) {
      /* Free the memory that was dynamically allocated for the arrays. */
      free_dvector(TEMP,1,n);
      free_dvector(Y,1,n);
      free_dvector(X,1,n);
      free_dmatrix(A,1,n,1,n);
      NAA_do_last(outfile);	/* NAA finish-up procedure. */
      exit (0);			/* STOP  - Procedure completed successfully. */
    }

    /* STEP #8 */
    k++;			/* k = k + 1. */

  }

  /* STEP #9 */		/* Procudure completed unseccessfully. */
  printf2("\nMaximum number of iterations (%d) exceeded.\n", N);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(TEMP,1,n);
  free_dvector(Y,1,n);
  free_dvector(X,1,n);
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
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
