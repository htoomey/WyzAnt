/******************************** EQEVAL.C ************************************
		"Numerical Analysis Algorithms in C" v4.2
		     Equation Evaluator Routines
******************************************************************************/

/*
**  These routines allow one to enter a mathematical equation during a
**  program's run time and have it evaluated.  It saves you from the hassle
**  of editing an equation inside the program's source code and recompiling
**  it again.
**
**  Equation Evaluator uses infix notation.  Only mathematical functions
**  defined in C are valid.  If ANSI is set to TRUE, then the new unary +
**  operator can also be used.  Some C compilers implement non-standard
**  mathematical functions, such as "hypot()" (same as sqrt(x*x+y*y)),
**  "j0()", "j1()", "jn()" (bessel functions), and "fcos()", "fsin()",
**  "ftan()" (float arguments only functions).  These are currently not
**  supported.  See the #defines below for a list of valid functions.
**
**  This file contains the following procedures and functions:
**
**    Return   Procedure
**    Value    Name           Description
**    ----------------------------------------------------------------------
**    void     get_eq        -  Gets the equation to evaluate
**
**    void     parse_eq      -  Parses the equation into a structure
**    int      ispun         -  Identifies punctuation
**    int      isvar         -  Identifies variables
**    int      isop1         -  Identifies operators *, / and %
**    int      isop2         -  Identifies operators + and -
**    int      isnum         -  Identifies numbers
**    int      isfunct       -  Identifies functions
**
**    double   eval_eq       -  Evaluates the parsed function
**    void     traverse_tree -  Performs a post-order traversal for eval_eq
*/

/*****************
 * INCLUDE FILES *
 *****************/

#include <math.h>		/* For sin(), cos(), etc.               */
#include <limits.h>		/* For INT_MIN and INT_MAX.             */
#include <string.h>		/* For strcpy(), strcat() and strncmp() */
#include <ctype.h>		/* For isalpha(), etc.                  */

/*
**  Requires <stdio.h>, <stdlib.h> and <stdarg.h> which are included in
**  "naautil.h."
*/


/***********
 * DEFINES *
 ***********/

/*
**  The FLAGs below allows the user more flexibility when compiling and
**  running these Numerical Analysis Algorithms.  Many of the below flags
**  are previously defined in "naautil.c."
*/

#ifndef FALSE
#define FALSE		0	/* Define FALSE if not already defined. */
#endif

#ifndef TRUE
#define TRUE		!FALSE	/* Define TRUE if not already defined.  */
#endif

#define VERBOSE		FALSE	/* Set to "TRUE" to see diagnostics.    */

#ifndef ANSI
#define ANSI		TRUE	/* For ANSI C compilers */
#endif

#ifndef ANSI_FUNCT
#define ANSI_FUNCT	FALSE	/* For ANSI C function style */
#endif

#ifndef OLD_UNIX_OS
#define OLD_UNIX_OS	FALSE	/* For older UNIX compilers */
#endif

#ifndef MAX_LINE_SIZE
#define	MAX_LINE_SIZE	130	/* Largest input line size accepted.      */
#endif				/* Set to 130 for MS-DOS computers.       */

#ifndef HUGE_VAL		/* Needed for VAX C.  See <math.h> */
#define HUGE_VAL	1.0e+37
#endif


#define	EQEVAL_OBJ	FALSE	/* Set to TRUE if using "eqeval.c" as a    */
				/* pre-compiled object code file to be     */
				/* linked to at algorithm compile time.    */
				/* Set to FALSE if using "eqeval.c" as an  */
				/* un-compiled include file (default).     */

/* Limits */
#define	MAX_EQ_SZ	MAX_LINE_SIZE	/* Max. chars. for an equation.    */
#define	MAX_NUM_OPS	(MAX_EQ_SZ/2)	/* Max. number of operators.       */


/* Mathematical element ID tags. */

/* Control */
#define	SKIP_ID		0	/* skip ID                             */
#define	NO_ID		1	/* invalid ID                          */

/* Punctuation */
#define LEFT_ID		2	/* left parenthesis '('                */
#define RIGHT_ID	3	/* right parenthesis ')'               */
#define COMMA_ID	4	/* comma ','                           */

/* Numbers */
#define	NUM_ID		5	/* number (constant)                   */

/* Variables */
#define	VARX_ID		6	/* variable x                          */
#define	VARY_ID		7	/* variable y                          */
#define	VART_ID		8	/* variable t                          */

/* Operands */
#define	ADD_ID		9	/* addition                            */
#define	SUB_ID		10	/* subtraction                         */
#define	MULT_ID		11	/* multiplication                      */
#define	DIV_ID		12	/* division                            */
#define	REM_ID		13	/* integer remainder (see fmod)        */

