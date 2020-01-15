/******************************************************************************
 * $Id: act.comm.c,v 1.46 2004/02/04 07:05:28 fang Exp $
 *   
 * File: act.comm.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: act.comm.c,v $
 *   Revision 1.46  2004/02/04 07:05:28  fang
 *   Tiek: One can no longer send tells to AFK players while muted.
 *
 *   Revision 1.45  2003/12/05 09:38:15  fang
 *   Tze: AFK and AFW flags now go away if you quit out, as requested by Kalenth. And tells buffer doesn't repeat your tell back to you when you send a linkdead or afk person a tesll, as requested by Marra.
 *
 *   Revision 1.44  2003/12/02 03:59:49  fang
 *   Corrected Grammatical Errors
 *
 *   Revision 1.43  2003/11/13 04:59:07  fang
 *   Added xlog message to qecho
 *
 *   Revision 1.42  2003/06/18 08:23:13  fang
 *   Fixed bug where sleeping people could not see their own messages on quest channel
 *
 *   Revision 1.41  2003/06/13 02:15:17  fang
 *   Corrected bug in say/osay which crept in from the removal of ignorelist
 *
 *   Revision 1.40  2003/06/12 07:22:12  fang
 *   Changed testing for channeling ability as per HGL request. Fixed Room Seal to work as originally intended. Commented out ignorelist. Will fully remove once certain no problems will arise
 *
 *   Revision 1.39  2003/05/29 02:31:07  fang
 *   Reply gives message to afk persons that they have tells waiting.
 *   AFW no longer buffers tells.
 *
 *   Revision 1.38  2003/05/28 07:15:02  fang
 *   Added AFW to the tells notification thing to match AFK.
 *
 *   Revision 1.37  2003/05/24 02:55:38  fang
 *   Added RPTAG for nonvictims in pemote.
 *
 *   Revision 1.36  2003/03/04 22:06:04  fang
 *   fisch: added a quick logger function to add coments to syslog
 *
 *   Revision 1.35  2003/02/04 08:45:22  fang
 *   Fixed bug in deletion of entries in ignore/notify lists. Pound can no longer be used in peaceful rooms
 *
 *   Revision 1.34  2003/02/04 08:21:50  fang
 *   RPLog no longer logs stuff from OOC-characters.
 *
 *   Revision 1.33  2003/02/03 09:04:43  fang
 *   NPC-check in add_to_rplog
 *
 *   Revision 1.32  2002/11/10 22:18:34  fang
 *   Fisch: Changeed Tells message and onl display it on AFK not AFW
 *
 *   Revision 1.31  2002/11/07 04:02:00  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.30  2002/10/29 02:48:42  fang
 *   fisch: added a [you have tells] flag set when tells sent to and AFK/AFW
 *   person.
 *
 *   Revision 1.29  2002/10/16 16:54:40  fang
 *   *** empty log message ***
 *
 *   Revision 1.28  2002/10/10 20:27:03  fang
 *   fischer: tells buffer bug SQUASHED.  It was in "do_reply" not "do_tell".
 *
 *   Revision 1.27  2002/10/08 06:00:34  fang
 *   fischer: debug type code in the tells for that off buffer command
 *
 *   Revision 1.26  2002/10/02 05:05:46  fischer
 *   fisch: added timestamp flag for Ealasaid
 *
 *   Revision 1.25  2002/09/30 22:31:04  fischer
 *   fisher: stop logging of private channels
 *
 *   Revision 1.24  2002/09/30 19:50:49  fischer
 *   fischer: tells buffer fix
 *
 *   Revision 1.23  2002/09/29 01:55:00  fischer
 *   fischer: tells buffers fix
 *
 *   Revision 1.22  2002/09/28 01:14:59  fischer
 *   fischer: debug code to find buffer error
 *
 *   Revision 1.21  2002/09/27 08:52:36  fischer
 *   fischer: minor bug fixes.. cosemetic stuff
 *
 *   Revision 1.20  2002/09/27 08:32:16  fischer
 *   fischer: added public channel buffers
 *
 *   Revision 1.19  2002/09/26 00:00:33  fischer
 *   Fisch: Added Channel list to public channels and made some changes towards
 *   making visible objects on invis admins, invisible to where.
 *
 *   Revision 1.18  2002/09/24 03:22:00  fischer
 *   fisch: Made Immortals immune to arrows from other rooms
 *
 *   Revision 1.17  2002/09/23 06:43:24  fischer
 *   Fischer: fixed a few tells that were not timestampped
 *
 *   Revision 1.16  2002/09/23 06:06:52  fischer
 *   fisch: spelling fix on tells timestamp
 *
 *   Revision 1.15  2002/09/23 05:52:41  fischer
 *   fisch: clearing out lua triggers part 1
 *
 *   Revision 1.14  2002/09/19 06:12:26  fang
 *   fisch: added "notify" to the datestamp  list
 *
 *   Revision 1.13  2002/09/19 01:54:34  fang
 *   Added a timestamp to the entries in the tell buffer
 *
 *   Revision 1.12  2002/09/16 18:22:56  fang
 *   Put in WC guild retainers and some IC checks for retainers.
 *
 *   Revision 1.11  2002/09/15 00:50:07  fang
 *   Drath: XLOG update - Categories added to xlog display automatically
 *
 *   Revision 1.10  2002/09/13 18:00:42  fang
 *   Drath: Small addition to MyColor and MyColor option added to Prompts.
 *
 *   Revision 1.9  2002/09/01 15:27:06  fang
 *   Blocked public channels for retainers.
 *
 *   Revision 1.8  2002/08/20 21:03:50  fang
 *   Drath: Hunter GSKILLS
 *
 *   Revision 1.7  2002/08/06 21:24:27  fang
 *   Drath: Reinabled all ignore settings
 *
 *   Revision 1.6  2002/08/04 18:55:34  fang
 *   Drath: TELLS from NPCs no longer added to the tells buffer
 *
 *   Revision 1.5  2002/07/28 14:11:54  fang
 *   Forgot this file when commenting out dmlloc includes...
 *
 *   Revision 1.4  2002/07/28 13:56:03  fang
 *   Removed dmlloc...just to test.
 *
 *   Revision 1.3  2002/07/26 19:39:52  fang
 *   Dath: MYCOLOR mem leak fixed.  Thanks, Fred!
 *
 *   Revision 1.2  2002/07/22 15:08:04  fang
 *   Drath: Typo fixes.
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.12  2002/07/12 03:37:32  fang
 *   Drath: MYCOLOR
 *
 *   Revision 1.11  2002/07/09 22:37:17  fang
 *   Drath: REPLYLOCK
 *
 *   Revision 1.10  2002/06/20 03:07:10  fang
 *   Maz: RPTAG
 *
 *   Revision 1.9  2002/06/10 14:24:25  fang
 *   Maz: TOGGLE PCHAN added so you can turn off being added to pchans.
 *
 *   Revision 1.8  2002/05/27 10:52:48  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.7  2002/05/13 03:03:06  fang
 *   Maz: Forgot to uncomment out is_ignoring... yes, I really am blonde in real life.
 *
 *   Revision 1.6  2002/05/13 01:50:31  fang
 *   Maz: Ignorelist bug, have to fix later.  Commented it out for now.
 *
 *   Revision 1.5  2002/05/12 23:50:46  fang
 *   Added in IGNORE system, and removed AutoHotboots until the cap can be fixed for good. -Maz
 *
 *   Revision 1.4  2002/05/06 03:53:47  fang
 *   Maz: Forgot to fix ADT.  Oops.  All better.
 *
 *   Revision 1.3  2002/05/01 13:02:18  fang
 *   Added: Cleartells.  Silly command that no one will use.  But it's there now.
 *
 *   Revision 1.2  2002/02/22 15:04:27  fang
 *   *** empty log message ***
 *
 *   Revision 1.37  2002/01/28 08:00:00  mud
 *   Telss should not be added twice when fighting any more...
 *
 *   Revision 1.36  2002/01/24 21:33:11  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.35.2.1  2002/01/18 00:44:19  mud
 *   Cleanup started for real. Removed ct, ctalk, dt, tt, wt, ot and a few other leftovers from old guild system.
 *
 *
 *   Revision 1.34  2002/01/17 00:48:50  mud
 *   Can now sell more than 1 item at a time in shops.
 *   Split the special_data.saved.act var into two vars, act reamins
 *   to hold MOB_XX flags, while special_saved.plr is now used to hold
 *   PLR_XX flags. Reason, MOB_FLAGS and PLR_FLAGS checks, when not beeing
 *   careful to check if char is NPC or not caused some commands to fail. Most
 *   noteably in shop code, when shopkeepers never told their clients anything
 *   when trade failed.
 *
 *   Revision 1.31  2001/05/22 09:07:01  mud
 *   DG-Scripts for Fang.
 *
 *   Revision 1.30  2001/05/02 12:30:49  mud
 *   Notifylists are now saved in pfile. Notifylist add <name> now first checks if there exists a player with that name before adding
 *
 *   Revision 1.29  2001/04/15 02:30:13  mud
 *   Added a bundle of fixes and notifylist.
 *
 *   Revision 1.28  2001/03/30 21:09:07  mud
 *   Added support for most 8-bit characters
 *
 *   Revision 1.27  2001/03/17 10:40:45  mud
 *   New and Improved TAR.
 *
 *   Revision 1.23  2000/12/11 11:23:58  mud
 *   Tell bug for mobs fixed
 *
 *   Revision 1.22  2000/11/26 20:10:34  mud
 *   fixed pose so immorts would see IC poses when OOC
 *
 *   Revision 1.21  2000/11/24 23:26:18  mud
 *   *** empty log message ***
 *
 *   Revision 1.20  2000/11/07 12:00:38  mud
 *   Finally. XML Mobiles are -done.
 *
 *   Revision 1.19  2000/11/06 21:05:32  mud
 *   Fixed pemote, removed null setting for no second string
 *
 *   Revision 1.17  2000/10/12 18:48:05  mud
 *   Major memory hog fix with rplog and tell buffer
 *
 *   Revision 1.15  2000/10/06 16:01:39  mud
 *   The new tell buffer and the RPlog :)

 *   Revision 1.11  2000/08/14 19:11:54  mud
 *   The new guild system.
 *
 *   Revision 1.3  2000/03/08 11:41:45  mud
 *   New who func, minor modificantion to gossip command where new gossips are
 *   pre-pended to gossip now.
 *
 *   Revision 1.2  2000/02/21 14:00:15  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/*************************************************************************
*   File: act.comm.c                                    Part of CircleMUD *
*  Usage: Player-level communication commands                             *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "conf.h"
#include "sysdep.h"

#include "config.h"
#include "constants.h"
#include "class.h"
#include "act.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "screen.h" 
#include "spells.h"
#include "guild.h"
#include "dg_scripts.h"
#include "retainer.h"

char  ChannelBuffer[8][30][MAX_STRING_LENGTH];

/* Functions and ACMDs for the buffers' system */

void add_tell_to_buffer(struct char_data *ch, char *buf )
{ int i;

  if (ch->player_specials->tells_index != MAX_TELLBUFFER_LENGTH-1) 
  { strcpy(ch->player_specials->tells[ch->player_specials->tells_index], buf);
    ch->player_specials->tells_index += 1;
  }
  else if (*ch->player_specials->tells[ch->player_specials->tells_index])
  { for (i=0; i<=MAX_TELLBUFFER_LENGTH-2; i++) 
      strcpy(ch->player_specials->tells[i], ch->player_specials->tells[i+1]);
    strcpy(ch->player_specials->tells[MAX_TELLBUFFER_LENGTH-1], buf);  
  }
  else strcpy(ch->player_specials->tells[ch->player_specials->tells_index], buf);
}

ACMD(do_cleartells)
{
 ch->player_specials->tells_index = 0;
 sprintf(buf, "&rTells cleared.\r\n&n");
 send_to_char(buf, ch);
}

ACMD(do_tells)
{ int i, num_tells,strstart;

  num_tells = (((ch->player_specials->tells_index == MAX_TELLBUFFER_LENGTH-1) &&
              (*ch->player_specials->tells[MAX_TELLBUFFER_LENGTH-1])) ? 
                MAX_TELLBUFFER_LENGTH-1 : ch->player_specials->tells_index-1);
  strstart = (IS_SET(PRF2_FLAGS(ch),PRF2_TIMESTAMP))?(0):(16);

  for (i=0; i<= num_tells; i++) 
  { sprintf(buf, "(%d) %s", i, (ch->player_specials->tells[i])+strstart);
    send_to_char(buf, ch);
  }
  if( strstart == 0 )
    send_to_char( "(Time is in Mud Time.  See \"date\" for details.)\r\n", ch );
  ch->player_specials->buffer_flag = 0;

}
 
/*
 * PS: Doing char buf[len], or char b[] in a func. declaration is pointless, it will still decay
 * to a Ptr-to-char anyway. You will not get any limit checks,. :)
 */
