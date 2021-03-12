/******************************************************************************
 * $Id: xmlZones.c,v 1.3 2002/11/07 04:02:04 fang Exp $
 *   
 * File: xmlZones.c
 *
 * Created: Wed Mar 1 2000 by Stefan Lindstrom (asmodean@ludd.luth.se)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * For storing/loading zones in XML format. 
 * 
 *   $Log: xmlZones.c,v 
 *   Revision 1.2  2002/07/28 13:56:05  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.3  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.2  2002/02/22 15:04:29  fang
 *   *** empty log message ***
 *
 *   Revision 1.14  2001/11/10 11:16:16  mud
 *   Temp fix for object status...
 *
 *   Revision 1.13  2001/09/24 17:08:24  mud
 *   *** empty log message ***
 *
 *   Revision 1.12  2001/05/22 09:07:08  mud
 *   DG-Scripts for Fang.
 *
 *   Revision 1.11  2000/06/08 14:49:22  mud
 *   olc name lists saved/parsed in xmlZones. Names and all flag checked in OLC permissions.
 *
 *   Revision 1.10  2000/05/01 14:23:43  mud
 *   &X colour codes are now both saved and parsed properly by both room and zone xml parser
 *
 *   Revision 1.9  2000/05/01 14:00:38  mud
 *   *** empty log message ***
 *
 *   Revision 1.8  2000/05/01 13:12:32  mud
 *   Fixed problem with top_of_world and top_of_zone_table for xml loading.
 *
 *   Revision 1.7  2000/03/07 13:30:14  mud
 *   unused trace code deleted.
 *
 *   Revision 1.6  2000/03/02 12:54:52  mud
 *   Zones are now saved/loaded in XML-format.
 *
 *   Revision 1.5  2000/03/02 11:57:37  mud
 *   Parsing (but not inserting) of zones in XML format works..
 *
 *   Revision 1.4  2000/03/02 09:34:56  mud
 *   zone reset now honours the percentage load, 3 new commands added to zone cmds, but not implemented yet
 *
 *   Revision 1.3  2000/03/01 19:18:11  mud
 *   Support for 'percentage loading' added to zedit.
 *
 *   Revision 1.2  2000/03/01 18:06:18  mud
 *   Skeleton code added for zone XML, as well as code for future percentage chance of zone cmds...
 *
 *   Revision 1.1  2000/03/01 15:54:32  mud
 *   First commit.
 *
 *
 *****************************************************************************/
#include <stddef.h>
#include <errno.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/entities.h>
#include <libxml/parserInternals.h>
#include <stdio.h>

#include "xmlZones.h"
#include "db.h"
#include "olc.h"
#include "interpreter.h"
#include "utils.h"
#include "xmlRooms.h"
#include "dg_olc.h"

static char string2command(char *s) 
{
  if (!s) {
    return "*";
  }
  
  if (!strcasecmp(s,"ReadMobile")) {
    free(s);
    return 'M';
  }
  else if (!strcasecmp(s,"ReadObject")) {
    free(s);
    return 'O';
  }
  else if (!strcasecmp(s,"Obj2Mob")) {
    free(s);
    return 'G';
  }
  else if (!strcasecmp(s,"Obj2Obj")) {
    free(s);
    return 'P';
  }
  else if (!strcasecmp(s,"EquipMobWithObj")) {
    free(s);
    return 'E';
  }
  else if (!strcasecmp(s,"SetStateOfDoor")) {
    free(s);
    return 'D';
  }
  else if (!strcasecmp(s,"RemoveObj")) {
    free(s);
     return 'R';
  }
  else if (!strcasecmp(s,"PutMobOnObjSleep")) {
    free(s);
    return 'L';
  }
  else if (!strcasecmp(s,"PutMobOnObjSit")) {
    free(s);
    return 'T';
  }
  else if (!strcasecmp(s,"PutObjOnObj")) {
    free(s);
    return 'U';
  }
  else if (!strcasecmp(s,"AttachTrigger")) {
    free(s);
    return 'A';
  }
  else if (!strcasecmp(s,"AssignVariable")) {
    free(s);
    return 'V';
  }
  else if (!strcasecmp(s,"STOP")) {
    free(s);
    return 'S';
  }
  free(s);
  return '*'; // Means disabled... :>
}

