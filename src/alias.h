/******************************************************************************
 * $Id: alias.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: alias.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: alias.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/02/21 14:00:16  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#ifndef __ALIAS_H__
#define __ALIAS_H__

#include "structs.h"

/*
 * Funcs
 */

extern void write_aliases(struct char_data *ch);
extern void read_aliases(struct char_data *ch);

#endif

