/******************************************************************************
 * $Id: xmlcharsave.c,v 1.22 2003/12/02 08:15:36 fang Exp $
 *   
 * File: xmlcharsave.c
 *
 * Created: Mon Feb 21 2000 by Fredrik (fredrik_lannergren@yahoo.com)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * This file contains and should contain all that is needed for loading and
 * saving a players stats into an XML structure.
 * The expected position of each file is lib/plrfiles/[]/[].plr.
 * 
 *   $Log: xmlcharsave.c,v $
 *   Revision 1.22  2003/12/02 08:15:36  fang
 *   Cappen: Added mycolor to items displayed by the toggle command
 *
 *   Revision 1.21  2003/06/12 07:22:13  fang
 *   Changed testing for channeling ability as per HGL request. Fixed Room Seal to work as originally intended. Commented out ignorelist. Will fully remove once certain no problems will arise
 *
 *   Revision 1.20  2003/05/08 08:25:29  fang
 *   Crash bug in description load fixed
 *
 *   Revision 1.19  2003/03/10 08:17:49  fang
 *   *** empty log message ***
 *
 *   Revision 1.18  2003/03/04 23:43:21  fang
 *   fisch: added creation of multilist at boot time
 *
 *   Revision 1.17  2003/03/01 07:17:05  fang
 *   Fisch: Multiflag for authorized multies
 *
 *   Revision 1.16  2003/02/04 08:21:50  fang
 *   RPLog no longer logs stuff from OOC-characters.
 *
 *   Revision 1.15  2003/02/03 09:04:44  fang
 *   NPC-check in add_to_rplog
 *
 *   Revision 1.14  2003/01/24 09:17:42  fang
 *   Brands ar now saved in pfile...
 *
 *   Revision 1.13  2003/01/24 08:32:28  fang
 *   redemption of pracs for unused skills is now saved, and done totall when generating player_index.
 *
 *   Revision 1.12  2002/12/11 19:44:20  fang
 *   fisch: added "mail end" notification and removed guild info from score
 *
 *   Revision 1.11  2002/11/07 04:02:04  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.10  2002/10/23 04:17:19  fang
 *   fisch: "list board" to list boards with new messages added
 *
 *   Revision 1.9  2002/10/04 07:49:31  fischer
 *   fischer: added auto-redemption of skill set to name !UNUSED!
 *
 *   Revision 1.8  2002/09/22 21:43:50  fischer
 *   Removing mounts and horsetrain from the code
 *
 *   Revision 1.7  2002/08/30 21:23:09  fang
 *   Put in retainers.
 *
 *   Revision 1.6  2002/08/28 14:50:13  fang
 *   Drath: Player Extradesc bug fix #2
 *
 *   Revision 1.5  2002/08/27 21:29:39  fang
 *   Drath: Player Extra Descriptions
 *
 *   Revision 1.4  2002/08/13 22:24:20  fang
 *   Drath: MarkedMap hunter GSKILL
 *
 *   Revision 1.3  2002/07/29 22:18:56  fang
 *   AFF3 created and gskill update
 *
 *   Revision 1.2  2002/07/28 13:56:05  fang
 *   Removed dmaloc...just to test.
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.13  2002/07/12 03:37:33  fang
 *   Drath: MYCOLOR
 *
 *   Revision 1.12  2002/06/25 19:23:52  fang
 *   Drath: XLOG Commit #2
 *
 *   Revision 1.11  2002/06/20 03:07:10  fang
 *   Maz: RPTAG
 *
 *   Revision 1.10  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.9  2002/05/12 23:50:46  fang
 *   Added in IGNORE system, and removed AutoHotboots until the cap can be fixed for good. -Maz
 *
 *   Revision 1.8  2002/04/30 18:54:57  fang
 *   AC Cap - Request per CRE
 *
 *   Revision 1.7  2002/03/08 14:45:38  fang
 *   *** empty log message ***
 *
 *   Revision 1.6  2002/03/08 13:49:06  fang
 *   Added saving of tell and rplog buffer.
 *
 *   Revision 1.5  2002/02/27 06:03:56  fang
 *   Further modifications to disguise saving code.
 *
 *   Revision 1.4  2002/02/27 02:17:41  fang
 *   Fixed disguise so it saves.
 *
 *   Revision 1.3  2002/02/22 19:17:19  fang
 *   Fixed entity substitution up for properties.
 *
 *   Revision 1.2  2002/02/22 15:04:29  fang
 *   *** empty log message ***
 *
 *   Revision 1.63  2002/01/28 15:29:05  mud
 *   Customizable prompts..
 *
 *   Revision 1.62  2002/01/24 21:33:21  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.61.2.3  2002/01/24 03:20:36  mud
 *   Removed some further unescessary files (player.[ch], roomprog.[ch]
 *   old2new.c). Cleaned up some other stuff (guild stuff mostly).
 *
 *   Revision 1.61.2.2  2002/01/23 21:49:43  mud
 *   Removed objsave.[c|h] (old objsave stuff).
 *   Removed obj_file_elem and rent structs.
 *   Removed final vestiges of old guild system, except parts used
 *   in guild.[c|h] for backwards compatibility.
 *
 *   Revision 1.61.2.1  2002/01/21 01:23:42  mud
 *   Code cleanup. Aiel clans/socities gone. Lot of old guild code gone.
 *
 *   Revision 1.61  2002/01/15 19:17:26  mud
 *   Warning free...let's keep it that way...
 *
 *   Revision 1.60  2002/01/15 01:07:37  mud
 *   affected command now shows if a weave is [T]ied or [M]aintained.
 *
 *   Revision 1.59  2001/10/28 13:07:59  mud
 *   *** empty log message ***
 *
 *   Revision 1.58  2001/10/14 03:54:08  mud
 *   *** empty log message ***
 *
 *   Revision 1.57  2001/09/24 17:08:10  mud
 *   *** empty log message ***
 *
 *   Revision 1.56  2001/09/14 11:35:52  mud
 *   AC bug fixed...I hope. :)
 *
 *   Revision 1.55  2001/09/11 11:04:27  mud
 *   vanishing mana bug when unraveling weaves hopefulyl fixed
 *
 *   Revision 1.54  2001/09/06 15:12:18  mud
 *   *** empty log message ***
 *
 *   Revision 1.53  2001/09/06 15:05:01  mud
 *   *** empty log message ***
 *
 *   Revision 1.52  2001/09/06 06:30:59  mud
 *   *** empty log message ***
 *
 *   Revision 1.51  2001/09/06 05:41:19  mud
 *   *** empty log message ***
 *
 *   Revision 1.50  2001/08/23 14:32:23  mud
 *   *** empty log message ***
 *
 *   Revision 1.49  2001/08/23 14:07:30  mud
 *   *** empty log message ***
 *
 *   Revision 1.48  2001/08/19 08:58:39  mud
 *   *** empty log message ***
 *
 *   Revision 1.47  2001/08/13 22:18:55  mud
 *   *** empty log message ***
 *
 *   Revision 1.46  2001/08/13 15:49:21  mud
 *   *** empty log message ***
 *
 *   Revision 1.45  2001/08/12 10:28:29  mud
 *   *** empty log message ***
 *
 *   Revision 1.44  2001/08/05 12:25:42  mud
 *   *** empty log message ***
 *
 *   Revision 1.43  2001/07/31 00:40:10  mud
 *   *** empty log message ***
 *
 *   Revision 1.42  2001/07/30 08:02:09  mud
 *   *** empty log message ***
 *
 *   Revision 1.41  2001/07/25 15:25:30  mud
 *   *** empty log message ***
 *
 *   Revision 1.40  2001/07/25 14:09:19  mud
 *   *** empty log message ***
 *
 *   Revision 1.39  2001/07/23 07:00:59  mud
 *   *** empty log message ***
 *
 *   Revision 1.38  2001/07/23 06:59:57  mud
 *   *** empty log message ***
 *
 *   Revision 1.37  2001/07/09 19:48:02  mud
 *   *** empty log message ***
 *
 *   Revision 1.36  2001/07/09 17:47:29  mud
 *   *** empty log message ***
 *
 *   Revision 1.35  2001/07/08 17:20:26  mud
 *   *** empty log message ***
 *
 *   Revision 1.34  2001/07/08 17:12:47  mud
 *   *** empty log message ***
 *
 *   Revision 1.33  2001/07/06 20:45:00  mud
 *   Fixing memleak, I was told...
 *
 *   Revision 1.32  2001/05/22 09:07:08  mud
 *   DG-Scripts for Fang.
 *
 *   Revision 1.31  2001/05/17 11:21:09  mud
 *   Final residues of old pfile code should be swept away by now. No cleansing of the structures and such things though
 *
 *   Revision 1.30  2001/05/17 10:11:18  mud
 *   Pretitle code in and working.
 *
 *   Revision 1.29  2001/05/04 05:39:12  mud
 *   Multiple descs now works, without disrupting old desc system.
 *
 *   Revision 1.28  2001/05/03 12:29:12  mud
 *   *** empty log message ***
 *
 *   Revision 1.27  2001/05/02 12:30:53  mud
 *   Notifylists are now saved in pfile. Notifylist add <name> now first checks if there exists a player with that name before adding
 *
 *   Revision 1.26  2001/04/29 16:38:00  mud
 *   *** empty log message ***
 *
 *   Revision 1.25  2001/04/27 05:04:19  mud
 *   Bugfixes for XML. "set file" now actually saves settings to files, and saves CMDGroups for morts as well as Imms! :)
 *

 *   Revision 1.24  2001/04/25 08:50:58  mud
 *   Email and background entry added to pfiles, commands for setting email/background, and a viewbground command added.
 *
 *   Revision 1.23  2001/04/25 02:22:58  mud
 *   Bugfixes: Base AC back to 10, disguise OK,. IP and correct last-logon-time in last-command
 *
 *   Revision 1.22  2001/04/06 07:00:23  mud
 *   Final xml changes before going to Beta... .:)
 *
 *   Revision 1.21  2001/03/24 04:30:42  mud
 *   Minor bugfixes
 *
 *   Revision 1.20  2001/03/22 12:52:09  mud
 *   Fixed a bug in affections save, weaves only saves if they're tied
 *
 *   Revision 1.19  2001/03/21 10:43:15  mud
 *   More bugfixes to xml char save code
 *
 *   Revision 1.18  2001/03/18 10:30:53  mud
 *   Some bugfixes and upodates to the xmlcharsave code
 *
 *   Revision 1.17  2001/02/15 07:38:08  mud
 *   Final (hopefully) bugfixes. Now we can start the real testing
 *
 *   Revision 1.16  2001/01/28 08:44:55  mud
 *   *** empty log message ***
 *
 *   Revision 1.15  2000/12/11 12:14:18  mud
 *   Removed most references to PFILEPOS() get_char_ppos(), since with the xml code ppos will(may) change every reboot
 *
 *   Revision 1.14  2000/12/03 14:14:26  mud
 *   xml char save seems to work now, but we need a big, f**kin testing round to make sure.
 *
 *   Revision 1.13  2000/11/28 08:48:24  mud
 *   XML char save/load owrks. As does ptable building. Conversions looks fine and dandy too, although there's a few minor things to polish
 *
 *   Revision 1.12  2000/11/22 07:59:45  mud
 *   load_playertable() seems to work OK, most test cases in xml char save/load works (see xmlcharsave.c for alist of tested characters)
 *
 *   Revision 1.9  2000/11/12 14:44:14  mud
 *   More support code for xml transforming, a few bugfixes to xmlchar code, and index_from_array().
 *
 *   Revision 1.8  2000/11/09 14:13:48  mud
 *   Getting closer to switching-dat with xml chars
 *
 *   Revision 1.7  2000/11/08 08:25:39  mud
 *   Moved update_p_tabel() to xmlcharsave.[ch].
 *
 *   Revision 1.5  2000/09/10 06:36:20  mud
 *   Some updates...
 *
 *   Revision 1.4  2000/04/09 20:34:05  mud
 *   Changed player obj saving to XML format.
 *
 *   Revision 1.3  2000/03/10 18:26:42  mud
 *   Added a game of cards, which includes two new files,
 *   cards.c and cards.h, and modifying of a bunch of others.
 *   Note: xmlcharsve.x shouldn't be here, they just sort of tagged along.
 *
 *   Revision 1.2  2000/02/21 14:00:20  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/*************************************************************************
*   File: xmlcharsave.c                             Part of Dragons Fang *
*  Usage: Loads or saves player, in xml format. Simple.                  *
************************************************************************ */

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stddef.h>

