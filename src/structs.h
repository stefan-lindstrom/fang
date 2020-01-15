/*************************************************************************
 *   File: structs.h                                     Part of CircleMUD *
 *  Usage: header file for central structures and contstants               *
 *                                                                         *
 *  All rights reserved.  See license.doc for complete information.        *
 *                                                                         *
 *  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
 *  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
 ************************************************************************ */
#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include "sysdep.h"
#include "bitvector.h"

/* Misc editor defines (for DGS) ********************************************/
/* format modes for format_text */
#define FORMAT_INDENT		(1 << 0)

/* for increasing stats! */

#define MAX_TAINT 0xffffffff
#define MAX_CAN 14

struct cost {
  int level,cost;
};

/* AutoQuests */
#define QUEST_BROKENSWORD (1 << 0) /* 1st bit in first location */

/* For weaves! */
#define NO_POWER   0
#define TRUE_POWER 1
#define SAIDIN     2
#define SAIDAR     3

/* For Craft/Tool */

/* preamble *************************************************************/

#define NOWHERE    -1    /* nil reference for room-database	*/
#define NOTHING	   -1    /* nil reference for objects		*/
#define NOBODY	   -1    /* nil reference for mobiles		*/

#define SPECIAL(name) \
   int (name)(struct char_data *ch, void *me, int cmd, char *argument)

/* misc editor defines **************************************************/

/* format modes for format_text */
#define FORMAT_INDENT         (1 << 0)

/* talent defines *************************************************/
#define TALENT_DREAMER		(1 << 0)
#define TALENT_SNIFFER   	(1 << 1)
#define TALENT_BLOCKGW   	(1 << 2)
#define TALENT_GATE      	(1 << 3)
#define TALENT_SKIM      	(1 << 4)
#define TALENT_INVERT    	(1 << 5)
#define TALENT_HEALSTILL 	(1 << 6)
#define TALENT_BALEFIRE  	(1 << 7)
#define TALENT_CREATEANG 	(1 << 8)
#define TALENT_AFF4HEAL  	(1 << 9)
#define TALENT_AFF4WARD  	(1 << 10)
#define TALENT_POWERFORG 	(1 << 11)
#define TALENT_DISGUISE  	(1 << 12)
#define TALENT_MASKCHAN  	(1 << 13)
#define TALENT_FORETELL  	(1 << 14)
#define TALENT_INVIS     	(1 << 15)
#define TALENT_AURAREAD  	(1 << 16)
#define TALENT_PICKWEAVE 	(1 << 17)
#define TALENT_RESIDUE   	(1 << 18)
#define TALENT_TARGATE   	(1 << 19)
#define TALENT_TREESING		(1 << 20)
#define TALENT_MINCOMPULSION	(1 << 21)
#define TALENT_COMPULSION	(1 << 22)
#define TALENT_UNUSED		(1 << 23)
#define TALENT_ILLUSION		(1 << 24)
#define TALENT_AFF4WEATHER	(1 << 25)
#define TALENT_AFF4PAIN		(1 << 26)

/* room-related defines *************************************************/

/* The cardinal directions: used as index to room_data.dir_option[] */
#define NORTH          0
#define EAST           1
#define SOUTH          2
#define WEST           3
#define UP             4
#define DOWN           5
#define NORTHEAST      6
#define SOUTHEAST      7
#define SOUTHWEST      8
#define NORTHWEST      9


/* Room flags: used in room_data.room_flags */
/* WARNING: In the world files, NEVER set the bits marked "R" ("Reserved") */
#define ROOM_DARK		(1 << 0)   /* Dark			*/
#define ROOM_NOGATE		(1 << 1)   /* You can't gate to here    */
#define ROOM_NOMOB		(1 << 2)   /* MOBs not allowed		*/
#define ROOM_INDOORS		(1 << 3)   /* Indoors			*/
#define ROOM_PEACEFUL		(1 << 4)   /* Violence not allowed	*/
#define ROOM_SOUNDPROOF		(1 << 5)   /* Shouts, gossip blocked	*/
#define ROOM_NOTRACK		(1 << 6)   /* Track won't go through	*/
#define ROOM_NOMAGIC		(1 << 7)   /* Magic not allowed		*/
#define ROOM_TUNNEL		(1 << 8)   /* room for only 1 pers	*/
#define ROOM_PRIVATE		(1 << 9)   /* Can't teleport in		*/
#define ROOM_GODROOM		(1 << 10)  /* LVL_200+ only allowed	*/
#define ROOM_VERY_DARK		(1 << 11)  /* Even Wolfkin are blind    */
#define ROOM_GUILD		(1 << 12)  /* Only for guilded people 	*/
#define ROOM_NORECALL		(1 << 13)  /* No recall from here       */
#define ROOM_OLC		(1 << 14)  /* (R) Modifyable/!compress	*/
#define ROOM_BFS_MARK		(1 << 15)  /* for TRACK!		*/
#define ROOM_WAGON		(1 << 16)  /* Room is actually a wagon  */
#define ROOM_HOUSE		(1 << 17)  /* Room is house owned by pc */
#define ROOM_STEDDING           (1 << 18)  /* Room is a stedding        */
#define ROOM_DREAM              (1 << 19)  /* Room used in dreamenter	*/
#define ROOM_NEWBIEGOTO		(1 << 20)  /* Newbiegoto can be used    */
#define ROOM_NOSPAM		(1 << 21)  /* !SPAM rooms		*/
#define NUM_ROOM_FLAGS 		22

/* Exit info: used in room_data.dir_option.exit_info */
#define EX_ISDOOR		(1 << 0)   /* Exit is a door		*/
#define EX_CLOSED		(1 << 1)   /* The door is closed	*/
#define EX_LOCKED		(1 << 2)   /* The door is locked	*/
#define EX_PICKPROOF		(1 << 3)   /* Lock can't be picked	*/
#define EX_HIDDEN               (1 << 4)   /* Exit is hidden            */


/* Sector types: used in room_data.sector_type */
#define SECT_INSIDE          0		   /* Indoors			*/
#define SECT_CITY            1		   /* In a city			*/
#define SECT_FIELD           2		   /* In a field		*/
#define SECT_FOREST          3		   /* In a forest		*/
#define SECT_HILLS           4		   /* In the hills		*/
#define SECT_MOUNTAIN        5		   /* On a mountain		*/
#define SECT_WATER_SWIM      6		   /* Swimmable water		*/
#define SECT_WATER_NOSWIM    7		   /* Water - need a boat	*/
#define SECT_UNDERWATER	     8		   /* Underwater		*/
#define SECT_FLYING	     9		   /* Wheee!			*/


/* char and mob-related defines *****************************************/


/* PC classes */
#define CLASS_UNDEFINED	  -1
#define CLASS_ADVENTURER  0
#define CLASS_SOLDIER     1
#define CLASS_CRAFTSMAN   2
#define CLASS_THIEF       3
#define CLASS_WISDOM      4
#define CLASS_WILDER      5
#define CLASS_CHANNELER   6

#define NUM_CLASSES	  7  /* This must be the number of classes!! */

/* Sex */
#define SEX_NEUTRAL   0
#define SEX_MALE      1
#define SEX_FEMALE    2

/* Positions */
#define POS_DEAD       0	/* dead			*/
#define POS_MORTALLYW  1	/* mortally wounded	*/
#define POS_INCAP      2	/* incapacitated	*/
#define POS_STUNNED    3	/* stunned		*/
#define POS_SLEEPING   4	/* sleeping		*/
#define POS_RESTING    5	/* resting		*/
#define POS_SITTING    6	/* sitting		*/
#define POS_FIGHTING   7	/* fighting		*/
#define POS_STANDING   8	/* standing		*/
#define POS_SUBDUED    9        /* subdued */


/* Player flags: used by char_data.char_specials.act */
#define PLR_SNIFFER	(1 << 0)   /* Player is a sniffer		*/
#define PLR_NOBLE	(1 << 1)   /* Player is a noble one		*/
#define PLR_FROZEN	(1 << 2)   /* Player is frozen			*/
#define PLR_DONTSET     (1 << 3)   /* Don't EVER set (ISNPC bit)	*/
#define PLR_WRITING	(1 << 4)   /* Player writing (board/mail/olc)	*/
#define PLR_MAILING	(1 << 5)   /* Player is writing mail		*/
#define PLR_CRASH	(1 << 6)   /* Player needs to be crash-saved	*/
#define PLR_SITEOK	(1 << 7)   /* Player has been site-cleared	*/
#define PLR_NOSHOUT	(1 << 8)   /* Player not allowed to shout/goss	*/
#define PLR_NOTITLE	(1 << 9)   /* Player not allowed to set title	*/
#define PLR_DELETED	(1 << 10)  /* Player deleted - space reusable	*/
#define PLR_LOADROOM	(1 << 11)  /* Player uses nonstandard loadroom	*/
#define PLR_NOWIZLIST	(1 << 12)  /* Player shouldn't be on wizlist	*/
#define PLR_NODELETE	(1 << 13)  /* Player shouldn't be deleted	*/
#define PLR_INVSTART	(1 << 14)  /* Player should enter game wizinvis	*/
#define PLR_CRYO	(1 << 15)  /* Player is cryo-saved (purge prog)	*/
#define PLR_NOTELL      (1 << 16)  /* Player forbidden to use Tell      */
#define PLR_NOASK	(1 << 17)  /* Player forbidden to use Ask       */
#define PLR_WIZNOSNOOP  (1 << 18)  /* Immortal player cannot be snooped */
#define PLR_NOGECHO	(1 << 19)  /* Immortal player wont see gecho's  */
#define PLR_NOGAIN      (1 << 20)  /* XPs and stat gains prohibited     */
#define PLR_MMAILING    (1 << 21)  /* Player is MultiMailing!		*/
#define PLR_GMAILING    (1 << 22)  /* Player is GuildMailing!           */
#define PLR_SITTER      (1 << 23)  /* Player is a Sitter for their Ajah */
#define PLR_HEAD        (1 << 24)  /* Player is Head of their Ajah      */
#define PLR_NOMAIL	(1 << 25)  /* Player is not allowed to send mail */
#define PLR_CPROMPT     (1 << 26)  /* player has a custim prompt */ 
#define PLR_CANTGECHO   (1 << 27)  /* User cannot GECHO.  Admin Set Flag */
#define PLR_CANTPOST    (1 << 28)  /* User cannot post to boards.  Admin Set Flag */
#define PLR_CANMULTI    (1 << 29)  /* User cannot post to boards.  Admin Set Flag */
#define PLR_NOPRAY	(1 << 30)  /* User cannot pray */

