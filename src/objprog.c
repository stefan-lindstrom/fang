/******************************************************************************
 * $Id: objprog.c,v 1.5 2003/03/18 13:21:55 fang Exp $
 *   
 * File: objprog.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: objprog.c,v $
 *   Revision 1.5  2003/03/18 13:21:55  fang
 *   Some mem. bugs reported by Valgrind crushed.
 *
 *   Revision 1.4  2002/11/07 04:02:03  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.3  2002/09/09 04:26:13  fang
 *   Drath: Final bug fixes (we hope) for AEDIT crashes
 *
 *   Revision 1.2  2002/07/28 13:56:04  fang
 *   Removed dmlloc...just to test.
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.4  2002/06/12 09:55:16  fang
 *   More mem thingies...
 *
 *   Revision 1.3  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.6  2002/01/24 21:33:17  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.5.2.1  2002/01/22 03:05:15  mud
 *   More cleanup. Removed extern decls from source files. Protected some DG headers from multiple inclision
 *
 *   Revision 1.5  2000/08/24 12:46:22  mud
 *   Fixed mob progs bug with new guild code.
 *
 *   Revision 1.4  2000/05/13 14:54:59  mud
 *   *blush* Re-disable of objprogs.
 *
 *   Revision 1.3  2000/05/13 13:57:57  mud
 *   No name flag for boards, where players names won't show up on boards
 *   with the ITEM_NONAMEBOARD flag.
 *
 *   Revision 1.2  2000/02/21 14:00:18  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/********************************************************************/
/* Obj program test, made by Fredrik Lännergren for Circle 3 bpl 11 */
/********************************************************************/

#include "class.h"
#include "conf.h"
#include "sysdep.h"
#include "constants.h"
#include "objprog.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "spells.h"
#include "screen.h"
#include "guild.h"

