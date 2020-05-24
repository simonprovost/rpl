/* replace.c */
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
/*  In other words, please do NOT release updates or modified versions yourself.     */
/*  If you modify the source code and would like to see your changes incorporated 	*/
/*  please submit your source code to software@laffeycomputer.com 					*/
/*  Please report bugs to that address as well. 									*/

/*  rpl IS PROVIDED AS IS AND COMES WITH NO WARRANTY OF ANY KIND, EITHER EXPRESSED 	*/
/*  OR IMPLIED. IN NO EVENT WILL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY DAMAGES 	*/
/*  RESULTING FROM THE USE OF THIS SOFTWARE. 										*/




#include <sys/types.h>
#include <utime.h>
#include <sys/stat.h>
#include <sys/errno.h> 
#include <sys/param.h>
#include <errno.h>
#include <sysexits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/uio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

#include "replace.h"
#include "suffix.h"
#include "estrcpy.h"
#include "usage.h"

/****************************/
/* UTILS (SIMPLE LINKED LIST). */
typedef struct stringData {
    char *s;
    struct stringData *next;
} Node;

Node *createNode(char *s) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->s = s;
    newNode->next = NULL;
    return newNode;
}

void insert(Node **link, Node *newNode) {
    newNode->next = *link;
    *link = newNode;
}

void freeList(Node* head)
{
	Node* tmp;

   while (head != NULL)
    {
       tmp = head;
       head = head->next;
       free(tmp);
    }
}

/****************************/
/*  Get if current Directory is not excluded by the user.   */
int isExcludedDirectory(fileList *currentItem, char **argv)
{
	Node *head = NULL;
	Node *tail = NULL;
	Node *n;

	int i;

	for (int i = 0; argv[i] != NULL; i++) {
		if (strcmp(argv[i], "--exclude-directories") == 0) {
			for (int j = i; strcmp(argv[j], "--end-excludes-directories") != 0; j++) {
				n = createNode(argv[j]);
				insert(&head, n);
			}
		}
	}
	while (head != NULL) {
		if (strcmp(currentItem->name, head->s) == 0) {
			freeList(head);
			return 1;
		}
        head = head->next;
    }
	freeList(head);
	return 0;
}

