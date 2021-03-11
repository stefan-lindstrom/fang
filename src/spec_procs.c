/******************************************************************************
 * $Id: spec_procs.c,v 1.10 2002/11/07 04:02:04 fang Exp $
 *   
 * File: spec_procs.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: spec_procs.c,v $
 *   Revision 1.10  2002/11/07 04:02:04  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.9  2002/09/13 01:43:16  fang
 *   Drath: New boards added for Houses project
 *
 *   Revision 1.8  2002/09/04 15:06:06  fang
 *   Drath: Trimmed skill/weave names
 *
 *   Revision 1.7  2002/08/30 20:46:28  fang
 *   Drath: Oops, broke GATE.  All better now.
 *
 *   Revision 1.6  2002/08/30 19:52:28  fang
 *   Drath: New room DG trigger, LEAVE
 *
 *   Revision 1.5  2002/08/08 04:04:02  fang
 *   Drath: Added auto save to WIRE so a crash wont give money back.
 *
 *   Revision 1.4  2002/07/31 04:02:48  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/07/31 03:40:04  fang
 *   Drath: WIRE, gold transfer for users
 *
 *   Revision 1.2  2002/07/28 13:56:05  fang
 *   Removed dmlloc...just to test.
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.7  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.6  2002/03/21 02:10:53  fang
 *   *** empty log message ***
 *
 *   Revision 1.5  2002/03/13 12:56:28  fang
 *   TicksInRoom reset when entering gates/tar-gates.
 *
 *   Revision 1.4  2002/03/08 09:50:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/03/07 08:02:49  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.27  2002/01/30 14:37:46  mud
 *   Mino5r bugfixes. Wagons now pulled through gates. Notifylist expanded to 10 names, etc..
 *
 *   Revision 1.26  2002/01/24 21:33:20  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.25.2.4  2002/01/24 20:49:37  mud
 *   More cleanup. Removed files:  specialday.c string.c string.h
 *   Removed DONT_TRADE_WITH-stuff from shops.
 *
 *   Revision 1.25.2.3  2002/01/23 21:49:42  mud
 *   Removed objsave.[c|h] (old objsave stuff).
 *   Removed obj_file_elem and rent structs.
 *   Removed final vestiges of old guild system, except parts used
 *   in guild.[c|h] for backwards compatibility.
 *
 *   Revision 1.25.2.2  2002/01/22 20:50:59  mud
 *
 *
 *   Removed boats and info system. Obsolete, unused code with no value.
 *   Fixed bug in portal_stone SPECIAL. Now checks that a room exists
 *   before attempting to transf chars there. :)
 *
 *   Revision 1.25.2.1  2002/01/18 02:24:52  mud
 *   More cleanup...
 *
 *   Revision 1.25  2001/12/15 12:44:49  mud
 *   Removed "train" command.
 *
 *   Revision 1.24  2001/11/06 09:13:59  mud
 *   TaR-gate and gate now also flushes followers through when leader enters
 *
 *   Revision 1.23  2001/10/14 05:22:47  mud
 *   *** empty log message ***
 *
 *   Revision 1.22  2001/10/10 02:36:46  mud
 *   *** empty log message ***
 *
 *   Revision 1.21  2001/09/28 13:59:27  mud
 *   *** empty log message ***
 *
 *   Revision 1.20  2001/08/31 12:57:50  mud
 *   *** empty log message ***
 *
 *   Revision 1.19  2001/08/20 13:49:47  mud
 *   *** empty log message ***
 *
 *   Revision 1.18  2001/08/20 11:21:50  mud
 *   Fixed gate closing when weaver enters.
 *
 *   Revision 1.17  2001/08/11 11:43:39  mud
 *   *** empty log message ***
 *
 *   Revision 1.16  2001/08/10 02:11:43  mud
 *   *** empty log message ***
 *
 *   Revision 1.15  2001/03/28 22:06:19  mud
 *   *** empty log message ***
 *
 *   Revision 1.14  2001/03/24 19:54:27  mud
 *   *** empty log message ***
 *
 *   Revision 1.13  2001/03/24 03:25:46  mud
 *   *** empty log message ***
 *
 *   Revision 1.12  2001/03/22 16:37:07  mud
 *   *** empty log message ***
 *
 *   Revision 1.11  2001/03/17 10:40:47  mud
 *   New and Improved TAR.
 *
 *   Revision 1.10  2000/12/11 12:14:18  mud
 *   Removed most references to PFILEPOS() get_char_ppos(), since with the xml code ppos will(may) change every reboot
 *
 *   Revision 1.9  2000/12/10 09:32:13  mud
 *   Fixed train overflow when raising mana/move/hp
 *
 *   Revision 1.8  2000/12/01 07:23:11  mud
 *   *** empty log message ***
 *
 *   Revision 1.7  2000/11/25 23:29:01  mud
 *   *** empty log message ***
 *
 *   Revision 1.6  2000/08/03 19:28:43  mud
 *
 *   Patches for talent, and added talent requirements for skills/spells
 *
 *   Revision 1.5  2000/08/01 20:43:43  mud
 *   fixed up trainer spec proc (didn't check for valid commands or for NPCs)
 *
 *   Revision 1.4  2000/08/01 11:51:00  mud
 *   added roomset command. sets roomflags that won't save
 *
 *   Revision 1.3  2000/05/24 08:41:13  mud
 *   Aiel endurance changes
 *
 *   Revision 1.2  2000/02/21 14:00:19  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/*************************************************************************
*   File: spec_procs.c                                  Part of CircleMUD *
*  Usage: implementation of special procedures for mobiles/objects/rooms  *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */
#include "config.h"
#include "class.h"
#include "conf.h"
#include "sysdep.h"
#include "fight.h"
#include "act.h"
#include "spec_procs.h"
#include "act.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "spells.h"
#include "limits.h"
#include "modify.h"
#include "guild.h"
#include "xmlcharsave.h"
#include "dg_scripts.h"
#include "mail.h"

#define PORTAL_LIST        800
#define PORTAL_TRANSF_GM   801
#define PORTAL_TRANSF_SELF 802

struct social_type {
  char *cmd;
  long next_line;
};

struct portal_stones {
  room_num  in_room;	     /* If your in this room, your standing besides THIS stone    */
  char      *name;	     /* The name of this Room, i.e. not the real roomname.        */
  long 	    linked_with[20]; /* This is the locations you can travel t			  */
};

#define MAX_STONES 10

struct portal_stones stones[] = {
  {  3292,"Thick Forest",{1,2,3,4,5,6,7,8,9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}},
  {  4928,"Large mountain",{0,2,3,4,5,6,7,8,9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}},
  {  7031,"A short hill",{0,1,3,4,5,6,7,8,9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}}, 
  {  7118,"Foothills", {0,1,2,4,5,6,7,8,9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}},
  {  8939,"Dry forest", {0,1,2,3,5,6,7,8,9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}},
  {  9411,"Bustling city",{0,1,2,3,4,6,7,8,9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}},
  { 17320,"A small clearing",{0,1,2,3,4,5,7,8,9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}},
  { 23335,"Road in sight",{0,1,2,3,4,5,6,8,9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}},
  { 24231,"Intersection",{0,1,2,3,4,5,6,7,9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}},
  { 31212,"Dead bushes",{0,1,2,3,4,5,6,7,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}},
};


/* ********************************************************************
*  Special procedures for mobiles                                     *
******************************************************************** */

int spell_sort_info[MAX_SKILLS+1];
int skill_sort_info[MAX_SKILLS+1];

byte horse_timer = 5;
byte race_is_on = 0;

void sort_spells(void)
{
  int a, b, tmp;

  /* initialize array */
  for (a = 0; a <= MAX_SKILLS; a++)
    spell_sort_info[a] = a;
  for (a = 0; a <= MAX_SKILLS; a++)
    skill_sort_info[a] = a;


  /* Sort.  'a' starts at 1, not 0, to remove 'RESERVED' */
  
  for (a = 1; a < MAX_SKILLS - 1; a++)
    for (b = a + 1; b < MAX_SKILLS; b++)
      if (strcmp(spells[spell_sort_info[a]], spells[spell_sort_info[b]]) > 0) {
    tmp = spell_sort_info[a];
    spell_sort_info[a] = spell_sort_info[b];
    spell_sort_info[b] = tmp;
      }
}

#define LEARNED_LEVEL	0	/* % known which is considered "learned" */
#define MAX_PER_PRAC	1	/* max percent gain in skill per practice */
#define MIN_PER_PRAC	2	/* min percent gain in skill per practice */
#define PRAC_TYPE	3	/* should it say 'spell' or 'skill'?	 */

/* actual prac_params are in class.c */

#define LEARNED(ch) (prac_params[LEARNED_LEVEL][(int)GET_CLASS(ch)])
#define MINGAIN(ch) (prac_params[MIN_PER_PRAC][(int)GET_CLASS(ch)])
#define MAXGAIN(ch) (prac_params[MAX_PER_PRAC][(int)GET_CLASS(ch)])
#define SINFO(num)  (spell_info[num])
#define STYPE(num)  (spell_info[num].type)
#define SLVL(num,ch) (spell_info[num].min_level[(int)GET_CLASS(ch)][0])
#define ISSKL(num,ch) (spell_info[num].min_level[(int)GET_CLASS(ch)][1])

