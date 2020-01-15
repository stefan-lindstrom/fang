/******************************************************************************
 * $Id: objprog.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: objprog.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: objprog.h,v $
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
*   File: objprog.h                                 Part of Dragons Fang *
*  Usage: header file for object programs                                *
************************************************************************ */


#ifndef __OBJPROG_H__
#define __OBJPROG_H__

struct obj_data;
struct char_data;

#include "prog.h"

/* Definitions of conditions that trigger obj action lists */

#define OPC_TIME         1
#define OPC_TICK         2
#define OPC_RANDOM       3
#define OPC_ENTRY        4
#define OPC_LEAVE        5
#define OPC_TAKE         6
#define OPC_DROP         7
#define OPC_WEAR         8
#define OPC_REMOVE       9
#define OPC_JUNK        10
#define OPC_DONATE      11
#define OPC_DIE         12
#define OPC_STEAL       13
#define OPC_ACTIONROOM	14
#define OPC_ACTIONTOROOM 15
#define OPC_ACTIONWEAR  16
#define OPC_ACTIONTOWEAR 17
#define OPC_ACTIONCARRY 18
#define OPC_ACTIONTOCARRY 19
#define OPC_SAYROOM     20
#define OPC_SAYWEAR	21
#define OPC_SAYCARRY	22
#define NUM_OBJ_MAINTERMS 22

#define OPC_SEX         1
#define OPC_LEVEL       2
#define OPC_WOLFKIN     3
#define OPC_DARKFRIEND  4
#define OPC_CARRYING    5
#define OPC_WEARING     6
#define OPC_RACE        7
#define OPC_HURT        8
#define OPC_TIRED       9
#define OPC_GUILD       10
#define OPC_NOBILITY    11
#define OPC_SNEAKING    12
#define OPC_BLIND       13
#define OPC_GRASPING    14
#define OPC_TAVEREN     15
#define OPC_STATUS      16
#define OPC_SLEEPING    17
#define OPC_QUESTING    18
#define OPC_WEARINGN    19
#define OPC_CARRYINGN   20
#define OPC_OUTSIDE     21
#define OPC_CHANNELER   22
#define OPC_QUESTED     23
#define OPC_AUTHORIZED  24
#define OPC_GRANKIS	25
#define OPC_GRANKLESS	26
#define OPC_GRANKMORE	27
#define NUM_OBJ_SIDETERMS 27

/* Definitions of Obj Program Actions (OPA) that the obj can do */
#define OPA_EMOTE       1
#define OPA_FORCE       2
#define OPA_STOP        3
#define OPA_PAUSE       4
#define OPA_GOTO        5
#define OPA_DESTROY     6
#define OPA_WEARDOWN    7
#define OPA_EMOTETO     8

#define NUM_OBJ_ACTIONS 8

extern void parse_objprog(FILE *obj_f, int i, int vnum);
extern int obj_side_conditions_match(struct char_data *ch, struct obj_data *obj, struct obj_prog_list *program);
extern int stops_in_objprog(struct obj_prog_list *program);
extern int obj_main_condition_matches(struct char_data *ch, struct obj_data *target, struct obj_data *obj, char *arg, struct obj_prog_list *program, int mode, int cmd);
extern int check_objprogs_at_interval(struct obj_data *obj, int mode);
extern int subcheck(struct char_data *ch, struct obj_data *target, char *arg, int mode, int cmd);
extern int check_for_objprogs_and_stops(struct char_data *ch, struct obj_data *target, char *arg, struct obj_data *list, int mode,  int cmd);
extern  void run_single_obj_program(struct obj_prog_list *program, struct char_data *ch, struct obj_data *obj, int cont);
extern void run_objprog(struct char_data *ch, struct obj_data *list);
extern void save_objprog(FILE *obj_file, struct obj_data *obj);
extern void free_objprog(struct obj_data *obj);

#endif