/* Functions */
#define ABS_ID		14	/* integer absolute value (see fabs)   */
#define ACOS_ID		15	/* arccosine                           */
#define ASIN_ID		16	/* arcsin                              */
#define ATAN_ID		17	/* arctangent                          */
#define ATAN2_ID	18	/* arctangent                          */
#define CEIL_ID		19	/* ceiling                             */
#define COS_ID		20	/* cosine                              */
#define COSH_ID		21	/* hyperbolic cosine                   */
#define EXP_ID		22	/* exponential e^x                     */
#define FABS_ID		23	/* floating-point absolute value       */
#define FLOOR_ID	24	/* floor                               */
#define FMOD_ID		25	/* floating-point modulus              */
#define FREXP_ID	26	/* normalized fraction                 */
#define LDEXP_ID	27	/* x * 2^n                             */
#define LOG_ID		28	/* natural logarithm                   */
#define LOG10_ID	29	/* base 10 logarithm                   */
#define MODF_ID		30	/* fractions                           */
#define POW_ID		31	/* exponentiation x^y                  */
#define SIN_ID		32	/* sine                                */
#define SINH_ID		33	/* hyperbolic sine                     */
#define SQRT_ID		34	/* square root                         */
#define TAN_ID		35	/* tangent                             */
#define TANH_ID		36	/* hyperbolic tangent                  */


/* Operator Types */
#define PUNCT_TYPE	1	/* punctuation types                   */
#define NUM_TYPE	2	/* number types                        */
#define VAR_TYPE	3	/* variable types                      */
#define OP_TYPE		4	/* operand types                       */
#define	UNARY_TYPE	5	/* unary plus or minus type            */
#define FUNCT1_TYPE	6	/* single argument function types      */
#define FUNCT2_TYPE	7	/* double argument function types      */

/* Tree Node Identifiers */
#define INTERIOR	1	/* Non-leaf nodes, needing evaluating  */
#define LEAF		2	/* Values to be used by INTERIOR nodes */
#define DONE		3	/* Already-been-used LEAF nodes        */


/********************
 * GLOBAL VARIABLES *
 ********************/

struct EQ_NODE {		/* Equation node.  Used to build a tree. */
  int id;			/* For ADD_ID, SIN_ID, POW_ID, etc.      */
  double value;			/* Floating-point value once evaluated.  */
  struct EQ_NODE *left;		/* Pointer to left child node.           */
  struct EQ_NODE *right;	/* Pointer to right child node.          */
};

/* Allocates an array of node pointers */
#if EQEVAL_OBJ == FALSE		/* If "eqeval.c" is not pre-compiled. */
  struct EQ_NODE *eq_node[MAX_NUM_OPS];
  double eq_eval_x;		/* Global storage for Eq. Eval's x. */
  double eq_eval_y;		/* Global storage for Eq. Eval's y. */
  double eq_eval_t;		/* Global storage for Eq. Eval's t. */
#else				/* If "eqeval.c" is pre-compiled.   */
  extern struct EQ_NODE *eq_node[MAX_NUM_OPS];
  extern double eq_eval_x;
  extern double eq_eval_y;
  extern double eq_eval_t;
#endif  /* EQEVAL_OBJ */


/************************
 * FUNCTION PROTOTYPING *
 ************************/

#if ANSI == TRUE
  /* ANSI STANDARD PROTOTYPING (Post-December 14, 1989) */
  void     get_eq        (void);
  void     parse_eq      (void);
  int      ispun         (char **, int *, int *);
  int      isvar         (char **, int *, int *);
  int      isop1         (char **, int *, int *);
  int      isop2         (char **, int *, int *);
  int      isnum         (char **, int *, int *, double *);
  int      isfunct       (char **, int *, int *);
  double   eval_eq       (double, ...);
  void     traverse_tree (struct EQ_NODE *);
#else
  /* OLDER STYLE PROTOTYPING (Pre-December 14, 1989)       */
  /* Placed here for compatibility with older C compilers. */
  void     get_eq();
  void     parse_eq();
  int      ispun();
  int      isvar();
  int      isop1();
  int      isop2();
  int      isnum();
  int      isfunct();
  double   eval_eq();
  void     traverse_tree();
#endif	/* ANSI */


#if EQEVAL_OBJ == FALSE		/* Do not include if "eqeval.c" has been   */
				/* pre-compiled and is to be linked.       */

/***************
 * SUBROUTINES *
 ***************/


/*****************************************************************************/
/* get_eq() - Gets the equation from the user and stores it into "tmpstr."   */
/*****************************************************************************/
void get_eq()
{

  /* Ask if the Equation Evaluator is needed. */
  printf("Will you use the Equation Evaluator? (Y/N) N: ");
  fgets(tmpstr, MAX_LINE_SIZE, stdin);

  if (tolower(tmpstr[0]) == 'y') {
    /* Enter the equation (130 characters max. in MS-DOS). */
    printf("Enter the equation using x, y and/or t as variables:\n----> ");
    fgets(tmpstr, MAX_LINE_SIZE, stdin);

    eqeval = TRUE;		/* Set the eqeval flag */

    if (tmpstr[0] == '\n') {
      printf("WARNING - No equation was entered.  Defaulting ");
      printf("to the pre-compiled function.\n");
      eqeval = FALSE;		/* Disable the Equation Evaluator */
    }

    /* Remove the newline character from the end of "tmpstr". */
    tmpstr[strlen(tmpstr)-1] = '\0';

  } else {
    eqeval = FALSE;		/* Reset the eqeval flag */
  }

  printf("\n");

}  /* get_eq() */


