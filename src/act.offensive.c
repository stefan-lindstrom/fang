/******************************************************************************
 * $Id: act.offensive.c,v 1.11 2003/12/19 20:08:04 fang Exp $
 *   
 * File: act.offensive.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: act.offensive.c,v $
 *   Revision 1.11  2003/12/19 20:08:04  fang
 *   Tiek: Fixed the bug with dirtkick and a few other offensive skills I caused.
 *
 *   Revision 1.10  2003/12/17 21:14:24  fang
 *   Subdued players can no longer be attacked.  This prevents the exploit where one can spam backstab up by attacking a subdued player.
 *
 *   Revision 1.9  2003/06/10 06:05:54  fang
 *   Fixed problem with adrenalin, spearmastery and blademastery being able to go above 100
 *
 *   Revision 1.8  2003/05/29 03:52:09  fang
 *   Can wimpy after bashing now.
 *
 *   Revision 1.7  2002/11/07 04:02:01  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.6  2002/10/08 21:12:31  fang
 *   fisch: included blockage of "BLEEDING" message in NOSPAM toggle
 *
 *   Revision 1.5  2002/09/24 03:22:01  fischer
 *   fisch: Made Immortals immune to arrows from other rooms
 *
 *   Revision 1.4  2002/09/22 08:11:16  fang
 *   Put in a delay between backstab and flee, a halving of woven shields
 *   efficiency and a full !Magic protection of mobs.
 *
 *   Revision 1.3  2002/07/28 13:56:03  fang
 *   Removed dmlloc...just to test.
 *
 *   Revision 1.2  2002/07/26 19:39:52  fang
 *   Dath: MYCOLOR mem leak fixed.  Thanks, Fred!
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.4  2002/05/27 10:52:48  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.3  2002/05/10 10:42:25  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.13  2002/02/16 07:10:20  mud
 *   It's no longer possible to flee when a mob is sitting.  Prevents certain
 *   crash situations.
 *
 *   Revision 1.12  2002/02/15 08:38:16  mud
 *   *** empty log message ***
 *
 *   Revision 1.11  2002/01/30 13:25:28  mud
 *   Bug fix with treesing guildskill...
 *
 *   Revision 1.10  2002/01/14 00:15:46  mud
 *   Score should no longer show negative amount of objs until you hit some 2*10^9 objs. :)
 *
 *   Revision 1.9  2001/08/20 13:49:46  mud
 *   *** empty log message ***
 *
 *   Revision 1.8  2001/08/11 17:46:01  mud
 *   Modified to make wardersense and warderheal obsolete and have the code that
 *   checked for those skills check to see if the character is a bonded Gaidin - Illaria
 *
 *   Revision 1.7  2001/05/28 12:56:34  mud
 *   *** empty log message ***
 *
 *   Revision 1.6  2001/02/18 13:38:47  mud
 *   *** empty log message ***
 *
 *   Revision 1.5  2000/12/11 12:14:18  mud
 *   Removed most references to PFILEPOS() get_char_ppos(), since with the xml code ppos will(may) change every reboot
 *
 *   Revision 1.4  2000/11/30 15:46:45  mud
 *   Added extra security features to make sure shopkillers shouldn't be kill'able by anyone
 *
 *   Revision 1.3  2000/11/14 04:46:38  mud
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/02/21 14:00:15  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/*
************************************************************************
*   File: act.offensive.c                               Part of CircleMUD *
*  Usage: player-level commands of an offensive nature                    *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "config.h"
#include "conf.h"
#include "sysdep.h"
#include "shop.h"
#include "act.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "spells.h"
#include "fight.h"
#include "constants.h"
#include "magic.h"

ACMD(do_aim)
{
  struct char_data *vict;
  struct obj_data *wielded;
  int dam = 0;
  
  if (!GET_SKILL(ch, SKILL_AIM))
  {
    send_to_char(" You!? Aim!? Hah! You can't hit a barn wall from 10 feet away!\r\n", ch);
    return;
  }

  if (ROOM_FLAGGED(ch->in_room, ROOM_PEACEFUL)) {
    send_to_char("Uh uh. Not here. Give it some rest, pal.\r\n", ch);
    return;
  }

  one_argument(argument, arg);
  if ((!*arg) || (!(vict = get_char_room_vis(ch, arg))))
  {
    send_to_char("Aim at whom? \r\n", ch);
    return;
  }
  if (!(wielded = GET_EQ(ch, WEAR_WIELD_L)))
    if (!(wielded = GET_EQ(ch, WEAR_WIELD_R)))
    {
      send_to_char("You have to have a weapon to aim at someone!\r\n", ch);
      return;
    }
  if (number(1, 100) < GET_SKILL(ch, SKILL_AIM))
  {
    dam = dice(GET_OBJ_VAL(wielded,1), GET_OBJ_VAL(wielded, 2));
    if (number(1,100) < 4)
      if (GET_SKILL(ch, SKILL_AIM) < 100)
        ADD_SKILL(ch, SKILL_AIM, 1);   
  }
  dam *= 3;
  damage(ch, vict, dam, SKILL_AIM); 
}


ACMD(do_free)
{
  struct char_data *victim;
  static struct affected_type *af,*next;

  int is_the_right_person=FALSE;

  one_argument(argument, arg);
  if ((!*arg) || (!(victim = get_char_room_vis(ch, arg))))
    {
    send_to_char("Release whom?",ch);
    return;
    }
  
  if (!IS_AFFECTED2(victim,AFF2_SUBDUED))
    {
    send_to_char("That person is no one's prisoner!",ch);
    return;
    }

  if (IS_AFFECTED2(victim, AFF2_SUBDUED)) 
    {
    for (af = victim->affected; af; af = next) 
      {
      next = af->next;
      if (af->weaver == GET_IDNUM(ch))
         is_the_right_person=TRUE;
      }

    if (is_the_right_person)
      {
      affect_from_char(victim, SKILL_ARREST);
      REMOVE_BIT(AFF_FLAGS(victim), AFF2_ARRESTED);
      send_to_char("You release your prisoner.\r\n", ch);
      act("You watch as $n releases a prisoner.",FALSE,ch,NULL,NULL,TO_ROOM);
      act("You have been released by $n!", TRUE, ch, 0, victim, TO_VICT);
      return;
      }
    else
      {
      send_to_char("That person is not your prisoner to free!\n\r",ch);
      return;
      }

   }
}


ACMD(do_subdue)
{
   if (!GET_SUBDUE(ch))
     {
     send_to_char("You will now capture your opponents alive in fights.\r\n",ch);
     GET_SUBDUE(ch)=TRUE;
     }
   else
   if (GET_SUBDUE(ch))
     {
     send_to_char("You will now kill in fights.\r\n",ch);
     GET_SUBDUE(ch)=FALSE;
     }
}

ACMD(do_assist)
{
  struct char_data *helpee, *opponent;

  if (FIGHTING(ch)) {
    send_to_char("You are already fighting!  How can you assist someone else?\r\n", ch);
    return;
  }
  one_argument(argument, arg);

  if (!*arg)
    send_to_char("Whom do you wish to assist?\r\n", ch);
  else if (!(helpee = get_char_room_vis(ch, arg)))
    send_to_char(NOPERSON, ch);
  else if (helpee == ch)
    send_to_char("You cannot help yourself any more than this!\r\n", ch);
  else {
    for (opponent = world[ch->in_room].people;
     opponent && (FIGHTING(opponent) != helpee);
     opponent = opponent->next_in_room)
        ;

    if (!opponent)
      act("But nobody is fighting $M!", FALSE, ch, 0, helpee, TO_CHAR);
    else if (!CAN_SEE(ch, opponent))
      act("You cannot see who is fighting $M!", FALSE, ch, 0, helpee, TO_CHAR);
    else if (!pk_allowed && !IS_NPC(opponent))	/* prevent accidental pkill */
      act("Use \\c01murder\\c00 if you really want to attack $N.", FALSE,
      ch, 0, opponent, TO_CHAR);
    else {
      send_to_char("You join the fight!\r\n", ch);
      act("$N assists you!", 0, helpee, 0, ch, TO_CHAR);
      act("$n assists $N.", FALSE, ch, 0, helpee, TO_NOTVICT);
      hit(ch, opponent, TYPE_UNDEFINED);
    }
  }
}


ACMD(do_flank)
{
  struct char_data *vict;
  int dam = 0, mprog = 0;
  if (!GET_SKILL(ch, SKILL_FLANK))
  {
    send_to_char("You cannot even flank a dead cow!\r\n", ch);
    return;
  }

  if (ROOM_FLAGGED(ch->in_room, ROOM_PEACEFUL)) {
    send_to_char("Uh uh. Not here. Give it some rest, pal.\r\n", ch);
    return;
  }

 if (FIGHTING(ch))
  {
    send_to_char("Sorry, mate. It is a bit hard to flank someone while fighting.\r\n", ch);
    return;
  }
  one_argument(argument, arg);
  if (!*arg)
    send_to_char("Flank whom, really? \r\n", ch);
  else if (!(vict = get_char_room_vis(ch, arg)))
    send_to_char("Cannot really see that person here right now.\r\n", ch);
  else
  {
    mprog = check_for_mobprogs_and_stops(ch, vict, NULL, MPC_ATTACK, 0, 0);
    if (mprog != 2) {
      if (number(0,100) < GET_SKILL(ch, SKILL_FLANK))
      {
        dam = 4 * number(1, 10);
        GET_POS(vict) = POS_SITTING;
        WAIT_STATE(vict, PULSE_VIOLENCE * 3);
        damage(ch, vict, dam, SKILL_FLANK);       
        if ((number(0,100) < 4) && (GET_SKILL(ch, SKILL_FLANK) < 100)) {
          ADD_SKILL(ch, SKILL_FLANK, 1);
          send_to_char("Man, what a nice move. You learn something from it too!\r\n", ch);
        }
      }
      else
        damage(ch, vict, 0, SKILL_FLANK);
    }
    if (mprog)
      run_mobprog(ch, 0);
  } 
}


