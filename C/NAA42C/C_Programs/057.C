/******************************************************************************
	Runge-Kutta for Systems of Differential Equations - Algorithm 5.7
*******************************************************************************

To approximate the solution of the mth-order system of first-order
initial-value problems

	u'(j) = f(j)(t,u1, u2,...,um),  j = 1,2,...,m.
	a ó t ó b,  u(j)(a) = àj,       j = 1,2,...,m.

at (N + 1) equally spaced numbers in the interval [a,b]:

INPUT endpoints a, b; number of equations m; integer N or interval h; initial
conditions à1,...,àm; the function vector f().

OUTPUT approximation w(j) to u(j)(t) at the (N + 1) values of t.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile    = "057.out";	/* Customized default output file name.     */
char *eq_text_f1 = "y'(1) = f(1,t,y) = -4*y[1] + 3*y[2] + 6";
				/* Needs updating  $  */
char *eq_text_f2 = "y'(2) = f(2,t,y) = -2.4*y[1] + 1.6*y[2] + 3.6";
				/* Needs updating  $  */

#define m	2		/* Number of equations - Needs updating  $  */


/*****************************************************************************/
/* f(j,t,y) - Function vector y'(j) = f(j,t,y) to evaluate.  Index j selects */
/*            function number.  Needs updating $.                            */
/*****************************************************************************/
double f(j,t,y)
int j;
double t, *y;
{
  switch (j) {
    case 1: return (-4*y[1] + 3*y[2] + 6);
    case 2: return (-2.4*y[1] + 1.6*y[2] + 3.6);
    default: naaerror("Oops!  You forgot to modify f().");
  }
}
/*****************************************************************************/


main()
{
  double a, b, h, t, *W, *ALPHA, **k, *temp, f();
  int i, j, ch, N;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Runge-Kutta for Systems of Differential Equations");
  printf2(" - Algorithm 5.7\n\n");
  printf2("%s\n", eq_text_f1);
  printf2("%s\n\n", eq_text_f2);

  printf("Enter endpoint a: ");
  scanf("%lf", &a);
  printf("Enter endpoint b: ");
  scanf("%lf", &b);
  fprintf(file_id, "Solving for points on interval [%lg,%lg].\n", a, b);

  /* Dynamically allocate memory for the needed arrays. */
  W     = dvector(1,m);
  ALPHA = dvector(1,m);
  temp  = dvector(1,m);
  k     = dmatrix(1,4,1,m);

  for (i=1;i<=m;i++) {
    printf("Enter initial condition y[%d](%lg) = à%d: ", i, a, i);
    scanf("%lf", &ALPHA[i]);
    fprintf(file_id, "y[%d](%lg) = à%d = %lg\n", i, a, i, ALPHA[i]);
  }

  printf("Do you want to enter N or h? H: ");
  getchar();			/* Cleans up after scanf(). */
  ch = tolower(getchar());
  if (ch == 'n') {
    printf("Enter number of intervals N: ");
    scanf("%d", &N);
    h = (b - a)/N;		/* STEP #1 */
  } else {			/* default */
    printf("Enter interval number h: ");
    scanf("%lf", &h);
    N = (int) ceil((b - a)/h);
  }
  printf2("Number of intervals N = %d\n", N);
  printf2("Interval number h = %lg\n", h);

  printf2("\n t\t");	/* Print table header. */
  for (i=1;i<=m;i++)
    printf2(" W[%d]\t\t", i);
  printf2("\n-------");
  for (i=1;i<=m;i++)
    printf2("----------------");
  printf2("\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  t = a;

  /* STEPS #2 & #3 */
  printf2("% .2lf\t", t);
  for (j=1;j<=m;j++) {
    W[j] = ALPHA[j];
    printf2("% .9lf\t", W[j]);
  }
  printf2("\n");

  /* STEP #4 */
  for (i=1;i<=N;i++) {		/* Do Steps 5-11. */

    /* STEP #5 */
    for (j=1;j<=m;j++)
      temp[j] = W[j];
    for (j=1;j<=m;j++)
      k[1][j] = h*f(j, t, temp);

    /* STEP #6 */
    for (j=1;j<=m;j++)
      temp[j] = W[j] + k[1][j]/2;
    for (j=1;j<=m;j++)
      k[2][j] = h*f(j, t + h/2, temp);

    /* STEP #7 */
    for (j=1;j<=m;j++)
      temp[j] = W[j] + k[2][j]/2;
    for (j=1;j<=m;j++)
      k[3][j] = h*f(j, t + h/2, temp);

    /* STEP #8 */
    for (j=1;j<=m;j++)
      temp[j] = W[j] + k[3][j];
    for (j=1;j<=m;j++)
      k[4][j] = h*f(j, t + h, temp);

    /* STEP #9 */
    for (j=1;j<=m;j++)
      W[j] += (k[1][j] + 2*k[2][j] + 2*k[3][j] + k[4][j])/6;

    /* STEP #10 */
    t = a + i*h;

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #11 */
    printf2("% .2lf\t", t);
    for (j=1;j<=m;j++)
      printf2("% .9lf\t", W[j]);
    printf2("\n");
  }

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dmatrix(k,1,4,1,m);
  free_dvector(temp,1,m);
  free_dvector(ALPHA,1,m);
  free_dvector(W,1,m);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}  /* STEP #12 */		/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
