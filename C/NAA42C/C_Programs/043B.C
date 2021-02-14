/******************************************************************************
			Gaussian Quadrature - Algorithm 4.3B
*******************************************************************************

                                ôb
To approximate the integral I = ³  f(x) dx:
                                õa

INPUT endpoints a, b; positive integer n between 1 and 5; the function f().

OUTPUT approximation XI to I.

NOTE: This algorithm was included as a "Homework Helper."  See p. 203,
Exercise Set 4.7, Problems 1-4.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "043b.out";	/* Customized default output file name.     */
char *eq_text_f = "f(x) = exp(-x*x)";	/* Needs updating  $  */


/*****************************************************************************/
/* f(x) - Function to evaluate, f(x).  Needs updating $.                     */
/*****************************************************************************/
double f(x)
double x;
{
  if (eqeval)
    return (eval_eq(x));	/* Use the Equation Evaluator  */
  else
    return (exp(-x*x));		/* Use the default function.   */
}
/*****************************************************************************/

/*
** Note: To evaluate higher order Legendre polynomials, add to the R[] and C[]
**       arrays below.  Also increase #define MAXN used for range checking.
*/

#define MAXN	5		/* Largest degree polynomial to evaluate. */

/* Table of constants C[][] and roots R[][] of some Legendre polynomials. */
double R[MAXN+1][MAXN] = {
  { 0.0000000000,  0.0000000000,  0.0000000000,  0.0000000000,  0.0000000000 },
  { 0.0000000000,  0.0000000000,  0.0000000000,  0.0000000000,  0.0000000000 },
  { 0.5773502692, -0.5773502692,  0.0000000000,  0.0000000000,  0.0000000000 },
  { 0.7745966692,  0.0000000000, -0.7745966692,  0.0000000000,  0.0000000000 },
  { 0.8611363116,  0.3399810436, -0.3399810436, -0.8611363116,  0.0000000000 },
  { 0.9061798459,  0.5384693101,  0.0000000000,	-0.5384693101, -0.9061798459 },
};

double C[MAXN+1][MAXN] = {
  { 0.0000000000,  0.0000000000,  0.0000000000,  0.0000000000,  0.0000000000 },
  { 0.0000000000,  0.0000000000,  0.0000000000,  0.0000000000,  0.0000000000 },
  { 1.0000000000,  1.0000000000,  0.0000000000,  0.0000000000,  0.0000000000 },
  { 0.5555555556,  0.8888888889,  0.5555555556,  0.0000000000,  0.0000000000 },
  { 0.3478548451,  0.6521451549,  0.6521451549,  0.3478548451,  0.0000000000 },
  { 0.2369268850,  0.4786286705,  0.5688888889,	 0.4786286705,  0.2369268850 },
};

double a, b, x(), f();


main()
{
  double I;
  int i, n;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Gaussian Quadrature - Algorithm 4.3B\n\n");

  if (eqeval)
    printf2("f(x) = %s", tmpstr);	/* Print the Equation Evaluator text */
  else
    printf2("%s", eq_text_f);		/* Print the default equation text */
  printf("\n\n");

  printf("Enter endpoint a: ");
  scanf("%lf", &a);
  printf("Enter endpoint b: ");
  scanf("%lf", &b);
  fprintf(file_id, " from %lg to %lg.\n\n", a, b);

  do {
    printf("Enter number of points to evaluate, n (2-%d): ", MAXN);
    scanf("%d", &n);
    if ((n < 2) || (n > MAXN))
      printf("ERROR - n must be between 2 and %d.\n", MAXN);
  } while ((n < 2) || (n > MAXN));
  fprintf(file_id, "Number of points to evaluate, n = %d\n", n);

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  I = 0.0;
  for (i=0;i<n;i++)		/* Compute the integral. */
    I += C[n][i] * f(x(R[n][i]));
  I *= (b - a) / 2.0;

  /***********
   * OUTPUTS *
   ***********/

  printf2("\n");
  printf2("    ô%lg\n", b);
  printf2("I = ³  f(x) dx = %.9lg\n", I);
  printf2("    õ%lg\n\n", a);
  printf2("Required %d functional evaluations.\n", n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* x() - Transforms variables into the interval [-1,1].                      */
/*****************************************************************************/
double x(t)
double t;
{
  return (((b - a)*t + b + a) / 2.0);
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
