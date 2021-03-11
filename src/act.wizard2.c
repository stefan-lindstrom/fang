/******************************************************************************
 * $Id: act.wizard2.c,v 1.30 2004/12/05 08:36:35 fang Exp $
 *   
 * File: act.wizard2.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: act.wizard2.c,v $
 *   Revision 1.30  2004/12/05 08:36:35  fang
 *   doh.  -tiek
 *
 *   Revision 1.29  2004/12/05 08:33:07  fang
 *   added a syslog message for checking flowcaps -tiek
 *
 *   Revision 1.28  2003/10/27 03:59:56  fang
 *   Minor tweaks to 'set list'
 *
 *   Revision 1.27  2003/10/26 02:51:19  fang
 *   Added 'list' option to do_set
 *
 *   Revision 1.26  2003/10/18 02:52:07  fang
 *   Tze: Added xlog message for CANMULTI
 *
 *   Revision 1.25  2003/08/14 03:15:03  fang
 *   Added nopray command.
 *
 *   Revision 1.24  2003/06/17 10:30:05  fang
 *
 *   Removed a new set of "externs" from source files. For fucks sake people,
 *   let's keep those externs where they belong, in an apropriate header, ok?
 *
 *   Revision 1.23  2003/06/07 07:18:43  fang
 *   Added cantgecho and nogecho to xlog for SET.
 *
 *   Revision 1.22  2003/05/28 09:04:26  fang
 *   Fixed syslog message for set nobility off.
 *
 *   Revision 1.21  2003/05/28 07:41:36  fang
 *   Setting someone's noble title off now removes their ability to hire and
 *   fire servants.
 *
 *   Revision 1.20  2003/05/22 02:18:40  fang
 *   The zap command (do_pzap) was only useful for harassing morts. By admin
 *   request, it has been disabled. Instead of removing it, I just commented
 *   it out. --Tze
 *
 *   Revision 1.19  2003/03/10 08:17:49  fang
 *   *** empty log message ***
 *
 *   Revision 1.18  2003/03/01 07:17:05  fang
 *   Fisch: Multiflag for authorized multies
 *
 *   Revision 1.17  2002/12/12 00:15:58  fang
 *   Fisch:  added "cantpost" to protect boards to the PLR_ flags
 *
 *   Revision 1.16  2002/11/10 22:18:34  fang
 *   Fisch: Changeed Tells message and onl display it on AFK not AFW
 *
 *   Revision 1.15  2002/11/10 09:20:32  fang
 *   fischer: added "show shop zone" patch to make "show shop" usefull
 *
 *   Revision 1.14  2002/11/07 04:02:01  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.13  2002/11/04 01:15:09  fang
 *   fisch: changed "zap" text from lightning to balefire
 *
 *   Revision 1.12  2002/11/03 08:57:31  fang
 *   fisch: changed the pzap text to be balefire, not lighting
 *
 *   Revision 1.11  2002/11/01 05:06:50  fang
 *   fisch: removed "show rent" since we don't have rent
 *
 *   Revision 1.10  2002/10/23 04:17:19  fang
 *   fisch: "list board" to list boards with new messages added
 *
 *   Revision 1.9  2002/09/27 09:15:04  fischer
 *   fischer: removing debuging code
 *
 *   Revision 1.8  2002/09/27 08:52:36  fischer
 *   fischer: minor bug fixes.. cosemetic stuff
 *
 *   Revision 1.7  2002/09/26 00:00:33  fischer
 *   Fisch: Added Channel list to public channels and made some changes towards
 *   making visible objects on invis admins, invisible to where.
 *
 *   Revision 1.6  2002/09/25 17:44:27  fischer
 *   fisch: fixed playerlist to list idents greater than 32k
 *
 *   Revision 1.5  2002/09/15 22:53:56  fang
 *   Drath: XLOG tag fix for SET and MCL update for BALANCE
 *
 *   Revision 1.4  2002/09/15 00:50:07  fang
 *   Drath: XLOG update - Categories added to xlog display automatically
 *
 *   Revision 1.3  2002/09/08 07:22:51  fang
 *   Drath: Finals on AEDIT CG
 *
 *   Revision 1.2  2002/07/28 13:56:03  fang
 *   Removed dmlloc...just to test.
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.12  2002/07/06 07:05:12  fang
 *   Drath: IMMLOCK for Fred and 2nd to last XLOG commit
 *
 *   Revision 1.11  2002/06/29 18:33:04  fang
 *   Drath: In-MUD syslog searching.  Replaces websyslog search
 *
 *   Revision 1.10  2002/06/24 14:51:40  fang
 *   *** empty log message ***
 *
 *   Revision 1.9  2002/06/24 14:29:54  fang
 *   Drath: Added CANTGECHO setting to per-user disable GECHO
 *
 *   Revision 1.8  2002/06/04 09:36:31  fang
 *   *** empty log message ***
 *
 *   Revision 1.7  2002/05/27 10:52:48  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.6  2002/05/23 22:50:17  fang
 *   Maz: Newbie death message and other stuff...
 *
 *   Revision 1.4  2002/05/06 02:20:20  fang
 *   Mazrim: fixed Ajahset to check off guild 53
 *
 *   Revision 1.3  2002/02/27 02:17:40  fang
 *   Fixed disguise so it saves.
 *
 *   Revision 1.84  2002/01/24 21:33:13  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.83.2.4  2002/01/23 21:49:38  mud
 *   Removed objsave.[c|h] (old objsave stuff).
 *   Removed obj_file_elem and rent structs.
 *   Removed final vestiges of old guild system, except parts used
 *   in guild.[c|h] for backwards compatibility.
 *
 *   Revision 1.83.2.3  2002/01/22 20:50:57  mud
 *   Removed boats and info system. Obsolete, unused code with no value.
 *   Fixed bug in portal_stone SPECIAL. Now checks that a room exists
 *   before attempting to transf chars there. :)
 *
 *   Revision 1.83.2.2  2002/01/22 03:05:11  mud
 *   More cleanup. Removed extern decls from source files. Protected some 
 *   DG headers from multiple inclision
 *
 *   Revision 1.83.2.1  2002/01/21 01:23:40  mud
 *   Code cleanup. Aiel clans/socities gone. Lot of old guild code gone.
 *
 *   Revision 1.83  2002/01/13 21:28:25  mud
 *   Default desc changes name, when character changes name...
 *
 *****************************************************************************/

/* ************************************************************************
*   File: act.wizard2.c                                 Part of CircleMUD *
*  Usage: Player-level god commands and other goodies                     *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */
#include "config.h"
#include "shop.h"
#include "act.h"
#include "conf.h"
#include "sysdep.h"
#include "constants.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "spells.h"
#include "screen.h"
#include "olc.h"
#include "class.h"
#include "xmlcharsave.h"
#include "guild.h"
#include "guild_parser.h"
#include "xmlobjsave.h"
#include <malloc.h>
#include "dg_scripts.h"
#include "portstone.h"
#include "boards.h"


ACMD(do_isquest) 
{
  struct obj_data *obj;

  skip_spaces(&argument);
  if (!*argument) {
    send_to_char("Checki if \\c01what\\c00 item is an quest item?\r\n",ch);
    return;
  }
  if (!(obj = get_obj_in_list_vis(ch,argument,ch->carrying))) {
    send_to_char("You ain`t carrying that!\r\n",ch);
    return;
  }
  if (IS_QUEST_ITEM(obj)) {
    asend_to_char(ch,"The item %s is a Quest item with cost of %d QPs\r\n",obj->short_description,IS_QUEST_ITEM(obj));
  } else {
    asend_to_char(ch,"The item %s is \\c01NOT\\c00 a Quest item.\r\n",obj->short_description);
  }
}

ACMD(do_wiznet)
{
  struct descriptor_data *d;
  char emote = FALSE;
  char any = FALSE;
  int level = LVL_IMMORT;
  char *nameptr = NULL;

  skip_spaces(&argument);
  delete_doubledollar(argument);

  if (!*argument) {
    send_to_char("Usage: wiznet <text> | #<level> <text>\r\n "
         "       wiznet @<level> *<emotetext> | wiz @\r\n", ch);
    return;
  }
  switch (*argument) {
  case '#':
    one_argument(argument + 1, buf1);
    if (is_number(buf1)) {
      half_chop(argument+1, buf1, argument);
      level = MAX(atoi(buf1), LVL_IMMORT);
      if (level > GET_LEVEL(ch)) {
    send_to_char("You can't wizline above your own level.\r\n", ch);
    return;
      }
    } else if (emote)
      argument++;
    break;
  case '@':
    for (d = descriptor_list; d; d = d->next) {
      if (!d->connected && GET_LEVEL(d->character) >= LVL_IMMORT &&
      !PRF_FLAGGED(d->character, PRF_NOWIZ) &&
      (CAN_SEE(ch, d->character) || GET_LEVEL(ch) == LVL_IMPL)) {
    if (!any) {
      sprintf(buf1, "Gods online:\r\n");
      any = TRUE;
    }
    nameptr = ((GET_DISGUISED(d->character)) ? GET_RNAME(d->character) : GET_NAME(d->character));
        if(!GET_DISGUISED(d->character))
       sprintf(buf1, "%s  %s", buf1,  nameptr);
        else
           sprintf(buf1, "%s %s (%s)", buf1, nameptr, GET_NAME(d->character));
    if (PLR_FLAGGED(d->character, PLR_WRITING))
      sprintf(buf1, "%s (Writing)\r\n", buf1);
    else if (PLR_FLAGGED(d->character, PLR_MAILING))
      sprintf(buf1, "%s (Writing mail)\r\n", buf1);
    else
      sprintf(buf1, "%s\r\n", buf1);

      }
    }
    any = FALSE;
    for (d = descriptor_list; d; d = d->next) {
      if (!d->connected && GET_LEVEL(d->character) >= LVL_IMMORT &&
      PRF_FLAGGED(d->character, PRF_NOWIZ) &&
      CAN_SEE(ch, d->character)) {
        nameptr = ((GET_DISGUISED(d->character)) ? GET_RNAME(d->character) : GET_NAME(d->character));
    if (!any) {
      sprintf(buf1, "%sGods offline:\r\n", buf1);
      any = TRUE;
    }
    sprintf(buf1, "%s  %s\r\n", buf1, nameptr);
      }
    }
    send_to_char(buf1, ch);
    return;
    break;
  case '\\':
    ++argument;
    break;
  default:
    break;
  }
  if (PRF_FLAGGED(ch, PRF_NOWIZ)) {
    send_to_char("You are offline!\r\n", ch);
    return;
  }
  skip_spaces(&argument);

  if (!*argument) {
    send_to_char("Don't bother the gods like that!\r\n", ch);
    return;
  }

  nameptr = ((GET_DISGUISED(ch)) ? GET_RNAME(ch) : GET_NAME(ch));

  if(GET_DISGUISED(ch))
     {
     if (level > LVL_IMMORT) 
        {
        sprintf(buf1, "%s (%s): <%d> %s%s\r\n", nameptr , GET_NAME(ch), level, emote ? "<--- " : "", argument);
        sprintf(buf2, "Someone: <%d> %s%s\r\n", level, emote ? "<--- " : "", argument);
        } 
     else 
        {
        sprintf(buf1, "%s (%s): %s%s\r\n", nameptr, GET_NAME(ch), emote ? "<--- " : "",argument);
        sprintf(buf2, "Someone: %s%s\r\n", emote ? "<--- " : "", argument);
        }
     }
 
   else

  {
  if (level > LVL_IMMORT) {
    sprintf(buf1, "%s: <%d> %s%s\r\n", nameptr , level,
        emote ? "<--- " : "", argument);
    sprintf(buf2, "Someone: <%d> %s%s\r\n", level, emote ? "<--- " : "",
        argument);
  } else {
    sprintf(buf1, "%s: %s%s\r\n", nameptr, emote ? "<--- " : "",
        argument);
    sprintf(buf2, "Someone: %s%s\r\n", emote ? "<--- " : "", argument);
  }
  }

  for (d = descriptor_list; d; d = d->next) {
    if ((!d->connected) && (GET_LEVEL(d->character) >= level) &&
    (!PRF_FLAGGED(d->character, PRF_NOWIZ)) &&
    (!PLR_FLAGGED(d->character, PLR_WRITING | PLR_MAILING))
    && (d != ch->desc || !(PRF_FLAGGED(d->character, PRF_NOREPEAT)))) {
      send_to_char(CCCYN(d->character, C_NRM), d->character);
      if (CAN_SEE(d->character, ch))
    send_to_char(buf1, d->character);
      else
    send_to_char(buf2, d->character);
      send_to_char(CCNRM(d->character, C_NRM), d->character);
    }
  }

  if (PRF_FLAGGED(ch, PRF_NOREPEAT))
    send_to_char(OK, ch);
}

ACMD(do_ginvis)  
{
   struct char_data *tch;

   if (!GET_GUILD_INVIS(ch))
     {
     send_to_char("You will now be invisible to all but your guildies.\r\n",ch);
     GET_GUILD_INVIS(ch)=TRUE;
     }
   else
   if (GET_GUILD_INVIS(ch))
     {
     send_to_char("Guildinvisible is now off.\r\n",ch);
     GET_GUILD_INVIS(ch)=FALSE;
     }

   for (tch = world[ch->in_room].people; tch; tch = tch->next_in_room) {
    if (tch == ch)
      continue;
    if (!CAN_SEE(tch,ch))
      act("You blink and suddenly realize that $n is gone.", FALSE, ch, 0,
          tch, TO_VICT);
    if (CAN_SEE(tch,ch))
      act("You suddenly realize that $n is standing beside you.", FALSE, ch, 0,
          tch, TO_VICT);
    }
}

