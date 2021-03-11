/******************************************************************************
 * $Id: act.wizard.c,v 1.31 2004/07/17 22:07:17 fang Exp $
 *   
 * File: act.wizard.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: act.wizard.c,v $
 *   Revision 1.31  2004/07/17 22:07:17  fang
 *   changed behavior of tunnel rooms -tiek
 *
 *   Revision 1.30  2003/12/05 22:24:06  fang
 *   Fixed for teleport too.
 *
 *   Revision 1.29  2003/12/05 19:58:25  fang
 *   Immortals will now vacate furniture when using goto.
 *
 *   Revision 1.28  2003/06/10 12:16:08  fang
 *
 *   Added UNIQUE flag for mobs and objs. A unique obj/mob can only exists one
 *   instance at the time on the MUD, no matter what. A unique obj will not
 *   save to characters obj file either.
 *
 *   Revision 1.27  2003/06/06 07:14:06  fang
 *   Fixed typo in xlog.
 *
 *   Revision 1.26  2003/06/06 02:41:30  fang
 *   Tied guilding, guildieset, guildset xlog messages in to the GL CG.
 *
 *   Revision 1.25  2003/06/05 07:15:33  fang
 *   Guildieset xlog is now level 203.
 *
 *   Revision 1.24  2003/05/18 09:27:53  fang
 *   Add syslog and xlog for guilding/deguilding
 *
 *   Revision 1.23  2003/03/26 14:53:19  fang
 *   Minor stuff fixed...
 *
 *   Revision 1.22  2003/02/03 09:04:44  fang
 *   NPC-check in add_to_rplog
 *
 *   Revision 1.21  2003/01/24 08:54:29  fang
 *   manaconv now uses a PRM_ flag tokeep track opf players who have used manaconv. A smallish bug fixed in do_stat_character() so that it nows properly displays PRM-flags for the stat'ed char.
 *
 *   Revision 1.20  2002/11/22 07:22:05  fang
 *   fisc: prpared do_multi for multi checks with auth checks
 *
 *   Revision 1.19  2002/11/14 14:18:22  fang
 *   Just some minor changes to qcount, to display a total.
 *
 *   Revision 1.18  2002/11/07 04:02:01  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.17  2002/10/13 15:35:30  fang
 *   fisch: chaned xlog SYSTEMERRORS to have an easier trigger word.
 *
 *   Revision 1.16  2002/10/10 17:28:12  fang
 *   fisch: adding a SYSTEM ERROR class to xlog
 *
 *   Revision 1.15  2002/10/03 06:40:48  fischer
 *   fischer: tedit error that would have screwed up half the teditable files
 *
 *   Revision 1.14  2002/10/03 05:55:09  fischer
 *   fischer: moved ideas to be used as an old ideas and added newideas file
 *
 *   Revision 1.13  2002/09/27 18:43:59  fischer
 *   Fisch: fixed bug that overwrote oldnews with news if editor was aborted
 *
 *   Revision 1.12  2002/09/16 18:22:56  fang
 *   Put in WC guild retainers and some IC checks for retainers.
 *
 *   Revision 1.11  2002/09/15 00:50:07  fang
 *   Drath: XLOG update - Categories added to xlog display automatically
 *
 *   Revision 1.10  2002/09/14 06:06:12  fang
 *   fisch: minor changes in the xlog code
 *
 *   Revision 1.9  2002/09/13 19:05:39  fang
 *   fisch: Added XLOG DEBUG catagory and set some around guild channel
 *
 *   Revision 1.8  2002/09/11 08:32:36  fang
 *   Fischer: Retainer xlog entries (and catagory) for name and title changes
 *
 *   Revision 1.7  2002/09/05 20:56:30  fang
 *   Drath: added CRETODO and SHOWCRETODO for MassyPants
 *
 *   Revision 1.6  2002/08/27 21:29:39  fang
 *   Drath: Player Extra Descriptions
 *
 *   Revision 1.5  2002/08/20 21:03:50  fang
 *   Drath: Hunter GSKILLS
 *
 *   Revision 1.4  2002/08/04 09:39:55  fang
 *   Drath: SOCIALIDEA and SHOWSOCIALIDEA seperate from IDEA
 *
 *   Revision 1.3  2002/07/29 22:18:56  fang
 *   AFF3 created and gskill update
 *
 *   Revision 1.2  2002/07/28 13:56:03  fang
 *   Removed dmlloc...just to test.
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.21  2002/07/12 03:37:33  fang
 *   Drath: MYCOLOR
 *
 *   Revision 1.20  2002/07/06 07:05:12  fang
 *   Drath: IMMLOCK for Fred and 2nd to last XLOG commit
 *
 *   Revision 1.19  2002/06/25 19:38:22  fang
 *   Drath: XLOG Commit #2
 *
 *   Revision 1.18  2002/06/25 19:23:51  fang
 *   Drath: XLOG Commit #2
 *
 *   Revision 1.17  2002/06/24 14:29:54  fang
 *   Drath: Added CANTGECHO setting to per-user disable GECHO
 *
 *   Revision 1.16  2002/06/23 19:11:18  fang
 *   Drath: Fix to do_echo and first commit of XLOG
 *
 *   Revision 1.15  2002/06/21 14:36:13  fang
 *   Drath: Fixed a small bug with RPTAG where EMOTE crashes when the char is in a mirror room.
 *
 *   Revision 1.14  2002/06/20 14:44:33  fang
 *   Drath: Fixed ECHO
 *
 *   Revision 1.13  2002/06/20 03:07:10  fang
 *   Maz: RPTAG
 *
 *   Revision 1.12  2002/06/02 22:14:44  fang
 *   Maz: PEDIT
 *
 *   Revision 1.11  2002/06/01 20:43:16  fang
 *   Maz: LOAD and CLONE log real name of disguised players, and fixed SFX bug.
 *
 *   Revision 1.10  2002/05/31 19:17:56  fang
 *   fixed invis snoop syslog bug
 *
 *   Revision 1.9  2002/05/27 10:52:48  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.8  2002/05/24 17:46:43  fang
 *   Maz: Added WIZLIST to TEDIT and removed IMMLIST.
 *
 *   Revision 1.7  2002/05/18 04:04:33  fang
 *   Maz: Added QPs to STAT
 *
 *   Revision 1.6  2002/05/12 23:50:46  fang
 *   Added in IGNORE system, and removed AutoHotboots until the cap can be fixed for good. -Maz
 *
 *   Revision 1.5  2002/03/14 08:03:20  fang
 *   *** empty log message ***
 *
 *   Revision 1.4  2002/03/12 15:02:15  fang
 *   Minor fixes to interpreter.c. dig is updated to utilize the new build permission thingies.
 *
 *   Revision 1.3  2002/02/27 02:17:40  fang
 *   Fixed disguise so it saves.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.80  2002/02/14 13:41:56  mud
 *   *** empty log message ***
 *
 *   Revision 1.79  2002/02/01 13:53:47  mud
 *   PRM fl|ags now shown in stat
 *
 *   Revision 1.78  2002/01/28 08:52:57  mud
 *   Changed delete so that is a character has qps (raw, skills or eq)
 *   the normal delete proicedure will not work, and a warning is printed.
 *   Instead of "delete yes delete me" (qhich still works for no-qp chars)
 *   the qp-chars will have to type "delete yes delete me and my qps!".
 *
 *   Revision 1.77  2002/01/24 21:33:13  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.76.2.3  2002/01/23 21:49:37  mud
 *   Removed objsave.[c|h] (old objsave stuff).
 *   Removed obj_file_elem and rent structs.
 *   Removed final vestiges of old guild system, except parts used
 *   in guild.[c|h] for backwards compatibility.
 *
 *   Revision 1.76.2.2  2002/01/22 03:05:10  mud
 *   More cleanup. Removed extern decls from source files. Protected some DG headers from multiple inclision
 *
 *   Revision 1.76.2.1  2002/01/21 01:23:39  mud
 *   Code cleanup. Aiel clans/socities gone. Lot of old guild code gone.
 *
 *   Revision 1.76  2002/01/15 19:17:26  mud
 *   Warning free...let's keep it that way...
 *
 *   Revision 1.75  2002/01/14 00:15:46  mud
 *   Score should no longer show negative amount of objs until you hit some 2*10^9 objs. :)
 *
 *   Revision 1.74  2002/01/07 07:52:52  mud
 *   Added fix to revive code.
 *
 *   Revision 1.73  2001/11/18 13:17:01  mud
 *   Spelling mistakes and etcetera.
 *
 *   Revision 1.72  2001/10/14 03:54:07  mud
 *   *** empty log message ***
 *
 *   Revision 1.71  2001/09/26 13:55:19  mud
 *   *** empty log message ***
 *
 *   Revision 1.70  2001/09/15 15:11:32  mud
 *   *** empty log message ***
 *
 *   Revision 1.69  2001/09/06 15:05:00  mud
 *   *** empty log message ***
 *
 *   Revision 1.68  2001/09/06 06:30:58  mud
 *   *** empty log message ***
 *
 *   Revision 1.67  2001/09/06 05:41:19  mud
 *   *** empty log message ***
 *
 *   Revision 1.66  2001/08/23 16:55:54  mud
 *   *** empty log message ***
 *
 *   Revision 1.65  2001/08/22 18:28:50  mud
 *   *** empty log message ***
 *
 *   Revision 1.64  2001/08/17 04:22:12  mud
 *   *** empty log message ***
 *
 *   Revision 1.63  2001/08/11 17:48:57  mud
 *   Modified to make wardersense and warderheal obsolete and have the code that
 *   checked for those skills check to see if the character is a bonded Gaidin - Illaria
 *
 *   Revision 1.62  2001/08/06 17:58:13  mud
 *   Modified clearbond so that when a sedai bond is cleared it sets
 *   wardersense and warderheal to 0. - Illaria
 *
 *   Revision 1.61  2001/07/16 09:51:45  mud
 *   *** empty log message ***
 *
 *   Revision 1.60  2001/07/09 15:11:17  mud
 *   *** empty log message ***
 *
 *   Revision 1.59  2001/07/09 15:08:46  mud
 *   *** empty log message ***
 *
 *   Revision 1.58  2001/05/29 08:12:44  mud
 *   Minor bugfix to zone_reset(), soe more work on qstrip...
 *
 *   Revision 1.57  2001/05/22 09:07:02  mud
 *   DG-Scripts for Fang.
 *
 *   Revision 1.56  2001/05/17 11:21:07  mud
 *   Final residues of old pfile code should be swept away by now. No cleansing of the structures and such things though
 *
 *   Revision 1.55  2001/05/17 10:11:17  mud
 *   Pretitle code in and working.
 *
 *   Revision 1.54  2001/05/14 13:47:02  mud
 *   Qstrip now handles qeq/qps/qskills both for online and offline chars.
 *
 *   Revision 1.53  2001/04/27 05:04:18  mud
 *   Bugfixes for XML. "set file" now actually saves settings to files, and saves CMDGroups for morts as well as Imms! :)
 *
 *   Revision 1.52  2001/04/25 02:22:58  mud
 *   Bugfixes: Base AC back to 10, disguise OK,. IP and correct last-logon-time in last-command
 *
 *   Revision 1.51  2001/04/20 14:50:01  mud
 *   *** empty log message ***
 *
 *   Revision 1.50  2001/04/15 02:30:14  mud
 *   Added a bundle of fixes and notifylist.
 *
 *   Revision 1.49  2001/04/06 07:00:23  mud
 *   Final xml changes before going to Beta... .:)
 *
 *   Revision 1.48  2001/04/04 18:35:27  mud
 *   Bug fixing
 *
 *   Revision 1.47  2001/03/13 04:53:04  mud
 *   Fixed some typos
 *   !Save objects won't get saved (any !save containes will delete everything it contains)
 *
 *   Revision 1.46  2001/03/09 11:15:42  mud
 *   Fixed so it doesn't crash if a non-guilded GOD tries to glist
 *   Added showing of gchannel to "myguilds"
 *   Changed the parsing of the socials file (to handle comments)
 *
 *   Revision 1.45  2001/01/31 15:44:53  mud
 *   *** empty log message ***
 *
 *   Revision 1.44  2001/01/23 12:09:45  mud
 *   *** empty log message ***
 *
 *   Revision 1.43  2001/01/19 17:51:49  mud
 *   Added line numbers to the editor.
 *
 *   Revision 1.42  2000/12/11 16:21:13  mud
 *   Some minor bugfixes, and some parts of qstrip done
 *
 *   Revision 1.41  2000/12/10 09:32:13  mud
 *   Fixed train overflow when raising mana/move/hp
 *
 *   Revision 1.40  2000/12/03 10:26:10  mud
 *   More QEQ tracking, begun some minro mods to have exdescs editable by craft/tool.
 *
 *   Revision 1.39  2000/12/02 10:30:23  mud
 *   qcount method added and tested.
 *
 *   Revision 1.38  2000/12/02 09:03:14  mud
 *   Added tracking info for qeq. If a questitem is purchased in shop, the name
 *   of the byer, the vnum of the shop (and the room  the shop was in) as well
 *   as the date of purchase is added to item. If item is gaine din any other
 *   way (loading, slaying shopkeepers, etc, etc) then this info will not be
 *   present. stat <object> will display this info, or a warning if none is
 *   present.
 *
 *   Revision 1.37  2000/11/28 16:31:39  mud
 *   Fixed trans to make the player stand up.
 *
 *   Revision 1.36  2000/11/17 18:05:27  mud
 *   *** empty log message ***
 *
 *   Revision 1.35  2000/11/13 04:52:16  mud
 *   *** empty log message ***
 *
 *   Revision 1.34  2000/11/13 00:05:45  mud
 *   *** empty log message ***
 *
 *   Revision 1.33  2000/11/09 14:13:47  mud
 *   Getting closer to switching-dat with xml chars
 *
 *   Revision 1.32  2000/11/08 20:34:25  mud
 *   Added the todo list for imms
 *
 *   Revision 1.31  2000/11/07 15:32:40  mud
 *   Fixed tedit for Builder CG
 *
 *   Revision 1.30  2000/10/31 01:28:09  mud
 *   added multiple recipients for mudmail. 5 should be enough..
 *
 *   Revision 1.29  2000/10/07 04:00:43  mud
 *   *** empty log message ***
 *
 *   Revision 1.28  2000/10/06 18:23:12  mud
 *   *** empty log message ***
 *
 *   Revision 1.27  2000/10/06 16:01:40  mud
 *   The new tell buffer and the RPlog :)
 *
 *   Revision 1.26  2000/09/04 02:55:27  mud
 *   Warnings removed...
 *
 *   Revision 1.25  2000/08/20 07:59:52  mud
 *   added a help_index. ordered linkedlist of help keywords and their pos in the help_table
 *
 *   Revision 1.24  2000/08/20 06:58:38  mud
 *   *** empty log message ***
 *
 *   Revision 1.23  2000/08/14 19:11:55  mud
 *   The new guild system.
 *
 *   Revision 1.22  2000/08/05 16:54:04  mud
 *   changed lastemote to also do pose, pemote, and cemote.
 *
 *   Revision 1.21  2000/08/01 11:50:59  mud
 *   added roomset command. sets roomflags that won't save
 *
 *   Revision 1.20  2000/07/31 16:13:51  mud
 *   added lastemote
 *
 *   Revision 1.19  2000/07/28 17:44:32  mud
 *
 *   New talent code.
 *
 *   Revision 1.18  2000/06/23 19:29:50  mud
 *   Gavin: Removed syslog messages for score and stat.
 *
 *   Revision 1.17  2000/06/22 13:54:41  mud
 *   Added syslog entires to scoring and stating other people.
 *
 *   Revision 1.16  2000/06/17 20:23:40  mud
 *   New OLC perm system fixes.
 *
 *   Revision 1.15  2000/06/16 01:16:33  mud
 *   Added vnum -f .. -Tesla
 *
 *   Revision 1.14  2000/06/13 17:58:58  mud
 *
 *   Removed gecho from do_authorize, modified text sent to victim of do_authorize.
 *
 *   Revision 1.13  2000/05/28 17:49:41  mud
 *   Fixes to wardersense, plus tedit for bugs, ideas and typos.
 *
 *   Revision 1.12  2000/05/26 14:07:02  mud
 *   Recall command, showideas, showbugs, showtypos.
 *
 *   Revision 1.11  2000/05/26 09:20:32  mud
 *   Fixed findherb, added a herbident ACMD, IMM commands level fixes.
 *
 *   Revision 1.10  2000/05/25 15:06:47  mud
 *
 *   Talent code changed to utilize char* arrays instead of a million #defines.
 *   Changes made to do_set, do_stat, do_score.
 *   Imm command Prove added.
 *
 *   Revision 1.9  2000/05/24 08:41:12  mud
 *   Aiel endurance changes
 *
 *   Revision 1.8  2000/04/29 15:38:15  mud
 *   Fixed disguise so that a darkfriend will still be a darkfriend after
 *   disguise.
 *   Added hotboot check as well.
 *
 *   Revision 1.7  2000/04/20 18:28:34  mud
 *   Wrote 'tedit'.
 *
 *   Revision 1.6  2000/04/10 22:19:52  mud
 *   New Talent System
 *
 *   Revision 1.5  2000/04/07 15:04:07  mud
 *
 *
 *   The new Grant System
 *
 *   Revision 1.4  2000/03/18 01:37:08  mud
 *   Removed some code that Quinn had left to make himself immune to stuff like force, snoop, etc.
 *
 *   Revision 1.3  2000/03/02 12:54:00  mud
 *   Cosmetic changes to stat_room()
 *
 *   Revision 1.2  2000/02/21 14:00:15  mud
 *   CVS-logging headers added to all files.
 *
 m
 *****************************************************************************/

/* ************************************************************************
*   File: act.wizard.c                                  Part of CircleMUD *
*  Usage: Player-level god commands and other goodies                     *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */
#include "fight.h"
#include "class.h"
#include "config.h"
#include "conf.h"
#include "sysdep.h"
#include "constants.h"
#include "act.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "spells.h"
#include "screen.h"
#include "olc.h"
#include "hotboot.h"
#include "xmlcharsave.h"
#include "namedspec.h"
#include "xmlobjsave.h"
#include "dll.h"
#include "dg_scripts.h"

struct cnt_data {
  long items;
  long i_items;
  long tot_qps;
};

typedef struct cnt_data qcnt_t;

static void cnt_qeq(struct obj_data *obj,qcnt_t *cnt)
{
  struct obj_data *cont;

  if (GET_OBJ_QCOST(obj)) {
    cnt->items++;
    cnt->tot_qps += GET_OBJ_QCOST(obj);

    if (NULL == obj->obj_flags.boughtBy)
      cnt->i_items++;
  }

  for (cont = obj->contains; NULL != cont; cont = cont->next_content)
    cnt_qeq(cont,cnt);
}

