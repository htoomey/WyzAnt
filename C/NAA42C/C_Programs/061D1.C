/******************************************************************************
      Gaussian-Elimination - Gauss-Jordan Hybrid Method - Algorithm 6.1D1
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

NOTE: This algorithm was included as a "Homework Helper."  See p. 325,
Exercise Set 6.2, Problems 10, 12.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "061d1.out";	/* Customized default output file name.     */


main()
{
  float **A, **E, **m, *X, temp;
  int i, j, k, n, p, FLAG;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Gaussian-Elimination - Gauss-Jordan Hybrid Method");
  printf2(" - Algorithm 6.1D1\n\n");
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
  X = vector(1,n);		/* Solution vector.           */
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

  /* STEP #1 */
  for (i=1;i<n;i++) {		/* Do Steps 2-4.  Elimination process.  */

    printf2("At row %d:\n", i);
    printf2("E = ");
    print_matrix(E,n);		/* Show intermediate results for looks. */

    /* STEP #2 */
    for (p=i;p<=n;p++) {	/* Let p be the smallest integer with   */
      if (A[p][i] != 0.0) {	/* i ó p ó n and a(pi) != 0.            */
        FLAG = TRUE;
        break;
      }
      FLAG = FALSE;		/* FLAG = FALSE indicates an all zero row. */
    }

    if (i == p)
      printf2("No rows need swapping.\n\n");	/* For consistency. */
    else
      printf2("Need to swap row %d with row %d.\n\n", i, p);

    if (FLAG == FALSE) {
      printf2("No unique solution exists (1).\n");
      NAA_do_last(outfile);	/* NAA finish-up procedure. */
      exit (-1);
    }

    /* STEP #3 */
    if (p != i)			/* Swap Ep with Ei. */
      for (k=1;k<=n+1;k++) {
        temp = E[p][k];
        E[p][k] = E[i][k];
        E[i][k] = temp;
      }

    /* STEP #4 */
    for (j=i+1;j<=n;j++) {	/* Do Steps 5-6. */

      /* STEP #5 */
      m[j][i] = A[j][i]/A[i][i];

      /* STEP #6 */
      for (k=1;k<=n+1;k++)
        E[j][k] -= m[j][i]*E[i][k];
    }
  }

  /* STEP #7 */
  if (A[n][n] == 0.0) {
    printf2("No unique solution exists (2).\n");
    NAA_do_last(outfile);	/* NAA finish-up procedure. */
    exit (-1);
  }

  printf2("At row %d:\n", n);
  printf2("E = ");
  print_matrix(E,n);		/* Show intermediate results for looks. */

  /*
  **  Addition needed to turn Gaussian Elimination with Backward Substitution
  **  into Gaussian-Elimination - Gauss-Jordan Hybrid Method. Steps 8 and 9
  **  were modified too.
  */

  /* STEP #8 */
  for (j=n;j>=2;j--) {		/* <--- Different from Gauss-Jordan Method. */
    for (i=1;i<j;i++) {
      temp = A[j][j] / A[i][j];
      for (k=1;k<=n+1;k++)
        E[i][k] = -temp*E[i][k] + E[j][k];
    }
    printf2("At column %d:\n", j);
    printf2("E = ");
    print_matrix(E,n);		/* Show intermediate results for looks. */
  }

  /* STEP #9 */
  for (i=1;i<=n;i++)		/* Obtain solution.                     */
    X[i] = E[i][n+1] / A[i][i];

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #10 */
  printf2("\nX = [ ");		/* Procedure completed successfully. */
  for (i=1;i<=n;i++)
    printf2("% .8g ", X[i]);
  printf2("]\n");

  /* Free the memory that was dynamically allocated for the arrays. */
  free_vector(X,1,n);
  free_matrix(m,1,n,1,n+1);
  free_matrix(E,1,n,1,n+1);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* print_matrix() - Prints out a matrix.                                     */
/*****************************************************************************/
print_matrix(E,n)
float **E;
int n;
{
  int i, j;

  for (i=1;i<=n;i++) {
    printf2("[ ");
    for (j=1;j<=n;j++)
      printf2("% 3g ", E[i][j]);
    printf2(" | % 3g ", E[i][n+1]);
    printf2("]\n    ");
  }
  printf2("\n");

}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
