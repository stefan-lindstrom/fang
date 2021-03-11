/******************************************************************************
 * $Id: utils.c,v 1.24 2003/03/18 13:21:55 fang Exp $
 *   
 * File: utils.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: utils.c,v $
 *   Revision 1.24  2003/03/18 13:21:55  fang
 *   Some mem. bugs reported by Valgrind crushed.
 *
 *   Revision 1.23  2003/02/02 15:18:01  fang
 *   *** empty log message ***
 *
 *   Revision 1.22  2002/12/12 00:15:59  fang
 *   Fisch:  added "cantpost" to protect boards to the PLR_ flags
 *
 *   Revision 1.21  2002/11/07 04:02:04  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.20  2002/11/06 04:43:18  fang
 *   Fisch: fixed some "uninitialized data used" errors
 *
 *   Revision 1.19  2002/10/16 19:18:49  fang
 *   Fischer: added ERROR xlog type
 *
 *   Revision 1.18  2002/10/12 06:30:15  fang
 *   fisch: converted boards to xml and made them dynamic not coded.
 *
 *   Revision 1.17  2002/10/08 21:12:31  fang
 *   fisch: included blockage of "BLEEDING" message in NOSPAM toggle
 *
 *   Revision 1.16  2002/09/23 05:52:42  fischer
 *   fisch: clearing out lua triggers part 1
 *
 *   Revision 1.15  2002/09/16 18:22:56  fang
 *   Put in WC guild retainers and some IC checks for retainers.
 *
 *   Revision 1.14  2002/09/15 07:17:25  fang
 *   Fisch: Removed or commented out the debug messages used to find the
 *   guild channel bug.
 *
 *   Revision 1.13  2002/09/15 03:45:13  fang
 *   Drath: Added an IMM toggle for XLOG tags
 *
 *   Revision 1.12  2002/09/15 00:50:08  fang
 *   Drath: XLOG update - Categories added to xlog display automatically
 *
 *   Revision 1.11  2002/09/14 05:52:00  fang
 *   fischer: added a temp fix to prevent non weavers from getting debug spam
 *
 *   Revision 1.10  2002/09/14 02:58:13  fang
 *   Drath: NH syslog for non-help check for chars less than level 20
 *
 *   Revision 1.9  2002/09/12 20:20:43  fang
 *   Fischer: took out the (type) debugging code that made it into CVS
 *
 *   Revision 1.8  2002/09/11 08:32:36  fang
 *   Fischer: Retainer xlog entries (and catagory) for name and title changes
 *
 *   Revision 1.7  2002/09/05 15:58:54  fang
 *   Drath: CHARMd mobs dont cry when they try to leave and fail.
 *
 *   Revision 1.6  2002/08/30 21:23:09  fang
 *   Put in retainers.
 *
 *   Revision 1.5  2002/08/27 21:29:39  fang
 *   Drath: Player Extra Descriptions
 *
 *   Revision 1.4  2002/08/20 21:03:50  fang
 *   Drath: Hunter GSKILLS
 *
 *   Revision 1.3  2002/07/28 13:56:05  fang
 *   Removed dmlloc...just to test.
 *
 *   Revision 1.2  2002/07/26 19:39:52  fang
 *   Dath: MYCOLOR mem leak fixed.  Thanks, Fred!
 *
 *   Revision 1.8  2002/07/12 04:51:32  fang
 *   *** empty log message ***
 *
 *   Revision 1.7  2002/07/12 03:37:33  fang
 *   Drath: MYCOLOR
 *
 *   Revision 1.6  2002/06/29 18:33:05  fang
 *   Drath: In-MUD syslog searching.  Replaces websyslog search
 *
 *   Revision 1.5  2002/06/25 19:23:51  fang
 *   Drath: XLOG Commit #2
 *
 *   Revision 1.4  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.3  2002/03/24 04:39:56  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.19  2002/01/24 21:33:21  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.18.2.2  2002/01/24 20:49:38  mud
 *   More cleanup. Removed files:  specialday.c string.c string.h
 *   Removed DONT_TRADE_WITH-stuff from shops.
 *
 *   Revision 1.18.2.1  2002/01/24 03:20:36  mud
 *   Removed some further unescessary files (player.[ch], roomprog.[ch]
 *   old2new.c). Cleaned up some other stuff (guild stuff mostly).
 *
 *   Revision 1.18  2002/01/15 14:01:52  mud
 *   *** empty log message ***
 *
 *   Revision 1.17  2001/10/06 15:30:04  mud
 *   *** empty log message ***
 *
 *   Revision 1.16  2001/10/03 14:34:19  mud
 *   *** empty log message ***
 *
 *   Revision 1.15  2001/09/19 05:52:58  mud
 *   Dynamic buffer and select-\>poll mod.
 *
 *   Revision 1.14  2001/09/12 06:32:04  mud
 *   *** empty log message ***
 *
 *   Revision 1.13  2001/09/06 05:41:19  mud
 *   *** empty log message ***
 *
 *   Revision 1.12  2001/05/22 09:07:07  mud
 *   DG-Scripts for Fang.
 *
 *   Revision 1.11  2001/01/19 17:51:51  mud
 *   Added line numbers to the editor.
 *
 *   Revision 1.10  2001/01/10 17:34:54  mud
 *   *** empty log message ***
 *
 *   Revision 1.9  2000/11/12 14:44:13  mud
 *   More support code for xml transforming, a few bugfixes to xmlchar code, and index_from_array().
 *
 *   Revision 1.8  2000/06/16 01:38:22  mud
 *   /f text formatting changed from 70 characters to 80. -Tesla
 *
 *   Revision 1.7  2000/05/18 10:25:18  mud
 *   *** empty log message ***
 *
 *   Revision 1.6  2000/05/16 13:03:38  mud
 *   *** empty log message ***
 *
 *   Revision 1.5  2000/05/01 11:07:43  mud
 *   Added a small helper function, index_from_array(), along with a description
 *   on how to use it.
 *
 *   Revision 1.4  2000/04/07 16:28:28  mud
 *   *** empty log message ***
 *
 *   Revision 1.3  2000/03/01 18:06:18  mud
 *   Skeleton code added for zone XML, as well as code for future percentage chance of zone cmds...
 *
 *   Revision 1.2  2000/02/21 14:00:20  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* ************************************************************************
*   File: utils.c                                       Part of CircleMUD *
*  Usage: various internal functions of a utility nature                  *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */
#include "strutil.h"
#include "spec_procs.h"
#include "conf.h"
#include "sysdep.h"
#include <stdarg.h>
#include "db.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "screen.h"
#include "spells.h"
#include "handler.h"
#include "interpreter.h"
#include "color.h"
#include "constants.h"
#include "retainer.h"

