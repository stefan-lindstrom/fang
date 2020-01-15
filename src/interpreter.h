/******************************************************************************
 * $Id: interpreter.h,v 1.14 2003/08/14 03:15:03 fang Exp $
 *   
 * File: interpreter.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: interpreter.h,v $
 *   Revision 1.14  2003/08/14 03:15:03  fang
 *   Added nopray command.
 *
 *   Revision 1.13  2003/05/29 03:50:28  fang
 *   Can wimpy after bashing now.
 *
 *   Revision 1.12  2003/03/01 07:17:05  fang
 *   Fisch: Multiflag for authorized multies
 *
 *   Revision 1.11  2002/12/19 06:41:26  fang
 *   fisch: remove toggle oldscore
 *
 *   Revision 1.10  2002/10/16 16:54:40  fang
 *   *** empty log message ***
 *
 *   Revision 1.9  2002/10/03 05:55:09  fischer
 *   fischer: moved ideas to be used as an old ideas and added newideas file
 *
 *   Revision 1.8  2002/10/02 05:05:46  fischer
 *   fisch: added timestamp flag for Ealasaid
 *
 *   Revision 1.7  2002/09/15 02:15:26  fang
 *   Drath: Fixed guild code bug which would cause random crashes.  Uptime should last days now.
 *
 *   Revision 1.6  2002/09/12 22:21:33  fang
 *   fischer: removed the prove and journal commands
 *
 *   Revision 1.5  2002/09/08 07:02:35  fang
 *   Drath: Added AEDIT CG
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
 *   Revision 1.9  2002/06/29 18:33:05  fang
 *   Drath: In-MUD syslog searching.  Replaces websyslog search
 *
 *   Revision 1.8  2002/06/20 03:07:10  fang
 *   Maz: RPTAG
 *
 *   Revision 1.7  2002/06/10 14:24:25  fang
 *   Maz: TOGGLE PCHAN added so you can turn off being added to pchans.
 *
 *   Revision 1.6  2002/06/02 22:14:44  fang
 *   Maz: PEDIT
 *
 *   Revision 1.5  2002/05/12 23:50:46  fang
 *   Added in IGNORE system, and removed AutoHotboots until the cap can be fixed for good. -Maz
 *
 *   Revision 1.4  2002/03/13 13:38:12  fang
 *   Ashasense added
 *
 *   Revision 1.3  2002/03/07 10:30:18  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.20  2001/08/23 14:32:23  mud
 *   *** empty log message ***
 *
 *   Revision 1.19  2001/08/13 22:19:10  mud
 *   *** empty log message ***
 *
 *   Revision 1.18  2001/05/22 09:07:05  mud
 *   DG-Scripts for Fang.
 *
 *   Revision 1.17  2001/04/15 02:30:15  mud
 *   Added a bundle of fixes and notifylist.
 *
 *   Revision 1.16  2001/01/29 14:03:06  mud
 *   Added WebMaster pretitle
 *
 *   Revision 1.15  2000/11/18 17:13:55  mud
 *   added guildmail command
 *
 *   Revision 1.14  2000/11/08 20:34:25  mud
 *   Added the todo list for imms
 *
 *   Revision 1.13  2000/11/08 07:47:42  mud
 *   *** empty log message ***
 *
 *   Revision 1.12  2000/08/10 15:06:29  mud
 *   added hedit (help editor)
 *
 *   Revision 1.11  2000/08/05 16:54:04  mud
 *   changed lastemote to also do pose, pemote, and cemote.
 *
 *   Revision 1.10  2000/08/04 09:42:39  mud
 *   finished changes to do_toggle, rmoved all old toggle commands (noooc, afk, goic, etc)
 *
 *   Revision 1.9  2000/07/31 16:13:51  mud
 *   added lastemote
 *
 *   Revision 1.8  2000/07/29 09:37:49  mud
 *   added 'toggle blah' capabaility to do_toggle
 *
 *   Revision 1.7  2000/06/17 20:23:42  mud
 *   New OLC perm system fixes.
 *
 *   Revision 1.6  2000/05/26 14:07:02  mud
 *   Recall command, showideas, showbugs, showtypos.
 *
 *   Revision 1.5  2000/04/09 14:21:30  mud
 *   Coded private channels!
 *
 *   Revision 1.4  2000/04/07 15:04:07  mud
 *
 *
 *   The new Grant System
 *
 *   Revision 1.3  2000/03/09 04:55:06  mud
 *   Added song channel.
 *
 *   Revision 1.2  2000/02/21 14:00:17  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/


/* ************************************************************************
*   File: interpreter.h                                 Part of CircleMUD *
*  Usage: header file: public procs, macro defs, subcommand defines       *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */
#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include "structs.h"