void add_to_rplog(struct char_data *ch, char *buf, struct char_data *from)
{
 int i;

  if (from && IS_NPC(from) && !is_retainer(from)) /* don't log from non-retainer mobs */ 
    return;

  if (!PRF_FLAGGED(from, PRF_IC)) /* don't log from OOC characters! */
    return;

  if (ch->player_specials->rplog_index != MAX_TELLBUFFER_LENGTH-1) {
    strcpy(ch->player_specials->rplog[ch->player_specials->rplog_index], buf);
    ch->player_specials->rplog_index += 1;
   } else if (*ch->player_specials->rplog[ch->player_specials->rplog_index]){
     for (i=0; i<=MAX_TELLBUFFER_LENGTH-2; i++)
       strcpy(ch->player_specials->rplog[i], ch->player_specials->rplog[i+1]);
     strcpy(ch->player_specials->rplog[MAX_TELLBUFFER_LENGTH-1], buf);
  }
  else 
    strcpy(ch->player_specials->rplog[ch->player_specials->rplog_index], buf);
}

ACMD(do_rplog)
{
 int i, num;

 num = (((ch->player_specials->rplog_index == MAX_TELLBUFFER_LENGTH-1) && 
        (*ch->player_specials->rplog[MAX_TELLBUFFER_LENGTH-1])) ? 
          MAX_TELLBUFFER_LENGTH-1 : ch->player_specials->rplog_index-1);
 for (i=0; i<= num; i++) {
  sprintf(buf, "(&c%d&n) %s", i, ch->player_specials->rplog[i]);
  send_to_char(buf, ch);
 }

}
  
ACMD(do_notifylist)
{
  char name[MAX_NAME_LENGTH+1], arg1[MAX_INPUT_LENGTH+1];
  int i;

  two_arguments(argument, arg1, name);

  if (!*arg1) {
   if (!*ch->player_specials->notifylist[0])
   {
    send_to_char("&cYour notify list is empty.&n\r\n", ch);
    return;
   }
   send_to_char("&cNotify List: &n", ch);
   for (i=0; i<MAX_NOTIFYLIST_LENGTH; i++)
    if (*ch->player_specials->notifylist[i])
    {
     sprintf(buf, "&G%s&n ", ch->player_specials->notifylist[i]);
     send_to_char(buf, ch);
    }
   send_to_char("\r\n", ch);
   return;
  }

  if (is_abbrev(arg1, "add"))
  {
   if (*ch->player_specials->notifylist[MAX_NOTIFYLIST_LENGTH-1])
   {
    send_to_char("&cYour notify list is full. Remove someone from it first.&n\r\n", ch);
    return;
   }

   if (!*name)
   {
    send_to_char("&cWhom do you want to add to your notify list ? Use 'notifylist add <name>'.&n\r\n", ch);
    return;
   }

   if (-1 == find_name(name)) {
     send_to_char("There exists no player with that name!\r\n",ch);
     return;
   }

   i=0;
   while (*ch->player_specials->notifylist[i])
    i++;

   strcpy(ch->player_specials->notifylist[i], CAP(name));
   sprintf(buf, "&c%s added to notify list.&n\r\n", name);
   send_to_char(buf, ch);
   return;
  }

  if (is_abbrev(arg1, "remove"))
  {
   if (!*ch->player_specials->notifylist[0])
   {
    send_to_char("&cYour notify list is empty already.&n\r\n", ch);
    return;
   }

   if (!*name)
   {
    send_to_char("&cWhom do you want to remove from your notify list ? Use 'notifylist remove <name>'.&n\r\n", ch);
    return;
   }

   i=0;
   while (i<MAX_NOTIFYLIST_LENGTH)
   {
    if (!strncasecmp(name, ch->player_specials->notifylist[i], strlen(name)))
     break;
    i++;
   }

   if (i == MAX_NOTIFYLIST_LENGTH)
   {
    send_to_char("&cNo such person in your notify list.&n\r\n", ch);
    return;
   }

   strcpy(name, ch->player_specials->notifylist[i]);
   strcpy(ch->player_specials->notifylist[i], "");

   while (i<MAX_NOTIFYLIST_LENGTH)
   {
    if (i == MAX_NOTIFYLIST_LENGTH-1)
    {
     /* Just empty last slot! */ 
     strcpy(ch->player_specials->notifylist[MAX_NOTIFYLIST_LENGTH-1],""); 
     i++;
    }
    else 
    {
     strcpy(ch->player_specials->notifylist[i], ch->player_specials->notifylist[i+1]);
     i++;
    }
   }

   sprintf(buf, "&c%s removed from notify list.&n\r\n", name);
   send_to_char(buf, ch);
   return;
  }

  send_to_char("&cUnrecognized option. Type 'HELP NOTIFYLIST' for known options.&n\r\n", ch);
  return;
}


/*
ACMD(do_ignorelist)
{
  char name[MAX_NAME_LENGTH+1], arg1[MAX_INPUT_LENGTH+1];
  int i;

  two_arguments(argument, arg1, name);

  if (!*arg1) {
   if (!*ch->player_specials->ignorelist[0])
   {
    send_to_char("&cYour ignore list is empty.&n\r\n", ch);
    return;
   }
   send_to_char("&cIgnore List: &n", ch);
   for (i=0; i<MAX_IGNORELIST_LENGTH; i++)
    if (*ch->player_specials->ignorelist[i])
    {
     sprintf(buf, "&G%s&n ", ch->player_specials->ignorelist[i]);
     send_to_char(buf, ch);
    }
   send_to_char("\r\n", ch);
   return;
  }

  if (is_abbrev(arg1, "add"))
  {
   if (*ch->player_specials->ignorelist[MAX_IGNORELIST_LENGTH-1])
   {
    send_to_char("&cYour ignore list is full. Remove someone from it first.&n\r\n", ch);
    return;
   }

   if (!*name)
   {
    send_to_char("&cWho do you want to add to your ignore list ? Use 'ignorelist add <name>'.&n\r\n", ch);
    return;
   }

   if (-1 == find_name(name)) {
     send_to_char("There exists no player with that name!\r\n",ch);
     return;
   }

   if (find_name_level(name) >= LVL_IMMORT) {
     send_to_char("&cYou can't ignore immortals!&n\r\n",ch);
     return;
   }

    i=0;
    while (*ch->player_specials->ignorelist[i])
     i++;

    strcpy(ch->player_specials->ignorelist[i], CAP(name));
    sprintf(buf, "&c%s added to ignore list.&n\r\n", name);
    send_to_char(buf, ch);
    return;

  }

  if (is_abbrev(arg1, "remove"))
  {
   if (!*ch->player_specials->ignorelist[0])
   {
    send_to_char("&cYour ignore list is empty already.&n\r\n", ch);
    return;
   }

   if (!*name)
   {
    send_to_char("&cWho do you want to remove from your ignore list ? Use 'ignorelist remove <name>'.&n\r\n", ch);
    return;
   }

   i=0;
   while (i<MAX_IGNORELIST_LENGTH)
   {
    if (!strncasecmp(name, ch->player_specials->ignorelist[i], strlen(name)))
     break;
    i++;
   }

   if (i == MAX_IGNORELIST_LENGTH)
   {
    send_to_char("&cNo such person in your ignore list.&n\r\n", ch);
    return;
   }

   strcpy(name, ch->player_specials->ignorelist[i]);
   strcpy(ch->player_specials->ignorelist[i], "");

   while (i<MAX_IGNORELIST_LENGTH)
   {
    if (i == MAX_IGNORELIST_LENGTH-1)
    { */
     /* Just empty last item! */
/*     strcpy(ch->player_specials->ignorelist[MAX_IGNORELIST_LENGTH-1],""); 
     i++;
    }
    else 
    {
     strcpy(ch->player_specials->ignorelist[i], ch->player_specials->ignorelist[i+1]);
     i++;
    }
   }

   sprintf(buf, "&c%s removed from ignore list.&n\r\n", name);
   send_to_char(buf, ch);
   return;
  }

  send_to_char("&cUnrecognized option. Type 'HELP IGNORELIST' for known options.&n\r\n", ch);
  return;
} */





ACMD(do_adt)
{
  struct descriptor_data *i;
  short guild_no;

  if (get_guild(ch, 53))
    guild_no = 53; /* rebel tower */
  else guild_no = 0;

  if (!GET_AJAH(ch) || !guild_no)
  {
    send_to_char("You can't use the Ajah channel if you aren't even an Aes Sedai!\r\n", ch);
    return;
  }

  if (PRF_FLAGGED(ch, PRF_NOGDT))
  {
    send_to_char("You can't use the Ajah channel while you have the guild channel off.\r\n", ch);
    return;
  }

  if (!*argument)
  {
    send_to_char("What do you want to say on the Ajah channel?",ch);
    return;
  }

  if (PLR_FLAGGED(ch, PLR_HEAD) && PLR_FLAGGED(ch, PLR_SITTER)) {
    sprintf(buf,"(%s%s&n) Head and Sitter %s:%s\\c00",
            ajah_colors[GET_AJAH(ch)-1], ajahs[GET_AJAH(ch)-1],
            GET_NAME(ch), argument);
  } else if (PLR_FLAGGED(ch, PLR_HEAD)) {
    sprintf(buf,"(%s%s&n) Head %s:%s\\c00", ajah_colors[GET_AJAH(ch)-1],
            ajahs[GET_AJAH(ch)-1], GET_NAME(ch), argument);
  } else if (PLR_FLAGGED(ch, PLR_SITTER)) {
    sprintf(buf,"(%s%s&n) Sitter %s:%s\\c00", ajah_colors[GET_AJAH(ch)-1],
            ajahs[GET_AJAH(ch)-1], GET_NAME(ch), argument);
  } else {
    sprintf(buf,"(%s%s&n) %s:%s\\c00", ajah_colors[GET_AJAH(ch)-1],
            ajahs[GET_AJAH(ch)-1], GET_NAME(ch), argument);
  }

  for (i = descriptor_list; i; i = i->next)
    if (!i->connected && !PRF_FLAGGED(i->character, PRF_NOGDT))
      if (get_guild(i->character, guild_no) && ((GET_AJAH(ch)
         == GET_AJAH(i->character)) || (GET_LEVEL(i->character) >= LVL_GOD)))
        act(buf, 0, ch, 0, i->character, TO_VICT | TO_SLEEP);
}

ACMD(do_handtalk)
{

  struct descriptor_data *i;
  int a;

 if (!GET_SKILL(ch, SKILL_HANDTALK))
  {
    send_to_char("You don't have any clue as to how to do that.\r\n", ch);
    return;
  }

  skip_spaces(&argument);

  if (!*argument)
    send_to_char("Yes, but what exactly is it that you want to signal?\r\n", ch);
  else
  {
     if ((number(0,100) <= 3) && (GET_SKILL(ch, SKILL_HANDTALK) < 100))
    {
      send_to_char("Congratulations!  You increase your proficiency in signaling!", ch);
      if (!IS_NPC(ch)) {
        int skill = GET_SKILL(ch, SKILL_HANDTALK);
        SET_SKILL(ch, SKILL_HANDTALK, skill + 1);
      }
      //GET_SKILL(ch, SKILL_HANDTALK)++;
    }
    sprintf(buf, "With a highly complex series of hand motions, you signal, '%s\\c00'\r\n", argument);
    send_to_char(buf, ch);
    for (a = 0; argument[a] != '\0'; a++)
      if ((argument[a] == ' ') && (number(0, 100) > GET_SKILL(ch, SKILL_HANDTALK)))
        for (a++ ; (argument[a] != ' ') && (argument[a] != '\0');a++)
          argument[a] = '?';

    for (i = descriptor_list; i; i = i->next)
    {
      if ((!i->connected) && (AWAKE(i->character)) && (IN_ROOM(ch) == IN_ROOM(i->character))) {
        if (!GET_SKILL(i->character, SKILL_HANDTALK))
        {
          sprintf(buf, "%s makes a series of complex hand gestures.\r\n", GET_NAME(ch));
          send_to_char(buf, i->character);
        }
        else if (ch != i->character)
        {
          for (a = 0; argument[a] != '\0'; a++)
            if ((argument[a] == ' ') && (number(0, 85) > GET_SKILL(i->character, SKILL_HANDTALK)))
              for (a++; (argument[a] != ' ') && (argument[a] != '\0'); a++)
                argument[a] = '?';

          sprintf(buf, "With a highly complex series of hand motions, %s signals, '%s\\c00'\r\n", GET_NAME(ch), argument);
          if ((number(0,100) <= 3) && (GET_SKILL(ch, SKILL_HANDTALK) < 100))
          {
            send_to_char("Congratulations!  You increase your proficiency in signaling!", ch);
            if (!IS_NPC(ch)) {
              int skill = GET_SKILL(ch, SKILL_HANDTALK);
              SET_SKILL(ch, SKILL_HANDTALK, skill + 1);
            }
            //GET_SKILL(ch, SKILL_HANDTALK)++;
          }
          send_to_char(buf, i->character);
        }
      }
    }
  }
}



