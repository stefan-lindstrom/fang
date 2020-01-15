/******************************************************************************
 * $Id: act.other.c,v 1.33 2003/12/15 06:50:03 fang Exp $
 *   
 * File: act.other.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: act.other.c,v $
 *   Revision 1.33  2003/12/15 06:50:03  fang
 *   Tiek: Mens' skins will now tingle when females grasp.
 *
 *   Revision 1.32  2003/12/05 09:38:15  fang
 *   Tze: AFK and AFW flags now go away if you quit out, as requested by Kalenth. And tells buffer doesn't repeat your tell back to you when you send a linkdead or afk person a tesll, as requested by Marra.
 *
 *   Revision 1.31  2003/06/22 08:43:29  fang
 *   Removed all references to Claudia in do_toggle
 *
 *   Revision 1.30  2003/01/24 08:54:29  fang
 *   manaconv now uses a PRM_ flag tokeep track opf players who have used manaconv. A smallish bug fixed in do_stat_character() so that it nows properly displays PRM-flags for the stat'ed char.
 *
 *   Revision 1.29  2002/12/19 06:41:26  fang
 *   fisch: remove toggle oldscore
 *
 *   Revision 1.28  2002/11/07 04:02:01  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.27  2002/10/16 16:54:40  fang
 *   *** empty log message ***
 *
 *   Revision 1.26  2002/10/10 17:28:12  fang
 *   fisch: adding a SYSTEM ERROR class to xlog
 *
 *   Revision 1.25  2002/10/03 05:55:09  fischer
 *   fischer: moved ideas to be used as an old ideas and added newideas file
 *
 *   Revision 1.24  2002/10/02 17:11:20  fischer
 *   fischer: toggle timestamps - added to stat and typo fix
 *
 *   Revision 1.23  2002/10/02 05:07:27  fischer
 *   fischer: timestamp flag added for Ealasaid
 *
 *   Revision 1.22  2002/09/15 07:17:25  fang
 *   Fisch: Removed or commented out the debug messages used to find the
 *   guild channel bug.
 *
 *   Revision 1.21  2002/09/15 03:45:13  fang
 *   Drath: Added an IMM toggle for XLOG tags
 *
 *   Revision 1.20  2002/09/15 02:15:26  fang
 *   Drath: Fixed guild code bug which would cause random crashes.  Uptime should last days now.
 *
 *   Revision 1.19  2002/09/14 00:28:03  fang
 *   fisch: cleaned up some debugging code and added others to track that guilie
 *
 *   Revision 1.18  2002/09/13 20:15:04  fang
 *   Fisch: More debug messages surrounding "become" and "guilds"
 *
 *   Revision 1.17  2002/09/13 18:00:42  fang
 *   Drath: Small addition to MyColor and MyColor option added to Prompts.
 *
 *   Revision 1.16  2002/09/13 16:37:23  fang
 *   Drath: More new prompt additions and mcl edits
 *
 *   Revision 1.15  2002/09/13 15:34:06  fang
 *   Drath: More PROMPT additions
 *
 *   Revision 1.14  2002/09/10 19:00:43  fang
 *   Drath: Added %l (Level) to prompt
 *
 *   Revision 1.13  2002/09/10 14:45:32  fang
 *   Drath: Added Name variable to Prompt
 *
 *   Revision 1.12  2002/09/05 20:56:30  fang
 *   Drath: added CRETODO and SHOWCRETODO for MassyPants
 *
 *   Revision 1.11  2002/09/03 18:45:37  fang
 *   Drath: Manaconv bug wiping eq fixed.
 *
 *   Revision 1.10  2002/08/31 19:18:55  fang
 *   Drath: Added Reta SCORE and fixed some of Freds compile errors
 *
 *   Revision 1.9  2002/08/30 21:23:09  fang
 *   Put in retainers.
 *
 *   Revision 1.8  2002/08/28 15:07:41  fang
 *   Drath: Player Extradesc bug fix #2
 *
 *   Revision 1.7  2002/08/28 14:23:01  fang
 *   Drathus: Player XDesc bugfix #1
 *
 *   Revision 1.6  2002/08/27 21:31:30  fang
 *   Drath: Player Extra Descriptions
 *
 *   Revision 1.5  2002/08/27 21:29:39  fang
 *   Drath: Player Extra Descriptions
 *
 *   Revision 1.4  2002/08/20 21:03:50  fang
 *   Drath: Hunter GSKILLS
 *
 *   Revision 1.3  2002/08/04 09:39:55  fang
 *   Drath: SOCIALIDEA and SHOWSOCIALIDEA seperate from IDEA
 *
 *   Revision 1.2  2002/07/28 13:56:03  fang
 *   Removed dmlloc...just to test.
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.27  2002/07/06 07:05:12  fang
 *   Drath: IMMLOCK for Fred and 2nd to last XLOG commit
 *
 *   Revision 1.26  2002/06/20 03:07:10  fang
 *   Maz: RPTAG
 *
 *   Revision 1.25  2002/06/13 21:12:02  fang
 *   Maz: Small bug fix.
 *
 *   Revision 1.24  2002/06/11 19:55:02  fang
 *   Maz: Me blond.  All better.
 *
 *   Revision 1.23  2002/06/10 14:24:25  fang
 *   Maz: TOGGLE PCHAN added so you can turn off being added to pchans.
 *
 *   Revision 1.22  2002/06/06 13:06:15  fang
 *   *** empty log message ***
 *
 *   Revision 1.21  2002/05/27 10:52:48  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.20  2002/05/19 21:59:22  fang
 *   Maz: SCORE FILE <playername> is in.
 *
 *   Revision 1.19  2002/05/19 10:04:06  fang
 *   Maz: Fixed mort PRACTICE so that they see their own list for PRAC FILE instead of nothing... Yes, I'm blonde.
 *
 *   Revision 1.18  2002/05/19 09:57:30  fang
 *   Maz: Rewrote PRACTICE to support PRAC file <name>
 *
 *   Revision 1.17  2002/05/18 06:32:43  fang
 *   Maz: Added an automated mudmail note to players who's QPs have been adjusted while offline.
 *
 *   Revision 1.16  2002/05/17 02:30:15  fang
 *   Maz: AWARD will now AWARD a specified player even if not online.  Syslog shows OFFLINE award.
 *
 *   Revision 1.15  2002/04/04 04:52:50  fang
 *   Re-wrote process_input... fixed manaconv.
 *
 *   Revision 1.14  2002/03/18 09:49:37  fang
 *   Disguises saved when become'ing
 *
 *   Revision 1.13  2002/03/18 09:39:21  fang
 *   Quiting/BEcoming now properly closes private channels...
 *
 *   Revision 1.12  2002/03/18 08:25:41  fang
 *   Crash bug in toggle afw/afk then become'ing fixed.
 *
 *   Revision 1.11  2002/03/12 00:59:13  fang
 *   *** empty log message ***
 *
 *   Revision 1.10  2002/03/12 00:45:13  fang
 *   Removed long-winded become message in liue of smaller one line one
 *
 *   Revision 1.9  2002/03/11 11:01:42  fang
 *   removed social coloring
 *
 *   Revision 1.8  2002/03/11 10:50:59  fang
 *   xname fix
 *
 *   Revision 1.7  2002/03/08 14:31:07  fang
 *   Added message for room when 'becoming'...
 *
 *   Revision 1.6  2002/03/07 10:30:18  fang
 *   *** empty log message ***
 *
 *   Revision 1.5  2002/03/07 08:02:49  fang
 *   *** empty log message ***
 *
 *   Revision 1.4  2002/03/02 03:39:46  fang
 *   Changed char_desc size.
 *
 *   Revision 1.3  2002/02/27 02:17:40  fang
 *   Fixed disguise so it saves.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.53  2002/02/01 14:57:00  mud
 *   *** empty log message ***
 *
 *   Revision 1.52  2002/02/01 07:55:18  mud
 *   Bugfix to shortgate.
 *
 *   Revision 1.51  2002/01/30 14:37:45  mud
 *   Mino5r bugfixes. Wagons now pulled through gates. Notifylist expanded to 10 names, etc..
 *
 *   Revision 1.50  2002/01/30 09:20:43  mud
 *   shortgate added. Also made gate not choke on unknown rooms, if distance between rooms are not morre than 10 rooms
 *
 *   Revision 1.49  2002/01/29 15:42:13  mud
 *   Minor bugfixes and whishes from ideas list...
 *
 *   Revision 1.48  2002/01/28 15:29:05  mud
 *   Customizable prompts..
 *
 *   Revision 1.47  2002/01/28 08:52:56  mud
 *   Changed delete so that is a character has qps (raw, skills or eq)
 *   the normal delete proicedure will not work, and a warning is printed.
 *   Instead of "delete yes delete me" (qhich still works for no-qp chars)
 *   the qp-chars will have to type "delete yes delete me and my qps!".
 *
 *   Revision 1.46  2002/01/24 21:33:12  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.45.2.3  2002/01/23 21:49:37  mud
 *   Removed objsave.[c|h] (old objsave stuff).
 *   Removed obj_file_elem and rent structs.
 *   Removed final vestiges of old guild system, except parts used
 *   in guild.[c|h] for backwards compatibility.
 *
 *   Revision 1.45.2.2  2002/01/22 03:05:09  mud
 *   More cleanup. Removed extern decls from source files. Protected some DG headers from multiple inclision
 *
 *   Revision 1.45.2.1  2002/01/21 01:23:38  mud
 *   Code cleanup. Aiel clans/socities gone. Lot of old guild code gone.
 *
 *   Revision 1.45  2001/11/18 13:17:00  mud
 *   Spelling mistakes and etcetera.
 *
 *   Revision 1.44  2001/11/10 11:16:16  mud
 *   Temp fix for object status...
 *
 *   Revision 1.43  2001/09/06 15:20:15  mud
 *   *** empty log message ***
 *
 *   Revision 1.42  2001/08/23 18:04:23  mud
 *   *** empty log message ***
 *
 *   Revision 1.41  2001/08/23 14:32:21  mud
 *   *** empty log message ***
 *
 *   Revision 1.40  2001/08/21 16:14:34  mud
 *   *** empty log message ***
 *
 *   Revision 1.39  2001/08/21 15:12:36  mud
 *   Fixed sneak!
 *
 *   Revision 1.38  2001/08/20 13:49:46  mud
 *   *** empty log message ***
 *
 *   Revision 1.37  2001/08/20 10:59:42  mud
 *   Added 'become' code.
 *
 *   Revision 1.36  2001/08/20 10:26:54  mud
 *   *** empty log message ***
 *
 *   Revision 1.35  2001/05/28 04:10:50  mud
 *   *** empty log message ***
 *
 *   Revision 1.34  2001/05/08 09:00:20  mud
 *   disguise no longer takes 'backups' on descs, nor does there exists any specific disguise-desc either.
 *
 *   Revision 1.33  2001/05/04 05:39:12  mud
 *   Multiple descs now works, without disrupting old desc system.
 *
 *   Revision 1.32  2001/04/29 16:38:00  mud
 *   *** empty log message ***
 *
 *   Revision 1.31  2001/04/25 08:50:57  mud
 *   Email and background entry added to pfiles, commands for setting email/background, and a viewbground command added.
 *
 *   Revision 1.30  2001/04/15 02:30:14  mud
 *   Added a bundle of fixes and notifylist.
 *
 *   Revision 1.29  2001/04/04 18:35:27  mud
 *   Bug fixing
 *
 *   Revision 1.28  2001/03/17 10:40:46  mud
 *   New and Improved TAR.
 *
 *   Revision 1.27  2001/01/19 17:51:49  mud
 *   Added line numbers to the editor.
 *
 *   Revision 1.26  2001/01/11 17:18:18  mud
 *   *** empty log message ***
 *
 *   Revision 1.25  2000/12/11 12:14:18  mud
 *   Removed most references to PFILEPOS() get_char_ppos(), since with the xml code ppos will(may) change every reboot
 *
 *   Revision 1.24  2000/11/09 14:13:47  mud
 *   Getting closer to switching-dat with xml chars
 *
 *   Revision 1.23  2000/11/08 20:34:25  mud
 *   Added the todo list for imms
 *
 *   Revision 1.22  2000/11/08 08:25:39  mud
 *   Moved update_p_tabel() to xmlcharsave.[ch].
 *
 *   Revision 1.21  2000/11/06 15:13:14  mud
 *   *** empty log message ***
 *
 *   Revision 1.20  2000/10/26 22:32:38  mud
 *   Spelling fixed on pwimpy
 *
 *   Revision 1.19  2000/10/24 01:03:08  mud
 *   *** empty log message ***
 *
 *   Revision 1.18  2000/09/04 02:55:27  mud
 *   Warnings removed...
 *
 *   Revision 1.17  2000/08/21 03:27:07  mud
 *   *** empty log message ***
 *
 *   Revision 1.16  2000/08/14 19:11:55  mud
 *   The new guild system.
 *
 *   Revision 1.15  2000/08/04 09:42:38  mud
 *   finished changes to do_toggle, rmoved all old toggle commands (noooc, afk, goic, etc)
 *
 *   Revision 1.14  2000/07/30 15:04:55  mud
 *   added afk and afw to do_toggle
 *
 *   Revision 1.13  2000/07/29 09:37:48  mud
 *   added 'toggle blah' capabaility to do_toggle
 *
 *   Revision 1.12  2000/07/14 07:19:34  mud
 *   Added a call to write_aliases in do_save.
 *
 *   Revision 1.11  2000/04/15 23:18:22  mud
 *   A bunch of things....fix to club code -addition- of call to save_char
 *   in ACMD(do_save) *whap Fred* and that's it methinks.
 *
 *   Revision 1.10  2000/04/09 20:34:03  mud
 *   Changed player obj saving to XML format.
 *
 *   Revision 1.9  2000/03/30 02:15:12  mud
 *   Updated commentes for toggle display
 *
 *   Revision 1.8  2000/03/11 03:13:26  mud
 *   Added the command 'pray' for newbies at Edin's request.
 *
 *   Revision 1.7  2000/03/09 12:47:44  mud
 *   Code cleanup. Compiles without warnings now...
 *
 *   Revision 1.6  2000/03/09 05:06:41  mud
 *   Changed "song" to "sing"....visibly at least..internally still "song".
 *
 *   Revision 1.5  2000/03/09 04:55:05  mud
 *   Added song channel.
 *
 *   Revision 1.4  2000/03/09 04:06:49  mud
 *   Changed who func again!
 *   Added in "prefix" command...changed two header files in "structs.h" and "utils.h"
 *
 *   Revision 1.3  2000/03/08 11:41:46  mud
 *   New who func, minor modificantion to gossip command where new gossips are
 *   pre-pended to gossip now.
 *
 *   Revision 1.2  2000/02/21 14:00:15  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/*************************************************************************
*   File: act.other.c                                   Part of CircleMUD *
*  Usage: Miscellaneous player-level commands                             *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */
#include "skimming.h"
#include "class.h"
#include "alias.h"
#include "fight.h"
#include "magic.h"
#include "constants.h"
#include "config.h"
#include "conf.h"
#include "sysdep.h"
#include "act.h"
#include <sys/stat.h>
#include "spec_procs.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "spells.h"
#include "screen.h"
#include "xmlobjsave.h"
#include "xmlcharsave.h"
#include "guild_parser.h"
#include "guild.h"
#include "graph.h"
#include "mail.h"
#include "retainer.h"

struct affected_type clear_aff_var(struct affected_type af)
{
  memset((char *)&af,0,sizeof(struct affected_type));
  return af;
}

/*
 * I'll try this one for now, gotta clear ith with Da Boss(tm) before
 * enabling. 
 *
 * Idea: 1-3 hp/lvl, 
 * 1 prac / 20 mana?
 */
/*
 * Note I removed the reload of eq and pulled out taking the pointers to
 * 'store'. Peeps will still get all their eq removed but they can rewear
 * it themselves.
 */
ACMD(do_manaconv)
{
  const int max_hp = 3; // 1 -> 3 hp/lvl?
  int sum_hp = 0, sum_prac,i,t;
  struct obj_data *objs[NUM_WEARS];

  if (IS_NPC(ch)) {
    send_to_char("Go away... Mobs can't do this.\r\n", ch);
    return;
  }

  if (!cant_channel(ch)) {
    send_to_char("Exscuuuuse me! You'll need your mana, beeing a channeler and all....\r\n",ch);
    return;
  }

  if (GET_MAX_MANA(ch) <= 0 || IS_SET(GET_PERMANENT(ch), PRM_CONVED)) {
    send_to_char("Hmm....seems you've already \\c01have\\c00 converted your mana....\r\n",ch);
    return;
  }

  for (i = 0; i < NUM_WEARS; i++) {
    objs[i] = NULL;
    if (GET_EQ(ch, i))
      objs[i] = unequip_char(ch, i);
  }

  sum_prac = (int)((GET_MAX_MANA(ch) / 20.0)+0.5);
  GET_PRACTICES(ch) += sum_prac;

  GET_MANA(ch) = GET_MAX_MANA(ch) = 0;  


  for (i = 0; i < GET_LEVEL(ch); i++) {
    t = number(1,max_hp);
    GET_MAX_HIT(ch) += t;
    sum_hp += t;
  }
  asend_to_char(ch,"You gained %d HP and %d practice.\r\n",sum_hp,sum_prac);
  alog("%s (@lvl %d) converted mana and got %d HPs and %d practices",GET_NAME(ch),GET_LEVEL(ch),sum_hp,sum_prac);

  for (i = 0; i < NUM_WEARS; i++)
    if (objs[i])
      equip_char(ch, objs[i], i);
  SET_BIT(GET_PERMANENT(ch),PRM_CONVED);
  xmlsave_char(ch);
}

