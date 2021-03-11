/* ************************************************************************
*   File: act.item.c                                    Part of CircleMUD *
*  Usage: object handling routines -- get/drop and container handling     *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "config.h"
#include "conf.h"
#include "sysdep.h"
#include "constants.h"
#include "act.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "spells.h"
#include "craft.h"
#include "guild.h"
#include "dg_scripts.h"

void perform_put(struct char_data * ch, struct obj_data * obj,struct obj_data * cont)
{
  int counter = 1;
  struct obj_data *i = NULL, *list = world[ch->in_room].contents;

  if (!put_otrigger(obj, ch, cont))
   return;

  if (GET_OBJ_TYPE(cont) == ITEM_FURNITURE) {
    if (GET_OBJ_VAL(cont, 0) != 3) {
      send_to_char("You can't put things there !\r\n", ch);
      return;
   }
   
    for (i = list; i; i = i->next_content) {
      if (i->on_obj == cont) {
	counter++;
      }
    }

    if (GET_OBJ_VAL(cont, 2) < counter) {
      send_to_char("No room !\r\n", ch);
      return;
    }
    obj_from_char(obj);
    obj_to_room(obj, ch->in_room);
    obj->on_obj = cont;
   
    sprintf(buf, "You put $p %s $P.", furniture_usages[GET_OBJ_VAL(cont, 1)]);
    act(buf, FALSE, ch, obj, cont, TO_CHAR);
    sprintf(buf, "$n puts $p %s $P.", furniture_usages[GET_OBJ_VAL(cont, 1)]);
    act(buf, FALSE, ch, obj, cont, TO_ROOM);
    return;
  }

  /* If we got here , it means that this item is a container */

  if (GET_OBJ_WEIGHT(cont) + GET_OBJ_WEIGHT(obj) > GET_OBJ_VAL(cont, 0)) {
    act("$p won't fit in $P.", FALSE, ch, obj, cont, TO_CHAR);
  } else {
    obj_from_char(obj);
    obj_to_obj(obj, cont);
    act("You put $p in $P.", FALSE, ch, obj, cont, TO_CHAR);
    act("$n puts $p in $P.", TRUE, ch, obj, cont, TO_ROOM);
  }
}


/* The following put modes are supported by the code below:

    1) put <object> <container>
    2) put all.<object> <container>
    3) put all <container>

    <container> must be worn, carried or in room.
    all objects to be put into container must be in inventory.
*/

int put_obj_on_obj(struct obj_data *to_put,struct obj_data *put_on)
{
  int counter = 1;
  struct obj_data *i = NULL, *list = world[put_on->in_room].contents;

  if ((GET_OBJ_TYPE(put_on) != ITEM_CONTAINER) && (GET_OBJ_TYPE(put_on) != ITEM_FURNITURE))
    return 0;
  if (GET_OBJ_VAL(put_on, 0) != 3) 
    return 0;

  for (i = list; i; i = i->next_content)
     if (i->on_obj == put_on)
      counter++;

  if (GET_OBJ_VAL(put_on, 2) < counter) 
    return 0;

  if (to_put->in_room == -1 || to_put->in_room != put_on->in_room)
    obj_to_room(to_put,put_on->in_room);
  to_put->on_obj = put_on;

  return 1;
}

ACMD(do_put)
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  struct obj_data *obj, *next_obj, *cont;
  int obj_dotmode, cont_dotmode, found = 0;

  if (ROOM_FLAGGED(ch->in_room, ROOM_NOSPAM) && (!IS_NPC(ch)) && (GET_LEVEL(ch) < LVL_GRGOD))
  {
    send_to_char("Sorry, you can't do that here.\r\n", ch);
    return;
  }

  if (!argument || !*argument) {
    send_to_char("Put what and where ?\r\n", ch);
    return;
  }

  two_arguments(argument, arg1, arg2);
  obj_dotmode = find_all_dots(arg1);
  cont_dotmode = find_all_dots(arg2);

  if (!*arg1)
    send_to_char("Put what where ?\r\n", ch);
  else if (cont_dotmode != FIND_INDIV)
    send_to_char("You can only put things in one place at a time.\r\n", ch);
  else if (!*arg2) {
    sprintf(buf, "Where do you want to put %s ?\r\n",
        ((obj_dotmode == FIND_INDIV) ? "it" : "them"));
    send_to_char(buf, ch);
  } else {

/*    generic_find(arg2, (1 << FIND_OBJ_INV) | (1 << FIND_OBJ_ROOM) | (1 << FIND_OBJ_EQUIP), ch, &tmp_char, &cont); */
    if (!(cont = get_obj_in_list_vis(ch,arg2,world[ch->in_room].contents)) && !(cont = get_obj_in_list_vis(ch,arg2,ch->carrying)) && !(cont = get_obj_worn_vis(ch,arg2)))
 /*   if (!cont) */ {
      sprintf(buf, "You don't see %s %s here.\r\n", AN(arg2), arg2);
      send_to_char(buf, ch);
    } else if ((GET_OBJ_TYPE(cont) != ITEM_CONTAINER) && (GET_OBJ_TYPE(cont) != ITEM_FURNITURE))
      act("$p is not furniture nor a container.", FALSE, ch, cont, 0, TO_CHAR);
    else if ((IS_SET(GET_OBJ_VAL(cont, 1), CONT_CLOSED)) && (GET_OBJ_TYPE(cont) == ITEM_CONTAINER))
      send_to_char("You'd better open it first!\r\n", ch);
    else {
      if (obj_dotmode == FIND_INDIV) {	/* put <obj> <container> */
    if (!(obj = get_obj_in_list_vis(ch, arg1, ch->carrying))) {
      sprintf(buf, "You aren't carrying %s %s.\r\n", AN(arg1), arg1);
      send_to_char(buf, ch);
    } else if (obj == cont)
      send_to_char("You attempt to fold it into itself, but fail.\r\n", ch);
    else
      perform_put(ch, obj, cont);
      } else {
    for (obj = ch->carrying; obj; obj = next_obj) {
      next_obj = obj->next_content;
      if (obj != cont && CAN_SEE_OBJ(ch, obj) &&
          (obj_dotmode == FIND_ALL || isname(arg1, obj->name))) {
        found = 1;
        perform_put(ch, obj, cont);
      }
    }
    if (!found) {
      if (obj_dotmode == FIND_ALL)
        send_to_char("You don't seem to have anything to put there.\r\n", ch);
      else {
        sprintf(buf, "You don't seem to have any %ss.\r\n", arg1);
        send_to_char(buf, ch);
      }
    }
      }
    }
  }
}



int can_take_obj(struct char_data * ch, struct obj_data * obj)
{
  if (IS_CARRYING_N(ch) >= CAN_CARRY_N(ch)) {
    act("$p: you can't carry that many items.", FALSE, ch, obj, 0, TO_CHAR);
    return 0;
  } else if ((IS_CARRYING_W(ch) + GET_OBJ_WEIGHT(obj)) > CAN_CARRY_W(ch) && GET_LEVEL(ch) < 210) {
    act("$p: you can't carry that much weight.", FALSE, ch, obj, 0, TO_CHAR);
    return 0;
  } else if (!(CAN_WEAR(obj, ITEM_WEAR_TAKE))) {
    act("$p: you can't take that!", FALSE, ch, obj, 0, TO_CHAR);
    return 0;
  }
  return 1;
}


void get_check_money(struct char_data * ch, struct obj_data * obj)
{
  if ((GET_OBJ_TYPE(obj) == ITEM_MONEY) && (GET_OBJ_VAL(obj, 0) > 0)) {
    obj_from_char(obj);
    if (GET_OBJ_VAL(obj, 0) > 1) {
      sprintf(buf, "There were %ld coins.\r\n", GET_OBJ_VAL(obj, 0));
      send_to_char(buf, ch);
    }
    GET_GOLD(ch) += GET_OBJ_VAL(obj, 0);
    extract_obj(obj);
  }
}


void perform_get_from_container(struct char_data * ch, struct obj_data * obj,
                     struct obj_data * cont, int mode)
{
  if (mode == FIND_OBJ_INV || can_take_obj(ch, obj)) {
    if (IS_CARRYING_N(ch) >= CAN_CARRY_N(ch))
      act("$p: you can't hold any more items.", FALSE, ch, obj, 0, TO_CHAR);
    else if (get_otrigger(obj, ch)) {
      obj_from_obj(obj);
      obj_to_char(obj, ch);
      act("You get $p from $P.", FALSE, ch, obj, cont, TO_CHAR);
      act("$n gets $p from $P.", TRUE, ch, obj, cont, TO_ROOM);
      get_check_money(ch, obj);
    }
  }
}


void get_from_container(struct char_data * ch, struct obj_data * cont,
                char *arg, int mode)
{
  struct obj_data *obj, *next_obj;
  int obj_dotmode, found = 0;

  obj_dotmode = find_all_dots(arg);

  if (IS_SET(GET_OBJ_VAL(cont, 1), CONT_CLOSED))
    act("$p is closed.", FALSE, ch, cont, 0, TO_CHAR);
  else if (obj_dotmode == FIND_INDIV) {
    if (!(obj = get_obj_in_list_vis(ch, arg, cont->contains))) {
      sprintf(buf, "There doesn't seem to be %s %s in $p.", AN(arg), arg);
      act(buf, FALSE, ch, cont, 0, TO_CHAR);
    } else
      perform_get_from_container(ch, obj, cont, mode);
  } else {
    if (obj_dotmode == FIND_ALLDOT && !*arg) {
      send_to_char("Get all of what?\r\n", ch);
      return;
    }
    for (obj = cont->contains; obj; obj = next_obj) {
      next_obj = obj->next_content;
      if (CAN_SEE_OBJ(ch, obj) &&
      (obj_dotmode == FIND_ALL || isname(arg, obj->name))) {
    found = 1;
    perform_get_from_container(ch, obj, cont, mode);
      }
    }
    if (!found) {
      if (obj_dotmode == FIND_ALL)
    act("$p seems to be empty.", FALSE, ch, cont, 0, TO_CHAR);
      else {
    sprintf(buf, "You can't seem to find any %ss in $p.", arg);
    act(buf, FALSE, ch, cont, 0, TO_CHAR);
      }
    }
  }
}


int perform_get_from_room(struct char_data * ch, struct obj_data * obj)
{
  int oprog;

  if (can_take_obj(ch, obj) && get_otrigger(obj, ch)) {
    oprog = check_for_objprogs_and_stops(ch, obj, NULL, world[ch->in_room].contents, OPC_TAKE,0);
    if (oprog != 2) {

     if (obj->on_obj) {
      act("You get $p from $P.", FALSE, ch, obj, obj->on_obj, TO_CHAR);
      act("$n gets $p from $P.", TRUE, ch, obj, obj->on_obj, TO_ROOM);
      obj_from_room(obj);
      obj_to_char(obj, ch);
     } 
     else {
      obj_from_room(obj);  
      obj_to_char(obj, ch);
      act("You get $p.", FALSE, ch, obj, 0, TO_CHAR);
      act("$n gets $p.", TRUE, ch, obj, 0, TO_ROOM);
     }

      get_check_money(ch, obj);
      if (oprog)
        run_objprog(ch, obj);
      return 1;
    }
    if (oprog)
      run_objprog(ch, obj);
  } 
  return 0;
}


