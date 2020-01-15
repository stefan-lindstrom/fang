/*
 * $Id: guild.c,v 1.42 2004/01/06 03:38:35 fang Exp $
 *   
 * File: guild.c
 *
 * Created: Sun May 14 2000 by Yohay Etsion (yohay_e@netvision.net.il)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: guild.c,v $
 *   Revision 1.42  2004/01/06 03:38:35  fang
 *   channel tags now on by default when someone is guilded. (really, this time)
 *
 *   Revision 1.41  2003/12/07 09:59:07  fang
 *   Tze: Fixed the sponsorer crash bug. *writes on board 100 times 'I will not create infinite loops'*
 *
 *   Revision 1.40  2003/12/07 08:17:40  fang
 *   Tze: Gchan tags now default to ON when a person is guilded.
 *
 *   Revision 1.39  2003/11/19 04:14:56  fang
 *   Tze: Added bounds checking to guildsetting of gchan_type to fix bug caused by setting gchan_type to invalid value.
 *
 *   Revision 1.38  2003/11/06 06:33:49  fang
 *   Minor fix to addguild.
 *
 *   Revision 1.37  2003/06/16 06:23:40  fang
 *   Ok, Fixed the morts deguilding their GLs thing for real this time. *blush*
 *
 *   Revision 1.36  2003/06/15 05:44:58  fang
 *   typo correction
 *
 *   Revision 1.35  2003/06/15 05:36:14  fang
 *   Morts with perm_guild can no longer deguild their GLs.
 *
 *   Revision 1.34  2003/06/15 05:06:24  fang
 *   fixed xlog error in do_guild and do_deguild
 *
 *   Revision 1.33  2003/06/07 07:30:19  fang
 *   Added gload to xlog.
 *
 *   Revision 1.32  2003/06/06 07:06:28  fang
 *   Disabled gequipsend. Until it is fixed, there's no reason to leave people with access to it.
 *
 *   Revision 1.31  2003/06/05 08:25:49  fang
 *   Fixed (i hope) bug with guild(ie)set xlogs where a syslog message with blanks was sent when the actor received the "Can't set that!" message.
 *
 *   Revision 1.30  2003/06/05 07:15:10  fang
 *   Guildieset xlog is now level 203.
 *
 *   Revision 1.29  2003/06/05 06:53:24  fang
 *   Enabled xlog messages for guildset. Fixed admin level glist to seek and unseek instead of seek and deguild.
 *
 *   Revision 1.28  2003/05/28 06:56:39  fang
 *   Fixed bug with do_sponsor. Can no longer sponsor the same seeker multiple
 *   times.
 *
 *   Revision 1.27  2003/05/21 02:28:41  fang
 *   Fixed the lor/lorimel bug with guild/guildieset -- Tze
 *
 *   Revision 1.26  2003/05/18 09:27:53  fang
 *   Add syslog and xlog for guilding/deguilding
 *
 *   Revision 1.25  2003/05/18 06:55:57  fang
 *   You can no longer sponsor seekers without authorized backgrounds
 *
 *   Revision 1.24  2003/01/22 09:03:56  fang
 *   Fixed 'clubs' command so that it lists both IC and OOC clubs, with respective heading
 *
 *   Revision 1.23  2003/01/21 20:34:29  fang
 *   Removed Misc. guilds from allguilds listing. Added sanity check to setguild so that guild type must be a valid type for setting to be accepted.
 *
 *   Revision 1.22  2002/12/14 05:23:32  fang
 *   fisch: added 'tag' indicator to the 'myguild' list
 *
 *   Revision 1.21  2002/12/12 08:52:08  fang
 *   fisch: returned to oldscore and added myguilds <player> (and file <player>)
 *
 *   Revision 1.20  2002/11/07 04:02:02  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.19  2002/10/12 06:30:15  fang
 *   fisch: converted boards to xml and made them dynamic not coded.
 *
 *   Revision 1.18  2002/10/08 19:09:12  fang
 *   fischer: added optional player name to glist command for single player
 *   lookup
 *
 *   Revision 1.17  2002/10/02 17:35:46  kenshin
 *   gl_title and guildie_title shouldn't be possible to set to any length other
 *   than 11, any more.
 *
 *   Revision 1.16  2002/10/01 22:19:59  fischer
 *   fischer: added a null pointer check before it's referenced in do_deguild
 *
 *   Revision 1.15  2002/09/23 05:52:42  fischer
 *   fisch: clearing out lua triggers part 1
 *
 *   Revision 1.14  2002/09/15 07:17:25  fang
 *   Fisch: Removed or commented out the debug messages used to find the
 *   guild channel bug.
 *
 *   Revision 1.13  2002/09/15 00:50:08  fang
 *   Drath: XLOG update - Categories added to xlog display automatically
 *
 *   Revision 1.12  2002/09/14 16:32:52  fang
 *   fisch: more to take debug code away from the non weaver imms
 *
 *   Revision 1.11  2002/09/14 06:06:12  fang
 *   fisch: minor changes in the xlog code
 *
 *   Revision 1.10  2002/09/14 00:28:03  fang
 *   fisch: cleaned up some debugging code and added others to track that guilie
 *
 *   Revision 1.9  2002/09/13 20:15:05  fang
 *   Fisch: More debug messages surrounding "become" and "guilds"
 *
 *   Revision 1.8  2002/09/13 19:05:39  fang
 *   fisch: Added XLOG DEBUG catagory and set some around guild channel
 *
 *   Revision 1.7  2002/08/27 21:29:39  fang
 *   Drath: Player Extra Descriptions
 *
 *   Revision 1.6  2002/08/16 21:01:42  fang
 *   Drath: Gchan patch?
 *
 *   Revision 1.5  2002/08/13 15:45:55  fang
 *   Drath: gchan fix?
 *
 *   Revision 1.4  2002/08/07 00:38:16  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/08/06 21:24:27  fang
 *   Drath: Reinabled all ignore settings
 *
 *   Revision 1.2  2002/07/28 13:56:04  fang
 *   Removed dmlloc...just to test.
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.17  2002/06/25 19:23:51  fang
 *   Drath: XLOG Commit #2
 *
 *   Revision 1.16  2002/06/03 15:00:17  fang
 *   Mazrim: Removed IGNORE from GuildChannels to see if it's been crashing us.
 *
 *   Revision 1.15  2002/05/31 19:17:56  fang
 *   fixed invis snoop syslog bug
 *
 *   Revision 1.14  2002/05/28 20:22:28  fang
 *   Maz: Small fix for Nose/Ear Stat Removal
 *
 *   Revision 1.13  2002/05/27 10:52:49  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.12  2002/05/24 20:25:29  fang
 *   Maz: REMGUILD check/fix
 *
 *   Revision 1.11  2002/05/12 23:50:46  fang
 *   Added in IGNORE system, and removed AutoHotboots until the cap can be fixed for good. -Maz
 *
 *   Revision 1.10  2002/03/19 02:22:19  fang
 *   *** empty log message ***
 *
 *   Revision 1.9  2002/03/18 11:52:03  fang
 *   *** empty log message ***
 *
 *   Revision 1.8  2002/03/18 08:51:43  fang
 *   Crash bug when doing disguise off after guild disguise foxed
 *
 *   Revision 1.7  2002/03/12 10:46:56  fang
 *   Minor bug fixed in addguild.
 *
 *   Revision 1.6  2002/03/02 03:25:27  fang
 *   *** empty log message ***
 *
 *   Revision 1.5  2002/02/28 13:14:52  fang
 *   *** empty log message ***
 *
 *   Revision 1.4  2002/02/27 06:03:56  fang
 *   Further modifications to disguise saving code.
 *
 *   Revision 1.3  2002/02/27 02:17:40  fang
 *   Fixed disguise so it saves.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.66  2002/02/07 09:50:51  mud
 *   Logging to disguise added per Mas' request :)
 *
 *   Revision 1.65  2002/01/24 21:33:16  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.64.2.2  2002/01/23 21:49:41  mud
 *   Removed objsave.[c|h] (old objsave stuff).
 *   Removed obj_file_elem and rent structs.
 *   Removed final vestiges of old guild system, except parts used
 *   in guild.[c|h] for backwards compatibility.
 *
 *   Revision 1.64.2.1  2002/01/22 20:50:58  mud
 *
 *
 *   Removed boats and info system. Obsolete, unused code with no value.
 *   Fixed bug in portal_stone SPECIAL. Now checks that a room exists
 *   before attempting to transf chars there. :)
 *
 *****************************************************************************/

#include "guild.h"
#include "guild_parser.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "spells.h"
#include "screen.h"
#include "constants.h"
#include "act.h"
#include "boards.h"
#include "xmlcharsave.h"
#include "class.h"

/* Misc utilities */

void free_sponsorer(struct sponsorer_info *s) {
  if (s) {
    if (s->name)
      free(s->name);
    if (s->next)
      free_sponsorer(s->next);
    free(s);
  }
}

void free_qpadd(struct qp_info *q) {
  if (q) {
    if (q->name)
      free(q->name);
    if (q->next)
      free_qpadd(q->next);
    free(q);
  }
}

void free_gequipsent(struct gequip_info *g) {
  if (g) {
    if (g->next)
      free_gequipsent(g->next);
    free(g);
  }
}

void free_guildie_info(struct guildie_info *g) {
  if (g) {
    if (g->name)
      free(g->name);
    if (g->subrank)
      free(g->subrank);
    if (g->sponsorers)
      free_sponsorer(g->sponsorers);
    if (g->qpadd)
      free_qpadd(g->qpadd);
    if (g->gequipsent)
      free_gequipsent(g->gequipsent);
    free(g);
  }
}

const char *guild_types[] =
{
  "Undefined",
  "Organization",
  "Group",
  "Homeland",
  "Dark",
  "Miscellaneous",
  "Subguild",
  "Undefined",
  "Undefined",
  "Undefined",
  "Undefined",
  "Undefined",
  "Undefined",
  "Undefined",
  "IC Club",
  "OOC Club",
  "\n"
};

void save_guild(struct guild_info *g) {  
    FILE *fp = NULL;  

    if (!g) {    
      alog("save_guild:[guild.c]: NULL guild pointer.");    
      return;  
    }  

    sprintf(buf, "%s/%s", GUILD_PREFIX, g->guild_filename);  
    if (!(fp = fopen(buf, "w"))) {    
      alog("save_guild:[guild.c]: Can't open file for writing.");    
      return;  
    }  

    GuildToXml(g, fp);  
    fclose(fp);
}

void save_all_guilds() {  
    struct guild_info *g;  
    FILE *fp = NULL;  

    g = guilds_data;  
    sprintf(buf, "%s/%s", GUILD_PREFIX, INDEX_FILE);  
    if (!(fp = fopen(buf, "w"))) {    
      alog("save_all_guilds:[guild.c]: Can't open file for writing.");    
      return;  
    }  

    while (g) {    
      save_guild(g);
      fprintf(fp, "%s\n", g->guild_filename);    
      g = g->next;  
    }  
        
        fprintf(fp, "$");
    fclose(fp);
}   

void fetch_char_guild_data(struct char_data *ch) 
{
  int r_num, needs_saving = 0;
  struct obj_data *obj;
  struct guild_info *g;
  struct guildie_info *guildie;
  struct char_guild_element *element;
  struct qp_info *qp;
  struct gequip_info *gequip;

  g = guilds_data;

  while (g) 
  {
    guildie = g->guildies;
    while (guildie) 
    {
      if (GET_DISGUISED(ch) 
	&& !strncasecmp(guildie->name, GET_RNAME(ch), strlen(guildie->name)) 
	&& (GET_IDNUM(ch) == guildie->idnum) && GLD_FLAGGED(g, GLD_DISGUISE_STAY))
	break;
      if (!strncasecmp(guildie->name, GET_NAME(ch), strlen(guildie->name)) 
	&& (GET_IDNUM(ch) == guildie->idnum))
	break;
      guildie = guildie->next;
    }

    for(element = GET_CHAR_GUILDS(ch); element; element = element->next)
      if (element->guild->id == g->id)
	guildie = NULL;

    if (guildie) 
    {
      needs_saving = 0;
      guildie->ch = ch;
      element = calloc(1,sizeof(struct char_guild_element));
      element->guild = g;
      element->guildie = guildie;
      element->next = GET_CHAR_GUILDS(ch);
      GET_CHAR_GUILDS(ch) = element;

      /* For qpadd and sent gequip */
      qp = guildie->qpadd;
      gequip = guildie->gequipsent;

      while (qp) 
      {
	GET_QP(ch) += qp->num;
	sprintf(buf, "&G%s has awarded you %ld questpoints while you were offline.&n\r\n", qp->name, qp->num);
	send_to_char(buf, ch);
	guildie->qpadd = qp->next;
	free(qp);
	qp = guildie->qpadd;
	needs_saving = 1;
      }
      if (GET_QP(ch) < 0)
	GET_QP(ch) = 0;

      while (gequip) 
      {
	if ((r_num = real_object(gequip->vnum)) >= 0) 
	{
	  obj = read_object(r_num, REAL);
	  obj_to_char(obj, ch);
	  sprintf(buf, "&RYour GL has sent you %s while you were offline.&n\r\n", obj->short_description);
	  send_to_char(buf, ch);
	}
	guildie->gequipsent = gequip->next;
	free(gequip);
	gequip = guildie->gequipsent;
	needs_saving = 1;
      }

      if (needs_saving)
	save_guild(g);

      /* For old-system support */
      if (g->id < MAXGUILDS) 
      {
	if ((g->id != GLD_DARKFRIEND) 
			&& (g->id != GLD_OGIER) 
			&& (g->id != GLD_WOLFKIN) 
			&& STATUS_FLAGGED(guildie, STATUS_MEMBER)) 
	  if (g->id == GLD_OGIER 
			  && STATUS_FLAGGED(guildie, STATUS_MEMBER))
	    GET_OGIER(ch) = 1;
	if (g->id == GLD_WOLFKIN && STATUS_FLAGGED(guildie, STATUS_MEMBER))
	  GET_WOLFKIN(ch) = 1;
      }
    }
    g = g->next;
  }
}

void remove_char_guild_data(struct char_data *ch) 
{
  struct char_guild_element *element, *e;

  element = GET_CHAR_GUILDS(ch);
  
  while (element) 
  { e = element;
    element->guildie->ch = NULL;
    element = element->next;
    free(e);
  }
  GET_CHAR_GUILDS(ch) = NULL;
}

int get_guild(struct char_data *ch, int num)
{
  struct char_guild_element *element;

  element = GET_CHAR_GUILDS(ch);
  while (element) {
   if (element->guild->id == num && STATUS_FLAGGED(element->guildie, STATUS_MEMBER) 
       && element->guild->id != 3 && element->guild->id != 5) return 1;
   if (element->guild->id == 3 && num == 3 && STATUS_FLAGGED(element->guildie, STATUS_MEMBER) &&
       !STATUS_FLAGGED(element->guildie, STATUS_WARDER))
    return 1;
   if (element->guild->id == 3 && num == 4 && STATUS_FLAGGED(element->guildie, STATUS_MEMBER) &&
       STATUS_FLAGGED(element->guildie, STATUS_WARDER))
    return 1;
   if (element->guild->id == 5 && num == 5 && STATUS_FLAGGED(element->guildie, STATUS_MEMBER) &&
       !STATUS_FLAGGED(element->guildie, STATUS_WARDER))
    return 1;
   if (element->guild->id == 5 && num == 6 && STATUS_FLAGGED(element->guildie, STATUS_MEMBER) &&
       STATUS_FLAGGED(element->guildie, STATUS_WARDER))
    return 1;
   element = element->next;
  }
  return 0;
}

int get_guild_by_name(struct char_data *ch, char *name)
{
  struct char_guild_element *element = NULL;

  if (!ch || !name)
    return 0;

  for (element = GET_CHAR_GUILDS(ch); element; element = element->next)
    if (!strncasecmp(element->guild->name, name, strlen(name)) && 
        STATUS_FLAGGED(element->guildie, STATUS_MEMBER))
      return 1;

  return 0;
}

int get_guild_by_room(struct char_data *ch, int room) {
  struct char_guild_element *element = NULL;
  struct gzone_info *gz = NULL;

  if (!ch || room < 0 || room > top_of_world)
    return 0;

  for (element = GET_CHAR_GUILDS(ch); element; element = element->next)
    for (gz = element->guild->gzones; gz; gz = gz->next)
      if (zone_table[world[room].zone].number == gz->zone)
       if (STATUS_FLAGGED(element->guildie, STATUS_MEMBER))
    return 1;

  return 0;
}

void delete_char_guilds(struct char_data *ch) {
  struct char_guild_element *element, *e;
  struct guildie_info *guildie, *guildie2;

  element = GET_CHAR_GUILDS(ch);
  while (element) {
    guildie = element->guildie;
    if (element->guild->guildies == guildie)
      element->guild->guildies = guildie->next;
    else {
      guildie2 = element->guild->guildies;
      while (guildie2->next != guildie)
        guildie2 = guildie2->next;
      guildie2->next = guildie->next;
    }
    e = element;
    element->guildie->ch = NULL;
    element = element->next;
    save_guild(e->guild);
    free(e);
    free_guildie_info(guildie);
  }
  GET_CHAR_GUILDS(ch) = NULL;
}

/* Notice: this function -doesn't- delete the guilds 
   that carry through disguise, as I wrote it for
   the new disguise scheme. 
*/
void delete_char_all_guilds(struct char_data *ch)
{
  struct char_guild_element *element, *e;
  struct guildie_info *guildie;

  element = GET_CHAR_GUILDS(ch);
  while (element) {
    if (!GLD_FLAGGED(element->guild, GLD_DISGUISE_STAY)) {
      if (element->guild->guildies == element->guildie)
    element->guild->guildies = element->guildie->next;
      else {
        guildie = element->guild->guildies;
        while (guildie->next != element->guildie)
          guildie = guildie->next;
        guildie->next = element->guildie->next;
      }
      e = element;
      element->guildie->ch = NULL;
      if (element->next == NULL && element == GET_CHAR_GUILDS(ch))
    GET_CHAR_GUILDS(ch) = NULL;
      element = element->next;
      free(e);
    }
    else element = element->next;
  }
  /*
   * Need to nulkl this for disguise off, or the char_guilds will point at some
   * highly bad memory :)
   */
  GET_CHAR_GUILDS(ch) = NULL; /* NULL! NULL, I say! :) */
}