ACMD(do_dswitch)
{
  int pos;

  skip_spaces(&argument);

  if (!*argument) {
    asend_to_char(ch,"You currently have desc #%d as active description.\r\n",(GET_CURRENTDESC(ch)+1));
    return;
  }

  pos = atoi(argument);

  if (pos < 1 || pos > MAX_DESCRIPTIONS) {
    asend_to_char(ch, "Usage: dswitch <num> where: 1 <= num <= %d\r\n", MAX_DESCRIPTIONS);
    return;
  }

  if (NULL == GET_DESCNUM(ch,(pos - 1))) {
    sprintf(buf,"A typical, run of the mill %s.\r\n",GET_RNAME(ch));
    GET_DESCNUM(ch,(pos - 1)) = strdup(buf);
  }
  GET_CURRENTDESC(ch) = (pos - 1);
  asend_to_char(ch,"Description #%d set as your current description.\r\n",(GET_CURRENTDESC(ch)+1));
}

ACMD(do_wait)
{
  struct affected_type af;
  int delay;

  skip_spaces(&argument);

  if (!argument)
    return;
  if (!(delay = atoi(argument))) {
    send_to_char("Type in a number instead.\r\n", ch);
     return;
  }
  if ((delay < 0) || (delay > 300)) {
    send_to_char("Type in a reasonable number of seconds to wait, please.\r\n", ch);
    return;
  }
  af = clear_aff_var(af);
  if (IS_NPC(ch))
    return;
  af.bitvector2 = AFF2_HALT;
  af.duration = 2*delay;
  af.type = SKILL_SECOND_ATTACK; /* What the heck.... */
  affect_to_char(ch, &af);
}

ACMD(do_forget)
{
  int skill, trains,qps;

  skip_spaces(&argument);

  if (!*argument) {
    send_to_char("Usage: forget <skill name>\r\n",ch);
    return;
  }
  skill = find_skill_num(argument);

  if ((skill < 1) || (skill > MAX_SKILLS)) {
    send_to_char("Very funny...-\\c01NOT\\c00-\r\n",ch);
    return;
  }
  if (!GET_SKILL(ch,skill)) {
    asend_to_char(ch,"Hey, you don't know %s, so how can you forget it?\r\n",spells[skill]);
    return;
  }
  if (!IS_SET(spell_info[skill].type,SPELL_NORMAL|SKILL_NORMAL|SKILL_QUEST|SPELL_QUEST)) {
    asend_to_char(ch,"Hey. You can only forget normal skills/spell and quest "
        "skills/spells. And %s is neither!\r\n",spells[skill]);
    return;
  }
  if (IS_SET(spell_info[skill].type,SPELL_NORMAL|SKILL_NORMAL)) {
    qps    = 0;
    trains = spell_info[skill].cost[(int)GET_CLASS(ch)];
    trains = ((trains > 100) ? 5 : trains);
  }
  else {
    trains = 0;
    qps    = spell_info[skill].cost[(int)GET_CLASS(ch)];
  }
  GET_SKILL(ch, skill) = 0;
  if (trains >= 0)
    GET_TRAINS(ch) += trains;
  else 
    send_to_char("Since this skill costs a negative amount of trains, you won't get any trains back.\r\n", ch);
  if (qps >= 0)
    GET_QP(ch) += qps;
  else
    send_to_char("Since this skill costs a negative amount of quest points, you won't get any qp back.\r\n", ch);

  asend_to_char(ch,"Okay, you forget %s and get %d %s back.\r\n",spells[skill],((trains == 0) ? qps : trains),((trains == 0) ? "Questpoints" : "Trains"));
  alog("%s forgets %s and gets %d %s back.",GET_NAME(ch),spells[skill],((trains == 0) ? qps : trains),((trains == 0) ? "Questpoints" : "Trains"));
}  
 
ACMD(do_quit)
{
  struct descriptor_data *d, *next_d;

  if (IS_NPC(ch) || !ch->desc)
    return;

  if (any_owned_retainer_is_fighting(ch))
  { send_to_char("No. Not while any of your retainers is fighting.\r\n", ch);
    return;
  }

  if (SESS_FLAGGED(ch, SESS_EAVESDROP)) 
  { send_to_char("You move away from where you were and can no longer hear soft voices.\r\n", ch);
    (void)SESS_TOG_CHK(ch, SESS_EAVESDROP);
    rem_eavesdropper(ch, GET_EAVESDROPROOM(ch));
    GET_EAVESDROPROOM(ch) = 0;
  }
  
  if (!(IS_NPC(ch)) && (GET_SLEEPMOB(ch)))
    wakeup_sequence(ch); 

  if (NORMAL_DREAMER(ch)) 
  { if (ROOM_FLAGGED(ch->in_room, ROOM_DREAM)) 
    { REMOVE_BIT(ROOM_FLAGS(IN_ROOM(ch)), ROOM_DREAM);
      if (GET_DREAMENTERED(ch)) wakeup_sequence(GET_DREAMENTERED(ch));
    }
    wakeup_sequence(ch);
  }

  if (ch->player_specials->linking[0])
  { send_to_char("You must dissolve your link first!\r\n", ch);
    return;
  }

  if( PRF_FLAGGED(ch, PRF_QUEST) )
    (void)PRF_TOG_CHK(ch, PRF_QUEST);

  if (PRF_FLAGGED(ch, PRF_AFK))
    do_noafk(ch, 0, cmd, 0);

  if (PRF2_FLAGGED(ch, PRF2_AFW))
    do_noafw(ch, 0, cmd, 0);

  if (ch->player_specials->linked_by)
  { send_to_char("You cannot leave until you are released from your link!\r\n", ch);
    return;
  }

  if (subcmd != SCMD_QUIT && GET_LEVEL(ch) < LVL_IMMORT)
    send_to_char("You have to type quit--no less, to quit!\r\n", ch);
  else if (GET_POS(ch) == POS_FIGHTING)
    send_to_char("No way!  You're fighting for your life!\r\n", ch);
  else if (GET_POS(ch) < POS_STUNNED) 
  { send_to_char("You die before your time...\r\n", ch);
    die(ch, ch);
  } 
  else if (IS_AFFECTED(ch,AFF_NOQUIT)) 
    send_to_char("Sorry, you are not allowed to quit yet.\r\n",ch);
  else 
  { if (GET_PRIVATE(ch))
      do_private_channel(ch,"",0,SCMD_P_CLOSE);

    if (!cant_channel(ch) && (GET_LEVEL(ch) > 10)) 
    { send_to_char(REM_MAIN_TXT,ch);
      remove_char_maintained(ch);
      remove_room_maintained(ch);
    }

    if (DICING(ch) && char_still_online(DICING(ch))) 
      DICING(DICING(ch)) = NULL;
      
    write_aliases(ch);
    if (!GET_INVIS_LEV(ch))
      act("$n has left the game.", TRUE, ch, 0, 0, TO_ROOM);
    sprintf(buf, "%s has quit the game.", GET_NAME(ch));
    xlog(SYS_CONN, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), buf, TRUE);

    if (IS_AFFECTED(ch,AFF_MORALE))
      affect_from_char(ch,SKILL_MORALE);
    
    send_to_char("Goodbye, muddict...You will come back soon!\r\n", ch);
    
    /*
     * kill off all sockets connected to the same player as the one who is
     * trying to quit.  Helps to maintain sanity as well as prevent duping.
     */
    for (d = descriptor_list; d; d = next_d) 
    { next_d = d->next;
      if (d == ch->desc)
        continue;
      if (d->character && (GET_IDNUM(d->character) == GET_IDNUM(ch)))
        close_socket(d);
    }

    save_XML_playerfile(ch, CRASH_FILE);
    strip_char_of_all_eq(ch);
    extract_char(ch, FALSE);		/* Char is saved in extract char */

    if (GET_WAGON(ch) && ch->pulling)
      extract_obj(ch->pulling);
  }
}




ACMD(do_save)
{
  if (IS_NPC(ch) || !ch->desc)
    return;

  if (cmd) {
    sprintf(buf, "Saving %s.\r\n", GET_NAME(ch));
    send_to_char(buf, ch);
  }
  write_aliases(ch);
  save_XML_playerfile(ch, CRASH_FILE);
  save_char(ch, IN_ROOM(ch));
}


/* generic function for commands which are normally overridden by
   special procedures - i.e., shop commands, mail commands, etc. */
ACMD(do_not_here)
{
  send_to_char("Sorry, but you cannot do that here!\r\n", ch);
}



ACMD(do_sneak)
{
  struct affected_type af;
  byte percent;
  af = clear_aff_var(af);

  if (IS_AFFECTED(ch,AFF_SNEAK)) {
     send_to_char("You're already sneaking... duh :p\r\n", ch);
     return;
   }

  if (!GET_SKILL(ch,SKILL_SNEAK)) {
    send_to_char("You don't know how!\r\n",ch);
    return;
  }

  send_to_char("Okay, you'll try to move silently for a while.\r\n", ch);

  percent = generic_result_mod(ch,SKILL_SNEAK,NULL,FALSE,(GET_DEX(ch) - 15));	

  if ((percent == TOTAL_FUCKUP) || (percent == TOTAL_FAILURE))
    return;

  af.type = SKILL_SNEAK;
  af.duration = GET_LEVEL(ch);
  af.modifier = 0;
  af.location = APPLY_NONE;
  SET_BIT(af.bitvector, AFF_SNEAK);
  affect_to_char(ch, &af);
}


ACMD(do_locate)
{
   byte percent;
   int result, found=0;
   struct char_data *people;
   struct affected_type af;

   if(GET_NO_LOCATE(ch)>0 && GET_LEVEL(ch)<LVL_IMMORT)
      {
      send_to_char("You just searched the room a small while ago!\r\n",ch);
      return;
      }

   if(!GET_SKILL(ch,SKILL_LOCATE))
      {
      send_to_char("You really aren't good at this!\r\n",ch);
      return;
      }

   send_to_char("You begin searching the room carefully.\r\n", ch);

   GET_NO_LOCATE(ch)=2; 

   percent = generic_result_mod(ch,SKILL_HIDE,NULL,FALSE,(GET_INT(ch) - 15));

   if ((percent == TOTAL_FUCKUP) || (percent == TOTAL_FAILURE))
    {
    send_to_char("You didn't find any hidden people in this room.\r\n",ch);
    return;
    } 

   for (people = world[ch->in_room].people; people; people = people->next_in_room)
     {
        if (IS_AFFECTED(people, AFF_HIDE))
        {
           percent = number(0,100);
           result  = GET_LEVEL(ch) - GET_LEVEL(people);
           result += 1.5*GET_SKILL(ch, SKILL_LOCATE) - GET_SKILL(people,SKILL_HIDE);
     

           if (percent>result)
              {
              send_to_char("You didn't find any hidden people in this room.\r\n",ch);
              return;
              }           
           else
           {
              found=TRUE;

              af  = clear_aff_var(af);
    
              af.bitvector2 = AFF2_DISCOVERED;
              af.duration   = 2;
              af.type       = SKILL_LOCATE;
    
              affect_to_char(people,&af);
  
              REMOVE_BIT(AFF_FLAGS(people),AFF_HIDE);
              send_to_char("You sense someone hidding nearby! Look!\r\n",ch);
              act("$n has seen you!.", TRUE, ch, 0, people,TO_VICT);
             
              if((number(0,95)<3) && (GET_SKILL(ch,SKILL_LOCATE)<95))
              {
                send_to_char("Your skill in locate has improved!\r\n", ch);
                GET_SKILL(ch, SKILL_LOCATE)++;
              }
           }
        }     
     }

   if(!found)
     {
     send_to_char("You didn't find any hidden people in this room.\r\n",ch);
     return;
     }   


}

ACMD(do_hide)
{
  byte percent;

  if (!GET_SKILL(ch,SKILL_HIDE)) {
    send_to_char("You don't know how!\r\n",ch);
    return;
  }
  
  if(IS_AFFECTED2(ch,AFF2_DISCOVERED)) {
    send_to_char("You were just discovered hiding!\r\n",ch);
    return;
  }

  if(TICKS_IN_ROOM(ch)<2 && GET_LEVEL(ch)<LVL_IMMORT) {
    send_to_char("You haven't studied this area long enough yet.\r\n",ch);
    return;
  }

  send_to_char("You attempt to hide yourself.\r\n", ch);

  percent = generic_result_mod(ch,SKILL_HIDE,NULL,FALSE,(GET_INT(ch) - 15));  

  if ((percent == TOTAL_FUCKUP) || (percent == TOTAL_FAILURE))
    return;

  SET_BIT(AFF_FLAGS(ch), AFF_HIDE);
}

ACMD(do_steal)
{
  struct char_data *vict;
  struct obj_data *obj;
  char vict_name[MAX_INPUT_LENGTH], obj_name[MAX_INPUT_LENGTH];
  int percent, gold, eq_pos, pcsteal = 0, ohoh = 0;

  ACMD(do_gen_comm);

  argument = one_argument(argument, obj_name);
  one_argument(argument, vict_name);
  
  if (!GET_SKILL(ch,SKILL_STEAL)) {
    send_to_char("You don't know how!\r\n",ch);
    return;
  }

  if (ROOM_FLAGGED(ch->in_room, ROOM_PEACEFUL)) {
    send_to_char("Stealing is not allowed in this room. \r\n", ch);
    return;
  }
  
  if (!(vict = get_char_room_vis(ch, vict_name))) {
    send_to_char("Steal what from whom?\r\n", ch);
    return;
  } else if (vict == ch) {
    send_to_char("Come on now, that's rather stupid!\r\n", ch);
    return;
  }

if ( !IS_NPC(vict) ) {
  if (GET_LEVEL(ch) < 20 || GET_LEVEL(vict) < 20) {
    send_to_char("Both you and your victim must be over level 20 for this!\r\n", ch);
    return;
  }

  if (!IS_SET(PRF_FLAGS(ch),PRF_IC) || !IS_SET(PRF_FLAGS(vict),PRF_IC))
    {
    send_to_char("Both you and your victim must be In Character for this!\r\n",ch);
    return;
    }
}

  percent = generic_result_mod(ch,SKILL_STEAL,NULL,FALSE,(GET_DEX(ch) - 15));

  if (GET_POS(vict) < POS_SLEEPING)
    percent = TOTAL_SUCCESS;		/* ALWAYS SUCCESS */

  if (!pt_allowed && !IS_NPC(vict))
    pcsteal = 1;

  /* NO NO With Imp's and Shopkeepers, and if player thieving is not allowed */
  if (GET_LEVEL(vict) >= LVL_IMMORT || pcsteal ||
      GET_MOB_SPEC(vict) == shop_keeper)
    percent = TOTAL_FUCKUP;		/* Failure */

  if (str_cmp(obj_name, "coins") && str_cmp(obj_name, "gold")) {

    if (!(obj = get_obj_in_list_vis(vict, obj_name, vict->carrying))) {

      for (eq_pos = 0; eq_pos < NUM_WEARS; eq_pos++)
    if (GET_EQ(vict, eq_pos) &&
        (isname(obj_name, GET_EQ(vict, eq_pos)->name)) &&
        CAN_SEE_OBJ(ch, GET_EQ(vict, eq_pos))) {
      obj = GET_EQ(vict, eq_pos);
      break;
    }
      if (!obj) {
    act("$E hasn't got that item.", FALSE, ch, 0, vict, TO_CHAR);
    return;
      } else {			/* It is equipment */
    if ((GET_POS(vict) > POS_STUNNED)) {
      send_to_char("Steal the equipment now?  Impossible!\r\n", ch);
      return;
    }
        else if (IS_SET(GET_OBJ_EXTRA(obj),ITEM_NODROP) || GET_OBJ_QCOST(obj)) {
          act("You try to steal $p from $N but it doesn't seems to work?",
           FALSE,ch,obj,vict,TO_CHAR);
          act("$n tries to steal $p from $N, but it doesn't seems to work...",
           FALSE,ch,obj,vict,TO_NOTVICT);
          act("$N just tried to steal $p from you, silly bugger, doesn't know"
           "that you can't steal Quest- nor Guild objects...",FALSE,vict,obj,
       ch,TO_CHAR);
      return;
        } 
        else {
      act("You unequip $p and steal it.", FALSE, ch, obj, 0, TO_CHAR);
      act("$n steals $p from $N.", FALSE, ch, obj, vict, TO_NOTVICT);
      obj_to_char(unequip_char(vict, eq_pos), ch);
    }
      }
    } 
    else {			/* obj found in inventory */
      if (IS_SET(GET_OBJ_EXTRA(obj),ITEM_NODROP) || GET_OBJ_QCOST(obj)) {
        act("You try to steal $p from $N but it doesn't seems to work?",
          FALSE,ch,obj,vict,TO_CHAR);
        act("$n tries to steal $p from $N, but it doesn't seems to work...",
           FALSE,ch,obj,vict,TO_NOTVICT);
          act("$N just tried to steal $p from you, silly bugger, doesn't know"
           "that you can't steal Quest- nor Guild objects...",FALSE,vict,obj,
           ch,TO_CHAR);
        return;
      }

      percent += GET_OBJ_WEIGHT(obj);	/* Make heavy harder */

      if (AWAKE(vict) && ((percent == TOTAL_FUCKUP)||(percent == TOTAL_FAILURE))) {
    ohoh = TRUE;
    act("Oops..", FALSE, ch, 0, 0, TO_CHAR);
    act("$n tried to steal something from you!", FALSE, ch, 0, vict, TO_VICT);
    act("$n tries to steal something from $N.", TRUE, ch, 0, vict, TO_NOTVICT);
      } else {			/* Steal the item */
    if ((IS_CARRYING_N(ch) + 1 < CAN_CARRY_N(ch))) {
      if ((IS_CARRYING_W(ch) + GET_OBJ_WEIGHT(obj)) < CAN_CARRY_W(ch)) {
        obj_from_char(obj);
        obj_to_char(obj, ch);
        send_to_char("Got it!\r\n", ch);
      }
    } else
      send_to_char("You cannot carry that much.\r\n", ch);
      }
    }
  } else {			/* Steal some coins */
    if (AWAKE(vict) && ((percent == TOTAL_FUCKUP)||(percent == TOTAL_FAILURE))) {
      ohoh = TRUE;
      act("Oops..", FALSE, ch, 0, 0, TO_CHAR);
      act("You discover that $n has $s hands in your wallet.", FALSE, ch, 0, vict, TO_VICT);
      act("$n tries to steal gold from $N.", TRUE, ch, 0, vict, TO_NOTVICT);
    } else {
      /* Steal some gold coins */
      gold = (int) ((GET_GOLD(vict) * number(1, 10)) / 100);
      gold = MIN(1782, gold);
      if (gold > 0) {
    GET_GOLD(ch) += gold;
    GET_GOLD(vict) -= gold;
        if (gold > 1) {
      sprintf(buf, "Bingo!  You got %d gold coins.\r\n", gold);
      send_to_char(buf, ch);
    } else {
      send_to_char("You manage to swipe a solitary gold coin.\r\n", ch);
    }
      } else {
    send_to_char("You couldn't get any gold...\r\n", ch);
      }
    }
  }

  if (ohoh && IS_NPC(vict) && AWAKE(vict))
    hit(vict, ch, TYPE_UNDEFINED);
}