static int cnt_qskills(struct char_data *ch) 
{
  int i, amount = 0; 

  for (i = 1; i <= MAX_SKILLS; i++) {
    if (strcasecmp(spells[i],"!UNUSED!") && (IS_SET(spell_info[i].type,SKILL_QUEST) || IS_SET(spell_info[i].type,SPELL_QUEST)) && GET_SKILL(ch,i))
      amount += spell_info[i].cost[0]; /* Shouldn't matter, quest skills cost are same for all classes! */
  }
  return amount;
}

static void disp_qeq(struct char_data *ch,struct obj_data *obj)
{
  struct obj_data *cont;

  if (GET_OBJ_QCOST(obj)) {
    if (obj->obj_flags.boughtBy) {
      asend_to_char(ch,"%s: cost %d QPs, bought by %s in (%d;%d) on %s\r\n",obj->short_description,GET_OBJ_QCOST(obj),
            obj->obj_flags.boughtBy, obj->obj_flags.boughtFrom[0],obj->obj_flags.boughtFrom[1],
            ctime(&(obj->obj_flags.boughtWhen)));
    } else {
      asend_to_char(ch,"%s: Object is QeQ(Cost %d QPs), but has no 'tracking info', \\c01MAY\\c00 be illegally obtained.\r\n",
            obj->short_description,GET_OBJ_QCOST(obj));
    }
  }
  for (cont = obj->contains; NULL != cont; cont = cont->next_content)
    disp_qeq(ch,cont);
}

/*
 * List  quest-items a character has, it's cost 
 */
ACMD(do_qlist) 
{
  int i = 0; 
  struct obj_data  *inv  = NULL;
  struct char_data *vict = NULL;

  skip_spaces(&argument);

  if (!argument || !*argument) {
    send_to_char("Usage: qcount <victim>\r\n",ch);
    return;
  }
  if (NULL == (vict = get_char_vis(ch,argument))) {
    send_to_char("That person isn't here!\r\n",ch);
    return;
  }
  if (IS_NPC(vict)) {
    send_to_char("Not on mobs!\r\n",ch);
    return;
  }

  for (i = 0; i < NUM_WEARS; i++) {
    if (NULL != GET_EQ(vict,i))
      disp_qeq(ch,GET_EQ(vict,i));
  }

  inv = vict->carrying;
  while (NULL != inv) {
    disp_qeq(ch,inv);
    inv = inv->next_content;
  }
}

void get_qs(struct char_data *ch, long *inskill, long *ingear)
{
  qcnt_t cntd = { 0,0,0 };
  int i; 
  struct obj_data *inv;

  if (!ch || !inskill || !ingear)
    return;

  (*inskill) = (long)cnt_qskills(ch);

  for (i = 0; i < NUM_WEARS; i++) {
    if (NULL != GET_EQ(ch, i))
      cnt_qeq(GET_EQ(ch,i),&cntd);
  }

  inv = ch->carrying;
  while (NULL != inv) {
    cnt_qeq(inv,&cntd);
    inv = inv->next_content;
  }  

  (*ingear) = cntd.tot_qps;
}

/*
 * Is supposed to count quest items on person (worn or in inventory), calculate total amount of qpoints, 
 * (and itmes without the 'proper' tracking info). 
 * Also displays QPS and qps in skills, of course. :)
 */
ACMD(do_qcount)
{
  struct char_data *vict;
  qcnt_t cntd = { 0,0,0 };
  int i; 
  struct obj_data *inv;

  skip_spaces(&argument);

  if (!argument || !*argument) {
    send_to_char("Usage: qcount <victim>\r\n",ch);
    return;
  }
  if (NULL == (vict = get_char_vis(ch,argument))) {
    send_to_char("That person isn't here!\r\n",ch);
    return;
  }
  if (IS_NPC(vict)) {
    send_to_char("Not on mobs!\r\n",ch);
    return;
  }

  for (i = 0; i < NUM_WEARS; i++) {
    if (NULL != GET_EQ(vict, i))
      cnt_qeq(GET_EQ(vict,i),&cntd);
  }

  inv = vict->carrying;
  while (NULL != inv) {
    cnt_qeq(inv,&cntd);
    inv = inv->next_content;
  }

  asend_to_char(ch,"%s has %d quest points, and %d quest points in skills \r\n",GET_RNAME(vict),GET_QP(vict),cnt_qskills(vict));
  asend_to_char(ch,"and is carrying %ld quest items (whereof %ld \\c01MAY\\c00 be illegally obtained) at\r\n"
           "total cost of %ld questpoints.\r\nThis sums up to %ld questpoints.\r\n",cntd.items, cntd.i_items,cntd.tot_qps,GET_QP(vict)+cnt_qskills(vict)+cntd.tot_qps);
}

ACMD(do_dig) 
{
  char buf2[100];
  char buf3[100];
  int iroom = 0, rroom = 0;
  int door = 0,z1 = 0,z2 = 0;
  struct room_data *room1,*room2; 
  struct room_direction_data *i1,*i2;

  two_arguments(argument, buf2, buf3); 

  if (!*buf2 || !*buf3) {
    asend_to_char(ch,"Format: dig <direction> <room vnum>\r\n"); 
    return;
  }
  iroom = atoi(buf3);
  rroom = real_room(iroom);

  if (NOWHERE == rroom) {
    asend_to_char(ch,"No room with vnum %d\r\n", iroom);
    return;
  }

  z2 = zone_table[real_zone(iroom)].number;
  z1 = zone_table[real_zone(world[ch->in_room].number)].number;

  if (!allowed_to_edit_room(ch,iroom) || !allowed_to_edit_room(ch,world[ch->in_room].number)) {
    send_to_char("You are not allowed to do any changes to this zone!\r\n",ch);
    return;
  }

  if (-1 == (door = search_block(buf2,dirs,FALSE))) {
    asend_to_char(ch,"%s is -\\c01NOT\\c00- a valid direction!\r\n",buf2);
    return;
  }
  room1 = &world[ch->in_room];
  room2 = &world[rroom];
 
  CREATE(i1, struct room_direction_data,1); 
  CREATE(i2, struct room_direction_data,1);

  if ((NULL == i1) || (NULL == i2)) {
    asend_to_char(ch,"Couldn't allocate memory for dig(), sorry!\r\n");
    alog("SYSERR: Not enough memory avaialbel in dig() for %s.\r\n",GET_NAME(ch));
    return;
  }

  if ((NULL != room1->dir_option[door]) || (NULL != room2->dir_option[rev_dir[door]])) {
    free(i1);
    free(i2);
    asend_to_char(ch,"You can't open an exit between these rooms! They're already taken!\r\n");
    return;
  }
  room1->dir_option[door] = i1;
  room2->dir_option[rev_dir[door]] = i2;
  i1->general_description = str_dup("None\r\n");
  i1->keyword = str_dup("Not set yet...\r\n");
  i1->to_room  = rroom;
  i2->general_description = str_dup("None\r\n");
  i2->keyword = str_dup("Not set yet...\r\n");
  i2->to_room  = ch->in_room;

  olc_add_to_save_list(z1,OLC_SAVE_ROOM);
  if (z1 != z2)
    olc_add_to_save_list(z2,OLC_SAVE_ROOM);

  asend_to_char(ch,"OK, exit between %d and %d dug!\r\n",world[ch->in_room].number,iroom);
}


ACMD(do_authcheck)
{
  struct descriptor_data *d;
  int found_level_one=0, found_mort = 0;
        
  for (d = descriptor_list; d; d = d->next)
      {
      if ( d->connected || !d->host || !d->character)
        continue;
      if (!IS_MOB(d->character)) {
      if(!GET_AUTHORIZED(d->character))
        {
        if ( GET_LEVEL(d->character)>1)
           {
           asend_to_char(ch,"\\c14%s\\c00's name is unauthorized at level %d.\r\n",GET_NAME(d->character), GET_LEVEL(d->character));
           found_mort++;
           }
        else
           {  
           asend_to_char(ch,"\\c01%s\\c00's name is unauthorized at level one.\r\n",GET_NAME(d->character));
           found_level_one++;
           }
        }
      }
     }

   if (found_level_one || found_mort)  
     {
     asend_to_char(ch, "\r\nFound: %d real newbies and %d older characters' matches.\r\n", found_level_one, found_mort);
     }
   else
     send_to_char("No one online has an unauthorized name.\r\n",ch);  

}

/*
 * Allowing of "\n" and similar thingies in gecho 
 * I.e "\n" -> \r\n :) 
 *     "\t" -> \t
 * Leaves colourcodes intact! :)
 */
static char *gecho_to_str(char *str)
{
  char *tmp = strdup(str);
  char *ret = tmp;

  while (*str) {
    if ('\\' != *str) {
      *tmp++ = *str++;
    } else {
      str++;
      switch(*str) {
    case 't':
          *tmp = '\t';
          break;
    case 'n': *tmp++ = '\r';
          *tmp = '\n';
          break;
    case 'c':
    case 'C': 
          *tmp++ = '\\';
          *tmp   = *str;
          break;
    default : // *tmp++ = '\\';
          *tmp = *str;
          break;
      }
      tmp++; 
      str++;
    }
  }
  *tmp = '\0';

  return ret;
}




ACMD(do_echo)
{
  struct char_data *people;
  int copy_emote_to = 0, i;

  skip_spaces(&argument); 
  if (!IS_NPC(ch) && (GET_LEVEL(ch) < LVL_IMMORT) && (subcmd != SCMD_EMOTE)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }

  if (!*argument)
    send_to_char("Yes.. but what?\r\n", ch);
  else if (subcmd != SCMD_EMOTE)
  {
    for (people = world[ch->in_room].people; people; people = people->next_in_room) {
      if (people == NULL)
        continue;

      if (PRF2_FLAGGED(people, PRF2_RPTAG))
        sprintf(buf, "[RP] %s\r\n", gecho_to_str(argument));
      else
        sprintf(buf, "%s\r\n", gecho_to_str(argument));
     
      send_to_char(coltext(buf, ch), people);
    }
    to_eavesdropper(coltext(buf, ch), ch->in_room);
  }
  else {
    if (!CMD_IS("lastemote")) {
      if (GET_LASTEMOTESTR(ch))
        free(GET_LASTEMOTESTR(ch));
      GET_LASTEMOTESTR(ch) = str_dup(coltext(argument, ch));
      GET_LASTEMOTETYPE(ch) = EMOTE_EMOTE;
    }
    for (people = world[ch->in_room].people; people; people = people->next_in_room) {
      if (people == NULL)
        continue;
      if (people == ch)
        continue;
      if (!SEE_DREAMING_OK(ch, people))
        continue;
      GET_LAST_EMOTE(ch)=0; 

      if(!check_and_notify_non_IC_receiver(ch, people, "does"))
      {
        if (*argument == '\'' && !PLR_FLAGGED(ch, PLR_MAILING | PLR_WRITING)) {
          if (PRF2_FLAGGED(people, PRF2_RPTAG))
            sprintf(buf,"[RP] %s%s", GET_NAME(ch), argument);
          else
            sprintf(buf,"%s%s",GET_NAME(ch),argument);

          act(coltext(buf, ch), TRUE, ch, 0, people, TO_VICT);
          sprintf(buf, "%s\r\n", coltext(buf, ch));
          add_to_rplog(people, coltext(buf, ch),ch);
        }
        else if ( !PLR_FLAGGED(ch, PLR_MAILING | PLR_WRITING)) {
          if (PRF2_FLAGGED(people, PRF2_RPTAG))
            sprintf(buf,"[RP] %s %s", GET_NAME(ch), argument);
          else
            sprintf(buf,"%s %s",GET_NAME(ch),argument);

          act(coltext(buf, ch), TRUE, ch, 0, people, TO_VICT);
          sprintf(buf, "%s\r\n", coltext(buf, ch));
          add_to_rplog(people, coltext(buf, ch),ch);
        }
      }
    }
     
    sprintf(buf, "%s %s\r\n", GET_NAME(ch), argument);
    to_eavesdropper(coltext(buf, ch), ch->in_room);

    if (IS_SET(PRF_FLAGS(ch), PRF_IC)) {
      for (i = 0; mirrored_rooms[i].from != -1; i++) {
    if (ch->in_room == real_room(mirrored_rooms[i].from) && (copy_emote_to = real_room(mirrored_rooms[i].to)) >= 0) {
      if (*argument == '\'' && !PLR_FLAGGED(ch, PLR_MAILING | PLR_WRITING)) {
        sprintf(buf, "%s%s\r\n", GET_NAME(ch), argument);
      }
      else if (!PLR_FLAGGED(ch, PLR_MAILING | PLR_WRITING)) {
        sprintf(buf, "%s %s\r\n", GET_NAME(ch), argument);
      }
      copy_emote_to_room(coltext(buf, ch), copy_emote_to, ch);
    }
      }
    }

    if (*argument == '\'' && !PLR_FLAGGED(ch, PLR_MAILING | PLR_WRITING)) {
      if (PRF2_FLAGGED(ch, PRF2_RPTAG))
        sprintf(buf,"[RP] %s%s\r\n", GET_NAME(ch), argument);
      else
        sprintf(buf,"%s%s\r\n",GET_NAME(ch),argument);
      send_to_char(coltext(buf, ch), ch);
      add_to_rplog(ch, coltext(buf, ch),ch);
    }
    else if(  !PLR_FLAGGED(ch, PLR_MAILING | PLR_WRITING)) {
      if (PRF2_FLAGGED(ch, PRF2_RPTAG))
        sprintf(buf,"[RP] %s %s\r\n", GET_NAME(ch), argument);
      else
        sprintf(buf,"%s %s\r\n",GET_NAME(ch),argument);
      send_to_char(coltext(buf, ch), ch);
      add_to_rplog(ch, coltext(buf, ch),ch);
    }
  }       
}


ACMD(do_send)
{
  struct char_data *vict;

  half_chop(argument, arg, buf);

  if (!*arg) {
    send_to_char("Send what to who?\r\n", ch);
    return;
  }
  if (!(vict = get_char_vis(ch, arg))) {
    send_to_char(NOPERSON, ch);
    return;
  }

/*
  if( PLR_FLAGGED( vict, PLR_WRITING ) ) {
    send_to_char("$E's writing a message right now; try again later.", ch);
    return;
  }
*/

  send_to_char(buf, vict);
  send_to_char("\r\n", vict);
  if (PRF_FLAGGED(ch, PRF_NOREPEAT))
    send_to_char("Sent.\r\n", ch);
  else {
    sprintf(buf2, "You send '%s' to %s.\r\n", buf, GET_NAME(vict));
    send_to_char(buf2, ch);
  }
}



/* take a string, and return an rnum.. used for goto, at, etc.  -je 4/6/93 */
sh_int find_target_room(struct char_data * ch, char *rawroomstr)
{
  int tmp;
  sh_int location;
  struct char_data *target_mob;
  struct obj_data *target_obj;
  char roomstr[MAX_INPUT_LENGTH];

  one_argument(rawroomstr, roomstr);

  if (!*roomstr) {
    send_to_char("You must supply a room number or name.\r\n", ch);
    return NOWHERE;
  }
  if (isdigit(*roomstr) && !strchr(roomstr, '.')) {
    tmp = atoi(roomstr);
    if ((location = real_room(tmp)) < 0) {
      send_to_char("No room exists with that number.\r\n", ch);
      return NOWHERE;
    }
  } else if ((target_mob = get_char_vis(ch, roomstr)))
    location = target_mob->in_room;
  else if ((target_obj = get_obj_vis(ch, roomstr))) {
    if (target_obj->in_room != NOWHERE)
      location = target_obj->in_room;
    else {
      send_to_char("That object is not available.\r\n", ch);
      return NOWHERE;
    }
  } else {
    send_to_char("No such creature or object around.\r\n", ch);
    return NOWHERE;
  }

  /* a location has been found -- if you're < NEWBIEGOD, check restrictions. */
  if (GET_LEVEL(ch) < 200) {
    if (ROOM_FLAGGED(location, ROOM_GODROOM)) {
      send_to_char("You are not godly enough to use that room!\r\n", ch);
      return NOWHERE;
    }
    if (ROOM_FLAGGED(location, ROOM_PRIVATE) &&
    world[location].people && world[location].people->next_in_room) {
      send_to_char("There's a private conversation going on in that room.\r\n", ch);
      return NOWHERE;
    }
  }
  return location;
}



ACMD(do_at)
{
  char command[MAX_INPUT_LENGTH];
  int location, original_loc;

  half_chop(argument, buf, command);
  if (!*buf) {
    send_to_char("You must supply a room number or a name.\r\n", ch);
    return;
  }

  if (!*command) {
    send_to_char("What do you want to do there?\r\n", ch);
    return;
  }

  if ((location = find_target_room(ch, buf)) < 0)
    return;

  if( ROOM_FLAGGED( location, ROOM_PRIVATE ) )
  {
    if( GET_REAL_LEVEL( ch ) < LVL_GRGOD )
    {
      if( number_of_players_in_room( location ) > 1 )
      {
    send_to_char( "That room is private right now.\r\n", ch );
    return;
      }
    }
  }

  /* a location has been found. */
  original_loc = ch->in_room;
  char_from_room(ch);
  char_to_room(ch, location);
  command_interpreter(ch, command);

  /* check if the char is still there */
  if (ch->in_room == location) {
    char_from_room(ch);
    char_to_room(ch, original_loc);
  }
}


ACMD(do_goto)
{
  sh_int location;

  if ((location = find_target_room(ch, argument)) < 0)
    return;

  if ((ROOM_FLAGGED(location, ROOM_TUNNEL)) && (number_of_players_in_room(location)))
  {
    send_to_char( "That room is private right now.\r\n", ch );
    return;
   }
  
  // PRIVATE room added by Purjo 970404
  if( ROOM_FLAGGED( location, ROOM_PRIVATE ) )
  {
    if( GET_REAL_LEVEL( ch ) < LVL_GRGOD )
    {
      if( number_of_players_in_room( location ) > 1 )
      {
    send_to_char( "That room is private right now.\r\n", ch );
    return;
      }
    }
  }
    

  if (POOFOUT(ch))
    sprintf(buf, "%s", POOFOUT(ch));
  else
    strcpy(buf, "$n disappears in a puff of smoke.");

  act(buf, TRUE, ch, 0, 0, TO_ROOM);
  GET_SEAT(ch) = NULL;
  char_from_room(ch);
  if (SESS_FLAGGED(ch, SESS_EAVESDROP)) {
    send_to_char("You move away from where you were and can no longer hear soft voices.\r\n", ch);
    (void)SESS_TOG_CHK(ch, SESS_EAVESDROP);
    rem_eavesdropper(ch, GET_EAVESDROPROOM(ch));
    GET_EAVESDROPROOM(ch) = 0;
  }
  char_to_room(ch, location);

  if (POOFIN(ch))
    sprintf(buf, "%s", POOFIN(ch));
  else
    strcpy(buf, "$n appears with an ear-splitting bang.");

  act(buf, TRUE, ch, 0, 0, TO_ROOM);
  GET_POS(ch) = POS_STANDING;
  look_at_room(ch, 0);
}



