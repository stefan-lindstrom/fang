/******************************************************************************
 * $Id: limits.c,v 1.17 2003/12/23 05:28:48 fang Exp $
 *   
 * File: limits.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: limits.c,v $
 *   Revision 1.17  2003/12/23 05:28:48  fang
 *   Tiek: Moved ban/unban, force-rent, and descriptor without char from syslog to xlogs
 *
 *   Revision 1.16  2003/12/04 07:02:20  fang
 *   tze: mebbe actually fixed the damn who list this time
 *
 *   Revision 1.15  2003/12/01 10:03:12  fang
 *   *** empty log message ***
 *
 *   Revision 1.14  2003/11/23 08:34:24  fang
 *   oops. corrected port number on who page
 *
 *   Revision 1.13  2003/11/23 08:02:24  fang
 *   Tze: Changed who page to write every 30 seconds as per admin request
 *
 *   Revision 1.12  2003/08/16 16:17:19  fang
 *   Changed colors and fonts on the webpage who list to match the reset of the site
 *
 *   Revision 1.11  2003/08/13 07:40:11  fang
 *   changed directory for webpage who list
 *
 *   Revision 1.10  2003/06/17 10:30:05  fang
 *
 *   Removed a new set of "externs" from source files. For fucks sake people,
 *   let's keep those externs where they belong, in an apropriate header, ok?
 *
 *   Revision 1.9  2003/06/12 07:22:13  fang
 *   Changed testing for channeling ability as per HGL request. Fixed Room Seal to work as originally intended. Commented out ignorelist. Will fully remove once certain no problems will arise
 *
 *   Revision 1.8  2003/03/01 07:17:05  fang
 *   Fisch: Multiflag for authorized multies
 *
 *   Revision 1.7  2002/11/22 08:43:29  fang
 *   fisch: added 5 minute check/warning for multies
 *
 *   Revision 1.6  2002/11/07 04:02:02  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.5  2002/10/29 03:09:26  fang
 *   fisch: added the command "scene".  Scenes are like doings for rooms.
 *
 *   Revision 1.4  2002/09/13 22:51:59  fang
 *   fischer: more debug code to track guildie pointer error
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
 *   Revision 1.11  2002/05/27 10:52:49  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.10  2002/04/19 17:14:12  fang
 *   Fixed rp exp problem.
 *
 *   Revision 1.9  2002/04/02 07:09:30  fang
 *   *** empty log message ***
 *
 *   Revision 1.7  2002/03/14 16:50:40  fang
 *   *** empty log message ***
 *
 *   Revision 1.6  2002/02/28 17:02:35  fang
 *   *** empty log message ***
 *
 *   Revision 1.5  2002/02/25 16:01:32  fang
 *   *** empty log message ***
 *
 *   Revision 1.4  2002/02/25 15:43:42  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/02/24 12:25:20  fang
 *   Fixed problem with chars going back to 10186 when force extracted from idling.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.32  2002/01/30 14:37:46  mud
 *   Mino5r bugfixes. Wagons now pulled through gates. Notifylist expanded to 10 names, etc..
 *
 *   Revision 1.31  2002/01/15 19:17:26  mud
 *   Warning free...let's keep it that way...
 *
 *   Revision 1.30  2001/11/06 08:10:01  mud
 *   Cart&Speedwalk/Travel now stopped by DomeOfAir
 *
 *   Revision 1.29  2001/09/30 13:30:05  mud
 *   *** empty log message ***
 *
 *   Revision 1.28  2001/09/30 10:18:51  mud
 *   *** empty log message ***
 *
 *   Revision 1.27  2001/09/28 13:59:26  mud
 *   *** empty log message ***
 *
 *   Revision 1.26  2001/09/28 07:57:28  mud
 *   *** empty log message ***
 *
 *   Revision 1.25  2001/09/03 03:22:30  mud
 *   *** empty log message ***
 *
 *   Revision 1.24  2001/09/03 03:19:09  mud
 *   *** empty log message ***
 *
 *   Revision 1.23  2001/08/20 13:49:46  mud
 *   *** empty log message ***
 *
 *   Revision 1.22  2001/08/19 08:58:38  mud
 *   *** empty log message ***
 *
 *   Revision 1.21  2001/08/14 15:43:32  mud
 *   *** empty log message ***
 *
 *   Revision 1.20  2001/08/14 12:52:34  mud
 *   Changed exp cap to 1000.
 *
 *   Revision 1.19  2001/08/11 17:49:53  mud
 *   Modified to make wardersense and warderheal obsolete and have the code that
 *   checked for those skills check to see if the character is a bonded Gaidin - Illaria
 *
 *   Revision 1.18  2001/07/04 13:58:33  mud
 *   Fixed so that Invix Imms don't show up on who.html  - Illaria
 *
 *   Revision 1.17  2001/05/22 09:07:05  mud
 *   DG-Scripts for Fang.
 *
 *   Revision 1.16  2001/04/21 10:21:50  mud
 *   *** empty log message ***
 *
 *   Revision 1.15  2001/04/04 18:35:28  mud
 *   Bug fixing
 *
 *   Revision 1.14  2001/03/30 18:47:37  mud
 *   Misc bug and typo fixes
 *
 *   Revision 1.13  2001/03/09 11:15:43  mud
 *   Fixed so it doesn't crash if a non-guilded GOD tries to glist
 *   Added showing of gchannel to "myguilds"
 *   Changed the parsing of the socials file (to handle comments)
 *
 *   Revision 1.12  2001/01/24 17:16:04  mud
 *   Changed newbie equipment. Changed max exp gain for newbies from 1000 to 500
 *
 *   Revision 1.11  2001/01/19 17:51:50  mud
 *   Added line numbers to the editor.
 *
 *   Revision 1.10  2000/05/24 08:41:13  mud
 *   Aiel endurance changes
 *
 *   Revision 1.9  2000/05/08 15:20:35  mud
 *   *** empty log message ***
 *
 *   Revision 1.8  2000/04/25 12:55:22  mud
 *   Reports correct gained exp now.
 *
 *   Revision 1.7  2000/04/25 11:10:38  mud
 *   EXP gain is modified on behalf of newbies (lvl <= 20)
 *
 *   Revision 1.6  2000/04/25 09:00:19  mud
 *   Minor modification to rp exp. Emote delay is 10 tic's before no exp is
 *   gained in relation to another rp'er in the room and for multiple
 *   people in the room, the rp exp added is now (xpadd += (people*2))
 *
 *   Revision 1.5  2000/04/22 09:43:33  mud
 *   Added a 'die_follower' before the entire extraction part of idling in
 *   void.
 *
 *   Revision 1.4  2000/04/09 20:34:04  mud
 *   Changed player obj saving to XML format.
 *
 *   Revision 1.3  2000/03/02 17:56:13  mud
 *   Change to check for production port instead of development port for five_minute_update function
 *
 *   Revision 1.2  2000/02/21 14:00:17  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* ************************************************************************
*   File: limits.c                                      Part of CircleMUD *
*  Usage: limits & gain funcs for HMV, exp, hunger/thirst, idle time      *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */
#include "skimming.h"
#include "bitvector.h"
#include "act.h"
#include "xmlobjsave.h"
#include "conf.h"
#include "graph.h"
#include "sysdep.h"
#include "fight.h"
#include "config.h"
#include "class.h"
#include "spec_procs.h"
#include "structs.h"
#include "utils.h"
#include "spells.h"
#include "comm.h"
#include "db.h"
#include "handler.h"
#include "limits.h"
#include "color.h"
#include "dg_scripts.h"
#include "retainer.h"


