/******************************************************************************
			Horner's Method - Algorithm 2.6
*******************************************************************************

To evaluate the polynomial

	P(x) = a(n)*x^(n) + a(n-1)*x^(n-1) + ... + a(1)*x + a(0)

and its derivatives at x0:

INPUT degree n; coefficients a(0), a(1),..., a(n); x0:

OUTPUT y = P(x0); z = P'(x0).

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "026.out";	/* Customized default output file name.     */


main()
{
  double *a, x0, y, z;
  int i, j, n;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Horner's Method - Algorithm 2.6\n\n");

  do {
    printf("Enter degree of polynomial: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greater than zero.\n");
  } while (n <= 0);

  /* Dynamically allocate memory for the needed arrays. */
  a = dvector(0,n);

  for (i=n;i>=0;i--) {		/* Prompts for real polynomial coefficients */
    printf("Enter a(%d): ", i);
    scanf("%lf", &a[i]);
  }

  printf("\nEnter x0: ");		/* Enter x(0).           */
  scanf("%lf", &x0);

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  y = a[n];				/* Compute b(n) for P.   */
  z = a[n];				/* Compute b(n-1) for Q. */

  /* STEP #2 */
  for (j=n-1; j>=1; j--) {
    y = x0*y + a[j];			/* Compute b(j) for P.   */
    z = x0*z + y;			/* Compute b(j-1) for Q. */
  }

  /* STEP #3 */
  y = x0*y + a[0];			/* Compute b(0) for P.    */

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #4 */
  print_poly(a, n);		/* Prints a real Polynomial nicely. */
  printf2("\nx0 = %.11lg\n", x0);
  printf2("P(%lg) = %.11lg\n", x0, y);
  printf2("P'(%lg) = %.11lg\n", x0, z);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(a,0,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* print_poly() - Prints out a real valued polynomial formatted nicely.      */
/*                Example: P(x) = -X^4 - 2*X^2 + 3*X + 7                     */
/*****************************************************************************/
print_poly(a, n)
double *a;
int n;
{
  int i;

  printf2("P(x) = ");
  for (i=n;i>=1;i--) { 		/* Special case: a(n), ..., a(1).      */
    if (a[i] != 0.0) {		/* Don't print if coefficient is zero. */
      if (a[i] > 0.0) {		/* positive real part.                 */
        if (a[i] == 1.0) {	/* Special case: coefficient = 1.      */
          if (i == 1)  printf2("X");  /* a(1) */
          else         printf2("X^%d", i);
        } else {
          if (i == 1)  printf2("%g*X", a[i]);  /* a(1) */
          else         printf2("%g*X^%d", a[i], i);
        }
      } else {			/* negative real part.             */
        if (a[i] == -1.0) {	/* Special case: coefficient = -1. */
          if ((i == n) && (i != 1))       printf2("-X^%d", i);  /* a(n) */
          else if ((i == n) && (i == 1))  printf2("-X");        /* a(n) */
          else if (i == 1)                printf2("X");         /* a(1) */
          else                            printf2("X^%d", i);
        } else {				    /* a(n) */
          if ((i == n) && (i != 1))  printf2("-%g*X^%d", -a[i], i);
          else if (i == 1)           printf2("%g*X", -a[i]);  /* a(1) */
          else                       printf2("%g*X^%d", -a[i], i);
        }
      }
    }
    if (a[i-1] > 0.0)     /* look at next coefficient to determine sign. */
      printf2(" + ");
    else if (a[i-1] < 0.0)
      printf2(" - ");
  }

  if (a[0] > 0.0)		/* special case:  a(0). */
    printf2("%g\n", a[0]);
  else if (a[0] < 0.0)
    printf2("%g\n", -a[0]);
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
