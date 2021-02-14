/******************************************************************************
		Newton-Cotes Formulas for Integrals - Algorithm 4.1D
*******************************************************************************

                                ôb
To approximate the integral I = ³  f(x) dx.
                                õa

Uses Equations 4.40, 4.41, 4.42, 4.43, 4.46, 4.47, 4.48, 4.49:

INPUT endpoints a, b; formula selection, type; the function f().

OUTPUT approximation I to I.

NOTE: This algorithm was included as a "Homework Helper."  See p. 171,
Exercise Set 4.3, Problems 1-8.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile   = "041d.out";	/* Customized default output file name.     */
char *eq_text_f = "f(x) = sin(x)";	/* Needs updating  $  */
double a, b, f();


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
  int ch, type;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Newton-Cotes Formulas for Integrals - Algorithm 4.1D\n\n");

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

    printf("\nSelect desired formula:\n");
    printf("  Closed Newton-Cotes Formulas:\n");
    printf("    (1)   (n=1)  Trapezoidal Rule             Eq. (4.40)\n");
    printf("    (2)   (n=2)  Simpson's Rule               Eq. (4.41)\n");
    printf("    (3)   (n=3)  Simpson's Three-eighth Rule  Eq. (4.42)\n");
    printf("    (4)   (n=4)                               Eq. (4.43)\n");
    printf("  Open Newton-Cotes Formulas:\n");
    printf("    (5)   (n=0)  Midpoint Rule                Eq. (4.46)\n");
    printf("    (6)   (n=1)                               Eq. (4.47)\n");
    printf("    (7)   (n=2)                               Eq. (4.48)\n");
    printf("    (8)   (n=3)                               Eq. (4.49)\n");
    printf("    (9)   All of the above (1-8).\n");
    printf("----> ");
    scanf("%d", &type);
    printf("\n");

    /*************
     * ALGORITHM *
     *************/

    /* STEP #1 */
    if ((type < 1) || (type >= 9)) {	/* Default to all methods */
      for (type=1;type<=8;type++)
        Evaluate(type);
    } else			/* Try just one method at a time. */
      Evaluate(type);

    printf("Would you like to try another method? (Y/N) N: ");
    getchar();			/* Cleans up after scanf(). */
    ch = tolower(getchar());

  } while (ch == 'y');

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* Evaluate() - Evaluates a specific integral formula.                       */
/*****************************************************************************/
Evaluate(type)
int type;
{
  double Eq4_40(), Eq4_41(), Eq4_42(), Eq4_43();
  double Eq4_46(), Eq4_47(), Eq4_48(), Eq4_49();
  double I;

  switch (type) {
    case 1: printf2("Using Eq. (4.40)  Trapezoidal Rule\n\n");
            I = Eq4_40();  break;

    case 2: printf2("Using Eq. (4.41)  Simpson's Rule\n\n"); 
            I = Eq4_41();  break;

    case 3: printf2("Using Eq. (4.42)  Simpson's Three-Eights Rule\n\n"); 
            I = Eq4_42();  break;

    case 4: printf2("Using Eq. (4.43)  \n\n"); 
            I = Eq4_43();  break;

    case 5: printf2("Using Eq. (4.46)  Midpoint Rule\n\n"); 
            I = Eq4_46();  break;

    case 6: printf2("Using Eq. (4.47)  \n\n"); 
            I = Eq4_47();  break;

    case 7: printf2("Using Eq. (4.48)  \n\n"); 
            I = Eq4_48();  break;

    case 8: printf2("Using Eq. (4.49)  \n\n"); 
            I = Eq4_49();  break;
  }

  /***********
   * OUTPUTS *
   ***********/

  printf2("    ô%lg\n", b);
  printf2("I = ³  f(x) dx = %.11lg\n", I);
  printf2("    õ%lg\n\n", a);

}				/* STOP */

/*****************************************************************************/
/* Eq4_40() - Equation 4.40, p.169 (Trapezoidal Rule) for integration.       */
/*****************************************************************************/
double Eq4_40()
{
  return (((b - a)/2.0) * (f(a) + f(b)));
}

/*****************************************************************************/
/* Eq4_41() - Equation 4.41, p.169 (Simpson's Rule) for integration.         */
/*****************************************************************************/
double Eq4_41()
{
  double h;

  h = (b - a) / 2.0;
  return ((h/3.0) * (f(a) + 4.0*f(a+h) + f(b)));
}

/*****************************************************************************/
/* Eq4_42() - Equation 4.42, p.169 (Simpson's Three-eights Rule) for         */
/*            integration.                                                   */
/*****************************************************************************/
double Eq4_42()
{
  double h;

  h = (b - a) / 3.0;
  return (((3.0*h)/8.0) * (f(a) + 3.0*f(a + h) + 3.0*f(a + 2*h) + f(b)));
}

/*****************************************************************************/
/* Eq4_43() - Equation 4.43, p.169 for integration.                          */
/*****************************************************************************/
double Eq4_43()
{
  double h;

  h = (b - a) / 4.0;
  return (((2.0*h)/45.0) * (7.0*f(a) + 32.0*f(a+h) + 12.0*f(a + 2*h)
	+ 32.0*f(a + 3*h) + 7.0*f(b)));
}

/*****************************************************************************/
/* Eq4_46() - Equation 4.46, p.170 (Midpoint Rule) for integration.          */
/*****************************************************************************/
double Eq4_46()
{
  double h;

  h = (b - a) / 2.0;
  return (2.0*h * f(a + h));
}

/*****************************************************************************/
/* Eq4_47() - Equation 4.47, p.171 for integration.                          */
/*****************************************************************************/
double Eq4_47()
{
  double h;

  h = (b - a) / 3.0;
  return (((3.0*h)/2.0) * (f(a + h) + f(a + 2*h)));
}

/*****************************************************************************/
/* Eq4_48() - Equation 4.48, p.171 for integration.                          */
/*****************************************************************************/
double Eq4_48()
{
  double h;

  h = (b - a) / 4.0;
  return (((4.0*h)/3.0) * (2.0*f(a + h) - f(a + 2*h) + 2.0*f(a + 3*h)));
}

/*****************************************************************************/
/* Eq4_49() - Equation 4.49, p.171 for integration.                          */
/*****************************************************************************/
double Eq4_49()
{
  double h;

  h = (b - a) / 5.0;
  return (((5.0*h)/24.0) * (11.0*f(a + h) + f(a + 2*h) + f(a + 3*h)
         + 11.0*f(a + 4*h)));
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
