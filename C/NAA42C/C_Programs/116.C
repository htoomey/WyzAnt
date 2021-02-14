/******************************************************************************
		Cubic Spline Rayleigh-Ritz - Algorithm 11.6
*******************************************************************************

To approximate the solution of the boundry-value problem

	-d/dx (p(x)*dy/dx) + q(x)*y = f(x),  0 ó x ó 1,  y(0) = y(1) = 0.

with the sum of cubic splines

	è(x) = äc(i)*è(i)(x)  for i = 0 to n + 1.

INPUT integer n; the functions p(x), q(x), and f(x).

OUTPUT coefficients c(0),...,c(n + 1).

(Note: Modifying the variable nn in simpson() (making it larger than 10) will
       give more accurate results at the expense of longer computing time.)

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */
#include "gaussj.c"

char *outfile = "116.out";	/* Customized default output file name.     */
char *eq_text_p = "p(x) = 1.0";			/* Needs updating  $  */
char *eq_text_q = "q(x) = PI*PI";		/* Needs updating  $  */
char *eq_text_f = "f(x) = 2.0*PI*PI*sin(PI*x)";	/* Needs updating  $  */
int n;						/* Global variables.  */
double h, *x;


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
/* f(x) - Function f(x) to evaluate.  Needs updating $.                      */
/*****************************************************************************/
double f(x)
double x;
{
  return (2.0*PI*PI*sin(PI*x));
}
/*****************************************************************************/


