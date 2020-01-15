/******************************************************************************
 * $Id: act.informative.c,v 1.78 2004/11/24 09:49:04 fang Exp $
 *   
 * File: act.informative.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD
 * Usage: Player-level commands of an informative nature
 *
 * All rights reserved.  See license.doc for complete information.
 *
 * Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University
 * CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.
 * 
 *   $Log: act.informative.c,v $
 *   Revision 1.78  2004/11/24 09:49:04  fang
 *   Bug fix in print_score
 *
 *   Revision 1.77  2003/12/16 02:47:44  fang
 *   Tiek: Obscure now works for who -l and who -h
 *
 *   Revision 1.76  2003/12/08 02:45:35  fang
 *   Tiek: Fixed some typos and grammatical errors
 *
 *   Revision 1.75  2003/12/02 08:15:36  fang
 *   Cappen: Added mycolor to items displayed by the toggle command
 *
 *   Revision 1.74  2003/10/30 04:36:02  fang
 *   fixed bug with do_test
 *
 *   Revision 1.73  2003/08/14 03:15:03  fang
 *   Added nopray command.
 *
 *   Revision 1.72  2003/08/08 03:37:20  fang
 *   And now the MUD has winter.
 *
 *   Revision 1.71  2003/06/24 09:07:47  fang
 *   No un-closed multi-line string literals please.
 *
 *   Revision 1.70  2003/06/17 10:30:05  fang
 *
 *   Removed a new set of "externs" from source files. For fucks sake people,
 *   let's keep those externs where they belong, in an apropriate header, ok?
 *
 *   Revision 1.69  2003/06/12 07:22:13  fang
 *   Changed testing for channeling ability as per HGL request. Fixed Room Seal to work as originally intended. Commented out ignorelist. Will fully remove once certain no problems will arise
 *
 *   Revision 1.68  2003/06/11 09:53:17  fang
 *
 *   Enabled dynvars in mobdescs/exdescs and obj exdescs.
 *
 *   Revision 1.67  2003/05/12 09:08:33  fang
 *   Enabled dynvars for room descs/xdescs
 *
 *   Revision 1.66  2003/05/11 06:15:59  fang
 *   Added wset to xlog and fixed a typo in the text message. --Tzeentch
 *
 *   Revision 1.65  2003/05/11 03:05:11  fang
 *   Fixed do_weather, added wset to mcl. --Tzeentch.
 *
 *   Revision 1.64  2003/05/09 08:09:27  fang
 *   Added coder room and weather status info to the 'weather' command. --Tzeentch
 *
 *   Revision 1.63  2003/03/26 14:53:19  fang
 *   Minor stuff fixed...
 *
 *   Revision 1.62  2003/03/01 07:17:05  fang
 *   Fisch: Multiflag for authorized multies
 *
 *   Revision 1.61  2003/02/04 08:45:23  fang
 *   Fixed bug in deletion of entries in ignore/notify lists. Pound can no longer be used in peaceful rooms
 *
 *   Revision 1.60  2003/01/20 07:49:26  fang
 *   *** empty log message ***
 *
 *   Revision 1.59  2003/01/10 00:13:09  fang
 *   fisch: modified header
 *
 *   Revision 1.58  2003/01/10 00:12:29  fang
 *   fisch: added header
 * *********************************************************************** */
#include "constants.h"
#include "config.h"
#include "act.h"
#include "conf.h"
#include "sysdep.h"
#include "spec_procs.h"
#include "class.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "spells.h"
#include "screen.h"
#include "olc.h"
#include "handler.h"
#include "color.h"
#include "guild.h"
#include "guild_parser.h"
#include "xmlcharsave.h"
#include "dg_scripts.h"
#include "retainer.h"
#include "boards.h"
#include "dynvars.h"
#include "prefight.h"

ACMD(do_inform)
{
  skip_spaces(&argument);
  if (!*argument) {
    send_to_char("Huh?\r\n",ch);
    return;
  }
  sprintf(buf, "$n types ---> %s\r\n", argument);
  act(buf, TRUE, ch, 0, 0, TO_ROOM);
  sprintf(buf, "You type ---> %s\r\n", argument);
  send_to_char(buf, ch);
  command_interpreter(ch,argument);
}

ACMD(do_lweapon){
  struct obj_data *obj;
  struct char_data *vict;

  if (!*argument){				/* If there is no argument with the lweapon command */
    send_to_char("Whose weapon are you trying to look at?\r\n", ch);
    return;
  }
  one_argument(argument,buf); 			/* Take the ONE WORD argument and place in buf */
  if (!(vict = get_char_room_vis(ch, buf))){	/* If the buf char is not visible to ch or not in room */
    send_to_char("They aren't in the room, so sorry.\r\n", ch);
    return;
  }
  if (!(obj = GET_EQ(vict, WEAR_WIELD_R)))
    if (!(obj = GET_EQ(vict, WEAR_WIELD_L))){
      send_to_char("They aren't wielding ANY weapons!\r\n", ch);
      return;
    }
  send_to_char(obj->action_description, ch);
  return;
}
/*
 * Info gathering about a Ter'angreal, i.e which spell it casts, 
 * at which level, with which mana-add, what kind of spell, expected 
 * arguments, etc...
 * 
 * 3% chance at burning out if failing. 
 */
ACMD(do_terinfo)
{
  int result,ist = 0,spell_num;
  struct obj_data *obj;

  skip_spaces(&argument);
  
  if (!GET_SKILL(ch,SKILL_TERINFO)) {
    send_to_char("Huh?!?\r\n",ch);   
    return;
  }
  if (cant_channel(ch)) {
    send_to_char("You must be able to channel to find info about Ter'angreals!\r\n",ch);
    return;
  }
  if (!*argument) {
    send_to_char("Get info about what?\r\n",ch);
    return;
  } 
  if (!(obj = get_obj_in_list_vis(ch,argument,ch->carrying))) {
    asend_to_char(ch,"You don't seem to have a \"%s\" whatever it is...\r\n",argument);
    return;
  }
  ist = IS_SET(GET_OBJ_EXTRA(obj),ITEM_TERANGREAL);
  asend_to_char(ch,"You weave flows of Spirit at %s to find info about it.\r\n",obj->short_description);
  act("$n peers intently at $p as if trying to determine something.",FALSE,ch,obj,0,TO_NOTCHANN);
  act("$n peers intently at $p as if trying to determine something.",FALSE,ch,obj,0,TO_OPUSERNSS);
  act("$n weaves delicate flows of Spirit at $p, testing for something.",FALSE,ch,obj,0,TO_OPUSERSS);

  result = generic_result_mod(ch,SKILL_APPRAISE,NULL,FALSE,-(30 - GET_INT(ch)));
  if ((TOTAL_FUCKUP == result) || (TOTAL_FAILURE == result && number(1,100) <= 3)) {
    send_to_char("You screwed up examining a Ter'angreal. You've burned yourself out!\r\n",ch);
    SET_BIT(GET_PERMANENT(ch),PRM_GENTLED);
    act("$n suddenly cluthches at $s head, screaming in terror.",FALSE,ch,0,0,TO_ROOM);
    return;
  }  else if (TOTAL_FAILURE == result) {
    asend_to_char(ch,"\"%s\" is not a Ter'angreal.\r\n",obj->short_description);
    return;
  }
  if (!ist) {
    asend_to_char(ch,"\"%s\" is not a Ter'angreal.\r\n",obj->short_description);
    return;
  }
  asend_to_char(ch,"This Ter'angreal will weave the \"\\c01%s\\c00\" weave, as if",spells[GET_OBJ_VAL(obj,0)]);
  asend_to_char(ch," someone at level %d had woven it with a ",GET_OBJ_VAL(obj,1));
  asend_to_char(ch,"mana add of %d.\r\n\r\n",GET_OBJ_VAL(obj,2));
  asend_to_char(ch,"And the spell is ");

  spell_num = GET_OBJ_VAL(obj,0);

  if (IS_SET(spell_info[spell_num].targets,IS_AGGRESSIVE))
    asend_to_char(ch,"agressive.\r\n");
  else
    asend_to_char(ch,"non-agressive.\r\n");

  if (IS_SET(spell_info[spell_num].targets,NO_TARGET) ||
      IS_SET(spell_info[spell_num].targets,ONLY_SELF) ||
      IS_SET(spell_info[spell_num].targets,CHAR_ROOM))
    asend_to_char(ch,"Doesn't take any arguments in usage.\r\n");
  else if (IS_SET(spell_info[spell_num].targets,OBJ_TARGET))
    asend_to_char(ch,"Takes an object name as argument.\r\n");
  else if (IS_SET(spell_info[spell_num].targets,DOOR_TARGET))
    asend_to_char(ch,"Takes a direction as argument.\r\n");
  else if (IS_SET(spell_info[spell_num].targets,SPELL_TARGET))
    asend_to_char(ch,"Takes a spell name (enclosed within '') as argument.\r\n");
  else if (IS_SET(spell_info[spell_num].targets,ROOM_TARGET))
    asend_to_char(ch,"Takes a room name (old or new syntax) as argument.\r\n");
  else 
    asend_to_char(ch,"Takes a character(mob) name as argument.\r\n");
}

/*
 * Test of ter'angreal. See if it's a ter'angreal or not, and which
 * gender can use it (if any restrictions. 
 * 1% chance of burning out if failing. 
 */
ACMD(do_tertest)
{
  int result,ist = 0,gender = -1;
  struct obj_data *obj;

  skip_spaces(&argument);

  if (!GET_SKILL(ch,SKILL_TERTEST)) {
    send_to_char("Huh?!?\r\n",ch);
    return;
  }
  if (cant_channel(ch)) {
    send_to_char("You must be able to channel to test Ter'angreals!\r\n",ch);
    return;
  }
  if (!*argument) {
    send_to_char("Test what?\r\n",ch);
    return;
  }
  if (!(obj = get_obj_in_list_vis(ch,argument,ch->carrying))) {
    asend_to_char(ch,"You don't seem to have a \"%s\" whatever it is...\r\n",argument);
    return;
  }
  ist = IS_SET(GET_OBJ_EXTRA(obj),ITEM_TERANGREAL);
  gender = (IS_SET(GET_OBJ_EXTRA(obj),ITEM_NOMALE) ? SEX_FEMALE : gender);
  gender = (IS_SET(GET_OBJ_EXTRA(obj),ITEM_NOFEMALE) ? SEX_MALE : gender);

  asend_to_char(ch,"You weave flows of Spirit at %s to test if it's a Ter'angreal or not.\r\n",obj->short_description);
  act("$n peers intently at $p as if trying to determine something.",FALSE,ch,obj,0,TO_NOTCHANN);
  act("$n peers intently at $p as if trying to determine something.",FALSE,ch,obj,0,TO_OPUSERNSS);
  act("$n weaves delicate flows of Spirit at $p, testing for something.",FALSE,ch,obj,0,TO_OPUSERSS);

  result = generic_result_mod(ch,SKILL_APPRAISE,NULL,FALSE,-(30 - GET_INT(ch)));
  if ((TOTAL_FUCKUP == result) || (TOTAL_FAILURE == result && number(1,100) == 1)) {
    send_to_char("You screwed up examining a Ter'angreal. You've burned yourself out!\r\n",ch);
    SET_BIT(GET_PERMANENT(ch),PRM_GENTLED);
    act("$n suddenly cluthches at $s head, screaming in terror.",FALSE,ch,0,0,TO_ROOM);
    return;
  } else if (TOTAL_FAILURE == result) {
    asend_to_char(ch,"\"%s\" is not a Ter'angreal.\r\n",obj->short_description);
    return;
  }
  if (!ist) {
    asend_to_char(ch,"\"%s\" is not a Ter'angreal.\r\n",obj->short_description);
    return;
  }
  else {
    asend_to_char(ch,"\"%s\" is a Ter'angreal",obj->short_description);
    if (-1 != gender) 
      asend_to_char(ch,", useable only by %ss!\r\n",(SEX_MALE == gender ? "male" : "female"));
    else
      asend_to_char(ch,".\r\n");
  }
}

ACMD(do_background)
{
  asend_to_char(ch, "%s\r\n", background);
}

ACMD(do_bmlist)
{
  register int i;

  if (!GET_BM(ch) && GET_LEVEL(ch) < LVL_IMMORT) {
    send_to_char("Only Blademasters can use this....\r\n", ch);
    return;
  }
  sprintf(buf, "These swordsman are also Blademasters'...\r\n"
          "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n\r\n");
  for (i = 0; i <= top_of_p_table; i++)
    if (player_table[i].bm && !(player_table[i].deleted))
      sprintf(buf, "%s%-30s %s\r\n", buf, CAP(player_table[i].name), blademastery[player_table[i].bm]);

  page_string(ch->desc, buf, 1);
}

ACMD(do_multilist)
{
  register int i;

  if( GET_LEVEL(ch) < LVL_IMMORT ) 
  {
    send_to_char( "Only Imms can use this....\r\n", ch);
    return;
  }
  sprintf(buf, "These users are allowed to Multi...\r\n"
                "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n\r\n");
  for (i = 0; i <= top_of_p_table; i++)
    if (player_table[i].multi && !(player_table[i].deleted))
      sprintf(buf, "%s%-30s\r\n", buf, CAP(player_table[i].name));

  page_string(ch->desc, buf, 1);
}

void show_obj_to_char(struct obj_data * object, struct char_data * ch,
            int mode)
{
  bool found;
  int a, counter = 0;
  struct char_data *i = NULL;
  struct obj_data *j = NULL;
  char buf2[MAX_STRING_LENGTH];

  *buf = '\0';
  if ((mode == 0) && object->description) {

    if (object->on_obj)
     return;

    strcpy(buf, object->description);

    if (GET_OBJ_TYPE(object) == ITEM_FURNITURE) {

     for (i = world[ch->in_room].people; i; i = i->next_in_room)
      if ((GET_SEAT(i) == object) && (CAN_SEE(ch, i))) {
            sprintf(buf2, " (%s)", GET_NAME(i));
            strcat(buf, buf2);
          }

     for (j = world[ch->in_room].contents; j; j = j->next_content)
      if (j->on_obj == object) 
       counter++;
      
     if (counter)
      strcat(buf, " [+]");
      
    }
     
  }
  else if (object->short_description && ((mode == 1) ||
                 (mode == 2) || (mode == 3) || (mode == 4))) {
    strcpy(buf, object->short_description);
    strcat(buf, " ");
      if ((GET_OBJ_TYPE(object) == ITEM_ARMOR) ||
        (GET_OBJ_TYPE(object) == ITEM_WEAPON)) 
        strcat(buf, obj_status[(GET_OBJ_STATUS(object)+19)/20]);
  } else if (mode == 5) {
    if (GET_OBJ_TYPE(object) == ITEM_NOTE) {
      if (object->action_description) {
    strcpy(buf, "There is something written upon it:\r\n\r\n");
    send_to_char(buf, ch);
        strcpy(buf, object->action_description);
    if (GET_OBJ_VAL(object, 0)){
      for (a = 0; buf[a] != '\0'; a++)
            if ((buf[a] == ' ') && (number(0, 100) > GET_SKILL(ch, SKILL_TROLLOC + GET_OBJ_VAL(object, 0) - 1)))
             for (a++; (buf[a] != ' ') && (buf[a] != '\0'); a++)
               buf[a] = 'a' + number(0, 20);
    }
    page_string(ch->desc, buf, 1);
    send_to_retainer_owner(buf, ch);
      } else
    act("It's blank.", FALSE, ch, 0, 0, TO_CHAR);
      return;
    } else if (GET_OBJ_TYPE(object) == ITEM_FURNITURE) {
     for (j = world[ch->in_room].contents; j; j = j->next_content)
      if (j->on_obj == object) {
       counter++;
       sprintf(buf2, "There's a %s %s it.\r\n", j->short_description, furniture_usages[GET_OBJ_VAL(object, 1)]);
       send_to_char(buf2, ch);
      }
     if (!counter)
      strcpy(buf, "You see nothing special..");
    } else if (GET_OBJ_TYPE(object) != ITEM_DRINKCON) {
      strcpy(buf, "You see nothing special..");
    } else			/* ITEM_TYPE == ITEM_DRINKCON||FOUNTAIN */
      strcpy(buf, "It looks like a drink container.");
  }
  if (mode != 3) {
    found = FALSE;
    if (IS_OBJ_STAT(object, ITEM_INVISIBLE)) {
      strcat(buf, " (invisible)");
      found = TRUE;
    }
  }
  strcat(buf, "\r\n"); 
  page_string(ch->desc, buf, 1);
  send_to_retainer_owner(buf, ch);
}


int list_obj_to_char(struct obj_data * list, struct char_data * ch, int mode, bool show)
{
  const int max_num_of_objs = 255;
  struct obj_data *i, *ilist[max_num_of_objs];
  int n = 0, k = 0, amount[max_num_of_objs], found = 0;
  char text_buf[50];
  
  for (n = 0; n < max_num_of_objs; n++)  /* Clear the pointer list and the counter list for safety */
  {
    ilist[n] = NULL;
    amount[n] = 0;
  }       
  
  if (list)
    ilist[0] = list;
    
  for (i = list; i; i = i->next_content) /* Check through the given list of items */
  {
    if (CAN_SEE_OBJ(ch, i) && !(i->on_obj)) /* Only show what he can see */
    {
      found = FALSE;
      for (k = 0 ; ilist[k]; k++)  /* Check the list of -different- vnums for the one just found */
      {
        if ((GET_OBJ_VNUM(ilist[k]) == GET_OBJ_VNUM(i)) && !(strcmp(ilist[k]->short_description,i->short_description)))  /* Ok, here's another one with */
        {						/* the same vnum AND the same name!*/
          amount[k] ++;     /* Add one to the counter of this vnum */
          found = TRUE;
        }
      }
      if (!found)   /* This is a new vnum, let's add it to the list of different vnums */
      {
        k = 0;
        while (amount[k])
          k++;
        ilist[k] = i;
        amount[k] ++;
      }
    } 
  }
  for (k = 0; amount[k]; k++) /* Show the new, internal list of different items */
  {
    if (amount[k] > 1)
    {
      sprintf(text_buf,"%s[%s%d%s] ", CCNRM(ch, C_NRM), CCCYN(ch, C_NRM), amount[k], CCNRM(ch, C_NRM));
      send_to_char(text_buf, ch);
    }
    show_obj_to_char(ilist[k], ch, mode);
  }

  if (!amount[0] && show)  {
    send_to_char(" Nothing.\r\n", ch);
    return 0;
  }

  for (k = 0; k <max_num_of_objs; k ++)  /* Clear the lists again */
  {
    amount[k] = 0;
    ilist[k] = NULL;
  }

  return 1;
}


void diag_char_to_char(struct char_data * i, struct char_data * ch)
{
  int percent;

  if (GET_MAX_HIT(i) > 0)
    percent = (100 * GET_HIT(i)) / GET_MAX_HIT(i);
  else
    percent = -1;		/* How could MAX_HIT be < 1?? */

  strcpy(buf, PERS(i, ch));
  CAP(buf);

  if (percent >= 100)
    strcat(buf, " \\c02is in excellent condition.\\c00\r\n");
  else if (percent >= 90)
    strcat(buf, " \\c02has a few scratches.\\c00\r\n");
  else if (percent >= 75)
    strcat(buf, " \\c09has some small wounds and bruises.\\c00\r\n");
  else if (percent >= 50)
    strcat(buf, " \\c09has quite a few wounds.\\c00\r\n");
  else if (percent >= 30)
    strcat(buf, " \\c03has some big nasty wounds and scratches.\\c00\r\n");
  else if (percent >= 15)
    strcat(buf, " \\c08looks pretty hurt.\\c00\r\n");
  else if (percent >= 0)
    strcat(buf, " \\c08is in awful condition.\\c00\r\n");
  else
    strcat(buf, " \\c01is bleeding awfully from big wounds.\\c00\r\n");

  send_to_char(buf, ch);
}

void do_affect_look(struct char_data *ch,struct char_data *vict)
{
  char tmp[200],tmp2[200];
  bool tp   = GET_SKILL(ch,SKILL_TGRASP),found = 0;
  int power;
  struct affected_type *aff;
  char *powers[] = { "!ERROR!","the True Power","Saidin","Saidar" };
  

  if (GET_SEX(ch) == SEX_MALE) power = SAIDIN;
  else                         power = SAIDAR;
 
  sprintf(tmp,"You can see the following affecting %s:\r\n",GET_NAME(vict));
  send_to_char(tmp,ch);

  for (aff = vict->affected; aff != NULL; aff = aff->next) {
    if( ( ( aff->sex == power )
    || ( ( aff->sex == TRUE_POWER )
         && tp) )
    && ( aff->type >= 1 )
    && ( aff->type <= MAX_SPELLS ) )
    {
      found = 1;
      if (GET_SKILL(ch,aff->type) && ((aff->sex == power) || ((aff->sex == TRUE_POWER)&&tp)))
        sprintf(tmp2,"%s woven with ",spells[aff->type]);
      else
        sprintf(tmp2,"A weave you don't recognize woven with ");

      strcat(tmp2,powers[aff->sex]);
      strcat(tmp2,".\r\n");
      send_to_char(tmp2,ch);
    }
  }  
  if (!found)
    send_to_char("Nothing you can sense anyway.\r\n",ch);
}

static void chan_strength_test(struct char_data * ch, struct char_data *
victim)
{

  int target_strength = 0, ch_strength = 0;

  if (GET_SEX(ch) == SEX_FEMALE) {
    if (GET_SKILL(ch, SKILL_RELEASE) < 85)
      return;

    if (GET_SEX(victim) != SEX_FEMALE) 
      return;

   if (!(IS_NPC(victim)))
    if (cant_channel(victim) || IS_AFFECTED2(victim,AFF2_MASKED)){
      send_to_char("She can't channel.\r\n", ch);
      return;
    }
  }

  if (GET_SEX(ch) == SEX_MALE) {
    if (GET_SKILL(ch, SKILL_RELEASE) < 85)
      return;

    if (GET_SEX(victim) != SEX_MALE)
      return;

    if (!(IS_SET(PRF_FLAGS(victim), PRF_GRASPING)) && !(IS_NPC(victim)))
      return;
  }

  if (IS_NPC(victim))  {
    if (MOB_FLAGGED(victim,MOB_CHANNELER)) {
      if (GET_LEVEL(victim) > GET_LEVEL(ch)) {
        send_to_char("This mob is stronger in the Power than you...\r\n",ch);
      }
      else {
        send_to_char("You seem to be stronger in the Power than this mob...\r\n",ch);
      }
      return;
    } else {
        send_to_char("This mob can't channel.\r\n", ch);
        return;
    }
  } 

  sprintf(buf,"%s can channel and is ", GET_NAME(victim));
  if (GET_LEVEL(victim)>= LVL_IMMORT)
    strcat(buf, "infinitely stronger than you. Don't even think about it.");
  else
  {   
    target_strength = GET_AIR(victim) + GET_EARTH(victim) +
GET_FIRE(victim) + 
    GET_SPIRIT(victim) + GET_WATER(victim);
    ch_strength = GET_AIR(ch) + GET_EARTH(ch) + GET_FIRE(ch) + 
    GET_SPIRIT(ch) + GET_WATER(ch);
    if (target_strength <= 5)
      strcat(buf, "very weak indeed. Perhaps a beginner.");
    else if (target_strength <= ch_strength/10)
      strcat(buf, "far weaker than you.");
    else if (target_strength <= ch_strength*0.3)
      strcat(buf, "a good deal weaker than you.");
    else if (target_strength <= ch_strength*0.45)
      strcat(buf, "weaker than you.");
    else if (target_strength <= ch_strength*0.55)
      strcat(buf, "about half as strong as you.");
    else if (target_strength <= ch_strength*0.7)
      strcat(buf, "a little weaker than you.");
    else if (target_strength <= ch_strength*0.85)
      strcat(buf, "slightly weaker than you.");
    else if (target_strength <= ch_strength*0.95)
      strcat(buf, "almost as strong as you.");
    else if (target_strength <= ch_strength*1.05)
      strcat(buf, "about the same in strength.");
    else if (target_strength <= ch_strength*1.15)
      strcat(buf, "a tiny bit stronger than you.");
    else if (target_strength <= ch_strength*1.45)
      strcat(buf, "slightly stronger than you.");
    else if (target_strength <= ch_strength*1.80)
      strcat(buf, "a little stronger than you.");
    else if (target_strength <= ch_strength*2.20)
      strcat(buf, "about twice as strong as you.");
    else if (target_strength <= ch_strength*3)
      strcat(buf, "stronger than you.");
    else if (target_strength <= ch_strength*10)
      strcat(buf, "a good deal stronger than you.");
    else 
      strcat(buf,"far stronger than you!");
  }
  strcat(buf,"\r\n");
  send_to_char(buf, ch);
}

