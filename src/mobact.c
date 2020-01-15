/******************************************************************************
 * $Id: mobact.c,v 1.3 2002/11/07 04:02:03 fang Exp $
 *   
 * File: mobact.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: mobact.c,v 
 *   Revision 1.2  2002/07/28 13:56:04  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.3  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.5  2001/01/10 17:34:54  mud
 *   *** empty log message ***
 *
 *   Revision 1.4  2000/03/09 12:47:44  mud
 *   Code cleanup. Compiles without warnings now...
 *
 *   Revision 1.3  2000/02/21 14:41:10  mud
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/02/21 14:00:18  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* ************************************************************************
*   File: mobact.c                                      Part of CircleMUD *
*  Usage: Functions for generating intelligent (?) behavior in mobiles    *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "conf.h"
#include "sysdep.h"
#include "class.h"
#include "mobact.h"
#include "structs.h"
#include "utils.h"
#include "db.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "spells.h"
#include "fight.h"
#include "act.h"
#include "graph.h"

void mobile_activity(void)
{
  register struct char_data *ch, *next_ch, *vict;
  struct obj_data *obj, *best_obj;
  int door, found, max;
  memory_rec *names;

  for (ch = character_list; ch; ch = next_ch) {
    next_ch = ch->next;

    if (!IS_MOB(ch) || FIGHTING(ch) || !AWAKE(ch))
      continue;

    /* Examine call for special procedure */
    if (MOB_FLAGGED(ch, MOB_SPEC) && !no_specials) {
      if (mob_index[GET_MOB_RNUM(ch)].func == NULL) {
    sprintf(buf, "%s (#%ld): Attempting to call non-existing mob func",
        GET_NAME(ch), GET_MOB_VNUM(ch));
    mlog(buf);
    REMOVE_BIT(MOB_FLAGS(ch), MOB_SPEC);
      } else {
    if ((mob_index[GET_MOB_RNUM(ch)].func) (ch, ch, 0, ""))
      continue;		/* go to next char */
      }
    }


    /* Scavenger (picking up objects) */
    if ((!(ch->desc)) && MOB_FLAGGED(ch, MOB_SCAVENGER) && !FIGHTING(ch) && AWAKE(ch))
      if (world[ch->in_room].contents && !number(0, 10)) {
    max = 1;
    best_obj = NULL;
    for (obj = world[ch->in_room].contents; obj; obj = obj->next_content)
      if (CAN_GET_OBJ(ch, obj) && GET_OBJ_COST(obj) > max) {
        best_obj = obj;
        max = GET_OBJ_COST(obj);
      }
    if (best_obj != NULL) {
      obj_from_room(best_obj);
      obj_to_char(best_obj, ch);
      act("$n gets $p.", FALSE, ch, best_obj, 0, TO_ROOM);
    }
      }

    /* Mob Movement */
    if ((!(ch->desc)) && !MOB_FLAGGED(ch, MOB_SENTINEL) && (GET_POS(ch) == POS_STANDING) &&
    ((door = number(0, 18)) < NUM_OF_DIRS) && CAN_GO(ch, door) &&
    !ROOM_FLAGGED(EXIT(ch, door)->to_room, ROOM_NOMOB) && !IS_AFFECTED(ch,AFF_WRAPPED) &&
    (!MOB_FLAGGED(ch, MOB_STAY_ZONE) ||
     (world[EXIT(ch, door)->to_room].zone == world[ch->in_room].zone))) {
      if (!IS_AFFECTED(ch,AFF_WRAPPED))
        perform_move(ch, door, 1);
    }

    /* Aggressive Mobs */
    if ((!(ch->desc)) && MOB_FLAGGED(ch, MOB_AGGRESSIVE)) {
      found = FALSE;
      for (vict = world[ch->in_room].people; vict && !found; vict = vict->next_in_room) {
    if (IS_NPC(vict) || !CAN_SEE(ch, vict) || PRF_FLAGGED(vict, PRF_NOHASSLE) || 
    GET_SKILL(vict, SKILL_PEACE) || IS_AFFECTED2(vict,AFF2_LONGSLEEP))   /* Peace, Aiel Wise One Guild Skill */
      continue;
    if (MOB_FLAGGED(ch, MOB_WIMPY) && AWAKE(vict))
      continue;
    if (IS_SET(AFF_FLAGS(ch), AFF_WRAPPED))
      continue;
        if (GET_DEST(ch)) 
          continue;       /* Inna TAXI, or similar */
    else {
      hit(ch, vict, TYPE_UNDEFINED);
      found = TRUE;
    }
      }
    }

    /* Mob Memory */
    if ((!(ch->desc)) && MOB_FLAGGED(ch, MOB_MEMORY) && MEMORY(ch)) {
      found = FALSE;
      for (vict = world[ch->in_room].people; vict && !found; vict = vict->next_in_room) {
    if (IS_NPC(vict) || !CAN_SEE(ch, vict) || PRF_FLAGGED(vict, PRF_NOHASSLE) ||
            IS_AFFECTED2(vict,AFF2_LONGSLEEP))
      continue;
    for (names = MEMORY(ch); names && !found; names = names->next)
      if (names->id == GET_IDNUM(vict)) {
        found = TRUE;
        act("'Hey!  You're the fiend that attacked me!!!', exclaims $n.",
        FALSE, ch, 0, 0, TO_ROOM);
        hit(ch, vict, TYPE_UNDEFINED);
      }
      }
    }

    /* Helper Mobs */
    if ((!(ch->desc)) && MOB_FLAGGED(ch, MOB_HELPER)) {
      found = FALSE;
      for (vict = world[ch->in_room].people; vict && !found; vict = vict->next_in_room) {
    if (IS_AFFECTED(vict,AFF_DREAMING) != MOB_FLAGGED(ch,MOB_TAR)) 
      continue;
    if (ch->master == FIGHTING(vict) && number(0, 100) > 3)
      continue;
    if (ch != vict && IS_NPC(vict) && FIGHTING(vict) &&
            !IS_NPC(FIGHTING(vict)) && ch != FIGHTING(vict)) {
      act("$n jumps to the aid of $N!", FALSE, ch, 0, vict, TO_ROOM);
      hit(ch, FIGHTING(vict), TYPE_UNDEFINED);
      found = TRUE;
    }
      }
    }
    /* Tracking mobs */
    if ((!(ch->desc)) && MOB_FLAGGED(ch, MOB_TRACKER) && HUNTING(ch))
      hunt_victim(ch);
      
    /* Charmed mobs */
    if (IS_SET(AFF_FLAGS(ch), AFF_CHARM))
      if((ch->master) && (ch->master != ch) && (ch->in_room == ch->master->in_room) &&
         FIGHTING(ch->master) && (FIGHTING(ch->master)->in_room == ch->in_room)) {
     if (number(0, 100) > 5) {
           act("$n screams and jumps to the aid of $s master!", FALSE, ch, 0, 0, TO_ROOM);
           hit(ch, FIGHTING(ch->master), TYPE_UNDEFINED);
         } else {
        act("$N doesn't seem intrested in joining your fight!", FALSE, ch->master, 0, ch, TO_CHAR);
        act("$N doesn't seems intrested in joining $n's fight.", FALSE, ch->master, 0, ch, TO_NOTCHAR);
     }
      }
         
   /* Mobs that have been programmed to go to a certain room */
   if ((!(ch->desc)) && GET_MOB_GOINGTO(ch))
     mob_goto(ch, GET_MOB_GOINGTO(ch));
     
          /* Add new mobile actions here */
  }				/* end for() */
}



