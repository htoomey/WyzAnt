/******************************************************************************
				kb.c
******************************************************************************/
#include <conio.h>		/* cputs() */
#include <dos.h>		/* inportb(), enable(), disable() */
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include "key.h"

#define KEYBD 	0x60
#define F1      59
#define F2      60
#define F3      61
#define F4      62
#define ZZ      44

void interrupt (*oldkvec)();
void interrupt (*newkvec)();

/*****************************************************************************/
void interrupt kint()
{
  disable();			/* Disable more keyboard interrupts. */
  kint2();
  enable();			/* Enable more keyboard interrupts. */

  oldkvec();			/* Call original interrupt. */
}

/*****************************************************************************/
void kint2()
{
  unsigned char code;
  
  code = inportb(KEYBD);	/* get scan code */
 
  if (code > 0x7F)		/* Return if just a depressed key */
    return;

  if (code == ZZ)
    cputs("Harold is Great!");	/* Write to cursor position */

  if (code == F2)		/* quit */
    setvect(9, oldkvec);	/* Replace old vector with new vector. */
}

/*****************************************************************************/
main()				
{
  oldkvec = getvect(9);		/* Save original keyboard vector. */
  newkvec = kint;		/* Get address of new keyboard handler. */
  setvect(9, newkvec);		/* Replace old vector with new vector. */

  printf("New keyboard vector is installed\n");
  keep(0, TSRMEM);		/* Terminate and Stay Resident */
}

/*****************************************************************************/
