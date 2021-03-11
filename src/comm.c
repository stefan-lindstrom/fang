/******************************************************************************
 * $Id: comm.c,v 1.36 2004/05/17 21:37:32 fang Exp $
 *   
 * File: comm.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: comm.c,v $
 *   Revision 1.36  2004/05/17 21:37:32  fang
 *   Tiek: no more message about losing descriptor without char.
 *
 *   Revision 1.35  2003/12/23 05:28:48  fang
 *   Tiek: Moved ban/unban, force-rent, and descriptor without char from syslog to xlogs
 *
 *   Revision 1.34  2003/11/23 08:02:24  fang
 *   Tze: Changed who page to write every 30 seconds as per admin request
 *
 *   Revision 1.33  2003/06/17 13:02:37  fang
 *
 *   Added a single file for messages. Code for now contains only functionalty to
 *   save messages (fight/spell op/spell tp) as XML. All structs, and data, and
 *   defines pertaining to messages have been moved to the new header.
 *
 *   Revision 1.32  2003/06/12 07:22:13  fang
 *   Changed testing for channeling ability as per HGL request. Fixed Room Seal to work as originally intended. Commented out ignorelist. Will fully remove once certain no problems will arise
 *
 *   Revision 1.31  2003/06/01 09:34:40  fang
 *   Added $u and $U to the act() function, from circle bpl18. $u is uppercase previous word, $U is uppercase next word.
 *
 *   Revision 1.30  2003/05/29 09:59:30  fang
 *   Dropping link while tooling/crafting no longer causes crashes
 *
 *   Revision 1.29  2003/05/08 08:26:45  fang
 *   DO NOT EDIT CODE IN THW ~/fang1/src DIR FOR FRELLS SAKE
 *
 *   Revision 1.28  2003/02/04 08:21:50  fang
 *   RPLog no longer logs stuff from OOC-characters.
 *
 *   Revision 1.27  2003/02/03 09:04:44  fang
 *   NPC-check in add_to_rplog
 *
 *   Revision 1.26  2002/12/14 04:14:24  fang
 *   fisch: added "at" to "list board" for imms
 *
 *   Revision 1.25  2002/12/11 19:44:20  fang
 *   fisch: added "mail end" notification and removed guild info from score
 *
 *   Revision 1.24  2002/12/10 18:30:43  fang
 *   fisch: changes need to make it work on the new host (dg_mobcmd.c)
 *   and mask the "tick" crash problem (comm.c)
 *
 *   Revision 1.23  2002/11/10 22:18:34  fang
 *   Fisch: Changeed Tells message and onl display it on AFK not AFW
 *
 *   Revision 1.22  2002/11/07 04:02:01  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.21  2002/10/08 17:12:23  fang
 *   fischer: format changes and SYSERR message for mobprogs
 *
 *   Revision 1.20  2002/10/03 05:55:09  fischer
 *   fischer: moved ideas to be used as an old ideas and added newideas file
 *
 *   Revision 1.19  2002/09/23 05:52:41  fischer
 *   fisch: clearing out lua triggers part 1
 *
 *   Revision 1.18  2002/09/22 21:27:46  fischer
 *   Added a host to losing descriptor
 *
 *   Revision 1.17  2002/09/15 00:50:08  fang
 *   Drath: XLOG update - Categories added to xlog display automatically
 *
 *   Revision 1.16  2002/09/13 18:00:42  fang
 *   Drath: Small addition to MyColor and MyColor option added to Prompts.
 *
 *   Revision 1.15  2002/09/13 16:37:23  fang
 *   Drath: More new prompt additions and mcl edits
 *
 *   Revision 1.14  2002/09/13 15:34:06  fang
 *   Drath: More PROMPT additions
 *
 *   Revision 1.13  2002/09/12 22:21:33  fang
 *   fischer: removed the prove and journal commands
 *
 *   Revision 1.12  2002/09/10 19:00:44  fang
 *   Drath: Added %l (Level) to prompt
 *
 *   Revision 1.11  2002/09/10 14:45:32  fang
 *   Drath: Added Name variable to Prompt
 *
 *   Revision 1.10  2002/09/08 06:41:16  fang
 *   Drath: Fischer added AEDIT.  Welcome to the staff, Fischer.
 *
 *   Revision 1.9  2002/09/05 20:56:30  fang
 *   Drath: added CRETODO and SHOWCRETODO for MassyPants
 *
 *   Revision 1.8  2002/08/30 21:23:09  fang
 *   Put in retainers.
 *
 *   Revision 1.7  2002/08/20 21:03:50  fang
 *   Drath: Hunter GSKILLS
 *
 *   Revision 1.6  2002/08/20 18:11:27  fang
 *   Changed priorities of threads (?) in glib timeout functions.
 *
 *   Revision 1.5  2002/08/16 15:50:56  fang
 *   Drath: send_to_char check for null ch and messg
 *
 *   Revision 1.4  2002/08/07 00:38:16  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/08/06 21:24:27  fang
 *   Drath: Reinabled all ignore settings
 *
 *   Revision 1.2  2002/07/28 13:56:03  fang
 *   Removed dmlloc...just to test.
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.36  2002/07/06 07:05:12  fang
 *   Drath: IMMLOCK for Fred and 2nd to last XLOG commit
 *
 *   Revision 1.35  2002/06/21 14:36:20  fang
 *   Drath: Fixed a small bug with RPTAG where EMOTE crashes when the char is in a mirror room.
 *
 *   Revision 1.34  2002/06/19 19:08:18  fang
 *   Drath: (mail waiting) in prompt.
 *
 *   Revision 1.33  2002/06/18 09:08:07  fang
 *   *** empty log message ***
 *
 *   Revision 1.32  2002/06/17 08:14:15  fang
 *   *** empty log message ***
 *
 *   Revision 1.31  2002/06/13 10:12:28  fang
 *   More free'ing stuff...
 *
 *   Revision 1.30  2002/06/12 09:55:16  fang
 *   More mem thingies...
 *
 *   Revision 1.29  2002/06/11 09:04:38  fang
 *   *** empty log message ***
 *
 *   Revision 1.28  2002/06/11 08:43:48  fang
 *   *** empty log message ***
 *
 *   Revision 1.27  2002/05/27 10:52:48  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.26  2002/05/14 08:58:32  fang
 *   *** empty log message ***
 *
 *   Revision 1.25  2002/05/13 13:59:30  fang
 *   Maz: Turned off act() ignore cheks for now.
 *
 *   Revision 1.24  2002/05/13 13:36:33  fang
 *   *** empty log message ***
 *
 *   Revision 1.23  2002/05/13 13:17:56  fang
 *   *** empty log message ***
 *
 *   Revision 1.22  2002/05/13 03:00:33  fang
 *   Maz: I was blonde and forgot a couple IS_NPC checks which made the system unhappy.  These have (hopefully) been fixed now.
 *
 *   Revision 1.21  2002/05/12 23:50:46  fang
 *   Added in IGNORE system, and removed AutoHotboots until the cap can be fixed for good. -Maz
 *
 *   Revision 1.20  2002/05/08 10:52:38  fang
 *   Another attempt to fix the decreasing desc cap...
 *
 *   Revision 1.19  2002/04/13 18:05:59  fang
 *   *** empty log message ***
 *
 *   Revision 1.18  2002/04/04 04:52:50  fang
 *   Re-wrote process_input... fixed manaconv.
 *
 *   Revision 1.17  2002/04/02 01:19:35  fang
 *   *** empty log message ***
 *
 *   Revision 1.16  2002/03/11 10:50:59  fang
 *   xname fix
 *
 *   Revision 1.15  2002/03/03 13:11:12  fang
 *   *** empty log message ***
 *
 *   Revision 1.14  2002/02/26 15:15:27  fang
 *   Fixed poll code. No more caps\!
 *
 *   Revision 1.13  2002/02/26 10:13:12  fang

 *   *** empty log message ***
 *
 *   Revision 1.12  2002/02/26 09:12:26  fang
 *   *** empty log message ***
 *
 *   Revision 1.11  2002/02/26 09:08:08  fang
 *   *** empty log message ***
 *
 *   Revision 1.10  2002/02/25 15:43:42  fang
 *   *** empty log message ***
 *
 *   Revision 1.9  2002/02/24 07:44:47  fang
 *   *** empty log message ***
 *
 *   Revision 1.8  2002/02/24 07:31:58  fang
 *   *** empty log message ***
 *
 *   Revision 1.7  2002/02/24 06:46:57  fang
 *   *** empty log message ***
 *
 *   Revision 1.6  2002/02/24 03:48:05  fang
 *   *** empty log message ***
 *
 *   Revision 1.5  2002/02/23 14:49:34  fang
 *   *** empty log message ***
 *
 *   Revision 1.4  2002/02/23 12:34:58  fang
 *   Fixed where -g
 *
 *   Revision 1.3  2002/02/22 17:48:26  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.81  2002/02/21 12:19:18  mud
 *   *** empty log message ***
 *
 *   Revision 1.80  2002/02/21 12:09:02  mud
 *   Bleading page-string fixed.
 *
 *   Revision 1.79  2002/02/20 03:12:28  mud
 *   *** empty log message ***
 *
 *   Revision 1.78  2002/02/19 15:56:58  mud
 *   *** empty log message ***
 *
 *   Revision 1.77  2002/02/19 14:01:06  mud
 *   act\(\) fixes and stuff
 *
 *   Revision 1.76  2002/02/19 08:05:33  mud
 *   Fixed a bug if make_prompt
 *
 *   Revision 1.75  2002/02/17 21:23:38  mud
 *   *** empty log message ***
 *
 *   Revision 1.74  2002/02/16 04:22:15  mud
 *   Fix in act to prevent crashes when sending to an empty room.
 *
 *   Revision 1.73  2002/02/15 22:10:29  mud
 *   put check in for to value in act
 *
 *   Revision 1.72  2002/01/31 09:58:23  mud
 *   Custom prompts now handles colour codes properly.
 *
 *   Revision 1.71  2002/01/31 09:14:38  mud
 *   *** empty log message ***
 *
 *   Revision 1.70  2002/01/29 15:42:14  mud
 *   Minor bugfixes and whishes from ideas list...
 *
 *   Revision 1.69  2002/01/28 15:29:05  mud
 *   Customizable prompts..
 *
 *   Revision 1.68  2002/01/24 21:33:14  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.67.2.2  2002/01/24 03:20:29  mud
 *   Removed some further unescessary files (player.[ch], roomprog.[ch]
 *   old2new.c). Cleaned up some other stuff (guild stuff mostly).
 *
 *   Revision 1.67.2.1  2002/01/22 20:50:57  mud
 *
 *
 *   Removed boats and info system. Obsolete, unused code with no value.
 *   Fixed bug in portal_stone SPECIAL. Now checks that a room exists
 *   before attempting to transf chars there. :)
 *
 *****************************************************************************/

/* ************************************************************************
*   File: comm.c                                        Part of CircleMUD *
*  Usage: Communication, socket handling, main(), central game loop       *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#define __COMM_C__
#include <string.h>
#include "conf.h"
#include "sysdep.h"
#include <stdarg.h>
#include "config.h"
#include "modify.h"
#include "skimming.h"
#include "xmlcharsave.h"
#include "hotboot.h"
#include <sys/ioctl.h>

#ifdef CIRCLE_WINDOWS		/* Includes for Win32 */
#include <direct.h>
#include <mmsystem.h>
#else				/* Includes for UNIX */
#include <sys/socket.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <bits/sigaction.h> // ugly hack for linux when compiling with std=c99
#include <malloc.h>
#include <errno.h>
#endif

#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "olc.h"
#include "screen.h"
#include "prog.h"
#include "objprog.h"
#include "mobprog.h"
#include "magic.h"
#include "limits.h"
#include "fight.h"
#include "mobact.h"
#include "ban.h"
#include "color.h"
#include "weather.h"
#include "xmlobjsave.h"
#include "guild.h"
#include "dg_scripts.h"
#include "dg_event.h"
#include "shop.h"
#include "act.h"
#include "namedspec.h"
#include "skills.h"
#include "guild_parser.h"
#include "mail.h"
#include "retainer.h"
#include "messages.h"

#include <sys/poll.h>

#ifdef HAVE_ARPA_TELNET_H
#include <arpa/telnet.h>
#else
#include "telnet.h"
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

/* local globals */
GMainLoop *main_loop = NULL;
struct descriptor_data *descriptor_list = NULL;		/* master desc list */
struct txt_block *bufpool = 0;	/* pool of large output buffers */
int buf_largecount = 0;		/* # of large buffers which exist */
int buf_overflows = 0;		/* # of overflows of output */
int buf_switches = 0;		/* # of switches from small to large buf */
int circle_shutdown = 0;	/* clean shutdown */
int circle_reboot = 0;		/* reboot the game after a shutdown */
int no_specials = 0;		/* Suppress ass. of special routines */
int max_players = 0;		/* max descriptors available */
int tics = 0;			/* for extern checkpointing */
int scheck = 0;			/* for syntax checking mode */
bool MOBTrigger = TRUE;		/* For MOBProgs */
struct timeval null_time;	/* zero-valued time structure */
GIOChannel *g_mother_desc = NULL;
int gmde_num;
int mother_desc;		/* Mother descriptor */
int dg_act_check;		/* toggle for act_trigger */
unsigned long dg_global_pulse = 0; /* number of pulses since game start */
unsigned long pulse = 0;