#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "handler.h"
#include "db.h"
#include "interpreter.h"
#include "utils.h"
#include "class.h"
#include "constants.h"
#include "olc.h"
#include "xmlcharsave.h"
#include "xmlobjsave.h"
#include "guild.h"
#include "interpreter.h"
#include "dg_olc.h"
#include "act.h"
#include "retainer.h"

void addStringToXml(xmlNodePtr node, const char *tag, const char *entry);
int string_length(char *str);

/*
 * Creates the <immstuff> node (poofin, commandgroups etc)
 */
static xmlNodePtr immstuff_to_XML(struct char_data *ch,xmlDocPtr doc)
{
  xmlNodePtr imm;
  char *p;

  if (!ch || (GET_LEVEL(ch) < LVL_IMMORT)) {
     return NULL;
  }

  imm = xmlNewNode(NULL, "immstuff");
  imm->doc = doc;

  if (POOFIN(ch)) {
    xmlNewChild(imm, NULL, "poofin",(p = xmlEncodeEntitiesReentrant(doc,POOFIN(ch))));
    free(p);
  }

  if (POOFOUT(ch)) {
    xmlNewChild(imm, NULL, "poofout", (p = xmlEncodeEntitiesReentrant(doc,POOFOUT(ch))));
    free(p);
  }

  if (GET_INVSTART(ch) > 0) {
    sprintf(buf, "%ld", GET_INVSTART(ch));
    xmlNewChild(imm, NULL, "invstart", buf);
  }

  if (GET_OLC_ZONE(ch)) {
    sprintf(buf, "%ld", GET_OLC_ZONE(ch));
    xmlNewChild(imm, NULL, "olczone", buf);
  }

  if (ch->player_specials->saved.command_groups) {
    sprintf(buf, "%ld", ch->player_specials->saved.command_groups);
    xmlNewChild(imm, NULL, "commandgroups", buf);
  }

  return imm;
}

/*
 * Creates the <conditions> node.
 */
static xmlNodePtr conditions_to_XML(struct char_data *ch)
{
  xmlNodePtr conds, temp;

  if (!ch) {
     return NULL;
  }

  conds = xmlNewNode(NULL, "conditions");

  temp = xmlNewChild(conds, NULL, "cond", NULL);
  xmlNewProp(temp, "type", "drunk");
  sprintf(buf, "%d", GET_COND(ch, DRUNK));
  xmlNewProp(temp, "val", buf);

  temp = xmlNewChild(conds, NULL, "cond", NULL);
  xmlNewProp(temp, "type", "hunger");
  sprintf(buf, "%d", GET_COND(ch, FULL));
  xmlNewProp(temp, "val", buf);

  temp = xmlNewChild(conds, NULL, "cond", NULL);
  xmlNewProp(temp, "type", "thirsty");
  sprintf(buf, "%d", GET_COND(ch, THIRST));
  xmlNewProp(temp, "val", buf);

  temp = xmlNewChild(conds, NULL, "cond", NULL);
  xmlNewProp(temp, "type", "taint");
  sprintf(buf, "%ld", GET_TAINT(ch));
  xmlNewProp(temp, "val", buf);

  temp = xmlNewChild(conds,NULL,"cond",NULL);
  xmlNewProp(temp, "type", "compulsion");
  sprintf(buf, "%ld",GET_COMPULSION(ch));
  xmlNewProp(temp, "val", buf);

  return conds;
}

/*
 * Creates the <skills> node
 */
static xmlNodePtr skills_to_XML(struct char_data *ch)
{
  xmlNodePtr skills, temp;
  int i = 0;

  if (!ch) {
    return NULL;
  }
  
  skills = xmlNewNode(NULL,"skills");

  if (GET_LEVEL(ch) >= LVL_IMMORT) {
    return skills;
  }

  for (i = 0; i <= MAX_SKILLS; i ++) {
    if (GET_SKILL(ch,i) > 0) {
      temp = xmlNewChild(skills, NULL, "skill", NULL);
      sprintf(buf, "%d", i);
      xmlNewProp(temp, "skillnum", buf);
      sprintf(buf, "%d", GET_SKILL(ch, i));
      xmlNewProp(temp, "prof", buf);
    }
  }
  return skills;
}

static xmlNodePtr xdescs_to_XML(struct char_data *ch, xmlDocPtr doc)
{
  struct player_extradesc *xdesc;
  char *tmpstr;
  xmlNodePtr extradescs, temp;

  extradescs=xmlNewNode(NULL, "extradescs");
  extradescs->doc = doc;

  if (NULL != (xdesc=GET_PLAYERXDESC(ch))) {
    while (xdesc) {
      tmpstr=xmlEncodeEntitiesReentrant(doc,xdesc->desc);
      temp = xmlNewChild(extradescs, NULL, "xdesc", tmpstr);
      free(tmpstr);
      sprintf(buf, "%d", xdesc->descnum);
      xmlNewProp(temp, "descnum", buf);
      sprintf(buf, "%s", xdesc->keyword);
      xmlNewProp(temp, "keyword", buf);
      xdesc=xdesc->next;
    }
    return extradescs; 
  } else {
    return NULL;
  }
} 

static xmlNodePtr tells_to_XML(struct char_data *ch, xmlDocPtr doc)
{
  xmlNodePtr tells;
  int i = 0, num_tells = (((ch->player_specials->tells_index == MAX_TELLBUFFER_LENGTH-1) &&
              (*ch->player_specials->tells[MAX_TELLBUFFER_LENGTH-1])) ?
                MAX_TELLBUFFER_LENGTH-1 : ch->player_specials->tells_index-1);

  if (!ch)
    return NULL;

  tells = xmlNewNode(NULL, "tells");
  tells->doc = doc;
  for (i = 0; i <= num_tells; i++)
    addStringToXml(tells, "tell", ch->player_specials->tells[i]);

  return tells;
}

static xmlNodePtr rplog_to_XML(struct char_data *ch, xmlDocPtr doc)
{
  xmlNodePtr rplog;
  int i = 0, num = (((ch->player_specials->rplog_index == MAX_TELLBUFFER_LENGTH-1) &&
              (*ch->player_specials->rplog[MAX_TELLBUFFER_LENGTH-1])) ?
                MAX_TELLBUFFER_LENGTH-1 : ch->player_specials->rplog_index-1);

  if (!ch)
    return NULL;

  rplog = xmlNewNode(NULL, "rplog");
  rplog->doc = doc;
  for (i = 0; i <= num; i++)
    addStringToXml(rplog, "rp", ch->player_specials->rplog[i]);

  return rplog;
}

/*
 * Creates the <channeling> node
 */
static xmlNodePtr channeling_to_XML(struct char_data *ch)
{
  xmlNodePtr chan;

  if (!ch || cant_channel(ch)) {
    return NULL;
  }

  chan = xmlNewNode(NULL, "channeling");
  /*
   * Even 'classed' channelers have spark/teachable set. Or should have. :)
   * but only one or the other, not both!
   */
  if (GET_SPARK(ch)) {
    xmlNewProp(chan, "type", "spark");
  } else  if (GET_TEACHABLE(ch)) { // teachable
    xmlNewProp(chan, "type", "teachable");
  }

  sprintf(buf, "%d", GET_AIR(ch));
  xmlNewChild(chan, NULL, "air", buf);
  sprintf(buf, "%d", GET_EARTH(ch));
  xmlNewChild(chan, NULL, "earth", buf);
  sprintf(buf, "%d", GET_FIRE(ch));
  xmlNewChild(chan, NULL, "fire", buf);
  sprintf(buf, "%d", GET_SPIRIT(ch));
  xmlNewChild(chan, NULL, "spirit", buf);
  sprintf(buf, "%d", GET_WATER(ch));
  xmlNewChild(chan, NULL, "water", buf);
   
  return chan;
}

static xmlNodePtr notifylist_to_XML(struct char_data *ch,xmlDocPtr doc)
{
  xmlNodePtr not, temp;
  char *p;
  int i,cnt = 0;
  not = xmlNewNode(NULL, "notifylist");
  not->doc = doc;

  for (i = 0; i < MAX_NOTIFYLIST_LENGTH; i++) {
    if (*ch->player_specials->notifylist[i]) {
      cnt++;
      temp = xmlNewChild(not, NULL, "listentry", (p = xmlEncodeEntitiesReentrant(doc,ch->player_specials->notifylist[i])));
      free(p);
      sprintf(buf,"%d",i);
      xmlNewProp(temp,"pos",buf);
    }
  }
  sprintf(buf,"%d",cnt);
  xmlNewProp(not,"numset",buf);

  return not;
}

static xmlNodePtr markedmap_to_XML(struct char_data *ch,xmlDocPtr doc)
{
  xmlNodePtr not, temp;
  char *p;
  char ts[10];
  int i,cnt = 0;
  not = xmlNewNode(NULL, "markedmap");
  not->doc = doc;

  for (i = 0; i < 10; i++) {
    if (GET_MAPPED(ch, i)) {
      cnt++;
      sprintf(ts, "%ld", GET_MAPPED(ch, i));
      temp = xmlNewChild(not, NULL, "maploc", (p = xmlEncodeEntitiesReentrant(doc,ts)));
      free(p);
      sprintf(buf,"%d",i);
      xmlNewProp(temp,"pos",buf);
    }
  }

  return not;
}

/*
static xmlNodePtr ignorelist_to_XML(struct char_data *ch,xmlDocPtr doc)
{
  xmlNodePtr not, temp;
  char *p;
  int i,cnt = 0;
  not = xmlNewNode(NULL, "ignorelist");
  not->doc = doc;

  for (i = 0; i < MAX_IGNORELIST_LENGTH; i++) {
    if (*ch->player_specials->ignorelist[i]) {
      cnt++;
      temp = xmlNewChild(not, NULL, "listentry", (p = xmlEncodeEntitiesReentrant(doc,ch->player_specials->ignorelist[i])));
      free(p);
      sprintf(buf,"%d",i);
      xmlNewProp(temp,"pos",buf);
    }
  }
  sprintf(buf,"%d",cnt);
  xmlNewProp(not,"numset",buf);

  return not;
} */

