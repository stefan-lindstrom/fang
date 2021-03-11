/******************************************************************************
 * $Id: db.h,v 1.12 2003/06/17 13:02:37 fang Exp $
 *   
 * File: db.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: db.h,v $
 *   Revision 1.12  2003/06/17 13:02:37  fang
 *
 *   Added a single file for messages. Code for now contains only functionalty to
 *   save messages (fight/spell op/spell tp) as XML. All structs, and data, and
 *   defines pertaining to messages have been moved to the new header.
 *
 *   Revision 1.11  2003/06/10 12:16:08  fang
 *
 *   Added UNIQUE flag for mobs and objs. A unique obj/mob can only exists one
 *   instance at the time on the MUD, no matter what. A unique obj will not
 *   save to characters obj file either.
 *
 *   Revision 1.10  2003/05/12 09:08:33  fang
 *   Enabled dynvars for room descs/xdescs
 *
 *   Revision 1.9  2003/03/10 08:17:49  fang
 *   *** empty log message ***
 *
 *   Revision 1.8  2003/03/01 07:17:05  fang
 *   Fisch: Multiflag for authorized multies
 *
 *   Revision 1.7  2002/10/12 06:30:15  fang
 *   fisch: converted boards to xml and made them dynamic not coded.
 *
 *   Revision 1.6  2002/10/03 05:55:09  fischer
 *   fischer: moved ideas to be used as an old ideas and added newideas file
 *
 *   Revision 1.5  2002/09/12 22:21:33  fang
 *   fischer: removed the prove and journal commands
 *
 *   Revision 1.4  2002/09/08 06:41:16  fang
 *   Drath: Fischer added AEDIT.  Welcome to the staff, Fischer.
 *
 *   Revision 1.3  2002/09/05 20:56:30  fang
 *   Drath: added CRETODO and SHOWCRETODO for MassyPants
 *
 *   Revision 1.2  2002/08/04 09:39:55  fang
 *   Drath: SOCIALIDEA and SHOWSOCIALIDEA seperate from IDEA
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.11  2002/07/06 07:05:12  fang
 *   Drath: IMMLOCK for Fred and 2nd to last XLOG commit
 *
 *   Revision 1.10  2002/06/29 18:33:04  fang
 *   Drath: In-MUD syslog searching.  Replaces websyslog search
 *
 *   Revision 1.9  2002/06/17 08:14:15  fang
 *   *** empty log message ***
 *
 *   Revision 1.8  2002/06/13 10:12:28  fang
 *   More free'ing stuff...
 *
 *   Revision 1.7  2002/06/11 21:32:17  fang
 *   Fixed a board that was set for 204s to remove posts.
 *
 *   Revision 1.6  2002/06/02 22:14:44  fang
 *   Maz: PEDIT
 *
 *   Revision 1.5  2002/05/12 23:50:46  fang
 *   Added in IGNORE system, and removed AutoHotboots until the cap can be fixed for good. -Maz
 *
 *   Revision 1.4  2002/03/19 04:06:37  fang
 *   Moved the greeting into an actual file.
 *
 *   Revision 1.3  2002/02/27 06:03:56  fang
 *   Further modifications to disguise saving code.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.27  2002/01/24 21:33:14  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.26.2.3  2002/01/23 21:49:40  mud
 *   Removed objsave.[c|h] (old objsave stuff).
 *   Removed obj_file_elem and rent structs.
 *   Removed final vestiges of old guild system, except parts used
 *   in guild.[c|h] for backwards compatibility.
 *
 *   Revision 1.26.2.2  2002/01/22 03:05:12  mud
 *   More cleanup. Removed extern decls from source files. Protected some DG headers from multiple inclision
 *
 *   Revision 1.26.2.1  2002/01/18 02:24:51  mud
 *   More cleanup...
 *
 *   Revision 1.26  2001/08/23 14:32:22  mud
 *   *** empty log message ***
 *
 *   Revision 1.25  2001/05/22 09:07:04  mud
 *   DG-Scripts for Fang.
 *
 *   Revision 1.24  2001/05/19 10:07:33  mud
 *   *** empty log message ***
 *
 *   Revision 1.23  2001/05/17 11:21:08  mud
 *   Final residues of old pfile code should be swept away by now. No cleansing of the structures and such things though
 *
 *   Revision 1.22  2001/03/18 10:30:53  mud
 *   Some bugfixes and upodates to the xmlcharsave code
 *
 *   Revision 1.21  2000/12/16 11:02:07  mud
 *   *** empty log message ***
 *
 *   Revision 1.20  2000/11/08 20:34:25  mud
 *   Added the todo list for imms
 *
 *   Revision 1.19  2000/11/08 07:47:42  mud
 *   *** empty log message ***
 *
 *   Revision 1.18  2000/11/07 12:00:39  mud
 *   Finally. XML Mobiles are done.
 *
 *   Revision 1.17  2000/08/20 07:59:53  mud
 *   added a help_index. ordered linkedlist of help keywords and their pos in the help_table
 *
 *   Revision 1.16  2000/08/14 19:11:55  mud
 *   The new guild system.
 *
 *   Revision 1.15  2000/08/10 15:06:29  mud
 *   added hedit (help editor)
 *
 *   Revision 1.14  2000/06/16 01:16:34  mud
 *   Added vnum -f .. -Tesla
 *
 *   Revision 1.13  2000/06/08 14:49:22  mud
 *   olc name lists saved/parsed in xmlZones. Names and all flag checked in OLC permissions.
 *
 *   Revision 1.12  2000/05/26 14:07:02  mud
 *   Recall command, showideas, showbugs, showtypos.
 *
 *   Revision 1.11  2000/04/25 11:10:38  mud
 *   EXP gain is modified on behalf of newbies (lvl <= 20)
 *
 *   Revision 1.10  2000/04/02 12:15:49  mud
 *   *mumble* Fixed up last errors with policies *smacks a HBL*
 *
 *   Revision 1.9  2000/04/02 05:16:04  mud
 *   Put policies file back the way it was before.  New file crashed the
 *   builderport for unknown reasons, despite being typed correctly.
 *
 *   Revision 1.8  2000/04/02 05:03:28  mud
 *   *** empty log message ***
 *
 *   Revision 1.7  2000/04/02 04:40:26  mud
 *   Changed the policies file that reload loads from 'text/policies/policies'
 *   to 'text/policies/policies_file.pol'.
 *
 *   Revision 1.6  2000/03/02 12:54:52  mud
 *   Zones are now saved/loaded in XML-format.
 *
 *   Revision 1.5  2000/03/02 09:34:56  mud
 *   zone reset now honours the percentage load, 3 new commands added to zone cmds, but not implemented yet
 *
 *   Revision 1.4  2000/03/01 18:06:17  mud
 *   Skeleton code added for zone XML, as well as code for future percentage chance of zone cmds...
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
*   File: db.h                                          Part of CircleMUD *
*  Usage: header file for database handling                               *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#ifndef __DB_H__
#define __DB_H__

#include "structs.h"
#include "dll.h"

#define MAX_RANKS 100	/* need room for all those darn Aiel Ranks! ;) */