ACMD(do_hit)
{
  struct char_data *vict;
  struct affected_type af;
  int mprog = 0;

  af = clear_aff_var(af);
  one_argument(argument, arg);
  

  if (!*arg)
    send_to_char("Hit who?\r\n", ch);
  else if (!(vict = get_char_room_vis(ch, arg)))
    send_to_char("They do not seem to be here.\r\n", ch);
  else if (vict == ch) 
  {
    send_to_char("You hit yourself...OUCH!.\r\n", ch);
    act("$n hits $mself, and says OUCH!", FALSE, ch, 0, vict, TO_ROOM);
  } else if (IS_AFFECTED(ch, AFF_CHARM) && (ch->master == vict))
    act("$N is just such a good friend, you simply cannot hit $M.", FALSE, ch, 0, vict, TO_CHAR);
  else 
  {
    if (1) { /* (!pk_allowed) removed here by Fredrik 970815 */
      if (!IS_NPC(vict) && !IS_NPC(ch) && subcmd == SCMD_MURDER && IS_AFFECTED2(vict, AFF2_SUBDUED)){
	send_to_char("You can't attack someone who is subdued.\r\n", ch);
	return;
      }
      if (!IS_NPC(vict) && !IS_NPC(ch) && subcmd == SCMD_MURDER && IS_AFFECTED2(ch, AFF2_SUBDUED)  ) {
        send_to_char("You can't attack while subdued.\r\n", ch);
        return;
      } else  
      if (!IS_NPC(vict) && !IS_NPC(ch) && (subcmd != SCMD_MURDER)) {
    send_to_char("Use \\c01murder\\c00 to hit another player.\r\n", ch);
    return;
      }
      if (IS_AFFECTED(ch, AFF_CHARM) && !IS_NPC(ch->master) && !IS_NPC(vict))
    return;			/* you can't order a charmed pet to attack a
                 * player */
    }
    if (ROOM_FLAGGED(ch->in_room, ROOM_PEACEFUL))
    {
      send_to_char("Uh uh. Not here. Give it some rest, pal.\r\n", ch);
      return;
    }
    mprog = check_for_mobprogs_and_stops(ch, vict, NULL, MPC_ATTACK, 0, 0);
    if (mprog != 2) {
      if ((GET_POS(ch) >= POS_STANDING) && (vict != FIGHTING(ch))) 
      {
        hit(ch, vict, TYPE_UNDEFINED);
        WAIT_STATE(ch, PULSE_VIOLENCE + 2);
        af.duration  = 10;
        af.bitvector = AFF_NOQUIT;
        af.bitvector2 = af.bitvector3 = 0;
        af.location = 0;
        af.modifier = 0;
        af.type = 0;
        
        if (!IS_NPC(vict))
          affect_to_char(ch,&af); /* Set !QUIT for 10 MUDhrs if PKilling */     
      } else
        send_to_char("You do the best you can!\r\n", ch);
    }
    if (mprog)
      run_mobprog(ch, 0);
  }
}


ACMD(do_kill)
{
  struct char_data *vict;

  if ((GET_LEVEL(ch) < LVL_IMPL) || IS_NPC(ch)) {
    do_hit(ch, argument, cmd, subcmd);
    return;
  }
  one_argument(argument, arg);

  if (!*arg) {
    send_to_char("Kill whom?\r\n", ch);
  } else {
    if (!(vict = get_char_room_vis(ch, arg)))
      send_to_char("They aren't here.\r\n", ch);
    else if (ch == vict)
      send_to_char("Your mother would be so sad.. :(\r\n", ch);
    else {
      act("You chop $M to pieces!  Ah!  The blood!", FALSE, ch, 0, vict, TO_CHAR);
      act("$N chops you to pieces!", FALSE, vict, 0, ch, TO_CHAR);
      act("$n brutally slays $N!", FALSE, ch, 0, vict, TO_NOTVICT);
      if(!GET_SUBDUE(ch) || IS_NPC(vict))
         raw_kill(vict, ch);
    }
  }
}



ACMD(do_backstab)
{
  struct char_data *vict;
  int percent, mprog = 0;
  struct obj_data *wpn;

  one_argument(argument, buf);

  if (!*arg || !(vict = get_char_room_vis(ch, buf))) {
    send_to_char("Backstab whom?\r\n", ch);
    return;
  }
  
  if (vict == ch) {
    send_to_char("How can you sneak up on yourself?\r\n", ch);
    return;
  }
  if (ROOM_FLAGGED(ch->in_room, ROOM_PEACEFUL)) {
    send_to_char("Now where did you get that idea from? This is a peaceful room.\r\n", ch);
    return;
  }
  if (!GET_EQ(ch, WEAR_WIELD_R) && !GET_EQ(ch, WEAR_WIELD_L)) {
    send_to_char("You need to wield a weapon to make it a success.\r\n", ch);
    return;
  }
  if (!IS_NPC(ch) && !IS_NPC(vict) && IS_AFFECTED2(vict, AFF2_SUBDUED)){
    send_to_char("You cannot attack someone who is subdued.\r\n", ch);
    return;
  }
  
  if (!ok_damage_shopkeeper(ch, vict)) {
    send_to_char(" The shopkeeper merely laughs at your attempt to backstab.\r\n", ch);
    act("$n tries to backstab $N, but $E merely laughs at $m.", TRUE, ch, 0, vict, TO_NOTVICT);
    return;
  }

  wpn = ((GET_EQ(ch,WEAR_WIELD_R) == NULL) ? GET_EQ(ch,WEAR_WIELD_L) : GET_EQ(ch,WEAR_WIELD_R));

  if ((GET_OBJ_VAL(wpn,3) != (TYPE_PIERCE - TYPE_HIT)) && (GET_OBJ_VAL(wpn,3) != (TYPE_STAB - TYPE_HIT))) {
    send_to_char("Only piercing/stabbing weapons can be used for backstabbing.\r\n", ch);
    return;
  }
  if (IS_AFFECTED2(vict, AFF2_AWARE)) {
    send_to_char("You try to sneak up on them, but they see you coming.\r\n", ch);
    return;
  }  
  if (FIGHTING(vict)) {
    send_to_char("You can't backstab a fighting person -- they are too alert!\r\n", ch);
    return;
  }
  mprog = check_for_mobprogs_and_stops(ch, vict, NULL, MPC_ATTACK, 0, 0);
  if (mprog != 2) {
    if (MOB_FLAGGED(vict, MOB_AWARE)) {
      act("You notice $N lunging at you!", FALSE, vict, 0, ch, TO_CHAR);
      act("$e notices you lunging at $m!", FALSE, vict, 0, ch, TO_VICT);
      act("$n notices $N lunging at $m!", FALSE, vict, 0, ch, TO_NOTVICT);
      hit(vict, ch, TYPE_UNDEFINED);
      return;
    }

    if (GET_LEVEL(vict) >= LVL_IMMORT) {
      damage(ch,vict,0,SKILL_BACKSTAB);
      return;
    }
    if (!IS_NPC(ch))
      percent = generic_result(ch,SKILL_BACKSTAB,NULL,FALSE);
    else 
      if (number(0,100) < (75+number(0,(GET_LEVEL(ch)/10))))
        percent = SOME_SUCCESS;
      else
        percent = TOTAL_FAILURE;

    if (IS_AFFECTED2(ch, AFF2_UNBALANCE))
      percent = TOTAL_FUCKUP;
    if (GET_POS(vict) == POS_SLEEPING)
      percent = SOME_SUCCESS;
  
     switch (percent) {
      case TOTAL_FUCKUP:
        hit(vict,ch,SKILL_BACKSTAB);
        break;
      case TOTAL_FAILURE:
        damage(ch,vict,0,SKILL_BACKSTAB);
        break;
      case SOME_SUCCESS:
        hit(ch, vict, SKILL_BACKSTAB);
        break;
      case TOTAL_SUCCESS: 
        hit(ch, vict, SKILL_BACKSTAB);
        break;
    }
     SET_TIMES(ch, CAN_FLEE,  (10 * PULSE_VIOLENCE));
  }
  if (mprog)
    run_mobprog(ch, 0);
}


