/**************************************************************************
*  File: scripts.c                                                        *
*  Usage: contains general functions for using scripts.                   *
*                                                                         *
*                                                                         *
*  $Author: fang $
*  $Date: 2003/03/25 07:58:50 $
*  $Revision: 1.5 $
**************************************************************************/

#include <gnome-xml/parser.h>
#include <gnome-xml/tree.h>
#include <gnome-xml/entities.h>
#include <gnome-xml/parserInternals.h>
#include <string.h>
#include <stdio.h>

#include "act.h"
#include "constants.h"
#include "structs.h"
#include "dg_scripts.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "dg_event.h"
#include "db.h"
#include "screen.h"
#include "xmlcharsave.h"
#include "class.h"
#include "spells.h"

#define PULSES_PER_MUD_HOUR     (SECS_PER_MUD_HOUR*PASSES_PER_SEC)

/* local structures */
struct wait_event_data {
  trig_data *trigger;
  void *go;
  int type;
};


struct trig_data *trigger_list = NULL;  /* all attached triggers */

/* Return pointer to first occurrence in string ct in */
/* cs, or NULL if not present.  Case insensitive */
char *str_str(char *cs, char *ct)
{
  char *s, *t;

  if (!cs || !ct)
    return NULL;

  while (*cs) {
    t = ct;

    while (*cs && (LOWER(*cs) != LOWER(*t)))
      cs++;
 
    s = cs;
 
    while (*t && *cs && (LOWER(*cs) == LOWER(*t))) {
      t++;
      cs++;
    }

    if (!*t)
      return s;
  }

  return NULL;
}


int trgvar_in_room(int vnum) {
    int rnum = real_room(vnum);
    int i = 0;
    char_data *ch;

    if (NOWHERE == rnum) {
    script_log("people.vnum: world[rnum] does not exist");
    return (-1);
    }

    for (ch = world[rnum].people; ch !=NULL; ch = ch->next_in_room)
    i++;

    return i;
}

obj_data *get_obj_in_list(char *name, obj_data *list)
{
    obj_data *i;
    long id;
    
    if (*name == UID_CHAR)
    {
        id = atoi(name + 1);
     
        for (i = list; i; i = i->next_content)
            if (id == GET_ID(i))
                return i;
    }
    else
    {
        for (i = list; i; i = i->next_content)
            if (isname(name, i->name))
                return i;
    }
        
    return NULL;
}

obj_data *get_object_in_equip(char_data * ch, char *name)
{
    int j, n = 0, number;
    obj_data *obj;
    char tmpname[MAX_INPUT_LENGTH];
    char *tmp = tmpname; 
    long id;

    if (*name == UID_CHAR)
    {
        id = atoi(name + 1);
            
        for (j = 0; j < NUM_WEARS; j++)
            if ((obj = GET_EQ(ch, j)))
                if (id == GET_ID(obj))
                    return (obj);
    }
    else
    {
        strcpy(tmp, name);
        if (!(number = get_number(&tmp)))
            return NULL;

        for (j = 0; (j < NUM_WEARS) && (n <= number); j++)
            if ((obj = GET_EQ(ch, j)))
                if (isname(tmp, obj->name))
                    if (++n == number)
                        return (obj);
    }
    
    return NULL;
}

/************************************************************
 * search by number routines
 ************************************************************/
 
/* return char with UID n */
struct char_data *find_char(long n)
{
  struct char_data *ch;

  for (ch = character_list; ch; ch=ch->next)
  {
    if (GET_ID(ch)==n) return (ch);
  }
 
  return NULL;
}


/* return object with UID n */
obj_data *find_obj(long n)
{
    obj_data *i;
    
    for (i = object_list; i; i = i->next)
        if (n == GET_ID(i))
            return i;
 
    return NULL;
}

/* return room with UID n */
room_data *find_room(long n)
{
    n -= ROOM_ID_BASE;
    
    if ((n >= 0) && (n <= top_of_world))
        return &world[n];

    return NULL;
}



/************************************************************
 * generic searches based only on name
 ************************************************************/

/* search the entire world for a char, and return a pointer */
char_data *get_char(char *name)
{
    char_data *i;

    if (*name == UID_CHAR)
    {
        i = find_char(atoi(name + 1));

        if (i && (IS_NPC(i) || !GET_INVIS_LEV(i)))
            return i;
    }
    else
    {
        for (i = character_list; i; i = i->next)
            if (isname(name, GET_NAME(i)) &&
                (IS_NPC(i) || !GET_INVIS_LEV(i)))
                return i;
    }

    return NULL;
}


/* returns the object in the world with name name, or NULL if not found */
obj_data *get_obj(char *name)  
{
    obj_data *obj;
    long id;
    
    if (*name == UID_CHAR)
    {
        id = atoi(name + 1);
        
        for (obj = object_list; obj; obj = obj->next)
            if (id == GET_ID(obj))
                return obj;
    }      
    else
    {
        for (obj = object_list; obj; obj = obj->next)
            if (isname(name, obj->name))
                return obj;
    }

    return NULL;
}
 

/* finds room by with name.  returns NULL if not found */
room_data *get_room(char *name)
{
    int nr; 

    if (*name == UID_CHAR)
        return find_room(atoi(name + 1));
    else if ((nr = real_room(atoi(name))) == NOWHERE)
        return NULL;
    else
        return &world[nr];
}


/*
 * returns a pointer to the first character in world by name name,
 * or NULL if none found.  Starts searching with the person owing the object
 */
char_data *get_char_by_obj(obj_data *obj, char *name)
{
    char_data *ch;

    if (*name == UID_CHAR)
    {
        ch = find_char(atoi(name + 1));
            
        if (ch && (IS_NPC(ch) || !GET_INVIS_LEV(ch)))
            return ch;
    }
    else
    {
        if (obj->carried_by &&
            isname(name, GET_NAME(obj->carried_by)) &&
            (IS_NPC(obj->carried_by) || !GET_INVIS_LEV(obj->carried_by)))
            return obj->carried_by;
     
        if (obj->worn_by &&
            isname(name, GET_NAME(obj->worn_by)) &&
            (IS_NPC(obj->worn_by) || !GET_INVIS_LEV(obj->worn_by)))
            return obj->worn_by;
     
        for (ch = character_list; ch; ch = ch->next)
            if (isname(name, GET_NAME(ch)) &&
                (IS_NPC(ch) || !GET_INVIS_LEV(ch)))
                return ch;
    }
        
    return NULL;
}
            
                
/*
 * returns a pointer to the first character in world by name name,
 * or NULL if none found.  Starts searching in room room first
 */
char_data *get_char_by_room(room_data *room, char *name)
{    
    char_data *ch;

    if (*name == UID_CHAR)
    {
        ch = find_char(atoi(name + 1));
 
        if (ch && (IS_NPC(ch) || !GET_INVIS_LEV(ch)))
            return ch;
    }
    else
    {
        for (ch = room->people; ch; ch = ch->next_in_room)
            if (isname(name, GET_NAME(ch)) &&
                (IS_NPC(ch) || !GET_INVIS_LEV(ch)))
                return ch;
        
        for (ch = character_list; ch; ch = ch->next)
            if (isname(name, GET_NAME(ch)) &&
                (IS_NPC(ch) || !GET_INVIS_LEV(ch)))
                return ch;
    }
            
    return NULL;
}

/*
 * copy of routine above, but without an Invis check
 * so invis chars can get messages.
 */

char_data *get_char_by_room_winvis(room_data *room, char *name)
{    
    char_data *ch;

    if (*name == UID_CHAR)
    {
        ch = find_char(atoi(name + 1));
 
        if (ch && (IS_NPC(ch)))
            return ch;
    }
    else
    {
        for (ch = room->people; ch; ch = ch->next_in_room)
            if (isname(name, GET_NAME(ch)) &&
                (IS_NPC(ch)))
                return ch;
        
        for (ch = character_list; ch; ch = ch->next)
            if (isname(name, GET_NAME(ch)) &&
                (IS_NPC(ch)))
                return ch;
    }
            
    return NULL;
}


/*
 * returns the object in the world with name name, or NULL if not found
 * search based on obj
 */  
obj_data *get_obj_by_obj(obj_data *obj, char *name)
{
    obj_data *i = NULL;  
    int rm;
    long id;

    if (!str_cmp(name, "self") || !str_cmp(name, "me"))
        return obj;
    
    if (obj->contains && (i = get_obj_in_list(name, obj->contains)))
        return i;
    
    if (obj->in_obj)
    {
        if (*name == UID_CHAR)
        {
            id = atoi(name + 1);
        
            if (id == GET_ID(obj->in_obj))
                return obj->in_obj;
        }
        else if (isname(name, obj->in_obj->name))
            return obj->in_obj;
    }   
     
    else if (obj->worn_by && (i = get_object_in_equip(obj->worn_by, name)))
        return i;
    else if (obj->carried_by &&
             (i = get_obj_in_list(name, obj->carried_by->carrying)))
        return i;
    else if (((rm = obj_room(obj)) != NOWHERE) &&
             (i = get_obj_in_list(name, world[rm].contents)))
        return i;
                        
    if (*name == UID_CHAR)
    {           
        id = atoi(name + 1);
 
        for (i = object_list; i; i = i->next)
            if (id == GET_ID(i))
                break;
    }

    else
    {
        for (i = object_list; i; i = i->next)
            if (isname(name, i->name))
                break;
    }

    return i;
}   

        
/* returns obj with name */
obj_data *get_obj_by_room(room_data *room, char *name)
{
    obj_data *obj;
    long id;
     
    if (*name == UID_CHAR)
    {
        id = atoi(name + 1);
        
        for (obj = room->contents; obj; obj = obj->next_content)
            if (id == GET_ID(obj)) 
                return obj;
        
        for (obj = object_list; obj; obj = obj->next)
            if (id == GET_ID(obj))
                return obj;
    }
    else
    {
        for (obj = room->contents; obj; obj = obj->next_content)
            if (isname(name, obj->name))
                return obj;
             
        for (obj = object_list; obj; obj = obj->next)
            if (isname(name, obj->name))
                return obj;
    }           
        
    return NULL;
}



/* checks every PLUSE_SCRIPT for random triggers */
void script_trigger_check(void)
{
  char_data *ch;
  obj_data *obj;
  struct room_data *room=NULL;
  int nr;
  struct script_data *sc;

  for (ch = character_list; ch; ch = ch->next) {
    if (SCRIPT(ch)) {
      sc = SCRIPT(ch);

      if (IS_SET(SCRIPT_TYPES(sc), WTRIG_RANDOM) &&
      (!is_empty(world[IN_ROOM(ch)].zone) ||
       IS_SET(SCRIPT_TYPES(sc), WTRIG_GLOBAL)))
    random_mtrigger(ch);
    }
  }
  
  for (obj = object_list; obj; obj = obj->next) {
    if (SCRIPT(obj)) {
      sc = SCRIPT(obj);

      if (IS_SET(SCRIPT_TYPES(sc), OTRIG_RANDOM))
    random_otrigger(obj);
    }
  }

  for (nr = 0; nr <= top_of_world; nr++) {
    if (SCRIPT(&world[nr])) {
      room = &world[nr];
      sc = SCRIPT(room);
      
      if (IS_SET(SCRIPT_TYPES(sc), WTRIG_RANDOM) &&
      (!is_empty(room->zone) ||
       IS_SET(SCRIPT_TYPES(sc), WTRIG_GLOBAL)))
    random_wtrigger(room);
    }
  }
}


EVENT(trig_wait_event)
{
  struct wait_event_data *wait_event_obj = (struct wait_event_data *)info;
  trig_data *trig;
  void *go;
  int type;

  trig = wait_event_obj->trigger;
  go = wait_event_obj->go;
  type = wait_event_obj->type;

  free(wait_event_obj);  
  GET_TRIG_WAIT(trig) = NULL;

  script_driver(go, trig, type, TRIG_RESTART);
}


void do_stat_trigger(struct char_data *ch, trig_data *trig)
{
    struct cmdlist_element *cmd_list;
    char sb[MAX_STRING_LENGTH];

    if (!trig)
    {
    alog("SYSERR: NULL trigger passed to do_stat_trigger.");
    return;
    }

    sprintf(sb, "&cName:&n '%s%s%s',  &cVNum:&n [%s%5ld%s], &cRNum:&n [%5d]\r\n",
          CCYEL(ch, C_NRM), GET_TRIG_NAME(trig), CCNRM(ch, C_NRM),
          CCGRN(ch, C_NRM), GET_TRIG_VNUM(trig), CCNRM(ch, C_NRM),
          GET_TRIG_RNUM(trig));

    if (trig->attach_type==OBJ_TRIGGER) {
      send_to_char("&cTrigger Intended Assignment:&n Objects\r\n", ch);
      sprintbit(GET_TRIG_TYPE(trig), otrig_types, buf);
    } else if (trig->attach_type==WLD_TRIGGER) {
      send_to_char("&cTrigger Intended Assignment:&n Rooms\r\n", ch);
      sprintbit(GET_TRIG_TYPE(trig), wtrig_types, buf);
    } else {
      send_to_char("&cTrigger Intended Assignment:&n Mobiles\r\n", ch);
      sprintbit(GET_TRIG_TYPE(trig), trig_types, buf);
    }
    
    sprintf(sb, "&cTrigger Type:&n %s, &cNumeric Arg:&n %d, &cArg list:&n %s\r\n",
          buf, GET_TRIG_NARG(trig), 
          ((GET_TRIG_ARG(trig) && *GET_TRIG_ARG(trig))
        ? GET_TRIG_ARG(trig) : "None"));

    strcat(sb,"&wCommands:&n\r\n   ");

    cmd_list = trig->cmdlist;
    while (cmd_list)
    {
    if (cmd_list->cmd)
    {
        strcat(sb,cmd_list->cmd);
        strcat(sb,"\r\n   ");
    }

    cmd_list = cmd_list->next;
    }

    page_string(ch->desc, sb, 1);
}