#define LOG_CLONE	(1 << 0)   /* CLONE syslog messages */
#define LOG_SET		(1 << 1)   /* SET syslog messages */
#define LOG_PLEVEL	(1 << 2)   /* PLEVELS syslog messages */
#define LOG_PDEATH	(1 << 3)   /* PDEATHS syslog messages */
#define LOG_CONN	(1 << 4)   /* CONNECTIONS syslog messages */
#define LOG_OLC		(1 << 5)   /* OLC syslog messages */
#define LOG_SNOOP	(1 << 6)   /* SNOOP syslog messages */
#define LOG_ZRESET	(1 << 7)   /* ZRESET syslog messages */
#define LOG_ZERROR	(1 << 8)   /* ZERROR syslog messages */
#define LOG_NOHELP	(1 << 9)   /* NOHELP syslog messages */
#define LOG_DGSERROR	(1 << 10)  /* DGSERROR syslog messages */
#define LOG_TOOLCRAFT	(1 << 11)  /* TOOLCRAFT syslog messages */
#define LOG_LOAD	(1 << 12)  /* LOAD syslog messages */
#define LOG_NOPOLICY	(1 << 13)  /* NOPOLICY syslog messages */
#define LOG_GUILDIESET	(1 << 14)  /* GUILDIESET syslog messages */
#define LOG_GUILDSET 	(1 << 15)  /* GUILDSET syslog messages */
#define LOG_SKILLSET	(1 << 16)  /* SKILLSET syslog messages */
#define LOG_PREPORT	(1 << 17)  /* PREPORT syslog messages */
#define LOG_AWARD	(1 << 18)  /* AWARD syslog messages */
#define LOG_REVIVE      (1 << 19)  /* REVIVE syslog messages */
#define LOG_COMM	(1 << 20)  /* *ECHO syslog messages */
#define LOG_GOD         (1 << 21)  /* GOD syslog messages */
#define LOG_RETAINER    (1 << 22)  /* RETAINER syslog messages */
#define LOG_DEBUG       (1 << 23)  /* DEBUG syslog messages */
#define LOG_ERROR       (1 << 24)  /* System Error syslog messages */
#define LOG_GUILD       (1 << 25)  /* GUILD/DEGUILD syslog messages */

#define NUM_LOG_FLAGS	25

#define SYS_CLONE	0	   /* CLONE syslog messages */
#define SYS_SET		1	   /* SET syslog messages */
#define SYS_PLEVEL	2	   /* PLEVELS syslog messages */
#define SYS_PDEATH	3	   /* PDEATHS syslog messages */
#define SYS_CONN	4	   /* CONNECTIONS syslog messages */
#define SYS_OLC		5	   /* OLC syslog messages */
#define SYS_SNOOP	6	   /* SNOOP syslog messages */
#define SYS_ZRESET	7	   /* ZRESET syslog messages */
#define SYS_ZERROR	8	   /* ZERROR syslog messages */
#define SYS_NOHELP	9	   /* NOHELP syslog messages */
#define SYS_DGSERROR	10	   /* DGSERROR syslog messages */
#define SYS_TOOLCRAFT	11	   /* TOOLCRAFT syslog messages */
#define SYS_LOAD	12	   /* LOAD syslog messages */
#define SYS_NOPOLICY	13	   /* NOPOLICY syslog messages */
#define SYS_GUILDIESET	14	   /* GUILDIESET syslog messages */
#define SYS_GUILDSET 	15	   /* GUILDSET syslog messages */
#define SYS_SKILLSET	16	   /* SKILLSET syslog messages */
#define SYS_PREPORT	17	   /* PREPORT syslog messages */
#define SYS_AWARD	18	   /* AWARD syslog messages */
#define SYS_REVIVE	19	   /* REVIVE syslog messages */
#define SYS_COMM	20
#define SYS_GOD		21
#define SYS_RETAINER	22
#define SYS_DEBUG	23
#define SYS_ERROR	24
#define SYS_GUILD       25

/* Mobile flags: used by char_data.char_specials.act */
#define MOB_SPEC         (1 << 0)  /* Mob has a callable spec-proc	*/
#define MOB_SENTINEL     (1 << 1)  /* Mob should not move		*/
#define MOB_SCAVENGER    (1 << 2)  /* Mob picks up stuff on the ground	*/
#define MOB_ISNPC        (1 << 3)  /* (R) Automatically set on all Mobs	*/
#define MOB_AWARE	 (1 << 4)  /* Mob can't be backstabbed		*/
#define MOB_AGGRESSIVE   (1 << 5)  /* Mob hits players in the room	*/
#define MOB_STAY_ZONE    (1 << 6)  /* Mob shouldn't wander out of zone	*/
#define MOB_WIMPY        (1 << 7)  /* Mob flees if severely injured	*/
#define MOB_UNUSED	 (1 << 8)  /* Unused flag                      */
#define MOB_TAR		 (1 << 9)  /* Mob is a Tel'aran'rhiod one	*/
#define MOB_TRACKER	 (1 << 10) /* 					*/
#define MOB_MEMORY	 (1 << 11) /* remember attackers if attacked	*/
#define MOB_HELPER	 (1 << 12) /* attack PCs fighting other NPCs	*/
#define MOB_NOCHARM	 (1 << 13) /* Mob can't be charmed		*/
#define MOB_NOMAGIC	 (1 << 14) /* Mob can't be zapped		*/
#define MOB_NOSLEEP	 (1 << 15) /* Mob can't be slept		*/
#define MOB_NOBASH	 (1 << 16) /* Mob can't be bashed (e.g. trees)	*/
#define MOB_NOBLIND	 (1 << 17) /* Mob can't be blinded		*/
#define MOB_NOTRIP       (1 << 18) /* Mob can't be tripped		*/
#define MOB_NOSTUN       (1 << 19) /* Mob can't be stunned		*/
#define MOB_HUMAN        (1 << 20) /* Mob is human			*/
#define MOB_SHADOWSPAWN  (1 << 21) /* Mob is Shadowspawn		*/
#define MOB_CHANNELER    (1 << 22) /* Mob can channel 			*/
#define MOB_NOKILL       (1 << 23) /* Mob can't take damage from attacks, nor die */
#define MOB_UNIQUE       (1 << 24) /* Mob is unique and mud can NEVER load more than one! */

 /* Mobile skills flags: used by char_data.mob_specials.skillvector */

#define MOB_BASH	 (1 << 0) /* Mob can bash */
#define MOB_TRIP	 (1 << 1) /* Mob can trip */
#define MOB_DIRTKICK	 (1 << 2) /* Mob can dirtkick */
#define MOB_KICK	 (1 << 3) /* Mob can kick */
#define MOB_DISARM	 (1 << 4) /* Mob can disarm */
#define MOB_STUN	 (1 << 5) /* Mob can stun */

/* Preference flags: used by char_data.player_specials.pref */
#define PRF_BRIEF       (1 << 0)  /* Room descs won't normally be shown	*/
#define PRF_COMPACT     (1 << 1)  /* No extra CRLF pair before prompts	*/
#define PRF_DEAF	(1 << 2)  /* Can't hear shouts			*/
#define PRF_NOTELL	(1 << 3)  /* Can't receive tells		*/
#define PRF_DISPHP	(1 << 4)  /* Display hit points in prompt	*/
#define PRF_DISPMANA	(1 << 5)  /* Display mana points in prompt	*/
#define PRF_DISPMOVE	(1 << 6)  /* Display move points in prompt	*/
#define PRF_AUTOEXIT	(1 << 7)  /* Display exits in a room		*/
#define PRF_NOHASSLE	(1 << 8)  /* Aggr mobs won't attack		*/
#define PRF_QUEST	(1 << 9)  /* On quest				*/
#define PRF_NOSPAM	(1 << 10) /* Won't be spammed with fight-mess.	*/
#define PRF_NOREPEAT	(1 << 11) /* No repetition of comm commands	*/
#define PRF_HOLYLIGHT	(1 << 12) /* Can see in dark			*/
#define PRF_COLOR_1	(1 << 13) /* Color (low bit)			*/
#define PRF_COLOR_2	(1 << 14) /* Color (high bit)			*/
#define PRF_NOWIZ	(1 << 15) /* Can't hear wizline			*/
#define PRF_LOG1	(1 << 16) /* On-line System Log (low bit)	*/
#define PRF_LOG2	(1 << 17) /* On-line System Log (high bit)	*/
#define PRF_NOAUCT	(1 << 18) /* Can't hear auction channel		*/
#define PRF_IC	        (1 << 19) /* Character is IC      		*/
#define PRF_NOOOC	(1 << 20) /* Can't hear OOC channel		*/
#define PRF_ROOMFLAGS	(1 << 21) /* Can see room flags (ROOM_x)	*/
#define PRF_AFK         (1 << 22) /* Is AFK and can't hear anything     */
#define PRF_NOGDT       (1 << 23) /* Can't hear GDT              	*/
#define PRF_GRASPING    (1 << 24) /* character is Grasping the OP	*/
#define PRF_TGRASP      (1 << 25) /* character is grasping the TP       */
#define PRF_NOWT        (1 << 26) /* Player won't listen to WolfTell    */
#define PRF_DISPEXP	(1 << 27) /* Display exp to lvl in prompt       */
#define PRF_DISPGOLD	(1 << 28) /* Display carried gold in prompt     */
#define PRF_LOOKAHEAD	(1 << 29) /* Will scan forth in walking dir     */
#define PRF_COMMSPAM	(1 << 30) /* PLayer won't hear Tower Talk       */
#define PRF_PERCWIMPY	(1 << 31) /* Player are using percentual wimpy-lvl */