ACMD(do_delete)
{
  long gear = 0, skills = 0;
  void do_newdisguise(struct char_data *ch, char *arg, int cmd, int subcmd);

  if (IS_NPC(ch)) {
    send_to_char("You can't delete yourself, you are no player!", ch);  
    return;
  }

  if (GET_DISGUISED(ch))
    do_newdisguise(ch, "off", 0, 0);

  get_qs(ch,&skills,&gear);
  if (skills || gear || GET_QP(ch)) {
    if (strcasecmp(argument," yes delete me and my qps!")) {
      asend_to_char(ch,"You have %ld quest points on this char, if you want to\r\n"
            "delete this character, you have to type the following: \r\n"
            "'delete yes delete me and my qps!'. \r\n\r\n\\c01WARNING:\\c00"
            "once you've done this, there's no way to get this character "
            "back!\r\n",(skills+gear+(long)GET_QP(ch)));
      return;
    }
  } else {
    if (strcasecmp(argument, " yes delete me")) {
      send_to_char("To really delete yourself, you have to type the following:\r\n", ch);
      send_to_char("'delete yes delete me'. WARNING: After you have typed\r\n", ch);
      send_to_char("this, there is -no- way back with this character!!\r\n", ch);  
      return;
    }
  }
  /*
   * Character has typed in correctly, despite warnings, check for ice...
   */
  if (PLR_FLAGGED(ch, PLR_FROZEN)) {
    send_to_char("You try to kill yourself, but the ice stops you.\r\n", ch);
    send_to_char("Character not deleted.\r\n\r\n", ch);
    return;
  } 

  /*
   * No ice. Correct args typed in. Time to erase....
   */
  sprintf(buf, "Character '%s' deleted!\r\n", GET_NAME(ch));
  send_to_char(buf, ch);
  delete_char_guilds(ch);

  if (GET_LEVEL(ch) < LVL_GRGOD)
    SET_BIT(PLR_FLAGS(ch), PLR_DELETED);

  sprintf(buf, "%s (lev %ld) has self-deleted.", GET_NAME(ch), GET_LEVEL(ch));
  xlog(SYS_CONN, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), buf, TRUE);
  extract_char(ch, TRUE);
}

ACMD(do_practice)
{
  struct char_data * vict = 0;  

  half_chop(argument, buf1, buf2);
  
  if (!*buf1) {
    list_skills(ch, ch);
    return;   
  }
  else if (is_abbrev(buf1, "file")) {
    if (GET_LEVEL(ch) > LVL_IMMORT) {
      if (NULL != (vict = xmlload_char(buf2))) {
        if (GET_LEVEL(ch) < GET_LEVEL(vict)) {
          send_to_char("You can't do that.\r\n", ch);
        } else {
          list_skills(ch, vict);
        }
      } else {
        send_to_char("No such player found.\r\n", ch);
      }
      free_char(vict);
    } else {
      list_skills(ch, ch);
    }
  } else {
    if (GET_LEVEL(ch) > LVL_IMMORT && (vict=get_char_vis(ch,buf1)) )
    {
      if (GET_LEVEL(ch) < GET_LEVEL(get_char_vis(ch, buf1)))
        send_to_char("You can't do that.\r\n", ch);
      else
        list_skills(ch, vict);
      return;
    }
    if (GET_LEVEL(ch) > LVL_IMMORT && !(vict=get_char_vis(ch,buf1)) )
    {
      send_to_char("No such person around.\r\n", ch);
      return;
    }
    list_skills(ch, ch);
  }
    
/*
 * Old PRACTICE routine handler code
 *
 *  one_argument(argument, arg);
 * vict = get_char_vis(ch,arg);
 * sort_spells();
 * if ( (!*arg) &&  (GET_LEVEL(ch) < LVL_IMMORT) )
 *    list_skills(ch,ch);
 * else
 * {
 * if ( (*arg) && (GET_LEVEL(ch) > LVL_IMMORT) && (vict = get_char_vis(ch,arg)) )
 *       list_skills(ch,vict);
 * if ( (*arg) && (GET_LEVEL(ch) > LVL_IMMORT) && !(vict = get_char_vis(ch,arg)) )
 *       send_to_char("No such person around.\r\n", ch);
 * if ( !*arg )
 *    list_skills(ch,ch);
 * }
 */
}



ACMD(do_visible)
{
  if (GET_LEVEL(ch) >= LVL_IMMORT) {
    perform_immort_vis(ch);
    return;
  }

  if (IS_AFFECTED(ch, AFF_INVISIBLE) || IS_AFFECTED(ch, AFF_CAMOUFLAGED)) {
    appear(ch);
    send_to_char("You break the spell of invisibility.\r\n", ch);
  } else     
    send_to_char("You are already visible.\r\n", ch);
}

ACMD(do_contactinfo)
{
 struct char_data *vict = NULL;
 char arg1[MAX_INPUT_LENGTH+1];

 one_argument(argument, arg1);

 if (!*arg1)
 {
  if (GET_CONTACTINFO(ch))
   send_to_char(GET_CONTACTINFO(ch), ch);
  else 
   send_to_char("No contact information set.\r\n", ch);
  return;
 }

 if (is_abbrev(arg1, "write"))
 {
  send_to_char("Normal editing options apply. Save your character afterwards.\r\n", ch);
  send_to_char("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n", ch);
  ch->desc->str = &GET_CONTACTINFO(ch);
  ch->desc->max_str = MAX_INPUT_LENGTH;
  ch->desc->backstr = str_dup(GET_CONTACTINFO(ch));
  SET_BIT(PLR_FLAGS(ch), PLR_WRITING); 
  return;
 }

 if (!(vict = get_char_vis(ch, arg1))) 
 {
  if (!(vict = xmlload_char(arg1))) 
  {
   send_to_char("No such character.\r\n", ch);
   return;
  }
  if (GET_CONTACTINFO(vict))
   send_to_char(GET_CONTACTINFO(vict), ch);
  else
   send_to_char("No contact information set.\r\n", ch);
  return; 
 } 

 if (GET_CONTACTINFO(vict))
  send_to_char(GET_CONTACTINFO(vict), ch);
 else
  send_to_char("No contact information set.\r\n", ch);
 return;
}

ACMD(do_title)
{
  skip_spaces(&argument);
  delete_doubledollar(argument);

  if (IS_NPC(ch))
    send_to_char("Your title is fine... go away.\r\n", ch);
  else if (PLR_FLAGGED(ch, PLR_NOTITLE))
    send_to_char("You can't title yourself -- you shouldn't have abused it!\r\n", ch);
  else if (strstr(argument, "(") || strstr(argument, ")"))
    send_to_char("Titles can't contain the ( or ) characters.\r\n", ch);
  else if (c_strlen(argument) > MAX_TITLE_LENGTH) {
    sprintf(buf, "Sorry, titles can't be longer than %d characters.\r\n",
        MAX_TITLE_LENGTH);
    send_to_char(buf, ch);
  } else {
    set_title(ch, argument);
    sprintf(buf, "Okay, you're now %s %s.\r\n", GET_NAME(ch), GET_TITLE(ch));
    send_to_char(buf, ch);
  }
}


int perform_group(struct char_data *ch, struct char_data *vict)
{
  if (IS_AFFECTED(vict, AFF_GROUP) || !CAN_SEE(ch, vict) || IS_AFFECTED(vict, AFF_TAILING))
    return 0;

  SET_BIT(AFF_FLAGS(vict), AFF_GROUP);
  if (ch != vict)
    act("$N is now a member of your group.", FALSE, ch, 0, vict, TO_CHAR);
  act("You are now a member of $n's group.", FALSE, ch, 0, vict, TO_VICT);
  act("$N is now a member of $n's group.", FALSE, ch, 0, vict, TO_NOTVICT);
  return 1;
}


void print_group(struct char_data *ch)
{
  struct char_data *k;
  struct follow_type *f;

  if (!IS_AFFECTED(ch, AFF_GROUP))
    send_to_char("But you are not the member of a group!\r\n", ch);
  else {
    send_to_char("Your group consists of:\r\n", ch);

    k = (ch->master ? ch->master : ch);

    if (IS_AFFECTED(k, AFF_GROUP)) {
      sprintf(buf, "     [%5dH %5ldM %5dV] [%3ld %s] $N (Head of group)",
          GET_HIT(k), GET_MANA(k), GET_MOVE(k), GET_LEVEL(k), CLASS_ABBR(k));
      act(buf, FALSE, ch, 0, k, TO_CHAR);
    }

    for (f = k->followers; f; f = f->next) {
      if (!IS_AFFECTED(f->follower, AFF_GROUP))
    continue;

      sprintf(buf, "     [%5dH %5ldM %5dV] [%3ld %s] $N", GET_HIT(f->follower),
          GET_MANA(f->follower), GET_MOVE(f->follower),
          GET_LEVEL(f->follower), CLASS_ABBR(f->follower));
      act(buf, FALSE, ch, 0, f->follower, TO_CHAR);
    }
  }
}



ACMD(do_group)
{
  struct char_data *vict;
  struct follow_type *f;
  int found;

  one_argument(argument, buf);

  if (!*buf) {
    print_group(ch);
    return;
  }

  if (ch->master) {
    act("You can not enroll group members without being head of a group.",
    FALSE, ch, 0, 0, TO_CHAR);
    return;
  }

  if (!str_cmp(buf, "all")) {
    perform_group(ch, ch);
    for (found = 0, f = ch->followers; f; f = f->next)
      found += perform_group(ch, f->follower);
    if (!found)
      send_to_char("Everyone following you is already in your group.\r\n", ch);
    return;
  }

  if (!(vict = get_char_room_vis(ch, buf)))
    send_to_char(NOPERSON, ch);
  else if ((vict->master != ch) && (vict != ch))
    act("$N must follow you to enter your group.", FALSE, ch, 0, vict, TO_CHAR);
  else {
    if (!IS_AFFECTED(vict, AFF_GROUP))
    {
      if (IS_NPC(vict))
        send_to_char("You can't group up with mobs.\r\n", ch);
      else
        perform_group(ch, vict);
    }
    else {
      if (ch != vict)
    act("$N is no longer a member of your group.", FALSE, ch, 0, vict, TO_CHAR);
      act("You have been kicked out of $n's group!", FALSE, ch, 0, vict, TO_VICT);
      act("$N has been kicked out of $n's group!", FALSE, ch, 0, vict, TO_NOTVICT);
      REMOVE_BIT(AFF_FLAGS(vict), AFF_GROUP);
    }
  }
}

ACMD(do_ungroup)
{
  struct follow_type *f, *next_fol;
  struct char_data *tch;

  one_argument(argument, buf);

  if (!*buf)
  {
    if (ch->master || !(IS_AFFECTED(ch, AFF_GROUP))) {
      send_to_char("But you lead no group!\r\n", ch);
      return;
   }
   sprintf(buf2, "%s has disbanded the group.\r\n", GET_NAME(ch));
   for (f = ch->followers; f; f = next_fol)
   {
     next_fol = f->next;
     if (IS_AFFECTED(f->follower, AFF_GROUP))
     {
      REMOVE_BIT(AFF_FLAGS(f->follower), AFF_GROUP);
      send_to_char(buf2, f->follower);
      if (IS_AFFECTED(f->follower,AFF_MORALE))
        affect_from_char(f->follower,SKILL_MORALE);
      if (!IS_AFFECTED(f->follower, AFF_CHARM))
        stop_follower(f->follower);
      }
    }
    if (IS_AFFECTED(ch,AFF_MORALE))
      affect_from_char(ch,SKILL_MORALE);

    REMOVE_BIT(AFF_FLAGS(ch), AFF_GROUP);
    send_to_char("You disband the group.\r\n", ch);
    return;
  }
  if (!(tch = get_char_room_vis(ch, buf))) {
    send_to_char("There is no such person!\r\n", ch);
    return;
  }
  if (tch->master != ch) {
    send_to_char("That person is not following you!\r\n", ch);
    return;
  }

  if (!IS_AFFECTED(tch, AFF_GROUP)) {
    send_to_char("That person isn't in your group.\r\n", ch);
    return;
  }

  REMOVE_BIT(AFF_FLAGS(tch), AFF_GROUP);

  act("$N is no longer a member of your group.", FALSE, ch, 0, tch, TO_CHAR);
  act("You have been kicked out of $n's group!", FALSE, ch, 0, tch, TO_VICT);
  act("$N has been kicked out of $n's group!", FALSE, ch, 0, tch, TO_NOTVICT);
 
  if (IS_AFFECTED(tch,AFF_MORALE))
    affect_from_char(tch,SKILL_MORALE);
  if (!IS_AFFECTED(tch, AFF_CHARM))
    stop_follower(tch);
}




ACMD(do_report)
{
  struct char_data *k;
  struct follow_type *f;

  if (!IS_AFFECTED(ch, AFF_GROUP)) {
    send_to_char("But you are not a member of any group!\r\n", ch);
    return;
  }
  sprintf(buf, "%s reports: %d/%dH, %ld/%ldM, %d/%dV\r\n",
      GET_NAME(ch), GET_HIT(ch), GET_MAX_HIT(ch),
      GET_MANA(ch), GET_MAX_MANA(ch),
      GET_MOVE(ch), GET_MAX_MOVE(ch));

  CAP(buf);

  k = (ch->master ? ch->master : ch);

  for (f = k->followers; f; f = f->next)
    if (IS_AFFECTED(f->follower, AFF_GROUP) && f->follower != ch)
      send_to_char(buf, f->follower);
  if (k != ch)
    send_to_char(buf, k);
  send_to_char("You report to the group.\r\n", ch);
}

ACMD(do_oreport)
{

  sprintf(buf, "%s reports: %d/%dH, %ld/%ldM, %d/%dV.\r\n",
          GET_NAME(ch), GET_HIT(ch), GET_MAX_HIT(ch),
          GET_MANA(ch), GET_MAX_MANA(ch),
          GET_MOVE(ch), GET_MAX_MOVE(ch));
  
  act(buf, FALSE, ch, 0, 0, TO_ROOM);
  

}