#define TO_WEAVER  0
#define TO_TARGET1 1 /* if target can't channel! */
#define TO_TARGET2 2 /* if target can channel but not of same sex */
#define TO_TARGET3 3 /* -------------||----------- of same sex    */
#define TO_REST1   4 /* To rest of room that can't channel        */
#define TO_REST2   5 /* To rest of room that cna channel, not of same sex */
#define TO_REST3   6 /* Se to target3! */

int dummy_int[MAX_SKILLS+1];

/* creates a random number in interval [from;to] */
int number(int from, int to)
{
  /* error checking in case people call number() incorrectly */
  if (from > to) {
    int tmp = from;
    from = to;
    to = tmp;
  }

  return ((random() % (to - from + 1)) + from);
}

void to_eavesdropper(char *tosend, long rnum) {

  struct eavesdrop_info *eavesd;
  struct char_data *tch=NULL;

  if (NULL != (eavesd = world[rnum].eaves) && tosend && *tosend) {
    while (eavesd) {
      if (eavesd->charname != NULL) {
        tch = get_char_pointer(eavesd->charname);
        if (!IS_NPC(tch)) {
          send_to_char("&n(&c!&n) ", tch);
          send_to_char(tosend, tch);
        }
      }
      eavesd=eavesd->next;
    }
  }
}

struct player_extradesc *find_playerxdesc(char *keywd, int dnum, struct char_data *ch) {

  struct player_extradesc *current;

  if (NULL != (current = GET_PLAYERXDESC(ch))) {
    while (current) {
      if (((current)->descnum) == dnum) {
        if (is_abbrev(keywd, current->keyword)) {
          return current;
        }
      }
      current=current->next;
    }
    return NULL;
  } else {
    return NULL;
  }
}

void rem_playerxdesc(struct player_extradesc *xdesc, struct char_data *ch) {

  struct player_extradesc *current, *last=0;
  int numfound=0;

  //last=calloc(1, sizeof(struct player_extradesc));

  if (NULL != (current = (ch)->player.xdesc)) {
    while (current) {
      numfound++;
      if (current == xdesc) {
        if (numfound > 1) {
          last->next = current->next;
        } else {
          if (current->next == NULL) {
            (ch)->player.xdesc = NULL;
          } else {
            (ch)->player.xdesc = current->next;
            return;
          }
        }
      }
      last=current;
      current=current->next;
    }
  }
}

void rem_eavesdropper(struct char_data *ch, long rnum) {

  struct eavesdrop_info *current, *last=0;
  int numfound=0;

//  last=calloc(1, sizeof(struct eavesdrop_info));

  if (NULL != (current = world[rnum].eaves)) {
    while (current) {
      numfound++;
      if (current->charname == GET_NAME(ch)) {
        if (numfound > 1) {
          last->next = current->next;
        } else {
          if (current->next == NULL) {
            world[rnum].eaves = NULL;
          } else {
            world[rnum].eaves = current->next;
            return;
          }
        }
      }
      last = current;
      current = current->next;
    }
  }
}

int check_and_notify_non_IC_receiver(struct char_data *actor, struct char_data *receiver, char *verb)
{
  if(!is_retainer(receiver) &&
     IS_SET(PRF_FLAGS(actor),PRF_IC) &&
     GET_LEVEL(actor)<LVL_CARPENTER &&
     !IS_SET(PRF_FLAGS(receiver),PRF_IC) &&
     GET_LEVEL(receiver)<LVL_IMMORT &&
     !PLR_FLAGGED(actor, PLR_MAILING | PLR_WRITING))
  {
    sprintf(buf, "$n %s something, but $e is In Character and you aren't.", verb);
    act(buf, TRUE, actor, 0, receiver, TO_VICT);
    return 1;
  }
  else
    return 0;
}

void add_playerxdesc(struct player_extradesc *xdesc, struct char_data *ch) {

  struct player_extradesc *current;

  if (NULL != (current = GET_PLAYERXDESC(ch))) {
    while (current) {
      if (current->next == NULL) {
        current->next=xdesc;
        return;
      }
      current=current->next;
    }
  } else {
    GET_PLAYERXDESC(ch) = xdesc;
  }
}

