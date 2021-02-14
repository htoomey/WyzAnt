:*********************************************************************
:  Program:  CC.BAT  (MS-DOS batch file)
:
:  Purpose:
:    Cc.bat is an MS-DOS batch file used for compiling, running and
:    viewing a Microsoft C 5.0 program.  It can be easily altered to
:    allow for linking to "NAAUTIL.OBJ" (with or without "EQEVAL.C"),
:    speeding up the compile time.  Can also be altered to increase
:    the stack size of a program.
:
:  Written By:  Harold A. Toomey
:  Date:        21 Aug 1991
:
:  Example:  C:\> CC 041   where "041.C" is the file name.
:                          (Leave off the .C extension)
:*********************************************************************
:
:  Do a compile and link (CL), disabling optimization (/Od) and
:  invoke the QuickC compiler option (/qc) to shorten the compile
:  time.
:
CL /Od /qc %1.c
:
:  Replace the main line above with the line below if a larger stack
:  is needed.  This allows for a stack size of 4K bytes (/link
:  /ST:4096).  A MUST when compiling algorithms 2.8A, 5.6 and 8.1.  
:
: CL /Od %1.c /link /ST:4096
:
:  Replace the main line above with the two lines below if
:  "NAAUTIL.C" has been compiled and exists as object code
:  ("NAAUTIL.OBJ").  The first line creates %1.OBJ while the second
:  line links %1.OBJ to "NAAUTIL.OBJ" to form %1.EXE.  (Most
:  flexible)
:
: CL /Od /c %1.c
: CL %1 NAAUTIL
:
:
:  Create "NAAUTIL.OBJ" object code by running the below step once.
:  Remember to re-compile "NAAUTIL.C" every time one of its flags
:  are changed.
:
: CL /c NAAUTIL.C
:
:
:  If the program compiled correctly, then run it.  If not, exit
:  this batch file.  Replace "%1" with "%1 < IN\%1.IN" if using the
:  input file for initial testing.
IF ERRORLEVEL 1 GOTO END
%1
:
:  View the program's output file using "LIST.COM".  You may
:  substitute DOS's TYPE command for "LIST.COM."
LIST %1.OUT
:END
:*********************************************************************
