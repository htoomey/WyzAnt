/******************************************************************************
		    Newton-Raphson Method - Algorithm 2.3
*******************************************************************************

To find a solution to f(x) = 0 given an initial approximation p0:

(This algorithm's order of convergence is à = 2.)

INPUT initial approximation p0; tolerance TOL; maximum number of iterations N0;
the functions f() and fprime() = df()/dy.

OUTPUT approximate solution p or message of failure.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile        = "023.out";   /* Customized default output file name. */
char *eq_text_f      = "f(x) = cos(x) - x";	/* Needs updating  $  */
char *eq_text_fprime = "f'(x) = -sin(x) - 1";	/* Needs updating  $  */


/*****************************************************************************/
/* f(x) - Function to evaluate, f(x).  Needs updating $.                     */
/*****************************************************************************/
double f(x)
double x;
{
  return (cos(x) - x);
}

/*****************************************************************************/
/* fprime(x) - Derivative of function to evaluate, f'(x).  Needs updating $. */
/*****************************************************************************/
double fprime(x)
double x;
{
  return (-sin(x) - 1.0);
}
/*****************************************************************************/


main()
{
  double p, p0, TOL, f(), fprime();
  int i, N0;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Newton-Raphson Method - Algorithm 2.3\n\n");
  printf2("%s\n", eq_text_f);
  printf2("%s\n\n", eq_text_fprime);

  printf("Enter maximum number of iterations, N0: ");
  scanf("%d", &N0);
  fprintf(file_id, "Maximum number of iterations: %d\n", N0);

  printf("Enter Tolerance TOL (1.0e-9)?: ");
  scanf("%lf", &TOL);
  fprintf(file_id, "Tolerance: %lg\n", TOL);

  printf("Enter initial approximation p0: ");
  scanf("%lf", &p0);

  printf2("\n n\t  p");		/* Print table header. */
  printf2("\n-----------------------\n");
  printf2("% d\t% .11lf\n", 0, p0);

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  i = 1;

  /* STEP #2 */
  while (i <= N0) {		/* Do Steps 3-6.  */

    /* STEP #3 */
    p = p0 - f(p0) / fprime(p0); /* Compute p(i). */

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
