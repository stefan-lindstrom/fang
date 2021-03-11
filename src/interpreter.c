/******************************************************************************
 * $Id: interpreter.c,v 1.37 2003/12/05 09:38:15 fang Exp $
 *   
 * File: interpreter.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: interpreter.c,v $
 *   Revision 1.37  2003/12/05 09:38:15  fang
 *   Tze: AFK and AFW flags now go away if you quit out, as requested by Kalenth. And tells buffer doesn't repeat your tell back to you when you send a linkdead or afk person a tesll, as requested by Marra.
 *
 *   Revision 1.36  2003/10/28 04:14:35  fang
 *   Tze: removed a useless and annoying xlog message.
 *
 *   Revision 1.35  2003/10/01 05:02:02  fang
 *   Removed Ogier as a starting race.
 *
 *   Revision 1.34  2003/08/14 03:53:53  fang
 *   changed debug xlogs to god level and to check invis status
 *
 *   Revision 1.33  2003/05/06 08:26:42  fang
 *   Fixed alias bugs
 *
 *   Revision 1.32  2003/03/18 13:21:55  fang
 *   Some mem. bugs reported by Valgrind crushed.
 *
 *   Revision 1.31  2003/03/15 11:42:30  fang
 *   *** empty log message ***
 *
 *   Revision 1.30  2003/02/04 09:28:00  fang
 *   osay no longer removes invis-weave
 *
 *   Revision 1.29  2003/02/02 13:38:36  fang
 *   notifylist now uses strcasecmp() instead of strncasecmp() to determine name match.
 *
 *   Revision 1.28  2002/12/23 15:01:22  fang
 *   That last one didn't work, so here's the old one back in, comments added.
 *
 *   Revision 1.26  2002/12/10 18:30:43  fang
 *   fisch: changes need to make it work on the new host (dg_mobcmd.c)
 *   and mask the "tick" crash problem (comm.c)
 *
 *   Revision 1.25  2002/11/07 04:02:02  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.24  2002/11/01 05:06:51  fang
 *   fisch: removed "show rent" since we don't have rent
 *
 *   Revision 1.23  2002/10/22 03:31:46  fang
 *   Fisch: added code for zcmd command
 *
 *   Revision 1.22  2002/09/26 00:00:33  fischer
 *   Fisch: Added Channel list to public channels and made some changes towards
 *   making visible objects on invis admins, invisible to where.
 *
 *   Revision 1.21  2002/09/23 06:43:24  fischer
 *   Fischer: fixed a few tells that were not timestampped
 *
 *   Revision 1.20  2002/09/23 05:52:42  fischer
 *   fisch: clearing out lua triggers part 1
 *
 *   Revision 1.19  2002/09/22 21:43:49  fischer
 *   Removing mounts and horsetrain from the code
 *
 *   Revision 1.18  2002/09/19 06:12:26  fang
 *   fisch: added "notify" to the datestamp  list
 *
 *   Revision 1.17  2002/09/15 07:17:25  fang
 *   Fisch: Removed or commented out the debug messages used to find the
 *   guild channel bug.
 *
 *   Revision 1.16  2002/09/15 00:50:08  fang
 *   Drath: XLOG update - Categories added to xlog display automatically
 *
 *   Revision 1.15  2002/09/14 00:28:03  fang
 *   fisch: cleaned up some debugging code and added others to track that guilie
 *
 *   Revision 1.14  2002/09/13 14:15:46  fang
 *   Drath: Added Wiznet and ; to allowed commands in a cart
 *
 *   Revision 1.13  2002/09/08 06:41:16  fang
 *   Drath: Fischer added AEDIT.  Welcome to the staff, Fischer.
 *
 *   Revision 1.12  2002/09/06 21:24:24  fang
 *   Drath: Cleaning the MCL - Part 1
 *
 *   Revision 1.11  2002/09/05 20:56:30  fang
 *   Drath: added CRETODO and SHOWCRETODO for MassyPants
 *
 *   Revision 1.10  2002/08/30 21:41:48  fang
 *   Made "retainer" command available to morts. My bad, I missed it last time around.
 *
 *   Revision 1.9  2002/08/30 21:23:09  fang
 *   Put in retainers.
 *
 *   Revision 1.8  2002/08/28 16:24:39  fang
 *   Drath: Can use ADT when sleeping
 *
 *   Revision 1.7  2002/08/20 21:03:50  fang
 *   Drath: Hunter GSKILLS
 *
 *   Revision 1.6  2002/08/12 21:08:20  fang
 *   Drath: PLIST added
 *
 *   Revision 1.5  2002/08/04 09:39:55  fang
 *   Drath: SOCIALIDEA and SHOWSOCIALIDEA seperate from IDEA
 *
 *   Revision 1.4  2002/07/31 03:40:03  fang
 *   Drath: WIRE, gold transfer for users
 *
 *   Revision 1.3  2002/07/28 13:56:04  fang
 *   Removed dmlloc...just to test.
 *
 *   Revision 1.2  2002/07/26 19:39:52  fang
 *   Dath: MYCOLOR mem leak fixed.  Thanks, Fred!
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.43  2002/07/16 15:16:16  fang
 *   Drath: Seafolk Guildskills are done.
 *
 *   Revision 1.42  2002/07/12 04:51:32  fang
 *   *** empty log message ***
 *
 *   Revision 1.41  2002/07/12 03:37:33  fang
 *   Drath: MYCOLOR
 *
 *   Revision 1.40  2002/07/09 22:37:17  fang
 *   Drath: REPLYLOCK
 *
 *   Revision 1.39  2002/07/06 07:05:13  fang
 *   Drath: IMMLOCK for Fred and 2nd to last XLOG commit
 *
 *   Revision 1.38  2002/06/29 18:33:05  fang
 *   Drath: In-MUD syslog searching.  Replaces websyslog search
 *
 *   Revision 1.37  2002/06/23 19:11:18  fang
 *   Drath: Fix to do_echo and first commit of XLOG
 *
 *   Revision 1.36  2002/06/18 10:01:23  fang
 *   One bigger leak found in nanny()
 *
 *   Revision 1.35  2002/06/18 09:08:07  fang
 *   *** empty log message ***
 *
 *   Revision 1.34  2002/06/11 18:44:55  fang
 *   Maz: Removed NOHASSLE, NOWIZ, ROOMFLAGS, etc and moved them into TOGGLE.
 *
 *   Revision 1.33  2002/06/07 18:24:26  fang
 *   Maz: NoSpam room flag.
 *
 *   Revision 1.32  2002/06/03 16:51:40  fang
 *   Maz: Oops.  Gave all IMMs access to PEDITUTIL.  Don't want things deleted by silly non-admins.  Fixed.
 *
 *   Revision 1.31  2002/06/03 16:25:25  fang
 *   Mazrim: Enabled autoREBOOT on an 8 hour cycle to keep the memory usage of the mud down.
 *
 *   Revision 1.30  2002/06/02 22:14:44  fang
 *   Maz: PEDIT
 *
 *   Revision 1.29  2002/05/28 20:22:29  fang
 *   Maz: Small fix for Nose/Ear Stat Removal
 *
 *   Revision 1.28  2002/05/27 10:52:49  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.27  2002/05/24 17:46:43  fang
 *   Maz: Added WIZLIST to TEDIT and removed IMMLIST.
 *
 *   Revision 1.26  2002/05/24 13:07:04  fang
 *   more stone stuff...
 *
 *   Revision 1.25  2002/05/15 10:11:36  fang
 *   *** empty log message ***
 *
 *   Revision 1.24  2002/05/12 23:50:46  fang
 *   Added in IGNORE system, and removed AutoHotboots until the cap can be fixed for good. -Maz
 *
 *   Revision 1.23  2002/05/06 02:20:20  fang
 *   Mazrim: fixed Ajahset to check off guild 53
 *
 *   Revision 1.22  2002/05/01 13:02:18  fang
 *   Added: Cleartells.  Silly command that no one will use.  But it's there now.
 *
 *   Revision 1.21  2002/04/13 16:08:44  fang
 *   *** empty log message ***
 *
 *   Revision 1.20  2002/03/24 04:50:44  fang
 *   *** empty log message ***
 *
 *   Revision 1.19  2002/03/21 10:17:13  fang
 *   *** empty log message ***
 *
 *   Revision 1.18  2002/03/19 09:57:15  fang
 *   *** empty log message ***
 *
 *   Revision 1.17  2002/03/19 09:47:25  fang
 *   *** empty log message ***
 *
 *   Revision 1.16  2002/03/18 09:39:21  fang
 *   Quiting/BEcoming now properly closes private channels...
 *
 *   Revision 1.15  2002/03/14 16:50:40  fang
 *   *** empty log message ***
 *
 *   Revision 1.14  2002/03/14 08:02:53  fang
 *   *** empty log message ***
 *
 *   Revision 1.13  2002/03/13 13:38:12  fang
 *   Ashasense added
 *
 *   Revision 1.12  2002/03/12 15:02:15  fang
 *   Minor fixes to interpreter.c. dig is updated to utilize the new build permission thingies.
 *
 *   Revision 1.11  2002/03/12 08:42:17  fang
 *   *** empty log message ***
 *
 *   Revision 1.10  2002/03/08 14:45:38  fang
 *   *** empty log message ***
 *
 *   Revision 1.9  2002/03/08 10:35:00  fang
 *   *** empty log message ***
 *
 *   Revision 1.8  2002/03/08 09:50:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.7  2002/03/07 08:47:39  fang
 *   *** empty log message ***
 *
 *   Revision 1.6  2002/03/07 08:02:49  fang
 *   *** empty log message ***
 *
 *   Revision 1.5  2002/02/28 07:55:54  fang
 *
 *
 *   :q!
 *
 *   Revision 1.4  2002/02/27 06:03:56  fang
 *   Further modifications to disguise saving code.
 *
 *   Revision 1.3  2002/02/25 13:20:43  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.175  2002/02/22 14:54:43  mud
 *   *** empty log message ***
 *
 *   Revision 1.174  2002/02/19 15:40:39  mud
 *   Fixed a minor problem with the fix for the loadroom bug... hehe
 *
 *   Revision 1.173  2002/02/07 09:50:51  mud
 *   Logging to disguise added per Mas' request :)
 *
 *   Revision 1.172  2002/02/02 00:00:26  mud
 *   *** empty log message ***
 *
 *   Revision 1.171  2002/01/31 15:15:37  mud
 *   *** empty log message ***
 *
 *   Revision 1.170  2002/01/31 15:03:59  mud
 *   Added hlist for Mas.
 *
 *   Revision 1.169  2002/01/30 09:20:44  mud
 *   shortgate added. Also made gate not choke on unknown rooms, if distance between rooms are not morre than 10 rooms
 *
 *   Revision 1.168  2002/01/28 15:29:05  mud
 *   Customizable prompts..
 *
 *   Revision 1.167  2002/01/24 21:33:17  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.166.2.7  2002/01/24 20:49:36  mud
 *   More cleanup. Removed files:  specialday.c string.c string.h
 *   Removed DONT_TRADE_WITH-stuff from shops.
 *
 *   Revision 1.166.2.6  2002/01/24 03:20:30  mud
 *   Removed some further unescessary files (player.[ch], roomprog.[ch]
 *   old2new.c). Cleaned up some other stuff (guild stuff mostly).
 *
 *   Revision 1.166.2.5  2002/01/23 21:49:41  mud
 *   Removed objsave.[c|h] (old objsave stuff).
 *   Removed obj_file_elem and rent structs.
 *   Removed final vestiges of old guild system, except parts used
 *   in guild.[c|h] for backwards compatibility.
 *
 *   Revision 1.166.2.4  2002/01/22 20:50:59  mud
 *   Removed boats and info system. Obsolete, unused code with no value.
 *   Fixed bug in portal_stone SPECIAL. Now checks that a room exists
 *   before attempting to transf chars there. :)
 *
 *   Revision 1.166.2.3  2002/01/21 01:23:40  mud
 *   Code cleanup. Aiel clans/socities gone. Lot of old guild code gone.
 *
 *   Revision 1.166.2.2  2002/01/18 02:24:51  mud
 *   More cleanup...
 *
 *   Revision 1.166.2.1  2002/01/18 00:44:20  mud
 *   Cleanup started for real. Removed ct, ctalk, dt, tt, wt, ot and a 
 *   few other leftovers from old guild system.
 *
 *   Score should no longer show negative amount of objs until you hit some 2*10^9 objs. :)
 *
 *   Revision 1.130  2001/05/22 09:07:05  mud
 *   DG-Scripts for Fang.
 *
 *   Revision 1.123  2001/05/04 05:39:12  mud
 *   Multiple descs now works, without disrupting old desc system.
 *
 *   Revision 1.112  2001/03/30 12:52:08  mud
 *   Fixed the webmaster command group
 *
 *   Revision 1.111  2001/03/28 17:01:24  mud
 *   added CG_WEBMASTER to the reload command
 *
 *   Revision 1.83  2000/11/07 15:32:40  mud
 *   Fixed tedit for Builder CG
 *
 *   Revision 1.82  2000/11/07 12:00:39  mud
 *   Finally. XML Mobiles are done.
 *
 *   Revision 1.74  2000/09/09 10:50:15  mud
 *   fixed do_ajahset to work with new guild system
 *
 *   Revision 1.59  2000/08/16 10:51:13  mud
 *   Hotboot changes...
 *
 *   Revision 1.58  2000/08/14 19:11:56  mud
 *   The new guild system.
 *
 *   Revision 1.46  2000/06/30 19:39:28  mud
 *   Fix for specials interferring with other specials in same room.
 *
 *   Revision 1.45  2000/06/17 20:23:42  mud
 *   New OLC perm system fixes.
 *
 *   Revision 1.33  2000/04/12 14:47:17  mud
 *   Modified invstart slightly..one won't show up -at all- if it's set.
 *   (to people that wouldn't normally see you of course)
 *
 *   Revision 1.17  2000/03/18 12:23:10  mud
 *   Changed recall so that recall guild will now call the do_guildwalk, commented out guildwalk, and uncommented the guild
 *   command.
 *
 *   Revision 1.2  2000/02/21 14:00:17  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* ************************************************************************
*   File: interpreter.c                                 Part of CircleMUD *
*  Usage: parse user commands, search for specials, call ACMD functions   *
*                                                                         *
*  All rights resered.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#define __INTERPRETER_C__
#include "alias.h"
#include "ban.h"
#include "craft.h"
#include "class.h"
#include "conf.h"
#include "sysdep.h"
#include "config.h"
#include "constants.h"
#include "act.h"
#include "structs.h"
#include "comm.h"
#include "interpreter.h"
#include "db.h"
#include "utils.h"
#include "spells.h"
#include "handler.h"
#include "mail.h"
#include "screen.h"
#include "olc.h"
#include "mobprog.h"
#include "craft.h"
#include "color.h"
#include "xmlRooms.h"
#include "xmlZones.h"
#include "cards.h"
#include "skills.h"
#include "xmlObjects.h"
#include "xmlobjsave.h"
#include "guild_parser.h"
#include "guild.h"
#include "chess.h"
#include "xmlMobiles.h"
#include "guild.h"
#include "xmlcharsave.h"
#include "dg_olc.h"
#include "magic.h"
#include "portstone.h"
#include "retainer.h"

char *fill[] =
{
  "in",
  "from",
  "with",
  "the",
  "on",
  "at",
  "to",
  "\n"
};

char *reserved[] =
{
  "self",
  "me",
  "all",
  "room",
  "someone",
  "something",
  "\n"
};

/*
char *allowed_compulsed_1 =
{ "sob" };

char *allowed_compulsed_2 =
{ "sob bucket moose" };

char *allowed_compulsed_3 =
{ "worthy" };

char *allowed_compulsion_1 =
{
  "pat"
};

char *allowed_compulsion_2 =
{
  "pat follow group"
};
/ * About everything allowed at lvl3 compulsion! */