ACMD(do_split)
{
  int amount, num, share;
  struct char_data *k;
  struct follow_type *f;

  if (IS_NPC(ch))
    return;

  one_argument(argument, buf);

  if (is_number(buf)) {
    amount = atoi(buf);
    if (amount <= 0) {
      send_to_char("Sorry, you can't do that.\r\n", ch);
      return;
    }
    if (amount > GET_GOLD(ch)) {
      send_to_char("You don't seem to have that much gold to split.\r\n", ch);
      return;
    }
    k = (ch->master ? ch->master : ch);

    if (IS_AFFECTED(k, AFF_GROUP) && (k->in_room == ch->in_room))
      num = 1;
    else
      num = 0;

    for (f = k->followers; f; f = f->next)
      if (IS_AFFECTED(f->follower, AFF_GROUP) &&
      (!IS_NPC(f->follower)) &&
      (f->follower->in_room == ch->in_room))
    num++;

    if (num && IS_AFFECTED(ch, AFF_GROUP))
      share = amount / num;
    else {
      send_to_char("With whom do you wish to share your gold?\r\n", ch);
      return;
    }

    GET_GOLD(ch) -= share * (num - 1);

    if (IS_AFFECTED(k, AFF_GROUP) && (k->in_room == ch->in_room)
    && !(IS_NPC(k)) && k != ch) {
      GET_GOLD(k) += share;
      sprintf(buf, "%s splits %d coins; you receive %d.\r\n", GET_NAME(ch),
          amount, share);
      send_to_char(buf, k);
    }
    for (f = k->followers; f; f = f->next) {
      if (IS_AFFECTED(f->follower, AFF_GROUP) &&
      (!IS_NPC(f->follower)) &&
      (f->follower->in_room == ch->in_room) &&
      f->follower != ch) {
    GET_GOLD(f->follower) += share;
    sprintf(buf, "%s splits %d coins; you receive %d.\r\n", GET_NAME(ch),
        amount, share);
    send_to_char(buf, f->follower);
      }
    }
    sprintf(buf, "You split %d coins among %d members -- %d coins each.\r\n",
        amount, num, share);
    send_to_char(buf, ch);
  } else {
    send_to_char("How many coins do you wish to split with your group?\r\n", ch);
    return;
  }
}



/*
 * Since ter'angreals must be HELD, no need to name the ter'angie 
 * to use it, no?
 *
 * So, find the eq on pos=HELD, if ter'angreal, find it's spell. 
 * According to spell_info, see if we need a target (i.e an argument), 
 * the whole argument, is the arguments to the 'spell'. 
 *
 * GET_OBJ_VAL(obj,0) = Spell. 
 * GET_OBJ_VAL(obj,1) = Lvl of spell-caster. 
 * GET_OBJ_VAL(obj,2) = Mana-add from ter'angreal. 
 */
ACMD(do_use)
{
  struct obj_data *ter = NULL;
  int spelln,lvl_cast,mana_cost,orig_lvl,orig_mana,madd,orig_mmana,orig_skill;
  int cc,isg,i;
  char buf[MAX_INPUT_LENGTH];
  char orig_flow[5],*fp = &(GET_AIR(ch));

  skip_spaces(&argument);

  if (NULL == (ter = GET_EQ(ch,WEAR_HOLD))) {
     send_to_char("Use what? You're not holding anything!\r\n",ch);
     return;
  }
  if (!IS_SET(GET_OBJ_EXTRA(ter),ITEM_TERANGREAL)) {
    asend_to_char(ch, "Hey buddy, %s is not a flaming Ter'angreal!\r\n",ter->short_description);
    return;
  }

  if ((IS_SET(GET_OBJ_EXTRA(ter),ITEM_NOMALE) && (SEX_MALE == GET_SEX(ch))) || (IS_SET(GET_OBJ_EXTRA(ter),ITEM_NOFEMALE) && (SEX_FEMALE == GET_SEX(ch)))) {
    asend_to_char(ch,"Only %s may use this Ter'angreal!\r\n",(SEX_MALE == GET_SEX(ch) ? "females" : "males"));
    return;
  }

  spelln = GET_OBJ_VAL(ter,0);
  if ((spelln < 1) || (spelln > MAX_SPELLS) || (NULL == spell_list[spelln])) {
    send_to_char("Something's fishy with this ter'angreal. Please inform an Imm\r\n",ch);
    return;
  }
  lvl_cast = GET_OBJ_VAL(ter,1);
  lvl_cast = (lvl_cast <= 0   ? 1   : lvl_cast);
  lvl_cast = (lvl_cast >= 200 ? 199 : lvl_cast);

  madd = GET_OBJ_VAL(ter,2);  
  madd = (madd < 0     ? 0     : madd);
  madd = (madd > 12000 ? 12000 : madd);
  mana_cost = spell_info[spelln].mana + madd;

  /* 
   * Set values on char, so that char can cast spell
   */
  cc  = !(cant_channel(ch));  
  isg = IS_SET(PRF_FLAGS(ch),PRF_GRASPING);
  if (!cc) {
    GET_SPARK(ch) = 1;
  }
  if (!isg) {
    SET_BIT(PRF_FLAGS(ch),PRF_GRASPING);
  }
  orig_lvl   = GET_LEVEL(ch);
  orig_mana  = GET_MANA(ch);
  orig_mmana = GET_MAX_MANA(ch);
  orig_skill = GET_SKILL(ch,spelln);
  GET_SKILL(ch,spelln) = 100;
  GET_LEVEL(ch) = lvl_cast;
  GET_MANA(ch) = GET_MAX_MANA(ch) = mana_cost;
  for (i = 0; i < 5; i++) {
    orig_flow[i] = fp[i];
    fp[i] = spell_info[spelln].min_level[CLASS_WILDER][i];
  }
  /*
   * Do the casting, always tied, if nescessary 
   */
  sprintf(buf,"garbage '%s'",spells[spelln]);
  if (*argument)
    sprintf(buf,"%s %s",buf,argument);
  if (madd)
    sprintf(buf,"%s %d",buf,madd);

  do_cast(ch,buf,0,1);

  /*
   * Restore original values!
   */
  GET_SKILL(ch,spelln) = orig_skill;
  GET_MANA(ch) = orig_mana;
  GET_MAX_MANA(ch) = orig_mmana;
  GET_LEVEL(ch) = orig_lvl;
  if (!cc) {
    GET_SPARK(ch) = 0;
  }
  if (!isg) {
    REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
  }
  for (i = 0; i < 5; i++) {
    fp[i] = orig_flow[i];
  }
}

void do_perc_wimpy(struct char_data *ch,char *arg)
{
  int wimp_lev;

  if (!*arg) {
    if (GET_WIMP_LEV(ch)) {
      sprintf(buf,"Your current wimp level is %ld%% = %d hit points.\r\n",GET_WIMP_LEV(ch),
     (int) ((GET_WIMP_LEV(ch)/100.0)*(float)GET_MAX_HIT(ch)));
      send_to_char(buf,ch);
      return;
    }
    else {
      send_to_char("At the moment, you're not a wimp.  (sure, sure...)\r\n",ch);
      return;
    }
  }
  if (isdigit(*arg)) {
    if ((wimp_lev = atoi(arg))) {
      if (wimp_lev < 0)
        send_to_char("Feeling funny today?\r\n",ch);
      else if (wimp_lev > 50)
        send_to_char("You can't set your wimp level to max 50%%!\r\n",ch);
      else {
        sprintf(buf,"Okay, you will wimp if you drop below %d%% = %d hit points.\r\n",wimp_lev,
           (int)((float)GET_MAX_HIT(ch)*(wimp_lev/100.0)));
        send_to_char(buf,ch);
        GET_WIMP_LEV(ch) = wimp_lev;
      }
    }
    else {
      send_to_char("Okay, now you'll fight to the bitter end.\r\n",ch);
      GET_WIMP_LEV(ch) = 0;
    }
  }
  else 
    send_to_char("Specify at what percantage of your hitpoints you wish to wimp out at.\r\n",ch);
  return;
}

ACMD(do_wimpy)
{
  int wimp_lev;

  one_argument(argument, arg);

  if (PRF_FLAGGED(ch,PRF_PERCWIMPY))
    do_perc_wimpy(ch,arg);
  else {
    if (!*arg) {
      if (GET_WIMP_LEV(ch)) {
        sprintf(buf, "Your current wimp level is %ld hit points.\r\n",
          GET_WIMP_LEV(ch));
        send_to_char(buf, ch);
        return;
      } else {
        send_to_char("At the moment, you're not a wimp.  (sure, sure...)\r\n", ch);
        return;
      }
    }
    if (isdigit(*arg)) {
      if ((wimp_lev = atoi(arg))) {
        if (wimp_lev < 0)
      send_to_char("Heh, heh, heh.. we are jolly funny today, eh?\r\n", ch);
        else if (wimp_lev > GET_MAX_HIT(ch))
      send_to_char("That doesn't make much sense, now does it?\r\n", ch);
        else if (wimp_lev > (GET_MAX_HIT(ch) >> 1))
      send_to_char("You can't set your wimp level above half your hit points.\r\n", ch);
        else {
      sprintf(buf, "Okay, you'll wimp out if you drop below %d hit points.\r\n",
        wimp_lev);
      send_to_char(buf, ch);
      GET_WIMP_LEV(ch) = wimp_lev;
        }
      } else {
        send_to_char("Okay, you'll now tough out fights to the bitter end.\r\n", ch);
        GET_WIMP_LEV(ch) = 0;
      }
    } else
      send_to_char("Specify at how many hit points you want to wimp out at.  (0 to disable)\r\n", ch);
  }
  return;
}

static void strip_prompt(struct char_data *ch,char *p)
{
  while (*p) {
    if ('%' == *p) {
      p++;
      switch(*p) {
      case 'h':
      case 'H':
      case 'm':
      case 'M':
      case 'v':
      case 'V':
      case 'g':
      case 'G':
      case 'E':
      case 'N':
      case 'l':
      case 'i':
      case 'I':
      case 'n':
      case 'D':
      case 'c':
      case ' ': // alow single % if, and only if there's a space or \0 following. 
      case '\0':
    break;
      default:
    asend_to_char(ch,"%%%c is not a valid prompt sequence...\r\n",*p);
    *(p-1) = *p = ' ';
    break;
      }
    }
    p++;
  }
}

ACMD(do_prompt)
{
  if (IS_NPC(ch)) {
    send_to_char("Monsters don't need displays.  Go away.\r\n", ch);
    return;
  }
  skip_spaces(&argument);

  if (!*argument) {
    send_to_char("Usage: prompt <promptstring>|off\r\nwhere <promptstring> is "
         "the prompt you want.\r\n\r\n",ch);
    send_to_char("<promptstring> can contain these special codes:\r\n\r\n",ch);
    asend_to_char(ch,"%ch: display current hitpoints.\r\n",'%');
    asend_to_char(ch,"%cH: display maxmimum hitpoints.\r\n",'%');
    asend_to_char(ch,"%cm: display current mana.\r\n",'%');
    asend_to_char(ch,"%cM: display maximum mana.\r\n",'%');
    asend_to_char(ch,"%cv: display current moves.\r\n",'%');
    asend_to_char(ch,"%cV: display maximum moves.\r\n",'%');
    asend_to_char(ch,"%cE: display exp to next level.\r\n",'%');
    asend_to_char(ch,"%cg: gold carried.\r\n",'%');
    asend_to_char(ch,"%cG: gold in bank.\r\n",'%');
    asend_to_char(ch,"%cN: player name.\r\n",'%');
    asend_to_char(ch,"%cl: player level.\r\n",'%');
    asend_to_char(ch,"%ci: IC/OOC prompt > (Red for OOC, White for IC)\r\n",'%');
    asend_to_char(ch,"%cI: IC or OOC depending on IC status.\r\n",'%');
    asend_to_char(ch,"%cn: New line.\r\n",'%');
    asend_to_char(ch,"%cD: Server Date and Time.\r\n",'%');
    asend_to_char(ch,"%cc: Use in place of a color code for your MYCOLOR color.\r\n",'%');
    return;
  }

  if (!strcasecmp(argument,"off")) {
    REMOVE_BIT(PLR_FLAGS(ch),PLR_CPROMPT);
    if (GET_CPROMPT(ch)) {
      free(GET_CPROMPT(ch));
      GET_CPROMPT(ch) = NULL;
    }
    send_to_char("Custom prompt removed, old prompt settings now used.\r\n",ch);
    return;
  }

  if (strlen(argument) > 200) {
    send_to_char("Prompt is too large!\r\n",ch);
    return;
  }

  strip_prompt(ch,argument);

  if (GET_CPROMPT(ch)) {   /* Free evntual old prompt... */
    free(GET_CPROMPT(ch));
  }
  GET_CPROMPT(ch) = strdup(argument);

  SET_BIT(PLR_FLAGS(ch),PLR_CPROMPT);
  send_to_char(OK, ch);
}


ACMD(do_display)
{
  size_t i;

  if (IS_NPC(ch)) {
    send_to_char("Monsters don't need displays.  Go away.\r\n", ch);
    return;
  }
  skip_spaces(&argument);

  if (!*argument) {
    send_to_char("Usage: display { H | M | V | G | X | all | none }\r\n", ch);
    return;
  }
  if ((!str_cmp(argument, "on")) || (!str_cmp(argument, "all")))
    SET_BIT(PRF_FLAGS(ch), PRF_DISPHP | PRF_DISPMANA | PRF_DISPMOVE | PRF_DISPEXP | PRF_DISPGOLD);
  else {
    REMOVE_BIT(PRF_FLAGS(ch), PRF_DISPHP | PRF_DISPMANA | PRF_DISPMOVE | PRF_DISPEXP | PRF_DISPGOLD);

    for (i = 0; i < strlen(argument); i++) {
      switch (LOWER(argument[i])) {
      case 'h':
    SET_BIT(PRF_FLAGS(ch), PRF_DISPHP);
    break;
      case 'm':
    SET_BIT(PRF_FLAGS(ch), PRF_DISPMANA);
    break;
      case 'v':
    SET_BIT(PRF_FLAGS(ch), PRF_DISPMOVE);
    break;
      case 'g':
        SET_BIT(PRF_FLAGS(ch), PRF_DISPGOLD);
        break;
      case 'x':
        SET_BIT(PRF_FLAGS(ch), PRF_DISPEXP);
        break;        
      }
    }
  }

  send_to_char(OK, ch);
  if (GET_CPROMPT(ch)) {
    free(GET_CPROMPT(ch));
    GET_CPROMPT(ch) = NULL;
  }
  REMOVE_BIT(PLR_FLAGS(ch),PLR_CPROMPT);
}



ACMD(do_gen_write)
{
  FILE *fl;
  char *tmp, *filename, buf[MAX_STRING_LENGTH];
  struct stat fbuf;
  time_t ct;

  switch (subcmd) {
  case SCMD_BUG:
    filename = BUG_FILE;
    break;
  case SCMD_TYPO:
    filename = TYPO_FILE;
    break;
  case SCMD_IDEA:
    filename = NEWIDEA_FILE;
    break;
  case SCMD_SOCIALIDEA:
    filename = SOCIDEA_FILE;
    break;
  case SCMD_TODO:
    filename = TODO_FILE;
    break;
  case SCMD_CRETODO:
    filename = CRETODO_FILE;
    break;
  default:
    return;
  }

  ct = time(0);
  tmp = asctime(localtime(&ct));

  if (IS_NPC(ch)) {
    send_to_char("Monsters can't have ideas - Go away.\r\n", ch);
    return;
  }

  skip_spaces(&argument);
  delete_doubledollar(argument);

  if (!*argument) {
    send_to_char("That must be a mistake...\r\n", ch);
    return;
  }
  sprintf(buf, "(PLAYERREPORTS) %s (in Room# %5d) %s: %s", GET_NAME(ch),world[ch->in_room].number, CMD_NAME, argument);
  if (subcmd == SCMD_TODO || subcmd == SCMD_CRETODO)
    xlog(SYS_PREPORT, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), buf, FALSE);
  else
    xlog(SYS_PREPORT, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), buf, FALSE);

  if (stat(filename, &fbuf) < 0) {
    mlog("SYSERR: Error statting file");
    return;
  }
  if (fbuf.st_size >= max_filesize) {
    send_to_char("Sorry, the file is full right now.. try again later.\r\n", ch);
    return;
  }
  if (!(fl = fopen(filename, "a"))) {
    mlog("SYSERR: do_gen_write couldn't open file.");
    send_to_char("Could not open the file.  Sorry.\r\n", ch);
    return;
  }
  fprintf(fl, "%-8s (%6.6s) [%5d] %s\n", GET_NAME(ch), (tmp + 4),
      world[ch->in_room].number, argument);
  fclose(fl);
  send_to_char("Okay.  Thanks!\r\n", ch);
  
  switch (subcmd) {
  case SCMD_BUG:
    file_to_string_alloc(BUG_FILE, &bugs);
    break;
  case SCMD_TYPO:
    file_to_string_alloc(TYPO_FILE, &typos);
    break;
  case SCMD_IDEA:
    file_to_string_alloc(NEWIDEA_FILE, &newideas);
    break;
  case SCMD_SOCIALIDEA:
    file_to_string_alloc(SOCIDEA_FILE, &socialideas);
    break;
  case SCMD_TODO:
    file_to_string_alloc(TODO_FILE, &todo);
    break;
  case SCMD_CRETODO:
    file_to_string_alloc(CRETODO_FILE, &cretodo);
    break;
  default:
    return;
  }

  
}




