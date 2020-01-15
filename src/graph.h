/******************************************************************************
 * $Id: graph.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: graph.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: graph.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/01/30 09:20:44  mud
 *   shortgate added. Also made gate not choke on unknown rooms, if distance between rooms are not morre than 10 rooms
 *
 *   Revision 1.2  2000/02/21 14:00:17  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "structs.h"

/*
 * Data
 */

/*
 * Funcs
 */
extern int find_first_step(sh_int src, sh_int target);
extern int b_find_first_step(sh_int src, sh_int target);
extern void mob_goto(struct char_data *ch, int to_room);
extern void hunt_victim(struct char_data * ch);
extern int steps_between(int rroom1, int rroom2);

#endif

