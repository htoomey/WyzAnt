/******************************************************************************
		Determinant of a Matrix - Algorithm 6.0C
*******************************************************************************

Computes the determinant of matrix A:

INPUT matrix dimension n; the entries a(i)(j), 1 ó i,j ó n of the matrix A.

OUTPUT the determinant of A.

NOTE: This algorithm was included as a "Homework Helper."  See p. 352,
Exercise Set 6.5, Problem double-checker.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "060c.out";	/* Customized default output file name.     */


main()
{
  double **A, det;
  int i, j, n, *indx;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Determinant of a Matrix - Algorithm 6.0C\n\n");

  do {
    printf("Enter the dimension, n, of the matrix A: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greater than zero.\n");
  } while(n <= 0);
  fprintf(file_id, "Dimension, n, of the matrix A: %d\n\n", n);

  /* Dynamically allocate memory for the needed arrays. */
  A    = dmatrix(1,n,1,n);
  indx = ivector(1,n);

  printf("Enter the coefficients for matrix A:\n");	/* Get A. */
  for (i=1;i<=n;i++)
    for (j=1;j<=n;j++) {
      printf("\tA[%d][%d] = ", i, j);
      scanf("%lf", &A[i][j]);
    }

  fprintf(file_id, "A = ");	/* Print Matrix A to a file. */
  for (i=1;i<=n;i++) {
    fprintf(file_id, "[ ");
    for (j=1;j<=n;j++)
      fprintf(file_id, "% 3lg ", A[i][j]);
    fprintf(file_id, "]\n    ");
  }

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  if (n == 1)			/* For a 1 x 1 matrix.                  */
    det = A[1][1];

  /* STEP #2 */
  else if (n == 2)		/* For a 2 x 2 matrix.                  */
    det = A[2][2] * A[1][1] - A[2][1] * A[1][2];

  /* STEP #3 */
  else {			/* For an n x n matrix, with n > 2.     */
    ludcmp(A,n,indx,&det);	/* This makes an LU decomposition of A  */
    for (j=1;j<=n;j++)		/* and returns det as ñ1.               */
      det *= A[j][j];
  }

  /***********
   * OUTPUTS *
   ***********/

  printf2("\nDeterminant of A = %lg\n", det);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_ivector(indx,1,n);
  free_dmatrix(A,1,n,1,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* ldcmp() - Performes an LU decomposition of an n x n matrix.               */
/*****************************************************************************/
#define TINY	1.0e-20
ludcmp(a,n,indx,d)	/* From "Numerical Recipes in C" */
int n, *indx;
double **a, *d;

/*
** Given an n x n matrix a[1..n][1..n], this routine replaces it by the LU
** decomposition of a rowwise permutation of itself.  a and n are input.  a is
** output, arranged in the equation below:
** 			[ á11 á12 á13 á14 ]
** 			[ à21 á22 á23 á24 ]	(equation 2.3.14)
** 			[ à31 à32 á33 á34 ]
** 			[ à41 à42 à43 á44 ]
** indx[1..n] is an output vector which records the row permutation effected by
** the partial pivoting; d id output as ñ1 depending on whether the number of
** row interchanges was even or odd, respectively.  This routine can be is
** used in combination with lubksb to solve linear equations or invert a
** matrix.
*/

{
  int i, imax, j, k;
  double big, dum, sum, temp;
  double *vv, *dvector();     /* vv stores the implicit scaling of each row. */
  void naaerror(), free_dvector();

  vv = dvector(1,n);
  *d = 1.0;

  for (i=1;i<=n;i++) {	/* Loop over rows to get implicit scaling info. */
    big = 0.0;
    for (j=1;j<=n;j++)
      if ((temp=fabs(a[i][j])) > big)
        big = temp;
    if (big == 0.0)
      naaerror("Singular matrix in routine LUDCMP");

    /* No nonzero largest element. */
    vv[i] = 1.0/big;		/* Save the scaling. */
  }

  for (j=1;j<=n;j++) {	/* This is the loop over columns of Crout's method. */
    for (i=1;i<j;i++) {	/* This is equation 2.3.12 except for i = j.        */
      sum = a[i][j];
      for (k=1;k<i;k++)
        sum -= a[i][k]*a[k][j];
      a[i][j] = sum;
    }

    big = 0.0;	/* Initialize for the search for largest pivot element.   */
    for (i=j;i<=n;i++) {	/* This is i = j of equation 2.3.12 and i */
      sum = a[i][j];		/* ... = j+1,...,N of equation 2.3.12.    */
      for (k=1;k<j;k++)
        sum -= a[i][k]*a[k][j];
      a[i][j] = sum;
      if ( (dum=vv[i]*fabs(sum)) >= big) {	/* Is the figure of merit */
        big = dum;		/* ... for the pivot better than the best */
        imax = i;		/* ... so far?                            */
      }
    }

    if (j != imax) {		/* Do we need to interchange rows?    */
      for (k=1;k<=n;k++) {	/* Yes, do so...                      */
        dum = a[imax][k];
        a[imax][k] = a[j][k];
        a[j][k] = dum;
      }
      *d = -(*d);		/* ... and change the parity of d.    */
      vv[imax] = vv[j];		/* Also interchange the scale factor. */
    }

    indx[j] = imax;
    if (a[j][j] == 0.0)  a[j][j] = TINY;
    if (j != n) {	/* Now, finally, divide by the pivot element. */
      dum = 1.0/(a[j][j]);
      /*
      ** If the pivot element is zero the matrix is singular (at least to
      ** the precision of the algorithm).  For some applications on
      ** singular matrices, it is desirable to substitute TINY for zero.
      */
      for (i=j+1;i<=n;i++)
        a[i][j] *= dum;
    }
  }			/* Go back for the next column in the reduction. */
  free_dvector(vv,1,n);
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
