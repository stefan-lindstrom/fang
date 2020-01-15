/* ************************************************************************
*   File: act.movement.c                                Part of CircleMUD *
*  Usage: movement commands, door handling, & sleep/rest/etc state        *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */
#include "bitvector.h" 
#include "graph.h"
#include "spec_procs.h"
#include "class.h"
#include "shop.h"
#include "magic.h"
#include "constants.h"
#include "conf.h"
#include "sysdep.h"
#include "fight.h"
#include "config.h"
#include "act.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "spells.h"
#include "xmlobjsave.h"
#include "guild.h"
#include "guild_parser.h"
#include "dg_scripts.h"
#include "retainer.h"

int char_sit_on_obj(struct char_data *ch, struct obj_data *obj)
{
  struct char_data *i = NULL, *list = world[obj->in_room].people;
  int counter = 1;

  if ((GET_OBJ_TYPE(obj) != ITEM_FURNITURE) || (GET_OBJ_VAL(obj, 0) == 1)
      || (GET_OBJ_VAL(obj, 0) == 3))
    return 0;

  for (i = list; i; i = i->next_in_room)
    if ((ch != i) && (GET_SEAT(i) == obj))
      counter++;

  if (counter > GET_OBJ_VAL(obj, 2))
    return 0;

  if (ch->in_room == -1 || ch->in_room != obj->in_room)
    char_to_room(ch,obj->in_room);

  switch (GET_POS(ch)) {
  case POS_STANDING:
    GET_SEAT(ch) = obj;
    GET_POS(ch) = POS_SITTING;
    break;
  default:
    return 0;
  }
  return 1;
}

int char_sleep_on_obj(struct char_data *ch, struct obj_data *obj)
{
  struct char_data *i = NULL, *list = world[obj->in_room].people;
  int counter = 1;

  if ((GET_OBJ_TYPE(obj) != ITEM_FURNITURE) || (GET_OBJ_VAL(obj, 0) == 1)
      || (GET_OBJ_VAL(obj, 0) == 3))
    return 0;

  for (i = list; i; i = i->next_in_room)
    if ((ch != i) && (GET_SEAT(i) == obj))
      counter++;

  if (counter > GET_OBJ_VAL(obj, 2))
    return 0;

  if (ch->in_room == -1 || ch->in_room != obj->in_room)
    char_to_room(ch,obj->in_room);

  switch (GET_POS(ch)) {
  case POS_STANDING:
  case POS_SITTING:
  case POS_RESTING:

    if (!cant_channel(ch)) {
      if (IS_SET(PRF_FLAGS(ch),PRF_GRASPING)) {
        REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
      }
        remove_char_maintained(ch);
      remove_room_maintained(ch);
    }

    GET_SEAT(ch) = obj;
    GET_POS(ch) = POS_SLEEPING;
    break;
  default:
    return 0;
  }

  return 1;
}
 
int is_guild_member = 0;

/* simple function to determine if char can walk on water */
int has_boat(struct char_data *ch)
{
  struct obj_data *obj;
  int i;

  /* non-wearable boats in inventory will do it */
  for (obj = ch->carrying; obj; obj = obj->next_content)
    if (GET_OBJ_TYPE(obj) == ITEM_BOAT && (find_eq_pos(ch, obj, NULL) < 0))
      return 1;

  /* and any boat you're wearing will do it too */
  for (i = 0; i < NUM_WEARS; i++)
    if (GET_EQ(ch, i) && GET_OBJ_TYPE(GET_EQ(ch, i)) == ITEM_BOAT)
      return 1;

  return 0;
}

void execute_trigger(struct affected_type *aff, struct char_data *ch)
{
  struct char_data *mob;
  int spell;
  
  if (!aff || !ch || !aff->type)
    return;
  spell = aff->type;
  
  if (!spell_list[spell])
    return;
  if (!(mob = read_mobile(3, VIRTUAL))) 
    return;
  char_to_room(mob, ch->in_room);
  mob->points.mana = 14000;

  spell_list[spell](mob, ch, NULL, NULL, NULL, 0, 0, 0,1,1.0);
  affect_from_room(&world[mob->in_room], RAFF_TRAP);
  extract_char(mob, TRUE);
}

void check_room_affects(struct char_data *ch)
{
  char *ch_mss[] = { "neutral","menacing","golden"}, tpbuf[200];
  struct char_data *weaver = NULL;
  struct affected_type *raf = NULL,*hlp;
  
  if (room_affected_by_spell(&world[ch->in_room], SPELL_SANCTUARY)) 
    send_to_char("Oooh, this room sure is calm. Maybe you should rest here for a while.\r\n", ch);

  if (ROOM_AFFECTED(ch->in_room, RAFF_DOMEOFFIRE) && (GET_LEVEL(ch) < LVL_IMMORT)) { /* Burn! */
    send_to_char("Suddenly you are immersed in scorching flames as the ward protecting this room reacts!\r\n", ch);
    act("As $n enters the room, $e is immersed in flames!", TRUE, ch, 0, 0, TO_ROOM);
    GET_HIT(ch) -= 500;
    update_pos(ch);
    if (GET_POS(ch) == POS_DEAD) {
      if (IS_NPC(ch)) {
        death_cry(ch);
        make_corpse(ch);
        extract_char(ch, FALSE);
        return;
      }
      else {
        die(ch, NULL); 
        return;
      }
    }
  }
  if (ROOM_AFFECTED(ch->in_room,RAFF_WARDED_HUMAN) && (!IS_NPC(ch) || MOB_FLAGGED(ch,MOB_HUMAN))) {
    for (hlp = world[ch->in_room].aff; hlp; hlp = hlp->next) {
     if (SPELL_WARDHUMAN == hlp->type) {
       raf = hlp;
       break; 
     }
    }
    if (NULL != raf) {
      weaver = get_char_id(raf->weaver);
      if ((NULL != weaver) && (weaver != ch)) {
        send_to_char("Something triggers your ward!",weaver);
      }
    }
  }

  if (ROOM_AFFECTED(ch->in_room, RAFF_DOMEOFFROST) && (GET_LEVEL(ch) < LVL_IMMORT)) { /* Freeze! */
    send_to_char("Suddenly you are immersed in extreme cold  as the ward protecting this room reacts!\r\n", ch);
    act("As $n enters the room, $e is almost frozen solid!", TRUE, ch, 0, 0, TO_ROOM);
    GET_HIT(ch) -= 500;
    update_pos(ch);
    if (GET_POS(ch) == POS_DEAD) {
      if (IS_NPC(ch)) {
        death_cry(ch);
        make_corpse(ch);
        extract_char(ch, FALSE);
        return;
      }
      else {
        die(ch, NULL); 
        return;
      }
    }
  }
  if (ROOM_AFFECTED(ch->in_room, RAFF_FOG))
     send_to_char("This place is filled with a fog as thick as pea soup!\r\n", ch);
  if (ROOM_AFFECTED(ch->in_room, RAFF_COLD))
    send_to_char("Light, this place is cold! You shiver already after three seconds!\r\n", ch);

  if (ROOM_FLAGGED(ch->in_room,ROOM_NOMAGIC) && IS_SET(PRF_FLAGS(ch),PRF_GRASPING)) {
    send_to_char("Suddenly you lose your grip on the One Power!\r\n",ch);
    sprintf(tpbuf,"The %s aura around $n suddenly vanishes!",ch_mss[(int) GET_SEX(ch)]);
    act(tpbuf,FALSE,ch,NULL,NULL,TO_OPUSERSS);
    remove_char_maintained(ch);
    remove_room_maintained(ch);
    REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
  }
  
  if (ROOM_AFFECTED(ch->in_room, RAFF_TRAP) && (GET_LEVEL(ch) < LVL_IMMORT)) { 
    for (hlp = world[ch->in_room].aff; hlp; hlp = hlp->next) {
      if (hlp->bitvector != RAFF_TRAP)
        continue;
      if (!hlp->location) /* Trigger is a timer, don't check here */
        continue;
      if ((hlp->location == 1)  && (!IS_NPC(ch) || MOB_FLAGGED(ch, MOB_HUMAN)))
        raf = hlp;
      else if ((hlp->location == 2) &&
        ((!IS_NPC(ch) && !cant_channel(ch)) || MOB_FLAGGED(ch, MOB_CHANNELER)))
        raf = hlp;
      else if ((hlp->location == 3) &&
        ((IS_NPC(ch) && MOB_FLAGGED(ch, MOB_SHADOWSPAWN)) || (!IS_NPC(ch) && (IS_GHOLAM(ch) || IS_GREYMAN(ch)))))
        raf = hlp;
    }
   if (raf) { /* ch has triggered the trap! Zap! */
      execute_trigger(raf, ch);
      raf = NULL; /* For checks further down */
    }
  }
}


void lookahead(struct char_data *ch, int dir)
{
  room_num start, was_in;
  start = ch->in_room;
  was_in = ch->in_room;
  if (IS_AFFECTED(ch, AFF_BLIND) || IS_SET(GET_PERMANENT(ch), PRM_BLIND)) 
    return;
  
    
  if (CAN_GO(ch, dir)) {
    sprintf(buf, "You look ahead %s and see the following:\r\n", dirs[dir]);
    send_to_char(buf, ch);
    char_from_room(ch);
    char_to_room(ch, world[start].dir_option[dir]->to_room);
    send_to_char("------------------ close by ----------------------\r\n", ch);
    list_char_to_char(world[ch->in_room].people, ch);
    if (CAN_GO(ch, dir)) {
      send_to_char("---------------- further away --------------------\r\n", ch);
      was_in = ch->in_room;
      char_from_room(ch);
      char_to_room(ch, world[was_in].dir_option[dir]->to_room);
      list_char_to_char(world[ch->in_room].people, ch);
    }
  }
  char_from_room(ch);
  char_to_room(ch, start);
}

void inform_commander(struct affected_type *af,struct char_data *ch,int enter)
{
  struct affected_type *hlp,*raf = NULL;
  struct char_data *weaver = NULL;

  for (hlp = af; hlp; hlp = hlp->next) {
    if (SKILL_FORTIFY == hlp->type) {
      raf = hlp;
      break;
    }
  }
  if (NULL != raf) {
    weaver = get_char_id(raf->weaver);
    if (NULL != weaver) {
      asend_to_char(weaver,"%s tries to %s your camp.\r\n",GET_NAME(ch),((enter) ? "enter" : "leave" ));
    }
  }
}

int enter_fortified_ok(struct char_data *ch)
{
  if (IS_IMMORTAL(ch) || (get_guild(ch,1)))
    return 1;
  if (!IS_AFFECTED(ch,AFF_SNEAK)) 
    return 0;
  if (IS_AFFECTED(ch,AFF_SNEAK)) {
    if (GET_SKILL(ch,SKILL_SNEAK) < 100)
      return 0;
    else {
      if (number(0,100) < number(40,60))
        return 0;
    }
  }
  return 1;
}

/* do_simple_move assumes
 *    1. That there is no master and no followers.
 *    2. That the direction exists.
 *
 *   Returns :
 *   1 : If succes.
 *   0 : If fail
 */
