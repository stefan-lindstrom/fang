/******************************************************************************
 * $Id: modify.c,v 1.12 2003/06/07 08:07:54 fang Exp $
 *   
 * File: modify.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: modify.c,v $
 *   Revision 1.12  2003/06/07 08:07:54  fang
 *   Moved syslog messages to xlog and took invis level into account.
 *
 *   Revision 1.11  2003/02/02 14:48:49  fang
 *   Extra descs added to tool as well.
 *
 *   Revision 1.10  2003/01/25 16:47:14  fang
 *   Extra descs in craft completed. For tool  is yet to be done. The code can be cleaner tho'
 *
 *   Revision 1.9  2002/12/11 19:44:20  fang
 *   fisch: added "mail end" notification and removed guild info from score
 *
 *   Revision 1.8  2002/11/07 04:02:03  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.7  2002/10/12 06:30:15  fang
 *   fisch: converted boards to xml and made them dynamic not coded.
 *
 *   Revision 1.6  2002/10/03 05:55:09  fischer
 *   fischer: moved ideas to be used as an old ideas and added newideas file
 *
 *   Revision 1.5  2002/09/15 00:50:08  fang
 *   Drath: XLOG update - Categories added to xlog display automatically
 *
 *   Revision 1.4  2002/07/30 20:02:30  fang
 *   Drath: SKILLSET FILE added
 *
 *   Revision 1.3  2002/07/28 18:00:33  fang
 *   Fixed a bug with descs.
 *
 *   Revision 1.2  2002/07/28 13:56:04  fang
 *   Removed dmlloc...just to test.
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.6  2002/06/25 19:23:51  fang
 *   Drath: XLOG Commit #2
 *
 *   Revision 1.5  2002/06/06 13:06:15  fang
 *   *** empty log message ***
 *
 *   Revision 1.4  2002/06/03 16:25:25  fang
 *   Mazrim: Enabled autoREBOOT on an 8 hour cycle to keep the memory usage of the mud down.
 *
 *   Revision 1.3  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.16  2002/01/24 21:33:17  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.15.2.1  2002/01/22 20:50:59  mud
 *
 *
 *   Removed boats and info system. Obsolete, unused code with no value.
 *   Fixed bug in portal_stone SPECIAL. Now checks that a room exists
 *   before attempting to transf chars there. :)
 *
 *   Revision 1.15  2001/03/22 16:37:07  mud
 *   *** empty log message ***
 *
 *   Revision 1.14  2001/01/19 17:51:51  mud
 *   Added line numbers to the editor.
 *
 *   Revision 1.13  2000/11/23 22:16:16  mud
 *   *** empty log message ***
 *
 *   Revision 1.12  2000/11/18 17:13:55  mud
 *   added guildmail command
 *
 *   Revision 1.11  2000/11/08 08:25:39  mud
 *   Moved update_p_tabel() to xmlcharsave.[ch].
 *
 *   Revision 1.10  2000/10/31 01:28:10  mud
 *   added multiple recipients for mudmail. 5 should be enough..
 *
 *   Revision 1.9  2000/08/10 15:06:29  mud
 *   added hedit (help editor)
 *
 *   Revision 1.8  2000/08/03 19:28:43  mud
 *
 *   Patches for talent, and added talent requirements for skills/spells
 *
 *   Revision 1.7  2000/06/09 18:40:53  mud
 *   Purdans 'Editor Name List' for zoned added.
 *
 *   Revision 1.6  2000/04/20 18:28:34  mud
 *   Wrote 'tedit'.
 *
 *   Revision 1.5  2000/04/20 17:36:52  mud
 *   Fixed erronous saving of tildes '~' in mobs anything else not saving
 *   xml yet.
 *
 *   Revision 1.4  2000/03/15 12:00:30  mud
 *   Latest changes...
 *
 *   Revision 1.3  2000/03/08 11:41:47  mud
 *   New who func, minor modificantion to gossip command where new gossips are
 *   pre-pended to gossip now.
 *
 *   Revision 1.2  2000/02/21 14:00:18  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/*************************************************************************
*   File: modify.c                                      Part of CircleMUD *
*  Usage: Run-time modification of game variables                         *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "conf.h"
#include "sysdep.h"
#include "config.h"
#include "modify.h"
#include "structs.h"
#include "utils.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "comm.h"
#include "spells.h"
#include "mail.h"
#include "boards.h"
#include "olc.h"
#include "spec_procs.h"
#include "constants.h"
#include "guild.h"
#include "guild_parser.h"
#include "xmlcharsave.h"
#include "act.h"
#include "craft.h"

/* action modes for parse_action */
#define PARSE_FORMAT          0
#define PARSE_REPLACE         1
#define PARSE_HELP            2
#define PARSE_DELETE          3
#define PARSE_INSERT          4
#define PARSE_LIST_NORM       5
#define PARSE_LIST_NUM        6
#define PARSE_EDIT            7            

void show_string(struct descriptor_data *d, char *input);

char *string_fields[] =
{
  "name",
  "short",
  "long",
  "description",
  "title",
  "delete-description",
  "\n"
};


/* maximum length for text field x+1 */
int length[] =
{
  15,
  60,
  256,
  240,
  60
};


/* ************************************************************************
*  modification of mlloc'ed strings                                      *
************************************************************************ */

 /*  handle some editor commands */
