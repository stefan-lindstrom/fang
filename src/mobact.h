/******************************************************************************
 * $Id: mobact.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: mobact.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: mobact.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/02/21 14:00:18  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#ifndef __MOBACT_H__
#define __MOBACT_H__

#include "structs.h"

/*
 * Funcs
 */
extern void mobile_activity(void);
extern void perform_mob_spells(struct char_data *ch);
extern void clearMemory(struct char_data * ch);
extern void remember(struct char_data * ch, struct char_data * victim);
extern void forget(struct char_data * ch, struct char_data * victim);

#endif