int do_simple_move(struct char_data *ch, int dir, int need_specials_check)
{
  int was_in = -1, need_movement, mprog = 0;
  struct char_data *temp = NULL;
  int danger = 0, oprog = 0, result = 0;

  if (GET_POS(ch) < POS_FIGHTING) {
    send_to_char("You can't do that!\r\n", ch);
    return 0;
  }
   
  if (IS_AFFECTED(ch,AFF_WRAPPED)) {
    send_to_char("You cant! You're wrapped!\r\n",ch);
    return 0;
  }
  if (IS_AFFECTED2(ch,AFF2_DARKDREAM)) {
    send_to_char("Hey! You can't move around in T'A'R like that!\r\n",ch);
    return 0;
  }
  if (ROOM_AFFECTED(ch->in_room, RAFF_ROOMSEAL)) {
    if (!cant_channel(ch))
      send_to_char("Someone has made a protective shield of Air around this room.\r\n",  ch);
    else
      send_to_char("Something prevents you from leaving this room!\r\n", ch);
    return 0;
  }
  if (ROOM_AFFECTED(EXIT(ch, dir)->to_room, RAFF_DOMEOFAIR) ||
  ROOM_AFFECTED(EXIT(ch, dir)->to_room, RAFF_ROOMSEAL)) {
    if (!cant_channel(ch))
      send_to_char("Someone has made a protective shield of Air around that room.\r\n",  ch);
    else
      send_to_char("Something prevents you from entering this room!\r\n", ch);
    return 0;
  }
  
  if (ROOM_FLAGGED(EXIT(ch, dir)->to_room, ROOM_PRIVATE) && (GET_REAL_LEVEL(ch) < LVL_GRGOD)
      && (number_of_players_in_room(EXIT(ch, dir)->to_room) > 1)) {
    send_to_char("That room is private right now.\r\n", ch);
    return 0;
  }

  if (ROOM_AFFECTED(EXIT(ch,dir)->to_room,RAFF_FORTIFY)) {
    if (!enter_fortified_ok(ch)) {
      send_to_char("You can't enter a fortified area like that!\r\n",ch);
      inform_commander(world[EXIT(ch,dir)->to_room].aff,ch,TRUE);
      return 0;
    }
  }
  if (ROOM_AFFECTED(ch->in_room,RAFF_FORTIFY)) {
    if (!enter_fortified_ok(ch)) {
      send_to_char("You can't leave a fortified area like that!\r\n",ch);
      inform_commander(world[ch->in_room].aff,ch,FALSE);
      return 0;
    }
  } 
  if (ROOM_FLAGGED(EXIT(ch, dir)->to_room, ROOM_GUILD) && (!IS_NPC(ch)) &&
     (GET_LEVEL(ch) < LVL_IMMORT)){
    if (!get_guild_by_room(ch, EXIT(ch, dir)->to_room)){
      send_to_char("You're not allowed to go there...Guild area.\r\n", ch);
      return 0;
  }
}
  /*
   * Check for special routines (North is 1 in command list, but 0 here) Note
   * -- only check if following; this avoids 'double spec-proc' bug
   */
  if (need_specials_check && special(ch, dir + 1, ""))
    return 0;

  /* charmed? */
  if (IS_AFFECTED(ch, AFF_CHARM) && ch->master && ch->in_room == ch->master->in_room) {
    send_to_char("The thought of leaving your master makes you weep.\r\n", ch);
//    act("$n bursts into tears.", FALSE, ch, 0, 0, TO_ROOM);
    return 0;
  }

  /* if this room or the one we're going to needs a boat, check for one */
  if ((SECT(ch->in_room) == SECT_WATER_NOSWIM) ||
      (SECT(EXIT(ch, dir)->to_room) == SECT_WATER_NOSWIM)) {
    if (!has_boat(ch)) {
      send_to_char("You need a boat to go there.\r\n", ch);
      return 0;
    }
  }

  /* move points needed is avg. move loss for src and destination sect type */
  need_movement = (movement_loss[SECT(ch->in_room)] +
           movement_loss[SECT(EXIT(ch, dir)->to_room)]) >> 1;
     
  if (weather_info.sky == SKY_RAINING)
    need_movement ++;                 /* Bad weather makes travel more difficult */
  if (IS_SET(GET_PERMANENT(ch), PRM_CRIPPLED)) /* Crippled people have more difficult moving */
    need_movement *= 2;
  if (IS_AFFECTED(ch, AFF_RUNNING))
    need_movement = (int) (need_movement/3);
  if (get_guild(ch,9))
  {
    result = generic_result_mod(ch,SKILL_ENDURE,NULL,FALSE,(GET_CON(ch)-30));
    if ((result != TOTAL_FAILURE) && (result != TOTAL_FUCKUP))
      need_movement = (int) (int) (need_movement/2);
  }
  if (GET_MOVE(ch) < need_movement && !IS_NPC(ch))  {
    if (need_specials_check && ch->master)
      send_to_char("You are too exhausted to follow.\r\n", ch);
    else
      send_to_char("You are too exhausted.\r\n", ch);

    return 0;
  }

  if ((ROOM_FLAGGED(EXIT(ch, dir)->to_room, ROOM_TUNNEL)) &&
      num_pc_in_room(&(world[EXIT(ch, dir)->to_room])) > 1) {
    send_to_char("There isn't enough room there for more than one person!\r\n", ch);
    return 0;
  }

  if (!leave_wtrigger(&world[ch->in_room], ch))
    return 0;

  /* Player can actually move into the room */
  if (!IS_NPC(ch)) {
    mprog = check_for_mobprogs_and_stops(ch, NULL, NULL, MPC_LEAVE, 0, 0);
    oprog = check_for_objprogs_and_stops(ch, NULL, NULL, world[ch->in_room].contents, OPC_LEAVE, 0);
  }
  else
    mprog = check_for_mobprogs_and_stops(ch, NULL, NULL, MPC_MOBLEAVE, 0, 0);
  if ((mprog != 2 ) && (oprog != 2))
  { 
    /* If programmed mobs and objs lets the player leave the room */
    if (GET_LEVEL(ch) < LVL_IMMORT && !IS_NPC(ch))
    {
      if (result == TOTAL_FAILURE)
      {
        GET_MOVE(ch) -= need_movement;
      } else {
        if (need_movement == 1)
        {
          if (GET_FRACT_MOVE(ch) == 1)
          {
            GET_FRACT_MOVE(ch) = 0;
          } else {
            GET_FRACT_MOVE(ch) = 1;
            GET_MOVE(ch) -= 1;
          }
        } else {
          GET_MOVE(ch) -= (int) (need_movement/2);
          GET_FRACT_MOVE(ch) = need_movement % 2;
        }
      }
    }
    if (!IS_AFFECTED(ch, AFF_SNEAK) && !IS_GREYMAN(ch)) 
    {
      if (IS_AFFECTED(ch, AFF_RUNNING))
        sprintf(buf2, "$n runs through, heading %s%s.", dirs[dir], (ch->pulling)? ", pulling a wagon": "");
      else if (GET_COND(ch, DRUNK) > 12)
        sprintf(buf2, "$n staggers drunkenly %s%s.", dirs[dir], (ch->pulling)? ", pulling a wagon": "");
      else
        sprintf(buf2, "$n %s %s%s.", (!IS_NPC(ch) && IS_SET(GET_PERMANENT(ch), PRM_CRIPPLED)) ? "hobbles" : "leaves", 
        dirs[dir], (ch->pulling)? ", pulling a wagon": "");
      act(buf2, TRUE, ch, 0, 0, TO_ROOM);
    }
    if (IS_AFFECTED(ch, AFF_CAMOUFLAGED))          /*If the disguised one moves out from */
      TOGGLE_BIT(AFF_FLAGS(ch), AFF_CAMOUFLAGED);  /* the room, the camouflage ends      */    
    if (mprog)
      run_mobprog(ch, 0);
    if (oprog)
      run_objprog(ch, world[ch->in_room].contents);

    /* see if a leave trigger disallows the move */
//    if (!leave_mtrigger(ch))
//      return 0;

    if (!farewell_mtrigger(ch, dir)) {
      char_from_room(ch);
      look_at_room(ch, 0);
      return 1;
    }

    /* see if an entry trigger disallows the move */
    if (!entry_mtrigger(ch))
     return 0;
    if (!enter_wtrigger(&world[EXIT(ch, dir)->to_room], ch, dir))
     return 0;

    was_in = ch->in_room;
    char_from_room(ch);
    char_to_room(ch, world[was_in].dir_option[dir]->to_room);
    if (ch->pulling) 
    {
      obj_from_room(ch->pulling);
      obj_to_room(ch->pulling, ch->in_room);
    }
    if (!IS_AFFECTED(ch, AFF_SNEAK) && !IS_GREYMAN(ch) && !IS_AFFECTED(ch, AFF_RUNNING)) 
    {
      if (ch->pulling)
        act("$n has arrived, pulling $p.", TRUE, ch, ch->pulling, 0, TO_ROOM);
      else
        act("$n has arrived.", TRUE, ch, 0, 0, TO_ROOM);
    }
    if (IS_NPC(ch))
      mprog = check_for_mobprogs_and_stops(ch, NULL, NULL, MPC_MOBENTRY, 0, 0); 
    else 
    {
      mprog = check_for_mobprogs_and_stops(ch, NULL, NULL, MPC_ENTRY, 0, 0); 
      oprog = check_for_objprogs_and_stops(ch, NULL, NULL, world[ch->in_room].contents, OPC_ENTRY, 0);
    }
    if ((ch->desc != NULL) || is_retainer(ch))
      look_at_room(ch, 0);
    
    TICKS_IN_ROOM(ch)=0; // Player moved; he's been in the new room for 0 ticks.

    if (IS_SET(PRF_FLAGS(ch),PRF_LOOKAHEAD))
      lookahead(ch, dir);
      
    if (mprog) 
      run_mobprog(ch, 0);
    if (oprog)
      run_objprog(ch, world[ch->in_room].contents);

    if (GET_WOLFKIN(ch) || (GET_BONDED(ch) && !(GET_BONDED(ch) & (1 << 30)))) {
      for (temp = world[ch->in_room].people; NULL != temp; temp = temp->next_in_room) {
        if (ch == temp)
          continue;
        if (IS_NPC(temp) && MOB_FLAGGED(temp,MOB_SHADOWSPAWN)) {
          danger = TRUE;
        }
        if (!IS_NPC(temp) && (IS_GHOLAM(temp) || IS_GREYMAN(temp)) && GET_LEVEL(temp)<LVL_CARPENTER) {
          danger = TRUE;
        }
      }
      if (danger) {
        danger = FALSE;
        send_to_char("\\c01Something feels very wrong here...\r\n\\c00", ch);
      }
    }
 
    if (IS_GHOLAM(ch) || IS_GREYMAN(ch) || MOB_FLAGGED(ch,MOB_SHADOWSPAWN)) 
      for (temp = world[ch->in_room].people; temp; temp = temp->next_in_room)
       if (!IS_NPC(temp) && (GET_WOLFKIN(temp) || (GET_BONDED(temp) && !(GET_BONDED(temp) & (1 << 30)))) && (temp != ch)) {
          send_to_char("\\c01Something suddenly feels very wrong here...\r\n\\c00", temp);
         }

    if (!IS_NPC(ch) && (GET_WOLFKIN(ch) || (GET_BONDED(ch) && !(GET_BONDED(ch) & (1 << 30))))) 
       for (temp = world[ch->in_room].people; temp; temp = temp->next_in_room)
        if (!IS_NPC(temp) && (IS_GHOLAM(temp) || IS_GREYMAN(temp) || MOB_FLAGGED(ch,MOB_SHADOWSPAWN
)) && (temp != ch))
          danger = TRUE;
    
    if (danger) {
     send_to_char("\\c01Something feels very wrong here...\r\n\\c00", ch);
    }
 
    if (!IS_NPC(ch) && IS_AFFECTED(ch, AFF_DREAMING))
      if (number(0, 100) > GET_SKILL(ch, SKILL_DREAM))
        send_to_char("\\c10The room seems to waver and then solidify as you enter.\\c00\r\n", ch);
  } 
  else { /* Player prevented from leaving room by obj or mob */
    if (oprog)
      run_objprog(ch, world[ch->in_room].contents);
    if (mprog)  
      run_mobprog(ch, 0); 
  }

  check_room_affects(ch);

  entry_memory_mtrigger(ch);
  if (!greet_mtrigger(ch, dir, was_in) || !greet_otrigger(ch, dir)) {
    char_from_room(ch);
    look_at_room(ch, 0);
    return 1;
  } else greet_memory_mtrigger(ch);

  return 1;
}

static int cmdnumber_of_movement(const char *dirname) 
{
  register int i;

  for (i = 0; i < (2*NUM_OF_DIRS); i++) {
    if (!strcasecmp(dirname,complete_cmd_info[i].command))
      return i;
  }
  return -1;
}

