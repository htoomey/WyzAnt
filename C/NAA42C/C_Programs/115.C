/******************************************************************************
		Piecewise Linear Rayleigh-Ritz - Algorithm 11.5
*******************************************************************************

To approximate the solution of the boundry-value problem

	-d/dx (p(x)*dy/dx) + q(x)*y = f(x),  0 ó x ó 1,  y(0) = y(1) = 0.

with the piecewise linear function

	è(x) = äc(i)*è(i)(x)  for i = 1 to n.

INPUT integer n; points x0 = 0 < x(1) < ... < x(n) < x(n+1) = 1; the functions
p(x), q(x), and f(x).

OUTPUT coefficients c1,...,cn.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "115.out";	/* Customized default output file name.     */
char *eq_text_p = "p(x) = 1.0";			/* Needs updating  $  */
char *eq_text_q = "q(x) = PI*PI";		/* Needs updating  $  */
char *eq_text_f = "f(x) = 2.0*PI*PI*sin(PI*x)";	/* Needs updating  $  */


/*****************************************************************************/
/* p(x) - Function p(x) to evaluate.  Needs updating $.                      */
/*****************************************************************************/
double p(x)
double x;
{
  return (1.0);
}

/*****************************************************************************/
/* q(x) - Function q(x) to evaluate.  Needs updating $.                      */
/*****************************************************************************/
double q(x)
double x;
{
  return (PI*PI);
}

/*****************************************************************************/
/* f(x) - Function to evaluate, f(x).  Needs updating $.                     */
/*****************************************************************************/
double f(x)
double x;
{
  return (2.0*PI*PI*sin(PI*x));
}
/*****************************************************************************/