#define PRF2_SLICEWEAVE  (1 << 0)  /* Player slices all weaves at him    */
#define PRF2_NOHUNGER    (1 << 1)  /* Player won't get any hunger messages */
#define PRF2_NOKILLMESS  (1 << 2)  /* Player won't hear death cries. */
#define PRF2_UNUSED3     (1 << 3)  /* Player won't hear darkfriend channel. */
#define PRF2_UNUSED4	 (1 << 4)  /* PLayer won't hear Ogier Talk */
#define PRF2_NOQUEST	 (1 << 5)  /* Player wont hear question-channel */
#define PRF2_NOOBSCENE	 (1 << 6)  /* Player won't hear insult-channel */
#define PRF2_TIMESTAMP	 (1 << 7)  /* Player wants timestamps where avail */
#define PRF2_AFW         (1 << 8)  /* Is AFW and can't hear anything     */
#define PRF2_NOPOSEID	 (1 << 9)  /* No pose identification will be shown. */
#define PRF2_NOSONG	 (1 << 10) /* Won't hear song channel */
#define PRF2_NOWEATHER   (1 << 11) /* Won't see weather messages */
#define PRF2_UNUSED12    (1 << 12) /* Use old style score layout. */
#define PRF2_NOPADD      (1 << 13) /* Can't be added to PChans */
#define PRF2_RPTAG	 (1 << 14) /* RP commands preceeded with a tag */
#define PRF2_XLOGTAG     (1 << 15) /* XLOG Tag?  IMM Toggle */

/* Session flags, not saved upon quit */

#define SESS_REPLYLOCK	(1 << 0)  /* User has REPLYLOCK on */
#define SESS_EAVESDROP  (1 << 1)  /* User is EAVESDROPping */

/* Permanent flags, like Gentling, perm. blindness and such */
#define PRM_GENTLED     (1 << 0)  /* Character is cut off from the OP	*/
#define PRM_BLIND	(1 << 1)  /* character is permanently blind     */
#define PRM_DEAF	(1 << 2)  /* character is permanently deaf      */
#define PRM_MUTE	(1 << 3)  /* character is permanently mute	*/
#define PRM_CRIPPLED	(1 << 4)  /* character is permanently crippled  */
#define PRM_RH_CUT	(1 << 5)  /* character's right hand is severed  */
#define PRM_LH_CUT      (1 << 6)  /* character's left hand is severed   */
#define PRM_TAINTRES    (1 << 7)  /* Character is slightly resistant to taint */
#define PRM_CONVED      (1 << 8)  /* Character has used manaconv */

#define DRF_GHOLAM      (1 << 0) /* Character is a Gholam, only for Df-guild*/
#define DRF_GREYMAN     (1 << 1) /* character is a greyman,  ----||---- */

/* Affect bits: used in char_data.char_specials.saved.affected_by */
/* WARNING: In the world files, NEVER set the bits marked "R" ("Reserved") */
#define AFF_BLIND             (1 << 0)	   /* (R) Char is blind		*/
#define AFF_INVISIBLE         (1 << 1)	   /* Char is invisible		*/
#define AFF_ENDURANCE	      (1 << 2)	   /* Char is more endurancing  */
#define AFF_SUFFOCATING       (1 << 3)	   /* Char is slowly dying      */
#define AFF_DETECT_INVIS      (1 << 4)	   /* Char is sensitive to invis*/
#define AFF_SENSE_LIFE        (1 << 5)	   /* Char can sense hidden life*/
#define AFF_WRAPPED	      (1 << 6)	   /* Char is wrapped		*/
#define AFF_RUNNING           (1 << 7)     /* Char not allwed to teach  */
#define AFF_NOTEACH           (1 << 8)	   /* Char can't teach		*/
#define AFF_GROUP             (1 << 9)	   /* (R) Char is grouped	*/
#define AFF_GAGED             (1 << 10)	   /* Char is gaged		*/
#define AFF_INFRAVISION       (1 << 11)	   /* Char can see in dark	*/
#define AFF_POISON            (1 << 12)	   /* (R) Char is poisoned	*/
#define AFF_SHIELDED	      (1 << 13)	   /* Char is shielded		*/
#define AFF_LIPREADING	      (1 << 14)	   /* Char reads lips		*/
#define AFF_SLEEP             (1 << 15)	   /* (R) Char magically asleep	*/
#define AFF_NOTRACK	      (1 << 16)	   /* Char can't be tracked	*/
#define AFF_DREAMING	      (1 << 17)	   /* Char is in Tel'aran'rhiod	*/
#define AFF_ARMOR	      (1 << 18)	   /* Char is armored, magically*/
#define AFF_SNEAK             (1 << 19)	   /* Char can move quietly	*/
#define AFF_HIDE              (1 << 20)	   /* Char is hidden		*/
#define AFF_CAMOUFLAGED	      (1 << 21)	   /* Aiel guild affection.	*/
#define AFF_CHARM             (1 << 22)	   /* Char is charmed		*/
#define AFF_BANDAGED          (1 << 23)    /* Char is properly bandaged */
#define AFF_VEILED	      (1 << 24)    /* Aiel char is veiled, ware */
#define AFF_TAILING	      (1 << 25)    /* Gleeman is tailing secr.  */
#define AFF_MORALE	      (1 << 26)	   /* Char is under morale      */
#define AFF_STUNNED	      (1 << 27)	   /* Char is stunned by stun-skill */
#define AFF_NOSEARCH          (1 << 28)    /* Char cannot search for springs*/
#define AFF_NOREINFORCE       (1 << 29)    /* Char cannot use reinforce */
#define AFF_MASTERY           (1 << 30)    /* Char is an Xmaster */
#define AFF_NOQUIT	      (1 << 31)    /* Char is not allowed to quit*/

             /* Second flag bitvectors defines */
#define AFF2_TAR_GATE	      (1 << 0)     /* Char is in TAR in flesh   */
#define AFF2_NODIRTKICK       (1 << 1)     /* Char can't be dirtkicked  */
#define AFF2_CONFUSION	      (1 << 2)     /* Char is magically confused*/
#define AFF2_UNBALANCE        (1 << 3)     /* Char's balance is decreased*/
#define AFF2_CONCENTRATE      (1 << 4)     /* Char is concentrating (sic!) */
#define AFF2_STUBBORNESS      (1 << 5)     /* Char is stubborn          */
#define AFF2_ARRESTED	      (1 << 6)     /* Char is arrested          */
#define AFF2_NOFINDHERB	      (1 << 7)     /* Char can't find herbs     */
#define AFF2_COMPULSED	      (1 << 8)	   /* Char is affected by Compulsion */
#define AFF2_SHIELDWALL       (1 << 9)     /* Char is behind a shieldwall */
#define AFF2_NOPATROL         (1 << 10)    /* Char can't call a patrol */
#define AFF2_BADMORALE        (1 << 11)    /* Char has play-instrument aff */
#define AFF2_NOFLEE	      (1 << 12)    /* Char won't be able to flee! */
#define AFF2_TRACTION	      (1 << 13)	   /* Char can't use traction  */
#define AFF2_RAGE	      (1 << 14)    /* Char is in rage          */
#define AFF2_DARKDREAM	      (1 << 15)	   /* Char is affected by darkdream! */
#define AFF2_HALT	      (1 << 16)    /* Char can't do anything!  */
#define AFF2_DEFENSE	      (1 << 17)    /* Char is defending */
#define AFF2_MASKED           (1 << 18)    /* Char is masking channeling ability! */
#define AFF2_ARROGANCE        (1 << 19)    /* Char is arrogant - could be replaced by shieldwall */
#define AFF2_INSULTED         (1 << 20)    /* Char is insulted */
#define AFF2_DISCOVERED       (1 << 21)    /* Char has been discovered by locate */
#define AFF2_SUBDUED          (1 << 22)    /* Char has been subdued */
#define AFF2_LONGSLEEP        (1 << 23)    /* Char is sleeping late :) */
#define AFF2_NOHERBIDENT      (1 << 24)    /* Char just tried to identify an herb */
#define AFF2_TARBLUR          (1 << 25)    /* Char looks blurred in TAR */
#define AFF2_NOBARGAIN	      (1 << 26)    /* Char cannot use BARGAIN */
#define AFF2_BARGAIN          (1 << 27)    /* Char is Barganing */
#define AFF2_NOCHARM	      (1 << 28)	   /* Char cannot use CHARM */
#define AFF2_CHARM            (1 << 29)    /* Char is Charming */
#define AFF2_AWARE	      (1 << 30)    /* Char cannot be backstabbed */
#define AFF2_ADRENALIN        (1 << 31)    /* Char is affected by Adrenalin */

