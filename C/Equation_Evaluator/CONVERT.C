/********************************** CONVERT.C *********************************
    		"Numerical Analysis Algorithms in C", Version 4.2             		
*******************************************************************************

PROGRAM:  convert.c

DESCRIPTION:
    Converts files with EXTENDED ASCII characters into files with ASCII-only
    characters.  Useful when loading the "Numerical Analysis Algorithms in C"
    programs onto non-MS-DOS computer platforms.  Will convert a single file
    or a list of files found in a file.

USAGE:
    C:\> convert file_name
    C:\> convert -l file_name_list

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
*******************************************************************************

  Many programs use the extended ASCII character set available on
  MS-DOS machines.  These turn into garbage when uploaded to many UNIX
  machines.  ASCII uses 7-bits per byte while EXTENDED ASCII uses 8-bits
  per byte.  When EXTENDED ASCII characters are uploaded to ASCII-only
  platforms, the 8th bit is masked off making 7-bit ASCII characters.  To
  avoid the ill effects of this masking this program has been written.  It
  converts these programs (any MS-DOS file) into ASCII-only files, making the
  appropriate substitutions.

  Erroneous uploading may look like this:

      normal         w/ convert.c   w/out convert.c      w/out convert.c
                                      7-bit ASCII          8-bit ASCII

       Ù „              [ PI             t c              \364 \343
       ≥ f(x)           | f(x)           3 f(x)           \263 f(x)
       ı ‡              ] alpha          u `              \365 \340
                                                          (in octal)

  Past versions of this product came in EXTENDED ASCII (v4.1) and ASCII-only
  versions (v4.11).  This program should eliminate the need for two different
  versions.  Below is a table called ASCIIlist[] which contains the EXTENDED
  ASCII characters and their appropriate substitutions.  They can be easily
  modified by the user.

******************************************************************************/

/*****************/
/* INCLUDE FILES */
/*****************/

#include <io.h>
#include <stdio.h>
#include <string.h>

/**************/
/* PROTOTYPES */
/**************/

int getline(char *line, int max, FILE *stream);
int subst(int ch, int j);

/***********/
/* DEFINES */
/***********/

#define MAXLINE		160		/* Maximum buffer length             */
#define MAXFILE		80		/* Maximum file name size            */

/********************/
/* GLOBAL VARIABLES */
/********************/

