/******************************************************************************
 * $Id: xmlobjsave.c,v 1.9 2003/06/10 12:16:09 fang Exp $
 *   
 * File: xmlobjsave.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: xmlobjsave.c,v $
 *   Revision 1.9  2003/06/10 12:16:09  fang
 *
 *   Added UNIQUE flag for mobs and objs. A unique obj/mob can only exists one
 *   instance at the time on the MUD, no matter what. A unique obj will not
 *   save to characters obj file either.
 *
 *   Revision 1.8  2003/01/27 17:06:31  fang
 *   All should work with exdescs and craft now.
 *
 *   Revision 1.7  2003/01/25 22:30:05  fang
 *   *** empty log message ***
 *
 *   Revision 1.6  2003/01/25 13:38:52  fang
 *   Re-enabled saving/loading of exdescs in tooled/crafted objects. Let's see if it can keep from crashing with this addition.
 *
 *   Revision 1.5  2002/11/07 04:02:04  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.4  2002/09/22 21:43:50  fischer
 *   Removing mounts and horsetrain from the code
 *
 *   Revision 1.3  2002/08/30 21:23:09  fang
 *   Put in retainers.
 *
 *   Revision 1.2  2002/07/28 13:56:05  fang
 *   Removed dmlloc...just to test.
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.6  2002/05/28 20:22:29  fang
 *   Maz: Small fix for Nose/Ear Stat Removal
 *
 *   Revision 1.5  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.4  2002/05/15 14:52:05  fang
 *   Maz: Added Nose/Ear Stat-Removal code for existing CRAFT EQ per Masema.  Also fixed two typos.
 *
 *   Revision 1.3  2002/02/27 02:17:41  fang
 *   Fixed disguise so it saves.
 *
 *   Revision 1.2  2002/02/22 15:04:29  fang
 *   *** empty log message ***
 *
 *   Revision 1.32  2002/01/24 21:33:21  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.31.2.1  2002/01/23 21:49:43  mud
 *   Removed objsave.[c|h] (old objsave stuff).
 *   Removed obj_file_elem and rent structs.
 *   Removed final vestiges of old guild system, except parts used
 *   in guild.[c|h] for backwards compatibility.
 *
 *   Revision 1.31  2002/01/15 19:17:26  mud
 *   Warning free...let's keep it that way...
 *
 *   Revision 1.30  2002/01/14 01:12:43  mud
 *   Started on fixing disappearing obj bug. I.E a \!SAVE container containing items previously threw all those items into binary heaven. Partly fixed.
 *
 *   Revision 1.29  2002/01/14 00:15:47  mud
 *   Score should no longer show negative amount of objs until you hit some 2*10^9 objs. :)
 *
 *   Revision 1.28  2001/11/10 11:27:35  mud
 *   *** empty log message ***
 *
 *   Revision 1.27  2001/11/10 11:16:16  mud
 *   Temp fix for object status...
 *
 *   Revision 1.26  2001/11/10 11:00:09  mud
 *   *** empty log message ***
 *
 *   Revision 1.25  2001/11/07 08:31:05  mud
 *   Obj status is now saved in objs, i.e No more instant repairs by logging out, and then on again
 *
 *   Revision 1.24  2001/09/24 17:08:10  mud
 *   *** empty log message ***
 *
 *   Revision 1.23  2001/07/09 19:17:03  mud
 *   *** empty log message ***
 *
 *   Revision 1.22  2001/05/17 11:21:09  mud
 *   Final residues of old pfile code should be swept away by now. No cleansing of the structures and such things though
 *
 *   Revision 1.21  2001/05/07 11:19:31  mud
 *   The set name bug has been eradicated
 *
 *   Revision 1.20  2001/03/22 12:52:37  mud
 *   When deleting chars obj-file, char file is deleted as well (delete, zap, etc)
 *
 *   Revision 1.19  2001/03/13 04:53:04  mud
 *   Fixed some typos
 *   !Save objects won't get saved (any !save containes will delete everything it contains)
 *
 *   Revision 1.18  2001/01/13 16:48:00  mud
 *   *** empty log message ***
 *
 *   Revision 1.17  2000/12/03 11:43:31  mud
 *   *** empty log message ***
 *
 *   Revision 1.16  2000/12/03 11:03:41  mud
 *   Some startup work for editable exdescs in craft/tool done
 *
 *   Revision 1.15  2000/12/03 10:26:10  mud
 *   More QEQ tracking, begun some minro mods to have exdescs editable by craft/tool.
 *
 *   Revision 1.14  2000/12/02 09:03:15  mud
 *
 *
 *   Added tracking info for qeq. If a questitem is purchased in shop, the name
 *   of the byer, the vnum of the shop (and the room  the shop was in) as well
 *   as the date of purchase is added to item. If item is gaine din any other
 *   way (loading, slaying shopkeepers, etc, etc) then this info will not be
 *   present. stat <object> will display this info, or a warning if none is
 *   present.
 *
 *   Revision 1.13  2000/09/04 03:34:40  mud
 *   Warnings removed...
 *
 *   Revision 1.12  2000/07/14 02:34:44  mud
 *   *** empty log message ***
 *
 *   Revision 1.11  2000/06/30 04:03:55  mud
 *   *** empty log message ***
 *
 *   Revision 1.10  2000/06/30 03:56:29  mud
 *   Craft fix.
 *
 *   Revision 1.9  2000/06/25 14:53:43  mud
 *   Gavin: Squished tooling and craft bugs, as well as changing the craft
 *   prices to adhere to current builder prices.
 *
 *   Revision 1.8  2000/04/15 23:18:22  mud
 *   A bunch of things....fix to club code -addition- of call to save_char
 *   in ACMD(do_save) *whap Fred* and that's it methinks.
 *
 *   Revision 1.7  2000/04/13 18:05:28  mud
 *   Added a check in obj_to_XML for rnum of -1 (.e. minor bug if gold in
 *   corpse a player picks up say)
 *
 *   Revision 1.6  2000/04/11 21:20:57  mud
 *   Plugged a dozen blatant memory leaks.
 *
 *   Revision 1.5  2000/04/11 15:47:07  mud
 *   Added the illusion talent and fixed xmlobjsave! (The bug with zero
 *   length descs/names/etc for tooled objects)
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

/*************************************************************************
*   File: xmlobjsave.c                               Part of Dragons Fang *
*  Usage: loading/saving player objects for rent and crash-save           *
************************************************************************ */

