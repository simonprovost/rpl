/* replace.h */
/* ########################### */
/* #           rpl           # */
/* #     by Joe Laffey       # */
/* # joe@laffeycomputer.com  # */
/* #                         # */
/* ########################### */

/* 
 # Last Modified:  $Date: 2008/04/24 13:05:58 $                  
 # by:             $Author: joe $              
*/


/*  rpl v1.4.1 by Joe Laffey, LAFFEY Computer Imaging. 							*/
/*  Visit http://www.laffeycomputer.com for updates 								*/
/*  This software is copyright 1998-2008 by Joe Laffey.   				*/
    
/*  Permission is granted to any individual, institution, or company to use, 		*/
/*  copy, or redistribute rpl in source code or binary form so long as it is 		*/
/*  not modified in any way (beyond modifications required to compile or */
/*  "package"), and it is not sold by itself for a profit. 							*/
/*  Permission is also granted to bundle rpl in software distributions which 		*/
/*  are sold for a profit (e.g. CD-ROMs, etc.), as long as there are at least 		*/
/*  ten programs included in the distribution.  									*/
/*  In other words, please do NOT release updates or modified verions yourself.     */
/*  If you modify the source code and would like to see your changes incorporated 	*/
/*  please submit your source code to software@laffeycomputer.com 					*/
/*  Please report bugs to that address as well. 									*/

/*  rpl IS PROVIDED AS IS AND COMES WITH NO WARRANTY OF ANY KIND, EITHER EXPRESSED 	*/
/*  OR IMPLIED. IN NO EVENT WILL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY DAMAGES 	*/
/*  RESULTING FROM THE USE OF THIS SOFTWARE. 										*/


#ifndef REPLACE_H
#define REPLACE_H

#include "suffix.h"

#undef DEBUG
#undef DEBUG2


#define	FIND_FIRST_CHAR			2000		/*  These numbers are arbitrary */
#define FINISH_MATCH			3000		/*  They just represent the state */
#define MATCH_STRING			4000		/*  of our search for chars. */



#define MAX_STRING_LENGTH		2048		/* The maximum size of the search or replace strings. */


#define MAX_INPUT_LINE			1024




/* Externals */
extern char *optarg;   
extern int optind;

/*Globals*/
static	int	interrupted = 0;



/* For a linked list to hold filename to search */
/*  We use a linked list to avoid recursion... */
typedef struct fileList {
							char*				name;
							struct fileList*	next;
						}fileList;
						

#endif

/* This is a simple macro to call malloc() and die if it returns null */
#define MALLOC_FAIL_STRING   ("\n" PACKAGE " Unable to allocate memory!")
#define MALLOC_AND_CHECK( x, y )       (x)=malloc((y));if(x==NULL){fprintf(stderr,"\n%s --> %s:%d\n", MALLOC_FAIL_STRING, __FILE__, __LINE__ );exit(EX_OSERR);}
	

/*  Function Prototypes */

static void DoInterrupt();

static fileList*	GetNextItem( fileList* );

static void EchoModes(int, int, int, int, unsigned char*, unsigned char*, unsigned char*, unsigned char*);

static void EchoFeedback(int, int, int, char*);

static void SetPerms(struct stat, int, int, char*, int* );