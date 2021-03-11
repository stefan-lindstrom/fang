/******************************************************************************
 * $Id: fight.c,v 1.22 2004/02/25 00:37:22 fang Exp $
 *   
 * File: fight.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: fight.c,v $
 *   Revision 1.22  2004/02/25 00:37:22  fang
 *   Removed the bit of code that gave Rebel Warders their own private death room
 *
 *   Revision 1.21  2003/10/03 05:40:55  fang
 *   Removed useless syslog message involving flee
 *
 *   Revision 1.20  2003/06/20 07:49:55  fang
 *
 *   db.c now loads xml-messages instead of old message files. Old code for message
 *   loading have been removed.
 *
 *   Revision 1.19  2003/06/17 13:02:37  fang
 *
 *   Added a single file for messages. Code for now contains only functionalty to
 *   save messages (fight/spell op/spell tp) as XML. All structs, and data, and
 *   defines pertaining to messages have been moved to the new header.
 *
 *   Revision 1.18  2003/06/10 08:08:46  fang
 *   Fixed the fix to Adrenalin. *oops*
 *
 *   Revision 1.17  2003/06/10 06:05:55  fang
 *   Fixed problem with adrenalin, spearmastery and blademastery being able to go above 100
 *
 *   Revision 1.16  2003/05/29 03:50:54  fang
 *   Can wimpy after bashing now.
 *
 *   Revision 1.15  2003/03/05 08:23:26  fang
 *   Removed annyoing Syserr
 *
 *   Revision 1.14  2002/11/07 04:02:02  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.13  2002/10/29 03:09:26  fang
 *   fisch: added the command "scene".  Scenes are like doings for rooms.
 *
 *   Revision 1.12  2002/10/08 21:12:31  fang
 *   fisch: included blockage of "BLEEDING" message in NOSPAM toggle
 *
 *   Revision 1.11  2002/09/22 10:50:07  fang
 *   Made shield reduction work for triggered %damage% calls as well.
 *
 *   Revision 1.10  2002/09/22 08:11:16  fang
 *   Put in a delay between backstab and flee, a halving of woven shields
 *   efficiency and a full !Magic protection of mobs.
 *
 *   Revision 1.9  2002/09/15 00:50:08  fang
 *   Drath: XLOG update - Categories added to xlog display automatically
 *
 *   Revision 1.8  2002/09/13 19:05:08  fang
 *   fischer: code cleanup.  Removed some variables that were not used anymore
 *
 *   Revision 1.7  2002/09/13 04:54:09  fang
 *   Drath: Fixed ravage check and extended bargain and charm to 2 hours
 *
 *   Revision 1.6  2002/09/12 23:00:02  fang
 *   fischer: revamp the consider code to make it more relevent
 *
 *   Revision 1.5  2002/08/30 21:23:09  fang
 *   Put in retainers.
 *
 *   Revision 1.4  2002/07/28 13:56:04  fang
 *   Removed dmlloc...just to test.
 *
 *   Revision 1.3  2002/07/26 19:39:52  fang
 *   Dath: MYCOLOR mem leak fixed.  Thanks, Fred!
 *
 *   Revision 1.2  2002/07/22 15:08:04  fang
 *   Drath: Typo fixes.
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.7  2002/07/06 07:05:12  fang
 *   Drath: IMMLOCK for Fred and 2nd to last XLOG commit
 *
 *   Revision 1.6  2002/05/27 10:52:49  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.5  2002/05/23 22:50:17  fang
 *   Maz: Newbie death message and other stuff...
 *
 *   Revision 1.4  2002/05/21 13:30:50  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/02/28 17:15:55  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.33  2002/02/20 03:12:28  mud
 *   *** empty log message ***
 *
 *   Revision 1.32  2002/02/19 14:01:06  mud
 *   act\(\) fixes and stuff
 *
 *   Revision 1.31  2002/02/17 21:23:39  mud
 *   *** empty log message ***
 *
 *   Revision 1.30  2002/02/17 08:29:50  mud
 *   *** empty log message ***
 *
 *   Revision 1.29  2002/02/16 10:36:46  mud
 *   *** empty log message ***
 *
 *   Revision 1.28  2002/02/14 14:17:10  mud
 *   *** empty log message ***
 *
 *   Revision 1.27  2002/01/24 21:33:15  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.26.2.1  2002/01/22 03:05:14  mud
 *   More cleanup. Removed extern decls from source files. Protected some DG headers from multiple inclision
 *
 *   Revision 1.26  2002/01/14 19:28:01  mud
 *
 *
 *   Autoloot now checks that character can carr item before transfing the
 *   item to its inventory. If carrying is not possible, the item will
 *   fall to the ground in the room. Blind people can autojunk (i.e after
 *   gettinf dirtkicked).
 *
 *   Revision 1.25  2002/01/13 21:28:26  mud
 *   Default desc changes name, when character changes name...
 *
 *   Revision 1.24  2002/01/13 19:59:48  mud
 *   \!ATTACK in cart messages fixed. I.e, TO_NOTVICT instead of TO_ROOM in the general, but not to the attacked msg
 *
 *   Revision 1.23  2001/11/25 11:55:46  mud
 *   Fixed little bug when killer == NULL in raw_kill.
 *
 *   Revision 1.22  2001/08/22 21:09:31  mud
 *   *** empty log message ***
 *
 *   Revision 1.21  2001/08/21 15:46:50  mud
 *   *** empty log message ***
 *
 *   Revision 1.20  2001/08/14 12:52:34  mud
 *   Changed exp cap to 1000.
 *
 *   Revision 1.19  2001/06/11 14:53:04  mud
 *   *** empty log message ***
 *
 *   Revision 1.18  2001/05/22 09:07:04  mud
 *   DG-Scripts for Fang.
 *
 *   Revision 1.17  2001/05/10 11:33:00  mud
 *   *** empty log message ***
 *
 *   Revision 1.16  2001/01/31 12:27:18  mud
 *   Changed the exp loss when dying to 0 for level <= 20 (newbies) and 1000
 *   for level > 20
 *
 *   Revision 1.15  2000/12/11 12:14:18  mud
 *   Removed most references to PFILEPOS() get_char_ppos(), since with the xml code ppos will(may) change every reboot
 *
 *   Revision 1.14  2000/12/10 11:49:16  mud
 *   *** empty log message ***
 *
 *   Revision 1.13  2000/12/01 23:17:06  mud
 *   converted GET_GUILD()s to get_guild()s
 *
 *   Revision 1.11  2000/11/27 02:17:07  mud
 *   *** empty log message ***
 *
 *   Revision 1.10  2000/11/15 22:52:05  mud
 *   *** empty log message ***
 *
 *   Revision 1.9  2000/11/15 21:50:42  mud
 *   *** empty log message ***
 *
 *   Revision 1.8  2000/09/04 03:00:26  mud
 *   Warnings removed...
 *
 *   Revision 1.7  2000/08/14 19:11:56  mud
 *   The new guild system.
 *
 *   Revision 1.6  2000/05/24 08:41:13  mud
 *   Aiel endurance changes
 *
 *   Revision 1.5  2000/04/29 16:09:01  mud
 *   Fixed multiple subdue stuff.
 *
 *   Revision 1.4  2000/04/25 12:55:22  mud
 *   Reports correct gained exp now.
 *
 *   Revision 1.3  2000/04/02 15:52:10  mud
 *   Fixed the bug that made people attacked when carting still ride the cart.
 *   Now the rider stops where attacked.
 *
 *   Revision 1.2  2000/02/21 14:00:17  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/**************************************************************************
File: fight.c Part of CircleMUD * * Usage: Combat system * * * * All
rights reserved.  See license.doc for complete information.  * * * *
Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University * *
CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.  *
************************************************************************
*/
#include "config.h"
#include "class.h"
#include "shop.h"
#include "conf.h"
#include "sysdep.h"
#include "magic.h"
#include "fight.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "handler.h"
#include "interpreter.h"
#include "db.h"
#include "spells.h"
#include "screen.h"
#include "act.h"
#include "mobprog.h"
#include "mobact.h"
#include "guild.h"
#include "guild_parser.h"
#include "dg_scripts.h"
#include "retainer.h"
#include "messages.h"

/* Structures */
struct char_data *combat_list = NULL;	/* head of l-list of fighting chars */
struct char_data *next_combat_list = NULL;

char debug[255];
/* Weapon attack texts */
struct attack_hit_type attack_hit_text[] =
{
  {"hit", "hits"},		/* 0 */
  {"sting", "stings"},
  {"whip", "whips"},
  {"slash", "slashes"},
  {"bite", "bites"},
  {"bludgeon", "bludgeons"},	/* 5 */
  {"crush", "crushes"},
  {"pound", "pounds"},
  {"claw", "claws"},
  {"maul", "mauls"},
  {"thrash", "thrashes"},	/* 10 */
  {"pierce", "pierces"},
  {"blast", "blasts"},
  {"punch", "punches"},
  {"stab", "stabs"},
  {"whack", "whacks"}
};

#define IS_WEAPON(type) (((type) >= TYPE_HIT) && ((type) < TYPE_SUFFERING))

/* The Fight related routines */
int nospam_set(struct char_data *ch)
{
  if (!IS_NPC(ch) && IS_SET(PRF_FLAGS(ch), PRF_NOSPAM))
    return 1;
  return 0;
}

void appear(struct char_data * ch)
{
  if (affected_by_spell(ch, SPELL_INVISIBILITY))
    affect_from_char(ch, SPELL_INVISIBILITY);
  
  REMOVE_BIT(AFF_FLAGS(ch), AFF_INVISIBLE);
  REMOVE_BIT(AFF_FLAGS(ch), AFF_HIDE);
  REMOVE_BIT(AFF_FLAGS(ch), AFF_CAMOUFLAGED);

  if (GET_LEVEL(ch) < LVL_IMMORT)
    act("$n slowly fades into existence.", FALSE, ch, 0, 0, TO_ROOM);
  else
    act("You feel a strange presence as $n appears, seemingly from nowhere.",
    FALSE, ch, 0, 0, TO_ROOM);
}

void update_pos(struct char_data * victim)
{

  if ((GET_HIT(victim) > 0) && (GET_POS(victim) > POS_STUNNED))
    return;
  else if (GET_HIT(victim) > 0) 
    GET_POS(victim) = POS_STANDING;
  else if (GET_HIT(victim) <= -11) 
    GET_POS(victim) = POS_DEAD;
  else if (GET_HIT(victim) <= -6)
    GET_POS(victim) = POS_MORTALLYW;
  else if (GET_HIT(victim) <= -3)
    GET_POS(victim) = POS_INCAP;
  else
    GET_POS(victim) = POS_STUNNED;
}

void perform_autoassist(struct char_data *ch)
{
  struct char_data    *k;
  struct follow_type  *f;

  k = (ch->master ? ch->master : ch);

  for( f = k->followers; f != NULL; f = f->next) {

    if (AUT_FLAGGED(f->follower,AUTO_ASSIST) && (f->follower != ch) && (!FIGHTING(f->follower)) && 
        (ch->in_room == f->follower->in_room) && IS_AFFECTED(f->follower,AFF_GROUP)) {

      set_fighting(f->follower, FIGHTING(ch));
      send_to_char("You join the fighting!\r\n",f->follower);
      act("$N assists $n!",FALSE,ch,0,f->follower,TO_NOTCHAR);
      act("$N assists you!",FALSE,ch,0,f->follower,TO_CHAR);
    }
  }

  if ((!FIGHTING(k)) && (k != ch) && AUT_FLAGGED(k,AUTO_ASSIST) && 
(FIGHTING(ch) != k) && (k->in_room == ch->in_room)) {
    set_fighting(k, FIGHTING(ch));
    send_to_char("You join the fighting!\r\n",k);
    act("$N assists $n!",FALSE,ch,0,k,TO_NOTCHAR);
    act("$N assists you!",FALSE,ch,0,k,TO_CHAR);
  }
}


/* start one char fighting another (yes, it is horrible, I know... )  */
void set_fighting(struct char_data * ch, struct char_data * vict)
{
  struct affected_type af;

  if (!ch)
    return;
  if (!vict)
    return;
  if (ch == vict)
    return;
  if (FIGHTING(ch)) 
    return;

  ch->next_fighting = combat_list;
  combat_list = ch;

  if (IS_AFFECTED(ch, AFF_SLEEP))
    affect_from_char(ch, SPELL_SLEEP);

  FIGHTING(ch) = vict;
  GET_POS(ch) = POS_FIGHTING;

  if (!IS_NPC(ch) && !IS_NPC(vict)) {
    af.duration = 10;
    af.modifier = 0;
    af.location = APPLY_NONE;
    af.bitvector = AFF_NOQUIT;
    af.bitvector2 = 0;
    af.bitvector3 = 0;
    af.next = NULL;
    af.type = NOQUIT_TEXT;
    affect_to_char(ch,&af);
    affect_to_char(vict,&af);
  }
}

