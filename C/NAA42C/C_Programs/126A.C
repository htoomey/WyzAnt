/******************************************************************************
       Parabolic Equations with Newton Iteration in 1-D - Algorithm 12.6A
*******************************************************************************

1-D Evolution-Type Partial Differential Equation Solver (You must discretize
and solve for U(r,s)).

To approximate the solution of any Parabolic partial-differential equation

	Ut = F(t, x, Ut, Ux, Uxx),  0 < x < 1,  0 < t < T*deltat,

        where Ut = ëU(t,x)/ët etc.

subject to the boundry conditions

	U(t,x) = f(),  for x = 0; x = 1;  0 < t ó T*deltat,

and the initial conditions

	U(0,x) = init(),  0 ó x ó 1:

INPUT see the #defines below with their accompaning comments.

OUTPUT approximations U_APPROX(t,x) to U(t,x).

OUTPUT MODES:

BINARY_FLAG - To save ALL output points as binary data into the file defined
by binary_file, use the function "make_binary1()";

ASCII_FLAG - To save ALL output points as ASCII data into the output file,
use the function "print_ascii1()";

SAMPLE_FLAG - To see FIVE representative values of the total solution, use
the function "print_solution1()"  (Not usually used with "print_ascii1()").

*******************************************************************************
  EXAMPLE:

  Ut = Uxx + (1/eps)^2 * F(U)
	where F(U) = U - U^3 (for nonlinearity) and U = U(t,x) = U[r][s]
        given that t = r*deltat and x = s*deltax.
    Boundary Conditions:
	Ux(t,0) = Ux(t,1) = 0.0
    Initial Conditions:
	U(0,x) = sumation from i = 0 to 22 of:
		alpha(i) * cos(i*PI*x)
        where alpha(i) is a random number between 0 and 1.

*******************************************************************************

NOTE: This algorithm was included as a "Professor's Favorite, Must-Have"
algorithm.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities.  */

char *outfile     = "126a.out";	/* Default ASCII output file name.           */
char *binary_file = "126a.bin";	/* Default binary output file name.          */
				/* (May be needed for PLOT79.)               */


/* MODIFY THESE PARAMETERS */
#define	ITER	2000	 	/* Maximum number of iterations              */

#define	TOL	5.0e-11		/* Desired tolerance                         */

#define	N	4		/* Number of grid points on a side (0..N).   */
				/* N = 64 is nice for 3-D plots, but may be  */
				/* too time consuming.  N = 16 is reasonable.*/

#define	T	300		/* Maximum number of time intervals.         */
				/* note that tmax = T*deltat                 */

#define	TSTART	1		/* Set to 1 initially.  Use other if you     */
				/* already have initial data (t = TSTART*h). */
				/* Must be <= T                              */

#define	FIRSTTIME 1		/* Set to 1 if TSTART = 1, otherwise set to 0*/
				/* Useful for files with initial data.       */


#define ASCII_FLAG	FALSE	/* To save ALL output points as ASCII data   */
				/* into the output file, use the function    */
				/* "print_ascii1()";                         */

#define BINARY_FLAG	FALSE	/* To save ALL output points as binary data  */
				/* into the file defined by binary_file, use */
				/* the function "make_binary1()";            */

#define SAMPLE_FLAG	TRUE	/* To see FIVE representative values of the  */
				/* total solution, use the function          */
				/* "print_solution1()".                      */
				/* (Not usually used with "print_ascii1()"). */

double h   = 1.0e-4;		/* delta t                                   */
double eps = 0.10;		/* epsilon (0.1 to 0.01)                     */



/* DO NOT MODIFY THESE PARAMETERS */
#define	NUMPTS	(N + 1)
double k = 1.0 / N;		/* delta x */
double U_APPROX(), f(), F(), infinite_norm1();
double *U, *UPREV, *UOLD, *TEMP;