/* Holy Trout, we've used 64 player affects... Here's another 32. */

#define AFF3_NOKNIFETOSS      (1 << 0)     /* Char cannot Knifetoss */


 /* Room affection-flags! */
#define RAFF_QUICK         	0	/* Update every sec 		*/
#define RAFF_SLOW 		1       /* Update every MUDhr		*/

#define RAFF_DARKNESS	      (1 << 0) 	   /* Room is magically dark    */
#define RAFF_LIGHT	      (1 << 1)     /* Room is magically lighted */
#define RAFF_EAVESDROPPING    (1 << 2)     /* Room is m. snooped upon	*/
#define RAFF_DOMEOFAIR        (1 << 3)     /* Room is shielded by a DoA */
#define RAFF_DOMEOFFIRE	      (1 << 4)     /* Room is protected by a DoF*/
#define RAFF_DOMEOFFROST      (1 << 5)     /* Room is protected by DoFR */
#define RAFF_WARDED_HUMAN     (1 << 6)     /* Room is warded against humans */
#define RAFF_FOG	      (1 << 7)     /* Room is fogged            */
#define RAFF_TRAP	      (1 << 8)     /* Room has a trap weave set */
#define RAFF_COLD	      (1 << 9)     /* Room is deadly cold       */
#define RAFF_HOT	      (1 << 10)    /* Room is scorching hot     */
#define RAFF_FORTIFY	      (1 << 11)    /* Room is fortified         */
#define RAFF_ROOMSEAL         (1 << 12)    /* Room is sealed by weave   */

/* Modes of connectedness: used by descriptor_data.state */
#define CON_PLAYING	 0		/* Playing - Nominal state	*/
#define CON_CLOSE	 1		/* Disconnecting		*/
#define CON_GET_NAME	 2		/* By what name ..?		*/
#define CON_NAME_CNFRM	 3		/* Did I get that right, x?	*/
#define CON_PASSWORD	 4		/* Password:			*/
#define CON_NEWPASSWD	 5		/* Give me a password for x	*/
#define CON_CNFPASSWD	 6		/* Please retype password:	*/
#define CON_ADV_OR_SIM   7              /* Advanced or simple creation  */
#define CON_RMOTD	 9		/* PRESS RETURN after MOTD	*/
#define CON_OEDIT	 17		/*. OLC mode - object edit     .*/
#define CON_REDIT	 18		/*. OLC mode - room edit       .*/
#define CON_ZEDIT	 19		/*. OLC mode - zone info edit  .*/
#define CON_MEDIT	 20		/*. OLC mode - mobile edit     .*/
#define CON_SEDIT	 21		/*. OLC mode - shop edit       .*/
#define CON_CREATE	 22		/* Our creatin-routine          */
#define CON_TOOL	 23		/* For tool-skill!		*/
#define CON_CRAFT	 24		/* For Craft-skill!		*/
#define CON_QEDIT	 25		/* For Autoquests		*/
#define CON_GOSSIP	 26
#define OLD_CON_HORSEEDIT    27
#define CON_PEDIT	 28
#define CON_TEXTED	 29		/* For text editor		*/
#define CON_HEDIT      	 30		/* OLC - Help edit		*/
#define CON_TRIGEDIT     31		/*. OLC mode - trigger edit    .*/
#define CON_STONEEDIT    32
#define CON_AEDIT        33             /* OLC - action edit            */
#define CON_MSGEDIT	 34             /* message editor               */

/* Character equipment positions: used as index for char_data.equipment[] */
/* NOTE: Don't confuse these constants with the ITEM_ bitvectors
   which control the valid places you can wear a piece of equipment */
#define WEAR_LIGHT      0
#define WEAR_FINGER_R   1
#define WEAR_FINGER_L   2
#define WEAR_NECK_1     3
#define WEAR_NECK_2     4
#define WEAR_BODY       5
#define WEAR_HEAD       6
#define WEAR_LEGS       7
#define WEAR_FEET       8
#define WEAR_HANDS      9
#define WEAR_ARMS      10
#define WEAR_SHIELD    11
#define WEAR_ABOUT     12
#define WEAR_WAIST     13
#define WEAR_WRIST_R   14
#define WEAR_WRIST_L   15
#define WEAR_WIELD_R   16
#define WEAR_WIELD_L   17
#define WEAR_HOLD      18
#define WEAR_WIELD_DUAL 19 /* only to avoid ooffending cmds in zone-parsing! */
#define WEAR_EAR_R     20
#define WEAR_EAR_L     21
#define WEAR_NOSE      22

#define NUM_WEARS      23	/* This must be the # of eq positions!! */


/* object-related defines ********************************************/


/* Item types: used by obj_data.obj_flags.type_flag */
#define ITEM_LIGHT      1		/* Item is a light source	*/
#define ITEM_INSTRUMENT 2		/* Item is a musical instrument	*/
#define ITEM_HERB       3		/* Item is a herb		*/
#define ITEM_HEALING    4		/* Item is a %ual healing potion*/
#define ITEM_WEAPON     5		/* Item is a weapon		*/
#define ITEM_BOW    	6		/* Item is a bow		*/
#define ITEM_ARROW    	7		/* Item is an arrow		*/
#define ITEM_TREASURE   8		/* Item is a treasure, not gold	*/
#define ITEM_ARMOR      9		/* Item is armor		*/
#define ITEM_POTION    10 		/* Item is a potion		*/
#define ITEM_WORN      11		/* Unimplemented		*/
#define ITEM_OTHER     12		/* Misc object			*/
#define ITEM_TRASH     13		/* Trash - shopkeeps won't buy	*/
#define ITEM_WAGON     14		/* Item is a wagon		*/
#define ITEM_CONTAINER 15		/* Item is a container		*/
#define ITEM_NOTE      16		/* Item is note 		*/
#define ITEM_DRINKCON  17		/* Item is a drink container	*/
#define ITEM_KEY       18		/* Item is a key		*/
#define ITEM_FOOD      19		/* Item is food			*/
#define ITEM_MONEY     20		/* Item is money (gold)		*/
#define ITEM_PEN       21		/* Item is a pen		*/
#define ITEM_BOAT      22		/* Item is a boat		*/
#define ITEM_FOUNTAIN  23		/* Item is a fountain		*/
#define ITEM_FURNITURE 24               /* Item is furniture            */
#define OLD_ITEM_MOUNTFOOD 25               /* Item is what mounts eat      */
#define ITEM_NONAMEBOARD 26		/* Any board set with this won't show player names */
#define ITEM_DAGGER    27		/* Item is a dagger (throwing)  */

/* Take/Wear flags: used by obj_data.obj_flags.wear_flags */
#define ITEM_WEAR_TAKE		(1 << 0)  /* Item can be takes		*/
#define ITEM_WEAR_FINGER	(1 << 1)  /* Can be worn on finger	*/
#define ITEM_WEAR_NECK		(1 << 2)  /* Can be worn around neck 	*/
#define ITEM_WEAR_BODY		(1 << 3)  /* Can be worn on body 	*/
#define ITEM_WEAR_HEAD		(1 << 4)  /* Can be worn on head 	*/
#define ITEM_WEAR_LEGS		(1 << 5)  /* Can be worn on legs	*/
#define ITEM_WEAR_FEET		(1 << 6)  /* Can be worn on feet	*/
#define ITEM_WEAR_HANDS		(1 << 7)  /* Can be worn on hands	*/
#define ITEM_WEAR_ARMS		(1 << 8)  /* Can be worn on arms	*/
#define ITEM_WEAR_SHIELD	(1 << 9)  /* Can be used as a shield	*/
#define ITEM_WEAR_ABOUT		(1 << 10) /* Can be worn about body 	*/
#define ITEM_WEAR_WAIST 	(1 << 11) /* Can be worn around waist 	*/
#define ITEM_WEAR_WRIST		(1 << 12) /* Can be worn on wrist 	*/
#define ITEM_WEAR_WIELD		(1 << 13) /* Can be wielded		*/
#define ITEM_WEAR_HOLD		(1 << 14) /* Can be held		*/
#define ITEM_WEAR_DUALWIELD     (1 << 15) /* Wielded in BOTH hands!     */
#define ITEM_WEAR_EAR           (1 << 16) /* con be worn in(?) ears     */
#define ITEM_WEAR_NOSE          (1 << 17) /* Can be worn in nose....    */

/* Extra object flags: used by obj_data.obj_flags.extra_flags */
#define ITEM_ANGREAL       (1 << 0)	/* Item is an angreal		*/
#define ITEM_SAANGREAL     (1 << 1)	/* Item is a sa'angreal		*/
#define ITEM_TERANGREAL    (1 << 2)	/* Item is a terangreal		*/
#define ITEM_NODONATE      (1 << 3)	/* Item cannot be donated	*/
#define ITEM_TAR	   (1 << 4)	/* Item is only in Tel'aran'rhiod*/
#define ITEM_INVISIBLE     (1 << 5)	/* Item is invisible		*/
#define ITEM_NOTOOL        (1 << 6)	/* Item can't be tooled		*/
#define ITEM_NODROP        (1 << 7)	/* Can't be dropped		*/
#define ITEM_NOMALE        (1 << 8)	/* *'angreal not usable by men	*/
#define ITEM_NOFEMALE      (1 << 9)	/* *'angreal not usable by women*/
#define ITEM_SUNG          (1 << 10)	/* Item is sung by an Ogier	*/
#define ITEM_NOPULL        (1 << 11)	/* Can't pull -Wagon-	*/
#define ITEM_NOBREAK       (1 << 12)	/* Item can't break/wear down   */
#define ITEM_NOSAVE        (1 << 13)	/* Never saves           	*/
#define ITEM_UNUSED14	   (1 << 14)	/* Not usable by thieves	*/
#define ITEM_UNUSED15      (1 << 15)	/* Not usable by warriors	*/
#define ITEM_NOSELL	   (1 << 16)	/* Shopkeepers won't touch it	*/
#define ITEM_THROW	   (1 << 17)    /* Item can be used with throw-skill */
#define ITEM_ENHANCED	   (1 << 18)    /* Item has been enhanced */
#define ITEM_ADDED         (1 << 19)    /* STealing this. if item's tooled, and had original keywords added to it, this bit is set! */
#define ITEM_UNIQUE        (1 << 20)    /* only 1 can ever be loaded, !SAVE is implied */