/* remove a char from the list of fighting chars */
void stop_fighting(struct char_data * ch)
{
  struct char_data *temp;

  if (ch == next_combat_list)
    next_combat_list = ch->next_fighting;

  REMOVE_FROM_LIST(ch, combat_list, next_fighting);
  ch->next_fighting = NULL;
  FIGHTING(ch) = NULL;
  GET_POS(ch) = POS_STANDING;
  update_pos(ch);
}



struct obj_data *make_corpse(struct char_data * ch)
{
  struct obj_data *corpse, *o;
  struct obj_data *money;
  int i;

  corpse = create_obj();

  if (is_dreamer(ch))
    SET_BIT(OBJ_EXTRA_FLAGS(corpse),ITEM_TAR);

  corpse->item_number = NOTHING;
  corpse->in_room = NOWHERE;
  corpse->name = str_dup("corpse");

  sprintf(buf2, "The corpse of %s is lying here.", GET_NAME(ch));
  corpse->description = str_dup(buf2);

  sprintf(buf2, "the corpse of %s", GET_NAME(ch));
  corpse->short_description = str_dup(buf2);
  if (IS_NPC(ch) && IS_AFFECTED(ch, AFF_DREAMING)) /* Mob dreamer -> corpse dreamitem */
    SET_BIT(GET_OBJ_EXTRA(corpse), ITEM_TAR);
  GET_OBJ_TYPE(corpse) = ITEM_CONTAINER;
  GET_OBJ_WEAR(corpse) = ITEM_WEAR_TAKE;
  GET_OBJ_VAL(corpse, 0) = 0;	/* You can't store stuff in a corpse */
  GET_OBJ_VAL(corpse, 3) = 1;	/* corpse identifier */
  GET_OBJ_WEIGHT(corpse) = 0;   /* 0 corpse weight, makes sure we always */
                                /* can get and junk a corpse. 		 */ 
  if (IS_NPC(ch))
    GET_OBJ_TIMER(corpse) = max_npc_corpse_time;
  else
    GET_OBJ_TIMER(corpse) = max_pc_corpse_time;

  /* transfer character's inventory to the corpse */
  corpse->contains = ch->carrying;
  for (o = corpse->contains; o != NULL; o = o->next_content)
    o->in_obj = corpse;
  object_list_new_owner(corpse, NULL);

  /* transfer character's equipment to the corpse */
  for (i = 0; i < NUM_WEARS; i++)
    if (GET_EQ(ch, i)) {
      remove_otrigger(GET_EQ(ch, i), ch);
      obj_to_obj(unequip_char(ch, i), corpse);
    }

  /* transfer gold */
  if (GET_GOLD(ch) > 0) {
    /* following 'if' clause added to fix gold duplication loophole */
    if (IS_NPC(ch) || (!IS_NPC(ch) && ch->desc)) {
      money = create_money(GET_GOLD(ch));
      obj_to_obj(money, corpse);
    }
    GET_GOLD(ch) = 0;
  }
  ch->carrying = NULL;
  IS_CARRYING_N(ch) = 0;
  IS_CARRYING_W(ch) = 0;

  obj_to_room(corpse, ch->in_room);
  return corpse;
}

void death_cry(struct char_data * ch)
{
  int door, was_in;
  struct char_data *list;

  for (list = world[ch->in_room].people; list; list = list->next_in_room) {
    if (!PRF2_FLAGGED(list, PRF2_NOKILLMESS)) { 
      act("Your blood freezes as you hear $n's death cry.", FALSE, ch, 0, list, TO_VICT);
    }
  }
  was_in = ch->in_room;

  for (door = 0; door < NUM_OF_DIRS; door++) {
    if (CAN_GO(ch, door)) {
      ch->in_room = world[was_in].dir_option[door]->to_room;
      for (list = world[ch->in_room].people; list; list = list->next_in_room) {
        if (!PRF2_FLAGGED(list,PRF2_NOKILLMESS)) {
          act("Your blood freezes as you hear someone's death cry.", FALSE, ch, 0, list, TO_VICT);
        }
      }
      list = NULL;
      ch->in_room = was_in;
    }
  }
  ch->in_room = was_in;
}

void perform_split(struct char_data *ch,int amount)
{
  struct follow_type *f;
  struct char_data   *k;
  int members = 1,gold;
  char tmp[200];

  k = (ch->master ? ch->master : ch);

  for (f = k->followers; f != NULL; f = f->next) {
    if (!IS_AFFECTED(f->follower,AFF_GROUP) || (ch->in_room != f->follower->in_room))
      continue;
    members++;
  }
  gold = amount / members;

  sprintf(tmp,"%s splits %d coins with the group\r\n"
              "You receive your share of %d coins.\r\n",GET_NAME(ch),amount,gold);

  for (f = k->followers; f != NULL; f = f->next) {
    if (!IS_AFFECTED(f->follower,AFF_GROUP) || (ch->in_room != f->follower->in_room))
      continue;
    if (f->follower != ch) {
      GET_GOLD(f->follower) += gold;
      send_to_char(tmp,f->follower);
    }
  }
  if (k != ch) {
    send_to_char(tmp,k);
    GET_GOLD(k) += gold;
  }
    
  GET_GOLD(ch) += gold;
  sprintf(tmp,"You split %d coins with your group, you get %d coins.\r\n",amount,gold);
  send_to_char(tmp,ch);
}

void raw_kill(struct char_data * ch, struct char_data *killer)
{
/*  #define RWARD_DEATH 3571 */

  char tmp[300];
  struct affected_type *aff,*next;
  struct obj_data *obj;
  int i, dreamer = 0, room = -1;
/*  struct char_guild_element *el; */

  if (FIGHTING(ch)) 
    stop_fighting(ch);
  
  if (IS_NPC(ch) && IS_SET(AFF_FLAGS(ch), AFF_DREAMING)) /* Dreaming flag must remain on mobs */
    dreamer = TRUE;
  
  for (aff = ch->affected; aff != NULL; aff = next) {
    next = aff->next;
    if (!IS_SET(aff->bitvector,AFF_NOQUIT))
      affect_remove(ch, aff);
  }

  if(!(killer && GET_SUBDUE(killer) && !IS_NPC(ch)))
  {
   if (killer) {
    if (death_mtrigger(ch, killer))
     death_cry(ch);
   } else 
      death_cry(ch);
  }
     
  if (!IS_NPC(ch)) 
  {
    if (IS_AFFECTED2(ch,AFF2_DEFENSE)) {
      remove_defense(ch);
    }
    if (IS_SET(PRF_FLAGS(ch),PRF_GRASPING)) {
      REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
      remove_char_maintained(ch);
      remove_room_maintained(ch);
    }
    if (killer && !IS_NPC(killer) && IS_AFFECTED2(killer,AFF2_DEFENSE)) {
      remove_defense(killer);
    }
#if 0
    if ((world[ch->in_room].number/1000 == 9) && killer && !GET_SUBDUE(killer) )
       room = 9614;
    else
#endif
    if (killer && !GET_SUBDUE(killer))
       room = death_room;

#if 0	/* No more warder death room. */
    if (real_room(RWARD_DEATH) != NOWHERE) 
      for(el = GET_CHAR_GUILDS(ch);el;el = el->next) 
        if ((el->guild->id == 5) && STATUS_FLAGGED(el->guildie, STATUS_WARDER)) {
          room = RWARD_DEATH;
          break;
        }
#endif

    if(killer && !GET_SUBDUE(killer) && !IS_NPC(ch))
    {
      char_from_room(ch);
      char_to_room(ch, real_room(room));
      GET_POS(ch) = POS_SLEEPING;
    }
    GET_HIT(ch) = 1;
    if (!cant_channel(ch)) 
    {
      send_to_char("You release any weaves you may be maintaining\r\n",ch);
      remove_char_maintained(ch);
      remove_room_maintained(ch);
    }

  }
  else 
  {  
    if (is_retainer(ch) && get_retainer_owner(ch))
    {
      send_death_message_to_retainer_owner(ch);
      remove_and_free_retainer(get_retainer_owner(ch), ch->mob_specials.retainer);
    }
    if (killer && AUT_FLAGGED(killer,AUTO_GOLD)) {
      if (AUT_FLAGGED(killer,AUTO_SPLIT) && IS_AFFECTED(killer,AFF_GROUP)) {
        GET_GOLD(killer) += GET_GOLD(ch);
        sprintf(tmp,"split %ld\n",GET_GOLD(ch));
    command_interpreter(killer,tmp);
      }
      else {
        GET_GOLD(killer) += GET_GOLD(ch);
        sprintf(tmp,"You get %ld coins from %ss corpse.\r\n",GET_GOLD(ch),GET_NAME(ch));
        send_to_char(tmp,killer); 
      }
      GET_GOLD(ch)  = 0;
    }  
    if (dreamer)
      SET_BIT(AFF_FLAGS(ch), AFF_DREAMING);
      
    if ((NULL != killer) && AUT_FLAGGED(killer,AUTO_LOOT)) {

      while (ch->carrying) {
    obj = ch->carrying;
        if (obj->carried_by == ch) {
      if ( (IS_CARRYING_N(killer) >= CAN_CARRY_N(killer)) || 
        (IS_CARRYING_W(killer) + GET_OBJ_WEIGHT(obj) > CAN_CARRY_W(killer)) ) {
        asend_to_char(killer,"You try to get %s from the corpse of %s, but can't hold "
                    "onto it. It falls to the ground.\r\n",
              obj->short_description,GET_NAME(ch));
        
        act("$n tries to get $a $o from the corpse of $N but $e can't hold on to it, " 
        "and drops it to the ground!",FALSE,killer,obj,ch,TO_ROOM);
        obj_from_char(obj);
        obj_to_room(obj,killer->in_room);
      } else {
        sprintf(tmp,"You get %s from the corpse of %s.\r\n",obj->short_description,GET_NAME(ch));
        send_to_char(tmp,killer);
        sprintf(tmp,"$n gets %s from the corpse of $N.",obj->short_description);
        act(tmp,FALSE,killer,0,ch,TO_ROOM);
        obj_from_char(obj);
        obj_to_char(obj,killer);
      }
        }
      }

      for (i = 0; i < NUM_WEARS; i++) {
        if (GET_EQ(ch,i)) {
          obj = unequip_char(ch,i);
      if ( (IS_CARRYING_N(killer) >= CAN_CARRY_N(killer)) || 
        (IS_CARRYING_W(killer) + GET_OBJ_WEIGHT(obj) > CAN_CARRY_W(killer)) ) {
        asend_to_char(killer,"You try to get %s from the corpse of %s, but can't hold "
                    "onto it. It falls to the ground.\r\n",
              obj->short_description,GET_NAME(ch));
        
        act("$n tries to get $a $o from the corpse of $N but $e can't hold on to it, " 
        "and drops it to the ground!",FALSE,killer,obj,ch,TO_ROOM);

        obj_to_room(obj,killer->in_room);
      } else {
        sprintf(tmp,"You get %s from the corpse of %s.\r\n",obj->short_description,GET_NAME(ch));
        obj_to_char(obj,killer);
        send_to_char(tmp,killer);
        sprintf(tmp,"$n gets %s from the corpse of $N.",obj->short_description);
        act(tmp,FALSE,killer,0,ch,TO_ROOM);
      }
        }
      }
    }
    
    // Now even blind ppl can get and junk corpses! :)
    if ((NULL != killer) && AUT_FLAGGED(killer,AUTO_JUNK)) {
      act("$n picks up the corpse of $N and junk it. It vanishes in a puff of smoke.",
      FALSE,killer,0,ch,TO_ROOM);
      act("You pick up the corpse of $N and junk it. It vanishes in a puff of smoke.",
      FALSE,killer,0,ch,TO_CHAR);
    } else {
      obj = make_corpse(ch);
    }
    extract_char(ch, FALSE);
  }
}