void list_skills(struct char_data * ch, struct char_data *vict)
{
  int i, sortpos,j=1,k=1;
  int skills[MAX_SKILLS+1],spell[MAX_SKILLS+1];

  for (i = 0; i < MAX_SKILLS; i++)
    skills[i] = spell[i] = -1; 
   
  for (sortpos = 1; sortpos < MAX_SKILLS; sortpos++) {
    i = spell_sort_info[sortpos];
    if ((i < 0) || (i >= MAX_SKILLS)) {
      alog("Weird spellnumber in sortpos for %s [%d,%d]",GET_NAME(ch),i, sortpos);
      asend_to_char(ch,"Sorry %s, seems to be a problem with spells/skills rignt now.\r\n",GET_NAME(ch));
      return;
    }

    if (strcmp(spells[i],"!UNUSED!") && (ISSKL(i,vict) == -1)) {
      skills[j] = i;
      j++;
    }
    else if (strcmp(spells[i],"!UNUSED!") && (ISSKL(i,vict) != -1)) {
      spell[k] = i;
      k++;
    }
  }	    

  sprintf(buf, "You know of the following skills:\r\n");
  sprintf(buf, "%s~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n\r\n",buf);
  j = 1;
  for (i = 1; skills[i] != -1; i++) { 
    if (strlen(buf) >= MAX_STRING_LENGTH - 32) {
      strcat(buf, "**OVERFLOW**\r\n");
      break;
    }
    if (!(j%4)) {
      sprintf(buf, "%s\r\n", buf);
      j ++;
    }
    if (GET_SKILL(vict,skills[i])) {
      j ++;   
      sprintf(buf, "%s%-16s %3d%%   ", buf, spells[skills[i]], GET_SKILL(vict, skills[i]));   
    }
  }
  
  if (GET_TEACHABLE(vict)||GET_SPARK(vict)||(GET_CLASS(vict)==CLASS_WILDER)||
     (GET_CLASS(vict)==CLASS_CHANNELER)) {
    sprintf(buf,"%s\r\n\r\nAnd you know the following weaves:\r\n", buf);
    sprintf(buf,"%s~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n\r\n",buf);
    j = 1;
    for (i = 1; spell[i] != -1; i++) {     
      if (strlen(buf) >= MAX_STRING_LENGTH - 32) {
        strcat(buf, "**OVERFLOW**\r\n");
        break;
      }
      if (!(j%4)) {
        j ++;
        sprintf(buf, "%s\r\n", buf);
      }
      if (GET_SKILL(vict,spell[i])> 0) {
        j ++;
        sprintf(buf , "%s%-16s %3d%%   ", buf, spells[spell[i]], GET_SKILL(vict, spell[i]));  
      }          
    }
  }   
  page_string(ch->desc, buf, 1);
}


void gain(struct char_data *ch)
{
  int i, sortpos,j=1,k=1;
  int skills[MAX_SKILLS+1],spell[MAX_SKILLS+1];
  char t_buf[300];
  
  memset(t_buf,'\0',200);
    
  for (i = 0; i < MAX_SKILLS; i++)
    skills[i] = spell[i] = -1; 
   
  for (sortpos = 1; sortpos < MAX_SKILLS; sortpos++) {
    i = spell_sort_info[sortpos];
    if ((i >= MAX_SKILLS) || (i < 0) || (spells[i] == NULL))
      continue;

    if (strcmp(spells[i],"!UNUSED!") && (SINFO(i).type == SKILL_NORMAL)) {
      skills[j] = i;
      j++;
    }
    else if (strcmp(spells[i],"!UNUSED!") && (SINFO(i).type == SPELL_NORMAL)) {
      spell[k] = i;
      k++;
    }
  }	    
  sprintf(buf2, "You can learn the following skills:\r\n");
  sprintf(buf2, "%s~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n\r\n",buf2);
  
  k = 1;
  
  for (i = 1;skills[i] != -1; i++) { 
    if (strlen(buf2) >= MAX_STRING_LENGTH - 32) {
      strcat(buf2, "**OVERFLOW**\r\n");  
      break;
    }
    if (!(k%4)) {
      k ++;
      strcat(buf2, "\r\n");
    }
    if (!GET_SKILL(ch,skills[i]) && (spell_info[skills[i]].min_level[(int) GET_CLASS(ch)][0] <= GET_LEVEL(ch)) && (SINFO(skills[i]).cost[(int)GET_CLASS(ch)] < 200)) {
      sprintf(buf2, "%s%-15s %2ld  ", buf2, spells[skills[i]],SINFO(skills[i]).cost[(int)GET_CLASS(ch)]);
      k++;    
    }    
  }
  strcat(buf2, "\r\n");
  
  if (GET_TEACHABLE(ch)||GET_SPARK(ch)||(GET_CLASS(ch)==CLASS_WILDER)||(GET_CLASS(ch)==CLASS_CHANNELER)) {
    sprintf(buf2,"%s\r\n\r\nAnd you can learn the following weaves:\r\n", buf2);
    sprintf(buf2,"%s~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n\r\n",buf2);
    
    k = 1;
    for (i = 1; spell[i] != -1; i++) {
 
      if (strlen(buf2) >= MAX_STRING_LENGTH - 32) {
        strcat(buf2, "**OVERFLOW**\r\n");  
        break;
      }
      if (!(k%4)) {
        k++;
        strcat(buf2, "\r\n");
      }
      if (!GET_SKILL(ch,spell[i]) && (check_weaves(spell[i],ch)) && (200 > SINFO(spell[i]).cost[(int)GET_CLASS(ch)])) {
        sprintf(buf2, "%s%-15s %2ld  ", buf2, spells[spell[i]],SINFO(spell[i]).cost[(int)GET_CLASS(ch)]);
        k++;
      }
    } 
  }    
  page_string(ch->desc, buf2, 1);
}


int check_weaves(int i,struct char_data *ch)
{
  int rc = FALSE;

  rc = (   (GET_AIR(ch) >= spell_info[i].min_level[(int) GET_CLASS(ch)][AIR]      ) 
        && (GET_FIRE(ch) >= spell_info[i].min_level[(int) GET_CLASS(ch)][FIRE]    )
        && (GET_SPIRIT(ch) >= spell_info[i].min_level[(int) GET_CLASS(ch)][SPIRIT]) 
        && (GET_WATER(ch)  >= spell_info[i].min_level[(int) GET_CLASS(ch)][WATER] )
        && (GET_EARTH(ch)  >= spell_info[i].min_level[(int) GET_CLASS(ch)][EARTH] ));
  
  return rc;
}

/*
 * Evil thing. Once you picked up an object with this special, as soon as you drop it, 
 * junk it, (anything to get rid of it from you're inventory, it'll teleport to caemlyn
 * plaza, explode, and do -much- damage to anyone in the room. :)
 */
SPECIAL(bomb)
{
  char *cmds[] = { "donate", "drop", "junk", "give", "eat", "throw", NULL };
  int centre = 10186,i, r = 0,or;
  struct obj_data *bomb = me, *target = NULL;
  char arg1[1024];

  one_argument(argument,arg1);

  if (bomb->carried_by != ch) // Make sure we only trigger if <ch> tries to get rid of us. 
    return 0;
 
  for (i = 0; cmds[i] != NULL; i++) {
    r = r || CMD_IS(cmds[i]);
  }  
  if (!r) { // None of the cmds we were interested in
    return 0;
  }
  target = get_obj_in_list_vis(ch,arg1,ch->carrying);

  if ((NULL != target && target == me) || !strcasecmp(arg1,"all")) {
    send_to_char("You light the fuse on the bomb, throwing it away into a wormhole, before ducking.\r\n",ch);
    act("$n lits the fuse on a bomb, before sloppily tossing it into a wormhole...Wonder where it'll end up?",
    FALSE,ch,NULL,NULL,TO_NOTCHAR);

    // Now place it into oval plaza *EG*
    obj_from_char(bomb);
    obj_to_room(bomb,real_room(centre));

    asend_to_room(real_room(centre),"A wormhole suddenly appears, and %s tosses a lit bomb out of it!\r\n",GET_NAME(ch));
    send_to_room("The bomb explodes with an ear-splitting bang, right in your face!\r\n",real_room(centre));
    or = ch->in_room;
    char_from_room(ch);
    char_to_room(ch,real_room(centre));

    if (IS_SET(ROOM_FLAGS(ch->in_room),ROOM_PEACEFUL)) {
      TOGGLE_BIT(ROOM_FLAGS(ch->in_room),ROOM_PEACEFUL);
      do_area_spell(ch,30000,5000);
      TOGGLE_BIT(ROOM_FLAGS(ch->in_room),ROOM_PEACEFUL);
    } else if (IS_SET(ROOM_TEMP_FLAGS(ch->in_room),ROOM_PEACEFUL)) {
      TOGGLE_BIT(ROOM_TEMP_FLAGS(ch->in_room),ROOM_PEACEFUL);
      do_area_spell(ch,30000,5000);
      TOGGLE_BIT(ROOM_TEMP_FLAGS(ch->in_room),ROOM_PEACEFUL);
    } else 
      do_area_spell(ch,30000,5000);

    char_from_room(ch);
    char_to_room(ch,or);

    return 1;
  }
  return 0;
}