void parse_objprog(FILE *obj_f, int i, int vnum)
{
  char line[MAX_INPUT_LENGTH], arg[MAX_INPUT_LENGTH], *temp = NULL, 
       mainarg[MAX_INPUT_LENGTH], sidearg[MAX_INPUT_LENGTH], 
       actarg[MAX_INPUT_LENGTH];
  int imain= 0, side = 0, main_intarg= 0, end = FALSE, not = FALSE, action = 0,
  act_intarg = 0, sidecount = 0, j = 0;
  struct obj_prog_list *present_main, *last_main = NULL;
  struct side_term_list *present_side, *last_side = NULL;
  struct obj_action_list *present_action, *last_action = NULL;

  memset(line   ,'\0',MAX_INPUT_LENGTH);
  memset(arg    ,'\0',MAX_INPUT_LENGTH);
  memset(mainarg,'\0',MAX_INPUT_LENGTH);
  memset(sidearg,'\0',MAX_INPUT_LENGTH);
  memset(actarg ,'\0',MAX_INPUT_LENGTH);


  CREATE(obj_proto[i].oprog, struct obj_prog_list, 1);
  CREATE(obj_proto[i].oprog->sideterms, struct side_term_list, 1);
  CREATE(obj_proto[i].oprog->actions, struct obj_action_list, 1);
  
  present_main = obj_proto[i].oprog;
  present_side = obj_proto[i].oprog->sideterms;
  present_action = obj_proto[i].oprog->actions;
  
  get_line(obj_f, line);
  while (strcasecmp(line, "Q")) {
    temp = one_argument(line, arg);
    for (j = 1; j <= NUM_OBJ_MAINTERMS; j++) 
      if (!strcasecmp(arg, omt_file[j]))
       imain = j;
    if (!imain) {
      sprintf(buf, "Error in obj #%d's obj prog parsing. Unknown main condition.\r\n", vnum);
      mlog(buf);
      exit(1);
    }
    /* Argument is an integer */
    if ((imain == OPC_TIME) || (imain == OPC_RANDOM)){      
      temp = one_argument(temp,arg);                     
      mainarg[0] = '\0';
      main_intarg = atoi(arg);
      if (!main_intarg) {
        sprintf(buf, "Unknown integer argument in main arg to obj %d.", vnum);
        mlog(buf);
        exit(1);
      }
    }
    else if ((imain == OPC_ACTIONROOM) || (imain == OPC_ACTIONTOROOM) ||
             (imain == OPC_ACTIONWEAR) || (imain == OPC_ACTIONTOWEAR) ||
             (imain == OPC_ACTIONCARRY) || (imain == OPC_ACTIONTOCARRY) ||
             (imain == OPC_SAYROOM) || (imain == OPC_SAYWEAR) ||
             (imain == OPC_SAYCARRY)) { /* Arg is a string */
      temp = one_word(temp, mainarg);
      main_intarg = 0;
      if (!*mainarg){
        sprintf(buf, "Bad arg list (between \" \") in objprog for %d.", vnum);
        mlog(buf);
        exit(1);
      }
    }
    present_main->main = imain; 			/* Copy over what we've got so far */
    present_main->intarg = main_intarg;
    present_main->argument = str_dup(mainarg);
    CREATE(present_main->next, struct obj_prog_list, 1); /* Create a new main condition struct */
    last_main = present_main;
    present_main = present_main->next;			 /* Move over to next main struct */
    present_main->next = NULL;
/*ok, so far so good...We have now found the main condition with args.. Now for side terms */
    end = FALSE;
    sidecount = 0;
    while (!end) {
      not = FALSE;
      temp = one_argument(temp, arg);
      if (!strcmp(arg,";") || !*arg || !strcmp(" ",arg))
        end = TRUE;
      else if (!strcmp(arg, "&")) {  /* New arg found, but which? */
        temp = one_argument(temp, arg);
        if (!strcmp(arg, "!")) {      /* Catch the negating sign */
          not = TRUE;
          temp = one_argument(temp, arg);
        }
        if (!*arg) /* What the..? No side condition after & or !...*/{
          alog("Unexpected end in argument line, objprog for obj %d.", vnum);
          exit(1);
        }
        for (j = 1; j <= NUM_OBJ_SIDETERMS; j ++)
          if (!strcasecmp(arg,ost_file[j])) /* Ok, time to see if it's a valid side condition */
            side = j;
        
        if (!side) {
          alog("Unknown side term '%s' in obj prog for obj %d.", arg, vnum);
          exit(1);
        }
        sidearg[0] = '\0';
        if ((side == OPC_SEX) || (side == OPC_RACE) || (side == OPC_GUILD) || (side == OPC_LEVEL) ||
        (side == OPC_WEARINGN) || (side == OPC_CARRYINGN) || 
    (OPC_GRANKIS == side) || (OPC_GRANKLESS == side) || (OPC_GRANKMORE == side)) { /* Argument for side condition is a string. */
          temp = one_word(temp, sidearg);
          if (!sidearg) {
            alog("String in side argument wrong (\"  \" expected) in objprog for %d.", vnum);
            exit(1);
          }
        }
        else if ((side == OPC_CARRYING) || (side == OPC_WEARING) || 
                 (side == OPC_STATUS) || (side == OPC_QUESTED)){
          temp = one_argument(temp, sidearg);
          if ((!*sidearg) || (!atoi(sidearg))) {
            alog("Integer argument for side condition invalid on objprog for obj %d.", vnum);
            exit(1);
          }
        }
      }
      else {
        alog("Unexpected token in term list, eol,; or & expected. %s received.", arg);
        exit(1);       
      }
/* Ok...it seems the side condition syntax is ok, let's add it in ! */
      if (!end) {
        present_side->number = side;
        present_side->not = not;
        if ((side == OPC_WEARING) || (side == OPC_CARRYING) || (side == OPC_STATUS))
          present_side->int_arg = atoi(sidearg);
        else if ((side == OPC_LEVEL) || (side == OPC_RACE) || (side == OPC_GUILD) ||
                 (side == OPC_WEARINGN) || (side == OPC_CARRYINGN) ||
                 (side == OPC_SEX))
          present_side->argument = strdup(sidearg);
        else {
          present_side->argument = NULL;
          present_side->int_arg = 0;
        }
        CREATE(present_side->next, struct side_term_list, 1);
        last_side = present_side;
        present_side = present_side->next; /* Step to next side argument */
        present_side->next = NULL;
        sidecount ++;
      }
    } 
    free(present_side);      /* Free the last struct that never is used */

    CREATE(present_main->sideterms, struct side_term_list, 1); /* Create first sideterm for next proglist */
    present_side = present_main->sideterms; 
    
    if (!sidecount)
      last_main->sideterms = NULL;
    else
      last_side->next = NULL;  
      
    /* All conditions taken care of, time to parse the action list */
    get_line(obj_f, line);
    while (strcasecmp(line, "End")) {
      temp = one_argument(line, arg);
      if (!*arg) { /* Strange...an empty line? */
        alog("Error: Empty line found in action list, objprog for obj %d", vnum);
        exit(1);
      }
      for (j = 1; j <= NUM_OBJ_ACTIONS; j++)
        if (!strcasecmp(arg,obj_act_file[j]))
          action = j;

      if (!action) {  
        alog("Unknown action '%s' in action list, objprog for obj %d.", arg, vnum);
        exit(1);
      }
      if ((action == OPA_GOTO) || (action == OPA_WEARDOWN) || (action == MPA_PAUSE)) {
        temp = one_argument(temp, arg);
        if (!*arg) {
          alog("Argument expected for command in actionlist, objprog for obj %d", vnum);
          exit(1);
        }
        act_intarg = atoi(arg);
        *actarg = 0;
      }
      else if ((action != OPA_STOP) && (action != OPA_DESTROY)) {
        temp = one_word(temp, actarg);
        if (actarg == NULL) {
          alog("Error: Argument expected for action in objprog for obj %d",vnum);
          exit(1);
        }
        act_intarg = 0;
      }    
      else {
        *actarg = 0;
        act_intarg = 0;
      }
/* It seems the command line syntax is accurate, time to add it in the list */
      present_action->action = action;
      if (*actarg)
        present_action->argument = str_dup(actarg);
        
      present_action->intarg = act_intarg;
 
      CREATE(present_action->next, struct obj_action_list, 1);
      last_action = present_action;
      present_action = present_action->next;
      get_line(obj_f, line);
    }
    last_action->next = NULL;
    free(present_action);
    CREATE(present_main->actions, struct obj_action_list, 1);
    present_action = present_main->actions;
    get_line(obj_f, line);
  }
  if (last_side)
    last_side->next = NULL;
  free(present_side);
  last_action->next = NULL;
  free(present_action);
  last_main->next = NULL;
  free(present_main);
}

