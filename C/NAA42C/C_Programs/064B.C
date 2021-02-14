/******************************************************************************
	Direct Factorization which solves AX=B - Algorithm 6.4B
*******************************************************************************

To factor the n x n matrix A = (a(i,j)) into the product of the lower-
triangular matrix L = (l(i,j)) and the upper-triangular matrix U = (u(i,j)),
that is, A = LU where the main diagonal of either L or U is given.  Afterwards
solve the n x n linear system

	E1:   a(1,1)*x1 + a(1,2)*x2 + ... a(1,n)*xn = a(1,n+1)
	E2:   a(2,1)*x1 + a(2,2)*x2 + ... a(2,n)*xn = a(2,n+1)
	.        .           .               .           .
	.        .           .               .           .
	.        .           .               .           .
	En:   a(n,1)*x1 + a(n,2)*x2 + ... a(n,n)*xn = a(n,n+1)

with A factored into LU, solving LZ = B and UX = Z where the main diagonal of
either L or U is given:

INPUT dimension n; the entries (a(i,j)) 1 ó i,j ó n, of the augmented form of
A; the diagonal l(1,1),...,l(n,n) of L or the diagonal u(1,1),...,u(n,n) of U.

OUTPUT solution x1,x2,...,xn or message that linear system has no unique
solution.

NOTE: This algorithm was included as a "Homework Helper."  See p. 364,
Exercise Set 6.6, Problem *** 2 ***.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "064b.out";	/* Customized default output file name.     */