SPECIAL(guild)
{
  int skill_num, percent;
  
  if (IS_NPC(ch) || (!CMD_IS("gain") && !CMD_IS("practice") && !CMD_IS("tutor") 
                     && !CMD_IS("achieve")))
    return 0;
  if (PLR_FLAGGED(ch, PLR_NOGAIN)){
    send_to_char("Sorry.  Bookies and some 'special' darkfriends may not do this.\r\n", ch);
    return 1;
  }
  skip_spaces(&argument);
  

  if (CMD_IS("achieve")) {
    if (!GET_TRAINS(ch)) {
      send_to_char("You don't have any trains to convert to practices.\r\n", ch);
      return 1;
    }
// Protection against overflows...
    if (GET_PRACTICES(ch) == 0xFFFFFFFF) {
      send_to_char("You already carry as many practices as you can!\r\n",ch);
      return 1;
    }
    GET_TRAINS(ch) --;
    GET_PRACTICES(ch) +=10;
    send_to_char("1 train converted to 10 practices.\r\n", ch);
    return 1;
  }
  if (CMD_IS("tutor")) {
    if (GET_PRACTICES(ch) < 10) {
      send_to_char("You don't have 10 practices to convert.\r\n", ch);
      return 1;
    }
// Protection against overflows!
    if ((GET_TRAINS(ch) == (ubyte)0xFF)) {
      send_to_char("You already carry as many trains as you can have!\r\n",ch);
      return 1;
    }
    GET_PRACTICES(ch) -= 10;
    GET_TRAINS(ch) ++;
    send_to_char("10 Practices converted into 1 Train.\r\n", ch);
    return 1;
  }
  
  if ((!*argument) && CMD_IS("practice")) {
    list_skills(ch, ch);
    return 1;
  }
  
  if ((!*argument) && CMD_IS("gain")) {
    gain(ch);
    return 1;
  }
  
  if ((GET_PRACTICES(ch) <= 0) && CMD_IS("practice")){
    send_to_char("You do not seem to be able to practice now.\r\n", ch);
    return 1;
  }
  

  skill_num = find_skill_num(argument);

  if ((skill_num < 0) || (skill_num > MAX_SKILLS)) {
    send_to_char("You've probably misspelled the name of the skill.. Try again.\r\n",ch);
    return 1;
  }
  
  if (CMD_IS("gain")) {

   if ((SINFO(skill_num).type != SKILL_NORMAL) && (SINFO(skill_num).type != SPELL_NORMAL)) {
      send_to_char("Hey! You can't learn that here!\r\n",ch);
      return 1;
    }
 

    if ((SINFO(skill_num).type == SKILL_NORMAL) && SLVL(skill_num,ch) > GET_LEVEL(ch)) {
      sprintf(buf, "Sorry, you're not experienced enough to learn that skill.\r\n");
      send_to_char(buf,ch);
      return 1;
    }
  
    if (!check_weaves(skill_num,ch) && (SINFO(skill_num).type == SPELL_NORMAL)) {
      send_to_char("Sorry, you're not experienced enough to learn that weave.\r\n",ch);
      return 1;
    }
  
    if (GET_SKILL(ch,skill_num)) {
      send_to_char("Why gain something you already got?\r\n",ch);
      return 1;
    }
    
    if (SINFO(skill_num).cost[(int)GET_CLASS(ch)] > GET_TRAINS(ch)) {
      send_to_char("You don't have enough trains to gain that!\r\n",ch);
      return 1;
    }
    
    if(meets_skillspell_reqs(ch, skill_num)){
      GET_TRAINS(ch) -= SINFO(skill_num).cost[(int)GET_CLASS(ch)];
      SET_SKILL(ch,skill_num,1);
      if (SINFO(skill_num).type == SKILL_NORMAL)
        send_to_char("You gain a new skill!\r\n",ch);
      else
        send_to_char("You gain a new weave!\r\n",ch);
      save_char(ch,ch->in_room);
      return 1;
    }
    else{
      send_to_char("You do not have the appropriate talents or special skills to gain this skill/spell.\r\n", ch);
      return 1;
    } 
  }
  
  if (CMD_IS("practice")) {
    if (!GET_SKILL(ch,skill_num)) {
      send_to_char("You can only practice things you've gained!\r\n",ch);
      return 1;
    }
    if ((skill_num != 231) && (skill_num != 232)) /*grasp and release max at 50%, not 70%*/ {
      if (GET_SKILL(ch,skill_num) >= 70)  /*hard-coded! Can't learn more than 70%! */ {
        send_to_char("Only experience can make you better now!\r\n",ch);
        return 1;
      }
  
      send_to_char("You practice long and hard to become a master in thy art...\r\n",ch);
      GET_PRACTICES(ch)--;
      
      percent = ((30 + number(0,GET_RINT(ch)) + number(0,GET_RWIS(ch))) >> 1);
      percent += check_taveren(ch);
   
      if (percent > 70) { 
	percent = 70;
      }     
      ADD_SKILL(ch, skill_num, percent);
    
      if (GET_SKILL(ch,skill_num) > 70) {
	SET_SKILL(ch, skill_num, 70);
      }
      return 1;
    }
    else {
      if (GET_SKILL(ch,skill_num) >= 50)  /*Can't learn grasp or release more than 50%! */ {
        send_to_char("Only experience can make you better now!\r\n",ch);
        return 1;
      }
  
      send_to_char("You practice long and hard to become a master in thy art...\r\n",ch);
      GET_PRACTICES(ch)--;

      percent = ((30 + number(0,GET_RINT(ch)) + number(0,GET_RWIS(ch))) >> 1);
      percent += check_taveren(ch);
   
      if (percent > 50) {
	percent = 50;
      }
      ADD_SKILL(ch, skill_num, percent);
      
      if (GET_SKILL(ch,skill_num) > 50) {
	SET_SKILL(ch, skill_num, 50);
      }
      return 1;
    }
  }
  return 1;
}

int check_taveren(struct char_data *ch)
{
  if (GET_TAVEREN(ch)) {
    if (number(0,100) <= 5) {
      if (GET_LEVEL(ch) >= 11) {
        act("Suddenly you feel chance running wild! Is there a Ta'Veren here?",FALSE,ch,0,0,TO_CHAR);
      }
      return (number(-25,25));
    }
    else return 0;
  }
  else
    return 0;
    
  return 0;
}
  


SPECIAL(dump)
{
  struct obj_data *k;
  int value = 0;

  for (k = world[ch->in_room].contents; k; k = world[ch->in_room].contents) {
    act("$p vanishes in a puff of smoke!", FALSE, 0, k, 0, TO_ROOM);
    extract_obj(k);
  }

  if (!CMD_IS("drop"))
    return 0;

  do_drop(ch, argument, cmd, 0);

  for (k = world[ch->in_room].contents; k; k = world[ch->in_room].contents) {
    act("$p vanishes in a puff of smoke!", FALSE, 0, k, 0, TO_ROOM);
    value += MAX(1, MIN(50, GET_OBJ_COST(k) / 10));
    extract_obj(k);
  }

  if (value) {
    act("You are awarded for outstanding performance.", FALSE, ch, 0, 0, TO_CHAR);
    act("$n has been awarded for being a good citizen.", TRUE, ch, 0, 0, TO_ROOM);


    if (GET_LEVEL(ch) > 2)
      GET_GOLD(ch) += value;
  }
  return 1;
}

SPECIAL(button_room)
{
  struct char_data *list;
  char help_str[500];
  
  if (!CMD_IS("press")) {
    return 0;
  }
  skip_spaces(&argument);
 
  if (!*argument) {
    send_to_char("Press what?\r\n",ch);
    return 1;
  }
  if (strncasecmp(argument,"button",strlen(argument))) {
    send_to_char("You can't see anything like that here!\r\n",ch);
    return 1;
  }
  
  sprintf(help_str,"%s requires help in room %d!\r\n",GET_NAME(ch),world[ch->in_room].number);
  send_to_char("With a huge \"clonk\" you press down the button."
               "\r\nWonder what'll happen next?\r\n",ch);
  act("With a huge \"clonk\" $n presses the button.",FALSE,ch,0,0,TO_ROOM);
  
  for (list = character_list; list != NULL; list = list->next) {
    if ((GET_LEVEL(list) >= LVL_IMMORT) && (list != ch))  
       send_to_char(help_str,list);
  }
  return 1;
}

SPECIAL(laras)
{
   char *scolds[] =
   {
    "You lazy little girls! Can't you see the kettle is boiling?!",
    "I must say that you are one of the better working girls I've ever had down here.",
    "Now, come here little girl...didn't I tell you to clean this? Is it clean? Well?",
    "Why are you just sitting there? Get to work, before i work my spoon on you!",
    "If you work that slow, little one, you won't be away until tomorrow.",
    "Hah! Caught you filching, didn't I? Run off before I tell a Sedai.",
    "Well. It seems you've finally manage to get it right. Good work.",
    "No, no, no, no! I've told you a thousand times! Scrub first, wash later.",
    "Why are you sitting there feeling sorry for yourself? Move, move!",
    "You have an attitude, young woman...Watch it, or it's to the Mistress of Novices for you!",
    "Can't you even hold a tray of plates? Now, go clean this mess up, at once.",
    "Ok. I guess you can leave now, but don't putter around, you've got lots to read, I hear."
  };
 if (cmd)
   return 0;
 if (number(0,100) < 20) { 
   do_say(ch, scolds[number(0,11)], 0, 0);  
 }
 return 1;
}
SPECIAL(headgear)
{
  if (ch == (struct char_data *)me)
    return 0;

  if (cmd && !FIGHTING(ch) && GET_EQ(ch, WEAR_HEAD) && (GET_LEVEL(ch) < LVL_IMMORT)) {
    act("$N shouts 'Fade!' and attacks $n instantly!", TRUE, ch, 0, (struct char_data *)me, TO_NOTVICT);
    act("$N screams something about 'Fade!' and attacks you!", TRUE, ch, 0, (struct char_data *)me, TO_CHAR);
    hit((struct char_data *) me, ch, TYPE_UNDEFINED);
    return 1;
  }  
  return 0;
}

SPECIAL(newbiehealer)
{
  struct char_data *mob = (struct char_data *) me;

  if (ch == mob)
  {   
    return 0;
  }
  if ((GET_POS(ch) == POS_FIGHTING) || (FIGHTING(ch) != mob))
  {
    return 0;
  }
  if (!CMD_IS("kill"))
  {
    if ((GET_LEVEL(ch) < 20) &&  (GET_HIT(ch) < GET_MAX_HIT(ch)))
    {
      act("$n looks at you and shakes $s head.", TRUE, mob, 0, ch, TO_VICT);
      act("$n looks at $N and shakes $s head.", TRUE, mob, 0, ch, TO_ROOM);
      act("$n says, 'You really should be more careful...'", TRUE, mob, 0, ch, TO_VICT);
      act("$n says to $N, 'You really should be more careful...'", TRUE, mob, 0, ch, TO_VICT);
      act("$n gives you some purple leaves, which you for some strange reason eat. You feel better!", TRUE, mob, 0, ch, TO_VICT);
      act("$n gives $N some purple leaves, which he eats, after which he looks instantly better.", TRUE, mob, 0, ch, TO_ROOM);
      GET_HIT(ch) = GET_MAX_HIT(ch);
      SET_MANA(ch, GET_MAX_MANA(ch));
    }
  }
  return 0;
}