int obj_side_conditions_match(struct char_data *ch, struct obj_data *obj, struct obj_prog_list *program) {
  struct side_term_list *list;
  struct obj_data *o = NULL;
  int match = 1, figure = 0,  i, partmatch = 0;
  char arg1[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH];

  return FALSE;  

  if (program->sideterms == NULL) /* No sideterms? Then no checks needed */
    return TRUE;

  if (IS_NPC(ch))  /* Mobs can never trigger obj progs */
    return FALSE; 
  
  for (list = program->sideterms; list && match; list = list->next) {
    partmatch = FALSE;    
    switch (list->number) {
      case OPC_SEX:
        if (((GET_SEX(ch) == SEX_MALE) && 
              !strncasecmp("male", list->argument, strlen(list->argument))) ||
              ((GET_SEX(ch) == SEX_FEMALE) && 
              !strncasecmp("female", list->argument, strlen(list->argument))))
          partmatch = TRUE;
      break;	
      case OPC_LEVEL:
        two_arguments(list->argument, arg1, arg2);
        if ((GET_LEVEL(ch) >= atoi(arg1)) && (GET_LEVEL(ch) <= atoi(arg2)))
          partmatch = TRUE;
      break;
      case OPC_WOLFKIN:
        if (GET_ISWOLFKIN(ch))
          partmatch = TRUE;
      break;      
      case OPC_GRANKIS:
    if (!list->argument) 
      alog("SYSERR: objprog() -> Got NULL pointer arg.");
    partmatch = TRUE;
    break;
      case OPC_GRANKLESS:
        if (!list->argument)
          alog("SYSERR: objprog() -> Got NULL pointer arg.");
    partmatch = TRUE;
    break;
      case OPC_GRANKMORE:
        if (!list->argument)
          alog("SYSERR: objprog() -> Got NULL pointer arg.");
    partmatch = TRUE;
    break;
      case OPC_DARKFRIEND:
        if (is_dark(ch))
          partmatch = TRUE;
      break;
      case OPC_CARRYING:
        figure = list->int_arg;
        for (o = ch->carrying; o; o = o->next_content) 
          if (GET_OBJ_VNUM(o) == figure)
            partmatch = TRUE;
      break;
      case OPC_WEARING:
        figure = list->int_arg;
        for (i = 0; i < NUM_WEARS-1; i ++)
          if ((o = GET_EQ(ch, i)))
            if (GET_OBJ_VNUM(o) == figure)
              partmatch = TRUE;
      break;
      case OPC_RACE:	
        for (i = 0; !partmatch && strcmp(hometown[i], "\n"); i ++)
          if (!strncasecmp(hometown[i], list->argument, strlen(list->argument)))
            partmatch = TRUE;
      break;
      case OPC_HURT:
        if (GET_MAX_HIT(ch) > GET_HIT(ch))
          partmatch = TRUE;
      break;
      case OPC_TIRED:
        if (GET_MAX_MOVE(ch) > GET_MOVE(ch))
          partmatch = TRUE;
      break;
      case OPC_GUILD:
    if (get_guild_by_name(ch, list->argument))
      partmatch = TRUE;
      break;
      case OPC_NOBILITY:
        if (GET_NOBILITY(ch))
          partmatch = TRUE;
      break;
      case OPC_SNEAKING:
        if (IS_AFFECTED(ch, AFF_SNEAK))
          partmatch = TRUE;
      break;
      case OPC_BLIND:
        if (IS_AFFECTED(ch, AFF_BLIND))
          partmatch = TRUE;
      break;
      case OPC_GRASPING:
        if (IS_SET(PRF_FLAGS(ch), PRF_GRASPING))
          partmatch = TRUE;
      break;
      case OPC_TAVEREN:
        if (GET_TAVEREN(ch))
          partmatch = TRUE;
      break;
      case OPC_STATUS:
        if (GET_OBJ_STATUS(obj) >= list->int_arg) 
          partmatch = TRUE;
      break;
      case OPC_SLEEPING:
        if (GET_POS(ch) == POS_SLEEPING)
          partmatch = TRUE;
      break;
      case OPC_QUESTING:
        if (IS_SET(PRF_FLAGS(ch), PRF_QUEST))
          partmatch = TRUE;
      break;
      case OPC_WEARINGN:
        strcpy(arg2, list->argument);
        while (*arg1 && !partmatch) {
          strcpy(arg2, one_argument(arg2, arg1));        
          for (i = 0; !partmatch && (i < NUM_WEARS-1); i ++)
            if ((o = GET_EQ(ch, i)))
              if (isname(arg1, o->name))
                partmatch = TRUE;        
        }
      break;
      case OPC_CARRYINGN:
        strcpy(arg2, list->argument);
        while (!partmatch && *arg1) {
          strcpy(arg2, one_argument(arg2, arg1)); 
          for (o = ch->carrying; o && !partmatch; o = o->next_content) 
            if (isname(arg1, o->name))
              partmatch = TRUE;
        }
      break;
      case OPC_OUTSIDE:
        if (OUTSIDE(ch))
          partmatch = TRUE;
      break;
      case OPC_CHANNELER:
        if (ch && !cant_channel(ch))
          partmatch = TRUE;
      break;
      case OPC_QUESTED:
        if ((list->int_arg < 320)
        && ch && (ch->player_specials->saved.AutoQuest[list->int_arg/32] & (1 << (list->int_arg%32)))) 
          partmatch = TRUE;
      break;
      case OPC_AUTHORIZED:
        if (ch && GET_AUTHORIZED(ch))
          partmatch = TRUE;
      break;
      default:
        mlog("Objprog error: Unknown side condition value (objprog.c, obj_side_conditions_match())");
      break;
    }
    if (list->not) 
      partmatch = (!partmatch);
    if (!partmatch)
      match = FALSE;
  }

  return match;
}