#define ACMD(name)  \
   void (name)(struct char_data *ch, char *argument, int cmd, int subcmd)

#define CMD_NAME (complete_cmd_info[cmd].command)
#define CMD_IS(cmd_name) (!strncasecmp(cmd_name, complete_cmd_info[cmd].command,strlen(complete_cmd_info[cmd].command)))
#define IS_MOVE(cmdnum) (cmdnum >= 1 && cmdnum <= 6)

void	command_interpreter(struct char_data *ch, char *argument);
int	search_block(char *arg, const char **list, bool exact);
int	search_block_case(char *arg, const char **list, bool exact);
char	lower( char c );
char	*one_argument(char *argument, char *first_arg);
char	*one_word(char *argument, char *first_arg);
char	*any_one_arg(char *argument, char *first_arg);
char	*two_arguments(char *argument, char *first_arg, char *second_arg);
int	fill_word(char *argument);
void	half_chop(char *string, char *arg1, char *arg2);
void	nanny(struct descriptor_data *d, char *arg);
int	is_abbrev(const char *arg1,const char *arg2);
int	is_number(char *str);
int	find_command(char *command);
void	skip_spaces(char **string);
char	*delete_doubledollar(char *string);

/* for compatibility with 2.20: */
#define argument_interpreter(a, b, c) two_arguments(a, b, c)


struct command_info {
   char *command;
   char *sort_as;
   long minimum_position;
   void	(*command_pointer)
   (struct char_data *ch, char * argument, int cmd, int subcmd);
   long minimum_level;
   long	subcmd;
   long command_groups;
};

extern struct command_info *complete_cmd_info;
extern struct command_info cmd_info[];

struct alias {
  char *alias;
  char *replacement;
  long type;
  struct alias *next;
};

#define ALIAS_SIMPLE	0
#define ALIAS_COMPLEX	1

#define ALIAS_SEP_CHAR	';'
#define ALIAS_VAR_CHAR	'$'
#define ALIAS_GLOB_CHAR	'*'

/*
 * SUBCOMMANDS
 *   You can define these however you want to, and the definitions of the
 *   subcommands are independent from function to function.
 */

/* Bond/Minion channels. */
#define SCMD_BDT	1
#define SCMD_MDT	2
#define SCMD_AMT	3

#define SCMD_ASHASENSE  1
#define SCMD_BONDSENSE  2

/* Buffered tells. */
#define SCMD_BREPLY     1       /* Reply to a buffered tell                    */
#define SCMD_BSEND      2       /* Send a tell to a char's buffer              */
#define SCMD_BDISPLAY   3       /* Display all tells currently in chars buffer */
#define SCMD_BSHOW      4       /* Display specific tell in chars buffer       */
#define SCMD_BDELETE    5       /* Deletes a specific tell in chars buffer     */

/* directions */
#define SCMD_NORTH	1
#define SCMD_EAST	2
#define SCMD_SOUTH	3
#define SCMD_WEST	4
#define SCMD_UP		5
#define SCMD_DOWN	6
#define SCMD_NORTHEAST  7
#define SCMD_SOUTHEAST  8
#define SCMD_SOUTHWEST  9
#define SCMD_NORTHWEST 10

/* do_gen_ps */
#define SCMD_INFO       0
#define SCMD_HANDBOOK   1 
#define SCMD_CREDITS    2
#define SCMD_NEWS       3
#define SCMD_WIZLIST    4
#define SCMD_VERSION    5
#define SCMD_IMMLIST    6
#define SCMD_MOTD	7
#define SCMD_IMOTD	8
#define SCMD_CLEAR	9
#define SCMD_WHOAMI	10
#define SCMD_NEWIDEAS	11
#define SCMD_BUGS       12
#define SCMD_IDEAS      13
#define SCMD_TYPOS      14
#define SCMD_TODOS	15
#define SCMD_OLDNEWS    16
#define SCMD_SOCIALIDEAS 17
#define SCMD_CRETODOS   18