/*
 * Creates the <pspecial> node
 */
static xmlNodePtr playerspecials_to_XML(struct char_data *ch, xmlDocPtr doc)
{
  xmlNodePtr spec, temp, temp2,tgt;
  char *tmpstr,*p;

  if (!ch)  {
    return NULL;
  }

  spec = xmlNewNode(NULL, "pspecial");
  spec->doc = doc;

  xmlAddChild(spec, xdescs_to_XML(ch,doc));

  /* not only imms can have olc permissions! */
  if (GET_OLC_ZONE(ch)) {
    sprintf(buf,"%ld",GET_OLC_ZONE(ch));
    xmlNewProp(spec,"olczone",buf);
  }

  if (IS_BRANDED(ch)) {
    sprintf(buf,"%ld", IS_BRANDED(ch));
    xmlNewProp(spec,"branded",buf);
  }

  sprintf(buf, "%ld", GET_WIMP_LEV(ch));
  xmlNewProp(spec, "wimpy", buf);

  sprintf(buf, "%d", GET_INVIS_LEV(ch));
  xmlNewProp(spec, "invis", buf);

  sprintf(buf, "%d", GET_LOADROOM(ch));
  xmlNewProp(spec, "loadroom", buf);

  sprintf(buf,"%ld",GET_POWER(ch));
  xmlNewProp(spec, "powerpoints",buf);

  sprintf(buf,"%ld",GET_LAST_EMOTE(ch));
  xmlNewProp(spec,"emoteCnt",buf);

  sprintf(buf,"%ld",GET_AUTHORIZED(ch));
  xmlNewProp(spec,"auth",buf);

  tgt = xmlNewNode(NULL,"taughtn");
  sprintf(buf,"%d",TAUGHT(ch));
  xmlNewProp(tgt,"taught",buf);
  sprintf(buf,"%d",HPTAUGHT(ch));
  xmlNewProp(tgt,"hptaught",buf);
  xmlAddChild(spec,tgt);

  xmlAddChild(spec, skills_to_XML(ch));
  xmlAddChild(spec, tells_to_XML(ch, doc));
  xmlAddChild(spec, rplog_to_XML(ch, doc));
  xmlAddChild(spec, conditions_to_XML(ch));

  sprintf(buf, "%d", GET_FREEZE_LEV(ch));
  xmlNewChild(spec, NULL, "freeze", buf);
  sprintf(buf, "%d", GET_BAD_PWS(ch));
  xmlNewChild(spec, NULL, "badpws", buf);

  if (ch->player_specials->saved.command_groups) {
    sprintf(buf, "%ld", ch->player_specials->saved.command_groups);
    xmlNewChild(spec, NULL, "commandgroups", buf);
  }  

  if (GET_SERVANT(ch) && *GET_SERVANT(ch)) {
    tmpstr = xmlEncodeEntitiesReentrant(doc, GET_SERVANT(ch));
    xmlNewChild(spec, NULL, "servant", tmpstr);
    free(tmpstr);
  }

  if (GET_DOING(ch)) {
    xmlNewChild(spec, NULL, "doing",(p = xmlEncodeEntitiesReentrant(doc,GET_DOING(ch))));
    free(p);
  }
  
  if (GET_SKILL(ch, SKILL_MARKEDMAP)) {
    xmlAddChild(spec, markedmap_to_XML(ch,doc));
  }

  if (*ch->player_specials->notifylist[0]) {
    xmlAddChild(spec, notifylist_to_XML(ch,doc));
  }

/*  if (*ch->player_specials->ignorelist[0]) {
    xmlAddChild(spec,ignorelist_to_XML(ch,doc));
  }
*/
  if (!cant_channel(ch)) {
    xmlAddChild(spec, channeling_to_XML(ch));
  }

  if (GET_BM(ch)) {
    sprintf(buf, "%d", GET_BM(ch));
    xmlNewChild(spec, NULL, "blademastery", buf);
  }

  if (GET_TAVEREN(ch)) {
    xmlNewChild(spec, NULL, "taveren", NULL);
  }

  if (GET_WOLFKIN(ch)) {
    temp = xmlNewChild(spec, NULL, "wolfkin", NULL);
    xmlNewProp(temp, "metKindred", GET_ISWOLFKIN(ch)? "yes":"no");
  }
  
  if (GET_NOBILITY(ch)) {
    temp = xmlNewChild(spec, NULL, "nobility", NULL);
    temp->doc = doc;
    xmlNewProp(temp, "rank", GET_NOBILITY(ch));
  }

  if (GET_DFINTR(ch)) {
    sprintf(buf, "%d", GET_DFINTR(ch));
    temp = xmlNewChild(spec, NULL, "drfflags", NULL);
    xmlNewProp(temp, "flag", buf);
  }    

  if (GET_AJAH(ch))  {
    temp = xmlNewChild(spec, NULL, "ajah", NULL);
    temp->doc = doc;
    xmlNewProp(temp, "colour", ajah_colors[GET_AJAH(ch)-1]);
  }

  if (GET_BONDED(ch) || GET_ASHABONDED(ch) || GET_MINION(ch)) {
    temp = xmlNewChild(spec, NULL, "bonds", NULL);

    if (GET_BONDED(ch))  {
      temp2 = xmlNewChild(temp, NULL, "bond", NULL);
      xmlNewProp(temp2, "type", "bonded");
      sprintf(buf, "%ld", GET_BONDED(ch));
      xmlNewProp(temp2, "id", buf);
    }   

    if (GET_ASHABONDED(ch)) {
      temp2 = xmlNewChild(temp, NULL, "bond", NULL);
      xmlNewProp(temp2, "type", "asha");
      sprintf(buf, "%ld", GET_ASHABONDED(ch));
      xmlNewProp(temp2, "id", buf);
    }   

    if (GET_MINION(ch)) {
      temp2 = xmlNewChild(temp, NULL, "bond", NULL);
      xmlNewProp(temp2, "type", "minion");
      sprintf(buf, "%ld", GET_MINION(ch));
      xmlNewProp(temp2, "id", buf);
    }   
  }

  temp = xmlNewChild(spec, NULL, "flags", NULL);
  temp2 = xmlNewChild(temp, NULL, "permanent", NULL);
  sprintf(buf, "%ld", GET_PERMANENT(ch));
  xmlNewProp(temp2, "flag", buf);
  temp2 = xmlNewChild(temp, NULL, "prefs", NULL);
  sprintf(buf, "%ld", PRF_FLAGS(ch));
  xmlNewProp(temp2, "flag", buf);
  temp2 = xmlNewChild(temp, NULL, "prefs2", NULL);
  sprintf(buf, "%ld", PRF2_FLAGS(ch));
  xmlNewProp(temp2, "flag", buf);
  temp2 = xmlNewChild(temp, NULL, "syslog", NULL);
  sprintf(buf, "%ld", EXTENDED_SYSLOG(ch));
  xmlNewProp(temp2, "flag", buf);

  temp2 = xmlNewChild(temp, NULL, "autoflags", NULL);
  sprintf(buf, "%ld", AUTO_FLAGS(ch));
  xmlNewProp(temp2, "flag", buf);

  sprintf(buf, "%ld", PLR_FLAGS(ch));
  xmlNewProp(temp2, "act", buf);

  sprintf(buf, "%ld", AFF_FLAGS(ch));
  xmlNewProp(temp2, "affs", buf);

  temp = xmlNewChild(spec, NULL, "questpoints", NULL);
  sprintf(buf, "%ld", GET_QP(ch));
  xmlNewProp(temp, "amount", buf);

  if (GET_SUBDUE(ch)) {
    xmlNewChild(spec, NULL, "subdue", NULL);
  }
  
  temp = xmlNewChild(spec, NULL, "lines", NULL);
  sprintf(buf, "%d", GET_LINES(ch));
  xmlNewProp(temp, "value", buf);

  if (GET_PREFIX(ch)) {
    xmlNewChild(spec, NULL, "prefix", (tmpstr =  xmlEncodeEntitiesReentrant(doc,GET_PREFIX(ch))));
    free(tmpstr);
  }
  
  if (GET_SPSKILLS1(ch) || GET_SPSKILLS2(ch)) {
    temp =  xmlNewChild(spec, NULL, "spec_skills",NULL);
    if (GET_SPSKILLS1(ch)) {
      sprintf(buf, "%ld", GET_SPSKILLS1(ch));
      xmlNewProp(temp,"spec1",buf);
    }
    if (GET_SPSKILLS2(ch)) {
      sprintf(buf,"%ld",GET_SPSKILLS2(ch));
      xmlNewProp(temp,"spec2",buf);
    }
  }

  if (GET_PRIMTALENTS1(ch) || GET_PRIMTALENTS2(ch) ||
      GET_SECTALENTS1(ch) || GET_SECTALENTS2(ch)) {

    temp = xmlNewChild(spec, NULL, "talents", NULL);

    if (GET_PRIMTALENTS1(ch)) {
      sprintf(buf, "%ld", GET_PRIMTALENTS1(ch));
      xmlNewProp(temp, "primary1", buf);
    }

    if (GET_PRIMTALENTS2(ch))  { 
      sprintf(buf, "%ld", GET_PRIMTALENTS2(ch));
      xmlNewProp(temp, "primary2", buf); 
    }
    if (GET_SECTALENTS1(ch))  {
      sprintf(buf, "%ld", GET_SECTALENTS1(ch));
      xmlNewProp(temp, "secondary1", buf);
    }
    if (GET_SECTALENTS2(ch))  {
      sprintf(buf, "%ld", GET_SECTALENTS2(ch));
      xmlNewProp(temp, "secondary2", buf);
    }
  }
  return spec;
}

/*
 * Creates the <abilities> node.
 */
static xmlNodePtr abilities_to_XML(struct char_data *ch)
{
  xmlNodePtr abil;

  if (!ch) {
    return NULL;
  }

  abil = xmlNewNode(NULL, "abilities");
  
  sprintf(buf, "%d", GET_RSTR(ch));
  xmlNewChild(abil, NULL, "strength", buf);
  sprintf(buf, "%d", GET_RCON(ch));
  xmlNewChild(abil, NULL, "constitution", buf);
  sprintf(buf, "%d", GET_RDEX(ch));
  xmlNewChild(abil, NULL, "dexterity", buf);
  sprintf(buf, "%d", GET_RINT(ch));
  xmlNewChild(abil, NULL, "intelligence", buf);
  sprintf(buf, "%d", GET_RWIS(ch));
  xmlNewChild(abil, NULL, "wisdom", buf);
  sprintf(buf, "%d", GET_RCHA(ch));
  xmlNewChild(abil, NULL, "charisma", buf);

  return abil;
}

/*
 * Creates the <pdata> node
 */
