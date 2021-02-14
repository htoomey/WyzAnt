/******************************************************************************
			Natural Cubic Spline - Algorithm 3.4
				(Free Cubic Spline)
*******************************************************************************

To construct the cubic spline interpolant S for the function f(), defined at
the numbers x0 < x1 < ... < xn, satisfying S"(x0) = S"(xn) = 0:

INPUT n; x0,x1,...,xn; either generate a(i) = f(x(i)) for i = 0,1,...,n or
input a(i) for i = 0,...,n.

OUTPUT a(j), b(j), c(j), d(j), for j = 0,1,...,n-1.
(Note: S(x) = S(j)(x) = a(j) + b(j)*(x-x(j)) + c(j)*(x-x(j))^2
	+ d(j)*(x-x(j))^3 for x(j) ó x ó x(j+1).)

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "034.out";	/* Customized default output file name.     */
char *eq_text_f = "f(x) = cos(PI*X)";	/* Needs updating  $  */


/*****************************************************************************/
/* f(x) - Function to evaluate, f(x).  May be unnecessary if using tabulated */
/*        data.  Needs updating $.                                           */
/*****************************************************************************/
double f(x)
double x;
{
  if (eqeval)
    return (eval_eq(x));	/* Use the Equation Evaluator  */
  else
    return (cos(PI*x));		/* Use the default function.   */
}
/*****************************************************************************/


