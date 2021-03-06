/******************************************************************************
		Composite Simpson's Rule - Algorithm 4.1
*******************************************************************************

                                �b
To approximate the integral I = �  f(x) dx:
                                �a

INPUT endpoints a, b; even positive integer n; the function f().

OUTPUT approximation XI to I.

NOTE: Listed as Simpson's Composite Rule in 3rd edition of the text.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "041.out";	/* Default name of the output file.         */
char *eq_text_f = "f(x) = sin(x)";	/* Needs updating  $        */


/*****************************************************************************/
/* f(x) - Function to evaluate, f(x).  Needs updating $.                     */
/*****************************************************************************/
double f(x)
double x;
{
  if (eqeval)
    return (eval_eq(x));	/* Use the Equation Evaluator  */
  else
    return (sin(x));		/* Use the default function.   */
}
/*****************************************************************************/


main()
{
  double a, b, h, X, XI, XI0, XI1, XI2, f();
  int i, n;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Composite Simpson's Rule - Algorithm 4.1\n\n");

  if (eqeval)
    printf2("f(x) = %s", tmpstr);	/* Print the Equation Evaluator text */
  else
    printf2("%s", eq_text_f);		/* Print the default equation text */
  printf("\n\n");

  printf("Enter endpoint a: ");
  scanf("%lf", &a);
  printf("Enter endpoint b: ");
  scanf("%lf", &b);
  fprintf(file_id, " from %lg to %lg.\n\n", a, b);

  do {
    printf("Enter even number of intervals on [a,b], n: ");
    scanf("%d", &n);
    if ((n <= 0) || (n % 2 != 0))
      printf("ERROR - n must be even and positive.\n");
  } while ((n <= 0) || (n % 2 != 0));
  fprintf(file_id, "n = %d intervals on [a,b].\n", n);

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  h = (b - a)/n;

  /* STEP #2 */
  XI0 = f(a) + f(b);
  XI1 = 0.0;			/* Summation of f(x(2i-1)). */
  XI2 = 0.0;			/* Summation of f(x(2i)).   */

  /* STEP #3 */
  for (i=1;i<n;i++) {

    /* STEP #4 */
    X = a + i*h;

    /* STEP #5 */
    if (i % 2 == 0)
      XI2 += f(X);		/* For even i. */
    else
      XI1 += f(X);		/* For odd i.  */
  }

  /* STEP #6 */
  XI = h*(XI0 + 2.0*XI2 + 4.0*XI1) / 3.0;

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #7 */
  printf2("Interval number h = %lg\n\n", h);
  printf2("     �%lg\n", b);
  printf2("XI = �  f(x) dx = %.11lg\n", XI);
  printf2("     �%lg\n\n", a);
  printf2("Required %d functional evaluations.\n", n+1);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}  				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