#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "handler.h"
#include "db.h"
#include "interpreter.h"
#include "utils.h"
#include "xmlobjsave.h"
#include "class.h"

/*
 * This function is a replacement for the Crash_delete(), since the old one was
 * unnecessarily complex and had things we didn't need.
 * 
 * Last edited: 000212 by Fredrik
 * Arguments: Full name of file to be removed ("lib/plrobjs/A-E/Agne.objs" etc)
 * Returns: -
 */
void delete_playerfile(char *name)
{
  int i = 0;
  char fname[100];
   
  for (i = 0; i < strlen(name); i++)
    if (isupper(name[i]))
      name[i] = tolower(name[i]);
  
  if (get_filename(name, fname, CRASH_FILE)) {
    unlink(fname);
  }  
  if (get_filename(name, fname, DREAM_FILE)) {
    unlink(fname);
  }

  if (get_filename(name,fname,PLR_FILE)) {
    unlink(fname);
  }
     
  if (get_filename(name,fname,ALIAS_FILE)) {
    unlink(fname);
  }
}

void extract_objs(struct obj_data * obj)
{
  if (obj) {
    extract_objs(obj->contains);   
    extract_objs(obj->next_content);
    extract_obj(obj);
  }
}

/*
 * Removes everything the character wears and carries.
 * 
 * Argument: Character to remove all objs from.
 * Last edited: 990409 by Fredrik
 */
void strip_char_of_all_eq(struct char_data *ch)
{
  int i;

  if (!ch)
    return;
  extract_objs(ch->carrying);
  for (i = 0; i < NUM_WEARS; i++)
    if (GET_EQ(ch,i)) 
      extract_objs(unequip_char(ch,i));
}

/*
 * Takes an obj and creates an xml node struct, to be added to an xml document.
 * If the obj is a container and contains objs, they are also included into
 * the structure, recursively.
 * Note: You can't save this directly to file, you must have it in an xmlDocPtr
 * structure.
 * 
 * Last edited: 000212 by Fredrik
 * Arguments: Obj to 'xmlify' and xml document to contain the new xml objs
 * Returns: pointer to xml structure reflecting obj, or null if obj was null
 */
