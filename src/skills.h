/******************************************************************************
 * $Id: skills.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: skills.h
 *
 * Created: Wed Mar 15 2000 by Stefan Lindstrom (asmodean@ludd.luth.se)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * Yet another encapsulation of skills/spells name->index# mapping. guess we'll
 * drop the old binarytree one, if this porperly allows us to find skills/weaves
 * in an non-erronmous way. :)
 * 
 *   $Log: skills.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.3  2002/06/13 10:12:28  fang
 *   More free'ing stuff...
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2002/01/24 21:33:20  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.1.2.1  2002/01/24 20:49:37  mud
 *   More cleanup. Removed files:  specialday.c string.c string.h
 *   Removed DONT_TRADE_WITH-stuff from shops.
 *
 *   Revision 1.1  2000/03/15 13:11:20  mud
 *   First commit
 *
 *
 *****************************************************************************/

#ifndef __SKILLS_H__
#define __SKILLS_H__

#include "interpreter.h"

extern void build_skill_index(void);
extern int find_skillnum_by_name(char *skill);
extern void free_skill_index(void);

#endif
