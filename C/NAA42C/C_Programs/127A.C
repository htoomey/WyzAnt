/******************************************************************************
      Parabolic Equations with Newton Iteration in 2-D - Algorithm 12.7A
*******************************************************************************

2-D Evolution-Type Partial Differential Equation Solver (You must discretize
and solve for U(r,s,t)).

To approximate the solution of any Parabolic partial-differential equation

	Ut = F(t, x, y, Ut, Ux, Uxx, Uy, Uyy),  0 < x,y < 1,  0 < t < T*deltat,

        where Ut = ëU(t,x,y)/ët etc.

subject to the boundry conditions

	U(t,x,y) = f(),  for y = 0; y = 1;  0 < t ó T*deltat,
	U(t,x,y) = f(),  for x = 0; x = 1;  0 < t ó T*deltat,

and the initial conditions

	U(0,x,y) = initx(),  0 ó x,y ó 1:

INPUT see the #defines below with their accompaning comments.

OUTPUT approximations U_APPROX(t,x,y) to U(t,x,y).

OUTPUT MODES:

ASCII_FLAG - To save ALL output points as ASCII data into the output file,
use the function "print_ascii2()";

BINARY_FLAG - To save ALL output points as binary data into the file defined
by binary_file, use the function "make_binary2()";

SAMPLE_FLAG - To see FIVE representative values of the total solution, use
the function "print_solution2()"  (Not usually used with "print_ascii2()").

*******************************************************************************
  EXAMPLE:

  Ut = Uxx + Uyy + (1/eps)^2 * F(U),  0 ó x,y ó 1
	where F(U) = U - U^3 (for nonlinearity) and U = U(t,x,y) = U[r][s][t]
        given that t = r*deltat ,x = s*deltax, and y = t*deltax.
    Boundary Conditions:
	Ux(t,x,y) = 0.0  for y = 0; y = 1;
	Ux(t,x,y) = 0.0  for x = 0; x = 1;
    Initial Conditions:
	U(0,x,y) = sumation from i = 0 to 22 of:
		alpha(i) * cos(i*PI*x) * cos(i*PI*y)
        where alpha(i) is a random number between 0 and 1.

*******************************************************************************

NOTE: This algorithm was included as a "Professor's Favorite, Must-Have"
algorithm.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities.  */

char *outfile     = "127a.out";	/* Default ASCII output file name.           */
char *binary_file = "127a.bin";	/* Default binary output file name.          */
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
				/* Must be <= T.                             */

#define	FIRSTTIME TRUE		/* Set to TRUE if TSTART = 1, otherwise set  */
				/* to FALSE                                  */
				/* Useful for files with initial data.       */

#define ASCII_FLAG	FALSE	/* To save ALL output points as ASCII data   */
				/* into the output file, use the function    */
				/* "print_ascii2()";                         */

#define BINARY_FLAG	FALSE	/* To save ALL output points as binary data  */
				/* into the file defined by binary_file, use */
				/* the function "make_binary2()";            */

#define SAMPLE_FLAG	TRUE	/* To see FIVE representative values of the  */
				/* total solution, use the function          */
				/* "print_solution2()".                      */
				/* (Not usually used with "print_ascii2()"). */

double h   = 1.0e-4;		/* delta t                                   */
double eps = 0.10;		/* epsilon (0.1 to 0.01)                     */


/* DO NOT MODIFY THESE PARAMETERS */
#define	NUMPTS	((N + 1)*(N + 1))
double k = 1.0 / N;		/* delta x and delta y                       */
double U_APPROX(), f(), F(), infinite_norm2();
double **U, **UPREV, **UOLD, **TEMP, UU[NUMPTS];


/*****************************************************************************/
/* U_APPROX(r,s) - Solves for U[r][s] = ...   Is the heart of the algorithm. */
/*                 Needs updating $.                                         */
/*****************************************************************************/
double U_APPROX(r,s,t)
int r,s,t;
{
  /* Boundary conditions (Dirchelet and/or Neumann). */
  if (s==0)  return (U[s+1][t]);
  if (s==N)  return (U[s-1][t]);
  if (t==0)  return (U[s][t+1]);
  if (t==N)  return (U[s][t-1]);

  /* Internal points U[r][s][t] - r for time, s & t for space. */
  return((UPREV[s][t]/h + (U[s+1][t] + U[s-1][t] + U[s][t+1]
         + U[s][t-1])/(k*k)) / ((1.0/h) + (4.0/(k*k))
         + (U[s][t]*U[s][t] - 1)/(eps*eps)));
}