void add_eavesdropper(struct char_data *ch, long rnum, int dist) {

  struct eavesdrop_info *current, *new;

  if (NULL != (current = world[rnum].eaves)) {
    while (current) {
      if (current->next == NULL) {
        new = calloc(1, sizeof(struct eavesdrop_info));
        new->charname = GET_NAME(ch);
        new->distance = dist;
        new->next = NULL;
        current->next = new;
        return;
      }
    current=current->next;
    }
  } else {
    new = calloc(1, sizeof(struct eavesdrop_info));
    new->charname = GET_NAME(ch);
    new->distance = dist;
    new->next = NULL;
    world[rnum].eaves = new;
  }
}


/* simulates dice roll */
int dice(int number, int size)
{
  int sum = 0;

  if (size <= 0 || number <= 0)
    return 0;

  while (number-- > 0)
    sum += ((random() % size) + 1);

  return sum;
}


int MIN(int a, int b)
{
  return a < b ? a : b;
}


int MAX(int a, int b)
{
  return a > b ? a : b;
}

char *coltext(char *source, struct char_data * ch)
{
  char repstr[MAX_INPUT_LENGTH + MAX_INPUT_LENGTH];
  char *ptr = repstr;
  int found=0;
  strcpy(repstr, source);
  *source = 0;
  while(*ptr != '\0') {
    switch (*ptr) {
    case '\"':
      if (found == 0) {
        found=1;
        strncat(source, ptr, 1);
        strcat(source, GET_MYCOLOR(ch));  
      } else {
        found=0;
        strcat(source, "&n\0");
        strncat(source, ptr, 1);
      }
      break;
    default:
      strncat(source, ptr, 1);
      break;
    }
    ptr++;
  }
  return source;
}

/* Create a duplicate of a string */
char *str_dup(const char *source)
{
  char *new;

  if (NULL == source) {
    mlog("SYSERR:  str_dup()[utils.c]: source is NULL!");
    return NULL;
  }

  new = calloc(strlen(source)+1,sizeof(char));
  return (strcpy(new, source));
}


int c_strlen(const char *arg)
{
  GString *temp = NULL, *temp1 = NULL;
  int len = 0;

  if (arg)
    temp = g_string_new(arg);
  else
    return len;

  temp1 = proc_color(temp, 0);
  len = temp1->len;
  g_string_free(temp, TRUE);
  g_string_free(temp1, TRUE);

  return len;
}

/* str_cmp: a case-insensitive version of strcmp */
/* returns: 0 if equal, 1 if arg1 > arg2, -1 if arg1 < arg2  */
/* scan 'till found different or end of both                 */
int str_cmp(const char *arg1,const char *arg2)
{
  if (!arg1 || !arg2) {
    alog("Null args in str_cmp!!!");
    return -1;
  }
  return strcasecmp(arg1, arg2);
}


/* strn_cmp: a case-insensitive version of strncmp */
/* returns: 0 if equal, 1 if arg1 > arg2, -1 if arg1 < arg2  */
/* scan 'till found different, end of both, or n reached     */
int strn_cmp(const char *arg1, const char *arg2, int n)
{
  return strncasecmp(arg1, arg2, n);
}

/* log a death trap hit */
void log_death_trap(struct char_data * ch)
{
  char buf[150];

  sprintf(buf, "%s hit death trap #%d (%s)", GET_NAME(ch),
      world[ch->in_room].number, world[ch->in_room].name);
  mudlog(buf, BRF, LVL_IMMORT, TRUE);
}

/* Advanced log(). Can use printf()-format style logs */

void alog(char *fmt,...)
{
  va_list argp;
  char tmp[MAX_STRING_LENGTH*8];	/* nasty, but we shouldn't have longer messgs than this */

  if (NULL == fmt) {
    return;
  }

  va_start(argp,fmt);
  vsprintf(tmp,fmt,argp);
  va_end(argp);

  mlog(tmp);
}

/* writes a string to the log */
void mlog(char *str)
{
  time_t ct;
  char *tmstr;

  ct = time(0);
  tmstr = asctime(localtime(&ct));
  *(tmstr + strlen(tmstr) - 1) = '\0';
  fprintf(stderr, "%-19.19s :: %s\n", tmstr, str);
}


/* the "touch" command, essentially. */
int touch(char *path)
{
  FILE *fl;

  if (!(fl = fopen(path, "a"))) {
    mlog("SYSERR: touch() failed to open file.");
    return -1;
  } else {
    fclose(fl);
    return 0;
  }
}

void nhlog(char *str)
{
  char buf[MAX_STRING_LENGTH];
  struct descriptor_data *tch;

  sprintf(buf, "&W[ (NH) %s &W]&n\r\n", str);
  for (tch=descriptor_list;tch;tch=tch->next)
    if (!tch->connected && !PLR_FLAGGED(tch->character, PLR_WRITING))
      if (GET_LEVEL(tch->character) < LVL_IMMORT)
        if (IS_SET((tch->character)->player_specials->saved.command_groups, CG_NEWBIE))        
          send_to_char(buf, tch->character);

}

void xlog(int type, int level, char *str, byte file)
{
  char buf[MAX_STRING_LENGTH];
  struct descriptor_data *tch;
  char *tmp;
  time_t ct;

  ct = time(0);
  tmp = asctime(localtime(&ct));

  if (file)
    fprintf(stderr, "%19.19s :: %s\n", tmp, str);
  if (level < 0)
    return;

  for (tch=descriptor_list; tch; tch=tch->next)
    if (!tch->connected && !PLR_FLAGGED(tch->character, PLR_WRITING))
      if (GET_LEVEL(tch->character) >= level)
        if (GOT_SYSLOG(tch->character, (1 << type)))
	{ if (PRF2_FLAGGED(tch->character, PRF2_XLOGTAG))
            sprintf(buf, "&g[%3d][ (%s) %s &g]&n\r\n",level, extended_syslogs[type], str);
          else
            sprintf(buf, "&g[%3d][ %s &g]&n\r\n",level, str);

	  send_to_char(buf, tch->character);
        }
}