/* Modifier constants used with obj affects ('A' fields) */
#define APPLY_NONE              0	/* No effect			*/
#define APPLY_STR               1	/* Apply to strength		*/
#define APPLY_DEX               2	/* Apply to dexterity		*/
#define APPLY_INT               3	/* Apply to constitution	*/
#define APPLY_WIS               4	/* Apply to wisdom		*/
#define APPLY_CON               5	/* Apply to constitution	*/
#define APPLY_CHA		6	/* Apply to charisma		*/
#define APPLY_UNUSED1           7	/* Reserved			*/
#define APPLY_UNUSED2           8	/* Reserved			*/
#define APPLY_ABS               9	/* Apply to age			*/
#define APPLY_UNUSED4          10	/* Apply to weight		*/
#define APPLY_UNUSED5          11	/* Apply to height		*/
#define APPLY_MANA             12	/* Apply to max mana		*/
#define APPLY_HIT              13	/* Apply to max hit points	*/
#define APPLY_MOVE             14	/* Apply to max move points	*/
#define APPLY_UNUSED6          15	/* Reserved			*/
#define APPLY_UNUSED7          16	/* Reserved			*/
#define APPLY_AC               17	/* Apply to Armor Class		*/
#define APPLY_HITROLL          18	/* Apply to hitroll		*/
#define APPLY_DAMROLL          19	/* Apply to damage roll		*/
#define APPLY_XATTACKS         20	/* Apply to extra attacks 	*/
#define APPLY_UNUSED9          21	/* Apply to save throw: rods	*/
#define APPLY_UNUSED10         22	/* Apply to save throw: petrif	*/
#define APPLY_UNUSED11         23	/* Apply to save throw: breath	*/
#define APPLY_UNUSED12         24	/* Apply to save throw: spells	*/


/* Container flags - value[1] */
#define CONT_CLOSEABLE      (1 << 0)	/* Container can be closed	*/
#define CONT_PICKPROOF      (1 << 1)	/* Container is pickproof	*/
#define CONT_CLOSED         (1 << 2)	/* Container is closed		*/
#define CONT_LOCKED         (1 << 3)	/* Container is locked		*/



/* Some different kind of liquids for use in values of drink containers */
#define LIQ_WATER      0
#define LIQ_BEER       1
#define LIQ_WINE       2
#define LIQ_ALE        3
#define LIQ_DARKALE    4
#define LIQ_OOSQUAI    5
#define LIQ_WHISKY     6
#define LIQ_LEMONADE   7
#define LIQ_FIREBRT    8
#define LIQ_LOCALSPC   9
#define LIQ_SLIME      10
#define LIQ_MILK       11
#define LIQ_TEA        12
#define LIQ_COFFE      13
#define LIQ_BLOOD      14
#define LIQ_SALTWATER  15
#define LIQ_CLEARWATER 16
#define LIQ_BRANDY     17
#define LIQ_MAPLES     18
#define LIQ_SOAPYWATER 19
#define LIQ_EGGNOG     20
	
/**************************other miscellaneous defines *******************************************/


/* Player conditions */
#define DRUNK        0
#define FULL         1
#define THIRST       2


/* Sun state for weather_data */
#define SUN_DARK	0
#define SUN_RISE	1
#define SUN_LIGHT	2
#define SUN_SET		3


/* Sky conditions for weather_data */
#define SKY_CLOUDLESS	0
#define SKY_CLOUDY	1
#define SKY_RAINING	2
#define SKY_LIGHTNING	3


/* Rent codes */
#define RENT_UNDEF      0
#define RENT_CRASH      1
#define RENT_RENTED     2
#define RENT_CRYO       3
#define RENT_FORCED     4
#define RENT_TIMEDOUT   5

/* emote types. used by ch->player_specials->lastemotetype */
#define EMOTE_EMOTE	0
#define EMOTE_POSE	1
#define EMOTE_PEMOTE	2
#define EMOTE_CEMOTE	3


/* other #defined constants **********************************************/

/*
 * **DO**NOT** blindly change the number of levels in your MUD merely by
 * changing these numbers and without changing the rest of the code to match.
 * Other changes throughout the code are required.
 */
#define LVL_IMPL		211
#define LVL_GRGOD		210
#define	LVL_HBUILDER	209
#define LVL_GROOVYBGL	208
#define LVL_GROOVYB		207
#define LVL_GROOVYGL	206
#define LVL_BUILDERGL   205
#define LVL_BUILDER		204
#define LVL_GOD			203
#define LVL_QUESTOR		202
#define LVL_CARPENTER	201
#define LVL_IMMORT		200

#define LVL_FREEZE	LVL_QUESTOR

#define NUM_OF_DIRS	10	/* number of directions in a room (nsewud+4) */

#define OPT_USEC	100000	/* 10 passes per second */
#define PASSES_PER_SEC	(1000000 / OPT_USEC)
#define RL_SEC		* PASSES_PER_SEC

#define PULSE_ZONE      (10 RL_SEC)
#define PULSE_MOBILE    (10 RL_SEC)
#define PULSE_VIOLENCE  (2 RL_SEC)
#define PULSE_TAINT     ((5 * 60 ) RL_SEC)
#define PULSE_NOINC     (90 RL_SEC)
#define PULSE_LONGING   (120 RL_SEC)    /* One LP every 30 sec, 3 back every *
                                         * 30 sec in a Stedding              */
#define PULSE_LMESS     ((10 * 60) RL_SEC) /* Longing mess, every 10 minutes */
#define PULSE_CPTABLE   (5 RL_SEC)     /* Check ptable if saving is needed every 5 secs */

#define SMALL_BUFSIZE		(2*1024)
#define LARGE_BUFSIZE		(20 * 1024)
#define GARBAGE_SPACE		32

#define MAX_STRING_LENGTH	16384
#define MAX_INPUT_LENGTH	8190	/* Max length per *line* of input */
#define MAX_RAW_INPUT_LENGTH	1024	/* Max size of *raw* input */
#define MAX_NAME_LENGTH		20   
#define MAX_PWD_LENGTH		10   
#define MAX_TITLE_LENGTH	80   
#define HOST_LENGTH		30   
#define EXDSCR_LENGTH		1024 
#define MAX_TONGUE		3    
#define MAX_SKILLS		400  
#define MAX_AFFECT		32   
#define MAX_OBJ_AFFECT		6   
#define MAX_COMPLETED_QUESTS	320
#define MAX_NUMBER_IN_LINK  	30
#define MAX_ROOMS_ON_MUD	40000L
#define MAX_NOTIFYLIST_LENGTH   10
/* #define MAX_IGNORELIST_LENGTH   20 Removed ignorelist */
#define MAX_TELLBUFFER_LENGTH   10

#ifndef MAX_TRIGGER
#define MAX_TRIGGER 8
#endif

/**********************************************************************
* Structures                                                          *
**********************************************************************/

typedef signed char		sbyte;
typedef unsigned char		ubyte;
typedef signed short int	sh_int;
typedef unsigned short int	ush_int;
typedef char			bool;

#ifndef CIRCLE_WINDOWS
typedef char			byte;
#endif

typedef sh_int room_num;
typedef sh_int obj_num; 

typedef struct tag_scene_data SCENE_DATA;
struct tag_scene_data
{
  char		    *text;
  struct room_data  *room;
  struct char_data  *owner;
};

/* Extra description: used in objects, mobiles, and rooms */
struct extra_descr_data {
   char	*keyword;                 /* Keyword in look/examine          */
   char	*description;             /* What to see                      */
   struct extra_descr_data *next; /* Next in list                     */
};

typedef struct extra_descr_data exdesc_t, *exdescptr_t;

struct player_extradesc {
  int descnum;                    /* Which desc does this xdesc go to? */
  char *keyword;                  /* Keyword for the desc */
  char *desc;                     /* Desc itself */
  struct player_extradesc *next;  /* next xdesc */
};

typedef struct player_extradesc pxdesc_t, *pexdescptr_t;

/* object-related structures ******************************************/

#define NUM_OBJ_VAL_POSITIONS 4

/* object flags; used in obj_data */
struct obj_flag_data {
   long	value[NUM_OBJ_VAL_POSITIONS];	/* Values of the item (see list)    */
   byte type_flag;	/* Type of item			    */
   long wear_flags;	/* Where you can wear it	    */
   long	extra_flags;	/* If it hums, glows, etc.	    */
   long	weight;		/* Weigt what else                  */
   long	cost;		/* Value when sold (gp.)            */
   long	questcost;	/* Cost in quest points             */
   long	timer;		/* Timer for object                 */
   long	bitvector;	/* To set chars bits                */
   char *boughtBy;      /* Set to the char who originally bought the QEQ */
   long  boughtFrom[2]; /* VNum of the shop(0)/room(1) where the QEQ was bought */
   time_t boughtWhen;   /* The time the item was bought. */
};