ACMD(do_zreset)
{
  int i, j;

  one_argument(argument, arg);
  if (!*arg) {
    send_to_char("You must specify a zone.\r\n", ch);
    return;
  }
  if (*arg == '*') {
    for (i = 0; i <= top_of_zone_table; i++)
      reset_zone(i);
    send_to_char("Reset world.\r\n", ch);
    sprintf(buf, "%s reset entire world.", GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch));
    xlog(SYS_OLC, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), buf, TRUE);
    return;
  } else if (*arg == '.')
    i = world[ch->in_room].zone;
  else {
    j = atoi(arg);
    for (i = 0; i <= top_of_zone_table; i++)
      if (zone_table[i].number == j)
    break;
  }
  if (i >= 0 && i <= top_of_zone_table) {
    reset_zone(i);
    sprintf(buf, "Reset zone %d (#%d): %s.\r\n", i, zone_table[i].number,
        zone_table[i].name);
    send_to_char(buf, ch);
    sprintf(buf, "%s reset zone %d (%s)", GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch), i, zone_table[i].name);
    xlog(SYS_OLC, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), buf, TRUE);
  } else
    send_to_char("Invalid zone number.\r\n", ch);
}


/*
 *  General fn for wizcommands of the sort: cmd <player>
 */

ACMD(do_wizutil)
{
  struct char_data *vict;
  long result;

  one_argument(argument, arg);

  if (!*arg)
    send_to_char("Yes, but for whom?!?\r\n", ch);
  else if (!(vict = get_char_vis(ch, arg)))
    send_to_char("There is no such player.\r\n", ch);
  else if (IS_NPC(vict))
    send_to_char("You can't do that to a mob!\r\n", ch);
  else if (GET_LEVEL(vict) >= GET_LEVEL(ch))
    send_to_char("Hmmm...you'd better not.\r\n", ch);
  else 
  {
    switch (subcmd) 
    {
    case SCMD_NOTITLE:
      result = PLR_TOG_CHK(vict, PLR_NOTITLE);
      sprintf(buf, "Notitle %s for %s by %s.", ONOFF(result),
          GET_NAME(vict), GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch));
      xlog(SYS_SET, MAX(LVL_GOD, GET_INVIS_LEV(ch)), buf, TRUE);
      strcat(buf, "\r\n");
      send_to_char(buf, ch);
      break;
    case SCMD_SQUELCH:
      result = PLR_TOG_CHK(vict, PLR_NOSHOUT);
      TOGGLE_BIT(PLR_FLAGS(vict), PLR_NOTELL);
      sprintf(buf, "Mute %s for %s by %s.", ONOFF(result),
          GET_NAME(vict), GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch));
      xlog(SYS_SET, MAX(LVL_GOD, GET_INVIS_LEV(ch)), buf, TRUE);
      strcat(buf, "\r\n");
      send_to_char(buf, ch);
      sprintf(buf, "&R%s has just set your mute to %s.&n\r\n", GET_NAME(ch), ONOFF(result));
      send_to_char(buf, vict); 
      break;
    case SCMD_NOPRAY:
      result = PLR_TOG_CHK(vict, PLR_NOPRAY);
      sprintf(buf, "Nopray %s for %s by %s.", ONOFF(result),
	  GET_NAME(vict), GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch));
      xlog(SYS_SET, MAX(LVL_GOD, GET_INVIS_LEV(ch)), buf, TRUE);
      strcat(buf, "\r\n");
      send_to_char(buf, ch);
      sprintf(buf, "&R%s has just set your nopray to %s.&n\r\n", GET_NAME(ch), ONOFF(result));
      send_to_char(buf, vict);
      break;
    case SCMD_FREEZE:
      if (ch == vict) 
      {
        send_to_char("Oh, yeah, THAT'S real smart...\r\n", ch);
        return;
      }
      if (PLR_FLAGGED(vict, PLR_FROZEN)) 
      {
        send_to_char("Your victim is already pretty cold.\r\n", ch);
        return;
      }
      SET_BIT(PLR_FLAGS(vict), PLR_FROZEN);
      GET_FREEZE_LEV(vict) = GET_LEVEL(ch);
      send_to_char("A bitter wind suddenly rises and drains every erg of heat from your body!\r\nYou feel frozen!\r\n", vict);
      send_to_char("Frozen.\r\n", ch);
      act("A sudden cold wind conjured from nowhere freezes $n!", FALSE, vict, 0, 0, TO_ROOM);
        sprintf(buf, "%s frozen by %s.", GET_NAME(vict), GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch));
      xlog(SYS_SET, MAX(LVL_GOD, GET_INVIS_LEV(ch)), buf, TRUE);
      break;
    case SCMD_THAW:
      if (!PLR_FLAGGED(vict, PLR_FROZEN)) 
      {
        send_to_char("Sorry, your victim is not morbidly encased in ice at the moment.\r\n", ch);
        return;
      }
      if (GET_FREEZE_LEV(vict) > GET_LEVEL(ch)) 
      {
        sprintf(buf, "Sorry, a level %d God froze %s... you can't unfreeze %s.\r\n",
        GET_FREEZE_LEV(vict), GET_NAME(vict), HMHR(vict));
        send_to_char(buf, ch);
        return;
      }
      sprintf(buf, "%s un-frozen by %s.", GET_NAME(vict), GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch));
      xlog(SYS_SET, MAX(LVL_GOD, GET_INVIS_LEV(ch)), buf, TRUE);
      REMOVE_BIT(PLR_FLAGS(vict), PLR_FROZEN);
      send_to_char("A fireball suddenly explodes in front of you, melting the ice!\r\nYou feel thawed.\r\n", vict);
      send_to_char("Thawed.\r\n", ch);
      act("A sudden fireball conjured from nowhere thaws $n!", FALSE, vict, 0, 0, TO_ROOM);
      break;
    case SCMD_NOGECHO:
      result = PLR_TOG_CHK(vict, PLR_NOGECHO);
      sprintf(buf, "Nogecho %s from now on for %s.\r\n", ONOFF(result), GET_NAME(vict));
      send_to_char(buf, ch);
      sprintf(buf, "%s has set %s's NOGECHO to %s.", GET_DISGUISED(ch) ?
      GET_RNAME(ch) : GET_NAME(ch), GET_NAME(vict), ONOFF(result));
      xlog(SYS_SET, MAX(LVL_GOD, GET_INVIS_LEV(ch)), buf, TRUE);
      break;
    case SCMD_MULTI:
      result = PLR_TOG_CHK( vict, PLR_CANMULTI );
      sprintf( buf, "MULTI %s from now on for %s.\r\n", ONOFF(result), GET_NAME( vict ) );
      send_to_char( buf, ch );
      sprintf(buf, "%s has set %s's CANMULTI to %s.", GET_DISGUISED(ch) ?
		      GET_RNAME(ch) : GET_NAME(ch), GET_NAME(vict),
		      ONOFF(result));
      xlog(SYS_SET, MAX(LVL_GOD, GET_INVIS_LEV(ch)), buf, TRUE);
      break;
    default:
      mlog("SYSERR: Unknown subcmd passed to do_wizutil (act.wizard.c)");
      break;
    }
    save_char(vict, NOWHERE);
  }
}

ACMD(do_unaffect)
{
  struct room_data *room;
  struct char_data *vict;
  one_argument(argument, arg);
  
  if (!*arg) {
    send_to_char("Yes, but for what?\r\n", ch);
    return;
  }
  
  if (!strcasecmp(arg, "room")) {
    room = &world[ch->in_room];
    if (!(room->aff)) {
      send_to_char("The room has no affections.\r\n", ch);
      return;
    }
    while (room->aff)
      affect_remove_room(room, room->aff);
    act("$n waves a mighty arm and there is a brief flash of light!", TRUE, ch, 0, 0, TO_ROOM);
    send_to_char("All affections on room removed.\r\n", ch); 
    return;
  }
  else if (!(vict = get_char_vis(ch, arg)))
    send_to_char("There is no such player.\r\n", ch);
  else if (IS_NPC(vict))
    send_to_char("You can't do that to a mob!\r\n", ch);
  else if (vict->affected) {
    while (vict->affected)
      affect_remove(vict, vict->affected);
    send_to_char("There is a brief flash of light!\r\n"
                 "You feel slightly different.\r\n", vict);
    send_to_char("All spells removed.\r\n", ch);
  } else {
    send_to_char("Your victim does not have any affections!\r\n", ch);
    return;
  }
}

/* single zone printing fn used by "show zone" so it's not repeated in the
   code 3 times ... -je, 4/6/93 */
/* Less detailed version, for ordinary "show zone", by request from Purdan */
void print_zone_to_buf_1(char *bufptr, int zone)
{

  int ble = (NULL != zone_table[zone].hfl_Editors && 0 != getNumber(zone_table[zone].hfl_Editors));

  if (ble)
    first(zone_table[zone].hfl_Editors);

  sprintf(bufptr,"%s%3d &y%-10s&n %-34s Reset: %2d (%1d) Top: %5d\r\n",bufptr
      ,zone_table[zone].number,(ble ? (char *)this(zone_table[zone].hfl_Editors) : (zone_table[zone].all ? "Anyone" : ""))
      ,zone_table[zone].name,zone_table[zone].lifespan, zone_table[zone].reset_mode,zone_table[zone].top);
}

/*
   prints a little more detailed zone info if doing: show zone <number>
*/
void print_zone_to_buf_2(char *bufptr, int zone)
{
  sprintf(bufptr, "%s%3d %-30s Age: %3d; Reset: %3d (%1d); Top: %5d\r\n",
      bufptr, zone_table[zone].number, zone_table[zone].name,
      zone_table[zone].age, zone_table[zone].lifespan,
      zone_table[zone].reset_mode, zone_table[zone].top);

  if (zone_table[zone].all) { 
    sprintf(bufptr,"%s\r\nPeople allowed to edit the zone:\r\n\r\n",bufptr); 
    sprintf(bufptr,"%sAny builder...\r\n",bufptr);
    return;
  }  

  if (NULL != zone_table[zone].hfl_Editors && 0 != getNumber(zone_table[zone].hfl_Editors)) {  
    sprintf(bufptr,"%s\r\nPeople allowed to edit the zone:\r\n\r\n",bufptr);
    first(zone_table[zone].hfl_Editors);
    do { 
      char *tp = this(zone_table[zone].hfl_Editors);
      sprintf(bufptr,"%s%s ",bufptr,tp);
    } while(next(zone_table[zone].hfl_Editors));
    sprintf(bufptr,"%s\r\n",bufptr); 
  } else { 
    sprintf(bufptr,"%sNo explicit building permission found.\r\n",bufptr);
  }  
}


