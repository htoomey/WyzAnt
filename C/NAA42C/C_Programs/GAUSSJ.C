/********************************* GAUSSJ.C ***********************************
			Gauss-Jordan Matrix solver
******************************************************************************/

/*
**  This include file contains the routine gaussj().  This routine solves for a
**  system of linear equations by Gauss-Jordan elimination for Ax = B.
**  Gaussj() originated from the book "Numerical Recipes in C."
**
**  Requires "#include <math.h>" which is usually included when using
**  "naautil.h".
*/

/***********************/
/* FUNCTION PROTOTYPES */
/***********************/

#if ANSI == TRUE
  void gaussj(double **a, int n, double **b, int m);
#else
  void gaussj();
#endif


/**********/
/* MACROS */
/**********/

#define	SWAP(a,b)	{double temp = (a); (a) = (b); (b) = temp;}


/***************
 * SUBROUTINES *
 ***************/

/*****************************************************************************/
/* gaussj() - Linear equation solution by Gauss-Jordan elimination for Ax=B. */
/*            a[1..n][1..n] is an input matrix of n by n elements.           */
/*            b[1..n][1..m] is an input matrix of size n by m containing the */
/*            m right-hand side vectors.  On out, a is replaced by its       */
/*            matrix inverse, and b is replaced by the corresponding set of  */
/*            solution vectors.                                              */
/*                                                                           */
/* NOTE:  This function can be found in the book "Numerical Recipes in C".  */
/*****************************************************************************/
void gaussj(a,n,b,m)
double **a, **b;
int n, m;
{
  int *indxc, *indxr, *ipiv;

/*
**  The integer arrays ipiv[1..n], indxr[1..n], indxc[1..n] are used
**  for bookkeeping on the pivoting.
*/

  int i, icol, irow, j, k, l, ll, *ivector();
  double big, dum, pivinv;
  void naaerror(), free_ivector();

  indxc=ivector(1,n);
  indxr=ivector(1,n);
  ipiv=ivector(1,n);
  for (j=1; j<=n; j++)  ipiv[j] = 0;
  for (i=1; i<=n; i++) {	/* This is the main loop over the columns */
    big = 0.0;			/* ... to be reduced                      */
    for (j=1; j<=n; j++)	/* This is the outer loop of the search   */
      if (ipiv[j] != 1)		/* ... for a pivot element                */
	for (k=1; k<=n; k++) {
	  if (ipiv[k] == 0) {
	    if (fabs(a[j][k]) >= big) {
	      big = fabs(a[j][k]);
	      irow = j;
	      icol = k;
	    }
	  } else if (ipiv[k] > 1)  naaerror("GAUSSJ: Singular Matrix-1");
	}
  ++(ipiv[icol]);

/*
**  We now have the pivot elements. so we interchange rows, if needed, to put
**  the pivot element on the diagonal.  The columns are not physically inter-
**  changed, only relabeled: indx[i], the column of the ith pivot element, is
**  the ith column that is reduced, while indxr[i] is the row in which that 
**  pivot element was originally located.  If indxr != indxc[i] there is an
**  implied column interchange.  With this form of bookkeeping, the solution
**  b's will end up in the correct order, and the inverse matrix will be
**  scrambled by columns.
*/

  if (irow != icol) {
    for (l=1; l<=n; l++)  SWAP(a[irow][l], a[icol][l])
    for (l=1; l<=m; l++)  SWAP(b[irow][l], b[icol][l])
  }

  indxr[i] = irow;		/* We are now ready to divide the pivot row  */
  indxc[i] = icol;		/* ... by the pivot element, located at irow */
  if (a[icol][icol] == 0.0)	/* ... and icol.                             */
    naaerror("GAUSSJ: Singular Matrix-2");
  pivinv = 1.0 / a[icol][icol];
  a[icol][icol] = 1.0;
  for (l=1;l<=n; l++)  a[icol][l] *= pivinv;
  for (l=1;l<=m; l++)  b[icol][l] *= pivinv;
  for (ll=1; ll<=n; ll++)	/* Next, we reduce the rows...             */
    if (ll != icol) {		/* ... except for the pivot one, of course */
      dum = a[ll][icol];
      a[ll][icol] = 0.0;
      for (l=1; l<=n; l++)  a[ll][l] -= a[icol][l] * dum;
      for (l=1; l<=m; l++)  b[ll][l] -= b[icol][l] * dum;
    }
  }

/*
**  This is the end of the main loop over columns of the reduction.  It only
**  remains to unscramble the solution in view of the column interchanges.
**  We do this by interchanging pairs of columns in the reverse order that the
**  permutation was built up.
*/

  for (l=n; l>=1; l--) {
    if (indxr[l] != indxc[l])
      for (k=1; k<=n; k++)
	SWAP(a[k][indxr[l]], a[k][indxc[l]])
  }
  free_ivector(ipiv,1,n);	/* And we are done. */
  free_ivector(indxr,1,n);
  free_ivector(indxc,1,n);
}

/******************************************************************************
*	Used by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2         *
******************************************************************************/
