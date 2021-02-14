/******************************************************************************
	Adams Variable Step-Size Predictor-Corrector - Algorithm 5.5
*******************************************************************************

To approximate the solution of the initial-value problem

	y' = f(t,y),  a ó t ó b,  y(a) = à,

with local truncation error within a given tolerance:

INPUT endpoints a, b; initial condition à; tolerance TOL; maximum step size
hmax; minimum step size hmin; the function f().

OUTPUT i, t, w, h where at the ith step w approximates y(t), and step size h
was used or a message that the minimum step size was exceeded.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "055.out";	/* Customized default output file name.     */
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
  double a, b, h, hmax, hmin, tt, t[50], w[50], alpha, q, TOL;
  double f(), WP, WC, sigma, val1, val2, val3, val4;
  int i, j, NFLAG, FLAG, LAST;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Adams Variable Step-Size Predictor-Corrector - Algorithm 5.5\n\n");

  if (eqeval)
    printf2("y' = f(t,y) = %s", tmpstr);/* Print the Equation Evaluator text */
  else
    printf2("%s", eq_text_f);		/* Print the default equation text */
  printf2("\n\n");

  printf("Enter Tolerance TOL (5.0e-6)?: ");
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

  printf2("\n i\t t\t\t h\t\t w ÷ y");	/* Print table header. */
  printf2("\n-------------------------------------------------------\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */

  /*
  **  Set up a subalgorithm for the Runge-Kutta forth-order method to be called
  **  RK4(h,v0,x0,v1,x1,v2,x2,v3,x3), which accepts as input a step size h and
  **  starting values v0 = y(x0) and returns {(x(j),v(j))| j=1,2,3} defined by
  **  the following:
  **  (See RK4() function below)
  */

  /* STEP #2 */
  t[0] = a;
  w[0] = alpha;
  h    = hmax;
  FLAG = 1;	/* FLAG will be used to exit the loop in Step 4.         */
  LAST = 0;	/* LAST will indicate whem the last value is calculated. */
  printf2("% d\t% .9lf\t% .9lf\t% .9lf\n", 0, t[0], h, w[0]);

  /* STEP #3 */
  RK4(h, &t[0], &w[0]);
  NFLAG = 1;			/* Indicates computation from RK4. */
  i     = 4;
  tt    = t[3] + h;

  /* STEP #4 */			/* Ensures that algorithm stops with an */
  while (FLAG == 1) {		/* .. acceptable value.  Do Steps 5-20. */

    /* STEP #5 */
    val1 = f(t[i-1], w[i-1]);	/* For speed.  Saves 1 function evaluation. */
    val2 = f(t[i-2], w[i-2]);	/* For speed.  Saves 1 function evaluation. */
    val3 = f(t[i-3], w[i-3]);	/* For speed.  Saves 1 function evaluation. */
    val4 = f(t[i-4], w[i-4]);	/* For speed.  Saves 1 function evaluation. */
    WP   = w[i-1] + (h/24)*(55*val1 - 59*val2 + 37*val3 - 9*val4);
				/* Predict wi */
    WC   = w[i-1] + (h/24)*(9*f(tt, WP) + 19*val1 - 5*val2 + val3);
				/* Correct wi */
    sigma = 19.0 * fabs(WC-WP) / (270*h);

    /* STEP #6 */
    if (sigma <= TOL) {		/* then do Steps 7-16.  Result accepted.  */
				/* else do Steps 17-19.  Result rejected. */
      /* STEP #7 */
      w[i] = WC;		/* Result accepted.                       */
      t[i] = tt;

      /***********
       * OUTPUTS *
       ***********/

      /* STEP #8 */
      if (NFLAG == 1)
        for (j=i-3;j<=i;j++)	/* Previous results also accepted.    */
          printf2("% d\t% .9lf\t% .9lf\t% .9lf\n", j, t[j], h, w[j]);
        else			/* Previous results already accepted. */
          printf2("% d\t% .9lf\t% .9lf\t% .9lf\n", i, t[i], h, w[i]);

      /* STEP #9 */
      if (LAST == 1) {
        FLAG = 0;
      } else {			/* Do Steps 10-16. */

        /* STEP #10 */
        i++;			/* i = i + 1.      */
        NFLAG = 0;

        /* STEP #11 */
        if ((sigma <= (0.1*TOL)) || (t[i-1] + h > b)) {

	  /*
 	  **  Increase h if it is more accurate than required or decrease h
          **  to include b as a mesh point.  Do Steps 12-16.
	  */

          /* STEP #12 */
          q = pow(TOL/(2.0*sigma), 0.25);

          /* STEP #13 */
          if (q > 4.0)
            h *= 4.0;
          else
            h *= q;

          /* STEP #14 */
          if (h > hmax)
            h = hmax;

          /* STEP #15 */  /* Avoid terminating with change in step size. */
          if ((t[i-1] + 4.0*h) > b) {
            h    = (b - t[i-1]) / 4.0;
            LAST = 1;
          }

          /* STEP #16 */
          RK4(h, &t[i-1], &w[i-1]);
          NFLAG = 1;
          i += 3;
        }
      }
    } else {

      /* STEP #17 */
      q = pow(TOL/(2.0*sigma), 0.25);	/* False branch from Step 6 */
					/* - result rejected.       */        
      /* STEP #18 */
      if (q < 0.1)
        h *= 0.1;
      else
        h *= q;

      /* STEP #19 */
      if (h < hmin) {		/* Procedure fails.                */
        FLAG = 0;
        printf2("hmin exceeded\n");
      } else {
        if (NFLAG == 1)
          i -= 3;		/* Previous results also rejected. */
        RK4(h, &t[i-1], &w[i-1]);
        i += 3;
        NFLAG = 1;
      }
    }

    /* STEP #20 */
    tt = t[i-1] + h;
  }

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}  /* STEP #21 */		/* STOP */

/*****************************************************************************/
/* RK4() - Runge-Kutta (Order Four) method for solving an initial-value      */
/*         differential equation.  See Algorithm 5.2                         */
/*****************************************************************************/
RK4(h, X, V)		/* STEP #1 - Continued */
double h, X[], V[];
{
  double K1, K2, K3, K4, f();
  int j;

  for (j=1;j<=3;j++) {
    K1 = h*f(X[j-1],         V[j-1]);
    K2 = h*f(X[j-1] + h/2.0, V[j-1] + K1/2.0);
    K3 = h*f(X[j-1] + h/2.0, V[j-1] + K2/2.0);
    K4 = h*f(X[j-1] + h,     V[j-1] + K3);

    V[j] = V[j-1] + (K1 + 2.0*K2 + 2.0*K3 + K4)/6.0;
    X[j] = X[0] + j*h;
  }
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
