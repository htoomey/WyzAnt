/******************************************************************************
	     Adams Forth-Order Predictor-Corrector - Algorithm 5.4
*******************************************************************************

To approximate the solution of the initial-value problem

	y' = f(t,y),  a ó t ó b,  y(a) = à,

at (N + 1) equally spaced numbers in the interval [a,b]:

INPUT endpoints a, b; initial condition à; tolerance TOL; maximum step size
hmax; minimum stepsize hmin; the functions f() and y(t) (if known).

OUTPUT approximation w to y at the (N + 1) values of t.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "054.out";	/* Customized default output file name.     */
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
  double a, b, h, t[4], w[4], alpha, T, W;
  double f(), y(), K1, K2, K3, K4, val1, val2, val3;
  int i, j, iprev, ch, N;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Adams Forth-Order Predictor-Corrector - Algorithm 5.4\n\n");

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
  t[0] = a;
  w[0] = alpha;
  i    = 0;
  printf2("% .2lf\t% .11lf\n", t[i], w[i]);

  /* STEP #2 */
  for (i=1;i<=3;i++) {		/* Do Steps 3-5. */
  /* Compute starting values using Runge-Kutta method. */

    /* STEP #3 */
    iprev = i - 1;		/* For speed. */
    K1 = h*f(t[iprev],         w[iprev]);
    K2 = h*f(t[iprev] + h/2.0, w[iprev] + K1/2.0);
    K3 = h*f(t[iprev] + h/2.0, w[iprev] + K2/2.0);
    K4 = h*f(t[iprev] + h,     w[iprev] + K3);

    /* STEP #4 */
    w[i] = w[iprev] + (K1 + 2.0*K2 + 2.0*K3 + K4)/6.0;
    t[i] = a + i*h;

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #5 */
    printf2("% .2lf\t% .11lf\n", t[i], w[i]);
  }

  /* STEP #6 */
  for (i=4;i<=N;i++) {		/* Do Steps 7-10. */

    /* STEP #7 */
    T    = a + i*h;
    val3 = f(t[3],w[3]);	/* For speed.  Saves 1 function evaluation. */
    val2 = f(t[2],w[2]);	/* For speed.  Saves 1 function evaluation. */
    val1 = f(t[1],w[1]);	/* For speed.  Saves 1 function evaluation. */
    W    = w[3] + h*(55*val3 - 59*val2 + 37*val1 - 9*f(t[0],w[0])) / 24;
				/* Predict w(i) using Equation (5.53) or    */
				/* (5.65) Fourth-order Adams-Bashforth      */
				/* technique (explicit).                    */
    W    = w[3] + h*(9*f(T,W) + 19*val3 - 5*val2 + val1) / 24;
				/* Correct w(i) using Equation (5.54) or    */
				/* (5.68) Fourth-order Adams-Moulton        */
				/* technique (implicit).                    */

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #8 */
    printf2("% .2lf\t% .11lf\n", T, W);

    /* STEP #9 */
    for (j=0;j<=2;j++) {	/* Prepare for next iteration. */
      t[j] = t[j+1];
      w[j] = w[j+1];
    }

    /* STEP #10 */
    t[3] = T;
    w[3] = W;
  }

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}  /* STEP #11 */		/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