#define TOG_OFF 0
#define TOG_ON  1



ACMD(do_gen_tog)
{
  long result = 0;

  char *tog_messages[][2] = {
    {"You will now see the messages in fights.\r\n",	//0
    "Nospam enabled. Fight messages suppressed.\r\n"},
    {"Nohassle disabled.\r\n",				//1
    "Nohassle enabled.\r\n"},
    {"Brief mode off.\r\n",				//2
    "Brief mode on.\r\n"},
    {"Compact mode off.\r\n",				//3
    "Compact mode on.\r\n"},
    {"You can now hear tells.\r\n",			//4
    "You are now deaf to tells.\r\n"},
    {"You can now hear auctions.\r\n",			//5
    "You are now deaf to auctions.\r\n"},
    {"You can now hear shouts.\r\n",
    "You are now deaf to shouts.\r\n"},
    {"You can now hear Questions.\r\n",
    "You are now deaf to Questions.\r\n"},
    {"You can now hear OOC.\r\n",
    "You are now deaf to OOC.\r\n"},
    {"You can now hear the Wiz-channel.\r\n",
    "You are now deaf to the Wiz-channel.\r\n"},
    {"You are no longer part of the Quest.\r\n",	//10
    "Okay, you are part of the Quest!\r\n"},
    {"You will no longer see the room flags.\r\n",
    "You will now see the room flags.\r\n"},
    {"You will now have your communication repeated.\r\n",
    "You will no longer have your communication repeated.\r\n"},
    {"HolyLight mode off.\r\n",
    "HolyLight mode on.\r\n"},
    {"Nameserver_is_slow changed to NO; IP addresses will now be resolved.\r\n",
    "Nameserver_is_slow changed to YES; sitenames will no longer be resolved.\r\n"},
    {"Autoexits disabled.\r\n",				//15
    "Autoexits enabled.\r\n"},
    {"You can now hear your fellow guildmembers.\r\n",
     "You are now deaf to the guildchannel\r\n"},
    {"Autogold disabled.\r\n",
     "Autogold enabled.\r\n"},
    {"Autoloot disabled.\r\n",
     "Autoloot enabled.\r\n"},
    {"Autojunk disabled.\r\n",
     "Autojunk enabled.\r\n"},
    {"Autoassist disabled.\r\n",			//20
     "Autoassist enabled.\r\n"},
    {"Autosplit disabled.\r\n",
     "Autosplit enabled.\r\n"},
    {"You will now hear wolftalk again.\r\n",
     "You will no longer hear wolftalk.\r\n"},
    {"You will no longer look ahead.\r\n",
     "You will now look ahead when moving.\r\n"},
    {"You will no longer hear Comm Spam.\r\n",
     "You will now hear Comm Spam.\r\n"},
    {"You will no longer user percentual wimp level.\r\n",  //25
     "You will now use percentual wimp level.\r\n"},
    {"Sliceweave disabled.\r\n",
     "Sliceweave enabled.\r\n"},
    {"Hunger messages will be shown again.\r\n",
     "Hunger messages will no longer be shown.\r\n"},
    {"Death cries will be shown again.\r\n",
     "Death cries will no longer be shown.\r\n"},
    {"You tune back into the Darkfriend network.\r\n",
     "You tune out the Darkfriend network.\r\n"},
    {"You now hear your fellow Alantins again.\r\n",	    //30
     "You no longer hear your brethren Ogiers.\r\n"},
    {"You will now hear those filthy people on obscene!\r\n",
     "You will no longer hear that foul language on obscene!\r\n"},
    {"You will now see pose indentification.\r\n",
     "You will no longer see pose indentification.\r\n"},
    {"You will now hear the sing channel!\r\n",
     "You will no longer hear the sing channel!\r\n"}, 
    {"You are now Out of Character.\r\n",
     "You are now In Character.\r\n"},
    {"You will now see weather messages.\r\n",		    //35
     "You will no longer see weather messages.\r\n"},
    { "You will now use the new style score sheet.\r\n",
      "You will now use the old style score sheet.\r\n"},
    { "You can now be added to Private Channels.\r\n",
      "You can no longer be added to Private Channels.\r\n"},
    { "You will no longer see the RP Tag.\r\n",
      "You will now see the RP Tag.\r\n"},
    { "You will no longer see XLOG tags.\r\n",
      "You will now see XLOG tags.\r\n"},
    { "You will no longer see timestamps.\r\n",
      "You will now see timestamps where available.\r\n"},
 };


  if (IS_NPC(ch))
    return;

  switch (subcmd) {
  case SCMD_NOSPAM:
    result = PRF_TOG_CHK(ch, PRF_NOSPAM);
    break;
  case SCMD_NOHASSLE: /* Not on toggle, Imm setting */
    if (GET_LEVEL(ch) < LVL_IMMORT)
    {
     send_to_char("Could not toggle nohassle.\r\n", ch);
     return;
    }
    result = PRF_TOG_CHK(ch, PRF_NOHASSLE);
    break;
  case SCMD_BRIEF:
    result = PRF_TOG_CHK(ch, PRF_BRIEF);
    break;
  case SCMD_COMPACT:
    result = PRF_TOG_CHK(ch, PRF_COMPACT);
    break;
  case SCMD_NOTELL:
    result = PRF_TOG_CHK(ch, PRF_NOTELL);
    break;
  case SCMD_NOAUCTION:
    result = PRF_TOG_CHK(ch, PRF_NOAUCT);
    break;
  case SCMD_DEAF:
    result = PRF_TOG_CHK(ch, PRF_DEAF);
    break;
  case SCMD_NOQUESTION:
    result = PRF2_TOG_CHK(ch, PRF2_NOQUEST);
    break;
  case SCMD_NOOOC:
    result = PRF_TOG_CHK(ch, PRF_NOOOC);
    break;
  case SCMD_NOWIZ: /* Not on toggle, Imm setting */
    if (GET_LEVEL(ch) < LVL_IMMORT)
    {
     send_to_char("Could not toggle wiznet.\r\n", ch);
     return;
    }
    result = PRF_TOG_CHK(ch, PRF_NOWIZ);
    break;
  case SCMD_QUEST:
    if((!questlock && !PRF_FLAGGED(ch, PRF_QUEST)) || PRF_FLAGGED(ch, PRF_QUEST))
       result = PRF_TOG_CHK(ch, PRF_QUEST);
    else
    if(questlock && !PRF_FLAGGED(ch, PRF_QUEST))
       {
       send_to_char("\\c01The Quest is locked at the moment! Please wait for a new one.\\c00\r\n",ch);
       return;
       }
    break;
  case SCMD_ROOMFLAGS: /* Not on toggle, Imm setting */
    if (GET_LEVEL(ch) < LVL_IMMORT)
    {
     send_to_char("Could not toggle roomflags.\r\n", ch);
     return;
    }
    result = PRF_TOG_CHK(ch, PRF_ROOMFLAGS);
    break;
  case SCMD_NOREPEAT:
    result = PRF_TOG_CHK(ch, PRF_NOREPEAT);
    break;
  case SCMD_HOLYLIGHT: /* Not on toggle, Imm setting */
    if (GET_LEVEL(ch) < LVL_IMMORT)
    {
     send_to_char("Could not toggle holylight.\r\n", ch);
     return;
    }
    result = PRF_TOG_CHK(ch, PRF_HOLYLIGHT);
    break;
  case SCMD_SLOWNS: /* Not on toggle, Imm setting */
    result = (nameserver_is_slow = !nameserver_is_slow);
    break;
  case SCMD_AUTOEXIT: 
    result = PRF_TOG_CHK(ch, PRF_AUTOEXIT);
    break;
  case SCMD_NOGDT:
    result = PRF_TOG_CHK(ch, PRF_NOGDT);
    break;
  case SCMD_AUTOJUNK: 
    result = AUT_TOG_CHK(ch,AUTO_JUNK);
    break;
  case SCMD_AUTOLOOT:
    result = AUT_TOG_CHK(ch,AUTO_LOOT);
    break;
  case SCMD_AUTOASSIST:
    result = AUT_TOG_CHK(ch,AUTO_ASSIST);
    break;
  case SCMD_AUTOSPLIT:
    result = AUT_TOG_CHK(ch,AUTO_SPLIT);
    break;
  case SCMD_AUTOGOLD:
    result = AUT_TOG_CHK(ch,AUTO_GOLD);
    break;
  case SCMD_NOWT:
     result = PRF_TOG_CHK(ch,PRF_NOWT);
     break;
  case SCMD_LOOKAHEAD:
     result = PRF_TOG_CHK(ch,PRF_LOOKAHEAD);
     break;
  case SCMD_PWIMPY:
     result = PRF_TOG_CHK(ch,PRF_PERCWIMPY);
     break;
  case SCMD_SLICEWEAVE:
     if (!IS_SET(PRF2_FLAGS(ch), PRF2_SLICEWEAVE) && 
     (IS_SET(GET_PERMANENT(ch), PRM_GENTLED) || !GET_SKILL(ch, SKILL_SLICEWEAVE))) {
       send_to_char("You don't even know how!\r\n", ch);
       return;
     }
     result = PRF2_TOG_CHK(ch, PRF2_SLICEWEAVE);
     break;
  case SCMD_NOHUNGER:
    result = PRF2_TOG_CHK(ch, PRF2_NOHUNGER);
    break;
  case SCMD_COMMSPAM:
    result = PRF_TOG_CHK(ch, PRF_COMMSPAM );
    break;
  case SCMD_NOKILLMESS:
    result = PRF2_TOG_CHK(ch,PRF2_NOKILLMESS);
    break;
  case SCMD_TIMESTAMP:
    result = PRF2_TOG_CHK(ch,PRF2_TIMESTAMP);
    break;
  case SCMD_NOOBSCENE:
    result = PRF2_TOG_CHK(ch,PRF2_NOOBSCENE);
    break;
  case SCMD_NOPOSEID:
    result = PRF2_TOG_CHK(ch, PRF2_NOPOSEID);
    break;
  case SCMD_NOSONG:
    result = PRF2_TOG_CHK(ch, PRF2_NOSONG);
    break;
  case SCMD_IC:
    GET_LAST_EMOTE(ch) = 0;
    result = PRF_TOG_CHK(ch, PRF_IC);
    break;
  case SCMD_NOWEATHER:
    result = PRF2_TOG_CHK(ch, PRF2_NOWEATHER);
    break;
  case SCMD_NOPADD:
    result = PRF2_TOG_CHK(ch, PRF2_NOPADD);
    break;
  case SCMD_RPTAG:
    result = PRF2_TOG_CHK(ch, PRF2_RPTAG);
    break;
  case SCMD_XLOGTAG:
    if (GET_LEVEL(ch) < LVL_IMMORT)
    {
     send_to_char("Could not toggle xlogtag.\r\n", ch);
     return;
    }
    result = PRF2_TOG_CHK(ch, PRF2_XLOGTAG);
    break;
  default:
    mlog("SYSERR: Unknown subcmd in do_gen_toggle");
    return;
    break;
  }

  if (result)
    send_to_char(tog_messages[subcmd][TOG_ON], ch);
  else
    send_to_char(tog_messages[subcmd][TOG_OFF], ch);

  return;
}

ACMD(do_afk)
{

  if (IS_SET(PRF_FLAGS(ch),PRF_AFK)) {
    send_to_char("You're already AFK, bonehead!\r\n",ch);
    return;
  }

  if (IS_SET(PRF2_FLAGS(ch),PRF2_AFW)) {
    send_to_char("You're already AFW, woolhead!\r\n",ch);
    return;
  }


  if (!*argument) {
    GET_AFK(ch) = std_Flag;
   }
  else {
    skip_spaces(&argument);
    CREATE( GET_AFK( ch ), char, strlen( argument ) + 10 );
    strcpy(GET_AFK(ch),argument);
    strcat(GET_AFK(ch),"\r\n");
  } 

  SET_BIT(PRF_FLAGS(ch),PRF_AFK);
  send_to_char("Okey-dokey, you're now AFK.\r\n",ch);
}

ACMD(do_noafk)
{
  if (!IS_SET(PRF_FLAGS(ch),PRF_AFK)) {
    send_to_char("C'mon, you're not AFK!\r\n",ch);
    return;
  } 

  if (GET_AFK(ch) != std_Flag) {
    free(GET_AFK(ch));
  }

  GET_AFK(ch) = NULL;

  REMOVE_BIT(PRF_FLAGS(ch),PRF_AFK);
  send_to_char("You're no longer AFK. Enjoy!\r\n",ch);
} 

ACMD(do_afw)
{

  if (IS_SET(PRF2_FLAGS(ch),PRF2_AFW)) {
    send_to_char("You're already AFW, woolhead!\r\n",ch);
    return;
  }

  if (IS_SET(PRF_FLAGS(ch),PRF_AFK)) {
    send_to_char("You're already AFK, bonehead!\r\n",ch);
    return;
  }


  if (!*argument) {
    GET_AFW(ch) = std_afw;
   }
  else {
    skip_spaces(&argument);
    CREATE( GET_AFW( ch ), char, strlen( argument )+10 );
    strcpy(GET_AFW(ch),argument);
    strcat(GET_AFW(ch),"\r\n");
  } 

  SET_BIT(PRF2_FLAGS(ch),PRF2_AFW);
  send_to_char("Okey-dokey, you're now AFW.\r\n",ch);
}

ACMD(do_noafw)
{
  if (!IS_SET(PRF2_FLAGS(ch),PRF2_AFW)) {
    send_to_char("C'mon, you're not AFW!\r\n",ch);
    return;
  } 

  if (GET_AFW(ch) != std_afw) {
    free(GET_AFW(ch));
  }
  GET_AFW(ch) = NULL;

  REMOVE_BIT(PRF2_FLAGS(ch),PRF2_AFW);
  send_to_char("You're no longer AFW. Enjoy!\r\n",ch);
} 

ACMD(do_award)
{
  struct char_data *victim, *i;
  char arg1[100], *arg2, logstr[100];
  char mesg[100];
  int countpeople=0;

  if (!*argument){
    send_to_char("Syntax: Award Playername Questpoints.\n\r", ch);
    return;
  }
  arg2 = one_argument(argument, arg1);

  if ((!*arg2) || ((!atoi(arg2)) && !strncasecmp(arg1,"all",strlen(arg1)) && !strncasecmp(arg1,"quest",strlen(arg1))))
    {
    send_to_char("How many quest points do you want to award them?\r\n", ch);
    return;
    }

  if ( !strncasecmp(arg1,"all",strlen(arg1)) && GET_LEVEL(ch)<LVL_GROOVYGL)
     send_to_char("I don't think so.\r\n", ch);
  else   
  if (!strncasecmp(arg1,"all",strlen(arg1)) && GET_LEVEL(ch)>=LVL_GROOVYGL)
     {     
     for (i = character_list; i != NULL; i = i->next) {
       if (!IS_NPC(i) && (GET_LEVEL(i) < LVL_IMMORT)) 
          {
          countpeople++;
          GET_QP(i) += atoi(arg2);
          sprintf(buf,"You have been awarded %d questpoints by %s!\r\n", atoi(arg2), GET_NAME(ch));
          send_to_char(buf, i);
          sprintf(buf,"Ok, you give %d quest points to %s.\r\n", atoi(arg2), GET_NAME(i));
          send_to_char(buf, ch);
          sprintf(logstr, "(GC) %s has awarded %s %d Qp.", GET_NAME(ch),
          GET_NAME(i), atoi(arg2));
          xlog(SYS_AWARD, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), logstr, TRUE);
          }
     }
     if (countpeople==0)
        send_to_char("But there are no mortals on.\r\n",ch);
     countpeople=0; /* Just to make sure no screwups. */
     } 

  if (!strncasecmp(arg1,"quest",strlen(arg1)))
     {
     for (i = character_list; i != NULL; i = i->next) {
       if (!IS_NPC(i) && (GET_LEVEL(i) < LVL_IMMORT) && PRF_FLAGGED(i, PRF_QUEST))
          {
          countpeople++;
          GET_QP(i) += atoi(arg2);
          sprintf(buf,"You have been awarded %d questpoints by %s!\r\n", atoi(arg2), GET_NAME(ch));
          send_to_char(buf, i);
          sprintf(buf,"Ok, you give %d quest points to %s.\r\n", atoi(arg2), GET_NAME(i));
          send_to_char(buf, ch);
          sprintf(logstr, "(GC) %s has awarded %s %d Qp.", GET_NAME(ch),
          GET_NAME(i), atoi(arg2));
          xlog(SYS_AWARD, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), logstr, TRUE);
          }
     }
     if (countpeople==0)       
        send_to_char("But there are no mortals Questing!\r\n",ch);  
     countpeople=0; /* Just to make sure no screwups. */
     }


  if (!(victim = get_char_vis(ch, arg1))) {
    if (-1 == find_name(arg1)) {
      send_to_char("Player not found, online or offline.\r\n", ch);
      return;
    }
      if (NULL != (i = xmlload_char(arg1))) {
        GET_QP(i)+=atoi(arg2);
        sprintf(buf, "%s has been awarded %d Quest Points while OFFLINE.\r\n", GET_NAME(i), atoi(arg2));
        send_to_char(buf, ch);
        sprintf(mesg, "&gAutomated System Mudmail&n:\r\n\r\nWhile you were offline you were awarded %d Quest Points.\r\n",  atoi(arg2));
        store_mail(GET_IDNUM(i), GET_IDNUM(ch), mesg);
        sprintf(logstr, "(GC) %s has awarded OFFLINE PLAYER %s %d Qp.", GET_NAME(ch), GET_NAME(i), atoi(arg2));
        xlog(SYS_AWARD, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), logstr, TRUE);
        xmlsave_char(i);
        free_char(i);
        return;
      }
    send_to_char("Sorry, mon, no such player here right now.\r\n", ch);
    return;
  }
  if (IS_NPC(victim) && strncasecmp(arg1,"all",strlen(arg1)) && strncasecmp(arg1,"quest",strlen(arg1))) {
    send_to_char("Hey, not on mobs!\r\n",ch);
    return;
  }
    if ((!*arg2)||(!atoi(arg2))) {
    sprintf(buf,"How many quest points do you want to award to %s?\r\n", GET_NAME(victim));
    send_to_char(buf, ch);
    return;
  }
  
  if (strncasecmp(arg1,"all",strlen(arg1)) && strncasecmp(arg1,"quest",strlen(arg1)))
    {
    GET_QP(victim) += atoi(arg2);
    sprintf(buf,"You have been awarded %d questpoints by %s!\r\n", atoi(arg2), GET_NAME(ch));
    send_to_char(buf, victim);
    sprintf(buf,"Ok, you give %d quest points to %s.\r\n", atoi(arg2), GET_NAME(victim));
    send_to_char(buf, ch);
    sprintf(logstr, "(GC) %s has awarded %s %d Qp.", GET_NAME(ch),
          GET_NAME(victim), atoi(arg2));
    xlog(SYS_AWARD, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), logstr, TRUE);
    }
}

