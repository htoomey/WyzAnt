/******************************************************************************
	    Nonlinear Shooting - Algorithm 11.2  (Newton's Method)
*******************************************************************************

To approximate the solution of the nonlinear boundry-value problem

	y'' = f(x,y,y'),  a ó x ó b,  y(a) = à,  y(b) = á:

(Note: Equations (11.15) and (11.17) are written as first-order systems and
       solved.)

INPUT endpoints a, b; boundry conditions à, á; number of subintervals N
or step size h; tolerance TOL; maximum number of iterations M; the functions
f(), fy() = ëf/ëy, fy'() = ëf/ëy' and y() (if known).

OUTPUT approximations w(1,i) to y(x(i)); w(2,i) to y'(x(i)) for each i =
0,1,...,N or a message that the maximum number of iterations was exceeded.

NOTE: Listed as Nonlinear Shooting with Newton's Method in the 3rd edition of
the text.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 1Q 1992, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "112.out";	/* Customized default output file name.     */
				/* Needs updating  $  */
char *eq_text_y2prime = "y'' = f(x,y,y') = (32.0 + 2.0*x*x*x - y*y')/8.0";
char *eq_text_fy      = "fy  = ëf(x,y,y')/ëy = -y'/8.0";
char *eq_text_fyprime = "fy' = ëf(x,y,y')/ëy' = -y/8.0";
char *eq_text_y       = "y   = x*x + 16.0/x";	/* OPTIONAL */


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
  double **K, **w, u1, u2, f(), fy(), fyprime(), y();
  double a, b, alpha, beta, h, x, TK, TOL;
  int i, kk, N, ch, M;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Nonlinear Shooting - Algorithm 11.2  (Newton's Method)\n\n");
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
    h = (b - a)/N;		/* STEP #1 */
  } else {			/* default */
    printf("Enter interval number h: ");
    scanf("%lf", &h);
    N = (int) ceil((b - a)/h);
  }

  printf2("Number of subintervals N = %d\n", N);
  printf2("Interval number h = %lg\n", h);

  /*
  **  w[1][] = solution - y()
  **  w[2][] = derivative of solution - y'()
  */

  /* Dynamically allocate memory for the needed arrays. */
  w = dmatrix(1,2,0,N);
  K = dmatrix(1,4,1,2);		/* For w[1], w[2], u1 & u2 using RK4 method. */

				/* Print table header. */
  printf2("\n x\t w2 ÷ y'(x)\t w1 ÷ y(x)\t y(x)\t\t |w1 - y(x)|");
  printf2("\n----------------------------------");
  printf2("----------------------------------\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  kk = 1;
  TK = (beta - alpha) / (b - a);

  /* STEP #2 */
  while (kk < M) {		/* Do Steps 3-10. */

    /* STEP #3 */
    w[1][0] = alpha;		/* y(a,t) = à          */
    w[2][0] = TK;		/* y'(a,t) = t (slope) */
    u1      = 0.0;		/* z(a) = 0            */
    u2      = 1.0;		/* z'(a) = 1           */

    /* STEP #4 */
    for (i=1;i<=N;i++) {	/* Do Steps 5-6. */
    /* Runge-Kutta method for systems is used in Steps 5 and 6. */

      /* STEP #5 */
      x = a + (i - 1)*h;

      /* STEP #6 */
      K[1][1] = h * w[2][i-1];
      K[1][2] = h * f(x,w[1][i-1],w[2][i-1]);
      K[2][1] = h * (w[2][i-1] + 0.5*K[1][2]);
      K[2][2] = h * f(x+0.5*h,w[1][i-1]+0.5*K[1][1],w[2][i-1]+0.5*K[1][2]);
      K[3][1] = h * (w[2][i-1] + 0.5*K[2][2]);
      K[3][2] = h * f(x+0.5*h,w[1][i-1]+0.5*K[2][1],w[2][i-1]+0.5*K[2][2]);
      K[4][1] = h * (w[2][i-1] + K[3][2]);
      K[4][2] = h * f(x+h,w[1][i-1]+K[3][1],w[2][i-1]+K[3][2]);
      w[1][i] = w[1][i-1] + (K[1][1] + 2.0*K[2][1] + 2.0*K[3][1]
		+ K[4][1])/6.0;
      w[2][i] = w[2][i-1] + (K[1][2] + 2.0*K[2][2] + 2.0*K[3][2]
		+ K[4][2])/6.0;

      K[1][1] = h * u2;
      K[1][2] = h * (fy(x,w[1][i-1],w[2][i-1])*u1
		+ fyprime(x,w[1][i-1],w[2][i-1])*u2);
      K[2][1] = h * (u2 + 0.5*K[1][2]);
/*
**  TEXT ERROR - Changed K[2][1] to K[1][2] below. (p. 571)
*/
      K[2][2] = h * (fy(x+0.5*h,w[1][i-1],w[2][i-1])*(u1+0.5*K[1][1])
		 + fyprime(x+0.5*h,w[1][i-1],w[2][i-1])*(u2+0.5*K[1][2]));
      K[3][1] = h * (u2 + 0.5*K[2][2]);
      K[3][2] = h * (fy(x+0.5*h,w[1][i-1],w[2][i-1])*(u1+0.5*K[2][1])
		 + fyprime(x+0.5*h,w[1][i-1],w[2][i-1])*(u2+0.5*K[2][2]));
      K[4][1] = h * (u2 + K[3][2]);
      K[4][2] = h * (fy(x+h,w[1][i-1],w[2][i-1])*(u1+K[3][1])
		 + fyprime(x+h,w[1][i-1],w[2][i-1])*(u2+K[3][2]));
      u1 += (K[1][1] + 2.0*K[2][1] + 2.0*K[3][1] + K[4][1])/6.0;
      u2 += (K[1][2] + 2.0*K[2][2] + 2.0*K[3][2] + K[4][2])/6.0;
    }

    /* STEP #7 */
    if (fabs(w[1][N] - beta) <= TOL) {	/* Do Steps 8-9. */

      /***********
       * OUTPUTS *
       ***********/

      /* STEP #8 */
      for (i=0;i<=N;i++) {
        x = a + i*h;
        printf2("% .2lf\t% .9lf\t% .9lf\t% .9lf\t% .2lE\n",
		 x, w[2][i], w[1][i], y(x), fabs(w[1][i] - y(x)));
      }

      /* STEP #9 */
      printf2("\nSolution took %d iterations for requested tolerance.\n", kk);
      /* Free the memory that was dynamically allocated for the arrays. */
      free_dmatrix(K,1,4,1,2);
      free_dmatrix(w,1,2,0,N);

      NAA_do_last(outfile);	/* NAA finish-up procedure.      */
      exit (1);			/* STOP - Procedure is complete. */
    }

    /* STEP #10 */
    /* Newton's method is used to compute TK. */
    TK -= (w[1][N] - beta) / u1;
    kk++;			/* kk = kk + 1. */

  }

  /* STEP #11 */
  printf2("\nMaximum number of iterations (%d) exceeded.\n", M);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dmatrix(K,1,4,1,2);
  free_dmatrix(w,1,2,0,N);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP - Procedure completed unsuccessfully */

/*****************************************************************************/
/*	Copyright (C) 1988-1992, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
