/******************************************************************************
 * $Id: namedspec.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: namedspec.h
 *
 * Created: Wed 2 Mar 2000 by Stefan Lindstrom (asmodean@ludd.luth.se)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * Code for named specials. Specials will (at least) be saved by name in 
 * rooms, and will be setable via OLC. 
 * 
 *   $Log: namedspec.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.3  2002/06/13 10:12:28  fang
 *   More free'ing stuff...
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2000/03/01 15:54:32  mud
 *   First commit.
 *
 *   Revision 1.2  2000/03/01 15:15:45  mud
 *   Named specials added, saved in room XML, and setable in redit
 *
 *   Revision 1.1  2000/03/01 12:54:19  mud
 *   First commit
 *
 *
 *****************************************************************************/

#ifndef __NAMEDSPEC_H__
#define __NAMEDSPEC_H__

#include "structs.h"
#include "interpreter.h"
#include "utils.h"

typedef int(*spec_p)(struct char_data *ch, void *me, int cmd, char *argument);

extern spec_p        getSpecialByNum(int numbertotry);
extern void         *getSpecialName(char *name);
extern void         *getSpecialPointer(SPECIAL(*fn));
extern void          registerSpecial(char *name, SPECIAL(*fn));
extern void          bootNamedSpecials(void);
extern void          makeSpecialList(struct char_data *ch);
extern void          de_boot_namedspecials(void);

extern ACMD(test_ns);

#endif
