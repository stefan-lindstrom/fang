/******************************************************************************
 * $Id: color.c,v 1.3 2002/11/07 04:02:01 fang Exp $
 *   
 * File: color.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: color.c,v 
 *   Revision 1.2  2002/07/28 13:56:03  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.3  2002/05/27 10:52:48  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.8  2001/10/06 14:55:12  mud
 *   *** empty log message ***
 *
 *   Revision 1.7  2001/10/04 17:43:53  mud
 *   *** empty log message ***
 *
 *   Revision 1.6  2001/10/03 14:34:19  mud
 *   *** empty log message ***
 *
 *   Revision 1.5  2001/09/19 05:52:58  mud
 *   Dynamic buffer and select-\>poll mod.
 *
 *   Revision 1.4  2000/09/04 02:37:59  mud
 *   Warnings removed...
 *
 *   Revision 1.3  2000/03/09 12:47:44  mud
 *   Code cleanup. Compiles without warnings now...
 *
 *   Revision 1.2  2000/02/21 14:00:16  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/*************************************************************************
*   File: colour.c                                      Part of CircleMUD *
*  Usage: interprets inline colour codes                                  *
*  Name: Easy Colour v2.2                                                 *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*  Modifications Copyright Trevor Man 1997                                *
*  Based on the Easy Color patch by mud@proimages.proimages.com           *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "sysdep.h"
#include "conf.h"
#include "color.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "utils.h"

#define CNRM  "\x1B[0;0m"
#define CBLK  "\x1B[0;30m"
#define CRED  "\x1B[0;31m"
#define CGRN  "\x1B[0;32m"
#define CYEL  "\x1B[0;33m"
#define CBLU  "\x1B[0;34m"
#define CMAG  "\x1B[0;35m"
#define CCYN  "\x1B[0;36m"
#define CWHT  "\x1B[0;37m"
#define CNUL  ""

#define BBLK  "\x1B[1;30m"
#define BRED  "\x1B[1;31m"
#define BGRN  "\x1B[1;32m"
#define BYEL  "\x1B[1;33m"
#define BBLU  "\x1B[1;34m"
#define BMAG  "\x1B[1;35m"
#define BCYN  "\x1B[1;36m"
#define BWHT  "\x1B[1;37m"

#define BKBLK  "\x1B[40m"
#define BKRED  "\x1B[41m"
#define BKGRN  "\x1B[42m"
#define BKYEL  "\x1B[43m"
#define BKBLU  "\x1B[44m"
#define BKMAG  "\x1B[45m"
#define BKCYN  "\x1B[46m"
#define BKWHT  "\x1B[47m"

#define CAMP  "&"
#define CSLH  "\\"

#define CUDL  "\x1B[4m"	/* Underline ANSI code */
#define CFSH  "\x1B[5m"	/* Flashing ANSI code.  Change to #define CFSH "" if
                         * you want to disable flashing colour codes
                         */
#define CRVS  "\x1B[7m" /* Reverse video ANSI code */

const char *COLOURLIST[] = {CNRM,  CRED,  CGRN,  CYEL,  CBLU,  CMAG,  CCYN, CWHT,
                           BRED,  BGRN,  BYEL,  BBLU,  BMAG,  BCYN,  BWHT, BKRED, 
                            BKGRN, BKYEL, BKBLU, BKMAG, BKCYN, BKWHT, CBLK, BBLK,  
                            BKBLK, CSLH,  CFSH,  CRVS,  CUDL, CAMP };

#define MAX_COLORS 30

/*
* Array of map colors between \c color codes and the numbers that the users type in to get them
* in the mud.
*/
char *colormapWithC[] =
{
  "white",
  "red",
  "green",
  "yellow",
  "blue",
  "#FF00FF",
  "#00FFFF",
  "white",

  "red",
  "green",
  "yellow",
  "blue",
  "#FF00FF",
  "#00FFFF",
  "white",

  "red",
  "green",
  "yellow",
  "blue",
  "#FF00FF",
  "#00FFFF",
  "white",

  "#666666",
  "#666666",
  "#666666",
};

/*
* Array of map colors between & color codes and the numbers that the users type in to get them
* in the mud.
*/
char *colormapWithAnd[] =
{
  "white",
  "black",
  "red",
  "green",
  "yellow",
  "blue",
  "#FF00FF",
  "#00FFFF",
  "white",

  "black",
  "red",
  "green",
  "yellow",
  "blue",
  "#FF00FF",
  "#00FFFF",
  "white",

  "black",
  "red",
  "green",
  "yellow",
  "blue",
  "#FF00FF",
  "#00FFFF",
  "white",

};
/*
 * Since the & color codes use characters instead of numbers, a second stage of mapping
 * has to be done for those, hence the string below.
 */