void axlog( int type, int level, byte file, char *fmt,...)
{
  va_list argp;
  char tmp[MAX_STRING_LENGTH*8];	/* nasty, but we shouldn't have longer messgs than this */

  if (NULL == fmt) {
    return;
  }

  va_start(argp,fmt);
  vsprintf(tmp,fmt,argp);
  va_end(argp);

  xlog( type, level, tmp, file );
}

/*
 * mudlog -- log mud messages to a file & to online imm's syslogs
 * based on syslog by Fen Jul 3, 1992
 */
void mudlog(char *str, char type, int level, byte file)
{
  char buf[MAX_STRING_LENGTH];
  struct descriptor_data *i;
  char *tmp, tp;
  time_t ct;

  ct = time(0);
  tmp = asctime(localtime(&ct));

  if (file)
    fprintf(stderr, "%-19.19s :: %s\n", tmp, str);
  if (level < 0)
    return;

  sprintf(buf, "[ %s ]\r\n", str);

  for (i = descriptor_list; i; i = i->next)
    if (!i->connected && !PLR_FLAGGED(i->character, PLR_WRITING)) {
      tp = ((PRF_FLAGGED(i->character, PRF_LOG1) ? 1 : 0) +
        (PRF_FLAGGED(i->character, PRF_LOG2) ? 2 : 0));

      if ((GET_LEVEL(i->character) >= level) && (tp >= type)) {
    send_to_char(CCGRN(i->character, C_NRM), i->character);
    send_to_char(buf, i->character);
    send_to_char(CCNRM(i->character, C_NRM), i->character);
      }
    }
}



void sprintbit(long bitvector, const char *names[], char *result)
{
  long nr;
  *result = '\0';

  for (nr = 0; (nr < 32);nr ++) 
  {
    if (IS_SET(bitvector, (1 << nr))) {
      if (*names[nr] != '\n') {
        strcat(result, names[nr]);
        strcat(result, " "); 
      } 
      else
        strcat(result, "UNDEFINED ");
    }
  }
  if (!*result)
    strcpy(result, "NOBITS ");
}



void sprinttype(int type, const char *names[], char *result)
{
  int nr = 0;

  while (type && *names[nr] != '\n') {
    type--;
    nr++;
  }

  if (*names[nr] != '\n')
    strcpy(result, names[nr]);
  else
    strcpy(result, "UNDEFINED");
}


/* Calculate the REAL time passed over the last t2-t1 centuries (secs) */
struct time_info_data real_time_passed(time_t t2, time_t t1)
{
  long secs;
  struct time_info_data now;

  secs = (long) (t2 - t1);

  now.hours = (secs / SECS_PER_REAL_HOUR) % 24;	/* 0..23 hours */
  secs -= SECS_PER_REAL_HOUR * now.hours;

  now.day = (secs / SECS_PER_REAL_DAY);	/* 0..34 days  */
  secs -= SECS_PER_REAL_DAY * now.day;

  now.month = -1;
  now.year = -1;

  return now;
}

/* Calculate the MUD time passed over the last t2-t1 centuries (secs) */
struct time_info_data mud_time_passed(time_t t2, time_t t1)
{
  long secs;
  struct time_info_data now;

  secs = (long) (t2 - t1);

  now.hours = (secs / SECS_PER_MUD_HOUR) % 24;	/* 0..23 hours */
  secs -= SECS_PER_MUD_HOUR * now.hours;

  now.day = (secs / SECS_PER_MUD_DAY) % 28;	/* 0..27 days  */
  secs -= SECS_PER_MUD_DAY * now.day;

  now.month = (secs / SECS_PER_MUD_MONTH) % 13;	/* 0..12 months */
  secs -= SECS_PER_MUD_MONTH * now.month;

  now.year = (secs / SECS_PER_MUD_YEAR);	/* 0..XX? years */

  return now;
}



struct time_info_data age(struct char_data * ch)
{
  struct time_info_data player_age;

  player_age = mud_time_passed(time(0), ch->player.time.birth);

  player_age.year += 17;	/* All players start at 17 */

  return player_age;
}


/* Check if making CH follow VICTIM will create an illegal */
/* Follow "Loop/circle"                                    */
bool circle_follow(struct char_data * ch, struct char_data * victim)
{
  struct char_data *k;

  for (k = victim; k; k = k->master) {
    if (k == ch)
      return TRUE;
  }

  return FALSE;
}