void get_from_room(struct char_data * ch, char *arg)
{
  struct obj_data *obj, *next_obj;
  int dotmode, found = 0;

  dotmode = find_all_dots(arg);

  if (dotmode == FIND_INDIV) {
    if (!(obj = get_obj_in_list_vis(ch, arg, world[ch->in_room].contents))) {
      sprintf(buf, "You don't see %s %s here.\r\n", AN(arg), arg);
      send_to_char(buf, ch);
      return;
    } 
    if (obj->on_obj) {
      send_to_char("Where do you want to get it from ? Use 'get <object> <place>'.\r\n", ch);
      return;
    }
    perform_get_from_room(ch, obj);
  } else {
    if (dotmode == FIND_ALLDOT && !*arg) {
      send_to_char("Get all of what?\r\n", ch);
      return;
    }
    for (obj = world[ch->in_room].contents; obj; obj = next_obj) {
      next_obj = obj->next_content;
      if (CAN_SEE_OBJ(ch, obj) &&
      (dotmode == FIND_ALL || isname(arg, obj->name)) && !(obj->on_obj)) {
    found = 1;
    perform_get_from_room(ch, obj);
      }
    }
    if (!found) {
      if (dotmode == FIND_ALL)
    send_to_char("There doesn't seem to be anything here.\r\n", ch);
      else {
    sprintf(buf, "You don't see any %ss here.\r\n", arg);
    send_to_char(buf, ch);
      }
    }
  }
}

void get_from_furniture(struct char_data * ch, char *arg) 
{
  struct obj_data *obj, *next_obj;
  int dotmode, found = 0;
      
  dotmode = find_all_dots(arg);
        
  if (dotmode == FIND_INDIV) {
    if (!(obj = get_obj_in_list_vis(ch, arg, world[ch->in_room].contents))) {
      sprintf(buf, "You don't see %s %s here.\r\n", AN(arg), arg);
      send_to_char(buf, ch);
      return;
    }
    perform_get_from_room(ch, obj);
  } else {
    if (dotmode == FIND_ALLDOT && !*arg) {
      send_to_char("Get all of what?\r\n", ch);
      return;
    }
    for (obj = world[ch->in_room].contents; obj; obj = next_obj) {
      next_obj = obj->next_content;
      if (CAN_SEE_OBJ(ch, obj) &&
          (dotmode == FIND_ALL || isname(arg, obj->name))) {
        found = 1;
        perform_get_from_room(ch, obj);
      }
    }
    if (!found) {
      if (dotmode == FIND_ALL)
        send_to_char("There doesn't seem to be anything here.\r\n", ch);
      else {
        sprintf(buf, "You don't see any %ss here.\r\n", arg);
        send_to_char(buf, ch);
      }
    }
  }
}

ACMD(do_get)
{
  char arg1[255], arg2[255], *idiot_ptr = arg2;
  int cont_dotmode, found = 0, mode;
  struct obj_data *cont;
  struct char_data *tmp_char;

  if (ROOM_FLAGGED(ch->in_room, ROOM_NOSPAM) && (!IS_NPC(ch)) && (GET_LEVEL(ch) < LVL_GRGOD))
  {
    send_to_char("Sorry, you can't do that here.\r\n", ch);
    return;
  }

  if (!argument || !*argument) {
    send_to_char("Get what?\r\n", ch);
    return;
  }
  two_arguments(argument, arg1, arg2);

  if (IS_CARRYING_N(ch) >= CAN_CARRY_N(ch))
    send_to_char("Your arms are already full!\r\n", ch);
  else if (!*arg1)
    send_to_char("Get what?\r\n", ch);
  else if (!*arg2)
    get_from_room(ch, arg1);
  else {
    cont_dotmode = find_all_dots(arg2);
    if (cont_dotmode == FIND_INDIV) {
      mode = generic_find(idiot_ptr, FIND_OBJ_INV | 
      FIND_OBJ_ROOM | FIND_OBJ_EQUIP, ch, &tmp_char, &cont);
      if (!cont) {
    sprintf(buf, "You don't have %s %s.\r\n", AN(arg2), arg2);
    send_to_char(buf, ch);
      } else if ((GET_OBJ_TYPE(cont) != ITEM_CONTAINER) && (GET_OBJ_TYPE(cont) != ITEM_FURNITURE))
    act("$p is not furniture nor a container.", FALSE, ch, cont, 0, TO_CHAR);
      else {
    if (GET_OBJ_TYPE(cont) == ITEM_CONTAINER) get_from_container(ch, cont, arg1, mode);
        if (GET_OBJ_TYPE(cont) == ITEM_FURNITURE) get_from_furniture(ch, arg1);
      }
    } else {
      if (cont_dotmode == FIND_ALLDOT && !*arg2) {
    send_to_char("Get from all of what?\r\n", ch);
    return;
      }
      for (cont = ch->carrying; cont; cont = cont->next_content)
    if (CAN_SEE_OBJ(ch, cont) &&
        (cont_dotmode == FIND_ALL || isname(arg2, cont->name))) {
      if (GET_OBJ_TYPE(cont) == ITEM_CONTAINER) {
        found = 1;
        get_from_container(ch, cont, arg1, FIND_OBJ_INV);
      } else if (cont_dotmode == FIND_ALLDOT) {
        found = 1;
        act("$p is not a container.", FALSE, ch, cont, 0, TO_CHAR);
      }
        }

      for (cont = world[ch->in_room].contents; cont; cont = cont->next_content)
    if (CAN_SEE_OBJ(ch, cont) &&
        (cont_dotmode == FIND_ALL || isname(arg2, cont->name))) {
      if (GET_OBJ_TYPE(cont) == ITEM_CONTAINER) {
        get_from_container(ch, cont, arg1, FIND_OBJ_ROOM);
        found = 1;
      } else if (cont_dotmode == FIND_ALLDOT) {
        act("$p is not a container.", FALSE, ch, cont, 0, TO_CHAR);
        found = 1;
      }
        }
      if (!found) {
    if (cont_dotmode == FIND_ALL)
      send_to_char("You can't seem to find any containers.\r\n", ch);
    else {
      sprintf(buf, "You can't seem to find any %ss here.\r\n", arg2);
      send_to_char(buf, ch);
    }
      }
    }
  }
}


void perform_drop_gold(struct char_data * ch, int amount,
                  byte mode, sh_int RDR)
{
  struct obj_data *obj;

  if (amount <= 0) {
    send_to_char("Heh heh heh.. we are jolly funny today, eh?\r\n", ch);
  }
  else if (GET_GOLD(ch) < amount) {
    send_to_char("You don't have that many coins!\r\n", ch);
  }
  else {
    if (mode != SCMD_JUNK) {
      WAIT_STATE(ch, PULSE_VIOLENCE);	/* to prevent coin-bombing */
      obj = create_money(amount);
      if (mode == SCMD_DONATE) {
    send_to_char("You throw some gold into the air where it disappears in a puff of smoke!\r\n", ch);
    act("$n throws some gold into the air where it disappears in a puff of smoke!",
        FALSE, ch, 0, 0, TO_ROOM);
    obj_to_room(obj, RDR);
    act("$p suddenly appears in a puff of orange smoke!", 0, 0, obj, 0, TO_ROOM);
      } else {
        if (!drop_wtrigger(obj, ch)) {
      extract_obj(obj);
      return;
        }
    send_to_char("You drop some gold.\r\n", ch);
    sprintf(buf, "$n drops %s.", money_desc(amount));
    act(buf, FALSE, ch, 0, 0, TO_ROOM);
    obj_to_room(obj, ch->in_room);
      }
    } else {
      sprintf(buf, "$n drops %s which disappears in a puff of smoke!",
          money_desc(amount));
      act(buf, FALSE, ch, 0, 0, TO_ROOM);
      send_to_char("You drop some gold which disappears in a puff of smoke!\r\n", ch);
    }
    GET_GOLD(ch) -= amount;
  }
}


#define VANISH(mode) ((mode == SCMD_DONATE || mode == SCMD_JUNK) ? \
            "  It vanishes in a puff of smoke!" : "")

int perform_drop(struct char_data * ch, struct obj_data * obj,
             byte mode, char *sname, sh_int RDR)
{
  int value = 0, oprog = 0;

  if (!drop_otrigger(obj, ch))
   return 0;
  if ((mode == SCMD_DROP) && !drop_wtrigger(obj, ch))
   return 0;

  if (IS_OBJ_STAT(obj, ITEM_NODROP)) {
    sprintf(buf, "You can't %s $p, it must be CURSED!", sname);
    act(buf, FALSE, ch, obj, 0, TO_CHAR);
    return 0;
  }
  if (mode == SCMD_DONATE)
    oprog = check_for_objprogs_and_stops(ch, obj, NULL, ch->carrying, OPC_DONATE, 0);
  else if (mode == SCMD_JUNK)
    oprog = check_for_objprogs_and_stops(ch, obj, NULL, ch->carrying, OPC_JUNK, 0);
  else if (mode == SCMD_DROP)
    oprog = check_for_objprogs_and_stops(ch, obj, NULL, ch->carrying, OPC_DROP, 0);
  if (oprog)
    run_objprog(ch, obj);

  if (oprog != 2) {
    sprintf(buf, "You %s $p.%s", sname, VANISH(mode));
    act(buf, FALSE, ch, obj, 0, TO_CHAR);
    sprintf(buf, "$n %ss $p.%s", sname, VANISH(mode));
    act(buf, TRUE, ch, obj, 0, TO_ROOM);
    obj_from_char(obj);

    if ((mode == SCMD_DONATE) && IS_OBJ_STAT(obj, ITEM_NODONATE))
      mode = SCMD_JUNK;

    switch (mode) {
    case SCMD_DROP:
      obj_to_room(obj, ch->in_room);
    break;
    case SCMD_DONATE:
      obj_to_room(obj, RDR);
      sprintf(buf, "%s suddenly appears in a puff of smoke!\r\n", obj->short_description);
      send_to_room(buf, obj->in_room);
      break;
    case SCMD_JUNK:
      value = MAX(1, MIN(200, GET_OBJ_COST(obj) >> 4));
      extract_obj(obj);
      break;
    default:
      mlog("SYSERR: Incorrect argument passed to perform_drop");
      break;
    }
  }
  return value;
}



ACMD(do_drop)
{
  struct obj_data *obj, *next_obj;
  sh_int RDR = 0;
  byte mode = SCMD_DROP;
  int dotmode, amount = 0;
  char *sname;

  switch (subcmd) {
  case SCMD_JUNK:
    sname = "junk";
    mode = SCMD_JUNK;
    break;
  case SCMD_DONATE:
    sname = "donate";
    mode = SCMD_DONATE;
    switch (number(0, 2)) {
    case 0:
    case 1:
    case 2:
      if (world[ch->in_room].number/1000 == 9) /*Stuff donated on the Seanchan */
        RDR = real_room(9615);                 /* continent ends up in Seandar */
      else
        RDR = real_room(donation_room_1);
      break;
/*    case 3: RDR = real_room(donation_room_2); break;
      case 4: RDR = real_room(donation_room_3); break;
*/
    }
    if (RDR == NOWHERE) {
      send_to_char("Sorry, you can't donate anything right now.\r\n", ch);
      return;
    }
    break;
  default:
    sname = "drop";
    break;
  }

  argument = one_argument(argument, arg);

  if (ROOM_FLAGGED(ch->in_room, ROOM_NOSPAM) && (!IS_NPC(ch)) && (GET_LEVEL(ch) < LVL_GRGOD))
  {
    send_to_char("Sorry, you can't do that here.\r\n", ch);
    return;
  }

  if (!*arg) {
    sprintf(buf, "What do you want to %s?\r\n", sname);
    send_to_char(buf, ch);
    return;
  } else if (is_number(arg)) {
    amount = atoi(arg);
    argument = one_argument(argument, arg);
    if (!str_cmp("coins", arg) || !str_cmp("coin", arg)) {
      perform_drop_gold(ch, amount, mode, RDR);
      return;
    }
    else {
      int cnt = 0;
      /* 
       * Ugly attempt at multi-droip code (i.e drop 5 sword to drop the 5 first item
       *in inventory having keyword sword). Stefan, 2002-01-21
       */
      if (amount < 0) {
    send_to_char("Try dropping a \\c01POSITIVE\\c00 amount of that object, eh?\r\n",ch);
    return;
      }

      do {
    if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying))) {
      sprintf(buf, "You don't seem to have %s %s.\r\n", AN(arg), arg);
      send_to_char(buf, ch);
      return;
    }
    cnt += perform_drop(ch, obj, mode, sname, RDR);
    amount--;
      } while (amount);
      amount = cnt;
    }
  } else {
    dotmode = find_all_dots(arg);

    /* Can't junk or donate all */
    if ((dotmode == FIND_ALL) && (subcmd == SCMD_JUNK || subcmd == SCMD_DONATE)) {
      if (subcmd == SCMD_JUNK)
    send_to_char("Go to the dump if you want to junk EVERYTHING!\r\n", ch);
      else
    send_to_char("Go do the donation room if you want to donate EVERYTHING!\r\n", ch);
      return;
    }
    if (dotmode == FIND_ALL) {
      if (!ch->carrying)
    send_to_char("You don't seem to be carrying anything.\r\n", ch);
      else
    for (obj = ch->carrying; obj; obj = next_obj) {
      next_obj = obj->next_content;
      amount += perform_drop(ch, obj, mode, sname, RDR);
    }
    } else if (dotmode == FIND_ALLDOT) {
      if (!*arg) {
    sprintf(buf, "What do you want to %s all of?\r\n", sname);
    send_to_char(buf, ch);
    return;
      }
      if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying))) {
    sprintf(buf, "You don't seem to have any %ss.\r\n", arg);
    send_to_char(buf, ch);
      }
      while (obj) {
    next_obj = get_obj_in_list_vis(ch, arg, obj->next_content);
    amount += perform_drop(ch, obj, mode, sname, RDR);
    obj = next_obj;
      }
    } else {
      if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying))) {
    sprintf(buf, "You don't seem to have %s %s.\r\n", AN(arg), arg);
    send_to_char(buf, ch);
      } else
    amount += perform_drop(ch, obj, mode, sname, RDR);
    }
  }

  if (amount && (subcmd == SCMD_JUNK)) {
    send_to_char("You have been rewarded by the gods!\r\n", ch);
    act("$n has been rewarded by the gods!", TRUE, ch, 0, 0, TO_ROOM);
    GET_GOLD(ch) += amount;
  }
}


