/******************************************************************************
 * $Id: limits.h,v 1.2 2003/11/23 08:02:24 fang Exp $
 *   
 * File: limits.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: limits.h,v $
 *   Revision 1.2  2003/11/23 08:02:24  fang
 *   Tze: Changed who page to write every 30 seconds as per admin request
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/02/21 14:00:17  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#ifndef __LIMITS_H__
#define __LIMITS_H__

/*
 * Funcs
 */
extern void fast_update(void);
extern void heartbeat_regen(void);
extern void five_minute_update(void);
extern void write_whowebpage(void);

/*
 * Data
 */
/*extern struct horse_data;
extern struct horse_data horses[10];*/
#endif