static xmlNodePtr pdata_to_XML(struct char_data *ch)
{
  xmlNodePtr pdata, temp;

  if (!ch) {
    return NULL;
  }

  pdata = xmlNewNode(NULL, "pdata");

  temp = xmlNewChild(pdata, NULL, "hitp", NULL);
  sprintf(buf, "%d", GET_HIT(ch));
  xmlNewProp(temp, "value", buf);
  sprintf(buf, "%d", GET_MAX_HIT(ch));
  xmlNewProp(temp, "max", buf);

  temp = xmlNewChild(pdata, NULL, "mana", NULL);
  sprintf(buf, "%ld", GET_MANA(ch));
  xmlNewProp(temp, "value", buf);
  sprintf(buf, "%ld", GET_MAX_MANA(ch));
  xmlNewProp(temp, "max", buf);

  temp = xmlNewChild(pdata, NULL, "move", NULL);
  sprintf(buf, "%d", GET_MOVE(ch));
  xmlNewProp(temp, "value", buf);
  sprintf(buf, "%d", GET_MAX_MOVE(ch));
  xmlNewProp(temp, "max", buf);

  temp = xmlNewChild(pdata, NULL, "gold", NULL);
  sprintf(buf, "%ld", GET_GOLD(ch));
  xmlNewProp(temp, "carrying", buf);
  sprintf(buf, "%ld", GET_BANK_GOLD(ch));
  xmlNewProp(temp, "inbank", buf);

  sprintf(buf, "%ld", GET_EXP(ch));
  xmlNewChild(pdata, NULL, "experience", buf);
 
  sprintf(buf, "%d", GET_TRAINS(ch));
  xmlNewChild(pdata, NULL, "trains", buf);

  sprintf(buf, "%ld", GET_PRACTICES(ch));
  xmlNewChild(pdata, NULL, "pracs", buf);
  
  return pdata;
}

/*
 * Creates the <playerdata> node
 */
static xmlNodePtr playerdata_to_XML(struct char_data *ch,xmlDocPtr doc)
{
  xmlNodePtr data;
  char *p;

  if (!ch) {
    return NULL;
  }

  data = xmlNewNode(NULL, "playerdata");
  data->doc = doc;

  xmlNewProp(data, "hometown", hometown[GET_RACE(ch)]);
  sprintf(buf, "%ld", ((long)(time(0) - ch->player.time.logon)) + ch->player.time.played);
  xmlNewProp(data, "played", buf);
  sprintf(buf, "%ld",time(0)); // Now is the last-logon time. :)
  xmlNewProp(data,"lastLogon",buf);
  sprintf(buf, "%d", GET_HEIGHT(ch));
  xmlNewProp(data, "height", buf);
  sprintf(buf, "%d", GET_WEIGHT(ch));
  xmlNewProp(data, "weight", buf);
  xmlNewChild(data, NULL, "password", (p =  xmlEncodeEntitiesReentrant(doc,GET_PASSWD(ch))));
  free(p);
  sprintf(buf, "%ld", ch->player.time.birth);
  xmlNewChild(data, NULL, "birth", buf);
  sprintf(buf, "%ld", ch->player.time.logon);
  xmlNewChild(data, NULL, "lastLogon", buf);
  if (ch->desc)
    xmlNewChild(data, NULL, "host", ch->desc->host);  
  else
    xmlNewChild(data, NULL, "host", ch->tempHost);
  
  return data;
}

/*
 * Creates the <affections> node
 */
static xmlNodePtr affects_to_XML(struct char_data *ch)
{
  xmlNodePtr aff, affs, temp;
  struct affected_type *thisAff;

  if (!ch) {
    return NULL;
  }

  affs = xmlNewNode(NULL, "affections");
  thisAff = ch->affected;

  while (thisAff) {
    if ((thisAff->type > MAX_SPELLS) || (thisAff->tie_info == SCMD_TIED)) {// only if weave is tied,  save it!
      aff = xmlNewChild(affs, NULL, "affection", NULL);
      sprintf(buf,"%ld",thisAff->result);
      xmlNewProp(aff,"result",buf);
      
      sprintf(buf, "%d", thisAff->type);
      xmlNewChild(aff, NULL, "type", buf);

      if (thisAff->duration) {  
    sprintf(buf, "%d", thisAff->duration);
    xmlNewChild(aff, NULL, "duration", buf);
      }

      if (thisAff->location) {
    sprintf(buf, "%d", thisAff->location);
    xmlNewChild(aff, NULL, "location", buf);
      }

      sprintf(buf, "%d", thisAff->tie_info);
      xmlNewChild(aff, NULL, "tieinfo", buf);

      if (thisAff->bitvector || thisAff->bitvector2 || 
        thisAff->bitvector3 || thisAff->bitvector4)  {

    temp = xmlNewChild(aff, NULL, "bvs", buf);
    
    if (thisAff->bitvector) {
      sprintf(buf, "%ld", thisAff->bitvector);
      xmlNewProp(temp, "bv1", buf);
    }

    if (thisAff->bitvector2) {
      sprintf(buf, "%ld", thisAff->bitvector2);
      xmlNewProp(temp, "bv2", buf);
    }

    if (thisAff->bitvector3) {
      sprintf(buf, "%ld", thisAff->bitvector3);
      xmlNewProp(temp, "bv3", buf);
    }

    if (thisAff->bitvector4) {
      sprintf(buf, "%ld", thisAff->bitvector4);
      xmlNewProp(temp, "bv4", buf);
    }
      } // bitvectors

      if (thisAff->modifier) {
    sprintf(buf, "%ld", thisAff->modifier);
    xmlNewChild(aff, NULL, "modifier", buf);
      }
      
      if (thisAff->weaver) {
    sprintf(buf, "%ld", thisAff->weaver);
    xmlNewChild(aff, NULL, "weaver", buf);
      }

      if (thisAff->speed) {
    sprintf(buf, "%ld", thisAff->speed);
    xmlNewChild(aff, NULL, "speed", buf);
      }

      if (thisAff->mana_add) {
    sprintf(buf, "%ld", thisAff->mana_add);
    xmlNewChild(aff, NULL, "mana_add", buf);
      }

      if (thisAff->spell) {
    sprintf(buf, "%ld", thisAff->spell);
    xmlNewChild(aff, NULL, "spell", buf);
      }

      if (thisAff->sex) {
    sprintf(buf, "%ld", thisAff->sex);
    xmlNewChild(aff, NULL, "gender", buf);
      }
    } // if tied, etc, etc...
    thisAff = thisAff->next;
  } // while

  return affs;
}

/*
 * Saves a player to file.
 * 
 * Last edited: 000812 by Fredrik
 * Arguments: Character to save
 * Returns: -
 */
void xmlsave_char(struct char_data *ch)
{
  struct board_stamp  *bstamp;
  char fname[100], *tmpstr, bakn[1000];
  xmlDocPtr doc;
  xmlNodePtr  temp, player,descs,desc;
  struct obj_data *char_eq[NUM_WEARS];
  struct affected_type *affs = NULL, *aff;
  int i,cnt,rc;

  if (!ch || IS_NPC(ch)) {
    return;
  }

  if (!get_filename(GET_RNAME(ch), fname, PLR_FILE))  {
    asend_to_char(ch, "Couldn't get your savefilename, reason: %s.\r\n", strerror(errno));
    return;
  }
  sprintf(bakn,"%s.bak",fname);

  /*
   * fopen("w[+]") truncates existing files! bad mojo! save to a tempfile first, iv all is OK, 
   * then x-change files!
   
  if (!(fl = fopen(bakn, "w"))) {
    asend_to_char(ch, "Couldn't open your savefile, reason: %s.\r\n", strerror(errno));
    return;
  }
  */

  doc = xmlNewDoc("1.0");
  player = xmlNewDocNode(doc, NULL, "player",NULL);
  doc->children = player;

  /* Here starts the node building */
    
  sprintf(buf, "%ld %ld",  GET_GOLD(ch), GET_BANK_GOLD(ch));
  xmlNewProp(player,"name", ch->player.name);

  if (GET_DISGUISED(ch))
    xmlNewProp(player, "dname", GET_DNAME(ch));
  
  xmlNewProp(player, "gender", genders[(int)GET_SEX(ch)]);
  xmlNewProp(player, "class", pc_class_types[(int) GET_CLASS(ch)]);
  sprintf(buf, "%d", (NOWHERE != ch->in_room) ? world[ch->in_room].number : -1);
  xmlNewProp(player, "inroom", buf);
  sprintf(buf, "%ld", GET_LEVEL(ch));
  xmlNewProp(player, "level", buf);

  if (ch->affected) {
    xmlAddChild(player, affects_to_XML(ch)); // Must come before stripping affects...
  }

  cnt = 0; 
  for (i = 0; i < MAX_DESCRIPTIONS; i++) 
    cnt += (NULL != GET_DESCNUM(ch,i) ? 1 : 0);

  if (0 != cnt) {
    descs = xmlNewChild(player,NULL,"descriptions",NULL);
    sprintf(buf,"%d",GET_CURRENTDESC(ch));
    xmlNewProp(descs,"current",buf);
    for (i = 0; i < MAX_DESCRIPTIONS; i++) {
      if (GET_DESCNUM(ch,i)) {
        desc = xmlNewChild(descs,NULL,"desc",(tmpstr = xmlEncodeEntitiesReentrant(doc,GET_DESCNUM(ch,i))));
        free(tmpstr);
        sprintf(buf,"%d",i);
        xmlNewProp(desc,"pos",buf);
      }
    }
  }

  if (GET_CPROMPT(ch) && *GET_CPROMPT(ch)) {
    tmpstr = xmlEncodeEntitiesReentrant(doc, GET_CPROMPT(ch));
    xmlNewChild(player,NULL,"prompt",tmpstr);
    free(tmpstr);
  }

  if (GET_RTITLE(ch) && *GET_RTITLE(ch)) {
    tmpstr =  xmlEncodeEntitiesReentrant(doc, GET_RTITLE(ch));
    xmlNewChild(player, NULL, "title", tmpstr);
    free(tmpstr);
  }

  if (GET_DISGUISED(ch) && GET_DTITLE(ch) && *GET_DTITLE(ch)) {
    tmpstr = xmlEncodeEntitiesReentrant(doc, GET_DTITLE(ch));
    xmlNewChild(player, NULL, "dtitle", tmpstr);
    free(tmpstr);
  }

  if (GET_PRETITLE(ch) && *(GET_PRETITLE(ch))) {
     tmpstr =  xmlEncodeEntitiesReentrant(doc,GET_PRETITLE(ch));
     xmlNewChild(player,NULL,"pretitle",tmpstr);
     free(tmpstr);
  }

  if (GET_CONTACTINFO(ch) && *(GET_CONTACTINFO(ch))) {
    tmpstr =  xmlEncodeEntitiesReentrant(doc,GET_CONTACTINFO(ch));
    xmlNewChild(player,NULL,"email",tmpstr);
    free(tmpstr);
  }

  if (GET_BACKGROUND(ch) && *(GET_BACKGROUND(ch))) {
    tmpstr =  xmlEncodeEntitiesReentrant(doc,GET_BACKGROUND(ch));
    xmlNewChild(player,NULL,"background",tmpstr);
    free(tmpstr);
  }

  if (GET_MYCOLOR(ch) && *(GET_MYCOLOR(ch))) {
    tmpstr=xmlEncodeEntitiesReentrant(doc,GET_MYCOLOR(ch));
    xmlNewChild(player,NULL,"mycolor",tmpstr);
    free(tmpstr);
  }
  

  /* Must strip char now */

  for (i = 0; i < NUM_WEARS; i ++)  {
    if (GET_EQ(ch, i)) {
      char_eq[i] = unequip_char(ch, i);
    } else {
      char_eq[i] = NULL;
    }
  }

  
  cnt = 0; 
  aff = ch->affected;
  while (aff) {
    cnt++, aff = aff->next;
  }
  aff = ch->affected;
  if (cnt) {
    affs = calloc(sizeof(struct affected_type),cnt);
  
    for (i = 0; i < cnt; i ++) {
      affs[i] = *aff;
      affs[i].next = NULL;
      aff = aff->next;
    }
  }

  while(ch->affected)
    affect_remove(ch,ch->affected);

  xmlAddChild(player, playerdata_to_XML(ch,doc));
  temp = xmlNewChild(player, NULL, "cspecial", NULL);
  sprintf(buf, "%ld", GET_IDNUM(ch));
  xmlNewProp(temp, "idnum", buf);
  
  xmlAddChild(player, temp);

  bstamp = ch->brd_stamp;
  while( bstamp )
  {
    temp = xmlNewChild( player, NULL, "board", NULL );
    sprintf( buf, "%d", bstamp->vnum );
    xmlNewProp( temp, "vnum", buf );
    sprintf( buf, "%ld", bstamp->stamp );
    xmlNewProp( temp, "stamp", buf );
    xmlAddChild( player, temp );
    bstamp = bstamp->next;
  }

  xmlAddChild(player, playerspecials_to_XML(ch, doc));
  xmlAddChild(player, abilities_to_XML(ch));
  xmlAddChild(player, pdata_to_XML(ch));

  if (GET_LEVEL(ch) >= LVL_IMMORT) {
    xmlAddChild(player, immstuff_to_XML(ch,doc));
  }

  /* DGS */
  if (SCRIPT(ch) && !IS_NPC(ch) && ch->script->global_vars)
   xmlAddChild(player, DGVariables_to_XML(ch));

  rc = xmlSaveFile(bakn,doc);
  //  xmlDocDump(fl, doc);
  xmlFreeDoc(doc);
  // fclose(fl);
  
  if (-1 != rc) {
    remove(fname); // old save file
    rename(bakn,fname); // mv bakn fname! :) 
  } else {
    asend_to_char(ch,"Failed to save your playerfile, reason: %s\r\n",strerror(errno));
    remove(bakn); // In case it's there...
  }

  /*
   * here is time to swap files if everything is OK!
   */

  // Now put the eq and the affects back into place
  for (i = 0; i < NUM_WEARS; i ++) {
    if (char_eq[i]) {
      equip_char(ch, char_eq[i], i);
    }
  }

  if (cnt && affs) {
    for (i = 0; i < cnt; i ++) {
      if (affs[i].type) {
    affect_to_char(ch, &affs[i]);
      }
    }
    free(affs);
  }
  save_retainers(ch);
}

