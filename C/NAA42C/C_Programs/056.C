/******************************************************************************
			Extrapolation - Algorithm 5.6
*******************************************************************************

To approximate the solution of the initial-value problem

	y' = f(t,y),  a ó t ó b,  y(a) = à,

with local truncation error within a given tolerance:

INPUT endpoints a, b; initial condition à; tolerance TOL; level limit
p ó 8; maximum step size hmax; minimum step size hmin; the function f().

++++++++++++++++++++++++++++ COMPILING HINTS +++++++++++++++++++++++++++++++++
To compile this program you may need to increase the stack size.  On an IBM PC
using Microsoft C 5.0, compile this program with the stack switch /F.

	Example: CL 056.C /F 1000        (For a 4Kbyte stack in hex)
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

OUTPUT T, W, h where W approximates y(t) and step size h was used or a message
that the minimum step size was exceeded.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "056.out";	/* Customized default output file name.     */
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

double NK[9] = { 0.0, 2.0, 3.0, 4.0, 6.0, 8.0, 12.0, 16.0, 24.0 };


main()
{
  double a, b, h, hmax, hmin, y[9], alpha, v, TOL;
  double f(), W0, W1, W2, W3, T, T0, HK;
  double Q[8][8];
  int i, j, k, p=8, FLAG, NFLAG;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Extrapolation - Algorithm 5.6\n\n");

  if (eqeval)
    printf2("y' = f(t,y) = %s", tmpstr);/* Print the Equation Evaluator text */
  else
    printf2("%s", eq_text_f);		/* Print the default equation text */
  printf2("\n\n");

  printf("Enter Tolerance TOL (1.0e-5)?: ");
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

  /* Print first row of table. */
  printf2("% d\t% .9lf\t% .9lf\t% .9lf\n", 0, a, hmax, alpha);

  /* STEP #1 */
  /* Already initilaized above main() as NK[]. */

  /* STEP #2 */
  T0   = a;
  W0   = alpha;
  h    = hmax;
  FLAG = 1;			/* FLAG is used to exit the loop in STEP #4. */

  /* STEP #3 */
  for (i=1;i<=7;i++)
    for (j=1;j<=i;j++)
      Q[i][j] = pow((NK[i+1]/NK[j]), 2.0);

  i = 1;			/* Counts iterations. */

  /* STEP #4 */
  while (FLAG == 1) {		/* Do Steps 5-20.     */

    /* STEP #5 */
    k     = 1;
    NFLAG = 0;	/* When desired accuracy is achieved NFLAG is set to 1. */

    /* STEP #6 */
    while ((k <= 8) && (NFLAG == 0)) {	/* Do Steps 7-14. */

      /* STEP #7 */
      HK = h/NK[k];
      T  = T0;
      W2 = W0;
      W3 = W2 + HK*f(T, W2);		/* Euler first step. */
      T  = T0 + HK;

      /* STEP #8 */
      for (j=1;j<=NK[k]-1;j++) {
        W1 = W2;
        W2 = W3;
        W3 = W1 + 2.0*HK*f(T, W2);	/* Midpoint method.  */
        T  = T0 + (j + 1)*HK;
      }

      /* STEP #9 */		/* Smoothing to compute y[k][1]. */
      y[k] = (W3 + W2 + HK*f(T, W3)) / 2.0;

      /* STEP #10 */
      if (k >= 2) {		/* Do steps 11-13. */

      /*
      **  Note: y[k-1] ð y[k-1,1] y[k-2,2,...,y1] ð y[k-1,k-1] since only
      **        previous row of table is saved.
      */

        /* STEP #11 */
        j = k;
        v = y[1];

        /* STEP #12 */
        while (j >= 2) {
          y[j-1] = y[j] + (y[j] - y[j-1]) / (Q[k-1][j-1] - 1.0);

		/*
		**  Extrapolation to compute y[j-1] ð y[k][k-j+2].
		**
		**  Note:             (h[j-]^2 * y[j] - h[k]^2 * y[j-1])
                **           y[j-1] = ----------------------------------
                **                            (h[j-1]^2 - h[k]^2)
		*/

          j--;			/* j = j - 1. */
        }

        /* STEP #13 */
        if (fabs(y[1] - v) <= TOL)
          NFLAG = 1;		/* y[1] accepted as new w. */
      }

      /* STEP #14 */
      k++;			/* k = k + 1. */
    }

    /* STEP #15 */
    k--;			/* k = k - 1. */

    /* STEP #16 */
    if (NFLAG == 0) {		/* Then do Steps 17 and 18. (Result rejected)*/
				/* Else do Steps 19 and 20. (Result accepted)*/
      /* STEP #17 */
      h /= 2.0;			/* New value for w rejected, decrease h.     */

      /* STEP #18 */
      if (h < hmin) {
        printf2("hmin (= %lg) exceeded.\n", hmin);
        FLAG = 0;
      }
    } else {

      /***********
       * OUTPUTS *
       ***********/

      /* STEP #19 */
      W0 = y[1];		/* New value for w accepted.          */
      T0 += h;
      printf2("% d\t% .9lf\t% .9lf\t% .9lf\n", i, T0, h, W0);
      i++;			/* i = i + 1.                         */

      /* STEP #20 */		/* Increase h if possible.            */
      if (fabs(T0 - b) < 0.5*hmin)
        FLAG = 0;		/* Proceedure completed successfully. */
      else if (T0 + h > b)
        h = b - T0;		/* Terminate at t = b.                */
      else if ((k <= 3) && (h < 0.5*hmax))
        h *= 2.0;		/* Increase step size if possible.    */
    }
  }

  NAA_do_last(outfile);		/* NAA finish-up procedure.           */

}  /* STEP #21 */		/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