int perform_move(struct char_data *ch, int dir, int need_specials_check)
{
  int was_in, newdir = dir;
  struct follow_type *k, *next;

  if (ch == NULL || dir < 0 || dir >= NUM_OF_DIRS)
    return 0;

  if (SESS_FLAGGED(ch, SESS_EAVESDROP)) {
    send_to_char("You move away from where you were and can no longer hear soft voices.\r\n", ch);
    (void)SESS_TOG_CHK(ch, SESS_EAVESDROP);
    rem_eavesdropper(ch, GET_EAVESDROPROOM(ch));
    GET_EAVESDROPROOM(ch) = 0;
  }

  if ((GET_LEVEL(ch) < LVL_IMMORT) && (GET_COND(ch, DRUNK) > 15)) {  /* Drunk bozo misses exit */
    while (newdir == dir)                 /* Don't want him to miss the door and find it again */
      newdir = number(0, NUM_OF_DIRS - 1);
    sprintf(buf, "$n was about to go %s, but is so drunk $e tries to go %s instead.\r\n", 
      dirs[dir], dirs[newdir]);
    act(buf, TRUE, ch, 0, 0, TO_ROOM);
    sprintf(buf, "You try to go %s, but are so drunk that you try to go %s instead!\r\n", 
      dirs[dir], dirs[newdir]);
    send_to_char(buf, ch);
    
    axlog(SYS_DEBUG,MAX(LVL_GOD, GET_INVIS_LEV(ch)) ,FALSE,"%s is drunk and moves %s instead of %s",GET_NAME(ch),dirs[dir],dirs[newdir]);
    dir = newdir;
    /*
     * think we need to check command_triggers here, and perhaps specials?, to see if 
     * the new direction staggered in will trigger anything. 
     */
     if (GET_LEVEL(ch)<LVL_IMMORT) {
       int cont = 0; /* continue the command checks */
       int mprog = 0,oprog = 0, spec = 0, cmd = -1;
 
       cont = command_wtrigger(ch, (char *)dirs[dir], (char *)dirs[dir]);
       if (!cont) cont = command_mtrigger(ch, (char *)dirs[dir], (char *)dirs[dir]);
       if (!cont) cont = command_otrigger(ch, (char *)dirs[dir], (char *)dirs[dir]);
       if (cont) return 0; /* command trigger took over */


       cmd = cmdnumber_of_movement(dirs[dir]);
       if (-1 == cmd) {
         send_to_char("Something horrible has happened, and your movement have been canceled. Please inform a Weaver ASAP.\r\n",ch);
         return 0;
       }
/* need to check [m|o]progs too! */
       mprog = check_for_mobprogs_and_stops(ch, NULL, NULL, MPC_ACTION, cmd, 0);
       
       oprog = 0;

       if ((mprog != 2) && (oprog != 2))
	 spec = special(ch, cmd, (char *)dirs[dir]);

       if (spec)
         return 0;

       if (mprog)
         run_mobprog(ch,0);

       if (mprog == 2)
         return 0;
     }
  } 
  if (!EXIT(ch, dir) || EXIT(ch, dir)->to_room == NOWHERE)
    send_to_char("Alas, you cannot go that way...\r\n", ch);
  else if (zone_table[world[EXIT(ch, dir)->to_room].zone].lock && (GET_LEVEL(ch) < LVL_IMMORT)) 
    send_to_char("That zone is locked, for now.\r\n", ch);
  else if (IS_SET(EXIT(ch, dir)->exit_info, EX_CLOSED) && !IS_GHOLAM(ch)) {
    if (EXIT(ch, dir)->keyword) {
      sprintf(buf2, "The %s seems to be closed.\r\n", fname(EXIT(ch, dir)->keyword));
      send_to_char(buf2, ch);
    } else 
        send_to_char("It seems to be closed.\r\n",ch);
  }
  else if (IS_GHOLAM(ch) && IS_SET(EXIT(ch,dir)->exit_info, EX_CLOSED) &&GET_SEX(ch)==SEX_MALE) {
    send_to_char("You squeeze yourself past whatever is blocking your way!\r\n",ch);
    act("$n squeezes himself flat and moves on.",FALSE,ch,0,0,TO_ROOM);
    return (do_simple_move(ch,dir,need_specials_check));
  }
  else if (IS_GHOLAM(ch) && IS_SET(EXIT(ch,dir)->exit_info, EX_CLOSED) &&GET_SEX(ch)==SEX_FEMALE) {
    send_to_char("You squeeze yourself past whatever is blocking your way!\r\n",ch);
    act("$n squeezes herself flat and moves on.",FALSE,ch,0,0,TO_ROOM);
    return (do_simple_move(ch,dir,need_specials_check));
  } 
 else {
      if (!ch->followers && !ch->tailed)
        return (do_simple_move(ch, dir, need_specials_check));

      was_in = ch->in_room;
      if (!do_simple_move(ch, dir, need_specials_check))
        return 0;
      if (ch->followers) {
        for (k = ch->followers; k; k = next) {
          next = k->next;
          if ((k->follower->in_room == was_in) &&
        (GET_POS(k->follower) >= POS_STANDING)) {
        act("You follow $N.\r\n", FALSE, k->follower, 0, ch, TO_CHAR);
        perform_move(k->follower, dir, 1);
          }
        }
      }
      else {
        for (k=ch->tailed; ch != NULL; k=k->next)
          if ((k->follower->in_room == was_in) && (GET_POS(k->follower) >= POS_STANDING)) {
            act("You tail $N.\r\n",FALSE,k->follower,0,ch,TO_CHAR);
            perform_move(k->follower,dir,1);
          }
      }
      return 1;
  }
  return 0;
}


ACMD(do_move)
{
  /*
   * This is basically a mapping of cmd numbers to perform_move indices.
   * It cannot be done in perform_move because perform_move is called
   * by other functions which do not require the remapping.
   */
  
  if (ch->player_specials->linked_by)
    send_to_char("You cannot leave until you are released from the link!\r\n", ch);
  else
    perform_move(ch, subcmd - 1, 0);
}


int find_door(struct char_data *ch, char *type, char *dir, char *cmdname)
{
  int door;

  if (*dir) {			/* a direction was specified */
    if ((door = search_block(dir, dirs, FALSE)) == -1) {	/* Partial Match */
      send_to_char("That's not a direction.\r\n", ch);
      return -1;
    }
    if (EXIT(ch, door))
      if (EXIT(ch, door)->keyword)
    if (isname(type, EXIT(ch, door)->keyword))
      return door;
    else
      if (!str_cmp(type, "door"))
        return door;
    else { 
      sprintf(buf2, "I see no %s there.\r\n", type);
      send_to_char(buf2, ch);
      return -1;
      } else
    return door;
    else {
      sprintf(buf2, "I really don't see how you can %s anything there.\r\n", cmdname);
      send_to_char(buf2, ch);
      return -1;
    }
  } else {			/* try to locate the keyword */
    if (!*type) {
      sprintf(buf2, "What is it you want to %s?\r\n", cmdname);
      send_to_char(buf2, ch);
      return -1;
    }
    for (door = 0; door < NUM_OF_DIRS; door++)
      if (EXIT(ch, door))
    if (EXIT(ch, door)->keyword) {
      if (isname(type, EXIT(ch, door)->keyword))
        return door;
          else
        if (!str_cmp(type, "door"))
          return door;
    }

    sprintf(buf2, "There doesn't seem to be %s %s here.\r\n", AN(type), type);
    send_to_char(buf2, ch);
    return -1;
  }
}


int has_key(struct char_data *ch, int key)
{
  struct obj_data *o;

  for (o = ch->carrying; o; o = o->next_content)
    if (GET_OBJ_VNUM(o) == key)
      return 1;

  if (GET_EQ(ch, WEAR_HOLD))
    if (GET_OBJ_VNUM(GET_EQ(ch, WEAR_HOLD)) == key)
      return 1;

  return 0;
}



#define NEED_OPEN	1
#define NEED_CLOSED	2
#define NEED_UNLOCKED	4
#define NEED_LOCKED	8

char *cmd_door[] =
{
  "open",
  "close",
  "unlock",
  "lock",
  "pick"
};

const int flags_door[] =
{
  NEED_CLOSED | NEED_UNLOCKED,
  NEED_OPEN,
  NEED_CLOSED | NEED_LOCKED,
  NEED_CLOSED | NEED_UNLOCKED,
  NEED_CLOSED | NEED_LOCKED
};


#define EXITN(room, door)		(world[room].dir_option[door])
#define OPEN_DOOR(room, obj, door)	((obj) ?\
        (TOGGLE_BIT(GET_OBJ_VAL(obj, 1), CONT_CLOSED)) :\
        (TOGGLE_BIT(EXITN(room, door)->exit_info, EX_CLOSED)))
#define LOCK_DOOR(room, obj, door)	((obj) ?\
        (TOGGLE_BIT(GET_OBJ_VAL(obj, 1), CONT_LOCKED)) :\
        (TOGGLE_BIT(EXITN(room, door)->exit_info, EX_LOCKED)))

void do_doorcmd(struct char_data *ch, struct obj_data *obj, int door, int scmd)
{
  int other_room = 0;
  struct room_direction_data *back = 0;

  sprintf(buf, "$n %ss ", cmd_door[scmd]);
  if (!obj && ((other_room = EXIT(ch, door)->to_room) != NOWHERE))
    if ((back = world[other_room].dir_option[rev_dir[door]]))
      if (back->to_room != ch->in_room)
    back = 0;

  switch (scmd) {
  case SCMD_OPEN:
  case SCMD_CLOSE:
    OPEN_DOOR(ch->in_room, obj, door);
    if (back)
      OPEN_DOOR(other_room, obj, rev_dir[door]);
    send_to_char(OK, ch);
    break;
  case SCMD_UNLOCK:
  case SCMD_LOCK:
    LOCK_DOOR(ch->in_room, obj, door);
    if (back)
      LOCK_DOOR(other_room, obj, rev_dir[door]);
    send_to_char("*Click*\r\n", ch);
    break;
  case SCMD_PICK:
    LOCK_DOOR(ch->in_room, obj, door);
    if (back)
      LOCK_DOOR(other_room, obj, rev_dir[door]);
    send_to_char("The lock quickly yields to your skills.\r\n", ch);
    strcpy(buf, "$n skillfully picks the lock on ");
    break;
  }

  /* Notify the room */
  sprintf(buf + strlen(buf), "%s%s.", ((obj) ? "" : "the "), (obj) ? "$p" :
      (EXIT(ch, door)->keyword ? "$F" : "door"));
  if (!(obj) || (obj->in_room != NOWHERE)) {
    if (IS_AFFECTED(ch,AFF_DREAMING)) {
      act(buf, FALSE, ch, obj, obj ? 0 : EXIT(ch, door)->keyword, TO_DREAMERS);
    }
    else { 
      act(buf, FALSE, ch, obj, obj ? 0 : EXIT(ch, door)->keyword, TO_ROOM);
    }
  }
  /* Notify the other room */
  if ((scmd == SCMD_OPEN || scmd == SCMD_CLOSE) && back) {
    sprintf(buf, "The %s is %s%s from the other side.\r\n",
     (back->keyword ? fname(back->keyword) : "door"), cmd_door[scmd],
        (scmd == SCMD_CLOSE) ? "d" : "ed");
    if (world[EXIT(ch, door)->to_room].people) {
      if (IS_AFFECTED(ch,AFF_DREAMING))
        act(buf, FALSE, world[EXIT(ch, door)->to_room].people, 0, 0, TO_DREAMERS);
      else
        act(buf, FALSE, world[EXIT(ch, door)->to_room].people, 0, 0, TO_ROOM);
      act(buf, FALSE, world[EXIT(ch, door)->to_room].people, 0, 0, TO_CHAR);
    }
  }
}


int ok_pick(struct char_data *ch, int keynum, int pickproof, int scmd)
{
  int percent;

  percent = number(1, 101);

  if (scmd == SCMD_PICK) 
  {

    if (keynum < 0)
      send_to_char("Odd - you can't seem to find a keyhole.\r\n", ch);
    else if (pickproof)
      send_to_char("It resists your attempts to pick it.\r\n", ch);
    else if (percent > GET_SKILL(ch, SKILL_PICK_LOCK))
      send_to_char("You failed to pick the lock.\r\n", ch);
    else if ((number(0,100)<3) && (GET_SKILL(ch,SKILL_PICK_LOCK) < 100))
      {
        send_to_char("You have improved at picking locks!\r\n",ch);
        GET_SKILL(ch, SKILL_PICK_LOCK)++;
      }
    else      
      return (1);
    return (0);
  }
  return (1);
}


#define DOOR_IS_OPENABLE(ch, obj, door)	((obj) ? \
            (((GET_OBJ_TYPE(obj) == ITEM_CONTAINER) || (GET_OBJ_TYPE(obj) == ITEM_WAGON))&& \
             (IS_SET(GET_OBJ_VAL(obj, 1), CONT_CLOSEABLE))) :\
             IS_SET(EXIT(ch, door)->exit_info, EX_ISDOOR))
#define DOOR_IS_OPEN(ch, obj, door)	((obj) ? \
            (!IS_SET(GET_OBJ_VAL(obj, 1), CONT_CLOSED)) :\
            (!IS_SET(EXIT(ch, door)->exit_info, EX_CLOSED)))
#define DOOR_IS_UNLOCKED(ch, obj, door)	((obj) ? \
            (!IS_SET(GET_OBJ_VAL(obj, 1), CONT_LOCKED)) :\
            (!IS_SET(EXIT(ch, door)->exit_info, EX_LOCKED)))
#define DOOR_IS_PICKPROOF(ch, obj, door) ((obj) ? \
            (IS_SET(GET_OBJ_VAL(obj, 1), CONT_PICKPROOF)) : \
            (IS_SET(EXIT(ch, door)->exit_info, EX_PICKPROOF)))

#define DOOR_IS_CLOSED(ch, obj, door)	(!(DOOR_IS_OPEN(ch, obj, door)))
#define DOOR_IS_LOCKED(ch, obj, door)	(!(DOOR_IS_UNLOCKED(ch, obj, door)))
#define DOOR_KEY(ch, obj, door)		((obj) ? (GET_OBJ_VAL(obj, 2)) : \
                    (EXIT(ch, door)->key))
#define DOOR_LOCK(ch, obj, door)	((obj) ? (GET_OBJ_VAL(obj, 1)) : \
                    (EXIT(ch, door)->exit_info))