/* functions in this file */
struct timeval timediff(struct timeval a, struct timeval b);
struct timeval timeadd(struct timeval a, struct timeval b);
void flush_queues(struct descriptor_data *d);
void nonblock(socket_t s);
int perform_subst(struct descriptor_data *t, const char *orig, char *subst);
int perform_alias(struct descriptor_data *d, char *orig);
void record_usage(void);
void update_timers(void);
void make_prompt(struct descriptor_data *point);
void check_idle_passwords(void);
gboolean heartbeat(gpointer data);

/* *********************************************************************
*  main game loop and related stuff                                    *
********************************************************************* */

/* Windows doesn't have gettimeofday, so we'll simulate it. */
#ifdef CIRCLE_WINDOWS

void gettimeofday(struct timeval *t, struct timezone *dummy)
{
  DWORD millisec = GetTickCount();

  t->tv_sec = (int) (millisec / 1000);
  t->tv_usec = (millisec % 1000) * 1000;
}

#endif

static void st_free_room(struct room_data *r)
{
  struct extra_descr_data    *e, *enext;
  struct room_direction_data *d;
  struct affected_type       *aff, *anext;
  struct trig_proto_list     *proto, *pnext;
  int i;

  FREEPTR(r->name);
  FREEPTR(r->description);
 
  for (i = 0; i < NUM_OF_DIRS; i++) {
    if (NULL != (d = r->dir_option[i])) {
      FREEPTR(d->general_description);
      FREEPTR(d->keyword);
      free(d);      
    }
  }
 
  for (e = r->ex_description; NULL != e; e = enext) {
    enext = e->next;
    FREEPTR(e->keyword);
    FREEPTR(e->description);
    free(e);
  }
  r->ex_description = NULL;

  for (aff = r->aff; NULL != aff; aff = anext) {
    anext = aff->next;
    free(aff);
  }
  r->aff = NULL;

  for (proto = r->proto_script; NULL != proto; proto = pnext) {
    pnext = proto->next;
    free(proto);
  }
  r->proto_script = NULL;
  if (r->script) {
    free_script(r->script);
    r->script = NULL;
  }
}

static void st_free_ptable_entry(struct player_index_element *p) 
{
  FREEPTR(p->name);
  FREEPTR(p->dname);
}

static void st_free_index_element(struct index_data *i)
{
  FREEPTR(i->farg);

  if (i->proto) {
    struct cmdlist_element *c, *cn;
    for (c = i->proto->cmdlist; c; c = cn) {
      cn = c->next;
      free(c->cmd);
      free(c);
    }
    if (i->proto->name)
      free(i->proto->name);
    if (i->proto->arglist)
      free(i->proto->arglist);
    if (i->proto->var_list)
      free_varlist(i->proto->var_list);

    if (i->proto->wait_event)
      remove_event(i->proto->wait_event);

    free(i->proto);
  }
  i->proto = NULL;
}

static void st_free_char(struct char_data *ch)
{
  int i;
  struct trig_proto_list     *proto, *pnext;
  struct script_memory       *mem,   *mnext;

  for (i = 0; i < MAX_DESCRIPTIONS; i++) {
    if (GET_DESCNUM(ch,i))
      free(GET_DESCNUM(ch,i));
  }
  if (ch->player.title)
    free(ch->player.title);
  if (GET_DTITLE(ch))
    free(GET_DTITLE(ch));
  if (ch->player.short_descr)
    free(ch->player.short_descr);
  if (ch->player.long_descr)
    free(ch->player.long_descr);
  if (ch->player.description)
    free(ch->player.description);
  while (ch->affected)
    affect_remove(ch, ch->affected);
  if (ch->player.contactinfo)
    free(ch->player.contactinfo);
  if (ch->player.background)
    free(ch->player.background);
  if (ch->player.pretitle)
    free(ch->player.pretitle);
  if (ch->player.nobility)
    free(ch->player.nobility);

  for (proto = ch->proto_script; NULL != proto; proto = pnext) {
    pnext = proto->next;
    free(proto);
  }
  ch->proto_script = NULL;

  if (ch->mprog)
    free_mobprog(ch);

  if (ch->script)
    free_script(ch->script);

  if (ch->memory) {
    for (mem = ch->memory; NULL != mem; mem = mnext) {
      mnext = mem->next;

      FREEPTR(mem->cmd);
      free(mem);         
    }                    
    ch->memory = NULL;     
  }                        
                         
  if (MEMORY(ch))
    clearMemory(ch);
}

static void st_free_obj(struct obj_data *obj)
{
  struct extra_descr_data *this, *next_one;
  struct trig_proto_list     *proto, *pnext;

  if (obj->name)
    free(obj->name);
  if (obj->description)
    free(obj->description);
  if (obj->short_description)
    free(obj->short_description);
  if (obj->action_description)
    free(obj->action_description);
  if (obj->ex_description) {
    for (this = obj->ex_description; this; this = next_one) {
      next_one = this->next;
      if (this->keyword)
    free(this->keyword);
      if (this->description)
    free(this->description);
      free(this);
    }
  }
  
  for (proto = obj->proto_script; NULL != proto; proto = pnext) {
    pnext = proto->next;
    free(proto);
  }
  obj->proto_script = NULL;

  if (obj->oprog)    /* objprog */
    free_objprog(obj);

  if (obj->script) /* DGS */
    free_script(obj->script);
}

static void free_editors(ELEMENT a)
{
  free(a);
}

static void st_free_zone(struct zone_data *z)
{
  struct reset_com *c;

  FREEPTR(z->name);

  if (z->hfl_Editors)
    deleteAll(z->hfl_Editors,free_editors);

  if (NULL != (c = z->cmd)) {
    int i; 

    for (i = 0; 'S' != c[i].command; i++) {
      FREEPTR((c+i)->sarg1);
      FREEPTR((c+i)->sarg2);
    }
    FREEPTR(z->cmd);
  }
}

static void st_free_shop(struct shop_data *s)
{
  int i;

  if (NULL != s->type) {
    for (i = 0; -1 != s->type[i].type; i++) {
      FREEPTR(s->type[i].keywords);    
    }
    FREEPTR(s->type);
  }
  FREEPTR(s->no_such_item1);
  FREEPTR(s->no_such_item2);
  FREEPTR(s->missing_cash1);
  FREEPTR(s->missing_cash2);
  FREEPTR(s->do_not_buy);
  FREEPTR(s->message_buy);
  FREEPTR(s->message_sell);     
  FREEPTR(s->in_room);
  FREEPTR(s->producing);
}

static void free_all_mem(void)
{
  int i;
  struct char_data *ch,  *cnext;
  struct obj_data  *obj, *onext;

  struct tp_message_type *mess, *mnext;
  struct sp_message_type *smess, *smnext;
  struct message_type    *skmess, *sknext;

  struct reset_q_element *iter,*inext;

  mlog("");
  mlog("Freeing all dynamic memory!");
  mlog("Freeing world data...");

  for (i = 0; i <= top_of_world; i++) {
    st_free_room( (world + i) );
  }
  free(world);
  world = NULL;

  mlog("World data freed, moving on to player table");
  for (i = 0; i <= top_of_p_table; i++) {
    st_free_ptable_entry(player_table + i);
  }
  free(player_table);
  player_table = NULL;

  mlog("Player table free'd, moving on to character list");
  for (ch = character_list; NULL != ch; ch = cnext) {
    cnext = ch->next;
    free_char(ch);
  }
  character_list = NULL;
  
  mlog("Character list free'd, moving on to mob index/mob proto");
  for (i = 0; i <= top_of_mobt; i++) {
    st_free_char(mob_proto+i);
    st_free_index_element(mob_index+i);
  }
  free(mob_proto);
  mob_proto = NULL;
  free(mob_index);
  mob_index = NULL;

  mlog("Mob index/proto free'd, moving on to object list");
  for (obj = object_list; NULL != obj; obj = onext) {
    onext = obj->next;
    free_obj(obj);
  }
  object_list = NULL;

  mlog("Object list free'd moving on to object proto/index");
  for (i = 0; i <= top_of_objt; i++) {
    st_free_index_element(obj_index+i);
    st_free_obj(obj_proto+i);
  }
  free(obj_proto);
  free(obj_index);
  obj_proto = NULL;
  obj_index = NULL;

  mlog("Mob index/proto free'd, moving on to zone table");
  for (i = 0; i <=top_of_zone_table; i++) {
    st_free_zone(zone_table+i);
  }
  free(zone_table);
  zone_table = NULL;
  
  mlog("Zone table free'd, moving on to shops");
  for (i = 0; i < top_shop; i++) {
    st_free_shop((shop_index + i));
  }
  free(shop_index);
  shop_index = NULL;

  mlog("Shops free'd, moving on to messages (sp/tp/sk)");
  for (i = 0; i < MAX_SPELL_MESS; i++) {
    if (tp_messages[i].number_of_attacks) {
      for (mess = tp_messages[i].msg; NULL != mess; mess = mnext) {
    mnext = mess->next;
    FREEPTR(mess->die_msg.weaver);
    FREEPTR(mess->die_msg.vict_tpu);
    FREEPTR(mess->die_msg.vict_ntp);
    FREEPTR(mess->die_msg.onl_tpu);
    FREEPTR(mess->die_msg.onl_npu);
    FREEPTR(mess->miss_msg.weaver);
    FREEPTR(mess->miss_msg.vict_tpu);
    FREEPTR(mess->miss_msg.vict_ntp);
    FREEPTR(mess->miss_msg.onl_tpu);
    FREEPTR(mess->miss_msg.onl_npu);
    FREEPTR(mess->hit_msg.weaver);
    FREEPTR(mess->hit_msg.vict_tpu);
    FREEPTR(mess->hit_msg.vict_ntp);
    FREEPTR(mess->hit_msg.onl_tpu);
    FREEPTR(mess->hit_msg.onl_npu);
    free(mess);
      }
      tp_messages[i].msg = NULL;
    }
    if (spell_messages[i].number_of_attacks) {
      for (smess = spell_messages[i].msg; NULL != smess; smess = smnext) {
    smnext = smess->next;
    FREEPTR(smess->die_msg.weaver);
    FREEPTR(smess->die_msg.vict_nc);
    FREEPTR(smess->die_msg.vict_cos);
    FREEPTR(smess->die_msg.vict_css);
    FREEPTR(smess->die_msg.onl_nc);
    FREEPTR(smess->die_msg.onl_cos);
    FREEPTR(smess->die_msg.onl_css);
    FREEPTR(smess->miss_msg.weaver);
    FREEPTR(smess->miss_msg.vict_nc);
    FREEPTR(smess->miss_msg.vict_cos);
    FREEPTR(smess->miss_msg.vict_css);
    FREEPTR(smess->miss_msg.onl_nc);
    FREEPTR(smess->miss_msg.onl_cos);
    FREEPTR(smess->miss_msg.onl_css);
    FREEPTR(smess->hit_msg.weaver);
    FREEPTR(smess->hit_msg.vict_nc);
    FREEPTR(smess->hit_msg.vict_cos);
    FREEPTR(smess->hit_msg.vict_css);
    FREEPTR(smess->hit_msg.onl_nc);
    FREEPTR(smess->hit_msg.onl_cos);
    FREEPTR(smess->hit_msg.onl_css);

    free(smess);
      }
      spell_messages[i].msg = NULL;
    }
  }
  for (i = 0; i < MAX_MESSAGES; i++) {
    if (fight_messages[i].number_of_attacks) {
      for (skmess = fight_messages[i].msg; NULL != skmess; skmess = sknext) {
    sknext = skmess->next;
    FREEPTR(skmess->die_msg.attacker_msg);
    FREEPTR(skmess->die_msg.victim_msg);
    FREEPTR(skmess->die_msg.room_msg);
    FREEPTR(skmess->miss_msg.attacker_msg);
    FREEPTR(skmess->miss_msg.victim_msg);
    FREEPTR(skmess->miss_msg.room_msg);
    FREEPTR(skmess->hit_msg.attacker_msg);
    FREEPTR(skmess->hit_msg.victim_msg);
    FREEPTR(skmess->hit_msg.room_msg);
    FREEPTR(skmess->god_msg.attacker_msg);
    FREEPTR(skmess->god_msg.victim_msg);
    FREEPTR(skmess->god_msg.room_msg);
    free(skmess);
      }
    }
    fight_messages[i].msg = NULL;
  }

  mlog("Messages (sp/tp/sk) freed, moving on to file strings");
  FREEPTR(GREETINGS);
  FREEPTR(wizlist);
  FREEPTR(immlist);
  FREEPTR(news);
  FREEPTR(oldnews);
  FREEPTR(credits);
  FREEPTR(motd);
  FREEPTR(imotd);
  FREEPTR(help);
  FREEPTR(poli);
  FREEPTR(info);
  FREEPTR(handbook);
  FREEPTR(background);
  FREEPTR(gossip);
  FREEPTR(bugs);
  FREEPTR(ideas);
  FREEPTR(newideas);
  FREEPTR(socialideas);
  FREEPTR(todo);
  FREEPTR(typos);
  FREEPTR(cretodo);

  mlog("File strings free'd moving on to help/policy");
  if (help_table) {
    for (i = 0; i <= top_of_helpt; i++)
      free_help(help_table + i);
    free(help_table);
    help_table = NULL;
  }
  if (poli_table) {
    for (i = 0; i <= top_of_polit; i++)
      free_poli(poli_table + i);
    free(poli_table);
    poli_table = NULL;
  }

  mlog("Help/policy free'd, moving onto socials");
  de_boot_socials();

  mlog("Socials free'd, moving on to named specs");
  de_boot_namedspecials();

  mlog("Named specials free'd, moving on to guilds");
  free_guild_data();

  mlog("Guilds free'd, moving on to skill index");
  free_skill_index();

  mlog("Skill index free'd, moving on to banned/invalid names");
  free_banned();
  free_invalid_list();

  mlog("Banned/invalid names free'd, moving on to DNS cache");
  free_dns_cache();

  mlog("DNS cache free'd. moving on to reset queue");
  for (iter = reset_q.head; iter != NULL; iter = inext) {
    inext = iter->next;
    free(iter);
  }
  reset_q.head = reset_q.tail = NULL;
  
  mlog("Reset queue free'd, moving on to triggers");
  for (i = 0; i < top_of_trigt; i++) {
    st_free_index_element(trig_index[i]);
    free(trig_index[i]);
  }
  free(trig_index);
  trigger_list = NULL;

  while(olc_save_list) {
    olc_remove_from_save_list(olc_save_list->zone,olc_save_list->type);
  }
  olc_save_list = NULL;

  mlog("Triggers free'd, all should be done here...Or at least, the most important stuff. :)\n");
}

