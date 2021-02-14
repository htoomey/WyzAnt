/******************************************************************************
			Finite Element - Algorithm 12.5
*******************************************************************************

To approximate the solution of the partial-differential equation

	ë(p(x,y)*(ëu/ëx))/ëx + ë(q(x,y)*(ëu/ëy))/ëy  + r(x,y)*u = f(x,y),
						(x,y) î D

subject to the boundary conditions

	u(x,y) = g(x,y),			(x,y) î œ1
and
	p(x,y)*(ëu(x,y)/ëx)*cos(é1) + q(x,y)*(ëu(x,y)/ëx)*cos(é2) +

	g1(x,y)*u(x,y) = g2(x,y),		(x,y) î œ2

where œ1 U œ2 is the boundary of D, and é1 and é2 are the direction angles
of the normal to the boundary:

INPUT integers K, N, M, n, m; vertices (x_1(i), y_1(i)), (x_2(i), y_2(i)),
(x_3(i), y_3(i)) for each i = 1,...,M; nodes E_j for each j = 1,...,m; the
functions p(), q(), r(), f(), g(), g1(), and g2().

(Note: All that is needed is a means of corresponding a vertex (x_k(i), y_k(i))
to a node E_j = (x_j, y_j).)

OUTPUT constants ç_1,...,ç_m; a_j(i), b_j(i), c_j(i) for each j = 1,2,3 and
i = 1,...,M.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

#include "naautil.c"		/* Numerical Analysis Algorithms Utilities.  */
#include "naautil2.c"		/* For dcube().                              */
#include "gaussj.c"		/* Guass-Jordan matrix solver.               */

char *outfile    = "125.out";	/* Customized default output file name.      */
char *eq_text_p  = "p(x,y) = 1.0";		/* Needs updating  $  */
char *eq_text_q  = "q(x,y) = 1.0";		/* Needs updating  $  */
char *eq_text_r  = "r(x,y) = 0.0";		/* Needs updating  $  */
char *eq_text_f  = "f(x,y) = 0.0";		/* Needs updating  $  */
char *eq_text_g  = "g(x,y) = 4.0";		/* Needs updating  $  */
char *eq_text_g1 = "g1(x,y) = sin(PI*x)";	/* Needs updating  $  */
char *eq_text_g2 = "g2(x,y) = sin(PI*x)";	/* Needs updating  $  */


/*****************************************************************************/
/* p(x,y) - Function p(x,y) to evaluate.  Needs updating $.                  */
/*****************************************************************************/
double p(x,y)
double x, y;
{
  return (1.0);
}

/*****************************************************************************/
/* q(x,y) - Function q(x,y) to evaluate.  Needs updating $.                  */
/*****************************************************************************/
double q(x,y)
double x, y;
{
  return (1.0);
}

/*****************************************************************************/
/* r(x,y) - Function r(x,y) to evaluate.  Needs updating $.                  */
/*****************************************************************************/
double r(x,y)
double x, y;
{
  return (0.0);
}

/*****************************************************************************/
/* f(x,y) - Function to evaluate, f(x,y).  Needs updating $.                 */
/*****************************************************************************/
double f(x,y)
double x, y;
{
  return (0.0);
}

/*****************************************************************************/
/* g(x,y) - Boundary condition u(x,y) = g(x,y).  Needs updating $.           */
/*****************************************************************************/
double g(x,y)
double x, y;
{
  return (4.0);
}

/*****************************************************************************/
/* g1(x,y) - Boundary condition g1(x,y) on œ2.  Needs updating $.            */
/*****************************************************************************/
double g1(x,y)
double x, y;
{
  return (sin(PI*x));
}

/*****************************************************************************/
/* g2(x,y) - Boundary condition g2(x,y) on œ2.  Needs updating $.            */
/*****************************************************************************/
double g2(x,y)
double x, y;
{
  return (sin(PI*x));
}
/*****************************************************************************/


