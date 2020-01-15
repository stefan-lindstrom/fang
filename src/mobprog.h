/******************************************************************************
 * $Id: mobprog.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: mobprog.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: mobprog.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.3  2002/06/12 09:55:16  fang
 *   More mem thingies...
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/02/21 14:00:18  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/*************************************************************************
*   File: mobprog.h                                 Part of Dragons Fang *
*  Usage: header file for mobile programs                                *
************************************************************************ */

#ifndef __MOBPROG_H__
#define __MOBPROG_H__

#include <stdio.h>

#include "prog.h"

struct char_data;

#define MPC_TIME         1
#define MPC_MOBENTRY     2
#define MPC_MOBLEAVE     3
#define MPC_MOBDIE       4
#define MPC_ENTRY        5
#define MPC_LEAVE        6
#define MPC_SAY          7
#define MPC_SAYTO        8
#define MPC_ACTION       9
#define MPC_ACTIONTO    10
#define MPC_ATTACK      11
#define MPC_MOBATTACK   12
#define MPC_DIE         13
#define MPC_GIVE        14
#define MPC_TICK        15
#define MPC_RANDOM      16
#define MPC_TELL        17

#define NUM_MAINTERMS   17
/* Definitions of side conditions that trigger mob action lists */


#define MPC_SEX          1
#define MPC_LEVEL        2
#define MPC_WOLFKIN      3
#define MPC_DARKFRIEND   4
#define MPC_CARRYING     5
#define MPC_WEARING      6
#define MPC_RACE         7
#define MPC_HURT         8
#define MPC_TIRED        9
#define MPC_GUILD       10
#define MPC_NOBILITY    11
#define MPC_SNEAKING    12
#define MPC_BLIND       13
#define MPC_GRASPING    14
#define MPC_TAVEREN     15
#define MPC_MODE        16
#define MPC_SLEEPING    17
#define MPC_INVISIBLE   18
#define MPC_QUESTING    19
#define MPC_WEARINGN    20
#define MPC_CARRYINGN   21
#define MPC_OUTSIDE     22
#define MPC_CANCHANNEL  23
#define MPC_QUESTED     24
#define MPC_AUTHORIZED  25
#define MPC_GRANKIS     26
#define MPC_GRANKLESS   27
#define MPC_GRANKMORE   28


#define NUM_SIDETERMS   28

/* Definitions of Mob Program Actions  (MPA) that the mob can do */


#define MPA_MOVETO       1
#define MPA_SAY          2
#define MPA_SAYTO        3
#define MPA_ACTION       4
#define MPA_ACTIONTO     5
#define MPA_FORCE        6
#define MPA_MODE         7
#define MPA_GIVE         8
#define MPA_CAST         9
#define MPA_EMOTE       10
#define MPA_STOP        11
#define MPA_MOVE        12
#define MPA_PAUSE       13
#define MPA_DOING       14
#define MPA_PURGE       15
#define MPA_SHOUT       16

#define NUM_MOBACTIONS  16

extern void parse_mobprog(FILE *mob_f, int i, int vnum);
extern int side_conditions_match(struct char_data *ch, struct char_data *mob, struct mob_prog_list *program);
extern int stops_in_mobprog(struct mob_prog_list *program);
extern int main_condition_matches(struct char_data *ch, struct char_data *target, struct char_data *mob, char *arg, struct mob_prog_list *program, int mode, int cmd, int num);
extern int check_mobprogs_at_interval(struct char_data *mob, int mode);
extern int check_for_mobprogs_and_stops(struct char_data *ch, struct char_data *target, char *arg, int mode, int cmd, int number);
extern void run_single_program(struct mob_prog_list *program, struct char_data *ch, struct char_data *mob, int cont);
extern void run_mobprog(struct char_data *ch, long room);
extern void save_mobprog(FILE *mob_file, struct char_data *mob);
extern void free_mobprog(struct char_data *ch);

#endif