/* ********************************************************************
*  General special procedures for mobiles                             *
******************************************************************** */


void npc_steal(struct char_data * ch, struct char_data * victim)
{
  int gold;

  if (IS_NPC(victim))
    return;
  if (GET_LEVEL(victim) >= LVL_IMMORT)
    return;

  if (AWAKE(victim) && (number(0, GET_LEVEL(ch)) == 0)) {
    act("You discover that $n has $s hands in your wallet.", FALSE, ch, 0, victim, TO_VICT);
    act("$n tries to steal gold from $N.", TRUE, ch, 0, victim, TO_NOTVICT);
  } else {
    /* Steal some gold coins */
    gold = (int) ((GET_GOLD(victim) * number(1, 10)) / 100);
    if (gold > 0) {
      GET_GOLD(ch) += gold;
      GET_GOLD(victim) -= gold;
    }
  }
}

SPECIAL(trainer)
{
  sbyte stat = -1, stat3 = -1;
  sh_int stat2 = -1;
  sbyte cost;
  char text[100];
  
  if (IS_NPC(ch) || (!CMD_IS("stre") && !CMD_IS("inte") && !CMD_IS("wisd") && !CMD_IS("cons") && 
      !CMD_IS("dext") && !CMD_IS("char") && !CMD_IS("mana") && !CMD_IS("move") && !CMD_IS("hitp") &&
      !CMD_IS("air") && !CMD_IS("earth") && !CMD_IS("fire") && !CMD_IS("spirit") && !CMD_IS("water")))
    return 0;

  if (PLR_FLAGGED(ch, PLR_NOGAIN))
  {
    send_to_char("Sorry.  Bookies and some 'special' darkfriends may not do this.\n", ch);
    return 0;
  }

  if (CMD_IS("stre"))
  {
    stat = ch->real_abils.str;
    strcpy(text,"strength");
  }  
  else if (CMD_IS("inte"))
  {
    stat = ch->real_abils.intel;
    strcpy(text, "intelligence");
  }
  else if (CMD_IS("wisd"))
  {
    stat = ch->real_abils.wis;
    strcpy(text, "wisdom");
  }
  else if (CMD_IS("cons"))
  { 
    stat = ch->real_abils.con;
    strcpy(text, "constitution");
  }
  else if (CMD_IS("dext"))
  {
    stat = ch->real_abils.dex;
    strcpy(text, "dexterity");
  }
  else if (CMD_IS("char"))
  {
    stat = ch->real_abils.cha;
    strcpy(text, "charisma");
  }
  else if (CMD_IS("mana"))
  {
    if (cant_channel(ch))
    {
      send_to_char("You can't channel. It's no use. *sigh*\r\n", ch);
      return 1;  
    }
    stat2 = GET_MAX_MANA(ch);
    strcpy(text, "mana");
  }
  else if (CMD_IS("move"))
  {
    stat2 = GET_MAX_MOVE(ch);
    strcpy(text, "move");
  }
  else if (CMD_IS("hitp"))
  {
    stat2 = GET_MAX_HIT(ch);
    strcpy(text, "hitpoints");
  }
  else if (CMD_IS("air"))
  {
    stat3 = GET_AIR(ch);
    strcpy(text,"Air");
  }
  else if (CMD_IS("earth"))
  {
    stat3 = GET_EARTH(ch);
    strcpy(text,"Earth");
  }
  else if (CMD_IS("fire"))
  {
    stat3 = GET_FIRE(ch);
    strcpy(text,"Fire");
  }
  else if (CMD_IS("spirit")){
    stat3 = GET_SPIRIT(ch);
    strcpy(text,"Spirit");
  }
  else if (CMD_IS("water")){
    stat3 = GET_WATER(ch);
    strcpy(text,"Water");
  }
  else
    return 0;

  if (!GET_TRAINS(ch)&&(stat3 != -1)){
    sprintf(buf, "You haven't got any trains to increase your %s with!\r\n", text);
    send_to_char(buf, ch); 
    return 1;
  }
  if (stat >= 30){
    sprintf(buf,"You have already maxed your %s!\r\n", text);
    send_to_char(buf, ch);
    return 1;
  }
  if (stat >= 0){
    if (stat < da_costs[1].level)
      if (stat < da_costs[0].level)
        cost = 1;
      else cost = 2;
    else cost = 3;
    if (cost > GET_TRAINS(ch)){
       sprintf(buf, "The stat %s is too costly for you to raise right now.\r\n", text);
       send_to_char(buf, ch);
       return 1;
    }
    sprintf(buf, "Ok, you raise %s by one step at the cost of %d trains.\r\n", text, cost);
    send_to_char(buf, ch);
    GET_TRAINS(ch) -= cost;
    if (!strcmp(text, "strength")) 
    {
      ch->real_abils.str++;
      if (GET_STR(ch) < 30) GET_STR(ch)++;
    }
    if (!strcmp(text, "constitution")) 
    {
      ch->real_abils.con++;
      if (GET_CON(ch) < 30) GET_CON(ch)++;
     }
    if (!strcmp(text, "dexterity")) 
    {
      ch->real_abils.dex++;
      if (GET_DEX(ch) < 30) GET_DEX(ch)++;
    }
    if (!strcmp(text, "wisdom")) 
    {
      ch->real_abils.wis++;
      if (GET_WIS(ch) < 30) GET_WIS(ch)++;
    }
    if (!strcmp(text, "intelligence")) 
    {
      ch->real_abils.intel++; 
      if (GET_INT(ch) < 30) GET_INT(ch)++;
    }
    if (!strcmp(text, "charisma")) 
    {
      ch->real_abils.cha++;
      if (GET_CHA(ch) < 30) GET_CHA(ch)++;
    }
    return 1;
  }
  else if (stat2 != -1)
  {
    if (!GET_TRAINS(ch)) {
      asend_to_char(ch,"Alas! You don't have the trains to raise %s!\r\n",text);
      return 1;
    }
    sprintf(buf, "Ok, you raise %s by ten steps at the cost of a train.\r\n", text);
    send_to_char(buf, ch);
    if (!strcmp(text, "mana")) ADD_MAX_MANA(ch, 10);
    if (!strcmp(text, "hitpoints")) GET_MAX_HIT(ch) +=10;
    if (!strcmp(text, "move")) GET_MAX_MOVE(ch) +=10;
    GET_TRAINS(ch) --;
    return 1;
  }
  else if (stat3 != -1)
  {  
    if (cant_channel(ch))
    {
      send_to_char("You can't channel. It's no use. *sigh*\r\n", ch);
      return 1;  
    }
    if (!GET_TRAINS(ch))
    {
      send_to_char("Alas! You don't have the trains to raise flows.\r\n", ch);
      return 1;
    }
    /* Here we set the limits for player flowstrength depending on class <Maz>*/
    if ((GET_CLASS(ch) == CLASS_WILDER) || (GET_CLASS(ch) == CLASS_CHANNELER)) {
        //For Wilders
        if (GET_CLASS(ch) == CLASS_WILDER) {
            if (GET_AIR(ch)+GET_SPIRIT(ch)+GET_FIRE(ch)+GET_WATER(ch)+GET_EARTH(ch)-
                GET_POWER(ch) >= 200) {
                send_to_char("Your strength in the One power has reached it's peak, Wilder.\r\n", ch);
                return 1;
            }
        }
        //For Channies
        else {
            if (GET_AIR(ch)+GET_SPIRIT(ch)+GET_FIRE(ch)+GET_WATER(ch)+GET_EARTH(ch)-
                GET_POWER(ch) >= 200) {
                send_to_char("Your strength in the One power has reached it's peak Channeler.\r\n", ch);
                return 1;
            }
        }
    }
    /* If not channie or wilder the players is obviously either sparked or teachable <Maz> */
    else {
        if (GET_AIR(ch)+GET_SPIRIT(ch)+GET_FIRE(ch)+GET_WATER(ch)+GET_EARTH(ch)-
            GET_POWER(ch) >= 170) {
            send_to_char("Your strength in the One power has reached it's peak.\r\n", ch);
            return 1;
        }
    }
    if (!strcmp(text, "Air")) GET_AIR(ch)++;    
    if (!strcmp(text, "Earth")) GET_EARTH(ch)++;    
    if (!strcmp(text, "Fire")) GET_FIRE(ch)++;    
    if (!strcmp(text, "Spirit")) GET_SPIRIT(ch)++;    
    if (!strcmp(text, "Water")) GET_WATER(ch)++;
    GET_TRAINS(ch)--;
    sprintf(buf, "%s raised one step.\r\n", text);
    send_to_char(buf,ch);
    return 1;
  }
  else
  {
    send_to_char("ERROR! Please report this bug to the imms!\r\n", ch);
    return 1;
  }
} 