ACMD(do_breakshield)
{
  struct char_data  *weaver;
  char	tmp[MAX_STRING_LENGTH];
  int result = 0, chance = 0, found = 0;
  int vstr, wstr;
  struct affected_type *tweave = NULL, *af;    

  if (GET_TIMES(ch,CAN_BREAK)) 
  {
    send_to_char("You are already exhausted from your last attempt to break the shield!",ch);
    return;
  }   

  // Character can't even channel.
  if (cant_channel(ch)) 
  {
    send_to_char("But you can't channel, how can you be shielded?\r\n",ch);
    return;
  }   
    
  // Character is already grasping, thus can't be shielded.
  if (!IS_AFFECTED(ch,AFF_SHIELDED)) 
  {
    send_to_char("But you are not shielded as it is!\r\n",ch);
    return;
  }
      
  // Character is Greyman.
  if (IS_GREYMAN(ch))
  {
    send_to_char("Go away! The One Power can only be wielded by the living!\r\n", ch);
    return;
  }
  
  // Character doesn't know how to grasp. Doh.
  if (!GET_SKILL(ch,SKILL_GRASP))
  {
    send_to_char("You don't know how to even grasp, let alone break the shield!\r\n", ch);
    return;
  }
       
  // Character is in a !magic room.
  if (ROOM_FLAGGED(ch->in_room,ROOM_NOMAGIC))
  {
    send_to_char("You can't even sense the One Power here, let alone grasp it!",ch);
    return;
  }
      
  // Character is gentled/stilled permanently.
  if (IS_SET(GET_PERMANENT(ch),PRM_GENTLED))
  {
    send_to_char("You are cut off from the One Power, for good!\r\n",ch);
    return;
  }

     
  /* Character is near-exhausted. This can be removed for desparate
   attempts to be made, if needed.  */

  if (GET_MANA(ch) <= 20)
  {
    send_to_char("You better rest for a while before trying this.\r\n",ch);
    return;
  }

  /* Find the weave */
  for (af = ch->affected; (af && !found); af = af->next)
  {
    if (af->type == SPELL_SHIELD)
    {  
      found = TRUE;
      tweave = af;
    }
  }

  // If it's not found, tell them otherwide get the weaver.
  if (!found) 
  {
    send_to_char("You cant sense that weave on your target!\r\n",ch);
    return;
  }
  else
    weaver = get_char_id(tweave->weaver);
    
  /* The result is handled seperately for easier checking.
   First priority is the difference in Spirit, second priority the mana 
   difference, third the level difference on a scale of 1:3, then a number
   of subtle factors. Note than when char is shielded by linked opponents, 
   their total mana increase, so it further explains the reason for the mana 
   checks without a check specifically for linked shielders. */
  //changed from spirit to total strength
  vstr = GET_SPIRIT( ch ) + GET_FIRE( ch ) + GET_EARTH( ch ) + GET_WATER( ch ) + GET_AIR( ch );
  
  if (NULL != weaver) 
  {
    wstr = GET_SPIRIT( weaver ) + GET_FIRE( weaver ) + GET_EARTH( weaver ) + GET_WATER( weaver ) + GET_AIR( weaver );
    result += vstr - wstr;
    result += (int)( (GET_MAX_MANA(ch) - GET_MAX_MANA(weaver))/200);
  }
  else /* No weaver, do a nice random effect. */
    result += vstr - (number(1,20)+number(1,20)+number(1,20)+number(1,20)+number(1,20));

  /* If a male channeler is breaking a female's shield, add a very small  bonus. */  
  if ((NULL != weaver) && (GET_SEX(ch)==SEX_MALE) && (GET_SEX(weaver)==SEX_FEMALE))
      result += number(0,5);

  // If it is, increase the chance of the guy breaking the shield. 
  if ( !(tweave->tie_info) && (found) )
     result += 15 + number(0,10);

  // Add skill for BREAKWSHIELD
  switch( generic_result( ch, SKILL_BREAKSHIELD, tmp, 0 ) )
  {
    case TOTAL_SUCCESS:
      result+=GET_SKILL( ch, SKILL_BREAKSHIELD ) /2;
      send_to_char( tmp, ch );
      send_to_char( "\r\n", ch );
      break;
    case SOME_SUCCESS:
      result+= GET_SKILL( ch, SKILL_BREAKSHIELD )/4;
      break;
  }

  // Calculate the chances of success..
  chance = number(0,100);
  
// if (weaver) {
// sprintf(buf, "You are shielded by %s and your chances are %d\r\n", GET_NAME(weaver), result);
// send_to_char(buf,ch);
// } Debuging-stuff. 

// Did it succeed? If so, remove the bloody shield!

  if ( chance <= result )
  {
    send_to_char("You slam on the shield from the True Source with full force, breaking it!\r\n", ch);
    SET_TIMES(ch,CAN_BREAK, (PULSE_VIOLENCE * 14));
    if (weaver && (SCMD_MAINTAINED == tweave->tie_info)) 
      asend_to_char(weaver,"%s slams in to your shield, shattering it...\r\n",GET_NAME(ch));
    affect_from_char(ch,SPELL_SHIELD);
    do_grasp(ch,"",0,0);
  }
  else
  {
    send_to_char("You slam and slap against the shield with no results.. its too strong.\r\n",ch);
    SET_TIMES(ch,CAN_BREAK, (PULSE_VIOLENCE * 14));
    if (weaver && (SCMD_MAINTAINED == tweave->tie_info))
      asend_to_char(weaver,"%s slams into your shield, but fails to shatter it...\r\n",GET_NAME(ch));
  }
}

    



ACMD(do_stab)
{
  struct char_data *vict = NULL;
  int percent, mprog = 0;
  int dam;

  skip_spaces(&argument);

  if (!GET_SKILL(ch, SKILL_STAB)){
    send_to_char("Huh?!?\r\n",ch);
    return;
  }
  if (ROOM_FLAGGED(ch->in_room,ROOM_PEACEFUL)) {
    send_to_char("Hey! This is a peaceful room!\r\n",ch);
    return;
  }

  vict = get_char_room_vis(ch, argument);

  if  (!*argument || !(vict)) {
    if (FIGHTING(ch)) {
      vict = FIGHTING(ch);
    }
    else {
      send_to_char("Stab whom?\r\n",ch);
      return;
    }
  }

  // Do we need an extra sanity check here? *shrug* Actually, we shouldn't
  // But what the heck. :)

  if (NULL == vict) {
    send_to_char("Stab whom?\r\n",ch);
    return;
  }

  if (vict == ch)
  {
    send_to_char("You stab yourself in the side.\r\n", ch);
    send_to_char("Ouch!  Dammit!!! You stabbed yourself in the ribs!\r\n",  ch);
    act("$n stabs himself in the side.",FALSE,ch,0,0,TO_ROOM);
    return;
  }

  if (!GET_EQ(ch, WEAR_WIELD_R) && !GET_EQ(ch, WEAR_WIELD_L)) {
    send_to_char("You need to wield a weapon to make it a success.\r\n", ch);
    return;
  }
  
  if (GET_EQ(ch,WEAR_WIELD_R) && (GET_OBJ_VAL(GET_EQ(ch,WEAR_WIELD_R),3) != (TYPE_PIERCE - TYPE_HIT))) {
    if (GET_EQ(ch,WEAR_WIELD_L) && (GET_OBJ_VAL(GET_EQ(ch,WEAR_WIELD_L),3) != (TYPE_PIERCE - TYPE_HIT))) {
      send_to_char("Only piercing weapons can be used for stabbing.\r\n", ch);
      return;
    }
  }

  if (GET_TIMES(ch,CAN_STAB)) {
    send_to_char("You haven't regained your balance since your last stab!\r\n",ch);
    return;
  }

  dam  = dice(GET_OBJ_VAL(GET_EQ(ch,WEAR_WIELD_R),1),GET_OBJ_VAL(GET_EQ(ch,WEAR_WIELD_R),2));
  dam += GET_DAMROLL(ch);
  dam  = (int) (dam * 1.5);

  if (NULL == vict) {
    send_to_char("Stab whom?\r\n",ch);
    return;
  }


  mprog = check_for_mobprogs_and_stops(ch, vict, NULL, MPC_ATTACK, 0, 0);
  if (mprog != 2) {
    if (GET_LEVEL(vict) >= LVL_IMMORT) {
      damage(ch,vict,0,SKILL_STAB);
      return;
    }

    percent = generic_result(ch,SKILL_STAB,NULL,FALSE);
   
     switch (percent) {
      case TOTAL_FUCKUP:
        damage(vict,ch,dam,SKILL_STAB);
        SET_TIMES(ch,CAN_STAB, (4 * PULSE_VIOLENCE));
        break;
      case TOTAL_FAILURE:
        damage(ch,vict,0,SKILL_STAB);
        SET_TIMES(ch,CAN_STAB,  (3 * PULSE_VIOLENCE));
        break;
      case SOME_SUCCESS:
        damage(ch, vict, dam,SKILL_STAB);
        SET_TIMES(ch,CAN_STAB, (3 * PULSE_VIOLENCE));
        break;
      case TOTAL_SUCCESS: 
        damage(ch, vict, dam,SKILL_STAB);
        SET_TIMES(ch,CAN_STAB, (2 * PULSE_VIOLENCE));
        break;
    }
  }
  if (mprog)
    run_mobprog(ch, 0);
}


ACMD(do_order)
{
  char name[100], message[256];
  char buf[256];
  bool found = FALSE;
  int org_room;
  struct char_data *vict = NULL;
  struct follow_type *k;

  half_chop(argument, name, message);

  if (!*name || !*message)
    send_to_char("Order who to do what?\r\n", ch);
  else if (!(vict = get_char_room_vis(ch, name)) && !is_abbrev(name, "followers"))
    send_to_char("That person is not here.\r\n", ch);
  else if (ch == vict)
    send_to_char("You obviously suffer from schitzofrenia.\r\n", ch);
  else 
  {
    if (vict && (IS_AFFECTED2(ch, AFF2_COMPULSED) ||  IS_AFFECTED(ch,AFF_CHARM))) {
      send_to_char("Your superior would not aprove of you giving orders.\r\n", ch);
      return;
    }
    
    if (vict) {    
      sprintf(buf, "$N orders you to '%s'", message);
      act(buf, FALSE, vict, 0, ch, TO_CHAR);
      if ((vict->master != ch) || (!IS_AFFECTED2(vict, AFF2_COMPULSED) && !IS_AFFECTED(vict,AFF_CHARM)))
    act("$n has an indifferent look.", FALSE, vict, 0, 0, TO_ROOM);
      else 
      {
        if (IS_AFFECTED2(vict,AFF_CHARM)) {
      send_to_char(OK, ch);
          REMOVE_BIT(AFF2_FLAGS(vict),AFF2_COMPULSED);
      command_interpreter(vict, message);
          SET_BIT(AFF2_FLAGS(vict),AFF2_COMPULSED);
        }else{
          send_to_char(OK,ch);
          command_interpreter(vict,message);
        }
      }
    } 
    else {			/* This is order "followers" */

      org_room = ch->in_room;

      for (k = ch->followers; k; k = k->next) {
    if (org_room == k->follower->in_room) {
      if (IS_AFFECTED2(k->follower, AFF2_COMPULSED)) 
      {
        found = TRUE;
            REMOVE_BIT(AFF2_FLAGS(k->follower),AFF2_COMPULSED);
        command_interpreter(k->follower, message);
            SET_BIT(AFF2_FLAGS(k->follower),AFF2_COMPULSED);
      }
          else if (IS_AFFECTED(k->follower,AFF_CHARM)) {
            found = TRUE;
        command_interpreter(k->follower,message);
          }
        }
      }
      if (found)
    send_to_char(OK, ch);
      else
    send_to_char("Nobody here is a loyal subject of yours!\r\n", ch);
    }
  }
}



