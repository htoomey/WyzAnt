/******************************************************************************
	Crout Reduction for Tridiagonal Linear Systems - Algorithm 6.7
*******************************************************************************

To solve the n x n linear system

E1:   a(1,1)*x1 + a(1,2)*x2                                       = a(1,n+1)
E2:   a(2,1)*x1 + a(2,2)*x2 + a(2,3)*x3                           = a(2,n+1)
 .                               .                                     .
 .                               .                                     .
 .                               .                                     .
En-1:             a(n-1,n-2)*xn-2 + a(n-1,n-1)*xn-1 + a(n-1,n)*xn = a(n-1,n+1)
En:                                   a(n,n-1)*xn-1 +   a(n,n)*xn = a(n,n+1),

which is assumed to have a unique solution:

INPUT the dimension n; the entries of A.

OUTPUT the solution x1,...,xn.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "067.out";	/* Customized default output file name.     */


main()
{
  double **A, **E, **U, **L, *X, *Z;
  int i, j, n;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Crout Reduction for Tridiagonal Linear Systems");
  printf2(" - Algorithm 6.7\n\n");
  printf2("Solves A*X = B.\n\n");

  do {
    printf("Enter the dimension, n, of the matrix A: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greter than zero.\n");
  } while (n <= 0);

  /* Dynamically allocate memory for the needed arrays. */
  E = dmatrix(1,n,1,n+1);	/* Matrix A augmented with B. */
  U = dmatrix(1,n,1,n);		/* Matrix U.                  */
  L = dmatrix(1,n,1,n);		/* Matrix L.                  */
  X = dvector(1,n);		/* Solution vector.           */
  Z = dvector(1,n);		/* Partial solution vector.   */
  A = E;			/* Matrix A (a subset of E).  */

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

  printf("Enter the coefficients for vector B:\n");	/* Get B. */
  for (i=1;i<=n;i++) {
    printf("\tB[%d] = ", i);
    scanf("%lf", &E[i][n+1]);
  }
  printf("\n");

  fprintf(file_id, "B = [ ");	/* Print Vector B to file. */
  for (i=1;i<=n;i++)
    fprintf(file_id, "% 9.9lg ", E[i][n+1]);
  fprintf(file_id, "]\n\n");

  for (i=1;i<=n;i++)	/* Initialize L leaving zeros on upper triangle. */
    for (j=1;j<=i;j++)
      L[i][j] = A[i][j];

  for (i=1;i<=n;i++)	/* Initialize U leaving zeros on lower triangle. */
    for (j=i;j<=n;j++)
      U[i][j] = A[i][j];

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  L[1][1] = A[1][1];
  U[1][2] = A[1][2] / L[1][1];

  /* STEP #2 */
  for (i=2;i<n;i++) {
    L[i][i-1] = A[i][i-1];	/* ith row of L. */
    L[i][i] = A[i][i] - L[i][i-1] * U[i-1][i];
    U[i][i+1] = A[i][i+1] / L[i][i];	/* (i + 1)th column of U. */
  }

  /* STEP #3 */
  L[n][n-1] = A[n][n-1];	/* nth row of L. */
  L[n][n] = A[n][n] - L[n][n-1] * U[n-1][n];

  /*
  **  Steps 4 and 5 solve L*Z = B.
  */

  /* STEP #4 */
  Z[1] = A[1][n+1] / L[1][1];

  /* STEP #5 */
  for (i=2;i<=n;i++)
    Z[i] = (A[i][n+1] - L[i][i-1] * Z[i-1]) / L[i][i];

  /*
  **  Steps 6 and 7 solve U*X = Z.
  */

  /* STEP #6 */
  X[n] = Z[n];

  /* STEP #7 */
  for (i=n-1;i>=1;i--)
    X[i] = Z[i] - U[i][i+1] * X[i+1];

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #8 */
  printf2("X = [ ");		/* Procedure completed successfully. */
  for (i=1;i<=n;i++)
    printf2("% 9.9lg ", X[i]);
  printf2("]\n");

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(Z,1,n);
  free_dvector(X,1,n);
  free_dmatrix(L,1,n,1,n);
  free_dmatrix(U,1,n,1,n);
  free_dmatrix(E,1,n,1,n+1);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