int port_used,headpid;

int main(int argc, char **argv)
{
  int port;
  char buf[512],*buf2;
  int pos = 1;
  char *dir;
  void init_game(int port);
  int xmlSubstituteEntitiesDefault(int val);

  port_used = port = DFLT_PORT;
  dir = DFLT_DIR;

  while ((pos < argc) && (*(argv[pos]) == '-')) {
    switch (*(argv[pos] + 1)) {
    case 'd':
      if (*(argv[pos] + 2))
    dir = argv[pos] + 2;
      else if (++pos < argc)
    dir = argv[pos];
      else {
    mlog("Directory arg expected after option -d.");
    exit(1);
      }
      break;
    case 'm':
      mini_mud = 1;
      no_rent_check = 1;
      mlog("Running in minimized mode & with no rent check.");
      break;
    case 'c':
      scheck = 1;
      mlog("Syntax check mode enabled.");
      break;
    case 'q':
      no_rent_check = 1;
      mlog("Quick boot mode -- rent check supressed.");
      break;
    case 'r':
      restrict_run = 1;
      mlog("Restricting game -- no new players allowed.");
      break;
    case 's':
      no_specials = 1;
      mlog("Suppressing assignment of special routines.");
      break;
    case 'h':
      buf2 = (argv[pos]+2);
      while ('-' != *buf2)
        buf2++;
      *buf2 = '\0';
      buf2++;
      mother_desc = atoi(argv[pos]+2);
      headpid = atoi(buf2);
      alog("DEBUG: %s-%s -> %d-%d",argv[pos]+2,buf2,mother_desc,headpid);

      mother_desc = (*(argv[pos++]+2)-'0');
      hotboot = 1;
      mlog("Hot boot in progress.....");
      chdir("../");
      break;
    default:
      sprintf(buf, "SYSERR: Unknown option -%c in argument string.", *(argv[pos] + 1));
      mlog(buf);
      break;
    }
    pos++;
  }

  if (pos < argc) {
    if (!isdigit(*argv[pos])) {
      fprintf(stderr, "Usage: %s [-c] [-m] [-q] [-r] [-s] [-d pathname] [port #]\n", argv[0]);
      exit(1);
    } else if ((port = atoi(argv[pos])) <= 1024) {
      fprintf(stderr, "Illegal port number.\n");
      exit(1);
    }
    port_used = port;
  }
#ifdef CIRCLE_WINDOWS
  if (_chdir(dir) < 0) {
#else
  if (chdir(dir) < 0) {
#endif
    mlog("SYSERR:Fatal error changing to data directory");
    exit(1);
  }
  sprintf(buf, "Using %s as data directory.", dir);
  mlog(buf);

  alog("Substitute entites original value: %d. Set to 1 :)", xmlSubstituteEntitiesDefault(1));

  if (scheck) {
    boot_world();
    mlog("Done.");
    exit(0);
  } else {
    if (!hotboot) {
      sprintf(buf, "Running game on port %d.", port);
      mlog(buf);
    } else {
      alog("Recovering from hotboot....");
    }
    init_game(port);
  }

  return 0;
}

gboolean accept_new(GIOChannel *source, GIOCondition condition, gpointer data) {
  int new_descriptor(socket_t s);

  if (!hotboot)
    new_descriptor(mother_desc);

  return TRUE;
}

/* Init sockets, run game, and cleanup sockets */
void init_game(int port)
{
  struct descriptor_data *d;
  void signal_setup(void);
  gboolean game_loop(gpointer data);
  socket_t init_socket(int port);
  int get_max_players(void);
  int process_output(struct descriptor_data *t);

  main_loop = g_main_new(FALSE);

  srandom(time(0));

  mlog("Finding player limit.");
  max_players = get_max_players();

  if (!hotboot) {
    /* If hotboot mother_desc is already set up */
    mlog ("Opening mother connection.");
    mother_desc = init_socket(port);
  }
  else {
    nonblock(mother_desc);
    listen(mother_desc, 5);
  }

  g_mother_desc = g_io_channel_unix_new(mother_desc);
  gmde_num = g_io_add_watch_full(g_mother_desc, G_PRIORITY_LOW, G_IO_IN, accept_new, NULL, NULL);

  boot_db();

  // Wait until db is booted....
  if (hotboot) {
    int rc = recover_from_hotboot();
    if (rc < 0) {
      mlog("Failed to recover from hotboot...Exiting...");
      exit(1);
    }
    alog("Recoverd OK from hotboot, running on port %d",port);
  }
#ifndef CIRCLE_WINDOWS
  mlog("Signal trapping.");
  tics = 1;
  signal_setup();
#endif

  mlog("Entering game loop.");
  g_timeout_add_full(G_PRIORITY_DEFAULT_IDLE, OPT_USEC / 1000, game_loop, NULL, NULL);

  g_main_run(main_loop);

  g_main_destroy(main_loop);

  mlog("Closing all sockets and extracting characters.");
  while ((d = descriptor_list)) {
    if (!d)
      break;
    if (*d->output->str)
      process_output(d);
    close_socket(d);
    
  }

  g_io_channel_close(g_mother_desc);
  g_source_remove(gmde_num);
  CLOSE_SOCKET(mother_desc);

  if (circle_reboot) {
    mlog("Rebooting.");
  /*  free_all_mem(); only when checking for memleaks! */
    exit(52);                   /* what's so great about HHGTTG, anyhow? */
  }
  if (0)
    free_all_mem(); 
  mlog("Normal termination of game.");
}


/*
 * init_socket sets up the mother descriptor - creates the socket, sets
 * its options up, binds it, and listens.
 */
int init_socket(int port)
{
  int s, opt;
  struct sockaddr_in sa;
  /*
   * Should the first argument to socket() be AF_INET or PF_INET?  I don't
   * know, take your pick.  PF_INET seems to be more widely adopted, and
   * Comer (_Internetworking with TCP/IP_) even makes a point to say that
   * people erroneously use AF_INET with socket() when they should be using
   * PF_INET.  However, the man pages of some systems indicate that AF_INET
   * is correct; some such as ConvexOS even say that you can use either one.
   * All implementations I've seen define AF_INET and PF_INET to be the same
   * number anyway, so ths point is (hopefully) moot.
   */

#ifdef CIRCLE_WINDOWS
  {
    WORD wVersionRequested;
    WSADATA wsaData;

    wVersionRequested = MAKEWORD(1, 1);

    if (WSAStartup(wVersionRequested, &wsaData) != 0) {
      mlog("WinSock not available!\n");
      exit(1);
    }
    if ((wsaData.iMaxSockets - 4) < max_players) {
      max_players = wsaData.iMaxSockets - 4;
    }
    sprintf(buf, "Max players set to %d", max_players);
    mlog(buf);

    if ((s = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
      fprintf(stderr, "Error opening network connection: Winsock err #%d\n", WSAGetLastError());
      exit(1);
    }
  }
#else
  if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    mlog("SYSERR:Error creating socket");
    exit(1);
  }
#endif				/* CIRCLE_WINDOWS */

#if defined(SO_SNDBUF)
  opt = LARGE_BUFSIZE + GARBAGE_SPACE;
  if (setsockopt(s, SOL_SOCKET, SO_SNDBUF, (char *) &opt, sizeof(opt)) < 0) {
    mlog("SYSERR:setsockopt SNDBUF");
    exit(1);
  }
#endif

#if defined(SO_REUSEADDR)
  opt = 1;
  if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt)) < 0) {
    mlog("SYSERR:setsockopt REUSEADDR");
    exit(1);
  }
#endif

#if defined(SO_LINGER)
  {
    struct linger ld;

    ld.l_onoff = 0;
    ld.l_linger = 0;
    if (setsockopt(s, SOL_SOCKET, SO_LINGER, (char *) &ld, sizeof(ld)) < 0) {
      mlog("SYSERR:setsockopt LINGER");
      exit(1);
    }
  }
#endif

  sa.sin_family = AF_INET;
  sa.sin_port = htons(port);
  sa.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(s, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
    mlog("SYSERR:bind");
    mlog(strerror(errno));
    CLOSE_SOCKET(s);
    exit(1);
  }
  nonblock(s);
  listen(s, 5);
  return s;
}


int get_max_players(void)
{
#if defined(CIRCLE_OS2) || defined(CIRCLE_WINDOWS)
  return MAX_PLAYERS;
#else

  int max_descs = 0;
  char *method;

/*
 * First, we'll try using getrlimit/setrlimit.  This will probably work
 * on most systems.
 */
#if defined (RLIMIT_NOFILE) || defined (RLIMIT_OFILE)
#if !defined(RLIMIT_NOFILE)
#define RLIMIT_NOFILE RLIMIT_OFILE
#endif
  {
    struct rlimit limit;

    /* find the limit of file descs */
    method = "rlimit";
    if (getrlimit(RLIMIT_NOFILE, &limit) < 0) {
      mlog("SYSERR:calling getrlimit");
      exit(1);
    }
    /* set the current to the maximum */
    limit.rlim_cur = limit.rlim_max;
    if (setrlimit(RLIMIT_NOFILE, &limit) < 0) {
      mlog("SYSERR:calling setrlimit");
      exit(1);
    }
#ifdef RLIM_INFINITY
    if (limit.rlim_max == RLIM_INFINITY)
      max_descs = MAX_PLAYERS + NUM_RESERVED_DESCS;
    else
      max_descs = MIN(MAX_PLAYERS + NUM_RESERVED_DESCS, limit.rlim_max);
#else
    max_descs = MIN(MAX_PLAYERS + NUM_RESERVED_DESCS, limit.rlim_max);
#endif
  }

#elif defined (OPEN_MAX) || defined(FOPEN_MAX)
#if !defined(OPEN_MAX)
#define OPEN_MAX FOPEN_MAX
#endif
  method = "OPEN_MAX";
  max_descs = OPEN_MAX;		/* Uh oh.. rlimit didn't work, but we have
                 * OPEN_MAX */
#elif defined (POSIX)
  /*
   * Okay, you don't have getrlimit() and you don't have OPEN_MAX.  Time to
   * use the POSIX sysconf() function.  (See Stevens' _Advanced Programming
   * in the UNIX Environment_).
   */
  method = "POSIX sysconf";
  errno = 0;
  if ((max_descs = sysconf(_SC_OPEN_MAX)) < 0) {
    if (errno == 0)
      max_descs = MAX_PLAYERS + NUM_RESERVED_DESCS;
    else {
      mlog("SYSERR:Error calling sysconf");
      exit(1);
    }
  }
#else
  /* if everything has failed, we'll just take a guess */
  max_descs = MAX_PLAYERS + NUM_RESERVED_DESCS;
#endif

  /* now calculate max _players_ based on max descs */
  max_descs = MIN(MAX_PLAYERS, max_descs - NUM_RESERVED_DESCS);

  if (max_descs <= 0) {
    sprintf(buf, "Non-positive max player limit!  (Set at %d using %s).",
        max_descs, method);
    mlog(buf);
    exit(1);
  }
  sprintf(buf, "Setting player limit to %d using %s.", max_descs, method);
  mlog(buf);
  return max_descs;
#endif				/* WINDOWS or OS2 */
}

