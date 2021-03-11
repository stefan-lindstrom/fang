/******************************************************************************
 * $Id: portstone.c,v 1.3 2002/11/07 04:02:04 fang Exp $
 *
 * File: portstones.h
 *
 * Created: Fri May 24 2002 by Stefan Lindstrom (asmodean@ludd.luth.se)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada,
 * yada...
 *
 * Attempt at a more dynamic portal stone systems. Portal stones are stored in
 * files which are read at bootup. New stones can be added, and old edited/deleted
 * online with an OLC like system.
 *
 *   $Log: portstone.c,v 
 *   Revision 1.2  2002/07/28 13:56:05  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.7  2002/06/06 08:04:08  fang
 *   *** empty log message ***
 *
 *   Revision 1.6  2002/06/04 09:36:31  fang
 *   *** empty log message ***
 *
 *   Revision 1.5  2002/06/04 09:20:01  fang
 *   More portstone work
 *
 *   Revision 1.4  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *
 *   Revision 1.2  2002/05/24 13:07:04  fang
 *   more stone stuff...
 *
 *   Revision 1.1  2002/05/24 11:39:52  fang
 *   First commit
 *
 *
 ******************************************************************************/
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/entities.h>
#include <libxml/parserInternals.h>
#include <errno.h>

#include "dll.h"
#include "utils.h"
#include "comm.h"
#include "db.h"
#include "spells.h"
#include "strutil.h"
#include "handler.h"
#include "interpreter.h"
#include "fight.h"
#include "act.h"

#include "portstone.h"


struct stone {
  stone_num num;
  room_num  in_room;      /* VNUM of room stone is placed in!            */
  char     *name;         /* If other than room name, short name, etc... */
  DLL       destinations;
};

static DLL __stones;

int stones_start(void)
{
  return first(__stones);
}

int stones_next(void)
{
  return next(__stones);
}

stone_t *stones_this(void)
{
  return this(__stones);
}

stone_t *create_stone(void)
{
  stone_t *rc = calloc(1,sizeof(stone_t));
  rc->destinations = createDll();
  
  rc->num     = -1;
  rc->in_room = -1;
  rc->name    = strdup("Unfinished portal stone");

  return rc;
}

int st_destination_start(stone_t *s)
{
  return first(s->destinations);
}

int st_destination_next(stone_t *s)
{
  return next(s->destinations);
}

int st_destination_this(stone_t *s)
{
  return (int)this(s->destinations);
}

void add_stone(stone_t *st)
{
  return append(__stones,st);
}

void st_add_destination(stone_t *s, int destroom)
{
  append(s->destinations,(void *)destroom);
}

void st_set_name(stone_t *st, const char *name)
{
  if (st->name)
    free(st->name);
  st->name = strdup(name);
}

void st_set_stonenum(stone_t *st, int num)
{
  st->num = num;
}

void st_set_in_room(stone_t *s, int room)
{
  s->in_room = room;
}

static int bynum(ELEMENT stone, ELEMENT stonenum)
{
  int      snum = (int)stonenum;
  stone_t *st   = (stone_t *)stone;

  if (st->num == snum)
    return 1;

  return 0;
}

static int byname(ELEMENT stone, ELEMENT sname)
{
  char *name = sname;
  stone_t *st   = (stone_t *)stone;

  return (strcmp(st->name, name) ? 0 : 1);
}

static int bydest(ELEMENT D1, ELEMENT D2)
{
  int d1 = (int)D1, d2 = (int)D2;

  if (d1 == d2)
    return 1;
  return 0;
}

void stone_free(stone_t *s)
{
  if (s->name)
    free(s->name);
  free(s);
}

static void i_stone_free(ELEMENT e)
{
  stone_free(e);
}

void delete_stone_num(int stonenum)
{
  stone_t *s;

  if (!search(__stones,(void *)stonenum,bynum)) {
    s = this(__stones);

    if (world[real_room(s->in_room)].func == new_stone)
      world[real_room(s->in_room)].func = NULL;

    deleteCurrentElement(__stones, i_stone_free);
  }
}

void delete_stone_name(const char *name)
{
  stone_t *s;

  if (!search(__stones,(char *)name,byname)) {
    s = this(__stones);

    if (world[real_room(s->in_room)].func == new_stone)
      world[real_room(s->in_room)].func = NULL;

    deleteCurrentElement(__stones, i_stone_free);
  }
}

