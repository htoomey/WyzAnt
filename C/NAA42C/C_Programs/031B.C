/******************************************************************************
	  Neville's Iterated Interpolation (with rounding) - Algorithm 3.1B
*******************************************************************************

To evaluate the interplating polynomial P on the (n + 1) distinct numbers
x0,..., xn at the number x for the function f():

INPUT numbers x0, x1,..., xn; values f(x0), f(x1),.., f(xn) as the first column
Q(0,0), Q(1,0),..., Q(n,0) of Q; the number of desired significant digits SIG.

OUTPUT the table Q with P(x) = Q(n,n).

NOTE: This algorithm was included as a "Homework Helper."  See p. 105,
Exercise Set 3.3, Problem 7.  For chopping arithmetic, replace EVERY
occurance of the word "round" with "trunc", including the "round.c" file.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */
#include "round.c"		/* For SIG-digit rounding arithmetic.       */

char *outfile   = "031b.out";	/* Customized default output file name.     */
char *eq_text_f = "f(x) = 3^x";	/* Needs updating  $  */


/*****************************************************************************/
/* f(x) - Function to evaluate, f(x).  May be unnecessary if using tabulated */
/*        data.  Needs updating $.                                           */
/*****************************************************************************/
double f(x)
double x;
{
  if (eqeval)
    return (eval_eq(x));	/* Use the Equation Evaluator  */
  else
    return (pow(3.0, x));	/* Use the default function.   */
}
/*****************************************************************************/


main()
{
  double **Q, *x, X, f();
  int i, j, n, ch, SIG;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Neville's Iterated Interpolation (with rounding)");
  printf2(" - Algorithm 3.1B\n\n");

  do {
    printf("Enter the number of significant digits, SIG (1-%d): ", DBL_DIG);
    scanf("%d", &SIG);
    if (SIG < 1 || SIG > DBL_DIG)	/* ERROR */
      printf("Enter 1 to %d only for number of significant digits.\n",
        DBL_DIG);
  } while (SIG < 1 || SIG > DBL_DIG);
  fprintf(file_id, "Computed with %d-digit rounding arithmetic.\n", SIG);

  do {
    printf("Enter number of data points, n+1: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greater than zero.\n");
  } while (n <= 0);

  n--;				/* n = n - 1. */
  fprintf(file_id, "Interpolating polynomial is of degree %d.\n", n);

  /* Dynamically allocate memory for the needed arrays. */
  Q = dmatrix(0,n,0,n);
  x = dvector(0,n);

  printf("Enter the x to evaluate: ");
  scanf("%lf", &X);

  printf("Does f(x) need to be evaluated? (Y/N) Y: ");
  getchar();			/* Cleans up after scanf(). */
  ch = tolower(getchar());

  for (i=0;i<=n;i++) {		/* Enter data.      */
    printf("Enter x%d: ", i);
    scanf("%lf", &x[i]);
    if (ch == 'n') {		/* Use data points. */
      printf("Enter f(%lg): ", x[i]);
      scanf("%lf", &Q[i][0]);
      printf("\n");
    } else
      Q[i][0] = f(x[i]);	/* Use function.    */
    Q[i][0] = round(Q[i][0], SIG);	/* round all values. */
  }

  fprintf(file_id, "Evaluating P(x) at x = %lg\n", X);
  if (ch != 'n') {
    if (eqeval)
      printf2("f(x) = %s", tmpstr);	/* Print the Equation Evaluator text */
    else
      printf2("%s", eq_text_f);		/* Print the default equation text */
    printf2("\n");
  }

  printf2("\n i\t  x(i) ");    	/* Print table header. */
  for (i=0;i<=n;i++)
    if (SIG <= 4)  printf2("  Q(i,%d)", i);
    else           printf2("\t  Q(i,%d)", i);
  printf2("\n--------------------");
  for (i=0;i<=n;i++)
    if (SIG <= 4)  printf2("-------");
    else           printf2("---------------");
  printf2("\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  for (i=1;i<=n;i++)
    for (j=1;j<=i;j++) {
      Q[i][j] = (round(X-x[i-j], SIG) * Q[i][j-1]
		- round(X-x[i], SIG) * Q[i-1][j-1])
		/ round(x[i] - x[i-j], SIG);
      Q[i][j] = round(Q[i][j], SIG);	/* round all values. */
    }

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #2*/
  for (i=0;i<=n;i++) {
    printf2("% d\t % .2lf\t", i, x[i]);
    for (j=0;j<=i;j++)
      printf2("% .*lg\t", SIG, Q[i][j]);
    printf2("\n");
  }

  printf2("\nP(%lg) = %.*lg\n", X, SIG, Q[n][n]);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(x,0,n);
  free_dmatrix(Q,0,n,0,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