void die(struct char_data * ch, struct char_data *killer)
{
  // Following if clause is to stop people repeat-kill their victims
  // Added 990929 by Fredrik
  if (FIGHTING(ch))  
  {
    if (FIGHTING(FIGHTING(ch)) == killer)
    {
      stop_fighting(killer);
    }
    stop_fighting(ch);
  }

  if(IS_NPC(ch) || (!GET_SUBDUE(killer) && !IS_NPC(ch)) ) 
  {
    if (IS_NPC(killer)) 
    {
      int exp;
      exp = IS_NEWBIE(ch) ? 0 : 1000;
      /* gain_exp(ch, -GET_LEVEL_EXP(ch),ch); // originally */
      gain_exp(ch, -exp, ch);
    }
    raw_kill(ch, killer);
  }
}



void perform_group_gain(struct char_data * ch, int base,
                struct char_data * victim)
{
  int share = base,texp;

  if (!IS_NPC(victim) && !IS_NPC(ch))
    return;

  if (base > 1000) share = 1000;
  if (base <= 0)   share = 1;

  texp = calc_exp(ch,share,victim);

  if (texp > 1) {
    sprintf(buf2, "You receive your share of experience -- %d points.\r\n", texp);
    send_to_char(buf2, ch);
  } else
    send_to_char("You receive your share of experience -- one measly little point!\r\n", ch);

  gain_exp(ch, share,victim);
}


void group_gain(struct char_data * ch, struct char_data * victim)
{
  int high_level = 0, base;
  float temp;
  struct char_data *k;
  struct follow_type *f;
  float divisor = 1.0;
  
  if (!(k = ch->master))
    k = ch;

  high_level = GET_LEVEL(k);
  
  if(PLR_FLAGGED(k, PLR_NOGAIN))
    return;

  for (f = k->followers; f; f = f->next)
    if (IS_AFFECTED(f->follower, AFF_GROUP) && f->follower->in_room == ch->in_room) {
      if (high_level < GET_LEVEL(f->follower))
        high_level =  GET_LEVEL(f->follower);
     /* Divide by 1.5 recursively for every follower */
      divisor *= 1.5;
    if(PLR_FLAGGED(f->follower, PLR_NOGAIN))
      return;
    }
  
  if ((high_level - GET_LEVEL(victim)) > 40)
    base = 0;
  else if ((GET_LEVEL(victim) - high_level) > 32)
    base = 1000;
  else {
    temp = ((high_level-GET_LEVEL(victim))/10.0) + 5.0;
    base = (int) (25.0*(GET_EXP(victim)+100.0)/(temp*temp) - 100.0);
  }
  base = base/divisor;

  if (IS_AFFECTED(k, AFF_GROUP) && (k->in_room == ch->in_room))
    perform_group_gain(k, base, victim);

  for (f = k->followers; f; f = f->next)
    if (IS_AFFECTED(f->follower, AFF_GROUP) && (f->follower->in_room == ch->in_room))
      perform_group_gain(f->follower, base, victim);
}

char *replace_string(char *str, char *weapon_singular, char *weapon_plural)
{
  static char buf[256];
  char *cp;

  cp = buf;

  for (; *str; str++) {
    if (*str == '#') {
      switch (*(++str)) {
      case 'W':
    for (; *weapon_plural; *(cp++) = *(weapon_plural++));
    break;
      case 'w':
    for (; *weapon_singular; *(cp++) = *(weapon_singular++));
    break;
      default:
    *(cp++) = '#';
    break;
      }
    } else
      *(cp++) = *str;

    *cp = 0;
  }				/* For */

  return (buf);
}


/* message for doing damage with a weapon */
void dam_message(int dam, struct char_data * ch, struct char_data * victim,
            int w_type)
{
  char *buf;
  int msgnum;
  char tmp[500], cmisshit[20], vmisshit[20];
  char *mess;
 
  static struct dam_weapon_type {
    char *to_room;
    char *to_char;
    char *to_victim;
  } dam_weapons[] = {

    /* use #w for singular (i.e. "slash") and #W for plural (i.e. "slashes") */

    {
      "$n performs %s but misses $N.",	/* 0: 0     */
      "You perform %s but miss $N.",
      "$n performs %s but misses you."
    },

    {
      "$n performs %s that tickles $N as $e #W $M.",	/* 1: 1..2  */
      "You perform %s that tickles $N as you #w $M.",
      "$n performs %s that tickles you as $e #W you."
    },

    {
      "$n performs %s that barely #W $N.",		/* 2: 3..4  */
      "You perform %s that barely #W $N.",
      "$n performs %s that barely #W you."
    },

    {
      "$n performs %s that #W $N.",			/* 3: 5..6  */
      "You perform %s that #W $N.",
      "$n perform %s that #W you."
    },

    {
      "$n performs %s that #W $N hard.",			/* 4: 7..10  */
      "You perform %s that #W $N hard.",
      "$n performs %s that #W you hard."
    },

    {
      "$n performs %s that #W $N very hard.",		/* 5: 11..14  */
      "You perform %s that #W $N very hard.",
      "$n performs %s that #W you very hard."
    },

    {
      "$n performs %s that #W $N extremely hard.",	/* 6: 15..19  */
      "You perform %s that #W $N extremely hard.",
      "$n performs %s that #W you extremely hard."
    },

    {
      "$n massacres $N to small fragments with $s excellent performance of %s.",	/* 7: 19..23 */
      "You massacre $N to small fragments with your excellent performance of %s.",
      "$n massacres you to small fragments with $s excellent performance of %s."
    },

    {
      "$n performs %s that totally OBLITERATES $N with $s deadly #w!!",	/* 8: > 23   */
      "You perform %s that totally OBLITERATES $N with your deadly #w!!",
      "$n performs %s that OBLITERATES you with $s deadly #w!!"
    }
  };

  strcpy(cmisshit, (dam)?"\\c01 [Hit] \\c00":"\\c02 [Miss] \\c00");
  strcpy(vmisshit, (dam)?"\\c01 [Ouch] \\c00":"\\c02 [Whew] \\c00");

  w_type -= TYPE_HIT;		/* Change to base of table with text */

  if (dam == 0)		msgnum = 0;
  else if (dam <= 2)    msgnum = 1;
  else if (dam <= 10)    msgnum = 2;
  else if (dam <= 15)    msgnum = 3;
  else if (dam <= 20)   msgnum = 4;
  else if (dam <= 25)   msgnum = 5;
  else if (dam <= 30)   msgnum = 6;
  else if (dam <= 35)   msgnum = 7;
  else			msgnum = 8;

  /* damage message to onlookers */
  mess =  write_random_message(ch);

  buf = replace_string(dam_weapons[msgnum].to_room,
      attack_hit_text[w_type].singular, attack_hit_text[w_type].plural);

  sprintf(tmp,buf,mess);

  act(tmp, TRUE, ch, NULL, victim, TO_NOTVICTSPAM);

  /* damage message to damager */

  buf = replace_string(dam_weapons[msgnum].to_char,
      attack_hit_text[w_type].singular, attack_hit_text[w_type].plural);

  sprintf(tmp,buf,mess);

  if (nospam_set(ch))
    send_to_char( cmisshit, ch);
  else
    act(tmp, FALSE, ch, NULL, victim, TO_CHAR);
    

  /* damage message to damagee */

  buf = replace_string(dam_weapons[msgnum].to_victim,
      attack_hit_text[w_type].singular, attack_hit_text[w_type].plural);

  sprintf(tmp,buf,mess);

  if (nospam_set(victim))
    send_to_char(vmisshit, victim);
  else
    act(tmp, FALSE, ch, NULL, victim, TO_VICT | TO_SLEEP);
}


/*
 * message for doing damage with a spell or skill
 *  C3.0: Also used for weapon damage on miss and death blows
 */
int skill_message(int dam, struct char_data * ch, struct char_data * vict,
            int attacktype)
{
  int i, j, nr;
  struct message_type *msg;
  struct obj_data *weap = GET_EQ(ch, WEAR_WIELD_R);
  char miss[25];
  
  weap = ((weap) ? weap : GET_EQ(ch,WEAR_WIELD_L));

  strcpy(miss, "\\c02 [Miss] \\c00");
  
  for (i = 0; i < MAX_MESSAGES; i++) {
    if (fight_messages[i].a_type == attacktype) {
      nr = dice(1, fight_messages[i].number_of_attacks);
      for (j = 1, msg = fight_messages[i].msg; (j < nr) && msg; j++)
    msg = msg->next;

      if (!IS_NPC(vict) && (GET_LEVEL(vict) >= LVL_IMMORT)) {
        if (nospam_set(ch))
          send_to_char(miss, ch);
        else
          act(msg->god_msg.attacker_msg, FALSE, ch, weap, vict, TO_CHAR);
        if (nospam_set(vict))
          send_to_char(miss, vict);
        else
          act(msg->god_msg.victim_msg, FALSE, ch, weap, vict, TO_VICT);
    act(msg->god_msg.room_msg, FALSE, ch, weap, vict, TO_NOTVICTSPAM);
      } else if (dam != 0) {
    if (GET_POS(vict) == POS_DEAD) {
      if (nospam_set(ch))
        send_to_char("\r\n", ch);
      act(msg->die_msg.attacker_msg, FALSE, ch, weap, vict, TO_CHAR);
      if (nospam_set(vict))
        send_to_char("\r\n", vict);
      act(msg->die_msg.victim_msg, FALSE, ch, weap, vict, TO_VICT | TO_SLEEP);
      act(msg->die_msg.room_msg, FALSE, ch, weap, vict, TO_NOTVICT);
        } else {
          if (!nospam_set(ch) || !IS_WEAPON(attacktype))
        act(msg->hit_msg.attacker_msg, FALSE, ch, weap, vict, TO_CHAR);
      if (!nospam_set(vict) || !IS_WEAPON(attacktype))
        act(msg->hit_msg.victim_msg, FALSE, ch, weap, vict, TO_VICT | TO_SLEEP);
          if (!IS_WEAPON(attacktype))
            act(msg->hit_msg.room_msg, FALSE, ch, weap, vict, TO_NOTVICT);
          else
            act(msg->hit_msg.room_msg, FALSE, ch, weap, vict, TO_NOTVICTSPAM);
    }
      } else if (ch != vict) {	/* Dam == 0 */
        if (nospam_set(ch) && IS_WEAPON(attacktype))
          send_to_char(miss, ch);
        else
      act(msg->miss_msg.attacker_msg, FALSE, ch, weap, vict, TO_CHAR);
    if (nospam_set(vict) && IS_WEAPON(attacktype))
          send_to_char(miss, vict);
        else 
      act(msg->miss_msg.victim_msg, FALSE, ch, weap, vict, TO_VICT | TO_SLEEP);
        if (IS_WEAPON(attacktype))
          act(msg->miss_msg.room_msg, FALSE, ch, weap, vict, TO_NOTVICTSPAM);
        else
          act(msg->miss_msg.room_msg, FALSE, ch, weap, vict, TO_NOTVICT);
      }
      return 1;
    }
  }
  return 0;
}

void flee_check(struct char_data *victim, struct char_data *ch)
{
  int limit = 0;

       /* Can't flee when incap, dead or even sitting */
  if ((GET_POS(victim) < POS_FIGHTING) || (GET_HIT(victim) < 0))
    return;

  if (IS_NPC(victim)) {
    if (MOB_FLAGGED(victim, MOB_WIMPY) && (ch != victim))
      limit = GET_MAX_HIT(victim)/4; 
  } else {
    if (!GET_WIMP_LEV(victim)) /* Player chooses to never flee */
      return;
    if (PRF_FLAGGED(victim, PRF_PERCWIMPY))
      limit = (GET_WIMP_LEV(victim)*GET_MAX_HIT(victim))/100;
    else 
      limit = GET_WIMP_LEV(victim);    
  }

  if (limit > GET_HIT(victim)) {  /* Fleeing time */
    send_to_char("You wimp out, and attempt to flee!\r\n", victim);
    act("$n wimps out, and attempts to flee!\r\n", TRUE, victim, 0, 0, TO_NOTCHAR);
    if (!FIGHTING(victim)) {
      act("$n is rescued by divine forces.", FALSE, victim, 0, 0, TO_ROOM);
      GET_WAS_IN(victim) = victim->in_room;
      char_from_room(victim);
      char_to_room(victim, 0);
      return;
    }
    do_flee(victim, "", SCMD_WIMPY, 0);
  }
}