/* abilities */
#define ABL_STR   0
#define ABL_CON   1
#define ABL_DEX   2
#define ABL_INT   3
#define ABL_WIS   4
#define ABL_CHA   5
#define ABL_MOVES 6
#define ABL_MANA  7
#define ABL_HIT   8

/*for the Races */
#define MAX_RACES 20
#define MAX_ABILITIES 9

#define RACE_AIEL        0
#define RACE_ALMOTH      1
#define RACE_AMADICIA    2
#define RACE_ANDOR       3
#define RACE_ARADDOMAN   4
#define RACE_BORDERLANDS 5
#define RACE_CAIRHIEN    6
#define RACE_ILLIAN      7
#define RACE_MURANDY     8
#define RACE_SEANCHAN    9
#define RACE_TARABON    10
#define RACE_TARVALON   11
#define RACE_TEAR       12
#define RACE_TREMALKING 13
#define RACE_TWO_RIVERS 14
#define RACE_OGIER      15
#define RACE_COUR_SOUVRA 16
#define RACE_SALDAEA	17
#define RACE_MAYENE		18
#define RACE_GHALDEAN	19

extern const char *hometown[];
extern const int modifiers[MAX_RACES][MAX_ABILITIES];
extern long max_id;

/* For the AFW-business */
extern char *std_afw;
#define AFW_LOOK " (afw)"

/* For the AFK-business */
extern char *std_Flag;
#define AFK_LOOK " (afk)"


/* arbitrary constants used by index_boot() (must be unique) */
#define DB_BOOT_WLD	0
#define DB_BOOT_MOB	1
#define DB_BOOT_OBJ	2
#define DB_BOOT_ZON	3
#define DB_BOOT_SHP	4
#define DB_BOOT_HLP	5
#define DB_BOOT_QST	6
#define DB_BOOT_POL	7
#define DB_BOOT_GLD	8
#define DB_BOOT_TRG     9

