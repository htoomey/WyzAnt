/******************************************************************************
		Richardson's Extrapolation - Algorithm 4.0D2
				(with rounding)
*******************************************************************************

To use extrapolation to obtain more accurate results, with less functional
evaluations, on a given data set.

INPUT the number of terms to be used in the extrapolation, n; interval h;
data points N(h), N(h/2), N(h/4),...,N(h/(2^(n-1))); number of significant
digits to round to, SIG:

OUTPUT A more accurate result as compared to only one approximation, N1(h).

NOTE: This algorithm was included as a "Homework Helper."  See p. 162,
Exercise Set 4.2, Problems 3-4.  For chopping arithmetic, replace EVERY
occurance of the word "round" with "trunc", including the "round.c" file.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities. */
#include "round.c"		/* Rounds X to SIG significant digits.      */

char *outfile = "040d2.out";	/* Customized default output file name.     */


main()
{
  double **N, tmp;
  int i, j, k, n, SIG;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Richardson's Extrapolation (with rounding) - Algorithm 4.0D2\n\n");

  do {
    printf("Enter the number of significant digits, SIG (1-%d): ", DBL_DIG);
    scanf("%d", &SIG);
    if (SIG < 1 || SIG > DBL_DIG)	/* ERROR */
      printf("Enter 1 to %d only for number of significant digits.\n",
        DBL_DIG);
  } while (SIG < 1 || SIG > DBL_DIG);
  fprintf(file_id, "Computed with %d-digit rounding arithmetic.\n", SIG);

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
    else
      printf("Enter N(h/%d): ", (i = (int) pow(2.0, (double) (k-1))));

    scanf("%lf", &N[j][k]);
    N[j][k] = round(N[j][k], SIG);	/* Rounds all values. */

    if (k == 1)
      fprintf(file_id, "N%d(h) = %*lG\n", j, SIG, N[j][k]);
    else
      fprintf(file_id, "N%d(h/%d) = %*lG\n", j, i, SIG, N[j][k]);
  }

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  for (j=2;j<=n;j++) {		/* Do the Extrapolation. */
    printf2("\n");
    for (k=1;k<=(n-j+1);k++) {
      tmp = round(pow(4.0, (double) (j-1)), SIG);
				/* THE Formula */
      N[j][k] = round(tmp*N[j-1][k+1] - N[j-1][k], SIG)
		/ round(tmp - 1.0, SIG);
      N[j][k] = round(N[j][k], SIG);	/* Rounds all values. */

      /***********
       * OUTPUTS *
       ***********/

      if (k == 1)
        printf2("N%d(h) = %*lG\n", j, SIG, N[j][k]);
      else
        printf2("N%d(h/%d) = %*lG\n", j, k, SIG, N[j][k]);
    }
  }

  /* Free the memory that was dynamically allocated for the arrays. */
  free_dmatrix(N,1,n,1,n);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
