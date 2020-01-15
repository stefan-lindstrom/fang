/******************************************************************************
 * $Id: constants.h,v 1.2 2002/07/29 22:18:56 fang Exp $
 *   
 * File: constants.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: constants.h,v $
 *   Revision 1.2  2002/07/29 22:18:56  fang
 *   AFF3 created and gskill update
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.4  2002/06/25 19:23:51  fang
 *   Drath: XLOG Commit #2
 *
 *   Revision 1.3  2002/06/02 22:14:44  fang
 *   Maz: PEDIT
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.11  2002/02/01 13:53:48  mud
 *   PRM fl|ags now shown in stat
 *
 *   Revision 1.10  2002/01/24 21:33:14  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.9.2.1  2002/01/22 03:05:11  mud
 *   More cleanup. Removed extern decls from source files. Protected some DG headers from multiple inclision
 *
 *   Revision 1.9  2001/01/31 15:44:53  mud
 *   *** empty log message ***
 *
 *   Revision 1.8  2000/08/20 07:59:52  mud
 *   added a help_index. ordered linkedlist of help keywords and their pos in the help_table
 *
 *   Revision 1.7  2000/08/10 15:06:29  mud
 *   added hedit (help editor)
 *
 *   Revision 1.6  2000/08/03 19:28:43  mud
 *
 *   Patches for talent, and added talent requirements for skills/spells
 *
 *   Revision 1.5  2000/07/28 17:44:33  mud
 *
 *   New talent code.
 *
 *   Revision 1.4  2000/07/24 06:15:52  mud
 *   Added global board!
 *
 *   Revision 1.3  2000/05/25 15:06:48  mud
 *
 *   Talent code changed to utilize char* arrays instead of a million #defines.
 *   Changes made to do_set, do_stat, do_score.
 *   Imm command Prove added.
 *
 *   Revision 1.2  2000/02/21 14:00:16  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include "structs.h"
extern const char *extended_syslogs[];
extern const struct mirroring_data mirrored_rooms[];
extern const char circlemud_version[];
extern const char *dirs[];
extern const char *room_bits[];
extern const char *exit_bits[];
extern const char *sector_types[];
extern const char *genders[];
extern const char *position_types[];
extern const char *player_bits[];
extern const char *mobskill_bits[];
extern const char *action_bits[];
extern const char *preference_bits[];
extern const char *preference2_bits[];
extern const char *permanent_bits[];
extern const char *affected_bits[];
extern const char *affected2_bits[];
extern const char *affected3_bits[];
extern const char *raffected_bits[];
extern const char *connected_types[];
extern const char *where[];
extern const char *equipment_types[];
extern const char *item_types[];
extern const char *wear_bits[];
extern const char *extra_bits[];
extern const char *apply_types[];
extern const char *container_bits[];
extern const char *drinks[];
extern const char *drinknames[];
extern const char *color_liquid[];
extern const char *fullness[];
extern const char *obj_status[];
extern const char *room_mainterms[];
extern const char *room_sideterms[];
extern const char *room_actions[];
extern const char *obj_mainterms[];
extern const char *omt_file[];
extern const char *obj_sideterms[];
extern const char *ost_file[];
extern const char *obj_actions[];
extern const char *obj_act_file[];
extern const char *mainterms[];
extern const char *mt_file[];
extern const char *sideterms[];
extern const char *st_file[];
extern const char *actions[];
extern const char *act_file[];
extern const char *spell_wear_off_msg[];
extern const char *npc_class_types[];
extern const char *languages[];
extern const char *weekdays[];
extern const char *month_name[];
extern const int drink_aff[][3];
extern const char *room_mainterms[];
extern const char *obj_mainterms[];
extern const char *mainterms[];
extern const int rev_dir[];
extern const int movement_loss[];
extern const int sharp[];
extern char REM_MAIN_TXT[];
extern const char *furniture_types[];
extern const char *furniture_usages[];
extern struct help_index_element *help_table;
extern struct help_indexer_element *help_index;
extern struct poli_index_element *poli_table;
extern struct poli_indexer_element *poli_index;
#define NUM_TALENTS 33
#define IS_TALENT_SET(flag1, flag2, bit) ((bit < 32)? ((flag1) & (1 << bit)) : ((flag2) & (1 << (bit - 32))))
#define SET_TALENT(flag1, flag2, bit)  (bit < 32) ? ((flag1) |= (1 << bit)) : ((flag2) |= (1 << (bit - 32)))
#define REMOVE_TALENT(flag1, flag2, bit) (bit < 32) ? ((flag1) &= ~(1 << bit)) : ((flag2) &= ~(1 << (bit - 32)))
#define IS_PRIMARY(bit) (master_talent_list[bit].primtalent)
#define IS_SECONDARY(bit) (master_talent_list[bit].sectalent)
#define IS_SPSKILL(bit) (master_talent_list[bit].sskill)
struct talents{
	char* name;
	ubyte primtalent;
	ubyte sectalent;
	ubyte sskill;
};
extern const struct talents master_talent_list[NUM_TALENTS];
extern const char *cg_names[];
extern const char *toggle_scmds[];
extern const char *cg_bits[];

#define GBOARD 23
#endif