/*
 * The hit_limit, mana_limit, and move_limit functions are gone.  They
 * added an unnecessary level of complexity to the internal structure,
 * weren't particularly useful, and led to some annoying bugs.  From the
 * players' point of view, the only difference the removal of these
 * functions will make is that a character's age will now only affect
 * the HMV gain per tick, and _not_ the HMV maximums.
 */

/* manapoint gain pr. game hour */
int mana_gain(struct char_data * ch)
{
  int gain, grouped = 0;
  struct char_data *leader, *ppl;
  struct follow_type *f = NULL;

  if (IS_NPC(ch)) {
    /* Neat and fast */
    gain = GET_LEVEL(ch)/25 + 1;
  } else {
    gain = 1 + (GET_MAX_MANA(ch)/16 + GET_WIS(ch) + GET_INT(ch))/25;

    /* Class calculations */
    if ((GET_CLASS(ch) == CLASS_CHANNELER) || (GET_CLASS(ch) == CLASS_WILDER))
      gain *= 1.3;
    if (GET_SKILL(ch, SKILL_BLACK_ARTS)) 
      gain *= 1.3;
    /* Skill/Spell calculations */
    if (GET_SKILL(ch, SKILL_QUICK_MIND) && GET_SKILL(ch, SKILL_QUICK_MIND) > number(0, 100))
      gain *= 1.5;
    if (GET_SKILL(ch, SKILL_MEDITATION) && GET_SKILL(ch, SKILL_MEDITATION) > number(0, 100))
      gain *= 1.5;
    if (GET_SKILL(ch, SKILL_BANDHEAL) && IS_AFFECTED(ch, AFF_GROUP)){
      if (ch->master) 
        leader = ch->master;
      else leader = ch;
      for (f = leader->followers; f; f = f->next)
        if (GET_SKILL(f->follower, SKILL_BANDHEAL) && (f->follower != ch))
          grouped = 1;
      if (grouped)
         gain *= 2;
    }   
    /* Position calculations    */
    if (NORMAL_DREAMER(ch))
      gain <<= 1;
    else
    switch (GET_POS(ch)) {
    case POS_SLEEPING:
      gain += gain;
      break;
    case POS_RESTING:
      gain += (gain >> 1);	/* Divide by 2 */
      break;
    case POS_SITTING:
      gain += (gain >> 2);	/* Divide by 4 */
      break;
    case POS_FIGHTING:
      gain = gain/2;
      break;
    }
  }

  if ((GET_COND(ch, FULL) == 0) || (GET_COND(ch, THIRST) == 0))
    gain >>= 2;

  gain = MAX(1,gain);

  if (IS_SET(PRF_FLAGS(ch),PRF_GRASPING) || IS_SET(PRF_FLAGS(ch),PRF_TGRASP))
    gain = 0;
  
  if (NOWHERE != ch->in_room) {  
    if (IS_AFFECTED(ch, AFF_POISON) || ROOM_AFFECTED(ch->in_room, RAFF_COLD))
      gain = 0;

    if (room_affected_by_spell(&world[ch->in_room], SPELL_SANCTUARY))
      gain = (int) gain * 1.5;
  }

  if (IS_AFFECTED2(ch,AFF2_CONCENTRATE))
    gain += 3;		/* Always gain +3 in mana when concentrating! */

  if (NOWHERE != ch->in_room) {
    for (ppl = world[ch->in_room].people; ppl; ppl = ppl->next_in_room) 
      if (affected_by_spell(ppl, SKILL_CAMP))
        gain = (int) (1.25*gain);
  }

  /* Half gain if Longing. */
  if (GET_OGIER(ch) && (LONGING_NOGAIN >= GET_LONGING(ch)))
    gain /= 2;

  gain = MAX(0,gain);

  return (gain);
}