xmlNodePtr obj_to_XML(struct obj_data *obj, xmlDocPtr doc)
{
  xmlNodePtr xobj,temp;
  struct obj_data *content;
  int tooled = 0, i = 0, j = 0;
  char *tb;
  struct extra_descr_data *desc; 

  if (!obj)
    return NULL;
  
  xobj = xmlNewNode(NULL, "obj");
  xobj->doc = doc;
  sprintf(buf, "%ld", GET_OBJ_VNUM(obj));
  xmlNewProp(xobj, "vnum", buf);

  sprintf(buf,"%d",GET_OBJ_STATUS(obj));
  xmlNewProp(xobj,"status",buf);

  sprintf(buf, "%d", GET_OBJ_TYPE(obj));
  xmlNewProp(xobj, "type", buf);

  if (obj->worn_by)
  {
    sprintf(buf, "%d", obj->worn_on);
    xmlNewProp(xobj, "worn", buf);
  }
  if ((i = real_object(GET_OBJ_VNUM(obj))) < 0)
    return NULL;
  if ((obj->name && !obj_proto[i].name) ||
     (obj_proto[i].name && !obj->name) ||
     (obj->description && !obj_proto[i].description) ||
     (obj_proto[i].description && !obj->description) ||
     (obj->short_description && !obj_proto[i].short_description) ||
     (obj_proto[i].short_description && !obj->short_description) ||
     (obj->action_description && !obj_proto[i].action_description) ||
     (obj_proto[i].action_description && !obj->action_description))
    tooled = TRUE;
  else if (strcasecmp(obj->name, obj_proto[i].name) ||
      strcasecmp(obj->description, obj_proto[i].description) ||
      (GET_OBJ_TYPE(obj) == ITEM_NOTE) ||
      strcasecmp(obj->short_description, obj_proto[i].short_description))
    tooled = TRUE;

  if (IS_SET(OBJ_EXTRA_FLAGS(obj), ITEM_ENHANCED))
    tooled = TRUE;

  /*
   * TOOL, sets exdesc to NULL, if obj hasn't exdesc, but objproto has, or objproto and obj has different 
   * ex_descs ptr (since these are shared, not copied) the object is tooled...
   */
  if ((!obj->ex_description && obj_proto[i].ex_description) || (obj->ex_description != obj_proto[i].ex_description))
    tooled = TRUE;

  if (tooled)
  {
    if (obj->short_description && *(obj->short_description) && strcasecmp(obj->short_description, (obj_proto[i].short_description ? obj_proto[i].short_description : "")))
    {
      tb =  xmlEncodeEntitiesReentrant(doc, obj->short_description);
      xmlNewChild(xobj, NULL, "name", tb);
      free(tb); 
    }
    if (obj->name && *(obj->name) && strcasecmp(obj->name, (obj_proto[i].name ? obj_proto[i].name : "")))
    {
      tb = xmlEncodeEntitiesReentrant(doc, obj->name);
      xmlNewChild(xobj, NULL, "keywords", tb);
      free(tb);
    }
    if (obj->description && *(obj->description) && strcasecmp(obj->description, (obj_proto[i].description ? obj_proto[i].description : "" )))
    {
       tb = xmlEncodeEntitiesReentrant(doc, obj->description);
       xmlNewChild(xobj, NULL, "desc", tb);
       free(tb);
    }
    if (obj->action_description && *(obj->action_description) && strcasecmp(obj->action_description, (obj_proto[i].action_description ? obj_proto[i].action_description : "")))
    {
      tb = xmlEncodeEntitiesReentrant(doc, obj->action_description);
      xmlNewChild(xobj, NULL, "adesc", tb);
      free(tb);
    }

    if ((desc = obj->ex_description)) // test object 23010!, or other with many desc!
    {
      while (desc && desc->keyword && *(desc->keyword) && desc->description && *(desc->description))
      {
        tb = xmlEncodeEntitiesReentrant(doc, desc->description);
        temp = xmlNewChild(xobj, NULL, "extradesc", tb);
        free(tb);
        tb = xmlEncodeEntitiesReentrant(doc, desc->keyword);
        xmlNewProp(temp, "keyword", tb);
        free(tb);    
        desc = desc->next;
      }
    }
    sprintf(buf, "%ld %ld %ld %ld", GET_OBJ_VAL(obj,0), GET_OBJ_VAL(obj, 1),
            GET_OBJ_VAL(obj, 2), GET_OBJ_VAL(obj, 3));
    xmlNewChild(xobj, NULL, "values", buf);
   
    if (GET_OBJ_EXTRA(obj) != GET_OBJ_EXTRA(&obj_proto[i]))
    {
      sprintf(buf, "%ld", GET_OBJ_EXTRA(obj));
      xmlNewChild(xobj, NULL, "extraflags", buf);
    }

    if (GET_OBJ_WEAR(obj) != GET_OBJ_WEAR(&obj_proto[i])) {
      sprintf(buf, "%ld", GET_OBJ_WEAR(obj));
      xmlNewChild(xobj, NULL, "wearflags", buf);
    }

    if (GET_OBJ_COST(obj) != GET_OBJ_COST(&obj_proto[i]))
    {
      sprintf(buf, "%ld", GET_OBJ_COST(obj));
      xmlNewChild(xobj, NULL, "worth", buf);
    }

    for (j = 0; j < MAX_OBJ_AFFECT; j++)
      if ((obj->affected[j].location) && !(obj_proto[i].affected[j].location)) {
    sprintf(buf, "%d %ld", (obj->affected[j].location), (obj->affected[j].modifier));
    xmlNewChild(xobj, NULL, "aff", buf);
      }
  } // tooled

  if (obj->obj_flags.boughtBy) {
    xmlNodePtr q = xmlNewChild(xobj,NULL,"qeqdata",obj->obj_flags.boughtBy);
    q->doc = doc;
    sprintf(buf,"%ld",obj->obj_flags.boughtFrom[0]);
    xmlNewProp(q,"shopVnum",buf);
    sprintf(buf,"%ld",obj->obj_flags.boughtFrom[1]);
    xmlNewProp(q,"shopRoomVnum",buf);
    sprintf(buf,"%ld",obj->obj_flags.boughtWhen);
    xmlNewProp(q,"byingTime",buf);
  }
  

  for (content = obj->contains; content; content = content->next_content)
  {
    if (IS_SET(OBJ_EXTRA_FLAGS(content), ITEM_NOSAVE) || IS_SET(OBJ_EXTRA_FLAGS(content), ITEM_UNIQUE)) {
      if (content->contains)
    xmlAddChild(xobj,obj_to_XML(content->contains,doc));
      continue;
    }
    xmlAddChild(xobj, obj_to_XML(content, doc));
  }
  return xobj;
}


