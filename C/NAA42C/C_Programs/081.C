/******************************************************************************
		  Fast Fourier Transformation - Algorithm 8.1
*******************************************************************************

To compute the discrete approximation

	F(x) = (1/m) * ä c(k)*e(ikx)                 for k = 0,..., 2m-1,
	F(x) = (1/m) * ä c(k)*(cos(kx) + i*sin(kx))  for k = 0,..., 2m-1,

where i = û(-1), for the data {(x(j), y(j))} for j = 0,..., 2m-1 where
m = 2^p and x(j) = -ã + j*ã/m for j = 0,..., 2m-1:

++++++++++++++++++++++++++++ COMPILING HINTS +++++++++++++++++++++++++++++++++
To compile this program you may need more far heap space.  On an IBM PC
using Microsoft C 5.0, using QuickC will limit the far heap space causing an
error (ERROR: C1060).  Use CL without the "/qc" option.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

INPUT m, (p is determined by m); y(0), y(1),..., y(2m-1); interval a,b.

OUTPUT complex numbers c(0),..., c(2m-1); real numbers a(0),..., a(m);
b(1),..., b(m-1).  Note: the a(j) and b(j) coefficients are used for a
trigometric interpolatory polynomial S(n) on the interval [a,b].

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

/*
**  Set the EQ_EVAL flag to TRUE in "naautil.c" to use the Equation Evaluator.
*/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities.  */
#include "naautil2.c"		/* Numerical Analysis Algorithms Util. II.   */
#include "naautil3.c"		/* Numerical Analysis Algorithms Util. III.  */

char *outfile   = "081.out";	/* Customized default output file name.      */
char *eq_text_f = "f(x) = e^(-x)";	/* Needs updating  $  */


/*****************************************************************************/
/* f(x) - Function to evaluate, f(x).  May not be needed.  Needs updating $. */
/*****************************************************************************/
double f(x)
double x;
{
  if (eqeval)
    return (eval_eq(x));	/* Use the Equation Evaluator  */
  else
    return (exp(-x));		/* Use the default function.   */
}
/*****************************************************************************/


