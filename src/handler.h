/******************************************************************************
 * $Id: handler.h,v 1.3 2003/05/14 07:55:31 fang Exp $
 *   
 * File: handler.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: handler.h,v $
 *   Revision 1.3  2003/05/14 07:55:31  fang
 *   Added Improved Isname code --Tzeentch
 *
 *   Revision 1.2  2002/07/30 14:39:46  fang
 *   Drath: Another gskill update
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.5  2002/01/24 21:33:17  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.4.2.1  2002/01/23 21:49:41  mud
 *   Removed objsave.[c|h] (old objsave stuff).
 *   Removed obj_file_elem and rent structs.
 *   Removed final vestiges of old guild system, except parts used
 *   in guild.[c|h] for backwards compatibility.
 *
 *   Revision 1.4  2001/03/17 13:48:46  mud
 *   *** empty log message ***
 *
 *   Revision 1.3  2000/11/08 08:25:39  mud
 *   Moved update_p_tabel() to xmlcharsave.[ch].
 *
 *   Revision 1.2  2000/02/21 14:00:17  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* ************************************************************************
*   File: handler.h                                     Part of CircleMUD *
*  Usage: header file: prototypes of handling and utility functions       *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#ifndef __HANDLER_H__
#define __HANDLER_H__

#include "structs.h"

/* handling the affected-structures */
void	affect_total(struct char_data *ch);
void affect_modify(struct char_data * ch, byte loc, long mod, long bitv, long bitv2, long bitv3,
		        bool add,struct obj_data *obj);
void    affect_to_room(struct room_data *room,struct affected_type *af);
void    affect_from_room(struct room_data *room,int type);
void	affect_remove_room(struct room_data *room,struct affected_type *af);
void	affect_to_char(struct char_data *ch, struct affected_type *af);
void	affect_remove(struct char_data *ch, struct affected_type *af);
void	affect_from_char(struct char_data *ch, int type);
bool	affected_by_spell(struct char_data *ch, int type);
void	affect_join(struct char_data *ch, struct affected_type *af,
bool add_dur, bool avg_dur, bool add_mod, bool avg_mod);


/* utility */
char *money_desc(int amount);
struct obj_data *create_money(int amount);
int	isname(const char *str, const char *namelist);
char	*fname(char *namelist);
int	get_number(char **name);

/* ******** objects *********** */

void	obj_to_char(struct obj_data *object, struct char_data *ch);
void	obj_from_char(struct obj_data *object);

void	equip_char(struct char_data *ch, struct obj_data *obj, int pos);
struct obj_data *unequip_char(struct char_data *ch, int pos);

struct obj_data *get_obj_in_list(char *name, struct obj_data *list);
struct obj_data *get_obj_in_list_num(int num, struct obj_data *list);
struct obj_data *get_obj(char *name);
struct obj_data *get_obj_num(int nr);

void	obj_to_room(struct obj_data *object, int room);
void	obj_from_room(struct obj_data *object);
void	obj_to_obj(struct obj_data *obj, struct obj_data *obj_to);
void	obj_from_obj(struct obj_data *obj);
void	object_list_new_owner(struct obj_data *list, struct char_data *ch);

void	extract_obj(struct obj_data *obj);

/* ******* characters ********* */

struct char_data *get_char_room(char *name, int room);
struct char_data *get_char_num(int nr);
struct char_data *get_char(char *name);

void	char_from_room(struct char_data *ch);
void	char_to_room(struct char_data *ch, int room);
void	extract_char(struct char_data *ch, int delete);

struct char_data *get_char_pointer(char *name);
struct char_data *get_char_id(int id);
/* find if character can see */
struct char_data *get_char_room_vis(struct char_data *ch, char *name);
struct char_data *get_player_vis(struct char_data *ch, char *name, int inroom);
struct char_data *get_player(char *name, int inroom);
struct char_data *get_char_vis(struct char_data *ch, char *name);
struct char_data *get_char_vis_including_tar(struct char_data * ch, char *name);
struct obj_data *get_obj_in_list_vis(struct char_data *ch, char *name, 
struct obj_data *list);
struct obj_data *get_obj_vis(struct char_data *ch, char *name);
struct obj_data *get_object_in_equip_vis(struct char_data *ch,
char *arg, struct obj_data *equipment[], int *j);


/* find all dots */

int	find_all_dots(char *arg);

#define FIND_INDIV	0
#define FIND_ALL	1
#define FIND_ALLDOT	2


/* Generic Find */

int	generic_find(char *arg, int bitvector, struct char_data *ch,
struct char_data **tar_ch, struct obj_data **tar_obj);

#define FIND_CHAR_ROOM     1
#define FIND_CHAR_WORLD    2
#define FIND_OBJ_INV       4
#define FIND_OBJ_ROOM      8
#define FIND_OBJ_WORLD    16
#define FIND_OBJ_EQUIP    32

/*
 * Funcs
 */
extern struct obj_data *get_obj_worn_vis(struct char_data *ch,char *name);
extern bool room_affected_by_spell(struct room_data *room, int type);
extern int char_still_online(struct char_data *ch);

#endif
