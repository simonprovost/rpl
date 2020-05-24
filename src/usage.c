/* usage.c */
/* ########################### */
/* #           rpl           # */
/* #     by Joe Laffey       # */
/* # joe@laffeycomputer.com  # */
/* #                         # */
/* ########################### */

/* 
 # Last Modified:  $Date: 2008/04/24 13:05:46 $                  
 # by:             $Author: joe $              
*/


/*  rpl v1.4.1 by Joe Laffey, LAFFEY Computer Imaging. 							*/
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



#include <sysexits.h>
#include <stdio.h>
#include <stdlib.h>

#include "suffix.h"


/*****************************/
/*  Print out what we can do */
void usage(void)
{
	fprintf(stderr, "------------------------------------------------\n");
	fprintf(stderr, PACKAGE " " VERSION  " by Joe Laffey, LAFFEY Computer Imaging.\n");
	fprintf(stderr, PACKAGE " " VERSION  " by Joe Laffey, LAFFEY Computer Imaging.\n");
	fprintf(stderr, "Visit http://www.laffeycomputer.com/ for updates.\n");
	fprintf(stderr, "This software comes with ABSOLUTELY NO WARRANTY. rpl -L for license.\n");   
    fprintf(stderr, "Usage:  rpl [-iwRspfdtxe [-q | -v] ] <old_string> <new_string> <target_file(s)>\n");
	fprintf(stderr, "        ( Put strings in single quotes (\') )\n");
	fprintf(stderr, "        -i   Ignore case of old_string\n");
	fprintf(stderr, "        -w   Whole words (old_string bounded by white space in file)\n");	
	fprintf(stderr, "        -q   Quiet mode (no output at all)\n");
	fprintf(stderr, "        -v   Verbose mode (lots of output)\n");
	fprintf(stderr, "        -R   Search directories recursively\n");
	fprintf(stderr, "        -x   Specify file suffix to match (e.g. '.html')\n");
	fprintf(stderr, "             Only files with this suffix will be searched.\n");
	fprintf(stderr, "             You may specify up to %d suffixes. (e.g. -x'.html' -x'.jsp' ...)\n",MAX_SUFFIXES);
	fprintf(stderr, "        -p   Prompt to modify each file\n");
	fprintf(stderr, "        -s   Simulation mode (lists files that would be changed)\n");
	fprintf(stderr, "        -e   Honor string Escapes in old_string and new_string\n");
	fprintf(stderr, "             (e.g.'\\n', '\\r', '\\\\', '\\015' (octal), '0x0d' (hex) etc.)\n");
	fprintf(stderr, "        -f   Force overwriting files when permissions cannot be matched\n");
	fprintf(stderr, "        -d   Don't change modification times of altered files\n");
	fprintf(stderr, "        -t   Use value of $TMPDIR for temp files instead of original file dir\n");
	fprintf(stderr, "        -L   Display the software license\n");
	fprintf(stderr, "        -h   Display this help screen\n");
	fprintf(stderr, "Returns: 0 on success, system error code on failure.\n");
	fprintf(stderr, "Note: If you are using a \"-\" in either the old_string or then ew_string\n");
	fprintf(stderr, "      you must put a \"--\" as your last option before the strings.\n");
	exit(EX_USAGE);
}



/*****************************/
/*  Print out the license */
void License(void)
{
	fprintf(stderr, "------------------------------------------------\n");
	fprintf(stderr, PACKAGE " " VERSION " by Joe Laffey, LAFFEY Computer Imaging.\n");
	fprintf(stderr, PACKAGE " " VERSION " New feature by Provost Simon (excluded-directories)\n");
	fprintf(stderr, "Visit http:// www.laffeycomputer.com/ for updates.\n");
	fprintf(stderr, "This software is copyright 1998, 1999, 2000, 2001 by Joe Laffey.\n\n");   
    
	fprintf(stderr, "Permission is granted to any individual, institution, or company to use, copy,\n");
	fprintf(stderr, "or redistribute rpl in source code or binary form so long as it is not\n");
	fprintf(stderr, "modified in any way (beyond modifications required to compile or \"package\"),\n");
	fprintf(stderr, "and it is not sold by itself for profit.\n\n");

	fprintf(stderr, "Permission is also granted to bundle rpl in software distributions which\n");
	fprintf(stderr, "are sold for a profit (e.g. CD-ROMs, etc.), as long as there are at least \n");
	fprintf(stderr, "ten programs included in the distribution.\n\n");

	fprintf(stderr, "If you modify the source code and would like to see your changes incorporated\n");
	fprintf(stderr, "please submit your source code to software@laffeycomputer.com\n");
	fprintf(stderr, "Please report bugs to that address as well.\n\n");

	fprintf(stderr, "rpl IS PROVIDED AS IS AND COMES WITH NO WARRANTY OF ANY KIND, EITHER EXPRESSED\n");
	fprintf(stderr, "OR IMPLIED;  without even the implied warranty of MERCHANTABILITY or FITNESS\n");
	fprintf(stderr, "FOR A PARTICULAR PURPOSE. IN NO EVENT WILL THE COPYRIGHT HOLDERS OR\n");
	fprintf(stderr, "CONTRIBUTORS BE LIABLE FOR ANY DAMAGES RESULTING FROM THE USE OF THIS SOFTWARE.\n\n");


	exit(EX_USAGE);
}