/* find the name of what the uid points to */
void find_uid_name(char *uid, char *name)
{
  char_data *ch;
  obj_data *obj;

  if ((ch = get_char(uid)))
    strcpy(name, GET_NAME(ch));
  else if ((obj = get_obj(uid)))
    strcpy(name, obj->name);
  else
    sprintf(name, "uid = %s, (not found)", uid + 1);
}


/* general function to display stats on script sc */
void script_stat (char_data *ch, struct script_data *sc)
{
  struct trig_var_data *tv;
  trig_data *t;
  char name[MAX_INPUT_LENGTH];
  char namebuf[512];

  sprintf(buf, "&WGlobal Variables:&n %s\r\n", sc->global_vars ? "" : "None");
  send_to_char(buf, ch);
  sprintf(buf, "&WGlobal context:&n %ld\r\n", sc->context);
  send_to_char(buf, ch);

  for (tv = sc->global_vars; tv; tv = tv->next) {
    sprintf(namebuf,"%s:%ld", tv->name, tv->context);
    if (*(tv->value) == UID_CHAR) {
      find_uid_name(tv->value, name);
      sprintf(buf, "    %15s:  %s\r\n", tv->context?namebuf:tv->name, name);
    } else 
      sprintf(buf, "    %15s:  %s\r\n", tv->context?namebuf:tv->name,
              tv->value);
    send_to_char(buf, ch);
  }

  for (t = TRIGGERS(sc); t; t = t->next) {
    sprintf(buf, "\r\n&cTrigger:&n %s%s%s, &cVNum:&n [%s%5ld%s], &cRNum:&n [%5d]\r\n",
        CCYEL(ch, C_NRM), GET_TRIG_NAME(t), CCNRM(ch, C_NRM),
        CCGRN(ch, C_NRM), GET_TRIG_VNUM(t), CCNRM(ch, C_NRM),
        GET_TRIG_RNUM(t));
    send_to_char(buf, ch);

    if (t->attach_type==OBJ_TRIGGER) {
      send_to_char("&cTrigger Intended Assignment:&n Objects\r\n", ch);
      sprintbit(GET_TRIG_TYPE(t), otrig_types, buf1);
    } else if (t->attach_type==WLD_TRIGGER) {
      send_to_char("&cTrigger Intended Assignment:&n Rooms\r\n", ch);
      sprintbit(GET_TRIG_TYPE(t), wtrig_types, buf1);
    } else {
      send_to_char("&cTrigger Intended Assignment:&n Mobiles\r\n", ch);
      sprintbit(GET_TRIG_TYPE(t), trig_types, buf1);
    }
    
    sprintf(buf, "&cTrigger Type:&n %s, &cNumeric Arg:&n %d, &cArg list:&n %s\r\n", 
        buf1, GET_TRIG_NARG(t), 
        ((GET_TRIG_ARG(t) && *GET_TRIG_ARG(t)) ? GET_TRIG_ARG(t) :
         "None"));
    send_to_char(buf, ch);

    if (GET_TRIG_WAIT(t)) {
      sprintf(buf, "&cWait:&n %d, &cCurrent line:&n %s\r\n",
          GET_TRIG_WAIT(t)->time_remaining, t->curr_state->cmd);
      send_to_char(buf, ch);

      sprintf(buf, "&cVariables:&n %s\r\n", GET_TRIG_VARS(t) ? "" : "None");
      send_to_char(buf, ch);

      for (tv = GET_TRIG_VARS(t); tv; tv = tv->next) {
    if (*(tv->value) == UID_CHAR) {
      find_uid_name(tv->value, name);
      sprintf(buf, "    %15s:  %s\r\n", tv->name, name);
    } else 
      sprintf(buf, "    %15s:  %s\r\n", tv->name, tv->value);
    send_to_char(buf, ch);
      }
    }
  }  
}

void list_triggers_to_char(struct char_data *ch, struct script_data *sc)
{
  trig_data *t;

  sprintf(buf, "Triggers: &C");
  for (t = TRIGGERS(sc); t; t = t->next) 
   sprintf(buf, "%s%ld ", buf, GET_TRIG_VNUM(t));

  sprintf(buf, "%s&n\r\n", buf);
  send_to_char(buf, ch);
}

void do_sstat_room(struct char_data * ch)
{
  struct room_data *rm = &world[ch->in_room];

  if (!SCRIPT(rm)) {
    send_to_char("&CNo Script.&n\r\n", ch);
    return;
  }

  send_to_char("&C     Script information&n\r\n", ch);
  send_to_char("&R     ==================&n\r\n", ch);

  script_stat(ch, SCRIPT(rm));
}


void do_sstat_object(char_data *ch, obj_data *j)
{
  if (!SCRIPT(j)) {
    send_to_char("&CNo Script.&n\r\n", ch);
    return;
  }

  send_to_char("&C     Script information&n\r\n", ch);
  send_to_char("&R     ==================&n\r\n", ch);

  script_stat(ch, SCRIPT(j));
}


void do_sstat_character(char_data *ch, char_data *k)
{
  if (!SCRIPT(k)) {
    send_to_char("&CNo Script.&n\r\n", ch);
    return;
  }

  if (IS_NPC(k))
  {
   send_to_char("&C     Script information&n\r\n", ch);
   send_to_char("&R     ==================&n\r\n", ch);
   script_stat(ch, SCRIPT(k));
   if (SCRIPT_MEM(k))
   {
    struct script_memory *mem = SCRIPT_MEM(k);
    send_to_char("&cScript memory:&n\r\n &wRemember             Command&n\r\n", ch);
    while (mem)
    {
     struct char_data *mc = find_char(mem->id);
     if (!mc)
      send_to_char("&RCorrupted!&n\r\n", ch);
     else {
      if (mem->cmd)
       sprintf(buf,"  %-20.20s%s\r\n",GET_NAME(mc),mem->cmd);
      else
       sprintf(buf,"  %-20.20s <default>\r\n",GET_NAME(mc));
      send_to_char(buf, ch);
     }
     mem = mem->next;
    }
   }
  }
  else  /* PC */
  {
   struct trig_var_data *tv;
   char name[MAX_INPUT_LENGTH];
   void find_uid_name(char *uid, char *name);
   
   if (!k->script->global_vars)
   {
    send_to_char("&CNo Script.&n\r\n", ch);
    return;
   }

   send_to_char("&C     Script information&n\r\n", ch);
   send_to_char("&R     ==================&n\r\n", ch);
   send_to_char("Global Variables:\r\n", ch);

   /* currently, variable context for players is always 0, so it is */
   /* not displayed here. in the future, this might change */
   for (tv = k->script->global_vars; tv; tv = tv->next) {
     if (*(tv->value) == UID_CHAR) {
       find_uid_name(tv->value, name);
       sprintf(buf, "    %10s:  [UID]: %s\r\n", tv->name, name);
     } else
       sprintf(buf, "    %10s:  %s\r\n", tv->name, tv->value);
     send_to_char(buf, ch);
   }
  }
}

/*
 * adds the trigger t to script sc in in location loc.  loc = -1 means
 * add to the end, loc = 0 means add before all other triggers.
 */
void add_trigger(struct script_data *sc, trig_data *t, int loc)
{
  trig_data *i;
  int n;

  for (n = loc, i = TRIGGERS(sc); i && i->next && (n != 0); n--, i = i->next);

  if (!loc) {
          t->next = TRIGGERS(sc);
    TRIGGERS(sc) = t;
  } else if (!i)
    TRIGGERS(sc) = t;
  else {
    t->next = i->next;
    i->next = t;
  }

  SCRIPT_TYPES(sc) |= GET_TRIG_TYPE(t);

  t->next_in_world = trigger_list;
  trigger_list = t;
}


ACMD(do_attach) 
{
  char_data *victim;
  obj_data *object;
  trig_data *trig;
  char targ_name[MAX_INPUT_LENGTH], trig_name[MAX_INPUT_LENGTH];
  char loc_name[MAX_INPUT_LENGTH];
  int loc, room, tn, rn;


  argument = two_arguments(argument, arg, trig_name);
  two_arguments(argument, targ_name, loc_name);

  if (!*arg || !*targ_name || !*trig_name) {
    send_to_char("Usage: attach { mtr | otr | wtr } { trigger } { name } [ location ]\r\n", ch);
    return;
  }
  
  tn = atoi(trig_name);
  loc = (*loc_name) ? atoi(loc_name) : -1;
  
  if (is_abbrev(arg, "mtr")) {
    if ((victim = get_char_vis(ch, targ_name))) {
      if (IS_NPC(victim))  {
    
    /* have a valid mob, now get trigger */
        rn = real_trigger(tn);
    if ((rn >= 0) && (trig = read_trigger(rn))) {

      if (!SCRIPT(victim))
        CREATE(SCRIPT(victim), struct script_data, 1);
      add_trigger(SCRIPT(victim), trig, loc);
      
      sprintf(buf, "Trigger %d (%s) attached to %s.\r\n",
          tn, GET_TRIG_NAME(trig), GET_SHORT(victim));
      send_to_char(buf, ch);
    } else
      send_to_char("That trigger does not exist.\r\n", ch);
      } else
    send_to_char("Players can't have scripts.\r\n", ch);
    } else
      send_to_char("That mob does not exist.\r\n", ch);
  }

  else if (is_abbrev(arg, "otr")) {
    if ((object = get_obj_vis(ch, targ_name))) {
    
      /* have a valid obj, now get trigger */
      rn = real_trigger(tn);
      if ((rn >= 0) && (trig = read_trigger(rn))) {
    
      if (!SCRIPT(object))
        CREATE(SCRIPT(object), struct script_data, 1);
      add_trigger(SCRIPT(object), trig, loc);
      
      sprintf(buf, "Trigger %d (%s) attached to %s.\r\n",
          tn, GET_TRIG_NAME(trig), 
          (object->short_description ?
           object->short_description : object->name));
      send_to_char(buf, ch);
      } else
    send_to_char("That trigger does not exist.\r\n", ch);
    } else
      send_to_char("That object does not exist.\r\n", ch); 
  }

  else if (is_abbrev(arg, "wtr")) {
    if (isdigit(*targ_name) && !strchr(targ_name, '.')) {
      if ((room = find_target_room(ch, targ_name)) != NOWHERE) {
    
    /* have a valid room, now get trigger */
    rn = real_trigger(tn);
    if ((rn >= 0) && (trig = read_trigger(rn))) {

      if (!(world[room].script))
        CREATE(world[room].script, struct script_data, 1);
      add_trigger(world[room].script, trig, loc);
      
      sprintf(buf, "Trigger %d (%s) attached to room %d.\r\n",
          tn, GET_TRIG_NAME(trig), world[room].number);
      send_to_char(buf, ch);
    } else
      send_to_char("That trigger does not exist.\r\n", ch);
      }
    } else
      send_to_char("You need to supply a room number.\r\n", ch);
  }

  else
    send_to_char("Please specify 'mtr', otr', or 'wtr'.\r\n", ch);
}


/* adds a variable with given name and value to trigger */
void add_var(struct trig_var_data **var_list, char *name, char *value, long id)
{
  struct trig_var_data *vd;

  for (vd = *var_list; vd && str_cmp(vd->name, name); vd = vd->next);

  if (vd && (!vd->context || vd->context==id)) {
    free(vd->value);
    CREATE(vd->value, char, strlen(value) + 1);
  }

  else {
    CREATE(vd, struct trig_var_data, 1);
    
    CREATE(vd->name, char, strlen(name) + 1);
    strcpy(vd->name, name);
    
    CREATE(vd->value, char, strlen(value) + 1);

    vd->next = *var_list;
    vd->context = id;
    *var_list = vd;
  }

  strcpy(vd->value, value);
}


/*
 *  removes the trigger specified by name, and the script of o if
 *  it removes the last trigger.  name can either be a number, or
 *  a 'silly' name for the trigger, including things like 2.beggar-death.
 *  returns 0 if did not find the trigger, otherwise 1.  If it matters,
 *  you might need to check to see if all the triggers were removed after
 *  this function returns, in order to remove the script.
 */
