/******************************************************************************
		    Gauss-Jordan Method - Algorithm 6.1C2
				(with rounding)
*******************************************************************************

To solve the n x n linear system

	E1:   a(1,1)*x1 + a(1,2)*x2 + ... a(1,n)*xn = a(1,n+1)
	E2:   a(2,1)*x1 + a(2,2)*x2 + ... a(2,n)*xn = a(2,n+1)
	.        .           .               .           .
	.        .           .               .           .
	.        .           .               .           .
	En:   a(n,1)*x1 + a(n,2)*x2 + ... a(n,n)*xn = a(n,n+1):

INPUT number of equations and unknowns n; augmented matrix A = (a(i,j)) where
1 � i � n and 1 � j � n + 1; the number of desired significant digits SIG.

OUTPUT solution x1,x2,...,xn or message that linear system has no unique
solution.

NOTE: This algorithm was included as a "Homework Helper."  See p. 325,
Exercise Set 6.2, Problem *** 7 ***.  For chopping arithmetic, replace EVERY
occurance of the word "round" with "trunc", including the "round.c" file.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */
#include "round.c"		/* Rounds X to SIG significant digits.      */

char *outfile = "061c2.out";	/* Customized default output file name.     */


main()
{
  float **A, **E, **m, *X, temp;
  int i, j, k, n, p, FLAG, SIG;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Gauss-Jordan Method - Algorithm 6.1C2\n\n");
  printf2("Solves A*X = B.\n\n");

  do {
    printf("Enter the number of significant digits, SIG (1-%d): ", DBL_DIG);
    scanf("%d", &SIG);
    if (SIG < 1 || SIG > DBL_DIG)	/* ERROR */
      printf("Enter 1 to %d only for number of significant digits.\n",
        DBL_DIG);
  } while (SIG < 1 || SIG > DBL_DIG);
  fprintf(file_id, "Computed with %d-digit rounding arithmetic.\n\n", SIG);

  do {
    printf("Enter the dimension, n, of the matrix A: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greater than zero.\n");
  } while (n <= 0);

  /* Dynamically allocate memory for the needed arrays. */
  E = matrix(1,n,1,n+1);	/* Matrix A augmented with B. */
  m = matrix(1,n,1,n+1);	/* Matrix m.                  */
  X = vector(1,n);		/* Solution vector.           */
  A = E;			/* Matrix A (a subset of E).  */

  printf("Enter the coefficients for matrix A:\n");	/* get A. */
  for (i=1;i<=n;i++)
    for (j=1;j<=n;j++) {
      printf("\tA[%d][%d] = ", i, j);
      scanf("%f", &A[i][j]);
      A[i][j] = round(A[i][j], SIG);
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
    E[i][n+1] = round(E[i][n+1], SIG);
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
  for (i=1;i<=n;i++) {		/* Do Steps 2-4.  Elimination process.  */
				/* Modified from Algorithm 6.1.         */
    printf2("At row %d:\n", i);
    printf2("E = ");
    print_matrix(E,n);		/* Show intermediate results for looks. */

    /* STEP #2 */
    for (p=i;p<=n;p++) {	/* Let p be the smallest integer with   */
      if (A[p][i] != 0.0) {	/* i � p � n and a(pi) != 0.            */
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
    if (p != i)			/* Swap E(p) with E(i). */
      for (k=1;k<=n+1;k++) {
        temp    = E[p][k];
        E[p][k] = E[i][k];
        E[i][k] = temp;
      }

    /* STEP #4 */
    for (j=1;j<=n;j++)		/* Do Steps 5-6. */

      if (j != i) {  		/* Modified from Algorithm 6.1. */

        /* STEP #5 */
        m[j][i] = round(A[j][i] / A[i][i], SIG);

        /* STEP #6 */
        for (k=1;k<=n+1;k++) {
          E[j][k] -= round(m[j][i] * E[i][k], SIG);
          E[j][k] = round(E[j][k], SIG);
        }
      }
  }

  /* STEP #7 */
  if (A[n][n] == 0.0) {
    printf2("No unique solution exists (2).\n");
    NAA_do_last(outfile);	/* NAA finish-up procedure. */
    exit (-1);
  }

  /***********
   * OUTPUTS *
   ***********/

  printf2("E = ");
  print_matrix(E,n);		/* Show final results. */

  /* STEPS #8-#9 */
  for (i=1;i<=n;i++)		/* Obtain solution.    */
    X[i] = round(E[i][n+1] / A[i][i], SIG);

  /* STEP #10 */
  printf2("\nX = [ ");		/* Procedure completed successfully. */
  for (i=1;i<=n;i++)
    printf2("% *g ", SIG, X[i]);
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