/*****************************************************************************/
/* parse_eq() - Parses the entered equation, then builds a binary tree.      */
/*****************************************************************************/
void parse_eq()
{
  int i, j, k, id, last_id, len, node_cnt, type, last_type;
  int fixed_level, actual_level, last_fixed_level;
  int al[MAX_NUM_OPS], state[MAX_NUM_OPS];
  int node_stack[MAX_NUM_OPS], stack_cnt, node_num;
  int min_level, max_level, al_tmp, state_tmp, left_cnt, right_cnt;
  double val;
  char *eq_ptr;

  /* Convert equation to lower case characters and remove white spaces */
  len = strlen(tmpstr);
  for (i=0,j=0;i<len;i++)
    if (!isspace(tmpstr[i]))
      tmpstr[j++] = tolower(tmpstr[i]);
  tmpstr[j] = '\0';

#if VERBOSE == TRUE
  printf("equation = %s\n", tmpstr);	/* feedback */
#endif

  /*
  **  Key to fixed level assignments:
  **
  **     LEVEL  |  OPERATION
  **  -------------------------
  **      -3    |  (
  **       0    |  xyt 123 sin  (variables, numbers and functions)
  **       1    |  * / %
  **       2    |  + -
  **       3    |  )
  **
  **  See precedence rules in K&R.  The lower the level, the higher the
  **  precedence (-3 is highest precedence).
  **
  **
  **  Extract the following information from the equation:
  **    id           - Operand ID			(ie:  ADD_ID)
  **    val          - Value (if id = NUM_ID)		(ie:  3.1415)
  **    fixed_level  - Fixed Level (see key above)	(ie:  -3,0,1,2,3)
  **    actual_level - Actual Level (dynamic)  		(ie:  0 = valid)
  **    type         - Element Types                    (ie:  OP_TYPE)
  **    state[]      - State of Elements                (ie:  INTERIOR)
  */

  /* initialize these variables before entering while loop */
  left_cnt     = 0;	/* counts number of left parentheses */
  right_cnt    = 0;	/* counts number of right parentheses */
  node_cnt     = 0;	/* node counter (eq_node[0] points to root element) */
  stack_cnt    = 0;	/* node stack counter */
  id           = NO_ID;	/* element ID */
  last_id      = NO_ID;	/* last element's ID */
  last_type    = 0;	/* last type */
  fixed_level  = 0;	/* element's fixed level */
  actual_level = 0;	/* dynamic actual level */
  eq_ptr       = tmpstr;/* pointer to allow easy equation string traversal */

  while (*eq_ptr != '\0') {	/* Parse entire equation string */

    last_id = id;		/* set last_ values (history effect) */
    last_fixed_level = fixed_level;
    last_type = type;		/* saves previous type */
    val = 0.0;			/* initialize val used with NUM_ID */

    /* Assign IDs to id and set fixed_level and type. */
    if ((id = ispun(&eq_ptr, &fixed_level, &type)) == NO_ID)
      if ((id = isop1(&eq_ptr, &fixed_level, &type)) == NO_ID)
        if ((id = isop2(&eq_ptr, &fixed_level, &type)) == NO_ID)
          if ((id = isvar(&eq_ptr, &fixed_level, &type)) == NO_ID)
            if ((id = isnum(&eq_ptr, &fixed_level, &type, &val)) == NO_ID)
              if ((id = isfunct(&eq_ptr, &fixed_level, &type)) == NO_ID) {
                fprintf(stderr,		/* ie: "3$4" */
                  "ERROR: Unrecognized mathematical operator encountered.\n");
                exit (-1);		/* Exit program */
              }

    /* Adjust actual level (or depth of tree nodes) */
    if (id != SKIP_ID) {
      if (last_id == LEFT_ID || last_id == RIGHT_ID)
        actual_level += fixed_level;    /* Adjust levels due to parentheses */
      else
        actual_level += fixed_level - last_fixed_level;	/* key equation */
    } else {
      fixed_level = last_fixed_level;
    }

    /* Create element nodes one at a time and initialize them. */
    if (type != PUNCT_TYPE) {
      node_cnt++;			/* increase node count */
      eq_node[node_cnt] = (struct EQ_NODE *) calloc(1, sizeof(struct EQ_NODE));
      eq_node[node_cnt]->id    = id;
      eq_node[node_cnt]->value = val;
      eq_node[node_cnt]->left  = NULL;
      eq_node[node_cnt]->right = NULL;
      al[node_cnt] = actual_level;	/* store node's actual level */

      if ((type == NUM_TYPE) || (type == VAR_TYPE))
        state[node_cnt] = LEAF;		/* store node's state */
      else
        state[node_cnt] = INTERIOR;	/* store node's state */
    }  /* if */


    /* Adjust for single argument functions, for negatives (unary "-") and */
    /* for positives (unary "+")                                           */
    if ((type == FUNCT1_TYPE) || (type == UNARY_TYPE)) {

      /* Create another node and make it identical to the previous node */
      node_cnt++;			/* increase node count */
      eq_node[node_cnt] = (struct EQ_NODE *) calloc(1, sizeof(struct EQ_NODE));
      eq_node[node_cnt]->id    = id;
      eq_node[node_cnt]->value = val;
      eq_node[node_cnt]->left  = NULL;
      eq_node[node_cnt]->right = NULL;
      al[node_cnt] = actual_level;	/* store node's actual level */
      state[node_cnt] = INTERIOR;	/* store node's state */

      /* set the previous node to hold the value zero */
      eq_node[node_cnt-1]->id    = NUM_ID;
      eq_node[node_cnt-1]->value = 0.0;
      eq_node[node_cnt-1]->left  = NULL;
      eq_node[node_cnt-1]->right = NULL;
      al[node_cnt-1] = actual_level - 3;/* set node's actual level */
      state[node_cnt-1] = LEAF;		/* set node's state */

    }  /* if */


    /* Adjust for dual argument functions */
    if (type == FUNCT2_TYPE)	/* push node number onto stack */
      node_stack[stack_cnt++] = node_cnt;


    /* Adjust for comma in dual argument functions */
    if (id == COMMA_ID) {

      /* pop node number off of the stack */
      node_num = node_stack[--stack_cnt];

      /* temporarily save all information about the FUNCT2 node */
      eq_node[0]  = eq_node[node_num];	/* save pointer */
      al_tmp      = al[node_num];	/* save actual level */
      state_tmp   = state[node_num];	/* save state */

      /* shift node information left to align sub-trees */
      for (i=node_num;i<node_cnt;i++) {
        eq_node[i] = eq_node[i+1];	/* shift node pointers */
        al[i]      = al[i+1];		/* shift actual levels */
        state[i]   = state[i+1];	/* shift states */
      }

      /* restore FUNCT2 node temporarily saved above */
      eq_node[node_cnt] = eq_node[0];	/* place FUNCT2 where comma was */
      al[node_cnt]      = al_tmp;	/* restore actual level */
      state[node_cnt]   = state_tmp;	/* restore state */

    }  /* if */

    /* Checks for illegal use of parantheses (ie:  "1+2)*(3" ) */
    if (id == LEFT_ID)
      left_cnt++;
    else if (id == RIGHT_ID)
      right_cnt++;
    if (right_cnt > left_cnt) {
      fprintf(stderr, "ERROR: Misuse of parentheses.\n");
      exit (-1);		/* Exit program */
    }

    /* Checks for too many sequential unary operators */
    if ((id == SKIP_ID) &&	/* ie: "x - - -3" and "+ -3" */
        ((last_id == SKIP_ID) || (last_type == UNARY_TYPE))) {
      fprintf(stderr, "ERROR: Too many sequential unary operators used.\n");
      exit (-1);		/* Exit program */
    }

  }  /* while */


#if VERBOSE == TRUE
  for (i=1;i<=node_cnt;i++) {	/* Feedback */
      printf("node: %2d\t", i);
      printf("AL = %3d\t", al[i]);
      printf("st = %3d\t", state[i]);
      printf("id = %d\t", eq_node[i]->id);
      if (eq_node[i]->id == NUM_ID)
        printf("val = %lg", eq_node[i]->value);
      printf("\n");
  }
#endif


  /* Check for incomplete equations */
  if (actual_level != 0) {
    if (left_cnt != right_cnt)	/* ie: "((3)" */
      fprintf(stderr, "ERROR: Parentheses are mismatched.\n");
    else			/* ie: "5+" */
      fprintf(stderr, "ERROR: Incomplete equation entered.\n");
    exit (-1);		/* Exit program */
  }


  /*
  **  Now build a tree structure from the 2-D grid.
  **  Grid:  eq_node[] x al[]  or  [1..node_cnt][min_level..max_level]
  **  
  **  Example:  (1 + x) * 3
  **
  **         <---  eq_node[]  --->
  **  actual
  **  levels
  **    3
  **    2              (root)        node 0 = * (root node.  same as node 4.)
  **    1             *              node 1 = 1
  **    0 --------- /   3 ---------  node 2 = +
  **   -1         +                  node 3 = x
  **   -2       /   \                node 4 = *
  **   -3      1     x               node 5 = 3
  **   -4
  */

  /* Find limits of tree levels.  Store in min_level and max_level. */
  min_level = INT_MAX;
  max_level = INT_MIN;
  for (i=1;i<=node_cnt;i++) {
    min_level = (al[i] < min_level) ? al[i] : min_level;
    max_level = (al[i] > max_level) ? al[i] : max_level;
  }

#if VERBOSE == TRUE
	printf("\nmin_level = %d:\n", min_level);
	printf("max_level = %d:\n", max_level);
#endif

  /* Build tree by scanning from left to right, starting at min_level. */
  for (j=min_level;j<=max_level;j++) {
    for (i=1;i<=node_cnt;i++) {

      /* Search for INTERIOR nodes and link them to LEAF nodes */
      if ((state[i] == INTERIOR) && (al[i] <= j)) {

#if VERBOSE == TRUE
	printf("\nAt node %d:\n", i);
#endif

        state_tmp = 0;		/* used for error detection */

	/* find and link left LEAF node */
        for (k=i-1;k>=1;k--) {
          if ((al[k] <= al[i]) && (state[k] == LEAF)) {
            eq_node[i]->left = eq_node[k];
            state[k] = DONE;	/* label state so as not to be used again */
            state_tmp++;	/* count when node is set to DONE */
            break;
          }
        }

#if VERBOSE == TRUE
        if (k >= 1)
	  printf("\tleft node  = %d\n", k);
#endif

	/* find and link right LEAF node */
        for (k=i+1;k<=node_cnt;k++) {
          if ((al[k] <= al[i]) && (state[k] == LEAF)) {
            eq_node[i]->right = eq_node[k];
            state[k] = DONE;	/* label state so as not to be used again */
            state_tmp++;	/* count when node is set to DONE */
            break;
          }
        }

#if VERBOSE == TRUE
        if (k <= node_cnt)
	  printf("\tright node = %d\n", k);
#endif

        if (state_tmp == 2)	/* checks that both nodes where used */
          state[i] = LEAF;	/* make INTERIOR node a LEAF node */

      }  /* if */

    }  /* for */
  }  /* for */


  /* Assign root node */
  for (i=node_cnt;i>=1;i--)
    if (al[i] == max_level) {
      eq_node[0] = eq_node[i];	/* root node is always eq_node[0] */
      if ((state_tmp == 2) || (node_cnt == 1))
        state[i] = DONE;	/* for catching errors like:  2**3 */
      break;
    }


  /* Check to see that all nodes have been used.  Error if not. */
  for (i=1;i<=node_cnt;i++) {
    if (state[i] != DONE) {	/* ie: "3 * / 4" */
      fprintf(stderr, "ERROR: Incorrect equation entered.");
      fprintf(stderr, "  Check for missing operands.\n");
      exit (-1);		/* Exit program */
    }
  }

  /* Special Case: Check for "()" */
  if (node_cnt == 0) {		/* ie:  "()" */
    fprintf(stderr, "ERROR: \"()\" is not a valid equation.\n");
    exit (-1);		/* Exit program */
  }

}  /* parse_eq() */