void proc_output(gpointer data, gpointer user_data) {
  struct descriptor_data *d = (struct descriptor_data *)data;
  int process_output(struct descriptor_data *t);

  if (!d || !d->output)
    return;

  if (d->output->len) {
    /* Output for this player is ready */
    if (process_output(d) < 0) {
      close_socket(d);
      return;
    }
    else
      d->prompt_mode = 0;
  }
  if (!d->prompt_mode) {
    make_prompt(d);
    d->prompt_mode = 1;
  }
}

void proc_comm(gpointer data, gpointer user_data) {
  struct descriptor_data *d = (struct descriptor_data *)data;
  int aliased;
  char comm[MAX_INPUT_LENGTH];
  int perform_alias(struct descriptor_data *d, char *orig);
  int get_from_q(struct txt_q *queue, char *dest, int *aliased);

  if (!d )
    return;
  if ( STATE(d) == CON_CLOSE)
  {
    if (get_from_q(&d->input, comm, &aliased))
      axlog( SYS_ERROR, 200, TRUE, "SYSERR: Queued Data (%s) for closed socket in proc_comm", comm );
    return;
  }

  if (d->character && (d->wait -= (d->wait > 0)))
    return;

  if (!get_from_q(&d->input, comm, &aliased))
    return;

  if (d->character) 
  {
    /* Reset the idle timer & pull char back from void if necessary */
    d->character->char_specials.timer = 0;
    if (STATE(d) == CON_PLAYING && GET_WAS_IN(d->character) != NOWHERE) {
      if (IN_ROOM(d->character) != NOWHERE)
        char_from_room(d->character);
      char_to_room(d->character, GET_WAS_IN(d->character));
      GET_WAS_IN(d->character) = NOWHERE;
      act("$n has returned.", TRUE, d->character, 0, 0, TO_ROOM);
    }
    d->wait = 1;
  }
  d->prompt_mode = 0;

  if (d->showstr_count) /* Reading something w/ pager */
    show_string(d, comm);
  else if (d->str)              /* Writing boards, mail, etc. */
    string_add(d, comm);
  else if (STATE(d) != CON_PLAYING) /* In menus, etc. */
    nanny(d, comm);
  else {                        /* else: we're playing normally. */
    if (d->character && GET_PREFIX(d->character) && *GET_PREFIX(d->character) && strncmp(comm, "prefix", strlen("prefix"))) {
      sprintf(buf, "%s ", GET_PREFIX(d->character));
      strcat(buf, comm);
      strcpy(comm, buf);
    }

    if (aliased)                /* To prevent recursive aliases. */
      d->prompt_mode = 1;        /* To get newline before next cmd output. */
    else if (perform_alias(d, comm))    /* Run it through aliasing system */
      get_from_q(&d->input, comm, &aliased);
    command_interpreter(d->character, comm); /* Send it to interpreter */
  }
}

gboolean proc_desc_exc(GIOChannel *source, GIOCondition condition, gpointer data) {
  struct descriptor_data *d = (struct descriptor_data *)data;

  if (!d)
    return FALSE;

  if (IS_SET(condition, G_IO_ERR | G_IO_HUP))
    close_socket(d);
  return TRUE;
}

static int process_input(struct descriptor_data *d);

gboolean proc_desc_input(GIOChannel *source, GIOCondition condition, gpointer data) {
  struct descriptor_data *d = (struct descriptor_data *)data;
  int empty_q(struct txt_q *queue);
  int ret = 0;

  if (!d)
    return FALSE;

  if (IS_SET(condition, G_IO_IN)) {
    if ((ret = process_input(d)) == 0)
      return TRUE;
    else if (ret == -1) {
      close_socket(d);
      return TRUE;
    }
    while( d && !empty_q(&d->input))
      proc_comm(d, NULL);
  }
  return TRUE;
}

/*
 * game_loop contains the main loop which drives the entire MUD.  It
 * cycles once every 0.10 seconds and is responsible for accepting new
 * new connections, polling existing connections for input, dequeueing
 * output and sending it out to players, and calling "heartbeat" functions
 * such as mobile_activity().
 */
gboolean game_loop(gpointer data)
{
  struct descriptor_data *d = NULL, *prev = NULL;

  for (d = descriptor_list; d;) 
  {
    if (STATE(d) == CON_CLOSE) 
    {
      if (d->output && *d->output->str)
	proc_output(d, NULL);
      close_socket(d);
      d = prev;
    }
    else
      proc_output(d, NULL);
    prev = d;
    if (d)
      d = d->next;
  }

  tics++;
  heartbeat(data);
  if (circle_shutdown) 
    g_main_quit(main_loop);

  return TRUE;
}

void check_objprogs()
{
  int oprog = 0, i = 0;
  struct obj_data *obj, *next;
  struct char_data *dum;

  for (i = 0; i < top_of_world; i++) {
    if ((dum = world[i].people) && (world[i].contents))
      for (obj = world[i].contents; obj; obj = next) {
        next = obj->next_content;
        if (GET_OBJPROG(obj)) {
          oprog  = check_objprogs_at_interval(obj, OPC_TICK);
          oprog += check_objprogs_at_interval(obj, OPC_TIME);
          oprog += check_objprogs_at_interval(obj, OPC_RANDOM);
          if (oprog)
            run_objprog(dum, obj);
      }
    }
  }
}

void check_mobprogs()
{
  int mprog = 0;
  struct char_data *mob;
    
  for (mob = character_list; mob; mob = mob->next) {
    if (!IS_MOB(mob) || FIGHTING(mob))
      continue;
      
    mprog = 0;
    mprog += check_mobprogs_at_interval(mob, MPC_TICK);
    mprog += check_mobprogs_at_interval(mob, MPC_TIME);
    mprog += check_mobprogs_at_interval(mob, MPC_RANDOM);
    if (mprog)
      run_mobprog(mob, 0);
  }
}

gboolean heartbeat(gpointer data)
{
  static int mins_since_crashsave = 0;

  if (hotboot)
    return TRUE;

  if (++pulse == ULONG_MAX)
    pulse = 0;

  dg_global_pulse++;
  timed_hotboot();
  process_events();
  if (!(pulse % PULSE_DG_SCRIPT))
   script_trigger_check();

  if (!(pulse % (PASSES_PER_SEC/2)))
    fast_update();

  if (!(pulse % PULSE_ZONE))
    zone_update();

  if (!(pulse % (15 * PASSES_PER_SEC)))		/* 15 seconds */
    check_idle_passwords();

  if (!(pulse % PULSE_MOBILE))
    mobile_activity();

  if (!(pulse % PULSE_VIOLENCE))  {
    perform_violence();
    update_timers();
    raffect_update(RAFF_QUICK);
  }
  if (!(pulse % (30*PASSES_PER_SEC))) {
    mana_update(!(10*30*PASSES_PER_SEC));
    write_whowebpage();
  }

  if (!(pulse % (60 * PASSES_PER_SEC)))
    update_skims();

  if (!(pulse % PULSE_LONGING)) {
    add_longing();
    sub_longing();
  } 
  
  if (!(pulse % PULSE_LMESS))
    do_longing();


  if (!(pulse % PULSE_TAINT)) {
    taint_effects();
  }

  if (!(pulse % PULSE_NOINC)) {
    update_noinc();
  }

  if (!(pulse % (3 * PASSES_PER_SEC))) 
    heartbeat_regen();

  if (!(pulse % (SECS_PER_MUD_HOUR * PASSES_PER_SEC))) {
    weather_and_time(1);
    raffect_update(RAFF_SLOW);
    affect_update();
    point_update();
    check_mobprogs();
    check_objprogs();
  }
  if (auto_save && !(pulse % (2 * 60 * PASSES_PER_SEC))) {	/* 1 minute */
    if (++mins_since_crashsave >= autosave_time) {
      mins_since_crashsave = 0;
      save_all_xml();
    }
  }
  if (!(pulse % (5 * 60 * PASSES_PER_SEC))) {	/* 5 minutes */
    record_usage();
    five_minute_update();
  }

  return TRUE;
}


/* ******************************************************************
*  general utility stuff (for local use)                            *
****************************************************************** */

/*
 *  new code to calculate time differences, which works on systems
 *  for which tv_usec is unsigned (and thus comparisons for something
 *  being < 0 fail).  Based on code submitted by ss@sirocco.cup.hp.com.
 */

/*
 * code to return the time difference between a and b (a-b).
 * always returns a nonnegative value (floors at 0).
 */
struct timeval timediff(struct timeval a, struct timeval b)
{
  struct timeval rslt;

  if (a.tv_sec < b.tv_sec)
    return null_time;
  else if (a.tv_sec == b.tv_sec) {
    if (a.tv_usec < b.tv_usec)
      return null_time;
    else {
      rslt.tv_sec = 0;
      rslt.tv_usec = a.tv_usec - b.tv_usec;
      return rslt;
    }
  } else {			/* a->tv_sec > b->tv_sec */
    rslt.tv_sec = a.tv_sec - b.tv_sec;
    if (a.tv_usec < b.tv_usec) {
      rslt.tv_usec = a.tv_usec + 1000000 - b.tv_usec;
      rslt.tv_sec--;
    } else
      rslt.tv_usec = a.tv_usec - b.tv_usec;
    return rslt;
  }
}

/* add 2 timevals */
struct timeval timeadd(struct timeval a, struct timeval b)
{
  struct timeval rslt;

  rslt.tv_sec = a.tv_sec + b.tv_sec;
  rslt.tv_usec = a.tv_usec + b.tv_usec;

  while (rslt.tv_usec >= 1000000) {
    rslt.tv_usec -= 1000000;
    rslt.tv_sec++;
  }

  return rslt;
}


void record_usage(void)
{
  int sockets_connected = 0, sockets_playing = 0;
  struct descriptor_data *d;
  char buf[256];

  for (d = descriptor_list; d; d = d->next) {
    sockets_connected++;
    if (!d->connected)
      sockets_playing++;
  }

  sprintf(buf, "nusage: %-3d sockets connected, %-3d sockets playing",
      sockets_connected, sockets_playing);
  mlog(buf);

#ifdef RUSAGE
  {
    struct rusage ru;

    getrusage(0, &ru);
    sprintf(buf, "rusage: user time: %ld sec, system time: %ld sec, max res size: %ld",
        ru.ru_utime.tv_sec, ru.ru_stime.tv_sec, ru.ru_maxrss);
    mlog(buf);
  }
#endif

}



/*
 * Turn off echoing (specific to telnet client)
 */
void echo_off(struct descriptor_data *d)
{
  char off_string[] =
  {
    (char) IAC,
    (char) WILL,
    (char) TELOPT_ECHO,
    (char) 0,
  };

  SEND_TO_Q(off_string, d);
}


/*
 * Turn on echoing (specific to telnet client)
 */
void echo_on(struct descriptor_data *d)
{
  char on_string[] =
  {
    (char) IAC,
    (char) WONT,
    (char) TELOPT_ECHO,
    (char) TELOPT_NAOFFD,
    (char) TELOPT_NAOCRD,
    (char) 0,
  };

  SEND_TO_Q(on_string, d);
}

