/******************************************************************************
 * $Id: act.social.c,v 1.7 2003/02/04 09:02:16 fang Exp $
 *   
 * File: act.social.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: act.social.c,v 
 *   Revision 1.2  2002/07/28 13:56:03  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.7  2002/05/27 12:31:27  fang
 *   *** empty log message ***
 *
 *   Revision 1.6  2002/05/27 10:52:48  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.5  2002/03/11 11:01:42  fang
 *   removed social coloring
 *
 *   Revision 1.4  2002/03/08 09:50:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/02/27 02:17:40  fang
 *   Fixed disguise so it saves.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.17  2002/01/24 21:33:12  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.16.2.2  2002/01/23 21:49:37  mud
 *   Removed objsave.[c|h] (old objsave stuff).
 *   Removed obj_file_elem and rent structs.
 *   Removed final vestiges of old guild system, except parts used
 *   in guild.[c|h] for backwards compatibility.
 *
 *   Revision 1.16.2.1  2002/01/21 01:23:39  mud
 *   Code cleanup. Aiel clans/socities gone. Lot of old guild code gone.
 *
 *   Revision 1.16  2001/08/23 14:32:21  mud
 *   *** empty log message ***
 *
 *   Revision 1.15  2001/03/30 18:47:37  mud
 *   Misc bug and typo fixes
 *
 *   Revision 1.14  2001/03/09 11:15:42  mud
 *   Fixed so it doesn't crash if a non-guilded GOD tries to glist
 *   Added showing of gchannel to "myguilds"
 *   Changed the parsing of the socials file (to handle comments)
 *
 *   Revision 1.13  2000/11/09 14:13:47  mud
 *   Getting closer to switching-dat with xml chars
 *
 *   Revision 1.12  2000/11/08 08:25:39  mud
 *   Moved update_p_tabel() to xmlcharsave.[ch].
 *
 *   Revision 1.11  2000/04/24 07:09:34  mud
 *   Fixed invstart.
 *
 *   Revision 1.10  2000/04/24 03:44:05  mud
 *   Final fix to club code hopefully.
 *
 *   Revision 1.9  2000/04/24 03:13:27  mud
 *   *** empty log message ***
 *
 *   Revision 1.8  2000/04/22 04:47:42  mud
 *   *** empty log message ***
 *
 *   Revision 1.7  2000/04/22 00:42:49  mud
 *   Fix to do_accept and addition of prog checking for entering/exiting
 *   wagons.
 *
 *   Revision 1.6  2000/04/20 17:15:20  mud
 *   Minor fix to wdo_accept.
 *
 *   Revision 1.5  2000/04/18 00:47:55  mud
 *   Minor fix to club code (do_reject)
 *
 *   Revision 1.4  2000/04/15 23:18:22  mud
 *   A bunch of things....fix to club code -addition- of call to save_char
 *   in ACMD(do_save) *whap Fred* and that's it methinks.
 *
 *   Revision 1.3  2000/04/15 01:59:46  mud
 *   Some modifications to club code. Mainly in the accept function.
 *
 *   Revision 1.2  2000/02/21 14:00:15  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* ************************************************************************
*   File: act.social.c                                  Part of CircleMUD *
*  Usage: Functions to handle socials                                     *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "conf.h"
#include "sysdep.h"
#include "class.h"
#include "act.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "spells.h"
#include "xmlcharsave.h"
#include "guild.h"

/* local globals */
int top_of_socialt = -1;

struct social_messg *soc_mess_list = NULL;

#define NUM_RESERVED_CMDS	15
  
void free_action(struct social_messg *mess)  {
   if (mess->command) free(mess->command);
   if (mess->sort_as) free(mess->sort_as);
   if (mess->char_no_arg) free(mess->char_no_arg);
   if (mess->others_no_arg) free(mess->others_no_arg);
   if (mess->char_found) free(mess->char_found);
   if (mess->others_found) free(mess->others_found);
   if (mess->vict_found) free(mess->vict_found);
   if (mess->char_body_found) free(mess->char_body_found);
   if (mess->others_body_found) free(mess->others_body_found);
   if (mess->vict_body_found) free(mess->vict_body_found);
   if (mess->not_found) free(mess->not_found);
   if (mess->char_auto) free(mess->char_auto);
   if (mess->others_auto) free(mess->others_auto);
   if (mess->char_obj_found) free(mess->char_obj_found);
   if (mess->others_obj_found) free(mess->others_obj_found);
   memset(mess, 0, sizeof(struct social_messg));
}
  
