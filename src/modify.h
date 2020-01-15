/******************************************************************************
 * $Id: modify.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: modify.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: modify.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.4  2000/08/03 19:28:43  mud
 *
 *   Patches for talent, and added talent requirements for skills/spells
 *
 *   Revision 1.3  2000/06/09 18:40:53  mud
 *   Purdans 'Editor Name List' for zoned added.
 *
 *   Revision 1.2  2000/02/21 14:00:18  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#ifndef __MODIFY_H__
#define __MODIFY_H__

#include "structs.h"

/*
 * Funcs
 */
extern void show_string(struct descriptor_data *d, char *input);
extern void string_add(struct descriptor_data *d, char *str);   
extern int  ptable_lookup(char *name);
extern int  meets_skillspell_reqs(struct char_data* vict, int skill);
#endif