void do_guild_channel(struct char_data *ch, struct char_guild_element *element, char *argument)
{
  char buf1[MAX_STRING_LENGTH], buf2[MAX_STRING_LENGTH], buf3[MAX_STRING_LENGTH];
  //struct char_data *tch=NULL;
  struct guildie_info *guildie;
  struct char_data *vict;

  int ign, fnd;
 
  skip_spaces(&argument);
  delete_doubledollar(argument);
  
  if (IS_AFFECTED(ch,AFF_GAGED)) {
    send_to_char("Not while gagged!\r\n",ch);
    return;
  }

  if (PLR_FLAGGED(ch, PLR_NOSHOUT)) {
    send_to_char("You were muted, so you can't use public channels.\r\n", ch);
    return;
  }

  if (PLR_FLAGGED(ch, PLR_FROZEN) && GET_LEVEL(ch) < LVL_IMPL) {
    send_to_char("You try, but the mind-numbing cold prevents you...\r\n", ch);
    return;
  }

  if (!STATUS_FLAGGED(element->guildie, STATUS_CHAN_ON)) {
    send_to_char("You can't use this channel while it's off.\r\n", element->guildie->ch);
    return;
  }

  if (!*argument){
    send_to_char("What do you want to say on this channel?\r\n",ch);
    return;
  }

  if (element->guild->gchan_type == 1) {
    if (!STATUS_FLAGGED(element->guildie, STATUS_SUBRANK)) 
    { sprintf(buf1,"%s %s: &%s%s&n\r\n", 
	((element->guildie->rank_num != 0) ? element->guildie->rank->name : "Unranked"),
        (GLD_FLAGGED(element->guild, GLD_SECRETIVE) ? "Someone" : GET_NAME(ch)),               
        element->guild->gchan_color,
        argument);
      sprintf(buf2,"%s %s: &%s%s&n\r\n", 
        ((element->guildie->rank_num != 0) ? element->guildie->rank->name : "Unranked"),
        GET_NAME(ch),
        element->guild->gchan_color,
        argument);
    }
    else 
    { sprintf(buf1,"%s [%s] %s: &%s%s&n\r\n", 
	((element->guildie->rank_num != 0) ? element->guildie->rank->name : "Unranked"),
        element->guildie->subrank,
        (GLD_FLAGGED(element->guild, GLD_SECRETIVE) ? "Someone" : GET_NAME(ch)), 
        element->guild->gchan_color, argument);
      sprintf(buf2,"%s [%s] %s: &%s%s&n\r\n", 
        ((element->guildie->rank_num != 0) ? element->guildie->rank->name : "Unranked"),
        element->guildie->subrank,
        GET_NAME(ch),
        element->guild->gchan_color, argument); 
    }
  }
  else if (element->guild->gchan_type == 2) 
  { if (!STATUS_FLAGGED(element->guildie, STATUS_SUBRANK)) 
    { sprintf(buf1,"Someone: &%s%s&n\r\n", 
                  element->guild->gchan_color,
                  argument);
      sprintf(buf2,"Someone: &%s%s&n\r\n", 
                  element->guild->gchan_color,
                  argument);
    }
    else 
    { sprintf(buf1,"%s: &%s%s&n\r\n", 
                  element->guildie->subrank,
                  element->guild->gchan_color, argument);
      sprintf(buf2,"%s: &%s%s&n\r\n",
                  element->guildie->subrank,
                  element->guild->gchan_color, argument);
    }
  }
  else if (element->guild->gchan_type == 3) 
  { if (!STATUS_FLAGGED(element->guildie, STATUS_SUBRANK)) 
    { sprintf(buf1,"%s Someone: &%s%s&n\r\n", 
                  ((element->guildie->rank_num != 0) ? element->guildie->rank->name : "Unranked"),
                  element->guild->gchan_color,
                  argument);
      sprintf(buf2,"%s Someone: &%s%s&n\r\n", 
                  ((element->guildie->rank_num != 0) ? element->guildie->rank->name : "Unranked"),
                  element->guild->gchan_color,
                  argument);
    }
    else 
    { sprintf(buf1,"%s %s: &%s%s&n\r\n", 
                  ((element->guildie->rank_num != 0) ? element->guildie->rank->name : "Unranked"),
                  element->guildie->subrank,
                  element->guild->gchan_color, argument);
      sprintf(buf2,"%s %s: &%s%s&n\r\n", 
                  ((element->guildie->rank_num != 0) ? element->guildie->rank->name : "Unranked"),
                  element->guildie->subrank,
                  element->guild->gchan_color, argument);
    }
  } 
  else sprintf(buf1, "%s: &%s%s&n\r\n", GET_NAME(ch), element->guild->gchan_color, argument);

  guildie = element->guild->guildies;
  while (guildie)
  {
    buf3[0] = '\0';

    ign=0;
    fnd=0;

    vict=NULL;
/*    for (vict = character_list; vict; vict=vict->next) 
      if (!IS_NPC(vict))
	if (!str_cmp(GET_NAME(vict), (guildie->name)))
	  if (is_ignoring(ch,vict))
	    ign=1; */

   // tch = get_char_pointer(guildie->name);

    if (STATUS_FLAGGED(guildie, STATUS_TAG) 
	&& guildie->ch 
	&& !PLR_FLAGGED(guildie->ch, PLR_WRITING)
	&& STATUS_FLAGGED(guildie, STATUS_CHAN_ON) 
	&& STATUS_FLAGGED(guildie, STATUS_MEMBER)) 
    { if (!ign) 
      { 
	sprintf(buf3, "[%s] ", element->guild->gchan_name);
	send_to_char(buf3, guildie->ch);
      }
    }
    if (STATUS_FLAGGED(guildie, STATUS_CHAN_ON) 
	&& guildie->ch 
	&& !PLR_FLAGGED(guildie->ch, PLR_WRITING)
	&& STATUS_FLAGGED(guildie, STATUS_MEMBER)) 
    { if (GET_LEVEL(guildie->ch) >= LVL_GRGOD || STATUS_FLAGGED(guildie, STATUS_GL))
      { if (!ign)
	  send_to_char(buf2, guildie->ch);
      } 
      else 
      { if (!ign)     
	  send_to_char(buf1, guildie->ch);
      }
    }
    guildie = guildie->next;
  }
}

void do_guild_gossip(struct char_data *ch, struct char_guild_element *element, char *argument)
{
  char arg[MAX_INPUT_LENGTH+1];

  one_argument(argument, arg);
  
  if (!*arg)
    page_string(ch->desc, element->guild->gossip, 1);
  else if (is_abbrev(arg, "write")) {
    if (!PERM_FLAGGED(element->guildie, PERM_GOSSIP_WRITE)) {
     send_to_char("You have no permission to do so.\r\n", ch);
     return;
    }
       send_to_char("Normal editing options apply. Use 'saveguild' afterwards.\r\n", ch);
       send_to_char("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n", ch);
       ch->desc->str = &(element->guild->gossip);
       ch->desc->max_str = MAX_MESSAGE_LENGTH;
       ch->desc->backstr = str_dup(element->guild->gossip);
       ch->desc->current_text_line = count_newlines(ch->desc->str) + 1;
       SET_BIT(PLR_FLAGS(ch), PLR_WRITING); 
  }
}

void do_guild_walk(struct char_data *ch, struct char_guild_element *element)
{

  if (!GET_POS(ch) == POS_FIGHTING)
  {
    send_to_char("Oh no you don't! Not while you're fighting.\r\n", ch);
    return;
  }

  if(IS_AFFECTED2(ch, AFF2_SUBDUED))
    {
    send_to_char("You can't guildwalk while being a captive!\n\r",ch);
    return;
    }


  if (ROOM_FLAGGED(ch->in_room, ROOM_NORECALL))
  {
    send_to_char("You can't guildwalk from here!\r\n", ch);
    return;
  }

  if (GET_LEVEL(ch) > 40 && IS_SET(PRF_FLAGS(ch),PRF_IC))
  {
    send_to_char("Sorry, guildwalking is only allowed up to level 40 and OOC people. Have a nice walk.\r\n",
ch);
    return;
  }

  if (ch->player_specials->linking[0])
  {
    send_to_char("You must dissolve your link first!\r\n", ch);
    return;
  }

  if (GLD_FLAGGED(element->guild, GLD_NOGUILDWALK)) {
   send_to_char("This guild has no guildwalk room.\r\n", ch);
   return;
  }

  if(element->guild->guildwalk_room == 0) { 
    send_to_char("No guildwalk room set. Contact your GL.\r\n", ch);
    return;
  }

  act("$n departs for $s homeland!",TRUE,ch,0,0,TO_ROOM);
  TICKS_IN_ROOM(ch)=0; 

  
  if ((GET_MOVE(ch)*0.2) > (GET_MOVE(ch) - 300))
    GET_MOVE(ch) = 0.2*GET_MOVE(ch);
  else GET_MOVE(ch) -= 300;
  char_from_room(ch);
  char_to_room(ch,real_room(element->guild->guildwalk_room));
  act("$n arrives from some distant land, weary and tired.",TRUE,ch,0,0,TO_ROOM);
  send_to_char("You have returned to your home, feeling deeply tired.\r\n",ch);
  do_look(ch,"\0",0,SCMD_LOOK);
    
}

int is_dark(struct char_data *ch)
{
  struct char_guild_element *element;
  
  element = GET_CHAR_GUILDS(ch);

  while (element) {
   if (GLD_FLAGGED(element->guild, GLD_DARK)) 
    return 1;
   element = element->next;
  }

  return 0;
}

int can_use_gskill(struct char_data *ch, int skill)
{
  struct char_guild_element *element;
  struct gskill_info *gskill;

  if (GET_LEVEL(ch) >= LVL_IMMORT)
   return 1;

  element = GET_CHAR_GUILDS(ch);

  while (element) {
   gskill = element->guild->gskills;
   while (gskill) {
    if (gskill->skill == skill)
     return 1;
    gskill = gskill->next;
   }
   element = element->next;
  }

  return 0;
}

/* ACMDs */

ACMD(do_addguild) 
{	

 struct guild_info *g;	
 int newid;	

 skip_spaces(&argument);
 one_argument(argument, arg);

 if (!*arg) {
   send_to_char("Usage: addguild <guildname>\r\n",ch);
   return;
 }

 g = guilds_data;    
 if (g) {	 

  newid = -1;
  while (g) {
   if (g->id > newid)
    newid = g->id;
   g = g->next;
  }
  newid++;

  g = guilds_data;
  while (g->next)      
   g = g->next;	 
   
  g->next = calloc(1, sizeof(struct guild_info));     
  g = g->next;	
 }    
 else {     
  guilds_data = calloc(1, sizeof(struct guild_info));     
  g = guilds_data;     
  newid = 1;	
 }    

 g->name = str_dup(arg);	
 g->description = str_dup("No description yet.\r\n");	
 g->requirements = str_dup("No requirements set yet.\r\n");
 g->gossip = str_dup("No gossip yet.\r\n");
 g->type = 0;	
 g->id = newid;	
 g->guild_filename = str_dup(arg);
 g->gossip_name = NULL;
 g->gl_title = NULL;
 g->guildie_titles = NULL;	
 g->gchan_name = str_dup(arg);
 g->gchan_color = str_dup("R");
 g->gchan_type = 1;

 sprintf(buf, "&B%s guild added.&n\r\n", g->name);
 send_to_char(buf, ch);

 save_all_guilds();
}

ACMD(do_newremguild)
{
  struct guild_info *g;
  int id;

  g = guilds_data;
    
  argument = one_argument(argument, arg);
  
  if (!*arg) {
    send_to_char("Which guild fo you want to remove ? Use 'remguild <guild number> yes remove guild'.\r\n", ch);
    return;
  }
  
  if (strcasecmp(argument, " yes remove guild")) {
    send_to_char("We don't want any accidents here.. Use 'remguild <num> yes remove guild'.\r\n", ch);
    return;
  }

  id = atoi(arg);

  while (g) {
    if (g->id == id)
         break;
    if (!strncasecmp(g->name, arg, strlen(g->name)))
         break;
    g = g->next;
  }

  if (!g) {
    send_to_char("No such guild !\r\n", ch);
    return;
  } else {
    sprintf(buf, "Found guild #%d, %s.\r\n", id, (g->name));
    send_to_char(buf, ch);
    g->guild_filename = NULL;

    save_all_guilds();
    return;
  }

}

ACMD(do_remguild)
{
  struct guild_info *g, *temp;
  struct descriptor_data *d;
  struct char_guild_element *temp1 = NULL, *temp2 = NULL;
  int id;

  g = guilds_data;
    
  argument = one_argument(argument, arg);
  
  if (!*arg) {
    send_to_char("Which guild fo you want to remove ? Use 'remguild <guild number> yes remove guild'.\r\n", ch);
    return;
  }
  
  if (strcasecmp(argument, " yes remove guild")) {
    send_to_char("We don't want any accidents here.. Use 'remguild <num> yes remove guild'.\r\n", ch);
    return;
  }

  id = atoi(arg);

  while (g) {
    if (g->id == id)
         break;
    if (!strncasecmp(g->name, arg, strlen(g->name)))
         break;
    g = g->next;
  }

  if (!g) {
    send_to_char("No such guild !\r\n", ch);
    return;
  }

  for (d = descriptor_list; d; d = d->next)
    if (!d->connected) {
     temp1 = GET_CHAR_GUILDS(d->character);
     if (temp1->guild == g) {
       GET_CHAR_GUILDS(d->character) = temp1->next;
     } else {
       temp2 = temp1;
       temp1 = temp1->next;
       while (temp1) {
             if ((temp1->guild) != NULL) {
            if (temp1->guild == g) {
          temp2->next = temp1->next;
          break;
        }
        temp2 = temp1;
        temp1 = temp1->next;
             }
       }
     }
    }
    
    if (temp1) free(temp1);

    if (g == guilds_data) {
     free(guilds_data);
     return;
    }

    temp = guilds_data;
    while (temp->next != g)
     temp = temp->next;

    temp->next = g->next;

    sprintf(buf, "&B%s guild removed.&n\r\n", g->name);
    send_to_char(buf, ch);
    free(g);

    save_all_guilds();
}

ACMD(do_allguilds)
{	
  struct guild_info *g;
  int i, num;

  *buf = '\0';
  for (i = 1; i <= MAX_GUILD_TYPES; i++) {
   /*
    * ugly, but nescessary to be able to skip listing opf Misc guilds
    * and remain 'compatible' with the typesystem
    */
   if (!strcasecmp(guild_types[i],"Miscellaneous")) 
     continue;

   g = guilds_data;	 
   sprintf(buf, "%s\r\n      &c%s list&n\r\n", buf, guild_types[i]);	 
   sprintf(buf, "%s      &R=====", buf);

   for (num = 1; num <= strlen(guild_types[i]); num++)
     sprintf(buf, "%s=", buf);

   sprintf(buf, "%s&n\r\n\r\n", buf);	 
   while (g) {
     if (GLD_TYPE(g) == i)		  
      sprintf(buf, "%s%s  [%d] [%s]\r\n", buf, g->name, g->id,                   
              (GLD_FLAGGED(g, GLD_IC) ? "IC" : "OOC"));
      g = g->next;	 
    } /* while(g) */
  } /* for (i = ... */

  sprintf(buf, "%s\r\n\r\n", buf);
  page_string(ch->desc,buf,1);

}

ACMD(do_clubs)
{
    struct guild_info *g;
    char *h1 = "IC Clubs list", *h2 = "OOC Clubs list";
    register int i;
    g = guilds_data;

    sprintf(buf, "\r\n      &c%s&n\r\n",h1);
    sprintf(buf, "%s      &R",buf);
    for (i = 0; i < strlen(h1); i++) 
      sprintf(buf,"%s%c",buf,'=');
    sprintf(buf,"%s&n\r\n\r\n", buf);

    while (g) {
      if (GLD_TYPE(g) == GUILD_IC_CLUB)
        sprintf(buf, "%s%s  [%d]\r\n", buf, g->name, g->id);

      g = g->next;
    }

    g = guilds_data;
    
    sprintf(buf, "%s\r\n      &c%s&n\r\n",buf,h2);         
    sprintf(buf, "%s      &R",buf);
    for (i = 0; i < strlen(h2); i++) sprintf(buf,"%s%c",buf,'=');
    sprintf(buf,"%s&n\r\n\r\n", buf);
    
    while (g) {
      if (GLD_TYPE(g) == GUILD_OOC_CLUB)
        sprintf(buf, "%s%s  [%d]\r\n", buf, g->name, g->id);
     
      g = g->next;
    }


    sprintf(buf, "%s\r\n\r\n", buf);
    page_string(ch->desc,buf,1);
}


void print_myguilds( struct char_data *ch, struct char_data *tch)
{
  struct char_guild_element *element;
  struct guild_info *g;
  struct guildie_info *guildie;
  int bool = 0;

  element = GET_CHAR_GUILDS(tch);

  sprintf(buf, "\r\n      &cGuild list&n\r\n");
  sprintf(buf, "%s      &R==========&n\r\n", buf);

  while (element) 
  {
    g = element->guild;
    guildie = element->guildie;

    sprintf(buf, "%s\r\n%s  [%s] [%d] [%s]", 
            buf, 
            g->name, 
            guild_types[g->type], 
            g->id,
            (GLD_FLAGGED(g, GLD_IC) ? "IC" : "OOC")
    );

    if( STATUS_FLAGGED(guildie, STATUS_SEEKING) ) 
      sprintf(buf, "%s &r-Seeking-&n", buf);

    if (STATUS_FLAGGED(guildie, STATUS_AUTHORIZED))
      sprintf(buf, "%s &G-BG approved-&n", buf);

    if (STATUS_FLAGGED(guildie, STATUS_GL)) 
        sprintf(buf, "%s [&rGL&n]", buf);

    if (STATUS_FLAGGED(guildie, STATUS_MEMBER))
    {
      sprintf(buf, "%s [gchan: &%s%s&n (%s", 
              buf, 
              g->gchan_color, 
              g->gchan_name,
              (STATUS_FLAGGED(guildie, STATUS_CHAN_ON) ? "on" : "off")
      );
      if( STATUS_FLAGGED( guildie, STATUS_TAG ) )
        sprintf( buf, "%s,tag)]", buf );
      else
        sprintf( buf, "%s)]", buf );
    }

        if (guildie->perm && !STATUS_FLAGGED(guildie, STATUS_GL)) {
         sprintf(buf, "%s [&R ", buf);
         if (PERM_FLAGGED(guildie, PERM_GUILD))
      sprintf(buf, "%sguild ", buf);
         if (PERM_FLAGGED(guildie, PERM_WITHDRAW))
          sprintf(buf, "%swithdraw ", buf);
         if (PERM_FLAGGED(guildie, PERM_GOSSIP_WRITE))
          sprintf(buf, "%swrite-gossip ", buf);
         if (PERM_FLAGGED(guildie, PERM_SPONSOR))
          sprintf(buf, "%ssponsor ", buf);
         if (PERM_FLAGGED(guildie, PERM_AUTHORIZE))
          sprintf(buf, "%sauthorize ", buf);
         if (PERM_FLAGGED(guildie, PERM_GSKILLSET))
          sprintf(buf, "%sgskillset ", buf);
         sprintf(buf, "%s&n]", buf);
        }
  
        sprintf(buf, "%s\r\n     ", buf);

        if (guildie->rank_num != 0)
    {
     sprintf(buf, "%s [Rank: %s (%ld)]", buf, guildie->rank->name, guildie->rank_num);
     bool = 1;
    }

    if (STATUS_FLAGGED(guildie, STATUS_SUBRANK)) 
    {
     sprintf(buf, "%s [Subrank: %s]", buf, guildie->subrank);
         bool = 1;
    }

    if (guildie->deposited || guildie->withdrew) 
    { 
     sprintf(buf, "%s [dep:%ld with:%ld]", buf, guildie->deposited, guildie->withdrew);
     bool = 1;
    }

    if (bool == 1)
     sprintf(buf, "%s\r\n", buf);

    element = element->next;
  }
 
  sprintf(buf, "%s\r\n\r\n", buf);
  page_string(ch->desc,buf,1);
}