int main(int argc, char** argv)
{

	register 	int 	i;
	register 	int		currentChar;
	register 	int		prevChar;
	register 	int		currentState;
			
	char		*suffixes[MAX_SUFFIXES];
	int			suffixLens[MAX_SUFFIXES];
	int			minSuffixLength =0;
	int			numSuffixes = 0;
		
	int		verbose = 0;				/*							*/
	int		force = 0;					/*		These are the		*/
	int		useTmp = 0;					/*		modes from the		*/
	int		quiet = 0;					/*		command-line		*/
	int		whole_words = 0;			/*		switches.			*/
	int		recursive = 0;				/*							*/
	int		ignore_case = 0;			/*							*/
	int		keepDates = 0;				/*							*/
	int		useSuffixes = 0;			/*							*/
	int		escapeInput = 0; 	/*should we escape stuff like \n \r \t; etc? */
	int		doneFlag;
	int		stringChanged = 0;
	int		simulationMode = 0;
	int		prompt = 0;
	int		replaceStringLength;
	int		pathLength;
	int		ch;
	int		good;
	int		seen =0;
	int		skipIt = 0;
	int		outfile_fd;
	int		myLen;
	int		oldStrLen; 	/* length of old (search) string */
	

		
	unsigned long 	fileMatches = 0;	/* These are counts for tallies */
	unsigned long	totalMatches = 0;
	unsigned long	filecount = 0;
	unsigned long	lineCount = 1;
		
	unsigned	char	searchString[MAX_STRING_LENGTH];	/* The actual string we are looking for */
	unsigned	char	replaceString[MAX_STRING_LENGTH];	/* The string we will use to replace the search string */
						
	unsigned	char	replaceBuffer[MAX_STRING_LENGTH];	/* holds the string that gets written to the modified file. */
															/* we use this so we can write out new lines inplace of spaces, etc. */

	unsigned	char	*orgSearchString;	/* The original unescaped search string */
	unsigned	char	*orgReplaceString;/* The original unescaped replace string */
						
	unsigned	char	buff[MAX_STRING_LENGTH];		/* Buffer we read data into */
	unsigned	char	line[MAX_INPUT_LINE];

	char 	absPathIn[MAXPATHLEN];	/* absolute path of the original file */
	char	absPathOut[MAXPATHLEN];	/*absolute path of the temp file we are writing to */
			

		
	
	FILE	*INFILE;
	FILE	*OUTFILE;
	
	struct stat		fileStat;
				
	fileList 		*firstItem;
	fileList		*currentItem;
	fileList		*nextItem;
	fileList		*oldItem;
					
	DIR		*dirp;
	
	struct dirent	*dp;
	

	


	while ((ch = getopt(argc, argv, "iwqvRLx:hpsftde")) != -1)		/* get our options */
		switch(ch) {
		case 'i':
				 ignore_case = 1;
				 break;
		case 'v':
				verbose = 1;
				break;
		case 'q':
				quiet = 1;
				break;
		case 'e':
				escapeInput = 1;
				break;
		case 'w':
				whole_words = 1;
				break;
		case 'R':
				recursive = 1;
				break;
		case 'L':
				License();
				break;
		case 'p':
				prompt = 1;
				break;
		case 's':
				simulationMode = 1;
				break;
		case 'f':
				force = 1;
				break;
		case 't':
				useTmp = 1;
				break;
		case 'd':
				keepDates = 1;
				break;
		case 'x':
				useSuffixes = 1;
				AddSuffix(optarg, suffixes, suffixLens, &minSuffixLength, &numSuffixes);
				break;
		case 'h':
		default:
				 usage();
	 }
	 argc -= optind;
	 argv += optind;

	if( argc < 3 )						/* We Need at least three arguments */
		usage();
		
	if(argv[0] == 0)					/*  old_string can't be null */
		usage();
	
	for(i=2; strcmp(argv[i], "--exclude-directories") != 0; i++)
	{
		if(stat(argv[i], &fileStat))	/* be sure files exist */
		{	
			fprintf(stderr, "\n" PACKAGE ": File \"%s\" not found.\n", argv[i] );
			exit(EX_DATAERR);
		}	
	}

	oldStrLen = strlen(argv[0]);
	if( oldStrLen < MAX_STRING_LENGTH-1 )
	{
		if(escapeInput)
		{
			estrcpy((char*)searchString,argv[0]);
			MALLOC_AND_CHECK(orgSearchString, (size_t)oldStrLen+1);

			/* put the original search string into orgSearchString */
			strcpy((char*)orgSearchString,argv[0]);
		}
		else
		{
			strcpy((char*)searchString,argv[0]);		/* old_string */
		}
		
	}
	else
	{
		fprintf( stderr, "\n" PACKAGE ": old_string is too long!\nMaximum string length is %d.", MAX_STRING_LENGTH);
		exit(EX_DATAERR);
	}

	argv++;
	argc--;

	if(argv[0] != 0)	/* Ok for replacestring to be null */
	{
		myLen = strlen(argv[0]);
		if( myLen < MAX_STRING_LENGTH-1 )
		{
			if(escapeInput)
			{
				estrcpy((char*)replaceString,argv[0]);
				MALLOC_AND_CHECK(orgReplaceString, (size_t)myLen+1);

				/* put original replace string into orgReplaceString */
				strcpy((char*)orgReplaceString,argv[0]);
			}
			else
				strcpy((char*)replaceString,argv[0]);	/*  but can't strcpy it if it is!! */
				
			
		}
		else
		{
			fprintf( stderr, "\n" PACKAGE ": new_string is too long!\nMaximum string length is %d.", MAX_STRING_LENGTH);
			exit(-1);
		}

	}
	else
		replaceString[0] = '\0';			/* the solution... */
		
	replaceStringLength = strlen((char*)replaceString);
	
	strcpy((char*) replaceBuffer, (char*)replaceString); 	/*  replaceBuffer lets us change the text that gets */
															/* written in the case where a match is over a line break. */
															/* This lets us replace a space with a line break, etc. */

	argv++; /*  Move down... */
	argc--;	/*  Another cup of tea... */


	/* Set up our linked list */
	MALLOC_AND_CHECK(firstItem, sizeof(fileList));
	
	
	currentItem = firstItem;
	
	for(i=0; i<argc; )/* i++ below */
	{
		
		pathLength = strlen(argv[i]);
		
		if(argv[i][pathLength-1] == '/')	/*drop trailing slash */
			argv[i][pathLength-1] = '\0';
		
		
		MALLOC_AND_CHECK( currentItem->name, strlen(argv[i])+3); /* length + / null */
		
		
		strcpy( currentItem->name, argv[i]);
		
		i++;
		
		if( i < argc )
		{
   			MALLOC_AND_CHECK( nextItem, sizeof(fileList) );
			
			currentItem->next = nextItem;
			
			currentItem = nextItem;
		}
		else
		{
			currentItem->next = 0;
			break;
		}
	
	}
	

	
	if(replaceString[0] == '\0')	/* Try to protect users from deleteing strings */
	{
		if( quiet == 0)
		{
			fprintf(stderr, "Really DELETE all occurences of %s ",(escapeInput)?orgSearchString:searchString);
			if(ignore_case == 1)
			{
				fprintf(stderr,"(ignoring case)? (Y/[N]) ");
				
			}
			else
			{
				fprintf(stderr,"(case sensitive)? (Y/[N]) ");
			}
	
			fgets((char*)line, 255, stdin);	/*read in response */
				
			
			if( line[0] != 'y' && line[0] != 'Y' )
			{
				fprintf(stderr,"\n" PACKAGE ":  User cancelled operation.\n");
				exit(EX_TEMPFAIL);
			}
		}
	}


	/*  Set up our Interrupt (control-C) handler... */
	signal(SIGINT, DoInterrupt);


	/* Print out what we are doing to stderr for the user to see */
	EchoModes(escapeInput, ignore_case, whole_words, simulationMode, searchString, replaceString, orgSearchString, orgReplaceString);



	/* Start at the beginning */
	currentItem = firstItem;
	
	 /* CONSTANTCONDITION */
	while( 1 )	/* loop forever... breaking is at the end of the loop if there are no */
	{			/* more items in our linked list... */
		if (strcmp(currentItem->name, "--exclude-directories") == 0 || strcmp(currentItem->name, "--end-excludes-directories") == 0) {
			currentItem = GetNextItem(currentItem);
			if(currentItem == NULL)
				break;	/* break if there are no more files */
			continue;
		}
		if(realpath(currentItem->name, absPathIn) == 0) 	/* get the absolute path to the file */
		{
			fprintf(stderr, "\n" PACKAGE ": Could not get realpath of %s.\n" PACKAGE ": Error: %s\n" PACKAGE ": SKIPPING %s\n\n", absPathIn, strerror(errno), absPathIn);
			
			currentItem = GetNextItem(currentItem);
			if(currentItem == NULL)
				break;	/* break if there are no more files */
				
						
			continue;
		}

		
		
		if(stat(absPathIn, &fileStat))		/* read the original permissions so our new file can match */
		{	
			fprintf(stderr, "\n" PACKAGE ": Unable to read permissions of %s.\n" PACKAGE ": Error: %s\n" PACKAGE ": SKIPPING %s\n\n!",absPathIn, strerror(errno), absPathIn );
			
			currentItem = GetNextItem(currentItem);
			if(currentItem == NULL)
				break;	/* break if there are no more files */
				
						
			continue;
			
		}
	
	
	
		/* Check for directory */
		if( (fileStat.st_mode) & S_IFDIR)
		{
			
			
			if(!recursive || isExcludedDirectory(currentItem, argv) == 1)	/* If we're not doing a recursive search then just  */
			{				/* ignore directories... */
			
				if(verbose)
					fprintf(stderr, "Directory: %s skipped.\n",currentItem->name);
			
				currentItem = GetNextItem(currentItem);
				if(currentItem == NULL)
					break;	/* break if there are no more files */
							
				continue;
			
			}
			else	/* we have a directory */
			{
				if(verbose)
					fprintf(stderr, "Scanning Directory: %s\n",currentItem->name);
					
		
				
				dirp = opendir(absPathIn); /* open sesame! */
				
				
				
				
				if( ( readdir(dirp)) != NULL )		/*  These two reads skip the "." */
					if( ( readdir(dirp)) != NULL )	/*  and ".." entries... */
					{
							
						if( (dp = readdir(dirp)) != NULL )	/* be sure something is in there */
						{
						
							/* Set up our linked list */
							oldItem = currentItem;

	   						MALLOC_AND_CHECK( firstItem, sizeof(fileList) );
							
							currentItem = firstItem;
												
							pathLength = strlen(oldItem->name) + 1;
							oldItem->name[pathLength - 1] = '/';
							oldItem->name[pathLength] = '\0';
						
							 /* CONSTANTCONDITION */
							while (1)
							{
							
								MALLOC_AND_CHECK( currentItem->name, (size_t)(strlen(dp->d_name) + pathLength+3));
							
								strcpy( currentItem->name, oldItem->name);
							
								strcat( currentItem->name, dp->d_name);
								
								
								if( (dp = readdir(dirp)) != NULL )
								{
									MALLOC_AND_CHECK( nextItem, sizeof(fileList) );

									currentItem->next = nextItem;
									
									currentItem = nextItem;
								}
								else
									break;
								
							}
											
			
							currentItem->next = oldItem->next;  /*  fix up links in the */
							oldItem->next = firstItem;			/*  list */
							
							currentItem = oldItem;		/* Get back in place */
							
						}/* end the first two dps */
					}
					
				
				
				
				if(closedir(dirp))	/* cleanup */
				{
					fprintf( stderr, "\n" PACKAGE ": Unable to close directory!\nError: %s\n", strerror(errno));
					exit(EX_OSERR);
				}
				

				currentItem = GetNextItem(currentItem);
				if(currentItem == NULL)
					break;	/* break if there are no more files */
					
							
				continue;
			}
			
		} /* end if this is a dir */
		else
		{
			/* We have a file. Be sure we should process this file by checking suffix list if needed. */
		
			if(useSuffixes && SuffixIsBad(absPathIn, suffixes, suffixLens, minSuffixLength, numSuffixes))	/* If we are using suffix matching and this file has a bad suffix */
			{
				if(verbose)
					fprintf(stderr, "Skipping: %s. (suffix not in list)\n",currentItem->name);
					
				currentItem = GetNextItem(currentItem);
				if(currentItem == NULL)
					break;	/* break if there are no more files */
				
						
				continue;
			}
		}
		
		INFILE = fopen( absPathIn, "rb");  /* open up the input file */

		if(INFILE == NULL)
		{
			fprintf(stderr, "\n" PACKAGE ": Unable to open %s for reading.\n" PACKAGE ": Error: %s\n" PACKAGE ": SKIPPING %s\n", absPathIn, strerror(errno), absPathIn);

			currentItem = GetNextItem(currentItem);
			if(currentItem == NULL)
				break;	/* break if there are no more files */
				
						
			continue;

		}
	
	
		
		
		if( useTmp )
		{
			/* try to get the $TMPDIR env var to set the temp dir to use for temp files */
			if(getenv("TMPDIR") != NULL )
			{
				strcpy(absPathOut,getenv("TMPDIR"));
				fprintf(stderr, "\n" PACKAGE ": Unable to read environment var $TMPDIR with -t option active.\n" PACKAGE ": Using source file directory instead.\n" );

			}
			else
			{
				/* No-Go. So use the file's directory for the temp file */
				strcpy(absPathOut,absPathIn);
			}
				
		}
		else
		{
			strcpy(absPathOut,absPathIn); /* use the file's directory for the temp file */
		}

		strcat(absPathOut, ".tmp.XXXXXX");		/* set up our temp file name */
		

		outfile_fd = mkstemp(absPathOut); /* Make the tempfile */

		if(outfile_fd == -1)
		{
			fprintf(stderr, "\n" PACKAGE ": Unable to create temp file %s.\n" PACKAGE ": Error: %s\n" PACKAGE ": (Type \"" PACKAGE " -h\" and consider \"-t\" to specify temp file location.)\n" PACKAGE ": SKIPPING %s.\n\n",absPathOut, strerror(errno), absPathIn);

			fclose(INFILE);
			
			currentItem = GetNextItem(currentItem);
			if(currentItem == NULL)
				break;	/* break if there are no more files */
			
			continue;
		}

		OUTFILE = fdopen( outfile_fd, "wb");  /* open up the tempfile to write to */

		if(OUTFILE == NULL)
		{
			fprintf(stderr, "\n" PACKAGE ": Unable to open temp file %s for writing.\n" PACKAGE ": Error: %s\n" PACKAGE ": Aborting. Some files may not have been processed.\n\n", absPathOut, strerror(errno));
			exit(EX_IOERR);
		}
		
		/*Set Permissions of newly created output file */
		SetPerms(fileStat, force, outfile_fd, absPathIn, &skipIt);
		
	
		
		/* Echo feedback of processing to the user */
		EchoFeedback(verbose, simulationMode, quiet, currentItem->name);
		
		
		
		/* Set our current state (What we are looking for) */
		currentState = FIND_FIRST_CHAR;	
	
		currentChar = 0; /* start at the first char of the string */
	
		prevChar = ' '; /* set this up so match whole words will work at the begining of a file */
		
		doneFlag = 0;
		
		
		
		while( !doneFlag ) 
		{
			
				
			if( interrupted )
			{
				fprintf(stderr, "\n" PACKAGE ":  User cancelled.\n");
				if( simulationMode )
					fprintf(stderr, "      A Total of %lu matches found in %lu file%s searched.\n      None replaced (simulation mode).\n", totalMatches > 0 ? (totalMatches - fileMatches): fileMatches, filecount+1, (filecount>0)?"s":"" );
				else
				{
					fprintf(stderr, "      A Total of %lu matches replaced in %lu file%s searched.\n", totalMatches > 0 ? (totalMatches - fileMatches): fileMatches, filecount, (filecount>1)?"s":"" );
					fprintf(stderr, "      Changes to file \"%s\" not saved.\n",currentItem->name);
				}
				
				if(unlink(absPathOut))
				{
					fprintf(stderr, "\n" PACKAGE ": An error occured deleting temp file %s.\n" PACKAGE ": Error: %s\n\n",absPathOut, strerror(errno));
					exit(EX_IOERR);
				}	
				exit(EX_TEMPFAIL);
			
			}
			
			ch = getc(INFILE);
			
			if(ch == EOF )
			{
				/* probably just EOF, but check for error */
				if(ferror(INFILE))
				{
					fprintf(stderr, "\n" PACKAGE ": An error occurred reading %s.\n" PACKAGE ": Error: %s\n\n",absPathIn, strerror(errno));
					exit(EX_OSERR);
				}	
				
				if( currentState != MATCH_STRING )
					break; /* we're done with this file */
				else
					currentState = FINISH_MATCH;
			}
			else if(ch == '\n')
			{
				lineCount++;
				seen = 0; 		/*mark line as not seen */
			}
			
			
				
			
			switch(currentState) 
			{
				case FIND_FIRST_CHAR:
						/* if we're not looking for whole words then it doesn't matter what */
						/* prevChar is. But if we are, then we have to be sure it was whitespace */
						
						if(whole_words == 0 || prevChar == ' ' || prevChar == '\n' ||
							prevChar == '\t' || prevChar == '\r')
						{
							if( searchString[0] == ch || (ignore_case &&  searchString[0] == toupper(ch)))	/* we found the first character of the searchstring */
							{
								currentState = MATCH_STRING; /* change state */
								currentChar = 0;

								/* let's read that char again */
								ungetc(ch,INFILE);
								if( ch == '\n' )
									lineCount--;
								break; /* we're done this pass */
							}
						}
						
						putc( ch, OUTFILE);  /* write out the char */
						
						break;
						
				case MATCH_STRING:
					
						if(searchString[currentChar] == '\0')  /* we're at the end of searchString */
						{										/*  must be a match */
							
							if(whole_words == 0 || ch == ' ' || ch == '\n' ||	/* be sure we have whitespace */
							ch == '\t' || ch == '\r' )							/*  next if looking for whole words */
							{
								fwrite( replaceBuffer, (size_t)replaceStringLength, (size_t)1, OUTFILE); /* write the replacement out */
							
								if(stringChanged)
								{
									strcpy( (char*)replaceBuffer, (char*)replaceString);
									stringChanged = 0;
								}
							
								
		
							
								fileMatches++;
							
								/* let's read that char again */
								ungetc(ch,INFILE);
								if( ch == '\n' )
									lineCount--;
							}
							else /* not a whole word */
							{
								buff[currentChar++] = (unsigned char)ch;  /* copy ch into buffer to write it later */
						
								fwrite( buff, (size_t)currentChar, (size_t)1, OUTFILE); /* write the buff out */
							}
							
							currentState = FIND_FIRST_CHAR;  /* change state */
							
							if(verbose == 1)
							{
								if( !seen ) /* if we have not already listed this line */
								{
									/*curPos = ftell(INFILE);*/
									if( simulationMode )
										printf( "  Line: %ld String Found\n", lineCount);	/* print position found */
									else
										printf( "  Line: %ld String Found and Replaced\n", lineCount);	/* print position found */
								
									seen = 1; /* Mark this line as seen*/
								}
							}
							
							break;
						}
						
						buff[currentChar] = (unsigned char)ch;  /* copy ch into buffer in case we need to write it later */
						
						
						if( (searchString[currentChar] == ' ') &&     /* if searchString has a space in it */
							(ch == '\n' || ch == '\r') )			/*  consider any newlines as a match and keep matching */
						{
							
							for(i=0; i < replaceStringLength; i++ )
							{
								if(replaceBuffer[i] == ' ')
								{
									replaceBuffer[i] = (unsigned char)ch; /* replace spaces with the actual char (newline, etc.) */
									
									stringChanged = 1;     /* mark string as changed */
									
									break;
									
								}
							}
							
							
							if(i >= replaceStringLength)
							{
								if(replaceStringLength+1 > MAX_STRING_LENGTH)
								{
									/* This is possible. Should rework to avoid this */
									fprintf( stderr, "\n" PACKAGE ": old_string has too many spaces at the end. Be sure the length of the new_string plus the number of spaces at the end of old_string is less than %d.\n", MAX_STRING_LENGTH);
										exit(EX_SOFTWARE);
								}
								
								replaceBuffer[replaceStringLength] = (unsigned char)ch; 		/* This will add a newline after the replace string */
								replaceStringLength++;							/* if the search string has a space and the located */
																				/* string has a newline */
						
							}	
							currentChar++;
							break;
							
						}
						
						if( searchString[currentChar] != ch && !(ignore_case &&  searchString[currentChar] == toupper(ch)))/* we don't match the whole string		 */
						{
							
							currentState = FIND_FIRST_CHAR;  /* change state */
							
							fwrite( buff, (size_t)currentChar ,(size_t)1, OUTFILE); /* write the buff out */
							
							/*read char again*/
							ungetc(ch, INFILE);
							if( ch == '\n' )
								lineCount--;
							
							/* If wea altered our replace buffer due to substituting spaces with line breaks, etc. */
							/* we have to refresh the buffer */
							if(stringChanged)
							{
								strcpy( (char*)replaceBuffer, (char*)replaceString);  /* refresh the buffer */
								stringChanged = 0;
							}
						}
						
						
					
						currentChar++;
						
						break;	
						
				case FINISH_MATCH:
						if(searchString[currentChar] == '\0')  /* we're at the end of searchString */
						{										/*  must be a match */
							
							
							fwrite( replaceBuffer, (size_t)replaceStringLength, (size_t)1, OUTFILE); /* write the replacement out */
						
							fileMatches++;
						
							
							doneFlag = 1; /* done with this file */
								
								
							if(verbose == 1)
							{
								if( !seen ) /* if we have not already listed this line */
								{
									/*curPos = ftell(INFILE);*/
									if( simulationMode )
										printf( "  Line: %ld String Found\n", lineCount);	/* print position found */
									else
										printf( "  Line: %ld String Found and Replaced\n", lineCount);	/* print position found */
								
									seen = 1; /* Mark this line as seen*/
								}
							}
		
							break;
						}
						
					
						doneFlag = 1; /* done with this file */
						
						fwrite( buff, (size_t)currentChar, (size_t)1, OUTFILE); /* write the buff out */
						
						
						
						break;
				
				default:
						/* never get here... in theory ;-) */
						fprintf(stderr, "\n" PACKAGE "Ack!!! This should never happen... Bad case in switch... Tell somebody, quick!\n");
						exit(EX_SOFTWARE);
						break;
			 }/*  end switch(currentState) */
					
		
			prevChar = ch; /* copy this char for whole word searches. */
		
		
		}/* end while not eof */

		fclose(INFILE);
		fclose(OUTFILE);
		
		if( interrupted )
		{
			fprintf(stderr, "\n" PACKAGE ":  User cancelled.\n");
			if( simulationMode )
				fprintf(stderr, "      A Total of %lu matches found in %lu file%s searched.\n      None replaced (simulation mode).\n", totalMatches , filecount+1, (filecount>0)?"s":"");
			else
			{
				fprintf(stderr, "      A Total of %lu matches replaced in %lu file%s searched.\n", totalMatches > 0 ? (totalMatches - fileMatches): fileMatches, filecount, (filecount>1)?"s":"");
			}
			
			if(unlink(absPathOut))
			{
				fprintf( stderr, "\n" PACKAGE ": An error occured deleting %s.\nError: %s\n",absPathOut, strerror(errno));
				exit(EX_IOERR);
			}
                                
			exit(EX_TEMPFAIL);
		
		}
		
		
		if(fileMatches > 0)
		{
			
			if( simulationMode )
			{
				fprintf( stderr, "  %s\n",absPathIn);	/*list the file*/
				/* delete tmp file */
				if(unlink(absPathOut))
					fprintf(stderr, "\n" PACKAGE ": An error occured deleting temp file %s.\n" PACKAGE ": Error: %s\n\n",absPathOut, strerror(errno));
				totalMatches += fileMatches;		/* handle our count */
			}
			else
			{
				
				
				if( prompt )	/* User must decide to process this file or not */
				{				
					good = 0;
					while( !good )
					{
						fprintf(stderr, "\nSave '%s' ? ([Y]/N) ",absPathIn);
					
						fgets((char*)line, 255, stdin);	/*read in response */
				
						
						if( line[0] == 'y' || line[0] == 'Y' || line[0] == '\n' )
						{
							good = 1; 		/*we got what we were looking for */
							fprintf(stderr, "Saved.\n");
							
						}
						else if( line[0] == 'n' || line[0] == 'N' )
						{
							fprintf(stderr, "Not Saved.\n");		/*We need to skip this file */
							good = 1;
							skipIt = 1;  /*mark file to be skipped */
						}
					
					}
					
				}

				
				
				if( !skipIt )
				{
					/* rename tmpfile to old file */
					if(rename(absPathOut, absPathIn))
					{
						fprintf( stderr, "\n" PACKAGE ": An error occured replacing %s with %s.\n" PACKAGE ": Error: %s\n\n",absPathIn,absPathOut, strerror(errno));
						if(unlink(absPathOut))
							fprintf(stderr, "\n" PACKAGE ": An error occured deleting temp file %s.\n" PACKAGE ": Error: %s\n\n",absPathOut, strerror(errno));
		
					}
					
					/* Set mod date to old mod date if we need to */
					
					if(keepDates)
					{
						 struct utimbuf timeStruct;
						 
						 timeStruct.actime = fileStat.st_atime;
						 timeStruct.modtime = fileStat.st_mtime;
						
						 if( utime(absPathIn, &timeStruct) )
						 	fprintf(stderr, "\n" PACKAGE ": An error occured setting the access time and mod time of the file %s.\n" PACKAGE ": Error: %s\n\n",absPathIn, strerror(errno));

					}
					
					totalMatches += fileMatches;		/* handle our count */
				}
				else
				{
					skipIt = 0;
					if(unlink(absPathOut))
							fprintf(stderr, "\n" PACKAGE ": An error occured deleting temp file %s.\n" PACKAGE ": Error: %s\n\n",absPathOut, strerror(errno));
		
				}
			}
			
			fileMatches = 0;
			
			
		}
		else
		{
			/* delete tmp file */
			if(unlink(absPathOut))
				fprintf(stderr, "\n" PACKAGE ": An error occured deleting temp file %s.\n" PACKAGE ":Error: %s\n\n",absPathOut, strerror(errno));
		}

		lineCount = 1;
		filecount++;
		
		
		currentItem = GetNextItem(currentItem);
		if(currentItem == NULL)
			break;	/* break if there are no more files */
		
		
	}/*  end while there are more files */

	if( simulationMode )
		fprintf(stderr, "\nA Total of %lu matches found in %lu file%s searched.\nNone replaced (simulation mode).\n", totalMatches, filecount, (filecount>1)?"s":"");
	else
		fprintf(stderr, "\nA Total of %lu matches replaced in %lu file%s searched.\n", totalMatches, filecount, (filecount>1)?"s":"");


	if(currentItem)
	{	
		if(currentItem->name)
			free(currentItem->name);	/*   won't be needing */
			
		free(currentItem);  		/*   these... */
	}

	return(EX_OK); /*success*/

} /* end main */




