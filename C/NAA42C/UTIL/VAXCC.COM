$! *******************************************************************
$!   Program:  VAXCC.COM  (VAX command file)
$! 
$!   Purpose:
$!     VAXCC.COM is a VAX command file used for compiling and linking
$!     a mathematical C program.  It can be easily altered to allow
$!     for linking to "NAAUTIL.OBJ" (with or without "EQEVAL.OBJ"),
$!     speeding up the compile time.
$! 
$!   Written By:  Harold A. Toomey
$!   Date:        21 August 1991
$! 
$!   Example:  $ @VAXCC 041   where "041.C" is the file name.
$!                            (Leave off the .C extension)
$! *******************************************************************
$!
$!  Define the link libraries to allow the use of the /G_FLOAT
$!  qualifier but not the use of Curses.  These two defines may be
$!  placed into your "LOGIN.COM" file to be performed once at login
$!  time.
$!
$ DEFINE LNK$LIBRARY    SYS$LIBRARY:VAXCRTLG
$ DEFINE LNK$LIBRARY_1  SYS$LIBRARY:VAXCRTL
$!
$!  Compile the program with the floating-point qualifier to create
$!  an .OBJ object file.  Link the object file to create the
$!  executable.
$!
$ CC /G_FLOAT 'P1'.C
$ LINK 'P1', LNK$LIBRARY/LIB, LNK$LIBRARY_1/LIB 
$!
$! *******************************************************************
$!  Replace the above "LINK" line with the below "LINK" line to link
$!  to the pre-compiled "NAAUTIL.C" object code file.
$! 
$! LINK 'P1', NAAUTIL, LNK$LIBRARY/LIB, LNK$LIBRARY_1/LIB
$! 
$! *******************************************************************
$!  Create "NAAUTIL.OBJ" object code by running the below step once.
$!  Rember to re-compile "NAAUTIL.C" every time one of its flags
$!  are changed.
$! 
$! CC /G_FLOAT NAAUTIL.C 
$! 
$! *******************************************************************