/********************* LOADING OF CHAR FROM XML FILE BELOW ***********************/
/*********************************************************************************/

/*
 * Parses the <abilities> node
 */
static void xml_to_abilities(xmlNodePtr node, struct char_data *ch)
{
  xmlNodePtr temp;

  if (!node || !ch) {
    return;
  }

  if (strcasecmp("abilities", node->name)) {
    return;
  }

  for (temp = node->children; temp; temp = temp->next)  {
    if (!strcasecmp(temp->name, "strength"))  {
      GET_STR(ch) = atoi(temp->children->content);
      GET_RSTR(ch) = GET_STR(ch);
    } else if (!strcasecmp(temp->name, "constitution"))  {
      GET_CON(ch) = atoi(temp->children->content);
      GET_RCON(ch) = GET_CON(ch);
    } else if (!strcasecmp(temp->name, "dexterity")) {
      GET_DEX(ch) = atoi(temp->children->content);
      GET_RDEX(ch) = GET_DEX(ch);
    } else if (!strcasecmp(temp->name, "intelligence")) {
      GET_INT(ch) = atoi(temp->children->content);
      GET_RINT(ch) = GET_INT(ch);
    } else if (!strcasecmp(temp->name, "wisdom")) {
      GET_WIS(ch) = atoi(temp->children->content);
      GET_RWIS(ch) = GET_WIS(ch);
    } else if (!strcasecmp(temp->name, "charisma")) { 
      GET_CHA(ch) = atoi(temp->children->content);
      GET_RCHA(ch) = GET_CHA(ch);
    }
  }
}


/*
 * Parses the <pdata> node
 */
static void xml_to_pdata(xmlNodePtr node, struct char_data *ch)
{
  xmlNodePtr temp;

  if (!node || !ch) {
    return;
  }

  if (strcasecmp("pdata", node->name)) {
    return;
  }

  for (temp = node->children; temp; temp = temp->next) {
    if (!strcasecmp(temp->name, "hitp"))  {
      GET_HIT(ch) = xmlAtoi(xmlGetProp(temp, "value"));
      GET_MAX_HIT(ch) = xmlAtoi(xmlGetProp(temp, "max"));
    } else if (!strcasecmp(temp->name, "mana")) {
      SET_MANA(ch, xmlAtoi(xmlGetProp(temp, "value")));
      SET_MAX_MANA(ch, xmlAtoi(xmlGetProp(temp, "max")));
    } else if (!strcasecmp(temp->name, "move")) {
      GET_MOVE(ch) = xmlAtoi(xmlGetProp(temp, "value")); 
      GET_MAX_MOVE(ch) = xmlAtoi(xmlGetProp(temp, "max"));
    } else if (!strcasecmp(temp->name, "gold")) {
      GET_GOLD(ch) = xmlAtol(xmlGetProp(temp, "carrying"));
      GET_BANK_GOLD(ch) = xmlAtol(xmlGetProp(temp, "inbank"));
    } else if (!strcasecmp(temp->name, "experience")) {
      GET_EXP(ch) = atol(temp->children->content);
    } else if (!strcasecmp(temp->name, "trains")) {
      GET_TRAINS(ch) = atoi(temp->children->content);
    } else if (!strcasecmp(temp->name, "pracs")) {
      GET_PRACTICES(ch) = atol(temp->children->content);
    }
  }
}

/*
 * Parses the <immstuff> node
 */
static void xml_to_immstuff(xmlNodePtr node, struct char_data *ch)
{
  xmlNodePtr temp;

  if (!node || !ch) {
    return;
  }

  if (strcasecmp("immstuff", node->name)) {
    return;
  }

  for (temp = node->children; temp; temp = temp->next) {
    if (!strcasecmp("poofin", temp->name)) {
      POOFIN(ch) = strdup(temp->children->content);
    }  else if (!strcasecmp("poofout", temp->name)) {
      POOFOUT(ch) = strdup(temp->children->content);
    } else if (!strcasecmp("invstart", temp->name)) {
      GET_INVSTART(ch) = MIN(atol(temp->children->content), LVL_IMPL);
    } else if (!strcasecmp("olczone", temp->name)) {
      GET_OLC_ZONE(ch) = atol(temp->children->content);
    } else if (!strcasecmp("commandgroups", temp->name)) {
      ch->player_specials->saved.command_groups = atol(temp->children->content);
    }
  } 
}

/*
 * Parses the <affections> node
 */
static void xml_to_affects(xmlNodePtr node, struct char_data *ch)
{
  xmlNodePtr temp, temp2;
  struct affected_type *aff;

  if (!node || !ch) {
    return;
  }

  if (strcasecmp("affections", node->name)) {
    return;
  }

  for (temp = node->children; temp; temp = temp->next)  {
    if (strcasecmp(temp->name, "affection")) {
      continue;
    }

    aff = (struct affected_type *) calloc(1, sizeof(struct affected_type));
    aff->result = xmlAtol(xmlGetProp(temp,"result"));

    for (temp2 = temp->children; temp2; temp2 = temp2->next) {
      if (!strcasecmp(temp2->name, "type")) {
        aff->type = atoi(temp2->children->content);
      } else if (!strcasecmp(temp2->name, "duration")) {
        aff->duration = atoi(temp2->children->content);
      } else if (!strcasecmp(temp2->name, "location")) {
        aff->location = atoi(temp2->children->content);
      } else if (!strcasecmp(temp2->name, "tieinfo")) {
        aff->tie_info = atoi(temp2->children->content);
      } else if (!strcasecmp(temp2->name, "weaver")) {
    aff->weaver = atol(temp2->children->content);
      } else if (!strcasecmp(temp2->name, "bvs")) {

        if ((aff->bitvector = xmlAtol(xmlGetProp(temp2, "bv1"))) == -1)
      aff->bitvector = 0;

        if ((aff->bitvector2 = xmlAtol(xmlGetProp(temp2, "bv2"))) == -1)
      aff->bitvector2 = 0;

        if ((aff->bitvector3 = xmlAtol(xmlGetProp(temp2, "bv3"))) == -1)
      aff->bitvector3 = 0;

        if ((aff->bitvector4 = xmlAtol(xmlGetProp(temp2, "bv4"))) == -1)
      aff->bitvector4 = 0;

      } else if (!strcasecmp(temp2->name, "modifier")) {
        aff->modifier = atol(temp2->children->content);
      } else if (!strcasecmp(temp2->name, "speed")) {
        aff->speed = atol(temp2->children->content);
      } else if (!strcasecmp(temp2->name, "mana_add")) {
        aff->mana_add = atol(temp2->children->content);
      } else if (!strcasecmp(temp2->name, "spell")) {
        aff->spell = atol(temp2->children->content);
      } else if (!strcasecmp(temp2->name, "gender")) {
        aff->sex = atol(temp2->children->content);
      }
    }
    affect_to_char(ch,aff);
  }
}

/*
 * Parses the <skills> node
 */
static void xml_to_skills(xmlNodePtr node, struct char_data *ch)
{

  xmlNodePtr temp;
  int num, percent;

  if (!node || !ch) {
    return;
  }

  if (strcasecmp("skills", node->name)) {
    return;
  }

  // Imms get 100% in everything...
  if (GET_LEVEL(ch) >= LVL_IMMORT)  {
    for (num = 1 ; num <= MAX_SKILLS; num++) {
      SET_SKILL(ch, num, 100);
    }
    SET_SKILL(ch, 0, 0); /* skill 0 is unused, should -always- be zero! */
  }

  for (temp = node->children; temp; temp = temp->next) 
  {
    if (strcasecmp(temp->name, "skill"))
      continue;

    if ((num = xmlAtoi(xmlGetProp(temp, "skillnum"))) == -1)
      num = 0;

    if( (num > 0) && (num <= MAX_SKILLS) )
    {
      percent = xmlAtoi(xmlGetProp(temp, "prof"));
      if ((percent > 0) && (percent <= 100)) 
	SET_SKILL(ch, num, percent);
    }    
  } // for...
}