SPECIAL(taunt)
{
#define MAX_TAUNTS 11

  char *taunts[] = {
    "I fart in your general direction!\r\n",
    "I blow my nose at you, you and your so-called fighting-skills.\r\n",
    "Your mother was a Trolloc, and your father smelled of elder-berries!\r\n",
    "Run away! Live to flee another day!\r\n",
    "I first mistook you for a statue, when I regarded thy head of stone.\r\n",
    "Hey, as agile as a lame ox!\r\n",
    "Where did you learn to fight like that? Blind-school?\r\n",
    "If I had an arse half as ugly as your face, I'd only crap in the dark!\r\n",
    "Geee, with that face you'd scare a Trolloc away!\r\n",
    "You sad, sad looser!\r\n",
    "Quickly! Put the sword back before your dad misses it!\r\n",
    "People like you should be prematurely turned into glue!\r\n"
  };

  if (cmd) return FALSE;
  if (GET_POS(ch) != POS_FIGHTING) return FALSE;
  if (FIGHTING(ch) && (FIGHTING(ch)->in_room == ch->in_room) && (number(0,25) < 10)) {
    do_say(ch,taunts[number(0,MAX_TAUNTS)],0,0);
  }

  return TRUE;

#undef MAX_TAUNTS
}



SPECIAL(thief)
{
  struct char_data *cons;

  if (cmd)
    return FALSE;

  if (GET_POS(ch) != POS_STANDING)
    return FALSE;

  for (cons = world[ch->in_room].people; cons; cons = cons->next_in_room)
    if (!IS_NPC(cons) && (GET_LEVEL(cons) < LVL_IMMORT) && (!number(0, 4))) {
      npc_steal(ch, cons);
      return TRUE;
    }
  return FALSE;
}


/* ********************************************************************
*  Special procedures for mobiles                                      *
******************************************************************** */

SPECIAL(puff)
{
  if (cmd)
    return (0);

  switch (number(0, 60)) {
  case 0:
    do_osay(ch, "My god!  It's full of stars!", 0, 0);
    return (1);
  case 1:
    do_osay(ch, "How'd all those false dragons get up here?", 0, 0);
    return (1);
  case 2:
    do_osay(ch, "Oh my god, it's Spiney Norman, and he's 500' tall!", 0, 0);
    return (1);
  case 3:
    do_osay(ch, "42", 0, 0);
    return (1);
  case 4:
    do_osay(ch, "FABRICATI DIEM, PVNK!",0,0);
    return (1);
  case 5:
    do_osay(ch, "#define Question ((BB) || !(BB))", 0, 0);
    return (1);
  case 6: 
    do_osay(ch, "I wouldn't mind a very large pizza right now.", 0, 0);
    return (1);
  case 7:
    do_osay(ch, "We are the knights who say 'Ni!'", 0, 0);
    return (1);
  case 8:
    do_osay(ch, "Your mother was a hamster and your father smelled of elderberries!", 0, 0);
    return (1);
  case 9:
    do_osay(ch, "You can join any guild after making it to level 20.. or Ogier and tinkers at any level.", 0, 0);
    return(1);
  case 10:
    do_osay(ch, "Type \\c02help RP\\c00 to read something you may find of interest.", 0, 0);
    return(1);
  case 11:
    do_osay(ch, "Type \\c02help pk\\c00 to read some things you absolutely must know.", 0, 0);
    return(1);
  case 12:
    do_osay(ch, "Type \\c03policy\\c00. You need to know these things!", 0,0);
    return(1);
  case 13:
    do_osay(ch, "All channeling done for leveling is \\c01OOC\\c00! You can't see someone channel and know it ICly that way.", 0,0);
    return(1);
  case 14:
    do_osay(ch, "There are many cities to visits and thousands of rooms. See them all!", 0,0);
    return(1);
  case 15:
    do_osay(ch, "Have you played in a Quest yet?", 0,0);
    return(1);
  case 17:
    do_osay(ch, "No good deed goes unpunished.", 0,0);
    return(1);
  case 18: 
    do_osay(ch, "People with no discernable neck is not allowed to have an opinion!",0,0);
    return 1;
  case 19:
    do_osay(ch,"The path of Tae Kwan Leep is not a path to a door, but a journey forever towards the horizon.",0,0);
    return 1;
  case 20:
    do_osay(ch, "Let's meditate upon this wisdom...*ohhhhm*",0,0);
    return 1;
  case 21:
    do_osay(ch,"Fighting for peace is like fu...Ooops, that was close!",0,0);
    return 1;
  case 22:
    do_osay(ch,"I'm a lumberjack and I'm okay!",0,0);
    return 1;
  default:
    return (0);
  }
}



SPECIAL(fido)
{

  struct obj_data *i, *temp, *next_obj;

  if (cmd || !AWAKE(ch))
    return (FALSE);

  for (i = world[ch->in_room].contents; i; i = i->next_content) {
    if (GET_OBJ_TYPE(i) == ITEM_CONTAINER && GET_OBJ_VAL(i, 3)) {
      act("$n savagely devours a corpse.", FALSE, ch, 0, 0, TO_ROOM);
      for (temp = i->contains; temp; temp = next_obj) {
    next_obj = temp->next_content;
    obj_from_obj(temp);
    obj_to_room(temp, ch->in_room);
      }
      extract_obj(i);
      return (TRUE);
    }
  }
  return (FALSE);
}



SPECIAL(janitor)
{
  struct obj_data *i;

  if (cmd || !AWAKE(ch))
    return (FALSE);

  for (i = world[ch->in_room].contents; i; i = i->next_content) {
    if (!CAN_WEAR(i, ITEM_WEAR_TAKE))
      continue;
    if (GET_OBJ_TYPE(i) != ITEM_DRINKCON && GET_OBJ_COST(i) >= 15)
      continue;
    act("$n picks up some trash.", FALSE, ch, 0, 0, TO_ROOM);
    obj_from_room(i);
    obj_to_char(i, ch);
    return TRUE;
  }

  return FALSE;
}



#define PET_PRICE(pet) (GET_LEVEL(pet) * 300)



/* ********************************************************************
*  Special procedures for objects                                     *
******************************************************************** */

SPECIAL(warder_cloak)
{
  struct obj_data *cloak = (struct obj_data *)me;

  if (!CMD_IS("wear") && !CMD_IS("remove"))
    return 0;

  skip_spaces(&argument);

  if (CMD_IS("wear")) {
    if (cloak != get_obj_in_list_vis(ch,argument,ch->carrying))
      return 0;
    do_wear(ch,argument,0,0);

    if (cloak == GET_EQ(ch,find_eq_pos(ch,cloak,NULL))) 
      SET_BIT(AFF_FLAGS(ch),AFF_INVISIBLE);
    return 1;
  }
  if (CMD_IS("remove")) {
    if (cloak != get_obj_worn_vis(ch,argument))
      return 0;
    do_remove(ch,argument,0,0);
    REMOVE_BIT(AFF_FLAGS(ch),AFF_INVISIBLE);
    return 1;
  }
  return 0;
}

SPECIAL(strong_dream_obj)
{
  struct obj_data *ring = (struct obj_data *)me;

  if (CMD_IS("dream") && !GET_SKILL(ch, SKILL_DREAM)) {
    if (GET_POS(ch) != POS_SLEEPING) 
      send_to_char("You can't enter Tel'aran'rhiod awake!\r\n", ch);
    else if (!ring->worn_by) 
      send_to_char("You must wear the dream ter'angreal first.\r\n", ch);
    else if (IS_SET(OBJ_EXTRA_FLAGS(ring), ITEM_NOMALE) && (GET_SEX(ch) == SEX_MALE))
      send_to_char("Only women can use this ter'angreal.\r\n", ch);
    else if (IS_SET(OBJ_EXTRA_FLAGS(ring), ITEM_NOFEMALE) && (GET_SEX(ch) == SEX_FEMALE))
      send_to_char("Only men can use this ter'angreal.\r\n", ch);
    else if (IS_AFFECTED(ch,AFF_DREAMING)) 
      send_to_char("Hey, you're already in Tel'Aran'Rhiod!\r\n",ch);
    else
      do_dream(ch, "", 0, 1);
    return 1;
  }
  else if (CMD_IS("wake") && !GET_SKILL(ch,SKILL_DREAM) && (IS_AFFECTED((ch),AFF_DREAMING))) {
    send_to_char("You awaken, and sit up.\r\n", ch);
    //act("$n suddenly gets misty and vanishes, no trace remaining.",FALSE,ch,0,0,TO_ROOM);
    wakeup_sequence(ch);
    act("$n awakens.", TRUE, ch, 0, 0, TO_ROOM);
    GET_POS(ch) = POS_SITTING;
    return 1;
  }
  return 0;
}

SPECIAL(weak_dream_obj)
{
  struct obj_data *ring = (struct obj_data *)me;

  if (CMD_IS("dream") && !GET_SKILL(ch, SKILL_DREAM)) {
    if (GET_POS(ch) != POS_SLEEPING)
      send_to_char("You can't enter Tel'aran'rhiod awake!\r\n", ch);
    else if (!ring->worn_by)
      send_to_char("You must wear the dream ter'angreal first.\r\n", ch);
    else if (IS_SET(OBJ_EXTRA_FLAGS(ring), ITEM_NOMALE) && (GET_SEX(ch) == SEX_MALE))
      send_to_char("Only women can use this ter'angreal.\r\n", ch);
    else if (IS_SET(OBJ_EXTRA_FLAGS(ring), ITEM_NOFEMALE) && (GET_SEX(ch) == SEX_FEMALE))
      send_to_char("Only men can use this ter'angreal.\r\n", ch);
    else if (IS_AFFECTED(ch,AFF_DREAMING))
      send_to_char("Hey, you're already in Tel'Aran'Rhiod!\r\n",ch);
    else
    {
      do_dream(ch, "", 0, 1);
      SET_BIT(AFF2_FLAGS(ch), AFF2_TARBLUR);
    }
    return 1;
  }
  else if (CMD_IS("wake") && !GET_SKILL(ch,SKILL_DREAM) && (IS_AFFECTED((ch),AFF_DREAMING))) {
    send_to_char("You awaken, and sit up.\r\n", ch);
    //act("$n suddenly gets misty and vanishes, no trace remaining.",FALSE,ch,0,0,TO_ROOM);
    wakeup_sequence(ch);
    act("$n awakens.", TRUE, ch, 0, 0, TO_ROOM);
    GET_POS(ch) = POS_SITTING;
    return 1;
  }
  return 0;
}

