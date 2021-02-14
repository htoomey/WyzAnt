/******************************************************************************
		Householder Method (old version) - Algorithm 9.5B
*******************************************************************************

To obtain a symmetric tridiagonal matrix A(n-1) similar to the symmetric
matrix A = A(1) construct the following matrices A(2), A(3),..., A(n-1) where
A(k) = a(k)(i,j) for each k = 1, 2,..., n - 1.

Note: This algorithm is from the third edition of the text.

INPUT dimension n; matrix A.

OUTPUT A(n-1).  (Could over-write A.)

PROGRAMMER'S NOTE: Don't over-write A.  Used A1 for A(k) and A2 for A(k+1).

NOTE: Modifications to the algorithm were introduced into the 4th edition of
the text.  This code is retained for compatibility with the third edition of
the text.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "095b.out";	/* Customized default output file name.     */
int n;				/* Number of equations and unknowns.        */


main()
{
  double **A1, **A2, *U, *V, *Z, q, s, RSQ, PROD;
  int i, j, k, l;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Householder Method (old version) - Algorithm 9.5B\n\n");

  do {
    printf("Enter the dimension, n, of the matrix A: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greter than zero.\n");
  } while (n <= 0);
  fprintf(file_id, "System has %d equations and %d unknowns.\n\n", n, n);

  /* Dynamically allocate memory for the needed arrays. */
  A1 = dmatrix(1,n,1,n);	/* Matrix A(k)   */
  A2 = dmatrix(1,n,1,n);	/* Matrix A(k+1) */
  U  = dvector(1,n);
  V  = dvector(1,n);
  Z  = dvector(1,n);

  printf("Enter the coefficients for matrix A:\n");	/* Get A. */
  for (i=1;i<=n;i++)
    for (j=1;j<=n;j++) {
      printf("\tA1[%d][%d] = ", i, j);
      scanf("%lf", &A1[i][j]);
    }

  fprintf(file_id, "A = ");	/* Print Matrix A to file. */
  for (i=1;i<=n;i++) {
    fprintf(file_id, "[ ");
    for (j=1;j<=n;j++)
      fprintf(file_id, "% 3lg  ", A1[i][j]);
    fprintf(file_id, "]\n    ");
  }
  fprintf(file_id, "\n");

  for (j=1;j<=n;j++)		/* Copy A(k+1) into A(k) since some of A2 */
    for (l=1;l<=n;l++)		/* never gets modified.                   */
      A2[j][l] = A1[j][l];

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */ 
  for (k=1;k<=n-2;k++) {	/* Do Steps 2-10. */

    /* STEP #2 */
    i = n - k + 2;

    /* STEP #3 */
    q = 0.0;
    for (j=1;j<=i-2;j++)
      q += A1[i-1][j] * A1[i-1][j];
    q = sqrt(q);

    /* STEP #4 */
    if (A1[i-1][i-2] == 0.0)
      s = q;
    else
      s = q*A1[i-1][i-2] / fabs(A1[i-1][i-2]);

    /* STEP #5 */
    RSQ = q*q + s*A1[i-1][i-2];	/* Note: RSQ = 2r^2 */

    /* STEP #6 */
    V[i-1] = 0.0;	/* Note: vi = ... = vn = 0, but are not needed. */
    V[i-2] = A1[i-1][i-2] + s;
    if (i > 3)
      for (j=1;j<=i-3;j++)
        V[j] = A1[i-1][j];
    /* (Note: W = (1 / sqrt(2*RSQ)) * V  = V / 2*r.) */

    /* STEP #7 */
    for (j=1;j<=i-1;j++) {
      U[j] = 0.0;
      for (l=1;l<=i-2;l++)
        U[j] += A1[j][l] * V[l];
      U[j] /= RSQ;
    }
    /* (Note: U = (1 / RSQ) * A(k) * V  = V*A(k) / 2*r^2.) */

    /* STEP #8 */
    PROD = 0.0;
    for (j=1;j<=i-2;j++)
      PROD += V[j] * U[j];
    /* (Note: PROD = Vt*U = Vt*A(k)*V / 2*r^2)             */

    /* STEP #9 */
    for (j=1;j<i;j++)
      Z[j] = U[j] - PROD * V[j] / (2.0*RSQ);

    /*
    **  Note: Z = U - Vt*U*V / 2*RSQ = U - Vt*U*V / 4*r^2
    **          = U - W*Wt*U = A(K)*W / r - W8Wt*(1/r)*A(k)*W.
    */

    /* STEP #10 */
    /* Note: Compute A(k+1) = A(k) - VZt - ZVt = (I - 2WWt)A(k)(I - 2WWt). */
    for (l=2;l<=i-2;l++) {
      for (j=1;j<l;j++) {
        A2[l][j] = A1[l][j] - V[l] * Z[j] - V[j] * Z[l];
        A2[j][l] = A2[l][j];
      }
      A2[l][l] = A1[l][l] - 2.0 * V[l] * Z[l];
      A2[1][1] = A1[1][1] - 2.0 * V[1] * Z[1];
      for (j=1;j<=i-3;j++) {
        A2[i-1][j] = 0.0;
        A2[j][i-1] = 0.0;
      }
      A2[i-1][i-2] = A1[i-1][i-2] - V[i-2] * Z[i-1];
      A2[i-2][i-1] = A2[i-1][i-2];
      /* Note: The other elements of A(k+1) are the same as A(k). */
    }

    for (j=1;j<=n;j++)
      for (l=1;l<=n;l++)
        A1[j][l] = A2[j][l];	/* Set A(k) = A(k+1) for next iteration. */
  }

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #11 */
  /* The process is complete.  A(n-1) is symmetric, tridiagonal and similar */
  /* to A.                                                                  */
  printf2("The tridiagonal Matrix A is:\n\n");
  printf2("A = ");
  for (i=1;i<=n;i++) {
    printf2("[ ");
    for (j=1;j<=n;j++)
      printf2("% .9lf\t", A2[i][j]);
    printf2("]\n    ");
  }

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dmatrix(A1,1,n,1,n);
  free_dmatrix(A2,1,n,1,n);
  free_dvector(U,1,n);
  free_dvector(V,1,n);
  free_dvector(Z,1,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
