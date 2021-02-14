/******************************************************************************
		Wave Equation Finite-Difference - Algorithm 12.4
*******************************************************************************

To approximate the solution of the wave equation

	ëýu(x,t)/ëtý - àýëýu(x,t)/ëxý = 0,  0 < x < l, 0 < t < T,

subject to the boundry conditions

	u(0,t) = u(l,t) = 0,  0 < t < T,

and the initial conditions

	u(x,0) = f(x),      0 ó x ó l:

	ëu(x,0)/ët = g(x),  0 ó x ó l;

INPUT endpoint l; maximum time T; constant à; integers m, N; the
functions f(), and g().

OUTPUT approximations w(i,j) to u(x(i),t(j)) for each i = 0,...,m and
j = 0,...,N.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "124.out";	/* Customized default output file name.     */
char *eq_text_f = "f(x)   = sin(PI*x)";	/* Needs updating  $  */
char *eq_text_g = "g(x)   = 0.0";	/* Needs updating  $  */


/*****************************************************************************/
/* f(x) - Initial condition function, f(x).  Needs updating $.               */
/*****************************************************************************/
double f(x)
double x;
{
  return (sin(PI*x));
}

/*****************************************************************************/
/* g(x) - Initial condition function, g(x).  Needs updating $.               */
/*****************************************************************************/
double g(x)
double x;
{
  return (0.0);
}
/*****************************************************************************/


main()
{
  double **w, f(), g();
  double h, k, l, T, lambda, alpha;
  int i, j, m, N;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Wave Equation Finite-Difference - Algorithm 12.4\n\n");
  printf2("ëu(x,t)/ët - àýëýu(x,t)/ëxý = 0\n\n");
  printf2("%s\n", eq_text_f);
  printf2("%s\n", eq_text_g);

  printf("Enter endpoint l: ");
  scanf("%lf", &l);
  printf("Enter maximum time, T: ");
  scanf("%lf", &T);
  fprintf(file_id, "u(0,t) = u(l,t) = 0,  0 < t < T = %lG\n", T);
  fprintf(file_id, "u(x,0) = f(x),        0 ó x ó l = %lG\n", l);
  fprintf(file_id, "ëu(x,0)/ët = g(x),    0 ó x ó l = %lG\n\n", l);

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
  w = dmatrix(0,m,0,N);

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  h      = l/m;
  k      = T/N;
  lambda = (k*alpha) / h;

  /* STEP #2 */
  for (j=1;j<=N;j++) {
    w[0][j] = 0.0;
    w[m][j] = 0.0;
  }

  /* STEP #3 */
  w[0][0] = f(0.0);
  w[m][0] = f(l);

  /* STEP #4 */
  for (i=1;i<m;i++) {		/* Initialize for t=0 and t=k. */
    w[i][0] = f(i*h);
    w[i][1] = (1.0 - lambda*lambda)*w[i][0] + (lambda*lambda/2.0)*(f((i+1)*h)
		+ f((i-1)*h)) + k*g(i*h);
  }

  /* STEP #5 */
  for (j=1;j<N;j++)		/* Perform matrix multiplication. */
    for (i=1;i<m;i++)
      w[i][j+1] = 2.0*(1.0-lambda*lambda)*w[i][j] + lambda*lambda*(w[i+1][j] +
		w[i-1][j]) - w[i][j-1];

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #6 */
  for (j=0;j<=N;j++) {
    printf2("t = %lG\nj = %d\n", j*k, j);
    printf2(" i\t     x(i)\t    w(i,j)");	/* Print table header. */
    printf2("\n---------------------------------------\n");
    for (i=0;i<=m;i++)
      printf2(" %d\t % 8.8lG\t% 10.10lG\n", i, i*h, w[i][j]);
    printf2("\n\n");
  }

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dmatrix(w,0,m,0,N);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}  /* STEP #7 */		/* STOP - Procedure is complete. */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
