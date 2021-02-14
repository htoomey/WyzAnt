/****************************** CALLOC.C *************************************/

#define MAX_BYTES	0x2000	/* Number of available bytes (8K) */

#include <stdio.h>		/* For size_t and NULL */


/*****************************************************************************/
/* naacalloc() - Use to replace calloc() if it is not implemented correctly  */
/*               on your non-ANSI C compiler.                                */
/*****************************************************************************/

/*
**  To use, first insert #include "calloc.c" just after #include <stdlib.h>
**  in "naautil.h".  Do not place it inside "naautil2.h" or "naautil3.h"
**  since these two files are always followed by "naautil.h" inside each of
**  the algorithms.
**
**  Secondly, replace every occurance of "calloc" with "naacalloc" inside
**  "naautil.h", "naautil2.h" and "naautil3.h."
**
**  Finally, replace every occurance of "free" with "naafree" inside
**  "naautil.h", "naautil2.h" and "naautil3.h."
**
**  If problems occur, you may need to remove #include <stdlib.h>, replacing
**  it with <malloc.h> or <alloc.h> for older C compilers, or make MAX_BYTES
**  larger or smaller to suit your data requirements.
*/

char *naacalloc(number_of_words, word_size)
size_t number_of_words, word_size;
{
  static char naacalloc_memory[MAX_BYTES];	/* Makes a static array */
  static int buffer_ptr = 0;	/* Initialize pointer to first byte */
  char *ptr;
  int i, size;

  size = number_of_words * word_size;	/* Compute number of bytes needed */

  if (buffer_ptr + size > MAX_BYTES) {		/* Range checking */
    fprintf(stderr, "\nERROR - naacalloc() memory size exceeded.");
    fprintf(stderr, "\n        Fix by enlarging MAX_BYTES in \"calloc.c\".\n");
    return (NULL);		/* mimics calloc() if not enough memory */
  }

  ptr = &naacalloc_memory[buffer_ptr];	/* Set pointer to first free byte */
  for (i=0; i < size; i++)
    naacalloc_memory[buffer_ptr++] = '\0'; /* Initialize each byte to zero */
  return (ptr);				/* Return pointer to first free byte */
}


/*****************************************************************************/
/* naafree() - Use to replace free() if using naacalloc() above.  Does not   */
/*             really free up any memory until the program is terminated.    */
/*****************************************************************************/

/*
**  Be sure to replace every occurance of "free" with "naafree" inside
**  "naautil.h", "naautil2.h" and "naautil3.h."
*/

void naafree(dummy_ptr)
char *dummy_ptr;
{
  /* Do nothing.  Too much overhead to implement. */
}

/******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 2Q 1991, v4.2      *
*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.       *
******************************************************************************/
