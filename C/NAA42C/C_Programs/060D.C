/******************************************************************************
		     Matrix Multiplier - Algorithm 6.0D
*******************************************************************************

Multiplies two n x n matrices (A x B = C):

INPUT matrix dimensions n; the entries a(i)(j), 1 ó i,j ó n of the matrix A
and the entries b(i)(j), 1 ó i,j ó n of the matrix B.

OUTPUT the product matrix C.

NOTE: This algorithm was included as a "Homework Helper."  See p. 344,
Exercise Set 6.4, Problem 2.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "060d.out";	/* Customized default output file name.     */


main()
{
  double **A, **B, **C;
  int i, j, k, n;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Matrix Multiplier - Algorithm 6.0D\n\n");

  do {
    printf("Enter the dimension of the matrix A and B: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greater than zero.\n");
  } while (n <= 0);
  fprintf(file_id, "Dimension of matrix A and B: n = %d\n\n", n);

  /* Dynamically allocate memory for the needed arrays. */
  A = dmatrix(1,n,1,n);		/* Matrix A */
  B = dmatrix(1,n,1,n);		/* Matrix B */
  C = dmatrix(1,n,1,n);		/* Matrix C */

  printf("Enter the coefficients for matrix A:\n");	/* Get A. */
  for (i=1;i<=n;i++)
    for (j=1;j<=n;j++) {
      printf("\tA[%d][%d] = ", i, j);
      scanf("%lf", &A[i][j]);
    }
  printf("\n");

  fprintf(file_id, "A = ");	/* Print Matrix A to a file. */
  for (i=1;i<=n;i++) {
    fprintf(file_id, "[ ");
    for (j=1;j<=n;j++)
      fprintf(file_id, "% 9lg ", A[i][j]);
    fprintf(file_id, "]\n    ");
  }
  fprintf(file_id, "\n");

  printf("Enter the coefficients for matrix B:\n");	/* Get B. */
  for (i=1;i<=n;i++)
    for (j=1;j<=n;j++) {
      printf("\tB[%d][%d] = ", i, j);
      scanf("%lf", &B[i][j]);
    }
  printf("\n");

  fprintf(file_id, "B = ");	/* Print Matrix B to a file. */
  for (i=1;i<=n;i++) {
    fprintf(file_id, "[ ");
    for (j=1;j<=n;j++)
      fprintf(file_id, "% 9lg ", B[i][j]);
    fprintf(file_id, "]\n    ");
  }
  fprintf(file_id, "\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  for (i=1;i<=n;i++)		/* Do the actual multiplication. */
    for (j=1;j<=n;j++)
      for (k=1;k<=n;k++)
        C[i][j] += A[i][k] * B[k][j];

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #2 */
  printf2("C = ");		/* Print Solution Matrix C. */
  for (i=1;i<=n;i++) {
    printf2("[ ");
    for (j=1;j<=n;j++)
      printf2("% 9lg ", C[i][j]);
    printf2("]\n    ");
  }

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dmatrix(C,1,n,1,n);
  free_dmatrix(B,1,n,1,n);
  free_dmatrix(A,1,n,1,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