ACMD(do_myguilds)
{
  struct char_data *vict = NULL;

  half_chop( argument, buf1, buf2 );

  if( !*buf1 )
  {
    print_myguilds( ch, ch );
    return;
  }
  else if ( is_abbrev( buf1, "file" ) )
  {
    if (GET_LEVEL(ch) > LVL_IMMORT)
    {
      if (NULL != (vict = xmlload_char(buf2)))
      {
        fetch_char_guild_data(vict);
        if (GET_LEVEL(ch) < GET_LEVEL(vict))
          send_to_char("You can't do that.\r\n", ch);
        else
        {
          GET_POS(vict) = POS_STANDING;
          print_myguilds(ch, vict);
        }
        remove_char_guild_data(vict);
      }
      else
        send_to_char("No such player found.\r\n", ch);
    } else
      print_myguilds(ch, ch);
  }
  else
  {
    if (GET_LEVEL(ch) > LVL_IMMORT && (vict=get_char_vis(ch,buf1)) )
    {
      if (GET_LEVEL(ch) < GET_LEVEL(get_char_vis(ch, buf1)))
        send_to_char("You can't do that.\r\n", ch);
      else
        print_myguilds(ch, vict);

      return;
    }
    if (GET_LEVEL(ch) > LVL_IMMORT && !(vict=get_char_vis(ch,buf1)) )
    {
      send_to_char("No such person around.\r\n", ch);
      return;
    }
    print_myguilds(ch, ch);
  }
}

ACMD(do_granks)
{
 struct guild_info *g;
 struct rank_info *rank;
 int num;

 one_argument(argument, arg);

 if (!*arg) {
  send_to_char("Ranks of which guild ? Use 'ranks <guild name/num>'.\r\n", ch);
  return;
 }

 num = atoi(arg);
 g = guilds_data;

 while (g) {
  if (g->id == num) break;
  if (!strncasecmp(g->name, arg, strlen(arg))) break;
  g = g->next;
 }

 if (!g) {
  send_to_char("No such guild.\r\n", ch);
  return;
 }

 rank = g->ranks;
 
 sprintf(buf, "\r\n    &c%s rank list\r\n", g->name);
 sprintf(buf, "%s    &R==========", buf);

 for (num = 1; num <= strlen(g->name); num++)
  sprintf(buf, "%s=", buf);

 sprintf(buf, "%s&n\r\n\r\n", buf);
 while (rank) {
  sprintf(buf, "%s%s [rank num: %ld]\r\n", buf, rank->name, rank->num);
  rank = rank->next;
 }
  
 page_string(ch->desc,buf,1);

}

ACMD(do_guildinfo)
{
    struct guild_info *g = NULL;
    struct guildie_info *guildie;
    struct char_guild_element *element;
    int num;

    one_argument(argument, arg);

    if (!*arg) {
    element = GET_CHAR_GUILDS(ch);
    while (element) {
     g = element->guild;
     if (GLD_FLAGGED(g, GLD_EXCLUSIVE))
      break;
     element = element->next;
        }

        if (!element) {
          send_to_char("Guild information on which guild ? Use 'guildinfo <guild number/name>'.\r\n", ch);
      return;
    }
     }

     if (*arg) {

     g = guilds_data;
     num = atoi(arg);

     while (g) {
        if (g->id == num) break;
        if (!strncasecmp(g->name, arg, strlen(arg))) break;
        g = g->next;
     }

     if (!g) {
        send_to_char("No such guild !\r\n", ch);
        return;
     }
      }

        guildie = g->guildies;

    sprintf(buf, "\r\n    &c%s information\r\n", g->name);
    sprintf(buf, "%s    &R============", buf);

    for (num = 1; num <= strlen(g->name); num++)
        sprintf(buf, "%s=", buf);

    sprintf(buf, "%s&n\r\n\r\n", buf);

    sprintf(buf, "%sNumber: %d, Type: %s/%s, GL title: %s, Guildies titles: %s\r\n",
         buf, g->id, 
        guild_types[g->type],
        (GLD_FLAGGED(g, GLD_IC) ? "IC" : "OOC"), 
        (GLD_FLAGGED(g, GLD_GLTITLE) ? g->gl_title : "None"),
        (GLD_FLAGGED(g, GLD_GUILDIESTITLE) ? g->guildie_titles : "None"));

    sprintf(buf, "%sGuild channel: %s, Exclusive: %s\r\n", buf, g->gchan_name,
        (GLD_FLAGGED(g, GLD_EXCLUSIVE) ? "&GYes&n" : "&RNo&n"));

        sprintf(buf, "%sGuild gossip: %s\r\n", buf,
                (GLD_FLAGGED(g, GLD_GOSSIP) ? g->gossip_name : "None"));

    sprintf(buf, "%sGuildleaders: ", buf);

    while (guildie) {
        if (STATUS_FLAGGED(guildie, STATUS_GL)) 
          sprintf(buf, "%s%s ", buf, guildie->name);
        guildie = guildie->next;
    }

    sprintf(buf, "%s\r\n\r\n", buf);
    sprintf(buf, "%sDescription:\r\n\r\n", buf);
    sprintf(buf, "%s%s\r\n\r\n", buf, g->description);
    sprintf(buf, "%sRequirements:\r\n\r\n", buf);
    sprintf(buf, "%s%s\r\n\r\n", buf, g->requirements);
    
    page_string(ch->desc,buf,1);
}

ACMD(do_gadmininfo)
{
 struct guild_info *g = NULL;
 struct guildie_info *guildie;
 struct gskill_info *gskill;
 struct gequip_info *gequip;
 struct gzone_info *gzone;
 struct rank_info *rank;
 struct char_guild_element *element;
 int num, i, rnum;
 struct obj_data *obj;

 one_argument(argument, arg);

 if (!*arg) {

  element = GET_CHAR_GUILDS(ch);
  while (element) {
   guildie = element->guildie;
   if (STATUS_FLAGGED(guildie, STATUS_GL)) break;
   element = element->next;
  }

  if (!element) {
   send_to_char("Admin information on which guild ? Use 'gadmininfo <guild number/name>'.\r\n", ch);
   return;
  }

  g = element->guild;
 }

 if (*arg) {

  num = atoi(arg);
  g = guilds_data;

  while (g) {
   if (g->id == num) break;
   if (!strncasecmp(g->name, arg, strlen(arg))) break;
   g = g->next;
  }

  if (!g) {
   send_to_char("No such guild.\r\n", ch);
   return;
  }

  if (GET_LEVEL(ch) < LVL_GRGOD) {
   element = GET_CHAR_GUILDS(ch);
   while (element) {
    if (element->guild == g) break;
    element = element->next;
   }
   
   if (!element) {
    send_to_char("You are not a part of this guild.\r\n", ch);
    return;
   }

   if (!STATUS_FLAGGED(element->guildie, STATUS_GL)) {
    send_to_char("Are you a guildleader ?!\r\n", ch);
    return;
   }
  }
 }

 rank = g->ranks;
 gskill = g->gskills;
 gequip = g->gequip;
 gzone = g->gzones;
 
 sprintf(buf, "\r\n    &c%s admin information\r\n", g->name);
 sprintf(buf, "%s    &R==================", buf);

 for (num = 1; num <= strlen(g->name); num++)
  sprintf(buf, "%s=", buf);

 sprintf(buf, "%s&n\r\n\r\n", buf);
 sprintf(buf, "%sGuild filename: %s\r\n", buf, g->guild_filename);
 if (!GLD_FLAGGED(g, GLD_NOGCHAN))
  sprintf(buf, "%sChannel color: %s, Channel type: %d\r\n", buf, g->gchan_color, g->gchan_type);
 if (!GLD_FLAGGED(g, GLD_NOGUILDWALK))
  sprintf(buf, "%sGuildwalk room: %d\r\n", buf, g->guildwalk_room);  
 if (!GLD_FLAGGED(g, GLD_NOBANK))
  sprintf(buf, "%sMoney in bank account: %d\r\n", buf, g->gold); 

 sprintf(buf, "%s\r\n", buf);
 if (g->gflags) {
  sprintf(buf, "%s&BGuild flags:&n", buf);
  if (GLD_FLAGGED(g, GLD_EXCLUSIVE))
   sprintf(buf, "%s Exclusive", buf);
  if (GLD_FLAGGED(g, GLD_IC))
   sprintf(buf, "%s IC", buf);
  if (GLD_FLAGGED(g, GLD_GLTITLE))
   sprintf(buf, "%s GLtitle", buf);
  if (GLD_FLAGGED(g, GLD_GUILDIESTITLE))
   sprintf(buf, "%s Guildiestitle", buf);
  if (GLD_FLAGGED(g, GLD_GOSSIP))
   sprintf(buf, "%s Gossip", buf);
  if (GLD_FLAGGED(g, GLD_NOGCHAN))
   sprintf(buf, "%s Nogchan", buf);
  if (GLD_FLAGGED(g, GLD_SECRETIVE))
   sprintf(buf, "%s Secretive", buf);
  if (GLD_FLAGGED(g, GLD_NOGLIST))
   sprintf(buf, "%s Noglist", buf);
  if (GLD_FLAGGED(g, GLD_NOGWHO))
   sprintf(buf, "%s Nogwho", buf);
  if (GLD_FLAGGED(g, GLD_NOGUILDWALK))
   sprintf(buf, "%s Noguildwalk", buf);
  if (GLD_FLAGGED(g, GLD_NOBANK))
   sprintf(buf, "%s Nobank", buf);
  if (GLD_FLAGGED(g, GLD_DISGUISE_STAY))
   sprintf(buf, "%s Disguise-stay", buf);
  sprintf(buf, "%s\r\n\r\n", buf);
 }

 if (rank) { 
  sprintf(buf, "%s&RRanks:&n\r\n\r\n", buf);
  while (rank) {
   sprintf(buf, "%s%s [rank num: %ld]\r\n", buf, rank->name, rank->num);
   rank = rank->next;
  }
  sprintf(buf, "%s\r\n\r\n", buf);
 }
    
 if (gskill) { 
  sprintf(buf, "%s&RGskills:&n\r\n\r\n", buf);
  while (gskill) {
   sprintf(buf, "%s%s [skillnum: %d] [maximum set: %d]\r\n", buf, spells[gskill->skill],
           gskill->skill, gskill->maximum_set);
   gskill = gskill->next;
  }
  sprintf(buf, "%s\r\n\r\n", buf);
 }

 if (gequip) { 
  sprintf(buf, "%s&RGequipment:&n\r\n\r\n", buf);
  while (gequip) {
   if ((rnum = real_object(gequip->vnum)) < 0) {
    send_to_char("There is no object with that number.\r\n", ch);
    return;
   }
   obj = read_object(rnum, REAL);
   sprintf(buf, "%s%s [vnum: %d]\r\n", buf, obj->short_description, gequip->vnum);
   gequip = gequip->next;
  }
  sprintf(buf, "%s\r\n\r\n", buf);
 }

 if (gzone) { 
  sprintf(buf, "%s&RGzones:&n\r\n\r\n", buf);
  while (gzone) {
   for (i = 0; zone_table[i].number != gzone->zone && i <= top_of_zone_table; i++);   
   if (i <= top_of_zone_table)
    sprintf(buf, "%s%s [zone num: %d]\r\n", buf, zone_table[i].name, gzone->zone); 
   else sprintf(buf, "%sUndefined [zone num: %d]\r\n", buf, gzone->zone);
   gzone = gzone->next;
  }
  sprintf(buf, "%s\r\n\r\n", buf);
 }

 page_string(ch->desc,buf,1);

}

ACMD(do_guildieinfo) 
{
  char guild_name[MAX_INPUT_LENGTH], guildie_name[MAX_INPUT_LENGTH];
  int guild_id, num, bool=0;
  struct guild_info *g, *g2;
  struct char_guild_element *element;
  struct guildie_info *guildie, *guildie2;
  struct gequip_info *gequip;
  struct qp_info *qp;
  struct char_data *chdata;

  two_arguments(argument, guild_name, guildie_name);

  if (!*guild_name || !*guildie_name) {
    send_to_char("Usage: guildieinfo <guild number/name> <guildie name>\r\n", ch);
    return;
  }

  guild_id = atoi(guild_name);

  g = guilds_data;

  while (g) {
   if (g->id == guild_id) break;
   if (!strncasecmp(g->name, guild_name, strlen(guild_name))) break; 
   g = g->next;
  }

  if (!g) {
   send_to_char("No such guild.\r\n", ch);
   return;
  }

  if (GET_LEVEL(ch) < LVL_GRGOD) {
   
   element = GET_CHAR_GUILDS(ch);
   while (element) {
    if (element->guild == g) break;
    element = element->next;
   }
      
   if (!element) {
    send_to_char("You are not a part of this guild.\r\n", ch);
    return;
   }

   if (!STATUS_FLAGGED(element->guildie, STATUS_GL)) {
    send_to_char("You are not the guildleader of this guild.\r\n", ch);
    return;
   }
    
  }
  
  guildie = g->guildies;

  while (guildie) {
   if (!strcasecmp(guildie->name, guildie_name)) break;
   guildie = guildie->next;
  }

  if (!guildie) {
   send_to_char("No such person in the guild.\r\n", ch);
   return;
  }

  sprintf(buf, "\r\n    &c%s guildie information\r\n", guildie->name);
  sprintf(buf, "%s    &R====================", buf);

  for (num = 1; num <= strlen(guildie->name); num++)
   sprintf(buf, "%s=", buf);

  sprintf(buf, "%s&n\r\n", buf);

  g2 = guilds_data;

  while (g2) {
   guildie2 = g2->guildies;
   while (guildie2) {
    if (guildie2->idnum == guildie->idnum) {

     sprintf(buf, "%s\r\n%s  [%s] [%d] [%s]", buf, g2->name, 
             guild_types[g2->type], g2->id,
         (GLD_FLAGGED(g2, GLD_IC) ? "IC" : "OOC"));
     if (STATUS_FLAGGED(guildie2, STATUS_SEEKING)) 
    sprintf(buf, "%s &r-Seeking-&n", buf);

     if (STATUS_FLAGGED(guildie2, STATUS_AUTHORIZED))
        sprintf(buf, "%s &G-BG approved-&n", buf);

     if (STATUS_FLAGGED(guildie2, STATUS_GL)) 
    sprintf(buf, "%s [&rGL&n]", buf);

     if (STATUS_FLAGGED(guildie2, STATUS_CHAN_ON))
        sprintf(buf, "%s [gchan on]", buf);

     if (g == g2  || GET_LEVEL(ch) >= LVL_GRGOD) {
      if (guildie2->rank_num != 0) {
       sprintf(buf, "%s\r\n     [Rank: %s (%ld)]", buf, guildie2->rank->name, guildie2->rank_num);
       bool = 1;
      }	 
      if (STATUS_FLAGGED(guildie2, STATUS_SUBRANK)) {
       if (!bool)
        sprintf(buf, "%s\r\n    ", buf);
       sprintf(buf, "%s [Subrank: %s]", buf, guildie2->subrank);
      }
     }

     if ((guildie2->perm && g == g2 && !STATUS_FLAGGED(guildie2, STATUS_GL)) ||
     (guildie2->perm && !STATUS_FLAGGED(guildie2, STATUS_GL) && GET_LEVEL(ch) >= LVL_GRGOD)) {
         sprintf(buf, "%s\r\n     [&R ", buf);

         if (PERM_FLAGGED(guildie2, PERM_GUILD))
        sprintf(buf, "%sguild ", buf);
         if (PERM_FLAGGED(guildie2, PERM_WITHDRAW))
          sprintf(buf, "%swithdraw ", buf);
         if (PERM_FLAGGED(guildie2, PERM_GOSSIP_WRITE))
          sprintf(buf, "%swrite-gossip ", buf);
         if (PERM_FLAGGED(guildie2, PERM_SPONSOR))
          sprintf(buf, "%ssponsor ", buf);
         if (PERM_FLAGGED(guildie2, PERM_AUTHORIZE))
          sprintf(buf, "%sauthorize ", buf);
         if (PERM_FLAGGED(guildie2, PERM_GSKILLSET))
          sprintf(buf, "%sgskillset ", buf);
         sprintf(buf, "%s&n]", buf);
      }
        
      if ((guildie2->deposited || guildie2->withdrew) && 
     (g == g2 || GET_LEVEL(ch) >= LVL_GRGOD))
       sprintf(buf, "%s\r\n [dep:%ld with:%ld]", buf, guildie2->deposited, guildie2->withdrew);

      if (guildie2->qpadd && (g == g2 || GET_LEVEL(ch) >= LVL_GRGOD)) {
       sprintf(buf, "%s\r\n     &GQP awards:&n ", buf);
       qp = guildie2->qpadd;
       while (qp) {
        sprintf(buf, "%s&R%ld(%s)&n ", buf, qp->num, qp->name);
        qp = qp->next;
       }
      }

      if (guildie2->gequipsent && (g == g2 || GET_LEVEL(ch) >= LVL_GRGOD)) {
       sprintf(buf, "%s\r\n     &GGequipment sent:&n ", buf);
       gequip = guildie2->gequipsent;
       while (gequip) {
        sprintf(buf, "%s&R%d&n ", buf, gequip->vnum);
        gequip = gequip->next;
       }
      }

     }
     guildie2 = guildie2->next;
   }
 //  sprintf(buf, "%s\r\n", buf);
   g2 = g2->next;
  }

  if (NULL != (chdata = xmlload_char(guildie->name))) {
     sprintf(buf, "%s\r\n[Level: %2d] [Class: %s] Last logon: %s\r\n", buf,
         (int)GET_LEVEL(chdata), class_abbrevs[(int)GET_CLASS(chdata)],ctime(&chdata->player.time.logon));
     free_char(chdata);
  }

  sprintf(buf, "%s\r\n", buf);
  page_string(ch->desc,buf,1);
}

#define MISC	0
#define BINARY	1
#define NUMBER	2

#define SET_OR_REMOVE(flagset, flags) { \
        if (on) SET_BIT(flagset, flags); \
        else if (off) REMOVE_BIT(flagset, flags); }