typedef struct obj_flag_data objflags_t, *objflagsptr_t;

struct obj_affected_type {
   byte location;      /* Which ability to change (APPLY_XXX) */
   long modifier;     /* How much it changes by              */
};

typedef struct obj_affected_type objaffect_t, *objaffectptr_t;

/* ================== Memory Structure for Objects ================== */
struct obj_data {
   obj_num item_number;		/* Where in data-base			*/
   room_num in_room;		/* In what room -1 when conta/carr	*/

   struct obj_flag_data obj_flags; /* Object information               */
   struct obj_affected_type affected[MAX_OBJ_AFFECT];  /* affects */

   char	*name;                    /* Title of object :get etc.        */
   char	*description;		  /* When in room                     */
   char	*short_description;       /* when worn/carry/in cont.         */
   char	*action_description;      /* What to write when used          */
   struct extra_descr_data *ex_description; /* extra descriptions     */
   struct char_data *pulled_by;   /* Wagon is pulled by?              */
   struct char_data *carried_by;  /* Carried by :NULL in room/conta   */
   struct char_data *worn_by;	  /* Worn by?			      */
   sh_int worn_on;		  /* Worn where?		      */
   long   mana_add;	          /* Manaadd if obj has APPLY_MANA    */
   int obj_status;                /* How new/worn down is the obj?    */
   struct obj_prog_list *oprog;   /* Object program                   */
   struct obj_action_list *paused_at; /* Program paused here          */
   int pause;                     /* The amount of time to pause there*/
   long op_activated;		  /* Bitvector for activated programs */
   struct obj_data *linked;       /* linked for gate-spell!           */
   struct obj_data *in_obj;       /* In what object NULL when none    */
   struct obj_data *on_obj;       /* On what furniture object ? Null if none */
   struct obj_data *contains;     /* Contains objects                 */

   struct obj_data *next_content; /* For 'contains' lists             */
   struct obj_data *next;         /* For the object list              */

   /* For DGS */
   long id;                       /* used by DG triggers              */
   struct trig_proto_list *proto_script; /* list of default triggers  */
   struct script_data *script;    /* script info for the object       */

   int status[10];
};

typedef struct obj_data obj_t, *objptr_t;

/* ======================================================================= */


/* room-related structures ************************************************/


struct room_direction_data {
   char	*general_description;       /* When look DIR.			*/

   char	*keyword;		/* for open/close			*/

   sh_int exit_info;		/* Exit info				*/
   obj_num key;			/* Key's number (-1 for no key)		*/
   room_num to_room;		/* Where direction leads (NOWHERE)	*/
};

typedef struct room_direction_data roomdir_t, *roomdirptr_t;

/* ================== Memory Structure for room ======================= */
struct room_data {
   room_num number;		/* Rooms number	(vnum)		      */
   int zone;                 /* Room zone (for resetting)          */
   long	sector_type;            /* sector type (move/hide)            */
   char	*name;                  /* Rooms name 'You are ...'           */
   char	*description;           /* Shown when entered                 */
   struct extra_descr_data *ex_description; /* for examine/look       */
   struct room_direction_data *dir_option[NUM_OF_DIRS]; /* Directions */
   long room_flags;		/* DEATH,DARK ... etc                 */
   struct obj_data *wagon;      /* Wagon obj room is connected to     */
   byte light;                  /* Number of lightsources in room     */
   SPECIAL(*func);

   struct obj_data *contents;   /* List of items in room              */
   struct char_data *people;    /* List of NPC / PC in room           */
   struct affected_type *aff;   /* Room is affected by                */
   struct eavesdrop_info *eaves; /* List of people Eavesdropping       */
   long aff_flags;               /* Affection-flags for room 	      */
   int status[10];
   struct char_data *skimmer;	/* If skimming room, this'll point at the weaver */
   int travel_to;		/* Skimming to room no? */
   int eta;			/* How many ticks (left) to travel? */
   char skstat;			/* Haven't started traveloing yet/is traveling/reached the target */
   long skim_ppos;		/* Shoudl skimmer have loged of,use ppos instead */
   long temp_flags;		/* same as room_flags, but not saved */

   /* For DGS */
   struct trig_proto_list *proto_script; /* list of default triggers  */
   struct script_data *script;  /* script info for the object         */
   SCENE_DATA	    *scene;	/*room doing info -- fi set*/
};

extern struct room_data *world;
typedef struct room_data room_t, *roomptr_t;

/* ====================================================================== */


/* char-related structures ************************************************/

/* memory structure for characters */
struct memory_rec_struct {
   long	id;
   struct memory_rec_struct *next;
};

typedef struct memory_rec_struct memory_rec;

/* This structure is purely intended to be an easy way to transfer */
/* and return information about time (real or mudwise).            */
struct time_info_data {
   long hours, day, month;
   sh_int year;
};

/* These data contain information about a players time data */
struct time_data {
   time_t birth;    /* This represents the characters age                */
   time_t logon;    /* Time of the last logon (used to calculate played) */
   long	played;     /* This is the total accumulated time played in secs */
};

#define MAX_DESCRIPTIONS 5

/* general player-related info, usually PC's and NPC's */
struct char_player_data {
  char	 passwd[MAX_PWD_LENGTH+1]; /* character's password     */
  char	*name;	       /* PC / NPC s name (kill ...  )         */
  char	*short_descr;  /* for NPC 'actions'                    */
  char	*long_descr;   /* for 'look'			       */

  int    current_desc;
  char  *descriptions[MAX_DESCRIPTIONS];
  struct player_extradesc *xdesc;

  char 	*description;  /* Extra descriptions(TO REMOVE)        */
  char	*title;        /* PC / NPC's title                     */
  char  *custom_prompt;

  char  *contactinfo;  /* Contact info of char's player        */
  char  *background;   /* BG of character                      */
  char  *pretitle;     /* Imm-set pretitle in who...           */
  char  *nobility;
  char  mycolor[3];

  
  byte sex;            /* PC / NPC's sex                       */
  byte class;          /* PC / NPC's class		       */
  long level;          /* PC / NPC's level                     */
  long	hometown;      /* PC s Hometown (zone)                 */
  struct time_data time;  /* PC's AGE in days                  */
  time_t last_logon;
  ubyte weight;        /* PC / NPC's weight                    */
  ubyte height;        /* PC / NPC's height                    */
};


/* Char's abilities.  */
struct char_ability_data {
   sbyte str;
   sbyte intel;
   sbyte wis;
   sbyte dex;
   sbyte con;
   sbyte cha;
};


/* Char's points.  */
struct char_point_data {
   sh_int mana;
   sh_int max_mana;     /* Max move for PC/NPC			   */
   sh_int hit;
   sh_int max_hit;      /* Max hit for PC/NPC                      */
   sh_int move;
   sh_int max_move;     /* Max move for PC/NPC                     */

   sh_int armor;        /* Internal -100..100, external -10..10 AC */
   long	gold;           /* Money carried                           */
   long	bank_gold;	/* Gold the char has in a bank account	   */
   long	exp;            /* The experience of the player            */

   long hitroll;       /* Any bonus or penalty to the hit roll    */
   long damroll;       /* Any bonus or penalty to the damage roll */
};


/*
 * char_special_data_saved: specials which both a PC and an NPC have in
 * common, but which must be saved to the playerfile for PC's.
 *
 * WARNING:  Do not change this structure.  Doing so will ruin the
 * playerfile.  If you want to add to the playerfile, use the spares
 * in player_special_data.
 */
struct char_special_data_saved {
   long	idnum;			/* player's idnum; -1 for mobiles	*/
   long	act;			/* mob flag for NPC's;                  */
   long plr;                    /* player flag for PC's                 */

   long	affected_by;		/* Bitvector for spells/skills affected by */
   long affected2;		/* Extra bits for affections */
   long affected3;
   long affected4;
   long unused;			/* I need this one for tie-weave, sorry! */
};


/* Special playing constants shared by PCs and NPCs which aren't in pfile */
struct char_special_data {
   struct char_data *fighting;	/* Opponent				 */
   struct char_data *hunting;	/* Char hunted by this char	 	 */
   byte talking;		/* The language the char speaks presently*/

   byte position;		/* Standing, fighting, sleeping, etc.	 */
   struct obj_data *sitting_on; /* What the char is sitting on           */

   long	carry_weight;		/* Carried weight			 */
   long carry_items;		/* Number of items carried		 */
   long	timer;			/* Timer for update			 */
   long goingto;		/* Better place it here, if we want mobs */
				/* to be able to go cart-riding!	 */

   struct char_guild_element *guilds_data; /* Guilds data */

   ubyte fract_move;		/* Online usage only, store fractional
				 * moves				 */
   struct char_special_data_saved saved; /* constants saved in plrfile	 */
};


/*
 *  If you want to add new values to the playerfile, do it here.  DO NOT
 * ADD, DELETE OR MOVE ANY OF THE VARIABLES - doing so will change the
 * size of the structure and ruin the playerfile.  However, you can change
 * the names of the spares to something more meaningful, and then use them
 * in your new code.  They will automatically be transferred from the
 * playerfile into memory when players log in.
 */