int hit_gain(struct char_data * ch)
/* Hitpoint gain pr. game hour */
{
  int gain, grouped = 0;
  struct follow_type *f = NULL;
  struct char_data *leader, *ppl;
      
  if (IS_NPC(ch)) {
    gain = 1 + GET_LEVEL(ch)/25;
    /* Neat and fast */
  } else {

    gain = 1 + (GET_MAX_HIT(ch)/16 + GET_CON(ch)*2)/25;

    /* Class/Level calculations */
    if ((GET_CLASS(ch) == CLASS_ADVENTURER) || (GET_CLASS(ch) == CLASS_SOLDIER))
      gain *= 1.3;
    /* Skill/Spell calculations */
    if (GET_SKILL(ch, SKILL_TRUELIGHT) && (GET_SKILL(ch, SKILL_TRUELIGHT) > number(0, 100)))
      gain += 3;
    if (GET_BONDED(ch) && !(GET_BONDED(ch) & (1 << 30))) {
      gain += 3;
    }
    if (GET_SKILL(ch, SKILL_BANDHEAL) && IS_AFFECTED(ch, AFF_GROUP))
    {
      if (ch->master) 
        leader = ch->master;
      else leader = ch;
      for (f = leader->followers; f; f = f->next)
        if (GET_SKILL(f->follower, SKILL_BANDHEAL) && (f->follower != ch))
          grouped = 1;
      if (grouped)
         gain *= 1.3;
    }   
    /* Position calculations    */
    if (NORMAL_DREAMER(ch))
      gain <<= 1;
    else
    switch (GET_POS(ch)) {
    case POS_SLEEPING:
      gain += gain;
      break;
    case POS_RESTING:
      gain += (gain >> 2);	/* Divide by 4 */
      break;
    case POS_SITTING:
      gain += (gain >> 3);	/* Divide by 8 */
      break;
    case POS_FIGHTING:
      gain = gain/2;
      break;
    }
  }

  if ((GET_COND(ch, FULL) == 0) || (GET_COND(ch, THIRST) == 0))
    gain >>= 2;

  gain = MAX(1,gain);

  if (IS_AFFECTED(ch, AFF_POISON))
    gain = 0;

  if (NOWHERE != ch->in_room) {
    for (ppl = world[ch->in_room].people; ppl; ppl = ppl->next_in_room) 
      if (!IS_NPC(ch) && affected_by_spell(ppl, SKILL_CAMP))
        gain = (int) (1.25*gain);

    if (room_affected_by_spell(&world[ch->in_room], SPELL_SANCTUARY))
      gain = (int) gain * 1.5;
  }

  if (IS_AFFECTED(ch, AFF_SUFFOCATING)) {
    act("$n looks all blue in the face, something is suffocating $m!", TRUE, ch, 0, 0, TO_ROOM);
    send_to_char("You're slowly dying from lack of air!\r\n", ch);
    gain = -5;
  }
  if (NOWHERE != ch->in_room) {
    if (ROOM_AFFECTED(ch->in_room, RAFF_COLD)) {
      send_to_char("\\c02It's so cold here it hurts!\\c00\r\n", ch);
      act("$n shivers in the cold.", TRUE, ch, 0, 0, TO_ROOM);
      gain = -10;
    }
  }
  if (GET_OGIER(ch) && (LONGING_NOGAIN >= GET_LONGING(ch)))
    gain /= 2;
    
  gain = MAX(0,gain);

  return (gain);
}



int move_gain(struct char_data * ch)
/* move gain pr. game hour */
{
  int gain, grouped = 0;
  struct follow_type *f = NULL;
  struct char_data *leader, *ppl;

  if (IS_NPC(ch)) {
    return (1 + GET_LEVEL(ch)/25);
    /* Neat and fast */
  } else {
    
    gain = 1 + (GET_MAX_MOVE(ch)/16 + GET_DEX(ch))/25;

    /* Class/Level calculations */

    /* Skill/Spell calculations */
    if (GET_SKILL(ch, SKILL_TRUELIGHT) && (GET_SKILL(ch, SKILL_TRUELIGHT) > number(0, 100)))
      gain += 3;   
    if (GET_BONDED(ch) && !(GET_BONDED(ch) & (1 << 30)))
      gain += 3;
    if (GET_SKILL(ch, SKILL_BANDHEAL) && IS_AFFECTED(ch, AFF_GROUP))
    {
      if (ch->master) 
        leader = ch->master;
      else leader = ch;
      for (f = leader->followers; f; f = f->next)
        if (GET_SKILL(f->follower, SKILL_BANDHEAL) && (f->follower != ch))
          grouped = 1;
      if (grouped)
         gain *= 1.3;
    }   
    /* Position calculations    */
    if (NORMAL_DREAMER(ch))
      gain += (gain >> 1);
    else
    switch (GET_POS(ch)) {
    case POS_SLEEPING:
      gain += gain;	
      break;
    case POS_RESTING:
      gain += (gain >> 2);	/* Divide by 4 */
      break;
    case POS_SITTING:
      gain += (gain >> 3);	/* Divide by 8 */
      break;
    case POS_FIGHTING:
      gain = gain/2;
      break;      
    }
  }

  if ((GET_COND(ch, FULL) == 0) || (GET_COND(ch, THIRST) == 0))
    gain >>= 2;

  gain = MAX(1,gain);

  if (NOWHERE != ch->in_room) {
    if (IS_AFFECTED(ch, AFF_POISON) || ROOM_AFFECTED(ch->in_room,RAFF_COLD))
      gain = 0;
  }

  if (NOWHERE != ch->in_room) {
    for (ppl = world[ch->in_room].people; ppl; ppl = ppl->next_in_room) 
      if (affected_by_spell(ppl, SKILL_CAMP))
        gain = (int) (1.25*gain);

    if (room_affected_by_spell(&world[ch->in_room], SPELL_SANCTUARY))
      gain = (int) gain * 1.5;
  }

  if (GET_OGIER(ch) && (LONGING_NOGAIN >= GET_LONGING(ch)))
    gain /= 2;

  gain = MAX(0,gain);
   
  return (gain);
}

void exit_cart(struct char_data *ch)
{
  act("A cart stops just outside the room.", FALSE, ch, 0, 0, TO_ROOM);
  act("$n steps out of the cart.", TRUE, ch, 0, 0, TO_ROOM);

  if ((ch->master) && (GET_DEST(ch) == GET_DEST(ch->master))){
    sprintf(buf, "The cart stops and you exit it, right after %s.\r\n", GET_NAME(ch->master));
    send_to_char(buf, ch);
  }
  else{
     send_to_char("The cart stops, and you exit it.\r\n", ch);
  }
  GET_DEST(ch) = 0;
  do_look(ch,"",0,0);
}

