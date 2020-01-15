/******************************************************************************
 * $Id: dynvars.h,v 1.5 2003/05/13 12:02:56 fang Exp $
 *   
 * File: dynvars.h
 *
 * Created: Tue May 6 2003 by Stefan Lindstrom (asmodean@ludd.luth.se)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * Embedded "dynamic vars" in descriptions. 
 * 
 *   $Log: dynvars.h,v $
 *   Revision 1.5  2003/05/13 12:02:56  fang
 *   Minor corrections...
 *
 *   Revision 1.3  2003/05/12 09:08:33  fang
 *   Enabled dynvars for room descs/xdescs
 *
 *   Revision 1.2  2003/05/08 13:18:16  fang
 *   Some more work on dyn.vars
 *
 *   Revision 1.1  2003/05/06 12:37:32  fang
 *   *** empty log message ***
 *
 *
 ******************************************************************************/
#ifndef __DYNVARS_H__
#define __DYNVARS_H__

#include "interpreter.h"

#ifndef FORMAT_WIDTH
#define FORMAT_WIDTH 78
#endif

extern int init_dynvars(void);
extern int close_dynvars(void);

extern char *char_look_at_char(character_t *ch, character_t *vict, const char *desc);
extern char *char_look_at_room(character_t *ch, const char *txt);
extern char *char_look_at_obj(character_t *ch, obj_t *obj, const char *desc);

#endif

