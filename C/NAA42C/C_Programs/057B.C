/******************************************************************************
	    Euler's Variable Step-Size for Systems - Algorithm 5.7B
				(For Stiff Equations)
*******************************************************************************

To approximate the solution of the mth-order system of first-order
initial-value problems

	u'(j) = f(j)(t,u1, u2,...,um),  j = 1,2,...,m.
	a ó t ó b,  u(j)(a) = àj,       j = 1,2,...,m.

at (N + 1) equally spaced numbers in the interval [a,b,c]:

INPUT endpoints a, c; midpoint b;  number of equations M; interval numbers h1
and h2 where h1 ó h2;  initial conditions à1,...,àm; the function vector f().

OUTPUT approximation w(j) to u(j)(t) at the (N + 1) values of t.

NOTE: This algorithm was included as a "Homework Helper."  See p. 309,
Exercise Set 5.11, Problems 1 c-f).  For Stiff Equations.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile    = "057b.out";	/* Customized default output file name.     */
char *eq_text_f1 = "y'(1) = f(1,t,y) = 32*y1 + 66*y2 + 2*t/3 + 2/3";
				/* Needs updating  $  */
char *eq_text_f2 = "y'(2) = f(2,t,y) = -66*y1 - 33*y2 - t/3 - 1/3";
				/* Needs updating  $  */

#define M	2		/* Number of equations - Needs updating  $  */


/*****************************************************************************/
/* f(j,t,y) - Function vector y'(j) = f(j,t,y) to evaluate.  Index j selects */
/*            function number.  Needs updating $.                            */
/*****************************************************************************/
double f(j,t,y)
int j;
double t, y[];
{
  switch (j) {
    case 1: return (32*y[1] + 66*y[2] + 2*t/3.0 + 2.0/3.0);
    case 2: return (-66*y[1] - 133*y[2] - t/3.0 - 1.0/3.0);
    default: naaerror("Oops!  You forgot to modify f().");
  }
}
/*****************************************************************************/


main()
{
  double a, b, c, h, h1, h2, t, *W, *ALPHA, f();
  int i, j, N;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Euler's Variable Step-Size for Systems - Algorithm 5.7B\n\n");
  printf2("From p.309, Problem #1. f)\n");
  printf2("%s\n", eq_text_f1);
  printf2("%s\n\n", eq_text_f2);

  printf2("a <= b <= c.  ");
  printf2("Use h1 for a <= t <= b and use h2 for b <= t <= c\n");

  printf("Enter endpoint a: ");
  scanf("%lf", &a);
  printf("Enter midpoint b: ");
  scanf("%lf", &b);
  printf("Enter endpoint c: ");
  scanf("%lf", &c);
  fprintf(file_id, "Solving for points on intervals [%lg,%lg]", a, b);
  fprintf(file_id, " and [%lg,%lg].\n", b, c);

  /* Dynamically allocate memory for the needed arrays. */
  W     = dvector(1,M);
  ALPHA = dvector(1,M);

  for (i=1;i<=M;i++) {
    printf("Enter initial condition y[%d](%lg) = à%d: ",i,a,i);
    scanf("%lf", &ALPHA[i]);
    fprintf(file_id, "y[%d](%lg) = à%d = %lg\n", i, a, i, ALPHA[i]);
  }

  printf("Enter interval number h1: ");
  scanf("%lf", &h1);
  fprintf(file_id, "Interval number h1 = %lg\n", h1);

  printf("Enter interval number h2: ");
  scanf("%lf", &h2);
  fprintf(file_id, "Interval number h2 = %lg\n", h2);

  N = (int) ceil((b - a)/h1) + (int) ceil((c - b)/h2);
  printf2("Number of intervals N = %d\n", N);

  printf2("\n t\t");	/* Print table header. */
  for (i=1;i<=M;i++)
    printf2("  W[%d]\t\t", i);
  printf2("\n-------");
  for (i=1;i<=M;i++)
    printf2("----------------");
  printf2("\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  t = a;

  /* STEPS #2 & #3 */
  printf2("% .2lf\t", t);
  for (j=1;j<=M;j++) {
    W[j] = ALPHA[j];
    printf2("% .9lg\t", W[j]);
  }
  printf2("\n");

  /* STEP #4 */
  h = h1;
  for (i=1;i<=N;i++) {		/* Do Steps 5-11. */

    /* STEP #5 */
    for (j=1;j<=M;j++)
      W[j] = W[j] + h*f(j, t, &W[0]);	/* Euler's method */

    /* STEP #6 */
    if (t < b) {		/* For interval h1. */
      h = h1;
      t = a + i*h;
    } else {			/* For interval h2. */
      h = h2;
      t += h;
    }    

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #7 */
    printf2("% .2lf\t", t);
    for (j=1;j<=M;j++)
      if (W[j] > 1.0e9 || W[j] < -1.0e8)
        printf2("% .7lE\t", W[j]);
      else
        printf2("% .9lG\t", W[j]);
    printf2("\n");
  }

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(ALPHA,1,M);
  free_dvector(W,1,M);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}  /* STEP #8 */		/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