ACMD(do_flee)
{
  int i, attempt;

  if (IS_AFFECTED2(ch,AFF2_NOFLEE)||IS_AFFECTED2(ch,AFF2_RAGE)) {
    return;
  }

  if (!FIGHTING(ch) && !IS_NPC(ch)) {
    send_to_char("Flee from what? You're not fighting.\r\n", ch);
    return;
  }

  if (GET_TIMES(ch, CAN_FLEE)) 
  {
    send_to_char("You can't, yet! Stand and fight!\r\n", ch);
    return;
  }

  if (FIGHTING(ch) &&  (GET_POS(FIGHTING(ch)) < POS_FIGHTING) && (subcmd == SCMD_FLEE)) {
    send_to_char("Why flee? You've got him on the ground now!\r\n",ch);
    return;
  }

  act("$n panics, and attempts to flee!", TRUE, ch, 0, 0, TO_ROOM);
  for (i = 0; (i < 6) && FIGHTING(ch); i++) {
    attempt = number(0, NUM_OF_DIRS - 1);	/* Select a random direction */
    if (CAN_GO(ch, attempt)) {
      if (IS_AFFECTED2(ch,AFF2_DEFENSE)) 
        remove_defense(ch);      
      if (do_simple_move(ch, attempt, TRUE)) {
        send_to_char("You flee head over heels.\r\n", ch);
        if (FIGHTING(ch)) {
          if (!IS_NPC(ch)) 
            gain_exp(ch, -20,ch);
          if (FIGHTING(FIGHTING(ch)) == ch)
            stop_fighting(FIGHTING(ch));
          stop_fighting(ch);        
        }
    return;
      } else {
        act("$n tries to flee, but does not manage to!", TRUE, ch, 0, 0, TO_ROOM);
        return;
      }
    }
  }
  send_to_char("PANIC!  You couldn't escape!\r\n", ch);
}



ACMD(do_retreat)
{
  int attempt;
  
  one_argument(argument, arg);

  if (!GET_SKILL(ch, SKILL_RETREAT))
  {
    send_to_char("Maybe fleeing would be more prudent, since you don't know how to retreat.\r\n", ch);
    return;
  }

  if (!*arg)
  {
    send_to_char("Which direction would you like to retreat?\r\n", ch);
    return;
  }

  for (attempt = 0;(strncasecmp(arg, dirs[attempt], strlen(arg)) && (attempt < NUM_OF_DIRS)); attempt++);

  if (attempt == NUM_OF_DIRS)
    send_to_char("You want to go which way!?\r\n", ch);
  else
  {
    if ((number (0, 100) < 3) && (GET_SKILL(ch, SKILL_RETREAT) < 100))
    {
      send_to_char("You have become better at retreating!/r/n", ch);
      ADD_SKILL(ch, SKILL_RETREAT, 1);
    }

    if ((number (0, 101) > GET_SKILL(ch, SKILL_RETREAT)) || (!CAN_GO(ch, attempt)))
    {
      send_to_char("You attempt to retreat, but are intercepted.\r\n", ch);
      if (number(0, 85) > GET_SKILL(ch, SKILL_RETREAT))
      {
        send_to_char("The unexpected attack causes you to lose your footing!\r\n", ch);
        GET_POS(ch) = POS_SITTING;
      }
    }
    else
    {
      act("$n withdraws from combat and attempts a retreat.", TRUE, ch, 0, 0, TO_ROOM);
    
      if (do_simple_move(ch, attempt, TRUE))
      {
        send_to_char("You cautiously and tactfully retreat from combat.\r\n", ch);

        if (FIGHTING(ch))
        {
          if (FIGHTING(FIGHTING(ch)) == ch)
            stop_fighting(FIGHTING(ch));
          stop_fighting(ch);
        }
      }
    }
  }
  return;
}



ACMD(do_bash)
{
  struct char_data *vict;
  int percent, mprog = 0;
  int mod = 100,old = 0;

  one_argument(argument, arg);

  if (ROOM_FLAGGED(ch->in_room,ROOM_PEACEFUL)) {
    send_to_char("Hey! This is a peaceful room!\r\n",ch);
    return;
  }

  if (!*arg || !(vict = get_char_room_vis(ch, arg))) {
    if (FIGHTING(ch)) {
      vict = FIGHTING(ch);
    } else {
      send_to_char("Bash whom?\r\n", ch);
      return;
    }
  }
  if (vict == ch) {
    send_to_char("Aren't we funny today...\r\n", ch);
    return;
  }
  if (!IS_NPC(ch) && !IS_NPC(vict) && IS_AFFECTED2(vict, AFF2_SUBDUED)){
    send_to_char("You can't attack someone who is subdued.\r\n", ch);
    return;
  }
  
  if (!GET_EQ(ch, WEAR_SHIELD)) {
    mod = number(50,75);
  }

  old = GET_SKILL(ch,SKILL_BASH);
  SET_SKILL(ch,SKILL_BASH, (int)(old * (double)(mod/100.0)));

  if (GET_TIMES(ch,CAN_BASH)) {
    send_to_char("You haven't regained your balance since your last bash!",ch);
    SET_SKILL(ch, SKILL_BASH, old);
    return;
  }
  mprog = check_for_mobprogs_and_stops(ch, vict, NULL, MPC_ATTACK, 0, 0);
  if (mprog != 2) {
    if (GET_LEVEL(vict) >= LVL_IMMORT) {
      damage(ch,vict,0,SKILL_BASH);
      SET_SKILL(ch,SKILL_BASH,  old);
      return;
    }
    if (!IS_NPC(ch))
      percent = generic_result_mod(ch,SKILL_BASH,NULL,FALSE,-(GET_AC(vict) / 5));
    else
      if (number(0,100) < (75+number(0,(GET_LEVEL(ch) / 10))))
         percent = SOME_SUCCESS;
      else
         percent = TOTAL_FAILURE;
    if ((IS_NPC(vict) && MOB_FLAGGED(vict, MOB_NOBASH)) || 
         IS_AFFECTED2(ch, AFF2_UNBALANCE))
      percent = TOTAL_FAILURE;
  
    if (!IS_NPC(vict) && IS_AFFECTED2(vict,AFF2_TRACTION)) {
      if (((SOME_SUCCESS == percent)||(TOTAL_SUCCESS == percent)) && (number(0,100) < 50)) {
        percent = TOTAL_FAILURE;
      }
    }
      
    switch (percent) {
      case TOTAL_FUCKUP:
        damage(vict,ch,10,SKILL_BASH);
        GET_POS(ch) = POS_SITTING;
        SET_TIMES(ch,CAN_BASH, (PULSE_VIOLENCE * 6));
        break;
      case TOTAL_FAILURE:
        damage(ch, vict, 0, SKILL_BASH);
        GET_POS(ch) = POS_SITTING;
        SET_TIMES(ch,CAN_BASH, (PULSE_VIOLENCE * 4));
        break;
      case SOME_SUCCESS:
        damage(ch,vict,1,SKILL_BASH);
        GET_POS(vict) = POS_SITTING;
        SET_TIMES(ch,CAN_BASH, (PULSE_VIOLENCE * 4));

        if (IS_NPC(vict))
          GET_MOB_WAIT(vict) += PULSE_VIOLENCE;

        break;
      case TOTAL_SUCCESS:
        damage(ch,vict,50,SKILL_BASH);
        GET_POS(vict) = POS_SITTING;
        SET_TIMES(ch,CAN_BASH,  (2*PULSE_VIOLENCE));

        if (IS_NPC(vict))
          GET_MOB_WAIT(vict) += 2 * PULSE_VIOLENCE;

        break;
    }
  }
  SET_SKILL(ch,SKILL_BASH, old);
  /* Gotta check for increases when the "real" Bash%age is back in.*sigh* */
  if (!IS_NPC(ch))
    percent = generic_result_mod(ch,SKILL_BASH, NULL, FALSE, -(GET_AC(vict) / 5));

  if (mprog)
    run_mobprog(ch, 0);
}


ACMD(do_rescue)
{
  struct char_data *vict, *tmp_ch;
  int percent;

  one_argument(argument, arg);

  if (!(vict = get_char_room_vis(ch, arg))) {
    send_to_char("Whom do you want to rescue?\r\n", ch);
    return;
  }
  if (vict == ch) {
    send_to_char("What about fleeing instead?\r\n", ch);
    return;
  }
  if (FIGHTING(ch) == vict) {
    send_to_char("How can you rescue someone you are trying to kill?\r\n", ch);
    return;
  }
  for (tmp_ch = world[ch->in_room].people; tmp_ch &&
       (FIGHTING(tmp_ch) != vict); tmp_ch = tmp_ch->next_in_room);

  if (!tmp_ch) {
    act("But nobody is fighting $M!", FALSE, ch, 0, vict, TO_CHAR);
    return;
  }
  else {
    if (IS_NPC(ch))
      percent = SOME_SUCCESS;
    else
      percent= generic_result_mod(ch,SKILL_RESCUE,NULL,FALSE,(GET_DEX(ch) - 15));	
    
    if ((percent == TOTAL_FUCKUP) || (percent == TOTAL_FAILURE)) {
      send_to_char("You fail the rescue!\r\n", ch);
      return;
    }
    send_to_char("Banzai!  To the rescue...\r\n", ch);
    act("You are rescued by $N, you are confused!", FALSE, vict, 0, ch, TO_CHAR);
    act("$n heroically rescues $N!", FALSE, ch, 0, vict, TO_NOTVICT);

    if (FIGHTING(vict) == tmp_ch)
      stop_fighting(vict);
    if (FIGHTING(tmp_ch))
      stop_fighting(tmp_ch);
    if (FIGHTING(ch))
      stop_fighting(ch);

    set_fighting(ch, tmp_ch);
    set_fighting(tmp_ch, ch);

    WAIT_STATE(vict, 2 * PULSE_VIOLENCE);
  }

}



