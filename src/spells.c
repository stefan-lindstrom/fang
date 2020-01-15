/******************************************************************************
 * $Id: spells.c,v 1.10 2003/11/19 05:17:39 fang Exp $
 *   
 * File: spells.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: spells.c,v 
 *   Revision 1.2  2002/07/28 13:56:05  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.13  2002/07/16 15:16:17  fang
 *   Drath: Seafolk Guildskills are done.
 *
 *   Revision 1.12  2002/07/06 23:18:24  fang
 *   Drath: IMMs canot do SS or SoS
 *
 *   Revision 1.11  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.10  2002/05/21 13:30:50  fang
 *   *** empty log message ***
 *
 *   Revision 1.9  2002/05/21 12:46:12  fang
 *   *** empty log message ***
 *
 *   Revision 1.8  2002/05/17 13:31:22  fang
 *   *** empty log message ***
 *
 *   Revision 1.7  2002/05/16 14:23:15  fang
 *   *** empty log message ***
 *
 *   Revision 1.6  2002/05/15 10:08:49  fang
 *   open lock spell now works on containers in room/worn/in inventory
 *
 *   Revision 1.5  2002/05/15 08:45:45  fang
 *   *** empty log message ***
 *
 *   Revision 1.4  2002/05/10 12:51:35  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/03/19 02:22:19  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.27  2002/01/30 09:20:44  mud
 *   shortgate added. Also made gate not choke on unknown rooms, if distance between rooms are not morre than 10 rooms
 *
 *   Revision 1.26  2002/01/24 21:33:20  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.25.2.2  2002/01/23 21:49:43  mud
 *   Removed objsave.[c|h] (old objsave stuff).
 *   Removed obj_file_elem and rent structs.
 *   Removed final vestiges of old guild system, except parts used
 *   in guild.[c|h] for backwards compatibility.
 *
 *   Revision 1.25.2.1  2002/01/22 03:05:15  mud
 *   More cleanup. Removed extern decls from source files. Protected some DG headers from multiple inclision
 *
 *   Revision 1.25  2002/01/14 00:24:29  mud
 *   Fireball forced to max 4K damage
 *
 *   Revision 1.24  2001/11/07 09:21:37  mud
 *   *** empty log message ***
 *
 *   Revision 1.23  2001/08/20 13:49:47  mud
 *   *** empty log message ***
 *
 *   Revision 1.22  2001/08/11 17:50:47  mud
 *   Modified to make wardersense and warderheal obsolete and have the code that
 *   checked for those skills check to see if the character is a bonded Gaidin - Illaria
 *
 *   Revision 1.21  2001/08/06 17:44:32  mud
 *   Modified sedaibond so that wardersense and warderheal are set to 100 -
 *   Illaria
 *
 *   Revision 1.20  2001/08/05 17:07:06  mud
 *   Modified sedaibond to automatically set wardersense and warderheal skills. -
 *   Illaria
 *
 *   Revision 1.19  2001/08/05 16:03:53  mud
 *   *** empty log message ***
 *
 *   Revision 1.18  2001/07/18 17:09:11  mud
 *
 *   bond becomes spells ashabond and sedaibond
 *
 *   Revision 1.17  2001/05/02 09:57:29  mud
 *   *** empty log message ***
 *
 *   Revision 1.16  2001/03/24 03:25:46  mud
 *   *** empty log message ***
 *
 *   Revision 1.15  2001/03/22 16:37:07  mud
 *   *** empty log message ***
 *
 *   Revision 1.14  2001/02/17 14:53:00  mud
 *   Commented out test comments asdent_to_char in airblow - Illaria
 *
 *   Revision 1.13  2001/01/28 13:11:38  mud
 *   *** empty log message ***
 *
 *   Revision 1.12  2000/12/11 12:14:18  mud
 *   Removed most references to PFILEPOS() get_char_ppos(), since with the xml code ppos will(may) change every reboot
 *
 *   Revision 1.11  2000/12/09 18:43:27  mud
 *   *** empty log message ***
 *
 *   Revision 1.10  2000/12/08 13:49:38  mud
 *   *** empty log message ***
 *
 *   Revision 1.9  2000/12/08 12:09:48  mud
 *   *** empty log message ***
 *
 *   Revision 1.8  2000/08/03 19:28:43  mud
 *
 *   Patches for talent, and added talent requirements for skills/spells
 *
 *   Revision 1.7  2000/05/24 08:41:14  mud
 *   Aiel endurance changes
 *
 *   Revision 1.6  2000/05/21 14:42:07  mud
 *   Addition to do_who
 *
 *   Revision 1.5  2000/04/11 13:55:25  mud
 *   Added void, minor compulsion, compulsion and treesinging talents.
 *
 *   Revision 1.4  2000/04/10 22:19:53  mud
 *   New Talent System
 *
 *   Revision 1.3  2000/03/15 13:11:40  mud
 *   Changed from B-tree based skill/weave index to 'something else' :)
 *
 *   Revision 1.2  2000/02/21 14:00:19  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* ************************************************************************
*   File: spells.c                                      Part of CircleMUD *
*  Usage: Implementation of "manual spells".  Circle 2.2 spell compat.    *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "fight.h"
#include "conf.h"
#include "sysdep.h"
#include "act.h"
#include "config.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "spells.h"
#include "handler.h"
#include "db.h"
#include "interpreter.h"
#include "constants.h"
#include "xmlcharsave.h"
#include "guild.h"
#include "magic.h"
#include "graph.h"


#define CAP_LOW  0
#define CAP_HIGH 1

static long cap_amount(long amount,short type,long cap_val,int spell)
{
  long cap;

  if (CAP_LOW == type) {
    if (amount > 0) 
      cap = cap_val;
    else if (amount < cap_val)
      cap = cap_val;
    else
      cap = amount;       
  }
  else if (CAP_HIGH == type) {
    if (amount < 0) 
      cap = cap_val;
    else if (amount > cap_val)
      cap = cap_val;
    else
      cap = amount;
  }
  else {
    alog("SYSERR: Unknown cap-type supplied to cap_amount() from SPELL(%s).\n",spells[spell]);
    alog("        Setting cap to 0!\n");
    cap = 0;
  }
  return cap;
}
/* amount    -> the calculated amount (i,.e damage, modifiers */
/* capamount -> the caped amount of damage/modifier           */
/* factor    -> the factor of bonus of manadd, i.e if a spell */
/*		gives mana_add/3 extra pnts of damage when    */
/*              the factor should be 3. if it gives 2*mana_add*/
/*		factor should be 0.5			      */
static long cap_mana_add(long amount, long capamount,double factor,int spell)
{
  long tmp;
  if (amount == capamount)
  {
    return 0;
  }  
  else if (amount > capamount)
  {
    tmp = (amount - capamount)*factor;
    tmp = MAX(0, tmp);
    return tmp;
  }
  else
  {
    alog("SYSERR: Capamount is -HIGHER- than amount, from SPELL(%s)\n",spells[spell]);
    return 0;
  }
  return 0;
} 

/* Sanity check, so we don't get a wrapp-around when adding taint! */

/* Additional per 980418: Removed the unsigned on amount,so that a */
/* negative 'amount' doesn't give any unreasonably large taint add.*/
/* /Fredrik							   */
unsigned long add_taint(struct char_data *ch, long amount)
{
  unsigned long a;
  double half = 1;

  if (IS_NPC(ch))
    return 0;

  if (amount < 0)  /* Just in case...  */
    amount = 1000; /* Arbitrary amount */

  if (IS_SET(GET_PERMANENT(ch),PRM_TAINTRES)) {
    if (20 < number(0,100)) {
      half = 0.5;
    }
  }
  if (get_guild(ch,7) || is_dark(ch))
    return 0;

  if (SEX_FEMALE == GET_SEX(ch))
    return 0;

  a = GET_TAINT(ch);

  if (a == MAX_TAINT)
    return 0;
  
  if ((MAX_TAINT - a) <= amount*half) { /* Ch will max taint */
    GET_TAINT(ch) = MAX_TAINT; /* Just set it and return */
    return 0;
  }
  
  return (int)(half * amount);
}

bool is_in_link(struct char_data *master, struct char_data *ch)
{
    int i = 0;
    
    if(ch==master) return(TRUE);
    
    /* check each player in master's link */
    while(master->player_specials->linking[i]){
        if (ch == master->player_specials->linking[i])
            return(TRUE);
        ++i;
        }
    /* if we got this far without returning, the player isn't part of the link */
    return(FALSE);
} /* end of is_in_link */


int count_link(struct char_data *ch, int type)
{

    int temp = 0;
    int i = 0;
    struct char_data *f;
    while(ch->player_specials->linking[i])
    {
      f = ch->player_specials->linking[i];
      if (GET_SEX(ch) == type) ++ temp;
      ++i;
      }
    if (type == -1) return(i+1);
    
    if (GET_SEX(ch) == type)  
        return(temp+1);
    else
        return(temp);	
} /* end of count_link */


void perform_link(struct char_data *ch, struct char_data *vict){

  int i = 0;
  
  if (!(IS_SET(PRF_FLAGS(vict), PRF_GRASPING))){
    act("$N must be grasping the source before $S can be linked.\r\n", FALSE, ch, 0, vict, TO_CHAR);
    return;
    }

  if (ch->player_specials->linked_by) {
    send_to_char("Hey, you're already a part of a link!\r\n",ch);
    return;
  }

  /* set up links */	
  if (vict->player_specials->linked_by) {
    if (vict->player_specials->linked_by == ch) {
      asend_to_char(ch,"%s is already a part of your link!\r\n",GET_NAME(ch));
    } else {
      asend_to_char(ch,"%s is already in %ss link!\r\n",GET_NAME(vict->player_specials->linked_by));
    }
    return;
  }

  vict->player_specials->linked_by = ch;
  
  while (ch->player_specials->linking[i])
    i ++;
  ch->player_specials->linking[i] = vict;
    
  
  /* force the player to follow the link */
  if (ch->master)
    stop_follower(ch);
  add_follower(vict, ch);
  act("You are now linked to $n.\r\n", FALSE, ch, 0, vict, TO_VICT);
  act("$N is now linked to you.\r\n", FALSE, ch, 0, vict, TO_CHAR);
  act("$N is now linked to $n.\r\n", FALSE, ch, 0, vict, TO_ROOM);

  /* adjust mana pool */
  if (ch->player_specials->max_manalinked == 0){
     ch->player_specials->max_manalinked = ch->points.mana;
     ch->player_specials->manalinked = ch->points.mana;
     ch->player_specials->managiven = ch->points.mana;
     }
    
  ch->player_specials->max_manalinked += vict->points.mana;
  ch->player_specials->manalinked += vict->points.mana;
  vict->player_specials->managiven = vict->points.mana;
  vict->points.mana = 0;
  
} /* end of perform_link */


void perform_unlink(struct char_data *ch)
{
  int i = 0;
  struct char_data *vict = NULL;
  
  /* reimburse victim's mana */
  if (!ch->player_specials->linking[0]) {
    send_to_char("You aren't in control of any link, smeghead!\r\n",ch);
    return;
  }
  while (ch->player_specials->linking[i]) {
    vict = ch->player_specials->linking[i];

    if (vict->player_specials->linked_by != ch) {
      asend_to_char(vict,"Error! %s tried to unlink you, but you aren't a part of %ss link!\r\n",GET_NAME(ch),GET_NAME(ch));
      asend_to_char(ch,"%s isn't in your link!\r\n",GET_NAME(vict));
      alog("SYSERR: %s tried to unlink %s, but %s isn't in %ss link!\r\n",GET_NAME(ch),GET_NAME(vict),GET_NAME(vict),GET_NAME(ch));
      return;
    }

    if (ch->player_specials->manalinked >= 0)
      GET_MANA(vict) = (ch->player_specials->manalinked * vict->player_specials->managiven) / 
      ch->player_specials->max_manalinked;
    else
      vict->points.mana = ch->player_specials->manalinked;
    /* free the victim */
    vict->player_specials->linked_by = 0;
    vict->player_specials->managiven = 0;
    /* adjust mana pool */
    ch->player_specials->manalinked -= vict->points.mana;
    ch->player_specials->max_manalinked -= vict->points.mana;	
    stop_follower(vict);
   
    /* Notify world what happened */
    act("$N is no longer part of your link.\r\n", FALSE, ch, 0, vict, TO_CHAR);
    act("You are no longer part of $n's link.\r\n", FALSE, ch, 0, vict, TO_VICT);
    act("$N is no longer part of $n's link.\r\n", FALSE, ch, 0, vict, TO_NOTVICT);
    ch->player_specials->linking[i] = NULL;   
    i ++;
  } 
} /* end of perform_unlink */



void add_to_link(struct char_data *ch, struct char_data *vict)
{

  if ((GET_SEX(ch) == SEX_MALE) && (GET_LEVEL(ch) < LVL_IMMORT)){
      send_to_char("Only women can start a link, moron!\r\n", ch);
      return;
      }
  
  if (ch->player_specials->linked_by) {
    send_to_char("You can't start a link when already -\\c01IN\\c00 a link!\r\n",ch);
    return;
  }
  if ((vict->player_specials->linked_by) || (vict->player_specials->linking[0])){
      act("$N is already part of a link!\r\n", FALSE, ch, 0, vict, TO_CHAR);
      return;
      }
      
  if (GET_SEX(vict) == SEX_MALE){
    /* This is a 2 person link where the woman must surrender control to the man */
    if (!(ch->player_specials->linking[0])){
      if (GET_LEVEL(vict) < LVL_IMMORT){
        perform_link(vict, ch);
        return;
        }
      else{
        /* if the vict is an immort, we won't switch control.  This is the case of two male
           imms linking each other */
        perform_link(ch, vict);
        return;
        }
      }
    
    if (count_link(ch, SEX_FEMALE) <= count_link(ch, SEX_MALE)-2){
      send_to_char("This link needs another woman to expand!\r\n", ch);
      return;
      }
   
    if (count_link(ch, -1) >= MAX_NUMBER_IN_LINK){
      send_to_char("This link cannot expand!  Whine to an imm if you don't like it!\r\n", ch);
      return;
      }
      
    perform_link(ch, vict);
    return;
    }
          
  if (GET_SEX(vict) == SEX_FEMALE){
    if ((count_link(ch, SEX_FEMALE) / 13) >= (count_link(ch, SEX_MALE) + 1)){
     send_to_char("This link needs another man to expand!\r\n", ch);
     return;
     }
    
    if (count_link(ch, -1) >= MAX_NUMBER_IN_LINK){
     send_to_char("This link cannot expand!  Whine to an imm if you don't like it!\r\n", ch);
     return;
     }
    perform_link(ch, vict);
    return;
    }
    
  /* victim is a neuter, poor thing */
  send_to_char("You cannot link a neuter.\r\n", ch);
  return;
} /* end of add_to_link */


int MadeWith(struct char_data *ch)
{
  
  if (!IS_NPC(ch) && IS_SET(PRF_FLAGS(ch),PRF_TGRASP))
    return TRUE_POWER;
  else {
    if (GET_SEX(ch) == SEX_MALE) 
      return SAIDIN;
    else
      return SAIDAR;
  }
}


ACMD(do_powertrap)
{
  struct affected_type af;
  char *str = NULL; 
  int spell = 0, delay = 0, i = 0, result = 0;
  char *valid_triggers[] = {
  "NOTHING",
  "human",
  "channeler",
  "shadowspawn",
  "STOP"
  };
  
  if (!GET_SKILL(ch, SKILL_POWERTRAP)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }

  if (ROOM_FLAGGED(ch->in_room, ROOM_PEACEFUL)) {
    send_to_char("In a peaceful room!? I think not.\r\n", ch);
    return;
  }
  if (ROOM_AFFECTED(ch->in_room, RAFF_TRAP)) {
    send_to_char("There is already a trap in this room. Can't have more than one.\r\n", ch);
    return;
  }

  skip_spaces(&argument);
  if (!argument) {
    send_to_char("Usage: Powertrap '<spell>' <condition|time>\r\n", ch);
    return;
  }
  if (*argument != '\'') {
    send_to_char("Spell must be enclosed in ' '.\r\n", ch);
    return;
  }
  argument ++;
  str = argument;
  while ((*str != '\'') && *str) 
    str++;
  if (!*str) {
    send_to_char("End ' missing. Try again. \r\n", ch);
    return;
  }

  *str = 0;
  if (*(str+1))
    str ++;
  else {
    send_to_char("Must give condition for trigger too.\r\n", ch);
    return;
  }
  skip_spaces(&str);
  if (!*str) {
    send_to_char("Must give condition for trigger too.\r\n", ch);
     return;
  }
  spell = find_skill_num(argument);
  if ((spell < 1) || (spell > MAX_SPELLS)) {
    send_to_char("Unknown weave. Try again.\r\n", ch);
    return;
  }
  if (!GET_SKILL(ch, spell)) {
    send_to_char("You don't know that weave.\r\n", ch);
    return;
  }
  if (!IS_SET(spell_info[spell].targets,IS_AGGRESSIVE)) {
    send_to_char("Trapping can only be done with aggressive spells.\r\n", ch);
    return;
  }
  if (is_number(str)) {
    delay = atoi(str);
    if (delay < 1) {
      send_to_char("Set the delay to less than one hour? You suicidal?\r\n", ch);
      return;
    }
  }
  else {
    for (i = 0; strcmp(valid_triggers[i], "STOP"); i++) 
      if (!strncasecmp(str, valid_triggers[i], strlen(str)))
        break;
    if (!strcmp(valid_triggers[i], "STOP")) {
      send_to_char("That's not a valid trigger...Valid ones are channeler, shadowspawn and human.\r\n", ch);
      return;
    }
  }
  result = generic_result_mod(ch, SKILL_POWERTRAP, NULL, FALSE, 0);
  if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
    send_to_char("You fail to set a trap here.\r\n", ch);
    return;
  }

  result = generic_result_mod(ch, spell, NULL, FALSE, 0);
  if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
    send_to_char("Your weave fizzles.\r\n", ch);
    GET_MANA(ch) -= 200;
    check_mana(ch);
    return;
  }
  if (delay)
    sprintf(buf, "%d hours have passed.\r\n", delay);
  else
    sprintf(buf, "a %s enters the room.\r\n", valid_triggers[i]);
  asend_to_char(ch, "Ok, you set the weave %s to go off when %s",
   spells[spell], buf);
  GET_MANA(ch) -= spell_info[spell].mana;
  af.duration = (delay)?delay: 1000;
  af.bitvector = RAFF_TRAP;
  af.speed = RAFF_SLOW;
  af.type = spell;
  af.sex = MadeWith(ch);
  af.next = NULL;
  af.tie_info = 1;
  af.weaver = GET_IDNUM(ch);
  af.location = (delay)?0:i; /* If not delay, what trigger? */
  affect_to_room(&world[ch->in_room], &af);
}

void check_if_learned(struct char_data *ch,int spell)
{
}