ACMD(do_gen_door)
{
  int door = -1, keynum;
  char type[MAX_INPUT_LENGTH], dir[MAX_INPUT_LENGTH];
  struct obj_data *obj = NULL;
  struct char_data *victim = NULL;

  skip_spaces(&argument);
  if (!argument || !*argument) {
    sprintf(buf, "%s what?\r\n", cmd_door[subcmd]);
    send_to_char(CAP(buf), ch);
    return;
  }
  two_arguments(argument, type, dir);
  if (!generic_find(type, FIND_OBJ_INV | FIND_OBJ_ROOM, ch, &victim, &obj))
    door = find_door(ch, type, dir, cmd_door[subcmd]);

  if ((obj) || (door >= 0)) {
    keynum = DOOR_KEY(ch, obj, door);
    if (!(DOOR_IS_OPENABLE(ch, obj, door)))
      act("You can't $F that!", FALSE, ch, 0, cmd_door[subcmd], TO_CHAR);
    else if (!DOOR_IS_OPEN(ch, obj, door) &&
         IS_SET(flags_door[subcmd], NEED_OPEN))
      send_to_char("But it's already closed!\r\n", ch);
    else if (!DOOR_IS_CLOSED(ch, obj, door) && IS_SET(flags_door[subcmd], NEED_CLOSED) && has_key(ch,keynum)) {
      do_doorcmd(ch,obj,door,SCMD_CLOSE);
      do_doorcmd(ch,obj,door,SCMD_LOCK);
    }
    else if (!DOOR_IS_CLOSED(ch, obj, door) &&
         IS_SET(flags_door[subcmd], NEED_CLOSED))
      send_to_char("But it's currently open!\r\n", ch);
    else if (!(DOOR_IS_LOCKED(ch, obj, door)) &&
         IS_SET(flags_door[subcmd], NEED_LOCKED))
      send_to_char("Oh.. it wasn't locked, after all..\r\n", ch);
    else if (!(DOOR_IS_UNLOCKED(ch, obj, door)) && IS_SET(flags_door[subcmd], NEED_UNLOCKED) && has_key(ch,keynum)) {
       do_doorcmd(ch,obj,door,SCMD_UNLOCK);
       do_doorcmd(ch,obj,door,SCMD_OPEN);
    }      
    else if (!(DOOR_IS_UNLOCKED(ch, obj, door)) &&
         IS_SET(flags_door[subcmd], NEED_UNLOCKED))
      send_to_char("It seems to be locked.\r\n", ch);
    else if (!has_key(ch, keynum) && (GET_LEVEL(ch) < LVL_GOD) &&
         ((subcmd == SCMD_LOCK) || (subcmd == SCMD_UNLOCK)))
      send_to_char("You don't seem to have the proper key.\r\n", ch);
    else if (ok_pick(ch, keynum, DOOR_IS_PICKPROOF(ch, obj, door), subcmd))
      do_doorcmd(ch, obj, door, subcmd);
  }
  return;
}



ACMD(do_enter)
{
  struct obj_data *wagon = NULL;
  int door, room = NOWHERE, mprog = 0, oprog = 0, was_in = NOWHERE;
  struct room_data *rooms;
  struct follow_type *k, *next;
  
  if (SESS_FLAGGED(ch, SESS_EAVESDROP)) {
    send_to_char("You move away from where you were and can no longer hear soft voices.\r\n", ch);
    (void)SESS_TOG_CHK(ch, SESS_EAVESDROP);
    rem_eavesdropper(ch, GET_EAVESDROPROOM(ch));
    GET_EAVESDROPROOM(ch) = 0;
  }

  if (ch->player_specials->linking[0]){
    send_to_char("You must dissolve your link first!\r\n", ch);
    return;
  }

  if(IS_AFFECTED2(ch, AFF2_SUBDUED))
    {
    send_to_char("You can't enter that while being held captive!\n\r",ch);
    return;
    }

  if (ch->player_specials->linked_by) {
    send_to_char("You cannot leave until you are released from your link!\r\n", ch);
    return;
  }
  one_argument(argument, buf);
  
  if ((wagon = get_obj_in_list_vis(ch, buf, world[ch->in_room].contents))) 
    if (GET_OBJ_TYPE(wagon) == ITEM_WAGON) {
      if (ch->pulling) {
        send_to_char("You can't enter a wagon while pulling one!\r\n", ch);
        return;
      }
      if ((room = real_room(GET_OBJ_VNUM(wagon))) != NOWHERE) {
        rooms = &world[room];
        if (IS_SET(GET_OBJ_VAL(wagon, 1), CONT_CLOSED)) {
          act("$p is closed.", TRUE, ch, wagon, 0, TO_CHAR);
          return;
        }
        if (ROOM_FLAGGED(room, ROOM_WAGON)) {
      if (!IS_NPC(ch)) {
        mprog = check_for_mobprogs_and_stops(ch, NULL, NULL, MPC_LEAVE, 0, 0);
        oprog = check_for_objprogs_and_stops(ch, NULL, NULL, world[ch->in_room].contents, OPC_LEAVE, 0);
      }
      else
        mprog = check_for_mobprogs_and_stops(ch, NULL, NULL, MPC_MOBLEAVE, 0,0);
      if ((mprog != 2 ) && (oprog != 2)) {
            act("$n enters $p.", TRUE, ch, wagon, 0, TO_ROOM);
            act("Ok, you enter $p.", TRUE, ch, wagon, 0, TO_CHAR);
        if (mprog)
          run_mobprog(ch, 0);
        if (oprog)
          run_objprog(ch, world[ch->in_room].contents);
        was_in = IN_ROOM(ch);
            char_from_room(ch);
            char_to_room(ch, room);
            act("$n has entered $p.", TRUE, ch, wagon, 0, TO_ROOM);
            do_look(ch, "\0", 0, SCMD_LOOK);
            rooms->wagon = wagon;
        if (IS_NPC(ch))
          mprog = check_for_mobprogs_and_stops(ch, NULL, NULL, MPC_MOBENTRY, 0, 0);
        else
        {
          mprog = check_for_mobprogs_and_stops(ch, NULL, NULL, MPC_ENTRY, 0, 0);
          oprog = check_for_objprogs_and_stops(ch, NULL, NULL, world[ch->in_room].contents, OPC_ENTRY, 0);
        }
        if (mprog)
          run_mobprog(ch, 0);
        if (oprog)
          run_objprog(ch, world[ch->in_room].contents);

        k = ch->followers;
        while (k) {
          next = k->next;
          if (k->follower->in_room == was_in && GET_POS(k->follower) == POS_STANDING) {
        act("You follow $N.\r\n", FALSE, k->follower, 0, ch, TO_CHAR);
        do_enter(k->follower, argument, 0, 0);
          }
          k = next;
        }
        k = ch->tailed;
        while (k) {
          next = k->next;
          if (k->follower->in_room == was_in && GET_POS(k->follower) == POS_STANDING) {
        act("You follow $N.\r\n", FALSE, k->follower, 0, ch, TO_CHAR);
        do_enter(k->follower, argument, 0, 0);
          }
          k = next;
        }
      }
      else {
        if (oprog)
          run_objprog(ch, world[ch->in_room].contents);
        if (mprog)
          run_mobprog(ch, 0);
      }
          return;
        }
        else {
          send_to_char("The room connected to the wagon item isn't flagged as a room. Tell an imm.\r\n", ch);
          return;
        }
      }
      else {
        send_to_char("There is no room connected to this wagon. Tell an imm.\r\n", ch);
        return;
      }
    }
      
  if (*buf) {			/* an argument was supplied, search for door
                 * keyword */    
    for (door = 0; door < NUM_OF_DIRS; door++)
      if (EXIT(ch, door))
    if (EXIT(ch, door)->keyword)
      if (!str_cmp(EXIT(ch, door)->keyword, buf)) {
        perform_move(ch, door, 1);
        return;
      }
    sprintf(buf2, "There is no %s here.\r\n", buf);
    send_to_char(buf2, ch);
  } else if (ROOM_FLAGGED(ch->in_room, ROOM_INDOORS))
    send_to_char("You are already indoors.\r\n", ch);
  else {
    /* try to locate an entrance */
    for (door = 0; door < NUM_OF_DIRS; door++)
      if (EXIT(ch, door))
    if (EXIT(ch, door)->to_room != NOWHERE)
      if (!IS_SET(EXIT(ch, door)->exit_info, EX_CLOSED) &&
          ROOM_FLAGGED(EXIT(ch, door)->to_room, ROOM_INDOORS)) {
        perform_move(ch, door, 1);
        return;
      }
    send_to_char("You can't seem to find anything to enter.\r\n", ch);
  }
}


ACMD(do_leave)
{
  int door, mprog = 0, oprog = 0, was_in = NOWHERE;
    char *ch_mss[]={"neutral","menacing","golden"}, tpbuf[200];
  struct follow_type *k, *next;

  struct obj_data *wagon;

  if (SESS_FLAGGED(ch, SESS_EAVESDROP)) {
    send_to_char("You move away from where you were and can no longer hear soft voices.\r\n", ch);
    (void)SESS_TOG_CHK(ch, SESS_EAVESDROP);
    rem_eavesdropper(ch, GET_EAVESDROPROOM(ch));
    GET_EAVESDROPROOM(ch) = 0;
  }

  if (ROOM_FLAGGED(ch->in_room, ROOM_WAGON)) {
    if (world[ch->in_room].wagon)
      wagon = world[ch->in_room].wagon;
    else {
      send_to_char("There is no wagon item connected to this room. Tell an imm.\r\n", ch);
      return;
    }
    if ((GET_OBJ_TYPE(wagon) == ITEM_WAGON) && (wagon->in_room != NOWHERE)) {
      if (!IS_NPC(ch)) {
        mprog = check_for_mobprogs_and_stops(ch, NULL, NULL, MPC_LEAVE, 0, 0);
        oprog = check_for_objprogs_and_stops(ch, NULL, NULL, world[ch->in_room].contents, OPC_LEAVE, 0);
      }
      else
        mprog = check_for_mobprogs_and_stops(ch, NULL, NULL, MPC_MOBLEAVE, 0,0);
      if ((mprog != 2 ) && (oprog != 2)) {
        send_to_char("You leave the wagon.\r\n", ch);
        act("$n leaves the wagon.\r\n", TRUE, ch, 0, 0, TO_ROOM);
        if (mprog)
          run_mobprog(ch, 0);
        if (oprog)
          run_objprog(ch, world[ch->in_room].contents);
    was_in = IN_ROOM(ch);
        char_from_room(ch);
        char_to_room(ch, wagon->in_room);
        act("$n comes out of $p.", TRUE, ch, wagon, 0, TO_ROOM);
        if(ROOM_FLAGGED(ch->in_room,ROOM_NOMAGIC) && IS_SET(PRF_FLAGS(ch),PRF_GRASPING))
        {
          send_to_char("Suddenly you lose your grip on the One Power!\r\n",ch);
          sprintf(tpbuf,"The %s aura around $n suddenly vanishes!",ch_mss[(int)GET_SEX(ch)]);
          act(tpbuf,FALSE,ch,NULL,NULL,TO_OPUSERSS);
          remove_char_maintained(ch);
          remove_room_maintained(ch);
          REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
        }
        do_look(ch, "\0", 0, SCMD_LOOK);
        if (IS_NPC(ch))
          mprog = check_for_mobprogs_and_stops(ch, NULL, NULL, MPC_MOBENTRY, 0, 0);
        else
        {
          mprog = check_for_mobprogs_and_stops(ch, NULL, NULL, MPC_ENTRY, 0, 0);
          oprog = check_for_objprogs_and_stops(ch, NULL, NULL, world[ch->in_room].contents, OPC_ENTRY, 0);
        }
        if (mprog)
          run_mobprog(ch, 0);
        if (oprog)
          run_objprog(ch, world[ch->in_room].contents);
    k = ch->followers;
    while (k) {
      next = k->next;
      if (k->follower->in_room == was_in && GET_POS(k->follower) == POS_STANDING) {
        act("You follow $N.\r\n", FALSE, k->follower, 0, ch, TO_CHAR);
        do_leave(k->follower, argument, 0, 0);
      }
      k = next;
    }
    k = ch->tailed;
    while (k) {
      next = k->next;
      if (k->follower->in_room == was_in && GET_POS(k->follower) == POS_STANDING) {
        act("You follow $N.\r\n", FALSE, k->follower, 0, ch, TO_CHAR);
        do_leave(k->follower, argument, 0, 0);
      }
      k = next;
    }
      }
      else {
        if (oprog)
          run_objprog(ch, world[ch->in_room].contents);
        if (mprog)
          run_mobprog(ch, 0);
      }
    } else
    send_to_char("The wagon item to which this room is connected is bugged. Tell an imm.\r\n", ch);
    return;
  }
  if (!ROOM_FLAGGED(ch->in_room, ROOM_INDOORS))
    send_to_char("You are outside.. where do you want to go?\r\n", ch);
  else {
    for (door = 0; door < NUM_OF_DIRS; door++)
      if (EXIT(ch, door))
    if (EXIT(ch, door)->to_room != NOWHERE)
      if (!IS_SET(EXIT(ch, door)->exit_info, EX_CLOSED) &&
        !ROOM_FLAGGED(EXIT(ch, door)->to_room, ROOM_INDOORS)) {
        perform_move(ch, door, 1);
        return;
      }
    send_to_char("I see no obvious exits to the outside.\r\n", ch);
  }
}