SPECIAL(recall_charm)
{
  if (CMD_IS("recall")) {
    if (GET_LEVEL(ch) > 40) {
      if (GET_POS(ch) == POS_FIGHTING) {
        send_to_char("Not while fighting.\r\n", ch);
        return 1;
      }
      act("$n invokes the powers of $s recall ter'angreal and vanishes!", TRUE, ch, 0, 0, TO_ROOM);
      act("Ok, you use your recall ter'angreal.", TRUE, ch, 0, 0, TO_CHAR);
      char_from_room(ch);
      char_to_room(ch, real_room(recall_room));
      do_look(ch, "\0", 0, SCMD_LOOK);
      act("$n arrives through a shimmering haze.", TRUE, ch, 0, 0, TO_ROOM);
      extract_obj((struct obj_data *)me);
      return 1;
    }
  }
  return 0;
}  

SPECIAL(bank)
{
  struct char_data *vict;
  char mesg[500];
  char name[100];
  char amt[100];
  int amount;

  if (CMD_IS("balance")) {
    if (GET_BANK_GOLD(ch) > 0)
      sprintf(buf, "Your current balance is %ld coins.\r\n",
          GET_BANK_GOLD(ch));
    else
      sprintf(buf, "You currently have no money deposited.\r\n");
    send_to_char(buf, ch);
    return 1;
  } else if (CMD_IS("deposit")) {
    if ((amount = atoi(argument)) <= 0) {
      send_to_char("How much do you want to deposit?\r\n", ch);
      return 1;
    }
    if (GET_GOLD(ch) < amount) {
      send_to_char("You don't have that many coins!\r\n", ch);
      return 1;
    }
    GET_GOLD(ch) -= amount;
    GET_BANK_GOLD(ch) += amount;
    sprintf(buf, "You deposit %d coins.\r\n", amount);
    send_to_char(buf, ch);
    act("$n makes a bank transaction.", TRUE, ch, 0, FALSE, TO_ROOM);
    return 1;
  } else if (CMD_IS("withdraw")) {
    if ((amount = atoi(argument)) <= 0) {
      send_to_char("How much do you want to withdraw?\r\n", ch);
      return 1;
    }
    if (GET_BANK_GOLD(ch) < amount) {
      send_to_char("You don't have that many coins deposited!\r\n", ch);
      return 1;
    }
    GET_GOLD(ch) += amount;
    GET_BANK_GOLD(ch) -= amount;
    sprintf(buf, "You withdraw %d coins.\r\n", amount);
    send_to_char(buf, ch);
    act("$n makes a bank transaction.", TRUE, ch, 0, FALSE, TO_ROOM);
    return 1;
  } else if (CMD_IS("wire")) {
    if (GET_LEVEL(ch) >= LVL_IMMORT) {
      send_to_char("Sorry, IMMs can't wire gold.\r\n", ch);
       return 1;
    }
    two_arguments(argument, name, amt);
    if (!*name) {
      send_to_char("Usage:\r\nWire <player> <amount of gold>\r\n\r\nNote: This is a Bank-to-Bank transfer.\r\n", ch);
      return 1;
    }
    if (!*amt) {
      send_to_char("How many coins do you wish to wire?\r\n", ch);
      return 1;
    }
    if ((vict=get_char_vis(ch, name))) {
      if (ch == vict) {
        send_to_char("Wiring gold to yourself?  Neat trick.\r\n", ch);
        return 1;
      }
      if (IS_MOB(vict)) {
        send_to_char("Don't send gold to Mobs.  That's silly.\r\n", ch);
        return 1;
      }
      amount=atoi(amt);
      if (amount <= 0) {
        send_to_char("Now why would you want to do that?\r\n", ch);
        return 1;
      }
      if (GET_BANK_GOLD(ch) < amount) {
        send_to_char("You don't have that much gold in the bank!\r\n", ch);
        return 1;
      }
      GET_BANK_GOLD(vict) += amount;
      GET_BANK_GOLD(ch) -= amount;
      sprintf(buf, "You've just received %s coins from %s.\r\n", amt, GET_NAME(ch));
      send_to_char(buf, vict);
      sprintf(buf, "You send %s coins to %s.\r\n", amt, GET_NAME(vict));
      send_to_char(buf, ch);
      act("$n makes a bank transaction.", TRUE, ch, 0, FALSE, TO_ROOM);
      do_save(ch, "", 0, 0);
      return 1;
    } else {
      if (NULL != (vict=xmlload_char(name))) {
        amount=atoi(amt);
        if (amount <= 0) {
          send_to_char("Now why would you want to do that?\r\n", ch);
          return 1;
        }
        if (GET_BANK_GOLD(ch) < amount) {
          send_to_char("You don't have that much gold in the bank!\r\n", ch);
          return 1;
        }
        GET_BANK_GOLD(vict) += amount;
        GET_BANK_GOLD(ch) -= amount;
        sprintf(mesg, "&gAutomated System Mudmail&n:\r\n\r\nWhile you were offline you receieved a transfer of %s gold.\r\n", amt);
        store_mail(GET_IDNUM(vict), GET_IDNUM(ch), mesg);
        sprintf(buf, "You send %s coins to %s.\r\n", amt, GET_NAME(vict));
        send_to_char(buf, ch);
        act("$n makes a bank transaction.", TRUE, ch, 0, FALSE, TO_ROOM);
        xmlsave_char(vict);
        free_char(vict);
        do_save(ch, "", 0, 0);
        return 1;
      } else {
        send_to_char("Char not found online or offline.\r\n", ch);
        return 1;
      }
    }
  }  else
    return 0;
}

void all_obj_to_tar(struct char_data *ch)
{
  int i;
  struct obj_data *obj = NULL;

  for (i = 0; i < NUM_WEARS; i++) {
    obj = GET_EQ(ch,i);
    if (obj) {
      SET_BIT(GET_OBJ_EXTRA(obj),ITEM_TAR);
    }
  }
}

void all_obj_from_tar(struct char_data *ch)
{
  int i;
  struct obj_data *obj = NULL;

  for (i = 0; i < NUM_WEARS; i++) {
    obj = GET_EQ(ch,i);
    if (obj) {
      REMOVE_BIT(GET_OBJ_EXTRA(obj),ITEM_TAR);
    }
  }
}

static void enter_tar_gate(struct char_data *ch) 
{
  if (ch->pulling) {
    send_to_char("You stop pulling your wagon.\r\n",ch);
    ch->pulling->pulled_by = NULL;
    ch->pulling = NULL;
  }

  TICKS_IN_ROOM(ch) = 0; /* reset tick ctr... */

  if (IS_AFFECTED2(ch,AFF2_TAR_GATE)) {
    act("&W$n enters the gate and disappears.&n",TRUE,ch,0,0,TO_DREAMERS);
    send_to_char("&WYou enter the gate, and step back to the waking world.&n\r\n",ch);
    if (GET_DISGUISED(ch))
    {
      do_newdisguise(ch,"off",0,0);
      fetch_char_guild_data(ch);
    }
    REMOVE_BIT(AFF2_FLAGS(ch),AFF2_TAR_GATE);
    REMOVE_BIT(AFF_FLAGS(ch),AFF_DREAMING);
    all_obj_from_tar(ch);
    act("&W$n steps out of a gate.&n",TRUE,ch,0,0,TO_ROOM);
    do_look(ch,"",0,SCMD_LOOK);
  } else { // entering gate to T'a'R. 
    act("&W$n enters the gate and disappears.&n",TRUE,ch,0,0,TO_ROOM);
    send_to_char("&WYou enter the gate into Tel'Aran'Rhiod....&n\r\n",ch);
    SET_BIT(AFF2_FLAGS(ch),AFF2_TAR_GATE);
    SET_BIT(AFF_FLAGS(ch),AFF_DREAMING);
    all_obj_to_tar(ch);
    act("&W$n steps out from a gate.&n",TRUE,ch,0,0,TO_DREAMERS);
    do_look(ch,"",0,SCMD_LOOK);
  }
}

