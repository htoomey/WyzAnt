/*
**  FILE NAME: SIMPSON.C
**  LANGUAGE:  C
**  COMPILERS: Any ANSI C Compiler  (Microsoft C 5.0, Turbo C 2.0, etc.)
**  STATUS:    Compiles and runs correctly
*/

/******************************************************************************
		Composite Simpson's Rule - Algorithm 4.1
*******************************************************************************

                                ôb
To approximate the integral I = ³  f(x) dx:
                                õa

INPUT endpoints a, b; positive integer n; the function f().

OUTPUT approximation XI to I.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 2Q 1991, v4.0      *
******************************************************************************/

#include "naautil.h"		/* Numerical Analysis Algorithms Utilities. */
#define	PI	3.141592653589793238462643383279502884197

char *outfile   = "simpson.out";	/* Default name of the output file. */
char *eq_text_f = "f(x) = sin(x)";	/* Needs updating  $.               */


/*****************************************************************************/
/* f(x) - Function to evaluate, f(x).  Needs updating $.                     */
/*****************************************************************************/
double f(x)
double x;
{
  return (sin(x));
}
/*****************************************************************************/


main()
{
  double a, b, h, X, XI, XI0, XI1, XI2, f();
  int i, n;

  /**********
   * INPUTS *
   **********/

  get_title();			/* Prompts for optional comments. */

  printf("Composite Simpson's Rule - Algorithm 4.1\n\n");
  printf("%s\n\n", eq_text_f);

  printf("Enter endpoint a: ");
  scanf("%lf", &a);
  printf("Enter endpoint b: ");
  scanf("%lf", &b);

  do {
    printf("Enter number of intervals on [a,b], n: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greater than zero.\n");
  } while (n <= 0);

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
  /* Write output to the screen. */
  printf("Interval number h = %lg\n\n", h);
  printf("     ô%lg\n", b);
  printf("XI = ³  f(x) dx = %.11lg\n", XI);
  printf("     õ%lg\n\n", a);
  printf("Required %d functional evaluations.\n", n + 1);

  /* Write output to a file. */
  open_outfile(outfile);
  fprintf(file_id, "Composite Simpson's Rule - Algorithm 4.1\n\n");
  fprintf(file_id, "%s from %lg to %lg.\n\n", eq_text_f, a, b);
  fprintf(file_id, "n = %d intervals on [a,b].\n", n);
  fprintf(file_id, "Interval number h = %lg\n\n", h);
  fprintf(file_id, "     ô%lg\n", b);
  fprintf(file_id, "XI = ³  f(x) dx = %.11lg\n", XI);
  fprintf(file_id, "     õ%lg\n\n", a);
  fprintf(file_id, "Required %d functional evaluations.\n", n + 1);
  close_outfile(outfile);

  printf("\nOutput saved into file \"%s\".\n", outfile);

}  				/* STOP */

/*****************************************************************************/
/*	   Copyright (C) 1991, Harold A. Toomey, All Rights Reserved.        */
/*****************************************************************************/