void look_at_char(struct char_data * i, struct char_data * ch)
{
#define POS_DUALWIELD 19
  int j, found;

  if (IS_SET(GET_DFINTR(i),DRF_GREYMAN)) {
    if (!PRF_FLAGGED(ch, PRF_HOLYLIGHT)) {
      send_to_char("\\c05This character is a greyman.  Because holylight is not an 'in character'\r\n"
       "flag, you cannot ICly see this character, and therefor, you may\r\n"
       "not reveal their presence to other characters, or react to them in any way.\\c00\r\n", ch);
    }
    else {
      return;
    }
  }
  if (!IS_NPC(i)) {
    if (GET_DESC(i))
      send_to_char(GET_DESC(i), ch);
    else
     act("You see nothing special about $m.", FALSE, i, 0, ch, TO_VICT);      
  }
  else {
    char *p;
    if (i->player.description) {
      send_to_char(p = char_look_at_char(ch, i, i->player.description), ch);
      free(p);
    }
    else
      act("You see nothing special about $m.", FALSE, i, 0, ch, TO_VICT);
  }
  diag_char_to_char(i, ch);
  if (!IS_NPC(i) && IS_SET(GET_PERMANENT(i), PRM_CRIPPLED)) 
    act("$n seems to be crippled in some way, poor creature.", FALSE, i, 0, ch, TO_VICT);
  if (!IS_NPC(i) && IS_SET(GET_PERMANENT(i), PRM_BLIND)) 
    act("$n seems to be blind.", FALSE, i, 0, ch, TO_VICT);
  if (!IS_NPC(i) && IS_SET(GET_PERMANENT(i), PRM_RH_CUT)) 
    act("$n's right hand is cut off.", FALSE, i, 0, ch, TO_VICT);
  if (!IS_NPC(i) && IS_SET(GET_PERMANENT(i), PRM_LH_CUT)) 
    act("$n's left hand is cut off.", FALSE, i, 0, ch, TO_VICT);  
  if (IS_BRANDED(i)) {
    sprintf(buf, "\\c08%s is branded as a Darkfriend!!\r\n\\c00", GET_NAME(i));
    send_to_char(buf, ch);
  }
  chan_strength_test(ch, i);
  found = FALSE;
  for (j = 0; !found && j < NUM_WEARS; j++)
    if (GET_EQ(i, j) && CAN_SEE_OBJ(ch, GET_EQ(i, j)))
      found = TRUE;

  if (found) {
    act("\r\n$n is using:", FALSE, i, 0, ch, TO_VICT);
    for (j = 0; j < NUM_WEARS; j++)
      if (GET_EQ(i, j) && CAN_SEE_OBJ(ch, GET_EQ(i, j))) {
        if (CAN_WEAR(GET_EQ(i,j),ITEM_WEAR_DUALWIELD)) 
          send_to_char(where[POS_DUALWIELD],ch);
        else
      send_to_char(where[j], ch);
    show_obj_to_char(GET_EQ(i, j), ch, 1);
      }
  }
  if (ch != i && (GET_CLASS(ch) == CLASS_THIEF || GET_LEVEL(ch) >= LVL_IMMORT)) {
    found = FALSE;
    act("\r\nYou attempt to peek at $s inventory:", FALSE, i, 0, ch, TO_VICT);
    if (NULL == i->carrying) {
      found = FALSE;
    }
    else if ((number(0, 20) < GET_LEVEL(ch))||(GET_LEVEL(ch) >= LVL_IMMORT)) {
      found = list_obj_to_char(i->carrying,ch,1,0);
      found = TRUE;
    }
    else  {
      found = FALSE;
    }  
    if (!found)
      send_to_char("You can't see anything.\r\n", ch);
  }
  if ((i->affected) && (!cant_channel(ch)||GET_SKILL(ch,SKILL_TGRASP))) 
    do_affect_look(ch,i);
#undef POS_DUALWIELD
}

void list_one_char(struct char_data * i, struct char_data * ch)
{
  char buf2[MAX_STRING_LENGTH];
  char *positions[] = {
    " is lying here, dead.",
    " is lying here, mortally wounded.",
    " is lying here, incapacitated.",
    " is lying here, stunned.",
    " is sleeping here.",
    " is resting here.",
    " is sitting here.",
    " !FIGHTING!",
    " is standing here.",
    " is sitting on a mount."
  };

  char *positions_on_furniture[] = {
    " is lying here, dead.",
    " is lying here, mortally wounded.",
    " is lying here, incapacitated.",
    " is lying here, stunned.",
    " is sleeping %s %s here.",   
    " is resting %s %s here.",
    " is sitting %s %s here.",
    " !FIGHTING!",
    " is standing here.",
    " is sitting on a mount."
  };

  if (!IS_NPC(i))
    {
    sprintf( buf, "%s\\c00", (IS_SET(PRF_FLAGS(i),PRF_IC) ? "" : "\\c06* ") );
    send_to_char(buf,ch);
    }
  else
  if (IS_NPC(i) && i->player.long_descr && GET_POS(i) == GET_DEFAULT_POS(i)) {
    if (IS_AFFECTED(i, AFF_INVISIBLE))
      strcpy(buf, "* ");
    else
      *buf = '\0';

    strcat(buf, i->player.long_descr);
    send_to_char(buf,ch);

    if (IS_AFFECTED(i, AFF_BLIND) || IS_SET(GET_PERMANENT(ch), PRM_BLIND))
      act("...$e is groping around blindly!", FALSE, i, 0, ch, TO_VICT);

    return;
  }
  strcpy(buf,"\0");
  if (GET_INVIS_LEV(i))
    strcat(buf, "(invis)");
  if (IS_AFFECTED(i, AFF_INVISIBLE))
    strcat(buf, "(invisible) ");
  if (IS_AFFECTED(i, AFF_HIDE))
    strcat(buf, "(hidden) ");
  if (IS_AFFECTED(i, AFF_VEILED))
    strcat(buf, "(veiled) ");
  if (!IS_NPC(i) && !i->desc)
    strcat(buf, "(linkless) ");
  if (PLR_FLAGGED(i, PLR_WRITING))
    strcat(buf, "(writing) ");
  if (PLR_FLAGGED(i, PLR_MAILING))
    strcat(buf, "(mailing) ");
  if (PRF2_FLAGGED(i, PRF2_AFW))
    strcat(buf,AFW_LOOK);
  if (PRF_FLAGGED(i, PRF_AFK))
    strcat(buf,AFK_LOOK);
  if (IS_SET(PRF_FLAGS(i),PRF_GRASPING) && can_channel_same_sex(i,ch) && (GET_SEX(i) == SEX_MALE))
    strcat(buf,"(menacing aura) ");
  if (IS_SET(PRF_FLAGS(i),PRF_GRASPING) && can_channel_same_sex(i,ch) && (GET_SEX(i) == SEX_FEMALE))
    strcat(buf,"(golden aura) ");
  if (IS_SET(PRF_FLAGS(i),PRF_TGRASP) && GET_SKILL(ch,SKILL_TGRASP))
    strcat(buf,"(dark aura) ");
  if (IS_AFFECTED(i, AFF_DREAMING) && IS_AFFECTED2(i, AFF2_TARBLUR))
    strcat(buf,"&g(&Cblurred&g)&n ");
    



   if (IS_NPC(i)) {
     strcpy(buf,i->player.short_descr);
     CAP(buf);
   }
   else
     sprintf(buf,"%s%s %s",buf,GET_NAME(i),GET_TITLE(i));

  if (GET_POS(i) != POS_FIGHTING) {
    if (!IS_NPC(i) && GET_DOING(i))
      strcat(buf, GET_DOING(i));
    else {
     if (!GET_SEAT(i))
      strcat(buf, positions[(int) GET_POS(i)]);  
     if (GET_SEAT(i)) {
      sprintf(buf2, positions_on_furniture[(int) GET_POS(i)], 
              furniture_usages[GET_OBJ_VAL(GET_SEAT(i), 1)], GET_SEAT(i)->short_description);
      strcat(buf, buf2);
     }
    }
  }
  else {
    if (FIGHTING(i)) {
      strcat(buf, " is here, fighting ");
      if (FIGHTING(i) == ch)
    strcat(buf, "YOU!");
      else {
    if (i->in_room == FIGHTING(i)->in_room)
      strcat(buf, PERS(FIGHTING(i), ch));
    else
      strcat(buf, "someone who has already left");
    strcat(buf, "!");
      }
    } else			/* NIL fighting pointer */
      strcat(buf, " is here struggling with thin air.");
  }

  strcat(buf, "\r\n");
  send_to_char(buf, ch);
}



void list_char_to_char(struct char_data * list, struct char_data * ch)
{
  struct char_data *i,*ilist[300];
  int n= 0, k = 0, amount[300],found = 0, num_smelled = 0;
  char tmp[600];

  for (n = 0; n < 300; n++) {
    ilist[n]  = NULL;
    amount[n] = 0;
  }

  for (i = list; i; i = i->next_in_room) 
  {
    if ((ch != i) && IS_NPC(i)) 
    {
      if (CAN_SEE(ch,i) && (GET_DEST(ch) == GET_DEST(i))) 
      {
        found = FALSE;
        for (k = 0; ilist[k]; k++) 
        {
          if (!is_retainer(i) && (GET_MOB_VNUM(i) == GET_MOB_VNUM(ilist[k])) && (GET_POS(i) == GET_POS(ilist[k]))) 
          {
            amount[k]++;
            found = TRUE;
          }
        }
        if (!found) 
        {
          k = 0; 
          while(amount[k]) 
            k++;
          ilist[k] = i;
          amount[k]++;
        }
      }
      else if (!IS_NPC(ch) && GET_WOLFKIN(ch))
        num_smelled ++;
    }
  }

  for (k = 0; amount[k]; k++) 
  {
    if (amount[k] > 1) 
    {
      sprintf(tmp,"%s[%s%d%s] ",CCNRM(ch, C_NRM), CCCYN(ch, C_NRM),amount[k],CCNRM(ch, C_NRM));
      send_to_char(tmp,ch);
    }
    list_one_char(ilist[k],ch);
  }

  if (!IS_NPC(ch))
  {
    for (i = list; i; i = i->next_in_room) 
    {
      if ((ch != i) && !IS_NPC(i)) 
      {
        if ( CAN_SEE(ch, i) && (!IS_SET(GET_DFINTR(i),DRF_GREYMAN) ||
           PRF_FLAGGED(ch, PRF_HOLYLIGHT)) && (GET_DEST(ch) == GET_DEST(i))) 
        {
          if (IS_SET(GET_DFINTR(i), DRF_GREYMAN))
            send_to_char("(Greyman)  ", ch);
          list_one_char(i, ch);
        }
        else if (IS_DARK(ch->in_room) && !CAN_SEE_IN_DARK(ch) &&
          IS_AFFECTED(i, AFF_INFRAVISION))
        send_to_char("You see a pair of glowing red eyes looking your way.\r\n", ch);
      }
    }
    if (num_smelled)
      asend_to_char(ch, "You smell %d individuals somewhere in this area.\r\n", num_smelled);
  }
  else if (is_retainer(ch))
  {
    for (i = list; i; i = i->next_in_room) 
      if (!IS_NPC(i) && CAN_SEE(ch, i))
        list_one_char(i, ch); 
  }
}


void do_auto_exits(struct char_data * ch)
{
  int door;
  char *abbr[] =
  {
   "n",
   "e",
   "s",
   "w",
   "u",
   "d",
   "ne",
   "se",
   "sw",
   "nw"
  };
  
  *buf = '\0';
  
  for (door = 0; door < NUM_OF_DIRS; door++) {
    if (EXIT(ch, door) && EXIT(ch, door)->to_room != NOWHERE) {
       if (IS_SET(EXIT(ch, door)->exit_info, EX_CLOSED)) {
         if (IS_SET(EXIT(ch, door)->exit_info, EX_HIDDEN)) { /* Closed and hidden */
           if (PRF_FLAGGED(ch, PRF_HOLYLIGHT)) /* Only imms can see */
             sprintf(buf, "%s*%s* ", buf, abbr[door]);
           else
             continue;
         }
         else  /* Just closed */
           sprintf(buf, "%s-%s- ", buf, abbr[door]);
       } else   /* Not closed or hidden */
       sprintf(buf, "%s%s ", buf, abbr[door]); 
    }
  }
  sprintf(buf2, "%s[ Exits: %s]%s\r\n", CCCYN(ch, C_NRM),
      *buf ? buf : "None! ", CCNRM(ch, C_NRM));

  send_to_char(buf2, ch);
}


ACMD(do_exits)
{
  int door;

  *buf = '\0';

  if (IS_AFFECTED(ch, AFF_BLIND) || IS_SET(GET_PERMANENT(ch), PRM_BLIND)) {
    send_to_char("You can't see a damned thing, you're blind!\r\n", ch);
    return;
  }
  for (door = 0; door < NUM_OF_DIRS; door++)
    if (EXIT(ch, door) && EXIT(ch, door)->to_room != NOWHERE &&
    !IS_SET(EXIT(ch, door)->exit_info, EX_CLOSED)) {
      if (GET_LEVEL(ch) >= LVL_IMMORT)
    sprintf(buf2, "%-5s - [%5d] %s\r\n", dirs[door],
        world[EXIT(ch, door)->to_room].number,
        world[EXIT(ch, door)->to_room].name);
      else {
    sprintf(buf2, "%-5s - ", dirs[door]);
    if (IS_DARK(EXIT(ch, door)->to_room) && !CAN_SEE_IN_DARK(ch) && !GET_WOLFKIN(ch))
      strcat(buf2, "Too dark to tell\r\n");
    else {
      strcat(buf2, world[EXIT(ch, door)->to_room].name);
      strcat(buf2, "\r\n");
    }
      }
      strcat(buf, CAP(buf2));
    }
  send_to_char("Obvious exits:\r\n", ch);

  if (*buf)
    send_to_char(buf, ch);
  else
    send_to_char(" None.\r\n", ch);
}

void do_affroom_look(struct char_data *ch,struct affected_type *af)
{
  char tmp2[8000];
  bool tp   = GET_SKILL(ch,SKILL_TGRASP);
  int power,found = 0;
  struct affected_type *aff;
  char *powers[] = { "!ERROR!","the true power","Saidin","Saidar" };

  if (ROOM_AFFECTED(ch->in_room,RAFF_FORTIFY)) {
    send_to_char("Someone has built -\\c01HUGE\\c00- fortifications here.\r\n",ch);
  }

  if (IS_NPC(ch) || cant_channel(ch))
  {
    return;
  }
  if (GET_SEX(ch) == SEX_MALE) power = SAIDIN;
  else                         power = SAIDAR;

  sprintf(tmp2, "You can see the following affecting the room:\r\n");
  for (aff = af; aff != NULL; aff = aff->next) {
    if(((aff->sex == power)||((aff->sex == TRUE_POWER) && tp))&&(aff->type >= 1) && (aff->type <= MAX_SPELLS )) {
      found = 1;
      if (GET_SKILL(ch,aff->type) && ((aff->sex == power) || ((aff->sex == TRUE_POWER)&&tp)))
        sprintf(tmp2,"%s%s woven with ",tmp2,spells[aff->type]);
      else
        sprintf(tmp2,"%sA weave you don't recognize woven with ",tmp2);

      strcat(tmp2,powers[aff->sex]);
      strcat(tmp2,".\r\n");  
    }
  }
  if (found) 
    send_to_char(tmp2,ch);
}

void look_at_room(struct char_data * ch, int ignore_brief)
{
  struct char_data *ppl;
  struct room_data *rm = &world[ch->in_room];
  char *p; 

  if (IS_DARK(ch->in_room))
  { 
    if (IS_NPC(ch))
    {
      send_to_char("It is pitch black...\r\n", ch);
      return;
    } 

    if(!CAN_SEE_IN_DARK(ch) && !GET_WOLFKIN(ch)) 
    {
      send_to_char("It is pitch black...\r\n", ch);
      return;
    } else if (IS_AFFECTED(ch, AFF_BLIND) || IS_SET(GET_PERMANENT(ch), PRM_BLIND)) 
    {
      send_to_char("You see nothing but infinite darkness...\r\n", ch);
      return;
    }
  }
  send_to_char(CCCYN(ch, C_NRM), ch);
  if (PRF_FLAGGED(ch, PRF_ROOMFLAGS)) 
  {
    sprintbit((long) ROOM_FLAGS(ch->in_room), room_bits, buf);
    sprintf(buf2, "[%d] %s [ %s]", world[ch->in_room].number,
        world[ch->in_room].name, buf);
    if (ROOM_TEMP_FLAGS(ch->in_room)) 
    {
      sprintbit((long) ROOM_TEMP_FLAGS(ch->in_room), room_bits, buf);
      sprintf(buf2, "%s [TEMP: %s]", buf2, buf);
    }
    send_to_char(buf2, ch);
  } else
    send_to_char(world[ch->in_room].name, ch);

  send_to_char(CCNRM(ch, C_NRM), ch);
  send_to_char("\r\n", ch);

  if (!PRF_FLAGGED(ch, PRF_BRIEF) || ignore_brief) {
    send_to_char((p = char_look_at_room(ch,world[ch->in_room].description)), ch);
    free(p);
  }

  /* autoexits */
  if (PRF_FLAGGED(ch, PRF_AUTOEXIT) || is_retainer(ch))
    do_auto_exits(ch);

  /* spells affecting the room, only list TP affections to TP users */
  if ((rm->aff))
    do_affroom_look(ch,rm->aff);

  /* Look for people with SKILL_CAMP active*/
  for (ppl = world [ch->in_room].people; ppl; ppl = ppl->next_in_room) 
  {
    if (affected_by_spell(ppl, SKILL_CAMP)) 
    {
      send_to_char("A camp with a warming campfire is here.\r\n", ch);
      break;
    }
  }
  if (ROOM_AFFECTED(ch->in_room, RAFF_FOG))
    send_to_char("The entire area is covered by a thick fog.\r\n", ch);

 /* now list characters & objects */
  if( ROOM_NO_SCENE( ch->in_room ) )
  {
    if( ROOM_NO_SCENE(ch->in_room)->text )
    {
      send_to_char( CCMAG(ch,C_NRM),ch);
      send_to_char( ROOM_NO_SCENE( ch->in_room )->text, ch );
      send_to_char( "\r\n", ch );
    }
    else
      CLEAR_SCENE( ROOM_NO_SCENE( ch->in_room ) );
  }
  send_to_char(CCGRN(ch, C_NRM), ch);
  list_obj_to_char(world[ch->in_room].contents, ch, 0, FALSE);
  send_to_char(CCYEL(ch, C_NRM), ch);
  list_char_to_char(world[ch->in_room].people, ch);
  send_to_char(CCNRM(ch, C_NRM), ch);
}



void look_in_direction(struct char_data * ch, int dir)
{
  if (EXIT(ch, dir)) {
    if (EXIT(ch, dir)->general_description)
      send_to_char(EXIT(ch, dir)->general_description, ch);
    else
      send_to_char("You see nothing special.\r\n", ch);

    if (IS_SET(EXIT(ch, dir)->exit_info, EX_CLOSED) && EXIT(ch, dir)->keyword) {
      sprintf(buf, "The %s is closed.\r\n", fname(EXIT(ch, dir)->keyword));
      send_to_char(buf, ch);
    } else if (IS_SET(EXIT(ch, dir)->exit_info, EX_ISDOOR) && EXIT(ch, dir)->keyword) {
      sprintf(buf, "The %s is open.\r\n", fname(EXIT(ch, dir)->keyword));
      send_to_char(buf, ch);
    }
  } else
    send_to_char("Nothing special there...\r\n", ch);
}



void look_in_obj(struct char_data * ch, char *arg)
{
  struct obj_data *obj = NULL;
  struct char_data *dummy = NULL;
  int amt, bits;

  if (!arg || !*arg)
    send_to_char("Look in what?\r\n", ch);
  else if (!(bits = generic_find(arg, FIND_OBJ_INV | FIND_OBJ_ROOM |
                 FIND_OBJ_EQUIP, ch, &dummy, &obj))) {
    sprintf(buf, "There doesn't seem to be %s %s here.\r\n", AN(arg), arg);
    send_to_char(buf, ch);
  } else if ((GET_OBJ_TYPE(obj) != ITEM_DRINKCON) &&
         (GET_OBJ_TYPE(obj) != ITEM_FOUNTAIN) &&
         (GET_OBJ_TYPE(obj) != ITEM_CONTAINER))
    send_to_char("There's nothing inside that!\r\n", ch);
  else {
    if (GET_OBJ_TYPE(obj) == ITEM_CONTAINER) {
      if (IS_SET(GET_OBJ_VAL(obj, 1), CONT_CLOSED))
    send_to_char("It is closed.\r\n", ch);
      else {
    send_to_char(fname(obj->name), ch);
    switch (bits) {
    case (FIND_OBJ_INV):
      send_to_char(" (carried): \r\n", ch);
      break;
    case (FIND_OBJ_ROOM):
      send_to_char(" (here): \r\n", ch);
      break;
    case (FIND_OBJ_EQUIP):
      send_to_char(" (used): \r\n", ch);
      break;
    }

    list_obj_to_char(obj->contains, ch, 2, TRUE);
      }
    } else {		/* item must be a fountain or drink container */
      if (GET_OBJ_VAL(obj, 1) <= 0)
    send_to_char("It is empty.\r\n", ch);
      else {
    if (GET_OBJ_VAL(obj,0) <= 0 || GET_OBJ_VAL(obj,1)>GET_OBJ_VAL(obj,0)) {
      sprintf(buf, "Its contents seem somewhat murky.\r\n"); /* BUG */
    } else {
      amt = (GET_OBJ_VAL(obj, 1) * 3) / GET_OBJ_VAL(obj, 0);
      sprinttype(GET_OBJ_VAL(obj, 2), color_liquid, buf2);
      sprintf(buf, "It's %sfull of a %s liquid.\r\n", fullness[amt], buf2);
    }
    send_to_char(buf, ch);
      }
    }
  }
}