/*
 * Parses through an xml node structure and creates objs from it. 
 * The name of the function is a bit erroneous, since it also creates all the eventual
 * objs contained within the obj, recursively.
 * See head of this file for accepted obj node format.
 *
 * Last edited: 000212 by Fredrik
 * Arguments: XML node to be parsed through
 * Returns: obj pointer, or null if the xml node was of wrong format or simply not
 *          containing an obj.
 */
struct obj_data *get_one_obj_XML(xmlNodePtr xobj)
{
  xmlNodePtr temp;
  struct obj_data *obj, *contained;
  int vnum = 0, i = 0, tooled = 0;
  //  char *tmp = NULL;

  if (!xobj || strcasecmp(xobj->name, "obj")) // <obj>
  {
    return NULL;
  }
  vnum = xmlAtoi(xmlGetProp(xobj, "vnum")); // <obj vnum="">
  if (real_object(vnum) > -1) 
  {
    obj = read_object(vnum, VIRTUAL);
  }
  else
  {
    return NULL;
  }

  GET_OBJ_STATUS(obj) = xmlAtoi(xmlGetProp(xobj, "status"));

  if ((GET_OBJ_TYPE(obj) = xmlAtoi(xmlGetProp(xobj, "type"))) == -1)
    GET_OBJ_TYPE(obj) = 0;

  obj->obj_flags.boughtBy = NULL;
  obj->obj_flags.boughtFrom[0] = obj->obj_flags.boughtFrom[1] = -1;
  obj->obj_flags.boughtWhen = 0; 



  for (temp  = xobj->childs; temp; temp = temp->next)
  {
    if (!strcasecmp("name", temp->name)) // <name>
    {
      obj->short_description = strdup(temp->childs->content);
      tooled = 1;
    }
    else if (!strcasecmp("keywords", temp->name)) // <keywords>
    {
      obj->name = strdup(temp->childs->content);
      tooled = 1;
    }
    else if (!strcasecmp("desc", temp->name)) // <desc>
    {
      obj->description = strdup(temp->childs->content);
      tooled = 1;
    }
    else if (!strcasecmp("adesc", temp->name)) // <adesc>
    {
      obj->action_description = strdup(temp->childs->content);
      tooled = 1;
    }
    else if (!strcasecmp("values", temp->name)) // <values>
    {
      sscanf(temp->childs->content, "%ld %ld %ld %ld", &GET_OBJ_VAL(obj,0),    
       &GET_OBJ_VAL(obj, 1), &GET_OBJ_VAL(obj, 2), &GET_OBJ_VAL(obj, 3));
      tooled = 1;
    }   
    else if (!strcasecmp("worth", temp->name)) // <worth>
    {
      GET_OBJ_COST(obj) = atoi(temp->childs->content);
      tooled = 1;
    }
    else if (!strcasecmp("extraflags", temp->name)) // <extraflags>
    {
      tooled = 1;
      GET_OBJ_EXTRA(obj) = atol(temp->childs->content);
    }
    else if (!strcasecmp("wearflags", temp->name)) // <wearflags>
    {
      GET_OBJ_WEAR(obj) = atol(temp->childs->content);
      tooled = 1;
    }
    else if (!strcasecmp("aff", temp->name))
    {
      for (i = 0; i < MAX_OBJ_AFFECT; i++)
        if (!obj->affected[i].location) {
          int tmp;
          sscanf(temp->childs->content, "%d %ld", &tmp, &(obj->affected[i].modifier));
          obj->affected[i].location = (byte)tmp;
          break;      
        }
   
      tooled = 1;
    }
    else if (!strcasecmp("qeqdata",temp->name)) {
      obj->obj_flags.boughtBy = strdup(temp->childs->content);
      obj->obj_flags.boughtFrom[0] = xmlAtol(xmlGetProp(temp,"shopVnum"));
      obj->obj_flags.boughtFrom[1] = xmlAtol(xmlGetProp(temp,"shopRoomVnum"));
      obj->obj_flags.boughtWhen    = xmlAtol(xmlGetProp(temp,"byingTime"));   
    }
    else if (!strcasecmp("obj", temp->name)) {  /* <obj> (contained) */
      if ((contained = get_one_obj_XML(temp))) {
        obj_to_obj(contained, obj);
      }
    }
  }