main()
{
  double *a, *alpha, *b, *c, *d, *h, *l, *mu, *x, *z, f();
  double area, X, tmp, simpson_S();
  int i, j, n, ch;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Natural Cubic Spline - Algorithm 3.4\n\n");

  do {
    printf("Enter number of data points, n+1: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greater than zero.\n");
  } while (n <= 0);

  n--;				/* n = n - 1. */
  fprintf(file_id, "Uses %d data points.\n", n+1);
  fprintf(file_id,
    "Computes %d coefficient sets for a Natural Cubic Spline.\n\n", n);

  /* Dynamically allocate memory for the needed arrays. */
  a     = dvector(0,n);
  alpha = dvector(0,n);
  b     = dvector(0,n);
  c     = dvector(0,n);
  d     = dvector(0,n);
  h     = dvector(0,n);
  l     = dvector(0,n);
  mu    = dvector(0,n);
  x     = dvector(0,n);
  z     = dvector(0,n);

  printf("Do the a(i) = f(x(i)) need to be generated from f(x)? (Y/N) Y: ");
  getchar();			/* Cleans up after scanf(). */
  ch = tolower(getchar());

  for (i=0;i<=n;i++) {		/* Enter x(i). */
    printf("Enter x%d: ", i);
    scanf("%lf", &x[i]);
    if (ch == 'n') {		/* Input a(i). */
      printf("Enter f(%lg): ", x[i]);
      scanf("%lf", &a[i]);
      printf("\n");
    } else
      a[i] = f(x[i]);		/* Use function f(x(i)) for a(i). */
  }

  printf2("S(x) =  S(j)(x) = a(j) + b(j)*(x-x(j)) + c(j)*(x-x(j))^2 ");
  printf2("+ d(j)*(x-x(j))^3\n\tfor x(j) ó x ó x(j+1)\n\n");
  if (ch != 'n') {
    if (eqeval)
      printf2("f(x) = %s", tmpstr);	/* Print the Equation Evaluator text */
    else
      printf2("%s", eq_text_f);		/* Print the default equation text */
    printf2("\n\n");
  }

				/* Print table header. */
  printf2(" i\tx(i)\t a(i) = f(x)\t b(i)\t\t c(i)\t\t d(i)\n");
  printf2("----------------------------------------");
  printf2("--------------------------------------\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  for (i=0;i<n;i++)
    h[i] = x[i+1] - x[i];

  /* STEP #2 */
  for (i=1;i<n;i++)
    alpha[i] = (3.0*(a[i+1]*h[i-1] - a[i]*(x[i+1]-x[i-1]) + a[i-1]*h[i])) /
		 (h[i-1]*h[i]);

  /* STEP #3 */
  l[0]  = 1.0;			/* Steps 3, 4, 5, and part of step 6 solve   */
  mu[0] = 0.0;			/* a tridiagonal system using Algorithm 6.7. */
  z[0]  = 0.0;

  /* STEP #4 */
  for (i=1;i<n;i++) {
    l[i]  = 2.0*(x[i+1] - x[i-1]) - h[i-1]*mu[i-1];
    mu[i] = h[i]/l[i];
    z[i]  = (alpha[i] - h[i-1]*z[i-1]) / l[i];
  }

  /* STEP #5 */
  l[n] = 1.0;
  z[n] = 0.0;
  c[n] = 0.0;

  /* STEP #6 */
  for (j=n-1;j>=0;j--) {
    c[j] = z[j] - mu[j]*c[j+1];
    b[j] = (a[j+1]-a[j])/h[j] - h[j]*(c[j+1]+2.0*c[j])/3.0;
    d[j] = (c[j+1]-c[j])/(3.0*h[j]);
  }

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #7 */
  area = 0.0;
  for (i=0;i<n;i++) {
    printf2(" %d\t% .3lg\t% .9lf\t% .9lf\t% .9lf\t% .9lf\n",
	 i, x[i], a[i], b[i], c[i], d[i]);
				/* Integrate the spline.        */
    area += simpson_S(x[i], x[i+1], a[i], b[i], c[i], d[i], a[i+1]);
  }
				/* Print a(i) only in last row. */
  printf2(" %d\t% .2lg\t% .9lf\n\n", n, x[n], a[n]);

  printf2("    ô%lg\n", x[n]);	/* Print integral.              */
  printf2("I = ³  S(x) dx = %.11lg\n", area);
  printf2("    õ%lg\n\n", x[0]);

  printf("Would you like to evaluate f(x), f'(x), and f''(x)");
  printf(" at a certain x? (Y/N) N: ");
  getchar();			/* Cleans up after scanf(). */
  ch = tolower(getchar());

  if (ch == 'y') {

    printf("\nEnter a value of x between %lg and %lg\n", x[0], x[n]);
    printf("for which f(x), f'(x), and f''(x) may be evaluated.");
    scanf("%lf", &X);
    printf("\n");

    for (j=0;j<n;j++) {		/* Search for correct x interval. */
      if ((X >= x[j]) && (X < x[j+1]))
        break;			/* keep j.                        */
    }

    /* Find f(x). */
    tmp = a[j] + b[j]*(X-x[j]) + c[j]*pow(X-x[j], 2.0) + d[j]*pow(X-x[j], 3.0);
    printf2("f(%lg) ÷ S(%lg) = %.11lg\n", X, X, tmp);

    /* Find f'(x). */
    tmp = b[j] + 2.0*c[j]*(X-x[j]) + 3.0*d[j]*pow(X-x[j], 2.0);
    printf2("f'(%lg) ÷ S'(%lg) = %.11lg\n", X, X, tmp);

    /* Find f''(x). */
    tmp = 2.0*c[j] + 6.0*d[j]*(X-x[j]);
    printf2("f''(%lg) ÷ S''(%lg) = %.11lg\n", X, X, tmp);

  }

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(z,0,n);
  free_dvector(x,0,n);
  free_dvector(mu,0,n);
  free_dvector(l,0,n);
  free_dvector(h,0,n);
  free_dvector(d,0,n);
  free_dvector(c,0,n);
  free_dvector(b,0,n);
  free_dvector(alpha,0,n);
  free_dvector(a,0,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* simpson_S() - Composite Simpson's Rule - Algorithm 4.1 (with f() = S())   */
/*****************************************************************************/
double simpson_S(aa,bb,a,b,c,d,a2)
double aa, bb;		/* Bounds of integration. aa = x(j), bb = x(j+1) */
double a,b,c,d;		/* Cubic polynomial coefficients.                */
double a2;		/* a = f(aa), a2 = f(bb)                         */
{
  double h, x, X, XI, XI0, XI1, XI2, f();
  int i, n=10;			/* n = intervals on [a,b].  */

  h   = (bb - aa)/n;
  XI0 = a + a2;			/* S(xj) = f(xj) = a(j).    */
  XI1 = 0.0;			/* Summation of S(x(2i-1)). */
  XI2 = 0.0;			/* Summation of S(x(2i)).   */

  for (i=1;i<n;i++) {
    X = aa + i*h;
    x = X - aa;			/* x = (x-xj) = (x-aa)      */
    if (i % 2 == 0)
      XI2 += ((d*x+c)*x+b)*x+a;	/* Compute S(x) for i even. */
    else
      XI1 += ((d*x+c)*x+b)*x+a;	/* Compute S(x) for i odd.  */
  }

  XI = h*(XI0 + 2.0*XI2 + 4.0*XI1) / 3.0;
  return (XI);

}

/* S(x) = S(j)(x) = a(j) + b(j)*(x-x(j)) + c(j)*(x-x(j))^2 + d(j)*(x-x(j))^3 */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