char *find_exdesc(char *word, struct extra_descr_data * list)
{
  struct extra_descr_data *i;

  for (i = list; i; i = i->next)
    if ((NULL != i) && isname(word, i->keyword))
      return (i->description);

  return NULL;
}


/*
 * Given the argument "look at <target>", figure out what object or char
 * matches the target.  First, see if there is another char in the room
 * with the name.  Then check local objs for exdescs.
 */
void look_at_target(struct char_data * ch, char *arg, char *arg2 )
{
  int bits, found = 0, j;
  struct char_data *found_char = NULL;
  struct obj_data *obj = NULL, *found_obj = NULL;
  char *desc;

  if (!arg || !*arg) 
  {
    send_to_char("Look at what?\r\n", ch);
    return;
  }
  bits = generic_find(arg, FIND_OBJ_INV | FIND_OBJ_ROOM | FIND_OBJ_EQUIP |
            FIND_CHAR_ROOM, ch, &found_char, &found_obj);

  /* Is the target a character? */
  if (found_char != NULL) 
  {
    look_at_char(found_char, ch);
    if (ch != found_char) {
      if (CAN_SEE(found_char, ch) && (!IS_SET(GET_DFINTR(found_char),DRF_GREYMAN) || PRF_FLAGGED(ch, PRF_HOLYLIGHT)))
      {
	act("$n looks at you.", TRUE, ch, 0, found_char, TO_VICT);     
        if(!IS_SET(GET_DFINTR(found_char),DRF_GREYMAN))
        act("$n looks at $N.", TRUE, ch, 0, found_char, TO_NOTVICT);
      }
    }
    return;
  }

  /* Does the argument match an extra desc in the room? */
  if ((desc = find_exdesc(arg, world[ch->in_room].ex_description)) != NULL) 
  {
    char *p;
    page_string(ch->desc,(p =  char_look_at_room(ch,desc)), 0);
    free(p);
    return;
  }

  /* Does the argument match an extra desc in the char's equipment? */
  for (j = 0; j < NUM_WEARS && !found; j++)
    if (GET_EQ(ch, j) && CAN_SEE_OBJ(ch, GET_EQ(ch, j)))
      if ((desc = find_exdesc(arg, GET_EQ(ch, j)->ex_description)) != NULL) 
      {
	char *p;
	send_to_char(p = char_look_at_obj(ch,GET_EQ(ch,j),desc), ch);
        free(p);
	found = 1;
      }

  /* Does the argument match an extra desc in the char's inventory? */
  for (obj = ch->carrying; obj && !found; obj = obj->next_content) 
  {
    if (CAN_SEE_OBJ(ch, obj))
      if ((desc = find_exdesc(arg, obj->ex_description)) != NULL) 
      {
	char *p;
	send_to_char(p = char_look_at_obj(ch,obj,desc), ch);
	free(p);
	found = 1;
      }
  }

  /* Does the argument match an extra desc of an object in the room? */
  for (obj = world[ch->in_room].contents; obj && !found; obj = obj->next_content)
    if (CAN_SEE_OBJ(ch, obj))
    if ((desc = find_exdesc(arg, obj->ex_description)) != NULL) 
    {
      char *p;
      send_to_char(p = char_look_at_obj(ch,obj,desc), ch);
      free(p);
      found = 1;
    }
  if (bits) 	/* If an object was found back in generic_find */
  {
    if (!found)
      show_obj_to_char(found_obj, ch, 5);	/* Show no-description */
    else
      show_obj_to_char(found_obj, ch, 6);	/* Find hum, glow etc */
  } 
  else if (!found)
  {
    MSG_BOARD *brd;
    for( brd = top_of_boards; brd; brd=brd->next )
    {
      if( !strcasecmp( arg, brd->key ) )
      {
	if( GET_LEVEL( ch ) < LVL_GOD )
	{
	  if( !CHK_BRD_FLAG( brd, BRD_FLAG_GLOBAL ) )
	  {
	    send_to_char( "Those messages can not be read globally.\r\n", ch );
	    return;
	  }
	  else
	  {
	    send_to_char( "You do not see that here.\r\n", ch );
	    //send_to_char( "Global reading of MUD boards not available to mortals.\r\n", ch );
	    return;
	  }
	}
	if( !arg2 || !*arg2 )
	  Board_show_board( brd, ch, "board", NULL );
	else
	  Board_display_msg( brd, ch, arg2, NULL );
	return;
      }
    }
    send_to_char("You do not see that here.\r\n", ch);
  }
}

ACMD(do_glance)
{
  struct char_data *target;
  struct player_extradesc *xdesc;

  static char arg1[MAX_INPUT_LENGTH];
  static char arg2[MAX_INPUT_LENGTH];

  skip_spaces(&argument);

  half_chop(argument, arg1, arg2);

  if (!*arg1) {
    send_to_char("Glance at whom?\r\n",ch);
    return;
  }

  if (!(target = get_char_room_vis(ch,arg1))) {
    send_to_char("Will you hang around until that person shows up?\r\n",ch);
    return;
  }
  if (!*arg2) {
    if (GET_DESC(target)) {
      send_to_char(GET_DESC(target),ch);
      send_to_char("\r\n",ch);
    }
    diag_char_to_char(target,ch);
    if (IS_BRANDED(target)) {
     asend_to_char(ch, "\\c08%s is branded as a Darkfriend!!\r\n\\c00",GET_NAME(target));
    }
    chan_strength_test(ch, target);

  } else {
  // Glance ExtraDesc code

    if (!strcasecmp("self", arg1)) {
      if (NULL != (xdesc=find_playerxdesc(arg2, GET_CURRENTDESC(ch), ch))) {
        sprintf(buf, "\r\n%s", xdesc->desc);
        send_to_char(buf, ch);
     } else {
        send_to_char("You see nothing special.\r\n", ch);
      }
    } else {
      if (NULL != (target= get_char_pointer(arg1))) {
        if (NULL != (xdesc=find_playerxdesc(arg2, GET_CURRENTDESC(target), target))) {
          sprintf(buf, "\r\n%s", xdesc->desc);
          send_to_char(buf, ch);
          act("$n looks closely at you.", TRUE, ch, 0, target, TO_VICT);
          act("$n looks closely at $N.", TRUE, ch, 0, target, TO_NOTVICT);
        } else {
          send_to_char("You see nothing special.\r\n", ch);
        }
      } else {
          send_to_char("What do you want to look at?\r\n", ch);
      }
    }
  }
}

ACMD(do_look)
{
  static char tempstr[MAX_INPUT_LENGTH];
  static char arg2[MAX_INPUT_LENGTH];
  static char arg3[MAX_INPUT_LENGTH];

  struct player_extradesc *xdesc;
  struct char_data *vict;

  int look_type;

  if (!ch->desc && !is_retainer(ch))
    return;

  if (GET_POS(ch) < POS_SLEEPING)
    send_to_char("You can't see anything but stars!\r\n", ch);
  else if (IS_AFFECTED(ch, AFF_BLIND) || 
          (!IS_NPC(ch) && IS_SET(GET_PERMANENT(ch), PRM_BLIND)))
    send_to_char("You can't see a damned thing, you're blind!\r\n", ch);
  else if (IS_DARK(ch->in_room) && !CAN_SEE_IN_DARK(ch) && 
    (!GET_WOLFKIN(ch) || ROOM_FLAGGED(ch->in_room, ROOM_VERY_DARK)))
  {
    send_to_char("It is pitch black...\r\n", ch);
    list_char_to_char(world[ch->in_room].people, ch);	/* glowing red eyes */
  } else {
    half_chop(argument, arg, tempstr);
    half_chop(tempstr, arg2, arg3);    

    if (subcmd == SCMD_READ) 
    {
      if (!*arg)
	send_to_char("Read what?\r\n", ch);
      else
	look_at_target( ch, arg, arg2 );
      return;
    }
    if (!*arg)			/* "look" alone, without an argument at all */
      look_at_room(ch, 1);
    else if (is_abbrev(arg, "in"))
      look_in_obj(ch, arg2);
    /* did the char type 'look <direction>?' */
    else if ((look_type = search_block(arg, dirs, FALSE)) >= 0)
      look_in_direction(ch, look_type);
    else if (is_abbrev(arg, "at"))
      if (!*arg3) {
        look_at_target(ch, arg2, "" );
      } else {
        // Extra Desc Look at Code.

        if (!strcasecmp("self", arg2)) {
          if (NULL != (xdesc=find_playerxdesc(arg3, GET_CURRENTDESC(ch), ch))) {
            sprintf(buf, "\r\n%s", xdesc->desc);
            send_to_char(buf, ch);            
          } else {
            send_to_char("You see nothing special.\r\n", ch);
          }
        } else {
          if (NULL != (vict= get_char_pointer(arg2))) {
            if (NULL != (xdesc=find_playerxdesc(arg3, GET_CURRENTDESC(vict), vict))) {
              sprintf(buf, "\r\n%s", xdesc->desc);
              send_to_char(buf, ch);
              act("$n looks closely at you.", TRUE, ch, 0, vict, TO_VICT);
              act("$n looks closely at $N.", TRUE, ch, 0, vict, TO_NOTVICT);
            } else {
              send_to_char("You see nothing special.\r\n", ch);
            }
          } else {
            send_to_char("What do you want to look at?\r\n", ch);
          }
        }
      }
    else
      if (!*arg2) {
        look_at_target(ch, arg, "");
      } else {
        if (!strcasecmp("self", arg)) {
          if (NULL != (xdesc=find_playerxdesc(arg2, GET_CURRENTDESC(ch), ch))) {
            sprintf(buf, "\r\n%s", xdesc->desc);
            send_to_char(buf, ch);
          } else {
            send_to_char("You see nothing special.\r\n", ch);
          }
        } else {
          if (NULL != (vict= get_char_pointer(arg))) {
            if (NULL != (xdesc=find_playerxdesc(arg2, GET_CURRENTDESC(vict), vict))) {
              sprintf(buf, "\r\n%s", xdesc->desc);
              send_to_char(buf, ch);
              act("$n looks closely at you.", TRUE, ch, 0, vict, TO_VICT);
              act("$n looks closely at $N.", TRUE, ch, 0, vict, TO_NOTVICT);
            } else {
              send_to_char("You see nothing special.\r\n", ch);
            }
          } else {
            send_to_char("What do you want to look at?\r\n", ch);
          }
        }
      }
  }
}



ACMD(do_examine)
{
  int bits;
  struct char_data *tmp_char;
  struct obj_data *tmp_object;

  one_argument(argument, arg);

  if (!arg || !*arg) {
    send_to_char("Examine what?\r\n", ch);
    return;
  }
  look_at_target(ch, arg, "");

  bits = generic_find(arg, FIND_OBJ_INV | FIND_OBJ_ROOM | FIND_CHAR_ROOM |
             FIND_OBJ_EQUIP, ch, &tmp_char, &tmp_object);

  if (tmp_object) {
    if ((GET_OBJ_TYPE(tmp_object) == ITEM_DRINKCON) ||
    (GET_OBJ_TYPE(tmp_object) == ITEM_FOUNTAIN) ||
    (GET_OBJ_TYPE(tmp_object) == ITEM_CONTAINER)) {
      send_to_char("When you look inside, you see:\r\n", ch);
      look_in_obj(ch, arg);
    }
  }
}



ACMD(do_gold)
{
  if (GET_GOLD(ch) == 0)
    send_to_char("You're broke!\r\n", ch);
  else if (GET_GOLD(ch) == 1)
    send_to_char("You have one miserable little gold coin.\r\n", ch);
  else {
    sprintf(buf, "You have %ld gold coins.\r\n", GET_GOLD(ch));
    send_to_char(buf, ch);
  }
}

ACMD(do_worth)
{
  long gear = 0, skills = 0;

  if (IS_NPC(ch)) {
    send_to_char("Hey! You're a mob! You're by definition not worth -anything-!\r\n",ch);
    return;
  }

  get_qs(ch,&skills,&gear);
  sprintf(buf,"You have %ld gold on you and %ld in the bank.\r\n", 
      GET_GOLD(ch), GET_BANK_GOLD(ch));
  sprintf(buf,"%sYou have %ld exp and need %ld to level.\r\n", buf,
      GET_EXP(ch), GET_LEVEL(ch)*GET_LEVEL_EXP(ch) - GET_EXP(ch));
  sprintf(buf,"%sYou have %ld QPs (%ld in quest eq, %ld in skills and "
      "%ld in \"raw\" QPs.)\r\n",buf,GET_QP(ch)+gear+skills,gear,skills,GET_QP(ch));

  send_to_char(buf, ch);
}

ACMD(do_score)
{
  struct char_data *vict = 0;
  void print_score(struct char_data *ch,struct char_data *tch);

  half_chop(argument, buf1, buf2);
 
  if (!*buf1) 
  {
    print_score(ch, ch);
    return;
  }
  else if (is_abbrev(buf1, "file")) 
  {
    if (GET_LEVEL(ch) > LVL_IMMORT) 
    {
      if (NULL != (vict = xmlload_char(buf2))) 
      {
        fetch_char_guild_data(vict);
        if (GET_LEVEL(ch) < GET_LEVEL(vict))
          send_to_char("You can't do that.\r\n", ch);
        else 
        {
          GET_POS(vict) = POS_STANDING;
          print_score(ch, vict);
        }
        remove_char_guild_data(vict);
      } 
      else
        send_to_char("No such player found.\r\n", ch);
    } else
      print_score(ch, ch);
  } 
  else 
  {
    if (GET_LEVEL(ch) > LVL_IMMORT && (vict=get_char_vis(ch,buf1)) )
    {
      if (GET_LEVEL(ch) < GET_LEVEL(get_char_vis(ch, buf1)))
        send_to_char("You can't do that.\r\n", ch);
      else
        print_score(ch, vict);

      return;
    }
    if (GET_LEVEL(ch) > LVL_IMMORT && !(vict=get_char_vis(ch,buf1)) )
    {
      send_to_char("No such person around.\r\n", ch);
      return;
    }
    print_score(ch, ch);
  }
  if (NULL != vict)
    free_char(vict);
}

ACMD(do_skills)
{
  struct char_data *tchar;
  void print_skills(struct char_data *ch,struct char_data *tchar);
  sort_spells();

  if ((GET_LEVEL(ch) < LVL_IMMORT) || (!*argument))
    print_skills(ch,ch);
  else {
    one_argument(argument,arg);
    if (!(tchar = get_char_vis(ch,arg))) {  
       send_to_char("No such person around!\r\n",ch);
       return;
    }
    if (GET_LEVEL(ch) < GET_LEVEL(tchar)) {
      send_to_char("You're not Godly enough to do that!\n",ch);
      return;
    }
    print_skills(ch,tchar);
  }
}

#define MASTER_BIT 0 /* It's not defined or used anywhere else so (M) */

