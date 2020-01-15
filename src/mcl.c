/******************************************************************************
 * $Id: mcl.c,v 1.43 2003/08/14 03:15:03 fang Exp $
 *   
 * Fil: mcl.c
 *
 * Created: Mon Sep 2 2002 by Darren Benham (fischer@thepics.org)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: mcl.c,v $
 *   Revision 1.43  2003/08/14 03:15:03  fang
 *   Added nopray command.
 *
 *   Revision 1.42  2003/06/20 09:46:21  fang
 *
 *   Start code for a msg-editor in. Very basic, skeletal code for the first simple
 *   step only.
 *
 *   Revision 1.41  2003/06/17 13:02:37  fang
 *
 *   Added a single file for messages. Code for now contains only functionalty to
 *   save messages (fight/spell op/spell tp) as XML. All structs, and data, and
 *   defines pertaining to messages have been moved to the new header.
 *
 *   Revision 1.40  2003/06/12 07:22:13  fang
 *   Changed testing for channeling ability as per HGL request. Fixed Room Seal to work as originally intended. Commented out ignorelist. Will fully remove once certain no problems will arise
 *
 *   Revision 1.39  2003/05/29 03:51:27  fang
 *   Added subcmds to flee.
 *
 *   Revision 1.38  2003/05/22 02:18:40  fang
 *   The zap command (do_pzap) was only useful for harassing morts. By admin
 *   request, it has been disabled. Instead of removing it, I just commented
 *   it out. --Tze
 *
 *   Revision 1.37  2003/05/13 12:02:56  fang
 *   Minor corrections...
 *
 *   Revision 1.36  2003/05/11 14:50:20  fang
 *   DYnvar/noformat handling code should be
 *    complete. Minor bugs may remain, so keep an eye open
 *
 *   Revision 1.35  2003/05/11 03:05:11  fang
 *   Fixed do_weather, added wset to mcl. --Tzeentch.
 *
 *   Revision 1.34  2003/05/06 12:37:32  fang
 *   *** empty log message ***
 *
 *   Revision 1.33  2003/03/04 22:06:04  fang
 *   fisch: added a quick logger function to add coments to syslog
 *
 *   Revision 1.32  2003/03/01 07:17:05  fang
 *   Fisch: Multiflag for authorized multies
 *
 *   Revision 1.31  2002/10/31 03:01:03  fang
 *   fischer: removing the last traces of old newbiegoto
 *
 *   Revision 1.30  2002/10/29 03:09:26  fang
 *   fisch: added the command "scene".  Scenes are like doings for rooms.
 *
 *   Revision 1.29  2002/10/23 04:17:19  fang
 *   fisch: "list board" to list boards with new messages added
 *
 *   Revision 1.28  2002/10/22 04:14:59  fang
 *   fisch: removing gboard
 *
 *   Revision 1.27  2002/10/22 03:31:46  fang
 *   Fisch: added code for zcmd command
 *
 *   Revision 1.26  2002/10/16 16:54:40  fang
 *   *** empty log message ***
 *
 *   Revision 1.25  2002/10/14 03:15:50  fang
 *   fisch: added "edit" for the message board messages
 *
 *   Revision 1.24  2002/10/09 19:13:01  fang
 *   Added "apropos" command to search help texts.
 *
 *   Revision 1.23  2002/10/02 20:57:22  fischer
 *   fischer: removed CG_ADMIN from "revive"
 *
 *   Revision 1.22  2002/09/27 08:14:50  fischer
 *   Fischer: changed revive from level 210 to 203.  Still CG_ADMIN
 *
 *   Revision 1.21  2002/09/23 06:42:57  fischer
 *   fischer: removed autoassist command in favor of toggle autoassist (uniform)
 *
 *   Revision 1.20  2002/09/22 21:43:49  fischer
 *   Removing mounts and horsetrain from the code
 *
 *   Revision 1.19  2002/09/16 22:18:22  fang
 *   fisch: changed the order of "back" so that backstab comes before backhand
 *
 *   Revision 1.18  2002/09/15 22:53:56  fang
 *   Drath: XLOG tag fix for SET and MCL update for BALANCE
 *
 *   Revision 1.17  2002/09/15 02:15:26  fang
 *   Drath: Fixed guild code bug which would cause random crashes.  Uptime should last days now.
 *
 *   Revision 1.16  2002/09/14 02:58:13  fang
 *   Drath: NH syslog for non-help check for chars less than level 20
 *
 *   Revision 1.15  2002/09/13 16:37:23  fang
 *   Drath: More new prompt additions and mcl edits
 *
 *   Revision 1.14  2002/09/13 15:34:06  fang
 *   Drath: More PROMPT additions
 *
 *   Revision 1.13  2002/09/13 01:43:16  fang
 *   Drath: New boards added for Houses project
 *
 *   Revision 1.12  2002/09/12 20:25:14  fang
 *   fischer: Added the "standard" header to C file I created.
 *
 */

#include "interpreter.h"
#include "act.h"
#include "chess.h"
#include "db.h"
#include "portstone.h"
#include "olc.h"
#include "retainer.h"
#include "dynvars.h"
#include "weather.h"
#include "messages.h"
#include "msgedit.h"

/* This is the Master Command List(tm).

 * You can put new commands in, take commands out, change the order
 * they appear in, etc.  You can adjust the "priority" of commands
 * simply by changing the order they appear in the command list.
 * (For example, if you want "as" to mean "assist" instead of "ask",
 * just put "assist" above "ask" in the Master Command List(tm).
 *
 * In general, utility commands such as "at" should have high priority;
 * infrequently used and dangerously destructive commands should have low
 * priority.
 */
struct command_info *complete_cmd_info;
struct command_info cmd_info[] = {
  { "RESERVED"		, ""		, 0		, 0			, 0		, 0			, 0  },

  { "north"		, "north"	, POS_STANDING	, do_move		, 0		, SCMD_NORTH		, 0  },
  { "east"		, "east"	, POS_STANDING	, do_move		, 0		, SCMD_EAST		, 0  },
  { "south"		, "south"	, POS_STANDING	, do_move		, 0		, SCMD_SOUTH		, 0  },
  { "west"		, "west"	, POS_STANDING	, do_move		, 0		, SCMD_WEST		, 0  },
  { "up"		, "up"		, POS_STANDING	, do_move		, 0		, SCMD_UP		, 0  },
  { "down"		, "down"	, POS_STANDING	, do_move		, 0		, SCMD_DOWN		, 0  },
  { "northeast"		, "northeast"	, POS_STANDING	, do_move		, 0		, SCMD_NORTHEAST	, 0  },
  { "ne"		, "ne"		, POS_STANDING	, do_move		, 0		, SCMD_NORTHEAST	, 0  },
  { "southeast"		, "southeast"	, POS_STANDING	, do_move		, 0		, SCMD_SOUTHEAST	, 0  },
  { "se"       		, "se"       	, POS_STANDING	, do_move		, 0		, SCMD_SOUTHEAST	, 0  },
  { "southwest"		, "southwest"	, POS_STANDING	, do_move		, 0		, SCMD_SOUTHWEST	, 0  },
  { "sw"		, "sw"		, POS_STANDING	, do_move		, 0		, SCMD_SOUTHWEST	, 0  },
  { "northwest"		, "northwest"	, POS_STANDING	, do_move		, 0		, SCMD_NORTHWEST	, 0  },
  { "nw"		, "nw"		, POS_STANDING	, do_move		, 0		, SCMD_NORTHWEST	, 0  },

  /* Here's where we'll put the "priority commands" for the MCL */

  { "at"		, "at"		, POS_DEAD	, do_at			, LVL_IMMORT	, 0			, 0  },
  { "balance"		, "ba"		, POS_STANDING	, do_not_here		, 1		, 0			, 0  },
  { "canmulti"    ,  "can"     ,  POS_DEAD    ,  do_wizutil  ,  LVL_IMMORT, SCMD_MULTI,  0  },
  { "cast"  		, "c"		, POS_SITTING	, do_cast		, 1		, SCMD_MAINTAINED	, 0  },
  { "char"		, "cha"		, POS_RESTING	, do_not_here		, 0		, 0			, 0  },
  { "cons"		, "con"		, POS_RESTING	, do_not_here		, 0		, 0			, 0  },
  { "dext"		, "dex"		, POS_RESTING	, do_not_here		, 0		, 0			, 0  },
  { "drop"		, "dr"		, POS_RESTING	, do_drop		, 0		, SCMD_DROP		, 0  },
  { "glance"		, "gl"		, POS_RESTING	, do_glance		, 0		, 0			, 0  },
  { "inventory"		, "i"		, POS_DEAD	, do_inventory		, 0		, 0			, 0  },
  { "inte"		, "int"		, POS_RESTING	, do_not_here		, 0		, 0			, 0  },
  { "kill"		, "k"		, POS_FIGHTING	, do_kill		, 0		, 0			, 0  },
  { "look"  		, "l"		, POS_RESTING	, do_look		, 0		, SCMD_LOOK		, 0  },
  { "ooc"   	   	, "o"		, POS_DEAD	, do_gen_comm		, 1		, SCMD_OOC		, 0  },
  { "put"		, "p"		, POS_RESTING	, do_put		, 0		, 0			, 0  },
  { "read"		, "r"		, POS_RESTING	, do_look		, 0		, SCMD_READ		, 0  },
  { "score"    		, "sc"		, POS_DEAD	, do_score		, 0		, 0			, 0  },
  { "stand"		, "sta"		, POS_SLEEPING	, do_stand		, 0		, 0			, 0  },
  { "stre"		, "str"		, POS_RESTING	, do_not_here		, 0		, 0			, 0  },
  { "tell"	     	, "t"		, POS_DEAD	, do_tell		, 0		, 0			, 0  },
  { "who"		, "wh"		, POS_DEAD	, do_who		, 0		, 0			, 0  },
  { "wisd"		, "wis"		, POS_RESTING	, do_not_here		, 0		, 0			, 0  },

