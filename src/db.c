/******************************************************************************
 * $Id: db.c,v 1.26 2003/06/20 07:49:55 fang Exp $
 *   
 * File: db.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: db.c,v $
 *   Revision 1.26  2003/06/20 07:49:55  fang
 *
 *   db.c now loads xml-messages instead of old message files. Old code for message
 *   loading have been removed.
 *
 *   Revision 1.25  2003/06/19 13:09:13  fang
 *
 *   Code for loading xml-messages added. Also, reload options to reload messages
 *   from xml.-files added. Soon time to do the switch.
 *
 *   Revision 1.24  2003/06/17 13:02:37  fang
 *
 *   Added a single file for messages. Code for now contains only functionalty to
 *   save messages (fight/spell op/spell tp) as XML. All structs, and data, and
 *   defines pertaining to messages have been moved to the new header.
 *
 *   Revision 1.23  2003/06/10 12:16:08  fang
 *
 *   Added UNIQUE flag for mobs and objs. A unique obj/mob can only exists one
 *   instance at the time on the MUD, no matter what. A unique obj will not
 *   save to characters obj file either.
 *
 *   Revision 1.22  2003/06/10 09:43:24  fang
 *   *** empty log message ***
 *
 *   Revision 1.21  2003/06/10 09:42:17  fang
 *   *** empty log message ***
 *
 *   Revision 1.20  2003/05/12 09:08:33  fang
 *   Enabled dynvars for room descs/xdescs
 *
 *   Revision 1.19  2003/03/18 13:21:55  fang
 *   Some mem. bugs reported by Valgrind crushed.
 *
 *   Revision 1.18  2002/11/07 04:02:01  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.17  2002/11/06 04:43:18  fang
 *   Fisch: fixed some "uninitialized data used" errors
 *
 *   Revision 1.16  2002/10/23 18:29:58  fang
 *   fisch: activated the "GLOBAL" flag to mark boards global readable or not
 *
 *   Revision 1.15  2002/10/23 04:17:19  fang
 *   fisch: "list board" to list boards with new messages added
 *
 *   Revision 1.14  2002/10/12 06:30:15  fang
 *   fisch: converted boards to xml and made them dynamic not coded.
 *
 *   Revision 1.13  2002/10/03 05:55:09  fischer
 *   fischer: moved ideas to be used as an old ideas and added newideas file
 *
 *   Revision 1.12  2002/09/23 05:52:42  fischer
 *   fisch: clearing out lua triggers part 1
 *
 *   Revision 1.11  2002/09/22 21:43:49  fischer
 *   Removing mounts and horsetrain from the code
 *
 *   Revision 1.10  2002/09/15 07:17:25  fang
 *   Fisch: Removed or commented out the debug messages used to find the
 *   guild channel bug.
 *
 *   Revision 1.9  2002/09/15 00:50:08  fang
 *   Drath: XLOG update - Categories added to xlog display automatically
 *
 *   Revision 1.8  2002/09/14 16:32:52  fang
 *   fisch: more to take debug code away from the non weaver imms
 *
 *   Revision 1.7  2002/09/14 00:28:03  fang
 *   fisch: cleaned up some debugging code and added others to track that guilie
 *
 *   Revision 1.6  2002/09/12 22:21:33  fang
 *   fischer: removed the prove and journal commands
 *
 *   Revision 1.5  2002/09/08 06:41:16  fang
 *   Drath: Fischer added AEDIT.  Welcome to the staff, Fischer.
 *
 *   Revision 1.4  2002/09/05 20:56:30  fang
 *   Drath: added CRETODO and SHOWCRETODO for MassyPants
 *
 *   Revision 1.3  2002/08/04 09:39:55  fang
 *   Drath: SOCIALIDEA and SHOWSOCIALIDEA seperate from IDEA
 *
 *   Revision 1.2  2002/07/28 13:56:04  fang
 *   Removed dmlloc...just to test.
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.18  2002/07/12 03:37:33  fang
 *   Drath: MYCOLOR
 *
 *   Revision 1.17  2002/07/06 07:05:12  fang
 *   Drath: IMMLOCK for Fred and 2nd to last XLOG commit
 *
 *   Revision 1.16  2002/06/18 10:01:23  fang
 *   One bigger leak found in nanny()
 *
 *   Revision 1.15  2002/06/18 09:08:07  fang
 *   *** empty log message ***
 *
 *   Revision 1.14  2002/06/17 08:14:15  fang
 *   *** empty log message ***
 *
 *   Revision 1.13  2002/06/13 10:12:28  fang
 *   More free'ing stuff...
 *
 *   Revision 1.12  2002/06/12 09:55:16  fang
 *   More mem thingies...
 *
 *   Revision 1.11  2002/06/04 09:36:31  fang
 *   *** empty log message ***
 *
 *   Revision 1.10  2002/06/02 22:14:44  fang
 *   Maz: PEDIT
 *
 *   Revision 1.9  2002/05/27 10:52:48  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.8  2002/05/12 23:50:46  fang
 *   Added in IGNORE system, and removed AutoHotboots until the cap can be fixed for good. -Maz
 *
 *   Revision 1.7  2002/04/30 18:54:56  fang
 *   AC Cap - Request per CRE
 *
 *   Revision 1.6  2002/03/22 08:55:56  fang
 *   *** empty log message ***
 *
 *   Revision 1.5  2002/03/19 04:06:37  fang
 *   Moved the greeting into an actual file.
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
 *   Revision 1.76  2002/01/31 13:25:35  mud
 *   *** empty log message ***
 *
 *   Revision 1.75  2002/01/24 21:33:14  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.74.2.5  2002/01/24 03:20:29  mud
 *   Removed some further unescessary files (player.[ch], roomprog.[ch]
 *   old2new.c). Cleaned up some other stuff (guild stuff mostly).
 *
 *   Revision 1.74.2.4  2002/01/23 21:49:40  mud
 *   Removed objsave.[c|h] (old objsave stuff).
 *   Removed obj_file_elem and rent structs.
 *   Removed final vestiges of old guild system, except parts used
 *   in guild.[c|h] for backwards compatibility.
 *
 *   Revision 1.74.2.3  2002/01/22 20:50:58  mud
 *
 *
 *   Removed boats and info system. Obsolete, unused code with no value.
 *   Fixed bug in portal_stone SPECIAL. Now checks that a room exists
 *   before attempting to transf chars there. :)
 *
 *   Revision 1.74.2.2  2002/01/22 03:05:12  mud
 *   More cleanup. Removed extern decls from source files. Protected some DG headers from multiple inclision
 *
 *   Revision 1.74.2.1  2002/01/18 02:24:51  mud
 *   More cleanup...
 *
 *   Revision 1.74  2001/10/14 03:54:08  mud
 *   *** empty log message ***
 *
 *   Revision 1.73  2001/08/23 14:32:22  mud
 *   *** empty log message ***
 *
 *   Revision 1.72  2001/07/31 00:59:17  mud
 *   *** empty log message ***
 *
 *   Revision 1.71  2001/07/25 21:49:28  mud
 *   *** empty log message ***
 *
 *   Revision 1.70  2001/07/25 21:10:12  mud
 *   *** empty log message ***
 *
 *   Revision 1.69  2001/07/12 15:27:40  mud
 *   *** empty log message ***
 *
 *   Revision 1.68  2001/07/07 23:28:48  mud
 *   *** empty log message ***
 *
 *   Revision 1.67  2001/07/06 21:11:11  mud
 *   *** empty log message ***
 *
 *   Revision 1.66  2001/07/06 20:59:02  mud
 *   *** empty log message ***
 *
 *   Revision 1.65  2001/07/06 20:45:00  mud
 *   Fixing memleak, I was told...
 *
 *   Revision 1.64  2001/06/26 13:15:29  mud
 *   *** empty log message ***
 *
 *   Revision 1.63  2001/05/29 08:12:45  mud
 *   Minor bugfix to zone_reset(), soe more work on qstrip...
 *
 *   Revision 1.62  2001/05/28 15:46:59  mud
 *   *** empty log message ***
 *
 *   Revision 1.61  2001/05/22 09:07:03  mud
 *   DG-Scripts for Fang.
 *
 *   Revision 1.60  2001/05/17 11:21:08  mud
 *   Final residues of old pfile code should be swept away by now. No cleansing of the structures and such things though
 *
 *   Revision 1.59  2001/05/04 05:39:12  mud
 *   Multiple descs now works, without disrupting old desc system.
 *
 *   Revision 1.58  2001/04/17 12:53:52  mud
 *   *** empty log message ***
 *
 *   Revision 1.57  2001/04/11 13:10:29  mud
 *   *** empty log message ***
 *
 *   Revision 1.56  2001/04/07 14:06:31  mud
 *   Fixed a small bug in zedit
 *
 *   Revision 1.55  2001/04/06 07:00:23  mud
 *   Final xml changes before going to Beta... .:)
 *
 *   Revision 1.54  2001/03/30 21:09:07  mud
 *   Added support for most 8-bit characters
 *
 *   Revision 1.53  2001/03/18 10:30:52  mud
 *   Some bugfixes and upodates to the xmlcharsave code
 *
 *   Revision 1.52  2001/01/31 15:44:53  mud
 *   *** empty log message ***
 *
 *   Revision 1.51  2001/01/24 14:23:57  mud
 *   *** empty log message ***
 *
 *   Revision 1.50  2001/01/13 16:47:59  mud
 *   *** empty log message ***
 *
 *   Revision 1.49  2000/12/11 16:21:13  mud
 *   Some minor bugfixes, and some parts of qstrip done
 *
 *   Revision 1.48  2000/11/28 08:48:24  mud
 *   XML char save/load owrks. As does ptable building. Conversions looks fine and dandy too, although there's a few minor things to polish
 *
 *   Revision 1.47  2000/11/21 15:31:13  mud
 *   *** empty log message ***
 *
 *   Revision 1.46  2000/11/12 14:44:13  mud
 *   More support code for xml transforming, a few bugfixes to xmlchar code, and index_from_array().
 *
 *   Revision 1.45  2000/11/09 14:13:47  mud
 *   Getting closer to switching-dat with xml chars
 *
 *   Revision 1.44  2000/11/08 20:34:25  mud
 *   Added the todo list for imms
 *
 *   Revision 1.43  2000/11/08 08:25:39  mud
 *   Moved update_p_tabel() to xmlcharsave.[ch].
 *
 *   Revision 1.42  2000/11/08 07:47:42  mud
 *   *** empty log message ***
 *
 *   Revision 1.41  2000/11/07 12:00:39  mud
 *   Finally. XML Mobiles are done.
 *
 *   Revision 1.40  2000/11/03 23:00:24  mud
 *   *** empty log message ***
 *
 *   Revision 1.39  2000/10/28 15:28:31  mud
 *   added a PLR_NOMAIL flag (so a player can't send mail)
 *
 *   Revision 1.38  2000/09/04 02:58:49  mud
 *   Warnings removed...
 *
 *   Revision 1.37  2000/08/30 11:05:03  mud
 *   Added freeing of chess structure in free_char()
 *
 *   Revision 1.36  2000/08/30 02:32:17  mud
 *   More on dice stuff...
 *
 *   Revision 1.35  2000/08/30 02:15:08  mud
 *   *** empty log message ***
 *
 *   Revision 1.34  2000/08/29 05:10:00  mud
 *   Now the 'real' hpdice code should be working. As soon as xml saving/parsing is in properly, the same will be true for mana/move.
 *
 *   Revision 1.33  2000/08/29 04:32:12  mud
 *   fixed support for mana/move/hp dice thingy (xdy+z) for future use
 *
 *   Revision 1.32  2000/08/20 07:59:52  mud
 *   added a help_index. ordered linkedlist of help keywords and their pos in the help_table
 *
 *   Revision 1.31  2000/08/14 19:53:48  mud
 *   added check in db.c::load_help for too large help entries.
 *
 *   Revision 1.30  2000/08/14 19:11:55  mud
 *   The new guild system.
 *
 *   Revision 1.29  2000/08/14 14:45:31  mud
 *   changes to hedit: changed 'heditsave' to 'heditutil' with save and delete
 *
 *   Revision 1.28  2000/08/10 15:06:29  mud
 *   added hedit (help editor)
 *
 *   Revision 1.27  2000/08/05 16:54:04  mud
 *   changed lastemote to also do pose, pemote, and cemote.
 *
 *   Revision 1.26  2000/08/03 13:47:32  mud
 *   fixed zone 'R' command
 *
 *   Revision 1.25  2000/08/01 11:55:44  mud
 *   *** empty log message ***
 *
 *   Revision 1.24  2000/08/01 11:50:59  mud
 *   added roomset command. sets roomflags that won't save
 *
 *   Revision 1.23  2000/07/31 16:13:51  mud
 *   added lastemote
 *
 *   Revision 1.22  2000/06/16 01:16:33  mud
 *   Added vnum -f .. -Tesla
 *
 *   Revision 1.21  2000/06/08 14:49:22  mud
 *   olc name lists saved/parsed in xmlZones. Names and all flag checked in OLC permissions.
 *
 *   Revision 1.20  2000/06/06 13:56:13  mud
 *   Finally. XML Objects works...The Mystery of the Vanishing ZCMDs is resolved as well.
 *
 *   Revision 1.19  2000/05/26 14:07:02  mud
 *   Recall command, showideas, showbugs, showtypos.
 *
 *   Revision 1.18  2000/05/24 08:41:13  mud
 *   Aiel endurance changes
 *
 *   Revision 1.17  2000/04/25 11:10:38  mud
 *   EXP gain is modified on behalf of newbies (lvl <= 20)
 *
 *   Revision 1.16  2000/04/24 03:03:12  mud
 *   Fixed a bug in reset_zone...
 *
 *   Revision 1.15  2000/04/18 00:47:55  mud
 *   Minor fix to club code (do_reject)
 *
 *   Revision 1.14  2000/04/10 21:14:46  mud
 *   Fixed a few minor things that had to do with xml saving of room objs.
 *
 *   Revision 1.13  2000/04/09 20:34:04  mud
 *   Changed player obj saving to XML format.
 *
 *   Revision 1.12  2000/04/02 11:57:10  mud
 *   Policy files can now be reloaded via 'reload xpolicy'.
 *
 *   Revision 1.11  2000/03/15 13:11:40  mud
 *   Changed from B-tree based skill/weave index to 'something else' :)
 *
 *   Revision 1.10  2000/03/09 12:47:44  mud
 *   Code cleanup. Compiles without warnings now...
 *
 *   Revision 1.9  2000/03/08 15:54:17  mud
 *   More on INFO, now almost 100% searchable.. :)
 *
 *   Revision 1.8  2000/03/02 16:27:42  mud
 *   ZCMDs for loading mobs/objs on furniture now works...
 *
 *   Revision 1.7  2000/03/02 12:54:51  mud
 *   Zones are now saved/loaded in XML-format.
 *
 *   Revision 1.6  2000/03/02 09:34:55  mud
 *   zone reset now honours the percentage load, 3 new commands added to zone cmds, but not implemented yet
 *
 *   Revision 1.5  2000/03/01 18:06:17  mud
 *   Skeleton code added for zone XML, as well as code for future percentage chance of zone cmds...
 *
 *   Revision 1.4  2000/03/01 15:15:45  mud
 *   Named specials added, saved in room XML, and setable in redit
 *
 *   Revision 1.3  2000/02/29 16:41:32  mud
 *   XML saving/parsing should actually work now! :)
 *
 *   Revision 1.2  2000/02/21 14:00:16  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* ************************************************************************
*   File: db.c                                          Part of CircleMUD *
*  Usage: Loading/saving chars, booting/resetting world, internal funcs   *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#define __DB_C__

#include "act.h"
#include "config.h"
#include "conf.h"
#include "sysdep.h"
#include "fight.h"
#include "structs.h"
#include "utils.h"
#include "db.h"
#include "comm.h"
#include "handler.h"
#include "spells.h"
#include "shop.h"
#include "ban.h"
#include "spec_procs.h"
#include "mail.h"
#include "interpreter.h"
#include "objprog.h"
#include "bitvector.h"
#include "class.h"
#include "xmlRooms.h"
#include "namedspec.h"
#include "xmlZones.h"
#include "skills.h"
#include "xmlobjsave.h"
#include "guild_parser.h"
#include "xmlObjects.h"
#include "dll.h"
#include "olc.h"
#include "chess.h"
#include "xmlMobiles.h"
#include "interpreter.h"
#include "xmlcharsave.h"
#include "dg_scripts.h"
#include "constants.h"
#include "portstone.h"
#include "mobact.h"
#include "boards.h"
#include "dynvars.h"
#include "messages.h"

char    buf[MAX_STRING_LENGTH]  = {0};
char    buf1[MAX_STRING_LENGTH] = {0};
char    buf2[MAX_STRING_LENGTH] = {0};
char    arg[MAX_STRING_LENGTH]  = {0};

char err_buf[MAX_STRING_LENGTH] = {0};

/**************************************************************************
*  declarations of most of the 'global' variables                         *
************************************************************************ */

struct room_data *world = NULL;	/* array of rooms		 */
int top_of_world = 0;		/* ref to top element of world	 */
int last_room_rnum = 0;

struct char_data *character_list = NULL;	/* global linked list of
                         * chars	 */