void de_boot_socials(void)
{
  int i;
  struct social_messg *this = soc_mess_list; 

  for (i = 0; i < top_of_socialt; i++) {
    this = &soc_mess_list[i];

    if (NULL != this->char_no_arg)   free(this->char_no_arg);
    if (NULL != this->others_no_arg) free(this->others_no_arg);
    if (NULL != this->char_found)    free(this->char_found);
    if (NULL != this->others_found)  free(this->others_found);
    if (NULL != this->vict_found)    free(this->vict_found);
    if (NULL != this->not_found)     free(this->not_found);
    if (NULL != this->char_auto)     free(this->char_auto);
    if (NULL != this->others_auto)   free(this->others_auto);
  }
  free(soc_mess_list);
  soc_mess_list = NULL;
  top_of_socialt = -1;
}

int find_action(int cmd)
{
  int bot, top, mid;

  bot = 0;
  top = top_of_socialt;

  if (top < 0)
    return (-1);

  for (;;) {
    mid = (bot + top) >> 1;

    if (soc_mess_list[mid].act_nr == cmd)
      return (mid);
    if (bot >= top)
      return (-1);

    if (soc_mess_list[mid].act_nr > cmd)
      top = --mid;
    else
      bot = ++mid;
  }
}



ACMD(do_action) {
  int act_nr;
  struct social_messg *action;
  struct char_data *vict;
  struct obj_data *targ;
  char bf[16384];
#define	do_act( a,b,c,d,e,f ) if( a ) { act( a,b,c,d,e,f ); } else { \
      sprintf(bf, "Social bad for '%s' in social file", CMD_NAME); \
      xlog(SYS_GOD, LVL_IMMORT, bf, TRUE); }

  /*search for the social*/
  if ((act_nr = find_action(cmd)) < 0) {
    send_to_char("That action is not supported.\r\n", ch);
    return;
  }
  action = &soc_mess_list[act_nr];

  /*parse the arguments and validate*/
  two_arguments(argument,buf, buf2);
  if ((!action->char_body_found) && (*buf2)) {
    send_to_char("Sorry, this social does not support body parts\r\n", ch );
    return;
  }
  if (!action->char_found) *buf = '\0';

  /*do no argument actions*/
  if (!*buf) {
    if (action->char_no_arg) {
      send_to_char(action->char_no_arg, ch);
      send_to_char("\r\n", ch);
    } else {
      sprintf(bf, "No message to character for '%s' in social file", CMD_NAME);
      xlog(SYS_GOD, LVL_IMMORT, bf, TRUE);
    }
    if ( action->others_no_arg ) {
      sprintf(bf,"%s%s%s","",action->others_no_arg,"");
      act(bf, action->hide, ch, 0, 0, TO_ROOM);
    }
    return;
  }

  /*do actions against someone*/
  if (!(vict = get_char_room_vis(ch, buf))) {
    /*no victim by that name, character have object?*/
    if( (action->char_obj_found) &&
      ((targ = get_obj_in_list_vis(ch, buf, ch->carrying)) ||
      (targ = get_obj_in_list_vis(ch, buf, world[ch->in_room].contents)))) {
      act(action->char_obj_found, action->hide, ch, targ, 0, TO_CHAR);
      do_act(action->others_obj_found, action->hide, ch, targ, 0, TO_ROOM);
      return;
    }
    send_to_char( action->not_found,ch );
    send_to_char( "\r\n",ch);
    return;
  } else if (vict == ch) {
    do_act( action->char_auto, action->hide, ch, 0, 0, TO_CHAR);
    sprintf(bf,"%s%s%s","",action->others_auto,"");
    act(bf, action->hide, ch, 0, 0, TO_ROOM);
  } else {
    if (GET_POS(vict) < action->min_victim_position) {
      act("$N is not in a proper position for that.",
      FALSE, ch, 0, vict, TO_CHAR | TO_SLEEP);
    } else {
      if (*buf2) {
	  do_act( action->char_body_found, 0, ch, (struct obj_data*)buf2, vict, TO_CHAR | TO_SLEEP );
	  act( action->others_body_found, action->hide, ch, (struct obj_data*)buf2, vict, TO_NOTVICT );
	  act( action->vict_body_found, action->hide, ch, (struct obj_data*)buf2, vict, TO_VICT );
      } else {
	do_act( action->char_found, 0, ch, 0, vict, TO_CHAR | TO_SLEEP );
        act( action->others_found, action->hide, ch, 0, vict, TO_NOTVICT );
	act( action->vict_found, action->hide, ch, 0, vict, TO_VICT );
      }
    }
  }
}