ACMD(do_show)
{
  int i, j, k, l, con;
  char self = 0;
  struct char_data *vict;
  struct obj_data *obj;
  char field[MAX_INPUT_LENGTH], value[MAX_INPUT_LENGTH], birth[80];
  struct mallinfo m_info;

  struct show_struct {
    char *cmd;
    char level;
  } fields[] = {
    { "nothing",	0  },				/* 0 */
    { "zones",		LVL_IMMORT },			/* 1 */
    { "player",		LVL_GOD },
    { "UNUSED",		LVL_GOD },
    { "stats",		LVL_IMMORT },
    { "errors",		LVL_GRGOD },			/* 5 */
    { "godrooms",	200 },			
    { "shops",		LVL_IMMORT },
    { "rooms", 		LVL_GOD },
    { "objects",        LVL_GOD },
    { "mobs",		LVL_GOD },			/* 10 */
    { "qitems",         LVL_GOD },
    { "mirrors",        LVL_GRGOD },
    { "triggers",	LVL_GOD },
    { "portalstones",   LVL_GOD },
    { "\n", 0 }
  };

  skip_spaces(&argument);

  if (!*argument) {
    strcpy(buf, "Show options:\r\n");
    for (j = 0, i = 1; fields[i].level; i++)
      if (fields[i].level <= GET_LEVEL(ch))
    sprintf(buf, "%s%-15s%s", buf, fields[i].cmd, (!(++j % 5) ? "\r\n" : ""));
    strcat(buf, "\r\n");
    send_to_char(buf, ch);
    return;
  }

  strcpy(arg, two_arguments(argument, field, value));

  for (l = 0; *(fields[l].cmd) != '\n'; l++)
    if (!strncmp(field, fields[l].cmd, strlen(field)))
      break;

  if (GET_LEVEL(ch) < fields[l].level) {
    send_to_char("You are not godly enough for that!\r\n", ch);
    return;
  }
  if (!strcmp(value, "."))
    self = 1;
  buf[0] = '\0';
  switch (l) {
  case 1:			/* zone */
    /* tightened up by JE 4/6/93 */
    if (self)
      print_zone_to_buf_2(buf, world[ch->in_room].zone);
    else if (*value && is_number(value)) {
      for (j = atoi(value), i = 0; zone_table[i].number != j && i <= top_of_zone_table; i++);
      if (i <= top_of_zone_table)
    print_zone_to_buf_2(buf, i);
      else {
    send_to_char("That is not a valid zone.\r\n", ch);
    return;
      }
    } else
      for (i = 0; i <= top_of_zone_table; i++)
    print_zone_to_buf_1(buf, i);
    page_string(ch->desc, buf, 1);
    break;
  case 2:			/* player */
    if (!*value) {
      send_to_char("A name would help.\r\n", ch);
      return;
    }
    if (NULL == (vict = xmlload_char(value))) {
      send_to_char("There is no such player.\r\n", ch);
      return;
    }
    if (GET_LEVEL(vict) > GET_LEVEL(ch)) {
      send_to_char("You are not sufficiently godly for that!", ch);
      free_char(vict);
      return;
    }
    sprintf(buf, "Player: %-12s (%s) [%2ld %s]\r\n", GET_NAME(vict),
        genders[(int) GET_SEX(vict)], GET_LEVEL(vict), class_abbrevs[(int)GET_CLASS(vict)]);

    sprintf(buf,
     "%sAu: %-8ld  Bal: %-8ld  Exp: %-8ld  Lessons: %-3ld\r\n",
        buf, GET_GOLD(vict), GET_BANK_GOLD(vict), GET_EXP(vict),
        GET_PRACTICES(vict));

    strcpy(birth, ctime(&vict->player.time.birth));

    sprintf(buf,
        "%sStarted: %-20.16s  Last: %-20.16s  Played: %3dh %2dm\r\n",
        buf, birth, ctime(&vict->player.time.logon), (int) (vict->player.time.played / 3600),
        (int) (vict->player.time.played / 60 % 60));
    free_char(vict);
    send_to_char(buf, ch);
    break;
  case 3:
    send_to_char("Don't do this! We -have- no rent at Fang!\r\n", ch);
    break;
  case 4:
    i = 0;
    j = 0;
    k = 0;
    con = 0;
    for (vict = character_list; vict; vict = vict->next) {
      if (IS_NPC(vict))
    j++;
      else if (CAN_SEE(ch, vict)) {
    i++;
    if (vict->desc)
      con++;
      }
    }
    for (obj = object_list; obj; obj = obj->next)
      k++;
    sprintf(buf, "Current stats:\r\n");
    sprintf(buf, "%s  %5d players in game  %5d connected\r\n", buf, i, con);
    sprintf(buf, "%s  %5d registered\r\n", buf, top_of_p_table + 1);
    sprintf(buf, "%s  %5d mobiles          %5d prototypes\r\n",
        buf, j, top_of_mobt + 1);
    sprintf(buf, "%s  %5d objects          %5d prototypes\r\n",
        buf, k, top_of_objt + 1);
    sprintf(buf, "%s  %5d rooms            %5d zones\r\n",
        buf, top_of_world + 1, top_of_zone_table + 1);
    sprintf(buf, "%s  %5d large bufs\r\n", buf, buf_largecount);
    sprintf(buf, "%s  %5d buf switches     %5d overflows\r\n", buf,
        buf_switches, buf_overflows);
    m_info = mallinfo();
    sprintf(buf, "%s\r\n\r\nMalloc info:\r\n"
         "Arena: %d      Ordblks: %d      Smblks: %d\r\n"
         "Hblks: %d      Hblkhd: %d       Usmblks: %d\r\n"
         "Fsmblks: %d    Uordblks: %d     Fordblks: %d\r\n"
         "               Keepcost: %d\r\n", buf, 
         m_info.arena, m_info.ordblks, m_info.smblks,
         m_info.hblks, m_info.hblkhd, m_info.usmblks,
         m_info.fsmblks, m_info.uordblks, m_info.fordblks,
         m_info.keepcost);
    send_to_char(buf, ch);
    break;
  case 5:
    strcpy(buf, "Errant Rooms\r\n------------\r\n");
    for (i = 0, k = 0; i <= top_of_world; i++)
      for (j = 0; j < NUM_OF_DIRS; j++)
    if (world[i].dir_option[j] && world[i].dir_option[j]->to_room == 0)
      sprintf(buf, "%s%2d: [%5d] %s\r\n", buf, ++k, world[i].number,
          world[i].name);
    page_string(ch->desc, buf, 1);
    break;
  case 6:
#define GOD_ROOMS_ZONE 326
    strcpy(buf, "Godrooms\r\n--------------------------\r\n");
    for (i = 0, j = 0; i < top_of_world; i++)
      if (world[i].zone == GOD_ROOMS_ZONE)
    sprintf(buf, "%s%2d: [%5d] %s\r\n", buf, j++, world[i].number,
        world[i].name);

    send_to_char(buf, ch);
    break;
  case 7:
    if ((!*value) || !is_number(value))
    {
      send_to_char("Show shops in which zone?\r\n", ch);
      break;
    }
    i = atoi(value);
    sprintf(buf, "Shops in zone %d:\r\n\r\n", i);
    sprintf(buf+strlen(buf), " ##   Virtual   Where    Keeper    Buy   Sell   Customers\r\n---------------------------------------------------------\r\n");
    for (j = 0; j <= top_shop; j++) {
      if ( i == (int)(SHOP_NUM(j) / 100 ) ) {
        sprintf(buf+strlen(buf), "%3d   %6ld   %6ld    ", j+1, SHOP_NUM(j), SHOP_ROOM(j, 0));
        if (SHOP_KEEPER(j) < 0)
          sprintf(buf+strlen(buf), "<NONE>");
        else
          sprintf(buf+strlen(buf), "%6ld", mob_index[SHOP_KEEPER(j)].virtual);
        sprintf(buf+strlen(buf), "   %3.2f   %3.2f    ", SHOP_SELLPROFIT(j), SHOP_BUYPROFIT(j));
	strcpy(buf2, customer_string(j, FALSE));
	sprintf(buf+strlen(buf), "%s\r\n", buf2);
      }
    }
    if (!j)
      send_to_char("Hmmm...No shops in that zone.  You sure you typed in the right number?\r\n", ch);
    else
      page_string(ch->desc, buf, 1);
    break;
  case 8:     		/* Rooms */
    if ((!*value) || !is_number(value))
    {  
      send_to_char("Show rooms in what zone?\r\n", ch);
      break;
    }
    i = atoi(value);
    sprintf(buf, "Rooms in zone %d:\r\n-----------------\r\n", i);
    k = 0;
    for (j = 0; j <= top_of_world; j++)
    {
      if ((world[j].number/100) == i)
      {
        k++;
        sprintf(buf, "%s%d : %40s       Exits: [",buf, world[j].number, world[j].name? world[j].name: "\\c01WARNING: Room has no name.\\c00");
        if (world[j].dir_option[0]) strcat(buf, "N");
        if (world[j].dir_option[1]) strcat(buf, " E");
        if (world[j].dir_option[2]) strcat(buf, " S");
        if (world[j].dir_option[3]) strcat(buf, " W");
        if (world[j].dir_option[4]) strcat(buf, " U");
        if (world[j].dir_option[5]) strcat(buf, " D");
        if (world[j].dir_option[6]) strcat(buf, " NE");
        if (world[j].dir_option[7]) strcat(buf, " SE");
        if (world[j].dir_option[8]) strcat(buf, " SW");
        if (world[j].dir_option[9]) strcat(buf, " NW");
        strcat(buf,"]\r\n");
      }
    }
    if (!k)
      send_to_char("Hmm...No rooms in that zone. You sure you typed in the right number?\r\n", ch);
    else
      page_string(ch->desc, buf, 1);
    break;
  case 9:  		/* Objects */
    if ((!*value) || !is_number(value))
    {  
      send_to_char("Show objects in what zone?\r\n", ch);
      break;
    }
    i = atoi(value);
    sprintf(buf, "Objects in zone %d:\r\n-------------------\r\n", i);
    k = 0;
    for (j = 0; j <= top_of_objt; j++)
    {
      if (obj_index[j].virtual/100 == i)
      {
        k++;
        sprintf(buf, "%s%ld : %s\r\n", buf, obj_index[j].virtual,
                obj_proto[j].short_description? obj_proto[j].short_description: "NO NAME");
      }
    }
    if (!k)
      send_to_char("Hmm...No objects in that zone. You sure you typed in the right number?\r\n", ch);
    else
      page_string(ch->desc, buf, 1);
    break;

  case 10:  		/* Mobiles */
    if ((!*value) || !is_number(value))
    {  
      send_to_char("Show mobiles in what zone?\r\n", ch);
      break;
    }
    i = atoi(value);
    sprintf(buf, "Mobiles in zone %d:\r\n---------------------------\r\n", i);
    k = 0;
    for (j = 0; j <= top_of_mobt; j++)
    {   
      if (mob_index[j].virtual/100 == i)
      {
        k++;
        sprintf(buf, "%s%ld : %s\r\n", buf, mob_index[j].virtual, mob_proto[j].player.short_descr? mob_proto[j].player.short_descr: "NO NAME");
      }
    }
    if (!k)
      send_to_char("Hmm...No mobiles in that zone. You sure you typed in the right number?\r\n", ch);
    else
      page_string(ch->desc, buf, 1);
    break;    
  case 11:
    k = 0; 
    for (j = 0; j <= top_of_objt; j++) {
      if ( GET_OBJ_QCOST( (obj_proto+j) ) ) {
        k++;
        sprintf(buf, "%s%ld : %s\r\n", buf, obj_index[j].virtual,
                obj_proto[j].short_description? obj_proto[j].short_description: "NO NAME");
      }
    }
    if (!k)
      send_to_char("Hmm...No quest items found at all...\r\n",ch);
    else
      page_string(ch->desc, buf, 1);
    break;
  case 12:
    strcpy(buf, "&KMirrored Rooms&n\r\n&W--------------&n\r\n");
    for (i = 0; mirrored_rooms[i].to != -1; i++)
      sprintf(buf, "%s&W[&K%5ld&W]&n -> &W[&K%5ld&W]&n\r\n", buf, mirrored_rooms[i].from, mirrored_rooms[i].to);
    page_string(ch->desc, buf, 1);
    break;
  case 13:
    if (!*value || !is_number(value)) {
      send_to_char("Show triggers in what zone?\r\n", ch);
      return;
    }
    i = atoi(value);
    sprintf(buf, "Triggers in zone %d:\r\n--------------------\r\n", i);
    k = 0;
    for (j = 0; j < top_of_trigt; j++) {
      if ((trig_index[j]->virtual/100) == i)
    sprintf(buf, "%s%5d. [%5ld] %s\r\n", buf, ++k,
      trig_index[j]->virtual, trig_index[j]->proto->name);
    }
    if (!k)
      send_to_char("Hmm...No triggers in that zone. You sure you typed in the right number?\r\n", ch);
    else
      page_string(ch->desc, buf, 1);
    break;
  case 14:
    st_list_wiz(ch);
    break;
  default:
    send_to_char("Sorry, I don't understand that.\r\n", ch);
    break;
  }
}


static void change_default_descs(struct char_data *ch)
{
  int i;
  char buf[250];
  int len = strlen("A typical, run of the mill");
  

  for (i = 0; i < MAX_DESCRIPTIONS; i++) {
    sprintf(buf,"A typical, run of the mill %s.\r\n",GET_RNAME(ch));
    if (NULL == GET_DESCNUM(ch,i)) {
      GET_DESCNUM(ch,i) = strdup(buf);
    } else {
      if (!strncasecmp("A typical, run of the mill", GET_DESCNUM(ch,i),len) && 
      strlen(GET_DESCNUM(ch,i)) < 2*len) {	
    free(GET_DESCNUM(ch,i));
    GET_DESCNUM(ch,i) = strdup(buf);
      }
    }
  }
}

#define PC   1
#define NPC  2
#define BOTH 3

#define MISC	0
#define BINARY	1
#define NUMBER	2

#define SET_OR_REMOVE(flagset, flags) { \
        if (on) SET_BIT(flagset, flags); \
        else if (off) REMOVE_BIT(flagset, flags); }

#define SET_OR_REMOVE_TALENT(___flags1, ___flags2, ___flags) do { \
    if (on) SET_TALENT((___flags1), (___flags2), ___flags); \
    else if (off) REMOVE_TALENT((___flags1), (___flags2), ___flags); } while(0)

#define RANGE(low, high) (value = MAX((low), MIN((high), (value))))