ACMD(do_grasp)
{
  int result;
  char pow_buf[200];
  char *auras[] = {"bogus","menacing","golden"};

  sprintf(pow_buf,"$n is suddenly surrounded by a %s aura.",auras[(int)GET_SEX(ch)]);
    
  if (IS_NPC(ch)) {
    send_to_char("You're a mob, go away!\r\n",ch);
    return;
  }

  if (!GET_SPARK(ch) && !GET_TEACHABLE(ch) && (GET_CLASS(ch) != CLASS_CHANNELER) && (GET_CLASS(ch) != CLASS_WILDER)) {
    send_to_char("I'm afraid you can't channel a spark!\r\n",ch);
    return;
  }

  if (IS_GREYMAN(ch)) {
    send_to_char("Go away! The One Power can only be wielded by the living!\r\n",ch);
    return;
  }
  
  if (!GET_SKILL(ch,SKILL_GRASP)) {
    send_to_char("You don't know how!\r\n",ch);
    return;
  }
 
  

  if (ROOM_FLAGGED(ch->in_room,ROOM_NOMAGIC)) {
    send_to_char("You can't even sense the One Power here, much less grasp it!\r\n",ch);
    return;
  }
  
  if (IS_SET(GET_PERMANENT(ch),PRM_GENTLED)) {
    send_to_char("You are cut off from the One Power, for good!\r\n",ch);
    return;
  }
  
  if (IS_SET(PRF_FLAGS(ch),PRF_TGRASP)) {
    send_to_char("You can't grasp the One Power while holding on to the True Power!\r\n",ch);
    return;
  }

  if (IS_SET(AFF_FLAGS(ch),AFF_SHIELDED)) {
    send_to_char("You are shielded from the One Power!\r\n",ch);
    return;
  }
  
  if (GET_MANA(ch) <= 20) {
    send_to_char("You better rest for a while before trying this.\r\n",ch);
    return;
  }
 
  if (IS_SET(PRF_FLAGS(ch),PRF_GRASPING)) {
    send_to_char("You're already grasping the One Power!\r\n",ch);
    return;
  }
  
  result = number(0,100);
  
  if (result == 100) {
    send_to_char("You couldn't even grasp for breath, stupid!\r\n",ch);
    GET_MANA(ch) -= 100;
    return;
  }
  else if (result > GET_SKILL(ch,SKILL_GRASP)) {
    send_to_char("You fail to reach the One Power!\r\n",ch);
    GET_MANA(ch) -= 10;
    return;
  }
  else if ((result <= GET_SKILL(ch,SKILL_GRASP)) && (result > 5)) {
    send_to_char("You reach the One Power!\r\n",ch);
    SET_BIT(PRF_FLAGS(ch),PRF_GRASPING);
    GET_MANA(ch) -= 5;
  }
  else {
    result = number(0,100);
    if ((result <= GET_SKILL(ch,SKILL_GRASP)) && (GET_SKILL(ch,SKILL_GRASP) < 50)) {
      send_to_char("You become better in reaching the One Power!\r\n",ch);
      GET_SKILL(ch,SKILL_GRASP)++;
    }
    else {
      send_to_char("You reach the One Power!\r\n",ch);
    }
    SET_BIT(PRF_FLAGS(ch),PRF_GRASPING);
  }
  if (IS_SET(PRF_FLAGS(ch),PRF_IC))
     GET_TAINT(ch) += add_taint(ch,100);
  act(pow_buf,FALSE,ch,NULL,NULL,TO_OPUSERSS);
  if (GET_SEX(ch) == SEX_FEMALE)
     act("Your skin starts to tingle",FALSE,ch,NULL,NULL,TO_OPUSERNSS);
  return;
}




ACMD(do_release)
{
  int result;
  char pow_buf[200];
  char *auras[] = {"bogus","menacing","golden"};

  sprintf(pow_buf,"The %s aura around $n vanishes.",auras[(int) GET_SEX(ch)]);

  if (!IS_SET(PRF_FLAGS(ch),PRF_GRASPING)) {
    send_to_char("You aren't grasping the One Power!\r\n",ch);
    return;
  }
  
  if( ch->player_specials->linked_by)
  {
    send_to_char("You cannot release the source until the link is broken!\r\n", ch);
    return;
  }
  
  if(ch->player_specials->linking[0])
    perform_unlink(ch);
  
  if (!GET_SKILL(ch,SKILL_RELEASE)) {
    send_to_char("Panic! You don't know how to release the One Power!\r\n",ch);
    return;
  }
  
  result = number(0,100);
  
  if (result == 100) {
    send_to_char("Aaargh! The One Power scourge through you and you aren't\r\n"
                 "able to stop it!\r\n",ch);
    GET_MANA(ch) -= 100;
    return;
  }
  else if (result > GET_SKILL(ch,SKILL_RELEASE)) {
    send_to_char("Panic! You can't let go of the One Power!\r\n",ch);
    GET_MANA(ch) -= 25;
    return;
  }
  else if ((result <= GET_SKILL(ch,SKILL_RELEASE)) && (result > 5)) {
    send_to_char("With great effort you reluctantly let go of the One Power\r\n",ch);
    REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
  }
  else {
    result = number(0,100);
    if ((result <= GET_SKILL(ch,SKILL_RELEASE)) && GET_SKILL(ch,SKILL_RELEASE) < 50) {
      send_to_char("You become better at releasing the One Power!\r\n",ch);
      GET_SKILL(ch,SKILL_RELEASE)++;
    }
    else 
      send_to_char("With great effort you reluctantly let go of the One Power.\r\n",ch);
  }

  act(pow_buf,FALSE,ch,NULL,NULL,TO_OPUSERSS);
  REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
  check_mana(ch);
  fix_skim_gates(ch,GET_IDNUM(ch));
  send_to_char(REM_MAIN_TXT,ch);
  remove_char_maintained(ch);
  remove_room_maintained(ch);
  return;
}

ACMD(do_bground)
{
  if (IS_NPC(ch)) {
    send_to_char("Go away. You can't use this command.\r\n", ch);
    return;
  }
  send_to_char("Write your background as you want it to be seen by GLs:\r\n",ch);
  send_to_char("(/s saves /h for help)\r\n", ch); 
  send_to_char("Please format your desc to less than 75 characters per line.\r\n", ch);
  send_to_char("This will make your desc easier to read for those with smaller screens\r\n",ch);
  send_to_char("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n",ch);

  if (GET_BACKGROUND(ch)) {
    send_to_char("Old background:\r\n",ch);
    send_to_char(GET_BACKGROUND(ch),ch);
  }
  ch->desc->max_str = 5*(EXDSCR_LENGTH-24);
  ch->desc->backstr = ((NULL == GET_BACKGROUND(ch)) ? str_dup("No background entered yet.\r\n") : str_dup(GET_BACKGROUND(ch)));
  ch->desc->str = &GET_BACKGROUND(ch);
  ch->desc->current_text_line = count_newlines(ch->desc->str) + 1;
  SET_BIT(PLR_FLAGS(ch), PLR_WRITING); 
}

ACMD(do_showbground)
{
  struct char_data *vict = NULL;
  char arg1[MAX_INPUT_LENGTH+1], arg2[MAX_INPUT_LENGTH+1];
  struct char_guild_element *element;
  struct guildie_info *guildie;
  int i, guild_id;

  two_arguments(argument, arg1, arg2);

  if (!*arg1) 
  {
   if (!GET_BACKGROUND(ch)) 
   {
    send_to_char("You haven't set a background yet.\r\n",ch);
    return;
   } 
   sprintf(buf, "     &C%s's Background&n\r\n", GET_NAME(ch));
   sprintf(buf, "%s     &R=============", buf);
   for (i=0; i< strlen(GET_NAME(ch)); i++)
    sprintf(buf, "%s=", buf);
   sprintf(buf, "%s&n\r\n", buf);
   sprintf(buf, "%s%s", buf, GET_BACKGROUND(ch));
   page_string(ch->desc, buf, 1);
   return;
  }
  
  if (!*arg2)
  {
   if (!IS_IMMORTAL(ch))
   {
    send_to_char("Use 'showbground <guild name/num> <guildie>'.\r\n",ch);
    return;
   } 
   if (!(vict = get_char_vis(ch, arg1))) 
   {
    if (!(vict = xmlload_char(arg1))) 
    {
     send_to_char("No such character.\r\n", ch);
     return;
    }
    if (!GET_BACKGROUND(vict))
    {
     send_to_char("No Background set.\r\n", ch);
     return;
    }
    sprintf(buf, "     &C%s's Background&n\r\n", GET_NAME(vict));
    sprintf(buf, "%s     &R=============", buf);
    for (i=0; i< strlen(GET_NAME(vict)); i++)
     sprintf(buf, "%s=", buf);
    sprintf(buf, "%s&n\r\n", buf);
    sprintf(buf, "%s%s", buf, GET_BACKGROUND(vict));
    page_string(ch->desc, buf, 1);
    return; 
   }
   if (!GET_BACKGROUND(vict))
   {
    send_to_char("No Background set.\r\n", ch);
    return;
   }
   sprintf(buf, "     &C%s's Background&n\r\n", GET_NAME(vict));
   sprintf(buf, "%s     &R=============", buf);
   for (i=0; i< strlen(GET_NAME(vict)); i++)
    sprintf(buf, "%s=", buf);
   sprintf(buf, "%s&n\r\n", buf);
   sprintf(buf, "%s%s", buf, GET_BACKGROUND(vict));
   page_string(ch->desc, buf, 1);
   return;
  }

  element = GET_CHAR_GUILDS(ch);
  guild_id = atoi(arg1);

  while (element) {
   if (element->guild->id == guild_id) break;
   if (!strncasecmp(element->guild->name, arg1, strlen(arg1))) break; 
   element = element->next;
  }

  if (!element)
  {
   send_to_char("You are not a part of any such guild.\r\n", ch);
   return;
  }

  if (!STATUS_FLAGGED(element->guildie, STATUS_GL))
  {
   send_to_char("You are not a GL in that guild.\r\n", ch);
   return;
  }
   
  guildie = element->guild->guildies;
  while (guildie)
  { 
   if (!strncasecmp(guildie->name, arg2, strlen(arg2))) break;
   guildie = guildie->next;
  }
    
  if (!guildie)
  { 
   send_to_char("No such guildie.\r\n", ch);
   return;
  }

  if (!(vict = get_char_vis(ch, arg2))) 
  {
   if (!(vict = xmlload_char(arg2))) 
   {
    send_to_char("No such character.\r\n", ch);
    return;
   }
   if (!GET_BACKGROUND(vict))
   {
    send_to_char("No Background set.\r\n", ch);
    return;
   }
   sprintf(buf, "     &C%s's Background&n\r\n", GET_NAME(vict));
   sprintf(buf, "%s     &R=============", buf);
   for (i=0; i< strlen(GET_NAME(vict)); i++)
    sprintf(buf, "%s=", buf);
   sprintf(buf, "%s&n\r\n", buf);
   sprintf(buf, "%s%s", buf, GET_BACKGROUND(vict));
   page_string(ch->desc, buf, 1);
   return; 
  }
  if (!GET_BACKGROUND(vict))
  {
   send_to_char("No Background set.\r\n", ch);
   return;
  }
  sprintf(buf, "     &C%s's Background&n\r\n", GET_NAME(vict));
  sprintf(buf, "%s     &R=============", buf);
  for (i=0; i< strlen(GET_NAME(vict)); i++)
   sprintf(buf, "%s=", buf);
  sprintf(buf, "%s&n\r\n", buf);
  sprintf(buf, "%s%s", buf, GET_BACKGROUND(vict));
  page_string(ch->desc, buf, 1);
  return; 
}

ACMD(do_desc)
{
  char arg1[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH];
  struct player_extradesc *xdesc, *checkxd;
  int numfound=0;

  if (IS_NPC(ch)) {
    send_to_char("Go away. You can't use this command.\r\n", ch);
    return;
  }

 if (!*argument) {
  send_to_char("Write what you want people to see when looking at you.\r\n", ch);
  if (GET_DISGUISED(ch))
    send_to_char("Note: This is your disguised description that you change now.\r\n", ch);

  send_to_char("(/s saves /h for help)\r\n", ch); 
  send_to_char("Please format your desc to less than 75 characters per line.\r\n", ch);
  send_to_char("This will make your desc easier to read for those with smaller screens\r\n",ch);
  send_to_char("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n",ch);
  if (GET_DESC(ch))  {
    send_to_char("Old description:\r\n", ch);
    send_to_char(GET_DESC(ch), ch);
  }

  ch->desc->max_str = 3*(EXDSCR_LENGTH-24);
  ch->desc->backstr = ((NULL == GET_DESC(ch)) ? str_dup("An average MUD-addict\r\n") : str_dup(GET_DESC(ch)));
  ch->desc->str = &GET_DESC(ch);
  ch->desc->current_text_line = count_newlines(ch->desc->str) + 1;
  SET_BIT(PLR_FLAGS(ch), PLR_WRITING); 
 } else {
  two_arguments(argument, arg1, arg2);

  if (!strcasecmp("add", arg1)) {
    if (!*arg2) {
      send_to_char("USAGE:\r\n\r\n  DESC ADD <keyword>\r\n", ch);
      return;
    } else {
      if (NULL != (checkxd=find_playerxdesc(arg2, GET_CURRENTDESC(ch), ch))) {
        send_to_char("That extra desc exists.  Try using DESC EDIT instead.\r\n", ch);
      } else {
        xdesc=calloc(1, sizeof(struct player_extradesc));
        xdesc->keyword = strdup(arg2);
        xdesc->descnum=GET_CURRENTDESC(ch);
        xdesc->next=NULL;     

        send_to_char("(/s saves /h for help)\r\n", ch); 
        send_to_char("Please format your desc to less than 75 characters per line.\r\n", ch);
        send_to_char("This will make your desc easier to read for those with smaller screens\r\n",ch);
        send_to_char("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n",ch);

        ch->desc->max_str = 3*(EXDSCR_LENGTH-24);
        ch->desc->backstr = str_dup("A blank xdesc.\r\n");
        ch->desc->str = &xdesc->desc;
        ch->desc->current_text_line = count_newlines(ch->desc->str) + 1;
        SET_BIT(PLR_FLAGS(ch), PLR_WRITING);    

        add_playerxdesc(xdesc, ch);
      }
    }
  } else if (!strcasecmp("remove", arg1)) {
    if (!*arg2) {
      send_to_char("USAGE:\r\n\r\n  DESC REMOVE <keyword>\r\n", ch);
      return;
    } else {
      if (NULL != (xdesc=find_playerxdesc(arg2, GET_CURRENTDESC(ch), ch))) {
        rem_playerxdesc(xdesc, ch);
        send_to_char("Extra description removed.\r\n", ch);
      } else {
        send_to_char("Extra description not found.\r\n", ch);
      }
    }
  } else if (!strcasecmp("list", arg1)) {
    sprintf(buf, "&nDescription #%d has the following extra descs defined: ", (GET_CURRENTDESC(ch)+1));
    send_to_char(buf, ch);
    if (NULL!=(xdesc=GET_PLAYERXDESC(ch))) {
      while (xdesc) {
       if (xdesc->descnum == GET_CURRENTDESC(ch)) {
        numfound++;
        sprintf(buf, "&C%s&n ", xdesc->keyword);
        send_to_char(buf, ch);
       }
       xdesc=xdesc->next;
      }
      if (numfound<1) {
        send_to_char("None!\r\n", ch);
      } else {
        send_to_char("\r\n", ch);
      }
    } else {
      send_to_char("None!\r\n", ch);
    }
  } else if (!strcasecmp("edit", arg1)) {
    if (!*arg2) {
      send_to_char("USAGE:\r\n\r\n  DESC EDIT <keyword>\r\n", ch);
      return;
    } else {
      if (NULL != (xdesc=find_playerxdesc(arg2, GET_CURRENTDESC(ch), ch))) {
        rem_playerxdesc(xdesc, ch);

        send_to_char("(/s saves /h for help)\r\n", ch); 
        send_to_char("Please format your desc to less than 75 characters per line.\r\n", ch);
        send_to_char("This will make your desc easier to read for those with smaller screens\r\n",ch);
        send_to_char("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n",ch);

        ch->desc->max_str = 3*(EXDSCR_LENGTH-24);
        ch->desc->backstr = (xdesc->desc ? str_dup(xdesc->desc) : str_dup("A blank extra description."));
        ch->desc->str = &xdesc->desc;
        ch->desc->current_text_line = count_newlines(ch->desc->str) + 1;
        SET_BIT(PLR_FLAGS(ch), PLR_WRITING);    

        xdesc->next=NULL;
        add_playerxdesc(xdesc, ch);
      } else {
        send_to_char("Extra description not found.\r\n", ch);
      }
    }
  } else {
    send_to_char("USAGE:\r\n\r\n  DESC                  - Edit or description\r\n  DESC ADD <keyword>    - Add an extra desc\r\n  DESC REMOVE <keyword> - Remove an extra desc\r\n  DESC EDIT <keyword>   - Edit an existing extra desc\r\n  DESC LIST             - List your extra descs\r\n", ch);
  }
 }
}    

