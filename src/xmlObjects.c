/******************************************************************************
 * $Id: xmlObjects.c,v 1.5 2003/06/10 12:16:08 fang Exp $
 *   
 * File: xmlObjects.c
 *
 * Created: Thu Mar 23 2000 by Stefan Lindstrom (asmodean@ludd.luth.se)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: xmlObjects.c,v 
 *   Revision 1.2  2002/07/28 13:56:05  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.8  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.7  2002/03/08 14:45:38  fang
 *   *** empty log message ***
 *
 *   Revision 1.6  2002/03/07 08:47:39  fang
 *   *** empty log message ***
 *
 *   Revision 1.5  2002/02/24 03:54:41  fang
 *   *** empty log message ***
 *
 *   Revision 1.4  2002/02/24 03:48:05  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/02/22 19:17:19  fang
 *   Fixed entity substitution up for properties.
 *
 *   Revision 1.2  2002/02/22 15:04:29  fang
 *   *** empty log message ***
 *
 *   Revision 1.13  2001/11/10 11:16:16  mud
 *   Temp fix for object status...
 *
 *   Revision 1.12  2001/09/24 17:08:10  mud
 *   *** empty log message ***
 *
 *   Revision 1.11  2001/05/22 09:07:08  mud
 *   DG-Scripts for Fang.
 *
 *   Revision 1.10  2000/11/07 12:00:40  mud
 *   Finally. XML Mobiles are done.
 *
 *   Revision 1.9  2000/06/06 13:56:13  mud
 *   Finally. XML Objects works...The Mystery of the Vanishing ZCMDs is resolved as well.
 *
 *   Revision 1.8  2000/05/03 11:47:13  mud
 *   Parsing should now be complete. Need to make objects save as XML (oedit) and to call the correct parse when an xml-obj file is encountered in db-booting
 *
 *   Revision 1.7  2000/05/02 15:20:35  mud
 *
 *
 *   Loading of xml objects almost complete. Final touch on ObjProgs needed, as
 *   well as actually add the saving/loading function calls, and of course, the
 *   dreaded testing. :)
 *
 *   Revision 1.6  2000/05/01 14:23:43  mud
 *   &X colour codes are now both saved and parsed properly by both room and zone xml parser
 *
 *   Revision 1.5  2000/04/25 11:10:38  mud
 *   EXP gain is modified on behalf of newbies (lvl <= 20)
 *
 *   Revision 1.4  2000/03/31 09:13:37  mud
 *   Saving of objs in XML is done.
 *
 *   Revision 1.3  2000/03/29 15:11:05  mud
 *   First skeleton code for XML saving of .obj-files
 *
 *   Revision 1.2  2000/03/28 09:44:07  mud
 *   *** empty log message ***
 *
 *   Revision 1.1  2000/03/23 14:30:45  mud
 *   First commit, nothing much in here, yet... :)
 *
 *
 *****************************************************************************/
#include <stddef.h>
#include <errno.h>
#include <string.h>

#include <gnome-xml/parser.h>
#include <gnome-xml/tree.h>
#include <gnome-xml/entities.h>
#include <gnome-xml/parserInternals.h>
#include <stdio.h>

#include "db.h"
#include "olc.h"
#include "interpreter.h"
#include "utils.h"
#include "xmlRooms.h"           
#include "xmlObjects.h"
#include "comm.h"
#include "utils.h"
#include "constants.h"
#include "strutil.h"
#include "namedspec.h"
#include "dg_olc.h"

static int apply_string_to_int(char *str)
{ 
  int i = 0; 
  if (NULL == str)
    return 0;

  for (; '\n' != (*apply_types[i]); i++) 
    if (!strcasecmp(str,apply_types[i]))
      return i;

  return 0;
}  

static void fetchObjAffect(xmlNodePtr affect, int pos, struct obj_data *obj)
{ 
  char *p = xmlGetProp(affect,"location");
  obj->affected[pos].location = apply_string_to_int(p);
  free(p);

  obj->affected[pos].modifier = xmlAtoi(xmlGetProp(affect, "modifier"));
} 

