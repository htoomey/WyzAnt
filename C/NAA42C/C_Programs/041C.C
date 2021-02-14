/******************************************************************************
		Composite Midpoint Rule - Algorithm 4.1C
*******************************************************************************

                                ôb
To approximate the integral I = ³  f(x) dx:
                                õa

INPUT endpoints a, b; positive integer n; the function f().

OUTPUT approximation XI to I.

NOTE: This algorithm was included as a "Homework Helper."  See p. 179,
Exercise Set 4.4, Problems 2, 4 c), 5 c), 9, 12, 13 c), *** 15 ***, 26-27.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "041c.out";	/* Default name of the output file.         */
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
  int i, m, n;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Composite Midpoint Rule - Algorithm 4.1C\n\n");

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
    printf("Enter number of intervals on [a,b], n = 2m: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greater than zero.\n");
    if (n % 2 != 0)
      printf("ERROR - n must be an even number.\n");
  } while ((n <= 0) || (n % 2 != 0));
  fprintf(file_id, "n = 2m = %d intervals on [a,b].\n", n);

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  m = n / 2;
  h = (b - a)/(n + 2);

  /* STEP #2 */
  XI = 0.0;

  /* STEP #3 */
  for (i=0;i<=m;i++) {

    /* STEP #4 */
    X = a + (2*i + 1)*h;

    /* STEP #5 */
    XI += f(X);
  }

  /* STEP #6 */
  XI = 2.0*h*XI;

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #7 */
  printf2("Interval number h = %lg\n\n", h);
  printf2("    ô%lg\n", b);
  printf2("I = ³  f(x) dx = %.11lg\n", XI);
  printf2("    õ%lg\n\n", a);
  printf2("Required %d functional evaluations.\n", m+1);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}  				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