static char *make_custom_prompt(struct char_data *ch, char *buf)
{
  char *p = GET_CPROMPT(ch),*bb = NULL,*rc = buf;
  char b[200];
  GString *res,*col;
  char *tmstr = NULL, *temp = NULL;
  time_t currtime = time(0);

  if (NULL == buf)
    return "";

  if (NULL == ch || NULL == GET_CPROMPT(ch)) {
    *buf = '\0';
    return buf;
  }

  *buf = '\0';
  while (*p) {
    if ('%' != *p) {
      *buf = *p;
      buf++;
      *buf = '\0';
      p++;
      continue;
    } else {
      /* *p == %, hop to next p, i.e code */
      p++;
      switch (*p) {
      case 'h':
    if (GET_HIT(ch) < 0.25*GET_MAX_HIT(ch))
      bb = CCRED(ch,C_NRM);
    if (GET_HIT(ch) < 0.5*GET_MAX_HIT(ch))
      bb = CCYEL(ch,C_NRM);
    else
      bb = CCGRN(ch,C_NRM);
    sprintf(b,"%s%d%s",bb,GET_HIT(ch),CCNRM(ch,C_NRM));
    strcat(buf,b);
    buf += strlen(b);
    break;
      case 'H':
    sprintf(b,"%d",GET_MAX_HIT(ch));
    strcat(buf,b);      
    buf += strlen(b);
    break;
      case 'm':
    sprintf(b,"%ld",GET_MANA(ch));
    strcat(buf,b);
    buf += strlen(b);
    break;
      case 'M':
    sprintf(b,"%ld",GET_MAX_MANA(ch));
    strcat(buf,b);
    buf += strlen(b);
    break;
      case 'v':
    sprintf(b,"%d",GET_MOVE(ch));
    strcat(buf,b);
    buf += strlen(b);
    break;
      case 'V':
    sprintf(b,"%d",GET_MAX_MOVE(ch));
    strcat(buf,b);
    buf += strlen(b); 
    break;
      case 'E':
    sprintf(b,"%ld",GET_LEVEL(ch)*GET_LEVEL_EXP(ch) - GET_EXP(ch));
    strcat(buf,b);
    buf += strlen(b);
    break;
      case 'g':
    sprintf(b,"%ld",GET_GOLD(ch));
    strcat(buf,b);
    buf += strlen(b);
    break;
      case 'G':
    sprintf(b,"%ld",GET_BANK_GOLD(ch));
    strcat(buf,b);
    buf += strlen(b);
    break;
      case 'N':
    sprintf(b, "%s", GET_NAME(ch));
    strcat(buf, b);
    buf +=strlen(b);
    break;
      case 'l':
    sprintf(b, "%ld", GET_LEVEL(ch));
    strcat(buf, b);
    buf +=strlen(b);
    break;
      case 'i':
    sprintf(b, "%s>&n", IS_SET(PRF_FLAGS(ch),PRF_IC) ? "&w" : "&r");
    strcat(buf, b);
    buf +=strlen(b);
    break;
      case 'c':
    sprintf(b, "%s", GET_MYCOLOR(ch));
    strcat(buf, b);
    buf +=strlen(b);
    break;
      case 'n':
    sprintf(b, "\n");
    strcat(buf, b);
    buf +=strlen(b);
    break;
      case 'I':
    sprintf(b, "%s", IS_SET(PRF_FLAGS(ch),PRF_IC) ? "IC" : "OOC");
    strcat(buf, b);
    buf +=strlen(b);
    break;
      case 'D':
    temp = (char *) asctime(localtime(&currtime));
    *(temp + strlen(temp) - 1) = '\0';
    tmstr = strdup(temp);
    sprintf(b, "%s", tmstr);
    strcat(buf, b);
    buf +=strlen(b);
    break;
      default:
    *buf = *(p-1);
    buf++;
    *buf = *p;
    buf++;
    break;
      }
      p++;
      *buf = '\0';
    }
  }
  *buf = '\0';

  strcat(rc," "); /* always end prompt with a space */

  col = proc_color((res = g_string_new(rc)),clr(ch,C_NRM));
  
  strcpy(rc,col->str);
  g_string_free(res,1);
  g_string_free(col,1);

  return rc;
}

void make_prompt(struct descriptor_data *d)
{
  char prompt[MAX_INPUT_LENGTH],p2[MAX_INPUT_LENGTH];
  GString *tmp = NULL, *tmp1 = NULL;

  if (d->showstr_count) {
    if (d->character) {
      sprintf(prompt,
          "\r\n&n[&r&uEnter&n/&r&uQ&nuit/&r&uR&nefresh/&r&uB&nack______________________________________or page #(&r&u%ld&n/&r&u%ld&n)]",
          d->showstr_page,  d->showstr_count);
    } else
      sprintf(prompt,
          "\r\n[ Enter to continue, (q)uit, (r)efresh, (b)ack, or page number (%ld/%ld) ]",
          d->showstr_page, d->showstr_count);
    strcat(prompt,"\r\n");
    tmp1 = proc_color((tmp = g_string_new(prompt)), d->character ? clr(d->character, C_NRM) : 0);
    if (write_to_descriptor(d, d->descriptor, tmp1->str) < 0) {
      g_string_free(tmp, TRUE);
      g_string_free(tmp1, TRUE);
      STATE(d) = CON_CLOSE;
      return;
    }
    g_string_free(tmp, TRUE);
    g_string_free(tmp1, TRUE);
  } 
  else if (d->str) {
    if (d->current_text_line)
      sprintf(buf, "%3ld", d->current_text_line);
    else
      sprintf(buf, "  ?");

    if(d->character && IS_SET(PRF_FLAGS(d->character),PRF_IC))
      sprintf(buf, "%s&w]&n ", buf);
    else
      sprintf(buf,"%s&r]&n ", buf);
    tmp1 = proc_color((tmp = g_string_new(buf)), d->character ? clr(d->character, C_NRM) : 0);
    if (write_to_descriptor(d, d->descriptor, tmp1->str) < 0) {
      g_string_free(tmp, TRUE);  
      g_string_free(tmp1, TRUE);  
      STATE(d) = CON_CLOSE;
      return;
    }
    g_string_free(tmp, TRUE);  
    g_string_free(tmp1, TRUE);  
  }
  else if (!d->connected) {
    char prompt[MAX_INPUT_LENGTH];

    *prompt = '\0';

    if (GET_INVIS_LEV(d->character))
      sprintf(prompt, "i%d ", GET_INVIS_LEV(d->character));

    if (has_mail(GET_IDNUM(d->character)))
      sprintf(prompt, "%s(mail waiting) ", prompt);

    if (PLR_FLAGGED(d->character,PLR_CPROMPT)) {
      make_custom_prompt(d->character,p2);
      sprintf(prompt,"%s%s",prompt,p2);
    } else {
      if (PRF_FLAGGED(d->character, PRF_DISPHP))
    sprintf(prompt, "%s%s%dH&n ", prompt, 
        (GET_HIT(d->character) < 0.25*GET_MAX_HIT(d->character)) ?
        "&r" : ((GET_HIT(d->character) < 0.5*GET_MAX_HIT(d->character)) ?
        "&y" : "&g"), GET_HIT(d->character));

      if (PRF_FLAGGED(d->character, PRF_DISPMANA))
    sprintf(prompt, "%s%ldM ", prompt, GET_MANA(d->character));
      if (PRF_FLAGGED(d->character, PRF_DISPMOVE))
    sprintf(prompt, "%s%dV ", prompt, GET_MOVE(d->character));
      if (PRF_FLAGGED(d->character, PRF_DISPEXP))
    sprintf(prompt, "%s%ldX ", prompt, 
        GET_LEVEL(d->character)*GET_LEVEL_EXP(d->character) - GET_EXP(d->character));
      
      if(IS_SET(PRF_FLAGS(d->character),PRF_IC))
    sprintf(prompt, "%s&w>&n ", prompt);
      else
    sprintf(prompt,"%s&r>&n ", prompt);
    }
    if (IS_AFFECTED(d->character,AFF_HIDE))
      strcat(prompt,"(h) ");
    if (IS_SET(PRF2_FLAGS(d->character),PRF2_AFW)) 
      strcat(prompt,"(afw) ");
    if (IS_SET(PRF_FLAGS(d->character),PRF_AFK)) 
      strcat(prompt,"(afk) ");
    if (IS_AFFECTED(d->character,AFF_SNEAK))
      strcat(prompt,"(s) ");
    if (PRF_FLAGGED(d->character,PRF_GRASPING))
      strcat(prompt,"(grasping) ");
    if (IS_AFFECTED(d->character,AFF_VEILED))
      strcat(prompt,"(v) ");
    if (IS_AFFECTED(d->character,AFF_CAMOUFLAGED))
      strcat(prompt,"(c) ");
    if (IS_AFFECTED(d->character,AFF_INVISIBLE))
      strcat(prompt,"(i) ");
    if (GET_GUILD_INVIS(d->character))
      strcat(prompt, "(gi) ");
    if (d->character->player_specials->buffer_flag == 1)
      strcat(prompt, "(Type 'tells') ");
    if (PRF_FLAGGED(d->character, PRF_NOWIZ) && 
            GET_LEVEL(d->character) >= LVL_IMMORT)
      strcat(prompt,"(!) ");
    if (GET_PREFIX(d->character))
      sprintf(prompt, "%s%s ", prompt, GET_PREFIX(d->character));


    if (!IS_AFFECTED2(d->character, AFF2_HALT))/* Halted dudes get no prompt */ {
      tmp1 = proc_color((tmp = g_string_new(prompt)), clr(d->character, C_NRM));
      if (write_to_descriptor(d, d->descriptor, tmp1->str) < 0) {
        g_string_free(tmp, TRUE);  
        g_string_free(tmp1, TRUE);  
    STATE(d) = CON_CLOSE;
        return;
      }
      g_string_free(tmp, TRUE);  
      g_string_free(tmp1, TRUE);
    }
  }
}


void write_to_q(const char *txt, struct txt_q *queue, int aliased)
{
  struct txt_block *new;

  CREATE(new, struct txt_block, 1);
  if (NULL == new) {
    mlog("SYSERR: write_to_q() failed to Create txt_block.");
    return;
  }
  if (NULL == txt) {
    mlog("SYSERR: write_to_q() NULL-pointer as text.");
    return;
  }
  if (NULL == (new->text = g_strndup(txt, MAX_INPUT_LENGTH))) {
    mlog("SYSERR: write_to_q() failed to duplicate string.");
    return;
  }
  new->aliased = aliased;

  /* queue empty? */
  if (!queue->head) {
    new->next = NULL;
    queue->head = queue->tail = new;
  } else {
    queue->tail->next = new;
    queue->tail = new;
    new->next = NULL;
  }
}

int empty_q(struct txt_q *queue) {
  if (NULL == queue || NULL == queue->head)
    return 1;
  return 0;
}

int get_from_q(struct txt_q *queue, char *dest, int *aliased)
{
  struct txt_block *tmp;

  /* queue empty? */
  if (NULL == queue || NULL == queue->head || NULL == dest)
    return 0;

  tmp = queue->head;
  if (NULL == queue->head->text) {
    mlog("SYSERR: get_from_q(): NULL-pointer in text!");
  } else
    strcpy(dest, queue->head->text);

  if (NULL == aliased) {
    mlog("SYSERR: get_from_q(): Aliased is NULL-pointer!");
  } else {
    *aliased = queue->head->aliased;
  }
  queue->head = queue->head->next;

  if (NULL != tmp->text)
    free(tmp->text);
  free(tmp);

  return 1;
}



/* Empty the queues before closing connection */
void flush_queues(struct descriptor_data *d)
{
  int dummy;
  char comm[MAX_INPUT_LENGTH];

  g_string_free(d->output, TRUE);
  d->output = NULL;

  while (get_from_q(&d->input, comm, &dummy));
}

/* Add a new string to a player's output queue */
void write_to_output(const char *txt, struct descriptor_data *t)
{
  if (t->output == NULL)
    t->output = g_string_new(txt);
  else
    t->output = g_string_append(t->output, txt);
}



/* ******************************************************************
*  socket handling                                                  *
****************************************************************** */


int new_descriptor(int s)
{
/*  socket_t desc; */
  int desc, sockets_connected = 0;
  unsigned long addr;
  unsigned int i;
  struct dns_entry dns;
  static int last_desc = 0;	/* last descriptor number */
  struct descriptor_data *newd;
  struct sockaddr_in peer;
  struct hostent *from;
  memset(&peer, 0, sizeof(struct sockaddr_in));

  /* accept the new connection */
  if (!hotboot) {
    i = sizeof(peer);
    if ((desc = accept(s, (struct sockaddr *) &peer, &i)) == INVALID_SOCKET) {
      return -1;
    }
  } else {
    desc = s;
  }
  /* keep it from blocking */
  nonblock(desc);

  /* make sure we have room for it */
  for (newd = descriptor_list; newd; newd = newd->next)
    sockets_connected++;

  if (sockets_connected >= max_players) {
    write_to_descriptor(NULL, desc, "Sorry, CircleMUD is full right now... please try again later!\r\n");
    close(desc);
    return 0;
  }
  /* create a new descriptor */
  CREATE(newd, struct descriptor_data, 1);
  memset((char *) newd, 0, sizeof(struct descriptor_data));

   /* find the numeric site address */
  addr = ntohl(peer.sin_addr.s_addr);
  dns.ip[0] = (int) ((addr & 0xFF000000) >> 24);
  dns.ip[1] = (int) ((addr & 0x00FF0000) >> 16);
  dns.ip[2] = (int) ((addr & 0x0000FF00) >> 8);
  dns.ip[3] = (int) ((addr & 0x000000FF));
  dns.name = NULL;
  dns.next = NULL;

  if(!get_host_from_cache(&dns)) { /* cache lookup failed */
      
  /* find the sitename */
    if (1) { /* since gethostbyaddr causes problems with valgrind */
//    if (nameserver_is_slow || !(from = gethostbyaddr((char *) &peer.sin_addr,
//                      sizeof(peer.sin_addr), AF_INET))) {

    /* resolution failed */
      if (!nameserver_is_slow) {
        mlog("SYSERR:gethostbyaddr..Setting nameserver_is_slow to TRUE again.");
        nameserver_is_slow = TRUE;
      }

     /* find the sitename */
      sprintf(newd->host, "%03u.%03u.%03u.%03u", (int) ((addr & 0xFF000000) >> 24),
      (int) ((addr & 0x00FF0000) >> 16), (int) ((addr & 0x0000FF00) >> 8),
        (int) ((addr & 0x000000FF)));
    
  } else {
    strncpy(newd->host, from->h_name, HOST_LENGTH);
    *(newd->host + HOST_LENGTH) = '\0';
    add_dns_host(&dns,newd->host);
  }
 } else {
     strncpy(newd->host, dns.name, HOST_LENGTH);
     free(dns.name);
   }

  /* determine if the site is banned */
  if (isbanned(newd->host) == BAN_ALL) {
    write_to_descriptor(NULL, desc,"Your site is BANNED!\r\n");
    close(desc);
    sprintf(buf2, "Connection attempt denied from [%s]", newd->host);
    mudlog(buf2, CMP, LVL_GOD, TRUE);
    free(newd);
    return 0;
  }
#if 0
  /* Log new connections - probably unnecessary, but you may want it */
  sprintf(buf2, "New connection from [%s]", newd->host);
  mudlog(buf2, CMP, LVL_GOD, FALSE);
#endif

  /* initialize descriptor data */
  newd->descriptor = desc;
  newd->connected = CON_GET_NAME;
  newd->idle_tics = 0;
  newd->wait = 1;
  newd->output = g_string_new(NULL);
  newd->next = descriptor_list;
  newd->login_time = time(0);
  newd->tries = 0;

  if (++last_desc == 1000)
    last_desc = 1;
  newd->desc_num = last_desc;

  /* prepend to list */
  descriptor_list = newd;
  
  if (!hotboot) {
    SEND_TO_Q(GREETINGS, newd);
  }

  newd->gdescriptor = g_io_channel_unix_new(newd->descriptor);

  newd->gde_num[0] = g_io_add_watch(newd->gdescriptor, G_IO_IN, proc_desc_input, newd);

  newd->gde_num[1] = g_io_add_watch(newd->gdescriptor, G_IO_HUP | G_IO_ERR, proc_desc_exc, newd);

  return 0;
}