struct index_data **trig_index; /* index table for triggers      */
int top_of_trigt = 0;           /* top of trigger index table    */
long max_id = MOBOBJ_ID_BASE;   /* for unique mob/obj id's       */

struct index_data *mob_index=NULL;	/* index table for mobile file */
struct char_data *mob_proto=NULL;	/* prototypes for mobs */
int top_of_mobt = 0;		/* top of mobile index table	 */
int last_mob_rnum = 0; 

struct obj_data *object_list=NULL;	/* global linked list of objs	 */
struct index_data *obj_index=NULL;	/* index table for object file */
struct obj_data *obj_proto=NULL;	/* prototypes for objs */
int top_of_objt = 0;		/* top of object index table	 */
int last_obj_rnum = 0;

struct zone_data *zone_table=NULL;	/* zone table */
int top_of_zone_table = 0;	/* top element of zone tab	 */
int last_zone_rnum = 0;

struct player_index_element *player_table = NULL;	/* index to plr file	 */
int top_of_p_table = 0;		/* ref to top of table		 */
int top_of_p_file = 0;		/* ref of size of p file	 */
long top_idnum = 0;		/* highest idnum in use		 */

int no_mail = 0;		/* mail disabled?		 */
int mini_mud = 0;		/* mini-mud mode?		 */
int no_rent_check = 0;		/* skip rent check on boot?	 */
int hotboot = 0;
time_t boot_time = 0;		/* time of mud boot		 */
time_t hotboot_time = 0;	/* time of mud hotboot		 */
int restrict_run = 0;		/* level of game restriction	 */
int immlock = 0;		/* IMMs restricted from logon?   */
int time_restrict = 0;          /* Should time-messages be supressed ? */
sh_int r_mortal_start_room = 0;	/* rnum of mortal start room	 */
sh_int r_immort_start_room = 0;	/* rnum of immort start room	 */
sh_int r_frozen_start_room = 0;	/* rnum of frozen start room	 */
int max_logged_on = 0;		/* Max # players logged on       */
long top_bond_id = 0;               /* Unique ID for bonding         */
long top_minion_id = 1;         /* Unique ID for minionizing     */
long top_ashabond_id = 1;       /* Unique ID for ashabonding     */ 
char *GREETINGS = NULL;
char *credits = NULL;		/* game credits			 */
char *news = NULL;		/* mud news			 */
char *oldnews = NULL;		/* old mud news - for archive    */
char *journal = NULL;           /* mud journal                   */
char *motd = NULL;		/* message of the day - mortals */
char *imotd = NULL;		/* message of the day - immorts */
char *help = NULL;		/* help screen			 */
char *poli = NULL;		/* new policy screen			 */
char *policy = NULL;		/* policy screen		 */
char *info = NULL;		/* info page			 */
char *wizlist = NULL;		/* list of higher gods		 */
char *immlist = NULL;		/* list of peon gods		 */
char *background = NULL;	/* background story		 */
char *handbook = NULL;		/* handbook for new immortals	 */
char *gossip = NULL;		/* weekly gossips....		 */
char *bugs = NULL;		/* bugs file */
char *ideas = NULL;		/* ideas file */
char *newideas = NULL;		/* newideas file */
char *socialideas = NULL;	/* social ideas file */
char *todo = NULL;		/* todo file */
char *typos = NULL;		/* typos file */
char *cretodo = NULL;
int questlock =0;               /* Flag for quest on/off         */
const int antidir[10] = {2,3,0,1,5,4,8,9,6,7};

struct help_index_element *help_table = 0;	/* the help table	 */
struct help_indexer_element *help_index = 0;	/* the help index	*/
int top_of_helpt = 0;		/* top of help index table	 */
struct dns_entry *dns_cache[DNS_HASH_NUM] = {NULL};

struct poli_index_element *poli_table = 0;	/* the poli table	 */
struct poli_indexer_element *poli_index = 0;	/* the poli index	*/
int top_of_polit = 0;		/* top of poli index table	 */

struct time_info_data time_info = {0};/* the infomation about the time    */
struct weather_data weather_info = {0};	/* the infomation about the weather */
struct player_special_data dummy_mob;	/* dummy spec area for mobs	 */
struct reset_q_type reset_q = {0};	/* queue of zones to be reset	 */

/* local functions */
void setup_dir(FILE * fl, int room, int dir);
void index_boot(int mode);
void discrete_load(FILE * fl, int mode);
void parse_room(FILE * fl, int virtual_nr);
void parse_mobile(FILE * mob_f, int nr);
char *parse_object(FILE * obj_f, int nr);
void load_zones(FILE * fl, char *zonename);
void load_help(FILE *fl);
void load_poli(FILE *fl);
void load_policy(FILE *fp);
void assign_mobiles(void);
void assign_objects(void);
void assign_rooms(void);
void assign_the_shopkeepers(void);
void build_player_index(void);
int is_empty(int zone_nr);
void reset_zone(int zone);
int file_to_string(char *name, char *buf);
void check_start_rooms(void);
void renum_world(void);
void renum_zone_table(void);
void log_zone_error(int zone, int cmd_no, char *message);
void reset_time(void);
void clear_char(struct char_data * ch);
void boot_dns(void);
void save_dns_cache(void);
int get_host_from_cache(struct dns_entry *dnsd); /* dnsmod */
void add_dns_host(struct dns_entry *dnsd, char *hostname); /* dnsmod */

#define READ_SIZE 256

/*************************************************************************
*  routines for booting the system                                       *
*********************************************************************** */

/* this is necessary for the autowiz system */
void reboot_wizlists(void)
{
  file_to_string_alloc(WIZLIST_FILE, &wizlist);
  file_to_string_alloc(IMMLIST_FILE, &immlist);
}

#define NULL_ONE_CHAR(str,char) ((str)[strlen((str)) - 1] == (char) ? \
                                 (str)[strlen((str)) - 1] = '\0' : \
                                 (str)[strlen((str)) - 1])

ACMD(do_reboot)
{
  void free_help(struct help_index_element *help);
  void free_poli(struct poli_index_element *poli);

  int i;
  struct olc_save_info *entry;

  one_argument(argument, arg);

  if (!str_cmp(arg, "all") || *arg == '*') {
    file_to_string_alloc(GREETINGS_FILE, &GREETINGS);
    NULL_ONE_CHAR(GREETINGS,'\n');
    NULL_ONE_CHAR(GREETINGS,'\r');

    file_to_string_alloc(WIZLIST_FILE, &wizlist);
    file_to_string_alloc(IMMLIST_FILE, &immlist);
    file_to_string_alloc(NEWS_FILE, &news);
    file_to_string_alloc(OLDNEWS_FILE, &oldnews);
    file_to_string_alloc(CREDITS_FILE, &credits);
    file_to_string_alloc(MOTD_FILE, &motd);
    file_to_string_alloc(IMOTD_FILE, &imotd);
    file_to_string_alloc(HELP_PAGE_FILE, &help);
    file_to_string_alloc(POLI_PAGE_FILE, &poli);
    file_to_string_alloc(INFO_FILE, &info);
    file_to_string_alloc(HANDBOOK_FILE, &handbook);
    file_to_string_alloc(BACKGROUND_FILE, &background);
    file_to_string_alloc(GOSSIP_FILE, &gossip);
    file_to_string_alloc(BUG_FILE, &bugs);
    file_to_string_alloc(IDEA_FILE, &ideas);
    file_to_string_alloc(NEWIDEA_FILE, &newideas);
    file_to_string_alloc(SOCIDEA_FILE, &socialideas);
    file_to_string_alloc(TODO_FILE, &todo);
    file_to_string_alloc(TYPO_FILE, &typos);
    file_to_string_alloc(CRETODO_FILE, &cretodo);
  }
  else if (!str_cmp(arg, "greeting")) {
    file_to_string_alloc(GREETINGS_FILE, &GREETINGS);
    NULL_ONE_CHAR(GREETINGS,'\n');
    NULL_ONE_CHAR(GREETINGS,'\r');
  }

  else if (!str_cmp(arg, "wizlist"))
    file_to_string_alloc(WIZLIST_FILE, &wizlist);
  else if (!str_cmp(arg, "immlist"))
    file_to_string_alloc(IMMLIST_FILE, &immlist);
  else if (!str_cmp(arg, "news"))
    file_to_string_alloc(NEWS_FILE, &news);
  else if (!str_cmp(arg, "oldnews"))
    file_to_string_alloc(OLDNEWS_FILE, &oldnews);
  else if (!str_cmp(arg, "credits"))
    file_to_string_alloc(CREDITS_FILE, &credits);
  else if (!str_cmp(arg, "motd"))
    file_to_string_alloc(MOTD_FILE, &motd);
  else if (!str_cmp(arg, "imotd"))
    file_to_string_alloc(IMOTD_FILE, &imotd);
  else if (!str_cmp(arg, "help"))
    file_to_string_alloc(HELP_PAGE_FILE, &help);
  else if (!str_cmp(arg, "poli"))
    file_to_string_alloc(POLI_PAGE_FILE, &poli);
  else if (!str_cmp(arg, "info"))
    file_to_string_alloc(INFO_FILE, &info);
  else if (!str_cmp(arg, "handbook"))
    file_to_string_alloc(HANDBOOK_FILE, &handbook);
  else if (!str_cmp(arg, "background"))
    file_to_string_alloc(BACKGROUND_FILE, &background);
  else if (!str_cmp(arg, "gossip"))
    file_to_string_alloc(GOSSIP_FILE, &gossip);
  else if (!str_cmp(arg, "bugs")) 
    file_to_string_alloc(BUG_FILE, &bugs);  
  else if (!str_cmp(arg, "ideas")) 
    file_to_string_alloc(IDEA_FILE, &ideas);  
  else if (!str_cmp(arg, "newideas")) 
    file_to_string_alloc(NEWIDEA_FILE, &newideas);  
  else if (!str_cmp(arg, "socialideas")) 
    file_to_string_alloc(SOCIDEA_FILE, &socialideas);  
  else if (!str_cmp(arg, "todo"))
    file_to_string_alloc(TODO_FILE, &todo);
  else if (!str_cmp(arg, "cretodo"))
    file_to_string_alloc(CRETODO_FILE, &cretodo);
  else if (!str_cmp(arg, "typos")) 
    file_to_string_alloc(TYPO_FILE, &typos);  
  else if (!str_cmp(arg, "xhelp")) {
    if (olc_save_list)
      for (entry = olc_save_list; entry; entry = entry->next)
        if ((int)entry->type == OLC_SAVE_HELP) {
          send_to_char("You must save the changed help entries first.\r\n", ch);
          return;
        }
    if (help_table) {
      for (i = 0; i <= top_of_helpt; i++)
        free_help(help_table + i);
      free(help_table);
    }
    top_of_helpt = 0;
    index_boot(DB_BOOT_HLP);
  } else if (!str_cmp(arg, "xpolicy")) {
    if (poli_table) {
      for (i = 0; i <= top_of_polit; i++)
        free_poli(poli_table + i);
      free(poli_table);
    }
    index_boot(DB_BOOT_POL);
  } else if (!str_cmp(arg,"fightmsg")) {
    register int i;
    for (i = 0; i < MAX_MESSAGES; i++) {
      if (fight_messages[i].a_type) {
	struct message_type *m, *mnext;

	fight_messages[i].a_type = fight_messages[i].number_of_attacks = 0;
	for (m = fight_messages[i].msg; NULL != m; m = mnext) {
	  mnext = m->next;
	  free_fight_msg(&m->die_msg);
	  free_fight_msg(&m->hit_msg);
	  free_fight_msg(&m->miss_msg);
	  free_fight_msg(&m->god_msg);
	  free(m);
	}
	fight_messages[i].msg = NULL;
      }
    }
    i = load_messages_xml(TYPE_MSG);
    if (i) {
	asend_to_char(ch,"reload failed: %s\r\n",msg_error(i));
	return;
    }
  } else if (!str_cmp(arg,"opmsg")) {
    register int i;
    for (i = 0; i < MAX_SPELL_MESS; i++) {
      if (spell_messages[i].a_type) {
        struct sp_message_type *m, *mnext;

        spell_messages[i].a_type = spell_messages[i].number_of_attacks = 0;
        for (m = spell_messages[i].msg; NULL != m; m = mnext) {
          mnext = m->next;
          free_sp_msg(&m->die_msg);
          free_sp_msg(&m->hit_msg);
          free_sp_msg(&m->miss_msg);
          free_sp_msg(&m->god_msg);
	  free(m);
        }
        spell_messages[i].msg = NULL;
      }
    }
    i = load_messages_xml(TYPE_SP_MSG);
    if (i) {
      asend_to_char(ch,"reload failed: %s\r\n",msg_error(i));
      return;
    }
  } else if (!str_cmp(arg,"tpmsg")) {
    register int i;
    for (i = 0; i < MAX_SPELL_MESS; i++) {
      if (tp_messages[i].a_type) {
        struct tp_message_type *m, *mnext;

        tp_messages[i].a_type = tp_messages[i].number_of_attacks = 0;
        for (m = tp_messages[i].msg; NULL != m; m = mnext) {
          mnext = m->next;
          free_tp_msg(&m->die_msg);
          free_tp_msg(&m->hit_msg);
          free_tp_msg(&m->miss_msg);
          free(m);
        }
        tp_messages[i].msg = NULL;
      }
    }
    i = load_messages_xml(TYPE_TP_MSG);
    if (i) {
      asend_to_char(ch,"reload failed: %s\r\n",msg_error(i));
      return;
    } 
  } else {
    send_to_char("Unknown reload option.\r\n", ch);
    return;
  }

  send_to_char(OK, ch);
}

/*
 * Loads objects saved to file at boot time and puts the loaded objects
 * into the rooms they were saved.
 */
void load_objs_saved_in_rooms()
{
  int  i = 0;
 
  for (i = 0; rooms_that_save_objs[i] > 0; i++) 
    read_XML_roomobjs(rooms_that_save_objs[i]);
}