ACMD(do_knock)
{
  int fromdir = -1, dir = -1, i, found = FALSE;
  struct char_data *dummy = NULL;
  char buffer[MAX_STRING_LENGTH];

  one_argument(argument, arg);
  if (!*arg){
    send_to_char("Where is that door?\r\n", ch);
    return;
  }
  
  for (i = 0; i < 6; i++)
    if (!strncasecmp(dirs[i], arg, 1)){
      dir = i;
      found = TRUE;
    }
  if (!found) {
    send_to_char(" Err...That direction is new to me...Try another one.\r\n", ch);
    return;
  }
  if (!EXIT(ch, dir)){
    send_to_char("No exit in that direction.\r\n", ch);
    return;
  }
  if (!IS_SET(EXIT(ch, dir)->exit_info, EX_CLOSED) &&
     !IS_SET(EXIT(ch, dir)->exit_info, EX_LOCKED)){
    send_to_char("That door isn't closed.\r\n", ch);
    return;
  }
  sprintf(buffer,"You politely knock on the %s.\r\n", EXIT(ch, dir)->keyword ? fname(EXIT(ch, dir)->keyword) : "door");
  send_to_char(buffer, ch);
  sprintf(buffer, "$n knocks on the %s %s.", EXIT(ch, dir)->keyword ? fname(EXIT(ch, dir)->keyword) : "door", dirs[dir]);
  act(buffer, TRUE, ch, 0, 0, TO_ROOM);
  for (i = 0; i < NUM_OF_DIRS; i++)
    if (world[EXIT(ch, dir)->to_room].dir_option[i]) 
      if (world[EXIT(ch, dir)->to_room].dir_option[i]->to_room == ch->in_room)
        fromdir = i;
  dummy = world[EXIT(ch, dir)->to_room].people;
  if ((fromdir > -1) && dummy)
  {
    sprintf(buffer, "You hear someone knock on the %s %s.\r\n", 
      EXIT(dummy, fromdir)->keyword ? fname(EXIT(dummy, fromdir)->keyword) : "door", dirs[fromdir]);
    act(buffer, FALSE, dummy, 0, 0, TO_ROOM);
    send_to_char(buffer, dummy);
  }
}

void perform_evil_eye(struct char_data *ch)
{
  struct char_data *shaity = NULL;

  if (IS_IMMORTAL(ch)) {
    return;
  }

  act("The eye of the Dark One passes over $n.",TRUE,ch,0,0,TO_ROOM);
  send_to_char("As you say the Dark One's name, you feel like something "
               "evil watches you. The room lurches, and you fall to "
               "the floor.\r\n",ch);
  GET_POS(ch) = POS_SITTING;
  shaity = get_char_pointer("shai'tan");
  if (NULL != shaity) {
    asend_to_char(shaity,"%s just said your name in room# %d.\r\n",GET_NAME(ch),world[ch->in_room].number);
  }
}

ACMD(do_say)
{
  int i, a, mprog = 0, oprog = 0,evil_eye = 0;
  char buf2[MAX_STRING_LENGTH], buf3[MAX_STRING_LENGTH], buf4[MAX_STRING_LENGTH];
  struct char_data *people;
  
  skip_spaces(&argument);  
  if (IS_AFFECTED(ch,AFF_GAGED)) {
    send_to_char("You can't. You're gagged!\r\n",ch);
    return;
  }
  
  if (!*argument)
    send_to_char("Yes, but WHAT do you want to say?\r\n", ch);
  else {
    oprog = check_for_objprogs_and_stops(ch, NULL, argument, NULL, OPC_SAYROOM, 0);
    oprog += check_for_objprogs_and_stops(ch, NULL, argument, NULL, OPC_SAYWEAR, 0);
    oprog += check_for_objprogs_and_stops(ch, NULL, argument, NULL, OPC_SAYCARRY, 0);

    mprog = check_for_mobprogs_and_stops(ch, NULL, argument, MPC_SAY, 0, 0);
    if ((mprog != 2) && (oprog != 2)) {    

      if (isname("shai'tan",argument)) {
        evil_eye = 1;
      }

      strcpy(buf2, argument);
      if (SPEAKING(ch))   /* If the one talking speaks other than common */
        sprintf(buf3, ", in %s,", languages[SPEAKING(ch)]);
      else
        sprintf(buf3,",");
      for (people = world[ch->in_room].people; people; people = people->next_in_room)
      { /* if (is_ignoring(ch, people)) */
          /* continue; */
        if (people == ch)
          continue;
        if( !check_and_notify_non_IC_receiver(ch, people, "says"))
	{ if (SPEAKING(ch) && !GET_SKILL(people, SKILL_TROLLOC + SPEAKING(ch) - 1)) 
	  { sprintf(buf4, "%s says something completely incomprehensible.\r\n", GET_NAME(ch));
            add_to_rplog(people, buf4,ch);
            act("$n says something completely incomprehensible.", TRUE, ch, 0, people, TO_VICT);
	  } 
	  else
	  { if (SPEAKING(ch))
	      /* Person has a skill in this language ch speaks, lets garble it */
	      for (a = 0; buf2[a] != '\0'; a++)
		if( (argument[a] == ' ') 
		  && (number(0, 90) > GET_SKILL(people, SKILL_TROLLOC + SPEAKING(ch) - 1)))
		  for (a++; (buf2[a] != ' ') && (buf2[a] != '\0'); a++)
		    buf2[a] = 'a' + number(0, 20);
	    i = strlen(buf2) - 1;
	    GET_LAST_EMOTE(ch)=0;
	    if (buf2[i] == '?')
	    { if (PRF2_FLAGGED(people, PRF2_RPTAG))
		sprintf(buf, "[RP] %s asks%s '%s%s\\c00'", 
		    CAN_SEE(people, ch)? GET_NAME(ch): "Someone", buf3, GET_MYCOLOR(ch), buf2);
	      else
		sprintf(buf, "%s asks%s '%s%s\\c00'", 
		    CAN_SEE(people, ch)? GET_NAME(ch): "Someone", buf3, GET_MYCOLOR(ch), buf2);
	    }
	    else if (buf2[i] == '!')
	    { if (PRF2_FLAGGED(people, PRF2_RPTAG))
		sprintf(buf, "[RP] %s exclaims%s '%s%s\\c00'", 
		    CAN_SEE(people, ch)? GET_NAME(ch): "Someone", buf3, GET_MYCOLOR(ch), buf2);
	      else
		sprintf(buf, "%s exclaims%s '%s%s\\c00'", 
		    CAN_SEE(people, ch)? GET_NAME(ch): "Someone", buf3, GET_MYCOLOR(ch), buf2);
	    }
	    else 
	    { if (PRF2_FLAGGED(people, PRF2_RPTAG))
		sprintf(buf, "[RP] %s says%s '%s%s\\c00'", 
		    CAN_SEE(people, ch)? GET_NAME(ch): "Someone", buf3, GET_MYCOLOR(ch), buf2);
	      else
		sprintf(buf, "%s says%s '%s%s\\c00'", 
		    CAN_SEE(people, ch)? GET_NAME(ch): "Someone", buf3, GET_MYCOLOR(ch), buf2);
	    }
	    if (GET_POS(people) != POS_SLEEPING ) 
	    { if (IS_SET(GET_PERMANENT(people), PRM_DEAF)) 
	      { sprintf(buf4, "%s says something.\r\n", GET_NAME(ch));
		add_to_rplog(people, buf4,ch);
		act("$n says something.", TRUE, ch, 0, people, TO_VICT);
	      }
	      else 
	      { act(buf, TRUE, ch, 0, people, TO_VICT|DG_NO_TRIG);
		sprintf(buf, "%s\r\n", buf); 
		add_to_rplog(people, buf,ch);
	      }
	    }
	  }      
	}
      }
      if (evil_eye) {
        perform_evil_eye(ch);
      }
  
      /* The following is very un-tested code for EAVESDROP */

      to_eavesdropper(buf, ch->in_room);

      /* End of EAVESDROP test code */

      if (PRF_FLAGGED(ch, PRF_NOREPEAT))
        send_to_char(OK, ch);
      else {
       if (PRF2_FLAGGED(ch, PRF2_RPTAG))
        sprintf(buf, "[RP] You say%s '%s%s\\c00'\r\n", buf3, GET_MYCOLOR(ch), argument); 
       else
        sprintf(buf, "You say%s '%s%s\\c00'\r\n", buf3, GET_MYCOLOR(ch), argument); 

        send_to_char(buf, ch);
        add_to_rplog(ch, buf, ch);
      }
    }
  }
  if (oprog)
    run_objprog(ch, world[ch->in_room].contents);

  if (mprog) 
    run_mobprog(ch, 0);    

  /* trigger check (DGS)*/
  speech_mtrigger(ch, argument);
  speech_wtrigger(ch, argument);
}

ACMD(do_log) 
{
  char buf2[MAX_STRING_LENGTH];

  skip_spaces(&argument);  

  if (!*argument)
    send_to_char("Yes, but WHAT do you want to log?\r\n", ch);
  else 
  {
    strcpy(buf2, argument);

    sprintf( buf, "%s logged, '%s'", GET_NAME( ch ), buf2 );
    xlog( SYS_GOD, MAX(LVL_GOD, GET_INVIS_LEV(ch)), buf, TRUE );
    sprintf(buf, "You logged, '%s%s\\c00'", GET_MYCOLOR(ch), argument);
    act(buf, FALSE, ch, 0, argument, TO_CHAR);
  }
}

ACMD(do_osay) /* shameless copy from do_say */
{
  int i;
  char buf2[MAX_STRING_LENGTH];
  struct char_data *people;

  skip_spaces(&argument);  

  if (!*argument)
    send_to_char("Yes, but WHAT do you want to say?\r\n", ch);
  else {
    
  strcpy(buf2, argument);

  for (people = world[ch->in_room].people; people; people = people->next_in_room){
 /*  if (is_ignoring(ch, people)) */
/*    continue; */
   if (people == ch)
    continue;
   if (!SEE_DREAMING_OK(people, ch))
    continue;
   i = strlen(buf2) - 1;
   if (buf2[i] == '?')
    sprintf(buf, "%s asks OOCly, '%s%s\\c00'", CAN_SEE(people, ch)? GET_NAME(ch): "Someone", GET_MYCOLOR(ch), buf2);
   else if (buf2[i] == '!')
    sprintf(buf, "%s exclaims OOCly, '%s%s\\c00'", CAN_SEE(people, ch)? GET_NAME(ch): "Someone", GET_MYCOLOR(ch), buf2);
   else 
    sprintf(buf, "%s says OOCly, '%s%s\\c00'", CAN_SEE(people, ch)? GET_NAME(ch): "Someone", GET_MYCOLOR(ch), buf2);
   act(buf, TRUE, ch, 0, people, TO_VICT);
  }

    if (PRF_FLAGGED(ch, PRF_NOREPEAT))
      send_to_char(OK, ch);
    else {
      sprintf(buf, "You say OOCly, '%s%s\\c00'", GET_MYCOLOR(ch), argument);
      act(buf, FALSE, ch, 0, argument, TO_CHAR);
    }
  }
}


ACMD(do_darksay)
{
  int result;
  struct char_data *people;
  
  skip_spaces(&argument);  
  if (IS_AFFECTED(ch,AFF_WRAPPED)) {
    send_to_char("You can't.  You're wrapped!\r\n",ch);
    return;
  }
  if (!IS_IMMORTAL(ch))
    if (!GET_SKILL(ch, SKILL_DTALK) || !is_dark(ch)) {
      send_to_char("Huh?!?\r\n", ch);
      return;
    }
  if (!*argument) {
    send_to_char("Yes, but WHAT do you want to say?\r\n", ch);
    return;
  }

  result = generic_result(ch, SKILL_DTALK, NULL, FALSE);
  if ((result == TOTAL_FAILURE) || (result == TOTAL_FUCKUP)) {
    send_to_char("Darn...You forgot some movement in the signalling. Try again.\r\n", ch);
    return;
  }

  for (people = world[ch->in_room].people; people; people = people->next_in_room) {
    if (people == ch)
      continue;
    if (GET_SKILL(people, SKILL_DTALK))
      asend_to_char(people, "%s signals, '%s%s&n'.\r\n", GET_NAME(ch), GET_MYCOLOR(ch), argument);
  }
  asend_to_char(ch, "You signal, '%s%s&n'\r\n", GET_MYCOLOR(ch), argument);
}


