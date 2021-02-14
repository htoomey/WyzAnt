/******************************************************************************
	Midpoint, Modified Euler, and Heun's Methods - Algorithm 5.1B
*******************************************************************************

To approximate the solution of the initial-value problem

	y' = f(t,y),  a ó t ó b,  y(a) = à,

at (N + 1) equally spaced numbers in the interval [a,b].  See Equations 5.35,
5.37, and 5.38:

INPUT endpoints a, b; integer N or interval h; initial condition à; the
function f(); which method(s) to try.

OUTPUT approximation w to y at the (N + 1) values of t.

NOTE: This algorithm was included as a "Homework Helper."  See p. 247,
Exercise Set 5.4, Problems 1-2, 4, 5 a), 6 a), 7-8, 10, 13.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "051b.out";	/* Customized default output file name.     */
char *eq_text_f = "y' = f(t,y) = -y + t*t + 1";	/* Needs updating  $  */


/*****************************************************************************/
/* f(t,y) - Function to evaluate, y' = f(t,y).  Needs updating $.            */
/*****************************************************************************/
double f(t,y)
double t, y;
{
  if (eqeval)
    return (eval_eq(0.0,y,t));	/* Use the Equation Evaluator  */
  else
    return (-y + t*t + 1);	/* Use the default function.   */
}
/*****************************************************************************/

double h, f();


main()
{
  double Midpoint(), Modified_Euler(), Heuns();
  double t, w, w1, w2, w3, a, b, alpha;
  int i, ch, type, N;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Midpoint, Modified Euler, and Heun's Methods - Algorithm 5.1B\n\n");

  if (eqeval)
    printf2("y' = f(t,y) = %s", tmpstr);/* Print the Equation Evaluator text */
  else
    printf2("%s", eq_text_f);		/* Print the default equation text */
  printf2("\n\n");

  printf("Enter endpoint a: ");
  scanf("%lf", &a);
  printf("Enter endpoint b: ");
  scanf("%lf", &b);
  fprintf(file_id, "Solving for points on interval [%lg,%lg].\n", a, b);

  printf("Enter initial condition y(%lg) = à: ", a);
  scanf("%lf", &alpha);
  fprintf(file_id, "y(%lg) = à = %lg\n", a, alpha);

  printf("Do you want to enter N or h? H: ");
  getchar();			/* Cleans up after scanf(). */
  ch = tolower(getchar());
  if (ch == 'n') {
    printf("Enter number of intervals N: ");
    scanf("%d", &N);
    h = (b - a)/N;		/* STEP 1 */
  } else {			/* default */
    printf("Enter interval number h: ");
    scanf("%lf", &h);
    N = (int) ceil((b - a)/h);
  }
  printf2("Number of intervals N = %d\n", N);
  printf2("Interval number h = %lg\n", h);

  do {

    do {
      printf("\nSelect desired method:\n");
      printf("    (1)   Midpoint Method\n");
      printf("    (2)   Modified Euler Method\n");
      printf("    (3)   Heun's Method\n");
      printf("    (4)   All of the above (1-3).\n");
      printf("----> ");
      scanf("%d", &type);
      if ((type < 1) || (type > 4))	/* Do range checking. */
        printf("ERROR - Invalid selection.  Select 1-4 only.\n\n");
    } while ((type < 1) || (type > 4));

    switch (type) {		/* Find solution using Method "type". */
      case 1: printf2("\nUsing Midpoint Method:\n");
              printf2("\n t\t  w ÷ y\n-----------------------\n");
              break;

      case 2: printf2("\nUsing Modified Euler Method:\n");
              printf2("\n t\t  w ÷ y\n-----------------------\n");
              break;

      case 3: printf2("\nUsing Heun's Method:\n");
              printf2("\n t\t  w ÷ y\n-----------------------\n");
              break;

      case 4: printf2("\n t\t  Midpoint\t  Modified Euler  Heun's\n");
              printf2("----------------------------");
              printf2("---------------------------\n");
              break;
    }

  /*************
   * ALGORITHM *
   *************/

    /* STEP #1 */
    t = a;			/* Set initial time value.     */

    /* STEP #2 */
    if (type != 4) {
      w = alpha;		/* Set initial fuction value.  */
      printf2("% .2lf\t% .11lf\n", t, w);

      /* STEP #3 */
      for (i=1;i<=N;i++) {
        switch (type) {		/* compute w(i)                */
          case 1:  w = Midpoint(t,w);        break;
          case 2:  w = Modified_Euler(t,w);  break;
          case 3:  w = Heuns(t,w);           break;
        }
        t += h;			/* compute t(i)                */
        printf2("% .2lf\t% .11lf\n", t, w);
      }
    } else {			/* Type == 4                   */
      w1 = alpha;		/* Set initial fuction values. */
      w2 = alpha;
      w3 = alpha;
      printf2("% .2lf\t% .9lf\t% .9lf\t% .9lf\n", t, w1, w2, w3);

      /* STEP #3 */
      for (i=1;i<=N;i++) {	/* compute w(i)                */
        w1 = Midpoint(t,w1);
        w2 = Modified_Euler(t,w2);
        w3 = Heuns(t,w3);
        t += h;			/* compute t(i)                */
        printf2("% .2lf\t% .9lf\t% .9lf\t% .9lf\n", t, w1, w2, w3);
      }
      NAA_do_last(outfile);	/* NAA finish-up procedure.    */
      exit (1);
    }

    printf("\nWould you like to try another method? (Y/N) N: ");
    getchar();			/* Cleans up after scanf(). */
    ch = tolower(getchar());

  } while (ch == 'y');

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* Midpoint() - Midpoint method for solving an initial-value differential    */
/*              equation.  See Equation 5.35.                                */
/*****************************************************************************/
double Midpoint(t,w)
double t, w;
{
  return (w + h*f(t + (h/2), w + (h/2)*f(t,w)));
}

/*****************************************************************************/
/* Modified_Euler() - Modified Euler method (RK2) for solving an initial-    */
/*                    value differential equations.  See Equation 5.37.      */
/*****************************************************************************/
double Modified_Euler(t,w)
double t, w;
{
  double F;

  F = f(t,w);			/* Saves a function look-up. */
  return (w + (h/2)*(F + f(t+h, w + h*F)));
}

/*****************************************************************************/
/* Heuns() - Heun's method (RK2) for solving an initial-value differential   */
/*           equation.  See Equation 5.38.                                   */
/*****************************************************************************/
double Heuns(t,w)
double t, w;
{
  double F;

  F = f(t,w);			/* Saves a function look-up. */
  return (w + (h/4)*(F + 3*f(t + (2*h/3), w + (2*h/3)*F)) );
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
