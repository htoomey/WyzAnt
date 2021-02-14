/******************************************************************************
	Composite Trapezoid Rule for Triple Integrals - Algorithm 4.5B
*******************************************************************************

                                Ùb Ùd(x) Ù·(x,y)
To approximate the integral I = ≥  ≥     ≥       f(x,y,z) dx dy dz:
                                ıa ıc(x) ı‡(x,y)

INPUT endpoints a, b; positive integers m, n, p; the function f(); the
functions c(x) and d(x) as needed; the functions ‡(x,y) and ·(x,y) as needed.

OUTPUT approximation J to I.

NOTE: This algorithm was included as a "Homework Helper."  See p. 216,
Exercise Set 4.8, Problem 11.  Really included for completeness.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities.  */

char *outfile        = "045b.out";  /* Customized default output file name.  */
char *eq_text_f      = "f(x,y,z)    = sqrt(x*x + y*y)";/* Needs updating  $  */
char *eq_text_c      = "c(x)        = -sqrt(4.0 - x*x)";/*Needs updating  $  */
char *eq_text_d      = "d(x)        = sqrt(4.0 - x*x)";/* Needs updating  $  */
char *eq_text_alphaf = "alphaf(x,y) = sqrt(x*x + y*y)";/* Needs updating  $  */
char *eq_text_betaf  = "betaf(x,y)  = 2.0";	       /* Needs updating  $  */


/*****************************************************************************/
/* f(x,y,z) - Function to evaluate, f(x,y,z).  Needs updating $.             */
/*****************************************************************************/
double f(x,y,z)
double x, y, z;
{
  return (sqrt(x*x + y*y));
}
/* NOTE: Can also use hypot(x,y) = sqrt(x*x + y*y) */

/*****************************************************************************/
/* c(x) - Lower bound function to evaluate, c(x).  Needs updating $.         */
/*****************************************************************************/
double c(x)
double x;
{
  return (-sqrt(4.0 - x*x));
}

/*****************************************************************************/
/* d(x) - Upper bound function to evaluate, d(x).  Needs updating $.         */
/*****************************************************************************/
double d(x)
double x;
{
  return (sqrt(4.0 - x*x));
}

/*****************************************************************************/
/* alphaf(x,y) - Lower bound function to evaluate, alphaf(x,y).              */
/*               Needs updating $.                                           */
/*****************************************************************************/
double alphaf(x,y)
double x, y;
{
  return (sqrt(x*x + y*y));
}
/* NOTE: Can also use hypot(x,y) = sqrt(x*x + y*y) */

/*****************************************************************************/
/* betaf(x,y) - Upper bound function to evaluate, betaf(x,y).                */
/*              Needs updating $.                                            */
/*****************************************************************************/
double betaf(x,y)
double x, y;
{
  return (2.0);
}
/*****************************************************************************/


