/******************************************************************************
	Composite Simpson's Rule for Triple Integrals - Algorithm 4.5
*******************************************************************************

                                �b �d(x) ��(x,y)
To approximate the integral I = �  �     �       f(x,y,z) dx dy dz:
                                �a �c(x) ��(x,y)

INPUT endpoints a, b; positive integers m, n, p; the function f(); the
functions c(x) and d(x) as needed; the functions �(x,y) and �(x,y) as needed.

OUTPUT approximation J to I.

NOTE: Newly introduced into the 4th edition of the text.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities.  */

char *outfile        = "045.out";  /* Customized default output file name.   */
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
  double J, J1, J2, J3, K, K1, K2, K3, L, L1, L2, L3;
  double x, y, z, c(), d(), alphaf(), betaf(), f();
  int i, j, k, m, n, p, ch, CONST1, CONST2;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Composite Simpson's Rule for Triple Integrals - Algorithm 4.5\n\n");
  printf2("                                �b �d(x) ��(x,y)\n");
  printf2("To approximate the integral I = �  �     �       f(x,y,z)");
  printf2(" dx dy dz\n");
  printf2("                                �a �c(x) ��(x,y)\n\n");

  printf2("%s\n", eq_text_f);
  printf2("%s\n", eq_text_c);
  printf2("%s\n", eq_text_d);
  printf2("%s\n", eq_text_alphaf);
  printf2("%s\n\n", eq_text_betaf);

  printf("Enter endpoint a: ");
  scanf("%lf", &a);
  printf("Enter endpoint b: ");
  scanf("%lf", &b);
  fprintf(file_id, "For %lg � x � %lg", a, b);

  printf("Are c(x) and d(x) constants? (Y/N) N: ");
  getchar();			/* Cleans up after scanf(). */
  ch = tolower(getchar());
  if (ch == 'y') {
    printf( "Enter endpoint c: ");
    scanf("%lf", &cc);
    printf("Enter endpoint d: ");
    scanf("%lf", &dd);
    fprintf(file_id, " and %lg � y � %lg", cc, dd);

    CONST1 = TRUE;	/* CONST1 = TRUE if c(x) and d(x) are constants.  */
  } else {
    fprintf(file_id, " and c(x) � y � d(x)");
    CONST1 = FALSE;	/* CONST1 = FALSE if c(x) and d(x) are functions. */
  }

  printf("Are �(x,y) and �(x,y) constants? (Y/N) N: ");
  getchar();			/* Cleans up after scanf(). */
  ch = tolower(getchar());
  if (ch == 'y') {
    printf("Enter endpoint alpha: ");
    scanf("%lf", &alpha);
    printf("Enter endpoint beta: ");
    scanf("%lf", &beta);
    fprintf(file_id, " and %lg � z � %lg\n\n", alpha, beta);

    CONST2 = TRUE;	/* CONST2 = TRUE if �(x,y) and �(x,y) are constants. */
  } else {
    fprintf(file_id, " and �(x,y) � z � �(x,y)\n\n");
    CONST2 = FALSE;	/* CONST2 = FALSE if �(x,y) and �(x,y) are functions.*/
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
  h  = (b - a)/(2*n);
  J1 = 0.0;			/* End terms.  */
  J2 = 0.0;			/* Even terms. */
  J3 = 0.0;			/* Odd terms.  */

  /* STEP #2 */
  for (i=0;i<=2*n;i++) {	/* Do Steps 3-12.       */

    printf(".");		/* For THINKING output. */

    /* STEP #3 */
    x = a + i*h;		/* Composite Simpson's method for fixed x.   */
    if (CONST1 == FALSE) {	/* Use c(x) and d(x) functions.              */
      cc = c(x);		/* Use cc and dd to save (2*n+1)*(2+(2*m+1)) */
      dd = d(x);		/* ... functional evaluations.               */
    }
    HX = (dd - cc)/(2*m);
    K1 = 0.0;			/* End terms for each x.  */
    K2 = 0.0;			/* Even terms for each x. */
    K3 = 0.0;			/* Odd terms for each x.  */

    /* STEP #4 */
    for (j=0;j<=2*m;j++) {	/* Do Steps 5-10.         */

      /* STEP #5 */
      y = cc + j*HX;		/* The Composite Simpson's method for fixed  */
				/* ... x and y.                              */
      if (CONST2 == FALSE) {	/* Use alphaf(x,y) and betaf(x,y) functions. */
        alpha = alphaf(x,y);	/* Use alpha and beta to save lots of integ- */
        beta  = betaf(x,y);	/* ... grand limit functional evaluations.   */
      }
      HY = (beta - alpha)/(2.0*p);
      L1 = f(x,y,alpha) + f(x,y,beta);	/* End terms.  */
      L2 = 0.0;				/* Even terms. */
      L3 = 0.0;				/* Odd terms.  */

      /* STEP #6 */
      for (k=1;k<2*p;k++) {	/* Do Steps 7-8. */

        /* STEP #7 */
        z = alpha + k*HY;
        Q = f(x,y,z);

        /* STEP #8 */
        if (k % 2 == 0)
          L2 += Q;		/* For even k. */
        else
          L3 += Q;		/* For odd k.  */
      }

      /* STEP #9 */
      L = (L1 + 2.0*L2 + 4.0*L3)*HY / 3.0;

      /*
      **  (     �beta(x(i),y(j))					)
      **  ( L � �     f(x(i),y(j),z) dz  by Composite Simpson's Method. )
      **  (     �alpha(x(i),y(j))					)
      */

      /* STEP #10 */
      if (j == 0 || i == 2*m)
        K1 += L;
      else if (j % 2 == 0)
        K2 += L;		/* For even j. */
      else
        K3 += L;		/* For odd j.  */
    }

    /* STEP #11 */
    K = (K1 + 2.0*K2 + 4.0*K3)*HX / 3.0;

    /*
    **	(     �d(x(i))  �beta(x(i),y(j))                                     )
    **	( K � �         �   f(x(i),y,z) dz dy  by Composite Simpson's Method.)
    **	(     �c(x(i))  �alpha(x(i),y(j))                                    )
    */

    /* STEP #12 */
    if (i == 0 || i == 2*n)
      J1 += K;			/* For end-points. */
    else if (i % 2 == 0)
      J2 += K;			/* For even i. */
    else
      J3 += K;			/* For odd i.  */
  }

  /* STEP #13 */
  J = (J1 + 2.0*J2 + 4.0*J3)*h / 3.0;

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #14 */
  printf("\n\n");
  if ((CONST1 == TRUE) && (CONST2 == TRUE)) {
    printf2("    �%-4lg  �%-4lg  �%-4lg\n", b, dd, beta);
    printf2("I = �      �      �     f(x,y,z) dx dy dz = %.11lg\n", J);
    printf2("    �%-4lg  �%-4lg  �%-4lg\n\n", a, cc, alpha);
    printf2("Required %ld functional evaluations.\n",
	 (long) (2*n + 1)*(2*m + 1) * (long) (2)*(2*p - 1));
  } else {
    printf2("    �%-4lg  �d(x)  �beta(x,y)\n", b);
    printf2("I = �      �      �         f(x,y,z) dx dy dz = %.11lg\n", J);
    printf2("    �%-4lg  �c(x)  �alpha(x,y)\n\n", a);
    printf2("Required %ld functional evaluations.\n",
	 (long) (2*n + 1)*(2)*(2*m + 1) * (long) (4)*(2*p - 1));
  }

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