ACMD(do_insult)
{
  struct char_data *victim;

  one_argument(argument, arg);

  if (*arg) {
    if (!(victim = get_char_room_vis(ch, arg)))
      send_to_char("Can't hear you!\r\n", ch);
    else {
      if (victim != ch) {
    sprintf(buf, "You insult %s.\r\n", GET_NAME(victim));
    send_to_char(buf, ch);

    switch (number(0, 2)) {
    case 0:
      if (GET_SEX(ch) == SEX_MALE) {
        if (GET_SEX(victim) == SEX_MALE)
          act("$n accuses you of fighting like a woman!", FALSE, ch, 0, victim, TO_VICT);
        else
          act("$n says that women can't fight.", FALSE, ch, 0, victim, TO_VICT);
      } else {		/* Ch == Woman */
        if (GET_SEX(victim) == SEX_MALE)
          act("$n accuses you of having the smallest... (brain?)",
          FALSE, ch, 0, victim, TO_VICT);
        else
          act("$n tells you that you'd lose a beauty contest against a trolloc.",
          FALSE, ch, 0, victim, TO_VICT);
      }
      break;
    case 1:
      act("$n calls your mother a bitch!", FALSE, ch, 0, victim, TO_VICT);
      break;
    default:
      act("$n tells you to get lost!", FALSE, ch, 0, victim, TO_VICT);
      break;
    }			/* end switch */

    act("$n insults $N.", TRUE, ch, 0, victim, TO_NOTVICT);
      } else {			/* ch == victim */
    send_to_char("You feel insulted.\r\n", ch);
      }
    }
  } else
    send_to_char("I'm sure you don't want to insult *everybody*...\r\n", ch);
}