char *allowed_longsleep =
{
  "gdt bdt dt wt ct adt mdt ooc ot tt tell / ctell affected score"
};

char *allowed_while_hidden =
{
  "affected consider equipment exits help hide ic look ooc score who \n "
  "inventory reply tell / save stat sneak gdt bdt gold group scan gt worth "
  "adt tt bdt practice grasp release osay amt mdt ,"
};

char *allowed_while_wrapped =
{
  " ' admire agree accuse bdt blink blush cast chuckle consider "
  "cough cry ctell curse drool equipment exits eyebrow faint "
  "frown dt fume gdt giggle glare gold grasp grin groan growl gsay "
  "gtell help hmm holler inventory ic look laugh "
  "moan mumble ponder pout release reply say sayto > save scream at shiver sigh "
  "sing smile smirk snicker snarl sneeze sniff spit stare stat sulk "
  "tell / tgrasp thank think trelease weave who whisper whine wt yaw grasp ooc release say score "
  "set unaffect unravel unknot tie tieweave affected osay breakshield , "
};

char *not_allowed_entangled =
{
  "flee east west south north up down se southeast "
  "sw southwest ne northeast nw northwest leave enter jump "
  "bandmarch hunt follow run travel taxi"
};

char *not_allowed_gaged =
{
  " ` say qsay qtell gtell gsay gdt ct dt echo gecho emote : tell / whisper"
};

char *not_allowed_invis =
{
  "hit kill west east south north down up flee target weave bash kick dirtkick stun trip "
  "shoot murder unravel"
};

char *sorta_allowed_invis =
{
  "say emote hop kiss poke brand tackle french : ' rest sleep sit "
  "follow shout holler smile smirk cackle ponder sneeze sniff spit "
  "laugh snicker giggle nuzzle tickle noogie thank clap sing weave "
  "cast moan mutter rofl snarl growl shake nod sagenod sayto pout blink chuckle "
  "admire pinch stroke blush cough cry curse drool eyebrow faint frown"
};

char *not_allowed_arrested =
{
  "east west south north up down flee recall rest sleep sit enter leave follow "
  "home drag push run taxi jump kill bash backstab kick trip steal sneak "
  "assist hit stab rescue punch dirtkick shoot stun bandmarch weave cast tieweave"
};

char *allowed_in_hotboot =
{
  "ack amt at adt admire admonish affected agree accuse answer approval applaud "
  "apologize appraise ask auction attention bounce background balance ballad bdisplay "
  "bdt beckon beam bearhug beer beg bite bleed blink blush bmlist boggle bondsense "
  "bonk bop borg bow box brb brood bshow bucket burp bug bugcheck cackle caress "
  "cemote check cheer chuckle clap clear cls consider comfort comb commands compare "
  "congrat cough crack credits cring cry ct cuddle curl curse curtsy dance date "
  "daydream despair diagnose doing doubt doze drool drunk earlick echo eek eediot "
  "emote : embrace equipment exits examine eyebrow eyelash eyes force faint faith fart "
  "flex finger flame flip flirt flop flourish fondle forgive french frown fume "
  "fx gadmininfo gdt gasp gecho ghelp gibber giggle glance glare glist goose gold greet"
  "grin grimace groan grope grovel growl gsay grin grumble grunt gtell guildinfo guildieinfo "
  "help halo handtalk handraise handbook herbident herring hiccup highfive hint hmm hmft holdhands "
  "hop hotta hpull hug huggle hum hunger inventory idea imotd immlist info inno isic isquest "
  "multiplay keg kiss kneel look locate laces lag lap laugh last lastemote liege list "
  "lick love lurk martini moan mdt meep men mgrin moose moon mope motd mourn massage mumble "
  "mutter myguilds nestle news newbie ni nibble nirg nod noogie notes notepad nudge "
  "nuzzle ooc obscene order ogle oops olc osay pace pant pat page peck pemote pet peer "
  "pie pinch play playerlist point poke policy ponder pose pounce pout pray private psay "
  ". phelp pwho pcheck puke purr prove qecho question qsay reply ramble ranks read "
  "report oreport rock rofl roll rub ruffle sa'sara say ' sayto > sagenod salute score scorn "
  "scratch scan scream seeking send sfx shout shake sheathe sheep shiver show showbugs "
  "showideas showtodo showtypos shrug shudder sigh sing sip skills slap smack smile smirk smooch "
  "snicker snap snarl sneeze sniff sniffle snore snort snowball snuggle sob socials "
  "spank spells spin spit squeeze stagger stare stat steam strangle squeal stretch stroke "
  "strut sulk sundance superior swoon sympathy tell / tackle tango tap taunt tender "
  "thank think thump thwap tinfo tip tickle time timetable tongue tsk ttest twiddle "
  "twitch type typo uga unsheathe uptime users value vent version vnum vstat walls warcry "
  "wave weather who wheeze whois whoami where whisper whistle wiggle wink wince wiznet "
  "; wizhelp wizlist wlist women woolhead worship worth worthy yawn yodel ; zecho"
};

/*
 * This is the actual command interpreter called from game_loop() in comm.c
 * It makes sure you are the proper level and position to execute the command,
 * then calls the appropriate function.
 */