int remove_trigger(struct script_data *sc, char *name)
{
  trig_data *i, *j;
  int num = 0, string = FALSE, n;
  char *cname;
      
        
  if (!sc)
    return 0;

  if ((cname = strstr(name,".")) || (!isdigit(*name)) ) {
    string = TRUE;
    if (cname) {
      *cname = '\0';
      num = atoi(name);
      name = ++cname;
    }
  } else
    num = atoi(name);
  
  for (n = 0, j = NULL, i = TRIGGERS(sc); i; j = i, i = i->next) {
    if (string) {
      if (isname(name, GET_TRIG_NAME(i)))
        if (++n >= num)
          break;
    }

    /* this isn't clean... */
    /* a numeric value will match if it's position OR vnum */
    /* is found. originally the number was position-only */
    else if (++n >= num)
      break;
    else if (trig_index[i->nr]->virtual == num)
      break;
  }
      
  if (i) {
    if (j) {
      j->next = i->next;
      extract_trigger(i);
    }
  
    /* this was the first trigger */
    else {
      TRIGGERS(sc) = i->next;
      extract_trigger(i);
    }
 
    /* update the script type bitvector */
    SCRIPT_TYPES(sc) = 0;
    for (i = TRIGGERS(sc); i; i = i->next)
      SCRIPT_TYPES(sc) |= GET_TRIG_TYPE(i);
 
    return 1;
  } else
    return 0; 
}     

ACMD(do_detach)
{  
  char_data *victim = NULL;
  obj_data *object = NULL;
  struct room_data *room;
  char arg1[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH], arg3[MAX_INPUT_LENGTH];
  char *trigger = 0;   
  int tmp;

  argument = two_arguments(argument, arg1, arg2);
  one_argument(argument, arg3);
  
  if (!*arg1 || !*arg2) {
    send_to_char("Usage: detach [ mob | object | room ] { target } { trigger |"
                 " 'all' }\r\n", ch);
    return;
  }
     
  if (!str_cmp(arg1, "room")) {
    room = &world[IN_ROOM(ch)];
    if (!SCRIPT(room))
      send_to_char("This room does not have any triggers.\r\n", ch);
    else if (!str_cmp(arg2, "all")) {
      extract_script(SCRIPT(room));
      SCRIPT(room) = NULL;
      send_to_char("All triggers removed from room.\r\n", ch);
    }
     
    else if (remove_trigger(SCRIPT(room), arg2)) {
      send_to_char("Trigger removed.\r\n", ch);
      if (!TRIGGERS(SCRIPT(room))) {
        extract_script(SCRIPT(room));
        SCRIPT(room) = NULL;
      }
    } else
      send_to_char("That trigger was not found.\r\n", ch);
  }
    
  else {
    if (is_abbrev(arg1, "mob")) {
      if (!(victim = get_char_vis(ch, arg2)))
        send_to_char("No such mobile around.\r\n", ch);
      else if (!arg3 || !*arg3)
        send_to_char("You must specify a trigger to remove.\r\n", ch);
      else
        trigger = arg3;
    }
   
    else if (is_abbrev(arg1, "object")) {
      if (!(object = get_obj_vis(ch, arg2)))
        send_to_char("No such object around.\r\n", ch);
      else if (!*arg3)
        send_to_char("You must specify a trigger to remove.\r\n", ch);
      else
        trigger = arg3;
    }
    else  {
      if ((object = get_object_in_equip_vis(ch, arg1, ch->equipment, &tmp)));
      else if ((object = get_obj_in_list_vis(ch, arg1, ch->carrying)));
      else if ((victim = get_char_room_vis(ch, arg1)));
      else if ((object = get_obj_in_list_vis(ch, arg1, world[IN_ROOM(ch)].contents)));
      else if ((victim = get_char_vis(ch, arg1)));
      else if ((object = get_obj_vis(ch, arg1)));
      else
        send_to_char("Nothing around by that name.\r\n", ch);
    
      trigger = arg2;
    }
      
    if (victim) {
      if (!IS_NPC(victim))
        send_to_char("Players don't have triggers.\r\n", ch);
     
      else if (!SCRIPT(victim))
        send_to_char("That mob doesn't have any triggers.\r\n", ch);
      else if (!str_cmp(arg2, "all")) {
        extract_script(SCRIPT(victim));
        SCRIPT(victim) = NULL;
        sprintf(buf, "All triggers removed from %s.\r\n", GET_SHORT(victim));
        send_to_char(buf, ch);
      }
   
      else if (trigger && remove_trigger(SCRIPT(victim), trigger)) {
        send_to_char("Trigger removed.\r\n", ch);
        if (!TRIGGERS(SCRIPT(victim))) {
          extract_script(SCRIPT(victim));
          SCRIPT(victim) = NULL;
        }
      } else
        send_to_char("That trigger was not found.\r\n", ch);
    }
     
    else if (object) {
      if (!SCRIPT(object))
        send_to_char("That object doesn't have any triggers.\r\n", ch);
        
      else if (!str_cmp(arg2, "all")) {
        extract_script(SCRIPT(object));
        SCRIPT(object) = NULL;
        sprintf(buf, "All triggers removed from %s.\r\n",
                object->short_description ? object->short_description :
                object->name);
        send_to_char(buf, ch);
      }
       
      else if (remove_trigger(SCRIPT(object), trigger)) {
        send_to_char("Trigger removed.\r\n", ch);
        if (!TRIGGERS(SCRIPT(object))) {
          extract_script(SCRIPT(object));
          SCRIPT(object) = NULL;
        }
      } else
        send_to_char("That trigger was not found.\r\n", ch);
    }
  }  
}    


/* frees memory associated with var */
void free_var_el(struct trig_var_data *var)
{
  free(var->name);
  free(var->value);
  free(var);
}


/*
 * remove var name from var_list
 * returns 1 if found, else 0
 */
int remove_var(struct trig_var_data **var_list, char *name)
{
  struct trig_var_data *i, *j;

  for (j = NULL, i = *var_list; i && str_cmp(name, i->name);
       j = i, i = i->next);

  if (i) {
    if (j) {
      j->next = i->next;
      free_var_el(i);
    } else {
      *var_list = i->next;
      free_var_el(i);
    }

    return 1;      
  }
  
  return 0;
}


/*  
 *  Logs any errors caused by scripts to the system log.
 *  Will eventually allow on-line view of script errors.
 */
void script_log(char *msg)
{
  char buf[256];

  sprintf(buf,"SCRIPT ERR: %s", msg);
  mudlog(buf, NRM, LVL_GOD, TRUE);
}

int text_processed(char *field, char *subfield, struct trig_var_data *vd,
  char *str)
{
  char *p, *p2;

  if (!str_cmp(field, "strlen")) {                     /* strlen    */
    sprintf(str, "%d", strlen(vd->value));
    return TRUE;
  } else if (!str_cmp(field, "trim")) {                /* trim      */
    /* trim whitespace from ends */
    p = vd->value;
    p2 = vd->value + strlen(vd->value) - 1;
    while (*p && isspace(*p)) p++;
    while ((p>=p2) && isspace(*p2)) p2--;
    if (p>p2) { /* nothing left */
      *str = '\0';
      return TRUE;
    }
    while (p<=p2)
      *str++ = *p++;
    *str = '\0';
    return TRUE;
  } else if (!str_cmp(field, "contains")) {            /* contains  */
    if (str_str(vd->value, subfield))
      sprintf(str, "1");
    else 
      sprintf(str, "0");
    return TRUE;
  } else if (!str_cmp(field, "car")) {                 /* car       */
    char *car = vd->value;
    while (*car && !isspace(*car))
      *str++ = *car++;
    *str = '\0';
    return TRUE;
  } else if (!str_cmp(field, "cdr")) {                 /* cdr       */
    char *cdr = vd->value;
    while (*cdr && !isspace(*cdr)) cdr++; /* skip 1st field */
    while (*cdr && isspace(*cdr)) cdr++;  /* skip to next */
    while (*cdr)
      *str++ = *cdr++;
    *str = '\0';
    return TRUE;
  } else if (!str_cmp(field, "mudcommand")) {
    /* find the mud command returned from this text */
/* NOTE: you may need to replace "cmd_info" with "complete_cmd_info", */
/* depending on what patches you've got applied.                      */

    int length, cmd;
    for (length = strlen(vd->value), cmd = 0;
        *complete_cmd_info[cmd].command != '\n'; cmd++)
      if (!strncmp(complete_cmd_info[cmd].command, vd->value, length))
        break;

    if (*complete_cmd_info[cmd].command == '\n') { strcpy(str,""); return FALSE; }
    else strcpy(str, complete_cmd_info[cmd].command);
    return TRUE;
  }

  return FALSE;
}