void boot_world(void)
{
  int rnum;
  struct affected_type af;

  alog("Loading zone table. (zone_data=%d)", sizeof(struct zone_data));
  index_boot(DB_BOOT_ZON);

  mlog("Loading triggers and generating index.");
  index_boot(DB_BOOT_TRG);

  mlog("Booting Named Specials Data...");
  bootNamedSpecials();

  alog("Loading rooms. (room_data=%d)", sizeof(struct room_data));
  index_boot(DB_BOOT_WLD);

  mlog("Renumbering rooms.");
  renum_world();
  
  mlog("Checking start rooms.");
  check_start_rooms();

  alog("Loading objs and generating index. (obj_data=%d)", sizeof(struct obj_data));
  index_boot(DB_BOOT_OBJ);

  // mlog("Renumbering zone table.");
  // renum_zone_table();

  alog("Loading mobs and generating index. (char_data=%d)", sizeof(struct char_data));
  index_boot(DB_BOOT_MOB);

  mlog("Loading portal stones.");
//  read_stones();

  mlog("Loading objs saved in rooms.");
  load_objs_saved_in_rooms();

  if (!no_specials) {
    mlog("Loading shops.");
    index_boot(DB_BOOT_SHP);
  }

  if (-1 != (rnum = real_room(10186))) {
    // Urgh, hard coded... Better way?
    af.duration = -1;
    af.bitvector= 0;
    af.speed    = RAFF_SLOW;
    af.type     = SPELL_SANCTUARY;
    af.sex      = TRUE_POWER;
    af.next	= NULL;
    af.tie_info = SCMD_TIED;
    af.weaver   = 0;
    af.mana_add = 0;
    affect_to_room(&(world[rnum]),&af);
    mlog("Adding permanent Sanctuary to Oval Plaza.");
  }

  if (-1 != (rnum = real_room(10188))) {
    // Urgh, hard coded... Better way?
    af.duration = -1;
    af.bitvector= 0;
    af.speed    = RAFF_SLOW;
    af.type     = SPELL_SANCTUARY;
    af.sex      = TRUE_POWER;
    af.next	= NULL;
    af.tie_info = SCMD_TIED;
    af.weaver   = 0;
    af.mana_add = 0;
    affect_to_room(&(world[rnum]),&af);
    mlog("Adding permanent Sanctuary to Death Room.");
  }
}

  
/* body of the booting system */
void boot_db(void)
{
  int i;

  mlog("Boot db -- BEGIN.");

  mlog("Resetting the game time:");
  reset_time();

  mlog("Reading news, oldnews, credits, help, policy, bground, info & motds.");
  file_to_string_alloc(GREETINGS_FILE, &GREETINGS);
  NULL_ONE_CHAR(GREETINGS,'\n');
  NULL_ONE_CHAR(GREETINGS,'\r');

  file_to_string_alloc(NEWS_FILE, &news);
  file_to_string_alloc(OLDNEWS_FILE, &oldnews);
  file_to_string_alloc(CREDITS_FILE, &credits);
  file_to_string_alloc(MOTD_FILE, &motd);
  file_to_string_alloc(IMOTD_FILE, &imotd);
  file_to_string_alloc(HELP_PAGE_FILE, &help);
  file_to_string_alloc(POLI_PAGE_FILE, &poli);
  file_to_string_alloc(INFO_FILE, &info);
  file_to_string_alloc(WIZLIST_FILE, &wizlist);
  file_to_string_alloc(IMMLIST_FILE, &immlist);
  file_to_string_alloc(HANDBOOK_FILE, &handbook);
  file_to_string_alloc(BACKGROUND_FILE, &background);
  file_to_string_alloc(GOSSIP_FILE, &gossip);
  file_to_string_alloc(BUG_FILE, &bugs);
  file_to_string_alloc(IDEA_FILE, &ideas);
  file_to_string_alloc(NEWIDEA_FILE, &newideas);
  file_to_string_alloc(SOCIDEA_FILE, &socialideas);
  file_to_string_alloc(TODO_FILE, &todo);
  file_to_string_alloc(TYPO_FILE, &typos);
  file_to_string_alloc(CRETODO_FILE, &cretodo);

  boot_world();

  mlog("Loading help entries.");
  index_boot(DB_BOOT_HLP);

  mlog("Loading Boards.");
  init_boards();

  mlog("Starting dynvars system.");
  init_dynvars();

  mlog("Loading policy entries.");
  index_boot(DB_BOOT_POL);

  mlog("Booting guilds.");  
  index_boot(DB_BOOT_GLD);

  alog("Generating player index. (player_index_element=%d)", sizeof(struct player_index_element));
  build_player_index();

  i = load_messages_xml(TYPE_MSG);
  if (i) {
    alog("Loading XML fight messages.....%s",msg_error(i));
  } else {
    mlog("Loading XML fight messages.....Ok");
  }

  i = load_messages_xml(TYPE_SP_MSG);
  if (i) {
    alog("Loading XML spell messages.....%s", msg_error(i));
  } else {
    mlog("Loading XML spell messages.....OK");
  }

  i = load_messages_xml(TYPE_TP_MSG);
  if (i) {
    alog("Loading XML messages for True Power.....%s", msg_error(i));
  } else {
    mlog("Loading XML messages for True Power.....Ok");
  }

  mlog("Loading social messages.");
  boot_social_messages();
  create_command_list();

  mlog("Assigning function pointers:");

  if (!no_specials) {
    mlog("   Mobiles.");
    assign_mobiles();
    mlog("   Shopkeepers.");
    assign_the_shopkeepers();
    mlog("   Objects.");
    assign_objects();
    mlog("   Rooms.");
    assign_rooms();
  }
  mlog("   Spells.");
  mag_assign_spells();

  mlog("Assigning spell and skill levels.");
  init_spell_levels();

  mlog("Sorting command list.");
  sort_commands();
  mlog("Sorting spells.");
  sort_spells();

  mlog("Building index of spells and skills.");
  build_skill_index();

  mlog("Booting mail system.");
  if (!scan_file()) {
    mlog("    Mail boot failed -- Mail system disabled");
    no_mail = 1;
  }
  mlog("Reading banned site and invalid-name list.");
  load_banned();
  Read_Invalid_List();

  mlog("Booting DNS-cache.");
  boot_dns();

  for (i = 0; i <= top_of_zone_table; i++) {
    sprintf(buf2, "Resetting %s (rooms %d-%d).",
        zone_table[i].name, (i ? (zone_table[i - 1].top + 1) : 0),
        zone_table[i].top);
    mlog(buf2);
    reset_zone(i);
  }
  reset_q.head = reset_q.tail = NULL;

  boot_time = time(0);
  hotboot_time = time(0);

  mlog("Boot db -- DONE.");
}


/* reset the time in the game from file */
void reset_time(void)
{
  long beginning_of_time = 650336715;

  time_info = mud_time_passed(time(0), beginning_of_time);

  if (time_info.hours <= 4)
    weather_info.sunlight = SUN_DARK;
  else if (time_info.hours == 5)
    weather_info.sunlight = SUN_RISE;
  else if (time_info.hours <= 20)
    weather_info.sunlight = SUN_LIGHT;
  else if (time_info.hours == 21)
    weather_info.sunlight = SUN_SET;
  else
    weather_info.sunlight = SUN_DARK;

  sprintf(buf, "   Current Gametime: %ldH %ldD %ldM %dY.", time_info.hours,
      time_info.day, time_info.month, time_info.year);
  mlog(buf);

  weather_info.pressure = 960;
  if ((time_info.month >= 7) && (time_info.month <= 12))
    weather_info.pressure += dice(1, 50);
  else
    weather_info.pressure += dice(1, 80);

  weather_info.change = 0;

  if (weather_info.pressure <= 980)
    weather_info.sky = SKY_LIGHTNING;
  else if (weather_info.pressure <= 1000)
    weather_info.sky = SKY_RAINING;
  else if (weather_info.pressure <= 1020)
    weather_info.sky = SKY_CLOUDY;
  else
    weather_info.sky = SKY_CLOUDLESS;
}

char *id_to_name(long pid)
{
  if ((pid < 0) || (pid > top_of_p_table) || (player_table[pid].deleted))
    return NULL;
  else
    return player_table[pid].name;
}

/* generate index table for the player file */
void build_player_index(void)
{
  if (!load_playertable()) {
    mlog("No playerfiles.");
    player_table = NULL;
    top_of_p_file = top_of_p_table = -1;

    return;
  }
  sprintf(buf, "   %d players in database.", (top_of_p_table+1));
  mlog(buf);
}



/* function to count how many hash-mark delimited records exist in a file */
int count_hash_records(FILE * fl)
{
  char buf[128];
  int count = 0;

  while (fgets(buf, 128, fl))
    if (*buf == '#')
      count++;

  return count;
}



void index_boot(int mode)
{
  char *index_filename, *prefix = NULL;
  FILE *index, *db_file, *new_index;
  int rec_count = 0;
  char new_index_name[MAX_STRING_LENGTH];
  int file_not_found = 0;

  switch (mode) {
  case DB_BOOT_TRG:
    prefix = TRG_PREFIX;
    break;
  case DB_BOOT_WLD:
    prefix = WLD_PREFIX;
    break;
  case DB_BOOT_MOB:
    prefix = MOB_PREFIX;
    break;
  case DB_BOOT_OBJ:
    prefix = OBJ_PREFIX;
    break;
  case DB_BOOT_ZON:
    prefix = ZON_PREFIX;
    break;
  case DB_BOOT_SHP:
    prefix = SHP_PREFIX;
    break;
  case DB_BOOT_HLP:
    prefix = HLP_PREFIX;
    break;
  case DB_BOOT_QST:
    prefix = QST_PREFIX;
    break;
  case DB_BOOT_POL:
    prefix = POL_PREFIX;
    break;
  case DB_BOOT_GLD:
    prefix = GUILD_PREFIX;
    break;
  default:
    mlog("SYSERR: Unknown subcommand to index_boot!");
    exit(1);
    break;
  }

  if (mini_mud)
    index_filename = MINDEX_FILE;
  else
    index_filename = INDEX_FILE;

  sprintf(buf2, "%s/%s", prefix, index_filename);

  if (!(index = fopen(buf2, "r"))) {
    sprintf(buf1, "SYSERR: Error opening index file '%s'", buf2);
    mlog(buf1);
    exit(1);
  }
   
  /* first, count the number of records in the file so we can alloc */
  fscanf(index, "%s\n", buf1);
   
  while (*buf1 != '$') {
    sprintf(buf2, "%s/%s", prefix, buf1);

    if (!(db_file = fopen(buf2, "r"))) {
      alog("SYSERR: File '%s' listed in index not found, skipping file!", buf2);
      file_not_found = 1;
    } 
    else {
       if (mode == DB_BOOT_ZON || mode == DB_BOOT_GLD)
	 rec_count++;
      else {
	char bf[1024];
    
        /* We need to check for XML-saved files here....
         */
	fgets(bf,1024,db_file);
	rewind(db_file);

        if (!strncasecmp("<?xml version=\"1.0\"?>",bf,strlen("<?xml version=\"1.0\"?>"))) {
	  /** do some kind of 'cached' count_xml_records here? **/
	  /** Actually we need to check ofr mode here too....for future **/
	  /** xml saving of shops/objs/mobs **/
	  
	  if (DB_BOOT_WLD == mode)
	    rec_count += count_xml_roomentries(db_file);
	  if (DB_BOOT_OBJ == mode)
	    rec_count += count_xml_objectentries(db_file);
	  if (DB_BOOT_MOB == mode)
	    rec_count += count_xml_mobileentries(db_file);
	  
	  // alog("SYSERR: boot error - %d records counted",rec_count);
        } else {
	  rec_count += count_hash_records(db_file);
        }
      }
    }
    
    if (db_file)
      fclose(db_file);
    fscanf(index, "%s\n", buf1);
  }

  /* Exit if 0 records, unless this is shops */
  if (!rec_count) {
    if (mode == DB_BOOT_SHP || mode == DB_BOOT_GLD)
      return;
    sprintf(buf, "SYSERR: boot error - 0 records counted: `%s'", buf2);
    mlog(buf);
    exit(1);
  }

  rec_count+=2;

  switch (mode) {
  case DB_BOOT_TRG:
    CREATE(trig_index, struct index_data *, rec_count);
    break;
  case DB_BOOT_WLD:
    CREATE(world, struct room_data, rec_count);
    break;
  case DB_BOOT_MOB:
    CREATE(mob_proto, struct char_data, rec_count);
    CREATE(mob_index, struct index_data, rec_count);
    break;
  case DB_BOOT_OBJ:
    CREATE(obj_proto, struct obj_data, rec_count);
    CREATE(obj_index, struct index_data, rec_count);
    break;
  case DB_BOOT_ZON:
    CREATE(zone_table, struct zone_data, rec_count);
    break;
  case DB_BOOT_HLP:
    CREATE(help_table, struct help_index_element, rec_count);
    break;
  case DB_BOOT_POL:
    CREATE(poli_table, struct poli_index_element, rec_count);
    break;
  }

  rewind(index);
  fscanf(index, "%s\n", buf1);
  while (*buf1 != '$') {
    char bf[1024];

    sprintf(buf2, "%s/%s", prefix, buf1);
    if (!(db_file = fopen(buf2, "r"))) {
      alog("SYSERR: File '%s' listed in index not found, skipping file!", buf2);
      file_not_found = 1;
    }
    else 
    {
      /** Before calling discrete_load, we need to check if an XML-saved file... **/
      fgets(bf,1024,db_file);
      rewind(db_file);
    
      if (!strncasecmp("<?xml version=\"1.0\"?>",bf,strlen("<?xml version=\"1.0\"?>"))) {
        if (DB_BOOT_WLD == mode)
	  load_xml_rooms(buf2);
        if (DB_BOOT_ZON == mode)
	  load_xml_zone(buf2);
        if (mode == DB_BOOT_GLD)
	  XmlToGuild(buf2);
        if (DB_BOOT_OBJ == mode)
	  load_xml_objects(buf2);
        if (DB_BOOT_MOB == mode)
	  load_xml_mobiles(buf2);

        /** call func for parsing and stuffing-into-world-array xml rooms here **/
        /** as above, we need to check for different modes here too... **/ 
      } else {
        switch (mode) {
        case DB_BOOT_TRG:
        case DB_BOOT_WLD:
        case DB_BOOT_OBJ:
        case DB_BOOT_MOB:
        case DB_BOOT_QST:
	  discrete_load(db_file, mode);
	  break;
        case DB_BOOT_ZON:
	  load_zones(db_file, buf2);
	  break;
        case DB_BOOT_HLP:
	  load_help(db_file);
	  break;
        case DB_BOOT_POL:
	  load_poli(db_file);
	  break;
        case DB_BOOT_SHP:
	  boot_the_shops(db_file, buf2, rec_count); 
	  break;
        }
      }
      fclose(db_file);
    }
    fscanf(index, "%s\n", buf1);
  }

  fclose(index);
  
  if (file_not_found) {
    alog("Removing invalid index in '%s/%s'", prefix, index_filename);
    sprintf(buf2, "%s/%s", prefix, index_filename);
    if (!(index = fopen(buf2, "r"))) {
      alog("SYSERR: Could not open file '%s' to rewrite index!", buf2);
      exit(1);
    }
     
    sprintf(new_index_name, "%s.new", buf2);
    if (!(new_index = fopen(new_index_name, "w"))) {
      alog("SYSERR: Could not open file '%s' to write new index!", new_index_name);
      exit(1);
    }
     
    fscanf(index, "%s\n", buf1);
    while (*buf1 != '$' && !feof(index))
    {
      fscanf(index, "%s\n", buf1);
      sprintf(buf2, "%s/%s", prefix, buf1);
      if (*buf1 != '$' && (db_file = fopen(buf2, "r")))
      {
        fprintf(new_index, "%s\n", buf1);
	fclose(db_file);
      }
    }
     
    fprintf(new_index, "$");
    fclose(index);
    fclose(new_index);
    sprintf(buf2, "%s/%s", prefix, index_filename);
    unlink(buf2);
    rename(new_index_name, buf2);
    
  } 
}


void discrete_load(FILE * fl, int mode)
{
  int nr = -1, last = 0;
  char line[256], temp[200];

  char *modes[] = {"world", "mob", "obj", "zone", "shp", "hlp", "qst", "pol"};

  for (;;) {
    /*
     * we have to do special processing with the obj files because they have
     * no end-of-record marker :(
     */
    if (mode != DB_BOOT_OBJ || nr < 0)
      if (!get_line(fl, line)) {
    sprintf(temp, "SYSERR: Format error after %s #%d\n", modes[mode], nr);
        mlog(temp);
    exit(1);
      }
    if (*line == '$')
      return;

    if (*line == '#') {
      last = nr;
      if (sscanf(line, "#%d", &nr) != 1) {
    sprintf(temp, "SYSERR: Format error after %s #%d\n", modes[mode], last);
        mlog(temp);
    exit(1);
      }
      if (nr >= 99999)
    return;
      else
    switch (mode) {
        case DB_BOOT_TRG:
          parse_trigger(fl, nr);
          break;
    case DB_BOOT_WLD:
      parse_room(fl, nr);
      break;
    case DB_BOOT_MOB:
      parse_mobile(fl, nr);
      break;
    case DB_BOOT_OBJ:
      strcpy(line, parse_object(fl, nr));
      break;
    }
    } else {
      fprintf(stderr, "Format error in %s file near %s #%d\n",
          modes[mode], modes[mode], nr);
      fprintf(stderr, "Offending line: '%s'\n", line);
      exit(1);
    }
  }
}


long asciiflag_conv(char *flag)
{
  long flags = 0;
  int is_number = 1;
  register char *p;

  for (p = flag; *p; p++) {
    if (islower(*p))
      flags |= 1 << (*p - 'a');
    else if (isupper(*p))
      flags |= 1 << (26 + (*p - 'A'));

    if (!isdigit(*p))
      is_number = 0;
  }

  if (is_number)
    flags = atol(flag);

  return flags;
}

char fread_letter(FILE *fp)
{
  char c;
  do {
   c = getc(fp);
  } while (isspace(c));
  return c;
}

/* load the rooms */
void parse_room(FILE * fl, int virtual_nr)
{
  static int room_nr = 0, zone = 0;
  int t[10], i = 0;
  char line[256], flags[128], temp[200];
  struct extra_descr_data *new_descr;

  room_nr = last_room_rnum;

  sprintf(buf2, "room #%d", virtual_nr);

  if (virtual_nr <= (zone ? zone_table[zone - 1].top : -1)) {
    sprintf(temp, "SYSERR: Room #%d is below zone %d.\n", virtual_nr, zone);
    mlog(temp);
    exit(1);
  }
  while (virtual_nr > zone_table[zone].top)
    if (++zone > top_of_zone_table) {
      sprintf(temp, "SYSERR: Room %d is outside of any zone.\n", virtual_nr);
      mlog(temp);
      exit(1);
    }
  world[room_nr].zone = zone;
  world[room_nr].number = virtual_nr;
  world[room_nr].name = fread_string(fl, buf2);
  if (!(world[room_nr].name))
  {
    CREATE(world[room_nr].name, char, 20);
    strcpy(world[room_nr].name, "Error text\r\n");
  }
  world[room_nr].description = fread_string(fl, buf2);
  if (!(world[room_nr].description))
  {
    CREATE(world[room_nr].description, char, 20);
    strcpy(world[room_nr].description, "Error text\r\n");
  }
  if (!get_line(fl, line) || sscanf(line, " %d %s %d ", t, flags, t + 2) != 3) {
    sprintf(temp, "SYSERR: Format error in room #%d\n", virtual_nr);
    mlog(temp);
    exit(1);
  }
  /* t[0] is the zone number; ignored with the zone-file system */
  world[room_nr].room_flags = asciiflag_conv(flags);
  world[room_nr].sector_type = t[2];
  world[room_nr].temp_flags = 0;

  world[room_nr].func = NULL;
  world[room_nr].contents = NULL;
  world[room_nr].people = NULL;
  world[room_nr].light = 0;	/* Zero light sources */

  for (i = 0; i < NUM_OF_DIRS; i++)
    world[room_nr].dir_option[i] = NULL;

  world[room_nr].ex_description = NULL;
 
  sprintf(buf, "Format error in room #%d (expecting D/E/S/P)", virtual_nr);

  for (;;) {
    if (!get_line(fl, line)) {
      fprintf(stderr, "%s\n", buf);
      exit(1);
    }
    switch (*line) {
    case 'D':
      setup_dir(fl, room_nr, atoi(line + 1));
      break;
    case 'P':
      alog("SYSERR: Attempt to parse roomprog (# %d) although no roomprogs should exist!",virtual_nr);
      break;
    case 'E':
      CREATE(new_descr, struct extra_descr_data, 1);
      new_descr->keyword = fread_string(fl, buf2);
      new_descr->description = fread_string(fl, buf2);
      new_descr->next = world[room_nr].ex_description;
      world[room_nr].ex_description = new_descr;
      break;
    case 'S':			/* end of room */
//      alog("ROOMLoad: room %d (vnum %d) loaded ok.",room_nr,virtual_nr);
      top_of_world = room_nr++;
      last_room_rnum = room_nr;
      return;
      break;
    default:
      fprintf(stderr, "%s\n", buf);
      exit(1);
      break;
    }
  }
}

