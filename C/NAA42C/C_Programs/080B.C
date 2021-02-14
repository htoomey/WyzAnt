/******************************************************************************
	    Least-Squares Polynomial Approximation - Algorithm 8.0B
*******************************************************************************

To determine a Least-Squares polynomial of degree n given m data points or
the function f(x).  Uses NORMAL EQUATIONS on page 431 for discrete data.
Uses NORMAL EQUATIONS on page 440 for f(x).  Uses Simpson's rule to
approximate the integrals needed for continuous functions.

INPUT Degree of polynomial, n; number of data points, m; function f() if
desired; point to evaluate P(x) at, x.

OUTPUT Coefficients of an nth degree polynomial and optional values of P(x)
evaluated from the polynomial at x.

NOTE: This algorithm was included as a "Homework Helper."  See p. 435,
Exercise Set 8.1, Problems 1-13.  Also see p. 449, Exercise Set 8.2, Problems
1-7.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */
#include "gaussj.c"		/* To solve a matrix.                       */

char *outfile   = "080b.out";	/* Customized default output file name.     */
char *eq_text_f = "f(x) = sin(PI*X)";	/* Needs updating  $  */


/*****************************************************************************/
/* f(x) - Function to evaluate, f(x).  May not be needed.  Needs updating $. */
/*****************************************************************************/
double f(x)
double x;
{
  if (eqeval)
    return (eval_eq(x));	/* Use the Equation Evaluator  */
  else
    return (sin(PI*x));		/* Use the default function.   */
}
/*****************************************************************************/