ACMD(do_set)
{
  int i, l, found = 0, race = 0;/* qend; */
  struct char_data *vict = NULL, *cbuf = NULL,*vict2;
  char field[MAX_INPUT_LENGTH], name[MAX_INPUT_LENGTH], val_arg[MAX_INPUT_LENGTH], 
    to_log[MAX_STRING_LENGTH], strng[MAX_INPUT_LENGTH], *tmp_ptr = NULL;
  int on = 0, off = 0, value = 0, temp = 0, mode, count = 1;
  char is_file = 0, is_mob = 0, is_player = 0;
  int do_log = 0;
   
  void write_aliases(struct char_data *ch);
  void read_aliases(struct char_data *ch);
  void save_all_guilds();

  struct set_struct {
    char *cmd;
    int level;
    char pcnpc;
    char type;
  }          fields[] = {
   { "brief",		LVL_GOD, 	PC, 	BINARY },  /* 0 */
   { "invstart", 	LVL_GOD, 	PC, 	NUMBER },  /* 1 */
   { "title",		LVL_GOD, 	PC, 	MISC },
   { "nospam", 		LVL_IMMORT, 	PC, 	BINARY },
   { "maxhit",		LVL_IMMORT, 	BOTH, 	NUMBER },
   { "maxmana", 	LVL_IMMORT, 	BOTH, 	NUMBER },  /* 5 */
   { "maxmove", 	LVL_IMMORT, 	BOTH, 	NUMBER },
   { "hit", 		LVL_IMMORT, 	BOTH, 	NUMBER },
   { "mana",		LVL_IMMORT, 	BOTH, 	NUMBER },
   { "move",		LVL_IMMORT, 	BOTH, 	NUMBER },
   { "UNUSED",		LVL_IMPL, 	BOTH, 	NUMBER },  /* 10 - Align, not used */
   { "str",		LVL_GOD, 	BOTH, 	NUMBER },
   { "UNUSED2", 	LVL_IMPL, 	BOTH, 	NUMBER },  /* stra dd, not used by us! */
   { "int", 		LVL_GOD, 	BOTH, 	NUMBER },
   { "wis", 		LVL_GOD, 	BOTH, 	NUMBER },
   { "dex", 		LVL_GOD, 	BOTH, 	NUMBER },  /* 15 */
   { "con", 		LVL_GOD, 	BOTH, 	NUMBER },
   { "sex", 		LVL_IMMORT, 	BOTH, 	MISC },
   { "ac", 		LVL_GRGOD, 	BOTH, 	NUMBER },
   { "gold",		LVL_GOD, 	BOTH, 	NUMBER },
   { "bank",		LVL_GOD, 	PC, 	NUMBER },  /* 20 */
   { "exp", 		LVL_GRGOD, 	BOTH, 	NUMBER },
   { "hitroll", 	LVL_GRGOD, 	BOTH, 	NUMBER },
   { "damroll", 	LVL_GRGOD, 	BOTH, 	NUMBER },
   { "invis",		LVL_IMPL, 	PC, 	NUMBER },
   { "nohassle", 	LVL_GRGOD, 	PC, 	BINARY },  /* 25 */
   { "frozen",		LVL_FREEZE, 	PC, 	BINARY },
   { "practices", 	LVL_GRGOD, 	PC, 	NUMBER },
   { "trains", 	 	LVL_GRGOD, 	PC, 	NUMBER },
   { "drunk",		LVL_IMMORT, 	BOTH, 	MISC },
   { "hunger",		LVL_IMMORT, 	BOTH, 	MISC },    /* 30 */
   { "thirst",		LVL_IMMORT, 	BOTH, 	MISC },
   { "level",		LVL_GRGOD, 	BOTH, 	NUMBER },
   { "room",		LVL_IMPL, 	BOTH, 	NUMBER },  
   { "roomflag", 	LVL_GRGOD, 	PC, 	BINARY },
   { "siteok",		LVL_GRGOD, 	PC, 	BINARY },  /* 35 */
   { "deleted", 	LVL_GOD, 	PC, 	BINARY },
   { "class",		LVL_GRGOD, 	BOTH, 	MISC },
   { "nowizlist", 	LVL_GOD, 	PC, 	BINARY },  
   { "quest",		LVL_GOD, 	PC, 	BINARY },
   { "loadroom", 	LVL_GOD, 	PC, 	MISC },    /* 40 */
   { "color",		LVL_GOD, 	PC, 	BINARY },
   { "idnum",		LVL_IMPL, 	PC, 	NUMBER },
   { "passwd",		LVL_GOD, 	PC, 	MISC },    
   { "nodelete", 	LVL_GOD, 	PC, 	BINARY },
   { "cha",		LVL_GOD, 	BOTH, 	NUMBER },  /* 45 */
   { "olc",		LVL_GRGOD, 	PC, 	NUMBER },
   { "air",             LVL_GOD,        PC,     NUMBER },
   { "earth",           LVL_GOD,        PC,     NUMBER },  
   { "fire",            LVL_GOD,        PC,     NUMBER },
   { "spirit",          LVL_GOD,        PC,     NUMBER },  /* 50 */
   { "water",           LVL_GOD,        PC,     NUMBER },
   { "spark",           LVL_GOD,        PC,     BINARY },
   { "teachable",       LVL_GOD,        PC,     BINARY },  
   { "ta'veren",	LVL_GOD,	PC,	BINARY },
   { "wolfkin",		LVL_GOD,	PC,	BINARY },  /* 55 */
   { "name",            LVL_IMMORT,      PC,     MISC},
   { "greyman",         LVL_GOD,        PC,     BINARY }, 
   { "gholam",          LVL_GOD,        PC,     BINARY }, 
   { "nobility",        LVL_GOD,        PC,     MISC },
   { "leader",		LVL_GOD,	PC,	MISC },	  /* 60 */
   { "powerpoints",	LVL_GOD,	PC,	NUMBER},
   { "gentled",         LVL_GOD,        PC,     BINARY},
   { "nogain",          LVL_GOD,        PC,     BINARY},
   { "cantpost",        LVL_GRGOD,      PC,     BINARY}, 
   { "!compulsion!",    LVL_IMPL,        PC,     NUMBER},
   { "wagon",		LVL_GOD,	PC, 	NUMBER},
   { "taint",		LVL_GOD,	PC,	NUMBER},  
   { "taintres",	LVL_GOD,	PC,	BINARY},  
   { "race",		LVL_GOD,	PC,	MISC}, 	 
   { "blademastery",    LVL_GOD,        PC,     NUMBER},  /* 70 */
   { "UNUSED3",		LVL_IMPL,       PC,     NUMBER},  // 71
   { "UNUSED4",		LVL_IMPL, 	PC,	MISC},     // 72
   { "UNUSED5",		LVL_IMPL,       PC,     BINARY},  
   { "UNUSED6",		LVL_IMPL,       PC,     MISC},  
   { "UNUSED7",	 	LVL_IMPL,	PC,	MISC},    // 75
   { "newbie-helper",   LVL_GOD,        PC,     BINARY},
   { "guildleader",     LVL_GRGOD,      PC,     BINARY},
   { "storyteller",     LVL_GRGOD,      PC,     BINARY},
   { "gl-builder",      LVL_GRGOD,      PC,     BINARY},
   { "builder",         LVL_GRGOD,      PC,     BINARY},  /* 80 */
   { "admin",           LVL_GRGOD,      PC,     BINARY},
   { "storymaster",     LVL_GRGOD,      PC,     BINARY},
   { "headgl",          LVL_GRGOD,      PC,     BINARY},
   { "headbuilder",     LVL_GRGOD,      PC,     BINARY},
   { "owner",           LVL_GRGOD,      PC,     BINARY},
   { "weaver",          LVL_GRGOD,      PC,     BINARY},
   { "headcoder",       LVL_GRGOD,      PC,     BINARY},
   { "headcouncilor",   LVL_GRGOD,      PC,     BINARY},
   { "newbiepr",        LVL_GOD,        PC,     BINARY},
   { "primtalent",      LVL_GOD,        PC,     MISC},  /* 90 */
   { "sectalent",       LVL_GOD,        PC,     MISC},
   { "spskill",         LVL_GOD,        PC,     MISC}, 
   { "talent",          LVL_GOD,        PC,     MISC},
   { "nomail",		LVL_GOD, 	PC,	BINARY},
   { "webmaster",       LVL_GRGOD,      PC,     BINARY}, /* 95 */
   { "dgs",		LVL_GRGOD, 	PC,	BINARY},
   { "pretitle" ,       LVL_GOD,        PC,     MISC},
   { "cantgecho", 	LVL_GRGOD, 	PC, 	BINARY},
   { "aedit",		LVL_GRGOD,	PC,	BINARY},
   { "\n", 0, BOTH, MISC }				  
  };
  sprintf(to_log, "%s has set", GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch));
  half_chop(argument, name, buf);

  if (!strcmp(name, "list")) {
    sprintf(buf, "The following fields can be set:\r\n");
    for(mode = 0; *(fields[mode].cmd) != '\n'; mode++) {
      if (GET_LEVEL(ch) >= fields[mode].level) {
        sprintf(buf +strlen(buf), "%-11s", fields[mode].cmd);
        if (!(count % 7))
          strcat(buf, "\r\n");
          count++;
      }
    }
    strcat(buf, "\r\n");
    send_to_char(buf, ch);
    return;
  }


  if (!strcmp(name, "brd")) {
    do_board_set( ch, buf );
    return; 
  } else if (!strcmp(name, "file")) {
    is_file = 1;
    half_chop(buf, name, buf);
    sprintf(to_log, "%s file", to_log);
  } else if (!str_cmp(name, "player")) {
    is_player = 1;
    half_chop(buf, name, buf);
    sprintf(to_log, "%s player", to_log);
  } else if (!str_cmp(name, "mob")) {
    is_mob = 1;
    half_chop(buf, name, buf);
    sprintf(to_log, "%s mob", to_log);
  }
  half_chop(buf, field, buf);
  strcpy(val_arg, buf);

  if (!*name || !*field) {
    send_to_char("Usage: set <victim> <field> <value>\r\n", ch);
    return;
  }
  if (!is_file) {
    if (is_player) {
      if (!(vict = get_player_vis(ch, name, 0))) {
    send_to_char("There is no such player.\r\n", ch);
    return;
      }
    } else {
      if (!(vict = get_char_vis(ch, name))) {
    send_to_char("There is no such creature.\r\n", ch);
    return;
      }
    }
  } else if (is_file) {
    if (NULL == (vict = xmlload_char(name))) {
      send_to_char("There is no such player.\r\n", ch);
      return;
    } 
    if ((GET_LEVEL(vict) >= GET_LEVEL(ch) && GET_IDNUM(ch) != 1) && (GET_LEVEL(ch) < LVL_IMPL - 1)) {
      free_char(vict);
      send_to_char("Sorry, you can't do that.\r\n", ch);
      return;
    }
    // vict is ready to be used here...
  }
  if (GET_LEVEL(ch) < LVL_IMPL - 1) {
    if (!IS_NPC(vict) && GET_LEVEL(ch) <= GET_LEVEL(vict) && vict != ch && GET_IDNUM(ch) != 1)
{
      send_to_char("Maybe that's not such a great idea...\r\n", ch);
      return;
    }
  }
  sprintf(to_log, "%s %s's", to_log, GET_NAME(vict));
  for (l = 0; *(fields[l].cmd) != '\n'; l++)
    if (!strncmp(field, fields[l].cmd, strlen(field)))
      break;
  sprintf(to_log, "%s %s to", to_log, fields[l].cmd);

  if (GET_LEVEL(ch) < fields[l].level && !IS_SET((ch)->player_specials->saved.command_groups, CG_WEAVER)) {
    send_to_char("You are not godly enough for that!\r\n", ch);
    return;
  }
  if (IS_NPC(vict) && !(fields[l].pcnpc & NPC)) {
    send_to_char("You can't do that to a beast!\r\n", ch);
    return;
  } else if (!IS_NPC(vict) && !(fields[l].pcnpc & PC)) {
    send_to_char("That can only be done to a beast!\r\n", ch);
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
  strcpy(buf, "Okay.");  /* can't use OK macro here 'cause of \r\n */
  switch (l) {
  case 0:
    SET_OR_REMOVE(PRF_FLAGS(vict), PRF_BRIEF);
    break;
  case 1:
    GET_INVSTART(vict) = RANGE(0, GET_LEVEL(vict));
    sprintf(to_log, "%s %ld.", to_log, GET_INVSTART(vict)); 
/*
    SET_OR_REMOVE(PRF_FLAGS(vict), PLR_INVSTART);
    break;
*/
    break;
  case 2:
    set_title(vict, val_arg);
    sprintf(buf, "%s's title is now: %s", GET_NAME(vict), GET_TITLE(vict));
    sprintf(to_log, "%s '%s'", to_log, GET_TITLE(vict));
    break;
  case 3:
    SET_OR_REMOVE(PRF_FLAGS(vict), PRF_NOSPAM);
    on = !on;			/* so output will be correct */
    break;
  case 4:
    vict->points.max_hit = RANGE(1, 15000);
    affect_total(vict);
    sprintf(to_log, "%s %d.", to_log, vict->points.max_hit);
    break;
  case 5:
    vict->points.max_mana = RANGE(1, 15000);
    affect_total(vict);
    sprintf(to_log, "%s %d.", to_log, vict->points.max_mana);
    break;
  case 6:
    vict->points.max_move = RANGE(1, 15000);
    affect_total(vict);
    sprintf(to_log, "%s %d.", to_log, vict->points.max_move);
    break;
  case 7:
    vict->points.hit = RANGE(-9, vict->points.max_hit);
    affect_total(vict);
    sprintf(to_log, "%s %d.", to_log, vict->points.hit);
    break;
  case 8:
    vict->points.mana = RANGE(0, vict->points.max_mana);
    affect_total(vict);
    sprintf(to_log, "%s %d.", to_log, vict->points.mana);
    break;
  case 9:
    vict->points.move = RANGE(0, vict->points.max_move);
    affect_total(vict);
    sprintf(to_log, "%s %d.", to_log, vict->points.move);
    break;
  case 10:
    break;
  case 11:
    if (IS_NPC(vict) || GET_LEVEL(vict) >= LVL_GRGOD)
      RANGE(0, 30);
    else
      RANGE(0, 30);
    vict->real_abils.str = value;
    affect_total(vict);
    sprintf(to_log, "%s %d.", to_log, vict->real_abils.str);
    break;
  case 12:
    break;
  case 13:
    if (IS_NPC(vict) || GET_LEVEL(vict) >= LVL_GRGOD)
      RANGE(0, 30);
    else
      RANGE(0, 30);
    vict->real_abils.intel = value;
    affect_total(vict);
    sprintf(to_log, "%s %d.", to_log, vict->real_abils.intel);
    break;
  case 14:
    if (IS_NPC(vict) || GET_LEVEL(vict) >= LVL_GRGOD)
      RANGE(0, 30);
    else
      RANGE(0, 30);
    vict->real_abils.wis = value;
    affect_total(vict);
    sprintf(to_log, "%s %d.", to_log, vict->real_abils.wis);
    break;
  case 15:
    if (IS_NPC(vict) || GET_LEVEL(vict) >= LVL_GRGOD)
      RANGE(0, 30);
    else
      RANGE(0, 30);
    vict->real_abils.dex = value;
    affect_total(vict);
    sprintf(to_log, "%s %d.", to_log, vict->real_abils.dex);
    break;
  case 16:
    if (IS_NPC(vict) || GET_LEVEL(vict) >= LVL_GRGOD)
      RANGE(0, 30);
    else
      RANGE(0, 30);
    vict->real_abils.con = value;
    affect_total(vict);
    sprintf(to_log, "%s %d.", to_log, vict->real_abils.con);
    break;
  case 17:
    if (!str_cmp(val_arg, "male")){
      sprintf(to_log, "%s male.", to_log);
      vict->player.sex = SEX_MALE;
    }
    else if (!str_cmp(val_arg, "female")){
      sprintf(to_log, "%s female.", to_log);
      vict->player.sex = SEX_FEMALE;
    }
    else if (!str_cmp(val_arg, "neutral")){
      sprintf(to_log, "%s androgenous.", to_log);
      vict->player.sex = SEX_NEUTRAL;
    }
    else {
      send_to_char("Must be 'male', 'female', or 'neutral'.\r\n", ch);
      return;
    }
    break;
  case 18:
    vict->points.armor = RANGE(-200, 100);
    affect_total(vict);
    sprintf(to_log, "%s %d.", to_log, vict->points.armor);
    break;
  case 19:
    GET_GOLD(vict) = RANGE(0, 100000000);
    sprintf(to_log, "%s %ld.", to_log, GET_GOLD(vict));
    break;
  case 20:
    GET_BANK_GOLD(vict) = RANGE(0, 100000000);
    sprintf(to_log, "%s %ld.", to_log, GET_BANK_GOLD(vict));
    break;
  case 21:
    vict->points.exp = RANGE(0, 50000000);
    vict->points.exp = MIN(GET_LEVEL_EXP(ch)*(LVL_IMMORT-2), vict->points.exp);
    sprintf(to_log, "%s %ld.", to_log, vict->points.exp);
    break;
  case 22:
    vict->points.hitroll = RANGE(-90000, 90000);
    affect_total(vict);
    sprintf(to_log, "%s %ld.", to_log, vict->points.hitroll);
    break;
  case 23:
    vict->points.damroll = RANGE(-90000, 90000);
    affect_total(vict);
    sprintf(to_log, "%s %ld.", to_log, vict->points.damroll);
    break;
  case 24:
    if (GET_LEVEL(ch) < LVL_IMPL && ch != vict) {
      send_to_char("You aren't godly enough for that!\r\n", ch);
      return;
    }
    GET_INVIS_LEV(vict) = RANGE(0, GET_LEVEL(vict));
    sprintf(to_log, "%s %d.", to_log, GET_INVIS_LEV(vict));
    break;
  case 25:
    if (GET_LEVEL(ch) < LVL_IMPL && ch != vict) {
      send_to_char("You aren't godly enough for that!\r\n", ch);
      return;
    }
    SET_OR_REMOVE(PRF_FLAGS(vict), PRF_NOHASSLE);
    break;
  case 26:
    if (ch == vict) {
      send_to_char("Better not -- could be a long winter!\r\n", ch);
      return;
    }
    SET_OR_REMOVE(PLR_FLAGS(vict), PLR_FROZEN);
    break;
  case 27:
    GET_PRACTICES(vict) = RANGE(0,10000);
    sprintf(to_log,"%s %ld.",to_log,GET_PRACTICES(vict));
    break;
  case 28:
    GET_TRAINS(vict) = RANGE(0, 255);
    sprintf(to_log, "%s %d.", to_log, GET_TRAINS(vict));
    break;
  case 29:
  case 30:
  case 31:
    if (!str_cmp(val_arg, "off")) {
      value = -1;
      GET_COND(vict, (l - 29)) = (char) -1;
      sprintf(buf, "%s's %s now off.", GET_NAME(vict), fields[l].cmd);
    } else if (is_number(val_arg)) {
      value = atoi(val_arg);
      RANGE(0, 24);
      GET_COND(vict, (l - 29)) = (char) value;
      sprintf(buf, "%s's %s set to %d.", GET_NAME(vict), fields[l].cmd,
          value);
      sprintf(to_log, "%s %d.", to_log, value);
    } else {
      send_to_char("Must be 'off' or a value from 0 to 24.\r\n", ch);
      return;
    }
    break;

  case 32:
    if (((value > GET_LEVEL(ch)) && (GET_LEVEL(ch) < LVL_GRGOD) && !IS_SET((ch)->player_specials->saved.command_groups, CG_WEAVER)) || value > LVL_IMPL) {
      send_to_char("You can't do that.\r\n", ch);
      return;
    }
    RANGE(0, LVL_IMPL);
    vict->player.level = value;
    sprintf(to_log, "%s %ld.", to_log, vict->player.level);
    break;
  case 33:
    if ((i = real_room(value)) < 0) {
      send_to_char("No room exists with that number.\r\n", ch);
      return;
    }
    char_from_room(vict);
    char_to_room(vict, i);
    sprintf(to_log, "%s %d.", to_log, vict->in_room);
    break;
  case 34:
    SET_OR_REMOVE(PRF_FLAGS(vict), PRF_ROOMFLAGS);
    break;
  case 35:
    SET_OR_REMOVE(PLR_FLAGS(vict), PLR_SITEOK);
    break;
  case 36:
    SET_OR_REMOVE(PLR_FLAGS(vict), PLR_DELETED);
    break;
  case 37:
    if ((i = parse_class(val_arg)) == CLASS_UNDEFINED) {
      send_to_char("That is not a class.\r\n", ch);
      return;
    }
    GET_CLASS(vict) = i;
    sprintf(to_log, "%s %s.", to_log, pc_class_types[i]);
    break;
  case 38:
    SET_OR_REMOVE(PLR_FLAGS(vict), PLR_NOWIZLIST);
    break;
  case 39:
    SET_OR_REMOVE(PRF_FLAGS(vict), PRF_QUEST);
    break;
  case 40:
    if (!str_cmp(val_arg, "off"))
      REMOVE_BIT(PLR_FLAGS(vict), PLR_LOADROOM);
    else if (is_number(val_arg)) {
      value = atoi(val_arg);
      if (real_room(value) != NOWHERE) {
        SET_BIT(PLR_FLAGS(vict), PLR_LOADROOM);
    GET_LOADROOM(vict) = value;
    sprintf(buf, "%s will enter at room #%d.", GET_NAME(vict),
        GET_LOADROOM(vict));
        sprintf(to_log, "%s %d.", to_log, GET_LOADROOM(vict));
      } else {
    sprintf(buf, "That room does not exist!");
      }
    } else {
      strcpy(buf, "Must be 'off' or a room's virtual number.\r\n");
    }
    break;
  case 41:
    SET_OR_REMOVE(PRF_FLAGS(vict), (PRF_COLOR_1 | PRF_COLOR_2));
    break;
  case 42:
    if (GET_IDNUM(ch) != 1 || !IS_NPC(vict))
      return;
    GET_IDNUM(vict) = value;
    sprintf(to_log, "%s %ld.", to_log, GET_IDNUM(vict));
    break;
  case 43:
    if (!is_file)
      return;

    if (GET_LEVEL(vict) >= LVL_GRGOD) {
      send_to_char("You cannot change that.\r\n", ch);
      return;
    }
    strncpy(GET_PASSWD(vict),CRYPT(val_arg,GET_RNAME(vict)),MAX_PWD_LENGTH);
    GET_PASSWD(vict)[MAX_PWD_LENGTH] = '\0';
    if (NULL != (vict2 = get_player_vis(ch, name, 0))) {
      strncpy(GET_PASSWD(vict2),CRYPT(val_arg,GET_RNAME(vict)),MAX_PWD_LENGTH);
      GET_PASSWD(vict2)[MAX_PWD_LENGTH] = '\0';
    }
    sprintf(buf, "Password changed to '%s'\r\n", val_arg);
    send_to_char(buf, ch);
    sprintf(to_log, "%s...  Oop!!!  I'm not allowed to tell.", to_log);
    break;
  case 44:
    SET_OR_REMOVE(PLR_FLAGS(vict), PLR_NODELETE);
    break;
  case 45:
    if (IS_NPC(vict) || GET_LEVEL(vict) >= LVL_GRGOD)
      RANGE(0, 30);
    else
      RANGE(0, 30);
    vict->real_abils.cha = value;
    affect_total(vict);
    sprintf(to_log, "%s %d.", to_log, vict->real_abils.cha);
    break;
  case 46:
    if (((GET_LEVEL(ch) >= LVL_GRGOD) || (IS_SET((ch)->player_specials->saved.command_groups, CG_BUILDER))) && 
         (ch == vict))
      GET_OLC_ZONE(vict) = value;
    if ((GET_LEVEL(ch) >= LVL_GRGOD) && (ch != vict))
      GET_OLC_ZONE(vict) = value;
    if ((GET_LEVEL(ch) >= LVL_GOD) && (GET_LEVEL(ch) < LVL_GRGOD) && (ch != vict)) 
     send_to_char("You can't set other people's olc !\r\n", ch);
    if (GET_LEVEL(ch) < LVL_GOD) {
     send_to_char("You're not godly enough for that.\r\n", ch);
     sprintf(to_log, "%s %ld.", to_log, GET_OLC_ZONE(vict));
    } 
   break;
  case 47:
    GET_AIR(vict) = value;
    sprintf(to_log, "%s %d.", to_log, GET_AIR(vict));
    break;
  case 48:
    GET_EARTH(vict) = value;
    sprintf(to_log, "%s %d.", to_log, GET_EARTH(vict));
    break;
  case 49:
    GET_FIRE(vict) = value;
    sprintf(to_log, "%s %d.", to_log, GET_FIRE(vict));
    break;
  case 50:
    GET_SPIRIT(vict) = value;
    sprintf(to_log, "%s %d.", to_log, GET_SPIRIT(vict));
    break;
  case 51:
    GET_WATER(vict) = value;
    sprintf(to_log, "%s %d.", to_log, GET_WATER(vict));
    break;
  case 52:
    if (GET_LEVEL(ch) < LVL_GRGOD && on)
      {
      send_to_char("You aren't holy enough for this command.\r\n", ch);
      return;
      } 
    GET_SPARK(vict) = on ? 1 : 0;
    GET_FIRE(vict)   = (GET_FIRE(vict)   ? GET_FIRE(vict)   : 1);
    GET_SPIRIT(vict) = (GET_SPIRIT(vict) ? GET_SPIRIT(vict) : 1);
    GET_WATER(vict)  = (GET_WATER(vict)  ? GET_WATER(vict)  : 1);
    GET_EARTH(vict)  = (GET_EARTH(vict)  ? GET_EARTH(vict)  : 1);
    GET_AIR(vict)    = (GET_AIR(vict)    ? GET_AIR(vict)    : 1);
    break;
  case 53:
    GET_TEACHABLE(vict) = on ? 1 : 0;
    GET_FIRE(vict)   = (GET_FIRE(vict)   ? GET_FIRE(vict)   : 1);
    GET_SPIRIT(vict) = (GET_SPIRIT(vict) ? GET_SPIRIT(vict) : 1);
    GET_WATER(vict)  = (GET_WATER(vict)  ? GET_WATER(vict)  : 1);
    GET_EARTH(vict)  = (GET_EARTH(vict)  ? GET_EARTH(vict)  : 1);
    GET_AIR(vict)    = (GET_AIR(vict)    ? GET_AIR(vict)    : 1);
    break;
  case 54:
    GET_TAVEREN(vict) = on ? 1 : 0;
    break;
  case 55:
    GET_WOLFKIN(vict) = on ? 1 : 0;
    break;
  case 56:
    if (GET_DISGUISED(vict)) {
      send_to_char("Are you serious!? That character's disguised!\r\n", ch);
      return;
    }
    if (get_char_vis(ch,val_arg) || (NULL != (cbuf = xmlload_char(val_arg)) && !PLR_FLAGGED(cbuf, PLR_DELETED))) {
      free_char(cbuf);
      send_to_char("Name already exists, please choose another one!\r\n",ch);
      return;
    } 
     if ((_parse_name(val_arg,strng)) || strlen(strng) < 2 || strlen(strng) > MAX_NAME_LENGTH ||
     fill_word(strcpy(buf2,strng)) || reserved_word(buf2)) {
       send_to_char("Invalid name, please try again.\r\n",ch);
       return;
     }
     
     if (is_file) {
       read_aliases(vict);
       read_XML_playerfile(vict, CRASH_FILE);
     }
     
     sprintf(strng, "%s's name changed to %s.\r\n", GET_RNAME(vict), CAP(buf2));
     send_to_char(strng, ch);
     sprintf(strng, "%s has changed your name to %s.\r\n", GET_RNAME(ch), buf2);
     send_to_char(strng, vict);

     sprintf(strng,"%s", GET_RNAME(vict));
     guildie_rename(vict, buf2, is_file);
     // Delete -old- pfile before changing name. 
     delete_playerfile(strng);

     // Now old file is deleted, name change can take place!
     free(GET_RNAME(vict));
     SET_RNAME(vict, strdup(buf2));
     change_default_descs(vict);
     write_aliases(vict); 
     save_XML_playerfile(vict, CRASH_FILE);
     save_char(vict, NOWHERE);
     update_p_table(vict);
     save_all_guilds();
     sprintf(to_log, "%s %s.", to_log, GET_RNAME(vict));
     break;
  case 57: 
    if (on)
      SET_BIT(GET_DFINTR(vict),DRF_GREYMAN);
    else
      REMOVE_BIT(GET_DFINTR(vict),DRF_GREYMAN);
    break;

  case 58:
   if (on)
     SET_BIT(GET_DFINTR(vict),DRF_GHOLAM);
   else
     REMOVE_BIT(GET_DFINTR(vict),DRF_GHOLAM);
   break;
   case 59:
     if (GET_NOBILITY(vict))
      free(GET_NOBILITY(vict));
     if (!strcmp(val_arg, "off")){
       SET_NOBILITY(vict,  NULL);
       act("$N's nobility title has been turned off.", TRUE, ch, NULL, vict,
       TO_CHAR);
       act("$n has turned off your nobility title.", TRUE, ch, NULL,
       vict, TO_VICT); 
       sprintf(to_log, "%s OFF.",to_log);
       break;
     }
     SET_NOBILITY(vict,  str_dup(val_arg));
     sprintf(buf, "%s has been set to obility title %s.\r\n", GET_NAME(vict), 
        GET_NOBILITY(vict));
     send_to_char(buf, ch);
     sprintf(buf, "$n has set you to nobility, title %s.",
        GET_NOBILITY(vict));
    act(buf, TRUE, ch, NULL, vict, TO_VICT);
    sprintf(buf, "Okay.");
    sprintf(to_log, "%s nobility title %s.", to_log, GET_NOBILITY(vict));
     break;
   case 60:
     break;
  case 61:
        if (GET_LEVEL(ch)<LVL_GRGOD)
           {
           send_to_char("You aren't holy enough for this.\r\n",ch);
           return;
           }
    sprintf(buf,"%s's random flow flag is %i.\r\n",GET_NAME(vict),(int)GET_POWER(vict));
    send_to_char(buf, ch);
    sprintf(buf,"%s has just checked %s's flowcap.",GET_NAME(ch),GET_NAME(vict));
    xlog(SYS_SET, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), buf, TRUE);
    break;
  case 62:
    if (GET_LEVEL(ch) < LVL_GRGOD)
      {
      send_to_char("You aren't holy enough for this command.\r\n", ch);
      return;
      }
    on ? SET_BIT(GET_PERMANENT(vict),PRM_GENTLED) : REMOVE_BIT(GET_PERMANENT(vict),PRM_GENTLED);
    break;
  case 63:
    TOGGLE_BIT(PLR_FLAGS(vict), PLR_NOGAIN);
    break;
  case 64:
    if (GET_LEVEL(ch) > GET_LEVEL(vict)) 
    {
      SET_OR_REMOVE(PLR_FLAGS(vict), PLR_CANTPOST);
      sprintf(to_log, "%s set %s cantpost to %s.", GET_NAME(ch), GET_NAME(vict), ONOFF(PLR_FLAGGED(vict, PLR_CANTPOST)));
      xlog(SYS_SET, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), to_log, TRUE);
      return;
    } 
    else 
    {
      send_to_char("I don't think so.\r\n", ch);
      return;
    }
    break;
  case 65:
    GET_COMPULSION(vict) = RANGE(0,3);
    sprintf(to_log,"%s set %s compulsion to %ld.\r\n",GET_NAME(ch),GET_NAME(vict),GET_COMPULSION(vict));    
    break;
  case 66:
    i = real_object(value);
    if (value && (i < 0)) {
      send_to_char("There is no object with that vnum.\r\n", ch);
      return;
    }
    if (value && (GET_OBJ_TYPE(&obj_proto[i]) != ITEM_WAGON)) {
      act("$p is not a wagon.", TRUE, ch, &obj_proto[i], 0, TO_CHAR);
      return;
    }
    GET_WAGON(vict) = value;
    sprintf(to_log,"%s set %s wagon to %d.\r\n", GET_NAME(ch), GET_NAME(vict), value);
    break;
  case 67:
    GET_TAINT(vict) = value;
    sprintf(to_log,"%s set %s taint to %08lx.\r\n",GET_NAME(ch),GET_NAME(vict),GET_TAINT(vict));
    break;
  case 68:
    if (on) {
      SET_BIT(GET_PERMANENT(vict),PRM_TAINTRES);
    }
    else {
      REMOVE_BIT(GET_PERMANENT(vict),PRM_TAINTRES);
    }
    break;
  case 69:
    found = race = 0;
    for (i = 0; ((i < MAX_RACES) && (!found)); i++) {
      if (!strncasecmp(hometown[i],val_arg,strlen(val_arg))) {
        found = 1;
    race  = i;
        break;
      }
    }
    if (!found) {
      asend_to_char(ch,"The race %s doesn't exist!\r\n",val_arg);
      return;
    }
    GET_RACE(vict) = race;
    asend_to_char(ch,"Okay, %s is now from %s\r\n",GET_NAME(vict),hometown[race]);
    asend_to_char(vict,"%s has set your race to %s.\r\n",GET_NAME(ch),hometown[race]);
    break;
  case 70:
     while (strcmp(blademastery[temp], "\n") &&
       strncasecmp(blademastery[temp], val_arg, strlen(val_arg)))
       temp++;
     if (!(strcmp(blademastery[temp], "\n")))
     {
       send_to_char("There is no such blademastery rank.\r\n", ch);
       return;
     }

     GET_BM(vict) = temp;

     update_p_table(vict);
     sprintf(buf, "%s has been set to blademastery rank %s.\r\n", GET_NAME(vict),
        blademastery[temp]);
     send_to_char(buf, ch);
     sprintf(buf, "%s has set you to blademastery ranking %s.\r\n", GET_NAME(ch),
        blademastery[temp]);
     sprintf(to_log, "%s blademastery title %s.", to_log, blademastery[temp]);
     send_to_char(buf, vict);
     break;
   case 71:
    break;
   case 72:
    break;
  case 73:
    break;
  case 74:
    break;
  case 75:
    break;   
  case 76:
    SET_OR_REMOVE((vict)->player_specials->saved.command_groups, CG_NEWBIE);
  break;
  case 77:
    SET_OR_REMOVE((vict)->player_specials->saved.command_groups, CG_GL);
  break;
  case 78:
    SET_OR_REMOVE((vict)->player_specials->saved.command_groups, CG_ST);
  break;
  case 79:
    SET_OR_REMOVE((vict)->player_specials->saved.command_groups, CG_GL_BUILDER);
  break;
  case 80:
    SET_OR_REMOVE((vict)->player_specials->saved.command_groups, CG_BUILDER);
  break;
  case 81:
    SET_OR_REMOVE((vict)->player_specials->saved.command_groups, CG_ADMIN);
  break;
  case 82:
    SET_OR_REMOVE((vict)->player_specials->saved.command_groups, CG_SM);
  break;
  case 83:
    SET_OR_REMOVE((vict)->player_specials->saved.command_groups, CG_HGL);
  break;
  case 84:
    SET_OR_REMOVE((vict)->player_specials->saved.command_groups, CG_HB);
  break;
  case 85:
    SET_OR_REMOVE((vict)->player_specials->saved.command_groups, CG_OWNER);
  break;
  case 86:
    SET_OR_REMOVE((vict)->player_specials->saved.command_groups, CG_WEAVER);
  break;
  case 87:
    SET_OR_REMOVE((vict)->player_specials->saved.command_groups, CG_HCODER);
  break;
  case 88:
    SET_OR_REMOVE((vict)->player_specials->saved.command_groups, CG_HCOUNCIL);
  break;
  case 89:
    SET_OR_REMOVE((vict)->player_specials->saved.command_groups, CG_NEWBIEPR);
  break;
  case 90: 
  case 91: 
  case 92:
    /*If no char in argument*/
    if (!*val_arg){
      send_to_char("Talent name expected.\n\r", ch);
      return;
    }

    /*if not in quotes*/   
    if (*val_arg != '"') {
      send_to_char("Skill must be enclosed in: \" \"\n\r", ch);
      return;
    }
    /* extract the talent name by locating the last quote */
    
    if ((tmp_ptr = strchr(val_arg+1, '"')) == NULL) {
      send_to_char("Skill must be enclosed in: \" \"\r\n", ch);
      return;
    }

    *(tmp_ptr++) = '\0';
    skip_spaces(&tmp_ptr);
   
    /*find talent in array */
    for (i = 0, found = 0; i < NUM_TALENTS ; i++)
      if (is_abbrev(val_arg+1, master_talent_list[i].name)) {
    found = 1;
        break;
      }

    if (!found) {
      send_to_char("You want to set -which- talent?!?\r\n", ch);
      return;
    }

    if (!*tmp_ptr) {
      send_to_char("On/off expected!!\r\n", ch);
      return;
    }

    if (is_abbrev(tmp_ptr, "on")) {
      on = 1;
      off = 0;
    }
    else if (is_abbrev(tmp_ptr, "off")) {
      off = 1;
      on = 0;
    }
    else {
      send_to_char("Do -what- to the talent?!?\r\n", ch);
      return;
    }

    switch(l) {
      case 90:
        if (master_talent_list[i].primtalent)
          SET_OR_REMOVE_TALENT(GET_PRIMTALENTS1(vict), GET_PRIMTALENTS2(vict), i);
        else {
      send_to_char("That is not available as a Primary talent.", ch);
      return;
    }
    break;
      case 91:
    if (master_talent_list[i].sectalent)
      SET_OR_REMOVE_TALENT(GET_SECTALENTS1(vict), GET_SECTALENTS2(vict), i);
    else{
      send_to_char("That is not available as a Secondary talent.", ch);
      return;
    }
    break;
      case 92:
    if (master_talent_list[i].sskill)
      SET_OR_REMOVE_TALENT(GET_SPSKILLS1(vict), GET_SPSKILLS2(vict), i);
    else {
      send_to_char("That is not available as a Special Skill.", ch);
      return;
    }
    break;
    }
    sprintf(to_log, "%s %s %s.", to_log, master_talent_list[i].name, (on ? "on" : "off"));
    break;
  case 93:
    /*If no char in argument*/
    if (!*val_arg){
      send_to_char("talent name expected.\n\r", ch);
      return;
    }
    /*if not in quotes*/
    if (*val_arg != '\'') {
      send_to_char("Skill must be enclosed in: ''\n\r", ch);
      return;
    }
    /* extract the talent name by locating the last quote */

    if ((tmp_ptr = strchr(val_arg+1, '\'')) == NULL) {
      send_to_char("Skill must be enclosed in: ''\r\n", ch);
      return;
    }

    *(tmp_ptr++) = '\0';

    /*find talent in array */
    for (i = 0, found = 0; i < NUM_TALENTS ; i++)
      if (is_abbrev(val_arg+1, master_talent_list[i].name)) {
    found = 1;
    break;
      }
    if (!found) {
      send_to_char("-Which- talent?!?!?\r\n", ch);
      return;
    }

    if (!*tmp_ptr) {
      send_to_char("You need to specify on/off!!\r\n", ch);
      return;
    }

    if (is_abbrev(tmp_ptr, "on")) {
      on = 1;
      off = 0;
    }
    else if (is_abbrev(tmp_ptr, "off")) {
      off = 1;
      on = 0;
    }
    else {
      send_to_char("Do -what- to the talent?!?\r\n", ch);
      return;
    }

    /*set bit*/
    SET_OR_REMOVE((vict)->player_specials->saved.talents, (1 << i));
    sprintf(to_log, "%s %s %s.", to_log, master_talent_list[i].name, (on ? "on" : "off"));
    break;
  case 94:
    SET_OR_REMOVE(PLR_FLAGS(vict), PLR_NOMAIL);
    break;
  case 95:
    SET_OR_REMOVE((vict)->player_specials->saved.command_groups, CG_WEBMASTER);
    break;
  case 96:
    SET_OR_REMOVE((vict)->player_specials->saved.command_groups, CG_DGS);
    break;
  case 97:
    if (!*val_arg) {
      asend_to_char(ch,"Usage: set  <name> pretitle <title>|<off>\r\n");
      return;
    }

    if (c_strlen(val_arg) > 11) {
      asend_to_char(ch,"Pretitles (excluding colour codes!) must be 11 characters or less!");
      return;
    }

    if (!strcasecmp(val_arg,"off")) {
      if (GET_PRETITLE(vict))
    free(GET_PRETITLE(vict));
      GET_PRETITLE(vict) = NULL;
      sprintf(to_log,"%s set %s pretitle off.",GET_NAME(ch),GET_NAME(vict));
    } else {
      if (GET_PRETITLE(vict)) 
    free(GET_PRETITLE(vict));
      GET_PRETITLE(vict) = strdup(val_arg);
      sprintf(to_log,"%s set %s pretitle to %s.",GET_NAME(ch),GET_NAME(vict),GET_PRETITLE(vict));
    }
    break;
  case 98:
    if (GET_LEVEL(ch) > GET_LEVEL(vict)) {
      SET_OR_REMOVE(PLR_FLAGS(vict), PLR_CANTGECHO);
      sprintf(to_log, "%s set %s cantgecho to %s.", GET_NAME(ch), GET_NAME(vict), ONOFF(PLR_FLAGGED(vict, PLR_CANTGECHO)));
    } else {
      send_to_char("I don't think so.\r\n", ch);
      return;
    }
    break;
  case 99:
    SET_OR_REMOVE((vict)->player_specials->saved.command_groups, CG_AEDIT);
    break;
  default:
    sprintf(buf, "Can't set that!");
    do_log++;
    break;
  }
  update_p_table(vict);
  if(l!=61)
      {
      if (!do_log)
          xlog(SYS_SET, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), to_log, TRUE);

      if (fields[l].type == BINARY) {
        sprintf(buf, "%s %s for %s.\r\n", fields[l].cmd, ONOFF(on),
            GET_NAME(vict));
        CAP(buf);
      } else if (fields[l].type == NUMBER) {
        sprintf(buf, "%s's %s set to %d.\r\n", GET_NAME(vict),
            fields[l].cmd, value);
      } else
        strcpy(to_log, buf);
        strcat(buf, "\r\n");
      send_to_char(CAP(buf), ch);

      if (!is_file && !IS_NPC(vict))
        save_char(vict, NOWHERE);

      if (is_file) {
        vict->desc = ch->desc;
        save_char(vict,NOWHERE);
        vict->desc = NULL;

        free_char(vict);
        send_to_char("Saved in file.\r\n", ch);
      }
      }
}