char *andmap = "nkrgybmcwKRGYBMCW01234567";


int isnum(char s)
{
  if (s == '\0')
    return 0;
  return( (s>='0') && (s<='9') );
}

int is_colour(char code)
{
  switch (code) {
  /* Normal colours */
  case  'k': return 22; break;	/* Black */
  case  'r': return 1;	break;	/* Red */
  case  'g': return 2;	break;	/* Green */
  case  'y': return 3;	break;	/* Yellow */
  case  'b': return 4;	break;	/* Blue */
  case  'm': return 5;	break;	/* Magenta */
  case  'c': return 6;	break;	/* Cyan */
  case  'w': return 7;	break;	/* White */

  /* Bold colours */
  case  'K': return 23; break;  /* Bold black (Just for completeness) */
  case  'R': return 8;	break;	/* Bold red */
  case  'G': return 9;	break;	/* Bold green */
  case  'Y': return 10;	break;	/* Bold yellow */
  case  'B': return 11;	break;	/* Bold blue */
  case  'M': return 12;	break;	/* Bold magenta */
  case  'C': return 13;	break;	/* Bold cyan */
  case  'W': return 14;	break;	/* Bold white */
  
  /* Background colours */
  case  '0': return 24;	break; 	/* Black background */
  case  '1': return 15;	break;	/* Red background */
  case  '2': return 16;	break;	/* Green background */
  case  '3': return 17;	break;	/* Yellow background */
  case  '4': return 18;	break;	/* Blue background */
  case  '5': return 19;	break;	/* Magenta background */
  case  '6': return 20;	break;	/* Cyan background */
  case  '7': return 21;	break;	/* White background */

  /* Misc characters */
  case  '&': return 29;	break;	/* The & character */
  case '\\': return 25;	break;	/* The \ character */
  
  /* Special codes */
  case  'n': return 0;	break;	/* Normal */
  case  'f': return 26;	break;	/* Flash */
  case  'v': return 27; break;	/* Reverse video */
  case  'u': return 28; break;	/* Underline (Only for mono screens) */

  default:   return -1;	break;
  }
  return -1;
}

GString *proc_color(GString *inbuf, int colour)
{
  register int j = 0;
  int max, c = 0;
  GString *out = g_string_new(NULL);

  if (inbuf->str[0] == '\0')
    return out;

  while (inbuf->str[j] != '\0') {
    if ((inbuf->str[j]=='\\') && (inbuf->str[j+1]=='c')
        && isnum(inbuf->str[j + 2]) && isnum(inbuf->str[j + 3])) {
      c = (inbuf->str[j + 2] - '0')*10 + inbuf->str[j + 3]-'0';
      j += 4;
    } else if ((inbuf->str[j] == '&') && !(is_colour(inbuf->str[j + 1]) == -1)) {
      c = is_colour(inbuf->str[j + 1]);
      j += 2;
    } else {
      out = g_string_append_c(out, inbuf->str[j]);
      j++;
      continue;
    }
    if (c > MAX_COLORS)
      c = 0;
    max = strlen(COLOURLIST[c]);
    if (colour || max == 1)
      out = g_string_append(out, COLOURLIST[c]);
  }

  return out;
}

