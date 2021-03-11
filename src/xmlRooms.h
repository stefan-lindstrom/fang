/******************************************************************************
 * $Id: xmlRooms.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: xmlRooms.h
 *
 * Created: Thu Feb 24 2000 by Stefan Lindstrom (asmodean@ludd.luth.se)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * Saves/Parses rooms in XML. 
 * 
 *   $Log: xmlRooms.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:29  fang
 *   *** empty log message ***
 *
 *   Revision 1.6  2000/05/01 14:23:43  mud
 *   &X colour codes are now both saved and parsed properly by both room and zone xml parser
 *
 *   Revision 1.5  2000/03/02 12:54:52  mud
 *   Zones are now saved/loaded in XML-format.
 *
 *   Revision 1.4  2000/03/02 11:57:37  mud
 *   Parsing (but not inserting) of zones in XML format works..
 *
 *   Revision 1.3  2000/02/29 16:41:32  mud
 *   XML saving/parsing should actually work now! :)
 *
 *   Revision 1.2  2000/02/29 13:56:46  mud
 *   XML saving of rooms should now be in order.
 *
 *   Revision 1.1  2000/02/24 14:59:29  mud
 *   First commit...
 *
 *
 *****************************************************************************/

#ifndef __XMLROOM_H__
#define __XMLROOM_H__

#include <stddef.h>
#include <errno.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/entities.h>
#include <libxml/parserInternals.h>
#include <stdio.h>

#include "structs.h"
#include "interpreter.h"
#include "dll.h"

// --- FUNCTIONS and stuff ----------------------------------
extern void        load_xml_rooms(char *f);
extern void        room_save_zone_to_file(int,int,FILE *f);
extern int         count_xml_roomentries(FILE *f);
extern char       *xmlToString(xmlNodePtr p);
extern char       *internalXmlString(char *old,xmlDocPtr doc);
extern ACMD(testxml);

//-----------------------------------------------------------

#endif