ACMD(do_sayto)
{
  char *message;
  int i, a, mprog = 0,evil_eye = 0;
  char buf2[MAX_STRING_LENGTH], buf3[MAX_STRING_LENGTH];
  struct char_data *people, *vict;
  
  skip_spaces(&argument);

  if (IS_AFFECTED(ch,AFF_GAGED)) {
    send_to_char("You can't.  You're gagged!\r\n",ch);
    return;
  }

  if (!*argument)
    send_to_char("Yes, but WHAT do you want to say, and to whom?\r\n", ch);
  else 
  {
    message = one_argument(argument, arg);
    skip_spaces(&message);
    if (!*arg)
    {
      send_to_char("To whom do you want to say this?\r\n", ch);
      return;
    }
    if (!(vict = get_char_room_vis(ch, arg)))
    {
      send_to_char("That person seems not to be here at the moment.\r\n", ch);
      return;
    }
    if (!*message)
    {
      sprintf(buf, "What do you want to say to %s?\r\n", GET_NAME(vict));
      send_to_char(buf, ch);
      return;
    }
    
    mprog = check_for_mobprogs_and_stops(ch, vict, argument, MPC_SAYTO, 0, 0);
    
    if (mprog != 2) {

      if (isname("shai'tan",message)) {
        evil_eye = 1;
      }

      strcpy(buf2, message);
      if (SPEAKING(ch))   /* If the one talking speaks other than common */
        sprintf(buf3, ", in %s,", languages[SPEAKING(ch)]);
      else
        sprintf(buf3,",");
      
      for (people = world[ch->in_room].people; people; people = people->next_in_room){
        if (people == ch) 
          continue;
        if (SPEAKING(ch) && !GET_SKILL(people, SKILL_TROLLOC + SPEAKING(ch) - 1)){
          sprintf(buf, "%s says something completely incomprehensible to %s", CAN_SEE(people, ch)? 
          GET_NAME(ch): "Someone", (people == vict)? "you": (CAN_SEE(people, vict)? GET_NAME(vict): "someone"));
          if (IS_SET(GET_PERMANENT(people), PRM_DEAF))
            sprintf(buf, "$n says something to %s.", PERS(vict, people));      
          act(buf, TRUE, ch, 0, people, TO_VICT);
        }
        else{
          if (SPEAKING(ch))
/* Person has a skill in this language ch speaks, lets garble it */
            for (a = 0; buf2[a] != '\0'; a++)
              if ((message[a] == ' ') && (number(0, 90) > GET_SKILL(people, SKILL_TROLLOC + SPEAKING(ch) - 1)))
                for (a++; (buf2[a] != ' ') && (buf2[a] != '\0'); a++)
                  buf2[a] = 'a' + number(0, 20);
          i = strlen(buf2) - 1;
          GET_LAST_EMOTE(ch)=0;  
          sprintf(buf, "$n says to %s%s '%s%s&n'",  (vict == people)? "you" : GET_NAME(vict), buf3, GET_MYCOLOR(ch), buf2);
          if (IS_SET(GET_PERMANENT(people), PRM_DEAF))
            sprintf(buf, "$n says something to %s.", PERS(vict, people));                
          act(buf, TRUE, ch, 0, people, TO_VICT);
        }      
      }
      if (evil_eye) {
        perform_evil_eye(ch);
      }
      if (PRF_FLAGGED(ch, PRF_NOREPEAT))
        send_to_char(OK, ch);
      else {
        sprintf(buf, "You say to %s%s '%s%s&n'", GET_NAME(vict), buf3, GET_MYCOLOR(ch), message);
        act(buf, FALSE, ch, 0, message, TO_CHAR);
      }
    }
  }
  if (mprog)
    run_mobprog(ch, 0);
}

ACMD(do_gsay)
{
  struct char_data *k;
  struct follow_type *f;

  skip_spaces(&argument);

  if (!IS_AFFECTED(ch, AFF_GROUP)) {
    send_to_char("But you are not a member of any group!\r\n", ch);
    return;
  }

  if (IS_AFFECTED(ch,AFF_GAGED)) {
    send_to_char("You try to tell your group something\r\n, but you"
                 " can't get a word out since you're GAGGED!\r\n",ch);
    return;
  }

  if (!*argument)
    send_to_char("Yes, but WHAT do you want to group-say?\r\n", ch);
  else {
    if (ch->master)
      k = ch->master;
    else
      k = ch;

    sprintf(buf, "$n tells the group, '%s%s\\c00'", GET_MYCOLOR(ch), argument);

    if (IS_AFFECTED(k, AFF_GROUP) && (k != ch))
      act(buf, FALSE, ch, 0, k, TO_VICT | TO_SLEEP);
    for (f = k->followers; f; f = f->next)
      if (IS_AFFECTED(f->follower, AFF_GROUP) && (f->follower != ch))
	act(buf, FALSE, ch, 0, f->follower, TO_VICT | TO_SLEEP);

    if (PRF_FLAGGED(ch, PRF_NOREPEAT))
      send_to_char(OK, ch);
    else {
      sprintf(buf, "You tell the group, '%s%s\\c00'", GET_MYCOLOR(ch), argument);
      act(buf, FALSE, ch, 0, 0, TO_CHAR | TO_SLEEP);
    }
  }
}


void perform_tell( struct char_data *ch, struct char_data *vict, char *arg )
{ int mprog = 0;
  time_t  timet;
  char	buf[MAX_INPUT_LENGTH];
  char	tbuf[48];

  mprog = check_for_mobprogs_and_stops(ch, vict, arg, MPC_TELL, 0, 0);
  if (mprog != 2) 
  { send_to_char(CCRED(vict, C_NRM), vict);
    sprintf(buf, "$n tells you, '%s\\c00'", arg);
    act(buf, FALSE, ch, 0, vict, TO_VICT | TO_SLEEP);
    send_to_char(CCNRM(vict, C_NRM), vict);

     /* A quick hack for the buffers' system, lesee if it'll work */
    timet = time( NULL );
    strftime( tbuf, 20, "%m/%d - %H:%M", localtime( &timet ) );
    sprintf( buf, "[%s] %s told you, '&r%s&n'\r\n", tbuf, GET_NAME(ch), arg );

    if( !IS_NPC(ch) )
      add_tell_to_buffer( vict, buf );

    if( PRF_FLAGGED(ch, PRF_NOREPEAT) )
      send_to_char(OK, ch);
    else 
    { send_to_char(CCRED(ch, C_CMP), ch);
      sprintf(buf, "You tell $N, '%s\\c00'", arg);
      act(buf, FALSE, ch, 0, vict, TO_CHAR | TO_SLEEP);
      send_to_char(CCNRM(ch, C_CMP), ch);
    }
    GET_LAST_TELL(vict) = GET_IDNUM(ch);
  }
  if (mprog) 
    run_mobprog(ch, vict->in_room);
}

/*
 * Yes, do_tell probably could be combined with whisper and ask, but
 * called frequently, and should IMHO be kept as tight as possible.
 */
ACMD(do_tell)
{
  struct char_data *vict;
  time_t  timet;
  char	tbuf[48];
  char  buf2[MAX_STRING_LENGTH];

  half_chop(argument, buf, buf2);

  if(!IS_NPC(ch) && IS_SET(PLR_FLAGS(ch),PLR_NOTELL)){
    send_to_char("You have been banned from tells by the Imms!\r\n", ch);
    return;
  }
  
  if(!*buf || !*buf2) {
    send_to_char("Whom do you wish to tell what??\r\n", ch);
    return;
  }

  if(!(vict = get_char_vis_including_tar(ch,buf))) {
    send_to_char(NOPERSON, ch);
    return;
  }

  if( ch == vict ) {
    send_to_char("You try to tell yourself something.\r\n", ch);
    return;
  }

  if( (!IS_NPC(ch) || !IS_MOB(ch)) && PRF_FLAGGED(ch, PRF_NOTELL) ) {
    send_to_char("You can't send tells to other people while you have notell on.\r\n", ch);
    return;
  }

  if( ROOM_FLAGGED( ch->in_room, ROOM_SOUNDPROOF ) && (GET_LEVEL( ch ) < LVL_IMMORT)) {
    send_to_char("The walls seem to absorb your words.\r\n", ch );
    return;
  }

  timet = time( NULL );
  strftime( tbuf, 20, "%m/%d - %H:%M", localtime( &timet ) );
  sprintf(buf, "[%s] %s told you, '&r%s&n'\r\n", tbuf, GET_NAME(ch), buf2);
  if( ! IS_NPC( vict ) && !vict->desc )	/* linkless */ {
    act( "$E's linkless at the moment.", FALSE, ch, 0, vict, TO_CHAR | TO_SLEEP );
    send_to_char("The tell will be added to his/her tell buffer.\r\n",ch);

    vict->player_specials->buffer_flag = 1;
    if (!IS_NPC(ch))
      add_tell_to_buffer(vict, buf);   
    return;
  }

  if( PLR_FLAGGED( vict, PLR_WRITING ) ) {
    act( "$E's writing a message right now.",
	 FALSE, ch, 0, vict, TO_CHAR | TO_SLEEP );
    send_to_char("The tell will be added to his/her tell buffer.\r\n",ch);

    vict->player_specials->buffer_flag = 1;
    if (!IS_NPC(ch))
      add_tell_to_buffer(vict, buf);
    return;
  }

  if (PLR_FLAGGED(vict,PLR_MAILING|PLR_MMAILING|PLR_GMAILING)) {
    act("$E's mailing right now.",FALSE,ch,NULL,vict,TO_CHAR|TO_SLEEP);
    send_to_char("The tell will be added to his/her tell buffer.\r\n",ch);

    vict->player_specials->buffer_flag = 1;
    if (!IS_NPC(ch))
      add_tell_to_buffer(vict, buf);
    return;
  }

  if (PRF_FLAGGED(vict, PRF_NOTELL)) 
   if ((IS_IMMORTAL(ch) && IS_IMMORTAL(vict)) || (!IS_IMMORTAL(ch))) {
    act("$E can't hear you.", FALSE, ch, 0, vict, TO_CHAR | TO_SLEEP);
    return;
   }

  if (ROOM_FLAGGED(vict->in_room, ROOM_SOUNDPROOF) && GET_LEVEL(ch) < LVL_IMMORT) { 
  /* Imms can tell through soundproof rooms */
    act("$E can't hear you.", FALSE, ch, 0, vict, TO_CHAR | TO_SLEEP);
    return;
  }

  if( PRF2_FLAGGED( vict,PRF2_AFW ) ) {
    send_to_char( GET_AFW( vict ), ch );
    perform_tell( ch, vict, buf2 );
    return;
  }

  if( PRF_FLAGGED( vict,PRF_AFK ) ) {
    send_to_char( GET_AFK( vict ), ch );
    perform_tell( ch, vict, buf2 );
    vict->player_specials->buffer_flag = 1;
    return;
  }

  if(!IS_NPC(ch) && PLR_FLAGGED( vict, PLR_MAILING ) ) {
    act( "$E is mailing right now.", FALSE, ch, 0, vict, TO_CHAR | TO_SLEEP );
    send_to_char("The tell will be added to his/her tell buffer, though. :)\r\n",ch);
    vict->player_specials->buffer_flag = 1;
    if (!IS_NPC(ch))
      add_tell_to_buffer(vict, buf);
    return;
  }

  if( IS_AFFECTED( ch, AFF_GAGED ) && !IS_IMMORTAL( ch ) ) {
    send_to_char("You can't speak while gagged.\r\n",ch);
    return;
  }

/*  if (is_ignoring(ch, vict)) {
    send_to_char("They're ignoring you.\r\n",ch);
    return;
  } */

  if (FIGHTING(vict)) {
    asend_to_char(ch,"%s is fighting right now.\r\n",GET_NAME(vict));
    send_to_char("The tell will be added to his/her tell buffer, though. :)\r\n",ch);
    vict->player_specials->buffer_flag = 1;
  }

  perform_tell(ch, vict, buf2);
}

ACMD(do_replylock)
{
  struct char_data *tch = character_list;

  skip_spaces(&argument);

  if (!*argument) 
  {
    if (GET_LAST_TELL(ch) == NOBODY) 
    {
      send_to_char("Whom do you want to lock your replies to?\r\n", ch);
      return;
    } 
    else 
    {
      while (tch != NULL && GET_IDNUM(tch) != GET_LAST_TELL(ch))
        tch = tch->next;

      if (tch == NULL)
        send_to_char("He or she is no longer playing.\r\n", ch);
      else
      {
        GET_REPLYTO(ch) = GET_NAME(tch);
        sprintf(buf, "Replies are now locked to %s.\r\n", GET_REPLYTO(ch));
        send_to_char(buf, ch);
        if (!SESS_FLAGGED(ch, SESS_REPLYLOCK))
          (void)SESS_TOG_CHK(ch, SESS_REPLYLOCK);
      }
      return;
    }
  } else {
    if (!strcmp(argument, "off")) {
      if (SESS_FLAGGED(ch, SESS_REPLYLOCK)) {
        (void)SESS_TOG_CHK(ch, SESS_REPLYLOCK);
        send_to_char("Replies are no longer locked.\r\n", ch);
      } else {
        send_to_char("You don't have replies locked!\r\n", ch);
      }
    } else {
      tch = get_player_vis(ch, argument, 0);
      if (!tch) {
        send_to_char("Sorry, that person cannot be found online.\r\n", ch);
      } else {
        GET_REPLYTO(ch) = GET_NAME(tch);
        if (!SESS_FLAGGED(ch, SESS_REPLYLOCK))
          (void)SESS_TOG_CHK(ch, SESS_REPLYLOCK);
        sprintf(buf, "Replies are now locked to %s.\r\n", GET_REPLYTO(ch));
        send_to_char(buf, ch);
      }
    }
  }
}


