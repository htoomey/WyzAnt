/******************************************************************************
			Romberg - Algorithm 4.3
*******************************************************************************

                                ôb
To approximate the integral I = ³  f(x) dx, select an integer n > 0.
                                õa

Uses the Trapezoidal Rule followed by Richardson's Extrapolation.

INPUT endpoints a, b; integer n; the function f().

OUTPUT an array R.  (R(n,n) is the approximation to I.  Computed by rows; only
2 rows saved in storage.)

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "043.out";	/* Customized default output file name.     */
char *eq_text_f = "f(x) = sin(x)";	/* Needs updating  $  */
#define N	8		/* Limit to number of levels.               */


/*****************************************************************************/
/* f(x) - Function to evaluate, f(x).  Needs updating $.                     */
/*****************************************************************************/
double f(x)
double x;
{
  if (eqeval)
    return (eval_eq(x));	/* Use the Equation Evaluator  */
  else
    return (sin(x));		/* Use the default function.   */
}
/*****************************************************************************/


main()
{
  double a, b, h, sum, **R, f();
  int i, j, k, n;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Romberg - Algorithm 4.3\n\n");

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
    printf("Enter number of intervals on [a,b], n: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greater than zero.\n");
  } while (n <= 0);
  fprintf(file_id, "n = %d intervals on [a,b].\n", n);

  printf2("\n i\t");		/* Prints table header. */
  for (i=1;i<=n;i++)
    printf2(" R[i][%d]\t", i);
  printf2("\n-------");
  for (i=1;i<=n;i++)
    printf2("----------------");
  printf2("\n");

  /* Dynamically allocate memory for the needed arrays. */
  R = dmatrix(1,2,1,n);		/* Romberg matrix. */

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  h = b - a;
  R[1][1] = h*(f(a) + f(b))/2.0;

  /* STEP #2 */
  printf2(" 1\t% .9lf\n", R[1][1]);

  /* STEP #3 */
  for (i=2;i<=n;i++) {		/* Do Steps 4-8. */

    /* STEP #4 */
    sum = 0.0;			/* Approximation from trapezoidal method. */
    for (k=1;k<=(1<<i-2);k++)
      sum += f(a + ((double) k - 0.5)*h);
    R[2][1] = 0.5*(R[1][1] + h*sum);

    /* STEP #5 */
    for (j=2;j<=i;j++) {		/* Extrapolation.      */
      sum = pow(4.0,(double)(j-1));	/* sum used for speed. */
      R[2][j] = (sum*R[2][j-1] - R[1][j-1]) / (sum - 1.0);
    }

    /* STEP #6 */
    printf2(" %d\t", i);
    for (j=1;j<=i;j++)
      printf2("% .9lf\t", R[2][j]);
    printf2("\n");

    /* STEP #7 */
    h /= 2.0;			/* h = h / 2.         */

    /* STEP #8 */
    for (j=1;j<=i;j++)
      R[1][j] = R[2][j];	/* Update row 1 of R. */
  }

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #6 Cont. */
  printf2("\n");
  printf2("    ô%lg\n", b);	/* R[n][n] approximates I. */
  printf2("I = ³  f(x) dx = %.11lg\n", R[2][n]);
  printf2("    õ%lg\n\n", a);
  printf2("Required %d functional evaluations.\n", (1<<n-1)-1);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dmatrix(R,1,2,1,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}  /* STEP #9 */		/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
