/******************************************************************************
	Composite Trapezoid Rule for Double Integrals - Algorithm 4.4B
*******************************************************************************

                                ôb ôd(x)
To approximate the integral I = ³  ³     f(x,y) dy dx:
                                õa õc(x)

INPUT endpoints a, b; positive integers m, n; the function f() or tabulated
data points; the functions c(x) and d(x) as needed.

OUTPUT approximation J to I.

NOTE: This algorithm was included as a "Homework Helper."  See p. 215,
Exercise Set 4.8, Problems *** 5-6 ***.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "044b.out";	/* Customized default output file name.     */
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


main()
{
  double a, b, cc, dd, h, HX, J, J1, J2, K1, K2, **F;
  double L, x, y, Z, c(), d(), f();
  int i, j, m, n, ch, ch2, CONST, DATA;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Composite Trapezoid Rule for Double Integrals");
  printf2(" - Algorithm 4.4B\n\n");
  printf2("%s\n", eq_text_f);
  printf2("%s\n", eq_text_c);
  printf2("%s\n\n", eq_text_d);

  printf("Enter endpoint a: ");
  scanf("%lf", &a);
  printf("Enter endpoint b: ");
  scanf("%lf", &b);
  fprintf(file_id, "For %lg ó x ó %lg", a, b);

  printf("Are c(x) and d(x) constants? (Y/N) N: ");
  getchar();				/* Cleans up after scanf(). */
  ch = tolower(getchar());
  if (ch == 'y') {
    printf("Enter endpoint c: ");
    scanf("%lf", &cc);
    printf("Enter endpoint d: ");
    scanf("%lf", &dd);
    fprintf(file_id, " and %lg ó y ó %lg\n\n", cc, dd);

    CONST = TRUE;	/* CONST = TRUE if c(x) and d(x) are constants. */

    printf("Does f(x,y) come from a table? (Y/N) N: ");
    getchar();			/* Cleans up after scanf().       */
    ch2 = tolower(getchar());
    if (ch2 == 'y')  DATA = TRUE;	/* Will use discrete data points. */
    else             DATA = FALSE;	/* Will evaluate f(x,y).          */

  } else {
    fprintf(file_id, " and c(x) ó y ó d(x)\n\n");
    DATA  = FALSE;
    CONST = FALSE;
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
  fprintf(file_id, "m = %d intervals on [c(x),d(x)].\n\n", m);

  if (DATA == TRUE) {		/* Read in values from table. */
    F = dmatrix(0,n,0,m);
    for (i=0;i<=n;i++)
      for (j=0;j<=m;j++) {
        printf("Enter f(%lg,%lg): ", a + i*(b-a)/n, cc + j*(dd-cc)/m);
        scanf("%lf", &F[i][j]);
      }
  }
  printf("\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  h  = (b - a)/n;
  J1 = 0.0;			/* End terms.      */
  J2 = 0.0;			/* Internal terms. */

  /* STEP #2 */
  for (i=0;i<=n;i++) {		/* Do Steps 3-8. */

    /* STEP #3 */
    x = a + i*h;		/* Composite Trapezoid Rule for fixed x.   */
    if (CONST == FALSE) {	/* Use c(x) and d(x) functions.            */
      cc = c(x);		/* Use cc and dd to save (n+1)*(2+(m-1))   */
      dd = d(x);		/* ... functional evaluations.             */
    }
    HX = (dd - cc)/m;		/* Use c(x) and d(x) functions.            */
    if (DATA == TRUE)  K1 = F[i][0] + F[i][m];
    else               K1 = f(x,cc) + f(x,dd); /* End terms for each x.    */
    K2 = 0.0;			/* Internal terms for each x.              */

    /* STEP #4 */
    for (j=1;j<m;j++) {		/* Do Steps 5-6. */

      /* STEP #5 */
      if (DATA == TRUE) {
        Z = F[i][j];
      } else {
        y = cc + j*HX;
        Z = f(x,y);
      }

      /* STEP #6 */
      K2 += Z;			/* K2 = K2 + Z. */
    }

    /* STEP #7 */
    L = HX * (K1 + 2.0*K2)/2.0;

    /*
    **	(     ô d(x(i))						)
    **	( L ÷ ³     f(x(i),y) dy  by Composite Trapezoid Method.)
    **	(     õ c(x(i))						)
    */

    /* STEP #8 */
    if ((i == 0) || (i == n))
      J1 += L;			/* J1 = J1 + L. */
    else
      J2 += L;			/* J2 = J2 + L. */
  }

  /* STEP #9 */
  J = h * (J1 + 2.0*J2)/2.0;

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #10 */
  if (CONST == TRUE) {
    printf2("    ô%-4lg  ô%-4lg\n", b, dd);
    printf2("I = ³      ³     f(x) dy dx = %.11lg\n", J);
    printf2("    õ%-4lg  õ%-4lg\n\n", a, cc);
    if (DATA == FALSE)
      printf2("Required %d functional evaluations.\n", (n+1)*(m+1));
  } else {
    printf2("    ô%-4lg  ôd(x)\n", b);
    printf2("I = ³      ³     f(x) dy dx = %.11lg\n", J);
    printf2("    õ%-4lg  õc(x)\n\n", a);
    if (DATA == FALSE)
      printf2("Required %d functional evaluations.\n", (n+1)*(m+3));
  }

  /* Free the memory that was dynamically allocated for the arrays. */
  if (DATA == TRUE)
    free_dmatrix(F,0,n,0,m);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
