/******************************************************************************
	      Poisson Equation Finite-Difference - Algorithm 12.1
*******************************************************************************

To approximate the solution of the Poisson equation

	λύu(x,t)/λxύ + λύu(x,t)/λyύ = f(x,y),  a σ x σ b, c σ y σ d,

subject to the boundry conditions

	u(x,y) = g(x,y)		if x = a or x = b and c σ y σ d

and	u(x,y) = g(x,y)		if y = c or y = d and a σ x σ b:

INPUT endpoints a, b, c, d; integers m, n; tolerance TOL; maximum number of
iterations N; the functions u(), and f().

OUTPUT approximations w(i,j) to u(x(i),y(j)) for each i = 1,...,n-1 and
j = 1,...,m-1 or a message that the maximum number of iterations was exceeded.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "121.out";	/* Customized default output file name.     */
char *eq_text_f   = "f(x,y) = x*exp(y)";	/* Needs updating  $  */
char *eq_text_uay = "u(0,y) = 0";		/* Needs updating  $  */
char *eq_text_uby = "u(2,y) = 2*exp(y)";	/* Needs updating  $  */
char *eq_text_uxc = "u(x,0) = x";		/* Needs updating  $  */
char *eq_text_uxd = "u(x,1) = exp(1)*x";	/* Needs updating  $  */

#define N	100		/* Maximum number of iterations             */


/*****************************************************************************/
/* f(x,y) - Function to evaluate, f(x,y).  Needs updating $.                 */
/*****************************************************************************/
double f(x,y)
double x, y;
{
  return (x*exp(y));
}

/*****************************************************************************/
/* u(ch,x,y) - Boundary conditions vector.  Needs updating $.                */
/*****************************************************************************/
double u(ch,x,y)
char ch;
double x,y;
{
  switch (ch) {
    case 'a':  return (0.0);		/* For u(a,y). */
    case 'b':  return (2.0*exp(y));	/* For u(b,y). */
    case 'c':  return (x);		/* For u(x,c). */
    case 'd':  return (exp(1.0)*x);	/* For u(x,d). */
    default:   naaerror("Invalid U(x,y) function call.");
  }
}
/*****************************************************************************/