main()
{
  double **a, **b, **c, **E, **H, **I, **NN, *x, *y;
  double ***J, ***z;
  double **alpha, **beta, *delta, *gamma;
  double p(), q(), r(), f(), g(), g1(), g2();
  double NNN(), simpsons(), simpsons_2();
  int **T;
  int i, j, k, l, K, M, N, m, n, t;


  /* STEP #0 */

  /*
  **   Divide the region D into triangles T(1),...,T(M) such that:
  ** 
  **   T(1),...,T(K) are the triangles with all vertices interior to D;
  **     (Note: K = 0 implies that no triangle is interior to D.)
  ** 
  **   T(K+1),...,T(N) are the triangles with at least one edge on œ2;
  **   T(N+1),...,T(M) are the remaining triangles.
  **     (Note: M = N implies that all triangles have edges on œ2.)
  ** 
  **   Label the three vertices of the triangle T(i) by (x1(i),y1(i)),
  **     (x2(i),y2(i)), and (x3(i),y3(i)).
  ** 
  **   Label the nodes (vertices) E(1),...,E(m) where
  **     E(1),...,E(n) are in D U œ2 and E(n+1),...,E(m) are on œ1.
  **     (Note: n = m implies that œ1 containes no nodes.)
  **
  */

  /**********
   * INPUTS *
   **********/

  NAA_do_first(outfile);	/* NAA initialization procedure. */

  printf2("Finite Element - Algorithm 12.5\n\n");
  printf2("ë(p(x,y)*(ëu/ëx))/ëx + ë(q(x,y)*(ëu/ëy))/ëy  + r(x,y)*u");
  printf2(" = f(x,y)\n\n");
  printf2("%s\n", eq_text_p);
  printf2("%s\n", eq_text_q);
  printf2("%s\n", eq_text_r);
  printf2("%s\n\n", eq_text_f);
  printf2("subject to the boundary conditions:\n");
  printf2("u(x,y) = %s\t(x,y) î œ1\n", eq_text_g);
  printf2("%s\t\t(x,y) î œ2\n", eq_text_g1);
  printf2("%s\t\t(x,y) î œ2\n\n", eq_text_g2);

  printf("M is the total number of elements (triangles).\n");
  do {
    printf("Enter M: ");
    scanf("%d", &M);
    if (M <= 0)
      printf("ERROR - M must be greater than zero.\n");
  } while (M <= 0);
  fprintf(file_id, "Total number of elements (triangles) M = %d\n", M);

  printf("\nK is the number of elements (triangles) with NO edges on the ");
  printf("boundary.\n");
  do {
    printf("Enter K: ");
    scanf("%d", &K);
    if (K >= M)
      printf("ERROR - K must be less than M\n.");
  } while (K >= M);
  if (K == 0)
    printf("Note: K = 0 implies that no triangle is interior to region D.\n");
  fprintf(file_id, "Number of elements (triangles) with NO edges on the");
  fprintf(file_id, " boundary K = %d\n", K);

  printf("\nN is the number of elements (triangles) with at least one edge\n");
  printf(" on the boundary œ2.\n");
  do {
    printf("Enter N: ");
    scanf("%d", &N);
    if (N < K)
      printf("ERROR - N must be greater than or equal to K.\n");
  } while (N < K);
  if (N == M)
    printf("Note: N = M implies that all triangle have edges on œ2.\n");
  fprintf(file_id, "Number of elements (triangles) with at least one edge");
  fprintf(file_id, " on the boundary œ2\nN = %d\n", N);

  printf("\nm is the total number of nodes (vertices).\n");
  do {
    printf("Enter m: ");
    scanf("%d", &m);
    if (m <= 0)
      printf("ERROR - m must be greater than zero.\n");
  } while (m <= 0);
  fprintf(file_id, "Number of nodes (vertices) m = %d\n", m);

  printf("\nThe nodes on œ1 are known from the initial condition ");
  printf("u(x,y) = g(x,y).\n");
  printf("n is the number of nodes interior and on œ2, but not on œ1.\n");
  do {
    printf("Enter n: ");
    scanf("%d", &n);
    if ((n <= 0) || (n > m))
      printf("ERROR - n must be greater than zero and less than m.\n");
  } while ((n <= 0) || (n > m));
  if (n == m)
    printf("Note: n = m implies that œ1 contains no nodes.\n");
  fprintf(file_id, "Number of nodes interior and on œ2, but not on œ1 ");
  fprintf(file_id, "n = %d\n", n);

  /* Dynamically allocate memory for the needed arrays. */
  a  = dmatrix(1,3,1,M);	/* For matrix coefficients.                  */
  b  = dmatrix(1,3,1,M);
  c  = dmatrix(1,3,1,M);

  E  = dmatrix(1,m,1,2);	/* For each node.  E[][1] = x(j), and        */
				/*                 E[][2] = y(j).            */
  x  = dvector(1,3);		/* For E [T[i][j]] [1]  j = 1, 2, or 3       */
  y  = dvector(1,3);		/* For E [T[i][j]] [2]  j = 1, 2, or 3       */

  H  = dmatrix(1,3,1,M);
  I  = dmatrix(1,3,1,3);
  NN = dmatrix(1,3,1,M);
  z  = dcube(1,3,1,3,1,M);
  J  = dcube(1,3,1,3,1,M);

  alpha = dmatrix(1,n,1,n);
  beta  = dmatrix(1,n,1,1);
  delta = dvector(1,M);
  gamma = dvector(1,m);

  T  = imatrix(1,M,1,3);	/* Holds indices to the three nodes          */
				/* (E_i, E_j, E_k) belonging to the triangle */
				/* T_i.                                      */

  /* Enter node (vertice) data. */
  printf("The suggested node naming convention is to label the nodes on ");
  printf("the known\nboundary (œ1) with the highest numbers, ");
  printf("like E_n+1 ... E_m\n\n");

  printf("Enter all nodes (vertices) NOT on the œ1 boundary:\n");
  fprintf(file_id, "\nNodes (vertices) are:\n");
  for (j=1;j<=n;j++) {
    printf("\nE_%d = (x_%d, y_%d)\n", j, j, j);
    printf("\tx%d = ", j);
    scanf("%lf", &E[j][1]);
    printf("\ty%d = ", j);
    scanf("%lf", &E[j][2]);
    fprintf(file_id, "\tE_%d =\t(%lg,\t%lg)\ton œ2 U D boundary\n",
      j, E[j][1], E[j][2]);
  }

  printf("\nNow enter all nodes (vertices) on the œ1 boundary:\n");
  for (j=n+1;j<=m;j++) {
    printf("\nE_%d = (x_%d, y_%d)\n", j, j, j);
    printf("\tx%d = ", j);
    scanf("%lf", &E[j][1]);
    printf("\ty%d = ", j);
    scanf("%lf", &E[j][2]);
    fprintf(file_id, "\tE_%d =\t(%lg,\t%lg)\ton œ1 boundary\n",
      j, E[j][1], E[j][2]);
  }

  /* Enter element (triangle) data. */
  printf("Identify each element (triangle) by it's three nodes (vertices):\n");
  fprintf(file_id, "\nElements (triangles) are:\n");
  for (i=1;i<=M;i++) {
    printf("For triangle T_%d:\n", i);
    printf("\tEnter 1st node number: ");
    scanf("%d", &T[i][1]);
    printf("\tEnter 2nd node number: ");
    scanf("%d", &T[i][2]);
    printf("\tEnter 3rd node number: ");
    scanf("%d", &T[i][3]);
    fprintf(file_id,
      "\tT_%d = (E_%d, E_%d, E_%d)\n", i, T[i][1], T[i][2], T[i][3]);
    printf("\n");
  }

  /*************
   * ALGORITHM *
   *************/

  /* STEP #1 */
  for (l=n+1;l<=m;l++)
    gamma[l] = g(E[l][1], E[l][2]);

  /* STEP #2 */
  for (i=1;i<=n;i++) {
    beta[i][1] = 0.0;
    for (j=1;j<=n;j++)
      alpha[i][j] = 0.0;
  }

  /* STEP #3 */
  for (i=1;i<=M;i++) {		/* For each Triangle.                */

    for (j=1;j<=3;j++) {
      x[j] = E[T[i][j]][1];	/* Load x and y with element points. */
      y[j] = E[T[i][j]][2];	/* (For readability)                 */
    }

    /* Determinant. */
    delta[i] = (x[2]*y[3] + x[1]*y[2] + x[3]*y[1]) -
	       (x[2]*y[1] + x[3]*y[2] + x[1]*y[3]);

    a[1][i] = (x[2]*y[3] - y[2]*x[3]) / delta[i];
    a[2][i] = (x[3]*y[1] - y[3]*x[1]) / delta[i];
    a[3][i] = (x[1]*y[2] - y[1]*x[2]) / delta[i];

    b[1][i] = (y[2] - y[3]) / delta[i];
    b[2][i] = (y[3] - y[1]) / delta[i];
    b[3][i] = (y[1] - y[2]) / delta[i];

    c[1][i] = (x[3] - x[2]) / delta[i];
    c[2][i] = (x[1] - x[3]) / delta[i];
    c[3][i] = (x[2] - x[1]) / delta[i];

    /* Defines N_j (i) used for integration.  */

    /*
    **  for (j=1;j<=3;j++)
    **    NN[j][i] = NNN(a[j][i], b[j][i], c[j][i], x, y);
    **             = a_j(i) + b_j(i) * x + c_j(i) * y
    */

  }

  /* STEP #4 */			/* The integrals in Steps 4 and 5 can be    */
  for (i=1;i<=M;i++)		/* evaluated using numerical integration.   */
    for (j=1;j<=3;j++)
      for (k=1;k<=j;k++) {
			/* Compute all double integrals over the triangles. */
        z[j][k][i] = b[j][i]*b[k][i]*simpsons_2(1,i) +
		 c[j][i]*c[k][i]*simpsons_2(2,i) - simpsons_2(3,i);
        H[j][i] = -simpsons_2(4,i);
      }

  /* STEP #5 */
  for (i=K+1;i<=N;i++)		/* Compute all line integrals.              */
    for (j=1;j<=3;j++) {
      for (k=1;k<=j;k++)
        J[j][k][i] = simpsons(1,i,j,k);	/* fix */
      I[j][i] = simpsons(2,i,j,0);	/* fix */
    }

  /* STEP #6 */			/* Assembling the integrals over each        */
  for (i=1;i<=M;i++) {		/* into the linear system.  Do Steps 7-12.   */

    /* STEP #7 */
    for (k=1;k<=3;k++) {	/* Do Steps 8-12.                            */

      /* STEP #8 */
      l = T[i][k];		/* Find l so that E_l = (x_k(i), y_k(i)).    */

      /* STEP #9 */
      if (k > 1)
        for (j=1;j<k;j++) {	/* Do Steps 10-11.                           */

        /* STEP #10 */
        t = T[i][j];		/* Find t so that E_t = (x_j(i), y_j(i)).    */

        /* STEP #11 */
        if (l <= n) {
          if (t <= n) {
            alpha[l][t] += z[k][j][i];
            alpha[t][l] += z[k][j][i];
          } else
            beta[l][1] -= gamma[t] * z[k][j][i];
        } else if (t <= n)
          beta[t][1] -= gamma[l] * z[k][j][i];
      }

      /* STEP #12 */
      if (l <= n) {
        alpha[l][l] = alpha[l][l] + z[k][k][i];	/* Text error? (a --> alpha) */
        beta[l][1] += H[k][i];
      }
    }
  }

  /* STEP #13 */		/* Assembling the line integrals into the */
  for (i=K+1;i<=N;i++) {	/* linear system.  Do Steps 14-19.        */
    
    /* STEP #14 */
    for (k=1;k<=3;k++) {	/* Do Steps 15-19.                        */

      /* STEP #15 */
      l = T[i][k];		/* Find l so that E_l = (x_k(i), y_k(i)). */

      /* STEP #16 */
      if (k > 1)
        for (j=1;j<k;j++) {	/* Do Steps 17-18.                        */

          /* STEP #17 */
          t = T[i][j];		/* Find t so that E_t = (x_j(i), y_j(i)). */

          /* STEP #18 */
          if (l <= n) {
            if (t <= n) {
              alpha[l][t] += J[k][j][i];
              alpha[t][l] += J[k][j][i];
            } else
              beta[l][1] -= gamma[t] * J[k][j][i];
          } else if (t <= n)
            beta[t][1] -= gamma[l] * J[k][j][i];
        }

      /* STEP #19 */
      if (l <= n) {
        alpha[l][l] += J[k][k][i];
        beta[l][1] += I[k][i];
      }
    }
  }

  /* STEP #20 */
  /* Solves the system A*C=B where A=(alpha(l,t)), B=(beta(l,1)) and        */
  /* C=(gamma(t)) for 1 < l,t < n.                                          */
  gaussj(alpha, n, beta, 1);

  for (i=1;i<=n;i++)
    gamma[i] = beta[i][1];	/* Gaussj() returns the solution into beta. */

  /***********
   * OUTPUTS *
   ***********/

  /* STEP #21 */
  printf2("\n i\t gamma(j)\n");
  printf2("--------------------\n");
  for (i=1;i<=m;i++)
    printf2(" %d\t% 8.8lG\n", i, gamma[i]);

  /* STEP #22 */
  printf2("\n\n i\t j\t a(j)(i)\t b(j)(i)\t c(j)(i)\n");
  printf2("-----------------------------------------------------------\n");
  for (i=1;i<=M;i++)
    for (j=1;j<=3;j++)
      printf2(" %d\t %d% 8.8lG\t% 8.8lG\t% 8.8lG\n",
        i, j, a[j][i], b[j][i], c[j][i]);

  /* Free the memory that was dynamically allocated for the arrays. */
  free_imatrix(T,1,M,1,3);
  free_dvector(gamma,1,m);
  free_dvector(delta,1,M);
  free_dmatrix(beta,1,n,1,1);
  free_dmatrix(alpha,1,n,1,n);
  free_dcube(J,1,3,1,3,1,M);
  free_dcube(z,1,3,1,3,1,M);

/*
**  Strangely enough, this free_dmatrix() causes a null pointer assignment
**  error when using Microsoft C 5.0.  It can be omitted without introducing
**  errors in the algorithm.
**
**  free_dmatrix(NN,1,3,1,M);
*/

  free_dmatrix(I,1,3,1,3);
  free_dmatrix(H,1,3,1,M);
  free_dvector(y,1,3);
  free_dvector(x,1,3);
  free_dmatrix(E,1,m,1,2);
  free_dmatrix(c,1,3,1,M);
  free_dmatrix(b,1,3,1,M);
  free_dmatrix(a,1,3,1,M);

  NAA_do_last(outfile);		/* NAA finish-up procedure. */

}    /* STEP #23 */		/* STOP - Procedure is complete. */