void print_score(struct char_data *ch,struct char_data *tch)
{
  struct time_info_data playing_time;
  char sexig[10],buf2[2048],buf3[4096];
  long lbid = 0;
  int i = 0;
  struct char_guild_element *element;  

  *buf2 = *buf3 = '\0';
  if (GET_SEX(tch) == SEX_MALE)
    strcpy(sexig,"Male");
  else if (GET_SEX(tch) == SEX_FEMALE)
    strcpy(sexig,"Female");
  else
    strcpy(sexig,"It");

  /* line one  - nobility title, name, title */
  sprintf(buf, "&cYou are:");
  if (GET_NOBILITY(tch))
    sprintf(buf,"%s &r%s&n", buf, GET_NOBILITY(tch));
  sprintf(buf, "%s &W%s %s&n\r\n", buf, GET_NAME(tch), GET_TITLE(tch));  
  if (GET_DISGUISED(tch))
    sprintf(buf,"%sYou are disguised: Real Name is %s\r\n",
        buf,GET_RNAME(tch));
  /* next line - doing if it exists */
  if (GET_DOING(tch))
    sprintf(buf,"%s&cYour doing is: &W%s%s&n\r\n", 
        buf, GET_NAME(tch),GET_DOING(tch));

  /* next line - homeland age playing time */
  sprintf(buf, "%s&cHomeland: &R%s   &cAge: &W%d",
      buf,hometown[(int)GET_RACE(tch)],GET_AGE(tch));

  if (!IS_NPC(tch)) {
    playing_time = real_time_passed((time(0) - tch->player.time.logon) +
                    tch->player.time.played, 0);
    sprintf(buf, "%s  &cTime Played: &g%ld &cdays and &g%ld &chours&n",
        buf, playing_time.day, playing_time.hours);

    if ( GET_LEVEL(tch) >= LVL_IMMORT ) 
      sprintf(buf, "%s  &cInvstart: &W%ld&n\r\n", buf, GET_INVSTART(tch));
    else
      sprintf(buf, "%s\r\n", buf);
  }
  else {
    sprintf(buf, "%s\r\n", buf);
  }

  /* next line - sex class lvl qps*/
  sprintf(buf, "%s&cSex: &W%s    &cClass: &W%s    &cLevel: &W%3ld", 
          buf, sexig, pc_class_types[(int) GET_CLASS(tch)],GET_LEVEL(tch));
  sprintf(buf, "%s  &c Questpoints: &W%ld&n",
          buf, GET_QP(tch));
  if ( GET_LEVEL(tch) >= LVL_IMMORT )
    sprintf(buf, "%s  &c OLC: &W%ld&n\r\n",buf, GET_OLC_ZONE(tch));
  else
    sprintf(buf, "%s \r\n", buf);

  /* next line - str hit tot_exp exp_to_lvl */
  sprintf(buf,"%s  &cStrength    : &g%2d&c/&g%2d", 
          buf, GET_STR(tch),tch->real_abils.str);
  sprintf(buf,"%s    &cHitp: &g%5d&c/&g%5d  &cExp: &W%6ld&c  To level: &W%4ld&n\n\r",
          buf,GET_HIT(tch), GET_MAX_HIT(tch), GET_EXP(tch),
          (GET_LEVEL(tch))*GET_LEVEL_EXP(tch) - GET_EXP(tch));

  /* next line - con mana trains prac */
  sprintf(buf,"%s  &cConstitution: &g%2d&c/&g%2d", 
          buf, GET_CON(tch),tch->real_abils.con);
  sprintf(buf,"%s    &cMana: &g%5ld&c/&g%5ld&c  &cTrains: &W%3d  &cPractices: &W%3ld&n\r\n", 
          buf, GET_MANA(tch), GET_MAX_MANA(tch),GET_TRAINS(tch),
          GET_PRACTICES(tch));

  /* next line - dex move gold bank */
  sprintf(buf,"%s  &cDexterity   : &g%2d&c/&g%2d",
          buf, GET_DEX(tch),tch->real_abils.dex);
  sprintf(buf,"%s    &cMove: &g%5d&c/&g%5d", 
          buf, GET_MOVE(tch), GET_MAX_MOVE(tch));
  sprintf(buf,"%s  &cGold: &Y%5ld  &cBank: &y%ld&n\r\n",
          buf, GET_GOLD(tch),GET_BANK_GOLD(tch));

  /* next line - int  carrying */
  sprintf(buf,"%s  &cIntelligence: &g%2d&c/&g%2d&n", 
          buf, GET_INT(tch), tch->real_abils.intel);
  sprintf(buf,"%s      &cCarrying: &W%ld &wlbs, &W%ld &witems&n\r\n",
          buf, IS_CARRYING_W(tch),IS_CARRYING_N(tch));


  /* next line - wis  */
  sprintf(buf,"%s  &cWisdom      : &g%2d&c/&g%2d\r\n",
          buf, GET_WIS(tch),tch->real_abils.wis);

  /* next line - cha ac abs damage-dice */
  sprintf(buf,"%s  &cCharisma    : &g%2d&c/&g%2d&n",
          buf, GET_CHA(tch),tch->real_abils.cha);
  sprintf(buf,"%s    &cAC: &W%d  &cAbs: &W%ld  &cHit: &W%ld  &cDam: &W%ld  ",
          buf, GET_AC(tch), GET_ABS(tch), GET_HITROLL(tch),GET_DAMROLL(tch));

  if (GET_EQ(tch, WEAR_WIELD_R))
    sprintf(buf, "%s&cDamage Dice: &W%ldd%ld&n\r\n", buf, GET_OBJ_VAL(GET_EQ(tch,WEAR_WIELD_R),1),
        GET_OBJ_VAL(GET_EQ(tch,WEAR_WIELD_R),2));
  else if (GET_EQ(tch, WEAR_WIELD_L))
    sprintf(buf, "%s&cDamage Dice: &W%ldd%ld&n\r\n", buf, GET_OBJ_VAL(GET_EQ(tch,WEAR_WIELD_L),1),
        GET_OBJ_VAL(GET_EQ(tch,WEAR_WIELD_L),2));
  else sprintf(buf, "%s&n\r\n", buf);

  /* Command Groups */
  if ((tch)->player_specials->saved.command_groups) {
    sprintbit((tch)->player_specials->saved.command_groups, cg_names, buf2);
    sprintf(buf, "%s\r\n&rYou have the following command-groups :&y %s&n\r\n", buf, buf2);
  }

  
  /* Talents */
  /*primary talents*/
  if (GET_PRIMTALENTS1(tch) || GET_PRIMTALENTS2(tch)) {
    *buf2 = 0;
    sprintf(buf, "%s\r\n&wYou have the following primary talents : &n", buf);
    /*get list of primary talents*/
    for (i = 0; i < NUM_TALENTS; i++)    /*32 is num of bits in a long */
      if(IS_TALENT_SET(GET_PRIMTALENTS1(tch), GET_PRIMTALENTS2(tch), i))
    sprintf(buf2, "%s %s", buf2, master_talent_list[i].name);

    /*send to buffer*/
    sprintf(buf, "%s%s\r\n",buf,  buf2);
  }

  /*secondary talents*/
  if (GET_SECTALENTS1(tch) || GET_SECTALENTS2(tch)) {
    sprintf(buf2, "&wYou have the following secondary talents : &n");
    /*get list of secondary talents*/
    for (i = 0; i < NUM_TALENTS; i++)              /*32 is num of bits in a long
                            */
      if(IS_TALENT_SET(GET_SECTALENTS1(tch), GET_SECTALENTS2(tch), i))
    sprintf(buf2, "%s %s", buf2, master_talent_list[i].name);
   
    /*send to buffer*/
    sprintf(buf, "%s%s\r\n", buf, buf2);
  }

  /*special skills*/
  if (GET_SPSKILLS1(tch) || GET_SPSKILLS2(tch)) {
    sprintf(buf2, "&wYou have the following special skills : &n");
    /*get list of special skills*/
    for (i = 0; i < NUM_TALENTS; i++)              /*32 is num of bits in a long
                            */
      if(IS_TALENT_SET(GET_SPSKILLS1(tch), GET_SPSKILLS2(tch), i))
    sprintf(buf2, "%s %s", buf2, master_talent_list[i].name);
   
    /*send to buffer*/
    sprintf(buf, "%s%s\r\n",buf, buf2);
  }
  /* channey stuff */

  /*add a blank line to space things out a bit */
  sprintf(buf,"%s\r\n",buf);

  if (GET_SPARK(tch) || GET_TEACHABLE(tch) || 
      (GET_CLASS(tch) == CLASS_CHANNELER) ||
      (GET_CLASS(tch) == CLASS_WILDER)) {
        sprintf(buf, "%s&mChanneling Abilities:  &w(&r&uMaximum&n&w IC strength: &r%d&w)&n\r\n", buf,
                (int)((GET_AIR(tch) + GET_FIRE(tch) + GET_WATER(tch) + GET_EARTH(tch) + GET_SPIRIT(tch)) *
                ((GET_SEX(tch) == SEX_MALE && !PLR_FLAGGED(tch, PLR_NOGAIN)) ? 1.3 : 1)));
    sprintf(buf,"%s&cAir: &w%d  &cEarth: &y%d  &cFire: &r%d  &cSpirit: &W%d  &cWater: &b%d&n\r\n", 
        buf, GET_AIR(tch), GET_EARTH(tch), 
        GET_FIRE(tch), GET_SPIRIT(tch), GET_WATER(tch));
  }

  if (!IS_NPC(tch)) {

    /* wolfkin/club */
    if (*GET_SERVANT(tch) && !GET_ISWOLFKIN(tch))
      sprintf(buf, "%s%s\r\n", buf, GET_SERVANT(tch));

    if (GET_WOLFKIN(tch) && GET_LEVEL(tch) >= 15) {
      sprintf(buf,"%s&c(&W*&c)&cYou are Wolfkin",buf);

      if (GET_ISWOLFKIN(tch) && *GET_SERVANT(tch)) {

    sprintf(buf, "%s with the wolfname %s&n\r\n",buf,GET_SERVANT(tch));

    if (GET_WKRANK(tch) < 3) {
      sprintf(buf, "%s   &cYou are %s &cby the wolves\r\n",buf, wkrank[GET_WKRANK(tch)]);
    }         
    else if (GET_WKRANK(tch) < 6 && GET_WKRANK(tch) > 2) {
      sprintf(buf, "%s   &cThe wolves see you as %s.\r\n",buf, wkrank[GET_WKRANK(tch)]); 
    }
    else if (GET_WKRANK(tch) > 5) {
      sprintf(buf, "%s &c  Among the wolves, you are %s.\r\n",buf, wkrank[GET_WKRANK(tch)]);
    } 
    else {
      sprintf(buf, "%s a &y%s&c for your pack.&n\r\n",
          buf, wkrank[GET_WKRANK(tch)]); 
    }
      }
      else if ((GET_ISWOLFKIN(tch) && !( *GET_SERVANT(tch) )) || get_guild(tch, 20))
    sprintf(buf, "%s.\r\n", buf);
      else
    sprintf(buf, "%s but you haven't met your kin yet.&n\r\n",buf);
    }

    if (IS_BRANDED(tch))
      sprintf(buf, "%s&rYou are branded as a Darkfriend, and will remain so for %ld hours.&n\r\n",
              buf, IS_BRANDED(tch));
    if (IS_SET(GET_PERMANENT(tch), PRM_DEAF))
      sprintf(buf, "%sYou are permanently deaf.\r\n",buf);
    if (IS_SET(GET_PERMANENT(tch), PRM_MUTE))
      sprintf(buf, "%sYou are permanently mute (tongue removed).\r\n",buf);
    if (IS_SET(GET_PERMANENT(tch), PRM_CRIPPLED))
      sprintf(buf, "%sYou are permanently crippled.\r\n",buf);
    if (IS_SET(GET_PERMANENT(tch), PRM_RH_CUT))      sprintf(buf, "%sYour right hand has been cut off.\r\n",buf);
    if (IS_SET(GET_PERMANENT(tch), PRM_LH_CUT))
      sprintf(buf, "%sYour left hand has been cut off.\r\n",buf);      
    if (!cant_channel(tch) && IS_SET(GET_PERMANENT(tch), PRM_GENTLED))
      sprintf(buf,"%sYou have been cut off from the One Power,\r\n",buf);

    /* misc flags' stuff */
    if (IS_SET(GET_DFINTR(tch),DRF_GREYMAN))
      sprintf(buf,"%s&kYou have given up your soul to serve the Great Lord of the Dark&n\r\n",buf);
    if (IS_SET(GET_DFINTR(tch),DRF_GHOLAM))
      sprintf(buf,"%s&kYou serve the Great Lord as a Gholam.&n\r\n",buf);

    if (IS_SET(PRF_FLAGS(tch),PRF_GRASPING)) 
      sprintf(buf,"%s&WYou are grasping the One Power.&n\r\n",buf);
    if (IS_SET(PRF_FLAGS(tch),PRF_TGRASP))
      sprintf(buf,"%s&KYou are grasping the True Power.&n\r\n",buf);

    if(GET_SUBDUE(tch))
      sprintf(buf,"%s&GYou will capture your opponents alive in combat.&n\n\r", 
              buf);
    if(!GET_SUBDUE(tch))
      sprintf(buf,"%s&RYou will kill your opponents in combat.&n\r\n", buf);

    if (IS_AFFECTED(tch, AFF_DREAMING))
      sprintf(buf,"%s&CYou are in Tel'aran'rhiod.&c\r\n", buf);

    if (GET_BM(tch))
      sprintf(buf,"%sYou are considered a %s among the Blademasters.\r\n",
              buf, blademastery[GET_BM(tch)]);
  }
  send_to_char(buf, ch);

  *buf2 = *buf3 = '\0';
  // Aes Sedai bond check
  if (GET_BONDED(tch)) {
    if (GET_BONDED(tch) & (1 << 30)) { // Master bit set, several bonds
      lbid = GET_BONDED(tch);
      REMOVE_BIT(lbid, (1 << 30));
      sprintf(buf, "&cYou have bonded the following person(s) as your warders:&n\r\n");
      for (i = 0; i <= top_of_p_table; i++) 
        if (player_table[i].bonded == lbid) 
          sprintf(buf,"%s%s\r\n",buf,CAP(player_table[i].name));
      send_to_char(buf, ch);
    } else {  // Bonded person
      sprintf(buf3, "Nobody!\r\n");
      sprintf(buf2, "&cYou're bonded as warder to &n");
      lbid = GET_BONDED(tch);
      SET_BIT(lbid,(1 << 30));
      for (i = 0; i <= top_of_p_table; i++) {
        if (player_table[i].bonded == lbid) {
          sprintf(buf3, "%s.\r\n", CAP(player_table[i].name));
          break;
        }
      }
      send_to_char(buf2, ch);
      send_to_char(buf3, ch);
    }
  }
  // Ashabonded test
  if (GET_ASHABONDED(tch)) {
    if (GET_ASHABONDED(tch) & (1 << 30)) { // Master bit set, several bonds
      lbid = GET_ASHABONDED(tch);
      REMOVE_BIT(lbid, (1 << 30));
      sprintf(buf, "&cYou have bonded the following person(s) as your Ashaman warders:&n\r\n");
      for (i = 0; i <= top_of_p_table; i++) 
        if (player_table[i].ashabonded == lbid)  
          sprintf(buf,"%s%s\r\n",buf,CAP(player_table[i].name));
      send_to_char(buf, ch);
    } else {  // Bonded person
      sprintf(buf3, "Nobody!\r\n");
      sprintf(buf2, "&cYou're bonded as Ashaman warder to &n");
      lbid = GET_ASHABONDED(tch);
      SET_BIT(lbid,(1 << 30));
      for (i = 0; i <= top_of_p_table; i++) 
        if (player_table[i].ashabonded == lbid) 
          sprintf(buf3, "%s.\r\n", CAP(player_table[i].name));
      send_to_char(buf2, ch);
      send_to_char(buf3, ch);
    }
  }
  // Minion bond check
  if (GET_MINION(tch)) {
    if (GET_MINION(tch) & (1 << 30)) { // Master bit set, several bonds
      lbid = GET_MINION(tch);
      REMOVE_BIT(lbid, (1 << 30));
      sprintf(buf, "&cThe following people are your obedient minions:&n\r\n");
      for (i = 0; i <= top_of_p_table; i++) 
        if (player_table[i].minion == lbid)  
          sprintf(buf,"%s%s\r\n",buf,CAP(player_table[i].name));
      send_to_char(buf, ch);
    } else {  // Bonded person
      sprintf(buf3, "Nobody!\r\n");
      sprintf(buf2, "&cYou're the obedient minion of &n");
      lbid = GET_MINION(tch);
      SET_BIT(lbid,(1 << 30));
      for (i = 0; i <= top_of_p_table; i++) 
        if (player_table[i].minion == lbid) 
          sprintf(buf3, "%s.\r\n", CAP(player_table[i].name));
      send_to_char(buf2, ch);
      send_to_char(buf3, ch);
    }
  }

  element = GET_CHAR_GUILDS(tch);
  if( element )
     sprintf(buf, "%s\r\n&GUse 'myguild' for Guild Membership information&n\n", buf);
#if 0
  if (element) send_to_char("\r\n", ch);
  while (element) {
    if (STATUS_FLAGGED(element->guildie, STATUS_MEMBER)) {
      if (!STATUS_FLAGGED(element->guildie, STATUS_SUBRANK))
    sprintf(buf, "You are guilded to the &G%s&n as %s.\r\n", element->guild->name, 
        ((element->guildie->rank_num != 0) ? element->guildie->rank->name : 
         "Unranked"));
    else 
      sprintf(buf, "You are guilded to the &G%s&n as %s [%s].\r\n", element->guild->name,
          ((element->guildie->rank_num != 0) ? element->guildie->rank->name : 
        "Unranked"),
          element->guildie->subrank);
    }
    else sprintf(buf, "You are seeking the &R%s&n.\r\n", element->guild->name);
    send_to_char(buf, ch);
    element = element->next;
  }
#endif
  send_to_char("\r\n", ch);
}

ACMD(do_inventory)
{
  send_to_char("You are carrying:\r\n", ch);
  list_obj_to_char(ch->carrying, ch, 1, TRUE);
}

ACMD(do_isic)
{
  struct char_data *tch;

  one_argument(argument, arg);

  if (!*arg) {
    send_to_char("Usage: isic <player>.\r\n", ch);
    return;
  }

 if ( !(tch = get_char_vis_including_tar(ch, arg))) {
    send_to_char("Try someone logged in.\r\n", ch);
    return;
 }

 if (IS_SET(PRF_FLAGS(tch),PRF_IC)) {
    sprintf(buf, "%s is \\c02IC\\c00.", GET_NAME(tch));
 } else if (!IS_SET(PRF_FLAGS(tch),PRF_IC)) {
    sprintf(buf, "%s is \\c01OOC\\c00.", GET_NAME(tch));
 }
 send_to_char(buf, ch);

 return;
}


ACMD(do_know_health)
{
  struct char_data *vict;
  char sexig[10], sex[10];
  float hpratio, mvratio;

  if (!GET_SKILL(ch, SKILL_KNOWHEALTH))
  {
    send_to_char("You don't even know how!\r\n", ch);
    return;
  }

  one_argument(argument, arg);

  if (!*arg)
  {
    send_to_char("Learn the health of whom?\r\n", ch);
    return;
  }

  if (!(vict = get_char_room_vis(ch, arg)))
  {
    send_to_char("That person is not here right now.\r\n", ch);
    return;
  }

  if (GET_SEX(vict) == SEX_MALE)
    strcpy(sexig,"He");
  else if (GET_SEX(vict) == SEX_FEMALE)
    strcpy(sexig,"She");
  else
    strcpy(sexig,"It");

  if (GET_SEX(vict) == SEX_MALE)
    strcpy(sex,"his");
  else if (GET_SEX(vict) == SEX_FEMALE)
    strcpy(sex,"her");
  else
    strcpy(sex,"its");

  hpratio= (float)((float)GET_HIT(vict)/(float)GET_MAX_HIT(vict))*100;
  mvratio= (float)((float)GET_MOVE(vict)/(float)GET_MAX_MOVE(vict))*100;

  sprintf(buf, "%s has %.2f%% of %s total health, %.2f%% of %s movement, and ", GET_NAME(vict), hpratio, sex, mvratio, sex); 
  if(IS_SET(GET_PERMANENT(vict), PRM_BLIND))
     sprintf(buf, "%sis permanently blind.\n\r", buf);
  else
     sprintf(buf, "%sisn't permanently blind.\n\r",buf);
  if (IS_SET(GET_PERMANENT(vict), PRM_CRIPPLED))
     sprintf(buf, "%s%s is crippled in some way ",buf, GET_NAME(vict));
  else
     sprintf(buf, "%s%s isn't crippled in any way ",buf, GET_NAME(vict));
  if (IS_SET(GET_PERMANENT(vict), PRM_RH_CUT) && IS_SET(GET_PERMANENT(vict), PRM_LH_CUT))
     sprintf(buf, "%sbut both %s hands have been cut off!\r\n",buf,sex);
  else
  if (IS_SET(GET_PERMANENT(vict), PRM_RH_CUT))
     sprintf(buf, "%sbut %s right hand has been cut off!\r\n",buf,sex);
  else
  if (IS_SET(GET_PERMANENT(vict), PRM_LH_CUT))
     sprintf(buf, "%sbut %s left hand has been cut off!\r\n",buf,sex);
  else
     sprintf(buf, "%sand both %s hands are intact.\r\n",buf, sex);

  if (IS_SET(GET_PERMANENT(vict), PRM_DEAF) && IS_SET(GET_PERMANENT(vict), PRM_MUTE))
     sprintf(buf, "%s%s is permanently deaf and mute.\n\r", buf,sexig);
  else 
  if (IS_SET(GET_PERMANENT(vict), PRM_DEAF) && !IS_SET(GET_PERMANENT(vict), PRM_MUTE))
     sprintf(buf, "%s%s is permanently deaf but not mute.\r\n", buf,sexig);
  else
  if (!IS_SET(GET_PERMANENT(vict), PRM_DEAF) && IS_SET(GET_PERMANENT(vict), PRM_MUTE))
     sprintf(buf, "%s%s is permanently mute but not deaf.\n\r", buf, sexig); 
  else
     sprintf(buf, "%s%s is neither mute or deaf.\n\r", buf, sexig);


  send_to_char(buf, ch);
}


ACMD(do_equipment)
{
#define POS_DUALWIELD 19

  int i, found = 0;

  send_to_char("You are using:\r\n", ch);
  for (i = 0; i < NUM_WEARS; i++) {
    if (GET_EQ(ch, i)) {
      if (CAN_SEE_OBJ(ch, GET_EQ(ch, i))) {
        if (CAN_WEAR(GET_EQ(ch,i),ITEM_WEAR_DUALWIELD)) 
          send_to_char(where[POS_DUALWIELD],ch);
        else
      send_to_char(where[i], ch);
    show_obj_to_char(GET_EQ(ch, i), ch, 1);
    found = TRUE;
      } else {

        if (CAN_WEAR(GET_EQ(ch,i),ITEM_WEAR_DUALWIELD)) 
          send_to_char(where[POS_DUALWIELD],ch);
        else
      send_to_char(where[i], ch);
    send_to_char("Something.\r\n", ch);
    found = TRUE;
      }
    }
  }
  if (!found) {
    send_to_char(" Nothing.\r\n", ch);
  }
}

/* 
   Looks up the day in the calendar and checks whether it's a special day
   in Randland, such as the Feast of Lights.                          
*/
char *special_day()
{
  int d = time_info.day, m = time_info.month;

  switch (m) {
    case 0:
      if (d == 0)
        return "Today is the Feast of Lights.\r\n";
      else if (d == 11)
        return "Today is the day of High Chasaline\r\n";
    break;
    case 1:
      if (d == 2)
        return "Today is the day of Chansun.\r\n";
      else if (d == 8)
        return "Today is the Feast of Abram.\r\n";
    break;
    case 4:
      if (d == 8)
        return "Today is the Feast of Neman.\r\n";
      if (d == 20)
        return "Today is the Feast of Freia.\r\n";
    break;
    case 5:
      if (d == 0)
        return "Today is the Day of Dahan.\r\n";
    break;
    case 6:
      if (d == 5)
        return "Today is the Feast of Maia.\r\n";
      if (d == 0)
        return "Today is the Day of Bailene.\r\n";
    break;
    case 7:
      if (d == 2)
        return "Today is the Day of Genshai.\r\n";
      if (d == 17)
        return "Today is Mabriams Day.\r\n";
    break;
    case 8:
     if (d == 8)
        return "Today is the Day of Tandar.\r\n";
    if (d == 17)
        return "Today is Low Chasaline.\r\n";
    break;
    case 9:
      if (d == 0)
        return "Today is the Festival of Lanterns.\r\n";
    break;
    case 10:
      if (d == 5)
        return "Today is the Day of Amaethon.\r\n";
      if (d == 11)
        return "Today is Day of Shaoman.\r\n";
    break;
    case 11:
      if (d == 27)
        return "Today is the Day of Danshu.\r\n";
    break;
  }
  return "\\c00";
}

ACMD(do_time)
{
  char *suf;
  int weekday, day;

  sprintf(buf, "It is %ld o'clock %s, on ",
      ((time_info.hours % 12 == 0) ? 12 : ((time_info.hours) % 12)),
      ((time_info.hours >= 12) ? "pm" : "am"));

  /* 28 days in a month */
  weekday = ((28 * time_info.month) + time_info.day + 1) % 7;

  strcat(buf, weekdays[weekday]);
  strcat(buf, "\r\n");
  send_to_char(buf, ch);

  day = time_info.day + 1;	/* day in [1..28] */

  if (day == 1)
    suf = "st";
  else if (day == 2)
    suf = "nd";
  else if (day == 3)
    suf = "rd";
  else if (day < 20)
    suf = "th";
  else if ((day % 10) == 1)
    suf = "st";
  else if ((day % 10) == 2)
    suf = "nd";
  else if ((day % 10) == 3)
    suf = "rd";
  else
    suf = "th";

  sprintf(buf, "The %d%s Day of the %s, Year %d.\r\n%s",
      day, suf, month_name[(int) time_info.month], time_info.year,
          special_day());
  
  send_to_char(buf, ch);
}


ACMD(do_weather)
{
  static char *sky_look[] = {
    "cloudless",
    "cloudy",
    "snowy",
    "lit by flashes of lightning"};

  if (OUTSIDE(ch)) {
    sprintf(buf, "The sky is %s and %s.\r\n", sky_look[weather_info.sky],
        (weather_info.change >= 0 ? "you feel a warm wind from the south" :
         "your foot tells you hot, dry weather is due"));
    send_to_char(buf, ch);
  } else
    send_to_char("You have no feeling about the weather at all.\r\n", ch);

    /*  room and _real_ weather information visible to WEAVERS */
    if (IS_SET((ch)->player_specials->saved.command_groups, CG_WEAVER)){
      sprintf(buf, "\r\n\r\n\r\n&WCODE ROOM AND WEATHER INFO&n\r\n\r\nIt is %s.\r\n",
      ((weather_info.sunlight == SUN_SET || weather_info.sunlight ==
      SUN_DARK) ? "night" : "daytime"));
      sprintf(buf2, "You are %s.\r\n", (OUTSIDE(ch) ? "outdoors" :
      "indoors"));
      strcat(buf, buf2);
      /* barometer readings */
      sprintf(buf2, "The sky is %s and the weather is %s.\r\n",
      sky_look[weather_info.sky],
      (weather_info.change >= 0 ? "improving" : "worsening"));
      strcat(buf, buf2);
      sprintf(buf2, "Barometer: %li and %s. (%li next hour)\r\n",
      weather_info.pressure, (weather_info.change >= 0 ? "rising" : "falling"),
      weather_info.change + weather_info.pressure);
      strcat(buf, buf2);

      send_to_char(buf, ch);
      }
}

struct help_index_element *find_help(char *keyword)
{
  struct help_indexer_element *el;

  for (el = help_index;el;el = el->next)
    if (is_abbrev(keyword, el->keyword))
      return (help_table + el->pos);

  return NULL;
}

struct poli_index_element *find_poli(char *keyword)
{
  struct poli_indexer_element *el;

  for (el = poli_index;el;el = el->next)
    if (is_abbrev(keyword, el->keyword))
      return (poli_table + el->pos);

  return NULL;
}

static const char *warn ="**** BUFFER OVERFLOW **** -remaining items lost-\r\n";

ACMD(do_hlist) 
{
  struct help_index_element *h;
  int i;
  char buf[MAX_STRING_LENGTH];

  if (!ch->desc)
    return;

  if (!help_table) {
    send_to_char("No help available.\r\n", ch);
    return;
  }

  *buf = 0;

  for (i = 0; i < top_of_helpt; i++) {
    h = help_table+i;
    if (h->min_level <= GET_LEVEL(ch)) {
      if (strlen(buf) >= (MAX_STRING_LENGTH - 2*strlen(warn))) {
        strcat(buf,warn);
        page_string(ch->desc,buf,1);
        return;
      }
      sprintf(buf,"%s[%4d]: %s\r\n",buf,(i+1),h->keywords);
    }
  }
  page_string(ch->desc,buf,1);
}

ACMD(do_plist) 
{
  struct poli_index_element *p;
  int i;
  char buf[MAX_STRING_LENGTH];

  if (!ch->desc)
    return;

  if (!poli_table) {
    send_to_char("No policies available.\r\n", ch);
    return;
  }

  *buf = 0;

  for (i = 0; i < top_of_polit; i++) {
    p = poli_table+i;
    if (p->min_level <= GET_LEVEL(ch)) {
      if (strlen(buf) >= (MAX_STRING_LENGTH - 2*strlen(warn))) {
    strcat(buf,warn);
    page_string(ch->desc,buf,1);
    return;
      }
      sprintf(buf,"%s[%4d]: %s\r\n",buf,(i+1),p->keywords);
    }
  }
  page_string(ch->desc,buf,1);
}

ACMD(do_help)
{
  struct help_index_element *this_help;
  char entry[MAX_STRING_LENGTH];

  if (!ch->desc)
    return;

  one_argument(argument,arg);

  skip_spaces(&argument);

  if (!*argument) {
    page_string(ch->desc, help, 0);
    return;
  }
  if (!help_table) {
    send_to_char("No help available.\r\n", ch);
    return;
  }

  if (!(this_help = find_help(argument))) {
    sprintf(buf, "%s tried to view non-existant help %s", GET_NAME(ch), argument);
    xlog(SYS_NOHELP, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), buf, TRUE);
    if (GET_LEVEL(ch) <= 20) {
      sprintf(buf, "%s tried to view non-existant help %s.", GET_NAME(ch), argument);
      nhlog(buf);
    }
    send_to_char("There is no help on that word.\r\n", ch);
    return;
  }

  if (this_help->min_level > GET_LEVEL(ch)) {
    send_to_char("There is no help on that word.\r\n", ch);
    return;
  }
  sprintf(entry, "\\c03%s\\c00\r\n%s", this_help->keywords, this_help->entry);
  
  page_string(ch->desc, entry, 1);
}

ACMD(do_newpolicy)
{
  struct poli_index_element *this_poli;
  char entry[MAX_STRING_LENGTH];

  if (!ch->desc)
    return;

  skip_spaces(&argument);

  if (!*argument) {
    page_string(ch->desc, poli, 0);
    return;
  }
  if (!poli_table) {
    send_to_char("No policies available.\r\n", ch);
    return;
  }

  if (!(this_poli = find_poli(argument))) {
    sprintf(buf, "%s tried to view non-existant policy %s", GET_NAME(ch), argument);
    xlog(SYS_NOPOLICY, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), buf, TRUE);
    send_to_char("Policy file does not exist.\r\n", ch);
    return;
  }

  if (this_poli->min_level > GET_LEVEL(ch)) {
    send_to_char("Policy file does not exist.\r\n", ch);
    return;
  }
  sprintf(entry, "&rPOLICY: &c%s\\c00\r\n%s", this_poli->keywords, this_poli->entry);
  
  page_string(ch->desc, entry, 1);
}

