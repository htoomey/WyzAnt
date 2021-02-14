/******************************************************************************
			Iterative Refinement - Algorithm 7.4B
		     (For single-precision computer arithmetic.)
*******************************************************************************

To approximate the solution to the linear system AX = B when A is suspected
to be ill conditioned:

INPUT the number of equations and unknowns n; the entries a(i,j),
1 ó i,j ó n of the matrix A; the entries b(i), 1 ó i ó n of B;
maximum number of iterations N; tolerance TOL.

OUTPUT the approximate solution XX = (xx1,...,xxn)t or a message that the
number of iterations was exceeded.

NOTE: This algorithm was included as a "Homework Helper."  See p. 422,
Exercise Set 7.4, Problems 1 d), 7 c).

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "074b.out";	/* Customized default output file name.     */
int n;


main()
{
  double dtemp, *TEMP, infinite_norm();
  float **A, *B, *R, *X, *XX, *Y, TOL;
  int i, j, k, N;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Iterative Refinement - Algorithm 7.4B\n\n");

  printf("Enter maximum number of iterations, N: ");
  scanf("%d", &N);
  fprintf(file_id, "Maximum number of iterations: %d\n", N);

  printf("Enter the tolerance, TOL (1.0e-8): ");
  scanf("%f", &TOL);
  fprintf(file_id, "Tolerance = %g\n", TOL);

  do {
    printf("Enter the dimension, n, of the matrix A: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greter than zero.\n");
  } while (n <= 0);
  fprintf(file_id, "System has %d equations and %d unknowns.\n\n", n, n);

  /* Dynamically allocate memory for the needed arrays. */
  A    = matrix(1,n,1,n);	/* Matrix A                      */
  B    = vector(1,n);		/* Matrix B                      */
  R    = vector(1,n);		/* residual vector               */
  X    = vector(1,n);		/* solution vector               */
  XX   = vector(1,n);		/* more accurate solution vector */
  Y    = vector(1,n);		/* error vector                  */
  TEMP = dvector(1,n);		/* Temporary matrix for error    */

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
      fprintf(file_id, "% 3G ", A[i][j]);
    fprintf(file_id, "]\n    ");
  }
  fprintf(file_id, "\n");

  printf("Enter the coefficients for matrix B:\n");	/* Get B. */
  for (i=1;i<=n;i++) {
    printf("\tB[%d] = ", i);
    scanf("%f", &B[i]);
  }
  printf("\n");

  fprintf(file_id, "B = [ ");	/* Print Vector B to file. */
  for (i=1;i<=n;i++)
    fprintf(file_id, "% 3G ", B[i]);
  fprintf(file_id, "]t\n\n");

  /* STEP #0 */

  /*
  **  Solve the system AX = B for x1,...,xn by Gaussian elimination saving the
  **  multipliers m(j,i), j = i+1, i+2,...,n, i = 1,2,...,n-1 and noting row
  **  interchanges.  A and B do not get altered.
  */

  gauss(A,X,B,n);

  printf2("X(%d) = [ ", 0);	/* Print solution XX. */
  for (i=1;i<=n;i++)
    printf2("% 7.7G ", X[i]);
  printf2("]t\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  k = 1;

  /* STEP #2 */
  while (k <= N) {		/* Do Steps 3-8. */

    /* STEP #3 */
    for (i=1;i<=n;i++) {	/* Compute residual R. */
      dtemp = (double) B[i];
      for (j=1;j<=n;j++)
        dtemp -= (double) A[i][j] * (double) X[j];
      R[i] = (float) dtemp;
      /* Performed in double-precision arithmetic. */
    }

    printf2("Residual = [");	/* Print out residual vector. */
    for (i=1;i<=n;i++)
      printf2("% .4E ", R[i]);
    printf2("]t\n\n");

    /* STEP #4 */

    /*
    **  Solve the linear system AY = R by using Gaussian elimination in the
    **  same order as in step 0.  A and R do not get altered.
    */

    gauss(A,Y,R,n);

    /* STEP #5 */
    for (i=1;i<=n;i++)
      XX[i] = X[i] + Y[i];

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #6 */
    printf2("X(%d) = [ ", k);	/* Print solution XX. */
    for (i=1;i<=n;i++)
      printf2("% 7.7G ", XX[i]);
    printf2("]t\n");

    for (i=1;i<=n;i++)		/* Check for requested accuracy. */
      TEMP[i] = (double) X[i] - (double) XX[i];
    if (fabs(infinite_norm(TEMP)) < TOL) {
      /* Free the memory that was dynamically allocated for the arrays. */
      free_dvector(TEMP,1,n);
      free_vector(Y,1,n);
      free_vector(XX,1,n);
      free_vector(X,1,n);
      free_vector(R,1,n);
      free_vector(B,1,n);
      free_matrix(A,1,n,1,n);
      NAA_do_last(outfile);	/* NAA finish-up procedure. */
      exit (1);			/* STOP - Procedure completed successfully. */
    }

    /* STEP #7 */
    k++;			/* k = k + 1. */

    /* STEP #8 */
    for (i=1;i<=n;i++)		/* Update old solution, X. */
      X[i] = XX[i];
  }

  /* STEP #9 */			/* Procedure completed unsuccessfully. */
  printf2("\nMaximum number of iterations (%d) exceeded.\n", N);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(TEMP,1,n);
  free_vector(Y,1,n);
  free_vector(XX,1,n);
  free_vector(X,1,n);
  free_vector(R,1,n);
  free_vector(B,1,n);
  free_matrix(A,1,n,1,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/****************************************************************************/
/* infinite_norm() - Computes the infinite norm of a vector.                */
/****************************************************************************/
double infinite_norm(X)
double *X;
{
  double max = 0.0;
  int i;

  for (i=1;i<=n;i++)
    if (fabs(X[i]) > fabs(max))
      max = X[i];

  return (max);
}

/*****************************************************************************/
/* gauss() - Gaussian Elimination with Backward Substitution - Algorithm 6.1.*/
/*           Solves AX = B.                                                  */
/*****************************************************************************/
gauss(A,X,B,n)
float **A, *X, *B;
int n;
{
  float **E, **m, sum, temp, *vector(), **matrix();
  int i, j, k, p, FLAG, *ivector();

  /* Dynamically allocate memory for the needed arrays. */
  E = matrix(1,n,1,n+1);	/* Matrix A augmented with B. */
  m = matrix(1,n,1,n+1);	/* Matrix m.                  */

  for (i=1;i<=n;i++) {		/* Copy A and B into augmented matrix E. */
    E[i][n+1] = B[i];
    for (j=1;j<=n;j++)
      E[i][j] = A[i][j];
  }

  /* STEP #1 */
  for (i=1;i<n;i++) {		/* Elimination process. */

    /* STEP #2 */
    for (p=i;p<=n;p++) {
      if (E[p][i] != 0.0) {
        FLAG = TRUE;
        break;
      }
      FLAG = FALSE;		/* FLAG = FALSE indicates an all zero row. */
    }

    if (FLAG == 0) {
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
    for (j=i+1;j<=n;j++) {

      /* STEP #5 */
      m[j][i] = E[j][i] / E[i][i];

      /* STEP #6 */
      for (k=1;k<=n+1;k++)
        E[j][k] -= m[j][i] * E[i][k];
    }
  }

  /* STEP #7 */
  if (E[n][n] == 0.0) {
    printf2("No unique solution exists (2).\n");
    NAA_do_last(outfile);	/* NAA finish-up procedure. */
    exit (-1);
  }

  /* STEP #8 */
  X[n] = E[n][n+1] / E[n][n];	/* Start backward substitution. */

  /* STEP #9 */
  for (i=n-1;i>=1;i--) {
    sum = 0.0;
    for (j=i+1;j<=n;j++)
      sum += E[i][j] * X[j];
    X[i] = (E[i][n+1] - sum) / E[i][i];
  }

  /* Free the memory that was dynamically allocated for the arrays. */
  free_matrix(m,1,n,1,n+1);
  free_matrix(E,1,n,1,n+1);

}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
