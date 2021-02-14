/******************************************************************************
	     Steepest Descent Method for Systems - Algorithm 10.3
			     (with F(x) and J(x))
*******************************************************************************

To approximate a solution P to the minimization problem g(P) = min g(X) given
an initial approximation X:

INPUT number n of variables; initial approximation X = (x1,...,xn)t, tolerance
TOL; maximum number of iterations N; equations for J(X) in function
jacobian(), and F(X) in f().

OUTPUT approximate solution X = (x1,...,xn)t or a message that the number of
iterations was exceeded.

NOTE: Modifications to the algorithm were introduced into the 4th edition of
the text.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "103.out";	/* Customized default output file name.     */
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
int i, j;		/* Use (j,i) like:  (ith row, jth column) */
double *X;		/* (1,1) (2,1) (3,1)                      */
{			/* (1,2) (2,2) (3,2)                      */
  switch (j) {		/* (1,3) (2,3) (3,3)                      */
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
  double *F, **J, *X, *Z, TOL, f(), jacobian(), euclidean_norm(), find_G();
  double g, g0, g1, g2, g3, a, a0, a1, a2, a3, h1, h2, h3, z0;
  int i, j, k, N;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Steepest Descent Method for Systems - Algorithm 10.3\n\n");

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
  J = dmatrix(1,n,1,n);
  F = dvector(1,n);
  X = dvector(1,n);
  Z = dvector(1,n);

  printf("Enter initial approximations to X:\n");	/* Get X. */
  for (i=1;i<=n;i++) {
    printf("X0[%d] = ", i);
    scanf("%lf", &X[i]);
  }

  printf2("\n k\t");		/* Print table header. */
  for (i=1;i<=n;i++)
    printf2(" X[%d]\t\t", i);
  printf2(" G(X) = ä f(X)^2\n-------------");
  for (i=1;i<=n+1;i++)
    printf2("---------------");
  printf2("\n");

  k = 0;
  printf2("% d\t", k);		/* Output initial approximation to X. */
  for (i=1;i<=n;i++)
    printf2("% .9lf\t", X[i]);
  g1 = 0.0;
  for (i=1;i<=n;i++)
    g1 += pow(f(i, X), 2.0);
  printf2("% .7lE\n", g1);

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  k = 1;

  /* STEP #2 */
  while (k <= N) {		/* Do Steps 3-15. */

    /* STEP #3 */
    g1 = 0.0;
    for (i=1;i<=n;i++) {
      g1 += pow(f(i,X), 2.0);		/* Note: g1 = g(X(k)).   */
      for (j=1;j<=n;j++)
        J[i][j] = jacobian(i,j,X);	/* Load Jacobian Matrix. */
      F[i] = f(i,X);			/* Load Function Vector. */
    }
    for (i=1;i<=n;i++) {
      Z[i] = 0.0;
      for (j=1;j<=n;j++)		/* Note: Z = grad_g(X(k)). */
	Z[i] += 2.0 * J[j][i] * F[j];	/* Also grad_g = 2*Jt*F    */
    }
    z0 = euclidean_norm(Z);

    /* STEP #4 */
    if (z0 == 0.0) {
      printf2("**** zero gradient ****\n");
      printf2("% d\t", k);
      for (i=1;i<=n;i++)
        printf2("% .9lf\t", X[i]);
      printf2("% .7lE\n", g1);
      /* Free the memory that was dynamically allocated for the arrays. */
      free_dvector(Z,1,n);
      free_dvector(X,1,n);
      free_dvector(F,1,n);
      free_dmatrix(J,1,n,1,n);

      NAA_do_last(outfile);	/* NAA finish-up procedure. */
      exit (1);			/* STOP */
      /* Procedure completed, may have a minimum. */
    }

    /* STEP #5 */
    for (i=1;i<=n;i++)		/* Make Z a unit vector. */
      Z[i] /= z0;		/* Z[i] = Z[i] / z0.     */
    a1 = 0.0;
    a3 = 1.0;
    g3 = find_G(X,Z,a3);	/* Evaluates G(X - aZ).  */

    /* STEP #6 */
    while (fabs(g3) >= fabs(g1)) {	/* Do Steps 7-8. */

      /* STEP #7 */
      a3 /= 2.0;		/* a3 = a3 / 2. */
      g3 = find_G(X,Z,a3);

      /* STEP #8 */
      if (a3 < (TOL/2.0)) {
	printf2("**** no likely improvement ****\n");
        printf2(" %d\t", k);
        for (i=1;i<=n;i++)
          printf2("% .9lf\t", X[i]);
        printf2("% .7lE\n", g1);
        /* Free the memory that was dynamically allocated for the arrays. */
        free_dvector(Z,1,n);
        free_dvector(X,1,n);
        free_dvector(F,1,n);
        free_dmatrix(J,1,n,1,n);

        NAA_do_last(outfile);	/* NAA finish-up procedure. */
        exit (1);		/* STOP */
        /* Procedure completed, may have a minimum. */
      }
    }
	
    /* STEP #9 */
    a2 = a3 / 2.0;
    g2 = find_G(X,Z,a2);

    /*
    **  Note: Steps 10-12 are have been slightly modified from the 3rd
    **        edition of the text.  Steps 5 and 13 have also been corrected.
    */

    /* STEP #10 */
    h1 = (g2 - g1) / a2;	/* Note: Quadratic P(a) = g1 + h1*a +      */
    h2 = (g3 - g2) / (a3 - a2);	/*   h3*a*(a-a2) interpolates h(a) at a=0, */
    h3 = (h2 - h1) / a3;	/* a=a2, a=a3.                             */

    /* STEP #11 */
    a0 = 0.5*(a2 - h1/h3);	/* Critical point of P occurs at a0. */
    g0 = find_G(X,Z,a0);

    /* STEP #12 */
    g = (g0 < g3) ? (g0) : (g3); /* Finds 'a' from {a0, a3} so that */
    a = (g0 < g3) ? (a0) : (a3); /* g = g(X - a0*Z) = min(g0, g3).  */

    /* STEP #13 */
    for (i=1;i<=n;i++)
      X[i] -= a * Z[i];		/* Text error: 3rd edition had a "+". */

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #14 */
    printf2("% d\t", k);
    for (i=1;i<=n;i++)
      printf2("% .9lf\t", X[i]);
    printf2("% .7lE\n", g);

    if (fabs(g - g1) < TOL) {
      /* Free the memory that was dynamically allocated for the arrays. */
      free_dvector(Z,1,n);
      free_dvector(X,1,n);
      free_dvector(F,1,n);
      free_dmatrix(J,1,n,1,n);

      NAA_do_last(outfile);	/* NAA finish-up procedure.                 */
      exit (1);			/* STOP - Procedure completed successfully. */
    }

    /* STEP #15 */
    k++;			/* k = k + 1. */

  }

  /* STEP #16 */		/* Procedure completed unsuccessfully. */
  printf2("\nMaximum number of iterations (%d) exceeded.\n", N);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(Z,1,n);
  free_dvector(X,1,n);
  free_dvector(F,1,n);
  free_dmatrix(J,1,n,1,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/****************************************************************************/
/* euclidean_norm() - Computes the euclidean norm of a vector.              */
/*****************************************************************************/
double euclidean_norm(X)
double *X;
{
  double sum_of_sqrs = 0.0;
  int i;

  for (i=1;i<=n;i++)
    sum_of_sqrs += (X[i] * X[i]);

  return (sqrt(sum_of_sqrs));
}

/*****************************************************************************/
/* find_G() - Evaluates G(X - aZ)                                            */
/*****************************************************************************/
double find_G(X,Z,a)
double *X, *Z;
double a;
{
  int i;
  double *TEMP;

  TEMP = dvector(1,n);

  for (i=1;i<=n;i++)
    TEMP[i] = X[i] - a * Z[i];	/* Text error: 3rd edition had a "+". */
  a = 0.0;
  for (i=1;i<=n;i++)
    a += pow(f(i, TEMP), 2.0);

  free_dvector(TEMP,1,n);
  return (a);
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
