/******************************************************************************
 * $Id: bitvector.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: bitvector.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: bitvector.h,v $
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

/*
 * Attempt at a generic bitvector module, that can easily adapt to 
 * changes in size between saves, etc....
 * primary to be used in the room-knowledge thingy (gate/skimmin), 
 * but might come in handy for players/affections/etc...
 */

#ifndef __BVECT_H__
#define __BVECT_H__

#include <stdio.h>

struct bitv;
typedef struct bitv bv_t, *bv_tp, **bv_tpp;

extern bv_tp BITVECTOR(unsigned long); 
extern void  set_bit(bv_tp,unsigned long); 
extern int   test_bit(bv_tp,unsigned long);
extern void  toggle_bit(bv_tp,unsigned long);
extern void  clear_bit(bv_tp,unsigned long);
extern void  KILL_BV(bv_tp);
extern int   to_file(bv_tp,FILE *);
extern bv_tp from_file(bv_tp,FILE *);
extern char *bv_toString(bv_tp,char **);

#endif

