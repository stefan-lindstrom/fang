/******************************************************************************
 * $Id: xmlMobiles.c,v 1.4 2002/11/07 04:02:04 fang Exp $
 *   
 * File: xmlMobiles.c
 *
 * Created: Sat Jun 10 2000 by stefan lindström (asmodean@ludd.luth.se)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: xmlMobiles.c,v 
 *   Revision 1.2  2002/07/28 13:56:05  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.7  2002/06/17 08:14:15  fang
 *   *** empty log message ***
 *
 *   Revision 1.6  2002/06/07 09:34:54  fang
 *   *** empty log message ***
 *
 *   Revision 1.5  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.4  2002/03/07 08:47:39  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/02/22 19:17:19  fang
 *   Fixed entity substitution up for properties.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.14  2001/11/10 11:16:16  mud
 *   Temp fix for object status...
 *
 *   Revision 1.13  2001/11/08 08:53:50  mud
 *   Setting #dice/dice size/bonus for move/mana/hp on mobs in medit now works as it should.
 *
 *   Revision 1.12  2001/09/28 07:51:26  mud
 *   More mem leaks taken care of
 *
 *   Revision 1.11  2001/09/24 17:08:10  mud
 *   *** empty log message ***
 *
 *   Revision 1.10  2001/05/22 09:07:08  mud
 *   DG-Scripts for Fang.
 *
 *   Revision 1.9  2001/02/13 14:35:54  mud
 *   *** empty log message ***
 *
 *   Revision 1.8  2000/12/25 15:38:40  mud
 *   *** empty log message ***
 *
 *   Revision 1.7  2000/12/23 11:41:55  mud
 *   *** empty log message ***
 *
 *   Revision 1.6  2000/11/07 12:00:40  mud
 *   Finally. XML Mobiles are done.
 *
 *   Revision 1.5  2000/08/31 06:03:54  mud
 *   Saving of mobs in XML is finished...Onto the loading part. :)
 *
 *   Revision 1.4  2000/08/30 02:32:38  mud
 *   Safety commit...
 *
 *   Revision 1.3  2000/08/29 04:32:30  mud
 *   Almost done with xml mob saves...
 *
 *   Revision 1.2  2000/07/28 17:44:33  mud
 *
 *   New talent code.
 *
 *   Revision 1.1  2000/06/10 17:48:24  mud
 *   First commit, not much here yet...
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
#include "class.h"
#include "dg_olc.h"

static void fetchCharData(xmlNodePtr m, struct char_data *mob) 
{
  char *sex,*class, *level, *weight, *height;

  sex    = xmlGetProp(m,"gender");
  class  = xmlGetProp(m,"class");
  level  = xmlGetProp(m,"level");
  weight = xmlGetProp(m,"weight");
  height = xmlGetProp(m,"height");

  if (NULL == sex || !strcasecmp(sex,"NEUTRAL"))
    mob->player.sex = SEX_NEUTRAL;
  else if (!strcasecmp(sex,"MALE"))
    mob->player.sex = SEX_MALE;
  else 
    mob->player.sex = SEX_FEMALE;

  if (NULL == class)
    mob->player.class = CLASS_ADVENTURER;
  else
    mob->player.class = classStr2Num(class);

  mob->player.level  = atoi(level  ? level  : "0");
  mob->player.height = atoi(height ? height : "0");
  mob->player.weight = atoi(weight ? weight : "0");

  if (sex)
    free(sex);
  if (class)
    free(class);
  if (level)
    free(level);
  if (weight)
    free(weight);
  if (height)
    free(height);
}

static void fetchAbils(xmlNodePtr a,struct char_data *mob)
{
  xmlNodePtr atmp;
  char *p = NULL;

  for (atmp = a->children; NULL != atmp; atmp = atmp->next) {
    if (!strcasecmp(atmp->name,"STR")) {
      mob->real_abils.str = (char)atoi((p == xmlToString(atmp->children)) ? p : "18");
      if (p) free(p);
    }
    else if (!strcasecmp(atmp->name,"INT")) {
      mob->real_abils.intel = (char)atoi((p == xmlToString(atmp->children)) ? p : "18");
      if (p) free(p);
    }
    else if (!strcasecmp(atmp->name,"WIS")) {
      mob->real_abils.wis = (char)atoi((p == xmlToString(atmp->children)) ? p : "18");
      if (p) free(p);
    }
    else if (!strcasecmp(atmp->name,"DEX")) {
      mob->real_abils.dex = (char)atoi((p == xmlToString(atmp->children)) ? p : "18");
      if (p) free(p);
    }
    else if (!strcasecmp(atmp->name,"CON")) {
      mob->real_abils.con = (char)atoi((p == xmlToString(atmp->children)) ? p : "18");
      if (p) free(p);
    }
    else if (!strcasecmp(atmp->name,"CHA")) {
      mob->real_abils.cha = (char)atoi((p == xmlToString(atmp->children)) ? p : "18");
      if (p) free(p);
    }
    else
      ; // Future expansion
  }
  mob->aff_abils = mob->real_abils;
}

static void fetchStats(xmlNodePtr s, struct char_data *mob)
{
  xmlNodePtr tmp;
  char *p;

  for (tmp = s->children; NULL != tmp; tmp = tmp->next) {
    if (!strcasecmp(tmp->name,"MANA")) {
      mob->mob_specials.mananodice   = atoi((p = xmlGetProp(tmp,"numd"))  ? p : "1");
      if (p) free(p);
      mob->mob_specials.manasizedice = atoi((p = xmlGetProp(tmp,"dsize")) ? p : "100");
      if (p) free(p);
      mob->mob_specials.manabonus    = atoi((p = xmlGetProp(tmp,"bonus")) ? p : "100");
      if (p) free(p);
    }
    else if (!strcasecmp(tmp->name,"HIT")) {
      mob->mob_specials.hpnodice   = atoi((p = xmlGetProp(tmp,"numd"))  ? p : "1");
      if (p) free(p);
      mob->mob_specials.hpsizedice = atoi((p = xmlGetProp(tmp,"dsize")) ? p : "100");
      if (p) free(p);
      mob->mob_specials.hpbonus    = atoi((p = xmlGetProp(tmp,"bonus")) ? p : "100");
      if (p) free(p);
    }
    else if (!strcasecmp(tmp->name,"MOVE")) {
      mob->mob_specials.movenodice   = atoi((p = xmlGetProp(tmp,"ndize"))  ? p : "1");
      if (p) free(p);
      mob->mob_specials.movesizedice = atoi((p = xmlGetProp(tmp,"dsize")) ? p : "100");
      if (p) free(p);
      mob->mob_specials.movebonus    = atoi((p = xmlGetProp(tmp,"bonus")) ? p : "100");
      if (p) free(p);
    }
    else if (!strcasecmp(tmp->name,"AC")) {
      p = xmlToString(tmp->children);
      p = (NULL == p ? "100" : p);
      mob->points.armor = atoi(p);
      if (p) free(p);
    }
    else if (!strcasecmp(tmp->name,"GOLD")) {
      p = xmlToString(tmp->children);
      p = (NULL == p ? "0" : p);
      mob->points.gold = atoi(p);
      if (p) free(p);
    }
    else if (!strcasecmp(tmp->name,"BANKGOLD")) {
      p = xmlToString(tmp->children);
      p = (NULL == p ? "0" : p);
      mob->points.bank_gold = atoi(p);
      if (p) free(p);
    }
    else if (!strcasecmp(tmp->name,"EXP")) {
      p = xmlToString(tmp->children);
      p = (NULL == p ? "0" : p);
      mob->points.exp = atoi(p);
      if (p) free(p);
    }
    else if (!strcasecmp(tmp->name,"HITROLL")) {
      p = xmlToString(tmp->children);
      p = (NULL == p ? "0" : p);
      mob->points.hitroll = atoi(p);
      if (p) free(p);
    }
    else if (!strcasecmp(tmp->name,"DAMROLL")) {
      p = xmlToString(tmp->children);
      p = (NULL == p ? "0" : p);
      mob->points.damroll = atoi(p);
      if (p) free(p);
    }
    else if (!strcasecmp(tmp->name,"ABS")) {
      p = xmlToString(tmp->children);
      p = (NULL == p ? "0" : p);
      if (p) free(p);
      /* What to do with this, we've no MOB ABS saved, no? */
    }
    else
      ; // Future expansion?
  }
}

