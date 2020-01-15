/******************************************************************************
 * $Id: spec_procs.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: spec_procs.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: spec_procs.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.5  2002/01/24 21:33:20  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.4.2.1  2002/01/24 20:49:38  mud
 *   More cleanup. Removed files:  specialday.c string.c string.h
 *   Removed DONT_TRADE_WITH-stuff from shops.
 *
 *   Revision 1.4  2001/03/17 10:40:47  mud
 *   New and Improved TAR.
 *
 *   Revision 1.3  2000/03/01 12:54:36  mud
 *   General cleanup off stuff
 *
 *   Revision 1.2  2000/02/21 14:00:19  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#ifndef __SPECPROC_H__
#define __SPECPROC_H__

#include "spells.h"
#include "structs.h"

/*
 * Data
 */
extern int spell_sort_info[MAX_SKILLS+1];
extern byte race_is_on;
extern byte horse_timer;

/*
 * Funcs
 */
extern int check_taveren(struct char_data *ch);
extern int check_weaves(int i,struct char_data *ch);
extern SPECIAL(shop_keeper);
SPECIAL(bomb);
SPECIAL(taunt);  
  SPECIAL(bank); 
  SPECIAL(postmaster);
  SPECIAL(guild);
  SPECIAL(puff);
  SPECIAL(fido);
  SPECIAL(janitor);
  SPECIAL(thief);
  SPECIAL(trainer);
  SPECIAL(laras);
  SPECIAL(headgear);
  SPECIAL(sp_gate);
  SPECIAL(gen_board);
  SPECIAL(pigeon);   
  SPECIAL(herb);  
  SPECIAL(aelfinn);
  SPECIAL(recall_charm);
  SPECIAL(strong_dream_obj);   
  SPECIAL(weak_dream_obj);
  SPECIAL(dream_gate);
  SPECIAL(warder_cloak); 
  SPECIAL(dump);
  SPECIAL(customs);
  SPECIAL(hospital);
  SPECIAL(button_room);
  SPECIAL(skillshop);
  SPECIAL(portal_stones);
  SPECIAL(bank);
  SPECIAL(death_wheel);
  SPECIAL(dreamroom);
  SPECIAL(repair_shop);
  SPECIAL(newbiehealer);

extern SPECIAL(strong_dream_obj);
extern SPECIAL(weak_dream_obj);
extern void list_skills(struct char_data * ch, struct char_data *vict);
extern void sort_spells(void);
extern void ASSIGNOBJ(int obj, SPECIAL(fname));

#endif

