/******************************************************************************
       Elliptic Equations with Newton Iteration in 2-D - Algorithm 12.8A
*******************************************************************************

2-D Elliptic-Type Partial Differential Equation Solver (You must discretize
and solve for U(r,s)).

To approximate the solution of any Elliptic partial-differential equation

	F1(x, y, Ux, Uxx, Uy, Uyy, Uxy) = F(),  0 < x,y < 1

        where Ux = ëU(x,y)/ëx etc.

subject to the boundry conditions

	U(x,y) = f(),  for y = 0; y = 1;
	U(x,y) = f(),  for x = 0; x = 1;

INPUT see the #defines below with their accompaning comments.

OUTPUT approximation to U(x,y) accurate to within TOL.

OUTPUT MODES:

ASCII_FLAG - To print the solution and error in ASCII.  Uses the functions
"print_ascii_solution()" and "print_ascii_error()".

BINARY_FLAG - To print the solution and error in binary.  Uses the functions
"print_binary_solution()" and "print_binary_error()".  (U_TRUE_FLAG must be
set to TRUE).

*******************************************************************************
  EXAMPLE:

  Uxx + (1+y*y)*Uyy - Ux - (1+y*y)*Uy = F(x,y),  0 ó x,y ó 1
	where F(x,y) = log(1+y*y) * (2+x*(-14+x*(18-4*x)))
		+ 2*(x*x*(x-1)*(x-1)*(1-y-(2*y*y)/(1+y*y))),
        and Ux = ëU(x,y)/ëx etc., and U = U(x,y) = U[r][s], given that
	x = r*deltax, and y = s*deltax.
    Boundary Conditions:
	-U + Ux = 0		on x = 1, 0 ó y ó 1;
	U = exp(x)		on 0 ó x ó 1, y = 0;
	U + Ux = 2*exp(y)	on x = 0, 0 ó y ó 1;
	U = exp(1+x) + x*x*(x-1)*(x-1)*log(2)	on 0 ó x ó 1, y = 1;

*******************************************************************************

NOTE: This algorithm was included as a "Professor's Favorite, Must-Have"
algorithm.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities.  */

char *outfile      = "128a.out";	/* Default ASCII output file name.   */
char *binary_file1 = "128a.bin";	/* Default binary output file name.  */
char *binary_file2 = "128aerr.bin";	/* Default binary error file name.   */
					/* (May be needed for PLOT79.)       */


/* MODIFY THESE PARAMETERS */
#define	ITER	2000	 	/* Maximum number of iterations              */
#define	TOL	5.0e-8		/* Desired tolerance                         */
#define	N	5		/* Number of grid points on a side (0..N+1). */

#define ASCII_FLAG	TRUE	/* To save ALL output points as ASCII data   */
				/* into the output file.  Uses the functions */
				/* "print_ascii_solution()" and              */
				/* "print_ascii_error()".                    */

#define BINARY_FLAG	FALSE	/* To save ALL output points as binary data  */
				/* into the files defined by binary_file1    */
				/* and binary_file2.  Uses the functions     */
				/* "print_binary_solution()" and             */
				/* "print_binary_error()".                   */

#define U_TRUE_FLAG	TRUE	/* Set to TRUE if the true solution is known.*/


#define	NUMPTS	((N + 1)*(N + 1))
double deltax = 1.0 / (N + 1);
double k = 1.0 / (N + 1);
double U_TRUE(), U_APPROX(), F(), g(), infinite_norm();
double **U, **UOLD, **TEMP;
FILE *stream;