int stops_in_objprog(struct obj_prog_list *program)
{
  struct obj_action_list *commands;

  return FALSE;  

  for (commands = program->actions; commands; commands = commands->next)
    if (commands->action == OPA_STOP)
      return TRUE;
  return FALSE;
}

int worn_by_char(struct char_data *ch,struct obj_data *obj)
{
  int j;

  for (j = 0; j < NUM_WEARS; j++) {
    if (GET_EQ(ch,j) && (GET_EQ(ch,j) == obj)) {
      return 1;
    }
  }
  return 0;
}

int in_inventory(struct char_data *ch,struct obj_data *obj)
{
  struct obj_data *hlp = ch->carrying;
  
  for (; hlp; hlp = hlp->next_content) {
    if (hlp == obj) {
      return 1;
    }
  }
  return 0;
}

int obj_main_condition_matches(struct char_data *ch, struct obj_data *target, 
    struct obj_data *obj, char *arg, struct obj_prog_list *program, 
    int mode, int cmd)
{  
  int i, match = 0;
  char *temp = NULL, temp2[MAX_INPUT_LENGTH], *temp3;
  
  return FALSE;  

  if (!program)
    return 0;
      
  if (ch && IS_NPC(ch)) /* Objs never react to mobs */
    return FALSE;

  i = program->main;

  /* Only tick, time and random can be true without a player starting it */
  if (!ch && (i !=OPC_TICK) && (i != OPC_TIME) && (i != OPC_RANDOM))
    return FALSE;

  if (mode != program->main)
    return 0;
  
  switch (mode){
    case OPC_DIE:
      match = TRUE;  
    break;    
    case OPC_TIME:
      i = atoi(program->argument);
      if (i == time_info.hours)
        match = TRUE;
    break;
    case OPC_SAYWEAR:
    case OPC_SAYROOM:
    case OPC_SAYCARRY:
      temp = str_dup(program->argument);
      while (!match && *temp) {
        temp = one_argument(temp, temp2);
        temp3 = temp2;
        if (isname(temp3, arg))
          match = TRUE;
      }
    break;
    case OPC_ACTIONTOROOM:
    case OPC_ACTIONTOWEAR:
    case OPC_ACTIONTOCARRY:
      temp = strdup(program->argument);
      one_argument(arg, temp2);
      if (*temp2 && isname(temp2, obj->name) &&
          isname(complete_cmd_info[cmd].command, temp))
        match = TRUE;
      else
        match = FALSE;
    break;
    case OPC_ACTIONROOM:
    case OPC_ACTIONWEAR:
    case OPC_ACTIONCARRY:
      strcpy(temp2, program->argument);
      if (isname(complete_cmd_info[cmd].command, temp2)) 
        match = TRUE;
    break;
    case OPC_RANDOM:
      if ((program->intarg < 100) && (program->intarg > number(0, 100)))
        match = TRUE;
    break;
    case OPC_LEAVE:
      if ((obj == target) && !in_inventory(ch,obj) && !worn_by_char(ch,obj))
        match = TRUE;
      else
        match = FALSE;
      break;
    case OPC_DROP:
    case OPC_WEAR:
    case OPC_TICK:
    case OPC_ENTRY:
    case OPC_TAKE:
    case OPC_REMOVE:
    case OPC_JUNK:
    case OPC_DONATE:
    case OPC_STEAL:
      if (obj == target)
        match = TRUE;
    break;
    default:
      match = FALSE;
    break;
  }

  return match;
}
    