/* do_gen_tog */
#define SCMD_NOSPAM     0
#define SCMD_NOHASSLE   1
#define SCMD_BRIEF      2
#define SCMD_COMPACT    3
#define SCMD_NOTELL	4
#define SCMD_NOAUCTION	5
#define SCMD_DEAF	6
#define SCMD_NOQUESTION	7
#define SCMD_NOOOC	8
#define SCMD_NOWIZ	9
#define SCMD_QUEST	10
#define SCMD_ROOMFLAGS	11
#define SCMD_NOREPEAT	12
#define SCMD_HOLYLIGHT	13
#define SCMD_SLOWNS	14
#define SCMD_AUTOEXIT	15
#define SCMD_NOGDT      16
#define SCMD_AUTOGOLD   17
#define SCMD_AUTOLOOT   18
#define SCMD_AUTOJUNK   19
#define SCMD_AUTOASSIST 20
#define SCMD_AUTOSPLIT  21
#define SCMD_NOWT	22
#define SCMD_LOOKAHEAD  23
#define SCMD_COMMSPAM	24
#define SCMD_PWIMPY	25
#define SCMD_SLICEWEAVE 26
#define SCMD_NOHUNGER	27
#define SCMD_NOKILLMESS 28
#define SCMD_NODT       29
#define SCMD_NOOT       30
#define SCMD_NOOBSCENE	31
#define SCMD_NOPOSEID	32
#define SCMD_NOSONG	33
#define SCMD_IC		34
#define SCMD_NOWEATHER  35
#define SCMD_MULTI      36
#define SCMD_NOPADD	37
#define SCMD_RPTAG      38
#define SCMD_XLOGTAG    39
#define SCMD_TIMESTAMP	40
#define NUM_TOG_SCMDS	41

#define SCMD_UNKNOT  0
#define SCMD_UNRAVEL 1
#define SCMD_TIE     2

/* do_wizutil */
#define SCMD_REROLL	0
#define SCMD_PARDON     1
#define SCMD_NOTITLE    2
#define SCMD_SQUELCH    3
#define SCMD_FREEZE	4
#define SCMD_THAW	5
#define SCMD_UNAFFECT	6
#define SCMD_NOGECHO    7
#define SCMD_NOPRAY	8

/* do_spec_com */
#define SCMD_WHISPER	0
#define SCMD_ASK	1

/* do_gen_com */
#define SCMD_HOLLER	0
#define SCMD_SHOUT	1
#define SCMD_QUESTION 	2
#define SCMD_AUCTION	3
#define SCMD_OOC	4
#define SCMD_OBSCENE	5
#define SCMD_SONG	6
#define SCMD_PRIVATE	7

/* do_private_channel */
#define SCMD_P_HELP    0
#define SCMD_P_OPEN    1
#define SCMD_P_CLOSE   2
#define SCMD_P_OFF     3
#define SCMD_P_ADD     4
#define SCMD_P_REMOVE  5
#define SCMD_P_WHO     6
#define SCMD_P_CHECK   7

/* do_shutdown */
#define SCMD_SHUTDOW	0
#define SCMD_SHUTDOWN   1

/* do_quit */
#define SCMD_QUI	0
#define SCMD_QUIT	1
#define SCMD_DELETE     2 /* goes in as int cmd from do_delete */ 

/* do_date */
#define SCMD_DATE	0
#define SCMD_UPTIME	1

/* weave */
#define SCMD_MAINTAINED 0
#define SCMD_TIED	1

/* do_commands */
#define SCMD_COMMANDS	0
#define SCMD_SOCIALS	1
#define SCMD_WIZHELP	2

/* do_drop */
#define SCMD_DROP	0
#define SCMD_JUNK	1
#define SCMD_DONATE	2

/* do_follow */
#define SCMD_TAIL 	1

/* do_gen_write */
#define SCMD_BUG	0
#define SCMD_TYPO	1
#define SCMD_IDEA	2
#define SCMD_TODO	3
#define SCMD_SOCIALIDEA	4
#define SCMD_CRETODO    5

