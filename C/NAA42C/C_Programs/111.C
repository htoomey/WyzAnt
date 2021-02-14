/******************************************************************************
		      Linear Shooting - Algorithm 11.1
*******************************************************************************

To approximate the solution of the linear boundry-value problem

	-y'' + p(x)y' + q(x)y + r(x) = 0,  a ó x ó b,  y(a) = à,  y(b) = á:

(Note: Equations (11.5) and (11.6) are written as first-order systems and
       solved.)

INPUT endpoints a, b; boundry conditions à, á; number of subintervals N or step
size h; the functions p(), q(), and r():

OUTPUT approximations w(1,i) to y(x(i)); w(2,i) to y'(x(i)) for each i =
0,1,...,N

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "111.out";	/* Customized default output file name.     */
char *eq_text_p = "p(x) = -2.0/x";		/* Needs updating  $  */
char *eq_text_q = "q(x) = 2.0/(x*x)";		/* Needs updating  $  */
char *eq_text_r = "r(x) = sin(log(x))/(x*x)";	/* Needs updating  $  */


/*****************************************************************************/
/* p(x) - Function p(x) to evaluate.  Needs updating $.                      */
/*****************************************************************************/
double p(x)
double x;
{
  return (-2.0/x);
}

/*****************************************************************************/
/* q(x) - Function q(x) to evaluate.  Needs updating $.                      */
/*****************************************************************************/
double q(x)
double x;
{
  return (2.0/(x*x));
}

/*****************************************************************************/
/* r(x) - Function r(x) to evaluate.  Needs updating $.                      */
/*****************************************************************************/
double r(x)
double x;
{
  return (sin(log(x))/(x*x));
}
/*****************************************************************************/