static void xml_to_tells(xmlNodePtr node, struct char_data *ch)
{
  xmlNodePtr temp;
  char *tmp;

  if (!node || strcmp(node->name, "tells") || !ch)
    return;

  for (temp = node->children; temp; temp = temp->next) {
    if (strcmp(temp->name, "tell"))
      continue;

    add_tell_to_buffer(ch, (tmp = xmlNodeGetContent(temp)));
    free(tmp);
  }
}

static void xml_to_rplog(xmlNodePtr node, struct char_data *ch)
{
  xmlNodePtr temp;
  char *tmp;
  long old;

  if (!node || strcmp(node->name, "rplog") || !ch)
    return;

  for (temp = node->children; temp; temp = temp->next) {
    if (strcmp(temp->name, "rp"))
      continue;

    old = PLR_FLAGS(ch);
    SET_BIT(PRF_FLAGS(ch), PRF_IC); 
    add_to_rplog(ch, (tmp = xmlNodeGetContent(temp)),ch);
    PRF_FLAGS(ch) = old;

    free(tmp);
  }
}

/*
 * Parses the <conditions> node
 */
static void xml_to_conditions(xmlNodePtr node, struct char_data *ch)
{
  xmlNodePtr temp;
  char *type, *val;

  if (!node || !ch)  {
    return;
  }

  if (strcasecmp("conditions", node->name)) {
    return;
  }

  for (temp = node->children; temp; temp = temp->next) {
    if (!strcasecmp(temp->name, "cond")) {

      if ((type = xmlGetProp(temp, "type")) && (val = xmlGetProp(temp, "val")))  {

        if (!strcasecmp(type, "drunk")) {
          GET_COND(ch, DRUNK) = MAX(0, MIN(24, atoi(val)));
        } else if (!strcasecmp(type, "hunger")) {
          GET_COND(ch, FULL)  = MAX(-1, MIN(24, atoi(val)));
        } else if (!strcasecmp(type, "thirsty")) {
          GET_COND(ch, THIRST)  = MAX(-1, MIN(24, atoi(val)));
        } else if (!strcasecmp(type, "taint")) {
          GET_TAINT(ch) = MAX(0, atol(val));
        } else if (!strcasecmp(type,"compulsion")) {
	  GET_COMPULSION(ch) = atol(val);
	}
	free(type);
	free(val);
      }
    }
  } // for
}

/*
 * Parses the <channeling> node
 */
static void xml_to_channeling(xmlNodePtr node, struct char_data *ch)
{
  xmlNodePtr temp;
  char *type;

  if (!node || !ch) {
    return;
  }

  if (strcasecmp("channeling", node->name)) {
    return;
  }

  type = xmlGetProp(node, "type");

  if (type)  {
    if (!strcasecmp(type, "spark")) {
      GET_SPARK(ch) = 1;
    } else if (!strcasecmp(type, "teachable")) {
      GET_TEACHABLE(ch) = 1;
    }
    free(type);
  }

  for (temp = node->children; temp; temp = temp->next) {
    if (!strcasecmp(temp->name, "air")) {
      GET_AIR(ch) = MAX(0, atoi(temp->children->content));
    } else if (!strcasecmp(temp->name, "earth")) {
      GET_EARTH(ch) = MAX(0, atoi(temp->children->content));
    } else if (!strcasecmp(temp->name, "fire")) {
      GET_FIRE(ch) = MAX(0, atoi(temp->children->content));
    } else if (!strcasecmp(temp->name, "spirit")) {
      GET_SPIRIT(ch) = MAX(0, atoi(temp->children->content));
    } else if (!strcasecmp(temp->name, "water")) {
      GET_WATER(ch) = MAX(0, atoi(temp->children->content));
    }
  } /* for... */
}

static void xml_to_notifylist(xmlNodePtr list,struct char_data *ch)
{
  int num;
  int empty_strings = 0;
  xmlNodePtr tmp;

  for (tmp = list->children; NULL != tmp; tmp = tmp->next) {
    if (!strcasecmp(tmp->name,"listentry")) {
      num = xmlAtoi(xmlGetProp(tmp,"pos"));
      if (!tmp->children || !tmp->children->content || !*tmp->children->content) {
	empty_strings++;
	continue;
      }
      
      strcpy(ch->player_specials->notifylist[num-empty_strings],tmp->children->content);
    }
  }
}

static void xml_to_xdescs(xmlNodePtr list, struct char_data *ch)
{
  xmlNodePtr tmp;
  struct player_extradesc *xd;

  for (tmp = list->children; NULL != tmp; tmp = tmp->next) {
    if (!strcasecmp(tmp->name,"xdesc")) {
      xd=calloc(1, sizeof(struct player_extradesc));
      xd->descnum=xmlAtoi(xmlGetProp(tmp, "descnum"));
      xd->keyword=str_dup(xmlGetProp(tmp, "keyword"));
      if (!tmp->children || !tmp->children->content || !*tmp->children->content) {
        continue;
      }
      xd->desc=str_dup(tmp->children->content);
      xd->next=NULL;
      add_playerxdesc(xd, ch);      
    }
  }
}

static void xml_to_markedmap(xmlNodePtr list,struct char_data *ch)
{
  int num;
  int empty_strings = 0;
  xmlNodePtr tmp;

  for (tmp = list->children; NULL != tmp; tmp = tmp->next) {
    if (!strcasecmp(tmp->name,"maploc")) {
      num = xmlAtoi(xmlGetProp(tmp,"pos"));
      if (!tmp->children || !tmp->children->content || !*tmp->children->content) {
	empty_strings++;
	continue;
      }
      GET_MAPPED(ch, num)=atol(tmp->children->content);      
    }
  }
}
/*
static void xml_to_ignorelist(xmlNodePtr list,struct char_data *ch)
{
  int num;
  int empty_strings = 0;
  xmlNodePtr tmp;

  for (tmp = list->childs; NULL != tmp; tmp = tmp->next) {
    if (!strcasecmp(tmp->name,"listentry")) {
      num = xmlAtoi(xmlGetProp(tmp,"pos"));
      if (!tmp->childs || !tmp->childs->content || !*tmp->childs->content) {
    empty_strings++;
    continue;
      }
      
      strcpy(ch->player_specials->ignorelist[num-empty_strings],tmp->childs->content);
    }
  }
} */


/*
 * Parses the <pspecial> node
 */
static void xml_to_playerspecials(xmlNodePtr node, struct char_data *ch)
{
  xmlNodePtr temp;
  xmlNodePtr temp2;
  long num; //tmp;
  char *ptr ;
  
  if (!node || !ch) {
    return;
  }

  if (strcasecmp("pspecial", node->name)) {
    return;
  }

  GET_OLC_ZONE(ch)   = xmlAtol(xmlGetProp(node,"olczone"));
  IS_BRANDED(ch)     = xmlAtol(xmlGetProp(node,"branded"));
  IS_BRANDED(ch)     = (-1 == IS_BRANDED(ch)) ? 0 : IS_BRANDED(ch);
  GET_WIMP_LEV(ch)   = xmlAtol(xmlGetProp(node,"wimpy"));
  GET_INVIS_LEV(ch)  = xmlAtoi(xmlGetProp(node,"invis"));

  /* Only set this if we have a 'proper' loadroom */
  GET_LOADROOM(ch)   = xmlAtoi(xmlGetProp(node, "loadroom"));

  GET_POWER(ch)      = xmlAtoi(xmlGetProp(node,"powerpoints"));
  GET_LAST_EMOTE(ch) = xmlAtoi(xmlGetProp(node,"emoteCnt")); 
  GET_AUTHORIZED(ch) = xmlAtoi(xmlGetProp(node,"auth"));

  for (temp = node->children; temp; temp = temp->next) {
    if (!strcasecmp(temp->name, "skills"))
      xml_to_skills(temp, ch);
    else if (!strcasecmp(temp->name, "tells"))
      xml_to_tells(temp, ch);
    else if (!strcasecmp(temp->name, "rplog"))
      xml_to_rplog(temp, ch);
    else if (!strcasecmp("taughtn",temp->name)) { 
      TAUGHT(ch) = xmlAtoi(xmlGetProp(temp,"taught"));
      HPTAUGHT(ch)=xmlAtoi(xmlGetProp(temp,"hptaught"));
    }
    else if (!strcasecmp("commandgroups", temp->name))
      ch->player_specials->saved.command_groups = atol(temp->children->content);
    else if (!strcasecmp("extradescs", temp->name) && temp->children)
      xml_to_xdescs(temp,ch);
    else if (!strcasecmp("notifylist",temp->name) && temp->children)
      xml_to_notifylist(temp,ch);
    else if (!strcasecmp("markedmap",temp->name) && temp->children)
      xml_to_markedmap(temp,ch);
/*    else if (!strcasecmp("ignorelist",temp->name) && temp->childs)
      xml_to_ignorelist(temp,ch); */
    else if (!strcasecmp("doing", temp->name) && temp->children) 
      GET_DOING(ch) = strdup(temp->children->content);
    else if (!strcasecmp(temp->name, "conditions") && temp->children)
      xml_to_conditions(temp, ch);
    else if (!strcasecmp(temp->name, "freeze") && temp->children)
      GET_FREEZE_LEV(ch) = atoi(temp->children->content);
    else if (!strcasecmp(temp->name, "badpws") && temp->children)
      GET_BAD_PWS(ch) = atoi(temp->children->content);
    else if (!strcasecmp(temp->name, "servant")  && temp->children)
      sprintf(GET_SERVANT(ch), temp->children->content);
    else if (!strcasecmp("channeling", temp->name) && temp->children)
      xml_to_channeling(temp, ch);
    else if ( !strcasecmp("blademastery", temp->name) && temp->children)
      GET_BM(ch) = atoi(temp->children->content);
    else if (!strcasecmp("aiel", temp->name)) {
    } else if (!strcasecmp("taveren",temp->name)) {
      GET_TAVEREN(ch) = 1;
    } else if (!strcasecmp("wolfkin",temp->name)) {
       GET_WOLFKIN(ch) = 1;
       if ((ptr = xmlGetProp(temp, "metKindred")) && !strcasecmp(ptr, "yes")) {
         GET_ISWOLFKIN(ch) = 1;
	 free(ptr);
       }
    } else if (!strcasecmp("nobility",temp->name)) {
      SET_NOBILITY(ch,  xmlGetProp(temp, "rank"));
    } else if (!strcasecmp("drfflags",temp->name)) {
      GET_DFINTR(ch) = xmlAtoi(xmlGetProp(temp, "flag"));
    } else if (!strcasecmp("ajah",temp->name)) {
      GET_AJAH(ch) = search_block_case((ptr = xmlGetProp(temp, "colour")), ajah_colors, 1)+1;
      if (ptr) free(ptr);
    } else if (!strcasecmp("ogier",temp->name)) {
    }
    else if (!strcasecmp("bonds",temp->name))
    {
      for (temp2 = temp->children; temp2; temp2 = temp2->next) {
        if (!strcasecmp(temp2->name, "bond")) {
          if ((ptr = xmlGetProp(temp2, "type")) && xmlAtoi(xmlGetProp(temp2, "id")) != -1)
          {
            if (!strcasecmp(ptr, "bonded")) {
              GET_BONDED(ch) = xmlAtol(xmlGetProp(temp2, "id"));
            } else if (!strcasecmp(ptr, "asha")) {
              GET_ASHABONDED(ch) = xmlAtol(xmlGetProp(temp2, "id"));
            } else if (!strcasecmp(ptr, "minion")) {
              GET_MINION(ch) = xmlAtol(xmlGetProp(temp2, "id"));
            }
	    free(ptr);
          }
        }
      } 
    } // bonds

    else if (!strcasecmp("flags",temp->name)) {

      for (temp2 = temp->children; temp2; temp2 = temp2->next) {
        if (!(ptr = xmlGetProp(temp2, "flag")))
          continue;

        num = atol(ptr);

        if (!strcasecmp(temp2->name, "permanent")) {
          GET_PERMANENT(ch) = num;
        } else if (!strcasecmp(temp2->name, "prefs")) {
          PRF_FLAGS(ch) = num;
        } else if (!strcasecmp(temp2->name, "prefs2")) {
          PRF2_FLAGS(ch) = num;
        } else if (!strcasecmp(temp2->name, "syslog")) {
          EXTENDED_SYSLOG(ch) = num;
        } else if (!strcasecmp(temp2->name, "autoflags")) {
          AUTO_FLAGS(ch) = num;
          if ((PLR_FLAGS(ch) = xmlAtol(xmlGetProp(temp2,"act"))) == -1)
	    PLR_FLAGS(ch) = 0;
	  if ((AFF_FLAGS(ch) = xmlAtol(xmlGetProp(temp2,"affs"))) == -1)
	    AFF_FLAGS(ch) = 0;
        }
	free(ptr);
      }
    } // flags

    else if (!strcasecmp("questpoints",temp->name)) {
      GET_QP(ch) = xmlAtoi(xmlGetProp(temp, "amount"));
    } else if (!strcasecmp("subdue",temp->name)) {
      GET_SUBDUE(ch) = 1;
    } else if (!strcasecmp("lines",temp->name)) {
      GET_LINES(ch) = xmlAtoi(xmlGetProp(temp, "value"));
    } else if (!strcasecmp("prefix", temp->name)) {
      GET_PREFIX(ch) = strdup(temp->children->content);
    } else if (!strcasecmp("spec_skills",temp->name)) {
      if ((GET_SPSKILLS1(ch) = xmlAtol(xmlGetProp(temp,"spec1"))) == -1)
	GET_SPSKILLS1(ch) = 0;

      if ((GET_SPSKILLS2(ch) = xmlAtol(xmlGetProp(temp,"spec2"))) == -1)
	GET_SPSKILLS2(ch) = 0;

    } else if (!strcasecmp("talents", temp->name)) {
      if ((GET_PRIMTALENTS1(ch) = xmlAtol(xmlGetProp(temp, "primary1"))) == -1)
	GET_PRIMTALENTS1(ch) = 0;

      if ((GET_PRIMTALENTS2(ch) =xmlAtol(xmlGetProp(temp, "primary2"))) == -1)
	GET_PRIMTALENTS2(ch) = 0;

      if ((GET_SECTALENTS1(ch) = xmlAtol(xmlGetProp(temp, "secondary1"))) == -1)
	GET_SECTALENTS1(ch) = 0;

      if ((GET_SECTALENTS2(ch) = xmlAtol(xmlGetProp(temp, "secondary2"))) == -1)
	GET_SECTALENTS2(ch) = 0;
    }
  }
}


