/******************************************************************************
		Nonlinear Finite-Difference - Algorithm 11.4B
		      (Uses Richardson's Extrapolation)
*******************************************************************************

To approximate the solution of the nonlinear boundry-value problem

	y'' = f(x,y,y'),  a ó x ó b,  y(a) = à,  y(b) = á.

INPUT endpoints a, b; boundry conditions à, á; number of subintervals N
or step size h; tolerance TOL; maximum number of iterations M; the functions
f(), fy() = ëf/ëy, fyprime() = ëf/ëy' and y() (if known):

OUTPUT approximations w(i) to y(x(i)) for each i = 0,1,...,N + 1 or a message
that the maximum number of iterations was exceeded.

NOTE: This algorithm was included as a "Homework Helper."  See p. 586,
Exercise Set 11.4, Problem *** 3 ***.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "114b.out";	/* Customized default output file name.     */
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


main()
{
  double *aa, *bb, *c, *d, *l, *u, *v, **w, *z, f(), fy(), fyprime();
  double a, b, alpha, beta, h, h0, t, x, TOL, euclidean_norm(), **Ext;
  int i, j, k, m=0, N0, ch, M;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Nonlinear Finite-Difference - Algorithm 11.4B\n\n");
  printf2("Uses Richardson's Extrapolation for O(h^6) accuracy.\n\n");
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

				/* Print table header. */
  printf2("\n x\t w(h)\t\t w(h/2)\t\t w(h/4)\t\t Ext1\t\t Ext2\t\t Ext3\n");
  printf2("----------------------------------------------------");
  printf2("---------------------------------------------------\n");

  /* Dynamically allocate memory for the needed arrays. */
  aa  = dvector(0,5*N);
  bb  = dvector(0,5*N);
  c   = dvector(0,5*N);
  d   = dvector(0,5*N);
  l   = dvector(0,5*N);
  u   = dvector(0,5*N);
  v   = dvector(0,5*N);
  w   = dmatrix(0,3,0,5*(N+1));
  z   = dvector(0,5*N);
  Ext = dmatrix(1,3,0,N+1);

  N0 = N;			/* Save initial h and N values. */
  h0 = h;

  for (j=1;j<=3;j++) {
  /* For generating h, h/2, and h/4 used in Richardson's extrapolation. */

    h = h0 / (1<<j-1);
    N = (int) ceil((b - a)/h) - 1;

    /*************
     * ALGORITHM *
     *************/

    /* STEP #1 */
    w[j][0]   = alpha;
    w[j][N+1] = beta;

    /* STEP #2 */
    for (i=1;i<=N;i++)
      w[j][i] = alpha + i*((beta-alpha)/(b-a))*h;

    /* STEP #3 */
    k = 1;

    /* STEP #4 */
    while (k <= M) {		/* Do Steps 5-18. */

      /* STEP #5 */
      x     = a + h;
      t     = (w[j][2] - alpha) / (2.0*h);
      aa[1] = 2.0 + h*h*fy(x,w[j][1],t);
      bb[1] = -1.0 + (h/2.0)*fyprime(x,w[j][1],t);
      d[1]  = -(2.0*w[j][1] - w[j][2] - alpha + h*h*f(x,w[j][1],t));

      /* STEP #6 */
      for (i=2;i<N;i++) {
        x     = a + i*h;
        t     = (w[j][i+1] - w[j][i-1]) / (2.0*h);
        aa[i] = 2.0 + h*h*fy(x,w[j][i],t);
        bb[i] = -1.0 + (h/2.0)*fyprime(x,w[j][i],t);
        c[i]  = -1.0 - (h/2.0)*fyprime(x,w[j][i],t);
        d[i]  = -(2.0*w[j][i] - w[j][i+1] - w[j][i-1] + h*h*f(x,w[j][i],t));
      }

      /* STEP #7 */
      x     = b - h;
      t     = (beta-w[j][N-1]) / (2.0*h);
      aa[N] = 2.0 + h*h*fy(x,w[j][N],t);
      c[N]  = -1.0 - (h/2.0)*fyprime(x,w[j][N],t);
      d[N]  = -(2.0*w[j][N] - w[j][N-1] - beta + h*h*f(x,w[j][N],t));

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
      v[N]    = z[N];
      w[j][N] += v[N];

      /* STEP #14 */
      for (i=N-1;i>=1;i--) {
        v[i]    = z[i] - u[i]*v[i+1];
        w[j][i] += v[i];
      }

      /* STEP #15 */
      if (euclidean_norm(v) <= TOL) {	/* Wait to do Steps 16-17. */
        m += k;
        k = M + 1;		/* Will force an exit to next j level. */
      }

      /* STEP 18 */
      k++;			/* k = k + 1. */

    } /* for k loop */
  }  /* for j loop */

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #16 */
  for (i=0;i<=N0+1;i++) {
    Ext[1][i] = (4.0*w[2][2*i] - w[1][i]) / 3.0;
    Ext[2][i] = (4.0*w[3][4*i] - w[2][2*i]) / 3.0;
    Ext[3][i] = (16.0*Ext[2][i] - Ext[1][i]) / 15.0;
    x = a + i*h0;
    printf2("% .2lf\t% .9lf\t% .9lf\t% .9lf\t% .9lf\t% .9lf\t% .9lf\n",
	 x, w[1][i], w[2][2*i], w[3][4*i], Ext[1][i], Ext[2][i], Ext[3][i]);
  }

  /* STEPS #17 and #19 */
  if (!(euclidean_norm(v) <= TOL)) /* Procedure completed unsuccessfully. */
    printf2("\nMaximum number of iterations (%d) exceeded.\n", M);
  else				/* Procedure completed successfully. */
    printf2("\nSolution took %d iterations for desired tolerance.\n", m);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dmatrix(Ext,1,3,0,N+1);
  free_dvector(z,0,5*N);
  free_dmatrix(w,0,3,0,5*(N+1));
  free_dvector(v,0,5*N);
  free_dvector(u,0,5*N);
  free_dvector(l,0,5*N);
  free_dvector(d,0,5*N);
  free_dvector(c,0,5*N);
  free_dvector(bb,0,5*N);
  free_dvector(aa,0,5*N);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/****************************************************************************/
/* euclidean_norm() - Computes the euclidean norm of a vector.              */
/*****************************************************************************/
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