ACMD(do_dice)
{
  char arg1[50], arg2[50];
  byte dices1[5], dices2[5];
  struct char_data *victim;
  int bet, i, total1, total2;
  
  two_arguments(argument, arg1, arg2);
  
  if (!DICING(ch))
  {
    if (!*arg1)
      send_to_char("You have to first determine who to dice with and then how much to bet.\r\n", ch);
    else {
      if (!(victim = get_char_room_vis(ch, arg1)))
        send_to_char("There seems to be noone named that here.\r\n", ch);
      else if (IS_NPC(victim)) {
        send_to_char("Play with a mob? No sirree.\r\n", ch);
      }
      else if (DICING(victim) && (DICING(victim) != ch)){
        send_to_char("But that person is already occupied with a dice game!\r\n", ch);
      } else if (!victim->desc) {
        send_to_char("That player seems to be linkless right now. Try later.\r\n", ch);
      } else if (victim == ch) {
        send_to_char("Play dice with yourself? Boy, you must be lonely....\r\n", ch);
      } else {
        DICING(ch) = victim;
        if (DICING(DICING(ch)) != ch) {
           sprintf(buf, "%s invites you to play some dice.\r\n", GET_NAME(ch));
          send_to_char(buf, victim);
          sprintf(buf, "You invite %s to play some dice.\r\n", GET_NAME(victim));
          send_to_char(buf, ch);
          act("$n invites $N to play some dice.", TRUE, ch, 0, victim, TO_NOTVICT);
        }
        else {
           act("$n accepts $Ns invitation to dice.", TRUE, ch, 0, victim, TO_NOTVICT);
           sprintf(buf, "%s accepts your invitation to play.\r\n", GET_NAME(ch));
           send_to_char(buf, victim);
           sprintf(buf, "You accept %ss invitation to play dice.\r\n", GET_NAME(victim));
           send_to_char(buf, ch);
         }
      }
    }
  }
  else if (!DICEBET(ch)){
    if (!char_still_online(DICING(ch))) {
       send_to_char("Yer dicing bud is no longer here!\r\n",ch);
       DICING(ch) = NULL;
       return;
    }
    if (DICING(ch)->in_room != ch->in_room) {
      sprintf(buf, "It seems %s doesn't want to play dice...Dude's not here.\r\n", GET_NAME(DICING(ch)));
      send_to_char(buf, ch);
      DICEBET(DICING(ch)) = 0;
      DICING(DICING(ch)) = NULL;
      DICEBET(ch) = 0;
      DICING(ch) = NULL;
    } else if (!DICING(ch)->desc) {
      sprintf(buf,"%s is linkless at the moment. Wait and try later.\r\n", GET_NAME(DICING(ch)));
      send_to_char(buf, ch);
    } else if (!DICING(DICING(ch))) {
      sprintf(buf, "%s has not yet accepted your offer to play. Wait a bit.\r\n", GET_NAME(DICING(ch)));
      send_to_char(buf, ch);
      return;
    } else if (!*arg1)
      send_to_char("You have to set a wager before you play.\r\n", ch);
    else {
      bet = atoi(arg1);
      if ((bet <= 0) || (bet > GET_GOLD(ch)) || (bet > GET_GOLD(DICING(ch))))
        send_to_char("Try betting something reasonable.\r\n", ch);
      else if (DICEBET(DICING(ch)) && bet != DICEBET(DICING(ch))) {
        sprintf(buf, "But %s has bet %ld coins...\r\n", GET_NAME(DICING(ch)), DICEBET(DICING(ch)));
        send_to_char(buf, ch);
      } else {
        DICEBET(ch) = bet;
        if (DICEBET(DICING(ch))) {
          act("$n accepts $Ns bet.", TRUE, ch, 0, DICING(ch), TO_NOTVICT);
          sprintf(buf, "%s accepts your bet of %d coins.\r\n", GET_NAME(ch), bet);
          send_to_char(buf, DICING(ch));
          sprintf(buf, "You accept %ss bet of %d coins.\r\n", GET_NAME(DICING(ch)), bet);
          send_to_char(buf, ch);
        } else {
          act("$n places $s bet in the dicing game.", TRUE, ch, 0, DICING(ch), TO_NOTVICT);
          sprintf(buf, "%s bets %d coins.\r\n", GET_NAME(ch), bet);
          send_to_char(buf, DICING(ch));
          sprintf(buf, "You bet %d coins.\r\n", bet);
          send_to_char(buf, ch);
        }
      }
    }
  }
  else if (*arg1) {
    if (!char_still_online(DICING(ch))) {
       send_to_char("Seems your dice bud isn't here anymore.\r\n",ch);
       DICING(ch) = NULL;
       return;
    }
    if (!DICING(ch)->desc) {
      sprintf(buf,"%s is linkless at the moment. Wait and try later.\r\n", GET_NAME(DICING(ch)));
      send_to_char(buf, ch);
    } else if (!DICEBET(DICING(ch))) 
      act("$N has not yet accepted your bet!", 0, ch, 0, DICING(ch), TO_CHAR);
      else if (!strncasecmp(arg1, "off", strlen(arg1))) {
      act("$n stops playing dice with $N.", TRUE, ch, 0, DICING(ch), TO_NOTVICT);
      sprintf(buf,"%s stops playing with you.\r\n", GET_NAME(ch));
      send_to_char(buf, DICING(ch));
      sprintf(buf,"You stop playing dice with %s.\r\n", GET_NAME(DICING(ch)));
      send_to_char(buf, ch);
      DICEBET(DICING(ch)) = 0;
      DICING(DICING(ch)) = NULL;
      DICING(ch) = NULL;
      DICEBET(ch) = 0;
    }
  }
  else  /* Ok...There is an adversary, and there is a bet set. Let's play some dice */
  {
    if (!char_still_online(DICING(ch))) {
      DICING(ch) = NULL;
      send_to_char("Your dice bud is no longer here.\r\n",ch);
      return;
    }
    if (DICING(ch)->in_room != ch->in_room) {
      send_to_char("What the? Your counterpart in the dice game isn't here!\r\n", ch);
      DICEBET(DICING(ch)) = 0;
      DICING(DICING(ch)) = NULL;
      DICING(ch) = NULL;
      DICEBET(ch) = 0;
      return;
    } 
    if (!DICING(ch)->desc) {
      sprintf(buf,"%s is linkless at the moment. Wait and try later.\r\n", GET_NAME(DICING(ch)));
      send_to_char(buf, ch);
      return;
    }
    total1 = total2 = 0;
    for (i = 0; i < 5; i ++) {
      if (GET_TAVEREN(ch))
        dices1[i] = number(4,6);
      else 
        dices1[i] = number(1,6);
      total1 += dices1[i];
    }
    
    for (i = 0; i < 5; i ++) {
      if (GET_TAVEREN(DICING(ch)))
        dices2[i] = number(4,6);
      else 
        dices2[i] = number(1,6);
      total2 += dices2[i];
    }
    act("$ns rattles the dice, and then $N.", TRUE, ch, 0, DICING(ch), TO_NOTVICT);
    sprintf(buf, "%s's dice show %d,%d,%d,%d,%d and %s's dice %d,%d,%d,%d,%d.\r\n", 
      GET_NAME(ch), dices1[0],dices1[1],dices1[2],dices1[3],dices1[4],
      GET_NAME(DICING(ch)), dices2[0],dices2[1],dices2[2],dices2[3],dices2[4]);
    send_to_room(buf, ch->in_room);
    if (total1 == total2) {
      send_to_room("It was a tie.", ch->in_room);
      DICEBET(ch) = 0;
      DICEBET(DICING(ch)) = 0;
      return;
    }
    send_to_char("You won.\r\n", (total1 > total2)?ch: DICING(ch));
    send_to_char("You lost. Tough luck.\r\n", (total1 < total2)?ch: DICING(ch));
    act("$n won, and $N looks a little annoyed.", TRUE, (total1 > total2)?ch: DICING(ch), 0, 
      (total1 < total2)?ch: DICING(ch), TO_NOTVICT);

    if (total1 > total2) {
      sprintf(buf, "You just got %ld coins richer, thanks to %s.\r\n", DICEBET(ch), GET_NAME(DICING(ch)));
      send_to_char(buf, ch);
      GET_GOLD(ch)+=DICEBET(ch);
      GET_GOLD(DICING(ch)) -= MIN(GET_GOLD(DICING(ch)), DICEBET(ch));
      if (!GET_GOLD(DICING(ch))){
        act("$n just got broke on dicing.", TRUE, DICING(ch), 0, 0, TO_ROOM);
        send_to_char("You just lost your last coin. Better get some more.\r\n", DICING(ch));
      }
    } else {
      sprintf(buf, "You just got %ld coins richer, thanks to %s.\r\n", DICEBET(ch), GET_NAME(ch));
      send_to_char(buf, DICING(ch));
      GET_GOLD(DICING(ch))+=DICEBET(ch);
      GET_GOLD(ch) -= MIN(GET_GOLD(ch), DICEBET(ch));
      if (!GET_GOLD(ch)){
        act("$n just got broke on dicing.", TRUE, ch, 0, 0, TO_ROOM);
        send_to_char("You just lost your last coin. Better get some more.\r\n", ch);
      }    
    }
    DICEBET(ch) = 0;
    DICEBET(DICING(ch)) = 0;
    return;
  }
  return;
}    

