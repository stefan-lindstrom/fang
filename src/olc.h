/******************************************************************************
 * $Id: olc.h,v 1.8 2004/12/04 09:10:01 fang Exp $
 *   
 * File: olc.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: olc.h,v $
 *   Revision 1.8  2004/12/04 09:10:01  fang
 *   added eggnog -tiek
 *
 *   Revision 1.7  2003/08/03 03:57:46  fang
 *   Added soapy water as a liquid type in response to a builder request.
 *
 *   Revision 1.6  2003/06/10 12:16:08  fang
 *
 *   Added UNIQUE flag for mobs and objs. A unique obj/mob can only exists one
 *   instance at the time on the MUD, no matter what. A unique obj will not
 *   save to characters obj file either.
 *
 *   Revision 1.5  2003/05/12 10:09:09  fang
 *   increased exdescr lenght for rooms
 *
 *   Revision 1.4  2003/05/12 09:08:33  fang
 *   Enabled dynvars for room descs/xdescs
 *
 *   Revision 1.3  2002/09/08 06:41:16  fang
 *   Drath: Fischer added AEDIT.  Welcome to the staff, Fischer.
 *
 *   Revision 1.2  2002/07/26 19:39:52  fang
 *   Dath: MYCOLOR mem leak fixed.  Thanks, Fred!
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.4  2002/06/02 22:14:44  fang
 *   Maz: PEDIT
 *
 *   Revision 1.3  2002/03/12 15:02:15  fang
 *   Minor fixes to interpreter.c. dig is updated to utilize the new build permission thingies.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.19  2002/01/28 07:58:39  mud
 *   New wear slots, 2 ears, 1 nose...
 *
 *   Revision 1.18  2002/01/24 21:33:17  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.17.2.1  2002/01/22 20:50:59  mud
 *
 *
 *   Removed boats and info system. Obsolete, unused code with no value.
 *   Fixed bug in portal_stone SPECIAL. Now checks that a room exists
 *   before attempting to transf chars there. :)
 *
 *   Revision 1.17  2001/05/22 09:07:06  mud
 *   DG-Scripts for Fang.
 *
 *   Revision 1.16  2001/01/31 15:44:54  mud
 *   *** empty log message ***
 *
 *   Revision 1.15  2001/01/23 12:09:45  mud
 *   *** empty log message ***
 *
 *   Revision 1.14  2000/12/14 19:55:55  mud
 *   *** empty log message ***
 *
 *   Revision 1.13  2000/09/03 01:16:26  mud
 *   *** empty log message ***
 *
 *   Revision 1.12  2000/08/20 07:59:53  mud
 *   added a help_index. ordered linkedlist of help keywords and their pos in the help_table
 *
 *   Revision 1.11  2000/08/10 15:06:29  mud
 *   added hedit (help editor)
 *
 *   Revision 1.10  2000/08/01 11:51:00  mud
 *   added roomset command. sets roomflags that won't save
 *
 *   Revision 1.9  2000/06/09 18:40:53  mud
 *   Purdans 'Editor Name List' for zoned added.
 *
 *   Revision 1.8  2000/05/13 13:57:57  mud
 *   No name flag for boards, where players names won't show up on boards
 *   with the ITEM_NONAMEBOARD flag.
 *
 *   Revision 1.7  2000/04/20 18:28:34  mud
 *   Wrote 'tedit'.
 *
 *   Revision 1.6  2000/03/25 16:31:00  mud
 *   Added rlink to F1.
 *
 *   Revision 1.5  2000/03/01 19:18:10  mud
 *   Support for 'percentage loading' added to zedit.
 *
 *   Revision 1.4  2000/03/01 15:15:45  mud
 *   Named specials added, saved in room XML, and setable in redit
 *
 *   Revision 1.3  2000/03/01 10:44:06  mud
 *   Function for checking multiple edits in OLC added
 *
 *   Revision 1.2  2000/02/21 14:00:18  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*  _TwyliteMud_ by Rv.                          Based on CircleMud3.0bpl9 *
*    				                                          *
*  OasisOLC - olc.h 		                                          *
*    				                                          *
*  Copyright 1996 Harvey Gilpin.                                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*. Macros, defines, structs and globals for the OLC suite .*/