/*****************************************************************************/
/* U_APPROX(r,s) - Solves for U[r][s] = ...   Is the heart of the algorithm. */
/*                 Needs updating $.                                         */
/*****************************************************************************/
double U_APPROX(r,s)
int r,s;
{
  /* Boundary conditions (Dirchelet and/or Neumann). */
  if (s==0)  return (U[s+1]);
  if (s==N)  return (U[s-1]);

  /* Internal points U[r][s] - r for time, s for space. Heart of algorithm. */
  return ((UPREV[s]/h + (U[s+1] + U[s-1])/(k*k))
	/ ((1.0/h) + (2.0/(k*k)) + (U[s]*U[s] - 1)/(eps*eps)));
}

/*
**  HINT:  When you discretize and solve for U[r][s], be sure to go out of
**         your way to use large numbers such as (1.0/h).  Other seemingly
**         "good" discretizations will not work correctly.
**
**         U[r-1][s] = UPREV[s]
**         U[s] = U[r][s]
**         deltat = h
**         deltax = k
**
**  Remember: U = U[r][s]    
**
**            Ut = (U[r+1][s] - U[r][s]) / deltat	(Forward Difference)
**            Ut = (U[r][s] - U[r-1][s]) / deltat	(Backward Difference)
**
**            Ux = (U[r][s+1] - U[r][s]) / deltax	(Forward Difference)
**            Ux = (U[r][s] - U[r][s-1]) / deltax	(Backward Difference)
**            Uxx = (U[r][s+1] - 2*U[r][s] + U[r][s-1]) / deltax^2
*/

/*****************************************************************************/
/* init() - Initial condition function.  Needs updating $.                   */
/*****************************************************************************/
init()
{
  int i, s;
  double alpha[23], sum;

  for (s=0;s<=N;s++) {
    for (i=0;i<=22;i++)		/* Generate random alphas                    */
      alpha[i] = rand() / (double) RAND_MAX;	/* RAND_MAX usually = 32,767 */
				/* RAND_MAX is defined in <stdlib.h>.        */
    for (s=0;s<=N;s++) {	/* Initial Conditions at t=0.                */
      sum = 0.0;
      for (i=0;i<=22;i++)
        sum += alpha[i] * cos(i*PI*s*k);
      UPREV[s] = U[s] = sum;
    }
  }
}

/*****************************************************************************/
/* f() - Function f() to be evaluated.  Needs updating $.                    */
/*****************************************************************************/
double f()
{
  return(0.0);		/* f() was discretized in this example. */
}

/*****************************************************************************/
/* F() - Function F() to be evaluated.  Needs updating $.                    */
/*****************************************************************************/
double F()
{
  return(0.0);		/* F() was discretized in this example. */
}
/*****************************************************************************/