BSPELL(frostbite)
{
  int door = 0, toroom = 0;
  struct affected_type af;
  int bonus, success, mana;
  
  bonus = IntBonus(ch);
  mana  = spell_info[SPELL_FROSTBITE].mana + mana_add;
  success = normal_spell_test(SPELL_FROSTBITE, ch, NULL, mana, bonus);
  af = clear_aff_var(af);
  if (success) {
    af.duration = 3 + mana_add/100 + (GET_AIR(ch)+GET_WATER(ch))/5;
    af.bitvector= RAFF_COLD;
    af.speed    = RAFF_SLOW;
    af.type     = SPELL_FROSTBITE;
    af.sex      = MadeWith(ch);
    af.next     = NULL;
    af.tie_info = tied;
    af.weaver   = GET_IDNUM(ch);
    af.mana_add  = mana_add;

    gen_message(SPELL_FROSTBITE,ch,NULL,FALSE,TRUE);
    affect_to_room(&world[ch->in_room],&af);    
    af.duration = 3;
    for (door = 0; door < NUM_OF_DIRS; door ++)
      if (CAN_GO(ch, door)) {
        toroom = world[ch->in_room].dir_option[door]->to_room;
        affect_to_room(&world[toroom],&af);
        send_to_room("Suddenly it gets very, very cold!\r\n",toroom);
      }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.4));       
  }
  return;
}

BSPELL(fog)
{
  int door = 0, toroom = 0;
  struct affected_type af;
  int bonus, success, mana;

  bonus = IntBonus(ch);
  mana  = spell_info[SPELL_FOG].mana;
  success = normal_spell_test(SPELL_FOG, ch, NULL, mana, bonus);
  af = clear_aff_var(af);
  if (success) {
    af.duration = 10;
    af.bitvector= RAFF_FOG;
    af.speed    = RAFF_SLOW;
    af.type     = SPELL_FOG;
    af.sex      = MadeWith(ch);
    af.next     = NULL;
    af.tie_info = tied;
    af.weaver   = GET_IDNUM(ch);
    af.mana_add  = mana_add;

    gen_message(SPELL_FOG,ch,NULL,FALSE,TRUE);
    affect_to_room(&world[ch->in_room],&af);
    for (door = 0; door < NUM_OF_DIRS; door ++) 
      if (CAN_GO(ch, door)) {
        toroom = world[ch->in_room].dir_option[door]->to_room;
        affect_to_room(&world[toroom],&af);
        send_to_room("Suddenly a fog rolls in from nowhere, covering the area!\r\n", toroom);
      }
    check_if_learned(ch,SPELL_FOG);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.3));       
  return;
}

BSPELL(armor)
{
 struct affected_type af;
 int bonus,result,mana,tmp,tmp2,tmp3;
 
 af = clear_aff_var(af);
  if (affected_by_spell(ch, SPELL_ARMOR)) {
    send_to_char("You are already armored as it is.\r\n", ch);
    return;
  }

  tmp       = (int)(-30 * mod);
  tmp       = cap_amount(tmp,CAP_LOW,-200,SPELL_ARMOR);
  tmp2      = 10 + (int) ((5*mana_add)/spell_info[SPELL_ARMOR].mana_add);
  tmp3      = MIN(50, MAX(0, tmp2));
  if (tmp2 > tmp3)              // If duration indeed was capped.
    mana_add -= 20*(tmp2-tmp3); // get back 20 mana for each tick.

  bonus  = IntBonus(ch);  
  result = generic_result_mod(ch,SPELL_ARMOR,NULL,FALSE,bonus);
  mana = (spell_info[SPELL_ARMOR].mana + mana_add);

  if (tmp > 0)
    tmp = -200;
  if (tmp3 < 0)
    tmp3 = 50;

  af.duration  = tmp3;
  af.modifier  = tmp;
  af.location  = APPLY_AC;
  af.bitvector = AFF_ARMOR;
  af.next      = NULL;
  af.type      = SPELL_ARMOR;
  af.sex       = MadeWith(ch);
  af.tie_info  = tied;
  af.weaver    = GET_IDNUM(ch);
  af.mana_add  = mana_add;

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.4));

  switch(result) {
    case TOTAL_FUCKUP:
        gen_message(SPELL_ARMOR,ch,ch,FALSE,FALSE);
        return;
        break;
    case TOTAL_FAILURE:
        GET_MANA(ch) -= (mana / 2);
        gen_message(SPELL_ARMOR,ch,ch,FALSE,FALSE);
        return;
        break;
    case SOME_SUCCESS:
        break;
    case TOTAL_SUCCESS:
        mana /= 2;
        af.duration = (int) (1.25 * af.duration);
        af.modifier -= 20;
        break;
  }

  GET_MANA(ch) -= mana;
  affect_to_char(ch,&af); 
  gen_message(SPELL_ARMOR,ch,ch,FALSE,TRUE);	 
  check_if_learned(ch,SPELL_ARMOR);	/* Doesn't do anything, yet...*/
  return;
}

BSPELL(wind_armor)
{
  struct affected_type af, af2, af3;
  int mana,bonus,success,tmp1,tmp2,tmp3; 
 
  af = clear_aff_var(af);
  af2 = clear_aff_var(af2);
  af3 = clear_aff_var(af3);
  if (affected_by_spell(vict, SPELL_WIND_ARMOR)){
    send_to_char("Already affected by Wind Armor.\r\n", ch);
    return;
  }
  
  tmp1      = cap_amount((10 + mana_add/100),CAP_HIGH,50,SPELL_WIND_ARMOR); 
  mana_add -= cap_mana_add((10 + mana_add/100),tmp1,100.0,SPELL_WIND_ARMOR);
  tmp2      = cap_amount((int)(30*mod),CAP_HIGH,80,SPELL_WIND_ARMOR);
  tmp3      = cap_amount((int)(-10*mod),CAP_LOW,-100,SPELL_WIND_ARMOR);

  mana   = (spell_info[SPELL_WIND_ARMOR].mana + mana_add);
  bonus  = IntBonus(ch);
  
  if (tmp1 <= 0)
    tmp1 = 50;

  af.duration   = tmp1;
  af.modifier   = tmp2;
  af.location   = APPLY_ABS;
  af.next       = NULL;
  af.type       = SPELL_WIND_ARMOR;
  af.sex        = MadeWith(ch);
  af.tie_info   = tied;
  af.weaver     = GET_IDNUM(ch);

  if (af.modifier < 0) 
    af.modifier = 80;

  af2.duration   = tmp1;
  af2.modifier   = tmp3;
  af2.location   = APPLY_AC;
  af2.next       = NULL;
  af2.type       = SPELL_WIND_ARMOR;
  af2.sex        = MadeWith(ch);
  af2.tie_info   = tied;
  af2.weaver     = GET_IDNUM(ch);

  if (af2.modifier > 0) 
    af2.modifier = -100;
  
  af3.duration   = tmp1;
  af3.modifier   = 0;
  af3.location   = 0;
  af3.bitvector2 = AFF2_NODIRTKICK;
  af3.next       = NULL;
  af3.type       = SPELL_WIND_ARMOR;
  af3.sex        = MadeWith(ch);
  af3.tie_info   = tied;
  af3.weaver     = GET_IDNUM(ch);

  af.mana_add = af2.mana_add = af3.mana_add = mana_add;

  success = normal_spell_test(SPELL_WIND_ARMOR, ch, NULL, mana, bonus);
  if (success) {
    gen_message(SPELL_WIND_ARMOR,ch,vict,FALSE,TRUE);
    affect_to_char(vict, &af);
    affect_to_char(vict, &af2);
    affect_to_char(vict, &af3);
    check_if_learned(ch,SPELL_WIND_ARMOR);
  }
  GET_TAINT(ch) += add_taint(ch,(int)mana * 0.4);
  return;
}

BSPELL(infravision)
{
  struct affected_type af;
  int mana,bonus,success,tmp1,tmp2; 
 
  af = clear_aff_var(af);
  if (affected_by_spell(ch, SPELL_INFRAVISION))
  {
    send_to_char("You are already an owl as it is.\r\n", ch);
    return;
  }
  
  tmp1      = 10 + (mana_add/spell_info[SPELL_INFRAVISION].mana_add);
  tmp2      = cap_amount(tmp1,CAP_HIGH,50,SPELL_INFRAVISION);
  mana_add -= cap_mana_add(tmp1,tmp2,(double)(spell_info[SPELL_INFRAVISION].mana_add),SPELL_INFRAVISION);

  mana   = (spell_info[SPELL_INFRAVISION].mana + mana_add);
  bonus  = IntBonus(ch);
 
  af.duration  = tmp2;
  af.modifier  = 0;
  af.location  = 0;
  af.bitvector = AFF_INFRAVISION;
  af.next      = NULL;
  af.type      = SPELL_INFRAVISION;
  af.sex       = MadeWith(ch);
  af.tie_info  = tied;
  af.weaver    = GET_IDNUM(ch);
  af.mana_add  = mana_add;

  success = normal_spell_test(SPELL_INFRAVISION, ch, NULL, mana, bonus);
  if (success) {
    gen_message(SPELL_INFRAVISION,ch,NULL,FALSE,TRUE);
    affect_to_char(ch, &af);
    check_if_learned(ch,SPELL_INFRAVISION);
  }
  else
    gen_message(SPELL_INFRAVISION,ch,NULL,FALSE,FALSE);
  
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.4));
  return;
}


BSPELL(ball_of_light)
{
  int mana,bonus,success; 
  struct obj_data *ball;
 
  mana   = spell_info[SPELL_BALLOFLIGHT].mana;
  bonus = IntBonus(ch);
  success = normal_spell_test(SPELL_BALLOFLIGHT, ch, NULL, mana, bonus);
  if (success) {
    gen_message(SPELL_BALLOFLIGHT,ch,NULL,FALSE,TRUE);
     ball = create_obj();

     ball->item_number       = NOTHING;
     ball->in_room           = NOWHERE;
     ball->name              = str_dup("ball light");
     ball->description       = str_dup("A glowing orb lies here, illuminating the dark");
     ball->short_description = str_dup("a glowing ball");
     ball->action_description= str_dup("It shines with an eerie, steady bright light.");

     GET_OBJ_TYPE(ball)      = ITEM_LIGHT;
     GET_OBJ_WEAR(ball)      = ITEM_WEAR_TAKE | ITEM_WEAR_HOLD;
     GET_OBJ_EXTRA(ball)     = ITEM_NOSELL | ITEM_NOTOOL | ITEM_NODROP | ITEM_NODONATE;

     if (is_dreamer(ch)) 
       SET_BIT(OBJ_EXTRA_FLAGS(ball),ITEM_TAR);

     GET_OBJ_VAL(ball,3)     = 0;
     GET_OBJ_VAL(ball,0)	    = 0;
     GET_OBJ_VAL(ball,1)     = 0;
     GET_OBJ_VAL(ball,2)     = -1;
     GET_OBJ_WEIGHT(ball)    = 1;
     GET_OBJ_TIMER(ball)     = 8 + GET_LEVEL(ch)/10;
     obj_to_char(ball, ch);
     check_if_learned(ch,SPELL_BALLOFLIGHT);
  }
  else
    gen_message(SPELL_BALLOFLIGHT,ch,NULL,FALSE,FALSE);

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.4));

  return;
}

BSPELL(invigorate)
{
  int mana,bonus,success,move,tmp1; 
 
  tmp1      = (int)((75 + mana_add/2)*mod);
  move      = cap_amount(tmp1,CAP_HIGH,200,SPELL_INVIGORATE);
  mana_add -= (long) (cap_mana_add(tmp1,move,2.0,SPELL_INVIGORATE)/mod); 
  mana_add  = MAX( 0, mana_add);

  mana   = (spell_info[SPELL_INVIGORATE].mana + mana_add);
  bonus  = IntBonus(ch);
  success = normal_spell_test(SPELL_INVIGORATE, ch, NULL, mana, bonus);

  if (success) {
    gen_message(SPELL_INVIGORATE,ch,vict,TRUE,TRUE);
    GET_MOVE(vict) = MIN(GET_MAX_MOVE(vict), (GET_MOVE(vict) + move));
    GET_FRACT_MOVE(vict) = 0;
    check_if_learned(ch,SPELL_INVIGORATE);
  }
  else
    gen_message(SPELL_INVIGORATE,ch,vict,TRUE,FALSE);

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.4));

  return;
}

BSPELL(minor_healing)
{
  int mana,bonus,success,tmp1,hitp; 
 
  tmp1      = (int)((mana_add/2)*mod);
  hitp      = cap_amount(tmp1,CAP_HIGH,300,SPELL_MINOR_HEAL);
  mana_add -= (long) (cap_mana_add(tmp1,hitp,2.0,SPELL_MINOR_HEAL)/mod);

  mana   = (spell_info[SPELL_MINOR_HEAL].mana + mana_add);
  bonus  = IntBonus(ch);
  success = normal_spell_test(SPELL_MINOR_HEAL, ch, NULL, mana, bonus);

  if (success) {
    GET_TIMES(ch,CAN_WEAVE) =  (PULSE_VIOLENCE);
    gen_message(SPELL_MINOR_HEAL,ch,vict,TRUE,TRUE);
    GET_HIT(vict) = MIN(GET_MAX_HIT(vict), GET_HIT(vict) + hitp);
    check_if_learned(ch,SPELL_MINOR_HEAL);
  }
  else
  {
    GET_TIMES(ch,CAN_WEAVE) =  (2*PULSE_VIOLENCE);
    gen_message(SPELL_MINOR_HEAL,ch,vict,TRUE,FALSE);
  }  

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.4));

  return;
}

BSPELL(group_restore)
{
  struct follow_type *f;
  struct char_data *leader;
  int mana, bonus, success, hit_per_vict, num_people,tmp1;

  if (!IS_AFFECTED(ch, AFF_GROUP)) {
    send_to_char("But you're not grouped!\r\n", ch);
    return;
  }
  
  leader = ch->master? ch->master: ch;
  for (num_people = 0, f = leader->followers; f; num_people ++, f = f->next);
  if (num_people < 2) {
    send_to_char("You're only grouped with yourself. This weave can't be used then.\r\n", ch);
    return;
  }
  num_people--;         /* -1 since the caster won't benefit from this weave */

  tmp1         = (int)((3*mana_add/num_people)*mod);
  hit_per_vict = cap_amount(tmp1,CAP_HIGH,1000,SPELL_GROUP_RESTORE);
  mana_add    -=(long) (cap_mana_add(tmp1,hit_per_vict,(num_people/3.0),SPELL_GROUP_RESTORE)/mod);

  mana   = spell_info[SPELL_GROUP_RESTORE].mana + mana_add;
  bonus  = IntBonus(ch);
  success = normal_spell_test(SPELL_GROUP_RESTORE, ch, NULL, mana, bonus);
  
  if (success) {
    gen_message(SPELL_GROUP_RESTORE,ch,NULL,FALSE,TRUE);
    if (leader != ch)
      GET_HIT(leader) = MIN(GET_MAX_HIT(leader), GET_HIT(f->follower) + hit_per_vict);
    for (f = leader->followers; f; f = f->next)
      if (f->follower != ch)
        GET_HIT(f->follower) = MIN(GET_MAX_HIT(f->follower), GET_HIT(f->follower) + hit_per_vict);
    check_if_learned(ch,SPELL_GROUP_RESTORE);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.4));
  
}

BSPELL(invisibility)
{
  struct affected_type af;
  int mana,bonus,result,acmod,dur,tmp1, i;

  for (i=0; i <= NUM_TALENTS - 1; i++){
    if (!strcasecmp("invisibility", master_talent_list[i].name))
      break;
  }


  if (!(IS_TALENT_SET(GET_SPSKILLS1(ch), GET_SPSKILLS2(ch), i) || IS_IMMORTAL(ch))) {
    send_to_char("Sorry. You need to report this talent to the Storymaster.\r\n",ch);
    return;
  }
 
  af = clear_aff_var(af);
  if (affected_by_spell(ch, SPELL_INVISIBILITY))
  {
    send_to_char("What's the use? You are already invisible.\r\n", ch);
    return;
  }
  mod = MAX(0.2, mod);
  tmp1      = (int)(-40 * mod);
  acmod     = cap_amount(tmp1,CAP_LOW,-120,SPELL_INVISIBILITY);
  tmp1      = (10 + (mana_add/spell_info[SPELL_INVISIBILITY].mana_add));
  dur       = cap_amount(tmp1,CAP_HIGH,50,SPELL_INVISIBILITY);
  mana_add -=(long) (cap_mana_add(tmp1,dur,(double)spell_info[SPELL_INVISIBILITY].mana_add,SPELL_INVISIBILITY)/mod);

  mana   = (spell_info[SPELL_INVISIBILITY].mana + mana_add);
  bonus  = IntBonus(ch);
  result = generic_result_mod(ch,SPELL_INVISIBILITY,NULL,FALSE,bonus); 

  af.duration  = dur;
  af.modifier  = acmod;
  af.location  = APPLY_AC;
  af.bitvector = AFF_INVISIBLE;
  af.next      = NULL;
  af.type      = SPELL_INVISIBILITY;
  af.sex       = MadeWith(ch);
  af.tie_info  = tied;
  af.weaver    = GET_IDNUM(ch);
  af.mana_add  = mana_add;

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.4));

  switch(result) {
    case TOTAL_FUCKUP:
        GET_MANA(ch) -= (mana * 2);
        gen_message(SPELL_INVISIBILITY,ch,NULL,FALSE,FALSE);
        return;
        break;
    case TOTAL_FAILURE:
        GET_MANA(ch) -= (mana / 2);
        gen_message(SPELL_INVISIBILITY,ch,NULL,FALSE,FALSE);
        return;
        break;
    case SOME_SUCCESS:
        break;
    case TOTAL_SUCCESS:
        mana /= 2;
        af.duration  = (int) (1.25 * af.duration);
        af.modifier -= 20;
        break;
  }
  GET_MANA(ch) -= mana;
  gen_message(SPELL_INVISIBILITY,ch,NULL,FALSE,TRUE);
  affect_to_char(ch, &af);
  check_if_learned(ch,SPELL_INVISIBILITY);

}

BSPELL(airshield)
{
  struct obj_data *shield;
  int success,mana,bonus,oval,tmp1;
 
  bonus   = IntBonus(ch);
  mana    = spell_info[SPELL_AIRSHIELD].mana;
  success = normal_spell_test(SPELL_AIRSHIELD, ch, NULL, mana, bonus);
  if (success) {
    shield = create_obj();

    shield->item_number       = NOTHING;
    shield->in_room           = NOWHERE;
    shield->name              = str_dup("shield air");
    shield->description       = str_dup("A greyish-white glittering shield lies here.");
    shield->short_description = str_dup("an Airshield");
    shield->action_description= str_dup("An Airshield");
  
    tmp1 = (int)((20 + (GET_LEVEL(ch)/10))*mod);
    oval = cap_amount(tmp1,CAP_HIGH,80,SPELL_AIRSHIELD);

    GET_OBJ_TYPE(shield)      = ITEM_ARMOR;
    GET_OBJ_WEAR(shield)      = ITEM_WEAR_TAKE | ITEM_WEAR_SHIELD;
    GET_OBJ_EXTRA(shield)     = ITEM_NOSELL;

    if (is_dreamer(ch))
      SET_BIT(OBJ_EXTRA_FLAGS(shield),ITEM_TAR);

    GET_OBJ_VAL(shield,3)     = 0; 
    GET_OBJ_VAL(shield,0)     = oval;
    GET_OBJ_VAL(shield,1)     = GET_OBJ_VAL(shield, 0);
    GET_OBJ_VAL(shield,2)     = 0;
    GET_OBJ_WEIGHT(shield)    = 9;
    GET_OBJ_TIMER(shield)     = 10 + GET_LEVEL(ch)/20;
    
    gen_message(SPELL_AIRSHIELD,ch, NULL,FALSE,TRUE);
   
    if (GET_EQ(ch,WEAR_SHIELD)) {
      obj_to_char(unequip_char(ch,WEAR_SHIELD),ch);
      send_to_char("You put your old shield in your inventory.\r\n",ch);
    } 
    equip_char(ch,shield,WEAR_SHIELD);
    check_if_learned(ch,SPELL_AIRSHIELD);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.4));

}

