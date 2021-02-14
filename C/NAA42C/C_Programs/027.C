/******************************************************************************
			MÅller's Method - Algorithm 2.7
*******************************************************************************

To find a solution to f(x) = 0 given three approximations x0, x1 and x2:

(This algorithm's order of convergence is ‡ = 1.84.)

INPUT x0, x1, x2; tolerance TOL; maximum number of iterations N0; the function
f().

OUTPUT approximate solution p or message of failure.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */
#include "naautil3.c"		/* Numerical Analysis Algorithms Util. III. */

char *outfile   = "027.out";	/* Customized default output file name.     */
char *eq_text_f = "f(x) = 16.0*x^4 - 40.0*x^3 + 5.0*x^2 + 20.0*x + 6.0";
				/* Needs updating  $  */


/*****************************************************************************/
/* f(x) - Function to evaluate, f(x).  Needs updating $.                     */
/*****************************************************************************/
double f(x)
double x;
{
  if (eqeval)
    return (eval_eq(x));	/* Use the Equation Evaluator  */
  else				/* Use the default function.   */
    return ((((16.0*x - 40.0)*x + 5.0)*x + 20.0)*x + 6.0);
}

/*
**  f(x) = 16x^4 - 40x^3 + 5x^2 + 20x + 6 is written in nested format above.
**  This method is also called Horner's Method.  See p. 18 in the text for
**  another example and its benefits.
*/

/*****************************************************************************/


main()
{
  double p, x0, x1, x2, h, h1, h2, b, d, D, E, delta1, delta2, TOL, f();
  int i, N0;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("MÅller's Method - Algorithm 2.7\n\n");

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

  printf("Enter initial approximation x0: ");
  scanf("%lf", &x0);

  printf("Enter initial approximation x1: ");
  scanf("%lf", &x1);

  printf("Enter initial approximation x2: ");
  scanf("%lf", &x2);

  printf2("\n n\t  x\t\t  f(x)\n");	/* Print table header. */
  printf2("---------------------------------------\n");
  printf2("% d\t% .9lf\t% .6le\n", 0, x0, f(x0));
  printf2("% d\t% .9lf\t% .6le\n", 1, x1, f(x1));
  printf2("% d\t% .9lf\t% .6le\n", 2, x2, f(x2));

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  h1     = x1 - x0;
  h2     = x2 - x1;
  delta1 = (f(x1) - f(x0)) / h1;
  delta2 = (f(x2) - f(x1)) / h2;
  d      = (delta2 - delta1) / (h2 + h1);
  i      = 3;			/* (Text is in error.) */

  /* STEP #2 */
  while (i <= N0) {		/* Do Steps 3-7.       */

    /* STEP #3 */
    b = delta2 + h2*d;
    if ((b*b - 4.0*f(x2)*d) < 0.0) {	/* Check for complex numbers. */
      NAA_do_last(outfile);		/* NAA finish-up procedure.   */
      naaerror("Can not do complex arithmetic.  Use Algorithm 2.8A.");
    }

    D = sqrt(b*b - 4.0*f(x2)*d); /* Note: May be complex arithmatic. */

    /* STEP #4 */
    if (fabs(b - D) < fabs(b + D))
      E = b + D;		/* Third edition used d where D is now. */
    else
      E = b - D;		/* Third edition used d where D is now. */

    /* STEP #5 */
    h = -2.0*f(x2) / E;
    p = x2 + h;

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #6 */
    printf2("% d\t% .9lf\t% .6le\n", i, p, f(p));

    if (fabs(h) < TOL) {
      NAA_do_last(outfile);	/* NAA finish-up procedure.                 */
      exit (1);			/* STOP - Procedure completed successfully. */
    }

    /* STEP #7 */
    x0     = x1;		/* Prepare for next iteration.              */
    x1     = x2;
    x2     = p;
    h1     = x1 - x0;
    h2     = x2 - x1;
    delta1 = (f(x1) - f(x0)) / h1;
    delta2 = (f(x2) - f(x1)) / h2;
    d      = (delta2 - delta1) / (h2 + h1);
    i++;			/* i = i + 1. */
  }

  /* STEP #8 */			/* Procedure completed unsuccessfully. */
  printf2("\nMethod failed after N0 iterations, N0 = %d\n", N0);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
