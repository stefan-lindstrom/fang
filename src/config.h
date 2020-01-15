/******************************************************************************
 * $Id: config.h,v 1.2 2003/05/14 09:15:05 fang Exp $
 *   
 * File: config.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: config.h,v $
 *   Revision 1.2  2003/05/14 09:15:05  fang
 *   Added simple regexp codo isname. Might need some tweaking.
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2001/01/24 18:28:13  mud
 *   Added a way to get to the newbie zone (forgotten in the last commit)
 *
 *   Revision 1.2  2000/02/21 14:00:16  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "structs.h"

extern int use_regexp;
extern int use_abbrevs;
extern int pk_allowed;
extern int pt_allowed;
extern int level_can_shout;
extern int holler_move_cost;
extern int max_exp_gain;
extern int max_exp_loss;
extern int max_npc_corpse_time;
extern int max_pc_corpse_time;
extern int dts_are_dumps;
extern int free_rent;
extern int max_obj_save;
extern int min_rent_cost;
extern int auto_save;
extern int autosave_time;
extern int crash_file_timeout;
extern int rent_file_timeout;
extern sh_int mortal_start_room;
extern sh_int immort_start_room;
extern sh_int frozen_start_room;
extern sh_int death_room;
extern sh_int recall_room;
extern sh_int newbie_room;
extern sh_int donation_room_1;
extern sh_int donation_room_2;
extern sh_int donation_room_3;
extern int DFLT_PORT;
extern int MAX_PLAYERS;
extern int max_filesize;
extern int max_bad_pws;
extern int nameserver_is_slow;
extern int use_autowiz;
extern int min_wizlist_lev;
extern char *OK;
extern char *NOPERSON;
extern char *NOEFFECT;
extern char *DFLT_DIR;
extern char *GREETINGS;
extern char *WELC_MESSG;
extern char *START_MESSG;

#endif