static char *logtypes[] = {
"off", "brief", "normal", "complete", "\n"};

ACMD(do_syslog)
{
  int tp;

  one_argument(argument, arg);

  if (!*arg) {
    tp = ((PRF_FLAGGED(ch, PRF_LOG1) ? 1 : 0) +
      (PRF_FLAGGED(ch, PRF_LOG2) ? 2 : 0));
    sprintf(buf, "Your syslog is currently %s.\r\n", logtypes[tp]);
    send_to_char(buf, ch);
    return;
  }
  if (((tp = search_block(arg, (const char **)logtypes, FALSE)) == -1)) {
    send_to_char("Usage: syslog { Off | Brief | Normal | Complete }\r\n", ch);
    return;
  }
  REMOVE_BIT(PRF_FLAGS(ch), PRF_LOG1);
  REMOVE_BIT(PRF_FLAGS(ch), PRF_LOG2);
  SET_BIT(PRF_FLAGS(ch), (PRF_LOG1 * (tp & 1)));
  SET_BIT(PRF_FLAGS(ch), (PRF_LOG2 * (tp & 2) >> 1));

  sprintf(buf, "Your syslog is now %s.\r\n", logtypes[tp]);
  send_to_char(buf, ch);
}

ACMD(do_ajahset)
{
  char arg1[200], *arg2 = NULL, *name;
  short failed = FALSE, found = FALSE, i = 0, new_ajah = -1, is_file = 0;
  short type = -1;  /* 1 for ajah, 2 for head, 3 for sitter */
  struct char_guild_element *element;
  struct guildie_info *guildie = 0;
  struct char_data *victim;

  if (!*argument) {
    send_to_char("Syntax: ajahset <playername> <ajah|sitter|head> <ajah>\r\n", ch);
    return;
  }

  bzero(arg1, 200);

  argument = one_argument(argument, arg1);
  skip_spaces(&argument);
  name = str_dup(arg1);

  element = GET_CHAR_GUILDS(ch);

  while (element) {
    if (STATUS_FLAGGED(element->guildie, STATUS_GL) &&
        ((element->guild->id == 53))) /* white or rebel tower */
      break;
    element = element->next;
  }

  victim = get_char_vis(ch, name);
  if (!victim) {
    is_file = 1;
    if (NULL == (victim = xmlload_char(name))) {
      send_to_char("There is no such player.\r\n", ch);
      return;
    }
    if ((GET_LEVEL(victim) >= GET_LEVEL(ch) && GET_IDNUM(ch) != 1) && (GET_LEVEL(ch) < LVL_IMPL - 1)) {
      free_char(victim);
      send_to_char("Sorry, you can't do that.\r\n", ch);
      return;
    }
  fetch_char_guild_data(victim);
  }
  
  if (element) {
    guildie = element->guild->guildies;
    while(guildie) {
      if (!strncasecmp(guildie->name, name, strlen(guildie->name))) break;
      guildie = guildie->next;
    }
  }

  if (!element) {
    send_to_char("Hey! You don't even lead that Guild!\n\r", ch);
    failed = TRUE;
  }

  if (!guildie) {
    send_to_char("That person is not part of your guild.\r\n", ch);
    failed = TRUE;
  }

  if (!*argument && failed == FALSE) {
    send_to_char("Specify either ajah, head or sitter to be set.\r\n", ch);
    failed = TRUE;
  } else if (failed == FALSE) {
    argument = one_argument(argument, arg1);
    skip_spaces(&argument);
    if (!strncasecmp(arg1, "ajah", strlen(arg1))) {
      type = 1;
    } else if (!strncasecmp(arg1, "head", strlen(arg1))) {
      type = 2;
    } else if (!strncasecmp(arg1, "sitter", strlen(arg1))) {
      type = 3;
    } else {
      send_to_char("You must choose one of either ajah, head or sitter to set.\r\n", ch);
      failed = TRUE;
    }
  }
   

  arg2 = argument;
  if (!*arg2 && failed == FALSE) {
    switch (type) {
      case 1: 
        send_to_char("You must give an ajah name or \"off\" to set.\r\n", ch);
        break;
      case 2:
      case 3:  
        send_to_char("You must supply either \"on\" or \"off\" to set.\r\n",
                     ch);
        break;
      default: 
        send_to_char("Secify either ajah, head or sitter to be set.\r\n", ch);
        break;
    }
    failed = TRUE;
  } else if (failed == FALSE) {
    if (type == 1) {
      for (i = 0; (strcmp(ajahs[i], "NO MORE AJAHS"))&&(!found); i++)
        if (!strncasecmp(arg2, ajahs[i], strlen(arg2))) {
          found = TRUE;
          new_ajah = i;
        }
      if (found == FALSE && strncasecmp(arg2, "off", strlen(arg2)) != 0) {
        send_to_char("You must supply either an ajah or \"off\".\r\n", ch);
        failed = TRUE;
      }
      if (found == FALSE && failed == FALSE) /* "off" */
       new_ajah = -1;
    }
    if (type == 2 || type == 3) {
      if (strncasecmp(arg2, "off", strlen(arg2)) == 0) {
        new_ajah = 0;
      } else if (strncasecmp(arg2, "on", strlen(arg2)) == 0) {
        new_ajah = 1;
      } else {
        send_to_char("You must provide either \"on\" or \"off\".\r\n", ch);
        failed = TRUE;
      }
    }
  }


  if (failed == TRUE) {
    if (is_file) {
      remove_char_guild_data(victim);
      free_char(victim);
    }
    return;
  }
     
  if (type == 1) {
    if (new_ajah == -1) /* setting to "off" */
    {
      if (PLR_FLAGGED(victim, PLR_HEAD)) {
        asend_to_char(ch, "Removing %s as Head of Ajah.\r\n",
          GET_NAME(victim));
        REMOVE_BIT(PLR_FLAGS(victim), PLR_HEAD);
      }
      if (PLR_FLAGGED(victim, PLR_SITTER)) {
        asend_to_char(ch, "Removing %s as Sitter of Ajah.\r\n",
          GET_NAME(victim));
        REMOVE_BIT(PLR_FLAGS(victim), PLR_SITTER);
      }
      act("You remove the Ajah setting for $N.", TRUE, ch, 0, victim, TO_CHAR);
      act("$n has removed you from your Ajah.", TRUE, ch, 0, victim, TO_VICT);
      GET_AJAH(victim) = 0;
    } else { /* setting ajah */
      new_ajah++;
      GET_AJAH(victim) = new_ajah;
      sprintf(buf, "You have added $N to the %s Ajah.", ajahs[new_ajah - 1]);
      act(buf, TRUE, ch, 0, victim, TO_CHAR);
      sprintf(buf, "$n has added you to the %s Ajah.", ajahs[new_ajah - 1]);
    }
  } else if (type == 2) {
    if (new_ajah == 0) { /* "off" */
      if (PLR_FLAGGED(victim, PLR_HEAD)) {  
        REMOVE_BIT(PLR_FLAGS(victim), PLR_HEAD);
        act("You remove $N from position of Head of Ajah.", TRUE, ch, 0,
            victim, TO_CHAR);
        act("$n removes you from position of Head of your Ajah.", TRUE, ch,
            0, victim, TO_VICT);
      } else {
        asend_to_char(ch, "%s is already _not_ Head of her Ajah.\r\n",
                      GET_NAME(victim));
        failed = TRUE;
      }
    } else if (new_ajah == 1) { /* "on" */
      if (!PLR_FLAGGED(victim, PLR_HEAD)) {  
        SET_BIT(PLR_FLAGS(victim), PLR_HEAD);  
        act("You make $N Head of her Ajah.", TRUE, ch, 0,
            victim, TO_CHAR);
        act("$n makes you the Head of your Ajah.", TRUE, ch, 0,
            victim, TO_VICT);
      } else {
        asend_to_char(ch, "%s is already Head of her Ajah.\r\n",
                      GET_NAME(victim));
        failed = TRUE;
      }
    }
  } else if (type == 3) {
    if (new_ajah == 0) { /* "off" */
      if (PLR_FLAGGED(victim, PLR_SITTER)) {
        REMOVE_BIT(PLR_FLAGS(victim), PLR_SITTER);
        act("You remove $N from position of Sitter for her Ajah.", TRUE,
            ch, 0, victim, TO_CHAR);
        act("$n removes your from position of Sitter for your Ajah.", TRUE,
            ch, 0, victim, TO_VICT);
      } else {
        asend_to_char(ch, "%s is already _not_ a Sitter for her Ajah.\r\n",
                      GET_NAME(victim));
        failed = TRUE;
      }
    } else if (new_ajah == 1) { /* "on" */
      if (!PLR_FLAGGED(victim, PLR_SITTER)) {
        SET_BIT(PLR_FLAGS(victim), PLR_SITTER);
        act("You make $N a Sitter for her Ajah.", TRUE, ch, 0,
            victim, TO_CHAR);
        act("$n makes you a Sitter for your Ajah.", TRUE, ch, 0,
            victim, TO_VICT);
      } else {
        asend_to_char(ch, "%s is already a Sitter for her Ajah.\r\n",
                      GET_NAME(victim));
        failed = TRUE;
      }
    }
  }


  if (failed == TRUE) {
    if (is_file == 1) {
      remove_char_guild_data(victim); 
      free_char(victim);
    }
    return;
  }


  save_char(victim,NOWHERE);
  send_to_char("Saved....",ch);

}