/*****************************************************************************/
/* ispun() - Identifies punctuation.  Returns variable ID if successful and  */
/*           advances string pointer to next element.  Also returns the      */
/*           fixed level and operand type.                                   */
/*****************************************************************************/
int ispun(str, level, type)
char **str;
int *level, *type;
{
  int id;

    switch ((*str)[0]) {
      case '(':  id = LEFT_ID;  *level = -3; break;
      case ')':  id = RIGHT_ID; *level =  3; break;
      case ',':  id = COMMA_ID; *level =  3; break;
      default:   id = NO_ID;                 break;	/* no match */
    }

    if (id != NO_ID) {
      (*str)++;		/* Advance string pointer to next element */
      *type = PUNCT_TYPE;	/* Set type */
    }

  return id;

}  /* ispun() */


/*****************************************************************************/
/* isvar() - Identifies variables.  Returns variable ID if successful and    */
/*           advances string pointer to next element.   Also returns the     */
/*           fixed level and operand type.                                   */
/*****************************************************************************/
int isvar(str, level, type)
char **str;
int *level, *type;
{
  int id;

  if (!isalpha((*str)[1])) {	/* may be a function name */

    switch ((*str)[0]) {
      case 'x':  id = VARX_ID;  break;	/* variable x */
      case 'y':  id = VARY_ID;  break;	/* variable y */
      case 't':  id = VART_ID;  break;	/* variable t */
      default:   id = NO_ID;    break;	/* no match */
    }

    if (id != NO_ID) {
      (*str)++;		/* Advance string pointer to next element */
      *level = 0;	/* Set level */
      *type = VAR_TYPE;	/* Set type */
    }

  } else
    id = NO_ID;

  return id;

}  /* isvar() */


