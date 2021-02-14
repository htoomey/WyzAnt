/******************************************************************************
	Adams-Bashforth (all four) and Milne's Methods - Algorithm 5.4B
*******************************************************************************

To approximate the solution of the initial-value problem

	y' = f(t,y),  a ó t ó b,  y(a) = à,

at (N + 1) equally spaced numbers in the interval [a,b].  See Equations 5.39,
5.63, 5.64, 5.65, 5.66 and 5.70:

INPUT endpoints a, b; integer N or interval h; initial conditions à, à1, à2,
à3, and à4 where applicable; the function f(); which method(s) to try.

OUTPUT approximation w to y at the (N+1) values of t.

NOTE: This algorithm was included as a "Homework Helper."  See p. 268,
Exercise Set 5.6, Problems 2-3.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "054b.out";	/* Customized default output file name.     */
char *eq_text_f = "y' = f(t,y) = -y + t + 1";	/* Needs updating  $  */

double a, b, h, alpha[5], f();
int N;


/*****************************************************************************/
/* f(t,y) - Function to evaluate, y' = f(t,y).  Needs updating $.            */
/*****************************************************************************/
double f(t,y)
double t, y;
{
  if (eqeval)
    return (eval_eq(0.0,y,t));	/* Use the Equation Evaluator  */
  else
    return (-y + t + 1);	/* Use the default function.   */
}
/*****************************************************************************/


main()
{
  int i, ch, type, HAVEID, HAVEALPHA;
  double t, Runge_Kutta_4();

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Adams-Bashforth (all four) and Milne's Methods");
  printf2(" - Algorithm 5.4B\n\n");

  if (eqeval)
    printf2("y' = f(t,y) = %s", tmpstr);/* Print the Equation Evaluator text */
  else
    printf2("%s", eq_text_f);		/* Print the default equation text */
  printf2("\n\n");

  HAVEID    = FALSE;	/* Flag to indicate initial data status. */
  HAVEALPHA = FALSE;	/* Flag to indicate alpha status.        */

  do {

    do {
      printf("Select desired method:\n\n");
      printf("    (1)   Runge-Kutta (Order Four) Method   (5.39)\n\n");

      printf("    (2)   Adams-Bashforth Two-Step Method   (5.63)\n");
      printf("    (3)   Adams-Bashforth Three-Step Method (5.64)\n");
      printf("    (4)   Adams-Bashforth Four-Step Method  (5.65)\n");
      printf("    (5)   Adams-Bashforth Five-Step Method  (5.66)\n\n");

      printf("    (6)   Milne's Method                    (5.70)\n\n");

      printf("    (7)   All Adams-Bashforth Methods above (2-5) with\n");
      printf("          starting values from the Runge-Kutta (Order\n");
      printf("          Four) Method.\n\n");

      printf("    (8)   ALL of the above (1-7).\n");
      printf("----> ");
      scanf("%d", &type);

      if ((type < 1) || (type > 8))
        printf("ERROR - Invalid selection.  Choose 1-8 only.\n");
    } while ((type < 1) || (type > 8));

    if (HAVEID == FALSE) {		/* Get initial data ONCE. */
      HAVEID = TRUE;

      /* Enter endpoint data */
      printf("Enter endpoint a: ");
      scanf("%lf", &a);
      printf("Enter endpoint b: ");
      scanf("%lf", &b);
      fprintf(file_id, "Solving for points on interval [%lg,%lg].\n", a, b);

      printf("Do you want to enter N or h? H: ");
      getchar();		/* Cleans up after scanf(). */
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
    }

    /* Enter initial conditions (à), unless determined by RK4 method. */
    if ((HAVEALPHA == FALSE)
         && (((type != 1) && (type != 7)) || (type == 8))) {
      printf("\nEnter initial conditions ");
      printf("(enter 0 for each à NOT needed): \n");
      fprintf(file_id, "\n");
      for (i=0;i<=4;i++) {
        printf("\ty(%lg) = à%d: ", a+i*h, i);
        scanf("%lf", &alpha[i]);
        fprintf(file_id, "y(%lg) = à%d = % .11lf\n", a+i*h, i, alpha[i]);
      }
      fprintf(file_id, "\n");
      HAVEALPHA = TRUE;
    }
				/* For RK4's à. */
    if ((HAVEALPHA == FALSE) && ((type == 1) || (type >= 7))) {
      printf("\ty(%lg) = à: ", a);
      scanf("%lf", &alpha[0]);
      fprintf(file_id, "y(%lg) = à = % .11lf\n", a, alpha[0]);
    }

    /*************
     * ALGORITHM *
     *************/

    /* STEP #1 */
    if (type == 8) {		/* Try ALL methods */
      for (type=1;type<=6;type++)
        Evaluate(type);
      type = 7;
      printf2("\nUsing all 4 Adams-Bashforth Methods with starting\n");
      printf2("values from the Runge-Kutta (Order Four) Method:\n");
    }

    if (type == 7) {		/* Try all Adams-Bashforth methods */
      for (i=0;i<=3;i++) {	/* Determine à's from RK4 Method.  */
        t = a + i*h;
        alpha[i+1] = Runge_Kutta_4(t,alpha[i]);
      }
      for (type=2;type<=5;type++)
        Evaluate(type);
      HAVEID    = FALSE;	/* Reset flags.                    */
      HAVEALPHA = FALSE;
    } else			/* Try just one method at a time.  */
      Evaluate(type);

    printf("Would you like to try another method? (Y/N) N: ");
    getchar();			/* Cleans up after scanf(). */
    ch = tolower(getchar());

  } while (ch == 'y');

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}