void command_interpreter(struct char_data *ch, char *argument)
{
  int cmd, wanted_cmd = 0, length, repeat, counter, n, /* is_guild_member = 0,*/  mprog = 0,
    mprog2 = 0, oprog = 0, oprog2 = 0;
  int cmdlistend = 1;
  char *line, figure[5];
  struct char_guild_element *element;
  char *ch_mss[] = { "neutral","menacing","golden"};
  
  /*check to make sure player who is currently laying is actually in a room.  If not, put in recall room*/
  if ((GET_CON(ch) == 0) && (ch->in_room == NOWHERE)){
    char_from_room(ch);    
    if (ch->was_in_room == NOWHERE) {
      char_to_room(ch, recall_room);
    } else {
      char_to_room(ch, ch->was_in_room);
    }
  }

  if (ROOM_FLAGGED(ch->in_room,ROOM_NOMAGIC) && IS_SET(PRF_FLAGS(ch),PRF_GRASPING)) {
    send_to_char("Suddenly you lose your grip of the One Power!\r\n",ch);
    sprintf(buf,"The %s aura around $n suddenly vanishes!",ch_mss[(int) GET_SEX(ch)]);
    act(buf,FALSE,ch,NULL,NULL,TO_OPUSERSS);
    remove_char_maintained(ch);
    remove_room_maintained(ch);
    REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
  }
  
  if (ROOM_FLAGGED(ch->in_room, ROOM_GUILD) && (!IS_NPC(ch)) && (GET_LEVEL(ch) < LVL_IMMORT)){
    if (!get_guild_by_room(ch, IN_ROOM(ch))) {
      send_to_char("You're not supposed to be here, little one.\r\n", ch);
      send_to_char("Come here and daddy will take you home.\r\n", ch);
      char_from_room(ch);
      char_to_room(ch, real_room(recall_room));
      return;
    }
  }

  if (GET_TRAVEL_TO(ch))
    send_to_char("You stop travelling.\r\n", ch);
  GET_TRAVEL_TO(ch) = 0;
  /* just drop to next line for hitting CR */
  skip_spaces(&argument);
  if (!*argument)
    return;

  if (IS_AFFECTED2(ch, AFF2_HALT)) /* Player actions blocked */
    return;

  /* Code patch for iterated commands, so that people don't have */
  /* to enter the same command n times. Instead they just type   */
  /* "n [command] ". The code will only accept 0 < n < 30.       */

  /* We won't allow iterated commands of any sort in !SPAM rooms */
  /* So we'll update this for the room flag                      */

  counter = 0;
  if (isdigit(*argument)) {
    while (isdigit(argument[counter]) && (counter < 5)) {
      figure[counter] = argument[counter];
      counter ++;
    }
    figure[counter] = '\0';
    argument = argument + counter;
    skip_spaces(&argument);
    if (!(repeat = atoi(figure))) /* If something really weird happened... */ {
      send_to_char("Que?\r\n", ch);
      return;
    }
    if ((repeat < 0) || (repeat > 30)) {
      send_to_char("Too large number entered. Changed to 30.\r\n", ch);
      repeat = 30;
    }
  }
  else
    repeat = 1;
  if (ROOM_FLAGGED(ch->in_room, ROOM_NOSPAM) && (!IS_NPC(ch)) && (GET_LEVEL(ch) < LVL_GRGOD))
    repeat = 1;
  /*
   *  Why not first find command, check if command is allowed for ch, arg parsing and whatnot and
   * when that is done, do the repat stuff...?
   *
   for (n = 0; n < repeat; n++) 
  */
  /*
   * special case to handle one-character, non-alphanumeric commands;
   * requested by many people so "'hi" or ";godnet test" is possible.
   * Patch sent by Eric Green and Stefan Wasilewski.
   */
  if (!isalpha(*argument)) {
    arg[0] = argument[0];
    arg[1] = '\0';
    line = argument + 1;
  } else
    line = any_one_arg(argument, arg);
  
  /* otherwise, find the command */
  if (GET_LEVEL(ch)<LVL_IMMORT) {
    int cont; /* continue the command checks */
    cont = command_wtrigger(ch, arg, line);
    if (!cont) cont += command_mtrigger(ch, arg, line);
    if (!cont) cont = command_otrigger(ch, arg, line);
    if (cont) return; /* command trigger took over */
  }
  
  if (strlen(arg) > 1)  // So 1 char directions will have higher priority
  { element = GET_CHAR_GUILDS(ch);
    while (element) 
    { if (!strncmp(element->guild->gchan_name, arg, strlen(arg))) 
      { if (GLD_FLAGGED(element->guild, GLD_NOGCHAN)) 
	{ send_to_char("Huh ?!\r\n", ch);
	  return;
	}
	do_guild_channel(ch, element, line);
	return;
      }
      if (GLD_FLAGGED(element->guild, GLD_GOSSIP)) 
	if (!strncmp(element->guild->gossip_name, arg, strlen(arg))) 
	{ do_guild_gossip(ch, element, line);
	  return;
	}
	element = element->next;
    }
  }
  
  cmdlistend = 1;
  for (length = strlen(arg), cmd = 0; *complete_cmd_info[cmd].command != '\n'; cmd++) {
    if (!strncmp(complete_cmd_info[cmd].command, arg, length)) {
      cmdlistend = 0;
      wanted_cmd = cmd;

      /*
       * This must be first if 210's are supposed to be able to use -all- cmds!
       */
      if (GET_LEVEL(ch) >= LVL_GRGOD)
        break;

      if (complete_cmd_info[cmd].command_groups && !IS_SET(complete_cmd_info[cmd].command_groups, ((ch)->player_specials->saved.command_groups)))
    continue;

      /*
       * Non-switched mob, level above prescribed for cmd. Succesful. 
       */
      if ((IS_NPC(ch) && (!(ch->desc)) && GET_LEVEL(ch) >= complete_cmd_info[cmd].minimum_level))
    break;

      /*
       * player, level above prescribed for cmd. Succesful.
       */ 
      if (!(IS_NPC(ch)) && GET_LEVEL(ch) >= complete_cmd_info[cmd].minimum_level)
    break;

      /*
       * GRGOD or higgher can use all cmds...or so Mas says :)
       */
      if (GET_LEVEL(ch) >= LVL_GRGOD)
    break;

      /*
       * Switched mob, cmd level less than mort lvl, successful. 
       */
      if (IS_NPC(ch) && (ch->desc) && complete_cmd_info[cmd].minimum_level < 201)
    break;
    }
  }

  if (cmdlistend)
    wanted_cmd = cmd;

  if (hotboot && !isname(complete_cmd_info[cmd].command, allowed_in_hotboot) && 
      (complete_cmd_info[cmd].command_pointer != (void *)do_action)) {
    send_to_char("Sorry, that command isn't allowed while we're hotbooting!\r\n", ch);
    return;
  }

  if (IS_AFFECTED2(ch,AFF2_LONGSLEEP) && !isname(complete_cmd_info[cmd].command,allowed_longsleep)) {
    if (!IS_IMMORTAL(ch)) {
      send_to_char("You can't do that when long-sleeping.\r\nAllowed cmds are: ",ch);
      send_to_char(allowed_longsleep,ch);
      send_to_char("\r\n",ch);
      return;
    }
  }

  if (IS_AFFECTED(ch,AFF_HIDE) && (!isname(complete_cmd_info[cmd].command,allowed_while_hidden)))
    REMOVE_BIT(AFF_FLAGS(ch),AFF_HIDE);
  
  if (IS_AFFECTED(ch,AFF_GAGED) && (isname(complete_cmd_info[cmd].command,not_allowed_gaged))) {
    send_to_char("You can't do that, you're gagged!\r\n",ch);
    return;
  }

  if ( (IS_AFFECTED2(ch,AFF2_ARRESTED) || IS_AFFECTED2(ch,AFF2_SUBDUED) ) && (isname(complete_cmd_info[cmd].command,not_allowed_arrested)) ) {
    send_to_char("You can't do that, you've been arrested!\r\n",ch);
    return;
  }

  if (IS_AFFECTED(ch, AFF_WRAPPED) && (!isname(complete_cmd_info[cmd].command, allowed_while_wrapped))) {
    send_to_char("You try to do that, but something holds you tight.\r\n", ch);
    return;
  }

  if (affected_by_spell(ch, SPELL_ENTANGLE) && isname(complete_cmd_info[cmd].command, not_allowed_entangled)) {
    send_to_char("You try to do that, but those bloody vines stop you.\r\n", ch);
    return;
  }

  if (IS_AFFECTED2(ch,AFF2_COMPULSED) && (GET_LEVEL(ch) < LVL_IMMORT)) {
    send_to_char("Your master wouldn't allow that!\r\n",ch);
    return;
  }
 
  if (IS_AFFECTED(ch, AFF_INVISIBLE) && (isname(complete_cmd_info[cmd].command, not_allowed_invis))) {
    affect_from_char(ch, SPELL_INVISIBILITY);
    act("The air before you shimmers and suddenly $n appears, seemingly from nowhere!", FALSE, ch, 0, 0, TO_NOTCHAR);
  }

  if (IS_AFFECTED(ch, AFF_INVISIBLE) && (isname(complete_cmd_info[cmd].command, sorta_allowed_invis))) {
    act("The air nearby shimmers and wavers suspiciously.", TRUE, ch, 0, 0, TO_NOTCHAR);
    send_to_char("The weaves that render you invisible shimmer and waver around you!\r\n",ch);
  }

  if (GET_DEST(ch) && !(isname(complete_cmd_info[cmd].command, "' : help score . "
                  " . reply tell / who skills spells inventory look ooc ic gdt tt wdt get eat bug ghelp . "
                  " . wiznet ; ."
                  " . drink equipment group bdt amt mdt gtell gsay osay question . psay private release")) && 
      (complete_cmd_info[cmd].command_pointer != do_action)) {
    send_to_char("You can't do that while riding a cart.\r\n", ch);
    return;
  }

  if (*complete_cmd_info[cmd].command == '\n') {
    /* only to give textual feedback about cmd? */

    if ( (GET_LEVEL(ch) < complete_cmd_info[wanted_cmd].minimum_level) &&
     (/*complete_cmd_info[wanted_cmd].minimum_level < LVL_IMMORT ||*/ GET_LEVEL(ch) >= LVL_IMMORT) )
      send_to_char("You do not have enough experience to do this.\r\n", ch);
    else if (GET_LEVEL(ch) >= LVL_IMMORT && complete_cmd_info[wanted_cmd].command_groups) {
      long cmd_grp = complete_cmd_info[wanted_cmd].command_groups;
      long plr_cmd_grp = (ch)->player_specials->saved.command_groups;
      
      if ( (IS_SET(cmd_grp, CG_NEWBIE) && !IS_SET(plr_cmd_grp, CG_NEWBIE)) ||
       (IS_SET(cmd_grp, CG_GL) && !IS_SET(plr_cmd_grp, CG_NEWBIE)) ||
       (IS_SET(cmd_grp, CG_ST) && !IS_SET(plr_cmd_grp, CG_NEWBIE)) ||
         (IS_SET(cmd_grp, CG_GL_BUILDER) && !IS_SET(plr_cmd_grp, CG_NEWBIE)) ||
       (IS_SET(cmd_grp, CG_BUILDER) && !IS_SET(plr_cmd_grp, CG_NEWBIE)) ||
       (IS_SET(cmd_grp, CG_ADMIN) && !IS_SET(plr_cmd_grp, CG_NEWBIE)) ||
       (IS_SET(cmd_grp, CG_WEBMASTER) && !IS_SET(plr_cmd_grp, CG_WEBMASTER)) )
    send_to_char("You are not holy enough to do this command.\r\n", ch);
      else
    send_to_char("Huh?!?\r\n", ch); /* should never get here I believe */
    }
    else {
      send_to_char("Huh?!?\r\n", ch);
      return;
    }
  }
  else if (PLR_FLAGGED(ch, PLR_FROZEN) && GET_LEVEL(ch) < LVL_IMPL)
    send_to_char("You try, but the mind-numbing cold prevents you...\r\n", ch);
  else if (complete_cmd_info[cmd].command_pointer == NULL)
    send_to_char("Sorry, that command hasn't been implemented yet.\r\n", ch);
  else if ((repeat > 1) && isname(complete_cmd_info[cmd].command, "steal weave cast tieweave who")) {
    send_to_char("Iterated commands are not allowed for this command.\r\n", ch);
    repeat = 1;
  }
  else if (GET_POS(ch) < complete_cmd_info[cmd].minimum_position)
    switch (GET_POS(ch)) {
    case POS_DEAD:    /* Why does the guy stay dead when he ain't supposed to?? */
      if(!IS_AFFECTED2(ch,AFF2_SUBDUED) || IS_NPC(ch)) {
    char_from_room(ch);
    char_to_room(ch, real_room(death_room));
    GET_POS(ch) = POS_SLEEPING;
    GET_HIT(ch) = 1;
    GET_EXP(ch) = MAX(0, GET_EXP(ch) - GET_LEVEL_EXP(ch));
      } else if(IS_AFFECTED2(ch,AFF2_SUBDUED)) {
    GET_HIT(ch) = 1;
    GET_POS(ch) = POS_STANDING;
      }
      break;
    case POS_INCAP:
    case POS_MORTALLYW:
      send_to_char("You are in a pretty bad shape, unable to do anything!\r\n", ch);
      break;
    case POS_STUNNED:
      send_to_char("All you can do right now is think about the stars!\r\n", ch);
      break;
    case POS_SLEEPING:
      send_to_char("In your dreams, or what?\r\n", ch);
      break;
    case POS_RESTING:
      send_to_char("Nah... You feel too relaxed to do that..\r\n", ch);
      break;
    case POS_SITTING:
      send_to_char("Maybe you should get on your feet first?\r\n", ch);
      break;
    case POS_FIGHTING:
      send_to_char("No way!  You're fighting for your life!\r\n", ch);
      break;
    } else 
  {
      /* Everything checked, run repeat times... */

    for (n = 0; n < repeat; n++) 
    {
      if(GET_LEVEL(ch)<LVL_HBUILDER) 
      {
        mprog = check_for_mobprogs_and_stops(ch, NULL, NULL, MPC_ACTION, cmd, 0);
        mprog2 = check_for_mobprogs_and_stops(ch, NULL, line, MPC_ACTIONTO, cmd, 0);
      }
    
      if ((mprog != 2) && (mprog2 != 2) && (oprog != 2) && (oprog2 != 2))
        if (no_specials || !special(ch, cmd, line))
          ((*complete_cmd_info[cmd].command_pointer) (ch, line, cmd, complete_cmd_info[cmd].subcmd));
      if (mprog || mprog2)
        run_mobprog(ch, 0);
    }
  }
}

/**************************************************************************
 * Routines to handle aliasing                                             *
  **************************************************************************/


struct alias *find_alias(struct alias *alias_list, char *str)
{
  while (alias_list != NULL) {
    if (*str == *alias_list->alias)	/* hey, every little bit counts :-) */
      if (!strncmp(str, alias_list->alias, MAX_INPUT_LENGTH-1))
    return alias_list;

    alias_list = alias_list->next;
  }

  return NULL;
}


void free_alias(struct alias *a)
{
  if (a->alias)
    free(a->alias);
  if (a->replacement)
    free(a->replacement);
  free(a);
}


/* The interface to the outside world: do_alias */
ACMD(do_alias)
{
  char *repl;
  struct alias *a, *temp;

  if (IS_NPC(ch))
    return;

  repl = any_one_arg(argument, arg);

  if (!*arg) {			/* no argument specified -- list currently defined aliases */
    send_to_char("Currently defined aliases:\r\n", ch);
    if ((a = GET_ALIASES(ch)) == NULL)
      send_to_char(" None.\r\n", ch);
    else {
      while (a != NULL) {
        sprintf(buf, "%-15s %s\r\n", a->alias, a->replacement);
        send_to_char(buf, ch);
        a = a->next;
      }
    }
  } else {			/* otherwise, add or remove aliases */
    /*
     * check alias length BEFORE removing! 
     */
    if (strlen(arg) > 8000 || strlen(repl) > 8000) {
      send_to_char("Your alais is too long! Can max be 8000 chars!\r\n",ch);
      return;  
    }

    /* is this an alias we've already defined? */
    if ((a = find_alias(GET_ALIASES(ch), arg)) != NULL) {
      REMOVE_FROM_LIST(a, GET_ALIASES(ch), next);
      free_alias(a);
    }
    /* if no replacement string is specified, assume we want to delete */
    if (!*repl) {
      if (a == NULL)
        send_to_char("No such alias.\r\n", ch);
      else
        send_to_char("Alias deleted.\r\n", ch);
    } else {			/* otherwise, either add or redefine an alias */
      if (!str_cmp(arg, "alias")) {
        send_to_char("You can't alias 'alias'.\r\n", ch);
        return;
      }
      CREATE(a, struct alias, 1);
      a->alias = str_dup(arg);
      delete_doubledollar(repl);
      a->replacement = str_dup(repl);
      if (strchr(repl, ALIAS_SEP_CHAR) || strchr(repl, ALIAS_VAR_CHAR))
    a->type = ALIAS_COMPLEX;
      else
    a->type = ALIAS_SIMPLE;
      a->next = GET_ALIASES(ch);
      GET_ALIASES(ch) = a;
      send_to_char("Alias added.\r\n", ch);
    }
  }
}

/*
 * Valid numeric replacements are only $1 .. $9 (makes parsing a little
 * easier, and it's not that much of a limitation anyway.)  Also valid
 * is "$*", which stands for the entire original line after the alias.
 * ";" is used to delimit commands.
 */
#define NUM_TOKENS       9

void perform_complex_alias(struct txt_q *input_q, char *orig, struct alias *a)
{
  struct txt_q temp_queue;
  char *tokens[NUM_TOKENS], *temp, *write_point;
  int num_of_tokens = 0, num;

  /* First, parse the original string */
  temp = strtok(strcpy(buf2, orig), " ");
  while (temp != NULL && num_of_tokens < NUM_TOKENS) {
    tokens[num_of_tokens++] = temp;
    temp = strtok(NULL, " ");
  }

  /* initialize */
  write_point = buf;
  temp_queue.head = temp_queue.tail = NULL;

  /* now parse the alias */
  for (temp = a->replacement; *temp; temp++) {
    if (*temp == ALIAS_SEP_CHAR && *(temp - 1) != '\\') {
      *write_point = '\0';
      buf[MAX_INPUT_LENGTH - 1] = '\0';
      write_to_q(buf, &temp_queue, 1);
      write_point = buf;
    } else if (*temp == ALIAS_VAR_CHAR && *(temp-1) != '\\') {
      temp++;
      if ((num = *temp - '1') < num_of_tokens && num >= 0) {
    strcpy(write_point, tokens[num]);
    write_point += strlen(tokens[num]);
      } else if (*temp == ALIAS_GLOB_CHAR) {
        while (*orig && *orig == ' ')
          orig++;
    strncpy(write_point, orig, MAX_STRING_LENGTH-1);
    write_point += strlen(orig);
      } else if ((*(write_point++) = *temp) == '$')	/* redouble $ for act safety */
    *(write_point++) = '$';
    } else
      *(write_point++) = *temp;
  }

  *write_point = '\0';
  buf[MAX_STRING_LENGTH - 1] = '\0';
  write_to_q(buf, &temp_queue, 1);

  /* push our temp_queue on to the _front_ of the input queue */
  if (input_q->head == NULL)
    *input_q = temp_queue;
  else {
    temp_queue.tail->next = input_q->head;
    input_q->head = temp_queue.head;
  }
}


/*
 * Given a character and a string, perform alias replacement on it.
 *
 * Return values:
 *   0: String was modified in place; call command_interpreter immediately.
 *   1: String was _not_ modified in place; rather, the expanded aliases
 *      have been placed at the front of the character's input queue.
 */
int perform_alias(struct descriptor_data *d, char *orig)
{
  char first_arg[MAX_STRING_LENGTH], *ptr;
  struct alias *a, *tmp;

  /* bail out immediately if the guy doesn't have any aliases */
  if ((tmp = GET_ALIASES(d->character)) == NULL)
    return 0;

  /* find the alias we're supposed to match */
  ptr = any_one_arg(orig, first_arg);

  /* bail out if it's null */
  if (!*first_arg)
    return 0;

  /* if the first arg is not an alias, return without doing anything */
  if ((a = find_alias(tmp, first_arg)) == NULL)
    return 0;

  if (a->type == ALIAS_SIMPLE) {
    strcpy(orig, a->replacement);
    return 0;
  } else {
    perform_complex_alias(&d->input, ptr, a);
    return 1;
  }
}



