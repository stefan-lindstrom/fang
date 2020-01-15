/******************************************************************************
 * $Id: xmlRooms.c,v 1.6 2003/06/17 10:30:05 fang Exp $
 *   
 * File: xmlRooms.c
 *
 * Created: Thu Feb 24 2000 by Stefan Lindstrom (asmodean@ludd.luth.se)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * Storing/parsing rooms in XML format. 
 * 
 *   $Log: xmlRooms.c,v 
 *   Revision 1.2  2002/07/28 13:56:05  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.6  2002/06/07 09:34:54  fang
 *   *** empty log message ***
 *
 *   Revision 1.5  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.4  2002/03/08 14:45:38  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/02/22 19:17:19  fang
 *   Fixed entity substitution up for properties.
 *
 *   Revision 1.2  2002/02/22 15:04:29  fang
 *   *** empty log message ***
 *
 *   Revision 1.15  2001/11/10 11:16:16  mud
 *   Temp fix for object status...
 *
 *   Revision 1.14  2001/09/24 17:08:10  mud
 *   *** empty log message ***
 *
 *   Revision 1.13  2001/05/22 09:07:08  mud
 *   DG-Scripts for Fang.
 *
 *   Revision 1.12  2001/01/31 15:44:54  mud
 *   *** empty log message ***
 *
 *   Revision 1.11  2001/01/23 12:09:45  mud
 *   *** empty log message ***
 *
 *   Revision 1.10  2000/05/01 14:23:43  mud
 *   &X colour codes are now both saved and parsed properly by both room and zone xml parser
 *
 *   Revision 1.9  2000/05/01 14:00:38  mud
 *   *** empty log message ***
 *
 *   Revision 1.8  2000/05/01 13:12:31  mud
 *   Fixed problem with top_of_world and top_of_zone_table for xml loading.
 *
 *   Revision 1.7  2000/03/07 12:45:02  mud
 *   Alas! The bug is squashed at last. Needed to have a valid doc ptr in every node, when adding an atribute that might contain an entity..
 *
 *   Revision 1.6  2000/03/02 12:54:52  mud
 *   Zones are now saved/loaded in XML-format.
 *
 *   Revision 1.5  2000/03/02 11:57:37  mud
 *   Parsing (but not inserting) of zones in XML format works..
 *
 *   Revision 1.4  2000/03/01 15:15:46  mud
 *   Named specials added, saved in room XML, and setable in redit
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

#include <stddef.h>
#include <fnmatch.h>
//#include <mlloc.h>

#include "xmlRooms.h"
#include "utils.h"
#include "handler.h"
#include "db.h"
#include "comm.h"
#include "constants.h"
#include "strutil.h"
#include "namedspec.h"
#include "dg_olc.h"
#include "olc.h"

/*
 * Reads and parses all rooms in file <f>
 * Ex:
 *
 * load_xml_rooms("./world/wld/326.wld");
 *
 * Do we still need this? *shrug*
 */
void load_xml_rooms(char *f)
{
  static void readRoomsFromXml(char *);

  readRoomsFromXml(f);
}

void room_save_zone_to_file(int rstart,int rstop, FILE *f)
{
  static void dumpRoomsToXmlFile(int start, int stop, FILE *file);

  // Attempt to dump this room-file as xml as well...
  dumpRoomsToXmlFile(rstart,rstop,f);

  return;
}

/*
 * Kinda unescessary wrapping, but since I use this func in a lot of places...
 */
char *internalXmlString(char *old, xmlDocPtr doc)
{
  return (xmlEncodeEntitiesReentrant(doc,old));
}

static xmlNodePtr makeRoomExDesc(struct extra_descr_data *exd,xmlDocPtr doc)
{
  char *p;
  xmlNodePtr desc = xmlNewNode(NULL,"exdesc");
  desc->doc = doc;

  xmlNewProp(desc,"keyword",exd->keyword);
  xmlNewChild(desc,NULL,"description",(p = internalXmlString(exd->description,doc)));
  free(p);

  return desc;
}

static xmlNodePtr makeRoomExit(struct room_direction_data *ex,int dir,xmlDocPtr doc)
{
  char buf[1024],*p;
  xmlNodePtr exit = xmlNewNode(NULL,"exit");
  exit->doc = doc;

  xmlNewProp(exit,"direction",dirs[dir]);
  xmlNewProp(exit,"keyword",ex->keyword);
  
  sprintf(buf,"%d",ex->exit_info);
  xmlNewProp(exit,"exitinfo",buf);

  sprintf(buf,"%d",ex->key);
  xmlNewProp(exit,"keynum",buf);

  sprintf(buf,"%d",world[ex->to_room].number);
  xmlNewProp(exit,"toroom",buf);

  xmlNewChild(exit,NULL,"description",(p = internalXmlString(ex->general_description,doc)));
  free(p);

  return exit;
}