/* names of various files and directories */
#define INDEX_FILE	"index"		/* index of world files		*/
#define MINDEX_FILE	"index.mini"	/* ... and for mini-mud-mode	*/
#define WLD_PREFIX	"world/wld"	/* room definitions		*/
#define MOB_PREFIX	"world/mob"	/* monster prototypes		*/
#define OBJ_PREFIX	"world/obj"	/* object prototypes		*/
#define ZON_PREFIX	"world/zon"	/* zon defs & command tables	*/
#define SHP_PREFIX	"world/shp"	/* shop definitions		*/
#define MOB_DIR   "world/prg" /* Mob programs     */
#define HLP_PREFIX	"text/help"	/* for HELP <keyword>		*/
#define HELP_FILE       "help.hlp"      /* default file for help        */
#define QST_PREFIX	"world/qst"	/* For quest files 		*/
#define POL_PREFIX	"text/policies" /* For policy files		*/
#define POLI_FILE	"policy.pol"	/* default file for policy	*/
#define HORSE_PREFIX    "world/horse"   /* For mounts                   */
#define GUILD_PREFIX	"guilds"	/* For guilds			*/
#define TRG_PREFIX      "world/trg"     /* For DGS                      */
#define BRD_PREFIX	"boards"	/* for board messages & data	*/

#define GHLP_FILE 	"text/help/ghelp" /* For guildskills            */
#define CREDITS_FILE	"text/credits"	/* for the 'credits' command	*/
#define NEWS_FILE	"text/news"	/* for the 'news' command	*/
#define OLDNEWS_FILE	"text/oldnews"	/* for the 'oldnews' command	*/
/*#define JOURNAL_FILE    "text/journal"   for the 'journal' command    */
#define MOTD_FILE	"text/motd"	/* messages of the day / mortal	*/
#define IMOTD_FILE	"text/imotd"	/* messages of the day / immort	*/
#define HELP_PAGE_FILE	"text/help/screen" /* for HELP <CR>		*/
#define INFO_FILE	"text/info"	/* for INFO			*/
#define GREETINGS_FILE	"text/greeting"
#define WIZLIST_FILE	"text/wizlist"	/* for WIZLIST			*/
#define IMMLIST_FILE	"text/immlist"	/* for IMMLIST			*/
#define BACKGROUND_FILE	"text/background" /* for the background story	*/
#define HANDBOOK_FILE	"text/handbook"	/* handbook for new immorts	*/
#define GOSSIP_FILE	"text/gossip"	/* contains all the weekly gossip */
#define POLI_PAGE_FILE	"text/policies/policies" /*Polices file*/

#define CRETODO_FILE    "misc/cretodo"  /* for the cretodo command      */
#define IDEA_FILE	"misc/ideas"	/* for the 'idea'-command	*/
#define NEWIDEA_FILE	"misc/newideas"	/* for the 'idea'-command	*/
#define SOCIDEA_FILE	"misc/socideas"	/* for the 'socialidea'-command	*/
#define TYPO_FILE	"misc/typos"	/*         'typo'		*/
#define BUG_FILE	"misc/bugs"	/*         'bug'		*/
#define TODO_FILE	"misc/todo"	/*	   'todo'		*/
#define SOCMESS_FILE	"misc/socials"	/* messgs for social acts	*/
#define XNAME_FILE	"misc/xnames"	/* invalid name substrings	*/

#define PLAYER_FILE	"etc/players"	/* the player database, will soon be obsolete! */
#define PTABLE_FILE     "etc/ptable"    /* XML file for player table    */
#define MAIL_FILE	"etc/plrmail"	/* for the mudmail system	*/
#define BAN_FILE	"etc/badsites"	/* for the siteban system	*/
#define HCONTROL_FILE	"etc/hcontrol"  /* for the house system		*/
#define DNS_FILE	"etc/dns"	/* for the dns cache   * dnsmod */

#define LOGDIR		"../log/"
#define CIRCDIR		"../"

#define LF_SYSLOG     CIRCDIR"syslog"
#define LF_SYSLOG1     LOGDIR"syslog.1"
#define LF_SYSLOG2     LOGDIR"syslog.2" 
#define LF_SYSLOG3     LOGDIR"syslog.3"
#define LF_SYSLOG4     LOGDIR"syslog.4"
#define LF_SYSLOG5     LOGDIR"syslog.5"
#define LF_SYSLOG6     LOGDIR"syslog.6"
#define LF_BADPWS      LOGDIR"badpws"
#define LF_DELETE      LOGDIR"delete"
#define LF_ERRORS      LOGDIR"errors"
#define LF_GODCMDS     LOGDIR"godcmds"
#define LF_LEVELS      LOGDIR"levels"
#define LF_NEWPLAYERS  LOGDIR"newplayers"
#define LF_RESTARTS    LOGDIR"restarts"
#define LF_RIP         LOGDIR"rip"
#define LF_USAGE       LOGDIR"usage"
#define LF_CRASH      CIRCDIR"syslog.CRASH"