ACMD(do_policies)
{
}

int compareCharName(const void *l, const void *r) {
  struct char_data *left = *((struct char_data **)l);
  struct char_data *right = *((struct char_data **)r);

  if (!left || !right)
    return 0;
  return strcmp(GET_NAME(left), GET_NAME(right));
}

char *get_immtitle(struct char_data *ch, int imm_can_see) {
  struct char_guild_element *element;
  static char ptbuf[100];
  int clen, len;

  if (GET_DISGUISED(ch)) {
    if (imm_can_see)
      return "&R DISGUISED ";
    else
      return "           ";
  }

  if (GET_PRETITLE(ch)) {
    len  = strlen(GET_PRETITLE(ch));
    clen = c_strlen(GET_PRETITLE(ch));
    sprintf(ptbuf,"%-*s",11+(len-clen),GET_PRETITLE(ch));
    return ptbuf;
  }

  if (GET_LEVEL(ch) < 5)
    return "&cPoor Newbie";

  if (GET_LEVEL(ch) == 211)
    return "\\c08CREATOR    \\c00";

  if (IS_SET((ch)->player_specials->saved.command_groups, CG_OWNER))
    return "\\c11OWNER      \\c00"; 

  if (IS_SET((ch)->player_specials->saved.command_groups, CG_HCODER))
    return "\\c14HEADWEAVER \\c00";

  if (IS_SET((ch)->player_specials->saved.command_groups, CG_HCOUNCIL))
    return "\\c14HEADCOUNCIL\\c00";

  if (IS_SET((ch)->player_specials->saved.command_groups, CG_SM)) 
    return "\\c14STORYMASTER\\c00";

  if (IS_SET((ch)->player_specials->saved.command_groups, CG_HGL))   
    return "\\c14HEADGL     \\c00";

  if (IS_SET((ch)->player_specials->saved.command_groups, CG_HB))
    return "\\c14HEADBUILDER\\c00";

  if (IS_SET((ch)->player_specials->saved.command_groups, CG_GL)) {
   element = GET_CHAR_GUILDS(ch);
   while (element) {
    if (GLD_FLAGGED(element->guild, GLD_GLTITLE) && STATUS_FLAGGED(element->guildie, STATUS_GL))
     return element->guild->gl_title;
    element = element->next;
   }
  }

  if (IS_SET((ch)->player_specials->saved.command_groups, CG_WEAVER))
    return "\\c09WEAVER     \\c00";

  if (IS_SET((ch)->player_specials->saved.command_groups, CG_NEWBIEPR))
    return "\\c14NEWBIE PR  \\c00";

  if (IS_SET((ch)->player_specials->saved.command_groups, CG_WEBMASTER))
    return "\\c09WEBMASTER  \\c00";

  if (IS_SET((ch)->player_specials->saved.command_groups, CG_ST)) 
    return "\\c06STORYTELLER\\c00";

  if (IS_SET((ch)->player_specials->saved.command_groups, CG_BUILDER))
    return "\\c05BUILDER    \\c00"; 

  if (GET_LEVEL(ch) > 199)
    return "\\c05IMMORTAL   \\c00";

  element = GET_CHAR_GUILDS(ch);
  while (element) {
   if (GLD_FLAGGED(element->guild, GLD_GLTITLE) && STATUS_FLAGGED(element->guildie, STATUS_GL))
    return element->guild->gl_title;
   element = element->next;
  }

  if (IS_SET((ch)->player_specials->saved.command_groups, CG_NEWBIE))
    return "\\c05Newbieguide\\c00";

  if (GET_BM(ch) == 5 || GET_BM(ch) == 6)
   return "\\c14Blademaster\\c00";

  element = GET_CHAR_GUILDS(ch);
  while (element) {
   if (GLD_FLAGGED(element->guild, GLD_GUILDIESTITLE) && STATUS_FLAGGED(element->guildie, STATUS_MEMBER))
    return element->guild->guildie_titles;
   element = element->next;
  }

 return "           ";
}

void get_player_flags(char *flags, struct char_data *ch, int imm_can_see) {

  flags[0] = '\0';
  if (GET_INVIS_LEV(ch))
    sprintf(flags, "%s (i%d)", flags, GET_INVIS_LEV(ch));
  else if (IS_AFFECTED(ch, AFF_INVISIBLE))
    strcat(flags, " (invis)");
  if (GET_GUILD_INVIS(ch))
    strcat(flags, " (gi)");
  if (IS_AFFECTED(ch, AFF_VEILED))
    strcat(buf, " (veiled)");
  if (PLR_FLAGGED(ch, PLR_MAILING))
    strcat(flags, " (mailing)");
  else if (PLR_FLAGGED(ch, PLR_WRITING))
    strcat(flags, " (writing)");
  if (PRF_FLAGGED(ch, PRF_DEAF))
    strcat(flags, " (deaf)");
  if (PRF_FLAGGED(ch, PRF_NOTELL))
    strcat(flags, " (notell)");
  if (PRF_FLAGGED(ch, PRF_QUEST))
    strcat(flags, " (quest)");
  if (PRF2_FLAGGED(ch, PRF2_AFW))
    strcat(flags, " (AFW)");
  if (PRF_FLAGGED(ch, PRF_AFK))
    strcat(flags, " (AFK)");
  if (IS_AFFECTED(ch, AFF_DREAMING))
    strcat(flags, " (dreaming)");
}

#define WHO_NAME	1
#define WHO_GUILD	2
#define WHO_ROOM	3
#define WHO_ZONE	4
#define WHO_IC		5
#define WHO_DF		6
#define WHO_CLUB	7
#define WHO_NOBIL	8
#define WHO_OGIER	9
#define WHO_TOWER	10
#define WHO_CLAN	11
#define WHO_NEWBIE	12
#define WHO_WK		13
#define WHO_SORT	14
#define WHO_QUEST	15
#define WHO_ALLGUILD	16
#define WHO_NH		17
#define WHO_AWAY	18

#define SET_WHO_STATE(who_state)	SET_BIT(state, (1 << who_state));tmp_state=who_state
#define IS_WHO_STATE(who_state)		(IS_SET(state, (1 << who_state)))

#define WHO_FORMAT			"Format: \"who [-n name] [-g [guildname/number]] [-r] [-z] [-i] [-s] [-b] [-j] [-w] [-l lowlev] [-h highlev] [-a]\"\r\n"

ACMD(do_who)
{
  struct descriptor_data *d = NULL;
  struct char_data *wholist[MAX_PLAYERS], *tch = NULL;
  int num_can_see = 0, i = 0, tmp_state = 0, multi_states = 0, lowlev = 0, highlev = LVL_IMPL, guild_num;
  long state = 0;
  char *temp = NULL, arg[MAX_INPUT_LENGTH], *icness = NULL, flags[MAX_INPUT_LENGTH], name[MAX_NAME_LENGTH+1];
  char title[50] = "Characters Online";
  struct char_guild_element *element = NULL, *e2 = NULL;
  int is_gl = 0;
  int tmp_lvl = 1;
  
  if (IN_ROOM(ch) == NOWHERE)
    return;

  if (*argument)
    for (temp = argument; *temp != '\0'; temp++) 
    {
      if (isspace(*temp))
	continue;
      if (*temp != '-') 
      {
	send_to_char(WHO_FORMAT, ch);
	return;
      }
      multi_states++;
      switch(*(++temp)) 
      {
	case 'n':
	  sprintf(title, "Characters Online");
	  SET_WHO_STATE(WHO_NAME);
	  temp = one_argument(++temp, name);
	  temp--;
	  break;
	case 'g':
	  temp = one_argument(++temp, arg);
          temp--;
	  guild_num = atoi(arg);
          element = GET_CHAR_GUILDS(ch);
          while (element) 
	  {
	    if (element->guild->id == guild_num) break;
	    if (!strncasecmp(element->guild->name, arg, strlen(arg))) break;
	    element = element->next;
	  }
          if (!element) 
	  {
	    element = GET_CHAR_GUILDS(ch);
	    while (element) 
	    {
	      if (STATUS_FLAGGED(element->guildie, STATUS_GL)) break;
	      element = element->next;
	    }
          }
          if (!element)
	  {
	    element = GET_CHAR_GUILDS(ch);
	    while (element) 
	    {
	      if (GLD_FLAGGED(element->guild, GLD_EXCLUSIVE) && STATUS_FLAGGED(element->guildie, STATUS_MEMBER))
		break;
	      element = element->next;
	    }
	  }
          if (!element) 
	    element = GET_CHAR_GUILDS(ch);
          if (!element) 
	  {
	    send_to_char("You are not a part of any guild.\r\n", ch);
	    return;
          }
          if (!STATUS_FLAGGED(element->guildie, STATUS_MEMBER)) 
	  {
	    send_to_char("You are not a member of this guild.\r\n", ch);
	    return;
          }
          if (!STATUS_FLAGGED(element->guildie, STATUS_GL) 
	    && !IS_IMMORTAL(ch) 
	    &&  GLD_FLAGGED(element->guild, GLD_NOGWHO)) 
	  {
	    send_to_char("You are not allowed to see this guild's who list.\r\n", ch);
	    return;
          }

          sprintf(title, "Characters from the %s", element->guild->name);
	  SET_WHO_STATE(WHO_GUILD);
	  break;
	case 'r':
          sprintf(title, "Characters in the room");
	  SET_WHO_STATE(WHO_ROOM);
	  break;
	case 'z':
          sprintf(title, "Characters in the zone");
	  SET_WHO_STATE(WHO_ZONE);
	  break;
	case 'i':
          sprintf(title, "Characters who are IC");
	  SET_WHO_STATE(WHO_IC);
	  break;
	case 's':
	  if (!GET_NOBILITY(ch)) 
	  {
	    send_to_char("Sorry, you're not a noble one.\r\n", ch);
	    return;
	  }
          sprintf(title, "Servants online");
	  SET_WHO_STATE(WHO_NOBIL);
	  break;
	case 'b':
          sprintf(title, "Newbies Online");
	  SET_WHO_STATE(WHO_NEWBIE);
	  break;
	case 'l':
	  temp = one_argument(++temp, arg);
	  temp--;
	  lowlev = atoi(arg);
	  if (lowlev < 0)
	    lowlev = 0;
	  else if (lowlev > highlev)
	    lowlev = highlev;
          sprintf(title, "Characters above level %d", lowlev);
	  break;
	case 'h':
	  temp = one_argument(++temp, arg);
	  temp--;
	  highlev = atoi(arg);
	  if (highlev < lowlev)
	    highlev = lowlev;
	  else if (highlev > LVL_IMPL)
	    highlev = LVL_IMPL;
          sprintf(title, "Characters below level %d", highlev);
	  break;
	case 'a':
	  SET_BIT(state, (1 << WHO_SORT));
	  multi_states--;
          sprintf(title, "A sorted list of characters online");
	  break;
	case 'q':
          sprintf(title, "Characters participating in the current Quest");
	  SET_WHO_STATE(WHO_QUEST);
	  break;
	case 'j':
          sprintf(title, "Online Newbie-Guides"); 
	  SET_WHO_STATE(WHO_NH);
	  break;
	case 'w':
	  SET_WHO_STATE(WHO_AWAY);
	  multi_states--;
	  break;
	default:
          sprintf(title, "Characters Online");
	  send_to_char(WHO_FORMAT, ch);
	  return;
      }
    }

  if (!*title)
   sprintf(title, "Characters Online");

  if (multi_states > 1)
    tmp_state = 0;

  for (d = descriptor_list, num_can_see = 0; d != NULL && num_can_see < MAX_PLAYERS; d = d->next) 
  {
    if (STATE(d) != CON_PLAYING && STATE(d) < CON_OEDIT)
      continue;
    if (d->character == NULL)
      continue;
    if (IN_ROOM(d->character) == NOWHERE)
      continue;
    if (!CAN_SEE_INCLUDING_TAR(ch, d->character))
      continue;
    /* For making the obscure skill change level in who -h and who -l*/
    tmp_lvl = (GET_LEVEL(d->character));
    if (d->character->player_specials->fool_level)
      tmp_lvl = (d->character->player_specials->fool_level);
    if (tmp_lvl < lowlev || tmp_lvl > highlev)
      continue;
    if( IS_WHO_STATE(WHO_NAME) ) 
    {
      if (strncasecmp(GET_NAME(d->character), name, strlen(name)))
        continue;
    }
    if( IS_WHO_STATE(WHO_GUILD) )
    {
      e2 = GET_CHAR_GUILDS(d->character);
      while (e2) 
      {
	if ((element->guild == e2->guild) && STATUS_FLAGGED(e2->guildie, STATUS_MEMBER)) break;
	e2 = e2->next;
      }
      if (!e2) continue;
    }
    if (IS_WHO_STATE(WHO_ROOM)) 
    {
      if( IN_ROOM(ch) != IN_ROOM(d->character))
      continue;
    }
    if (IS_WHO_STATE(WHO_ZONE)) 
    {
      if (world[IN_ROOM(ch)].zone != world[IN_ROOM(d->character)].zone)
      continue;
    }
    if (IS_WHO_STATE(WHO_IC)) 
    {
      if (!PRF_FLAGGED(d->character, PRF_IC))
      continue;
    }
    if (IS_WHO_STATE(WHO_NOBIL)) 
    {
      if (*GET_SERVANT(d->character)) 
      {
	if (!isname(GET_NAME(ch), GET_SERVANT(d->character)))
        continue;
      }
      else
	continue;
    }
    if (IS_WHO_STATE(WHO_NEWBIE)) 
    {
      if (GET_LEVEL(d->character) > 20)
      continue;
    }
    if (IS_WHO_STATE(WHO_QUEST)) 
    {
      if (!PRF_FLAGGED(d->character, PRF_QUEST))
      continue;
    }
    if (IS_WHO_STATE(WHO_NH)) 
    {
      if (!IS_SET((d->character->player_specials->saved.command_groups), CG_NEWBIE))
      continue;
    }
    wholist[num_can_see++] = d->character;
  }

  if (IS_WHO_STATE(WHO_SORT))
    qsort(wholist, num_can_see, sizeof(struct char_data *), compareCharName);

  buf[0] = '\0';
  for (i = 0, tch = NULL; i < num_can_see; i++) 
  {
    tch = wholist[i];
    icness = (PRF_FLAGGED(tch, PRF_IC) ? "&gIC&n " : "&rOOC&n");
    switch(tmp_state) 
    {
      case WHO_GUILD:
	if (STATUS_FLAGGED(element->guildie, STATUS_GL) || GET_LEVEL(ch) >= LVL_GRGOD)
	is_gl = 1;
	e2 = GET_CHAR_GUILDS(tch);
	while (e2) 
	{
	  if (e2->guild == element->guild) break;
	  e2 = e2->next;
	}
	sprintf(buf2, "%s", (e2->guildie->rank_num != 0) ? e2->guildie->rank->name : "Unranked");
	if (STATUS_FLAGGED(e2->guildie, STATUS_SUBRANK))
	  sprintf(buf2, "%s &W[&n%s&W]", buf2, e2->guildie->subrank);
	sprintf(buf, "%s&W[&wGchan %s&W][&n%s&W][&n%s&W]&n &r%s&n %s&n %s&n", 
	  buf,
	  (STATUS_FLAGGED(e2->guildie, STATUS_CHAN_ON) ? "&gON " : "&rOFF"), 
	  icness, 
	  buf2,
	  ((GET_NOBILITY(tch) && (!GLD_FLAGGED(e2->guild, GLD_SECRETIVE) || is_gl)) ?  GET_NOBILITY(tch) : ""), 
	  ((GLD_FLAGGED(element->guild, GLD_SECRETIVE) && !is_gl) ? "Someone" : GET_NAME(tch)),
	  ((GLD_FLAGGED(element->guild, GLD_SECRETIVE) && !is_gl) ? "" : GET_TITLE(tch))
	);
	break;
      default:
	sprintf( buf, "%s&W[&n%s&W][&n%s&W]&n &r%s&n %s&n %s&n", 
	  buf, 
	  get_immtitle(tch, IS_IMMORTAL(ch)), 
	  icness, (GET_NOBILITY(tch) ? GET_NOBILITY(tch) : ""), 
	  GET_NAME(tch), 
	  GET_TITLE(tch)
	);
	break;
    }
    get_player_flags(flags, tch, IS_IMMORTAL(ch));
    sprintf(buf, "%s%s\r\n", buf, flags);
    if( IS_WHO_STATE( WHO_AWAY ) )
    {
      if( GET_AFK( tch ) )
      {
	char  *tmpstr = strdup( GET_AFK( tch ) );
	sprintf( buf, "%s                    %s\r\n", buf, strtok( tmpstr, "\r\n" ) );
	free( tmpstr );
      }
    }
  }
  max_logged_on = MAX(num_can_see, max_logged_on);
  sprintf(buf, "%s\r\n%d player%s displayed.\r\n"
        "Maximum of %d this reboot/hotboot.\r\n",
        buf, num_can_see, (num_can_see == 1 ? "" : "s"), max_logged_on);

  sprintf(buf2, "\r\n   &c%s\r\n   ", title);
  for (i = 0; i < strlen(title); i++)
    strcat(buf2, "=");
  strcat(buf2, "&n\r\n\r\n");
  strcat(buf2, buf);
  page_string(ch->desc, buf2, 1);
}

#define USERS_FORMAT \
"format: users [-l minlevel[-maxlevel]] [-n name] [-h host] [-o] [-p]\r\n"

/*
 * Guess this is used to see if more than one character is connected from 
 * the same host.  
 */
int find_multi( struct char_data *ch, int *iImm, int *iMort, int *iUnauth )
{
  char name_search[MAX_INPUT_LENGTH], host_search[MAX_INPUT_LENGTH];
  struct descriptor_data *d, *d2;
  int found_imm=0, found_mort = 0, unauth = 0;

  host_search[0] = name_search[0] = '\0';

  for (d = descriptor_list; d; d = d->next) 
    for ( d2 = d->next; d2; d2= d2->next) 
    {
      if (d2->connected || d->connected || !d2->host || !d->host || !d->character || !d2->character )
        continue;
      if ( ch && (!CAN_SEE(ch, d->character) || !CAN_SEE(ch, d2->character)) )
        continue;
      if(!strcmp(d2->host,d->host) && strcmp(GET_NAME(d2->character), GET_NAME(d->character)) !=0 )
      { 
	if (IS_IMMORTAL(d->character) || IS_IMMORTAL(d2->character))
	{
	  if( ch )
	    asend_to_char(ch,"Both &W%s&n and &W%s&n are connected from %s.\r\n",
                        GET_NAME(d2->character),GET_NAME(d->character),d->host);
           found_imm++;
        }
        else
        {
	  if( ch )
          {
	    asend_to_char( ch, "Both " );
            if( PLR_FLAGGED( d2->character, PLR_CANMULTI ) )
              asend_to_char( ch, "&c" );
            else
              asend_to_char( ch, "&r" );
	    asend_to_char( ch, "%s&n and ", GET_NAME(d2->character) );
            if( PLR_FLAGGED( d->character, PLR_CANMULTI ) )
              asend_to_char( ch, "&c" );
            else
              asend_to_char( ch, "&r" );
	    asend_to_char( ch, "%s&n are connected from %s.\r\n", GET_NAME(d->character), d->host );
          }
          found_mort++;
          if( (!PLR_FLAGGED( d->character, PLR_CANMULTI )) || (!PLR_FLAGGED( d2->character, PLR_CANMULTI )) )
            unauth++;
        }
      }
    }
  if( iImm )
    *iImm = found_imm;
  if( iMort )
    *iMort = found_mort;
  if( iUnauth )
    *iUnauth = unauth;
  return unauth;
}

ACMD(do_multi)
{
  int found_imm=0, found_mort=0;
  find_multi( ch, &found_imm, &found_mort, NULL );
  if (found_imm || found_mort)
  {
    asend_to_char(ch, "\r\nReport: %d true multiplayer matches, and %d Imm-related matches.\r\n", found_mort, found_imm);
    send_to_char("No more matches found!\r\n",ch);
  }
  else
    send_to_char("No matches at all found.\r\n",ch);
}

ACMD(do_users)
{
  char line[200], line2[220], idletime[10], classname[20];
  char state[30], *timeptr, *format, mode;
  char name_search[MAX_INPUT_LENGTH], host_search[MAX_INPUT_LENGTH];
  struct char_data *tch;
  struct descriptor_data *d;
  int low = 0, high = LVL_IMPL, num_can_see = 0;
  int outlaws = 0, playing = 0, deadweight = 0;

  host_search[0] = name_search[0] = '\0';

  strcpy(buf, argument);
  while (*buf) {
    half_chop(buf, arg, buf1);
    if (*arg == '-') {
      mode = *(arg + 1);  /* just in case; we destroy arg in the switch */
      switch (mode) {
      case 'o':
      case 'k':
    outlaws = 1;
    playing = 1;
    strcpy(buf, buf1);
    break;
      case 'p':
    playing = 1;
    strcpy(buf, buf1);
    break;
      case 'd':
    deadweight = 1;
    strcpy(buf, buf1);
    break;
      case 'l':
    playing = 1;
    half_chop(buf1, arg, buf);
    sscanf(arg, "%d-%d", &low, &high);
    break;
      case 'n':
    playing = 1;
    half_chop(buf1, name_search, buf);
    break;
      case 'h':
    playing = 1;
    half_chop(buf1, host_search, buf);
    break;
      default:
    send_to_char(USERS_FORMAT, ch);
    return;
    break;
      }				/* end of switch */

    } else {			/* endif */
      send_to_char(USERS_FORMAT, ch);
      return;
    }
  }				/* end while (parser) */
  strcpy(line,
     "Num Class     Name         State          Idl Login@   Site\r\n");
  strcat(line,
     "--- --------- ------------ -------------- --- -------- ------------------------\r\n");
  send_to_char(line, ch);

  one_argument(argument, arg);

  for (d = descriptor_list; d; d = d->next) {
    if (d->connected && playing)
      continue;
    if (!d->connected && deadweight)
      continue;
    if (!d->connected) {
      if (d->original)
    tch = d->original;
      else if (!(tch = d->character))
    continue;

      if (*host_search && !strstr(d->host, host_search))
    continue;
      if (*name_search && str_cmp(GET_NAME(tch), name_search))
    continue;
      if (!CAN_SEE(ch, tch) || GET_LEVEL(tch) < low || GET_LEVEL(tch) > high)
    continue;
      if (GET_INVIS_LEV(ch) > GET_LEVEL(ch))
    continue;

      if (d->original)
    sprintf(classname, "[%3ld %3s]", GET_LEVEL(d->original),
        CLASS_ABBR(d->original));
      else
    sprintf(classname, "[%3ld %3s]", GET_LEVEL(d->character),
        CLASS_ABBR(d->character));
    } else
      strcpy(classname, "    -    ");

    timeptr = asctime(localtime(&d->login_time));
    timeptr += 11;
    *(timeptr + 8) = '\0';

    if (!d->connected && d->original)
      strcpy(state, "Switched");
    else
      strcpy(state, connected_types[d->connected]);

    if (d->character && !d->connected && GET_LEVEL(d->character) < LVL_IMMORT)
      sprintf(idletime, "%3ld", d->character->char_specials.timer *
          SECS_PER_MUD_HOUR / SECS_PER_REAL_MIN);
    else
      strcpy(idletime, "");

    format = "%3d %-7s %-12s %-14s %-3s %-8s ";

    if (d->character && GET_RNAME(d->character)) {
      if (d->original)
    sprintf(line, format, d->desc_num, classname,
        GET_RNAME(d->original), state, idletime, timeptr);
      else
    sprintf(line, format, d->desc_num, classname,
        GET_RNAME(d->character), state, idletime, timeptr);
    } else
      sprintf(line, format, d->desc_num, "   -   ", "UNDEFINED",
          state, idletime, timeptr);

    if (d->host && *d->host)
      sprintf(line + strlen(line), "[%s]\r\n", d->host);
    else
      strcat(line, "[Hostname unknown]\r\n");

    if (d->connected) {
      sprintf(line2, "%s%s%s", CCGRN(ch, C_SPR), line, CCNRM(ch, C_SPR));
      strcpy(line, line2);
    }
    if (d->connected || (!d->connected && CAN_SEE(ch, d->character))) {
      send_to_char(line, ch);
      num_can_see++;
    }
  }

  sprintf(line, "\r\n%d visible sockets connected.\r\n", num_can_see);
  send_to_char(line, ch);
}