/*****************************************************************************/
/* Evaluate() - Evaluates a specific derivative formula.                     */
/*****************************************************************************/
Evaluate(type)
int type;
{
  double Runge_Kutta_4(), Adams_Bashforth_2(), Adams_Bashforth_3();
  double Adams_Bashforth_4(), Adams_Bashforth_5(), Milne();
  double t, w[5], W;
  int i, j, start;

  switch (type) {
    case 1: printf2("\nUsing Runge-Kutta (Order Four) Method:\n");    break;
    case 2: printf2("\nUsing Adams-Bashforth Two-Step Method:\n");    break;
    case 3: printf2("\nUsing Adams-Bashforth Three-Step Method:\n");  break;
    case 4: printf2("\nUsing Adams-Bashforth Four-Step Method:\n");   break;
    case 5: printf2("\nUsing Adams-Bashforth Five-Step Method:\n");   break;
    case 6: printf2("\nUsing Milne's Method:\n");                     break;
  }
  start = (type == 6) ? 4 : type;

				/* Print table header. */
  printf2("\n t\t  w ÷ y\n-----------------------\n");
  t = a;

  for (i=0;i<start;i++) {	/* Print given initial values. */
    t    = a + i*h;
    w[i] = alpha[i];
    printf2("% .2lf\t% .11lf\n", t, w[i]);
  }

  for (i=start;i<=N;i++) {
    switch (type) {		/* compute wi */
      case 1:  W = Runge_Kutta_4(t,w[0]);           break;
      case 2:  W = Adams_Bashforth_2(t,w,start-1);  break;
      case 3:  W = Adams_Bashforth_3(t,w,start-1);  break;
      case 4:  W = Adams_Bashforth_4(t,w,start-1);  break;
      case 5:  W = Adams_Bashforth_5(t,w,start-1);  break;
      case 6:  W = Milne(t,w,start-1);              break;
    }

    t = a + i*h;		/* compute ti */

    if (type != 1)	/* Prepare w[] for next run. */
      for (j=0;j<start-1;j++)
        w[j] = w[j+1];
    w[start-1] = W;

    /***********
     * OUTPUTS *
     ***********/

    printf2("% .2lf\t% .11lf\n", t, W);
  }
  printf2("\n");
}				/* STOP */

/*****************************************************************************/
/* Runge_Kutta_4() - Runge-Kutta (Order Four) Method for an initial-value    */
/*                   differential equation.  See Equation 5.39.              */
/*****************************************************************************/
double Runge_Kutta_4(t,w)
double t, w;
{
  double K1, K2, K3, K4;

  K1 = h*f(t,       w);
  K2 = h*f(t + h/2, w + K1/2);
  K3 = h*f(t + h/2, w + K2/2);
  K4 = h*f(t + h,   w + K3);

  return (w + (K1 + 2*K2 + 2*K3 + K4)/6);
}

/*****************************************************************************/
/* Adams_Bashforth_2() - Adams-Bashforth Two-Step Method for an initial-     */
/*                       value differential equation.  See Equation 5.63.    */
/*****************************************************************************/
double Adams_Bashforth_2(t,w,i)
double t, w[5];
int i;
{
  return (w[i] + (h/2)*(3*f(t, w[i]) - f(t - h, w[i-1])));
}

/*****************************************************************************/
/* Adams_Bashforth_3() - Adams-Bashforth Three-Step Method for an initial-   */
/*                       value differential equation.  See Equation 5.64.    */
/*****************************************************************************/
double Adams_Bashforth_3(t,w,i)
double t, w[5];
int i;
{
  return (w[i] + (h/12)*(23*f(t, w[i]) - 16*f(t - h, w[i-1]) + 5*f(t - 2*h,
          w[i-2])));
}

/*****************************************************************************/
/* Adams_Bashforth_4() - Adams-Bashforth Four-Step Method for an initial-    */
/*                       value differential equation.  See Equation 5.65.    */
/*****************************************************************************/
double Adams_Bashforth_4(t,w,i)
double t, w[5];
int i;
{
  return (w[i] + (h/24)*(55*f(t, w[i]) - 59*f(t - h, w[i-1])
	 + 37*f(t - 2*h, w[i-2]) - 9*f(t - 3*h, w[i-3])));
}

/*****************************************************************************/
/* Adams_Bashforth_5() - Adams-Bashforth Five-Step Method for an initial-    */
/*                       value differential equation.  See Equation 5.66.    */
/*****************************************************************************/
double Adams_Bashforth_5(t,w,i)
double t, w[5];
int i;
{
  return (w[i] + (h/720)*(1901*f(t, w[i]) - 2774*f(t - h, w[i-1])
	+ 2616*f(t - 2*h, w[i-2]) - 1274*f(t - 3*h, w[i-3])
	+ 251*f(t - 4*h, w[i-4])));
}

/*****************************************************************************/
/* Milne() - Milne's Method for an initial-value differential equation.      */
/*           See Equation 5.70.                                              */
/*****************************************************************************/
double Milne(t,w,i)
double t, w[5];
int i;
{
  return (w[i-3] + (4*h/3)*(2*f(t, w[i]) - f(t - h, w[i-1])
	+ 2*f(t - 2*h, w[i-2])));
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