/*****************************************************************************/
/* NNN() - Computes N_j (i).                                                 */
/*****************************************************************************/
double NNN(a,b,c,x,y)
double a,b,c,x,y;
{
  return (a + b*x + c*y);
}

/*****************************************************************************/
/* simpsons() - Composite Simpson's rule for single integrals.  See          */
/*              Algorithm 4.1                                                */
/*****************************************************************************/
double simpsons(type, nodei, nodej, nodek)
int type, nodei, nodej, nodek;	/* nodes are E_i on œ2. */
{
  int i, j, k, n = 10;
  double A, B;
  double **a, **b, **c, x, y;
  double h, X, XI, XI0, XI1, XI2, f(), g1(), g2();

  /* Define A, and B from E_i using nodes. */
  /* complete later.                       */
return(1.0);

  /* STEP #1 */
  h = (B-A)/n;

  /* STEP #2 */
  switch (type) {
/* g1(x,y) ---> g(a)? fix. */
    case 1: XI0 = g1(A,0) * NNN(a[j][i], b[j][i], c[j][i], x, y)
                * NNN(a[k][i], b[k][i], c[k][i], x, y)
                + g1(B,0) * NNN(a[j][i], b[j][i], c[j][i], x, y)
                * NNN(a[k][i], b[k][i], c[k][i], x, y);  break;
    case 2: XI0 = g2(A,0) * NNN(a[j][i], b[j][i], c[j][i], x, y)
                + g2(B,0) * NNN(a[j][i], b[j][i], c[j][i], x, y);  break;
  }

  XI1 = 0.0;			/* Summation of f(x(2i-1)). */
  XI2 = 0.0;			/* Summation of f(x(2i)).   */

  /* STEP #3 */
  for (i=1;i<n;i++) {

    /* STEP #4 */
    X = A + i*h;

    /* STEP #5 */
    if (i % 2 == 0)
/*      XI2 += f(X);		/* For even i. */
      switch (type) {
/* g1(x,y) ---> g(X)? fix.  need an x and y. */
        case 1: XI2 += g1(X,0) * NNN(a[j][i], b[j][i], c[j][i], x, y)
                       * NNN(a[k][i], b[k][i], c[k][i], x, y);  break;
        case 2: XI2 += g2(X,0) * NNN(a[j][i], b[j][i], c[j][i], x, y);  break;
      }

    else
/*      XI1 += f(X);		/* For odd i. */
      switch (type) {
/* g1(x,y) ---> g(X)? fix.  need an x and y. */
        case 1: XI1 += g1(X,0) * NNN(a[j][i], b[j][i], c[j][i], x, y)
                       * NNN(a[k][i], b[k][i], c[k][i], x, y);  break;
        case 2: XI1 += g2(X,0) * NNN(a[j][i], b[j][i], c[j][i], x, y);  break;
      }
  }

  /* STEP #6 */
  XI = h*(XI0 + 2.0*XI2 + 4.0*XI1) / 3.0;

  /* STEP #7 */
  return (XI);

}  				/* STOP */

