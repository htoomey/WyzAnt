/******************************************************************************
	Method of False Position (or Regula Falsi) - Algorithm 2.4B
*******************************************************************************

To find a solution to f(x) = 0 given the continuous function f on the
interval [a,b] where f(a) and f(b) have opposite signs (+/- or -/+).  This
method is really the Bisection Method combined with the Secant Method:

(This algorithm's order of convergence is 1 < à < 1.62.)

INPUT endpoints a, b; tolerance TOL; maximum number of iterations N0; the
function f().

OUTPUT approximate solution p or message of failure.

NOTE: This algorithm was included as a "Homework Helper."  See p. 55,
Exercise Set 2.3, Problems 16-17, 31 b).

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "024b.out";	/* Customized default output file name.     */
char *eq_text_f = "f(x) = x^3 + 4x^2 - 10";	/* Needs updating  $  */


/*****************************************************************************/
/* f(x) - Function to evaluate, f(x).  Needs updating $.                     */
/*****************************************************************************/
double f(x)
double x;
{
  if (eqeval)
    return (eval_eq(x));		/* Use the Equation Evaluator  */
  else
    return (x*x*x + 4.0*x*x - 10.0);	/* Use the default function.   */
}
/*****************************************************************************/


main()
{
  double a, b, p, pold, m, val, f(), TOL;
  int i, N0;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Method of False Position (or Regula Falsi) - Algorithm 2.4B\n\n");

  if (eqeval)
    printf2("f(x) = %s", tmpstr);	/* Print the Equation Evaluator text */
  else
    printf2("%s", eq_text_f);		/* Print the default equation text */
  printf2("\n\n");

  printf("Enter maximum number of iterations, N0: ");
  scanf("%d", &N0);
  fprintf(file_id, "Maximum number of iterations: %d\n", N0);

  printf("Enter Tolerance TOL (1.0e-5)?: ");
  scanf("%lf", &TOL);
  fprintf(file_id, "Tolerance: %lg\n", TOL);

  printf("Enter endpoint a: ");
  scanf("%lf", &a);
  fprintf(file_id, "Endpoint a = %lg\n", a);

  printf("Enter endpoint b: ");
  scanf("%lf", &b);
  fprintf(file_id, "Endpoint b = %lg\n", b);

  printf2("\n n\t a\t\t b\t\t p\t\t f(p)");    /* Print table header. */
  printf2("\n-----------------------------------");
  printf2("------------------------------------\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  i = 1;
  p = f(a);			/* A good starting value for p. */

  /* STEP #2 */
  while (i <= N0) {		/* Do Steps 3-6. */

    /*
    **  DERIVATION OF THE SOLUTION EQUATION:
    **
    **  y - y0 = m(x - x0)		General Equation of a line.
    **  m = (f(bi) - f(ai)) / (bi - ai)	Equation for slope between points.
    **  y = m(x-x0) + y0 = 0		Solve x for the x-intercept.
    **  y0 = -m(x-x0)
    **  x = x0 - y0/m
    **  Let x0 = ai and y0 = f(ai).	Make these assumptions.
    **  pi = ai - f(ai)/m		Use this equation.
    */

    /* STEP #3 */
    pold = p;				/* Save old p.        */
    m    = (f(b) - f(a)) / (b - a);	/* Compute the slope. */
    p    = a - f(a)/m;			/* compute p(i).      */

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #4 */
    val = f(p);				/* For speed.         */
    printf2("% d\t% .9lf\t% .9lf\t% .9lf\t% .9lf\n", i, a, b, p, val);

    if ((val == 0.0) || (fabs(p - pold) < TOL)) {
      NAA_do_last(outfile);	/* NAA finish-up procedure.          */
      exit (1);			/* Procedure completed successfully. */
    }

    /* STEP #5 */
    i++;			/* i = i + 1.        */

    /* STEP #6 */
    if (val*f(a) < 0.0)  b = p;	/* Compute b(i). */
    else                 a = p;	/* Compute a(i). */
  }

  /* STEP #7 */			/* Procedure completed unsuccessfully. */
  printf2("\nMethod failed after N0 iterations, N0 = %d\n", N0);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