int get_where(struct obj_data *obj)
{
  if (IS_SET(GET_OBJ_WEAR(obj),ITEM_WEAR_FINGER))
    return WEAR_FINGER_R;
  if (IS_SET(GET_OBJ_WEAR(obj),ITEM_WEAR_NECK))
    return WEAR_NECK_1;
  if (IS_SET(GET_OBJ_WEAR(obj),ITEM_WEAR_BODY))
    return WEAR_BODY;
  if (IS_SET(GET_OBJ_WEAR(obj),ITEM_WEAR_HEAD))
    return WEAR_HEAD;
  if (IS_SET(GET_OBJ_WEAR(obj),ITEM_WEAR_LEGS))
    return WEAR_LEGS;
  if (IS_SET(GET_OBJ_WEAR(obj),ITEM_WEAR_HANDS))
    return WEAR_HANDS;
  if (IS_SET(GET_OBJ_WEAR(obj),ITEM_WEAR_FEET))
    return WEAR_FEET;
  if (IS_SET(GET_OBJ_WEAR(obj),ITEM_WEAR_ARMS))
    return WEAR_ARMS;
  if (IS_SET(GET_OBJ_WEAR(obj),ITEM_WEAR_SHIELD))
    return WEAR_SHIELD;
  if (IS_SET(GET_OBJ_WEAR(obj),ITEM_WEAR_ABOUT))
    return WEAR_ABOUT;
  if (IS_SET(GET_OBJ_WEAR(obj),ITEM_WEAR_WAIST))
    return WEAR_WAIST;
  if (IS_SET(GET_OBJ_WEAR(obj),ITEM_WEAR_WRIST))
    return WEAR_WRIST_R;
  if (IS_SET(GET_OBJ_WEAR(obj),ITEM_WEAR_HOLD))
    return WEAR_HOLD;

  return 0;
}



void comp_weapons(struct obj_data *obj1,struct obj_data *obj2,struct char_data *ch)
{
  char *name1,*name2;
  int temp = 0,temp2 = 0;

  temp  = (((GET_OBJ_VAL(obj1,1)*GET_OBJ_VAL(obj1,2)) + GET_OBJ_VAL(obj1,1)) / 2); 
  temp2 = (((GET_OBJ_VAL(obj2,1)*GET_OBJ_VAL(obj2,2)) + GET_OBJ_VAL(obj2,1)) / 2); 
  name1 = obj1->short_description;
  name2 = obj2->short_description;

  if (GET_OBJ_TYPE(obj1) != GET_OBJ_TYPE(obj2)) {
    sprintf(buf,"You can't compare %s and %s!\r\n",name1,name2);
    send_to_char(buf,ch);
    return;
  }

  if (temp == temp2)
    sprintf(buf,"%s seems almost as good as %s\r\n",name1,name2);
  else if (temp > temp2)
    sprintf(buf,"%s seems better than %s\r\n",name1,name2);
  else
    sprintf(buf,"%s seems worse than %s\r\n",name1,name2);
  send_to_char(buf,ch);
}

void comp_armor(struct obj_data *obj1,struct obj_data *obj2,struct char_data *ch)
{
  char *name1,*name2;

  name1 = obj1->short_description;
  name2 = obj2->short_description;
          
  if (GET_OBJ_TYPE(obj1) != GET_OBJ_TYPE(obj2)) {
    sprintf(buf,"You can't compare %s and %s!\r\n",name1,name2);
    send_to_char(buf,ch);
    return;
  }

  sprintf(buf,"%s seems to be ",name1);

  if (GET_OBJ_VAL(obj1, 0) > GET_OBJ_VAL(obj2, 0))
    sprintf(buf,"%sbetter than %s when ",buf,name2);
  else if (GET_OBJ_VAL(obj1, 0) < GET_OBJ_VAL(obj2, 0))
    sprintf(buf,"%sworse than %s when ",buf,name2);
  else
    sprintf(buf,"%salmost the same as %s when ",buf,name2);
  sprintf(buf,"%swhen it comes to encumbrance.\r\n",buf);
  send_to_char(buf,ch);

  sprintf(buf,"%s seems to protect you ",name1);

  if (GET_OBJ_VAL(obj1, 1) > GET_OBJ_VAL(obj2, 1))
    sprintf(buf, "%smore than %s\r\n", buf, name2);
  else if (GET_OBJ_VAL(obj1, 1) < GET_OBJ_VAL(obj2, 1))
    sprintf(buf, "%sless than %s\r\n", buf, name2);
  else 
    sprintf(buf, "%sjust as much as %s\r\n", buf, name2);
  send_to_char(buf,ch);
}

void comp_generic(struct obj_data *obj1,struct obj_data *obj2,struct char_data *ch)
{
  char *name1,*name2;
  int i,j,k=0,l=0,found = 0;

  name1 = obj1->short_description;
  name2 = obj2->short_description;
          
  for (i = 0; ((i < MAX_OBJ_AFFECT) && !found); i++) {
    for (j = 0; ((j < MAX_OBJ_AFFECT) && !found); j++) {
      if (obj1->affected[i].location && (obj1->affected[i].location == obj2->affected[j].location)) {
        found = TRUE;
        k = i;
        l = j;
      }
    }
  }
  if (!found) {
    sprintf(buf,"You can't compare %s and %s!\r\n",name1,name2);
    send_to_char(buf,ch);
    return;
  }
  else {
    sprintf(buf,"%s seems to be ",name1);
    if (obj1->affected[k].modifier > obj2->affected[l].modifier)
      sprintf(buf,"%sbetter than ",buf);
    else if(obj1->affected[k].modifier < obj2->affected[l].modifier)
      sprintf(buf,"%sworse than ",buf);
    else
      sprintf(buf,"%sabout as good as ",buf);
    sprintf(buf,"%s%s\r\n",buf,name2);
    send_to_char(buf,ch);
  }
}

ACMD(do_compare)
{
  struct obj_data *obj1 = NULL;
  char nr1[50];
  int type = 0,location = 0;
  
  one_argument(argument, nr1);
  if (!*nr1) {
    send_to_char("Compare what item?\r\n", ch);
    return;
  }

  if (!(obj1 = get_obj_in_list_vis(ch,nr1,ch->carrying)))  {
    send_to_char("Hey pal, you ain't carrying one of these!\r\n", ch);
    return;
  }

  type = GET_OBJ_TYPE(obj1);

  if (GET_OBJ_WEAR(obj1)) /* Compare if item is wearable! */
  switch (type)
  {
    case ITEM_WEAPON:
       if (!GET_EQ(ch,WEAR_WIELD_L) && !GET_EQ(ch,WEAR_WIELD_R)) {
         send_to_char("You aren't wearing anything compatible.\r\n",ch);
         return;
       }

       if (GET_EQ(ch,WEAR_WIELD_R) && CAN_WEAR(GET_EQ(ch,WEAR_WIELD_R),ITEM_WEAR_DUALWIELD)) 
         comp_weapons(obj1,GET_EQ(ch,WEAR_WIELD_R),ch);
       else {
         if (GET_EQ(ch,WEAR_WIELD_R)) 
           comp_weapons(obj1,GET_EQ(ch,WEAR_WIELD_R),ch);
         if (GET_EQ(ch,WEAR_WIELD_L)) 
           comp_weapons(obj1,GET_EQ(ch,WEAR_WIELD_L),ch);
      }
      break;

    case ITEM_BOW:
      if (GET_EQ(ch,WEAR_WIELD_R)) 
        comp_weapons(obj1,GET_EQ(ch,WEAR_WIELD_R),ch);
      if (GET_EQ(ch,WEAR_WIELD_L)) 
        comp_weapons(obj1,GET_EQ(ch,WEAR_WIELD_L),ch);
    break;
    
    case ITEM_ARMOR:
      location = get_where(obj1);

      if (!location) {
        send_to_char("I don't really understand how I should be able to compare THAT!\r\n",ch);
        return;
      }
      switch(location) {
        case WEAR_FINGER_R:
        case WEAR_NECK_1:
        case WEAR_WRIST_R:
          if (!GET_EQ(ch,location) && !GET_EQ(ch,(location+1))) {
            send_to_char("You aren`t wearing anything comparable!\r\n",ch);
            return;
          }
          if (GET_EQ(ch,location))
            comp_armor(obj1,GET_EQ(ch,location),ch);
          if (GET_EQ(ch,(location + 1)))
            comp_armor(obj1,GET_EQ(ch,(location + 1)),ch);
          break;
 
        default:
          if (!GET_EQ(ch,location)) {
            send_to_char("You aren`t wearing anything comparable!\r\n",ch);
            return;
          }
          comp_armor(obj1,GET_EQ(ch,location),ch);
          break;  
      }
    break;
    default:
      location = get_where(obj1);

      if (!location) {
        send_to_char("I don't really understand how I should be able to compare THAT!\r\n",ch);
        return;
      }
      switch(location) {
        case WEAR_FINGER_R:
        case WEAR_NECK_1:
        case WEAR_WRIST_R:
          if (!GET_EQ(ch,location) && !GET_EQ(ch,(location+1))) {
            send_to_char("You aren`t wearing anything comparable!\r\n",ch);
            return;
          }
          if (GET_EQ(ch,location))
            comp_generic(obj1,GET_EQ(ch,location),ch);
          if (GET_EQ(ch,(location + 1)))
            comp_generic(obj1,GET_EQ(ch,(location + 1)),ch);
          break;
 
        default:
          if (!GET_EQ(ch,location)) {
            send_to_char("You aren`t wearing anything comparable!\r\n",ch);
            return;
          }
          comp_generic(obj1,GET_EQ(ch,location),ch);
          break;  
      }
    break;
  } /* Switch */
  else
    send_to_char("I really don't see how you could be wearing an item like that?\r\n",ch);
}  

ACMD(do_imagine)
{
  
  int power;
  struct obj_data *obj;
  
  if (!GET_SKILL(ch, SKILL_DREAM)) {
    send_to_char("You imagine what it would be like to be rich.\r\n", ch);
    return;
  }
  if (!IS_AFFECTED(ch, AFF_DREAMING)) {
    send_to_char("But you're not in Tel'aran'rhiod now!\r\n", ch);
    return;
  }
  one_argument(argument, arg);
  if (!*arg) {
    send_to_char("&CYou can imagine these items in Tel'aran'rhiod:&n\r\n", ch);
    send_to_char("&GSword   Shield   Shoes   Helmet   Pants   Gloves&n\r\n", ch);
    send_to_char("&GShirt   Necklace Ring    Bracelet Backpack&n\r\n", ch);
    return;
  }
  power = GET_SKILL(ch, SKILL_DREAM);
  if (!isname(arg, "sword shield shoes helmet pants gloves shirt necklace ring bracelet backpack")) {
    send_to_char("Unknown imagine option.\r\n", ch);
    return;
  }
  obj = create_obj();
  obj->item_number = NOTHING;
  obj->in_room = NOWHERE;
  GET_OBJ_EXTRA(obj) = ITEM_TAR | ITEM_NODONATE | ITEM_NODROP | ITEM_NOSELL;
  GET_OBJ_WEIGHT(obj) = 1;
  GET_OBJ_TIMER(obj) = 300;
  
  if (isname(arg, "sword")) {
    obj->name = strdup("dream sword");
    obj->description = strdup("A glittering sword made of dreams lies here.");
    obj->short_description = strdup("a glittering dream sword");
    obj->action_description = strdup("A glittering sword made of dreams");
    GET_OBJ_TYPE(obj) = ITEM_WEAPON;
    GET_OBJ_WEAR(obj) = ITEM_WEAR_TAKE | ITEM_WEAR_WIELD;
    GET_OBJ_VAL(obj, 0) = power/5;
    GET_OBJ_VAL(obj, 1) = power/10;
    GET_OBJ_VAL(obj, 2) = 8;
    GET_OBJ_VAL(obj, 3) = 0;
  }
  else if (isname(arg, "shield")) {
    obj->name = strdup("dream shield");
    obj->description = strdup("A strong shield made of dreams lies here.");
    obj->short_description = strdup("a dreamshield");
    obj->action_description = strdup("A dreamshield.");
    GET_OBJ_TYPE(obj) = ITEM_ARMOR;
    GET_OBJ_WEAR(obj) = ITEM_WEAR_TAKE | ITEM_WEAR_SHIELD;
    GET_OBJ_VAL(obj, 0) = power/5;
    GET_OBJ_VAL(obj, 1) = power/5;
    GET_OBJ_VAL(obj, 2) = 0;
    GET_OBJ_VAL(obj, 3) = 0;
  }
  else if (isname(arg, "shoes")) {
    obj->name = strdup("dream shoes");
    obj->description = strdup("A shiny pair, made out of someones imagination stands here.");
    obj->short_description = strdup("a pair of shiny dreamboots");
    obj->action_description = strdup("A pair of dreamboots.");
    GET_OBJ_TYPE(obj) = ITEM_ARMOR;
    GET_OBJ_WEAR(obj) = ITEM_WEAR_TAKE | ITEM_WEAR_FEET;
    GET_OBJ_VAL(obj, 0) = power/5;
    GET_OBJ_VAL(obj, 1) = power/5;
    GET_OBJ_VAL(obj, 2) = 0;
    GET_OBJ_VAL(obj, 3) = 0;
  }
  else if (isname(arg, "helmet")) {
    obj->name = strdup("dream helmet");
    obj->description = strdup("A glittering helmet of dreams awaits an owner.");
    obj->short_description = strdup("a dream helmet");
    obj->action_description = strdup("A dream helmet.");
    GET_OBJ_TYPE(obj) = ITEM_ARMOR;
    GET_OBJ_WEAR(obj) = ITEM_WEAR_TAKE | ITEM_WEAR_HEAD;
    GET_OBJ_VAL(obj, 0) = power/5;
    GET_OBJ_VAL(obj, 1) = power/5;
    GET_OBJ_VAL(obj, 2) = 0;
    GET_OBJ_VAL(obj, 3) = 0;
  }
  else if (isname(arg, "pants")) {
    obj->name = strdup("dream pants");
    obj->description = strdup("A superb pair of pants, dream made, lies here.");
    obj->short_description = strdup("a perfect pair of dream pants");
    obj->action_description = strdup("A pair of dream pants.");
    GET_OBJ_TYPE(obj) = ITEM_ARMOR;
    GET_OBJ_WEAR(obj) = ITEM_WEAR_TAKE | ITEM_WEAR_LEGS;
    GET_OBJ_VAL(obj, 0) = power/5;
    GET_OBJ_VAL(obj, 1) = power/5;
    GET_OBJ_VAL(obj, 2) = 0;
    GET_OBJ_VAL(obj, 3) = 0;
  }
  else if (isname(arg, "gloves")) {
    obj->name = strdup("dream gloves");
    obj->description = strdup("A pair of superb gloves, made of a rare animals skin, lies here.");
    obj->short_description = strdup("a pair of dream gloves");
    obj->action_description = strdup("A pair of dream gloves.");
    GET_OBJ_TYPE(obj) = ITEM_ARMOR;
    GET_OBJ_WEAR(obj) = ITEM_WEAR_TAKE | ITEM_WEAR_HANDS;
    GET_OBJ_VAL(obj, 0) = power/10;
    GET_OBJ_VAL(obj, 1) = power/10;
    GET_OBJ_VAL(obj, 2) = 0;
    GET_OBJ_VAL(obj, 3) = 0;
  }
  else if (isname(arg, "shirt")) {
    obj->name = strdup("dream shirt");
    obj->description = strdup("A perfectly ironed, brilliant white shirt lies here.");
    obj->short_description = strdup("a white dream shirt");
    obj->action_description = strdup("A white dream shirt.");
    GET_OBJ_TYPE(obj) = ITEM_ARMOR;
    GET_OBJ_WEAR(obj) = ITEM_WEAR_TAKE | ITEM_WEAR_BODY;
    GET_OBJ_VAL(obj, 0) = power/5;
    GET_OBJ_VAL(obj, 1) = power/5;
    GET_OBJ_VAL(obj, 2) = 0;
    GET_OBJ_VAL(obj, 3) = 0;
  }
  else if (isname(arg, "necklace")) {
    obj->name = strdup("dream necklace");
    obj->description = strdup("A gold necklace with a beautiful setting of diamonds lies here.");
    obj->short_description = strdup("a dream gold necklace with diamonds");
    obj->action_description = strdup("");
    GET_OBJ_TYPE(obj) = ITEM_ARMOR;
    GET_OBJ_WEAR(obj) = ITEM_WEAR_TAKE | ITEM_WEAR_NECK;
    GET_OBJ_VAL(obj, 0) = power/10;
    GET_OBJ_VAL(obj, 1) = power/10;
    GET_OBJ_VAL(obj, 2) = 0;
    GET_OBJ_VAL(obj, 3) = 0;
  }
  else if (isname(arg, "ring")) {
    obj->name = strdup("dream ring");
    obj->description = strdup("A platinum ring with a large diamond lies here.");
    obj->short_description = strdup("a dream platinum ring");
    obj->action_description = strdup("A dream platinum ring");
    GET_OBJ_TYPE(obj) = ITEM_ARMOR;
    GET_OBJ_WEAR(obj) = ITEM_WEAR_TAKE | ITEM_WEAR_FINGER;
    GET_OBJ_VAL(obj, 0) = power/10;
    GET_OBJ_VAL(obj, 1) = power/10;
    GET_OBJ_VAL(obj, 2) = 0;
    GET_OBJ_VAL(obj, 3) = 0;
  }
  else if (isname(arg, "bracelet")) {
    obj->name = strdup("dream bracelet");
    obj->description = strdup("A stunningly beautiful bracelet in gold lies here.");
    obj->short_description = strdup("a dream gold bracelet");
    obj->action_description = strdup("A dream gold bracelet.");
    GET_OBJ_TYPE(obj) = ITEM_ARMOR;
    GET_OBJ_WEAR(obj) = ITEM_WEAR_TAKE | ITEM_WEAR_WRIST;
    GET_OBJ_VAL(obj, 0) = power/10;
    GET_OBJ_VAL(obj, 1) = power/10;
    GET_OBJ_VAL(obj, 2) = 0;
    GET_OBJ_VAL(obj, 3) = 0;
  }
  else if (isname(arg, "backpack")) {
    obj->name = strdup("dream backpack");
    obj->description = strdup("A nice looking backpack with many pockets stands here.");
    obj->short_description = strdup("a spacious dream backpack");
    obj->action_description = strdup("A dream backpack");
    GET_OBJ_TYPE(obj) = ITEM_CONTAINER;
    GET_OBJ_WEAR(obj) = ITEM_WEAR_TAKE | ITEM_WEAR_WAIST;
    GET_OBJ_VAL(obj, 0) = 5*power;
    GET_OBJ_VAL(obj, 1) = 0;
    GET_OBJ_VAL(obj, 2) = -1;
    GET_OBJ_VAL(obj, 3) = 0;
  }
  else {
    send_to_char("Something very strange has happened...Please tell an imm.\r\n", ch);
    extract_obj(obj);
    return;
  }
  sprintf(buf, "%s suddenly appears in front of $n, who takes it.", obj->short_description);
  act(buf, TRUE, ch, 0, 0, TO_ROOM);
  send_to_char("You create an item with the power of your imagination.\r\n", ch);
  obj_to_char(obj, ch);  
}

