/******************************************************************************
	Gaussian Quadrature for Triple Integrals - Algorithm 4.5C
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

char *outfile        = "045c.out";  /* Customized default output file name.  */
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

/*
**  Note: To evaluate higher order Legendre polynomials, add to the R[] and C[]
**        arrays below.  Also increase #define MAXN used for range checking.
*/

#define MAXN	5		/* Largest degree polynomial to evaluate. */

/* Table of constants C[][] and roots R[][] of some Legendre polynomials. */
double R[MAXN+1][MAXN] = {
  { 0.0000000000,  0.0000000000,  0.0000000000,  0.0000000000,  0.0000000000 },
  { 0.0000000000,  0.0000000000,  0.0000000000,  0.0000000000,  0.0000000000 },
  { 0.5773502692, -0.5773502692,  0.0000000000,  0.0000000000,  0.0000000000 },
  { 0.7745966692,  0.0000000000, -0.7745966692,  0.0000000000,  0.0000000000 },
  { 0.8611363116,  0.3399810436, -0.3399810436, -0.8611363116,  0.0000000000 },
  { 0.9061798459,  0.5384693101,  0.0000000000,	-0.5384693101, -0.9061798459 },
};

double C[MAXN+1][MAXN] = {
  { 0.0000000000,  0.0000000000,  0.0000000000,  0.0000000000,  0.0000000000 },
  { 0.0000000000,  0.0000000000,  0.0000000000,  0.0000000000,  0.0000000000 },
  { 1.0000000000,  1.0000000000,  0.0000000000,  0.0000000000,  0.0000000000 },
  { 0.5555555556,  0.8888888889,  0.5555555556,  0.0000000000,  0.0000000000 },
  { 0.3478548451,  0.6521451549,  0.6521451549,  0.3478548451,  0.0000000000 },
  { 0.2369268850,  0.4786286705,  0.5688888889,	 0.4786286705,  0.2369268850 },
};

double a, b, cc, dd, alpha, beta, f(), u(), v(), w();


main()
{
  double I, x, y, z, c(), d(), alphaf(), betaf(), tmp, tmpu, tmpv;
  int i, j, k, m, n, p, ch, CONST1, CONST2;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Gaussian Quadrature for Triple Integrals - Algorithm 4.5C\n\n");
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
    printf("Enter number of intervals on [a,b], n (2-%d): ", MAXN);
    scanf("%d", &n);
    if ((n < 2) || (n > MAXN))
      printf("ERROR - n must be between 2 and %d.\n", MAXN);
  } while ((n < 2) || (n > MAXN));
  fprintf(file_id, "n = %d intervals on [a,b].\n", n);

  do {
    printf("Enter number of intervals on [c(x),d(x)], m (2-%d): ", MAXN);
    scanf("%d", &m);
    if ((m < 2) || (m > MAXN))
      printf("ERROR - m must be between 2 and %d.\n", MAXN);
  } while ((m < 2) || (m > MAXN));
  fprintf(file_id, "m = %d intervals on [c(x),d(x)].\n", m);

  do {
    printf("Enter number of intervals on [alpha(x,y),beta(x,y)], p (2-%d): ",
      MAXN);
    scanf("%d", &p);
    if ((p < 2) || (p > MAXN))
      printf("ERROR - p must be between 2 and %d.\n", MAXN);
  } while ((p < 2) || (p > MAXN));
  fprintf(file_id, "p = %d intervals on [alpha(x,y),beta(x,y)].\n\n", p);

  printf("THINKING");		/* For feedback during computation. */

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  I = 0.0;
  for (i=0;i<n;i++) {		/* Compute the integral. */
    printf(".");		/* For THINKING output.  */
    for (j=0;j<m;j++)
      for (k=0;k<p;k++) {
        tmpu = u(R[n][i]);	/* Saves several function evaluations. */
        tmpv = v(R[n][j]);	/* Saves several function evaluations. */
        if (CONST1 == FALSE) {
          cc = c(tmpu);
          dd = d(tmpu);
        }
        if (CONST2 == FALSE) {
          alpha = alphaf(tmpu, tmpv);
          beta  = betaf(tmpu, tmpv);
        }

        /* STEP #2 */
        tmp = ((b - a)*(dd - cc)*(beta - alpha)) / 8.0;
        I += tmp * C[n][i] * C[n][j] * C[n][k] * f(tmpu, tmpv, w(R[n][k]));
      }
  }

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #3 */
  printf("\n\n");
  if ((CONST1 == TRUE) && (CONST2 == TRUE)) {
    printf2("    Ù%-4lg  Ù%-4lg  Ù%-4lg\n", b, dd, beta);
    printf2("I = ≥      ≥      ≥     f(x,y,z) dx dy dz = %.11lg\n", I);
    printf2("    ı%-4lg  ı%-4lg  ı%-4lg\n\n", a, cc, alpha);
  } else {
    printf2("    Ù%-4lg  Ùd(x)  Ùbeta(x,y)\n", b);
    printf2("I = ≥      ≥      ≥         f(x,y,z) dx dy dz = %.11lg\n", I);
    printf2("    ı%-4lg  ıc(x)  ıalpha(x,y)\n\n", a);
  }
  printf2("Required %ld functional evaluations.\n", (long) (n*m*p));

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* u() - Transforms variables into the interval [-1,1].                      */
/*****************************************************************************/
double u(x)
double x;
{
  return (((b - a)*x + b + a) / 2.0);
}

/*****************************************************************************/
/* v() - Transforms variables into the interval [-1,1].                      */
/*****************************************************************************/
double v(y)
double y;
{
  return (((dd - cc)*y + dd + cc) / 2.0);
}

/*****************************************************************************/
/* w() - Transforms variables into the interval [-1,1].                      */
/*****************************************************************************/
double w(z)
double z;
{
  return (((beta - alpha)*z + beta + alpha) / 2.0);
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