/*****************************************************************************/
/* isop1() - Identifies operators.  Returns operand ID if successful and     */
/*           advances string pointer to next element.  Also returns the      */
/*           fixed level and operand type.                                   */
/*****************************************************************************/
int isop1(str, level, type)
char **str;
int *level, *type;
{
  int id;

  switch ((*str)[0]) {
    case '*':  id = MULT_ID; break;	/* multiplication */
    case '/':  id = DIV_ID;  break;	/* division */
    case '%':  id = REM_ID;  break;	/* integer remainder (see fmod) */
    default:   id = NO_ID;   break;	/* no match */
  }

  if (id != NO_ID) {
    (*str)++;		/* Advance string pointer to next element */
    *level = 1;		/* Set level */
    *type = OP_TYPE;	/* Set type */
  }

  return id;

}  /* isop1() */


/*****************************************************************************/
/* isop2() - Identifies operators.  Returns operand ID if successful and     */
/*           advances string pointer to next element.  Also returns the      */
/*           fixed level and operand type.                                   */
/*****************************************************************************/
int isop2(str, level, type)
char **str;
int *level, *type;
{
  int id;

  switch ((*str)[0]) {
    case '+':  id = ADD_ID;  break;	/* addition */
    case '-':  id = SUB_ID;  break;	/* subtraction */
    default:   id = NO_ID;   break;	/* no match */
  }

  /* Discard second "+" or "-" in "+ +", "+ -", "- +" and "- -" cases */
  if ((id != NO_ID) && (((*str)[-1] == '+') || ((*str)[-1] == '-'))) {
    id = SKIP_ID;	/* Assign skip ID */
    *type = PUNCT_TYPE;	/* Set type so that last_fixed_level will be zero */
    (*str)++;		/* Advance string pointer to next element */
  }

  if ((id == ADD_ID) || (id == SUB_ID)) {

    /* Check for "+ -" case */
    if ((id == ADD_ID) && ((*str)[1] == '-'))
      id = SUB_ID;

    /* Check for "- -" case */
    else if ((id == SUB_ID) && ((*str)[1] == '-'))
      id = ADD_ID;

    /* NOTE: the "+ +" and "- +" cases are defaults */

    *type = OP_TYPE;	/* Set type */

    /* Check for unary "+" and "-" */
    if ((*str == tmpstr) ||	/* first character of equation is a unary */
        ((*str)[-1] == '(') ||	/* unary following a left parenthesis     */
        ((*str)[-1] == ','))	/* unary following a comma in a function  */
      *type = UNARY_TYPE;

#if ANSI == FALSE		/* New to ANSI C (ie: sin(+3.14)) */
    /* Check for positives (unary "+") instead of addition. */
    if ((id == ADD_ID) && (*type == UNARY_TYPE))
      fprintf(stderr, "WARNING:  Unary + is new to ANSI C\n");
#endif  /* ANSI */

    (*str)++;		/* Advance string pointer to next element */
    *level = 2;		/* Set level */

  }

  return id;

}  /* isop2() */