/* public procedures in db.c */
void	boot_db(void);
int	create_entry(char *name);
void	zone_update(void);
int	real_room(int virtual);
char	*fread_string(FILE *fl, char *error);
int file_to_string_alloc(char *name, char **buf);
long	get_id_by_name(char *name);
char	*get_name_by_id(long id);

void	save_char(struct char_data *ch, sh_int load_room);
void	init_char(struct char_data *ch);
struct char_data* create_char(void);
struct char_data *read_mobile(int nr, int type);
int	real_mobile(int virtual);
int	vnum_mobile(char *searchname, struct char_data *ch);
int     vnum_mobile_flag(int flag, struct char_data *ch);
void	clear_char(struct char_data *ch);
void	reset_char(struct char_data *ch);
void	free_char(struct char_data *ch);

struct obj_data *create_obj(void);
void	clear_object(struct obj_data *obj);
void	free_obj(struct obj_data *obj);
int	real_object(int virtual);
struct obj_data *read_object(int nr, int type);
int	vnum_object(char *searchname, struct char_data *ch);
int     vnum_object_flag(int flag, struct char_data *ch);

int     vnum_room_flag(int flag, struct char_data *ch);

#define REAL 0
#define VIRTUAL 1

/* structure for the reset commands */
struct reset_com {
   char	command;   /* current command                      */

   bool if_flag;	/* if TRUE: exe only if preceding exe'd */
   long	arg1;		/*                                      */
   long	arg2;		/* Arguments to the command             */
   long	arg3;		/*                                      */
   long line;		/* line number this command appears on  */
   int  percentage;      /* Percentage chance for this cmd to execute */
   /* 
	*  Commands:              *
	*  'M': Read a mobile     *
	*  'O': Read an object    *
	*  'G': Give obj to mob   *
	*  'P': Put obj in obj    *
	*  'G': Obj to char       *
	*  'E': Obj to char equip *
	*  'D': Set state of door *
	*  'R': Remove object?    *
	*  'L': put mob sleeping on furniture obj *
	*  'T': put mob (sitting) on furniture obj *
	*  'U': put obj on furniture obj *
        *  'T': Trigger command   *
   */
 
   /* For DGS */
   char *sarg1;		/* string argument                      */
   char *sarg2;		/* string argument                      */
};



/* zone definition structure. for the 'zone-table'   */
struct zone_data {
   char	*name;		    /* name of this zone                  */
   int	lifespan;           /* how long between resets (minutes)  */
   int	age;                /* current age of this zone (minutes) */
   int	top;                /* upper limit for rooms in this zone */

   int	reset_mode;         /* conditions for reset (see below)   */
   int	number;		    /* virtual number of this zone	  */
   struct reset_com *cmd;   /* command table for reset	          */
   int  lock;               /* Zone is locked for morts if true   */
   /*
	*  Reset mode:                              *
	*  0: Don't reset, and don't update age.    *
	*  1: Reset if no PC's are located in zone. *
	*  2: Just reset.                           *
   */
  DLL   hfl_Editors; /* Huge F***ing List to (in addtition to normal OLC) set names of
		      * people allowed to edit this zone. */
  int all;           /* if true, anyone with the builder command group may edit this zone */
};

typedef struct zone_data zone_t, *zoneptr_t;

/* for queueing zones for update   */
struct reset_q_element {
   long	zone_to_reset;            /* ref to zone_data */
   struct reset_q_element *next;
};



/* structure for the update queue     */
struct reset_q_type {
   struct reset_q_element *head;
   struct reset_q_element *tail;
};

struct player_index_element {
   char	*name;
   char *dname;
   int srank;
   long id;
   long guild;
   long darkie;
   long grank;
   byte race;
   int  level;
   byte class;
   int  deleted;
   byte sex;
   long played;
   int bonded;
   int ashabonded;
   int minion;
   int  wkin;
   time_t last_logon;
   int  disguised;
   int  bm;
   byte multi;
   byte nogain;
};

struct clone_index_el
{
  struct player_index_element *orig;
  long id;
};