ACMD(do_guildset)
{
  int l;
  struct guild_info *g;
  char guild_num[MAX_INPUT_LENGTH];
  char field[MAX_INPUT_LENGTH], val_arg[MAX_INPUT_LENGTH], to_log[MAX_STRING_LENGTH];
  int guild_id, rank_num, gskill_num, on = 0, off = 0, value = 0, do_log = 1;
  char rank_name[MAX_INPUT_LENGTH], gskill_name[MAX_INPUT_LENGTH], to_char[MAX_INPUT_LENGTH];
  struct char_guild_element *element;
  struct gequip_info *gequip;
  struct gskill_info *gskill;
  struct gzone_info *gzone;
  struct rank_info *rank, *r, *r2;
  struct guildie_info *guildie;

  struct set_struct {
    char *cmd;
    int level;
    char type;
  }          fields[] = {
   { "gl_title", 			LVL_GOD, 	MISC },    /* 0 */
   { "guildie_titles",			LVL_GOD, 	MISC },
   { "gossip_name",			LVL_GRGOD,	MISC },
   { "guild_filename", 	 		LVL_GRGOD, 	MISC },
   { "id",				LVL_GRGOD, 	NUMBER },
   { "name",				LVL_GRGOD, 	MISC },    /* 5 */
   { "type",				LVL_GRGOD, 	NUMBER },
   { "f_exclusive", 			LVL_GOD, 	BINARY },
   { "f_ic",				LVL_GOD, 	BINARY },
   { "f_gltitle",			LVL_GOD, 	BINARY },
   { "f_guildiestitle",			LVL_GRGOD, 	BINARY },  /* 10 */
   { "f_gossip",			LVL_GOD, 	BINARY },
   { "f_nogchan",			LVL_GOD, 	BINARY },
   { "f_secretive", 			1,		BINARY },
   { "f_noglist", 			1, 		BINARY },
   { "f_nogwho", 			1, 		BINARY },  /* 15 */
   { "gold",				LVL_GRGOD,	NUMBER },
   { "guildwalk",			LVL_GOD, 	NUMBER },
   { "gchan_name",			1, 		MISC },
   { "gchan_color",			1, 		MISC },
   { "gchan_type",			1, 		NUMBER },  /* 20 */
   { "gequipadd", 			LVL_GRGOD,      NUMBER },  
   { "gequipremove", 			LVL_GRGOD,	NUMBER },
   { "gskilladd",			LVL_GRGOD,	MISC },
   { "gskillremove", 			LVL_GRGOD,	MISC },
   { "gzoneadd", 			LVL_GRGOD, 	NUMBER },  /* 25 */
   { "gzoneremove",			LVL_GRGOD,	NUMBER }, 
   { "grankadd",			1, 		MISC },
   { "grankremove",			1, 		NUMBER },
   { "f_noguildwalk",			LVL_GOD, 	BINARY },
   { "f_nobank",			LVL_GOD, 	BINARY },  /* 30 */
   { "f_dark",				LVL_GRGOD,	BINARY },
   { "f_disguise_stay",			LVL_GRGOD,      BINARY },
   { "grankchange",			1,		MISC },
   { "\n",                  		0,	        MISC }				  
  };

  sprintf(to_log, "%s has guildset ", GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch));
  half_chop(argument, guild_num, buf);
  half_chop(buf, field, buf);
  strcpy(val_arg, buf);

  if (!*guild_num || !*field) {
    send_to_char("Usage: guildset <guild number/name> <field> <value>\r\n", ch);
    return;
  }

  guild_id = atoi(guild_num);

  g = guilds_data;

  while (g) {
   if (g->id == guild_id) break;
   if (!strncasecmp(g->name, guild_num, strlen(guild_num))) break; 
   g = g->next;
  }

  if (!g) {
   send_to_char("No such guild !\r\n", ch);
   return;
  }

  sprintf(to_log, "%s%s's ", to_log, g->name);

  for (l = 0; *(fields[l].cmd) != '\n'; l++)
    if (!strncmp(field, fields[l].cmd, strlen(field)))
      break;

  sprintf(to_log, "%s%s to", to_log, fields[l].cmd);

  if (GET_LEVEL(ch) < fields[l].level) {
    send_to_char("You are not godly enough for that!\r\n", ch);
    return;
  }

  if (GET_LEVEL(ch) < LVL_GRGOD) {

   element = GET_CHAR_GUILDS(ch);
   while (element) {
    if (element->guild == g) break;
    element = element->next;
   }

   if (!element) {
    send_to_char("You are not a part of this guild.\r\n", ch);
    return;
   }

   if (!STATUS_FLAGGED(element->guildie, STATUS_GL)) {
    send_to_char("You are not a guildleader of this guild !\r\n", ch);
    return;
   }
  
  }

  if (fields[l].type == BINARY) {
    if (!strcmp(val_arg, "on") || !strcmp(val_arg, "yes")){
      on = 1;
      sprintf(to_log, "%s on.", to_log);
    }
    else if (!strcmp(val_arg, "off") || !strcmp(val_arg, "no")){
      off = 1;
      sprintf(to_log, "%s off.", to_log);
    }
    if (!(on || off)) {
      send_to_char("Value must be on or off.\r\n", ch);
      return;
    }
  } else if (fields[l].type == NUMBER) {
    value = atoi(val_arg);
  }

  strcpy(to_char, "Okay.\r\n");  /* can't use OK macro here 'cause of \r\n */
  switch (l) {
  case 0:
    if (val_arg[0] == '\0')
     strcpy(val_arg, "NONE");
    if (c_strlen(val_arg) > 11) /* Used to be: strlen(...) > MAX_NAME_LENGTH */
     val_arg[MAX_TITLE_LENGTH] = '\0';
    if (g->gl_title != NULL)
     free(g->gl_title);
    g->gl_title = str_dup(val_arg); 
    sprintf(to_log, "%s %s.", to_log, val_arg);
    break;
  case 1:
    if (val_arg[0] == '\0')
     strcpy(val_arg, "NONE");
    if (c_strlen(val_arg) > 11) /* See last comment. */
     val_arg[MAX_TITLE_LENGTH] = '\0';
    if (g->guildie_titles != NULL)
     free(g->guildie_titles);
    g->guildie_titles = str_dup(val_arg); 
    sprintf(to_log, "%s %s.", to_log, val_arg);
    break;
  case 2:
    if (strlen(val_arg) > MAX_NAME_LENGTH)
     val_arg[MAX_TITLE_LENGTH] = '\0';
    if (g->gossip_name != NULL)
     free(g->gossip_name);
    g->gossip_name = str_dup(val_arg); 
    sprintf(to_log, "%s %s.", to_log, val_arg);
    break;
  case 3:
    if (val_arg[0] == '\0')
     strcpy(val_arg, "NONE");
    if (strlen(val_arg) > MAX_NAME_LENGTH)
     val_arg[MAX_TITLE_LENGTH] = '\0';
    if (g->guild_filename != NULL)
     free(g->guild_filename);
    g->guild_filename = str_dup(val_arg); 
    sprintf(to_log, "%s %s.", to_log, val_arg);
    save_all_guilds();
    break;
  case 4:
    g->id = value;
    sprintf(to_log, "%s %d.", to_log, value);
    break;
  case 5:
    if (val_arg[0] == '\0')
     strcpy(val_arg, "NONE");
    if (strlen(val_arg) > MAX_NAME_LENGTH)
     val_arg[MAX_TITLE_LENGTH] = '\0';
    if (g->name != NULL)
     free(g->name);
    g->name = str_dup(val_arg); 
    sprintf(to_log, "%s %s.", to_log, val_arg);
    break;
  case 6:
    if (value < 1 || value > RMAX_GUILD_TYPES) { 
      asend_to_char(ch,"Invalid guild type, valid range is [1 ; %d]\r\n",RMAX_GUILD_TYPES);
      return;
    }
    if (!strcasecmp(guild_types[value],"Undefined")) {       
      asend_to_char(ch,"Guild type #%d is currently undefined.\r\n",value);
      return;
    }
    g->type = value;
    sprintf(to_log, "%s %d.", to_log, value);
    break;
  case 7:
    SET_OR_REMOVE(g->gflags, GLD_EXCLUSIVE);
    break;
  case 8:
    SET_OR_REMOVE(g->gflags, GLD_IC);
    break;
  case 9:
    SET_OR_REMOVE(g->gflags, GLD_GLTITLE);
    break;
  case 10:
    SET_OR_REMOVE(g->gflags, GLD_GUILDIESTITLE);
    break;
  case 11:
    SET_OR_REMOVE(g->gflags, GLD_GOSSIP);
    break;
  case 12:
    SET_OR_REMOVE(g->gflags, GLD_NOGCHAN);
    break;
  case 13:
    SET_OR_REMOVE(g->gflags, GLD_SECRETIVE);
    break;
  case 14:
    SET_OR_REMOVE(g->gflags, GLD_NOGLIST);
    break;
  case 15:
    SET_OR_REMOVE(g->gflags, GLD_NOGWHO);
    break;
  case 16:
    if (GLD_FLAGGED(g, GLD_NOBANK)) {
     send_to_char("This guild has no bank account.\r\n", ch);
     return;
    }
    g->gold = value;
    sprintf(to_log, "%s %d.", to_log, value);
    break;
  case 17:
    g->guildwalk_room = value;
    sprintf(to_log, "%s %d.", to_log, value);
    break;
  case 18:
    if (val_arg[0] == '\0')
     strcpy(val_arg, "NONE");
    if (strlen(val_arg) > MAX_NAME_LENGTH)
     val_arg[MAX_TITLE_LENGTH] = '\0';
    if (g->gchan_name != NULL)
     free(g->gchan_name);
    g->gchan_name = str_dup(val_arg);
    sprintf(to_log, "%s %s.", to_log, val_arg);
    break;
  case 19:
    if (val_arg[0] == '\0')
     strcpy(val_arg, "R");
    if (strlen(val_arg) > MAX_NAME_LENGTH)
     val_arg[MAX_TITLE_LENGTH] = '\0';
    if (g->gchan_color != NULL)
     free(g->gchan_color);
    g->gchan_color = str_dup(val_arg);
    sprintf(to_log, "%s %s.", to_log, val_arg);
    break;
  case 20:
    if (value < 1 || value > 3) {
      send_to_char("The gchan_type must be a value between 1 and 3, inclusive.\r\n", ch);
      return;
    }
    g->gchan_type = value;
    sprintf(to_log, "%s %d.", to_log, value);
    break;
  case 21:
    gequip = calloc(1, sizeof(struct gequip_info));
    gequip->next = g->gequip;
    g->gequip = gequip;
    gequip->vnum = value;
    sprintf(to_log, "%s %d.", to_log, value);
    break;
  case 22:
    gequip = g->gequip;
    while (gequip) {
     if (gequip->vnum == value) break;
     gequip = gequip->next;
    }
    if (!gequip) {
     send_to_char("No such gequip.\r\n", ch);
     return;
    }
    if (g->gequip->vnum == value)
     g->gequip = g->gequip->next;
    else {
     gequip = g->gequip;
     while (gequip->next->vnum != value)
      gequip = gequip->next;
     gequip->next = gequip->next->next;
    }
    sprintf(to_log, "%s %d.", to_log, value);
    break;
  case 23:
    gskill = calloc(1, sizeof(struct gskill_info));
    half_chop(val_arg, buf, gskill_name);
    gskill_num = find_skill_num(gskill_name);
    if (gskill_num <= 0) {
     send_to_char("Unrecognized skill.\n\r", ch);
     return;
    }
    gskill->maximum_set = atoi(buf);
    gskill->skill = gskill_num;
    gskill->next = g->gskills;
    g->gskills = gskill;
    sprintf(to_log, "%s %s.", to_log, gskill_name);
    break;
  case 24:
    gskill = g->gskills;
    gskill_num = find_skill_num(val_arg);
    while (gskill) {
     if (gskill->skill == gskill_num) break;
     gskill = gskill->next;
    }
    if (!gskill) {
     send_to_char("No such gskill.\r\n", ch);
     return;
    }
    if (g->gskills->skill == gskill_num) 
     g->gskills = g->gskills->next; 
    else {
     gskill = g->gskills;
     while (gskill->next->skill != gskill_num)
      gskill = gskill->next;
     gskill->next = gskill->next->next;
    }
    sprintf(to_log, "%s %s.", to_log, val_arg);
    break;     
  case 25:
    gzone = calloc(1, sizeof(struct gzone_info));
    gzone->next = g->gzones;
    g->gzones = gzone;
    gzone->zone = value;
    sprintf(to_log, "%s %d.", to_log, value);
    break;
  case 26:
    gzone = g->gzones;
    while (gzone) {
     if (gzone->zone == value) break;
     gzone = gzone->next;
    }
    if (!gzone) {
     send_to_char("No such gzone.\r\n", ch);
     return;
    }
    if (g->gzones->zone == value)
     g->gzones = g->gzones->next;
    else {
     gzone = g->gzones; 
     while (gzone->next->zone != value)
      gzone = gzone->next;
     gzone->next = gzone->next->next;
    }
    sprintf(to_log, "%s %d.", to_log, value);
    break;
  case 27:
   half_chop(val_arg, buf, rank_name);
   rank_num = atoi(buf);
   if (rank_num <= 0) {
    send_to_char("Illegal rank number.\r\n", ch);
    return;
   }
   rank = calloc(1, sizeof(struct rank_info));
   rank->num = rank_num;
   rank->name = str_dup(rank_name);
   if (!g->ranks) 
    g->ranks = rank;
   else {
    r = g->ranks;
    while (r) {
     if (r->num >= rank_num) break;
     r = r->next;
    }
    if (!r) {
      r = g->ranks;
      while (r->next)
       r = r->next;
      r->next = rank;
    }
    else if (g->ranks == r) {
      rank->next = r;
      g->ranks = rank;
    }
    else {
      rank->next = r;
      r = g->ranks;
      while (r->next != rank->next)
       r = r->next;
      r->next = rank;
     }
    }

    r = rank->next;
    while (r) {
      r->num += 1;
      r = r->next;
     }

    guildie = g->guildies;
    while (guildie) {
     if (guildie->rank_num >= rank_num)
      guildie->rank_num += 1;
     guildie = guildie->next;
    }
    sprintf(to_log, "%s %s (%d).", to_log, rank_name, rank_num);
    break;
  case 28:
    rank = g->ranks;
    while (rank) {
     if (rank->num == value) break;
     rank = rank->next;
    }
    if (!rank) {
     send_to_char("No such rank.\r\n", ch);
     return;
    }
    if (g->ranks == rank) {
     g->ranks = g->ranks->next;
     r = g->ranks;
    }
    else {
     r = g->ranks;
     while (r->next != rank)
      r = r->next;
     r->next = rank->next;
     r = r->next;
    }
    r2 = r;
    while (r2) {
     r2->num -= 1;
     r2 = r2->next;
    }
    guildie = g->guildies;
    while (guildie) {
     if (guildie->rank_num >= value)
      guildie->rank_num -= 1;
     if (guildie->rank == rank) {
      if (r) {
       guildie->rank = r;
       guildie->rank_num = r->num;
      }
      else guildie->rank_num = 0;
     }
     guildie = guildie->next;
    }
    sprintf(to_log, "%s %d.", to_log, value);
    break;
  case 29:
    SET_OR_REMOVE(g->gflags, GLD_NOGUILDWALK);
    break;
  case 30:
    SET_OR_REMOVE(g->gflags, GLD_NOBANK);
    break;
  case 31:
    SET_OR_REMOVE(g->gflags, GLD_DARK);
    break;
  case 32:
    SET_OR_REMOVE(g->gflags, GLD_DISGUISE_STAY);
    break;
  case 33:
    half_chop(val_arg, buf, rank_name);
    if (!*buf || !*rank_name) {
      send_to_char("Usage: guildset <guild> grankchange <rank number> <new rank name>.\r\n", ch);
      return;
    }
    rank_num = atoi(buf);
    if (rank_num <= 0) {
      send_to_char("Illegal rank number.\r\n", ch);
      return;
    }

    rank = g->ranks;
    while (rank) {
     if (rank->num == rank_num) break;
     rank = rank->next;
    }
    
    if (rank) {
     if (rank->name)
      free(rank->name);
     rank->name = str_dup(rank_name);
     sprintf(to_char, "Rank %ld, changed to %s.\r\n", rank->num, rank->name);
    }
    else sprintf(to_char, "Cannot find that guild rank!\r\n");
    break;
  default:
    sprintf(to_char, "Can't set that!\r\n");
    do_log--;
    break;
  }

  save_guild(g);
  
  if (do_log) 
    xlog(SYS_GUILDSET, MAX(LVL_GOD, GET_INVIS_LEV(ch)), to_log, TRUE);
      
  send_to_char(to_char, ch);
}