/* Called when stop following persons, or stopping charm */
/* This will NOT do if a character quits/dies!!          */
void stop_follower(struct char_data * ch)
{
  struct follow_type *j, *k;
  
  if (!ch) {
    mlog("SYSERR: Character nonexistent! (stop_follower()). Exiting.");
    exit(1);
  }
  if (!ch->master) {
    alog("SYSERR: %s has no master! (stop_follower()). Exiting.", GET_NAME(ch));
    exit(1);
  } 
  if (!ch->master->followers) {
    alog("SYSERR: %s has master, but master has no followers! (stop_follower()[utils.c]. Returning.",GET_NAME(ch));
    return;
  }

  if (IS_AFFECTED(ch, AFF_CHARM)) 
  {
    act("You realize that $N just sent you away!", FALSE, ch, 0, ch->master, TO_CHAR);
    act("$n realizes that $N did something nasty!", FALSE, ch, 0, ch->master, TO_NOTVICT);
//    act("$n doesn't like you much!!", FALSE, ch, 0, ch->master, TO_VICT);
/*    if (affected_by_spell(ch, SPELL_CHARM))
      affect_from_char(ch, SPELL_CHARM);*/
  }
  else 
  {
    act("You stop following $N.", FALSE, ch, 0, ch->master, TO_CHAR);
    if (!IS_AFFECTED(ch, AFF_TAILING))
    {
      act("$n stops following $N.", TRUE, ch, 0, ch->master, TO_NOTVICT);
      act("$n stops following you.", TRUE, ch, 0, ch->master, TO_VICT);
    }
    else 
      TOGGLE_BIT(AFF_FLAGS(ch), AFF_TAILING);
  }

  if (ch->master->followers->follower == ch) {	/* Head of follower-list? */
    k = ch->master->followers;
    ch->master->followers = k->next;
    free(k);
  } else {			/* locate follower who is not head of list */
    for (k = ch->master->followers; k->next->follower != ch; k = k->next);

    j = k->next;
    k->next = j->next;
    free(j);
  }

  ch->master = NULL;
  REMOVE_BIT(AFF_FLAGS(ch), AFF_CHARM);
  REMOVE_BIT(AFF_FLAGS(ch), AFF_GROUP);
}



/* Called when a character that follows/is followed dies OR quits */
void die_follower(struct char_data * ch, int quit)
{
  struct follow_type *j = NULL, *k;

  if (ch->master)
    stop_follower(ch);
  if (ch->followers)
    for (j = ch->followers; j; j = k) 
     {
      k = j->next;
      stop_follower(j->follower);
    }
}



/* Do NOT call this before having checked if a circle of followers */
/* will arise. CH will follow leader                               */
void add_follower(struct char_data * ch, struct char_data * leader)
{
  struct follow_type *k;
  
  if (!ch->master)
    ch->master = leader;
  else {
    send_to_char("You are already following someone.\r\n", ch);
    return;
  }
  
  CREATE(k, struct follow_type, 1);

  k->follower = ch;
  k->next = leader->followers;
  leader->followers = k;

  act("You now follow $N.", FALSE, ch, 0, leader, TO_CHAR);
  if (!IS_AFFECTED(ch, AFF_TAILING))
  {
    if (CAN_SEE(leader, ch))
      act("$n starts following you.", TRUE, ch, 0, leader, TO_VICT);
    act("$n starts to follow $N.", TRUE, ch, 0, leader, TO_NOTVICT);
  }
}

/*
 * get_line reads the next non-blank line off of the input stream.
 * The newline character is removed from the input.  Lines which begin
 * with '*' are considered to be comments.
 *
 * Returns the number of lines advanced in the file.
 */
int get_line(FILE * fl, char *buf)
{
  char temp[1024] = { 0 };
  int lines = 0;

  do 
  {
    lines++;
    fgets(temp, 1024, fl);
    if (*temp) 
      temp[strlen(temp) - 1] = '\0';
  } while (!feof(fl) && (*temp == '*' || !*temp));

  if (feof(fl))
  {
    buf[0] = 0;
    return 0;
  }
  else 
  {
    strcpy(buf, temp);
    return lines;
  }
}


int get_filename(char *orig_name, char *filename, int mode)
{
  char *prefix, *middle, *suffix, *ptr, name[64];

  switch (mode) {
  case ALIAS_FILE :
     prefix = "plralias";
     suffix = "alias"; 
     break;
  case CRASH_FILE:
    prefix = "plrobjs";
    suffix = "objs";
    break;
  case ETEXT_FILE:
    prefix = "plrtext";
    suffix = "text";
    break;
  case DREAM_FILE:
    prefix = "plrobjs";
    suffix = "dobjs";
    break;
  case PLR_FILE:
    prefix = "plrfiles";
    suffix = "plr";
    break;
  case KNOW_FILE:
    prefix = "plrobjs";
    suffix = "know";
    break;
  case SCRIPT_VARS_FILE:
    prefix = "plrvars";
    suffix = "mem";
    break;
  case OLD_PLR_FILE:
    prefix = "plrfiles";
    suffix = "old";
    break;
  case RETAINER_FILE:
    prefix = "plrobjs";
    suffix = "ret";
    break;
  default:
    return 0;
    break;
  }

  if (!*orig_name)
    return 0;

  strcpy(name, orig_name);
  for (ptr = name; *ptr; ptr++)
    *ptr = LOWER(*ptr);

  switch (LOWER(*name)) {
  case 'a':  case 'b':  case 'c':  case 'd':  case 'e':
    middle = "A-E";
    break;
  case 'f':  case 'g':  case 'h':  case 'i':  case 'j':
    middle = "F-J";
    break;
  case 'k':  case 'l':  case 'm':  case 'n':  case 'o':
    middle = "K-O";
    break;
  case 'p':  case 'q':  case 'r':  case 's':  case 't':
    middle = "P-T";
    break;
  case 'u':  case 'v':  case 'w':  case 'x':  case 'y':  case 'z':
    middle = "U-Z";
    break;
  default:
    middle = "ZZZ";
    break;
  }

  sprintf(filename, "%s/%s/%s.%s", prefix, middle, name, suffix);
  return 1;
}