ACMD(do_trans)
{
  struct descriptor_data *i;
  struct char_data *victim, *master;
  int u = 0, countpeople=0;
  one_argument(argument, buf);

  if (!*buf)
    send_to_char("Whom do you wish to transfer?\r\n", ch);
  else 
  if (strcmp("all", buf) && strcmp("quest", buf)) {
    if (!(victim = get_char_vis(ch, buf)))
      send_to_char(NOPERSON, ch);
    else if (victim == ch)
      send_to_char("That doesn't make much sense, does it?\r\n", ch);
    else {
      if ((GET_LEVEL(ch) < GET_LEVEL(victim)) && !IS_NPC(victim)) {
    send_to_char("Go transfer someone your own size.\r\n", ch);
    return;
      }
      GET_POS(victim) = POS_STANDING;
      GET_SEAT(victim) = NULL;
      act("$n disappears in a mushroom cloud.", FALSE, victim, 0, 0, TO_ROOM);
      char_from_room(victim);
      if (SESS_FLAGGED(victim, SESS_EAVESDROP)) {
        send_to_char("You move away from where you were and can no longer hear soft voices.\r\n", victim);
        (void)SESS_TOG_CHK(victim, SESS_EAVESDROP);
        rem_eavesdropper(victim, GET_EAVESDROPROOM(victim));
        GET_EAVESDROPROOM(victim) = 0;
      }
      char_to_room(victim, ch->in_room);
      alog("(GC) %s transferred %s to [%d].", GET_RNAME(ch), GET_RNAME(victim), world[ch->in_room].number);
      act("$n arrives from a puff of smoke.", FALSE, victim, 0, 0, TO_ROOM);
      act("$n has transferred you!", FALSE, ch, 0, victim, TO_VICT);
      TICKS_IN_ROOM(ch)=0;
      look_at_room(victim, 0);
      /* A linking check so that the one transed gets away from the link */
      if ((victim->player_specials->linking[0]) || (victim->player_specials->linked_by))
      {
        master = (victim->player_specials->linked_by? victim->player_specials->linked_by : victim);
        while(master->player_specials->linking[u])
        {
          perform_unlink(master);
          ++u;
        }
      }
    }
  }
  else
  if (!strcmp("quest", buf))
  {                     /* Trans Quest */
        
    for (i = descriptor_list; i; i = i->next)
      if (!i->connected && i->character && i->character != ch && PRF_FLAGGED(i->character, PRF_QUEST))
        {
        countpeople++;
        victim = i->character;
        if (GET_LEVEL(victim) >= GET_LEVEL(ch))
          continue;
        act("$n disappears in a mushroom cloud.", FALSE, victim, 0, 0, TO_ROOM);
        char_from_room(victim);
    GET_POS(victim) = POS_STANDING;
    GET_SEAT(victim) = NULL;
        char_to_room(victim, ch->in_room);
        act("$n arrives from a puff of smoke.", FALSE, victim, 0, 0, TO_ROOM);
        act("$n has transferred you!", FALSE, ch, 0, victim, TO_VICT);
        look_at_room(victim, 0);
        }
    if(countpeople>0)
       send_to_char(OK, ch);
    else
       send_to_char("But there are no mortals Questing!\n\r",ch); 
}
  else 
  {			/* Trans All */
    if (GET_LEVEL(ch) < LVL_GRGOD) {
      send_to_char("I think not.\r\n", ch);
      return;
    }

    for (i = descriptor_list; i; i = i->next)
      if (!i->connected && i->character && i->character != ch) {
    victim = i->character;
    if (GET_LEVEL(victim) >= GET_LEVEL(ch))
      continue;
    act("$n disappears in a mushroom cloud.", FALSE, victim, 0, 0, TO_ROOM);
    char_from_room(victim);
        GET_POS(victim) = POS_STANDING;
    GET_SEAT(victim) = NULL;
    char_to_room(victim, ch->in_room);
    act("$n arrives from a puff of smoke.", FALSE, victim, 0, 0, TO_ROOM);
    act("$n has transferred you!", FALSE, ch, 0, victim, TO_VICT);
    look_at_room(victim, 0);
      }
    send_to_char(OK, ch);
  }


}



ACMD(do_teleport)
{
  struct char_data *victim;
  sh_int target;

  two_arguments(argument, buf, buf2);

  if( ! *buf ) {
    send_to_char( "Whom do you wish to teleport?\r\n", ch );
    return;
  }

  if( ! ( victim = get_char_vis(ch, buf ) ) ) {
    send_to_char( NOPERSON, ch );
    return;
  }

  if( victim == ch ) {
    send_to_char( "Use 'goto' to teleport yourself.\r\n", ch );
    return;
  }

  if( GET_LEVEL( victim ) >= GET_LEVEL(ch) && GET_LEVEL( ch ) < LVL_IMPL ) {
    send_to_char( "Maybe you shouldn't do that.\r\n", ch );
    return;
  }

  if( ! *buf2 ) {
    send_to_char( "Where do you wish to send this person?\r\n", ch );
    return;
  }

  if( ( target = find_target_room( ch, buf2 ) ) < 0 ) {
    send_to_char( "There is no such location!\r\n", ch );
    return;
  }

  if( ROOM_FLAGGED( target, ROOM_PRIVATE )
      && GET_LEVEL( ch ) < LVL_GRGOD 
      && number_of_players_in_room( target ) > 1 ) {
    send_to_char( "That room is private right now!\r\n", ch );
    return;
  }

  
  send_to_char(OK, ch);
  GET_POS(victim) = POS_STANDING;
  GET_SEAT(victim) = NULL;
  act("$n disappears in a puff of smoke.", FALSE, victim, 0, 0, TO_ROOM);
  char_from_room(victim);
      if (SESS_FLAGGED(victim, SESS_EAVESDROP)) {
        send_to_char("You move away from where you were and can no longer hear soft voices.\r\n", victim);
        (void)SESS_TOG_CHK(victim, SESS_EAVESDROP);
        rem_eavesdropper(victim, GET_EAVESDROPROOM(victim));
        GET_EAVESDROPROOM(victim) = 0;
      }

  char_to_room(victim, target);
  alog("(GC) %s teleported %s to [%d].", GET_RNAME(ch), GET_RNAME(victim), world[target].number);
  act("$n arrives from a puff of smoke.", FALSE, victim, 0, 0, TO_ROOM);
  act("$n has teleported you!", FALSE, ch, 0, (char *) victim, TO_VICT);
  look_at_room(victim, 0);
  
}

ACMD(do_qtrans)
{
  struct descriptor_data  *list = descriptor_list;

  for (;list;list = list->next) {
    if ((list->character != ch) && (PRF_FLAGGED(list->character,PRF_QUEST))) {
      act("$n dissappears in a mushroom cloud!",TRUE,list->character,0,0,TO_ROOM);
      char_from_room(list->character);
      char_to_room(list->character,ch->in_room);
      act("$n appears in a puff of smoke.",TRUE,list->character,0,0,TO_ROOM);
      act("You have been transfered by $n!",TRUE,ch,0,list->character,TO_VICT);
    }
  }
}

ACMD(do_qrestore)
{
  struct descriptor_data *list = descriptor_list;

  for (;list;list = list->next) {
    if ((list->character != ch) && (GET_LEVEL(list->character) <LVL_IMMORT) && PRF_FLAGGED(list->character,PRF_QUEST)) {
      SET_MANA(list->character, GET_MAX_MANA(list->character));
      GET_MOVE(list->character) = GET_MAX_MOVE(list->character);
      GET_FRACT_MOVE(list->character) = 0;
      GET_HIT(list->character)  = GET_MAX_HIT(list->character);
      act("You have been fully healed by $n!",TRUE,ch,0,list->character,TO_VICT);
    }
  }
}


ACMD(do_vnum)
{
  char *buf3;
  const char *usage = "Usage: vnum { obj | mob | room } <name>\r\n"
                      "       vnum { obj | mob | room } -f <flag>\r\n";
   
  buf3 = two_arguments(argument, buf, buf2);

  if( ! *buf
      || ! *buf2 
      || ( ! is_abbrev( buf, "mob" )
       && ! is_abbrev( buf, "obj" )
       && ! is_abbrev( buf, "room" ) ) )
  {
    send_to_char(usage, ch);
    return;
  }

  if (*buf2 == '-')
  {
    if (*(buf2 + 1) == 'f' && *buf3 && *buf3 != (char) NULL)
    {
      if (is_abbrev(buf, "mob"))
        do_vnum_flags(ch, 0, buf3);
      if (is_abbrev(buf, "obj"))   
        do_vnum_flags(ch, 1, buf3);
      if (is_abbrev(buf, "room"))  
        do_vnum_flags(ch, 2, buf3);
    } else {
      send_to_char(usage, ch);
    }
    return;
  }

  if (is_abbrev(buf, "mob"))
    if (!vnum_mobile(buf2, ch))
      send_to_char("No mobiles by that name.\r\n", ch);

  if (is_abbrev(buf, "obj"))
    if (!vnum_object(buf2, ch))
      send_to_char("No objects by that name.\r\n", ch);

  if( is_abbrev( buf, "room" ) )
    if( ! vnum_room( buf2, ch ) )
      send_to_char( "No room by that name.\r\n", ch );
}



void do_stat_room(struct char_data * ch)
{
  struct extra_descr_data *desc;
  struct room_data *rm = &world[ch->in_room];
  struct affected_type *list;
  int i, found = 0;
  struct obj_data *j = 0;
  struct char_data *k = 0;
  struct eavesdrop_info *eavesdropper;

  sprintf(buf, "Room name: %s%s%s\r\n", CCCYN(ch, C_NRM), rm->name,
      CCNRM(ch, C_NRM));
  send_to_char(buf, ch);

  sprinttype(rm->sector_type, sector_types, buf1);
  sprintf(buf, "Zone: [%3d], VNum: [%s%5d%s], RNum: [%5d], Type: %s\r\n",
      rm->zone, CCGRN(ch, C_NRM), rm->number, CCNRM(ch, C_NRM), ch->in_room, buf2);
  send_to_char(buf, ch);

  sprintbit((long) rm->room_flags, room_bits, buf2);
  sprintf(buf, "SpecProc: %s, Flags: %s\r\n",
      (rm->func == NULL) ? "None" : (
                     (NULL == getSpecialPointer(rm->func) ? 
                      "Unknown" : 
                      (char *)getSpecialPointer(rm->func))),
      buf2);

  send_to_char(buf, ch);

  sprintbit((long) rm->temp_flags, room_bits, buf2);
  sprintf(buf, "Temporary Flags: %s\r\n", buf2);
  send_to_char(buf, ch);

  send_to_char("Description:\r\n", ch);
  if (rm->description)
    send_to_char(rm->description, ch);
  else
    send_to_char("  None.\r\n", ch);

  if (rm->ex_description) {
    sprintf(buf, "Extra descs:%s", CCCYN(ch, C_NRM));
    for (desc = rm->ex_description; desc; desc = desc->next) {
      strcat(buf, " ");
      strcat(buf, desc->keyword);
    }
    strcat(buf, CCNRM(ch, C_NRM));
    send_to_char(strcat(buf, "\r\n"), ch);
  }
  sprintf(buf, "Chars present:%s", CCYEL(ch, C_NRM));
  for (found = 0, k = rm->people; k; k = k->next_in_room) {
    if (!CAN_SEE(ch, k))
      continue;
    sprintf(buf2, "%s %s(%s)", found++ ? "," : "", GET_NAME(k),
        (!IS_NPC(k) ? "PC" : (!IS_MOB(k) ? "NPC" : "MOB")));
    strcat(buf, buf2);
    if (strlen(buf) >= 62) {
      if (k->next_in_room)
    send_to_char(strcat(buf, ",\r\n"), ch);
      else
    send_to_char(strcat(buf, "\r\n"), ch);
      *buf = found = 0;
    }
  }

  if (*buf)
    send_to_char(strcat(buf, "\r\n"), ch);
  send_to_char(CCNRM(ch, C_NRM), ch);

  /* Eavesdropping List */

  sprintf(buf, "&wEavesdroppers: &y");
  if (rm->eaves) {
    for (eavesdropper=world[ch->in_room].eaves; eavesdropper; eavesdropper=eavesdropper->next)
      if (eavesdropper->charname)
        sprintf(buf, "%s%s&w(&y%d&y)&n, &y", buf, eavesdropper->charname, eavesdropper->distance);
  } else {
    sprintf(buf, "%s&yNONE&n", buf);
  }
  send_to_char(strcat(buf, "&n\r\n"), ch);

  /* End Eavesdropping List */

  if (rm->contents) {
    sprintf(buf, "Contents:%s", CCGRN(ch, C_NRM));
    for (found = 0, j = rm->contents; j; j = j->next_content) {
      if (!CAN_SEE_OBJ(ch, j))
    continue;
      sprintf(buf2, "%s %s", found++ ? "," : "", j->short_description);
      strcat(buf, buf2);
      if (strlen(buf) >= 62) {
    if (j->next_content)
      send_to_char(strcat(buf, ",\r\n"), ch);
    else
      send_to_char(strcat(buf, "\r\n"), ch);
    *buf = found = 0;
      }
    }

    if (*buf)
      send_to_char(strcat(buf, "\r\n"), ch);
    send_to_char(CCNRM(ch, C_NRM), ch);
  }
  for (i = 0; i < NUM_OF_DIRS; i++) {
    if (rm->dir_option[i]) {
      if (rm->dir_option[i]->to_room == NOWHERE)
    sprintf(buf1, " %sNONE%s", CCCYN(ch, C_NRM), CCNRM(ch, C_NRM));
      else
    sprintf(buf1, "%s%5d%s", CCCYN(ch, C_NRM),
        world[rm->dir_option[i]->to_room].number, CCNRM(ch, C_NRM));
      sprintbit(rm->dir_option[i]->exit_info, exit_bits, buf2);
      sprintf(buf, "Exit %s%-5s%s:  To: [%s], Key: [%5d], Keywrd: %s, Type: %s\r\n ",
          CCCYN(ch, C_NRM), dirs[i], CCNRM(ch, C_NRM), buf1, rm->dir_option[i]->key,
       rm->dir_option[i]->keyword ? rm->dir_option[i]->keyword : "None",
          buf2);
      send_to_char(buf, ch);
      if (rm->dir_option[i]->general_description)
    strcpy(buf, rm->dir_option[i]->general_description);
      else
    strcpy(buf, "  No exit description.\r\n");
      send_to_char(buf, ch);
    }
  }
  if (rm->aff) {
    *buf = '\0';
    for (list = rm->aff; list != NULL; list = list->next) {
      sprintf(buf,"%sSPL: (%3d hrs) %-21s\r\n",buf,list->duration,spells[list->type]);
    }   
    send_to_char(buf,ch);
  }
  if (NULL != rm->skimmer) {
    sprintf(buf,"\r\nIs a skim room:\r\nTravelling to: %d, ETA: %d\r\n",rm->travel_to,rm->eta);
    send_to_char(buf,ch);
  }

  /* check the room for a script */
  if (SCRIPT(&world[ch->in_room]))
   list_triggers_to_char(ch, SCRIPT(&world[ch->in_room]));
}