ACMD(do_kick)
{
  struct char_data *vict;
  int percent, mprog = 0;

  one_argument(argument, arg);

  if (ROOM_FLAGGED(ch->in_room,ROOM_PEACEFUL)) {
    send_to_char("Sigh....This -IS- a peaceful room, y'know...\r\n",ch);
    return;
  }

  if (!*arg || !(vict = get_char_room_vis(ch, arg))) {
    if (FIGHTING(ch)) {
      vict = FIGHTING(ch);
    } else {
      send_to_char("Kick whom?\r\n", ch);
      return;
    }
  }
  if (vict == ch) {
    send_to_char("Aren't we funny today...\r\n", ch);
    return;
  }
  
  if (!IS_NPC(ch) && !IS_NPC(vict) && IS_AFFECTED2(vict, AFF2_SUBDUED)) {
    send_to_char("You can't attack someone who is subdued.\r\n", ch);
    return;
  }
  
  if (GET_TIMES(ch,CAN_KICK)) {
    send_to_char("You haven't regained your balance since your last kick!",ch);
    return;
  }
  mprog = check_for_mobprogs_and_stops(ch, vict, NULL, MPC_ATTACK, 0, 0);
  if (mprog != 2) {
    if (GET_LEVEL(vict) >= LVL_IMMORT) {
      damage(ch,vict,0,SKILL_KICK);
      return;
    }
    if (!IS_NPC(ch))
      percent = generic_result_mod(ch,SKILL_KICK,NULL,FALSE,-(GET_AC(vict) / 5));
    else
     if (number(0,100) < (75+number(0,(GET_LEVEL(ch)/10))))
       percent = SOME_SUCCESS;
     else
       percent = TOTAL_FAILURE;

    switch (percent) {
      case TOTAL_FUCKUP:
        damage(vict,ch,GET_LEVEL(vict),SKILL_KICK);
        SET_TIMES(ch,CAN_KICK, (PULSE_VIOLENCE * 6));
        break;
      case TOTAL_FAILURE:
        damage(ch, vict, 0, SKILL_KICK);
        SET_TIMES(ch,CAN_KICK, (PULSE_VIOLENCE * 4));
        break;
      case SOME_SUCCESS:
        damage(ch, vict, GET_LEVEL(ch) , SKILL_KICK);
        SET_TIMES(ch,CAN_KICK, (PULSE_VIOLENCE * 4));
        break;
      case TOTAL_SUCCESS:
        damage(ch, vict, GET_LEVEL(ch) << 1, SKILL_KICK);
        SET_TIMES(ch,CAN_KICK, (2*PULSE_VIOLENCE));
        break;
    }
  }
  if (mprog)
    run_mobprog(ch, 0);
}

ACMD(do_punch)
{
  struct char_data *vict;
  int percent, mprog = 0;

  one_argument(argument, arg);

  if (ROOM_FLAGGED(ch->in_room,ROOM_PEACEFUL)) {
    send_to_char("Hey! this is a peaceful room y'know!\r\n",ch);
    return;
  }

  if (!*arg || !(vict = get_char_room_vis(ch, arg))) {
    if (FIGHTING(ch)) {
      vict = FIGHTING(ch);
    } else {
      send_to_char("Punch whom?\r\n", ch);
      return;
    }
  }
  if (vict == ch) {
    send_to_char("Aren't we funny today...\r\n", ch);
    return;
  }
  
  if (!IS_NPC(ch) && !IS_NPC(vict) && IS_AFFECTED2(vict, AFF2_SUBDUED)){
    send_to_char("You can't attack someone who is subdued.\r\n",ch);
    return;
  }
  
  if (GET_TIMES(ch,CAN_PUNCH)) {
    send_to_char("You haven't regained your balance since your last punch!\r\n",ch);
    return;
  }
  mprog = check_for_mobprogs_and_stops(ch, vict, NULL, MPC_ATTACK, 0, 0);
  if (mprog != 2) {
    if (GET_LEVEL(vict) >= LVL_IMMORT) {
      damage(ch,vict,0,SKILL_PUNCH);
      return;
    }
    if (!IS_NPC(ch))
      percent = generic_result_mod(ch,SKILL_PUNCH,NULL,FALSE,-(GET_AC(vict) / 5));
    else
      if (number(0,100) < (75 + number(0,(GET_LEVEL(ch) / 10))))
        percent = SOME_SUCCESS;
      else
        percent = TOTAL_FAILURE;;


    switch (percent) {
      case TOTAL_FUCKUP:
        damage(vict,ch,GET_LEVEL(vict),SKILL_PUNCH);
        SET_TIMES(ch,CAN_PUNCH, (PULSE_VIOLENCE * 6));
        break;
      case TOTAL_FAILURE:
        damage(ch, vict, 0, SKILL_PUNCH);
        SET_TIMES(ch,CAN_PUNCH, (PULSE_VIOLENCE * 4));
        break;
      case SOME_SUCCESS:
        damage(ch, vict, GET_LEVEL(ch) , SKILL_PUNCH);
        SET_TIMES(ch,CAN_PUNCH, (PULSE_VIOLENCE * 4));
        break;
      case TOTAL_SUCCESS:
        damage(ch, vict, GET_LEVEL(ch) << 1, SKILL_PUNCH);
        SET_TIMES(ch,CAN_PUNCH, (PULSE_VIOLENCE * 2));
        break;
    }
  }
  if (mprog)
    run_mobprog(ch, 0);
} 

ACMD(do_trip)
{
  struct char_data *vict;
  int percent, mprog = 0;

  one_argument(argument,arg);

  if (!IS_NPC(ch) && !GET_SKILL(ch,SKILL_TRIP)) {
    send_to_char("You don't know how!\r\n",ch);
    return;
  }

  if (GET_TIMES(ch,CAN_TRIP)) {
    send_to_char("You haven't regained balance since your last trip!\r\n",ch);
    return;
  }
   
  if (ROOM_FLAGGED(ch->in_room,ROOM_PEACEFUL)) {
    send_to_char("Hey! This is a peaceful room!\r\n",ch);
    return;
  }
   
  if (!*arg || !(vict = get_char_room_vis(ch,arg))) {
    if (FIGHTING(ch))
      vict = FIGHTING(ch);
    else {
      send_to_char("Trip whom?\r\n",ch);
      return;
    }
  }

  if (!IS_NPC(ch) && !IS_NPC(vict) && IS_AFFECTED2(vict, AFF2_SUBDUED)) {
    send_to_char("You can't attack someone who is subdued.\r\n", ch);
    return;
  }
    
  if (vict == ch) {
    send_to_char("Haha! Very funny, NOT!\r\n",ch);
    return;
  }
  mprog = check_for_mobprogs_and_stops(ch, vict, NULL, MPC_ATTACK, 0, 0);
  if (mprog != 2) {
    if (GET_LEVEL(vict) >= LVL_IMMORT) {
      damage(ch,vict,0,SKILL_TRIP);
      return;
    }
    if (!IS_NPC(ch)) 
      percent = generic_result_mod(ch,SKILL_TRIP,NULL,TRUE,-(GET_AC(vict) / 5));
    else
      if (number(0,100) < (75 + number(0,(GET_LEVEL(ch) / 10))))
         percent = SOME_SUCCESS;
      else
         percent = TOTAL_FAILURE;

    if ((IS_NPC(vict) && MOB_FLAGGED(vict,MOB_NOTRIP)) ||
         IS_AFFECTED2(ch, AFF2_UNBALANCE))
      percent = TOTAL_FAILURE;

    if (!IS_NPC(vict) && IS_AFFECTED2(vict,AFF2_TRACTION)) {
      if (((SOME_SUCCESS == percent)||(TOTAL_SUCCESS == percent)) && (number(0,100) < 50)) {
        percent = TOTAL_FAILURE;
      }
    }

    switch (percent) {
      case TOTAL_FUCKUP :
        damage(vict,ch,GET_LEVEL(vict),SKILL_TRIP);
        SET_TIMES(ch,CAN_TRIP, (6 * PULSE_VIOLENCE));
        GET_POS(ch) = POS_SITTING;       
        break;
   
      case TOTAL_FAILURE:
        SET_TIMES(ch,CAN_TRIP, (4 * PULSE_VIOLENCE));
        damage(ch,vict,0,SKILL_TRIP);
        GET_POS(ch) = POS_SITTING;       
        break;
  
      case SOME_SUCCESS: 
        GET_POS(vict) = POS_SITTING;       
        damage(ch,vict,5,SKILL_TRIP);      
        SET_TIMES(ch,CAN_TRIP, (4 * PULSE_VIOLENCE));

        if (IS_NPC(vict)) 
          GET_MOB_WAIT(vict) += PULSE_VIOLENCE;

        break;

      case TOTAL_SUCCESS:
        GET_POS(vict) = POS_SITTING;
        SET_TIMES(ch,CAN_TRIP, (2 * PULSE_VIOLENCE));
        damage(ch,vict,GET_LEVEL(ch),SKILL_TRIP);

    if (IS_NPC(vict))
          GET_MOB_WAIT(vict) += 2 * PULSE_VIOLENCE;

        break;
    }   
  }
  if (mprog)
    run_mobprog(ch, 0);
}