ACMD(do_reply)
{
  struct char_data *tch = character_list;

  skip_spaces(&argument);

  if (SESS_FLAGGED(ch, SESS_REPLYLOCK)) 
  {
    tch = get_player_vis(ch, GET_REPLYTO(ch), 0);
    if (!tch) 
    {
      send_to_char("They're not here!\r\n", ch);
      (void)SESS_TOG_CHK(ch, SESS_REPLYLOCK);
      return;
    } 
    else
      GET_LAST_TELL(ch) = GET_IDNUM(tch);
  }        

  if (GET_LAST_TELL(ch) == NOBODY)
    send_to_char("You have no one to reply to!\r\n", ch);
  else if (IS_AFFECTED(ch,AFF_GAGED)) 
    send_to_char("You can't while you're gagged.\r\n",ch);
  else if (IS_SET(PLR_FLAGS(ch),PLR_NOTELL))
    send_to_char("You've been banned from tells by the Imms!\r\n",ch);
  else if (!*argument)
    send_to_char("What is your reply?\r\n", ch);
  else 
  {
    /*
     * Make sure the person you're replying to is still playing by searching
     * for them.  Note, now last tell is stored as player IDnum instead of
     * a pointer, which is much better because it's safer, plus will still
     * work if someone logs out and back in again.
     */
				     
    while (tch != NULL && GET_IDNUM(tch) != GET_LAST_TELL(ch))
      tch = tch->next;

    if (tch == NULL)
      send_to_char("He or she is no longer playing.\r\n", ch);
    else 
    {
      if (PRF_FLAGGED(tch, PRF_NOTELL)) 
        if ((IS_IMMORTAL(ch) && IS_IMMORTAL(tch)) || (!IS_IMMORTAL(ch))) 
	{
          act("$E can't hear you.", FALSE, ch, 0, tch, TO_CHAR | TO_SLEEP);
          return;
        }
      if (IS_SET(PLR_FLAGS(tch),PLR_WRITING)) 
      {
	time_t  timet;
	char	tbuf[48];
        send_to_char("He or she is writing a message.\r\n",ch);
        send_to_char("The tell will be added to his/her tell buffer, though. :)\r\n",ch);
	timet = time( NULL );
	strftime( tbuf, 20, "%m/%d - %H:%M", localtime( &timet ) );
	sprintf(buf, "[%s] %s told you, '&r%s&n'\r\n", tbuf, GET_NAME(ch), argument);
        tch->player_specials->buffer_flag = 1;
        if (!IS_NPC(ch))
          add_tell_to_buffer(tch, buf);
        return;
      }
/*      if (is_ignoring(ch,tch)) {
        send_to_char("They're ignoring you!\r\n",ch);
        return;
      } */

      if(PRF_FLAGGED(tch ,PRF_AFK))
        tch->player_specials->buffer_flag = 1;

      perform_tell(ch, tch, argument);

    }
  }
}


ACMD(do_spec_comm)
{
  struct char_data *vict,*list;
  char *action_sing, *action_plur, *action_others;
  char wolfbuf[MAX_RAW_INPUT_LENGTH], buf3[MAX_RAW_INPUT_LENGTH], 
       buf4[MAX_RAW_INPUT_LENGTH];
  int a;

  if (subcmd == SCMD_WHISPER) {
    action_sing = "whisper to";
    action_plur = "whispers to";
    action_others = "$n whispers something to $N.";
  } else {
    action_sing = "ask";
    action_plur = "asks";
    action_others = "$n asks $N a question.";
  }

  half_chop(argument, buf, buf2);

  if( ! *buf || ! *buf2 ) {
    sprintf( buf, "Whom do you want to %s.. and what??\r\n", action_sing );
    send_to_char( buf, ch );
    return;
  } 

  if( IS_AFFECTED( ch,AFF_GAGED ) ) { 
    send_to_char( "You can't, schmuck, you're gagged!\r\n", ch );
    return;
  }

  if( IS_SET( PLR_FLAGS( ch ), PLR_NOASK ) ) {
    send_to_char("You've been forbiddden to use ask by the Imms!\r\n",ch);
    return;
  }

  if( ! ( vict = get_char_room_vis( ch, buf ) ) ){
    send_to_char(NOPERSON, ch);
    return;
  }

  if( vict == ch ){
    send_to_char("You can't get your mouth close enough to your ear...\r\n", ch );
    return;
  }

  {
    sprintf( buf, "$n %s you, '%s'", action_plur, buf2 );
    if (IS_SET(GET_PERMANENT(vict), PRM_DEAF))
       act("$n whispers or says something to you.", TRUE, ch, 0, vict, TO_VICT);            
    else
      act( buf, FALSE, ch, 0, vict, TO_VICT );

    // This is a silly if, both cases go through...
    if( ( subcmd == SCMD_WHISPER)  || (subcmd == SCMD_ASK)) 
    {
      for( list = world[ch->in_room].people; list; list = list->next_in_room) 
      {
	if( ch == list )
	  continue;

        if( (list != ch) && (list != vict) && 
	    ( GET_ISWOLFKIN(list) || get_guild(list, 20) ||  	/* wolfkin */
	      IS_AFFECTED( list, AFF_LIPREADING ) ) ) 
	{
          if( SPEAKING( ch ) )
	  {
            sprintf(buf3, ", in %s, ", languages[SPEAKING(ch)]);
            if (!GET_SKILL(list, SKILL_TROLLOC + SPEAKING(ch) - 1)) 
	    {
              sprintf( wolfbuf, "%s %s %s something incomprehensible.\r\n", 
                PERS(ch, list), action_plur,  PERS(vict, list));
              if (IS_SET(GET_PERMANENT(list), PRM_DEAF))
                sprintf(wolfbuf, "$n %s something to %s.", action_plur, PERS(vict, list));                    
              act(wolfbuf, TRUE, ch, 0, list, TO_VICT);
            }
            else 
	    {
              strcpy(buf4, buf2);
              for (a = 0; buf4[a] != '\0'; a++) 
                if ((buf2[a] == ' ') && (number(0, 90) > GET_SKILL(list, SKILL_TROLLOC + SPEAKING(ch) - 1)))
                  for (a++; (buf4[a] != ' ') && (buf4[a] != '\0'); a++)
                    buf4[a] = 'a' + number(0, 20);
              sprintf(wolfbuf,"%s %s %s%s '%s\\c00'",PERS(ch, list), action_plur,
              PERS(vict, list), buf3, buf4);
              if (IS_SET(GET_PERMANENT(list), PRM_DEAF))
                sprintf(wolfbuf, "$n %s something to %s.", action_plur, PERS(vict, list));                                
              act(wolfbuf, TRUE, ch, 0, list, TO_VICT);
            }
          }
          else
	  {
            sprintf(buf3, ", ");
            sprintf(buf4, "%s %s %s, '%s\\c00'\r\n",PERS(ch, list), action_plur, PERS(vict, list), buf2);
            if (IS_SET(GET_PERMANENT(list), PRM_DEAF))
              sprintf(wolfbuf, "$n %s something to %s.", action_plur, PERS(vict, list));                                            
            act( buf4, TRUE, ch, 0, list, TO_VICT);
          }
        }
      }        
    }
      
    if( PRF_FLAGGED( ch, PRF_NOREPEAT ) )
      send_to_char( OK, ch );
    else 
    {
      sprintf( buf, "You %s %s '%s\\c00'\r\n", action_sing, GET_NAME(vict), buf2);
      act(buf, FALSE, ch, 0, 0, TO_CHAR);
    }


    for (list = world[ch->in_room].people; list != NULL; list = list->next_in_room) 
    {
      if ((ch != list) 
	&& (SENDOK(list) || is_retainer(list))
	&& (PRF_FLAGGED( list, PRF_COMMSPAM ) ) )
        perform_act( action_others,ch,0,vict,list,0);
    }
  }
}



#define MAX_NOTE_LENGTH 5000	/* arbitrary */

ACMD(do_write)
{
  struct obj_data *paper = 0, *pen = 0;
  char *papername, *penname;
  int i;

  papername = buf1;
  penname = buf2;

  two_arguments(argument, papername, penname);

  if (!ch->desc)
    return;

  if (!*papername) {		/* nothing was delivered */
    send_to_char("Write?  With what?  On what?  What are you trying to do?!?\r\n", ch);
    return;
  }
  if (*penname) {		/* there were two arguments */
    if (!(paper = get_obj_in_list_vis(ch, papername, ch->carrying))) {
      sprintf(buf, "You have no %s.\r\n", papername);
      send_to_char(buf, ch);
      return;
    }
    if (!(pen = get_obj_in_list_vis(ch, penname, ch->carrying))) {
      sprintf(buf, "You have no %s.\r\n", penname);
      send_to_char(buf, ch);
      return;
    }
  } else {		/* there was one arg.. let's see what we can find */
    if (!(paper = get_obj_in_list_vis(ch, papername, ch->carrying))) {
      sprintf(buf, "There is no %s in your inventory.\r\n", papername);
      send_to_char(buf, ch);
      return;
    }
    if (GET_OBJ_TYPE(paper) == ITEM_PEN) {	/* oops, a pen.. */
      pen = paper;
      paper = 0;
    } else if (GET_OBJ_TYPE(paper) != ITEM_NOTE) {
      send_to_char("That thing has nothing to do with writing.\r\n", ch);
      return;
    }
    /* One object was found.. now for the other one. */
    if (!GET_EQ(ch, WEAR_HOLD)) {
      sprintf(buf, "You can't write with %s %s alone.\r\n", AN(papername),
	      papername);
      send_to_char(buf, ch);
      return;
    }
    if (!CAN_SEE_OBJ(ch, GET_EQ(ch, WEAR_HOLD))) {
      send_to_char("The stuff in your hand is invisible!  Yeech!!\r\n", ch);
      return;
    }
    if (pen)
      paper = GET_EQ(ch, WEAR_HOLD);
    else
      pen = GET_EQ(ch, WEAR_HOLD);
  }

  i = real_object(paper->item_number);

  /* ok.. now let's see what kind of stuff we've found */
  if (GET_OBJ_TYPE(pen) != ITEM_PEN)
    act("$p is no good for writing with.", FALSE, ch, pen, 0, TO_CHAR);
  else if (GET_OBJ_TYPE(paper) != ITEM_NOTE)
    act("You can't write on $p.", FALSE, ch, paper, 0, TO_CHAR);
  else if ((NOTHING != i) && paper->action_description && obj_proto[i].action_description && !strcmp(paper->action_description,obj_proto[i].action_description))
    send_to_char("There's something written on it already.\r\n", ch);
  else {
    /* we can write - hooray! */
      send_to_char("Write your note.  (/s saves /h for help)\r\n", ch);
send_to_char("Please format your note to less than 75 characters per line.\r\n", ch);
send_to_char("This will make your note easier to read for those with smaller screens.\r\n",ch);
send_to_char("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n",ch);

      /* ok, here we check for a message ALREADY on the paper */
      if (paper->action_description) {
      /* we str_dup the original text to the descriptors->backstr */
      ch->desc->backstr = str_dup(paper->action_description);
      /* send to the player what was on the paper (cause this is already */
      /* loaded into the editor) */
      send_to_char(paper->action_description, ch);
    }
    act("$n begins to jot down a note.", TRUE, ch, 0, 0, TO_ROOM);
    GET_OBJ_VAL(paper, 0) = SPEAKING(ch);
    paper->action_description = NULL;
    ch->desc->str = &paper->action_description;
    ch->desc->max_str = MAX_NOTE_LENGTH;
    ch->desc->current_text_line = count_newlines(ch->desc->str) + 1;
    SET_BIT(PLR_FLAGS(ch), PLR_WRITING);
  }
}



ACMD(do_page)
{
  struct descriptor_data *d;
  struct char_data *vict;

  half_chop(argument, arg, buf2);

  if (IS_NPC(ch))
    send_to_char("Monsters can't page.. go away.\r\n", ch);
  else if (!*arg)
    send_to_char("Whom do you wish to page?\r\n", ch);
  else {
    sprintf(buf, "\007\007*%s* %s\\c00\r\n", GET_NAME(ch), buf2);
    if (!str_cmp(arg, "all")) {
      if (GET_LEVEL(ch) > LVL_GOD) {
	for (d = descriptor_list; d; d = d->next)
	  if (!d->connected && d->character)
	    act(buf, FALSE, ch, 0, d->character, TO_VICT);
      } else
	send_to_char("You will never be godly enough to do that!\r\n", ch);
      return;
    }
    if ((vict = get_char_vis(ch, arg)) != NULL) {
      act(buf, FALSE, ch, 0, vict, TO_VICT);
      if (PRF_FLAGGED(ch, PRF_NOREPEAT))
	send_to_char(OK, ch);
      else
	act(buf, FALSE, ch, 0, vict, TO_CHAR);
      return;
    } else
      send_to_char("There is no such person in the game!\r\n", ch);
  }
}


/**********************************************************************
 * generalized communication func, originally by Fred C. Merkel (Torg) *
  *********************************************************************/