static xmlNodePtr makeRoomBasic(struct room_data *rm,xmlDocPtr doc)
{
  char buf[10240],*p;
  xmlNodePtr basic = xmlNewNode(NULL,"roombasic");
  basic->doc = doc;

  xmlNewChild(basic,NULL,"roomdescription",(p = internalXmlString(rm->description,doc)));
  free(p);
  xmlNewProp(basic,"title",rm->name);

  //xmlNewProp(basic,"keyword",rm->keyword);
  // keyword support for rooms later on?
  
  sprintf(buf,"%d",rm->number);
  xmlNewProp(basic,"vnum",buf);

  sprintf(buf,"%d",zone_table[rm->zone].number);
  xmlNewProp(basic,"zone",buf);

  sprintf(buf,"%ld",rm->room_flags);
  xmlNewChild(basic,NULL,"flags",buf);

  xmlNewChild(basic,NULL,"sector",sector_types[rm->sector_type]);
  xmlNewChild(basic,NULL,"special",(NULL == getSpecialPointer(rm->func) ? "NONE" : getSpecialPointer(rm->func)));
  
  return basic;
}


/*
static xmlNodePtr makeJScriptXml(struct jscript *js)
{
}
*/

static xmlNodePtr roomToXml(struct room_data *r,xmlDocPtr doc)
{
  int i;
  struct extra_descr_data *e;
  struct trig_proto_list *trig;
 
  xmlNodePtr room  = xmlNewNode(NULL,"room");
  room->doc = doc;

  xmlAddChild(room,makeRoomBasic(r,doc));

  for (i = 0; i < NUM_OF_DIRS; i++) 
  {
    if( r->dir_option[i] && r->dir_option[i]->to_room > 0 )
      xmlAddChild(room,makeRoomExit(r->dir_option[i],i,doc));
  }

  if (NULL != (e = r->ex_description)) {
    while(e) {
      xmlAddChild(room,makeRoomExDesc(e,doc));
      e = e->next;
    }
  }

  /* DGS */
  trig = r->proto_script;
  while (trig)
  {
   xmlAddChild(room, makeDGScript(trig, doc));
   trig = trig->next;
  }

  return room;
}

static void dumpRoomsToXmlFile(int start, int stop, FILE *file)
{
  xmlNodePtr tree,room;
  xmlDocPtr doc;
  int i, rr;

  doc = xmlNewDoc("1.0");
  
  tree = xmlNewDocNode(doc,NULL,"rooms",NULL);
  doc->root = tree;

  for (i = start; i <= stop; i++) {
    if (-1 != (rr = real_room(i))) {
      room = roomToXml((world+rr),doc);
      xmlAddChild(tree,room);
    }
  }

  xmlDocDump(file,doc);
  xmlFreeDoc(doc);
}

static int string2sector(char *sec)
{
  int s = 0,i;

  for (i = 0; *sector_types[i] != '\n'; i++) {
    if (!strcasecmp(sector_types[i],sec))
      return i;
  }
  return s;
} 

static int string2direction(char *dir)
{
  int d = -1,i;

  for (i = 0; *dirs[i] != '\n'; i++) {
    if (!strcasecmp(dirs[i],dir)) {
      free(dir);
      return i;
    }
  }
  free(dir);
  return d;
}

char *xmlToString(xmlNodePtr p)
{
  xmlNodePtr temp;
  char *target;
  int size = 0;

  for (temp = p; NULL != temp; temp = temp->next) {
    size += strlen(temp->content);
  }
  size += 1;

  target = calloc(1,size);
  *target = '\0';

  for (temp = p; NULL != temp; temp = temp->next) {
    strcat(target,temp->content);
  }
  if (strlen(target) >= MAX_STRING_LENGTH) {
    free(target);
    return (strdup("STRING TO LONG!\r\n"));
  }
  // The gnome XML parser (according to a od -cx at least :))
  // preserves the \r\n sequence of our descriptions, hence, we
  // can just return our string here...
  
  return target;
}