void do_stat_object(struct char_data * ch, struct obj_data * j)
{
  int i, virtual, found;
  struct obj_data *j2;
  struct extra_descr_data *desc;

  virtual = GET_OBJ_VNUM(j);
  sprintf(buf, "Name: '%s%s%s', Aliases: %s\r\n", CCYEL(ch, C_NRM),
      ((j->short_description) ? j->short_description : "<None>"),
      CCNRM(ch, C_NRM), j->name);
  send_to_char(buf, ch);
  sprinttype(GET_OBJ_TYPE(j), item_types, buf1);
  if (GET_OBJ_RNUM(j) >= 0)
    strcpy(buf2, (obj_index[GET_OBJ_RNUM(j)].func ? "Exists" : "None"));
  else
    strcpy(buf2, "None");
  sprintf(buf, "VNum: [%s%5d%s], RNum: [%5d], Type: %s, SpecProc: %s\r\n",
   CCGRN(ch, C_NRM), virtual, CCNRM(ch, C_NRM), GET_OBJ_RNUM(j), buf1, buf2);
  send_to_char(buf, ch);
  if ((GET_OBJ_TYPE(j) == ITEM_WEAPON) || (GET_OBJ_TYPE(j) == ITEM_ARMOR)) { 
    if (GET_OBJ_QCOST(j) || IS_SET(GET_OBJ_STATUS(j), ITEM_NOBREAK))
      sprintf(buf, "Wear status: Infinite \r\n");
    else
      sprintf(buf, "Wear status: %d %%\r\n", GET_OBJ_STATUS(j));
    send_to_char(buf, ch);  
  }
sprintf(buf, "L-Des: %s\r\n", ((j->description) ? j->description : "None"));
  send_to_char(buf, ch);

  if (j->ex_description) {
    sprintf(buf, "Extra descs:%s", CCCYN(ch, C_NRM));
    for (desc = j->ex_description; desc; desc = desc->next) {
      strcat(buf, " ");
      strcat(buf, desc->keyword);
    }
    strcat(buf, CCNRM(ch, C_NRM));
    send_to_char(strcat(buf, "\r\n"), ch);
  }
  send_to_char("Can be worn on: ", ch);
  sprintbit(j->obj_flags.wear_flags, wear_bits, buf);
  strcat(buf, "\r\n");
  send_to_char(buf, ch);

  send_to_char("Set char bits : ", ch);
  sprintbit(j->obj_flags.bitvector, affected_bits, buf);
  strcat(buf, "\r\n");
  send_to_char(buf, ch);

  send_to_char("Extra flags   : ", ch);
  sprintbit(GET_OBJ_EXTRA(j), extra_bits, buf);
  strcat(buf, "\r\n");
  send_to_char(buf, ch);

  sprintf(buf, "Weight: %ld, Value: %ld, Cost in QP: %ld, Timer: %ld\r\n",
     GET_OBJ_WEIGHT(j), GET_OBJ_COST(j), GET_OBJ_QCOST(j), GET_OBJ_TIMER(j));
  send_to_char(buf, ch);

  strcpy(buf, "In room: ");
  if (j->in_room == NOWHERE)
    strcat(buf, "Nowhere");
  else {
    sprintf(buf2, "%d", world[j->in_room].number);
    strcat(buf, buf2);
  }
  strcat(buf, ", In object: ");
  strcat(buf, j->in_obj ? j->in_obj->short_description : "None");
  strcat(buf, ", Carried by: ");
  strcat(buf, j->carried_by ? GET_NAME(j->carried_by) : "Nobody");
  strcat(buf, ", Worn by: ");
  strcat(buf, j->worn_by ? GET_NAME(j->worn_by) : "Nobody");
  strcat(buf, "\r\n");
  send_to_char(buf, ch);

  switch (GET_OBJ_TYPE(j)) {
  case ITEM_LIGHT:
    if (GET_OBJ_VAL(j, 2) == -1)
      strcpy(buf, "Hours left: Infinite");
    else
      sprintf(buf, "Hours left: [%ld]", GET_OBJ_VAL(j, 2));
    break;
  case ITEM_POTION:
    sprintf(buf, "A potion, much yet to implement here!");
    break;
  case ITEM_WEAPON:
    sprintf(buf, "Todam: %ldd%ld, Message type: %ld",
        GET_OBJ_VAL(j, 1), GET_OBJ_VAL(j, 2), GET_OBJ_VAL(j, 3));
    break;
  case ITEM_ARMOR:
    sprintf(buf, "AC-apply: [%ld]", GET_OBJ_VAL(j, 0));
    break;
  case ITEM_CONTAINER:
    sprintbit(GET_OBJ_VAL(j, 1), container_bits, buf2);
    sprintf(buf, "Weight capacity: %ld, Lock Type: %s, Key Num: %ld, Corpse: %s",
        GET_OBJ_VAL(j, 0), buf2, GET_OBJ_VAL(j, 2),
        YESNO(GET_OBJ_VAL(j, 3)));
    break;
  case ITEM_DRINKCON:
  case ITEM_FOUNTAIN:
    sprinttype(GET_OBJ_VAL(j, 2), drinks, buf2);
    sprintf(buf, "Capacity: %ld, Contains: %ld, Poisoned: %s, Liquid: %s",
        GET_OBJ_VAL(j, 0), GET_OBJ_VAL(j, 1), YESNO(GET_OBJ_VAL(j, 3)),
        buf2);
    break;
  case ITEM_NOTE:
    sprintf(buf, "Tongue: %ld", GET_OBJ_VAL(j, 0));
    break;
  case ITEM_KEY:
    strcpy(buf, "");
    break;
  case ITEM_FOOD:
    sprintf(buf, "Makes full: %ld, Poisoned: %s", GET_OBJ_VAL(j, 0),
        YESNO(GET_OBJ_VAL(j, 3)));
    break;
  case ITEM_MONEY:
    sprintf(buf, "Coins: %ld", GET_OBJ_VAL(j, 0));
    break;
  default:
    sprintf(buf, "Values 0-3: [%ld] [%ld] [%ld] [%ld]",
        GET_OBJ_VAL(j, 0), GET_OBJ_VAL(j, 1),
        GET_OBJ_VAL(j, 2), GET_OBJ_VAL(j, 3));
    break;
  }
  send_to_char(strcat(buf, "\r\n"), ch);

  if (GET_OBJ_QCOST(j)) {
    send_to_char("\r\nObject is a Quest item\r\n",ch);
    if (NULL == j->obj_flags.boughtBy) {
      send_to_char("No buying info on quest item, item \\c01MIGHT\\c00 be illegaly obtained!\r\n\r\n",ch);
    } else {
      sprintf(buf,"Object wqas bought by %s, in store with vnum %ld, (room vnum %ld), at time %s\r\n",
          j->obj_flags.boughtBy,j->obj_flags.boughtFrom[0],j->obj_flags.boughtFrom[1],ctime(&j->obj_flags.boughtWhen));
      send_to_char(buf,ch);
    }
  }

  /*
   * I deleted the "equipment status" code from here because it seemed
   * more or less useless and just takes up valuable screen space.
   */

  if (j->contains) {
    sprintf(buf, "\r\nContents:%s", CCGRN(ch, C_NRM));
    for (found = 0, j2 = j->contains; j2; j2 = j2->next_content) {
      sprintf(buf2, "%s %s", found++ ? "," : "", j2->short_description);
      strcat(buf, buf2);
      if (strlen(buf) >= 62) {
    if (j2->next_content)
      send_to_char(strcat(buf, ",\r\n"), ch);
    else
      send_to_char(strcat(buf, "\r\n"), ch);
    *buf = found = 0;
      }
    }

    if (*buf)
      send_to_char(strcat(buf, "\r\n"), ch);
    send_to_char(CCNRM(ch, C_NRM), ch);
  }
  found = 0;
  send_to_char("Affections:", ch);
  for (i = 0; i < MAX_OBJ_AFFECT; i++)
    if (j->affected[i].modifier) {
      sprinttype(j->affected[i].location, apply_types, buf2);
      sprintf(buf, "%s %+ld to %s", found++ ? "," : "",
          j->affected[i].modifier, buf2);
      send_to_char(buf, ch);
    }
  if (!found)
    send_to_char(" None", ch);

  send_to_char("\r\n", ch);

  /* check the object for a script */
  if (SCRIPT(j))
   list_triggers_to_char(ch, SCRIPT(j));
}


void do_stat_character(struct char_data * ch, struct char_data * k)
{
  int i, i2, found = 0;
  struct obj_data *j;
  struct follow_type *fol;

  switch (GET_SEX(k)) {
  case SEX_NEUTRAL:    strcpy(buf, "NEUTRAL-SEX");   break;
  case SEX_MALE:       strcpy(buf, "MALE");          break;
  case SEX_FEMALE:     strcpy(buf, "FEMALE");        break;
  default:             strcpy(buf, "ILLEGAL-SEX!!"); break;
  }

  if (GET_DISGUISED(k))
    sprintf(buf2, " %s '%s', disguised as: %s. IDNum: [%5ld], In room [%5d]\r\n",
        (!IS_NPC(k) ? "PC" : (!IS_MOB(k) ? "NPC" : "MOB")),
        GET_RNAME(k),GET_NAME(k), GET_IDNUM(k), (NOWHERE == k->in_room ? -1 : world[k->in_room].number));
  else
    sprintf(buf2, " %s '%s', IDNum: [%5ld], In room [%5d] Pfilepos [%5ld]\r\n",
        (!IS_NPC(k) ? "PC" : (!IS_MOB(k) ? "NPC" : "MOB")),
        GET_NAME(k), GET_IDNUM(k), (NOWHERE == k->in_room ? -1 : world[k->in_room].number),
        GET_PFILEPOS(k));

  send_to_char(strcat(buf, buf2), ch);
  
  if (IS_MOB(k)) {
    sprintf(buf, "Alias: %s, VNum: [%5ld], RNum: [%5d]\r\n",
        k->player.name, GET_MOB_VNUM(k), GET_MOB_RNUM(k));
    send_to_char(buf, ch);
  }
  sprintf(buf, "Title: %s\r\n", (k->player.title ? k->player.title : "<None>"));
  send_to_char(buf, ch);

  sprintf(buf, "L-Des: %s", (k->player.long_descr ? k->player.long_descr : "<None> "));
  send_to_char(buf, ch);

  if (GET_NOBILITY(k)) {
     sprintf(buf,"Nobility: &r%s&n\r\n", GET_NOBILITY(k));
     send_to_char(buf, ch);
  }
  else {
     sprintf(buf,"\r\n");
     send_to_char(buf,ch);
  }

  if (IS_NPC(k)) {
    strcpy(buf, "Monster Class: ");
    sprinttype(k->player.class, npc_class_types, buf2);
  } else {
    strcpy(buf, "Class: ");
    sprinttype(k->player.class, pc_class_types, buf2);
  }
  strcat(buf, buf2);

  sprintf(buf2, ", Lev: [%s%3ld%s], XP: [%s%9ld%s]\r\n",
      CCYEL(ch, C_NRM), GET_LEVEL(k), CCNRM(ch, C_NRM),
      CCYEL(ch, C_NRM), GET_EXP(k), CCNRM(ch, C_NRM));
  strcat(buf, buf2);
  send_to_char(buf, ch);

  if (!IS_NPC(k)) {
    strcpy(buf1, (char *) asctime(localtime(&(k->player.time.birth))));
    strcpy(buf2, (char *) asctime(localtime(&(k->player.time.logon))));
    buf1[10] = buf2[10] = '\0';

    sprintf(buf, "Created: [%s], Last Logon: [%s], Played [%ldh %ldm], Age [%d]",
        buf1, buf2, k->player.time.played / 3600,
        ((k->player.time.played / 3600) % 60), age(k).year);

    /*. Display OLC zone for immorts .*/
    if(GET_LEVEL(k) >= LVL_IMMORT)
      sprintf(buf, "%s, OLC[%ld]", buf, GET_OLC_ZONE(k));
    strcat(buf, "\r\n");
    send_to_char(buf, ch);
  }
  sprintf(buf, "Str: [%s%d%s]  Int: [%s%d%s]  Wis: [%s%d%s]  "
      "Dex: [%s%d%s]  Con: [%s%d%s]  Cha: [%s%d%s]\r\n",
      CCCYN(ch, C_NRM), GET_STR(k), CCNRM(ch, C_NRM),
      CCCYN(ch, C_NRM), GET_INT(k), CCNRM(ch, C_NRM),
      CCCYN(ch, C_NRM), GET_WIS(k), CCNRM(ch, C_NRM),
      CCCYN(ch, C_NRM), GET_DEX(k), CCNRM(ch, C_NRM),
      CCCYN(ch, C_NRM), GET_CON(k), CCNRM(ch, C_NRM),
      CCCYN(ch, C_NRM), GET_CHA(k), CCNRM(ch, C_NRM));
  send_to_char(buf, ch);

  sprintf(buf, "Hit p.:[%s%4d/%4d+%3d%s]  Mana p.:[%s%4ld/%4ld+%3d%s] Move p.:[%s%4d/%4d+%3d%s] \r\nFract. Move: [%s%4d%s]\r\n",
      CCGRN(ch, C_NRM), GET_HIT(k), GET_MAX_HIT(k), hit_gain(k), CCNRM(ch, C_NRM),
      CCGRN(ch, C_NRM), GET_MANA(k), GET_MAX_MANA(k), mana_gain(k), CCNRM(ch, C_NRM),
      CCGRN(ch, C_NRM), GET_MOVE(k), GET_MAX_MOVE(k), move_gain(k), CCNRM(ch, C_NRM),
          CCGRN(ch, C_NRM), GET_FRACT_MOVE(k), CCNRM(ch, C_NRM));
  send_to_char(buf, ch);

  sprintf(buf, "Coins: [%9ld], Bank: [%9ld], QPs: [%11ld]\r\n",
      GET_GOLD(k), GET_BANK_GOLD(k), GET_QP(k));
  send_to_char(buf, ch);

  sprintf(buf, "AC: [%d/10]  Abs:[%.1f]  Hitroll: [%5ld], Damroll: [%5ld]\r\n",GET_AC(k), IS_NPC(k)?0 : (GET_ABS(k)/10.0), k->points.hitroll, k->points.damroll);
  send_to_char(buf, ch);
  if (GET_SPARK(k)) 
    send_to_char("Is born with the Spark\r\n",ch);
  if (GET_TEACHABLE(k))
    send_to_char("Is teachable.\r\n",ch);
  if (GET_WOLFKIN(k))
    send_to_char("Is born as a Wolfkin.\r\n",ch);
  if (GET_TAVEREN(k))
    send_to_char("Is Ta'veren.\r\n",ch);
  if (!cant_channel(k) && IS_SET(GET_PERMANENT(k), PRM_GENTLED))
    send_to_char("Is burnt out.\r\n",ch);
  
  sprintf(buf,"Air: [%3d], Water: [%3d], Spirit: [%3d], Fire: [%3d], Earth: [%3d]\r\n",GET_AIR(k),GET_WATER(k),
                GET_SPIRIT(k), GET_FIRE(k), GET_EARTH(k));
  send_to_char(buf,ch);		
  
  if (!cant_channel(k) && (SEX_MALE == GET_SEX(k))) {
    sprintf(buf,"Taint: %08lx\r\n",GET_TAINT(k));
    send_to_char(buf,ch);
  }

  if (GET_OGIER(k)) {
    asend_to_char(ch,"Longing: %d\r\n", GET_LONGING(k));
  }
        
  sprinttype(GET_POS(k), position_types, buf2);
  sprintf(buf, "Pos: %s, Fighting: %s", buf2,
      (FIGHTING(k) ? GET_NAME(FIGHTING(k)) : "Nobody"));

  if (IS_NPC(k)) {
    strcat(buf, ", Attack type: ");
    strcat(buf, attack_hit_text[k->mob_specials.attack_type].singular);
  }
  if (k->desc) {
    sprinttype(k->desc->connected, connected_types, buf2);
    strcat(buf, ", Connected: ");
    strcat(buf, buf2);
  }
  send_to_char(strcat(buf, "\r\n"), ch);

  strcpy(buf, "Default position: ");
  sprinttype((k->mob_specials.default_pos), position_types, buf2);
  strcat(buf, buf2);

  sprintf(buf2, ", Idle Timer (in tics) [%ld]\r\n", k->char_specials.timer);
  strcat(buf, buf2);
  send_to_char(buf, ch);

  if (IS_NPC(k)) {
    sprintbit(MOB_FLAGS(k), action_bits, buf2);
    sprintf(buf, "NPC flags: %s%s%s\r\n", CCCYN(ch, C_NRM), buf2, CCNRM(ch, C_NRM));
    send_to_char(buf, ch);
  } else {
    sprintbit(PLR_FLAGS(k), player_bits, buf2);
    sprintf(buf, "PLR: %s%s%s\r\n", CCCYN(ch, C_NRM), buf2, CCNRM(ch, C_NRM));
    send_to_char(buf, ch);
    sprintbit(PRF_FLAGS(k), preference_bits, buf2);
    sprintf(buf, "PRF: %s%s%s\r\n", CCGRN(ch, C_NRM), buf2, CCNRM(ch, C_NRM));
    send_to_char(buf, ch);
    sprintbit(PRF2_FLAGS(k), preference2_bits, buf2);
    sprintf(buf, "PRF2: %s%s%s\r\n", CCGRN(ch, C_NRM), buf2, CCNRM(ch, C_NRM));
    send_to_char(buf, ch);
    sprintbit(GET_PERMANENT(k), permanent_bits, buf2);
    sprintf(buf,"PERM: %s%s%s\r\n",CCGRN(ch, C_NRM), buf2, CCNRM(ch, C_NRM));
    send_to_char(buf, ch);
    buf[0] = '\0';
    /* A quick hack for Command Groups */

    if ((k)->player_specials->saved.command_groups) {
      sprintbit((k)->player_specials->saved.command_groups, cg_bits, buf2);
      sprintf(buf, "CGs: %s\r\n", buf2);
      send_to_char(buf, ch);
      buf[0] = '\0';
    }

   /* A quick hack for OLD Talents system!!! */
   if ((k)->player_specials->saved.talents) {
      sprintf(buf2, "**OLD**Talents: ");   	
      /*get list of talents*/
    for (i = 0; i < 26; i++)
        if (IS_SET((k)->player_specials->saved.talents, (1 << i)))
        sprintf(buf2, "%s %s", buf2, master_talent_list[i].name);
      /*print to char*/
      sprintf(buf2, "%s\r\n", buf2);
      send_to_char(buf2, ch);
   }

   
   /*display Primary/Secondary talents and special skills*/
   /*Primary talents*/
   if ((GET_PRIMTALENTS1(k)) || (GET_PRIMTALENTS2(k))) {
    sprintf(buf2, "Primary Talents: ");
    /*get list of talents*/
    for (i = 0; i < NUM_TALENTS; i++)
        if (IS_TALENT_SET(GET_PRIMTALENTS1(k), GET_PRIMTALENTS2(k), i))
        sprintf(buf2, "%s %s", buf2, master_talent_list[i].name);

        sprintf(buf, "%s\r\n", buf2);

    }
    /*secondary talents*/
    if ((GET_SECTALENTS1(k)) || (GET_SECTALENTS2(k))) {
        sprintf(buf2, "Secondary Talents: ");
        /*get list of talents*/
        for (i = 0; i < NUM_TALENTS; i++)
                if (IS_TALENT_SET(GET_SECTALENTS1(k), GET_SECTALENTS2(k),  i))
                    sprintf(buf2, "%s %s", buf2, master_talent_list[i].name);

        sprintf(buf, "%s%s\r\n", buf, buf2);

    }         
    /*Special Skills*/
    if ((GET_SPSKILLS1(k)) || (GET_SPSKILLS2(k))) {
        sprintf(buf2, "Special Skills: ");
        /*get list of talents*/
        for (i = 0; i < NUM_TALENTS; i++)
                if (IS_TALENT_SET(GET_SPSKILLS1(k), GET_SPSKILLS2(k), i))
                    sprintf(buf2, "%s %s", buf2, master_talent_list[i].name);

        sprintf(buf, "%s%s\r\n", buf, buf2);
         
    }
   send_to_char(buf, ch);
  }  
  if (IS_MOB(k)) {
    sprintf(buf, "Mob Spec-Proc: %s, NPC Bare Hand Dam: %dd%d\r\n",
        (mob_index[GET_MOB_RNUM(k)].func ? "Exists" : "None"),
        k->mob_specials.damnodice, k->mob_specials.damsizedice);
    send_to_char(buf, ch);
  }
  sprintf(buf, "Carried: weight: %ld, items: %ld ",
      IS_CARRYING_W(k), IS_CARRYING_N(k));

  for (i = 0, j = k->carrying; j; j = j->next_content, i++);
  sprintf(buf, "%sItems in: inventory: %d, ", buf, i);

  for (i = 0, i2 = 0; i < NUM_WEARS; i++)
    if (GET_EQ(k, i))
      i2++;
  sprintf(buf2, "eq: %d\r\n", i2);
  strcat(buf, buf2);
  send_to_char(buf, ch);

  sprintf(buf, "Hunger: %d, Thirst: %d, Drunk: %d\r\n",
      GET_COND(k, FULL), GET_COND(k, THIRST), GET_COND(k, DRUNK));
  send_to_char(buf, ch);

  sprintf(buf, "Master is: %s, Followers are:",
      ((k->master) ? GET_NAME(k->master) : "<none>"));

  for (fol = k->followers; fol; fol = fol->next) {
    sprintf(buf2, "%s %s", found++ ? "," : "", PERS(fol->follower, ch));
    strcat(buf, buf2);
    if (strlen(buf) >= 62) {
      if (fol->next)
    send_to_char(strcat(buf, ",\r\n"), ch);
      else
    send_to_char(strcat(buf, "\r\n"), ch);
      *buf = found = 0;
    }
  }
  if (*buf)
    send_to_char(strcat(buf, "\r\n"), ch);

  /* Showing the bitvector */
  sprintbit(AFF_FLAGS(k), affected_bits, buf2);
  sprintf(buf, "AFF: %s%s", CCYEL(ch, C_NRM), buf2);
  sprintbit(AFF2_FLAGS(k), affected2_bits, buf2);
  sprintf(buf, "%s %s", buf, buf2);
  send_to_char(buf, ch);
  sprintbit(AFF3_FLAGS(k), affected3_bits, buf2);
  sprintf(buf, " %s\\c00", buf2);
  send_to_char(buf, ch);
  if (IS_NPC(k)) {
    sprintbit(MOB_SKILLS(k), mobskill_bits, buf2);
    sprintf(buf, "SKILL: \\c04%s\\c00\r\n", buf2);
    send_to_char(buf, ch);
  }

  /* check mobiles for a script */
  if (IS_NPC(k)) 
   if (SCRIPT(k))
    list_triggers_to_char(ch, SCRIPT(k));
}