ACMD(do_gen_comm)
{
  struct descriptor_data *i;
  char color_on[24];
  int	  x;

  /* Array of flags which must _not_ be set in order for comm to be heard */
  static int channels[] = {
    0,
    PRF_DEAF,
    PRF2_NOQUEST,
    PRF_NOAUCT,
    PRF_NOOOC,
    PRF2_NOOBSCENE,
    PRF2_NOSONG,
    0
  };

  /*
   * com_msgs: [0] Message if you can't perform the action because of noshout
   *           [1] name of the action
   *           [2] message if you're not on the channel
   *           [3] a color string.
   */
  static char *com_msgs[][4] = {
    {"You cannot holler!!\r\n",
      "holler",
      "",
    KYEL},

    {"You cannot shout!!\r\n",
      "shout",
      "Turn off your noshout flag first!\r\n",
    KYEL},

    {"You cannot use Question!!\r\n",
      "Question",
      "You aren't even on the Question channel!\r\n",
    KYEL},

    {"You cannot auction!!\r\n",
      "auction",
      "You aren't even on the channel!\r\n",
    KMAG},

    {"You cannot OOC!\r\n",
      "OOC",
      "You aren't even on the OOC channel!\r\n",
    KGRN},

    {"You cannot obscene!\r\n",
      "obscene",
      "You aren't even on the obscene channel!\r\n",
    "&M"},

    {"You cannot sing anymore! :(\r\n",
      "sing",
      "You aren't even on the sing channel!\r\n",
    "&G"},
    {"You cannot say privately anymore!\r\n",
      "privately say",
      "You aren't on a private channel!\r\n",
    "&C"}
  };

  if (subcmd == SCMD_PRIVATE && !GET_PRIVATE(ch)) {
    send_to_char("You are not on any private channel!\r\n", ch);
    return;
  }

  if (PLR_FLAGGED(ch, PLR_NOSHOUT)) {
    send_to_char(com_msgs[subcmd][0], ch);
    return;
  }

  if (is_retainer(ch))
  {
    send_to_char("You can't use public channels, you're a retainer!\r\n", ch);
    return;
  }

  // Some preconditions that might stop a comm attempt if (ch) ain't a wiz
  // Purjo 970403
  if( ! IS_IMMORTAL( ch ) )
  {
    if( ROOM_FLAGGED( ch->in_room, ROOM_SOUNDPROOF ) )
    {
      send_to_char("The walls seem to absorb your words.\r\n", ch);
      return;
    }

    /* level_can_shout defined in config.c */
    if( GET_LEVEL( ch ) < level_can_shout)
    {
      sprintf( buf1, "You must be at least level %d before you can %s.\r\n",
	      level_can_shout, com_msgs[ subcmd ][ 1 ] );
      send_to_char( buf1, ch );
      return;
    }

    if( IS_AFFECTED( ch, AFF_GAGED ) )
    {
      send_to_char("Not while you're gagged!\r\n",ch);
      return;
    }

    if( subcmd == SCMD_HOLLER )
    {
      if( GET_MOVE( ch ) < holler_move_cost )
      {
	send_to_char( "You're too exhausted to holler.\r\n", ch );
	return;
      }
      else
      {
	GET_MOVE( ch ) -= holler_move_cost;
      }
    }

  } // End of imm check

  /* make sure the char is on the channel */
  if ((subcmd == SCMD_QUESTION || subcmd == SCMD_OBSCENE || subcmd == SCMD_SONG) && PRF2_FLAGGED(ch,channels[subcmd])) {
    send_to_char(com_msgs[subcmd][2], ch);
    return;
  }
  else if (subcmd == SCMD_PRIVATE);
  else if((subcmd == SCMD_QUESTION || subcmd == SCMD_OBSCENE || subcmd == SCMD_SONG) && !PRF2_FLAGGED(ch,channels[subcmd])) {
    // Clear to go...
  } 
  else if( PRF_FLAGGED( ch, channels[ subcmd ] ) ) {
    send_to_char(com_msgs[subcmd][2], ch);
    return;
  }

  /* skip leading spaces */
  skip_spaces(&argument);

  /* make sure that there is something there to say! */
  if( ! *argument )
  { 
    sprintf( buf1, "Yes, %s, fine, %s we must, but WHAT???\r\n",
	     com_msgs[ subcmd ][ 1 ], com_msgs[ subcmd ][ 1 ] );
  //  send_to_char( buf1, ch );
    for( x=0; x<30; x++ )
    { send_to_char( ChannelBuffer[subcmd][x], ch ); }
    return;
  }

  if( !strcmp( argument, "@" ) )
  { int	line_size, ign;
    sprintf( buf1, "   &cCharacters on the %s channel.\r\n", com_msgs[subcmd][1] );
    line_size = strlen( buf1 ) - 7;
    send_to_char( buf1, ch );
    sprintf( buf1, "   %.*s\r\n\r\n", line_size, "================================================================================" );
    send_to_char( buf1, ch );
    for( i = descriptor_list; i; i = i->next )
    {
      if( !i->connected && i->character )
      { ign = 0;
	if( GET_REAL_LEVEL( ch ) < GET_INVIS_LEV( i->character ) ) 
	  continue;
	if ((subcmd != SCMD_QUESTION && subcmd != SCMD_OBSCENE && subcmd != SCMD_SONG) && PRF_FLAGGED(i->character,channels[subcmd]))
	  ign = 1;
	else if (subcmd == SCMD_QUESTION && PRF2_FLAGGED(i->character,PRF2_NOQUEST))
	  ign = 1;
	else if (subcmd == SCMD_OBSCENE && PRF2_FLAGGED(i->character, PRF2_NOOBSCENE))
	  ign = 1;
	else if (subcmd == SCMD_SONG && PRF2_FLAGGED(i->character, PRF2_NOSONG))
	  ign = 1;
	else if (subcmd == SCMD_SHOUT &&
	  ((world[ch->in_room].zone != world[i->character->in_room].zone) ||
	   GET_POS(i->character) < POS_RESTING))
	  ign = 1;
	else if (subcmd == SCMD_PRIVATE && (GET_PRIVATE(ch) != GET_PRIVATE(i->character)))
	  ign = 1;
/*	if (is_ignoring(ch,i->character)) 
	  ign = 1; */

	if( ign )
	  send_to_char( "&W[&wChan &ROFF&W]  ", ch );
	else
	  send_to_char( "&W[&wChan &GON &W]  ", ch );

 	if( IS_IMMORTAL( i->character ) )	
	  sprintf( buf1, "&W%s&n", i->character->player.name );
	else
	  sprintf( buf1, "&w%s&n", i->character->player.name );
	send_to_char( buf1, ch );

	if( PLR_FLAGGED( i->character, PLR_WRITING) )
	  send_to_char( "(writing)", ch );
	
	if( ROOM_FLAGGED( i->character->in_room, ROOM_SOUNDPROOF ) )
	  send_to_char( "(muffled)", ch );

	send_to_char( "\r\n", ch );

//      if (COLOR_LEV(i->character) >= C_NRM)
//	send_to_char(color_on, i->character);
 //     act(buf, FALSE, ch, 0, i->character, TO_VICT | TO_SLEEP);
  //    if (COLOR_LEV(i->character) >= C_NRM)
//	send_to_char(KNRM, i->character);
    }
  }
    return;
  }

  /* set up the color on code */
  strcpy(color_on, com_msgs[subcmd][3]);

  /* first, set up strings to be given to the communicator */
  if (PRF_FLAGGED(ch, PRF_NOREPEAT))
    send_to_char(OK, ch);
  else {
    if (COLOR_LEV(ch) >= C_CMP)
      sprintf(buf1, "%sYou %s, '%s\\c00'", color_on, com_msgs[subcmd][1],
	      argument);
    else
      sprintf(buf1, "You %s, '%s'", com_msgs[subcmd][1], argument);
    act(buf1, FALSE, ch, 0, 0, TO_CHAR | TO_SLEEP);
  }

  sprintf(buf, "%s %ss, '%s'\r\n", ch->player.name, com_msgs[subcmd][1], argument);
  if (subcmd != SCMD_PRIVATE)
  {
    for( x=1; x<30; x++ )
      memcpy( ChannelBuffer[subcmd][x-1], ChannelBuffer[subcmd][x], MAX_STRING_LENGTH );
    strcpy( ChannelBuffer[subcmd][29], buf );
  }

  /* now send all the strings out */
  for( i = descriptor_list; i; i = i->next )
  {
    if( ! i->connected
	&& i != ch->desc
	&& i->character
	&& ! PLR_FLAGGED( i->character, PLR_WRITING)
	&& ( ! ROOM_FLAGGED( i->character->in_room, ROOM_SOUNDPROOF )
	     || IS_IMMORTAL( ch ) ) )
    {

      if ((subcmd != SCMD_QUESTION && subcmd != SCMD_OBSCENE && subcmd != SCMD_SONG) && PRF_FLAGGED(i->character,channels[subcmd]))
        continue;
      if (subcmd == SCMD_QUESTION && PRF2_FLAGGED(i->character,PRF2_NOQUEST))
        continue;
      if (subcmd == SCMD_OBSCENE && PRF2_FLAGGED(i->character, PRF2_NOOBSCENE))
        continue;
      if (subcmd == SCMD_SONG && PRF2_FLAGGED(i->character, PRF2_NOSONG))
	continue;
      if (subcmd == SCMD_SHOUT &&
	  ((world[ch->in_room].zone != world[i->character->in_room].zone) ||
	   GET_POS(i->character) < POS_RESTING))
	continue;
      if (subcmd == SCMD_PRIVATE && (GET_PRIVATE(ch) != GET_PRIVATE(i->character)))
	continue;

/*      if (is_ignoring(ch,i->character)) 
        break; */

      if (COLOR_LEV(i->character) >= C_NRM)
	send_to_char(color_on, i->character);
      //act(buf, FALSE, ch, 0, i->character, TO_VICT | TO_SLEEP);
      send_to_char( buf, i->character );
      if (COLOR_LEV(i->character) >= C_NRM)
	send_to_char(KNRM, i->character);
    }
  }
}

ACMD(do_qcomm)
{
  struct descriptor_data *i;

  if (!PRF_FLAGGED(ch, PRF_QUEST)) {
    send_to_char("You aren't even part of the quest!\r\n", ch);
    return;
  }
  skip_spaces(&argument);

  if (IS_AFFECTED(ch,AFF_GAGED)) {
    send_to_char("You can't qsay when you're gagged!\r\n",ch);
    return;
  }

  if (!*argument) {
    sprintf(buf, "%s?  Yes, fine, %s we must, but WHAT??\r\n", CMD_NAME,
	    CMD_NAME);
    CAP(buf);
    send_to_char(buf, ch);
  } else {
    if (PRF_FLAGGED(ch, PRF_NOREPEAT))
      send_to_char(OK, ch);
    else {
      if (subcmd == SCMD_QSAY)
	sprintf(buf, "You quest-say, '%s\\c00'", argument);
      else
	strcpy(buf, argument);
      act(buf, FALSE, ch, 0, argument, TO_CHAR | TO_SLEEP);
    }

    if (subcmd == SCMD_QSAY)
      sprintf(buf, "$n quest-says, '%s\\c00'", argument);
    else
      strcpy(buf, argument);

    for (i = descriptor_list; i; i = i->next)
      if (!i->connected && i != ch->desc &&
	  PRF_FLAGGED(i->character, PRF_QUEST))
	act(buf, 0, ch, 0, i->character, TO_VICT | TO_SLEEP);
    if (subcmd == SCMD_QECHO) {
      sprintf(buf, "%s has qechoed.", GET_NAME(ch));
      xlog(SYS_COMM, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), buf, TRUE);
    }
  }
}

ACMD(do_amt)
{
  struct descriptor_data *i;
  long cid = 0, vid = 0;
  byte boss = FALSE;
  char sender[50], recipients[15];

  if (!GET_ASHABONDED(ch)) {
    send_to_char("Sorry, dude...You ain't bonded.\r\n", ch);
    return;
  }
  if (IS_AFFECTED(ch,AFF_GAGED)) {
    send_to_char("You can't while gagged!\r\n",ch);
    return;
  }
  cid = GET_ASHABONDED(ch);
  if (IS_SET(cid, (1 << 30))) {  /* Ch is holder of the bond */
    boss = TRUE;
    sprintf(sender, "Asha'man %s:", GET_NAME(ch));
    sprintf(recipients, "loved ones");
    REMOVE_BIT(cid, (1 << 30));
  }
  else {
    sprintf(sender, "Bonded %s:", GET_NAME(ch));
    sprintf(recipients, "Asha'man");
  }
  if (!*argument) {
    sprintf(buf,"What do you want to say to your %s?\r\n", recipients);
    send_to_char(buf, ch);
    return;
  }
  for (i = descriptor_list; i; i = i->next)
    if (!i->connected) {
      vid = GET_ASHABONDED(i->character);
      REMOVE_BIT(vid, (1 << 30));
      if (vid == cid) {
        sprintf(buf,"%s%s\\c00",sender, argument);
        act(buf, 0, ch, 0, i->character, TO_VICT | TO_SLEEP);
      }
    }
}

