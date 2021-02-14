/******************************************************************************
		Taylor Polynomial Approximation - Algorithm 1.1B
*******************************************************************************

To approximate the 4th Taylor Polynomial, P4(x), from the function f(x) about
the point x0.  Note: f(5)(x) must exist and x0 must not be at a discontinuity.

(This algorithm's order of convergence is à = 3.)

INPUT the point x0; evaluation points x; the function f():

OUTPUT approximation to the 4th Taylor Polynomial.

NOTE: This algorithm was included as a "Homework Helper."  See p. 8,
Exercise Set 1.1, Problems 9-10.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "011b.out";	/* Customized default output file name.     */
char *eq_text_f = "f(x) = exp(-x^2)";	/* Needs updating  $  */


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


main()
{
  double x0, x, h=0.001, *U, *Df, f(), tmp, approx;
  int i, ch;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Taylor Polynomial Approximation - Algorithm 1.1B\n\n");

  if (eqeval)
    printf2("f(x) = %s", tmpstr);	/* Print the Equation Evaluator text */
  else
    printf2("%s", eq_text_f);		/* Print the default equation text */
  printf2("\n\n");

  printf("Enter point x0: ");
  scanf("%lf", &x0);
  fprintf(file_id, "Point x0 = %lg\n\n", x0);

  printf2("GENERAL FORM:\n");
  printf2("P4(x) = f(x0)\n\t+ f'(x0)(x-x0)\n\t+ (f''(x0)/2!)(x-x0)^2\n\t");
  printf2("+ (f'''(x0)/3!)(x-x0)^3\n\t+ (f(4)(x0)/4!)(x-x0)^4\n\t");  
  printf2("+ R4(x)\n\n");

  /* Dynamically allocate memory for the needed arrays. */
  U  = dvector(-2,2);		/* Stores values of f(x) to be used.      */
  Df = dvector(0,4);		/* Stores derivatives of f(x) to be used. */

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  for (i= -2;i<=2;i++)		/* Evaluate five function points. */
    U[i] = f(x0 + i*h);

  /* STEP #2 */
  Df[0] = U[0];			/* Compute derivatives.           */
  Df[1] = (-U[2] + 8*U[1] - 8*U[-1] + U[-2]) / (12*h);
  Df[2] = (-U[2] + 16*U[1] - 30*U[0] + 16*U[-1] - U[-2]) / (12*h*h);
  Df[3] = (U[2] - 2*U[1] + 2*U[-1] - U[-2]) / (2*h*h*h);
  Df[4] = (U[2] - 4*U[1] + 6*U[0] - 4*U[-1] + U[-2]) / pow(h,4.0);

  /* STEP #3 */
  Df[2] /= 2.0;			/* Divide by factorials. */
  Df[3] /= 6.0;
  Df[4] /= 24.0;

  /***********
   * OUTPUTS *
   ***********/

  printf2("P4(x) = (%lg)\n\t+ (%lg)(x-%lg)\n\t", Df[0], Df[1], x0);
  printf2("+ (%lg)(x-%lg)^2\n\t+ (%lg)(x-%lg)^3\n\t", Df[2], x0, Df[3], x0);
  printf2("+ (%lg)(x-%lg)^4\n\t+ R4(x)\n\n", Df[4], x0);

  do {

    printf("Evaluate P4(x) at x = ");
    scanf("%lf", &x);

    tmp = x - x0;
    approx = (((((Df[4] * tmp) + Df[3]) * tmp + Df[2])
		* tmp + Df[1]) * tmp + Df[0]);
    printf2("P4(%lg) = %lg\n", x, approx);

    printf("Would you like to evaluate P4(x) at another point? (Y/N) N: ");
    getchar();			/* Cleans up after scanf(). */
    ch = tolower(getchar());

  } while (ch == 'y');

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(Df, 0,4);
  free_dvector(U, -2,2);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