int get_da_power(char *power)
{
  if (!strncasecmp("air",power,strlen(power)))
    return AIR;
  else if (!strncasecmp("earth",power,strlen(power)))
    return EARTH;
  else if (!strncasecmp("fire",power,strlen(power)))
    return FIRE;
  else if (!strncasecmp("spirit",power,strlen(power)))
    return SPIRIT;
  else if (!strncasecmp("water",power,strlen(power)))
    return WATER;
  else
    return -1;
  return -1;
}

ACMD(do_teach)
{
  struct char_data *vict;
  struct affected_type af;
  char arg2[200];
  int pwr=0,sumpwr = 0;
  char *pwrptr=0,*tpwrptr = 0;
  char *powerstr[]={
    "Air",
    "Earth",
    "Fire",
    "Spirit",
    "Water"
  };

  af = clear_aff_var(af);

  two_arguments(argument, arg,arg2);
  
  if (!GET_SKILL(ch, SKILL_TEACH)) {
    send_to_char("Huh? How?\r\n", ch);
    return;
  }
  if (!GET_CHAR_GUILDS(ch) && !is_dark(ch)) {
    send_to_char("How the hell did you get this skill!? It's for guilded only.\r\n", ch);
    return;
  }
  
  if (!*arg){
    send_to_char("You have to tell me who to teach as well...\r\n", ch);
    return;
  }
  
  if (!*arg2 || (-1 == (pwr = get_da_power(arg2)))) {
    send_to_char("Teach that person in what flow?\r\n",ch);
    return;
  }

  if (!(vict = get_char_room_vis(ch, arg))) {
    send_to_char("Teach who? \r\n", ch);
    return;
  }

  if (IS_NPC(vict)) {
    send_to_char("Very funny...teaching mobs..This attempt to abuse advantages is now logged.\r\n", ch);
    return;
  }

  if (IS_AFFECTED(vict,AFF_NOTEACH)) {
    send_to_char("You can't teach that person again so soon.\r\n",ch);
    return;
  }

  pwrptr = (&GET_AIR(vict));
  tpwrptr= (&GET_AIR(ch));

  sumpwr = GET_AIR(vict)+GET_FIRE(vict)+GET_WATER(vict)+GET_EARTH(vict)+GET_SPIRIT(vict);

  if ((pwrptr[pwr]/(double)tpwrptr[pwr]) >= 0.60) {
    send_to_char("Your student is far too advanced to be taught by you!\r\n",ch);
    return;
  }

  if ((sumpwr - GET_POWER(vict)) >= 200) {
    act("$N has already reached $es maximum strength in the One Power!",
     FALSE,ch,0,vict,TO_CHAR);
    act("$n tries to teach you, but you have already reached you maximum" 
       "strength in the One Power!",FALSE,ch,0,vict,TO_VICT);
    return;
  }
  if (TAUGHT(vict) > 20) {
    act("$N wouldn't gain anything from any more lessons.", TRUE, ch, 0, vict, TO_CHAR);
    return;
  }

  if (vict == ch) {
    send_to_char("Trying to teach yourself!? Yeah right.\r\n", ch);
    return;
  }
  
  if (cant_channel(vict)) {
    send_to_char("Why don't you try to teach a log as well? It knows just as much about OP.\r\n", ch);
    return;
  }

  if ((is_dark(ch) && !is_dark(vict)) || !share_guild(ch,vict)) {
    send_to_char("Sorry, you are only allowed to teach members of your guild.\r\n", ch);
    return;
  } 

  af.type = SKILL_TEACH;
  af.duration = 20;
  af.location = APPLY_NONE;
  af.modifier = 0;
  af.bitvector = AFF_NOTEACH;
  
  affect_to_char(vict, &af);
  
  pwrptr[pwr] += 1;

  act("$n teaches $N skillfully, and $N seems to see things differently now.", TRUE, ch, 0, vict, TO_NOTVICT);
  sprintf(buf,"%s gives you deeper insight in how to weave %s.\r\n", GET_NAME(ch),powerstr[pwr]);
  send_to_char(buf, vict);
  sprintf(buf, "You teach %s a little about %s.\r\n", GET_NAME(vict),powerstr[pwr]);
  send_to_char(buf, ch);  
  TAUGHT(vict) ++; /* Number of lessons taken */
}

