/******************************************************************************
 * $Id: xmlObjects.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: xmlObjects.h
 *
 * Created: Thu Mar 23 2000 by Stefan Lindstrom (asmodean@ludd.luth.se)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: xmlObjects.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:29  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/03/28 09:44:07  mud
 *   *** empty log message ***
 *
 *   Revision 1.1  2000/03/23 14:30:45  mud
 *   First commit, nothing much in here, yet... :)
 *
 *
 *****************************************************************************/

#ifndef __XMLOBJECTS_H__
#define __XMLOBJECTS_H__

#include <stddef.h>
#include <errno.h>
#include <string.h>

#include <gnome-xml/parser.h>
#include <gnome-xml/tree.h>
#include <gnome-xml/entities.h>
#include <gnome-xml/parserInternals.h>
#include <stdio.h>

#include "structs.h"
#include "db.h"
#include "interpreter.h"          

extern void        load_xml_objects(char *f);
extern void        objects_save_zone_to_file(int,int,FILE *f);
extern int         count_xml_objectentries(FILE *f);                  

extern ACMD(test_xmlobj);

#endif