/*****************************************************************************/
/* U_APPROX(r,s) - Returns next approximaton for solution U(x,y).            */
/*                 Needs updating $.                                         */
/*****************************************************************************/
double U_APPROX(r,s)
int r,s;
{
 /* Boundary conditions. */
 if (s==0)	/* y = 0 */
    return (exp(r*k));

  if (s==N+1)	/* y = 1 */
    return (exp(1.0 + r*k) + (r*k)*(r*k)*(r*k - 1.0)*(r*k - 1.0)*log(2.0));

  if (r==0)	/* x = 0 */
    return ((U[r+1][s] - 2.0*k*exp(s*k)) / (1.0 - k));

  if (r==N+1)	/* x = 1 */
    return (U[r-1][s] / (1.0 - k));

  /* Internal points U[r][s] - r & s for space. */
  return (((k-1)*(U[r+1][s] + g(r,s)*U[r][s+1]) - U[r-1][s] - g(r,s)*U[r][s-1]
	+ k*k*F(r,s)) / ((k - 2)*(1.0 + g(r,s))) );

/*
**  HINT:  When you discretize and solve for U[r][s], be sure to go out of
**         your way to use large numbers such as (1.0/k).  Other seemingly
**         "good" discretizations will not work correctly.
**
**         deltax = k
**         deltay = k
**
**  Remember: U = U[r][s]
**
**            Ux = (U[r+1][s] - U[r][s]) / deltax	(Forward Difference)
**            Ux = (U[r][s] - U[r-1][s]) / deltax	(Backward Difference)
**            Uxx = (U[r+1][s] - 2*U[r][s] + U[r-1][s]) / deltax^2
**
**            Uy = (U[r][s+1] - U[r][s]) / deltay	(Forward Difference)
**            Uy = (U[r][s] - U[r][s-1]) / deltay	(Backward Difference)
**            Uyy = (U[r][s+1] - 2*U[r][s] + U[r][s-1]) / deltay^2
*/
}

/*****************************************************************************/
/* g(r,s) - An often used function, g().  Needs updating $.                  */
/*****************************************************************************/
double g(r,s)
int r,s;
{
  double y;

  y = s*deltax;
  return (1.0 + y*y);
}

/*****************************************************************************/
/* F(r,s) - Function, F().  Needs updating $.                                */
/*****************************************************************************/
double F(r,s)
int r,s;
{
  double x,y;

  x = r*deltax;
  y = s*deltax;
  return (log(1.0 + y*y) * (2.0 + x*(-14.0 + x*(18.0 - 4.0*x)))
	+ 2.0*(x*x*(x - 1.0)*(x - 1.0)*(1.0 - y - (2.0*y*y)/(1.0 + y*y))) );
}

/*****************************************************************************/
/* U_TRUE(r,s) - True solution, if known.  Needs updating $.                 */
/*****************************************************************************/
double U_TRUE(r,s)
int r,s;
{
  double x,y;

  x = r * deltax;
  y = s * deltax;
  return (exp(x + y) + pow((x*(x - 1.0)), 2.0)*log(1.0 + y*y));
}
/*****************************************************************************/