void parse_action(int command, char *string, struct descriptor_data *d) 
{
  int indent = 0, rep_all = 0, flags = 0, total_len, replaced;
  register int j = 0;
  int i, line_low, line_high;
  char *s, *t, temp;

  switch (command) {
   case PARSE_HELP:
     sprintf(buf,  
             "Editor command formats: /<letter>\r\n\r\n"
             "/a         -  aborts editor\r\n"
             "/c         -  clears buffer\r\n"
             "/d#        -  deletes a line #\r\n"
             "/e# <text> -  changes the line at # with <text>\r\n"
             "/f         -  formats text\r\n"
             "/fi        -  indented formatting of text\r\n"
             "/h         -  list text editor commands\r\n"
             "/i# <text> -  inserts <text> before line #\r\n"
             "/l         -  lists buffer\r\n"
             "/n         -  lists buffer with line numbers\r\n"
             "/r 'a' 'b' -  replace 1st occurance of text <a> in buffer with <b>'\r\n"
             "/ra 'a' 'b'-  replace all occurances of text <a> within buffer with <b>\r\n"
             "              usage: /r[a] 'pattern' 'replacement'\r\n"
             "/s         -  saves text and exits\r\n");
       SEND_TO_Q(buf, d);
       break;
     case PARSE_FORMAT:
       while (isalpha(string[j]) && j < 2) {
        switch (string[j]) {
         case 'i':
           if (!indent) {
              indent = 1;
              flags += FORMAT_INDENT;
           }
           break;
         default: 
           break;
        }
        j++;
       }
       format_text(d->str, flags, d, d->max_str);
       sprintf(buf, "Text formatted with%s indent.\r\n", (indent ? "" : "out"));
       SEND_TO_Q(buf, d);
       break;
     case PARSE_REPLACE:
       if ((d->str) == NULL) {
         break;
       }
       if (!strlen(*d->str)) {
     SEND_TO_Q("Buffer is empty!\r\n",d);
     break;
       }
       while (isalpha(string[j]) && j < 2) {
        switch (string[j]) {
         case 'a':
           if (!indent) {
              rep_all = 1;
           }
           break;
         default:
           break;
        }
        j++;
       }
       s = strtok(string, "'");
       if (s == NULL) {
        SEND_TO_Q("Invalid format.\r\n", d);
        return;
       }
       s = strtok(NULL, "'");
       if (s == NULL) {
        SEND_TO_Q("Target string must be enclosed in single quotes.\r\n", d);
        return;
       }
       t = strtok(NULL, "'");
       if (t == NULL) {
        SEND_TO_Q("No replacement string.\r\n", d);
        return;
       }
       t = strtok(NULL, "'");
       if (t == NULL) {
        SEND_TO_Q("Replacement string must be enclosed in single quotes.\r\n",d);
        return;
       }
       total_len = ((strlen(t) - strlen(s)) + strlen(*d->str));
       if (total_len <= d->max_str) {
        if ((replaced = replace_str(d->str, s, t, rep_all, d->max_str)) > 0) {
           sprintf(buf, "Replaced %d occurance%sof '%s' with '%s'.\r\n", replaced, ((replaced != 1)?"s ":" "), s, t);
           SEND_TO_Q(buf, d);
        }
        else if (replaced == 0) {
           sprintf(buf, "String '%s' not found.\r\n", s);
           SEND_TO_Q(buf, d);
        }
        else {
           SEND_TO_Q("ERROR: Replacement string causes buffer overflow, aborted replace.\r\n", d);
        }
       }
       else
       SEND_TO_Q("Not enough space left in buffer.\r\n", d);
       break;
     case PARSE_DELETE:
       switch (sscanf(string, " %d - %d ", &line_low, &line_high)) {
        case 0:
        SEND_TO_Q("You must specify a line number or range to delete.\r\n", d);
        return;
        case 1:
        line_high = line_low;
        break;
        case 2:
        if (line_high < line_low) {
           SEND_TO_Q("That range is invalid.\r\n", d);
           return;
        }
        break;
       }

       i = 1;
       total_len = 1;
       if ((s = *d->str) == NULL) {
        SEND_TO_Q("Buffer is empty.\r\n", d);
        return;
       }
       if (line_low > 0) {
                while (s && (i < line_low))
          if ((s = strchr(s, '\n')) != NULL) {
             i++;
             s++;
          }
        if ((i < line_low) || (s == NULL)) {
           SEND_TO_Q("Line(s) out of range; not deleting.\r\n", d);
           return;
        }

        t = s;
        while (s && (i < line_high))
          if ((s = strchr(s, '\n')) != NULL) {
             i++;
             total_len++;
             s++;
          }
        if ((s) && ((s = strchr(s, '\n')) != NULL)) {
           s++;
           while (*s != '\0') *(t++) = *(s++);
        }
        else total_len--;
        *t = '\0';
        RECREATE(*d->str, char, strlen(*d->str) + 3);
        sprintf(buf, "%d line%sdeleted.\r\n", total_len,
                ((total_len != 1)?"s ":" "));
    d->character->desc->current_text_line -= total_len;
        SEND_TO_Q(buf, d);
       }
       else {
        SEND_TO_Q("Invalid line numbers to delete must be higher than 0.\r\n",d);
        return;
       }
       break;
     case PARSE_LIST_NORM:
       /* note: my buf,buf1,buf2 vars are defined at 32k sizes so they
        * are prolly ok fer what i want to do here. */
       *buf = '\0';
       if (*string != '\0')
       switch (sscanf(string, " %d - %d ", &line_low, &line_high)) {
        case 0:
          line_low = 1;
          line_high = 999999;
          break;
        case 1:
          line_high = line_low;
          break;
       }
       else {
        line_low = 1;
        line_high = 999999;
       }
       if (line_low < 1) {
        SEND_TO_Q("Line numbers must be greater than 0.\r\n", d);
        return;
       }
       if (line_high < line_low) {
        SEND_TO_Q("That range is invalid.\r\n", d);
        return;
       }
       *buf = '\0';
       if ((line_high < 999999) || (line_low > 1)) {
        sprintf(buf, "Current buffer range [%d - %d]:\r\n", line_low, line_high);
       }
       i = 1;
       total_len = 0;
       s = *d->str;
       while (s && (i < line_low))
       if ((s = strchr(s, '\n')) != NULL) {
          i++;
          s++;
       }
       if ((i < line_low) || (s == NULL)) {
        SEND_TO_Q("Line(s) out of range; no buffer listing.\r\n", d);
        return;
       }

       t = s;
       while (s && (i <= line_high))
       if ((s = strchr(s, '\n')) != NULL) {
          i++;
          total_len++;
          s++;
       }
       if (s)  {
        temp = *s;
        *s = '\0';
        strcat(buf, t);
        *s = temp;
       }
       else strcat(buf, t);
       /* this is kind of annoying.. will have to take a poll and see..
       sprintf(buf, "%s\r\n%d line%sshown.\r\n", buf, total_len,
             ((total_len != 1)?"s ":" "));
        */
       page_string(d, buf, TRUE);
       break;
     case PARSE_LIST_NUM:
       /* note: my buf,buf1,buf2 vars are defined at 32k sizes so they
        * are prolly ok fer what i want to do here. */
       *buf = '\0';
       if (*string != '\0')
       switch (sscanf(string, " %d - %d ", &line_low, &line_high)) {
        case 0:
          line_low = 1;
          line_high = 999999;
          break;
        case 1:
          line_high = line_low;
          break;
       }
       else {
        line_low = 1;
        line_high = 999999;
       }
       if (line_low < 1) {
        SEND_TO_Q("Line numbers must be greater than 0.\r\n", d);
        return;
       }
       if (line_high < line_low) {
        SEND_TO_Q("That range is invalid.\r\n", d);
        return;
       }
       *buf = '\0';
       i = 1;
       total_len = 0;
       s = *d->str;
       while (s && (i < line_low))
       if ((s = strchr(s, '\n')) != NULL) {
          i++;
          s++;
       }
       if ((i < line_low) || (s == NULL)) {
        SEND_TO_Q("Line(s) out of range; no buffer listing.\r\n", d);
        return;
       }

       t = s;
       while (s && (i <= line_high))
       if ((s = strchr(s, '\n')) != NULL) {
          i++;
          total_len++;
          s++;
          temp = *s;
          *s = '\0';
          sprintf(buf, "%s%3d: ", buf, (i-1));
          strcat(buf, t);
          *s = temp;
          t = s;
       }
       if (s && t) {
        temp = *s;
        *s = '\0';
        strcat(buf, t);
        *s = temp;
       }
       else if (t) strcat(buf, t);
       /* this is kind of annoying .. seeing as the lines are #ed
       sprintf(buf, "%s\r\n%d numbered line%slisted.\r\n", buf, total_len,
             ((total_len != 1)?"s ":" "));
        */
       page_string(d, buf, TRUE);
       break;

     case PARSE_INSERT:
       half_chop(string, buf, buf2);
       if (*buf == '\0') {
        SEND_TO_Q("You must specify a line number before which to insert text.\r\n", d);
        return;
       }
       line_low = atoi(buf);
       strcat(buf2, "\r\n");

       i = 1;
       *buf = '\0';
       if ((s = *d->str) == NULL) {
        SEND_TO_Q("Buffer is empty, nowhere to insert.\r\n", d);
        return;
       }
       if (line_low > 0) {
                while (s && (i < line_low))
          if ((s = strchr(s, '\n')) != NULL) {
             i++;
             s++;
          }
        if ((i < line_low) || (s == NULL)) {
           SEND_TO_Q("Line number out of range; insert aborted.\r\n", d);
           return;
        }
        temp = *s;
        *s = '\0';
        if ((strlen(*d->str) + strlen(buf2) + strlen(s+1) + 3) > d->max_str) {
           *s = temp;
           SEND_TO_Q("Insert text pushes buffer over maximum size, insert aborted.\r\n", d);
           return;
        }
        if (*d->str && (**d->str != '\0')) strcat(buf, *d->str);
        *s = temp;
        strcat(buf, buf2);
        if (s && (*s != '\0')) strcat(buf, s);
        RECREATE(*d->str, char, strlen(buf) + 3);
        strcpy(*d->str, buf);
    (d->character->desc->current_text_line)++;
        SEND_TO_Q("Line inserted.\r\n", d);
       }
       else {
        SEND_TO_Q("Line number must be higher than 0.\r\n", d);
        return;
       }
       break;

     case PARSE_EDIT:
       half_chop(string, buf, buf2);
       if (*buf == '\0') {
        SEND_TO_Q("You must specify a line number at which to change text.\r\n", d);
        return;
       }
       line_low = atoi(buf);
       strcat(buf2, "\r\n");

       i = 1;
       *buf = '\0';
       if ((s = *d->str) == NULL) {
        SEND_TO_Q("Buffer is empty, nothing to change.\r\n", d);
        return;
       }
       if (line_low > 0) {
        /* loop through the text counting /n chars till we get to the line */
                while (s && (i < line_low))
          if ((s = strchr(s, '\n')) != NULL) {
             i++;
             s++;
          }
        /* make sure that there was a THAT line in the text */
        if ((i < line_low) || (s == NULL)) {
           SEND_TO_Q("Line number out of range; change aborted.\r\n", d);
           return;
        }
        /* if s is the same as *d->str that means im at the beginning of the
         * message text and i dont need to put that into the changed buffer */
        if (s != *d->str) {
           /* first things first .. we get this part into buf. */
           temp = *s;
           *s = '\0';
           /* put the first 'good' half of the text into storage */
           strcat(buf, *d->str);
           *s = temp;
        }
        /* put the new 'good' line into place. */
        strcat(buf, buf2);
        if ((s = strchr(s, '\n')) != NULL) {
           s++;
           /* now put the last 'good' half of buffer into storage */
           strcat(buf, s);
        }
        /* check for buffer overflow */
        if (strlen(buf) > d->max_str) {
           SEND_TO_Q("Change causes new length to exceed buffer maximum size, aborted.\r\n",d);
           return;
        }
        /* change the size of the REAL buffer to fit the new text */
        RECREATE(*d->str, char, strlen(buf) + 3);
        strcpy(*d->str, buf);
        SEND_TO_Q("Line changed.\r\n", d);
       }
       else {
        SEND_TO_Q("Line number must be higher than 0.\r\n", d);
       return;
      }
      break;
    default:
      SEND_TO_Q("Invalid option.\r\n", d);
      mudlog("SYSERR: invalid command passed to parse_action", BRF, LVL_IMPL, TRUE);
      return;
   }
}