int process_output(struct descriptor_data *t)
{
  int result;
  GString *temp = NULL;

  if(t->character)
    temp = proc_color(t->output, clr(t->character, C_NRM));
  else
    temp = proc_color(t->output, 0);

  g_string_assign(t->output, temp->str);
  g_string_free(temp, TRUE);
  temp = NULL;

  if (STATE(t) == CON_PLAYING && t->character && !PRF_FLAGGED(t->character, PRF_COMPACT))
    t->output = g_string_append(t->output, "\r\n");

  /*
   * now, send the output.  If this is an 'interruption', use the prepended
   * CRLF, otherwise send the straight output sans CRLF.
   */
  if (t->prompt_mode)           /* && !t->connected) */
    t->output = g_string_prepend(t->output, "\r\n");
  result = write_to_descriptor(t, t->descriptor, t->output->str);

  /* handle snooping: prepend "% " and send to snooper */
  if (t->snoop_by) {
    SEND_TO_Q("% ", t->snoop_by);
    SEND_TO_Q(t->output->str, t->snoop_by);
    SEND_TO_Q("%%", t->snoop_by);
  }

  g_string_free(t->output, TRUE);
  t->output = g_string_new(NULL);

  return (result);
}



int write_to_descriptor(struct descriptor_data *d, socket_t desc, char *txt)
{
  size_t total;
  ssize_t bytes_written = 0;
  char *orig = txt;

  total = strlen(txt);

  if (d && d->missed && d->missed->len && !d->tries) {
    if (d->missed->str != txt)
      d->missed = g_string_append(d->missed, txt);
    d->tries++;
    if ((bytes_written = write_to_descriptor(d, desc, d->missed->str)) > 0)
      d->missed = g_string_erase(d->missed, 0, bytes_written);
    d->tries = 0;
    if (d->missed->len == 0) {
      g_string_free(d->missed, TRUE);
      d->missed = g_string_new(NULL);
    }
    return 1;
  }

  while (total > 0) {
    bytes_written = write(desc, txt, MIN(total, MAX_INPUT_LENGTH));

    if (bytes_written <= 0) {
      if (errno == EPIPE || !d) {
        perror("SYSERR: Write to socket");
    return -1;
      }
      if (d->tries);
      else if (d->missed)
    d->missed = g_string_append(d->missed, txt);
      else
    d->missed = g_string_new(txt);
      return (txt-orig);
    }
    else {
      txt += bytes_written;
      total -= bytes_written;
    }
  }

  return (txt-orig);
}

/*
RFC 854                                                         May 1983

TELNET COMMAND STRUCTURE

   All TELNET commands consist of at least a two byte sequence:  the
   "Interpret as Command" (IAC) escape character followed by the code
   for the command.  The commands dealing with option negotiation are
   three byte sequences, the third byte being the code for the option
   referenced.  This format was chosen so that as more comprehensive use
   of the "data space" is made -- by negotiations from the basic NVT, of
   course -- collisions of data bytes with reserved command values will
   be minimized, all such collisions requiring the inconvenience, and
   inefficiency, of "escaping" the data bytes into the stream.  With the
   current set-up, only the IAC need be doubled to be sent as data, and
   the other 255 codes may be passed transparently.

   The following are the defined TELNET commands.  Note that these codes
   and code sequences have the indicated meaning only when immediately
   preceded by an IAC.

      NAME               CODE              MEANING

 ð    SE                  240    End of subnegotiation parameters.
 ñ    NOP                 241    No operation.
 ò    Data Mark           242    The data stream portion of a Synch.
                                 This should always be accompanied
                                 by a TCP Urgent notification.
 ó    Break               243    NVT character BRK.
 ô    Interrupt Process   244    The function IP.
 õ    Abort output        245    The function AO.
 ö    Are You There       246    The function AYT.
 ÷    Erase character     247    The function EC.
 ø    Erase Line          248    The function EL.
 ù    Go ahead            249    The GA signal.
 ú    SB                  250    Indicates that what follows is
                                 subnegotiation of the indicated
                                 option.
 û    WILL (option code)  251    Indicates the desire to begin
                                 performing, or confirmation that
                                 you are now performing, the
                                 indicated option.
 ü    WON'T (option code) 252    Indicates the refusal to perform,
                                 or continue performing, the
                                 indicated option.
 ý    DO (option code)    253    Indicates the request that the
                                 other party perform, or
                                 confirmation that you are expecting
                                 the other party to perform, the
                                 indicated option.
 þ    DON'T (option code) 254    Indicates the demand that the
                                 other party stop performing,
                                 or confirmation that you are no
                                 longer expecting the other party
                                 to perform, the indicated option.
 ÿ    IAC                 255    Data Byte 255.
*/

int is_swede(char ch)
{
  if ('ö' == ch || 'Ö' == ch || 'ä' == ch || 'Ä' == ch || 'å' == ch || 'Å' == ch)
  // ¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ
  /*
  makes problems with eg zmud under some circumstances...  :(
  if ( (ch >= 'À' && ch <= 'Æ') || (ch >= 'à' && ch <= 'æ') || ch == 'Ç' || ch == 'ç' ||
       (ch >= 'È' && ch <= 'Ï') || (ch >= 'è' && ch <= 'ï') ||
       (ch >= 'Ñ' && ch <= 'Ö') || ch == 'Ø' || (ch >= 'ñ' && ch <= 'ö') || ch == 'ø' ||
       (ch >= 'Ù' && ch <= 'Ü') || (ch >= 'ù' && ch <= 'ü') )
  */
    return 1;
  return 0;
}

void process_raw_input(struct descriptor_data *t) {
  int failed_subst = 0;
  char sub[MAX_INPUT_LENGTH];

  while (!ISNEWL(t->raw_inbuf->str[0])) {
    if (t->raw_inbuf->str[0] == '\b') {
      if (t->inbuf->len != 0)
    g_string_truncate(t->inbuf, t->inbuf->len - 1);
    }
    else if (t->raw_inbuf->str[0] == '$') {
      g_string_append_c(t->inbuf, '$');
      g_string_append_c(t->inbuf, '$');
    }
    else if ((isascii(t->raw_inbuf->str[0]) && isprint(t->raw_inbuf->str[0])) || is_swede(t->raw_inbuf->str[0]))
      g_string_append_c(t->inbuf, t->raw_inbuf->str[0]);
    g_string_erase(t->raw_inbuf, 0, 1);
  }
  g_string_erase(t->raw_inbuf, 0, 1);
  if (t->raw_inbuf->len != 0 && ISNEWL(t->raw_inbuf->str[0]))
    g_string_erase(t->raw_inbuf, 0, 1);

  if (t->snoop_by) {
    SEND_TO_Q("% ", t->snoop_by);
    SEND_TO_Q(t->inbuf->str, t->snoop_by);
    SEND_TO_Q("\r\n", t->snoop_by);
  }

  if (t->inbuf->str[0] == '!')
    g_string_assign(t->inbuf, t->last_input->str);
  else if (t->inbuf->str[0] == '^') {
    strncpy(sub, t->inbuf->str, MAX_INPUT_LENGTH);
    if (!(failed_subst = perform_subst(t, t->last_input->str, sub)))
      g_string_assign(t->last_input, sub);
  }
  else
    g_string_assign(t->last_input, t->inbuf->str);

  if (!failed_subst) {
    strncpy(sub, t->inbuf->str, MAX_INPUT_LENGTH);
    write_to_q(sub, &t->input, 0);
  }
  g_string_free(t->inbuf, TRUE);
  t->inbuf = g_string_new(NULL);
}

/*
 * ASSUMPTION: There will be no newlines in the raw input buffer when this
 * function is called.  We must maintain that before returning.  */
static int process_input(struct descriptor_data *t)
{
  char inp[MAX_STRING_LENGTH];
  int tst = 0;

  if (!t->inbuf)
    t->inbuf = g_string_new(NULL);
  if (!t->raw_inbuf)
    t->raw_inbuf = g_string_new(NULL);
  if (!t->last_input)
    t->last_input = g_string_new(NULL);

  if ((tst = read(t->descriptor, inp, MAX_STRING_LENGTH-1)) < 0) {
    if (errno != EAGAIN) {
      alog("SYSERR:process_input: about to lose connection");
      return -1;
    }
    else
      return 0;
  }
  else if (tst == 0) {
    if (!hotboot)
      alog("EOF on socket read (connection broken by peer)");
    return -1;
  }
  else {
    inp[tst] = '\0';
    t->raw_inbuf = g_string_append(t->raw_inbuf, inp);
    while (strchr(t->raw_inbuf->str, '\r') || strchr(t->raw_inbuf->str, '\n'))
      process_raw_input(t);
  }

  return 1;
}



/*
 * perform substitution for the '^..^' csh-esque syntax
 * orig is the orig string (i.e. the one being modified.
 * subst contains the substition string, i.e. "^telm^tell"
 */
int perform_subst(struct descriptor_data *t, const char *orig, char *subst)
{
  char new[MAX_INPUT_LENGTH + 5];

  char *first, *second, *strpos;

  /*
   * first is the position of the beginning of the first string (the one
   * to be replaced
   */
  first = subst + 1;

  /* now find the second '^' */
  if (!(second = strchr(first, '^'))) {
    SEND_TO_Q("Invalid substitution.\r\n", t);
    return 1;
  }
  /* terminate "first" at the position of the '^' and make 'second' point
   * to the beginning of the second string */
  *(second++) = '\0';

  /* now, see if the contents of the first string appear in the original */
  if (!(strpos = strstr(orig, first))) {
    SEND_TO_Q("Invalid substitution.\r\n", t);
    return 1;
  }
  /* now, we construct the new string for output. */

  /* first, everything in the original, up to the string to be replaced */
  strncpy(new, orig, (strpos - orig));
  new[(strpos - orig)] = '\0';

  /* now, the replacement string */
  strncat(new, second, (MAX_INPUT_LENGTH - strlen(new) - 1));

  /* now, if there's anything left in the original after the string to
   * replaced, copy that too. */
  if (((strpos - orig) + strlen(first)) < strlen(orig))
    strncat(new, strpos + strlen(first), (MAX_INPUT_LENGTH - strlen(new) - 1));

  /* terminate the string in case of an overflow from strncat */
  new[MAX_INPUT_LENGTH - 1] = '\0';
  strcpy(subst, new);

  return 0;
}



