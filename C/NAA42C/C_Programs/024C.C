/******************************************************************************
		Modified Newton-Raphson Method - Algorithm 2.4C
*******************************************************************************

To find a solution to f(x) = 0 given an initial approximation p0.  Ideal for
functions having roots of multiplicity m ò 1: (Uses Eq. (2.26))

(This algorithm's order of convergence is à = 2.)

INPUT initial approximation p0; tolerance TOL; maximum number of iterations N0;
the functions f(), fprime() = df()/dx, and f2prime() = d(df()/dx)/dx.

OUTPUT approximate solution p or message of failure.

NOTE: This algorithm was included as a "Homework Helper."  See p. 66,
Exercise Set 2.4, Problem 2.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities.  */

char *outfile         = "024c.out";  /* Customized default output file name. */
char *eq_text_f       = "f(x) = exp(x) - x - 1";/* Needs updating  $  */
char *eq_text_fprime  = "f(x) = exp(x) - 1";	/* Needs updating  $  */
char *eq_text_f2prime = "f(x) = exp(x)";	/* Needs updating  $  */


/*****************************************************************************/
/* f(x) - Function to evaluate, f(x).  Needs updating $.                     */
/*****************************************************************************/
double f(x)
double x;
{
  return (exp(x) - x - 1.0);
}

/*****************************************************************************/
/* fprime(x) - Derivative of function to evaluate, f'(x).  Needs updating $. */
/*****************************************************************************/
double fprime(x)
double x;
{
  return (exp(x) - 1.0);
}

/*****************************************************************************/
/* f2prime(x) - 2nd derivative of function to evaluate, f''(x).              */
/*              Needs updating $.                                            */
/*****************************************************************************/
double f2prime(x)
double x;
{
  return (exp(x));
}
/*****************************************************************************/


main()
{
  double p, p0, TOL, tmp1, tmp2, f(), fprime(), f2prime();
  int i, N0;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Modified Newton-Raphson Method - Algorithm 2.4C\n\n");
  printf2("%s\n", eq_text_f);
  printf2("%s\n", eq_text_fprime);
  printf2("%s\n\n", eq_text_f2prime);

  printf("Enter maximum number of iterations, N0: ");
  scanf("%d", &N0);
  fprintf(file_id, "Maximum number of iterations: %d\n", N0);

  printf("Enter Tolerance TOL (1.0e-9)?: ");
  scanf("%lf", &TOL);
  fprintf(file_id, "Tolerance: %lg\n", TOL);

  printf("Enter initial approximation p0: ");
  scanf("%lf", &p0);

  printf2("\n n\t  p");    /* Print table header. */
  printf2("\n-----------------------\n");
  printf2("% d\t% .11lf\n", 0, p0);

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  i = 1;

  /* STEP #2 */
  while (i <= N0) {		/* Do Steps 3-6. */

    /* STEP #3 */
    tmp1 = f(p0);		/* For speed.  Saves 1 function evaluation.  */
    tmp2 = fprime(p0);		/* Eq. 2.26.   Saves 2 function evaluations. */
				/* Compute p(i).                             */
    p    = p0 - (tmp1*tmp2) / (tmp2*tmp2 - tmp1*f2prime(p0));

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #4 */
    printf2("% d\t% .11lf\n", i, p);
    if (fabs(p - p0) < TOL) {
      NAA_do_last(outfile);	/* NAA finish-up procedure.                 */
      exit (1);			/* STOP - Procedure completed successfully. */
    }

    /* STEP #5 */
    i++;			/* i = i + 1.   */

    /* STEP #6 */
    p0 = p;			/* Update p(0). */
  }

  /* STEP #7 */			/* Procedure completed unsuccessfully. */
  printf2("\nMethod failed after N0 iterations, N0 = %d\n", N0);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