/*. CONFIG DEFINES - these should be in structs.h, but here is easyer.*/
#include "db.h"

#ifndef __OLC_H__
#define __OLC_H__

struct shop_buy_data;

#include "screen.h" /* For CCXXX defines */
#include "interpreter.h" /* For ACMD definition */
#include "structs.h"

#define NUM_ROOM_SECTORS	10

#define NUM_MOB_FLAGS		25
#define NUM_MOB_SKILLS		 6
#define NUM_AFF_FLAGS		30
#define NUM_ATTACK_TYPES	15

#define NUM_ITEM_TYPES		28
#define NUM_ITEM_FLAGS		21
#define NUM_ITEM_WEARS 		18
#define NUM_APPLIES		25
#define NUM_LIQ_TYPES 		21
#define NUM_POSITIONS		15
#define NUM_SPELLS		51

#define NUM_GENDERS		3
#define NUM_SHOP_FLAGS 		2
#define NUM_TRADERS 		7

#define NUM_FURNITURE_TYPES     4
#define NUM_FURNITURE_USAGE_TYPES 6

/*. Utils exported from olc.c .*/
void strip_string(char *);
void cleanup_olc(struct descriptor_data *d, byte cleanup_type);
void get_char_cols(struct char_data *ch);
void olc_add_to_save_list(int zone, char type);
void olc_remove_from_save_list(int zone, char type);
extern void olc_saveinfo(struct char_data *ch);
extern int check_lock(int number,int subcmd);
extern int builder_named(struct char_data *ch,int number);
extern int allowed_to_edit_room(struct char_data *ch,int vroom);

/*. OLC structs .*/

struct olc_data {
  long mode;
  long zone_num;
  long number;
  long value;
  char *storage;
  struct char_data *mob;
  struct room_data *room;
  struct obj_data *obj;
  struct zone_data *zone;
  struct shop_data *shop;
  struct social_messg *action;
  struct extra_descr_data *desc;
  struct aq_data *quest;
  struct mount_list_data *md;
  struct help_index_element *help; 
  struct poli_index_element *poli;

  /* For DGS */
  struct trig_data *trig;
  int script_mode;
  int trigger_position;
  int item_type;
  struct trig_proto_list *script;
};

struct olc_save_info {
  long zone;
  char type;
  struct olc_save_info *next;
};


/*. Exported globals .*/
#define nrm CCNRM(d->character, C_NRM)
#define grn CCGRN(d->character, C_NRM)
#define cyn CCCYN(d->character, C_NRM)
#define yel CCYEL(d->character, C_NRM)

#ifdef _RV_OLC_
/* NO NO NO NO NO (Elkins) */
/*
char *nrm=NULL, *grn=NULL, *cyn=NULL, *yel=NULL;
*/
struct olc_save_info *olc_save_list = NULL;
#else
/* See above */
/*
extern char *nrm, *grn, *cyn, *yel;
*/
extern struct olc_save_info *olc_save_list;
#endif