  /* second pass */
  if (tooled) {
    obj->ex_description = NULL;

    for (temp  = xobj->childs; temp; temp = temp->next) {

      if (!strcasecmp("extradesc",temp->name)) {
        struct extra_descr_data *ex = calloc(1,sizeof(struct extra_descr_data));
        ex->description = strdup(temp->childs->content);
        ex->keyword     = strdup(xmlGetProp(temp,"keyword"));

        ex->next            = obj->ex_description;
        obj->ex_description = ex;
      }
    }
  }

  return obj;
}


/*
 * Takes an xml node and parses through it, equipping the character with the objs parsed.
 * It also sets the characters money data, if such nodes are present.
 * It silently ignores any faulty -child- nodes.
 * 
 * This fails if:
 * - ch is null
 * - xml node:
 *   - is null
 *   - isn't a <playerobjs> node
 *   - doesn't contain <obj> nodes 
 *   - contains only faulty <obj> nodes.
 *
 * Last edited: 000212 by Fredrik
 * Arguments: XML node to be parsed through and character to equip
 * Returns: -
 */
void equip_char_XML(struct char_data *ch, xmlNodePtr root)
{
  xmlNodePtr temp;
  struct obj_data *obj;
  char *name;
  int where, i;

  if (!ch)
  {
    alog("SYSERR: (equip_char_XML) Attempt to equip NULL character.");
    return;
  }
  if (!root)
  {
    alog("SYSERR: (equip_char_XML) Attempt to equip %s with NULL xml node.", GET_RNAME(ch));
    return;
  }
  if (strcasecmp(root->name, "playerobjs"))
  {
    alog("SYSERR: (equip_char_XML) Attempt to equip %s with an <%s> xml node when it should be <playerobjs>", GET_RNAME(ch), root->name);
    return;
  }
  for (temp = root->childs; temp; temp = temp->next)
  {
    if (!temp->name || !*temp->name)
    {
      continue;
    }
    if (!strcasecmp(temp->name, "obj"))
    {
      if (( obj = get_one_obj_XML(temp)))
      {
        if ((name = xmlGetProp(temp, "worn"))) // <obj worn="">
        {
          where = atoi(name);
      free(name);
          if (GET_OBJ_VNUM(obj) == 2) {
            if (CAN_WEAR(obj,ITEM_WEAR_EAR) || CAN_WEAR(obj,ITEM_WEAR_NOSE)) {
              if (GET_OBJ_TYPE(obj) != ITEM_OTHER) {
                GET_OBJ_TYPE(obj) = ITEM_OTHER;
                GET_OBJ_VAL(obj, 0) = 0;  /* To avoid confusions, clear values  */
                GET_OBJ_VAL(obj, 1) = 0;
                GET_OBJ_VAL(obj, 2) = 0;
                GET_OBJ_VAL(obj, 3) = 0;
              }
              for (i = 0; i < MAX_OBJ_AFFECT; i++) {
                obj->affected[i].location = 0;
                obj->affected[i].modifier = 0;
              }
            }
          }
          equip_char(ch, obj, where); 
        }
        else
        {
          if (GET_OBJ_VNUM(obj) == 2) {
            if (CAN_WEAR(obj,ITEM_WEAR_EAR) || CAN_WEAR(obj,ITEM_WEAR_NOSE)) {
              if (GET_OBJ_TYPE(obj) != ITEM_OTHER) {
                GET_OBJ_TYPE(obj) = ITEM_OTHER;
                GET_OBJ_VAL(obj, 0) = 0;  /* To avoid confusions, clear values  */
                GET_OBJ_VAL(obj, 1) = 0;
                GET_OBJ_VAL(obj, 2) = 0;
                GET_OBJ_VAL(obj, 3) = 0;
              }
              for (i = 0; i < MAX_OBJ_AFFECT; i++) {
                obj->affected[i].location = 0;
                obj->affected[i].modifier = 0;
              }
            }
          }
          obj_to_char(obj, ch);
        }
      }
    }
    else if (xmlNodeIsText(temp) && !strcasecmp(temp->name, "coins")) // <coins>
    {
      sscanf(temp->childs->content, "%ld %ld", &GET_GOLD(ch), &GET_BANK_GOLD(ch));
    }
  }
}

