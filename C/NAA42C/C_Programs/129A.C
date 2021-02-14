/******************************************************************************
	Biharmonic Equation using Gauss-Jordan Method - Algorithm 12.9A
*******************************************************************************

2-D Biharmonic Partial Differential Equation Solver (U(r,s) has already been
discretized and put into matrix format).

To approximate the solution of the Biharmonic partial-differential equation

	Uxxxx - 2*Uxxyy + Uyyyy = f(x,y),  0 < x,y < 1
        where Uxxxx = ë(4)U(x,y)/ëx(4) etc. and
	U[i][j] = U(i*k,j*k), i = 0,1,2,...,N, j = 0,1,2,...,N.

subject to the boundry conditions

	U(0,y)  = f1(y), U(1,y)  = f2(y),
	U(x,0)  = g1(x), U(x,1)  = g2(x),
	Ux(0,y) = u0(y), Ux(1,y) = u1(y),
	Uy(x,0) = v0(x), Uy(x,1) = v1(x);

with imaginary points

	U[-1][j]  = U[0][j] - k*Ux(0,j*k),
	U[N+1][j] = U[N0][j] + k*Ux(1,j*k),
	U[i][-1]  = U[i][0] - k*Uy(i*k,0),
	U[i][N+1] = U[i][N] + k*Uy(i*k,1),
	k = 0,1,2,...,N.

INPUT see the #defines below with their accompaning comments.

OUTPUT approximation to U(x,y).

OUTPUT MODES:

ASCII_FLAG - To print the solution in ASCII.  Uses the function
"print_ascii()".

BINARY_FLAG - To print the solution in binary.  Uses the function
"print_binary()".

*******************************************************************************
  EXAMPLE:

  Uxxxx - 2*Uxxyy + Uyyyy = f(x,y),  0 < x,y < 1
	where f(x,y) = sin(x*y), and U = U(x,y) = U[r][s], given that
	x = r*deltax, and y = s*deltay.
    Boundary Conditions:
	g(0,y) = g(1,y) = g(x,0) = g(x,1) = sin(x*y),
        gx(0,y) = gx(1,y) = y*cos(x*y),
        gy(x,0) = gy(x,1) = x*cos(x*y).

*******************************************************************************

NOTE: This algorithm was included as a "Professor's Favorite, Must-Have"
algorithm.
        
*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities.  */
#include "gaussj.c"		/* Solves Ax=b using Gauss-Jordan method.    */

char *outfile     = "129a.out";	/* Default ASCII output file name.           */
char *binary_file = "129a.bin";	/* Default binary output file name.          */
				/* (May be needed for PLOT79.)               */

/* MODIFY THESE PARAMETERS */
#define	N	6		/* Number of intervals (0..N).               */
#define	n	25		/* Dimension for matrix A.                   */
#define	m	5		/* Dimension for matrices D, B, & C.         */

#define ASCII_FLAG	TRUE	/* To save ALL output points as ASCII data   */
				/* into the output file.  Uses the function  */
				/* "print_ascii()"                           */

#define BINARY_FLAG	FALSE	/* To save ALL output points as binary data  */
				/* into the files defined by binary_file.    */
				/* Uses the function "print_binary()".       */


#define	NUMPTS	((m + 2)*(m + 2))
double k = 1.0 / N;		/* k = delta x = delta y                     */
double **A, **BB, **X;
double U(), UX(), UY(), f();

double D[5][5] = {
	{ 20, -8,  1,  0,  0 },
	{ -8, 20, -8,  1,  0 },
	{  1, -8, 20, -8,  1 },
	{  0,  1, -8, 20, -8 },
	{  0,  0,  1, -8, 20 },
};

double B[5][5] = {
	{ -8,  2,  0,  0,  0 },
	{  2, -8,  2,  0,  0 },
	{  0,  2, -8,  2,  0 },
	{  0,  0,  2, -8,  2 },
	{  0,  0,  0,  2, -8 },
};

double C[5][5] = {
	{ 1, 0, 0, 0, 0 },
	{ 0, 1, 0, 0, 0 },
	{ 0, 0, 1, 0, 0 },
	{ 0, 0, 0, 1, 0 },
	{ 0, 0, 0, 0, 1 },
};