ACMD(do_dirtkick)
{
  struct char_data *vict;
  int percent, mprog = 0;
  struct affected_type af1,af2;
  
  af1 = clear_aff_var(af1);
  af2 = clear_aff_var(af2);
  argument = one_argument(argument,arg);

  if (!IS_NPC(ch) && !GET_SKILL(ch,SKILL_DIRTKICK)) {
    send_to_char("Pathetic! You couldn't kick dirt on a house!\r\n",ch);
    return;
  }

  if (ROOM_FLAGGED(ch->in_room,ROOM_PEACEFUL)) {
    send_to_char("Hey! This is a peaceful room!\r\n",ch);
    return;
  }
  
  if (GET_TIMES(ch,CAN_DIRTKICK)) {
    send_to_char("You haven't regained your balance yet!\r\n",ch);
    return;
  }

  if (!*arg || !(vict = get_char_room_vis(ch,arg))) {
    if (FIGHTING(ch))
      vict = FIGHTING(ch);
    else {
      send_to_char("Kick dirt, yes, kick dirt, fine, but on WHOM!?\r\n",ch);
      return;
    }
  }
  
  if (vict == ch) {
    send_to_char("we're funny today, aren't we?\r\n",ch);
    return;
  }
  
  if (!IS_NPC(ch) && !IS_NPC(vict) && IS_AFFECTED2(vict, AFF2_SUBDUED)) {
    send_to_char("You can't attack someone who is subdued.\r\n",ch);
    return;
  }
  
  mprog = check_for_mobprogs_and_stops(ch, vict, NULL, MPC_ATTACK, 0, 0);
  if (mprog != 2) {
    if (GET_LEVEL(vict) >= LVL_IMMORT) {
      damage(ch,vict,0,SKILL_DIRTKICK);
      return;
    }
  
    if (IS_AFFECTED2(vict, AFF2_NODIRTKICK)) {
      act("$n tries to kick dirt on $N, but something seems to protect $M.", TRUE, ch, 0, vict, TO_ROOM);
      send_to_char("Darn! That bugger seems to be protected in some way.\r\n", ch);
      act("$n tries to kick dirt on you, but your protection stops it.", TRUE, ch, 0, vict, TO_VICT);
      return;
    }
    
    if (!IS_NPC(ch))
      percent = generic_result_mod(ch,SKILL_DIRTKICK,NULL,TRUE,-(GET_AC(vict) / 5));
    else
      if (number(0,100) < (75+number(0,(GET_LEVEL(ch)/10))))
        percent = SOME_SUCCESS;
      else
        percent = TOTAL_FAILURE;

    if (IS_NPC(vict) && MOB_FLAGGED(vict,MOB_NOBLIND))
      percent = TOTAL_FAILURE;

    af1.duration  = 4;
    af2.duration  = 4;
    af1.modifier  = 50;
    af2.modifier  = -25;
    af1.location  = APPLY_AC;
    af2.location  = APPLY_HITROLL;
    SET_BIT(af1.bitvector, AFF_BLIND);
    SET_BIT(af2.bitvector, AFF_BLIND);
    af1.next = af2.next = NULL;
    af1.type = af2.type = SKILL_DIRTKICK;

    switch (percent) {
      case TOTAL_FUCKUP : 
        damage(vict,ch,10,SKILL_DIRTKICK);
        SET_TIMES(ch,CAN_DIRTKICK, (10 * PULSE_VIOLENCE));
        if (!affected_by_spell(ch,SKILL_DIRTKICK) && !IS_AFFECTED2(ch,AFF2_NODIRTKICK)) {
          affect_to_char(ch,&af1);
          affect_to_char(ch,&af2);
        }
        else {
          affect_join(ch,&af1,TRUE,FALSE,FALSE,TRUE);
      affect_join(ch,&af2,TRUE,FALSE,FALSE,TRUE);
        }
        break;
     
      case TOTAL_FAILURE:
        damage(ch,vict,0,SKILL_DIRTKICK);
        SET_TIMES(ch,CAN_DIRTKICK,  (5 * PULSE_VIOLENCE));
        break;
     
      case SOME_SUCCESS:
        SET_TIMES(ch,CAN_DIRTKICK, (5 * PULSE_VIOLENCE));
        if (!affected_by_spell(vict,SKILL_DIRTKICK)) {
          affect_to_char(vict,&af1);
          affect_to_char(vict,&af2);
        }
        else {
          affect_join(vict,&af1,TRUE,FALSE,FALSE,TRUE);
	  affect_join(vict,&af2,TRUE,FALSE,FALSE,TRUE);
        }
        damage(ch,vict,5,SKILL_DIRTKICK);
        break;

      case TOTAL_SUCCESS:
        SET_TIMES(ch,CAN_DIRTKICK, (2 * PULSE_VIOLENCE));
        af1.duration = af2.duration = 10 + (GET_LEVEL(ch) / 4);
        af1.modifier *=2;
        af2.modifier *=2;
        if (!affected_by_spell(vict,SKILL_DIRTKICK)) {
          affect_to_char(vict,&af1);
          affect_to_char(vict,&af2);
        } 
        else {
          affect_join(vict,&af1,TRUE,FALSE,FALSE,TRUE);
          affect_join(vict,&af2,TRUE,FALSE,FALSE,TRUE);
        }
        damage(ch,vict,GET_LEVEL(ch),SKILL_DIRTKICK);
        break;
    }
  }
  if (mprog)
    run_mobprog(ch, 0);
}

ACMD(do_shoot)
{ struct char_data *target = NULL;
  struct obj_data *bow = NULL, *arrow = NULL, *temp= NULL;
  int damage, distance, i, temproom, fromroom, dir = 0, dirfound = 0;
  int bonus = 0;
  char targetstr[50], dirstr[50]; 

  char *fromdir[] = 
  { "the south",
    "the west",
    "the north",
    "the east",
    "below",
    "above",
    "the southwest",
    "the northwest",
    "the northeast",
    "the southeast"
  };
  
  if (GET_TIMES(ch, CAN_SHOOT)) {
    send_to_char("Hey now, mister lightning...You have to wait a little until you can shoot again.\r\n", ch);
    return;
  }
    
  if( GET_WOLFKIN(ch) )
    bonus = 1;
  if( GET_BONDED(ch) && !(GET_BONDED(ch) & (1 << 30)) )
    bonus = 2;  
  if( (bow = GET_EQ(ch, WEAR_WIELD_L)) )
    if (GET_OBJ_TYPE(bow) != ITEM_BOW) bow = NULL;
  if( !bow ) 
    if( (bow = GET_EQ(ch, WEAR_WIELD_R)) )
      if(GET_OBJ_TYPE(bow) != ITEM_BOW) bow = NULL;

  if (!bow)  {
    send_to_char("Shoot with what?\r\n", ch);
    return;
  }

  for (temp = ch->carrying; temp; temp = temp->next_content)
    if (GET_OBJ_TYPE(temp) == ITEM_ARROW)
      arrow = temp;

  if( !arrow )
  { send_to_char("You need arrows to be able to shoot someone, you know...\r\n", ch);
    return;
  }

  two_arguments(argument, targetstr, dirstr);
  if ((!*targetstr) || (!*dirstr)) {
    send_to_char("Shoot who where?\r\n", ch);
    return;
  }
  for (i = 0; i < NUM_OF_DIRS; i++) /* Check if shooter typed in a correct dir */
    if(!strcasecmp(dirs[i], dirstr))
    { dir = i;
      dirfound = TRUE;
    }
  if( dirfound )
  { distance = 0;
    temproom = ch->in_room;   
    for (i=1; i < 4 + bonus; i++) 
    { if (!target)
        if (CAN_GO(ch, dir))
	{ fromroom = ch->in_room;
          char_from_room(ch);               /* Ugly, I know. But easy :) */
          char_to_room(ch, world[fromroom].dir_option[dir]->to_room);
          target = get_char_room_vis(ch, targetstr);
          distance++;
        }
    }
    char_from_room(ch);
    char_to_room(ch, temproom);
    if( !target )
    { send_to_char("You aim your bow at thin air...\r\n", ch);
      return;
    }
    if (IS_NPC(target) && IS_SET(MOB_FLAGS(target),MOB_NOKILL)) 
    { send_to_char("You can't aim at that person!\r\n",ch);
      return;
    }
  }
  else 
  { send_to_char("Which way is this unfortunate target, really? \r\n", ch);
    return;
  }

  if( GET_LEVEL( target ) >= LVL_IMMORT )
  { char  error[255];
    send_to_char("Aiming at an Immortal can be hazardous to your health\r\n", ch );
    sprintf( error, "%s is pointing an ready bow at you\r\n", ch->player.name );
    send_to_char( error, target );
    return;
  }

  if (!GET_SKILL(ch, SKILL_SHOOT))
  { send_to_char("You're no good with this weapon, alas.\r\n", ch);
    return;
  }
    
  if (ROOM_FLAGGED(target->in_room, ROOM_PEACEFUL))
  { send_to_char("You thought you were clever there, huh? Shooting into a peaceful room...\r\n", ch);
    return;
  }

  SET_TIMES(ch, CAN_SHOOT, ((2-(GET_DEX(ch)-15)/10) * PULSE_VIOLENCE));
  obj_from_char(arrow);
  sprintf(buf,"$n shoots an arrow %s", dirs[dir]);
  act(buf, TRUE, ch, NULL, NULL, TO_ROOM);
  if (ROOM_AFFECTED(target->in_room, RAFF_DOMEOFAIR)) {
    send_to_char("What the...? The arrow bounced against some invisible wall..?!\r\n", ch);
    extract_obj(arrow);
    return;
  }
  if ((GET_SKILL(ch, SKILL_SHOOT) - 5*distance)> number(0, 100)){
    damage = dice(GET_OBJ_VAL(bow, 1), GET_OBJ_VAL(bow, 2)) + GET_DAMROLL(ch)
    + MAX(0, GET_DEX(ch) - 20);
    damage -= damage*(distance-1.0)*0.1;
    if (damage > number(0,100))  /* Damage % risk that the arrow breaks */
      obj_to_room(arrow, target->in_room);
    else
      extract_obj(arrow);
    send_to_char("It's a hit!\r\n", ch);
    sprintf(buf,"$n is suddenly hit by an arrow coming from %s!", fromdir[dir]);
    act(buf, TRUE, target, NULL, NULL, TO_ROOM);
    sprintf(buf, "Someone just shot you! The arrow came from %s.\r\n", fromdir[dir]);
    send_to_char(buf, target);
    GET_HIT(target) -= damage;
    update_pos(target);
    switch (GET_POS(target)) 
    {
    case POS_MORTALLYW:
      act("$n lies on the ground, arrow protruding from $s chest, slowly dying.", TRUE, target, 0, 0, TO_ROOM);
      send_to_char("You've been hit, badly, and will die if not help arrives, soon.\r\n", target);
    break;
    case POS_INCAP:
      act("$n lies on the ground, bleeding badly, and will die if noone helps $m.", TRUE, target, 0, 0, TO_ROOM);
      send_to_char("That arrow sure knew where it took...You're slowly dying, fading away.\r\n", target);
    break;
    case POS_STUNNED:
      act("$n is stunned by the glancing blow to $s head, but will wake up again.", TRUE, target, 0, 0, TO_ROOM);
      send_to_char("That arrow struck good into your head, but you'll wake up again.\r\n", target);
    break;
    case POS_DEAD:
      act("$n falls down on the ground with an arrow through $s throat, dead...", FALSE, target, 0, 0, TO_ROOM);
      send_to_char("You caught an arrow in your throat and died...\r\n", target);
      die(target, ch);
    break;

    default:			/* >= POSITION SLEEPING */
      if (damage > (GET_MAX_HIT(target) >> 2))
        act("That really did HURT! That arrow looks stuck...Aow.", FALSE, target, 0, 0, TO_CHAR);   
    }
    if ((number(0,100)< 6) && (GET_SKILL(ch, SKILL_SHOOT)<100)) {
      send_to_char("Excellent shot! You learn how to aim better from this.\r\n", ch);
      ADD_SKILL(ch, SKILL_SHOOT, 1);
    }
  }
  else
  {
     send_to_char("Darn! you missed.\r\n", ch);
     sprintf(buf,"Suddenly an arrow flies past, coming from %s.", fromdir[dir]);
     act(buf, TRUE, target, NULL, NULL, TO_ROOM);
     sprintf(buf, "Suddenly an arrow comes from %s, missing you by inches.", fromdir[dir]);
     send_to_char(buf, target);
     if (50 > number(0,100))     /* 50-50 that an arrow that misses breaks */
       obj_to_room(arrow, target->in_room);
     else
       extract_obj(arrow);
   }      
}