/* sets str to be the value of var.field */
void find_replacement(void *go, struct script_data *sc, trig_data *trig,
                int type, char *var, char *field, char *subfield, char *str)
{
  struct trig_var_data *vd=NULL;
  char_data *ch, *c = NULL, *rndm;
  obj_data *obj, *o = NULL;
  struct room_data *room, *r = NULL;
  char *name;
  int num, count;

  char *send_cmd[] = {"msend", "osend", "wsend"};
  char *echo_cmd[] = {"mecho", "oecho", "wecho"};
  char *echoaround_cmd[] = {"mechoaround", "oechoaround", "wechoaround"};
  char *door[] = {"mdoor",	"odoor",	"wdoor"};
  char *force[] = {"mforce",	"oforce",	"wforce"};
  char *load[] = {"mload",	"oload",	"wload"};
  char *purge[] = {"mpurge",	"opurge",	"wpurge"};
  char *teleport[] = {"mteleport","oteleport",	"wteleport"};
  char *damage[] = {"mdamage",	"odamage",	"wdamage"};

  /* X.global() will have a NULL trig */
  if (trig)
    for (vd = GET_TRIG_VARS(trig); vd; vd = vd->next)
      if (!str_cmp(vd->name, var))
        break;
  
  if (!vd)
    for (vd = sc->global_vars; vd; vd = vd->next)
      if (!str_cmp(vd->name, var) &&
          (vd->context==0 || vd->context==sc->context))
    break; 

  if (!*field) {
    if (vd)
      strcpy(str, vd->value);
    else {
      if (!str_cmp(var, "self"))
    strcpy(str, "self");
      else if (!str_cmp(var, "door"))
        strcpy(str,door[type]);
      else if (!str_cmp(var, "force"))
        strcpy(str,force[type]);
      else if (!str_cmp(var, "load"))
        strcpy(str,load[type]);
      else if (!str_cmp(var, "purge"))
        strcpy(str,purge[type]);
      else if (!str_cmp(var, "teleport"))
        strcpy(str,teleport[type]);
      else if (!str_cmp(var, "damage"))
        strcpy(str,damage[type]);
      else if (!str_cmp(var, "send"))
        strcpy(str,send_cmd[type]);
      else if (!str_cmp(var, "echo"))
        strcpy(str,echo_cmd[type]);
      else if (!str_cmp(var, "echoaround"))
        strcpy(str,echoaround_cmd[type]);
      else
    *str = '\0';
    }

    return;
  }

  else {
    if (vd) {
      name = vd->value;

      switch (type) {
      case MOB_TRIGGER:
    ch = (char_data *) go;

    if ((o = get_object_in_equip(ch, name)));
    else if ((o = get_obj_in_list(name, ch->carrying)));
    else if ((c = get_char_room(name, IN_ROOM(ch))));
    else if ((o = get_obj_in_list(name,world[IN_ROOM(ch)].contents)));
    else if ((c = get_char(name)));
    else if ((o = get_obj(name)));
    else if ((r = get_room(name))) {}

    break;
      case OBJ_TRIGGER:
    obj = (obj_data *) go;

    if ((c = get_char_by_obj(obj, name)));
    else if ((o = get_obj_by_obj(obj, name)));
    else if ((r = get_room(name))) {}

    break;
      case WLD_TRIGGER:
    room = (struct room_data *) go;

    if ((c = get_char_by_room(room, name)));
    else if ((o = get_obj_by_room(room, name)));
    else if ((r = get_room(name))) {}

    break;
      }
    }

    else {
      if (!str_cmp(var, "self")) {
    switch (type) {
    case MOB_TRIGGER:
      c = (char_data *) go;
      break;
    case OBJ_TRIGGER:
      o = (obj_data *) go;
      break;
    case WLD_TRIGGER:
      r = (struct room_data *) go;
      break;
    }
      }
      
      else if (!str_cmp(var, "people")) {
    sprintf(str,"%d",((num = atoi(field)) > 0) ? trgvar_in_room(num) : 0);	
    return;
      }
      else if (!str_cmp(var, "time")) {
        if (!str_cmp(field, "hour"))
          sprintf(str,"%ld", time_info.hours);
        else if (!str_cmp(field, "day"))
          sprintf(str,"%ld", time_info.day + 1);
        else if (!str_cmp(field, "month"))
          sprintf(str,"%ld", time_info.month + 1);
        else if (!str_cmp(field, "year"))
          sprintf(str,"%d", time_info.year);
        else *str = '\0';
        return;
      }
      else if (!str_cmp(var, "random")) {
    if (!str_cmp(field, "char")) {
      rndm = NULL;
      count = 0;

      if (type == MOB_TRIGGER) {
        ch = (char_data *) go;
        for (c = world[IN_ROOM(ch)].people; c; c = c->next_in_room)
          if (!PRF_FLAGGED(c, PRF_NOHASSLE) && (c != ch) &&
          CAN_SEE(ch, c)) {
        if (!number(0, count))
          rndm = c;
        count++;
          }
      }
      
      else if (type == OBJ_TRIGGER) {
        for (c = world[obj_room((obj_data *) go)].people; c;
         c = c->next_in_room)
          if (IS_NPC(c) ||
                  (!PRF_FLAGGED(c, PRF_NOHASSLE) && !GET_INVIS_LEV(c))) {
        if (!number(0, count))
          rndm = c;
        count++;
          }
      }
      
      else if (type == WLD_TRIGGER) {
        for (c = ((struct room_data *) go)->people; c;
         c = c->next_in_room)
          if (IS_NPC(c) ||
                  (!PRF_FLAGGED(c, PRF_NOHASSLE) && !GET_INVIS_LEV(c))) {
        if (!number(0, count))
          rndm = c;
        count++;
          }
      }
      
      if (rndm)
        sprintf(str, "%c%ld", UID_CHAR, GET_ID(rndm));
      else
        *str = '\0';
    }

    else
      sprintf(str, "%d", ((num = atoi(field)) > 0) ? number(1, num) : 0);
    
    return;
      }
    }
    
    if (c) {
      if (text_processed(field, subfield, vd, str)) return;

      else if (!str_cmp(field, "global")) { /* get global of something else */
        if (IS_NPC(c) && c->script) {
          find_replacement(go, c->script, NULL, MOB_TRIGGER,
            subfield, NULL, NULL, str);
        }
      }

      else if (!str_cmp(field, "name"))
        if (GET_SHORT(c))
          strcpy(str, GET_SHORT(c));
        else
          strcpy(str, GET_NAME(c));

      else if (!str_cmp(field, "id"))
    sprintf(str, "%ld", GET_ID(c));

      else if (!str_cmp(field, "alias"))
    strcpy(str, GET_NAME(c));

      else if (!str_cmp(field, "heshe"))
       {
        if (GET_SEX(c) == 0)
          sprintf(str, "it"); 
        else if (GET_SEX(c) == 1)
          sprintf(str, "he");
        else if (GET_SEX(c) == 2)
          sprintf(str, "she");
       }

      else if (!str_cmp(field, "hisher"))
       {
        if (GET_SEX(c) == 0)
          sprintf(str, "its"); 
        else if (GET_SEX(c) == 1)
          sprintf(str, "his"); 
        else if (GET_SEX(c) == 2)
          sprintf(str, "her"); 
       }

      else if (!str_cmp(field, "himher"))
       {
        if (GET_SEX(c) == 0)
          sprintf(str, "it"); 
        else if (GET_SEX(c) == 1)
          sprintf(str, "him"); 
        else if (GET_SEX(c) == 2)
          sprintf(str, "her"); 
       }

      else if (!str_cmp(field, "cheshe"))
       {
        if (GET_SEX(c) == 0)
          sprintf(str, "It");
        else if (GET_SEX(c) == 1)
          sprintf(str, "He");
        else if (GET_SEX(c) == 2)
          sprintf(str, "She");
       }

      else if (!str_cmp(field, "chisher"))
       {
        if (GET_SEX(c) == 0)
          sprintf(str, "Its");
        else if (GET_SEX(c) == 1)
          sprintf(str, "His");
        else if (GET_SEX(c) == 2)
          sprintf(str, "Her");
       }

      else if (!str_cmp(field, "chimher"))
       {
        if (GET_SEX(c) == 0)
          sprintf(str, "It");
        else if (GET_SEX(c) == 1)
          sprintf(str, "Him");
        else if (GET_SEX(c) == 2)
          sprintf(str, "Her");
       }

      else if (!str_cmp(field, "isnpc"))
        sprintf(str, "%ld", IS_NPC(c));

      else if (!str_cmp(field, "sneaking"))
        sprintf(str, "%ld", (IS_AFFECTED(c,AFF_SNEAK)));

      else if (!str_cmp(field, "blind"))
        sprintf(str, "%ld", (IS_AFFECTED(c,AFF_BLIND)));

      else if (!str_cmp(field, "invis"))
        sprintf(str, "%ld", (IS_AFFECTED(c,AFF_INVISIBLE)));

      else if (!str_cmp(field, "suffocating"))
        sprintf(str, "%ld", (IS_AFFECTED(c,AFF_SUFFOCATING)));

      else if (!str_cmp(field, "wrapped"))
        sprintf(str, "%ld", (IS_AFFECTED(c,AFF_WRAPPED)));

      else if (!str_cmp(field, "grouped"))
        sprintf(str, "%ld", (IS_AFFECTED(c,AFF_GROUP)));

      else if (!str_cmp(field, "gagged"))
        sprintf(str, "%ld", (IS_AFFECTED(c,AFF_GAGED)));

      else if (!str_cmp(field, "poisoned"))
        sprintf(str, "%ld", (IS_AFFECTED(c,AFF_POISON)));

      else if (!str_cmp(field, "shielded"))
        sprintf(str, "%ld", (IS_AFFECTED(c,AFF_SHIELDED)));

      else if (!str_cmp(field, "sleeping"))
        sprintf(str, "%ld", (IS_AFFECTED(c,AFF_SLEEP)));

      else if (!str_cmp(field, "notrack"))
        sprintf(str, "%ld", (IS_AFFECTED(c,AFF_NOTRACK)));

      else if (!str_cmp(field, "dreaming"))
        sprintf(str, "%ld", (IS_AFFECTED(c,AFF_DREAMING)));

      else if (!str_cmp(field, "armored"))
        sprintf(str, "%ld", (IS_AFFECTED(c,AFF_ARMOR)));

      else if (!str_cmp(field, "hiding"))
        sprintf(str, "%ld", (IS_AFFECTED(c,AFF_HIDE)));

      else if (!str_cmp(field, "camouflaged"))
        sprintf(str, "%ld", (IS_AFFECTED(c,AFF_CAMOUFLAGED)));

      else if (!str_cmp(field, "veiled"))
        sprintf(str, "%ld", (IS_AFFECTED(c,AFF_VEILED)));

      else if (!str_cmp(field, "tailing"))
        sprintf(str, "%ld", (IS_AFFECTED(c,AFF_TAILING)));

      else if (!str_cmp(field, "stunned"))
        sprintf(str, "%ld", (IS_AFFECTED(c,AFF_STUNNED)));

      else if (!str_cmp(field, "noquit"))
        sprintf(str, "%ld", (IS_AFFECTED(c,AFF_NOQUIT)));

      else if (!str_cmp(field, "targate"))
        sprintf(str, "%ld", (IS_AFFECTED2(c, AFF2_TAR_GATE)));

      else if (!str_cmp(field, "nodirtkick"))
        sprintf(str, "%ld", (IS_AFFECTED2(c, AFF2_NODIRTKICK)));

      else if (!str_cmp(field, "confused"))
        sprintf(str, "%ld", (IS_AFFECTED2(c, AFF2_CONFUSION)));

      else if (!str_cmp(field, "unbalanced"))
        sprintf(str, "%ld", (IS_AFFECTED2(c, AFF2_UNBALANCE)));

      else if (!str_cmp(field, "concentrate"))
        sprintf(str, "%ld", (IS_AFFECTED2(c, AFF2_CONCENTRATE)));

      else if (!str_cmp(field, "stubborn"))
        sprintf(str, "%ld", (IS_AFFECTED2(c, AFF2_STUBBORNESS)));

      else if (!str_cmp(field, "arrested"))
        sprintf(str, "%ld", (IS_AFFECTED2(c, AFF2_ARRESTED)));

      else if (!str_cmp(field, "compulsed"))
        sprintf(str, "%ld", (IS_AFFECTED2(c, AFF2_COMPULSED)));

      else if (!str_cmp(field, "compulsed"))
        sprintf(str, "%ld", (IS_AFFECTED2(c, AFF2_COMPULSED)));

      else if (!str_cmp(field, "noflee"))
        sprintf(str, "%ld", (IS_AFFECTED2(c, AFF2_NOFLEE)));

      else if (!str_cmp(field, "rage"))
        sprintf(str, "%ld", (IS_AFFECTED2(c, AFF2_RAGE)));

      else if (!str_cmp(field, "darkdream"))
        sprintf(str, "%ld", (IS_AFFECTED2(c, AFF2_DARKDREAM)));

      else if (!str_cmp(field, "masking"))
        sprintf(str, "%ld", (IS_AFFECTED2(c, AFF2_MASKED)));

      else if (!str_cmp(field, "subdued"))
        sprintf(str, "%ld", (IS_AFFECTED2(c, AFF2_SUBDUED)));

      else if (!str_cmp(field, "tarblur"))
        sprintf(str, "%ld", (IS_AFFECTED2(c, AFF2_TARBLUR)));

      else if (!str_cmp(field, "level"))
    sprintf(str, "%ld", GET_LEVEL(c));

      else if (!str_cmp(field, "hitp"))
    sprintf(str, "%d", GET_HIT(c));

      else if (!str_cmp(field, "maxhitp"))
    sprintf(str, "%d", GET_MAX_HIT(c));

      else if (!str_cmp(field, "mana"))
    sprintf(str, "%ld", GET_MANA(c));

      else if (!str_cmp(field, "maxmana"))
    sprintf(str, "%ld", GET_MAX_MANA(c));

      else if (!str_cmp(field, "move"))
    sprintf(str, "%d", GET_MOVE(c));

      else if (!str_cmp(field, "maxmove"))
    sprintf(str, "%d", GET_MAX_MOVE(c));

      else if (!str_cmp(field, "align"))
    sprintf(str, "%ld", 0L);

      else if (!str_cmp(field, "gold")) {
        if (subfield && *subfield) {
          int addition = atoi(subfield);
          GET_GOLD(c) += addition;
        }
        sprintf(str, "%ld", GET_GOLD(c));
      }

      else if (!str_cmp(field, "exp")) {
        if (subfield && *subfield) {
          int addition = MAX(atoi(subfield), 1000);
          GET_EXP(c) += addition;
        }
        sprintf(str, "%ld", GET_EXP(c));
      }

      else if (!str_cmp(field, "sex"))
    strcpy(str, genders[(int)GET_SEX(c)]);

      else if (!str_cmp(field, "weight"))
    sprintf(str, "%d", GET_WEIGHT(c));

      else if (!str_cmp(field, "canbeseen")) {
    if ((type == MOB_TRIGGER) && !CAN_SEE(((char_data *)go), c))
      strcpy(str, "0");
    else
      strcpy(str, "1");
      }

      else if (!str_cmp(field, "class"))
    sprinttype(GET_CLASS(c), pc_class_types, str);

      else if (!str_cmp(field, "race"))
    sprinttype(GET_RACE(c), hometown, str);

      else if (!str_cmp(field, "fighting"))
        if (FIGHTING(c))
          sprintf(str, "%c%ld", UID_CHAR, GET_ID(FIGHTING(c)));
        else *str = '\0';

#ifdef RIDING
      else if (!str_cmp(field, "riding"))
        if (RIDING(c))
          sprintf(str, "%c%ld", UID_CHAR, GET_ID(RIDING(c)));
        else *str = '\0';
#endif

#ifdef RIDDEN_BY
      else if (!str_cmp(field, "ridden_by"))
        if (RIDDEN_BY(c))
          sprintf(str, "%c%ld", UID_CHAR, GET_ID(RIDDEN_BY(c)));
        else *str = '\0';
#endif

      else if (!str_cmp(field, "vnum"))
    sprintf(str, "%ld", GET_MOB_VNUM(c));

      else if (!str_cmp(field, "str"))
    sprintf(str, "%d", GET_STR(c));

      else if (!str_cmp(field, "stradd"))
    sprintf(str, "0");

      else if (!str_cmp(field, "int"))
    sprintf(str, "%d", GET_INT(c));

      else if (!str_cmp(field, "wis"))
    sprintf(str, "%d", GET_WIS(c));

      else if (!str_cmp(field, "dex"))
    sprintf(str, "%d", GET_DEX(c));

      else if (!str_cmp(field, "con"))
    sprintf(str, "%d", GET_CON(c));

      else if (!str_cmp(field, "cha"))
    sprintf(str, "%d", GET_CHA(c));

      else if (!str_cmp(field, "room"))
    sprintf(str, "%d", world[IN_ROOM(c)].number);

      else if (!str_cmp(field, "skill"))
        strcpy(str,skill_percent(c, subfield));

      else if (!str_cmp(field, "eq")) {
        int pos = find_eq_pos(c, NULL, subfield);
        if (pos==-1 && isdigit(*subfield))
          pos = atoi(subfield);
        if (!subfield || !*subfield || pos < 0 || pos > NUM_WEARS)
          strcpy(str,"");
        else {
          if (!GET_EQ(c, pos))
            strcpy(str,"");
          else
            sprintf(str,"%c%ld",UID_CHAR, GET_ID(GET_EQ(c, pos)));
        }
      }

      else if (!str_cmp(field, "guild"))
        strcpy(str, guildie_of(c, subfield));

      else if (!str_cmp(field, "ranknum"))
        strcpy(str, rank_num_of(c, subfield));

      else if (!str_cmp(field, "rank"))
        strcpy(str, rank_name_of(c, subfield));

      else if (!str_cmp(field, "varexists")) {
        struct trig_var_data *vd;
        strcpy(str, "0");
        if (SCRIPT(c)) {
          for (vd = SCRIPT(c)->global_vars; vd; vd = vd->next) {
            if (!str_cmp(vd->name, subfield)) break;
          }
          if (vd) strcpy(str, "1");
        }
      }

      else if (!str_cmp(field, "next_in_room")) {
        if (c->next_in_room)
          sprintf(str,"%c%ld",UID_CHAR, GET_ID(c->next_in_room));
        else strcpy(str,"");
      }

      else {
        if (SCRIPT(c)) {
          for (vd = (SCRIPT(c))->global_vars; vd; vd = vd->next)
            if (!str_cmp(vd->name, field))
              break;
          if (vd)
            sprintf(str, "%s", vd->value);
          else {
            *str = '\0';
            sprintf(buf2, 
                   "Trigger: %s, VNum %ld. unknown char field: '%s'",
                   GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), field);
            script_log(buf2);
          }
        } else {
          *str = '\0';
          sprintf(buf2, 
                 "Trigger: %s, VNum %ld. unknown char field: '%s'",
                 GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), field);
          script_log(buf2);
        }
      }
    }

    else if (o) {
      if (text_processed(field, subfield, vd, str)) return;

      else if (!str_cmp(field, "name"))
    strcpy(str, o->name);

      else if (!str_cmp(field, "id"))
    sprintf(str, "%ld", GET_ID(o));

      else if (!str_cmp(field, "shortdesc"))
    strcpy(str, o->short_description);

      else if (!str_cmp(field, "vnum"))
    sprintf(str, "%ld", GET_OBJ_VNUM(o));

      else if (!str_cmp(field, "type"))
    sprinttype(GET_OBJ_TYPE(o), item_types, str);

      else if (!str_cmp(field, "timer"))
        sprintf(str, "%ld", GET_OBJ_TIMER(o));
      
      else if (!str_cmp(field, "val0"))
    sprintf(str, "%ld", GET_OBJ_VAL(o, 0));
      
      else if (!str_cmp(field, "val1"))
    sprintf(str, "%ld", GET_OBJ_VAL(o, 1));
      
      else if (!str_cmp(field, "val2"))
    sprintf(str, "%ld", GET_OBJ_VAL(o, 2));
      
      else if (!str_cmp(field, "val3"))
    sprintf(str, "%ld", GET_OBJ_VAL(o, 3));
      
      else if (!str_cmp(field, "carried_by"))
        if (o->carried_by)
          sprintf(str,"%c%ld",UID_CHAR, GET_ID(o->carried_by));
        else strcpy(str,"");

      else if (!str_cmp(field, "worn_by"))
        if (o->worn_by)
          sprintf(str,"%c%ld",UID_CHAR, GET_ID(o->worn_by));
        else strcpy(str,"");
      
      else {
    *str = '\0';
    sprintf(buf2,
        "Trigger: %s, VNum %ld, type: %d. unknown object field: '%s'",
        GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), type, field);
    script_log(buf2);
      }
    }

    else if (r) {
      if (text_processed(field, subfield, vd, str)) return;

      else if (!str_cmp(field, "name"))
    strcpy(str, r->name);
      else if (!str_cmp(field, "id"))
    sprintf(str, "%d", (ROOM_ID_BASE + (r - world)));
      else if (!str_cmp(field, "north")) {
    if (r->dir_option[NORTH])
      sprintbit(r->dir_option[NORTH]->exit_info ,exit_bits, str);
    else
      *str = '\0';
      } else if (!str_cmp(field, "east")) {
    if (r->dir_option[EAST])
      sprintbit(r->dir_option[EAST]->exit_info ,exit_bits, str);
    else
      *str = '\0';
      } else if (!str_cmp(field, "south")) {
    if (r->dir_option[SOUTH])
      sprintbit(r->dir_option[SOUTH]->exit_info ,exit_bits, str);
    else
      *str = '\0';
      } else if (!str_cmp(field, "west")) {
    if (r->dir_option[WEST])
      sprintbit(r->dir_option[WEST]->exit_info ,exit_bits, str);
    else
      *str = '\0';
      } else if (!str_cmp(field, "up")) {
    if (r->dir_option[UP])
      sprintbit(r->dir_option[UP]->exit_info ,exit_bits, str);
    else
      *str = '\0';
      } else if (!str_cmp(field, "down")) {
    if (r->dir_option[DOWN])
      sprintbit(r->dir_option[DOWN]->exit_info ,exit_bits, str);
    else
      *str = '\0';
      } else if (!str_cmp(field, "vnum")) {
          sprintf(str,"%d",r->number); 
      } else if (!str_cmp(field, "people")) {
          if (r->people)
            sprintf(str,"%c%ld",UID_CHAR,GET_ID(r->people));
          else *str = '\0';
      } else {
    *str = '\0';
    sprintf(buf2,
        "Trigger: %s, VNum %ld, type: %d. unknown room field: '%s'",
        GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), type, field);
    script_log(buf2);
      }
    }

    else
      *str = '\0';
  }
}


