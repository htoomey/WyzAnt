/******************************************************************************
		    Adaptive Quadrature - Algorithm 4.2
*******************************************************************************

                                ôb
To approximate the integral I = ³  f(x) dx to within a given tolerance TOL > 0:
                                õa

INPUT endpoints a, b; tolerance TOL; limit N to number of levels; the function
f().

OUTPUT approximation APP to I or message that N is exceeded.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "042.out";	/* Customized default output file name.     */
char *eq_text_f = "f(x) = (100.0/(x*x))*sin(10.0/x)"; /* Needs updating  $  */

#define N	8		/* Limit to number of levels.               */


/*****************************************************************************/
/* f(x) - Function to evaluate, f(x).  Needs updating $.                     */
/*****************************************************************************/
double f(x)
double x;
{
  if (eqeval)
    return (eval_eq(x));		/* Use the Equation Evaluator  */
  else
    return ((100.0/(x*x))*sin(10.0/x));	/* Use the default function.   */
}
/*****************************************************************************/


main()
{
  double a, b, APP, FD, FE, S1, S2, f();
  double TOL[N+1], A[N+1], H[N+1], FA[N+1], FB[N+1];
  double FC[N+1], L[N+1], S[N+1], v[9];
  int i, feval;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Adaptive Quadrature - Algorithm 4.2\n\n");

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
    printf("Enter tolerance TOL (1.0e-4)?: ");
    scanf("%lf", &TOL[0]);
    if (TOL[0] <= 0.0)		/* Checks for invalid tolerance value. */
      printf("ERROR - Tolerance TOL must be positive.\n");
  } while (TOL[0] <= 0.0);
  printf2("Limit N to number of levels = %d\n", N);
  printf("\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  APP    = 0.0;
  i      = 1;
  TOL[i] = 10.0*TOL[0];
  A[i]   = a;
  H[i]   = (b - a)/2.0;
  FA[i]  = f(a);
  FC[i]  = f(a + H[i]);
  FB[i]  = f(b);
  feval  = 3;

  /* Approximation from Simpson's method for entire interval. */
  S[i] = H[i]*(FA[i] + 4.0*FC[i] + FB[i]) / 3.0;
  L[i] = 1.0;

  /* STEP #2 */
  while (i > 0) {		/* Do Steps 3-5. */

    /* STEP #3 */
    FD     = f(A[i] + H[i]/2.0);
    FE     = f(A[i] + 3.0*H[i]/2.0);
    feval += 2;			/* Counts number of function evaluations. */

    /* Approximations from Simpson's method for halves of subintervals.   */
    S1   = H[i]*(FA[i] + 4.0*FD + FC[i]) / 6.0;
    S2   = H[i]*(FC[i] + 4.0*FE + FB[i]) / 6.0;
    v[1] = A[i];		/* Save data at this level.         */
    v[2] = FA[i];
    v[3] = FC[i];
    v[4] = FB[i];
    v[5] = H[i];
    v[6] = TOL[i];
    v[7] = S[i];
    v[8] = L[i];

    /* STEP #4 */
    i--;			/* Delete the level.  i = i - 1.    */

    /* STEP #5 */
    if (fabs(S1+S2-v[7]) < v[6])
      APP += (S1 + S2);
    else if (v[8] >= N) {	/* Procedure fails.                 */
      printf2("\nMAX LEVEL EXCEEDED (%d MAX.).\n", N);
      NAA_do_last(outfile);	/* NAA finish-up procedure.         */
      naaerror("MAX. LEVEL EXCEEDED.");
    } else {			/* Add one level.                   */
      i++;	/* i = i + 1. *//* Data for right half subinterval. */
      A[i]   = v[1] + v[5];
      FA[i]  = v[3];
      FC[i]  = FE;
      FB[i]  = v[4];
      H[i]   = v[5]/2.0;
      TOL[i] = v[6]/2.0;
      S[i]   = S2;
      L[i]   = v[8] + 1.0;

      i++;	/* i = i + 1. *//* Data for left half subinterval. */
      A[i]   = v[1];
      FA[i]  = v[2];
      FC[i]  = FD;
      FB[i]  = v[3];
      H[i]   = H[i-1];
      TOL[i] = TOL[i-1];
      S[i]   = S1;
      L[i]   = L[i-1];
    }
  }

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #6 */
  fprintf(file_id, "Tolerance = %lg\n\n", TOL[0]);
  printf2("      ô%lg\n", b);	/* APP approximates I to within TOL. */
  printf2("APP = ³  f(x) dx = %.11lg\n", APP);
  printf2("      õ%lg\n\n", a);
  printf2("Required %d functional evaluations.\n", feval);

  NAA_do_last(outfile);		/* NAA finish-up procedure.          */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