int check_objprogs_at_interval(struct obj_data *obj, int mode)
{
  int i = 0, run = 0;
  struct obj_prog_list *prog;

  return FALSE;  

  if (!obj)
    return 0;

  if (GET_OBJPROG(obj)) 
  for (prog = GET_OBJPROG(obj); prog; prog = prog->next) {
    if (obj_main_condition_matches(NULL, obj, obj, NULL, prog, mode, 0) &&
      obj_side_conditions_match(NULL, obj, prog)) {
      run = 1;
      SET_BIT(GET_OP_ACTIVE(obj), (1 << i));
      if (i>30) {
        sprintf(buf, "Error in objprogram (obj %ld): Too many programs.", GET_OBJ_VNUM(obj));
        mlog(buf);
        return 0;
      }
    }  
    i++;          
  }   
  return run;
}

int obj_subcheck(struct char_data *ch, struct obj_data *target, char *arg,int mode, int cmd) 
{
  int run = 0, i = 0;
  struct obj_prog_list *program;

  if (GET_OBJPROG(target))
    for (program = GET_OBJPROG(target); program ; program = program->next) {
      if (obj_main_condition_matches(ch, target, target, arg, program, mode, cmd) &&
           obj_side_conditions_match(ch, target, program)) {
        run = MAX(1, run);
        SET_BIT(GET_OP_ACTIVE(target), (1 << i));
        if (stops_in_objprog(program))
          run = 2;  
      }
      i++;
    } 
  return run;

}
/* "list" is so far superfluous, remove if you want */
int check_for_objprogs_and_stops(struct char_data *ch,  struct obj_data *target, char *arg, struct obj_data *list,   int mode,  int cmd) 
{
  struct obj_data *objs = NULL, *next = NULL;
  int run = 0, i = 0;

  return FALSE;  

  if (target) 
    run = obj_subcheck(ch, target, arg, mode, cmd);
  else if (ch) { 
    if ((mode == OPC_ACTIONCARRY) || (mode == OPC_ACTIONTOCARRY) || (mode == OPC_SAYCARRY)) {
      for (objs = ch->carrying; objs ; objs = next) {
        next = objs->next_content;  
        run = MAX(run, obj_subcheck(ch, objs, arg, mode, cmd)); 
      }
    } 
    else if ((mode == OPC_ACTIONWEAR) || (mode == OPC_ACTIONTOWEAR) || (mode == OPC_SAYWEAR)) {
      for (i = 0; i < NUM_WEARS; objs = GET_EQ(ch, i++))
        if (objs)
          run = MAX(run, obj_subcheck(ch, objs, arg, mode, cmd));           
    }
    else if ((mode == OPC_ACTIONROOM) || (mode == OPC_ACTIONTOROOM) || (mode == OPC_SAYROOM)) {
      for (objs = world[ch->in_room].contents; objs ; objs = objs->next_content)
        run = MAX(run, obj_subcheck(ch, objs, arg, mode, cmd));  
    }
    else {
      for (objs = world[ch->in_room].contents; objs ; objs = objs->next_content)
        run = MAX(run, obj_subcheck(ch, objs, arg, mode, cmd));  
      for (i = 0; NUM_WEARS > i ; i++, objs = GET_EQ(ch, i))
        if (objs)
          run = MAX(run, obj_subcheck(ch, objs, arg, mode, cmd));           
      for (objs = ch->carrying; objs ; objs = objs->next_content) 
        run = MAX(run, obj_subcheck(ch, objs, arg, mode, cmd)); 
    }
  }
  return run;

}