SPECIAL(dream_gate)
{
  char arg[1024];
  struct obj_data *gate1,*gate2 = (struct obj_data *)me;
  struct follow_type *k, *next;
  int was_in = ch->in_room;

  if (!CMD_IS("enter") && !CMD_IS("look") && !CMD_IS("examine"))
    return 0;

  argument = one_argument(argument,arg);

  if (!(gate1 = get_obj_in_list_vis(ch,arg,world[ch->in_room].contents))) {
    return 0;
  }

  if (gate1 != gate2)
    return 0;

  if (!*arg)
    return 0; 

  gate1 = gate2->linked;

  if (IS_AFFECTED(ch,AFF_DREAMING) && !IS_AFFECTED2(ch,AFF2_TAR_GATE) && CMD_IS("enter")) {
    send_to_char("You can't leave Tel'Aran'Rhiod through this gate, since you're not here in the flesh!\r\n",ch);
    return 1;
  }

  if (IS_AFFECTED2(ch,AFF2_TAR_GATE)) {
    if (CMD_IS("enter")) {
      enter_tar_gate(ch);

      k = ch->followers;
      while (k) {
    next = k->next;
    if (k->follower->in_room == was_in && GET_POS(k->follower) == POS_STANDING) {
      act("You follow $N.\r\n", FALSE, k->follower, 0, ch, TO_CHAR);
      enter_tar_gate(k->follower);
    }
    k = next;
      }

      k = ch->tailed;
      while (k) {
    next = k->next;
    if (k->follower->in_room == was_in && GET_POS(k->follower) == POS_STANDING) {
      act("You follow $N.\r\n", FALSE, k->follower, 0, ch, TO_CHAR);
      enter_tar_gate(k->follower);
    }
    k = next;
      }
    } else { /* Examine command! */
      send_to_char("\\c01You peer through the gate at the waking world:\\c00\r\n\r\n",ch);
      REMOVE_BIT(AFF2_FLAGS(ch),AFF2_TAR_GATE);
      REMOVE_BIT(AFF_FLAGS(ch),AFF_DREAMING);
      do_look(ch,"",0,SCMD_LOOK);
      SET_BIT(AFF2_FLAGS(ch),AFF2_TAR_GATE);
      SET_BIT(AFF_FLAGS(ch),AFF_DREAMING);
    }   
  } else {
    if (CMD_IS("enter")) {
      enter_tar_gate(ch);
      
      k = ch->followers;
      while (k) {
    next = k->next;
    if (k->follower->in_room == was_in && GET_POS(k->follower) == POS_STANDING) {
      act("You follow $N.\r\n", FALSE, k->follower, 0, ch, TO_CHAR);
      enter_tar_gate(k->follower);
    }
    k = next;
      }

      k = ch->tailed;
      while (k) {
    next = k->next;
    if (k->follower->in_room == was_in && GET_POS(k->follower) == POS_STANDING) {
      act("You follow $N.\r\n", FALSE, k->follower, 0, ch, TO_CHAR);
      enter_tar_gate(k->follower);
    }
    k = next;
      }
    } else { /* Examine-command */
      send_to_char("\\c01You peer through the misty gate into Tel'Aran'Rhiod:\\c00\r\n\r\n",ch);
      SET_BIT(AFF2_FLAGS(ch),AFF2_TAR_GATE);
      SET_BIT(AFF_FLAGS(ch),AFF_DREAMING);
      do_look(ch,"",0,SCMD_LOOK);
      REMOVE_BIT(AFF2_FLAGS(ch),AFF2_TAR_GATE);
      REMOVE_BIT(AFF_FLAGS(ch),AFF_DREAMING);
    }
  }
  return 1;
}

static void enter_gate(int room, struct char_data *ch) 
{
  int was_in = ch->in_room;

  TICKS_IN_ROOM(ch) = 0;

  act("$n enters the Gate and disappears.",TRUE,ch,NULL,NULL,TO_ROOM);
  act("You enter a Gate and step out at another place.",TRUE,ch,NULL,NULL,TO_CHAR);
  char_from_room(ch);
  char_to_room(ch,room);
    
  act("$n steps out of a Gate.",TRUE,ch,NULL,NULL,TO_ROOM);
  act("You step out of a Gate at....",TRUE,ch,NULL,NULL,TO_CHAR);
  do_look(ch,"\0",0,SCMD_LOOK);

  if (ch->pulling) {
    obj_from_room(ch->pulling);
    obj_to_room(ch->pulling,ch->in_room);
  }

  entry_memory_mtrigger(ch);
  if (!greet_mtrigger(ch, -2, was_in) || !greet_otrigger(ch, -2)) {
    char_from_room(ch);
    look_at_room(ch, 0);
  } else greet_memory_mtrigger(ch);
  
}

SPECIAL(sp_gate)
{
  char arg1[200];
  struct obj_data *gate1,*gate2 = (struct obj_data *) me;
  struct follow_type *k, *next;
  sh_int room, was_in;
  int made_by;

  if (!CMD_IS("enter") && !CMD_IS("look") && !CMD_IS("examine"))
    return 0;
    
  argument = one_argument(argument,arg1);
  
  if (!(gate1 = get_obj_in_list_vis(ch,arg1,world[ch->in_room].contents))) {
    return 0;
  }

  if (gate1 != gate2)
    return 0;
    
  if (!*arg1)
    return 0;  
  
  if (CMD_IS("enter")) {
    was_in = IN_ROOM(ch);
    room    = GET_OBJ_VAL(gate2,2);
    made_by = GET_OBJ_VAL(gate2,1);
    gate1 = gate2->linked;
    
    if (!entry_mtrigger(ch))
      return 1;
    if (!enter_wtrigger(&world[room], ch, -2))
      return 1;
    if (!leave_wtrigger(&world[ch->in_room], ch))
      return 1;

    enter_gate(room,ch);
  
    k = ch->followers;
    while (k) {
      next = k->next;
      if (k->follower->in_room == was_in && GET_POS(k->follower) == POS_STANDING) {
        act("You follow $N.\r\n", FALSE, k->follower, 0, ch, TO_CHAR);
    enter_gate(room,k->follower);
      }
      k = next;
    }

    k = ch->tailed;
    while (k) {
      next = k->next;
      if (k->follower->in_room == was_in && GET_POS(k->follower) == POS_STANDING) {
        act("You follow $N.\r\n", FALSE, k->follower, 0, ch, TO_CHAR);
    enter_gate(room,k->follower);
      }
      k = next;
    }
  }
  else {
    send_to_char("You look through the gate and you see:\r\n",ch);
    char_from_room(ch);
    char_to_room(ch,GET_OBJ_VAL(gate2,2));
    do_look(ch,"\0",0,SCMD_LOOK);
    char_from_room(ch);
    char_to_room(ch,GET_OBJ_VAL(gate2->linked,2));
    return 1;
  }
  return 1;
}

SPECIAL(pigeon)  
{                
  struct obj_data *target = (struct obj_data *) me;
  int cove;
  if (CMD_IS("pigeon")){
    if (!GET_CHAR_GUILDS(ch) && !is_dark(ch)){
      send_to_char("Only guild members can use the pigeons. \r\n", ch);
      return 1;  
    }
    cove = GET_OBJ_VNUM(target);  /* The pigeon note's vnum decides where it goes */
    obj_from_char(target);   
    send_to_char("You release the pigeon, and watch the bird fly away.\r\n", ch);   
    act("$n sends a pigeon away, carrying a message to someone.", TRUE, ch, 0, 0, TO_ROOM);
    obj_to_room(target, real_room(cove));
    act("A pigeon arrives, carrying a message.", FALSE, 0, target, 0, TO_ROOM);
    return 1;
  } 
  return 0;
}


SPECIAL(herb)
{
  struct obj_data *herb;
  char	*name;
  CREATE( name, char, 10 );
  sprintf(name,"herb");

  if(CMD_IS("eat")){
    if (!(herb = get_obj_in_list_vis(ch,name, ch->carrying)))
      return 0;
    obj_from_char(herb);
    GET_HIT(ch) = MIN(GET_MAX_HIT(ch), GET_HIT(ch) + 100);
    SET_MANA(ch, MIN(GET_MAX_MANA(ch), GET_MANA(ch) + 100));
    GET_MOVE(ch) = MIN(GET_MAX_MOVE(ch), GET_MOVE(ch) + 100);
    GET_FRACT_MOVE(ch) = 0;
    send_to_char("You eat the Herbs and feal refreshed.\r\n", ch);
    free(name);
    return 1;
  }
  free(name);
  return 0;
}

SPECIAL(aelfinn)
{
  room_num to;

  if (!CMD_IS("enter")){
    return 0;
  }
  if (ch->in_room != real_room(100))          /* Location of Aelfinn side */
    to = 100;                      /*   of the stone doorway   */
  else
    to = 32601; /* Temporary, arbitrary room_num, until stone of tear is done */
  act("$n enters the stone doorway and doesn't come out the other side.",TRUE,ch,NULL,NULL,TO_ROOM);
  act("You step into the stone doorway, and get the earie fealing that time\r\nhas actually accelerated around you.  You stumble and fall out in a new\r\nplace.",TRUE,ch,NULL,NULL,TO_CHAR);

  act("$n enters the stone doorway and doesn't come out the other side.",TRUE,ch,NULL,NULL,TO_ROOM);
  char_from_room(ch);

  char_to_room(ch,real_room(to));
  GET_POS(ch) = POS_SITTING;
  act("$n enters the stone doorway and doesn't come out the other side.",TRUE,ch,NULL,NULL,TO_ROOM);
  do_look(ch,"\0",0,SCMD_LOOK);

  return 1;
}


/*******************************************************************************
*                           Special procedures for rooms                       *
*******************************************************************************/

SPECIAL(customs)
{
  int dir = 0;
  if (CMD_IS("north"))
    dir = 0;
  else if (CMD_IS("east"))
    dir = 1;
  else if (CMD_IS("south"))
    dir = 2;
  else if (CMD_IS("west"))
    dir = 3;
  else if (CMD_IS("up"))
    dir = 4;
  else if (CMD_IS("down"))
    dir = 5;
  else return 0;
  if (CAN_GO(ch, dir) &&  (EXIT(ch, dir)->general_description) && 
    !strncasecmp(EXIT(ch, dir)->general_description,"You look ", 8))  
                    /* If the exit description begins with "You look", the */
  if (!IS_NPC(ch))
  {                
    act("The guardman says, 'Ya hafta pay toll to get in here. 10 gold, iff'n ye pleeze' to $n.", FALSE, ch, 0, 0 , TO_ROOM);
    send_to_char("The guardsman takes 10 gold in toll for you to go in there.\r\n", ch);
    if (GET_GOLD(ch)< 10)  /* The guy is broke and won't be let in. */
    {
      act("The guardsman notices that $n doesn't have any money and refuses to let $m in.", FALSE, ch, 0, 0, TO_ROOM);
      send_to_char("The guardsman says, annoyed, 'Hey! Ye're broke! No -way- I'll let ya in here.\r\n", ch);
      return 1;
    }
    GET_GOLD(ch)-= 10;
    return 0;
  }
  return 0; 
}