void perform_give(struct char_data * ch, struct char_data * vict,
             struct obj_data * obj)
{
  int mprog;

  if ( IS_OBJ_STAT(obj, ITEM_NODROP) && GET_LEVEL(ch)<LVL_IMMORT ) 
    {
    act("You can't let go of $p!!  Yeech!", FALSE, ch, obj, 0, TO_CHAR);
    return;
    }
  if (IS_CARRYING_N(vict) >= CAN_CARRY_N(vict)) {
    act("$N seems to have $S hands full.", FALSE, ch, 0, vict, TO_CHAR);
    return;
  }
  if (GET_OBJ_WEIGHT(obj) + IS_CARRYING_W(vict) > CAN_CARRY_W(vict)) {
    act("$E can't carry that much weight.", FALSE, ch, 0, vict, TO_CHAR);
    return;
  }

  if (!give_otrigger(obj, ch, vict) || !receive_mtrigger(vict, ch, obj))
   return;

  mprog = check_for_mobprogs_and_stops(ch, vict, 
         NULL, MPC_GIVE, 0, GET_OBJ_VNUM(obj));
  if (mprog != 2 ) {
    obj_from_char(obj);
    obj_to_char(obj, vict);
    act("You give $p to $N.", FALSE, ch, obj, vict, TO_CHAR);
    act("$n gives you $p.", FALSE, ch, obj, vict, TO_VICT);
    act("$n gives $p to $N.", TRUE, ch, obj, vict, TO_NOTVICT);
  }
  if (mprog)
    run_mobprog(ch, 0);
}

/* utility function for give */
struct char_data *give_find_vict(struct char_data * ch, char *arg)
{
  struct char_data *vict;

  if (!*arg) {
    send_to_char("To who?\r\n", ch);
    return NULL;
  } else if (!(vict = get_char_room_vis(ch, arg))) {
    send_to_char(NOPERSON, ch);
    return NULL;
  } else if (vict == ch) {
    send_to_char("What's the point of that?\r\n", ch);
    return NULL;
  } else
    return vict;
}


void perform_give_gold(struct char_data * ch, struct char_data * vict,
                  int amount)
{
  if (amount <= 0) {
    send_to_char("Heh heh heh ... we are jolly funny today, eh?\r\n", ch);
    return;
  }
  if ((GET_GOLD(ch) < amount) && (IS_NPC(ch) || (GET_LEVEL(ch) < LVL_GOD))) {
    send_to_char("You don't have that many coins!\r\n", ch);
    return;
  }
  send_to_char(OK, ch);
  sprintf(buf, "$n gives you %d gold coins.", amount);
  act(buf, FALSE, ch, 0, vict, TO_VICT);
  sprintf(buf, "$n gives %s to $N.", money_desc(amount));
  act(buf, TRUE, ch, 0, vict, TO_NOTVICT);
  if (IS_NPC(ch) || (GET_LEVEL(ch) < LVL_GOD))
    GET_GOLD(ch) -= amount;
  GET_GOLD(vict) += amount;

  bribe_mtrigger(vict, ch, amount);
}


ACMD(do_give)
{
  int amount, dotmode;
  struct char_data *vict;
  struct obj_data *obj, *next_obj;

  argument = one_argument(argument, arg);

  if (!*arg)
    send_to_char("Give what to who?\r\n", ch);
  else if (is_number(arg)) {
    amount = atoi(arg);
    argument = one_argument(argument, arg);
    if (!str_cmp("coins", arg) || !str_cmp("coin", arg)) {
      argument = one_argument(argument, arg);
      if ((vict = give_find_vict(ch, arg)))
    perform_give_gold(ch, vict, amount);
      return;
    } else {
      if (amount < 0) {
    send_to_char("Heh heh heh ... we are jolly funny today, eh?\r\n", ch);
    return;
      }
      /*
       * Ugly attempt at multi-give code (i.e drop 5 sword to drop the 5 first item
       *in inventory having keyword sword). Stefan, 2002-01-21
       */
      argument = one_argument(argument, buf1);
      if (!(vict = give_find_vict(ch, buf1))) {
    return;
      }
      do {
    if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying))) {
      sprintf(buf, "You don't seem to have %s %s.\r\n", AN(arg), arg);
      send_to_char(buf, ch);
      return;
    }
    perform_give(ch, vict, obj);
    amount--;
      } while (amount);
    }
  } else {
    one_argument(argument, buf1);
    if (!(vict = give_find_vict(ch, buf1)))
      return;
    dotmode = find_all_dots(arg);
    if (dotmode == FIND_INDIV) {
      if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying))) {
    sprintf(buf, "You don't seem to have %s %s.\r\n", AN(arg), arg);
    send_to_char(buf, ch);
      } else
    perform_give(ch, vict, obj);
    } else {
      if (dotmode == FIND_ALLDOT && !*arg) {
    send_to_char("All of what?\r\n", ch);
    return;
      }
      if (!ch->carrying)
    send_to_char("You don't seem to be holding anything.\r\n", ch);
      else
    for (obj = ch->carrying; obj; obj = next_obj) {
      next_obj = obj->next_content;
      if (CAN_SEE_OBJ(ch, obj) &&
          ((dotmode == FIND_ALL || isname(arg, obj->name))))
        perform_give(ch, vict, obj);
    }
    }
  }
}



void weight_change_object(struct obj_data * obj, int weight)
{
  struct obj_data *tmp_obj;
  struct char_data *tmp_ch;

  if (obj->in_room != NOWHERE) {
    GET_OBJ_WEIGHT(obj) += weight;
  } else if ((tmp_ch = obj->carried_by)) {
    obj_from_char(obj);
    GET_OBJ_WEIGHT(obj) += weight;
    obj_to_char(obj, tmp_ch);
  } else if ((tmp_obj = obj->in_obj)) {
    obj_from_obj(obj);
    GET_OBJ_WEIGHT(obj) += weight;
    obj_to_obj(obj, tmp_obj);
  } else {
    mlog("SYSERR: Unknown attempt to subtract weight from an object.");
  }
}



void name_from_drinkcon(struct obj_data * obj)
{
  int i;
  char *new_name;

  for (i = 0; (*((obj->name) + i) != ' ') && (*((obj->name) + i) != '\0'); i++);

  if (*((obj->name) + i) == ' ') {
    new_name = str_dup((obj->name) + i + 1);
    if (GET_OBJ_RNUM(obj) < 0 || obj->name != obj_proto[GET_OBJ_RNUM(obj)].name)
      free(obj->name);
    obj->name = new_name;
  }
}



void name_to_drinkcon(struct obj_data * obj, int type)
{
  char *new_name;

  CREATE(new_name, char, strlen(obj->name) + strlen(drinknames[type]) + 2);
  sprintf(new_name, "%s %s", drinknames[type], obj->name);
  if (GET_OBJ_RNUM(obj) < 0 || obj->name != obj_proto[GET_OBJ_RNUM(obj)].name)
    free(obj->name);
  obj->name = new_name;
}



ACMD(do_drink)
{
  struct obj_data *temp;
  int amount, weight, endu;
  int on_ground = 0;

  one_argument(argument, arg);

  if (!*arg) {
    send_to_char("Drink from what?\r\n", ch);
    return;
  }
  if (!(temp = get_obj_in_list_vis(ch, arg, ch->carrying))) {
    if (!(temp = get_obj_in_list_vis(ch, arg, world[ch->in_room].contents))) {
      act("You can't find it!", FALSE, ch, 0, 0, TO_CHAR);
      return;
    } else
      on_ground = 1;
  }
  if ((GET_OBJ_TYPE(temp) != ITEM_DRINKCON) &&
      (GET_OBJ_TYPE(temp) != ITEM_FOUNTAIN)) {
    send_to_char("You can't drink from that!\r\n", ch);
    return;
  }
  if (on_ground && (GET_OBJ_TYPE(temp) == ITEM_DRINKCON)) {
    send_to_char("You have to be holding that to drink from it.\r\n", ch);
    return;
  }
  if ((GET_COND(ch, DRUNK) > 10) && (GET_COND(ch, THIRST) > 0)) {
    /* The pig is drunk */
    send_to_char("You can't seem to get close enough to your mouth.\r\n", ch);
    act("$n tries to drink but misses $s mouth!", TRUE, ch, 0, 0, TO_ROOM);
    return;
  }
  if ((GET_COND(ch, FULL) > 20) && (GET_COND(ch, THIRST) > 0)) {
    send_to_char("Your stomach can't contain anymore!\r\n", ch);
    return;
  }
  if (GET_COND(ch, THIRST) > 20) {
    send_to_char("You don't feel thirsty at all.\r\n", ch);
    return;
  }
  if (!GET_OBJ_VAL(temp, 1)) {
    send_to_char("It's empty.\r\n", ch);
    return;
  }
  if (subcmd == SCMD_DRINK) {
    sprintf(buf, "$n drinks %s from $p.", drinks[GET_OBJ_VAL(temp, 2)]);
    act(buf, TRUE, ch, temp, 0, TO_ROOM);

    sprintf(buf, "You drink the %s.\r\n", drinks[GET_OBJ_VAL(temp, 2)]);
    send_to_char(buf, ch);

    if (drink_aff[GET_OBJ_VAL(temp, 2)][DRUNK] > 0)
      amount = (25 - GET_COND(ch, THIRST)) / drink_aff[GET_OBJ_VAL(temp, 2)][DRUNK];
    else
      amount = 25 - GET_COND(ch, THIRST);

  } else {
    act("$n sips from $p.", TRUE, ch, temp, 0, TO_ROOM);
    sprintf(buf, "It tastes like %s.\r\n", drinks[GET_OBJ_VAL(temp, 2)]);
    send_to_char(buf, ch);
    amount = 1;
  }

  amount = MIN(amount, GET_OBJ_VAL(temp, 1));

  /* You can't subtract more than the object weighs */
  weight = MIN(amount, GET_OBJ_WEIGHT(temp));

  weight_change_object(temp, -weight);	/* Subtract amount */
  
  if (IS_AFFECTED(ch, AFF_ENDURANCE))
    endu = amount;
  else
    endu = 0;
   
  gain_condition(ch, DRUNK,
     (int) ((int) drink_aff[GET_OBJ_VAL(temp, 2)][DRUNK] * (amount + endu)) / 4);

  gain_condition(ch, FULL,
      (int) ((int) drink_aff[GET_OBJ_VAL(temp, 2)][FULL] * (amount + endu)) / 4);

  gain_condition(ch, THIRST,
    (int) ((int) drink_aff[GET_OBJ_VAL(temp, 2)][THIRST] * (amount + endu)) / 4);

  if (GET_COND(ch, DRUNK) > 10)
    send_to_char("You feel drunk.\r\n", ch);

  if (GET_COND(ch, THIRST) > 20)
    send_to_char("You don't feel thirsty any more.\r\n", ch);

  if (GET_COND(ch, FULL) > 20)
    send_to_char("You are full.\r\n", ch);

  /* empty the container, and no longer poison. */
  GET_OBJ_VAL(temp, 1) -= amount;
  if (!GET_OBJ_VAL(temp, 1)) {	/* The last bit */
    GET_OBJ_VAL(temp, 2) = 0;
    GET_OBJ_VAL(temp, 3) = 0;
    name_from_drinkcon(temp);
  }
  return;
}