static void fetchObjFlags(xmlNodePtr flags, struct obj_data *obj)
{ 
  xmlNodePtr tmp = flags->childs;
  
  for (; NULL != tmp; tmp = tmp->next) {
    if (!strcasecmp(tmp->name,"VALUES")) { 
      obj->obj_flags.value[0] = xmlAtoi(xmlGetProp(tmp,"value1"));
      
      obj->obj_flags.value[1] = xmlAtoi(xmlGetProp(tmp,"value2")); 

      obj->obj_flags.value[2] = xmlAtoi(xmlGetProp(tmp,"value3"));

      obj->obj_flags.value[3] = xmlAtoi(xmlGetProp(tmp,"value4"));
    } 
    else if (!strcasecmp(tmp->name,"FLAGS")) { 
      obj->obj_flags.type_flag = xmlAtoi(xmlGetProp(tmp,"type"));

      obj->obj_flags.wear_flags = xmlAtoi(xmlGetProp(tmp,"wear"));

      obj->obj_flags.extra_flags = xmlAtoi(xmlGetProp(tmp,"extra"));

      if (IS_SET(obj->obj_flags.extra_flags, ITEM_UNIQUE))
        SET_BIT(obj->obj_flags.extra_flags, ITEM_NOSAVE);
    } 
    else if (!strcasecmp(tmp->name,"DATA")) { 
      obj->obj_flags.weight = xmlAtoi(xmlGetProp(tmp,"weight"));

      obj->obj_flags.cost = xmlAtoi(xmlGetProp(tmp,"cost"));

      obj->obj_flags.questcost = xmlAtoi(xmlGetProp(tmp,"questcost"));

      obj->obj_flags.timer = xmlAtoi(xmlGetProp(tmp,"timer"));

      obj->obj_flags.bitvector = xmlAtoi(xmlGetProp(tmp,"bitvector"));
    }  
    else
      ; // Room for future expansion. ;)
  }
} 

static void fetchExDesc(xmlNodePtr desc, struct obj_data *obj)
{
  xmlNodePtr tmp;
  struct extra_descr_data *exd = calloc(1,sizeof(struct extra_descr_data));

  exd->keyword = xmlGetProp(desc,"keyword");

  for (tmp = desc->childs; NULL != tmp; tmp = tmp->next) { 
    if (!strcasecmp(tmp->name,"description"))
      exd->description = xmlToString(tmp->childs);
    else
      ; // Room for future expansion. :)
  }
  exd->next = obj->ex_description;
  obj->ex_description = exd;
}

static int MainTerm2Int(char *term) 
{ 
  int i = 0; 
  for (; i <= NUM_OBJ_MAINTERMS; i++) 
    if (!strcasecmp(omt_file[i],term)) {
      free(term);
      return i;
    }

  free(term);
  return 0;
} 

static int Side2Int(char *side)
{ 
  int i; 

  if (NULL == side)
    return 0;

  for (i = 0; i <= NUM_OBJ_SIDETERMS; i++)
    if (!strcasecmp(ost_file[i],side)) {
      free(side);
      return i;
    }

  free(side);
  return 0;
} 

static int Action2Int(char *action)
{
  int i;

  if (NULL == action)
    return 0;

  for (i = 0; i <= NUM_OBJ_ACTIONS; i++)
    if (!strcasecmp(obj_act_file[i],action)) {
      free(action);
      return i;
    }

  free(action);
  return 0;
} 