/* Add user input to the 'current' string (as defined by d->str) */
void string_add(struct descriptor_data *d, char *str)
{
  int terminator = 0, action;
  char *dummy = NULL;
  register int i = 2, j = 0;
  char actions[MAX_INPUT_LENGTH];
  FILE *fp;
  struct guild_info *g;
  struct guildie_info *guildie;
  
  /* determine if this is the terminal string, and truncate if so */
  /* Following three lines made to avoid crashes due to duplicate delimiters */
  dummy = str;
  while ((dummy = strchr(dummy, '~')))
    *dummy = '#';
  i = 2; /* Wonder why... */ 
  delete_doubledollar(str);
  
  if ((action = (*str == '/')))
  {
    while (str[i] != '\0')
    {
      actions[j] = str[i];
      i++;
      j++;
    }
    actions[j] = '\0';
    *str = '\0';
    switch (str[1])
    {
      case 'a':
      case 'A':
        terminator = 2; /* working on an abort message */
        break;
    case 'c':
    case 'C':
      if (d->str && *(d->str) && **(d->str))
        {
          free(*(d->str));
          *(d->str) = strdup("");
          SEND_TO_Q("Current buffer cleared.\r\n", d);
        }
      else
        SEND_TO_Q("Current buffer empty.\r\n", d);
      d->character->desc->current_text_line = 1;
      break;
    case 'd':
    case 'D':
      parse_action(PARSE_DELETE, actions, d);
      break;
    case 'e':
    case 'E':
      parse_action(PARSE_EDIT, actions, d);
      break;
    case 'f':
    case 'F':
      if (*(d->str))
        parse_action(PARSE_FORMAT, actions, d);
      else
        SEND_TO_Q("Current buffer empty.\r\n", d);
      break;
    case 'i':
    case 'I':
      if (*(d->str))
        parse_action(PARSE_INSERT, actions, d);
      else
        SEND_TO_Q("Current buffer empty.\r\n", d);
      break;
    case 'h':
    case 'H':
      parse_action(PARSE_HELP, actions, d);
      break;
    case 'l':
    case 'L':
      if (*d->str)
        parse_action(PARSE_LIST_NORM, actions, d);
      else
        SEND_TO_Q("Current buffer empty.\r\n", d);
      break;
    case 'n':
    case 'N':
      if (*d->str)
        parse_action(PARSE_LIST_NUM, actions, d);
      else
        SEND_TO_Q("Current buffer empty.\r\n", d);
      break;
    case 'r':
    case 'R':
      parse_action(PARSE_REPLACE, actions, d);
      break;
    case 's':
    case 'S':
      terminator = 1;
      if (!(*d->str))
        {
          *d->str = str_dup("\\c00");
        }
      else
        {
          *str = '\0';
        }
      break;
    default:
      SEND_TO_Q("Invalid option.\r\n", d);
      break;
    }
    }
  if (!action)
    {
      if (!(*d->str))
    {
      if (strlen(str) > d->max_str)
        {
          send_to_char("String too long - Truncated.\r\n",
               d->character);
          *(str + d->max_str) = '\0';
          action = 1;
        }
      CREATE(*d->str, char, strlen(str) + 3);
      strcpy(*d->str, str);
      (d->character->desc->current_text_line)++;
    }
      else
    {
      if (strlen(str) + strlen(*d->str) > d->max_str)
        {
          send_to_char("String too long, limit reached on message.  Last line ignored.\r\n",  d->character);
          action = 1;
        }
      else
        {
          char *tmpstr=NULL;
          if (NULL == *d->str)
        {
          *d->str = strdup(" "); // Create a dummy string if NULL
        }
          if ((strlen(*d->str) + strlen(str) + 3) < 1)
        { // This should never actually be able to happen!
          mlog("SYSERR: string_add failed! Attempt to add null-length string!");
          return;
        }
          /* Since realloc doesn't seem happy we're try mlloc */
          /*
        if (!(*d->str = (char *) realloc(*d->str, strlen(*d->str) +
        strlen(str) + 3))) {
        mlog("SYSERR: string_add failed! Out of memory?");
        exit(1);
        }
          */
	  CREATE( tmpstr, char, strlen(*d->str)+strlen(str)+3 );
          strcpy (tmpstr, *(d->str));
          strcat(tmpstr, str);
          free (*(d->str));
          *(d->str)=tmpstr;
          (d->character->desc->current_text_line)++;
        }
    }
    }
  if (terminator)
  {
    /* here we check for the abort option and reset the pointers */
    if ((terminator == 2) &&
    ((STATE(d) == CON_REDIT) ||
        (STATE(d) == CON_HEDIT) ||
     (STATE(d) == CON_MEDIT) ||
     (STATE(d) == CON_OEDIT) ||
     (STATE(d) == CON_QEDIT) ||
     (STATE(d) == CON_TEXTED)))
      {
        free(*(d->str));
        if (d->backstr)
          *(d->str) = d->backstr;
        else
          *(d->str) = NULL;
        d->backstr = NULL;
        d->str = NULL;
      }
      else if ((d->str) && (*d->str) && (**d->str == '\0'))
      {
        free(*(d->str));
        *d->str = NULL;
        *d->str = str_dup("An empty buffer...Hmm...A SBS-error perhaps?\r\n");
      }
        

      if (STATE(d) == CON_MEDIT)
        medit_disp_menu(d);
    
      if (STATE(d) == CON_OEDIT)
      {
        switch(OLC_MODE(d))
        {
          case OEDIT_ACTDESC:
            oedit_disp_menu(d);
            break;
          case OEDIT_EXTRADESC_DESCRIPTION:
            oedit_disp_extradesc_menu(d);
        }
      }
      if (CON_CRAFT == STATE(d)) {
        switch (GET_CRAFT_STATE(d->character)) {
          case CRAFT_EDESC:
            show_craft_edesc_menu(d->character, GET_CRAFT_EDESC(d->character));
            break;
          break;
        }
      }

      if (CON_TOOL == STATE(d)) {
        switch (GET_CRAFT_STATE(d->character)) {
          case TOOL_EDESC:
            show_tool_edesc_menu(d->character, GET_CRAFT_EDESC(d->character));
            break;
          break;
        }
      }

      else if (STATE(d) == CON_HEDIT)
        hedit_disp_menu(d);
      else if (STATE(d) == CON_REDIT) 
      {
        switch(OLC_MODE(d))
        {
          case REDIT_DESC:
            redit_disp_menu(d);
            break;
          case REDIT_EXIT_DESCRIPTION:
            redit_disp_exit_menu(d);
            break;
          case REDIT_EXTRADESC_DESCRIPTION:
            redit_disp_extradesc_menu(d);
            break;
        }
      }

      if (!d->connected && (PLR_FLAGGED(d->character, PLR_MAILING)))
      {
        if ((terminator == 1) && *d->str) 
        {
          if (d->mail_to[0] == -1) 
          { /* guildmail */
            g = guilds_data;
            while (g) 
            {
              if (g->id == d->mail_to[2]) break;
              g = g->next;
            }
            guildie = g->guildies;
            while (guildie) 
            {
              if ((guildie->idnum != d->mail_to[1]) && (STATUS_FLAGGED(guildie, STATUS_MEMBER)))
                store_mail(guildie->idnum, d->mail_to[1], *d->str);
              guildie = guildie->next;
            }
          } 
          else 
            for (i = 0;i < 5;i++)
              if (d->mail_to[i]) store_mail(d->mail_to[i], GET_IDNUM(d->character), *d->str);
          SEND_TO_Q("Message sent!\r\n", d);
        }
        else
          SEND_TO_Q("Mail aborted.\r\n", d);

        for (i = 0;i < 5;i++) d->mail_to[i] = 0;
        free(*d->str);
        free(d->str);
        act("$n stops writing and puts the pencil down.", TRUE, d->character, 0, 0, TO_ROOM);
      }
      else if (d->mail_to[0] >= BOARD_MAGIC)
      {
        Board_save_board((MSG_BOARD*)d->mail_to[1]);
        SEND_TO_Q("Message posted, use 'REMOVE <post #>' if you want to remove it.\r\n", d);
        d->mail_to[0] = 0;
        act("$n stops writing and puts the pencil down.", TRUE, d->character, 0, 0, TO_ROOM);
      }
      else if (STATE(d) == CON_GOSSIP)
      {
        if ((terminator == 1) && *d->str)
        {
          if (!(fp = fopen(GOSSIP_FILE, "w")))
          {
            SEND_TO_Q("Major muck up in this function. SEE A WEAVER ASAP!\r\n", d);
            return;
          }
          strip_string(*d->str);
          strip_string(gossip);
          fprintf(fp, "%s\n%s", *d->str, gossip);
          fclose(fp);
          free(gossip);
          gossip = NULL;
          file_to_string_alloc(GOSSIP_FILE, &gossip);
          SEND_TO_Q("Gossip added!\r\n", d);
        }
        else
          SEND_TO_Q("Gossip aborted!\r\n", d);
        free(*d->str);
        free(d->str);
        STATE(d) = CON_PLAYING;
        act("$n stops gossiping and looks around guiltily.", TRUE, d->character, 0, 0, TO_ROOM);
      }
      else if (STATE(d) == CON_TEXTED) 
      {
        if (terminator == 1) 
        {
          if (!(fp = fopen((char *)OLC_STORAGE(d), "w"))) 
          {
            sprintf(buf, "SYSERR: Can't write file '%s'.", OLC_STORAGE(d));
            mlog(buf);
          } 
          else 
          {
            if (*d->str) 
            {
              strip_string(*d->str);
              fputs(*d->str, fp);
            }
            fclose(fp);
            sprintf(buf, "%s saves '%s'.", GET_NAME(d->character), OLC_STORAGE(d));
            xlog(SYS_OLC, MAX(LVL_GOD, GET_INVIS_LEV(d->character)), buf, TRUE);
            SEND_TO_Q("Saved.\r\n", d);
          }
        }
        else SEND_TO_Q("Edit aborted.\r\n", d);
        act("$n stops editing some scrolls.", TRUE, d->character, 0, 0, TO_ROOM);
        free(OLC_STORAGE(d));
        OLC_STORAGE(d) = NULL;
        free(d->olc);
        STATE(d) = CON_PLAYING;
      }
      else if (!d->connected && d->character && !IS_NPC(d->character))
      {
        if (terminator == 1)
        {
          if (strlen(*d->str) == 0)
          {
            free(*d->str);
            *d->str = NULL;
          }
        }
        else
        {
          free(*d->str);
          if (d->backstr)
            *d->str = d->backstr;
          else *d->str = NULL;
          d->backstr = NULL;
          SEND_TO_Q("Message aborted.\r\n", d);
        }
      }
      if (d->character && !IS_NPC(d->character))
        REMOVE_BIT(PLR_FLAGS(d->character), PLR_WRITING | PLR_MAILING);
      if (d->backstr) free(d->backstr);
      d->backstr = NULL;
      d->str = NULL;
  }
  else if (!action)
    strcat(*d->str, "\r\n");
}