void move_cart(struct char_data *ch)
{
  struct char_data *people = NULL;
  int dir, room = 0;

  dir = b_find_first_step(ch->in_room, real_room(GET_DEST(ch)));
  if ((dir == BFS_NO_PATH) || (dir == BFS_ERROR)) {
    send_to_char("The driver looks really embarrassed as he confesses he doesn't know the way.\r\n", ch);
    send_to_char("He gives you the money back and leaves hurriedly.\r\n", ch);
    GET_GOLD(ch) += 200;
    GET_DEST(ch) = 0;
    do_look(ch,"",0,0);
    return;
  }

  room = EXIT(ch, dir)->to_room;
  if (ROOM_AFFECTED(room,RAFF_DOMEOFAIR) || ROOM_AFFECTED(room,RAFF_ROOMSEAL)) {
    send_to_char("The cart bumbs into an invisible wall and stops dead in its tracks.\r\n",ch);
    send_to_char("The driver looks both scared and embarrassed as he pushes you out of\r\n",ch);
    send_to_char("the wagon with your luggage and hands you your money back before\r\n",ch);
    send_to_char("leaving hurriedly.\r\n", ch);
    GET_GOLD(ch) += 200;
    GET_DEST(ch) = 0;
    do_look(ch,"",0,0);
    return;
  }

  char_from_room(ch);
  char_to_room(ch, room);
  if (!((ch->master) && (GET_DEST(ch) == GET_DEST(ch->master))))
    for (people = world[ch->in_room].people; people; people = people->next_in_room)
      // Only send the rumbling message to OOC players not riding a cart.
      if (!GET_DEST(people) && !IS_NPC(people) && !IS_SET(PRF_FLAGS(people),PRF_IC))
        send_to_char("A cart rumbles by.\r\n", people);
}

void move_player(struct char_data *ch) 
{
  int dir;

  if (!GET_TRAVEL_TO(ch))
    return;

  dir = b_find_first_step(ch->in_room, GET_TRAVEL_TO(ch));
  if ((dir == BFS_NO_PATH) || (dir == BFS_ERROR)) {
    send_to_char("Hm. There seems to be no way to that place.\r\n", ch);
    GET_TRAVEL_TO(ch) = 0;
    do_look(ch,"",0,0);
    return;
  }
  if (GET_MOVE(ch) <= 1) {
    send_to_char("You're to exhausted to continue!\r\n",ch);
    GET_TRAVEL_TO(ch) = 0;
    do_look(ch,"",0,0);
    return;
  }
  if (IS_SET(EXIT(ch, dir)->exit_info, EX_CLOSED) && !IS_GHOLAM(ch)) {
    send_to_char("Seems to be a closed door in your way! Better open it.\r\n",ch);
    GET_TRAVEL_TO(ch) = 0;
    do_look(ch,"",0,0);
    return;
  }


  if (ROOM_AFFECTED(EXIT(ch, dir)->to_room,RAFF_DOMEOFAIR) ||
  ROOM_AFFECTED(EXIT(ch, dir)->to_room, RAFF_ROOMSEAL)) {
    send_to_char("You suddenly bumbs into an invisible wall obstructing further movement!\r\n",ch);
    send_to_char("It seems you must find another way around...\r\n",ch);
    GET_DEST(ch) = 0;
    do_look(ch,"",0,0);
    return;
  }

  perform_move(ch, dir, 0);
}

void write_whowebpage()
{
  FILE *f;
  struct descriptor_data *d;
  struct char_data *ch;
  char title[5000];
  int num = 0, i,k;
  time_t now;

  if (4000 != port_used)
    return;

  if (!(f = fopen("/home/fang/public_html/who.html", "w+"))) {
    xlog(SYS_GOD, LVL_GRGOD, "SYSERR: Can't write who.html", TRUE);
    return;
  }

  if (f) {
    fprintf(f, "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 3.2//EN\">\r\n");
    fprintf(f, "<HTML>\r\n<HEAD>\r\n");
    fprintf(f, "<TITLE>Who list for Dragon's Fang</TITLE>\r\n");
    fprintf(f, "</HEAD>\r\n");
    fprintf(f, "<BODY TEXT=\"#CCCC98\" BGCOLOR=\"#000000\" LINK=\"#CCCC98\" ALINK=\"#CCCC98\" VLINK=\"#CCCC98\">\r\n");
    fprintf(f, "<FONT FACE=\"Arial\">\r\n");
    fprintf(f, "<TABLE WIDTH=\"90%%\" Align=\"right\">\r\n<TR>\r\n<TD>\r\n");
    fprintf(f, "<BR><BR><BR>\r\n");
    fprintf(f, "<CENTER><H2>DRAGON FANG'S WHO LIST</H2></CENTER>\r\n");
    time(&now);   
    fprintf(f, "These were logged on to Dragon's Fang at <B>%s</B>.\r\n", ctime(&now));
    fprintf(f, "<HR>\r\n");
    fprintf(f, "<TABLE>\r\n<TR><TH><U>Name</U></TH><TH><U> Title</U></TH></TR>\r\n");

    for (d = descriptor_list; d ; d = d->next) {
      if (d->original)
        ch = d->original;
      else if (!(ch = d->character))
        continue;
      if (IS_NPC(ch))
        continue;
      if (GET_INVIS_LEV(ch) >= LVL_IMMORT)
        continue;
      if (IS_AFFECTED(ch, AFF_INVISIBLE))
        continue;
      num ++;
      if (GET_TITLE(ch)) {
        for (i = 0, k = 0; i < strlen(GET_TITLE(ch)); i ++) {
          if (*(GET_TITLE(ch) + i) == '\\' && *(GET_TITLE(ch) + i +1) == 'c')
            i += 4;
          else if (*(GET_TITLE(ch) + i) == '&')
            i += 2;
          if (i >= strlen(GET_TITLE(ch)))
            break;
          title[k++] = *(GET_TITLE(ch) + i);
        }
      }
      else {
        k = 0;
      }
      title[k] = '\0';
      fprintf(f, "<TR><TD>%s</TD><TD><I>%s</I></TD></TR>\r\n", GET_NAME(ch), title);
    }

    fprintf(f, "</TABLE>\r\n");
    fprintf(f, "All in all <B>%d</B> brave adventurers.\r\n", num);
    fprintf(f, "<HR>\r\n");
    fprintf(f, "</TD>\r\n</TR>\r\n</TABLE>\r\n</FONT>\r\n</BODY>\r\n</HTML>\r\n");
    fclose(f);
  }
}