/*****************************************************************************/
/* isnum() - Identifies numbers.  Returns NUM_ID if successful and stores    */
/*           the number into the variable num.  Also advances the string     */
/*           pointer to next element and returns the fixed level and operand */
/*           type.                                                           */
/*****************************************************************************/
int isnum(str, level, type, num)
char **str;
int *level, *type;
double *num;
{
  int id;
  char *endptr;

  if (isdigit((*str)[0]) || (*str)[0] == '.') {

    /* Convert element into a floating-point number */
    *num = strtod(*str, &endptr);

    /* Checks result for overflow, underflow, and no conversion */
    if (*num == HUGE_VAL || *num == -HUGE_VAL ||
       (*num == 0.0 && ((*str)[0] != '0' ||
       ((*str)[0] == '.' && (*str)[1] != '0')))) {	/* ie: "3.1e-r" */
      fprintf(stderr, "ERROR: Invalid number in equation.\n");
      exit (-1);
    } else {
      id = NUM_ID;	/* Number is valid */
      *str = endptr;	/* Advance string pointer to next element */
      *level = 0;	/* Set level */
      *type = NUM_TYPE;	/* Set type */
    }

  } else
    id = NO_ID;		/* Not a valid number */

  return id;

}  /* isnum() */


/*****************************************************************************/
#define	MAX_FUN_SZ	5		/* Max. size of a C function name. */