main()
{
  int i, j, kk, r, s;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Elliptic Equations with Newton Iteration in 2-D");
  printf2(" - Algorithm 12.8A\n\n");

  /* Dynamically allocate memory for the needed arrays. */
  U    = dmatrix(0,N+1,0,N+1);		/* Solution Matrix U.     */
  UOLD = dmatrix(0,N+1,0,N+1);		/* Old solution Matrix U. */
  TEMP = dmatrix(0,N+1,0,N+1);		/* For Error Checking     */

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  for (kk=1;kk<=ITER;kk++) {
    fprintf(stderr, "Iteration #%d\n", kk);	/* For feedback.         */

    /* STEP #2 */
    if (kk%2 == 1) {		/* Alternate for faster convergence.     */
      for (r=0;r<=N+1;r++)
        for (s=0;s<=N+1;s++) {
	  U[r][s] = U_APPROX(r,s);/* Update solution by one iteration    */
          TEMP[r][s] = U[r][s] - UOLD[r][s];	/* For accuracy checking */
        }
    } else {
      for (s=0;s<=N+1;s++)
        for (r=0;r<=N+1;r++) {
	  U[r][s] = U_APPROX(r,s);/* Update solution by one iteration    */
          TEMP[r][s] = U[r][s] - UOLD[r][s];	/* For accuracy checking */
        }
    }

    /***********
     * OUTPUTS *
     ***********/

    /* STEP #3 */
    if (infinite_norm(TEMP) < TOL) {
      printf2("Solution took %d iterations.\n", kk);

      if (ASCII_FLAG == TRUE) {
        print_ascii_solution();	/* Writes ASCII output to out_file.        */
        if (U_TRUE_FLAG == TRUE)
          print_ascii_error();	/* Writes ASCII error to out_file.         */
      }

      if (BINARY_FLAG == TRUE) {
        print_binary_solution(); /* Writes binary output to binary_file1.  */
        if (U_TRUE_FLAG == TRUE)
          print_binary_error();  /* Writes binary error to binary_file2.   */
      }

      /* Free the memory that was dynamically allocated for the arrays. */
      free_dmatrix(TEMP,0,N+1,0,N+1);
      free_dmatrix(UOLD,0,N+1,0,N+1);
      free_dmatrix(U,0,N+1,0,N+1);
      NAA_do_last(outfile);	/* NAA finish-up procedure. */
      exit (1);
    }

    /* STEP #4 */
    for (r=1;r<=N;r++)		/* Prepare for next iteration */
      for (s=1;s<=N;s++)
        UOLD[r][s] = U[r][s];
  }

  /* STEP #5 */
  printf2("\nMaximum number of iterations (%d) exceeded.\n\n", ITER);
  if (ASCII_FLAG == TRUE) {
    print_ascii_solution();	/* Writes ASCII output to out_file.      */
    if (U_TRUE_FLAG == TRUE)
      print_ascii_error();	/* Writes ASCII error to out_file.       */
  }

  if (BINARY_FLAG == TRUE) {
    print_binary_solution();	/* Writes binary output to binary_file1. */
    if (U_TRUE_FLAG == TRUE)
      print_binary_error();	/* Writes binary error to binary_file2.  */
  }

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dmatrix(TEMP,0,N+1,0,N+1);
  free_dmatrix(UOLD,0,N+1,0,N+1);
  free_dmatrix(U,0,N+1,0,N+1);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* print_ascii_solution() - Outputs solution in ASCII form.                  */
/*****************************************************************************/
print_ascii_solution()
{
  int i, j;

  /* SOLUTION */
  for (j=0;j<=N+1;j++) {	/* Print solution Vector U. */
    printf2("\n");
    for (i=0;i<=N+1;i++) {
      if (U_TRUE_FLAG == TRUE)
        printf2("U(%d,%d) =% .10lf\tU_TRUE = % .10lf\n",
		i, j, U[i][j], U_TRUE(i,j));
      else
        printf2("U(%d,%d) =% .10lf\n", i, j, U[i][j]);
    }
  }
}

/*****************************************************************************/
/* print_binary_solution() - Outputs solution in binary form.                */
/*****************************************************************************/
print_binary_solution()		/* Outputs solution in binary form. */
{
  int i, j;
  double UU[NUMPTS];

  stream = fopen(binary_file1, "w+b");

  /* SOLUTION */
  for (j=0;j<=N+1;j++)		/* Load solution Vector UU. */
    for (i=0;i<=N+1;i++)
      UU[j*(N+2)+i] = U[i][j];

  /* Write error to a binary file, usually for plotting. */
  fwrite((char *)UU, sizeof(double), NUMPTS, stream);
  fclose(stream);
}

/****************************************************************************/
/* print_ascii_error() - Outputs error in ASCII form.                       */
/****************************************************************************/
print_ascii_error()
{
  int i, j;

  /* ERROR */
  for (j=0;j<=N+1;j++) {
    printf2("\n");
    for (i=0;i<=N+1;i++)
      printf2("ERROR(%d,%d) =% .3lE\n", i, j, fabs(U_TRUE(i,j) - U[i][j]));
  }
}

/*****************************************************************************/
/* print_binary_error() - Outputs error in binary form.                      */
/*****************************************************************************/
print_binary_error()		/* Outputs error in binary form. */
{
  int i, j;
  double UU[NUMPTS];

  stream = fopen(binary_file2, "w+b");

  /* ERROR */
  for (j=0;j<=N+1;j++)
    for (i=0;i<=N+1;i++)
      UU[j*(N+2)+i] = fabs(U_TRUE(i,j) - U[i][j]);

  /* Write error to a binary file, usually for plotting. */
  fwrite((char *)UU, sizeof(double), NUMPTS, stream);
  fclose(stream);
}

/*****************************************************************************/
/* infinite_norm() - Computes the infinite norm of a matrix.                 */
/*****************************************************************************/
double infinite_norm(A)
double **A;
{
  double tmp, max = 0.0;
  int i, j;

  for (i=1;i<=N;i++)
    for (j=1;j<=N;j++)
      if ((tmp = fabs(A[i][j])) > max)
        max = tmp;

  return (max);
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