BSPELL(targate)
{
#define TAR_NUM 39
  struct obj_data *gate1 = NULL,*gate2 = NULL;

  int result,mana,bonus;

  bonus = IntBonus(ch);
  mana  = spell_info[SPELL_TARGATE].mana;

  if (!IS_IMMORTAL(ch) && !test_bit(GET_KNOW(ch),world[ch->in_room].number)&& IS_SET(PRF_FLAGS(ch),PRF_IC)) {
    send_to_char("You don't know this room well enough to open a gate to Tel'aran'Rhiod from here.\r\n",ch);
    return;
  }

  result = normal_spell_test(SPELL_TARGATE,ch,NULL,mana,bonus);

  if (result) {
    gate1 = read_object(TAR_NUM,VIRTUAL);
    gate2 = read_object(TAR_NUM,VIRTUAL);
    GET_OBJ_VAL(gate1,0) = 4;
    GET_OBJ_VAL(gate2,0) = 4;
    GET_OBJ_VAL(gate1,1) = GET_IDNUM(ch);
    GET_OBJ_VAL(gate2,1) = GET_IDNUM(ch);
    GET_OBJ_VAL(gate1,2) = ch->in_room;
    GET_OBJ_VAL(gate2,2) = ch->in_room;      /* Just in case... */
    SET_BIT(GET_OBJ_EXTRA(gate1),ITEM_TAR);  /* Gate1 is the gate in T'A'R */
    obj_to_room(gate1,ch->in_room);	     /* Gate2, the on in the Real World */
    obj_to_room(gate2,ch->in_room);
    
    act("&WWith a huge bang, a bar of light suddenly appears in the room, turning into a Gate!&n",TRUE,0,gate2,0,TO_ROOM);
    act("&WWith a huge bang, a bar of light suddenly appears in the room, turning into a Gate!&n",TRUE,0,gate1,0,TO_DREAMERS);

    gate1->linked = gate2;
    gate2->linked = gate1;
    gen_message(SPELL_TARGATE,ch,vict,FALSE,TRUE);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.5));

#undef TAR_NUM
}

BSPELL(gate)
{
#define GATE_NUM 40

  struct obj_data *gate1 = NULL,*gate2 = NULL;
  int success,mana,time,bonus;
  struct char_data *list, *next;
  int room_num,tmp1;


  if (IS_AFFECTED(ch,AFF_DREAMING)) {
    send_to_char("Sorry. No Gates in Tel'Aran'Rhiod.\r\n",ch);
    return;
  }

  /*
   * If character isn't immortal, and doesn't know the 
   * room currently in, and is IC, then he/she can't gate! 
   * Will be added in a few days (from the 19th of march 1999, 
   * to allow morts to get knowledge about a few rooms before
   * introducing this code. 
   */
  if (!IS_IMMORTAL(ch) && 
      !test_bit(GET_KNOW(ch),world[ch->in_room].number) && IS_SET(PRF_FLAGS(ch),PRF_IC)) {
    if (steps_between(ch->in_room,real_room(rm->number)) > 10) {
      send_to_char("Looks like you don't know this location well enough to Gate from here...\r\n",ch);
      return;
    }
  }

  tmp1      = 10 + (int)(((mana_add / spell_info[SPELL_GATE].mana_add) * 2)*mod);
  time      = cap_amount(tmp1,CAP_HIGH,50,SPELL_GATE);
  mana_add -= (long) (cap_mana_add(tmp1,time,(2.0/(double)spell_info[SPELL_GATE].mana_add),SPELL_GATE)/mod);

  room_num = real_room(rm->number);
  bonus    = IntBonus(ch);  
  mana     = spell_info[SPELL_GATE].mana + mana_add;
  success  = normal_spell_test(SPELL_GATE, ch, NULL, mana, bonus);

  if (zone_table[world[room_num].zone].lock && !IS_IMMORTAL(ch)) {
    send_to_char("Sorry.  That Zone is locked!\r\n", ch);
    success = 0;
  }
  
  if (success) {
    gate1 = read_object(GATE_NUM,VIRTUAL);
    GET_OBJ_VAL(gate1,0) = 4; 		   /* id-gate! */
    GET_OBJ_VAL(gate1,1) = GET_IDNUM(ch); /* who created */
    GET_OBJ_VAL(gate1,2) = room_num;	   /* leads to    */
    GET_OBJ_TIMER(gate1) = time;
  
    obj_to_room(gate1,ch->in_room);
  
    gate2 = read_object(GATE_NUM,VIRTUAL);
    GET_OBJ_VAL(gate2,0) = 4; 		   /* id-gate! */
    GET_OBJ_VAL(gate2,1) = GET_IDNUM(ch); /* who created */
    GET_OBJ_VAL(gate2,2) = ch->in_room;	   /* leads to    */
    GET_OBJ_TIMER(gate2)  = time;
  
    obj_to_room(gate2,room_num);
    if (world[room_num].people != NULL) {
      act("With a huge bang, a bar of light suddenly appears in the room,\r\n"
          "turning into a Gate!",TRUE,world[room_num].people,
           NULL,NULL,TO_ROOM);
      act("With a huge bang, a bar of light suddenly appears in the room,\r\n"
    "turning into a Gate!",TRUE,world[room_num].people,NULL,NULL,TO_CHAR);

      for (list = world[room_num].people; list != NULL; list = next) {
        next = list->next_in_room;
        if (GET_LEVEL(list) >= LVL_IMMORT)
          continue;
        if (number(1,100) == 1) /* 1% */ {
          send_to_char("You are hit by the opening Gate, and instantly die.\r\n",list);
          act("$n is instantly killed as $e is hit by the opening Gate.",FALSE,list,0,0,TO_ROOM);
          die(list,list);
        }
      }
    }
    gate1->linked = gate2;
    gate2->linked = gate1;
    gen_message(SPELL_GATE,ch,vict,FALSE,TRUE);    
    check_if_learned(ch,SPELL_GATE);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.4));

}

BSPELL(blindness)
{
  struct affected_type af,af2;
  int mana,bonus,result,tmp1,dur,acmod,hrmod;
  
  af = clear_aff_var(af);
  af2 = clear_aff_var(af2);
  if (affected_by_spell(vict, SPELL_BLINDNESS)) {
    send_to_char("What's the use? That one's already blind.\r\n", ch);
    return;
  }
  tmp1      = 10 + (sh_int) mana_add/spell_info[SPELL_BLINDNESS].mana_add;
  dur       = cap_amount(tmp1,CAP_HIGH,50,SPELL_BLINDNESS);
  mana_add -= cap_mana_add(tmp1,dur,(double)spell_info[SPELL_BLINDNESS].mana_add,SPELL_BLINDNESS);

  bonus  = IntBonus(ch) + calc_weave_bonus(ch,vict);
  result = generic_result_mod(ch,SPELL_BLINDNESS,NULL,FALSE,bonus);
  mana = spell_info[SPELL_ARMOR].mana + mana_add;

  tmp1  = (int)(50 * mod);
  acmod = cap_amount(tmp1,CAP_HIGH,200,SPELL_BLINDNESS);

  af.duration = dur;
  af.modifier = acmod;
  af.location = APPLY_AC;
  af.bitvector = AFF_BLIND;
  af.next = NULL;
  af.type = SPELL_BLINDNESS;
  af.sex = af2.sex = MadeWith(ch);
  af.tie_info = af2.tie_info = tied;
  af.weaver = af2.weaver = GET_IDNUM(ch);
    
  tmp1  = (int)(-30 * mod);
  hrmod = cap_amount(tmp1,CAP_LOW,-100,SPELL_BLINDNESS);

  af2.duration = dur;
  af2.modifier = hrmod;
  af2.location = APPLY_HITROLL;
  af2.bitvector = AFF_BLIND;
  af2.next = NULL;
  af2.type = SPELL_BLINDNESS;
  
  af2.mana_add = af.mana_add = mana_add;

  GET_TAINT(ch) += add_taint(ch, (int)(mana * 0.4));

  switch (result) {
    case TOTAL_FUCKUP:
        GET_MANA(ch) -= (mana * 2);
        gen_message(SPELL_BLINDNESS,ch,vict,TRUE,FALSE);
        return;
        break;
    case TOTAL_FAILURE:
        GET_MANA(ch) -= (mana / 2);
        gen_message(SPELL_BLINDNESS,ch,vict,TRUE,FALSE);
        return;
        break;
    case SOME_SUCCESS:
        break;
    case TOTAL_SUCCESS:
        mana /= 2;
            af.duration  = (int) (1.25 * af.duration);
        af2.duration = (int) (1.25 * af2.duration);
        af.modifier  += 25;
        af2.modifier -= 15;
        break;
  }
  GET_MANA(ch) -= mana;
  affect_to_char(vict,&af);
  affect_to_char(vict,&af2);
  gen_message(SPELL_BLINDNESS,ch,vict,TRUE,TRUE);
  check_if_learned(ch,SPELL_BLINDNESS);

}

BSPELL(pain)
{
  int success,mana,bonus;
 
  bonus   = IntBonus(ch);
  mana    = spell_info[SPELL_PAIN].mana;
  success = normal_spell_test(SPELL_PAIN, ch, NULL, mana, bonus);
  if (success) {
    GET_MOVE(vict) = 0;
    GET_FRACT_MOVE(ch) = 0;
    GET_POS(vict) = POS_STUNNED;
    WAIT_STATE(vict,PULSE_VIOLENCE * 2);
    gen_message(SPELL_PAIN,ch,vict,TRUE,TRUE);
  }
}

BSPELL(confusion)
{
  struct affected_type af,af2;
  int mana,bonus,success,dur,acmod,hrmod,tmp1;
  
  af = clear_aff_var(af);
  af2 = clear_aff_var(af2);
  if (affected_by_spell(vict, SPELL_CONFUSION)) {
    send_to_char("What's the use? That one's already confused.\r\n", ch);
    return;
  }

  tmp1      = 10 + (sh_int) mana_add/spell_info[SPELL_CONFUSION].mana_add;
  dur       = cap_amount(tmp1,CAP_HIGH,50,SPELL_CONFUSION);
  mana_add -= cap_mana_add(tmp1,dur,(double)spell_info[SPELL_CONFUSION].mana_add,SPELL_CONFUSION);

  bonus  = IntBonus(ch) + calc_weave_bonus(ch,vict);
  mana = spell_info[SPELL_CONFUSION].mana + mana_add;
  success = normal_spell_test(SPELL_CONFUSION, ch, vict, mana, bonus);
  if (success) {
    tmp1  = (int)(10 * mod);
    acmod = cap_amount(tmp1,CAP_HIGH,50,SPELL_CONFUSION);

    af.duration = dur;
    af.modifier = acmod;
    af.location = APPLY_AC;
    af.bitvector2 = AFF2_CONFUSION;
    af.next = NULL;
    af.type = SPELL_CONFUSION;
    af.sex = af2.sex = MadeWith(ch);
    af.tie_info = af2.tie_info = tied;
    af.weaver = af2.weaver = GET_IDNUM(ch);
    
    tmp1  = (int)(-10 * mod);
    hrmod = cap_amount(tmp1,CAP_LOW,-50,SPELL_CONFUSION);

    af2.duration = dur;
    af2.modifier = hrmod;
    af2.location = APPLY_HITROLL;
    af2.bitvector2 = AFF2_CONFUSION;
    af2.next = NULL;
    af2.type = SPELL_CONFUSION;
   
    af2.mana_add = af.mana_add = mana_add;

    affect_to_char(vict,&af);
    affect_to_char(vict,&af2);
    gen_message(SPELL_CONFUSION,ch,vict,TRUE,TRUE);
    check_if_learned(ch,SPELL_CONFUSION);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.4));
}

BSPELL(slow)
{
  struct affected_type af,af2;
  int mana,bonus,success,tmp,acmod,hitmod,dur;
  
  af = clear_aff_var(af);
  af2 = clear_aff_var(af2);
  if (affected_by_spell(vict, SPELL_SLOW)) {
    send_to_char("What's the use? That one's already slowed down.\r\n", ch);
    return;
  }

  tmp       = 5 + (mana_add/spell_info[SPELL_SLOW].mana_add);
  dur       = cap_amount(tmp,CAP_HIGH,50,SPELL_SLOW);
  mana_add -= cap_mana_add(tmp,dur,(double)spell_info[SPELL_SLOW].mana_add,SPELL_SLOW);

  bonus  = IntBonus(ch) + calc_weave_bonus(ch,vict);
  mana = spell_info[SPELL_SLOW].mana + mana_add;
  success = normal_spell_test(SPELL_SLOW, ch, vict, mana, bonus);

  if (success) {
    tmp   = (int)(10 * mod);
    acmod = cap_amount(tmp,CAP_HIGH,50,SPELL_SLOW);

    af.duration = dur;
    af.modifier = acmod;
    af.location = APPLY_AC;
    af.next = NULL;
    af.type = SPELL_SLOW;
    af.sex = af2.sex = MadeWith(ch);
    af.tie_info = af2.tie_info = tied;
    af.weaver = af2.weaver = GET_IDNUM(ch);
    
    tmp    = (int)(-10 * mod);
    hitmod = cap_amount(tmp,CAP_LOW,-50,SPELL_SLOW);

    af2.duration = dur;
    af2.modifier = hitmod;
    af2.location = APPLY_HITROLL;
    af2.next = NULL;
    af2.type = SPELL_SLOW;
   
    af2.mana_add = af.mana_add = mana_add;

    affect_to_char(vict,&af);
    affect_to_char(vict,&af2);
    gen_message(SPELL_SLOW,ch,vict,TRUE,TRUE);
    check_if_learned(ch,SPELL_SLOW);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.4));

}

BSPELL(unbalance)
{
  struct affected_type af,af2;
  int mana,bonus,success,tmp,dur,acmod,hitmod;
  
  af = clear_aff_var(af);
  af2 = clear_aff_var(af2);
  if (affected_by_spell(vict, SPELL_UNBALANCE)) {
    send_to_char("What's the use? That one's already unbalanced.\r\n", ch);
    return;
  }

  tmp       = 3 + (sh_int) mana_add/spell_info[SPELL_UNBALANCE].mana_add;
  dur       = cap_amount(tmp,CAP_HIGH,50,SPELL_UNBALANCE);
  mana_add -= cap_mana_add(tmp,dur,(double)spell_info[SPELL_UNBALANCE].mana_add,SPELL_UNBALANCE);

  bonus  = IntBonus(ch) + calc_weave_bonus(ch,vict);
  mana = spell_info[SPELL_UNBALANCE].mana + mana_add;
  success = normal_spell_test(SPELL_UNBALANCE, ch, vict, mana, bonus);
  if (success) {
    tmp   = (int)(10 * mod);
    acmod = cap_amount(tmp,CAP_HIGH,50,SPELL_UNBALANCE);

    af.duration = dur;
    af.modifier = acmod;
    af.location = APPLY_AC;
    af.bitvector2 = AFF2_UNBALANCE;
    af.next = NULL;
    af.type = SPELL_UNBALANCE;
    af.sex = af2.sex = MadeWith(ch);
    af.tie_info = af2.tie_info = tied;
    af.weaver = af2.weaver = GET_IDNUM(ch);
    
    tmp    = (int)(-10 * mod);
    hitmod = cap_amount(tmp,CAP_LOW,-50,SPELL_UNBALANCE);

    af2.duration = dur;
    af2.modifier = hitmod;
    af2.location = APPLY_HITROLL;
    af2.bitvector2 = AFF2_UNBALANCE;
    af2.next = NULL;
    af2.type = SPELL_UNBALANCE;
   
    af2.mana_add = af.mana_add = mana_add;

    affect_to_char(vict,&af);
    affect_to_char(vict,&af2);
    GET_POS(vict) = POS_SITTING;
    check_if_learned(ch,SPELL_UNBALANCE);
    gen_message(SPELL_SLOW,ch,vict,TRUE,TRUE);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.5));

}

BSPELL(call_lightning)
{
  int result,mana,damage,bonus;

  damage = (int)((500 + (mana_add/3))*mod);

  if (damage > 10000) 
    mana_add = MAX(0, mana_add - (damage - 10000)/(mod/3.0)); 
  damage = MIN(damage, 10000);

  bonus     = IntBonus(ch);
  result    = generic_result_mod(ch,SPELL_CALL_LIGHTNING,NULL,FALSE,bonus);
  mana      = spell_info[SPELL_CALL_LIGHTNING].mana + mana_add;

  GET_TAINT(ch) += add_taint(ch, (int)(mana * 0.4));
  
  switch (result) {
    case TOTAL_FUCKUP:
      mana *=2;
      GET_MANA(ch) -= mana;
      GET_HIT(ch)  -= (damage);
      do_own_damage(ch);
      act("$n draws lightning down on no-one, except himself! LOOOSER!",FALSE,ch,NULL,NULL,TO_ROOM);
      send_to_char("You're a real smeghead, unless you hadn't figured already\r\n",ch);
      GET_TIMES(ch,CAN_WEAVE) =  (7*PULSE_VIOLENCE);
      return;
    case TOTAL_FAILURE:
      mana /= 2;
      GET_MANA(ch) -= mana;
      GET_TIMES(ch,CAN_WEAVE) =  (7*PULSE_VIOLENCE);
      gen_message(SPELL_CALL_LIGHTNING,ch,NULL,FALSE,FALSE);
      return;
      break;
    case SOME_SUCCESS:
      GET_TIMES(ch,CAN_WEAVE) =  (5*PULSE_VIOLENCE);
      break;
    case TOTAL_SUCCESS:
      GET_TIMES(ch,CAN_WEAVE) =  (4*PULSE_VIOLENCE);
      mana /= 2;
      damage = (int) (damage * 1.5);
      break;
  }
  GET_MANA(ch) -= mana;
  check_if_learned(ch,SPELL_CALL_LIGHTNING);
  do_area_spell_agg(ch,damage,SPELL_CALL_LIGHTNING);

}

