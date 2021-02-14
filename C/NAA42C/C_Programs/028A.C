/******************************************************************************
		Complex Polynomial Solver (CPOLY) - Algorithm 2.8A
*******************************************************************************

To find all roots (real, complex, single, and multiple) of a polynomial of
degree n accurate to a tolerance of tol.  (Note: n is limited only by the
memory size of the hosting computer.)

	P(x) = a(n)*x^(n) + a(n-1)*x^(n-1) + ... + a(1)*x + a(0):

INPUT degree n; polynomial coefficients a(0), a(1), ..., a(n).

++++++++++++++++++++++++++++ COMPILING HINTS +++++++++++++++++++++++++++++++++
To compile this program you may need to increase the stack size.  On an IBM PC
using Microsoft C 5.0, compile this program with the stack switch /F.

	Example: CL 028A.C /F 1000        (For a 4Kbyte stack in hex)

Note: This program may not work if compiled with the "/qc" compiler switch
      to invoke the QuickC compiler above.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

OUTPUT all n roots (real, complex, single, and multiple) of P(x).

NOTE: This algorithm was included as a "Professor's Favorite, Must-Have"
algorithm.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities.  */
#include "naautil3.c"		/* For complex vector routines.  (cvector()) */

char *outfile = "028A.out";	/* Customized default output file name.      */
double TOL;