/*****************************************************************************/
/* simpson_2() - Composite simpson's rule for double integrals.  See         */
/*               Algorithm 4.4                                               */
/*****************************************************************************/
double simpsons_2(type, tri)
int type, tri;			/* type selects f(), tri indicate T_i. */
{
  double A, B, C, D;
  int n = 10;			/* n is for [a,b]. */
  int m = 10;			/* m is for [c,d]. */
  int k;		/* fix */
  double **a, **b, **c;
  double h, HX, J, J1, J2, J3, K1, K2, K3;
  double L, x, y, Z, p(), q(), r(), f();
  int i, j, ch;

  /* Define A, B, C, and D from T_i.*/
  /* complete later.                */
return(1.0);


  /* STEP #1 */
  h = (B - A)/(2*n);

  /* STEP #2 */
  J1 = 0.0;			/* End terms.  */
  J2 = 0.0;			/* Even terms. */
  J3 = 0.0;			/* Odd terms.  */

  /* STEP #3 */
  for (i=0;i<=2*n;i++) {
    x   = A + i*h;		/* Composite Simpson's method for fixed x. */
    HX = (D - C)/(2*m);		/* Use c(x) and d(x) functions.            */
    switch(type) {		/* End terms for each x.                   */
      case 1: K1 = p(x,C) + p(x,D);  break;
      case 2: K1 = q(x,C) + q(x,D);  break;
      case 3: K1 = r(x,C) * NNN(a[j][i], b[j][i], c[j][i], x, y)
                 * NNN(a[k][i], b[k][i], c[k][i], x, y)
                 + r(x,D) * NNN(a[j][i], b[j][i], c[j][i], x, y)
                 * NNN(a[k][i], b[k][i], c[k][i], x, y);  break;
      case 4: K1 = f(x,C) * NNN(a[j][i], b[j][i], c[j][i], x, y)
                 + f(x,D) * NNN(a[j][i], b[j][i], c[j][i], x, y);  break;
    }

    K2 = 0.0;			/* Even terms for each x. */
    K3 = 0.0;			/* Odd terms for each x.  */
    for (j=1;j<2*m;j++) {
      y = C + j*HX;
      switch(type) {		/* End terms for each x.  */
        case 1: Z = p(x,y);  break;
        case 2: Z = q(x,y);  break;
        case 3: Z = r(x,y) * NNN(a[j][i], b[j][i], c[j][i], x, y)
                  * NNN(a[k][i], b[k][i], c[k][i], x, y);  break;
        case 4: Z = f(x,y) * NNN(a[j][i], b[j][i], c[j][i], x, y);  break;
      }
      if (j % 2 == 0)  K2 += Z;		/* For even j. */
      else             K3 += Z;		/* For odd j.  */
    }
    L = (K1 + 2.0*K2 + 4.0*K3)*HX/3.0;
    if (i == 0 || i == 2*n)  J1 += L;
    else  if (i % 2 == 0)    J2 += L;	/* For even i. */
    else                     J3 += L;	/* For odd i.  */
  }

  /* STEP #4 */
  J = (J1 + 2.0*J2 + 4.0*J3)*h / 3.0;

  /* STEP #5 */
  return (J);
}

/*****************************************************************************/
/*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.      */
/*****************************************************************************/