static int posStr2Int(char *pos)
{
  register int i = 0; 

  if (NULL == pos)
    return POS_STANDING;

  for (i = 0; '\n' != *position_types[i]; i++) {
    if (!strcasecmp(pos,position_types[i])) {
      free(pos);
      return i;
    }
  }

  free(pos);
  return POS_STANDING;
}

static void fetchSkills(xmlNodePtr sv, struct char_data *mob)
{
  register int i = 0;
  int j;
  xmlNodePtr tmp;
  char *p = xmlGetProp(sv,"numberof"),*name;
  
  if (NULL == p)
    return;
      
  if (!atoi(p)) {
    free(p);
    return;
  }

  mob->mob_specials.teaches_num = j = atoi(p);
  free(p);
  mob->mob_specials.skills[0] = calloc(j, sizeof(int));
  mob->mob_specials.skills[1] = calloc(j, sizeof(int));

  tmp = sv->children;

  for (; i < j; i++) {
    name = xmlGetProp(tmp,"name");
    p    = xmlGetProp(tmp,"upto");
    mob->mob_specials.skills[0][i] = ((name) ? find_skill_num(name) : 0);
    mob->mob_specials.skills[1][i] = atoi(( p ? p : "0"));
    tmp = tmp->next;
    free(name);
    free(p);
  }
}