ACMD(do_bdt)
{
  struct descriptor_data *i;
  long cid = 0, vid = 0;
  byte boss = FALSE;
  char sender[50], recipients[15];

  if (!GET_BONDED(ch)) {
    send_to_char("Sorry, dude...You ain't bonded.\r\n", ch);
    return;
  }
  if (IS_AFFECTED(ch,AFF_GAGED)) {
    send_to_char("You can't while gagged!\r\n",ch);
    return;
  }
  cid = GET_BONDED(ch);
  if (IS_SET(cid, (1 << 30))) {  /* Ch is holder of the bond */
    boss = TRUE;
    sprintf(sender, "Aes Sedai %s:", GET_NAME(ch));
    sprintf(recipients, "Gaidin");
    REMOVE_BIT(cid, (1 << 30));
  }
  else {
    sprintf(sender, "Gaidin %s:", GET_NAME(ch));
    sprintf(recipients, "Aes Sedai");
  }
  if (!*argument) {
    sprintf(buf,"What do you want to say to your %s?\r\n", recipients);
    send_to_char(buf, ch);
    return;
  }
  for (i = descriptor_list; i; i = i->next)
    if (!i->connected) {
      vid = GET_BONDED(i->character);
      REMOVE_BIT(vid, (1 << 30));
      if (vid == cid) {
        sprintf(buf,"%s%s\\c00",sender, argument);
        act(buf, 0, ch, 0, i->character, TO_VICT | TO_SLEEP);
      }
    }
}

ACMD(do_mdt)
{
  struct descriptor_data *i;
  long cid = 0, vid = 0;
  byte boss = FALSE;
  char sender[50], recipients[15];

  if (!GET_MINION(ch)) {
    send_to_char("Sorry, dude...You ain't bonded.\r\n", ch);
    return;
  }
  if (IS_AFFECTED(ch,AFF_GAGED)) {
    send_to_char("You can't while gagged!\r\n",ch);
    return;
  }
  cid = GET_MINION(ch);
  if (IS_SET(cid, (1 << 30))) {  /* Ch is holder of the bond */
    boss = TRUE;
    sprintf(sender, "%s %s:", (GET_SEX(ch) == SEX_FEMALE)? "Mistress":"Master", GET_NAME(ch));
    sprintf(recipients, "minions");
    REMOVE_BIT(cid, (1 << 30));
  }
  else {
    sprintf(sender, "Minion %s:", GET_NAME(ch));
    sprintf(recipients, "Master");
  }
  if (!*argument) {
    sprintf(buf,"What do you want to say to your %s?\r\n", recipients);
    send_to_char(buf, ch);
    return;
  }
  for (i = descriptor_list; i; i = i->next)
    if (!i->connected) {
      vid = GET_MINION(i->character);
      REMOVE_BIT(vid, (1 << 30));
      if (vid == cid) {
        sprintf(buf,"%s%s\\c00",sender, argument);
        act(buf, 0, ch, 0, i->character, TO_VICT | TO_SLEEP);
      }
    }
}


ACMD(do_language)
{
  int l = 0;

  if (!*argument){
    sprintf(buf, "These are the languages you know:\r\n");
    strcat(buf,"  Common - 100 %\r\n");
    send_to_char(buf, ch);
    if (GET_SKILL(ch, SKILL_TROLLOC)){
      sprintf(buf,"  Trolloc - %d %%\r\n", GET_SKILL(ch, SKILL_TROLLOC));
      send_to_char(buf, ch);
    }
    if (GET_SKILL(ch, SKILL_OGIER)){
      sprintf(buf,"  Ogier - %d %%\r\n", GET_SKILL(ch, SKILL_OGIER));
      send_to_char(buf, ch);
    }
    if (GET_SKILL(ch, SKILL_OLDTONGUE)){
      sprintf(buf,"  Old tongue - %d %%\r\n", GET_SKILL(ch, SKILL_OLDTONGUE));
      send_to_char(buf, ch);
    }
    sprintf(buf, "You are currently speaking %s.\r\n", languages[SPEAKING(ch)]);
    send_to_char(buf, ch);
    return;
  }
  argument ++;
  while ((l < 4) && strncasecmp(argument, languages[l], strlen(argument)))
    l++;
    
  if (strncasecmp(argument, languages[l], strlen(argument))){
    send_to_char("In what language do you want to speak and write?\r\n", ch);
    return;
  }
  if ((l) && (!GET_SKILL(ch, SKILL_TROLLOC - 1 + l))){
    send_to_char("You don't know that language!\r\n", ch);
    return;
  }
  
  sprintf(buf, "%s chosen as the language in which you now speak and write.\r\n", languages[l]);
  send_to_char(buf, ch);
  SPEAKING(ch) = l;
  return;
}

// As fx, byt #N most be part of string (which will be substituted)
// With chars name. 
ACMD(do_pose)	
{
  struct char_data *tch;
  char buf2[MAX_STRING_LENGTH];
  char buf3[MAX_STRING_LENGTH];
  int copy_emote_to = 0, i;

  skip_spaces(&argument);
  if (!*argument) {
    send_to_char("Usage: pose <string>\r\n",ch);
    return;
  }
  if (!isname(GET_NAME(ch), argument)) {
    send_to_char("Your name must be used in the pose.\r\n", ch);
    return;
  }

  GET_LAST_EMOTE(ch) = 0;
  if (!CMD_IS("lastemote")) {
    if (GET_LASTEMOTESTR(ch))
      free(GET_LASTEMOTESTR(ch));
    GET_LASTEMOTESTR(ch) = str_dup(argument);
    GET_LASTEMOTETYPE(ch) = EMOTE_POSE;
  }


  for (tch = world[IN_ROOM(ch)].people; tch; tch = tch->next_in_room) {
    if (IS_NPC(tch))
      continue;

     if (PRF2_FLAGGED(tch, PRF2_RPTAG)) {
       sprintf(buf,  "[RP] %s (%s)", argument, GET_NAME(ch));
       sprintf(buf2, "[RP] %s (%s)\r\n", argument, GET_NAME(ch));
       sprintf(buf3, "[RP] %s\r\n", argument);
     } else {
       sprintf(buf,  "%s (%s)", argument, GET_NAME(ch));
       sprintf(buf2, "%s (%s)\r\n", argument, GET_NAME(ch));
       sprintf(buf3, "%s\r\n", argument);
     }

    if(IS_SET(PRF_FLAGS(ch),PRF_IC) && GET_LEVEL(ch)<LVL_CARPENTER && !IS_SET(PRF_FLAGS(tch),PRF_IC) && GET_LEVEL(tch)<LVL_IMMORT && !PLR_FLAGGED(ch, PLR_MAILING | PLR_WRITING))
      act("$n does something, but $e is In Character and you aren't.",TRUE, ch, 0, tch,TO_VICT);   
    else {
      if (PRF2_FLAGGED(tch, PRF2_NOPOSEID)) {
        act(coltext(buf3, ch), TRUE, ch, 0, tch, TO_VICT);
        add_to_rplog(tch, coltext(buf3, ch),ch);
      }
      else {
        act(coltext(buf, ch), TRUE, ch, 0, tch, TO_VICT);
        add_to_rplog(tch, coltext(buf2, ch), ch);
      }
    }
  }

  to_eavesdropper(coltext(buf2, ch), ch->in_room);

  if (IS_SET(PRF_FLAGS(ch), PRF_IC)) {
    for (i = 0; mirrored_rooms[i].from != -1; i++) {
      if (ch->in_room == real_room(mirrored_rooms[i].from) && (copy_emote_to = real_room(mirrored_rooms[i].to)) >= 0) {
        for (tch = world[copy_emote_to].people; tch; tch = tch->next_in_room) {
	  if (!(tch->desc && STATE(tch->desc) == CON_PLAYING && AWAKE(tch) && !PLR_FLAGGED(tch, PLR_WRITING | PLR_MAILING)))
	    continue;
  	  if (PRF2_FLAGGED(tch, PRF2_NOPOSEID)) {
	    send_to_char(coltext(buf3, ch), tch);
	    add_to_rplog(tch, coltext(buf3, ch),ch);
	  } else {
	    send_to_char(coltext(buf2, ch), tch);
	    add_to_rplog(tch, coltext(buf2, ch),ch);
	  }
        }
      }
    }
  }
}

// 'personal' emote. 
ACMD(do_pemote)
{
  char buf[MAX_INPUT_LENGTH];
  char *to_targ,*to_room;
  struct char_data *vict, *tch;
  int copy_emote_to = 0, i;

  skip_spaces(&argument);
  if (!*argument) {
    send_to_char("Usage: pemote char <string to char>|<string to room>\r\n",ch);
    return;
  }

  if (!CMD_IS("lastemote")) {
    if (GET_LASTEMOTESTR(ch))
      free(GET_LASTEMOTESTR(ch));
    GET_LASTEMOTESTR(ch) = str_dup(argument);
    GET_LASTEMOTETYPE(ch) = EMOTE_PEMOTE;
  }
  to_room = to_targ = one_argument(argument,buf);
  if (!(vict = get_char_room_vis(ch,buf))) {
    send_to_char("That person isn't here!\r\n",ch);
    return;
  }

  if (!*to_targ) {
    send_to_char("Pemote what?\r\n",ch);
    return;
  }
  skip_spaces(&to_targ);

  while (*to_room && '|' != *to_room)
    to_room++;
  
  if (!*to_room) { // Only one string...
    to_room = NULL;
  } else {
    *to_room = '\0';
    to_room++;
  }
  GET_LAST_EMOTE(ch) = 0;

  if (NULL != to_room)
   skip_spaces(&to_room);

  for (tch = world[IN_ROOM(ch)].people; tch; tch = tch->next_in_room) {
    if (IS_NPC(tch))
      continue;
    if (tch == ch) {
     if (PRF2_FLAGGED(ch, PRF2_RPTAG))
       sprintf(buf,"[RP] You: %s %s (to: %s)\r\n",GET_NAME(ch),to_targ,GET_NAME(vict));
     else
       sprintf(buf,"You: %s %s (to: %s)\r\n",GET_NAME(ch),to_targ,GET_NAME(vict));
     send_to_char(coltext(buf, ch),ch);
     add_to_rplog(tch, coltext(buf, ch),ch);
    }
    else if (tch == vict) {
     if (PRF2_FLAGGED(tch, PRF2_RPTAG))
       sprintf(buf,"[RP] %s %s",GET_NAME(ch),to_targ);
     else
       sprintf(buf,"%s %s",GET_NAME(ch),to_targ);
     act(coltext(buf, ch),FALSE,ch,0,tch,TO_VICT);
     sprintf(buf, "%s\r\n", buf);
     add_to_rplog(tch, coltext(buf, ch),ch);
    }
    else {
     if (to_room != NULL){
       if (PRF2_FLAGGED(tch, PRF2_RPTAG))
         sprintf(buf,"[RP] %s %s",GET_NAME(ch), to_room);
       else
         sprintf(buf,"%s %s",GET_NAME(ch), to_room);
       act(coltext(buf, ch),FALSE,ch,0,tch,TO_VICT);
       to_eavesdropper(coltext(buf, ch), ch->in_room);
       sprintf(buf, "%s\r\n", buf);
       add_to_rplog(tch, coltext(buf, ch),ch);
     }
    }
  }
   
  if (IS_SET(PRF_FLAGS(ch), PRF_IC)) {
    for (i = 0; mirrored_rooms[i].from != -1; i++) {
      if (ch->in_room == real_room(mirrored_rooms[i].from) && (copy_emote_to = real_room(mirrored_rooms[i].to)) >= 0) {
	copy_emote_to_room(coltext(buf, ch), copy_emote_to,ch);
      }
    }
  }
}

 /* Channeling emote */
