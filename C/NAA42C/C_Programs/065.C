/******************************************************************************
			LDLt Factorization - Algorithm 6.5
*******************************************************************************

To factor the positive definite n x n matrix A into the form LDLt, where L is
a lower triangular matrix with ones along the diagonal and D is a diagonal
matrix with positive entries on the diagonal:

INPUT dimension n; entries (a(i,j)) 1 ó i,j ó n.

OUTPUT the entries l(i,j), 1 ó j ó i, 1 ó i ó n of L and d(i), 1 ó i ó n
of D.

NOTE: Newly introduced into the 4th edition of the text.
For p. 379, Exercise Set 6.7, Problem 16, remove the comments just before
STEP #1.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "065.out";	/* Customized default output file name.     */


main()
{
  double **A, **L, *D, *V, sum;
  int i, j, k, n;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("LDLt Factorization - Algorithm 6.5\n\n");

  do {
    printf("Enter the dimension, n, of the matrix A: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greter than zero.\n");
  } while (n <= 0);

  /* Dynamically allocate memory for the needed arrays. */
  A = dmatrix(1,n,1,n);		/* Matrix A. */
  L = dmatrix(1,n,1,n);		/* Matrix L. */
  D = dvector(1,n);		/* Vector D. */
  V = dvector(1,n);		/* Vector V. */

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
  fprintf(file_id, "\n\n");

  /*
  ** Remove the below comments to factor a symmetric matrix.
  */

/*
**   for (i=1;i<=n;i++)
**     if (A[i][i] == 0.0)
**       naaerror("Can't be done.  There is a zero diagonal element.");
*/

  for (i=1;i<=n;i++)		/* Put 1's on diagonal of L. */
    L[i][i] = 1;		/* Should be in text!        */

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  for (i=1;i<=n;i++) {		/* Do Steps 2-4. */

    /* STEP #2 */
    for (j=1;j<i;j++)
      V[j] = L[i][j] * D[j];	/* The algorithm in the text is incorrect. */
				/* Replace A[i][j] with L[i][j].           */
    /* STEP #3 */
    sum = 0.0;
    for (j=1;j<i;j++)		/* The algorithm in the text is incorrect. */
      sum += L[i][j] * V[j];	/* Replace A[i][j] with L[i][j].           */

    D[i] = A[i][i] - sum;

    /* STEP #4 */
    for (j=i+1;j<=n;j++) {
      sum = 0.0;
      for (k=1;k<i;k++)		/* The algorithm in the text is incorrect. */
        sum += L[j][k] * V[k];	/* Replace A[j][k] with L[j][k].           */
      L[j][i] = (A[j][i] - sum) / D[i];
    }
  }

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #5 */
  printf2("L  = ");		/* Print out matrix L.            */
  print_matrix(L,n);

  for (i=1;i<=n;i++)		/* Put vector D into a matrix, A. */
    for (j=1;j<=n;j++)
      if (i == j)
        A[i][j] = D[i];
      else
        A[i][j] = 0.0;

  printf2("D  = ");		/* Print out matrix D.            */
  print_matrix(A,n);

  for (i=1;i<=n;i++)		/* Make Lt, storing it in A.      */
    for (j=1;j<=n;j++)
      A[i][j] = L[j][i];

  printf2("Lt = ");		/* Print out matrix L Transpose.  */
  print_matrix(A,n);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(V,1,n);
  free_dvector(D,1,n);
  free_dmatrix(L,1,n,1,n);
  free_dmatrix(A,1,n,1,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* print_matrix() - Prints out a matrix.                                     */
/*****************************************************************************/
print_matrix(A, n)
double **A;
int n;
{
  int i, j;

  for (i=1;i<=n;i++) {
    printf2("[ ");
    for (j=1;j<=n;j++)
      printf2("% 9.9lg ", A[i][j]);
    printf2("]\n     ");
  }
  printf2("\n");

}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