/*
 * Parses the <cspecial> node
 */
static void xml_to_board(xmlNodePtr node, struct char_data *ch)
{
  struct board_stamp  *bstamp;
  if (!node || !ch)
    return;

  if (strcasecmp("board", node->name))
    return;

  CREATE( bstamp, struct board_stamp, 1 );
  bstamp->vnum = xmlAtoi( xmlGetProp(node,"vnum") );
  bstamp->stamp = xmlAtol( xmlGetProp(node,"stamp") );
  bstamp->next = ch->brd_stamp;
  ch->brd_stamp = bstamp;
}

/*
 * Parses the <cspecial> node
 */
static void xml_to_cspecial(xmlNodePtr node, struct char_data *ch)
{
  if (!node || !ch) {
    return;
  }

  if (strcasecmp("cspecial", node->name)) {
    return;
  }

  GET_IDNUM(ch) = xmlAtol(xmlGetProp(node, "idnum"));
}

/*
 * Parses the <playerdata> node
 */
static void xml_to_playerdata(xmlNodePtr node, struct char_data *ch)
{
  xmlNodePtr temp;
  int tmp = -1;
  char *p = "";

  if (!node || !ch) {
    return;
  }

  if (strcasecmp("playerdata", node->name))  {
    return;
  }

  p = xmlGetProp(node,"hometown");
  tmp = index_from_array(p,hometown,"\n",0);
  free(p);

  GET_RACE(ch) = tmp;
  ch->player.time.played = xmlAtol(xmlGetProp(node, "played"));
  ch->player.last_logon = xmlAtol(xmlGetProp(node, "lastLogon"));
  if (ch->player.last_logon == -1)
    ch->player.last_logon = time(0);

  GET_HEIGHT(ch) = xmlAtoi(xmlGetProp(node, "height"));
  GET_WEIGHT(ch) = xmlAtoi(xmlGetProp(node, "weight"));

  for (temp = node->children; temp; temp = temp->next) {
    if (!strcasecmp("password", temp->name))
      sprintf(GET_PASSWD(ch), temp->children->content);
    else if (!strcasecmp("birth", temp->name))
      ch->player.time.birth = atol(temp->children->content);
    else if (!strcasecmp("lastLogon", temp->name))
      ch->player.time.logon = atol(temp->children->content);
    else if (!strcasecmp("host", temp->name))
      ch->tempHost = strdup(temp->children->content);
/* To come...there is no ch->desc at this time
    else if (!strcasecmp("host", temp->name))
      sprintf(ch->desc->host, temp->childs->content);
*/
  }
}


struct char_data *raw_load(char *name, int old);

struct char_data *xmlload_char(char *name)
{
  struct char_data *ch = raw_load(name, FALSE);
 
  if (NULL == ch)
    return NULL; // no char found to load!

  GET_PFILEPOS(ch) = find_name(GET_RNAME(ch));

/* Now we should have a valid char_data struct, check if it's ID=1 */
  if (1 == GET_IDNUM(ch) && (LVL_IMPL != GET_LEVEL(ch))) {
    GET_LEVEL(ch) = LVL_IMPL;
  }

  GET_KNOW(ch) = BITVECTOR(MAX_ROOMS_ON_MUD);
  read_know(ch);
  read_retainers(ch);
  /**
   * Not good..WTF whas I thinking of...Will cause AC to be 100 (worst) even after
   * affects and whatnot has been read...*argh*!
   */
  //  GET_AC(ch) = 100; // 100 is base AC, -not- 0!
  return ch;
}

static void xml_to_descriptions(struct char_data *ch,xmlNodePtr descs)
{
  xmlNodePtr temp;
  int pos;

  GET_CURRENTDESC(ch) = xmlAtoi(xmlGetProp(descs,"current"));
  for (temp = descs->children; NULL != temp; temp = temp->next) {
    if (!strcasecmp(temp->name,"desc")) {
      pos = xmlAtoi(xmlGetProp(temp,"pos"));

      if (pos < 0 || pos >= MAX_DESCRIPTIONS)
        continue;

      if (temp->children) {
        GET_DESCNUM(ch,pos) = str_dup(
         ((temp->children->content) ? 
          (char *)temp->children->content :
  	  "This space for rent!\r\n"));
      } else 
	GET_DESCNUM(ch,pos) = str_dup("This space for rent!\r\n");
    }
  }
}

/*
 * Loads a character with a given name from XML file.
 * Parameters: The name of the requested player
 * Returns: The loaded character, or null if the name is null or
 *          the file can't be found or the xml file can't be parsed.
 */
struct char_data *raw_load(char *name, int old)
{
  struct char_data *ch;
  struct player_special_data *player_specials;
  int i;
  char fname[200], *ptr;
  char xlogbuf[MAX_STRING_LENGTH];
  xmlDocPtr doc;
  xmlNodePtr root, temp;

  if (!name || !*name) {
    return NULL;
  }

  if (!get_filename(name, fname, (old ? OLD_PLR_FILE : PLR_FILE))) {
    return NULL;
  }

  if (!(doc = xmlParseFile(fname))) {
    return NULL;
  }
  
  if (!(root = doc->children)) {
    xmlFreeDoc(doc);
    return NULL;
  }

  if (strncasecmp("player", root->name, strlen(root->name)))  {
    xmlFreeDoc(doc);
    return NULL;
  }
  
  ch = (struct char_data *) calloc(1, sizeof(struct char_data));
  GET_REAL_AC(ch) = 100;  // Here's the place to 'null' AC...*sigh*

  player_specials = (struct player_special_data *) calloc(1, sizeof(struct player_special_data));
  ch->player_specials = player_specials;

  GET_CURRENTDESC(ch) = 0;
  for (i = 0; i < MAX_DESCRIPTIONS; i++)
    GET_DESCNUM(ch,i) = NULL;

  if (!(ptr = xmlGetProp(root, "name"))) {
    xmlFreeDoc(doc);
    free(ch->player_specials);
    free(ch);
    alog("SYSERR: Playerfile %s lacks <name> tag. Can't load player.", fname);
    return NULL;
  }
  ch->player.name = ptr;

  if ((ptr = xmlGetProp(root, "dname"))) {
    GET_DNAME(ch) = ptr;
    GET_DISGUISED(ch) = TRUE;
  }

  GET_SEX(ch) = index_from_array((ptr = xmlGetProp(root, "gender")), genders, NULL, 3);
  if (ptr) free(ptr);
  GET_CLASS(ch) = index_from_array((ptr = xmlGetProp(root, "class")), pc_class_types, 0, NUM_CLASSES+1);
  if (ptr) free(ptr);
  GET_LOADROOM(ch) = xmlAtoi(xmlGetProp(root, "inroom"));
  ch->in_room = -1;

  GET_LEVEL(ch) = xmlAtoi(xmlGetProp(root, "level"));
  if (GET_LEVEL(ch) == -1)
    GET_LEVEL(ch) = 1;

  /* For DGS */
  CREATE(SCRIPT(ch), struct script_data, 1);
  
  /* to make sure they are set to normal if they don't have a mycolor */
  (void)strcpy(GET_MYCOLOR(ch), "&n");