BSPELL(firesword)
{
  struct obj_data *sword;
  int result,dam_dice = 0,mana,time,bonus,tmp;
 
  tmp       = 4 + (mana_add / spell_info[SPELL_FIRESWORD].mana_add);
  tmp      += (GET_LEVEL(ch) / 20);
  tmp       = (int)(tmp * mod);
  dam_dice  = cap_amount(tmp,CAP_HIGH,40,SPELL_FIRESWORD);
  mana_add -= (long) (cap_mana_add(tmp,dam_dice,(double)spell_info[SPELL_FIRESWORD].mana_add,SPELL_FIRESWORD)/mod);  
  mana_add = MAX(0, mana_add);

  bonus    = IntBonus(ch);
  result   = generic_result_mod(ch,SPELL_FIRESWORD,NULL,FALSE,bonus);
  mana     = spell_info[SPELL_FIRESWORD].mana+mana_add;
  time     = 25;
  time    += (mana_add / spell_info[SPELL_FIRESWORD].mana_add); 
  time     = cap_amount(time,CAP_HIGH,50,SPELL_FIRESWORD);

  GET_TAINT(ch) += add_taint(ch, (int)(mana * 0.5));

  switch (result) {
    case TOTAL_FUCKUP:   
      mana *= 2;
      gen_message(SPELL_FIRESWORD,ch, NULL,FALSE,FALSE);
      GET_MANA(ch) -= mana;
      return;
      break;
    case TOTAL_FAILURE:
      gen_message(SPELL_FIRESWORD,ch, NULL,FALSE,FALSE);
      mana /= 2;
      GET_MANA(ch) -= mana;
      return;
      break;
    case SOME_SUCCESS:
      break;
    case TOTAL_SUCCESS:
      mana /= 2;
      dam_dice = (int) (dam_dice * 1.5);
      time     = (int) (time * 1.5);
      break;
   }

   GET_MANA(ch) -= mana;
   sword = create_obj();

   sword->item_number       = NOTHING;
   sword->in_room           = NOWHERE;
   sword->name              = str_dup("sword fire flame");
   sword->description       = str_dup("A sword carved from Fire");
   sword->short_description = str_dup("A sword carved from Fire");
   sword->action_description= str_dup("A sword carved from Fire");
 
   CREATE(sword->ex_description,struct extra_descr_data,1);
   sword->ex_description->next = NULL;
   sword->ex_description->keyword = str_dup(sword->name);
   sword->ex_description->description = str_dup("A sword made out of a strange looking, "
                        "almost glowing metal.\r\n");

   GET_OBJ_TYPE(sword)      = ITEM_WEAPON;
   GET_OBJ_WEAR(sword)      = ITEM_WEAR_TAKE | ITEM_WEAR_WIELD;
   GET_OBJ_EXTRA(sword)     = ITEM_NODONATE | ITEM_NODROP | ITEM_NOSELL;

   if (is_dreamer(ch)) 
     SET_BIT(OBJ_EXTRA_FLAGS(sword),ITEM_TAR);

   GET_OBJ_VAL(sword,3)     = 3; /* Id for flame-swords! */
   GET_OBJ_VAL(sword,0)	    = 0;
   GET_OBJ_VAL(sword,1)     = MIN(100,dam_dice);
   GET_OBJ_VAL(sword,2)     = 10;
   GET_OBJ_WEIGHT(sword)    = 14;
   GET_OBJ_TIMER(sword)     = time;
   
   gen_message(SPELL_FIRESWORD,ch, NULL,FALSE,TRUE);
   
   if (GET_EQ(ch,WEAR_WIELD_R)) {
     obj_to_char(unequip_char(ch,WEAR_WIELD_R),ch);
     send_to_char("You put your old weapon in your inventory.\r\n",ch);
   } 
   equip_char(ch,sword,WEAR_WIELD_R);
   check_if_learned(ch,SPELL_FIRESWORD);

}


BSPELL(earthshield)
{
  struct obj_data *shield;
  int success,mana,bonus,tmp,valn;
 
  tmp       = (int)((20 + (GET_LEVEL(ch)/10)) * mod);
  valn      = cap_amount(tmp,CAP_HIGH,100,SPELL_EARTHSHIELD);

  bonus   = IntBonus(ch);
  mana    = spell_info[SPELL_EARTHSHIELD].mana;
  success = normal_spell_test(SPELL_EARTHSHIELD, ch, NULL, mana, bonus);

  if (success) {
    shield = create_obj();

    shield->item_number       = NOTHING;
    shield->in_room           = NOWHERE;
    shield->name              = str_dup("shield earth");
    shield->description       = str_dup("A shield with strange earthen texture lies here.");
    shield->short_description = str_dup("an Earthshield");
    shield->action_description= str_dup("An Earthshield");
  
    GET_OBJ_TYPE(shield)      = ITEM_ARMOR;
    GET_OBJ_WEAR(shield)      = ITEM_WEAR_TAKE | ITEM_WEAR_SHIELD;
    GET_OBJ_EXTRA(shield)     = ITEM_NOSELL;

    if (is_dreamer(ch))
      SET_BIT(OBJ_EXTRA_FLAGS(shield),ITEM_TAR);

    GET_OBJ_VAL(shield,3)     = 0; 
    GET_OBJ_VAL(shield,0)     = valn;
    GET_OBJ_VAL(shield,1)     = GET_OBJ_VAL(shield, 0);
    GET_OBJ_VAL(shield,2)     = 0;
    GET_OBJ_WEIGHT(shield)    = 9;
    GET_OBJ_TIMER(shield)     = 10 + GET_LEVEL(ch)/20;
    
    gen_message(SPELL_EARTHSHIELD,ch, NULL,FALSE,TRUE);
   
    if (GET_EQ(ch,WEAR_SHIELD)) {
      obj_to_char(unequip_char(ch,WEAR_SHIELD),ch);
      send_to_char("You put your old weapon in your inventory.\r\n",ch);
    } 
    equip_char(ch,shield,WEAR_SHIELD);
    check_if_learned(ch,SPELL_EARTHSHIELD);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.5));

}

BSPELL(wrap)
{
  struct affected_type af;
  int result, mana, time,bonus,tmp1;
   
  af = clear_aff_var(af);
  
  if (IS_AFFECTED(vict, AFF_WRAPPED))
  {
    send_to_char("Not much use to wrap someone already wrapped, is there?\r\n", ch);
    return;
  }
  bonus  = IntBonus(ch) + calc_weave_bonus(ch,vict);
  result = generic_result_mod(ch, SPELL_WRAP, NULL, FALSE,bonus);
  mana     = spell_info[SPELL_WRAP].mana;
  tmp1     = 10 + (mana_add / spell_info[SPELL_WRAP].mana_add);
  time     = cap_amount(tmp1,CAP_HIGH,50,SPELL_WRAP);
  
  GET_TAINT(ch) += add_taint(ch, (int)(mana * 0.4));

  switch (result) 
  {
    case TOTAL_FUCKUP:     
      mana *= 2;
      gen_message(SPELL_WRAP,ch, vict,TRUE,FALSE);
      break;
    case TOTAL_FAILURE:
      gen_message(SPELL_WRAP,ch, vict,TRUE,FALSE);
      mana /= 2;
      return;
      break;
    case TOTAL_SUCCESS:
      mana /= 2;
      time  = (int) (time * 1.5);      
    case SOME_SUCCESS:
      af.duration = time;
      af.modifier = 0;
      af.location = APPLY_NONE;
      af.bitvector = AFF_WRAPPED;
      af.next = NULL;
      af.type = SPELL_WRAP;
      af.sex  = MadeWith(ch);
      af.tie_info = tied;
      af.weaver = GET_IDNUM(ch);
      af.mana_add = mana_add;
      break;
  }   
  GET_MANA(ch)-=mana;    
  gen_message(SPELL_WRAP,ch, vict,TRUE,TRUE);
  affect_to_char(vict,&af);
  check_if_learned(ch,SPELL_WRAP);

}

BSPELL(entangle)
{
  struct affected_type af;
  int mana, success, time, imod,tmp1;

  af = clear_aff_var(af);
  if (affected_by_spell(vict, SPELL_ENTANGLE)) {
    send_to_char("That person is already pretty stuck.\r\n",ch);
    return;
  }
  tmp1      = 7 + (mana_add / spell_info[SPELL_ENTANGLE].mana_add);
  time      = cap_amount(tmp1,CAP_HIGH,50,SPELL_ENTANGLE);
  mana_add -= cap_mana_add(tmp1,time,(double)spell_info[SPELL_ENTANGLE].mana_add,SPELL_ENTANGLE);

  imod = IntBonus(ch) + calc_weave_bonus(ch,vict);
  mana   = spell_info[SPELL_ENTANGLE].mana + mana_add;

  success  = normal_spell_test(SPELL_ENTANGLE, ch, vict, mana, imod);
  if (success) {
    af.duration  = time;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.type      = SPELL_ENTANGLE;
    af.next      = NULL;
    af.sex       = MadeWith(ch);
    af.tie_info  = tied;
    af.weaver    = GET_IDNUM(ch);
    af.mana_add  = mana_add;
    affect_to_char(vict,&af);
  }

}

BSPELL(gag)
{
  struct affected_type af;
  int mana,result,time,modi,tmp;

  af = clear_aff_var(af);
  if (IS_AFFECTED(vict,AFF_GAGED)) {
    send_to_char("That person are already Gag'ed!\r\n",ch);
    return;
  }

  tmp       = 10 + (mana_add / spell_info[SPELL_GAG].mana_add);
  time      = cap_amount(tmp,CAP_HIGH,50,SPELL_GAG);
  mana_add -= cap_mana_add(tmp,time,(double)spell_info[SPELL_GAG].mana_add,SPELL_GAG);
  modi = IntBonus(ch) + calc_weave_bonus(ch,vict);
  result = generic_result_mod(ch,SPELL_GAG,NULL,FALSE,modi);
  mana   = spell_info[SPELL_GAG].mana + mana_add;

  af.duration  = time;
  af.modifier  = 0;
  af.location  = APPLY_NONE;
  af.bitvector = AFF_GAGED;
  af.type      = SPELL_GAG;
  af.next      = NULL;
  af.sex       = MadeWith(ch);
  af.tie_info  = tied;
  af.weaver    = GET_IDNUM(ch);
  af.mana_add  = mana_add;

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.4));
  
  switch (result) {
    case TOTAL_FUCKUP:
      mana *= 2;
      gen_message(SPELL_GAG,ch,vict,TRUE,FALSE);
      GET_MANA(ch) -= mana;
      return;
      break;
    case TOTAL_FAILURE:
      mana /= 2;
      gen_message(SPELL_GAG,ch,vict,TRUE,FALSE);
      GET_MANA(ch) -= mana;
      return;
      break;
    case SOME_SUCCESS:
      break;
    case TOTAL_SUCCESS:
      af.duration *= 2;
      mana /= 2;
      break;
  } 
  gen_message(SPELL_GAG,ch,vict,TRUE,TRUE);
  mana = ((mana < 0) ? -mana : mana);
  GET_MANA(ch) -= mana;
  affect_to_char(vict,&af);
  check_if_learned(ch,SPELL_GAG);
}

BSPELL(chill)
{
  int success,bonus, mana = 0, dam;

  dam      = (int)((110 + (int) (mana_add/2.5))*mod);
  if (dam > 2000) 
    mana_add = MAX(0, mana_add - (dam - 2000)/(mod/2.5)); 
  dam = MIN(dam, 2000);

  bonus  = IntBonus(ch) + calc_weave_bonus(ch,vict);
  mana   = spell_info[SPELL_CHILL].mana + mana_add;
  success  = normal_spell_test(SPELL_CHILL, ch, vict, mana, bonus);

  if (success) {
    damage(ch, vict, dam, SPELL_CHILL);
    check_if_learned(ch,SPELL_CHILL);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.2));
}

BSPELL(shield)
{
  struct affected_type af;
  int  result, time,bon;
  long mana,tmp1;

  af = clear_aff_var(af);
  
  if (IS_AFFECTED(vict,AFF_SHIELDED)) {
   send_to_char("That person is already shielded.\r\n",ch);
   return;
  }

  if (IS_NPC(vict) || cant_channel(vict))
  {
    send_to_char("What's the use? That person can't channel anyway...\r\n", ch);
    return;
  }  
  tmp1      = 10 + (mana_add / spell_info[SPELL_SHIELD].mana_add);
  time      = cap_amount(tmp1,CAP_HIGH,50,SPELL_SHIELD);
  mana_add -= cap_mana_add(tmp1,time,(double)spell_info[SPELL_SHIELD].mana_add,SPELL_SHIELD);

  bon    = IntBonus(ch) + (2 * calc_weave_bonus(ch,vict));
  result = generic_result_mod(ch,SPELL_SHIELD,NULL,FALSE,bon);
  mana   = spell_info[SPELL_SHIELD].mana + mana_add;
  
  af.duration  = time;
  af.modifier  = 0;
  af.location  = APPLY_NONE;
  af.bitvector = AFF_SHIELDED;
  af.type      = SPELL_SHIELD;
  af.next      = NULL;
  af.sex       = MadeWith(ch);
  af.tie_info  = tied;
  af.weaver    = GET_IDNUM(ch);
  af.mana_add  = mana_add;

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.7));

   switch (result) 
   {
    case TOTAL_FUCKUP:
      mana *= 2;
      gen_message(SPELL_SHIELD,ch,vict,TRUE,FALSE);
      GET_MANA(ch) -= mana;
      return;
      break;
    case TOTAL_FAILURE:
      mana /= 2;
      gen_message(SPELL_SHIELD,ch,vict,TRUE,FALSE);
      GET_MANA(ch) -= mana;
      return;
      break;
    case TOTAL_SUCCESS:
      af.duration *= 2;
      mana /= 2;
      break;
  } 
  
  /* If attacker and defender have equal Spirit and attacker adds no mana to his */
  /* skill, it's 50-50 that the attack succeeds. For every 10 mana the attacker  */
  /* adds to his attack, he gets one % more chance of success			 */
  /* If attackers Spirit is thrice the defenders, attack succeeds automatically.  */
  if (mana_add <= 0)
    mana_add = 1;
  if ((25 + 25*GET_SPIRIT(ch)/GET_SPIRIT(vict) + (mana_add/10)) > number(0,100)) {
    gen_message(SPELL_SHIELD,ch,vict,TRUE,TRUE);
    affect_to_char(vict,&af);
    REMOVE_BIT(PRF_FLAGS(vict), PRF_GRASPING | PRF_TGRASP);
    check_if_learned(ch,SPELL_SHIELD);
    send_to_char("As the shield falls into place, you loose your grip on any weaves you are maintaining!\r\n", vict);
    remove_char_maintained(vict);
    remove_room_maintained(vict);
  }
  else {
    gen_message(SPELL_SHIELD,ch,vict,TRUE,FALSE);
    check_if_learned(ch,SPELL_SHIELD);
  }
  GET_MANA(ch) -= mana;
}


BSPELL(sleeper)
{
  struct affected_type af;
  int success, mana, modi;

  modi   = IntBonus(ch) + calc_weave_bonus(ch,vict);
  mana   = spell_info[SPELL_SLEEP].mana;
  success = normal_spell_test(SPELL_SLEEP, ch, vict, mana, modi);
  af = clear_aff_var(af);

  if (success) {
    if (IS_NPC(vict) && MOB_FLAGGED(vict, MOB_NOSLEEP)) {
      send_to_char("Your flows just glides off the mob! It is invulnerable to this weave!\r\n", ch);
      return;
    }
    af.duration  = 5;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = AFF_SLEEP;
    af.type      = SPELL_SLEEP;
    af.next      = NULL;
    af.sex       = MadeWith(ch);
    af.tie_info  = tied;
    af.weaver    = GET_IDNUM(ch);
    af.mana_add  = 0;

    gen_message(SPELL_SLEEP,ch,vict,TRUE,TRUE);
    affect_to_char(vict, &af);
    check_if_learned(ch, SPELL_SLEEP);
    if (FIGHTING(vict)) {
      if (FIGHTING(FIGHTING(vict)) == vict)
        stop_fighting(FIGHTING(vict));
      stop_fighting(vict);
    }
    GET_POS(vict) = POS_SLEEPING;
    if (IS_SET(PRF_FLAGS(vict),PRF_GRASPING)) {
     send_to_char("As you drift off to sleep, you lose your grip on the One Power.\r\n", vict);
     REMOVE_BIT(PRF_FLAGS(vict),PRF_GRASPING);
  }}
  GET_TAINT(ch) += add_taint(ch, (int)(mana * 0.4));

}

BSPELL(gentle)
{
  int success,mana,bonus,att_spirit, def_spirit,nuffra,modi;
  char *sex_str[] = 
  {
    "cut off from the One Power.\r\n",
    "gentled.\r\n",
    "stilled.\r\n"
  };    

  if (IS_SET(GET_PERMANENT(vict),PRM_GENTLED)) {
    send_to_char("That person is already ",ch);
    send_to_char(sex_str[(int) GET_SEX(vict)],ch);
    return;
  }
  
  if (!can_channel_same_sex(ch,vict) && !can_channel_not_same_sex(ch,vict)) {
    send_to_char("Why? That person can't even channel!\r\n",ch);
    return;
  }
  modi    = IntBonus(ch) + calc_weave_bonus(ch,vict);
  mana   = spell_info[SPELL_GENTLE].mana + mana_add;
  bonus  = (mana_add / spell_info[SPELL_GENTLE].mana_add);
  success = normal_spell_test(SPELL_GENTLE, ch, vict, mana, bonus);

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.8));
  
  if (success) {
    att_spirit = bonus + GET_SPIRIT(ch);
    def_spirit = GET_SPIRIT(vict);
  
    nuffra = 50 + ((att_spirit - def_spirit) * 5);
    
    if ((nuffra > 100) || (number(0,100) < nuffra)) {
      gen_message(SPELL_GENTLE,ch,vict,TRUE,TRUE);
      SET_BIT(GET_PERMANENT(vict),PRM_GENTLED);
      check_if_learned(ch,SPELL_GENTLE);
      return;
    }
    else {
      gen_message(SPELL_GENTLE,ch,vict,TRUE,FALSE);
      return;
    }
  }
}

BSPELL(heal_gentling)
{
  int success,mana,bonus;
     
  if (NULL == vict) {
    send_to_char("Internal error. Please inform a Coder.\r\n",ch);
    alog("SYSERR: heal_gentling()[spells.c]: Got vict = NULL, Caster: %s",GET_NAME(ch));
    return;
  }

  if (!IS_SET(GET_PERMANENT(vict),PRM_GENTLED)) {
    send_to_char("Find someone that NEEDS your damned Healing\r\n",ch);
    return;
  }
  if (!can_channel_same_sex(ch,vict) && !can_channel_not_same_sex(ch,vict)) {
    send_to_char("Find someone that NEEDS your damned Healing\r\n",ch);
    return;
  } 
  bonus = IntBonus(ch);
  mana   = spell_info[SPELL_HEAL_GENTLING].mana + mana_add;
  success = normal_spell_test(SPELL_HEAL_GENTLING, ch, vict, mana, bonus);
  if (success) {
    REMOVE_BIT(GET_PERMANENT(vict),PRM_GENTLED);  
    gen_message(SPELL_HEAL_GENTLING,ch, vict,FALSE,TRUE);
    check_if_learned(ch,SPELL_HEAL_GENTLING);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.8));
  if (IS_IMMORTAL(ch)) {
    alog("(GC) %s used heal gentling on %s.",GET_NAME(ch),GET_NAME(vict));
  }
}