struct player_special_data_saved {
   byte skills[MAX_SKILLS+1];	/* array of skills plus skill 0		*/
   byte PADDING0;		/* used to be spells_to_learn		*/
   bool talks[MAX_TONGUE];	/* PC s Tongues 0 for NPC		*/
   long	wimp_level;		/* Below this # of hit points, flee!	*/
   byte freeze_level;		/* Level of god who froze char, if any	*/
   sh_int invis_level;		/* level of invisibility		*/
   room_num load_room;		/* Which room to place char in(moved)	*/
   long	pref;			/* preference flags for PC's.		*/
   ubyte bad_pws;		/* number of bad password attemps	*/
   sbyte conditions[3];         /* Drunk, full, thirsty			*/
   char servant[60];		/* Nobles servants title, includes name */

   /* spares below for future expansion.  You can change the names from
      'sparen' to something meaningful, order is irrelevant (with xml chars!) */
   ubyte air;              /* pos: 588 */
   ubyte earth;
   ubyte fire;
   ubyte spirit;
   ubyte water;
   ubyte blademastery;
   ubyte spark;
   ubyte teachable;
   ubyte taveren;       
//   ubyte true_power;
   ubyte wolfkin;
   ubyte trains;
   ubyte nobility;
   ubyte race;

   ubyte drf_flags;

   ubyte ajah;
   ubyte betting;
 
   ubyte playing; 
   ubyte recruit_wait;
   ubyte taught;
   ubyte nohide;
   ubyte lines;
   ubyte hptaught;

   long spells_to_learn;		/* How many can you learn yet this level*/
   long olc_zone;
   long bonded;
   long minion;
   long powerpoints;   // 828
   long questpoints;
   long absorbation;
   long permanent;
   long current_quest;
   long AutoQuest[10]; 		/* Room for 320 AutoQuests! */
   long AutoFlags;
   long last_emote_counter;     /* Used for OOC->IC mode, reset every time you speak */
   long subdue;                 /* This is the subdue flag */ 

   long branded;
   long wagon;
   long houses[20];
   long wolfkinT;
   long wkrank;
   long invstart;
   long ogier;
   long primtal1;
   long primtal2;
   long spectal1;
   long spectal2;
   long specskills1;
   long specskills2;
   long pref2;
   long syslog;
   long sess;

   char *replyto;

   long eavesroom;

   long mapped[10];
   int nummapped;
   
   long	compulsion;     /* Pos 1700 */
   unsigned long taint;
   long	nolocate;
   long	ginvis;
   long	ashabonded; // See about 20 lines above for minion and bonded
   long unauthorized; 
  /* Authorization code: Notice the default is AUTHORIZED.
     This is so that we won't have to authorize every single
     old time player from scratch; it's simply turned off for
     newly created chars. */
   long	talents;  //Store talent for a channeler
   long command_groups; // For the new Grant System code

};

/*
 * Specials needed only by PCs, not NPCs.  Space for this structure is
 * not allocated in memory for NPCs, but it is for PCs and the portion
 * of it labelled 'saved' is saved in the playerfile.  This structure can
 * be changed freely; beware, though, that changing the contents of
 * player_special_data_saved will corrupt the playerfile.
 */
struct player_special_data {
   struct player_special_data_saved saved;

   int room_idle;                /* How long the player has been in room */
   char	*poofin;		 /* Description on arrival of a god.     */
   char	*poofout;		 /* Description upon a god's exit.       */
   struct alias *aliases;	 /* Character's aliases			 */
   long last_tell;	 	 /* idnum of last tell from		 */
   void *last_olc_targ;		 /* olc control				 */
   long last_olc_mode;		 /* olc control				 */
   char *afk_String;		 /* String to display when AFK, set to   */
                                 /* also for AFW: can be AFK or AFW      */
			 	 /* default at starttime, can be changed */
   struct char_data *linked_by;  	 /* player the character is linked to */
   struct char_data *linking[MAX_NUMBER_IN_LINK-2];  /* players the character is linking */
   long   max_manalinked;       /* maximum mana in the pool */
   long	  manalinked;  		/* mana pool the caster can draw from */
   long   managiven;  	        /* % contribution by char into the mana pool */
   long   no_skill[MAX_CAN];	/* to keep track on for how many rounds you */
				/* can't kick bash etc....		     */
   long   sleeproom;           /* The room the dreamer fell asleep in */
   struct char_data *dreampulled; /* Who dreampulled this char ? */
   struct char_data *dreamentered; /* Who entered your dream ? */
   long   fool_level;           /* Used in DF skill Obscure             */
   long   speaking;		/* The language the player uses currently*/
   char  *real_name;		/* Savepos for realname when using disguise skill */
   char  *real_title;		/* ------||--- realtitle ------------||---------- */
   char  *real_desc;		/* ------||--- realdesc -------------||---------- */
   char *real_nobility;
   ubyte nobility;              /* save nobility before disguising */
   ubyte is_disguised;		/* Is player disguised?				  */

   struct char_data *sleepmob;  /* If player is Dreaming, the copy-mob 		  */

   struct obj_data *craft;	/* Pointer to the object char is tooling/crafting */
   long    craft_state;		/* Which state in crafting/tooling?		  */
   long    obj_type;		/* what type of object?	*/
   struct  extra_descr_data *tl;/* Extra desc ptr for tooling */
   long    xattacks;		/* How many xtra-attacks*/
   long    travellingto;      /* Tinker guild skill */
   struct char_data *dicer;     /* The player that this player is playing dice with */
   long    bet;		       /* If the bet has been set, how large is it? */
   struct mob_prog_list *mob_prog_list;  /* Present mob program player is mediting */
   struct mob_action_list *mob_act_list; /* Present action list player is mediting */
   struct side_term_list *mob_st_list;   /* Present sidetermlist player is mediting */
   struct obj_prog_list *opl;
   struct obj_action_list *oal;
   char *doing;			/* Replacement string for "is standing here" */
   char *arrive;		/* -----||--------------- "$n arrives" */
   char *leave;			/* --------||------------ "$n leaves..." */
   char *prefix;		/* prefix string...*/
   long noincrease[13];		/* Bitvector for spells that can't be increased for a while */
   byte timeincrease[MAX_SKILLS+1]; /* time left before skill can be increased */
   long wheel_bet;
   int  wheel_nr;
   struct mmail *recepients;	/* A list of recepients for MMail! */
   int ticks_in_room;           /* How long has the player been in the room? */
   unsigned long  not_in_stedding;      /* For Ogier Longing     */
   byte forced;			/* Player forced by program to act? */
   bv_tp	known_rooms;	/* Player knows these rooms */
//   struct mount_data *riding;   /* What mount the character is riding */
//   struct mount_data *owned_mounts[MAX_MOUNTS]; /* Player can own MAX_MOUNTS horses */
   struct card_game *game;      /* Card game the PC is playing atm */
   int private;			/* Private channel id */
   char *lastemotestr;		/* the most recent emote. */
   int lastemotetype;		/* pose/emote/etc */
   struct game_of_chess *chess; /* game of chess */

   int buffer_flag;                  /* A flag for the tell buffer */
   char tells[MAX_TELLBUFFER_LENGTH][MAX_INPUT_LENGTH]; /* Buffer for tells */
   int tells_index;                  /* Index for the tells buffer */
   char rplog[MAX_TELLBUFFER_LENGTH][MAX_INPUT_LENGTH]; /* Buffer for RPlog */
   int rplog_index;                  /* Index for the RPlog buffer */

   char notifylist[MAX_NOTIFYLIST_LENGTH][MAX_NAME_LENGTH+1]; /* Notification list */
/*   char ignorelist[MAX_IGNORELIST_LENGTH][MAX_NAME_LENGTH+1]; Removed Ignore list */
   struct retainer_data *retainer[10];
   SCENE_DATA	*scene;
};

#define CAN_BASH	0	/* pos for fidd. skills */
#define CAN_KICK	1	
#define CAN_DIRTKICK	2
#define CAN_PUNCH	3
#define CAN_TRIP        4
#define CAN_DISARM      5
#define CAN_STUN        6
#define CAN_STAB        7
#define CAN_SHOOT	8
#define CAN_WEAVE       9
#define CAN_BREAK      10      // Break_shield counter.
#define CAN_TEACH      11
#define CAN_POUND      12
#define CAN_FLEE       13  /* after backstab or other such skills */

/* AutoFlags! */
#define AUTO_EXITS 	(1 << 0)   /* Display exits 					*/
#define AUTO_GOLD	(1 << 1)   /* Get all gold after a kill 	  		*/
#define AUTO_LOOT	(1 << 2)   /* Get all eq after kill     			*/
#define AUTO_JUNK	(1 << 3)   /* Junk corpses after a kill				*/
#define AUTO_SPLIT 	(1 << 4)   /* Split all gold in group after kill		*/
#define AUTO_ASSIST	(1 << 5)   /* Assist if someone in your group is attacked	*/
#define AUTO_SLICE      (1 << 6)   /* Slice all offensive weaves directed against you 	*/

/* Specials used by NPCs, not PCs */
struct mob_special_data {
   byte last_direction;       /* The last direction the monster went    */
   long	attack_type;          /* The Attack Type Bitvector for NPC's    */
   byte default_pos;          /* Default position for NPC               */
   memory_rec *memory;	      /* List of attackers to remember	       	*/
   byte damnodice;            /* The number of damage dice's	       	*/
   byte damsizedice;          /* The size of the damage dice's          */

   sh_int hpnodice;           /* Number of HP dices */
   sh_int hpsizedice;         /* Size of HP dices   */
   sh_int hpbonus;            /* Bonus to HP        */

   sh_int mananodice;           /* Number of mana dices */
   sh_int manasizedice;         /* Size of mana dices   */
   sh_int manabonus;            /* Bonus to mana        */

   sh_int movenodice;           /* Number of move dices */
   sh_int movesizedice;         /* Size of move dices   */
   sh_int movebonus;            /* Bonus to move        */

