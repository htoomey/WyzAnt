/******************************************************************************
		Heat Equation Backward-Difference - Algorithm 12.2
*******************************************************************************

To approximate the solution of the Parabolic partial-differential equation

	�u(x,t)/�t - alpha���u(x,t)/�x� = 0,  0 < x < l, 0 < t < T,

subject to the boundry conditions

	u(0,t) = u(l,t) = 0,  0 < t < T,

and the initial conditions

	u(x,0) = f(x),  0 � x � l:

INPUT endpoint l; maximum time T; constant �; integers m, N; the functions
u(), and f().

OUTPUT approximations w(i,t(j)) to u(x(i),t(j)) for each i = 1,...,m-1 and j =
1,...,N.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "122.out";	/* Customized default output file name.     */
char *eq_text_f = "f(x)   = sin(PI*x)";	/* Needs updating  $  */


/*****************************************************************************/
/* f(x) - Initial condition function, f(x).  Needs updating $.               */
/*****************************************************************************/
double f(x)
double x;
{
  if (eqeval)
    return (eval_eq(x));	/* Use the Equation Evaluator  */
  else
    return (sin(PI*x));		/* Use the default function.   */
}
/*****************************************************************************/


main()
{
  double *l, *u, *w, *z, f();
  double h, k, ll, t, T, lambda, alpha;
  int i, j, m, N;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Heat Equation Backward-Difference - Algorithm 12.2\n\n");
  printf2("�u(x,t)/�t - ����u(x,t)/�x� = 0\n\n");

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
  fprintf(file_id, "u(x,0) = f(x),        0 � x � l = %lG\n\n", ll);

  printf("Enter �: ");
  scanf("%lf", &alpha);
  fprintf(file_id, "� = %lG\n", alpha);

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
  l = dvector(1,m-1);
  u = dvector(1,m-1);
  w = dvector(1,m-1);
  z = dvector(1,m-1);

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  h      = ll/m;
  k      = T/N;
  lambda = (alpha*alpha*k) / (h*h);

  /* STEP #2 */
  for (i=1;i<m;i++)
    w[i] = f(i*h);		/* Initial values. */

  printf2("t = %lG\n", 0.0);
  printf2(" i\t     x(i)\t    w(i)");	/* Print table header. */
  printf2("\n---------------------------------------\n");
  printf2(" %d\t % 8.8lG\t% 8.8lG\n", 0, 0.0, 0.0);
  for (i=1;i<m;i++)
    printf2(" %d\t % 8.8lG\t% 8.8lG\n", i, i*h, w[i]);
  printf2(" %d\t % 8.8lG\t% 8.8lG\n\n\n", m, m*h, 0.0);

  /* Steps 3-11 solve a tridiagonal linear system using Algorithm 6.7. */
  /* STEP #3 */
  l[1] = 1.0 + 2.0*lambda;
  u[1] = -lambda/l[1];

  /* STEP #4 */
  for (i=2;i<=m-2;i++) {
    l[i] = 1.0 + 2.0*lambda + lambda*u[i-1];
    u[i] = -lambda/l[i];
  }

  /* STEP #5 */
  l[m-1] = 1.0 + 2.0*lambda + lambda*u[m-2];

  /* STEP #6 */
  for (j=1;j<=N;j++) {		/* Do Steps 7-11. */

    /* STEP #7 */
    t    = j*k;			/* Current t(j). */
    z[1] = w[1]/l[1];

    /* STEP #8 */
    for (i=2;i<m;i++)
      z[i] = (w[i] + lambda*z[i-1]) / l[i];

    /* STEP #9 */
    w[m-1] = z[m-1];

    /* STEP #10 */
    for (i=m-2;i>=1;i--)
      w[i] = z[i] - u[i]*w[i+1];

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #11 */
    printf2("t = %lG\n", t);
    printf2(" i\t     x(i)\t    w(i)");
    printf2("\n---------------------------------------\n");
    printf2(" %d\t % 8.8lG\t% 8.8lG\n", 0, 0.0, 0.0);
    for (i=1;i<m;i++)
      printf2(" %d\t % 8.8lG\t% 8.8lG\n", i, i*h, w[i]);
    printf2(" %d\t % 8.8lG\t% 8.8lG\n\n\n", m, m*h, 0.0);
  }

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(z,1,m-1);
  free_dvector(w,1,m-1);
  free_dvector(u,1,m-1);
  free_dvector(l,1,m-1);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}  /* STEP #12 */		/* STOP - Procedure is complete. */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
