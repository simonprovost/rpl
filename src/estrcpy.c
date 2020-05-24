/* estrcpy.c */
/* estrcpy, by Joe Laffey <joe@laffeycomputer.com>                     */


/* estrcpy is a string copy that converts escape sequences in strings, */
/* replacing them with the appropriate byte codes. It handles octal    */
/* in the form '\nnn' and hex in the form '\0xnn'                      */

/*estrcpy is released under the PERL "Artistic License" as described below. */

/* The "Artistic License"                                                   */
/*                                                                          */
/* Preamble                                                                 */
/*                                                                          */
/* The intent of this document is to state the conditions under which a     */
/* Package may be copied, such that the Copyright Holder maintains some     */
/* semblance of artistic control over the development of the package, while */
/* giving the users of the package the right to use and distribute the      */
/* Package in a more-or-less customary fashion, plus the right to make      */
/* reasonable modifications.                                                */
/*                                                                          */
/* Definitions                                                              */
/*                                                                          */
/*                                                                          */
/* "Package" refers to the collection of files distributed by the Copyright */
/* Holder, and derivatives of that collection of files created through      */
/* textual modification.                                                    */
/*                                                                          */
/* "Standard Version" refers to such a Package if it has not been modified, */
/* or has been modified in accordance with the wishes of the Copyright      */
/* Holder as specified below.                                               */
/*                                                                          */
/* "Copyright Holder" is whoever is named in the copyright or copyrights    */
/* for the package.                                                         */
/*                                                                          */
/* "You" is you, if you're thinking about copying or distributing this      */
/* Package.                                                                 */
/*                                                                          */
/* "Reasonable copying fee" is whatever you can justify on the basis of     */
/* media cost, duplication charges, time of people involved, and so on.     */
/* (You will not be required to justify it to the Copyright Holder, but     */
/* only to the computing community at large as a market that must bear the  */
/* fee.)                                                                    */
/*                                                                          */
/* "Freely Available" means that no fee is charged for the item itself,     */
/* though there may be fees involved in handling the item. It also means    */
/* that recipients of the item may redistribute it under the same           */
/* conditions they received it.                                             */
/*                                                                          */
/*                                                                          */
/* 1.	You may make and give away verbatim copies of the source form of    */
/* the Standard Version of this Package without restriction, provided that  */
/* you duplicate all of the original copyright notices and associated       */
/* disclaimers.                                                             */
/*                                                                          */
/* 2.	You may apply bug fixes, portability fixes and other modifications  */
/* derived from the Public Domain or from the Copyright Holder. A Package   */
/* modified in such a way shall still be considered the Standard Version.   */
/*                                                                          */
/* 3.	You may otherwise modify your copy of this Package in any way,      */
/* provided that you insert a prominent notice in each changed file stating */
/* how and when you changed that file, and provided that you do at least    */
/* ONE of the following:                                                    */
/*                                                                          */
/* a.	place your modifications in the Public Domain or otherwise make     */
/* them Freely Available, such as by posting said modifications to Usenet   */
/* or an equivalent medium, or placing the modifications on a major archive */
/* site such as uunet.uu.net, or by allowing the Copyright Holder to        */
/* include your modifications in the Standard Version of the Package.       */
/* b.	use the modified Package only within your corporation or            */
/* organization.                                                            */
/* c.	rename any non-standard executables so the names do not             */
/* conflict with standard executables, which must also be provided, and     */
/* provide a separate manual page for each non-standard executable that     */
/* clearly documents how it differs from the Standard Version.              */
/* d.	make other distribution arrangements with the Copyright Holder.     */
/*                                                                          */
/*                                                                          */
/* 4.	You may distribute the programs of this Package in object code or   */
/* executable form, provided that you do at least ONE of the following:     */
/*                                                                          */
/* a.	distribute a Standard Version of the executables and library        */
/* files, together with instructions (in the manual page or equivalent) on  */
/* where to get the Standard Version.                                       */
/* b.	accompany the distribution with the machine-readable source of the  */
/* Package with your modifications.                                         */
/* c.	give non-standard executables non-standard names, and clearly       */
/* document the differences in manual pages (or equivalent), together with  */
/* instructions on where to get the Standard Version.                       */
/* d.	make other distribution arrangements with the Copyright Holder.     */
/*                                                                          */
/*                                                                          */
/* 5.	You may charge a reasonable copying fee for any distribution of     */
/* this Package. You may charge any fee you choose for support of this      */
/* Package. You may not charge a fee for this Package itself. However, you  */
/* may distribute this Package in aggregate with other (possibly            */
/* commercial) programs as part of a larger (possibly commercial) software  */
/* distribution provided that you do not advertise this Package as a        */
/* product of your own. You may embed this Package's interpreter within an  */
/* executable of yours (by linking); this shall be construed as a mere form */
/* of aggregation, provided that the complete Standard Version of the       */
/* interpreter is so embedded.                                              */
/*                                                                          */
/* 6.	The scripts and library files supplied as input to or produced as   */
/* output from the programs of this Package do not automatically fall under */
/* the copyright of this Package, but belong to whomever generated them,    */
/* and may be sold commercially, and may be aggregated with this Package.   */
/* If such scripts or library files are aggregated with this Package via    */
/* the so-called "undump" or "unexec" methods of producing a binary         */
/* executable image, then distribution of such an image shall neither be    */
/* construed as a distribution of this Package nor shall it fall under the  */
/* restrictions of Paragraphs 3 and 4, provided that you do not represent   */
/* such an executable image as a Standard Version of this Package.          */
/*                                                                          */
/* 7.	C subroutines (or comparably compiled subroutines in other          */
/* languages) supplied by you and linked into this Package in order to      */
/* emulate subroutines and variables of the language defined by this        */
/* Package shall not be considered part of this Package, but are the        */
/* equivalent of input as in Paragraph 6, provided these subroutines do not */
/* change the language in any way that would cause it to fail the           */
/* regression tests for the language.                                       */
/*                                                                          */
/* 8.	Aggregation of this Package with a commercial distribution is       */
/* always permitted provided that the use of this Package is embedded; that */
/* is, when no overt attempt is made to make this Package's interfaces      */
/* visible to the end user of the commercial distribution. Such use shall   */
/* not be construed as a distribution of this Package.                      */
/*                                                                          */
/* 9.	The name of the Copyright Holder may not be used to endorse or      */
/* promote products derived from this software without specific prior       */
/* written permission.                                                      */
/*                                                                          */
/* 10.	THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR         */
/* IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED           */
/* WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.      */
/*                                                                          */
/*                                                                          */
/* The End                                                                  */