typedef struct clone_index_el clone_t,*clone_tp, **clone_tpp;

struct help_index_element {
   char	*keywords;
   char *entry;
   int min_level;
};

struct poli_index_element {
   char	*keywords;
   char *entry;
   int min_level;
};

struct help_indexer_element {
  char *keyword;
  long pos;
  struct help_indexer_element *next;
};

struct poli_indexer_element {
  char *keyword;
  long pos;
  struct poli_indexer_element *next;
};

struct dns_entry { /* dnsmod */
  int ip[4];
  char *name;
  struct dns_entry *next;
};

/* dnsmod - the magic number for dns caching */
#define DNS_HASH_NUM 257


/* don't change these */
#define BAN_NOT 	0
#define BAN_NEW 	1
#define BAN_SELECT	2
#define BAN_ALL		3

#define BANNED_SITE_LENGTH    50
struct ban_list_element {
   char	site[BANNED_SITE_LENGTH+1];
   int	type;
   time_t date;
   char	name[MAX_NAME_LENGTH+1];
   char reason[MAX_TITLE_LENGTH+1];
   struct ban_list_element *next;
};

/* global buffering system */

extern char	buf[MAX_STRING_LENGTH];
extern char	buf1[MAX_STRING_LENGTH];
extern char	buf2[MAX_STRING_LENGTH];
extern char	arg[MAX_STRING_LENGTH];

/*
 * Funcs
 */
extern long asciiflag_conv(char *flag);
extern int psort(const void *a, const void *b);
extern void reset_time(void);
extern void replace_index(struct index_data *old_index,struct index_data *new_index);
extern int vnum_room( char *searchname, struct char_data * ch );
extern int do_vnum_flags(struct char_data *ch, int which, char *flag_string);
extern struct obj_data *clone_obj(struct obj_data *original);
extern char *id_to_name(long pid);
extern void reset_zone(int zone);
extern void save_dns_cache(void);
extern void free_dns_cache(void);
extern void boot_world(void);
extern int get_host_from_cache(struct dns_entry *dnsd);
extern void add_dns_host(struct dns_entry *dnsd, char *hostname);
extern void read_know(struct char_data *ch);
extern int is_empty(int zone_nr);
extern int check_unique_mob(int mobrnum, int mobvnum);
extern int check_unique_obj(int objrnum, int objvnum);
/*
 * Data
 */
extern struct player_special_data dummy_mob;
extern int no_mail;
extern sh_int r_mortal_start_room;
extern sh_int r_immort_start_room;     
extern sh_int r_frozen_start_room;
extern int mini_mud;
extern int no_rent_check;
extern int hotboot;
extern struct dns_entry *dns_cache[DNS_HASH_NUM];
extern struct char_data *character_list;
extern struct obj_data *obj_proto;
extern int top_of_objt;
extern long top_bond_id;
extern long top_ashabond_id;
extern long top_minion_id;
extern long top_idnum;
extern int top_of_p_table;
extern struct player_index_element *player_table;
extern struct index_data *obj_index;
extern struct index_data *mob_index;
extern struct time_info_data time_info;
extern int max_logged_on;
extern char *help;
extern char *poli;
extern struct help_index_element *help_table;
extern struct poli_index_element *poli_table;
extern int top_of_helpt;
extern int top_of_polit;
extern struct help_indexer_element *help_index;
extern struct poli_indexer_element *poli_index;
extern char *credits;
extern char *news;
extern char *oldnews;
/*extern char *journal;*/
extern char *motd;
extern char *imotd;
extern char *info;
extern char *wizlist;
extern char *immlist;
extern char *background; 
extern char *handbook; 
extern char *gossip;
extern char *bugs;
extern char *ideas;
extern char *newideas;
extern char *socialideas;
extern char *todo;
extern char *typos;
extern char *cretodo;

extern struct obj_data *object_list;
extern struct zone_data *zone_table;
extern int top_of_zone_table;
extern struct social_messg *soc_mess_list;
extern int top_of_socialt;
extern int top_of_world;
extern int restrict_run;
extern int immlock;
extern int time_restrict;
extern int questlock;
extern time_t boot_time;
extern time_t hotboot_time;
extern int top_of_aquestt;    
extern struct aq_data *aquest_table;
extern int top_of_mobt;
extern struct char_data *mob_proto;
extern int last_room_rnum;
extern int last_zone_rnum;
extern int last_obj_rnum;
extern int last_mob_rnum;
extern struct index_data **trig_index;
extern int top_of_trigt;
extern struct reset_q_type reset_q;

#endif


