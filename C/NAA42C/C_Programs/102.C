/******************************************************************************
		Broyden's Method for Systems - Algorithm 10.2
*******************************************************************************

To approximate the solution of the nonlinear system F(X) = 0 given an initial
approximation X:

INPUT number n of equations and unknowns; initial approximation
X = (x1,...,xn)t, tolerance TOL; maximum number of iterations N, equations
for J(X) in function jacobian(), and F(X) in f().

OUTPUT approximate solution X = (x1,...,xn)t or a message that the number of
iterations was exceeded.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */
#include "gaussj.c"		/* Guass-Jordan method (inverts a matrix)   */

char *outfile = "102.out";	/* Customized default output file name.     */
int n;				/* Dimension of n x n matrix.               */


/*****************************************************************************/
/* f(i,X) - Function vector, f(i,X) to evaluate.  Index i selects function   */
/*          number.  Needs updating $.                                       */
/*****************************************************************************/
double f(i,X)
int i;
double *X;
{
  switch (i) {
    case 1: return (3.0*X[1] - cos(X[2]*X[3]) - 0.5);
    case 2: return (X[1]*X[1] - 81.0*(X[2]+0.1)*(X[2]+0.1) + sin(X[3]) + 1.06);
    case 3: return (exp(-X[1]*X[2]) + 20.0*X[3] + (10.0*PI - 3.0)/3.0);
    default: naaerror("Oops!  You forgot to modify f().");
  }
}

/*****************************************************************************/
/* jacobian(i,j,X) - Jacobian matrix, jacobian(i,j,X) to evaluate.  Index i  */
/*                   and j selects function number.  Needs updating $.       */
/*****************************************************************************/
double jacobian(i,j,X)
int i, j;
double *X;
{
  switch (j) {
    case 1:
      switch (i) {
        case 1: return (3.0);
        case 2: return (2.0*X[1]);
        case 3: return (-X[2]*exp(-X[1]*X[2]));
        default: naaerror("Oops!  You forgot to modify jacobian().");
      }

    case 2:
      switch (i) {
        case 1: return (X[3]*sin(X[2]*X[3]));
        case 2: return (-162.0*(X[2]+0.1));
        case 3: return (-X[1]*exp(-X[1]*X[2]));
      }

    case 3:
      switch (i) {
        case 1: return (X[2]*sin(X[2]*X[3]));
        case 2: return (cos(X[3]));
        case 3: return (20.0);
      }
  }
}
/*****************************************************************************/