int is_dreamer(struct char_data *ch)
{
  if (IS_NPC(ch)) {
    if (MOB_FLAGGED(ch,MOB_TAR)) 
      return 1;
    else
      return 0;
  } else {
    if (IS_AFFECTED(ch,AFF_DREAMING)) 
      return 1;
    else
      return 0;
  }
  return 0;
}

/*
 * Checks for the existence of a protective shield around ch
 * and reduce the damage accordingly if so.
 */
void shield_reduce(struct char_data *ch, int *dam)
{
  struct affected_type *af;
  int found = 0;
  if (affected_by_spell(ch, SPELL_SHIELD_OF_SAIDIN) || affected_by_spell(ch, SPELL_STORMSHIELD)){
    for (af = ch->affected; af && !found; af = af->next) {
      if ((af->type == SPELL_SHIELD_OF_SAIDIN) || (af->type == SPELL_STORMSHIELD)){
        found = TRUE;
        if (*dam <= af->spell) {
          //act("$n seems to absorb some of the damage, somehow.", TRUE, ch, 0, 0, TO_NOTVICTSPAM);        
          //send_to_char("Your shield protects you somewhat.\r\n", ch);
          af->spell -= *dam;
          *dam /= 2;
        }
        else {
          act("$n's shimmering shield vanishes as $N breaks through it.", TRUE, ch, 0, ch, TO_NOTVICTSPAM);
          send_to_char("Your woven Shield vanishes.\r\n", ch);
          *dam -= af->spell/2;
          affect_from_char(ch, af->type);
        }
      }
    }
  }
}

void damage(struct char_data * ch, struct char_data * victim, int dam, int attacktype)
{
  int exp = 0,texp;
  int mprog = 0;
  float temp;
  struct affected_type arrestaff;

  if ((NULL == ch) || (NULL == victim))
    return;

  if (dam < 0)
    dam = 0;

  if (dam > 0x00005fff) 
    dam = 0x00005fff; /* Cap damage so no overflow'll happen */

  if (GET_POS(victim) <= POS_DEAD) {
    mlog("SYSERR: Attempt to damage a corpse.");
    return;			/* -je, 7/7/92 */
  }

  if (!IS_NPC(victim) && GET_DEST(victim)) {
    act("$n tries to attack $N, but the cart $E is riding in just speeds on, leaving $n in the dust.", TRUE, ch, 0, victim, TO_NOTVICT);
    asend_to_char(victim, "You're attacked by %s but the cart driver decides to run on, and leaves %s spluttering in the dust behind the cart\r\n", 
          GET_NAME(ch),GET_NAME(ch));
    return;
  }

  if ((is_dreamer(ch) && !is_dreamer(victim)) || (!is_dreamer(ch) && is_dreamer(victim))) {
    alog("SYSERR: %s tried to damage %s. One is dreaming, the other isn't!",GET_NAME(ch),GET_NAME(victim));
    return;
  }

  if (IS_AFFECTED2(victim,AFF2_LONGSLEEP)) {
    send_to_char("Your victim is immune to your puny attack attempts for the moment being...\r\n",ch);
    return;
  }

  /* peaceful rooms */
  if (ch->nr != real_mobile(DG_CASTER_PROXY) &&
      ch != victim && ROOM_FLAGGED(ch->in_room, ROOM_PEACEFUL)) {
    send_to_char("This room just has such a peaceful, easy feeling...\r\n", ch);
    return;
  }
  if (!IS_NPC(ch) && !IS_NPC(victim))
    if (GET_LEVEL(ch) < 20 || GET_LEVEL(victim) < 20) {
      send_to_char("Both you and your victim must be higher than level 20 todo this!\r\n", ch);
      return;
    }
  
  if (ch!=victim && IS_AFFECTED2(ch,AFF2_SUBDUED))
    {
    send_to_char("You can't fight anyone while being captured like this!\r\n",ch);
    return;
    }

  if (IS_AFFECTED2(ch, AFF2_SUBDUED) && victim->master == ch) {
    send_to_char("You have already subdued that person!\r\n", ch);
    return;
  }

  /* Shield of Saidin protection */
  shield_reduce(victim, &dam);  

  /* arrested protection */  
  if (IS_AFFECTED2(victim, AFF2_ARRESTED)) {
    send_to_char("You're not allowed to attack arrested people.\r\n", ch);
    return;
  }
  if (IS_AFFECTED2(ch, AFF2_ARRESTED)) {
    send_to_char("You're not allowed to attack anyone while arrested.\r\n", ch);
    return;
  }
  
  if(IS_AFFECTED2(ch, AFF2_SUBDUED))
    {
    send_to_char("You cannot attack anyone while captured!\n\r",ch);
    return;
    }

  /* shopkeeper protection */
  if (!ok_damage_shopkeeper(ch, victim))
    return;

  if (victim != ch) {
    if (GET_POS(ch) > POS_STUNNED) {
      if (!(FIGHTING(ch)))
    set_fighting(ch, victim);

      if (IS_NPC(ch) && IS_NPC(victim) && victim->master &&
      !number(0, 10) && IS_AFFECTED(victim, AFF_CHARM) &&
      (victim->master->in_room == ch->in_room)) {
    if (FIGHTING(ch))
      stop_fighting(ch);
    hit(ch, victim->master, TYPE_UNDEFINED);
    return;
      }
    }
    if (GET_POS(victim) > POS_STUNNED && !FIGHTING(victim)) {
      set_fighting(victim, ch);
      if (MOB_FLAGGED(victim, MOB_MEMORY) && !IS_NPC(ch) &&
      (GET_LEVEL(ch) < LVL_IMMORT))
    remember(victim, ch);
    }
  }
  if (victim->master == ch)
    stop_follower(victim);
 
  if (IS_AFFECTED(ch, AFF_INVISIBLE) || IS_AFFECTED(ch, AFF_HIDE))
    appear(ch);
    
    
  if (IS_AFFECTED(ch, AFF_GROUP)) { /* Seanchan Guild Skill */
    if (ch->master) {
      if (GET_SKILL(ch->master, SKILL_LEADERSHIP)) {
        if (ch->master == ch) {
          dam += GET_LEVEL(ch)/20;
        }
        else {
          dam += GET_LEVEL(ch->master)/40;
        }
      }
    }
  }

  if ((GET_SKILL(ch, SKILL_RAVAGE) >= number(0, 100)) && 
     (30 > number(0, 100)))
    dam *= 2;
  if ((attacktype > MAX_SPELLS) && !IS_NPC(ch) && 
     (GET_SKILL(ch, SKILL_ENHANCED_SPELL) > number(0, 100)))
    dam = (int) dam*1.25;   
  if (!IS_NPC(victim) && GET_SKILL(victim, SKILL_INTIMIDATE) > number (0, 100))
    dam -= 11;

  /* You can't damage an immortal! */

  if (!IS_NPC(victim) && (GET_LEVEL(victim) >= LVL_IMMORT))
    dam = 0;
  /* Range of damage is 0-20k */
    dam = MAX(0, MIN(20000, dam));

  /* Tadaa!!      This is where the damage is -done- */
  GET_HIT(victim) -= dam;

  if (GET_HIT(victim) <= -10) {
      mprog = check_for_mobprogs_and_stops(ch, victim, NULL, MPC_MOBDIE, 0, 0);
      mprog += check_for_mobprogs_and_stops(ch, victim, NULL, MPC_DIE, 0, 0);
    if (mprog)
      run_mobprog(ch, 0);
  }
  
  update_pos(victim);   
  /*
   * skill_message sends a message from the messages file in lib/misc.
   * dam_message just sends a generic "You hit $n extremely hard.".
   * skill_message is preferable to dam_message because it is more
   * descriptive.
   * 
   * If we are attacking with a weapon: If this is a miss or a
   * death blow, send a skill_message if one exists; if not, default to a
   * dam_message. Otherwise, always send a dam_message.
   */

  if (attacktype == 5000) // Should be bomb-message!
    ; 
  else if (attacktype > -1 && attacktype < MAX_SPELLS)
    gen_message(attacktype, ch, victim, TRUE, TRUE);
  else if (!IS_WEAPON(attacktype))
    skill_message(dam, ch, victim, attacktype);
  else {
    if (GET_POS(victim) == POS_DEAD || dam == 0) {
      if (!skill_message(dam, ch, victim, attacktype))
        dam_message(dam, ch, victim, attacktype);
    } else
        dam_message(dam, ch, victim, attacktype);
  }
  /* Use send_to_char -- act() doesn't send message if you are DEAD. */
  if (!nospam_set(ch))
  switch (GET_POS(victim)) {
    case POS_MORTALLYW:
      act("$n is mortally wounded, and will die soon, if not aided.", TRUE, victim, 0, 0, TO_ROOM);
      send_to_char("You are mortally wounded, and will die soon, if not aided.\r\n", victim);
      break;
    case POS_INCAP:
      act("$n is incapacitated and will slowly die, if not aided.", TRUE, victim, 0, 0, TO_ROOM);
      send_to_char("You are incapacitated an will slowly die, if not aided.\r\n", victim);
      break;
    case POS_STUNNED:
      act("$n is stunned, but will probably regain consciousness again.", TRUE, victim, 0, 0, TO_ROOM);
      send_to_char("You're stunned, but will probably regain consciousness again.\r\n", victim);
      break;
    case POS_DEAD:
      act("$n is dead!  R.I.P.", FALSE, victim, 0, 0, TO_ROOM);
      send_to_char("You are dead!  Sorry...\r\n", victim);
      if (GET_LEVEL(victim)<=5) {
        send_to_char("---------------------------------------------------------------------------\r\n", victim);
        send_to_char("&CYou've died.  Now what?&n\r\n\r\n", victim);
        send_to_char("Unlike a lot of places, death on Dragon's Fang doesn't really mean that\r\n", victim);
        send_to_char("much.  When you die here, you still have all of your equipment, gold,\r\n", victim);
        send_to_char("and all of that.  All that happens is that it's going to take a bit more\r\n", victim);
        send_to_char("Experience to make it to the next level.\r\n\r\n", victim);
        send_to_char("However, even that isn't something you'll have to worry about until you\r\n", victim);
        send_to_char("reach level 20.\r\n\r\n", victim);
        send_to_char("If you're seeing this message often, and want to learn how to survive the\r\n", victim);
        send_to_char("Mobs you can always ask for help from other players.  Either on the &gOOC&n\r\n", victim);
        send_to_char("channel or the &yQUESTION&n channel.  Usually there's at least one player on\r\n", victim);
        send_to_char("willing to help out.\r\n\r\n", victim);
        send_to_char("Also, there are dedicated &mNewbie-Guides&n who's job it is to help out new\r\n", victim);
        send_to_char("players.  You can see who's a &mNewbie-Guide&n by typing &CWHO -j&n.\r\n\r\n", victim);
        send_to_char("We hope you continue your stay, and have fun here at Dragon's Fang.\r\n", victim);
        send_to_char("\r\n&CTIP:&n To get back to Caemlyn type &cWAKE&n and then go &cDOWN&n.\r\n", victim);
        send_to_char("---------------------------------------------------------------------------\r\n", victim);
      }
      break;  
    default:			/* >= POSITION SLEEPING */
      if (dam > (GET_MAX_HIT(victim) >> 2))
        act("That really did \\c01HURT!\\c00", FALSE, victim, 0, 0, TO_CHAR);   
      if (GET_HIT(victim) < (GET_MAX_HIT(victim) >> 2) && !nospam_set(victim)) 
        send_to_char("\\c01You wish that your wounds would stop BLEEDING so much!\\c00\r\n", victim);
      break;
  }
  /* Flee moved to separate function...Got bloody spammy here */
  flee_check(victim, ch);  

  if (!AWAKE(victim))
    if (FIGHTING(victim))
      stop_fighting(victim);

  if (GET_POS(victim) == POS_DEAD) {
    if (IS_NPC(victim) || victim->desc) {
      if (IS_AFFECTED(ch, AFF_GROUP))
    group_gain(ch, victim);
      else {
        if ((GET_LEVEL(ch)-GET_LEVEL(victim)) > 40)
          exp = 1;
        else if ((GET_LEVEL(victim)-GET_LEVEL(ch)) > 40)
          exp = 500;
        else
        {
    /* This formula is a bit tricky, but the idea is to give the     */
    /* victor exp that is a 1/x*x function depending on level dif (x)*/
           
          temp = ((GET_LEVEL(ch)-GET_LEVEL(victim))/10.0) + 5.0;
          exp = (int) (25.0*(GET_EXP(victim)+100.0)/(temp*temp) - 100.0);
        }
    if (exp <= 0)   exp = 1;
    
    texp = calc_exp(ch, exp,victim);

        if (GET_LEVEL(ch) <= 199) { 
      if (texp > 1) {
            if (IS_NPC(victim)) {
          sprintf(buf2, "You receive %d experience points.\r\n", texp);
          send_to_char(buf2, ch);
            }
      }  else {
            if (IS_NPC(victim)) {
          send_to_char("You receive one lousy experience point.\r\n", ch);
            }
          }
      gain_exp(ch, exp,victim);
        }
      }
    }
    if (IS_NPC(victim) || (!IS_NPC(victim) && (!GET_SUBDUE(ch) || ch == victim)))
    {
      if (!IS_NPC(victim)) 
        {
        sprintf(buf2, "%s killed by %s at %s", GET_NAME(victim), GET_NAME(ch),
          world[victim->in_room].name);
        xlog(SYS_PDEATH, LVL_IMMORT, buf2, TRUE);
        if (MOB_FLAGGED(ch, MOB_MEMORY))
              forget(ch, victim);
        }
      die(victim, ch);
    }
    else
    {
       sprintf(buf2, "%s subdued by %s at %s", GET_NAME(victim), GET_NAME(ch),
             world[victim->in_room].name);
       xlog(SYS_PDEATH, LVL_IMMORT, buf2, TRUE);
       send_to_char("You were subdued!\r\n",victim);

       arrestaff = clear_aff_var(arrestaff);

       arrestaff.type = SKILL_ARREST;
       arrestaff.duration = 10;
       arrestaff.bitvector2 = AFF2_SUBDUED;
       arrestaff.weaver    = GET_IDNUM(ch); 

       affect_join(victim,&arrestaff, TRUE, TRUE, FALSE, FALSE);
       GET_POS(victim)=POS_STANDING;
       GET_HIT(victim)=1;
       if(FIGHTING(ch)==victim)
    stop_fighting(ch);
       if (FIGHTING(victim))
         stop_fighting(victim);
       if (victim->master && victim->master!=ch)
     stop_follower(victim);
       if(victim->master != ch)
         add_follower(victim, ch);     
      }
  }
  return;
}