ACMD(do_dist_spell)
{
  struct char_data *target = NULL;
  int distance, i, targetroom, fromroom, temproom, dir = 0, dirfound = 0;
  int bonus = 0;
  char targetstr[50], dirstr[50], rest[50], *temp1 = NULL, *spell = NULL; 
  
  if (!GET_SKILL(ch, SKILL_TARGET)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }
  
  if (GET_POS(ch) == POS_FIGHTING) {
    send_to_char("You can't weave into other rooms when fighting.\r\n", ch);
    return;
  }

  if (ROOM_FLAGGED(ch->in_room, ROOM_PEACEFUL | ROOM_NOMAGIC)){
    send_to_char("\\c01Not\\c00 from this room,  you don't!\r\n", ch);
    return;
  }
  
  skip_spaces(&argument);
  
  if (!*argument) {
    send_to_char("Syntax: target '<spellname>' <targetname> <direction> \r\n", ch);
    return;
  }
    
  temp1 = strtok(argument, "'");
  spell = temp1;  
  temp1 = strtok(NULL, "'");

  if ((temp1 == NULL) || (spell == NULL)) {
    send_to_char("Syntax: target '<spellname>' <targetname> <direction> \r\n", ch);
    return;
  }  
  
  if (GET_WOLFKIN(ch))
    bonus = 1;
    
  if (GET_BONDED(ch) && !(GET_BONDED(ch) & (1 << 30)))
    bonus = 2;
      
  strcpy(rest, two_arguments(temp1, targetstr, dirstr));
  if ((!*targetstr) || (!*dirstr)){
    send_to_char("Weave what at who where?\r\n", ch);
    return;
  }
  for (i = 0; i < NUM_OF_DIRS; i++) /* Check if shooter typed in a correct dir */
    if(!strcasecmp(dirs[i], dirstr)) {
      dir = i;
      dirfound = TRUE;
    }
  if (dirfound){
    distance = 0;
    fromroom = ch->in_room;   
    for (i=1; i < 4 + bonus; i++) {
      if (!target)
        if (CAN_GO(ch, dir)){
          temproom = ch->in_room;
          char_from_room(ch);               /* Ugly, I know. But easy :) */
          char_to_room(ch, world[temproom].dir_option[dir]->to_room);
          target = get_char_room_vis(ch, targetstr);
          distance++;
        }
    }
    targetroom = ch->in_room;
    char_from_room(ch);
    char_to_room(ch, fromroom);
    if (!target)     {
      send_to_char("You prepare to cast the weave at...whom?\r\n", ch);
      return;
    }
    if (IS_NPC(target) && IS_SET(MOB_FLAGS(target),MOB_NOKILL)) {
      send_to_char("You can't target weaves against that person!\r\n",ch);
      return;
    }
  }
  else {
    send_to_char("Which way is this unfortunate target, really? \r\n", ch);
    return;
  }

  if (ROOM_FLAGGED(target->in_room, ROOM_PEACEFUL | ROOM_NOMAGIC)){
    send_to_char("You thought you were clever there, huh? Zapping into a peaceful room...\r\n", ch);
    return;
  }

  sprintf(buf,"$n casts a weave %s.", dirs[dir]);
  act(buf, TRUE, ch, NULL, NULL, TO_OPUSERSS);
  sprintf(buf, "weave '%s' %s %s", spell,  targetstr, rest);
  char_from_room(ch);
  char_to_room(ch, targetroom);
  /* why print the actual cmd line*
     send_to_char(buf, ch);
     send_to_char("\r\n", ch);
  */
  command_interpreter(ch, buf);
  char_from_room(ch);
  char_to_room(ch, fromroom);
}


ACMD(do_sworddance)
{
  struct char_data *opp1,*opp2,*k;
  
  int result = generic_result(ch,SKILL_SWORDDANCE,NULL,FALSE);
  
  opp1 = opp2 = NULL;
  
  for (k = combat_list; k != NULL; k = k->next_fighting) {
    if (FIGHTING(k) == ch)
      opp1 = k;
    if ((FIGHTING(k) == ch) && (k != opp1))
      opp2 = k;
  }      
  
  if (!opp1 || !opp2) /* No use without opponents! */
    return;
  
  switch (result) {
    case TOTAL_FUCKUP:
      send_to_char("You stumble on your own feet trying to do the sword dance!\r\n",ch);
      GET_POS(ch) = POS_SITTING;
      return;
      break;
    case TOTAL_FAILURE:
      send_to_char("You fail to dance away from this bugger.\r\n",ch);
      return;
      break;
  }
  stop_fighting(opp1);
  stop_fighting(opp2);
  stop_fighting(ch);
  
  set_fighting(opp2,ch);
  set_fighting(opp1,ch);
  
  act("You dance away from $N and face a new opponent!",FALSE,ch,0,opp1,TO_CHAR);
  act("$n dance away from you to face another opponent!",FALSE,ch,0,opp2,TO_VICT);
  act("$n dance away from $N to another battle.",FALSE,ch,0,opp1,TO_NOTVICT); 
}
  