/*
**  HINT:  When you discretize and solve for U[r][s][t], be sure to go out of
**         your way to use large numbers such as (1.0/h).  Other seemingly
**         "good" discretizations will not work correctly.
**
**         U[r-1][s][t] = UPREV[s][t]
**         U[s][t] = U[r][s][t]
**         deltat = h
**         deltax = k
**         deltay = k
**
**  Remember: U = U[r][s][t]
**
**            Ut = (U[r+1][s][t] - U[r][s][t]) / deltat	(Forward Difference)
**            Ut = (U[r][s][t] - U[r-1][s][t]) / deltat	(Backward Difference)
**
**            Ux = (U[r][s+1][t] - U[r][s][t]) / deltax	(Forward Difference)
**            Ux = (U[r][s][t] - U[r][s-1][t]) / deltax	(Backward Difference)
**            Uxx = (U[r][s+1][t] - 2*U[r][s][t] + U[r][s-1][t]) / deltax^2
**
**            Uy = (U[r][s][t+1] - U[r][s][t]) / deltay	(Forward Difference)
**            Uy = (U[r][s][t] - U[r][s][t-1]) / deltay	(Backward Difference)
**            Uyy = (U[r][s][t+1] - 2*U[r][s][t] + U[r][s][t-1]) / deltay^2
*/

