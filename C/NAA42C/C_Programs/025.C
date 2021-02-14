/******************************************************************************
		       Steffensen's Method - Algorithm 2.5
*******************************************************************************

To find a solution to p = g(p) given an initial approximation p0:

INPUT initial approximation p0; tolerance TOL; maximum number of
iterations N0; the function g().

OUTPUT approximate solution p or message of failure.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "025.out";	/* Customized default output file name.     */
char *eq_text_g = "g(x) = sqrt(10/(x + 4))";	/* Needs updating  $  */


/*****************************************************************************/
/* g(x) - Fixed-point function to evaluate, g(x).  Needs updating $.         */
/*****************************************************************************/
double g(x)
double x;
{
  if (eqeval)
    return (eval_eq(x));		/* Use the Equation Evaluator  */
  else
    return (sqrt(10.0/(x + 4.0)));	/* Use the default function.   */
}
/*****************************************************************************/


main()
{
  double p, p0, p1, p2, TOL, g();
  int i, N0;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Steffensen's Method - Algorithm 2.5\n\n");

  if (eqeval)
    printf2("g(x) = %s", tmpstr);	/* Print the Equation Evaluator text */
  else
    printf2("%s", eq_text_g);		/* Print the default equation text */
  printf2("\n\n");

  printf("Enter maximum number of iterations, N0: ");
  scanf("%d", &N0);
  fprintf(file_id, "Maximum number of iterations: %d\n", N0);

  printf("Enter Tolerance TOL (1.0e-9)?: ");
  scanf("%lf", &TOL);
  fprintf(file_id, "Tolerance: %lg\n", TOL);

  printf("Enter initial approximation p0: ");
  scanf("%lf", &p0);

  printf2("\n n\t p0\t\t p1\t\t p2");    /* Print table header. */
  printf2("\n---------------------------");
  printf2("---------------------------\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  i = 1;

  /* STEP #2 */
  while (i <= N0) {		/* Do Steps 3-6. */

    /* STEP #3 */
    p1 = g(p0);			/* Compute p1(i-1).         */
    p2 = g(p1);			/* Compute p2(i-1).         */
				/* Compute p0(i).           */
				/* Aitken's delta^2 method. */
    p  = p0 - (p1 - p0)*(p1 - p0) / (p2 - 2.0*p1 + p0);

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #4 */
    printf2("% d\t% .9lf\t% .9lf\t% .9lf\n", i-1, p0, p1, p2);

    if (fabs(p - p0) < TOL) {
      printf2("% d\t% .9lf\n", i, p);
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
