/******************************************************************************
		      Gauss-Seidel Iterative - Algorithm 7.2
*******************************************************************************

To solve AX = B given an initial approximation X0:

INPUT the number of equations and unknowns n; the entries a(i,j),
1 ó i,j ó n of the matrix A; the entries b(i), 1 ó i ó n of the
inhomogeneous term B; the entries X0(i), 1 ó i ó n of X0 = x(0);
tolerance TOL; maximum number of iterations N.

OUTPUT the approximate solution x1,...,xn or a message that the number of
iterations was exceeded.

NOTE: Listed as Gauss-Seidel in the 3rd edition of the text.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "072.out";	/* Customized default output file name.     */
int n;				/* Number of equations and unknowns.        */


main()
{
  double **A, *X, *B, *X0, *TEMP, TOL, infinite_norm();
  int i, j, k, N;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Gauss-Seidel Iterative - Algorithm 7.2\n\n");

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
  B    = dvector(1,n);		/* Matrix B                   */
  X0   = dvector(1,n);		/* initial solution vector    */
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
      fprintf(file_id, "% 9.9lg ", A[i][j]);
    fprintf(file_id, "]\n    ");
  }
  fprintf(file_id, "\n");

  printf("Enter the coefficients for matrix B:\n");	/* Get B. */
  for (i=1;i<=n;i++) {
    printf("\tB[%d] = ", i);
    scanf("%lf", &B[i]);
  }

  fprintf(file_id, "B = [ ");	/* Print Vector B to file. */
  for (i=1;i<=n;i++)
    fprintf(file_id, "% 9.9lg ", B[i]);
  fprintf(file_id, "]t\n\n");

  printf("Enter initial approximation vector X0:\n");	/* Get X0. */
  for (i=1;i<=n;i++) {
    printf("\tX0[%d] = ", i);
    scanf("%lf", &X0[i]);
  }

  fprintf(file_id, "X0 = [ ");	/* Print Vector X0 to file. */
  for (i=1;i<=n;i++)
    fprintf(file_id, "% 9.9lg ", X0[i]);
  fprintf(file_id, "]t\n");

  printf2("\n k\t");		/* print out table header   */
  for (i=1;i<=n;i++)
    printf2(" X[%d]\t\t", i);
  printf2("\n----------");
  for (i=1;i<=n;i++)
    printf2("---------------");
  printf2("\n");

  printf2("% d\t", 0);		/* Print initial values.    */
  for (i=1;i<=n;i++)
    printf2("% .9lf\t", X0[i]);
  printf2("\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  k = 1;

  /* STEP #2 */
  while (k <= N) {		/* Do Steps 3-6. */

    /* STEP #3 */
    for (i=1;i<=n;i++) {
      X[i] = 0.0;
      for (j=1;j<=i-1;j++)
        X[i] -= A[i][j] * X[j];
      for (j=i+1;j<=n;j++)
        X[i] -= A[i][j] * X0[j];
      X[i] = (X[i] + B[i]) / A[i][i];
    }

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #4 */
    printf2(" %d\t", k);
    for (i=1;i<=n;i++)
      printf2("% .9lf\t", X[i]);
    printf2("\n");

    for (i=1;i<=n;i++)
      TEMP[i] = X[i] - X0[i];
    if (fabs(infinite_norm(TEMP)) < TOL) {
      /* Free the memory that was dynamically allocated for the arrays. */
      free_dvector(TEMP,1,n);
      free_dvector(X0,1,n);
      free_dvector(B,1,n);
      free_dvector(X,1,n);
      free_dmatrix(A,1,n,1,n);
      NAA_do_last(outfile);	/* NAA finish-up procedure. */
      exit (1);			/* STOP - Procedure completed successfully. */
    }

    /* STEP #5 */
    k++;			/* k = k + 1. */

    /* STEP #6 */
    for (i=1;i<=n;i++)
      X0[i] = X[i];
  }

  /* STEP #7 */			/* Procedure completed unsuccessfully. */
  printf2("\nMaximum number of iterations (%d) exceeded.\n", N);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(TEMP,1,n);
  free_dvector(X0,1,n);
  free_dvector(B,1,n);
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
