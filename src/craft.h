/******************************************************************************
 * $Id: craft.h,v 1.3 2003/02/02 14:48:49 fang Exp $
 *   
 * File: craft.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: craft.h,v $
 *   Revision 1.3  2003/02/02 14:48:49  fang
 *   Extra descs added to tool as well.
 *
 *   Revision 1.2  2002/08/30 19:52:28  fang
 *   Drath: New room DG trigger, LEAVE
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2000/12/03 10:26:10  mud
 *   More QEQ tracking, begun some minro mods to have exdescs editable by craft/tool.
 *
 *   Revision 1.2  2000/02/21 14:00:16  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#ifndef __CRAFT_H__
#define __CARFT_H__

#include "structs.h"
#include "olc.h"

#define TOOL_QUIT     0
#define TOOL_MENU     1
#define TOOL_NAMELIST 2
#define TOOL_DESC     3
#define TOOL_SDESC    4
#define TOOL_ADESC    5
#define TOOL_EDESC    6
#define TOOL_PAUSE    7
#define TOOL_EDESCK   8   /* Editing Keyword of ExtraDesc */
#define TOOL_EDESCD   9   /* Editing Description of ExtraDesc */

#define CRAFT_MENU      0
#define CRAFT_KEYWORD	1
#define CRAFT_SDESC	2
#define CRAFT_DESC	3
#define CRAFT_TYPE	4
#define CRAFT_WEAR   	5
#define CRAFT_AFFECT1	6
#define CRAFT_AFFECT2	7
#define CRAFT_AFFECT3   8
#define CRAFT_VALUE1	9
#define CRAFT_VALUE2   10
#define CRAFT_VALUE3   12
#define CRAFT_VALUE4   13
#define CRAFT_REMOVE   14
#define CRAFT_END      15
#define CRAFT_EDESC    16
#define CRAFT_EDESCK   17
#define CRAFT_EDESCD   18

#define CRAFT_OBJ_VNUM	2 /* Change if you want another obj as prototype */

extern void show_craft_edesc_menu(struct char_data *ch, struct extra_descr_data *xdesc);
extern void show_tool_edesc_menu(struct char_data *ch, struct extra_descr_data *xdesc);
extern void tool_edesc_parse(struct char_data *ch,char *buf);
extern void craft_edesc_parse(struct char_data *ch,char *buf);
extern void parse_craft(struct descriptor_data *d,char *arg);
extern int craft_parse_menu(char *, struct char_data *);
extern int  craft_parse_type(struct char_data *);
extern int  craft_parse_affect1(struct char_data *,char *);
extern int  craft_parse_affect2(struct char_data *,char *);
extern int  check_craft_obj(struct char_data *);
extern void craft_disp_menu(struct char_data *);
extern long craft_calc_cost(struct char_data *);
extern void craft_show_wearbits(struct char_data *);
extern void craft_show_affects(struct char_data *);
extern int craft_parse_affect2(struct char_data *, char *);
extern void craft_parse_affect3(struct char_data *, char *);
extern void limit_objval1(struct char_data *);
extern void limit_objval2(struct char_data *);
extern void limit_objval3(struct char_data *);
extern void limit_objval4(struct char_data *);
extern int craft_check_obj(struct char_data *);
extern void show_tool_obj(struct char_data *ch);
extern void parse_tool_command(struct char_data *ch,char *arg);
extern void parse_tool(struct char_data *ch,char *arg);

#endif