  /* Here's the main MCL */

  { "achieve"		, "ach"		, POS_DEAD	, do_not_here		, 1		, 0			, 0  },
  { "air"		, "air"		, POS_DEAD	, do_not_here		, 1		, 0			, 0  },
  { "aim"		, "aim"		, POS_STANDING	, do_aim		, 0		, 0			, 0  },
  { "amt"		, "amt"		, POS_SLEEPING	, do_amt		, 0		, 0			, 0  },
  { "addguild"		, "addg"	, POS_DEAD	, do_addguild		, LVL_IMPL	, 0			, CG_ADMIN|CG_WEAVER|CG_OWNER  },
  { "adt"		, "adt"		, POS_SLEEPING	, do_adt		, 0		, 0			, 0  },
  { "advance"		, "adv"		, POS_DEAD	, do_advance		, LVL_GOD	, 0			, CG_ADMIN  },
  { "aedit"		, "aed"		, POS_DEAD	, do_olc		, LVL_IMMORT	, SCMD_OLC_AEDIT	, CG_AEDIT },
  { "affected"		, "aff"		, POS_DEAD	, do_affected		, 0		, 0			, 0  },
  { "ajahset"		, "aja"		, POS_DEAD	, do_ajahset		, LVL_GOD	, 0			, CG_GL  },
  { "alias"		, "ali"		, POS_DEAD	, do_alias		, 0		, 0			, 0  },
  { "allguilds"		, "allg"	, POS_DEAD	, do_allguilds		, 0		, 0			, 0  },
  { "apply"		, "appl"	, POS_RESTING	, do_apply		, 0		, 0			, 0  },
  { "appraise"		, "appr"	, POS_RESTING	, do_appraise		, 1		, 0			, 0  },
  { "apropos"		, "apr"		, POS_RESTING	, do_apropos		, 1		, 0			, 0  },
  { "armscry"		, "arm"		, POS_STANDING	, do_armscry		, 0		, 0			, 0  },
  { "arrest"		, "arre"	, POS_STANDING	, do_arrest		, 0		, 0			, 0  },
  { "arrogance"		, "arro"	, POS_STANDING	, do_arrogance		, 0		, 0			, 0  },
  { "ashasense"		, "ashas"	, POS_RESTING	, bondsense		, 0		, SCMD_ASHASENSE	, 0  },
  { "assist"		, "ass"		, POS_FIGHTING	, do_assist		, 1		, 0			, 0  },
  { "ask"		, "ask"		, POS_RESTING	, do_spec_comm		, 0		, SCMD_ASK		, 0  },
  { "auction"		, "auc"		, POS_SLEEPING	, do_gen_comm 		, 0		, SCMD_AUCTION		, 0  },
  { "authorize"		, "auth"	, POS_DEAD	, do_authorize		, LVL_IMMORT	, 0			, 0  },
  { "award"		, "award"	, POS_DEAD	, do_award		, LVL_IMMORT	, 0			, 0  },
  { "awareness"		, "aware"	, POS_RESTING	, do_awareness		, 20		, 0			, 0  },
 
  { "backstab"		, "ba"		, POS_STANDING	, do_backstab		, 1		, 0			, 0  },
  { "background"	, "backg"	, POS_DEAD	, do_background		, 0		, 0			, 0  },
  { "ban"		, "ban"		, POS_DEAD	, do_ban		, LVL_GRGOD	, 0			, 0  },
  { "bandmarch"		, "band"	, POS_STANDING	, do_bandmarch		, 20		, 0			, 0  },
  { "bargain"		, "bar"		, POS_STANDING	, do_bargain		, 20		, 0			, 0  },
  { "bash"		, "bas"		, POS_FIGHTING	, do_bash		, 1		, 0			, 0  },
  { "balefire"		, "bale"	, POS_DEAD	, do_zap		, LVL_IMPL	, 0			, 0  },
  { "bdt"		, "bdt"		, POS_DEAD	, do_bdt		, 0		, SCMD_BDT		, 0  },
  { "become"		, "bec"		, POS_RESTING	, do_become		, 0		, 0			, 0  },
  { "bground"		, "bgr"		, POS_DEAD	, do_bground		, 0		, 0			, 0  },
  { "blademastery"	, "bla"		, POS_DEAD	, do_blademastery	, 1		, 0			, 0  },
  { "bmlist"		, "bm"		, POS_RESTING	, do_bmlist		, 0		, 0			, 0  },
  { "bondsense"		, "bonds"	, POS_RESTING	, bondsense		, 0		, SCMD_BONDSENSE	, 0  },
  { "brand"		, "bra"		, POS_STANDING	, do_brand		, 20		, 0			, 0  },
  { "breakshield"	, "bre"		, POS_RESTING	, do_breakshield	, 0		, 0			, 0  },
  { "buy"		, "buy"		, POS_RESTING	, do_not_here		, 0		, 0			, 0  },
  { "bug"		, "bug"		, POS_DEAD	, do_gen_write		, 0		, SCMD_BUG		, 0  },
  { "bugcheck"		, "bugc"	, POS_DEAD	, do_bugcheck		, LVL_GRGOD	, 0			, 0  },

  { "camouflage"	, "camo"	, POS_STANDING	, do_camo		, 20		, 0			, 0  },
  { "camp"		, "camp"	, POS_RESTING	, do_camp		, 20		, 0			, 0  },
  { "cart"		, "car"		, POS_STANDING	, do_gencart		, 0		, 0			, 0  },
  { "cemote"		, "cem"		, POS_RESTING	, do_cemote		, 0		, 0			, 0  },
  { "charge"		, "charg"	, POS_STANDING	, do_charge		, 20		, 0			, 0  },
  { "charm"		, "charm"	, POS_STANDING	, do_charm		, 20		, 0			, 0  },
  { "check"		, "chec"	, POS_RESTING	, do_check		, 1		, 0			, 0  },
  { "chess"		, "ches"	, POS_RESTING	, do_chess		, 0		, 0			, 0  },
  { "clear"		, "cl"		, POS_DEAD	, do_gen_ps		, 0		, SCMD_CLEAR		, 0  },
  { "clearaff"		, "cleara"	, POS_DEAD	, do_clearaff		, LVL_GRGOD	, 0			, 0  },
  { "clearbond"		, "clearb"	, POS_DEAD	, do_clearbond		, LVL_IMMORT	, 0			, 0  },
  { "cleartells"	, "cleart"	, POS_DEAD	, do_cleartells		, 0		, 0			, 0  },
  { "clone"		, "clon"	, POS_DEAD	, do_clone		, LVL_IMMORT	, 0			, 0  },
  { "close"		, "clos"	, POS_SITTING	, do_gen_door		, 0		, SCMD_CLOSE		, 0  },
  { "cls"		, "cls"		, POS_DEAD 	, do_gen_ps		, 0		, SCMD_CLEAR		, 0  },
  { "clubs"		, "clu"		, POS_DEAD	, do_clubs		, 0		, 0			, 0  },
  { "codetalk"		, "cod"		, POS_RESTING	, do_darksay		, 0		, 0			, 0  },
  { "consider"		, "cons"	, POS_RESTING	, do_consider		, 0		, 0			, 0  },
  { "contactinfo"	, "cont"	, POS_DEAD	, do_contactinfo	, 0		, 0			, 0  },
  { "color"		, "col"		, POS_DEAD	, do_color		, 0		, 0			, 0  },
  { "commands"		, "comm"	, POS_DEAD	, do_commands		, 0		, SCMD_COMMANDS		, 0  },
  { "compare"		, "comp"	, POS_RESTING	, do_compare		, 0		, 0			, 0  },
  { "concentrate"	, "con"		, POS_RESTING	, do_concentrate	, 0		, 0			, 0  },
  { "craft"		, "cra"		, POS_SITTING	, do_craft		, 0		, 0			, 0  },
  { "credits"		, "cre"		, POS_DEAD	, do_gen_ps		, 0		, SCMD_CREDITS		, 0  },
  { "cretodo"		, "cret"	, POS_DEAD	, do_gen_write		, 211		, SCMD_CRETODO		, CG_ADMIN  },