main()
{
  double **k, **K, **u, **v, p(), q(), r();
  double a, b, alpha, beta, h, x, w10, w20, W1, W2;
  int i, N, ch;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Linear Shooting - Algorithm 11.1\n\n");
  printf2("y''  = p(x)y' + q(x)y + r(x)\n");
  printf2("%s\n", eq_text_p);
  printf2("%s\n", eq_text_q);
  printf2("%s\n\n", eq_text_r);

  printf("Enter endpoint a: ");
  scanf("%lf", &a);
  printf("Enter endpoint b: ");
  scanf("%lf", &b);
  fprintf(file_id, "Solving for points on interval [%lg,%lg].\n", a, b);

  printf("Enter boundry condition y(%lg) = à: ", a);
  scanf("%lf", &alpha);
  fprintf(file_id, "Boundry condition y(%lg) = à = %lg\n", a, alpha);

  printf("Enter boundry condition y(%lg) = á: ", b);
  scanf("%lf", &beta);
  fprintf(file_id, "Boundry condition y(%lg) = á = %lg\n", b, beta);

  printf("Do you want to enter N or h? H: ");
  getchar();			/* Cleans up after scanf(). */
  ch = tolower(getchar());
  if (ch == 'n') {
    printf("Enter number of subintervals N: ");
    scanf("%d", &N);
    h = (b - a)/N;		/* STEP #1 */
  } else {			/* default */
    printf("Enter interval number h: ");
    scanf("%lf", &h);
    N = (int) ceil((b - a)/h);
  }

  printf2("Number of subintervals N = %d\n", N);
  printf2("Interval number h = %lg\n", h);

  /*
  **  u[1][] is the particular solution - y1()
  **  u[2][] is the derivative of particular solution
  **  v[1][] is the homogeneous solution - y2()
  **  v[2][] is the derivative of homogeneous solution
  */

  /* Dynamically allocate memory for the needed arrays. */
  u = dmatrix(1,2,0,N);
  v = dmatrix(1,2,0,N);		
  k = dmatrix(1,4,1,4);		/* for y1() using RK4 method */
  K = dmatrix(1,4,1,4);		/* for y2() using RK4 method */

				/* Print table header. */
  printf2("\n x\t u1\t\t v1\t\t w2 ÷ y'(x)\t w1 ÷ y(x)");
  printf2("\n-----------------------------------");
  printf2("-----------------------------------\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  u[1][0] = alpha;		/* y1(a) = à  */
  u[2][0] = 0.0;		/* y1'(a) = 0 */
  v[1][0] = 0.0;		/* y2(a) = 0  */
  v[2][0] = 1.0;		/* y2'(a) = 1 */

  /* STEP #2 */
  for (i=0;i<N;i++) {		/* Do Steps 3-4. */

    /* STEP #3 */
    x = a + i*h;

    /* STEP #4 */
    k[1][1] = h*u[2][i];
    k[1][2] = h*(p(x)*u[2][i] + q(x)*u[1][i] + r(x));
    k[2][1] = h*(u[2][i] + 0.5*k[1][2]);
    k[2][2] = h*(p(x+h/2.0)*(u[2][i] + 0.5*k[1][2])
		 + q(x+h/2.0)*(u[1][i] + 0.5*k[1][1]) + r(x+h/2.0));
    k[3][1] = h*(u[2][i] + 0.5*k[2][2]);
    k[3][2] = h*(p(x+h/2.0)*(u[2][i] + 0.5*k[2][2])
		 + q(x+h/2.0)*(u[1][i] + 0.5*k[2][1]) + r(x+h/2.0));
    k[4][1] = h*(u[2][i] + k[3][2]);
    k[4][2] = h*(p(x+h)*(u[2][i]+k[3][2]) + q(x+h)*(u[1][i]+k[3][1]) + r(x+h));
    u[1][i+1] = u[1][i] + (k[1][1] + 2.0*k[2][1] + 2.0*k[3][1] + k[4][1])/6.0;
    u[2][i+1] = u[2][i] + (k[1][2] + 2.0*k[2][2] + 2.0*k[3][2] + k[4][2])/6.0;

    K[1][1] = h*v[2][i];
    K[1][2] = h*(p(x)*v[2][i] + q(x)*v[1][i]);
    K[2][1] = h*(v[2][i] + 0.5*K[1][2]);
    K[2][2] = h*(p(x+h/2.0)*(v[2][i] + 0.5*K[1][2])
		 + q(x+h/2.0)*(v[1][i] + 0.5*K[1][1]));
    K[3][1] = h*(v[2][i] + 0.5*K[2][2]);
    K[3][2] = h*(p(x+h/2.0)*(v[2][i] + 0.5*K[2][2])
		 + q(x+h/2.0)*(v[1][i] + 0.5*K[2][1]));
    K[4][1] = h*(v[2][i] + K[3][2]);
    K[4][2] = h*(p(x+h)*(v[2][i]+K[3][2]) + q(x+h)*(v[1][i]+K[3][1]));
    v[1][i+1] = v[1][i] + (K[1][1] + 2.0*K[2][1] + 2.0*K[3][1] + K[4][1])/6.0;
    v[2][i+1] = v[2][i] + (K[1][2] + 2.0*K[2][2] + 2.0*K[3][2] + K[4][2])/6.0;
  }

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #5 */
  w10 = alpha;
  w20 = (beta - u[1][N]) / v[1][N];
  printf2("% .2lf\t% .9lf\t% .9lf\t% .9lf\t% .9lf\n",
		 a, u[1][0], v[1][0], w20, w10);

  /* STEP #6 */
  for (i=1;i<=N;i++) {
    W1 = u[1][i] + w20 * v[1][i];
    W2 = u[2][i] + w20 * v[2][i];
    x  = a + i*h;
    printf2("% .2lf\t% .9lf\t% .9lf\t% .9lf\t% .9lf\n",
		 x, u[1][i], v[1][i], W2, W1);
  }

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dmatrix(K,1,4,1,4);
  free_dmatrix(k,1,4,1,4);
  free_dmatrix(v,1,2,0,N);
  free_dmatrix(u,1,2,0,N);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}  /* STEP #7 */		/* STOP - Process is complete. */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