/* **********************************************************************
*  Modification of character skills                                     *
********************************************************************** */

ACMD(do_skillset)
{
  struct char_data *vict;
  char name[100], buf2[100], buf[100], help[MAX_STRING_LENGTH];
  int skill, value, i, qend, fromfile=0;

  vict=NULL;

  argument = one_argument(argument, name);

  if (!strcasecmp("file", name)) {
    argument = one_argument(argument, name);
      if (NULL ==(vict=xmlload_char(name))) {
        sprintf(buf, "No such player file (%s) found.\r\n", CAP(name));
        send_to_char(buf, ch);
        return;
      } else {
        GET_POS(vict) = POS_STANDING;
        fromfile=1;
      }
  }

  if (!*name) {			/* no arguments. print an informative text */
    send_to_char("Syntax: skillset <name> '<skill>' <value>\r\n", ch);
    send_to_char("Syntax: skillset file <name> '<skill>' <value>\r\n", ch);
    strcpy(help, "Skill being one of the following:\n\r");
    for (i = 0; *spells[i] != '\n'; i++) {
      if (*spells[i] == '!')
    continue;
      sprintf(help + strlen(help), "%18s", spells[i]);
      if (i % 4 == 3) {
    strcat(help, "\r\n");
    send_to_char(help, ch);
    *help = '\0';
      }
    }
    if (*help)
      send_to_char(help, ch);
    send_to_char("\n\r", ch);
    return;
  }

  if (!fromfile) {
    if (!(vict = get_char_vis(ch, name))) {
      send_to_char(NOPERSON, ch);
      return;
    }
  }
  skip_spaces(&argument);

  /* If there is no chars in argument */
  if (!*argument) {
    send_to_char("Skill name expected.\n\r", ch);
    return;
  }
  if (*argument != '\'') {
    send_to_char("Skill must be enclosed in: ''\n\r", ch);
    return;
  }
  /* Locate the last quote && lowercase the magic words (if any) */

  for (qend = 1; *(argument + qend) && (*(argument + qend) != '\''); qend++)
    *(argument + qend) = LOWER(*(argument + qend));

  if (*(argument + qend) != '\'') {
    send_to_char("Skill must be enclosed in: ''\n\r", ch);
    return;
  }
  strcpy(help, (argument + 1));
  help[qend - 1] = '\0';
  if ((skill = find_skill_num(help)) <= 0) {
    send_to_char("Unrecognized skill.\n\r", ch);
    return;
  }
  argument += qend + 1;		/* skip to next parameter */
  argument = one_argument(argument, buf);

  if (!*buf) {
    send_to_char("Learned value expected.\n\r", ch);
    return;
  }
  value = atoi(buf);
  if (value < 0) {
    send_to_char("Minimum value for learned is 0.\n\r", ch);
    return;
  }
  if (value > 100) {
    send_to_char("Max value for learned is 100.\n\r", ch);
    return;
  }
  if (IS_NPC(vict)) {
    send_to_char("You can't set NPC skills.\n\r", ch);
    return;
  }

  if(meets_skillspell_reqs(vict, skill)){
    if (fromfile)
      sprintf(buf2, "%s changed OFFLINE PLAYER %s's %s to %d.", GET_NAME(ch), GET_NAME(vict), spells[skill], value);
    else
      sprintf(buf2, "%s changed %s's %s to %d.", GET_NAME(ch), GET_NAME(vict), spells[skill], value);

    mudlog(buf2, BRF, -1, TRUE);
    mlog(buf2); 
    xlog(SYS_SKILLSET, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), buf2, TRUE);

    SET_SKILL(vict, skill, value);

    sprintf(buf2, "You change %s's %s to %d.\n\r", GET_NAME(vict),
      spells[skill], value);
    send_to_char(buf2, ch);
    do_save(vict,"",0,0);
  }
  else{
    sprintf(buf2, "They do not have the approprite talents or special skills to have this spell/skill.\r\n");
    send_to_char(buf2, ch);
  }
  if (fromfile) {
    xmlsave_char(vict);
    free_char(vict);
  }
}