  { "darkdream"		, "dar"		, POS_SLEEPING	, do_darkdream		, 0		, 0			, 0  },
  { "date"		, "dat"		, POS_DEAD	, do_date		, 0		, SCMD_DATE		, 0  },
  { "dazzle"		, "daz"		, POS_STANDING	, do_dazzle		, 20		, 0			, 0  },
  { "dc"		, "dc"		, POS_DEAD	, do_dc			, LVL_IMMORT	, 0			, 0  },
  { "deguild"		, "deg"		, POS_DEAD	, do_deguild		, 0		, 0			, 0  },
  { "description"	, "des"		, POS_DEAD	, do_desc		, 0		, 0			, 0  },
  { "deposit"		, "dep"		, POS_STANDING	, do_not_here		, 1		, 0			, 0  },
  { "dgstat"		, "dgs"		, POS_DEAD	, do_dgstat		, LVL_IMMORT	, 0			, CG_DGS  },
  { "diagnose"		, "dia"		, POS_RESTING	, do_diagnose		, 0		, 0			, 0  },
  { "dice"		, "dic"		, POS_RESTING	, do_dice		, 0		, 0			, 0  },
  { "dig"		, "dig"		, POS_DEAD	, do_dig		, LVL_IMMORT	, 0			, CG_GL_BUILDER | CG_BUILDER  },
  { "dirt kick"		, "dit"		, POS_FIGHTING	, do_dirtkick		, 1		, 0			, 0  },
  { "disarm"		, "disa"	, POS_FIGHTING	, do_disarm		, 1		, 0			, 0  },
  { "disengage"		, "dise"	, POS_FIGHTING	, do_disengage		, 1		, 0			, 0  },
  { "disguise"		, "disg"	, POS_RESTING	, do_newdisguise	, 0		, 0			, 0  },
  { "display"		, "disp"	, POS_DEAD	, do_display		, 0		, 0			, 0  },
  { "dns"		, "dns"		, POS_DEAD	, do_dns		, LVL_GRGOD	, 0			, 0  },
  { "doing"		, "doi"		, POS_DEAD	, do_doing		, 0		, 0			, 0  },
  { "donate"		, "don"		, POS_RESTING	, do_drop		, 0		, SCMD_DONATE		, 0  },
  { "dontpull"		, "dont"	, POS_RESTING	, dont_pull		, 0		, 0			, 0  },
  { "drag"		, "dra"		, POS_STANDING	, do_drag		, 0		, 0			, 0  },
  { "dream"		, "dre"		, POS_SLEEPING	, do_dream		, 0		, 0			, 0  },
  { "dreamenter"	, "dreame"	, POS_SLEEPING	, do_dreamenter		, 0		, 0			, 0  },
  { "dreamgoto"		, "dreamg"	, POS_RESTING	, do_dreamgoto		, 0		, 0			, 0  },
  { "dreamlist"		, "dreaml"	, POS_RESTING	, do_dreamlist		, 0		, 0			, 0  },
  { "dreampull"		, "dreamp"	, POS_RESTING	, do_dreampull		, 0		, 0			, 0  },
  { "drink"		, "dri"		, POS_RESTING	, do_drink		, 0		, SCMD_DRINK		, 0  },
  { "dswitch"		, "dsw"		, POS_RESTING	, do_dswitch		, 0		, 0			, 0  },
  { "delete"		, "delete"	, POS_DEAD	, do_delete		, 0		, 0			, 0  },

  { "eat"		, "eat"		, POS_RESTING	, do_eat		, 0		, SCMD_EAT		, 0  },
  { "earth"		, "ear"		, POS_DEAD	, do_not_here		, 1		, 0			, 0  },
  { "eavesdrop"		, "eav"		, POS_STANDING	, do_eavesdrop		, 20		, 0			, 0  },
  { "echo"		, "ech"		, POS_SLEEPING	, do_echo		, LVL_IMMORT	, SCMD_ECHO		, 0  },
  { "edit"		, "ed"		, POS_DEAD	, do_edit		, 0		, 0			, 0  },
  { "emote"		, "emo"		, POS_RESTING	, do_echo		, 1		, SCMD_EMOTE		, 0  },
  { ":"			, ":"		, POS_RESTING	, do_echo		, 1		, SCMD_EMOTE		, 0  },
  { "employ"		, "emp"		, POS_RESTING	, do_employ		, 20		, 0			, 0  },
  { "enhance"		, "enh"		, POS_RESTING	, do_enhance		, 0		, 0			, 0  },
  { "enter"		, "ent"		, POS_STANDING	, do_enter		, 0		, 0			, 0  },
  { "eta"		, "eta"		, POS_RESTING	, do_not_here		, 0		, 0			, 0  },
  { "equipment"		, "eq"		, POS_SLEEPING	, do_equipment		, 0		, 0			, 0  },
  { "exits"		, "ex"		, POS_RESTING	, do_exits		, 0		, 0			, 0  },
  { "examine"		, "examine"	, POS_RESTING	, do_examine		, 0		, 0			, 0  },

  {  "forage"   ,  "fora"   ,  POS_STANDING,  do_forage   ,  0,  0,  0  },
  {  "force"    ,  "forc"    ,  POS_SLEEPING,  do_force    ,  LVL_IMMORT,  0,  0  },
  {  "fervor"   ,  "ferv"   ,  POS_RESTING ,  do_fervor   , 20,  0,  0  },
  {  "fill"     ,  "fi"     ,  POS_STANDING,  do_pour     ,  0,  SCMD_FILL,  0  },
  {  "fire"     ,  "fir"     ,  POS_DEAD    ,  do_fire     ,  1,  0,  0  },
  {  "findherb" ,  "fin" ,  POS_RESTING ,  do_findherb ,  0,  0,  0  },
  {  "fine"     ,  "fine"     ,  POS_STANDING,  do_fine     , 20,  0,  0  },
  {  "flank"    ,  "flan"    ,  POS_STANDING,  do_flank    ,  0,  0,  0  },
  {  "flee"     ,  "fl"     ,  POS_FIGHTING,  do_flee     ,  1,  SCMD_FLEE,  0  },
  {  "follow"   ,  "fol"   ,  POS_RESTING ,  do_follow   ,  0,  0,  0  },
  {  "forget"   ,  "for"   ,  POS_RESTING ,  do_forget   ,  0,  0,  0  },
  {  "fortify"  ,  "fort"  ,  POS_STANDING,  do_fortify  ,  0,  0,  0  },
  {  "free"     ,  "free"     ,  POS_RESTING ,  do_free     ,  0 , 0,  0  },
  {  "freeze"   ,  "freez"   ,  POS_DEAD    ,  do_wizutil  ,  LVL_IMMORT,  SCMD_FREEZE,  0  },
  {  "fx"       ,  "fx"       ,  POS_RESTING ,  do_fx       ,  0,  SCMD_FX,  0  },

  {  "gain"     ,  "gai"     ,  POS_DEAD    ,  do_not_here ,  1,  0,  0  },
  {  "gadmininfo",  "gad",  POS_DEAD   ,  do_gadmininfo,  0,  0,  0  },
  {  "gdeposit" ,  "gdep" ,  POS_DEAD    ,  do_gdeposit ,  0,  0,  0  },
  {  "gdesc"    ,  "gdes"    ,  POS_DEAD    ,  do_gdesc    ,  0,  0,  0  },
  {  "gdt"      ,  "gdt"      ,  POS_DEAD    ,  do_gdt      ,  0,  0,  0  },
  {  "get"      	, "ge"			, POS_RESTING	,  do_get      ,  0,  0,  0  },
  {  "gecho"    ,  "gecho"    ,  POS_DEAD    ,  do_gecho    ,  LVL_IMMORT,  0,  0  },
  {  "ghelp"    ,  "ghe"    ,  POS_DEAD    ,  do_newghelp    ,  0,  0,  0  },
  {  "give"     ,  "gi"     ,  POS_RESTING ,  do_give     ,  0,  0,  0  },
  {  "glist"    ,  "gli"    ,  POS_RESTING ,  do_glist    ,  0,  0,  0  },
  {  "gload"    ,  "glo"    ,  POS_RESTING ,  do_gload    ,  0,  0,  0  },
  {  "gmail"    ,  "gma"    ,  POS_RESTING ,  do_not_here ,  1,  0,  0  },
  {  "gossip"   ,  "gos"   ,  POS_RESTING ,  do_gossip   ,  1,  0,  0  },
  {  "goto"     ,  "got"     ,  POS_SLEEPING,  do_goto     ,  LVL_IMMORT,  0,  0  },
  {  "gold"     ,  "gol"     ,  POS_RESTING ,  do_gold     ,  0,  0,  0  },
  {  "group"    ,  "gr"     ,  POS_RESTING ,  do_group    ,  1,  0,  0  },
  {  "grasp"    ,  "gra"    ,  POS_RESTING ,  do_grasp    ,  0,  0,  0  },
  {  "grab"     ,  "grab"     ,  POS_RESTING ,  do_grab     ,  0,  0,  0  },
  {  "greceive" ,  "grec" ,  POS_DEAD    ,  do_greceive ,  0,  0,  0  },
  {  "greq"     ,  "greq"     ,  POS_DEAD    ,  do_greq     ,  0,  0,  0  },
  {  "gsay"     ,  "gsay"     ,  POS_SLEEPING,  do_gsay     ,  0,  0,  0  },
  {  "gskillset",  "gski",  POS_RESTING ,  do_gskillset,  0,  0,  0  },
  {  "gtell"    ,  "gtel"    ,  POS_SLEEPING,  do_gsay     ,  0,  0,  0  },
  {  "guild"    ,  "guild"    ,  POS_DEAD    ,  do_guild    ,  0,  0,  0  },
  {  "guilddisguise",  "guildd",  POS_RESTING,  do_guilddisguise,  0,  0,  0  },
  {  "guildinfo",  "guildi",  POS_DEAD    ,  do_guildinfo,  0,  0,  0 },
  {  "guildmail",  "guildm",  POS_RESTING ,  do_mail,  0,  SCMD_MAIL_GUILD,  0 },
  {  "guildset" ,  "guilds" ,  POS_DEAD    ,  do_guildset ,  0,  0,  0  },
  {  "guildieinfo",  "guildiei",  POS_DEAD  ,  do_guildieinfo,  0,  0,  0  },
  {  "guildieset",  "guildies",  POS_DEAD   ,  do_guildieset,  0,  0,  0  },
  {  "guildinv" ,  "guildi" ,  POS_DEAD    ,  do_ginvis,  LVL_IMMORT,  0,  CG_GL },
  {  "gwithdraw",  "gwith",  POS_DEAD    ,  do_gwithdraw,  0,  0,  0  },
  {  "help"     ,  "he"     ,  POS_DEAD    ,  do_help     ,  0,  0,  0  },
  {  "handtalk" ,  "hand" ,  POS_RESTING ,  do_handtalk ,  0,  0,  0  },
  {  "handbook" ,  "handb" ,  POS_DEAD ,  do_gen_ps ,  200,  SCMD_HANDBOOK,  0  },
  {  "heal"     ,  "hea"     ,  POS_RESTING ,  do_not_here ,  1,  0,  0  },
  {  "hedit"    ,  "hed"    ,  POS_DEAD    ,  do_olc      ,  LVL_IMMORT,  SCMD_OLC_HEDIT,  0  },
  {  "heditutil",  "heditu",  POS_DEAD    ,  do_heditutil,  LVL_IMMORT,  0,  0  },
  { "helpcheck"	, "helpc"	,POS_DEAD	, do_helpcheck	, LVL_IMMORT	,0	,0 },
  {  "herbident",  "herbi",  POS_RESTING ,  do_herbident,  1,  0,  0  },
  {  "herblore" ,  "herbl" ,  POS_RESTING ,  do_herblore ,  1,  0,  0  },
  {  "herbmastery",  "herbm", POS_RESTING,  do_herbmastery, 20, 0,  0 },
  {  "hide"     ,  "hid"     ,  POS_RESTING ,  do_hide     ,  1,  0,  0  },
  {  "hidetracks",  "hidet",  POS_STANDING,  do_hide_tracks, 0, 0,  0  },
  { "hindex"	, "hin"	, POS_DEAD,	do_hindex,	LVL_IMMORT,	0,	0 },
  {  "hire"     ,  "hir"     ,  POS_RESTING ,  do_hire     ,  1,  0,  0  },
  {  "hit"      ,  "hit"      ,  POS_FIGHTING,  do_hit      ,  0,  SCMD_HIT,  0  },
  {  "hitp"     ,  "hitp"     ,  POS_RESTING ,  do_not_here ,  0,  0,  0  },
  {  "hlist"    ,  "hli"    ,  POS_DEAD    ,  do_hlist    ,  0,  0,  0  },
  {  "hold"     ,  "hol"     ,  POS_RESTING ,  do_grab     ,  1,  0,  0  },
  {  "home"     ,  "hom"     ,  POS_RESTING ,  do_home     ,  0,  0,  0  },
//  {  "horses"   ,  "hors"   ,  POS_STANDING,  do_not_here ,  0,  0,  0  },
  {  "hunt"     ,  "hun"     ,  POS_STANDING,  do_hunt     ,  0,  0,  0  },

