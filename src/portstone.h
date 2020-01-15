/******************************************************************************
 * $Id: portstone.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *
 * File: portstones.h
 *
 * Created: Fri May 24 2002 by Stefan Lindstrom (asmodean@ludd.luth.se)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada,
 * yada...
 *
 * Attempt at a more dynamic portal stone systems. Portal stones are stored in
 * files which are read at bootup. New stones can be added, and old edited/deleted
 * online with an OLC like system. 
 *
 *   $Log: portstone.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.4  2002/06/06 08:04:08  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/06/04 09:20:02  fang
 *   More portstone work
 *
 *   Revision 1.2  2002/05/24 13:07:04  fang
 *   more stone stuff...
 *
 *   Revision 1.1  2002/05/24 11:39:52  fang
 *   First commit
 * 
 *
 ******************************************************************************/
#ifndef __PORTSTONE_H__
#define __PORTSTONE_H__

#include "structs.h"
#include "act.h"

#define PORTAL_LIST        800
#define PORTAL_TRANSF_GM   801
#define PORTAL_TRANSF_SELF 802

#define PORT_FILE "./world/wld/portal_stones"

typedef long stone_num;
struct  stone;
typedef struct stone stone_t;

extern void destination_list(struct char_data *ch);
extern void travel_to(struct char_data *ch, const char *dest);
extern stone_t *find_stone_by_roomnum(room_num vnum);
extern stone_t *find_stone_by_roomname(const char *namelist);
extern int read_stones(void);
extern int save_stones(void);

extern int      stones_start(void);
extern int      stones_next(void);
extern stone_t *stones_this(void);
extern stone_t *create_stone(void);
extern int      st_destination_start(stone_t *s);
extern int      st_destination_next(stone_t *s);
extern int      st_destination_this(stone_t *s);

extern void add_stone(stone_t *st);
extern void st_add_destination(stone_t *s, int destroom);
extern void st_set_name(stone_t *st, const char *name);
extern void st_set_stonenum(stone_t *st, int num);
extern void st_set_in_room(stone_t *s, int room);

extern void delete_stone_num(int stonenum);
extern void delete_stone_name(const char *name);
extern void st_delete_destination(stone_t *s, int dest);

extern void stone_free(stone_t *s);

extern void st_list_wiz(struct char_data *ch);

extern ACMD(do_pstest);
extern SPECIAL(new_stone);

#endif
