/******************************************************************************
		Runge-Kutta (Order Four) - Algorithm 5.2
*******************************************************************************

To approximate the solution of the initial-value problem

	y' = f(t,y),  a ó t ó b,  y(a) = à,

at (N + 1) equally spaced numbers in the interval [a,b]:

INPUT endpoints a, b; integer N or interval h; initial condition à; the
function f().

OUTPUT approximation w to y at the (N + 1) values of t.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "052.out";	/* Customized default output file name.     */
char *eq_text_f = "y' = f(t,y) = -y + t + 1";	/* Needs updating  $  */


/*****************************************************************************/
/* f(t,y) - Function to evaluate, y' = f(t,y).  Needs updating $.            */
/*****************************************************************************/
double f(t,y)
double t, y;
{
  if (eqeval)
    return (eval_eq(0.0,y,t));	/* Use the Equation Evaluator  */
  else
    return (-y + t + 1);	/* Use the default function.   */
}
/*****************************************************************************/


main()
{
  double a, b, h, t, w, alpha, f();
  double K1, K2, K3, K4;
  int i, N, ch;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Runge-Kutta (Order Four) - Algorithm 5.2\n\n");

  if (eqeval)
    printf2("y' = f(t,y) = %s", tmpstr);/* Print the Equation Evaluator text */
  else
    printf2("%s", eq_text_f);		/* Print the default equation text */
  printf2("\n\n");

  printf("Enter endpoint a: ");
  scanf("%lf", &a);
  printf("Enter endpoint b: ");
  scanf("%lf", &b);
  fprintf(file_id, "Solving for points on interval [%lg,%lg].\n", a, b);

  printf("Enter initial condition y(%lg) = à: ", a);
  scanf("%lf", &alpha);
  fprintf(file_id, "y(%lg) = à = %lg\n", a, alpha);

  printf("Do you want to enter N or h? H: ");
  getchar();			/* Cleans up after scanf(). */
  ch = tolower(getchar());
  if (ch == 'n') {
    printf("Enter number of intervals N: ");
    scanf("%d", &N);
    h = (b - a)/N;		/* STEP #1 */
  } else {			/* default */
    printf("Enter interval number h: ");
    scanf("%lf", &h);
    N = (int) ceil((b - a)/h);
  }
  printf2("Number of intervals N = %d\n", N);
  printf2("Interval number h = %lg\n", h);

				/* Print table header. */
  printf2("\n t\t  w ÷ y\n------------------------\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  t = a;
  w = alpha;
  printf2("% .2lf\t% .11lf\n", t, w);

  /* STEP #2 */
  for (i=1;i<=N;i++) {		/* Do Steps 3-5. */

    /* STEP # 3 */
    K1 = h*f(t,       w);
    K2 = h*f(t + h/2, w + K1/2);
    K3 = h*f(t + h/2, w + K2/2);
    K4 = h*f(t + h,   w + K3);

    /* STEP #4 */
    w = w + (K1 + 2*K2 + 2*K3 + K4)/6;	/* compute w(i). */
    t = a + i*h;			/* compute t(i). */

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #5 */
    printf2("% .2lf\t% .11lf\n", t, w);
  }

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}  /* STEP #6 */		/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