/**************************************/
/* EchoModes() prints out the         */
/* values of some vars to stderr      */
/* for the user to see...             */
/* (Take too many params!)            */
static void EchoModes(int escapeInput, int ignore_case, int whole_words, int simulationMode, unsigned char* searchString, unsigned char* replaceString, unsigned char* orgSearchString, unsigned char* orgReplaceString)
{
	int i;
	
	if(escapeInput)
	{
		if( simulationMode )
			fprintf(stderr, "Simulating replacement of \"%s\" with \"%s\" ",orgSearchString,orgReplaceString);
		else
			fprintf(stderr, "Replacing \"%s\" with \"%s\" ",orgSearchString,orgReplaceString);
	}
	else
	{
		if( simulationMode )
			fprintf(stderr, "Simulating replacement of \"%s\" with \"%s\" ",searchString,replaceString);
		else
			fprintf(stderr, "Replacing \"%s\" with \"%s\" ",searchString,replaceString);
	}
	
	if(ignore_case == 1)
	{
		fprintf(stderr, "(ignoring case) ");
		
		
		for(i = 0; searchString[i] != '\0'; i++)						/* upcase the whole thing */
			searchString[i] = (unsigned char)toupper(searchString[i]);
	}
	else
	{
		fprintf(stderr, "(case sensitive) ");
	}
	
	if(whole_words == 1)
	{
		fprintf(stderr, "(whole words only)\n");
	}
	else
	{
		fprintf(stderr, "(partial words matched)\n");
	}



	if( simulationMode )
		fprintf(stderr, "The files listed below would be modified in a replace operation.\n");
		
	return;
	
}