static void fetchRoomBasics(xmlNodePtr basic, struct room_data *rm)
{
  xmlNodePtr temp;

  rm->name    = xmlGetProp(basic,"title");
  rm->number  = xmlAtoi(xmlGetProp(basic,"vnum"));
  rm->zone    = real_zone(rm->number);

  for (temp = basic->childs; temp != NULL; temp = temp->next) {
    if (!strcasecmp(temp->name,"flags"))
      rm->room_flags = atoi(temp->childs->content);

    else if (!strcasecmp(temp->name,"sector"))
      rm->sector_type = string2sector(temp->childs->content);

    else if (!strcasecmp(temp->name,"special")) {
      if (!strcasecmp(temp->childs->content,"NONE")) {
    rm->func = NULL; 
      }
      rm->func = getSpecialName(temp->childs->content);
    }

    /* Saved if Ders wanna add depth/height later on.
    else if (!strcasecmp(temp->name,"height"))
      rm->room_height = atoi(temp->childs->content);

    else if (!strcasecmp(temp->name,"depth"))
      rm->room_depth = atoi(temp->childs->content);
    */
    else if (!strcasecmp(temp->name,"roomdescription"))
      rm->description = xmlToString(temp->childs);

    else
      continue; // Some unknown type here...*shudder* :)
  }
}

static void fetchRoomExit(xmlNodePtr ex, struct room_data *r)
{
  struct room_direction_data *new;
  int dir;
  char *p;

  new = calloc(1,sizeof(struct room_direction_data));
  dir = string2direction((p = xmlGetProp(ex,"direction")));

  if (-1 == dir) {
    free(new);
    return;
  }
  new->keyword = xmlGetProp(ex,"keyword");
  new->exit_info = xmlAtoi(xmlGetProp(ex,"exitinfo"));
  new->key       = xmlAtoi(xmlGetProp(ex,"keynum"));
  new->to_room   = xmlAtoi(xmlGetProp(ex,"toroom"));
  new->general_description = (xmlToString(ex->childs->childs));
  r->dir_option[dir] = new;
}

static void fetchRoomExDesc(xmlNodePtr de,struct room_data *r)
{
  struct extra_descr_data *new;
  new = calloc(1,sizeof(struct extra_descr_data));
  
  new->keyword     = xmlGetProp(de,"keyword");
  new->description = xmlToString(de->childs->childs);

  new->next = r->ex_description;
  r->ex_description = new;
}

static struct room_data *fetchXmlRoom(xmlNodePtr room)
{
  xmlNodePtr temp;
  struct room_data *r = calloc(1,sizeof(struct room_data));
  int i;

  r->name = r->description = NULL;

  for (i = 0; i < NUM_OF_DIRS; i++) {
    r->dir_option[i] = NULL;
  }

  for (temp = room->childs; NULL != temp; temp = temp->next) {
    if (!strcasecmp(temp->name,"ROOMBASIC")) {
      fetchRoomBasics(temp,r);
    }
    else if (!strcasecmp(temp->name,"EXIT")) {
      fetchRoomExit(temp,r);
    }
    else if (!strcasecmp(temp->name,"EXDESC")) {
      fetchRoomExDesc(temp,r);
    }
    else if (!strcasecmp(temp->name,"DGSCRIPT"))
      fetchDGScript(temp, r, WLD_TRIGGER);
    else if (!strcasecmp(temp->name,"TEXT"))
     ; // Most likely a comment or something...
    else if (!strcasecmp(temp->name,"JSCRIPT"))
       ; // For future Java scripting capability. 
    else {
      alog("SYSERR: Node type %s not known by roomproto XML parser.",temp->name);
      continue;
    }
  }

  return r;
}

static void readRoomsFromXml(char *file)
{
  xmlNodePtr root,temp;
  xmlDocPtr  doc;
  struct room_data *rm;

  if (!(doc = xmlParseFile(file))) {
    alog("readRoomsFromXml:[roomproto.c]: Couldn't parse XML-file %s",file);
    return;
  }
  if (!(root = doc->root)) {
    alog("readRoomsFromXml:[roomproto.c]: No XML-root in document %s",file);
    xmlFreeDoc(doc);
    return;
  }
  if (strncasecmp("ROOMS",root->name,strlen(root->name))) {
    alog("readRoomsFromXml:[roomproto.c]: XML-root (%s in file %s) is not a room-object!",root->name,file);
    xmlFreeDoc(doc);
    return;
  }
  for (temp = root->childs; NULL != temp; temp = temp->next) {
    rm = fetchXmlRoom(temp);
    *(world+last_room_rnum) = *rm;
    /*
     * Someone forgot to update the top of world here *grin* This should
     * do it.. <Gavin | 1/5/2000>
     */
    top_of_world = last_room_rnum++;
    free(rm);
  }
  xmlFreeDoc(doc);
}

/*
 * Very naive entry count method. Just read file, line
 * after line, and coun how many times the </room> tag
 * occurs. :)
 */
int count_xml_roomentries(FILE *f) 
{
  char bf[25000],*u;
  int cnt = 0;

  while (fgets(bf,25000,f) && !feof(f)) {
    bf[(strlen(bf)-1)] = '\0';
    u = strupper(bf);

    if (NULL != strstr(u,"</ROOM>")) {
      cnt++;
    }

    free(u);
  }
  
  return cnt;
}
