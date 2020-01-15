/******************************************************************************
 * $Id: class.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: class.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: class.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.6  2002/01/24 21:33:14  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.5.2.1  2002/01/21 01:23:40  mud
 *   Code cleanup. Aiel clans/socities gone. Lot of old guild code gone.
 *
 *   Revision 1.5  2001/10/14 05:22:47  mud
 *   *** empty log message ***
 *
 *   Revision 1.4  2000/11/07 12:00:39  mud
 *   Finally. XML Mobiles are done.
 *
 *   Revision 1.3  2000/04/16 17:48:52  mud
 *   Added "const" to three string arrays, for constistency's sake.
 *
 *   Revision 1.2  2000/02/21 14:00:16  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#ifndef __CLASS_H__
#define __CLASS_H__

#include "structs.h"
#include "spells.h"
#include "db.h"

/*
 * Data
 */
extern sh_int home_rooms[];
extern BPTR(spell_list[MAX_SPELLS]);
extern const char *ajahs[];
extern const char *ajah_colors[];
extern const char *pc_class_types[NUM_CLASSES+1];
extern const char *blademastery[];
extern const char *class_abbrevs[];
extern struct cost da_costs[3];
extern struct cart_data cartzones[];
extern struct wheel_data wheel[];
extern const int payoffs[];
extern int rooms_that_save_objs[];
extern const char *wkrank[];

/*
 * Funcs
 */
extern int backstab_mult(int level);
extern int parse_class(char *arg);
extern void do_start(struct char_data * ch);
extern int classStr2Num(const char *name);

#endif