char line[MAXLINE];			/* Original file line buffer         */
char ASCIIline[MAXLINE];		/* Converted file line buffer        */
char *ASCIIlist[] = {			/* Translation table                 */
	/* CODE CHAR	SUBST */
	/* ------------------ */
	/* 128  Ä */	"C",
	/* 129  Å */	"u",
	/* 130  Ç */	"e",
	/* 131  É */	"a",
	/* 132  Ñ */	"a",
	/* 133  Ö */	"a",
	/* 134  Ü */	"a",
	/* 135  á */	"c",
	/* 136  à */	"e",
	/* 137  â */	"e",
	/* 138  ä */	"e",
	/* 139  ã */	"i",
	/* 140  å */	"i",
	/* 141  ç */	"i",
	/* 142  é */	"A",
	/* 143  è */	"A",
	/* 144  ê */	"E",
	/* 145  ë */	"ae",
	/* 146  í */	"AE",
	/* 147  ì */	"o",
	/* 148  î */	"o",
	/* 149  ï */	"o",
	/* 150  ñ */	"u",
	/* 151  ó */	"u",
	/* 152  ò */	"y",
	/* 153  ô */	"O",
	/* 154  ö */	"U",
	/* 155  õ */	" cents",
	/* 156  ú */	"L",
	/* 157  ù */	"Y",
	/* 158  û */	"Pt",
	/* 159  ü */	"f",
	/* 160  † */	"a",
	/* 161  ° */	"i",
	/* 162  ¢ */	"o",
	/* 163  £ */	"u",
	/* 164  § */	"n",
	/* 165  • */	"N",
	/* 166  ¶ */	"a",
	/* 167  ß */	"o",
	/* 168  ® */	"?",
	/* 169  © */	"+",
	/* 170  ™ */	"+",
	/* 171  ´ */	"1/2",
	/* 172  ¨ */	"1/4",
	/* 173  ≠ */	"!",
	/* 174  Æ */	"<<",
	/* 175  Ø */	">>",
	/* 176  ∞ */	"|",
	/* 177  ± */	"|",
	/* 178  ≤ */	"|",
	/* 179  ≥ */	"|",
	/* 180  ¥ */	"+",
	/* 181  µ */	"+",
	/* 182  ∂ */	"+",
	/* 183  ∑ */	"+",
	/* 184  ∏ */	"+",
	/* 185  π */	"+",
	/* 186  ∫ */	"|",
	/* 187  ª */	"+",
	/* 188  º */	"+",
	/* 189  Ω */	"+",
	/* 190  æ */	"+",
	/* 191  ø */	"+",
	/* 192  ¿ */	"+",
	/* 193  ¡ */	"+",
	/* 194  ¬ */	"+",
	/* 195  √ */	"+",
	/* 196  ƒ */	"-",
	/* 197  ≈ */	"+",
	/* 198  ∆ */	"+",
	/* 199  « */	"+",
	/* 200  » */	"+",
	/* 201  … */	"+",
	/* 202    */	"+",
	/* 203  À */	"+",
	/* 204  Ã */	"+",
	/* 205  Õ */	"=",
	/* 206  Œ */	"+",
	/* 207  œ */	"+",
	/* 208  – */	"+",
	/* 209  — */	"+",
	/* 210  “ */	"+",
	/* 211  ” */	"+",
	/* 212  ‘ */	"+",
	/* 213  ’ */	"+",
	/* 214  ÷ */	"+",
	/* 215  ◊ */	"|",
	/* 216  ÿ */	"|",
	/* 217  Ÿ */	"+",
	/* 218  ⁄ */	"+",
	/* 219  € */	"*",
	/* 220  ‹ */	"*",
	/* 221  › */	"*",
	/* 222  ﬁ */	"*",
	/* 223  ﬂ */	"*",
	/* 224  ‡ */	"alpha",
	/* 225  · */	"beta",
	/* 226  ‚ */	"gamma",
	/* 227  „ */	"PI",
	/* 228  ‰ */	"SUM",
	/* 229  Â */	"sigma",
	/* 230  Ê */	"mu",
	/* 231  Á */	"tau",
	/* 232  Ë */	"phi",
	/* 233  È */	"theta",
	/* 234  Í */	"OHM",
	/* 235  Î */	"d",
	/* 236  Ï */	"infinity",
	/* 237  Ì */	"psi",
	/* 238  Ó */	"epsilon",
	/* 239  Ô */	"intersect",
	/* 240   */	"=",
	/* 241  Ò */	"+/-",
	/* 242  Ú */	">=",
	/* 243  Û */	"<=",
	/* 244  Ù */	"[",
	/* 245  ı */	"]",
	/* 246  ˆ */	"/",
	/* 247  ˜ */	"=",
	/* 248  ¯ */	" degrees",
	/* 249  ˘ */	"*",
	/* 250  ˙ */	"*",
	/* 251  ˚ */	"SQRT",
	/* 252  ¸ */	"n",
	/* 253  ˝ */	"^2",
	/* 254  ˛ */	"*",
	/* 255    */	" "
};


#define	TEMP_FILE	"tmpfile1"	/* A unique file name? */