  {  "idea"     ,  "idea"     ,  POS_DEAD    ,  do_gen_write,  0,  SCMD_IDEA,  0  },
/*  {  "ignorelist",  "ignore", POS_DEAD    ,  do_ignorelist, 0,  0,  0  }, */
  {  "imagine"  ,  "imag"  ,  POS_RESTING ,  do_imagine  ,  0,  0,  0  },
  {  "immlock"  ,  "imml"  ,  POS_DEAD    ,  do_immlock  ,  LVL_GRGOD,  0,  0  },
  {  "imotd"    ,  "imotd"    ,  POS_DEAD    ,  do_gen_ps   ,  LVL_IMMORT,  SCMD_IMOTD,  0  },
  {  "infravision",  "infra",  POS_RESTING,  do_infravision,  0,  0,  0  },
  {  "info"     ,  "info"     ,  POS_RESTING ,  do_info     ,  0,  0,  0  },
  {  "inform"   ,  "infor"   ,  POS_RESTING ,  do_inform   ,  0,  0,  0  },
  {  "\""       ,  "\""       ,  POS_SLEEPING,  do_inform   ,  0,  0,  0  },
  {  "inquisition",  "inq", POS_STANDING,  do_inquisition,  0,  0,  0  },
  {  "inspire"  ,  "insp"  ,  POS_STANDING,  do_inspire  ,  0,  0,  0  },
  {  "instruct" ,  "inst" ,  POS_RESTING ,  do_instruct ,  0,  0,  0  },
  {  "insult"   ,  "insu"   ,  POS_RESTING ,  do_insult   ,  0,  0,  0  },
  {  "invis"    ,  "invi"    ,  POS_DEAD    ,  do_invis    ,  LVL_IMMORT,  0,  0  },
  {  "isic"     ,  "isic"     ,  POS_DEAD    ,  do_isic     ,  0,  0,  0  },
  {  "isquest"  ,  "isqu"  ,  POS_DEAD    ,  do_isquest  ,  LVL_IMMORT,  0,  0  },
  {  "markedmap",  "mar",  POS_STANDING,  do_markedmap,  20,  0,  0  },
  {  "multiplay",  "mult",  POS_DEAD    ,  do_multi    ,  LVL_IMMORT,  0,  0  },
  {  "multilist",  "multi",  POS_DEAD    ,  do_multilist    ,  LVL_IMMORT,  0,  0  },
  {  "mycolor"  ,  "myc"  ,  POS_DEAD    ,  do_mycolor  ,  0,  0,  0  },
  {  "authcheck",  "authc",  POS_DEAD    ,  do_authcheck,  LVL_IMMORT,  0,  0 },
  {  "join"     ,  "joi"     ,  POS_STANDING,  do_not_here ,  0,  0,  0  },
  {  "junk"     ,  "jun"     ,  POS_RESTING ,  do_drop     ,  0,  SCMD_JUNK,  0  },
  {  "kick"     ,  "kic"     ,  POS_FIGHTING,  do_kick     ,  1,  0,  0  },
  {  "knifetoss",  "kni",  POS_STANDING,  do_knifetoss,  20,  0,  0  },
  {  "knock"    ,  "kno"    ,  POS_RESTING ,  do_knock    ,  0,  0,  0  },
  {  "know health",  "know", POS_SITTING,  do_know_health, 20, 0,  0 },
  {  "knowledge",  "knowl",  POS_RESTING ,  do_knowledge,  20,  0,  0  },
  {  "locate"   ,  "loc"   ,  POS_STANDING,  do_locate   ,  0,  0,  0  },
  {  "language" ,  "lan" ,  POS_SLEEPING,  do_language ,  0,  0,  0  },
  {  "last"     ,  "las"     ,  POS_DEAD    ,  do_last     ,  LVL_IMMORT,  0,  0  },
  {  "lastemote",  "laste",  POS_RESTING ,  do_lastemote,  1,  0,  0  },
  {  "learn"    ,  "lea"    ,  POS_STANDING,  do_learn    ,  0,  0,  0  },
  {  "leave"    ,  "leav"    ,  POS_STANDING,  do_leave    ,  0,  0,  0  },
  {  "lines"    ,  "lin"    ,  POS_DEAD    ,  do_lines    ,  0,  0,  0  },
  {  "list"     ,  "lis"     ,  POS_RESTING ,  do_list ,  0,  0,  0  },
  {  "lipread"  ,  "lip"  ,  POS_RESTING , do_lipreading,  0,  0,  0  },
  {  "lock"     ,  "loc"     ,  POS_SITTING ,  do_gen_door ,  0,  SCMD_LOCK,  0  },
  {  "load"     ,  "loa"     ,  POS_DEAD    ,  do_load     ,  LVL_IMMORT,  0,  0  },
  {  "log"      ,  "log",  POS_DEAD    ,  do_log,  LVL_GOD,  0,  0  },
  {  "logsearch",  "logs",  POS_DEAD    ,  do_logsearch,  LVL_GRGOD,  SCMD_LOGSEARCH,  0  },
  {  "longsleep",  "lon",  POS_SLEEPING, do_longsleep ,  0,  0,  0  },
  {  "lweapon"  ,  "lwe"  ,  POS_RESTING ,  do_lweapon  ,  0,  0,  0  },
  {  "mana"     ,  "man"     ,  POS_RESTING ,  do_not_here ,  0,  0,  0  },
  {  "manaconv" ,  "manac" ,  POS_RESTING ,  do_manaconv ,  0,  0,  0  },
  {  "mask"     ,  "mas"     ,  POS_RESTING ,  do_mask     ,  0,  0,  0  },
//  {  "mount"    ,  "mou"    ,  POS_STANDING,  do_gen_mount,  0,  SCMD_MOUNT,  0  },
//  {  "mountpet" ,  "mountpet" ,  POS_STANDING,  do_gen_mount,  0,  SCMD_PET,  0  },
//  {  "mounttake",  "mounttake",  POS_STANDING,  do_gen_mount,  0,  SCMD_TAKE,  0  },
  {  "mdt"      ,  "mdt"      ,  POS_SLEEPING,  do_mdt      ,  0,  0,  0  },
  {  "mediate"  ,  "media"  ,  POS_RESTING ,  do_mediate  ,  0,  0,  0  },
  {  "medicine" ,  "medic" ,  POS_RESTING ,  do_medicine ,  0,  0,  0  },
  {  "medit"    ,  "medit"    ,  POS_DEAD    ,  do_olc      ,  LVL_CARPENTER,  SCMD_OLC_MEDIT,  CG_GL_BUILDER | CG_BUILDER },
  {  "morale"   ,  "mor"   ,  POS_STANDING,  do_morale   ,  0,  0,  0  },
  {  "motd"     ,  "mot"     ,  POS_DEAD    ,  do_gen_ps   ,  0,  SCMD_MOTD,  0  },
  {  "move"     ,  "mov"     ,  POS_RESTING ,  do_not_here ,  0,  0,  0  },
  {  "mail"     ,  "mai"     ,  POS_RESTING ,  do_mail     ,  1,  0,  0  },
  {  "mmail"    ,  "mma"     ,  POS_STANDING,  do_not_here ,  1,  0,  0  },
  {  "msave"    ,  "msave"   ,  POS_DEAD    ,  save_msgs   ,  LVL_IMMORT, 0, 0 },    
  {  "msgedit"  ,  "msgedit" ,  POS_DEAD    ,  do_msgedit  ,  LVL_IMMORT, 0, CG_BUILDER|CG_ADMIN|CG_HB|CG_OWNER|CG_WEAVER|CG_HCODER },
  {  "mute"     ,  "mut"     ,  POS_DEAD    ,  do_wizutil  ,  LVL_IMMORT,  SCMD_SQUELCH,  0  },
  {  "murder"   ,  "mur"     ,  POS_FIGHTING,  do_hit      ,  0,  SCMD_MURDER,  0  },
  {  "myguilds" ,  "myg" ,  POS_DEAD    ,  do_myguilds ,  0,  0,  0  },
  {  "navigate" ,  "nav" ,  POS_STANDING,  do_navigate ,  20,  0,  0  },
  {  "news"     ,  "new"     ,  POS_SLEEPING,  do_gen_ps   ,  0,  SCMD_NEWS,  0  },
  {  "newbiegoto",  "newb", POS_STANDING, do_newbiegoto,  0,  0,  0  },
  {  "nopray", "nop", POS_DEAD	, do_wizutil, LVL_IMMORT, SCMD_NOPRAY, 0 },
  {  "nosnoop"  ,  "nos"  ,  POS_DEAD    ,  do_nosnoop  ,  LVL_IMMORT,  0,  0  },
  {  "notes"    ,  "not"    ,  POS_DEAD    ,  do_notes    ,  0,  0,  0 },
  {  "notitle"  ,  "notit"  ,  POS_DEAD    ,  do_wizutil  ,  LVL_IMMORT,  SCMD_NOTITLE,  0  },
  {  "notifylist",  "notif", POS_DEAD    ,  do_notifylist, 0,  0,  0  },
  {  "nogecho"  ,  "nog"  ,  POS_DEAD    ,  do_wizutil  ,  LVL_GRGOD,  SCMD_NOGECHO ,  0 },

