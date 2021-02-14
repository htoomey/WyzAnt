/******************************************************************************
	     LDLt Factorization which solves AX=B - Algorithm 6.5B
*******************************************************************************

To factor the positive definite n x n matrix A into the form LDLt, where L is
a lower triangular matrix with ones along the diagonal and D is a diagonal
matrix with positive entries on the diagonal:

INPUT dimension n; entries (a(i,j)) 1 ó i,j ó n.

OUTPUT the entries l(i,j), 1 ó j ó i, 1 ó i ó n of L and d(i), 1 ó i ó n
of D.

NOTE: This algorithm was included as a "Homework Helper."  See p. 379,
Exercise Set 6.7, Problem *** 12 ***.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "065b.out";	/* Customized default output file name.     */


main()
{
  double **A, *B, *D, **L, *V, *X, *Y, *Z, **TMP, sum;
  int i, j, k, n;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("LDLt Factorization which solves AX = B - Algorithm 6.5B\n\n");

  do {
    printf("Enter the dimension, n, of the matrix A: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greter than zero.\n");
  } while (n <= 0);

  /* Dynamically allocate memory for the needed arrays. */
  TMP = dmatrix(1,n,1,n);	/* A temporary matrix to save on space. */
  A = dmatrix(1,n,1,n);		/* Matrix A. */
  L = dmatrix(1,n,1,n);		/* Matrix L. */
  D = dvector(1,n);		/* Vector D. */
  V = dvector(1,n);		/* Vector V. */
  B = dvector(1,n);		/* Vector B. */
  X = V;			/* Vector X.  Aliased to save memory. */
  Y = dvector(1,n);		/* Vector Y. */
  Z = Y;			/* Vector Z.  Alaised to save memory. */

  printf("Enter the coefficients for matrix A:\n");	/* Get A. */
  for (i=1;i<=n;i++)
    for (j=1;j<=n;j++) {
      printf("\tA[%d][%d] = ", i, j);
      scanf("%lf", &A[i][j]);
    }

  fprintf(file_id, "A = ");	/* Print Matrix A to file. */
  for (i=1;i<=n;i++) {
    fprintf(file_id, "[ ");
    for (j=1;j<=n;j++)
      fprintf(file_id, "% 9.9lg ", A[i][j]);
    fprintf(file_id, "]\n    ");
  }
  fprintf(file_id, "\n");

  /*
  **  Remove the below comments to factor a symmetric matrix.
  */

/*
**   for (i=1;i<=n;i++)
**     if (A[i][i] == 0.0)
**       naaerror("Can't be done.  There is a zero diagonal element.");
*/

  printf("Enter the coefficients for vector B:\n");	/* Get B. */
  for (i=1;i<=n;i++) {
    printf("\tB[%d] = ", i);
    scanf("%lf", &B[i]);
  }
  printf("\n");

  fprintf(file_id, "B = [ ");	/* Print Vector B to file. */
  for (i=1;i<=n;i++)
    fprintf(file_id, "% 9.9lg ", B[i]);
  fprintf(file_id, "]\n\n");

  for (i=1;i<=n;i++)		/* Put 1's on diagonal of L. */
    L[i][i] = 1;		/* Should be in text!        */

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  for (i=1;i<=n;i++) {		/* Do Steps 2-4. */

    /* STEP #2 */
    for (j=1;j<i;j++)
      V[j] = L[i][j] * D[j];	/* The algorithm in the text is incorrect. */
				/* Replace A[i][j] with L[i][j].           */
    /* STEP #3 */
    sum = 0.0;
    for (j=1;j<i;j++)		/* The algorithm in the text is incorrect. */
      sum += L[i][j] * V[j];	/* Replace A[i][j] with L[i][j].           */

    D[i] = A[i][i] - sum;

    /* STEP #4 */
    for (j=i+1;j<=n;j++) {
      sum = 0.0;
      for (k=1;k<i;k++)		/* The algorithm in the text is incorrect. */
        sum += L[j][k] * V[k];	/* Replace A[j][k] with L[j][k].           */
      L[j][i] = (A[j][i] - sum) / D[i];
    }
  }

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #5 */
  printf2("L  = ");		/* Print out matrix L.                    */
  print_matrix(L,n);

  for (i=1;i<=n;i++)		/* Put vector D into matrix TMP, still 0. */
    TMP[i][i] = D[i];

  printf2("D  = ");		/* Print out matrix D.                    */
  print_matrix(TMP,n);

  for (i=1;i<=n;i++)		/* Make Lt, storing it in TMP.            */
    for (j=1;j<=n;j++)
      TMP[i][j] = L[j][i];

  printf2("Lt = ");		/* Print out matrix L Transpose.          */
  print_matrix(TMP,n);

  /*
  **  Steps 6-11 are additions needed to solve AX = B (also prompt for B).
  */

  /* STEP #6 */
  Y[1] = B[1];			/* Steps 6-7 solves the lower triangular */
				/* system LY = B.                        */
  /* STEP #7 */
  for (i=2;i<=n;i++) {
    sum = 0.0;
    for (j=1;j<i;j++)
      sum += L[i][j] * Y[j];
    Y[i] = B[i] - sum;
  }

  /* STEP #8 */
  for (i=1;i<=n;i++)		/* Solves DZ = Y. */
    Z[i] = Y[i] / D[i];

  /* STEP #9 */
  X[n] = Z[n];			/* Steps 9-11 solves the upper triangular */
				/* system LtX = Z.                        */
  /* STEP #10 */
  for (i=n-1;i>=1;i--) {
    sum = 0.0;
    for (j=i+1;j<=n;j++)
      sum += L[j][i] * X[j];	/* The algorithm in the text is incorrect. */
    X[i] = Z[i] - sum;		/* Change L[i][j] to L[j][i].              */
  }

  /* STEP #11 */
  printf2("X = [ ");		/* Procedure completed successfully.       */
  for (i=1;i<=n;i++)
    printf2("% 9.9lg ", X[i]);
  printf2("]\n");

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dvector(Y,1,n);
  free_dvector(B,1,n);
  free_dvector(V,1,n);
  free_dvector(D,1,n);
  free_dmatrix(L,1,n,1,n);
  free_dmatrix(A,1,n,1,n);
  free_dmatrix(TMP,1,n,1,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* print_matrix() - Prints out a matrix.                                     */
/*****************************************************************************/
print_matrix(A, n)
double **A;
int n;
{
  int i, j;

  for (i=1;i<=n;i++) {
    printf2("[ ");
    for (j=1;j<=n;j++)
      printf2("% 9.9lg ", A[i][j]);
    printf2("]\n     ");
  }
  printf2("\n");

}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
