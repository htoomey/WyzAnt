/******************************************************************************
		      Runge-Kutta-Fehlberg - Algorithm 5.3
*******************************************************************************

To approximate the solution of the initial-value problem

	y' = f(t,y),  a ó t ó b,  y(a) = à,

with local truncation error within a given tolerance:

INPUT endpoints a, b; initial condition à; tolerance TOL; maximum step size
hmax; minimum step size hmin; the functions f() and y(t) (if known).

OUTPUT t, w, h where w approximates y(t) and step size h was used or a message
that the minimum step size was exceeded.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "053.out";	/* Customized default output file name.     */
char *eq_text_f = "y' = f(t,y) = -y + t + 1";	/* Needs updating  $  */
char *eq_text_y = "y  = y(t)   = t + exp(-t)";	/* Needs updating  $  */


/*****************************************************************************/
/* f(t,y) - Function to evaluate, y' = f(t,y).  Needs updating $.            */
/*****************************************************************************/
double f(t,y)
double t, y;
{
  return (-y + t + 1);
}

/*****************************************************************************/
/* y(t) - Actual solution, y(t), if known.  Needs updating $.                */
/*****************************************************************************/
double y(t)
double t;
{
  return (t + exp(-t));
}
/*****************************************************************************/


main()
{
  double a, b, alpha, hmax, hmin, h, t, w, R, delta, TOL;
  double f(), y(), K1, K2, K3, K4, K5, K6;
  int i, FLAG;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Runge-Kutta-Fehlberg - Algorithm 5.3\n\n");
  printf2("%s\n", eq_text_y);
  printf2("%s\n\n", eq_text_f);

  printf("Enter Tolerance TOL (5.0e-5)?: ");
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

  printf("Enter minimum interval number, hmin: ");
  scanf("%lf", &hmin);
  fprintf(file_id, "Minimum interval number, hmin = %lg\n", hmin);

  printf("Enter maximum interval number, hmax: ");
  scanf("%lf", &hmax);
  fprintf(file_id, "Maximum interval number, hmax = %lg\n", hmax);

				/* Print table header. */
  printf2("\n i\t t\t   w ÷ y\t h\t\t R\t\t y(t)\t\t |y(t) - w|\n");
  printf2("---------------------------------------------");
  printf2("----------------------------------------------\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  t = a;
  w = alpha;
  h = hmax;
  i = 0;
  FLAG = 1;
  printf2("% d\t% lf % .9lf\t% .8lf\t% .3le\t% le\t% 5.3le\n",
	i, t, alpha, h, 0.0, alpha, 0.0);
  i++;				/* i = i + 1.     */

  /* STEP #2 */
  while (FLAG == 1) {		/* Do Steps 3-11. */

    /* STEP #3 */
    K1 = h*f(t,           w);
    K2 = h*f(t + h/4,     w + K1/4);
    K3 = h*f(t + 3*h/8,   w + 3*K1/32 + 9*K2/32);
    K4 = h*f(t + 12*h/13, w + 1932*K1/2197 -7200*K2/2197 + 7296*K3/2197);
    K5 = h*f(t + h,       w + 439*K1/216 - 8*K2 + 3680*K3/513 - 845*K4/4104);
    K6 = h*f(t + h/2,     w - 8*K1/27 + 2*K2 - 3544*K3/2565
				+ 1859*K4/4104 - 11*K5/40);

    /* STEP #4 */		/* Note R = |~w(i+1) - w(i+1)| / h */
    R = fabs(K1/360 - 128*K3/4275 - 2197*K4/75240 + K5/50 + 2*K6/55) / h;

    /* STEP #5 */
    delta = 0.84 * pow(TOL/R, 0.25);

    /* STEP #6 */
    if (R <= TOL) {		/* Do Steps 7-8. */

      /* STEP #7 */
      t += h;			/* Approximation accepted. */
      w += 25*K1/216 + 1408*K3/2565 + 2197*K4/4104 - K5/5;

      /***********
       * OUTPUTS *
       ***********/

      /* STEP #8 */
      printf2("% d\t% lf % .9lf\t% .8lf\t% .3le\t% le\t% 5.3le\n",
		i, t, w, h, R, y(t), fabs(y(t) - w));
      i++;			/* i = i + 1. */
    }

    /* STEP #9 */
    if (delta <= 0.1)
      h = 0.1 * h;
    else {
      if (delta >= 4.0)
        h *= 4.0;
      else
        h *= delta;		/* Calculate new h. */
    }

    /* STEP #10 */
    if (h > hmax)
      h = hmax;

    /* STEP #11 */
    if (t >= b)
      FLAG = 0;
    else if (t + h > b)
      h = b - t;
    else if (h < hmin) {
      printf2("Minimum h exceeded.\n");
      NAA_do_last(outfile);	/* NAA finish-up procedure.            */
      exit (-1);		/* Procedure completed unsuccessfully. */
    }
  }

  NAA_do_last(outfile);		/* NAA finish-up procedure.   */

				/* The procedure is complete. */
}  /* STEP #12 */		/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
