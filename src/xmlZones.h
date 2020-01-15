/******************************************************************************
 * $Id: xmlZones.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: xmlZones.h
 *
 * Created: Wed Mar 1 2000 by Stefan Lindstrom (asmodean@ludd.luth.se)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * Storing/loading of zones in XML format. 
 * 
 *   $Log: xmlZones.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:29  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/03/01 18:06:18  mud
 *   Skeleton code added for zone XML, as well as code for future percentage chance of zone cmds...
 *
 *   Revision 1.1  2000/03/01 15:54:32  mud
 *   First commit.
 *
 *
 *****************************************************************************/
#ifndef __XMLZONES_H__
#define __XMLZONES_H__

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

extern void load_xml_zone(char *f);
extern void save_xml_zone(FILE *f,int rnum);

extern ACMD(test_zon);

#endif

