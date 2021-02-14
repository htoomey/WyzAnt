/******************************************************************************
		Trapazoidal with Newton Iteration - Algorithm 5.8
*******************************************************************************

To approximate the solution of the initial-value problem

	y' = f(t,y),  a ó t ó b,  y(a) = à,

at (N + 1) equally spaced numbers in the interval [a,b]:

INPUT endpoints a, b; integer N or interval h; initial condition à;
tolerance TOL; maximum number of iterations M at any one step; the functions
f() and fy() = df()/dy.

OUTPUT approximation w to y at the (N + 1) values of t or a message of failure.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile    = "058.out";	/* Customized default output file name.     */
char *eq_text_f  = "y'  = f(t,y)  = -5*y";
				/* Needs updating  $  */
char *eq_text_fy = "y'' = fy(t,y) = df()/dy = -5";
				/* Needs updating  $  */


/*****************************************************************************/
/* f(t,y) - Function to evaluate, y' = f(t,y).  Needs updating $.            */
/*****************************************************************************/
double f(t, y)
double t, y;
{
  return (-5*y);
}

/*****************************************************************************/
/* fy(t,y) - Function to evaluate, y'' = fy() = df()/dy.  Needs updating $.  */
/*****************************************************************************/
double fy(t, y)
double t, y;
{
  return (-5.0);
}
/*****************************************************************************/


main()
{
  double a, b, h, t, w0, w, alpha, k1, TOL;
  double f(), fy();
  int M, N, i, j, ch, FLAG;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Trapazoidal with Newton Iteration - Algorithm 5.8\n\n");
  printf2("%s\n", eq_text_f);
  printf2("%s\n\n", eq_text_fy);

  printf("Enter maximum number of iterations at any one step, M: ");
  scanf("%d", &M);
  fprintf(file_id, "Maximum number of iterations at any one step: %d\n", M);

  printf("Enter Tolerance TOL (1.0e-4)?: ");
  scanf("%lf", &TOL);
  fprintf(file_id, "Tolerance: %lg\n", TOL);

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

  printf2("\n t\t  w ÷ y");	/* Print table header. */
  printf2("\n------------------------\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  t = a;
  w = alpha;
  printf2("% .2lf\t% .11lf\n", t, w);

  /* STEP #2 */
  for (i=1;i<=N;i++) {		/* Do Steps 3-7. */

    /* STEP #3 */
    k1   = w + h*f(t,w)/2.0;
    w0   = k1;
    j    = 1;
    FLAG = 0;

    /* STEP #4 */
    while (FLAG == 0) {		/* Do Steps 5-6. */

      /* STEP #5 */
      w = w0 - (w0 - h*f(t + h,w0) / 2.0 - k1) / (1.0 - h*fy(t + h,w0) / 2.0);

      /* STEP #6 */
      if (fabs(w - w0) < TOL)
        FLAG = 1;
      else {
        j++;			/* j = j + 1. */
        w0 = w;
        if (j > M) {
          printf2("Maximum number of iterations exceeded (%d).\n", M);
          NAA_do_last(outfile);	/* NAA finish-up procedure. */
          exit (-1);		/* STOP */
        }
      }
    }

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #7 */
    t = a + i*h;
    printf2("% .2lf\t% .11lf\n", t, w);
  }

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}  /* STEP #8 */		/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