/*****************************************************************************/
/* isfunct() - Identifies functions.  Returns function ID if successful and  */
/*             advances string pointer to next element.  Also returns the    */
/*             fixed level and operand type.                                 */
/*****************************************************************************/
int isfunct(str, level, type)
char **str;
int *level, *type;
{
  int id, i;
  char tmp[MAX_FUN_SZ+1];

  if (isalpha((*str)[0])) {	/* First character must be a letter */

    for (i=0; isalnum((*str)[i]); i++) { /* Put function name into tmp */
      tmp[i] = (*str)[i];
      if (i > MAX_FUN_SZ) {	/* Check for excessively large strings */
        fprintf(stderr, "ERROR: Invalid function in equation (1).\n");
        exit (-1);		/* ie: "tangent(x)" */
      }
    }
    tmp[i] = '\0';

    /* Match function name.  Assign it an id and type. */
    if (strncmp(tmp, "abs", MAX_FUN_SZ) == 0) {
      id = ABS_ID;	/* integer absolute value (see fabs) */
      *type = FUNCT1_TYPE;

    } else if (strncmp(tmp, "acos", MAX_FUN_SZ) == 0) {
      id = ACOS_ID;	/* arccosine */
      *type = FUNCT1_TYPE;

    } else if (strncmp(tmp, "atan", MAX_FUN_SZ) == 0) {
      id = ATAN_ID;	/* arctangent */
      *type = FUNCT1_TYPE;

    } else if (strncmp(tmp, "atan2", MAX_FUN_SZ) == 0) {
      id = ATAN2_ID;	/* arctangent */
      *type = FUNCT2_TYPE;

    } else if (strncmp(tmp, "ceil", MAX_FUN_SZ) == 0) {
      id = CEIL_ID;	/* ceiling */
      *type = FUNCT1_TYPE;

    } else if (strncmp(tmp, "cos", MAX_FUN_SZ) == 0) {
      id = COS_ID;	/* cosine */
      *type = FUNCT1_TYPE;

    } else if (strncmp(tmp, "cosh", MAX_FUN_SZ) == 0) {
      id = COSH_ID;	/* hyperbolic cosine */
      *type = FUNCT1_TYPE;

    } else if (strncmp(tmp, "exp", MAX_FUN_SZ) == 0) {
      id = EXP_ID;	/* exponential e^x */
      *type = FUNCT1_TYPE;

    } else if (strncmp(tmp, "fabs", MAX_FUN_SZ) == 0) {
      id = FABS_ID;	/* floating-point absolute value */
      *type = FUNCT1_TYPE;

    } else if (strncmp(tmp, "floor", MAX_FUN_SZ) == 0) {
      id = FLOOR_ID;	/* floor */
      *type = FUNCT1_TYPE;

    } else if (strncmp(tmp, "fmod", MAX_FUN_SZ) == 0) {
      id = FMOD_ID;	/* floating-point modulus */
      *type = FUNCT2_TYPE;

    } else if (strncmp(tmp, "frexp", MAX_FUN_SZ) == 0) {
      id = FREXP_ID;	/* normalized fraction */
      *type = FUNCT1_TYPE;

    } else if (strncmp(tmp, "ldexp", MAX_FUN_SZ) == 0) {
      id = LDEXP_ID;	/* x * 2^n */
      *type = FUNCT2_TYPE;

    } else if (strncmp(tmp, "log", MAX_FUN_SZ) == 0) {
      id = LOG_ID;	/* natural logarithm */
      *type = FUNCT1_TYPE;

    } else if (strncmp(tmp, "log10", MAX_FUN_SZ) == 0) {
      id = LOG10_ID;	/* base 10 logarithm */
      *type = FUNCT1_TYPE;

    } else if (strncmp(tmp, "modf", MAX_FUN_SZ) == 0) {
      id = MODF_ID;	/* fractions */
      *type = FUNCT1_TYPE;

    } else if (strncmp(tmp, "pow", MAX_FUN_SZ) == 0) {
      id = POW_ID;	/* exponentiation x^y */
      *type = FUNCT2_TYPE;

    } else if (strncmp(tmp, "sin", MAX_FUN_SZ) == 0) {
      id = SIN_ID;	/* sine */
      *type = FUNCT1_TYPE;

    } else if (strncmp(tmp, "sinh", MAX_FUN_SZ) == 0) {
      id = SINH_ID;	/* hyperbolic sine */
      *type = FUNCT1_TYPE;

    } else if (strncmp(tmp, "sqrt", MAX_FUN_SZ) == 0) {
      id = SQRT_ID;	/* square root */
      *type = FUNCT1_TYPE;

    } else if (strncmp(tmp, "tan", MAX_FUN_SZ) == 0) {
      id = TAN_ID;	/* tangent */
      *type = FUNCT1_TYPE;

    } else if (strncmp(tmp, "tanh", MAX_FUN_SZ) == 0) {
      id = TANH_ID;	/* hyperbolic tangent */
      *type = FUNCT1_TYPE;

    } else {
      id = NO_ID;	/* no match found */
      fprintf(stderr, "ERROR: Invalid function in equation (2).\n");
      exit (-1);
    }

    *str = &(*str)[i];	/* Advance string pointer to next element */
    *level = 0;		/* Set level */

  } else
    id = NO_ID;		/* Can not be a function */

  return id;

}  /* isfunct() */

#if OLD_UNIX_OS == TRUE			/* For older UNIX C compilers */

/*****************************************************************************/
/* eval_eq() - Evaluates the parsed equation after parse_eq() is run.        */
/*             Accepts f(x), f(x,y) and f(x,y,t).                            */
/*             Use this eval_eq() instead of the below eval_eq() if your C   */
/*             compiler does not have <stdarg.h>, but does have <varargs.h>. */
/*****************************************************************************/

/*
**  eval_eq() - OPTION #1 -- Uses #include <varargs.h>
*/

double eval_eq(x, va_alist)
double x;
va_list va_alist;
/* or   (w/out a ';')
va_decl
*/
{
  va_list ap;		/* Points to each unnamed arg in turn */

  eq_eval_x = x;	/* Makes x global so traverse_tree() can use it. */

  va_start(ap);		/* Make ap point to 1st unnamed arg */
  eq_eval_y = va_arg(ap, double);	/* Makes y global too */
  eq_eval_t = va_arg(ap, double);	/* Makes t global too */
  va_end(ap);		/* clean up when done */

  /*
  **  Note:  eq_node[0] points to the root node.
  **         eq_node[0]->value contains the final result of f().
  */

  /* Start at the root node of the binary tree to do a post-order traversal. */
  traverse_tree(eq_node[0]);

  return (eq_node[0]->value);	/* Return final result. */

}  /* eval_eq() */

