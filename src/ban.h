/******************************************************************************
 * $Id: ban.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: ban.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: ban.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.4  2002/06/13 10:12:28  fang
 *   More free'ing stuff...
 *
 *   Revision 1.3  2002/03/11 10:50:59  fang
 *   xname fix
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/02/21 14:00:16  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#ifndef __BAN_H__
#define __BAN_H__

#include "db.h"
/*
 * Data
 */
extern struct ban_list_element *ban_list;

/*
 * Funcs
 */
extern int isbanned(char *hostname);
extern void load_banned(void);
extern void free_banned(void);
extern void Read_Invalid_List(void);
extern void free_invalid_list(void);
extern int Valid_Name(char *newname);

#endif