/*. Descriptor access macros .*/
#define OLC_MODE(d) 	((d)->olc->mode) 	/*. Parse input mode	.*/
#define OLC_NUM(d) 	((d)->olc->number)	/*. Room/Obj VNUM 	.*/
#define OLC_VAL(d) 	((d)->olc->value)  	/*. Scratch variable	.*/
#define OLC_ZNUM(d) 	((d)->olc->zone_num) 	/*. Real zone number	.*/
#define OLC_STORAGE(d)	((d)->olc->storage)	/*. Command Storage	.*/
#define OLC_ROOM(d) 	((d)->olc->room)	/*. Room structure	.*/
#define OLC_OBJ(d) 	((d)->olc->obj)	  	/*. Object structure	.*/
#define OLC_ZONE(d)     ((d)->olc->zone)	/*. Zone structure	.*/
#define OLC_MOB(d)	((d)->olc->mob)	  	/*. Mob structure	.*/
#define OLC_SHOP(d) 	((d)->olc->shop)	/*. Shop structure	.*/
#define OLC_ACTION(d)	((d)->olc->action)	/*. Action structure	.*/
#define OLC_DESC(d) 	((d)->olc->desc)	/*. Extra description	.*/
#define OLC_QUEST(d)    ((d)->olc->quest)       /* Quest structure       */
#define OLC_MOUNT(d)    ((d)->olc->md)          /* mounts               */
#define OLC_STORAGE(d)	((d)->olc->storage)	/* for saving files and stuff */
#define OLC_HELP(d)	((d)->olc->help)	/* help entries		*/
#define OLC_TRIG(d)     ((d)->olc->trig)        /* Trigger structure.   */
#define OLC_POLI(d)	((d)->olc->poli)	/* policy entries	*/

/*. Other macros .*/

#define OLC_EXIT(d)	(OLC_ROOM(d)->dir_option[OLC_VAL(d)])
#define GET_OLC_ZONE(c)	((c)->player_specials->saved.olc_zone)

/*. Cleanup types .*/
#define CLEANUP_ALL			1	/*. Free the whole lot  .*/
#define CLEANUP_STRUCTS 		2	/*. Don't free strings  .*/

/*. Add/Remove save list types	.*/
#define OLC_SAVE_ROOM			0
#define OLC_SAVE_OBJ			1
#define OLC_SAVE_ZONE			2
#define OLC_SAVE_MOB			3
#define OLC_SAVE_SHOP			4
#define OLC_SAVE_QUEST        		5
#define OLC_SAVE_HORSE                  6
#define OLC_SAVE_HELP			7
#define OLC_SAVE_POLI			8
#define OLC_SAVE_ACTION			9

 /*. Quest editor .*/
#define QEDIT_MAIN_MENU                 0
#define QEDIT_CONFIRM_SAVESTRING        1
#define QEDIT_NAME                      2
#define QEDIT_DESC                      3
#define QEDIT_INFO                      4
#define QEDIT_ENDING                    5
#define QEDIT_QUESTMASTER               6
#define QEDIT_TYPE                      7
#define QEDIT_FLAGS                     8
#define QEDIT_TARGET                    9
#define QEDIT_EXP                      10
#define QEDIT_NEXT                     11
#define QEDIT_VALUE_0                  12
#define QEDIT_VALUE_1                  13
#define QEDIT_VALUE_2                  14
#define QEDIT_VALUE_3                  15

/* Submodes of OEDIT connectedness */
#define OEDIT_MAIN_MENU              	1
#define OEDIT_EDIT_NAMELIST          	2
#define OEDIT_SHORTDESC              	3
#define OEDIT_LONGDESC               	4
#define OEDIT_ACTDESC                	5
#define OEDIT_TYPE                   	6
#define OEDIT_EXTRAS                 	7
#define OEDIT_WEAR                  	8
#define OEDIT_WEIGHT                	9
#define OEDIT_COST                  	10
#define OEDIT_COSTPERDAY            	11
#define OEDIT_TIMER                 	12
#define OEDIT_VALUE_1               	13
#define OEDIT_VALUE_2               	14
#define OEDIT_VALUE_3               	15
#define OEDIT_VALUE_4               	16
#define OEDIT_APPLY                 	17
#define OEDIT_APPLYMOD              	18
#define OEDIT_EXTRADESC_KEY         	19
#define OEDIT_CONFIRM_SAVEDB        	20
#define OEDIT_CONFIRM_SAVESTRING    	21
#define OEDIT_PROMPT_APPLY          	22
#define OEDIT_EXTRADESC_DESCRIPTION 	23
#define OEDIT_EXTRADESC_MENU        	24
#define OEDIT_LEVEL                 	25
#define OEDIT_OP_SIDE				26
#define OEDIT_OP_MAIN				27
#define OEDIT_OP_ACTIONS			28
#define OEDIT_OP_MAINTERMS			29
#define OEDIT_OP_SIDETERMS			30
#define OEDIT_OP_ADDACTION1			31
#define OEDIT_OP_ADDACTION2			32
#define OEDIT_OP_ADDSIDE1			33
#define OEDIT_OP_ADDSIDE2			34
#define OEDIT_OP_MAINTERMS2			35
#define OEDIT_OP_ADDNOT				36
#define OEDIT_OBJPROG				37