main()
{
  double **A, *X, *Y, **AA, **A1, **AA1, f(), simpson(), x;
  double a, b;			/* Interval values. */
  int i, j, k, n, m, ch;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Least-Squares Polynomial Approximation - Algorithm 8.0B\n\n");

  if (eqeval)
    printf2("f(x) = %s", tmpstr);	/* Print the Equation Evaluator text */
  else
    printf2("%s", eq_text_f);		/* Print the default equation text */
  printf2("\n\n");

  do {
    printf("Enter the degree of the polynomial, n: ");
    scanf("%d", &n);
    if (n < 0)
      printf("ERROR - n must be greater than or equal to zero\n.");
  } while (n < 0);

  /* Dynamically allocate memory for the needed arrays. */
  AA  = dmatrix(1,n+1,1,n+1);	/* Summation matrix                 */
  A   = dmatrix(0,n+1,1,1);	/* Polynomial coefficients          */
  AA1 = dmatrix(1,n+1,1,n+1);	/* Saves AA before calling gaussj() */
  A1  = dmatrix(0,n+1,1,1);	/* Saves A before calling gaussj()  */

  printf("Will you evaluate a function, f(x), or tabulated data? (F/D) F: ");
  getchar();			/* Cleans up after scanf(). */
  ch = tolower(getchar());

  /*++++++++++++++++++++++ For Tabulated Data +++++++++++++++++++++++++++++*/
  if (ch == 'd') {		/* For Tabulated Data. */

    do {
      printf("How many points need evaluating?: ");
      scanf("%d", &m);

      if (m <= 0)
        printf("ERROR - m must be greater than zero.\n");

      if (n >= m-1) {
        printf("ERROR - Not enough data points for a polynomial");
        printf(" of that degree.\n");
      }
    } while (m <= 0 || n >= m-1);

    /* Dynamically allocate memory for the needed arrays.  */
    X = dvector(1,m);		/* X input                 */
    Y = dvector(1,m);		/* Y input or f(x) points. */

    /* Get (x,y) points. */
    for (i=1;i<=m;i++) {	/* Enter X(i).             */
      printf("\nEnter x%d: ", i);
      scanf("%lf", &X[i]);
      printf("Enter y%d: ", i);
      scanf("%lf", &Y[i]);
    }

    /*************
     * ALGORITHM *
     *************/

    /* Build matrix elements for AX = B type solution. */
    /* STEP #1 */
    for (i=0;i<=n;i++) {
      for (j=0;j<=n;j++) {
        AA[i+1][j+1] = 0.0;
        for (k=1;k<=m;k++)
          if ((i+j) != 0)
            AA[i+1][j+1] += pow(X[k], (double) (i+j));
          else
            AA[i+1][j+1]++;
      }

      /* STEP #2 */
      A[i+1][1] = 0.0;
      for (k=1;k<=m;k++)
        if (i != 0)
          A[i+1][1] += Y[k] * pow(X[k], (double) (i));
        else
          A[i+1][1] += Y[k];
    }

  /*++++++++++++++++++++++ For Functions ++++++++++++++++++++++++++++++++++*/
  } else {		/* For Functions. */

    ch = 'f';

    printf("Enter lower interval value, a: ");
    scanf("%lf", &a);
    printf("Enter upper interval value, b: ");
    scanf("%lf", &b);

    /*************
     * ALGORITHM *
     *************/

    printf("Computing integrals with Simpson's Rule");
    fprintf(file_id, "Computes integrals using Simpson's Rule\n");

    /* Build matrix elements for AX=B type solution. */
    /* STEP #1 */
    for (i=0;i<=n;i++) {
      for (j=0;j<=n;j++)
        AA[i+1][j+1] = simpson(1,i+j,a,b);	/* For x^(i+j)  */
      A[i+1][1] = simpson(2,i,a,b);		/* For x^i*f(x) */
    }
  }

  /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

  for (i=0;i<=n+1;i++)		/* Save A since gaussj() destroys it.  */
    A1[i][1] = A[i][1];

  for (i=1;i<=n+1;i++)		/* Save AA since gaussj() destroys it. */
    for (j=1;j<=n+1;j++)
      AA1[i][j] = AA[i][j];

  gaussj(AA,n+1,A,1);		/* Solve the matrix on p. 431  */

  for (i=0;i<=n;i++)		/* Want A[0..n] not A[1..n+1]. */
    A[i][1] = A[i+1][1];

  /***********
   * OUTPUTS *
   ***********/

  if (ch == 'd') {		/* For Tabulated Data.  */
    printf2("i\t x\t y\n");	/* Output (x,y) points. */
    printf2("--------------------------\n");
    for (i=1;i<=m;i++)
      printf2("%d\t %lG\t% lG\n", i, X[i], Y[i]);
    printf2("\n");

    /* Free the memory that was dynamically allocated for the arrays. */
    free_dvector(Y,1,m);
    free_dvector(X,1,m);

  }

  if (ch == 'f') {		/* For Functions. */
    printf2("\nThe lower interval value, a, is: %lg\n", a);
    printf2("The upper interval value, b, is: %lg\n\n", b);
  }

  printf2("The polynomial coefficient matrix is:\n\n");
  for (i=1;i<=n+1;i++) {
    printf2("[ ");
    for (j=1;j<=n+1;j++)
      printf2("% 9lg ", AA1[i][j]);
    printf2("][ a%d ] = [ %9lg ]\n", i-1, A1[i][1]);
  }
  printf2("\n");
  print_poly( A, n);		/* Show results in polynomial format. */
  printf2("\n");

  printf("Would you like to evaluate P(x)? (Y/N) N: ");
  getchar();			/* Cleans up after scanf(). */
  ch = tolower(getchar());

  while (ch == 'y') {
    double tmp;

    printf("\nEnter x: ");
    scanf("%lf", &x);

    /*
    **  P(x) = ä a(i)*x^i  summed from i = 0 to n.
    */

    if (n == 0)
      tmp = A[n][1];
    else {
      tmp = A[n][1]*x;
      for (i=n-1;i>0;i--)
        tmp = (tmp + A[i][1]) * x;
      tmp += A[0][1];
    }

    printf2("P(%lg) = %.9lg\n", x, tmp);

    printf("\nWould you like to evaluate P(x) again? (Y/N) N: ");
    getchar();			/* Cleans up after scanf(). */
    ch = tolower(getchar());
  }

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dmatrix(A1,0,n+1,1,1);
  free_dmatrix(AA1,1,n+1,1,n+1);
  free_dmatrix(A,0,n+1,1,1);
  free_dmatrix(AA,1,n+1,1,n+1);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* simpson() - Composite Simpson's Rule - Algorithm 4.1 Modified.            */
/*****************************************************************************/
double simpson(type, j, a, b)
int type, j;
double a, b;
{
  double h, X, XI, XI0, XI1, XI2, f();
  int i, n = 100;

  printf(".");		/* For feedback since it takes so long. */

  /* STEP #1 */
  h = (b - a)/n;

  /* STEP #2 */
  if (type == 1) {
    if (j != 0)
      XI0 = pow(a,(double)j) + pow(b,(double)j);
    else
      return (b-a);
  } else {			/* type == 2 */
    if (j != 0)
      XI0 = pow(a,(double)j)*f(a) + pow(b,(double)j)*f(b);
    else
      XI0 = f(a) + f(b);
  }

  XI1 = 0.0;			/* Summation of f(x(2i-1)). */
  XI2 = 0.0;			/* Summation of f(x(2i)).   */

  /* STEP #3 */
  for (i=1;i<n;i++) {

    /* STEP #4 */
    X = a + i*h;

    /* STEP #5 */
    if (i % 2 == 0) {		/* For even i. */      
      if (type == 1) {
        if (j != 0)
          XI2 += pow(X,(double)j);
        else
          XI2 += f(X);
      } else {			/* type == 2 */
        if (j != 0)
          XI2 += pow(X,(double)j) * f(X);
        else
          XI2 += f(X);
      }

    } else {			/* For odd i. */
      if (type == 1) {
        if (j != 0)
          XI1 += pow(X,(double)j);
        else
          XI1 += f(X);
      } else {			/* type == 2 */
        if (j != 0)
          XI1 += pow(X,(double)j) * f(X);
        else
          XI1 += f(X);
      }
    }
  }

  /* STEP #6 */
  XI = h*(XI0 + 2.0*XI2 + 4.0*XI1) / 3.0;

  /* STEP #7 */
  return (XI);
}  				/* STOP */

/*****************************************************************************/
/* print_poly() - Prints out a Polynomial, with real coefficients, in a      */
/*                pleasant format.  Example: P(x) = -X^4 - 2*X^2 + 3*X + 7   */
/*****************************************************************************/
print_poly(a, n)
double **a;
int n;
{
  int i;

  printf2("P(x) = ");
  for (i=n;i>=1;i--) { 		/* Special case: a(n), ..., a(1).      */
    if (a[i][1] != 0.0) {	/* Don't print if coefficient is zero. */
      if (a[i][1] > 0.0) {	/* positive real part.                 */
        if (a[i][1] == 1.0) {	/* Special case: coefficient = 1.      */
          if (i == 1)  printf2("X");				/* a(1) */
          else         printf2("X^%d", i);
        } else {
          if (i == 1)  printf2("%g*X", a[i][1]);		/* a(1) */
          else         printf2("%g*X^%d", a[i][1], i);
        }
      } else {			/* negative real part.             */
        if (a[i][1] == -1.0) {	/* Special case: coefficient = -1. */
          if ((i == n) && (i != 1))       printf2("-X^%d", i);	/* a(n)*/
          else if ((i == n) && (i == 1))  printf2("-X", i);	/* a(n) */
          else if (i == 1)                printf2("X");		/* a(1) */
          else                            printf2("X^%d", i);
        } else {						/* a(n) */
          if ((i == n) && (i != 1))  printf2("-%g*X^%d", -a[i][1], i);
          else if (i == 1)           printf2("%g*X", -a[i][1]);	/*a(1)*/
          else                       printf2("%g*X^%d", -a[i][1], i);
        }
      }
    }
    if (a[i-1][1] > 0.0)     /* look at next coefficient to determine sign. */
      printf2(" + ");
    if (a[i-1][1] < 0.0)
      printf2(" - ");
  }

  if (a[0][1] > 0.0)		/* special case:  a(0). */
    printf2("%g\n", a[0][1]);
  if (a[0][1] < 0.0)
    printf2("%g\n", -a[0][1]);
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