main()
{
  double **a, **b, **aa, **bb, U, L, p(), q(), f(), phi(), simpson();
  int i, j, ch;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Cubic Spline Rayleigh-Ritz - Algorithm 11.6\n\n");
  printf2("-d/dx (p(x)*dy/dx) + q(x)*y = f(x),  0 ó x ó 1,");
  printf2("  y(0) = y(1) = 0\n\n");
  printf2("%s\n", eq_text_p);
  printf2("%s\n", eq_text_q);
  printf2("%s\n\n", eq_text_f);

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */			/* Produce h and x(i). */
  printf("Do you want to enter N or h? H: ");
  ch = tolower(getchar());
  switch (ch) {
    case 'n':
        do {
          printf("\nEnter number of mesh points, n, between the");
          printf(" interval 0 and 1: ");
          scanf("%d", &n);
          if (n <= 0)
            printf("ERROR - n must be greter than zero.\n");
        } while (n <= 0);
        fprintf(file_id, "There are %d mesh points between 0 and 1.\n", n);

        /* Dynamically allocate memory for the needed arrays. */
        x = dvector(-2,n+3);

	/* STEP #1 */
        h = 1.0 / (n + 1);	/* Compute h(i). */
        printf2("Interval number h = %lg\n\n", h);

        for (i=0;i<=n+1;i++)
          x[i] = i*h;		/* Compute x(i). */
        break;

    default:
        printf("\nEnter interval number h: ");
        scanf("%lf", &h);
        n = (int) (ceil(1.0/h)) - 1;
        if (n <= 0)
          naaerror("n must be greater than zero determined from h.");
        printf2("There are %d mesh points between 0 and 1.\n", n);
        printf2("Interval number h = %lg\n\n", h);

        /* Dynamically allocate memory for the needed arrays. */
        x = dvector(-2,n+3);

        /* STEP #1 */
        for (i=0;i<=n+1;i++)
          x[i] = i*h;		/* Compute x(i). */
        break;
  }

  /* Dynamically allocate memory for the needed arrays. */
  a  = dmatrix(0,n+1,0,n+1);
  aa = dmatrix(1,n+2,1,n+2);
  b  = dmatrix(0,n+1,1,1);
  bb = dmatrix(1,n+2,1,1);

  /*************
   * ALGORITHM *
   *************/

  /* STEP #2 */
  x[-2]  = 0.0;
  x[-1]  = 0.0;
  /* See step #1 above for other x values. */
  x[n+2] = 1.0;
  x[n+3] = 1.0;

  /* STEP #3 */
  printf2("Define the function S by:\n\n");
  printf2("          { 0,                                      ");
  printf2("   t ó -2     }\n");
  printf2("          { ¬[(2-t)^3 - 4(1-t)^3 - 6t^3 + 4(1+t)^3],");
  printf2("  -2 < t ó -1 }\n");
  printf2("   S(t) = { ¬[(2-t)^3 - 4(1-t)^3 - 6t^3,            ");
  printf2("  -1 < t ó 0  }\n");
  printf2("          { ¬[(2-t)^3 - 4(1-t)^3,                   ");
  printf2("   0 < t ó 1  }\n");
  printf2("          { ¬[(2-t)^3,                              ");
  printf2("   1 < t ó  2 }\n");
  printf2("          { 0,                                      ");
  printf2("   2 < t      }\n\n");

  /* STEP #4 */
  printf2("Define the cubic spline basis {è(i)} for i = 0 to n+1 by:\n\n");
  printf2("          { S(x/h) - 4S((x+h)/h),                 i = 0       }\n");
  printf2("          { S((x-x1)/h) - S((x+h)/h),             i = 1       }\n");
  printf2("   è(x) = { S((x-xi)/h),                          2 ó i ó n-1 }\n");
  printf2("          { S((x-xn)/h) - S((x-(n+2)h)/h),        i = n       }\n");
  printf2("          { S((x-x(n+1))/h) - 4S((x-(n+2)h)/h)    i = n+1.    }\n");
  printf2("\n");

  printf("\nTHINKING....");	/* For feedback during computation. */

  /* STEP #5 */

  /*
  **  Note: The integrals in Steps 6-9 can be evaluated using a numerical
  **        integration procedure.
  */

  for (i=0;i<=n+1;i++) {

    printf("%d", i);	/* For THINKING output. */

    /* STEP #6 */
    ch = (i+3 < n+1) ? (i+3) : (n+1);	/* ch = min(i+3,n+1). */
    for (j=i;j<=ch;j++) {
      L       = (x[j-2] > 0.0) ? (x[j-2]) : (0.0);  /* L = max(x[j-2],0). */
      U       = (x[i+2] < 1.0) ? (x[i+2]) : (1.0);  /* U = min(x[i+2],1). */
      a[i][j] = simpson(L,U,1,i,j);
      a[j][i] = a[i][j];	/* Since a is symmetric. */
    }

    /* STEP #7 */
    if (i >= 4)			/* For lower triangle zeros of band matrix. */
      for (j=0;j<=i-4;j++)
        a[i][j] = 0.0;

    /* STEP #8 */
    if (i <= n-3)		/* For upper triangle zeros of band matrix. */
      for (j=i+4;j<=n+1;j++)
        a[i][j] = 0.0;

    /* STEP #9 */
    L       = (x[i-2] > 0.0) ? (x[i-2]) : (0.0);  /* L = max(x[i-2],0). */
    U       = (x[i+2] < 1.0) ? (x[i+2]) : (1.0);  /* U = min(x[i+2],1). */
    b[i][1] = simpson(L,U,2,i,0);
  }

  for (i=0;i<=n+1;i++) {  /* Need A[1..n+2][1..n+2] from A[0..n+1][0..n+1] */
    bb[i+1][1] = b[i][1];
    for (j=0;j<=n+1;j++)
      aa[i+1][j+1] = a[i][j];
  }
  
  printf2("A = ");		/* Print out symmetric band matrix a. */
  for (i=1;i<=n+2;i++) {
    printf2("[ ");
    for (j=1;j<=n+2;j++)
      printf2("% 3lg ", aa[i][j]);
    printf2("]\n    ");
  }
  printf2("\n");

  printf2("B = [ ");		/* Print out matrix B. */
  for (i=1;i<=n+2;i++)
    printf2("% 3lg ", bb[i][1]);
  printf2("]t\n\n");

  /* STEP #10 */
  gaussj(aa,n+2,bb,1);		/* Solves aa*c = bb (Ac = b), returning bb */
				/* as c.                                   */

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #11 */
  for (i=0;i<=n+1;i++)
    bb[i+1][1] /= 2.0;		/* Patch needed for correct output. */

  printf2("\n i\t c(i)\t\t x(i)\t phi(x(i))\n");
  printf2("------------------------------------------------\n");
  for (i=0;i<=n+1;i++) {
    L = 0.0;
    for (j=0;j<=n+1;j++)
      L += bb[j+1][1]*phi(x[i],j);
    printf2(" %d\t% 8.8lG\t% .4lf\t% 8.8lG\n", i, bb[i+1][1], x[i], L);
  }

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dmatrix(bb,1,n+2,1,1);
  free_dmatrix(b,0,n+1,1,1);
  free_dmatrix(aa,1,n+2,1,n+2);
  free_dmatrix(a,0,n+1,0,n+1);
  free_dvector(x,-2,n+3);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}  /* STEP #12 */

