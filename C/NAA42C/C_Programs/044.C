/******************************************************************************
	Composite Simpson's Rule for Double Integrals - Algorithm 4.4
*******************************************************************************

                                ôb ôd(x)
To approximate the integral I = ³  ³     f(x,y) dy dx:
                                õa õc(x)

INPUT endpoints a, b; positive integers m, n; the function f(); the functions
c(x) and d(x) as needed.

OUTPUT approximation J to I.

NOTE: Listed as Simpson's Composite Rule for Double Integrals in 3rd edition
of the text.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "044.out";	/* Customized default output file name.     */
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
  double a, b, cc, dd, h, HX, J, J1, J2, J3, K1, K2, K3;
  double L, x, y, Z, c(), d(), f();
  int i, j, m, n, ch, CONST;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Composite Simpson's Rule for Double Integrals - Algorithm 4.4\n\n");
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
  printf("\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  h  = (b - a)/(2*n);
  J1 = 0.0;			/* End terms.  */
  J2 = 0.0;			/* Even terms. */
  J3 = 0.0;			/* Odd terms.  */

  /* STEP #2 */
  for (i=0;i<=2*n;i++) {	/* Do Steps 3-8. */

    /* STEP #3 */
    x = a + i*h;		/* Composite Simpson's method for x.         */
    if (CONST == FALSE) {	/* Use c(x) and d(x) functions.              */
      cc = c(x);		/* Use cc and dd to save (2*n+1)*(2+(2*m-1)) */
      dd = d(x);		/* ... functional evaluations.               */
    }
    HX = (dd - cc)/(2*m);	/* Use c(x) and d(x) functions.              */
    K1 = f(x,cc) + f(x,dd);	/* End terms for each x.                     */
    K2 = 0.0;			/* Even terms for each x.                    */
    K3 = 0.0;			/* Odd terms for each x.                     */

    /* STEP #4 */
    for (j=1;j<2*m;j++) {	/* Do Steps 5 and 6. */

      /* STEP #5 */
      y = cc + j*HX;
      Z = f(x,y);

      /* STEP #6 */
      if (j % 2 == 0)  K2 += Z;	/* For even j. */
      else             K3 += Z;	/* For odd j.  */
    }

    /* STEP #7 */
    L = (K1 + 2.0*K2 + 4.0*K3)*HX/3.0;

    /*
    **	(     ô d(x(i))						)
    **	( L ÷ ³     f(x(i),y) dy  by Composite Simpson's method.)
    **	(     õ c(x(i))						)
    */

    /* STEP #8 */
    if (i == 0 || i == 2*n)
      J1 += L;			/* For end-points. */
    else if (i % 2 == 0)
      J2 += L;			/* For even i. */
    else
      J3 += L;			/* For odd i.  */
  }

  /* STEP #9 */
  J = (J1 + 2.0*J2 + 4.0*J3) * h/3.0;

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #10 */
  if (CONST) {
    printf2("    ô%-4lg  ô%-4lg\n", b, dd);
    printf2("I = ³      ³     f(x,y) dy dx = %.11lg\n", J);
    printf2("    õ%-4lg  õ%-4lg\n\n", a, cc);
    printf2("Required %d functional evaluations.\n", (2*n+1)*(2*m+1));
  } else {
    printf2("    ô%-4lg  ôd(x)\n", b);
    printf2("I = ³      ³     f(x,y) dy dx = %.11lg\n", J);
    printf2("    õ%-4lg  õc(x)\n\n", a);
    printf2("Required %d functional evaluations.\n", (2*n+1)*(2*m+3));
  }

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