ACMD(do_guildieset)
{
  int l;
  struct guild_info *g;
  char guild_name[MAX_INPUT_LENGTH], guildie_name[MAX_INPUT_LENGTH], to_char[MAX_STRING_LENGTH];
  char field[MAX_INPUT_LENGTH], val_arg[MAX_INPUT_LENGTH], to_log[MAX_STRING_LENGTH];
  int guild_id, on = 0, off = 0, value = 0, do_log = 1;
  struct char_guild_element *element = NULL;
  struct guildie_info *guildie;
  struct rank_info *rank;
  struct gequip_info *gequip, *gequip2;
  struct qp_info *qp;

  struct set_struct {
    char *cmd;
    int level;
    char type;
  }          fields[] = {
   { "gchan", 		 	1, 		BINARY },    /* 0 */
   { "gl",			LVL_GRGOD, 	BINARY },
   { "perm_guild",		1,		BINARY },
   { "perm_withdraw", 		1, 		BINARY },
   { "perm_gossip",		1, 		BINARY },
   { "perm_sponsor",		1, 		BINARY },    /* 5 */
   { "perm_authorize", 		1, 		BINARY },
   { "rank", 			1,		NUMBER },
   { "perm_gskillset",		1, 		BINARY },
   { "authorized",		1,		BINARY },
   { "subrank",			1, 		MISC },      /* 10 */
   { "status_warder",		1, 		BINARY },
   { "qpadd", 			LVL_GOD,	NUMBER },
   { "gequipsend",		1,		NUMBER },
   { "gequipunsend",		1,              NUMBER },
   { "sponsorlist",             1,              BINARY },    /* 15 */
   { "\n",                 	0,         	MISC }				  
  };

  sprintf(to_log, "%s has guildieset ", GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch));
  half_chop(argument, guild_name, buf);
  half_chop(buf, guildie_name, buf);
  half_chop(buf, field, buf);
  strcpy(val_arg, buf);

  if (!*guild_name || !*guildie_name || !*field) {
    send_to_char("Usage: guildieset <guild number/name> <guildie name> <field> <value>\r\n", ch);
    return;
  }

  guild_id = atoi(guild_name);

  g = guilds_data;

  while (g) {
   if (g->id == guild_id) break;
   if (!strncasecmp(g->name, guild_name, strlen(guild_name))) break; 
   g = g->next;
  }

  if (!g) {
   send_to_char("No such guild.\r\n", ch);
   return;
  }

  if (GET_LEVEL(ch) < LVL_GRGOD) {
   
   element = GET_CHAR_GUILDS(ch);
   while (element) {
    if (element->guild == g) break;
    element = element->next;
   }
      
   if (!element) {
    send_to_char("You are not a part of this guild.\r\n", ch);
    return;
   }
    
  }
  
  guildie = g->guildies;

  while (guildie) {
   if (!strcasecmp(guildie->name, guildie_name)) break;
   guildie = guildie->next;
  }

  if (!guildie) {
   send_to_char("No such person in the guild.\r\n", ch);
   return;
  }

  sprintf(to_log, "%s%s's ", to_log, guildie->name);

  for (l = 0; *(fields[l].cmd) != '\n'; l++)
    if (!strncmp(field, fields[l].cmd, strlen(field)))
      break;

  sprintf(to_log, "%s%s to", to_log, fields[l].cmd);

  if (GET_LEVEL(ch) < fields[l].level) {
    send_to_char("You are not godly enough for that!\r\n", ch);
    return;
  }

  if (fields[l].type == BINARY) {
    if (!strcmp(val_arg, "on") || !strcmp(val_arg, "yes")){
      on = 1;
      sprintf(to_log, "%s on.", to_log);
    }
    else if (!strcmp(val_arg, "off") || !strcmp(val_arg, "no")){
      off = 1;
      sprintf(to_log, "%s off.", to_log);
    }
    if (!(on || off)) {
      send_to_char("Value must be on or off.\r\n", ch);
      return;
    }
  } else if (fields[l].type == NUMBER) {
    value = atoi(val_arg);
  }

  strcpy(to_char, "Okay.\r\n");  /* can't use OK macro here 'cause of \r\n */
  switch (l) {
  case 0:
   if (GET_LEVEL(ch) < LVL_GRGOD)
    if (!STATUS_FLAGGED(element->guildie, STATUS_GL)) {
     send_to_char("You can't do that.\r\n", ch);
     return;
    }
   SET_OR_REMOVE(guildie->status, STATUS_CHAN_ON);
   break;
  case 1:
    SET_OR_REMOVE(guildie->status, STATUS_GL);
    SET_OR_REMOVE(guildie->perm, PERM_GUILD);
    SET_OR_REMOVE(guildie->perm, PERM_WITHDRAW); 
    SET_OR_REMOVE(guildie->perm, PERM_GOSSIP_WRITE); 
    SET_OR_REMOVE(guildie->perm, PERM_SPONSOR); 
    SET_OR_REMOVE(guildie->perm, PERM_GSKILLSET); 
    SET_OR_REMOVE(guildie->perm, PERM_AUTHORIZE);
    break;
  case 2:
   if (GET_LEVEL(ch) < LVL_GRGOD)
    if (!STATUS_FLAGGED(element->guildie, STATUS_GL)) {                            
     send_to_char("You can't do that.\r\n", ch);
     return;
    }
   SET_OR_REMOVE(guildie->perm, PERM_GUILD);
   break;
  case 3:
   if (GET_LEVEL(ch) < LVL_GRGOD)
    if (!STATUS_FLAGGED(element->guildie, STATUS_GL)) {                            
     send_to_char("You can't do that.\r\n", ch);
     return;
    }
   SET_OR_REMOVE(guildie->perm, PERM_WITHDRAW);
   break;
  case 4:
   if (GET_LEVEL(ch) < LVL_GRGOD)
    if (!STATUS_FLAGGED(element->guildie, STATUS_GL)) {                            
     send_to_char("You can't do that.\r\n", ch);
     return;
    }
   SET_OR_REMOVE(guildie->perm, PERM_GOSSIP_WRITE);
   break;
  case 5:
   if (GET_LEVEL(ch) < LVL_GRGOD)
    if (!STATUS_FLAGGED(element->guildie, STATUS_GL)) {                            
     send_to_char("You can't do that.\r\n", ch);
     return;
    }
   SET_OR_REMOVE(guildie->perm, PERM_SPONSOR);
   break;
  case 6:
   if (GET_LEVEL(ch) < LVL_GRGOD)
    if (!STATUS_FLAGGED(element->guildie, STATUS_GL)) {                            
     send_to_char("You can't do that.\r\n", ch);
     return;
    }
   SET_OR_REMOVE(guildie->perm, PERM_AUTHORIZE);
   break;
  case 7:
   if (GET_LEVEL(ch) < LVL_GRGOD)
    if (!STATUS_FLAGGED(element->guildie, STATUS_GL)) {                            
     send_to_char("You can't do that.\r\n", ch);
     return;
    } 
   if (!value) {
    guildie->rank = NULL;
    guildie->rank_num = 0;
    if (guildie->ch) {
     sprintf(buf, "&G%s has just removed your rank.&n\r\n", GET_NAME(ch));
     send_to_char(buf, guildie->ch);
    }
    break;
   }
   rank = g->ranks;
   while (rank) {
    if (rank->num == value) break;
    rank = rank->next;
   }
   if (!rank) {
    send_to_char("No such rank.\r\n", ch);
    return;
   }
   guildie->rank = rank;
   guildie->rank_num = rank->num;
   if (guildie->ch) {
    sprintf(buf, "&G%s has just set your rank to %s.&n\r\n", GET_NAME(ch), rank->name);
    send_to_char(buf, guildie->ch);
   }
   sprintf(to_log, "%s %d.", to_log, value);
   break;
  case 8:
   if (GET_LEVEL(ch) < LVL_GRGOD)
    if (!STATUS_FLAGGED(element->guildie, STATUS_GL)) {                            
     send_to_char("You can't do that.\r\n", ch);
     return;
    }
   SET_OR_REMOVE(guildie->perm, PERM_GSKILLSET);
   break;
  case 9:
   if (GET_LEVEL(ch) < LVL_GRGOD)
    if (!PERM_FLAGGED(element->guildie, PERM_AUTHORIZE)) {
     send_to_char("You can't do that.\r\n", ch);
     return;
    }
   SET_OR_REMOVE(guildie->status, STATUS_AUTHORIZED);  
   break;
  case 10:
   if (GET_LEVEL(ch) < LVL_GRGOD)
    if (!STATUS_FLAGGED(element->guildie, STATUS_GL)) {
     send_to_char("You can't do that.\r\n", ch);
     return;
    }
   if (val_arg[0] == '\0') {
    strcpy(val_arg, "NONE");
    REMOVE_BIT(guildie->status, STATUS_SUBRANK);
    if (guildie->ch) {
     sprintf(buf, "&G%s has just removed your subrank.&n\r\n", GET_NAME(ch));
     send_to_char(buf, guildie->ch);
    }
    sprintf(to_log, "%s %s.", to_log, val_arg);
    break;
   }
   if (strlen(val_arg) > MAX_NAME_LENGTH)
    val_arg[MAX_TITLE_LENGTH] = '\0';
   guildie->subrank = str_dup(val_arg); 
   SET_BIT(guildie->status, STATUS_SUBRANK);
   if (guildie->ch) {
    sprintf(buf, "&G%s has just set your subrank to %s.&n\r\n", GET_NAME(ch), val_arg);
    send_to_char(buf, guildie->ch);
   }
   sprintf(to_log, "%s %s.", to_log, val_arg);
   break;
  case 11:
   if (GET_LEVEL(ch) < LVL_GRGOD)
    if (!STATUS_FLAGGED(element->guildie, STATUS_GL)) {
     send_to_char("You can't do that.\r\n", ch);
     return;
    }
   SET_OR_REMOVE(guildie->status, STATUS_WARDER);
   break;
  case 12:
   if (GET_LEVEL(ch) < LVL_GRGOD)
    if (!STATUS_FLAGGED(element->guildie, STATUS_GL)) {                            
     send_to_char("You can't do that.\r\n", ch);
     return;
    } 
   if (!value) {
    send_to_char("Yes, but how many QPs ? Use 'guildieset <guild> <member> qpadd <number>'.\r\n", ch);
    return;
   }
   qp = calloc(1,sizeof(struct qp_info));
   qp->name = str_dup(GET_NAME(ch));
   qp->num = (long) value;
   qp->next = guildie->qpadd;
   guildie->qpadd = qp;
   sprintf(to_log, "%s %d.", to_log, value);
   //xlog(SYS_GUILDIESET, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), to_log, TRUE);   
   do_log = 1;
   break;
  case 13:
   send_to_char("Gequipsend is disabled for now.\r\n", ch);
   return;
  /* -- DISABLING UNTIL BUG IS FIXED!
   if (GET_LEVEL(ch) < LVL_GRGOD)
    if (!STATUS_FLAGGED(element->guildie, STATUS_GL)) {                            
     send_to_char("You can't do that.\r\n", ch);
     return;
    } 
   if (!value) {
    send_to_char("Yes, but which gequip ? Use 'guildieset <guild> <member> gequipsend <vnum>'.\r\n", ch);
    return;
   }
   
   gequip = g->gequip;
   while (gequip) {
    if (gequip->vnum == value)
     break;
    gequip = gequip->next;
   }
   if (!gequip) {
    send_to_char("No gequip with such a vnum in your guild.\r\n", ch);
    return;
   }

   gequip = calloc(1,sizeof(struct gequip_info)); 
   gequip->vnum = value;
   gequip->next = guildie->gequipsent;
   guildie->gequipsent = gequip;

   sprintf(to_log, "%s %d.", to_log, value);
   do_log = 1;
   */
   break;
  case 14:
   if (GET_LEVEL(ch) < LVL_GRGOD)
    if (!STATUS_FLAGGED(element->guildie, STATUS_GL)) {                            
     send_to_char("You can't do that.\r\n", ch);
     return;
    } 
   if (!value) {
    send_to_char("Yes, but which gequip ? Use 'guildieset <guild> <member> gequipunsend <vnum>'.\r\n", ch);
    return;
   }
   gequip = guildie->gequipsent;
   while (gequip)
   {
    if (gequip->vnum == value)
     break;
    gequip = gequip->next;
   }
   if (!gequip)
   {
    send_to_char("No such gequip was sent to this guildie.\r\n", ch);
    return;
   }

   if (guildie->gequipsent == gequip)
   {
    guildie->gequipsent = gequip->next;
    free(gequip);
   }
   else 
   {
    gequip2 = guildie->gequipsent;
    while (gequip2->next != gequip)
     gequip2 = gequip2->next;
    gequip2->next = gequip->next;
    free(gequip);
   }
  case 15:
   SET_OR_REMOVE(guildie->status, STATUS_SPONSORLIST);
   break;
     

   sprintf(to_log, "%s %d.", to_log, value);
   //xlog(SYS_GUILDIESET, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), to_log, TRUE);
   do_log = 1;
   break;
  default:
    sprintf(to_char, "Can't set that!\r\n");
    do_log = 0;
    break;
  }

  save_guild(g);
  if (do_log)
    xlog(SYS_GUILDIESET, MAX(LVL_GOD, GET_INVIS_LEV(ch)), to_log, TRUE);	
          
  send_to_char(to_char, ch);
}

ACMD(do_guild)
{
  char arg1[100], arg2[100];
  int num;
  struct guild_info *g, *g2;
  struct char_guild_element *element, *e2;
  struct guildie_info *guildie;
  struct char_data *vict;

  two_arguments(argument, arg1, arg2);

  if (!*arg1 || !*arg2)
  {  
    send_to_char("Usage: guild <playername> <guild name/id>\r\n", ch);
    return;
  }

  g = guilds_data;
  num = atoi(arg2);

  while (g) {
   if (g->id == num) break;
   if (!strncasecmp(g->name, arg2, strlen(arg2))) break;
   g = g->next;
  }

  if (!g) {
   send_to_char("No such guild !\r\n", ch);
   return;
  }

  if (GET_LEVEL(ch) < LVL_GRGOD) {

   element = GET_CHAR_GUILDS(ch);

   while (element) {
    if (element->guild == g) break;
    element = element->next;
   }

   if (!element) {
    send_to_char("You are not a member in this guild !\r\n", ch);
    return;
   }

   if (!PERM_FLAGGED(element->guildie, PERM_GUILD)) {
    send_to_char("You can't guild seekers.\r\n", ch);
    return;
   }
  }

  guildie = g->guildies;

  while (guildie) {
   if (!strcasecmp(guildie->name, arg1)) 
    break;
   guildie = guildie->next;
  }

  if (guildie) {

   if (STATUS_FLAGGED(guildie, STATUS_MEMBER)) {
    send_to_char("This character is already a member.\r\n", ch);
    return;
   }

   REMOVE_BIT(guildie->status, STATUS_SEEKING);
   REMOVE_BIT(guildie->status, STATUS_AUTHORIZED);
   SET_BIT(guildie->status, STATUS_MEMBER);
   SET_BIT(guildie->status, STATUS_CHAN_ON);
   SET_BIT(guildie->status, STATUS_TAG);

   save_guild(g);

   if (guildie->ch) {
    sprintf(buf, "&RCongratulations! You are now a member of the %s.&n\r\n", g->name);
    send_to_char(buf, guildie->ch);
   }

   sprintf(buf, "&R%s is now a member of the %s.&n\r\n", guildie->name,
        g->name);
   send_to_char(buf, ch);

   sprintf(buf2, "%s has guilded %s to %s.", GET_NAME(ch), guildie->name, 
        g->name);
   xlog(SYS_GUILD, MAX(LVL_GOD, GET_INVIS_LEV(ch)), buf2, TRUE);

   return;
  }

  vict = get_char_vis(ch, arg1);
  if (vict) 
   if (!IS_MOB(vict)) {

    g2 = guilds_data;
    while (g2) {
     guildie = g2->guildies;
     while (guildie) {
      if (guildie->idnum == GET_IDNUM(vict) && GLD_FLAGGED(g2, GLD_EXCLUSIVE) && GLD_FLAGGED(g, GLD_EXCLUSIVE))
       if (GET_LEVEL(vict) < LVL_GRGOD) {
        send_to_char("He/She is already a part of an exclusive guild.\r\n", ch);
        return;
       }
      guildie = guildie->next;
     }
     g2 = g2->next;
    }

    guildie = calloc(1,sizeof(struct guildie_info));  
    e2 = calloc(1,sizeof(struct char_guild_element));

    e2->guild = g;  
    e2->guildie = guildie;  
    e2->next = GET_CHAR_GUILDS(vict);  
    GET_CHAR_GUILDS(vict) = e2;

    guildie->next = g->guildies;  
    g->guildies = guildie;
  
    SET_BIT(guildie->status, STATUS_MEMBER);  
    SET_BIT(guildie->status, STATUS_CHAN_ON);
    SET_BIT(guildie->status, STATUS_TAG);
    guildie->idnum = GET_IDNUM(vict);  
    guildie->name = str_dup(GET_NAME(vict));   
    guildie->ch = vict;

    if (guildie->ch) {
     sprintf(buf, "&RCongratulations! You are now a member of the %s.&n\r\n", g->name);
     send_to_char(buf, guildie->ch);
    }
   
    sprintf(buf, "&R%s is now a member of the %s.&n\r\n", guildie->name,
               g->name);
    send_to_char(buf, ch);

    sprintf(buf2, "%s has guilded %s to %s.", GET_NAME(ch), guildie->name, 
        g->name);
    xlog(SYS_GUILD, MAX(LVL_GOD, GET_INVIS_LEV(ch)), buf2, TRUE);
 
    return;
   }

  if (NULL != (vict = xmlload_char(arg1))) {
    g2 = guilds_data;
    while (g2) {
      guildie = g2->guildies;
      while (guildie) {
    if (guildie->idnum == GET_IDNUM(vict) && GLD_FLAGGED(g2, GLD_EXCLUSIVE) && GLD_FLAGGED(g, GLD_EXCLUSIVE))
      if (GET_LEVEL(vict) < LVL_GRGOD) {
        send_to_char("He/She is already a part of an exclusive guild.\r\n", ch);
        return;
      }
    guildie = guildie->next;
      }
      g2 = g2->next;
    }

    guildie = calloc(1,sizeof(struct guildie_info));  
    e2 = calloc(1,sizeof(struct char_guild_element));

    e2->guild = g;  
    e2->guildie = guildie;  
    e2->next = GET_CHAR_GUILDS(vict);  
    GET_CHAR_GUILDS(vict) = e2;

    guildie->next = g->guildies;  
    g->guildies = guildie;
  
    SET_BIT(guildie->status, STATUS_MEMBER);  
    SET_BIT(guildie->status, STATUS_CHAN_ON);
    SET_BIT(guildie->status, STATUS_TAG);
    guildie->idnum = GET_IDNUM(vict);  
    guildie->name = str_dup(GET_NAME(vict));   
    guildie->ch = NULL;

    sprintf(buf, "&R%s is now a member of the %s.&n\r\n", guildie->name,
               g->name);
    send_to_char(buf, ch);

   sprintf(buf2, "%s has guilded %s to %s.", GET_NAME(ch), guildie->name, 
        g->name);
   xlog(SYS_GUILD, MAX(LVL_GOD, GET_INVIS_LEV(ch)), buf2, TRUE);

    free_char(vict);

    return;
  }
  send_to_char("No such character.\r\n", ch);
}

ACMD(do_deguild)
{
  char arg1[100], arg2[100];
  int num;
  struct guild_info *g;
  struct char_guild_element *element, *t_e;
  struct guildie_info *guildie, *guildie2;

  two_arguments(argument, arg1, arg2);

  if (!*argument || !*arg2)
  {  
    send_to_char("Usage: deguild <playername> <guild name/id>\r\n", ch);
    return;
  }

  g = guilds_data;
  num = atoi(arg2);

  while (g) {
   if (g->id == num) break;
   if (!strncasecmp(g->name, arg2, strlen(arg2))) break;
   g = g->next;
  }

  if (!g) {
   send_to_char("No such guild !\r\n", ch);
   return;
  }

  if (GET_LEVEL(ch) < LVL_GRGOD) {

   element = GET_CHAR_GUILDS(ch);

   while (element) {
    if (element->guild == g) break;
    element = element->next;
   }

   if (!element) {
    send_to_char("You are not a member in this guild !\r\n", ch);
    return;
   }

   if (!STATUS_FLAGGED(element->guildie, STATUS_GL)) {  
    send_to_char("You can't deguild members !\r\n", ch);
    return;
   }
  }

  guildie = g->guildies;

  while (guildie) {
   if (!strcasecmp(guildie->name, arg1)) 
    break;
   guildie = guildie->next;
  }

  if (!guildie) {
   send_to_char("No member with such a name.\r\n", ch);
   return;
  }
/* Likely causing a nasty bug
  if (STATUS_FLAGGED(guildie, STATUS_GL) && (GET_LEVEL(ch) < LVL_GOD)) {  
   send_to_char("&uYou&n aren't allowed to deguild a GL.\r\n", ch);
   return;
  }
*/
  
  if (g->guildies == guildie)
   g->guildies = g->guildies->next;
  else {
   guildie2 = g->guildies;
   while (guildie2->next != guildie)
    guildie2 = guildie2->next;
   guildie2->next = guildie->next;
  }

  save_guild(g);

  if (guildie->ch) 
  {
    sprintf(buf, "&RYou have been deguilded from the %s&n.\r\n", g->name);
    send_to_char(buf, guildie->ch);
    if ((GET_CHAR_GUILDS(guildie->ch))->guildie == guildie)
      GET_CHAR_GUILDS(guildie->ch) = (GET_CHAR_GUILDS(guildie->ch))->next;
    else 
    {
      element = GET_CHAR_GUILDS(guildie->ch);
      while(element->next && element->next->guildie != guildie)
	element = element->next;
      t_e = element->next->next;
      free(element->next);
      element->next = t_e;
    }
  }

  sprintf(buf, "&R%s is deguilded from the %s.&n\r\n", guildie->name,
        g->name);
  send_to_char(buf, ch);

  sprintf(buf2, "%s has deguilded %s from %s.", GET_NAME(ch), guildie->name,
        g->name);
  xlog(SYS_GUILD, MAX(LVL_GOD, GET_INVIS_LEV(ch)), buf2, TRUE);

  free_guildie_info(guildie);
}