/* Submodes of REDIT connectedness */
#define REDIT_MAIN_MENU 		1
#define REDIT_NAME 			2
#define REDIT_DESC 			3
#define REDIT_FLAGS 			4
#define REDIT_SECTOR 			5
#define REDIT_EXIT_MENU 		6
#define REDIT_CONFIRM_SAVEDB 		7
#define REDIT_CONFIRM_SAVESTRING 	8
#define REDIT_EXIT_NUMBER 		9
#define REDIT_EXIT_DESCRIPTION 		10
#define REDIT_EXIT_KEYWORD 		11
#define REDIT_EXIT_KEY 			12
#define REDIT_EXIT_DOORFLAGS 		13
#define REDIT_EXTRADESC_MENU 		14
#define REDIT_EXTRADESC_KEY 		15
#define REDIT_EXTRADESC_DESCRIPTION	16
#define REDIT_ROOMPROG			17
#define REDIT_RP_SIDE			18
#define REDIT_RP_MAIN			19
#define REDIT_RP_ACTIONS		20
#define REDIT_RP_MAINTERMS		21
#define REDIT_RP_SIDETERMS		22
#define REDIT_RP_ADDACTION1		23
#define REDIT_RP_ADDACTION2		24
#define REDIT_RP_ADDSIDE1		25
#define REDIT_RP_ADDSIDE2		26
#define REDIT_RP_MAINTERMS2		27
#define REDIT_RP_ADDNOT			28
#define REDIT_NAMED_SPEC		29
#define REDIT_PICK_SPEC                 30

/*. Submodes of ZEDIT connectedness 	.*/
#define ZEDIT_MAIN_MENU            	0
#define ZEDIT_DELETE_ENTRY		1
#define ZEDIT_NEW_ENTRY			2
#define ZEDIT_CHANGE_ENTRY		3
#define ZEDIT_COMMAND_TYPE		4
#define ZEDIT_IF_FLAG			5
#define ZEDIT_ARG1			6
#define ZEDIT_ARG2			7
#define ZEDIT_ARG3			8
#define ZEDIT_ZONE_NAME			9
#define ZEDIT_ZONE_LIFE			10
#define ZEDIT_ZONE_TOP			11
#define ZEDIT_ZONE_RESET		12
#define ZEDIT_CONFIRM_SAVESTRING	13
#define ZEDIT_PERC                      14
#define ZEDIT_ENTER_BUILD               15
#define ZEDIT_ADD_BUILDER               16
#define ZEDIT_DELETE_BUILDER            17
#define ZEDIT_SARG1                     18
#define ZEDIT_SARG2                     19