void run_single_obj_program(struct obj_prog_list *program, struct char_data *ch,  struct obj_data *obj, int cont)
{

  struct obj_action_list *al;
  struct obj_data *tmpobj = obj;
  char arg[MAX_INPUT_LENGTH];
  int room = 0;

  if (!obj) 
    return;
  if ((cont) && (obj->paused_at))
    al = obj->paused_at;
  else if (program)
    al = program->actions;
  else {    
    obj->paused_at = NULL;
    return;
  }
  if (obj->in_obj)
    for (; tmpobj->in_obj; tmpobj = tmpobj->in_obj);

  if (ch) {
    if ((obj->in_room != NOWHERE) && (obj->in_room != ch->in_room)) 
      room  = obj->in_room;
    else
      room = ch->in_room;
  }
  else {
    if (tmpobj->carried_by) {
      ch = tmpobj->carried_by;
      room = ch->in_room;
    }
    else if (tmpobj->worn_by) {
      ch = tmpobj->worn_by;
      room = ch->in_room;
    }
    else if (tmpobj->in_room)
      room =  tmpobj->in_room;
  }

  obj->paused_at = NULL;

  for(; al; al = al->next) {      
    switch (al->action) {
      case OPA_GOTO:

    /*    if (obj->worn_by)
          unequip_char(ch, obj);  */

        if (obj->carried_by)
          obj_from_char(obj); 
        if (real_room(al->intarg) != NOWHERE)
          obj_to_room(obj, real_room(al->intarg));
        else if (ch)
          obj_to_char(obj, ch);
        break;
      case OPA_FORCE:
        sprintf(arg, "%s", al->argument);
        if (ch && (GET_LEVEL(ch) < LVL_IMMORT))
          command_interpreter(ch, arg);
      break;
      case OPA_WEARDOWN:
        GET_OBJ_STATUS(obj) -= al->intarg;        
      break;
      case OPA_PAUSE:
        obj->paused_at = al->next;
        obj->pause = 2*al->intarg;
        return;
      break;
      case OPA_EMOTE:
        sprintf(arg, "%s", al->argument);
        if (!ch && room)
          send_to_room(arg, room);
        else if (ch) 
          act(arg, TRUE, ch, obj, ch, TO_ROOM);
      break;
      case OPA_EMOTETO:
        sprintf(arg, "%s", al->argument);
        if (ch)
          act(arg, TRUE, ch, obj, ch, TO_CHAR);
      break;       
      case OPA_DESTROY: 
        extract_obj(obj);
        return;
      break;        
    }
  }

}