/* Mob Memory Routines */

/* make ch remember victim */
void remember(struct char_data * ch, struct char_data * victim)
{
  memory_rec *tmp;
  bool present = FALSE;

  if (!IS_NPC(ch) || IS_NPC(victim))
    return;

  for (tmp = MEMORY(ch); tmp && !present; tmp = tmp->next)
    if (tmp->id == GET_IDNUM(victim))
      present = TRUE;

  if (!present) {
    CREATE(tmp, memory_rec, 1);
    tmp->next = MEMORY(ch);
    tmp->id = GET_IDNUM(victim);
    MEMORY(ch) = tmp;
  }
}


/* make ch forget victim */
void forget(struct char_data * ch, struct char_data * victim)
{
  memory_rec *curr, *prev = NULL;

  if (!(curr = MEMORY(ch)))
    return;

  while (curr && curr->id != GET_IDNUM(victim)) {
    prev = curr;
    curr = curr->next;
  }

  if (!curr)
    return;			/* person wasn't there at all. */

  if (curr == MEMORY(ch))
    MEMORY(ch) = curr->next;
  else
    prev->next = curr->next;

  free(curr);
}


/* erase ch's memory */
void clearMemory(struct char_data * ch)
{
  memory_rec *curr, *next;

  curr = MEMORY(ch);

  while (curr) {
    next = curr->next;
    free(curr);
    curr = next;
  }

  MEMORY(ch) = NULL;
}

void perform_mob_spells(struct char_data *ch)
{
  int mob_spells[25] =
  { 
    SPELL_DARTOFAIR,      SPELL_ICE_ARROWS,        SPELL_FIRE_ARROWS,
    SPELL_CHILL,          SPELL_EARTH_MISSILE,     SPELL_FIREBALL,
    SPELL_SLEEP,          SPELL_LIGHTNING,         SPELL_BURN,
    SPELL_AIRBLOW,        SPELL_SLOW,              SPELL_DROWN,
    SPELL_UNBALANCE,      SPELL_CONFUSION,         SPELL_DISINTEGRATE,
    SPELL_POISON,         SPELL_SHAFTOFFIRE,       SPELL_SHAFTOFFROST,
    SPELL_CRIPPLE,        SPELL_DEAF,              SPELL_DESTROY_ARMOR,
    SPELL_DESTROY_WEAPON, SPELL_FIRESWORD,         SPELL_CRIPPLE,
    SPELL_DISINTEGRATE 
  };
  int maxspell,sc = 0,lvl,stc = 0,mana,adder=0,diver=0,mobmana=0;

  if (GET_POS(ch) < POS_SITTING) {
    return;		/* Mobs should at least be sitting to weave */
  }

  if (IS_AFFECTED(ch,AFF_SHIELDED))
    return; // No channeling if shielded. 

  lvl = GET_LEVEL(ch);
  sc  = number(0,100);

  if (sc > ((lvl / 3) + 1))
    return;

  if (lvl >= 161) {
    maxspell = 25;
  }
  else if (lvl >= 121) {
    maxspell = 20;
  }
  else if (lvl >= 81) {
    maxspell = 15;
  }
  else if (lvl >= 41) {
    maxspell = 10;
  }
  else {
    maxspell = 5;
  }
  stc = mob_spells[number(0,maxspell)];
  mana = spell_info[stc].mana;

  if (mana > GET_MANA(ch)) {
    return;
  }
  mobmana = GET_MANA(ch) - mana;
 
  if (!mobmana) {
    spell_list[stc](ch,FIGHTING(ch),NULL,NULL,NULL,0,0,adder,1,1.0);
    return;
  }
  diver = number((mobmana/15),(mobmana/5));
  adder = number(0,diver);
  spell_list[stc](ch,FIGHTING(ch),NULL,NULL,NULL,0,0,adder,1,1.0);
}