   long wait_state;	      /* Wait state for bashed mobs	       	*/
   long timer;		      /* # of ticks until mob disappears	*/
   struct char_data *dreamer; /* If the mob is a copy of a dreamer, the pointer to that char */
   long skillvector;	      /* List of skills the mob knows in battle */
   long no_skill[MAX_CAN];    /* Timers for offensive skills		*/
   byte mode;		      /* Mode for programmed mobs		*/
   int pause;                 /* How long the mob program pauses (secs) */
   long prog_activated;	      /* Vector of programs that are activated  */
   struct mob_action_list *paused_at; /* Program pause pos              */
   int target_room;	      /* Target room rnum when mob's prog'd to go there */
   int *skills[2];            /* list of skills the mob can teach       */
   int teaches_num;           /* How many skills this mob teaches.      */
   int status[10];
//   struct mount_data *mount; /* Mount data for mounts */
   struct retainer_data *retainer;
};


/* An affect structure.  */
struct affected_type {
   sh_int type;          /* The type of spell that caused this      */
   sh_int duration;      /* For how long its effects will last      */
   sbyte tie_info;       /* This is added to apropriate ability     */
   byte location;        /* Tells which ability to change(APPLY_XXX)*/
   long	bitvector;       /* Tells which bits to set (AFF_XXX)       */
   long bitvector2;	 /* Extra bits for AFF_XXX 		    */
   long bitvector3;
   long bitvector4;
   long modifier;        /* For tie-weaves, unravel, maintain etc. */
   long weaver;		 /* For RAFF_XX, could be used with ordinary too */
   long speed;		 /* The speed to use for update of this RAFF_XX */
   long mana_add;        /* Mana_add for maintained weaves  */
   long spell;           /* For traps!                              */
   long sex;		 /* Sex of weaver, used for spells	    */
   long result;          /* result of spell(skill check roll thingy.*/
   struct affected_type *next;
};

typedef struct affected_type affect_t, *affectptr_t;

/* Structure used for chars following other chars */
struct follow_type {
   struct char_data *follower;
   struct follow_type *next;
};

struct board_stamp {
    int	    vnum;
    time_t  stamp;
    struct board_stamp	*next;
};

/* ================== Structure for player/non-player ===================== */
struct char_data {
   long pfilepos;			 /* playerfile pos		  */
   sh_int nr;                            /* Mob's rnum			  */
   room_num in_room;                     /* Location (real room number)	  */
   room_num was_in_room;		 /* location for linkdead people  */

   struct char_player_data player;       /* Normal data                   */
   struct char_ability_data real_abils;	 /* Abilities without modifiers   */
   struct char_ability_data aff_abils;	 /* Abils with spells/stones/etc  */
   struct char_point_data points;        /* Points                        */
   struct char_special_data char_specials;	/* PC/NPC specials	  */
   struct player_special_data *player_specials; /* PC specials		  */
   struct mob_special_data mob_specials;	/* NPC specials		  */

   struct affected_type *affected;       /* affected by what spells       */
   struct obj_data *equipment[NUM_WEARS];/* Equipment array               */

   struct obj_data *carrying;            /* Head of list                  */
   struct descriptor_data *desc;         /* NULL for mobiles              */

   struct board_stamp	*brd_stamp;	/* date stamps of last time board
					   was read			  */

   struct char_data *next_in_room;     /* For room->people - list         */
   struct char_data *next;             /* For either monster or ppl-list  */
   struct char_data *next_fighting;    /* For fighting list               */

   int current_quest;                    /* vnum of current quest         */
   struct follow_type *followers;        /* List of chars followers       */
   struct char_data *master;             /* Who is char following?        */
   struct follow_type *tailed;           /* Char is tailed by             */
   struct char_data *tailing;            /* Char is tailing               */
   struct obj_data *pulling;		 /* Wagon char is pulling around  */
   struct mob_prog_list *mprog;		 /* Mob program list, if existing */
   long mpactnum;
   char *tempHost;

   /* For DGS */
   long id;                            /* used by DG triggers             */
   struct trig_proto_list *proto_script; /* list of default triggers      */
   struct script_data *script;         /* script info for the object      */
   struct script_memory *memory;       /* for mob memory triggers         */
};
/* ====================================================================== */

typedef struct char_data character_t, *characterptr_t;

/* Here's the struct that we're adding to the room structure for eavesdrop */

struct eavesdrop_info {
        char *charname;			/* Name of the char eavesdropping    */
        int distance;			/* Num rooms away eavesdropping from */
        struct eavesdrop_info *next;	/* next record 			     */
};

/* descriptor-related structures ******************************************/


struct txt_block {
   char	*text;
   long aliased;
   struct txt_block *next;
   void *jalla;			/* At least worth a try. :) */
};


struct txt_q {
   struct txt_block *head;
   struct txt_block *tail;
   void *for_faan;
};


struct descriptor_data {
   socket_t descriptor;		/* file descriptor for socket		*/
   char	host[HOST_LENGTH+1];	/* hostname				*/
   int gde_num[2];
   GIOChannel *gdescriptor;
   byte	bad_pws;		/* number of bad pw attemps this login	*/
   byte idle_tics;		/* tics idle at password prompt		*/
   long  connected;		/* mode of 'connectedness'		*/
   long  wait;			/* wait for how many loops		*/
   long  desc_num;		/* unique num assigned to desc		*/
   time_t login_time;		/* when the person connected		*/
   char *showstr_head;		/* for keeping track of an internal str	*/
   char **showstr_vector;	/* for paging through texts		*/
   long  showstr_count;		/* number of pages to page through	*/
   long  showstr_page;		/* which page are we currently showing?	*/
   long  current_text_line;     /* which line is being modified / added */
   char **str;			/* for the modify-str system		*/
   char *backstr; 		/* added for handling abort buffers     */
   size_t max_str;		/*		-			*/
   long	mail_to[5];		/* name for mail system			*/
   long  prompt_mode;		/* control of prompt-printing		*/
   GString *inbuf;
   GString *raw_inbuf;
   GString *last_input;
   GString *output;
   GString *missed;

   int tries;
   struct txt_q input;		/* q of unprocessed input		*/
   struct char_data *character;	/* linked to char			*/
   struct char_data *original;	/* original char if switched		*/
   struct descriptor_data *snooping; /* Who is this char snooping	*/
   struct descriptor_data *snoop_by; /* And who is snooping this char	*/
   struct descriptor_data *next; /* link to next descriptor		*/
   struct olc_data *olc;	     /*. OLC info - defined in olc.h   .*/
   struct sedit *sedit; 
   struct msg_edit_data *msged;
};

typedef struct descriptor_data descriptor_t, *descriptorptr_t;

/* other misc structs */

struct weather_data {
   long	pressure;	/* How is the pressure ( Mb ) */
   long	change;	/* How fast and what way does it change. */
   long	sky;	/* How is the sky. */
   long	sunlight;	/* And how much sun. */
};


struct title_type {
   char	*title_m;
   char	*title_f;
   long	exp;
};


/* element in monster and object index-tables   */
struct index_data {
   long	virtual;    /* virtual number of this mob/obj           */
   long	number;     /* number of existing units of this mob/obj	*/
   SPECIAL(*func);
//   void *foo;       /* See if it's a specific size thing */

   /* For DGS */
   char *farg;         /* string argument for special function     */
   struct trig_data *proto;     /* for triggers... the trigger     */
};


/* used in the socials */
struct social_messg {
   int act_nr;
   char *command;               /* holds copy of activating command */
   char *sort_as;		/* holds a copy of a similar command or
				 * abbreviation to sort by for the parser */
   int hide;			/* ? */
   int min_victim_position;	/* Position of victim */
   int min_char_position;	/* Position of char */
   int min_level_char;          /* Minimum level of socialing char */

   /* No argument was supplied */
   char *char_no_arg;
   char *others_no_arg;

   /* An argument was there, and a victim was found */
   char *char_found;
   char *others_found;
   char *vict_found;

   /* An argument was there, as well as a body part, and a victim was found */
   char *char_body_found;
   char *others_body_found;
   char *vict_body_found;

   /* An argument was there, but no victim was found */
   char *not_found;

   /* The victim turned out to be the character */
   char *char_auto;
   char *others_auto;

   /* If the char cant be found search the char's inven and do these: */
   char *char_obj_found;
   char *others_obj_found;
};

/* linked list for mob/object prototype trigger lists (DGS) */
struct trig_proto_list {
   int vnum;                             /* vnum of the trigger   */
   struct trig_proto_list *next;         /* next trigger          */
};

/* Name and vnum and exit to where boats can connect */
struct port_def
{
  char *name;   /* Name of port "Navigate ..." */
  long vnum;	/* Vnum of port room           */
  long exit;     /* Exit number, N = 0, E = 1...*/
  long taken;    /* If a ship is in this port or is sailing to it, it's taken */
};

struct cart_data
{
  long max;		/* Min room vnum player has to be in to ride carts in this area */
  long min;
  long destinations[30]; /* Vnums of destinations the player can go to */
};

struct mmail_data
{
  long pos;
  struct mmail_data *next;
};

struct wheel_data
{
  char *name;
  int odds;
};

struct card_data
{
  char *color;
  char *name;
  struct char_data *held_by;
  int used;
};

struct deck_data
{
  struct card_data cards[4][13];
};

struct card_game
{
  struct char_data **players;
  struct deck_data deck;
  int max_players;
};

struct mirroring_data {
  long from;
  long to;
};
  

#include "objprog.h"
#include "mobprog.h"

#endif
