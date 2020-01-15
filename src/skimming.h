/******************************************************************************
 * $Id: skimming.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: skimming.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: skimming.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/02/21 14:00:19  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/*
 * Skimming. 
 * This one'll be pretty tough me thinks. We need to be able to dynamically
 * add Skim-rooms, and remove them as the char(s) reaches their destination. 
 * One could add to the index as 'usual' using OLC-like methods, and remove 
 * them when done, or create a special kind of fake'd skim-rooms, maintain
 * them internally, but that seems extremely bothersome. 
 * Adding to index like ordinary OLC is feasable, although, that'll require 
 * a zone or so to use, and we're short on zones as it is. 
 * If we could easily extend the room/zone/obj/mob->numbers to be 
 * 32 bits instead of 16. *sigh*
 */
#ifndef __SKIM_H__
#define __SKIM_H__

#include "structs.h"
#include "spells.h"

#define HASNT_STARTED  -1
#define IS_TRAVELING   -2
#define TARGET_REACHED -3

extern BSPELL(skim);
extern void fix_skim_gates(struct char_data *ch,long ppos);
extern void update_skims(void);

#endif
