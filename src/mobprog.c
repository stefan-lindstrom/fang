/******************************************************************************
 * $Id: mobprog.c,v 1.8 2004/12/16 04:04:50 fang Exp $
 *   
 * File: mobprog.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: mobprog.c,v $
 *   Revision 1.8  2004/12/16 04:04:50  fang
 *   Err, saying a SYSERR for mobprog assignments was just misleading.
 *
 *   Revision 1.7  2003/06/10 12:16:08  fang
 *
 *   Added UNIQUE flag for mobs and objs. A unique obj/mob can only exists one
 *   instance at the time on the MUD, no matter what. A unique obj will not
 *   save to characters obj file either.
 *
 *   Revision 1.6  2002/11/07 04:02:03  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.5  2002/10/08 17:12:23  fang
 *   fischer: format changes and SYSERR message for mobprogs
 *
 *   Revision 1.4  2002/09/09 04:26:13  fang
 *   Drath: Final bug fixes (we hope) for AEDIT crashes
 *
 *   Revision 1.3  2002/08/30 21:23:09  fang
 *   Put in retainers.
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
 *   Revision 1.7  2002/01/24 21:33:17  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.6.2.2  2002/01/23 21:49:42  mud
 *   Removed objsave.[c|h] (old objsave stuff).
 *   Removed obj_file_elem and rent structs.
 *   Removed final vestiges of old guild system, except parts used
 *   in guild.[c|h] for backwards compatibility.
 *
 *   Revision 1.6.2.1  2002/01/21 01:23:41  mud
 *   Code cleanup. Aiel clans/socities gone. Lot of old guild code gone.
 *
 *   Revision 1.6  2001/01/22 12:11:40  mud
 *   *** empty log message ***
 *
 *   Revision 1.5  2000/08/24 12:46:22  mud
 *   Fixed mob progs bug with new guild code.
 *
 *   Revision 1.4  2000/03/29 04:55:43  mud
 *   Last update to mobprog.c in regards to changing atplayeraction.
 *
 *   Revision 1.3  2000/03/29 04:40:14  mud
 *   Changed the atplayeraction command in mobprogs. Now it will add in the players
 *   name after the first word in the argument..for usage in tells and sends etc.
 *
 *   Revision 1.2  2000/02/21 14:00:18  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/
/* Mob program test, made by Fredrik Lännergren for Circle 3 bpl 11 */

#include "constants.h"
#include "conf.h"
#include "sysdep.h"
#include "class.h"
#include "mobprog.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "spells.h"
#include "screen.h"
#include "act.h"
#include "guild.h"