main()
{
  double *A, *B, *x, a, b, temp2, f();
  fcomplex *C, *Y, Eta, *Xi, temp, *C_carat;
  long i, j, m, p, pp, q, K, K2, L, M;
  int ch, *k;

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Fast Fourier Transformation - Algorithm 8.1\n\n");

  do {
    printf("Enter the degree of the polynomial, m = 2^p: ");
    scanf("%ld", &m);
    if (m <= 0)			/* ERROR - n must be positive. */
      printf("ERROR - m must be greater than zero.");
  } while (m <= 0);

  temp2 = log((double)(m)) / log(2.0);
  if (ceil(temp2) != floor(temp2))	/* ERROR - m must be a power of 2. */
    naaerror("m is not a power of 2.");

  p = (long) temp2;
  printf2("m = 2^p = %ld, where p = %ld.\n\n", m, p);

  /* Dynamically allocate memory for the needed arrays. */
  A       = dvector(0,m);	/* Real A coefficients          */
  B       = dvector(1,m-1);	/* Real B coefficients          */
  x       = dvector(0,2*m-1);	/* Real x coefficients          */
  C       = cvector(0,2*m-1);	/* Complex C coefficients       */
  C_carat = cvector(0,2*m-1);	/* Complex C_carat coefficients */
  Y       = cvector(0,2*m-1);	/* Complex Y coefficients       */
  Xi      = cvector(0,2*m-1);	/* Complex Xi coefficients      */
  k       = ivector(0,2*m-1);	/* For decomposing K            */

  printf("Is the desired interval [-ã, ã]? (Y/N) N: ");
  getchar();			/* Cleans up after scanf(). */
  ch = tolower(getchar());

  if (ch == 'y') {
    printf2("Using [-ã, ã] for the interval.\n");
    for (j=0;j<2*m;j++)
      x[j] = -PI + j*(PI/m);

  } else {

    printf("Enter start of interval, a: ");
    scanf("%lf", &a);
    printf("Enter end of interval, b: ");
    scanf("%lf", &b);

    printf2("Using [%lG, %lG] for the interval.\n\n", a, b);
    printf2("Transforming [%lG, %lG] to [-ã, ã] gives:\n\n", a, b);
    printf2("k\t X[k]\t\t\t Z[k]\n");
    printf2("--------------------------------------------\n");

    for (j=0;j<2*m;j++) {
      /* Calculate x(j) then transforms [a,b] to [-ã,ã]. */
      x[j] = a + j*((b - a)/(2.0*m));
      printf2("%ld\t% .8lf\t--->\t", j, x[j]);
      printf2("% .8lf\n", PI*(x[j] - (a+b)/2.0));
    }
  }

  printf("\nWill you transform a function, f(x)");
  printf(", or tabulated data? (F/D) F: ");
  getchar();			/* Cleans up after scanf(). */
  ch = tolower(getchar());

  if (ch == 'd') {	/* Output Y values (both real and imaginary). */
    printf2("\n\n k\t Y(k)\n");
    printf2("------------------------\n");

    printf("Enter the Y values:\n");
    for (j=0;j<2*m;j++) {
      printf("X[%ld] = % lG\n", j, x[j]);

      printf("Re(Y[%ld]) = ", j);
      scanf("%f", &Y[j].r);

      printf("Im(Y[%ld]) = ", j);
      scanf("%f", &Y[j].i);
      printf("\n");

      fprintf(file_id, "%ld\t% lG %c %lGi\n",
        j, Y[j].r, ((Y[j].i < 0.0) ? '-' : '+'), fabs(Y[j].i));
    }

  } else {
    printf2("\n");
    if (eqeval)
      printf2("f(x) = %s", tmpstr);	/* Print the Equation Evaluator text */
    else
      printf2("%s", eq_text_f);		/* Print the default equation text */
    printf2("\n");

    for (j=0;j<2*m;j++) {	/* Calculates Y values from f(x). */
      Y[j].r = f(x[j]);
      Y[j].i = 0.0;
    }
  }

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  M = m;
  q = p;

/*
**  Zeta.r = cos(PI/m);    Used in step #3.
**  Zeta.i = sin(PI/m);
*/

  /* STEP #2 */
  for (j=0;j<2*m;j++) {
    C[j].r = Y[j].r;
    C[j].i = Y[j].i;
  }

  /* STEP #3 */
  for (j=1;j<=M;j++) {
    Xi[j].r   = cos((PI*j) / (float) m);
    Xi[j].i   = sin((PI*j) / (float) m);
    Xi[j+M].r = -Xi[j].r;
    Xi[j+M].i = -Xi[j].i;
  }

  /* STEP #4 */
  K       = 0L;
  Xi[0].r = 1.0;
  Xi[0].i = 0.0;

  /* STEP #5 */
  for (L=1L;L<=p+1;L++) {	/* Do Steps 6-12. */

    /* STEP #6 */
    while (K < 2*m-1) {		/* Do Steps 7-11. */

      /* STEP #7 */
      for (j=1L;j<=M;j++) {	/* Do Steps 8-10. */

        /* STEP #8 */
	/*
        ** Let K = k(p)*2^p + k(p-1)*2^(p-1) + ... + k(1)*2 + k(0).
	*/

        decompose(K,p,k);	/* Decompose K into k[]. */

	/*
	**  K1 = K / (long) (pow(2.0, (double) q)); 	Never used.
	*/

        K2 = 0L;
        pp = p;
        for (i=q;i<=p;i++)
          K2 += ((long) k[i]) * (1L << pp--);

        /* STEP #9 */
        Eta    = Cmul(C[K+M], Xi[K2]);
        C[K+M] = Csub(C[K], Eta);
        C[K]   = Cadd(C[K], Eta);

        /* STEP #10 */
        K++;			/* k = k + 1. */
      }

      /* STEP #11 */
      K += M;
    }

    /* STEP #12 */
    K = 0L;
    M /= 2L;
    q--;			/* q = q - 1. */
  }

  /* STEP #13 */
  while (K < 2*m-1) {		/* Do Steps 14-16. */

    /* STEP #14 */

    /*
    **  Let K = k(p)*2^p + k(p-1)*2^(p-1) + ... + k(1)*2 + k(0).
    */

    decompose(K,p,k);		/* Decompose K into k[]. */
    j  = 0L;
    pp = p;
    for (i=0L;i<=p;i++)
      j += ((long) k[i]) * (1L << pp--);

    /* STEP #15 */
    if (j > K) {		/* Interchange C[j] and C[k]. */
      temp.r = C[j].r;
      temp.i = C[j].i;
      C[j].r = C[K].r;
      C[j].i = C[K].i;
      C[K].r = temp.r;
      C[K].i = temp.i;
    }

    /* STEP #16 */
    K++;			/* k = k + 1. */
  }

  /* STEP #17 */
  A[0]   = C[0].r / m;
  temp.r = cos(m * PI);
  temp.i = -sin(m * PI);
  temp   = Cmul(temp, C[m]);
  A[m]   = temp.r / m;

  C_carat[0].r = A[0];
  C_carat[0].i = 0.0;
  C_carat[m].r = A[m];
  C_carat[m].i = 0.0;

  /* STEP #18 */
  for (j=1;j<m;j++) {
    temp.r = cos(j*PI);
    temp.i = -sin(j*PI);
    temp   = Cmul(temp, C[j]);
    A[j]   = temp.r / m;
    B[j]   = temp.i / m;

    C_carat[j].r = A[j];
    C_carat[j].i = B[j];
  }

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #19 */
  /* Output complex terms. */
  printf2("\n k\t Re(C)\t\t Im(C)\t\t Re(C_carat)\t Im(C_carat)\n");
  printf2("------------------------------------");
  printf2("----------------------------------\n");

  for (j=0L;j<2*m;j++) {
    printf2(" %ld\t% 9.9G\t% 9.9G", j, C[j].r, C[j].i);
    if (j <= m)
      printf2("\t% 9.9G\t% 9.9G", C_carat[j].r, C_carat[j].i);
    else			/* if (j > m) */
      printf2("\t% 9.9G\t% 9.9G", C_carat[m-(j%m)].r, -C_carat[m-(j%m)].i);
    printf2("\n");
  }

  /* Output polynomial coefficients. */
  printf2("\n\n k\t A = Re(C_carat) B = Im(C_carat)\n");
  printf2("------------------------------------------\n");

  for (j=0L;j<=m;j++) {
    printf2(" %ld\t% 9.9lG", j, A[j]);
    if (j >= 1 && j < m)
      printf2("\t% 9.9lG", B[j]);
    printf2("\n");
  }

  printf2("\nUse the above coefficients in the trigonometric interpolatory\n");
  printf2("polynomial S(x) of degree %ld:\n\n", m);
  printf2("S(x) = A[0] / 2  +  A[%ld] * cos(%ldx)  +  ", m, m);
  printf2("ä (A[k] * cos(kx)  +  B[k] * sin(kx))\n");
  printf2("       summed from k = 1 to %ld.\n", m-1);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_ivector(k,0,2*m-1);
  free_cvector(Xi,0,2*m-1);
  free_cvector(Y,0,2*m-1);
  free_cvector(C_carat,0,2*m-1);
  free_cvector(C,0,2*m-1);
  free_dvector(x,0,2*m-1);
  free_dvector(B,1,m-1);
  free_dvector(A,0,m);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}				/* STOP */

/*****************************************************************************/
/* decompose() - Decomposes K from a binary bit-field into the vector k[].   */
/*****************************************************************************/
decompose(K,max,k)
long K, max;
int *k;
{
  long i, temp;

  for (i=max;i>=0L;i--) {		/* Decompose K from left to right. */
    temp = 1L << i;
    if (K >= temp) {
      K -= temp;
      k[i] = 1;
    } else
      k[i] = 0;
  }
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
