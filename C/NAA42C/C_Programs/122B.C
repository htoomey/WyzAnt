/******************************************************************************
		Heat Equation Forward-Difference - Algorithm 12.2B
*******************************************************************************

To approximate the solution of the Parabolic partial-differential equation

	ëu(x,t)/ët - àýëýu(x,t)/ëxý = 0,  0 < x < l, 0 < t < T,

subject to the boundry conditions

	u(0,t) = u(l,t) = 0,  0 < t < T,

and the initial conditions

	u(x,0) = f(x),  0 ó x ó l:

INPUT endpoint l; maximum time T; constant à; integers m, N; the functions
u(), and f().

OUTPUT approximations w(i,t(j)) to u(x(i),t(j)) for each i = 1,...,m-1 and
j = 1,...,N.

NOTE: This method is conditionally stable.

NOTE: This algorithm was included as a "Homework Helper."  See p. 632,
Exercise Set 12.3, Problem *** 3 ***.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "122b.out";	/* Customized default output file name.     */
char *eq_text_f = "f(x)   = sin(PI*x)";	/* Needs updating  $  */

double *w, *wprev, f();
double h, k, ll, T, lambda, alpha;
double w_APPROX(), w_TRUE();
int m, N;


/*****************************************************************************/
/* f(x) - Initial condition function, f(x).  Needs updating $.               */
/*****************************************************************************/
double f(x)
double x;
{
  if ((x == 0.0) || (x == ll))
    return (0.0);		/* Boundary conditions. */
  else {
    if (eqeval)
      return (eval_eq(x));	/* Use the Equation Evaluator  */
    else
      return (sin(PI*x));	/* Use the default function.   */
  }
}

/*****************************************************************************/
/* w_TRUE(x,t) - True solution function, w_TRUE(x,t).  Needs updating $.     */
/*****************************************************************************/
double w_TRUE(x,t)
double x,t;
{
  return (exp(-PI*PI*t)*sin(PI*x));
}
/*****************************************************************************/


main()
{
  int i, j;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Heat Equation Forward-Difference - Algorithm 12.2B\n\n");
  printf2("ëu(x,t)/ët - àýëýu(x,t)/ëxý = 0\n\n");

  if (eqeval)
    printf2("f(x) = %s", tmpstr);	/* Print the Equation Evaluator text */
  else
    printf2("%s", eq_text_f);		/* Print the default equation text */
  printf2("\n");

  printf("Enter endpoint l: ");
  scanf("%lf", &ll);
  printf("Enter maximum time, T: ");
  scanf("%lf", &T);
  fprintf(file_id, "u(0,t) = u(l,t) = 0,  0 < t < T = %lG\n", T);
  fprintf(file_id, "u(x,0) = f(x),        0 ó x ó l = %lG\n\n", ll);

  printf("Enter à: ");
  scanf("%lf", &alpha);
  fprintf(file_id, "à = %lG\n", alpha);

  do {
    printf("Enter number of space intervals, m: ");
    scanf("%d", &m);
    if (m <= 0)
      printf("ERROR - n must be greter than zero.\n");
  } while (m <= 0);
  fprintf(file_id, "Number of space intervals, m = %d\n", m);

  do {
    printf("Enter number of time intervals, N: ");
    scanf("%d", &N);
    if (N <= 0)
      printf("ERROR - N must be greter than zero.\n");
  } while (N <= 0);
  fprintf(file_id, "Number of time intervals, N = %d\n\n", N);

  /* Dynamically allocate memory for the needed arrays. */
  w     = dvector(0,m);
  wprev = dvector(0,m);

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  h      = ll/m;		/* x = i*h */
  k      = T/N;			/* t = j*k */
  lambda = (alpha*alpha*k) / (h*h);

  /* STEP #2 */
  for (i=0;i<=m;i++)		/* Initial values. */
    wprev[i] = f(i*h);

  printf2("t = %lG\n", 0.0);
  printf2(" i\t     x(i)\t    w(i)");	/* Print table header. */
  printf2("\n---------------------------------------\n");
  printf2(" %d\t % 8.8lG\t% 8.8lG\n", 0, 0.0, 0.0);
  for (i=1;i<m;i++)
    printf2(" %d\t % 8.8lG\t% 8.8lG\n", i, i*h, wprev[i]);
  printf2(" %d\t % 8.8lG\t% 8.8lG\n\n\n", m, m*h, 0.0);

  /*
  **  Step #4 does away with solving for a tridiagonal matrix if you state
  **  the boundary conditions in w_APPROX().  Much easier to program for.
  */

  /* STEP #3 */
  for (j=0;j<N;j++) {

    /* STEP #4 */
    for (i=0;i<=m;i++)		/* Compute points for t>t0. */
      w[i] = w_APPROX(i);

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #5 */
    printf2("t = %lG\n", (j+1)*k);
    printf2(" i\t     x(i)\t    w(i)");
    printf2("\n---------------------------------------\n");
    printf2(" %d\t % 8.8lG\t% 8.8lG\n", 0, 0.0, 0.0);
    for (i=1;i<m;i++)
      printf2(" %d\t % 8.8lG\t% 8.8lG\n", i, i*h, w[i]);
    printf2(" %d\t % 8.8lG\t% 8.8lG\n\n\n", m, m*h, 0.0);

    /* STEP #6 */
    for (i=0;i<=m;i++)		/* Prepare for next "time" iteration. */
      wprev[i] = w[i];

  }

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(wprev,0,m);
  free_dvector(w,0,m);

  NAA_do_last(outfile);		/* NAA finish-up procedure.      */

}    /* STEP #7 */		/* STOP - Procedure is complete. */

/*****************************************************************************/
/* w_APPROX(i) - Approximates the solution with a difference equation.       */
/*               See Eq. 12.20, p.621                                        */
/*****************************************************************************/
double w_APPROX(i)
int i;
{
  if ((i == 0) || (i == m))	/* Boudary conditions.          */
    return (0.0);
  else				/* Forward Difference Equation. */
    return ((1.0 - 2.0*lambda)*wprev[i] + lambda*(wprev[i+1] + wprev[i-1]));
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