void parse_mobprog(FILE *mob_f, int i, int vnum)
{
  char line[MAX_INPUT_LENGTH], arg[MAX_INPUT_LENGTH], *temp = NULL, 
       mainarg[MAX_INPUT_LENGTH], sidearg[MAX_INPUT_LENGTH], 
       actarg[MAX_INPUT_LENGTH];
  int imain= 0, side = 0, main_intarg= 0, end = FALSE, not = FALSE, action = 0,
  act_intarg = 0, sidecount = 0, j;

  struct mob_prog_list *present_main, *last_main = NULL;
  struct side_term_list *present_side, *last_side = NULL;
  struct mob_action_list *present_action, *last_action = NULL;

  memset(mainarg, 0, MAX_INPUT_LENGTH);
  alog("Assigning mobprog to mob #%d", vnum);

  CREATE(mob_proto[i].mprog, struct mob_prog_list, 1);
  CREATE(mob_proto[i].mprog->sideterms, struct side_term_list, 1);
  CREATE(mob_proto[i].mprog->actions, struct mob_action_list, 1);
  
  present_main = mob_proto[i].mprog;
  present_side = mob_proto[i].mprog->sideterms;
  present_action = mob_proto[i].mprog->actions;
  
  get_line(mob_f, line);
  if (feof(mob_f)) {
    alog("SYSERR: Unexpected end in mobprog for mob #%d", vnum);
    exit(1);
  }
  while (strcasecmp(line, "Q")) {
    temp = one_argument(line, arg);
    for (j = 1; j <= NUM_MAINTERMS; j++) 
      if (!strcasecmp(arg, mt_file[j]))
        imain = j;

    if (!imain) {
      sprintf(buf, "Error in mob #%d's mob prog parsing. Unknown main condition.\r\n", vnum);
      mlog(buf);
      exit(1);
    }
    /* Argument is an integer */
    if ((imain == MPC_GIVE) || (imain == MPC_TIME) || (imain == MPC_RANDOM)){      
      temp = one_argument(temp,arg);                     
      mainarg[0] = '\0';
      main_intarg = atoi(arg);
      if (!main_intarg) {
        sprintf(buf, "Unknown integer argument in main arg to mob %d.", vnum);
        mlog(buf);
        exit(1);
      }
    }
    else if ((imain == MPC_SAY)      || (imain == MPC_ACTION) || (imain == MPC_TELL) ||
             (imain == MPC_ACTIONTO) || (imain == MPC_SAYTO)) { /* Arg is a string */
      temp = one_word(temp, mainarg);
      main_intarg = 0;
      if (!*mainarg){
        sprintf(buf, "Bad arg list (between \" \") in mobprog for %d.", vnum);
        mlog(buf);
        exit(1);
      }
    }
    present_main->main = imain; 			/* Copy over what we've got so far */
    present_main->intarg = main_intarg;
    present_main->argument = str_dup(mainarg);
    CREATE(present_main->next, struct mob_prog_list, 1); /* Create a new main condition struct */
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
          sprintf(buf,"Unexpected end in argument line, mobprog for mob %d.", vnum);
          mlog(buf);
          exit(1);
        }
        for (j = 1; j <= NUM_SIDETERMS; j ++)
          if (!strcasecmp(arg,st_file[j])) /* Ok, time to see if it's a valid side condition */
            side = j;
        
        if (!side) {
          sprintf(buf, "Unknown side term '%s' in mob prog for mob %d.", arg, vnum);
          mlog(buf);
          exit(1);
        }
        sidearg[0] = '\0';
        if ((side == MPC_SEX) || (side == MPC_RACE) || (side == MPC_GUILD) || (side == MPC_LEVEL) ||
        (side == MPC_WEARINGN) || (side == MPC_CARRYINGN) ||
    (MPC_GRANKIS == side) || (MPC_GRANKLESS == side) || 
    (MPC_GRANKMORE == side)) { /* Argument for side condition is a string. */
          temp = one_word(temp, sidearg);
          if (!sidearg) {
            sprintf(buf, "String in side argument wrong (\"  \" expected) in mobprog for %d.", vnum);
            mlog(buf);
            exit(1);
          }
        }
        else if ((side == MPC_CARRYING) || (side == MPC_WEARING) || (side == MPC_MODE) || (side == MPC_QUESTED)){
          temp = one_argument(temp, sidearg);
          if ((!*sidearg) || (!atoi(sidearg))) {
            sprintf(buf, "Integer argument for side condition invalid on mobprog for mob %d.", vnum);
            mlog(buf);
            exit(1);
          }
        }
      }
      else {
        sprintf(buf, "Unexpected token in term list, eol,; or & expected. %s received.", arg);
        mlog(buf);
        exit(1);       
      }