main()
{
  float **A, **E, **U, **L, *X, *Z, sum, HOLD;
  int i, j, k, n, ch;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Direct Factorization which solves AX=B - Algorithm 6.4B\n\n");
  printf2("Solves A*X = B by forming A = LU.\n\n");

  do {
    printf("Enter the dimension, n, of the matrix A: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greter than zero.\n");
  } while (n <= 0);

  /* Dynamically allocate memory for the needed arrays. */
  E = matrix(1,n,1,n+1);	/* Matrix A augmented with B. */
  U = matrix(1,n,1,n);		/* Matrix U.                  */
  L = matrix(1,n,1,n);		/* Matrix L.                  */
  X = vector(1,n);		/* Solution vector.           */
  Z = vector(1,n);		/* Partial solution vector.   */
  A = E;			/* Matrix A (a subset of E).  */

  printf("Enter the coefficients for matrix A:\n");	/* Get A. */
  for (i=1;i<=n;i++)
    for (j=1;j<=n;j++) {
      printf("\tA[%d][%d] = ", i, j);
      scanf("%f", &A[i][j]);
    }

  fprintf(file_id, "A = ");	/* Print Matrix A to file. */
  for (i=1;i<=n;i++) {
    fprintf(file_id, "[ ");
    for (j=1;j<=n;j++)
      fprintf(file_id, "% 3g ", A[i][j]);
    fprintf(file_id, "]\n    ");
  }
  fprintf(file_id, "\n");

  printf("Enter the coefficients for vector B:\n");	/* Get B. */
  for (i=1;i<=n;i++) {
    printf("\tB[%d] = ", i);
    scanf("%f", &E[i][n+1]);
  }
  printf("\n");

  fprintf(file_id, "B = [ ");	/* Print Vector B to file. */
  for (i=1;i<=n;i++)
    fprintf(file_id, "% 3g ", E[i][n+1]);
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
  if (A[1][1] == 0.0) {
    printf2("Factorizaton Impossible (1).\n");
    NAA_do_last(outfile);	/* NAA finish-up procedure. */
    exit (-1);			/* STOP */
  }

  printf("\nWhat type of diagonal would you like?\n");
  printf("   1 - Doolittle     (l(i)(i) = 1)\n");
  printf("   2 - Crout         (u(i)(i) = 1)\n");
  printf("   3 - Other Upper   (u(i)(i) = ?)\n");
  printf("   4 - Other Lower   (l(i)(i) = ?)\n");
  printf("----> ");
  scanf("%d", &ch);
  if ((ch < 1) || (ch > 4))
    ch = 1;			/* Default to Doolittle method. */
  printf("\n");

  switch (ch) {
    case 1:  printf2("Using Doolittle's Method.\n\n");
             for (i=1;i<=n;i++)
               L[i][i] = 1.0;
             break;

    case 2:  printf2("Using Crout's Method.\n\n");
             for (i=1;i<=n;i++)
               U[i][i] = 1.0;
             break;

    case 3:  printf2("Using Other Upper Method.\n\n");
             for (i=1;i<=n;i++) {
               printf("U[%d][%d] = ", i, i);
               scanf("%f", &U[i][i]);
             }
             L[1][1] = A[1][1] / U[1][1];
             break;

    case 4:  printf2("Using Other Lower Method.\n\n");
             for (i=1;i<=n;i++) {
               printf("L[%d][%d] = ", i, i);
               scanf("%f", &L[i][i]);
             }
             U[1][1] = A[1][1] / L[1][1];
             break;
  }

  /* STEP #2 */
  for (j=2;j<=n;j++) {
    U[1][j] = A[1][j] / L[1][1];	/* First row of U.    */
    L[j][1] = A[j][1] / U[1][1];	/* First column of L. */
  }

  /* STEP #3 */
  for (i=2;i<n;i++) {		/* Do Steps 4-5. */

    /* STEP #4 */
    sum = 0.0;
    for (k=1;k<i;k++)
      sum += L[i][k]*U[k][i];
    switch (ch) {
      case 1:  
      case 4:  U[i][i] = (A[i][i]-sum) / L[i][i];  break;
      case 2:  
      case 3:  L[i][i] = (A[i][i]-sum) / U[i][i];  break;
    }

    if (L[i][i]*U[i][i] == 0.0) {	/* OPTIONAL? */
      printf2("Factorizaton Impossible (2).\n");
      NAA_do_last(outfile);	/* NAA finish-up procedure. */
      exit (-1);		/* STOP */
    }

    /* STEP #5 */
    for (j=i+1;j<=n;j++) {
      sum = 0.0;
      for (k=1;k<i;k++)
        sum += L[i][k] * U[k][j];	/* ith row of U.    */
      U[i][j] = (A[i][j] - sum) / L[i][i];
      sum = 0.0;
      for (k=1;k<i;k++)
        sum += L[j][k] * U[k][i];	/* ith column of L. */
      L[j][i] = (A[j][i] - sum) / U[i][i];
    }
  }

  /* STEP #6 */
  sum = 0.0;
  for (k=1;k<n;k++)
    sum += L[n][k] * U[k][n];
  HOLD = A[n][n] - sum;

  if (HOLD == 0.0) {
    printf2("No unique solution exists (3).\n");
    NAA_do_last(outfile);	/* NAA finish-up procedure. */
    exit (-1);			/* STOP */
  }

  switch (ch) {
    case 1:  
    case 4:  U[n][n] = (A[n][n]-sum) / L[n][n];  break;
    case 2:  
    case 3:  L[n][n] = (A[n][n]-sum) / U[n][n];  break;
  }

  /*
  ** Steps 7 and 8 solve lower-triangular system LZ = B.
  */

  /* STEP #7 */
  Z[1] = A[1][n+1] / L[1][1];

  /* STEP #8 */
  for (i=2;i<=n;i++) {
    sum = 0.0;
    for (j=1;j<i;j++)
      sum += L[i][j] * Z[j];
    Z[i] = (A[i][n+1] - sum) / L[i][i];
  }

  /*
  ** Steps 9 and 10 solve upper-triangular system UX = Z.
  */

  /* STEP #9 */
  X[n] = Z[n] / U[n][n];

  /* STEP #10 */
  for (i=n-1;i>=1;i--) {
    sum = 0.0;
    for (j=i+1;j<=n;j++)
      sum += U[i][j] * X[j];
    X[i] = (Z[i] - sum) / U[i][i];
  }

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #11 */
  printf2("L = ");
  print_matrix(L,n);

  printf2("U = ");
  print_matrix(U,n);

  printf2("\nX = [ ");		/* Procedure completed successfully. */
  for (i=1;i<=n;i++)
    printf2("% .8g ", X[i]);
  printf2("]\n");

  /* Free the memory that was dynamically allocated for the arrays. */
  free_vector(Z,1,n);
  free_vector(X,1,n);
  free_matrix(L,1,n,1,n);
  free_matrix(U,1,n,1,n);
  free_matrix(E,1,n,1,n+1);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* print_matrix() - Prints out a matrix.                                     */
/*****************************************************************************/
print_matrix(A,n)
float **A;
int n;
{
  int i, j;

  for (i=1;i<=n;i++) {
    printf2("[ ");
    for (j=1;j<=n;j++)
      printf2("% 10.7f ", A[i][j]);
    printf2("]\n    ");
  }
  printf2("\n");

}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