ACMD(do_seekguild)
{
  char arg1[100], arg2[100];
  int num;
  struct guild_info *g;
  struct char_guild_element *element, *t_e;
  struct guildie_info *guildie, *guildie2;

//  one_argument(argument, arg1);
  two_arguments(argument, arg1, arg2);

  if (!*argument || !*arg1)
  {  
    send_to_char("Usage: seekguild <guild name/id> [off]", ch);
    return;
  }

  g = guilds_data;
  num = atoi(arg1);

  while (g) {
   if (g->id == num) break;
   if (!strncasecmp(g->name, arg1, strlen(arg1))) break;
   g = g->next;
  }

  if (!g) {
   send_to_char("No such guild !\r\n", ch);
   return;
  }
  
  if (*arg2) {
    if (strncasecmp(arg2, "off", strlen("off")) == 0) {
      element = GET_CHAR_GUILDS(ch);
      while (element) {
    if (element->guild == g)
      break;
    element = element->next;
      }
     
      if (!element) {
        send_to_char("You are not seeking that guild.\r\n", ch);
        return;
      }
     
      if (!STATUS_FLAGGED(element->guildie, STATUS_SEEKING)) {
        send_to_char("You are already a member of this guild. Ask to be deguilded instead.\r\n", ch);
        return;
      }
     
      guildie = element->guildie;
    
      if (g->guildies == guildie)
        g->guildies = g->guildies->next;
      else {
        guildie2 = g->guildies;
        while (guildie2->next != guildie) 
      guildie2 = guildie2->next;
        guildie2->next = guildie->next;
      }
     
      save_guild(g);
     
      if ((GET_CHAR_GUILDS(ch))->guildie == guildie)  {
    t_e = (GET_CHAR_GUILDS(ch))->next;
    free(GET_CHAR_GUILDS(ch));
        (GET_CHAR_GUILDS(ch)) = t_e;
      }
      else {
        element = GET_CHAR_GUILDS(ch);
        while (element->next->guildie != guildie)
      element = element->next;
    t_e = element->next->next;
    free(element->next);
        element->next = t_e;
      }
    
      asend_to_char(ch, "You are no longer seeking %s.\r\n", g->name);
     
      free_guildie_info(guildie);
      return;
    }
  }

  element = GET_CHAR_GUILDS(ch);
  
  while (element) {
   if (element->guild == g)
    break;
   if (GLD_FLAGGED(element->guild, GLD_EXCLUSIVE) && GLD_FLAGGED(g, GLD_EXCLUSIVE) &&
       GET_LEVEL(ch) < LVL_GRGOD) {
    send_to_char("You can't join more than one exclusive guild.\r\n", ch);
    return;
   }
   element = element->next;
  }

  if (element) {
   if (STATUS_FLAGGED(element->guildie, STATUS_MEMBER)) 
    send_to_char("You are already a member :)\r\n", ch);
   if (STATUS_FLAGGED(element->guildie, STATUS_SEEKING))
    send_to_char("You are already seeking :)\r\n", ch);
   return;
  }

  guildie = calloc(1,sizeof(struct guildie_info));
  element = calloc(1,sizeof(struct char_guild_element));
  
  element->guild = g;
  element->guildie = guildie;
  element->next = GET_CHAR_GUILDS(ch);
  GET_CHAR_GUILDS(ch) = element;

  guildie->next = g->guildies;
  g->guildies = guildie;

  SET_BIT(guildie->status, STATUS_SEEKING);
  guildie->idnum = GET_IDNUM(ch);
  guildie->name = str_dup(GET_NAME(ch)); 
  guildie->ch = ch;

  save_guild(g);

  sprintf(buf, "&RYou are now seeking the %s.&n\r\n", g->name);
   send_to_char(buf, ch);

}

ACMD(do_seeking)
{
  char guild_name[MAX_INPUT_LENGTH];
  int guild_id, i;
  struct char_guild_element *element;
  struct guildie_info *guildie;
  struct sponsorer_info *sponsorer;

  one_argument(argument, guild_name);
  
  if (!*guild_name) {
    send_to_char("Usage: seeking <guild number/name>\r\n", ch);
    return;
  }

  guild_id = atoi(guild_name);

  element = GET_CHAR_GUILDS(ch);
  while (element) {
   if (element->guild->id == guild_id) break;
   if (!strncasecmp(element->guild->name, guild_name, strlen(guild_name))) break; 
   element = element->next;
  }

  if (!element) {
   send_to_char("You are not a part of this guild.\r\n", ch);
   return;
  }

  if (GLD_FLAGGED(element->guild, GLD_SECRETIVE) && !PERM_FLAGGED(element->guildie, PERM_SPONSOR)) {
   send_to_char("You haven't been authorized to sponsor seekers, so you can't.\r\n", ch);
   return;
  }

  sprintf(buf,"\r\n\\c14Players seeking the %s\\c00\r\n", element->guild->name);
  sprintf(buf,"%s\\c14====================", buf);

  for (i = 1; i <= strlen(element->guild->name); i++)
    sprintf(buf, "%s=", buf);

  sprintf(buf, "%s\\c00\r\n\r\n", buf);

  guildie = element->guild->guildies;

  while (guildie) {
   if (STATUS_FLAGGED(guildie, STATUS_SEEKING)) {

    if(guildie->ch)
     sprintf(buf,"%s%s (\\c02online\\c00)",buf,CAP(guildie->name));
    else
     sprintf(buf,"%s%s (\\c01offline\\c00)",buf,CAP(guildie->name));

    if (!STATUS_FLAGGED(guildie, STATUS_AUTHORIZED))
     sprintf(buf, "%s (&RUnauthorized&n)", buf);
    else
     sprintf(buf, "%s (&GAuthorized&n)", buf);

    /* Is the player ready to be guilded level-wise or not? */        
    /* I'll need this done sometimes :p */

    sponsorer = guildie->sponsorers;
    if (sponsorer) {
     sprintf(buf, "%s [ ", buf);
     while (sponsorer) {
      sprintf(buf, "%s%s ", buf, sponsorer->name);
      sponsorer = sponsorer->next;
     }
     sprintf(buf, "%s]", buf);
    }

    sprintf(buf, "%s\r\n", buf);
   }

   guildie = guildie->next;

  }

  page_string(ch->desc,buf,1);

}

ACMD(do_sponsor)
{
  char arg1[100], arg2[100];
  int num;
  struct guild_info *g;
  struct char_guild_element *element;
  struct guildie_info *guildie;
  struct sponsorer_info *sponsorer;

  two_arguments(argument, arg1, arg2);

  if (!*argument || !*arg2)
  {  
    send_to_char("Usage: sponsor <playername> <guild name/id>\r\n", ch);
    return;
  }

  g = guilds_data;
  num = atoi(arg2);

  while (g) {
   if (g->id == num) break;
   if (!strncasecmp(g->name, arg2, strlen(arg2))) break;
   g = g->next;
  }

  if (!g) {
   send_to_char("No such guild !\r\n", ch);
   return;
  }

  element = GET_CHAR_GUILDS(ch);

  while (element) {
   if (element->guild == g) break;
   element = element->next;
  }

  if (!element) {
   send_to_char("You are not a member in this guild !\r\n", ch);
   return;
  }

  if (!PERM_FLAGGED(element->guildie, PERM_SPONSOR)) {
   send_to_char("You can't sponsor seekers.\r\n", ch);
   return;
  }

  guildie = g->guildies;

  while (guildie) {
   if (!strncasecmp(guildie->name, arg1, strlen(guildie->name))) 
    break;
   guildie = guildie->next;
  }

  if (!guildie) {
   send_to_char("No seeker with such a name.\r\n", ch);
   return;
  }
  
  if (!STATUS_FLAGGED(guildie, STATUS_SEEKING)) {
   sprintf(buf, "%s is already a member.\r\n", guildie->name);
   send_to_char(buf, ch);
   return;
  }

  if (!STATUS_FLAGGED(guildie, STATUS_AUTHORIZED)) {
   sprintf(buf, "&M%s is not yet authorized.&n\r\n", guildie->name);
   send_to_char(buf, ch);
   return;
  }

  if (guildie->sponsorers) {
    sponsorer = guildie->sponsorers;
    while (sponsorer){
      if (sponsorer->idnum == GET_IDNUM(ch)){
        send_to_char("You can't sponsor the same seeker multiple times.\r\n", ch);
        return;
      }
      sponsorer = sponsorer->next;
    }
  }

  sponsorer = calloc(1,sizeof(struct sponsorer_info));
  sponsorer->idnum = GET_IDNUM(ch);
  sponsorer->name = str_dup(GET_NAME(ch));
  sponsorer->next = guildie->sponsorers;
  guildie->sponsorers = sponsorer;

  save_guild(g);

  if (guildie->ch) {
   sprintf(buf, "&RYou are now sponsored by %s.&n\r\n", GET_NAME(ch));
   send_to_char(buf, guildie->ch);
  }

  sprintf(buf, "&RYou are now sponsoring %s.&n\r\n", guildie->name);
  send_to_char(buf, ch);

}

ACMD(do_unsponsor)
{
  char arg1[100], arg2[100];
  int num;
  struct guild_info *g;
  struct char_guild_element *element;
  struct guildie_info *guildie;
  struct sponsorer_info *sponsorer, *spons_2;

  two_arguments(argument, arg1, arg2);

  if (!*argument || !*arg2)
  {  
    send_to_char("Usage: unsponsor <playername> <guild name/id>\r\n", ch);
    return;
  }

  g = guilds_data;
  num = atoi(arg2);

  while (g) {
   if (g->id == num) break;
   if (!strncasecmp(g->name, arg2, strlen(arg2))) break;
   g = g->next;
  }

  if (!g) {
   send_to_char("No such guild !\r\n", ch);
   return;
  }

  element = GET_CHAR_GUILDS(ch);

  while (element) {
   if (element->guild == g) break;
   element = element->next;
  }

  if (!element) {
   send_to_char("You are not a member in this guild !\r\n", ch);
   return;
  }

  if (!PERM_FLAGGED(element->guildie, PERM_SPONSOR)) {
   send_to_char("You can't even sponsor seekers.\r\n", ch);
   return;
  }

  guildie = g->guildies;

  while (guildie) {
   if (!strncasecmp(guildie->name, arg1, strlen(guildie->name))) 
    break;
   guildie = guildie->next;
  }

  if (!guildie) {
   send_to_char("No seeker with such a name.\r\n", ch);
   return;
  }
  
  if (!STATUS_FLAGGED(guildie, STATUS_SEEKING)) {
   sprintf(buf, "%s is already a member.\r\n", guildie->name);
   send_to_char(buf, ch);
   return;
  }

  if (!(guildie->sponsorers)) {
    sprintf(buf, "%s doesn't have any sponsors!\r\n", guildie->name);
    send_to_char(buf, ch);
    return;
  }
  
  sponsorer = guildie->sponsorers;
  if (sponsorer->idnum == GET_IDNUM(ch))
    guildie->sponsorers = guildie->sponsorers->next;
  else {
    spons_2 = sponsorer;
    sponsorer = sponsorer->next;
    while (sponsorer) {
      if (sponsorer->idnum == GET_IDNUM(ch)) {
        spons_2->next = sponsorer->next;
        break;
      }
      spons_2 = sponsorer;
      sponsorer = sponsorer->next;
    }
  }
  
  if (!sponsorer) {
    sprintf(buf, "You are not sponsoring %s.\r\n", guildie->name);
    send_to_char(buf, ch);
    return;
  }
  
  free(sponsorer->name);
  free(sponsorer);
      
  save_guild(g);

  if (guildie->ch) {
   sprintf(buf, "&RYou are no longer sponsored by %s.&n\r\n", GET_NAME(ch));
   send_to_char(buf, guildie->ch);
  }

  sprintf(buf, "&RYou are no longer sponsoring %s.&n\r\n", guildie->name);
  send_to_char(buf, ch);

}

ACMD(do_sponsorlist)
{
  char guild_name[MAX_INPUT_LENGTH];
  int guild_id, i, is_gl = 0;
  struct char_guild_element *element;
  struct guildie_info *guildie;
  int not_really_a_member = 0;

  one_argument(argument, guild_name);
  
  if (!*guild_name) {
    send_to_char("Usage: sponsorlist <guild number/name>\r\n", ch);
    return;
  }

  guild_id = atoi(guild_name);

  element = GET_CHAR_GUILDS(ch);
  while (element) {
   if (element->guild->id == guild_id) break;
   if (!strncasecmp(element->guild->name, guild_name, strlen(guild_name))) break; 
   element = element->next;
  }

  /* if you're not in that guild, element == NULL. this will cause a
     segfault further down... */
  if (!element) {
    if (GET_LEVEL(ch) < LVL_GRGOD) {
      send_to_char("You are not a part of this guild.\r\n", ch);
      return;
    } else
      not_really_a_member = 1;
  }

  if (not_really_a_member) {
    do_seekguild(ch, argument, 0, 0);
    element = GET_CHAR_GUILDS(ch);
    while (element) {
      if (element->guild->id == guild_id) break;
      if (!strncasecmp(element->guild->name, guild_name, strlen(guild_name))) break; 
      element = element->next;
    }    
  }

  if (element) {
    if (STATUS_FLAGGED(element->guildie, STATUS_GL) || GET_LEVEL(ch) >= LVL_GRGOD)
      is_gl = 1;
  } else {
    send_to_char("*oops* It seems something went wrong.\r\n", ch);
    return;
  }

  guildie = element->guild->guildies;

//  sprintf(buf,"\r\n\\c14%s guildies\\c00\r\n", element->guild->name);
  sprintf(buf,"&WSponsorlist for %s\r\n&n", element->guild->name);
                 
  sprintf(buf,"%s\\c14================", buf);

  for (i = 1; i <= strlen(element->guild->name); i++)
    sprintf(buf, "%s=", buf);

  sprintf(buf, "%s\\c00\r\n\r\n", buf);

  while (guildie) {
   if (STATUS_FLAGGED(guildie, STATUS_MEMBER) && STATUS_FLAGGED(guildie, STATUS_SPONSORLIST)) {
     if (STATUS_FLAGGED(guildie, STATUS_SUBRANK))
       sprintf(buf,"%s%-30s %s [%s]\r\n", buf,
         guildie->name,
             ((guildie->rank_num != 0) ? guildie->rank->name : "Unranked"),  
              guildie->subrank);
     else
       sprintf(buf, "%s%-30s %s\r\n", buf,
         guildie->name,
         ((guildie->rank_num != 0) ? guildie->rank->name : "Unranked"));
   }
   guildie = guildie->next;
  }

  page_string(ch->desc,buf,1);

  if (not_really_a_member) {
    sprintf(buf, "%s %s", GET_NAME(ch), argument);
    do_deguild(ch, buf, 0, 0);
  }
}

ACMD(do_glist)
{
  char guild_name[MAX_INPUT_LENGTH];
  char guildie_name[MAX_INPUT_LENGTH];
  int guild_id, i, is_gl = 0;
  struct char_guild_element *element;
  struct guildie_info *guildie;
  int not_really_a_member = 0;
  int found = 0;

  two_arguments(argument, guild_name, guildie_name);
  
  if (!*guild_name) {
    send_to_char("Usage: glist <guild number/name> [player name]\r\n", ch);
    return;
  }

  guild_id = atoi(guild_name);

  element = GET_CHAR_GUILDS(ch);
  while (element) {
   if (element->guild->id == guild_id) break;
   if (!strncasecmp(element->guild->name, guild_name, strlen(guild_name))) break; 
   element = element->next;
  }

  /* if you're not in that guild, element == NULL. this will cause a
     segfault further down... */
  if (!element) {
    if (GET_LEVEL(ch) < LVL_GRGOD) {
      send_to_char("You are not a part of this guild.\r\n", ch);
      return;
    } else
      not_really_a_member = 1;
  }

  if (not_really_a_member) {
    do_seekguild(ch, argument, 0, 0);
    element = GET_CHAR_GUILDS(ch);
    while (element) {
      if (element->guild->id == guild_id) break;
      if (!strncasecmp(element->guild->name, guild_name, strlen(guild_name))) break; 
      element = element->next;
    }    
  }

  if (element) {
    if (STATUS_FLAGGED(element->guildie, STATUS_GL) || GET_LEVEL(ch) >= LVL_GRGOD)
      is_gl = 1;
  } else {
    send_to_char("*oops* It seems something went wrong.\r\n", ch);
    return;
  }

  if (GLD_FLAGGED(element->guild, GLD_NOGLIST) && !is_gl) {
   send_to_char("You can't do that.\r\n", ch);
   return;
  }

  guildie = element->guild->guildies;

  sprintf(buf,"\r\n\\c14%s guildies\\c00\r\n", element->guild->name);
  sprintf(buf,"%s\\c14=========", buf);

  for (i = 1; i <= strlen(element->guild->name); i++)
    sprintf(buf, "%s=", buf);

  sprintf(buf, "%s\\c00\r\n\r\n", buf);

  while (guildie) 
  {
    if( !*guildie_name || !strncasecmp(guildie->name, guildie_name, strlen(guildie->name)) )
    {
      found = TRUE;
      if (STATUS_FLAGGED(guildie, STATUS_MEMBER)) 
      {
	if (!STATUS_FLAGGED(guildie, STATUS_SUBRANK))
	  sprintf(buf,"%s%-30s %s\r\n", buf, 
	   ((GLD_FLAGGED(element->guild, GLD_SECRETIVE) && !is_gl) ? "Someone" : guildie->name),
           ((guildie->rank_num != 0) ? guildie->rank->name : "Unranked"));                 
	else
	  sprintf(buf,"%s%-30s %s [%s]\r\n", buf,
	    ((GLD_FLAGGED(element->guild, GLD_SECRETIVE) && !is_gl)? "Someone" : guildie->name),
	    ((guildie->rank_num != 0) ? guildie->rank->name : "Unranked"),  
	    guildie->subrank);
      }
    }
    guildie = guildie->next;
  }

  if( !found )
    sprintf( buf, "%s is not a member of the %s guild.", guildie_name, element->guild->name );
  page_string(ch->desc,buf,1);

  if (not_really_a_member) {
    sprintf(buf, "%s %s", argument, "off");
    do_seekguild(ch, buf, 0, 0);
  }
}

