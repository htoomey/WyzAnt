/******************************************************************************
	Gaussian Quadrature for Double Integrals - Algorithm 4.4C
*******************************************************************************

                                ôb ôd(x)
To approximate the integral I = ³  ³     f(x,y) dy dx:
                                õa õc(x)

INPUT endpoints a, b; positive integers m, n; the function f(); the functions
c(x) and d(x) as needed.

OUTPUT approximation J to I.

NOTE: This algorithm was included as a "Homework Helper."  See p. 215,
Exercise Set 4.8, Problem *** 7 ***.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "044c.out";	/* Customized default output file name.     */
char *eq_text_f = "f(x,y) = exp(y/x)";	/* Needs updating  $  */
char *eq_text_c = "c(x)   = x*x*x";	/* Needs updating  $  */
char *eq_text_d = "d(x)   = x*x";	/* Needs updating  $  */


/*****************************************************************************/
/* f(x,y) - Function to evaluate, f(x,y).  Needs updating $.                 */
/*****************************************************************************/
double f(x,y)
double x, y;
{
  return (exp(y/x));
}

/*****************************************************************************/
/* c(x) - Lower bound function to evaluate, c(x).  Needs updating $.         */
/*****************************************************************************/
double c(x)
double x;
{
  return (x*x*x);
}

/*****************************************************************************/
/* d(x) - Upper bound function to evaluate, d(x).  Needs updating $.         */
/*****************************************************************************/
double d(x)
double x;
{
  return (x*x);
}
/*****************************************************************************/

/*
** Note: To evaluate higher order Legendre polynomials, add to the R[] and C[]
**       arrays below.  Also increase #define MAXN used for range checking.
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

double a, b, cc, dd, I, f(), u(), v();


main()
{
  double c(), d(), tmp, tmpu;
  int i, j, m, n, ch, CONST;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Gaussian Quadrature for Double Integrals - Algorithm 4.4C\n\n");
  printf2("%s\n", eq_text_f);
  printf2("%s\n", eq_text_c);
  printf2("%s\n\n", eq_text_d);

  printf("Enter endpoint a: ");
  scanf("%lf", &a);
  printf("Enter endpoint b: ");
  scanf("%lf", &b);
  fprintf(file_id, "For %lg ó x ó %lg", a, b);

  printf("Are c(x) and d(x) constants? (Y/N) N: ");
  getchar();			/* Cleans up after scanf(). */
  ch = tolower(getchar());
  if (ch == 'y') {
    printf("Enter endpoint c: ");
    scanf("%lf", &cc);
    printf("Enter endpoint d: ");
    scanf("%lf", &dd);
    fprintf(file_id, " and %lg ó y ó %lg\n\n", cc, dd);
    CONST = TRUE;	/* CONST = TRUE if c(x) and d(x) are constants. */
  } else {
    fprintf(file_id, " and c(x) ó y ó d(x)\n\n");
    CONST = FALSE;
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
  fprintf(file_id, "m = %d intervals on [c(x),d(x)].\n\n", m);
  printf("\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  I = 0.0;
  for (i=0;i<n;i++)		/* Compute the integral. */
    for (j=0;j<m;j++) {
      if (CONST == FALSE) {
        tmpu = u(R[n][i]);
        cc   = c(tmpu);
        dd   = d(tmpu);
      }

      /* STEP #2 */
      tmp = ((b - a)*(dd - cc)) / 4.0;
      I += tmp * C[n][i] * C[n][j] * f(tmpu, v(R[n][j]));
    }

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #3 */
  if (CONST == TRUE) {
    printf2("    ô%-4lg  ô%-4lg\n", b, dd);
    printf2("I = ³      ³     f(x) dy dx = %.11lg\n", I);
    printf2("    õ%-4lg  õ%-4lg\n\n", a, cc);
  } else {
    printf2("    ô%-4lg  ôd(x)\n", b);
    printf2("I = ³      ³     f(x) dy dx = %.11lg\n", I);
    printf2("    õ%-4lg  õc(x)\n\n", a);
  }
  printf2("Required %d functional evaluations.\n", (n*m));

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
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
