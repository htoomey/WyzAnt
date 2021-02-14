/******************************************************************************
			Matrix Inverter - Algorithm 6.0B
*******************************************************************************

Computes the inverse of matrix A:

INPUT matrix dimension n; the entries a(i)(j), 1 ó i,j ó n of the matrix A.

OUTPUT the inverse of A.

NOTE: This algorithm was included as a "Homework Helper."  See p. 344,
Exercise Set 6.4, Problem 1.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */
#include "gaussj.c"		/* Guass-Jordan matrix solver.              */

char *outfile = "060b.out";	/* Customized default output file name.     */


main()
{
  double **A, **I;
  int i, j, n;			/* n is for dimension of n x n matrix */

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Matrix Inverter - Algorithm 6.0B\n\n");

  do {
    printf("Enter the dimension, n, of the matrix A: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greater than zero.\n");
  } while (n <= 0);
  fprintf(file_id, "Dimension, n, of the matrix A: %d\n\n", n);

  /* Dynamically allocate memory for the needed arrays. */
  A = dmatrix(1,n,1,n);
  I = dmatrix(1,n,1,n);

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
      fprintf(file_id, "% 3lg ", A[i][j]);
    fprintf(file_id, "]\n    ");
  }

  for (i=1;i<=n;i++)		/* Forms the Identity matrix.                */
    I[i][i] = 1.0;		/* Other elements are zero due to dmatrix(). */

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  gaussj(A,n,I,n);		/* A returns from gaussj() as A_Inverse.    */

  /***********
   * OUTPUTS *
   ***********/

  printf2("\nA INVERSE = ");
  for (i=1;i<=n;i++) {
    printf2("[ ");
    for (j=1;j<=n;j++)
      printf2("% 3lg ", A[i][j]);
    printf2("]\n            ");
  }

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dmatrix(I,1,n,1,n);
  free_dmatrix(A,1,n,1,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