BSPELL(earthquake)
{
  int damage = 0,result,bonus,mana;
  struct char_data *list;
    
  damage = (int)((600 + (mana_add/4)) * mod);

  if (damage > 7500) 
    mana_add = MAX(0, mana_add - (damage - 7500)/(mod/4)); 
  damage = MIN(damage, 7500);
  if (damage < 0)
    damage = 7500;

  mana = (mana_add + spell_info[SPELL_EARTHQUAKE].mana);

 
  bonus  = IntBonus(ch);
  result = generic_result_mod(ch,SPELL_EARTHQUAKE,NULL,FALSE,bonus);
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.5));
 
  switch (result) {
    case TOTAL_FUCKUP :
      GET_MANA(ch) -= (2 * mana); 
      GET_HIT(ch)  -= (damage);
      GET_POS(ch) = POS_SITTING;
      send_to_char("Your uncontrolled channeling hurts no one except yourself!\r\n",ch);
      update_pos(ch);
      GET_TIMES(ch,CAN_WEAVE) =  (7*PULSE_VIOLENCE);
      return;
      break;
    case TOTAL_FAILURE:
      GET_MANA(ch) -= (mana / 2);
      gen_message(SPELL_EARTHQUAKE,ch,NULL,FALSE,FALSE);
      GET_TIMES(ch,CAN_WEAVE) =  (7*PULSE_VIOLENCE);
      return;
      break;
    case SOME_SUCCESS :
      GET_TIMES(ch,CAN_WEAVE) =  (5*PULSE_VIOLENCE);
      GET_MANA(ch) -= mana;
      break;
    case TOTAL_SUCCESS:
      GET_TIMES(ch,CAN_WEAVE) =  (4*PULSE_VIOLENCE);
      damage = (int) (damage * 1.25);
      GET_MANA(ch) -= (mana / 2);
      break;
  }
  do_area_spell(ch,damage,SPELL_EARTHQUAKE);
  check_if_learned(ch,SPELL_EARTHQUAKE);

  for (list = world[ch->in_room].people; list != NULL; list = list->next_in_room) {
    if ((list != ch) && (GET_LEVEL(list) < LVL_IMMORT))
      if (number(0,100) > GET_DEX(list)) {
        send_to_char("You can't keep your balance in this chaos!\r\n",list);
        GET_POS(list) = POS_SITTING;
      }
  }
}

BSPELL(sliceweave)
{
  int result, mana, chance, highest = 0,bonus;
  struct affected_type *af;
  
  bonus  = IntBonus(ch) + calc_weave_bonus(ch,vict);
  result = generic_result_mod(ch,SPELL_SLICEWEAVE,NULL,FALSE,bonus);
  mana     = spell_info[SPELL_SLICEWEAVE].mana + mana_add;

  if (vict->affected)
    for (af = vict->affected; af; af = af->next)
      if (spell_info[af->type].mana && (spell_info[af->type].mana > highest))
        highest = spell_info[af->type].mana;
        
  /* Chance to defeat the weaves is dependant on the spell which demands the */
  /* most mana to be cast, the mana sliceweave costs and the additional      */
  /* mana the caster adds to break through the weaves 			     */
  /* So if both the highest weave and Sliceweave costs 50 mana, base chance  */
  /* to cut through is 50 %. If caster adds 50 mana, the chance is 100 %     */
  /* In reality this should depend on the mana_add the first weave's caster  */
  /* put into the spell when casting, but this info isn't saved, so...       */
  
  chance = (50*mana)/highest;

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.6));

  switch (result) 
  {
    case TOTAL_FUCKUP:
      chance = 0;
      gen_message(SPELL_SLICEWEAVE, ch, vict, TRUE, FALSE);
      break;
    case TOTAL_FAILURE:
      mana = 0;
      chance = 0;
      gen_message(SPELL_SLICEWEAVE, ch, vict, TRUE, FALSE);
      break;
    case TOTAL_SUCCESS:
      mana = (int) mana/2;
      chance += 25;
      break;
    case SOME_SUCCESS :
      break;
  }
  GET_MANA(ch) -= mana;
  if (!chance)   /* I like that one :) */
    return;
   
  if (number(0,100) < chance)
  {
    gen_message(SPELL_SLICEWEAVE, ch, vict, TRUE, TRUE);
    if (vict->affected)
      while (vict->affected)
        affect_remove(vict, vict->affected);
     check_if_learned(ch,SPELL_SLICEWEAVE);
     return;
  }
  gen_message(SPELL_SLICEWEAVE, ch, vict, TRUE, FALSE);  
}

BSPELL(whirlwind)
{
  int success, mana, damage, bonus;

  bonus  = IntBonus(ch);
  damage =(int)((200 + 25*GET_AIR(ch))*mod);

  if (damage > 5000) 
    mana_add = MAX(0, mana_add - (damage - 5000)/mod); 
  damage = MIN(damage, 5000);
  mana = spell_info[SPELL_WHIRLWIND].mana + mana_add;

  success = normal_spell_test(SPELL_WHIRLWIND, ch, NULL, mana, bonus);
  if (success) {
    GET_TIMES(ch,CAN_WEAVE) =  (PULSE_VIOLENCE * 3);
    do_area_ngroup_agg(ch, damage,SPELL_WHIRLWIND);   
    check_if_learned(ch,SPELL_WHIRLWIND);
  }
  
  GET_TAINT(ch) += add_taint(ch, (int)(mana * 0.6));
}

int already_warded(room_num room) 
{
  if (ROOM_AFFECTED(room,RAFF_WARDED_HUMAN)) { /* add more when needed. :) */
    return 1;
  }
  return 0;
}

void do_warding(room_num room,struct affected_type *af,struct char_data *ch,int d) 
{
  char tmp[150];
  char *my_dir[] = {"to the north","to the east",
            "to the south","to the west",
                    "upwards","downwards","to the northeast",
            "to the southeast","to the southwest",
            "to the northwest"};

  if (already_warded(room)) {
    sprintf(tmp,"Something stops your ward from working %s.\n",my_dir[d]);
    send_to_char(tmp,ch);
  }
  else {
    affect_to_room(&world[room],af);
  }
}

BSPELL(ward_human)
{
  int bonus,mana,success,i;
   struct affected_type af;

  bonus   = IntBonus(ch);
  mana    = spell_info[SPELL_WARDHUMAN].mana + mana_add;
  success = normal_spell_test(SPELL_WARDHUMAN,ch,NULL,mana,bonus);
  GET_TAINT(ch) += add_taint(ch,(int)(mana *0.6));

  if (success) {
    check_if_learned(ch,SPELL_WARDHUMAN);
    gen_message(SPELL_WARDHUMAN,ch,NULL,FALSE,TRUE);    
    af.duration  = 30;
    af.bitvector = RAFF_WARDED_HUMAN;
    af.speed     = RAFF_SLOW;
    af.type      = SPELL_WARDHUMAN;
    af.sex       = MadeWith(ch);
    af.next      = NULL;
    af.tie_info  = tied;
    af.weaver    = GET_IDNUM(ch);
    af.mana_add  = mana_add;
    
    for (i = 0; i < NUM_OF_DIRS; i++) {
      if ((NULL != world[ch->in_room].dir_option[i]) && (NOWHERE != world[ch->in_room].dir_option[i]->to_room)) {
        do_warding(world[ch->in_room].dir_option[i]->to_room,&af,ch,i);
      }
    }
    asend_to_char(ch, "You wove a ward.\r\n");
  }
}

BSPELL(fireball) // Yes I am screwing with this Rei
{
  /* int success,mana,bonus, dam, tmp, orig_mana_add, mana_add = orig_mana_add; */
  int success,mana,bonus, dam, tmp, orig_mana_add = mana_add;
  /*  tmp       = (int)((300 + dice(mana_add, 3)) *mod);
   *
   * <manadd>d3 replaced with E(1d3)*<manaadd>
   */
  tmp = (int)((300 + 1.5*mana_add)*mod);
  /*
   * If damage overflows, i.e gets negative, then we shoudl give 
   * maximum damage, not minimum!
   */
  tmp       = ((tmp < 0) ? 4000 : tmp); 
  dam       = cap_amount(tmp,CAP_HIGH,4000,SPELL_FIREBALL);
  mana_add -= (long) (cap_mana_add(tmp, dam,(1/3.0),SPELL_FIREBALL)/mod);
  bonus  = IntBonus(ch) + calc_weave_bonus(ch,vict);
  mana_add = MIN(orig_mana_add, mana_add); // Something's wrong here...
  mana   = spell_info[SPELL_FIREBALL].mana + orig_mana_add;
  success = normal_spell_test(SPELL_FIREBALL, ch, vict, mana, bonus);

  dam = MIN(dam,4000);

  if (success) {
    GET_TIMES(ch,CAN_WEAVE) =  (PULSE_VIOLENCE * 3); 
    damage(ch, vict, dam, SPELL_FIREBALL);    
    check_if_learned(ch,SPELL_FIREBALL);
  }   

  GET_TAINT(ch) += add_taint(ch, (int)(mana * 0.8));
}

BSPELL(create_wat)
{
  int mana, success;
  struct obj_data *well;

  mana = spell_info[SPELL_CREATE_WATER].mana;
  success = normal_spell_test(SPELL_CREATE_WATER, ch, vict, mana, 0);
  if (success) {
    well = create_obj();

    well->item_number       = NOTHING;
    well->in_room           = NOWHERE;
    well->name              = str_dup("well water");
    well->description       = str_dup("A small well of crystal clear water is here.");
    well->short_description = str_dup("a small well of water");
    well->action_description= str_dup("a small well");
 
    GET_OBJ_TYPE(well)   = ITEM_FOUNTAIN; 
    GET_OBJ_WEAR(well)   = 0;
    GET_OBJ_EXTRA(well)  = ITEM_NODONATE | ITEM_NODROP | ITEM_NOSELL;

    if (is_dreamer(ch))
      SET_BIT(OBJ_EXTRA_FLAGS(well),ITEM_TAR);

    GET_OBJ_VAL(well,3)  = 0; 
    GET_OBJ_VAL(well,0)	 = 10000;
    GET_OBJ_VAL(well,1)  = 10000;
    GET_OBJ_VAL(well,2)  = 0;
    GET_OBJ_WEIGHT(well) = 10000;
    GET_OBJ_TIMER(well)  = 5;
    obj_to_room(well, ch->in_room);
    gen_message(SPELL_CREATE_WATER,ch,NULL,FALSE,TRUE); 
    check_if_learned(ch,SPELL_CREATE_WATER);
  }
  GET_TAINT(ch) += add_taint(ch, (int)(mana * 0.3));
}

BSPELL(create_food)
{
  int mana,success,num,i;
  struct obj_data *shrooms[MAX_SHROOMS];

  mana = spell_info[SPELL_CREATE_FOOD].mana;
  success = normal_spell_test(SPELL_CREATE_FOOD,ch,NULL,mana,0);

  num = MAX(1,(GET_LEVEL(ch) / 20));

  if (success) {
    for (i = 0; i < num; i++) {
      shrooms[i] = read_object(SHROOM_NUM,VIRTUAL);

      if (is_dreamer(ch)) 
        SET_BIT(OBJ_EXTRA_FLAGS(shrooms[i]),ITEM_TAR);

      obj_to_room(shrooms[i],ch->in_room);
    }
    gen_message(SPELL_CREATE_FOOD,ch,NULL,FALSE,TRUE);
    check_if_learned(ch,SPELL_CREATE_FOOD);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.3));
}

BSPELL(delve)
{
  int mana, result, bonus;
  
  mana   = spell_info[SPELL_DELVE].mana + mana_add;
  bonus  = IntBonus(ch);
  result = generic_result_mod(ch,SPELL_DELVE,NULL,FALSE,bonus);

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.4));

  switch(result) {
    case TOTAL_FUCKUP:
        mana = (mana * 2);
                GET_HIT(vict) -= dice(2,20);
                update_pos(vict);
                send_to_char("Instead of delving your victim, you harm it more!\r\n",ch);
                act("Instead of delving you, $n hurts you!",FALSE,ch,NULL,vict,TO_VICT);
                GET_MANA(ch) -= mana;
                return;
        break;
    case TOTAL_FAILURE:
            mana /= 2;
                GET_MANA(ch) -= mana;
                gen_message(SPELL_DELVE,ch,vict,TRUE,FALSE);
                return;
        break;
    case SOME_SUCCESS:
        break;
    case TOTAL_SUCCESS:
        mana /= 2;
        break;
  }
  GET_HIT(vict) = (int)(MIN(GET_MAX_HIT(vict), GET_HIT(vict) + 200 + dice(3,8))*mod);
  GET_MOVE(vict) = (int)(MIN(GET_MAX_MOVE(vict), GET_MOVE(vict) + 100 + dice(3,8))*mod);
  GET_FRACT_MOVE(vict) = 0;
  GET_MANA(ch)    -= mana;
  update_pos(vict);
  gen_message(SPELL_DELVE,ch,vict,TRUE,TRUE);
  check_if_learned(ch,SPELL_DELVE);
}

BSPELL(heal)
{
  int mana, result, bonus,ndice;
  struct affected_type *af, *af_next;
  mana   = spell_info[SPELL_HEAL].mana + mana_add;
  bonus  = IntBonus(ch);
  result = generic_result_mod(ch,SPELL_HEAL,NULL,FALSE,bonus);
  ndice   = (int)((20 + (GET_LEVEL(ch) / 25) + (90*mana_add/spell_info[SPELL_HEAL].mana_add))*mod);

  GET_TAINT(ch) += add_taint(ch, (int) (mana * 0.7));

  switch(result) {
    case TOTAL_FUCKUP:
                GET_TIMES(ch,CAN_WEAVE) =  (7*PULSE_VIOLENCE);
                GET_HIT(vict) -= dice(ndice,20);
                update_pos(vict);
                send_to_char("Instead of healing your victim, you harm it more!\r\n",ch);
                act("Instead of healing you, $n hurts you!",FALSE,ch,NULL,vict,TO_VICT);
                GET_MANA(ch) -= mana;
                return;
        break;
    case TOTAL_FAILURE:
                GET_TIMES(ch,CAN_WEAVE) =  (6*PULSE_VIOLENCE);
            mana /= 2;
                GET_MANA(ch) -= mana;
                gen_message(SPELL_HEAL,ch,vict,TRUE,FALSE);
                return;
        break;
    case SOME_SUCCESS:
                GET_TIMES(ch,CAN_WEAVE) =  (5*PULSE_VIOLENCE);
        break;
    case TOTAL_SUCCESS:
                GET_TIMES(ch,CAN_WEAVE) =  (4*PULSE_VIOLENCE);
        mana /= 2;
                ndice += (int) (1.75 * ndice);
        break;
  }
  GET_HIT(vict) = MIN(GET_MAX_HIT(vict), GET_HIT(vict) + dice(ndice,20));
  GET_MANA(ch)  -= mana;
  if (IS_AFFECTED(vict, AFF_POISON | AFF_BLIND))  /* Heal removes all poisons as well */
    for (af = vict->affected; af; af = af_next)  {
      af_next = af->next;
      if (IS_SET(af->bitvector, AFF_POISON | AFF_BLIND)) 
        affect_remove(vict, af);   
    }
  gen_message(SPELL_HEAL,ch,vict,TRUE,TRUE);
  check_if_learned(ch,SPELL_HEAL);
}

BSPELL(heal_II)
{
  int success,mana,bonus;
  struct affected_type *af, *af_next;
  bonus  = IntBonus(ch);
  mana   = spell_info[SPELL_HEAL_II].mana;
  success  = normal_spell_test(SPELL_HEAL_II, ch, vict, mana, bonus);
  
  if (success) {
    GET_HIT(vict) = (int) GET_MAX_HIT(vict)*0.9;
    GET_MOVE(vict) = 0;
    GET_FRACT_MOVE(vict) = 0;
    GET_MOVE(ch) = 0;
    GET_FRACT_MOVE(ch) = 0;
    /* This is bad! un-gentles, re-grows lost limbs, and removes Taint resistance (if that's still in the code)
     *
    GET_PERMANENT(vict) = 0;
    */
    REMOVE_BIT(GET_PERMANENT(vict),PRM_BLIND | PRM_DEAF |PRM_MUTE | PRM_CRIPPLED);

    gen_message(SPELL_HEAL_II,ch,vict,TRUE,TRUE);
    if (IS_AFFECTED(vict, AFF_POISON | AFF_BLIND))  /* Heal removes all poisons as well */
      for (af = vict->affected; af; af = af_next)  {
        af_next = af->next;
        if (IS_SET(af->bitvector, AFF_POISON | AFF_BLIND)) 
          affect_remove(vict, af);   
    }
    check_if_learned(ch,SPELL_HEAL_II);
  }
  else
    gen_message(SPELL_HEAL_II,ch,vict,TRUE,FALSE);

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.7));
}

BSPELL(airsword)
{
  struct obj_data *sword;
  int result,dam_dice = 0,mana,time,bonus,tmp;
 
  bonus    = IntBonus(ch);
  result   = generic_result_mod(ch,SPELL_AIRSWORD,NULL,FALSE,bonus);
  mana     = spell_info[SPELL_AIRSWORD].mana + mana_add;
  tmp = 4 + (mana_add / spell_info[SPELL_AIRSWORD].mana_add);
  tmp += (GET_LEVEL(ch) / 20);
  tmp = (int)(tmp * mod);
  dam_dice  = cap_amount(tmp,CAP_HIGH,40,SPELL_AIRSWORD);
  time     = 25;
  time    += (mana_add / spell_info[SPELL_AIRSWORD].mana_add); 

  GET_TAINT(ch) += add_taint(ch, (int)(mana * 0.9));

  switch (result) {
    case TOTAL_FUCKUP:   
      mana *= 2;
      gen_message(SPELL_AIRSWORD,ch, NULL,FALSE,FALSE);
      GET_MANA(ch) -= mana;
      return;
      break;
    case TOTAL_FAILURE:
      gen_message(SPELL_AIRSWORD,ch, NULL,FALSE,FALSE);
      mana /= 2;
      GET_MANA(ch) -= mana;
      return;
      break;
    case SOME_SUCCESS:
      break;
    case TOTAL_SUCCESS:
      mana /= 2;
      dam_dice = (int) (dam_dice * 1.5);
      time     = (int) (time * 1.5);
      break;
   }

   GET_MANA(ch) -= mana;
   sword = create_obj();

   sword->item_number       = NOTHING;
   sword->in_room           = NOWHERE;
   sword->name              = str_dup("sword air");
   sword->description       = str_dup("A sword carved from Air");
   sword->short_description = str_dup("A sword carved from Air");
   sword->action_description= str_dup("A sword carved from Air");
 
   CREATE(sword->ex_description,struct extra_descr_data,1);
   sword->ex_description->next = NULL;
   sword->ex_description->keyword = str_dup(sword->name);
   sword->ex_description->description = str_dup("A sword made out of a strange looking,\r\n"
                        "almost glowing metal.\r\n");

   GET_OBJ_TYPE(sword)      = ITEM_WEAPON;
   GET_OBJ_WEAR(sword)      = ITEM_WEAR_TAKE | ITEM_WEAR_WIELD;
   GET_OBJ_EXTRA(sword)     = ITEM_NODONATE | ITEM_NODROP | ITEM_NOSELL;

   if (is_dreamer(ch)) 
     SET_BIT(OBJ_EXTRA_FLAGS(sword),ITEM_TAR);

   GET_OBJ_VAL(sword,3)     = 3; /* Id for flame-swords! */
   GET_OBJ_VAL(sword,0)	    = 0;
   GET_OBJ_VAL(sword,1)     = MIN(dam_dice, 80);
   GET_OBJ_VAL(sword,2)     = 10;
   GET_OBJ_WEIGHT(sword)    = 14;
   GET_OBJ_TIMER(sword)     = time;
   
   gen_message(SPELL_AIRSWORD,ch, NULL,FALSE,TRUE);
   
   if (GET_EQ(ch,WEAR_WIELD_R)) {
     obj_to_char(unequip_char(ch,WEAR_WIELD_R),ch);
     send_to_char("You put your old weapon in your inventory.\r\n",ch);
   } 
   equip_char(ch,sword,WEAR_WIELD_R);
   check_if_learned(ch,SPELL_AIRSWORD);
}