/***************************************************************************
 * Various other parsing utilities                                         *
 **************************************************************************/

/*
 * searches an array of strings for a target string.  "exact" can be
 * 0 or non-0, depending on whether or not the match must be exact for
 * it to be returned.  Returns -1 if not found; 0..n otherwise.  Array
 * must be terminated with a '\n' so it knows to stop searching.
 */
int search_block(char *arg, const char **list, bool exact)
{
  register int i, l = 0;

  if (exact) {
    for (i = 0; **(list + i) != '\n'; i++)
      if (!strcasecmp(arg, *(list + i)))
    return (i);
  } else {
    if (!l)
      l = 1;			/* Avoid "" to match the first available
                 * string */
    for (i = 0; **(list + i) != '\n'; i++)
      if (!strncasecmp(arg, *(list + i), strlen(arg)))
    return (i);
  }

  return -1;
}

/*
 * searches an array of strings for a target string.  "exact" can be
 * 0 or non-0, depending on whether or not the match must be exact for
 * it to be returned.  Returns -1 if not found; 0..n otherwise.  Array
 * must be terminated with a '\n' so it knows to stop searching.
 */
int search_block_case(char *arg, const char **list, bool exact)
{
  register int i, l = 0;

  if (exact) {
    for (i = 0; **(list + i) != '\n'; i++)
      if (!strcmp(arg, *(list + i)))
    return (i);
  } else {
    if (!l)
      l = 1;			/* Avoid "" to match the first available
                 * string */
    for (i = 0; **(list + i) != '\n'; i++)
      if (!strncmp(arg, *(list + i), strlen(arg)))
    return (i);
  }

  return -1;
}


int is_number(char *str)
{
  while (*str)
    if (!isdigit(*(str++)))
      return 0;

  return 1;
}

/*
 * Suggestion: Rename this thingie "skip_leading_spaces".
 */

void skip_spaces(char **string)
{
  for (; **string && isspace(**string); (*string)++);
}

/*
 * Behold! A skip_spaces function which actually SKIPS
 * all spaces. :P
 */
/*
void skip_spaces(char **string)
{
  int i=0;
  int j,k=0;
  char *tmp;
  while((*string)[i]!='\0') i++;
  tmp = (char *)malloc(i*sizeof(char));
  for (j=0;j<i;j++)
    if(!isspace((*string)[j]))
      tmp[k++]=(*string)[j];
  free(*string);
  *string = tmp;
}
// */

char *delete_doubledollar(char *string)
{
  char *read, *write;

  if ((write = strchr(string, '$')) == NULL)
    return string;

  read = write;

  while (*read)
    if ((*(write++) = *(read++)) == '$')
      if (*read == '$')
    read++;

  *write = '\0';

  return string;
}


int fill_word(char *argument)
{
  return (search_block(argument,(const char **) fill, TRUE) >= 0);
}


int reserved_word(char *argument)
{
  return (search_block(argument, (const char **)reserved, TRUE) >= 0);
}


/*
 * copy the first non-fill-word, space-delimited argument of 'argument'
 * to 'first_arg'; return a pointer to the remainder of the string.
 */
char *one_argument(char *argument, char *first_arg)
{
  char *begin = first_arg;

  do {
    skip_spaces(&argument);

    first_arg = begin;
    while (*argument && !isspace(*argument)) {
      *(first_arg++) = LOWER(*argument);
      argument++;
    }

    *first_arg = '\0';
  } while (fill_word(begin));

  return argument;
}

/*
 * this does the same as one_argument does, but allows for caps.
 * currently only used for setting WK ranks. - jei'reen
 */
char *one_arg_case_sen(char *argument, char *first_arg)
{
  char *begin = first_arg;

  do {
    skip_spaces(&argument);

    first_arg = begin;
    while (*argument && !isspace(*argument)) {
      *(first_arg++) = (*argument);
      argument++;
    }

    *first_arg = '\0';
  } while (fill_word(begin));

  return argument;
}

/*
 * one_word is like one_argument, except that words in quotes ("") are
 * considered one word.
 */
char *one_word(char *argument, char *first_arg)
{
  char *begin = first_arg;

  do {
    skip_spaces(&argument);

    first_arg = begin;

    if (*argument == '\"') {
      argument++;
      while (*argument && *argument != '\"') {
        *(first_arg++) = *argument;
        argument++;
      }
      if (*argument)
        argument++;
    } else {
      while (*argument && !isspace(*argument)) {
        *(first_arg++) = *argument;
        argument++;
      }
    }

    *first_arg = '\0';
  } while (fill_word(begin));

  return argument;
}


/* same as one_argument except that it doesn't ignore fill words */
char *any_one_arg(char *argument, char *first_arg)
{
  skip_spaces(&argument);

  while (*argument && !isspace(*argument)) {
    *(first_arg++) = LOWER(*argument);
    argument++;
  }

  *first_arg = '\0';

  return argument;
}


/*
 * Same as one_argument except that it takes two args and returns the rest;
 * ignores fill words
 */
char *two_arguments(char *argument, char *first_arg, char *second_arg)
{
  return one_argument(one_argument(argument, first_arg), second_arg); /* :-) */
}



/*
 * determine if a given string is an abbreviation of another
 * (now works symmetrically -- JE 7/25/94)
 *
 * that was dumb.  it shouldn't be symmetrical.  JE 5/1/95
 *
 * returnss 1 if arg1 is an abbreviation of arg2
 */
int is_abbrev(const char *arg1,const char *arg2)
{
  if (!*arg1)
    return 0;

  for (; *arg1 && *arg2; arg1++, arg2++)
    if (LOWER(*arg1) != LOWER(*arg2))
      return 0;

  if (!*arg1)
    return 1;
  else
    return 0;
}



/* return first space-delimited token in arg1; remainder of string in arg2 */
void half_chop(char *string, char *arg1, char *arg2)
{
  char *temp;

  temp = any_one_arg(string, arg1);
  skip_spaces(&temp);
  strcpy(arg2, temp);
}



/* Used in specprocs, mostly.  (Exactly) matches "command" to cmd number */
int find_command(char *command)
{
  int cmd;

  for (cmd = 0; *complete_cmd_info[cmd].command != '\n'; cmd++)
    if (!strcmp(complete_cmd_info[cmd].command, command))
      return cmd;

  return -1;
}


int special(struct char_data *ch, int cmd, char *arg)
{
  struct obj_data *i;
  register struct char_data *k;
  int j;
  char tmp_arg[MAX_INPUT_LENGTH];

  /* special in room? */
  if (GET_ROOM_SPEC(ch->in_room) != NULL) {
    strcpy(tmp_arg, arg);
    if (GET_ROOM_SPEC(ch->in_room) (ch, world + ch->in_room, cmd, tmp_arg))
      return 1;
  }

  /* special in equipment list? */
  for (j = 0; j < NUM_WEARS; j++)
    if (GET_EQ(ch, j) && GET_OBJ_SPEC(GET_EQ(ch, j)) != NULL) {
      strcpy(tmp_arg, arg);
      if (GET_OBJ_SPEC(GET_EQ(ch, j)) (ch, GET_EQ(ch, j), cmd, tmp_arg))
    return 1;
    }

  /* special in inventory? */
  for (i = ch->carrying; i; i = i->next_content)
    if (GET_OBJ_SPEC(i) != NULL) {
      strcpy(tmp_arg, arg);
      if (GET_OBJ_SPEC(i) (ch, i, cmd, tmp_arg))
    return 1;
    }

  /* special in mobile present? */
  for (k = world[ch->in_room].people; k; k = k->next_in_room)
    if (GET_MOB_SPEC(k) != NULL) {
      strcpy(tmp_arg, arg);
      if (GET_MOB_SPEC(k) (ch, k, cmd, tmp_arg))
    return 1;
    }

  /* special in object present? */
  if (world[ch->in_room].contents)
    for (i = world[ch->in_room].contents; i; i = i->next_content)
      if (GET_OBJ_SPEC(i) != NULL) {
        strcpy(tmp_arg, arg);
        if (GET_OBJ_SPEC(i) (ch, i, cmd, tmp_arg))
      return 1;
      }

  return 0;
}



/* *************************************************************************
*  Stuff for controlling the non-playing sockets (get name, pwd etc)       *
************************************************************************* */


/* locate entry in p_table with entry->name == name. -1 mrks failed search */
int find_name(char *name)
{
  int i;

  for (i = 0; i <= top_of_p_table; i++) {
    if (!str_cmp((player_table + i)->name, name) || ((player_table + i)->disguised && !str_cmp((player_table + i)->dname, name)))
      return i;
  }

  return -1;
}

int find_name_level(char *name)
{
  int i;

  for (i = 0; i <= top_of_p_table; i++) {
    if (!str_cmp((player_table + i)->name, name) || ((player_table + i)->disguised && !str_cmp((player_table + i)->dname, name)))
      return ((player_table + i)->level);
  }

  return -1;
}


int _parse_name(char *arg, char *name)
{
  int i;

  /* skip whitespaces */
  for (; isspace(*arg); arg++);

  for (i = 0; (*name = *arg); arg++, i++, name++)
    if (!isalpha(*arg)&&(strncmp(arg,"'",1)))
      return 1;

  if (!i)
    return 1;

  return 0;
}


#define RECON		1
#define USURP		2
#define UNSWITCH	3

int perform_dupe_check(struct descriptor_data *d)
{
  struct descriptor_data *k, *next_k;
  struct char_data *target = NULL, *ch, *next_ch;
  int mode = 0;

  int id = GET_IDNUM(d->character);

  /*
   * Now that this descriptor has successfully logged in, disconnect all
   * other descriptors controlling a character with the same ID number.
   */

  for (k = descriptor_list; k; k = next_k) {
    next_k = k->next;

    if (k == d)
      continue;

    if (k->original && (GET_IDNUM(k->original) == id)) {    /* switched char */
      SEND_TO_Q("\r\nMultiple login detected -- disconnecting.\r\n", k);
      STATE(k) = CON_CLOSE;
      if (!target) {
    target = k->original;
    mode = UNSWITCH;
      }
      if (k->character)
    k->character->desc = NULL;
      k->character = NULL;
      k->original = NULL;
    } else if (k->character && (GET_IDNUM(k->character) == id)) {
      if (!target && STATE(k) == CON_PLAYING) {
    SEND_TO_Q("\r\nThis body has been usurped!\r\n", k);
    target = k->character;
    mode = USURP;
      }
      k->character->desc = NULL;
      k->character = NULL;
      k->original = NULL;
      SEND_TO_Q("\r\nMultiple login detected -- disconnecting.\r\n", k);
      STATE(k) = CON_CLOSE;
    }
  }

 /*
  * now, go through the character list, deleting all characters that
  * are not already marked for deletion from the above step (i.e., in the
  * CON_HANGUP state), and have not already been selected as a target for
  * switching into.  In addition, if we haven't already found a target,
  * choose one if one is available (while still deleting the other
  * duplicates, though theoretically none should be able to exist).
  */

  for (ch = character_list; ch; ch = next_ch) {
    next_ch = ch->next;

    if (IS_NPC(ch))
      continue;
    if (GET_IDNUM(ch) != id)
      continue;

    /* ignore chars with descriptors (already handled by above step) */
    if (ch->desc)
      continue;

    /* don't extract the target char we've found one already */
    if (ch == target)
      continue;

    /* we don't already have a target and found a candidate for switching */
    if (!target) {
      target = ch;
      mode = RECON;
      continue;
    }

    /* we've found a duplicate - blow him away, dumping his eq in limbo. */
    if (ch->in_room != NOWHERE)
      char_from_room(ch);
    char_to_room(ch, 1);
    extract_char(ch, FALSE);
  }

  /* no target for swicthing into was found - allow login to continue */
  if (!target)
    return 0;

  /* Okay, we've found a target.  Connect d to target. */
  free_char(d->character); /* get rid of the old char */
  d->character = target;
  d->character->desc = d;
  d->original = NULL;
  d->character->char_specials.timer = 0;
  REMOVE_BIT(PLR_FLAGS(d->character), PLR_MAILING);
  REMOVE_BIT(PLR_FLAGS(d->character), PLR_WRITING);
  STATE(d) = CON_PLAYING;

  switch (mode) {
  case RECON:
    SEND_TO_Q("Reconnecting.\r\n", d);
    act("$n has reconnected.", TRUE, d->character, 0, 0, TO_ROOM);
    sprintf(buf, "%s [%s] has reconnected.", GET_NAME(d->character), d->host);
    xlog(SYS_CONN, MAX(LVL_IMMORT, GET_INVIS_LEV(d->character)),buf, TRUE);
    break;
  case USURP:
    SEND_TO_Q("You take over your own body, already in use!\r\n", d);
    act("$n suddenly keels over in pain, surrounded by a white aura...\r\n"
    "$n's body has been taken over by a new spirit!",
    TRUE, d->character, 0, 0, TO_ROOM);
    sprintf(buf, "%s has re-logged in ... disconnecting old socket.",
        GET_NAME(d->character));
    xlog(SYS_CONN, MAX(LVL_IMMORT, GET_INVIS_LEV(d->character)),buf, TRUE);
    break;
  case UNSWITCH:
    SEND_TO_Q("Reconnecting to unswitched char.", d);
    sprintf(buf, "%s [%s] has reconnected.", GET_NAME(d->character), d->host);
    xlog(SYS_CONN, MAX(LVL_IMMORT, GET_INVIS_LEV(d->character)),buf, TRUE);
    break;
  }

  return 1;
}



