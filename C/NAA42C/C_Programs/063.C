/******************************************************************************
	Gaussian Elimination with Scaled Column Pivoting - Algorithm 6.3
*******************************************************************************

To solve the n x n linear system

	E1:   a(1,1)*x1 + a(1,2)*x2 + ... a(1,n)*xn = a(1,n+1)
	E2:   a(2,1)*x1 + a(2,2)*x2 + ... a(2,n)*xn = a(2,n+1)
	.        .           .               .           .
	.        .           .               .           .
	.        .           .               .           .
	En:   a(n,1)*x1 + a(n,2)*x2 + ... a(n,n)*xn = a(n,n+1):

INPUT number of equations and unknowns n; augmented matrix A = (a(i,j)) where
1 ó i ó n and 1 ó j ó n + 1.

OUTPUT solution x1,x2,...,xn or message that linear system has no unique
solution.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "063.out";	/* Customized default output file name.     */


main()
{
  float **A, **E, **m, *s, *X, max, sum, temp;
  int i, j, k, n, p, *NROW, NCOPY;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Gaussian Elimination with Scaled Column Pivoting");
  printf2(" - Algorithm 6.3\n\n");
  printf2("Solves A*X = B.\n\n");

  do {
    printf("Enter the dimension, n, of the matrix A: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greter than zero.\n");
  } while (n <= 0);

  /* Dynamically allocate memory for the needed arrays. */
  E = matrix(1,n,1,n+1);	/* Matrix A augmented with B. */
  m = matrix(1,n,1,n+1);	/* Matrix m.                  */
  s = vector(1,n);		/* Scale vector.              */
  X = vector(1,n);		/* Solution vector.           */
  NROW = ivector(1,n);		/* Row Pointer vector.        */
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

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */			/* Difference from Algorithm 6.2. */
  for (i=1;i<=n;i++) {
    max = 0.0;
    for (j=1;j<=n;j++) {	/* s(i) = max{| a(i)(j) |}        */
      temp = fabs(A[i][j]);
      if (temp > max)
        s[i] = max = temp;
    }
    if (s[i] == 0.0) {
      printf2("No unique solution exists (0).\n");
      NAA_do_last(outfile);	/* NAA finish-up procedure. */
      exit (-1);		/* STOP */
    }
    NROW[i] = i;		/* Initialize row pointer.  */
  }

  /* STEP #2 */			/* Difference from Algorithm 6.2.       */
  for (i=1;i<n;i++) {		/* Do Steps 3-6.  Elimination process.  */

    printf2("At row %d:\n", i);
    printf2("E = ");
    print_matrix(E,n,NROW);	/* Show intermediate results for looks. */

    /* STEP #3 */
    p = 1;
    max = 0.0;
    for (j=i;j<=n;j++) {	/* Find maximum element in column i.        */
      temp = fabs(A[NROW[j]][i]) / s[NROW[p]];	/* Different from Alg. 6.2. */
      if (temp > max) {
        max = temp;
        p = j;
      }
    }

    /* STEP #4 */
    if (max == 0.0) {		/* max = 0.0 indicates an all zero column. */
      printf2("No unique solution exists (1).\n");
      NAA_do_last(outfile);	/* NAA finish-up procedure. */
      exit (-1);		/* STOP */
    }

    /* STEP #5 */
    if (NROW[i] != NROW[p]) {	/* Simulated row interchange. */
      NCOPY   = NROW[i];
      NROW[i] = NROW[p];
      NROW[p] = NCOPY;
      printf2("Need to swap row %d with row %d.\n\n", i, p);
    } else
      printf2("No rows need swapping.\n\n");	/* For consistency. */

    /* STEP #6 */
    for (j=i+1;j<=n;j++) {	/* Do Steps 7-8. */

      /* STEP #7 */
      m[NROW[j]][i] = A[NROW[j]][i] / A[NROW[i]][i];

      /* STEP #8 */
      for (k=1;k<=n+1;k++)
        E[NROW[j]][k] -= m[NROW[j]][i] * E[NROW[i]][k];
    }
  }

  printf2("At row %d:\n", n);
  printf2("E = ");
  print_matrix(E,n,NROW);	/* Show intermediate results for looks. */

  /* STEP #9 */
  if (A[NROW[n]][n] == 0.0) {
    printf2("No unique solution exists (2).\n");
    NAA_do_last(outfile);	/* NAA finish-up procedure. */
    exit (-1);			/* STOP */
  }

  /* STEP #10 */
  X[n] = E[NROW[n]][n+1] / A[NROW[n]][n]; /* Start backward substitution. */

  /* STEP #11 */
  for (i=n-1;i>=1;i--) {
    sum = 0.0;
    for (j=i+1;j<=n;j++)  sum += A[NROW[i]][j] * X[j];
    X[i] = (E[NROW[i]][n+1] - sum) / A[NROW[i]][i];
  }

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #10 */
  printf2("\nX = [ ");		/* Procedure completed successfully. */
  for (i=1;i<=n;i++)
    printf2("% .8g ", X[i]);
  printf2("]\n");

  /* Free the memory that was dynamically allocated for the arrays. */
  free_ivector(NROW,1,n);
  free_vector(X,1,n);
  free_vector(s,1,n);
  free_matrix(m,1,n,1,n+1);
  free_matrix(E,1,n,1,n+1);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* print_matrix() - Prints out a matrix.                                     */
/*****************************************************************************/
print_matrix(E, n, NROW)
float **E;
int n, *NROW;
{
  int i, j;

  for (i=1;i<=n;i++) {
    printf2("[ ");
    for (j=1;j<=n;j++)
      printf2("% 3g ", E[NROW[i]][j]);
    printf2(" | % 3g ", E[NROW[i]][n+1]);
    printf2("]\n    ");
  }
  printf2("\n");

}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