ACMD(do_zap)
{
  struct char_data *victim;
  char t_buf[200],arg1[100];
  long gear = 0, skills = 0;

  one_argument(argument,arg1);

  if (GET_LEVEL(ch) == LVL_IMPL) {
    if (!*arg1) {
      send_to_char("Wanna zap who?\r\n",ch);
      return;
    }

    if (!(victim = get_char_vis(ch,arg1))) {
      send_to_char("That person isn't here!\r\n",ch);
      return;
    }
    if (IS_NPC(victim)) {
      send_to_char("Hey, not on mobs!\r\n",ch);
      return;
    }

    if (victim == ch) {
      send_to_char("Doesn't seem's like a good idea too me.\r\n",ch);
      return;
    }

    sprintf(t_buf,"%s has been bale-fired by %s.\r\n",GET_NAME(victim),GET_NAME(ch));
    act("A bar of liquid fire hits $n, and $e simply ceases to exist.",TRUE,victim,0,0,TO_ROOM);
    send_to_char("You have been bale-fired by the Gods!\r\nLearn to behave next time!\r\n",victim);
    GET_AUTHORIZED(victim) = TRUE;

    get_qs(ch,&skills,&gear);
    if (skills || gear || GET_QP(ch)) {
      do_delete(victim," yes delete me and my qps!",0,0);
    } else {
      do_delete(victim," yes delete me",0,0);
    }
    mlog(t_buf);
  }
}