/* deal with newcomers and other non-playing sockets */
void nanny(struct descriptor_data *d, char *arg)
{
  char buf[128];
  char tmp[200];
  int player_i = 0, load_result, i;
  char tmp_name[MAX_INPUT_LENGTH];
  long load_room;
  char arg1[200];
  char arg2[200];
  struct obj_data *wagon;
  struct descriptor_data *a, *d2;
  FILE *f;
  struct char_data *chtmp;

  skip_spaces(&arg);

  switch (STATE(d)) {
  case CON_CRAFT:
    parse_craft(d, arg);
    break;

  case CON_TOOL:
    parse_tool(d->character,arg);
    break;

  /*. OLC states .*/
  case CON_PEDIT:
    pedit_parse(d, arg);
    break;
  case CON_HEDIT:
    hedit_parse(d, arg);
    break;
  case CON_OEDIT:
    oedit_parse(d, arg);
    break;
  case CON_REDIT:
    redit_parse(d, arg);
    break;
  case CON_ZEDIT:
    zedit_parse(d, arg);
    break;
  case CON_MEDIT:
    medit_parse(d, arg);
    break;
  case CON_SEDIT:
    sedit_parse(d, arg);
    break;
  case CON_AEDIT:
    aedit_parse(d, arg);
    break;  
  case CON_TRIGEDIT:
    trigedit_parse(d, arg);
    break;
  /*. End of OLC states .*/

  case CON_GET_NAME:		/* wait for input of name */
    if (d->character == NULL) {
      CREATE(d->character, struct char_data, 1);
      clear_char(d->character);
      CREATE(d->character->player_specials, struct player_special_data, 1);
      memset(d->character->player_specials, 0, sizeof(struct player_special_data));
      d->character->desc = d;
      GET_PFILEPOS(d->character) = -1; /* Just in case */
    }
    if (!*arg) {
      free(d->character->player_specials);
      free(d->character);
      d->character = NULL;
      STATE(d) = CON_CLOSE;
      return;
    }
    else {
      if ((_parse_name(arg, tmp_name)) || strlen(tmp_name) < 2 ||
      strlen(tmp_name) > MAX_NAME_LENGTH ||
      fill_word(strcpy(buf, tmp_name)) || reserved_word(buf)) {
    SEND_TO_Q("Invalid name, please try another.\r\n"
          "Name: ", d);
    return;
      }
      if (NULL != (chtmp = xmlload_char(tmp_name))) {
    free(d->character->player_specials);
    free(d->character);
    d->character = chtmp;

    d->character->desc = d;
    d->wait = 1;
    d->character->char_specials.timer = 0;  /* reset all timeout timers... */
    player_i = GET_PFILEPOS(d->character); // player_i not set, but pfilepos is from xmlload_char()

    if (PLR_FLAGGED(d->character, PLR_DELETED)) {
      free_char(d->character);
      CREATE(d->character, struct char_data, 1);
      clear_char(d->character);
      CREATE(d->character->player_specials, struct player_special_data, 1);
      d->character->desc = d;
      CREATE(d->character->player.name, char, strlen(tmp_name) + 1);
      strcpy(d->character->player.name, CAP(tmp_name));
      GET_PFILEPOS(d->character) = player_i;
      sprintf(buf, "Did I get that right, %s (Y/N)? ", tmp_name);
      SEND_TO_Q(buf, d);
      STATE(d) = CON_NAME_CNFRM;
    } else {
      /* undo it just in case they are set */
      REMOVE_BIT(PLR_FLAGS(d->character),
             PLR_WRITING | PLR_MAILING | PLR_CRYO);

      if (PRF_FLAGGED(d->character, PRF_AFK))
        REMOVE_BIT(PRF_FLAGS(d->character), PRF_AFK);

      if (PRF2_FLAGGED(d->character, PRF2_AFW))
        REMOVE_BIT(PRF2_FLAGS(d->character), PRF2_AFW);


//	  if (GET_ABS(d->character) < 0)
//            GET_ABS(d->character) = 0; /* To correct saved errors in this stat */

      SEND_TO_Q("Password: ", d);
      echo_off(d);
      d->idle_tics = 0;
      STATE(d) = CON_PASSWORD;
    }
      } else {
    /* player unknown -- make new character */

    if (!Valid_Name(tmp_name)) {
      SEND_TO_Q("Invalid name, please try another.\r\n", d);
      SEND_TO_Q("Name: ", d);
      return;
    }
    /*
    CREATE(d->character, struct char_data, 1);
    clear_char(d->character);
    CREATE(d->character->player_specials, struct player_special_data, 1);

    already done above directly after case CON_xx: if (NULL == d->...) 
    */ 

    d->character->desc = d;

    CREATE(d->character->player.name, char, strlen(tmp_name) + 1);
    strcpy(d->character->player.name, CAP(tmp_name));
        
        SEND_TO_Q("\r\nPlease choose your name wisely. We have a Policy that requires\n", d);
        SEND_TO_Q("our players to chose names that are appropriate for the Wheel \n", d);
        SEND_TO_Q("of Time series by Robert Jordan. We also ask that you refrain \n", d);
        SEND_TO_Q("from chosing Popular names from Myths or others books/series. \n", d);
        SEND_TO_Q("Book Characters from the Wheel of Times series must be Applied for.\r\n\n", d); 
    sprintf(buf, "Is this name appropriate for the mud and the name you wish to be known by, %s (Y/N)? ", tmp_name);
    SEND_TO_Q(buf, d);
    STATE(d) = CON_NAME_CNFRM;
      }
    }
    break;
  case CON_NAME_CNFRM:		/* wait for conf. of new name    */
    if (UPPER(*arg) == 'Y') {
      if (isbanned(d->host) >= BAN_NEW) {
    sprintf(buf, "Request for new char %s denied from [%s] (siteban)",
        GET_NAME(d->character), d->host);
        xlog(SYS_CONN, LVL_IMMORT, buf, TRUE);
    SEND_TO_Q("Sorry, new characters are not allowed from your site!\r\n", d);
    STATE(d) = CON_CLOSE;
    return;
      }
      if (restrict_run) {
    SEND_TO_Q("Sorry, new players can't be created at the moment.\r\n", d);
    sprintf(buf, "Request for new char %s denied from %s (wizlock)",
        GET_NAME(d->character), d->host);
        xlog(SYS_CONN, LVL_IMMORT, buf, TRUE);
    STATE(d) = CON_CLOSE;
    return;
      }
      SEND_TO_Q("New character.\r\n", d);
      sprintf(buf, "Give me a password for %s: ", GET_NAME(d->character));
      SEND_TO_Q(buf, d);
      echo_off(d);
      STATE(d) = CON_NEWPASSWD;
    } else if (*arg == 'n' || *arg == 'N') {
      SEND_TO_Q("Okay, what IS it, then? ", d);
      free(d->character->player.name);
      d->character->player.name = NULL;
      STATE(d) = CON_GET_NAME;
    } else {
      SEND_TO_Q("Please type Yes or No: ", d);
    }
    break;
  case CON_PASSWORD:		/* get pwd for known player      */
    /*
     * To really prevent duping correctly, the player's record should
     * be reloaded from disk at this point (after the password has been
     * typed).  However I'm afraid that trying to load a character over
     * an already loaded character is going to cause some problem down the
     * road that I can't see at the moment.  So to compensate, I'm going to
     * (1) add a 15 or 20-second time limit for entering a password, and (2)
     * re-add the code to cut off duplicates when a player quits.  JE 6 Feb 96
     */

    echo_on(d);    /* turn echo back on */

    if (!*arg) {
      free_char(d->character);
      d->character = NULL;
      //      close_socket(d);
      STATE(d) = CON_CLOSE;
      return;
    }
    else {
      if(strncmp(CRYPT(arg, GET_PASSWD(d->character)), GET_PASSWD(d->character), MAX_PWD_LENGTH)) {
    sprintf(buf, "Bad PW: %s [%s]", GET_NAME(d->character), d->host);
        xlog(SYS_CONN, MAX(LVL_IMMORT, (GET_INVSTART(d->character) ? GET_INVSTART(d->character) : GET_INVIS_LEV(d->character))), buf, TRUE);
    GET_BAD_PWS(d->character)++;
    save_char(d->character, real_room(GET_LOADROOM(d->character)));
    if (++(d->bad_pws) >= max_bad_pws) {	/* 3 strikes and you're out. */
      SEND_TO_Q("Wrong password... disconnecting.\r\n", d);
      STATE(d) = CON_CLOSE;
    } else {
      SEND_TO_Q("Wrong password.\r\nPassword: ", d);
      echo_off(d);
    }
    return;
      }
      load_result = GET_BAD_PWS(d->character);
      GET_BAD_PWS(d->character) = 0;

      if (isbanned(d->host) == BAN_SELECT &&
      !PLR_FLAGGED(d->character, PLR_SITEOK) && GET_IDNUM(d->character) != 1) {
    SEND_TO_Q("Sorry, this char has not been cleared for login from your site!\r\n", d);
    STATE(d) = CON_CLOSE;
    sprintf(buf, "Connection attempt for %s denied from %s",
        GET_NAME(d->character), d->host);
        xlog(SYS_CONN, MAX(LVL_IMMORT, GET_INVIS_LEV(d->character)), buf, TRUE);
    return;
      }
      if (GET_LEVEL(d->character) < restrict_run) {
    SEND_TO_Q("The game is temporarily restricted.. try again later.\r\n", d);
    STATE(d) = CON_CLOSE;
    sprintf(buf, "Request for login denied for %s [%s] (wizlock)",
        GET_NAME(d->character), d->host);
        xlog(SYS_CONN, MAX(LVL_IMMORT, GET_INVIS_LEV(d->character)), buf, TRUE);
    return;
      }
      if (immlock) {
        if (GET_LEVEL(d->character) > LVL_IMMORT && GET_LEVEL(d->character) < LVL_GRGOD) {
          SEND_TO_Q("&rSorry, Immortals are currently locked out at the moment.&n\r\n", d);
          STATE(d) = CON_CLOSE;
          sprintf(buf, "Request for immortal login of %s denied from %s (immlock)",
                  GET_NAME(d->character), d->host);
          xlog(SYS_CONN, LVL_GRGOD, buf, TRUE);
          return;
        }
      }

       
      /* Since we don't set logon time upon loading of character, do so here... */
      d->character->player.time.logon = time(0);
       
      /* check and make sure no other copies of this player are logged in */
      if (perform_dupe_check(d))
    return;

      if (GET_LEVEL(d->character) >= LVL_IMMORT)
    SEND_TO_Q(imotd, d);
      else
    SEND_TO_Q(motd, d);

      sprintf(buf, "%s [%s] has connected.", GET_NAME(d->character), d->host);
      xlog(SYS_CONN, MAX(LVL_IMMORT, (GET_INVSTART(d->character) ? GET_INVSTART(d->character) : GET_INVIS_LEV(d->character))), buf, TRUE);

      if (load_result) {
    sprintf(buf, "\r\n\r\n\007\007\007"
        "%s%d LOGIN FAILURE%s SINCE LAST SUCCESSFUL LOGIN.%s\r\n",
        CCRED(d->character, C_SPR), load_result,
        (load_result > 1) ? "S" : "", CCNRM(d->character, C_SPR));
    SEND_TO_Q(buf, d);
    GET_BAD_PWS(d->character) = 0;
      }
      SEND_TO_Q("\r\n\n*** PRESS RETURN: ", d);
      if (!GET_MYCOLOR(d->character))
        (void)strcpy(GET_MYCOLOR(d->character), "&n");
      STATE(d) = CON_RMOTD;
    }
    break;

  case CON_NEWPASSWD:
    if (!*arg || strlen(arg) > MAX_PWD_LENGTH || strlen(arg) < 3 ||
    !str_cmp(arg, GET_NAME(d->character))) {
      SEND_TO_Q("\r\nIllegal password.\r\n", d);
      SEND_TO_Q("Password: ", d);
      return;
    }
    strncpy(GET_PASSWD(d->character), CRYPT(arg, GET_NAME(d->character)), MAX_PWD_LENGTH);
    *(GET_PASSWD(d->character) + MAX_PWD_LENGTH) = '\0';

    SEND_TO_Q("\r\nPlease retype password: ", d);
    STATE(d) = CON_CNFPASSWD;

    break;

  case CON_CNFPASSWD:
    if (strncmp(CRYPT(arg, GET_PASSWD(d->character)), GET_PASSWD(d->character),	MAX_PWD_LENGTH)) {
      SEND_TO_Q("\r\nPasswords don't match... start over.\r\n", d);
      SEND_TO_Q("Password: ", d);
      STATE(d) = CON_NEWPASSWD;

      return;
    }
    echo_on(d);
    if (STATE(d) == CON_CNFPASSWD) {
      if (!GET_LEVEL(d->character))
      {
        GET_CLASS(d->character) = CLASS_ADVENTURER;
        if (GET_PFILEPOS(d->character) < 0)
          GET_PFILEPOS(d->character) = create_entry(GET_NAME(d->character));
        init_char(d->character);
        player_table[GET_PFILEPOS(d->character)].id = GET_IDNUM(d->character);
        save_char(d->character, NOWHERE);
        d->character->real_abils.str = 11;
        d->character->real_abils.con = 11;
        d->character->real_abils.dex = 11;
        d->character->real_abils.intel = 11;
        d->character->real_abils.wis = 11;
        d->character->real_abils.cha = 11;
        SET_MANA(d->character, 50);
	SET_MAX_MANA(d->character, 50);
        GET_MOVE(d->character) = GET_MAX_MOVE(d->character) = 50;
        GET_FRACT_MOVE(d->character) = 0;
        GET_HIT(d->character)  = GET_MAX_HIT(d->character)  = 50;
        GET_TRAINS(d->character) = 50;
        GET_SEX(d->character) = SEX_MALE;
        GET_RACE(d->character) = RACE_ANDOR;

      }
      if (5 <= number(0,100)) {
        SET_BIT(GET_PERMANENT(d->character),PRM_TAINTRES);
/* Slightly ugly, add TAINT_RES-flag to anyone whith luck, thus here */
/* is the only place where the change is needed to be done           */
      }
      add_bonuses(d);
      do_welcome(d);
      showChar(d);
      STATE(d) = CON_CREATE;
      /* must zap all eventual old alias and eq files */
      if (get_filename(GET_RNAME(d->character), buf, CRASH_FILE) &&
          (f = fopen(buf, "r"))) {
        fclose(f);
        unlink(buf);
      }
      if (get_filename(GET_RNAME(d->character), buf, ALIAS_FILE) &&
          (f = fopen(buf, "r"))) {
        fclose(f);
        unlink(buf);
      }
      if (get_filename(GET_RNAME(d->character), buf, KNOW_FILE) &&
          (f = fopen(buf, "r"))) {
        fclose(f);
        unlink(buf);
      }
      return;
   }
   else {
      save_char(d->character, NOWHERE);
      echo_on(d);
      SEND_TO_Q("\r\nDone.\n\r", d);
      STATE(d) = CON_PLAYING;
    }

    break;

  case CON_CREATE:

    two_arguments(arg,arg1,arg2);

    if (*arg1) {
      if (!strncasecmp(arg1,"STATS",strlen(arg1)))
        showChar(d);

      else if(!strncasecmp(arg1,"RACE",strlen(arg1)))
        if (*arg2)
          do_set_race(d,arg2);
        else
          SEND_TO_Q("Perhaps if you supply a race next time?\r\n",d);

      else if(!strncasecmp(arg1,"CLASS",strlen(arg1)))
        if (*arg2)
          do_set_class(d,arg2);
        else
         SEND_TO_Q("How about setting a class too?\r\n",d);

      else if(!strncasecmp(arg1,"SEX",strlen(arg1)))
        if (*arg2)
          do_set_sex(d,arg2);
        else
          SEND_TO_Q("Hey, you've gotta give me a sex too!\r\n",d);

      else if(!strncasecmp(arg1,"STRENGTH",strlen(arg1)))
        if (*arg2)
          do_set_ability(d,arg2,ABL_STR);
        else
          SEND_TO_Q("And how high do you want your strength to be?\r\n",d);

      else if(!strncasecmp(arg1,"CONSTITUTION",strlen(arg1)))
        if (*arg2)
          do_set_ability(d,arg2,ABL_CON);
        else
          SEND_TO_Q("And how high do you want your constitution to be?\r\n",d);

      else if(!strncasecmp(arg1,"DEXTERITY",strlen(arg1)))
        if (*arg2)
          do_set_ability(d,arg2,ABL_DEX);
        else
          SEND_TO_Q("And how high do you want your dexterity to be?\r\n",d);

      else if(!strncasecmp(arg1,"INTELLIGENCE",strlen(arg1)))
        if (*arg2)
          do_set_ability(d,arg2,ABL_INT);
        else
          SEND_TO_Q("And how high do you want your intelligence to be?!\r\n",d);

      else if(!strncasecmp(arg1,"WISDOM",strlen(arg1)))
        if (*arg2)
          do_set_ability(d,arg2,ABL_WIS);
        else
          SEND_TO_Q("And how high do you want your wisdom to be?\r\n",d);

      else if(!strncasecmp(arg1,"CHARISMA",strlen(arg1)))
        if (*arg2)
          do_set_ability(d,arg2,ABL_CHA);
        else
          SEND_TO_Q("And how high do you want your charisma to be?\r\n",d);

      else if(!strncasecmp(arg1,"MOVES",strlen(arg1)))
        if (*arg2)
          do_set_ability(d,arg2,ABL_MOVES);
        else
          SEND_TO_Q("And how many moves do you wish to have?\r\n",d);

      else if(!strncasecmp(arg1,"MANA",strlen(arg1)))
        if (*arg2)
          do_set_ability(d,arg2,ABL_MANA);
        else
          SEND_TO_Q("And how much mana do you wish to have?\r\n",d);

      else if(!strncasecmp(arg1,"HITPOINTS",strlen(arg1)))
        if (*arg2)
          do_set_ability(d,arg2,ABL_HIT);
        else
          SEND_TO_Q("And how many hitpoints do you wish to have?\r\n",d);

      else if (!strncasecmp(arg1,"HELP",strlen(arg1))) {
        if (!*arg2) {
      do_help(d->character,"creation",0,0);
        }
        else {
     sprintf(tmp,"c%s",arg2);
     do_help(d->character,tmp,0,0);
        }
      }
      else if (!strncasecmp(arg,"READY",strlen(arg))) {
        add_bonuses(d);
        REMOVE_BIT(PRF_FLAGS(d->character),PRF_IC);
        SEND_TO_Q(START_MESSG, d);
        SEND_TO_Q(WELC_MESSG, d);
        SEND_TO_Q(motd, d);
        SEND_TO_Q("\r\n\n*** PRESS RETURN: ", d);

        sprintf(buf, "%s [%s] new player.", GET_NAME(d->character), d->host);
        xlog(SYS_CONN, LVL_IMMORT, buf, TRUE);
    STATE(d) = CON_RMOTD;
    return;
      }
      else {
       SEND_TO_Q("Huh?!?\r\nValid commands are:\r\n",d);
       SEND_TO_Q("   race <racename>            - to set your origin.\r\n", d);
       SEND_TO_Q("   sex <\"male\"|\"female\">  - to set your gender.\r\n", d);
       SEND_TO_Q("   class <classname>          - to set your class.\r\n", d);
       SEND_TO_Q("   <statname> <number>        - to set a stat to desired  value.\r\n", d);
       SEND_TO_Q("   help <topic>               - to get help about <topic>\r\n", d);
       SEND_TO_Q("   stats - when you want your current settings displayed.\r\n", d);
       SEND_TO_Q("   ready - when you'r done with creating and want to start playing.\r\n", d);
      }
    }
    SEND_TO_Q("race/sex/class/help/stats/ready> ", d);
    break;
  case CON_CLOSE:
    close_socket(d);
    break;

  case CON_RMOTD:		/* read CR after printing motd   */
                /* A lot of stuff to get the player goin, but hey, we've got rid of the menu! */
     reset_char(d->character);
       if ( GET_INVSTART(d->character) != 0 )
          GET_INVIS_LEV(d->character) = GET_INVSTART(d->character);

  /* Put ppl in their loadroom befor autoeq them! */
     load_room = GET_LOADROOM(d->character);
     if (!load_room || NOWHERE == load_room) /* Needed in case of strangeties */
       load_room = NOWHERE;
     if (load_room != NOWHERE)
       load_room = real_room(load_room);

    /* If char was saved with NOWHERE, or real_room above failed... */
    if (!GET_LEVEL(d->character))
      load_room = real_room(newbie_room);

    if (load_room == NOWHERE) {
      if (GET_LEVEL(d->character) >= LVL_IMMORT) {
    load_room = r_immort_start_room;
      }
      else {
    load_room = r_mortal_start_room;
      }
    }
    if (PLR_FLAGGED(d->character, PLR_FROZEN))
      load_room = r_frozen_start_room;

     GET_CHAR_GUILDS(d->character) = NULL; /* Just in case */
     fetch_char_guild_data(d->character); /* There could be guild triggers in the loadroom, must have this first */
     
     char_to_room(d->character, load_room);
     read_XML_playerfile(d->character, CRASH_FILE);

     GET_ID(d->character) = GET_IDNUM(d->character);
     save_char(d->character, d->character->in_room);
     save_XML_playerfile(d->character, CRASH_FILE); 
     read_retainers(d->character);
     // Save chars items, in case we stripped/changed anything. 
     send_to_char(WELC_MESSG, d->character);

     d->character->next = character_list;
     character_list = d->character;

     if (NULL == GET_RDESC(d->character)) {
    char temp[6000];
        sprintf(temp,"A typical, run of the mill %s\r\n",GET_NAME(d->character));
    GET_RDESC(d->character) = strdup(temp);
     }

    /* For DGS */
    GET_ID(d->character) = max_id++;
     
    act("$n has entered the game.", TRUE, d->character, 0, 0, TO_ROOM);

    greet_mtrigger(d->character, -1, -1);
    greet_otrigger(d->character, -1);
    greet_memory_mtrigger(d->character);

    if (GET_WAGON(d->character)) {
      wagon = read_object(GET_WAGON(d->character), VIRTUAL);
      obj_to_room(wagon, d->character->in_room);
      d->character->pulling = wagon;
      wagon->pulled_by = d->character;
    }

    if (!GET_LEVEL(d->character)) {
      do_start(d->character);
      do_newbie_eq(d->character);
      REMOVE_BIT(PRF_FLAGS(d->character), PRF_IC);
      send_to_char(START_MESSG, d->character);
    }
    look_at_room(d->character, 0);
    if (has_mail(GET_IDNUM(d->character)))
      send_to_char("You have mail waiting.\r\n", d->character);
    d->prompt_mode = 1;
    read_aliases(d->character);
    SPEAKING(d->character) = 0;
    d->character->master = NULL;

    if (IS_AFFECTED2(d->character,AFF2_LONGSLEEP)) {
      GET_POS(d->character) = POS_SLEEPING;
    }
    sprintf(buf, "%s entering game.", GET_NAME(d->character));
    xlog(SYS_CONN, MAX(LVL_IMMORT, GET_INVIS_LEV(d->character)), buf, TRUE);
//    sprintf(buf, "and given the record at 0x%lX", d->character);
//    xlog(SYS_DEBUG, 0, buf, TRUE );

    /* A quick hack for Newbieguides */
    if (GET_LEVEL(d->character) < 20)                                 
     for (a = descriptor_list; a; a = a->next)
      if (!a->connected && a->character && (a->character != d->character) &&
      !PLR_FLAGGED(a->character, PLR_MAILING | PLR_WRITING | PLR_NOGECHO) &&
      IS_SET((a->character)->player_specials->saved.command_groups, CG_NEWBIE)) {
       sprintf(buf, "[&W %s, the newbie, just entered the game.&n ]\r\n", GET_NAME(d->character));    
       send_to_char(buf, a->character);
     }

    /* Notify List checkups */
   for (d2 = descriptor_list; d2; d2 = d2->next)
    if (!d2->connected)
     if (CAN_SEE_INCLUDING_TAR(d2->character, d->character))
      for (i=0; i<MAX_NOTIFYLIST_LENGTH; i++)
/*      Should we really do partial name matche? A character named Foo loging on
 * will trigger notify.list entries with Foo, Foobar, Foonos, Foo.. etc.. You get
 * the point? :) 
 *
 * Fixed to use exact match, but non case sensitive 02/02-2003, /Stefan
 *
	if (!strncasecmp(d2->character->player_specials->notifylist[i], 
        GET_NAME(d->character), strlen(GET_NAME(d->character))))
 */
       if (!strcasecmp(d2->character->player_specials->notifylist[i],GET_NAME(d->character))) {
	  char	pbuf[255];
	  time_t  timet = time( NULL );
	  strftime( pbuf, 255, "[%m/%d - %H:%M] ", localtime( &timet ) );
	  sprintf( buf, "%s&G%s has logged on.&n\r\n", pbuf, GET_NAME(d->character));
          add_tell_to_buffer(d2->character, buf);
          sprintf(buf, "&G%s has logged on.&n\r\n", GET_NAME(d->character));

          if (PLR_FLAGGED(d2->character,PLR_WRITING|PLR_MAILING|PLR_MMAILING|PLR_GMAILING)) 
           d2->character->player_specials->buffer_flag = 1;
          else 
           send_to_char(buf, d2->character);
       }

    if ((GET_SEX(d->character) != SEX_MALE) && (GET_SEX(d->character) != SEX_FEMALE)) {
      send_to_char("\007\007\r\n&RIt seems there was an error in the Creation process.\r\n"
                   "Please type '&Bdelete yes delete me&R' and try again.\r\n"
                   "We apologise for any inconvienience.&n\r\n", d->character);
      sprintf(buf, "%s (level %ld) has no sex!", GET_NAME(d->character), GET_LEVEL(d->character));
      xlog(SYS_CONN, MAX(LVL_IMMORT, GET_INVIS_LEV(d->character)), buf, TRUE);
    }

    if ((GET_SPARK(d->character) || GET_TEACHABLE(d->character) ||
    (GET_CLASS(d->character) == CLASS_CHANNELER) ||
    (GET_CLASS(d->character) == CLASS_WILDER)) && GET_LEVEL(d->character) < LVL_IMMORT &&
    !PLR_FLAGGED(d->character, PLR_NOGAIN))
      if ((GET_AIR(d->character)+GET_FIRE(d->character)+GET_SPIRIT(d->character)+
      GET_EARTH(d->character)+GET_WATER(d->character)-GET_POWER(d->character)) > 200) {
    sprintf(buf, "SYSERR: %s is overcapped! (A:%d, F:%d, S:%d, E:%d, W:%d|P:%ld)",
      GET_NAME(d->character), GET_AIR(d->character), GET_FIRE(d->character),
      GET_SPIRIT(d->character), GET_EARTH(d->character), GET_WATER(d->character),
      GET_POWER(d->character));
        xlog(SYS_CONN, MAX(LVL_GRGOD, GET_INVIS_LEV(d->character)), buf, TRUE);
    sprintf(buf, "Your character's channeling 'cap' is somehow over your limit (%ld).\r\n\r\n", (200+GET_POWER(d->character)));
    send_to_char(buf, d->character);
    send_to_char("Your channeling flows have been reset to 1 for each flow, and\r\n", d->character);
    send_to_char("the difference reimbursed as trains. Please return to the trainer\r\n", d->character);
    send_to_char("and re-train your channeling ability.\r\n\r\n", d->character);
    GET_TRAINS(d->character) += (GET_POWER(d->character)+195);
    GET_AIR(d->character) = 1;
    GET_FIRE(d->character) = 1;
    GET_SPIRIT(d->character) = 1;
    GET_EARTH(d->character) = 1;
    GET_WATER(d->character) = 1;
      }

    if (NULL == GET_KNOW(d->character)) {
      GET_KNOW(d->character) = BITVECTOR(MAX_ROOMS_ON_MUD);
    }
    STATE(d) = CON_PLAYING;
    break;


  default:
    sprintf(buf, "SYSERR: Nanny: illegal state of con'ness: %ld ; closing connection", STATE(d));
    mlog(buf);
    close_socket(d);
    break;
  }
}