main()
{
  double a, b, cc, dd, alpha, beta, h, HX, HY, Q;
  double J, J1, J2, K, K1, K2, L, L1, L2;
  double x, y, z, c(), d(), alphaf(), betaf(), f();
  int i, j, k, m, n, p, ch, CONST1, CONST2;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Composite Trapezoid Rule for Triple Integrals");
  printf2(" - Algorithm 4.5B\n\n");
  printf2("                                Ùb Ùd(x) Ù·(x,y)\n");
  printf2("To approximate the integral I = ≥  ≥     ≥       f(x,y,z)");
  printf2(" dx dy dz\n");
  printf2("                                ıa ıc(x) ı‡(x,y)\n\n");

  printf2("%s\n", eq_text_f);
  printf2("%s\n", eq_text_c);
  printf2("%s\n", eq_text_d);
  printf2("%s\n", eq_text_alphaf);
  printf2("%s\n\n", eq_text_betaf);

  printf("Enter endpoint a: ");
  scanf("%lf", &a);
  printf("Enter endpoint b: ");
  scanf("%lf", &b);
  fprintf(file_id, "For %lg Û x Û %lg", a, b);

  printf("Are c(x) and d(x) constants? (Y/N) N: ");
  getchar();			/* Cleans up after scanf(). */
  ch = tolower(getchar());
  if (ch == 'y') {
    printf( "Enter endpoint c: ");
    scanf("%lf", &cc);
    printf("Enter endpoint d: ");
    scanf("%lf", &dd);
    fprintf(file_id, " and %lg Û y Û %lg", cc, dd);

    CONST1 = TRUE;	/* CONST1 = TRUE if c(x) and d(x) are constants.  */
  } else {
    fprintf(file_id, " and c(x) Û y Û d(x)");
    CONST1 = FALSE;	/* CONST1 = FALSE if c(x) and d(x) are functions. */
  }

  printf("Are ‡(x,y) and ·(x,y) constants? (Y/N) N: ");
  getchar();			/* Cleans up after scanf(). */
  ch = tolower(getchar());
  if (ch == 'y') {
    printf("Enter endpoint alpha: ");
    scanf("%lf", &alpha);
    printf("Enter endpoint beta: ");
    scanf("%lf", &beta);
    fprintf(file_id, " and %lg Û z Û %lg\n\n", alpha, beta);

    CONST2 = TRUE;	/* CONST2 = TRUE if ‡(x,y) and ·(x,y) are constants. */
  } else {
    fprintf(file_id, " and ‡(x,y) Û z Û ·(x,y)\n\n");
    CONST2 = FALSE;	/* CONST2 = FALSE if ‡(x,y) and ·(x,y) are functions.*/
  }

  do {
    printf("Enter number of intervals on [a,b], n: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greater than zero.\n");
  } while (n <= 0);
  fprintf(file_id, "n = %d intervals on [a,b].\n", n);

  do {
    printf("Enter number of intervals on [c(x),d(x)], m: ");
    scanf("%d", &m);
    if (m <= 0)
      printf("ERROR - m must be greater than zero.\n");
  } while (m <= 0);
  fprintf(file_id, "m = %d intervals on [c(x),d(x)].\n", m);

  do {
    printf("Enter number of intervals on [alpha(x,y), beta(x,y)], p: ");
    scanf("%d", &p);
    if (p <= 0)
      printf("ERROR - p must be greater than zero.\n");
  } while (p <= 0);
  fprintf(file_id, "p = %d intervals on [alpha(x,y), beta(x,y)].\n\n", p);

  printf("THINKING");		/* For feedback during computation. */

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  h  = (b - a)/n;
  J1 = 0.0;			/* End terms.           */
  J2 = 0.0;			/* Internal terms.      */

  /* STEP #2 */
  for (i=0;i<=n;i++) {		/* Do Steps 3-12.       */

    printf(".");		/* For THINKING output. */

    /* STEP #3 */
    x = a + i*h;		/* Composite Trapezoid method for fixed x. */
    if (CONST1 == FALSE) {	/* Use c(x) and d(x) functions.            */
      cc = c(x);		/* Use cc and dd to save (n+1)*(2+(m+1))   */
      dd = d(x);		/* ... functional evaluations.             */
    }
    HX = (dd - cc)/m;
    K1 = 0.0;			/* End terms for each x.                   */
    K2 = 0.0;			/* Internal terms for each x.              */

    /* STEP #4 */
    for (j=0;j<=m;j++) {	/* Do Steps 5-10. */

      /* STEP #5 */
      y = cc + j*HX;		/* The Composite Simpson's method for fixed  */
				/* ... x and y.                              */
      if (CONST2 == FALSE) {	/* Use alphaf(x,y) and betaf(x,y) functions. */
        alpha = alphaf(x,y);	/* Use alpha and beta to save lots of integ- */
        beta  = betaf(x,y);	/* ... grand limit functional evaluations.   */
      }
      HY = (beta - alpha)/p;
      L1 = f(x,y,alpha) + f(x,y,beta);	/* End terms.      */
      L2 = 0.0;				/* Internal terms. */

      /* STEP #6 */
      for (k=1;k<p;k++) {	/* Do Steps 7-8. */

        /* STEP #7 */
        z = alpha + k*HY;
        Q = f(x,y,z);

        /* STEP #8 */
        L2 += Q;
      }

      /* STEP #9 */
      L = (L1 + 2.0*L2) * HY/2.0;

      /*
      **  (     Ùbeta(x(i),y(j))					)
      **  ( L ˜ ≥     f(x(i),y(j),z) dz  by Composite Trapezoid Method. )
      **  (     ıalpha(x(i),y(j))					)
      */

      /* STEP #10 */
      if ((j == 0) || (i == m))
        K1 += L;
      else
        K2 += L;
    }

    /* STEP #11 */
    K = (K1 + 2.0*K2) * HX/2.0;

    /*
    **	(     Ùd(x(i))  Ùbeta(x(i),y(j))                                     )
    **	( K ˜ ≥         ≥   f(x(i),y,z) dz dy  by Composite Trapezoid Method.)
    **	(     ıc(x(i))  ıalpha(x(i),y(j))                                    )
    */

    /* STEP #12 */
    if ((i == 0) || (i == n))
      J1 += K;
    else
      J2 += K;
  }

  /* STEP #13 */
  J = (J1 + 2.0*J2) * h/2.0;

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #14 */
  printf("\n\n");
  if ((CONST1 == TRUE) && (CONST2 == TRUE)) {
    printf2("    Ù%-4lg  Ù%-4lg  Ù%-4lg\n", b, dd, beta);
    printf2("I = ≥      ≥      ≥     f(x,y,z) dx dy dz = %.11lg\n", J);
    printf2("    ı%-4lg  ı%-4lg  ı%-4lg\n\n", a, cc, alpha);
    printf2("Required %ld functional evaluations.\n",
	 (long) (n+1)*(m+1) * (long) (2)*(p-1));
  } else {
    printf2("    Ù%-4lg  Ùd(x)  Ùbeta(x,y)\n", b);
    printf2("I = ≥      ≥      ≥         f(x,y,z) dx dy dz = %.11lg\n", J);
    printf2("    ı%-4lg  ıc(x)  ıalpha(x,y)\n\n", a);
    printf2("Required %ld functional evaluations.\n",
	 (long) (n+1)*(2)*(m+1) * (long) (4)*(p-1));
  }

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
