/******************************************************************************
		Richardson's Extrapolation - Algorithm 4.0D1
*******************************************************************************

To use extrapolation to obtain more accurate results, with less functional
evaluations, on a given data set.  Uses Equation 4.29.

INPUT the number of terms to be used in the extrapolation, n; interval h;
data points N(h), N(h/2), N(h/4),...,N(h/(2^(n-1))):

OUTPUT A more accurate result as compared to only one approximation, N1(h).

NOTE: This algorithm was included as a "Homework Helper."  See p. 162,
Exercise Set 4.2, Problems 1-2, 5-6.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */

char *outfile = "040d1.out";	/* Customized default output file name.     */


main()
{
  double **N, tmp;
  int i, j, k, n;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Richardson's Extrapolation - Algorithm 4.0D1\n\n");

  do {
    printf("Enter number of terms to be extrapolated, n: ");
    scanf("%d", &n);
    if (n <= 0)
      printf("ERROR - n must be greater than zero.\n");
  } while (n <= 0);
  fprintf(file_id, "Extrapolating %d terms.\n\n", n);

  /* Dynamically allocate memory for the needed arrays. */
  N = dmatrix(1,n,1,n);

  j = 1;
  for (k=1;k<=n;k++) {		/* Read in values to Extrapolate. */
    if (k==1)
      printf("Enter N(h): ");
    else {
      i = (int) pow(2.0, (double) (k-1));
      printf("Enter N(h/%d): ", i);
    }
    scanf("%lf", &N[j][k]);

    if (k == 1)
      fprintf(file_id, "N%d(h) = %lf\n", j, N[j][k]);
    else
      fprintf(file_id, "N%d(h/%d) = %lf\n", j, i, N[j][k]);
  }

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  for (j=2;j<=n;j++) {		/* Do the Extrapolation. */
    printf2("\n");
    for (k=1;k<=(n-j+1);k++) {
      tmp     = pow(4.0, (double) (j - 1));
      N[j][k] = (tmp*N[j-1][k+1] - N[j-1][k]) / (tmp - 1.0);  /* THE Formula */

      /***********
       * OUTPUTS *
       ***********/

      if (k == 1)
        printf2("N%d(h) = %lf\n", j, N[j][k]);
      else
        printf2("N%d(h/%d) = %lf\n", j, k, N[j][k]);
    }
  }

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dmatrix(N,1,n,1,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
