10  'FILE NAME: SIMPSON.BAS
20  'LANGUAGE:  BASIC
30  'COMPILER:  Microsoft GW-BASIC Version 3.20
40  'STATUS:    Compiles and runs correctly
50  '
60  '**************************************************************************
70  '		Composite Simpson's Rule - Algorithm 4.1
80  '**************************************************************************
90  '
100  '                                  ôb
110 '  To approximate the integral I = ³  f(x) dx:
120 '                                  õa
130 '
140 '  INPUT endpoints a, b; positive integer n; the function fnf().
150 '
160 '  OUTPUT approximation XI to I.
170 '
180 '**************************************************************************
190 '  Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 2Q 1991, v4.0
200 '**************************************************************************
210 '
220  PI# = 3.141592653589793#	'The constant PI (Usage: X = 2 * PI# )
230  OUTFILE$ = "SIMPSON.OUT"	'Output file name
240  EQTEXTF$ = "f(x) = sin(x)"	'Needs updating  $
250 '
260  DEFDBL A, B, H, X		'For XI, XI0, XI1, and XI2 also
270  DEFINT I, N
280  DEFSTR T			'For title
290 '     
300 '**************************************************************************
310 '* FNF(X) - Function to evaluate, f(x).  Needs updating $.
320 '**************************************************************************
330  DEF FNF(X) = SIN(X)
340 '**************************************************************************
350 '
360 '**********
370 '* INPUTS *
380 '**********
390 '
400 'Get the optional title
410  PRINT "------------------------------------------------------------------"
420  PRINT "           'Numerical Analysis Algorithms in BASIC' v4.0          "
430  PRINT "------------------------------------------------------------------"
440  PRINT
450  PRINT "Enter a title [ie - Set 2.1,  Problem 2 a) ]."
460  INPUT "----->"; TITLE$
470 '
480  PRINT
490  PRINT "Composite Simpson's Rule - Algorithm 4.1"
500  PRINT
510  INPUT "Enter endpoint a = "; A
520  INPUT "Enter endpoint b = "; B
530  INPUT "Enter number of intervals on [a,b], n = "; N
540 '
550 'ERROR - N must be positive
560  IF (N <= 0) THEN PRINT "ERROR - n must be greater than zero.": GOTO 530
570 '
580 '*************
590 '* ALGORITHM *
600 '*************
610 '
620 'STEP #1
630  H = (B - A) / N
640 '
650 'STEP #2
660  XI0 = FNF(A) + FNF(B)
670  XI1 = 0!			'Summation of f(x(2i-1))
680  XI2 = 0!			'Summation of f(x(2i))
690 '
700 'STEP #3
710  FOR  I = 1 TO N - 1
720 '
730 '  STEP #4
740    X = A + I*H
750 '
760 '  STEP #5                (For even i)             (For odd i)
770    IF (I MOD 2 = 0)  THEN XI2 = XI2 + FNF(X)  ELSE XI1 = XI1 + FNF(X)
780 '
790  NEXT I
800 '
810 'STEP #6
820  XI = H * (XI0 + 2*XI2 + 4*XI1) / 3
830 '
840 '***********
850 '* OUTPUTS *
860 '***********
870 '
880 'STEP #7
890 'Write output to the SCREEN
900  PRINT "------------------------------------------------------------------"
910  PRINT "           'Numerical Analysis Algorithms in BASIC' v4.0          "
920  PRINT "------------------------------------------------------------------"
930  PRINT
940  PRINT TITLE$		'Print optional title
950  PRINT
960  PRINT "Composite Simpson's Rule - Algorithm 4.1"
970  PRINT
980  PRINT EQTEXTF$, " from"; A ;"to"; B ;"."
990  PRINT
1000  PRINT "n ="; N ;"intervals on [a,b]."
1010  PRINT "Interval number h ="; H
1020  PRINT
1030  PRINT "     ô"; B
1040  PRINT "XI = ³  f(x) dx = "; XI
1050  PRINT "     õ"; A
1060  PRINT
1070  PRINT "Required"; N+1 ;"functional evaluations."
1080 '
1090 'Write output to a FILE
1100  OPEN "O", #1, OUTFILE$	'Create or open the file "041.OUT"
1110  PRINT#1, "--------------------------------------------------------------"
1120  PRINT#1, "        'Numerical Analysis Algorithms in BASIC' v4.0         "
1130  PRINT#1, "--------------------------------------------------------------"
1140  PRINT#1,
1150  PRINT#1, TITLE$		'Print optional title
1160  PRINT#1,
1170  PRINT#1, "Composite Simpson's Rule - Algorithm 4.1"
1180  PRINT#1,
1190  PRINT#1, EQTEXTF$, " from"; A ;"to"; B ;"."
1200  PRINT#1,
1210  PRINT#1, "n ="; N ;"intervals on [a,b]."
1220  PRINT#1, "Interval number h ="; H
1230  PRINT#1,
1240  PRINT#1, "     ô"; B
1250  PRINT#1, "XI = ³  f(x) dx = "; XI
1260  PRINT#1, "     õ"; A
1270  PRINT#1,
1280  PRINT#1, "Required"; N + 1 ;"functional evaluations."
1290  CLOSE #1
1300 '
1310  PRINT
1320  PRINT "Output saved into file '"; OUTFILE$; "'."
1330  END			'STOP
1340 '
1350 '************************************************************************
1360 '  Copyright (C) 1991, Harold A. Toomey, All Rights Reserved.
1370 '*************************************************************************