  { "obscene"  , "obs"  , POS_DEAD , do_gen_comm , 1, SCMD_OBSCENE, 0 },
  {  "obscure"  ,  "obscu"  ,  POS_SLEEPING,  do_obscure  ,  20,  0,  0 },
  { "oedit"  , "oed"  , POS_DEAD , do_olc , LVL_IMMORT, SCMD_OLC_OEDIT, CG_GL_BUILDER | CG_BUILDER },
  { "oldnews"  , "old"  , POS_SLEEPING, do_gen_ps , 0, SCMD_OLDNEWS, 0 },
  { "order"  , "ord"  , POS_RESTING , do_order , 1, 0, 0 },
  {  "offend"   ,  "off"   ,  POS_STANDING,  do_offend   ,  0,  0,  0  },
  {  "open"     ,  "op"     ,  POS_SITTING ,  do_gen_door ,  0,  SCMD_OPEN,  0  },
  {  "olc"      ,  "ol"      ,  POS_DEAD    ,  do_olc      ,  LVL_IMMORT,  SCMD_OLC_SAVEINFO,  CG_GL_BUILDER | CG_BUILDER  },
  {  "osay"     ,  "os"     ,  POS_DEAD    ,  do_osay     ,  0,  0,  0  },
  {  ","        ,  "os"     ,  POS_DEAD    ,  do_osay     ,  0,  0, 0 },
  {  "password" ,  "pas" ,  POS_DEAD    ,  do_passwd ,  0,  0,  0  },
  {  "page"     ,  "pag"     ,  POS_DEAD    ,  do_page     ,  	LVL_IMMORT,  0,  0  },
  {  "passbond" ,  "passb" ,  POS_DEAD    ,  do_passbond ,  LVL_IMMORT,  0,  0  },
  {  "pedit"    ,  "ped"    ,  POS_DEAD    ,  do_olc      ,  LVL_GRGOD,  SCMD_OLC_PEDIT,  0  },
  {  "peditutil",  "peditu",  POS_DEAD    ,  do_peditutil,  LVL_GRGOD,  0,  0  },
  {  "pemote"   ,  "pe"   ,  POS_RESTING ,  do_pemote   ,  0,  0,  0  },
  {  "pick"     ,  "pic"     ,  POS_STANDING,  do_gen_door ,  1,  SCMD_PICK,  0  },
  {  "pigeon"   ,  "pig"   ,  POS_RESTING ,  do_not_here ,  0,  0,  0  },
  {  "play"     ,  "pla"     ,  POS_SITTING ,  do_play     ,  0,  0,  0  },
  {  "playerlist",  "playe", POS_DEAD    ,  do_playerlist, LVL_IMMORT,  0,  0  },
  {  "plist"    ,  "pli"    ,  POS_DEAD    ,  do_plist    ,  0,  0,  0  },
  {  "policy"   ,  "pol"   ,  POS_DEAD    ,  do_newpolicy ,  0,  0,  0  },
  {  "poofin"   ,  "poofi"   ,  POS_DEAD    ,  do_poofset  ,  LVL_IMMORT,  SCMD_POOFIN,  0  },
  {  "pose"     ,  "po"     ,  POS_RESTING ,  do_pose     ,  0,  0,  0  },
  {  "poofout"  ,  "poofo"  ,  POS_DEAD    ,  do_poofset  ,  LVL_IMMORT,  SCMD_POOFOUT,  0  },
  {  "pound"    ,  "poun"    ,  POS_FIGHTING,  do_pound    ,  0,  0,  0  },
  {  "pour"     ,  "pour"     ,  POS_STANDING,  do_pour     ,  0,  SCMD_POUR,  0  },
  {  "powertrap",  "powe",  POS_RESTING ,  do_powertrap,  0,  0,  0  },
  {  "practice" ,  "pra" ,  POS_RESTING ,  do_practice ,  1,  0,  0  },
  {  "prefix"   ,  "pre"   ,  POS_DEAD    ,  do_prefix   ,  0,  0,  0  },
  {  "prompt"   ,  "pro"   ,  POS_DEAD    ,  do_prompt   ,  0,  0,  0  },
  {  "pray"     ,  "pray"     ,  POS_SITTING ,  do_pray   ,  0,  0,  0  },
  {  "preach"     ,  "prea"     ,  POS_STANDING ,  do_preach     ,  20,  0,  0  },
  {  "press"    ,  "pres"    ,  POS_SITTING ,  do_not_here ,  0,  0,  0  },
  {  "privacy"  ,  "privac"  ,  POS_DEAD    ,  do_privacy  ,  0,  0,  0  },
  {  "private"  ,  "privat"  ,  POS_DEAD    ,  do_gen_comm ,  0,  SCMD_PRIVATE,  0  },
  {  "psay"     ,  "ps"     ,  POS_DEAD    ,  do_gen_comm ,  0,  SCMD_PRIVATE,  0  },
  {  "."        ,  "."        ,  POS_DEAD    ,  do_gen_comm ,  0,  SCMD_PRIVATE,  0  },
  {  "padd"     ,  "pad"     ,  POS_DEAD    ,  do_private_channel ,  0,  SCMD_P_ADD,  0  },
  {  "pclose"   ,  "pcl"   ,  POS_DEAD    ,  do_private_channel ,  0,  SCMD_P_CLOSE,  0  },
  {  "phelp"    ,  "phe"    ,  POS_DEAD    ,  do_private_channel ,  0,  SCMD_P_HELP,  0  },
  {  "poff"     ,  "pof"     ,  POS_DEAD    ,  do_private_channel ,  0,  SCMD_P_OFF,  0  },
  {  "popen"    ,  "pop"    ,  POS_DEAD    ,  do_private_channel ,  0,  SCMD_P_OPEN,  0  },
  {  "premove"  ,  "pre"  ,  POS_DEAD    ,  do_private_channel ,  0,  SCMD_P_REMOVE,  0  },
  {  "pwho"     ,  "pwh"     ,  POS_DEAD    ,  do_private_channel ,  0,  SCMD_P_WHO,  0  },
  {  "pcheck"   ,  "pch"   ,  POS_DEAD    ,  do_private_channel ,  LVL_GRGOD,  SCMD_P_CHECK,  0  },
  {  "protege"  ,  "pro"  ,  POS_STANDING,  do_prodigy  ,  0,  0,  0  },
  {  "ptest"    ,  "pte"    ,  POS_DEAD    ,  do_pstest   ,  LVL_GRGOD,  0,  CG_WEAVER  },
  {  "pull"     ,  "pul"     ,  POS_STANDING,  do_pull     ,  0,  0,  0  },
  {  "punch"    ,  "pun"    ,  POS_FIGHTING,  do_punch    ,  0,  0,  0  },
  {  "purge"    ,  "pur"    ,  POS_DEAD    ,  do_purge    ,  LVL_IMMORT,  0,  0  },
  {  "push"     ,  "pus"     ,  POS_STANDING,  do_push     ,  0,  0,  0  },
  {  "quaff"    ,  "qua"    ,  POS_RESTING ,  do_use      ,  0,  SCMD_QUAFF,  0  },
  {  "qcount"   ,  "qco"   ,  POS_DEAD    ,  do_qcount   ,  LVL_IMMORT,  0,  0  },
  {  "qecho"    ,  "qe"    ,  POS_DEAD    ,  do_qcomm    ,  LVL_IMMORT,  SCMD_QECHO,  0  },
  {  "qlist"    ,  "ql"    ,  POS_DEAD    ,  do_qlist    ,  LVL_IMMORT,  0,  0  },
  {  "question" ,  "que" ,  POS_DEAD    ,  do_gen_comm ,  0,  SCMD_QUESTION,  0  },
  {  "qui"      ,  "qui"      ,  POS_DEAD    ,  do_quit     ,  0,  0,  0  },
  {  "quit"     ,  "quit"     ,  POS_DEAD    ,  do_quit     ,  0,  SCMD_QUIT,  0  },
  {  "qsay"     ,  "qs"     ,  POS_DEAD    ,  do_qcomm    ,  0,  SCMD_QSAY,  0  },
  {  "questlock",  "qu",  POS_DEAD    ,  do_questlock , LVL_IMMORT, 0,  0  },
  {  "rage"     ,  "ra"     ,  POS_FIGHTING,  do_rage	   ,  0,  0,  0  },
  {  "rally"    ,  "ral"    ,  POS_RESTING ,  do_call     ,  0,  0,  0  },
  {  "refresh"  ,  "ref"  ,  POS_DEAD    ,  do_refresh  ,  LVL_IMMORT,  0,  0  },
  {  "reply"    ,  "re"    ,  POS_SLEEPING,  do_reply    ,  0,  0,  0  },
  {  "replylock",  "replyl",  POS_SLEEPING,  do_replylock,  0,  0,  0  },
  {  "rest"     ,  "res"     ,  POS_RESTING ,  do_rest     ,  0,  0,  0  },
  {  "respond"  ,  "resp", POS_STANDING, do_not_here, 0, 0, 0 },
  {  "revive"   ,  "rev"   ,  POS_DEAD    ,  do_revive   ,  LVL_IMMORT,  0,  0  },
  {  "ranks"    ,  "ran"    ,  POS_DEAD    ,  do_granks    ,  0,  0,  0  },
  {  "rcopy"    ,  "rco"    ,  POS_DEAD    ,  do_rcopy    ,  LVL_GOD,  0,  CG_GL_BUILDER | CG_BUILDER },
  {  "recall"   ,  "rec"   ,  POS_RESTING ,  do_recall   ,  0,  0,  0  },
  {  "recruit"  ,  "recr"  ,  POS_RESTING ,  do_recruit  , 20,  0,  0  },
  {  "reload"   ,  "rel"   ,  POS_DEAD    ,  do_reboot   ,  LVL_IMMORT,  0,  CG_ADMIN | CG_WEBMASTER  },
  {  "recite"   ,  "reci"   ,  POS_RESTING ,  do_use      ,  0,  SCMD_RECITE,  0  },
  {  "receive"  ,  "rece"  ,  POS_RESTING ,  do_receive  ,  1,  0,  0  },
  {  "reinforce",  "rei",  POS_STANDING,  do_reinforce,  1,  0,  0  },
  {  "release"  ,  "rel"  ,  POS_RESTING ,  do_release  ,  0,  0,  0  },
  {  "remove"   ,  "rem"   ,  POS_RESTING ,  do_remove   ,  0,  0,  0  },
  {  "remguild" ,  "remg" ,  POS_DEAD    ,  do_newremguild ,  LVL_IMPL,  0,  0  },
  {  "repair"   ,  "rep"   ,  POS_RESTING ,  do_repair   ,  1,  0,  0  },
  {  "report"   ,  "repo"   ,  POS_RESTING ,  do_report   ,  0,  0,  0  },
  {  "oreport"  ,  "orep"  ,  POS_RESTING ,  do_oreport  ,  0,  0,  0  },
  {  "rescue"   ,  "resc"   ,  POS_FIGHTING,  do_rescue   ,  1,  0,  0  },
  {  "restore"  ,  "resto"  ,  POS_DEAD    ,  do_restore  ,  LVL_IMMORT,  0,  0  },
  {  "retainer" ,  "reta" ,  POS_RESTING ,  do_retainer ,  1,  0,  0  },
  {  "retreat"  ,  "retr"  ,  POS_FIGHTING,  do_retreat  ,  0,  0,  0  },
  {  "return"   ,  "retu"   ,  POS_DEAD    ,  do_return   ,  0,  0,  0  },
  {  "redit"    ,  "red"    ,  POS_DEAD    ,  do_olc      ,  1,  SCMD_OLC_REDIT,  CG_GL_BUILDER | CG_BUILDER },
  {  "rlink"    ,  "rlin"    ,  POS_STANDING,  do_rlink    ,  LVL_IMMORT,  0,  CG_GL_BUILDER | CG_BUILDER  },
  {  "rlist"    ,  "rlis"    ,  POS_SLEEPING,  do_rlist    ,  0,  0,  0 },
  {  "roomset"  ,  "ro"  ,  POS_DEAD    ,  do_roomset  ,  LVL_IMMORT,  0,  0  },
  {  "rplog"    ,  "rp"    ,  POS_DEAD    ,  do_rplog    ,  0,  0,  0  },
  {  "run"      ,  "ru"      ,  POS_STANDING,  do_run      ,  0,  0,  0  },
  {  "rumor"    ,  "rum"    ,  POS_RESTING ,  do_rumor    ,  0,  0,  0  },
  {  "saltbrothers",  "sal",  POS_STANDING,  do_saltbrothers,  0,  0,  0  },
  {  "say"      ,  "sa"      ,  POS_RESTING ,  do_say      ,  0,  0,  0  },
  {  "'"        ,  "'"        ,  POS_RESTING ,  do_say      ,  0,  0,  0  },
  {  "sayto"    ,  "sayt"    ,  POS_RESTING ,  do_sayto    ,  0,  0,  0  },
  {  ">"        ,  ">"        ,  POS_RESTING ,  do_sayto    ,  0,  0,  0  },
  {  "sailtime" ,  "sai" ,  POS_RESTING ,  do_not_here ,  0,  0,  0  },
  {  "save"     ,  "sav"     ,  POS_SLEEPING,  do_save     ,  0,  0,  0  },
  {  "saveguild",  "saveg",  POS_DEAD,      do_saveguild,  0,  0,  0  },
  {  "scan"     ,  "sca"     ,  POS_RESTING ,  do_scan     ,  0,  0,  0  },
  {  "scene"    ,  "sce"     ,  POS_RESTING ,  do_scene     ,  0,  0,  0  },
  {  "search"   ,  "sea"   ,  POS_STANDING,  do_search   ,  0,  0,  0  },
  {  "seek"     ,  "see"     ,  POS_STANDING,  do_seek     ,  0,  0,  0  },
  {  "seekguild",  "seekg",  POS_DEAD    ,  do_seekguild,  0,  0,  0  },
  {  "seeking"  ,  "seeki"  ,  POS_DEAD,      do_seeking   ,  0,  0,  0  },
  {  "sell"     ,  "sel"     ,  POS_STANDING,  do_not_here ,  0,  0,  0  },
  {  "send"     ,  "sen"     ,  POS_SLEEPING,  do_send     ,  LVL_IMMORT,  0,  0  },
  {  "set"      ,  "set"      ,  POS_DEAD    ,  do_set      ,  LVL_IMMORT,  0,  0  },
  {  "sedit"    ,  "sed"    ,  POS_DEAD    ,  do_olc      ,  1,  SCMD_OLC_SEDIT,  CG_GL_BUILDER | CG_BUILDER },
  {  "sever"    ,  "sev"    ,  POS_STANDING,  do_sever    ,  0,  0,  0  },
  {  "sfx"      ,  "sfx"      ,  POS_RESTING ,  do_fx	   ,  0,  SCMD_SFX,  0 },
  {  "shortgate",  "shor",  POS_STANDING,  do_shortgate,  0,  0,  0  },
  {  "shout"    ,  "shou"    ,  POS_RESTING ,  do_gen_comm ,  0,  SCMD_SHOUT,  0  },
  {  "shadow"   ,  "shad"   ,  POS_STANDING,  do_inquisition,  0,  0,  0  },
  { "shieldwall", "shie",  POS_STANDING,  do_shieldwall,  0,  0,  0  },
  {  "show"     ,  "sho"     ,  POS_DEAD    ,  do_show     ,  1,  0,  CG_GL_BUILDER | CG_BUILDER  },
  {  "shoot"    ,  "shoo"    ,  POS_STANDING,  do_shoot    ,  0,  0,  0  },
  {  "showbground",  "showbg",  POS_DEAD  ,  do_showbground, 0, 0,  0  },
  {  "showbugs" ,  "showbu" ,  POS_DEAD    ,  do_gen_ps   ,  0,  SCMD_BUGS,  0  },
  {  "showideas",  "showi",  POS_DEAD    ,  do_gen_ps   ,  0,  SCMD_IDEAS,  0  },
  {  "showmail" ,  "showm" ,  POS_DEAD    ,  do_showmail ,  0,  0,  0  },
  {  "showsocialideas",  "showso",  POS_DEAD    ,  do_gen_ps   ,  0,  SCMD_SOCIALIDEAS,  0  },
  {  "showcretodo" ,  "showc" ,  POS_DEAD    ,  do_gen_ps   ,  LVL_GRGOD,  SCMD_CRETODOS,  CG_ADMIN  },
  {  "showtypos",  "showt",  POS_DEAD    ,  do_gen_ps   ,  0,  SCMD_TYPOS,  0  },
  {  "showtodo" ,  "showto" ,  POS_DEAD    ,  do_gen_ps   ,  LVL_IMMORT,  SCMD_TODOS,  CG_ADMIN  },
  {  "shutdow"  ,  "shut"  ,  POS_DEAD    ,  do_shutdown ,  LVL_IMMORT,  0,  CG_ADMIN  },
  {  "shutdown" ,  "shutdown" ,  POS_DEAD    ,  do_shutdown ,  LVL_IMMORT,  SCMD_SHUTDOWN,  CG_ADMIN  },
  {  "sing"     ,  "sin"     ,  POS_RESTING ,  do_gen_comm ,  0,  SCMD_SONG,  0  },
  {  "sip"      ,  "sip"      ,  POS_RESTING ,  do_drink    ,  0,  SCMD_SIP,  0  },
  {  "sit"      ,  "sit"      ,  POS_RESTING ,  do_sit      ,  0,  0,  0  },
  {  "skills"   ,  "skills"   ,  POS_SLEEPING,  do_skills   ,  0,  0,  0  },
  {  "skillset" ,  "skillset" ,  POS_SLEEPING,  do_skillset ,  LVL_IMMORT,  0,  CG_GL  },
  {  "sleep"    ,  "sle"    ,  POS_SLEEPING,  do_sleep    ,  0,  0,  0  },
  {  "slowns"   ,  "slo"   ,  POS_DEAD    ,  do_gen_tog  ,  LVL_GRGOD,  SCMD_SLOWNS,  0  },
  {  "sneak"    ,  "sne"    ,  POS_STANDING,  do_sneak    ,  1,  0,  0  },
  {  "snoop"    ,  "sno"    ,  POS_DEAD    ,  do_snoop    ,  LVL_IMMORT,  0,  0  },
  {  "socialidea",  "sociali",  POS_DEAD    ,  do_gen_write,  0,  SCMD_SOCIALIDEA,  0  },
  {  "socials"  ,  "socials"  ,  POS_DEAD    ,  do_commands ,  0,  SCMD_SOCIALS,  0  },
  {  "split"    ,  "spl"    ,  POS_SITTING ,  do_split    ,  1,  0,  0  },
  {  "sponsor"  ,  "spo"  ,  POS_DEAD    ,  do_sponsor  , 20,  0,  0  },
  {  "sponsorlist",  "sponsorl",  POS_DEAD  ,  do_sponsorlist,  0,  0,  0  },
  {  "speedwalk",  "spee",  POS_STANDING,  do_speedwalk,  0,  0,  0  },
  {  "speardance",  "spea", POS_STANDING, do_speardance,  0,  0,  0  },
  {  "spirit"   ,  "spi"   ,  POS_DEAD    ,  do_not_here ,  1,  0,  0  },
  {  "stab"     ,  "stab"     ,  POS_FIGHTING,  do_stab     ,  0,  0,  0  },
  {  "stalk"    ,  "stal"    ,  POS_STANDING,  do_stalk    ,  0,  0,  0  },
  {  "stat"     ,  "stat"     ,  POS_DEAD    ,  do_stat     ,  1,  0,  CG_ST | CG_GL | CG_GL_BUILDER | CG_BUILDER | CG_GL  },
  {  "steal"    ,  "stea"    ,  POS_STANDING,  do_steal    ,  1,  0,  0  },
  {  "stun"     ,  "stu"     ,  POS_FIGHTING,  do_stun     ,  0,  0,  0  },
  {  "supply"  ,  "sup"  ,  POS_RESTING ,  do_supply   ,  0,  0,  0  },
  {  "subdue"   ,  "sub"   ,  POS_SLEEPING,  do_subdue   ,  0,  0,  0 },
  {  "summon"   ,  "sum"   ,  POS_STANDING,  do_summon   ,  0,  0,  0  },
  {  "survival" ,  "sur" ,  POS_STANDING,  do_survival ,  0,  0,  0  },
  {  "switch"   ,  "sw"   ,  POS_DEAD    ,  do_switch   ,  LVL_IMMORT,  0,  0  },
  {  "syslog"   ,  "sy"   ,  POS_DEAD    ,  do_syslog   ,  LVL_IMMORT,  0,  0  },
  {  "/"        ,  "/"        ,  POS_DEAD    ,  do_tell     ,  0,  0,  0  },
  {  "tells"    ,  "tells"    ,  POS_DEAD    ,  do_tells    ,  0,  0,  0  },
  {  "tedit"    ,  "ted"    ,  POS_DEAD    ,  do_tedit    ,  LVL_GOD,  0,  CG_BUILDER  },
  {  "tail"     ,  "tai"     ,  POS_STANDING,  do_tail     ,  0,  0,  0  },
  {  "take"     ,  "tak"     ,  POS_RESTING ,  do_get      ,  0,  0,  0  },
  {  "target"   ,  "tar"   ,  POS_RESTING ,  do_dist_spell, 0,  0,  0  },
  {  "taxi"     ,  "tax"     ,  POS_STANDING,  do_getaride ,  0,  0,  0  },
  {  "taste"    ,  "tas"    ,  POS_RESTING ,  do_eat      ,  0,  SCMD_TASTE,  0  },
  {  "teach"    ,  "tea"    ,  POS_RESTING ,  do_teach    , 20,  0,  0  },
  {  "teleport" ,  "tele" ,  POS_DEAD    ,  do_teleport ,  LVL_IMMORT,  0,  0  },
  {  "test"     ,  "tes"     ,  POS_RESTING ,  do_test     ,  0,  0,  0  },
//  {  "tether"   ,  "tether"   ,  POS_RESTING ,  do_not_here ,  0,  0,  0  },
  {  "tgrasp"   ,  "tgr"   ,  POS_RESTING ,  t_grasp     ,  0,  0,  0  },
  {  "thaw"     ,  "tha"     ,  POS_DEAD    ,  do_wizutil  ,  LVL_IMMORT,  SCMD_THAW,  0  },
  {  "throw"    ,  "thr"    ,  POS_STANDING,  do_throw    ,  0,  0,  0  },
  {  "tinfo"    ,  "tin"    ,  POS_RESTING ,  do_terinfo  ,  0,  0,  0  },
  {  "tie"      ,  "tie"      ,  POS_SITTING ,  do_tie      ,  0,  SCMD_TIE,  0 },
  {  "tieweave" ,  "tiew" ,  POS_RESTING ,  do_cast     ,  0,  SCMD_TIED,  0 },
  {  "title"    ,  "tit"    ,  POS_DEAD    ,  do_title    ,  0,  0,  0  },
  {  "time"     ,  "tim"     ,  POS_DEAD    ,  do_time     ,  0,  0,  0  },
  {  "timetable",  "timet",  POS_RESTING ,  do_not_here ,  0,  0,  0  },
  {  "todo"     ,  "todo"     ,  POS_DEAD    ,  do_gen_write,  LVL_IMMORT,  SCMD_TODO,  CG_ADMIN  },
  {  "toggle"   ,  "tog"   ,  POS_DEAD    ,  do_toggle   ,  0,  0,  0  },
  {  "tool"     ,  "too"     ,  POS_SITTING ,  do_tool     ,  0,  0,  0  },
  {  "track"    ,  "track"    ,  POS_STANDING,  do_track    ,  0,  0,  0  },
  {  "traction" ,  "tract" ,  POS_STANDING,  do_tract    ,  0,  0,  0  },
  {  "transfer" ,  "tran" ,  POS_SLEEPING,  do_trans    ,  LVL_IMMORT,  0,  0  },
  {  "travel"   ,  "trav"   ,  POS_STANDING,  do_travel   ,  0,  0,  0  },
  {  "treesing" ,  "tree" ,  POS_RESTING ,  do_treesing ,  0,  0,  0  },
  {  "trelease" ,  "trel" ,  POS_RESTING ,  t_release   ,  0,  0,  0  },
  {  "trigedit" ,  "trige" ,  POS_DEAD    ,  do_olc      ,  LVL_IMMORT,  SCMD_OLC_TRIGEDIT,  CG_DGS },
  {  "trip"     ,  "tri"     ,  POS_FIGHTING,  do_trip     ,  1,  0,  0  },
  {  "tset"     ,  "tset"   , POS_RESTING, do_tset, LVL_IMMORT,CG_BUILDER|CG_WEAVER|CG_ADMIN},
  {  "ttest"    ,  "tte"    ,  POS_RESTING ,  do_tertest  ,  1,  0,  0  },
  {  "tutor"    ,  "tut"    ,  POS_DEAD    ,  do_not_here ,  1,  0,  0  },
  {  "typo"     ,  "typ"     ,  POS_DEAD    ,  do_gen_write,  0,  SCMD_TYPO,  0  },
  {  "unauthorize",  "unau",  POS_DEAD  ,  do_unauthorize,  LVL_IMMORT,  0,  0 },
  {  "unlock"   ,  "unl"   ,  POS_SITTING ,  do_gen_door ,  0,  SCMD_UNLOCK,  0  },
  {  "ungroup"  ,  "ung"  ,  POS_DEAD    ,  do_ungroup  ,  0,  0,  0  },
  {  "unban"    ,  "unba"    ,  POS_DEAD    ,  do_unban    ,  LVL_GRGOD,  0,  0  },
  {  "unbond"   ,  "unbo"   ,  POS_DEAD    ,  do_unbond   ,  0,  0,  0  },
  {  "unaffect" ,  "unaf" ,  POS_DEAD    ,  do_unaffect ,  LVL_IMMORT,  SCMD_UNAFFECT,  0  },
  {  "unknot"   ,  "unk"   ,  POS_SITTING ,  do_tie      ,  0,  SCMD_UNKNOT,  0  },
  {  "unravel"  ,  "unr"  ,  POS_SITTING,   do_tie      ,  0,  SCMD_UNRAVEL,  0 },
  {  "unsever"  ,  "unse"  ,  POS_DEAD    ,  do_unsever  ,  LVL_GOD,  0,  0  },
  {  "unsponsor",  "unsp",  POS_DEAD    ,  do_unsponsor, 20,  0,  0  },
//  {  "untether" ,  "untether" ,  POS_RESTING ,  do_not_here ,  0,  0,  0  },
  {  "untaint"  ,  "unta"  ,  POS_RESTING ,  do_untaint  , 100, 0,  0  },
  {  "uptime"   ,  "upti"   ,  POS_DEAD    ,  do_date     ,  0,  SCMD_UPTIME,  0  },
  {  "use"      ,  "us"      ,  POS_SITTING ,  do_use      ,  1,  SCMD_USE,  0  },
  {  "users"    ,  "user"    ,  POS_DEAD    ,  do_users    ,  LVL_IMMORT,  0,  0  },
  {  "value"    ,  "val"    ,  POS_STANDING,  do_not_here ,  0,  0,  0  },
  {  "veil"     ,  "vei"     ,  POS_RESTING ,  do_veil     , 20,  0,  0  },
  {  "version"  ,  "ver"  ,  POS_DEAD    ,  do_gen_ps   ,  0,  SCMD_VERSION,  0  },
  {  "viewlog"  ,  "vie"  ,  POS_DEAD    ,  do_logsearch,  LVL_GRGOD,  SCMD_VIEWLOG,  0 },
  {  "visible"  ,  "vis"  ,  POS_RESTING ,  do_visible  ,  1,  0,  0  },
  {  "vnum"     ,  "vnu"     ,  POS_DEAD    ,  do_vnum     ,  1,  0,  CG_GL | CG_ST | CG_GL_BUILDER | CG_BUILDER | CG_ADMIN  },
  {  "void"     ,  "voi"     ,  POS_RESTING ,  do_void     ,  20,  0,  0  },
  {  "vstat"    ,  "vs"    ,  POS_DEAD    ,  do_vstat    ,  1,  0,  CG_GL | CG_ST | CG_GL_BUILDER | CG_BUILDER | CG_ADMIN },
  {  "wake"     ,  "wa"     ,  POS_SLEEPING,  do_wake     ,  0,  0,  0  },
  {  "wait"     ,  "wai"     ,  POS_DEAD    ,  do_wait     ,  0,  0,  0  },
  {  "wash"     ,  "was"     ,  POS_RESTING ,  do_wash     ,  0,  0,  0  },
  {  "water"    ,  "wat"    ,  POS_DEAD    ,  do_not_here ,  1,  0,  0  },
  {  "wear"     ,  "wea"     ,  POS_RESTING ,  do_wear     ,  0,  0,  0  },
  {  "weather"  ,  "weat"  ,  POS_RESTING ,  do_weather  ,  0,  0,  0  },
  {  "weave"    ,  "weav"    ,  POS_RESTING ,  do_cast     ,  1,  SCMD_MAINTAINED,  0 },
  {  "weaves"   ,  "weaves"   ,  POS_SLEEPING,  do_spells   ,  0,  0,  0  },
  {  "whois"    ,  "whoi"    ,  POS_DEAD    ,  do_whois    ,  1,  0,  0  },
  {  "whoami"   ,  "whoa"   ,  POS_DEAD    ,  do_gen_ps   ,  0,  SCMD_WHOAMI,  0  },
  {  "where"    ,  "whe"    ,  POS_RESTING ,  do_where    ,  1,  0,  0  },
  {  "whisper"  ,  "whi"  ,  POS_RESTING ,  do_spec_comm,  0,  SCMD_WHISPER,  0  },
  {  "wield"    ,  "wie"    ,  POS_RESTING ,  do_wield    ,  0,  0,  0  },
  {  "wimpy"    ,  "wim"    ,  POS_DEAD    ,  do_wimpy    ,  0,  0,  0  },
  {  "wire"     ,  "wir"     ,  POS_STANDING,  do_not_here ,  1,  0,  0  },
  {  "withdraw" ,  "wit" ,  POS_STANDING,  do_not_here ,  1,  0,  0  },
  {  "wiznet"   ,  "wiz"   ,  POS_DEAD    ,  do_wiznet   ,  LVL_IMMORT,  0,  0  },
  {  ";"        ,  ";"        ,  POS_DEAD    ,  do_wiznet   ,  LVL_IMMORT,  0,  0  },
  {  "wizhelp"  ,  "wizh"  ,  POS_SLEEPING,  do_commands ,  LVL_IMMORT,  SCMD_WIZHELP,  0  },
  {  "wizlist"  ,  "wizl"  ,  POS_DEAD    ,  do_gen_ps   ,  0,  SCMD_WIZLIST,  0  },
  {  "wizlock"  ,  "wizlo"  ,  POS_DEAD    ,  do_wizlock  ,  LVL_GRGOD,  0,  0  },
  {  "wolfcall" ,  "wolfc" ,  POS_RESTING ,  do_wolfcall ,  0,  0,  0  },
  {  "wolfsense",  "wolfs",  POS_STANDING,  do_wolfsense,  0,  0,  0  },
  {  "worth"    ,  "wor"    ,  POS_DEAD    ,  do_worth    ,  0,  0,  0  },
  {  "write"    ,  "wr"    ,  POS_STANDING,  do_write    ,  1,  0,  0  },
  {  "wset"     ,  "ws"    , POS_DEAD     ,  do_wset , LVL_IMPL, 0, CG_WEAVER },
  {  "xlog"	,  "xl"	,  POS_DEAD	,  do_xlog, 	LVL_IMMORT, 	0, 	0  },
  {  "xname"    ,  "xn"    ,  POS_DEAD    ,  do_xname    ,  LVL_IMMORT,  0,  0  },
/*  {  "zap"      ,  "za"      ,  POS_DEAD    ,  do_pzap     ,  LVL_IMMORT,
   0,  0  }, */
  {  "zcmd"     ,  "zc"     ,  POS_DEAD    ,  do_zcmd    ,  1,  0,  CG_GL_BUILDER | CG_BUILDER },
  {  "zecho"    ,  "zec"    ,  POS_DEAD    ,  do_zecho    ,  LVL_IMMORT,  0,  0  },
  {  "zedit"    ,  "zed"    ,  POS_DEAD    ,  do_olc      ,  1,  SCMD_OLC_ZEDIT,  CG_GL_BUILDER | CG_BUILDER },
  {  "zlock"    ,  "zl"    ,  POS_DEAD    ,  do_zlock    ,  LVL_IMMORT,  0,  CG_BUILDER | CG_ADMIN  },
  {  "zreset"   ,  "zr"   ,  POS_DEAD    ,  do_zreset   ,  1,  0,  CG_GL_BUILDER | CG_BUILDER  },
  {  "attach"   ,  "atta"   ,  POS_DEAD    ,  do_attach   ,  LVL_IMMORT,  0,  CG_DGS  },
  {  "detach"   ,  "deta"   ,  POS_DEAD    ,  do_detach   ,  LVL_IMMORT,  0,  CG_DGS  },
  {  "tlist"    ,  "tli"    ,  POS_DEAD    ,  do_tlist    ,  LVL_IMMORT,  0,  CG_DGS  },
  {  "tstat"    ,  "tst"    ,  POS_DEAD    ,  do_tstat    ,  LVL_IMMORT,  0,  CG_DGS  },
  {  "masound"  ,  "maso"  ,  POS_DEAD    ,  do_masound  ,  -1,  0,  0  },
  {  "mkill"    ,  "mkil"    ,  POS_STANDING,  do_mkill    ,  -1,  0,  0  },
  {  "mjunk"    ,  "mjun"    ,  POS_SITTING ,  do_mjunk    ,  -1,  0,  0  },
  {  "mdoor"    ,  "mdoo"    ,  POS_DEAD    ,  do_mdoor    ,  -1,  0,  0  },
  {  "mecho"    ,  "mech"    ,  POS_DEAD    ,  do_mecho    ,  -1,  0,  0  },
  {  "mechoaround" ,  "mechoa" ,  POS_DEAD ,  do_mechoaround,  -1,  0,  0  },
  {  "msend"    ,  "msen"    ,  POS_DEAD    ,  do_msend    ,  -1,  0,  0  },
  {  "mload"    ,  "mloa"    ,  POS_DEAD    ,  do_mload    ,  -1,  0,  0  },
  {  "mpurge"   ,  "mpur"   ,  POS_DEAD    ,  do_mpurge   ,  -1,  0,  0  },
  {  "mgoto"    ,  "mgot"    ,  POS_DEAD    ,  do_mgoto    ,  -1,  0,  0  },
  {  "mat"      ,  "mat"      ,  POS_DEAD    ,  do_mat      ,  -1,  0,  0  },
  {  "mteleport",  "mtele",  POS_DEAD    ,  do_mteleport,  -1,  0,  0  },
  {  "mforce"   ,  "mfor"   ,  POS_DEAD    ,  do_mforce   ,  -1,  0,  0  },
  {  "mexp"     ,  "mexp"     ,  POS_DEAD    ,  do_mexp     ,  -1,  0,  0  },
  {  "mgold"    ,  "mgol"    ,  POS_DEAD    ,  do_mgold    ,  -1,  0,  0  },
  {  "mhunt"    ,  "mhun"    ,  POS_DEAD    ,  do_mhunt    ,  -1,  0,  0  },
  {  "mremember",  "mrem",  POS_DEAD    ,  do_mremember,  -1,  0,  0  },
  {  "mforget"  ,  "mfor"  ,  POS_DEAD    ,  do_mforget  ,  -1,  0,  0  },
  {  "mtransform",  "mtra", POS_DEAD    ,  do_mtransform, -1,  0,  0  },
  {  "vdelete"  ,  "vdel"  ,  POS_DEAD    ,  do_vdelete  ,  LVL_IMMORT,  0,  CG_DGS  },
  {  "\n",  "zzzzzz",  0,  0,  0,  0 } ,  };