ACMD(do_eat)
{
  struct obj_data *food;
  int amount;
  sh_int *now = NULL, max = 0, healamount = 0;
  byte percentual = FALSE;

  one_argument(argument, arg);

  if (!*arg) {
    send_to_char("Eat what?\r\n", ch);
    return;
  }
  if (!(food = get_obj_in_list_vis(ch, arg, ch->carrying))) {
    sprintf(buf, "You don't seem to have %s %s.\r\n", AN(arg), arg);
    send_to_char(buf, ch);
    return;
  }
  if (subcmd == SCMD_TASTE && ((GET_OBJ_TYPE(food) == ITEM_DRINKCON) ||
                  (GET_OBJ_TYPE(food) == ITEM_FOUNTAIN))) {
    do_drink(ch, argument, 0, SCMD_SIP);
    return;
  }

  if (GET_OBJ_TYPE(food) == ITEM_HEALING) {
    switch (GET_OBJ_VAL(food,0)) {
      case 0:
        max = GET_MAX_HIT(ch);
        now = &GET_HIT(ch);
      break;
      case 1:
        max = ch->points.max_mana;
        now = &ch->points.mana; 
      break;
      case 2:
        max = GET_MAX_MOVE(ch);
        now = &GET_MOVE(ch);
      break;
      default:
        alog("SYSERR: Wrong type of healing in obj #%d.", GET_OBJ_VNUM(food));
      break;
    }

    act("You eat $p.", TRUE, ch, food, 0, TO_CHAR);
    act("$n eats $p.", TRUE, ch, food, 0, TO_ROOM);

    if (max > *now) {   /* If the healing type is correct and person needs healing */   
      percentual = GET_OBJ_VAL(food, 1);
      healamount = (percentual)? max*GET_OBJ_VAL(food, 2)/100: GET_OBJ_VAL(food, 2);
      *now = MIN(max, *now + healamount);
      send_to_char("You feel better!\r\n", ch);
    }
    extract_obj(food);
    return;
  }

  if ((GET_OBJ_TYPE(food) != ITEM_FOOD) && (GET_LEVEL(ch) < LVL_CARPENTER)) {
    send_to_char("You can't eat THAT!\r\n", ch);
    return;
  }
  if (GET_COND(ch, FULL) > 20 && GET_LEVEL(ch)<LVL_CARPENTER) {/* Stomach full */
    act("You are too full to eat more!", FALSE, ch, 0, 0, TO_CHAR);
    return;
  }
  if (subcmd == SCMD_EAT) {
    act("You eat the $o.", FALSE, ch, food, 0, TO_CHAR);
    act("$n eats $p.", TRUE, ch, food, 0, TO_ROOM);
  } else {
    act("You nibble a little bit of the $o.", FALSE, ch, food, 0, TO_CHAR);
    act("$n tastes a little bit of $p.", TRUE, ch, food, 0, TO_ROOM);
  }

  amount = (subcmd == SCMD_EAT ? GET_OBJ_VAL(food, 0) : 1);
  if (IS_AFFECTED(ch, AFF_ENDURANCE))
    amount *= 2;
  gain_condition(ch, FULL, amount);

  if (GET_COND(ch, FULL) > 20)
    act("You are full.", FALSE, ch, 0, 0, TO_CHAR);

  if (subcmd == SCMD_EAT)
    extract_obj(food);
  else {
    if (!(--GET_OBJ_VAL(food, 0))) {
      send_to_char("There's nothing left now.\r\n", ch);
      extract_obj(food);
    }
  }
}


ACMD(do_pour)
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  struct obj_data *from_obj = NULL, *to_obj = NULL;
	  int amount;

	  two_arguments(argument, arg1, arg2);

	  if (subcmd == SCMD_POUR) {
	    if (!*arg1) {		/* No arguments */
	      act("From what do you want to pour?", FALSE, ch, 0, 0, TO_CHAR);
	      return;
	    }
	    if (!(from_obj = get_obj_in_list_vis(ch, arg1, ch->carrying))) {
	      act("You can't find it!", FALSE, ch, 0, 0, TO_CHAR);
	      return;
	    }
	    if (GET_OBJ_TYPE(from_obj) != ITEM_DRINKCON) {
	      act("You can't pour from that!", FALSE, ch, 0, 0, TO_CHAR);
	      return;
	    }
	  }
	  if (subcmd == SCMD_FILL) {
	    if (!*arg1) {		/* no arguments */
	      send_to_char("What do you want to fill?  And what are you filling it from?\r\n", ch);
	      return;
	    }
	    if (!(to_obj = get_obj_in_list_vis(ch, arg1, ch->carrying))) {
	      send_to_char("You can't find it!", ch);
	      return;
	    }
	    if (GET_OBJ_TYPE(to_obj) != ITEM_DRINKCON) {
	      act("You can't fill $p!", FALSE, ch, to_obj, 0, TO_CHAR);
	      return;
	    }
	    if (!*arg2) {		/* no 2nd argument */
	      act("What do you want to fill $p from?", FALSE, ch, to_obj, 0, TO_CHAR);
	      return;
	    }
	    if (!(from_obj = get_obj_in_list_vis(ch, arg2, world[ch->in_room].contents))) {
	      sprintf(buf, "There doesn't seem to be %s %s here.\r\n", AN(arg2), arg2);
	      send_to_char(buf, ch);
	      return;
	    }
	    if (GET_OBJ_TYPE(from_obj) != ITEM_FOUNTAIN) {
	      act("You can't fill something from $p.", FALSE, ch, from_obj, 0, TO_CHAR);
	      return;
	    }
	  }
	  if (GET_OBJ_VAL(from_obj, 1) == 0) {
	    act("The $p is empty.", FALSE, ch, from_obj, 0, TO_CHAR);
	    return;
	  }
	  if (subcmd == SCMD_POUR) {	/* pour */
	    if (!*arg2) {
	      act("Where do you want it?  Out or in what?", FALSE, ch, 0, 0, TO_CHAR);
	      return;
	    }
	    if (!str_cmp(arg2, "out")) {
	      act("$n empties $p.", TRUE, ch, from_obj, 0, TO_ROOM);
	      act("You empty $p.", FALSE, ch, from_obj, 0, TO_CHAR);

	      weight_change_object(from_obj, -GET_OBJ_VAL(from_obj, 1)); /* Empty */

	      GET_OBJ_VAL(from_obj, 1) = 0;
	      GET_OBJ_VAL(from_obj, 2) = 0;
	      GET_OBJ_VAL(from_obj, 3) = 0;
	      name_from_drinkcon(from_obj);

	      return;
	    }
	    if (!(to_obj = get_obj_in_list_vis(ch, arg2, ch->carrying))) {
	      act("You can't find it!", FALSE, ch, 0, 0, TO_CHAR);
	      return;
	    }
	    if ((GET_OBJ_TYPE(to_obj) != ITEM_DRINKCON) &&
	    (GET_OBJ_TYPE(to_obj) != ITEM_FOUNTAIN)) {
	      act("You can't pour anything into that.", FALSE, ch, 0, 0, TO_CHAR);
	      return;
	    }
	  }
	  if (to_obj == from_obj) {
	    act("A most unproductive effort.", FALSE, ch, 0, 0, TO_CHAR);
	    return;
	  }
	  if ((GET_OBJ_VAL(to_obj, 1) != 0) &&
	      (GET_OBJ_VAL(to_obj, 2) != GET_OBJ_VAL(from_obj, 2))) {
	    act("There is already another liquid in it!", FALSE, ch, 0, 0, TO_CHAR);
	    return;
	  }
	  if (!(GET_OBJ_VAL(to_obj, 1) < GET_OBJ_VAL(to_obj, 0))) {
	    act("There is no room for more.", FALSE, ch, 0, 0, TO_CHAR);
	    return;
	  }
	  if (subcmd == SCMD_POUR) {
	    sprintf(buf, "You pour the %s into the %s.",
		drinks[GET_OBJ_VAL(from_obj, 2)], arg2);
	    send_to_char(buf, ch);
	  }
	  if (subcmd == SCMD_FILL) {
	    act("You gently fill $p from $P.", FALSE, ch, to_obj, from_obj, TO_CHAR);
	    act("$n gently fills $p from $P.", TRUE, ch, to_obj, from_obj, TO_ROOM);
	  }
	  /* New alias */
	  if (GET_OBJ_VAL(to_obj, 1) == 0)
	    name_to_drinkcon(to_obj, GET_OBJ_VAL(from_obj, 2));

	  /* First same type liq. */
	  GET_OBJ_VAL(to_obj, 2) = GET_OBJ_VAL(from_obj, 2);

	  /* Then how much to pour */
	  GET_OBJ_VAL(from_obj, 1) -= (amount =
             (GET_OBJ_VAL(to_obj, 0) - GET_OBJ_VAL(to_obj, 1)));

  GET_OBJ_VAL(to_obj, 1) = GET_OBJ_VAL(to_obj, 0);

  if (GET_OBJ_VAL(from_obj, 1) < 0) {	/* There was too little */
    GET_OBJ_VAL(to_obj, 1) += GET_OBJ_VAL(from_obj, 1);
    amount += GET_OBJ_VAL(from_obj, 1);
    GET_OBJ_VAL(from_obj, 1) = 0;
    GET_OBJ_VAL(from_obj, 2) = 0;
    GET_OBJ_VAL(from_obj, 3) = 0;
    name_from_drinkcon(from_obj);
  }
  /* Then the poison boogie */
  GET_OBJ_VAL(to_obj, 3) =
    (GET_OBJ_VAL(to_obj, 3) || GET_OBJ_VAL(from_obj, 3));

  /* And the weight boogie */
  weight_change_object(from_obj, -amount);
  weight_change_object(to_obj, amount);	/* Add weight */

  return;
}



