/******************************************************************************
 * $Id: xmlobjsave.h,v 1.2 2002/08/30 21:23:09 fang Exp $
 *   
 * File: xmlobjsave.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: xmlobjsave.h,v $
 *   Revision 1.2  2002/08/30 21:23:09  fang
 *   Put in retainers.
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:29  fang
 *   *** empty log message ***
 *
 *   Revision 1.5  2000/04/10 21:14:46  mud
 *   Fixed a few minor things that had to do with xml saving of room objs.
 *
 *   Revision 1.4  2000/04/10 20:07:34  mud
 *   Fixed color saving with &'s in the strings.
 *
 *   Revision 1.3  2000/04/09 20:34:05  mud
 *   Changed player obj saving to XML format.
 *
 *   Revision 1.2  2000/02/21 14:00:20  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#ifndef __XMLOBJSAVE_H__
#define __XMLOBJSAVE_H__

#include "structs.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/entities.h>
#include <libxml/parserInternals.h>
#include <errno.h>


#include "structs.h"
#include "comm.h"
#include "handler.h"
#include "db.h"
#include "interpreter.h"
#include "utils.h"
#include "class.h"

struct mount_data; // Forward decl of mountdata, ugly but...

/* these factors should be unique integers */
/*
 * Functions...
 */
extern void save_XML_playerfile(struct char_data *ch, int mode);
extern void read_XML_playerfile(struct char_data *ch, int mode);
extern struct obj_data *get_one_obj_XML(xmlNodePtr p);
extern xmlNodePtr obj_to_XML(struct obj_data *obj, xmlDocPtr doc);
extern void delete_playerfile(char *name);
extern xmlNodePtr mount_to_XML(struct mount_data *m, xmlDocPtr p);
extern ACMD(do_listobj);
extern void save_all_xml();
extern void save_objs_in_rooms();
extern void read_XML_roomobjs(int vnum);
extern void strip_char_of_all_eq(struct char_data *ch);
extern void eq_to_xml(struct char_data *, xmlNodePtr, xmlDocPtr);
extern void equip_char_XML(struct char_data *ch, xmlNodePtr root);
/*
 * Data..
 */

#endif
