/******************************************************************************
		Nonlinear Finite-Difference - Algorithm 11.4
*******************************************************************************

To approximate the solution of the nonlinear boundry-value problem

	y'' = f(x,y,y'),  a ó x ó b,  y(a) = à,  y(b) = á.

INPUT endpoints a, b; boundry conditions à, á; number of subintervals N
or step size h; tolerance TOL; maximum number of iterations M; the functions
f(), fy() = ëf/ëy, fyprime() = ëf/ëy' and y() (if known):

OUTPUT approximations w(i) to y(x(i)) for each i = 0,1,...,N+1 or a message
that the maximum number of iterations was exceeded.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "114.out";	/* Customized default output file name.     */
				/* Needs updating  $  */
char *eq_text_y2prime = "y'' = f(x,y,y') = (32.0 + 2.0*x*x*x - y*y')/8.0";
char *eq_text_fy      = "fy  = ëf(x,y,y')/ëy = -y'/8.0";
char *eq_text_fyprime = "fy' = ëf(x,y,y')/ëy' = -y/8.0";
char *eq_text_y       = "y   = x*x + 16.0/x";	/* OPTIONAL */
int N;


/*****************************************************************************/
/* f(x,y,yprime) - Equation for y''.  Needs updating $.                      */
/*****************************************************************************/
double f(x,y,yprime)
double x,y,yprime;
{
  return ((32.0 + 2.0*x*x*x - y*yprime)/8.0);
}

/*****************************************************************************/
/* fy(x,y,yprime) - Equation for dy''/dy.  Needs updating $.                 */
/*****************************************************************************/
double fy(x,y,yprime)
double x,y,yprime;
{
  return (-yprime/8.0);
}

/*****************************************************************************/
/* fyprime(x,y,yprime) - Equation for dy''/dy'.  Needs updating $.           */
/*****************************************************************************/
double fyprime(x,y,yprime)
double x,y,yprime;
{
  return (-y/8.0);
}

/*****************************************************************************/
/* y(x) - Actual solution, y.  May not be known.  Needs updating $.          */
/*****************************************************************************/
double y(x)
double x;
{
  return (x*x + 16.0/x);
}
/*****************************************************************************/