void wear_message(struct char_data * ch, struct obj_data * obj, int where)
{
  char *wear_messages[][2] = {
    {"$n lights $p and holds it.",
    "You light $p and hold it."},

    {"$n slides $p on to $s right ring finger.",
    "You slide $p on to your right ring finger."},

    {"$n slides $p on to $s left ring finger.",
    "You slide $p on to your left ring finger."},

    {"$n wears $p around $s neck.",
    "You wear $p around your neck."},

    {"$n wears $p around $s neck.",
    "You wear $p around your neck."},

    {"$n wears $p on $s body.",
    "You wear $p on your body.",},

    {"$n wears $p on $s head.",
    "You wear $p on your head."},

    {"$n puts $p on $s legs.",
    "You put $p on your legs."},

    {"$n wears $p on $s feet.",
    "You wear $p on your feet."},

    {"$n puts $p on $s hands.",
    "You put $p on your hands."},

    {"$n wears $p on $s arms.",
    "You wear $p on your arms."},

    {"$n straps $p around $s arm as a shield.",
    "You start to use $p as a shield."},

    {"$n wears $p about $s body.",
    "You wear $p around your body."},

    {"$n wears $p around $s waist.",
    "You wear $p around your waist."},

    {"$n puts $p on around $s right wrist.",
    "You put $p on around your right wrist."},

    {"$n puts $p on around $s left wrist.",
    "You put $p on around your left wrist."},

    {"$n wields $p in $s right hand.",
    "You wield $p in your right hand."},

    {"$n wields $p in $s left hand.",
    "You wield $p in your left hand."},

    {"$n grabs $p.",
    "You grab $p."},

    {"$n wields $p in both $s hands.",
    "You wield $p in both your hands."} , 

    {"$n puts $p in $s right ear.",
     "You put $p in your right ear."},

    {"$n puts $p in $s left ear.",
     "You put $p in your left ear."}, 

    {"$n puts $p on $s nose.",
     "You put $p on your nose."}

  };

  act(wear_messages[where][0], TRUE, ch, obj, 0, TO_ROOM);
  act(wear_messages[where][1], FALSE, ch, obj, 0, TO_CHAR);
}



void perform_wear(struct char_data * ch, struct obj_data * obj, int where)
{
#define POS_DUALWIELD 19
  int oprog;
  /*
   * ITEM_WEAR_TAKE is used for objects that do not require special bits
   * to be put into that position (e.g. you can hold any object, not just
   * an object with a HOLD bit.)
   */

  int wear_bitvectors[] = {
    ITEM_WEAR_TAKE,  ITEM_WEAR_FINGER, ITEM_WEAR_FINGER, ITEM_WEAR_NECK,
    ITEM_WEAR_NECK,  ITEM_WEAR_BODY,   ITEM_WEAR_HEAD,   ITEM_WEAR_LEGS,
    ITEM_WEAR_FEET,  ITEM_WEAR_HANDS,  ITEM_WEAR_ARMS,   ITEM_WEAR_SHIELD,
    ITEM_WEAR_ABOUT, ITEM_WEAR_WAIST,  ITEM_WEAR_WRIST,  ITEM_WEAR_WRIST,
    ITEM_WEAR_WIELD, ITEM_WEAR_WIELD,  ITEM_WEAR_TAKE,   ITEM_WEAR_DUALWIELD,
    ITEM_WEAR_EAR  , ITEM_WEAR_EAR,    ITEM_WEAR_NOSE };

  char *already_wearing[] = {
    "You're already using a light.\r\n",
    "YOU SHOULD NEVER SEE THIS MESSAGE.  PLEASE REPORT.\r\n",
    "You're already wearing something on both of your ring fingers.\r\n",
    "YOU SHOULD NEVER SEE THIS MESSAGE.  PLEASE REPORT.\r\n",
    "You can't wear anything else around your neck.\r\n",
    "You're already wearing something on your body.\r\n", // 5
    "You're already wearing something on your head.\r\n",
    "You're already wearing something on your legs.\r\n",
    "You're already wearing something on your feet.\r\n",
    "You're already wearing something on your hands.\r\n",
    "You're already wearing something on your arms.\r\n", //
    "You're already using a shield.\r\n",
    "You're already wearing something about your body.\r\n",
    "You already have something around your waist.\r\n", // 13
    "YOU SHOULD NEVER SEE THIS MESSAGE.  PLEASE REPORT.\r\n",
    "You're already wearing something around both of your wrists.\r\n", // 15
    "YOU SHOULD NEVER SEE THIS MESSAGE.  PLEASE REPORT.\r\n",
    "You're already wielding a weapon in both your hands.\r\n",
    "You're already holding something.\r\n",
    "You need both your hands to wield a two-handed weapon.\r\n",
    "YOU SHOULD NEVER SEE THIS MESSAGE.  PLEASE REPORT.\r\n",
    "You're already wearing something in both your ears.\r\n",
    "You're already wearing something in your nose.\r\n"
  };

  /* first, make sure that the wear position is valid. */
  if (!CAN_WEAR(obj, wear_bitvectors[where]) && !CAN_WEAR(obj,wear_bitvectors[POS_DUALWIELD])) {
    act("You can't wear $p there.", FALSE, ch, obj, 0, TO_CHAR);
    return;
  }

  if ((GET_EQ(ch,WEAR_WIELD_R)||GET_EQ(ch,WEAR_WIELD_L)) && CAN_WEAR(obj,ITEM_WEAR_DUALWIELD)) {
    send_to_char(already_wearing[POS_DUALWIELD],ch);
    return;
  }

  /* for neck, finger, and wrist, try pos 2 if pos 1 is already full */
  if  ((where == WEAR_FINGER_R) || (where == WEAR_NECK_1) || (where == WEAR_WRIST_R) || (where == WEAR_WIELD_R) || (where == WEAR_EAR_R)) {
    if (GET_EQ(ch, where)&& !CAN_WEAR(GET_EQ(ch,where),ITEM_WEAR_DUALWIELD))
      where++;
    else if (GET_EQ(ch, where)&& CAN_WEAR(GET_EQ(ch,where),ITEM_WEAR_DUALWIELD)) {
      send_to_char(already_wearing[where + 1], ch);
      return;
    }
  }

  if  ((where == WEAR_FINGER_R) || (where == WEAR_WIELD_R)) {
    if (IS_SET(GET_PERMANENT(ch), PRM_RH_CUT)&& !CAN_WEAR(obj,ITEM_WEAR_DUALWIELD))
      where++;
    else if (IS_SET(GET_PERMANENT(ch), PRM_RH_CUT)&& CAN_WEAR(obj,ITEM_WEAR_DUALWIELD)) {
      send_to_char("You can't...That hand is cut, remember?\r\n", ch);
      return;
    }
  }

  if ((((where == WEAR_WIELD_R) || (where == WEAR_FINGER_R) || (where == WEAR_HANDS)) && IS_SET(GET_PERMANENT(ch), PRM_RH_CUT)) ||
      (((where == WEAR_WIELD_L) || (where == WEAR_FINGER_L) || (where == WEAR_HANDS)) && IS_SET(GET_PERMANENT(ch), PRM_LH_CUT))) {
    send_to_char("You can't...That hand is cut, remember?\r\n", ch);
    return;
  }
  if (GET_EQ(ch,where) && CAN_WEAR(obj,ITEM_WEAR_DUALWIELD)) {
    send_to_char(already_wearing[POS_DUALWIELD], ch);
    return;
  }  

  if (GET_EQ(ch, where)) {
    send_to_char(already_wearing[where], ch);
    return;
  }

  if (!wear_otrigger(obj, ch, where))
   return;

  oprog = check_for_objprogs_and_stops(ch, obj, NULL,ch->carrying, OPC_WEAR, 0);
  if (oprog != 2) {
    if (CAN_WEAR(obj,ITEM_WEAR_DUALWIELD)) {
      wear_message(ch, obj, POS_DUALWIELD);
      obj_from_char(obj);
      equip_char(ch, obj, where);
    } 
    else {
      wear_message(ch, obj, where);
      obj_from_char(obj);
      equip_char(ch, obj, where);
    }
  }
  if (oprog)
    run_objprog(ch, obj);
#undef POS_DUALWIELD
}



int find_eq_pos(struct char_data * ch, struct obj_data * obj, char *arg)
{
  int where = -1;

  static char *keywords[] = {
    "!RESERVED!",
    "finger",
    "!RESERVED!",
    "neck",
    "!RESERVED!",
    "body",
    "head",
    "legs",
    "feet",
    "hands",
    "arms",
    "shield",
    "about",
    "waist",
    "wrist",
    "!RESERVED!",
    "!RESERVED!",
    "!RESERVED!",
    "!RESERVED!",
    "!RESERVED!",
    "ear",
    "!RESERVED!",
    "nose",
    "\n"
  };

  if (!arg || !*arg) {
    if (CAN_WEAR(obj, ITEM_WEAR_FINGER))      where = WEAR_FINGER_R;
    if (CAN_WEAR(obj, ITEM_WEAR_NECK))        where = WEAR_NECK_1;
    if (CAN_WEAR(obj, ITEM_WEAR_BODY))        where = WEAR_BODY;
    if (CAN_WEAR(obj, ITEM_WEAR_HEAD))        where = WEAR_HEAD;
    if (CAN_WEAR(obj, ITEM_WEAR_LEGS))        where = WEAR_LEGS;
    if (CAN_WEAR(obj, ITEM_WEAR_FEET))        where = WEAR_FEET;
    if (CAN_WEAR(obj, ITEM_WEAR_HANDS))       where = WEAR_HANDS;
    if (CAN_WEAR(obj, ITEM_WEAR_ARMS))        where = WEAR_ARMS;
    if (CAN_WEAR(obj, ITEM_WEAR_SHIELD))      where = WEAR_SHIELD;
    if (CAN_WEAR(obj, ITEM_WEAR_ABOUT))       where = WEAR_ABOUT;
    if (CAN_WEAR(obj, ITEM_WEAR_WAIST))       where = WEAR_WAIST;
    if (CAN_WEAR(obj, ITEM_WEAR_WRIST))       where = WEAR_WRIST_R;
    if (CAN_WEAR(obj, ITEM_WEAR_EAR))         where = WEAR_EAR_R;
    if (CAN_WEAR(obj, ITEM_WEAR_NOSE))        where = WEAR_NOSE;
  } else {
    if (((where = search_block(arg, (const char **)keywords, FALSE)) < 0) ||
        (*arg=='!')) {
      sprintf(buf, "'%s'?  What part of your body is THAT?\r\n", arg);
      send_to_char(buf, ch);
      return -1;
    }
  }

  return where;
}



ACMD(do_wear)
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  struct obj_data *obj, *next_obj;
  int where, dotmode, items_worn = 0;

  two_arguments(argument, arg1, arg2);

  if (ROOM_FLAGGED(ch->in_room, ROOM_NOSPAM) && (!IS_NPC(ch)) && (GET_LEVEL(ch) < LVL_GRGOD))
  {
    send_to_char("Sorry, you can't do that here.\r\n", ch);
    return;
  }

  if (!*arg1) {
    send_to_char("Wear what?\r\n", ch);
    return;
  }
  dotmode = find_all_dots(arg1);

  if (*arg2 && (dotmode != FIND_INDIV)) {
    send_to_char("You can't specify the same body location for more than one item!\r\n", ch);
    return;
  }
  if (dotmode == FIND_ALL) {
    for (obj = ch->carrying; obj; obj = next_obj) {
      next_obj = obj->next_content;
      if (CAN_SEE_OBJ(ch, obj) && (where = find_eq_pos(ch, obj, 0)) >= 0) {
    items_worn++;
    perform_wear(ch, obj, where);
      }
    }
    if (!items_worn)
      send_to_char("You don't seem to have anything wearable.\r\n", ch);
  } else if (dotmode == FIND_ALLDOT) {
    if (!*arg1) {
      send_to_char("Wear all of what?\r\n", ch);
      return;
    }
    if (!(obj = get_obj_in_list_vis(ch, arg1, ch->carrying))) {
      sprintf(buf, "You don't seem to have any %ss.\r\n", arg1);
      send_to_char(buf, ch);
    } else
      while (obj) {
    next_obj = get_obj_in_list_vis(ch, arg1, obj->next_content);
    if (GET_OBJ_STATUS(obj) < 2) {
          act("$p seems to be broken.", TRUE, ch, obj, 0, TO_CHAR);
          continue;
        }
        if ((where = find_eq_pos(ch, obj, 0)) >= 0)
      perform_wear(ch, obj, where);
    else
      act("You can't wear $p.", FALSE, ch, obj, 0, TO_CHAR);
    obj = next_obj;
      }
  } else {
    if (!(obj = get_obj_in_list_vis(ch, arg1, ch->carrying))) {
      sprintf(buf, "You don't seem to have %s %s.\r\n", AN(arg1), arg1);
      send_to_char(buf, ch);
    } else {
      if (GET_OBJ_STATUS(obj)<2) {
        act("$p seems to be broken.", TRUE, ch, obj, 0, TO_CHAR);
        return;
      }
      if ((where = find_eq_pos(ch, obj, arg2)) >= 0)
    perform_wear(ch, obj, where);
      else if (!*arg2)
    act("You can't wear $p.", FALSE, ch, obj, 0, TO_CHAR);
    }
  }
}