/* substitutes any variables into line and returns it as buf */
void var_subst(void *go, struct script_data *sc, trig_data *trig,
        int type, char *line, char *buf)
{
  char tmp[MAX_INPUT_LENGTH], repl_str[MAX_INPUT_LENGTH], *var, *field, *p;
  char *subfield_p, subfield[MAX_INPUT_LENGTH];
  int left, len;
  int paren_count = 0;

  if (!strchr(line, '%')) {
    strcpy(buf, line);
    return;
  }
  
  p = strcpy(tmp, line);
  subfield_p = subfield;
  
  left = MAX_INPUT_LENGTH - 1;
  
  while (*p && (left > 0)) {

    while (*p && (*p != '%') && (left > 0)) {
      *(buf++) = *(p++);
      left--;
    }
    
    *buf = '\0';
    
    /* double % */
    if (*p && (*(++p) == '%') && (left > 0)) {
      *(buf++) = *(p++);
      *buf = '\0';
      left--;
      continue;
    }

    else if (*p && (left > 0)) {
      
      for (var = p; *p && (*p != '%') && (*p != '.'); p++);

      field = p;
      if (*p == '.') {
    *(p++) = '\0';
    for (field = p; *p && ((*p != '%')||(paren_count)); p++) {
          if (*p=='(') {
            *p = '\0';
            paren_count++;
          } else if (*p==')') {
            *p = '\0';
            paren_count--;
          } else if (paren_count) *subfield_p++ = *p;
        }
      }

      *(p++) = '\0';
      *subfield_p = '\0';

      find_replacement(go, sc, trig, type, var, field, subfield, repl_str);
      
      strncat(buf, repl_str, left);
      len = strlen(repl_str);
      buf += len;
      left -= len;
    }
  }  
}


/* returns 1 if string is all digits, else 0 */
int is_num(char *num)
{
  while (*num && (isdigit(*num) || *num=='-'))
    num++;

  if (!*num || isspace(*num))
    return 1;
  else
    return 0;
}


/* evaluates 'lhs op rhs', and copies to result */
void eval_op(char *op, char *lhs, char *rhs, char *result, void *go,
         struct script_data *sc, trig_data *trig)
{
  char *p;
  int n;

  /* strip off extra spaces at begin and end */
  while (*lhs && isspace(*lhs)) 
    lhs++;
  while (*rhs && isspace(*rhs))
    rhs++;
  
  for (p = lhs; *p; p++);
  for (--p; isspace(*p) && (p > lhs); *p-- = '\0');
  for (p = rhs; *p; p++);
  for (--p; isspace(*p) && (p > rhs); *p-- = '\0');  


  /* find the op, and figure out the value */
  if (!strcmp("||", op)) {
    if ((!*lhs || (*lhs == '0')) && (!*rhs || (*rhs == '0')))
      strcpy(result, "0");
    else
      strcpy(result, "1");
  }

  else if (!strcmp("&&", op)) {
    if (!*lhs || (*lhs == '0') || !*rhs || (*rhs == '0'))
      strcpy (result, "0");
    else
      strcpy (result, "1");
  }

  else if (!strcmp("==", op)) {
    if (is_num(lhs) && is_num(rhs))
      sprintf(result, "%d", atoi(lhs) == atoi(rhs));
    else
      sprintf(result, "%d", !str_cmp(lhs, rhs));
  }   

  else if (!strcmp("!=", op)) {
    if (is_num(lhs) && is_num(rhs))
      sprintf(result, "%d", atoi(lhs) != atoi(rhs));
    else
      sprintf(result, "%d", str_cmp(lhs, rhs));
  }   

  else if (!strcmp("<=", op)) {
    if (is_num(lhs) && is_num(rhs))
      sprintf(result, "%d", atoi(lhs) <= atoi(rhs));
    else
      sprintf(result, "%d", str_cmp(lhs, rhs) <= 0);
  }

  else if (!strcmp(">=", op)) {
    if (is_num(lhs) && is_num(rhs))
      sprintf(result, "%d", atoi(lhs) >= atoi(rhs));
    else
      sprintf(result, "%d", str_cmp(lhs, rhs) <= 0);
  }

  else if (!strcmp("<", op)) {
    if (is_num(lhs) && is_num(rhs))
      sprintf(result, "%d", atoi(lhs) < atoi(rhs));
    else
      sprintf(result, "%d", str_cmp(lhs, rhs) < 0);
  }

  else if (!strcmp(">", op)) {
    if (is_num(lhs) && is_num(rhs))
      sprintf(result, "%d", atoi(lhs) > atoi(rhs));
    else
      sprintf(result, "%d", str_cmp(lhs, rhs) > 0);
  }

  else if (!strcmp("/=", op))
    sprintf(result, "%c", str_str(lhs, rhs) ? '1' : '0');

  else if (!strcmp("*", op))
    sprintf(result, "%d", atoi(lhs) * atoi(rhs));
  
  else if (!strcmp("/", op))
    sprintf(result, "%d", (n = atoi(rhs)) ? (atoi(lhs) / n) : 0);

  else if (!strcmp("+", op)) 
    sprintf(result, "%d", atoi(lhs) + atoi(rhs));

  else if (!strcmp("-", op))
    sprintf(result, "%d", atoi(lhs) - atoi(rhs));

  else if (!strcmp("!", op)) {
    if (is_num(rhs))
      sprintf(result, "%d", !atoi(rhs));
    else
      sprintf(result, "%d", !*rhs);
  }
}


/*
 * p points to the first quote, returns the matching
 * end quote, or the last non-null char in p.
*/
char *matching_quote(char *p)
{
  for (p++; *p && (*p != '"'); p++) {
    if (*p == '\\')
      p++;
  }

  if (!*p)
    p--;

  return p;
}

/*
 * p points to the first paren.  returns a pointer to the
 * matching closing paren, or the last non-null char in p.
 */
char *matching_paren(char *p)
{
  int i;

  for (p++, i = 1; *p && i; p++) {
    if (*p == '(')
      i++;
    else if (*p == ')')
      i--;
    else if (*p == '"')
      p = matching_quote(p);
  }

  return --p;
}


/* evaluates line, and returns answer in result */
void eval_expr(char *line, char *result, void *go, struct script_data *sc,
        trig_data *trig, int type)
{
  char expr[MAX_INPUT_LENGTH], *p;

  while (*line && isspace(*line))
    line++;
  
  if (eval_lhs_op_rhs(line, result, go, sc, trig, type));

  else if (*line == '(') {
    p = strcpy(expr, line);
    p = matching_paren(expr);
    *p = '\0';
    eval_expr(expr + 1, result, go, sc, trig, type);
  }

  else
    var_subst(go, sc, trig, type, line, result);
}


/*
 * evaluates expr if it is in the form lhs op rhs, and copies
 * answer in result.  returns 1 if expr is evaluated, else 0
 */
