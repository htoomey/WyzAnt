/******************************************************************************
       1st Derivative Approximation (for tabulated data) - Algorithm 4.0B2
*******************************************************************************

To approximate the derivative of discrete data points at the point x0 for a
given h.  Uses Equation 4.2, 4.12, 4.13, 4.14, or 4.15.

INPUT the point x0; interval h; data points x[-2],...,x[4] as needed.

OUTPUT approximation f'(x0).

NOTE: This algorithm was included as a "Homework Helper."  See p. 154,
Exercise Set 4.1, Problems 1-2, 7, 13 a), 15, 17.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "040b2.out";	/* Customized default output file name.     */
double *X;


main()
{
  double Eq4_2(), Eq4_12(), Eq4_13(), Eq4_14(), Eq4_15();
  double x0, h, Dxf;
  int ch, Eqnum;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("1st Derivative Approximation (for tabulated data)");
  printf2(" - Algorithm 4.0B2\n\n");

  /* Dynamically allocate memory for the needed arrays. */
  X = dvector(-2,4);		/* Stores f(x) of all data points. */

  do {

    printf("Select desired formula:              ");
    printf("     x-2, x-1, x0, x1, x2, x3, x4\n");
    printf("  (1)   Equation 4.2   (2-Point Formula) ");
    printf("            X   X\n");
    printf("  (2)   Equation 4.12  (3-Point Formula) ");
    printf("            X   X   X\n");
    printf("  (3)   Equation 4.13  (3-Point Formula) ");
    printf("       X        X\n");
    printf("  (4)   Equation 4.14  (5-Point Formula) ");
    printf("  X    X        X   X\n");
    printf("  (5)   Equation 4.15  (5-Point Formula) ");
    printf("            X   X   X   X   X\n");
    printf("----> ");
    scanf("%d", &Eqnum);

    switch (Eqnum) {
      case 1:  printf2("Using Equation 4.2  (Two-Point Formula)");    break;
      case 2:  printf2("Using Equation 4.12 (Three-Point Formula)");  break;
      case 3:  printf2("Using Equation 4.13 (Three-Point Formula)");  break;
      case 4:  printf2("Using Equation 4.14 (Five-Point Formula)");   break;
      case 5:  printf2("Using Equation 4.15 (Five-Point Formula)");   break;
      default: printf2("Using Equation 4.13 (Three-Point Formula)");  break;
    }

    printf("\nEnter point to evaluate, x0: ");
    scanf("%lf", &x0);

    printf("Enter interval value, h (may be negative): ");
    scanf("%lf", &h);
    fprintf(file_id, " with h = %lg:\n", h);
    fprintf(file_id, "Using points:\n");

    switch (Eqnum) {
      case 1:  printf("Enter f(%lg): ", x0);
               scanf("%lf", &X[0]);
               fprintf(file_id, "\tf(%lg) = %lf\n", x0, X[0]);

               printf("Enter f(%lg): ", x0 + h);
               scanf("%lf", &X[1]);
               fprintf(file_id, "\tf(%lg) = %lf\n", x0 + h, X[1]);

               /* STEP #1 */
               Dxf = Eq4_2(h);		/* Evaluate the 1st derivative */
               break;

      case 2:  printf("Enter f(%lg): ", x0);
               scanf("%lf", &X[0]);
               fprintf(file_id, "\tf(%lg) = %lf\n", x0, X[0]);

               printf("Enter f(%lg): ", x0 + h);
               scanf("%lf", &X[1]);
               fprintf(file_id, "\tf(%lg) = %lf\n", x0 + h, X[1]);

               printf("Enter f(%lg): ", x0 + 2*h);
               scanf("%lf", &X[2]);
               fprintf(file_id, "\tf(%lg) = %lf\n", x0 + 2*h, X[2]);

               /* STEP #1 */
               Dxf = Eq4_12(h);		/* Evaluate the 1st derivative */
               break;

      case 3:  printf("Enter f(%lg): ", x0 - h);
               scanf("%lf", &X[-1]);
               fprintf(file_id, "\tf(%lg) = %lf\n", x0 - h, X[-1]);

               printf("Enter f(%lg): ", x0 + h);
               scanf("%lf", &X[1]);
               fprintf(file_id, "\tf(%lg) = %lf\n", x0 + h, X[1]);

               /* STEP #1 */
               Dxf = Eq4_13(h);		/* Evaluate the 1st derivative */
               break;

      case 4:  printf("Enter f(%lg): ", x0 - 2*h);
               scanf("%lf", &X[-2]);
               fprintf(file_id, "\tf(%lg) = %lf\n", x0 - 2*h, X[-2]);

               printf("Enter f(%lg): ", x0 - h);
               scanf("%lf", &X[-1]);
               fprintf(file_id, "\tf(%lg) = %lf\n", x0 - h, X[-1]);

               printf("Enter f(%lg): ", x0 + h);
               scanf("%lf", &X[1]);
               fprintf(file_id, "\tf(%lg) = %lf\n", x0 + h, X[1]);

               printf("Enter f(%lg): ", x0 + 2*h);
               scanf("%lf", &X[2]);
               fprintf(file_id, "\tf(%lg) = %lf\n", x0 + 2*h, X[2]);

               /* STEP #1 */
               Dxf = Eq4_14(h);		/* Evaluate the 1st derivative */
               break;

      case 5:  printf("Enter f(%lg): ", x0);
               scanf("%lf", &X[0]);
               fprintf(file_id, "\tf(%lg) = %lf\n", x0, X[0]);

               printf("Enter f(%lg): ", x0 + h);
               scanf("%lf", &X[1]);
               fprintf(file_id, "\tf(%lg) = %lf\n", x0 + h, X[1]);

               printf("Enter f(%lg): ", x0 + 2*h);
               scanf("%lf", &X[2]);
               fprintf(file_id, "\tf(%lg) = %lf\n", x0 + 2*h, X[2]);

               printf("Enter f(%lg): ", x0 + 3*h);
               scanf("%lf", &X[3]);
               fprintf(file_id, "\tf(%lg) = %lf\n", x0 + 3*h, X[3]);

               printf("Enter f(%lg): ", x0 + 4*h);
               scanf("%lf", &X[4]);
               fprintf(file_id, "\tf(%lg) = %lf\n", x0 + 4*h, X[4]);

               /* STEP #1 */
               Dxf = Eq4_15(h);		/* Evaluate the 1st derivative */
               break;

      default: printf("Enter f(%lg): ", x0 - h);
               scanf("%lf", &X[-1]);
               fprintf(file_id, "\tf(%lg) = %lf\n", x0 - h, X[-1]);

               printf("Enter f(%lg): ", x0 + h);
               scanf("%lf", &X[1]);
               fprintf(file_id, "\tf(%lg) = %lf\n", x0 + h, X[1]);

               /* STEP #1 */
               Dxf = Eq4_13(h);		/* Evaluate the 1st derivative */
               break;
    }

    /***********
     * OUTPUTS *
     ***********/

    printf2("f'(%lg) = %lf\n", x0, Dxf);
    fprintf(file_id, "\n");

    printf("\nWould you like to try another method, h, or x0? (Y/N) N: ");
    getchar();				/* Cleans up after scanf(). */
    ch = tolower(getchar());

  } while (ch == 'y');

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(X,-2,4);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/**************
 * ALGORITHMS *
 **************/

/*****************************************************************************/
/* Eq4_2() - First derivative equation 4.2.  (see p.146 of text)             */
/*****************************************************************************/
double Eq4_2(h)
double h;
{
  return ((X[1] - X[0]) / h);
}

/*****************************************************************************/
/* Eq4_12() - First derivative equation 4.12.  (see p.146 of text)           */
/*****************************************************************************/
double Eq4_12(h)
double h;
{
  return ((-3.0*X[0] + 4.0*X[1] - X[2]) / (2.0*h));
}

/*****************************************************************************/
/* Eq4_13() - First derivative equation 4.13.  (see p.149 of text)           */
/*****************************************************************************/
double Eq4_13(h)
double h;
{
  return ((X[1] - X[-1]) / (2.0*h));
}

/*****************************************************************************/
/* Eq4_14() - First derivative equation 4.14.  (see p.150 of text)           */
/*****************************************************************************/
double Eq4_14(h)
double h;
{
  return ((X[-2] - 8.0*X[-1] + 8.0*X[1] - X[2])	 / (12.0*h));
}

/*****************************************************************************/
/* Eq4_15() - First derivative equation 4.15.  (see p.150 of text)           */
/*****************************************************************************/
double Eq4_15(h)
double h;
{
  return ((-25.0*X[0] + 48.0*X[1] - 36.0*X[2] + 16.0*X[3] - 3.0*X[4])
          / (12.0*h));
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