ACMD(do_fire)
{
  struct char_data *victim;
  one_argument(argument, arg);
  
  if (!GET_NOBILITY(ch)){
    send_to_char("You're no nobleperson. Get someone to knight you, loser.\r\n", ch);
    return;
  }
  if (!*arg){
    send_to_char("Usage: Fire <servantname>\r\n", ch);
    return;
  }
  if (!(victim = get_char_room_vis(ch, arg))) {
    send_to_char("No such person here.\r\n", ch);
    return;
  } 
  if (IS_NPC(victim)){
    send_to_char("And just how are you going to fire a mob? Kill it instead.\r\n", ch);
    return;
  }
  if (!isname(GET_RNAME(ch), GET_SERVANT(victim))){
    sprintf(buf, "%s is not your servant!\r\n", GET_NAME(victim));
    send_to_char(buf, ch);
    return;
  }
  send_to_char("The lazy sod wasn't worth the money anyway...\r\n", ch);
  sprintf(buf, "%s has fired you!\r\n", GET_NAME(ch));
  send_to_char(buf, victim);
  sprintf(GET_SERVANT(victim), "                                  ");
  *GET_SERVANT(victim) = '\0';
}

ACMD(do_hire)
{
  struct char_data *victim;
  char *temp;
  
  temp = one_argument(argument, arg);

  if (!GET_NOBILITY(ch)){
    send_to_char("You're no nobleperson. Get someone to knight you, loser.\r\n", ch);
    return;
  }

  if ((!*arg) || (!*temp)){
    send_to_char("Syntax: Hire <playername>  <servant position name> \r\n", ch);
    return;
  }

  skip_spaces(&temp);

  if (strlen(temp) > (50 - strlen(GET_NAME(ch)))){
    send_to_char("Servant title is too long. Make it shorter, pls. \r\n", ch);
    return;
  }
  if (!(victim = get_char_room_vis(ch, arg))) {
    send_to_char("No such person here.\r\n", ch);
    return;
  } 

  if(GET_ISWOLFKIN(victim)) {
    send_to_char("You can't hire that brute!\r\n",ch);
    return;
  }

  if (IS_NPC(victim) || (victim == ch)) {
    send_to_char("Don't even think about it, bub. \r\n", ch);
    return;
  }
  if (*GET_SERVANT(victim)) {
    sprintf(buf,"%s is already serving someone else. Negotiate with employer first.\r\n", 
      GET_NAME(victim));
    send_to_char(buf, ch);
    return;
  }
  sprintf(buf, "You are %s's %s.", GET_NAME(ch), CAP(temp));
  sprintf(GET_SERVANT(victim), buf);
  send_to_char("Ok. You have now increased your staff by one. Hope this one serves you well.\r\n", ch);
  sprintf(buf, "%s has taken you into service as %s.\r\n", GET_NAME(ch), temp);  
  send_to_char(buf, victim);
}