/*. Submodes of MEDIT connectedness 	.*/
#define MEDIT_MAIN_MENU              	0
#define MEDIT_ALIAS			1
#define MEDIT_S_DESC			2
#define MEDIT_L_DESC			3
#define MEDIT_D_DESC			4
#define MEDIT_NPC_FLAGS			5
#define MEDIT_AFF_FLAGS			6
#define MEDIT_MOBPROG			7
#define MEDIT_CONFIRM_SAVESTRING	8
/*. Numerical responses .*/
#define MEDIT_NUMERICAL_RESPONSE	10
#define MEDIT_SEX			11
#define MEDIT_HITROLL			12
#define MEDIT_DAMROLL			13
#define MEDIT_NDD			14
#define MEDIT_SDD			15
#define MEDIT_NUM_HP_DICE		16
#define MEDIT_SIZE_HP_DICE		17
#define MEDIT_ADD_HP			18
#define MEDIT_AC			19
#define MEDIT_EXP			20
#define MEDIT_GOLD			21
#define MEDIT_POS			22
#define MEDIT_DEFAULT_POS		23
#define MEDIT_ATTACK			24
#define MEDIT_LEVEL			25
#define MEDIT_ALIGNMENT			26
#define MEDIT_SKILLS			27
#define MEDIT_MP_SIDE			28
#define MEDIT_MP_MAIN			29
#define MEDIT_MP_ACTIONS		30
#define MEDIT_MP_MAINTERMS		31
#define MEDIT_MP_SIDETERMS		32
#define MEDIT_MP_ADDACTION1		33
#define MEDIT_MP_ADDACTION2		34
#define MEDIT_MP_ADDSIDE1		35
#define MEDIT_MP_ADDSIDE2		36
#define MEDIT_MP_MAINTERMS2		37
#define MEDIT_MP_ADDNOT			38
#define MEDIT_TIMER			39
#define MEDIT_NUM_MANA_DICE             40
#define MEDIT_SIZE_MANA_DICE            41
#define MEDIT_ADD_MANA                  42 
#define MEDIT_NUM_MOVE_DICE             43
#define MEDIT_SIZE_MOVE_DICE            44
#define MEDIT_ADD_MOVE                  45 


/*. Submodes of SEDIT connectedness 	.*/
#define SEDIT_MAIN_MENU              	0
#define SEDIT_CONFIRM_SAVESTRING	1
#define SEDIT_NOITEM1			2
#define SEDIT_NOITEM2			3
#define SEDIT_NOCASH1			4
#define SEDIT_NOCASH2			5
#define SEDIT_NOBUY			6
#define SEDIT_BUY			7
#define SEDIT_SELL			8
#define SEDIT_PRODUCTS_MENU		11
#define SEDIT_ROOMS_MENU		12
#define SEDIT_NAMELIST_MENU		13
#define SEDIT_NAMELIST			14
/*. Numerical responses .*/
#define SEDIT_NUMERICAL_RESPONSE	20
#define SEDIT_OPEN1			21
#define SEDIT_OPEN2			22
#define SEDIT_CLOSE1			23
#define SEDIT_CLOSE2			24
#define SEDIT_KEEPER			25
#define SEDIT_BUY_PROFIT		26
#define SEDIT_SELL_PROFIT		27
#define SEDIT_TYPE_MENU			29
#define SEDIT_DELETE_TYPE		30
#define SEDIT_DELETE_PRODUCT		31
#define SEDIT_NEW_PRODUCT		32
#define SEDIT_DELETE_ROOM		33
#define SEDIT_NEW_ROOM			34
#define SEDIT_SHOP_FLAGS		35
#define SEDIT_NOTRADE			36


/*
 * Horse editing
 */
#define HORSEEDIT_MAIN_MENU             1
#define HORSEEDIT_VNUM                  2 
#define HORSEEDIT_MAX_CARRY             3
#define HORSEEDIT_MAX_CARRY_WEIGHT      4
#define HORSEEDIT_RIDABLE               5
#define HORSEEDIT_MAX_PASSENGERS        6
#define HORSEEDIT_FIGHT_BEHAVE          7
#define HORSEEDIT_LEAVE_BEHAVE          8
#define HORSEEDIT_TYPE                  9 
#define HORSEEDIT_SAVE_CONFIRM          10