ACMD(do_instruct)
{
  struct char_data *vict;
  char name[100], skillname[100];
  int skill, cost = 0, type = 0;

  if (PLR_FLAGGED(ch, PLR_NOGAIN)) {
    send_to_char("Sorry...You are not allowed to instruct other players as\r\n", ch);
    send_to_char("you probably have skills that others shouldn't have.\r\n", ch);
    return;
  }
  argument = one_argument(argument, name);
  argument++;
  strcpy(skillname,argument);	/* change back to two_args() if this doesn't work */

  if (!*name || !*skillname) {			
    send_to_char("Syntax: instruct <name> <skill>\r\n", ch);
    return;
  }
  if (!(vict = get_char_room_vis(ch, name))) {
    send_to_char(NOPERSON, ch);
    return;
  }
  if (IS_NPC(vict)) {
    send_to_char("Can't teach a mob.\r\n", ch);
    return;
  }
  if (vict == ch) {
    send_to_char("You can't teach YOURSELF!\r\n", ch);
    return;
  }
  if ((skill = find_skill_num(skillname)) <= 0) {
    send_to_char("Heh? What skill is that?\n\r", ch);
    return;
  }
  if (spell_info[skill].cost[(int)GET_CLASS(vict)] <= 0) {
    send_to_char("You can't instruct him/her in that skill.\r\n", ch);
    return;
  }
  if (GET_SKILL(ch, skill) < 80) {
    send_to_char("You're not good enough with that skill.\r\n",ch);
    return;
  }
  if (GET_SKILL(vict, skill)) {
    act("$N already knows that skill.", 0, ch, 0, vict, TO_CHAR);
    return;
  }
  type = spell_info[skill].type;
  if ((type != SKILL_NORMAL) && (type != SPELL_NORMAL)) {
    act("You can only teach $N normal skills or spells.", 0, ch, 0, vict, TO_CHAR);
    return;
  }
  if (type == SPELL_NORMAL) {
    if (cant_channel(ch)) {
      send_to_char("You gotta be able to channel in order to teach ppl weaves!\r\n",ch);
      return;
    }
    if (cant_channel(vict)) {
      send_to_char("Your victim can't channel! Useless!\r\n",ch);
      return;
    }
    if (GET_SEX(vict) != GET_SEX(ch)) {
      send_to_char("You can't train people of another sex weaves!\r\n",ch);
      return;
    }
    if (!check_weaves(skill, vict)) {
      asend_to_char(ch, "You can't train %s that weave, %s isn't advanced enough a channeler to learn that weave.\r\n", GET_NAME(vict), HSSH(vict));
      return;
    }
  }
  cost =  spell_info[skill].cost[(int)GET_CLASS(vict)];
  if ((cost < 0) || (cost > 50))   /* Sanity check limiter */
    cost = 5;

  if (cost > GET_TRAINS(vict)) {
    act("$N doesn't have enough trains.", 0, ch, 0, vict, TO_CHAR);
    return;
  }
  if ( ((GET_R_IDLE(ch) < 10) || (GET_R_IDLE(vict) < 10)) &&  GET_LEVEL(ch) < LVL_IMMORT ) {
    send_to_char("You haven't been together in the same room long enough.\r\n",ch);
    return;
  }
  if(meets_skillspell_reqs(vict, skill)){
    act("Ok. you teach $N something new.", 0, ch, 0, vict, TO_CHAR);
    sprintf(buf, "$n teaches you the %s %s!", 
       (type == SKILL_NORMAL)?"skill":"weave", CAP(skillname));
    act(buf, 0, ch, 0, vict, TO_VICT);
    act("$n teaches $N something new.", 0, ch, 0, vict, TO_ROOM);
    SET_SKILL(vict,skill,1);  
    GET_TRAINS(vict) -= cost;
    GET_R_IDLE(ch) = 0;  /* So they can't cheat */
    GET_R_IDLE(vict) = 0;

    alog("(GC) %s has instructed %s in %s!",GET_NAME(ch),GET_NAME(vict),skillname);
  }
  else{
    sprintf(buf, "They do not have the appropriate talents or special skills to have this spell/skill.\r\n");
    send_to_char(buf, ch);
  }
}

