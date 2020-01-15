/******************************************************************************
 * $Id: stoneedit.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *
 * File: stoneedit.h
 *
 * Created: Fri May 24 2002 by Stefan Lindstrom (asmodean@ludd.luth.se)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada,
 * yada...
 *
 * OnLine editor for Portal Stones. 'nuff said. 
 *
 *   $Log: stoneedit.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/06/06 13:06:15  fang
 *   *** empty log message ***
 *
 *   Revision 1.1  2002/06/04 09:36:26  fang
 *   First commit
 *
 ******************************************************************************/
#ifndef __STONEEDIT_H__
#define __STONEEDIT_H__

#include "interpreter.h"

struct sedit {
  int mode;
};

#define SE_MODE(d)  ((d)->sedit->mode)

#define STEDIT_MAIN_MENU 1
#define STEDIT_EDIT_NAME 2
#define STEDIT_DESTLISt  3

extern ACMD(do_stoneedit);

#endif