/* Submodes of AEDIT connectedness	*/
#define AEDIT_PERMISSION		999
#define AEDIT_CONFIRM_SAVESTRING	0
#define AEDIT_CONFIRM_EDIT		1
#define AEDIT_CONFIRM_ADD		2
#define AEDIT_MAIN_MENU			3
#define AEDIT_ACTION_NAME		4
#define AEDIT_SORT_AS			5
#define AEDIT_MIN_CHAR_POS		6
#define AEDIT_MIN_VICT_POS		7
#define AEDIT_HIDDEN_FLAG		8
#define AEDIT_MIN_CHAR_LEVEL		9
#define AEDIT_NOVICT_CHAR		10
#define AEDIT_NOVICT_OTHERS		11
#define AEDIT_VICT_CHAR_FOUND		12
#define AEDIT_VICT_OTHERS_FOUND		13
#define AEDIT_VICT_VICT_FOUND		14
#define AEDIT_VICT_NOT_FOUND		15
#define AEDIT_SELF_CHAR			16
#define AEDIT_SELF_OTHERS		17
#define AEDIT_VICT_CHAR_BODY_FOUND     	18
#define AEDIT_VICT_OTHERS_BODY_FOUND   	19
#define AEDIT_VICT_VICT_BODY_FOUND     	20
#define AEDIT_OBJ_CHAR_FOUND		21
#define AEDIT_OBJ_OTHERS_FOUND		22
  
/*. Limit info .*/
#define MAX_ROOM_NAME	150
#define MAX_MOB_NAME	100
#define MAX_OBJ_NAME	100
#define MAX_ROOM_DESC	4096
#define MAX_EXIT_DESC	512
#define MAX_EXTRA_DESC  2048
#define MAX_MOB_DESC	512
#define MAX_OBJ_DESC	512
#define MAX_QUEST_NAME 	100
#define MAX_QUEST_INFO  4096
#define MAX_QUEST_ENDING 4096
#define MAX_POLI_KEYWORDS	75
#define MAX_POLI_ENTRY		MAX_STRING_LENGTH
#define MAX_HELP_KEYWORDS      75
#define MAX_HELP_ENTRY         MAX_STRING_LENGTH

#define HEDIT_MAIN_MENU                        0
#define HEDIT_ENTRY                    1
#define HEDIT_MIN_LEVEL                        2
#define HEDIT_KEYWORDS                 3
#define HEDIT_CONFIRM_SAVESTRING       4

#define PEDIT_MAIN_MENU                        0
#define PEDIT_ENTRY                    1
#define PEDIT_MIN_LEVEL                        2
#define PEDIT_KEYWORDS                 3
#define PEDIT_CONFIRM_SAVESTRING       4


#define HEDIT_PERMISSION       666     /* set people's olc_zone to     *
                                        * this to allow them to edit   *
                                        * help entries                 */

#define PEDIT_PERMISSION       666     /* set people's olc_zone to     *
                                        * this to allow them to edit   *
                                        * help entries                 */

/* #define HEDIT_LIST          1 */    /* define to log saves          */

/*
 * Funcs
 */
extern void qedit_parse(struct descriptor_data * d, char *arg);
extern void qedit_setup_new(struct descriptor_data *d);
extern void qedit_setup_existing(struct descriptor_data *d, int real_num);
extern void qedit_save_internally(struct descriptor_data *d);
extern void qedit_save_to_disk(int znum);
extern void free_quest(struct aq_data *quest);
extern void qedit_disp_type_menu(struct descriptor_data *d);
extern void qedit_disp_menu(struct descriptor_data * d);
extern void qedit_parse(struct descriptor_data * d, char *arg);
extern void qedit_disp_flags_menu(struct descriptor_data *d);

extern int real_zone(int);
extern void medit_parse(struct descriptor_data * d, char *arg);