main()
{
  double *a, *b, *c, *h, *alpha, *beta, *x, *zeta, *z;
  double I3, I4, I5, I6, IOLD1, IOLD2, INEW1, INEW2, temp;
  double p(), q(), f(), simpson();
  int i, n, ch;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Piecewise Linear Rayleigh-Ritz - Algorithm 11.5\n\n");
  printf2("-d/dx (p(x)*dy/dx) + q(x)*y = f(x),  0 ó x ó 1,");
  printf2("  y(0) = y(1) = 0\n\n");
  printf2("%s\n", eq_text_p);
  printf2("%s\n", eq_text_q);
  printf2("%s\n\n", eq_text_f);

  /* Produce h(i) and x(i). */
  printf("\nHow will you enter the data?\n");
  printf("1 - n with irregular data intervals\n");
  printf("2 - n with regular data intervals\n");
  printf("3 - h with regular data intervals\n");
  printf("----> ");
  ch = getchar();
  switch (ch) {
    case '1':
        printf("Enter number of mesh points, n, between 0 and 1: ");
        scanf("%d", &n);
        fprintf(file_id, "There are %d mesh points between 0 and 1.\n", n);

        /* Dynamically allocate memory for the needed arrays. */
        x = dvector(0,n+1);
        h = dvector(0,n);

        x[0] = 0.0;
        printf("x(0) = 0\n");
        for (i=1;i<=n;i++) {		/* Read in x(i). */
          printf("x(%d) = ", i);
          scanf("%lf", &x[i]);
        }
        x[n+1] = 1.0;
        printf("x(%d) = 1\n", n+1);

        /*************
         * ALGORITHM *
         *************/

        /* STEP #1 */
        h[0] = x[1];			/* First h is x1 - 0.0 */
        for (i=1;i<n;i++)
          h[i] = x[i+1] - x[i];		/* Compute h(i). */
        h[n] = 1.0 - x[n];		/* Last h is 1.0 - xn */
        break;

    case '2':
        printf("Enter number of mesh points, n, between 0 and 1: ");
        scanf("%d", &n);
        fprintf(file_id, "There are %d mesh points between 0 and 1.\n", n);

        /* Dynamically allocate memory for the needed arrays. */
        x = dvector(0,n+1);
        h = dvector(0,n+1);

        /*************
         * ALGORITHM *
         *************/

	/* STEP #1 */
        for (i=0;i<=n;i++)
          h[i] = 1.0 / (n+1);		/* Compute h(i). */

        x[0] = 0.0;			/* Compute x(i). */
        for (i=1;i<=n;i++)
          x[i] = i*(1.0 / (n+1));
        x[n+1] = 1.0;
        break;

    default:			/* case '3' */
        printf("Enter interval number h: ");
        scanf("%lf", &temp);
        n = (int) (ceil(1.0/temp)) - 1;
        fprintf(file_id, "There are %d mesh points between 0 and 1.\n", n);

        /* Dynamically allocate memory for the needed arrays. */
        x = dvector(0,n+1);
        h = dvector(0,n+1);

        /*************
         * ALGORITHM *
         *************/

        /* STEP #1 */
        for (i=0;i<=n;i++)
          h[i] = temp;		/* Compute h(i). */

        x[0] = 0.0;		/* Compute x(i). */
        for (i=1;i<=n;i++)
          x[i] = i*(1.0/(double)(n+1));
        x[n+1] = 1.0;
        break;
  }

  /* Dynamically allocate memory for the needed arrays. */
  a     = dvector(1,n);
  b     = dvector(1,n);
  c     = dvector(1,n);
  alpha = dvector(1,n);
  beta  = dvector(1,n);
  zeta  = dvector(1,n);
  z     = dvector(1,n);

  /*************
   * ALGORITHM *
   *************/

  /* STEP #2 */
  printf2("\nFor i = 1,...,n define the piecewise linear basis phi(i)");
  printf2(" by:\n\n");
  printf2("               { 0                     0 ó x ó x(i-1)    }\n");
  printf2("   phi(i)(x) = { (x - x(i-1))/h(i-1)   x(i-1) ó x x(i)   }\n");
  printf2("               { (x(i+1) - x)/h(i)     x(i) ó x ó x(i+1) }\n");
  printf2("               { 0                     x(i+1) ó x ó 1    }\n\n");

  printf("THINKING");		/* For feedback during long computations. */

  /*
  **  Note: The notation in Steps 3-5 are from the third edition of the text.
  **        It performs the same computations as the fourth edition.
  */

  /* STEP #3 */
  /* Note: The integrals in Steps 3-5 can be evaluated using a numerical */
  /*       integration procedure.                                        */
  temp  = 1.0/(h[0]*h[0]);
  IOLD1 = temp * simpson(x[0],x[1],1,0.0,0.0);
  IOLD2 = temp * simpson(x[0],x[1],2,x[0],0.0);

  /* STEP #4 */
  for (i=1;i<n;i++) {
    temp     = 1.0/(h[i]*h[i]);
    INEW1    = temp * simpson(x[i],x[i+1],1,0.0,0.0);
    INEW2    = temp * simpson(x[i],x[i+1],2,x[i],0.0);
    I3       = temp * simpson(x[i],x[i+1],3,x[i+1],0.0);
    I4       = temp * simpson(x[i],x[i+1],4,x[i+1],x[i]);
    I5       = (1.0/h[i-1]) * simpson(x[i-1],x[i],5,x[i-1],0.0);
    I6       = (1.0/h[i]) * simpson(x[i],x[i+1],6,x[i+1],0.0);
    alpha[i] = IOLD1 + INEW1 + IOLD2 + I3;
    beta[i]  = -INEW1 + I4;
    b[i]     = I5 + I6;
    IOLD1    = INEW1;
    IOLD2    = INEW2;
  }

  /* STEP #5 */
  temp     = 1.0/(h[n]*h[n]);
  INEW1    = temp * simpson(x[n],x[n+1],1,0.0,0.0);
  I3       = temp * simpson(x[n],x[n+1],3,x[n+1],0.0);
  I5       = (1.0/h[n-1]) * simpson(x[n-1],x[n],5,x[n-1],0.0);
  I6       = (1.0/h[n]) * simpson(x[n],x[n+1],6,x[n+1],0.0);
  alpha[n] = IOLD1 + INEW1 + IOLD2 + I3;
  b[i]     = I5 + I6;

  /*
  **  Steps 6-12 solve a symmetric tridiagonal linear system using
  **  Algorithm 6.7.
  */

  /* STEP #6 */
  a[1]    = alpha[1];
  zeta[1] = beta[1] / alpha[1];

  /* STEP #7 */
  for (i=2;i<n;i++) {
    a[i]    = alpha[i] - beta[i-1] * zeta[i-1];
    zeta[i] = beta[i] / a[i];
  }

  /* STEP #8 */
  a[n] = alpha[n] - beta[n-1] * zeta[n-1];

  /* STEP #9 */
  z[1] = b[1] / a[1];

  /* STEP #10 */
  for (i=2;i<=n;i++)
    z[i] = (b[i]-beta[i-1] * z[i-1])/a[i];

  /* STEP #11 */
  c[n] = z[n];

  /* STEP #12 */
  for (i=n-1;i>=1;i--)
    c[i] = z[i] - zeta[i] * c[i+1];

  /***********
   * OUTPUTS *
   ***********/

  printf2("\n i\t x(i)\t phi(x(i))\n");
  printf2("--------------------------------\n");
  printf2(" %d\t% .6lg\t% .11lf\n", 0, 0.0, 0.0);
  for (i=1;i<=n;i++)
    printf2(" %d\t% .6lg\t% .11lf\n", i, x[i], c[i]);
  printf2(" %d\t% .6lg\t% .11lf\n", n+1, 1.0, 0.0);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(z,1,n);
  free_dvector(zeta,1,n);
  free_dvector(beta,1,n);
  free_dvector(alpha,1,n);
  free_dvector(c,1,n);
  free_dvector(b,1,n);
  free_dvector(a,1,n);
  free_dvector(h,0,n+1);
  free_dvector(x,0,n+1);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}  /* STEP #13 */		/* STOP - Procedure is complete. */