int num_pc_in_room(struct room_data *room)
{
  int i = 0;
  struct char_data *ch;

  for (ch = room->people; ch != NULL; ch = ch->next_in_room)
    if (!IS_NPC(ch))
      i++;

  return i;
}

int replace_str(char **string, char *pattern, char *replacement, int rep_all,
              int max_size) {
   char *replace_buffer = NULL;
   char *flow, *jetsam, temp;
   int len, i;

   if ((strlen(*string) - strlen(pattern)) + strlen(replacement) > max_size)
     return -1;
   CREATE(replace_buffer, char, max_size);
   i = 0;
   jetsam = *string;
   flow = *string;
   *replace_buffer = '\0';
   if (rep_all) {
      while ((flow = (char *)strstr(flow, pattern)) != NULL) {
       i++;
       temp = *flow;
       *flow = '\0';
       if ((strlen(replace_buffer) + strlen(jetsam) + strlen(replacement)) > max_size) {
          i = -1;
          break;
       }
       strcat(replace_buffer, jetsam);
       strcat(replace_buffer, replacement);
       *flow = temp;
       flow += strlen(pattern);
       jetsam = flow; 
      }
      strcat(replace_buffer, jetsam);
   }
   else {
      if ((flow = (char *)strstr(*string, pattern)) != NULL) {
       i++;
       flow += strlen(pattern);
       len = ((char *)flow - (char *)*string) - strlen(pattern);
       strncpy(replace_buffer, *string, len);
       strcat(replace_buffer, replacement);
       strcat(replace_buffer, flow);
      }
   }
   if (i == 0) return 0;
   if (i > 0) {
      RECREATE(*string, char, strlen(replace_buffer) + 3);
      strcpy(*string, replace_buffer);
   }
   free(replace_buffer);
   return i;
}

/* re-formats message type formatted char * */
/* (for strings edited with d->str) (mostly olc and mail)     */
void format_text(char **ptr_string, int mode, struct descriptor_data *d, int maxlen) {
   int total_chars, cap_next = TRUE, cap_next_next = FALSE;
   char *flow, *start = NULL, temp;
   /* warning: do not edit messages with max_str's of over this value */
   char formated[MAX_STRING_LENGTH];
   d->character->desc->current_text_line = 1;

   flow   = *ptr_string;
   if (!flow) return;

   if (IS_SET(mode, FORMAT_INDENT)) {
      strcpy(formated, "   ");
      total_chars = 3;
   }
   else {
      *formated = '\0';
      total_chars = 0;
   }

   while (*flow != '\0') {
      while ((*flow == '\n') ||
           (*flow == '\r') ||
           (*flow == '\f') || 
           (*flow == '\t') ||
           (*flow == '\v') ||
           (*flow == ' ')) flow++;
       if (*flow != '\0') {

       start = flow++;
       while ((*flow != '\0') &&
              (*flow != '\n') &&
              (*flow != '\r') &&
              (*flow != '\f') &&
              (*flow != '\t') &&
              (*flow != '\v') &&
              (*flow != ' ') &&
              (*flow != '.') &&
              (*flow != '?') &&
              (*flow != '!')) flow++;

       if (cap_next_next) {
          cap_next_next = FALSE;
          cap_next = TRUE;
       }

       while ((*flow == '.') || (*flow == '!') || (*flow == '?')) {
          cap_next_next = TRUE;
          flow++;
       }

       temp = *flow;
       *flow = '\0';
       if ((total_chars + c_strlen(start) + 1) > 74) {
          strcat(formated, "\r\n");
          total_chars = 0;
      (d->character->desc->current_text_line)++;
       }

       if (!cap_next) {
          if (total_chars > 0) {
             strcat(formated, " ");
             total_chars++;
          }
       }
       else {
          cap_next = FALSE;
          *start = UPPER(*start);
       }

       total_chars += c_strlen(start);
       strcat(formated, start);
       *flow = temp;
      }

      if (cap_next_next) {
       if ((total_chars + 3) > 74) {
          strcat(formated, "\r\n");
          total_chars = 0;
      (d->character->desc->current_text_line)++;
       }
       else {
          strcat(formated, "  ");
          total_chars += 2;
       }
      }
   }
   strcat(formated, "\r\n");
   (d->character->desc->current_text_line)++;
   if (strlen(formated) > maxlen) formated[maxlen] = '\0';
   RECREATE(*ptr_string, char, MIN(maxlen, strlen(formated)+3));
   strcpy(*ptr_string, formated);
}
int can_channel_same_sex(struct char_data *ch,struct char_data *list)
{
  if ((NULL == list)||(NULL == ch)) {
    mlog("SYSERR: NULL-argument in call to can_channel_same_sex().Defaulting to false");
    return 0;
  }

  return ((GET_SPARK(list) || GET_TEACHABLE(list) || (GET_CLASS(list) == CLASS_WILDER) 
          || (GET_CLASS(list) == CLASS_CHANNELER)) && (GET_SEX(ch) == GET_SEX(list)));
}

int can_channel_not_same_sex(struct char_data *ch,struct char_data *list)
{
  if ((NULL == list)||(NULL == ch)) {
    mlog("SYSERR: NULL-argument in call to can_channel_not_same_sex().Defaulting to FALSE");
    return 0;
  }

  return ((GET_SPARK(list) || GET_TEACHABLE(list) || (GET_CLASS(list) == CLASS_WILDER) 
          || (GET_CLASS(list) == CLASS_CHANNELER)) && (GET_SEX(ch) != GET_SEX(list)));
}