int skill_ok(struct char_data *ch,int skill)
{
  
  if ((skill == SKILL_DODGE)||(skill == SKILL_DUCK) || (SKILL_JUMP == skill))
    return 1;
  else if ((skill == SKILL_SHIELD_BLOCK) || (skill == SKILL_REPEL)) {
    if (!GET_EQ(ch,WEAR_SHIELD))
      return 0;
    else
      return 1;
  }
  else if (skill == SKILL_PARRY) {
    if (!GET_EQ(ch,WEAR_WIELD_R) && !GET_EQ(ch,WEAR_WIELD_L))
      return 0;
    else
      return 1;
  }
  else
    return 0;
}
/* ch is defender, vict is attacker! */
int defense_skills(struct char_data *ch,struct char_data *vict)
{
  int result,skill,mod;

  int skills[] = {
    SKILL_DODGE,
    SKILL_PARRY,
    SKILL_SHIELD_BLOCK,
    SKILL_REPEL,
    SKILL_DUCK,
    SKILL_JUMP
  };

  #define ATTACKER 0
  #define PUBLIC   1
  #define VICTIM   2  
  #define MAX_MESS 3

  int generic_result_mod(struct char_data *ch,int skill_num,char *string,byte use_gen,int mod);
  int in_pos(struct char_data *ch); /* In pos for defending? */

  char *act_mess[6][MAX_MESS] = 
  {
    {
      "You dodge $Ns attack.",
      "$n dodges $Ns attack.",
      "$n dodges your attack."
    },
    {
      "You parry $Ns attack.",
      "$n parries $Ns attack.",
      "$n parries your attack."
    },
    {
      "You block $Ns attack with your shield.",
      "$n blocks $Ns attack with $s shield.",
      "$n blocks your attack with $s shield."
    },
    {
      "You repel $Ns attack with your shield.",
      "$n repels $Ns attack with $s shield.",
      "$n repels your attack with $s shield."
    },
    { 
      "You duck under $Ns attack.",
      "$n ducks under $Ns attack.",
      "$n makes a sneaky move and ducks your attack."
    },
    {
      "You jump out of the way of $Ns attack.",
      "$n jumps out of the way of $Ns attack",
      "$n jumps quickly out of the way of your attack."
    }
  };   /* fill up with messages for the rest of the skills */

  if (!in_pos(ch)) return 0;

  mod = number((GET_DEX(vict)/2),(GET_DEX(vict)));

  mod = MAX(10,MIN(mod,50));
  skill = number(0,5);
  
  if (!GET_SKILL(ch,skills[skill]))
    return 0;

  if (!skill_ok(ch,skills[skill]))
    return 0;

  result = generic_result_mod(ch,skills[skill],NULL,FALSE,-mod);

  if ((result == TOTAL_SUCCESS) || (result == SOME_SUCCESS)) {
    if (nospam_set(ch))
      send_to_char("\\c02 [Miss] \\c00", ch);
    else
      act(act_mess[skill][ATTACKER],FALSE,ch,NULL,vict,TO_CHAR);
    act(act_mess[skill][PUBLIC]  ,FALSE,ch,NULL,vict,TO_NOTVICTSPAM);
    if (nospam_set(vict))
      send_to_char("\\c02 [Miss] \\c00", vict);
    else    
    act(act_mess[skill][VICTIM]  ,FALSE,ch,NULL,vict,TO_VICT);
    return 1;
  }
  return 0; /* Only one attempt/attack! */
}


/* A cosmetic function that makes it look as if someone makes nice Blademaster moves. */
char *write_random_message(struct char_data *ch)
{
  static char *messages[] = {
    "Heron wading in the rushes",
    "Parting the silk",
    "Moon over the water",
    "The Swallow takes flight",
    "Ribbon in the Air",
    "Stones Falling From the Cliff",
    "Lightning of Three Prongs",
    "Leaf on the Breeze",
    "River undercuts the bank",
    "Cat Dances on the Wall",
    "Courtier taps his Fan",
    "Cat on Hot Sand",
    "Heron Spreads Its Wings",
    "Swallow Takes Flight",
    "Hummingbird Kisses the Honeyrose",
    "Stones Falls from the Mountain",
    "Thistledown Floats on the Whirlwind",
    "Folding the Fan",
    "Water Flows Downhill",
    "The Wood Grouse Dances",
    "The Boar Rushes Down the Mountain",
    "The Swallow Rides the Air",
    "The Dove Takes Flight",
    "The Falling Leaf",
    "The Kingsfisher Takes a Silverback",
    "Striking the Spark",
    "Wind and Rain", 
    "Watered Silk",
    "Boar Rushes Downhill",
    "Unfolding the Fan",
    "Lizard in the Thornbush",
    "Whirlwind on the Mountain",
    "Wind Blows Over the Wall",
    "Low Wind Rising",
    "Lion on the Hill",
    "Arc of the Moon",
    "Tower of Morning",
    "The Grapevine Twines",
    "Falcon Stoops",
    "Creeper Embraces the Oak",
    "Moon Rises Over the Lakes",
    "Wood Grouse Dances",
    "Folding the Air",
    "Leopard in the Tree"
  };  
  static char *animal_str = "a strange, unpredictable move";
  if (!IS_NPC(ch) || (IS_NPC(ch) && (MOB_FLAGGED(ch,MOB_HUMAN) || MOB_FLAGGED(ch,MOB_SHADOWSPAWN))))
    return (messages[number(0,43)]);
  else
    return animal_str;
}


void do_mob_fighting_skills(struct char_data *ch)
{
  int i = 0, j = 0, found =  FALSE;
  
  if (!IS_NPC(ch) || !FIGHTING(ch)) /* Safeguard */
    return;

  if (IS_AFFECTED(ch, AFF_BLIND)) /* Can't do this when blind! */
    return;

  for (i = 0; (i< 6) && !found; i ++) { /* Six checks if the mob has    */
    j = number(0, 31);                  /* a randomly selected fighting */
    switch (1 << j) {                   /* skill, and use it if it has  */
      case MOB_BASH:
        if (IS_SET(MOB_SKILLS(ch), MOB_BASH)) {
          do_bash(ch, GET_NAME(FIGHTING(ch)), 0, 0);
          found = TRUE;
        }
      break;
      case MOB_TRIP:
        if (IS_SET(MOB_SKILLS(ch), MOB_TRIP)) {
          do_trip(ch, GET_NAME(FIGHTING(ch)), 0, 0);
          found = TRUE;
        }
      break;
      case MOB_DIRTKICK:
        if (IS_SET(MOB_SKILLS(ch), MOB_DIRTKICK)) {
          do_dirtkick(ch, GET_NAME(FIGHTING(ch)), 0, 0);
          found = TRUE;
        }
      break;            
      case MOB_KICK:
        if (IS_SET(MOB_SKILLS(ch), MOB_KICK)) {
          do_kick(ch, GET_NAME(FIGHTING(ch)), 0, 0);
          found = TRUE;
        }
      break;      
      case MOB_DISARM:
        if (IS_SET(MOB_SKILLS(ch), MOB_DISARM)) {
          do_disarm(ch, GET_NAME(FIGHTING(ch)), 0, 0);
          found = TRUE;
        }
      break;      
      case MOB_STUN:
        if (IS_SET(MOB_SKILLS(ch), MOB_STUN)) {
          do_stun(ch, GET_NAME(FIGHTING(ch)), 0, 0);
          found = TRUE;
        }
      break;      
    }      
  }
  return;
}