static void fetchSide(xmlNodePtr side, struct side_term_list *f)
{
  char *p;
  f->number = Side2Int(xmlGetProp(side,"number"));
  p = xmlGetProp(side,"not");
  if (NULL == p || strcasecmp(p,"FALSE"))
    f->not = 0;
  else
    f->not = 1;

  if (p)
    free(p);

  p = xmlGetProp(side,"argtype");
  if (NULL == p || !strcasecmp(p,"VOID")) 
    f->argument = NULL;
  else if (!strcasecmp(p,"STRING"))
    f->argument = xmlGetProp(side,"argument");
  else { 
    f->argument = NULL;
    f->int_arg  = xmlAtoi(xmlGetProp(side,"argument"));
  } 
  if (p)
    free(p);
}  

static void fetchAction(xmlNodePtr action,struct obj_action_list *f)
{
  char *p;
  f->action = Action2Int(xmlGetProp(action,"number"));

  p = xmlGetProp(action,"argtype");
  if (NULL == p || !strcasecmp(p,"VOID"))
    f->argument = NULL;
  else if (!strcasecmp(p,"STRING"))
    f->argument = xmlGetProp(action,"argument");
  else { 
    f->argument = NULL;
    f->intarg = xmlAtoi(xmlGetProp(action,"argument"));
  }  
  if (p)
    free(p);
} 

static void fetchObjProg(xmlNodePtr prog, struct obj_data *obj)
{
  char *p;
  xmlNodePtr tmp;
  struct obj_action_list *action = NULL,*ap;
  struct obj_prog_list   *maino = NULL ,*mp;
  struct side_term_list  *side = NULL  ,*sp;

  maino = calloc(1,sizeof(struct obj_prog_list));
  maino->main = MainTerm2Int(xmlGetProp(prog,"main"));
  p = xmlGetProp(prog,"argtype");
  if (NULL == p || !strcasecmp(p,"VOID")) 
    maino->argument = NULL;
  else if (!strcasecmp(p,"INT")) { 
    maino->argument = NULL;
    maino->intarg = xmlAtoi(xmlGetProp(prog,"argument"));
  } 
  else
    maino->argument = xmlGetProp(prog,"argument");

  if (p)
    free(p);
      
  for (tmp = prog->childs; NULL != tmp; tmp = tmp->next) {  
    if (!strcasecmp(tmp->name,"SIDETERM")) {  
      side = calloc(1,sizeof(struct side_term_list)); 
      fetchSide(tmp,side);
      side->next = NULL;
      
      // Need to insert side as the last element in maino's side list. 
      if (NULL == maino->sideterms)
    maino->sideterms = side;
      else {
    sp = maino->sideterms;
    while (NULL != sp->next)
      sp = sp->next;
    sp->next = side;
      } 
    } 
    else if (!strcasecmp(tmp->name,"ACTION")) { 
      action = calloc(1,sizeof(struct obj_action_list));
      fetchAction(tmp,action);
      action->next = NULL;

      // Need to inser action as the last element in maino's action list
      if (NULL == maino->actions)
    maino->actions = action;
      else { 
    ap = maino->actions;
    while (NULL != ap->next)
      ap = ap->next;
    ap->next = action;
      } 
    } 
    else
      ; // Room for future expansions. :)
  } 
  
  mp = obj->oprog;
  maino->next = NULL;

  if (NULL == mp) { 
    obj->oprog = maino;
  } else { 

    while (NULL != mp->next) 
      mp = mp->next;
    
    mp->next = maino;
  } 
} 

static struct obj_data *fetchXmlObj(xmlNodePtr o)
{
  xmlNodePtr tmp;
  int currAff = 0; 
  struct obj_data *obj = calloc(1,sizeof(struct obj_data));

  obj->item_number = last_obj_rnum;
  obj->name        = xmlGetProp(o,"name");