/* Dubbla player_in_room */

void do_welcome(struct descriptor_data *d)
{
  char *Welcome = "\f\r\nWelcome to the Realms of Dragon's Fang.\r\n"
                  "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n\r\n"
                  "You are now about to enter the most\r\n"
                  "important part of your characters life,\r\n"
                  "       _oOo_ CREATION! _oOo_\r\n\r\n"
                  "To see your present stats, type STATS.\r\n"
                  "To get basic help, type HELP.\r\n"
                  "To get additional help, type HELP <subject>.\r\n"
                  "To quit when satisfied, type READY.\r\n\r\n"
                  "/The Staff\r\n\r\n";

  SEND_TO_Q(Welcome,d);
}

void showChar(struct descriptor_data *d)
{
  char da_buf[12000];
  char *sex[3] = { "androgyn","male","female"};

  sprintf(da_buf,"You are %s a %s %s from %s.\r\n",GET_NAME(d->character),sex[(int) GET_SEX(d->character)],pc_class_types[(int) GET_CLASS(d->character)],hometown[(int) GET_RACE(d->character)]);
  sprintf(da_buf,"%sYou are %d years old, and your statistics are now:\r\n\r\n",da_buf,GET_AGE(d->character));
  sprintf(da_buf,"%sStrength    : %d\r\nDexterity   : %d\r\nIntelligence: %d\r\n",da_buf,d->character->real_abils.str,d->character->real_abils.dex , d->character->real_abils.intel);
  sprintf(da_buf,"%sWisdom      : %d\r\nConstitution: %d\r\nCharisma    : %d\r\n",da_buf,d->character->real_abils.wis,d->character->real_abils.con,d->character->real_abils.cha);
  sprintf(da_buf,"%s\r\nMana: %4ld    HP: %4d    Moves: %4d\r\n\r\n",da_buf,GET_MANA(d->character),GET_HIT(d->character),GET_MOVE(d->character));

  switch(GET_CLASS(d->character)) {
    case CLASS_WILDER:
    case CLASS_CHANNELER:
      sprintf(da_buf,"%sYour strength in the Five Powers are:\r\n\r\n",da_buf);
      sprintf(da_buf,"%sFire: 1   Earth: 1",da_buf);
      sprintf(da_buf,"%s   Water: 1   Air: 1   Spirit: 1\r\n\r\n",da_buf);
      break;
  }

  sprintf(da_buf,"%sYou have %d trains to spend.\r\n\r\n",da_buf,GET_TRAINS(d->character));
  sprintf(da_buf,"%srace/sex/class/help/stats/ready> ",da_buf);
  SEND_TO_Q(da_buf,d);
}

