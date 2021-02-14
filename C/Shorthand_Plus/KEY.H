/****************************************************************************/
/*                                                                          */
/*                                 Key.h                                    */
/*                                                                          */
/*             This header file defines a global variable, macros,          */
/*             function pointers, and function prototypes needed            */
/*             in the Key.c program.                                        */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

#define	NUMK	20		/* Number of KBytes to leave memory resident */
#define	TSRMEM	(NUMK / 16)	/* Number of paragraphs of memory to TSR */

/* Function Declarations */
void interrupt kint(void);
void kint2(void);