void htmlize_string(char *line, char *result)
{
  char lineout[1000], substring[1000], 
       *fg = colormapWithC[0], *bg = "black", *ptr;
  int i, j, index, bold = 0;
  
  if (!line || !result || !*line)
  {
    if (!line  || !*line)
      mlog("No line!");
    else if (!result)
      mlog("No result!");
    return;
  } 
    for (i = 0; i < strlen(line); i ++)
    {
      if (line[i] == '\\' && (i+3 < strlen(line)) && 
        line[i+1] == 'c' && isdigit(line[i+2]) && isdigit(line[i+3])) // \cxx color code
      {
        index = 10*(line[i+2]-'0') + line[i+3]-'0';
        if (index < 0)
        {
          index = 0; // Wrong color number for \c code given, ignoring this color code.
        }
        else if (index == 0)
        {
          fg = colormapWithC[0];
          bg = "black";
          bold = 0;
        }
        else if ((index < 8) || (index == 22))
        {
          fg = colormapWithC[index];
          bold = 0;
        }
        else if ((index < 15) || (index == 23))
        {
          fg = colormapWithC[index];
          bold = 1;
        }
        else
        {
          index = 0; // Wrong color number for \c code given, ignoring this color code.
        }
        i += 4;
        if (index)
        {
          if (bold)
            sprintf(lineout, "</font><font style=\"font-weight: bold; color: %s; background: %s\">", fg, bg);
          else
            sprintf(lineout, "</font><font style=\"color: %s; background: %s\">", fg, bg);
        }
        else
          memset(lineout, 0, 1000);
        // Now it's time to print out the text that is to be surroundsed by the <font> tag
        for (j = i; line[j] && line[j] != '\\' && line[j] != '&'; j ++);
        memset(substring, 0, 1000);
        strncpy(substring, (line+i), j-i);
        if (index && line[j]) // Don't type outend tag if end of string or normal text
          sprintf(lineout, "%s%s</font>", lineout, substring);
        else
          sprintf(lineout, "%s%s", lineout, substring);
        sprintf(result, "%s%s", result, lineout);
        i = j-1; // -1 since j is pointing at what might be a color code beginning
      }

      else if (line[i] == '&' && (ptr = strchr(andmap,line[i+1]))) // &x color code
      {
        index = ptr-andmap; // Which index in the andmap was pointed out?

        if (index == 0)
        {
          fg = colormapWithAnd[0];
          bg = "black";
          bold = 0;
        }
        else if (index < 9)
        {
          fg = colormapWithAnd[index];
          bold = 0;
        }
        else if (index < 17)
        {
          fg = colormapWithAnd[index];
          bold = 1;
        }
        else
        {
           bg = colormapWithAnd[index];
        }
        if (index)
        {
          if (bold)
            sprintf(lineout, "</font><font style=\"font-weight:bold; color: %s; background: %s\">", fg, bg);
          else
            sprintf(lineout, "</font><font style=\"color: %s; background: %s\">", fg, bg);
        }
        else
          memset(lineout, 0, 1000);
        i+=2;
        // Now it's time to print out the text that is to be surrounded by the <font> tag
        for (j = i; line[j] && line[j] != '\\' && line[j] != '&'; j ++);
        memset(substring, 0, 1000);
        strncpy(substring, (line+i), j-i);
        if (index && line[j])
          sprintf(lineout, "%s%s</font>", lineout, substring);
        else
          sprintf(lineout, "%s%s", lineout, substring);
        sprintf(result, "%s%s", result, lineout);
        i = j-1;  // -1 since j is pointing at what might be a color code beginning
      }
      // other special characters that need converting
      else if (line[i] == '&' && line[i+1] == '&')
      {
        sprintf(result, "%s&amp;", result);
        i++;
      }
      else if (line[i] == '&' && line[i+1] == '\\')
      {
        sprintf(result, "%s\\", result);
        i++;
      }
      else if (line[i] == '>')
        sprintf(result, "%s&gt;", result);
      else if (line[i] == '<')
        sprintf(result, "%s&lt;", result);
      else if (line[i] == '"')
        sprintf(result, "%s&quot;", result);
      else
        sprintf(result, "%s%c", result, line[i]); // no color code, print out character to string
    }
}


/*
 * Takes a mud help file or something like that, with color codes in the text, and
 * converts it so that the colors are html-encoded and writes it down to another file.
 * For it to look as true to the layout in the game as possible, the outfile
 * must have <pre> </pre> surrounding the output that is written to file with this function.
 * Surrounding tags, such as <html>, <body> and such must be written by the function
 * calling this one.
 * It does not close either of the files; the caller controls that. 
 *
 * Parameters: File to read from and file to write HTML output to.
 * Returns: 1 if everything went ok, 0 if something went wrong.
 */
int htmlize(FILE *infile, FILE *outfile)
{
  char line[10000], *result; // colorAtEndOfLine[100];
  if (!infile || !outfile)
  {
    return 0;
  }
  CREATE( result, char, 10000 );
  while (fgets(line, 10000, infile))
  {
    memset(result, 0, 10000);
    htmlize_string(line, result);
    fprintf(outfile, result);
  }
  free(result);
  return 1;
}

ACMD(do_htmlize)
{
  char filein[200], fileout[200];
  FILE *in, *out;
  two_arguments(argument, filein, fileout);
  if (!*filein || !*fileout)
  {
    send_to_char("Syntax: htmlize infile outfile.\r\n", ch);
    return;
  }
  if (!(in = fopen(filein, "r")))
  {
    asend_to_char(ch, "Couldn't open file %s for reading.\r\n", filein);
    return;
  }
  if (!(out = fopen(fileout, "w")))
  {
    asend_to_char(ch, "Couldn't open file %s for writing.\r\n", fileout);
    return;
  }
  send_to_char("Processing...\r\n", ch);
  fprintf(out, "<html>\r\n<head><title>Contents of file %s</title></head>\r\n", filein);
  fprintf(out, "<body text=white bgcolor=black>\r\n<pre>\r\n");
  htmlize(in, out);
  fprintf(out, "</pre>\r\n</body>\r\n</html>");
  send_to_char("Done!\r\n", ch);
  fclose(in);
  fclose(out);
}