/* read direction data */
void setup_dir(FILE * fl, int room, int dir)
{
  int t[5];
  char line[256];

  sprintf(buf2, "room #%d, direction D%d", world[room].number, dir);

  CREATE(world[room].dir_option[dir], struct room_direction_data, 1);
  world[room].dir_option[dir]->general_description = fread_string(fl, buf2);
  world[room].dir_option[dir]->keyword = fread_string(fl, buf2);

  if (!get_line(fl, line)) {
    fprintf(stderr, "Format error, %s\n", buf2);
    exit(1);
  }
  if (sscanf(line, " %d %d %d ", t, t + 1, t + 2) != 3) {
    fprintf(stderr, "Format error, %s\n", buf2);
    exit(1);
  }
  world[room].dir_option[dir]->exit_info = 0;  
  
  if (t[0] > 3) {
    t[0] -= 4;
    SET_BIT(world[room].dir_option[dir]->exit_info, EX_HIDDEN);
  }
  if (t[0] == 1)
    SET_BIT(world[room].dir_option[dir]->exit_info, EX_ISDOOR);
  else if (t[0] == 2)
    SET_BIT(world[room].dir_option[dir]->exit_info, EX_ISDOOR | EX_PICKPROOF);

  world[room].dir_option[dir]->key = t[1];
  world[room].dir_option[dir]->to_room = t[2];
}

/* make sure the start rooms exist & resolve their vnums to rnums */
void check_start_rooms(void)
{
  if ((r_mortal_start_room = real_room(mortal_start_room)) < 0) {
    mlog("SYSERR:  Mortal start room does not exist.  Change in config.c.");
    exit(1);
  }
  if ((r_immort_start_room = real_room(immort_start_room)) < 0) {
    if (!mini_mud)
      mlog("SYSERR:  Warning: Immort start room does not exist.  Change in config.c.");
    r_immort_start_room = r_mortal_start_room;
  }
  if ((r_frozen_start_room = real_room(frozen_start_room)) < 0) {
    if (!mini_mud)
      mlog("SYSERR:  Warning: Frozen start room does not exist.  Change in config.c.");
    r_frozen_start_room = r_mortal_start_room;
  }
}


/* resolve all vnums into rnums in the world */
void renum_world(void)
{
  register int room, door;

  for (room = 0; room <= top_of_world; room++)
    for (door = 0; door < NUM_OF_DIRS; door++)
      if (world[room].dir_option[door])
    if (world[room].dir_option[door]->to_room != NOWHERE)
      world[room].dir_option[door]->to_room =
        real_room(world[room].dir_option[door]->to_room);
}


#define ZCMD zone_table[zone].cmd[cmd_no]

/* resulve vnums into rnums in the zone reset tables */
void renum_zone_table(void)
{
  int zone, cmd_no, a, b;

  for (zone = 0; zone <= top_of_zone_table; zone++)
    for (cmd_no = 0; ZCMD.command != 'S'; cmd_no++) {
      a = b = 0;
      switch (ZCMD.command) {
      case 'M':
    a = ZCMD.arg1 = real_mobile(ZCMD.arg1);
    b = ZCMD.arg3 = real_room(ZCMD.arg3);
    break;
      case 'O':
    a = ZCMD.arg1 = real_object(ZCMD.arg1);
    if (ZCMD.arg3 != NOWHERE)
      b = ZCMD.arg3 = real_room(ZCMD.arg3);
    break;
      case 'G':
    a = ZCMD.arg1 = real_object(ZCMD.arg1);
    break;
      case 'E':
    a = ZCMD.arg1 = real_object(ZCMD.arg1);
    break;
      case 'P':
    a = ZCMD.arg1 = real_object(ZCMD.arg1);
    b = ZCMD.arg3 = real_object(ZCMD.arg3);
    break;
      case 'D':
    a = ZCMD.arg1 = real_room(ZCMD.arg1);
    break;
      case 'R': /* rem obj from room */
        a = ZCMD.arg1 = real_room(ZCMD.arg1);
    b = ZCMD.arg2 = real_object(ZCMD.arg2);
        break;
      }
      if (a < 0 || b < 0) {
    if (!mini_mud)
      log_zone_error(zone, cmd_no, "Invalid vnum, cmd disabled");
    ZCMD.command = '*';
      }
    }
}



void parse_simple_mob(FILE *mob_f, int i, int nr)
{
  int j, t[20];
  char line[256], temp[200];

    mob_proto[i].real_abils.str = 18;
    mob_proto[i].real_abils.intel = 18;
    mob_proto[i].real_abils.wis = 18;
    mob_proto[i].real_abils.dex = 18;
    mob_proto[i].real_abils.con = 18;
    mob_proto[i].real_abils.cha = 18;

    get_line(mob_f, line);
    if (sscanf(line, " %d %d %d %d %dd%d+%d %dd%d+%d ",
      t, t + 1, t + 2, t + 3, t + 4, t + 5, t + 6, t + 7, t + 8,t + 9)!=10) {
      sprintf(temp,"Format error in mob #%d, first line after S flag\n"
          "...expecting line of form '# # # # #d#+# #d#+#'\n", nr);
      mlog(temp);
      exit(1);
    }
    GET_LEVEL(mob_proto + i) = t[0];
    mob_proto[i].points.hitroll = 20 - t[1];
    mob_proto[i].points.armor = 10 * t[2];

  /*  GET_ABS(&mob_proto[i]) = t[3];  */

    /* max hit = 0 is a flag that H, M, V is xdy+z */
    mob_proto[i].points.max_hit = 0;
    mob_proto[i].mob_specials.hpnodice   = mob_proto[i].points.hit = t[4];
    mob_proto[i].mob_specials.hpsizedice = mob_proto[i].points.mana = t[5]; // Inna future, with xml, only mob_psec will be used. 
    mob_proto[i].mob_specials.hpbonus    = mob_proto[i].points.move = t[6];

    mob_proto[i].mob_specials.mananodice   = 4;
    mob_proto[i].mob_specials.manasizedice = 10;
    mob_proto[i].mob_specials.manabonus    = 10;

    mob_proto[i].mob_specials.movenodice   = 10;
    mob_proto[i].mob_specials.movesizedice = 20;
    mob_proto[i].mob_specials.movebonus    = 100;

    mob_proto[i].points.max_mana = 10;
    mob_proto[i].points.max_move = 50;

    mob_proto[i].mob_specials.damnodice = t[7];
    mob_proto[i].mob_specials.damsizedice = t[8];
    mob_proto[i].points.damroll = t[9];

    get_line(mob_f, line);
    sscanf(line, " %d %d ", t, t + 1);
    GET_GOLD(mob_proto + i) = t[0];
    GET_EXP(mob_proto + i) = t[1];

    get_line(mob_f, line);
    if (sscanf(line, " %d %d %d %d ", t, t + 1, t + 2, t + 3) < 3) {
      sprintf(temp, "Format error in mob #%d, second line after S flag\n"
          "...expecting line of form '# # #'\n", nr);
      mlog(temp);
    }

    mob_proto[i].char_specials.position = t[0];
    mob_proto[i].mob_specials.default_pos = t[1];
    mob_proto[i].player.sex = t[2];
    mob_proto[i].mob_specials.timer = t[3];

    mob_proto[i].player.class = 0;
    mob_proto[i].player.weight = 200;
    mob_proto[i].player.height = 198;

    for (j = 0; j < 3; j++)
      GET_COND(mob_proto + i, j) = -1;
}


/*
 * interpret_espec is the function that takes espec keywords and values
 * and assigns the correct value to the mob as appropriate.  Adding new
 * e-specs is absurdly easy -- just add a new CASE statement to this
 * function!  No other changes need to be made anywhere in the code.
 */

#define CASE(test) if (!matched && !str_cmp(keyword, test) && (matched = 1))
#define RANGE(low, high) (num_arg = MAX((low), MIN((high), (num_arg))))

void interpret_espec(char *keyword, char *value, int i, int nr)
{
  int num_arg, matched = 0;
  char temp[200];

  num_arg = atoi(value);

  CASE("BareHandAttack") {
    RANGE(0, 99);
    mob_proto[i].mob_specials.attack_type = num_arg;
  }

  CASE("Str") {
    RANGE(3, 25);
    mob_proto[i].real_abils.str = num_arg;
  }

  CASE("StrAdd") {
    alog("Parsing mob %d [%d]: str_add no longer in use!",nr,i);
  }

  CASE("Int") {
    RANGE(3, 25);
    mob_proto[i].real_abils.intel = num_arg;
  }

  CASE("Wis") {
    RANGE(3, 25);
    mob_proto[i].real_abils.wis = num_arg;
  }

  CASE("Dex") {
    RANGE(3, 25);
    mob_proto[i].real_abils.dex = num_arg;
  }

  CASE("Con") {
    RANGE(3, 25);
    mob_proto[i].real_abils.con = num_arg;
  }

  CASE("Cha") {
    RANGE(3, 25);
    mob_proto[i].real_abils.cha = num_arg;
  }

  CASE("Bash") 
    SET_BIT(mob_proto[i].mob_specials.skillvector, MOB_BASH); 

  CASE("Trip") 
    SET_BIT(mob_proto[i].mob_specials.skillvector, MOB_TRIP); 
    
  CASE("Dirtkick") 
    SET_BIT(mob_proto[i].mob_specials.skillvector, MOB_DIRTKICK); 
    
  CASE("Kick") 
    SET_BIT(mob_proto[i].mob_specials.skillvector, MOB_KICK); 

  CASE("Disarm") 
    SET_BIT(mob_proto[i].mob_specials.skillvector, MOB_DISARM); 

  CASE("Stun") 
    SET_BIT(mob_proto[i].mob_specials.skillvector, MOB_STUN); 
  
  if (!matched) {
    sprintf(temp, "Warning: unrecognized espec keyword %s in mob #%d\n",
        keyword, nr);
    mlog(temp);
  }    
}

#undef CASE
#undef RANGE

void parse_espec(char *buf, int i, int nr)
{
  char *ptr;

  if ((ptr = strchr(buf, ':')) != NULL) {
    *(ptr++) = '\0';
    while (isspace(*ptr))
      ptr++;
  } else
    ptr = "";

  interpret_espec(buf, ptr, i, nr);
}

void parse_enhanced_mob(FILE *mob_f, int i, int nr)
{
  char line[256];

  parse_simple_mob(mob_f, i, nr);

  while (get_line(mob_f, line)) {
    if (!strcmp(line, "E"))	/* end of the ehanced section */
      return;
    else if (!strcmp(line, "P"))/* Mob program exists, let's parse it */
      parse_mobprog(mob_f, i, nr);
    else if (*line == '#') {	/* we've hit the next mob, maybe? */
      alog("SYSERR: Unterminated E section in mob #%d\n", nr);
      exit(1);
    } else
      parse_espec(line, i, nr);
  }

  fprintf(stderr, "Unexpected end of file reached after mob #%d\n", nr);
  exit(1);
}


void parse_mobile(FILE * mob_f, int nr)
{
  static int i = 0;
  int j, t[10]; /*k,toadd;*/
  char line[256], *tmpptr, letter;
  char f1[128], f2[128];

  i = last_mob_rnum;

  mob_index[i].virtual = nr;
  mob_index[i].number = 0;
  mob_index[i].func = NULL;
 
  clear_char(mob_proto + i);

  mob_proto[i].player_specials = &dummy_mob;
  sprintf(buf2, "mob vnum %d", nr);

  /***** String data *** */
  mob_proto[i].player.name = fread_string(mob_f, buf2);
  tmpptr = mob_proto[i].player.short_descr = fread_string(mob_f, buf2);
  if (tmpptr && *tmpptr)
    if (!str_cmp(fname(tmpptr), "a") || !str_cmp(fname(tmpptr), "an") ||
    !str_cmp(fname(tmpptr), "the"))
      *tmpptr = LOWER(*tmpptr);
  mob_proto[i].player.long_descr = fread_string(mob_f, buf2);
  mob_proto[i].player.description = fread_string(mob_f, buf2);
  mob_proto[i].player.title = NULL;
  mob_proto[i].mprog = NULL;
  /* *** Numeric data *** */
  get_line(mob_f, line);
  sscanf(line, "%s %s %d %c", f1, f2, t + 2, &letter);
  MOB_FLAGS(mob_proto + i) = asciiflag_conv(f1);
  SET_BIT(MOB_FLAGS(mob_proto + i), MOB_ISNPC);
  AFF_FLAGS(mob_proto + i) = asciiflag_conv(f2);

  switch (letter) {
  case 'S':	/* Simple monsters */
    parse_simple_mob(mob_f, i, nr);
    break;
  case 'E':	/* Circle3 Enhanced monsters */
    parse_enhanced_mob(mob_f, i, nr);
    break;
  /* add new mob types here.. */
  default:
    fprintf(stderr, "Unsupported mob type '%c' in mob #%d\n", letter, nr);
    exit(1);
    break;
  }

  mob_proto[i].aff_abils = mob_proto[i].real_abils;

  for (j = 0; j < NUM_WEARS; j++)
    mob_proto[i].equipment[j] = NULL;

  mob_proto[i].nr = i;
  mob_proto[i].desc = NULL;
  //alog("MOBLoad: Mob %d (Vnum: %d) parsed succesfully.",i,GET_MOB_VNUM(&mob_proto[i])); 

  top_of_mobt = i++;
  last_mob_rnum = i;
}

/* read all objects from obj file; generate index and prototypes */
char *parse_object(FILE * obj_f, int nr)
{
  static int i = 0, retval;
  static char line[256];
  int t[10], j = 0;
  char *tmpptr;
  char f1[256], f2[256];
  struct extra_descr_data *new_descr;

  i = last_obj_rnum;

  obj_index[i].virtual = nr;
  obj_index[i].number = 0;
  obj_index[i].func = NULL;

  clear_object(obj_proto + i);
  obj_proto[i].in_room = NOWHERE;
  obj_proto[i].item_number = i;
  obj_proto[i].oprog = NULL;

  sprintf(buf2, "object #%d", nr);

  /* *** string data *** */
  if ((obj_proto[i].name = fread_string(obj_f, buf2)) == NULL) {
    fprintf(stderr, "Null obj name or format error at or near %s\n", buf2);
    exit(1);
  }
  tmpptr = obj_proto[i].short_description = fread_string(obj_f, buf2);
  if (tmpptr && *tmpptr)
    if (!str_cmp(fname(tmpptr), "a") || !str_cmp(fname(tmpptr), "an") ||
    !str_cmp(fname(tmpptr), "the"))
      *tmpptr = LOWER(*tmpptr);

  tmpptr = obj_proto[i].description = fread_string(obj_f, buf2);
  if (tmpptr && *tmpptr)
    *tmpptr = UPPER(*tmpptr);
  obj_proto[i].action_description = fread_string(obj_f, buf2);

  /* *** numeric data *** */
  if (!get_line(obj_f, line) ||
      (retval = sscanf(line, " %d %s %s", t, f1, f2)) != 3) {
    fprintf(stderr, "Format error in first numeric line (expecting 3 args, got %d), %s\n", retval, buf2);
    exit(1);
  }
  obj_proto[i].obj_flags.type_flag = t[0];
  obj_proto[i].obj_flags.extra_flags = asciiflag_conv(f1);
  obj_proto[i].obj_flags.wear_flags = asciiflag_conv(f2);

  if (!get_line(obj_f, line) ||
      (retval = sscanf(line, "%d %d %d %d", t, t + 1, t + 2, t + 3)) < 3) {
    fprintf(stderr, "Format error in second numeric line (expecting 4 args, got %d), %s\n", retval, buf2);
    exit(1);
  }
  obj_proto[i].obj_flags.value[0] = t[0];
  obj_proto[i].obj_flags.value[1] = t[1];
  obj_proto[i].obj_flags.value[2] = t[2];
  obj_proto[i].obj_flags.value[3] = t[3];

  if (!get_line(obj_f, line) ||
      (retval = sscanf(line, "%d %d %d", t, t + 1, t + 2)) != 3) {
    fprintf(stderr, "Format error in third numeric line (expecting 3 args, got %d), %s\n", retval, buf2);
    exit(1);
  }
  obj_proto[i].obj_flags.weight = t[0];
  obj_proto[i].obj_flags.cost = t[1];
  obj_proto[i].obj_flags.questcost = t[2];

  /* check to make sure that weight of containers exceeds curr. quantity */
  if (obj_proto[i].obj_flags.type_flag == ITEM_DRINKCON ||
      obj_proto[i].obj_flags.type_flag == ITEM_FOUNTAIN) {
    if (obj_proto[i].obj_flags.weight < obj_proto[i].obj_flags.value[1])
      obj_proto[i].obj_flags.weight = obj_proto[i].obj_flags.value[1] + 5;
  }

  /* *** extra descriptions and affect fields *** */

  for (j = 0; j < MAX_OBJ_AFFECT; j++) {
    obj_proto[i].affected[j].location = APPLY_NONE;
    obj_proto[i].affected[j].modifier = 0;
  }

  strcat(buf2, ", after numeric constants (expecting E/A/#xxx)");
  j = 0;

  for (;;) {
    if (!get_line(obj_f, line)) {
      fprintf(stderr, "Format error in %s\n", buf2);
      exit(1);
    }
    switch (*line) {
    case 'E':
      CREATE(new_descr, struct extra_descr_data, 1);
      new_descr->keyword = fread_string(obj_f, buf2);
      new_descr->description = fread_string(obj_f, buf2);
      new_descr->next = obj_proto[i].ex_description;
      obj_proto[i].ex_description = new_descr;
      break;
    case 'A':
      if (j >= MAX_OBJ_AFFECT) {
    fprintf(stderr, "Too many A fields (%d max), %s\n", MAX_OBJ_AFFECT, buf2);
    exit(1);
      }
      get_line(obj_f, line);
      sscanf(line, " %d %d ", t, t + 1);
      obj_proto[i].affected[j].location = t[0];
      obj_proto[i].affected[j].modifier = t[1];
      j++;
      break;
    case 'P':
      parse_objprog(obj_f, i, nr);
      break;
    case '$':
    case '#':
//      alog("OBJLoad: Object %d (vnum %d) loaded ok.",i,GET_OBJ_VNUM(&obj_proto[i])); 
      top_of_objt = i++;
      last_obj_rnum = i;
      return line;
      break;
    default:
      fprintf(stderr, "Format error in %s\n", buf2);
      exit(1);
      break;
    }
  }
}

