/******************************************************************************
			Secant Method - Algorithm 2.4
*******************************************************************************

To find a solution to f(x) = 0 given the initial approximations p0 and p1:

(This algorithm's order of convergence is à = 1.62.)

INPUT initial approximations p0, p1; tolerance TOL; maximum number of
iterations N0; the function f().

OUTPUT approximate solution p or message of failure.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "024.out";	/* Customized default output file name.     */
char *eq_text_f = "f(x) = cos(x) - x";	/* Needs updating  $  */


/*****************************************************************************/
/* f(x) - Function to evaluate, f(x).  Needs updating $.                     */
/*****************************************************************************/
double f(x)
double x;
{
  if (eqeval)
    return (eval_eq(x));	/* Use the Equation Evaluator  */
  else
    return (cos(x) - x);	/* Use the default function.   */
}
/*****************************************************************************/


main()
{
  double p, p0, p1, q0, q1, TOL, f();
  int i, N0;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Secant Method - Algorithm 2.4\n\n");

  if (eqeval)
    printf2("f(x) = %s", tmpstr);	/* Print the Equation Evaluator text */
  else
    printf2("%s", eq_text_f);		/* Print the default equation text */
  printf2("\n\n");

  printf("Enter maximum number of iterations, N0: ");
  scanf("%d", &N0);
  fprintf(file_id, "Maximum number of iterations: %d\n", N0);

  printf("Enter Tolerance TOL (1.0e-9)?: ");
  scanf("%lf", &TOL);
  fprintf(file_id, "Tolerance: %lg\n", TOL);

  printf("Enter initial approximation p0: ");
  scanf("%lf", &p0);

  printf("Enter initial approximation p1: ");
  scanf("%lf", &p1);

  printf2("\n n\t  p");			/* Print table header. */
  printf2("\n-----------------------\n");
  printf2("% d\t% .11lf\n", 0, p0);
  printf2("% d\t% .11lf\n", 1, p1);

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  i  = 2;
  q0 = f(p0);
  q1 = f(p1);

  /* STEP #2 */
  while (i <= N0) {		/* Do Steps 3-6. */

    /* STEP #3 */
    p = p1 - q1*(p1 - p0) / (q1 - q0);	/* Compute p(i). */

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #4 */
    printf2("% d\t% .11lf\n", i, p);
    if (fabs(p - p1) < TOL) {
      NAA_do_last(outfile);	/* NAA finish-up procedure.                 */
      exit (1);			/* STOP - Procedure completed successfully. */
    }

    /* STEP #5 */
    i++;			/* i = i + 1.                     */

    /* STEP #6 */
    p0 = p1;			/* Update p(0), q(0), p(1), q(1). */
    q0 = q1;
    p1 = p;
    q1 = f(p);
  }

  /* STEP #7 */			/* Procedure completed unsuccessfully. */
  printf2("\nMethod failed after N0 iterations, N0 = %d\n", N0);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