  for (temp = root->children; temp; temp = temp->next) {
    if (!strcasecmp("desc", temp->name) && temp->children)
      GET_RDESC(ch) = strdup(temp->children->content);
    else if (!strcasecmp("descriptions",temp->name) && temp->children)
      xml_to_descriptions(ch,temp);
    else if (!strcasecmp("prompt",temp->name))
      GET_CPROMPT(ch) = strdup(temp->children->content);
    else if (!strcasecmp("title", temp->name))
      GET_RTITLE(ch) = strdup(temp->children->content);
    else if (!strcasecmp("dtitle", temp->name))
      GET_DTITLE(ch) = strdup(temp->children->content);
    else if (!strcasecmp("email", temp->name) && temp->children)
      GET_CONTACTINFO(ch) = strdup(temp->children->content);
    else if (!strcasecmp("pretitle",temp->name) && temp->children)
      GET_PRETITLE(ch) = strdup(temp->children->content);
    else if (!strcasecmp("mycolor",temp->name) && temp->children)
      strcpy(GET_MYCOLOR(ch), temp->children->content);
    else if (!strcasecmp("background",temp->name) && temp->children)
      GET_BACKGROUND(ch) = strdup(temp->children->content);
    else if (!strcasecmp("playerdata", temp->name))
      xml_to_playerdata(temp, ch);
    else if (!strcasecmp("cspecial", temp->name))
      xml_to_cspecial(temp, ch);
    else if (!strcasecmp("board", temp->name))
      xml_to_board(temp, ch);
    else if (!strcasecmp("pspecial", temp->name))
      xml_to_playerspecials(temp, ch);
    else if (!strcasecmp("abilities", temp->name))
      xml_to_abilities(temp, ch);
    else if (!strcasecmp("pdata", temp->name))
      xml_to_pdata(temp, ch);
    else if (!strcasecmp("affections", temp->name))
      xml_to_affects(temp, ch);
    else if (!strcasecmp("immstuff", temp->name))
      xml_to_immstuff(temp, ch);
    else if (!strcasecmp("dgvariables", temp->name))
      XML_to_DGVariables(temp, ch);
  }

  ch->was_in_room = -1;
  /*
   * No NULL titles. 
   */
  if (!ch->player.title) {
    ch->player.title = calloc(1,sizeof(char));
    *(ch->player.title) = '\0';
  }

  if( GET_LEVEL( ch ) < LVL_IMMORT && !PLR_FLAGGED(ch,PLR_NOGAIN) ) {
    for( i=1; i<=MAX_SKILLS; i++ ) {
      if( GET_SKILL(ch, i ) && !strcmp( spells[i], "!UNUSED!" ) ) { 
	int percent = MIN(GET_SKILL( ch, i ), 70);
	int pracs = percent / 20;
	SET_SKILL( ch, i, 0);
	sprintf( xlogbuf, "%s has redeamed %d practices for !UNUSED! skill %d.", ch->player.name, pracs, i );
	xlog( SYS_SKILLSET, LVL_IMMORT, xlogbuf, TRUE );
      /* This should be unescesary, sinc redemption is done when reading player_index...
       *
	sprintf( xlogbuf, "You have redeamed %d practices for deleted skill %d.", pracs, i );
	send_to_char( xlogbuf, ch );
       */
        xmlsave_char(ch);
      }	
    }
  }
  xmlFreeDoc(doc);

  return ch;
}

/*
 * Updates ptable with <ch>s new data
 */
void update_p_table(struct char_data *ch) 
{
  int nr = 0;

  if (ch == NULL) 
  {
    alog("SYSERR: update_p_table: ch == NULL");
    return;
  }
    
  nr = GET_PFILEPOS(ch);
   
  if (nr < 0 || nr > top_of_p_table) 
  {
    alog("SYSERR: update_p_table: pfilepos out of range for %s (nr = %d, top = %d)!", GET_RNAME(ch),nr,top_of_p_table);
    return;
  }

  if (GET_IDNUM(ch) != player_table[nr].id) 
  {
    alog("SYSERR: update_p_table: id num's don't match: %d, %d for plr %d, %s!", GET_IDNUM(ch), player_table[nr].id, nr, GET_NAME(ch)); 
    return;
  }

  if (player_table[nr].name)
    free(player_table[nr].name);
  if (player_table[nr].dname)
    free(player_table[nr].dname);
  player_table[nr].dname = NULL;

  player_table[nr].name = str_dup(GET_RNAME(ch));
  if (GET_DISGUISED(ch))
    player_table[nr].dname = str_dup(GET_DNAME(ch));
  
  player_table[nr].sex   = GET_SEX(ch);
  player_table[nr].class = GET_CLASS(ch);
  player_table[nr].level = GET_LEVEL(ch);
    
  player_table[nr].deleted = PLR_FLAGGED(ch, PLR_DELETED);
  player_table[nr].race = GET_RACE(ch);   
  player_table[nr].played = ch->player.time.played;
  player_table[nr].bonded = GET_BONDED(ch);
  player_table[nr].minion = GET_MINION(ch);
  player_table[nr].ashabonded = GET_ASHABONDED(ch);
  player_table[nr].disguised = GET_DISGUISED(ch);
  player_table[nr].bm = GET_BM(ch);
  player_table[nr].last_logon =  ch->player.time.logon;
  player_table[nr].multi = PLR_FLAGGED( ch, PLR_CANMULTI );
}

static const char *__pfile_dirs[] = 
{
  "./plrfiles/A-E",
  "./plrfiles/F-J",
  "./plrfiles/K-O",
  "./plrfiles/P-T",
  "./plrfiles/U-Z",
  "./plrfiles/ZZZ",
  NULL
};

/*
 * Builds a playertable by scanning all pfile dirs. 
 */
int load_playertable(void)
{
  int i,cnt,c2;
  DIR *dir;
  struct dirent *tdir;
  char *ptr,name[1024];
  struct char_data *tmp;
  struct player_index_element *p;
  int bid = 0, delete = 0;

  //there should never be a player_table when this is called but just in
  //case, check for one and free all memory if there is.
  if (player_table) 
  { 
    for (i = 0; i <= top_of_p_table; i++) 
      if (player_table[i].name)
        free(player_table[i].name);
    free(player_table);
  }
  player_table = NULL;
  top_of_p_table = 0; 

  //scan directories to count number of player files to alloc the proper
  //sized player_table
  cnt = 0; 
  for (i = 0; NULL != __pfile_dirs[i]; i++) 
  {
    if (NULL == (dir = opendir(__pfile_dirs[i]))) 
    {
      alog("ERROR: load_playertable(): couldn't read pfile dir: %s!",__pfile_dirs[i]);
      exit(1);
    }
    while ((tdir = readdir(dir))) 
    {
      if (!strcmp(tdir->d_name, ".") || !strcmp(tdir->d_name, "..")) 
        continue;

      ptr = strstr(tdir->d_name, ".plr");
      if (!ptr || strcmp(ptr, ".plr")) 
        continue;

      cnt++;
    }
    closedir(dir);
  }

  //no players so end function here
  if (!cnt) 
    return 0;

  //create the player_table
  player_table = calloc(cnt,sizeof(struct player_index_element));
  top_of_p_table = cnt - 1; /* player_table is indexed zero-based so top_of_p_table is one less than 'normal' */

  //start some sanity check 
  //  alog("Before: topId %d, bond_id %d, asha_id: %d, minion_id: %d",top_idnum, top_bond_id, top_ashabond_id, top_minion_id);
  top_bond_id = top_idnum = 0; 
  top_minion_id = top_ashabond_id = 1;  

  //scan each directory, this time loading the file
  c2 = 0; 
  for (i = 0; NULL != __pfile_dirs[i]; i++) 
  {
    if (NULL == (dir = opendir(__pfile_dirs[i]))) 
    {
      alog("ERROR: load_playertable(): couldn't read pfile dir: %s!",__pfile_dirs[i]);
      exit(1);
    }
    while ((tdir = readdir(dir))) 
    {
      if (!strcmp(tdir->d_name, ".") || !strcmp(tdir->d_name, "..")) 
        continue;
      
      ptr = strstr(tdir->d_name, ".plr");
      if (!ptr || strcmp(ptr, ".plr")) 
        continue;

      /*
       * Must be a plr-file here, parse it (or try to at least)
       */
      snprintf(name, strlen(tdir->d_name) - 3, "%s", tdir->d_name);

      if (NULL == (tmp = raw_load(name, FALSE))) 
      {
        alog("Failed to read pfile %s\r\n",name);
        continue;
      }

      GET_CHAR_GUILDS(tmp) = NULL;
      fetch_char_guild_data(tmp);
      if (PLR_FLAGGED(tmp, PLR_DELETED))
        delete = 1;
      else if (difftime(time(0), (time_t)tmp->player.time.logon) > 15552000)
        delete = 2;
      else if (!GET_CHAR_GUILDS(tmp) && difftime(time(0), (time_t)tmp->player.time.logon) > 7776000)
        delete = 3;
      else
        delete = 0;

      remove_char_guild_data(tmp);
      if (delete && !PLR_FLAGGED(tmp, PLR_NODELETE)) 
      {
        sprintf(buf, "%s/%s", __pfile_dirs[i], tdir->d_name);
        sprintf(buf2, "%s/%s.old", __pfile_dirs[i], name);
        rename(buf, buf2);
        alog("Moved %s to %s [%ld:%d].", buf, buf2, tmp->player.time.logon, delete);
        continue;
      }

      p = &(player_table[c2]);
      c2++;
      p->name = str_dup(GET_RNAME(tmp));
      if (GET_DISGUISED(tmp))
        p->dname = str_dup(GET_DNAME(tmp));
  
      p->sex   = GET_SEX(tmp);
      p->class = GET_CLASS(tmp);
      p->level = GET_LEVEL(tmp);
    
      p->deleted = PLR_FLAGGED(tmp, PLR_DELETED);
      p->race = GET_RACE(tmp);   
      p->played = tmp->player.time.played;
      p->bonded = GET_BONDED(tmp);
      p->minion = GET_MINION(tmp);
      p->ashabonded = GET_ASHABONDED(tmp);
      p->disguised = GET_DISGUISED(tmp);
      p->bm = GET_BM(tmp);
      p->last_logon =  tmp->player.time.logon;
      p->id = GET_IDNUM(tmp);
      p->multi  = PLR_FLAGGED( tmp, PLR_CANMULTI );
      p->nogain = PLR_FLAGGED( tmp, PLR_NOGAIN );

      bid = p->bonded;
      REMOVE_BIT(bid,(1 << 30));
      top_bond_id = MAX(bid,top_bond_id);
      
      bid = p->ashabonded;
      REMOVE_BIT(bid,(1 << 30));
      top_ashabond_id = MAX(bid,top_ashabond_id);
      
      bid = p->minion;
      REMOVE_BIT(bid,(1 << 30));
      top_minion_id = MAX(bid,top_minion_id);

      top_idnum = MAX(top_idnum,GET_IDNUM(tmp));

      free_char(tmp);
    }
    closedir(dir);
  }

  RECREATE(player_table, struct player_index_element, c2);
  top_of_p_table = c2 - 1;

  alog("After: topId %d, bond_id %d, asha_id: %d, minion_id: %d",top_idnum, top_bond_id, top_ashabond_id, top_minion_id);

  return 1;
}

/*
 * Tells whether a character file (and thus the character) exists
 * Using access() should be better than doing an fopen()/fclose() pair of calls. 
 * one less syscall, no buffer setups, etc, etc.. which is done in those libcalls. 
 */
int char_exists(char *name)
{
  get_filename(name, buf, PLR_FILE);
  return (access(buf,F_OK) < 0) ? 0 : 1;
}

int string_length(char str[])
{
   int i;
   for(i=0; i<32760; i++)
   {
     if(str[i]=='\0')
     {
        return(i);
     }
   }
   return(i);
}

