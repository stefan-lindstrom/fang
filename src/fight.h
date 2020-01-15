/******************************************************************************
 * $Id: fight.h,v 1.3 2003/06/20 07:49:55 fang Exp $
 *   
 * File: fight.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: fight.h,v $
 *   Revision 1.3  2003/06/20 07:49:55  fang
 *
 *   db.c now loads xml-messages instead of old message files. Old code for message
 *   loading have been removed.
 *
 *   Revision 1.2  2002/09/22 10:50:07  fang
 *   Made shield reduction work for triggered %damage% calls as well.
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/01/14 19:28:01  mud
 *
 *
 *   Autoloot now checks that character can carr item before transfing the
 *   item to its inventory. If carrying is not possible, the item will
 *   fall to the ground in the room. Blind people can autojunk (i.e after
 *   gettinf dirtkicked).
 *
 *   Revision 1.2  2000/02/21 14:00:17  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#ifndef __FIGHT_H__
#define __FIGHT_H__

#include "structs.h"

/*
 * Data
 */
extern struct attack_hit_type attack_hit_text[];
extern struct char_data *next_combat_list ;
extern struct char_data *combat_list;

/*
 * Funcs 
 */
extern char *write_random_message(struct char_data *ch);
extern void die(struct char_data * ch, struct char_data *killer);
extern int is_dreamer(struct char_data *ch);
extern void check_mana(struct char_data *ch);
extern void appear(struct char_data * ch);
extern int is_in_group(struct char_data *ch,struct char_data *vict);
extern struct obj_data *make_corpse(struct char_data * ch);
extern void death_cry(struct char_data * ch);
extern void raw_kill(struct char_data * ch, struct char_data *killer);
extern void perform_violence(void);
extern void set_fighting(struct char_data * ch, struct char_data * vict);
extern void hit(struct char_data * ch, struct char_data * victim, int type);
extern void damage(struct char_data * ch, struct char_data * victim, int dam, int attacktype);
extern void stop_fighting(struct char_data * ch);
extern int not_same_dreamer(struct char_data *ch,struct char_data *vict);
extern void do_own_damage(struct char_data *list);
extern void do_area_spell_agg(struct char_data *ch,int dam,int skill);
extern void do_area_spell(struct char_data *ch,int damage,int skill);  
extern void do_area_ngroup_agg(struct char_data *ch,int dam,int skill);
extern int spell_message(int type,struct char_data *ch,struct char_data *vict,int target,int success);
extern int tp_message(int type, struct char_data *ch,struct char_data *vict,int target,int success);
extern void shield_reduce(struct char_data *ch, int *dam);

#endif