SPECIAL(hospital)
{
  int heal = 0;
  char buf[80];
  if (CMD_IS("heal"))
  {
    if (GET_HIT(ch) == GET_MAX_HIT(ch)){
      send_to_char("You don't need any healing, luckily.\r\n", ch);
      return 1;
    }
    
    argument = one_argument(argument, buf);
    if (!buf || (!is_number(buf) && strcasecmp(buf, "all")))
    {
      send_to_char("How many hitpoints do you want to heal?\r\n", ch);
      return 1;
    }
    if (!strcasecmp(buf, "all"))
      heal = GET_MAX_HIT(ch) - GET_HIT(ch);
    else
      heal = atoi(buf);
    if ((heal <= 0) || (heal > GET_MAX_HIT(ch) - GET_HIT(ch)))
    {
      send_to_char("Illegal number of hitpoints to heal. Try another.\r\n", ch);
      return 1;
    }
    if (GET_GOLD(ch) < heal)
    {
      send_to_char("You can't afford to heal so many hitpoints.\r\n", ch);
      return 1;
    }
    if (!heal)
    {
      send_to_char("Heal how many hitpoints?\r\n", ch);
      return 1;
    }
    GET_HIT(ch) += heal;
    GET_GOLD(ch) -= heal;
    send_to_char("You feel much, much better.\r\n", ch);  
    return 1;
  }
  else
    return 0;
}

SPECIAL(skillshop)
{
  int i = 0, j = 0, list[100], buy = 0, cost = 0;
  
  if (!(CMD_IS("list") || CMD_IS("buy")))
    return 0;
    
  one_argument(argument, arg);
  
  for (i = 0; i < 100; i ++)
    list[i] = 0;
    
  for (i = 1; i < MAX_SKILLS; i ++) {
    if ((spell_info[i].type == SPELL_QUEST) || (spell_info[i].type == SKILL_QUEST)) {
      list[j] = i;
      j ++;
    }
  }
  
  if (CMD_IS("list")) {
    sprintf(buf,"These are the skills/spells that you can buy with quest points.\r\n");
    for (i = 0; i < j ; i ++)
      if (!cant_channel(ch) || (spell_info[list[i]].type == SKILL_QUEST))
        sprintf(buf, "%s%-25s %ld QP\r\n", buf, spells[list[i]], 
          spell_info[list[i]].cost[1]);
    page_string(ch->desc,buf,1);
    return 1;
  }
  
  if (CMD_IS("buy")) {
    for (i = 0; i < j; i ++)  
      if (isname(arg, spells[list[i]]))
        buy = list[i];
        
    if (!buy) {
      send_to_char("No such questskill/spell found...Maybe you typed it wrong?\r\n", ch);
      return 1;
    }
    if (GET_SKILL(ch, buy)) {
      send_to_char("But you already know that spell/skill!\r\n", ch);
      return 1;
    }
    cost = spell_info[buy].cost[1];
    if (GET_QP(ch) < cost) {
      send_to_char("You can't afford it, alas...Quest more.\r\n", ch);
      return 1;
    }
    
    if(meets_skillspell_reqs(ch, buy)){
      sprintf(buf, "$n buys $mself a quest%s!", (spell_info[buy].type == SKILL_QUEST)? "skill":"spell");
      act(buf, TRUE, ch, 0, 0, TO_ROOM);
      sprintf(buf, "Ok, you buy the Quest%s %s! Enjoy :) \r\n", 
        (spell_info[buy].type == SKILL_QUEST)? "skill":"spell", spells[buy]);
      send_to_char(buf, ch);
      send_to_char("Don't forget to practice it now!\r\n", ch);
      SET_SKILL(ch, buy, 1);
      GET_QP(ch) -= cost;
      return 1;
    }
    else{
      send_to_char("You do not have the appropriate talents or special skills to gain this spell/skill.\r\n", ch);
      return 1;
    }
  }
  return 1;
}

SPECIAL(portal_stones)
{
  int i,j,found = 0,result;
  struct char_data *list;

  if (!CMD_IS("list") && !CMD_IS("travel"))
    return 0;

  if (!PRF_FLAGGED(ch,PRF_GRASPING) && !PRF_FLAGGED(ch,PRF_TGRASP)) {
     send_to_char("You must grasp the Power to use a Portal Stone.\r\n",ch);
     return 1;
  }
  if (!GET_SKILL(ch,SKILL_PORTAL_STONES)) {
    send_to_char("You know NOTHING about Portal Stones!\r\n",ch);
    return 1;
  }
    
  for (i = 0; ((i < MAX_STONES) && !found); i++) {
     if (ch->in_room == real_room(stones[i].in_room)) 
       found = TRUE;
   }               

   if (!found) {
     send_to_char("ERROR! Please report to the Imms!\r\n",ch);
     return 1;
   }
   i--;

   if (CMD_IS("list")) {
     gen_message(PORTAL_LIST,ch,NULL,FALSE,TRUE);

     j = 0;
     
     while(stones[i].linked_with[j] != -1 &&  
       (NOWHERE == real_room(stones[stones[i].linked_with[j]].in_room))) {
       send_to_char(stones[stones[i].linked_with[j]].name,ch);
       send_to_char("\r\n",ch);
       j++;
     }
     ADD_MANA(ch, -100);
     return 1;
   }
   if (CMD_IS("travel")) {
      argument++;
      if (!*argument) {
        send_to_char("Perhaps if you told me where you wish to go?\r\n",ch);
        return 1;
      }
      j = 0;
      found = 0;

      while((stones[i].linked_with[j] != -1) && !found) {
        if (!strncasecmp(stones[stones[i].linked_with[j]].name,argument,strlen(argument)))
          found = 1;
        j++;
      }
      j--;
      if (!found) {
        send_to_char("No such destination!\r\n",ch);
        return 1;
      }

      if (NOWHERE == real_room(stones[stones[i].linked_with[j]].in_room)) {
	send_to_char("Destination room does not exists in world...please inform an Imm!\r\n",ch);
	return 1;
      }

      result = generic_result_mod(ch,SKILL_PORTAL_STONES,NULL,FALSE,(GET_INT(ch) - 15));

      if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
        SET_MANA(ch, 0);
        GET_MOVE(ch) = 0;
        GET_FRACT_MOVE(ch) = 0;
        REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
        REMOVE_BIT(PRF_FLAGS(ch),PRF_TGRASP);
        GET_POS(ch) = POS_SLEEPING;
        gen_message(PORTAL_TRANSF_GM,ch,NULL,FALSE,FALSE);
        return 1;
      }

      if (IS_AFFECTED(ch,AFF_GROUP)) {
        for (list = world[ch->in_room].people; list; list = list->next_in_room) {
            if (is_in_group(ch,list) && (list != ch)) {
	      gen_message(PORTAL_TRANSF_GM,ch,list,TRUE,TRUE);
	      char_from_room(list);
	      char_to_room(list,real_room(stones[stones[i].linked_with[j]].in_room));
	      do_look(list,"",0,0);
	      ADD_MANA(ch, -200);
	   }
        }
      }
      gen_message(PORTAL_TRANSF_SELF,ch,ch,TRUE,TRUE);

      char_from_room(ch);
      char_to_room(ch,real_room(stones[stones[i].linked_with[j]].in_room));
      do_look(ch,"",0,0);
      ADD_MANA(ch, -200);

     return 1;
   }
   return 1;
}

SPECIAL(dreamroom)
{

  if (IS_NPC(ch))
    return 0;

  if (!IS_AFFECTED(ch, AFF_DREAMING))
    return 0;
  if (IS_IMMORTAL(ch))
    return 0;
  if (!get_guild(ch,7) && !is_dark(ch)) {
    send_to_char("Death to those who come here uninvited!!\r\n", ch);
    die(ch, ch);
    return 1;
  }
  
  if (!CMD_IS("say") && !CMD_IS("emote") && !CMD_IS("wake") && !CMD_IS("look") &&
      !CMD_IS("'") && !CMD_IS(":") && !CMD_IS("ask") && !CMD_IS("sayto") &&
   (get_guild(ch,7))) {
    send_to_char("You're not allowed to do anything besides talk, emote, look and wake up here.\r\n", ch);
    return 1;
  } 
  
  if (CMD_IS("wake") && (!get_guild(ch,7))) {
    send_to_char("You wake up from your dreams, your masters words still ringing in your ears.\r\n", ch);
    act("$n goes all misty and simply vanishes from the room.", TRUE, ch, 0, 0, TO_NOTCHAR);
    REMOVE_BIT(AFF2_FLAGS(ch),AFF2_DARKDREAM);
    wakeup_sequence(ch);
    return 1;
  }
  return 0;
}
     
SPECIAL(repair_shop)
{
  struct obj_data *obj;

  if (CMD_IS("repair"))
  {
    skip_spaces(&argument);
    if (!*argument) 
    {
      send_to_char("Repair what?\r\n", ch);
      return 1;
    }
    if (!(obj = get_obj_in_list_vis(ch, argument, ch->carrying))) {
      send_to_char("You don't carry any such thing.\r\n", ch);  
      return 1;
    }
    if ((GET_OBJ_TYPE(obj) != ITEM_WEAPON) && (GET_OBJ_TYPE(obj) != ITEM_ARMOR)) 
    {
     send_to_char("Erm..that type of object doesn't need repairing.\r\n",ch);
     return 1;
    }
    if (GET_OBJ_STATUS(obj) > 95) 
    {
      act("$p doesn't need any repairing.", TRUE, ch, obj, 0, TO_CHAR);
      return 1;
    }
    if (GET_GOLD(ch) < GET_OBJ_COST(obj)/5) 
    {
      send_to_char("You can't afford to repair the item.\r\n", ch);
      return 1;
    }
    act("The blacksmith swiftly repairs $n's $p without hesitation.", TRUE, ch, obj, 0,TO_ROOM);
    act("The blacksmith repairs $p for you.", TRUE, ch, obj, 0, TO_CHAR);
    GET_GOLD(ch) -= GET_OBJ_COST(obj)/5;
    GET_OBJ_STATUS(obj) = 100;
    return 1;
  }
  return 0;
} 