void close_socket(struct descriptor_data *d)
{
  char buf[128];
  struct descriptor_data *temp;
  long target_idnum = -1;
  int i = 0;
  struct char_data *master;
  sigset_t old, new;

  if (hotboot)
    return;

  g_io_channel_close(d->gdescriptor);
  g_source_remove(d->gde_num[0]);
  g_source_remove(d->gde_num[1]);

  sigemptyset(&old);
  sigfillset(&new);
  sigprocmask(SIG_SETMASK,&new,&old);
  CLOSE_SOCKET(d->descriptor);
  sigprocmask(SIG_SETMASK,&old,NULL);

  flush_queues(d);

  /* Forget snooping */
  if (d->snooping)
    d->snooping->snoop_by = NULL;

  if (d->snoop_by) {
    SEND_TO_Q("Your victim is no longer among us.\r\n", d->snoop_by);
    d->snoop_by->snooping = NULL;
  }

  /*. Kill any OLC stuff .*/
  switch(d->connected)
  { case CON_OEDIT:
    case CON_REDIT:
    case CON_ZEDIT:
    case CON_MEDIT:
    case CON_SEDIT:
    case CON_TRIGEDIT:
    case CON_AEDIT:
      cleanup_olc(d, CLEANUP_ALL);
    default:
      break;
  }

  if (d->character) 
  {
    target_idnum = GET_IDNUM(d->character);
    if (d->connected == CON_PLAYING) 
    {
      if ((d->character->in_room) && (d->character->in_room != NOWHERE)
      && (d->character->in_room != 1))
        save_char(d->character, d->character->in_room);
      else
        save_char(d->character, NOWHERE);
      act("$n has lost $s link.", TRUE, d->character, 0, 0, TO_ROOM);
      sprintf(buf, "Closing link to: %s.", GET_NAME(d->character));
      xlog(SYS_CONN, MAX(LVL_IMMORT, GET_INVIS_LEV(d->character)), buf, TRUE);

      if (SESS_FLAGGED(d->character, SESS_EAVESDROP)) {
        (void)SESS_TOG_CHK(d->character, SESS_EAVESDROP);
        rem_eavesdropper(d->character, GET_EAVESDROPROOM(d->character));
        GET_EAVESDROPROOM(d->character) = 0;
      }

      if ((d->character->player_specials->linked_by) || (d->character->player_specials->linking[0]))
      {
        master = (d->character->player_specials->linked_by?
          d->character->player_specials->linked_by : d->character);
        while(master->player_specials->linking[i])
        {
          perform_unlink(master);//, NULL, i);
          ++i;
        }
      }
      d->character->desc = NULL;
    } else 
    {
      struct char_data *temp;
      sprintf(buf, "Losing player: %s.",
          GET_NAME(d->character) ? GET_NAME(d->character) : "<null>");
      mudlog(buf, CMP, LVL_IMMORT, TRUE);
      char_from_room(d->character);
      /* pull the char from the list */
      REMOVE_FROM_LIST(d->character, character_list, next);
      free_char(d->character);
    }
  } else
  {
    //Tiek: This message is mostly just an annoyance.
    //sprintf( buf, "Losing descriptor without char from %s.", d->host );
    //xlog(SYS_CONN, LVL_IMMORT, buf, TRUE); 
  }

  /* JE 2/22/95 -- part of my unending quest to make switch stable */
  if (d->original && d->original->desc)
    d->original->desc = NULL;

  REMOVE_FROM_LIST(d, descriptor_list, next);

  if (d->showstr_head)
    free(d->showstr_head);
  if (d->showstr_count)
    free(d->showstr_vector);
  if (d->output)
    g_string_free(d->output, TRUE);
  if (d->missed)
    g_string_free(d->missed, TRUE);
  if (d->inbuf)
    g_string_free(d->inbuf, TRUE);
  if (d->last_input)
    g_string_free(d->last_input, TRUE);
  free(d);
}



void check_idle_passwords(void)
{
  struct descriptor_data *d, *next_d;

  for (d = descriptor_list; d; d = next_d) {
    next_d = d->next;
    if (STATE(d) != CON_PASSWORD && STATE(d) != CON_GET_NAME)
      continue;
    if (d->idle_tics < 2) {
      d->idle_tics++;
      continue;
    } else {
      echo_on(d);
      SEND_TO_Q("\r\nTimed out... goodbye.\r\n", d);
      STATE(d) = CON_CLOSE;
    }
  }
}



/*
 * I tried to universally convert Circle over to POSIX compliance, but
 * alas, some systems are still straggling behind and don't have all the
 * appropriate defines.  In particular, NeXT 2.x defines O_NDELAY but not
 * O_NONBLOCK.  Krusty old NeXT machines!  (Thanks to Michael Jones for
 * this and various other NeXT fixes.)
 */

#ifdef CIRCLE_WINDOWS

void nonblock(socket_t s)
{
  long val;

  val = 1;
  ioctlsocket(s, FIONBIO, &val);
}

#else

#ifndef O_NONBLOCK
#define O_NONBLOCK O_NDELAY
#endif

void nonblock(socket_t s)
{
  int flags;

  flags = fcntl(s, F_GETFL, 0);
  flags |= O_NONBLOCK;
  if (fcntl(s, F_SETFL, flags) < 0) {
    mlog("SYSERR:Fatal error executing nonblock (comm.c)");
    exit(1);
  }
}


/* ******************************************************************
*  signal-handling functions (formerly signals.c)                   *
****************************************************************** */


RETSIGTYPE checkpointing()
{
  if (!tics) {
    mlog("SYSERR: CHECKPOINT shutdown: tics not updated");
    abort();
  } else
    tics = 0;
}


RETSIGTYPE reread_wizlists()
{
  void reboot_wizlists(void);

  mudlog("Signal received - rereading wizlists.", CMP, LVL_IMMORT, TRUE);
  reboot_wizlists();
}


RETSIGTYPE unrestrict_game()
{
  mudlog("Received SIGUSR2 - completely unrestricting game (emergent)",
     BRF, LVL_IMMORT, TRUE);
  ban_list = NULL;
  restrict_run = 0;
}


RETSIGTYPE hupsig()
{
  struct descriptor_data *d;
  sigset_t old, new;

  mlog("Received SIGHUP, SIGINT, or SIGTERM.  Shutting down...");
  for (d = descriptor_list; d; d = d->next) {
    sigemptyset(&old);
    sigfillset(&new);
    sigprocmask(SIG_SETMASK,&new,&old);
    g_io_channel_close(d->gdescriptor);
    g_source_remove(d->gde_num[0]);
    g_source_remove(d->gde_num[1]);
    close(d->descriptor);
    sigprocmask(SIG_SETMASK,&old,NULL);
    alog("Closing descriptor %d",d->descriptor);
  }

  exit(0);			/* perhaps something more elegant should
                 * substituted */
}

RETSIGTYPE desc_clean()
{
  struct descriptor_data *d;

  mlog("Received KILL. Closing descriptors and shutting down!");
  for (d = descriptor_list; d; d = d->next) {
    close(d->descriptor);
  }
  exit(0);
}

/*
 * This is an implementation of signal() using sigaction() for portability.
 * (sigaction() is POSIX; signal() is not.)  Taken from Stevens' _Advanced
 * Programming in the UNIX Environment_.  We are specifying that all system
 * calls _not_ be automatically restarted for uniformity, because BSD systems
 * do not restart select(), even if SA_RESTART is used.
 *
 * Note that NeXT 2.x is not POSIX and does not have sigaction; therefore,
 * I just define it to be the old signal.  If your system doesn't have
 * sigaction either, you can use the same fix.
 *
 * SunOS Release 4.0.2 (sun386) needs this too, according to Tim Aldric.
 */

#ifndef POSIX
#define my_signal(signo, func) signal(signo, func)
#else
sigfunc *my_signal(int signo, sigfunc * func)
{
  struct sigaction act, oact;

  act.sa_handler = func;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
#ifdef SA_INTERRUPT
  act.sa_flags |= SA_INTERRUPT;	/* SunOS */
#endif

  if (sigaction(signo, &act, &oact) < 0)
    return SIG_ERR;

  return oact.sa_handler;
}
#endif				/* NeXT */


void signal_setup(void)
{
#ifndef CIRCLE_OS2
  struct itimerval itime;
#endif
  struct timeval interval;

  /* user signal 1: reread wizlists.  Used by autowiz system. */
  my_signal(SIGUSR1, reread_wizlists);

  /*
   * user signal 2: unrestrict game.  Used for emergencies if you lock
   * yourself out of the MUD somehow.  (Duh...)
   */
  my_signal(SIGUSR2, unrestrict_game);

  /*
   * set up the deadlock-protection so that the MUD aborts itself if it gets
   * caught in an infinite loop for more than 3 minutes.  Doesn't work with
   * OS/2.
   */
#ifndef CIRCLE_OS2
  interval.tv_sec = 180;
  interval.tv_usec = 0;
  itime.it_interval = interval;
  itime.it_value = interval;
  setitimer(ITIMER_VIRTUAL, &itime, NULL);
  my_signal(SIGVTALRM, checkpointing);
#endif

  /* just to be on the safe side: */
  my_signal(SIGHUP, hupsig);
  my_signal(SIGINT, hupsig);
  my_signal(SIGTERM, hupsig);
  my_signal(SIGPIPE, SIG_IGN);
  my_signal(SIGALRM, SIG_IGN);
  my_signal(SIGKILL, desc_clean);

#ifdef CIRCLE_OS2
#if defined(SIGABRT)
  my_signal(SIGABRT, hupsig);
#endif
#if defined(SIGFPE)
  my_signal(SIGFPE, hupsig);
#endif
#if defined(SIGILL)
  my_signal(SIGILL, hupsig);
#endif
#if defined(SIGSEGV)
  my_signal(SIGSEGV, hupsig);
#endif
#endif				/* CIRCLE_OS2 */

}

#endif				/* CIRCLE_WINDOWS */


/* ****************************************************************
*       Public routines for system-to-player-communication        *
**************************************************************** */

void asend_to_char(struct char_data *ch,char *fmt,...)
{
  va_list argp;
  char tmp[5000];

  if ((!ch->desc && !is_retainer(ch)) || (NULL == fmt)) {
    return;
  }
  
  va_start(argp,fmt);
  vsprintf(tmp,fmt,argp);
  va_end(argp);
 
  send_to_char(tmp,ch);
}

void send_to_char(const char *messg, struct char_data *ch)
{
  if (ch == NULL)
    return;

  if (messg == NULL)
    return;

  if (ch->desc && messg)
    SEND_TO_Q(messg, ch->desc);
  else if (is_retainer(ch))
    send_to_retainer_owner(messg, ch);
}


void asend_to_all(char *fmt,...)
{
  va_list argp;
  char tmp[5000];

  if (NULL == fmt) {
    return;
  }

  va_start(argp,fmt);
  vsprintf(tmp,fmt,argp);
  va_end(argp);

  send_to_all(tmp);
}

void send_to_all(char *messg)
{
  struct descriptor_data *i;

  if (messg)
    for (i = descriptor_list; i; i = i->next)
      if (!i->connected)
    SEND_TO_Q(messg, i);
}

void asend_to_outdoor(char *fmt,...)
{
  va_list argp;
  char tmp[5000];

  if (NULL == fmt) {
    return;
  }

  va_start(argp,fmt);
  vsprintf(tmp,fmt,argp);
  va_end(argp);

  send_to_outdoor(tmp);
}

void send_to_outdoor(char *messg)
{
  struct descriptor_data *i;

  if (!messg || !*messg)
    return;

  for (i = descriptor_list; i; i = i->next)
    if (!i->connected && i->character && AWAKE(i->character) && !PRF2_FLAGGED(i->character, PRF2_NOWEATHER) &&
    OUTSIDE(i->character) && !(IS_AFFECTED(i->character, AFF_DREAMING))
        && (STATE(i) == CON_PLAYING) && !PLR_FLAGGED(i->character, PLR_MAILING | PLR_WRITING))
      SEND_TO_Q(messg, i);
}


void asend_to_room(int room,char *fmt,...)
{
  va_list argp;
  char tmp[5000];       /* nasty, but we shouldn't have longer messgs than this */

  if (NULL == fmt) {
    return;
  }
  va_start(argp,fmt);
  vsprintf(tmp,fmt,argp);  
  va_end(argp);

  send_to_room(tmp,room);
}

void send_to_room(char *messg, int room)
{
  struct char_data *i;

  if (messg  && (room != NOWHERE))
    for (i = world[room].people; i; i = i->next_in_room)
      if ((IS_NPC(i) && (i->desc)) || (!IS_NPC(i) && SENDOK(i)))
    SEND_TO_Q(messg, i->desc);
}
   
void copy_emote_to_room(char *messg, int room, struct char_data *from)
{
  struct char_data *i;
  
  if (messg && (room != NOWHERE))
    for (i = world[room].people; i; i = i->next_in_room) {
      if ((IS_NPC(i) && (i->desc)) || (!IS_NPC(i) && SENDOK(i))) {
        if (PRF2_FLAGGED(i, PRF2_RPTAG))
          SEND_TO_Q("[RP] ", i->desc);

        SEND_TO_Q(messg, i->desc);
        add_to_rplog(i, messg, from);
      }
    }
}

char *ACTNULL = "<NULL>";

#define CHECK_NULL(pointer, expression) \
  if ((pointer) == NULL) i = ACTNULL; else i = (expression);