void five_minute_update()
{
  struct descriptor_data *d = NULL;
  char temp[80];
  int numUnauths = 0;

#if 0
  if (4000 == port_used) {
    write_whowebpage();
  } // Ugly as hell, don't write who if we're running dev. :)
#endif



  for (d = descriptor_list; d; d = d->next) {
    struct char_data *ch = d->character;
    if (!ch || IS_NPC(ch))
      continue; 
    charge_for_retainers(ch);
    if (NORMAL_DREAMER(ch))
      generic_result(ch, SKILL_DREAM, NULL, 0);
    if (!GET_AUTHORIZED(ch) && (GET_LEVEL(ch) == 1))
      numUnauths ++;
  }
  if( find_multi( NULL, NULL, NULL, NULL ) )
    mudlog( "&RWarning:&n There are currently true multiplayers.", BRF, LVL_IMMORT, TRUE );

  if (numUnauths) {
    sprintf(temp, "Note: There are currently %d characters awaiting authorization at the moment.", numUnauths);
    mudlog(temp,BRF,LVL_IMMORT, TRUE);
  }
  save_objs_in_rooms();
}

void fast_update()    /* NOTE: This function is called twice every second.     */
{                     /* Add only such functions that really are time critical */
  struct char_data *ch, *next_ch, *tch;
  char pulsestr[10] = ".:+*+:.";
  //  struct obj_data *obj, *next_obj;
  struct affected_type *af = NULL, *af_next = NULL;
  byte block;
  struct follow_type *follow = NULL;
  int destination;

  /* Check for paused objprogs */
/*
 * Commented this out because obj progs don't work huh? :p
 *
 * for (obj = object_list, next_obj = obj->next; obj; obj = next_obj) {
 *   next_obj = obj->next;
 *   if (obj->paused_at) 
 *   {
 *     if (!(--(obj->pause)))
 *       run_single_obj_program(NULL, NULL, obj, TRUE);
 *   }
 * }
 */

  for (ch = character_list, next_ch = ch->next; ch; ch = next_ch) {
    next_ch = ch->next;
    if (IS_AFFECTED2(ch, AFF2_HALT)) {
      block = FALSE;
      if (ch != NULL && ch->affected != NULL && ch->desc != NULL) {
        for (af = ch->affected; af; af = af_next) {
          af_next = af->next;
          if (af->bitvector2 & AFF2_HALT) {
            if (af->duration > 1) {
              asend_to_char(ch, "\033[J\\c04%c\\c00", pulsestr[af->duration%6]);
              af->duration--;
            }
            else 
              affect_remove(ch, af);
          }
        }
      }
    } 
    if (IS_NPC(ch) && GET_DREAMER(ch) && (GET_POS(ch) != POS_SLEEPING ))
      wakeup_sequence(GET_DREAMER(ch));
      /* Check for paused mobprogs */
    if (IS_NPC(ch) && (ch->mob_specials.paused_at)) 
      if (!(--ch->mob_specials.pause))
        run_single_program(NULL, NULL, ch, TRUE);

      /* Check if anyone is riding a cart and/or reached destination */
    if (GET_DEST(ch)) {
      if (real_room(GET_DEST(ch)) == ch->in_room) {
    destination = GET_DEST(ch);
    exit_cart(ch);

    if (ch->followers) {
      for (follow = ch->followers; follow; follow = follow->next) {
        tch = follow->follower;
        if (tch == ch)
          continue;
        if (destination == GET_DEST(tch) && ch->in_room == tch->in_room) {
          exit_cart(tch);
          asend_to_char(tch, "The cart stops and you exit it, right after %s.\r\n", GET_NAME(ch));
        }
      }
    }
      }
      else
        move_cart(ch);
    }
      /* Tuatha'an guildskill, Travel, works a lot like Carts */
  if (GET_TRAVEL_TO(ch)) {
      if (GET_TRAVEL_TO(ch) == ch->in_room) {
        send_to_char("You've reached your destination.\r\n", ch);
        GET_TRAVEL_TO(ch) = 0;
    do_look(ch,"",0,0);
      } else
        move_player(ch);
    }
  }
}

void set_title(struct char_data * ch, char *title)
{
  if (title == NULL)
    title = "the Player";

  if (c_strlen(title) > MAX_TITLE_LENGTH) {
    title[(strlen(title) - c_strlen(title)) + MAX_TITLE_LENGTH] = '\0';
    strcat(title, "&n");
  }

  if (GET_TITLE(ch) != NULL)
    free(GET_TITLE(ch));

  SET_TITLE(ch, str_dup(title));
}


void check_autowiz(struct char_data * ch)
{
  char buf[100];
  pid_t getpid(void);

  if (use_autowiz && GET_LEVEL(ch) >= LVL_IMMORT) {
    sprintf(buf, "nice ../bin/autowiz %d %s %d %s %d &", min_wizlist_lev,
        WIZLIST_FILE, LVL_IMMORT, IMMLIST_FILE, (int) getpid());
    mudlog("Initiating autowiz.", CMP, LVL_IMMORT, FALSE);
    system(buf);
  }
}

int calc_exp(struct char_data * ch, int gain,struct char_data *vict) 
{   
  double gain_mult = 1.0;
  int max_cap = 1000;

  return MIN((int)(gain*gain_mult),max_cap);
}   