ACMD(do_cemote)
{
  char channss[MAX_INPUT_LENGTH];
  char *p1,*p2=NULL,*p3;
  struct char_data * tch = NULL;
  int copy_emote_to = 0, i;
   
  skip_spaces(&argument);
  
  if (cant_channel(ch)) {
    send_to_char("You can't even channel. No use using this one.\r\n", ch);
    return;
  }
  sprintf(channss, "%s ", GET_NAME(ch));
   
  p3 = argument;
  while (*p3 && ('|' == *p3))
    p3++;
  
  if (!*p3) {
    send_to_char("Cemote what?\r\n", ch);
    return;
  }
   
  GET_LAST_EMOTE(ch) = 0;
  if (!CMD_IS("lastemote")) {
    if (GET_LASTEMOTESTR(ch))
      free(GET_LASTEMOTESTR(ch));
    GET_LASTEMOTESTR(ch) = str_dup(argument);
    GET_LASTEMOTETYPE(ch) = EMOTE_CEMOTE;
  }
   
  if (NULL == (p1 = index(p3,'|'))) {
    if (' ' == *p3)
      channss[0] = '\0';
    else
      strcat(channss,p3);
  }
  else { // more than one string
    *p1 = '\0';
    p1++;
     
    if (' ' == *p3)
      channss[0] = '\0';
    else
      strcat(channss,p3);

    while (*p1 && '|' == *p1) // To remove extra |s if any.
      p1++;
      if (!(NULL == (p2 = index(p1,'|')))) { // More then two emote strings.
	*p2 = '\0';
	p2++;
	 
	while (*p2 && '|' == *p2)
	  p2++;
	
	if (NULL != (p3 = index(p2,'|')))
	  *p3 = '\0';
      }
  }
  if (*channss) {
    act(channss,FALSE,ch,0,0,TO_OPUSERSS);
    act(channss,FALSE,ch,0,0,TO_CHAR);
  }
  if (p1 && *p1 && ' ' != *p1) {
    act(coltext(coltext(p1, ch), ch),FALSE,ch,0,0,TO_OPUSERNSS);
  }
  if (p2 && *p2 && ' ' != *p2) {
    act(coltext(coltext(p2, ch), ch),FALSE,ch,0,0,TO_NOTCHANN);     
  }

  if (IS_SET(PRF_FLAGS(ch), PRF_IC)) {
    for (i = 0; mirrored_rooms[i].from != -1; i++) {
      if (ch->in_room == real_room(mirrored_rooms[i].from) && (copy_emote_to = real_room(mirrored_rooms[i].to)) >= 0) {
        for (tch = world[copy_emote_to].people; tch; tch = tch->next_in_room) {
	  if (!(tch->desc && STATE(tch->desc) == CON_PLAYING && AWAKE(tch) && !PLR_FLAGGED(tch, PLR_WRITING | PLR_MAILING)))
	    continue;
	  if (can_channel_same_sex(ch, tch) && *channss) {
	    asend_to_char(tch, "%s\r\n", coltext(channss, ch));
	    sprintf(buf, "%s\r\n", channss);
	    add_to_rplog(tch, coltext(buf, ch),ch);
	  }
  	  else if (can_channel_not_same_sex(ch, tch) && p1 && *p1 && *p1 != ' ') {
	    asend_to_char(tch, "%s\r\n", coltext(p1, ch));
	    sprintf(buf, "%s\r\n", p1);
            add_to_rplog(tch, coltext(buf, ch),ch);
	  }
	  else if (p2 && *p2 && *p2 != ' ') {
 	    asend_to_char(tch, "%s\r\n", coltext(p2, ch));
	    sprintf(buf, "%s\r\n", p2);
            add_to_rplog(tch, coltext(buf, ch),ch);
	  } 
        }
      }
    }
  }
}


ACMD(do_private_channel)
{
  struct char_data *vict;
  struct descriptor_data *i;

  half_chop(argument, buf, buf2);

  if (subcmd == SCMD_P_HELP) {
  send_to_char("Private Channel (PC) commands\r\n"
		"-----------------------------\r\n"
		"popen   - opens your own private channel.\r\n"
		"padd    - adds a player to your PC.\r\n"
		"premove - remove a player from your PC.\r\n"
		"pclose  - closes your private channel.\r\n"
		"pwho    - lists all members on the current PC.\r\n"
		"poff    - exits you from your current PC.\r\n\r\n"
		"  NOTE: If you don't want to be added to another\r\n"
		"        player's PC open your own with no players.\r\n"
		"\r\nTo talk on the channel use -- private, psay or .\r\n",ch);
  } else if (subcmd == SCMD_P_OPEN) {
    if (GET_PRIVATE(ch))
      send_to_char("You are already on a private channel!\r\n", ch);
    else {
      GET_PRIVATE(ch) = GET_IDNUM(ch);
      send_to_char("You have just opened your own Private Channel.\r\n", ch);
    }
  } else if (subcmd == SCMD_P_OFF) {
    if (!GET_PRIVATE(ch))
      send_to_char("You aren't on any private channel!\r\n", ch);
    else {
      GET_PRIVATE(ch) = 0;
      send_to_char("You have just quit any Private Channels.\r\n", ch);
    }
  } else if (subcmd == SCMD_P_CLOSE) {
    if (!GET_PRIVATE(ch))
      send_to_char("You aren't on any private channel!\r\n", ch);
    else if (GET_PRIVATE(ch) != GET_IDNUM(ch)) {
      send_to_char("You have just CLOSED your Private Channel.\r\n", ch);
      GET_PRIVATE(ch) = 0;
    }
    else {
      for (i = descriptor_list; i; i = i->next)
        if (i->character && i->character != ch && GET_PRIVATE(i->character) == GET_IDNUM(ch)) {
          GET_PRIVATE(i->character) = 0;
          asend_to_char(i->character, "%s has just closed their Private Channel.\r\n",
              (CAN_SEE(i->character, ch) ? GET_NAME(ch) : "someone"));
        }
      GET_PRIVATE(ch) = 0;
      send_to_char("You have just CLOSED your Private Channel.\r\n", ch);
    }
  } else if (subcmd == SCMD_P_WHO) {
    if (GET_PRIVATE(ch) == 0)
      send_to_char("You are not on a private channel.\r\n",ch);
    else {
      /* show all people on the private channel */
      send_to_char("Private Channel Members\r\n", ch);
      send_to_char("-----------------------\r\n", ch);
      for (i = descriptor_list; i; i = i->next) 
        if (i->character && GET_PRIVATE(i->character) == GET_PRIVATE(ch))
          asend_to_char(ch, "%s\r\n", GET_NAME(i->character));
    }
  } else if (subcmd == SCMD_P_CHECK) {
    /* show all people on the ALL private channels */
    send_to_char("Private Channels\r\n", ch);
    send_to_char("----------------\r\n", ch);
    for (i = descriptor_list; i; i = i->next) 
      if (i->character)
        asend_to_char(ch, "[%-5d]  %s\r\n",
            GET_PRIVATE(i->character), GET_NAME(i->character));
  } else if (subcmd == SCMD_P_REMOVE) {
    if (GET_PRIVATE(ch) != GET_IDNUM(ch))
      send_to_char("You do not own this private channel!\r\n", ch);
    else if (!*buf)
      send_to_char("Whom do you wish to add to your private channel?\r\n", ch);
    else if (!(vict = get_char_vis(ch, buf)))
      send_to_char(NOPERSON, ch);
    else if (IS_NPC(vict))
      send_to_char("NPCs cannot be on private channels!\r\n", ch);
    else if (GET_PRIVATE(vict) != GET_IDNUM(ch))
      asend_to_char(ch, "%s is NOT on your Private Channel!\r\n",
          GET_NAME(vict));
    else {
      GET_PRIVATE(vict) = 0;
      asend_to_char(vict, "You have been REMOVED from %s's Private Channel!\r\n",
          GET_NAME(ch));
      asend_to_char(ch, "%s has been REMOVED from your Private Channel!\r\n",
          GET_NAME(vict));
    }
  } else if (subcmd == SCMD_P_ADD) {
    if (GET_PRIVATE(ch) != GET_IDNUM(ch)) 
      send_to_char("You must open your own private channel first.\r\n",ch);
    else if (!*buf)
      send_to_char("Whom do you wish to add to you private channel?\r\n", ch);
    else if (!(vict = get_char_vis(ch, buf)))
      send_to_char(NOPERSON, ch);
    else if (ch == vict)
      GET_PRIVATE(ch) = GET_IDNUM(ch);
    else if (IS_NPC(vict))
      send_to_char("NPCs cannot be added to private channels!\r\n", ch);
/*    else if (is_ignoring(ch,vict))
      send_to_char("They're ignoring you!\r\n",ch); */
    else if (GET_PRIVATE(vict) != 0)
      asend_to_char(ch, "%s is already on another private channel!\r\n",
          GET_NAME(vict));
    else {
     if (!PRF2_FLAGGED(vict, PRF2_NOPADD)) {
      GET_PRIVATE(vict) = GET_IDNUM(ch);
      asend_to_char(vict, "You have been ADDED to %s's Private Channel!\r\n",
          GET_NAME(ch));
      asend_to_char(ch, "%s has been ADDED to your Private Channel!\r\n",
          GET_NAME(vict));
      } else {
        asend_to_char(ch, "%s has Private Channels disabled.\r\n", GET_NAME(vict));
      }
    }
  }
}

ACMD(do_lastemote) {
  if (IS_NPC(ch)) {
    send_to_char("Go away!\r\n", ch);
    return;
  }
  if (!GET_LASTEMOTESTR(ch)) {
    send_to_char("You have no stored emote.\r\n", ch);
    return;
  }
  switch(GET_LASTEMOTETYPE(ch)) {
    case EMOTE_EMOTE:
      do_echo(ch, GET_LASTEMOTESTR(ch), cmd, SCMD_EMOTE);
      break;
    case EMOTE_POSE:
      do_pose(ch, GET_LASTEMOTESTR(ch), cmd, 0);
      break;
    case EMOTE_CEMOTE:
      do_cemote(ch, GET_LASTEMOTESTR(ch), cmd, 0);
      break;
    case EMOTE_PEMOTE:
      do_pemote(ch, GET_LASTEMOTESTR(ch), cmd, 0);
      break;
    default:
      break;
  }
}
/*
int is_ignoring(struct char_data *ch, struct char_data *vict) {

  int i;

   i=0;
   while (i<MAX_IGNORELIST_LENGTH)
   {
    if (!strncasecmp(GET_NAME(ch), vict->player_specials->ignorelist[i], strlen(GET_NAME(ch))))
     return 1;
    i++;
   }
   return 0;
} */


ACMD(do_mycolor)
{
  skip_spaces(&argument);
  if (!*argument) {
    send_to_char("Usage:\r\nMYCOLOR <LETTER>\r\n\r\nEx: MYCOLOR B\r\n", ch);
    sprintf(buf, "\r\nHere is \"%sa sample&n\" of your current MyColor setting.\r\n", GET_MYCOLOR(ch));
    send_to_char(buf, ch);
  } else {
  switch (*argument) {
  case 'W':
    send_to_char("MyColor set to &WBright White&n.\r\n", ch);
    (void)strcpy(GET_MYCOLOR(ch), "&W");
    return;
  case 'w':
    send_to_char("MyColor set to &wWhite&n.\r\n", ch);
    (void)strcpy(GET_MYCOLOR(ch), "&w");
    return;
  case 'Y':
    send_to_char("MyColor set to &YBright Yellow&n.\r\n", ch);
    (void)strcpy(GET_MYCOLOR(ch), "&Y");
    return;
  case 'y':
    send_to_char("MyColor set to &yYellow&n.\r\n", ch);
    (void)strcpy(GET_MYCOLOR(ch), "&y");
    return;
  case 'C':
    send_to_char("MyColor set to &CBright Cyan&n.\r\n", ch);
    (void)strcpy(GET_MYCOLOR(ch), "&C");
    return;
  case 'c':
    send_to_char("MyColor set to &cCyan&n.\r\n", ch);
    (void)strcpy(GET_MYCOLOR(ch), "&c");
    return;
  case 'G':
    send_to_char("MyColor set to &GBright Green&n.\r\n", ch);
    (void)strcpy(GET_MYCOLOR(ch), "&G");
    return;
  case 'g':
    send_to_char("MyColor set to &gGreen&n.\r\n", ch);
    (void)strcpy(GET_MYCOLOR(ch), "&g");
    return;
  case 'M':
    send_to_char("MyColor set to &MBright Magenta&n.\r\n", ch);
    (void)strcpy(GET_MYCOLOR(ch), "&M");
    return;
  case 'm':
    send_to_char("MyColor set to &mMagenta&n.\r\n", ch);
    (void)strcpy(GET_MYCOLOR(ch), "&m");
    return;
  case 'B':
    send_to_char("MyColor set to &BBright Blue&n.\r\n", ch);
    (void)strcpy(GET_MYCOLOR(ch), "&B");
    return;
  case 'b':
    send_to_char("MyColor set to &bBlue&n.\r\n", ch);
    (void)strcpy(GET_MYCOLOR(ch), "&b");
    return;
  case 'R':
    send_to_char("MyColor set to &RBright Red&n.\r\n", ch);
    (void)strcpy(GET_MYCOLOR(ch), "&R");
    return;
  case 'r':
    send_to_char("MyColor set to &rRed&n.\r\n", ch);
    (void)strcpy(GET_MYCOLOR(ch), "&r");
    return;
  case 'K':
    send_to_char("MyColor set to &KBright Black&n.\r\n", ch);
    (void)strcpy(GET_MYCOLOR(ch), "&K");
    return;
  case 'k':
    send_to_char("MyColor set to &kBlack&n.\r\n", ch);
    (void)strcpy(GET_MYCOLOR(ch), "&k");
    return;
  case 'n':
    send_to_char("MyColor set to &nNormal&n.\r\n", ch);
    (void)strcpy(GET_MYCOLOR(ch), "&n");
    return;
  default:
    send_to_char("Invalid choice.\r\n\r\nSee &yHELP COLORSETTING&n for valid single-letter color codes.\r\n", ch);
    return;
  }
  }
}