void do_class_help(struct descriptor_data *d)
{
  char *str_Class= "With the CLASS-command you can change the class of\r\n"
                   "of your character. Just type CLASS <class-name>. The\r\n"
                   "following classes are available:\r\n\r\n"
                   "Adventurer - An average class. Mostly a fighting-class,\r\n"
                   "             but also some knowledge about healing herbs,\r\n"
                   "             and some woodsman-skills\r\n"
                   "Soldier    - A fighting-class. Lot's of knowledge about\r\n"
                   "             different weapons, and other fighting-skills and\r\n"
                   "             not so very much more.\r\n\r\n"
                   "Craftsman  - Knows how to work with your hands, like repearing\r\n"
                   "             stuff, and tooling it (re-stringing). Some knowledge\r\n"
                   "             about fighting, but not much.\r\n\r\n"
                   "Thief      - Knows a lot about hiding, sneaking, stealing and to\r\n"
                   "             recognize other thieves (i.e finding their hiding-spots\r\n"
                   "             and similar). Knows some basic fighting skills too, and\r\n"
                   "             most important of all, knows how to backstab! :)\r\n\r\n"
                   "Wisdom     - The average willage-wisdom. Lot's of knowledge about\r\n"
                   "             herbs and other healing-skills. Most often NOT a channeler\r\n"
                   "             only a person devoting his/her life as a healer, and\r\n"
                   "             as a master of herb-lore. Of course a wisdom has some knowledge\r\n"
                   "             of fighting too.\r\n\r\n"
                   "Wilder     - A person able to channel, but a person that differs from the\r\n"
                   "             channeler-class as one that has tought her/him-self to channel\r\n"
                   "             avoiding the 3/4-chanse to die doing so. A wilder don't know as\r\n"
                   "             many weaves as the channeler does, although the wilders weaves\r\n"
                   "             are often more powerful that those of a channeler. Can learn about\r\n"
                   "             the same fighting-skills that an adventurer, although later in life\r\n"
                   "             and at a higher cost.\r\n\r\n"
                   "Channeler  - A person that have been tought have to channel, i.e someone has\r\n"
                   "             guided the character through the critical parts of learing.\r\n"
                   "             A channeler knows more weaves than the wilder, although often\r\n"
                   "             not as powerful ones. Like the wilder, the channeler can learn\r\n"
                   "             most of the fighting-skills that an adventurer can, but later in\r\n"
                   "             life, and at a higher cost.\r\n\r\n";

  SEND_TO_Q(str_Class,d);
}

void do_race_help(struct descriptor_data *d)
{
  char *str_Race = "Persons from differnet parts of the realm, has different\r\n"
           "characteristics. As an example, a person from Shienar\r\n"
                   "might be (in average) stronger than a person from Illian.\r\n"
                   "But the Illianer might be smarter than the Shienaran.\r\n"
                   "In Dragon's Fang we've made thos differences as modificatuions,\r\n"
                   "i.e. with a +3 modifier for strength, you start out with \r\n"
                   "strength 14 instead of strength 11. So, you'll save a few\r\n"
                   "points trying to increase your strength to max. (Max will always\r\n"
                   "be 30, despite your bonuses. Another thing with races is, with the\r\n"
                   "HOME-command, yoy'll be transfered to your hometown, i.e a Shienaran\r\n"
                   "will be tranfered to Fal Dara, and a Andorian will be sent to Caemlyn.\r\n"
                   "Below are a list of 'races' and their bonuses:\r\n\r\n"
                   "Races           | Str | Con | Dex | Int | Wis |Cha | Move | Mana | Hp |\r\n"
                   "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n"
                   "Three-fold land | +1  | +1  | +2  |  -2 |  0  | -2 |   0  |  0   |  0 |\r\n"
                   "Almoth          |  0  |  0  |  1  |  -1 | -1  |  1 | -10  |  0   | 10 |\r\n"
                   "Amadicia        |  0  |  0  |  0  |   1 |  1  |  0 | -10  |  0   |-10 |\r\n"
                   "Andor           |  0  |  0  |  1  |   0 | -1  |  0 |   0  |  0   |  0 |\r\n"
                   "Arad Doman      | -1  | -1  |  1  |   0 |  0  |  0 |   0  | 10   |  0 |\r\n"
                   "Borderlands     |  2  |  2  |  0  |  -1 | -2  | -2 |   0  |  0   |-10 |\r\n"
                   "Cairhien        | -1  | -1  | -1  |   2 |  1  |  0 |   0  |  0   |  0 |\r\n"
                   "Illian          | -1  | -2  |  0  |   0 |  2  |  0 |  10  |  0   |  0 |\r\n"
                   "Murandy         | -1  | -1  | -1  |   1 |  2  |  1 | -10  |  0   |  0 |\r\n"
                   "Seanchan        | -1  |  0  |  1  |   0 | -1  |  1 |   0  |  0   |  0 |\r\n"
                   "Tarabon         |  0  |  0  |  1  |   0 |  1  | -1 |   0  |-10   |  0 |\r\n"
                   "Tar Valon       | -1  | -1  |  0  |   1 |  1  |  0 |   0  |  0   |  0 |\r\n"
                   "Tear            |  2  |  1  | -1  |   0 | -1  | -1 |   0  |  0   |  0 |\r\n"
                   "Tremalking      |  0  | -1  |  1  |  -1 |  1  |  1 | -10  |  0   |  0 |\r\n"
                   "Two Rivers      |  1  |  1  |  0  |  -1 |  0  | -1 |   0  |  0   |  0 |\r\n"
                   "Altara          | -1  | -1  | -1  |   2 |  2  | -1 | -10  |  20  | -10|\r\n"
                   "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n\r\n";

  SEND_TO_Q(str_Race,d);
}

void do_ability_help(struct descriptor_data *d)
{
  char *str_Ability = "To define your characters abilities, or lack of abilites,\r\n"
                      "there are certain values that defines your Strength, \r\n"
                      "Constitution, Dexterity, Intelligence, Wisdom, Charisma\r\n"
                      "Moves, Mana and Hitpoints(HP).\r\n\r\n"
                      "Strength     - With a high strength, your character can carry more\r\n"
                      "               equipment, and give more damage in battle.\r\n\r\n"
                      "Constitution - With a high constitution your character will gain\r\n"
                      "               more Hitpoints every time you gain a level.\r\n\r\n"
                      "Dexterity    - With a high dexterity, your character will be more\r\n"
                      "               agile, i.e will have a higher chance to avoid being\r\n"
                      "               hit by an enemy-\r\n\r\n"
                      "Intelligence - With a high intelligence, your character have a higher\r\n"
                      "               chance to get more practice, which are used to learn skills.\r\n"
                      "               Learning will also be easier for the intelligent character.\r\n"
                      "Wisdom       - With a high wisdom, your character will have a higher chance\r\n"
                      "               to gain 'trains' (See HELP POWER). \r\n\r\n"
                      "Charisma     - With high charisma, your character will gain lower prices\r\n"
                      "               in shops, .i.e the perfect ability for the greedy. :)\r\n\r\n"
                      "Moves        - Moves are used to move around the realm. Every move in a direcrtion\r\n"
                      "               will cost you moves. When your moves are 0, you'll have to rest before\r\n"
                      "               continuing your travels.\r\n\r\n"
                      "Mana         - Mana is used for weaving the One Power. The more Mana, more powerful\r\n"
                      "               weaves your character will be able to use.\r\n\r\n"
                      "Hitpoints    - This is the amount of beating up your character will be able to take\r\n"
                      "               before he/she dies. The more Hitpoints, the tougher, hard-hitting mobs\r\n"
                      "               he/she will be able to fight.\r\n\r\n"
                      "To increase one of the abilities, just type the ability name, and the value you wish it\r\n"
                      "to have. I.e to get Strength 25, just type Strength 25. If yuo wish to lower your abilities\r\n"
                      "to gain more trains, just type (for example) Strength 5, which will give you some extra\r\n"
                      "trains.\r\nThe cost for increasing a ability is raising with raised ability as this:\r\n\r\n"
                      "  0 - 18 -> 1 train/step\r\n 19-25 -> 2 trains/step\r\n 26-39 -> 3 trains/step\r\n"
                      "To raise your Moves, Hitpoints or Mana, type the ability and the level you wish to have, \r\n"
                      "for example: Mana 110 will raise your mana from to 110. You can, as with the other abilities\r\n"
                      "lower your level. Mana/Moves/Hitpoints ALL costs 1 train/10 steps, despite what your levle\r\n"
                      "you already have.\r\n\r\n";

  SEND_TO_Q(str_Ability,d);
}

void do_power_help(struct descriptor_data *d)
{
  char *str_Power = "If you're a channeler/wisdom OR have the Spark inborn you will\r\n"
                    "have characteristics describing your strength in the Five Powers\r\n"
                    "(i.e Air, Water, Spirit, Fire and Earth). You will also be able\r\n"
                    "to spend the trains you gain as you advance in levels to increase your\r\n"
                    "strength.\r\nYour strength in the Five Powers will determine what\r\n"
                    "weaves your character can weave. I.e Level doesn't matter in this case.\r\n\r\n";

  SEND_TO_Q(str_Power,d);
}

void do_set_sex(struct descriptor_data *d,char *arg)
{
  if (!strncasecmp(arg,"MALE",strlen(arg))) {
    GET_SEX(d->character) = SEX_MALE;
    SEND_TO_Q("Okay, you're now male.\r\n",d);
  }
  else if (!strncasecmp(arg,"FEMALE",strlen(arg))) {
    GET_SEX(d->character) = SEX_FEMALE;
    SEND_TO_Q("Okay, you're now female.\r\n",d);
  }
  else
   SEND_TO_Q("Hey, you REALLY wanna be an androgyn!?\r\n",d);
}

void do_set_class(struct descriptor_data *d,char *arg)
{
  if (!strncasecmp(arg,"ADVENTURER",strlen(arg))) {
    GET_CLASS(d->character) = CLASS_ADVENTURER;
    SEND_TO_Q("Okay, you're now an Adventurer.\r\n",d);
  }
  else if (!strncasecmp(arg,"WILDER",strlen(arg))) {
    GET_CLASS(d->character) = CLASS_WILDER;
    SEND_TO_Q("Okay, you're now a Wilder.\r\n",d);
  }
  else if (!strncasecmp(arg,"CHANNELER",strlen(arg))) {
    GET_CLASS(d->character) = CLASS_CHANNELER;
    SEND_TO_Q("Okay, you're now a Channeler.\r\n",d);
  }
  else if (!strncasecmp(arg,"SOLDIER",strlen(arg))) {
    GET_CLASS(d->character) = CLASS_SOLDIER;
    SEND_TO_Q("Okay, you're now a Soldier.\r\n",d);
  }
  else if (!strncasecmp(arg,"THIEF",strlen(arg))) {
    GET_CLASS(d->character) = CLASS_THIEF;
    SEND_TO_Q("Okay, you're now a Thief.\r\n",d);
  }
  else if (!strncasecmp(arg,"WISDOM",strlen(arg))) {
    GET_CLASS(d->character) = CLASS_WISDOM;
    SEND_TO_Q("Okay, you're now a Wisdom.\r\n",d);
  }
  else if (!strncasecmp(arg,"CRAFTSMAN",strlen(arg))) {
    GET_CLASS(d->character) = CLASS_CRAFTSMAN;
    SEND_TO_Q("Okay, you're now a Craftsman.\r\n",d);
  }
  else
    SEND_TO_Q("And what kind of a class is that?\r\n",d);
}

