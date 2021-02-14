/******************************************************************************
	1st Derivative Approximation (for functions) - Algorithm 4.0B1
*******************************************************************************

To approximate the derivative of f(x) at the point x0 for a given h.  Uses
Equation 4.2, 4.12, 4.13, 4.14, or 4.15.

INPUT the point x0; interval h; function f() and f_prime() (if known):

OUTPUT approximation f'(x0).

NOTE: This algorithm was included as a "Homework Helper."  See p. 154,
Exercise Set 4.1, Problems 4-6.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities.  */

char *outfile        = "040b1.out";  /* Customized default output file name. */
char *eq_text_f      = "f(x)  = x*e^(x)";	/* Needs updating  $  */
char *eq_text_fprime = "f'(x) = (x+1)*e^(x)";	/* Needs updating  $  */


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
  double Eq4_2(), Eq4_12(), Eq4_13(), Eq4_14(), Eq4_15();
  double x0, h, f(), fprime(), Dxf, tv, er;
  int ch, Eqnum;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("1st Derivative Approximation (for functions)");
  printf2(" - Algorithm 4.0B1\n\n");
  printf2("%s\n", eq_text_f);
  printf2("%s\n\n", eq_text_fprime);

  do {

    printf("Select desired formula:              ");
    printf("     x-2, x-1, x0, x1, x2, x3, x4\n");
    printf("  (1)   Equation 4.2   (2-Point Formula) ");
    printf("            X   X\n");
    printf("  (2)   Equation 4.12  (3-Point Formula) ");
    printf("            X   X   X\n");
    printf("  (3)   Equation 4.13  (3-Point Formula) ");
    printf("       X        X\n");
    printf("  (4)   Equation 4.14  (5-Point Formula) ");
    printf("  X    X        X   X\n");
    printf("  (5)   Equation 4.15  (5-Point Formula) ");
    printf("            X   X   X   X   X\n");
    printf("----> ");
    scanf("%d", &Eqnum);

    switch (Eqnum) {
      case 1:  printf2("Using Equation 4.2  (Two-Point Formula)");    break;
      case 2:  printf2("Using Equation 4.12 (Three-Point Formula)");  break;
      case 3:  printf2("Using Equation 4.13 (Three-Point Formula)");  break;
      case 4:  printf2("Using Equation 4.14 (Five-Point Formula)");   break;
      case 5:  printf2("Using Equation 4.15 (Five-Point Formula)");   break;
      default: printf2("Using Equation 4.13 (Three-Point Formula)");  break;
    }

    printf("\nEnter point to evaluate, x0: ");
    scanf("%lf", &x0);
    printf("Enter interval value, h (may be negative): ");
    scanf("%lf", &h);
    fprintf(file_id, " with h = %lg:\n", h);

    /**************
     * ALGORITHMS *
     **************/

    /* STEP #1 */
    switch (Eqnum) {		/* Evaluate the 1st derivative */
      case 1:  Dxf = Eq4_2(x0, h);   break;
      case 2:  Dxf = Eq4_12(x0, h);  break;
      case 3:  Dxf = Eq4_13(x0, h);  break;
      case 4:  Dxf = Eq4_14(x0, h);  break;
      case 5:  Dxf = Eq4_15(x0, h);  break;
      default: Dxf = Eq4_13(x0, h);  break;
    }

    /* STEP #2 */
    tv = fprime(x0);			/* True value term */
    er = fabs(fprime(x0) - Dxf);	/* Error term      */

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #3 */
    printf2("\n  f'(%lg) = %lf\n", x0, Dxf);
    printf2("  True value = %lf\n", tv);
    printf2("  Error = %.2le\n\n", er);

    printf("Would you like to try another method, h, or x0? (Y/N) N: ");
    getchar();			/* Cleans up after scanf(). */
    ch = tolower(getchar());

  } while (ch == 'y');

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* Eq4_2() - First derivative equation 4.2.  (see p.146 of text)             */
/*****************************************************************************/
double Eq4_2(x0, h)
double x0, h;
{
  return ((f(x0 + h) - f(x0)) / h);
}

/*****************************************************************************/
/* Eq4_12() - First derivative equation 4.12.  (see p.146 of text)           */
/*****************************************************************************/
double Eq4_12(x0, h)
double x0, h;
{
  return ((-3.0*f(x0) + 4.0*f(x0 + h) - f(x0 + 2*h)) / (2.0*h));
}

/*****************************************************************************/
/* Eq4_13() - First derivative equation 4.13.  (see p.149 of text)           */
/*****************************************************************************/
double Eq4_13(x0, h)
double x0, h;
{
  return ((f(x0 + h) - f(x0 - h)) / (2.0*h));
}

/*****************************************************************************/
/* Eq4_14() - First derivative equation 4.14.  (see p.150 of text)           */
/*****************************************************************************/
double Eq4_14(x0, h)
double x0, h;
{
  return ((f(x0 - 2*h) - 8.0*f(x0 - h) + 8.0*f(x0 + h) - f(x0 + 2*h))
	 / (12.0*h));
}

/*****************************************************************************/
/* Eq4_15() - First derivative equation 4.15.  (see p.150 of text)           */
/*****************************************************************************/
double Eq4_15(x0, h)
double x0, h;
{
  return ((-25.0*f(x0) + 48.0*f(x0 + h) - 36.0*f(x0 + 2*h) + 16.0*f(x0 + 3*h)
	 - 3.0*f(x0 + 4*h)) / (12.0*h));
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
