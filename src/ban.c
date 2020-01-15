/******************************************************************************
 * $Id: ban.c,v 1.4 2003/12/23 05:28:48 fang Exp $
 *   
 * File: ban.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: ban.c,v 
 *   Revision 1.2  2002/07/28 13:56:03  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.5  2002/06/13 10:12:28  fang
 *   More free'ing stuff...
 *
 *   Revision 1.4  2002/05/27 10:52:48  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.3  2002/03/11 09:53:13  fang
 *   Re-wroote xnames code. Should be a bit safer now
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.5  2001/10/09 14:21:35  mud
 *   *** empty log message ***
 *
 *   Revision 1.4  2000/12/16 11:38:46  mud
 *   *** empty log message ***
 *
 *   Revision 1.3  2000/12/16 11:02:07  mud
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/02/21 14:00:16  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* ************************************************************************
*   File: ban.c                                         Part of CircleMUD *
*  Usage: banning/unbanning/checking sites and player names               *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "conf.h"
#include "sysdep.h"


#include "ban.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "config.h"
#include "dll.h"
#include "strutil.h"

struct ban_list_element *ban_list = NULL;

char *ban_types[] = {
  "no",
  "new",
  "select",
  "all",
  "ERROR"
};

void free_banned(void)
{
  struct ban_list_element *ban, *bnext;

  for (ban = ban_list; NULL != ban; ban = bnext) {
    bnext = ban->next;
    free(ban);
  }
  ban_list = NULL;
}

void load_banned(void)
{
  FILE *fl;
  int i, date;
  char site_name[BANNED_SITE_LENGTH + 1], ban_type[100];
  char name[MAX_NAME_LENGTH + 1];
  char reason[MAX_TITLE_LENGTH+1];
  struct ban_list_element *next_node;

  ban_list = 0;

  if (!(fl = fopen(BAN_FILE, "r"))) {
    mlog("SYSERR: Unable to open banfile");
    return;
  }
  while (fscanf(fl, " %s %s %d %s %[^\n]", ban_type, site_name, &date, name, reason) == 5) {
    CREATE(next_node, struct ban_list_element, 1);
    strncpy(next_node->site, site_name, BANNED_SITE_LENGTH);
    next_node->site[BANNED_SITE_LENGTH] = '\0';
    strncpy(next_node->name, name, MAX_NAME_LENGTH);
    next_node->name[MAX_NAME_LENGTH] = '\0';
    strncpy(next_node->reason, reason, MAX_TITLE_LENGTH);
    next_node->reason[MAX_TITLE_LENGTH] = '\0';
    next_node->date = date;

    for (i = BAN_NOT; i <= BAN_ALL; i++)
      if (!strcmp(ban_type, ban_types[i]))
    next_node->type = i;

    next_node->next = ban_list;
    ban_list = next_node;
  }

  fclose(fl);
}


int isbanned(char *hostname)
{
  int i;
  struct ban_list_element *banned_node;
  char *nextchar;

  if (!hostname || !*hostname)
    return (0);

  i = 0;
  for (nextchar = hostname; *nextchar; nextchar++)
    *nextchar = LOWER(*nextchar);

  for (banned_node = ban_list; banned_node; banned_node = banned_node->next)
    if (strstr(hostname, banned_node->site))	/* if hostname is a substring */
      i = MAX(i, banned_node->type);

  return i;
}


void _write_one_node(FILE * fp, struct ban_list_element * node)
{
  if (node) {
    _write_one_node(fp, node->next);
    fprintf(fp, "%s %s %ld %s %s\n", ban_types[node->type],
        node->site, (long) node->date, node->name, node->reason);
  }
}



void write_ban_list(void)
{
  FILE *fl;

  if (!(fl = fopen(BAN_FILE, "w"))) {
    mlog("SYSERR: write_ban_list failed to open file.");
    return;
  }
  _write_one_node(fl, ban_list);/* recursively write from end to start */
  fclose(fl);
  return;
}