static int MainTerm2Int(char *term) 
{ 
  int i = 0; 
  for (; i <= NUM_MAINTERMS; i++) 
    if (!strcasecmp(mt_file[i],term)) {
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

  for (i = 0; i <= NUM_SIDETERMS; i++)
    if (!strcasecmp(st_file[i],side)) {
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

  for (i = 0; i <= NUM_MOBACTIONS; i++)
    if (!strcasecmp(act_file[i],action)) {
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
  if (NULL == p || strcasecmp(p,"FALSE") == 0)
    f->not = 0;
  else 
    f->not = 1;
  if (p)
    free(p);

  p = xmlGetProp(side,"argtype");
  if (NULL == p || !strcasecmp(p,"VOID")) {
    f->argument = NULL;
    f->int_arg = 0;
  } else if (!strcasecmp(p,"STRING")) {
    f->argument = xmlGetProp(side,"argument");
    if (f->argument && isdigit(*f->argument)) 
      f->int_arg = atoi(f->argument);
  } else { 
    f->argument = xmlGetProp(side,"argument");
    f->int_arg  = atoi(f->argument);
    free(f->argument);
    f->argument = NULL;
  } 
  if (p)
    free(p);
}  

static void fetchAction(xmlNodePtr action,struct mob_action_list *f)
{
  char *p;
  f->action = Action2Int(xmlGetProp(action,"number"));

  p = xmlGetProp(action,"argtype");
  if (NULL == p || !strcasecmp(p,"VOID"))
    f->argument = NULL;
  else if (!strcasecmp(p,"STRING"))
    f->argument = xmlGetProp(action,"argument");
  else { 
    f->argument = xmlGetProp(action,"argument");
    f->intarg = atoi(f->argument);
    free(f->argument);
    f->argument = NULL;
  }  
  if (p)
    free(p);
} 

static void fetchMobProg(xmlNodePtr prog, struct char_data *obj)
{
  char *p;
  xmlNodePtr tmp;
  struct mob_action_list *action = NULL,*ap;
  struct mob_prog_list   *maino = NULL ,*mp;
  struct side_term_list  *side = NULL  ,*sp;

  maino = calloc(1,sizeof(struct mob_prog_list));
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
      
  for (tmp = prog->children; NULL != tmp; tmp = tmp->next) {  
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
      action = calloc(1,sizeof(struct mob_action_list));
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
  
  mp = obj->mprog;
  maino->next = NULL;

  if (NULL == mp) { 
    obj->mprog = maino;
  } else { 

    while (NULL != mp->next) 
      mp = mp->next;
    
    mp->next = maino;
  } 
} 

static struct char_data *fetchXmlMob(xmlNodePtr m) 
{
  xmlNodePtr tmp;
  char *p;
  struct char_data *mob = calloc(1,sizeof(struct char_data));

  mob->nr = last_obj_rnum;
  mob->player.name = xmlGetProp(m,"name");

  for (tmp = m->children; NULL != tmp; tmp = tmp->next) {
    if (!strcasecmp(tmp->name,"DESCRIPTION"))
      mob->player.description = xmlToString(tmp->children);

    else if (!strcasecmp(tmp->name,"SHORTDESC"))
      mob->player.short_descr = xmlToString(tmp->children);

    else if (!strcasecmp(tmp->name,"LONGDESC"))
      mob->player.long_descr = xmlToString(tmp->children);

    else if (!strcasecmp(tmp->name,"TITLE"))
      mob->player.title = xmlToString(tmp->children);

    else if (!strcasecmp(tmp->name,"CHARDATA"))
      fetchCharData(tmp,mob);
    
    else if (!strcasecmp(tmp->name,"ABILITIES"))
      fetchAbils(tmp,mob);

    else if (!strcasecmp(tmp->name,"STATS"))
      fetchStats(tmp,mob);

    else if (!strcasecmp(tmp->name,"ACT")) {
      mob->char_specials.saved.act = atoi(((p = xmlGetProp(tmp,"flag")) ? p : "0"));
      if (p)
    free(p);
    }
    
    else if (!strcasecmp(tmp->name,"AFFECTED")) {
      mob->char_specials.saved.affected_by = atoi(((p = xmlGetProp(tmp,"one"))   ? p : "0")); 
      if (p) free(p);
      mob->char_specials.saved.affected2   = atoi(((p = xmlGetProp(tmp,"two"))   ? p : "0"));
      if (p) free(p);
      mob->char_specials.saved.affected3   = atoi(((p = xmlGetProp(tmp,"three")) ? p : "0"));
      if (p) free(p);
      mob->char_specials.saved.affected4   = atoi(((p = xmlGetProp(tmp,"four"))  ? p : "0"));
      if (p) free(p);
    }

    else if (!strcasecmp(tmp->name,"ATTACK")) {
      mob->mob_specials.attack_type =  atoi(((p = xmlGetProp(tmp,"type")) ? p : "0"));
      if (p) free(p);
    }
    
    else if (!strcasecmp(tmp->name,"POS")) {
      mob->char_specials.position = mob->mob_specials.default_pos = posStr2Int(xmlGetProp(tmp,"default"));

      mob->mob_specials.default_pos = posStr2Int(xmlGetProp(tmp,"other"));
    }

    else if (!strcasecmp(tmp->name,"DAMAGE")) {
      mob->mob_specials.damnodice   = (char) atoi(((p = xmlGetProp(tmp,"num"))   ? p : "1"));
      if (p) free(p);
      mob->mob_specials.damsizedice = (char) atoi(((p = xmlGetProp(tmp,"sides")) ? p : "10"));
      if (p) free(p);
      mob->points.hitroll           =        atoi(((p = xmlGetProp(tmp,"add"))   ? p : "2"));
      if (p) free(p);
    }

    else if (!strcasecmp(tmp->name,"TIMER")) {
      mob->char_specials.timer = atoi(((p = xmlGetProp(tmp,"amount")) ? p : "0"));
      if (p) free(p);
    }

    else if (!strcasecmp(tmp->name,"SKILL")) {
      mob->mob_specials.skillvector = atoi(((p = xmlGetProp(tmp,"vector")) ? p : "0"));
      if (p) free(p);
    }

    else if (!strcasecmp(tmp->name,"TEACHES"))
      fetchSkills(tmp,mob);

    else if (!strcasecmp(tmp->name,"MOBPROG"))
      fetchMobProg(tmp,mob);
    
    else if (!strcasecmp(tmp->name,"DGSCRIPT"))
      fetchDGScript(tmp, mob, MOB_TRIGGER);

    else
      ;  // Future expansion! :)>
  }

  return mob;
}

void load_xml_mobiles(char *file)
{
  xmlNodePtr root,temp;
  xmlDocPtr  doc;
  struct char_data *mob;       
  char *p;

  if (!(doc = xmlParseFile(file))) {
    alog("load_xml_mobiles:[xmlMobiles.c]: Couldn't parse XML-file %s",file);
    return;
  }

  if (!(root = doc->children)) {
    alog("load_xml_mobiles:[xmlMobiles.c]: No XML-root in document %s",file);
    xmlFreeDoc(doc);
    return;
  }

  if (strncasecmp("MOBILES",root->name,strlen(root->name))) {
    alog("load_xml_mobiles:[xmlMobiles.c]: XML-root (%s in file %s) is not a mob-object!",root->name,file);
    xmlFreeDoc(doc);
    return;
  }
  
  for (temp = root->children; NULL != temp; temp = temp->next) {
    mob = fetchXmlMob(temp);
    mob->player_specials = &dummy_mob;

    *(mob_proto + last_mob_rnum) = *mob;
    (mob_proto + last_mob_rnum)->nr = last_mob_rnum;
    (mob_proto + last_mob_rnum)->in_room = NOWHERE;
    (mob_proto + last_mob_rnum)->was_in_room = NOWHERE;

    (mob_index + last_mob_rnum)->virtual = xmlAtoi(xmlGetProp(temp,"vnum"));
    (mob_index + last_mob_rnum)->number  = 0;

    if (NULL == (p = xmlGetProp(temp,"special")) || !strcasecmp(p,"NONE")) {
      if (p)
    free(p);
      (mob_index+last_mob_rnum)->func = NULL;
    }
    else {
      (mob_index+last_mob_rnum)->func = getSpecialName(p); 
      free(p);
    }

    free(mob);
    top_of_mobt = last_mob_rnum++;
  }
  //  top_of_mobt = last_mob_rnum;
  xmlFreeDoc(doc);
}

static xmlNodePtr makeCharData(struct char_data *mob,xmlDocPtr doc)
{
  char buf[1024];
  
  xmlNodePtr cdata = xmlNewNode(NULL,"chardata");
  cdata->doc = doc;

  xmlNewProp(cdata,"gender",(SEX_MALE == mob->player.sex ? "MALE" : 
                (SEX_FEMALE == mob->player.sex ? "FEMALE" : "NEUTRAL")));
  xmlNewProp(cdata,"class",(mob->player.class < 0 || mob->player.class > (NUM_CLASSES-1) ?
                pc_class_types[0] : pc_class_types[(int)mob->player.class]));
  sprintf(buf,"%ld",mob->player.level);
  xmlNewProp(cdata,"level",buf);
  sprintf(buf,"%d",(int)mob->player.weight);
  xmlNewProp(cdata,"weight",buf);
  sprintf(buf,"%d",(int)mob->player.height);
  xmlNewProp(cdata,"height",buf);

  return cdata;
}

static xmlNodePtr makeAdata(struct char_data *mob, xmlDocPtr doc)
{
  char buf[1024];
  
  xmlNodePtr adata = xmlNewNode(NULL,"abilities");
  adata->doc = doc;

  sprintf(buf,"%d",(int)mob->real_abils.str);
  xmlNewChild(adata,NULL,"str",buf);

  sprintf(buf,"%d",(int)mob->real_abils.intel);
  xmlNewChild(adata,NULL,"int",buf);

  sprintf(buf,"%d",(int)mob->real_abils.wis);
  xmlNewChild(adata,NULL,"wis",buf);

  sprintf(buf,"%d",(int)mob->real_abils.dex);
  xmlNewChild(adata,NULL,"dex",buf);

  sprintf(buf,"%d",(int)mob->real_abils.con);
  xmlNewChild(adata,NULL,"con",buf);

  sprintf(buf,"%d",(int)mob->real_abils.cha);
  xmlNewChild(adata,NULL,"cha",buf);

  return adata;
}

static xmlNodePtr makeStats(struct char_data *mob, xmlDocPtr doc)
{
  char buf1[100],buff2[100],buf3[100];
  xmlNodePtr stats = xmlNewNode(NULL,"stats"),temp;
  stats->doc = doc;

  sprintf(buf1,"%d",mob->mob_specials.mananodice);
  sprintf(buff2,"%d",mob->mob_specials.manasizedice);
  sprintf(buf3,"%d",mob->mob_specials.manabonus); 
  temp = xmlNewNode(NULL,"mana");
  temp->doc = doc;
  xmlNewProp(temp,"numd",buf1);
  xmlNewProp(temp,"dsize",buff2);
  xmlNewProp(temp,"bonus",buf3); // This wont be used for now, I guess...
  xmlAddChild(stats,temp);

  sprintf(buf1,"%d",mob->mob_specials.hpnodice);
  sprintf(buff2,"%d",mob->mob_specials.hpsizedice);
  sprintf(buf3,"%d",mob->mob_specials.hpbonus);
  temp = xmlNewNode(NULL,"hit");
  temp->doc = doc;
  xmlNewProp(temp,"numd",buf1);
  xmlNewProp(temp,"dsize",buff2);
  xmlNewProp(temp,"bonus",buf3);
  xmlAddChild(stats,temp);

  sprintf(buf1,"%d",mob->mob_specials.movenodice);
  sprintf(buff2,"%d",mob->mob_specials.movesizedice);
  sprintf(buf3,"%d",mob->mob_specials.movebonus);
  temp = xmlNewNode(NULL,"move");
  temp->doc = doc;
  xmlNewProp(temp,"ndize",buf1);
  xmlNewProp(temp,"dsize",buff2);
  xmlNewProp(temp,"bonus",buf3);
  xmlAddChild(stats,temp);

  sprintf(buf1,"%d",mob->points.armor);      // ac
  xmlNewChild(stats,NULL,"ac",buf1);

  sprintf(buf1,"%ld",mob->points.gold);      // gold
  xmlNewChild(stats,NULL,"gold",buf1);

  sprintf(buf1,"%ld",mob->points.bank_gold); // If mobs ever gets money onna bank. :)
  xmlNewChild(stats,NULL,"bankgold",buf1);

  sprintf(buf1,"%ld",mob->points.exp);       // exp
  xmlNewChild(stats,NULL,"exp",buf1);

  sprintf(buf1,"%ld",mob->points.hitroll);   // hr
  xmlNewChild(stats,NULL,"hitroll",buf1);

  sprintf(buf1,"%ld",mob->points.damroll);   // dr
  xmlNewChild(stats,NULL,"damroll",buf1);

  sprintf(buf1,"%d",0);                      // For eventual future abs...
  xmlNewChild(stats,NULL,"abs",buf1);

  return stats;
}

static const char *posInt2Str(int pos) 
{
  if (pos < POS_DEAD || pos > POS_SUBDUED) {
    return position_types[POS_STANDING];
  }
  return position_types[pos];
}

static xmlNodePtr buildSkillVector(struct char_data *mob,xmlDocPtr doc)
{
  int i;
  char buf[100];
  xmlNodePtr skillv = xmlNewNode(NULL,"teaches"),skill;
  skillv->doc = doc;

  sprintf(buf,"%d",mob->mob_specials.teaches_num);
  xmlNewProp(skillv,"numberof",buf);

  for (i = 0; i < mob->mob_specials.teaches_num ; i++) {
    skill = xmlNewNode(NULL,"skill");
    skill->doc = doc;
    xmlNewProp(skill,"name",spells[mob->mob_specials.skills[0][i]]);
    sprintf(buf,"%d",mob->mob_specials.skills[1][i]);
    xmlNewProp(skill,"upto",buf);
    xmlAddChild(skillv,skill);
  }

  return skillv;
}

static xmlNodePtr makeMobProg(struct mob_prog_list *op,xmlDocPtr doc)
{
  char buf[4096]; 
  struct side_term_list *st;
  struct mob_action_list *ac;

  xmlNodePtr prog = xmlNewNode(NULL,"mobprog"),xst,xac;
  prog->doc = doc;           

  xmlNewProp(prog,"main",(char *)mt_file[op->main]);

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

    xmlNewProp(xst,"number",(char *)st_file[st->number]);
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

    xmlNewProp(xac,"number",act_file[ac->action]);
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
      xmlNewProp(xac,"argument",ac->argument);
    }

    xmlAddChild(prog,xac);
    ac = ac->next;
  }     

  return prog;
} 

static xmlNodePtr mobToXml(struct char_data *mob, xmlDocPtr doc)
{
  char *p,buf[1024];
  struct mob_prog_list *mp;
  struct trig_proto_list *trig;
  xmlNodePtr xmob = xmlNewNode(NULL,"mobile"),tmp;
  xmob->doc = doc;

  sprintf(buf,"%ld",mob_index[mob->nr].virtual);
  xmlNewProp(xmob,"vnum",buf);
  xmlNewProp(xmob,"name",mob->player.name);

  xmlNewProp(xmob,"special",(NULL == GET_MOB_SPEC(mob) ? "NONE" : (NULL == 
                                   getSpecialPointer(GET_MOB_SPEC(mob)) 
                                   ? "NONE" : getSpecialPointer(GET_MOB_SPEC(mob)))));

  xmlNewChild(xmob,NULL,"description",(p = internalXmlString(mob->player.description,doc)));
  if(p) free(p);
  xmlNewChild(xmob,NULL,"shortdesc",(p = internalXmlString(mob->player.short_descr,doc)));
  if(p) free(p);
  xmlNewChild(xmob,NULL,"longdesc",(p = internalXmlString(mob->player.long_descr,doc)));
  if (p) free(p);
  xmlNewChild(xmob,NULL,"title",(p = internalXmlString(mob->player.title,doc)));
  if (p) free(p);

  xmlAddChild(xmob,makeCharData(mob,doc));
  xmlAddChild(xmob,makeAdata(mob,doc));
  xmlAddChild(xmob,makeStats(mob,doc));

  sprintf(buf,"%ld",mob->char_specials.saved.act); 
  tmp = xmlNewNode(NULL,"act");
  tmp->doc = doc;
  xmlNewProp(tmp,"flag",buf);
  xmlAddChild(xmob,tmp);

  tmp = xmlNewNode(NULL,"affected");
  tmp->doc = doc;
  sprintf(buf,"%ld",mob->char_specials.saved.affected_by);
  xmlNewProp(tmp,"one",buf);
  sprintf(buf,"%ld",mob->char_specials.saved.affected2);
  xmlNewProp(tmp,"two",buf);
  sprintf(buf,"%ld",mob->char_specials.saved.affected3);
  xmlNewProp(tmp,"three",buf);
  sprintf(buf,"%ld",mob->char_specials.saved.affected4);
  xmlNewProp(tmp,"four",buf);
  xmlAddChild(xmob,tmp);

  tmp = xmlNewNode(NULL,"attack");
  tmp->doc = doc;
  sprintf(buf,"%ld",mob->mob_specials.attack_type);
  xmlNewProp(tmp,"type",buf);
  xmlAddChild(xmob,tmp);

  tmp = xmlNewNode(NULL,"pos");
  tmp->doc = doc;
  xmlNewProp(tmp,"default",posInt2Str(mob->mob_specials.default_pos));
  xmlNewProp(tmp,"other",posInt2Str(mob->mob_specials.default_pos)); // ??
  xmlAddChild(xmob,tmp);

  tmp = xmlNewNode(NULL,"damage");
  tmp->doc = doc;
  sprintf(buf,"%d",(int)mob->mob_specials.damnodice);
  xmlNewProp(tmp,"num",buf);
  sprintf(buf,"%d",(int)mob->mob_specials.damsizedice);
  xmlNewProp(tmp,"sides",buf);
  sprintf(buf,"%ld",mob->points.hitroll);
  xmlNewProp(tmp,"add",buf);
  xmlAddChild(xmob,tmp);
  
  tmp = xmlNewNode(NULL,"timer");
  tmp->doc = doc;
  sprintf(buf,"%ld",mob->char_specials.timer);
  xmlNewProp(tmp,"amount",buf);
  xmlAddChild(xmob,tmp);

  tmp = xmlNewNode(NULL,"skills");
  tmp->doc = doc;
  sprintf(buf,"%ld",mob->mob_specials.skillvector);
  xmlNewProp(tmp,"vector",buf);
  xmlAddChild(xmob,tmp);

  xmlAddChild(xmob,buildSkillVector(mob,doc));

  if (NULL != (mp = mob->mprog)) {
    while (NULL != mp) {
      xmlAddChild(xmob,makeMobProg(mp,doc));
      mp = mp->next;
    }
  }

  /* DGS */
  trig = mob->proto_script;
  while (trig)
  {
   xmlAddChild(xmob, makeDGScript(trig, doc));
   trig = trig->next;
  }

  return xmob;
}

void mobiles_save_zone_to_file(int start,int stop,FILE *f)
{
  int i, rr;
  xmlNodePtr tree,mob;
  xmlDocPtr doc;
  
  doc = xmlNewDoc("1.0");

  tree = xmlNewDocNode(doc,NULL,"mobiles",NULL);
  doc->children = tree;

  for (i = start; i <= stop; i++) {     
    if (-1 != (rr = real_mobile(i))) {
      mob = mobToXml((mob_proto+rr),doc);
      xmlAddChild(tree,mob);
    }
  }
  xmlDocDump(f,doc);
  xmlFreeDoc(doc);
}

int count_xml_mobileentries(FILE *f)
{  
  char bf[25000],*u;
  int cnt = 0;

  while (fgets(bf,25000,f) && !feof(f)) {
    bf[(strlen(bf)-1)] = '\0';
    u = strupper(bf);
    
    if (NULL != strstr(u,"</MOBILE>")) {
      cnt++;
    }
    
    free(u);
  }

  return cnt;                
}

ACMD(test_xmlmob)
{
  FILE *test = fopen("/home/mud/stefans/cvs/immzone2.xml","w"); 

  if (NULL != test) {
    mobiles_save_zone_to_file(32600,32699,test);
    fclose(test);

    test = fopen("/home/mud/stefans/cvs/immzone2.xml","r");
    asend_to_char(ch,"Records in file: %d\r\n",count_xml_mobileentries(test));

    fclose(test);
    return;
  }
  send_to_char("Couldn't open file!\r\n",ch);  
}