/* higher-level communication: the act() function */
void perform_act(char *orig, struct char_data *ch, struct obj_data *obj,
         void *vict_obj, struct char_data *to, int to_dreaming)
{
  register char *i = NULL, *buf;
  static char lbuf[MAX_STRING_LENGTH];
  char *j, *bgn = orig;
  struct char_data *dg_victim = NULL;
  struct obj_data *dg_target = NULL;
  char *dg_arg = NULL;
  bool uppercasenext = FALSE;

  buf = lbuf;

/*  if (ch && !IS_NPC(ch))
    if (to && !IS_NPC(to))
      if (is_ignoring(ch, to))
        return; */

  for (;;) {
    if (*orig == '$') {
      switch (*(++orig)) {
      case 'n':
        if (!to_dreaming)
        {
         i = PERS(ch, to);
        }
        else
        {
         i = PERS_INCLUDING_TAR(ch, to);
        }
    break;
      case 'N':
        if (!to_dreaming) 
        {
     CHECK_NULL(vict_obj, PERS((struct char_data *) vict_obj, to));
        }
        else 
        {
         CHECK_NULL(vict_obj, PERS_INCLUDING_TAR((struct char_data *) vict_obj, to));
        }
        dg_victim = (struct char_data *) vict_obj;
    break;
      case 'm':
    i = HMHR(ch);
    break;
      case 'M':
    CHECK_NULL(vict_obj, HMHR((struct char_data *) vict_obj));
        dg_victim = (struct char_data *) vict_obj;
    break;
      case 's':
    i = HSHR(ch);
    break;
      case 'S':
    CHECK_NULL(vict_obj, HSHR((struct char_data *) vict_obj));
        dg_victim = (struct char_data *) vict_obj;
    break;
      case 'e':
    i = HSSH(ch);
    break;
      case 'E':
    CHECK_NULL(vict_obj, HSSH((struct char_data *) vict_obj));
        dg_victim = (struct char_data *) vict_obj;
    break;
      case 'o':
    CHECK_NULL(obj, OBJN(obj, to));
    break;
      case 'O':
    CHECK_NULL(vict_obj, OBJN((struct obj_data *) vict_obj, to));
        dg_victim = (struct char_data *) vict_obj;
    break;
      case 'p':
    CHECK_NULL(obj, OBJS(obj, to));
    break;
      case 'P':
    CHECK_NULL(vict_obj, OBJS((struct obj_data *) vict_obj, to));
        dg_victim = (struct char_data *) vict_obj;
    break;
      case 'a':
    CHECK_NULL(obj, SANA(obj));
    break;
      case 'A':
    CHECK_NULL(vict_obj, SANA((struct obj_data *) vict_obj));
        dg_victim = (struct char_data *) vict_obj;
    break;
      case 'T':
    CHECK_NULL(vict_obj, (char *) vict_obj);
        dg_victim = (struct char_data *) vict_obj;
    break;
      case 't':
    CHECK_NULL(obj, (char*) obj);
    break;
      case 'F':
    CHECK_NULL(vict_obj, fname((char *) vict_obj));
    break;
      case 'u':
        for (j=buf; j > lbuf && !isspace((int) *(j-1)); j--);
        if (j != buf)
          *j = UPPER(*j);
        i = "";
        break;
      /* uppercase next word */
      case 'U':
        uppercasenext = TRUE;
        i = "";
        break;
      case '$':
    i = "$";
    break;
      default:
        if (!bgn) 
          mlog("SYSERR: Null-string sent to act()!");
        else if (!*bgn || (strlen(bgn) < 2))
          mlog("SYSERR: One-or-Zero-length string sent to act()");
        else 
          alog("SYSERR: Illegal $-code to act(): %s", orig);
    break;
      }
      while ((*buf = *(i++)))
        {
        if (uppercasenext && !isspace((int) *buf))
          {
          *buf = UPPER(*buf);
          uppercasenext = FALSE;
          }
        buf++;
        }
      orig++;
    } else if (!(*(buf++) = *(orig++))) {
      break;
    } else if (uppercasenext && !isspace((int) *(buf-1))){
      *(buf-1) = UPPER(*(buf-1));
      uppercasenext = FALSE;
    }
  }

  *(--buf) = '\r';
  *(++buf) = '\n';
  *(++buf) = '\0';

  if (to->desc) {
    if (!ch && !obj)
      SEND_TO_Q(CAP(lbuf), to->desc);
    else if (ch && (SEE_DREAMING_OK(to, ch) || to_dreaming))
      SEND_TO_Q(CAP(lbuf), to->desc);
    else if (obj && SEE_DREAMING_OK_OBJ(to, obj))
      SEND_TO_Q(CAP(lbuf), to->desc);
  }
  else if (is_retainer(to))
    send_to_retainer_owner(lbuf, to);

  if ((IS_NPC(to) && dg_act_check) && (to != ch))
   act_mtrigger(to, lbuf, ch, dg_victim, obj, dg_target, dg_arg);
}


void act(char *str, int hide_invisible, struct char_data *ch,
     struct obj_data *obj, void *vict_obj, int type)
{
  struct char_data *to = NULL,*list = NULL;
  static int sleep;

  if (!str || !*str) {
    MOBTrigger = TRUE;
    return;
    }

  if (!(dg_act_check = !(type & DG_NO_TRIG)))
   type &= ~DG_NO_TRIG;

  /*
   * Warning: the following TO_SLEEP code is a hack.
   * 
   * I wanted to be able to tell act to deliver a message regardless of sleep
   * without adding an additional argument.  TO_SLEEP is 128 (a single bit
   * high up).  It's ONLY legal to combine TO_SLEEP with one other TO_x
   * command.  It's not legal to combine TO_x's with each other otherwise.
   */

  /* check if TO_SLEEP is there, and remove it if it is. */
  if ((sleep = (type & TO_SLEEP)))
    type &= ~TO_SLEEP;


  if (type == TO_CHAR) {
    if (ch && (SENDOK(ch) || is_retainer(ch))) 
    {
      if (!sleep)
        perform_act(str, ch, obj, vict_obj, ch, 0);
      else 
        perform_act(str, ch, obj, vict_obj, ch, 1);
    }
    MOBTrigger = TRUE;
    return;
  }

  if (type == TO_VICT) {
    if ((to = (struct char_data *) vict_obj) != NULL && 
        (SENDOK(to) || is_retainer(to))) {
     if (!sleep)
      perform_act(str, ch, obj, vict_obj, to, 0);
     else perform_act(str, ch, obj, vict_obj, to, 1);
    }    MOBTrigger = TRUE;
    return;
  }

  if (type == TO_NOTCHAR) {
    for (list = world[ch->in_room].people; list != NULL; list = list->next_in_room) {
      if ((ch != list) && 
        (SENDOK(list) || is_retainer(list)))
        perform_act(str,ch,obj,vict_obj,list,0);
    }
    MOBTrigger = TRUE;
    return;
  }
 
  if (TO_DREAMERS == type) {
    int room = -1;
    if (!ch && !obj) {
      alog("SYSERR: act()[comm.c]: Got null-pointers as ch and obj! (TO_DREAMERS)");
      return;
    } 
    room = (NULL == ch ? obj->in_room : ch->in_room);
    if (NOWHERE == room) {
      alog("SYSERR: act()[comm.c]: Obj/ch not in a room when called. (TO_DREAMERS)");
      return;
    }
    for (list = world[room].people; list != NULL; list = list->next_in_room) {
      if ((ch != list) && 
          (SENDOK(list) || is_retainer(list)) && IS_AFFECTED(list,AFF_DREAMING))
        perform_act(str,ch,obj,vict_obj,list,0);
    }
    MOBTrigger = TRUE;
    return;
  }

  if (type == TO_TPUSER) {
    int room = -1;

    if (!ch && !obj) {      alog("SYSERR: act(), got NULL pointers for both obj and ch (TO_DREAMERS)");
      return;
    }
    room = (NULL == ch ? obj->in_room : ch->in_room);
    if (NOWHERE == room) {
      alog("SYSERR: Obj/ch in Act() is not in a room!");
      return;
    }
    for (list = world[room].people; list != NULL; list = list->next_in_room) {
      if ((SENDOK(list) || is_retainer(list)) && (ch != list) && (list != (struct char_data *) vict_obj) && GET_SKILL(list,233))
        perform_act(str,ch,obj,vict_obj,list,0); /* 233, cause spells.h doesn't seem to workhere! */
    }
    MOBTrigger = TRUE;
    return;
  }

  if (type == TO_OPUSERSS) {
    for (list = world[ch->in_room].people; list != NULL; list = list->next_in_room) {
      if ((SENDOK(list) || is_retainer(list)) && (ch != list) && (list != (struct char_data *) vict_obj) && can_channel_same_sex(ch,list))
        perform_act(str,ch,obj,vict_obj,list,0);
    }
    MOBTrigger = TRUE;
    return;
  }

  if (type == TO_OPUSERNSS) {
    for(list = world[ch->in_room].people; list != NULL; list = list->next_in_room) {
      if ((SENDOK(list)  || is_retainer(list)) && (ch!=list) && (list != (struct char_data *) vict_obj) && can_channel_not_same_sex(ch,list)) 
        perform_act(str,ch,obj,vict_obj,list,0);
    }
    MOBTrigger = TRUE;
    return;
  }

  if (type == TO_NOTCHANN) {
    for(list = world[ch->in_room].people;list != NULL; list = list->next_in_room) {
      if ((SENDOK(list)  || is_retainer(list)) && (ch !=list) && (list != (struct char_data *) vict_obj) && cant_channel(list))
        perform_act(str,ch,obj,vict_obj,list,0);
    }
    MOBTrigger = TRUE;
    return;
  }

  if (type == TO_VICTCSS) {
    if (vict_obj == NULL) return;

    if ((SENDOK((struct char_data *) vict_obj) || 
         is_retainer((struct char_data *) vict_obj)) && 
        can_channel_same_sex(ch,(struct char_data *) vict_obj) && 
       (ch != (struct char_data *) vict_obj))
      perform_act(str,ch,obj,vict_obj,vict_obj,0);
    MOBTrigger = TRUE;
    return;
  }

  if (type == TO_VICTCNS) {
    if (vict_obj == NULL) return;

    if ((SENDOK((struct char_data *) vict_obj)  ||
         is_retainer((struct char_data *) vict_obj))
       && can_channel_not_same_sex(ch,(struct char_data *) vict_obj)
       && (ch!=(struct char_data *) vict_obj))
      perform_act(str,ch,obj,vict_obj,vict_obj,0);
    MOBTrigger = TRUE;
    return;
  }

  if (type == TO_VICTNC) {
    if (vict_obj == NULL) return;

    if ((SENDOK((struct char_data *) vict_obj) ||
         is_retainer((struct char_data *) vict_obj))
       && cant_channel((struct char_data *)vict_obj))
      perform_act(str,ch,obj,vict_obj,vict_obj,0);
    MOBTrigger = TRUE;
    return;
  }
 
  if (type == TO_NOTTPUSER) {
    for (list = world[ch->in_room].people; list != NULL; list = list->next_in_room) {
      if ((SENDOK(list) || is_retainer(list)) && 
          (ch != list) && (list != (struct char_data *) vict_obj) && 
          !GET_SKILL(list,233))
        perform_act(str,ch,obj,vict_obj,list,0);
    }
    MOBTrigger = TRUE;
    return;
  }

  if (type == TO_VICTTP) {
    if (vict_obj == NULL) return;

    if ((SENDOK((struct char_data *) vict_obj)  ||
         is_retainer((struct char_data *) vict_obj)) && 
        (ch != (struct char_data *) vict_obj) && 
       GET_SKILL((struct char_data *)vict_obj,233))
      perform_act(str,ch,obj,vict_obj,vict_obj,0);
    
    MOBTrigger = TRUE;
    return;
  }

  if (type == TO_VICTNOTTP) {
    if (vict_obj == NULL) return;

    if (( ch != (struct char_data *) vict_obj) && 
          !GET_SKILL((struct char_data *)vict_obj,233) &&
          (SENDOK((struct char_data *) vict_obj)  ||
         is_retainer((struct char_data *) vict_obj)))
      perform_act(str,ch,obj,vict_obj,vict_obj,0);

    MOBTrigger = TRUE;
    return;
  }
  if (type == TO_NOTVICTSPAM) {
    for (list = world[ch->in_room].people; list != NULL; list = list->next_in_room) {
      if (SENDOK(list) && (ch != list) && (list != (struct char_data *) vict_obj) && !IS_NPC(list) &&
      !IS_SET(PRF_FLAGS(list), PRF_NOSPAM) && CAN_SEE(list, ch))
        perform_act(str,ch,obj,vict_obj,list,0);
    }
    return;
  }
  /* ASSUMPTION: at this point we know type must be TO_NOTVICT or TO_ROOM */

  if (ch && ch->in_room != NOWHERE) {
   if ((to = world[ch->in_room].people) == NULL) 
      to = ch;
  }
  else if (obj && obj->in_room != NOWHERE) {
   if ((to = world[obj->in_room].people) == NULL) 
     to = ch;
  }
  else {
    to = ch;  /* fix for crashy */
    mlog("SYSERR: no valid target to act()!");
    return;
  }

  for (; to != NULL ; to = to->next_in_room) {
    if (to == ch)
      continue;
    if (!is_retainer(to) && (IS_NPC(to) || !SENDOK(to)))
      continue;
    if (hide_invisible && ch && !CAN_SEE(to, ch))
      continue;
    if (type != TO_ROOM && to == vict_obj)
      continue;
    perform_act(str, ch, obj, vict_obj, to, 0);
  }
  MOBTrigger = TRUE;
}