/* Ok...it seems the side condition syntax is ok, let's add it in ! */
      if (!end) {
        present_side->number = side;
        present_side->not = not;
        if ((side == MPC_WEARING) || (side == MPC_CARRYING) || 
             (side == MPC_MODE) || (side == MPC_QUESTED))
          present_side->int_arg = atoi(sidearg);
        else if ((side == MPC_LEVEL) || (side == MPC_RACE) || (side == MPC_GUILD) ||
                 (side == MPC_WEARINGN) || (side == MPC_CARRYINGN) ||
                 (side == MPC_SEX) || MPC_GRANKIS == side || MPC_GRANKLESS == side || MPC_GRANKMORE == side)
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
    get_line(mob_f, line);
    while (strcasecmp(line, "End")) {
      if (feof(mob_f)) {
        alog("SYSERR: Unexpected end in mobprog for mob #%d.", vnum);
        exit(1);
      }
      temp = one_argument(line, arg);
      if (!*arg) { /* Strange...an empty line? */
        alog("Error: Empty line found in action list, mobprog for mob %d", vnum);
        exit(1);
      }
      for (j = 1; j <= NUM_MOBACTIONS; j++)
        if (!(strcasecmp(arg,act_file[j])))
          action = j;

      if (!action) {  
        alog("Unknown action '%s' in action list, mobprog for mob %d.", arg, vnum);
        exit(1);
      }
      if ((action == MPA_GIVE) || (action == MPA_MOVETO) || (action == MPA_MODE)
          || (action == MPA_PAUSE)) {
        temp = one_argument(temp, arg);
        if (!*arg) {
          alog("Argument expected for command in actionlist, mobprog for mob %d", vnum);
          exit(1);
        }
        act_intarg = atoi(arg);
        *actarg = 0;
      }
      else if ((action != MPA_STOP) || (action != MPA_PURGE))  {
        temp = one_word(temp, actarg);
        if (actarg == NULL) {
          alog("Error: Argument expected for action in mobprog for mob %d",vnum);
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
 
      CREATE(present_action->next, struct mob_action_list, 1);
      last_action = present_action;
      present_action = present_action->next;
      get_line(mob_f, line);
    }
    last_action->next = NULL;
    free(present_action);
    CREATE(present_main->actions, struct mob_action_list, 1);
    present_action = present_main->actions;
    get_line(mob_f, line);
  }
  if (last_side)
    last_side->next = NULL;
  if (present_side)
    free(present_side);
  if (last_action)
    last_action->next = NULL;
  if (present_action)
    free(present_action);
  if (last_main)
    last_main->next = NULL;
  if (present_main)
    free(present_main);
}

int side_conditions_match(struct char_data *ch, struct char_data *mob, struct mob_prog_list *program)
{
  struct side_term_list *list;
  struct obj_data *obj;
  int match = 1, figure = 0,  i, partmatch = 0;
  char arg1[MAX_INPUT_LENGTH], arg2[100];
  
  if (program->sideterms == NULL) /* No sideterms? Then no checks needed */
    return TRUE;
  
  for (list = program->sideterms; list && match; list = list->next) {
    partmatch = FALSE;
  
    if (IS_NPC(ch)) {
      if (list->number == MPC_MODE) {
          if (list->argument) {
            if (atoi(list->argument) <= GET_MOB_MODE(ch)) /* Only valid mob sideterm */
              return TRUE;
        }
      }
      return FALSE;  
    }    
    switch (list->number) {
      case MPC_CANCHANNEL:
        if (!cant_channel(ch))
          partmatch = TRUE;
      break;
      case MPC_GRANKIS:
    if (!list->argument) 
      alog("SYSERR: MPC_GRANKIS: Got NULL pointer as arg!");
      partmatch = TRUE;
    break;
      case MPC_GRANKLESS:
        if (!list->argument)  
          alog("SYSERR: MPC_GRANKLESS: Got NULL pointer as arg!");
        else
      partmatch = TRUE;
    break;
      case MPC_GRANKMORE:
        if (!list->argument)  
          alog("SYSERR: MPC_GRANKMORE: Got NULL pointer as arg!");
        else
      partmatch = TRUE;
    break;
      case MPC_SEX:
        if (!list || !list->argument) 
      alog("SYSERR: MPC_SEX: Got NULL pointer as arg!");
        else if (((GET_SEX(ch) == SEX_MALE) && 
              !strncasecmp("male", list->argument, strlen(list->argument))) ||
              ((GET_SEX(ch) == SEX_FEMALE) && 
              !strncasecmp("female", list->argument, strlen(list->argument))))
          partmatch = TRUE;
      break;	
      case MPC_LEVEL:
        two_arguments(list->argument, arg1, arg2);
        if ((GET_LEVEL(ch) >= atoi(arg1)) && (GET_LEVEL(ch) <= atoi(arg2)))
          partmatch = TRUE;
      break;
      case MPC_WOLFKIN:
        if (GET_ISWOLFKIN(ch))
          partmatch = TRUE;;
      break;      
      case MPC_DARKFRIEND:
        if (is_dark(ch))
          partmatch = TRUE;
      break;
      case MPC_CARRYING:
        figure = list->int_arg;
        for (obj = ch->carrying; obj; obj = obj->next_content) 
          if (GET_OBJ_VNUM(obj) == figure)
            partmatch = TRUE;
      break;
      case MPC_WEARING:
        figure = list->int_arg;
        for (i = 0; i < NUM_WEARS-1; i ++)
          if ((obj = GET_EQ(ch, i)))
            if (GET_OBJ_VNUM(obj) == figure)
              partmatch = TRUE;
      break;
      case MPC_RACE:	
        for (i = 0; !partmatch && strcmp(hometown[i], "\n"); i ++)
          if (!strncasecmp(hometown[i], list->argument, strlen(list->argument)))
            partmatch = TRUE;
      break;
      case MPC_HURT:
        if (GET_MAX_HIT(ch) > GET_HIT(ch))
          partmatch = TRUE;
      break;
      case MPC_TIRED:
        if (GET_MAX_MOVE(ch) > GET_MOVE(ch))
          partmatch = TRUE;
      break;
      case MPC_GUILD:
    if (get_guild_by_name(ch, list->argument))
      partmatch = TRUE;
      break;
      case MPC_NOBILITY:
        if (GET_NOBILITY(ch))
          partmatch = TRUE;
      break;
      case MPC_SNEAKING:
        if (IS_AFFECTED(ch, AFF_SNEAK))
          partmatch = TRUE;
      break;
      case MPC_BLIND:
        if (IS_AFFECTED(ch, AFF_BLIND))
          partmatch = TRUE;
      break;
      case MPC_GRASPING:
        if (IS_SET(PRF_FLAGS(ch), PRF_GRASPING))
          partmatch = TRUE;
      break;
      case MPC_TAVEREN:
        if (GET_TAVEREN(ch))
          partmatch = TRUE;
      break;
      case MPC_MODE:
        if (GET_MOB_MODE(mob) >= list->int_arg) 
          partmatch = TRUE;
      break;
      case MPC_SLEEPING:
        if (GET_POS(ch) == POS_SLEEPING)
          partmatch = TRUE;
      break;
      case MPC_INVISIBLE:
        if (!CAN_SEE(mob, ch))
          partmatch = TRUE;
      break;
      case MPC_QUESTING:
        if (IS_SET(PRF_FLAGS(ch), PRF_QUEST))
          partmatch = TRUE;
      break;
      case MPC_WEARINGN:
        strcpy(arg2, list->argument);
        while (*arg1 && !partmatch) {
          strcpy(arg2, one_argument(arg2, arg1));        
          for (i = 0; !partmatch && (i < NUM_WEARS-1); i ++)
            if ((obj = GET_EQ(ch, i)))
              if (isname(arg1, obj->name))
                partmatch = TRUE;        
        }
      break;
      case MPC_CARRYINGN:
        strcpy(arg2, list->argument);
        while (!partmatch && *arg1) {
          strcpy(arg2, one_argument(arg2, arg1)); 
          for (obj = ch->carrying; obj && !partmatch; obj = obj->next_content) 
            if (isname(arg1, obj->name))
              partmatch = TRUE;
        }
      break;
      case MPC_OUTSIDE:
        if (OUTSIDE(ch))
          partmatch = TRUE;
      break;
      case MPC_QUESTED:
        if (IS_SET(GET_AUTOQUEST(ch), (1 << list->int_arg)))
          partmatch = TRUE;
      break;
      case MPC_AUTHORIZED:
        if (GET_AUTHORIZED(ch))
          partmatch = TRUE;
       break;
      default:
        mlog("Mobprog error: Unknown side condition value (mobprog.c, side_conditions_match())");
      break;
    }
    if (list->not) 
      partmatch = (!partmatch);
    if (!partmatch)
      match = FALSE;
  }

  return match;
}

int stops_in_mobprog(struct mob_prog_list *program)
{
  struct mob_action_list *commands;

  for (commands = program->actions; commands; commands = commands->next)
    if (commands->action == MPA_STOP)
      return TRUE;
  return FALSE;
}

int main_condition_matches(struct char_data *ch, struct char_data *target, 
    struct char_data *mob, char *arg, struct mob_prog_list *program,
    int mode, int cmd, int num)
{  
  int i, match = 0;
  char *temp = NULL, temp2[MAX_INPUT_LENGTH], *temp3,*ptr;
  
  if (!program)
    return 0;
    
  /* Following if statement is to ensure that mobs don't react to eachother, */
  /* thus creating a deadlock of mobspam.				     */
  
  // Mustn't have recursively called mobprogs.
  if (ch && !IS_NPC(ch) && (ch->player_specials->forced)) 
    return FALSE;
  
  if (IS_NPC(ch)) // Mobs can't triger other mobs, mebbe this'll help?
    return FALSE;

  if (mode != program->main)
    return 0;
  
  
  switch (mode){
    case MPC_MOBENTRY:
    case MPC_MOBLEAVE:
      if (ch == mob)
        match = TRUE;
    break;
    case MPC_MOBDIE:
      if (target == mob)
        match = TRUE;
    break;
    case MPC_DIE:
      if (FIGHTING(mob))
        if (target == FIGHTING(mob))
          match = TRUE;  
    break;    
    case MPC_TIME:
      i = atoi(program->argument);
      if (i == time_info.hours)
        match = TRUE;
    break;
    case MPC_SAYTO:
      if (target != mob)
        return FALSE;
    case MPC_SAY:
      ptr = temp = str_dup(program->argument);
      while (!match && *temp) {
        temp = one_argument(temp, temp2);
        temp3 = temp2;
        if (isname(temp3, arg))
          match = TRUE;
      }
      free(ptr);
    break;
    case MPC_TELL:
      temp = strdup(program->argument);
      one_argument(arg, temp2);
      if ((target == mob) && *temp2 && isname(temp2, temp))
        match = TRUE;
      else
        match = FALSE;
      free(temp);
    break;
    case MPC_ACTIONTO:
      temp = strdup(program->argument);
      one_argument(arg, temp2);
      if (*temp2 && isname(temp2, mob->player.name) &&
          !strncasecmp(complete_cmd_info[cmd].command, temp, strlen(temp)))
        match = TRUE;
      else
        match = FALSE;
      free(temp);
    break;
    case MPC_ACTION:
      strcpy(temp2, program->argument);
      if (isname(complete_cmd_info[cmd].command, temp2)) 
        match = TRUE;
    break;
    case MPC_ATTACK:
      if (target == mob && mob && !FIGHTING(mob))
        match = TRUE;
    break;
    case MPC_GIVE:
      if ((target == mob) && (program->intarg == num))
        match = TRUE;
    break;
    case MPC_RANDOM:
      if ((program->intarg < 100) && (program->intarg > number(0, 100)))
        match = TRUE;
    break;
    case MPC_ENTRY:
    case MPC_LEAVE:
    case MPC_TICK:
      match = TRUE;
    break;
    
    default:
      match = FALSE;
    break;
  }

  return match;
}
    
int check_mobprogs_at_interval(struct char_data *mob, int mode)
{
  int i = 0, run = 0;
  struct mob_prog_list *prog;
  
  if (IS_NPC(mob) && GET_MOBPROG(mob)) 
  for (prog = GET_MOBPROG(mob); prog; prog = prog->next) {
    if (main_condition_matches(mob, mob, mob, NULL, prog, mode, 0, 0) &&
      side_conditions_match(mob, mob, prog)) {
      run = 1;
      SET_BIT(GET_MP_ACTIVE(mob), (1 << i));
      if (i>30) {
        sprintf(buf, "Error in mobprogram (mob %ld): Too many programs.", GET_MOB_VNUM(mob));
        mlog(buf);
        return 0;
      }
    }  
    i++;          
  }   
  return run;
}

int check_for_mobprogs_and_stops(struct char_data *ch, 
    struct char_data *target, char *arg, int mode, int cmd, int number)
{
  struct char_data *people = NULL;
  struct mob_prog_list *program;
  int run = 0, i = 0;

  /* Separate test required in case target is in another room */
  // Forced characters can't trigger programs.
  if (!IS_NPC(ch) && ch->player_specials->forced)
    return 0;
  if (target) {
    if (IS_NPC(target) && GET_MOBPROG(target)) {
      for (program = GET_MOBPROG(target); program; program = program->next, i++) {
        if (main_condition_matches(ch, target, target, arg, program, mode,cmd, number) &&
        side_conditions_match(ch, target, program)) {
          run = MAX(1, run);
          SET_BIT(GET_MP_ACTIVE(target), (1 << i));
          if (stops_in_mobprog(program))
            run = 2;
          if (i > 30) {
            sprintf(buf,"Error in mobprog (mob %ld): Too many programs.", GET_MOB_VNUM(target));
            mlog(buf);
            return 0;
          }
        }
      }
    }   
  }
  /* Now for the rest of the people in ch's room, in case there is no target */

  i = 0;
  for (people = world[ch->in_room].people; people ; people = people->next_in_room, i = 0) {
    if (IS_NPC(people) && GET_MOBPROG(people)) {            
      for (program = GET_MOBPROG(people); program ; program = program->next) {
        if (people == target)
          continue;
        if (main_condition_matches(ch, target, people, arg, program, mode, cmd, number) && 
             side_conditions_match(ch, people, program)) {
          run = MAX(1, run);
          SET_BIT(GET_MP_ACTIVE(people), (1 << i));
          if (stops_in_mobprog(program))
            run = 2;
          if (i>30) {
            sprintf(buf, "Error in mobprogram (mob %ld): Too many programs.", GET_MOB_VNUM(people));
            mlog(buf);
            return 0;
          }
        }
        i++;
      }
    }
  }
  return run;
}


void run_single_program(struct mob_prog_list *program, struct char_data *ch, 
      struct char_data *mob, int cont)
{
  struct mob_action_list *al;
  char arg[MAX_INPUT_LENGTH], arg2[MAX_STRING_LENGTH];
  struct obj_data *obj = NULL;
  int sp;

  if ((cont) && (mob->mob_specials.paused_at)) 
    al = mob->mob_specials.paused_at;
  else if (program)
    al = program->actions;
  else {     /* No program to execute! */
    mob->mob_specials.paused_at = NULL;
    return;
  }

  mob->mob_specials.paused_at = NULL;

  for(; al; al = al->next) {      
    switch (al->action) {
      case MPA_MOVETO:
        if (real_room(al->intarg) != NOWHERE)
          GET_MOB_GOINGTO(mob) = real_room(al->intarg);
        break;
      case MPA_MOVE:    
        sprintf(arg, "%s", al->argument);
        command_interpreter(mob, arg);
      break;
      case MPA_SAY:
        sprintf(arg,"%s", al->argument);
        do_say(mob, arg, 0, 0);
      break;
      case MPA_SAYTO:
        sprintf(arg, "%s %s",GET_NAME(ch), al->argument); 
        do_sayto(mob, arg, 0, 0);
      break;
      case MPA_ACTIONTO:
        buf[0] = buf2[0] = '\0';
        half_chop(al->argument, buf, buf2);
//        sprintf(arg, "%s %s", al->argument, GET_NAME(ch));
        if (buf2[0] != '\0')
          sprintf(arg, "%s %s %s", buf, GET_NAME(ch), buf2);
    else
      sprintf(arg, "%s %s", buf, GET_NAME(ch));
        command_interpreter(mob, arg);
      break;
      case MPA_ACTION:
        sprintf(arg, "%s", al->argument);
        command_interpreter(mob, arg);
      break;
      case MPA_FORCE:
        sprintf(arg, "%s", al->argument);
        ch->player_specials->forced = 1;  // Player is forced to do this
        if (GET_LEVEL(ch) < LVL_HBUILDER) {
          command_interpreter(ch, arg);
        }
      break;
      case MPA_MODE:
        GET_MOB_MODE(mob) = al->intarg;        
      break;
      case MPA_CAST:
        sp = find_skill_num(al->argument);
        if (!((sp < 1)||(sp > MAX_SPELLS))) {
          if (NULL != spell_list[sp]) {
            GET_SKILL(mob,sp) = 100;
            spell_list[sp](mob,ch,NULL,&world[ch->in_room],NULL,0,sp,0,1,1.0);
            GET_SKILL(mob,sp) = 0;
          }
        }
        break;
      case MPA_SHOUT:
        sprintf(arg,"shout %s",al->argument);
        command_interpreter(mob, arg);
        break;
      case MPA_GIVE:
	if (!check_unique_obj(real_object(al->intarg), al->intarg)) {
	  alog("SYSERR: Mobaction GIVE_TO failed. Object %d is unqiue and already existant on mud!",al->intarg);
	  return;
	}
        obj = read_object(al->intarg,VIRTUAL);
	if (NULL == obj) {
	  sprintf(arg,"SYSERR: Mobaction GIVE_TO failed. Object %d does"
		  " not exists!\n",al->intarg);
	  return;
        } 
        obj_to_char(obj,mob);
        one_argument(obj->name,arg2);
        sprintf(arg,"give %s %s",arg2,GET_NAME(ch));
        command_interpreter(mob,arg);
      break;
      case MPA_PAUSE:
        mob->mob_specials.paused_at = al->next;
        mob->mob_specials.pause = 2*al->intarg;
        return;
      break;
      case MPA_EMOTE:
        sprintf(arg, "%s", al->argument);
        act(arg, TRUE, mob, 0, ch, TO_ROOM);
      break;        
      case MPA_DOING:
        if ((ch) && !IS_NPC(ch)) {
          free(GET_DOING(ch));
          GET_DOING(ch) = (al->argument)?strdup(al->argument): NULL;
        }
      break;
      case MPA_PURGE:
        //extract_char(mob,FALSE);
    break;
    }
  }
}

void run_mobprog(struct char_data *ch, long room)
{
  struct char_data *people;
  struct mob_prog_list *program;
  int i = 0, rm;

  if (room)
    rm = room;
  else
    rm = ch->in_room;

  for (people = world[rm].people; people; people = people->next_in_room) 
    if (IS_NPC(people) && GET_MP_ACTIVE(people) && GET_MOBPROG(people)) {
      program = GET_MOBPROG(people);
      for (i = 0; i<30 && (program != NULL); i ++, program = program->next) {
        if (IS_SET(GET_MP_ACTIVE(people), (1 << i))) {
          run_single_program(program, ch, people, 0);
          REMOVE_BIT(GET_MP_ACTIVE(people), (1 << i));
        }
      }
    }   
}

void save_mobprog(FILE *mob_file, struct char_data *mob)
{
  struct mob_prog_list *prog;
  struct side_term_list *side;
  struct mob_action_list *actn;
  int i = 0, not = 0;
  char temp[MAX_INPUT_LENGTH];

  if (!mob) {
    mlog("SYSERR: Saving mobprog, mob doesn't exist.");
    exit(1);
  }
  if (!GET_MOBPROG(mob)) {
    alog("Error in saving mob program, mob #%d: Program doesn't exist!\r\n", GET_MOB_VNUM(mob));
    exit(1);
  }  
  fprintf(mob_file,"P\n");
  
  for (prog = GET_MOBPROG(mob); prog; prog = prog->next) {
    if (!prog->main || !prog->actions) {
      alog("Error in saving mob program for mob #%d: Data missing.\r\n", GET_MOB_VNUM(mob));
      exit(1);
    }
    sprintf(buf, "%s", mt_file[prog->main]);
    i = prog->main;
    if ((i == MPC_SAY) || (i == MPC_SAYTO) || (i == MPC_ACTION) ||
      (i == MPC_ACTIONTO) || (i == MPC_TELL))
      sprintf(buf, "%s \"%s\"", buf, prog->argument);     
    else if ((i == MPC_TIME) || (i == MPC_GIVE) || (i == MPC_RANDOM))
      sprintf(buf, "%s %d", buf, prog->intarg);      
    
    fprintf(mob_file, buf);
     
    for (side = prog->sideterms; side; side = side->next) {
      not = side->not;
      i = side->number;
      if ((i == MPC_SEX) || (i == MPC_GUILD) || (i == MPC_WEARINGN) ||
          (i == MPC_RACE) || (i == MPC_CARRYINGN) || (i == MPC_TELL) ||
          (i == MPC_LEVEL) || (MPC_GRANKIS == i) || (MPC_GRANKLESS == i) ||
      (MPC_GRANKMORE == i))
        sprintf(temp, " &%s%s \"%s\"", not?" ! ":" ", st_file[i], side->argument);
      else if ((i == MPC_MODE) || (i == MPC_CARRYING) ||  
               (i == MPC_WEARING)  || (i == MPC_QUESTED))
        sprintf(temp, " &%s%s %d", not?" ! ":" ", st_file[i], side->int_arg);
      else 
        sprintf(temp, " &%s%s", not?" ! ":" ", st_file[side->number]);
      fprintf(mob_file, "%s", temp);
    }
    sprintf(buf, "\n");
    for (actn = prog->actions; actn; actn = actn->next) {
      sprintf(buf, "%s%s", buf, act_file[actn->action]);
      i = actn->action;
      if ((i == MPA_SAY) || (i == MPA_SAYTO) || (i == MPA_ACTION) ||
      (i == MPA_ACTIONTO) || (i == MPA_FORCE) || (i == MPA_CAST) ||
      (i == MPA_EMOTE) || (i == MPA_MOVE) || (i == MPA_DOING)) 
        sprintf(buf, "%s \"%s\"\r\n", buf, actn->argument);
      else if ((i == MPA_MOVETO) || (i == MPA_MODE) || (i == MPA_GIVE) ||
               (i == MPA_PAUSE)) 
        sprintf(buf, "%s %d\n", buf, actn->intarg);
      else sprintf(buf, "%s\n", buf);
    }
    sprintf(buf,"%sEnd\n", buf);
    fprintf(mob_file, buf);
  }
  fprintf(mob_file,"Q\n");
}

void free_mobprog(struct char_data *ch)
{
  struct mob_action_list *act,   *anext;
  struct mob_prog_list   *mprog, *mnext;
  struct side_term_list  *side,  *snext;

  for (mprog = ch->mprog; NULL != mprog; mprog = mnext) {
    mnext = mprog->next;

    for (side = mprog->sideterms; NULL != side; side = snext) {
      snext = side->next;

      FREEPTR(side->argument);
      free(side);
    }

    for (act = mprog->actions; NULL != act; act = anext) {
      anext = act->next;

      FREEPTR(act->argument);
      free(act);
    }

    FREEPTR(mprog->argument);
    mprog->argument = NULL;
    free(mprog);
  }

  ch->mprog = NULL;
}
