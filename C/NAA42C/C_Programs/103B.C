/******************************************************************************
	     Steepest Descent Method for Systems - Algorithm 10.3B
			   (with G(x) and gradG(x))
*******************************************************************************

To approximate a solution P to the minimization problem g(P) = min g(X) given
an initial approximation X:

INPUT number n of variables; initial approximation X = (x1,...,xn)t, tolerance
TOL; maximum number of iterations N; equation for G(X) in function G(), and
gradient of G(X) in grad_G().

OUTPUT approximate solution X = (x1,...,xn)t or a message that the number of
iterations was exceeded.

NOTE: Modifications to the algorithm were introduced into the 4th edition of
the text.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "103b.out";	/* Customized default output file name.     */
int n;				/* Dimension of n x n matrix.               */


/*****************************************************************************/
/* G(x) - Function to evaluate, X = G(X).  Needs updating $.                 */
/*****************************************************************************/
double G(X)
double *X;
{
  return (pow(X[1],4.0) + 2.0*pow(X[2], 4.0) + 3.0*pow(X[3], 4.0) + 1.01);
}

/*****************************************************************************/
/* grad_G(i,X) - Gradient of G() to evaluate.  Index i selects function      */
/*               number.  Needs updating $.                                  */
/*****************************************************************************/
double grad_G(i,X)
double *X;
int i;
{
  switch (i) {
    case 1: return (4.0*pow(X[1], 3.0));
    case 2: return (8.0*pow(X[2], 3.0));
    case 3: return (12.0*pow(X[3], 3.0));
    default: naaerror("Oops!  You forgot to modify grad_G().");
  }
}
/*****************************************************************************/


main()
{
  double *X, *Z, TOL, G(), grad_G(), euclidean_norm(), find_G();
  double g, g0, g1, g2, g3, a, a0, a1, a2, a3, h1, h2, h3, z0;
  int i, k, N;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Steepest Descent Method for Systems - Algorithm 10.3B\n\n");

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
  X = dvector(1,n);
  Z = dvector(1,n);

  printf("Enter initial approximations to X:\n");
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
  printf2("% .7lE\n", G(X));

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  k = 1;

  /* STEP #2 */
  while (k <= N) {		/* Do Steps 3-15. */

    /* STEP #3 */
    g1 = G(X);			/* Note: g1 = g(X(k)).     */
    for (i=1;i<=n;i++)
      Z[i] = grad_G(i,X);	/* Note: Z = grad_g(X(k)). */
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

      NAA_do_last(outfile);	/* NAA finish-up procedure. */
      exit (1);			/* STOP */
      /* Procedure completed, may have a minimum. */
    }

    /* STEP #5 */
    for (i=1;i<=n;i++)		/* Make Z a unit vector */
      Z[i] /= z0;		/* Z[i] = Z[i] / z0. */
    a1 = 0.0;
    a3 = 1.0;
    g3 = find_G(X,Z,a3);	/* Evaluates G(X - aZ)  */

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
    h1 = (g2 - g1) / a2;	/* Note: Quadratic P(a) = g1 + h1*a +     */
    h2 = (g3 - g2) / (a3 - a2);	/*   h3*a*(a - a2) interpolates h(a) at   */
    h3 = (h2 - h1) / a3;	/*   a = 0, a = a2, and a = a3.           */

    /* STEP #11 */
    a0 = 0.5*(a2 - h1/h3);	/* Critical point of P occurs at a0.       */
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
  a = G(TEMP);

  free_dvector(TEMP,1,n);

  return (a);
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