/*****************************************************************************/
/* U(r,s) - Boundary conditions of g(x,y).  Needs updating $.                */
/*****************************************************************************/
double U(r,s)
int r,s;
{
  if (r == -1)			/* Imaginary Points */
    return (U(0,s) - k*UX(0,s));

  if (r == N+1)
    return (U(N,s) + k*UX(1,s));

  if (s == -1)
    return (U(r,0) - k*UY(r,0));

  if (s == N+1)
    return (U(r,N) + k*UY(r,1));

  return (f(r,s));		/* Boundary Points */
}

/*****************************************************************************/
/* UX(r,s) - Neumann Boundary Conditions of gx(x,y).  Needs updating $.      */
/*****************************************************************************/
double UX(r,s)
int r,s;
{
  double x, y;

  x = r*k;
  y = s*k;
  return (y * cos(x*y));
}

/*****************************************************************************/
/* UY(r,s) - Neumann Boundary Conditions on gy(x,y).  Needs updating $.      */
/*****************************************************************************/
double UY(r,s)
int r,s;
{
  double x, y;

  x = r*k;
  y = s*k;
  return (x * cos(x*y));
}

/*****************************************************************************/
/* f(r,s) - Function f(x,y).  Needs updatng $.                               */
/*****************************************************************************/
double f(r,s)
int r,s;
{
  double x, y;

  x = r*k;
  y = s*k;
  return (sin(x*y));
}
/*****************************************************************************/