/*****************************************************************************/
/*                                main()                                     */
/*****************************************************************************/
main(int argc, char *argv[])
{
  int LIST;				/* LIST = 1 for file_name_list       */
  int i, j, linenum, ch, numfiles, length, VALID_FILE_NAME;
  char filename[MAXFILE], tmpfilename[9];
  FILE *fp, *fptmp, *fplist;


  printf("\n\"Numerical Analysis Algorithms in C\" CONVERT program ");
  printf("Version 4.2\n");
  printf("Copyright (c) CARE-FREE SOFTWARE 1991.  All rights reserved.\n\n");


  switch (argc) {

    case 1:			/* Help message */
      printf("\nUSAGE:  CONVERT file_name");
      printf("\n        CONVERT -l file_name_list\n");
      exit(-1);
      break;

    case 2:			/* Single file */
      if ((fp = fopen(argv[1], "r")) == NULL) {
        printf("ERROR - Can not open file \"%s\".\n", argv[1]);
        exit(-1);
      }
      strcpy(filename, argv[1]);
      LIST = 0;
      break;

    case 3:			/* Multiple files */
      if ((strcmp(argv[1], "-l") == 0) || (strcmp(argv[1], "-L") == 0)) {
        if ((fplist = fopen(argv[2], "r")) == NULL) {
          printf("ERROR - Can not open list of files in \"%s\".\n", argv[2]);
          exit(-1);
        }
      }
      LIST = 1;
      break;

    default:
      exit(-1);
      break;
  }

  numfiles = 0;				/* Initialize file counter */

  do {
    strcpy(tmpfilename, TEMP_FILE);

    if (LIST) {			/* Read in a file name from the list. */

      do {
        if (getline(filename, MAXFILE, fplist) != 0) {
          filename[strlen(filename)-1] = '\0';	/* Remove carriage return. */
          if ((fp = fopen(filename, "r")) == NULL) {
            printf("ERROR - Can not open file \"%s\".\n", filename);
            VALID_FILE_NAME = 0;	/* Invalid file name.  try again. */
          } else {
            VALID_FILE_NAME = 1;	/* Valid file name.  Can continue. */
          }

        } else {		/* No more files to convert, so exit. */
          printf("\nConverted %d file%s successfully\n",
              numfiles, (numfiles > 1) ? "s" : "");
          fcloseall();
          exit(1);
        }
      } while (!VALID_FILE_NAME);

    }

    numfiles++;				/* Counts number of files converted  */

    /* Open a temporary file. */
    if ((fptmp = fopen(tmpfilename, "w")) == NULL) {
      printf("ERROR - Can not open a temporary file.\n");
      exit(1);
    }

    linenum = 0;			/* Read each line of the file        */
    while ((length = getline(line, MAXLINE, fp)) != 0) {
      linenum++;

      j = 0;				/* Reset ASCIIline buffer index      */
      for (i=0;i<length;i++) {
        ch = ((int) line[i]) + 256;	/* Convert character into an index   */
        if ((ch >= 128) && (ch <= 255))	/* Is char EXTENDED ASCII?           */
          j += subst(ch, j);		/* Make the substitution             */
        else
          ASCIIline[j++] = line[i];	/* Is ASCII-only already             */
      }
      ASCIIline[j] = '\0';		/* Terminate the line properly       */
      fputs(ASCIIline, fptmp);		/* Write line to temporary file      */

    } /* while */

    fclose(fp);				/* Close original file               */
    fclose(fptmp);			/* Close temporary file              */

    remove(filename);			/* Delete original file              */
    rename(tmpfilename, filename);	/*   replacing it with the new one   */

    printf("%s : successfully converted to standard ASCII\n", filename);

  } while (LIST);			/* Loop through entire list          */

} /* main */

/*****************************************************************************/
/* getline:  read a line, return length.                                     */
/*****************************************************************************/
int getline(char *line, int max, FILE *stream)
{
  if (fgets(line, max, stream) == NULL)
    return 0;
  else
    return strlen(line);
}

/*****************************************************************************/
/* subst:  substitutes a string for a character, return length.              */
/*****************************************************************************/
int subst(int ch, int j)
{
  int i, len;

  len = strlen(ASCIIlist[ch - 128]);
  for (i=0;i<len;i++)
    ASCIIline[j++] = ASCIIlist[ch - 128][i];
  return len;
}

/******************************************************************************
*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.       *
******************************************************************************/
