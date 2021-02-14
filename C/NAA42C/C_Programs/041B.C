/******************************************************************************
		Composite Trapezoidal Rule - Algorithm 4.1B
*******************************************************************************

                                ôb
To approximate the integral I = ³  f(x) dx:
                                õa

INPUT endpoints a, b; positive integer n; the function f().

OUTPUT approximation XI to I.

NOTE: This algorithm was included as a "Homework Helper."  See p. 171,
Exercise Set 4.3, Problems 1, 2 a).  Also see p. 179, Exercise Set 4.4,
Problems 1, 4 b), 5 a), 7 a), 13 b), ** 14 **, 25 c).

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "041b.out";	/* Customized default output file name.     */
char *eq_text_f = "f(x) = sin(x)";	/* Needs updating  $  */


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
  double a, b, h, X, XI, f();
  int i, n;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Composite Trapezoidal Rule - Algorithm 4.1B\n\n");

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
    printf("Enter number of intervals on [a,b], n: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greater than zero.\n");
  } while (n <= 0);
  fprintf(file_id, "n = %d intervals on [a,b].\n", n);

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  h = (b - a)/n;

  /* STEP #2 */
  XI = 0.0;

  /* STEP #3 */
  for (i=1;i<n;i++) {

    /* STEP #4 */
    X = a + i*h;

    /* STEP #5 */
    XI += f(X);
  }

  /* STEP #6 */
  XI = h*(f(a) + f(b) + 2.0*XI) / 2.0;

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #7 */
  printf2("Interval number h = %lg\n\n", h);
  printf2("    ô%lg\n", b);
  printf2("I = ³  f(x) dx = %.11lg\n", XI);
  printf2("    õ%lg\n\n", a);
  printf2("Required %d functional evaluations.\n", n+1);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