  for (tmp = o->childs; NULL != tmp; tmp = tmp->next) { 
    if (!strcasecmp(tmp->name,"DESCRIPTION")) 
      obj->description = xmlToString(tmp->childs); // fetch description

    else if (!strcasecmp(tmp->name,"SHORTDESC"))
      obj->short_description = xmlToString(tmp->childs); // Fetch short desc

    else if (!strcasecmp(tmp->name,"ACTIONDESC"))
      obj->action_description = xmlToString(tmp->childs); // Fetch action desc

    else if (!strcasecmp(tmp->name,"AFFECTFLAG"))
      obj->obj_flags.bitvector = atoi(tmp->childs->name); // fetch affect flag stuff; 

    else if (!strcasecmp(tmp->name,"OBJAFFECT")) { 
      fetchObjAffect(tmp,currAff,obj); // fetch affect on pos currAff
      currAff++;
    } 

    else if (!strcasecmp(tmp->name,"OBJFLAGS")) 
      fetchObjFlags(tmp,obj); // fetch obj flag values. 

    else if (!strcasecmp(tmp->name,"OBJPROG"))
      fetchObjProg(tmp,obj); // fetch ObjProg

    else if (!strcasecmp(tmp->name,"EXDESC"))
      fetchExDesc(tmp,obj); // fetch exdesc

    else if (!strcasecmp(tmp->name,"DGSCRIPT"))
      fetchDGScript(tmp, obj, OBJ_TRIGGER);

    else // Something else. room for future expansionj. 
      ; 
  } 

  return obj;
} 

void load_xml_objects(char *file)
{
  xmlNodePtr root,temp;
  xmlDocPtr  doc;
  struct obj_data *obj;       
  char *p;

  if (!(doc = xmlParseFile(file))) {
    alog("load_xml_objects:[xmlObjects.c]: Couldn't parse XML-file %s",file);
    return;
  }
  if (!(root = doc->root)) {
    alog("load_xml_objects:[xmlObjects.c]: No XML-root in document %s",file);
    xmlFreeDoc(doc);
    return;
  }

  if (strncasecmp("OBJECTS",root->name,strlen(root->name))) {
    alog("load_xml_objects:[xmlObjects.c]: XML-root (%s in file %s) is not a object!",root->name,file);
    xmlFreeDoc(doc);
    return;
  }
  for (temp = root->childs; NULL != temp; temp = temp->next) {
    obj = fetchXmlObj(temp);
    *(obj_proto+last_obj_rnum) = *obj;
    (obj_proto+last_obj_rnum)->in_room = NOWHERE;
    (obj_proto+last_obj_rnum)->obj_status = 100;
    (obj_index+last_obj_rnum)->virtual = xmlAtoi(xmlGetProp(temp,"vnum"));
    (obj_index+last_obj_rnum)->number  = 0;
    if (NULL == (p = xmlGetProp(temp,"special")) || !strcasecmp(p,"NONE"))
      (obj_index+last_obj_rnum)->func = NULL;
    else {
      (obj_index+last_obj_rnum)->func = getSpecialName(p); 
      free(p);
    }

    free(obj);
    last_obj_rnum++;
  }
  top_of_objt = last_obj_rnum;//++; /* Gav's fix so we don't loose any zones. :) */
  xmlFreeDoc(doc);                                                     
}

static xmlNodePtr makeObjExdesc(struct extra_descr_data *e, xmlDocPtr doc)
{
  char *p = NULL;
  xmlNodePtr xex = xmlNewNode(NULL,"exdesc");
  xex->doc = doc;

  xmlNewProp(xex,"keyword",e->keyword);

  xmlNewChild(xex,NULL,"description",(p = internalXmlString(e->description,doc)));
  free(p);

  return xex;
}  