main()
{
  int i, j, kk;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Biharmonic Equation using Gauss-Jordan Method");
  printf2(" - Algorithm 12.9A.\n\n");

  /* Dynamically allocate memory for the needed arrays. */
  A  = dmatrix(1,n,1,n);	/* Matrix A.            */
  BB = dmatrix(1,n,1,1);	/* Matrix BB.  A*U = BB */
  X  = dmatrix(0,m+1,0,m+1);	/* Solution Matrix U.   */

  for (kk=0;kk<m;kk++)		/* Load Matrix A. */
    for (i=1;i<=m;i++)
      for (j=1;j<=m;j++) {
	A[kk*m+i][kk*m+j] = D[i-1][j-1];	/* store D. */
	if (kk < m-1) {				/* Store B. */
          A[(kk+1)*m+i][kk*m+j] = B[i-1][j-1];
          A[kk*m+i][(kk+1)*m+j] = B[i-1][j-1];
        }
	if (kk < m-2) {				/* Store C. */
          A[(kk+2)*m+i][kk*m+j] = C[i-1][j-1];
          A[kk*m+i][(kk+2)*m+j] = C[i-1][j-1];
        }
      }

  printf2("A =\n");		 /* Print matrix A */
  for (i=1;i<=n;i++) {
    for (j=1;j<n;j++)
      printf2("%2.0lf ", A[i][j]);
    printf2("\n");
  }
  printf2("\n");

  loadBB(BB);			 /* Compute and load matrix B (=BB). */

  printf2("B =\n");		 /* Print matrix B */
  for (j=0;j<5;j++) {
    for (kk=1;kk<=5;kk++)
      printf2("% lf  ", BB[5*j+kk][1]);
    printf2("\n");
  }

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  gaussj(A,n,BB,1);		/* Solve A*U=B (=BB) */

  /* STEP #2 */
  for (j=0;j<=m+1;j++)		/* Load solution Vector X = U. */
    for (i=0;i<=m+1;i++)
      if ((j == 0) || (j == m+1) || (i == 0) || (i == m+1))
	X[i][j] = f(i,j);
      else
	X[i][j] = BB[(j-1)*m+i][1];

  /***********
   * OUTPUTS *
   ***********/

  if (ASCII_FLAG == TRUE)
    print_ascii();		/* To print the solution in ASCII.  */

  if (BINARY_FLAG == TRUE)
    print_binary();		/* To print the solution in binary. */

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dmatrix(X,0,m+1,0,m+1);
  free_dmatrix(BB,1,n,1,1);
  free_dmatrix(A,1,n,1,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}

/*****************************************************************************/
/* loadBB() - Computes initial elements in the array.  Needs updating $.     */
/*****************************************************************************/
loadBB(BB)
double **BB;
{
  double z;

  z = pow(k,4.0);

  BB[1][1]  = z*f(1,1) - (U(1,-1) + 2.0*U(0,0) - 8.0*U(1,0) + 2.0*U(2,0)
		+ U(-1,1) -8.0*U(0,1) + 2.0*U(0,2));
  BB[2][1]  = z*f(2,1) - (U(2,-1) + 2.0*U(1,0) - 8.0*U(2,0) + 2.0*U(3,0)
		+ U(0,1));
  BB[3][1]  = z*f(3,1) - (U(3,-1) + 2.0*U(2,0) - 8.0*U(3,0) + 2.0*U(4,0));
  BB[4][1]  = z*f(4,1) - (U(4,-1) + 2.0*U(3,0) - 8.0*U(4,0) + 2.0*U(5,0));
  BB[5][1]  = z*f(5,1) - (U(5,-1) + 2.0*U(4,0) - 8.0*U(5,0) + 2.0*U(6,0));
  BB[6][1]  = z*f(1,2) - (U(1,0) + 2.0*U(0,1) + U(-1,2) - 8.0*U(0,2)
		+ 2.0*U(0,3));
  BB[7][1]  = z*f(2,2) - (U(2,0) + U(0,2));
  BB[8][1]  = z*f(3,2) - (U(3,0));
  BB[9][1]  = z*f(4,2) - (U(4,0) + U(6,2));
  BB[10][1] = z*f(5,2) - (U(5,0) + 2.0*U(6,1) - 8.0*U(6,2) + U(7,2)
		+ 2.0*U(6,3));
  BB[11][1] = z*f(1,3) - (2.0*U(0,2) + U(-1,3) - 8.0*U(0,3) + 2.0*U(0,4));
  BB[12][1] = z*f(2,3) - (U(0,3));
  BB[13][1] = z*f(3,3) - (0.0);
  BB[14][1] = z*f(4,3) - (U(6,3));
  BB[15][1] = z*f(5,3) - (2.0*U(6,2) - 8.0*U(6,3) + U(7,3) + 2.0*U(6,4));
  BB[16][1] = z*f(1,4) - (2.0*U(0,3) + U(-1,4) - 8.0*U(0,4)+ 2.0*U(0,5)
		+ U(1,6));
  BB[17][1] = z*f(2,4) - (U(0,4) + U(2,6));
  BB[18][1] = z*f(3,4) - (U(3,6));
  BB[19][1] = z*f(4,4) - (U(4,6) + U(6,4));
  BB[20][1] = z*f(5,4) - (U(5,6) + 2.0*U(6,3) - 8.0*U(6,4) + U(7,4)
		+ 2.0*U(6,5));
  BB[21][1] = z*f(1,5) - (2.0*U(0,4) + U(-1,5) - 8.0*U(0,5) + 2.0*U(0,6)
		- 8.0*U(1,6) + 2.0*U(2,6) + U(1,7));
  BB[22][1] = z*f(2,5) - (2.0*U(0,5) + 2.0*U(1,6) - 8.0*U(2,6) + 2.0*U(3,6)
		+ U(2,7));
  BB[23][1] = z*f(3,5) - (2.0*U(2,6) - 8.0*U(3,6) + 2.0*U(4,6) + U(3,7));
  BB[24][1] = z*f(4,5) - (U(6,5) + 2.0*U(3,6) - 8.0*U(4,6) + 2.0*U(5,6)
		+ U(4,7));
  BB[25][1] = z*f(5,5) - (2.0*U(6,4) - 8.0*U(6,5) + U(7,5) + 2.0*U(4,6)
		- 8.0*U(5,6) + 2.0*U(6,6) + U(5,7));
}

/*****************************************************************************/
/* print_ascii() - Prints the solution in ascii.                             */
/*****************************************************************************/
print_ascii()
{
  int i, j;

  for (j=0;j<=m+1;j++) {	/* Print solution Vector U. */
    printf2("\n");
    for (i=0;i<=m+1;i++)
      printf2("U(%d,%d) =% .17lf\n", i, j, X[i][j]);
  }
}

/*****************************************************************************/
/* print_binary() - Prints the solution in binary to binary_file.            */
/*****************************************************************************/
print_binary()
{
  int i, j;
  FILE *stream;
  double XX[NUMPTS];		/* Solution Vector U. */

  stream = fopen(binary_file, "w+b");

  for (j=0;j<=m+1;j++)
    for (i=0;i<=m+1;i++)
      XX[j*(m + 2) + i]= X[i][j];

  /* Make a binary file to plot with. */
  fwrite((char *) XX,sizeof(double),NUMPTS,stream);
  fclose(stream);
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
