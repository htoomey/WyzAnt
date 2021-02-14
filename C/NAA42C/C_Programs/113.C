/******************************************************************************
		  Linear Finite-Difference - Algorithm 11.3
*******************************************************************************

To approximate the solution of the linear boundry-value problem

	y'' = p(x)y' + q(x)y + r(x),  a ó x ó b,  y(a) = à,  y(b) = á.

INPUT endpoints a, b; boundry conditions à, á; number of subintervals N
or step size h; the functions p(), q(), and r().

OUTPUT approximations w(i) to y(xi); w2,i to y'(x(i)) for each i =
0,1,...,N + 1. 

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "113.out";	/* Customized default output file name.     */
char *eq_text_p = "p(x) = -2.0/x";		/* Needs updating  $  */
char *eq_text_q = "q(x) = 2.0/(x*x)";		/* Needs updating  $  */
char *eq_text_r = "r(x) = sin(log(x))/(x*x)";	/* Needs updating  $  */


/*****************************************************************************/
/* p(x) - Function p(x) to evaluate.  Needs updating $.                      */
/*****************************************************************************/
double p(x)
double x;
{
  return (-2.0/x);
}

/*****************************************************************************/
/* q(x) - Function q(x) to evaluate.  Needs updating $.                      */
/*****************************************************************************/
double q(x)
double x;
{
  return (2.0/(x*x));
}

/*****************************************************************************/
/* r(x) - Function r(x) to evaluate.  Needs updating $.                      */
/*****************************************************************************/
double r(x)
double x;
{
  return (sin(log(x))/(x*x));
}
/*****************************************************************************/


main()
{
  double *aa, *bb, *c, *d, *l, *u, *w, *z, p(), q(), r();
  double a, b, alpha, beta, h, x;
  int i, N, ch;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Linear Finite-Difference - Algorithm 11.3\n\n");
  printf2("y''  = p(x)y' + q(x)y + r(x)\n");
  printf2("%s\n", eq_text_p);
  printf2("%s\n", eq_text_q);
  printf2("%s\n\n", eq_text_r);

  printf("Enter endpoint a: ");
  scanf("%lf", &a);
  printf("Enter endpoint b: ");
  scanf("%lf", &b);
  fprintf(file_id, "Solving for points on interval [%lg,%lg].\n", a, b);

  printf("Enter boundry condition y(%lg) = à: ", a);
  scanf("%lf", &alpha);
  fprintf(file_id, "Boundry condition y(%lg) = à = %lg\n", a, alpha);

  printf("Enter boundry condition y(%lg) = á: ", b);
  scanf("%lf", &beta);
  fprintf(file_id, "Boundry condition y(%lg) = á = %lg\n", b, beta);

  printf("Do you want to enter N or h? H: ");
  getchar();			/* Cleans up after scanf(). */
  ch = tolower(getchar());
  if (ch == 'n') {
    printf("Enter number of subintervals N: ");
    scanf("%d", &N);
    h = (b - a)/(N + 1);	/* STEP #1 */
  } else {			/* default */
    printf("Enter interval number h: ");
    scanf("%lf", &h);
    N = (int) ceil((b - a)/h) - 1;
  }

  printf2("Number of subintervals N = %d\n", N);
  printf2("Interval number h = %lg\n", h);

  /* Dynamically allocate memory for the needed arrays. */
  aa = dvector(1,N);
  bb = dvector(1,N);
  c  = dvector(1,N);
  d  = dvector(1,N);
  l  = dvector(1,N);
  u  = dvector(1,N);
  w  = dvector(0,N+1);
  z  = dvector(1,N);

  printf2("\n x\t w\n");		/* Print table header. */
  printf2("-----------------------\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  x     = a + h;
  aa[1] = 2.0 + h*h*q(x);
  bb[1] = -1.0 + (h/2.0)*p(x);
  d[1]  = -h*h*r(x) + (1.0 + (h/2.0)*p(x))*alpha;

  /* STEP #2 */
  for (i=2;i<N;i++) {
    x     = a + i*h;
    aa[i] = 2.0 + h*h*q(x);
    bb[i] = -1.0 + (h/2.0)*p(x);
    c[i]  = -1.0 - (h/2.0)*p(x);
    d[i]  = -h*h*r(x);
  }

  /* STEP #3 */
  x     = b - h;
  aa[N] = 2.0 + h*h*q(x);
  c[N]  = -1.0 - (h/2.0)*p(x);
  d[N]  = -h*h*r(x) + (1.0 - (h/2.0)*p(x))*beta;

  /* STEP #4 */
  l[1] = aa[1];			/* Steps 4-10 solve a tridiagonal linear */
  u[1] = bb[1] / aa[1];		/* system using Algorithm 6.7.           */
  
  /* STEP #5 */
  for (i=2;i<N;i++) {
    l[i] = aa[i] - c[i]*u[i-1];
    u[i] = bb[i] / l[i];
  }

  /* STEP #6 */
  l[N] = aa[N] - c[N]*u[N-1];

  /* STEP #7 */
  z[1] = d[1] / l[1];

  /* STEP #8 */
  for (i=2;i<=N;i++)
    z[i] = (d[i] - c[i]*z[i-1]) / l[i];

  /* STEP #9 */
  w[0]   = alpha;
  w[N+1] = beta;
  w[N]   = z[N];

  /* STEP #10 */
  for (i=N-1;i>=1;i--)
    w[i] = z[i] - u[i]*w[i+1];

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #11 */
  for (i=0;i<=N+1;i++) {
    x = a + i*h;
    printf2("% .2lf\t% .9lf\n", x, w[i]);
  }

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(z,1,N);
  free_dvector(w,0,N+1);
  free_dvector(u,1,N);
  free_dvector(l,1,N);
  free_dvector(d,1,N);
  free_dvector(c,1,N);
  free_dvector(bb,1,N);
  free_dvector(aa,1,N);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}  /* STEP #12 */		/* STOP - Procedure is complete. */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