void withdraw_bonuses(struct descriptor_data *d)
{
  int race = GET_RACE(d->character);

  GET_RSTR(d->character)  -= modifiers[race][0];
  GET_RCON(d->character)  -= modifiers[race][1];
  GET_RDEX(d->character)  -= modifiers[race][2];
  GET_RINT(d->character)  -= modifiers[race][3];
  GET_RWIS(d->character)  -= modifiers[race][4];
  GET_RCHA(d->character)  -= modifiers[race][5];
  GET_MOVE(d->character) -= modifiers[race][6];
  GET_FRACT_MOVE(d->character) = 0;
  ADD_MANA(d->character, -modifiers[race][7]);
  GET_HIT(d->character)  -= modifiers[race][8];
}

void add_bonuses(struct descriptor_data *d)
{
  int race = (int) GET_RACE(d->character);

  GET_RSTR(d->character)  += modifiers[race][0];
  GET_RCON(d->character)  += modifiers[race][1];
  GET_RDEX(d->character)  += modifiers[race][2];
  GET_RINT(d->character)  += modifiers[race][3];
  GET_RWIS(d->character)  += modifiers[race][4];
  GET_RCHA(d->character)  += modifiers[race][5];
  GET_MOVE(d->character)  += modifiers[race][6];
  GET_FRACT_MOVE(d->character) = 0;
  ADD_MANA(d->character,  modifiers[race][7]);
  GET_HIT(d->character)   += modifiers[race][8];
}

void do_set_race(struct descriptor_data *d,char *arg)
{
  char temp_buf[200];
  void level_stats(struct descriptor_data *d);

  if (!strncasecmp(arg,hometown[RACE_AIEL],strlen(arg))) {
    GET_RACE(d->character) = RACE_AIEL;
    level_stats(d);
    sprintf(temp_buf,"Okey, you're now from %s.\r\n",hometown[RACE_AIEL]);
    SEND_TO_Q(temp_buf,d);
  }
  else if (!strncasecmp(arg,hometown[RACE_ALMOTH],strlen(arg))) {
    GET_RACE(d->character) = RACE_ALMOTH;
    level_stats(d);
    sprintf(temp_buf,"Okey, you're now from %s.\r\n",hometown[RACE_ALMOTH]);
    SEND_TO_Q(temp_buf,d);
  }
  else if (!strncasecmp(arg,hometown[RACE_AMADICIA],strlen(arg))) {
    GET_RACE(d->character) = RACE_AMADICIA;
    level_stats(d);
    sprintf(temp_buf,"Okey, you're now from %s.\r\n",hometown[RACE_AMADICIA]);
    SEND_TO_Q(temp_buf,d);
  }
  else if (!strncasecmp(arg,hometown[RACE_ANDOR],strlen(arg))) {
    GET_RACE(d->character) = RACE_ANDOR;
    level_stats(d);
    sprintf(temp_buf,"Okey, you're now from %s.\r\n",hometown[RACE_ANDOR]);
    SEND_TO_Q(temp_buf,d);
  }
  else if (!strncasecmp(arg,hometown[RACE_ARADDOMAN],strlen(arg))) {
    GET_RACE(d->character) = RACE_ARADDOMAN;
    level_stats(d);
    sprintf(temp_buf,"Okey, you're now from %s.\r\n",hometown[RACE_ARADDOMAN]);
    SEND_TO_Q(temp_buf,d);
  }
  else if (!strncasecmp(arg,hometown[RACE_BORDERLANDS],strlen(arg))) {
    GET_RACE(d->character) = RACE_BORDERLANDS;
    level_stats(d);
    sprintf(temp_buf,"Okey, you're now from %s.\r\n",hometown[RACE_BORDERLANDS]);
    SEND_TO_Q(temp_buf,d);
  }
   else if (!strncasecmp(arg,hometown[RACE_CAIRHIEN],strlen(arg))) {
    GET_RACE(d->character) = RACE_CAIRHIEN;
    level_stats(d);
    sprintf(temp_buf,"Okey, you're now from %s.\r\n",hometown[RACE_CAIRHIEN]);
    SEND_TO_Q(temp_buf,d);
  }
  else if (!strncasecmp(arg,hometown[RACE_ILLIAN],strlen(arg))) {
    GET_RACE(d->character) = RACE_ILLIAN;
     level_stats(d);
    sprintf(temp_buf,"Okey, you're now from %s.\r\n",hometown[RACE_ILLIAN]);
    SEND_TO_Q(temp_buf,d);
  }
  else if (!strncasecmp(arg,hometown[RACE_MURANDY],strlen(arg))) {
    GET_RACE(d->character) = RACE_MURANDY;
    level_stats(d);
    sprintf(temp_buf,"Okey, you're now from %s.\r\n",hometown[RACE_MURANDY]);
    SEND_TO_Q(temp_buf,d);
  }
  else if (!strncasecmp(arg,hometown[RACE_SEANCHAN],strlen(arg))) {
    GET_RACE(d->character) = RACE_SEANCHAN;
    level_stats(d);
    sprintf(temp_buf,"Okey, you're now from %s.\r\n",hometown[RACE_SEANCHAN]);
    SEND_TO_Q(temp_buf,d);
  }
  else if (!strncasecmp(arg,hometown[RACE_TARVALON],strlen(arg))) {
    GET_RACE(d->character) = RACE_TARVALON;
    level_stats(d);
    sprintf(temp_buf,"Okey, you're now from %s.\r\n",hometown[RACE_TARVALON]);
    SEND_TO_Q(temp_buf,d);
  }
  else if (!strncasecmp(arg,hometown[RACE_TARABON],strlen(arg))) {
    GET_RACE(d->character) = RACE_TARABON;
    level_stats(d);
    sprintf(temp_buf,"Okey, you're now from %s.\r\n",hometown[RACE_TARABON]);
    SEND_TO_Q(temp_buf,d);
  }
  else if (!strncasecmp(arg,hometown[RACE_TEAR],strlen(arg))) {
    GET_RACE(d->character) = RACE_TEAR;
    level_stats(d);
    sprintf(temp_buf,"Okey, you're now from %s.\r\n",hometown[RACE_TEAR]);
    SEND_TO_Q(temp_buf,d);
  }
  else if (!strncasecmp(arg,hometown[RACE_TREMALKING],strlen(arg))) {
    GET_RACE(d->character) = RACE_TREMALKING;
    level_stats(d);
    sprintf(temp_buf,"Okey, you're now from %s.\r\n",hometown[RACE_TREMALKING]);
    SEND_TO_Q(temp_buf,d);
  }
  else if (!strncasecmp(arg,hometown[RACE_TWO_RIVERS],strlen(arg))) {
    GET_RACE(d->character) = RACE_TWO_RIVERS;
    level_stats(d);
    sprintf(temp_buf,"Okey, you're now from %s.\r\n",hometown[RACE_TWO_RIVERS]);
    SEND_TO_Q(temp_buf,d);
  }
  else if (!strncasecmp(arg,hometown[RACE_COUR_SOUVRA],strlen(arg))) {
    GET_RACE(d->character) = RACE_COUR_SOUVRA;
    level_stats(d);
    sprintf(temp_buf,"Okey, you're now from %s.\r\n",hometown[RACE_COUR_SOUVRA]);
    SEND_TO_Q(temp_buf,d);
  }
/*  else if (!strncasecmp(arg,hometown[RACE_OGIER],strlen(arg))) {
    GET_RACE(d->character) = RACE_OGIER;
    level_stats(d);
    sprintf(temp_buf,"Okey, you're now from %s.\r\n",hometown[RACE_OGIER]);
    SEND_TO_Q(temp_buf,d);
  } */
  else
    SEND_TO_Q("And what kind of a race is that?\r\n",d);

}

void level_stats(struct descriptor_data *d)
{
  if (GET_RSTR(d->character)  > 30) GET_RSTR(d->character)   = 30;
  if (GET_RCON(d->character)  > 30) GET_RCON(d->character)   = 30;
  if (GET_RDEX(d->character)  > 30) GET_RDEX(d->character)   = 30;
  if (GET_RINT(d->character)  > 30) GET_RINT(d->character)   = 30;
  if (GET_RWIS(d->character)  > 30) GET_RWIS(d->character)   = 30;
  if (GET_RCHA(d->character)  > 30) GET_RCHA(d->character)   = 30;
}

void do_set_ability(struct descriptor_data *d,char *arg2,int abl)
{
  int value, wrong = 0;
  int current, cost, up, incr, i=0;
  char temp_buf[200];

  strcpy(temp_buf, arg2);
  while (temp_buf[i])
  {
    if ((temp_buf[i] < '0') || (temp_buf[i] > '9'))
      wrong = TRUE;
    i++;
  }
  if (wrong)
  {
    SEND_TO_Q("Please type in a valid number as an argument.\r\n", d);
    return;
  }
  value = atoi(arg2);
  if (((value < 3)||(value > 28)) && (abl != ABL_MOVES) && (abl != ABL_MANA) && (abl != ABL_HIT))
  {
    SEND_TO_Q("Sorry, you're not allowed to set the value above 28 or below 3.\r\n",d);
    return;
  }

  switch(abl) {
    case ABL_STR:
      current = d->character->real_abils.str;
      break;

    case ABL_CON  :
      current =d->character->real_abils.con;
     break;

    case ABL_DEX  :
      current = d->character->real_abils.dex;
           break;

    case ABL_INT  :
      current = d->character->real_abils.intel;
      break;

    case ABL_WIS  :
      current = d->character->real_abils.wis;
      break;

    case ABL_CHA  :
      current = d->character->real_abils.cha;
      break;

    case ABL_MOVES:
      current = GET_MAX_MOVE(d->character);
      break;

    case ABL_MANA :
      current = GET_MAX_MANA(d->character);
      break;

    case ABL_HIT  :
      current = GET_MAX_HIT(d->character);
      break;

    default       :
      SEND_TO_Q("And what kind of ability is that?\r\n",d);
      return;
      break;
  }
  if ((abl == ABL_MANA)||(abl == ABL_MOVES) || (abl ==ABL_HIT))
    current = (int) current/10;
  if (current < value)
    up = TRUE;
  else if (current > value)
    up = FALSE;
  else
  {
    SEND_TO_Q("You already have that value with that stat!\r\n", d);
    return;
  }
  if (up)
    incr = 1;
  else
    incr = -1;
  for (; current != value ; current+= incr)
  {
    if ((current + up) <= da_costs[1].level)
      if ((current + up) <= da_costs[0].level)
        cost = 1;
      else cost = 2;
    else cost = 3;

    if ((GET_TRAINS(d->character) < cost)&&(up))
    {
      sprintf(temp_buf, "You don't have trains enough to raise the stat above %d.\r\n", current);
      SEND_TO_Q(temp_buf, d);
      return;
    }

    switch(abl)
    {
      case ABL_STR:
        up? d->character->real_abils.str++ : d->character->real_abils.str--;
        break;
      case ABL_CON  :
        up? d->character->real_abils.con++ : d->character->real_abils.con--;
        break;
      case ABL_DEX  :
        up? d->character->real_abils.dex++ : d->character->real_abils.dex--;
        break;
      case ABL_INT  :
        up? d->character->real_abils.intel++ : d->character->real_abils.intel--;
        break;
      case ABL_WIS  :
        up? d->character->real_abils.wis++ : d->character->real_abils.wis--;
        break;
      case ABL_CHA  :
        up? d->character->real_abils.cha++ : d->character->real_abils.cha--;
        break;
      case ABL_MOVES:
        if (up)
          GET_MOVE(d->character) = GET_MAX_MOVE(d->character) += 10;
        else
          GET_MOVE(d->character) = GET_MAX_MOVE(d->character) -= 10;
        break;
      case ABL_MANA :
        if (up) {
          ADD_MANA(d->character, 10);
	  ADD_MAX_MANA(d->character, 10);
	}
        else {
          ADD_MANA(d->character, -10);
	  ADD_MAX_MANA(d->character, -10);
	}
        break;
      case ABL_HIT  :
        if (up)
          GET_HIT(d->character) = GET_MAX_HIT(d->character) += 10;
        else
          GET_HIT(d->character) = GET_MAX_HIT(d->character) -= 10;
     break;
    }
    if ((abl == ABL_MOVES) || (abl == ABL_MANA) || (abl == ABL_HIT))
    {
      if (up)
        GET_TRAINS(d->character)--;
      else
        GET_TRAINS(d->character)++;
    }
    else
    {
      if (up)
        GET_TRAINS(d->character) -= cost;
      else
        GET_TRAINS(d->character) += cost;
    }
  }
  if (abl == ABL_MANA)
    current = GET_MANA(d->character);
  if (abl == ABL_MOVES)
    current = GET_MOVE(d->character);
  if (abl ==ABL_HIT)
    current = GET_HIT(d->character);
  sprintf(temp_buf,"Stat set to %d and you have %d trains left. \r\n", current, GET_TRAINS(d->character));
  SEND_TO_Q(temp_buf, d);
}