void subrun(struct char_data *ch, struct obj_data *obj) 
{

  struct obj_prog_list *program;
  int i;

  if (!obj)
    return;
  
  if (GET_OP_ACTIVE(obj) && GET_OBJPROG(obj)) {
    program = GET_OBJPROG(obj);
    for (i = 0; i<30 && obj && program; i ++, program = program->next) {
      if (IS_SET(GET_OP_ACTIVE(obj), (1 << i))) {
        run_single_obj_program(program, ch, obj, 0);
        REMOVE_BIT(GET_OP_ACTIVE(obj), (1 << i));
      }
    }
  }

}

void run_objprog(struct char_data *ch, struct obj_data *list)
{

  struct obj_data *obj = NULL, *next = NULL;
  int k = 0;

  if (!list)  { 
    for (k = 0, obj = GET_EQ(ch, 0); k < NUM_WEARS; obj = GET_EQ(ch, k), k++) {
      if (!obj)
        continue;
      subrun(ch, obj);
    }
    for (obj = ch->carrying; obj; obj = next) {
      next = obj->next_content;
      subrun(ch, obj);
    }
    if (NOWHERE == ch->in_room) {
       return; 
    } 

    for (obj = world[ch->in_room].contents; obj; obj = next) {
      next = obj->next_content;
      subrun(ch, obj);
    }
    return;
  } 
  
  for (obj = list; obj; obj = next) {
    next = obj->next_content;
    subrun(ch, obj);
  }

}

