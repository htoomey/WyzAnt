/******************************************************************************
	2nd Derivative Approximation (for functions) - Algorithm 4.0C1
*******************************************************************************

To approximate the second derivative of f(x) at the point x0 for a given h.
Uses Equation 4.20.

INPUT the point x0; interval h; function f() and f_2prime() (if known):

OUTPUT approximation f''(x0).

NOTE: This algorithm was included as a "Homework Helper."  See p. 155,
Exercise Set 4.1, Problems 9-10.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities.  */

char *outfile         = "040c1.out"; /* Customized default output file name. */
char *eq_text_f       = "f(x)   = x*e^(x)";	/* Needs updating  $  */
char *eq_text_f2prime = "f''(x) = (x+2)*e^(x)";	/* Needs updating  $  */


/*****************************************************************************/
/* f(x) - Function to evaluate, f(x).  Needs updating $.                     */
/*****************************************************************************/
double f(x)
double x;
{
  return (x*exp(x));
}

/*****************************************************************************/
/* f2prime(x) - Actual second derivative of function, f''(x), if known.      */
/*              Needs updating $.                                            */
/*****************************************************************************/
double f2prime(x)
double x;
{
  return ((x+2.0)*exp(x));
}
/*****************************************************************************/


main()
{
  double x0, h, f(), f2prime(), Dxxf, Eq4_20(), tv, er;
  int ch;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("2nd Derivative Approximation (for functions)");
  printf2(" - Algorithm 4.0C1\n\n");
  printf2("%s\n", eq_text_f);
  printf2("%s\n\n", eq_text_f2prime);

  do {

    printf2("Using Equation 4.20 (Three-Point Formula)");
    printf("Enter point to evaluate, x0: ");
    scanf("%lf", &x0);

    printf("Enter interval value, h (may be negative): ");
    scanf("%lf", &h);
    fprintf(file_id, " with h = %lg:\n", h);

    /*************
     * ALGORITHM *
     *************/

    /* STEP #1 */
    Dxxf = Eq4_20(x0, h);		/* Evaluate the 2nd derivative */

    /* STEP #2 */
    tv = f2prime(x0);			/* True value term             */
    er = fabs(f2prime(x0) - Dxxf);	/* Error term                  */

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #3 */
    printf2("\n  f''(%lg) = %.6lf\n", x0, Dxxf);
    printf2("  True value = %lf\n", tv);
    printf2("  Error = %.2le\n\n", er);

    printf("Would you like to try another value of h? (Y/N) N: ");
    getchar();				/* Cleans up after scanf(). */
    ch = tolower(getchar());

  } while (ch == 'y');

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* Eq4_20() - Second derivative equation 4.20.  (see p.152 of text)          */
/*****************************************************************************/
double Eq4_20(x0, h)
double x0, h;
{
  return ((f(x0 - h) - 2.0*f(x0) + f(x0 + h)) / (h*h));
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