BSPELL(burn) /* No 17 */
{
  int tmp1, dam, success,mana,bonus,ndice;

  tmp1      = (int) ((150 + (mana_add/2))*mod);
  tmp1      = ((tmp1 < 0) ? 32000 : tmp1);
  dam       = cap_amount(tmp1,CAP_HIGH,5000,SPELL_BURN);

  bonus  = IntBonus(ch) + calc_weave_bonus(ch,vict);
  mana_add -= (long) (cap_mana_add(tmp1,dam,2.0,SPELL_BURN)/mod);
  mana   = spell_info[SPELL_BURN].mana + mana_add;
  success  = normal_spell_test(SPELL_BURN, ch, vict, mana, bonus);
  ndice  = 4 + (mana_add / spell_info[SPELL_BURN].mana_add) + (GET_LEVEL(ch) / 30);
  ndice  = (int)(ndice * mod);
  if (success) {
    damage(ch, vict, dice(ndice,10), SPELL_BURN);
    check_if_learned(ch,SPELL_BURN);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.4));
}

BSPELL(airblow) /* No 18 */
{
  int success,mana = 0,bonus,dam,tmp1;

  bonus  = IntBonus(ch) + calc_weave_bonus(ch,vict);

//asend_to_char(ch,"mana_add = %d\r\n",mana_add);
  tmp1      = (int) ((150 + (mana_add/2))*mod);
  tmp1      = ((tmp1 < 0) ? 32000 : tmp1);
  dam       = cap_amount(tmp1,CAP_HIGH,3000,SPELL_AIRBLOW);
  mana_add -= (long) (cap_mana_add(tmp1,dam,2.0,SPELL_AIRBLOW)/mod);
  mana      = spell_info[SPELL_AIRBLOW].mana + mana_add;
  success  = normal_spell_test(SPELL_AIRBLOW, ch, vict, mana, bonus);

//asend_to_char(ch,"tmp1 = %d, dam = %d, mana_add = %d\r\n",tmp1,dam,mana_add);

  if (success) {
    GET_TIMES(ch,CAN_WEAVE) =  (PULSE_VIOLENCE); 
    damage(ch, vict, dam, SPELL_AIRBLOW);
    check_if_learned(ch,SPELL_AIRBLOW); 
  }

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.5));
}

BSPELL(ice_arrows) /* No 19 */
{
  int dam,success,bonus,mana;

  dam = (int)((100 + mana_add)*mod);

  if (dam > 2000) 
    mana_add = MAX(0, mana_add - (dam - 2000)/mod); 
  dam = MIN(dam, 2000);

  bonus = IntBonus(ch) + calc_weave_bonus(ch,vict);
  mana  = spell_info[SPELL_ICE_ARROWS].mana + mana_add;

  success  = normal_spell_test(SPELL_ICE_ARROWS, ch, vict, mana, bonus);
  if (success) {
    GET_TIMES(ch,CAN_WEAVE) =  (PULSE_VIOLENCE * 1); 
    damage(ch, vict, dam, SPELL_ICE_ARROWS);
    check_if_learned(ch,SPELL_ICE_ARROWS);  
  }

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.5));
}

BSPELL(detect_magic)
{
}

BSPELL(ball_of_air)
{
  int success,bonus,mana, dam;
  dam = (300 + (mana_add/2))*mod;

  if (dam > 5000) 
    mana_add = MAX(0, mana_add - (dam - 5000)/(mod/2.0)); 
  dam = MIN(dam, 5000);

  bonus  = IntBonus(ch);
  mana   = spell_info[SPELL_BALLOFAIR].mana + mana_add;
  success  = normal_spell_test(SPELL_BALLOFAIR, ch, vict, mana, bonus);
  if (success) {
    damage(ch, vict, (int)((300 + (mana_add/2))*mod), SPELL_BALLOFAIR);
    check_if_learned(ch,SPELL_BALLOFAIR);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.3));
}

BSPELL(drown)
{
  struct affected_type af;
  int success, mana,time,bonus;
   
  af = clear_aff_var(af);

  if (IS_AFFECTED(vict, AFF_SUFFOCATING)){
    send_to_char("That one is already suffocating!\r\n", ch);
    return;
  }  
  bonus  = IntBonus(ch) + calc_weave_bonus(ch,vict);
  mana     = spell_info[SPELL_DROWN].mana + mana_add;
  success  = normal_spell_test(SPELL_DROWN, ch, vict, mana, bonus);
  if (success) {  
    time   = 4;
    time  += (2*mana_add / spell_info[SPELL_DROWN].mana_add); 
    gen_message(SPELL_DROWN,ch, vict,TRUE,TRUE);
    af.duration = time;
    af.modifier = 0;
    af.location = APPLY_NONE;
    af.bitvector = AFF_SUFFOCATING;
    af.next = NULL;
    af.type = SPELL_DROWN;
    af.sex  = MadeWith(ch);
    af.tie_info = tied;
    af.mana_add = mana_add;
    af.weaver = GET_IDNUM(ch);
    affect_to_char(vict,&af);
    check_if_learned(ch,SPELL_DROWN);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.6));
}

BSPELL(poison)
{
  struct affected_type af;
  int success, mana,time,bonus;
   
  af = clear_aff_var(af);

  if (IS_AFFECTED(vict, AFF_POISON)){
    send_to_char("That one is already poisoned!\r\n", ch);
    return;
  }  
  bonus  = IntBonus(ch) + calc_weave_bonus(ch,vict);
  mana     = spell_info[SPELL_POISON].mana + mana_add;
  success  = normal_spell_test(SPELL_POISON, ch, vict, mana, bonus);
  if (success) {  
    time   = 7;
    time  += (mana_add / spell_info[SPELL_POISON].mana_add); 
    gen_message(SPELL_POISON,ch, vict,TRUE,TRUE);
    af.duration = time;
    af.modifier = 0;
    af.location = APPLY_NONE;
    af.bitvector = AFF_POISON;
    af.next = NULL;
    af.type = SPELL_POISON;
    af.sex  = MadeWith(ch);
    af.tie_info = tied;
    af.weaver = GET_IDNUM(ch);
    af.mana_add = mana_add;
    affect_to_char(vict,&af);
    check_if_learned(ch,SPELL_POISON);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.6));
}

BSPELL(disintegrate)
{
  int success,bonus,mana, dam;

  bonus  = IntBonus(ch);
  dam = (800 + mana_add)*mod;

  if (dam > 7000) 
    mana_add = MAX(0, mana_add - (dam - 7000)/mod); 
  dam = MIN(dam, 7000);

  mana   = spell_info[SPELL_DISINTEGRATE].mana + mana_add;
  success  = normal_spell_test(SPELL_DISINTEGRATE, ch, vict, mana, bonus);
  if (success) {
    damage(ch, vict, dam, SPELL_DISINTEGRATE);
    check_if_learned(ch,SPELL_DISINTEGRATE);
    GET_TIMES(ch,CAN_WEAVE) = (3 * PULSE_VIOLENCE);    
  }

  GET_TAINT(ch) += add_taint(ch, (int)(mana * 0.9));
}

BSPELL(meteor) /* No 21 */
{
  int dam,success,bonus,mana;

  bonus  = IntBonus(ch);
  dam = (int)((650 + (mana_add/2))*mod);

  if (dam > 7500) 
    mana_add = MAX(0, mana_add - (dam - 7500)/(mod/3)); 
  dam = MIN(dam, 7500);

  mana   = spell_info[SPELL_METEOR].mana + mana_add;
  success  = normal_spell_test(SPELL_METEOR, ch, vict, mana, bonus);
  if (success) {
    GET_TIMES(ch,CAN_WEAVE) =  (4*PULSE_VIOLENCE);
    do_area_spell_agg(ch,dam,SPELL_METEOR);
    check_if_learned(ch,SPELL_METEOR);
  }

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.8));
}

BSPELL(lightning) /* No 25 */
{
  int mana,success,bonus, dam;

  dam = (550 + mana_add/2)*mod;
  if (dam > 6000) 
    mana_add = MAX(0, mana_add - (dam - 6000)/(mod/2)); 
  dam = MIN(dam, 6000);

  bonus  = IntBonus(ch) + calc_weave_bonus(ch,vict);

  mana   = spell_info[SPELL_LIGHTNING].mana + mana_add;

  success  = normal_spell_test(SPELL_LIGHTNING, ch, vict, mana, bonus);
  if (success) {
    damage(ch, vict, dam, SPELL_LIGHTNING);
    check_if_learned(ch,SPELL_LIGHTNING);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.7));
}

BSPELL(rolling_ring) /* No 24 */
{
  int bonus,result,mana,damag,num=0;
  struct follow_type *f;
  struct char_data   *k = NULL;

  damag = 3000 + (5*GET_LEVEL(ch)) + dice((mana_add/400),50);

  if (IS_AFFECTED(ch,AFF_GROUP)) {
    k = (ch->master ? ch->master : ch);
    for (f = k->followers; f != NULL; f = f->next) {
      if (!IS_AFFECTED(f->follower,AFF_GROUP))
        continue;
      if (get_guild(f->follower,8))
        num++;
    }
  }

  if (k != ch)
    num++;

  damag  += (500 * num);
  damag = (int)(damag * mod);
  mana    = spell_info[SPELL_ROLLING_RING].mana + mana_add;
  bonus   = IntBonus(ch);
  result  = generic_result_mod(ch,SPELL_ROLLING_RING,NULL,FALSE,bonus);
  
  GET_TAINT(ch) += add_taint(ch, (2 * mana));

  switch (result) {
    case TOTAL_FUCKUP:
            mana *= 2;
            GET_MANA(ch) -= mana;
                GET_TIMES(ch,CAN_WEAVE) =  (7*PULSE_VIOLENCE);
                damage(ch,ch,damag,SPELL_ROLLING_RING);
            return;
        break;
    case TOTAL_FAILURE:
                GET_TIMES(ch,CAN_WEAVE) =  (6*PULSE_VIOLENCE);
        mana /= 2;
        GET_MANA(ch) -= mana;
        gen_message(SPELL_ROLLING_RING,ch,NULL,FALSE,FALSE);
        return;
        break;
    case SOME_SUCCESS:
                GET_TIMES(ch,CAN_WEAVE) =  (5*PULSE_VIOLENCE);
        break;
    case TOTAL_SUCCESS:
                GET_TIMES(ch,CAN_WEAVE) =  (4*PULSE_VIOLENCE);
        mana /= 2;
        damag = (int) (1.25 * damag);
        break;
  }
  GET_MANA(ch) -= mana;

  if (damag < 0 || damag > 20000)
    damag = 20000;

  check_if_learned(ch,SPELL_ROLLING_RING);
  do_area_ngroup_agg(ch,damag,SPELL_ROLLING_RING);
}

BSPELL(mend)
{
  int bonus, success, mana;

  if (!obj) {
    send_to_char("This spell has a bug, object missing. Tell an imm.\r\n", ch);
    return;
  }

  if ((GET_OBJ_TYPE(obj) != ITEM_WEAPON) &&
      (GET_OBJ_TYPE(obj) != ITEM_BOW) &&
      (GET_OBJ_TYPE(obj) != ITEM_ARMOR)) {
    act("$p is not any object that needs repairs.", TRUE, ch, obj, 0, TO_CHAR);
    return;
  }
  bonus  = IntBonus(ch);
  mana   = spell_info[SPELL_MEND].mana + mana_add;
  success  = normal_spell_test(SPELL_MEND, ch, NULL, mana, bonus);
  if (success) 
    GET_OBJ_STATUS(obj) = MIN(100, GET_OBJ_STATUS(obj) + mana/10);
  else if (number(0, 100) < 5) {
    act("Ack! You fumbled and $p turns to dust! Disaster!", TRUE, ch, obj, 0, TO_CHAR);
    act("$p turns to dust in the hands of a chagrined $n.", TRUE, ch, obj, 0, TO_ROOM);
    obj_from_char(obj);
    extract_obj(obj);
  }
}

BSPELL(destroy_weapon)
{
  int bonus, success, mana, place;
  
  if (!GET_EQ(vict, WEAR_WIELD_R)) {
    if (!GET_EQ(vict, WEAR_WIELD_L)) {
      send_to_char("Your target doesn't wield any weapon.\r\n", ch);
      return;
    } 
    else
      place = WEAR_WIELD_L;
  }
  else
    place = WEAR_WIELD_R;
  
  bonus  = IntBonus(ch);
  mana   = spell_info[SPELL_DESTROY_WEAPON].mana + mana_add;
  success  = normal_spell_test(SPELL_DESTROY_WEAPON, ch, vict, mana, bonus);
  if (success) {
    extract_obj(unequip_char(vict, place));
    damage(ch, vict, (int)((75 + (mana_add/2))*mod), SPELL_DESTROY_WEAPON);
    check_if_learned(ch,SPELL_DESTROY_WEAPON);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 1.1));
}

BSPELL(dart_of_air)
{
  int success, mana, dam;

  dam = (dice(20,10) + (mana_add/2))*mod;
  if (dam > 1500) 
    mana_add = MAX(0, mana_add - (dam - 1500)/(mod/2)); 
  dam = MIN(dam, 1500);

  mana = spell_info[SPELL_DARTOFAIR].mana + mana_add;
  success = normal_spell_test(SPELL_DARTOFAIR, ch, NULL, mana, 0);
  if (success) {
    GET_TIMES(ch,CAN_WEAVE) =  (PULSE_VIOLENCE * 1); 
    damage(ch, vict, dam,SPELL_DARTOFAIR);
    check_if_learned(ch,SPELL_DARTOFAIR);  
  }
 
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.4));
}

BSPELL(call_frost)
{
  int bonus,success,mana, dam;
  dam = (800 + mana_add/2)*mod;

  if (dam > 10000) 
    mana_add = MAX(0, mana_add - (dam - 10000)/(mod/2)); 
  dam = MIN(dam, 10000);

  mana   = spell_info[SPELL_CALL_FROST].mana + mana_add;
  bonus  = IntBonus(ch);
  success  = normal_spell_test(SPELL_CALL_FROST, ch, NULL, mana, bonus);
  if (success) {
    do_area_ngroup_agg(ch, (int)((800 + (mana_add/2))*mod),SPELL_CALL_FROST);
    check_if_learned(ch,SPELL_CALL_FROST);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.7));
}

BSPELL(storm)
{
  int success, mana, dam;
  struct char_data *list = NULL;
  
  dam = (int)((300 + (mana_add/2))*mod);

  if (dam > 5000) 
    mana_add = MAX(0, mana_add - (dam - 10000)/(mod/2)); 
  dam = MIN(dam, 5000);

  mana = spell_info[SPELL_STORM].mana + mana_add;

  success = normal_spell_test(SPELL_STORM, ch, NULL, mana, 0);
  if (success)  {
    GET_TIMES(ch,CAN_WEAVE) =  (3*PULSE_VIOLENCE);
    do_area_spell_agg(ch, dam,SPELL_STORM); 
    GET_HIT(ch) -= (int)(100*mod);
    for (list = world[ch->in_room].people; list; list = list->next_in_room) {
      GET_MOVE(list) -= (int)(dice(5, 20)*mod);
      GET_FRACT_MOVE(list) = 0;
      GET_POS(list) = POS_SITTING;
    }
    check_if_learned(ch,SPELL_STORM);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.9));
}

BSPELL(call_fire)
{
  int bonus,success,mana, dam;

  dam = (800 + mana_add/2)*mod;
  if (dam > 10000) 
    mana_add = MAX(0, mana_add - (dam - 10000)/(mod/2)); 
  dam = MIN(dam, 10000);

  mana   = spell_info[SPELL_CALL_FIRE].mana + mana_add;
  bonus  = IntBonus(ch);
  success  = normal_spell_test(SPELL_CALL_FIRE, ch, NULL, mana, bonus);
  if (success) {
    do_area_ngroup_agg(ch, dam,SPELL_CALL_FIRE);
    check_if_learned(ch,SPELL_CALL_FIRE);
  }
  GET_TAINT(ch) += add_taint(ch, (int)(mana * 0.7));
}

BSPELL(thunderburst)
{
  int bonus,success,mana, dam;
  dam = (1500 + mana_add/2)*mod;

  if (dam > 8000) 
    mana_add = MAX(0, mana_add - (dam - 8000)/(mod/2)); 
  dam = MIN(dam, 8000);

  mana   = spell_info[SPELL_THUNDERBURST].mana + mana_add;
  bonus  = IntBonus(ch);
  success  = normal_spell_test(SPELL_THUNDERBURST, ch, NULL, mana, bonus);
  if (success) {
    GET_TIMES(ch,CAN_WEAVE) =  (PULSE_VIOLENCE * 4);
    do_area_ngroup_agg(ch, dam, SPELL_THUNDERBURST);
    check_if_learned(ch,SPELL_THUNDERBURST);
  }

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 1.5));
}

BSPELL(stormsfury)
{
  int bonus,success,mana, dam;
  dam = (1500 + mana_add/2)*mod;

  if (dam > 8000) 
    mana_add = MAX(0, mana_add - (dam - 8000)/(mod/2)); 
  dam = MIN(dam, 8000);

  mana   = spell_info[SPELL_STORMSFURY].mana + mana_add;
  bonus  = IntBonus(ch);
  success  = normal_spell_test(SPELL_STORMSFURY, ch, NULL, mana, bonus);
  if (success) {
    GET_TIMES(ch,CAN_WEAVE) =  (PULSE_VIOLENCE * 4);
    do_area_ngroup_agg(ch, dam, SPELL_STORMSFURY);
    check_if_learned(ch,SPELL_STORMSFURY);
  }

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 1.5));
}