/*
 * Reads the players objfile (xml format) and equips the player with the objs found in it.
 * It also sets the characters money, if such xml tags are found.
 * This fails if:
 * - ch is null
 * - ch is a mob
 * - ch has no name
 * - playerfilename for ch couldn't be resolved
 * - no xml file for ch was found
 * - parsing of xml file failed
 *
 * Last edited: 000212 by Fredrik
 * Arguments: Character to equip
 * Returns: -
 */
void read_XML_playerfile(struct char_data *ch, int mode)
{
  xmlDocPtr doc;
  xmlNodePtr root;
  char fname[100];
  FILE *fl;

  if (!ch || !GET_RNAME(ch))
  {
    return;
  }
  if (!get_filename(GET_RNAME(ch), fname, mode))
  {
    return;
  }
  if (!(fl = fopen(fname, "r")))
  {
    return;
  }
  fclose(fl);

  if (!(doc = xmlParseFile(fname)))
  {
    send_to_char("Ack! Your saved objs seem to be corrupt! Please tell an imm.\r\n", ch);
    return;
  }

  if (!(root = doc->root))
  {
    xmlFreeDoc(doc);
    return;
  }
  if (strncasecmp("playerobjs", root->name, strlen(root->name)))
  {
    xmlFreeDoc(doc);
    return;
  }
  equip_char_XML(ch, root);
  xmlFreeDoc(doc);
}


void read_XML_roomobjs(int vnum)
{
  xmlDocPtr doc;
  xmlNodePtr root, temp;
  char fname[100];
  struct obj_data *obj;
  FILE *fl;
  int rnum;

  if ((rnum = real_room(vnum)) < 0)
  {
    alog("SYSERR: Attempt to read objs into nonexisting room %d. Remove that room vnum from rooms_that_save_objs in class.c.", vnum);
    return;
  }
  sprintf(fname, "plrobjs/A-E/%d.objs", vnum);

  if (!(fl = fopen(fname, "r")))
  {
    return;
  }
  fclose(fl);
  if (!(doc = xmlParseFile(fname)))
  {
    return;
  }
  if (!(root = doc->root))
  {
    xmlFreeDoc(doc);
    return;
  }
  if (strncasecmp("roomobjs", root->name, strlen(root->name)))
  {
    xmlFreeDoc(doc);
    return;
  }
  for (temp = root->childs; temp; temp = temp->next)
  {
    if (!temp->name || !*temp->name)
    {
      continue;
    }
    if (!strcasecmp(temp->name, "obj"))
    {
      if (( obj = get_one_obj_XML(temp)))
      {
        obj_to_room(obj, rnum);
      }
    }
  }
  xmlFreeDoc(doc);
}

 /*
 * This method saves the objects that are in certains rooms defined in
 * class.c to file, and if the game crashes, they are loaded back in
 * again.
 */
void save_objs_in_rooms()
{
  xmlDocPtr doc;
  xmlNodePtr tree;
  int i = 0;
  struct room_data this_room;
  struct obj_data *this_obj = NULL;   
  FILE *fl;
  char filename[100];

  doc = xmlNewDoc("1.0");
  tree = xmlNewDocNode(doc, NULL, "roomobjs",NULL);
  doc->root = tree;
        
  for (i = 0; rooms_that_save_objs[i] > 0 ; i++) {
    if (real_room(rooms_that_save_objs[i]) == NOWHERE)
      continue;
    this_room = world[real_room(rooms_that_save_objs[i])];
    sprintf(filename, "plrobjs/A-E/%d.objs", this_room.number);
    if (!(fl = fopen(filename, "w"))) {
      alog("Error: (xmlobjsave.c, save_objs_in_room()) Couldn't open file %s for writing!", filename);
      continue;
    }
    for (this_obj = this_room.contents; this_obj; this_obj = this_obj->next_content) {
      if (GET_OBJ_RNUM(this_obj) <= 0)
        continue;
      if (GET_OBJ_VNUM(this_obj) <= 0)
        continue;
      if (IS_SET(OBJ_EXTRA_FLAGS(this_obj), ITEM_NOSAVE) || IS_SET(OBJ_EXTRA_FLAGS(this_obj), ITEM_UNIQUE))
        continue;
      xmlAddChild(tree, obj_to_XML(this_obj, doc));
    }
    xmlDocDump(fl, doc);
    xmlFreeDoc(doc);
    fclose(fl);
  }
}


/*
 * Saves the objs of all the players currently logged on.
 * Last edited: 000409 by Fredrik
 */
void save_all_xml()
{
}

/*
 * Returns a complete tree reflecting ch's all eq EXCEPT MOUNTS,
 * as children under the given node. 
 */