int cant_channel(struct char_data *ch)
{

  if (NULL == ch) {
    mlog("SYSERR: NULL-argument in call to cant_channel().Defaulting to FALSE");
    return 0;
  }

   return (!(GET_SPARK(ch) || GET_TEACHABLE(ch) || (GET_CLASS(ch) == CLASS_CHANNELER)
                         || (GET_CLASS(ch) == CLASS_WILDER)));
}

int generic_result(struct char_data *ch,int skill_num,char *string,byte use_gen)
{
  return (generic_result_mod(ch,skill_num,string,use_gen,0));
}

static int part_match(char *word,char *word_list)
{
  char *wU,*wlU;
  int isIn = 0;

  skip_spaces(&word);
  wU  = strupper(word);
  wlU = strupper(word_list);

  isIn = (NULL == strstr(wlU,wU) ? 0 : 1);

  free(wU); free(wlU);
  
  return isIn;
}     

sh_int find_room_by_name(char *arg)
{
  int i,skip = 0,num = 0,j;
  char **seek;
  int submatch = 1;

  skip = (get_number(&arg) - 1);
  seek = split(arg,' ',&num);	 /* Get an array of keyword strings */

  for (i = 0; i < top_of_world; i++) {
   if (world[i].name == NULL) {
      return 0;
    }
    else {
      submatch = 1;
      for (j = 0; j < num; j++) {
        submatch &= part_match(seek[j],world[i].name);
      } 
      if (submatch && !skip) {

        for (j = 0; j < num; j++) 
      free(seek[j]);

    free(seek);

    return i;
      }
      else if (submatch && skip)
        skip--;
    }
  }

 
  for (j = 0; j < num; j++)  
    free(seek[j]);

  free(seek);

  return 0;
}
     
int generic_result_mod(struct char_data *ch,int skill_num,char *string,byte use_gen,int mod)
{
  char t_buf[300],tmp2[300];
  int skill_lvl,noinc = 0;

  int result = number(0,100);

  if (IS_NPC(ch))
    return SOME_SUCCESS;

  if (!GET_SKILL(ch,skill_num))
    return TOTAL_FAILURE;

  noinc = GET_INC_TIME(ch,skill_num);
  
  skill_lvl = GET_SKILL(ch,skill_num) + check_taveren(ch) + mod;
  
  if (string == NULL) {
    string = t_buf;
    sprintf(string,"You have become better at %s.\r\n",spells[skill_num]);
  }
  sprintf(tmp2,"You learn from your mistakes and you become better at %s.\r\n",spells[skill_num]);
  
  if (result == 100)
  {
    if ((GET_SKILL(ch,skill_num) < 100) && !noinc) 
    {
      ADD_SKILL(ch,skill_num,1);
      send_to_char(tmp2,ch);
    }
    if (!noinc) 
    {
      GET_INC_TIME(ch,skill_num) = 1;
      SET_NOINC(ch,skill_num);
    }
    return TOTAL_FUCKUP;
  }
  else if (result > skill_lvl) 
    return TOTAL_FAILURE;
  else if ((result <= skill_lvl) && (result > 2))
    return SOME_SUCCESS;
  else 
  {
    result = number(0,100);
    
    if (result <= skill_lvl) 
    {
      if ((GET_SKILL(ch,skill_num) < 100) && !noinc) 
      {
        send_to_char(string,ch);
        ADD_SKILL(ch,skill_num,1);
      }
      if (!noinc) 
      {
        GET_INC_TIME(ch,skill_num) = 1;
        SET_NOINC(ch,skill_num);
      }
      return TOTAL_SUCCESS;
    }
    else 
      return SOME_SUCCESS;
  }
}

byte grouped_with(struct char_data *ch, struct char_data *target)
{
  struct char_data *leader;
  struct follow_type *follow;
  
  if (!IS_AFFECTED(ch, AFF_GROUP))
    return FALSE;
  
  if (!ch->master)
    leader = ch;
  else
    leader = ch->master;
    
  for (follow = leader->followers; follow; follow = follow->next)
    if (follow->follower == target)
      return TRUE;
  return FALSE;
}

int number_of_players_in_room( int room_number )
{
  int count = 0;
  struct char_data *i;

  for( i = world[ room_number ].people; i; i = i->next_in_room )
  {
    if( !IS_NPC( i ) )
    {
      count++;
    }
  }

  return count;
}

/*
 * This function is a facilitator to find the index of a certain string
 * in an array of strings. 
 *
 * How to use this function:
 *    You must give a non-NULL key and array, or it will return -1.
 *    Then you can either give the last string that signifies the end of the
 *    array, for example "NO MORE RANKS" in the case of rsrank and other arrays,
 *    -or- if the size is known, the size of the array.
 *
 *    If endtag is NULL, the size must be more than 0, and if size is 0,
 *    endtag must be non-NULL. If you give both, the size arg takes precedence.
 *    If you give neither (i.e, NULL and 0) it returns -1.
 *    The search is case insensitive when comparing array entries and the key, but
 *    the key can't be any abbreviation of the string sought after.
 *
 *****WARNING: The end tag arg, if given, _MUST EXACTLY_ match the actual end tag in the 
 *             array you want to search in, or this function will crash the game.
 *             Also, if you give a size larger than the actual size, it crashes too.
 *
 * Valid example:
 *   int guild = index_from_array("daRkFriEnDs", guilds, NULL, MAXGUILDS);
 *   Gives 0, which is the index of the "Darkfriends" string in the guilds array.
 * Crashing example:
 *   int guild = index_from_array("darkfryends", guilds, NULL, 50);
 *   Crashes the game, since the array is smaller than that, and the
 *   string isn't found (should be "darkfriends").
 * Valid, but faulty example:
 *   int society = index_from_array("Tain Shari", aielsocieties, "NO MORE SOCIETIES", 0);
 *   Gives -1, since the whole string is "Tain Shari (True bloods)".
 * Crashing example:
 *   int society = index_from_array("Tain Shari", aielsocieties, "NO MORE SOCIETY", 0);
 *   Crashes us, since the end tag is "NO MORE SOCIETIES", nothing else.
 *
 * Parameters: String to look for in the array,
 *             array of strings to search in,
 *             delimiting string that marks end of array (end tag),
 *             number of strings in array (size)
 *
 * Returns: Index of sought after string in the array, or -1.
 */