BSPELL(shaft_of_fire)
{
  int mana, success, bonus, dam;
  bonus  = IntBonus(ch);
  dam = (500 + mana_add)*mod;
  if (dam > 4500) 
    mana_add = MAX(0, mana_add - (dam - 4500)/mod); 
  dam = MIN(dam, 4500);

  mana   = spell_info[SPELL_SHAFTOFFIRE].mana + mana_add;
  success = normal_spell_test(SPELL_SHAFTOFFIRE, ch, vict, mana, IntBonus(ch));  
  if (success) {
    GET_TIMES(ch,CAN_WEAVE) =  (PULSE_VIOLENCE * 1);
    damage(ch, vict, dam, SPELL_SHAFTOFFIRE);
    check_if_learned(ch,SPELL_SHAFTOFFIRE);
  }

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.8));
}

BSPELL(shaft_of_frost)
{
  int mana, success, bonus, dam;
  bonus  = IntBonus(ch);

  dam = (600 + mana_add/2)*mod;
  if (dam > 4500) 
    mana_add = MAX(0, mana_add - (dam - 4500)/(mod/2)); 
  dam = MIN(dam, 4500);

  mana   = spell_info[SPELL_SHAFTOFFROST].mana + mana_add;
  success = normal_spell_test(SPELL_SHAFTOFFROST, ch, vict, mana, IntBonus(ch));  
  if (success) {
    GET_TIMES(ch,CAN_WEAVE) =  (PULSE_VIOLENCE * 1);
    damage(ch, vict, dam, SPELL_SHAFTOFFROST);
    check_if_learned(ch,SPELL_SHAFTOFFROST);  
  }

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.8));
}

BSPELL(open_lock)
{
  int mana,success;
  mana  = spell_info[SPELL_OPEN_LOCK].mana;

  if (-1 != direction) {
    if (!EXIT(ch, direction))
      send_to_char("There's no exit in that direction...\r\n", ch);
    else if (!IS_SET(EXIT(ch, direction)->exit_info, EX_LOCKED))
      send_to_char("But that door's not locked...\r\n", ch);
    else {  
      success = normal_spell_test(SPELL_OPEN_LOCK, ch, NULL, mana,0);  
      if (success) {
    gen_message(SPELL_OPEN_LOCK,ch,NULL,FALSE,TRUE);
    TOGGLE_BIT(EXIT(ch, direction)->exit_info, EX_LOCKED);
    check_if_learned(ch,SPELL_OPEN_LOCK);
      }
    }
  } else if (NULL != obj) {
    if (!((GET_OBJ_TYPE(obj) == ITEM_CONTAINER)||(GET_OBJ_TYPE(obj) == ITEM_WAGON))) {
      asend_to_char(ch,"Hey! %s %s is not a container!\r\n",AN(obj->short_description),obj->short_description);
      return;
    }
    if (!IS_SET(GET_OBJ_VAL(obj,1),CONT_CLOSED)) {
      send_to_char("But this container is not locked!\r\n",ch);
      return;
    }
    success = normal_spell_test(SPELL_OPEN_LOCK, ch, NULL, mana,0);
    if (success) {
      gen_message(SPELL_OPEN_LOCK,ch,NULL,FALSE,TRUE);
      TOGGLE_BIT(GET_OBJ_VAL(obj,1),CONT_CLOSED);
      check_if_learned(ch,SPELL_OPEN_LOCK);
    }
  } else {
    send_to_char("Fuckup in weave. Please inform an imm!\r\n",ch);
    return;
  }

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.2));
}

BSPELL(identify)
{

}

BSPELL(roomseal)
{
  int mana,success,bonus;
  struct affected_type af;

  if (ROOM_AFFECTED(ch->in_room, RAFF_ROOMSEAL)) {
    send_to_char("This room is already sealed...\r\n", ch);
    return;
  }
  
  bonus = IntBonus(ch);
  mana  = spell_info[SPELL_ROOMSEAL].mana + mana_add;
  success = normal_spell_test(SPELL_ROOMSEAL, ch, NULL, mana, bonus);  
  if (success) {
    af.duration = 5 + mana_add/50;
    af.bitvector= RAFF_ROOMSEAL;
    af.speed    = RAFF_SLOW;
    af.type     = SPELL_ROOMSEAL;
    af.sex      = MadeWith(ch);
    af.next	= NULL;
    af.tie_info = tied;
    af.weaver   = GET_IDNUM(ch);
    af.mana_add = mana_add;

    gen_message(SPELL_ROOMSEAL,ch,NULL,FALSE,TRUE);
    affect_to_room(&world[ch->in_room],&af);
    check_if_learned(ch,SPELL_ROOMSEAL);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.5));
}

BSPELL(sanctuary)
{
  int mana,success,bonus;
  struct affected_type af;

  if (room_affected_by_spell(&world[ch->in_room], SPELL_SANCTUARY)) {
    send_to_char("This room already has been affected by that weave\r\n", ch);
    return;
  }
  
  bonus = IntBonus(ch);
  mana_add = MIN(1000, mana_add); /* No more than 1000 added */
  mana  = spell_info[SPELL_SANCTUARY].mana + mana_add;
  success = normal_spell_test(SPELL_SANCTUARY, ch, NULL, mana, bonus);  
  if (success) {
    af.duration = 5 + mana_add/100;
    af.bitvector= 0;
    af.speed    = RAFF_SLOW;
    af.type     = SPELL_SANCTUARY;
    af.sex      = MadeWith(ch);
    af.next	= NULL;
    af.tie_info = tied;
    af.weaver   = GET_IDNUM(ch);
    af.mana_add = mana_add;

    gen_message(SPELL_SANCTUARY,ch,NULL,FALSE,TRUE);
    affect_to_room(&world[ch->in_room],&af);
    check_if_learned(ch,SPELL_SANCTUARY);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.4));
}

BSPELL(dome_of_fire) /* No 57 */
{
  int mana,success,bonus;
  struct affected_type af;

  if (ROOM_AFFECTED(ch->in_room, RAFF_DOMEOFFIRE)) {
    send_to_char("There is already a dome of Fire in this room...\r\n", ch);
    return;
  }
  
  bonus = IntBonus(ch);
  mana  = spell_info[SPELL_DOMEOFFIRE].mana + mana_add;
  success = normal_spell_test(SPELL_DOMEOFFIRE, ch, NULL, mana, bonus);  
  if (success) {
    af.duration = 5 + mana_add/200;
    af.bitvector= RAFF_DOMEOFFIRE;
    af.speed    = RAFF_SLOW;
    af.type     = SPELL_DOMEOFFIRE;
    af.sex      = MadeWith(ch);
    af.next	= NULL;
    af.tie_info = tied;
    af.weaver   = GET_IDNUM(ch);
    af.mana_add = mana_add;

    gen_message(SPELL_DOMEOFFIRE,ch,NULL,FALSE,TRUE);
    affect_to_room(&world[ch->in_room],&af);
    check_if_learned(ch,SPELL_DOMEOFFIRE);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 1.2));
}

BSPELL(light) /* No 22 */
{
  int mana,success,bonus;
  struct affected_type af;

  if (IS_LIGHT(ch->in_room)) {
    send_to_char("Are you blind? It's already VERY much light in here!\r\n",ch);
    return;
  }
  bonus = IntBonus(ch);
  mana  = spell_info[SPELL_LIGHT].mana;
  success = normal_spell_test(SPELL_LIGHT, ch, NULL, mana, bonus);  
  if (success) {
    af.duration = 10;
    af.bitvector= RAFF_LIGHT;
    af.speed    = RAFF_SLOW;
    af.type     = SPELL_LIGHT;
    af.sex	= MadeWith(ch);
    af.next	= NULL;
    af.tie_info = tied;
    af.weaver   = GET_IDNUM(ch);
    af.mana_add  = mana_add;

    gen_message(SPELL_LIGHT,ch,NULL,FALSE,TRUE);
    if (ROOM_AFFECTED(ch->in_room, RAFF_DARKNESS))
      affect_from_room(&world[ch->in_room],RAFF_DARKNESS);
    affect_to_room(&world[ch->in_room],&af);
    check_if_learned(ch,SPELL_LIGHT);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.5));
}

BSPELL(darkness) /* No 27 */
{
  int mana,success,bonus;
  struct affected_type af;

  if (IS_DARK(ch->in_room)) {
    send_to_char("It's already kinda dark here you know.\r\n",ch);
    return;
  }
  bonus  = IntBonus(ch);
  mana   = spell_info[SPELL_DARKNESS].mana;
  success = normal_spell_test(SPELL_DARKNESS, ch, NULL, mana, bonus);  
  if (success) {
    af.duration = 10;
    af.type     = SPELL_DARKNESS;
    af.bitvector= RAFF_DARKNESS;
    af.speed    = RAFF_SLOW;
    af.sex 	= MadeWith(ch);
    af.next	= NULL;
    af.tie_info = tied;
    af.weaver   = GET_IDNUM(ch);
    af.mana_add = mana_add;

    gen_message(SPELL_DARKNESS,ch,NULL,FALSE,TRUE);
    if (ROOM_AFFECTED(ch->in_room, RAFF_LIGHT))
      affect_from_room(&world[ch->in_room],RAFF_LIGHT);
    affect_to_room(&world[ch->in_room],&af);
    check_if_learned(ch,SPELL_DARKNESS);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.5));
}

BSPELL(explosion) /* No 71 */
{
  int dam,success,bonus,mana;

  bonus = IntBonus(ch);
  dam = (int)((700 + (mana_add/2))*mod);

  if (dam > 9000) 
    mana_add = MAX(0, mana_add - (dam - 9000)/(mod/2)); 
  dam = MIN(dam, 9000);

  mana  = spell_info[SPELL_EXPLOSION].mana + mana_add;
  success  = normal_spell_test(SPELL_EXPLOSION, ch, NULL, mana, bonus);
  if (success) { 
    do_area_ngroup_agg(ch,dam, SPELL_EXPLOSION);
    check_if_learned(ch,SPELL_EXPLOSION);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 1.2));
}

BSPELL(dome_of_frost) /* No 72 */
{
  int mana,success,bonus;
  struct affected_type af;

  if (ROOM_AFFECTED(ch->in_room, RAFF_DOMEOFFROST)) {
    send_to_char("There is already a dome of frost in this room...\r\n", ch);
    return;
  }
  
  bonus = IntBonus(ch);
  mana  = spell_info[SPELL_DOMEOFFROST].mana + mana_add;
  success = normal_spell_test(SPELL_DOMEOFFROST, ch, NULL, mana, bonus);  
  if (success) {
    af.duration = 5 + mana_add/50;
    af.bitvector= RAFF_DOMEOFFROST;
    af.speed    = RAFF_SLOW;
    af.type     = SPELL_DOMEOFFROST;
    af.sex      = MadeWith(ch);
    af.next	= NULL;
    af.tie_info = tied;
    af.weaver   = GET_IDNUM(ch);
    af.mana_add = mana_add;

    gen_message(SPELL_DOMEOFFROST,ch,NULL,FALSE,TRUE);
    affect_to_room(&world[ch->in_room],&af);
    check_if_learned(ch,SPELL_DOMEOFFROST);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 1.2));
}

BSPELL(fist_of_air) /* No 73 */
{
  int success,bonus,mana, dam;


  bonus = IntBonus(ch) + calc_weave_bonus(ch,vict);
  dam = (90 + mana_add/1.5)*mod;

  if (dam > 2500) 
    mana_add = MAX(0, mana_add - (dam - 2500)/(mod/1.5)); 
  dam = MIN(dam, 2500);

  mana  = spell_info[SPELL_FISTOFAIR].mana + mana_add;
  success  = normal_spell_test(SPELL_FISTOFAIR, ch, vict, mana, bonus);
  if (success) {
    GET_TIMES(ch,CAN_WEAVE) =  (PULSE_VIOLENCE); 
    damage(ch, vict, dam, SPELL_FISTOFAIR);
    check_if_learned(ch,SPELL_FISTOFAIR);
  }

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 1.2));
}

BSPELL(firestorm) /* No 74 */
{
  int success,bonus,mana, dam;

  bonus = IntBonus(ch);

  dam = (850 + mana_add)*mod;
  if (dam > 10000) 
    mana_add = MAX(0, mana_add - (dam - 10000)/mod); 
  dam = MIN(dam, 10000);

  mana  = spell_info[SPELL_FIRESTORM].mana + mana_add;
  success  = normal_spell_test(SPELL_FIRESTORM, ch, NULL, mana, bonus);
  if (success) { 
    do_area_ngroup_agg(ch, dam, SPELL_FIRESTORM);
    GET_TIMES(ch,CAN_WEAVE) =  (PULSE_VIOLENCE * 4);
    check_if_learned(ch,SPELL_FIRESTORM);
  }

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 1.3));
}

BSPELL(refresh) /* No 75 */
{
  int ndice,success,bonus,mana;

  bonus = IntBonus(ch);
  mana  = spell_info[SPELL_REFRESH].mana + mana_add;
  ndice = (int)((2 + (mana_add / 20) + (GET_LEVEL(ch) / 5))*mod);

  success = normal_spell_test(SPELL_REFRESH,ch,vict,mana,bonus);

  if (success) {
    GET_TIMES(ch,CAN_WEAVE) =  (PULSE_VIOLENCE);
    gen_message(SPELL_REFRESH,ch,vict,TRUE,TRUE);
    GET_MOVE(vict) += dice(ndice,10);
    GET_FRACT_MOVE(vict) = 0;
    check_if_learned(ch,SPELL_REFRESH);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.5));
}

BSPELL(earth_armor) /* No 76 */
{
  struct affected_type af, af2, af3;
  int mana,bonus,success; 
 
  af = clear_aff_var(af);
  af2 = clear_aff_var(af2);
  af3 = clear_aff_var(af3);
  if (affected_by_spell(vict, SPELL_EARTH_ARMOR)){
    send_to_char("Already affected by Earth Armor.\r\n", ch);
    return;
  }
  
  mana   = (spell_info[SPELL_EARTH_ARMOR].mana + mana_add);
  bonus  = IntBonus(ch);
 
  af.duration   = 10 + mana_add/100;
  af.modifier   = (int)(40*mod);
  af.location   = APPLY_ABS;
  af.next       = NULL;
  af.type       = SPELL_EARTH_ARMOR;
  af.sex        = MadeWith(ch);
  af.tie_info   = tied;
  af.weaver     = GET_IDNUM(ch);

  if (af.modifier < 0) 
    af.modifier = 80;
  if (af.duration < 0)
    af.duration = 50;
  
  af2.duration   = af.duration;
  af2.modifier   = (int)(-5*mod);
  af2.location   = APPLY_AC;
  af2.next       = NULL;
  af2.type       = SPELL_EARTH_ARMOR;
  af2.sex        = MadeWith(ch);
  af2.tie_info   = tied;
  af2.weaver     = GET_IDNUM(ch);

  if (af2.modifier > 0)
    af2.modifier = -100;
  
  af3.duration   = af.duration;
  af3.modifier   = 0;
  af3.location   = 0;
  af3.bitvector2 = AFF2_NODIRTKICK;
  af3.next       = NULL;
  af3.type       = SPELL_EARTH_ARMOR;
  af3.sex        = MadeWith(ch);
  af3.tie_info   = tied;
  af3.weaver     = GET_IDNUM(ch);

  af.mana_add = af2.mana_add = af3.mana_add = mana_add;

  success = normal_spell_test(SPELL_EARTH_ARMOR, ch, NULL, mana, bonus);
  if (success) {
    gen_message(SPELL_EARTH_ARMOR,ch,vict,FALSE,TRUE);
    affect_to_char(vict, &af);
    affect_to_char(vict, &af2);
    affect_to_char(vict, &af3);
    check_if_learned(ch,SPELL_EARTH_ARMOR);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.7));
}

BSPELL(destroy_armor) /* No 77 */
{
  int ndice,success,bonus,mana,dam_mult,pos;

  bonus = IntBonus(ch) + calc_weave_bonus(ch,vict);
  mana  = spell_info[SPELL_DESTROY_ARMOR].mana + mana_add;
  ndice = (int)((2 + (mana_add / 100) + (GET_LEVEL(ch) / 25))*mod);
  
  pos = item_to_destroy(vict,&dam_mult);

  if (pos == -1) {
    send_to_char("Your victim hasn`t GOT any armor to destroy!\r\n",ch);
    return;
  }
  success = normal_spell_test(SPELL_DESTROY_ARMOR,ch,vict,mana,bonus);

  if (success) {
    damage(ch, vict, (int)((dam_mult * dice(ndice, 10))*mod), SPELL_DESTROY_ARMOR);
    extract_obj(unequip_char(vict, pos));
    check_if_learned(ch,SPELL_DESTROY_ARMOR);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 1.8));
}

int item_to_destroy(struct char_data *ch,int *dam_mult)
{
  int test = -1;
  
  if (!GET_EQ(ch,WEAR_LIGHT) && !GET_EQ(ch,WEAR_FINGER_R) && !GET_EQ(ch,WEAR_FINGER_L) && !GET_EQ(ch,WEAR_NECK_1)
      && !GET_EQ(ch,WEAR_NECK_2) && !GET_EQ(ch,WEAR_BODY) && !GET_EQ(ch,WEAR_HEAD) && !GET_EQ(ch,WEAR_LEGS)
      && !GET_EQ(ch,WEAR_FEET) && !GET_EQ(ch,WEAR_HANDS) && !GET_EQ(ch,WEAR_ARMS) && !GET_EQ(ch,WEAR_SHIELD)
      && !GET_EQ(ch,WEAR_ABOUT) && !GET_EQ(ch,WEAR_WAIST) && !GET_EQ(ch,WEAR_WRIST_R) && !GET_EQ(ch,WEAR_WRIST_L))
    return -1;

  do {
    test = number(0,15);
  } while (!GET_EQ(ch,test));

  switch(test) {
    case WEAR_NECK_1:
    case WEAR_NECK_2:
    case WEAR_HEAD:
                    *dam_mult = 3;
                break;
    case WEAR_BODY:
    case WEAR_ABOUT:
    case WEAR_WAIST:
                    *dam_mult = 2;
                    break;
    default:
                    *dam_mult = 1;
                    break;
  }
  return test;
}

BSPELL(dome_of_air)
{
  int mana,success,bonus;
  struct affected_type af;

  if (ROOM_AFFECTED(ch->in_room, RAFF_DOMEOFAIR)) {
    send_to_char("There is already a dome of air in this room...\r\n", ch);
    return;
  }

  bonus = IntBonus(ch);
  mana  = spell_info[SPELL_ROOMSEAL].mana + mana_add;
  success = normal_spell_test(SPELL_DOMEOFAIR, ch, NULL, mana, bonus);
  if (success) {
    af.duration = 5 + mana_add/50;
    af.bitvector= RAFF_DOMEOFAIR;
    af.speed    = RAFF_SLOW;
    af.type     = SPELL_DOMEOFAIR;
    af.sex      = MadeWith(ch);
    af.next     = NULL;
    af.tie_info = tied;
    af.weaver   = GET_IDNUM(ch);
    af.mana_add = mana_add;
  
    gen_message(SPELL_DOMEOFAIR,ch,NULL,FALSE,TRUE);
    affect_to_room(&world[ch->in_room],&af);
    check_if_learned(ch,SPELL_DOMEOFAIR);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.5));
}