ACMD(do_stat)
{
  struct char_data *victim = 0;
  struct obj_data *object = 0;
  int tmp;

  half_chop(argument, buf1, buf2);

  if (!*buf1) {
    send_to_char("Stats on who or what?\r\n", ch);
    return;
  } else if (is_abbrev(buf1, "room")) {
    do_stat_room(ch);
  } else if (is_abbrev(buf1, "mob")) {
    if (!*buf2)
      send_to_char("Stats on which mobile?\r\n", ch);
    else {
      if ((victim = get_char_vis(ch, buf2)))
    do_stat_character(ch, victim);
      else
    send_to_char("No such mobile around.\r\n", ch);
    }
  } else if (is_abbrev(buf1, "player")) {
    if (!*buf2) {
      send_to_char("Stats on which player?\r\n", ch);
    } else {
      if ((victim = get_player_vis(ch, buf2, 0)))
    do_stat_character(ch, victim);
      else
    send_to_char("No such player around.\r\n", ch);
    }
  } else if (is_abbrev(buf1, "file")) {
    if (!*buf2) {
      send_to_char("Stats on which player?\r\n", ch);
    } else {
      if (NULL != (victim = xmlload_char(buf2))) {
    if (GET_LEVEL(victim) > GET_LEVEL(ch))
      send_to_char("Sorry, you can't do that.\r\n", ch);
    else {
      GET_POS(victim) = POS_STANDING;
      do_stat_character(ch, victim);
    }
    free_char(victim);
      } else {
    send_to_char("There is no such player.\r\n", ch);
      }
    }
  } else if (is_abbrev(buf1, "object")) {
    if (!*buf2)
      send_to_char("Stats on which object?\r\n", ch);
    else {
      if ((object = get_obj_vis(ch, buf2)))
    do_stat_object(ch, object);
      else
    send_to_char("No such object around.\r\n", ch);
    }
  } else {
    if ((object = get_object_in_equip_vis(ch, buf1, ch->equipment, &tmp)))
      do_stat_object(ch, object);
    else if ((object = get_obj_in_list_vis(ch, buf1, ch->carrying)))
      do_stat_object(ch, object);
    else if ((victim = get_char_room_vis(ch, buf1)))
      do_stat_character(ch, victim);
    else if ((object = get_obj_in_list_vis(ch, buf1, world[ch->in_room].contents)))
      do_stat_object(ch, object);
    else if ((victim = get_char_vis(ch, buf1)))
      do_stat_character(ch, victim);
    else if ((object = get_obj_vis(ch, buf1)))
      do_stat_object(ch, object);
    else
      send_to_char("Nothing around by that name.\r\n", ch);
  }
}


ACMD(do_shutdown)
{
  char arg2[1024];
  if (subcmd != SCMD_SHUTDOWN) {
    send_to_char("If you want to shut something down, say so!\r\n", ch);
    return;
  }
  two_arguments(argument, arg,arg2);

  if (!*arg) {
    sprintf(buf, "(GC) Shutdown by %s.", GET_NAME(ch));
    mlog(buf);
    send_to_all("Shutting down for now. But don't worry, it will be up in a few minutes.\r\n");
    circle_shutdown = 1;
  } else if (!str_cmp(arg, "reboot")) {
    sprintf(buf, "(GC) Reboot by %s.", GET_NAME(ch));
    mlog(buf);
    send_to_all("Rebooting. We will be up in a few seconds.\r\n");
    touch("../.fastboot");
    circle_shutdown = circle_reboot = 1;
  } else if (!str_cmp(arg, "die")) {
    sprintf(buf, "(GC) Shutdown by %s.", GET_NAME(ch));
    mlog(buf);
    send_to_all("Shutting down for maintenance.\r\n");
    touch("../.killscript");
    circle_shutdown = 1;
  } else if (!str_cmp(arg, "pause")) {
    sprintf(buf, "(GC) Shutdown by %s.", GET_NAME(ch));
    mlog(buf);
    send_to_all("Shutting down for maintenance.\r\n");
    touch("../pause");
    circle_shutdown = 1;
  } else if (!str_cmp(arg,"hotboot")) {
    if (!*arg2) {
      do_hotboot(NULL);
    } else if (!strcmp(arg2, "check")) {
      hotboot_check(ch);
    } else {
      do_hotboot(arg2);
    }
  }
  else
    send_to_char("Unknown shutdown option.\r\n", ch);
}


void stop_snooping(struct char_data * ch)
{
  if (!ch->desc->snooping)
    send_to_char("You aren't snooping anyone.\r\n", ch);
  else {
    send_to_char("You stop snooping.\r\n", ch);
    ch->desc->snooping->snoop_by = NULL;
    ch->desc->snooping = NULL;
    sprintf(buf,"(GC) %s has stopped snooping.", GET_NAME(ch));
    mlog(buf);
  }
}


ACMD(do_snoop)
{
  struct char_data *victim, *tch;

  if (!ch->desc)
    return;

  one_argument(argument, arg);

  if (!*arg)
    stop_snooping(ch);
  else if (!(victim = get_char_vis(ch, arg)))
    send_to_char("No such person around.\r\n", ch);
  else if (!victim->desc)
    send_to_char("There's no link.. nothing to snoop.\r\n", ch);
  else if (victim == ch)
    stop_snooping(ch);
  else if (victim->desc->snoop_by)
    send_to_char("Busy already. \r\n", ch);
  else if (victim->desc->snooping == ch->desc)
    send_to_char("Don't be stupid.\r\n", ch);
  else if (PLR_FLAGGED(victim, PLR_WIZNOSNOOP))
    send_to_char("That person has opted not to allow their private conversations to be\r\n"
        "infringed upon.  If you suspect them of some wrong doing, please\r\n"
        "read the 'godcmds' file, or allert someone who can read it of your\r\n"
        "suspitions.\r\n", ch);
  else {
    if (victim->desc->original)
      tch = victim->desc->original;
    else
      tch = victim;

    if (GET_LEVEL(tch) >= GET_LEVEL(ch)) {
      send_to_char("You can't.\r\n", ch);
      return;
    }
    send_to_char(OK, ch);
    sprintf(buf,"%s has begun snooping on %s.", GET_NAME(ch), GET_NAME(victim));
    xlog(SYS_SNOOP, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), buf, TRUE);

    if (ch->desc->snooping)
      ch->desc->snooping->snoop_by = NULL;

    ch->desc->snooping = victim->desc;
    victim->desc->snoop_by = ch->desc;
  }
}



ACMD(do_switch)
{
  struct char_data *victim;

  one_argument(argument, arg);

  if (ch->desc->original)
    send_to_char("You're already switched.\r\n", ch);
  else if (!*arg)
    send_to_char("Switch with who?\r\n", ch);
  else if (!(victim = get_char_vis(ch, arg)))
    send_to_char("No such character.\r\n", ch);
  else if (ch == victim)
    send_to_char("Hee hee... we are jolly funny today, eh?\r\n", ch);
  else if (victim->desc)
    send_to_char("You can't do that, the body is already in use!\r\n", ch);
  else if ((GET_LEVEL(ch) < LVL_IMPL) && !IS_NPC(victim))
    send_to_char("You aren't holy enough to use a mortal's body.\r\n", ch);
  else {
    send_to_char(OK, ch);

    ch->desc->character = victim;
    ch->desc->original = ch;

    victim->desc = ch->desc;
    ch->desc = NULL;

    alog("%s switched into %s!", GET_RNAME(ch), GET_RNAME(victim));
  }
}


ACMD(do_return)
{
  if (ch->desc && ch->desc->original) {
    send_to_char("You return to your body.\r\n", ch);

    /* JE 2/22/95 */
    /* if someone switched into your original body, disconnect them */
    if (ch->desc->original->desc)
      close_socket(ch->desc->original->desc);

    ch->desc->character = ch->desc->original;
    ch->desc->original = NULL;

    ch->desc->character->desc = ch->desc;
    ch->desc = NULL;
  }
}



ACMD(do_load)
{
  struct char_data *mob;
  struct obj_data *obj;
  int number, r_num;

  two_arguments(argument, buf, buf2);

  if (!*buf || !*buf2 || !isdigit(*buf2)) {
    send_to_char("Usage: load { obj | mob } <number>\r\n", ch);
    return;
  }
  if ((number = atoi(buf2)) < 0) {
    send_to_char("A NEGATIVE number??\r\n", ch);
    return;
  }
  if (is_abbrev(buf, "mob")) {
    if ((r_num = real_mobile(number)) < 0) {
      send_to_char("There is no monster with that number.\r\n", ch);
      return;
    }
    if (!check_unique_mob(r_num, number)) {
      send_to_char("That monster is flagged UNQIUE and already exists on the mud!\r\n", ch);
      return;
    }

    mob = read_mobile(r_num, REAL);
    char_to_room(mob, ch->in_room);

    act("$n makes a quaint, magical gesture with one hand.", TRUE, ch,
    0, 0, TO_ROOM);
    act("$n has created $N!", FALSE, ch, 0, mob, TO_ROOM);
    act("You create $N.", FALSE, ch, 0, mob, TO_CHAR);
    load_mtrigger(mob);

    if(!GET_DISGUISED(ch))
        sprintf(buf,"%s has loaded mob %d. (%s)", GET_NAME(ch), number, GET_NAME(mob));    
    else
        sprintf(buf,"%s (%s) has loaded mob %d. (%s)", GET_NAME(ch), GET_RNAME(ch), number, GET_NAME(mob));    

    xlog(SYS_LOAD, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), buf, TRUE);
    
  } else if (is_abbrev(buf, "obj")) {
    if ((r_num = real_object(number)) < 0) {
      send_to_char("There is no object with that number.\r\n", ch);
      return;
    }
    if (!check_unique_obj(r_num, number)) {
      send_to_char("That object is flagged UNQIUE and already exists on the mud!\r\n", ch);
      return;
    }

    obj = read_object(r_num, REAL);
    obj_to_char(obj, ch);
    act("$n makes a strange magical gesture.", TRUE, ch, 0, 0, TO_ROOM);
    act("$n has created $p!", FALSE, ch, obj, 0, TO_ROOM);
    act("You create $p.", FALSE, ch, obj, 0, TO_CHAR);
    load_otrigger(obj);
    
    if(!GET_DISGUISED(ch))
        sprintf(buf,"%s has loaded obj %d (%s).", GET_NAME(ch), number, obj->short_description);
    else
        sprintf(buf,"%s (%s) has loaded obj %d (%s).", GET_NAME(ch), GET_RNAME(ch), number, obj->short_description);

    xlog(SYS_LOAD, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), buf, TRUE);
  } else
    send_to_char("That'll have to be either 'obj' or 'mob'.\r\n", ch);

}


ACMD(do_clone)
{  
  struct obj_data *obj, *newobj;

  argument = one_argument(argument, arg);

  if (!*arg) {
    send_to_char("Duplicate which object?\r\n", ch);
    send_to_char("Usage:  clone <object name>\n\r", ch);
    return;
  }

  if( !(obj = get_obj_vis(ch, arg))) {
    send_to_char("No such object exists!\n\r", ch);
    return;
  }


  if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying)))  {
    sprintf(buf, "You don't seem to have any %ss.\r\n", arg);
    send_to_char(buf, ch);
    return;
  }

  if (IS_SET(GET_OBJ_EXTRA(obj), ITEM_UNIQUE)) {
    send_to_char("You can not clone unique objects!\r\n", ch);
    return;
  }

  newobj = clone_obj(obj);
  obj_to_char(newobj, ch);
  act("With a bright flash of light, $n duplicates an object.", TRUE, ch, 0, 0, TO_ROOM);
  act("$n has duplicated $p!", FALSE, ch, obj, 0, TO_ROOM);
  act("You duplicate $p.", FALSE, ch, obj, 0, TO_CHAR);

  if(!GET_DISGUISED(ch))
      sprintf(buf,"%s has cloned obj %ld (%s).", GET_NAME(ch), GET_OBJ_VNUM(obj), obj->short_description);
  else
      sprintf(buf,"%s (%s) has cloned obj %ld (%s).", GET_NAME(ch), GET_RNAME(ch), GET_OBJ_VNUM(obj), obj->short_description);

  xlog(SYS_CLONE, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), buf, TRUE);
}


ACMD(do_vstat)
{
  struct char_data *mob;
  struct obj_data *obj;
  int number, r_num;

  two_arguments(argument, buf, buf2);

  if (!*buf || !*buf2 || !isdigit(*buf2)) {
    send_to_char("Usage: vstat { obj | mob } <number>\r\n", ch);
    return;
  }
  if ((number = atoi(buf2)) < 0) {
    send_to_char("A NEGATIVE number??\r\n", ch);
    return;
  }
  if (is_abbrev(buf, "mob")) {
    if ((r_num = real_mobile(number)) < 0) {
      send_to_char("There is no monster with that number.\r\n", ch);
      return;
    }
    mob = read_mobile(r_num, REAL);
    char_to_room(mob, 0);
    do_stat_character(ch, mob);
    extract_char(mob, FALSE);
  } else if (is_abbrev(buf, "obj")) {
    if ((r_num = real_object(number)) < 0) {
      send_to_char("There is no object with that number.\r\n", ch);
      return;
    }
    obj = read_object(r_num, REAL);
    do_stat_object(ch, obj);
    extract_obj(obj);
  } else
    send_to_char("That'll have to be either 'obj' or 'mob'.\r\n", ch);
}




/* clean a room of all mobiles and objects */
ACMD(do_purge)
{
  struct char_data *vict, *next_v;
  struct obj_data *obj, *next_o;
  
  one_argument(argument, buf);

  if (*buf) {			/* argument supplied. destroy single object
                 * or char */
    if ((vict = get_char_room_vis(ch, buf))) {
      if (!IS_NPC(vict) && (GET_LEVEL(ch) <= GET_LEVEL(vict))) {
    send_to_char("Fuuuuuuuuu!\r\n", ch);
    return;
      }
      act("$n disintegrates $N.", FALSE, ch, 0, vict, TO_NOTVICT);

      if (!IS_NPC(vict)) {
    sprintf(buf, "%s has purged %s.", GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch), GET_NAME(vict));
        xlog(SYS_SET, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), buf, TRUE);
    if (vict->desc) {
      close_socket(vict->desc);
      vict->desc = NULL;
    }
        extract_char(vict, FALSE);
      } else
        extract_char(vict, FALSE);
    } else if ((obj = get_obj_in_list_vis(ch, buf, world[ch->in_room].contents))) {
      act("$n destroys $p.", FALSE, ch, obj, 0, TO_ROOM);
      extract_obj(obj);
    } else {
      send_to_char("Nothing here by that name.\r\n", ch);
      return;
    }

    send_to_char(OK, ch);
  } else {			/* no argument. clean out the room */
    act("$n gestures... You are surrounded by scorching flames!",
    FALSE, ch, 0, 0, TO_ROOM);
    send_to_room("The world seems a little cleaner.\r\n", ch->in_room);

    for (vict = world[ch->in_room].people; vict; vict = next_v) {
      next_v = vict->next_in_room;
      if (IS_NPC(vict))
    extract_char(vict, FALSE);
    }

    for (obj = world[ch->in_room].contents; obj; obj = next_o) {
      next_o = obj->next_content;
      extract_obj(obj);
    }
    if (ROOM_AFFECTED(ch->in_room,RAFF_FORTIFY))
          affect_from_room(&world[ch->in_room],SKILL_FORTIFY);

  }
}



ACMD(do_advance)
{
  struct char_data *victim;
  char *name = arg, *level = buf2;
  int newlevel, oldlevel,i;

  void do_start(struct char_data *ch);

  two_arguments(argument, name, level);

  if (*name) {
    if (!(victim = get_char_vis(ch, name))) {
      send_to_char("That player is not here.\r\n", ch);
      return;
    }
  } else {
    send_to_char("Advance who?\r\n", ch);
    return;
  }

  if (GET_LEVEL(ch) <= GET_LEVEL(victim)) {
    send_to_char("Maybe that's not such a great idea.\r\n", ch);
    return;
  }
  if (IS_NPC(victim)) {
    send_to_char("NO!  Not on NPC's.\r\n", ch);
    return;
  }
  if (!*level || (newlevel = atoi(level)) <= 0) {
    send_to_char("That's not a level!\r\n", ch);
    return;
  }
  if (newlevel > LVL_IMPL) {
    sprintf(buf, "%d is the highest possible level.\r\n", LVL_IMPL);
    send_to_char(buf, ch);
    return;
  }
  if (newlevel > GET_LEVEL(ch)) {
    send_to_char("Yeah, right.\r\n", ch);
    return;
  }
  if (newlevel == GET_LEVEL(victim)) {
    send_to_char("They are already at that level.\r\n", ch);
    return;
  }

  oldlevel = GET_LEVEL(victim);
  if (newlevel < GET_LEVEL(victim)) {
    oldlevel = 1;
    GET_LEVEL(victim) = 0;
    GET_TRAINS(victim) = 0;
    do_start(victim);
    SET_MAX_MANA(victim, 50);
    SET_MANA(victim,  50);
    GET_MAX_HIT(victim)  = GET_HIT(victim)  = 50;
    GET_MAX_MOVE(victim) = GET_MOVE(victim) = 50;
    GET_FRACT_MOVE(victim) = 0;
    send_to_char("You are momentarily enveloped by darkness!\r\n"
         "You feel somewhat diminished.\r\n", victim);
  } else {
    act("$n makes some strange gestures.\r\n"
    "A strange feeling comes upon you,\r\n"
    "Like a giant hand, light comes down\r\n"
    "from above, grabbing your body, that\r\n"
    "begins to pulse with colored lights\r\n"
    "from inside.\r\n\r\n"
    "Your head seems to be filled with demons\r\n"
    "from another plane as your body dissolves\r\n"
    "to the elements of time and space itself.\r\n"
    "Suddenly a silent explosion of light\r\n"
    "snaps you back to reality.\r\n\r\n"
    "You feel slightly different.", FALSE, ch, 0, victim, TO_VICT);
  }

  send_to_char(OK, ch);

  sprintf(buf, "%s has advanced %s to level %d (from %d)",
      GET_NAME(ch), GET_NAME(victim), newlevel, oldlevel);
  xlog(SYS_PLEVEL, LVL_IMMORT, buf, TRUE);

  GET_EXP(victim) = (newlevel - 1) * GET_LEVEL_EXP(victim);

  for (i = oldlevel; i < newlevel; i++)
    advance_level(victim);

  save_char(victim, NOWHERE);
}