int meets_skillspell_reqs(struct char_data *vict, int skill){
   char buf[100];
   int num;
   sprintf(buf, "%s", spells[skill]);
   
   /******************************************************************************
   * Here is were the skill/spell requirements go. use the following algorithm...
   * if (strcasecmp(buf, <skillname>){
   *    for(num = 0; ((num < NUM_TALENTS) && (strcasecmp(<Name of talent>, master_talent_list[num].name)));num++)
   *    if (all the requiremnts are met)
   *        return 1
   *    else
   *        return 0
   ****************************************************************************/

   if (!strcasecmp(buf, "Heal gentling/stilling")){
      for(num = 0; ((num < NUM_TALENTS) && (strcasecmp("Heal Stilling", master_talent_list[num].name)));num++);
      if (IS_TALENT_SET(GET_SPSKILLS1(vict), GET_SPSKILLS2(vict), num))
         return 1;
      else
         return 0;
   }

   if (!strcasecmp(buf, "Gate")){
      for(num = 0; ((num < NUM_TALENTS) && (strcasecmp("Travelling", master_talent_list[num].name)));num++);
      if (IS_TALENT_SET(GET_SPSKILLS1(vict), GET_SPSKILLS2(vict), num))
         return 1;
      else
         return 0;
   }

   if (!strcasecmp(buf, "skim")){
      for(num = 0; ((num < NUM_TALENTS) && (strcasecmp("Skimming", master_talent_list[num].name)));num++);
      if (IS_TALENT_SET(GET_SPSKILLS1(vict), GET_SPSKILLS2(vict), num))
         return 1;
      else
         return 0;
   }

   if (!strcasecmp(buf, "tar-gate")){
      for(num = 0; ((num < NUM_TALENTS) && (strcasecmp("Gating into TAR", master_talent_list[num].name)));num++);
      if (IS_TALENT_SET(GET_SPSKILLS1(vict), GET_SPSKILLS2(vict), num))
         return 1;
      else
         return 0;
   }

   return 1;  /*only if skill/spell has no reqs.*/
}

