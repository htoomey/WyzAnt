/******************************************************************************
			QL Algorithm - Algorithm 9.6B
*******************************************************************************

To obtain the eigenvalues of the symmetric, tridiagonal n x n matrix

	       [ [ a(1) b(2)   0    0    0    0   ]
		 [ b(2) a(2)   .    0    0    0   ]
		 [  0     .    .    .    0    0   ]
	A ð A1 ð [  0     0    .    .    .    0   ]
		 [  0     0    0    .    .   b(n) ]
		 [  0     0    0    0  b(n)  a(n) ] ]

Note: This algorithm is from the third edition of the text.

INPUT dimension n; diagonal elements a(1),..., a(n); off-diagonal elements
b(2),..., b(n); maximum number of iterations M.

OUTPUT eigenvalues of A or recommended splitting of A, or a message that the
maximum number of iterations was exceeded.

NOTE: Discontinued in 4th edition of the text.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "096b.out";	/* Customized default output file name.     */
int n;				/* Number of equations and unknowns.        */


main()
{
  double *A, *B, **AA, b, c, dd, f, g, p, r, s, sign;
  int i, j, l, m, iter, M;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("QL Algorithm - Algorithm 9.6B\n\n");

  printf("Enter maximum number of iterations, M: ");
  scanf("%d", &M);
  fprintf(file_id, "Maximum number of iterations: %d\n", M);

  do {
    printf("Enter the dimension, n, of the matrix A: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greter than zero.\n");
  } while (n <= 0);
  fprintf(file_id, "System has %d equations and %d unknowns.\n\n", n, n);

  /* Dynamically allocate memory for the needed arrays. */
  AA = dmatrix(1,n,1,n);	/* Whole Matrix with A and B */
  A  = dvector(1,n);		/* Diagonal Vector A         */
  B  = dvector(1,n);		/* Off-diagonal Vector B     */

  printf("\t       [ [ a(1) b(2)   0    0    0    0   ]\n");
  printf("\t         [ b(2) a(2)   .    0    0    0   ]\n");
  printf("\t         [  0     .    .    .    0    0   ]\n");
  printf("\t     A ð [  0     0    .    .    .    0   ]\n");
  printf("\t         [  0     0    0    .    .   b(n) ]\n");
  printf("\t         [  0     0    0    0  b(n)  a(n) ] ]\n\n");

  printf("Enter the coefficients for Diagonal Vector A:\n");
  for (i=1;i<=n;i++) {				/* Get A. */
    printf("\tA[%d] = ", i);
    scanf("%lf", &A[i]);
  }

  fprintf(file_id, "The coefficients for Diagonal Vector A:\n");
  fprintf(file_id, "A = [ ");			/* Print A. */
  for (i=1;i<=n;i++)
    fprintf(file_id, "% 3lg ", A[i]);
  fprintf(file_id, "]t\n\n");

  printf("Enter the coefficients for Off-diagonal Vector B:\n");
  for (i=2;i<=n;i++) {				/* Get B. */
    printf("\tB[%d] = ", i);
    scanf("%lf", &B[i]);
  }

  fprintf(file_id, "The coefficients for Off-diagonal Vector B:\n");
  fprintf(file_id, "B = [ ");			/* Print B. */
  for (i=2;i<=n;i++)
    fprintf(file_id, "% 3lg ", B[i]);
  fprintf(file_id, "]t\n\n");

  for (i=1;i<=n;i++)		/* Take A and B to make n x n AA. */
    for (j=1;j<=n;j++) {
      if (i == j)
        AA[i][j] = A[i];	/* Diagonal.     */
      else
        AA[i][j] = 0.0;		/* Other.        */
    }
  for (i=1;i<n;i++) {
    AA[i+1][i] = B[i+1];	/* Off diagonal. */
    AA[i][i+1] = B[i+1];	/* Off diagonal. */
  }

  printf2("MATRIX = ");		/* Print the n x n matrix. */
  for (i=1;i<=n;i++) {
    printf2("[ ");
    for (j=1;j<=n;j++)
      printf2("% .9lf  ", AA[i][j]);
    printf2("]\n         ");
  }
  printf2("\n");

  /*************
   * ALGORITHM *
   *************/

  /* STEPS #1 to #8 came from "Numerical Recipes in C." */
  for (i=2;i<=n;i++)
    B[i-1] = B[i];
  B[n]=0.0;
  for (l=1;l<=n;l++) {
    iter=0;
    do {
      for (m=l;m<=n-1;m++) {
	dd = fabs(A[m]) + fabs(A[m+1]);
	if (fabs(B[m])+dd ==dd)
          break;
      }
      if (m != l) {
	if (iter++ == M) {	/* STEP #9 */
          printf2("Maximum number of iterations (%d) exceeded.\n", M);
          NAA_do_last(outfile);	/* NAA finish-up procedure. */
          return (-1);		/* EXIT */
        }
	g = (A[l+1]-A[l]) / (2.0*B[l]);
	r = sqrt((g*g) + 1.0);

        if (g < 0.0)
          sign = -fabs(r);
        else
          sign = fabs(r);

	g = A[m]-A[l]+B[l] / (g+sign);
	c = 1.0;
	s = c;
	p = 0.0;
	for (i=m-1;i>=l;i--) {
	  f = s * B[i];
	  b = c * B[i];
	  if (fabs(f) >= fabs(g)) {
	    c = g / f;
	    r = sqrt((c*c) + 1.0);
	    B[i+1] = f * r;
	    c *= (s = 1.0/r);
	  } else {
	    s = f / g;
	    r = sqrt((s*s) + 1.0);
	    B[i+1] = g * r;
	    s *= (c = 1.0/r);
	  }
	  g = A[i+1] - p;
	  r = (A[i]-g) * s+2.0 * c * b;
	  p = s * r;
	  A[i+1] = g + p;
	  g = c * r - b;
	}
	A[l] = A[l] - p;
	B[l] = g;
	B[m] = 0.0;
      }
    } while (m != l);
  }

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #9 */
  printf2("Eigenvalues are:\n");
  for (i=1;i<=n;i++)
    printf2("\t% .11lf\n", A[i]);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(B,1,n);
  free_dvector(A,1,n);
  free_dmatrix(AA,1,n,1,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