ACMD(do_wield)
{
  struct obj_data *obj;

  one_argument(argument, arg);

  if (ROOM_FLAGGED(ch->in_room, ROOM_NOSPAM) && (!IS_NPC(ch)) && (GET_LEVEL(ch) < LVL_GRGOD))
  {
    send_to_char("Sorry, you can't do that here.\r\n", ch);
    return;
  }
  if (!*arg)
    send_to_char("Wield what?\r\n", ch);
  else if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying))) {
    sprintf(buf, "You don't seem to have %s %s.\r\n", AN(arg), arg);
    send_to_char(buf, ch);
  } else {
    if (!CAN_WEAR(obj, ITEM_WEAR_WIELD) && !CAN_WEAR(obj,ITEM_WEAR_DUALWIELD))
      send_to_char("You can't wield that.\r\n", ch);
    else {
      if (GET_OBJ_STATUS(obj)<2) {
        send_to_char("It's broken!\r\n", ch);
        return;
      } 
      perform_wear(ch, obj, WEAR_WIELD_R);   
    }
  }
}



ACMD(do_grab)
{
  struct obj_data *obj;

  one_argument(argument, arg);

  if (!*arg)
    send_to_char("Hold what?\r\n", ch);
  else if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying))) {
    sprintf(buf, "You don't seem to have %s %s.\r\n", AN(arg), arg);
    send_to_char(buf, ch);
  } else {
    if (IS_SET(GET_PERMANENT(ch), PRM_RH_CUT)) {
      send_to_char("Sorry, but your right hand is no more. You can't do that\r\n", ch);
      return;
    }
    if (GET_OBJ_TYPE(obj) == ITEM_LIGHT)
      perform_wear(ch, obj, WEAR_LIGHT);
    else {
      if (!CAN_WEAR(obj, ITEM_WEAR_HOLD) && GET_OBJ_TYPE(obj) != ITEM_POTION)
    send_to_char("You can't hold that.\r\n", ch);
      else
    perform_wear(ch, obj, WEAR_HOLD);
    }
  }
}



void perform_remove(struct char_data * ch, int pos)
{ 
  int oprog;
  struct obj_data *obj;

  if (!(obj = GET_EQ(ch, pos))) {
    mlog("Error in perform_remove: bad pos passed.");
    return;
  }
  if (!IS_NPC(ch) && (ch->player_specials->linking[0] || 
      ch->player_specials->linked_by) &&  GET_LEVEL(ch)<LVL_IMMORT &&
      (IS_SET(OBJ_EXTRA_FLAGS(obj),ITEM_ANGREAL) ||
       IS_SET(OBJ_EXTRA_FLAGS(obj),ITEM_SAANGREAL))) {
    send_to_char("To avoid a mana bug, you're not allowed to remove Sa'angreal or Angreal while linked or linking.\r\n", ch);
    return;
  }
  if (IS_CARRYING_N(ch) >= CAN_CARRY_N(ch)) {
    act("$p: you can't carry that many items!", FALSE, ch, obj, 0, TO_CHAR);
    return;
  }

  if (!remove_otrigger(obj, ch))
   return;
 
  oprog = check_for_objprogs_and_stops(ch, obj, NULL, GET_EQ(ch,pos), OPC_REMOVE, 0);
  if (oprog != 2) {
    obj_to_char(unequip_char(ch, pos), ch);
    GET_EQ(ch,pos) = NULL;
    act("You stop using $p.", FALSE, ch, obj, 0, TO_CHAR);
    act("$n stops using $p.", TRUE, ch, obj, 0, TO_ROOM);
  }
  if (oprog)
    run_objprog(ch, obj);
}



ACMD(do_remove)
{
  struct obj_data *obj;
  int i, dotmode, found;

  one_argument(argument, arg);

  if (ROOM_FLAGGED(ch->in_room, ROOM_NOSPAM) && (!IS_NPC(ch)) && (GET_LEVEL(ch) < LVL_GRGOD))
  {
    send_to_char("Sorry, you can't do that here.\r\n", ch);
    return;
  }

  if (FIGHTING(ch)) {
    send_to_char("Not while fighting!\r\n",ch);
    return;
  }

  if (!*arg) {
    send_to_char("Remove what?\r\n", ch);
    return;
  }
  dotmode = find_all_dots(arg);

  if (dotmode == FIND_ALL) {
    found = 0;
    for (i = 0; i < NUM_WEARS; i++)
      if (GET_EQ(ch, i)) {
    perform_remove(ch, i);
    found = 1;
      }
    if (!found)
      send_to_char("You're not using anything.\r\n", ch);
  } else if (dotmode == FIND_ALLDOT) {
    if (!*arg)
      send_to_char("Remove all of what?\r\n", ch);
    else {
      found = 0;
      for (i = 0; i < NUM_WEARS; i++)
    if (GET_EQ(ch, i) && CAN_SEE_OBJ(ch, GET_EQ(ch, i)) &&
        isname(arg, GET_EQ(ch, i)->name)) {
      perform_remove(ch, i);
      found = 1;
    }
      if (!found) {
    sprintf(buf, "You don't seem to be using any %ss.\r\n", arg);
    send_to_char(buf, ch);
      }
    }
  } else {
    if (!(obj = get_object_in_equip_vis(ch, arg, ch->equipment, &i))) {
      sprintf(buf, "You don't seem to be using %s %s.\r\n", AN(arg), arg);
      send_to_char(buf, ch);
    } else
      perform_remove(ch, i);
  }
}

ACMD(do_play)
{
  struct obj_data *obj;
  struct char_data *vict;
  int result, delay, val;
  struct affected_type af,af2;
  char tmp[500];

  if (!can_use_gskill(ch, SKILL_PLAY)) 
     { 
     send_to_char("You can't really play all that well, I'm afraid..\r\n", ch);
     return;
     }

  if (IS_PLAYING(ch)) {
    sprintf(buf, "Sorry, but you have to wait another %d ticks to attempt to play.\r\n", IS_PLAYING(ch));
    send_to_char(buf, ch);
    return;
  }
  one_argument(argument, arg);
  if (!GET_SKILL(ch, SKILL_PLAY)) {
    send_to_char("You'd better go get some guitar lessons first.\r\n", ch);
    return;
  }
  if (!*arg) {
    send_to_char("Play on what?\r\n", ch);
    return;
  }
  if (!(obj = GET_EQ(ch, WEAR_HOLD)) ){
    send_to_char("You're not holding an instrument! You want to play air guitar?\r\n", ch);
    return;
  }
  if (!(GET_OBJ_TYPE(obj) == ITEM_INSTRUMENT)) {
    send_to_char("That's no instrument. \r\n", ch);
    return;
  }
  result = generic_result(ch, SKILL_PLAY, NULL, FALSE); 
  if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
    sprintf(buf,"%s tries to play on %s, but fails.\r\n", GET_NAME(ch),
    OBJS(obj, ch));
  }
  else {
    switch (GET_OBJ_VAL(obj, 0)) {
      case 0:  /* No effect */
        return;
        break;
      case 1:  /* Harmful */        
          if (GET_OBJ_VAL(obj, 2) == 2) { /* Harmful for Mana */
          act("$n plays something really, really sad that makes you despair and grow weary.", TRUE, ch, 0, 0, TO_ROOM);
          send_to_char("You play a tune that makes your opponent sad and weary.\r\n", ch);
        } else if (GET_OBJ_VAL(obj, 2) == 3) { /*Harmful for Move */
          act("$n plays a strange tune that makes you very very tired. \r\n", TRUE, ch, 0, 0, TO_ROOM);
          send_to_char("You play a tune that makes your opponent feel sooo tired. \r\n", ch);
        } else if (GET_OBJ_VAL(obj, 2) != 1)
          send_to_char("Something's way wrong with your instrument. Please tell an imm. \r\n", ch);  
        break;
      case 2: /* healing */
        if (GET_OBJ_VAL(obj, 2) == 1) { /* Healing hitpoints */
          act("$n plays a soothing, wonderful tune that makes you ignore your wounds.", TRUE, ch, 0, 0, TO_ROOM);
          send_to_char("You play a soothing, healing tune that makes everyone feel better.\r\n", ch);
        } else if (GET_OBJ_VAL(obj, 2) == 2) { /* Healing Mana */
          act("$n plays a gay and frivolous tune that makes your heart feel lighter.", TRUE, ch, 0, 0, TO_ROOM);
          send_to_char("You play a happy and glad tune that can cheer people up.\r\n", ch);
        } else if (GET_OBJ_VAL(obj, 2) == 3) { /*Harmful for Move */
          act("$n plays a refreshing tune that makes you feel less tired. \r\n", TRUE, ch, 0, 0, TO_ROOM);
          send_to_char("You play a happy and invigorating tune.\r\n", ch);
        } else
          send_to_char("Your instrument has malfunctioned. Tell an imm. \r\n", ch);
        break;
      default:
          send_to_char("Your instrument has malfunctioned. Tell an imm. \r\n", ch);
        break;
    }
  }  
  val = GET_OBJ_VAL(obj, 1);
  delay = GET_OBJ_VAL(obj, 3);
  
  for (vict = world[ch->in_room].people; vict; vict = vict->next_in_room)
    switch (GET_OBJ_VAL(obj, 2)) {
      case 1: 

        af  = clear_aff_var(af);    // Clearing the
        af2 = clear_aff_var(af2);   // aff lists.


        if ( (GET_OBJ_VAL(obj, 2) == 1) &&  ( !IS_AFFECTED2(vict,AFF2_BADMORALE) ) && (vict != ch) )
              {
              act("$n plays something really horrible that makes your ears hurt! ARGH!", TRUE, ch, 0, 0, TO_ROOM);
              sprintf(tmp,"You play a nasty, offensive tune that hurts %s's morale!\r\n", GET_NAME(vict));
              send_to_char(tmp,ch);

              // send_to_char("You play a nasty, offensive tune that hurts $N's morale!\r\n", ch);
              }


        af.bitvector2 = AFF2_BADMORALE; // Setting affect specifics.
        af.duration   = 3;
        af.type       = SKILL_PLAY;
        af.location   = APPLY_AC;
        af.modifier   = val;

        af2.bitvector = af.bitvector;
        af2.duration  = af.duration;
        af2.type      = af.type;
        af2.location  = APPLY_HITROLL;
        af2.modifier  = -val;

         if (IS_AFFECTED2(vict,AFF2_BADMORALE)) 
            {
            send_to_char("Your opponent's morale is already at a really sad state.\r\n",ch);
            return;
            }

        if (vict != ch)
            {
            affect_to_char(vict,&af);
            affect_to_char(vict,&af2); 
            }
      break;
      case 2: 
        if (GET_OBJ_VAL(obj, 0) == 1) 
          SET_MANA(vict, MAX(-5 , GET_MANA(vict) - val));
        else
          SET_MANA(vict, MIN(GET_MANA(vict) + val , GET_MAX_MANA(vict)));
      break;
      case 3: 
        if(GET_OBJ_VAL(obj, 0) == 1) 
          GET_MOVE(vict) = MAX(0 , GET_MOVE(vict) - val); 
        else
          GET_MOVE(vict) = MIN(GET_MOVE(vict) + val , GET_MAX_MOVE(vict)); 
      break;
      default:
        send_to_char("Ooops..something about your instrument fucked up. Tell an imm.\r\n", ch);
    }
  IS_PLAYING(ch) = delay / (1 + (0.02*GET_SKILL(ch, SKILL_PLAY)));
}