ACMD(do_ban)
{
  char flag[MAX_INPUT_LENGTH], site[MAX_INPUT_LENGTH],
       format[MAX_INPUT_LENGTH], reason[MAX_INPUT_LENGTH],
       *nextchar, *timestr;
  int i;
  struct ban_list_element *ban_node;

  *buf = '\0';

  if (!*argument) {
    if (!ban_list) {
      send_to_char("No sites are banned.\r\n", ch);
      return;
    }
    strcpy(format, "%-25.25s  %-8.8s  %-10.10s  %-10.10s %-15.15s\r\n");
    sprintf(buf, format,
        "Banned Site Name",
        "Ban Type",
        "Banned On",
        "Banned By",
            "Reason");
    send_to_char(buf, ch);
    sprintf(buf, format,
        "---------------------------------",
        "---------------------------------",
        "---------------------------------",
        "---------------------------------",
            "---------------------------------");
    send_to_char(buf, ch);

    for (ban_node = ban_list; ban_node; ban_node = ban_node->next) {
      if (ban_node->date) {
    timestr = asctime(localtime(&(ban_node->date)));
    *(timestr + 10) = 0;
    strcpy(site, timestr);
      } else
    strcpy(site, "Unknown");
      sprintf(buf, format, ban_node->site, ban_types[ban_node->type], site,
          ban_node->name, ban_node->reason);
      send_to_char(buf, ch);
    }
    return;
  }
  half_chop(argument, flag, buf);
  half_chop(buf, site, reason);

  if (!*site || !*flag || !*reason) {
    send_to_char("Usage: ban {all | select | new} <site_name> <reason>\r\n", ch);
    return;
  }
  if (!(!str_cmp(flag, "select") || !str_cmp(flag, "all") || !str_cmp(flag, "new"))) {
    send_to_char("Flag must be ALL, SELECT, or NEW.\r\n", ch);
    return;
  }
  for (ban_node = ban_list; ban_node; ban_node = ban_node->next) {
    if (!str_cmp(ban_node->site, site)) {
      send_to_char("That site has already been banned -- unban it to change the ban type.\r\n", ch);
      return;
    }
  }

  CREATE(ban_node, struct ban_list_element, 1);
  strncpy(ban_node->site, site, BANNED_SITE_LENGTH);
  for (nextchar = ban_node->site; *nextchar; nextchar++)
    *nextchar = LOWER(*nextchar);
  ban_node->site[BANNED_SITE_LENGTH] = '\0';
  strncpy(ban_node->name, GET_NAME(ch), MAX_NAME_LENGTH);
  ban_node->name[MAX_NAME_LENGTH] = '\0';
  ban_node->date = time(0);
  strncpy(ban_node->reason, reason, MAX_TITLE_LENGTH);
  ban_node->reason[MAX_TITLE_LENGTH] = '\0';

  for (i = BAN_NEW; i <= BAN_ALL; i++)
    if (!str_cmp(flag, ban_types[i]))
      ban_node->type = i;

  ban_node->next = ban_list;
  ban_list = ban_node;

  sprintf(buf, "%s has banned %s for %s players. Reason: %s", GET_NAME(ch), site,
      ban_types[ban_node->type], reason);
  xlog(SYS_GOD, MAX(LVL_GOD, GET_INVIS_LEV(ch)), buf, TRUE);
  send_to_char("Site banned.\r\n", ch);
  write_ban_list();
}


ACMD(do_unban)
{
  char site[80];
  struct ban_list_element *ban_node, *temp;
  int found = 0;

  one_argument(argument, site);
  if (!*site) {
    send_to_char("A site to unban might help.\r\n", ch);
    return;
  }
  ban_node = ban_list;
  while (ban_node && !found) {
    if (!str_cmp(ban_node->site, site))
      found = 1;
    else
      ban_node = ban_node->next;
  }

  if (!found) {
    send_to_char("That site is not currently banned.\r\n", ch);
    return;
  }
  REMOVE_FROM_LIST(ban_node, ban_list, next);
  send_to_char("Site unbanned.\r\n", ch);
  sprintf(buf, "%s removed the %s-player ban on %s.",
      GET_NAME(ch), ban_types[ban_node->type], ban_node->site);
  xlog(SYS_GOD, MAX(LVL_GOD, GET_INVIS_LEV(ch)), buf, TRUE);

  free(ban_node);
  write_ban_list();
}


/**************************************************************************
 *  Code to check for invalid names (i.e., profanity, etc.)		  *
 *  Written by Sharon P. Goza						  *
 * 
 *  Re-written. Damn crap!
 **************************************************************************/

static DLL __xlist;

static int newstr(ELEMENT A, ELEMENT B)
{
  char *a = (char *)A, *b = (char *)B;

  return (NULL == strstr(a,b) ? 0 : 1);
}

static void free_xlist(ELEMENT e)
{
  free(e);
}

void free_invalid_list(void)
{
  deleteAll(__xlist,free_xlist);
  __xlist = NULL;
}

int Valid_Name(char *newname)
{
  int i;
  char *lname = strlower(newname);

  /* return valid if list doesn't exist */
  if (NULL == __xlist || !getNumber(__xlist))
    return 1;

  /* Does the desired name contain a string in the invalid list? */
  i = search(__xlist,lname,newstr);
 
  free(lname);
  return (i);
}


void Read_Invalid_List(void)
{
  FILE *fp;
  char string[15000];

  if (NULL == __xlist)
    __xlist = createDll();

  if (!(fp = fopen(XNAME_FILE, "r"))) {
    mlog("SYSERR: Unable to open invalid name file");
    return;
  }

  while (fgets(string,14000,fp) && !feof(fp)) {
    if (strlen(string) > 1) {
      string[strlen(string) - 1] = '\0';
      append(__xlist,strdup(string));
    }
  }

  fclose(fp);
}

void add_invalid_name(char *name) {
  FILE *fp = NULL;
  char *lname;

  if (!name || !*name)
    return;

  if (!(fp = fopen(XNAME_FILE, "a"))) {
    mlog("SYSERR: Unable to open invalid name file");
    return;
  }

  lname = strlower(name);
  fseek(fp,0,SEEK_END);
  fprintf(fp, "%s\n", lname);
  fclose(fp);
  append(__xlist,lname);
}

static int printfunc(ELEMENT a)
{
  char *n = (char *)a;

  if (strlen(buf) < (MAX_STRING_LENGTH - 500))
    sprintf(buf,"%s- %s\n",buf,n);

  return 0; 
}

ACMD(do_xname) 
{
  skip_spaces(&argument);

  if (!*argument) {
    buf[0] = '\0';
    sprintf(buf, "Invalid names:\r\n");

    map(__xlist,printfunc);
    send_to_char(buf, ch);
  }

  else if (!Valid_Name(argument))
    send_to_char("That is already an invalid name!\r\n", ch);
  else {

    if (strlen(argument) > 75) {
      send_to_char("Name is too long! 70 chars max!\r\n",ch);
      return;
    }
    add_invalid_name(argument);
    send_to_char(OK, ch);
  }
}