static void fetchZCmds(xmlNodePtr p,struct zone_data *z) 
{
  int numcmds = xmlAtoi(xmlGetProp(p,"numberof")),i;
  struct reset_com *curr;
  xmlNodePtr cmd = p->children, rest;
  char *ptr;
  z->cmd = calloc(numcmds,sizeof(struct reset_com));

  for (i = 0; i < numcmds; i++) {
    curr = (z->cmd + i);
    curr->command = string2command(xmlGetProp(cmd,"command"));
    if (NULL != (ptr = xmlGetProp(cmd,"ifflag"))) {
      curr->if_flag = (!strcasecmp(ptr,"no") ? 0 : 1);
      free(ptr);
    }
    if (NULL != (ptr = xmlGetProp(cmd,"percentage"))) {
      curr->percentage = atoi(ptr);
      free(ptr);
    } else {
      curr->percentage = 100;
    }

    rest = cmd->children;
    for (; NULL != rest; rest = rest->next) {
      if (!strcasecmp(rest->name,"ARGS")) {
    curr->arg1 = xmlAtol(xmlGetProp(rest,"arg1"));
    curr->arg2 = xmlAtol(xmlGetProp(rest,"arg2"));
    curr->arg3 = xmlAtol(xmlGetProp(rest,"arg3"));
        if (curr->command == 'V')
        {
         curr->sarg1 = xmlGetProp(rest,"sarg1");
         curr->sarg2 = xmlGetProp(rest,"sarg2");
        }
      }
      else if (!strcasecmp(rest->name,"line")) {
	curr->line = xmlAtoi(xmlGetProp(rest,"lineno"));
      }
      else // Comment, unknown node type?*shrug*
    ;
    }
    cmd = cmd->next;
  }
}

static void fetchEditors(xmlNodePtr node, struct zone_data *z)
{
  xmlNodePtr temp = node->children;
  char *name;

  for (; NULL != temp; temp = temp->next) { 
    if (!strcasecmp(temp->name,"ALL")) { 
      z->all = 1;
      deleteAll(z->hfl_Editors,free);
      z->hfl_Editors = NULL;

      return;
    } else { 
      name = xmlGetProp(temp,"name");
      append(z->hfl_Editors,name);
    } 
  }   
}

void load_xml_zone(char *file)
{
  xmlNodePtr root,temp;
  xmlDocPtr  doc;
  struct zone_data *z;
  char *tmp = NULL;

  if (!(doc = xmlParseFile(file))) {
    alog("load_xml_zone [xmlZones.c]: Couldn't parse XML-file %s",file);
    return;
  }
  if (!(root = doc->children)) {
    alog("load_xml_zone:[xmlZones.c]: No XML-root in document %s",file);
    xmlFreeDoc(doc);
    return;
  }
  if (strncasecmp("ZONE",root->name,strlen(root->name))) {
    alog("load_xml_zone:[xmlZones.c]: XML-root (%s in file %s) is not a "
     "zone-object!",root->name,file);
    xmlFreeDoc(doc);
    return;
  }

  z = calloc(1,sizeof(struct zone_data));
  z->hfl_Editors = createDll();
  z->number     = xmlAtoi(xmlGetProp(root,"vnum"));
  z->lifespan   = xmlAtoi(xmlGetProp(root,"lifespan"));
  z->top        = xmlAtoi(xmlGetProp(root,"top"));
  z->reset_mode = xmlAtoi(xmlGetProp(root,"mode"));
  tmp = xmlGetProp(root, "lock");
  z->lock       = (!strcasecmp(tmp,"no") ? 0 : 1);
  if (tmp)
    free(tmp);
  // Do actual reading here...

  for (temp = root->children; NULL != temp; temp = temp->next) {
    if (!strcasecmp(temp->name,"NAME"))
      z->name = xmlToString(temp->children);
    else if (!strcasecmp(temp->name,"RESETCMDS"))
      fetchZCmds(temp,z);
    else if (!strcasecmp(temp->name,"EDITORS"))
      fetchEditors(temp,z);
    else
      ; // Unknown type here... Let's just skip it (might be comments)
  }
  xmlFreeDoc(doc);
  zone_table[last_zone_rnum] = *z;

  /*
   * Fixed update of top_of_zone_table here....otherwise xml zones
   * dissapear! <Gavin | 1/5/2000>
   */
  top_of_zone_table = last_zone_rnum++;

  free(z);
}

static xmlNodePtr makeStopCmd(void) {
  xmlNodePtr stop = xmlNewNode(NULL,"cmd");
  xmlNewProp(stop,"command","STOP");
  return stop;
}