void eq_to_xml(struct char_data *ch, xmlNodePtr tree, xmlDocPtr doc)
{
  struct obj_data *obj;
  xmlNodePtr temp;
  int i;

  for (obj = ch->carrying; obj; obj = obj->next_content)
  {
    if (IS_SET(OBJ_EXTRA_FLAGS(obj), ITEM_NOSAVE) || IS_SET(OBJ_EXTRA_FLAGS(obj), ITEM_UNIQUE)) {
      if (obj->contains)
        xmlAddChild(tree,obj_to_XML(obj->contains,doc));
      continue;
    }
    temp = xmlAddChild(tree, obj_to_XML(obj, doc));
  }

  for (i = 0; i < NUM_WEARS; i++)
  {
    if (GET_EQ(ch, i))
    {
      if (!IS_SET(OBJ_EXTRA_FLAGS(GET_EQ(ch, i)), ITEM_NOSAVE) || IS_SET(OBJ_EXTRA_FLAGS(GET_EQ(ch,i)), ITEM_UNIQUE))
        xmlAddChild(tree, obj_to_XML(GET_EQ(ch, i), doc));
      else if (GET_EQ(ch,i)->contains) 
      {
        xmlAddChild(tree,obj_to_XML(GET_EQ(ch, i)->contains, doc));
      }
    }
  }
}

/*
 * Saves a characters objs and money to xml file.
 * This fails if:
 * - ch is a mob
 * - ch has no name
 * - playerfilename for ch couldn't be resolved
 *
 * Last edited: 020822 by Fredrik
 * Arguments: Character to save and mode (CRASH, DREAM, etc)
 * Returns: -
 */
void save_XML_playerfile(struct char_data *ch, int mode)
{
  char fname[100];
  xmlDocPtr doc;
  xmlNodePtr tree;
  xmlNodePtr temp;
  FILE *fl;

  if (!ch || IS_NPC(ch))
  {
    return;
  }
  if (!get_filename(GET_RNAME(ch), fname, mode))
  {
    asend_to_char(ch, "Couldn't get your savefilename, reason: %s.\r\n", strerror(errno));
    return;
  }

  doc = xmlNewDoc("1.0");
  tree = xmlNewDocNode(doc, NULL, "playerobjs",NULL);
  doc->root = tree;

  sprintf(buf, "%ld %ld",  GET_GOLD(ch), GET_BANK_GOLD(ch));
  temp = xmlNewChild(tree, NULL, "coins", buf);
  eq_to_xml(ch, tree, doc);

  if (!(fl = fopen(fname, "w")))
  {
    asend_to_char(ch, "Damn, failed to open file %s, reason: %s.\r\n", fname, strerror(errno));   
    xmlFreeDoc(doc);
    return;
  }
  xmlDocDump(fl, doc);
  xmlFreeDoc(doc);
  fclose(fl);
  save_char(ch, IN_ROOM(ch));
}

/*
 * Returns an alloced array of pointers to all the obj nodes in the
 * node given as an argument. It also sets the number given as argument
 * to reflect how many pointers are in that array.
 * This does a recursive search.
 *
 * Last edited: 000212 by Fredrik
 * Params: Node to search through and number to set
 * Returns: An array of pointers to all objs in that node, recursively, or null
 *          if no objs are in that node.
 */
xmlNodePtr  *all_objs_in_XML_node(xmlNodePtr xobj, int *num)
{
  xmlNodePtr *all = NULL, *part = NULL, tmp = NULL;
  int subnum = 0, sizeOfArray = 0, i = 0;

  if (!xobj)
  {
    *num = 0;
    return NULL;
  }

  sizeOfArray = 10; // to begin with, will maybe grow later on.
  CREATE( all, xmlNodePtr, sizeOfArray );

  *num = 0;
  for (tmp = xobj->childs; tmp; tmp = tmp->next) // Go over the contents
  {
    if (!strcasecmp(tmp->name, "obj")) // An obj here.
    {
      all[(*num)] = tmp;
      (*num)++;
      if ((*num) < 0 || (*num) == sizeOfArray -1)
        alog("Warning! num is %d!", *num);

      if ((*num) >= sizeOfArray-1)
      {
        sizeOfArray *= 2; // increase size in increments of 100%
        all = realloc(all, sizeOfArray * sizeof(xmlNodePtr)); // resize
      }
      part = all_objs_in_XML_node(tmp, &subnum); // find all the objs in this one.
      if (part && subnum)               // There -were- objs in this one.
      {
        while (sizeOfArray -2 <= (subnum +(*num))) // New bunch to add will exceed array
        {
          sizeOfArray *= 2; // increase size in increments of 100%
          all = realloc(all, sizeOfArray * sizeof(xmlNodePtr)); // resize
        }
        for (i = 0; i < subnum; i ++) // Time to add the new pointers
        {
          all[(*num)] = part[i];
          (*num) ++;
        } 
        for (i = *num; i < sizeOfArray; i++) // tidy up the rest of the array.
        {
          all[i] = NULL;
        }
        free(part); // "part" was allocated.
      }
    }
  }

  if (!*num) // no objs found
  {
    free(all); // no use having it still
    return NULL;
  }
  return all;
}

