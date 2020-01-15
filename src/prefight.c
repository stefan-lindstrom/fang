/******************************************************************************
 * $Id: prefight.c,v 1.2 2003/06/17 10:30:05 fang Exp $
 *   
 * File: prefight.c
 *
 * Created: Mon Sep 9 2002 by Darren Benham (fischer@thepics.org)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * This file holds functions that calculate some of the variables needed to
 * fight.  This is currently used only by the consider command but could be
 * incorporated into the fight routines to have a single "goto" point so that
 * changes to the fight system need to be made in only one place.
 * 
 *   $Log: prefight.c,v $
 *   Revision 1.2  2003/06/17 10:30:05  fang
 *
 *   Removed a new set of "externs" from source files. For fucks sake people,
 *   let's keep those externs where they belong, in an apropriate header, ok?
 *
 *   Revision 1.1  2002/09/12 23:00:02  fang
 *   fischer: revamp the consider code to make it more relevent
 *
 *
 */

#include "class.h"
#include "fight.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "spells.h"
#include "guild.h"
#include "prefight.h"

#define dice_avg(a,b)   (((a)+(a)*(b))/2)

int chance_to_hit(struct char_data * ch, struct char_data * victim, int attack_skill, int type)
{
  int  victim_ac, calc_thaco, mod = 0;
  int botrh_mod = 0;
  struct char_data *leader, *k;

  if (NULL == ch)
    return 100;
  
  if (NULL == victim)
    return 0;

  if (type == SKILL_BACKSTAB) 
  {
    calc_thaco = GET_SKILL(ch,SKILL_BACKSTAB);
    calc_thaco += GET_HITROLL(ch);


//    calc_thaco += (int) ((GET_INT(ch) - 16) / 4);	/* Intelligence helps! */
//    calc_thaco += (int) ((GET_WIS(ch) - 16) / 4);	/* So does wisdom */

    victim_ac = GET_AC(victim) / 10;
    
    /* Below is BotRH guild skill Tactics, which improves victims ac */
    /* with 5 per member in the group who has Tactics                */
    if (GET_SKILL(victim, SKILL_TACTICS) && IS_AFFECTED(victim, AFF_GROUP) && !IS_NPC(ch))
    {
      leader = NULL;
      botrh_mod = -5;
      if (victim->master)
        leader = victim->master;  /* A slight sanity check to see */
      else                    /* if ch really -is- grouped    */
        if (victim->followers)
          leader = victim;
      if (leader)
        for (k = leader->followers->follower; k; k = k->next)
          if (GET_SKILL(k, SKILL_TACTICS))
            botrh_mod -= 5;
      botrh_mod = MIN(botrh_mod, 40); /* No more than 40 ac mod */
      victim_ac -= botrh_mod/10;
    }
    
    if (AWAKE(victim))
      victim_ac -= (int) ((GET_DEX(victim) - 16) / 4);

    victim_ac = 5 * MAX(-20, victim_ac);  /* -20 is lowest */

    calc_thaco += victim_ac;
    calc_thaco -= (int) ((GET_INT(victim) - 16) / 4); /* Victims int helps him */
    calc_thaco -= (int) ((GET_WIS(victim) - 16) / 4);
    if (ROOM_AFFECTED(ch->in_room, RAFF_FOG))
      calc_thaco -= 25;
    return calc_thaco;
  }
  
  if (IS_NPC(ch)) 
    return mod+75+(GET_AC(victim)/4)+GET_HITROLL(ch);
  else  
  { 
	if ( attack_skill == -1 )
	  calc_thaco = 100;
	else if (GET_SKILL(ch,attack_skill))
	  calc_thaco = GET_SKILL(ch,attack_skill);
	else
	  return 0;
	if (ROOM_AFFECTED(ch->in_room, RAFF_FOG))
	  calc_thaco -= 20;

	if (NORMAL_DREAMER(ch))    /* If dreaming, all skills are multiplied by dream skill % */
	  calc_thaco = (calc_thaco*GET_SKILL(ch, SKILL_DREAM))/100;
            
	calc_thaco += GET_HITROLL(ch);
	calc_thaco += (int) ((GET_INT(ch) - 16) / 4);	/* Intelligence helps! */
	calc_thaco += (int) ((GET_WIS(ch) - 16) / 4);	/* So does wisdom */

	victim_ac = (GET_AC(victim) / 10) - 10;
	if (GET_SKILL(victim, SKILL_INTIMIDATE) > number(0, 100))
	  calc_thaco -= 20;
      
	if (AWAKE(victim))
	  victim_ac -= (int) ((GET_DEX(victim) - 16) / 4);

	victim_ac = 5 * MAX(-20, victim_ac);  /* -20 is lowest */
	if (GET_SKILL(ch, SKILL_INTIMIDATE) > number(0, 100))
	  victim_ac += 10;
      
	calc_thaco += victim_ac;
	calc_thaco -= (int) ((GET_INT(victim) - 16) / 4); /* Victims int helps him */
	calc_thaco -= (int) ((GET_WIS(victim) - 16) / 4);
   
	  return calc_thaco;
  }
}