ACMD(do_stand)
{
  if (affected_by_spell(ch, SKILL_CAMP)) {
    send_to_char("You break the camp and stand up.\r\n", ch);
    act("$n breaks the camp and stands up.", TRUE, ch, 0, 0, TO_ROOM);
    affect_from_char(ch, SKILL_CAMP);
  }
  switch (GET_POS(ch)) {
  case POS_STUNNED:
    act("You can't stand up. You're stunned!",FALSE,ch,0,0,TO_CHAR);
    break;
  case POS_STANDING:
    act("You are already standing.", FALSE, ch, 0, 0, TO_CHAR);
    break;
  case POS_SITTING:
    act("You stand up.", FALSE, ch, 0, 0, TO_CHAR);
    act("$n clambers to $s feet.", TRUE, ch, 0, 0, TO_ROOM);
    GET_POS(ch) = POS_STANDING;
    GET_SEAT(ch) = NULL;
    break;
  case POS_RESTING:
    act("You stop resting, and stand up.", FALSE, ch, 0, 0, TO_CHAR);
    act("$n stops resting, and clambers to $s feet.", TRUE, ch, 0, 0, TO_ROOM);
    GET_POS(ch) = POS_STANDING;
    GET_SEAT(ch) = NULL;
    break;
  case POS_SLEEPING:
    if (!(affected_by_spell(ch, SPELL_SLEEP))){
      GET_POS(ch) = POS_STANDING;
      GET_SEAT(ch) = NULL;
      act("You stop sleeping and scramble to your feet.", FALSE, ch, 0, 0, TO_CHAR);
      act("$n stops sleeping and clambers to $s feet.",TRUE, ch, 0, 0, TO_ROOM);	
    } 
    else
      send_to_char("You cannot wake up!  Someone is preventing it!\r\n", ch);
    break;
  case POS_FIGHTING:
    act("Do you not consider fighting as standing?", FALSE, ch, 0, 0, TO_CHAR);
    break;
  default:
    act("You stop floating around, and put your feet on the ground.",
    FALSE, ch, 0, 0, TO_CHAR);
    act("$n stops floating around, and puts $s feet on the ground.",
    TRUE, ch, 0, 0, TO_ROOM);
    GET_POS(ch) = POS_STANDING;
    GET_SEAT(ch) = NULL;
    break;
  }
}


ACMD(do_sit)
{

  struct obj_data *obj;
  struct char_data *i = NULL, *dummy = NULL, *list = world[ch->in_room].people;
  int bits , counter = 1;

  one_argument(argument, arg);

   if (*arg) {
          
    if (!(bits = generic_find(arg, FIND_OBJ_INV | FIND_OBJ_ROOM | FIND_OBJ_EQUIP, ch, &dummy, &obj))) {
      sprintf(buf, "There doesn't seem to be %s %s here.\r\n", AN(arg), arg);
      send_to_char(buf, ch);
      return;
    }
     
    if ((GET_OBJ_TYPE(obj) != ITEM_FURNITURE) || (GET_OBJ_VAL(obj, 0) == 1)
         || (GET_OBJ_VAL(obj, 0) == 3)) {
      send_to_char("You can't sit on that!\r\n", ch);
      return;
    }
    
    for (i = list; i; i = i->next_in_room)
     if ((ch != i) && (GET_SEAT(i) == obj))
      counter++;

    if (counter > GET_OBJ_VAL(obj, 2)) {
      send_to_char("No room for you!\r\n", ch);
      return;
    }

  }

  
  switch (GET_POS(ch)) {

  case POS_STANDING:

    if (*arg) {
     GET_SEAT(ch) = obj;
     act("You sit $T $p.", FALSE, ch, obj,(char *)furniture_usages[GET_OBJ_VAL(obj, 1)], TO_CHAR);
     act("$n sits $T $p.", FALSE, ch, obj, (char *)furniture_usages[GET_OBJ_VAL(obj, 1)], TO_ROOM);
    }
     
    if (!*arg) {
     GET_SEAT(ch) = NULL;
     act("You sit down on the floor.", FALSE, ch, 0, 0, TO_CHAR);
     act("$n sits down on the floor.", FALSE, ch, 0, 0, TO_ROOM);
    }
  
  GET_POS(ch) = POS_SITTING;
  break;
  case POS_SITTING:
    send_to_char("You're sitting already.\r\n", ch);
    break;
  case POS_RESTING:
    act("You stop resting, and sit up.", FALSE, ch, 0, 0, TO_CHAR);
    act("$n stops resting.", TRUE, ch, 0, 0, TO_ROOM);
    GET_POS(ch) = POS_SITTING;
    break;
  case POS_SLEEPING:
    act("You have to wake up first.", FALSE, ch, 0, 0, TO_CHAR);
    break;
  case POS_FIGHTING:
    act("Sit down while fighting? are you MAD?", FALSE, ch, 0, 0, TO_CHAR);
    break;
  default:
    act("You stop floating around, and sit down.", FALSE, ch, 0, 0, TO_CHAR);
    act("$n stops floating around, and sits down.", TRUE, ch, 0, 0, TO_ROOM);
    GET_POS(ch) = POS_SITTING;
    break;
  }
}


ACMD(do_rest)
{

  struct obj_data *obj;
  struct char_data *i = NULL, *dummy = NULL, *list = world[ch->in_room].people;
  int bits , counter = 1;
    
  one_argument(argument, arg);
      
   if (*arg) {
    
    if (!(bits = generic_find(arg, FIND_OBJ_INV | FIND_OBJ_ROOM | FIND_OBJ_EQUIP, ch, &dummy, &obj))) {
      sprintf(buf, "There doesn't seem to be %s %s here.\r\n", AN(arg), arg);
      send_to_char(buf, ch);
      return;
    }
  
    if ((GET_OBJ_TYPE(obj) != ITEM_FURNITURE) || (GET_OBJ_VAL(obj, 0) == 3)) {			 
      send_to_char("You can't rest on that !\r\n", ch);
      return;
    }
    
    for (i = list; i; i = i->next_in_room)
     if ((ch != i) && (GET_SEAT(i) == obj))
      counter++;
    
    if (counter > GET_OBJ_VAL(obj, 2)) {
      send_to_char("No room for you !\r\n", ch);
      return;
    }
     
  }

  switch (GET_POS(ch)) {
  case POS_STANDING:

    if (*arg) {
     GET_SEAT(ch) = obj;
     act("You sit $T $p, and rest your tired bones.", FALSE, ch, obj,(char *)furniture_usages[GET_OBJ_VAL(obj, 1)], TO_CHAR);
     act("$n sits $T $p, and rests.", FALSE, ch, obj,(char *)furniture_usages[GET_OBJ_VAL(obj, 1)], TO_ROOM);
    }
      
    if (!*arg) {
     GET_SEAT(ch) = NULL;
     act("You sit down on the floor.", FALSE, ch, 0, 0, TO_CHAR);
     act("$n sits down on the floor.", FALSE, ch, 0, 0, TO_ROOM);
    }

    GET_POS(ch) = POS_RESTING;
    break;
  case POS_SITTING:
    act("You rest your tired bones.", FALSE, ch, 0, 0, TO_CHAR);
    act("$n rests.", TRUE, ch, 0, 0, TO_ROOM);
    GET_POS(ch) = POS_RESTING;
    break;
  case POS_RESTING:
    act("You are already resting.", FALSE, ch, 0, 0, TO_CHAR);
    break;
  case POS_SLEEPING:
    act("You have to wake up first.", FALSE, ch, 0, 0, TO_CHAR);
    break;
  case POS_FIGHTING:
    act("Rest while fighting?  Are you MAD?", FALSE, ch, 0, 0, TO_CHAR);
    break;
  default:
    act("You stop floating around, and stop to rest your tired bones.",
    FALSE, ch, 0, 0, TO_CHAR);
    act("$n stops floating around, and rests.", FALSE, ch, 0, 0, TO_ROOM);
    GET_POS(ch) = POS_SITTING;
    break;
  }
}

void wakeup_sequence(struct char_data *ch)
{ 
  struct char_data *mob = NULL;
  int return_room, i = 0;
  struct obj_data *obj;
  
  if (GET_SLEEPMOB(ch)) {
    mob = GET_SLEEPMOB(ch);
    if (GET_POS(mob) != POS_SLEEPING) {
      send_to_char("Something is happening to your body! You return.\r\n", ch);
    }
  }
  act("$n suddenly vanishes without a trace!", 0, ch, 0, 0, TO_ROOM);
  
  return_room = GET_SLEEPROOM(ch);

  char_from_room(ch);
  char_to_room(ch, return_room); 
  if (mob)
    extract_char(mob, FALSE);
  GET_SLEEPMOB(ch) = NULL;
  save_XML_playerfile(ch,DREAM_FILE);         /* Save all dream eq on file */
  while (ch->carrying) {  /* Remove all the dream eq, since it's saved */
    obj = ch->carrying;
    obj_from_char(obj);
    extract_obj(obj);
  }
  for (i = 0; i < NUM_WEARS; i++)
    if (GET_EQ(ch, i))
      extract_obj(unequip_char(ch, i)); 
  if (GET_DISGUISED(ch))   /* Can't allow DreamDisguise to follow when you wake */
    do_newdisguise(ch,"off",0,0);
  // TOGGLE_BIT(AFF_FLAGS(ch), AFF_DREAMING);
  REMOVE_BIT(AFF_FLAGS(ch), AFF_DREAMING);
  REMOVE_BIT(AFF2_FLAGS(ch),AFF2_TARBLUR);
  GET_POS(ch) = POS_STANDING;
  read_XML_playerfile(ch, CRASH_FILE); /* No longer dreaming -> loading wake eq */
  // act("$n awakes and yawns.", 0, ch, 0, 0, TO_ROOM);
}

ACMD(do_longsleep) 
{
  void remove_char_maintained(struct char_data *ch);
  void remove_room_maintained(struct char_data *ch);
  struct affected_type af;
  af = clear_aff_var(af);

  if (IS_AFFECTED2(ch,AFF2_LONGSLEEP)) {
    send_to_char("You already -are- longsleeping!\r\n",ch);
    return;
  }
  skip_spaces(&argument);
  if (!*argument || strncasecmp(argument,"yes",MIN(strlen(argument),3))) {
    send_to_char("Longsleep will make you char sleep for 10 minues\r\n",ch);
    send_to_char("(RL-time), unable to wake up. However, mobs (or other players)\r\n",ch);
    send_to_char("will be unable to attack you in your sleep.\r\n\r\n",ch);
    send_to_char("If you're really sure you want to go to longsleep, type: LONGSLEEP YES\r\n",ch);
    return;
  }

  if (!cant_channel(ch) && (GET_LEVEL(ch) > 10)) {
    if (IS_SET(PRF_FLAGS(ch),PRF_GRASPING)) {
      REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
    }
    send_to_char("You release any weaves you may be maintaining.\r\n",ch);
    remove_char_maintained(ch);
    remove_room_maintained(ch);
  }
  send_to_char("You fall into deep sleep.\r\n", ch);
  act("$n lies down and falls into \\c01deep\\c00sleep.", TRUE, ch, 0, 0, TO_ROOM);
  GET_POS(ch) = POS_SLEEPING;

  af.type = LONGSLEEP_TEXT;
  af.duration   = 8;
  af.modifier   = 0;
  af.location   = APPLY_NONE;
  af.bitvector  = 0;
  af.bitvector2 = AFF2_LONGSLEEP;
  affect_to_char(ch,&af);
}

ACMD(do_sleep)
{

  struct obj_data *obj;
  struct char_data *i = NULL, *dummy = NULL, *list = world[ch->in_room].people;
  int bits , counter = 1;
  void remove_char_maintained(struct char_data *ch);
  void remove_room_maintained(struct char_data *ch);
    
  one_argument(argument, arg);
    
   if (*arg) {
    
    if (!(bits = generic_find(arg, FIND_OBJ_INV | FIND_OBJ_ROOM | FIND_OBJ_EQUIP, ch, &dummy, &obj))) {
      sprintf(buf, "There doesn't seem to be %s %s here.\r\n", AN(arg), arg);
      send_to_char(buf, ch);
      return;
    }

    if ((GET_OBJ_TYPE(obj) != ITEM_FURNITURE) || (GET_OBJ_VAL(obj, 0) == 0)
             || (GET_OBJ_VAL(obj, 0) == 3)) {
      send_to_char("You can't sleep on that!\r\n", ch);
      return;
    }

    for (i = list; i; i = i->next_in_room)
     if ((ch != i) && (GET_SEAT(i) == obj))
      counter++;

    if (counter > GET_OBJ_VAL(obj, 2)) {
      send_to_char("No room for you!\r\n", ch);
      return;
    }

  }
   
  if (IS_AFFECTED2(ch,AFF2_TAR_GATE)) {
   send_to_char("I don't think sleeping in Tel'Aran'Rhiod while in the flesh is such a good idea.\r\n", ch);
   return;
  }

  if (IS_AFFECTED(ch, AFF_DREAMING)) {
    send_to_char("Erm...you -are- asleep.\r\n", ch);     
    return;
  }

  if (!cant_channel(ch) && IS_SET(PRF_FLAGS(ch),PRF_GRASPING)) {
    if (GET_LEVEL(ch) > 10) {
      do_release(ch,"",0,0);
      if (IS_SET(PRF_FLAGS(ch),PRF_GRASPING)) {
    send_to_char("AAARGH! You have to release the One Power before sleeping!\r\n",ch);
    return;
      }
    } else {
      REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING); // For lvls lower than 10, just release. 
    }
  }

  switch (GET_POS(ch)) {
  case POS_STANDING:  

  if (*arg) {
     GET_SEAT(ch) = obj;
     act("You go to sleep $T $p.", FALSE, ch, obj, (char *)furniture_usages[GET_OBJ_VAL(obj, 1)], TO_CHAR);
     act("$n goes to sleep $T $p.", FALSE, ch, obj, (char *)furniture_usages[GET_OBJ_VAL(obj, 1)], TO_ROOM);
    }
      
  if (!*arg) {
     GET_SEAT(ch) = NULL;
     act("You lie on the floor, and go to sleep.", FALSE, ch, 0, 0, TO_CHAR);
     act("$n lies on the floor, and goes to sleep.", FALSE, ch, 0, 0, TO_ROOM);
    }

  if (!cant_channel(ch) && (GET_LEVEL(ch) > 10)) {
      if (IS_SET(PRF_FLAGS(ch),PRF_GRASPING)) {
        REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
      }
      send_to_char("You release any weaves you may be maintaining.\r\n",ch);
      remove_char_maintained(ch);
      remove_room_maintained(ch);
    }
  GET_POS(ch) = POS_SLEEPING;
  break;
  case POS_SITTING:  
  case POS_RESTING:  
    send_to_char("You go to sleep.\r\n", ch);
    act("$n lies down and falls asleep.", TRUE, ch, 0, 0, TO_ROOM);
    GET_POS(ch) = POS_SLEEPING;
    if (!cant_channel(ch) && (GET_LEVEL(ch) > 10)) {
      if (IS_SET(PRF_FLAGS(ch),PRF_GRASPING)) {
        REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
      }
      send_to_char("You release any weaves you may be maintaining.\r\n",ch);       
      remove_char_maintained(ch);
      remove_room_maintained(ch);
    }
    break;
  case POS_SLEEPING:     
    send_to_char("You are already sound asleep.\r\n", ch);
    break;
  case POS_FIGHTING:
    send_to_char("Sleep while fighting?  Are you MAD?\r\n", ch);
    break;
  default:
    act("You stop floating around, and lie down to sleep.",
    FALSE, ch, 0, 0, TO_CHAR);
    act("$n stops floating around, and lie down to sleep.",
    TRUE, ch, 0, 0, TO_ROOM);
    GET_POS(ch) = POS_SLEEPING;
    break;
  }
}


