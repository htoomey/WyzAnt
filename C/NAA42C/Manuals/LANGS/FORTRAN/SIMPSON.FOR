C  FILE NAME: SIMPSON.FOR
C  LANGUAGE:  FORTRAN 77
C  COMPILER:  Microsoft FORTRAN 77 Version 3.3
C  STATUS:    Compiles and runs correctly
C
C *****************************************************************************
C		    Composite Simpson's Rule - Algorithm 4.1
C *****************************************************************************
C
C                                 ôb
C To approximate the integral I = ³  f(x) dx:
C                                 õa
C
C INPUT endpoints a, b; positive integer n; the function f().
C
C OUTPUT approximation XI to I.
C
C *****************************************************************************
C 	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 2Q 1991, v4.0
C *****************************************************************************
C
C
      PROGRAM SIMPSON
C
      DOUBLE PRECISION A, B, H, X
      INTEGER OUT, I, N
      CHARACTER*80 TITLE
      CHARACTER*12 OUTFILE
C
C     -- OUT will be assigned to OUTFILE for printing to an output file
      PARAMETER (OUT=4)
C
C     -- Default name of the outfile
      DATA OUTFILE/'SIMPSON.OUT'/
C
C *****************************************************************************
C * F(X) - Function to evaluate, f(x).  Needs updating $.
C *****************************************************************************
C
      F(X) = DSIN(X)
C
C *****************************************************************************
C
C     **********
C     * INPUTS *
C     **********
C
C     -- Prompts for optional title
      WRITE (*,1)
      WRITE (*,2)
      WRITE (*,3)
      WRITE (*,*) ' Enter a title [ie - Set 2.1,  Problem 2 a) ].'
      WRITE (*,*) ' ----> '
      READ (*, 100) TITLE
100   FORMAT(A80)
      WRITE (*,4)
      WRITE (*,*) 'Enter endpoint a:'
      READ (*, 101) A
101   FORMAT(D8.0)
      WRITE (*,*) 'Enter endpoint b:'
      READ (*, 102) B
102   FORMAT(D8.0)
200   WRITE (*,*) 'Enter number of intervals on [a,b], n:'
      READ (*, 103) N
103   FORMAT(I8)
C
C     -- ERROR - n must be positive
      IF (N .LE. 0) THEN
        WRITE (*,*) 'ERROR - n must be greater than zero.'
        GOTO 200
      ENDIF
C
C     *************
C     * ALGORITHM *
C     *************
C
C     STEP #1
      H = (B - A) / N
C
C     STEP #2
      XI0 = F(A) + F(B)
C     -- Summation of f(x(2i-1))
      XI1 = 0.0
C     -- Summation of f(x(2i))
      XI2 = 0.0
C
C     STEP #3
      DO 50 I=1,N-1
C
C       STEP #4
        X = A + I*H
C
C       STEP #5
        IF (MOD(I,2) .EQ. 0) THEN
C         -- For I even
          XI2 = XI2 + F(X)
        ELSE
C         -- For I odd
          XI1 = XI1 + F(X)
        ENDIF
50    CONTINUE
C
C     STEP #6
      XI = H * (XI0 + 2*XI2 + 4*XI1) / 3.0
C
C     ***********
C     * OUTPUTS *
C     ***********
C
C     STEP #7
C     -- Write output to the screen (*) and to a file (OUT=4).
C     -- Open outfile for output
C     OPEN (OUT,FILE=OUTFILE,STATUS='UNKNOWN')
      OPEN (OUT,FILE=OUTFILE)
C
      WRITE (*,1)
      WRITE (OUT,1)
      WRITE (*,2)
      WRITE (OUT,2)
      WRITE (*,3)
      WRITE (OUT,3)
1     FORMAT(' ',78('-'))
2     FORMAT(' ',16X,'"Numerical Analysis Algorithms in FORTRAN" v4.0')
3     FORMAT(' ',78('-'),/)
      WRITE (*,*) TITLE
      WRITE (OUT,*) TITLE
      WRITE (*,4)
      WRITE (OUT,4)
4     FORMAT(' Composite Simpson`s Rule - Algorithm 4.1',/)
      WRITE (*,5) A, B
      WRITE (OUT,5) A, B
5     FORMAT(' f(x) = sin(x) from ',D12.7,' to ',D12.7,'.',/) A, B 
      WRITE (*,6) N
      WRITE (OUT,6) N
6     FORMAT(' n = ',I4,' intervals on [a,b].')
      WRITE (*,7) H
      WRITE (OUT,7) H
7     FORMAT(' Interval number h = ',D13.7/)
      WRITE (*,8) B
      WRITE (OUT,8) B
8     FORMAT('     ô',D13.7)
      WRITE (*,9) XI
      WRITE (OUT,9) XI
9     FORMAT(' I = ³  f(x) dx = ',D13.7)
      WRITE (*,10) A
      WRITE (OUT,10) A
10    FORMAT('     õ',D13.7/)
      WRITE (*,11) N + 1
      WRITE (OUT,11) N + 1
11    FORMAT(' Required ',I4,' functional evaluations.',/)
C
C     -- Closes outfile previously opened for output
      ENDFILE (OUT)
      CLOSE (OUT)
      WRITE (*,12) OUTFILE
12    FORMAT(' Output saved into file "', A, '".',/)
C
      END
C
C *****************************************************************************
C 	   Copyright (C) 1991, Harold A. Toomey, All Rights Reserved.
C *****************************************************************************
