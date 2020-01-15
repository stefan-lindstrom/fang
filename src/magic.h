/******************************************************************************
 * $Id: magic.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: magic.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: magic.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2001/07/12 15:27:42  mud
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/02/21 14:00:17  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#ifndef __MAGIC_H__
#define __MAGIC_H__

#include "structs.h"

/*
 * Funcs
 */
extern void remove_char_maintained(struct char_data *ch);
extern void remove_room_maintained(struct char_data *ch);
extern int get_ppos_char(struct char_data *ch);
extern struct char_data *get_char_ppos(int num);
extern void raffect_update(int speed);
extern void mana_update(int taint);
extern void add_longing(void);
extern void sub_longing(void);
extern void do_longing(void);
extern void taint_effects(void);
extern void update_noinc(void);
extern void affect_update(void);

#endif