ACMD(do_wake)
{
  struct char_data *vict;
  int self = 0;

  one_argument(argument, arg);
  if (*arg) {
    if (GET_POS(ch) == POS_SLEEPING)
      send_to_char("Maybe you should wake yourself up first.\r\n", ch);
    else if ((vict = get_char_room_vis(ch, arg)) == NULL)
      send_to_char(NOPERSON, ch);
    else if (vict == ch)
      self = 1;
    else if (GET_POS(vict) > POS_SLEEPING)
      act("$E is already awake.", FALSE, ch, 0, vict, TO_CHAR);
    else if (IS_AFFECTED(vict, AFF_SLEEP))
      act("You can't wake $M up!", FALSE, ch, 0, vict, TO_CHAR);
    else if (GET_POS(vict) < POS_SLEEPING)
      act("$E's in pretty bad shape!", FALSE, ch, 0, vict, TO_CHAR);
    else {
      if (IS_AFFECTED2(vict,AFF2_LONGSLEEP)) {
        asend_to_char(ch,"You can't wake %s up!\r\n",GET_NAME(vict));
        return;
      }
      act("You wake $M up.", FALSE, ch, 0, vict, TO_CHAR);
      act("You are awakened by $n.", FALSE, ch, 0, vict, TO_VICT | TO_SLEEP);
      GET_POS(vict) = POS_SITTING;
      if (GET_DREAMER(vict))
        wakeup_sequence(GET_DREAMER(vict));
    }
    if (!self)
      return;
  }
  if (FIGHTING(ch) && IS_AFFECTED(ch, AFF_DREAMING)) {
    send_to_char("You can't concentrate enough to wake up! You're stuck!\r\n", ch);
    return;
  }
  if (IS_AFFECTED(ch, AFF_SLEEP))
    send_to_char("You can't wake up!\r\n", ch);
  else if (IS_AFFECTED(ch, AFF_DREAMING) && GET_DREAMENTERED(ch)) {
    REMOVE_BIT(ROOM_FLAGS(IN_ROOM(ch)), ROOM_DREAM);
    wakeup_sequence(ch);
    send_to_char("You awaken, and stand up.\r\n", ch);
    act("$n awakens and stands up.", TRUE, ch, 0, 0, TO_ROOM);
    if (GET_DREAMENTERED(ch))
     if (GET_POS(GET_DREAMENTERED(ch)) == POS_SLEEPING)
     {
      wakeup_sequence(GET_DREAMENTERED(ch));
      send_to_char("You awaken, and stand up.\r\n", GET_DREAMENTERED(ch));
      act("$n awakens and stands up.", TRUE, GET_DREAMENTERED(ch), 0, 0, TO_ROOM);
     }
    GET_DREAMENTERED(ch) = NULL;
    return;
  }
  else if (IS_AFFECTED2(ch,AFF2_DARKDREAM) && IS_AFFECTED(ch,AFF_DREAMING) && !IS_AFFECTED2(ch,AFF2_TAR_GATE)) 
  {
    REMOVE_BIT(AFF2_FLAGS(ch),AFF2_DARKDREAM);
    //act("$n suddenly gets misty and vanishes, no trace remaining.",FALSE,ch,NULL,NULL,TO_ROOM);
    wakeup_sequence(ch);
    send_to_char("You awaken, and stand up.\r\n",ch);
    act("$n awakens.", TRUE, ch, 0, 0, TO_ROOM);
    GET_DREAMPULLED(ch) = NULL;
    return;
  }
/* Normal Dreamer */
  else if (NORMAL_DREAMER(ch) && (GET_SKILL(ch, SKILL_DREAM) < number(0,100)) 
             && (!(GET_SKILL(ch, SKILL_DREAM) == 0))) 
    send_to_char("You fail to wake up and leave Tel'aran'rhiod.\r\n", ch);
/* A pulled char , in the case when the puller already woke up */
  else if (NORMAL_DREAMER(ch) && (GET_SKILL(ch, SKILL_DREAM) == 0) 
                 && (!(IS_AFFECTED(GET_DREAMPULLED(ch), AFF_DREAMING))) 
             && (20 < number(0,100)))
    send_to_char("You fail to wake up and leave Tel'aran'rhiod.\r\n", ch); 
/* A pulled char , in the case when the puller is still in TAR */
  else if (NORMAL_DREAMER(ch) && (GET_SKILL(ch, SKILL_DREAM) == 0) 
             && (IS_AFFECTED(GET_DREAMPULLED(ch), AFF_DREAMING)))
    send_to_char("You fail to wake up and leave Tel'aran'rhiod.\r\n", ch); 
  else if ((GET_POS(ch) > POS_SLEEPING) && (!IS_AFFECTED(ch, AFF_DREAMING) || IS_AFFECTED2(ch,AFF2_TAR_GATE)))
    send_to_char("You are already awake...\r\n", ch);
  else { 
    send_to_char("You awaken, and stand up.\r\n", ch);
    GET_DREAMPULLED(ch) = NULL;
    GET_POS(ch) = POS_STANDING;
    GET_SEAT(ch) = NULL;
    if (NORMAL_DREAMER(ch)) {
      wakeup_sequence(ch);
    }
    act("$n awakens.", TRUE, ch, 0, 0, TO_ROOM);
  }
}


ACMD(do_follow)
{
  struct char_data *leader;

  void stop_follower(struct char_data *ch);
  void add_follower(struct char_data *ch, struct char_data *leader);

  one_argument(argument, buf);

  if (ch->player_specials->linked_by)
  {
    send_to_char("You cannot follow anyone else until you are released from the link!\r\n", ch);
    return;
  }
  
  if (*buf) {
    if (!(leader = get_char_room_vis(ch, buf))) {
      send_to_char(NOPERSON, ch);
      return;
    }
  } else {
    send_to_char("Whom do you wish to follow?\r\n", ch);
    return;
  }

  if (ch->master == leader) {
    act("You are already following $M.", FALSE, ch, 0, leader, TO_CHAR);
    return;
  }
  if (IS_AFFECTED(ch, AFF_CHARM) && (ch->master)) {
    act("But you only feel like following $N!", FALSE, ch, 0, ch->master, TO_CHAR);
  } else {			/* Not Charmed follow person */
    if (leader == ch) {
      if (!ch->master) {
    send_to_char("You are already following yourself.\r\n", ch);
    return;
      }
      stop_follower(ch);
      if (IS_AFFECTED(ch,AFF_MORALE))
        affect_from_char(ch,SKILL_MORALE);
    } else {
      if (circle_follow(ch, leader)) {
    act("Sorry, but following in loops is not allowed.", FALSE, ch, 0, 0, TO_CHAR);
    return;
      }
      if (ch->master)
    stop_follower(ch);
      if (IS_AFFECTED(ch,AFF_MORALE))
        affect_from_char(ch,SKILL_MORALE);
      REMOVE_BIT(AFF_FLAGS(ch), AFF_GROUP);
      add_follower(ch, leader);
    }
  }
}

ACMD(do_recall)
{
 int guild_num, room;
 char *ch_mss[]={"neutral","menacing","golden"}, tpbuf[200]; 
 struct char_guild_element *element;
 one_argument(argument, arg);

  if (SESS_FLAGGED(ch, SESS_EAVESDROP)) {
    send_to_char("You move away from where you were and can no longer hear soft voices.\r\n", ch);
    (void)SESS_TOG_CHK(ch, SESS_EAVESDROP);
    rem_eavesdropper(ch, GET_EAVESDROPROOM(ch));
    GET_EAVESDROPROOM(ch) = 0;
  }

 if (*arg) {
  element = GET_CHAR_GUILDS(ch);
  while (element) {
   guild_num = atoi(arg);
   if ((element->guild->id == guild_num) && STATUS_FLAGGED(element->guildie, STATUS_MEMBER)) {
    do_guild_walk(ch, element);
    return;
   }
   if (!strncasecmp(element->guild->name, arg, strlen(arg)) && 
       STATUS_FLAGGED(element->guildie, STATUS_MEMBER)) {
    do_guild_walk(ch, element);
    GET_POS(ch) = POS_STANDING;
    GET_SEAT(ch) = NULL; // Stand up from furnitures
    return;
   }
   element = element->next;
  }
 }

  if (GET_POS(ch) == POS_FIGHTING)
  {
    send_to_char("Oh no you don't! Not while you're fighting.\r\n", ch);
    return;
  }

  if (ROOM_FLAGGED(ch->in_room, ROOM_NORECALL))
  {
    send_to_char("You can't recall from here!\r\n", ch);
    return;
  }

  if (GET_LEVEL(ch) > 40 && IS_SET(PRF_FLAGS(ch),PRF_IC))
    {
    send_to_char("Sorry, recalling is only allowed up to level 40 unless you are OOC. Have a nice walk.\r\n",ch);
    return;
  }

  if(IS_AFFECTED2(ch, AFF2_SUBDUED))
    {
    send_to_char("You can't recall while being a captive!\n\r",ch);
    return;
    }

  if (ch->player_specials->linking[0])
  {
    send_to_char("You must dissolve your link first!\r\n", ch);
    return;
  }
  
  act("$n suddenly vanishes!",TRUE,ch,0,0,TO_ROOM);
  TICKS_IN_ROOM(ch)=0;
  room = (IS_NEWBIE(ch) && *arg && !strncasecmp("newbie", arg, strlen(arg))) ? newbie_room : recall_room;
  char_from_room(ch);
  char_to_room(ch,real_room(room)); 
  act("$n suddenly appears out of nowhere!",TRUE,ch,0,0,TO_ROOM);
  send_to_char("Okay, you have recalled. Enjoy! :)\r\n",ch);
  if(ROOM_FLAGGED(ch->in_room,ROOM_NOMAGIC) && IS_SET(PRF_FLAGS(ch),PRF_GRASPING))
  { 
     send_to_char("Suddenly you lose your grip on the One Power!\r\n",ch);
     sprintf(tpbuf,"The %s aura around $n suddenly vanishes!",ch_mss[(int)GET_SEX(ch)]);
     act(tpbuf,FALSE,ch,NULL,NULL,TO_OPUSERSS);
     remove_char_maintained(ch);
     remove_room_maintained(ch);
     REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
  }
  GET_POS(ch) = POS_STANDING;
  GET_SEAT(ch) = NULL;
  do_look(ch,"\0",0,SCMD_LOOK);
}

ACMD(do_home)
{
  char *ch_mss[]={"neutral","menacing","golden"}, tpbuf[200];

  if (GET_POS(ch) == POS_FIGHTING)
  {
    send_to_char("Oh no you don't! Not while fighting.\r\n", ch);
    return;
  }

  if(IS_AFFECTED2(ch, AFF2_SUBDUED))
    {
    send_to_char("You can't recall while being a captive!\n\r",ch);
    return;
    }


  if (ROOM_FLAGGED(ch->in_room, ROOM_NORECALL))
  {
    send_to_char("You can't recall from here!\r\n", ch);
    return;
  }
  if (ch->player_specials->linking[0])
  {
    send_to_char("You must dissolve your link first!\r\n", ch);
    return;
  }
 
  if (GET_LEVEL(ch) > 40) {
    send_to_char("Sorry, your level is too high. Enjoy the walk home!\r\n",ch);
    return;
  }
  act("$n suddenly vanishes!",TRUE,ch,0,0,TO_ROOM);
  TICKS_IN_ROOM(ch)=0;
  char_from_room(ch);
  char_to_room(ch,real_room(home_rooms[GET_RACE(ch)]));
  act("$n suddenly appears out of nowhere!",TRUE,ch,0,0,TO_ROOM);
  send_to_char("Home sweet home!\r\n",ch);
  if(ROOM_FLAGGED(ch->in_room,ROOM_NOMAGIC) && IS_SET(PRF_FLAGS(ch),PRF_GRASPING))
  {
    send_to_char("Suddenly you lose your grip on the One Power!\r\n",ch);
    sprintf(tpbuf,"The %s aura around $n suddenly vanishes!",ch_mss[(int)GET_SEX(ch)]);
    act(tpbuf,FALSE,ch,NULL,NULL,TO_OPUSERSS);
    remove_char_maintained(ch);
    remove_room_maintained(ch);
    REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
  }
  do_look(ch,"\0",0,SCMD_LOOK);
}