main()
{
  double *x, *y, **w, u(), f();
  double a, b, c, d, h, k, z, lambda, mu, NORM, TOL, temp;
  int i, j, l, m, n;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Poisson Equation Finite-Difference - Algorithm 12.1\n\n");
  printf2("λύu(x,t)/λxύ + λύu(x,t)/λyύ = f(x,y)\n");
  printf2("%s\n", eq_text_f);
  printf2("%s\n", eq_text_uay);
  printf2("%s\n", eq_text_uby);
  printf2("%s\n", eq_text_uxc);
  printf2("%s\n\n", eq_text_uxd);

  printf("Enter endpoint a: ");
  scanf("%lf", &a);
  printf("Enter endpoint b: ");
  scanf("%lf", &b);
  printf("Enter endpoint c: ");
  scanf("%lf", &c);
  printf("Enter endpoint d: ");
  scanf("%lf", &d);
  fprintf(file_id, "For %lg σ x σ %lg and %lg σ y σ %lg.\n", a, b, c, d);

  do {
    printf("Enter number of intervals on [a,b], n: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greter than zero.\n");
  } while (n <= 0);
  fprintf(file_id, "Number of intervals on [a,b] is n = %d\n", n);

  do {
    printf("Enter number of intervals on [c,d], m: ");
    scanf("%d", &m);
    if (n <= 0)
      printf("ERROR - m must be greter than zero.\n");
  } while (m <= 0);
  fprintf(file_id, "Number of intervals on [c,d] is m = %d\n", m);

  printf("Enter tolerance, TOL: ");
  scanf("%lf", &TOL);
  fprintf(file_id, "Tolerance: %lG\n\n\n", TOL);

  printf2(" i\t j\t    x(i)\t    y(i)\t w(i,j)");  /* Print table header. */
  printf2("\n-------------------------------------------------------------\n");

  printf("THINKING ");			/* For feedback */

  /* Dynamically allocate memory for the needed arrays. */
  x = dvector(1,n-1);		/* For values on x-axis. */
  y = dvector(1,m-1);		/* For values on y-axis. */
  w = dmatrix(1,n-1,1,m-1);	/* For w(i,j).           */

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  h = (b - a)/n;
  k = (d - c)/m;

  /* STEP #2 */			/* Steps 2 and 3 construct mesh points. */
  for (i=1;i<n;i++)
    x[i] = a + i*h;

  /* STEP #3 */
  for (j=1;j<m;j++)		/* Construct mesh points. */
    y[j] = c + j*k;

  /* STEP #4 */
  for (i=1;i<n;i++)		/* This step may be omitted since array w    */
    for (j=1;j<m;j++)		/* is initialized to zero by dmatrix() which */
      w[i][j] = 0.0;		/* uses calloc().                            */

  /* STEP #5 */
  lambda = (h*h) / (k*k);
  mu     = 2.0*(1.0+lambda);

  /* STEP #6 and #20 */	/* Steps 7-20 perform Gauss-Seidel iterations. */
  for (l=1;l<=N;l++) {	/* Do Steps 7-20. */

    printf(".");		/* For feedback */

    /* STEP #7 */
    z = (-h*h*f(x[1],y[m-1]) + u('a',a,y[m-1]) + lambda*u('d',x[1],d) +
		lambda*w[1][m-2] + w[2][m-1]) / mu;
    NORM = fabs(z-w[1][m-1]);
    w[1][m-1] = z;

    /* STEP #8 */
    for (i=2;i<=n-2;i++) {
      z = (-h*h*f(x[i],y[m-1]) + lambda*u('d',x[i],d) + w[i-1][m-1] +
		w[i+1][m-1] + lambda*w[i][m-2]) / mu;
      temp = fabs(w[i][m-1]-z);
      if (temp > NORM)
        NORM = temp;
      w[i][m-1] = z;
    }

    /* STEP #9 */
    z = (-h*h*f(x[n-1],y[m-1]) + u('b',b,y[m-1]) + lambda*u('d',x[n-1],d) +
		w[n-2][m-1] + lambda*w[n-1][m-2]) / mu;
    temp = fabs(w[n-1][m-1]-z);
    if (temp > NORM)
      NORM = temp;
    w[n-1][m-1] = z;

    /* STEP #10 */
    for (j=m-2;j>=2;j--) {	/* Do Steps 11-13. */

      /* STEP #11 */
      z = (-h*h*f(x[1],y[j]) + u('a',a,y[j]) + lambda*w[1][j+1] +
		lambda*w[1][j-1] + w[2][j]) / mu;
      temp = fabs(w[1][j]-z);
      if (temp > NORM)
        NORM = temp;
      w[1][j] = z;

      /* STEP #12 */
      for (i=2;i<=n-2;i++) {
        z = (-h*h*f(x[i],y[j]) + w[i-1][j] + lambda*w[i][j+1] + w[i+1][j] +
		lambda*w[i][j-1]) / mu;
        temp = fabs(w[i][j]-z);
        if (temp > NORM)
          NORM = temp;
        w[i][j] = z;
      }

      /* STEP #13 */
      z = (-h*h*f(x[n-1],y[j]) + u('b',b,y[j]) + w[n-2][j] +
		lambda*w[n-1][j+1] + lambda*w[n-1][j-1]) / mu;
      temp = fabs(w[n-1][j]-z);
      if (temp > NORM)
        NORM = temp;
      w[n-1][j] = z;
    }

    /* STEP #14 */
    z = (-h*h*f(x[1],y[1]) + u('a',a,y[1]) + lambda*u('c',x[1],c) +
		lambda*w[1][2] + w[2][1]) / mu;
    temp = fabs(w[1][1]-z);
    if (temp > NORM)
      NORM = temp;
    w[1][1] = z;

    /* STEP #15 */
    for (i=2;i<=n-2;i++) {
      z = (-h*h*f(x[i],y[1]) + lambda*u('c',x[i],c) + w[i-1][1] +
		lambda*w[i][2] + w[i+1][1]) / mu;
      temp = fabs(w[i][1]-z);
      if (temp > NORM)
        NORM = temp;
      w[i][1] = z;
    }

    /* STEP #16 */
    z = (-h*h*f(x[n-1],y[1]) + u('b',b,y[1]) + lambda*u('c',x[n-1],c) +
		w[n-2][1] + lambda*w[n-1][2]) / mu;
    temp = fabs(w[n-1][1]-z);
    if (temp > NORM)
      NORM = temp;
    w[n-1][1] = z;

    /* STEP #17 */
    if (NORM <= TOL) {		/* Do Steps 18-19. */

      /***********
       * OUTPUTS *
       ***********/

      /* STEP #18 */
      printf("\n");
      for (i=1;i<n;i++)
        for (j=1;j<m;j++)
          printf2(" %d\t %d\t% 8.8lG\t% 8.8lG\t% 8.8lG\n",
		 i, j, x[i], y[j], w[i][j]);

      /* STEP #19 */
      printf2("\nRequired %d iterations.\n", l);
      /* Free the memory that was dynamically allocated for the arrays. */
      free_dmatrix(w,1,n-1,1,m-1);
      free_dvector(y,1,m-1);
      free_dvector(x,1,n-1);

      NAA_do_last(outfile);	/* NAA finish-up procedure. */
      exit (1);		/* STOP - Procedure completed successfully. */
    }
  }

  /* STEP #21 */
  printf2("\nMaximum number of iterations (%d) exceeded.\n", N);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dmatrix(w,1,n-1,1,m-1);
  free_dvector(y,1,m-1);
  free_dvector(x,1,n-1);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