static xmlNodePtr makeZoneCommand(struct reset_com *c,int number)
{
  static int offset = 7;
  int thisline = offset + (number * 4);
  xmlNodePtr cmd,arg,line;
  char buf[1024];

  cmd  = xmlNewNode(NULL,"cmd");
  switch (c->command) {
  case 'M':
    xmlNewProp(cmd,"command","ReadMobile");
    break;
  case 'O':
    xmlNewProp(cmd,"command","ReadObject");
    break;
  case 'G':
    xmlNewProp(cmd,"command","Obj2Mob");
    break;
  case 'P':
    xmlNewProp(cmd,"command","Obj2Obj");
    break;
  case 'E':
    xmlNewProp(cmd,"command","EquipMobWithObj");
    break;
  case 'D':
    xmlNewProp(cmd,"command","SetStateOfDoor");
    break;
  case 'R':
    xmlNewProp(cmd,"command","RemoveObj");
    break;
  case 'L':
    xmlNewProp(cmd,"command","PutMobOnObjSleep");
    break;
  case 'T':
    xmlNewProp(cmd,"command","PutMobOnObjSit");
    break;
  case 'U':
    xmlNewProp(cmd,"command","PutObjOnObj");
    break;
  case 'A':
    xmlNewProp(cmd,"command","AttachTrigger");
    break;
  case 'V':
    xmlNewProp(cmd,"command","AssignVariable");
    break;
  default:
    xmlNewProp(cmd,"command","UNKNOWN");
    break;
  }
  xmlNewProp(cmd,"ifflag",(c->if_flag ? "yes" : "no"));

  if (c->percentage < 100 && c->percentage > 0) {
    sprintf(buf,"%d",c->percentage);
    xmlNewProp(cmd,"percentage",buf);
  } 

  arg  = xmlNewNode(NULL,"args");
  if (c->arg1 == -1)
    xmlNewProp(arg,"arg1","NONE");
  else {
    sprintf(buf,"%ld",c->arg1);
    xmlNewProp(arg,"arg1",buf);
  }

  if (c->arg2 == -1)
    xmlNewProp(arg,"arg2","NONE");
  else {
    sprintf(buf,"%ld",c->arg2);
    xmlNewProp(arg,"arg2",buf);
  }

  if (c->arg3 == -1)
    xmlNewProp(arg,"arg3","NONE");
  else {
    sprintf(buf,"%ld",c->arg3);
    xmlNewProp(arg,"arg3",buf);
  }

  if (c->command == 'V')
  {
   xmlNewProp(arg, "sarg1", c->sarg1);
   xmlNewProp(arg, "sarg2", c->sarg2);
  }

  xmlAddChild(cmd,arg);
  
  line = xmlNewNode(NULL,"line");
  sprintf(buf,"%d",thisline);
  xmlNewProp(line,"lineno",buf);

  xmlAddChild(cmd,line);

  return cmd;
}

void save_xml_zone(FILE *f,int rzone)
{
  struct zone_data *z = (zone_table+rzone);
  struct reset_com *c;
  int cnt = 0; 
  xmlNodePtr tree,tmp;
  xmlDocPtr doc;
  char buf[1025],*p;
  //  char bf[25*4096];
  int i;

  if (NULL != (c = z->cmd)) {
    for (i = 0; c[i].command != 'S'; i++) {
      cnt++;
    }
  }
  doc = xmlNewDoc("1.0");

  tree = xmlNewDocNode(doc,NULL,"zone",NULL);
  doc->children = tree;
  
  sprintf(buf,"%d",z->number);
  xmlNewProp(tree,"vnum",buf);

  sprintf(buf,"%d",z->lifespan);
  xmlNewProp(tree,"lifespan",buf);
  //  xmlNewProp(tree,"start",);

  sprintf(buf,"%d",z->top);
  xmlNewProp(tree,"top",buf);

  sprintf(buf,"%d",z->reset_mode);
  xmlNewProp(tree,"mode",buf);

  xmlNewProp(tree,"lock",(z->lock ? "yes" : "no"));
  xmlNewChild(tree,NULL,"name",(p = internalXmlString(z->name,doc)));
  free(p);

  tmp = xmlNewNode(NULL,"resetcmds");
  sprintf(buf,"%d",(cnt+1)); // Count 'end' command?
  xmlNewProp(tmp,"numberof",buf);

  for (i = 0; i < cnt; i++) {
    c = (z->cmd + i);
    xmlAddChild(tmp,makeZoneCommand(c,i));
  }
  xmlAddChild(tmp,makeStopCmd());
  xmlAddChild(tree,tmp);

  if (NULL != z->hfl_Editors && 0 != getNumber(z->hfl_Editors)) {       
    xmlNodePtr tnam;
    tmp = xmlNewNode(NULL,"editors");
    first(z->hfl_Editors);
    do {  
      char *n = this(z->hfl_Editors);
      tnam = xmlNewNode(NULL,"editor");
      xmlNewProp(tnam,"name",n);
      xmlAddChild(tmp,tnam);
    } while(next(z->hfl_Editors));
    xmlAddChild(tree,tmp);
  }
  else if (z->all) { 
    tmp = xmlNewNode(NULL,"editors");
    xmlAddChild(tmp,xmlNewNode(NULL,"all"));
    xmlAddChild(tree,tmp);
  } 

  xmlDocDump(f,doc);
  xmlFreeDoc(doc);
}

ACMD(test_zon)
{
  FILE *f = fopen("/home/mud/stefans/cvs/fang1/testz.xml","w");
  save_xml_zone(f,real_zone(32600));
  fclose(f);

  load_xml_zone("/home/mud/stefans/cvs/fang1/testz.xml");
}