void do_restore_all(struct char_data *ch)
{
  struct char_data *i;

  for (i = character_list; i != NULL; i = i->next) {
    if (!IS_NPC(i) && (GET_LEVEL(i) < LVL_IMMORT)) {
      GET_HIT(i)  = GET_MAX_HIT(i);
      SET_MANA(i, GET_MAX_MANA(i));
      GET_MOVE(i) = GET_MAX_MOVE(i);
      GET_FRACT_MOVE(i) = 0;
      update_pos(i);
      act("You have been fully healed by $N!",FALSE,i,0,ch,TO_CHAR);
    }
  }
  send_to_char(OK,ch);
}

void do_restore_quest(struct char_data *ch)
{
  struct char_data *i;
    
  for (i = character_list; i != NULL; i = i->next) {
    if (!IS_NPC(i) && (GET_LEVEL(i) < LVL_IMMORT) && PRF_FLAGGED(i, PRF_QUEST)) {
      GET_HIT(i)  = GET_MAX_HIT(i);
      SET_MANA(i, GET_MAX_MANA(i));
      GET_MOVE(i) = GET_MAX_MOVE(i);
      GET_FRACT_MOVE(i) = 0;
      update_pos(i);
      act("You have been fully healed by $N!",FALSE,i,0,ch,TO_CHAR);
    }
  }
  send_to_char(OK,ch);
}


ACMD(do_restore)
{
  struct char_data *vict;
  int i;

  one_argument(argument, buf);
  if (!*buf)
    send_to_char("Whom do you wish to restore?\r\n", ch);
  else if (!strncasecmp(buf,"all",strlen(buf)))
    do_restore_all(ch);
  else if (!strncasecmp(buf,"quest",strlen(buf)))
    do_restore_quest(ch);
  else if (!(vict = get_char_vis(ch, buf)))
    send_to_char(NOPERSON, ch);
  else {
    GET_HIT(vict) = GET_MAX_HIT(vict);
    SET_MANA(vict, GET_MAX_MANA(vict));
    GET_MOVE(vict) = GET_MAX_MOVE(vict);
    GET_FRACT_MOVE(vict) = 0;

    if ((GET_LEVEL(ch) >= LVL_GROOVYGL) && (GET_LEVEL(vict) >= LVL_IMMORT)) {
      for (i = 1; i <= MAX_SKILLS; i++)
    SET_SKILL(vict, i, 100);

      if (GET_LEVEL(vict) >= LVL_GRGOD) {
    vict->real_abils.intel = 50;
    vict->real_abils.wis = 50;
    vict->real_abils.dex = 50;
    vict->real_abils.str = 50;
    vict->real_abils.con = 50;
    vict->real_abils.cha = 50;
      }
      vict->aff_abils = vict->real_abils;
    }
    update_pos(vict);
    send_to_char(OK, ch);
    act("You have been fully healed by $N!", FALSE, vict, 0, ch, TO_CHAR);

    alog("(GC) %s restored %s!", GET_RNAME(ch), GET_RNAME(vict));
  }
}

void do_refresh_all(struct char_data *ch)
{
  struct char_data *i;

  for (i = character_list; i != NULL; i = i->next) {
    if (!IS_NPC(i) && (GET_LEVEL(i) < LVL_IMMORT)) {
      GET_MOVE(i) = GET_MAX_MOVE(i);
      GET_FRACT_MOVE(i) = 0;
      update_pos(i);
      act("You have been fully refreshed by $N!",FALSE,i,0,ch,TO_CHAR);
    }
  }
  send_to_char(OK,ch);
}

ACMD(do_refresh)
{
  struct char_data *vict;
 
  one_argument(argument, buf);
  if (!*buf)
    send_to_char("Whom do you wish to refresh?\r\n", ch);
  else if (!strncasecmp(buf,"all",strlen(buf)))
    do_refresh_all(ch);
  else if (!(vict = get_char_vis(ch, buf)))
    send_to_char(NOPERSON, ch);
  else {
    GET_MOVE(vict) = GET_MAX_MOVE(vict);
    GET_FRACT_MOVE(vict) = 0;
    update_pos(vict);
    send_to_char(OK, ch);
    act("You have been fully refreshed by $N!", FALSE, vict, 0, ch, TO_CHAR);
    alog("%s refreshed %s.", GET_RNAME(ch), GET_RNAME(vict));
  }
}

ACMD(do_bugcheck)
{
  int i, bad_roomdescs = 0, susp_rooms = 0, bad_exits = 0,
      recursive = 0, j, noexit, fromdir = 0, dummy, znum = 0, 
      no_returns = 0, noexits = 0, saved = ch->in_room,
      low, high, temp; 
  int bad_objdescs = 0, susp_objs = 0, unbalanced = 0, files = FALSE;
  FILE *badroomfl = NULL, *susproomfl = NULL, *recexitfl = NULL , 
       *nobackfl = NULL, *nowherefl = NULL,  *badobjfl = NULL, 
       *suspobjfl = NULL, *unbalfl = NULL, *noexitfl = NULL;

  skip_spaces(&argument);
  if (*argument && !strcmp(argument, "-f")) {
    files = TRUE;
    if (!(badroomfl = fopen("world/bad_rooms", "w")) || 
        !(susproomfl = fopen("world/susp_rooms", "w")) || 
        !(recexitfl = fopen("world/rec_exits", "w")) || 
        !(nobackfl = fopen("world/no_backs", "w")) || 
        !(nowherefl = fopen("world/nowheres", "w")) ||
        !(noexitfl = fopen("world/noexits", "w")) || 
        !(badobjfl = fopen("world/bad_objs", "w")) || 
        !(suspobjfl = fopen("world/susp_objs", "w")) || 
        !(unbalfl = fopen("world/unbalanced", "w"))) {
      send_to_char("Sorry, the files cannot be created.\r\n", ch);
      fclose(badroomfl); 
      fclose(susproomfl); 
      fclose(recexitfl);
      fclose(nobackfl); 
      fclose(nowherefl); 
      fclose(badobjfl);
      fclose(suspobjfl); 
      fclose(unbalfl);
      fclose(noexitfl);
      files = FALSE;
    }
  }   
  else if (*argument) {
    znum = 0;
    temp = atoi(argument);
    for (i = 0; i < top_of_zone_table; i ++)
      if (zone_table[i].number == temp) {
        znum = i;
        break;
      }
    if (!znum) {
      send_to_char("Sorry, there is no zone with that number.\r\n", ch);
      return;
    }
  }
  /* Checking rooms */ 
  if (files) {
    fprintf(badroomfl, "---------------- Bad room descs ---------\n");
    fprintf(susproomfl, "------------- Suspected room descs ---------\n");
    fprintf(recexitfl, "------------- Recursive exits ---------------\n");
    fprintf(nobackfl, "------------ Exits not leading back -----------\n");
    fprintf(nowherefl, "------------ Exits leading nowhere ------------\n");
    fprintf(badobjfl, "------------ Bad object descriptions ----------\n");
    fprintf(suspobjfl, "---------- Suspected object descriptions -------\n");
    fprintf(unbalfl, "-------------- Unbalanced objects ---------------\n");  
    fprintf(noexitfl, "-------------- Rooms with no exits --------------\n");
  }
  low = 0;
  high = top_of_world;
  if (znum) {
    for (i = 0; i < top_of_world; i ++)
      if (world[i].number >= zone_table[znum].number*100) {
        low = i;
        break;
      }
    for (i = low; i < top_of_world; i ++)
      if (world[i].number > zone_table[znum].top) {
        high = i;
        break;
      }
    sprintf(buf, "Zone chosen: %d, vnum %d '%s', zone range %d-%d.\r\n",
    znum, zone_table[znum].number, zone_table[znum].name,
    zone_table[znum].number*100, zone_table[znum].top);
    send_to_char(buf, ch);
  }
  for (i = low; i <= high; i ++) {
    if (!*(world[i].description) || !(world[i].name) ||
        isname("Room #", world[i].name)) {
      bad_roomdescs ++;
      if (files) 
        fprintf(badroomfl, "%d\n", world[i].number);
    }
    else if (isname("unfinished", world[i].name) ||
         isname("unfinished", world[i].description) ||
         isname("#", world[i].name)) {
      susp_rooms ++;
      if (files)
        fprintf(susproomfl, "%d\n", world[i].number);
    }  
    noexit = TRUE;
    char_from_room(ch);
    char_to_room(ch, i);
    for (j = 0; j < NUM_OF_DIRS; j ++) {
        if ((j < 2) || (j == 6) || (j == 7))
          fromdir = j + 2;
        else if ((j == 2) || (j == 3) || (j == 8) || (j == 9))
          fromdir = j - 2;
        else if (j == 4)
          fromdir = 5;
        else 
          fromdir = 4 ;
        if (EXIT(ch, j)) {
          noexit = FALSE;
          if (EXIT(ch, j)->to_room == NOWHERE) {
            bad_exits ++;
          if (files)
            fprintf(nowherefl,"%d, %s\n", world[i].number, dirs[j]);
          }
          else if (EXIT(ch, j)->to_room == i) {
            recursive ++;
            if (files)
              fprintf(recexitfl, "%d, %s\n", world[i].number, dirs[j]);
          }
          else {
            dummy = EXIT(ch, j)->to_room;
            char_from_room(ch);
            char_to_room(ch, dummy);
            if (!EXIT(ch, fromdir) || EXIT(ch, fromdir)->to_room != i) {
              no_returns ++;
              if (files)
                fprintf(nobackfl, "%d, exit %s to %d is one-way\n", 
                   world[i].number, dirs[j], world[dummy].number);
            } 
            char_from_room(ch);
            char_to_room(ch, i);
          }
      }
    }
    if (noexit) {
      noexits ++;
      if (files) 
        fprintf(noexitfl, "%d\n", world[i].number);
    }
  }
  sprintf(buf, "Bad roomdescs/names: %d\r\nSuspected room descs/names: %d\r\nRecursive exits: %d\r\nExits not leading back: %d \r\nRooms with no exits: %d \r\nExits leading nowhere: %d\r\n",
                bad_roomdescs, susp_rooms, recursive, no_returns, noexits,
                bad_exits);
  send_to_char(buf, ch);
  char_from_room(ch);
  char_to_room(ch, saved);
  low = 0;
  high = top_of_objt;
  if (znum) {
    for (i = 0; i < top_of_objt; i ++)
      if (GET_OBJ_VNUM(&obj_proto[i]) >= zone_table[znum].number*100) {
        low = i;
        break;
      }
    for (i = low; i < top_of_objt; i ++)
      if (GET_OBJ_VNUM(&obj_proto[i]) > zone_table[znum].top) {
        high = i;
        break;
      }
  }
  for (i = low; i <= high; i ++) {
    dummy = FALSE;
    if (!(obj_proto[i].name) || !(obj_proto[i].description) || 
        !(obj_proto[i].short_description)) {
      bad_objdescs ++;
      if (files)
        fprintf(badobjfl,"%ld\n", obj_index[i].virtual);
    }
    else if (isname("unfinished", obj_proto[i].name) ||
             isname("unfinished", obj_proto[i].description) ||
             isname("unfinished", obj_proto[i].short_description)) {
      susp_objs ++;
      if (files) 
        fprintf(suspobjfl, "%ld\n", obj_index[i].virtual);
    }
    if (GET_OBJ_TYPE(&obj_proto[i]) == ITEM_WEAPON) 
      if (((GET_OBJ_VAL(&obj_proto[i],1)*GET_OBJ_VAL(&obj_proto[i],2)/2 +
          GET_OBJ_VAL(&obj_proto[i], 0)) > 50))
        dummy = TRUE;
    if (GET_OBJ_TYPE(&obj_proto[i]) == ITEM_ARMOR)
      if ((GET_OBJ_VAL(&obj_proto[i], 0) > 40) ||
          (GET_OBJ_VAL(&obj_proto[i], 1) > 40))
        dummy = TRUE;

    for (j = 0; (j < MAX_OBJ_AFFECT) && !dummy; j ++) 
      if (((obj_proto[i].affected[j].location <= APPLY_CHA) &&
          (obj_proto[i].affected[j].modifier > 8)) || 
          ((obj_proto[i].affected[j].location <= APPLY_MOVE) &&
          (obj_proto[i].affected[j].modifier > 200)) ||
          ((obj_proto[i].affected[j].location <= APPLY_ABS) &&
          (obj_proto[i].affected[j].location > 50)))
        dummy = TRUE; 
    unbalanced += dummy;
    if (dummy && files) 
      fprintf(unbalfl, "%ld\n", obj_index[i].virtual);
  }
  sprintf(buf, "Bad objdescs/names: %d\r\nSuspected objdescs/names: %d\r\nUnbalanced objs: %d\r\n",
          bad_objdescs, susp_objs, unbalanced);
  send_to_char(buf, ch);      
  if (files) {
    fclose(badroomfl);
    fclose(susproomfl);
    fclose(recexitfl);
    fclose(nobackfl);
    fclose(nowherefl);
    fclose(badobjfl);
    fclose(suspobjfl);
    fclose(unbalfl);
    fclose(noexitfl);
  }
}

                        

void perform_immort_vis(struct char_data *ch)
{
  if (GET_INVIS_LEV(ch) == 0 && !IS_AFFECTED(ch, AFF_HIDE | AFF_INVISIBLE)) {
    send_to_char("You are already fully visible.\r\n", ch);
    return;
  }
   
  GET_INVIS_LEV(ch) = 0;
  appear(ch);
  send_to_char("You are now fully visible.\r\n", ch);
}


void perform_immort_invis(struct char_data *ch, int level)
{
  struct char_data *tch;

  if (IS_NPC(ch))
    return;

  for (tch = world[ch->in_room].people; tch; tch = tch->next_in_room) {
    if (tch == ch)
      continue;
    if (GET_LEVEL(tch) >= GET_INVIS_LEV(ch) && GET_LEVEL(tch) < level)
      act("You blink and suddenly realize that $n is gone.", FALSE, ch, 0,
      tch, TO_VICT);
    if (GET_LEVEL(tch) < GET_INVIS_LEV(ch) && GET_LEVEL(tch) >= level)
      act("You suddenly realize that $n is standing beside you.", FALSE, ch, 0,
      tch, TO_VICT);
  }

  GET_INVIS_LEV(ch) = level;
  sprintf(buf, "Your invisibility level is %d.\r\n", level);
  send_to_char(buf, ch);
}
  

ACMD(do_invis)
{
  int level;

  if (IS_NPC(ch)) {
    send_to_char("You can't do that!\r\n", ch);
    return;
  }

  one_argument(argument, arg);
  if (!*arg) {
    if (GET_INVIS_LEV(ch) > 0)
      perform_immort_vis(ch);
    else
      perform_immort_invis(ch, GET_LEVEL(ch));
  } else {
    level = atoi(arg);
    if (level > GET_LEVEL(ch))
      send_to_char("You can't go invisible above your own level.\r\n", ch);
    else if (level < 1)
      perform_immort_vis(ch);
    else
      perform_immort_invis(ch, level);
  }
}

ACMD(do_gecho)
{
  struct descriptor_data *pt;

  skip_spaces(&argument);

  if (PLR_FLAGGED(ch, PLR_CANTGECHO)) {
    send_to_char("Your GECHO rights have been removed.  You can't do this.\r\n", ch);
    return;
  }
  if (!*argument)
    send_to_char("That must be a mistake...\r\n", ch);
  else {
    sprintf(buf, "%s\r\n", gecho_to_str(argument));
    for (pt = descriptor_list; pt; pt = pt->next)
      if (!pt->connected && pt->character && (pt->character != ch) && 
      !PLR_FLAGGED(pt->character, PLR_MAILING | PLR_WRITING | PLR_NOGECHO))
    send_to_char(buf, pt->character);
    if (PRF_FLAGGED(ch, PRF_NOREPEAT))
      send_to_char(OK, ch);
    else
      send_to_char(buf, ch);

  sprintf(buf,"%s has gechoed.", GET_NAME(ch));
  xlog(SYS_COMM, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), buf, TRUE);

  }
}

ACMD(do_zecho)
{
  struct descriptor_data *pt;
  int zone = zone_table[IN_ROOM(ch)].number;

  skip_spaces(&argument);

  if (!*argument)
    send_to_char("That must be a mistake...\r\n", ch);
  else {
    sprintf(buf, "%s\r\n", gecho_to_str(argument));
    for (pt = descriptor_list; pt; pt = pt->next)
      if (!pt->connected && pt->character && (pt->character != ch) && 
      !PLR_FLAGGED(pt->character, PLR_MAILING | PLR_WRITING | PLR_NOGECHO))
    if ((IN_ROOM(pt->character) != NOWHERE) && (zone_table[IN_ROOM(pt->character)].number ==
    zone))
    send_to_char(buf, pt->character);
    if (PRF_FLAGGED(ch, PRF_NOREPEAT))
      send_to_char(OK, ch);
    else
      send_to_char(buf, ch);

  sprintf(buf,"%s has zechoed.", GET_NAME(ch));
  xlog(SYS_COMM, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), buf, TRUE);

  }
}

ACMD(do_poofset) {
  char **msg;

  switch (subcmd) {
  case SCMD_POOFIN:    msg = &(POOFIN(ch));    break;
  case SCMD_POOFOUT:   msg = &(POOFOUT(ch));   break;
  default:    return;    break;
  }

  ch->desc->str = msg;
  send_to_char("\x1B[H\x1B[J", ch);
  ch->desc->backstr = str_dup(*msg);
  ch->desc->max_str = MAX_STRING_LENGTH;
  ch->desc->mail_to[0] = 0;
  SET_BIT(PLR_FLAGS(ch), PLR_WRITING);
}

ACMD(do_dc)
{
  struct descriptor_data *d;
  int num_to_dc;

  one_argument(argument, arg);
  if (!(num_to_dc = atoi(arg))) {
    send_to_char("Usage: DC <connection number> (type USERS for a list)\r\n", ch);
    return;
  }
  for (d = descriptor_list; d && d->desc_num != num_to_dc; d = d->next);

  if (!d) {
    send_to_char("No such connection.\r\n", ch);
    return;
  }
  if (d->character && GET_LEVEL(d->character) >= GET_LEVEL(ch)) {
    send_to_char("Umm.. maybe that's not such a good idea...\r\n", ch);
    return;
  }
  sprintf(buf, "Connection for %s closed by %s.", (d->character ? GET_NAME(d->character) : "<unknown>"), GET_NAME(ch));
  xlog(SYS_CONN, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), buf, TRUE);
  sprintf(buf, "Connection #%d closed.\r\n", num_to_dc);
  send_to_char(buf, ch);
  close_socket(d);
}