/*********************************************************************
* New Pagination Code
* Michael Buselli submitted the following code for an enhanced pager
* for CircleMUD.  All functions below are his.  --JE 8 Mar 96
*
*********************************************************************/

#define PAGE_WIDTH      80

/* Traverse down the string until the begining of the next page has been
 * reached.  Return NULL if this is the last page of the string.
 */
char *next_page(char *str, int lines)
{
  int col = 1, line = 1, spec_code = FALSE;

  for (;; str++) {
    /* If end of string, return NULL. */
    if (*str == '\0')
      return NULL;

    /* If we're at the start of the next page, return this fact. */
    else if (line > lines)
      return str;
    /* Check for the begining of an ANSI color code block. */
    else if (*str == '\x1B' && !spec_code)
      spec_code = TRUE;

    /* Check for the end of an ANSI color code block. */
    else if (*str == 'm' && spec_code)
      spec_code = FALSE;

    /* Check for everything else. */
    else if (!spec_code) {
      /* Carriage return puts us in column one. */
      if (*str == '\r')
    col = 1;
      /* Newline puts us on the next line. */
      else if (*str == '\n')
    line++;

      /* We need to check here and see if we are over the page width,
       * and if so, compensate by going to the begining of the next line.
       */
//      else if (col++ > PAGE_WIDTH) {
//    col = 1;
//    line++;
//      }
    }
  }
}


