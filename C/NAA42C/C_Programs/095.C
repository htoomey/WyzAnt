/******************************************************************************
		    Householder Method - Algorithm 9.5
*******************************************************************************

To obtain a symmetric tridiagonal matrix A(n-1) similar to the symmetric
matrix A = A(1) construct the following matrices A(2), A(3),..., A(n - 1) where
A(k) = a(k)(i,j) for each k = 1, 2,..., n - 1.

INPUT dimension n; matrix A.

OUTPUT A(n - 1).  (At each step, A can be overwritten.)

NOTE: Modifications to the algorithm were introduced into the 4th edition of
the text.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "095.out";	/* Customized default output file name.     */
int n;				/* Number of equations and unknowns.        */


main()
{
  double **A, *U, *V, *Z, q, RSQ, PROD, alpha;
  int i, j, k, l;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Householder Method - Algorithm 9.5\n\n");

  do {
    printf("Enter the dimension, n, of the matrix A: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greter than zero.\n");
  } while (n <= 0);
  fprintf(file_id, "System has %d equations and %d unknowns.\n\n", n, n);

  /* Dynamically allocate memory for the needed arrays. */
  A  = dmatrix(1,n,1,n);	/* Matrix A */
  U  = dvector(1,n);
  V  = dvector(1,n);
  Z  = dvector(1,n);

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

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */ 
  for (k=1;k<=n-2;k++) {	/* Do Steps 2-14. */

    /* STEP #2 */
    q = 0.0;
    for (j=k+1;j<=n;j++)
      q += A[j][k] * A[j][k];

    /* STEP #3 */
    if (A[k+1][k] == 0.0)
      alpha = sqrt(q);
    else
      alpha = -sqrt(q)*A[k+1][k] / fabs(A[k+1][k]);

    /*
    **  To save a possible multiply and a divide, replace STEP #3 with:
    **
    **  STEP #3
    **  if (A[k+1][k] <= 0.0)
    **    alpha = sqrt(q);
    **  else
    **    alpha = -sqrt(q);
    */

    /* STEP #4 */
    RSQ = alpha*alpha - alpha*A[k+1][k];	/* Note: RSQ = 2r^2      */

    /* STEP #5 */
    V[k] = 0.0;		/* Note: vi = ... = vn = 0, but are not needed.  */
    V[k+1] = A[k+1][k] - alpha;
    for (j=k+2;j<=n;j++)
      V[j] = A[j][k];
			/* (Note: W = (1 / sqrt(2*RSQ)) * V  = V / 2*r.) */

    /* STEP #6 */
    for (j=k;j<=n;j++) {
      U[j] = 0.0;
      for (i=k+1;i<=n;i++)
        U[j] += A[j][i] * V[i];
      U[j] /= RSQ;
    }
			/* (Note: U = (1 / RSQ)*A(k)*V  = V*A(k) / 2*r^2.) */

    /* STEP #7 */
    PROD = 0.0;
    for (i=k+1;i<=n;i++)
      PROD += V[i] * U[i];
			/* (Note: PROD = Vt*U = Vt*A(k)*V / 2*r^2)         */

    /* STEP #8 */
    for (j=k;j<=n;j++)
      Z[j] = U[j] - (PROD / (2.0*RSQ)) * V[j];

    /*
    **  Note: Z = U - Vt*U*V / 2*RSQ = U - Vt*U*V / 4*r^2
    **           = U - W*Wt*U = A(K)*W / r - W8Wt*(1/r)*A(k)*W.
    **
    **  Note: Compute A(k+1) = A(k) - V*Zt - Z*Vt
    **                       = (I - 2*W*Wt)*A(k)*(I - 2*W*Wt).
    */

    /* STEP #9 */
    for (l=k+1;l<n;l++) {	/* Do Steps 10-11. */

      /* STEP #10 */
      for (j=l+1;j<=n;j++) {
        A[j][l] = A[j][l] - V[l] * Z[j] - V[j] * Z[l];
        A[l][j] = A[j][l];
      }

      /* STEP #11 */
      A[l][l] = A[l][l] - 2.0 * V[l] * Z[l];

    }

    /* STEP #12 */
    A[n][n] = A[n][n] - 2.0 * V[n] * Z[n];

    /* STEP #13 */
    for (j=k+2;j<=n;j++) {
      A[k][j] = 0.0;
      A[j][k] = 0.0;
    }

    /* STEP #14 */
    A[k+1][k] = A[k+1][k] - V[k+1] * Z[k];
    A[k][k+1] = A[k+1][k];
		/* Note: The other elements of A(k+1) are the same as A(k). */

  }

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #15 */
  /* The process is complete.  A(n-1) is symmetric, tridiagonal and similar */
  /* to A.                                                                  */
  printf2("The tridiagonal Matrix A is:\n\n");
  printf2("A = ");
  for (i=1;i<=n;i++) {
    printf2("[ ");
    for (j=1;j<=n;j++)
      printf2("% .9lf\t", A[i][j]);
    printf2("]\n    ");
  }

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(Z,1,n);
  free_dvector(V,1,n);
  free_dvector(U,1,n);
  free_dmatrix(A,1,n,1,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