main()
{
  int r, s, iter, OK;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure.            */

  printf2("Parabolic Equations with Newton Iteration in 1-D");
  printf2(" - Algorithm 12.6A\n\n");

  printf2("(Answers will vary with this problem due to random alpha).\n\n");

  /* Dynamically allocate memory for the needed arrays. */
  U     = dvector(0,N);		/* Solution Matrix U for t=t.               */
  UPREV = dvector(0,N);		/* Solution Matrix U for t=t-deltat.        */
  UOLD  = dvector(0,N);		/* Solution Matrix U for (k-1)th iteration. */
  TEMP  = dvector(0,N);		/* For tolerance Checking.                  */

  /* INITIAL CONDITIONS */
  if (FIRSTTIME)		/* Initial Conditions at t=TSTART.          */
    init();
  else
    for (s=0;s<=N;s++) {	/* Used for reading in previously computed  */
      scanf("%lf", &U[s]);	/* values from standard-in to save time.    */
      UPREV[s] = U[s];
    }

  if (ASCII_FLAG == TRUE)
    print_ascii1();		/* Writes ASCII output to out_file.      */

  if (BINARY_FLAG == TRUE)
    make_binary1();		/* Writes binary output to binary_file.  */

  if (SAMPLE_FLAG == TRUE)
    print_solution1(0);		/* Writes some ASCII output to out_file. */

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  /* TIME DEPENDANT SOLUTIONS */
  for (r=TSTART;r<=T;r++) {
    fprintf(stderr, "Time interval #%d\tt = %lg\n", r, r*h); /* For feedback */
    iter = 1;
    OK = 0;

    do {

      /* STEP #2 */
      for (s=0;s<=N;s++) {
        U[s]    = U_APPROX(r,s);	/* Update solution by one iteration. */
        TEMP[s] = U[s] - UOLD[s];	/* For tolerance checking.           */
      }

      /***********
       * OUTPUTS *
       ***********/

      /* STEP #3 */
      if (infinite_norm1(TEMP) < TOL) {	/* Is U accurate enough? */
        OK = 1;

	/*
	**  Can produce output at several time instances, appending to file.
	**  t = h     = 0.0001
	**  t = 10*h  = 0.001
	**  t = 100*h = 0.01    and
	**  t = 300*h = 0.03.
	*/

	if ((r==1) || (r==10) || (r==100) || (r==T)) {

          if (ASCII_FLAG == TRUE)
            print_ascii1();	/* Writes ASCII output to out_file.      */

          if (BINARY_FLAG == TRUE)
            make_binary1();	/* Writes binary output to binary_file.  */

          if (SAMPLE_FLAG == TRUE)
            print_solution1(r);	/* Writes some ASCII output to out_file. */

        }
      }

      /* STEP #4 */
      for (s=0;s<=N;s++)	/* Prepare for next "space" iteration */
        UOLD[s] = U[s];

      /* STEP #5 */
      if (iter++ == ITER) {
        printf2("\nMaximum number of iterations (%d) exceeded.\n\n", ITER);
        /* Free the memory that was dynamically allocated for the arrays. */
        free_dvector(TEMP,0,N);
        free_dvector(UOLD,0,N);
        free_dvector(UPREV,0,N);
        free_dvector(U,0,N);
        NAA_do_last(outfile);	/* NAA finish-up procedure. */
      }
    } while (!OK);

    /* STEP #6 */
    for (s=0;s<=N;s++)		/* Prepare for next "time" iteration. */
      UPREV[s] = U[s];

  } /* for */

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(TEMP,0,N);
  free_dvector(UOLD,0,N);
  free_dvector(UPREV,0,N);
  free_dvector(U,0,N);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/****************************************************************************/
/* print_solution1() - Prints a FEW sample points, not all.                 */
/*                     For a 1-D array.                                     */
/****************************************************************************/
print_solution1(r)
int r;
{
  printf2("At t = %lg\n", r*h);
  printf2("%lf  %lf  %lf  %lf  %lf\n\n",
    U[0], U[N/4], U[N/2], U[(3*N)/4], U[N]);
}

/****************************************************************************/
/* print_ascii1() - Prints ALL values of U in ASCII format.                 */
/*                  For a 1-D array.                                        */
/****************************************************************************/
print_ascii1()
{
  int s;

  for (s=0;s<=N;s++)
    printf("%lf\n", U[s]);
}

/****************************************************************************/
/* make_binary1() - Stores solution in Binary format into binary_file.      */
/*                  For a 1-D array.                                        */
/****************************************************************************/
make_binary1()
{
  FILE *stream;

  stream = fopen(binary_file, "a+b");
  fwrite((char *)U, sizeof(double), NUMPTS, stream);
  fclose(stream);
}

/*****************************************************************************/
/* infinite_norm1() - Computes the infinite norm of a vector.                */
/*                    For a 1-D array.                                       */
/*****************************************************************************/
double infinite_norm1(A)
double *A;
{
  double tmp, max = 0.0;
  int s;

  for (s=0;s<=N;s++)
    if ((tmp = fabs(A[s])) > max)
      max = tmp;

  return (max);
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