main()
{
  double *aa, *bb, *c, *d, *l, *u, *v, *w, *z, f(), fy(), fyprime(), y();
  double a, b, alpha, beta, h, t, x, TOL, euclidean_norm();
  int i, k, ch, M;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Nonlinear Finite-Difference - Algorithm 11.4\n\n");
  printf2("%s\n", eq_text_y2prime);
  printf2("%s\n", eq_text_fy);
  printf2("%s\n", eq_text_fyprime);
  printf2("%s\n\n", eq_text_y);

  printf("Enter maximum number of iterations, M: ");
  scanf("%d", &M);
  fprintf(file_id, "Maximum number of iterations: %d\n", M);

  printf("Enter the tolerance, TOL (1.0e-5): ");
  scanf("%lf", &TOL);
  fprintf(file_id, "Tolerance = %lg\n", TOL);

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
  v  = dvector(1,N);
  w  = dvector(0,N+1);
  z  = dvector(1,N);

				/* Print table header. */
  printf2("\n x\t w\t\t y(x)\t\t|w1 - y(x)|\n");
  printf2("---------------------------------------------------\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  w[0]   = alpha;
  w[N+1] = beta;

  /* STEP #2 */
  for (i=1;i<=N;i++)
    w[i] = alpha + i*((beta-alpha)/(b-a))*h;

  /* STEP #3 */
  k = 1;

  /* STEP #4 */
  while (k <= M) {		/* Do Steps 5-18. */

    /* STEP #5 */
    x     = a + h;
    t     = (w[2] - alpha) / (2.0*h);
    aa[1] = 2.0 + h*h*fy(x,w[1],t);
    bb[1] = -1.0 + (h/2.0)*fyprime(x,w[1],t);
    d[1]  = -(2.0*w[1] - w[2] - alpha + h*h*f(x,w[1],t));

    /* STEP #6 */
    for (i=2;i<N;i++) {
      x     = a + i*h;
      t     = (w[i+1] - w[i-1]) / (2.0*h);
      aa[i] = 2.0 + h*h*fy(x,w[i],t);
      bb[i] = -1.0 + (h/2.0)*fyprime(x,w[i],t);
      c[i]  = -1.0 - (h/2.0)*fyprime(x,w[i],t);
      d[i]  = -(2.0*w[i] - w[i+1] - w[i-1] + h*h*f(x,w[i],t));
    }

    /* STEP #7 */
    x     = b - h;
    t     = (beta - w[N-1]) / (2.0*h);
    aa[N] = 2.0 + h*h*fy(x,w[N],t);
    c[N]  = -1.0 - (h/2.0)*fyprime(x,w[N],t);
    d[N]  = -(2.0*w[N] - w[N-1] - beta + h*h*f(x,w[N],t));

    /* STEP #8 */
    /* Steps 8-14 solve a tridiagonal linear system using Algorithm 6.7. */    
    l[1] = aa[1];
    u[1] = bb[1] / aa[1];

    /* STEP #9 */
    for (i=2;i<N;i++) {
      l[i] = aa[i] - c[i]*u[i-1];
      u[i] = bb[i] / l[i];
    }

    /* STEP #10  */
    l[N] = aa[N] - c[N]*u[N-1];

    /* STEP #11 */
    z[1] = d[1] / l[1];

    /* STEP #12 */
    for (i=2;i<=N;i++)
      z[i] = (d[i] - c[i]*z[i-1]) / l[i];

    /* STEP #13 */
    v[N] = z[N];
    w[N] += v[N];

    /* STEP #14 */
    for (i=N-1;i>=1;i--) {
      v[i] = z[i] - u[i]*v[i+1];
      w[i] += v[i];
    }

    /* STEP #15 */
    if (euclidean_norm(v) <= TOL) {	/* Do Steps 16-17. */

      /***********
       * OUTPUTS *
       ***********/

      /* STEP #16 */
      for (i=0;i<=N+1;i++) {
        x = a + i*h;
        printf2("% .2lf\t% .9lf\t% .9lf\t% .2lE\n",
		 x, w[i], y(x), fabs(w[i] - y(x)));
      }
      printf2("\nSolution took %d iterations for requested tolerance.\n", k);

      /* Free the memory that was dynamically allocated for the arrays. */
      free_dvector(z,1,N);
      free_dvector(w,0,N+1);
      free_dvector(v,1,N);
      free_dvector(u,1,N);
      free_dvector(l,1,N);
      free_dvector(d,1,N);
      free_dvector(c,1,N);
      free_dvector(bb,1,N);
      free_dvector(aa,1,N);

      /* STEP #17 */
      NAA_do_last(outfile);	/* NAA finish-up procedure. */
      exit (1);			/* STOP - Procedure completed successfully. */
    }

    /* STEP 18 */
    k++;			/* k = k + 1. */

  }

  /* STEP #19 */
  printf2("\nMaximum number of iterations (%d) exceeded.\n", M);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(z,1,N);
  free_dvector(w,0,N+1);
  free_dvector(v,1,N);
  free_dvector(u,1,N);
  free_dvector(l,1,N);
  free_dvector(d,1,N);
  free_dvector(c,1,N);
  free_dvector(bb,1,N);
  free_dvector(aa,1,N);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP - Procedure completed unsuccessfully */

/****************************************************************************/
/* euclidean_norm() - Computes the euclidean norm of a vector.              */
/****************************************************************************/
double euclidean_norm(X)
double *X;
{
  double sum_of_sqrs = 0.0;
  int i;

  for (i=1;i<=N;i++)
    sum_of_sqrs += (X[i] * X[i]);

  return (sqrt(sum_of_sqrs));
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
