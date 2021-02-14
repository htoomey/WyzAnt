/******************************************************************************
   1st Derivative Approximation (for functions within TOL) - Algorithm 4.0B3
*******************************************************************************

To approximate the derivative of f(x) at the point x0 within a specified
tolerance.  Uses Equation 4.13.

INPUT the point x0; tolerance TOL; function f() and f_prime() (if known):

OUTPUT approximation f'(x0) accurate to within TOL.

NOTE: This algorithm was included as a "Homework Helper."  See p. 155,
Exercise Set 4.1, Problem 8.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities.  */

char *outfile        = "040b3.out";  /* Customized default output file name. */
char *eq_text_f      = "f(x) = x*e^(x)";	/* Needs updating  $  */
char *eq_text_fprime = "f'(x) = (x+1)*e^(x)";	/* Needs updating  $  */

#define	NUMTRIES	16	/* Max. number of attempts to be within TOL. */


/*****************************************************************************/
/* f(x) - Function to evaluate, f(x).  Needs updating $.                     */
/*****************************************************************************/
double f(x)
double x;
{
  return (x*exp(x));
}

/*****************************************************************************/
/* fprime(x) - Actual derivative of function, f'(x), if known.               */
/*             Needs updating $.                                             */
/*****************************************************************************/
double fprime(x)
double x;
{
  return ((x+1.0)*exp(x));
}
/*****************************************************************************/


main()
{
  double x0, h, TOL, f(), fprime(), Eq4_13(), fold, fnew;
  int i, ch;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("1st Derivative Approximation (for functions within TOL)");
  printf2(" - Algorithm 4.0B3\n\n");
  printf2("%s\n", eq_text_f);
  printf2("%s\n\n", eq_text_fprime);

  printf("Enter Tolerance, TOL (less than 1.0e-7)?: ");
  scanf("%lf", &TOL);
  fprintf(file_id, "Accurate to within a tolerance of %lg\n\n", TOL);

  do {
    printf("Enter point to evaluate, x0: ");
    scanf("%lf", &x0);

    /*************
     * ALGORITHM *
     *************/

    /* STEP #1 */
    h    = 0.1;
    fold = Eq4_13(x0, h);	/* Evaluate the 1st derivative           */

    /* STEP #2 */
    for (i=1;i<=NUMTRIES;i++) {
      h    *= 0.1;		/* Decrease h by one order of magnitude. */
      fnew = Eq4_13(x0, h);	/* Evaluate the 1st derivative           */

      /* STEP #3 */
      if (fabs(fold - fnew) < TOL) {

        /***********
         * OUTPUTS *
         ***********/

        printf("\n");
        printf2("f'(%lg) = %lf\n", x0, fnew);
        printf2("True value = %lf\n", fprime(x0));
        printf2("Error = %.2le\n\n", fabs(fprime(x0) - fnew));

        i = NUMTRIES + 2;		/* Will exit this "for" loop. */
      } else
        fold = fnew;			/* try again                  */
    }

    /* STEP #4 */
    if (i == NUMTRIES + 1)
      printf("ERROR - Tolerance (%lg) may be too tight.\n\n", TOL);

    printf("Would you like to evaluate another value of x0? (Y/N) N: ");
    getchar();				/* Cleans up after scanf(). */
    ch = tolower(getchar());

  } while (ch == 'y');

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* Eq4_13() - First derivative equation 4.13.  (see p.149 of text)           */
/*****************************************************************************/
double Eq4_13(x0, h)
double x0, h;
{
  return ((f(x0 + h) - f(x0 - h)) / (2.0*h));
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