ACMD(do_dream)
{
  int result = 0, i = 0,pos = -1;
  struct char_data *dreamer;
  struct obj_data *obj=NULL,*dreamobj = NULL;

  char dreamtext[300] = 
  {
    "\r\n&wYou find yourself floating in nothing for a short while...\r\n"
    "But suddenly everything clears, gets definition, and you\r\n"
    "find yourself in &CTel'aran'rhiod&w, the World of Dreams.\r\n"
    "Rejoice...and beware.&n\r\n\r\n"
  };

  if (GET_POS(ch) != POS_SLEEPING) {
    send_to_char("How can you dream while not sleeping? Daydreaming?\r\n", ch);
    return;
  }
  if (NORMAL_DREAMER(ch) || FLESH_DREAMER(ch)) {
    send_to_char("Hey! You're already in Tel'Aran'Rhiod!\r\n",ch);
    return;
  }
  if (!subcmd) { /*If not using a dream ter'angreal */
    if (!GET_SKILL(ch, SKILL_DREAM)) {
      switch (number(0,5)) {
        case 0:
          send_to_char("You dream of pink fluffy rabbits with nasty big pointed teeth.\r\n", ch);
        break;
        case 1:
          send_to_char("Your sleep is untroubled by dreams.\r\n", ch);
          break;
        case 2:
          send_to_char("You dream of a black, glistening, large, threatening cube..?\r\n", ch);
          break;
        case 3:
          send_to_char("You have a nightmare about pancakes...HUGE pancakes.\r\n", ch);
          break;
        case 4:
          send_to_char("You dream....but the dream feels so unreal you cannot grasp its meaning.\r\n", ch);
          break;
        case 5:
          send_to_char("You dream that you dream that you dream that...\r\n", ch);
          break;
      }
      return;
    }
    result = generic_result(ch, SKILL_DREAM, NULL, FALSE);
    if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
      send_to_char("You try to enter Tel'aran'rhiod, but fail.\r\n", ch);
      return;
    }
  }
  if (!(GET_DREAMENTERED(ch)))
   send_to_char(dreamtext, ch);
  GET_SLEEPROOM(ch) = ch->in_room;
  dreamer = read_mobile(real_mobile(2), REAL);
  sprintf(buf,"the sleeping form of %s", GET_NAME(ch));
  dreamer->player.short_descr = strdup(buf);
  sprintf(buf, "%s %s is sleeping here.\r\n", GET_NAME(ch), GET_TITLE(ch));
  dreamer->player.long_descr = strdup(buf);
  char_to_room(dreamer, ch->in_room);
  GET_SLEEPMOB(ch) = dreamer;
  GET_DREAMER(dreamer) = ch;
  
  if (subcmd && !GET_SKILL(ch,SKILL_DREAM)) { /* we're prolly using a dream-ter'angreal! */
    for (i = 0; i < NUM_WEARS; i++) {
      obj = GET_EQ(ch,i);
      if (obj && ((GET_OBJ_SPEC(obj) == strong_dream_obj) || (GET_OBJ_SPEC(obj) == weak_dream_obj))) {
        dreamobj = obj;
        pos = i;
      }
    }
  }
  if (dreamobj) {
    obj = read_object(GET_OBJ_RNUM(dreamobj),REAL);
    dreamobj = obj;
  }

  save_XML_playerfile(ch, CRASH_FILE);

  if (dreamobj) {
    SET_BIT(obj->obj_flags.extra_flags,ITEM_TAR);
    equip_char(ch,obj,pos);
    if (IS_SET(GET_OBJ_EXTRA(obj), ITEM_TERANGREAL))
     SET_BIT(AFF2_FLAGS(ch), AFF2_TARBLUR);
  }
  SET_BIT(AFF_FLAGS(ch), AFF_DREAMING);
  GET_POS(ch) = POS_STANDING;
  GET_POS(dreamer) = POS_SLEEPING;
  if (!(GET_DREAMENTERED(ch)))
   do_look(ch,"",0,0);
}

ACMD(do_drag)
{
  struct char_data *victim;
  char arg1[200],arg2[200];
  int dir,success = 0,num;

  memset(arg1,'\0',200);
  memset(arg2,'\0',200);

  two_arguments(argument,arg1,arg2);

  if (!*arg1 || !(victim = get_char_room_vis(ch,arg1))) {
    send_to_char("Drag whom?\r\n",ch);
    return;
  }
  
  if (!ok_damage_shopkeeper(ch, victim)) {
    act("$n tries to drag $N away, but $E just glares at $m.", TRUE, ch, 0, victim, TO_NOTVICT);
    return;
  }
  
  if (ROOM_FLAGGED(ch->in_room, ROOM_PEACEFUL)) {
    send_to_char("*sigh* this -is- a peaceful room. Try that somewhere else.\r\n", ch);
    return;
  }
 
  if (FIGHTING(ch)) {
    send_to_char("No, not while fighting!\r\n",ch);
    return;
  }
  if (victim == ch) {
    send_to_char("You try to drag yourself away and fall to the ground.\r\n",ch);
    GET_POS(ch) = POS_SITTING;
    return;
  }
  if (IS_AFFECTED(victim,AFF_DREAMING)) {
    send_to_char("Sorry, that isn't allowed.\r\n",ch);
    return;
  }

  if (!ok_damage_shopkeeper(ch, victim)) {
    return;
  }


  if (!*arg2 || (-1 == (dir = search_block(arg2,dirs,FALSE))) ||
               !EXIT(ch,dir) || !CAN_GO(ch,dir)) {
    act("You drag $N away, but stumble right into a wall!",FALSE,ch,0,victim,TO_CHAR);
    act("$n drags $N but walks right into a wall.",FALSE,ch,0,victim,TO_NOTVICT);
    act("$n tries to drag you away, but walks right into a wall.",FALSE,ch,0,victim,TO_VICT);
    return;
  }

  num = 50 + ((GET_STR(ch) - GET_STR(victim))*5);

  if (number(0,100) < num)
    success = 1;
  if ((GET_POS(victim) == POS_SLEEPING) || IS_AFFECTED(victim,AFF_WRAPPED)||IS_NPC(victim))
    success = 1;

  if (!success) {
    act("You try to drag $N away, but $E is too strong!",FALSE,ch,0,victim,TO_CHAR);
    act("$n tries to drag $N away, but $N is too strong.",FALSE,ch,0,victim,TO_NOTVICT);
    act("$n tries to drag you away, but $e isn't strong enough. Whimp!",FALSE,ch,0,victim,TO_VICT);
    return;
  }
  act("You drag $N away.",FALSE,ch,0,victim,TO_CHAR);
  act("$n drags $N away.",FALSE,ch,0,victim,TO_NOTVICT);
  act("$n drags you away.",FALSE,ch,0,victim,TO_VICT);
  do_simple_move(ch,dir,FALSE);
  do_simple_move(victim,dir,FALSE);

}

ACMD(do_push)
{
  struct char_data *victim;
  char arg1[200],arg2[200];
  char buf[500];
  int dir,success = 0,num;

  memset(arg1,'\0',200);
  memset(arg2,'\0',200);

  two_arguments(argument,arg1,arg2);

  if (!*arg1 || !(victim = get_char_room_vis(ch,arg1))) {
    send_to_char("Push who?\r\n",ch);
    return;
  }
  if (!ok_damage_shopkeeper(ch, victim)) {
    act("$n tries to push $N away, who glares at $m.", TRUE, ch, 0, victim, TO_NOTVICT);
    return;
  }
  
  if (ROOM_FLAGGED(ch->in_room, ROOM_PEACEFUL)) {
    send_to_char("*sigh* this -is- a peaceful room. Try that somewhere else.\r\n", ch);
    return;
  }  
  if (FIGHTING(ch)) {
    send_to_char("No, not while fighting!\r\n",ch);
    return;
  }
  if (victim == ch) {
    send_to_char("You push yourself and fall to the ground.\r\n",ch);
    GET_POS(ch) = POS_SITTING;
    return;
  }
  if (IS_AFFECTED(victim,AFF_DREAMING)) {
    send_to_char("Sorry, that isn't allowed.\r\n",ch);
    return;
  }
  if (!ok_damage_shopkeeper(ch, victim)) {
    return;
  }


  if (!*arg2 || (-1 == (dir = search_block(arg2,dirs,FALSE))) ||
               !EXIT(ch,dir) || !CAN_GO(ch,dir)) {
    act("You push $N.",FALSE,ch,0,victim,TO_CHAR);
    act("$n pushes $N.",FALSE,ch,0,victim,TO_NOTVICT);
    act("$n pushes you.",FALSE,ch,0,victim,TO_VICT);
    return;
  }
  num = 50 + ((GET_STR(ch) - GET_STR(victim))*5);

  if (number(0,100) < num)
    success = 1;
  if ((GET_POS(victim) == POS_SLEEPING) || IS_AFFECTED(victim,AFF_WRAPPED) || IS_NPC(victim))
    success = 1;

  if (!success) {
    act("You try to push $N, but $E is too strong!",FALSE,ch,0,victim,TO_CHAR);
    act("$n tries to push $N, but $N is too strong.",FALSE,ch,0,victim,TO_NOTVICT);
    act("$n tries to push you, but $e isn't strong enough. Whimp!",FALSE,ch,0,victim,TO_VICT);
    return;
  }
  sprintf(buf,"You push  $N %s.",dirs[dir]);
  act(buf,FALSE,ch,0,victim,TO_CHAR);
  sprintf(buf,"$n pushes $N %s",dirs[dir]);
  act(buf,FALSE,ch,0,victim,TO_NOTVICT);
  sprintf(buf,"$n pushes you %s.",dirs[dir]);
  act(buf,FALSE,ch,0,victim,TO_VICT);
  do_simple_move(victim,dir,FALSE);
}


/* Written for the run_which_direction routine.  opposite_direction is the
direction from which the character has come. */
/* This is based on the direction definitions.  NORTH == 0 and SOUTH == 2, etc */
int opposite_direction(int dir){
  if (dir == 0)
    return 2;
  if (dir == 1)
    return 3;
  if (dir == 2)
    return 0;
  if (dir == 3)
    return 1;
  if (dir == 4)
    return 5;
  if (dir == 5)
    return 4;
  if (dir == 6)
    return 8;
  if (dir == 7)
    return 9;
  if (dir == 8)
    return 6;
  return 7;  
}

int run_which_direction(struct char_data *ch, int dir){
  int i, exit_count = 0, exit = 0;
  for (i = 0; i < NUM_OF_DIRS; i++)
    if ((CAN_GO(ch, i)) && (opposite_direction(dir) != i)){
      exit_count++;
      exit = i;
    }
  if (exit_count != 1)
    return NUM_OF_DIRS;
  return exit;
}

ACMD(do_run){
  int dir, was_in;
  struct follow_type *k, *next;
  static int count;

  one_argument(argument, arg);

  if (!GET_SKILL(ch, SKILL_RUN)){
    send_to_char("You fall to the floor panting, just thinking about it!\r\n", ch);
    return;
  }

  if (!*arg){
    send_to_char("It is generally good to choose a direction in which to run.\r\n", ch);
    return;
  }

  for (dir = 0;(strncasecmp(arg, dirs[dir], strlen(arg)) && (dir < NUM_OF_DIRS)); dir++);

  was_in = ch->in_room;

  if (dir == NUM_OF_DIRS){
    send_to_char("You can try to run that way all you want, but it won't happen!\r\n", ch);
    return;
  }

  if (!CAN_GO(ch, dir)){
    send_to_char("You can try to run that way all you want, but it won't happen!\r\n", ch);
    return;
  }

  TOGGLE_BIT(AFF_FLAGS(ch), AFF_RUNNING);

  do{
    count++;
    if (!do_simple_move(ch, dir, TRUE) || count >= 50){
      act("$n stops running, and isn't even out of breath.", FALSE, ch, 0, 0, TO_ROOM);
      TOGGLE_BIT(AFF_FLAGS(ch), AFF_RUNNING);
      count = 0;
      return;
    }
  }
  while (NUM_OF_DIRS != (dir = run_which_direction(ch, dir)));

  act("$n stops running, and isn't even out of breath.", FALSE, ch, 0, 0, TO_ROOM);
  send_to_char("You stop running, wondering which way to go next.\r\n", ch);
  TOGGLE_BIT(AFF_FLAGS(ch), AFF_RUNNING);

  if (ch->followers){
    for (k = ch->followers; k; k = next){
      next = k->next;
      if ((k->follower->in_room == was_in) && (GET_POS(k->follower) >= POS_STANDING)){
        if (GET_SKILL(k->follower, SKILL_RUN))
          do_run(k->follower, arg, 0, 0);
        else
          send_to_char("You don't even try to keep up.\r\n", k->follower);
      }
    }
  }

}