/**************************************/
/* EchoFeedback() echoes filename     */
/* info, or a "." to show the user    */
/* that we are working.                */

static void EchoFeedback(int verbose, int simulationMode, int quiet, char* filename)
{

	if(verbose == 1 && simulationMode == 0)
	{
		fprintf(stderr, "Processing:  %s\n", filename);	/* print the current filename */
	}
	else if( quiet != 1 )
	{
		if( !simulationMode )
			fprintf(stderr,".");		/* print one dot per file for standard (non-quiet, non-verbose) mode */
	}

	return;	
}
	
	
/********************************/
/* SetPerms() sets the file     */
/* Owner/group and permissions  */
/* if possible. If force option */
/* is set we ignore any errors. */
/* Otherwise we skip the file   */
/* and do not svae the changes  */
/* later.                       */
static void SetPerms(struct stat fileStat, int force, int outfile_fd, char* absPathIn, int* skipIt )
{

	if(fchown(outfile_fd,fileStat.st_uid,fileStat.st_gid))	/* and its owner & group ids */
	{
		if( force )		/*we don't care that permissions don't match */
		{	
			fprintf( stderr, "\n" PACKAGE ": Unable to set owner/group of %s.\nError: %s\n" PACKAGE ": WARNING: New owner/group may not match!\n\n",absPathIn, strerror(errno) ); /* only root cant change the owner id, though */
		}
		else
		{
			fprintf( stderr, "\n" PACKAGE ": Unable to set owner/group of %s.\n" PACKAGE ": Error: %s\n" PACKAGE ": SKIPPING %s!\n\n",absPathIn, strerror(errno), absPathIn); /* only root cant change the owner id, though */
			*skipIt = 1; /* so we don't overwrite the file later */
		}
	}

	if(fchmod(outfile_fd,fileStat.st_mode))		/* set up its permissions */
	{
		if( force )		/*we don't care that permissions don't match */
		{	
			fprintf( stderr, "\n" PACKAGE ": Unable to set permissions of %s.\n" PACKAGE ": Error: %s\n" PACKAGE ": WARNING: New permissions may not match!\n",absPathIn, strerror(errno) );
		}
		else
		{
			fprintf( stderr, "\n" PACKAGE ": Unable to set permissions of %s.\n" PACKAGE ": Error: %s\n" PACKAGE ": SKIPPING %s!\n",absPathIn, strerror(errno), absPathIn );
			*skipIt = 1; /* so we don't overwrite the file later */
		}
	}
	
	return;
}


/********************************/
/* GetNextItem (in linked list) */
/* This ought to be put into    */
/* a separate linked list module */

static fileList*	GetNextItem( fileList* currentItem)
{
	fileList	*oldItem;
	
	oldItem = currentItem;
	if(currentItem->next)
	{
		currentItem = currentItem->next;				

		free(oldItem->name);	/*   won't be needing */
		free(oldItem);  		/*   these... */
	}
	else
		currentItem = NULL;
		
	return currentItem;
		
}

/****************************/
/*  Handle control-Cs       */
static void DoInterrupt(void)
{
	interrupted++;						/* we got a SIG_INT */
	write(STDERR_FILENO, "\n\n*Break*\n\n", 11);
	if( interrupted > 3 )
	{
		fprintf(stderr, "\n\n*Terminating*\n\n");
		exit(EX_TEMPFAIL);
	}
	return;
}