static void nodel(ELEMENT e) {}

void st_delete_destination(stone_t *s,int dest)
{
  if (!search(s->destinations,(void *)dest,bydest))
    deleteCurrentElement(s->destinations, nodel);
}

void destination_list(struct char_data *ch)
{
  stone_t *stn;
  int i,ri;

  stn = find_stone_by_roomnum(world[ch->in_room].number);
  if (NULL == stn) {
    asend_to_char(ch,"There is no stone in this room!\r\n");
    return;
  }

  gen_message(PORTAL_LIST,ch,NULL,FALSE,TRUE);
  ADD_MANA(ch, -100);

  asend_to_char(ch,"This stone will take you to: \r\n");

  ri = first(stn->destinations);
  if (!ri) {
    asend_to_char(ch,"This stone has no destinations set!\r\n");
    return;
  }

  ri = 0;
  do {
    i = (int)this(stn->destinations);
    if (NOWHERE != (ri = real_room(i))) { 
      asend_to_char(ch,"%s\r\n",world[ri].name);
    }
  } while(next(stn->destinations));
}

static int stone_linked_with(stone_t *src, const stone_t *dest)
{
  int vroom;
  
  if (!first(src->destinations))
    return 0;

  do {
    vroom = (int)this(src->destinations);
    if (vroom == dest->in_room)
      return 1;
  } while (next(src->destinations));

  return 0;
}
  
void travel_to(struct char_data *ch, const char *deststr)
{
  stone_t *src  = find_stone_by_roomnum(world[ch->in_room].number);
  stone_t *dest = find_stone_by_roomname(deststr);
  struct char_data *list, *next;

  if (NULL == src) {
    send_to_char("There's no portal stone in this room!\r\n",ch);
    return;
  }

  if (NULL == dest) {
    send_to_char("That destiantion does -not- exist!\r\n",ch);
    return;
  }

  if (!stone_linked_with(src,dest)) {
    send_to_char("That is not a valid destination!\r\n",ch);
  }

  if (IS_AFFECTED(ch,AFF_GROUP)) {
    for (list = world[ch->in_room].people; list; list = next) {
      next = list->next_in_room;

      if (is_in_group(ch,list) && (list != ch)) {
	gen_message(PORTAL_TRANSF_GM,ch,list,TRUE,TRUE);
	char_from_room(list);
	char_to_room(list,real_room(dest->in_room));
	do_look(list,"",0,0);
	ADD_MANA(ch, -200);
      }
    }
  }
  gen_message(PORTAL_TRANSF_SELF,ch,ch,TRUE,TRUE);
  char_from_room(ch);
  char_to_room(ch,real_room(dest->in_room));
  do_look(ch,"",0,0);
  ADD_MANA(ch, -200);
}

stone_t *find_stone_by_roomnum(room_num vnum)
{
  if (!first(__stones))
    return NULL;

  do {
    if (((stone_t *)this(__stones))->in_room == vnum)
      return this(__stones);
  } while (next(__stones));
  
  return NULL;
}

static int part_match(char *word,char *word_list)
{
  char *wU,*wlU;
  int isIn = 0;

  skip_spaces(&word);
  wU  = strupper(word);
  wlU = strupper(word_list);

  isIn = (NULL == strstr(wlU,wU) ? 0 : 1);

  free(wU); free(wlU);

  return isIn;
}

stone_t *find_stone_by_roomname(const char *namelist)
{
  stone_t *stn;
  int submatch,j,num,skip;
  char **seek;
  char *arg = strdup(namelist);


  skip = (get_number(&arg) - 1);
  seek = split(arg,' ',&num);  

  if (!first(__stones))
    return NULL;

  do {
    stn = (stone_t *)this(__stones);
    submatch = 1;
    for (j = 0; j < num; j++) {
      submatch &= part_match(seek[j],world[real_room(stn->in_room)].name);
    }
    if (submatch && !skip) {
      for (j = 0; j < num; j++)
    free(seek[j]);
      free(seek);
      free(arg);
      return stn;
    }
    else if (submatch && skip)
      skip--;
  } while (next(__stones));

  free(arg);
  return NULL;
}