void gain_exp(struct char_data * ch, int gain,struct char_data *vict)
{
  int is_altered = FALSE;
  int num_levels = 0;
  char buf[128];
  double gain_mult = 1.0;
  int max_cap = 1000;

  if (!IS_NPC(ch) && ((GET_LEVEL(ch) < 1 || GET_LEVEL(ch) >= (LVL_IMMORT-1))))
    return;

  if (PLR_FLAGGED(ch,PLR_NOGAIN)) {
    return;
  }


  if (IS_NPC(ch)) {
    GET_EXP(ch) += gain;
    return;
  }
  if (gain > 0) {
    if (vict && !IS_NPC(vict)) {
      send_to_char("You don't gain any xp by killing other players!\r\n",ch);
      return;
    }

    if (GET_LEVEL(ch) >= (LVL_IMMORT - 1)) {
      /* Line below to prevent morts becoming immorts. */
      GET_EXP(ch) = (GET_LEVEL(ch)-1)*GET_LEVEL_EXP(ch);
      return;
    }
    GET_EXP(ch) += MIN((int)(gain*gain_mult),max_cap);

    while ((GET_EXP(ch) >= (GET_LEVEL(ch))*GET_LEVEL_EXP(ch))) {
      num_levels++;
      advance_level(ch);
      is_altered = TRUE;
    }

    if (is_altered) {
      if (num_levels == 1)
        send_to_char("You rise a level!\r\n", ch);
      else {
    sprintf(buf, "You rise %d levels!\r\n", num_levels);
    send_to_char(buf, ch);
      }
      check_autowiz(ch);
    }
  } else if (gain < 0) {
    gain = MAX(-max_exp_loss, gain);	/* Cap max exp lost per death */
    GET_EXP(ch) += gain;
    if (GET_EXP(ch) < 0)
      GET_EXP(ch) = 0;
  }
}


void gain_exp_regardless(struct char_data * ch, int gain)
{
  int is_altered = FALSE;
  int num_levels = 0;
  double gain_mult = 1.0;
  int max_cap = 1000;     


  GET_EXP(ch) += MIN((int)(gain*gain_mult),max_cap);

  if (GET_EXP(ch) < 0)
    GET_EXP(ch) = 0;

  if (PLR_FLAGGED(ch,PLR_NOGAIN))
    return;

  if (!IS_NPC(ch)) {
    while (GET_LEVEL(ch) < 200 && GET_EXP(ch) >= (GET_LEVEL(ch))*GET_LEVEL_EXP(ch)) {
      num_levels++;
      advance_level(ch);
      is_altered = TRUE;
    }

    if (is_altered) {
      if (num_levels == 1)
        send_to_char("You rise a level!\r\n", ch);
      else {
    sprintf(buf, "You rise %d levels!\r\n", num_levels);
    send_to_char(buf, ch);
      }
    }
  }
}


void gain_condition(struct char_data * ch, int condition, int value)
{
  bool intoxicated;

  if (NULL == ch) {
    alog("SYSERR: gain_condition()[limits.c]: Got NULL char");
    return;
  }

  if (GET_COND(ch, condition) == -1)	/* No change */
    return;

  intoxicated = (GET_COND(ch, DRUNK) > 0);

  GET_COND(ch, condition) += value;

  GET_COND(ch, condition) = MAX(0, GET_COND(ch, condition));
  GET_COND(ch, condition) = MIN(24, GET_COND(ch, condition));

  if (GET_COND(ch, condition) || PLR_FLAGGED(ch, PLR_WRITING))
    return;

  switch (condition) {
  case FULL:
    if (!PRF2_FLAGGED(ch, PRF2_NOHUNGER))
      send_to_char("You are hungry.\r\n", ch);
    return;
  case THIRST:
    if (!PRF2_FLAGGED(ch, PRF2_NOHUNGER))
      send_to_char("You are thirsty.\r\n", ch);
    return;
  case DRUNK:
    if (intoxicated)
      send_to_char("You are now sober.\r\n", ch);
    return;
  default:
    break;
  }

}

// Returns 1 if idling resulted in extraction, otherwise 0
int check_idling(struct char_data * ch)
{
  int file;

  if (IS_AFFECTED(ch, AFF_DREAMING))
    file = DREAM_FILE;
  else
    file = CRASH_FILE;

  if (++(ch->char_specials.timer) > 8) {
    if (GET_WAS_IN(ch) == NOWHERE && ch->in_room != NOWHERE) {
      GET_WAS_IN(ch) = ch->in_room;
      if (FIGHTING(ch)) {
	stop_fighting(FIGHTING(ch));
	stop_fighting(ch);
      }
      act("$n disappears into the void.", TRUE, ch, 0, 0, TO_ROOM);
      send_to_char("You have been idle, and are pulled into a void.\r\n", ch);
      save_char(ch, NOWHERE);
      save_XML_playerfile(ch, file);
      char_from_room(ch);
      char_to_room(ch, 1);
    } else if (ch->char_specials.timer > 48) {
      die_follower(ch, 1);
      if (ch->in_room != NOWHERE)
	char_from_room(ch);
      char_to_room(ch, GET_WAS_IN(ch));
      if (ch->desc)
	close_socket(ch->desc);
      ch->desc = NULL;
      save_XML_playerfile(ch, file);
      sprintf(buf, "%s force-rented and extracted (idle).", GET_NAME(ch));
      xlog(SYS_CONN, LVL_GOD, buf, TRUE);
      strip_char_of_all_eq(ch);      
      extract_char(ch, FALSE);
      return 1;
    }
  }
  return 0;
}
 
void drunk_behavior(struct char_data *ch)
{
  int i;
  void command_interpreter(struct char_data *ch, char *argument);
  i = number(0, 50);

  sprintf(buf, "%s",
      i ==  0 ? "attention" :
      i ==  1 ? "bounce" :
      i ==  2 ? "burp" :
      i ==  3 ? "cackle" :
      i ==  4 ? "cry" :
      i ==  5 ? "curl" :
      i ==  6 ? "daydream" :
      i ==  7 ? "drunk" :
      i ==  8 ? "frown" :
      i ==  9 ? "gibber" :
      i == 10 ? "giggle" :
      i == 11 ? "groan" :
      i == 12 ? "herring" :
      i == 13 ? "hic" :
      i == 14 ? "hop" :
      i == 15 ? "hum" :
      i == 16 ? "keg" :
      i == 17 ? "laugh" :
      i == 18 ? "moan" :
      i == 19 ? "mumble" :
      i == 20 ? "pie" :
      i == 21 ? "ponder" :
      i == 22 ? "puke" :
      i == 23 ? "ramble" :
      i == 24 ? "sheep" :
      i == 25 ? "sigh" :
      i == 26 ? "spin" :
      i == 27 ? "tackle" :
      i == 28 ? "think" :
      i == 29 ? "uga" :
      i == 30 ? "wiggle" :
      i == 31 ? "worship" :
      i == 32 ? "worthy" :
      "");

  if (strlen(buf)) {
    command_interpreter(ch, buf);
  }
}