ACMD(do_apply)
{
  struct obj_data *herb = NULL;
  struct char_data *vict;
  char herbname[50], target[50];
  int val = 0;
  
  two_arguments(argument, herbname, target);
  
  if (!*herbname) {
    send_to_char("Apply what on whom?\r\n", ch);
    send_to_char("Syntax: Apply (herbname) [(target)]\r\n", ch);
    return;
  }

  if (!(herb = get_obj_in_list_vis(ch,herbname,ch->carrying))) {
    asend_to_char(ch,"You don't seem to have such a thing!\r\n");
    return;
  }

  if (ITEM_HERB != GET_OBJ_TYPE(herb)) {
    asend_to_char(ch,"Hey, %s %s is not a herb!\r\n",AN(herb->short_description),herb->short_description);
    return;
  }

  if (*target) {
    if (!(vict = get_char_room_vis(ch, target))) {
      send_to_char("That person isn't here...\r\n", ch);
      return;
    }
  } else
    vict = ch;
  
  if ((GET_POS(ch) == POS_FIGHTING) || (GET_POS(vict) == POS_FIGHTING)) {
    send_to_char("Can't do that while fighting or on fighting persons.\r\n", ch);
    return;
  }
  
#if 0
  if (IS_NPC(vict)) {
    send_to_char("DOH! Not on mobs!\r\n", ch);
    return;
  }
#endif
  sprintf(buf,"$n applies %s on %s.", herb->short_description, 
         (ch == vict) ? "$mself": GET_NAME(vict));
  act(buf, TRUE, ch, 0, vict, TO_NOTVICT);
  
  sprintf(buf, "You apply %s on %s.\r\n", herb->short_description, 
         (ch == vict) ? "yourself": GET_NAME(vict));
  send_to_char(buf, ch);
  
  if (ch != vict) {
    sprintf(buf,"%s applies %s on you.\r\n", GET_NAME(ch), herb->short_description);
    send_to_char(buf, vict);
  }
  
  if (!GET_OBJ_VAL(herb, 0))
    return;
  
  val = GET_OBJ_VAL(herb, 1);
  
  if (GET_OBJ_VAL(herb, 0) == 1)
    val = -val;
  
  switch (GET_OBJ_VAL(herb, 2)) {
    case 1:
      GET_HIT(vict) = MIN(GET_MAX_HIT(vict), GET_HIT(vict) + val);
    break;
    case 2:
      SET_MANA(vict, MIN(GET_MAX_MANA(vict), GET_MANA(vict) + val));
    break;
    case 3:
      GET_MOVE(vict) = MIN(GET_MAX_MOVE(vict), GET_MOVE(vict) + val);
    break;
  }
  update_pos(vict);  
  extract_obj(herb);
}


ACMD(do_pull)
{
  struct obj_data *obj= NULL;
  int vnum = 0;
  skip_spaces(&argument);
  
  if (!argument)  {
    send_to_char("What do you want to pull?\r\n", ch);
    return;
  }
  if (!(obj = get_obj_in_list_vis(ch, argument, world[ch->in_room].contents))) {
    send_to_char("I can't see any such wagon in this room.\r\n", ch);
    return;
  }
  if (GET_OBJ_TYPE(obj) != ITEM_WAGON) {
    send_to_char("That's not a wagon.\r\n", ch);
    return;
  }
  if (obj->pulled_by) {
    act("$p is already being pulled by $N.", TRUE, ch, obj, obj->pulled_by, TO_CHAR);
    return;
  }
  if (ch->pulling) {
    send_to_char("You're already pulling a wagon. Unpull it first.\r\n", ch);
    return;
  }
  if (GET_POS(ch) != POS_STANDING) {
    send_to_char("You must be standing to pull a wagon.\r\n", ch);    
    return;
  }
  if (IS_SET(OBJ_EXTRA_FLAGS(obj),ITEM_NOPULL)) {
    send_to_char("The object can't be pulled.\r\n", ch);
    return;
  }
  vnum = GET_OBJ_VNUM(obj);
  if (real_room(vnum) == NOWHERE) {
    send_to_char("The room connected to this wagon doesn't exist. Tell an imm.\r\n", ch);
    return;
  }  
  if (!ROOM_FLAGGED(real_room(vnum), ROOM_WAGON)) {
    send_to_char("The room connected to this wagon isn't flagged as a wagon. Tell an imm.\r\n", ch);
    return;
  }
  act("$n begins to pull $p.", TRUE, ch, obj, 0, TO_NOTCHAR);
  act("Ok, you begin to pull $p", TRUE, ch, obj, 0, TO_CHAR); 
  obj->pulled_by = ch;
  ch->pulling = obj;
}

ACMD(dont_pull)
{
  struct obj_data *wagon;

  if (!(wagon = ch->pulling)){
    send_to_char("But you're not pulling anything!\r\n", ch);
    return;
  }
  act("Ok, you stop pulling $p.", TRUE, ch, wagon, 0, TO_CHAR);
  act("$n stops pulling $p.", TRUE, ch, wagon, 0, TO_NOTCHAR);  
  wagon->pulled_by = NULL;
  ch->pulling = NULL;
}

ACMD(do_wash)
{
  struct obj_data *obj;
  int found = 0;
  if (!affected_by_spell(ch, SKILL_DIRTKICK)) {
    send_to_char("What for? You have nothing in your eyes.\r\n", ch);
    return;
  }
  if (GET_POS(ch) == POS_FIGHTING) {
    send_to_char("Not now! You're in the middle of a fight!\r\n", ch);
    return;
  }
  for (obj = ch->carrying; obj; obj = obj->next_content)
    if ((GET_OBJ_TYPE(obj) == ITEM_DRINKCON) && (GET_OBJ_VAL(obj,1) > 0)) {
      found = TRUE;
      weight_change_object(obj, -1);
      GET_OBJ_VAL(obj, 1)--;
      act("$n washes $s eyes, at last seeing again.", TRUE, ch, 0, 0, TO_ROOM);
      act("You wash your eyes, at last being able to see again.", TRUE, ch, 0, 0, TO_CHAR);
      break;
    }

  if( !found )
    for( obj = world[ch->in_room].contents; obj; obj = obj->next_content )
      if( GET_OBJ_TYPE( obj ) == ITEM_FOUNTAIN) 
      { found = TRUE;
	act("$n washes $s eyes in the fountain so that $e can see again.", TRUE, ch, 0, 0, TO_ROOM);
	act("You wash your eyes in the fountain so that you can see again.", TRUE, ch, 0, 0, TO_CHAR);
	break;
      }
		       
  if (!found) {
    send_to_char("You have nothing with which you can wash your eyes.\r\n", ch);
    return;
  }

  affect_from_char(ch, SKILL_DIRTKICK); 
}

void appraise_wpn(struct char_data *ch,struct obj_data *obj,long result)
{
  int i = 0;
  const char *damtype[] =
  {
    "hit",
    "sting",
    "whip",
    "slash",
    "bite",
    "bludgeon",
    "crush",
    "pound",
    "claw",
    "maul",
    "thrash",
    "pierce",
    "blast",
    "punch",
    "stab",
    "whack"
  };

  result = ((result == TOTAL_FUCKUP) ? TOTAL_FAILURE : result);
  result = ((result == TOTAL_SUCCESS)? SOME_SUCCESS  : result);

  asend_to_char(ch,"Item: %s is of type ",obj->short_description);
  if (ITEM_ARROW  ==GET_OBJ_TYPE(obj)) send_to_char("\\c01arrow\\c00",ch);
  if (ITEM_BOW    ==GET_OBJ_TYPE(obj)) send_to_char("\\c01bow\\c00",ch);
  if (ITEM_WEAPON ==GET_OBJ_TYPE(obj)) send_to_char("\\c01weapon\\c00",ch);
  send_to_char("\r\n",ch);

  asend_to_char(ch,"And it's status is %s\r\n",obj_status[(((obj->obj_status)+19)/20)]);
  asend_to_char(ch,"The damage of the weapon: %dd%d\r\n",
  ((result == TOTAL_FAILURE) ? number(1,GET_OBJ_VAL(obj,1)) : GET_OBJ_VAL(obj,1)),
  ((result == TOTAL_FAILURE) ? number(1,GET_OBJ_VAL(obj,2)) : GET_OBJ_VAL(obj,2)));
  asend_to_char(ch,"It weighs %d lbs, and it's worth ",obj->obj_flags.weight);
  if (!obj->obj_flags.cost) {
    if(!obj->obj_flags.questcost) {
      send_to_char("nothing, it's totally worthless!\r\n",ch);
    } else {
      send_to_char("it's unvaluable!\r\n",ch);
    }
  }
  else {
    if (!obj->obj_flags.questcost) {
      asend_to_char(ch,"%d gold\r\n",((result == TOTAL_FAILURE) ? number(1,obj->obj_flags.cost) : obj->obj_flags.cost));
    }
    else {
      send_to_char("it's unvaluable!\r\n",ch);
    }
  }
  asend_to_char(ch,"Damage type: %s\r\n", ((result == TOTAL_FAILURE) ? 
   damtype[(number(TYPE_HIT,TYPE_WHACK) - TYPE_HIT)] : damtype[GET_OBJ_VAL(obj,3)]));  
  asend_to_char(ch,"And it has the following affections: \r\n");
  if (result == TOTAL_FAILURE) {
    send_to_char("\\c01NOTHING!\\c00\r\n",ch);
  } else {
    if (!obj->affected[i].location) {
      send_to_char("\\c01NOTHING!\\c00\r\n",ch);
      return;
    }
    for (i = 0; (i < MAX_OBJ_AFFECT) && obj->affected[i].location; i++) {
      asend_to_char(ch,"%d to %s\r\n",obj->affected[i].modifier,apply_types[(int)obj->affected[i].location]);
    }
  }
}

void appraise_arm(struct char_data *ch,struct obj_data *obj,long result)
{
  char buf[2000];
  int i = 0;

  result = ((result == TOTAL_FUCKUP) ? TOTAL_FAILURE : result);
  result = ((result == TOTAL_SUCCESS)? SOME_SUCCESS  : result);
    
  asend_to_char(ch,"Item: %s is of type \\c01armor\\c00.\r\n",obj->short_description);
  asend_to_char(ch,"AC-apply: %d\r\n",
  ((result == TOTAL_FAILURE) ? number(0,GET_OBJ_VAL(obj,0)) : GET_OBJ_VAL(obj,0)));
  asend_to_char(ch,"ABS_apply: %d\r\n",
  ((result == TOTAL_FAILURE) ? number(0,GET_OBJ_VAL(obj,1)) : GET_OBJ_VAL(obj,1)));
  asend_to_char(ch,"Status: %s\r\n",obj_status[(((obj->obj_status)+19)/20)]);
  asend_to_char(ch,"Can be worn on: ");
  sprintbit(obj->obj_flags.wear_flags,wear_bits, buf);
  asend_to_char(ch,"%s\r\n",buf);
  asend_to_char(ch,"Weight: %d, value ",obj->obj_flags.weight);
  if (!obj->obj_flags.cost) {
    if(!obj->obj_flags.questcost) {
      send_to_char("nothing, it's totally worthless!\r\n",ch);
    } else {
      send_to_char("it's unvaluable!\r\n",ch);
    }
  }
  else {
    if (!obj->obj_flags.questcost) {
      asend_to_char(ch,"%d gold\r\n",((result == TOTAL_FAILURE) ? number(1,obj->obj_flags.cost) : obj->obj_flags.cost));
    }
    else {
      send_to_char("it's unvaluable!\r\n",ch);  
    }
  }
  asend_to_char(ch,"And it has the following affections: \r\n");
  if (result == TOTAL_FAILURE) {
    send_to_char("\\c01NOTHING!\\c00\r\n",ch);
  } else {
    if (!obj->affected[i].location) {
      send_to_char("\\c01NOTHING!\\c00\r\n",ch);
      return;
    }
    for (i = 0; (i < MAX_OBJ_AFFECT) && obj->affected[i].location; i++) {
      asend_to_char(ch,"%d to %s\r\n",obj->affected[i].modifier,apply_types[(int)obj->affected[i].location]);
    }
  }
}

