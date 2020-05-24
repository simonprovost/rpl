/* suffix.c */
/* ########################### */
/* #           rpl           # */
/* #     by Joe Laffey       # */
/* # joe@laffeycomputer.com  # */
/* #                         # */
/* ########################### */

/* 
 # Last Modified:  $Date: 2001/09/08 20:38:29 $                  
 # by:             $Author: joe $              
*/


/*  rpl v1.3.1b1 by Joe Laffey, LAFFEY Computer Imaging. 							*/
/*  Visit http://www.laffeycomputer.com for updates 								*/
/*  This software is copyright 1998, 1999, 2000, 2001 by Joe Laffey.   				*/
    
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

#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <stdio.h>

#include "suffix.h"


/*****************************/
/* Check to see if we have   */
/* a file with a bad suffix. */
/* i.e. one NOT in our list  */
int SuffixIsBad(char* fName, char** suffixes, int* suffixLens, int minSuffixLength, int numSuffixes)
{
	int fileNameLen;
	int i;
	
	
	 fileNameLen = strlen(fName);
  
     if(fileNameLen>=minSuffixLength)	/*can't match if this is false */
     {
		for (i=0;i<numSuffixes;i++)
		{
		 
		  
		   if (!strncmp(&fName[fileNameLen-suffixLens[i]], suffixes[i], (size_t)suffixLens[i]))
			 return 0;
		}
      }
      
      return 1; /* the suffix on the file _IS_ bad  not found */
}


void AddSuffix(char* theSuffix, char** suffixes, int* suffixLens, int* minSuffixLength, int* numSuffixes)
{
	int suffixLen;
	

   
   if(*numSuffixes < MAX_SUFFIXES)
   {
		suffixLen = strlen(theSuffix);

		if(suffixLen < *minSuffixLength || *numSuffixes == 0) 
			*minSuffixLength = suffixLen;
		
		suffixes[*numSuffixes] = malloc((size_t)suffixLen+1);
		
   
      
	 if(suffixes[*numSuffixes] == NULL)
	  {
			fprintf(stderr, "Unable to allocate memory.\n");
			exit(EX_OSERR);
	  }
      
      strcpy(suffixes[*numSuffixes],theSuffix);
      suffixLens[*numSuffixes] = strlen(theSuffix);
      *numSuffixes += 1;
   }
   else
   	fprintf(stderr,"rpl: Maximum of %d suffixes exceeded. Suffix \"%s\" ignored\n",MAX_SUFFIXES,theSuffix);

	
}
