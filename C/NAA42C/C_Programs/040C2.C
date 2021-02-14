/******************************************************************************
      2nd Derivative Approximation (for tabulated data) - Algorithm 4.0C2
*******************************************************************************

To approximate the second derivative of discrete data points at the point x0
for a given h.  Uses Equation 4.20.

INPUT the point x0; interval h; data points x[-1],x[0], and x[1].

OUTPUT approximation f''(x0).

NOTE: This algorithm was included as a "Homework Helper."  See p. 156,
Exercise Set 4.1, Problem 13 b).

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "040c2.out";	/* Customized default output file name.     */


main()
{
  double x0, h, Dxxf, *X, Eq4_20();
  int ch;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("2nd Derivative Approximation (for tabulated data)");
  printf2(" - Algorithm 4.0C2\n");

  /* Dynamically allocate memory for the needed arrays. */
  X = dvector(-1,1);		/* Stores f(x) of all data points. */

  do {

    printf2("\nUsing Equation 4.20 (Three-Point Formula)");

    printf("\nEnter point to evaluate, x0: ");
    scanf("%lf", &x0);

    printf("Enter interval value, h (may be negative): ");
    scanf("%lf", &h);
    fprintf(file_id, " with h = %lg:\n", h);

    fprintf(file_id, "Using points:\n");

    printf("Enter f(%lg): ", x0 - h);
    scanf("%lf", &X[-1]);
    fprintf(file_id, "\tf(%lg) = %lf\n", x0 - h, X[-1]);

    printf("Enter f(%lg): ", x0);
    scanf("%lf", &X[0]);
    fprintf(file_id, "\tf(%lg) = %lf\n", x0, X[0]);

    printf("Enter f(%lg): ", x0 + h);
    scanf("%lf", &X[1]);
    fprintf(file_id, "\tf(%lg) = %lf\n", x0 + h, X[1]);

    /*************
     * ALGORITHM *
     *************/

    /* STEP #1 */
    Dxxf = Eq4_20(X,h);		/* Evaluate the 2nd derivative */

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #2 */
    printf2("f'(%lg) = %lf\n", x0, Dxxf);

    printf("\nWould you like to try another method, h, or x0? (Y/N) N: ");
    getchar();				/* Cleans up after scanf(). */
    ch = tolower(getchar());

  } while (ch == 'y');

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(X,-1,1);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* Eq4_20() - Second derivative equation 4.20.  (see p.152 of text)          */
/*****************************************************************************/
double Eq4_20(X, h)
double *X, h;
{
  return ((X[-1] - 2.0*X[0] + X[1]) / (h*h));
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