/* 
 # Revision Info:  $Revision: 1.4 $
 # Last Modified:  $Date: 2001/09/08 20:38:29 $                  
 # by:             $Author: joe $              
*/



#include "estrcpy.h"



/* Don't let this overrun! Write your own estrncpy for anything critical!! */
char* estrcpy(char *dest, const char *src)
{
  register int i = 0, j = 0;
  int byte, k;
  
  	while(src[i]) 
	{
	 	if(src[i] != '\\') 
		{
		  dest[j++] = src[i++];
		}
	  else 
		{
		  switch (src[++i]) 
			{
			case 'n':		   /* lf */
			  dest[j++] = 0x0a;
			  break;
			case 'r':		   /* cr */
			  dest[j++] = 0x0d;
			  break;
			case 't':		   /* tab */
			  dest[j++] = '\t';
			  break;
			case '\\':		  /* backslash */
			  dest[j++] = '\\';
			  break;
			case '0':		   /* '\nnn' octal format  or '\0xnn' hex format */
				if(src[i+1] == 'x')
				{
					  /* hex */
					  i++;
					  
					  if( src[i+1] >= '0' && src[i+1] <= '9')
					  {
							byte = src[i+1] - '0';  
					  }
					  else if(src[i+1] >= 'a' && src[i+1] <= 'f')
					  {
						  	byte = src[i+1] - 'a' + 10; 
					  }
					  else if(src[i+1] >= 'A' && src[i+1] <= 'F')
					  {
						  	byte = src[i+1] - 'A' + 10; 
					  }
					  else
					  {
							break;  
					  }
					  
					  i++;
					  
					  if( src[i+1] >= '0' && src[i+1] <= '9')
					  {
							byte = (byte<<4) + src[i+1] - '0';  
					  }
					  else if(src[i+1] >= 'a' && src[i+1] <= 'f')
					  {
						  	byte = (byte<<4) + src[i+1] - 'a' + 10; 
					  }
					  else if(src[i+1] >= 'A' && src[i+1] <= 'F')
					  {
						  	byte = (byte<<4) + src[i+1] - 'A' + 10; 
					  }
					  else
					  {
							break;  
					  }
					  i++;
					  dest[j++] = (char)((unsigned char)byte);
					  break;		
				}
				/* no break here on purpose... */
			/* FALLTHROUGH */
			case '1':
			/* FALLTHROUGH */
			case '2':
			/* FALLTHROUGH */
			case '3':
			/* FALLTHROUGH */
			case '4':
			/* FALLTHROUGH */
			case '5':
			/* FALLTHROUGH */
			case '6':
			/* FALLTHROUGH */
			case '7':
			  /* octal */
			  byte = src[i] - '0';
			  for( k=0; k<3; k++)
			  { 
				  if ((src[i+1] < '0') || (src[i+1] > '7'))
				  	break;
				  else 
				  {
					byte = (byte<<3) + src[i+1] - '0';
					++i;
				  }
			  }
			  dest[j++] = (char)((unsigned char)byte);
			  break;
			  
			default:			
			  break;
			}
		  ++i;
		}
	} 
	dest[j] = '\0';
	return dest;
}



