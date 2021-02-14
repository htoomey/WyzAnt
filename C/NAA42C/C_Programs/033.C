/******************************************************************************
		Hermite Interpolation - Algorithm 3.3
*******************************************************************************

To obtain the coefficients of the Hermite interpolatating polynomial H on the
(n + 1)-distinct numbers x0,...,xn for the function f():

INPUT numbers x0,x1,...,xn; values f(x0), f(x1),..,f(xn) and f'(x0),f'(x1),
...,f'(xn).

OUTPUT the numbers Q(0,0), Q(1,1),..., Q(2n+1,2n+1) where
    H(x) = Q(0,0) + Q(1,1)*(x-x0) + Q(2,2)*(x-x0)^2 + Q(3,3)*(x-x0)^2*(x-x1)
           + Q(4,4)*(x-x0)^2*(x-x1)^2 + ...
           + Q(2n+1,2n+1)*(x-x0)^2*(x-x1)^2 * * * (x-x(n-1))^2*(x-xn).

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile        = "033.out";  /* Customized default output file name.  */
char *eq_text_f      = "f(x) = 3Xe^X - e^(2X)";	    /* Needs updating  $  */
char *eq_text_fprime = "f'(x) = 3X + 3 - 2e^X*e^X"; /* Needs updating  $  */


/*****************************************************************************/
/* f(x) - Function to evaluate, f(x).  May be unnecessary if using tabulated */
/*        data.  Needs updating $.                                           */
/*****************************************************************************/
double f(x)
double x;
{
  return (3.0*x*exp(x) - exp(2.0*x));
}

/*****************************************************************************/
/* fprime(x) - Derivative of function to evaluate, f'(x).                    */
/*             May be unnecessary if using tabulated data.                   */
/*             Needs updating $.                                             */
/*****************************************************************************/
double fprime(x)
double x;
{
  return ((3.0*x + 3.0 - 2.0*exp(x))*exp(x));
}
/*****************************************************************************/


main()
{
  double **Q, *x, *z, *F, H, X, tmp, *diff, *diff2, *FPRIME, f(), fprime();
  int i, j, n, ch;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Hermite Interpolation - Algorithm 3.3\n\n");

  do {
    printf("Enter number of data points, n+1: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greater than zero.\n");
  } while (n <= 0);

  n--;				/* n = n - 1. */
  fprintf(file_id, "Coefficients are for a Hermite polynomial ");
  fprintf(file_id, "of degree %d.\n\n", 2*n+1);

  /* Dynamically allocate memory for the needed arrays. */
  Q      = dmatrix(0,(2*n+1),0,(2*n+1));
  x      = dvector(0,n);
  z      = dvector(0,(2*n+1));
  F      = dvector(0,n);
  FPRIME = dvector(0,n);
  diff   = dvector(0,n);
  diff2  = dvector(0,n);

  printf("Enter the x to evaluate: ");
  scanf("%lf", &X);
  fprintf(file_id, "Evaluating H(x) at x = %lg\n\n", X);

  printf("Does f(x) and f'(x) need to be evaluated? (Y/N) Y: ");
  getchar();			/* Cleans up after scanf(). */
  ch = tolower(getchar());

  for (i=0;i<=n;i++) {		/* Enter data.      */
    printf("Enter x(%d): ", i);
    scanf("%lf", &x[i]);
    if (ch == 'n') {		/* Use data points. */
      printf("Enter f(%lg): ", x[i]);
      scanf("%lf", &F[i]);
      printf("Enter f'(%lg): ", x[i]);
      scanf("%lf", &FPRIME[i]);
      printf("\n");
    } else {
      F[i] = f(x[i]);		/* Use functions f() and fprime(). */
      FPRIME[i] = fprime(x[i]);
    }
  }

  printf2("H(x) =  Q(0,0) + Q(1,1)*(x-x0) + Q(2,2)*(x-x0)^2 + ");
  printf2("Q(3,3)*(x-x0)^2*(x-x1)\n");
  printf2("\t+ Q(4,4)*(x-x0)^2*(x-x1)^2 + ...\n");
  printf2("\t+ Q(2n+1,2n+1)*(x-x0)^2*(x-x1)^2 * * * ");
  printf2("(x-x(n-1))^2*(x-xn).\n\n");

  if (ch != 'n') {
    printf2("\n%s\n", eq_text_f);
    printf2("%s\n", eq_text_fprime);
  }

  printf2("\n i\t   x\t  f(x)\t\t  f'(x)\n");	/* Print table header. */
  printf2("-----------------------------------------------\n");
  for (i=0;i<=n;i++)
    printf2("% d\t % .2lg\t% .9lf\t% .9lf\n", i, x[i], F[i], FPRIME[i]);
  printf2("\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  for (i=0;i<=n;i++) {		/* Do Steps 2-3. */

    /* STEP #2*/
    z[2*i]      = x[i];
    z[2*i+1]    = x[i];
    Q[2*i][0]   = F[i];
    Q[2*i+1][0] = F[i];
    Q[2*i+1][1] = FPRIME[i];

    /* STEP #3*/
    if (i != 0)
      Q[2*i][1] = (Q[2*i][0] - Q[2*i-1][0]) / (z[2*i] - z[2*i-1]);
  }

  /* STEP #4*/
  for (i=2;i<=2*n+1;i++)
    for (j=2;j<=i;j++)
      Q[i][j] = (Q[i][j-1] - Q[i-1][j-1]) / (z[i] - z[i-j]);

  /* STEP #5*/
  for (i=0;i<=2*n+1;i++) {
    printf2("\tQ[%d][%d] = % .11lf\n", i, i, Q[i][i]);
  }

  /* This code segment evaluates the Hermite polynomial at the point X. */
  for (i=0;i<=n;i++) {		/* Evaluate terms once for speed. */
    diff[i]  = X - x[i];
    diff2[i] = pow(diff[i], 2.0);
  }

  H = Q[0][0];
  for (i=n;i>=0;i--) {		/* Determine H(x).     */
    tmp = Q[2*i+1][2*i+1];
    for (j=0;j<i;j++)
      tmp *= diff2[j];		/* Second power terms. */
    tmp *= diff[i];		/* First power term.   */
    H += tmp;			/* Sum of products.    */

    tmp = Q[2*i+2][2*i+2];
    for (j=0;j<=i;j++)
      tmp *= diff2[j];		/* Second power terms. */
    H += tmp;			/* Sum of products.    */
  }

  /***********
   * OUTPUTS *
   ***********/

  printf2("\nf(%lg) ÷ H(%lg) = %.11lg\n", X, X, H);	/* Print solution. */

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(diff2,0,n);
  free_dvector(diff,0,n);
  free_dvector(FPRIME,0,n);
  free_dvector(F,0,n);
  free_dvector(z,0,(2*n+1));
  free_dvector(x,0,n);
  free_dmatrix(Q,0,(2*n+1),0,(2*n+1));

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