int eval_lhs_op_rhs(char *expr, char *result, void *go, struct script_data *sc,
            trig_data *trig, int type)
{
  char *p = NULL , *tokens[MAX_INPUT_LENGTH];
  char line[MAX_INPUT_LENGTH], lhr[MAX_INPUT_LENGTH], rhr[MAX_INPUT_LENGTH];
  int i, j;
  
  /*
   * valid operands, in order of priority
   * each must also be defined in eval_op()
   */
  static char *ops[] = {
    "||",
    "&&",
    "==",
    "!=",
    "<=",
    ">=",
    "<",
    ">",
    "/=",
    "-",
    "+",
    "/",
    "*",
    "!",
    "\n"
  };

  memset(line,'\0',MAX_INPUT_LENGTH);
  memset(lhr ,'\0',MAX_INPUT_LENGTH);
  memset(rhr ,'\0',MAX_INPUT_LENGTH);
  memset(tokens,'\0', sizeof(char *)*MAX_INPUT_LENGTH);

  p = strcpy(line, expr);

  /*
   * initialize tokens, an array of pointers to locations
   * in line where the ops could possibly occur.
   */
  for (j = 0; *p; j++) {
    tokens[j] = p;
    if (*p == '(')
      p = matching_paren(p) + 1;
    else if (*p == '"')
      p = matching_quote(p) + 1;
    else if (isalnum(*p))
      for (p++; *p && (isalnum(*p) || isspace(*p)); p++);
    else
      p++;
  }
  tokens[j] = NULL;

  for (i = 0; *ops[i] != '\n'; i++)
    for (j = 0; tokens[j]; j++)
      if (!strn_cmp(ops[i], tokens[j], strlen(ops[i]))) {
    *tokens[j] = '\0';
    p = tokens[j] + strlen(ops[i]);

    eval_expr(line, lhr, go, sc, trig, type);
    eval_expr(p, rhr, go, sc, trig, type);
    eval_op(ops[i], lhr, rhr, result, go, sc, trig);

    return 1;
      }

  return 0;
}



/* returns 1 if cond is true, else 0 */
int process_if(char *cond, void *go, struct script_data *sc,
        trig_data *trig, int type)
{
  char result[MAX_INPUT_LENGTH], *p;

  eval_expr(cond, result, go, sc, trig, type);
  
  p = result;
  skip_spaces(&p);

  if (!*p || *p == '0')
    return 0;
  else
    return 1;
}


/*
 * scans for end of if-block.
 * returns the line containg 'end', or the last
 * line of the trigger if not found.
 */
struct cmdlist_element *find_end(struct cmdlist_element *cl)
{
  struct cmdlist_element *c;
  char *p;

  if (!(cl->next))
    return cl;

  for (c = cl->next; c->next; c = c->next) {
    for (p = c->cmd; *p && isspace(*p); p++);

    if (!strn_cmp("if ", p, 3))
      c = find_end(c);
    else if (!strn_cmp("end", p, 3))
      return c;
  }
  
  return c;
}


/*
 * searches for valid elseif, else, or end to continue execution at.
 * returns line of elseif, else, or end if found, or last line of trigger.
 */
struct cmdlist_element *find_else_end(trig_data *trig,
                      struct cmdlist_element *cl, void *go,
                      struct script_data *sc, int type)
{
  struct cmdlist_element *c;
  char *p;

  if (!(cl->next))
    return cl;

  for (c = cl->next; c->next; c = c->next) {
    for (p = c->cmd; *p && isspace(*p); p++);

    if (!strn_cmp("if ", p, 3))
      c = find_end(c);

    else if (!strn_cmp("elseif ", p, 7)) {
      if (process_if(p + 7, go, sc, trig, type)) {
    GET_TRIG_DEPTH(trig)++;
    return c;
      }
    }
    
    else if (!strn_cmp("else", p, 4)) {
      GET_TRIG_DEPTH(trig)++;
      return c;
    }

    else if (!strn_cmp("end", p, 3))
      return c;
  }

  return c;
}


/* processes any 'wait' commands in a trigger */
void process_wait(void *go, trig_data *trig, int type, char *cmd,
          struct cmdlist_element *cl)
{
  char buf[MAX_INPUT_LENGTH], *arg;
  struct wait_event_data *wait_event_obj;
  long time, hr, min, ntime;
  char c;

  arg = any_one_arg(cmd, buf);
  skip_spaces(&arg);
  
  if (!*arg) {
    sprintf(buf2, "Trigger: %s, VNum %ld. wait w/o an arg: '%s'",
        GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), cl->cmd);
    script_log(buf2);
  }

  else if (!strn_cmp(arg, "until ", 6)) {

    /* valid forms of time are 14:30 and 1430 */
    if (sscanf(arg, "until %ld:%ld", &hr, &min) == 2)
      min += (hr * 60);
    else
      min = (hr % 100) + ((hr / 100) * 60);

    /* calculate the pulse of the day of "until" time */
    ntime = (min * SECS_PER_MUD_HOUR * PASSES_PER_SEC) / 60;

    /* calculate pulse of day of current time */
    time = (dg_global_pulse % (SECS_PER_MUD_HOUR * PASSES_PER_SEC)) +
      (time_info.hours * SECS_PER_MUD_HOUR * PASSES_PER_SEC);
    
    if (time >= ntime) /* adjust for next day */
      time = (SECS_PER_MUD_DAY * PASSES_PER_SEC) - time + ntime;
    else
      time = ntime - time;
  }

  else {
    if (sscanf(arg, "%ld %c", &time, &c) == 2) {
      if (c == 't')
    time *= PULSES_PER_MUD_HOUR;
      else if (c == 's')
    time *= PASSES_PER_SEC;
    }
  }

  CREATE(wait_event_obj, struct wait_event_data, 1);
  wait_event_obj->trigger = trig;
  wait_event_obj->go = go;
  wait_event_obj->type = type;

  GET_TRIG_WAIT(trig) = add_event(time, trig_wait_event, wait_event_obj);
  trig->curr_state = cl->next;
}


/* processes a script set command */
void process_set(struct script_data *sc, trig_data *trig, char *cmd)
{
  char arg[MAX_INPUT_LENGTH], name[MAX_INPUT_LENGTH], *value;
  
  value = two_arguments(cmd, arg, name);

  skip_spaces(&value);

  if (!*name) {
    sprintf(buf2, "Trigger: %s, VNum %ld. set w/o an arg: '%s'",
        GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), cmd);
    script_log(buf2);
    return;
  }

  add_var(&GET_TRIG_VARS(trig), name, value, sc->context);

}

/* processes a script eval command */
void process_eval(void *go, struct script_data *sc, trig_data *trig,
         int type, char *cmd)
{
  char arg[MAX_INPUT_LENGTH], name[MAX_INPUT_LENGTH];
  char result[MAX_INPUT_LENGTH], *expr;
  
  expr = two_arguments(cmd, arg, name);

  skip_spaces(&expr);

  if (!*name) {
    sprintf(buf2, "Trigger: %s, VNum %ld. eval w/o an arg: '%s'",
        GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), cmd);
    script_log(buf2);
    return;
  }

  eval_expr(expr, result, go, sc, trig, type);
  add_var(&GET_TRIG_VARS(trig), name, result, sc->context);
}


/* script attaching a trigger to something */
void process_attach(void *go, struct script_data *sc, trig_data *trig,
                    int type, char *cmd)
{
  char arg[MAX_INPUT_LENGTH], trignum_s[MAX_INPUT_LENGTH];
  char result[MAX_INPUT_LENGTH], *id_p;
  trig_data *newtrig;
  char_data *c=NULL;
  obj_data *o=NULL;
  room_data *r=NULL;
  long trignum, id;

  id_p = two_arguments(cmd, arg, trignum_s);
  skip_spaces(&id_p);

  if (!*trignum_s) {
    sprintf(buf2, "Trigger: %s, VNum %ld. attach w/o an arg: '%s'",
        GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), cmd);
    script_log(buf2);
    return;
  }

  if (!id_p || !*id_p || atoi(id_p)==0) {
    sprintf(buf2, "Trigger: %s, VNum %ld. attach invalid id arg: '%s'",
        GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), cmd);
    script_log(buf2);
    return;
  }

  /* parse and locate the id specified */
  eval_expr(id_p, result, go, sc, trig, type);
  if (!(id = atoi(result))) {
    sprintf(buf2, "Trigger: %s, VNum %ld. attach invalid id arg: '%s'",
        GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), cmd);
    script_log(buf2);
    return;
  }
  c = find_char(id);
  if (!c) {
    o = find_obj(id);
    if (!o) {
      r = find_room(id);
      if (!r) {
        sprintf(buf2, "Trigger: %s, VNum %ld. attach invalid id arg: '%s'",
                GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), cmd);
        script_log(buf2);
        return;
      }
    }
  }

  /* locate and load the trigger specified */
  trignum = real_trigger(atoi(trignum_s));
  if (trignum<0 || !(newtrig=read_trigger(trignum))) {
    sprintf(buf2, "Trigger: %s, VNum %ld. attach invalid trigger: '%s'",
        GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), trignum_s);
    script_log(buf2);
    return;
  }

  if (c) {
    if (!SCRIPT(c))
      CREATE(SCRIPT(c), struct script_data, 1);
    add_trigger(SCRIPT(c), newtrig, -1);
    return;
  }
  
  if (o) {
    if (!SCRIPT(o))
      CREATE(SCRIPT(o), struct script_data, 1);
    add_trigger(SCRIPT(o), newtrig, -1);
    return;
  }
  
  if (r) {
    if (!SCRIPT(r))
      CREATE(SCRIPT(r), struct script_data, 1);
    add_trigger(SCRIPT(r), newtrig, -1);
    return;
  }
  
}


/* script detaching a trigger from something */
void process_detach(void *go, struct script_data *sc, trig_data *trig,
                    int type, char *cmd)
{
  char arg[MAX_INPUT_LENGTH], trignum_s[MAX_INPUT_LENGTH];
  char result[MAX_INPUT_LENGTH], *id_p;
  char_data *c=NULL;
  obj_data *o=NULL;
  room_data *r=NULL;
  long id;

  id_p = two_arguments(cmd, arg, trignum_s);
  skip_spaces(&id_p);

  if (!*trignum_s) {
    sprintf(buf2, "Trigger: %s, VNum %ld. detach w/o an arg: '%s'",
        GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), cmd);
    script_log(buf2);
    return;
  }

  if (!id_p || !*id_p || atoi(id_p)==0) {
    sprintf(buf2, "Trigger: %s, VNum %ld. detach invalid id arg: '%s'",
        GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), cmd);
    script_log(buf2);
    return;
  }

  /* parse and locate the id specified */
  eval_expr(id_p, result, go, sc, trig, type);
  if (!(id = atoi(result))) {
    sprintf(buf2, "Trigger: %s, VNum %ld. detach invalid id arg: '%s'",
        GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), cmd);
    script_log(buf2);
    return;
  }
  c = find_char(id);
  if (!c) {
    o = find_obj(id);
    if (!o) {
      r = find_room(id);
      if (!r) {
        sprintf(buf2, "Trigger: %s, VNum %ld. detach invalid id arg: '%s'",
                GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), cmd);
        script_log(buf2);
        return;
      }
    }
  }


  if (c && SCRIPT(c)) {
    if (remove_trigger(SCRIPT(c), trignum_s)) {
      if (!TRIGGERS(SCRIPT(c))) {
        extract_script(SCRIPT(c));
        SCRIPT(c) = NULL;
      }
    }
    return;
  }
  
  if (o && SCRIPT(o)) {
    if (remove_trigger(SCRIPT(o), trignum_s)) {
      if (!TRIGGERS(SCRIPT(o))) {
        extract_script(SCRIPT(o));
        SCRIPT(o) = NULL;
      }
    }
    return;
  }
  
  if (r && SCRIPT(r)) {
    if (remove_trigger(SCRIPT(r), trignum_s)) {
      if (!TRIGGERS(SCRIPT(r))) {
        extract_script(SCRIPT(r));
        SCRIPT(r) = NULL;
      }
    }
    return;
  }
  
}

struct room_data *dg_room_of_obj(struct obj_data *obj)
{
  if (obj->in_room > NOWHERE) return &world[obj->in_room];
  if (obj->carried_by)        return &world[obj->carried_by->in_room];
  if (obj->worn_by)           return &world[obj->worn_by->in_room];
  if (obj->in_obj)            return (dg_room_of_obj(obj->in_obj));
  return NULL;
}


/* create a UID variable from the id number */
void makeuid_var(void *go, struct script_data *sc, trig_data *trig,
         int type, char *cmd)
{
  char arg[MAX_INPUT_LENGTH], varname[MAX_INPUT_LENGTH];
  char result[MAX_INPUT_LENGTH], *uid_p;
  char uid[MAX_INPUT_LENGTH];
  
  uid_p = two_arguments(cmd, arg, varname);
  skip_spaces(&uid_p);

  if (!*varname) {
    sprintf(buf2, "Trigger: %s, VNum %ld. makeuid w/o an arg: '%s'",
        GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), cmd);
    script_log(buf2);
    return;
  }

  if (!uid_p || !*uid_p || atoi(uid_p)==0) {
    sprintf(buf2, "Trigger: %s, VNum %ld. makeuid invalid id arg: '%s'",
        GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), cmd);
    script_log(buf2);
    return;
  }

  eval_expr(uid_p, result, go, sc, trig, type);
  sprintf(uid,"%c%s",UID_CHAR, result);
  add_var(&GET_TRIG_VARS(trig), varname, uid, sc->context);
}