void hit(struct char_data * ch, struct char_data * victim, int type)
{
  int  victim_ac, calc_thaco, mod = 0;
  int can_two = FALSE, i, botrh_mod = 0;
  struct char_data *leader, *k;
  int result;

  int attack_skills[] =
  {
    -1,
    SKILL_SECOND_ATTACK,
    SKILL_THIRD_ATTACK,
    SKILL_FOURTH_ATTACK,
    SKILL_FIFTH_ATTACK,
    SKILL_WARDERCOMBAT,
    SKILL_HASTE,
    SKILL_STUBBORNESS,
    SKILL_SPEARDANCE,
    0
  };
  
  void do_da_fighting(int calc_thaco,int dw,struct char_data *ch,struct char_data *victim,int type);
  int backstab_mult(int level);
  int in_pos(struct char_data *ch);  /* In pos for attacking/defending? */

  if( ch == NULL )
  {
    stop_fighting( victim );
    axlog( SYS_ERROR, LVL_GOD, TRUE, "SYSERR: '%s' is being hit by NULL.", GET_NAME( victim ) );
    return;
  }
  if( victim == NULL )
  {
    stop_fighting( ch );
//    axlog( SYS_ERROR, LVL_GOD, TRUE, "SYSERR: '%s' is attacking NULL.", GET_NAME( ch ) );
    return;
  }
  /* check if the character has a fight trigger */
  fight_mtrigger(ch);

  /* check if the victim has a hitprcnt trigger */
  hitprcnt_mtrigger(victim);

  if (NULL == ch) {
    stop_fighting(victim);
    return;
  }
  if (NULL == victim) {
    stop_fighting(ch);
    return;
  }

  if (IS_AFFECTED2(victim,AFF2_LONGSLEEP)) { // Can't attack longsleeping chars
    stop_fighting(victim);
    stop_fighting(ch); 
    return;
  }

  if (!in_pos(ch)) 
    return; /* Char not in pos for attacking! */
  
  if ((GET_POS(victim) == POS_DEAD) || (ch->in_room != victim->in_room)) {
    stop_fighting(ch);
    if (FIGHTING(victim))
      stop_fighting(victim);
    return;  
  }

  if (NULL == FIGHTING(victim))
    set_fighting(victim,ch);
  
  if (GET_SKILL(ch,SKILL_DEFENSE) && can_use_gskill(ch, SKILL_DEFENSE) && !IS_AFFECTED2(ch,AFF2_DEFENSE)) {
    do_defense(ch);
  }

  if (GET_SKILL(ch, SKILL_ADRENALIN))
    if (generic_result_mod(ch, SKILL_ADRENALIN, NULL, FALSE, 0))
      if (!IS_AFFECTED2(ch, AFF2_ADRENALIN)) {
        struct affected_type af2;
        af2 = clear_aff_var(af2);
        af2.type = SKILL_ADRENALIN;
        af2.duration = 1;
        af2.location = APPLY_XATTACKS;
        af2.modifier = 1;
        af2.bitvector2 = AFF2_ADRENALIN;
        affect_to_char(ch, &af2);
        af2 = clear_aff_var(af2);
        af2.type = SKILL_ADRENALIN;
        af2.duration = 1;
        af2.location = APPLY_HITROLL;
        af2.modifier = (int) (18 * GET_SKILL(ch, SKILL_ADRENALIN) / 100);
        af2.bitvector2 = AFF2_ADRENALIN;
        affect_to_char(ch, &af2);
        af2 = clear_aff_var(af2);
        af2.type = SKILL_ADRENALIN;
        af2.duration = 1;
        af2.location = APPLY_DAMROLL;
        af2.modifier = (int) (18 * GET_SKILL(ch, SKILL_ADRENALIN) / 100);
        af2.bitvector2 = AFF2_ADRENALIN;
        affect_to_char(ch, &af2);
      }
  
  if (GET_SKILL(ch, SKILL_BLADEMASTERY) || GET_SKILL(ch, SKILL_SPEARMASTERY))
    if (!IS_AFFECTED(ch, AFF_MASTERY))
      if (mastery(ch)){
        struct affected_type af;
        af = clear_aff_var(af);
        af.type = SKILL_BLADEMASTERY;
        af.duration = 1;
        af.modifier = -10;
        af.location = APPLY_AC;
        SET_BIT(af.bitvector, AFF_MASTERY);
        affect_to_char(ch, &af);
        af = clear_aff_var(af);
        af.type = SKILL_BLADEMASTERY;
        af.duration = 1;
        af.modifier = 30;
        af.location = APPLY_HITROLL;
        SET_BIT(af.bitvector, AFF_MASTERY);
        affect_to_char(ch, &af);
        af = clear_aff_var(af);
        af.type = SKILL_BLADEMASTERY;
        af.duration = 1;
        af.modifier = 30;
        af.location = APPLY_DAMROLL;
        SET_BIT(af.bitvector, AFF_MASTERY);
        affect_to_char(ch, &af);
      }
  if (GET_SKILL(ch, SKILL_STUBBORNESS))
    if (!IS_AFFECTED2(ch, AFF2_STUBBORNESS))
      if (get_guild(ch, 11)){
        struct affected_type af;
        af = clear_aff_var(af);
        af.type = SKILL_STUBBORNESS;
        af.duration = 10+GET_LEVEL(ch)/4;
        af.modifier = -10;
        af.location = APPLY_AC;
        SET_BIT(af.bitvector2, AFF2_STUBBORNESS);
        affect_to_char(ch, &af);
        af = clear_aff_var(af);
        af.type = SKILL_STUBBORNESS;
        af.duration = 10+GET_LEVEL(ch)/4;
        af.modifier = 10;
        af.location = APPLY_HITROLL;
        SET_BIT(af.bitvector2, AFF2_STUBBORNESS);
        affect_to_char(ch, &af);
        af = clear_aff_var(af);
        af.type = SKILL_STUBBORNESS;
        af.duration = 10+GET_LEVEL(ch)/4;
        af.modifier = 10;
        af.location = APPLY_DAMROLL;
        SET_BIT(af.bitvector2, AFF2_STUBBORNESS);
        affect_to_char(ch, &af);
        af = clear_aff_var(af);
        af.type = SKILL_STUBBORNESS;
        af.duration = 10+GET_LEVEL(ch)/4;
        af.modifier = 50;
        af.location = APPLY_ABS;
        SET_BIT(af.bitvector2, AFF2_STUBBORNESS);
        affect_to_char(ch, &af);
   }
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
    do_da_fighting(calc_thaco,FALSE,ch,victim,SKILL_BACKSTAB);
    return;
  }
  
  if (GET_SKILL(ch,SKILL_TWO_WEAPONS) && (GET_SKILL(ch,SKILL_TWO_WEAPONS) > number(0,100))) 
    can_two = TRUE;
    
  if (IS_NPC(ch)) 
  {
    do_mob_fighting_skills(ch);
    for(k = world[ch->in_room].people; k; k = k->next_in_room) {
      if (nospam_set(k) && ((k == ch) || (k == victim))) {
        act("$N attacks $n!",FALSE, ch, 0, victim, TO_NOTVICT);
        act("You attack $N!",FALSE, ch, 0, victim, TO_CHAR);
      }
    }
      
    for (i = 0; i <= (GET_LEVEL(ch)/20); i++)
    { 
      if (IS_AFFECTED2(ch, AFF2_CONFUSION) && (number(0,100) < 10)) {
        act("$n tries to hit $N, but is so confused $e misses totally.", TRUE, ch, 0, victim, TO_NOTVICT);
        sprintf(buf, "You try to hit %s but are so confused you miss totally.", GET_NAME(victim));
        send_to_char(buf, ch);
        return;
      }
      /* -20 % chance if room is foggy */
      if (ROOM_AFFECTED(ch->in_room, RAFF_FOG))
         mod = - 20;
      if (!defense_skills(victim,ch))
        do_da_fighting(mod+75+(GET_AC(victim)/4)+GET_HITROLL(ch),FALSE,ch,victim,type);
      update_pos(victim);
      if ((GET_POS(victim) == POS_DEAD) || (ch->in_room != victim->in_room)) return;
    }
  } 
  else  {
    for(k = world[ch->in_room].people; k; k = k->next_in_room) {
      if (((k == ch) || (k == victim)) && nospam_set(k)) {
        act("$N attacks $n!",FALSE, ch, 0, victim, TO_NOTCHAR);       
        act("You attack $N!",FALSE, ch, 0, victim, TO_CHAR);
      }
    }
    if (GET_XATTACKS(ch)) {
      if (ch->in_room != victim->in_room) return;

      for (i = 0; i < GET_XATTACKS(ch); i++) {
        calc_thaco = 100;
	/* If Dreaming, all skills are multiplied by dream skill % */
	if (NORMAL_DREAMER(ch) && !IS_NPC(ch)) calc_thaco = (calc_thaco*GET_SKILL(ch, SKILL_DREAM))/100;
	if (ROOM_AFFECTED(ch->in_room, RAFF_FOG)) calc_thaco -= 20;            
	if (IS_AFFECTED(ch, AFF_GROUP) && !IS_NPC(ch))  {
	  if ((ch->master) && (ch->master != ch))
	    leader = ch->master;
	  else
	    leader = ch;
          
	  if (GET_SKILL(leader, SKILL_LEADERSHIP)) {
	    if (leader == ch)
	      calc_thaco += GET_LEVEL(ch)/20;
	    else 
	      calc_thaco += GET_LEVEL(leader)/40;
	  }
	}    
      
	calc_thaco += GET_HITROLL(ch);
	calc_thaco += (int) ((GET_INT(ch) - 16) / 4);	/* Intelligence helps! */
	calc_thaco += (int) ((GET_WIS(ch) - 16) / 4);	/* So does wisdom */

	victim_ac = GET_AC(victim) / 10;
      
	if (AWAKE(victim))
	  victim_ac -= (int) ((GET_DEX(victim) - 16) / 4);

	victim_ac = 5 * MAX(-20, victim_ac);  /* -20 is lowest */

	calc_thaco += victim_ac;
	calc_thaco -= (int) ((GET_INT(victim) - 16) / 4); /* Victims int helps him */
	calc_thaco -= (int) ((GET_WIS(victim) - 16) / 4);
/*   
      if (!IS_NPC(victim) && GET_SKILL(victim, SKILL_BRACE) && (GET_SKILL(victim, SKILL_BRACE) > number(0,100))) 
      {    
           // BotRH auto guild skill 
        damage(victim, ch, GET_LEVEL(victim)/10, SKILL_BRACE);
        GET_HIT(ch) -= GET_LEVEL(victim)/10;
        update_pos(ch);
     }
*/
        if (!defense_skills(victim,ch))
	  do_da_fighting(calc_thaco,can_two,ch,victim,type);  
      }
    }
  
    if (!IS_NPC(ch))
      for (i = 0; attack_skills[i]; i++) 
      { 
	if ((ch->in_room != victim->in_room) || (GET_POS(victim) == POS_DEAD) ||
	  (IS_AFFECTED2(victim, AFF2_SUBDUED) && GET_SUBDUE(ch)))
	{							
	  if (FIGHTING(ch) && FIGHTING(ch) == victim)       
	    stop_fighting(ch);
	  if (FIGHTING(victim) && FIGHTING(victim) == ch) // something weird might happen
	    stop_fighting(victim);
	  return;
	} 
	if ((attack_skills[i] == -1) || IS_NPC(ch))
	  calc_thaco = 100;
	else if (GET_SKILL(ch,attack_skills[i]))
	  calc_thaco = GET_SKILL(ch,attack_skills[i]);
	else
	  continue;
	if (ROOM_AFFECTED(ch->in_room, RAFF_FOG))
	  calc_thaco -= 20;
	if (attack_skills[i] != -1) 
	{ result = generic_result(ch,attack_skills[i],NULL,FALSE);
	  if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) 
	  { damage(ch,victim,0,type);
	    continue;
	  }
	}
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
   
	if (GET_SKILL(victim, SKILL_BRACE) && (GET_SKILL(victim, SKILL_BRACE) > number(0,100))) 
	{    
           /* BotRH auto guild skill */
	  damage(victim, ch, GET_LEVEL(victim)/10, SKILL_BRACE);
	  GET_HIT(ch) -= GET_LEVEL(victim)/10;
	  update_pos(ch);
	}
     
	if (!defense_skills(victim,ch))
	  do_da_fighting(calc_thaco,can_two,ch,victim,type);  
      }
    if (!IS_NPC(ch) && GET_SKILL(ch,SKILL_MARTIAL_ARTS) && (GET_POS(victim) > POS_DEAD) &&
      (ch->in_room == victim->in_room))
      martial_arts(ch,NULL,0,0);
  }
  if (nospam_set(ch))
    send_to_char("\r\n", ch);
  if (!IS_NPC(ch) && (ch->in_room == victim->in_room))
    diag_char_to_char(victim, ch);
}

void check_for_armor_wearing_and_breaking(struct char_data *ch)
{
  struct obj_data *pieces[20];
  int i, j = 0, pos[20];
 
  // Something buggy here...Commenting out all in function /Fredrik 990227
  return;
  if (number(0, 100) > 1 ) /* Armor didn't wear down this time, lucky */
    return;

  for (i = 0; i < NUM_WEARS; i ++) {
    if (!GET_EQ(ch, i))
      continue;
    if (GET_OBJ_QCOST(GET_EQ(ch, i)))
      continue;

    if ((GET_OBJ_TYPE(GET_EQ(ch, i)) == ITEM_ARMOR) &&
      !IS_SET(OBJ_EXTRA_FLAGS(GET_EQ(ch, i)), ITEM_NOBREAK)) {
      pieces[j] = GET_EQ(ch, i);
      pos[j] = i;
      j++;
    }
  }
  if (!j)   /* Player is naked or has only !BREAK armor */
    return;

  i = number(0, j-1);
  if (GET_OBJ_STATUS(pieces[i]) < number(0, 40)) { /* Armor breaks! */
    act("AOW! That blow hit you so hard that $p shatters!", TRUE, ch, 
pieces[i], 0, TO_CHAR);
    obj_to_char(unequip_char(ch, pos[i]), ch);
    GET_OBJ_STATUS(pieces[i]) = 0;
    return;
  }
  if (!(GET_OBJ_STATUS(pieces[i])%10))
    act("Darn..$p got a big nasty scratch now.", TRUE, ch, pieces[i], 0, 
TO_CHAR);
  GET_OBJ_STATUS(pieces[i])--;
 }