BSPELL(fire_arrows) /* No 78 */
{
  int success,bonus,mana, dam;

  bonus = IntBonus(ch) + calc_weave_bonus(ch,vict);
  dam = (100+mana_add)*mod;

  if (dam > 2500) 
    mana_add = MAX(0, mana_add - (dam - 2500)/mod); 
  dam = MIN(dam, 2500);

  mana  = spell_info[SPELL_FIRE_ARROWS].mana + mana_add;
  success  = normal_spell_test(SPELL_FIRE_ARROWS, ch, vict, mana, bonus);

  if (success) {
    GET_TIMES(ch,CAN_WEAVE) =  (PULSE_VIOLENCE * 1); 
    damage(ch, vict, dam, SPELL_FIRE_ARROWS);
    check_if_learned(ch,SPELL_FIRE_ARROWS);
  }

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.3));
}

BSPELL(earth_missile) /* No 79 */
{
  int success,bonus,mana, dam;
  
  bonus   = IntBonus(ch) + calc_weave_bonus(ch,vict);

  dam = (125 + mana_add/2)*mod;
  if (dam > 2500) 
    mana_add = MAX(0, mana_add - (dam - 2500)/(mod/2.0)); 
  dam = MIN(dam, 2500);

  mana    = spell_info[SPELL_EARTH_MISSILE].mana + mana_add;
  success = normal_spell_test(SPELL_EARTH_MISSILE,ch,vict,mana,bonus);
  if (success) {
    GET_TIMES(ch,CAN_WEAVE) =  (PULSE_VIOLENCE * 1);
    damage(ch,vict, dam,SPELL_EARTH_MISSILE);
    check_if_learned(ch,SPELL_EARTH_MISSILE);  
  }

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.4));
}

BSPELL(kinheal)
{
  int amt = 0, success, bonus, mana, i;
  struct char_data *target = NULL;

  if (!ch->player_specials->linking[0]) {
    send_to_char("You have to be linking to use this weave.\r\n", ch);
    return;
  }
  
  for (amt = 0; ch->player_specials->linking[amt]; amt ++);
  amt ++;
  if (amt < 3) {
    send_to_char("You must be at least three in the link for this weave to work.\r\n", ch);
    return;
  }
  bonus = IntBonus(ch) + calc_weave_bonus(ch,vict);
  mana =  (GET_MAX_MANA(ch)/3 * GET_SKILL(ch, SPELL_KINHEAL))/100;
  success = normal_spell_test(SPELL_KINHEAL,ch,NULL,mana,bonus);  
  if (success) {
    for (i = 0; ch->player_specials->linking[i]; i ++) {
      target = ch->player_specials->linking[i];
      GET_HIT(target) = (int)(MIN(GET_MAX_HIT(target), 
      GET_HIT(target) + GET_MAX_HIT(target)*GET_SKILL(ch, SPELL_KINHEAL)/100)*mod);
      update_pos(target);
    }
    check_if_learned(ch,SPELL_KINHEAL);
  }
  else
    gen_message(SPELL_KINHEAL,ch,NULL,FALSE,FALSE);
  
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.5));
}

BSPELL(shield_of_saidin)
{
  struct affected_type af;
  int mana,bonus,success; 
 
  af = clear_aff_var(af);
  if (affected_by_spell(vict, SPELL_SHIELD_OF_SAIDIN)){
    send_to_char("You can't put two Shields of Saidin at the same time on someone.\r\n", ch);
    return;
  }
  
  if (GET_LEVEL(ch) >= LVL_IMMORT) {
    send_to_char("Immortals cannot do this.  Sorry.\r\n", ch);
    return;
  }

  mana   = (spell_info[SPELL_SHIELD_OF_SAIDIN].mana + mana_add);
  bonus  = IntBonus(ch);
 
  af.duration  = 200;
  af.modifier  = 0;
  af.location  = 0;
  af.bitvector = 0;
  af.next      = NULL;
  af.type      = SPELL_SHIELD_OF_SAIDIN;
  af.sex       = MadeWith(ch);
  af.tie_info  = tied;
  af.weaver    = GET_IDNUM(ch);
  af.mana_add  = mana_add;
  af.spell     = (long)(mana*mod);  /* This is a hack, and should be removed when this variable gets used for real */

  if (af.spell < 0)
    af.spell = (0x7FFFFFFF); // Protect against overflow....

  success = normal_spell_test(SPELL_SHIELD_OF_SAIDIN, ch, vict, mana, bonus);
  if (success) {
    gen_message(SPELL_SHIELD_OF_SAIDIN,ch,vict,TRUE,TRUE);
    affect_to_char(vict, &af);
    check_if_learned(ch,SPELL_SHIELD_OF_SAIDIN);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 1.9));
}


BSPELL(stormshield)
{
  struct affected_type af;
  int mana,bonus,success; 
 
  af = clear_aff_var(af);
  if (affected_by_spell(vict, SPELL_STORMSHIELD)){
    send_to_char("You can't put two Stormshields at the same time on someone.\r\n", ch);
    return;
  }

  if (GET_LEVEL(ch) >= LVL_IMMORT) {
    send_to_char("Immortals cannot do this.  Sorry.\r\n", ch);
    return;
  }
  
  mana   = (spell_info[SPELL_STORMSHIELD].mana + mana_add);
  bonus  = IntBonus(ch);
 
  af.duration  = 200;
  af.modifier  = 0;
  af.location  = 0;
  af.bitvector = 0;
  af.next      = NULL;
  af.type      = SPELL_STORMSHIELD;
  af.sex       = MadeWith(ch);
  af.tie_info  = tied;
  af.weaver    = GET_IDNUM(ch);
  af.mana_add  = mana_add;
  af.spell     = (int)(mana*mod);  /* This is a hack, and should be removed when this variable gets used for real */

  if (af.spell < 0)
    af.spell = 0x7FFFFFFF;

  success = normal_spell_test(SPELL_STORMSHIELD, ch, vict, mana, bonus);
  if (success) {
    gen_message(SPELL_STORMSHIELD,ch,vict,TRUE,TRUE);
    affect_to_char(vict, &af);
    check_if_learned(ch,SPELL_STORMSHIELD);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 1.9));
}

BSPELL(watershield)
{
  struct affected_type af;
  int mana,bonus,success; 
 
  af = clear_aff_var(af);
  if (affected_by_spell(vict, SPELL_WATERSHIELD)){
    send_to_char("You can't put two Watershields at the same time on someone.\r\n", ch);
    return;
  }

  if (GET_LEVEL(ch) >= LVL_IMMORT) {
    send_to_char("Immortals cannot do this.  Sorry.\r\n", ch);
    return;
  }
  
  mana   = (spell_info[SPELL_WATERSHIELD].mana + mana_add);
  bonus  = IntBonus(ch);
 
  af.duration  = 200;
  af.modifier  = 0;
  af.location  = 0;
  af.bitvector = 0;
  af.next      = NULL;
  af.type      = SPELL_WATERSHIELD;
  af.sex       = MadeWith(ch);
  af.tie_info  = tied;
  af.weaver    = GET_IDNUM(ch);
  af.mana_add  = mana_add;
  af.spell     = (int)(mana*mod);  /* This is a hack, and should be removed when this variable gets used for real */

  if (af.spell < 0)
    af.spell = 0x7FFFFFFF;

  success = normal_spell_test(SPELL_WATERSHIELD, ch, vict, mana, bonus);
  if (success) {
    gen_message(SPELL_WATERSHIELD,ch,vict,TRUE,TRUE);
    affect_to_char(vict, &af);
    check_if_learned(ch,SPELL_WATERSHIELD);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 1.9));
}

BSPELL(cripple)
{
  int success, mana, modi;
  if (IS_NPC(vict)) {
    send_to_char("No use to cripple a mob, is there?\r\n", ch);
    return;
  }
  if (IS_SET(GET_PERMANENT(vict), PRM_CRIPPLED)) {
    send_to_char("What's the use? That one is already crippled.\r\n", ch);
    return;
  }
  
  modi    = IntBonus(ch) + calc_weave_bonus(ch,vict);
  mana   = spell_info[SPELL_CRIPPLE].mana;
  success = normal_spell_test(SPELL_CRIPPLE, ch, vict, mana, modi);

  if (success) {
    SET_BIT(GET_PERMANENT(vict), PRM_CRIPPLED);
    gen_message(SPELL_CRIPPLE,ch,vict,TRUE,TRUE);
    check_if_learned(ch,SPELL_CRIPPLE);
  }
  else
    gen_message(SPELL_CRIPPLE,ch,vict,TRUE,FALSE);
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.8));
}

BSPELL(deaf)
{
  int success, mana, modi;
  if (IS_NPC(vict)) {
    send_to_char("No use to deafen a mob, is there?\r\n", ch);
    return;
  }
  if (IS_SET(GET_PERMANENT(vict), PRM_DEAF)) {
    send_to_char("What's the use? That one is already deaf.\r\n", ch);
    return;
  }
  
  modi    = IntBonus(ch) + calc_weave_bonus(ch,vict);
  mana   = spell_info[SPELL_DEAF].mana;
  success = normal_spell_test(SPELL_DEAF, ch, vict, mana, modi);

  if (success) {
    SET_BIT(GET_PERMANENT(vict), PRM_DEAF);
    gen_message(SPELL_DEAF,ch,vict,TRUE,TRUE);
    check_if_learned(ch,SPELL_DEAF);
  }
  else
    gen_message(SPELL_DEAF,ch,vict,TRUE,FALSE);

  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.6));
}

BSPELL(compulsion)
{
  struct affected_type af;
  int mana,modi,success;

  af = clear_aff_var(af);
  af.type       = SPELL_COMPULSION;
  af.sex        = MadeWith(ch);
  af.duration   = 10 + (mana_add / 300);
  af.tie_info   = tied;
  af.weaver     = GET_IDNUM(ch);
  af.bitvector2 = AFF2_COMPULSED;

  if (!get_guild(ch,7)) {
    send_to_char("You have not ben Chosen to use this spell!\r\n",ch);
    return;
  }

  if (IS_AFFECTED2(vict,AFF2_COMPULSED)) {
    send_to_char("Your victim is already compulsed!\r\n",ch);
    return;
  }
  if (IS_IMMORTAL(vict)) {
    send_to_char("You can't Compulse \\c01Imms\\c00!\r\n",ch);
    return;
  }

  modi     = IntBonus(ch) + calc_weave_bonus(ch,vict);
  mana    = spell_info[SPELL_COMPULSION].mana + mana_add;
  success = normal_spell_test(SPELL_COMPULSION,ch,vict,mana,modi);

  if (success) {
    affect_to_char(vict,&af);
    gen_message(SPELL_COMPULSION,ch,vict,TRUE,TRUE);
    add_follower(vict,ch);
    check_if_learned(ch,SPELL_COMPULSION);
  }
  else
    gen_message(SPELL_COMPULSION,ch,vict,TRUE,FALSE);
}

BSPELL(sedaibond){
  if (GET_BONDED(vict)){
    send_to_char("Sorry! Already takeeeen! :P\r\n", ch);
    return;
  }    
  
  if (!(vict = get_char_room_vis(ch,GET_NAME(vict)))) {
    sprintf(buf, "Awwww....%s's not even here!\r\n", GET_NAME(vict));
    send_to_char(buf, ch);
    return;
  }

  if (IS_NPC(vict)) {
    send_to_char("Haha! Very funny. NOT! You can`t bond mobs!\r\n",ch);
    return;
  }  
  if (ch == vict) {
    send_to_char("You ought to know better than to try to bond yourself!\r\n", ch);
    return;
  }

  if(normal_spell_test (SPELL_SEDAIBOND, ch, vict, spell_info[SPELL_SEDAIBOND].mana, 0)){
    gen_message(SPELL_SEDAIBOND,ch,vict,TRUE,TRUE);
    if (!GET_BONDED(ch)) { /* New person bonding, increase unique id */
      top_bond_id ++;
      GET_BONDED(ch) = top_bond_id;
      SET_BIT(GET_BONDED(ch), 1 << 30); /* 30th bit set toindicate boss */
    }
    GET_BONDED(vict) = GET_BONDED(ch);   
    REMOVE_BIT(GET_BONDED(vict), 1 << 30);
    alog("%s has bonded %s as her warder.", GET_NAME(ch), GET_NAME(vict)); 
    act("$n has bonded you as $s warder.", 0, ch, 0, vict, TO_VICT);
    act("$N bonded as your warder!", 0, ch, 0, vict, TO_CHAR );
    update_p_table(ch);
    update_p_table(vict);
  }
}

BSPELL(ashabond){
  
  if (GET_ASHABONDED(vict)){
    send_to_char("Sorry! Already takeeeen! :P\r\n", ch);
    return;
  }
    
  if (!(vict = get_char_room_vis(ch, GET_NAME(vict)))) {
    sprintf(buf, "Awwww....%s's not even here!\r\n", GET_NAME(vict));
    send_to_char(buf, ch);
    return;
  }
    
  if (IS_NPC(vict)) {
    send_to_char("Haha! Very funny. NOT! You can`t bond mobs!\r\n",ch);
    return;
  }
  if (ch == vict) {
    send_to_char("You ought to know better than to try to bond yourself!\r\n", ch);
    return;
  }
    
  if(normal_spell_test (SPELL_ASHABOND, ch, vict, spell_info[SPELL_ASHABOND].mana, 0)){
    gen_message(SPELL_ASHABOND,ch,vict,TRUE,TRUE);
    if (!GET_ASHABONDED(ch)) { /* New person bonding, increase unique id */
      top_ashabond_id ++;
      GET_ASHABONDED(ch) = top_ashabond_id;
      SET_BIT(GET_ASHABONDED(ch), 1 << 30); /* 30th bit set to indicate boss */
    }
    GET_ASHABONDED(vict) = GET_ASHABONDED(ch);
    REMOVE_BIT(GET_ASHABONDED(vict), 1 << 30);
    act("$n has bonded you as $s Asha'man warder.", 0, ch, 0, vict, TO_VICT);
    act("$N bonded as your Asha'man warder!", 0, ch, 0, vict, TO_CHAR );
    update_p_table(ch);
    update_p_table(vict);
    GET_TAINT(ch) += add_taint(ch, 1200);
  }
}

BSPELL(wrath) /* No 101 */
{
  int bonus,result,mana, dam;
  dam = (2500 + mana_add/2)*mod;

  if ((dam > 15000) && (mana_add > 0))
    mana_add = MAX(0, mana_add - (dam - 15000)/(mod/2)); 
  dam = MIN(dam, 15000);

  mana   = spell_info[SPELL_WRATH].mana + mana_add;
  bonus  = IntBonus(ch);
/*  result  = normal_spell_test(SPELL_WRATH, ch, NULL, mana, bonus); */

  result  = generic_result_mod(ch,SPELL_WRATH,NULL,FALSE,bonus);
  
  GET_TAINT(ch) += add_taint(ch, (2 * mana));

  switch (result) {
    case TOTAL_FUCKUP:
            mana *= 2;
            dam /= 2;
            GET_MANA(ch) -= mana;
                GET_TIMES(ch,CAN_WEAVE) =  (7*PULSE_VIOLENCE);
                damage(ch,ch,dam,SPELL_WRATH);
            return;
        break;
    case TOTAL_FAILURE:
                GET_TIMES(ch,CAN_WEAVE) =  (6*PULSE_VIOLENCE);
        mana /= 2;
        GET_MANA(ch) -= mana;
        gen_message(SPELL_WRATH,ch,NULL,FALSE,FALSE);
        return;
        break;
    case SOME_SUCCESS:
                GET_TIMES(ch,CAN_WEAVE) =  (5*PULSE_VIOLENCE);
        break;
    case TOTAL_SUCCESS:
                GET_TIMES(ch,CAN_WEAVE) =  (4*PULSE_VIOLENCE);
        mana /= 2;
        dam = (int) (1.33 * dam);
        break;
  }
  GET_MANA(ch) -= mana;

  if (dam < 0 || dam > 20000)
    dam = 20000;

  check_if_learned(ch,SPELL_WRATH);
  do_area_ngroup_agg(ch,dam,SPELL_WRATH);

}

int IntBonus(struct char_data *ch)
{
  if (IS_NPC(ch))
    return 0;
  return ((GET_INT(ch) - 15) / 2);
}

int calc_weave_bonus(struct char_data *ch,struct char_data *vict)
{
  if (IS_NPC(ch))
    return 0;

  if ((NULL == ch)||(NULL == vict)) {
    mlog("SYSERR: NULL arguemnts in call to calc_weave_bonus(), defaulting to 0 in bonus.");
    return 0;
  }

  if (IS_SET(PRF_FLAGS(ch),PRF_GRASPING)) {
    if (can_channel_same_sex(ch,vict)) 
       return (-(GET_AC(ch) / 2));
    else if (can_channel_not_same_sex(ch,vict))
       return (-(GET_AC(ch) / 3));
    else
      return (-(GET_AC(ch) / 5));
  }
  else if (IS_SET(PRF_FLAGS(ch),PRF_TGRASP)) {
    if (GET_SKILL(vict,SKILL_TGRASP))
      return (-(GET_AC(ch) / 2));
    else
      return (-(GET_AC(ch) / 5));
  }
  return 0;
}

int normal_spell_test(int type, struct char_data *ch, struct char_data *victim,
                      int mana, int bonus)
{
  int target = FALSE;
  struct affected_type af;
  int result = generic_result_mod(ch, type, NULL, FALSE, bonus);
  if (victim) 
    target = TRUE;
  else  
    target = FALSE;
  if (mana < 0) 
    alog("Error: Mana cost negative when %s is casting spell '%s'", GET_NAME(ch), spells[type]);
  switch (result) {
    case TOTAL_FUCKUP:
    case TOTAL_FAILURE:
        GET_MANA(ch) -= mana;
        gen_message(type, ch, victim, target, FALSE);
        return 0;
        break;
    case SOME_SUCCESS:
        break;
    case TOTAL_SUCCESS:
                if ((victim) && (GET_LEVEL(ch) < LVL_IMMORT))
                  if (!IS_NPC(victim) && IS_SET(spell_info[type].targets, IS_AGGRESSIVE)) {
                    af.duration = 10;
                    af.modifier = 0;
                    af.location = APPLY_NONE;
                    af.bitvector = AFF_NOQUIT;
                    af.bitvector2 = af.bitvector3 = 0;
                    af.next = NULL;
                    af.type = NOQUIT_TEXT;
                    affect_to_char(ch, &af);
                  }
        mana /= 2;
        break;
  }
  GET_MANA(ch) -=mana;
  return 1;
}

int gen_message(int type,struct char_data *ch,struct char_data *vict,int target,int success)
{
  if (IS_SET(PRF_FLAGS(ch),PRF_GRASPING))
    spell_message(type,ch,vict,target,success);
  else
    tp_message(type,ch,vict,target,success);

  return 1;
}