#else  /* OLD_UNIX_OS */		/* For ANSI C compilers */

/*****************************************************************************/
/* eval_eq() - Evaluates the parsed equation after parse_eq() is run.        */
/*             Accepts f(x), f(x,y) and f(x,y,t).                            */
/*****************************************************************************/

/*
**  eval_eq() - OPTION #2 -- Uses #include <stdarg.h>
*/

#if ANSI_FUNCT == TRUE
double eval_eq(double x, ...)
#else
double eval_eq(x, ...)
double x;
#endif
{
  va_list ap;		/* Points to each unnamed arg in turn */

  eq_eval_x = x;	/* Makes x global so traverse_tree() can use it. */

  va_start(ap, x);	/* Make ap point to 1st unnamed arg */
  eq_eval_y = va_arg(ap, double);	/* Makes y global too */
  eq_eval_t = va_arg(ap, double);	/* Makes t global too */
  va_end(ap);		/* clean up when done */

  /*
  **  Note:  eq_node[0] points to the root node.
  **         eq_node[0]->value contains the final result of f().
  */

  /* Start at the root node of the binary tree to do a post-order traversal. */
  traverse_tree(eq_node[0]);

  return (eq_node[0]->value);	/* Return final result. */

}  /* eval_eq() */

#endif	/* OLD_UNIX_OS */

/*****************************************************************************/

/* Make code in traverse_tree() more readable with these simple defines. */
#define NPV	node_ptr->value
#define NPLV	node_ptr->left->value
#define NPRV	node_ptr->right->value

/*****************************************************************************/
/* traverse_tree() - Does a post-order traversal of the binary tree, storing */
/*                   the results of all sub-trees into "value" associated    */
/*                   with that node.  Is called recursively and may require  */
/*                   a larger stack when compiled.                           */
/*****************************************************************************/
void traverse_tree(node_ptr)
struct EQ_NODE *node_ptr;
{
  if (node_ptr != NULL) {		/* Only leaf nodes are NULL  */

    traverse_tree(node_ptr->left);	/* Search left sub-trees     */

    traverse_tree(node_ptr->right);	/* Search right sub-trees    */

    switch (node_ptr->id) {		/* Evaluate sub-tree's value */

      case NUM_ID:    break;  /* Work was already done in parse_eq(). */
      case VARX_ID:   NPV = eq_eval_x;                       break;
      case VARY_ID:   NPV = eq_eval_y;                       break;
      case VART_ID:   NPV = eq_eval_t;                       break;

      case ADD_ID:    NPV = NPLV + NPRV;                     break;
      case SUB_ID:    NPV = NPLV - NPRV;                     break;
      case MULT_ID:   NPV = NPLV * NPRV;                     break;
      case DIV_ID:    NPV = NPLV / NPRV;                     break;
      case REM_ID:    NPV = fmod  (NPLV, NPRV); /*subst.*/   break;

      case ABS_ID:    NPV = fabs  (NPRV);  /* substitute */  break;
      case ACOS_ID:   NPV = acos  (NPRV);                    break;
      case ASIN_ID:   NPV = asin  (NPRV);                    break;
      case ATAN_ID:   NPV = atan  (NPRV);                    break;
      case ATAN2_ID:  NPV = atan2 (NPLV, NPRV);              break;
      case CEIL_ID:   NPV = ceil  (NPRV);                    break;
      case COS_ID:    NPV = cos   (NPRV);                    break;
      case COSH_ID:   NPV = cosh  (NPRV);                    break;
      case EXP_ID:    NPV = exp   (NPRV);                    break;
      case FABS_ID:   NPV = fabs  (NPRV);                    break;
      case FLOOR_ID:  NPV = floor (NPRV);                    break;
      case FMOD_ID:   NPV = fmod  (NPLV, NPRV);              break;
      case FREXP_ID:  NPV = frexp (NPRV, (int *) tmpstr);    break;
      case LDEXP_ID:  NPV = ldexp (NPLV, (int) NPRV);        break;
      case LOG_ID:    NPV = log   (NPRV);                    break;
      case LOG10_ID:  NPV = log10 (NPRV);                    break;
      case MODF_ID:   NPV = modf  (NPRV, (double *) tmpstr); break;
      case POW_ID:    NPV = pow   (NPLV, NPRV);              break;
      case SIN_ID:    NPV = sin   (NPRV);                    break;
      case SINH_ID:   NPV = sinh  (NPRV);                    break;
      case SQRT_ID:   NPV = sqrt  (NPRV);                    break;
      case TAN_ID:    NPV = tan   (NPRV);                    break;
      case TANH_ID:   NPV = tanh  (NPRV);                    break;
      default:        NPV = 0.0;
                      fprintf(stderr, "ERROR: In parser.\n"); break;
    }  /* switch */

  }  /* if */
}  /* traverse_tree() */


/*****************************************************************************/

#endif	/* EQEVAL_OBJ */

/******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
*	Copyright (C) 1991, Harold A. Toomey, All Rights Reserved.            *
******************************************************************************/
