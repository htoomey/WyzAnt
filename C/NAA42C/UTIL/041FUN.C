/******************************************************************************
		Composite Simpson's Rule - Algorithm 4.1
			As A Stand-Alone Function
*******************************************************************************

                                ôb
To approximate the integral I = ³  f(x) dx:
                                õa

INPUT endpoints a, b; even positive integer n; the function f().

OUTPUT approximation XI to I.

NOTE: Listed as Simpson's Composite Rule in 3rd edition of the text.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

double f(double x);		/* Function prototype */


double simpson (a, b, n)
double a, b;
int n;
{
  double h, X, XI, XI0, XI1, XI2, f();
  int i;

  if ((n <= 0) || (n % 2 != 0)) {	/* Range checking */
    printf("ERROR - n must be even and greater than zero.\n");
    exit (-1);			/* Exit to system */
  }

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

  return (XI);

}  				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