char *fread_action(FILE * fl, int nr)
{
  char buf[MAX_STRING_LENGTH], *rslt;

 READ_LINE:
  fgets(buf, MAX_STRING_LENGTH, fl);
  if (feof(fl)) {
    fprintf(stderr, "fread_action - unexpected EOF near action #%d", nr);
    exit(1);
  }
  if (*buf == '#')
    return (NULL);
  else if (*buf == ';') {  /* comment */
    goto READ_LINE;
  }
  else {
    *(buf + strlen(buf) - 1) = '\0';
    CREATE(rslt, char, strlen(buf) + 1);
    strcpy(rslt, buf);
    return (rslt);
  }
}

/*
 * Fixes 1999-09-22, Stefan:
 *   When checking if a social exists in command_list, make sure the found 
 *   command is of type do_action. I.e we don't count a social that might 
 *   exist in social file, but only as a non-social in command list. 
 *
 *   When reading socials, make sure we never read more than we've found in top_of_socialt
 *   even if it means skipping valid socials (no mem over-writes). 
 *
 *   When encountering an invalid social(exists in file, but not in command list), 
 *   Make sure we skip it instead of reading it into a pos in the table
 */ 
void boot_social_messages(void) {
  FILE *fl;
  int nr=0, hide, min_char_pos, min_pos, min_level, curr_soc = -1;
  char next_soc[MAX_STRING_LENGTH], sorted[MAX_INPUT_LENGTH];

  /* open social file */
  if (!(fl = fopen(SOCMESS_FILE, "r"))) {
    sprintf(buf, "SYSLOG: Can't open socials file '%s'", SOCMESS_FILE);
    xlog(SYS_GOD, LVL_IMMORT, buf, TRUE);
    exit(1);
  }

  /* count socials & allocate space */
  *next_soc = '\0';
  while (!feof(fl)) {
    fgets( next_soc, MAX_STRING_LENGTH, fl);
    if (*next_soc == '~') top_of_socialt++;
  }
  sprintf( buf, "Social table contains %d socials.", top_of_socialt+1);
  xlog( SYS_GOD, LVL_IMMORT, buf, TRUE );
  rewind( fl );

  CREATE(soc_mess_list, struct social_messg, top_of_socialt + 1);

  /* now read 'em */
  for ( ;curr_soc <= top_of_socialt; ) {

    fscanf(fl, "%s", next_soc);
    if (feof(fl)) {
      fprintf(stderr, "fread_action - unexpected EOF near action #%d", nr);
      exit(1);
    }
    if (*next_soc == '$')
      break;
    else if (*next_soc == ';') {
      fgets(buf, MAX_STRING_LENGTH, fl);
      continue;
    }

    if (fscanf(fl, " %s %d %d %d %d \n", sorted, &hide, &min_char_pos, &min_pos, &min_level) != 5) {
      fprintf(stderr, "Format error in social file near social '%s'\n",
          next_soc);
      exit(1);
    }
    /* read the stuff */
      curr_soc++;
      soc_mess_list[curr_soc].command = str_dup( next_soc + 1 );
      soc_mess_list[curr_soc].sort_as = str_dup( sorted );
      soc_mess_list[curr_soc].hide = hide;
      soc_mess_list[curr_soc].min_char_position = min_char_pos;
      soc_mess_list[curr_soc].min_victim_position = min_pos;
      soc_mess_list[curr_soc].min_level_char = min_level;

      soc_mess_list[curr_soc].char_no_arg = fread_action(fl, nr);
      soc_mess_list[curr_soc].others_no_arg = fread_action(fl, nr);
      soc_mess_list[curr_soc].char_found = fread_action(fl, nr);
      soc_mess_list[curr_soc].others_found = fread_action(fl, nr);
      soc_mess_list[curr_soc].vict_found = fread_action(fl, nr);
      soc_mess_list[curr_soc].not_found = fread_action(fl, nr);
      soc_mess_list[curr_soc].char_auto = fread_action(fl, nr);
      soc_mess_list[curr_soc].others_auto = fread_action(fl, nr);
      soc_mess_list[curr_soc].char_body_found = fread_action(fl, nr);
      soc_mess_list[curr_soc].others_body_found = fread_action(fl, nr);
      soc_mess_list[curr_soc].vict_body_found = fread_action(fl, nr);
      soc_mess_list[curr_soc].char_obj_found = fread_action(fl, nr);
      soc_mess_list[curr_soc].others_obj_found = fread_action(fl, nr);
  }

  /* close file & set top */
  fclose(fl);
  top_of_socialt = curr_soc;
}