extern void oedit_parse(struct descriptor_data * d, char *arg);
extern void oedit_disp_spells_menu(struct descriptor_data * d);
extern void oedit_liquid_type(struct descriptor_data * d);
extern void oedit_setup_new(struct descriptor_data *d);
extern void oedit_setup_existing(struct descriptor_data *d, int real_num);
extern void oedit_save_to_disk(struct descriptor_data *d);
extern void oedit_save_internally(struct descriptor_data *d);
extern void oedit_disp_container_flags_menu(struct descriptor_data * d);
extern void oedit_disp_extradesc_menu(struct descriptor_data * d);
extern void oedit_disp_weapon_menu(struct descriptor_data * d);
extern void oedit_disp_val1_menu(struct descriptor_data * d);
extern void oedit_disp_val2_menu(struct descriptor_data * d);
extern void oedit_disp_val3_menu(struct descriptor_data * d);
extern void oedit_disp_val4_menu(struct descriptor_data * d);
extern void oedit_disp_type_menu(struct descriptor_data * d);
extern void oedit_disp_extra_menu(struct descriptor_data * d);
extern void oedit_disp_wear_menu(struct descriptor_data * d);
extern void oedit_disp_menu(struct descriptor_data * d);

extern void redit_parse(struct descriptor_data * d, char *arg);
extern void redit_disp_extradesc_menu(struct descriptor_data * d);
extern void redit_disp_exit_menu(struct descriptor_data * d);
extern void redit_disp_exit_flag_menu(struct descriptor_data * d);
extern void redit_disp_flag_menu(struct descriptor_data * d);
extern void redit_disp_sector_menu(struct descriptor_data * d);
extern void redit_disp_menu(struct descriptor_data * d);
extern void redit_disp_mainterms(struct descriptor_data *d);
extern void redit_disp_room_prog(struct descriptor_data *d);
extern void redit_parse(struct descriptor_data * d, char *arg);
extern void redit_setup_new(struct descriptor_data *d);
extern void redit_setup_existing(struct descriptor_data *d, int real_num);
extern void redit_save_to_disk(struct descriptor_data *d);
extern void redit_save_internally(struct descriptor_data *d);
extern void free_room(struct room_data *room);
extern void aedit_save_to_disk(struct descriptor_data *d);
extern void free_action(struct social_messg *action);
extern int  find_action( int cmd );

extern void aedit_parse(struct descriptor_data *d, char *arg);
extern void sedit_parse(struct descriptor_data * d, char *arg);
extern void free_shop_strings(struct shop_data *shop);
extern void free_type_list(struct shop_buy_data **list);
int real_shop(int vshop_num);
extern void free_shop(struct shop_data *shop);
void sedit_setup_new(struct descriptor_data *d);
void sedit_setup_existing(struct descriptor_data *d, int rmob_num);
void sedit_parse(struct descriptor_data * d, char *arg);
void sedit_disp_menu(struct descriptor_data * d);
void sedit_namelist_menu(struct descriptor_data *d); 
void sedit_types_menu(struct descriptor_data *d);
void sedit_products_menu(struct descriptor_data *d);
void sedit_rooms_menu(struct descriptor_data *d);
void sedit_compact_rooms_menu(struct descriptor_data *d);
void sedit_shop_flags_menu(struct descriptor_data *d);
void sedit_no_trade_menu(struct descriptor_data *d);
void sedit_save_internally(struct descriptor_data *d);
void sedit_save_to_disk(struct descriptor_data *d); 
void copy_shop(struct shop_data *tshop, struct shop_data *fshop);
void copy_list(long **tlist, long *flist);
void copy_type_list(struct shop_buy_data **tlist, struct shop_buy_data *flist);
void sedit_add_to_type_list(struct shop_buy_data **list, struct shop_buy_data *new);