void heartbeat_regen()
{
  struct char_data *ch;
  
  for (ch = character_list; ch; ch = ch->next) 
    if (GET_POS(ch) > POS_STUNNED) {
      GET_HIT(ch) = MIN(GET_HIT(ch) + hit_gain(ch), GET_MAX_HIT(ch));
      SET_MANA(ch, MIN(GET_MANA(ch) + mana_gain(ch), GET_MAX_MANA(ch)));
      GET_MOVE(ch) = MIN(GET_MOVE(ch) + move_gain(ch), GET_MAX_MOVE(ch));
      GET_FRACT_MOVE(ch) = 0;
    }
}

void update_char_objects(struct char_data * ch);	/* handler.c */
void extract_obj(struct obj_data * obj);	/* handler.c */

int calc_rp_gain(int tics_in_room) {
  double value = 0, mins_in_room = ((double)tics_in_room * (double)SECS_PER_MUD_HOUR * (double)60);

  if (mins_in_room < 5.0)
    return 0;

  if (mins_in_room > 120.0)
    mins_in_room = 120.0;

  value = (mins_in_room * mins_in_room);

  value /= 160.0;

  return (int)value;
}

/* Update PCs, NPCs, and objects */
void point_update(void)
{
  struct char_data *i, *next_char, *people;
  struct obj_data *j, *next_thing, *jj, *next_thing2;
  int  result, xpadd=0, people_in_the_room;
  

  /* miscellaneous */
  
  /* characters */
  for (i = character_list; NULL != i; i = next_char) 
  {
    next_char = i->next;

    if (GET_POS(i) <= POS_STUNNED)
      update_pos(i);
     else if (GET_POS(i) == POS_INCAP)
      damage(i, i, 1, TYPE_SUFFERING);
    else if (GET_POS(i) == POS_MORTALLYW)
      damage(i, i, 2, TYPE_SUFFERING);

    if (IS_NPC(i)) { // Things that -only- affects mob in here.
/* 
 *    mob timers are commented out, since they seem to give us problems
 *      /Fredrik 991005
 *     if (GET_TIMER(i) && !FIGHTING(i)) { // Don't do this while fighting
 *       GET_TIMER(i) --;
 *       if (!GET_TIMER(i)) {
 *         act("$n vanishes all of a sudden.", TRUE, i, 0, 0, TO_ROOM);
 *         extract_char(i, TRUE);
 *       }
 *     }
 */
      continue;
    } //Mob specific update code

    // Under here, only things that only affect -players-, 
    // not mobs, can be put.
    result = generic_result_mod(i,SKILL_ENDURE,NULL,FALSE,(GET_CON(i)-30));
    if (((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) &&
        (number(0, 1) == 1))
      {	
      gain_condition(i, FULL, -1);
      gain_condition(i, DRUNK, -1);
      gain_condition(i, THIRST, -1);
    }

    GET_R_IDLE(i) ++; 	
    GET_LAST_EMOTE(i) ++; // every tick, increase the time since last emote|say.
    TICKS_IN_ROOM(i)++;   // every tick, increase how many ticks player has been in room.
   /*
    * For knowledge.
    */
    if ((NOWHERE != i->in_room) && (TICKS_IN_ROOM(i) >= 4)) {
      if (!test_bit(GET_KNOW(i),world[i->in_room].number)) {
    set_bit(GET_KNOW(i),world[i->in_room].number);
        if ((GET_LEVEL(i) < LVL_IMMORT ) /* && !IS_SET(PRF_FLAGS(i), PRF_NOSPAM) */ )
           act("You feel that you know this location really well now.\r\n", TRUE, i, 0, 0, TO_CHAR);
      }
    }

    if( TICKS_IN_ROOM(i)>3 && GET_LAST_EMOTE(i)>3)
       TICKS_IN_ROOM(i)=1;

    if (GET_NO_LOCATE(i) > 0)
      GET_NO_LOCATE(i)--;
    if (GET_NO_LOCATE(i)<0 || GET_NO_LOCATE(i)>2)
      GET_NO_LOCATE(i)=0;

/* RP Exps code here. */
   if ((i->in_room != NOWHERE) && (TICKS_IN_ROOM(i)>2) &&
      (GET_LAST_EMOTE(i)<=4)  && IS_SET(PRF_FLAGS(i),PRF_IC) &&
      (GET_LEVEL(i)<LVL_CARPENTER-2) && !PLR_FLAGGED(i, PLR_NOGAIN)) 
      {
      for (people = world[i->in_room].people, people_in_the_room=0; people; people = people->next_in_room)
         if(!IS_NPC(people) && TICKS_IN_ROOM(people)>2 && GET_LAST_EMOTE(people)<10 && IS_SET(PRF_FLAGS(people),PRF_IC))
            people_in_the_room++;

      // If there's someone else in the room as well filling the requirements
      if (people_in_the_room>=1)
         {
         xpadd=1; // Starting value 
         if ((NULL != GET_DOING(i)) && strcmp(GET_DOING(i)," is standing here."))
            xpadd++;
         if (GET_MAX_HIT(i)==GET_HIT(i) && GET_MAX_MANA(i)==GET_MANA(i) && GET_MOVE(i)==GET_MAX_MOVE(i))
            xpadd++;
         if (people_in_the_room>2)
        xpadd += (people_in_the_room*2);
             

     xpadd += calc_rp_gain(TICKS_IN_ROOM(i));
     GET_EXP(i) += xpadd;
         xpadd=0; // Make sure no weird stuff take place.
         while ( (GET_EXP(i) >= (GET_LEVEL(i))*GET_LEVEL_EXP(i))) 
            {
            advance_level(i);
            send_to_char("You rise a level through role playing!!\r\n", i);
            } 
        }
    }// rp exp code

    update_char_objects(i);
    if (GET_LEVEL(i) < LVL_IMMORT)
      if (check_idling(i)) // Check idling can -extract- the char.
        continue;
    if (!IS_NPC(i))
       if (GET_NOHIDE(i))
         GET_NOHIDE(i) --;
    if (IS_PLAYING(i))
      IS_PLAYING(i) --;
    if (RECRUIT_WAIT(i))
      RECRUIT_WAIT(i)--;
    if (IS_BRANDED(i))
      IS_BRANDED(i) --;
    if (IS_AFFECTED(i, AFF_POISON)) {
      act("$n looks really sick, all green in the face..", TRUE, i, 0, 0, TO_ROOM);
      send_to_char("You feel the venom in your veins burning like fire.\r\n", i);
    }
    if (GET_COND(i, DRUNK) > 15) /* Random social for drunk people */
      drunk_behavior(i);
  } // Player and mob update

  /* objects */
  for (j = object_list; j; j = next_thing) {
    next_thing = j->next;	/* Next in object list */

    /* If this is a corpse */
    if ((GET_OBJ_TYPE(j) == ITEM_CONTAINER) && GET_OBJ_VAL(j, 3)) {
      /* timer count down */
      if (GET_OBJ_TIMER(j) > 0)
    GET_OBJ_TIMER(j)--;

      if (GET_OBJ_TIMER(j) < 1) {
    if (j->carried_by)
      act("$p decays in your hands.", FALSE, j->carried_by, j, 0, TO_CHAR);
    else if ((j->in_room != NOWHERE) && (world[j->in_room].people)) {
      act("A quivering horde of maggots consumes $p.",
          TRUE, world[j->in_room].people, j, 0, TO_ROOM);
      act("A quivering horde of maggots consumes $p.",
          TRUE, world[j->in_room].people, j, 0, TO_CHAR);
    }
    for (jj = j->contains; jj; jj = next_thing2) {
      next_thing2 = jj->next_content;	/* Next in inventory */
      obj_from_obj(jj);

      if (j->in_obj)
        obj_to_obj(jj, j->in_obj);
      else if (j->carried_by)
        obj_to_room(jj, j->carried_by->in_room);
      else if (j->in_room != NOWHERE)
        obj_to_room(jj, j->in_room);
      else
        assert(FALSE);
    }
    extract_obj(j);
        continue;
      }
    } // Corpse extraction
    if ((GET_OBJ_TYPE(j) == ITEM_FOUNTAIN) && GET_OBJ_TIMER(j)){
      GET_OBJ_TIMER(j) --;
      if (!GET_OBJ_TIMER(j)) 
      {
        if (j->in_room) /* Just in case some idiot has picked up a well, even though it's not possible */
          send_to_room("A small well slowly dries out and disappears, leaving a soggy ground.\r\n", j->in_room);
        extract_obj(j);
        continue;
      }
    }
    
    if (((GET_OBJ_TYPE(j) == ITEM_WEAPON) && (j->item_number == -1) && (GET_OBJ_VAL(j,3) == 3)) || 
       ((GET_OBJ_TYPE(j) == ITEM_ARMOR) && (j->item_number == -1))) { /* Flamesword & Earthshield */
           /* All worn and carried items already have their timers reduced
              in update_char_objects()...Must be item on ground to reducde
              here....  /Fredrik 970719                                    */
      if ((GET_OBJ_TIMER(j) > 0) && !(j->carried_by) && !(j->worn_by))
        GET_OBJ_TIMER(j)--;

      if (GET_OBJ_TIMER(j) <= 0) 
      {
    if (j->carried_by) 
      act("$p disappears as you can't keep your flows going.",FALSE,j->carried_by,j,NULL,TO_CHAR);
   
        if (j->worn_by)
          act("$p dissappears as you can't keep your flows going.",FALSE,j->worn_by,j,NULL,TO_CHAR);
          
        if ((j->in_room != NOWHERE) && (world[j->in_room].people)) 
        {
      act("$p suddenly disappears. Strange.",TRUE, world[j->in_room].people, j, 0, TO_ROOM);
      act("$p suddenly disappears.",TRUE,j->carried_by,NULL,j,TO_CHAR);
    }
    extract_obj(j);
        continue;
      }
    }
 
    if (39 == GET_OBJ_VNUM(j) || 43 == GET_OBJ_VNUM(j)) {
      if (GET_OBJ_TIMER(j) > 0)
        GET_OBJ_TIMER(j)--;
      mlog("Skim gate found!");
      if (GET_OBJ_TIMER(j) <= 0) {
        long pp = GET_OBJ_VAL(j,1); 
        act("A gateway disappears in a flash of light!",TRUE,NULL,j,NULL,TO_ROOM);
        extract_obj(j);
        fix_skim_gates(NULL,pp);
        continue;
      }
    }

    if ((GET_OBJ_VAL(j,0) == 4) && isname("gate",j->name)) { /* gate! */
      if (GET_OBJ_TIMER(j) > 0)
        GET_OBJ_TIMER(j)--;

      if (GET_OBJ_TIMER(j) <= 0) {
        act("A gateway disappears in a flash of light!",TRUE,world[j->in_room].people,NULL,NULL,TO_ROOM);
        extract_obj(j);
        continue;
      }
      if (isname("a glowing ball", j->short_description)) {
        if (GET_OBJ_TIMER(j))
          GET_OBJ_TIMER(j) --;
        else {
          if(j->carried_by || j->worn_by) {
            act("The glowing orb $n holds vanishes all of a sudden.", TRUE, j->carried_by?j->carried_by: j->worn_by, 0, 0, TO_ROOM);
            send_to_char("Your orb of light vanishes vithout a trace.\r\n", j->carried_by?j->carried_by: j->worn_by);
          } else if (j->in_room)            
              send_to_room("The glowing orb suddenly disappears without a trace.\r\n", j->in_room);
          extract_obj(j);
          continue;
        }
      }
    }
    else if (GET_OBJ_TIMER(j)>0) {
      GET_OBJ_TIMER(j)--;
      if (!GET_OBJ_TIMER(j))
        timer_otrigger(j);
    }
  }
}