ACMD(do_hide_tracks)
{
  struct affected_type af;
  struct char_data *list;
  int result;
  
  af = clear_aff_var(af);
  

  if (!GET_SKILL(ch,SKILL_HIDE_TRACKS)) {
    send_to_char("You haven't got a clue on how!\r\n",ch);
    return;
  }
  if (IS_AFFECTED(ch,AFF_NOTRACK)) {
    send_to_char("You're already hiding your tracks!\r\n",ch);
    return;
  }
 
  result = generic_result_mod(ch,SKILL_HIDE_TRACKS,NULL,FALSE,(GET_INT(ch) - 15));

  if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
    send_to_char("You try to hide your tracks, but fail utterly.\r\n",ch);
    return;
  }
  af.type = SKILL_HIDE_TRACKS;
  af.duration = 30 + (GET_LEVEL(ch) / 10);
  af.bitvector = AFF_NOTRACK;
  af.bitvector2 = 0;
  af.bitvector3 = 0;
  af.bitvector4 = 0;
  af.location = 0;
  af.modifier = 0;
   
  affect_to_char(ch,&af);
  send_to_char("You hide your tracks swiftly.\r\n",ch);
  act("$n begins hiding $s tracks expertly!",FALSE,ch,0,0,TO_ROOM);

  for (list = world[ch->in_room].people; list; list = list->next_in_room) {
    if ((list != ch) && is_in_group(ch,list)) {
      affect_to_char(list,&af);  
      act("You hide $Ns tracks.",FALSE,ch,0,list,TO_CHAR);
      act("$n hides your tracks.",FALSE,ch,0,list,TO_VICT);
    }
  }
}
    
ACMD(do_learn)
{
  if (!GET_SKILL(ch,SKILL_LEARN)) {
    send_to_char("You don't know how to teach!\r\n",ch);
    return;
  }
}


ACMD(do_lines)
{
  int l = 0;
  skip_spaces(&argument);

  if (!*argument) {
    sprintf(buf, "You will see %d lines at a time when paging.\r\n", GET_LINES(ch));
    send_to_char(buf, ch);
    return;
  }
  l = atoi(argument);
  if ((l < 10) || (l > 200)) {
    send_to_char("That's not a valid number. Range is 10-200 lines.\r\n", ch);
    return;
  }
  sprintf(buf, "Number of output lines changed to %d.\r\n", l);
  send_to_char(buf, ch);
  GET_LINES(ch) = l; 
}

ACMD(do_herblore)
{
  int mana = 0, hp = 0, move = 0, result = 0;
  struct char_data *vict = 0;
  struct affected_type af;

  if ((GET_CLASS(ch) != CLASS_WISDOM) && (GET_LEVEL(ch) < LVL_IMMORT)) {
    send_to_char("Only wisdoms can use this skill.\r\n", ch);
    return;
  }
  if (!GET_SKILL(ch, SKILL_HERBLORE)) {
    send_to_char("Learn the skill first!\r\n", ch);
    return;
  }  
  skip_spaces(&argument);
  if (!*argument) {
    send_to_char("Use your herblore skills on whom?\r\n", ch);
    return;
  }
  if (!(vict = get_char_room_vis(ch, argument))) {
    send_to_char(NOPERSON, ch);
    return;
  }
  if (IS_NPC(vict)) {
    send_to_char("Not on mobs, you don't!\r\n", ch);
    return;
  }
  if (GET_HIT(vict) == GET_MAX_HIT(vict))
  {
    asend_to_char(ch, "%s isn't hurt in any way...\r\n", GET_NAME(vict));
    return;
  }

  if (affected_by_spell(vict, SKILL_HERBLORE)) {
    act("$N already has herbal bandages on $M. Wait a little.", TRUE, ch, 0, vict, TO_CHAR);
    return;
  }
  af = clear_aff_var(af);
  result = generic_result(ch, SKILL_HERBLORE, NULL, FALSE);
  af.duration = 2;
  af.type = SKILL_HERBLORE;

  if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
    act("$n tries to utilize $s skills to heal $N, but fails.", TRUE, ch, 0, vict, TO_NOTVICT);
    asend_to_char(vict, "%s tried to help you with herbs, but failed.\r\n", GET_NAME(ch));
    asend_to_char(ch, "You try to help %s, but fail.\r\n", GET_NAME(vict));
    GET_MANA(ch) /= 2;
    affect_to_char(vict, &af);
    return;
  }
  mana =  number(0, GET_LEVEL(ch));
  move =  number(0, 5*GET_LEVEL(ch));
  hp   =  number(0, 5*GET_LEVEL(ch));
  act("$n attends $N's wounds..doesn't $E look better?", TRUE, ch, 0, vict, TO_NOTVICT);
  asend_to_char(ch, "You tend to %s's wounds...\r\n", GET_NAME(vict));
  asend_to_char(vict, "%s puts a herb bandange on your wounds. You feel better.\r\n", GET_NAME(ch));
  GET_HIT(vict) = MIN(GET_MAX_HIT(vict), GET_HIT(vict) + hp);
  GET_MOVE(vict) = MIN(GET_MAX_MOVE(vict), GET_MOVE(vict) + move);
  GET_MANA(vict) = MIN(GET_MAX_MANA(vict), GET_MANA(vict) + mana);
  af.duration ++;
  affect_to_char(vict, &af);
}

ACMD(do_prefix)
{
  skip_spaces(&argument);

  if (!*argument) {
    if (!GET_PREFIX(ch))
      send_to_char("You gotta make your prefix _something_!\r\n", ch);
    else {
      send_to_char("Prefix removed!\r\n", ch);
      free(GET_PREFIX(ch));
      GET_PREFIX(ch) = NULL;
    }
  }
  else {
    if (GET_PREFIX(ch))
      free(GET_PREFIX(ch));
    sprintf(buf, "Prefix changed to: %s\r\n", argument);
    send_to_char(buf, ch);
    GET_PREFIX(ch) = str_dup(argument);
  }
}

ACMD(do_become) 
{ struct descriptor_data *d, *next_d;

  if (IS_NPC(ch) || !ch->desc)
    return;

  if (!(IS_NPC(ch)) && (GET_SLEEPMOB(ch)))
    wakeup_sequence(ch);

  if (NORMAL_DREAMER(ch)) 
  { if (ROOM_FLAGGED(ch->in_room, ROOM_DREAM)) 
    { REMOVE_BIT(ROOM_FLAGS(IN_ROOM(ch)), ROOM_DREAM);
      if (GET_DREAMENTERED(ch)) 
	wakeup_sequence(GET_DREAMENTERED(ch));
    }
    wakeup_sequence(ch);
  }

  if (ch->player_specials->linking[0])
  { send_to_char("You must dissolve your link first!\r\n", ch);
    return;
  }

  if( PRF_FLAGGED(ch, PRF_QUEST) )
    (void)PRF_TOG_CHK(ch, PRF_QUEST);

  if (PRF_FLAGGED(ch, PRF_AFK))
    do_noafk(ch, 0, cmd, 0);

  if (PRF2_FLAGGED(ch, PRF2_AFW))
    do_noafw(ch, 0, cmd, 0);

  if (ch->player_specials->linked_by)
  { send_to_char("You cannot leave until you are released from your link!\r\n", ch);
    return;
  }
  if (GET_POS(ch) == POS_FIGHTING)
    send_to_char("No way!  You're fighting for your life!\r\n", ch);
  else if (GET_POS(ch) < POS_STUNNED) 
  { send_to_char("You die before your time...\r\n", ch);
    die(ch, ch);
  }
  else if (IS_AFFECTED(ch,AFF_NOQUIT))
    send_to_char("Sorry, you are not allowed to quit yet.\r\n",ch);
  else 
  { if (GET_PRIVATE(ch))
      do_private_channel(ch,"",0,SCMD_P_CLOSE);

    if (!cant_channel(ch) && (GET_LEVEL(ch) > 10)) 
    { send_to_char(REM_MAIN_TXT,ch);
      remove_char_maintained(ch);
      remove_room_maintained(ch);
    }

    if (DICING(ch) && char_still_online(DICING(ch)))
      DICING(DICING(ch)) = NULL;

    write_aliases(ch);

    if (IS_AFFECTED(ch,AFF_MORALE))
      affect_from_char(ch,SKILL_MORALE);

    /*
     * kill off all sockets connected to the same player as the one who is
     * trying to quit.  Helps to maintain sanity as well as prevent duping.
     */
    for (d = descriptor_list; d; d = next_d) 
    { next_d = d->next;
      if (d == ch->desc)
        continue;
      if (d->character && (GET_IDNUM(d->character) == GET_IDNUM(ch)))
        close_socket(d);
    }

    sprintf(buf, "%s is becoming...", GET_NAME(ch));
    act("$n becomes somebody else...", TRUE, ch, 0, 0, TO_ROOM);
    xlog(SYS_CONN, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), buf, TRUE);
    d = ch->desc;

    save_XML_playerfile(ch, CRASH_FILE);
    strip_char_of_all_eq(ch);
    extract_char(ch, FALSE);            /* Char is saved in extract char */

    if (GET_WAGON(ch) && ch->pulling)
      extract_obj(ch->pulling);

    SEND_TO_Q("Who do you wish to become? ", d);
    STATE(d) = CON_GET_NAME;

    free_char(ch);
    d->character = NULL;
  }
}

ACMD(do_shortgate)
{
  char arg1[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH];
  int dir = -1, steps = -1, i,curroom;

  if (!GET_SKILL(ch,SPELL_GATE)) {
    send_to_char("You do not know how to Gate!\r\n",ch);
    return;
  }

  if (GET_TIMES(ch,CAN_WEAVE)) {
    send_to_char("\\c01You can't weave again so soon!\\c00",ch);
    return;
  }

  if(ch->player_specials->linked_by) {
    send_to_char("You cannot control the One Power right now.\r\n", ch);
    return;
  }

  skip_spaces(&argument);
  if (!*argument) {
    send_to_char("Usage: shortgate <direction> <number of rooms>\r\n",ch);
    return;
  }

  two_arguments(argument,arg1,arg2);

  dir = search_block(arg1,(const char **)dirs,FALSE);
  if (-1 == dir) {
    asend_to_char(ch,"%s is not a direction I ever heard of!\r\n",arg1);
    return;
  }

  if (!is_number(arg2)) {
    asend_to_char(ch,"Second arg should be a number..."
          "Does %s looks like a number to you? "
          "It sure doesn't to me!\r\n",arg2);
    return;
  }

  steps = atoi(arg2);

  if (steps < 0) {
    send_to_char("Gate a negative number of rooms away? How odd, surely you jest!\r\n",ch);
    return;
  }

  if (!steps) {
    send_to_char("Whats the use to gate 0 rooms away, you're already here!\r\n",ch);
    return;
  }

  if (steps > 10) {
    send_to_char("*sigh* You can only shortgate 10 rooms away.\r\n",ch);
    return;
  }

  /* Now search steps step in direction dir */
  curroom = ch->in_room;
  for (i = 0; i < steps; i++) {
    if (NULL == world[curroom].dir_option[dir] 
    || -1 == world[curroom].dir_option[dir]->to_room) {
      asend_to_char(ch,"Can't gate %d rooms in that direction...not enough rooms!\r\n",steps);
      return;
    }
    curroom = world[curroom].dir_option[dir]->to_room;
  }

  if ((315 == zone_table[world[curroom].zone].number) ||
      (315 == zone_table[world[ch->in_room].zone].number)) {
    send_to_char("Shortgating is not allowed within this zone!\r\n",ch);
    return;
  }

  /*
   * Okay, we've a room here, call the actual spell...after making sure we've grasped!
   */
  if (!IS_SET(PRF_FLAGS(ch),PRF_GRASPING) && !IS_SET(PRF_FLAGS(ch),PRF_TGRASP)) {
    do_grasp(ch,"",0,0);
  }

  if (!IS_SET(PRF_FLAGS(ch),PRF_GRASPING) && !IS_SET(PRF_FLAGS(ch),PRF_TGRASP)) {
    if (GET_SKILL(ch,SKILL_TGRASP))
      send_to_char("You must grasp the One/True Power before weaving any weaves!\r\n",ch);
    else
      send_to_char("You must grasp the One Power before weaving any weaves!\r\n",ch);
    return;
  }

  if (ROOM_FLAGGED(curroom,ROOM_NOGATE)||ROOM_FLAGGED(curroom,ROOM_NOMAGIC)) {
    send_to_char("Sorry, this isn't allowed in that room!\r\n",ch);
    return;
  }

  gate(ch,NULL,NULL,&(world[curroom]),"",-1,-1,0,0,1.0);
}