ACMD(do_gdesc)
{
  struct guild_info *g;
  char guild_name[MAX_INPUT_LENGTH];
  int guild_id;
  struct char_guild_element *element;

  one_argument(argument, guild_name);
  
  if (!*guild_name) {
    send_to_char("Usage: gdesc <guild number/name>\r\n", ch);
    return;
  }

  guild_id = atoi(guild_name);

  g = guilds_data;

  while (g) {
   if (g->id == guild_id) break;
   if (!strncasecmp(g->name, guild_name, strlen(guild_name))) break; 
   g = g->next;
  }

  if (!g) {
   send_to_char("No such guild !\r\n", ch);
   return;
  }

  if (GET_LEVEL(ch) < LVL_GRGOD) {

   element = GET_CHAR_GUILDS(ch);
   while (element) {
    if (element->guild == g) break;
    element = element->next;
   }

   if (!element) {
    send_to_char("You are not a part of this guild.\r\n", ch);
    return;
   }

   if (!STATUS_FLAGGED(element->guildie, STATUS_GL)) {
    send_to_char("You are not a guildleader of this guild !\r\n", ch);
    return;
   }
  
  }

  send_to_char("Don't forget to use saveguild after changing the guild description.\r\n", ch);
  send_to_char("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n", ch);
  send_to_char("Old description:\r\n", ch);
  send_to_char(g->description, ch);
  
  ch->desc->str = &(g->description);
  ch->desc->max_str = MAX_MESSAGE_LENGTH;
  ch->desc->backstr = str_dup(g->description);
  ch->desc->current_text_line = count_newlines(ch->desc->str) + 1;
  SET_BIT(PLR_FLAGS(ch), PLR_WRITING); 
}

ACMD(do_greq)
{
  struct guild_info *g;
  char guild_name[MAX_INPUT_LENGTH];
  int guild_id;
  struct char_guild_element *element;

  one_argument(argument, guild_name);
  
  if (!*guild_name) {
    send_to_char("Usage: greq <guild number/name>\r\n", ch);
    return;
  }

  guild_id = atoi(guild_name);

  g = guilds_data;

  while (g) {
   if (g->id == guild_id) break;
   if (!strncasecmp(g->name, guild_name, strlen(guild_name))) break; 
   g = g->next;
  }

  if (!g) {
   send_to_char("No such guild !\r\n", ch);
   return;
  }

  if (GET_LEVEL(ch) < LVL_GRGOD) {

   element = GET_CHAR_GUILDS(ch);
   while (element) {
    if (element->guild == g) break;
    element = element->next;
   }

   if (!element) {
    send_to_char("You are not a part of this guild.\r\n", ch);
    return;
   }

   if (!STATUS_FLAGGED(element->guildie, STATUS_GL)) {
    send_to_char("You are not a guildleader of this guild !\r\n", ch);
    return;
   }
  
  }

  send_to_char("Don't forget to use saveguild after changing the guild requirements.\r\n", ch);
  send_to_char("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n", ch);
  send_to_char("Old requirements:\r\n", ch);
  send_to_char(g->requirements, ch);
  
  ch->desc->str = &(g->requirements);
  ch->desc->max_str = MAX_MESSAGE_LENGTH;
  ch->desc->backstr = str_dup(g->requirements);
  ch->desc->current_text_line = count_newlines(ch->desc->str) + 1;
  SET_BIT(PLR_FLAGS(ch), PLR_WRITING); 
}

ACMD(do_newghelp)
{
  char guild_name[MAX_INPUT_LENGTH], arg1[MAX_INPUT_LENGTH+1];
  char arg2[MAX_INPUT_LENGTH+1];
  int guild_id, num;
  struct char_guild_element *element;
  struct ghelp_info *ghelp, *ghelp2;

  half_chop(argument, guild_name, buf);
  half_chop(buf, arg1, arg2);
  
  if (!*guild_name) {
   send_to_char("Usages: 'ghelp <guild name/num>'.\r\n", ch);
   send_to_char("        'ghelp <guild name/num> <ghelp keyword>'.\r\n", ch);
   send_to_char("        'ghelp <guild name/num> write <existing/new ghelp keyword>'.\r\n", ch);
   send_to_char("        'ghelp <guild name/num> remove <existing ghelp keyword>'.\r\n", ch);
   return;
  }

  guild_id = atoi(guild_name);

  element = GET_CHAR_GUILDS(ch);
  while (element) {
   if (element->guild->id == guild_id) break;
   if (!strncasecmp(element->guild->name, guild_name, strlen(guild_name))) break;
   element = element->next;
  }
  
  if (!element || STATUS_FLAGGED(element->guildie, STATUS_SEEKING)) {
   send_to_char("You are not a part of such a guild.\r\n", ch);
   return;
  }

  ghelp = element->guild->ghelp;

  if (!*arg1) {
    sprintf(buf, "\r\n    &c%s ghelp entries list\r\n", element->guild->name);
    sprintf(buf, "%s    &R===================", buf);

    for (num = 1; num <= strlen(element->guild->name); num++)
     sprintf(buf, "%s=", buf);

    sprintf(buf, "%s&n\r\n\r\n", buf);
    while (ghelp) {
     sprintf(buf, "%s&G%s&n\r\n", buf, ghelp->keyword);
     ghelp = ghelp->next;
    }
    page_string(ch->desc, buf, 1);
    return;
  }

  if (!*arg2) {
   while (ghelp) {
    if (!strncasecmp(ghelp->keyword, arg1, strlen(ghelp->keyword))) break;
        ghelp = ghelp->next;
   }
   if (!ghelp) {
    ghelp = element->guild->ghelp;
    while (ghelp) {
     if (!strncasecmp(ghelp->keyword, arg1, strlen(arg1))) break;
     ghelp = ghelp->next;
    }
   }
   if (!ghelp) {
        send_to_char("No such ghelp entry.\r\n", ch);
        return;
   }
   page_string(ch->desc, ghelp->entry, 1);
   return;
  } 

  if (is_abbrev(arg1, "write")) {
   if (!STATUS_FLAGGED(element->guildie, STATUS_GL)) {
    send_to_char("You can't write ghelp.\r\n", ch);
    return;
   }
   if (!*arg2) {
    send_to_char("Wanna write which ghelp ?\r\n", ch);
    return;
   }
   while (ghelp) {
    if (!strncasecmp(ghelp->keyword, arg2, strlen(arg2))) break;
    ghelp = ghelp->next;
   }
   if (ghelp) { 
    send_to_char("Normal editing options apply. Use 'saveguild' afterwards.\r\n", ch);
    send_to_char("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n", ch);
    ch->desc->str = &(ghelp->entry);
    ch->desc->max_str = MAX_MESSAGE_LENGTH;
    ch->desc->backstr = str_dup(ghelp->entry);
    ch->desc->current_text_line = count_newlines(ch->desc->str) + 1;
    SET_BIT(PLR_FLAGS(ch), PLR_WRITING); 
    return;
   }
   ghelp = calloc(1,sizeof(struct ghelp_info)); 
   ghelp->keyword = str_dup(arg2);
   ghelp->next = element->guild->ghelp;
   element->guild->ghelp = ghelp;
   send_to_char("Normal editing options apply. Use 'saveguild' afterwards.\r\n", ch);
   send_to_char("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n", ch);
   ch->desc->str = &(ghelp->entry);
   ch->desc->max_str = MAX_MESSAGE_LENGTH;
   ch->desc->backstr = str_dup(ghelp->entry);
   ch->desc->current_text_line = count_newlines(ch->desc->str) + 1;
   SET_BIT(PLR_FLAGS(ch), PLR_WRITING); 
   return;
  }

  if (is_abbrev(arg1, "remove")) {
   if (!STATUS_FLAGGED(element->guildie, STATUS_GL)) {
    send_to_char("You can't remove ghelp.\r\n", ch);
    return;
   }
   if (!*arg2) {
    send_to_char("Wanna remove which ghelp ?\r\n", ch);
    return;
   }
   while (ghelp) {
    if (!strncasecmp(ghelp->keyword, arg2, strlen(arg2))) break;
    ghelp = ghelp->next;
   }
   if (!ghelp) {
    send_to_char("No such ghelp in that guild, sorry.\r\n", ch);
    return;
   }
   if (element->guild->ghelp == ghelp) { 
    element->guild->ghelp = ghelp->next;
    send_to_char("Done.\r\n", ch);
    return;
   }
   ghelp2 = element->guild->ghelp;
   while (ghelp2->next != ghelp)
    ghelp2 = ghelp2->next;
   ghelp2->next = ghelp->next;
   send_to_char("Done.\r\n", ch);
   return;
  }
  send_to_char("Huh ?!\r\n", ch);
}

ACMD(do_saveguild)
{
  char arg1[100];
  int num;
  struct guild_info *g;
  struct char_guild_element *element;

  one_argument(argument, arg1);

  if (!*arg1)
  {  
    send_to_char("Usage: saveguild <guild name/id>\r\n", ch);
    return;
  }

  g = guilds_data;
  num = atoi(arg1);

  while (g) {
   if (g->id == num) break;
   if (!strncasecmp(g->name, arg1, strlen(arg1))) break;
   g = g->next;
  }

  if (!g) {
   send_to_char("No such guild !\r\n", ch);
   return;
  }

  element = GET_CHAR_GUILDS(ch);

  while (element) {
   if (element->guild == g) break;
   element = element->next;
  }

  if (!element && GET_LEVEL(ch) < LVL_GRGOD) {
   send_to_char("You can't do that !\r\n", ch);
   return;
  }

  if (GET_LEVEL(ch) < LVL_GRGOD)
   if (!PERM_FLAGGED(element->guildie, PERM_GOSSIP_WRITE)) {
    send_to_char("You can't do that !\r\n", ch);
    return;
   }

  save_guild(g);

  send_to_char("Done.\r\n", ch);

}

ACMD(do_gdeposit)
{
  char arg1[100], arg2[100];
  int amount, num;
  struct guild_info *g;
  struct char_guild_element *element;

  two_arguments(argument, arg1, arg2);

  if (!*arg1 || !*arg2)
  {  
    send_to_char("Usage: gdeposit <amount> <guild name/id>\r\n", ch);
    return;
  }

  g = guilds_data;
  num = atoi(arg2);
  amount = atoi(arg1);

  while (g) {
   if (g->id == num) break;
   if (!strncasecmp(g->name, arg2, strlen(arg2))) break;
   g = g->next;
  }

  if (!g) {
   send_to_char("No such guild !\r\n", ch);
   return;
  }

  element = GET_CHAR_GUILDS(ch);

  while (element) {
   if (element->guild == g) break;
   element = element->next;
  }

  if (!element && GET_LEVEL(ch) < LVL_GRGOD) {
   send_to_char("You are not a member of this guild !\r\n", ch);
   return;
  }

  if (GLD_FLAGGED(g, GLD_NOBANK)) {
   send_to_char("This guild has no bank account.\r\n", ch);
   return;
  }

  if (amount <= 0) {
    send_to_char("Heh heh heh ... we are jolly funny today, eh?\r\n", ch);
    return;
  }
  if ((GET_GOLD(ch) < amount) && (IS_NPC(ch) || (GET_LEVEL(ch) < LVL_GOD))) {
    send_to_char("You don't have that many coins!\r\n", ch);
    return;
  }
  send_to_char("Done.\r\n", ch);
  if (IS_NPC(ch) || (GET_LEVEL(ch) < LVL_GOD))
    GET_GOLD(ch) -= amount;
  g->gold += amount;

  if (element) 
   element->guildie->deposited += amount;

  save_guild(g);

}


ACMD(do_gwithdraw)
{
  char arg1[100], arg2[100];
  int amount, num;
  struct guild_info *g;
  struct char_guild_element *element;

  two_arguments(argument, arg1, arg2);

  if (!*arg1 || !*arg2)
  {  
    send_to_char("Usage: gwithdraw <amount> <guild name/id>\r\n", ch);
    return;
  }

  g = guilds_data;
  num = atoi(arg2);
  amount = atoi(arg1);

  while (g) {
   if (g->id == num) break;
   if (!strncasecmp(g->name, arg2, strlen(arg2))) break;
   g = g->next;
  }

  if (!g) {
   send_to_char("No such guild !\r\n", ch);
   return;
  }

  element = GET_CHAR_GUILDS(ch);

  while (element) {
   if (element->guild == g) break;
   element = element->next;
  }

  if (!element && GET_LEVEL(ch) < LVL_GRGOD) {
   send_to_char("You are not a member of this guild !\r\n", ch);
   return;
  }

  if (GLD_FLAGGED(g, GLD_NOBANK)) {
   send_to_char("This guild has no bank account.\r\n", ch);
   return;
  }

  if (GET_LEVEL(ch) < LVL_GRGOD)  
   if(!PERM_FLAGGED(element->guildie, PERM_WITHDRAW)) {
    send_to_char("You can't withdraw from that account.\r\n", ch);
    return;
   }

  if (amount <= 0) {
    send_to_char("Heh heh heh ... we are jolly funny today, eh?\r\n", ch);
    return;
  }
  if (g->gold < amount) {
    send_to_char("Not enough gold in that account.\r\n", ch);
    return;
  }
  send_to_char("Done.\r\n", ch);
  g->gold -= amount;
  GET_GOLD(ch) += amount;

  if (element) 
   element->guildie->withdrew += amount;

  save_guild(g);

}

ACMD(do_gskillset)
{
  struct char_data *vict;
  char name[100], buf2[100], buf[100], help[MAX_STRING_LENGTH];
  int skill, value, qend;
  struct gskill_info *gskill = NULL;
  struct char_guild_element *element;

  element = GET_CHAR_GUILDS(ch);

  argument = one_argument(argument, name);

  if (!*name) {			/* no arguments. print an informative text */
    send_to_char("Syntax: gskillset <name> '<skill>' <value>\r\n", ch);
    strcpy(help, "Skill being one of the following:\n\r");
    while (element) {
     if (PERM_FLAGGED(element->guildie, PERM_GSKILLSET)) {
      gskill = element->guild->gskills;
      while (gskill) {
       sprintf(help, "%s%s [maximum set: %d]\r\n", help, spells[gskill->skill],
                    gskill->maximum_set);
       gskill = gskill->next;
      }
     }
     element = element->next;
    }
    page_string(ch->desc,help,1);
    send_to_char("\r\n", ch);
    return;
  }
       
  if (!(vict = get_char_vis(ch, name))) {
    send_to_char("No such person around.\r\n", ch);
    return;
  }
  skip_spaces(&argument);

  /* If there is no chars in argument */
  if (!*argument) {
    send_to_char("Gskill name expected.\n\r", ch);
    return;
  }
  if (*argument != '\'') {
    send_to_char("Gskill must be enclosed in: ''\n\r", ch);
    return;
  }
  /* Locate the last quote && lowercase the magic words (if any) */

  for (qend = 1; *(argument + qend) && (*(argument + qend) != '\''); qend++)
    *(argument + qend) = LOWER(*(argument + qend));

  if (*(argument + qend) != '\'') {
    send_to_char("Gskill must be enclosed in: ''\n\r", ch);
    return;
  }
  strcpy(help, (argument + 1));
  help[qend - 1] = '\0';
  if ((skill = find_skill_num(help)) <= 0) {
    send_to_char("Unrecognized skill.\n\r", ch);
    return;
  }

  element = GET_CHAR_GUILDS(ch);
  while (element) {
   if (PERM_FLAGGED(element->guildie, PERM_GSKILLSET)) {
    gskill = element->guild->gskills;
    while (gskill) {
     if (gskill->skill == skill) break;
     gskill = gskill->next;
    }
   }
   if (gskill) {
    if (!STATUS_FLAGGED(element->guildie, STATUS_GL) && !GET_SKILL(ch, skill)) {
        send_to_char("You don't know this gskill yourself.\r\n", ch);
        return;
    }
    break;
   }
   element = element->next;
  }

  if (!gskill) {
    send_to_char("You can't gskillset this skill.\r\n", ch);
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
  if (value > gskill->maximum_set) {
   sprintf(buf, "Max value for learned is %d.\r\n", gskill->maximum_set);
   send_to_char(buf, ch);
   return;
  }
  if (IS_NPC(vict)) {
   send_to_char("You can't set NPC skills.\n\r", ch);
   return;
  }
  sprintf(buf2, "(GC) %s gskillseted %s's %s to %d.", GET_NAME(ch), GET_NAME(vict),
      spells[skill], value);
  mudlog(buf2, BRF, -1, TRUE);
  mlog(buf2); 

  SET_SKILL(vict, skill, value);

  sprintf(buf2, "You gskillseted %s's %s to %d.\n\r", GET_NAME(vict),
      spells[skill], value);
  send_to_char(buf2, ch);
}

ACMD(do_gload)
{
  struct obj_data *obj;
  int number, r_num;
  struct char_guild_element *element = NULL;
  struct gequip_info *gequip = NULL;

  one_argument(argument, buf);

  if (!*buf) {
    send_to_char("Usage: gload <gequip vnum>\r\n", ch);
    return;
  }

  if ((number = atoi(buf)) < 0) {
    send_to_char("A NEGATIVE number??\r\n", ch);
    return;
  }

  element = GET_CHAR_GUILDS(ch);

  while (element) {
   gequip = NULL;
   if (STATUS_FLAGGED(element->guildie, STATUS_GL)) {
    gequip = element->guild->gequip;
    while (gequip) {
     if (gequip->vnum == number) break;
     gequip = gequip->next;
    }
   }
   if (gequip) break;
   element = element->next;
  }

   if (!gequip) {
    send_to_char("You can't gload this object.\r\n", ch);
    return;
   }

   if ((r_num = real_object(number)) < 0) {
    send_to_char("There is no object with that number.\r\n", ch);
    return;
   }

   obj = read_object(r_num, REAL);
   obj_to_char(obj, ch);
   act("$n makes a strange magical gesture.", TRUE, ch, 0, 0, TO_ROOM);
   act("$n has created $p!", FALSE, ch, obj, 0, TO_ROOM);
   act("You create $p.", FALSE, ch, obj, 0, TO_CHAR);
    
   sprintf(buf,"%s has gloaded obj %d (%s).", GET_NAME(ch), number, obj->short_description);
   xlog(SYS_LOAD, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), buf, TRUE);
 
}