/*
 * Takes an obj node xml pointer and searches through it, recursively, for
 * objs and returns the pointer to a string containing all the objs,
 * listed in a flat array.
 * Note that this doesn't look for crafted objs or tooled objs; all objs with
 * the same vnum is listed as the same.
 * Remember to free the returned pointer.
 *
 * Last edited: 000212 by Fredrik
 * Params: node that contains objs
 * Returns: A pointer to a string containing the listing desired, never null.
 */
char *list_obj_XML_contents(xmlNodePtr root)
{
  xmlNodePtr *objs = 0;
  int vnum = 0, num = 0, i = 0, j = 0, *allvnums = 0, *howmany = NULL, found;
  char *lbuf = calloc(MAX_STRING_LENGTH, sizeof(char));

  if (!root || strncasecmp("playerobjs", root->name, strlen(root->name)))
  {
    sprintf(lbuf, "Wrong format in xml node, cannot execute");
    return lbuf;
  }
  objs = all_objs_in_XML_node(root, &num);
  if (objs && num)
  {
    allvnums = (int *) calloc(num, sizeof(int));
    howmany = (int *) calloc(num, sizeof(int));
    for (i = 0; i < num; i++)
    {
      allvnums[i] = 0;
      howmany[i] = 0;
    }   
    for (i = 0; i < num; i ++)
    {
      found = 0;
      vnum = xmlAtoi(xmlGetProp(objs[i], "vnum"));
      for (j = 0; allvnums[j] && !found && (j < num); j ++)
      {
        if (allvnums[j] == vnum)  // this vnum is already added to list
        {
          found = 1;
          howmany[j]++;      
        }
      }
      if (!found)
      {
        howmany[j] = 1;
        allvnums[j] = vnum;  // new vnum to add to list
      }
    }
    *lbuf = 0;
    for (i = 0; allvnums[i] && (i < num); i++)
    {
      j = real_object(allvnums[i]);
      if (strlen(lbuf) > MAX_STRING_LENGTH - 100)
      {
        sprintf(lbuf, "%s<<<<<<<< OVERFLOW >>>>>>>>\r\n", lbuf);
        break;
      }
      sprintf(lbuf, "%s[\\c03%3d\\c00] (\\c02#%-5d\\c00) %s\r\n", lbuf, howmany[i], 
      allvnums[i],  (j <= 0)?"Something odd.":obj_proto[j].short_description);
    }
    sprintf(lbuf, "%sAll in all \\c02%d\\c00 objects.\r\n", lbuf,  num);
    free(objs);
    free(allvnums);
    free(howmany);
    return lbuf;
  }
  sprintf(lbuf, "...nothing!\r\n");
  return lbuf;
}

/*
 * Lists the objects the named player has in his objfile currently.
 */

ACMD(do_listobj)
{
  xmlDocPtr doc;
  xmlNodePtr root;
  char fname[100], *name = NULL, *result;
  int id = 0, found = 0;

  if (!argument || !*argument)
  {
    send_to_char("Syntax: listobj <playername>\r\n", ch);
    return;
  }  
  skip_spaces(&argument);

  for(id = 0; id <= top_of_p_table && !found; id++) 
  {
    if (!strncasecmp(player_table[id].name, argument, strlen(argument)))
    {
      found = 1;
      name = player_table[id].name;
      break;
    }
  }
  if (!found)
  {
    asend_to_char(ch, "No player whose name begins with '%s' exists.\r\n", argument);
    return;
  }
  if (!get_filename(name, fname, CRASH_FILE))
  {
    asend_to_char(ch, "Failed to get filename for %s...?\r\nErrors is: %s.\r\n", name, strerror(errno));
    return;
  }
  if (!(doc = xmlParseFile(fname)))
  {
    asend_to_char(ch, "Failed to parse file for %s, reason: %s.\r\n", name, strerror(errno)); 
    return;
  }
  if (!(root = doc->root))
  {
    asend_to_char(ch, "Odd. XML tree has no root? Failed to read file, somehow.\r\n");
    xmlFreeDoc(doc);
    return;
  }
  if (strncasecmp("playerobjs", root->name, strlen(root->name)))
  {
    asend_to_char(ch, "File %s isn't a playerobjs file! Root tag is <%s>. It should be deleted..\r\n", fname, root->name);
    xmlFreeDoc(doc);
    return;
  }

  asend_to_char(ch, "Contents of \\c02%s\\c00's obj file:\r\n", name);
  result = list_obj_XML_contents(root);
  page_string(ch->desc, result, 0);
  xmlFreeDoc(doc);
  free(result);
}
