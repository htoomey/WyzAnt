
/******************************************************************************
	    Nonlinear Shooting - Algorithm 11.2D (Newton's Method)
		    (Using Runge-Kutta-Fehlberg Method)
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

NOTE: This algorithm was included as a "Homework Helper."  See p. 573,
Exercise Set 11.2, Problem 6 b).

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 1Q 1992, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "112d.out";	/* Customized default output file name.     */
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
  double **w, u1, u2, f(), fy(), fyprime(), y();
  double a, b, alpha, beta, h, x, TK, TOL;
  double hmin, hmax, delta, R, **K, **KP, *X, tmp1, tmp2;
  int i, kk, N, ch, M, FLAG;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Nonlinear Shooting - Algorithm 11.2D (Newton's Method)\n");
  printf2("Using Runge-Kutta-Fehlberg Method\n\n");
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

  printf2("Range for the number of subintervals N = %d to %d\n", N, 10*N);
  printf2("Interval number h = %lg\n", h);

  /*
  **  w[1][] = solution ---> y()
  **  w[2][] = derivative of solution ---> y'()
  */

  /* Dynamically allocate memory for the needed arrays. */
  w  = dmatrix(1,2,0,10*N);
  K  = dmatrix(1,6,1,2);
  KP = dmatrix(1,6,1,2);
  X  = dvector(0,10*N);		/* Stores each x for final printout */

				/* Print table header. */
  printf2("\n x\t w2 ÷ y'(x)\t w1 ÷ y(x)\t y(x)\t\t |w1 - y(x)|");
  printf2("\n----------------------------------");
  printf2("----------------------------------\n");

  printf("THINKING ");		/* For feedback during long computations. */

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  kk = 1;
  TK = (beta - alpha) / (b - a);

  /*
  ** MODIFICATION #1 - Add two necessary Runge-Kutta-Fehlberg variables.
  **   All occurances of N must also be adjusted (ie - from N to 10*N).
  */
  hmax = h;
  hmin = h/10.0;

  /* STEP #2 */
  while (kk <= M) {		/* Do Steps 3-10. */

    /* STEP #3 */
    w[1][0] = alpha;		/* y(a,t) = à          */
    w[2][0] = TK;		/* y'(a,t) = t (slope) */
    u1      = 0.0;		/* z(a) = 0            */
    u2      = 1.0;		/* z'(a) = 1           */

    /*
    ** MODIFICATION #2 - Former steps #4 - #6 have been completely replaced
    **    below with Runge-Kutta-Fehlberg (RKF) Sub-Steps derived from
    **    Algorithm 5.3.  Other minor changes have also been made throughout
    **    this program.
    */

    /* RKF SUB-STEP #1 */
    x    = a;
    h    = hmax;
    i    = 0;
    FLAG = 1;
    X[i] = x;
    i++;

    /* RKF SUB-STEP #2 */
    while (FLAG == 1) {		/* Do RKF Sub-Steps 3-11. */

      /* RKF SUB-STEP #3 */
      /* Solve for w here. */
      K[1][1] = h*w[2][i-1];
      K[1][2] = h*f(x, w[1][i-1], w[2][i-1]);

      K[2][1] = h*(w[2][i-1] + K[1][2]/4);
      K[2][2] = h*f(x+h/4, w[1][i-1] + K[1][1]/4, w[2][i-1] + K[1][2]/4);

      K[3][1] = h*(w[2][i-1] + 3*K[1][2]/32 + 9*K[2][2]/32);
      K[3][2] = h*f(x+3*h/8, w[1][i-1] + 3*K[1][1]/32 + 9*K[2][1]/32,
                             w[2][i-1] + 3*K[1][2]/32 + 9*K[2][2]/32);

      K[4][1] = h*(w[2][i-1] + 1932*K[1][2]/2197
                   - 7200*K[2][2]/2197 + 7296*K[3][2]/2197);
      K[4][2] = h*f(x+12*h/13, w[1][i-1] + 1932*K[1][1]/2197
                               - 7200*K[2][1]/2197 + 7296*K[3][1]/2197,
                               w[2][i-1] + 1932*K[1][2]/2197
                               - 7200*K[2][2]/2197 + 7296*K[3][2]/2197);

      K[5][1] = h*(w[2][i-1] + 439*K[1][2]/216 - 8*K[2][2]
                 + 3680*K[3][2]/513 - 845*K[4][2]/4104);
      K[5][2] = h*f(x+h, w[1][i-1] + 439*K[1][1]/216 - 8*K[2][1]
                         + 3680*K[3][1]/513 - 845*K[4][1]/4104,
                         w[2][i-1] + 439*K[1][2]/216 - 8*K[2][2]
                         + 3680*K[3][2]/513 - 845*K[4][2]/4104);

      K[6][1] = h*(w[2][i-1] - 8*K[1][2]/27 + 2*K[2][2]
                           - 3544*K[3][2]/2565 + 1859*K[4][2]/4104
                           - 11*K[5][2]/40);
      tmp1 = w[1][i-1] - 8*K[1][1]/27 + 2*K[2][1]
                           - 3544*K[3][1]/2565 + 1859*K[4][1]/4104
                           - 11*K[5][1]/40;
      K[6][2] = h*f(x+h/2, tmp1,
                            w[2][i-1] - 8*K[1][2]/27 + 2*K[2][2]
                           - 3544*K[3][2]/2565 + 1859*K[4][2]/4104
                           - 11*K[5][2]/40);

      /* Solve for u1 and u2 here. */
      KP[1][1] = h * u2;
      KP[1][2] = h*(fy(x, w[1][i-1], w[2][i-1])*u1
                 + fyprime(x, w[1][i-1], w[2][i-1])*u2);

      KP[2][1] = h*(u2 + KP[1][2]/4);
      KP[2][2] = h*(fy(x+h/4, w[1][i-1] + KP[1][1]/4, w[2][i-1] + KP[1][2]/4)
                 * (u1 + KP[1][1]/4)
                 + fyprime(x+h/4, w[1][i-1] + KP[1][1]/4, w[2][i-1]
                   + KP[1][2]/4)
                 * (u2 + KP[1][2]/4));

      KP[3][1] = h*(u2 + 3*KP[1][2]/32 + 9*KP[2][2]/32);
      tmp1     = w[1][i-1] + 3*KP[1][1]/32 + 9*KP[2][1]/32;
      tmp2     = w[2][i-1] + 3*KP[1][2]/32 + 9*KP[2][2]/32;
      KP[3][2] = h*(fy(x+3*h/8, tmp1, tmp2)
                 * (u1 + 3*KP[1][1]/32 + 9*KP[2][1]/32)
                 + fyprime(x+3*h/8, tmp1, tmp2)
                 * (u2 + 3*KP[1][2]/32 + 9*KP[2][2]/32));

      KP[4][1] = h*(u2 + 1932*KP[1][2]/2197 - 7200*KP[2][2]/2197
                   + 7296*KP[3][2]/2197);
      tmp1     = w[1][i-1] + 1932*KP[1][1]/2197
                   - 7200*KP[2][1]/2197 + 7296*KP[3][1]/2197;
      tmp2     = w[2][i-1] + 1932*KP[1][2]/2197
                   - 7200*KP[2][2]/2197 + 7296*KP[3][2]/2197;
      KP[4][2] = h*(fy(x+12*h/13, tmp1, tmp2)
                 * (u1 + 1932*KP[1][1]/2197 - 7200*KP[2][1]/2197
                   + 7296*KP[3][1]/2197)
                 + fyprime(x+12*h/13, tmp1, tmp2)
                 * (u2 + 1932*KP[1][2]/2197 - 7200*KP[2][2]/2197
                   + 7296*KP[3][2]/2197));

      KP[5][1] = h*(u2 + 439*KP[1][2]/216 - 8*KP[2][2] + 3680*KP[3][2]/513
                   - 845*KP[4][2]/4104);
      tmp1     = w[1][i-1] + 439*KP[1][1]/216 - 8*KP[2][1]
                   + 3680*KP[3][1]/513 - 845*KP[4][1]/4104;
      tmp2     = w[2][i-1] + 439*KP[1][2]/216 - 8*KP[2][2]
                   + 3680*KP[3][2]/513 - 845*KP[4][2]/4104;
      KP[5][2] = h*(fy(x+h, tmp1, tmp2)
                 * (u1 + 439*KP[1][1]/216 - 8*KP[2][1] + 3680*KP[3][1]/513
                   - 845*KP[4][1]/4104)
                 + fyprime(x+h, tmp1, tmp2)
                 * (u2 + 439*KP[1][2]/216 - 8*KP[2][2] + 3680*KP[3][2]/513
                   - 845*KP[4][2]/4104));

      KP[6][1] = h*(u2 - 8*KP[1][2]/27 + 2*KP[2][2] - 3544*KP[3][2]/2565
                   + 1859*KP[4][2]/4104 - 11*KP[5][2]/40);
      tmp1     = w[1][i-1] - 8*KP[1][1]/27 + 2*KP[2][1] - 3544*KP[3][1]/2565
                   + 1859*KP[4][1]/4104 - 11*KP[5][1]/40;
      tmp2     = w[2][i-1] - 8*KP[1][2]/27 + 2*KP[2][2] - 3544*KP[3][2]/2565
                   + 1859*KP[4][2]/4104 - 11*KP[5][2]/40;
      KP[6][2] = h*(fy(x+h/2, tmp1, tmp2)
                 * (u1 - 8*KP[1][1]/27 + 2*KP[2][1] - 3544*KP[3][1]/2565
                   + 1859*KP[4][1]/4104 - 11*KP[5][1]/40)
                 + fyprime(x+h/2, tmp1, tmp2)
                 * (u2 - 8*KP[1][2]/27 + 2*KP[2][2] - 3544*KP[3][2]/2565
                   + 1859*KP[4][2]/4104 - 11*KP[5][2]/40));

      /* RKF SUB-STEP #4 */		/* Note R = |~w(i+1) - w(i+1)| / h */
      R = fabs(K[1][1]/360 - 128*K[3][1]/4275 - 2197*K[4][1]/75240
           + K[5][1]/50 + 2*K[6][1]/55) / h;

      /* RKF SUB-STEP #5 */
      delta = 0.84 * pow(TOL/R, 0.25);

      /* RKF SUB-STEP #6 */
      if (R <= TOL) {		/* Do Steps 7-8. */

        /* RKF SUB-STEP #7 */
        x += h;			/* Approximation accepted. */
        X[i] = x;		/* Save x to print out later. */

        /* Compute w1 and w2. */
        w[1][i] = w[1][i-1] + 25*K[1][1]/216 + 1408*K[3][1]/2565
                  + 2197*K[4][1]/4104 - K[5][1]/5;
        w[2][i] = w[2][i-1] + 25*K[1][2]/216 + 1408*K[3][2]/2565
                  + 2197*K[4][2]/4104 - K[5][2]/5;

        /* Compute u1 and u2. */
        u1 += 25*KP[1][1]/216 + 1408*KP[3][1]/2565 + 2197*KP[4][1]/4104
              - KP[5][1]/5;
        u2 += 25*KP[1][2]/216 + 1408*KP[3][2]/2565 + 2197*KP[4][2]/4104
              - KP[5][2]/5;

        /* RKF SUB-STEP #8 */
        i++;			/* i = i + 1. */
      }

      /* RKF SUB-STEP #9 */
      if (delta <= 0.1)
        h = 0.1 * h;
      else {
        if (delta >= 4.0)
          h *= 4.0;
        else
          h *= delta;		/* Calculate new h. */
      }

      /* RKF SUB-STEP #10 */
      if (h > hmax)
        h = hmax;

      /* RKF SUB-STEP #11 */
      if (x >= b)
        FLAG = 0;
      else if (x + h > b)
        h = b - x;
      else if (h < hmin) {
        printf2("Minimum h exceeded.\n");
        /* Free the memory that was dynamically allocated for the arrays. */
        free_dvector(X,0,10*N);
        free_dmatrix(KP,1,6,1,2);
        free_dmatrix(K,1,6,1,2);
        free_dmatrix(w,1,2,0,10*N);

        NAA_do_last(outfile);	/* NAA finish-up procedure.            */
        exit (-1);		/* Procedure completed unsuccessfully. */
      }
    }

    printf(".");		/* For feedback. */

    /* STEP #7 */
    /*  MODIFICATION #3 - Save N since it varies with different values of h. */
    N = i - 1;			/* Save number of steps used. */
    if (fabs(w[1][N] - beta) <= TOL) {	/* Do Steps 8-9. */

      /***********
       * OUTPUTS *
       ***********/

      /* STEP #8 */
      printf("\n");		/* Terminate the "THINKING" message. */
      for (i=0;i<=N;i++) {
          printf2("% .2lf\t% .9lf\t% .9lf\t% .9lf\t% .2lE\n",
		   X[i], w[2][i], w[1][i], y(X[i]), fabs(w[1][i] - y(X[i])));
      }

      /* STEP #9 */
      printf2("\nSolution took %d iterations for requested tolerance.\n", kk);
      /* Free the memory that was dynamically allocated for the arrays. */
      free_dvector(X,0,10*N);
      free_dmatrix(KP,1,6,1,2);
      free_dmatrix(K,1,6,1,2);
      free_dmatrix(w,1,2,0,10*N);

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
  free_dvector(X,0,10*N);
  free_dmatrix(KP,1,6,1,2);
  free_dmatrix(K,1,6,1,2);
  free_dmatrix(w,1,2,0,10*N);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP - Procedure completed unsuccessfully */

/*****************************************************************************/
/*	Copyright (C) 1988-1992, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