ACMD(do_gdt)
{
  struct char_guild_element *element;

  if (!*argument) {
    send_to_char("What do you want to say ?\r\n", ch);
    return;
  }

  element = GET_CHAR_GUILDS(ch);
  while (element) {
   if (STATUS_FLAGGED(element->guildie, STATUS_GL)) break;
   element = element->next;
  }

  if (!element) {
   element = GET_CHAR_GUILDS(ch);
   while (element) {
    if (GLD_FLAGGED(element->guild, GLD_EXCLUSIVE) && STATUS_FLAGGED(element->guildie, STATUS_MEMBER)) break;
    element = element->next;
   }
  }

  if (!element)
   element = GET_CHAR_GUILDS(ch);

  if (!element) {
   send_to_char("You are not even a part of one guild.\r\n", ch);
   return;
  }

  if (!STATUS_FLAGGED(element->guildie, STATUS_MEMBER)) {
   send_to_char("You are not even a part of one guild.\r\n", ch);
   return;
  }

  do_guild_channel(ch, element, argument);
}

ACMD(do_newdisguise)
{
  int i=0;
  struct char_guild_element *element, *e = NULL, *e2 = NULL;
  struct char_data *p;

  skip_spaces(&argument);
   
  if (!IS_AFFECTED(ch, AFF_DREAMING)) {
    if (!can_use_gskill(ch, SKILL_DISGUISE) && !PLR_FLAGGED(ch, PLR_NOGAIN) &&
         !GET_DISGUISED(ch)) {
      send_to_char("You can't use this skill.\r\n",ch);
      return;
    }

    if (!GET_SKILL(ch,SKILL_DISGUISE)) {
      send_to_char("Sorry. You don't know how!\r\n",ch);
      return;
    }
  }
  else if (!GET_DISGUISED(ch) && (GET_SKILL(ch, SKILL_DREAM) < 85)) {
    send_to_char("You're not experienced enough at dreaming for this.\r\n", ch);
    return;
  }
  if (!*argument) {
    send_to_char("Usage: Disguise [name] / Disguise off\r\n",ch);
    return;
  }

  if (!strncasecmp(argument,"off",strlen(argument))) {
    if (!GET_DISGUISED(ch)) {
      send_to_char("But you aren't disguised!\r\n",ch);
      return;
    }
    GET_DISGUISED(ch) = 0;
    free(GET_DNAME(ch));
    free(GET_DTITLE(ch));

    delete_char_all_guilds(ch);
    fetch_char_guild_data(ch);
    send_to_char("OK, all your original data is restored. Enjoy.\r\n",ch);
    
    update_p_table(ch);

    return;
  }


 if (GET_DISGUISED(ch)) {
   send_to_char("You're already disguised. Un-disguise first, bonehead!\r\n",ch);
   return;
 } /* Fnehul */
  for (i = strlen(argument)-1; (i > 0) && (argument[i] == ' '); i --)
    if ((argument[i]) && (argument[i] == ' ')) /* Don't change \0s! */
      argument[i] = '\0';
   
  // Check for color code cheating bastards now...
  for (i = 0; i < strlen(argument); i ++) 
  {
    if ((argument[i] == '\\') || (argument[i] == '&')) 
    {
      send_to_char("Sorry, but \\'s and &'s are not allowed in disguise names.\r\n", ch);
      return;
    }
  }

  p = NULL;
  if (get_char_vis(ch,argument) || (NULL != (p = xmlload_char(argument)))) {
    send_to_char("Name already exists, please choose another one!\r\n",ch);
    if (p != NULL)
      free_char(p);
    return;
  }
  
  if ((strlen(argument) < 2) || (strlen(argument) > MAX_NAME_LENGTH)) {
    sprintf(buf, "%s is not a valid name. Try again.\r\n", argument);
    send_to_char(buf,ch);
    /* free(arg1); arg1 isn't a pointer, but an array, can't free that */
    return;
  }
  if (',' == argument[(strlen(argument)-1)])
    argument[(strlen(argument)-1)] = '\0';	/* Remove 'short curcuit' character */

  GET_DNAME(ch)   = str_dup(CAP(argument));
  GET_DTITLE(ch)  = str_dup(GET_RTITLE(ch));

  element = GET_CHAR_GUILDS(ch);
  while (element) {
    if (GLD_FLAGGED(element->guild, GLD_DISGUISE_STAY)) {
      e2 = calloc(1,sizeof(struct char_guild_element));
      e2->guild = element->guild;
      e2->guildie = element->guildie;
      e2->next = e;
      e = e2;
    }
    element = element->next;
  }   
            
  GET_DISGUISED(ch) = TRUE;
  remove_char_guild_data(ch);
  GET_CHAR_GUILDS(ch) = e;
  
  while (e) {
    e->guildie->ch = ch;
    e = e->next;
  }
  update_p_table(ch);
  
  sprintf(buf, "Okay, you've disguised yourself as %s!\r\n",GET_NAME(ch));
  send_to_char(buf, ch);
  send_to_char("Don't forget to give yourself a nice title and a description now!\r\n",ch); 
  update_p_table(ch);
  alog("(DISGUISE): %s has disguised into name %s.",GET_RNAME(ch),GET_NAME(ch));
}

ACMD(do_guilddisguise)
{
  char arg1[100] = "\0", arg2[100] = "\0", arg3[100] = "\0";
  int num;
  struct guild_info *g = NULL;
  struct char_guild_element *element, *e2;
  struct guildie_info *guildie = NULL;
  struct rank_info *rank = NULL;

  if (!GET_SKILL(ch,SKILL_G_DISGUISE)) {
      send_to_char("Sorry. You don't know how!\r\n",ch);
      return;
  }

  if (!can_use_gskill(ch, SKILL_G_DISGUISE)) {
   send_to_char("You can't use this skill.\r\n", ch);
   return;
  }

  if (!GET_DISGUISED(ch)) {
   send_to_char("You need to disguise first.\r\n", ch);
   return;
  }

  if (argument && *argument)
    argument = one_argument(argument, arg1);
  if (argument && *argument)
    argument = one_argument(argument, arg2);
  if (argument && *argument) {
    skip_spaces(&argument);
    strcpy(arg3, argument);
  }

  if (!*arg1) {
    send_to_char("To which guild ? Use 'guilddisguise <guild name/num> <rank num> <subrank>.\r\n", ch);
    return;
  }

  num = atoi(arg1);
  g = guilds_data;

  while (g) {
   if (g->id == num) break;
   if (!strncasecmp(g->name, arg1, strlen(arg1))) break;
   g = g->next;
  }

  if (!g) {
   send_to_char("No such guild.\r\n", ch);
   return;
  }

  if (get_guild(ch, num)) {
    send_to_char("You are already a member of this guild!\r\n", ch);
    return;
  }

  element = GET_CHAR_GUILDS(ch);

  if (GLD_FLAGGED(g, GLD_EXCLUSIVE))
    while (element) {
      if (GLD_FLAGGED(element->guild, GLD_EXCLUSIVE)) {
    send_to_char("You can't join more than one exclusive guild.\r\n", ch);
    return;
      }
      element = element->next;
    }

   guildie = calloc(1,sizeof(struct guildie_info));  
   e2 = calloc(1,sizeof(struct char_guild_element));

   e2->guild = g;  
   e2->guildie = guildie;  
   e2->next = GET_CHAR_GUILDS(ch);  
   GET_CHAR_GUILDS(ch) = e2;

   guildie->next = g->guildies;  
   g->guildies = guildie;
  
   SET_BIT(guildie->status, STATUS_MEMBER);  
   SET_BIT(guildie->status, STATUS_CHAN_ON);
   guildie->idnum = GET_IDNUM(ch);  
   guildie->name = str_dup(GET_NAME(ch));   
   guildie->ch = ch;

   if (*arg2) {
    num = atoi(arg2);
    rank = g->ranks;
    while (rank) {
     if (rank->num == num) break;
     rank = rank->next;
    }
    if (!rank) {
     send_to_char("No such rank in the guild.\r\n", ch);
     return;
    }
    guildie->rank_num = num;
    guildie->rank = rank;
   }

   if (*arg3) {
    guildie->subrank = str_dup(arg3);
    SET_BIT(guildie->status, STATUS_SUBRANK);
   }

   send_to_char("Done.\r\n", ch);
   alog("(DISGUISE): %s (false name %s) has disguised into guild: %s%s%s%s%s.",
    GET_RNAME(ch),GET_NAME(ch),g->name,
    (rank  ? ", rank: "        : ""),
    (rank  ? rank->name        : ""),
    (*arg3 ? ", subrank: "     : ""),
    (*arg3 ? guildie->subrank  : "")
    );
}

/* For Tower Warders */
ACMD(do_twt)
{
  char buf1[MAX_STRING_LENGTH];
  struct guildie_info *guildie;
  struct char_guild_element *element;

  skip_spaces(&argument);
  
  element = GET_CHAR_GUILDS(ch);
  while (element) {
   if (element->guild->id == 3 && (STATUS_FLAGGED(element->guildie, STATUS_WARDER)
       || STATUS_FLAGGED(element->guildie, STATUS_GL))) 
     break;
   element = element->next;
  }

  if (!element) {
   send_to_char("You are not a Tower Warder.\r\n", ch);
   return;
  }

  if (!STATUS_FLAGGED(element->guildie, STATUS_MEMBER)) {
   send_to_char("Huh ?!?\r\n", ch);
   return;
  }
 
  if (IS_AFFECTED(ch,AFF_GAGED)) {
    send_to_char("Not while gagged!\r\n",ch);
    return;
  }

  if (!STATUS_FLAGGED(element->guildie, STATUS_CHAN_ON)) {
    send_to_char("You can't use this channel while it's off.\r\n", element->guildie->ch);
    return;
  }

  if (!*argument){
    send_to_char("What do you want to say on this channel?\r\n",ch);
    return;
  }

  sprintf(buf1,"[twt] %s %s: %s\r\n", 
         ((element->guildie->rank_num != 0) ? element->guildie->rank->name : "Unranked"), 
         GET_NAME(ch), argument);  
          
  guildie = element->guild->guildies;
  while (guildie) {
   if (STATUS_FLAGGED(guildie, STATUS_CHAN_ON) && guildie->ch && !PLR_FLAGGED(guildie->ch, PLR_WRITING))
    if (STATUS_FLAGGED(guildie, STATUS_WARDER) || STATUS_FLAGGED(guildie, STATUS_GL))
     send_to_char(buf1, guildie->ch);
   guildie = guildie->next;
  }
}

/* For Tower AS */
ACMD(do_tast)
{
  char buf1[MAX_STRING_LENGTH];
  struct guildie_info *guildie;
  struct char_guild_element *element;

  skip_spaces(&argument);
  
  element = GET_CHAR_GUILDS(ch);
  while (element) {
   if (element->guild->id == 3 && (!STATUS_FLAGGED(element->guildie, STATUS_WARDER) ||
       STATUS_FLAGGED(element->guildie, STATUS_GL)))
     break;
   element = element->next;
  }

  if (!element) {
   send_to_char("You are not a Tower Aes Sedai.\r\n", ch);
   return;
  }

  if (!STATUS_FLAGGED(element->guildie, STATUS_MEMBER)) {
   send_to_char("Huh ?!?\r\n", ch);
   return;
  }

  if (IS_AFFECTED(ch,AFF_GAGED)) {
    send_to_char("Not while gagged!\r\n",ch);
    return;
  }

  if (!STATUS_FLAGGED(element->guildie, STATUS_CHAN_ON)) {
    send_to_char("You can't use this channel while it's off.\r\n", element->guildie->ch);
    return;
  }

  if (!*argument){
    send_to_char("What do you want to say on this channel?\r\n",ch);
    return;
  }

  sprintf(buf1,"[tast] %s %s: %s\r\n", 
         ((element->guildie->rank_num != 0) ? element->guildie->rank->name : "Unranked"), 
         GET_NAME(ch), argument);  
          
  guildie = element->guild->guildies;
  while (guildie) {
   if (STATUS_FLAGGED(guildie, STATUS_CHAN_ON) && guildie->ch && !PLR_FLAGGED(guildie->ch, PLR_WRITING))
    if (!STATUS_FLAGGED(guildie, STATUS_WARDER) || STATUS_FLAGGED(guildie, STATUS_GL))
     send_to_char(buf1, guildie->ch);
   guildie = guildie->next;
  }
}

/* For Rebel Warders */
ACMD(do_rwt)
{
  char buf1[MAX_STRING_LENGTH];
  struct guildie_info *guildie;
  struct char_guild_element *element;

  skip_spaces(&argument);
  
  element = GET_CHAR_GUILDS(ch);
  while (element) {
   if (element->guild->id == 5 && (STATUS_FLAGGED(element->guildie, STATUS_WARDER) ||
       STATUS_FLAGGED(element->guildie, STATUS_GL)))
     break;
   element = element->next;
  }

  if (!element) {
   send_to_char("You are not a Rebel Warder.\r\n", ch);
   return;
  }

  if (!STATUS_FLAGGED(element->guildie, STATUS_MEMBER)) {
   send_to_char("Huh ?!?\r\n", ch);
   return;
  }

  if (IS_AFFECTED(ch,AFF_GAGED)) {
    send_to_char("Not while gagged!\r\n",ch);
    return;
  }

  if (!STATUS_FLAGGED(element->guildie, STATUS_CHAN_ON)) {
    send_to_char("You can't use this channel while it's off.\r\n", element->guildie->ch);
    return;
  }

  if (!*argument){
    send_to_char("What do you want to say on this channel?\r\n",ch);
    return;
  }

  sprintf(buf1,"[rwt] %s %s: %s\r\n", 
         ((element->guildie->rank_num != 0) ? element->guildie->rank->name : "Unranked"), 
         GET_NAME(ch), argument);  
          
  guildie = element->guild->guildies;
  while (guildie) {
   if (STATUS_FLAGGED(guildie, STATUS_CHAN_ON) && guildie->ch && !PLR_FLAGGED(guildie->ch, PLR_WRITING))
    if (STATUS_FLAGGED(guildie, STATUS_WARDER) || STATUS_FLAGGED(guildie, STATUS_GL))
     send_to_char(buf1, guildie->ch);
   guildie = guildie->next;
  }
}

/* For Rebel AS */
ACMD(do_rast)
{
  char buf1[MAX_STRING_LENGTH];
  struct guildie_info *guildie;
  struct char_guild_element *element;

  skip_spaces(&argument);
  
  element = GET_CHAR_GUILDS(ch);
  while (element) {
   if (element->guild->id == 5 && (!STATUS_FLAGGED(element->guildie, STATUS_WARDER) ||
       STATUS_FLAGGED(element->guildie, STATUS_GL)))
     break;
   element = element->next;
  }

  if (!element) {
   send_to_char("You are not a Rebel Aes Sedai.\r\n", ch);
   return;
  }

  if (!STATUS_FLAGGED(element->guildie, STATUS_MEMBER)) {
   send_to_char("Huh ?!?\r\n", ch);
   return;
  }

  if (IS_AFFECTED(ch,AFF_GAGED)) {
    send_to_char("Not while gagged!\r\n",ch);
    return;
  }

  if (!STATUS_FLAGGED(element->guildie, STATUS_CHAN_ON)) {
    send_to_char("You can't use this channel while it's off.\r\n", element->guildie->ch);
    return;
  }

  if (!*argument){
    send_to_char("What do you want to say on this channel?\r\n",ch);
    return;
  }

  sprintf(buf1,"[rast] %s %s: %s\r\n", 
         ((element->guildie->rank_num != 0) ? element->guildie->rank->name : "Unranked"), 
         GET_NAME(ch), argument);  
          
  guildie = element->guild->guildies;
  while (guildie) {
   if (STATUS_FLAGGED(guildie, STATUS_CHAN_ON) && guildie->ch && !PLR_FLAGGED(guildie->ch, PLR_WRITING))
    if (!STATUS_FLAGGED(guildie, STATUS_WARDER) || STATUS_FLAGGED(guildie, STATUS_GL))
     send_to_char(buf1, guildie->ch);
   guildie = guildie->next;
  }
}

void guildie_rename(struct char_data *ch, char *newname, int offline) {
  struct guild_info *g;
  struct guildie_info *guildie;

  if (!newname || !*newname || !ch) return;

  g = guilds_data;

  while (g) {
    guildie = g->guildies;
    while (guildie) {
      if (!offline) {
    if (guildie->ch && (guildie->ch == ch)) {
          free(guildie->name);
          guildie->name = str_dup(newname);
        }
      } else {
    if (guildie->name && GET_NAME(ch)) {
      if (strcasecmp(GET_NAME(ch), guildie->name) == 0) {
        free(guildie->name);
        guildie->name = str_dup(newname);
      }
    }
      }
      guildie = guildie->next;
    }
    g = g->next;
  }

  save_all_guilds();
}

int share_guild(struct char_data *ch1, struct char_data *ch2) {
  struct char_guild_element *el1, *el2;
  
  if (!ch1 || !ch2 || IS_NPC(ch1) || IS_NPC(ch2)) return 0;
  
  for (el1 = GET_CHAR_GUILDS(ch1);el1;el1 = el1->next) 
    for(el2 = GET_CHAR_GUILDS(ch2);el2;el2 = el2->next)
      if (el1->guild->id == el2->guild->id) return el1->guild->id;
  
  return 0;
}

ACMD(do_greceive)
{
  int r_num;
  struct obj_data *obj;
  struct char_guild_element *element;
  struct qp_info *qp;
  struct gequip_info *gequip;
  int bool = 1;

  element = GET_CHAR_GUILDS(ch);

  while (element)
  {
   if (element->guildie->qpadd)
   {
    qp = element->guildie->qpadd;
    while (qp) {
     GET_QP(ch) += qp->num;
     sprintf(buf, "&G%s has awarded you %ld questpoints.&n\r\n", qp->name, qp->num);
     send_to_char(buf, ch);
     element->guildie->qpadd = qp->next;
     free(qp);
     qp = element->guildie->qpadd;
    }
    if (GET_QP(ch) < 0)
     GET_QP(ch) = 0;
    bool = 0;
   }
   if (element->guildie->gequipsent)
   {
    gequip = element->guildie->gequipsent;
    while (gequip) {
     if ((r_num = real_object(gequip->vnum)) >= 0) {
      obj = read_object(r_num, REAL);
      obj_to_char(obj, ch);
      sprintf(buf, "&RYour GL has sent you %s.&n\r\n", obj->short_description);
      send_to_char(buf, ch);
     }
     element->guildie->gequipsent = gequip->next;
     free(gequip);
     gequip = element->guildie->gequipsent;
    }
    bool = 0;
   }
   element = element->next;
  }
  if (bool)
   send_to_char("&RNothing was sent to you by your GLs.&n\r\n", ch);
}