extern void zedit_parse(struct descriptor_data * d, char *arg);
extern void zedit_disp_menu(struct descriptor_data * d);  
extern void zedit_setup(struct descriptor_data *d, int room_num);
extern void add_cmd_to_list(struct reset_com **list, struct reset_com *newcmd, int pos);
extern void remove_cmd_from_list(struct reset_com **list, int pos);
extern void delete_command(struct descriptor_data *d, int pos);
extern int new_command(struct descriptor_data *d, int pos);
extern int start_change_command(struct descriptor_data *d, int pos);
extern void zedit_disp_comtype(struct descriptor_data *d);
extern void zedit_disp_arg1(struct descriptor_data *d);
extern void zedit_disp_arg2(struct descriptor_data *d);   
extern void zedit_disp_arg3(struct descriptor_data *d);
extern void zedit_save_internally(struct descriptor_data *d);
extern void zedit_save_to_disk(struct descriptor_data *d);   
extern void zedit_create_index(int znum, char *type);
extern void zedit_new_zone(struct char_data *ch, int vzone_num);

extern void medit_disp_menu(struct descriptor_data * d);
extern void medit_setup_new(struct descriptor_data *d); 
extern void medit_setup_existing(struct descriptor_data *d, int rmob_num);
extern void medit_save_internally(struct descriptor_data *d);
extern void medit_save_to_disk(struct descriptor_data *d);   
extern void init_mobile(struct char_data *mob);
extern void copy_mobile(struct char_data *tmob, struct char_data *fmob);
extern void medit_disp_positions(struct descriptor_data *d);
extern void medit_disp_mob_flags(struct descriptor_data *d);
extern void medit_disp_aff_flags(struct descriptor_data *d); 
extern void medit_disp_skill_flags(struct descriptor_data *d);
extern void medit_disp_attack_types(struct descriptor_data *d);
extern void medit_disp_teachermain(struct descriptor_data *d); 
extern void save_mobprog(FILE *file, struct char_data *mob);  
extern ACMD(do_rlink);

/* Copy from here down for PEDIT */

extern void hedit_parse(struct descriptor_data *d, char *arg);
extern void hedit_disp_menu(struct descriptor_data *d);
extern void hedit_save_to_disk(void);
extern void free_help(struct help_index_element *help);
extern int find_help_rnum(char *keyword);
extern void hedit_setup_new(struct descriptor_data *d, char *new_key);
extern void hedit_setup_existing(struct descriptor_data *d, int rnum);
extern void hedit_disp_extradesc_menu(struct descriptor_data *d);
extern void hedit_disp_exit_menu(struct descriptor_data *d);
extern void hedit_disp_exit_flag_menu(struct descriptor_data *d);
extern void hedit_disp_flag_menu(struct descriptor_data *d);
extern void hedit_disp_sector_menu(struct descriptor_data *d);
extern void hedit_disp_menu(struct descriptor_data *d);
extern void hedit_parse(struct descriptor_data *d, char *arg);
extern void hedit_setup_new(struct descriptor_data *d, char *new_key);
extern void hedit_setup_existing(struct descriptor_data *d, int rnum);
extern void hedit_save_to_disk(void);
extern void hedit_save_internally(struct descriptor_data *d);
extern void update_help_index();

extern void pedit_parse(struct descriptor_data *d, char *arg);
extern void pedit_disp_menu(struct descriptor_data *d);
extern void pedit_save_to_disk(void);
extern void free_poli(struct poli_index_element *poli);
extern int find_poli_rnum(char *keyword);
extern void pedit_setup_new(struct descriptor_data *d, char *new_key);
extern void pedit_setup_existing(struct descriptor_data *d, int rnum);
extern void pedit_disp_extradesc_menu(struct descriptor_data *d);
extern void pedit_disp_exit_menu(struct descriptor_data *d);
extern void pedit_disp_exit_flag_menu(struct descriptor_data *d);
extern void pedit_disp_flag_menu(struct descriptor_data *d);
extern void pedit_disp_sector_menu(struct descriptor_data *d);
extern void pedit_disp_menu(struct descriptor_data *d);
extern void pedit_parse(struct descriptor_data *d, char *arg);
extern void pedit_setup_new(struct descriptor_data *d, char *new_key);
extern void pedit_setup_existing(struct descriptor_data *d, int rnum);
extern void pedit_save_to_disk(void);
extern void pedit_save_internally(struct descriptor_data *d);
extern void update_poli_index();

#endif