void do_newbie_eq(struct char_data *ch)
{
  struct obj_data *temp_obj;
  const int aiel_wpn = 42;
  /* original newbie equipment
  const int newbie_vnum[] = { 10 , 23105 , 11 , 14, 15, 17, 16, 12, 18, 19, 13, -1};
  const int wear_where[]  = { WEAR_WIELD_R,
                 WEAR_HOLD,
                 WEAR_SHIELD,
                              WEAR_LEGS,
                 WEAR_HEAD,
                 WEAR_ARMS,
                 WEAR_HANDS,
                 WEAR_FEET,
                 WEAR_NECK_1,
                 WEAR_LIGHT,	
                      WEAR_BODY,
                 -1 };
  */
  /*
    10 = a plain short sword
 23105 = a welcome cookie
    11 = a small shield
    14 = a pair of woolen pants
    15 = a glittering metal helmet
    17 = a pair of studded leather arm plates
    16 = a pair of simple gloves
    12 = a worn pair of shoes
    18 = a simple silver chain necklace
    19 = a tarred torch
    13 = a plain woolen shirt

    42 = a short, very plain, Aiel spear
     9 = a waterskin
 30067 = a hot, crusty loaf of honey wheat bread
  */

  const int newbie_vnum[] = { 10 , 23105 , 11, 19, 13, -1};
  const int wear_where[]  = { WEAR_WIELD_R,
                 WEAR_HOLD,
                 WEAR_SHIELD,
                 WEAR_LIGHT,	
                      WEAR_BODY,
                 -1 };

  int i = 0;

  for (i = 0; i < 5; i ++) {
    temp_obj = read_object(30067, VIRTUAL); // bread
    if (temp_obj)
      obj_to_char(temp_obj, ch);
  }
  temp_obj = read_object(9, VIRTUAL); // Water skin.
  if (temp_obj)
    obj_to_char(temp_obj, ch);

  if (GET_RACE(ch) == RACE_AIEL) {
    temp_obj = read_object(aiel_wpn,VIRTUAL);
  } else {
    temp_obj = read_object(newbie_vnum[0],VIRTUAL);
  }  
  equip_char(ch,temp_obj,wear_where[0]);
  
  i = 1;
  while ((newbie_vnum[i] != -1) || (wear_where[i] != -1)) {
    temp_obj = read_object(newbie_vnum[i], VIRTUAL);
    if (temp_obj) {  
      equip_char(ch, temp_obj, wear_where[i]);
    }
    i++;
  }
}

ACMD(do_nosnoop){
  TOGGLE_BIT(PLR_FLAGS(ch), PLR_WIZNOSNOOP);
  if (PLR_FLAGGED(ch, PLR_WIZNOSNOOP))
    send_to_char("You can no longer be snooped.\r\n", ch);
  else
    send_to_char("You can now be snooped freely by higher level immortals.\r\n", ch);
}

ACMD(do_privacy)
{

  if (!GET_SKILL(ch, SKILL_PRIVACY)) {
    send_to_char("Sorry, you haven't been privileged to use thise command.\r\n", ch);
    return;
  }
  TOGGLE_BIT(PLR_FLAGS(ch), PLR_WIZNOSNOOP);
  if (PLR_FLAGGED(ch, PLR_WIZNOSNOOP))
    send_to_char("You can no longer be snooped.\r\n", ch);
  else
    send_to_char("You can now be snooped freely by the Immortals.\r\n", ch);
}