void check_for_weapons_wearing_and_breaking(struct char_data *ch, struct 
obj_data *wpn1, struct obj_data *wpn2) {
  if ((wpn1) && !IS_SET(OBJ_EXTRA_FLAGS(wpn1), ITEM_NOBREAK)) {
    if (!GET_OBJ_QCOST(wpn1) && (number(0, 100) <= 4)) { /* It wears down */
      if (GET_OBJ_STATUS(wpn1) < number(0, 25)) { /* It breaks */
        act("Oh no! $p breaks by the force of your blow!", TRUE, ch,  wpn1, 0, TO_CHAR);
        obj_to_char(unequip_char(ch, WEAR_WIELD_R), ch);
        GET_OBJ_STATUS(wpn1) = 0;
      }
      GET_OBJ_STATUS(wpn1)--;
      if (!(GET_OBJ_STATUS(wpn1)%10))
        act("Hm. $p looks a little more dented now...", TRUE, ch, wpn1, 0,  TO_CHAR);
    }
  }
  if ((wpn2) && !IS_SET(OBJ_EXTRA_FLAGS(wpn2), ITEM_NOBREAK)) {
    if (!GET_OBJ_QCOST(wpn2) && number(0,100) <= 4) { /* it wears down */
      if (GET_OBJ_STATUS(wpn2) < number(0, 25)) { /* It breaks */
        act("Oh no! $p breaks by the force of your blow!", TRUE, ch,  wpn2, 0, TO_CHAR);
        obj_to_char(unequip_char(ch, WEAR_WIELD_L), ch);
        GET_OBJ_STATUS(wpn2) = 0;
      }
      GET_OBJ_STATUS(wpn2)--;
      if (!(GET_OBJ_STATUS(wpn2)%10))
        act("Hm...$p looks a little more worn.", TRUE, ch, wpn2, 0, TO_CHAR);
    }
  }
}

void do_da_fighting(int calc_thaco,int sw,struct char_data *ch,struct char_data *victim,int type)
{
  int diceroll,w_type = 0,dam = 0,dam2 = 0,w_type2 = 0;
  int is_dual = FALSE;
  struct obj_data *wielded  = GET_EQ(ch, WEAR_WIELD_R);
  struct obj_data *wielded2 = NULL;

  if (ch->in_room != victim->in_room)
    return;
  if (GET_POS(victim) == POS_DEAD)
    return;

  if ((wielded) && (CAN_WEAR(wielded,ITEM_WEAR_DUALWIELD)))
    is_dual = TRUE; 

  if (!wielded) 
    wielded = GET_EQ(ch,WEAR_WIELD_L);
  else 
    wielded2 = GET_EQ(ch, WEAR_WIELD_L);
  
  if (wielded && GET_OBJ_TYPE(wielded) == ITEM_WEAPON)
    w_type = GET_OBJ_VAL(wielded, 3) + TYPE_HIT;
  else if (IS_NPC(ch) && (ch->mob_specials.attack_type != 0))
    w_type = ch->mob_specials.attack_type + TYPE_HIT;
  else
    w_type = TYPE_HIT;
  
   
  if (wielded2 && GET_OBJ_TYPE(wielded2) == ITEM_WEAPON)
    w_type2 = GET_OBJ_VAL(wielded2, 3) + TYPE_HIT;
  else
    w_type2 = TYPE_HIT;

  diceroll = number(1,100);
  
  if ((diceroll > calc_thaco) || (diceroll == 1)) { /* it's a MISS! */
    if (type == SKILL_BACKSTAB) 
      damage(ch, victim, 0, SKILL_BACKSTAB);
    else {
      damage(ch, victim, 0, w_type);
      update_pos(victim);
      if ((sw) && (wielded) && (wielded2))
        damage(ch,victim,0,w_type2);
      update_pos(victim);
    }
  }
  else {
    if ((sw) && (wielded) && (wielded2)) {
      dam2 = (int) ((GET_STR(ch) - 16)/ 4);
      dam2 += GET_DAMROLL(ch);
      dam2 += dice(GET_OBJ_VAL(wielded,1), GET_OBJ_VAL(wielded,2));
    }

    dam = (int) ((GET_STR(ch) - 16) / 4);
    dam += GET_DAMROLL(ch);
    
    if (wielded) {
      dam += dice(GET_OBJ_VAL(wielded,1), GET_OBJ_VAL(wielded,2));
      if (IS_NPC(ch)) {
        dam += dice(ch->mob_specials.damnodice,ch->mob_specials.damsizedice);
      }
    }
    else if (IS_NPC(ch) && !wielded) 
      dam += dice(ch->mob_specials.damnodice, ch->mob_specials.damsizedice);
    else 
      dam += number(0,2);

    if (is_dual) dam = (int) (dam * 1.75);

    
        /* Worn weapons give less damage */
    if (wielded && !(IS_SET(OBJ_EXTRA_FLAGS(wielded), ITEM_NOBREAK)))
      dam = (dam*GET_OBJ_STATUS(wielded))/100;

    if (wielded2 && !(IS_SET(OBJ_EXTRA_FLAGS(wielded2), ITEM_NOBREAK)))
      dam2 = (dam2*GET_OBJ_STATUS(wielded2))/100;

    if (GET_POS(victim) < POS_FIGHTING) {
      dam *= 1 + (POS_FIGHTING - GET_POS(victim)) / 3;
      if ((sw) && (wielded) && (wielded2)) 
        dam2 *= 1 + (POS_FIGHTING - GET_POS(victim)) / 3;
    } 
    /* Position  sitting  x 1.33 */
    /* Position  resting  x 1.66 */
    /* Position  sleeping x 2.00 */
    /* Position  stunned  x 2.33 */
    /* Position  incap    x 2.66 */
    /* Position  mortally x 3.00 */

           /* BotRH guild skill: Lucky Strike *grin* */
           /* Also, the DF guild skill: Dark gift    */
    if ((GET_SKILL(ch, SKILL_STRIKE) || GET_SKILL(ch, SKILL_DARKGIFT)
    || GET_SKILL(ch, SKILL_CRITICAL_HIT)) && (25 > number(0, 100))) dam *= 2;
    if (!IS_NPC(victim)) {
      dam = MAX(1, (dam - MIN((int)(GET_ABS(victim)/10.0),40)));    /*deduct with victims abs/10, at least 1 hp damage min per hit */
      asend_to_char(ch,"Yo! You strike %s extremely hard with this attack!\r\n",GET_NAME(victim));			
    }					  /* max Abs at 40						 */
    if ((sw) && (wielded) && (wielded2) && !IS_NPC(victim))
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
        damage(ch, victim, dam, SKILL_BACKSTAB);
      }
      else {
        dam2 *= backstab_mult(GET_LEVEL(ch));
        if (get_guild(ch, 15))
          dam *= 2;
        damage(ch,victim,dam2,SKILL_BACKSTAB);
      }
      update_pos(victim);
    }
    else {
     damage(ch, victim, dam, w_type);
     update_pos(victim);
     if ((sw) && (wielded) && (wielded2) && (ch->in_room == victim->in_room)) 
       damage(ch,victim,dam2,w_type2);
     update_pos(victim);
    }
  }
  if (!IS_NPC(victim))
    check_for_armor_wearing_and_breaking(victim);
  if (!IS_NPC(ch))
    check_for_weapons_wearing_and_breaking(ch, wielded, wielded2);
}

int in_pos(struct char_data *ch)
{    
  return ((GET_POS(ch) == POS_STANDING) || (GET_POS(ch) == POS_FIGHTING));
}
  

/* control the fights going on.  Called every 2 seconds from comm.c. */
void perform_violence(void)
{
  struct char_data *ch;

  for (ch = combat_list; ch; ch = next_combat_list) {
      
    next_combat_list = ch->next_fighting;

    if (FIGHTING(ch) == NULL || (ch->in_room != FIGHTING(ch)->in_room)) {
      stop_fighting(ch);
      continue;
    }

    if (NULL == FIGHTING(FIGHTING(ch))) {
      hit(FIGHTING(ch),ch,TYPE_UNDEFINED);
    }

    if (IS_NPC(ch)) 
    {
      if (MOB_FLAGGED(ch,MOB_CHANNELER) && FIGHTING(ch)) 
        perform_mob_spells(ch);

      if (GET_MOB_WAIT(ch) > 0) 
      {
	GET_MOB_WAIT(ch) -= PULSE_VIOLENCE;
	continue;
      }
      GET_MOB_WAIT(ch) = 0;
      if ((GET_POS(ch) == POS_STUNNED) && IS_AFFECTED(ch,AFF_STUNNED))
        GET_POS(ch) = POS_STUNNED;
      else if (GET_POS(ch) < POS_FIGHTING) 
      {
	GET_POS(ch) = POS_FIGHTING;
	if (FIGHTING(ch) != NULL)
	  act("$n scrambles to $s feet!", TRUE, ch, 0, 0, TO_ROOM);
/*
        else
	  axlog( SYS_ERROR, LVL_GOD, TRUE, "SYSERR: Character that '%s' was fighting is
gone.", GET_NAME( ch ) );
*/
      }
    }

    if (GET_POS(ch) < POS_FIGHTING) 
    {
      send_to_char("You can't fight while sitting!!\r\n", ch);
      continue;
    }

    if (IS_AFFECTED(ch,AFF_GROUP))
      perform_autoassist(ch);

    hit(ch, FIGHTING(ch), TYPE_UNDEFINED);
    if (MOB_FLAGGED(ch, MOB_SPEC) && mob_index[GET_MOB_RNUM(ch)].func != NULL)
      (mob_index[GET_MOB_RNUM(ch)].func) (ch, ch, 0, "");
  }
}

int is_in_group(struct char_data *ch,struct char_data *vict)
{
  struct char_data   *k;
  struct follow_type *f;

  k = (ch->master ? ch->master : ch);
  
  if (k == vict)
    return 1;

  for (f = k->followers; f != NULL; f = f->next) {
    if (!IS_AFFECTED(f->follower,AFF_GROUP))
      continue;
    if (f->follower == vict)
      return 1;
  }
  return 0; 
}

int not_same_dreamer(struct char_data *ch,struct char_data *vict)
{
  if ((is_dreamer(ch) && !is_dreamer(vict)) || (!is_dreamer(ch) && is_dreamer(vict))) 
    return 1;
  else
    return 0;
}

/* Do damage to all in room. except memebers in your group */
/* No aggression though					   */
void do_area_ngroup_nagg(struct char_data *ch,int dam,int skill)
{
  struct char_data *list;

  for (list = world[ch->in_room].people; list != NULL; list = list->next_in_room) {
    if ((list == ch) || is_in_group(ch,list) || not_same_dreamer(ch,list))
      continue;
    else if (IS_AFFECTED2(list,AFF2_LONGSLEEP)) 
      continue;
    else if (MOB_FLAGGED(list, MOB_NOMAGIC))
      continue;
    else {
      if ((GET_LEVEL(list) < LVL_IMMORT) && ok_damage_shopkeeper(ch,list)) {
         GET_HIT(list) -= dam;
         update_pos(list);
         gen_message(skill,ch,list,TRUE,TRUE);
         if (GET_POS(list) == POS_DEAD) 
           die(list,ch);
      }
    } 
  }  
}

/* Do damage to all in room except members in your group */
/* The aggressive version! 				 */
void do_area_ngroup_agg(struct char_data *ch,int dam,int skill)
{
  struct char_data *list, *last;

  for (list = world[ch->in_room].people; list; list = last) {
    last = list->next_in_room;
    if (IS_AFFECTED2(list,AFF2_LONGSLEEP))
      continue;
    if (MOB_FLAGGED(list, MOB_NOMAGIC))
      continue;
    if ((list != ch) && !is_in_group(ch,list) &&
     (GET_LEVEL(list) < LVL_IMMORT) && ok_damage_shopkeeper(ch,list)) 
      damage(ch,list,dam,skill);
  }
}

/* do damage to ALL in room, except self   */
/* mobs/players won't be aggressive though */ 
void do_area_spell(struct char_data *ch,int damag,int skill)
{
  struct char_data *list,*next;
  
  for (list = world[ch->in_room].people; list != NULL; list = next) {
    next = list->next_in_room;
    if (IS_AFFECTED2(list,AFF2_LONGSLEEP))
      continue;
    if (MOB_FLAGGED(list, MOB_NOMAGIC))
      continue;

    if ((list != ch) && !not_same_dreamer(ch,list)) {
      if (ok_damage_shopkeeper(ch,list) && (GET_LEVEL(list) < LVL_IMMORT)) {
    GET_HIT(list) -= damag;
    
    update_pos(list);

    if (GET_POS(list) == POS_DEAD) {
      GET_POS(list) = POS_SITTING;
      damage(ch,list,10000,skill);
    } else {
      gen_message(skill,ch,list,TRUE,TRUE);
    }
      }
    }
  }
}  