/* Generic page_string function for displaying text */
ACMD(do_gen_ps)
{
  switch (subcmd) {
  case SCMD_CREDITS:
    page_string(ch->desc, credits, 0);
    break;
  case SCMD_NEWS:
    page_string(ch->desc, news, 0);
    break;
  case SCMD_OLDNEWS:
    page_string(ch->desc, oldnews, 0);
    break;
  case SCMD_INFO:
    page_string(ch->desc, info, 0);
    break;
  case SCMD_WIZLIST:
    page_string(ch->desc, wizlist, 0);
    break;
  case SCMD_IMMLIST:
    page_string(ch->desc, immlist, 0);
    break;
  case SCMD_HANDBOOK:
    page_string(ch->desc, handbook, 0);
    break;
  case SCMD_MOTD:
    page_string(ch->desc, motd, 0);
    break;
  case SCMD_IMOTD:
    page_string(ch->desc, imotd, 0);
    break;
  case SCMD_CLEAR:
    send_to_char("\033[H\033[J", ch);
    break;
  case SCMD_VERSION:
    send_to_char(circlemud_version, ch);
    send_to_char(strcat(strcpy(buf, DG_SCRIPT_VERSION), "\r\n"), ch);
    break;
  case SCMD_WHOAMI:
    sprintf(buf, "%s\r\n", GET_NAME(ch));
    send_to_char(buf, ch);
    break;
  case SCMD_BUGS:
    page_string(ch->desc, bugs, 0);
    break;
  case SCMD_IDEAS:
  { char  *temp;
    CREATE( temp, char, strlen( ideas ) + strlen( newideas ) + 2 );
    memcpy( temp, ideas, strlen( ideas ) );
    memcpy( temp+strlen( ideas ), newideas, strlen( newideas )+1 );
    page_string(ch->desc, temp, 1);
    free( temp );
    break;
  }
  case SCMD_SOCIALIDEAS:
    page_string(ch->desc, socialideas, 0);
    break;
  case SCMD_TYPOS:
    page_string(ch->desc, typos, 0);
    break;
  case SCMD_TODOS:
    page_string(ch->desc, todo, 0);
    break;
  case SCMD_CRETODOS:
    page_string(ch->desc, cretodo, 0);
    break;
  default:
    return;
    break;
  }
}


void perform_mortal_where(struct char_data * ch, char *arg)
{
  register struct char_data *i;
  register struct descriptor_data *d;

  if (!*arg) {
    send_to_char("Players in your Zone\r\n--------------------\r\n", ch);
    for (d = descriptor_list; d; d = d->next)
      if (!d->connected) {
    i = (d->original ? d->original : d->character);
    if (i && CAN_SEE(ch, i) && (i->in_room != NOWHERE) &&
        (world[ch->in_room].zone == world[i->in_room].zone)) {
      sprintf(buf, "%-20s - %s\r\n", GET_NAME(i), world[i->in_room].name);
      send_to_char(buf, ch);
    }
      }
  } else {			/* print only FIRST char, not all. */
    for (i = character_list; i; i = i->next)
      if (world[i->in_room].zone == world[ch->in_room].zone && CAN_SEE(ch, i) &&
      (i->in_room != NOWHERE) && isname(arg, GET_NAME(i))) {
    sprintf(buf, "%-25s - %s\r\n", GET_NAME(i), world[i->in_room].name);
    send_to_char(buf, ch);
    return;
      }
    send_to_char("No-one around by that name.\r\n", ch);
  }
}


bool can_see_object( struct obj_data * obj, struct char_data * ch, int recur )
{
  if (obj->carried_by) 
  {
    if( !CAN_SEE( ch, obj->carried_by ) )
      return FALSE;
  } 
  else if (obj->worn_by) 
  {
    if( !CAN_SEE( ch, obj->worn_by ) )
      return FALSE;
  } 
  else if (obj->in_obj) 
  {
    if (recur)
      return can_see_object( obj->in_obj, ch, recur);
  } 
  return TRUE;
}

void print_object_location( int num, struct obj_data * obj, struct char_data * ch, int recur )
{
  if( num > 0 )
    sprintf(buf, "O%3d. %-25s - ", num, obj->short_description);
  else
    sprintf(buf, "%33s", " - ");

  if (obj->in_room > NOWHERE) 
  {
    sprintf(buf + strlen(buf), "[%5d] %s\n\r",
        world[obj->in_room].number, world[obj->in_room].name);
    send_to_char(buf, ch);
  } 
  else if (obj->carried_by) 
  {
    sprintf(buf + strlen(buf), "carried by %s\n\r",
        PERS(obj->carried_by, ch));
    send_to_char(buf, ch);
  } 
  else if (obj->worn_by) 
  {
    sprintf(buf + strlen(buf), "worn by %s\n\r",
        PERS(obj->worn_by, ch));
    send_to_char(buf, ch);
  } 
  else if (obj->in_obj) 
  {
    sprintf(buf + strlen(buf), "inside %s%s\n\r",
        obj->in_obj->short_description, (recur ? ", which is" : " "));
    send_to_char(buf, ch);
    if (recur)
      print_object_location(0, obj->in_obj, ch, recur);
  } 
  else 
  {
    sprintf(buf + strlen(buf), "in an unknown location\n\r");
    send_to_char(buf, ch);
  }
}



void perform_immort_where(struct char_data * ch, char *arg)
{
  register struct char_data *i;
  register struct obj_data *k;
  struct descriptor_data *d;
  int num = 0, found = 0, vnum = 0, mob = 0, obj = 0;
  struct char_guild_element *element = NULL;
  
  element = GET_CHAR_GUILDS(ch);

  if (!*arg) 
  {
    send_to_char("Players\r\n-------\r\n", ch);
    for (d = descriptor_list; d; d = d->next)
      if (!d->connected) 
      {
	i = (d->original ? d->original : d->character);
	if (i && CAN_SEE(ch, i) && (i->in_room != NOWHERE)) 
	{
	  if (d->original)
	    sprintf(buf, "%-20s - [%5d] %s (in %s)\r\n",
		GET_NAME(i), 
		world[d->character->in_room].number,
		world[d->character->in_room].name, 
		GET_NAME(d->character));
	  else
	    sprintf(buf, "%-20s - [%5d] %s\r\n", 
		GET_NAME(i),
		world[i->in_room].number, 
		world[i->in_room].name);
	  send_to_char(buf, ch);
	}
      }
  } 
  else 
  {
    skip_spaces(&arg);
    if (!strncasecmp(arg, "-vm", 3) || !strncasecmp(arg, "-vo", 3) )
    {
      if (!strncasecmp(arg, "-vm", 3))
        mob = 1;
      else if (!strncasecmp(arg, "-vo", 3))
        obj = 1;
       
      /* Probably need some changes in the code below or check will be odd? */
      
      arg += 3;
      vnum = atoi(arg);
      if (vnum <= 0)
      {
        asend_to_char(ch, "'%s' is not a valid vnum. Try again.\r\n", arg );
        asend_to_char(ch, "Tip: Don't put spaces betwen -vm or -vo and the number.\r\n" );
        return;
      }
    }
     
    if( !obj ) // No search for players or mobs if obj is set
    {
      for (i = character_list; i; i = i->next)
      {
        if (CAN_SEE(ch, i) && i->in_room)
        {
          if ((!mob && isname(arg, GET_NAME(i))) ||         // match for name
              (mob && IS_NPC(i) && (GET_MOB_VNUM(i) == vnum)))   // match for vnum, mobs only
          {
            found = 1;
            sprintf(buf, "M%3d. %-25s - [%5d] %s\r\n", ++num, GET_NAME(i),
              world[i->in_room].number, world[i->in_room].name);
            send_to_char(buf, ch);
          }
        }
      }
    } // if (!obj)

    if( !mob ) // No search for objs if mob is set
    {
      for (num = 0, k = object_list; k; k = k->next)
      {
	//if( CAN_SEE_OBJ(ch, k) )
	if( can_see_object(k, ch, TRUE) )
	{
	  if ((!vnum && isname(arg, k->name)) || // match for name
            (vnum && (GET_OBJ_VNUM(k) == vnum))) // match for vnum
	  {
	    found = 1;
	    print_object_location(++num, k, ch, TRUE);
	  }
	}
      }
    }
    
    if (!found)
      send_to_char("Couldn't find any such thing.\r\n", ch);
  }
}

ACMD(do_where)
{
  one_argument(argument, arg);

  if (GET_LEVEL(ch) >= LVL_IMMORT)
    perform_immort_where(ch, arg);
  else
    perform_mortal_where(ch, arg);
}

ACMD(do_whereg)
{
  register struct char_data *i;
  struct descriptor_data *d;
  int guild_num;
  struct char_guild_element *element = NULL;
  
  one_argument(argument, arg);

  guild_num = atoi(arg);
  element = GET_CHAR_GUILDS(ch);
  while (element)
  {
    if (element->guild->id == guild_num) break;
    if (!strncasecmp(element->guild->name, arg, strlen(arg))) break;
    element = element->next;
  }

  if (!element)
  {  
    element = GET_CHAR_GUILDS(ch);
    while (element)
    {
      if (STATUS_FLAGGED(element->guildie, STATUS_GL)) break;
      element = element->next;
    }
  }

  if (!element)
  {
    element = GET_CHAR_GUILDS(ch);
    while (element)
    {
      if (GLD_FLAGGED(element->guild, GLD_EXCLUSIVE) && STATUS_FLAGGED(element->guildie, STATUS_MEMBER)) break;
      element = element->next;
    }
  }
  
  if (!element)
  {
    send_to_char("You are not part of any guild!\r\n", ch);
    return;
  }

  guild_num = element->guild->id;

  sprintf(buf, "Players in %s\r\n"
        "-------\r\n", element->guild->name);
  for (d = descriptor_list; d; d = d->next) {
    if (d->connected)
      continue;

    i = (d->original ? d->original : d->character);

    if (i && CAN_SEE(ch, i) && (i->in_room != NOWHERE) && get_guild(i, guild_num)) {
      if (d->original)
    sprintf(buf, "%s%-20s - [%5d] %s (in %s)\r\n", buf,
            GET_NAME(i), world[d->character->in_room].number,
            world[d->character->in_room].name, GET_NAME(d->character));
      else
    sprintf(buf, "%s%-20s - [%5d] %s\r\n", buf, GET_NAME(i),
            world[i->in_room].number, world[i->in_room].name);
    }
  }
  page_string(ch->desc, buf, 1);
}


ACMD(do_consider)
{
  struct char_data *victim;
  int i, diff, exp;
  float fexp;
  char buf[255], param[255];
  char *name;
  int pthac0, vthac0, pdam, vdam;
  int phitp, phtk, pahtk;
  int vhitp, vhtk, vahtk;
  int hit_diff, dam_diff, hp_diff;
  int AC_diff;
  int attack_skills[] = 
  { SKILL_SECOND_ATTACK, 
    SKILL_THIRD_ATTACK, 
    SKILL_FOURTH_ATTACK, 
    SKILL_FIFTH_ATTACK, 
    SKILL_WARDERCOMBAT, 
    SKILL_HASTE, 
    SKILL_STUBBORNESS, 
    SKILL_SPEARDANCE, 
    0 
  };

  if (NULL == ch)
    return;

  two_arguments(argument, buf, param);

  if (!(victim = get_char_room_vis(ch, buf))) {
    send_to_char("Consider killing who?\r\n", ch);
    return;
  }
  if (victim == ch) {
    send_to_char("Easy!  Very easy indeed!\r\n", ch);
    return;
  }
  /*if (!IS_NPC(ch) && victim->player_specials->fool_level)
    level = (victim->player_specials->fool_level*100)/GET_LEVEL(victim);*/

  if( IS_NPC( victim ) )
    name = victim->player.short_descr;
  else
    name = victim->player.name;
 
  /* figure chance of player to hit*/
  pthac0 = MIN( 100, chance_to_hit( ch, victim, -1, 0 ) );
  if ( !IS_NPC( ch ) )
    for ( i = 0; attack_skills[i]; i++)
      //if (GET_SKILL(ch,attack_skills[i]))
	pthac0 += GET_SKILL( ch, attack_skills[i] );
  vthac0 = MIN( 100, chance_to_hit( victim, ch, -1, 0 ) );
  if ( !IS_NPC( victim ) )
    for ( i = 0; attack_skills[i]; i++)
      //if (GET_SKILL( victim,attack_skills[i]))
	vthac0 += GET_SKILL( victim, attack_skills[i] );

  /*figure players damage*/
  pdam = avg_damage( ch, victim, 0 );
  vdam = avg_damage( victim, ch, 0 );

  /*figure number of hits to kill victim*/
  vhitp = GET_HIT( victim ); 
  if( pdam )
  { phtk = vhitp/pdam+1; 
    if( pthac0 )
      pahtk = (phtk*100)/pthac0+1;
    else pahtk = 0;
  }
  else 
    phtk = pahtk = 0;

  phitp = GET_HIT( ch ); 
  if( vdam )
  { vhtk = phitp/vdam+1; 
    if( vthac0 )
      vahtk = (vhtk*100)/vthac0+1;
    else vahtk = 0;
  }
  else 
    vhtk = vahtk = 0;

  fexp = ((GET_LEVEL(ch)-GET_LEVEL(victim))/10.0) + 5.0;
  exp = (int) (25.0*(GET_EXP(victim)+100.0)/(fexp*fexp) - 100.0);
  if (exp <= 0)   exp = 1;

  /*print results*/
  if( !str_cmp( param, "secrets" ) /*&& IS_IMMORTAL( ch )*/ )
  {
    sprintf( buf, "You vs. %s\r\nYour chance to hit: %d\r\n", victim->player.name, pthac0 );
    send_to_char( buf, ch );
    sprintf( buf, "avg damage per hit: %d\r\n", pdam );
    send_to_char( buf, ch );
    sprintf( buf, "Hit points to reduce: %d\r\n", vhitp );
    send_to_char( buf, ch );
    sprintf( buf, "Hits to kill: %d\r\n", phtk );
    send_to_char( buf, ch );
    sprintf( buf, "Rounds for %d hits: %d\r\n", phtk, pahtk );
    send_to_char( buf, ch );
  
    sprintf( buf, "\r\n%s vs. you\r\nYour chance to be hit: %d\r\n", victim->player.name, vthac0 );
    send_to_char( buf, ch );
    sprintf( buf, "avg damage per hit: %d\r\n", vdam );
    send_to_char( buf, ch );
    sprintf( buf, "Hit points to protect: %d\r\n", phitp );
    send_to_char( buf, ch );
    sprintf( buf, "Hits to kill: %d\r\n", vhtk );
    send_to_char( buf, ch );
    sprintf( buf, "Rounds for %d hits: %d\r\n", vhtk, vahtk );
    send_to_char( buf, ch );
    sprintf( buf, "To gain in exp: %d\r\n", exp );
    send_to_char( buf, ch );
  } 

  diff = vahtk-pahtk;

  hit_diff = pthac0 - vthac0;
  dam_diff = (pdam * 100) / (pdam + vdam);
  hp_diff = (phitp * 100) / (phitp + vhitp);
  AC_diff = GET_AC( victim ) - GET_AC( ch );

  sprintf( buf, "You consider attacking %s.\r\n", name );
  send_to_char( buf, ch );
  if( AC_diff >= 50 )
    send_to_char( "You are &Gmuch b&getter&n ar&Wm&nored than your opponent.&n\r\n", ch );
  else if( AC_diff >= 25 )
    send_to_char( "You are &Gb&getter&n ar&Wm&nored than your opponent.n\r\n", ch );
  else if( AC_diff >= -25 )
    send_to_char( "You and your opponent are about &Ye&yqually&n ar&Wm&nored.\r\n", ch );
  else if( AC_diff >= -50 )
    send_to_char( "Your opponent is &Rb&retter&n ar&Wm&nored than you.\r\n", ch );
  else
    send_to_char( "Your opponent is &Rmuch b&retter&n ar&Wm&nored than you.\r\n", ch );
    
  if( hit_diff >= 20 )
    send_to_char( "You are &Gm&guch mor&Ge&n s&Wk&nilled than your opponent.\r\n", ch );
  else if (hit_diff >= 5 )
    send_to_char( "You are &Gm&gor&Ge&n s&Wk&nilled than your opponent.\r\n", ch );
  else if (hit_diff >= -5 )
    send_to_char( "You and your opponent are about &Yequally&n s&Wk&nilled.\r\n", ch );
  else if (hit_diff >= -20 )
    send_to_char( "Your opponent is &Rm&ror&Re&n s&Wk&nilled than you.\r\n", ch );
  else
    send_to_char( "Your opponent is &Rm&ruch mor&Re&n s&Wk&nilled than you.\r\n", ch );

  if( hp_diff >= 80 )
    send_to_char( "You can take &Gm&gany &Gm&gore&n hit&Ws&n than your opponent.\r\n", ch );
  else if( hp_diff >= 60 )
    send_to_char( "You can take &Gm&gore&n hit&Ws&n than your opponent.\r\n", ch );
  else if( hp_diff >= 40 )
    send_to_char( "You and your opponent are about &Yequally&n in shap&We&n.\r\n", ch );
  else if( hp_diff >= 20 )
    send_to_char( "Your opponent can take &Rm&rore&n hit&Ws&n than you.\r\n", ch );
  else
    send_to_char( "Your opponent can take &Rm&rany &Rm&rore&n hit&Ws&n than you.\r\n", ch );

  if( dam_diff >= 80 )
    send_to_char( "You will inflict &Ga lot&n more &Wdamage&n than your opponent.\r\n", ch );
  else if( dam_diff >= 60 )
    send_to_char( "You will inflict &gmore &Wdamage&n than your opponent.\r\n", ch );
  else if( dam_diff >= 40 )
    send_to_char( "You and your opponent will inflict about the &Ysame&n amount of &Wdamage&n.\r\n", ch );
  else if( dam_diff >= 20 )
    send_to_char( "You will inflict &rless &Wdamage&n than your opponent.\r\n", ch );
  else
    send_to_char( "You will inflict &Ra lot&n less &Wdamage&n than your opponent.\r\n", ch );

  if( pahtk == 1 && vahtk == 1 )
    send_to_char( "First one to hit is &Ylikely&n to win.\r\n", ch );
  else if( pahtk > 2 && vahtk == 1 )
    send_to_char( "&RO&rne h&Ri&rt&n will do you in.\r\n", ch );
  else if( pahtk == 1 && vahtk > 2 )
    send_to_char( "You can do it in &Go&gne goo&Gd&g h&Gi&gt.&n\r\n", ch );
  else if( diff > 10 )
    send_to_char( "Your overall chances are &Gv&gery go&Go&gd&n.\r\n", ch );
  else if (diff >= 5 )
    send_to_char( "Your overall chances are &ggo&Go&gd&n.\r\n", ch );
  else if (diff >= -5 )
    send_to_char( "Your overall chances are &Yequal&n.\r\n", ch );
  else if (diff >= -10 )
    send_to_char( "Your overall chances are &rPo&Ro&rr&n.\r\n", ch );
  else if (diff >= -20 )
    send_to_char( "Your overall chances are &RV&rery Po&Ro&rr&n.\r\n", ch );
  else
    send_to_char( "&RAre you insane?&n\r\n", ch );

  if ( exp > 500 )
    send_to_char( "A lot can be &Gg&ga&Gi&gn&Ge&gd&n by defeating this opponent\r\n", ch );
  else if ( exp > 300 )
    send_to_char( "There is something to be &Cg&ca&Ci&cn&Ce&cd&n by defeating this opponent\r\n", ch );
  else if ( exp > 100 )
    send_to_char( "Have you considered if this opponent is &Yw&yo&Yr&yt&Yh&n the trouble?\r\n", ch );
  else
    send_to_char( "This opponent is not &Rw&ro&Rr&rt&Rh&n the effort.\r\n", ch );

  send_to_char( "\r\n", ch );
}



ACMD(do_diagnose)
{
  struct char_data *vict;

  one_argument(argument, buf);

  if (*buf) {
    if (!(vict = get_char_room_vis(ch, buf))) {
      send_to_char(NOPERSON, ch);
      return;
    } else
      diag_char_to_char(vict, ch);
  } else {
    if (FIGHTING(ch))
      diag_char_to_char(FIGHTING(ch), ch);
    else
      send_to_char("Diagnose who?\r\n", ch);
  }
}


static char *ctypes[] = {
"off", "sparse", "normal", "complete", "\n"};

ACMD(do_color)
{
  int tp;

  if (IS_NPC(ch))
    return;

  one_argument(argument, arg);

  if (!*arg) {
    sprintf(buf, "Your current color level is %s.\r\n", ctypes[COLOR_LEV(ch)]);
    send_to_char(buf, ch);
    return;
  }
  if (((tp = search_block(arg, (const char **)ctypes, FALSE)) == -1)) {
    send_to_char("Usage: color { Off | Sparse | Normal | Complete }\r\n", ch);
    return;
  }
  REMOVE_BIT(PRF_FLAGS(ch), PRF_COLOR_1);
  REMOVE_BIT(PRF_FLAGS(ch), PRF_COLOR_2);
  SET_BIT(PRF_FLAGS(ch), (PRF_COLOR_1 * (tp & 1)));
  SET_BIT(PRF_FLAGS(ch), (PRF_COLOR_2 * (tp & 2) >> 1));
  sprintf(buf, "Your %scolor%s is now %s.\r\n", CCRED(ch, C_SPR),
      CCNRM(ch, C_OFF), ctypes[tp]);
  send_to_char(buf, ch);
}


