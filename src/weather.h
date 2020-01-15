/******************************************************************************
 * $Id: weather.h,v 1.2 2003/05/11 14:50:20 fang Exp $
 *   
 * File: weather.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: weather.h,v $
 *   Revision 1.2  2003/05/11 14:50:20  fang
 *   DYnvar/noformat handling code should be
 *    complete. Minor bugs may remain, so keep an eye open
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/02/21 14:00:20  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/
#include "interpreter.h"

#ifndef __WEATHER_H__
#define __WEATHER_H__

/* 
 * FUncs
 */
extern void weather_and_time(int mode);
extern void another_hour(int mode);
extern void weather_change(void);

extern ACMD(do_wset);
extern ACMD(do_tset);

#endif