void appraise_tre(struct char_data *ch,struct obj_data *obj,long result)
{ 
  char buf[2000];
  int i = 0;
    
  result = ((result == TOTAL_FUCKUP) ? TOTAL_FAILURE : result);
  result = ((result == TOTAL_SUCCESS)? SOME_SUCCESS  : result);

  asend_to_char(ch,"Item: %s is of type: \\c01treasure\\c00\r\n",obj->short_description);
  asend_to_char(ch,"Can be worn on: ");
  sprintbit(obj->obj_flags.wear_flags,wear_bits, buf);
  asend_to_char(ch,"%s\r\n",buf);
  asend_to_char(ch,"Weight: %d, value ",obj->obj_flags.weight);
  if (!obj->obj_flags.cost) {
    if(!obj->obj_flags.questcost) {
      send_to_char("nothing, it's totally worthless!\r\n",ch);
    } else {
      send_to_char("it's unvaluable!\r\n",ch);
    }
  }
  else {
    if (!obj->obj_flags.questcost) {
      asend_to_char(ch,"%d gold\r\n",((result == TOTAL_FAILURE) ? number(1,obj->obj_flags.cost) : obj->obj_flags.cost));
    }
    else {
      send_to_char("it's unvaluable!\r\n",ch);
    } 
  }
  asend_to_char(ch,"And it has the following affections: \r\n");
  if (result == TOTAL_FAILURE) {
    send_to_char("\\c01NOTHING!\\c00\r\n",ch);
  } else {
    if (!obj->affected[i].location) {
      send_to_char("\\c01NOTHING!\\c00\r\n",ch);
      return;
    }
    for (i = 0; (i < MAX_OBJ_AFFECT) && obj->affected[i].location; i++) {
      asend_to_char(ch,"%d to %s\r\n",obj->affected[i].modifier,apply_types[(int)obj->affected[i].location]);
    }
  }
}

ACMD(do_appraise)
{
  int result,objt;
  struct obj_data *obj;

  skip_spaces(&argument);

  if (!GET_SKILL(ch,SKILL_APPRAISE)) {
    send_to_char("Huh?!?\r\n",ch);
    return;
  }

  if (!*argument) {
    send_to_char("Usage: appraise <object>\r\n",ch);
    return;
  }
  if (!(obj = get_obj_in_list_vis(ch,argument,ch->carrying))) {
    if ((obj = get_object_in_equip_vis(ch, argument, ch->equipment, &objt)));
    else if ((obj = get_obj_in_list_vis(ch, argument, world[IN_ROOM(ch)].contents)));
    else {
      asend_to_char(ch,"You don't seem to have a \"%s\" whatever it is...\r\n",argument);
      return;
    }
  }
  result = generic_result_mod(ch,SKILL_APPRAISE,NULL,FALSE,-(30 - GET_INT(ch)));
  objt = GET_OBJ_TYPE(obj);
  switch(objt) {
    case ITEM_ARROW:
    case ITEM_BOW:
    case ITEM_WEAPON:
      appraise_wpn(ch,obj,result);
      break;
    case ITEM_ARMOR:
      appraise_arm(ch,obj,result);
      break;
    case ITEM_TREASURE:
      appraise_tre(ch,obj,result);
      break;
    case ITEM_HEALING:
      if ((result < SOME_SUCCESS) || (GET_INT(ch) < 20))
      {
       send_to_char("It looks like some kind of potion.\r\n", ch);
       return;
      }
      asend_to_char(ch,"You recognize %s as a %s \\c01healing potion\\c00 that affects a person's ", 
                    obj->short_description, (GET_OBJ_VAL(obj, 1) == 0 ? "strong" : "weak"));
      if (GET_OBJ_VAL(obj, 0) == 0)
       asend_to_char(ch, "hitpoints.\r\n");
      else if (GET_OBJ_VAL(obj, 0) == 1)
       asend_to_char(ch, "ability to channel.\r\n");
      else if (GET_OBJ_VAL(obj, 0) == 2)
       asend_to_char(ch, "fatigue.\r\n");
      break;
    case ITEM_LIGHT:
      if (result < SOME_SUCCESS)
      {
       send_to_char("It looks like some kind of light source.\r\n", ch);
       return;
      }
      if (GET_OBJ_VAL(obj, 2) == 0)
       asend_to_char(ch,"You recognize %s as a worn out \\c01light source\\c00.\r\n", obj->short_description);
      else if (GET_OBJ_VAL(obj, 2) == -1)
       asend_to_char(ch,"You recognize %s as an infinite \\c01light source\\c00.\r\n", obj->short_description);
      else
       asend_to_char(ch,"You recognize %s as a \\c01light source\\c00 that should last %ld hours.\r\n",
            obj->short_description, GET_OBJ_VAL(obj, 2));
      break;
    case ITEM_HERB:
      if ((result < SOME_SUCCESS) && !GET_SKILL(ch, SKILL_HERBIDENT))
      {
       send_to_char("It looks like some kind of herb.\r\n", ch);
       return;
      }
      if (GET_OBJ_VAL(obj, 0) == 0)
      { 
       asend_to_char(ch,"You recognize %s as a useless \\c01herb\\c00.\r\n", obj->short_description);
       return;
      }
      else if (GET_OBJ_VAL(obj, 0) == 1)
       asend_to_char(ch,"You recognize %s as a poisonous \\c01herb\\c00.\r\n", obj->short_description);
      else if (GET_OBJ_VAL(obj, 0) == 2)
       asend_to_char(ch,"You recognize %s as a healing \\c01herb\\c00.\r\n", obj->short_description);

      if (GET_OBJ_VAL(obj, 2) == 1)
       asend_to_char(ch,"It seems to affect a person's hitpoints.\r\n");
      else if (GET_OBJ_VAL(obj, 2) == 2)
       asend_to_char(ch,"It seems to affect a person's ability to channel.\r\n");
      else if (GET_OBJ_VAL(obj, 2) == 3)
       asend_to_char(ch,"It seems to affect a person's fatigue.\r\n");
      break;
    case ITEM_WAGON:
      send_to_char("This is a \\c01wagon\\c00.\r\n", ch);
      if (IS_SET(GET_OBJ_VAL(obj, 1), CONT_CLOSEABLE))
       send_to_char("It is closeable.\r\n", ch);
      else send_to_char("It isn't closeable.\r\n", ch);
      if (IS_SET(GET_OBJ_VAL(obj, 1), CONT_PICKPROOF))
       send_to_char("It is pickproof.\r\n", ch);
      else send_to_char("It can be picked.\r\n", ch);
      if (IS_SET(GET_OBJ_VAL(obj, 1), CONT_CLOSED))
       send_to_char("It is closed.\r\n", ch);
      else send_to_char("It isn't closed.\r\n", ch);
      if (IS_SET(GET_OBJ_VAL(obj, 1), CONT_LOCKED))
       send_to_char("It is locked.\r\n", ch);
      else send_to_char("It isn't locked.\r\n", ch);
      if (GET_OBJ_VAL(obj, 2) != -1)
       send_to_char("It requires a key.\r\n", ch);
      break;
    case ITEM_CONTAINER:
      if (result < SOME_SUCCESS)
      {
       send_to_char("It looks like some kind of a container.\r\n", ch);
       return;
      }
      asend_to_char(ch, "This is a \\c01container\\c00 that can hold up to %ld lbs.\r\n", GET_OBJ_VAL(obj, 0));
      if (IS_SET(GET_OBJ_VAL(obj, 1), CONT_CLOSEABLE))
       send_to_char("It is closeable.\r\n", ch);
      else send_to_char("It isn't closeable.\r\n", ch);
      if (IS_SET(GET_OBJ_VAL(obj, 1), CONT_PICKPROOF))
       send_to_char("It is pickproof.\r\n", ch);
      else send_to_char("It can be picked.\r\n", ch);
      if (IS_SET(GET_OBJ_VAL(obj, 1), CONT_CLOSED))
       send_to_char("It is closed.\r\n", ch);
      else send_to_char("It isn't closed.\r\n", ch);
      if (IS_SET(GET_OBJ_VAL(obj, 1), CONT_LOCKED))
       send_to_char("It is locked.\r\n", ch);
      else send_to_char("It isn't locked.\r\n", ch);
      if (GET_OBJ_VAL(obj, 2) != -1)
       send_to_char("It requires a key.\r\n", ch);
      break;
    case ITEM_NOTE:
      send_to_char("This is a \\c01note\\c00, you write on it.\r\n", ch);
      break;
    case ITEM_DRINKCON:
      if (result < SOME_SUCCESS)
      {
       send_to_char("It looks like some kind of a drink container.\r\n", ch);
       return;
      }
      asend_to_char(ch, "This is a \\c01drink container\\c00 that can hold up to %ld drink units.\r\n",
            GET_OBJ_VAL(obj, 0));
      if (GET_OBJ_VAL(obj, 1))
       asend_to_char(ch, "It currently contains %ld drink units of %s.\r\n", GET_OBJ_VAL(obj, 1),
            drinks[GET_OBJ_VAL(obj, 2)]);
      break;
    case ITEM_KEY:
      send_to_char("It looks like some sort of a \\c01key\\c00.\r\n", ch);
      break;
    case ITEM_FOOD:
      if (result < SOME_SUCCESS)
      {
       send_to_char("It looks edible.\r\n", ch);
       return;
      }
      asend_to_char(ch, "You think that eating %s can keep you full for %ld hours.\r\n",
            obj->short_description, GET_OBJ_VAL(obj, 0));
      break;
    case ITEM_MONEY:
      asend_to_char(ch, "These are %ld gold coins !\r\n", GET_OBJ_VAL(obj, 0));
      break;
    case ITEM_PEN:
      send_to_char("Looks like you can write with it.\r\n", ch);
      break;
    case ITEM_BOAT:
      send_to_char("Yes, it looks like something you can sail with. A boat maybe ?\r\n", ch);
      break;
    case ITEM_FOUNTAIN:
      if (result < SOME_SUCCESS)
      {
       send_to_char("It looks like some kind of drinking source.\r\n", ch);
       return;
      }
      asend_to_char(ch, "This is a \\c01fountain\\c00 that can hold up to %ld drink units.\r\n",
                        GET_OBJ_VAL(obj, 0)); 
      if (GET_OBJ_VAL(obj, 1))
       asend_to_char(ch, "It currently contains %ld drink units of %s.\r\n", GET_OBJ_VAL(obj, 1),
                        drinks[GET_OBJ_VAL(obj, 2)]);
      break;            
    case ITEM_FURNITURE:
      send_to_char("This is a furniture item.\r\n", ch);
      break;
    case ITEM_NONAMEBOARD:
      send_to_char("Looks like some kind of strange board with no names.\r\n", ch);
      break;
    default:
      send_to_char("You have no idea what this item is or what it does.\r\n", ch);
      break;
  }
  act("$n peers intently at $p, appraising it...",FALSE,ch,obj,0,TO_ROOM);
}