void do_area_spell_agg(struct char_data *ch,int dam,int skill)
{
  struct char_data *list,*next;
  
  for (list = world[ch->in_room].people; list != NULL; list = next) {
    next = list->next_in_room;
    if (IS_AFFECTED2(list,AFF2_LONGSLEEP))
      continue;
    if (MOB_FLAGGED(list, MOB_NOMAGIC))
      continue;

    if ((list != ch) && (GET_LEVEL(list) < LVL_IMMORT) && ok_damage_shopkeeper(ch,list))
      damage(ch,list,dam,skill);
  }
}  

void do_own_damage(struct char_data *list)
{
  update_pos(list);

  switch (GET_POS(list)) {
    case POS_MORTALLYW:
      act("$n is mortally wounded, and will die soon, if not aided.", TRUE, list, 0, 0, TO_ROOM);
      send_to_char("You are mortally wounded, and will die soon, if not aided.\r\n", list);
      break;
    case POS_INCAP:
      act("$n is incapacitated and will slowly die, if not aided.", TRUE, list, 0, 0, TO_ROOM);
      send_to_char("You are incapacitated an will slowly die, if not aided.\r\n", list);
      break;
    case POS_STUNNED:
      act("$n is stunned, but will probably regain consciousness again.", TRUE, list, 0, 0, TO_ROOM);
      send_to_char("You're stunned, but will probably regain consciousness again.\r\n", list);
      break;
    case POS_DEAD:
      act("$n is dead!  R.I.P.", FALSE, list, 0, 0, TO_ROOM);
      send_to_char("You are dead!  Sorry...\r\n", list);
      raw_kill(list,list);
      break;
  }
}

int tp_message(int type, struct char_data *ch,struct char_data *vict,int target,int success)
{
  int i,j,nr;
  struct tp_message_type *msg;

  for (i = 0; i < MAX_SPELL_MESS; i++) {
    if (tp_messages[i].a_type == type) {
      nr = dice(1,tp_messages[i].number_of_attacks);
      for (j = 1,msg = tp_messages[i].msg; (j < nr) && msg; j++)
        msg = msg->next;

      if ((vict != NULL) && (target) && (success) && (GET_POS(vict) == POS_DEAD)) {
        send_to_char(CCYEL(ch,C_CMP),ch);
        act(msg->die_msg.weaver,FALSE,ch,NULL,vict,TO_CHAR);
        send_to_char(CCNRM(ch,C_CMP),ch);

        act(msg->die_msg.onl_tpu,FALSE,ch,NULL,vict,TO_TPUSER);
        act(msg->die_msg.onl_npu,FALSE,ch,NULL,vict,TO_NOTTPUSER);
     
        send_to_char(CCRED(vict,C_CMP),vict);
        act(msg->die_msg.vict_tpu,FALSE,ch,NULL,vict,TO_VICTTP);
        act(msg->die_msg.vict_ntp,FALSE,ch,NULL,vict,TO_VICTNOTTP);
        send_to_char(CCNRM(vict,C_CMP),vict);
      } 
      else if ((vict != NULL) && (success) && (target)) {
        send_to_char(CCYEL(ch,C_CMP),ch);
        act(msg->hit_msg.weaver,FALSE,ch,NULL,vict,TO_CHAR);
        send_to_char(CCNRM(ch,C_CMP),ch);
    
        act(msg->hit_msg.onl_tpu,FALSE,ch,NULL,vict,TO_TPUSER);
        act(msg->hit_msg.onl_npu,FALSE,ch,NULL,vict,TO_NOTTPUSER);

        send_to_char(CCRED(vict,C_CMP),vict);
        act(msg->hit_msg.vict_tpu,FALSE,ch,NULL,vict,TO_VICTTP);
        act(msg->hit_msg.vict_ntp,FALSE,ch,NULL,vict,TO_VICTNOTTP);
        send_to_char(CCNRM(vict,C_CMP),vict);
      }
      else if ((vict != NULL) && (!success) && (target)) {
        send_to_char(CCYEL(ch,C_CMP),ch);
        act(msg->miss_msg.weaver,FALSE,ch,NULL,vict,TO_CHAR);
        send_to_char(CCNRM(ch,C_CMP),ch);
 
        act(msg->miss_msg.onl_tpu,FALSE,ch,NULL,vict,TO_TPUSER);
        act(msg->miss_msg.onl_tpu,FALSE,ch,NULL,vict,TO_NOTTPUSER);

        send_to_char(CCRED(vict,C_CMP),vict);
        act(msg->miss_msg.vict_tpu,FALSE,ch,NULL,vict,TO_VICTTP);
        act(msg->miss_msg.vict_ntp,FALSE,ch,NULL,vict,TO_VICTNOTTP);
        send_to_char(CCNRM(vict,C_CMP),vict);
      }
      else if ((!target) && (success)) {
        act(msg->hit_msg.weaver,FALSE,ch,NULL,NULL,TO_CHAR);
        act(msg->hit_msg.onl_tpu,FALSE,ch,NULL,NULL,TO_TPUSER);
        act(msg->hit_msg.onl_npu,FALSE,ch,NULL,NULL,TO_NOTTPUSER);
      } 
      else if ((!target) && (!success)) {
        act(msg->miss_msg.weaver,FALSE,ch,NULL,NULL,TO_CHAR);
        act(msg->miss_msg.onl_tpu,FALSE,ch,NULL,NULL,TO_TPUSER);
        act(msg->miss_msg.onl_npu,FALSE,ch,NULL,NULL,TO_NOTTPUSER);
      } 
    }
  }
  return 0;
}

int spell_message(int type,struct char_data *ch,struct char_data *vict,int target,int success)
{
  int i,j,nr, found = 0;
  struct sp_message_type *msg;

  for (i = 0; i < MAX_SPELL_MESS; i++) {
    if (spell_messages[i].a_type == type) {
      found = 1;
      nr = dice(1,spell_messages[i].number_of_attacks);
      for (j = 1, msg = spell_messages[i].msg; (j < nr) && msg; j++)
        msg = msg->next;
      if ((vict != NULL) && (GET_POS(vict) == POS_DEAD) && (target)) {
        send_to_char(CCYEL(ch,C_CMP),ch);
        act(msg->die_msg.weaver,FALSE,ch,NULL,vict,TO_CHAR);
        send_to_char(CCNRM(ch,C_CMP),ch);			/*weaver */

        act(msg->die_msg.onl_css,FALSE,ch,NULL,vict,TO_OPUSERSS);
        act(msg->die_msg.onl_cos,FALSE,ch,NULL,vict,TO_OPUSERNSS);/* onlookers*/
        act(msg->die_msg.onl_nc ,FALSE,ch,NULL,vict,TO_NOTCHANN);

        send_to_char(CCRED(vict,C_CMP),vict);
        act(msg->die_msg.vict_css,FALSE,ch,NULL,vict,TO_VICTCSS);
        act(msg->die_msg.vict_cos,FALSE,ch,NULL,vict,TO_VICTCNS);
        act(msg->die_msg.vict_nc,FALSE,ch,NULL,vict,TO_VICTNC);
        send_to_char(CCNRM(vict,C_CMP),vict);			/* Target */
      }
      else if ((success) && (target)) {
        send_to_char(CCYEL(ch,C_CMP),ch);
        act(msg->hit_msg.weaver,FALSE,ch,NULL,vict,TO_CHAR);
        send_to_char(CCNRM(ch,C_CMP),ch);			/* Weaver */

        send_to_char(CCRED(vict,C_CMP),vict);
        act(msg->hit_msg.vict_css,FALSE,ch,NULL,vict,TO_VICTCSS);
        act(msg->hit_msg.vict_cos,FALSE,ch,NULL,vict,TO_VICTCNS);
        act(msg->hit_msg.vict_nc ,FALSE,ch,NULL,vict,TO_VICTNC);
        send_to_char(CCNRM(vict,C_CMP),vict);			/*Target!*/

        act(msg->hit_msg.onl_css,FALSE,ch,NULL,vict,TO_OPUSERSS);
        act(msg->hit_msg.onl_cos,FALSE,ch,NULL,vict,TO_OPUSERNSS);
        act(msg->hit_msg.onl_nc ,FALSE,ch,NULL,vict,TO_NOTCHANN);
      }
      else if ((!success) && (target)) {
       send_to_char(CCYEL(ch,C_CMP),ch);
       act(msg->miss_msg.weaver,FALSE,ch,NULL,vict,TO_CHAR);
       send_to_char(CCNRM(ch,C_CMP),ch);

       send_to_char(CCRED(vict,C_CMP),vict);
       act(msg->miss_msg.vict_css,FALSE,ch,NULL,vict,TO_VICTCSS);
       act(msg->miss_msg.vict_cos,FALSE,ch,NULL,vict,TO_VICTCNS);
       act(msg->miss_msg.vict_nc ,FALSE,ch,NULL,vict,TO_VICTNC);
       send_to_char(CCNRM(vict,C_CMP),vict);

       act(msg->miss_msg.onl_css,FALSE,ch,NULL,vict,TO_OPUSERSS);
       act(msg->miss_msg.onl_css,FALSE,ch,NULL,vict,TO_OPUSERNSS);
       act(msg->miss_msg.onl_nc ,FALSE,ch,NULL,vict,TO_NOTCHANN);
     }
     else if ((success) && (!target)) {
       act(msg->hit_msg.weaver ,FALSE,ch,NULL,vict,TO_CHAR);
       act(msg->hit_msg.onl_css,FALSE,ch,NULL,vict,TO_OPUSERSS);
       act(msg->hit_msg.onl_cos,FALSE,ch,NULL,vict,TO_OPUSERNSS);
       act(msg->hit_msg.onl_nc ,FALSE,ch,NULL,vict,TO_NOTCHANN);
     }
     else if ((!success) && (!target)) {
       act(msg->miss_msg.weaver ,FALSE,ch,NULL,vict,TO_CHAR);
       act(msg->miss_msg.onl_css,FALSE,ch,NULL,vict,TO_OPUSERSS);
       act(msg->miss_msg.onl_cos,FALSE,ch,NULL,vict,TO_OPUSERNSS);
       act(msg->miss_msg.onl_nc ,FALSE,ch,NULL,vict,TO_NOTCHANN);
     }
   }
  } 
  if (!found)
  {
    alog("Warning: Spell '%d' has no messages.", type);
    alog("   Debug info: MAX_SPELL_MESS = %d.", MAX_SPELL_MESS);
    alog("               Player casting: %s", GET_NAME(ch));
    if (target && vict)
      alog("               Target: %s", GET_NAME(vict));
  }
  return 1;
}

void check_mana(struct char_data *ch)
{
  int max = GET_MAX_MANA(ch),now = GET_MANA(ch),diff = 0,result = number(0,100);

  if (now >= 0) return;
  if (now < 0) now = -now;
  diff = (int) ((now /(float) max) * 100.0);
  
  remove_char_maintained(ch);
  remove_room_maintained(ch);
  if (GET_LEVEL(ch) < 20) {
    send_to_char("You would probably have burned yourself out by now, but\r\n", ch);
    send_to_char("since you're below level 20, we let it pass this time.\r\n", ch);
    send_to_char("Watch your mana....never let it go below 0.\r\n", ch);
    SET_MANA(ch, 0);
    return;
  }
  if (result < (diff / 2) ) {
    send_to_char("Argh! You overdrew and killed yourself!\r\n",ch);
    send_to_char("As a result you will be cut off from the One Power\r\n",ch);
    send_to_char("in your next incarnation!\r\n",ch);
    REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
    SET_BIT(GET_PERMANENT(ch),PRM_GENTLED);
    GET_MOVE(ch) = 0;
    GET_FRACT_MOVE(ch) = 0;
    GET_HIT(ch)   = 1;
    SET_MANA(ch, 0);
    raw_kill(ch,NULL);
    alog("%s killed by overchanneling.",GET_NAME(ch));
    return;
  }
  if (result < diff )
  {
    send_to_char("Whoops. You've seem to have burned yourself out!\r\n",ch);
    REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
    SET_BIT(GET_PERMANENT(ch),PRM_GENTLED);
    GET_POS(ch) = POS_SLEEPING;
    GET_HIT(ch)   = 1;
    GET_MOVE(ch) = 0;
    GET_FRACT_MOVE(ch) = 0;
    SET_MANA(ch, 0);
    alog("%s burned out by overchanneling.",GET_NAME(ch));
    return;
  }
  
  send_to_char("You feel VERY tired!\r\n",ch);
  GET_POS(ch)   = POS_SLEEPING;
  SET_MANA(ch, 0);
  GET_MOVE(ch) = 0;
  GET_FRACT_MOVE(ch) = 0;
  REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
}