ACMD(do_immlock)
{
  if (immlock)
  {
    immlock = 0;
    send_to_char("Immortals are no longer locked out.\r\n", ch);
  } else {
    immlock = 1;
    send_to_char("Immortals are now locked out.\r\n", ch);
  }
}


ACMD(do_wizlock)
{
  int value;
  char *when;

  one_argument(argument, arg);
  if (*arg) {
    value = atoi(arg);
    if (value < 0 || value > GET_LEVEL(ch)) {
      send_to_char("Invalid wizlock value.\r\n", ch);
      return;
    }
    restrict_run = value;
    when = "now";
  } else
    when = "currently";

  switch (restrict_run) {
  case 0:
    sprintf(buf, "The game is %s completely open.\r\n", when);
    break;
  case 1:
    sprintf(buf, "The game is %s closed to new players.\r\n", when);
    break;
  default:
    sprintf(buf, "Only level %d and above may enter the game %s.\r\n",
        restrict_run, when);
    break;
  }
  send_to_char(buf, ch);
}

ACMD(do_timelock)
{
  int i;

  /* A quick hack :) */

  i = time_restrict;

  if (!i) {
   send_to_char("Time messages will _not_ be shown from now on.\r\n", ch);
   time_restrict = 1;
  }

  if (i) {
   send_to_char("Time messages will be shown from now on.\r\n", ch);
   time_restrict = 0;
  }
}

ACMD(do_questlock)
{
  if(!questlock)
     {
     questlock=1;
     send_to_char("Alright, the quest is now locked for new players.\r\n", ch);
     }
  else
     {
     questlock=0;
     send_to_char("Alright, the quest is now open for new players.\r\n", ch);
     }
}

ACMD(do_authorize)
{

  struct char_data *vict;
  /*struct descriptor_data *pt; */

  one_argument(argument, buf);

  if (!*buf)  {                 
    send_to_char("Authorize who?\r\n",ch);
    return;
  }
  if (!(vict = get_char_vis(ch, buf))) {
    send_to_char("There is no player with that name logged on.\r\n", ch);
    return;
  } 
  if (IS_NPC(vict)) {
    send_to_char("Mobs' names are just fine!\r\n",ch);
    return;
  }
  if (GET_AUTHORIZED(vict)) {
    asend_to_char(ch, "%s has already been authorized!\r\n",GET_NAME(vict));
    return;
  }
  GET_AUTHORIZED(vict) = TRUE;
  asend_to_char(ch, "%s is now authorized!\r\n", GET_NAME(vict));
  asend_to_char(vict, "Your name has been authorized by %s!\r\n"
            "Welcome to the \\c08Dragon's Fang\\c00!\r\n"
                      "If you have any questions regarding the MUD, you may use the \\c01Question\\c00 channel,\r\n"
            "Or type who -j to see a list of available Newbie Helpers.\r\n"
                      "Type: \\c02HELP NEWBIE\\c00 for more info.\r\n", GET_NAME(ch));
  REMOVE_BIT(PRF2_FLAGS(ch),PRF2_NOQUEST);
  sprintf(buf,"%s has authorized %s's name.", GET_NAME(ch), GET_NAME(vict));
  xlog(SYS_GOD, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), buf, TRUE);
}

ACMD(do_unauthorize)
{

  struct char_data *vict;
  
  one_argument(argument, buf);
     
    if (!*buf)
       {
       send_to_char("Unauthorize who?\r\n",ch);
       return;
       }
    if ((vict = get_char_vis(ch, buf)))
       {
      if (!GET_AUTHORIZED(vict))
         {
         asend_to_char(ch, "%s isn't authorized anyway!\r\n",GET_NAME(vict));
         return;
         }
      else
         {
         GET_AUTHORIZED(vict)=FALSE;
         asend_to_char(ch, "%s is now unauthorized!\r\n", GET_NAME(vict));
         asend_to_char(vict, "Your name has been unauthorized by %s!\r\n", GET_NAME(ch));
         sprintf(buf,"%s has unauthorized %s's name.", GET_NAME(ch), GET_NAME(vict));
         xlog(SYS_GOD, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), buf, TRUE);
         }
      }   
    else  
         send_to_char("Unauthorize who?\r\n",ch);
         
}  

ACMD(do_date)
{
  char *tmstr = NULL, *hbtmstr = NULL, *temp = NULL;
  time_t mytime;
  time_t hbmytime;
  time_t currtime = time(0);
  int d, h, m, hbd, hbh, hbm;

  if (subcmd == SCMD_DATE)
    mytime = currtime;
  else {
    mytime = boot_time;
    hbmytime = hotboot_time;
    temp = (char *) asctime(localtime(&hbmytime));
    *(temp + strlen(temp) - 1) = '\0';
    hbtmstr = strdup(temp);
  }

  temp = (char *) asctime(localtime(&mytime));
  *(temp + strlen(temp) - 1) = '\0';
  tmstr = strdup(temp);
  
  if (subcmd == SCMD_DATE)
    sprintf(buf, "Current machine time: %s\r\n", tmstr);
  else {
    mytime = currtime - boot_time;
    d = mytime / 86400;
    h = (mytime / 3600) % 24;
    m = (mytime / 60) % 60;

    hbmytime = currtime - hotboot_time;
    hbd = hbmytime / 86400;
    hbh = (hbmytime / 3600) % 24;
    hbm = (hbmytime / 60) % 60;

    if (hbmytime != mytime)
      sprintf(buf, "Up since %s: %d day%s, %d:%02d (Full reboot)\r\n"
           "         %s: %d day%s, %d:%02d (Hotboot)\r\n", tmstr, d,
         ((d == 1) ? "" : "s"), h, m, hbtmstr, hbd, ((hbd == 1) ? "" : "s"),
         hbh, hbm);
    else
      sprintf(buf, "Up since %s: %d day%s, %d:%02d\r\n", tmstr, d,
             ((d == 1) ? "" : "s"), h, m);
  }

  send_to_char(buf, ch);
}



ACMD(do_last)
{
  struct char_data *chdata;
  char arg1[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH], name[MAX_INPUT_LENGTH];
  int old = FALSE;

  two_arguments(argument, arg1, arg2);

  if (!*arg1) {
    send_to_char("Usage: last [old] <name>\r\n", ch);
    return;
  }

  if (!*arg2)
    strcpy(name, arg1);
  else if (is_abbrev(arg1, "old")) {
    old = TRUE;
    strcpy(name, arg2);
  }
  else
    strcpy(name, argument);

  if (NULL == (chdata = raw_load(name, old))) {
    send_to_char("There is no such player.\r\n", ch);
    return;
  }

  if ((GET_LEVEL(chdata) > GET_LEVEL(ch) && (GET_LEVEL(ch) < LVL_IMPL))) {
    send_to_char("You are not sufficiently godly for that!\r\n", ch);
    return;
  }
    
  sprintf(buf, "[%5ld] [%3ld %s] %-12s : %-18s : %-20s\r\n",
      GET_IDNUM(chdata),GET_LEVEL(chdata),class_abbrevs[(int)GET_CLASS(chdata)],
      GET_NAME(chdata),chdata->tempHost, ctime(&chdata->player.last_logon)); //.time.logon));
  free_char(chdata);

  send_to_char(buf, ch);
}


ACMD(do_force)
{
  struct descriptor_data *i, *next_desc;
  struct char_data *vict, *next_force;
  char to_force[MAX_INPUT_LENGTH + 2];

  if ((GET_LEVEL(ch) < LVL_QUESTOR) && (!GET_DREAMENTERED(ch))) {
      send_to_char("What ?!\r\n", ch);
      return;
  }

  half_chop(argument, arg, to_force);

  sprintf(buf1, "$n has forced you to '%s'.", to_force);

  if (!*arg || !*to_force)
    send_to_char("Whom do you wish to force do what?\r\n", ch);
  else if ((GET_LEVEL(ch) < LVL_GROOVYB) || (str_cmp("all", arg) && str_cmp("room", arg))) {
    if (!(vict = get_char_vis(ch, arg)))
      send_to_char(NOPERSON, ch);
    else if ((GET_LEVEL(ch) <= GET_LEVEL(vict)) && (!(GET_DREAMENTERED(ch))))
      send_to_char("No, no, no!\r\n", ch);
    else if (PLR_FLAGGED(vict, PLR_WRITING) || PLR_FLAGGED(vict, PLR_MAILING))
      asend_to_char(ch, "Wait until %s has stopped writing first.\r\n", GET_NAME(vict));
    else {
      send_to_char(OK, ch);
      act(buf1, TRUE, ch, NULL, vict, TO_VICT);
      sprintf(buf, "%s forced %s to %s", GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch), GET_NAME(vict), to_force);
      xlog(SYS_GOD, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), buf, TRUE);
      command_interpreter(vict, to_force);
    }
  } else if (!str_cmp("room", arg)) {
    send_to_char(OK, ch);
    sprintf(buf, "%s forced room %d to %s", GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch), world[ch->in_room].number, to_force);
    xlog(SYS_GOD, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), buf, TRUE);

    for (vict = world[ch->in_room].people; vict; vict = next_force) {
      next_force = vict->next_in_room;
      if (GET_LEVEL(vict) >= GET_LEVEL(ch))
    continue;
      if (PLR_FLAGGED(vict, PLR_WRITING) || PLR_FLAGGED(vict, PLR_MAILING))
         continue;
      act(buf1, TRUE, ch, NULL, vict, TO_VICT);
      command_interpreter(vict, to_force);
    }
  } else { /* force all */
    send_to_char(OK, ch);
    sprintf(buf, "%s forced all to %s", GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch), to_force);
    xlog(SYS_GOD, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), buf, TRUE);

    for (i = descriptor_list; i; i = next_desc) {
      next_desc = i->next;

      if (i->connected || !(vict = i->character) || GET_LEVEL(vict) >= GET_LEVEL(ch))
    continue;
      if (PLR_FLAGGED(vict, PLR_WRITING) || PLR_FLAGGED(vict, PLR_MAILING))
         continue;
      act(buf1, TRUE, ch, NULL, vict, TO_VICT);
      command_interpreter(vict, to_force);
    }
  }
}

/* 
 * Makes the bond references in playertable update to the bonds ch currently is holding.
 */
void update_bonds_in_playertable(struct char_data *ch)
{
  int i;
  
  if (!ch)
  {
    return;
  }
  for (i = 0; i < top_of_p_table; i ++)
  {
    if (player_table[i].id == GET_IDNUM(ch)) 
    {
      player_table[i].bonded = GET_BONDED(ch);
      player_table[i].minion = GET_MINION(ch);
      player_table[i].ashabonded = GET_ASHABONDED(ch);
      break;
    }
  }
}

/*
 * Finds the character named exactly as given as argument, either in playerlist or file.
 * If none is found, null is returned, else a pointer to the char found.
 * If player is found in file, in_file is set to non-zero.
 *
 * Warning: You must free the returned char struct if the player found was in file.
 */
struct char_data *get_char_exact_anywhere(char *name, int *in_file)
{
  struct char_data *temp;
   
  *in_file = 0;
   
  if (!name || !*name) {
    return NULL;
  }
    
  // Search through those logged on first, to save cpu time. 
  for (temp = character_list; temp; temp = temp->next)
    if (!strcasecmp(GET_NAME(temp), name))
      return temp;

  *in_file = 1;
  if (NULL == (temp = xmlload_char(name))) {
    *in_file = 0; 
    return NULL;
  }
  return temp;
}

/*
 * Removes bonds on a character.
 * First arg is the char to remove bonds on, the second is what type of bonds to remove:
 * "sedai" means that all sedai bonds that this character has to any other character are removed.
 * "asha"  ---||--------  asha  ---------------------------||----------------------------------
 * "forsaken" ---||----- forsaken ---------------------------||--------------------------------
 * The function seaches through the playerfiles as well, and updates the playertable to reflect the 
 * bond removal.
 */
void remove_bond(struct char_data *ch, char *what)
{
  int file, i;
  struct char_data *vict;
  
  if (!ch ||!what || !*what)
  {
    return;
  }
  
  if (!strncasecmp("sedai", what, strlen(what)) && GET_BONDED(ch))
  {
    for (i = 0; i < top_of_p_table; i ++)
    {
      if ((player_table[i].bonded & ~(1 << 30)) == (GET_BONDED(ch) & ~(1 << 30))) 
      {
        if (!(vict = get_char_exact_anywhere(player_table[i].name, &file)))
          continue;
        if (!(GET_BONDED(vict) & (1 << 30))) // must not remove all bonds on vict if vict is bondholder
        {
          GET_BONDED(vict) = 0;
          player_table[i].bonded = 0; // update player table.          
          save_char(vict, NOWHERE); // only need to save vict if vict is changed
        }
        if (file) // vict wasn't logged on, free the character
        {
          free_char(vict);
        }
      }
    }
    GET_BONDED(ch) = 0;
  } // sedai bond removal
  else if (!strncasecmp("asha", what, strlen(what)) && GET_ASHABONDED(ch))
  {
    for (i = 0; i < top_of_p_table; i ++)
    {
      if ((player_table[i].ashabonded & ~(1 << 30)) == (GET_ASHABONDED(ch) & ~(1 << 30))) 
      {
        if (!(vict = get_char_exact_anywhere(player_table[i].name, &file)))
          continue;
        if (!(GET_ASHABONDED(vict) & (1 << 30))) // must not remove all bonds on vict if vict is bondholder
        {
          GET_ASHABONDED(vict) = 0;
          player_table[i].ashabonded = 0; // update player table.
          save_char(vict, NOWHERE); // only need to save vict if vict is changed
        }
        if (file) // vict wasn't logged on, free the character
        {
          free_char(vict);
        }
      }
    }
    GET_ASHABONDED(ch) = 0; // finally, remove ch's connection to all other asha bonds.
  }// asha bond removal
  else if (!strncasecmp("forsaken", what, strlen(what)) && GET_MINION(ch))
  {
    for (i = 0; i < top_of_p_table; i ++)
    {
      if ((player_table[i].minion & ~(1 << 30)) == (GET_MINION(ch) & ~(1 << 30))) 
      {
        if (!(vict = get_char_exact_anywhere(player_table[i].name, &file)))
          continue;
        if (!(GET_MINION(vict) & (1 << 30))) // must not remove all bonds on vict if vict is bondholder
        {
          GET_MINION(vict) = 0;
          player_table[i].minion = 0; // update player table.
          save_char(vict, NOWHERE); // only need to save vict if vict is changed
        }
        if (file) // vict wasn't logged on, free the character
        {
          free_char(vict);
        }
      }
    }
    GET_MINION(ch) = 0;
  }// forsaken bond removal

  update_bonds_in_playertable(ch);
  save_char(ch, NOWHERE);
}

ACMD(do_clearbond)
{  
  struct char_data *victim = NULL, *victim2 = NULL;
  char targetname[MAX_INPUT_LENGTH], whattodo[MAX_INPUT_LENGTH];
  int i, in_file = 0, in_file2 = 0;

  if (!*argument) {
    send_to_char("Syntax: clearbond [targetname] [\"all\" | othertargetname | \"asha\" | \"sedai\" | \"forsaken\"] \r\n", ch);
    return;
  }
  two_arguments(argument, targetname, whattodo);
  victim = get_char_exact_anywhere(targetname, &in_file);
  
  if (!victim)
  {
    asend_to_char(ch, "There is nobody named '%s' anywhere.\r\n", targetname);
    if (in_file)
    {
      free(victim);
    }
    return;
  }
  
  if (GET_LEVEL(ch) < GET_LEVEL(victim))
  {
    asend_to_char(ch, "%s is a higher level than you -> DENIED!!\r\n", GET_NAME(victim));
    if (in_file)
    {
      free(victim);
    }
    return;
  }
  
  // Ok, target found, now for argument #2...
  if (!strncasecmp("all", whattodo, 3))
  {
    asend_to_char(ch, "Removing all bonds on %s.\r\n", GET_NAME(victim));
    remove_bond(victim, "asha");
    remove_bond(victim, "sedai");
    remove_bond(victim, "forsaken");
    if (in_file)
    {
      free(victim);
    }
  }
  else if (!strncasecmp("asha", whattodo, strlen(whattodo)) || 
           !strncasecmp("sedai", whattodo, strlen(whattodo)) ||
           !strncasecmp("forsaken", whattodo, strlen(whattodo)))
  {
    asend_to_char(ch, "Removing '%s' bond on %s.\r\n", whattodo, GET_NAME(victim));
    remove_bond(victim, whattodo);
    save_char(victim, NOWHERE);
    if (in_file)
    {
      free(victim);
    }
  }
  else // find someone bonded to victim
  {
    if ((victim2 = get_char_exact_anywhere(whattodo, &in_file2)) == NULL)
    {
      asend_to_char(ch, "There is nobody named '%s' anywhere.\r\n", whattodo);
      if (in_file)
      {
        free_char(victim);
      }
      return;
    }
    if ((GET_LEVEL(victim) > GET_LEVEL(ch)) || (GET_LEVEL(victim2) > GET_LEVEL(ch)))
    {
      asend_to_char(ch, "You can't do that, either %s or %s is higher level than you.\r\n", GET_NAME(victim), GET_NAME(victim2));
      if (in_file) // not logged on, must free player.
      {
        free_char(victim);
      }
      if (in_file2) //not logged on, must free player.
      {
        free_char(victim2);
      }
      return;
    }
    asend_to_char(ch, "Attempting to remove all bonds between %s and %s.\r\n", GET_NAME(victim), GET_NAME(victim2));
    // Time to see if either holds any bond on the other.  
    if (GET_BONDED(victim) && GET_BONDED(victim2)) // Both have some sort of sedai bond
    {
      if ((GET_BONDED(victim) & 1 << 30) != (GET_BONDED(victim2) & 1 << 30)) // Only one holds a bond
      {
        if (GET_BONDED(victim) & 1 << 30) // victim is bondholder, but is he holding victim2's bond?
        {
          i = GET_BONDED(victim);
          REMOVE_BIT(i, 1 << 30);
          if (GET_BONDED(victim2) == i) //victim holds victim2's bond
          {
            GET_BONDED(victim2) = 0;
            alog("%s has had %s unbonded as her warder by %s.", GET_NAME(victim), GET_NAME(victim2), GET_NAME(ch));
            asend_to_char(ch, "Removing %s's Sedai bond on %s.\r\n", GET_NAME(victim), GET_NAME(victim2));      
          }
        }
        else // victim2 is bondholder, but is he holding victim's bond?
        {
          i = GET_BONDED(victim2);
          REMOVE_BIT(i, 1 << 30);
          if (GET_BONDED(victim) == i) //victim2 holds victim's bond
          {
            GET_BONDED(victim) = 0;
            alog("%s has had %s unbonded as her warder by %s.", GET_NAME(victim2), GET_NAME(victim), GET_NAME(ch));
            asend_to_char(ch, "Removing %s's Sedai bond on %s.\r\n", GET_NAME(victim2), GET_NAME(victim));
          }          
        }
      }  
    } // Sedai bond release
    
    if (GET_ASHABONDED(victim) && GET_ASHABONDED(victim2)) // Both have some sort of asha bond
    {
      if ((GET_ASHABONDED(victim) & 1 << 30) != (GET_ASHABONDED(victim2) & 1 << 30)) // Only one holds a bond
      {
        if (GET_ASHABONDED(victim) & 1 << 30) // victim is bondholder, but is he holding victim2's bond?
        {
          i = GET_ASHABONDED(victim);
          REMOVE_BIT(i, 1 << 30);
          if (GET_ASHABONDED(victim2) == i) //victim holds victim2's bond
          {
            GET_ASHABONDED(victim2) = 0;
            asend_to_char(ch, "Removing %s's Asha'man bond on %s.\r\n", GET_NAME(victim), GET_NAME(victim2));      
          }
        }
        else // victim2 is asha bondholder, but is he holding victim's bond?
        {
          i = GET_ASHABONDED(victim2);
          REMOVE_BIT(i, 1 << 30);
          if (GET_ASHABONDED(victim) == i) //victim2 holds victim's bond
          {
            GET_ASHABONDED(victim) = 0;
            asend_to_char(ch, "Removing %s's Asha'man bond on %s.\r\n", GET_NAME(victim2), GET_NAME(victim));
          }          
        }
      }
    } // Asha bond check
    
    if (GET_MINION(victim) && GET_MINION(victim2)) // Both have some sort of minion bond
    {
      if ((GET_MINION(victim) & 1 << 30) != (GET_MINION(victim2) & 1 << 30)) // Only one holds a bond
      {
        if (GET_MINION(victim) & 1 << 30) // victim is bondholder, but is he holding victim2's bond?
        {
          i = GET_MINION(victim);
          REMOVE_BIT(i, 1 << 30);
          if (GET_MINION(victim2) == i) // victim holds victim2's minion bond
          {
            GET_MINION(victim2) = 0;
            asend_to_char(ch, "Removing %s's Minion bond on %s.\r\n", GET_NAME(victim), GET_NAME(victim2));      
          }
        }
        else // victim2 is bondholder, but is he holding victim's bond?
        {
          i = GET_MINION(victim2);
          REMOVE_BIT(i, 1 << 30);
          if (GET_MINION(victim) == i) // victim2 holds victim's minion bond
          {
            GET_MINION(victim) = 0;
            asend_to_char(ch, "Removing %s's Minion bond on %s.\r\n", GET_NAME(victim2), GET_NAME(victim));
          }
        }
      }  
    } // minion bond check
    
    update_bonds_in_playertable(victim);
    update_bonds_in_playertable(victim2);
    update_bonds_in_playertable(ch);
    if (in_file) // must free victim if he was found in playerfiles
    {
      free_char(victim);
    } 
    if (in_file2) // must free victim2 if he was found in playerfiles
    {
      free_char(victim2);
    }
  }
  send_to_char("Done.\r\n", ch);
}