#define Z	zone_table[zone]

/* load the zone table and command tables */
void load_zones(FILE * fl, char *zonename)
{
  static int zone = 0;
  int cmd_no = 0, num_of_cmds = 0, line_num = 0, tmp, error;
  char *ptr, buf[256], zname[256];

  zone = last_zone_rnum;

  strcpy(zname, zonename);

  while (get_line(fl, buf))
    num_of_cmds++;		/* this should be correct within 3 or so */
  rewind(fl);

  if (num_of_cmds == 0) {
    fprintf(stderr, "%s is empty!\n", zname);
    exit(0);
  } else
    CREATE(Z.cmd, struct reset_com, num_of_cmds);

  line_num += get_line(fl, buf);

  if (sscanf(buf, "#%d", &Z.number) != 1) {
    fprintf(stderr, "Format error in %s, line %d\n", zname, line_num);
    exit(0);
  }
  sprintf(buf2, "beginning of zone #%d", Z.number);

  line_num += get_line(fl, buf);
  if ((ptr = strchr(buf, '~')) != NULL)	/* take off the '~' if it's there */
    *ptr = '\0';
  Z.name = str_dup(buf);

  line_num += get_line(fl, buf);
  if (sscanf(buf, " %d %d %d %d ", &Z.top, &Z.lifespan, &Z.reset_mode, &Z.lock) < 3) {
    fprintf(stderr, "Format error in 4-constant line of %s", zname);
    exit(0);
  }
  cmd_no = 0;

  for (;;) {
    if ((tmp = get_line(fl, buf)) == 0) {
      fprintf(stderr, "Format error in %s - premature end of file\n", zname);
      exit(0);
    }
    line_num += tmp;
    ptr = buf;
    skip_spaces(&ptr);

    if ((ZCMD.command = *ptr) == '*')
      continue;

    ptr++;

    if (ZCMD.command == 'S' || ZCMD.command == '$') {
      ZCMD.command = 'S';
      break;
    }
    error = 0;
    if (strchr("MOEPD", ZCMD.command) == NULL) {	/* a 3-arg command */
      if (sscanf(ptr, " %d %ld %ld ", &tmp, &ZCMD.arg1, &ZCMD.arg2) != 3)
    error = 1;
    } else {
      if (sscanf(ptr, " %d %ld %ld %ld ", &tmp, &ZCMD.arg1, &ZCMD.arg2,
         &ZCMD.arg3) != 4)
    error = 1;
    }

    ZCMD.if_flag = tmp;

    if (error) {
      fprintf(stderr, "Format error in %s, line %d: '%s'\n", zname, line_num, buf);
      exit(0);
    }
    ZCMD.line = line_num;
    ZCMD.percentage = 100; // Default to 100% for 'old' style zones. 
    cmd_no++;
  }
//  alog("ZONLoad: zone %d (vnum %d loaded ok.",zone,zone_table[zone].number);
  top_of_zone_table = zone++;
  last_zone_rnum = zone;
}

#undef Z


void get_one_line(FILE *fl, char *buf)
{
  if (fgets(buf, READ_SIZE, fl) == NULL) {
    mlog("error reading help|policy file: not terminated with $?");
    exit(1);
  }

  buf[strlen(buf) - 1] = '\0'; /* take off the trailing \n */
}

void update_help_index() {
  int i, comp = 1;
  struct help_indexer_element *el, *el2;
  char *keywords, key[READ_SIZE + 1], *rec;

  mlog("Sorting help entries");
  
  if (!help_table) {
    help_index = NULL;
    return;
  }
  
  while(help_index) {
    el = help_index;
    help_index = help_index->next;
    el->next = NULL;
    free(el->keyword);
    free(el);
  }

  for (i = 0;i < top_of_helpt;i++) {
    rec = keywords = str_dup(help_table[i].keywords);
    while (keywords && *keywords) {
      keywords = one_word(keywords, key);
      if (key) {
	CREATE( el, struct help_indexer_element, 1 );
        el->keyword = str_dup(key);
        el->pos = i;
        if (!help_index) {
          el->next = NULL;
          help_index = el;
          continue;
        }
        if (str_cmp(el->keyword, help_index->keyword) < 0) {
          el->next = help_index;
          help_index = el;
          continue;
        }
        for(el2 = help_index;el2->next;el2 = el2->next) {
          comp = str_cmp(el->keyword, el2->next->keyword);
          if (comp < 0) break;
        }
        el->next = el2->next;
        el2->next = el;
      }
    }
    free(rec);
  }
}

void update_poli_index() {
  int i, comp = 1;
  struct poli_indexer_element *el, *el2;
  char *keywords, key[READ_SIZE + 1];

  mlog("Sorting policy entries");
  
  if (!poli_table) {
    poli_index = NULL;
    return;
  }
  
  while(poli_index) {
    el = poli_index;
    poli_index = poli_index->next;
    el->next = NULL;
    free(el->keyword);
    free(el);
  }

  for (i = 0;i < top_of_polit;i++) {
    keywords = str_dup(poli_table[i].keywords);
    while (keywords && *keywords) {
      keywords = one_word(keywords, key);
      if (key) {
	CREATE( el, struct poli_indexer_element, 1 );
        el->keyword = str_dup(key);
        el->pos = i;
        if (!poli_index) {
          el->next = NULL;
          poli_index = el;
          continue;
        }
        if (str_cmp(el->keyword, poli_index->keyword) < 0) {
          el->next = poli_index;
          poli_index = el;
          continue;
        }
        for(el2 = poli_index;el2->next;el2 = el2->next) {
          comp = str_cmp(el->keyword, el2->next->keyword);
          if (comp < 0) break;
        }
        el->next = el2->next;
        el2->next = el;
      }
    }
  }
}


void load_help(FILE *fl)
{
  char key[READ_SIZE+1], entry[32384];
  char line[READ_SIZE+1];
  struct help_index_element el;

  /* get the keyword line */
  get_one_line(fl, key);
  while (*key != '$') {
    get_one_line(fl, line);
    *entry = '\0';
    while (*line != '#') {
      if (strlen(entry) + strlen(line) + 1 > MAX_STRING_LENGTH) {
        sprintf(buf, "SYSERR: help entry too long: \'%s\', truncated (%d max)", key, MAX_STRING_LENGTH);
        mlog(buf);
        while (*line != '#') get_one_line(fl, line);
        break;
      }
      strcat(entry, strcat(line, "\r\n"));
      get_one_line(fl, line);
    }


    el.min_level = 0;
    if ((*line == '#') && (*(line + 1) != 0))
      el.min_level = atoi((line + 1));

    el.min_level = MAX(0, MIN(el.min_level, LVL_IMPL));

    /* now, add the entry to the index with each keyword on the keyword line */
    el.entry = str_dup(entry);
    el.keywords = str_dup(key);

    help_table[top_of_helpt] = el;
    top_of_helpt++;

    /* get next keyword line (or $) */
    get_one_line(fl, key);
  }
  update_help_index();
}

void load_poli(FILE *fl)
{
  char key[READ_SIZE+1], entry[32384];
  char line[READ_SIZE+1];
  struct poli_index_element el;

  /* get the keyword line */
  get_one_line(fl, key);
  while (*key != '$') {
    get_one_line(fl, line);
    *entry = '\0';
    while (*line != '#') {
      if (strlen(entry) + strlen(line) + 1 > MAX_STRING_LENGTH) {
        sprintf(buf, "SYSERR: policy entry too long: \'%s\', truncated (%d max)", key, MAX_STRING_LENGTH);
        mlog(buf);
        while (*line != '#') get_one_line(fl, line);
        break;
      }
      strcat(entry, strcat(line, "\r\n"));
      get_one_line(fl, line);
    }


    el.min_level = 0;
    if ((*line == '#') && (*(line + 1) != 0))
      el.min_level = atoi((line + 1));

    el.min_level = MAX(0, MIN(el.min_level, LVL_IMPL));

    /* now, add the entry to the index with each keyword on the keyword line */
    el.entry = str_dup(entry);
    el.keywords = str_dup(key);

    poli_table[top_of_polit] = el;
    top_of_polit++;

    /* get next keyword line (or $) */
    get_one_line(fl, key);
  }
  update_poli_index();
}


/*************************************************************************
*  procedures for resetting, both play-time and boot-time	 	 *
*********************************************************************** */



int vnum_mobile(char *searchname, struct char_data * ch)
{
  int nr, found = 0;

  for (nr = 0; nr <= top_of_mobt; nr++) 
  {
    if ((mob_proto[nr].player.name) && isname(searchname, mob_proto[nr].player.name)) 
    {
      sprintf(buf, "%3d. [%5ld] %s\r\n", ++found,
          mob_index[nr].virtual,
          mob_proto[nr].player.short_descr);
      send_to_char(buf, ch);
    }
  }

  return (found);
}



int vnum_object(char *searchname, struct char_data * ch)
{
  int nr, found = 0;

  for (nr = 0; nr <= top_of_objt; nr++) {
    if (isname(searchname, obj_proto[nr].name)) {
      sprintf(buf, "%3d. [%5ld] %s\r\n", ++found,
          obj_index[nr].virtual,
          obj_proto[nr].short_description);
      send_to_char(buf, ch);
    }
  }
  return (found);
}


int vnum_room( char *searchname, struct char_data * ch )
{
  int nr, found = 0;

  for( nr = 0; nr <= top_of_world; nr++ )
  {
    if( isname( searchname, world[ nr ].name ) )
    {
      sprintf( buf, "%3d. [%5d] %s\r\n", ++found,
          world[ nr ].number,
          world[ nr ].name );
      send_to_char( buf, ch );
    }
  }
  return (found);
}

int do_vnum_flags(struct char_data *ch, int which, char *flag)
{
  char *upper_ptr;
  int  bit_number;
  
  skip_spaces(&flag);
  upper_ptr = flag;
    
  while (*upper_ptr && !isspace(*upper_ptr))
  {
    *upper_ptr = UPPER(*upper_ptr);
    upper_ptr++;
  }
  
  /* search flags to get the value */
  switch(which)
  {
    case 0 : bit_number = search_block(flag, action_bits, 1);
             break;
    case 1 : bit_number = search_block(flag, extra_bits, 1) ;
             break;
    case 2 : bit_number = search_block(flag, room_bits, 1);
             break;
    default : /* This should never happen -Tesla */
              sprintf(buf, "Unknown value for which(%d) in do_vnum_flags().",
                      which);
              mlog(buf);
              return 1;
              break;
  }
  
  if (bit_number == -1)
  {
    send_to_char("Unknown flag.\r\n", ch);
    return 1;
  }
   
  if (bit_number > 32)
  {
    sprintf(buf, "Weird flag value (%d) for flag %s. In do_vnum_flags.",
            bit_number, flag);
    mlog(buf);
    send_to_char("Weird flag value, notify a weaver asap.\r\n", ch);
    return 1;
  }
   
  if (which == 0)
    if (!vnum_mobile_flag((1 << bit_number), ch))
      send_to_char("No mobiles with that flag.\r\n", ch);
      
  if (which == 1)
    if (!vnum_object_flag((1 << bit_number), ch))
      send_to_char("No objects with that flag.\r\n", ch);
      
  if (which == 2)
    if (!vnum_room_flag((1<< bit_number), ch))
      send_to_char("No rooms with that flag.\r\n", ch);

  return 0;
}

int vnum_mobile_flag(int flag, struct char_data *ch)
{
  int nr, found = 0;
  
  for (nr = 0; nr <= top_of_mobt; nr++)
  {
    if (IS_SET(mob_proto[nr].char_specials.saved.act, flag))
    {
      sprintf(buf, "%3d. [%5ld] %s\r\n", ++found,
              mob_index[nr].virtual,
              mob_proto[nr].player.short_descr);
      send_to_char(buf, ch);
    }
  }
  return (found);
}
 
int vnum_object_flag(int flag, struct char_data *ch)
{
  int nr, found = 0;
  
  for (nr = 0; nr <= top_of_objt; nr++)
  {
    if (IS_SET(obj_proto[nr].obj_flags.extra_flags, flag))
    {
      sprintf(buf, "%3d. [%5ld] %s\r\n", ++found,
              obj_index[nr].virtual,
              obj_proto[nr].short_description);
      send_to_char(buf, ch);
    }
  }
  return (found);
}
              
int vnum_room_flag(int flag, struct char_data *ch)
{
  int nr, found = 0;
   
  for(nr = 0; nr <= top_of_world; nr++)
  {
    if(IS_SET(ROOM_FLAGS(nr), flag))
    {
      sprintf( buf, "%3d. [%5d] %s\r\n", ++found,
              world[ nr ].number,
              world[ nr ].name );
      send_to_char( buf, ch );
    }
    if(IS_SET(ROOM_TEMP_FLAGS(nr), flag))
    {
      sprintf( buf, "(temp) %3d. [%5d] %s\r\n", ++found,
              world[ nr ].number,
              world[ nr ].name );
      send_to_char( buf, ch );
    }
  }  
  return (found);
}

/* create a character, and add it to the char list */
struct char_data *create_char(void)
{
//  char debug[255];
  struct char_data *ch;

  CREATE(ch, struct char_data, 1);
  clear_char(ch);
  ch->next = character_list;
  character_list = ch;
//  sprintf( debug, "Added character at 0x%lX", ch );
//  xlog( SYS_DEBUG, 0, debug, TRUE );
  GET_ID(ch) = max_id++;

  return ch;
}


/* create a new mobile from a prototype */
struct char_data *read_mobile(int nr, int type)
{
  int i, virtual;
  struct char_data *mob;

  if (type == VIRTUAL) {
    if ((i = real_mobile(nr)) < 0) {
      alog("SYSERR: Mobile (V) %d does not exist in database.", nr);
      return (0);
    }
    virtual = nr;
  } else {
    i = nr;
    virtual = mob_index[i].virtual;
  }

  CREATE(mob, struct char_data, 1);
  clear_char(mob);
  *mob = mob_proto[i];
  mob->next = character_list;
  character_list = mob;

  /**
   * New version of hit/mana/move calcs. :) 
   */
  mob->points.max_hit = dice(mob->mob_specials.hpnodice,mob->mob_specials.hpsizedice)+mob->mob_specials.hpbonus;
  mob->points.hit = mob->points.max_hit;
  mob->points.max_move = dice(mob->mob_specials.movenodice,mob->mob_specials.movesizedice)+mob->mob_specials.movebonus;
  mob->points.move = mob->points.max_move;
  mob->points.max_mana = dice(mob->mob_specials.mananodice,mob->mob_specials.manasizedice)+mob->mob_specials.manabonus;
  mob->points.mana = mob->points.max_mana;
  
  mob->player.time.birth = time(0);
  mob->player.time.played = 0;
  mob->player.time.logon = time(0);

  mob_index[i].number++;

  /* DGS */
  GET_ID(mob) = max_id++;
  assign_triggers(mob, MOB_TRIGGER);

  return mob;
}


/* create an object, and add it to the object list */
struct obj_data *create_obj(void)
{
  struct obj_data *obj;

  CREATE(obj, struct obj_data, 1);
  clear_object(obj);
  obj->next = object_list;
  object_list = obj;