static xmlNodePtr makeObjprog(struct obj_prog_list *op,xmlDocPtr doc)
{
  char buf[4096]; 
  struct side_term_list *st;
  struct obj_action_list *ac;

  xmlNodePtr prog = xmlNewNode(NULL,"objprog"),xst,xac;
  prog->doc = doc;           

  xmlNewProp(prog,"main",(char *)omt_file[op->main]);

  if (!op->intarg && (!op->argument || !*op->argument)) {
    xmlNewProp(prog,"argtype", "VOID");
     xmlNewProp(prog,"argument","NONE");
  }          
  else if (!op->argument || !*op->argument) {   
    xmlNewProp(prog,"argtype","INT");
    sprintf(buf,"%d",op->intarg);
    xmlNewProp(prog,"argument",buf);
  } 
  else { 
    xmlNewProp(prog,"argtype","STRING");
    xmlNewProp(prog,"argument", op->argument);
  } 

  /* Main stuff taken care of, now time to add all side terms */
  st = op->sideterms;
  while (NULL != st) { 
    xst = xmlNewNode(NULL,"sideterm");
    xst->doc = doc;

    xmlNewProp(xst,"number",(char *)ost_file[st->number]);
    xmlNewProp(xst,"not",(!st->not ? "FALSE" : "TRUE"));

    
    if (!st->int_arg && (!st->argument || !*st->argument)) {
      xmlNewProp(xst,"argtype", "VOID");
      xmlNewProp(xst,"argument","NONE");
    }
    else if (!st->argument || !*st->argument) {
      xmlNewProp(xst,"argtype","INT");
      sprintf(buf,"%d",st->int_arg);
      xmlNewProp(xst,"argument",buf);
    }
    else {
      xmlNewProp(xst,"argtype","STRING");
      xmlNewProp(xst,"argument",st->argument);
    }

    xmlAddChild(prog,xst);
    st = st->next;
  } 

  /* Sideterms dealt with, move on to action list */
  ac = op->actions;
  while (NULL != ac) {
    xac = xmlNewNode(NULL,"action");
    xac->doc = doc;

    xmlNewProp(xac,"number",obj_act_file[ac->action]);
    if (!ac->intarg && (!ac->argument || !*ac->argument)) {
      xmlNewProp(xac,"argtype", "VOID");
      xmlNewProp(xac,"argument","NONE");
    }
    else if (!ac->argument || !*ac->argument) {
      xmlNewProp(xac,"argtype","INT");
      sprintf(buf,"%d",ac->intarg);
      xmlNewProp(xac,"argument",buf);
    }
    else {
      xmlNewProp(xac,"argtype","STRING");
      xmlNewProp(xac,"argument", ac->argument);
    }

    xmlAddChild(prog,xac);
    ac = ac->next;
  }     

  return prog;
} 