void save_objprog(FILE *obj_file, struct obj_data *obj)
{
  struct obj_prog_list *prog;
  struct side_term_list *side;
  struct obj_action_list *actn;
  int i = 0, not = 0;
  char temp[MAX_INPUT_LENGTH];
  
  if (!GET_OBJPROG(obj)) {
    mlog("SYSERR: Saving obj program: Program doesn't exist!\r\n");
    exit(1);
  }  
  fprintf(obj_file,"P\n");
  
  for (prog = GET_OBJPROG(obj); prog; prog = prog->next) {
    if (!prog->actions) 
      alog("Error in saving obj program (#%ld): Actions don't exist!", GET_OBJ_VNUM(obj));
    sprintf(buf, "%s", omt_file[prog->main]);
    i = prog->main;
    if ((i == OPC_ACTIONROOM) || (i == OPC_ACTIONTOROOM) ||
        (i == OPC_ACTIONWEAR) || (i == OPC_ACTIONTOWEAR) ||
        (i == OPC_ACTIONCARRY) || (i == OPC_ACTIONTOCARRY) ||
        (i == OPC_SAYROOM) || (i == OPC_SAYWEAR) ||
        (i == OPC_SAYCARRY)) 
      sprintf(buf, "%s \"%s\"", buf, prog->argument);     
    else if ((i == OPC_TIME) || (i == OPC_RANDOM))
      sprintf(buf, "%s %d", buf, prog->intarg);      
    
    fprintf(obj_file, buf);
     
    for (side = prog->sideterms; side; side = side->next) {
      not = side->not;
      i = side->number;
      if ((i == OPC_SEX) || (i == OPC_GUILD) || (i == OPC_WEARINGN) ||
          (i == OPC_RACE) || (i == OPC_CARRYINGN) || (i == OPC_LEVEL) ||
      (OPC_GRANKIS == i) || (OPC_GRANKLESS == i) || (OPC_GRANKMORE == i))
        sprintf(temp, " &%s%s \"%s\"", not?" ! ":" ", ost_file[i], side->argument);
      else if ((i == OPC_STATUS) ||  (i == OPC_CARRYING) ||  
               (i == OPC_QUESTED) || (i == OPC_WEARING))
        sprintf(temp, " &%s%s %d", not?" ! ":" ", ost_file[i], side->int_arg);
      else 
        sprintf(temp, " &%s%s", not?" ! ":" ", ost_file[i]);
      fprintf(obj_file, "%s", temp);
    }
    sprintf(buf, "\n");
    if (!prog->actions)
      sprintf(buf,"%sEmote $p has no actions defined!\r\n", buf);
    else
    for (actn = prog->actions; actn; actn = actn->next) {
      sprintf(buf, "%s%s", buf, obj_act_file[actn->action]);
      i = actn->action;
      if ( (i == OPA_FORCE) || (i == OPA_EMOTE) || (i == OPA_EMOTETO)) 
        sprintf(buf, "%s \"%s\"\r\n", buf, actn->argument);
      else if ((i == OPA_GOTO) || (i == OPA_WEARDOWN) || (i == OPA_PAUSE))
        sprintf(buf, "%s %d\n", buf, actn->intarg);
      else sprintf(buf, "%s\n", buf);
    }
    sprintf(buf,"%sEnd\n", buf);
    fprintf(obj_file, buf);
  }
  fprintf(obj_file,"Q\n");
}

void free_objprog(struct obj_data *obj)
{
  struct obj_prog_list   *objp, *opnext;
  struct side_term_list  *side, *snext;
  struct obj_action_list *act,  *anext;

  for (objp = obj->oprog; NULL != objp; objp = opnext) {
    opnext = objp->next;

    for (side = objp->sideterms; NULL != side; side = snext) {
      snext = side->next;
      FREEPTR(side->argument);
      free(side);
    }

    for (act = objp->actions; NULL != act; act = anext) {
      anext = act->next;
      FREEPTR(act->argument);
      free(act);
    }

    FREEPTR(objp->argument);
    free(objp);
  }
  obj->oprog = NULL;
}