/* this function adds in the loaded socials and assigns them a command # */
void create_command_list(void)  {
   int i, j, k;
   struct social_messg temp;
  
   /* free up old command list */
   if (complete_cmd_info) free(complete_cmd_info);
   complete_cmd_info = NULL;

   /* re check the sort on the socials */
   for (j = 0; j < top_of_socialt; j++) {
      k = j;
      for (i = j + 1; i <= top_of_socialt; i++)
      	if (str_cmp(soc_mess_list[i].sort_as, soc_mess_list[k].sort_as) < 0)
      	k = i;
      if (j != k) {
	 temp = soc_mess_list[j];
 	soc_mess_list[j] = soc_mess_list[k];
	 soc_mess_list[k] = temp;
        }
     }

   /* count the commands in the command list */
   i = 0;
   while(*cmd_info[i].command != '\n') i++;
   i++;

   CREATE(complete_cmd_info, struct command_info, top_of_socialt + i + 2);

   /* this loop sorts the socials and commands together into one big list */
   i = 0;
   j = 0;
   k = 0;
   while ((*cmd_info[i].command != '\n') || (j <= top_of_socialt))  {
      if ((i < NUM_RESERVED_CMDS) ||
	  (j > top_of_socialt) ||
	  (str_cmp(cmd_info[i].sort_as, soc_mess_list[j].sort_as) < 1))
      	complete_cmd_info[k++] = cmd_info[i++];
      else  {
	 soc_mess_list[j].act_nr		= k;
	 complete_cmd_info[k].command		= soc_mess_list[j].command;
	 complete_cmd_info[k].sort_as		= soc_mess_list[j].sort_as;
	 complete_cmd_info[k].minimum_position	= soc_mess_list[j].min_char_position;
	 complete_cmd_info[k].command_pointer	= do_action;
	 complete_cmd_info[k].minimum_level    	= soc_mess_list[j++].min_level_char;
	 complete_cmd_info[k++].subcmd		= 0;
         complete_cmd_info[k].command_groups    = 0;
      }
   }
   complete_cmd_info[k].command		= str_dup("\n");
   complete_cmd_info[k].sort_as		= str_dup("zzzzzzz");
   complete_cmd_info[k].minimum_position = 0;
   complete_cmd_info[k].command_pointer	= 0;
   complete_cmd_info[k].minimum_level	= 0;
   complete_cmd_info[k].subcmd		= 0;
   complete_cmd_info[k].command_groups  = 0;
   sprintf(buf, "Command info rebuilt, %d total commands.", k);
   mlog(buf);
 }