static xmlNodePtr objToXml(struct obj_data *o, xmlDocPtr doc)
{
  char buf[4096],*p;
  int i;
  struct extra_descr_data *e;
  struct obj_prog_list *op;
  struct trig_proto_list *trig;
  xmlNodePtr obj = xmlNewNode(NULL,"object"),aff,tmp;
  
  obj->doc = doc;
  sprintf(buf,"%ld",obj_index[o->item_number].virtual);
  xmlNewProp(obj,"vnum",buf);
  xmlNewProp(obj,"name",o->name);

  xmlNewProp(obj,"special",(NULL == GET_OBJ_SPEC(o) ? 
                "NONE" : (NULL == getSpecialPointer(GET_OBJ_SPEC(o)) ? 
                      "NONE" : getSpecialPointer(GET_OBJ_SPEC(o)))));

  xmlNewChild(obj,NULL,"description",(p = internalXmlString(o->description,doc)));
  free(p);
  xmlNewChild(obj,NULL,"shortdesc",(p = internalXmlString(o->short_description,doc)));
  free(p);
  xmlNewChild(obj,NULL,"actiondesc",(p = internalXmlString(o->action_description,doc)));
  free(p);

  sprintf(buf,"%ld",o->obj_flags.bitvector);
  xmlNewChild(obj,NULL,"affectflag",buf);

  for (i = 0; i < MAX_OBJ_AFFECT; i++) { 
    if (o->affected[i].location >= 0 && o->affected[i].location < 25) { 
      aff = xmlNewNode(NULL,"objaffect");
      aff->doc = doc;
      xmlNewProp(aff,"location",apply_types[(int)o->affected[i].location]);
      sprintf(buf,"%ld",o->affected[i].modifier);
      xmlNewProp(aff,"modifier",buf);
      xmlAddChild(obj,aff);
    } 
  }  

  /* Obj flags stuff...*/
  aff = xmlNewNode(NULL,"objflags");
  aff->doc = doc;
  tmp = xmlNewNode(NULL,"values");
  tmp->doc = doc;
  sprintf(buf,"%ld",o->obj_flags.value[0]);
  xmlNewProp(tmp,"value1",buf);

  sprintf(buf,"%ld",o->obj_flags.value[1]);
  xmlNewProp(tmp,"value2",buf);  

  sprintf(buf,"%ld",o->obj_flags.value[2]);
  xmlNewProp(tmp,"value3",buf);     

  sprintf(buf,"%ld",o->obj_flags.value[3]);
  xmlNewProp(tmp,"value4",buf);     
  xmlAddChild(aff,tmp);

  tmp = xmlNewNode(NULL,"flags");
  tmp->doc = doc;
  sprintf(buf,"%d",(int)o->obj_flags.type_flag);
  xmlNewProp(tmp,"type",buf);
  sprintf(buf,"%ld",o->obj_flags.wear_flags);
  xmlNewProp(tmp,"wear",buf);

  if (IS_SET(o->obj_flags.extra_flags, ITEM_UNIQUE)) 
    SET_BIT(o->obj_flags.extra_flags, ITEM_NOSAVE);
  sprintf(buf,"%ld",o->obj_flags.extra_flags);
  xmlNewProp(tmp,"extra",buf);

  xmlAddChild(aff,tmp);

  tmp = xmlNewNode(NULL,"data");
  tmp->doc = doc;
  sprintf(buf,"%ld",o->obj_flags.weight);
  xmlNewProp(tmp,"weight",buf);
  sprintf(buf,"%ld",o->obj_flags.cost);
  xmlNewProp(tmp,"cost",buf);
  sprintf(buf,"%ld",o->obj_flags.questcost);
  xmlNewProp(tmp,"questcost",buf);
  sprintf(buf,"%ld",o->obj_flags.timer);
  xmlNewProp(tmp,"timer",buf);
  sprintf(buf,"%ld",o->obj_flags.bitvector);
  xmlNewProp(tmp,"bitvector",buf);
  xmlAddChild(aff,tmp);

  xmlAddChild(obj,aff);
  /* End of obj flags */

  if (NULL != (e = o->ex_description)) {   
    while (NULL != e) {   
      xmlAddChild(obj,makeObjExdesc(e,doc));
      e = e->next;
    }   
  }   
  
  if (NULL != (op = o->oprog)) { 
    while (NULL != op) { 
      xmlAddChild(obj,makeObjprog(op,doc));
      op = op->next;
    }
  }   

  /* DGS */
  trig = o->proto_script;
  while (trig)
  {
   xmlAddChild(obj, makeDGScript(trig, doc));
   trig = trig->next;
  }

  return obj;
}   

void objects_save_zone_to_file(int start,int stop,FILE *f)
{
  int i,rr;
  xmlNodePtr tree,obj;
  xmlDocPtr doc;    

  doc = xmlNewDoc("1.0");

  tree = xmlNewDocNode(doc,NULL,"objects",NULL);
  doc->root = tree;

  for (i = start; i <= stop; i++) {     
    if (-1 != (rr = real_object(i))) {
      obj = objToXml((obj_proto+rr),doc);
      xmlAddChild(tree,obj);
    }
  }

  xmlDocDump(f,doc);
  xmlFreeDoc(doc);          
}
    
int count_xml_objectentries(FILE *f)
{
  char bf[25000],*u;
  int cnt = 0;

  while (fgets(bf,25000,f) && !feof(f)) {
    bf[(strlen(bf)-1)] = '\0';
    u = strupper(bf);
    
    if (NULL != strstr(u,"</OBJECT>")) {
      cnt++;
    }
    
    free(u);
  }

  return cnt;                            
}