ACMD(do_playerlist)
{
  long id;
  long from =1, to = 99999;
  int cnt = 0, total = 0,
    lowlvl = 0, highlvl = 211,  deleted = 0, class = -1, sex = -1, nogain = 0;
  char buf[MAX_STRING_LENGTH*8], *ptr = NULL, name[MAX_INPUT_LENGTH];

  sprintf(buf, "Players");

  ptr = argument;
  *name = '\0';

  while((ptr = strchr(ptr, '-'))) {
    switch(*(++ptr)) {
      case 'i':
        if (sscanf(ptr+1, "%ld %ld", &from, &to) != 2)
          break;
        sprintf(buf, "%s with idnums ranging from %ld to %ld", buf, from, to);
        continue;

      case 'n':
        if (sscanf(ptr+1, "%s", name) != 1)
          break;
        sprintf(buf, "%s with name matching %s", buf, name);
        continue;

      case 'd':
        deleted = TRUE;
        sprintf(buf, "%s with the 'deleted' flag set", buf);
        continue;

      case 'l':
        if (sscanf(ptr+1, "%d %d", &lowlvl, &highlvl) != 2)
          break;
        sprintf(buf, "%s with levels ranging from %d to %d", buf, lowlvl, highlvl);
        continue;

      case 'c':
	if (sscanf(ptr+1, "%s", buf2) != 1)
          break;
        if ((class = search_block(buf2, pc_class_types, FALSE)) == -1)
	  break;
        sprintf(buf, "%s with class of %s", buf, pc_class_types[class]);
	continue;

      case 's':
	if (sscanf(ptr+1, "%s", buf2) != 1)
	  break;
	if ((sex = search_block(buf2, genders, FALSE)) == -1)
	  break;
	sprintf(buf, "%s with sex of %s", buf, genders[sex]);
	continue;

      case 'g':
        nogain = 1;
        sprintf(buf, "%s with the '!GAIN' flag set", buf);
	continue;

      default:
        break;
    }
    send_to_char("Format: playerlist [-i <from idnum> <to idnum>] [-l <from level> <to level>] [-n <name>] [-c <class>] [-d] [-g] [-s <sex>]\n", ch);
    return;
  }
  sprintf(buf, "%s:\r\n", buf);
  send_to_char(buf, ch);
  sprintf(buf, "\r\n");

  for(id = 0, total = 0; id <= top_of_p_table; id++) {
    if ((player_table[id].id < from) || (player_table[id].id > to))
      continue;
    if ((player_table[id].level < lowlvl) || (player_table[id].level > highlvl))
      continue;
    if ((deleted) && !(player_table[id].deleted))
      continue;
    if ((*name) && strncasecmp(name, player_table[id].name, strlen(name)))
      continue;
    if (class != -1 && player_table[id].class != class)
      continue;
    if (sex != -1 && player_table[id].sex != sex)
      continue;
    if ((nogain) && !(player_table[id].nogain))
      continue;
    total++;

    strcpy(buf2, player_table[id].name);
    sprintf(buf, "%s[%5ld] %-20s [%3d %s] - %s", buf, player_table[id].id,
      CAP(buf2),
      player_table[id].level, class_abbrevs[(int)player_table[id].class],
      ctime(&(player_table[id].last_logon)));
    cnt = strlen(buf);
    if (cnt > ((MAX_STRING_LENGTH*8) - 192)) {
      cnt = -1;
      break;
    }
  }

  sprintf(buf, "%s%d out of %d players listed.\r\n", buf, total, top_of_p_table+1);
  if (cnt == -1)
    sprintf(buf, "%sAn overflow occured in processing, list should be longer.\r\n", buf);

  page_string(ch->desc,buf,1);
  return;
}

ACMD(do_clearaff)
{
  struct char_data *victim;

  if (!*argument){
    send_to_char("Syntax: Clearaff <playername>\r\n", ch);
    return;
  }
  one_argument(argument, arg);
  if (!(victim = get_char_vis(ch,arg))) {
      send_to_char("That person isn't here!\r\n",ch);
      return;
    }
  AFF_FLAGS(victim) = 0;
  sprintf(buf, "Your affection flags have all been cleared by %s.\r\n", GET_NAME(ch));
  send_to_char(buf, victim);
  send_to_char("All affection flags cleared.\r\n", ch);  
}

ACMD(do_blademastery)
{
  int k = 0;

  sprintf(buf,"The rankings of Blademasters and wannabe's\n");
  for (k=0;strcmp(blademastery[k], "\n");k++)
  {
    sprintf(buf,"%s%-50s\r\n",buf,blademastery[k]);
  }
  page_string(ch->desc,buf,1);
                               
}

ACMD(do_zlock)
{
  int temp = 0, i = 1;

  skip_spaces(&argument);

  if (!*argument) {
    sprintf(buf,"These zones are currently locked: \r\n");
    for (temp = 0; temp <= top_of_zone_table; temp ++)  {
      if (zone_table[temp].lock) {
        sprintf(buf, "%s%10d", buf,zone_table[temp].number);
        i++;
        if (!(i%5))
          sprintf(buf, "%s\r\n", buf);
      }
    }
    send_to_char(buf, ch);
    return;
  }
  if (!(i = atoi(argument))) {
    send_to_char("You must type in a number as the argument.\r\n", ch);
    return;
  }
  for (temp = 0; temp < top_of_zone_table; temp ++)
    if (zone_table[temp].number == i)
      break;
  if (temp == top_of_zone_table) {
    send_to_char("There is no zone with that number.\r\n", ch);
    return;
  }
  zone_table[temp].lock = (zone_table[temp].lock)?0:1;

  sprintf(buf, "%s has %slocked zone %d.\r\n", GET_RNAME(ch), 
    (zone_table[temp].lock)?"":"un", zone_table[temp].number);
  mlog(buf);
  send_to_char("Ok.Remember you have to zedit the zone for the change to take effect.\r\n", ch); 
}

ACMD(do_whois)
{
  skip_spaces(&argument);
  sprintf(buf, "-n %s", argument);
  sprintf(argument, " %s", buf);
  do_who(ch, argument, cmd, subcmd);
}

ACMD(do_dns)
{
   int i;
   char arg1[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH];
   char ip[MAX_INPUT_LENGTH], name[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH*20];
   struct dns_entry *dns, *tdns;

   half_chop(argument, arg1, arg2);
   
   if(!*arg1) {
     send_to_char("You shouldn't be using this if you don't know what it does!\r\n", ch);
     return;
   }

   if(is_abbrev(arg1, "delete")) {
     if(!*arg2) {
       send_to_char("Delete what?\r\n", ch);
       return;
     }
     CREATE(dns, struct dns_entry, 1);
     if(sscanf(arg2, "%d.%d.%d", dns->ip, dns->ip + 1,
       dns->ip + 2) != 3) {
       send_to_char("Delete what?\r\n", ch);
       return;
     }
     for(i = 0; i < DNS_HASH_NUM; i++) {
       if(dns_cache[i]) {
       for(tdns = dns_cache[i]; tdns; tdns = tdns->next) {
         if(dns->ip[0] == tdns->ip[0] && dns->ip[1] == tdns->ip[1] &&
           dns->ip[2] == tdns->ip[2]) {
           sprintf(arg1, "Deleting %s.\r\n", tdns->name);
           send_to_char(arg1, ch);
           tdns->ip[0] = -1;
         }
       }
     }
   }
   save_dns_cache();
   return;
  } else if(is_abbrev(arg1, "add")) { 
     two_arguments(arg2, ip, name);
     if(!*ip || !*name) {
       send_to_char("Add what?\r\n", ch);
       return;
     }  
     CREATE(dns, struct dns_entry, 1);
     dns->ip[3] = -1;
     if(sscanf(ip, "%d.%d.%d.%d", dns->ip, dns->ip + 1,
       dns->ip + 2, dns->ip + 3) < 3) {
       send_to_char("Add what?\r\n", ch);
       return;
     }
     i = (dns->ip[0] + dns->ip[1] + dns->ip[2]) % DNS_HASH_NUM;
     dns->name = str_dup(name);
     dns->next = dns_cache[i];
     dns_cache[i] = dns;
     save_dns_cache();
     send_to_char("OK!\r\n", ch);
     return;
   } else if(is_abbrev(arg1, "list")) {  
     *buf = '\0';
     for(i = 0; i < DNS_HASH_NUM; i++) {
       if(dns_cache[i]) {
         for(tdns = dns_cache[i]; tdns; tdns = tdns->next) {
           sprintf(buf, "%s%d.%d.%d.%d %s\r\n", buf, tdns->ip[0],
           tdns->ip[1], tdns->ip[2], tdns->ip[3], tdns->name);
         }
       }
     }
     page_string(ch->desc, buf, 1);
     return;
   }
}

ACMD(do_unsever)
{
  struct char_data * victim = NULL;
   
  one_argument(argument, arg);

  if (!arg || !*arg) {
    send_to_char("unsever who?\r\n", ch);
    return;
  }
   
  if (!(victim = get_char_vis(ch,arg))) {
      send_to_char("That person isn't here!\r\n",ch);
      return;
  }
   
  REMOVE_BIT(GET_PERMANENT(victim), PRM_RH_CUT | PRM_LH_CUT);
  asend_to_char(ch, "All severed parts was restored on %s!\r\n", GET_NAME(victim));
}

/*            
ACMD(do_pzap)
{
  struct char_data *vict;
  one_argument(argument, arg);

  if (!*arg) {
    send_to_char("Who do youw ant to Zap?\r\n", ch);
    return;
  }

  if (!(vict = get_char_vis(ch, arg)))
    send_to_char("There is no such player.\r\n", ch);
  else if (IS_NPC(vict))
    send_to_char("You can't do that to a mob!\r\n", ch);
  else if (GET_LEVEL(vict) >= LVL_IMMORT || GET_LEVEL(vict) >= 
GET_LEVEL(ch))
    send_to_char("I don't think so.\r\n", ch);
  else {
    GET_HIT(vict)=1;
    send_to_char("That'll teach 'em!\r\n", ch);
    act("A bar of liquid fire hits $n, and $e simply ceases to exist.",TRUE,vict,0,0,TO_ROOM);
    send_to_char("A bar of liquid fire comes from somewhere to your right and goes straight through you.\r\nBut you don't exist to notice that.\r\nLearn to behave next time!\r\n",vict);
    //send_to_char("&RA jagged bolt of lightning stabs out of the clear sky, striking you in the chest!&n\r\n", vict);
    return;
  }
} */

ACMD(do_logsearch) {

     char shname[MAX_INPUT_LENGTH], searchstr[MAX_INPUT_LENGTH];
     unsigned long l = 0, match = 0, start = 0, lines = 15;
     char *temp;
     FILE *f;
     int i;

     const struct log_struct {
         const char *name;
         const char *path;
         int minlev;
     } loginf[] = {
         {"syslog"   , LF_SYSLOG     , LVL_GOD},
         {"syslog1"  , LF_SYSLOG1    , LVL_GOD},
         {"syslog2"  , LF_SYSLOG2    , LVL_GOD},
         {"syslog3"  , LF_SYSLOG3    , LVL_GOD},
         {"syslog4"  , LF_SYSLOG4    , LVL_GOD},
         {"syslog5"  , LF_SYSLOG5    , LVL_GOD},
         {"syslog6"  , LF_SYSLOG6    , LVL_GOD},
         {"badpws"   , LF_BADPWS     , LVL_GOD},
         {"errors"   , LF_ERRORS     , LVL_GOD},
         {"godcmds"  , LF_GODCMDS    , LVL_GOD},
         {"levels"   , LF_LEVELS     , LVL_GOD},
         {"new"      , LF_NEWPLAYERS , LVL_GOD},
         {"restarts" , LF_RESTARTS   , LVL_GOD},
         {"rip"      , LF_RIP        , LVL_GOD},
         {"usage"    , LF_USAGE      , LVL_GOD},
         {"levels"   , LF_LEVELS     , LVL_GOD},
         {"crash"    , LF_CRASH      , LVL_GOD},
         {"\0",0,0}
     };

     temp = two_arguments(argument, shname, buf);

     if (!*argument) {
         send_to_char("\r\n", ch);
         for (i = 0; *loginf[i].name; i++)
             if (GET_LEVEL(ch) >= loginf[i].minlev) {
                 sprintf(buf, "%-12s    [ %-28s ]\r\n", loginf[i].name, loginf[i].path);
                 send_to_char(buf, ch);
             }
         return;
     } else if (!isdigit(*buf))
         half_chop(argument, shname, searchstr);
     else {
         lines = atol(buf);
         skip_spaces(&temp);
         strcpy(searchstr, temp);
     }

     if (subcmd == SCMD_VIEWLOG && *searchstr) {
         start = lines;
         lines = atol(searchstr);
     }

     if (subcmd == SCMD_LOGSEARCH && !*searchstr)
         send_to_char("Please, at minimum, specify a valid log name and a search string.\r\n", ch);
     else {

         for (i = 0; *loginf[i].name && !is_abbrev(shname, loginf[i].name); i++);

         if (!*loginf[i].name)
             send_to_char("No such log file.\r\n", ch);
         else if (GET_LEVEL(ch) < loginf[i].minlev)
             send_to_char("You don't have access to that log file.\r\n", ch);
         else if (!(f = fopen(loginf[i].path, "r")))
             {
             sprintf(buf, "Error opening log file %s (%s).\r\n", loginf[i].name, loginf[i].path);
             send_to_char(buf, ch);
             }
         else {

             if (subcmd == SCMD_VIEWLOG)
                 if (start)
                     for (l = 1; l < start; l++)
                         get_line(f, buf);
                 else {
                     /* count lines */
                     do { l += get_line(f, buf); } while (!feof(f));
                     rewind(f);

                     /* sync if necessary */
                     if (lines > l)
                         l = 1;
                     else {
                         start = (l - lines) + 1;
                         for (l = 1; l < start; l++)
                             get_line(f, buf);
                     }
                 }
             else
                 l = 1;

             for (;match < lines; l++) {

                 get_line(f, buf);

                 if (feof(f))
                     break;

                 if (subcmd == SCMD_VIEWLOG || str_str(buf, searchstr)) {
                     sprintf(buf2, "[%ld] %s\r\n", l, buf);
                     send_to_char(buf2, ch);
                     match++;
                 }

             }
             if (!match)
                 send_to_char("No lines matched your criteria.\r\n", ch);

             fclose(f);
         }
     }
  }