ACMD(do_tedit) {
  int l, i;
  char field[MAX_INPUT_LENGTH];

  struct editor_struct {
    char *cmd;
    char level;
    char *buffer;
    int  size;
    char *filename;
  } fields[] = {
    /* edit the lvls to your own needs */
        { "credits",    LVL_IMPL,       credits,        2400,   CREDITS_FILE},
        { "news",       LVL_GRGOD,      news,           8192,   NEWS_FILE},
        { "motd",       LVL_GRGOD,      motd,           2400,   MOTD_FILE},
        { "imotd",      LVL_IMPL,       imotd,          2400,   IMOTD_FILE},
        { "help",       LVL_GRGOD,      help,           2400,   HELP_PAGE_FILE},
        { "info",       LVL_GRGOD,      info,           8192,   INFO_FILE},
        { "handbook",   LVL_IMPL,       handbook,       8192,   HANDBOOK_FILE},
        { "ideas",      LVL_GOD,        ideas,          8192,   IDEA_FILE},
        { "typos",      LVL_GOD,        typos,          8192,   TYPO_FILE},
        { "bugs",       LVL_GOD,        bugs,           8192,   BUG_FILE},
	{ "todo",       LVL_GOD,        todo,           8192,   TODO_FILE},
        { "oldnews",    LVL_GRGOD,      oldnews,        8192,   OLDNEWS_FILE},
	{ "wizlist",	LVL_GRGOD,	wizlist,	8192,	WIZLIST_FILE},
	{ "policy", 	LVL_GRGOD,	poli,	 	8192,	POLI_PAGE_FILE},
	{ "socialideas",LVL_GOD,        socialideas,    8192,   SOCIDEA_FILE},
	{ "cretodo",	LVL_GRGOD,	cretodo, 	8192,	CRETODO_FILE},
        { "newideas",   LVL_GOD,        newideas,       8192,   NEWIDEA_FILE},
        { "\n",         0,              NULL,           0,      NULL }
  };

  if (ch->desc == NULL) {
    send_to_char("Get outta here you linkdead head!\r\n", ch);
    return;
  }

  if (GET_LEVEL(ch) < LVL_GOD) {
    send_to_char("You do not have text editor permissions.\r\n", ch);
    return;
  }

  half_chop(argument, field, buf);

  if (!*field) {
    strcpy(buf, "Files available to be edited:\r\n");
    i = 1;
    for (l = 0; *fields[l].cmd != '\n'; l++) {
      if (GET_LEVEL(ch) >= fields[l].level) {
        sprintf(buf, "%s%-11.11s", buf, fields[l].cmd);
        if (!(i % 7)) strcat(buf, "\r\n");
        i++;
      }
    }
    if (--i % 7) strcat(buf, "\r\n");
    if (i == 0) strcat(buf, "None.\r\n");
    send_to_char(buf, ch);
    return;
  }

  for (l = 0; *(fields[l].cmd) != '\n'; l++)
    if (!strncmp(field, fields[l].cmd, strlen(field)))
      break;

  if (*fields[l].cmd == '\n') {
    send_to_char("Invalid text editor option.\r\n", ch);
    return;
  }

  if (GET_LEVEL(ch) < fields[l].level) {
    send_to_char("You are not godly enough for that!\r\n", ch);
    return;
  }

  switch (l) {
    case 0: ch->desc->str = &credits; break;
    case 1: ch->desc->str = &news; break;
    case 2: ch->desc->str = &motd; break;
    case 3: ch->desc->str = &imotd; break;
    case 4: ch->desc->str = &help; break;
    case 5: ch->desc->str = &info; break;
    case 6: ch->desc->str = &handbook; break;
    case 7: ch->desc->str = &ideas; break;
    case 8: ch->desc->str = &typos; break;
    case 9: ch->desc->str = &bugs; break;
    case 10: ch->desc->str = &todo; break;
    case 11: ch->desc->str = &oldnews; break;
    case 12: ch->desc->str = &wizlist; break;
    case 13: ch->desc->str = &poli; break;
    case 14: ch->desc->str = &socialideas; break;
    case 15: ch->desc->str = &cretodo; break;
    case 16: ch->desc->str = &newideas; break;
    default:
      send_to_char("Invalid text editor option.\r\n", ch);
      return;
  }

  /* set up editor stats */
  send_to_char("\x1B[H\x1B[J", ch);
  send_to_char("Edit file below: (/s saves /h for help)\r\n", ch);
  ch->desc->backstr = NULL;
  if (fields[l].buffer) {
    send_to_char(fields[l].buffer, ch);
    ch->desc->backstr = str_dup(fields[l].buffer);
  }
  ch->desc->max_str = fields[l].size;
  ch->desc->mail_to[0] = 0;
  CREATE(ch->desc->olc, struct olc_data, 1);
  OLC_STORAGE(ch->desc) = str_dup(fields[l].filename);
  act("$n begins editing a scroll.", TRUE, ch, 0, 0, TO_ROOM);
  ch->desc->current_text_line = count_newlines(ch->desc->str) + 1;
  SET_BIT(PLR_FLAGS(ch), PLR_WRITING);
  STATE(ch->desc) = CON_TEXTED;
}

ACMD(do_roomset) {
  int i;
  long mask = (ROOM_OLC | ROOM_BFS_MARK | ROOM_DREAM); /* ones we don't want set */
  
  if (IS_NPC(ch) || !ch->desc) {
    send_to_char("Go away!.\r\n", ch);
    return;
  }
  
  one_argument(argument, arg);
  
  i = search_block(arg, room_bits, TRUE);

  if (i == -1) {
    send_to_char("No such flag.\r\n", ch);
    return;
  }
  
  if ((1 << i) & mask) {
    send_to_char("Can't set that flag.\r\n", ch);
    return;
  }
  
  if (IS_SET(ROOM_FLAGS(ch->in_room), 1 << i)) {
    send_to_char("That flag is permamnent on this room. Change it in olc.\r\n", ch);
    return;
  }
  
  if (IS_SET(ROOM_TEMP_FLAGS(ch->in_room), 1 << i)) {
    sprintf(buf, "Room is no longer flagged %s\r\n", room_bits[i]);
    REMOVE_BIT(ROOM_TEMP_FLAGS(ch->in_room), 1 << i);
    send_to_char(buf, ch);
    return;
  }
  
  sprintf(buf, "Room is now flagged %s\r\n", room_bits[i]);
  SET_BIT(ROOM_TEMP_FLAGS(ch->in_room), 1 << i);
  send_to_char(buf, ch);
}

ACMD(do_revive) {
  struct char_data *tch = NULL;

  skip_spaces(&argument);

  if (!*argument) {
    send_to_char("Revive which character?\r\n", ch);
    return;
  }
  if ((tch = raw_load(argument, FALSE))) {
    send_to_char("A character already exists for that name! See a weaver about moving any old player to a different name.\r\n", ch);
    return;
  }
  if (!(tch = raw_load(argument, TRUE))) {
    send_to_char("No old character exists under that name.\r\n", ch);
    return;
  }
  GET_PFILEPOS(tch) = create_entry(GET_RNAME(tch));
  player_table[GET_PFILEPOS(tch)].id = GET_IDNUM(tch);
  tch->player.time.logon = time(0);
  xmlsave_char(tch);
  send_to_char("Player loaded successfully!\r\n", ch);
  sprintf(buf, "%s has revived %s.", GET_NAME(ch), GET_NAME(tch));
  xlog(SYS_REVIVE, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), buf, TRUE);
  free_char(tch);
}

ACMD(do_xlog) {
/* Extended Syslog
 *
 * Allows IMMs to set which syslog entries they wish to see.
 *
 * Will replace the old SYSLOG system instead of suplimenting
 * it such as the one Arcane Realms uses which this is based on.
 */

  long result;
  int l;
  char field[MAX_INPUT_LENGTH], value[MAX_INPUT_LENGTH];
  struct log_rights {
    char *cmd;
    int level;
    long cgs;
  } fields[] = {
    { "clone",		LVL_IMMORT,	0	},
    { "set", 		LVL_IMMORT, 	0	},
    { "playerlevels",	LVL_IMMORT,	0	},
    { "playerdeaths",	LVL_IMMORT,	0	},
    { "connections",	LVL_IMMORT,	0	},
    { "olc",		LVL_GRGOD,	0	},
    { "snoop",		LVL_IMMORT,	0	},
    { "zoneresets",	LVL_IMMORT,	0	},
    { "zoneerrors",	LVL_IMMORT,	0	},
    { "nohelp",		LVL_IMMORT,	0	},
    { "dgserrors",	LVL_IMMORT,	CG_DGS	},
    { "toolcraft",	LVL_GRGOD,	0	},
    { "load",		LVL_GRGOD,	0	},
    { "nopolicy",	LVL_GRGOD,	0	},
    { "guildieset",	LVL_GOD,	CG_GL	},
    { "guildset",	LVL_GOD,	CG_GL	},
    { "skillset",	LVL_GRGOD,	0	},
    { "playerreports",	LVL_IMMORT,	0	},
    { "award",		LVL_IMMORT,	0	},
    { "revive",		LVL_IMMORT,	0	},
    { "comm",		LVL_IMMORT,	0	},
    { "god",		LVL_IMMORT,	0	},
    { "retainer",	LVL_IMMORT,	0	},
    { "debug",		LVL_IMMORT,	CG_WEAVER	},
    { "systemerrors",	LVL_IMMORT,	0	},
    { "guilding",       LVL_GOD,        CG_GL   },

    /* Must be last */
    { "\n",		0,		0	}
  };


  if (!*argument)
  {
    send_to_char("USAGE: xlog <option>\r\n\r\n", ch);

    /*
     * First we'll show the 'All Imm' entries.
     */
    sprintf(buf, "&W[&CSYSLOG&W]\r\n"

                 "&c         CLONE : &w%-3s&n    "
                 "&c           SET : &w%-3s&n    "
                 "&c  PLAYERLEVELS : &w%-3s&n\r\n"

                 "&c  PLAYERDEATHS : &w%-3s&n    "
                 "&c   CONNECTIONS : &w%-3s&n    "
                 "&c         SNOOP : &w%-3s&n\r\n"

                 "&c    ZONERESETS : &w%-3s&n    "
                 "&c    ZONEERRORS : &w%-3s&n    "
                 "&c        NOHELP : &w%-3s&n\r\n"

                 "&c PLAYERREPORTS : &w%-3s&n    "
                 "&c         AWARD : &w%-3s&n    "
                 "&c        REVIVE : &w%-3s&n\r\n"

                 "&c          COMM : &w%-3s&n    "
		 "&c      RETAINER : &w%-3s&n    "
                 "&c           GOD : &w%-3s&n\r\n"

                 "&c  SYSTEMERRORS : &w%-3s&n    "
                 "&c               : &w%-3s&n    "
                 "&c               : &w%-3s&n\r\n\r\n",

                 ONOFF(GOT_SYSLOG(ch, LOG_CLONE)),  
                 ONOFF(GOT_SYSLOG(ch, LOG_SET)),  
                 ONOFF(GOT_SYSLOG(ch, LOG_PLEVEL)),  

                 ONOFF(GOT_SYSLOG(ch, LOG_PDEATH)),
                 ONOFF(GOT_SYSLOG(ch, LOG_CONN )),
                 ONOFF(GOT_SYSLOG(ch, LOG_SNOOP)),

                 ONOFF(GOT_SYSLOG(ch, LOG_ZRESET)),
                 ONOFF(GOT_SYSLOG(ch, LOG_ZERROR)),
                 ONOFF(GOT_SYSLOG(ch, LOG_NOHELP)),

                 ONOFF(GOT_SYSLOG(ch, LOG_PREPORT)),
                 ONOFF(GOT_SYSLOG(ch, LOG_AWARD)),
                 ONOFF(GOT_SYSLOG(ch, LOG_REVIVE)),

                 ONOFF(GOT_SYSLOG(ch, LOG_COMM)),
		 ONOFF(GOT_SYSLOG(ch, LOG_RETAINER)),
                 ONOFF(GOT_SYSLOG(ch, LOG_GOD)),

                 ONOFF(GOT_SYSLOG(ch, LOG_ERROR)),
                 "",
		 ""
          );
    send_to_char(buf, ch);

    if (IS_SET(ch->player_specials->saved.command_groups, CG_WEAVER )) {
      sprintf(buf, "&W[&CWEAVERS&W]\r\n"

                   "&c         DEBUG : &w%-3s&n\r\n\r\n",

                   ONOFF(GOT_SYSLOG(ch, LOG_DEBUG))
             );
      send_to_char(buf, ch);
    }

    if (IS_SET(ch->player_specials->saved.command_groups, CG_GL)) {
      sprintf(buf, "&W[&CGUILDLEADERS&W]\r\n"

                 "&c    GUILDIESET : &w%-3s&n    "
                 "&c      GUILDSET : &w%-3s&n    "
                 "&c      GUILDING : &w%-3s&n\r\n\r\n",

                 ONOFF(GOT_SYSLOG(ch, LOG_GUILDIESET)),
                 ONOFF(GOT_SYSLOG(ch, LOG_GUILDSET)),
                 ONOFF(GOT_SYSLOG(ch, LOG_GUILD))
            );
      send_to_char(buf, ch);
    }

    if (IS_SET(ch->player_specials->saved.command_groups, CG_DGS)) {
      sprintf(buf, "&W[&CDGS&W]\r\n"

                   "&c     DGSERRORS : &w%-3s&n\r\n\r\n",

                   ONOFF(GOT_SYSLOG(ch, LOG_DGSERROR))
             );
      send_to_char(buf, ch);
    }

    if (GET_LEVEL(ch) >= LVL_GRGOD) {
      sprintf(buf, "&W[&CADMIN&W]\r\n"

                   "&c     TOOLCRAFT : &w%-3s&n    "
                   "&c          LOAD : &w%-3s&n    "
                   "&c      NOPOLICY : &w%-3s&n\r\n"

                   "&c      SKILLSET : &w%-3s&n    "
                   "&c           OLC : &w%-3s&n    "
                   "&c               : &w%-3s&n\r\n",

                   ONOFF(GOT_SYSLOG(ch, LOG_TOOLCRAFT)),
                   ONOFF(GOT_SYSLOG(ch, LOG_LOAD)),
                   ONOFF(GOT_SYSLOG(ch, LOG_NOPOLICY)),

                   ONOFF(GOT_SYSLOG(ch, LOG_SKILLSET)),
                   ONOFF(GOT_SYSLOG(ch, LOG_OLC)),
                   ""

             );
      send_to_char(buf, ch);
    }
  return;
  }

  strcpy(arg, two_arguments(argument, field, value));

  for (l=0; *(fields[l].cmd) != '\n'; l++)
    if (!strncmp(field, fields[l].cmd, strlen(field)))
      break;

  if (*(fields[l].cmd) == '\n')
  {
    if( !strncmp( field, "stopall", strlen(field ) ) )
     EXTENDED_SYSLOG(ch) = 0;
    else
    {
      sprintf(buf,"Sorry, '%s' is not a valid syslog option!\r\n", field);
      send_to_char(buf, ch);
    }
    return;
  }

  if (GET_LEVEL(ch) >= fields[l].level)
  {
    if (fields[l].cgs != 0)
    {
      if (!IS_SET(ch->player_specials->saved.command_groups, fields[l].cgs))
      {
        send_to_char("Sorry, that's not a valid syslog option!\r\n", ch);
        return;
      }
    }
  } else {
    send_to_char("Sorry, that's not a valid syslog option!\r\n", ch);
    return;
  }    

  /*
   * If we're here they've selected a valid syslog option and have rights to it.
   */

  result = SYSL_TOG_CHK(ch, 1 << l);
  strcpy(buf1, fields[l].cmd);

  if (result)
    sprintf(buf, "%s syslog turned on.\r\n", CAP(buf1));
  else
    sprintf(buf, "%s syslog turned off.\r\n", CAP(buf1));
  send_to_char(buf, ch);
}