/*
 * processes a script return command.
 * returns the new value for the script to return.
 */
int process_return(trig_data *trig, char *cmd)
{
  char arg1[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH];
  
  two_arguments(cmd, arg1, arg2);
  
  if (!*arg2) {
    sprintf(buf2, "Trigger: %s, VNum %ld. return w/o an arg: '%s'",
        GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), cmd);
    script_log(buf2);
    return 1;
  }

  return atoi(arg2);
}


/*
 * removes a variable from the global vars of sc,
 * or the local vars of trig if not found in global list.
 */
void process_unset(struct script_data *sc, trig_data *trig, char *cmd)
{
  char arg[MAX_INPUT_LENGTH], *var;

  var = any_one_arg(cmd, arg);

  skip_spaces(&var);

  if (!*var) {
    sprintf(buf2, "Trigger: %s, VNum %ld. unset w/o an arg: '%s'",
        GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), cmd);
    script_log(buf2);
    return;
  }

  if (!remove_var(&(sc->global_vars), var))
    remove_var(&GET_TRIG_VARS(trig), var);
}


/*
 * copy a locally owned variable to the globals of another script
 *     'remote <variable_name> <uid>'
 */
void process_remote(struct script_data *sc, trig_data *trig, char *cmd)
{
  struct trig_var_data *vd;
  struct script_data *sc_remote=NULL;
  char *line, *var, *uid_p;
  char arg[MAX_INPUT_LENGTH];
  long uid, context;
  room_data *room;
  char_data *mob;
  obj_data *obj;

  line = any_one_arg(cmd, arg);
  two_arguments(line, buf, buf2);
  var = buf;
  uid_p = buf2;
  skip_spaces(&var);
  skip_spaces(&uid_p);
  

  if (!*buf || !*buf2) {
    sprintf(buf2, "Trigger: %s, VNum %ld. remote: invalid arguments '%s'",
        GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), cmd);
    script_log(buf2);
    return;
  }

  /* find the locally owned variable */
  for (vd = GET_TRIG_VARS(trig); vd; vd = vd->next)
    if (!str_cmp(vd->name, buf))
      break;

  if (!vd)
    for (vd = sc->global_vars; vd; vd = vd->next)
      if (!str_cmp(vd->name, var) &&
          (vd->context==0 || vd->context==sc->context))
    break; 

  if (!vd) {
    sprintf(buf2, "Trigger: %s, VNum %ld. local var '%s' not found in remote call",
        GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), buf);
    script_log(buf2);
    return;
  }    

  /* find the target script from the uid number */
  uid = atol(buf2);
  if (uid<=0) {
    sprintf(buf, "Trigger: %s, VNum %ld. remote: illegal uid '%s'",
            GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), buf2);
    script_log(buf);
    return;
  }

  /* for all but PC's, context comes from the existing context. */
  /* for PC's, context is 0 (global) */
  context = vd->context;

  if ((room = find_room(uid))) {
    sc_remote = SCRIPT(room);
  } else if ((mob = find_char(uid))) {
    sc_remote = SCRIPT(mob);
    if (!IS_NPC(mob)) context = 0;
  } else if ((obj = find_obj(uid))) {
    sc_remote = SCRIPT(obj);
  } else {
    sprintf(buf, "Trigger: %s, VNum %ld. remote: uid '%ld' invalid",
            GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), uid);
    script_log(buf);
    return;
  }

  if (!sc_remote) return; /* no script to assign */

  add_var(&(sc_remote->global_vars), vd->name, vd->value, context);
}


/*
 * command-line interface to rdelete
 * named vdelete so people didn't think it was to delete rooms
 */
ACMD(do_vdelete)
{
  struct trig_var_data *vd, *vd_prev=NULL;
  struct script_data *sc_remote=NULL;
  char *var, *uid_p;
  long uid, context;
  room_data *room;
  char_data *mob;
  obj_data *obj;

  argument = two_arguments(argument, buf, buf2);
  var = buf;
  uid_p = buf2;
  skip_spaces(&var);
  skip_spaces(&uid_p);


  if (!*buf || !*buf2) {
    send_to_char("Usage: vdelete <variablename> <id>\r\n", ch);
    return;
  }


  /* find the target script from the uid number */
  uid = atoi(buf2);
  if (uid<=0) {
    send_to_char("vdelete: illegal id specified.\r\n", ch);
    return;
  }


  if ((room = find_room(uid))) {
    sc_remote = SCRIPT(room);
  } else if ((mob = find_char(uid))) {
    sc_remote = SCRIPT(mob);
    if (!IS_NPC(mob)) context = 0;
  } else if ((obj = find_obj(uid))) {
    sc_remote = SCRIPT(obj);
  } else {
    send_to_char("vdelete: cannot resolve specified id.\r\n", ch);
    return;
  }

  if ((sc_remote==NULL) ||
      (sc_remote->global_vars==NULL)) {
    send_to_char("That id represents no global variables.\r\n", ch);
    return;
  }

  /* find the global */
  for (vd = sc_remote->global_vars; vd; vd_prev = vd, vd = vd->next)
    if (!str_cmp(vd->name, var))
      break;

  if (!vd) {
    send_to_char("That variable cannot be located.\r\n", ch);
    return;
  }

  /* ok, delete the variable */
  if (vd_prev) vd_prev->next = vd->next;
  else sc_remote->global_vars = vd->next;

  /* and free up the space */
  free(vd->value);
  free(vd->name);
  free(vd);

  send_to_char("Deleted.\r\n", ch);
}

/*
 * delete a variable from the globals of another script
 *     'rdelete <variable_name> <uid>'
 */
void process_rdelete(struct script_data *sc, trig_data *trig, char *cmd)
{
  struct trig_var_data *vd, *vd_prev=NULL;
  struct script_data *sc_remote=NULL;
  char *line, *var, *uid_p;
  char arg[MAX_INPUT_LENGTH];
  long uid, context;
  room_data *room;
  char_data *mob;
  obj_data *obj;

  line = any_one_arg(cmd, arg);
  two_arguments(line, buf, buf2);
  var = buf;
  uid_p = buf2;
  skip_spaces(&var);
  skip_spaces(&uid_p);


  if (!*buf || !*buf2) {
    sprintf(buf2, "Trigger: %s, VNum %ld. rdelete: invalid arguments '%s'",
            GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), cmd);
    script_log(buf2);
    return;
  }


  /* find the target script from the uid number */
  uid = atoi(buf2);
  if (uid<=0) {
    sprintf(buf, "Trigger: %s, VNum %ld. rdelete: illegal uid '%s'",
            GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), buf2);
    script_log(buf);
    return;
  }


  if ((room = find_room(uid))) {
    sc_remote = SCRIPT(room);
  } else if ((mob = find_char(uid))) {
    sc_remote = SCRIPT(mob);
    if (!IS_NPC(mob)) context = 0;
  } else if ((obj = find_obj(uid))) {
    sc_remote = SCRIPT(obj);
  } else {
    sprintf(buf, "Trigger: %s, VNum %ld. remote: uid '%ld' invalid",
            GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), uid);
    script_log(buf);
    return;
  }

  if (sc_remote==NULL) return; /* no script to delete a trigger from */
  if (sc_remote->global_vars==NULL) return; /* no script globals */

  /* find the global */
  for (vd = sc_remote->global_vars; vd; vd_prev = vd, vd = vd->next)
    if (!str_cmp(vd->name, var) &&
        (vd->context==0 || vd->context==sc->context))
      break;

  if (!vd) return; /* the variable doesn't exist, or is the wrong context */

  /* ok, delete the variable */
  if (vd_prev) vd_prev->next = vd->next;
  else sc_remote->global_vars = vd->next;

  /* and free up the space */
  free(vd->value);
  free(vd->name);
  free(vd);
}


/*
 * makes a local variable into a global variable
 */
void process_global(struct script_data *sc, trig_data *trig, char *cmd, long id)
{
  struct trig_var_data *vd;
  char arg[MAX_INPUT_LENGTH], *var;

  var = any_one_arg(cmd, arg);

  skip_spaces(&var);

  if (!*var) {
    sprintf(buf2, "Trigger: %s, VNum %ld. global w/o an arg: '%s'",
        GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), cmd);
    script_log(buf2);
    return;
  }

  for (vd = GET_TRIG_VARS(trig); vd; vd = vd->next)
    if (!str_cmp(vd->name, var))
      break;

  if (!vd) {
    sprintf(buf2, "Trigger: %s, VNum %ld. local var '%s' not found in global call",
        GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), var);
    script_log(buf2);
    return;
  }    

  add_var(&(sc->global_vars), vd->name, vd->value, id);
  remove_var(&GET_TRIG_VARS(trig), vd->name);
}


/* set the current context for a script */
void process_context(struct script_data *sc, trig_data *trig, char *cmd)
{
  char arg[MAX_INPUT_LENGTH], *var;
  
  var = any_one_arg(cmd, arg);

  skip_spaces(&var);

  if (!*var) {
    sprintf(buf2, "Trigger: %s, VNum %ld. context w/o an arg: '%s'",
        GET_TRIG_NAME(trig), GET_TRIG_VNUM(trig), cmd);
    script_log(buf2);
    return;
  }

  sc->context = atol(var);
}

void extract_value(struct script_data *sc, trig_data *trig, char *cmd)
{
  char buf[MAX_INPUT_LENGTH];
  char buf2[MAX_INPUT_LENGTH];
  char *buf3;
  char to[128];
  int num;

  buf3 = any_one_arg(cmd, buf);
  half_chop(buf3, buf2, buf);
  strcpy(to, buf2);

  num = atoi(buf);
  if (num < 1) {
    script_log("extract number < 1!");
    return;
  }

  half_chop(buf, buf3, buf2);

  while (num>0) {
    half_chop(buf2, buf, buf2);
    num--;
  }

  add_var(&GET_TRIG_VARS(trig), to, buf, sc->context);
}

int dg_owner_purged;