int index_from_array(char *key, const char **array, char *endtag, int numInArray)
{
  int num = numInArray;
  int i = 0;
  
  if ((num <= 0) && array && endtag && *endtag)
  {
    for (num = 0; *array[num] && strcmp(endtag, array[num]); num++);
  }
  
  if (!key || !array || num <= 0)
  {
    return -1;
  }
  
  for (i = 0; i < num; i++)
  { 
    if (!strcasecmp(key, array[i]))
    {
      return i;
    }
  }
  return -1;
}

long count_newlines(char **ptr_str)
{
  char *str;
  int i = 0;

  str = *ptr_str;
  if (str == NULL)
    return i;

  while (*str != '\0') {
    if (*str == '\r' || (*str == '\r' && *(str+1) == '\n'))
      i++;
    ++str;
  }

  return i;
}

void SET_MANA(struct char_data *ch, long val)
{
  if (!IS_NPC(ch) && ch->player_specials->linking[0]) {
    ch->player_specials->manalinked = val;
  } else {
    ch->points.mana = val;
  }
}

long GET_MANA(struct char_data *ch)
{
  if (!IS_NPC(ch) && ch->player_specials->linking[0]) {
    return  ch->player_specials->manalinked;
  }
  return ch->points.mana;
}

void ADD_MANA(struct char_data *ch, long val)
{
  long total = GET_MANA(ch) + val;
  SET_MANA(ch, total); 
}

byte GET_SKILL(struct char_data *ch, int skill)
{
  if (IS_NPC(ch)) {
    return 0;
  }
  return ch->player_specials->saved.skills[skill];
}

void SET_SKILL(struct char_data *ch, int skill, byte value)
{
  if (!IS_NPC(ch)) {
    ch->player_specials->saved.skills[skill] = value;
  }
}

void ADD_SKILL(struct char_data *ch, int skill, byte toAdd)
{
  if (!IS_NPC(ch)) {
    byte finalVal = GET_SKILL(ch, skill) + toAdd;
    SET_SKILL(ch, skill, finalVal);
  }
}


long GET_TIMES(struct char_data *ch, int skill)
{
  if (IS_NPC(ch)) {
    return ch->mob_specials.no_skill[skill];
  }
  return ch->player_specials->no_skill[skill];
}

void SET_TIMES(struct char_data *ch, int skill, long value)
{
  if (IS_NPC(ch)) {
    ch->mob_specials.no_skill[skill] = value;
   } else {
     ch->player_specials->no_skill[skill] = value;
   }
}

void ADD_TIMES(struct char_data *ch, int skill, long value)
{
  long new_val = GET_TIMES(ch, skill) + value;
  SET_TIMES(ch, skill, new_val);
}

long GET_MAX_MANA(struct char_data *ch)
{
  if (!IS_NPC(ch)) {
    if (ch->player_specials->linking[0]) {
      return ch->player_specials->max_manalinked;
    }
    return ch->points.max_mana;
  }
  return ch->points.mana;
}

void SET_MAX_MANA(struct char_data *ch, long value)
{
  if (!IS_NPC(ch)) {
    if (ch->player_specials->linking[0]) {
      ch->player_specials->max_manalinked = value;
    } else {
      ch->points.max_mana = value;
    }
  } else {
    ch->points.mana = value;
  }
}

void ADD_MAX_MANA(struct char_data *ch, long toAdd)
{
  long current = GET_MAX_MANA(ch);
  SET_MAX_MANA(ch, (current + toAdd));
}

char *GET_RNAME(struct char_data *ch)
{
  if (IS_NPC(ch)) {
    return ch->player.short_descr;
  }
  return ch->player.name;
}

void SET_RNAME(struct char_data *ch, const char *newName)
{
  if (IS_NPC(ch)) {
    ch->player.short_descr = newName;
  } else {
    ch->player.name = newName;
  }    
}

char *GET_NOBILITY(struct char_data *ch)
{
  if (GET_DISGUISED(ch)) {
    return GET_DNOBILITY(ch);
  }
  return GET_RNOBILITY(ch);
}

void SET_NOBILITY(struct char_data *ch, const char *nob)
{
  if (GET_DISGUISED(ch)) {
    GET_DNOBILITY(ch) = nob;
  } else {
    GET_RNOBILITY(ch) = nob;
  }
}

char *GET_TITLE(struct char_data *ch)
{
  if (GET_DISGUISED(ch)) {
    return GET_DTITLE(ch);
  }
  return GET_RTITLE(ch);
}

void SET_TITLE(struct char_data *ch, const char *title)
{
  if (GET_DISGUISED(ch)) {
    GET_DTITLE(ch) = title;
  } else {
    GET_RTITLE(ch)= title;
  }
}
