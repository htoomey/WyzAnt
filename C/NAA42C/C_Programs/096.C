/******************************************************************************
			QR Algorithm - Algorithm 9.6
*******************************************************************************

To obtain the eigenvalues of the symmetric, tridiagonal n x n matrix

	       [ [ a(1) b(2)   0    0    0    0   ]
		 [ b(2) a(2)   .    0    0    0   ]
		 [  0     .    .    .    0    0   ]
      A ð A(1) ð [  0     0    .    .    .    0   ]
		 [  0     0    0    .    .   b(n) ]
		 [  0     0    0    0  b(n)  a(n) ] ]

INPUT dimension n; diagonal elements a(1),..., a(n); off-diagonal elements
b(2),..., b(n); tolerance TOL; maximum number of iterations M.

OUTPUT eigenvalues of A, or recommended splitting of A, or a message that the
maximum number of iterations was exceeded.

NOTE: Newly introduced into the 4th edition of the text.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "096.out";	/* Customized default output file name.     */
int n;				/* Number of equations and unknowns.        */


main()
{
  double *A, *B, TOL, *C, *D, *Q, *R, *S, *X, *Y, *Z;
  double lambda, mu1, mu2, b, c, d, s, SHIFT;
  int i, j, k, M;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("QR Algorithm - Algorithm 9.6\n\n");

  printf("Enter maximum number of iterations, M: ");
  scanf("%d", &M);
  fprintf(file_id, "Maximum number of iterations: %d\n", M);

  printf("Enter Tolerance TOL (1.0e-5)?: ");
  scanf("%lf", &TOL);
  fprintf(file_id, "Tolerance: %lg\n", TOL);

  do {
    printf("Enter the dimension, n, of the matrix A: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greter than zero.\n");
  } while (n <= 0);
  fprintf(file_id, "System has %d equations and %d unknowns.\n\n", n, n);

  printf("\t       [ [ a(1) b(2)   0    0    0    0   ]\n");
  printf("\t         [ b(2) a(2)   .    0    0    0   ]\n");
  printf("\t         [  0     .    .    .    0    0   ]\n");
  printf("\t     A ð [  0     0    .    .    .    0   ]\n");
  printf("\t         [  0     0    0    .    .   b(n) ]\n");
  printf("\t         [  0     0    0    0  b(n)  a(n) ] ]\n\n");

  /* Dynamically allocate memory for the needed arrays. */
  A = dvector(1,n);		/* Diagonal Vector A     */
  B = dvector(2,n);		/* Off-diagonal Vector B */

  C = dvector(1,n);		/* For STEPS #6-#8       */
  D = dvector(1,n);
  Q = dvector(1,n);
  R = dvector(1,n);
  S = dvector(1,n);
  X = dvector(1,n);
  Y = dvector(1,n);
  Z = dvector(1,n);

  printf("Enter the coefficients for Diagonal Vector A:\n");
  for (i=1;i<=n;i++) {					/* Get A. */
    printf("\tA[%d] = ", i);
    scanf("%lf", &A[i]);
  }

  fprintf(file_id, "The coefficients for Diagonal Vector A:\n");
  fprintf(file_id, "\tA = [ ");				/* Print A. */
  for (i=1;i<=n;i++)
    fprintf(file_id, "% 3lg ", A[i]);
  fprintf(file_id, "]t\n\n");

  printf("Enter the coefficients for Off-diagonal Vector B:\n");
  for (i=2;i<=n;i++) {					/* get B. */
    printf("\tB[%d] = ", i);
    scanf("%lf", &B[i]);
  }

  fprintf(file_id, "The coefficients for Off-diagonal Vector B:\n");
  fprintf(file_id, "\tB = [ ");				/* Print B. */
  for (i=2;i<=n;i++)
    fprintf(file_id, "% 3lg ", B[i]);
  fprintf(file_id, "]t\n\n");

  printf2("A(1) = ");		/* Print the n x n matrix. */
  print_matrix_A(A, B, n);

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  k     = 1;
  SHIFT = 0.0;			/* Accumulated shift. */

  /* STEP #2 */
  while (k <= M) {		/* Do Steps 3-8.      */

    /* STEP #3 */		/* Test for success.  */
    if (fabs(B[n]) <= TOL) {
      lambda = A[n] + SHIFT;
      printf2("Eigenvalue:\t% .11lf\n", lambda);
      n--;			/* n = n - 1.         */
    }

    for (j=3;j<n;j++)
      if (fabs(B[j]) <= TOL) { 
        printf2("Split into a(1),...,a(%d), b(2),...,b(%d) and\n", j-1, j-1);
        printf2("a(%d),...,a(%d), b(%d),...,b(%d).\n", j, n, j+1, n);
        printf2("SHIFT = %lf\n", SHIFT);
        NAA_do_last(outfile);	/* NAA finish-up procedure. */
        naaerror("Needs splitting into two sub-matrices.");	/* STOP */
      }

    if (fabs(B[2]) <= TOL) {
      lambda = A[1] + SHIFT;
      printf2("Eigenvalue:\t% .11lf\n", lambda);
      n--;			/* n = n - 1. */
      A[1] = A[2];
      for (j=2;j<=n;j++) {
        A[j] = A[j+1];
        B[j] = B[j+1];
      }
    }

    /* STEP #4 */		/* Compute shift. */
    b = -(A[n-1] + A[n]);
    c = A[n] * A[n-1] - B[n] * B[n];
    d = sqrt(b*b - 4.0*c);

    if (b > 0.0) {
      mu1 = -2.0 * c / (b + d);
      mu2 = -(b + d) / 2.0;
    } else {
      mu1 = (d - b) / 2.0;
      mu2 = 2.0 * c / (d - b);
    }

    /***********
     * OUTPUTS *
     ***********/

    if (n == 2) {
      lambda = mu1 + SHIFT;	/* lambda1 */
      printf2("Eigenvalue:\t% .11lf\n", lambda);
      lambda = mu2 + SHIFT;	/* lambda2 */
      printf2("Eigenvalue:\t% .11lf\n", lambda);
      /* Free the memory that was dynamically allocated for the arrays. */
      free_dvector(Z,1,n);
      free_dvector(Y,1,n);
      free_dvector(X,1,n);
      free_dvector(S,1,n);
      free_dvector(R,1,n);
      free_dvector(Q,1,n);
      free_dvector(D,1,n);
      free_dvector(C,1,n);
      free_dvector(B,2,n);
      free_dvector(A,1,n);

      NAA_do_last(outfile);	/* NAA finish-up procedure. */
      exit (1);			/* STOP */
    }

    /* Chooses s so that |s - A[n]| = min(|mu1 - A[n]|, |mu2 - A[n]|). */
    s = (fabs(mu1 - A[n]) < fabs(mu2 - A[n])) ? mu1 : mu2;

    /* STEP #5 */		/* Accumulate shift. */
    SHIFT += s;

    /* STEP #6 */		/* Perform shift.    */
    for (j=1;j<=n;j++)
      D[j] = A[j] - s;

    /* STEP #7 */		/* Compute R.        */
    X[1] = D[1];
    Y[1] = B[2];
    for (j=2;j<=n;j++) {
      Z[j-1] = sqrt(X[j-1]*X[j-1] + B[j]*B[j]);
      C[j]   = X[j-1] / Z[j-1];
      S[j]   = B[j] / Z[j-1];
      Q[j-1] = C[j]*Y[j-1] + S[j]*D[j];
      X[j]   = -S[j]*Y[j-1] + C[j]*D[j];
      if (j != n) {
        R[j-1] = S[j]*B[j+1];
        Y[j] = C[j]*B[j+1];
      }
    /* (A[j] = P[j]*A[j-1] has just been computed and R = A[n].) */
    }

    /* STEP #8 */		/* Compute next iteration of A. */
    Z[n] = X[n];
    A[1] = S[2]*Q[1] + C[2]*Z[1];
    B[2] = S[2]*Z[2];

    for (j=2;j<n;j++) {
      A[j] = S[j+1]*Q[j] + C[j]*C[j+1]*Z[j];
      B[j+1] = S[j+1]*Z[j+1];
    }

    A[n] = C[n]*Z[n];
    k++;			/* k = k + 1. */

    printf2("A(%d) = ", k);	/* Print the next iteration of A. */
    print_matrix_A(A, B, n);

  }

  /* STEP #9 */		/* Procedure completed unsuccessfully. */
  printf2("Maximum number of iterations (%d) exceeded.\n", M);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(Z,1,n);
  free_dvector(Y,1,n);
  free_dvector(X,1,n);
  free_dvector(S,1,n);
  free_dvector(R,1,n);
  free_dvector(Q,1,n);
  free_dvector(D,1,n);
  free_dvector(C,1,n);
  free_dvector(B,2,n);
  free_dvector(A,1,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* print_matrix_A() - Prints a symmetric, tridiagonal matrix given           */
/*                    A = a1,..,an and B = b2,..,bn vectors.                 */
/*****************************************************************************/
print_matrix_A(A, B, n)
double *A, *B;
int n;
{
  double **AA;
  int i, j;

  /* Dynamically allocate memory for the needed arrays. */
  AA = dmatrix(1,n,1,n);	/* Whole Matrix with A and B      */

  for (i=1;i<=n;i++)		/* Take A and B to make n x n AA. */
    for (j=1;j<=n;j++) {
      if (i == j)
        AA[i][j] = A[i];	/* Diagonal.     */
      else
        AA[i][j] = 0.0;		/* Other.        */
    }
  for (i=1;i<n;i++) {
    AA[i+1][i] = B[i+1];	/* Off diagonal. */
    AA[i][i+1] = B[i+1];	/* Off diagonal. */
  }

  for (i=1;i<=n;i++) {
    printf2("[ ");
    for (j=1;j<=n;j++)
      printf2("% .9lf  ", AA[i][j]);
    printf2("]\n       ");
  }
  printf2("\n");

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dmatrix(AA,1,n,1,n);

}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