/*****************************************************************************/
/* simpson() - Simpson's Composite Rule for integrals - Algorithm 4.1.  Has  */
/*             been taylored for this algorithm.                             */
/*****************************************************************************/
double simpson(a,b,type,i,j)
double a, b;
int type, i, j;
{
  double hh, xx, XI0, XI1, XI2, F, p(), q(), f(), phi(), phiprime();
  int ii, nn=10;		/* nn = subintervals (even) of [a,b] */

  printf(".");			/* For THINKING output.              */

  hh = (b-a)/nn;
  switch (type) {		/* XI0 = F(a) + F(b)                  */
    case 1: XI0 = p(a)*phiprime(a,i)*phiprime(a,j) + q(a)*phi(a,i)*phi(a,j) +
		  p(b)*phiprime(b,i)*phiprime(b,j) + q(b)*phi(b,i)*phi(b,j);
                  break;
    case 2: XI0 = f(a)*phi(a,i) + f(b)*phi(b,i);  break;
  }
  XI1 = 0.0;			/* Summation of f(xx(2i-1)). */
  XI2 = 0.0;			/* Summation of f(xx(2i)).   */

  for (ii=1;ii<nn;ii++) {
    xx = a + ii*hh;
    switch (type) {
      case 1: F = p(xx)*phiprime(xx,i)*phiprime(xx,j) +
		  q(xx)*phi(xx,i)*phi(xx,j);  break;
      case 2: F = f(xx)*phi(xx,i);            break;
    }
    if (ii % 2 == 0)		/* For even i.  */
      XI2 += F;			/* XI2 += F(X); */
    else			/* For odd i.   */
      XI1 += F;			/* XI1 += F(X);	*/
  }

  return (hh*(XI0 + 2.0*XI2 + 4.0*XI1) / 3.0);
}

/*****************************************************************************/
/* S(t) - The function S(t) from STEP #3.                                    */
/*****************************************************************************/
double S(t)
double t;
{
  if (t <= -2.0)
    return (0.0);

  if (t > -2.0 && t <= -1.0)
    return (0.25*(pow(2.0-t,3.0)-4.0*pow(1.0-t,3.0)-6.0*t*t*t+
	  4.0*pow(1.0+t,3.0)));

  if (t > -1.0 && t <= 0.0)
    return (0.25*(pow(2.0-t,3.0)-4.0*pow(1.0-t,3.0)-6.0*t*t*t));

  if (t > 0.0 && t <= 1.0)
    return (0.25*(pow(2.0-t,3.0)-4.0*pow(1.0-t,3.0)));

  if (t > 1.0 && t <= 2.0)
    return (0.25*(pow(2.0-t,3.0)));

  if (t > 2.0)
    return (0.0);

  return (0.0);			/* Default. */
}

/*****************************************************************************/
/* Sprime(t) - The function S(t) from STEP #3.                               */
/*****************************************************************************/
double Sprime(t)
double t;
{
  if (t <= -2.0)
    return (0.0);

  if (t > -2.0 && t <= -1.0)
    return (0.75*(-pow(2.0-t,2.0)+4.0*pow(1.0-t,2.0)-6.0*t*t+
	  4.0*pow(1.0+t,2.0)));

  if (t > -1.0 && t <= 0.0)
    return (0.75*(-pow(2.0-t,2.0)+4.0*pow(1.0-t,2.0)-6.0*t*t));

  if (t > 0.0 && t <= 1.0)
    return (0.75*(-pow(2.0-t,2.0)+4.0*pow(1.0-t,2.0)));

  if (t > 1.0 && t <= 2.0)
    return (0.75*(-pow(2.0-t,2.0)));

  if (t > 2.0)
    return (0.0);

  return (0.0);			/* Default. */
}

/*****************************************************************************/
/* phi(X,i) - The function phi(x) from STEP #4.                              */
/*****************************************************************************/
double phi(X,i)
double X;
int i;
{
  double S();

  if (i == 0)
    return (S(X/h) - 4.0*S((X+h)/h));

  if (i == 1)
    return (S((X-x[1])/h) - S((X+h)/h));

  if (i >= 2 && i <= n-1)
    return (S((X-x[i])/h));

  if (i == n)
    return (S((X-x[n])/h) - S((X-(n+2)*h)/h));

  if (i == n+1)
    return (S((X-x[n+1])/h) - 4.0*S((X-(n+2)*h)/h));

  return (0.0);			/* Default. */
}

/*****************************************************************************/
/* phiprime(X,i) - The function phi'(x) for STEP #6.                         */
/*****************************************************************************/
double phiprime(X,i)
double X;
int i;
{
  double Sprime();

  if (i == 0)
    return (Sprime(X/h) - 4.0*Sprime((X+h)/h));

  if (i == 1)
    return (Sprime((X-x[1])/h) - Sprime((X+h)/h));

  if (i >= 2 && i <= n-1)
    return (Sprime((X-x[i])/h));

  if (i == n)
    return (Sprime((X-x[n])/h) - Sprime((X-(n+2)*h)/h));

  if (i == n+1)
    return (Sprime((X-x[n+1])/h) - 4.0*Sprime((X-(n+2)*h)/h));

  return (0.0);			/* Default. */
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
