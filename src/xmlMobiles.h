/******************************************************************************
 * $Id: xmlMobiles.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: xmlMobiles.h
 *
 * Created: Sat Jun 10 2000 by stefan lindström (asmodean@ludd.luth.se)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * Saving/Loading mobiles in XML-format. 
 * 
 *   $Log: xmlMobiles.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.1  2000/06/10 17:48:24  mud
 *   First commit, not much here yet...
 *
 *
 *****************************************************************************/

#ifndef __XML_MOBILES_H__
#define __XML_MOBILES_H__

#include <stddef.h>
#include <errno.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/entities.h>
#include <libxml/parserInternals.h>
#include <stdio.h>

#include "structs.h"
#include "db.h"
#include "interpreter.h"          

extern void        load_xml_mobiles(char *f);
extern void        mobiles_save_zone_to_file(int,int,FILE *f);
extern int         count_xml_mobileentries(FILE *f);                  

extern ACMD(test_xmlmob);

#endif