int read_stones(void)
{
  xmlDocPtr doc;
  xmlNodePtr stone,tmp1,tmp2;
  int rr;
  stone_t *st;
  __stones = createDll();

  if (!(doc = xmlParseFile(PORT_FILE))) {
    alog("Could not parse portal stone file \"%s\"",PORT_FILE);
    return 0;
  }

  if (!doc->children || !strcasecmp("stones",doc->children->name)) {
    alog("Document root in portal stone file %s!",doc->children ? 
     "is not named \"stones\"" : "does not exist");
    xmlFreeDoc(doc);
    return 0;
  }

  for (stone = doc->children; NULL != stone; stone = stone->next) {
    st = calloc(1,sizeof(stone_t));
    st->in_room = xmlAtoi(xmlGetProp(stone,"inroom"));
    st->num     = xmlAtoi(xmlGetProp(stone,"num"));
    st->name    = xmlGetProp(stone,"stonename");
    st->destinations = createDll();

    for (tmp1 = stone->children; tmp1 != NULL; tmp1 = tmp1->next) {
      if (!strcasecmp(tmp1->name,"destinations") && tmp1->children) {
    for (tmp2 = tmp1->children; NULL != tmp2; tmp2 = tmp2->next) {
      if (!strcasecmp(tmp2->name,"destination")) {
        append(st->destinations,(void *)xmlAtoi(xmlGetProp(tmp2,"vroom")));
      }
    }
      }
    }
    if (NOWHERE != (rr = real_room(st->in_room))) {
      append(__stones,st);
      world[rr].func = new_stone;
    } else {
      stone_free(st);
    }
  }

  return 1;
}

static xmlNodePtr stone_to_XML(xmlDocPtr doc, stone_t *st)
{
  xmlNodePtr stone,temp,temp2;
  char buf[500];
  int dest;

  stone = xmlNewNode(NULL,"stone");
  stone->doc = doc;

  sprintf(buf,"%ld",st->num);
  xmlNewProp(stone,"num",buf);

  sprintf(buf,"%d",st->in_room);
  xmlNewProp(stone,"inroom",buf);

  xmlNewProp(stone,"stonename",st->name);
  temp = xmlNewChild(stone,NULL,"destinations",NULL);

  if (first(st->destinations)) {
    do {
      dest  = (int)this(st->destinations);
      temp2 = xmlNewChild(temp,NULL,"destination",NULL);
      sprintf(buf,"%d",dest);
      xmlNewProp(temp2,"vroom",buf);
    } while(next(st->destinations));
  }
  return stone;
}

int save_stones(void)
{
  char filen[100], filenb[100];
  xmlDocPtr doc;
  xmlNodePtr stones;
  int rc;

  sprintf(filen,"%s",PORT_FILE);
  sprintf(filenb,"%s.bak",filen);

  doc = xmlNewDoc("1.0");
  stones = xmlNewDocNode(doc,NULL,"stones",NULL);
  doc->children = stones;

  if (first(__stones)) {
    do {
      xmlAddChild(stones,stone_to_XML(doc,this(__stones)));
    } while (next(__stones));
  }

  rc = xmlSaveFile(filenb,doc);
  xmlFreeDoc(doc);

  if (-1 != rc) {
    remove(filen); 
    rename(filenb,filen);
  } else {
    alog("Could not save stones file, reason: %s",strerror(errno));
    remove(filenb);
    return 0;
  }
  return 1;
}

void st_list_wiz(struct char_data *ch)
{
  stone_t *st;
  int dest,cnt;

  if (first(__stones)) {
    do {
      st = this(__stones);
      asend_to_char(ch,"Stone [vnum: %5d] %s, in_room: %5d\r\n",st->num,st->name,st->in_room);
      asend_to_char(ch,"Destination list (room vnums): \r\n");

      *buf = '\0';
      
      cnt = 1;
      if (first(st->destinations)) {
    do {
      dest = (int)this(st->destinations);
      asend_to_char(ch,"%5d ",dest);
      if (!(cnt % 10)) {
        asend_to_char(ch,"\r\n");
        cnt = 0;
      }
      cnt++; 
    } while(next(st->destinations));
      }
      if (cnt % 10)
    asend_to_char(ch,"\r\n");

      asend_to_char(ch,"---------------------------------------------------------------\r\n");
    } while (next(__stones));
  }
}


ACMD(do_pstest)
{
  st_list_wiz(ch);
  delete_stone_num(2);
  st_list_wiz(ch);
  delete_stone_name("Test 3");
  st_list_wiz(ch);
  first(__stones);
  st_delete_destination(this(__stones),32603);
  st_list_wiz(ch);
}

SPECIAL(new_stone)
{
  return 0;
}