/* Function that returns the number of pages in the string. */
int count_pages(char *str, int lines)
{
  int pages;

  for (pages = 1; (str = next_page(str, lines)); pages++);
  return pages;
}


/* This function assigns all the pointers for showstr_vector for the
 * page_string function, after showstr_vector has been allocated and
 * showstr_count set.
 */
void paginate_string(char *str, struct descriptor_data *d)
{
  int i, l;

  if( d->showstr_count )
    *(d->showstr_vector) = str;

  l = MAX(GET_LINES(d->character), 10);

  for (i = 1; i < d->showstr_count && str; i++)
    str = d->showstr_vector[i] = next_page(str, l);

  d->showstr_page = 0;
}


/* The call that gets the paging ball rolling... */
void page_string(struct descriptor_data *d, char *str, int keep_internal)
{
  int lines;

  if (!d)
    return;

  if (!str || !*str) 
  {
    send_to_char("", d->character);
    return;
  }
  lines = MAX( GET_LINES(d->character), 10 );
  d->showstr_count = count_pages( str, lines );
  CREATE( d->showstr_vector, char *, d->showstr_count );

  if( keep_internal ) 
  {
    d->showstr_head = str_dup(str);
    paginate_string( d->showstr_head, d );
  } 
  else
    paginate_string( str, d );

  show_string(d, "");
}


/* The call that displays the next page. */
void show_string(struct descriptor_data *d, char *input)
{
  char buffer[MAX_STRING_LENGTH];
  int diff;

  one_argument(input, buf);

  /* Q is for quit. :) */
  if (LOWER(*buf) == 'q') {
    free(d->showstr_vector);
    d->showstr_count = 0;
    if (d->showstr_head) {
      free(d->showstr_head);
      d->showstr_head = 0;
    }
    return;
  }
  /* R is for refresh, so back up one page internally so we can display
   * it again.
   */
  else if (LOWER(*buf) == 'r')
    d->showstr_page = MAX(0, d->showstr_page - 1);

  /* B is for back, so back up two pages internally so we can display the
   * correct page here.
   */
  else if (LOWER(*buf) == 'b')
    d->showstr_page = MAX(0, d->showstr_page - 2);

  /* Feature to 'goto' a page.  Just type the number of the page and you
   * are there!
   */
  else if (isdigit(*buf))
    d->showstr_page = MAX(0, MIN(atoi(buf) - 1, d->showstr_count - 1));

  else if (*buf) {
    send_to_char(
          "Valid commands while paging are ENTER, Q, R, B, or a numeric value.\r\n",
          d->character);
    return;
  }
  /* If we're displaying the last page, just send it to the character, and
   * then free up the space we used.
   */
  if (d->showstr_page + 1 >= d->showstr_count) {
    send_to_char(d->showstr_vector[d->showstr_page], d->character);
    free(d->showstr_vector);
    d->showstr_count = 0;
    if (d->showstr_head) {
      free(d->showstr_head);
      d->showstr_head = NULL;
    }
  }
  /* Or if we have more to show.... */
  else {
    strncpy(buffer, d->showstr_vector[d->showstr_page],
        diff = ((int) d->showstr_vector[d->showstr_page + 1])
        - ((int) d->showstr_vector[d->showstr_page]));
    buffer[diff] = '\0';
    send_to_char(buffer, d->character);
    d->showstr_page++;
  }
}

/*
 * Looks in ptable if player <name> exists...OBS! <name> must match exactly (except for case)
 * I.e Stef != stefan, but DeRAn == deraN
 */
int ptable_lookup(char *name)
{
  int i = 0;

  if (NULL == name || !*name)
    return 0;
  
  for (i = 0; i <= top_of_p_table; i++) {
    if (!strcasecmp(name,(player_table+i)->name))
      return 1;
  }  

  return 0;
}  