ACMD(do_disarm)
{
  struct char_data *vict;
  int result, mprog = 0;

  one_argument(argument,arg);

  if (!IS_NPC(ch) && !GET_SKILL(ch,SKILL_DISARM)) {
    send_to_char("You don't have any clue how to do that!\r\n",ch);
    return;
  }

  if (ROOM_FLAGGED(ch->in_room,ROOM_PEACEFUL)) {
    send_to_char("You can't make your body obey your commands.\r\n",ch);
    return;
  }
  
  if (GET_TIMES(ch,CAN_DISARM)) {
    send_to_char("You haven't regained your balance since your last disarm.\r\n",ch);
    return;
  }
  if (!*arg || !(vict = get_char_room_vis(ch,arg))) {
    if (FIGHTING(ch)) {
      vict = FIGHTING(ch);
    }
    else {
      send_to_char("Disarm whom?\r\n",ch);
      return;
    }
  }
  if (vict == ch) {
    send_to_char("That isn't a very bright idea you know.\r\n",ch);
    return;
  }
  
  if (!IS_NPC(ch) && !IS_NPC(vict) && IS_AFFECTED2(vict, AFF2_SUBDUED)) {
    send_to_char("You can't attack someone who is subdued.\r\n",ch);
    return;
  }
  
  if (!GET_EQ(vict,WEAR_WIELD_R) && !GET_EQ(vict,WEAR_WIELD_L)) {
    send_to_char("This person isn't even armed!\r\n",ch);
    return;
  }

  mprog = check_for_mobprogs_and_stops(ch, vict, NULL, MPC_ATTACK, 0, 0);
  if (mprog != 2) {  
    if (GET_LEVEL(vict) >= LVL_IMMORT) {
      damage(ch,vict,0,SKILL_DISARM);
      return;
    }

    if (!IS_NPC(ch))
      result = generic_result_mod(ch,SKILL_DISARM,NULL,FALSE,-(GET_AC(vict) / 10));
    else
      if (number(0,100) < (75 + number(0,(GET_LEVEL(ch) / 10))))
        result = SOME_SUCCESS;
      else
        result = TOTAL_FAILURE;

    switch(result) {
      case TOTAL_FUCKUP:
        SET_TIMES(ch,CAN_DISARM, (6 * PULSE_VIOLENCE));
        damage(ch,vict,0,SKILL_DISARM);
        return;
        break;
      case TOTAL_FAILURE:
        SET_TIMES(ch,CAN_DISARM, (3 * PULSE_VIOLENCE));
        damage(ch,vict,0,SKILL_DISARM);
        return;
      case SOME_SUCCESS:
        SET_TIMES(ch,CAN_DISARM, (3 * PULSE_VIOLENCE));
        break;
      case TOTAL_SUCCESS:
        SET_TIMES(ch,CAN_DISARM,  PULSE_VIOLENCE);
        break;
    }
    if (NULL != GET_EQ(vict,WEAR_WIELD_R)) {
      if (IS_SET(GET_OBJ_EXTRA(GET_EQ(vict,WEAR_WIELD_R)),ITEM_NODROP | ITEM_NODONATE))
        obj_to_char(unequip_char(vict,WEAR_WIELD_R),vict);
      else
        obj_to_room(unequip_char(vict,WEAR_WIELD_R),vict->in_room);
    }

    if (NULL != GET_EQ(vict,WEAR_WIELD_L)) {
      if (IS_SET(GET_OBJ_EXTRA(GET_EQ(vict,WEAR_WIELD_L)),ITEM_NODROP | ITEM_NODONATE))
        obj_to_char(unequip_char(vict,WEAR_WIELD_L),vict);
      else
        obj_to_room(unequip_char(vict,WEAR_WIELD_L),vict->in_room);
    }

    act("$n sends $N's weapon flying with a swift move!", TRUE, ch, 0, vict, TO_NOTVICT);
    act("You send $N's weapon flying with a clever move.", TRUE, ch, 0, vict, TO_CHAR);
    act("$n send your weapon flying across the room with a swift move!", TRUE, ch, 0, vict, TO_VICT);
    damage(ch,vict,1,SKILL_DISARM);
  }
  run_mobprog(ch, 0);
}
  
ACMD(do_stun) 
{
  struct char_data *victim;
  int result, mprog = 0;

  one_argument(argument,arg);

  if (ROOM_FLAGGED(ch->in_room,ROOM_PEACEFUL)) {
    send_to_char("Hey! this is a peaceful room!\r\n",ch);
    return;
  }

  if (GET_TIMES(ch,CAN_STUN)) {
    send_to_char("You haven't regained your balance since your last stun.\r\n",ch);
    return;
  }

  if (!*arg || !(victim = get_char_room_vis(ch,arg))) {
    if (!(victim = FIGHTING(ch))) {
      send_to_char("Stun who?\r\n",ch);
      return;
    }
  }
  if (ch == victim) {
    send_to_char("Haha, very funny, not!\r\n",ch);
    return;
  }
  
  if (!IS_NPC(ch) && !IS_NPC(victim) && IS_AFFECTED2(victim, AFF2_SUBDUED)) {
    send_to_char("You can't attack someone who is subdued.\r\n", ch);
    return;
  }
  
  if (!IS_NPC(ch) && !GET_SKILL(ch,SKILL_STUN)) {
    send_to_char("You don't know how!\r\n",ch);
    return;
  }

  mprog = check_for_mobprogs_and_stops(ch, victim, NULL, MPC_ATTACK, 0, 0);
  if (mprog != 2) {
    if (IS_NPC(ch))
      if (number(0,100) < number(70,MIN((GET_LEVEL(ch)/30),30)))
        result = SOME_SUCCESS;
      else
        result = TOTAL_FAILURE;
    else
      result = generic_result_mod(ch,SKILL_STUN,NULL,FALSE,-(GET_AC(ch) / 10));

    if (IS_NPC(victim) && MOB_FLAGGED(victim,MOB_NOSTUN))
    result = TOTAL_FAILURE;

    if ((GET_LEVEL(victim) >= LVL_IMMORT) || (result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE)){
      damage(ch,victim,0,SKILL_STUN);
      SET_TIMES(ch,CAN_STUN, (16 * PULSE_VIOLENCE));
      GET_POS(ch) = POS_SITTING;
      return;
    }
    if (result == SOME_SUCCESS) {
      WAIT_STATE(victim,PULSE_VIOLENCE * 2);
      damage(ch,victim,1,SKILL_STUN);
      GET_POS(victim) = POS_STUNNED;
      SET_TIMES(ch,CAN_STUN, (12 * PULSE_VIOLENCE));
      update_pos(victim);
      return;
    }
    if (result == TOTAL_SUCCESS) {
      WAIT_STATE(victim,PULSE_VIOLENCE * 3);
      GET_POS(victim) = POS_STUNNED;
      SET_TIMES(ch,CAN_STUN, (8 * PULSE_VIOLENCE));
      damage(ch,victim,GET_LEVEL(ch),SKILL_STUN);
      update_pos(victim);
      return;
    }
  }
  if (mprog)
    run_mobprog(ch, 0);
}
  

int mastery(struct char_data *ch)
{
  int true = 0;
  char skl[20] = "\0";

  if(GET_SKILL(ch, SKILL_BLADEMASTERY))
    if (check_weapon_alias(ch, SKILL_BLADEMASTERY)) {
      if (GET_SKILL(ch, SKILL_BLADEMASTERY) < 100)
        true = 1;
      else true = 3;
      (void)strcpy(skl, "blademastery");
    }

  if(GET_SKILL(ch, SKILL_SPEARMASTERY))
    if (check_weapon_alias(ch, SKILL_SPEARMASTERY)) {
      if (GET_SKILL(ch, SKILL_SPEARMASTERY) < 100)
        true = 2;
      else true = 3;
      (void)strcpy(skl, "spearmastery");
    }
/* not an elegant solution, but the skills won't go over 100 now */
  if ((number(1,100) <= 2) && (true < 3)) {
    asend_to_char(ch,"Your %s skill increases!\r\n",skl);
    if (1 == true)
      ADD_SKILL(ch,SKILL_BLADEMASTERY, 1);
    else if (2 == true)
      ADD_SKILL(ch,SKILL_SPEARMASTERY, 1);
  }

  return true;
}

int check_weapon_alias(struct char_data *ch, int mastery_type)
{
  char alias[10];

  switch(mastery_type){

    case SKILL_BLADEMASTERY:
      strcpy(alias, "sword");
      break;

    case SKILL_SPEARMASTERY:
      strcpy(alias, "spear");
      break;
  }

  if (GET_EQ(ch, WEAR_WIELD_L))
    if (isname(alias, GET_EQ(ch, WEAR_WIELD_L)->name))
      return 1;

  if (GET_EQ(ch, WEAR_WIELD_R))
    if (isname(alias, GET_EQ(ch, WEAR_WIELD_R)->name))
      return 1;

  if (GET_EQ(ch, WEAR_WIELD_DUAL))
    if (isname(alias, GET_EQ(ch, WEAR_WIELD_DUAL)->name))
      return 1;

  return 0;
}

ACMD(do_sever)
{
  char part[200];
  struct char_data *victim = NULL;
  
  two_arguments(argument, arg, part);
  
  if (!GET_SKILL(ch, SKILL_SEVER))
    send_to_char("You don't know how!\r\n", ch);
  else if (!*arg) 
    send_to_char("You have to supply a name as well...\r\n", ch);
  else if (!part[0])
    send_to_char("You must specify what part you want to cut as well.\r\n", ch);
  else if (!isname(part, "right left"))
    send_to_char("Partname must be either 'right' or 'left'.\r\n", ch);
  else if (!(victim = get_char_room_vis(ch, arg)))
    send_to_char(NOPERSON, ch);
  else if ((!strncasecmp(part, "right", strlen(part)) && IS_SET(GET_PERMANENT(victim), PRM_RH_CUT))
        || (!strncasecmp(part, "left", strlen(part)) && IS_SET(GET_PERMANENT(victim), PRM_LH_CUT)))
    send_to_char("That person has already got that hand cut off.\r\n", ch);
  else if (GET_LEVEL(victim) >= LVL_IMMORT || IS_NPC(victim))
    send_to_char("Sorry, you're not allowed to do that to that person.\r\n", ch);
  else if (ch == victim)
    send_to_char("Cut your own hand? Not a good idea.\r\n", ch);
  else {
    act("With one swift chop, you cut off $N's hand!", TRUE, ch, 0, victim, TO_CHAR);
    act("$n cuts off your hand!", TRUE, ch, 0, victim, TO_VICT);
    act("$n cuts off $N's hand in one sweeping motion!", TRUE, ch, 0, victim, TO_NOTVICT);  

    if (!strncasecmp(part, "right", strlen(part))) {
      SET_BIT(GET_PERMANENT(victim), PRM_RH_CUT);
      if (GET_EQ(victim, WEAR_WIELD_R))
        obj_to_char(unequip_char(victim, WEAR_WIELD_R), victim);        
      if (GET_EQ(victim, WEAR_HOLD))
        obj_to_char(unequip_char(victim, WEAR_HOLD), victim);
      if (GET_EQ(victim, WEAR_FINGER_R))
        obj_to_char(unequip_char(victim, WEAR_FINGER_R), victim);        
      return;
    }
    SET_BIT(GET_PERMANENT(victim), PRM_LH_CUT);
    if (GET_EQ(victim, WEAR_WIELD_L))
      obj_to_char(unequip_char(victim, WEAR_WIELD_L), victim);    
    if (GET_EQ(victim, WEAR_FINGER_L))
      obj_to_char(unequip_char(victim, WEAR_FINGER_L), victim);    
  } 
  return;
}
 