main()
{
  double **A, **C, *S, *V, *W, *X, *Y, *Z;
  double p, CA, TOL, jacobian(), infinite_norm(), f();
  int i, j, k, m, N;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Broyden's Method for Systems - Algorithm 10.2\n\n");

  printf("Enter maximum number of iterations, N: ");
  scanf("%d", &N);
  fprintf(file_id, "Maximum number of iterations: %d\n", N);

  printf("Enter Tolerance TOL (1.0e-5)?: ");
  scanf("%lf", &TOL);
  fprintf(file_id, "Tolerance: %lg\n", TOL);

  do {
    printf("Enter number of equations, n: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greter than zero.\n");
  } while (n <= 0);
  fprintf(file_id, "System has %d equations and %d unknowns.\n\n", n, n);

  /* Dynamically allocate memory for the needed arrays. */
  A = dmatrix(1,n,1,n);
  C = dmatrix(1,n,1,n);
  S = dvector(1,n);
  V = dvector(1,n);
  W = dvector(1,n);
  X = dvector(1,n);
  Y = dvector(1,n);
  Z = dvector(1,n);

  printf("Enter initial approximations to X:\n");
  for (i=1;i<=n;i++) {
    printf("X0[%d] =  ", i);
    scanf("%lf", &X[i]);
  }

  printf2("\n k\t");		/* Print table header. */
  for (i=1;i<=n;i++)
    printf2("  X[%d]\t\t", i);
  printf2("\n---------");
  for (i=1;i<=n;i++)
    printf2("---------------");
  printf2("\n");

  k = 0;
  printf2("% d\t", k);		/* Output initial conditions. */
  for (i=1;i<=n;i++)
    printf2("% .9lf\t", X[i]);
  printf2("\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */			/* Load matrix A with Jacobian. */
  for (i=1;i<=n;i++) {
    for (j=1;j<=n;j++)
      A[i][j] = jacobian(i,j,X);
    V[i] = f(i,X);		/* Note: V = F(X(0)).           */
  }

  /* STEP #2 */

  /*
  **  Computes inverse of A using Gauss-Jordan method.  Uses C as a dummy
  **  vector here.  From here on A is really A_inverse.
  */

  gaussj(A,n,C,1);

  /* STEP #3 */
  k = 1;
  m_mult(A,V,S);		/* Note: S = S1.   */
  for (i=1;i<=n;i++)		/* Note: X = X(1). */
    X[i] += S[i];
  printf2("% d\t", k);
  for (i=1;i<=n;i++)
    printf2("% .9lf\t", X[i]);
  printf2("\n");
 
  /* STEP #4 */
  k++;		/* k = k + 1. *//* Note: This step is left out of the text. */
  while (k <= N) {		/* Do Steps 5-13.                           */

    /* STEP #5 */
    for (i=1;i<=n;i++) {
      W[i] = V[i];		/* Save V.            */
      V[i] = f(i,X);		/* Note: V = F(X(k)). */
      Y[i] = V[i] - W[i];	/* Note: Y = Yk.      */
    }

    /* STEP #6 */
    m_mult(A,Y,Z);		/* Note: Z = -A_inverse(k-1)*Yk.   */

    /* STEP #7 */
    p = 0.0;			/* Note p = Skt*A_inverse(k-1)*Yk. */
    for (i=1;i<=n;i++)
      p -= S[i] * Z[i];

    /* STEP #8 */
    for (i=1;i<=n;i++)		/* C = pI + (S + Z)*St             */
      for (j=1;j<=n;j++)
        if (i == j)
          C[i][j] = p + (S[i] + Z[i]) * S[j];
        else
          C[i][j] = (S[i] + Z[i]) * S[j];

    /* STEP #9 */
    for (i=1;i<=n;i++)		/* Note: A = A_inverse(k).          */
      for (j=1;j<=n;j++) {
        CA = 0.0;
        for (m=1;m<=n;m++)		
          CA += C[i][m] * A[m][j];
        A[i][j] = CA / p;
      }

    /* STEP #10 */
    m_mult(A,V,S);		/* Note: S = -A_inverse(k)*F(X(k)). */

    /* STEP #11 */
    for (i=1;i<=n;i++)		/* Note: X = X(k+1)                 */
      X[i] += S[i];

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #12 */
    printf2("% d\t", k);
    for (i=1;i<=n;i++)
      printf2("% .9lf\t", X[i]);
    printf2("\n");

    if (infinite_norm(S) < TOL) {
      /* Free the memory that was dynamically allocated for the arrays. */
      free_dvector(Z,1,n);
      free_dvector(Y,1,n);
      free_dvector(X,1,n);
      free_dvector(W,1,n);
      free_dvector(V,1,n);
      free_dvector(S,1,n);
      free_dmatrix(C,1,n,1,n);
      free_dmatrix(A,1,n,1,n);

      NAA_do_last(outfile);	/* NAA finish-up procedure.                 */
      exit (1);			/* STOP - Procedure completed successfully. */
    }

    /* STEP #13 */
    k++;			/* k = k + 1. */

  }

  /* STEP #14 */		/* Procedure completed unsuccessfully. */
  printf2("\nMaximum number of iterations (%d) exceeded.\n", N);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(Z,1,n);
  free_dvector(Y,1,n);
  free_dvector(X,1,n);
  free_dvector(W,1,n);
  free_dvector(V,1,n);
  free_dvector(S,1,n);
  free_dmatrix(C,1,n,1,n);
  free_dmatrix(A,1,n,1,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* infinite_norm() - Computes the infinite norm of a vector.                 */
/*****************************************************************************/
double infinite_norm(X)
double *X;
{
  double max = 0.0;
  int i;

  for (i=1;i<=n;i++)
    if (fabs(X[i]) > max)
      max = fabs(X[i]);

  return (max);
}

/***************************************************************************/
/* m_mult() - (n x n) X (n x 1) matrix multiplication tailored for this    */
/*            algorithm.                                                   */
/***************************************************************************/
m_mult(X,Y,Z)
double **X, *Y, *Z;
{
  int i, j;

  for (i=1;i<=n;i++) {
    Z[i]=0.0;
    for (j=1;j<=n;j++)
      Z[i] -= X[i][j] * Y[j];		/* Z = -X * Y */
  }
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