ACMD(do_toggle)
{
  struct char_guild_element *element;
  int i;
  
  if (IS_NPC(ch))
    return;

  argument = any_one_arg(argument, arg);

  if (arg && *arg) 
  {
    //processes Away From toggles
    skip_spaces(&argument);   
    if (!str_cmp(arg, "afk")) 
    {
      if ((!argument || !*argument) && IS_SET(PRF_FLAGS(ch),PRF_AFK)) do_noafk(ch, 0, cmd, 0);
      else do_afk(ch, argument, cmd, 0);
      return;
    } 
    if (!str_cmp(arg, "afw")) 
    {
      if ((!argument || !*argument) && IS_SET(PRF2_FLAGS(ch),PRF2_AFW)) do_noafw(ch, 0, cmd, 0);
      else  do_afw(ch, argument, cmd, 0);
      return;
    } 

    //Check for guild related toggles
    element = GET_CHAR_GUILDS(ch);
    while (element) 
    {
      //guild channel toggles
      if (!strncmp(element->guild->gchan_name, arg, strlen(element->guild->gchan_name)) 
	&& !GLD_FLAGGED(element->guild, GLD_NOGCHAN)) 
      {
	if (STATUS_FLAGGED(element->guildie, STATUS_CHAN_ON))
	{
	  sprintf(buf, "You will no longer hear %s.\r\n", element->guild->gchan_name);
	  send_to_char(buf, ch);
	  REMOVE_BIT(element->guildie->status, STATUS_CHAN_ON);
	  return;
	}
	else 
	{
	  sprintf(buf, "You will now hear %s.\r\n", element->guild->gchan_name);
	  send_to_char(buf, ch);
	  SET_BIT(element->guildie->status, STATUS_CHAN_ON);
	  return;
	}
      }
      //guild channel tag toggles
      sprintf(buf, "tag-%s", element->guild->gchan_name);
      if (!strncmp(buf, arg, strlen(buf)) 
	&& !GLD_FLAGGED(element->guild, GLD_NOGCHAN)) 
      {
	if (STATUS_FLAGGED(element->guildie, STATUS_TAG)) 
	{
	  sprintf(buf, "You will no longer see the gchan tag in %s.\r\n", element->guild->gchan_name);
	  send_to_char(buf, ch);  
	  REMOVE_BIT(element->guildie->status, STATUS_TAG);
	  return;
	}
	else 
	{
	  sprintf(buf, "You will now see the gchan tag in %s.\r\n", element->guild->gchan_name);
	  send_to_char(buf, ch);
	  SET_BIT(element->guildie->status, STATUS_TAG);
	  return;
	}
      }
      element = element->next;
    } //while

    //toggles for check for bit-flip only, non-guild toggle commands
    for (i = 0;i < NUM_TOG_SCMDS;i++)
      if ((*toggle_scmds[i] != '\n') && !strcasecmp(arg, toggle_scmds[i])) 
      {
        do_gen_tog(ch, arg, cmd, i);
        return;
      }

    asend_to_char( ch, "Could not toggle %s.\r\n", arg );
    return;
  } //if( arg && *arg)

  if (GET_WIMP_LEV(ch) == 0)
    strcpy(buf2, "OFF");
  else
    sprintf(buf2, "%-3ld%c", GET_WIMP_LEV(ch), PRF_FLAGGED(ch, PRF_PERCWIMPY) ? '%' : ' ');

  if (!(GET_LEVEL(ch) < LVL_IMMORT))
  {
    sprintf(buf, "&W[&CIMM TOGGLES&W]&n\r\n"
          "     &cHoly Light: &w%-3s&n    "
          "      &cNo Hassle: &w%-3s&n    "
          "     &cRoom Flags: &w%-3s&n\r\n"

          "         &cWiznet: &w%-3s&n    "
          "      &cXLOG Tags: &w%-3s&n\r\n\r\n",

          ONOFF(PRF_FLAGGED(ch, PRF_HOLYLIGHT)),
          ONOFF(PRF_FLAGGED(ch, PRF_NOHASSLE)),
          ONOFF(PRF_FLAGGED(ch, PRF_ROOMFLAGS)),

          ONOFF(!PRF_FLAGGED(ch, PRF_NOWIZ)),
          ONOFF(PRF2_FLAGGED(ch, PRF2_XLOGTAG))
         );
    send_to_char(buf, ch);
  }

  sprintf(buf, "&W[&CCHANNELS&W]&n\r\n"
      "&cAuction Channel: &w%-3s&n    "
      "    &cOOC Channel: &w%-3s&n    "
          "&cObscene Channel: &w%-3s&n\r\n"

          "   &cSing Channel: &w%-3s&n    "
      "       &cQuestion: &w%-3s&n    "
          "  &cPrivate Chans: &w%-3s&n\r\n\r\n",    

      ONOFF(!PRF_FLAGGED(ch, PRF_NOAUCT)),
      ONOFF(!PRF_FLAGGED(ch, PRF_NOOOC)),          
          ONOFF(!PRF2_FLAGGED(ch, PRF2_NOOBSCENE)),

          ONOFF(!PRF2_FLAGGED(ch, PRF2_NOSONG)),
          ONOFF(!PRF2_FLAGGED(ch, PRF2_NOQUEST)),
          ONOFF(!PRF2_FLAGGED(ch, PRF2_NOPADD))
         );
  send_to_char(buf, ch);

  sprintf(buf, "&W[&CDISPLAY&W]&n\r\n"
      "&cHit Pnt Display: &w%-3s&n    "
      "   &cMove Display: &w%-3s&n    "
      "   &cMana Display: &w%-3s&n\r\n"

      "   &cCompact Mode: &w%-3s&n    "
      "   &cRepeat Comm.: &w%-3s&n    "
      "       &cAutoexit: &w%-3s&n\r\n"

      "     &cBrief Mode: &w%-3s&n    "
      "         &cNospam: &w%-3s&n    "
      "    &cColor Level: &w%s&n\r\n"

      "     &cTimestamps: &w%-3s&n\r\n\r\n",

      ONOFF(PRF_FLAGGED(ch, PRF_DISPHP)),
      ONOFF(PRF_FLAGGED(ch, PRF_DISPMOVE)),
      ONOFF(PRF_FLAGGED(ch, PRF_DISPMANA)),

      ONOFF(PRF_FLAGGED(ch, PRF_COMPACT)),
      YESNO(!PRF_FLAGGED(ch, PRF_NOREPEAT)),
      ONOFF(PRF_FLAGGED(ch, PRF_AUTOEXIT)),

      ONOFF(PRF_FLAGGED(ch, PRF_BRIEF)),
      ONOFF(PRF_FLAGGED(ch, PRF_NOSPAM)),
      ctypes[COLOR_LEV(ch)],
      ONOFF(PRF2_FLAGGED(ch, PRF2_TIMESTAMP))
         );
  send_to_char(buf, ch);

  sprintf(buf, "&W[&CCOMBAT&W]&n\r\n"
          "       &cAutoloot: &w%-3s&n    "
          "       &cAutogold: &w%-3s&n    "
          "      &cAutosplit: &w%-3s&n\r\n"

          "       &cAutojunk: &w%-3s&n    "
          "     &cAutoassist: &w%-3s&n    "
          "    &cDeath cries: &w%-3s&n\r\n"

      "  &cPercent wimpy: &w%-3s&n    "
      "     &cWimp Level: &w%-4s&n\r\n\r\n",

          ONOFF(AUT_FLAGGED(ch, AUTO_LOOT)),
          ONOFF(AUT_FLAGGED(ch,AUTO_GOLD)),
          ONOFF(AUT_FLAGGED(ch,AUTO_SPLIT)),

          ONOFF(AUT_FLAGGED(ch,AUTO_JUNK)),
          ONOFF(AUT_FLAGGED(ch,AUTO_ASSIST)),
          ONOFF(!PRF2_FLAGGED(ch, PRF2_NOKILLMESS)),

          ONOFF(PRF_FLAGGED(ch, PRF_PERCWIMPY)),
          buf2
         );
  send_to_char(buf, ch);

  sprintf(buf, "&W[&COTHER&W]&n\r\n"
      "       &cOn Quest: &w%-3s&n    "
      "         &cNoTell: &w%-3s&n    "
      "          &cShout: &w%-3s&n\r\n"

      "             &cIc: &w%-3s&n    "
          "     &cSliceweave: &w%-3s&n    "
          "&cHunger Messages: &w%-3s&n\r\n"

          "     &cLook Ahead: &w%-3s&n    "
          "        &cPose ID: &w%-3s&n    "
      "            &cAFK: &w%-3s&n\r\n"

      "            &cAFW: &w%-3s&n    "
      "      &cNoweather: &w%-3s&n    "
      "        &cMycolor: %s&%-3s&n\r\n"

          "&c      Comm Spam: &w%-3s&n    "
          "&c         RP Tag: &w%-3s&n\r\n",

      YESNO(PRF_FLAGGED(ch, PRF_QUEST)),
      ONOFF(PRF_FLAGGED(ch, PRF_NOTELL)),
      YESNO(!PRF_FLAGGED(ch, PRF_DEAF)),

          YESNO(PRF_FLAGGED(ch, PRF_IC)),
          ONOFF(PRF2_FLAGGED(ch, PRF2_SLICEWEAVE)),
          ONOFF(!PRF2_FLAGGED(ch, PRF2_NOHUNGER)),

          ONOFF(PRF_FLAGGED(ch, PRF_LOOKAHEAD)),
          ONOFF(!PRF2_FLAGGED(ch, PRF2_NOPOSEID)),
          YESNO(PRF_FLAGGED(ch, PRF_AFK)),

          YESNO(PRF2_FLAGGED(ch, PRF2_AFW)),
      ONOFF(PRF2_FLAGGED(ch, PRF2_NOWEATHER)),
      GET_MYCOLOR(ch), GET_MYCOLOR(ch),

      ONOFF(PRF_FLAGGED(ch, PRF_COMMSPAM)),
      ONOFF(PRF2_FLAGGED(ch, PRF2_RPTAG))
      );

  send_to_char(buf, ch);
}


struct sort_struct {
  int sort_pos;
  byte is_social;
} *cmd_sort_info = NULL;

int num_of_cmds;


void sort_commands(void) {
  int a, b, tmp;

  num_of_cmds = 0;

  /*
   * first, count commands (num_of_commands is actually one greater than the
  * number of commands; it inclues the '\n'.
   */
  while (*complete_cmd_info[num_of_cmds].command != '\n')
    num_of_cmds++;

  if (cmd_sort_info) free(cmd_sort_info);
  /* create data array */
  CREATE(cmd_sort_info, struct sort_struct, num_of_cmds);

  /* initialize it */
  for (a = 1; a < num_of_cmds; a++) {
    cmd_sort_info[a].sort_pos = a;
    cmd_sort_info[a].is_social = (complete_cmd_info[a].command_pointer == do_action);
  }

  /* the infernal special case */
  cmd_sort_info[find_command("insult")].is_social = TRUE;

  /* Sort.  'a' starts at 1, not 0, to remove 'RESERVED' */
  for (a = 1; a < num_of_cmds - 1; a++)
    for (b = a + 1; b < num_of_cmds; b++)
      if (strcmp(complete_cmd_info[cmd_sort_info[a].sort_pos].command,
         complete_cmd_info[cmd_sort_info[b].sort_pos].command) > 0) {
    tmp = cmd_sort_info[a].sort_pos;
    cmd_sort_info[a].sort_pos = cmd_sort_info[b].sort_pos;
    cmd_sort_info[b].sort_pos = tmp;
      }
}



ACMD(do_commands)
{
  int no, i, cmd_num;
  int wizhelp = 0, socials = 0;
  struct char_data *vict;

  one_argument(argument, arg);

  if (*arg) {
    if (!(vict = get_char_vis(ch, arg)) || IS_NPC(vict)) {
      send_to_char("Who is that?\r\n", ch);
      return;
    }
    if (GET_LEVEL(ch) < GET_LEVEL(vict)) {
      send_to_char("You can't see the commands of people above your level.\r\n", ch);
      return;
    }
  } else
    vict = ch;

  if (subcmd == SCMD_SOCIALS)
    socials = 1;
  else if (subcmd == SCMD_WIZHELP)
    wizhelp = 1;

  sprintf(buf, "The following %s%s are available to %s:\r\n",
      wizhelp ? "privileged " : "",
      socials ? "socials" : "commands",
      vict == ch ? "you" : GET_NAME(vict));

  /* cmd_num starts at 1, not 0, to remove 'RESERVED' */
  for (no = 1, cmd_num = 1; cmd_num < num_of_cmds; cmd_num++) {
    i = cmd_sort_info[cmd_num].sort_pos;
    if (complete_cmd_info[i].minimum_level >= 0 &&
    GET_LEVEL(vict) >= complete_cmd_info[i].minimum_level &&
    (complete_cmd_info[i].minimum_level >= LVL_IMMORT) == wizhelp &&
    (wizhelp || socials == cmd_sort_info[i].is_social)) {
      if ((complete_cmd_info[i].command_groups) && (GET_LEVEL(vict) <= LVL_GRGOD)) {
        if (IS_SET(complete_cmd_info[i].command_groups, CG_NEWBIE) &&
            !IS_SET((vict)->player_specials->saved.command_groups, CG_NEWBIE))
          continue;

        if (IS_SET(complete_cmd_info[i].command_groups, CG_GL) &&
            !IS_SET((vict)->player_specials->saved.command_groups, CG_GL))
          continue;

        if (IS_SET(complete_cmd_info[i].command_groups, CG_ST) &&
            !IS_SET((vict)->player_specials->saved.command_groups, CG_ST))
          continue;

        if (IS_SET(complete_cmd_info[i].command_groups, CG_GL_BUILDER) &&
            !IS_SET((vict)->player_specials->saved.command_groups, CG_GL_BUILDER))
          continue;

        if (IS_SET(complete_cmd_info[i].command_groups, CG_BUILDER) &&
            !IS_SET((vict)->player_specials->saved.command_groups, CG_BUILDER))
      continue;

        if (IS_SET(complete_cmd_info[i].command_groups, CG_ADMIN) &&
            !IS_SET((vict)->player_specials->saved.command_groups, CG_ADMIN))
          continue;

        if (IS_SET(complete_cmd_info[i].command_groups, CG_DGS) &&
            !IS_SET((vict)->player_specials->saved.command_groups, CG_DGS))
          continue;
      }
      sprintf(buf + strlen(buf), "%-11s", complete_cmd_info[i].command);
      if (!(no % 7))
    strcat(buf, "\r\n");
      no++;
    }
  }

  strcat(buf, "\r\n");
  send_to_char(buf, ch);
}

void list_scanned_chars(struct char_data * list, struct char_data * ch, int distance, int door)
{
  const char *how_far[] = {
    "close by",
    "a ways off",
    "far off",
    "VERY far off",
    "extremely far off"
  };

  struct char_data *i,*ilist[300];
  int count = 0,n = 0, k = 0, found = 0, amount[300];
  char tmp[600];


  for (n = 0; n < 300; n++) {
    ilist[n]  = NULL;
    amount[n] = 0;
  }

  for (i = list; i; i = i->next_in_room) {
    if (CAN_SEE(ch, i)) {
     count++;
    }
  }

  if (!count)
    return;

  for (i = list; i; i = i->next_in_room) {
    if (CAN_SEE(ch,i) && IS_NPC(i)) {
      found = FALSE;
      for (k = 0; ilist[k]; k++) {
        if (GET_MOB_VNUM(i) == GET_MOB_VNUM(ilist[k])) {
          amount[k]++;
          found = TRUE;
        }
      }
      if (!found) {
        k = 0;
        while(ilist[k]) k++;
        ilist[k] = i;
        amount[k]++;
      }
    }
  }
  

  tmp[0] = '\0';

  for (k = 0; amount[k]; k++) {
    if (amount[k] > 1) {
      sprintf(tmp,"%s[%s%d%s] ",CCNRM(ch, C_NRM), CCCYN(ch, C_NRM),amount[k],CCNRM(ch, C_NRM));
      send_to_char(tmp,ch);
    }
    sprintf(tmp,"\\c03%s\\c00 %s.\r\n",GET_NAME(ilist[k]),how_far[distance]);
    send_to_char(tmp,ch);
  }

  for (i = list; i; i = i->next_in_room) {
    if (!CAN_SEE(ch, i) || IS_NPC(i))
      continue; 
    sprintf(tmp, "%s %s.\r\n", GET_NAME(i),how_far[distance]);
    send_to_char(tmp,ch);    
  }

  for (k = 0; k < 300; k++) {
    amount[k] = 0;
    ilist[k]  = NULL;
  }

}

int check_and_scan_range(struct char_data *ch, int distance, int dir)
{
  int room = 0, i;
  struct room_direction_data *ex = NULL;
  char obstruction[50], localbuf[100];

  const char *how_far[] = {
    "right here",
    "close by",
    "a ways off",
    "far off",
    "VERY far off",
    "extremely far off"
  };

  if (!ch || (dir >= NUM_OF_DIRS))
    return FALSE;

  room = ch->in_room;

  /* Step to the room in the direction indicated, if possible */
  for (i = 0; distance > i; i ++) {
    ex = world[room].dir_option[dir];
    if (!ex)  /* No exit in that direction exists here */
      break;
    if (ex->to_room == NOWHERE) /* Buggy exit */
      break;
    if (IS_SET(ex->exit_info, EX_CLOSED)) /* Door closed, can't scan */
      break;
    if (ROOM_AFFECTED(ex->to_room, RAFF_FOG)) /* Next room foggy */
      break;
    /* Add more scan prevention checks here */
    room = ex->to_room; /* Ok, step to next room */
  }
  
  if ((distance > i) && ex) {
    /* Tell the player the reason for failure */
    if (IS_SET(ex->exit_info, EX_CLOSED))
      sprintf(obstruction, "closed door");
    if (ROOM_AFFECTED(ex->to_room, RAFF_FOG)) /* Fog overrides door */
      sprintf(obstruction, "dense fog");
    sprintf(localbuf, "A %s, %s.\r\n", obstruction, how_far[i]);
    send_to_char(localbuf, ch);
    return FALSE;
  }
  if (!ex && (distance > i)) /* No room so far away */
    return FALSE;
  /* Here, room is the room in which we want to list the chars */
  if (world[room].people) /* No use listing empty rooms */
    list_scanned_chars(world[room].people, ch, distance-1, dir);  

  return TRUE;
}

ACMD(do_scan) {
  int door;
  
  *buf = '\0';
  
  if (IS_AFFECTED(ch, AFF_BLIND) || IS_SET(GET_PERMANENT(ch), PRM_BLIND)) {
    send_to_char("You can't see a damned thing, you're blind!\r\n", ch);
    return;
  }
  if (ROOM_AFFECTED(ch->in_room, RAFF_FOG)) {
    send_to_char("You can't scan anything in this fog!\r\n", ch);
    return;
  }
  send_to_char("You quickly scan the area.\r\n", ch);
  for (door = 0; door < NUM_OF_DIRS ; door++) {
    if (EXIT(ch, door)) { 
      sprintf(buf, "\\c03---------------------\\c02%s\\c03------------------------\\c00\r\n", dirs[door]);
      send_to_char(buf, ch);
    }
    else    /* Short circuiting a little */
      continue;
    /* Tada! Somewhat shorter now, hopefully */
    if (check_and_scan_range(ch, 1, door))
      if (check_and_scan_range(ch, 2, door))
        if (check_and_scan_range(ch, 3, door) && 
           (GET_ISWOLFKIN(ch) || (GET_BONDED(ch) && !(GET_BONDED(ch) & (1 << 30)))))
          if (check_and_scan_range(ch, 4, door) && (GET_BONDED(ch) && !(GET_BONDED(ch) & (1 << 30))))
            check_and_scan_range(ch, 5, door);
  }
}
ACMD(do_test)
{
  struct char_data *victim = NULL;

  if ((GET_SEX(ch) == SEX_FEMALE) && GET_LEVEL(ch) < LVL_IMMORT) {
    send_to_char("Huh!?\r\n", ch);
    return;
  }
  if ((GET_SKILL(ch, SKILL_GRASP) < 90)) {
    send_to_char("Huh!?\r\n", ch);
    return;
  }

  one_argument(argument, arg);

  if (!*arg) {
    send_to_char("Who do you want to test for the channeling ability?\r\n", ch);
    return;
  }
  if (!(victim = get_char_room_vis(ch, arg))) {
    send_to_char("That person is not here.\r\n", ch);
    return;
  }
  if ((GET_SEX(ch) != GET_SEX(victim)) && (GET_LEVEL(ch) < LVL_IMMORT)) {
    send_to_char("Sorry, you can only test people of the same gender.\r\n", ch);
    return;
  }

  if (IS_NPC(victim))  {
    if (MOB_FLAGGED(victim,MOB_CHANNELER)) {
      if (GET_LEVEL(victim) > GET_LEVEL(ch)) {
        send_to_char("This mob is stronger than you...\r\n",ch);
      }
      else {
        send_to_char("You seem to be stronger than this mob...\r\n",ch);
      }
    }
    else
      send_to_char("This mob cannot channel...\r\n",ch);
    
    return;
  }


  if (ch == victim) {
    send_to_char("Why would you want to test yourself?\r\n", ch);
    return;
  }

  if ((IS_SET(PRF_FLAGS(victim), PRF_GRASPING)) && !(IS_NPC(victim))) {
    act("$N is grasping the One Power. It's a good bet that $E channels.",
FALSE, ch, NULL, victim, TO_CHAR);
    return;
  }

  if (((GET_R_IDLE(ch) < 10) || (GET_R_IDLE(victim) < 10)) &&
GET_LEVEL(ch) < LVL_IMMORT) {
    send_to_char("You haven't been together in the same room long enough.\r\n",ch);
    return;
  }

  act("A small flame dances over $n's palm as $e peers intently at $N.",
FALSE, ch, NULL, victim, TO_NOTVICT);
  act("A small flame dances over $n's palm as $e peers intently at you.",
FALSE, ch, NULL, victim, TO_VICT);
  act("You channel a small flame over your hand and study $N intently.",
FALSE, ch, NULL, victim, TO_CHAR);
  if (!cant_channel(victim) && !IS_AFFECTED2(victim,AFF2_MASKED))
    sprintf(buf,"%s can channel.\r\n", GET_NAME(victim)); 
  else
    sprintf(buf,"%s is unable to channel.\r\n", GET_NAME(victim));
  send_to_char(buf, ch);
}

#define ISSKL(num,ch) (spell_info[num].min_level[(int)GET_CLASS(ch)][1]) 