  GET_ID(obj) = max_id++;
  assign_triggers(obj, OBJ_TRIGGER);

  return obj;
}


/* create a new object from a prototype */
struct obj_data *read_object(int nr, int type)
{
  struct obj_data *obj;
  int i, virtual;

  if (nr < 0) {
    mlog("SYSERR: trying to create obj with negative num!");
    return NULL;
  }
  if (type == VIRTUAL) {
    if ((i = real_object(nr)) < 0) {
      sprintf(buf, "SYSERR: Object (V) %d does not exist in database.", nr);
      mlog(buf);
      return NULL;
    }
    virtual = nr;
  } else {
    i = nr;
    virtual = obj_index[i].virtual;
  }

  CREATE(obj, struct obj_data, 1);
  clear_object(obj);
  *obj = obj_proto[i];
  obj->next = object_list;
  object_list = obj;

  obj_index[i].number++;

  /* DGS */
  GET_ID(obj) = max_id++;
  assign_triggers(obj, OBJ_TRIGGER);

  return obj;
}

/* Clones an object, with the exception that's put in nowhere. */

struct obj_data *clone_obj(struct obj_data *original)
{
  struct obj_data *local;
  int i;

  if (!original)
    return 0;

  if (IS_SET(GET_OBJ_EXTRA(original), ITEM_UNIQUE)) 
    return NULL;

  // read_object() also copies over objprogs and triggers.
  if (GET_OBJ_RNUM(original) >= 0)
    local = read_object(GET_OBJ_VNUM(original), VIRTUAL);
  else {
    CREATE(local, struct obj_data, 1);
    clear_object(local);
  }
  // Now for the boring work...hand copying it all.
  // It must be done, since this clone also is capable of cloning
  // objects that have no prototypes...
  local->item_number = original->item_number;
  // It's a bit more efficient that copying all those variables...
  memcpy(&(local->obj_flags), &(original->obj_flags), sizeof(struct obj_flag_data));
  for (i = 0; i < MAX_OBJ_AFFECT; i ++) {
    local->affected[i].location = original->affected[i].location;
    local->affected[i].modifier = original->affected[i].modifier;
  }
    if (original->name)
      local->name = strdup(original->name);
    else
      local->name = strdup("nothing");

    if (original->description)
      local->description = strdup(original->description);
    else
      local->description = strdup("a nothing is here");
    if (original->short_description)
      local->short_description = strdup(original->short_description);
    else
      local->short_description = strdup("a nothing");
     if (original->action_description)
       local->action_description = strdup(original->action_description);
    else
      local->action_description =NULL;
  local->pulled_by = NULL;
  local->carried_by = NULL;
  local->worn_by = NULL;
  local->worn_on = 0;
  local->mana_add = original->mana_add;
  local->obj_status = original->obj_status;
  local->paused_at = NULL; // Can't be active, it was just created!
  local->pause = 0;
  local->op_activated = 0;
  local->linked = NULL; // If Gate is cloned, it still won't be linked.
  local->in_obj = NULL;
  local->contains = NULL;  // do -not- clone contents.
  local->next_content = NULL;
  for (i = 0; i < 10; i ++) 
   local->status[i] = original->status[i];
  if (!local->next) { // No prototype -> isn't put in the obj list, must fix
    local->next = object_list;
    object_list = local;
  }
  return local;
}

#define ZO_DEAD  999

/* update zone ages, queue for reset if necessary, and dequeue when possible */
void zone_update(void)
{
  int i;
  struct reset_q_element *update_u, *temp;
  static int timer = 0;
  char buf[128];

  /* jelson 10/22/92 */
  if (((++timer * PULSE_ZONE) / PASSES_PER_SEC) >= 60) {
    /* one minute has passed */
    /*
     * NOT accurate unless PULSE_ZONE is a multiple of PASSES_PER_SEC or a
     * factor of 60
     */

    timer = 0;

    /* since one minute has passed, increment zone ages */
    for (i = 0; i <= top_of_zone_table; i++) {
      if (zone_table[i].age < zone_table[i].lifespan &&
      zone_table[i].reset_mode)
    (zone_table[i].age)++;

      if (zone_table[i].age >= zone_table[i].lifespan &&
      zone_table[i].age < ZO_DEAD && zone_table[i].reset_mode) {
    /* enqueue zone */

    CREATE(update_u, struct reset_q_element, 1);

    update_u->zone_to_reset = i;
    update_u->next = 0;

    if (!reset_q.head)
      reset_q.head = reset_q.tail = update_u;
    else {
      reset_q.tail->next = update_u;
      reset_q.tail = update_u;
    }

    zone_table[i].age = ZO_DEAD;
      }
    }
  }	/* end - one minute has passed */


  /* dequeue zones (if possible) and reset */
  /* this code is executed every 10 seconds (i.e. PULSE_ZONE) */
  for (update_u = reset_q.head; update_u; update_u = update_u->next)
    if (zone_table[update_u->zone_to_reset].reset_mode == 2 ||
    is_empty(update_u->zone_to_reset)) {
      reset_zone(update_u->zone_to_reset);
      sprintf(buf, "Auto zone reset: %s",
          zone_table[update_u->zone_to_reset].name);
      xlog(SYS_ZRESET, LVL_IMMORT, buf, FALSE);
      /* dequeue */
      if (update_u == reset_q.head)
    reset_q.head = reset_q.head->next;
      else {
    for (temp = reset_q.head; temp->next != update_u;
         temp = temp->next);

    if (!update_u->next)
      reset_q.tail = temp;

    temp->next = update_u->next;
      }

      free(update_u);
      break;
    }
}

void log_zone_error(int zone, int cmd_no, char *message)
{
  char buf[256*8];

  sprintf(buf, "SYSERR: error in zone file: %s", message);
  mudlog(buf, CMP, LVL_IMMORT, TRUE);

  sprintf(buf, "SYSERR: ...offending cmd in zone #%d, line %ld.",
     zone_table[zone].number, ZCMD.line);
  mudlog(buf, CMP, LVL_IMMORT, TRUE);

}

static int check_percentage(int p) {
  int c;

  if (p <= 0 || p >= 100) // Out of bounds, always return true. 
    return 1;

  c = number(1,100);
  if (c <= p)
    return 1;

  return 0;
}

#define ZONE_ERROR(message) \
    { log_zone_error(zone, cmd_no, message); last_cmd = 0; }

/* execute the reset command table of a given zone */
void reset_zone(int zone)
{
  int cmd_no, last_cmd = 0, rnum =-1, rnum2 = -1;
  struct char_data *mob = NULL;
  struct obj_data *obj, *obj_to;
  struct room_data *rm = NULL, *toroom = NULL;
  struct room_direction_data *door = NULL, *todoor = NULL;
  int room_vnum, room_rnum;
  struct char_data *tmob=NULL; /* for trigger assignment */
  struct obj_data *tobj=NULL;  /* for trigger assignment */

  for (cmd_no = 0; ZCMD.command != 'S'; cmd_no++) {

    if (ZCMD.if_flag && !last_cmd)
      continue;

    switch (ZCMD.command) {
    case '*':			/* ignore command */
      last_cmd = 0;
      break;

    case 'M':			/* read a mobile */
      if ((rnum = real_mobile(ZCMD.arg1)) < 0) {
        sprintf(buf, "Illegal mob vnum %ld for M command. Command disabled", ZCMD.arg1);
        ZONE_ERROR(buf);
        ZCMD.command = '*';
        continue;
      }

      if (!check_unique_mob(rnum, ZCMD.arg1)) {
        sprintf(buf, "Unique mob vnum %ld for M command already exists. Command NOT disabled", ZCMD.arg1);
        ZONE_ERROR(buf);
	continue;
      }

      if ((rnum2 = real_room(ZCMD.arg3)) < 0) {
        sprintf(buf, "Illegal room vnum %ld for M command. Command disabled", ZCMD.arg3);
        ZONE_ERROR(buf);
        ZCMD.command = '*';
        continue;
      }
      if( ZCMD.arg3 > zone_table[zone].top )
      {
        sprintf(buf, "Rroom vnum %ld out of zone %d for M command. Command disabled", ZCMD.arg3, zone );
        ZONE_ERROR(buf);
        ZCMD.command = '*';
        continue;
      }

      if (mob_index[rnum].number < ZCMD.arg2) {
       if (check_percentage(ZCMD.percentage)) {
	 mob = read_mobile(rnum, REAL);
	 char_to_room(mob, rnum2);
	 load_mtrigger(mob);
	 tmob = mob;
	 last_cmd = 1;
       } else {
	 last_cmd = 0;
       }
      } else {  
	last_cmd = 0;
      } 

      tobj = NULL;
      break;

    case 'O':			/* read an object */
      if ((rnum = real_object(ZCMD.arg1)) < 0) {
        sprintf(buf, "Illegal obj vnum %ld for O command. Command disabled", ZCMD.arg1);
        ZONE_ERROR(buf);
        ZCMD.command = '*';
        continue;
      }
      
      if (!check_unique_obj(rnum, ZCMD.arg1)) {
        sprintf(buf, "Unique obj vnum %ld for O command already exists. Command NOT disabled", ZCMD.arg1);
        ZONE_ERROR(buf);
        continue;
      }

      if ((rnum2 = real_room(ZCMD.arg3)) < 0) {
        sprintf(buf, "Illegal room vnum %ld for O command. Command disabled", ZCMD.arg3);
        ZONE_ERROR(buf);
        ZCMD.command = '*';
        continue;
      }
      if( ZCMD.arg3 > zone_table[zone].top )
      {
        sprintf(buf, "Room vnum %ld out of zone %d for O command. Command disabled", ZCMD.arg3, zone);
        ZONE_ERROR(buf);
        ZCMD.command = '*';
        continue;
      }
      
      if (obj_index[rnum].number < ZCMD.arg2 && check_percentage(ZCMD.percentage)) {
	if (rnum2 >= 0) {
	  obj = read_object(rnum, REAL);
	  obj_to_room(obj, rnum2);
          load_otrigger(obj);
	  if (GET_OBJ_TYPE(obj) == ITEM_WAGON && (room_rnum = real_room(GET_OBJ_VNUM(obj))) != NOWHERE)
	    world[room_rnum].wagon = obj;
          tobj = obj;
	  last_cmd = 1;
	} else {
	  obj = read_object(rnum, REAL);
	  obj->in_room = NOWHERE;
          tobj = obj;
	  last_cmd = 1;
	}
      } else
	last_cmd = 0;
      tmob = NULL;
      break;

    case 'L': // Mob to Obj (sleeping)
      if ((rnum = real_mobile(ZCMD.arg1)) < 0) {
	sprintf(buf, "Illegal mob vnum %ld for L command. Command disabled", ZCMD.arg1);
	ZONE_ERROR(buf);
        ZCMD.command = '*';
        continue;
      }
      if ((rnum2 = real_object(ZCMD.arg3)) < 0) {
        sprintf(buf, "Illegal obj vnum %ld for L command. Command disabled", ZCMD.arg2);
        ZONE_ERROR(buf);
        ZCMD.command = '*';
        continue;
      }
      if (!check_unique_mob(rnum, ZCMD.arg1)) {
        sprintf(buf, "Unique mob vnum %ld for L command already exists. Command NOT disabled", ZCMD.arg1);
        ZONE_ERROR(buf);
        continue;
      }

      if (mob_index[rnum].number < ZCMD.arg2 && check_percentage(ZCMD.percentage)) {
	mob = read_mobile(rnum,REAL);

	if (!(obj_to = get_obj_num(rnum2))) {
          ZONE_ERROR("target obj not found");
          break;
        }
	char_to_room(mob,obj_to->in_room);
        load_mtrigger(mob);
        tmob = mob;
	char_sleep_on_obj(mob,obj_to);
        last_cmd = 1;
      } else {
        last_cmd = 0;
      }

      tobj = NULL;
      break;

    case 'T':
      if ((rnum = real_mobile(ZCMD.arg1)) < 0) {
        sprintf(buf, "Illegal mob vnum %ld for T command. Command disabled", ZCMD.arg1);
        ZONE_ERROR(buf);
        ZCMD.command = '*';
        continue;
      }
      if ((rnum2 = real_object(ZCMD.arg3)) < 0) {
        sprintf(buf, "Illegal obj vnum %ld for T command. Command disabled", ZCMD.arg2);
        ZONE_ERROR(buf);
        ZCMD.command = '*';
        continue;
      }
     
      if (!check_unique_mob(rnum, ZCMD.arg1)) {
        sprintf(buf, "Unique mob vnum %ld for T command already exists. Command NOT disabled", ZCMD.arg1);
        ZONE_ERROR(buf);
        continue;
      }

      if (mob_index[rnum].number <= ZCMD.arg2 && check_percentage(ZCMD.percentage)) {
        mob = read_mobile(rnum,REAL);
    
        if (!(obj_to = get_obj_num(rnum2))) {
          ZONE_ERROR("target obj not found");
          break;
        }
	char_to_room(mob,obj_to->in_room);
        load_mtrigger(mob);
        tmob = mob;
	char_sit_on_obj(mob,obj_to);
	
        last_cmd = 1;
      } else {
        last_cmd = 0;
      }

      tobj = NULL;
      break;

    case 'U':
      if ((rnum = real_object(ZCMD.arg1)) < 0) {
	sprintf(buf, "Illegal obj vnum %ld for U command. Command disabled", ZCMD.arg1);
        ZONE_ERROR(buf);
        ZCMD.command = '*';
        continue;
      }
      if ((rnum2 = real_object(ZCMD.arg3)) < 0) {
        sprintf(buf, "Illegal obj vnum %ld for U command. Command disabled", ZCMD.arg2);
        ZONE_ERROR(buf);
        ZCMD.command = '*';
        continue;
      }
      if (!check_unique_obj(rnum, ZCMD.arg1)) {
        sprintf(buf, "Unique obj vnum %ld for U command already exists. Command NOT disabled", ZCMD.arg1);
        ZONE_ERROR(buf);
        continue;
      }

      if (obj_index[rnum].number < ZCMD.arg2 && check_percentage(ZCMD.percentage)) {
        obj = read_object(rnum, REAL);

        if (!(obj_to = get_obj_num(rnum2))) {
          ZONE_ERROR("target obj not found");
          break;
        }
	if (!put_obj_on_obj(obj,obj_to))
	  obj_to_room(obj,obj_to->in_room);
        load_otrigger(obj);
        tobj = obj;
        last_cmd = 1;
      } else
        last_cmd = 0;

      tmob = NULL;
      break;
    case 'P':			/* object to object */
      if ((rnum = real_object(ZCMD.arg1)) < 0) {
        sprintf(buf, "Illegal obj vnum %ld for P command. Command disabled", ZCMD.arg1);
        ZONE_ERROR(buf);
        ZCMD.command = '*';
        continue;
      }
      if ((rnum2 = real_object(ZCMD.arg3)) < 0) {
        sprintf(buf, "Illegal obj vnum %ld for P command. Command disabled", ZCMD.arg2);
        ZONE_ERROR(buf);
        ZCMD.command = '*';
        continue;
      }
      if (!check_unique_obj(rnum, ZCMD.arg1)) {
        sprintf(buf, "Unique obj vnum %ld for P command already exists. Command NOT disabled", ZCMD.arg1);
        ZONE_ERROR(buf);
        continue;
      }

      if (obj_index[rnum].number < ZCMD.arg2 && check_percentage(ZCMD.percentage)) {
	obj = read_object(rnum, REAL);
	if (!(obj_to = get_obj_num(rnum2))) {
	  ZONE_ERROR("target obj not found");
	  break;
	}
	obj_to_obj(obj, obj_to);
        load_otrigger(obj);
        tobj = obj;
	last_cmd = 1;
      } else
	last_cmd = 0;
      
      tmob = NULL;
      break;

    case 'G':			/* obj_to_char */

      if ((rnum = real_object(ZCMD.arg1)) < 0) {
        sprintf(buf, "Illegal obj vnum %ld for G command. Command disabled", ZCMD.arg1);
        ZONE_ERROR(buf);
        ZCMD.command = '*';
        continue;
      }
      
      if (!mob) {
        sprintf(buf, "SYSERR: attempt to give obj #%ld to non-existant mob in room #%d.", 
                ZCMD.arg1, world[rnum2].number);
        alog(buf);
	break;
      }

      if (!check_unique_obj(rnum, ZCMD.arg1)) {
        sprintf(buf, "Unique obj vnum %ld for G command already exists. Command NOT disabled", ZCMD.arg1);
        ZONE_ERROR(buf);
        continue;
      }

      if (obj_index[rnum].number < ZCMD.arg2 && check_percentage(ZCMD.percentage)) {
	obj = read_object(rnum, REAL);
	obj_to_char(obj, mob);
        load_otrigger(obj);
        tobj = obj;
	last_cmd = 1;
      } else {  
	alog("ZReset: Something failed, either too many objects, or percentage-fail.");
	sprintf(buf, "ZReset: ... obj #%ld to mob #%ld in room #%d.", ZCMD.arg1, GET_MOB_VNUM(mob), world[rnum2].number);
	alog(buf);
	last_cmd = 0;
      } 

      tmob = NULL;
      break;

    case 'E':			/* object to equipment list */
      if ((rnum = real_object(ZCMD.arg1)) < 0) {
        sprintf(buf, "Illegal obj vnum %ld for E command. Command disabled", ZCMD.arg1);
        ZONE_ERROR(buf);
        ZCMD.command = '*';
        continue;
      }

      if (!mob) {
	ZONE_ERROR("trying to equip non-existant mob");
	break;
      }
      if (!check_unique_obj(rnum, ZCMD.arg1)) {
        sprintf(buf, "Unique obj vnum %ld for E command already exists. Command NOT disabled", ZCMD.arg1);
        ZONE_ERROR(buf);
        continue;
      }

      if (obj_index[rnum].number < ZCMD.arg2 && check_percentage(ZCMD.percentage)) {
	if (ZCMD.arg3 < 0 || ZCMD.arg3 >= NUM_WEARS) {
	  ZONE_ERROR("invalid equipment pos number");
	} else {
	  obj = read_object(rnum, REAL);
          obj->in_room = mob->in_room;
          load_otrigger(obj);
          if (wear_otrigger(obj, mob, ZCMD.arg3))
	    {
	      obj->in_room = NOWHERE;
	      equip_char(mob, obj, ZCMD.arg3);
	    }
          else
	    obj_to_char(obj, mob);
          tobj = obj;
	  last_cmd = 1;
	}
      } else
	last_cmd = 0;
      
      tmob = NULL;
      break;

    case 'R': /* rem obj from room */
      if ((rnum = real_room(ZCMD.arg1)) < 0) {
        sprintf(buf, "Illegal room vnum %ld for R command. Command disabled", ZCMD.arg1);
        ZONE_ERROR(buf);
        ZCMD.command = '*';
        continue;
      }
      if( ZCMD.arg1 > zone_table[zone].top )
      {
        sprintf(buf, "Room vnum %ld out of zone %d for R command. Command disabled", ZCMD.arg1, zone);
        ZONE_ERROR(buf);
        ZCMD.command = '*';
        continue;
      }
      if (((obj = get_obj_in_list_num(real_object(ZCMD.arg2), world[rnum].contents)) != NULL) &&
      check_percentage(ZCMD.percentage)) {
        obj_from_room(obj);
        extract_obj(obj);
      }
      last_cmd = 1;

      tmob = NULL;
      tobj = NULL;
      break;
    case 'D':			/* set state of door */
      if ((rnum = real_room(ZCMD.arg1)) < 0) {
        sprintf(buf, "Illegal room vnum %ld for D command. Command disabled", ZCMD.arg1);
        ZONE_ERROR(buf);
        ZCMD.command = '*';
        continue;
      }
      if( ZCMD.arg1 > zone_table[zone].top )
      {
        sprintf(buf, "Room vnum %ld out of zone %d for D command. Command disabled", ZCMD.arg1, zone);
        ZONE_ERROR(buf);
        ZCMD.command = '*';
        continue;
      }
      if (ZCMD.arg2 < 0 || ZCMD.arg2 >= NUM_OF_DIRS ||
      (world[rnum].dir_option[ZCMD.arg2] == NULL) ||
      (world[rnum].dir_option[ZCMD.arg2]->to_room == NOWHERE))
      {
        sprintf(buf, "Door does not exist: \\c01Info: Dir:%ld Room: %d Closed status: %ld", ZCMD.arg2, world[rnum].number, ZCMD.arg3);
    ZONE_ERROR(buf);
      }
      else if (!(world[rnum].dir_option[ZCMD.arg2]->exit_info & EX_ISDOOR)) 
      {
        sprintf(buf, "Exit is without door: \\c01Info: Dir:%ld Room: %d Closed status: %ld", ZCMD.arg2, world[rnum].number, ZCMD.arg3);
        ZONE_ERROR(buf);
      }
      else
      {
    if (check_percentage(ZCMD.percentage)) {
      rm = &world[rnum];  
      door = rm->dir_option[ZCMD.arg2];
      toroom = &world[door->to_room];
      if ((toroom > 0) && world[door->to_room].dir_option[antidir[ZCMD.arg2]])
            todoor = world[door->to_room].dir_option[antidir[ZCMD.arg2]];  
      switch (ZCMD.arg3) {
      case 0:
	REMOVE_BIT(door->exit_info, EX_LOCKED);
	REMOVE_BIT(door->exit_info, EX_CLOSED);
	if (todoor) {
	  REMOVE_BIT(todoor->exit_info, EX_LOCKED);
	  REMOVE_BIT(todoor->exit_info, EX_CLOSED); 
	}
	break;
      case 1:
	SET_BIT(door->exit_info, EX_CLOSED);
	REMOVE_BIT(door->exit_info, EX_LOCKED);
	if (todoor) {
          SET_BIT(door->exit_info, EX_CLOSED);
          REMOVE_BIT(door->exit_info, EX_LOCKED);   
        }
	break;
      case 2:
	SET_BIT(door->exit_info, EX_LOCKED);
	SET_BIT(door->exit_info, EX_CLOSED);
	if (todoor) {
	  SET_BIT(door->exit_info, EX_LOCKED);
	  SET_BIT(door->exit_info, EX_CLOSED); 
	}
	break;
      }
    } else {
      last_cmd = 0;
      continue;
    }
      }
      last_cmd = 1;

      tmob = NULL;
      tobj = NULL;
      break;

    case 'A': /* trigger command; details to be filled in later */
      if (ZCMD.arg1==MOB_TRIGGER && tmob) {
       if (!SCRIPT(tmob))
        CREATE(SCRIPT(tmob), struct script_data, 1);
       add_trigger(SCRIPT(tmob), read_trigger(real_trigger(ZCMD.arg2)), -1);
       last_cmd = 1;
      } 
      else if (ZCMD.arg1==OBJ_TRIGGER && tobj) {
       if (!SCRIPT(tobj))
        CREATE(SCRIPT(tobj), struct script_data, 1);
       add_trigger(SCRIPT(tobj), read_trigger(real_trigger(ZCMD.arg2)), -1);
       last_cmd = 1;
      }
      break;

    case 'V':
      if (ZCMD.arg1==MOB_TRIGGER && tmob) {
       if (!SCRIPT(tmob)) {
        ZONE_ERROR("Attempt to give variable to scriptless mobile");
       } 
       else
        add_var(&(SCRIPT(tmob)->global_vars), ZCMD.sarg1, ZCMD.sarg2, ZCMD.arg3);
       last_cmd = 1;
      }
      else if (ZCMD.arg1==OBJ_TRIGGER && tobj) {
       if (!SCRIPT(tobj)) {
        ZONE_ERROR("Attempt to give variable to scriptless object");
       }
       else
        add_var(&(SCRIPT(tobj)->global_vars), ZCMD.sarg1, ZCMD.sarg2, ZCMD.arg3);
       last_cmd = 1;
      }
      else if (ZCMD.arg1==WLD_TRIGGER) {
       if (ZCMD.arg2<0 || ZCMD.arg2>top_of_world) {
        ZONE_ERROR("Invalid room number in variable assignment");
       }
       else {
        if (!(world[ZCMD.arg2].script)) {
         ZONE_ERROR("Attempt to give variable to scriptless object");
        }
        else
         add_var(&(world[ZCMD.arg2].script->global_vars), ZCMD.sarg1, ZCMD.sarg2, ZCMD.arg3);
        last_cmd = 1;
       }
      }
      break; 
       
    default:
      {
	char buf[4096];
	sprintf(buf, "unknown cmd '%c' in reset table; cmd disabled", ZCMD.command);
	ZONE_ERROR(buf);
	ZCMD.command = '*';
	break;
      }
    }

  }
  zone_table[zone].age = 0;

  /* handle reset_wtrigger's */
  room_vnum = zone_table[zone].number * 100;
  while (room_vnum <= zone_table[zone].top) {
   room_rnum = real_room(room_vnum);
   if (room_rnum != NOWHERE)
   {
    if (SCRIPT(&world[room_rnum]))
     extract_script(SCRIPT(&world[room_rnum]));
    assign_triggers(&world[room_rnum], WLD_TRIGGER);
    reset_wtrigger(&world[room_rnum]);
   }
   room_vnum++;
  }
}