/* do_look */
#define SCMD_LOOK	0
#define SCMD_READ	1

/* do_qcomm */
#define SCMD_QSAY	0
#define SCMD_QECHO	1

/* do_pour */
#define SCMD_POUR	0
#define SCMD_FILL	1

/* do_poof */
#define SCMD_POOFIN	0
#define SCMD_POOFOUT	1

/* do_hit */
#define SCMD_HIT	0
#define SCMD_MURDER	1

/* do_eat */
#define SCMD_EAT	0
#define SCMD_TASTE	1
#define SCMD_DRINK	2
#define SCMD_SIP	3

/* do_use */
#define SCMD_USE	0
#define SCMD_QUAFF	1
#define SCMD_RECITE	2

/* do_echo */
#define SCMD_ECHO	0
#define SCMD_EMOTE	1
#define SCMD_FX		2
#define SCMD_SFX	3

/* do_gen_door */
#define SCMD_OPEN       0
#define SCMD_CLOSE      1
#define SCMD_UNLOCK     2
#define SCMD_LOCK       3
#define SCMD_PICK       4

/* do_logsearch */
#define SCMD_LOGSEARCH	0
#define SCMD_VIEWLOG	1

/*. do_olc .*/
#define SCMD_OLC_REDIT  0
#define SCMD_OLC_OEDIT  1
#define SCMD_OLC_ZEDIT  2
#define SCMD_OLC_MEDIT  3
#define SCMD_OLC_SEDIT  4
#define SCMD_OLC_QEDIT  5
#define SCMD_OLC_HORSEEDIT 6
#define SCMD_OLC_HEDIT	7
#define SCMD_OLC_AEDIT  8
#define SCMD_OLC_SAVEINFO  9
#define SCMD_OLC_TRIGEDIT  10
#define SCMD_OLC_PEDIT	11

/* do_disguise .*/
#define SCMD_MINOR 1

/* mount commands */
#define SCMD_PET        1
#define SCMD_FEED       2
#define SCMD_WATER      3
#define SCMD_MOUNT      4
#define SCMD_DISMOUNT   5
#define SCMD_TRAIN      6
#define SCMD_TAKE       7
#define SCMD_NAME       8
#define SCMD_STABLE     9
#define SCMD_UNSTABLE  10
#define SCMD_TETHER    11
#define SCMD_UNTETHER  12

/* do_mail */
#define SCMD_MAIL_GUILD	1

/* do_flee */
#define SCMD_FLEE     1
#define SCMD_WIMPY    2

/* Command groups for the grant system */

#define CG_NEWBIE         (1 << 0)
#define CG_GL             (1 << 1)
#define CG_ST             (1 << 2)
#define CG_GL_BUILDER     (1 << 3)
#define CG_BUILDER        (1 << 4)
#define CG_ADMIN          (1 << 5)
#define CG_SM             (1 << 6)
#define CG_HGL            (1 << 7)
#define CG_HB             (1 << 8)
#define CG_OWNER          (1 << 9)
#define CG_WEAVER         (1 << 10)
#define CG_HCODER         (1 << 11)
#define CG_HCOUNCIL       (1 << 12)
#define CG_NEWBIEPR       (1 << 13)
#define CG_WEBMASTER      (1 << 14)
#define CG_DGS            (1 << 15)
#define CG_AEDIT          (1 << 16)

/*
 * Funcs
 */
extern int _parse_name(char *arg, char *name);
extern int reserved_word(char *argument);
extern char *one_arg_case_sen(char *argument, char *first_arg);
extern void free_alias(struct alias *a);
extern int special(struct char_data *ch, int cmd, char *arg);
extern void do_set_race(struct descriptor_data *d,char *arg);
extern void do_set_class(struct descriptor_data *d,char *arg);
extern void do_set_sex(struct descriptor_data *d,char *arg);
extern void do_set_ability(struct descriptor_data *d,char *arg2,int abl);
extern void add_bonuses(struct descriptor_data *d);
extern void do_welcome(struct descriptor_data *d);
extern void showChar(struct descriptor_data *d);
extern int find_name(char *);
extern int find_name_level(char *);

/*
 * Data
 */

#endif