/*static int find_zone_top(int rvnum)
{
  int i;

  for (i = 0; i < top_of_zone_table; i++) {
    if (zone_table[i].top < rvnum && zone_table[i+1].top > rvnum)
      return zone_table[i].top;
  }
  if (zone_table[top_of_zone_table].top < rvnum)
    return zone_table[i].top;

  return 0;
}
*/
ACMD(do_speedwalk)
{
  sh_int room_num = 0;
  char *ch_mss[]={"neutral","menacing","golden"}, tpbuf[200];
  struct follow_type *foll = NULL;
  int dir;
  int get_guild_by_room(struct char_data *ch, int room);

  skip_spaces(&argument);

  if (!*argument) {
    send_to_char("usage: speedwalk <room name>\r\n",ch);
    return;
  }
  if (!test_bit(GET_KNOW(ch),world[ch->in_room].number)) {
    send_to_char("You don't know this location well enough to speedwalk from here.\r\n",ch);
    return;
  }
  if (!(room_num = find_room_by_name(argument))) { 
    send_to_char("Hey! There's no such room on this MUD!\r\n",ch);
    return;
  }
  if (!test_bit(GET_KNOW(ch),world[room_num].number)) {
    char  buf[MAX_STRING_LENGTH];
    sprintf( buf, "You don't know \"%-.100s\" well enough to speedwalk there.\r\n", world[room_num].name );
    send_to_char( buf, ch );
    return;
  }

  if (zone_table[world[room_num].zone].lock && !IS_IMMORTAL(ch)) {
    send_to_char("Sorry.  That Zone is locked!\r\n", ch);
    return;
  }
    
  if (ROOM_FLAGGED(room_num,ROOM_GODROOM) && !IS_IMMORTAL(ch)) {
    send_to_char("Sorry, you can't speedwalk to that room...\r\n",ch);
    return;
  }
   
  if (ROOM_FLAGGED(room_num,ROOM_NOGATE)) {
    send_to_char("Sorry, you can't speedwalk to that room...\r\n",ch);
    return;
  }
   
  if (ROOM_FLAGGED(room_num,ROOM_GUILD) && !get_guild_by_room(ch, room_num)) {
       send_to_char("Sorry, you can't speedwalk to that room...\r\n",ch);
       return;
  }

  dir = b_find_first_step(ch->in_room,room_num);
  if ((dir == BFS_NO_PATH) || (dir == BFS_ERROR)) {
    send_to_char("There's no way to travel from this room to that one!\r\n",ch);
    return;
  }

  send_to_char("Okay, you start to speedwalk towards your destination.\r\n",ch);
  act("$n starts walking away at high speed.",TRUE,ch,0,0,TO_ROOM);
  GET_TRAVEL_TO(ch) = room_num; //world[room_num].number;
  if(ROOM_FLAGGED(ch->in_room,ROOM_NOMAGIC) && IS_SET(PRF_FLAGS(ch),PRF_GRASPING))
  {
    send_to_char("Suddenly you lose your grip on the One Power!\r\n",ch);
    sprintf(tpbuf,"The %s aura around $n suddenly vanishes!",ch_mss[(int)GET_SEX(ch)]);
    act(tpbuf,FALSE,ch,NULL,NULL,TO_OPUSERSS);
    remove_char_maintained(ch);
    remove_room_maintained(ch);
    REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
  } 
  
  if (ch->followers) {
    for (foll = ch->followers; foll; foll = foll->next) {
      if (foll->follower->in_room == ch->in_room){

    act("$n follows right behind.", TRUE, foll->follower, 0, 0, TO_VICT);
    asend_to_char(foll->follower,"You follow right behind %s.\r\n",GET_NAME(ch)); 

        GET_TRAVEL_TO(foll->follower) = GET_TRAVEL_TO(ch);
        if(ROOM_FLAGGED(ch->in_room,ROOM_NOMAGIC) && IS_SET(PRF_FLAGS(ch),PRF_GRASPING))
        {
          send_to_char("Suddenly you lose your grip on the One Power!\r\n",ch);
          sprintf(tpbuf,"The %s aura around $n suddenly vanishes!",ch_mss[(int)GET_SEX(ch)]);
          act(tpbuf,FALSE,ch,NULL,NULL,TO_OPUSERSS);
          remove_char_maintained(ch);
          remove_room_maintained(ch);
          REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
        }
      }
    }
  }
}

ACMD(do_gencart)
{
  sh_int room_num = 0;
  struct follow_type *foll = NULL;
  int dir;
  int get_guild_by_room(struct char_data *ch, int room);

  skip_spaces(&argument);

  if (!*argument) {
    send_to_char("usage: cart <room name>\r\n",ch);
    return;
  }
  if (IS_AFFECTED(ch, AFF_DREAMING)){
    send_to_char("Sorry, no carts in Tel'aran'rhiod...\r\n", ch);
    return;
  }
  if (!OUTSIDE(ch)){
    act("$n stands in the room shouting for a cart. What an idiot.", TRUE, ch, 0, 0, TO_ROOM);
    send_to_char("You yell for a cart, but not one comes...Could it be because you are indoors?\r\n", ch);
    return;
  }
  if (!(room_num = find_room_by_name(argument))) {
    send_to_char("Hey! There's no such room on this MUD!\r\n",ch);
    return;
  }

  if (zone_table[world[room_num].zone].lock && !IS_IMMORTAL(ch)) {
    send_to_char("Sorry.  That Zone is locked!\r\n", ch);
    return;
  }

  if (ROOM_FLAGGED(room_num,ROOM_GODROOM) && !IS_IMMORTAL(ch)) {
    send_to_char("Sorry, you can't cart to that room...\r\n",ch);
    return;
  }

  if (ROOM_FLAGGED(room_num,ROOM_NOGATE)) {
    send_to_char("Sorry, you can't cart to that room...\r\n",ch);
    return;
  }

  if (ROOM_FLAGGED(room_num,ROOM_GUILD) && !get_guild_by_room(ch, room_num)) {
    send_to_char("Sorry, you can't cart to that room...\r\n",ch);
    return;
  }

  act("$n calls gallantly for a cart, flashing some gold.", TRUE, ch, 0, 0, TO_ROOM);
  send_to_char("You call for a cart driver's attention.\r\n", ch);

  dir = b_find_first_step(ch->in_room,room_num);
  if ((dir == BFS_NO_PATH) || (dir == BFS_ERROR)) {
    send_to_char("The driver looks really embarrassed as he confesses he doesn't know the way.\r\n",ch);
    return;
  }

  if (GET_GOLD(ch) < 1500) {
    act("The cart driver that appears screams obscenities at $n as he realises $e has no money.", TRUE, ch, 0, 0, TO_ROOM);
    send_to_char("The cart driver says sourly, '1500 gold or no ride.' and vanishes down the road.\r\n", ch);
    return;
  }
  GET_GOLD(ch) -= 1500;
  GET_DEST(ch) = world[room_num].number;
 
  send_to_char("The driver takes 1500 gold for the ride and then opens the door.\r\n", ch);
  act("$n pays the driver 1500 gold coin and enters the wagon.", TRUE, ch, 0, 0,TO_ROOM);
  
  if (ch->followers) {
    for (foll = ch->followers; foll; foll = foll->next) {
      if (foll->follower->in_room == ch->in_room){
        send_to_char("You enter as well.\r\n", foll->follower);
    act("$n follows right behind.", TRUE, foll->follower, 0, 0, TO_ROOM);
        GET_DEST(foll->follower) = GET_DEST(ch);
      }
    }
  }

}


ACMD(do_getaride)
{
  struct follow_type *foll;
  int temp, zone = 0, i = 0, found = 0;


  if (SESS_FLAGGED(ch, SESS_EAVESDROP)) {
    send_to_char("You move away from where you were and can no longer hear soft voices.\r\n", ch);
    (void)SESS_TOG_CHK(ch, SESS_EAVESDROP);
    rem_eavesdropper(ch, GET_EAVESDROPROOM(ch));
    GET_EAVESDROPROOM(ch) = 0;
  }
  if (IS_AFFECTED(ch, AFF_DREAMING)){
    send_to_char("Sorry, no carts in Tel'aran'rhiod...\r\n", ch);
    return;
  }
  if (!OUTSIDE(ch)){
    act("$n stands in the room shouting for a cart. What an idiot.", TRUE, ch, 0, 0, TO_ROOM);
    send_to_char("You yell for a cart, but not one comes...Could it be because you are indoors?\r\n", ch);
    return;
  }  
  for (temp = 0; (cartzones[temp].max) ; temp++)
    if ((cartzones[temp].max >= world[ch->in_room].number) && 
(cartzones[temp].min <= world[ch->in_room].number)){
      found =  TRUE;
      zone = temp;
    }
  if (!found){
    act("$n starts yelling in the middle of nowhere for a cart. City people...", TRUE, ch, 0, 0, TO_ROOM);
    send_to_char("You really expect to find a cart here?? Get a grip.\r\n", ch);
    return;
  }
  act("$n calls gallantly for a cart, flashing some gold.", TRUE, ch, 0, 0, TO_ROOM);
  send_to_char("You call for a cart driver's attention.\r\n", ch);
  if (GET_GOLD(ch) < 200){
    act("The cart driver that appears screams obscenities at $n as he realises $e has no money.", TRUE, ch, 0, 0, TO_ROOM);
    send_to_char("The cart driver says sourly, '200 gold or no ride.' and vanishes down the road.\r\n", ch);
    return;
  }

  if (!*argument)
  {
    send_to_char("The cart driver says politely, 'Where do you wish to go sir?'\r\n", ch);
    send_to_char("These places are the ones to which you can go:\r\n", ch);
    i = 0;
    do if (world[real_room(cartzones[zone].destinations[i])].name)
      { 
      sprintf(buf, "    %s\r\n", world[real_room(cartzones[zone].destinations[i])].name);
      send_to_char(buf, ch);
    }
    while (cartzones[zone].destinations[++i]); 
    return;
  }
  i = 0;
  argument++;
  while (!isname(argument, world[real_room(cartzones[zone].destinations[i])].name) &&
    (cartzones[zone].destinations[i]))
    i ++;
  if (!cartzones[zone].destinations[i]){
    send_to_char("The driver says, puzzled, 'Never heard of any such place sir...sorry.' and leaves.\r\n", ch);
    return;
  }
  GET_DEST(ch) = cartzones[zone].destinations[i];
  GET_GOLD(ch) -= 200;
  send_to_char("The drivers take 200 gold for the ride and then gallantly opens the door.\r\n", ch);
  act("$n pays the driver 200 gold coin and enters the wagon.", TRUE, ch, 0, 0, TO_ROOM);
  if (ch->followers) {
    for (foll = ch->followers; foll; foll = foll->next) {
      if (foll->follower->in_room == ch->in_room){
        send_to_char("You enter as well.\r\n", foll->follower);
    act("$n follows right behind.", TRUE, foll->follower, 0, 0, TO_ROOM);
        GET_DEST(foll->follower) = GET_DEST(ch);
      }
    }
  }
}

ACMD(do_dreamgoto)
{
  sh_int roomnr = 0;
  char *ch_mss[]={"neutral","menacing","golden"}, tpbuf[200];
  
  if (!IS_AFFECTED(ch, AFF_DREAMING)) {
    send_to_char("You jump for joy.\r\n", ch);
    act("$n jumps happily around the room.\r\n", TRUE, ch, 0, 0, TO_ROOM);
    return;
  }
  if (FIGHTING(ch)) {
    send_to_char("Impossible! You can't concentrate enough while fighting!\r\n", ch);
    return;
  }
  if (GET_SKILL(ch, SKILL_DREAM) < 75) {
    send_to_char("Sorry, you're not skilled enough at Dreaming to use this command.\r\n", ch);
    return;
  }
  if (!*argument) {
    send_to_char("You have to supply a room name too, you know. \r\n", ch);
    return;
  }
  argument ++;
  roomnr = find_room_by_name(argument);
  if (!roomnr) {
    send_to_char("No room found with that name...Try again.\r\n", ch);
    return;
  }
  if (ROOM_FLAGGED(roomnr,ROOM_NOGATE)||ROOM_FLAGGED(roomnr,ROOM_NOMAGIC)) {
    send_to_char("You try to jump there, but something seems to keep you out!\r\n",ch);
    return;
  }
  send_to_char(OK, ch);
  act("$n looks concentrated for a while, then $e suddenly shimmers and vanishes.", TRUE, ch, 0, 0, TO_ROOM);
  char_from_room(ch);
  char_to_room(ch, roomnr);
  if(ROOM_FLAGGED(ch->in_room,ROOM_NOMAGIC) && IS_SET(PRF_FLAGS(ch),PRF_GRASPING))
  {
    send_to_char("Suddenly you lose your grip of the One Power!\r\n",ch);
    sprintf(tpbuf,"The %s aura around $n suddenly vanishes!",ch_mss[(int)GET_SEX(ch)]);
    act(tpbuf,FALSE,ch,NULL,NULL,TO_OPUSERSS);
    remove_char_maintained(ch);
    remove_room_maintained(ch);
    REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
  }
  do_look(ch, "", 0, 0);
  act("The air shimmmers, and suddenly $n stands there.", TRUE, ch, 0, 0, TO_ROOM);
}