void print_skills(struct char_data *ch,struct char_data *tchar)
{
  int i,sortpos,j=1;
  int skills[MAX_SKILLS + 1];
  char lvl[10];

  *buf = '\0';

  for (i = 0; i < MAX_SKILLS; i++)
    skills[i] = -1;

  for(sortpos = 1; sortpos < MAX_SKILLS; sortpos++) {
    i = spell_sort_info[sortpos];
    if ((i >= MAX_SKILLS) || (i < 0) || (spells[i] == NULL) || !*spells[i])
      continue;
    if (strcmp(spells[i],"!UNUSED!") && (ISSKL(i,tchar) == -1)) {
      skills[j] = i;
      j++;
    }
  }

  sprintf(buf,"%s\r\n&W[&CEXPERIENCE&W]&n\r\n",buf);

  for (sortpos = 0; sortpos < MAX_SKILLS; sortpos++) {
     i = skills[sortpos];

    if ((i != -1) && (spell_info[i].type == SKILL_NORMAL)) {
      if (spell_info[i].min_level[(int) GET_CLASS(tchar)][0] < LVL_IMMORT) {
        sprintf(lvl, "%3ld", spell_info[i].min_level[(int) GET_CLASS(tchar)][0]);
        sprintf( buf, "%s&cLevel &w%s&c: %-25s &w%3d&c%%&n\r\n",
                 buf,
                 lvl,
                 spells[ i ],
                 GET_SKILL( tchar, i ) );
      }
    }
  }
  sprintf(buf, "%s\r\n&W[&CROLEPLAY&W]&n\r\n",buf);

  for (sortpos = 0; sortpos < MAX_SKILLS; sortpos++) {
     i = skills[sortpos];

    if ((i != -1) && (spell_info[i].type == SKILL_NORMAL)) {
      if (spell_info[i].min_level[(int) GET_CLASS(tchar)][0] >= LVL_IMMORT) {
	if (spell_info[i].cost[(int)GET_CLASS(tchar)] > 0) {
	  sprintf(lvl," RP");
	  sprintf( buf, "%s&cLevel &w%s&c: %-25s &w%3d&c%%&n\r\n",
		   buf,
		   lvl,
		   spells[ i ],
		   GET_SKILL( tchar, i ) );
	}
      }
    }
  }

  if ( GET_LEVEL(ch) >= LVL_IMMORT || (is_dark(tchar) || GET_CHAR_GUILDS(tchar)) ) {
    sprintf(buf,"%s\r\n&W[&CGUILD&W]&n\r\n",buf);

    for(sortpos =0;sortpos < MAX_SKILLS; sortpos++) {
      i = skills[sortpos];

      if ((i != -1) && (spell_info[i].type == SKILL_GUILD) && GET_SKILL(tchar,i)) {
        sprintf(buf, "%s&cLevel &wn/a&c: %-25s &w%3d&c%%&n\r\n",
                buf,spells[i],GET_SKILL(tchar,i));
      }
    }
  }
  sprintf(buf,"%s\r\n&W[&CSPECIAL/QUEST&W]&n\r\n",buf);

  for (sortpos = 0; sortpos < MAX_SKILLS; sortpos++) {
    i = skills[sortpos];

    if ((i != -1) && GET_SKILL(tchar,i) && ((spell_info[i].type == SKILL_SPECIAL) || (spell_info[i].type == SKILL_QUEST))) {
      sprintf(buf,"%s&cLevel &wn/a&c: %-25s &w%3d&c%%&n\r\n",
              buf,spells[i],GET_SKILL(tchar,i));
    }
  }
  page_string(ch->desc,buf,1);

}

ACMD(do_spells)
{
  int i,sortpos, j=1, k = GET_CLASS(ch);
  int skills[MAX_SKILLS + 1];
  sort_spells();
  if (cant_channel(ch)) {
    send_to_char("You? You can't even channel!\r\n", ch);
    return;
  }
  *buf = '\0';

  for (i = 0; i < MAX_SKILLS; i++)
    skills[i] = -1;

  for(sortpos = 1; sortpos < MAX_SKILLS; sortpos++) {
    i = spell_sort_info[sortpos];
    if ((i > MAX_SKILLS) || (i < 0))
      continue;
    if (strcmp(spells[i],"!UNUSED!") && (ISSKL(i,ch) != -1)) {
      skills[j] = i;
      j++;
    }
  }
   
  sprintf(buf,"%s\r\n&W[&CEXPERIENCE&W]&n\r\n",buf);
  sprintf(buf,"%s&c                               Air  Earth Fire Spirit Water Skill&n\r\n",buf);               
  for (sortpos = 1; sortpos < MAX_SKILLS; sortpos++) {
    i = skills[sortpos];
    if (((i != -1) && (spell_info[i].type == SPELL_NORMAL)) && spell_info[i].cost[(int) GET_CLASS(ch)] <= 50) {
      sprintf(buf,"%s&c%-30s ", buf, spells[i]);
      j = spell_info[i].min_level[k][0];
      if (j>GET_AIR(ch)) sprintf(buf,"%s&R",buf);
      else sprintf(buf,"%s&g",buf);
      sprintf(buf,"%s%-4d ",buf,j);
      j = spell_info[i].min_level[k][1];
      if (j>GET_EARTH(ch)) sprintf(buf,"%s&R",buf);
      else sprintf(buf,"%s&g",buf);
      sprintf(buf,"%s%-5d ",buf,j);
      j = spell_info[i].min_level[k][2];
      if (j>GET_FIRE(ch)) sprintf(buf,"%s&R",buf);
      else sprintf(buf,"%s&g",buf);
      sprintf(buf,"%s%-4d ",buf,j);
      j = spell_info[i].min_level[k][3];
      if (j>GET_SPIRIT(ch)) sprintf(buf,"%s&R",buf);
      else sprintf(buf,"%s&g",buf);
      sprintf(buf,"%s%-6d ",buf,j);
      j = spell_info[i].min_level[k][4];
      if (j>GET_WATER(ch)) sprintf(buf,"%s&R",buf);
      else sprintf(buf,"%s&g",buf);
      sprintf(buf,"%s%-5d ",buf,j);
      j = 0;
      if (spell_info[i].min_level[k][0]>GET_AIR(ch)) j++;
      if (spell_info[i].min_level[k][1]>GET_EARTH(ch)) j++;
      if (spell_info[i].min_level[k][2]>GET_FIRE(ch)) j++;
      if (spell_info[i].min_level[k][3]>GET_SPIRIT(ch)) j++;
      if (spell_info[i].min_level[k][4]>GET_WATER(ch)) j++;
      if ( j == 0 || GET_SKILL(ch,i) > 0) sprintf(buf,"%s&w%3d&c%%&n\r\n", buf, GET_SKILL(ch,i));
      else sprintf(buf,"%s&Rn/a&c%%&n\r\n",buf);
    }
  }
  sprintf(buf,"%s\r\n&W[&CROLEPLAY&W]&n\r\n",buf);
  sprintf(buf,"%s&c                               Air  Earth Fire Spirit Water Skill&n\r\n",buf);               
  for (sortpos = 1; sortpos < MAX_SKILLS; sortpos++) {
    i = skills[sortpos];
    if (((i != -1) && (spell_info[i].type == SPELL_NORMAL)) && spell_info[i].cost[(int) GET_CLASS(ch)] > 50) {
      sprintf(buf,"%s&c%-30s ", buf, spells[i]);
      j = spell_info[i].min_level[k][0];
      if (j>GET_AIR(ch)) sprintf(buf,"%s&R",buf);
      else sprintf(buf,"%s&g",buf);
      sprintf(buf,"%s%-4d ",buf,j);
      j = spell_info[i].min_level[k][1];
      if (j>GET_EARTH(ch)) sprintf(buf,"%s&R",buf);
      else sprintf(buf,"%s&g",buf);
      sprintf(buf,"%s%-5d ",buf,j);
      j = spell_info[i].min_level[k][2];
      if (j>GET_FIRE(ch)) sprintf(buf,"%s&R",buf);
      else sprintf(buf,"%s&g",buf);
      sprintf(buf,"%s%-4d ",buf,j);
      j = spell_info[i].min_level[k][3];
      if (j>GET_SPIRIT(ch)) sprintf(buf,"%s&R",buf);
      else sprintf(buf,"%s&g",buf);
      sprintf(buf,"%s%-6d ",buf,j);
      j = spell_info[i].min_level[k][4];
      if (j>GET_WATER(ch)) sprintf(buf,"%s&R",buf);
      else sprintf(buf,"%s&g",buf);
      sprintf(buf,"%s%-5d ",buf,j);
      j = 0;
      if (spell_info[i].min_level[k][0]>GET_AIR(ch)) j++;
      if (spell_info[i].min_level[k][1]>GET_EARTH(ch)) j++;
      if (spell_info[i].min_level[k][2]>GET_FIRE(ch)) j++;
      if (spell_info[i].min_level[k][3]>GET_SPIRIT(ch)) j++;
      if (spell_info[i].min_level[k][4]>GET_WATER(ch)) j++;
      if ( j == 0 ) sprintf(buf,"%s&w%3d&c%%&n\r\n", buf, GET_SKILL(ch,i));
      else sprintf(buf,"%s&Rn/a&c%%&n\r\n",buf);
    }
  }
  if (is_dark(ch) || GET_CHAR_GUILDS(ch)) {
    sprintf(buf,"%s\r\n&W[&CGUILD&W]&n\r\n",buf);
    sprintf(buf,"%s&c                               Air  Earth Fire Spirit Water Skill&n\r\n",buf);               
    for (sortpos = 0; sortpos < MAX_SKILLS; sortpos++) {
      i = skills[sortpos];
      if ((i != -1) && GET_SKILL(ch,i) && (spell_info[i].type == SPELL_GUILD)) 
        sprintf(buf,"%s&c%-30s &gn/a   n/a  n/a    n/a   n/a  &w%3d&c%%&n\r\n",buf,spells[i],GET_SKILL(ch,i));
    }
  }
/**
 * Code for displaying S/Q-weaves, not necessary here as none are implemented yet.
 ***
 * 
 *  sprintf(buf,"%s\r\n&W[&CSPECIAL/QUEST&W]&n\r\n",buf);
 *  sprintf(buf,"%s&c                               Air  Earth Fire Spirit Water Skill&n\r\n",buf);               
 *   
 *  for (sortpos = 0; sortpos < MAX_SKILLS; sortpos++) {
 *    i = skills[sortpos];
 *    if ((i != -1) && GET_SKILL(ch,i) && ((spell_info[i].type == SPELL_QUEST)||(spell_info[i].type == SPELL_SPECIAL))) 
 *      sprintf(buf,"%s&c%-30s  &gn/a   n/a  n/a    n/a   n/a &w%3d&c%%&n\r\n", buf,spells[i],GET_SKILL(ch,i));
 *  }
 */
  page_string(ch->desc,buf,1);
}

ACMD(do_affected)
{
  char tmp[200], tmp2[200],tie = ' ';
  struct affected_type *aff;
  int found = 0;

  send_to_char("You are affected by the following:\r\n",ch);

  for (aff = ch->affected; aff != NULL; aff = aff->next) {
    found++;
    *tmp = '\0';
    if (aff->type <= MAX_SPELLS) {
      if (SCMD_TIED == aff->tie_info)
    tie = 'T';
      else
    tie = 'M';
      sprintf(tmp,"WEAVE/SKILL (%4d hrs [%c]) %-21s ",aff->duration,tie,spells[aff->type]);
    } else
      sprintf(tmp,"WEAVE/SKILL (%4d hrs    ) %-21s ",aff->duration,spells[aff->type]);

    if (IS_SET(aff->bitvector,AFF_NOTEACH)) {
      found--;
      continue;
    }

    if (aff->modifier) {
      sprintf(tmp2,"%+3ld to %-10s",aff->modifier,apply_types[(int)aff->location]);
      strcat(tmp,tmp2);
    }
        
    if (aff->bitvector){
      sprintf(tmp2,", sets ");
      strcat(tmp,tmp2);
      sprintbit(aff->bitvector,affected_bits,tmp2);
      strcat(tmp,tmp2);
      strcat(tmp,"\r\n");
    }
    else if (aff->bitvector2){
      sprintf(tmp2,", sets ");
      strcat(tmp,tmp2);
      sprintbit(aff->bitvector2,affected2_bits,tmp2);
      strcat(tmp,tmp2);
      strcat(tmp,"\r\n");       
    }
    else if (aff->bitvector3){
      sprintf(tmp2,", sets ");
      strcat(tmp,tmp2);
      sprintbit(aff->bitvector3,affected3_bits,tmp2);
      strcat(tmp,tmp2);
      strcat(tmp,"\r\n");       
    } else if ((SPELL_SHIELD_OF_SAIDIN == aff->type)||(SPELL_STORMSHIELD == aff->type)||(SPELL_WATERSHIELD == aff->type))
      sprintf(tmp,"%s, %ld mana left in shield.\r\n",tmp,aff->spell);
    else
      strcat(tmp,"\r\n");

    if (found) 
      send_to_char(tmp,ch);
  }
  if (!GET_COND(ch, FULL))
    send_to_char("&CYou are hungry.&n\r\n", ch);
  if (!GET_COND(ch, THIRST))
    send_to_char("&CYou are thirsty.&n\r\n", ch);
  if (!found) 
    send_to_char("Nothing, nil,nada, shortly totally unaffected!\r\n",ch);
}

ACMD(do_doing)
{
  skip_spaces(&argument);
  free(GET_DOING(ch));
  if (!*argument) 
  {
    send_to_char("Standard doing message now shown to onlookers.\r\n", ch);
    GET_DOING(ch) = NULL;
  } else 
  {
    sprintf(buf, "Ok, you are now %s\r\n", argument);
    send_to_char(buf, ch);
    sprintf(buf, " is %s", argument);
    GET_DOING(ch) = strdup(buf);      
  }
}

ACMD(do_scene)
{
  char	*set_msg = "$n sets the stage for RP.";
  char	*clear_msg = "$n clears the stage of the scene.";
  char	*use_msg;
  struct char_data *people;
  skip_spaces(&argument);

  if( !IS_SET(PRF_FLAGS(ch),PRF_IC) )
  {
    send_to_char( "You are not IC enough.\r\n", ch );
    return;
  }

  if( ROOM_NO_SCENE( ch->in_room ) )
  { 
    SCENE_DATA	*scene = ROOM_NO_SCENE( ch->in_room );
    if( scene->owner && scene->owner != ch )
    {
      if( IS_SET(PRF_FLAGS( scene->owner ), PRF_IC )
	&& scene->owner->in_room == ch->in_room
	&& GET_LEVEL( ch ) < LVL_GOD )
      {
	send_to_char( "You can not override somebody else's scene.\r\n", ch );
	return;
      }
    }
  }
    
  if( CHAR_SCENE( ch ) )
    CLEAR_SCENE( CHAR_SCENE( ch ) );
  if( ROOM_NO_SCENE( ch->in_room ) )
    CLEAR_SCENE( ROOM_NO_SCENE( ch->in_room ) );

  if (!*argument) 
  {
    use_msg = clear_msg;
    send_to_char("Standard scene now shown to onlookers.\r\n", ch);
  }
  else
  {
    SCENE_DATA	*scene;
    CREATE( scene, SCENE_DATA, 1 );
    sprintf(buf, "Ok, scene set to '%s'.\r\n", argument);
    send_to_char(buf, ch);
    scene->text = strdup( argument );
    scene->owner = ch;
    scene->room = &(world[ch->in_room]);
    CHAR_SCENE( ch ) = scene;
    ROOM_NO_SCENE( ch->in_room ) = scene;
    use_msg = set_msg;
    axlog( SYS_COMM, LVL_GOD, TRUE, "%s set a scene to '%s'", GET_NAME( ch ), argument );
  }
  for (people = world[ch->in_room].people; people; people = people->next_in_room)
  {
    if (people == NULL)
      continue;
    if (people == ch)
      continue;
    if (!SEE_DREAMING_OK(ch, people))
      continue;

    if(!check_and_notify_non_IC_receiver(ch, people, "sets"))
    {
      act( use_msg, TRUE, ch, 0, people, TO_VICT);
      sprintf( buf, "&m%s&n\r\n", argument );
      send_to_char( buf, people );
    }
  }
}

ACMD(do_notes)
{
  send_to_char("Removed until further notice!\r\n",ch);
/*
  char arg1[100], arg2[100];
  struct entry *e;
  int num, max;

  skip_spaces(&argument);
  if (IS_NPC(ch)) {
    send_to_char("Sorry, mobs aren't allowed to read the notes.\r\n", ch);
    return;
  }
  send_to_char("Sorry, this command is out of order at the moment.\r\n", ch);
  return;
  entries = NULL;
  if (!argument || !*argument) { // Show player the list of notes.
    max = readNotesFromMud();
    if (max < 0) {
      send_to_char("Sorry, there was some kind of problem reading the notes. Try later. \r\n", ch);
      free(entries);
      return;
    }
    send_to_char(listEntries(), ch);
    send_to_char("\r\n", ch);
    free(entries);
    return;
  }
  two_arguments(argument, arg1, arg2);
  if (!strncasecmp(arg1, "read", strlen(arg1))) { // Wants to read one
    max = readNotesFromMud();
    if (max < 0) {
      send_to_char("Sorry, there was some kind of problem reading the notes. Try later. \r\n", ch);
      free(entries);
      return;
    }
    num = atoi(arg2);
    if (!(e = get(num))) {
      send_to_char("There is no note with that number.\r\n", ch);
      free(entries);
      return;
    }
    send_to_char(noteToString(num), ch);
    free(entries);
    return;
  }
*/
}

ACMD(do_passwd)
{
  skip_spaces(&argument);
  if (IS_NPC(ch)) {
    send_to_char("Mobs don't need this. Get lost.\r\n", ch);
    return;
  }
  if (PLR_FLAGGED(ch, PLR_NOGAIN)) {
    send_to_char("Sorry, you have the !gain flag set on your character.\r\n", ch);
    send_to_char("To be able to set your password, that flag must be cleared first.\r\n", ch);
    return;
  }
  if (!*argument) {
    send_to_char("Set password as what?\r\n", ch);
    return;
  }
  if (strlen(argument) > MAX_PWD_LENGTH) {
    send_to_char("Password is too long, try again.", ch);
    return;
  }
  strncpy(GET_PASSWD(ch), CRYPT(argument, GET_RNAME(ch)), MAX_PWD_LENGTH);
  GET_PASSWD(ch)[MAX_PWD_LENGTH] = '\0';
  sprintf(buf, "Password changed to '%s'.\r\n", argument); 
  send_to_char(buf, ch);
}

ACMD(do_gossip)
{
  char arg[MAX_INPUT_LENGTH+1];

  one_argument(argument, arg);

  if (!*arg || (GET_LEVEL(ch) < LVL_IMMORT))
    page_string(ch->desc, gossip, 1);
  else if (is_abbrev(arg, "write")) {
    send_to_char("Enter your new gossip..normal writing rules apply:\r\n", ch);
    send_to_char("\x1B[H\x1B[J", ch);
    CREATE( ch->desc->str, char*, 1 );
    *(ch->desc->str) = NULL;
    ch->desc->backstr = NULL;
    ch->desc->max_str = (MAX_STRING_LENGTH-strlen(gossip)-3);
    ch->desc->current_text_line = count_newlines(ch->desc->str) + 1;
    STATE(ch->desc) = CON_GOSSIP;
    SET_BIT(PLR_FLAGS(ch), PLR_WRITING);
  }
  else if (is_abbrev(arg, "write-all")) {
    send_to_char("Editing the entire gossip file:\r\n", ch);
    send_to_char("\x1B[H\x1B[J", ch);
    ch->desc->str = &gossip;
    ch->desc->max_str = MAX_STRING_LENGTH-3;
    ch->desc->backstr = NULL;
    ch->desc->current_text_line = count_newlines(ch->desc->str) + 1;
    SET_BIT(PLR_FLAGS(ch), PLR_WRITING);
  }
  else if (is_abbrev(arg, "clear") && GET_LEVEL(ch) >= LVL_GROOVYB) {
    unlink(GOSSIP_FILE);
    touch(GOSSIP_FILE);
    file_to_string_alloc(GOSSIP_FILE, &gossip);
    send_to_char("Gossip list cleared and saved to file!\r\n", ch);
  }
  else
    send_to_char("Hmmm...I don't recognise that....\r\n"
         "Try using just 'write'\r\n", ch);
}

ACMD(do_pray)
{
    char buf[MAX_STRING_LENGTH];
    struct descriptor_data *a;

    sprintf(buf, "&n[&W %s has requested help in %s (vnum %d).&n ]\r\n", GET_NAME(ch), world[IN_ROOM(ch)].name, world[IN_ROOM(ch)].number);

    if (PLR_FLAGGED(ch, PLR_NOPRAY)) {
	    send_to_char("Your prayers are not heard. You shouldn't have abused it!\r\n", ch);
	    return;
    }

    if (GET_LEVEL(ch) < 20) {
     for (a = descriptor_list; a; a = a->next)
      if (!a->connected && a->character && (a->character != ch) &&
          !PLR_FLAGGED(a->character, PLR_MAILING | PLR_WRITING | PLR_NOGECHO))
            if (GET_LEVEL(a->character) >= LVL_IMMORT || 
                IS_SET((a->character)->player_specials->saved.command_groups, CG_NEWBIE)) 
                  send_to_char(buf, a->character);                                 
     send_to_char("You pray to the gods for guidance.\r\n", ch);
     }
     else send_to_char("You don't need to do this! Honest!\r\n", ch);
}

ACMD(do_info)
{
  page_string(ch->desc,info,0);
}

ACMD(do_rlist)
{
  char buf[15000];
  int i;

  *buf = '\0';

  for (i = 0; i <= top_of_world; i++) 
  {
    if ( test_bit( GET_KNOW(ch),world[i].number)) 
    {
      if (strlen(buf) >= 14200) 
      {
	sprintf(buf,"%s****Buffer overflow! Remainong rooms lost!****\r\n",buf);
	break;
      }
      sprintf(buf,"%s&c%s&n\r\n", buf, world[i].name);
    }
  }
  page_string(ch->desc, buf, 1);
  //  send_to_char(buf,ch);
}

ACMD(do_apropos)
{
  int i, j = 0;
  if (!ch->desc)
    return;

  if (!help_table)
  {
    send_to_char("The help_table doesn't exist. Tough luck.\r\n", ch);
    return;
  }
  skip_spaces(&argument);
  if (!*argument)
  {
    send_to_char("Search for which word or phrase in the help text?\r\n", ch);
    return;
  }
  sprintf(buf, "Help entries which contain '%s':\r\n", argument);
  for(i = 0; i < top_of_helpt; i++) 
  {
    if (strstr(help_table[i].entry, argument))
    {
      j ++;
      sprintf(buf, "%s&y%-20s&n\r\n", buf, help_table[i].keywords);
      if (j >= 100)
      {
        strcat(buf, "\r\n&r<---- Too many hits, only first 100 shown ---->&n\r\n");
        break;
      }
    }
  }
  strcat(buf, "\r\n");

  if (!j)
    send_to_char("...none!\r\n", ch);
  else if (ch->desc)
    page_string(ch->desc, buf, 1);
}

ACMD(do_helpcheck)
{
   int i, w = 0;
   struct help_index_element *this_help;
   char str[64];

   if(!help_table)
      {
      send_to_char("The help_table doesn't exits !?!?!\r\n", ch);
      return;
      }

   sprintf(buf, "\r\n");
   strcpy(buf, "Commands without help entries:\r\n");
   strcat(buf, "-------------------------------------------------------------------\r\n");

   for(i = 0; *(complete_cmd_info[i].command) != '\n'; i++) {
      sprintf(str, "%s", complete_cmd_info[i].command);
      if(!(this_help = find_help(str))) {
         if(complete_cmd_info[i].command_pointer == do_action)
            continue;
         w++;
         w = w%3;
         sprintf(buf + strlen(buf), " %-20.20s%s", complete_cmd_info[i].command,
                 (w ? "|":"\r\n"));
      }
   }

   if(w)
      strcat(buf, "\r\n");

   if(ch->desc)
      page_string(ch->desc, buf, 1);

}

ACMD(do_hindex)
{
   int i, row = 0, minlen;

   if(!ch->desc)
      return;

   skip_spaces(&argument);

   sprintf(buf, "\r\n");
   if(!*argument) {
      send_to_char("USAGE: index <letter|phrase>\r\n", ch);
      return;
      }

   minlen = strlen(argument);

   for(i = 0; i < top_of_helpt; i++) {
      if(!strn_cmp(argument, help_table[i].keywords, minlen)) {
         row++;
         sprintf(buf + strlen(buf), "|%-23.23s |", help_table[i].keywords);
         if((row%3) == 0)
            strcat(buf, "\r\n");
      }
   }

   if(ch->desc)
      page_string(ch->desc,buf,1);
}