/* returns 0 if mob is unique and already in existance on the mud! */
int check_unique_mob(int mobrnum, int mobvnum)
{
  /* non-existant rnum...should nevwer happen, but... */
  if (mobrnum < 0 || mobrnum > top_of_mobt)
    return 1;

  /* obj is not flagged unique, how can the test fail? */
  if (!IS_SET(MOB_FLAGS(&mob_proto[mobrnum]),MOB_UNIQUE))
    return 1;

  /* obj is flagged, now look through the Master Obj List and see
     if ANY object with the vnum/rnum exists! */
  if (get_char_num(mobrnum))
    return 0;

  return 1;
}

/* returns 0 if obj is unique and already in existance on the mud! */
int check_unique_obj(int objrnum, int objvnum)
{
  /* non-existant rnum...should nevwer happen, but... */
  if (objrnum < 0 || objrnum > top_of_objt)
    return 1;

  /* obj is not flagged unique, how can the test fail? */
  if (!IS_SET(GET_OBJ_EXTRA(&obj_proto[objrnum]),ITEM_UNIQUE))
    return 1;

  /* obj is flagged, now look through the Master Obj List and see 
     if ANY object with the vnum/rnum exists! */
  if (get_obj_num(objrnum)) 
    return 0;

  return 1;
}


/* for use in reset_zone; return TRUE if zone 'nr' is free of PC's  */
int is_empty(int zone_nr)
{
  struct descriptor_data *i;

  for (i = descriptor_list; i; i = i->next)
    if (!i->connected)
      if (world[i->character->in_room].zone == zone_nr)
    return 0;

  return 1;
}





/*************************************************************************
*  stuff related to the save/load player system				 *
*********************************************************************** */


long get_id_by_name(char *name)
{
  int i;

  one_argument(name, arg);
  for (i = 0; i <= top_of_p_table; i++)
    if (!strcasecmp((player_table + i)->name, arg) || (player_table[i].disguised && !strcasecmp((player_table + i)->dname, arg)))
      return ((player_table + i)->id);

  return -1;
}


char *get_name_by_id(long id)
{
  int i;

  for (i = 0; i <= top_of_p_table; i++)
    if ((player_table + i)->id == id)
      return (player_table[i].disguised ? (player_table + i)->dname : (player_table + i)->name);

  return NULL;
}


static void write_knows(struct char_data *ch)
{
  char file[1024];
  FILE *fn;
  bv_tp tmp;

  get_filename(GET_RNAME(ch),file,KNOW_FILE);
  if (NULL == (fn = fopen(file,"w"))) {
    alog("SYSERR: failed to open know-file(%s) for %s",file,GET_RNAME(ch));
    return;
  }
  if (NULL == GET_KNOW(ch)) 
  {
    tmp = BITVECTOR(MAX_ROOMS_ON_MUD);
  } else 
  {
    tmp = GET_KNOW(ch);
  }
  to_file(tmp,fn);
  if (GET_KNOW(ch) == NULL) // Must get rid of calloc'd memory
  {
    KILL_BV(tmp);
  }
  fclose(fn);
}

/*
 * write the vital data of a player to the player file
 *
 * NOTE: load_room should be an *RNUM* now.  It is converted to a vnum here.
 */
void save_char(struct char_data * ch, sh_int load_room)
{
  if (IS_NPC(ch) || !ch->desc || GET_PFILEPOS(ch) < 0)
    return;

  if (!PLR_FLAGGED(ch, PLR_LOADROOM)) {
    if (NOWHERE == load_room) {
      GET_LOADROOM(ch) = NOWHERE;
    } else {
      GET_LOADROOM(ch) = world[load_room].number;
    }
  }
  xmlsave_char(ch);
  write_knows(ch);
  update_p_table(ch);

  return;
}

void read_know(struct char_data *ch)
{
  FILE *fn;
  char file[1024];
  bv_tp tmp;

  get_filename(GET_RNAME(ch),file,KNOW_FILE);
  if (NULL == (fn = fopen(file,"r"))) {
    alog("SYSERR: failed to open know-file(%s) for %s",file,GET_RNAME(ch));
    if (!GET_KNOW(ch))
      GET_KNOW(ch) = BITVECTOR(MAX_ROOMS_ON_MUD);
    return;
  }
  if (NULL == (tmp = from_file(GET_KNOW(ch),fn))) {
    alog("SYSERR: %s know file, wrong format!",GET_RNAME(ch));
    if (!GET_KNOW(ch))
      GET_KNOW(ch) = BITVECTOR(MAX_ROOMS_ON_MUD);
    return;
  }
  if ((tmp != GET_KNOW(ch)) && (GET_KNOW(ch) != NULL)) {
    KILL_BV(GET_KNOW(ch));
    GET_KNOW(ch) = tmp;
  }    
  fclose(fn);
}

/* create a new entry in the in-memory index table for the player file */
int create_entry(char *name)
{
  int i;

  if ((i = find_name(name)) != NOTHING)
    return i;

  if (top_of_p_table == -1) {
    CREATE(player_table, struct player_index_element, 1);
    top_of_p_table = 0;
  } else if (!(player_table = (struct player_index_element *)
        realloc(player_table, sizeof(struct player_index_element) *
             (++top_of_p_table + 1)))) {
    mlog("SYSERR: create entry");
    exit(1);
  }
  CREATE(player_table[top_of_p_table].name, char, strlen(name) + 1);

  player_table[top_of_p_table].dname = NULL;

  /* copy lowercase equivalent of name to table field */
  for (i = 0; (*(player_table[top_of_p_table].name + i) = LOWER(*(name + i)));
       i++);

  return (top_of_p_table);
}



/************************************************************************
*  funcs of a (more or less) general utility nature			*
********************************************************************** */
/*
 * Removes \r's and \n's that come in a sequence.
 * Modifies string given as an argument.
 */
void trim(char *str)
{
  char *ptr;
  int i = 0, length = 0;

  if (!str || !*str)
    return;

  ptr = str;
  length = strlen(str);

  while (ptr < str + length)
  {
    if ((*ptr == *(ptr +1)) && ((*ptr == '\n') || (*ptr == '\r')))
    {
      ptr++;
      continue;
    }
    buf[i++] = *ptr++;
  }
  while (i < length)
    buf[i++] = 0;
  strncpy(str, buf, i);
}

/* read and allocate space for a '~'-terminated string from a given file */
char *fread_string(FILE * fl, char *error)
{
  char buf[MAX_STRING_LENGTH], tmp[MAX_STRING_LENGTH], *rslt;
  register char *point;
  int done = 0, length = 0, templength = 0;

  *buf = '\0';

  do {
    if (!fgets(tmp, 512, fl)) {
      fprintf(stderr, "SYSERR: fread_string: format error at or near %s\n",
          error);
      exit(1);
    }
    /* If there is a '~', end the string; else put an "\r\n" over the '\n'. */
    if ((point = strchr(tmp, '~')) != NULL) {
      *point = '\0';
      done = 1;
    } else {
      point = tmp + strlen(tmp) - 1;
      *(point++) = '\r';
      *(point++) = '\n';
      *point = '\0';
    }

    templength = strlen(tmp);

    if (length + templength >= MAX_STRING_LENGTH) {
      mlog("SYSERR: fread_string: string too large (db.c)");
      mlog(error);
      exit(1);
    } else {
      strcat(buf + length, tmp);
      length += templength;
    }
  } while (!done);

  /* allocate space for the new string and copy it */
  if (strlen(buf) > 0) {
    CREATE(rslt, char, length + 1);
    strcpy(rslt, buf);
  } else
    rslt = NULL;
  trim(rslt);
  return rslt;
}