/*****************************************************************************/
/* init() - Initial condition function.  Needs updating $.                   */
/*****************************************************************************/
init()
{
  int i, s, t;
  double alpha[23], sum;

  for (s=0;s<=N;s++)
    for (t=0;t<=N;t++) {
      for (i=0;i<=22;i++)	/* Generate random alphas                    */
        alpha[i] = rand() / (double) RAND_MAX;	/* RAND_MAX usually = 32,767 */
				/* RAND_MAX is defined in <stdlib.h>.        */
      for (s=0;s<=N;s++)	/* Initial Conditions at t=0.                */
        for (t=0;t<=N;t++) {
          sum = 0.0;
          for (i=0;i<=22;i++)
            sum += alpha[i] * cos(i*PI*s*k) * cos(i*PI*t*k);
          U[s][t] = sum;
          UPREV[s][t] = sum;
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
  int r, s, t, iter, OK;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Parabolic Equations with Newton Iteration in 2-D ");
  printf2("- Algorithm 12.7A\n\n");

  printf2("(Answers will vary with this problem due to random alpha).\n\n");

  /* Dynamically allocate memory for the needed arrays. */
  U     = dmatrix(0,N,0,N);	/* Solution Matrix U for t=t.               */
  UPREV = dmatrix(0,N,0,N);	/* Solution Matrix U for t=t-deltat.        */
  UOLD  = dmatrix(0,N,0,N);	/* Solution Matrix U for (k-1)th iteration. */
  TEMP  = dmatrix(0,N,0,N);	/* For tolerance Checking.                  */

  /* INITIAL CONDITIONS */
  if (FIRSTTIME)		/* Initial Conditions at t=TSTART. */
    init();
  else
    for (s=0;s<=N;s++)		/* Used for reading in previously  */
      for (t=0;t<=N;t++) {	/* computed values to save time.   */
        scanf("%lf", &U[s][t]);
        UPREV[s][t] = U[s][t];
      }

  if (ASCII_FLAG == TRUE)
    print_ascii2();		/* Writes ASCII output to out_file.      */

  if (BINARY_FLAG == TRUE)
    make_binary2();		/* Writes binary output to binary_file.  */

  if (SAMPLE_FLAG == TRUE)
    print_solution2(0);		/* Writes some ASCII output to out_file. */

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  /* TIME DEPENDANT SOLUTIONS */
  for (r=TSTART;r<=T;r++) {
    fprintf(stderr, "Time interval #%d\tt = %lg\n", r, r*h);
    iter = 1;			/* For feedback.           */
    OK   = 0;
    do {

      /* STEP #2 */
      if (iter%2 == 1) {	/* Alternate for accuracy. */
        for (s=0;s<=N;s++)
          for (t=0;t<=N;t++) {
            U[s][t] = U_APPROX(r,s,t);/* Update solution by one iteration */
            TEMP[s][t] = U[s][t] - UOLD[s][t];	/* For tolerance checking */
          }
      } else {
        for (t=0;t<=N;t++)
          for (s=0;s<=N;s++) {
            U[s][t] = U_APPROX(r,s,t);
            TEMP[s][t] = U[s][t] - UOLD[s][t];
          }
      }

      /***********
       * OUTPUTS *
       ***********/

      /* STEP #3 */
      if (infinite_norm2(TEMP) < TOL) {	/* Is U accurate enough? */
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
            print_ascii2();	/* Writes ASCII output to out_file.      */

          if (BINARY_FLAG == TRUE)
            make_binary2();	/* Writes binary output to binary_file.  */

          if (SAMPLE_FLAG == TRUE)
            print_solution2(r);	/* Writes some ASCII output to out_file. */

        }
      }

      /* STEP #4 */
      for (s=0;s<=N;s++)	/* Prepare for next "space" iteration */
        for (t=0;t<=N;t++)
          UOLD[s][t] = U[s][t];

      /* STEP #5 */
      if (iter++ == ITER) {
        printf2("\nMaximum number of iterations (%d) exceeded.\n\n", ITER);
        /* Free the memory that was dynamically allocated for the arrays. */
        free_dmatrix(TEMP,0,N,0,N);
        free_dmatrix(UOLD,0,N,0,N);
        free_dmatrix(UPREV,0,N,0,N);
        free_dmatrix(U,0,N,0,N);
        NAA_do_last(outfile);	/* NAA finish-up procedure.           */
      }
    } while (!OK);

    /* STEP #6 */
    for (s=0;s<=N;s++)		/* Prepare for next "time" iteration. */
      for (t=0;t<=N;t++)
        UPREV[s][t] = U[s][t];

  }  /* for */

  /* Free the memory that was dynamically allocated for the arrays.   */
  free_dmatrix(TEMP,0,N,0,N);
  free_dmatrix(UOLD,0,N,0,N);
  free_dmatrix(UPREV,0,N,0,N);
  free_dmatrix(U,0,N,0,N);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/****************************************************************************/
/* print_solution2() - Prints a FEW sample points, not all.                 */
/*                     For a 2-D array.                                     */
/****************************************************************************/
print_solution2(r)
int r;
{
  printf2("At t = %lg\n", r*h);
  printf2("%lf  %lf  %lf  %lf  %lf\n",
	U[0][0], U[N/4][0], U[N/2][0], U[(3*N)/4][0], U[N][0]);
  printf2("%lf  %lf  %lf  %lf  %lf\n",
	U[0][N/4], U[N/4][N/4], U[N/2][N/4], U[(3*N)/4][N/4], U[N][N/4]);
  printf2("%lf  %lf  %lf  %lf  %lf\n",
	U[0][N/2], U[N/4][N/2], U[N/2][N/2], U[(3*N)/4][N/2], U[N][N/2]);
  printf2("%lf  %lf  %lf  %lf  %lf\n",
	U[0][(3*N)/4], U[N/4][(3*N)/4], U[N/2][(3*N)/4], U[(3*N)/4][(3*N)/4],
	U[N][(3*N)/4]);
  printf2("%lf  %lf  %lf  %lf  %lf\n",
	U[0][N], U[N/4][N], U[N/2][N], U[(3*N)/4][N], U[N][N]);
  printf2("\n");
}

/****************************************************************************/
/* print_ascii2() - Prints ALL values of U in ASCII format.  Use            */
/*                  redirection for the output file.                        */
/*                  For a 2-D array.                                        */
/****************************************************************************/
print_ascii2()
{
  int s, t;

  for (s=0;s<=N;s++)
    for (t=0;t<=N;t++)
      printf("%lf\n", U[s][t]);
}

/****************************************************************************/
/* make_binary2() - Stores solution in Binary format into binary_file.      */
/*                  For a 2-D array.                                        */
/****************************************************************************/
make_binary2()
{
  FILE *stream;
  int s, t, kk=0;

  stream = fopen(binary_file, "a+b");
  for (s=0;s<=N;s++)
    for (t=0;t<=N;t++)
      UU[kk++] = U[s][t];
  fwrite((char *)UU, sizeof(double), NUMPTS, stream);
  fclose(stream);
}

/*****************************************************************************/
/* infinite_norm2() - Computes the infinite norm of a matrix.                */
/*                    For a 2-D array.                                       */
/*****************************************************************************/
double infinite_norm2(A)
double **A;
{
  double tmp, max = 0.0;
  int s, t;

  for (s=0;s<=N;s++)
    for (t=0;t<=N;t++)
      if ((tmp = fabs(A[s][t])) > max)
        max = tmp;

  return (max);
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