/*  This is the core driver for scripts. */
int script_driver(void *go, trig_data *trig, int type, int mode)
{
  static int depth = 0;
  int ret_val = 1;
  struct cmdlist_element *cl;
  char cmd[MAX_INPUT_LENGTH], *p;
  struct script_data *sc = 0;
  struct cmdlist_element *temp;
  unsigned long loops = 0;


  void obj_command_interpreter(obj_data *obj, char *argument);
  void wld_command_interpreter(struct room_data *room, char *argument);

  if (depth > MAX_SCRIPT_DEPTH) {
    script_log("Triggers recursed beyond maximum allowed depth.");
    return ret_val;
  }

  depth++;

  switch (type) {
  case MOB_TRIGGER:
    sc = SCRIPT((char_data *) go);
    break;
  case OBJ_TRIGGER:
    sc = SCRIPT((obj_data *) go);
    break;
  case WLD_TRIGGER:
    sc = SCRIPT((struct room_data *) go);
    break;
  }

  if (mode == TRIG_NEW) {
    GET_TRIG_DEPTH(trig) = 1;
    GET_TRIG_LOOPS(trig) = 0;
    sc->context = 0;
  }

  dg_owner_purged = 0;
  
  for (cl = (mode == TRIG_NEW) ? trig->cmdlist : trig->curr_state;
       cl && GET_TRIG_DEPTH(trig); cl = cl->next) {
    for (p = cl->cmd; *p && isspace(*p); p++);

    if (*p == '*') /* comment */
      continue;

    else if (!strn_cmp(p, "if ", 3)) {
      if (process_if(p + 3, go, sc, trig, type))
    GET_TRIG_DEPTH(trig)++;
      else
    cl = find_else_end(trig, cl, go, sc, type);
    }
    
    else if (!strn_cmp("elseif ", p, 7) ||
         !strn_cmp("else", p, 4)) {
      cl = find_end(cl);
      GET_TRIG_DEPTH(trig)--;
    } else if (!strn_cmp("while ", p, 6)) {
      temp = find_done(cl);  
      if (process_if(p + 6, go, sc, trig, type)) {
         temp->original = cl;
      } else {
         cl = temp;
         loops = 0;
      }
    } else if (!strn_cmp("switch ", p, 7)) {
      cl = find_case(trig, cl, go, sc, type, p + 7);
    } else if (!strn_cmp("end", p, 3)) {   
      GET_TRIG_DEPTH(trig)--;
    } else if (!strn_cmp("done", p, 4)) {
      char *orig_cmd = cl->original->cmd;
      while (*orig_cmd && isspace(*orig_cmd)) orig_cmd++;
      if (cl->original && process_if(orig_cmd + 6, go, sc, trig,
          type)) {
        cl = cl->original;
        loops++;   
        GET_TRIG_LOOPS(trig)++;
        if (loops == 30) {
          process_wait(go, trig, type, "wait 1", cl);
           depth--;
          return ret_val;
        }
        if (GET_TRIG_LOOPS(trig) == 100) {
          char *buf;
	  CREATE( buf, char, MAX_STRING_LENGTH );
          sprintf(buf,"SCRIPTERR: Trigger VNum %ld has looped 100 times!!!",
            GET_TRIG_VNUM(trig));
          mudlog(buf, NRM, LVL_GOD, TRUE);
          free(buf);
        }
      }
    } else if (!strn_cmp("break", p, 5)) {
      cl = find_done(cl);
    } else if (!strn_cmp("case", p, 4)) { 
       /* Do nothing, this allows multiple cases to a single instance */
    }
    
      
    else {
      
      var_subst(go, sc, trig, type, p, cmd);

      if (!strn_cmp(cmd, "eval ", 5))
    process_eval(go, sc, trig, type, cmd);

      else if (!strn_cmp(cmd, "nop ", 4)); /* nop: do nothing */

      else if (!strn_cmp(cmd, "extract ", 8))
        extract_value(sc, trig, cmd);

      else if (!strn_cmp(cmd, "makeuid ", 8))
    makeuid_var(go, sc, trig, type, cmd);

      else if (!strn_cmp(cmd, "halt", 4))
    break;

      else if (!strn_cmp(cmd, "dg_cast ", 8))
    do_dg_cast(go, sc, trig, type, cmd);

      else if (!strn_cmp(cmd, "dg_affect ", 10))
    do_dg_affect(go, sc, trig, type, cmd);

      else if (!strn_cmp(cmd, "global ", 7))
    process_global(sc, trig, cmd, sc->context);

      else if (!strn_cmp(cmd, "context ", 8))
    process_context(sc, trig, cmd);

      else if (!strn_cmp(cmd, "remote ", 7))
    process_remote(sc, trig, cmd);

      else if (!strn_cmp(cmd, "rdelete ", 8))
    process_rdelete(sc, trig, cmd);

      else if (!strn_cmp(cmd, "return ", 7))
    ret_val = process_return(trig, cmd);
      
      else if (!strn_cmp(cmd, "set ", 4))
    process_set(sc, trig, cmd);
      
      else if (!strn_cmp(cmd, "unset ", 6))
    process_unset(sc, trig, cmd);
      
      else if (!strn_cmp(cmd, "wait ", 5)) {
    process_wait(go, trig, type, cmd, cl);
    depth--;
    return ret_val;
      }

      else if (!strn_cmp(cmd, "attach ", 7))
    process_attach(go, sc, trig, type, cmd);
      
      else if (!strn_cmp(cmd, "detach ", 7))
    process_detach(go, sc, trig, type, cmd);
      
      else if (!strn_cmp(cmd, "version", 7))
        mudlog(DG_SCRIPT_VERSION, NRM, LVL_GOD, TRUE);
      
      else {
    switch (type) {
    case MOB_TRIGGER:
      command_interpreter((char_data *) go, cmd);
      break;
    case OBJ_TRIGGER:
      obj_command_interpreter((obj_data *) go, cmd);
      break;
    case WLD_TRIGGER:
      wld_command_interpreter((struct room_data *) go, cmd);
      break;
    }
        if (dg_owner_purged) {
          depth--;
          return ret_val;
        }
      }

    }
  }

  free_varlist(GET_TRIG_VARS(trig));
  GET_TRIG_VARS(trig) = NULL;
  GET_TRIG_DEPTH(trig) = 0;

  depth--;
  return ret_val;
}

ACMD(do_tlist)
{

  int first, last, nr, found = 0;
  char pagebuf[65536];

  strcpy(pagebuf,"");

  two_arguments(argument, buf, buf2);

  if (!*buf) {
    send_to_char("Usage: tlist <begining number or zone> [<ending number>]\r\n", ch);
    return;
  }

  first = atoi(buf);
  if (*buf2) last = atoi(buf2);
  else {
    first *= 100;
    last = first+99;
  }

  if ((first < 0) || (first > 99999) || (last < 0) || (last > 99999)) {
    send_to_char("Values must be between 0 and 99999.\n\r", ch);
    return;
  }

  if (first >= last) {
    send_to_char("Second value must be greater than first.\n\r", ch);
    return;
  }

  for (nr = 0; nr < top_of_trigt && (trig_index[nr]->virtual <= last); nr++)
  {
    if (trig_index[nr]->virtual >= first) {
      sprintf(buf, "%5d. [%5ld] %s\r\n", ++found,
              trig_index[nr]->virtual,
              trig_index[nr]->proto->name);
      strcat(pagebuf, buf);
    }
  }

  if (!found)
    send_to_char("No triggers were found in those parameters.\n\r", ch);
  else page_string(ch->desc, pagebuf, TRUE);
}

int real_trigger(int vnum)
{
  int rnum;

  for (rnum=0; rnum < top_of_trigt; rnum++)
  {
    if (trig_index[rnum]->virtual==vnum) break;
  }

  if (rnum==top_of_trigt) rnum = -1;
  return (rnum);
}

ACMD(do_tstat)
{
  int vnum, rnum;
  char str[MAX_INPUT_LENGTH];

  half_chop(argument, str, argument);
  if (*str) {
    vnum = atoi(str);
    rnum = real_trigger(vnum);
    if (rnum<0) {
      send_to_char("That vnum does not exist.\r\n", ch);
      return;
    }

    do_stat_trigger(ch, trig_index[rnum]->proto);
  } else send_to_char("Usage: tstat <vnum>\r\n", ch);
}

/*
* scans for a case/default instance
* returns the line containg the correct case instance, or the last
* line of the trigger if not found.
*/
struct cmdlist_element *
find_case(struct trig_data *trig, struct cmdlist_element *cl,
          void *go, struct script_data *sc, int type, char *cond)
{
  char result[MAX_INPUT_LENGTH];
  struct cmdlist_element *c;
  char *p, *buf;

  eval_expr(cond, result, go, sc, trig, type);
  
  if (!(cl->next))
    return cl;  
        
  for (c = cl->next; c->next; c = c->next) {
    for (p = c->cmd; *p && isspace(*p); p++);
      
    if (!strn_cmp("while ", p, 6) || !strn_cmp("switch", p, 6))
      c = find_done(c);
    else if (!strn_cmp("case ", p, 5)) {
      CREATE( buf, char, MAX_STRING_LENGTH );
#if 0 /* the original implementation */
      sprintf(buf, "(%s) == (%s)", cond, p + 5);
      if (process_if(buf, go, sc, trig, type)) {
#else /* new! improved! bug fixed! */
      eval_op("==", result, p + 5, buf, go, sc, trig);
      if (*buf && *buf!='0') {
#endif
        free(buf);
        return c;
      }
      free(buf);
    } else if (!strn_cmp("default", p, 7))
      return c;
    else if (!strn_cmp("done", p, 3))   
     return c;
  }
  return c;
}        
       
/*
* scans for end of while/switch-blocks.   
* returns the line containg 'end', or the last
* line of the trigger if not found.     
*/
struct cmdlist_element *
find_done(struct cmdlist_element *cl)
{
  struct cmdlist_element *c;
  char *p;
  
  if (!(cl->next))
    return cl;

  for (c = cl->next; c->next; c = c->next) {
    for (p = c->cmd; *p && isspace(*p); p++);

    if (!strn_cmp("while ", p, 6) || !strn_cmp("switch ", p, 7))
      c = find_done(c);
    else if (!strn_cmp("done", p, 3))
      return c;
  }
    
  return c;
}

ACMD(do_dgstat)
{
  struct char_data *victim = 0;
  struct obj_data *object = 0;
  int tmp;

  half_chop(argument, buf1, buf2);

  if (!*buf1) {
    send_to_char("DGStats on who or what?\r\n", ch);
    return;
  } else if (is_abbrev(buf1, "room")) {
    do_sstat_room(ch);
  } else if (is_abbrev(buf1, "mob")) {
    if (!*buf2)
      send_to_char("DGStats on which mobile?\r\n", ch);
    else {
      if ((victim = get_char_vis(ch, buf2)))
        do_sstat_character(ch, victim);
      else
        send_to_char("No such mobile around.\r\n", ch);
    }
  } else if (is_abbrev(buf1, "player")) {
    if (!*buf2) {
      send_to_char("DGStats on which player?\r\n", ch);
    } else {
      if ((victim = get_player_vis(ch, buf2, 0)))
        do_sstat_character(ch, victim);
      else
        send_to_char("No such player around.\r\n", ch);
    }
  } else if (is_abbrev(buf1, "file")) {
    if (!*buf2) {
      send_to_char("DGStats on which player?\r\n", ch);
    } else {
      if (NULL != (victim = xmlload_char(buf2))) {
        if (GET_LEVEL(victim) > GET_LEVEL(ch))
          send_to_char("Sorry, you can't do that.\r\n", ch);
        else {
          GET_POS(victim) = POS_STANDING;
          do_sstat_character(ch, victim);
        }
        free_char(victim);
      } else {
        send_to_char("There is no such player.\r\n", ch);
      }
    }
  } else if (is_abbrev(buf1, "object")) {
    if (!*buf2)
      send_to_char("Stats on which object?\r\n", ch);
    else {
      if ((object = get_obj_vis(ch, buf2)))
        do_sstat_object(ch, object);
      else
        send_to_char("No such object around.\r\n", ch);
    }
  } else {
    if ((object = get_object_in_equip_vis(ch, buf1, ch->equipment, &tmp)))
      do_sstat_object(ch, object);
    else if ((object = get_obj_in_list_vis(ch, buf1, ch->carrying)))
      do_sstat_object(ch, object);
    else if ((victim = get_char_room_vis(ch, buf1)))
      do_sstat_character(ch, victim);
    else if ((object = get_obj_in_list_vis(ch, buf1, world[ch->in_room].contents)))
      do_sstat_object(ch, object);
    else if ((victim = get_char_vis(ch, buf1)))
      do_sstat_character(ch, victim);
    else if ((object = get_obj_vis(ch, buf1)))
      do_sstat_object(ch, object);
    else
      send_to_char("Nothing around by that name.\r\n", ch);
  }
}

/* XML support for DGS saving/loading */
xmlNodePtr makeDGScript(struct trig_proto_list *t, xmlDocPtr doc)
{
  char buf[4096];

  xmlNodePtr script = xmlNewNode(NULL,"dgscript");
  script->doc = doc;

  sprintf(buf,"%d",t->vnum);
  xmlNewProp(script, "vnum" ,buf);

  return script;
}

void fetchDGScript(xmlNodePtr script, void *item, int type)
{
  struct char_data *ch;
  struct obj_data *object;
  struct room_data *room;
  struct trig_proto_list *trig = NULL, *trig_tmp;

  CREATE(trig, struct trig_proto_list, 1);

  trig->vnum = atoi(xmlGetProp(script,"vnum"));

  if (type==MOB_TRIGGER)
  {
   ch = (struct char_data *) item;
   for (trig_tmp = ch->proto_script; trig_tmp && trig_tmp->next; trig_tmp = trig_tmp->next);

   if (!trig_tmp)
     ch->proto_script = trig;
   else
     trig_tmp->next = trig;
  }
  else if (type==OBJ_TRIGGER)
  {
   object = (struct obj_data *) item;
   for (trig_tmp = object->proto_script; trig_tmp && trig_tmp->next; trig_tmp = trig_tmp->next);

   if (!trig_tmp)
     object->proto_script = trig;
   else
     trig_tmp->next = trig;
  }
  else if (type==WLD_TRIGGER)
  {
   room = (struct room_data *) item;
   for (trig_tmp = room->proto_script; trig_tmp && trig_tmp->next; trig_tmp = trig_tmp->next);

   if (!trig_tmp)
     room->proto_script = trig;
   else
     trig_tmp->next = trig;
  }
  else {
   alog("SYSERR: Invalid type passed to script_save_mobobj_to_disk()");
   return;
  }
}

xmlNodePtr DGVariables_to_XML(struct char_data *ch)
{
  xmlNodePtr variables, temp;
  struct trig_var_data *vars;

  if (!ch) {
    return NULL;
  }

  variables = xmlNewNode(NULL, "dgvariables");
  vars = ch->script->global_vars;

  while (vars) {
    if (*vars->name != '-') /* don't save if it begins with - */
    {
     temp = xmlNewChild(variables, NULL, "variable", NULL);
     sprintf(buf, "%s", vars->name);
     xmlNewProp(temp, "name", buf);
     sprintf(buf, "%ld", vars->context);
     xmlNewProp(temp, "context", buf);
     sprintf(buf, "%s", vars->value);
     xmlNewProp(temp, "value", buf);
    }
    vars = vars->next;
  }

  return variables;
}

void XML_to_DGVariables(xmlNodePtr node, struct char_data *ch)
{
  xmlNodePtr temp;

  if (!node || !ch) {
    return;
  }

  if (strcasecmp("dgvariables", node->name)) {
    return;
  }

  for (temp = node->childs; temp; temp = temp->next)
    if (!strcasecmp(temp->name, "variable"))
      add_var(&(SCRIPT(ch)->global_vars), xmlGetProp(temp, "name"),
              xmlGetProp(temp, "value"), atol(xmlGetProp(temp, "context")));
}

