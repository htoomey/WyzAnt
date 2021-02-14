/******************************************************************************
		Choleski's which solves AX=B - Algorithm 6.6B
*******************************************************************************

To factor the positive definite n x n matrix A into LLt where L is lower
triangular:

INPUT the dimension n; entries (a(i,j)) 1 ó i,j ó n, of A.

OUTPUT the entries l(i,j), 1 ó j ó i, 1 ó i ó n of L.
(The entries of U = Lt are u(i,j) = l(j,i), i ó j ó n, 1 ó i ó n.)

NOTE: This algorithm was included as a "Homework Helper."  See p. 379,
Exercise Set 6.7, Problem *** 13 ***.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "066b.out";	/* Customized default output file name.     */


main()
{
  double **A, *B, **L, *X, *Y, sum;
  int i, j, k, n;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Choleski's which solves AX = B - Algorithm 6.6B\n\n");
  printf2("Forms A = LLt where L is lower triangular.\n\n");

  do {
    printf("Enter the dimension, n, of the matrix A: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greter than zero.\n");
  } while (n <= 0);

  /* Dynamically allocate memory for the needed arrays. */
  A = dmatrix(1,n,1,n+1);	/* Matrix A. */
  L = dmatrix(1,n,1,n);		/* Matrix L. */
  B = dvector(1,n);		/* Vector B. */
  X = dvector(1,n);		/* Vector X. */
  Y = dvector(1,n);		/* Vector Y. */

  printf("Enter the coefficients for matrix A:\n");	/* Get A. */
  for (i=1;i<=n;i++)
    for (j=1;j<=n;j++) {
      printf("\tA[%d][%d] = ", i, j);
      scanf("%lf", &A[i][j]);
    }
  printf("\n");

  fprintf(file_id, "A = ");	/* Print Matrix A to file. */
  for (i=1;i<=n;i++) {
    fprintf(file_id, "[ ");
    for (j=1;j<=n;j++)
      fprintf(file_id, "% 9.9lg ", A[i][j]);
    fprintf(file_id, "]\n    ");
  }
  fprintf(file_id, "\n");

  /* Validate input matrix, exiting if it is not positive definite. */
  for (i=1;i<=n;i++) {
    if (A[i][i] <= 0.0) {	/* Theorem 6.24 (b)         */
      printf2("ERROR: Matrix A is not positive definite.\n");
      NAA_do_last(outfile);	/* NAA finish-up procedure. */
      exit (-1);
    }
    for (j=1;j<=n;j++)		/* Theorem 6.24 (d)         */
      if (i != j)
        if (A[i][j]*A[i][j] >= A[i][i]*A[j][j]) {
          printf2("ERROR: Matrix A is not positive definite.\n");
          NAA_do_last(outfile);	/* NAA finish-up procedure. */
          exit (-1);
        }
  }

  printf("Enter the coefficients for vector B:\n");	/* Get B. */
  for (i=1;i<=n;i++) {
    printf("\tB[%d] = ", i);
    scanf("%lf", &B[i]);
  }
  printf("\n");

  fprintf(file_id, "B = [ ");	/* Print Vector B to file. */
  for (i=1;i<=n;i++)
    fprintf(file_id, "% 9.9lg ", B[i]);
  fprintf(file_id, "]\n\n");

  for (i=1;i<=n;i++)	/* Initialize L leaving zeros on upper triangle. */
    for (j=1;j<=i;j++)
      L[i][j] = A[i][j];

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  L[1][1] = sqrt(A[1][1]);

  /* STEP #2 */
  for (j=2;j<=n;j++)
    L[j][1] = A[j][1] / L[1][1];

  /* STEP #3 */
  for (i=2;i<n;i++) {		/* Do Steps 4-5. */

    /* STEP #4 */
    sum = 0.0;
    for (k=1;k<i;k++)
      sum += L[i][k] * L[i][k];
    L[i][i] = sqrt(A[i][i] - sum);

    /* STEP #5 */
    for (j=i+1;j<=n;j++) {
      sum = 0.0;
      for (k=1;k<i;k++)
        sum += L[j][k] * L[i][k];
      L[j][i] = (A[j][i] - sum) / L[i][i];
    }
  }

  /* STEP #6 */
  sum = 0.0;
  for (k=1;k<n;k++)
    sum += L[n][k] * L[n][k];
  L[n][n] = sqrt(A[n][n] - sum);

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #7 */
  printf2("L = ");
  for (i=1;i<=n;i++) {
    printf2("[ ");
    for (j=1;j<=n;j++)
      printf2("% 9.9lg ", L[i][j]);
    printf2("]\n    ");
  }
  printf2("\n");

  printf2("Lt= ");
  for (i=1;i<=n;i++) {
    printf2("[ ");
    for (j=1;j<=n;j++)
      printf2("% 9.9lg ", L[j][i]);
    printf2("]\n    ");
  }
  printf2("\n");

  /*
  **  Steps 8-12 are additions needed to solve AX = B (also prompt for B).
  */

  /* STEP #8 */
  Y[1] = B[1] / L[1][1];

  /* STEP #9 */
  for (i=2;i<=n;i++) {
    sum = 0.0;
    for (j=1;j<i;j++)
      sum += L[i][j] * Y[j];
    Y[i] = (B[i] - sum) / L[i][i];
  }

  /* STEP #10 */
  X[n] = Y[n] / L[n][n];

  /* STEP #11 */
  for (i=n-1;i>=1;i--) {
    sum = 0.0;
    for (j=i+1;j<=n;j++)	/* The algorithm in the text is incorrect. */
      sum += L[j][i] * X[j];		/* Change L[i][j] to L[j][i] AND   */
    X[i] = (Y[i] - sum) / L[i][i];	/* change Y[j] to X[j].            */
  }

  /* STEP #12 */
  printf2("X = [ ");		/* Procedure completed successfully. */
  for (i=1;i<=n;i++)
    printf2("% 9.9lg ", X[i]);
  printf2("]\n");

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(Y,1,n);
  free_dvector(X,1,n);
  free_dvector(B,1,n);
  free_dmatrix(L,1,n,1,n);
  free_dmatrix(A,1,n,1,n+1);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