main()
{
  fcomplex *a, *roots;
  int i, n, polish=1, ch, REALFLAG;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Complex Polynomial Solver (CPOLY) - Algorithm 2.8A\n\n");

  printf("Enter Tolerance TOL (2.0e-6)?: ");
  scanf("%lf", &TOL);
  fprintf(file_id, "Tolerance: %lg\n\n", TOL);

  printf("Are any of the coefficients imaginary? (Y/N) N: ");
  getchar();			/* Cleans-up after scanf().    */
  ch = tolower(getchar());
  if (ch == 'y')
    REALFLAG = FALSE;		/* Has complex coefficients.   */
  else
    REALFLAG = TRUE;		/* Has only real coefficients. */

  do {
    printf("\nEnter degree of polynomial: ");	/* Enter n */
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greater than zero.\n");
  } while (n <= 0);

  /* Dynamically allocate memory for the needed arrays. */
  a     = cvector(0,n);		/* For coefficients. */
  roots = cvector(0,n);		/* For roots.        */

  if (REALFLAG == TRUE)	{ /* Prompts for real-only polynomial coefficients. */
    for (i=n;i>=0;i--) {
      printf("Enter a(%d): ", i);
      scanf("%f", &a[i].r);
      a[i].i = 0.0;
    }
  } else {		/* Prompts for complex polynomial coefficients.     */
    for (i=n;i>=0;i--) {
      printf("Enter Re(a(%d)): ", i);
      scanf("%f", &a[i].r);
      printf("Enter Im(a(%d)): ", i);
      scanf("%f", &a[i].i);
    }
  }

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  zroots(a, n, roots, polish);	/* Solves for all n roots. */

  /***********
   * OUTPUTS *
   ***********/

  print_poly(a, n, REALFLAG);	/* Prints any Polynomial nicely. */

  /* STEP #2 */
  printf2("\nRoots of P(x) are:\n");
  for (i=1;i<=n;i++) {
    printf2("\t");
    if (roots[i].i < 0.0)
      printf2("% g - i%g\n", roots[i].r, -roots[i].i);
    else if (roots[i].i == 0.0)
      printf2("% g\n", roots[i].r);
    else
      printf2("% g + i%g\n", roots[i].r, roots[i].i);
  }

  /* Free the memory that was dynamically allocated for the arrays. */
  free_cvector(roots,0,n);
  free_cvector(a,0,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* laguer() - Uses Laguerre's method to converge to a root of a polynomial.  */
/*            Came from the book "Numerical Recipes in C" with alterations.  */
/*****************************************************************************/
#define	EPSS	6.0e-8		/* roundoff error estimate for float. */
#define	MAXIT	100		/* maximum number of iterations.      */

laguer(a, m, x, tol, polish)
fcomplex *a, *x;
int m, polish;
float tol;

/*
**  Given the degree m and the m+1 complex coefficients a[0..m] of the
**  polynomial äa[i]*x^i for i = 0,..., m, and given tol the desired fractional
**  accuracy, and given a complex value x, this ruotine improves x by
**  Laguerre's method until it converges to a root of the given polynomial.
**  For normal use polish should be input as FALSE (0).  When polish is input
**  as true (1), the routine ignores tol and instead attempts to improve x
**  (assumed to be a good initial guess) to the achievable roundoff limit.
*/

{
  int j, iter;
  float err, dxold, cdx, abx;
  fcomplex sq, h, gp, gm, g2, g, b, d, dx, f, x1;

  dxold = Cabs(*x);

  /* Loop over iterations up to allowed maximum. */
  for (iter=1;iter<=MAXIT;iter++) {
    b = a[m];
    err = Cabs(b);
    d = Complex(0.0, 0.0);
    f = Complex(0.0, 0.0);
    abx = Cabs(*x);

    /* Efficient computation of the polynomial and its first two derivatives.*/
    for (j=m-1;j>=0;j--) {
      f = Cadd(Cmul(*x, f), d);
      d = Cadd(Cmul(*x, d), b);
      b = Cadd(Cmul(*x, b), a[j]);
      err = Cabs(b) + abx*err;
    }

    err *= EPSS;  /* Estimate of roundoff error in evaluating polynomial. */
    if (Cabs(b) <= err) return;  /* We are on the root. */

    g  = Cdiv(d, b);  /* The generic case: use Laguerre's formula. */
    g2 = Cmul(g, g);
    h  = Csub(g2, RCmul(2.0, Cdiv(f, b)));
    sq = Csqrt(RCmul((float) (m-1), Csub(RCmul((float) m, h), g2)));
    gp = Cadd(g, sq);
    gm = Csub(g, sq);

    if (Cabs(gp) < Cabs(gm))  gp = gm;

    if (gp.r != 0.0 || gp.i != 0.0)	/* Avoids division by zero. */
      dx = Cdiv(Complex((float) m, 0.0), gp);
    x1 = Csub(*x, dx);

    if (x->r == x1.r && x->i == x1.i)  return;	/* Converged.              */
    *x  = x1;
    cdx = Cabs(dx);

    if (iter > 6 && cdx >= dxold)  return;	/* Reached roundoff limit. */
    dxold = cdx;

    if (!polish)
      if (cdx <= tol*Cabs(*x))  return;		/* Converged.              */
  }
  naaerror("Too many iterations in routine LAGUER");
}

/*****************************************************************************/
/* zroots() - Solves for the complex roots of a polynomial.  Came from the   */
/*            book "Numerical Recipes in C" with alterations.	             */
/*****************************************************************************/
zroots(a, m, roots, polish)
fcomplex *a, *roots;
int m, polish;

/*
**  Given the degree m and the m+1 complex coefficients a[0..m] of the
**  polynomial äa[i]*x^i for i = 0,..., m, this routine successively calls
**  laguer() and finds all m complex roots in roots[1..m].  The logical
**  variable polish should be input as TRUE (1) if polishing (also by
**  Laguerre's method) is desired, FALSE (0) if the roots will be subsequently
**  polished by other means.
*/

{
  int jj, j, i;
  fcomplex x, b, c, *ad;

  ad = cvector(0,m);

  /* Copy coefficients for succesive deflation. */
  for (j=0;j<=m;j++)
    ad[j] = a[j];

  for (j=m;j>=1;j--) {		/* Loop over each root to be found .*/

    /* Start at zero to favor convergence to smallest remaining root. */
    x = Complex(0.0, 0.0);
    laguer(ad, j, &x, TOL, 0);	/* Find the root. */

    if (fabs(x.i) <= (2.0*TOL*fabs(x.r)))  x.i = 0.0;
    roots[j] = x;
    b = ad[j];			/* Forward deflation. */
    for (jj=j-1;jj>=0;jj--) {
      c = ad[jj];
      ad[jj] = b;
      b = Cadd(Cmul(x, b), c);
    }
  }

  if (polish)
    for (j=1;j<=m;j++)	/* Polish roots using the undeflated coefficients. */
      laguer(a, m, &roots[j], TOL, 1);

  /* Sort roots by their real parts by straight insertion. */
  for (j=2;j<=m;j++) {
    x = roots[j];
    for (i=j-1;i>=1;i--) {
      if (roots[i].r <= x.r)  break;
      roots[i+1] = roots[i];
    }
    roots[i+1] = x;
  }
}

/*****************************************************************************/
/* print_poly() - Prints out a complex valued polynomial formatted nicely.   */
/*                Example: P(x) = (-1 + i5)*X^4 - 2*X^2 + (2 - i3)*X + 7     */
/*****************************************************************************/
print_poly(a, n, REALFLAG)
fcomplex *a;
int n, REALFLAG;
{
  int i;

  printf2("P(x) = ");
  if (REALFLAG) {		/* Enter real only polynomial coefficients */
    for (i=n;i>=1;i--) {	/* Special case: a(n), ..., a(1).          */
      if (a[i].r != 0.0) {	/* Don't print if coefficient is zero.     */
        if (a[i].r > 0.0) {	/* positive real part.                     */
          if (a[i].r == 1.0) {	/* Special case: coefficient = 1.          */
            if (i == 1)  printf2("X");			/* a(1) */
            else         printf2("X^%d", i);
          } else {
            if (i == 1)  printf2("%g*X", a[i].r);	/* a(1) */
            else         printf2("%g*X^%d", a[i].r, i);
          }
        } else {		/* negative real part.             */
          if (a[i].r == -1.0) {	/* Special case: coefficient = -1. */
            if ((i == n) && (i != 1))       printf2("-X^%d", i);  /* a(n) */
            else if ((i == n) && (i == 1))  printf2("-X");        /* a(n) */
            else if (i == 1)                printf2("X");         /* a(1) */
            else                            printf2("X^%d", i);
          } else {				        /* a(n) */
            if ((i == n) && (i != 1))  printf2("-%g*X^%d", -a[i].r, i);
            else if (i == 1)           printf2("%g*X", -a[i].r);  /* a(1) */
            else                       printf2("%g*X^%d", -a[i].r, i);
          }
        }
      }
      if (a[i-1].r > 0.0)     /* look at next coefficient to determine sign. */
        printf2(" + ");
      else if (a[i-1].r < 0.0)
        printf2(" - ");
    }
    if (a[0].r > 0.0)		/* special case:  a(0).                    */
      printf2("%g\n", a[0].r);
    else if (a[0].r < 0.0)
      printf2("%g\n", -a[0].r);

  } else {			/* Enter complex polynomial coefficients   */

    for (i=n;i>=1;i--) {	/* Special case: a(n),..., a(1).           */
      if (a[i].i < 0.0)	{	/* Special case: negative imaginary coeff. */
        if (i == 1)  printf2("(%g - i%g)*X", a[i].r, -a[i].i);	   /* a(1) */
        else         printf2("(%g - i%g)*X^%d", a[i].r, -a[i].i, i);
      }

      if (a[i].i == 0.0) {	/* Special case: zero imaginary coeff. */
        if (a[i].r != 0.0) {	/* Print nothing if Re = Im = 0.       */

          if (a[i].r > 0.0) {	/* positive real part.                 */
            if (a[i].r == 1.0) {	/* Special case: coefficient = 1. */
              if (i == 1)       printf2("X");		/* a(1) */
              else              printf2("X^%d", i);
            } else {
              if (i == 1)       printf2("%g*X", a[i].r);/* a(1) */
              else              printf2("%g*X^%d", a[i].r, i);
            }
          } else {		/* negative real part. */
            if (a[i].r == -1.0) {	/* Special case: coefficient = -1. */
              if (i == n)       printf2("-X^%d", i);	/* a(n) */
              else if (i == 1)  printf2("X");		/* a(1) */
              else              printf2("X^%d", i);
            } else {
              if (i == n)       printf2("-%g*X^%d", -a[i].r, i);  /* a(n) */
              else if (i == 1)  printf2("%g*X", -a[i].r);         /* a(1) */
              else              printf2("%g*X^%d", -a[i].r, i);
            }
          }
        }
      }

      if (a[i].i > 0.0)	/* Special case: positive imaginary coeff. */
        if (i == 1)  printf2("(%g + i%g)*X", a[i].r, a[i].i); /* a(1)*/
        else         printf2("(%g + i%g)*X^%d", a[i].r, a[i].i, i);

      /* look at next coefficient to determine sign. */
      if (!(a[i-1].r == 0.0 && a[i-1].i == 0.0)) {
        if (a[i-1].r < 0.0 && a[i-1].i == 0.0)
          printf2(" - ");
        else
          printf2(" + ");
      }
    }

    if (a[0].i < 0.0)		/* Special case: a(0). */
      printf2("(%g - i%g)\n", a[0].r, -a[0].i);
    else if (a[0].i == 0.0) {
      if (a[0].r != 0.0)
        printf2("%g\n", a[0].r);
    } else			/* if (a[0].i > 0.0)   */
      printf2("(%g + i%g)\n", a[0].r, a[0].i);
  }
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