int  avg_damage( struct char_data *ch,struct char_data *victim, int type )
{
  int dam = 0,dam2 = 0;
  int is_dual = FALSE;
  struct obj_data *wielded  = GET_EQ(ch, WEAR_WIELD_R);
  struct obj_data *wielded2 = NULL;


  if ((wielded) && (CAN_WEAR(wielded,ITEM_WEAR_DUALWIELD)))
    is_dual = TRUE; 

  if (!wielded) 
    wielded = GET_EQ(ch,WEAR_WIELD_L);
  else 
    wielded2 = GET_EQ(ch, WEAR_WIELD_L);
  
    if ( (wielded) && (wielded2) ) {
      dam2 = (int) ((GET_STR(ch) - 16)/ 4);
      dam2 += GET_DAMROLL(ch);
      dam2 += dice_avg(GET_OBJ_VAL(wielded,1), GET_OBJ_VAL(wielded,2));
    }

    dam = (int) ((GET_STR(ch) - 16) / 4);
    dam += GET_DAMROLL(ch);
    
    if (wielded) {
      dam += dice_avg(GET_OBJ_VAL(wielded,1), GET_OBJ_VAL(wielded,2));
      if (IS_NPC(ch)) {
        dam += dice_avg(ch->mob_specials.damnodice,ch->mob_specials.damsizedice);
      }
    }
    else if (IS_NPC(ch) && !wielded) 
      dam += dice_avg(ch->mob_specials.damnodice, ch->mob_specials.damsizedice);
    else 
      dam += 1;

    if (is_dual) dam = (int) (dam * 1.75);
    
    if ((GET_SKILL(ch, SKILL_STRIKE) || GET_SKILL(ch, SKILL_DARKGIFT)
    || GET_SKILL(ch, SKILL_CRITICAL_HIT)) && (25 > number(0, 100))) dam *= 2;
    if (!IS_NPC(victim)) 
      dam = MAX(1, (dam - MIN((int)(GET_ABS(victim)/10.0),40))); 
    if ( (wielded) && (wielded2) && !IS_NPC(victim))
      dam2 = MAX(1, (dam - (int) (GET_ABS(victim)/10.0)));
    
    if (NORMAL_DREAMER(ch) && !IS_NPC(ch))     /* If Dreaming, damage is multiplied by Dreaming skill % */
    {
      dam = dam*(GET_SKILL(ch, SKILL_DREAM))/100;
      if (dam2)
        dam2 = dam2*(GET_SKILL(ch, SKILL_DREAM))/100;
    }
    if (type == SKILL_BACKSTAB) {
       if (wielded) {
        dam *= backstab_mult(GET_LEVEL(ch));
        if (get_guild(ch, 15))
          dam *= 2;
        return dam;
      }
      else {
        dam2 *= backstab_mult(GET_LEVEL(ch));
        if (get_guild(ch, 15))
          dam *= 2;
        return dam2;
      }
    }
    else {
     if ((wielded) && (wielded2) && (ch->in_room == victim->in_room)) 
       return dam2 + dam;
     return dam;
    }
  }