/* release memory allocated for a char struct */
void free_char(struct char_data * ch)
{
  int i;
  struct alias *a;
  struct script_memory       *mem,   *mnext;

  /* Just do some testing first? */
  if (ch == NULL) {
    xlog( SYS_GOD, LVL_IMMORT, "free_char() called with NULL pointer!\n", TRUE );
    return;
  }

//  if( !IS_MOB( ch ) )
//  { char debug[255];
//    sprintf( debug, "Freeing Memory used by %s", ch->player.name );
//    xlog( SYS_DEBUG, 0, debug, FALSE );
//  }

  if (ch->player_specials != NULL && ch->player_specials != &dummy_mob) 
  { while ((a = GET_ALIASES(ch)) != NULL) 
    { GET_ALIASES(ch) = (GET_ALIASES(ch))->next;
      free_alias(a);
    }
    if (ch->player_specials->poofin)
      free(ch->player_specials->poofin);
    if (ch->player_specials->poofout)
      free(ch->player_specials->poofout);
    if (NULL != GET_KNOW(ch))
       KILL_BV(GET_KNOW(ch));
    if (GET_LASTEMOTESTR(ch))
      free(GET_LASTEMOTESTR(ch));
    if (ch->player_specials->chess)
      free_game_of_chess(ch->player_specials->chess);

    if (ch->player_specials->afk_String) 
    { if (strcmp(ch->player_specials->afk_String, std_Flag) == 0 || strcmp(ch->player_specials->afk_String, std_afw) == 0)
        ch->player_specials->afk_String = NULL;
      else
        free(ch->player_specials->afk_String);
    }
    if (ch->player_specials->doing)
      free(ch->player_specials->doing);
    if (ch->player_specials->prefix)
      free(ch->player_specials->prefix);

    /* free(ch->player_specials); We aren't done with this yet!! */

    while (ch->affected)
      affect_remove(ch, ch->affected);

    if (IS_NPC(ch))
      mlog("SYSERR: Mob had player_specials allocated!");
  }
  if (!IS_NPC(ch) || (IS_NPC(ch) && GET_MOB_RNUM(ch) == -1)) {
  /* if this is a player, or a non-prototyped non-player, free all */
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
  } else if ((i = GET_MOB_RNUM(ch)) > -1) {
    /* otherwise, free strings only if the string is not pointing at proto */
    if (ch->player.name && ch->player.name != mob_proto[i].player.name)
      free(ch->player.name);
    if (GET_DNAME(ch) && GET_DNAME(ch) != GET_DNAME((&mob_proto[i])))
      free(GET_DNAME(ch));
    if (ch->player.title && ch->player.title != mob_proto[i].player.title)
      free(ch->player.title);
    if (GET_DTITLE(ch) && GET_DTITLE(ch) != GET_DTITLE((&mob_proto[i])))
      free(GET_DTITLE(ch));
    if (ch->player.short_descr && ch->player.short_descr != mob_proto[i].player.short_descr)
      free(ch->player.short_descr);
    if (ch->player.long_descr && ch->player.long_descr != mob_proto[i].player.long_descr)
      free(ch->player.long_descr);
    if (ch->player.description && ch->player.description != mob_proto[i].player.description)
      free(ch->player.description);
  }
  else {
    while (ch->affected)
      affect_remove(ch, ch->affected);

    ch->affected = NULL;
  }

  while (ch->affected)
    affect_remove(ch, ch->affected);
   
  if (!IS_NPC(ch) && ch->player_specials != NULL) {
    free(ch->player_specials);
    ch->player_specials = NULL;
  }

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

  free(ch);
}




/* release memory allocated for an obj struct */
void free_obj(struct obj_data * obj)
{
  int nr;
  struct extra_descr_data *this, *next_one;

  if (!obj) {
    mlog("SYSERR: NULL-object passed to free_obj()[db.c]");
    return;
  }

  if ((nr = GET_OBJ_RNUM(obj)) == -1) {
    if (obj->name)
      free(obj->name);
    if (obj->description)
      free(obj->description);
    if (obj->short_description)
      free(obj->short_description);
    if (obj->action_description)
      free(obj->action_description);
    if (obj->ex_description)
      for (this = obj->ex_description; this; this = next_one) {
    next_one = this->next;
    if (this->keyword)
      free(this->keyword);
    if (this->description)
      free(this->description);
    free(this);
      }
  } else {
    if (nr < 0 || nr > top_of_objt) {
       alog("SYSERR: RNUM of obj (%d) passed to free_obj()[db.c] outside of scope for obj_table.",nr);
       return;
    }
    if (obj->name && obj->name != obj_proto[nr].name)
      free(obj->name);
    if (obj->description && obj->description != obj_proto[nr].description)
      free(obj->description);
    if (obj->short_description && obj->short_description != obj_proto[nr].short_description)
      free(obj->short_description);
    if (obj->action_description && obj->action_description != obj_proto[nr].action_description)
      free(obj->action_description);
    if (obj->ex_description && obj->ex_description != obj_proto[nr].ex_description)
      for (this = obj->ex_description; this; this = next_one) {
    next_one = this->next;
    if (this->keyword)
      free(this->keyword);
    if (this->description)
      free(this->description);
    free(this);
      }
  }

  obj->proto_script = NULL;

  if (obj->script) 
    free_script(obj->script);

  free(obj);
}



/* read contets of a text file, alloc space, point buf to it */
int file_to_string_alloc(char *name, char **buf)
{
  char temp[MAX_STRING_LENGTH];

  if (*buf)
    free(*buf);

  if (file_to_string(name, temp) < 0) {
    sprintf(temp,"<file \"%s\" is empty or non-existant!>\r\n",name);
    *buf = strdup(temp);
    return -1;
  } else {
    *buf = strdup(temp);
    return 0;
  }
}


/* read contents of a text file, and place in buf */
int file_to_string(char *name, char *buf)
{
  FILE *fl;
  char tmp[READ_SIZE+3];
  memset(tmp,'\0', READ_SIZE+3);

  *buf = '\0';

  if (!(fl = fopen(name, "r"))) {
    sprintf(tmp, "SYSERR: Error reading %s", name);
    mlog(tmp);
    return (-1);
  }
  do {
    fgets(tmp, READ_SIZE, fl);
    tmp[strlen(tmp) - 1] = '\0'; /* take off the trailing \n */
    strcat(tmp, "\r\n");

    if (!feof(fl)) {
      if (strlen(buf) + strlen(tmp) + 1 > MAX_STRING_LENGTH) {
        sprintf(buf, "SYSERR: %s: string too big (%d max)", name,
        MAX_STRING_LENGTH);
    mlog(buf);
    *buf = '\0';
    return -1;
      }
      strcat(buf, tmp);
    }
  } while (!feof(fl));

  fclose(fl);

  return (0);
}



/* clear some of the the working variables of a char */
void reset_char(struct char_data * ch)
{
  int i;

  for (i = 0; i < NUM_WEARS; i++)
    GET_EQ(ch, i) = NULL;

  ch->followers = NULL;
  ch->master = NULL;
  ch->in_room = NOWHERE;
  ch->carrying = NULL;
  ch->next = NULL;
  ch->next_fighting = NULL;
  ch->next_in_room = NULL;
  FIGHTING(ch) = NULL;
  ch->char_specials.position = POS_STANDING;
  ch->mob_specials.default_pos = POS_STANDING;
  ch->char_specials.carry_weight = 0;
  ch->char_specials.carry_items = 0;

  if (GET_HIT(ch) <= 0)
    GET_HIT(ch) = 1;
  if (GET_MOVE(ch) <= 0)
    GET_MOVE(ch) = 1;
  if (GET_FRACT_MOVE(ch) <= 0)
    GET_FRACT_MOVE(ch) = 0;
  if (GET_MANA(ch) <= 0)
    SET_MANA(ch, 1);

  GET_LAST_TELL(ch) = NOBODY;
}



/* clear ALL the working variables of a char; do NOT free any space alloc'ed */
void clear_char(struct char_data * ch)
{
  memset((char *) ch, 0, sizeof(struct char_data));

  ch->in_room = NOWHERE;
  GET_PFILEPOS(ch) = -1;
  GET_WAS_IN(ch) = NOWHERE;
  GET_POS(ch) = POS_STANDING;
  ch->mob_specials.default_pos = POS_STANDING;

  GET_REAL_AC(ch) = 100;		/* Basic Armor */
  if (ch->points.max_mana < 100)
    ch->points.max_mana = 100;
}


void clear_object(struct obj_data * obj)
{
  memset((char *) obj, 0, sizeof(struct obj_data));
  obj->oprog = NULL;
  obj->item_number = NOTHING;
  obj->in_room = NOWHERE;
  obj->obj_status = 100; /* Brand new item, hence the 100 % */
}



/* initialize a new character only if class is set */
void init_char(struct char_data * ch)
{
  int i;

  /* create a player_special structure */
  if (ch->player_specials == NULL)
    CREATE(ch->player_specials, struct player_special_data, 1);

  /* *** if this is our first player --- he be God *** */

  if (top_of_p_table == 0) {
    GET_EXP(ch) = 400000;
    GET_LEVEL(ch) = LVL_IMPL;

    ch->points.max_hit = 5000;
    ch->points.max_mana = 1000;
    ch->points.max_move = 1000;
  }
  set_title(ch, NULL);

  ch->player.short_descr = NULL;
  ch->player.long_descr = NULL;
  ch->player.description = NULL;

  for (i = 0; i < MAX_DESCRIPTIONS; i++) {
    GET_DESCNUM(ch,i) = NULL;
  }

  ch->player.hometown = 1;

  ch->player.time.birth = time(0);
  ch->player.time.played = 0;
  ch->player.time.logon = time(0);

  for (i = 0; i < MAX_TONGUE; i++)
    GET_TALK(ch, i) = 0;

  /* make favors for sex */
  if (ch->player.sex == SEX_MALE) {
    ch->player.weight = number(120, 180);
    ch->player.height = number(160, 200);
  } else {
    ch->player.weight = number(100, 160);
    ch->player.height = number(150, 180);
  }

  ch->points.armor = 100;

  player_table[top_of_p_table].id = GET_IDNUM(ch) = ++top_idnum;
  player_table[top_of_p_table].wkin = 0;
  player_table[top_of_p_table].bm = 0;

  GET_PFILEPOS(ch) = top_of_p_table;

  for (i = 1; i <= MAX_SKILLS; i++) {
    if (GET_LEVEL(ch) < LVL_IMPL)
      SET_SKILL(ch, i, 0);
    else
      SET_SKILL(ch, i, 100);
  }

  ch->char_specials.saved.affected_by = 0;
  ch->char_specials.saved.affected2 = 0;
  ch->char_specials.saved.affected3 = 0;
  ch->char_specials.saved.affected4 = 0;  


  for (i = 0; i < 3; i++)
    GET_COND(ch, i) = (GET_LEVEL(ch) == LVL_IMPL ? -1 : 24);

  GET_BONDED(ch) = 0;
  GET_ASHABONDED(ch) = 0;
  GET_MINION(ch) = 0;
  GET_LOADROOM(ch) = NOWHERE;
  //strcpy(GET_MYCOLOR(ch), "n");
}



/* returns the real number of the room with given virtual number */
int real_room(int virtual)
{
  int bot, top, mid;

  bot = 0;
  top = top_of_world;

  /* perform binary search on world-table */
  for (;;) {
    mid = (bot + top) / 2;

    if ((world + mid)->number == virtual)
      return mid;
    if (bot >= top)
      return NOWHERE;
    if ((world + mid)->number > virtual)
      top = mid - 1;
    else
      bot = mid + 1;
  }
}

/* dnsmod */
void boot_dns(void)   
{
  int i = 0;
  char line[256], name[256];
  FILE *fl;
  struct dns_entry *dns;
  
  memset((char *) dns_cache, 0, sizeof(struct dns_entry *) * DNS_HASH_NUM);

  if(!(fl = fopen(DNS_FILE, "r"))) {
    mlog("No DNS cache!");
    return;
  }
   
  do {
    i = 0;
    get_line(fl, line);
    if(*line != '~') {
      CREATE(dns, struct dns_entry, 1);
      dns->name = NULL;
      dns->next = NULL;  
      name[0] = 0;
      sscanf(line, "%d.%d.%d.%d %s", dns->ip, dns->ip + 1,
      dns->ip + 2, dns->ip + 3, name);
      dns->name = str_dup(name);
      i = (dns->ip[0] + dns->ip[1] + dns->ip[2]) % DNS_HASH_NUM;
      dns->next = dns_cache[i];
      dns_cache[i] = dns;
    }
  } while (!feof(fl) && *line != '~'); 
  fclose(fl);
}

void free_dns_cache(void)
{
  int i;
  struct dns_entry *dns, *dnext;

  for (i = 0; i < DNS_HASH_NUM; i++) {
    for (dns = dns_cache[i]; NULL != dns; dns = dnext) {
      dnext = dns->next;

      FREEPTR(dns->name);
      free(dns);
    }
    dns_cache[i] = NULL;
  }
}

/* dnsmod */
void save_dns_cache(void)
{
  int i;
  FILE *fl;
  struct dns_entry *dns;
   
  if(!(fl = fopen(DNS_FILE, "w"))) {
    mlog("SYSERR: Can't open dns cache file for write!");
    return;
  }
 
  for(i = 0; i < DNS_HASH_NUM; i++) {
    if(dns_cache[i]) {
      for(dns = dns_cache[i]; dns; dns = dns->next)
      fprintf(fl, "%d.%d.%d.%d %s\n", dns->ip[0], dns->ip[1],
      dns->ip[2], dns->ip[3], dns->name);
    }
  }
  fprintf(fl, "~\n");
  fclose(fl);
}

 /* dnsmod */
int get_host_from_cache(struct dns_entry *dnsd)
{
  int i;
  struct dns_entry *d;
  char buf[256];
   
  i = (dnsd->ip[0] + dnsd->ip[1] + dnsd->ip[2]) % DNS_HASH_NUM;
  if(dns_cache[i]) {
    for(d = dns_cache[i]; d; d = d->next) {
      if(dnsd->ip[0] == d->ip[0] && dnsd->ip[1] == d->ip[1] &&
      dnsd->ip[2] == d->ip[2]) {
        if(d->ip[3] == -1) {
          sprintf(buf, "%d.%s", dnsd->ip[3], d->name);
          dnsd->name = str_dup(buf);
          return TRUE;
        }  else if(dnsd->ip[3] == d->ip[3]) {
          dnsd->name = str_dup(d->name);
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

 /* dnsmod */
void add_dns_host(struct dns_entry *dnsd, char *hostname)
{ 
  int i;
  struct dns_entry *d;
    
  i = (dnsd->ip[0] + dnsd->ip[1] + dnsd->ip[2]) % DNS_HASH_NUM;
  CREATE(d, struct dns_entry, 1);
  d->ip[0] = dnsd->ip[0];
  d->ip[1] = dnsd->ip[1];
  d->ip[2] = dnsd->ip[2];
  d->ip[3] = dnsd->ip[3];
  d->name = str_dup(hostname);
  d->next = dns_cache[i];
  dns_cache[i] = d;   
  save_dns_cache();
}

 /*
  * Read a number from a file.
  */
 int fread_number(FILE *fp)
 {
     int number;
     bool sign;
     char c;
 
     do {
         c = getc(fp);
     } while (isspace(c));
 
     number = 0;
 
     sign   = FALSE;
     if (c == '+') {
         c = getc(fp);
     } else if (c == '-') {
         sign = TRUE;
         c = getc(fp);
     }
 
     if (!isdigit(c)) {
         mlog("Fread_number: bad format.");
         exit(1);
     }
 
     while (isdigit(c)) {
         number = number * 10 + c - '0';
         c      = getc(fp);
     }
 
     if (sign)
         number = 0 - number;
 
     if (c == '|')
         number += fread_number(fp);
     else if (c != ' ')
         ungetc(c, fp);
 
     return number;
 }
 
 /*
  * Read to end of line (for comments).
  */
 void fread_to_eol(FILE *fp)
 {
     char c;
 
     do {
         c = getc(fp);
     } while (c != '\n' && c != '\r');
 
     do {
         c = getc(fp);
     } while (c == '\n' || c == '\r');
 
     ungetc(c, fp);
     return;
 }
 
 /*
  * Read one word (into static buffer).
  */
 char *fread_word(FILE *fp)
 {
     static char word[MAX_INPUT_LENGTH];
     char *pword;
     char cEnd;
 
     do
     {
         cEnd = getc(fp);
     }
     while (isspace(cEnd));
 
     if (cEnd == '\'' || cEnd == '"')
     {
         pword   = word;
     }
     else
     {
         word[0] = cEnd;
         pword   = word+1;
         cEnd    = ' ';
     }
 
     for (; pword < word + MAX_INPUT_LENGTH; pword++)
     {
         *pword = getc(fp);
         if (cEnd == ' ' ? isspace(*pword) || *pword == '~' : *pword == cEnd)
         {
             if (cEnd == ' ' || cEnd == '~')
                 ungetc(*pword, fp);
             *pword = '\0';
             return word;
         }
     }
 
     mlog("SYSERR: Fread_word: word too long.");
     exit(1);
     return NULL;
 }
 
 
 struct index_data *get_obj_index (int vnum)
 {
   int nr;
   for(nr = 0; nr <= top_of_objt; nr++) {
     if(obj_index[nr].virtual == vnum) return &obj_index[nr];
   }
   return NULL;
 }
 
 struct index_data *get_mob_index (int vnum)
 {
   int nr;
   for(nr = 0; nr <= top_of_mobt; nr++) {
     if(mob_index[nr].virtual == vnum) return &mob_index[nr];
   }
   return NULL;
 }
 


/* returns the real number of the monster with given virtual number */
int real_mobile(int virtual)
{
  int bot, top, mid;

  bot = 0;
  top = top_of_mobt;

  /* perform binary search on mob-table */
  for (;;) {
    mid = (bot + top) / 2;

    if ((mob_index + mid)->virtual == virtual)
      return (mid);
    if (bot >= top)
      return (-1);
    if ((mob_index + mid)->virtual > virtual)
      top = mid - 1;
    else
      bot = mid + 1;
  }
}



/* returns the real number of the object with given virtual number */
int real_object(int virtual)
{
  int bot, top, mid;

  bot = 0;
  top = top_of_objt;

  /* perform binary search on obj-table */
  for (;;) {
    mid = (bot + top) / 2;

    if ((obj_index + mid)->virtual == virtual)
      return (mid);
    if (bot >= top)
      return (-1);
    if ((obj_index + mid)->virtual > virtual)
      top = mid - 1;
    else
      bot = mid + 1;
  }
}

void replace_index(struct index_data *old_index,struct index_data *new_index)
{
    free(old_index);
    old_index = new_index;
}