/*****************************************************************************/
/* simpson() - Simpson's Composite Rule for integrals - Algorithm 4.1.  Has  */
/*             been tailored for this algorithm.                             */
/*****************************************************************************/
double simpson(a,b,type,x1,x2)
double a,b,x1,x2;
int type;
{
  double h, x, XI0, XI1, XI2, F, p(), q(), f();
  int i, n=100;			/* n = subintervals (even) of [a,b] */

  printf(".");			/* Used for feedback.               */

  h = (b-a)/n;
  switch (type) {		/* XI0 = F(a) + F(b) */
    case 1: XI0 = p(a) + p(b);  break;
    case 2: XI0 = (a-x1)*(a-x1)*q(a) + (b-x1)*(b-x1)*q(b);  break;
    case 3: XI0 = (x1-a)*(x1-a)*q(a) + (x1-b)*(x1-b)*q(b);  break;
    case 4: XI0 = (x1-a)*(a-x2)*q(a) + (x1-b)*(b-x2)*q(b);  break;
    case 5: XI0 = (a-x1)*f(a) + (b-x1)*f(b);                break;
    case 6: XI0 = (x1-a)*f(a) + (x1-b)*f(b);                break;
  }
  XI1 = 0.0;			/* Summation of f(x(2i-1)). */
  XI2 = 0.0;			/* Summation of f(x(2i)).   */

  for (i=1;i<n;i++) {
    x = a + i*h;
    switch (type) {
      case 1: F = p(x);  break;
      case 2: F = (x-x1)*(x-x1)*q(x);  break;
      case 3: F = (x1-x)*(x1-x)*q(x);  break;
      case 4: F = (x1-x)*(x-x2)*q(x);  break;
      case 5: F = (x-x1)*f(x);         break;
      case 6: F = (x1-x)*f(x);         break;
    }
    if (i % 2 == 0)		/* For even i.       */
      XI2 += F;			/* XI2 = XI2 + F(X); */
    else			/* For odd i.        */
      XI1 += F;			/* XI1 = XI1 + F(X); */
  }

  return (h*(XI0 + 2.0*XI2 + 4.0*XI1) / 3.0);
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
