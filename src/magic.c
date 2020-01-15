/******************************************************************************
 * $Id: magic.c,v 1.5 2003/12/16 01:43:20 fang Exp $
 *   
 * File: magic.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: magic.c,v 
 *   Revision 1.2  2002/07/28 13:56:04  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.6  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.5  2002/05/15 14:52:05  fang
 *   Maz: Added Nose/Ear Stat-Removal code for existing CRAFT EQ per Masema.  Also fixed two typos.
 *
 *   Revision 1.4  2002/02/28 17:02:35  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/02/26 15:35:06  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.13  2002/01/31 13:25:35  mud
 *   *** empty log message ***
 *
 *   Revision 1.12  2002/01/24 21:33:17  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.11.2.2  2002/01/24 03:20:31  mud
 *   Removed some further unescessary files (player.[ch], roomprog.[ch]
 *   old2new.c). Cleaned up some other stuff (guild stuff mostly).
 *
 *   Revision 1.11.2.1  2002/01/23 21:49:42  mud
 *   Removed objsave.[c|h] (old objsave stuff).
 *   Removed obj_file_elem and rent structs.
 *   Removed final vestiges of old guild system, except parts used
 *   in guild.[c|h] for backwards compatibility.
 *
 *   Revision 1.11  2001/11/18 13:17:02  mud
 *   Spelling mistakes and etcetera.
 *
 *   Revision 1.10  2001/11/06 11:31:39  mud
 *   Arrrestees now stops followinf arrester when unarrested/effect wears off.
 *
 *   Revision 1.9  2001/10/16 13:14:52  mud
 *   *** empty log message ***
 *
 *   Revision 1.8  2001/09/30 13:35:17  mud
 *   *** empty log message ***
 *
 *   Revision 1.7  2001/09/28 15:00:36  mud
 *   Taint effects.
 *
 *   Revision 1.6  2001/07/12 15:27:41  mud
 *   *** empty log message ***
 *
 *   Revision 1.5  2000/12/11 12:14:18  mud
 *   Removed most references to PFILEPOS() get_char_ppos(), since with the xml code ppos will(may) change every reboot
 *
 *   Revision 1.4  2000/05/24 08:41:13  mud
 *   Aiel endurance changes
 *
 *   Revision 1.3  2000/03/16 21:30:57  mud
 *   Fixed typo in unravel message, and bm titles.
 *
 *   Revision 1.2  2000/02/21 14:00:17  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* ************************************************************************
*   File: magic.c                                       Part of CircleMUD *
*  Usage: low-level functions for magic; spell template code              *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */
#include "skimming.h"
#include "act.h"
#include "config.h"
#include "conf.h"
#include "sysdep.h"
#include "class.h"
#include "constants.h"
#include "magic.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "spells.h"
#include "handler.h"
#include "db.h"
#include "interpreter.h"
#include "guild.h"

void spring_trap(struct char_data *ch, struct affected_type *af)
{
  struct char_data *mob;
  int spell;

  if (!ch || !af || !af->type)
    return;

  spell = af->type;

  if (!spell_list[spell])
    return;
  if (!(mob = read_mobile(3, VIRTUAL)))
    return;
  char_to_room(mob, ch->in_room);
  mob->points.mana = 14000;
  spell_list[spell](mob, ch, NULL, NULL, NULL, 0, 0, 0,1,1.0);
  extract_char(mob, TRUE);  
}

static void long_mess(struct char_data *ch, int chance, unsigned long longing)
{
  int rv = number(1,100);
  unsigned long i = longing;

  if (rv > chance)
    return;
  if (!ROOM_FLAGGED(ch->in_room,ROOM_STEDDING)) {
    if (i < 40)
      asend_to_char(ch,"You feel a slight Longing to see a stedding again.\r\n");
   else if ((i >= 40) && (i < 100))
      asend_to_char(ch,"The Longing grows inside you, maybe you should visit a Stedding soon?\r\n");
   else if ((i >= 100) && (i < LONGING_NOGAIN))
    asend_to_char(ch,"Your Longing to once again see a Stedding is almost unbearable.\r\n");
    else {
      asend_to_char(ch,"You feel close to dying by the Longing to once again tread the ground of a Stedding.\r\n");
      act("$n moans slightly, what's wrong with $e?",FALSE,ch,0,0,TO_ROOM);
    }
  } else { 
    if (i < 40)
      asend_to_char(ch,"Ah! It certainly feels good to be back in the Stedding.\r\n");
    else if ((i >= 40) && (i < 100))
      asend_to_char(ch,"As you relax yourself in the Stedding, you can't help wondering how you've managed so long without visiting one.\r\n");
    else if ((i >= 100) && (i < LONGING_NOGAIN))
      asend_to_char(ch,"Feels good to be back in a Stedding. You don't think you  could've standed a second more without seeing one.\r\n");
    else {
      asend_to_char(ch,"Close call, pal...A few more days without seeing a Steding....\r\n");
      act("$n sighs with relief, and relaxes significantly....",FALSE,ch,0,0,TO_ROOM);
    }
  }  
}

/* Do longing msgs. */
void do_longing(void)
{
  static struct char_data *list;
  int i;
    
  for (list = character_list; list; list = list->next) {
    if (IS_NPC(list) || (!GET_OGIER(list)))
      continue;
    if (IS_IMMORTAL(list) || !GET_LONGING(list))
      continue;
    i = GET_LONGING(list);

    if (i < 40)
      long_mess(list,3,i);
    else if ((i >= 40) && (i < 100))
     long_mess(list,10,i);
    else if ((i >= 100) && (i < LONGING_NOGAIN))
      long_mess(list,50,i);
    else
      long_mess(list,75,i);
  }
}

/* Adds longing to Ogiers outside stedding. */
void add_longing(void)
{
  static struct char_data *list;
      
  for (list = character_list; list; list = list->next) {
    if (IS_NPC(list))
      continue;
    if (!GET_OGIER(list))
      continue;
    if (ROOM_FLAGGED(list->in_room,ROOM_STEDDING))
      continue;
    if (GET_LONGING(list) == 0xFFFFFFFF)
      continue;
 
    GET_LONGING(list)++;
  }   
}
    
/* Subs longing for Ogier within a stedding */
void sub_longing(void)
{
  static struct char_data *list;
    
  for (list = character_list; list; list = list->next) {
    if (IS_NPC(list))
      continue;
    if (!GET_OGIER(list))
      continue;
    if (!ROOM_FLAGGED(list->in_room,ROOM_STEDDING))
      continue;
    if (!GET_LONGING(list))
      continue;
 
    GET_LONGING(list) -= 3;
    if (GET_LONGING(list) < 0) GET_LONGING(list) = 0;
  }
}
 
/* Updates room_affects! */
void raffect_update(int speed)
{
  static int i;
  static struct affected_type *raff,*next;
  struct char_data *ch;
  int mana_cost;
  char tmp[500];

  for (i = 0; i < top_of_world; i++) {
    if (world[i].aff) {
      for (raff = world[i].aff; raff != NULL; raff = next) {
        next = raff->next;

    if (raff->duration == -1)
      continue;

        if (raff->speed == speed) {
          if (raff->duration > 1) {
            if (!(raff->tie_info) && (raff->type >= 1) && (raff->type <= MAX_SPELLS)) {
              if (!(ch = get_char_id(raff->weaver)))
                raff->duration--;
              else {
                mana_cost = ((spell_info[raff->type].mana + raff->mana_add)/raff->duration);
                GET_MANA(ch) -= mana_cost;
                sprintf(tmp,"Your maintaining of the %s-weave in %s cost you %d mana\r\n",spells[raff->type],world[i].name,mana_cost);
                send_to_char(tmp,ch);
              }
            }
            else
              raff->duration--;
          }
          else {
            if (world[i].people) {
               if (raff->bitvector == RAFF_TRAP) 
                 spring_trap(world[i].people, raff);
               else if (raff->type == SPELL_DARKNESS) {
                 act("The unnatural darkness suddenly vanishes.",FALSE,world[i].people,0,0,TO_ROOM);
         act("The unnatural darkness suddenly vanishes.",FALSE,world[i].people,0,0,TO_CHAR);
               }
               else if (raff->type == SPELL_LIGHT) {
                 act("The unnatural light suddenly vanishes.",FALSE,world[i].people,0,0,TO_ROOM);
         act("The unnatural light suddenly vanishes.",FALSE,world[i].people,0,0,TO_CHAR);
               }
               else if (raff->type == SPELL_DOMEOFAIR) {
                 act("The dome protecting this room suddenly vanishes.",FALSE,world[i].people,0,0,TO_ROOM);
         act("The dome protecting this room suddenly vanishes.",FALSE,world[i].people,0,0,TO_CHAR);
               }
               else if (raff->type == SPELL_DOMEOFFIRE) {
                 act("The fiery dome surrounding this room suddenly vanishes.",FALSE,world[i].people,0,0,TO_ROOM);
         act("The fiery dome surrounding this room suddenly vanishes.",FALSE,world[i].people,0,0,TO_CHAR);                
               }
               else if (raff->type == SPELL_DOMEOFFROST) {
                 act("The frosty dome surrounding this room suddenly vanishes.",FALSE,world[i].people,0,0,TO_ROOM);
         act("The frosty dome surrounding this room suddenly vanishes.",FALSE,world[i].people,0,0,TO_CHAR);                
               }
               else if (raff->type == SPELL_FOG) 
                 send_to_room("The fog covering the area vanishes.\r\n", i);
               else if (raff->type == SPELL_FROSTBITE)
                 send_to_room("The deadly cold here suddenly lets go.\r\n", i);
               else if (raff->type == SPELL_SANCTUARY)
                 send_to_room("The peaceful feeling slowly dwindles away...\r\n", i);
            }
            affect_remove_room(&world[i],raff);
          }
        }
      }
    }
  }
}

static void obj_not_grasping(struct char_data *ch)
{
  static struct obj_data *obj,*hlp;
  int vnum;

  for (obj = object_list; NULL != obj; obj = hlp) {
    hlp = obj->next;
    vnum = GET_OBJ_VNUM(obj);

    if ((40 == vnum || 39 == vnum || 43 == vnum) && (GET_IDNUM(ch) == GET_OBJ_VAL(obj,1))) {
      if (IS_SET(GET_OBJ_EXTRA(obj),ITEM_TAR)) {
        act("A gateway disappears in a flash of light!",TRUE,NULL,obj,NULL,TO_DREAMERS);
      } else {
        act("A gateway disappears in a flash of light!",TRUE,NULL,obj,NULL,TO_ROOM);
      }
      extract_obj(obj);
      if (43 == vnum) {
        fix_skim_gates(ch,-10);
      }
    }
  }
}

void remove_char_maintained(struct char_data *ch)
{
  static struct affected_type *af,*next;
  static struct char_data *i;
  char tmp[500];

  obj_not_grasping(ch);

  for (i = character_list; i; i = i->next) {
    if (!i->affected)
      continue;
    for (af = i->affected; af; af = next) {
      next = af->next;
      if ((af->weaver == GET_IDNUM(ch)) && (!(af->tie_info)) && (af->type >= 1) && (af->type <= MAX_SPELLS)) {
        send_to_char(spell_wear_off_msg[af->type],i);
        send_to_char("\r\n",i);
        sprintf(tmp,"You release the %s-weave you've woven on %s\r\n",spells[af->type],GET_NAME(i));  
        send_to_char(tmp,ch);       
        affect_remove(i,af);
      }
    }
  }
}

void remove_room_maintained(struct char_data *ch)
{
  static struct affected_type *af,*next;
  char tmp[500];
  int i;

  for (i = 0; i < top_of_world; i++) {
    if (!world[i].aff)
      continue;
    for (af = world[i].aff; af; af = next) {
      next = af->next;
      if ((af->weaver == GET_IDNUM(ch)) && (!(af->tie_info)) && (af->type >= 1)&&(af->type <= MAX_SPELLS)) {
        sprintf(tmp,"You release the %s-weave you've woven in %s\r\n",spells[af->type],world[i].name);
        affect_remove_room(&world[i],af);
        send_to_char(tmp,ch);
      }
    }
  }
}

int get_ppos_char(struct char_data *ch) {
  int i = 0;

  for (i = 0; i <= top_of_p_table; i++)
    if (player_table[i].id == GET_IDNUM(ch))
      return i;
  return NOTHING;
}

struct char_data *get_char_ppos(int num)
{
  static struct char_data *i;

  for (i = character_list; i; i = i->next) 
     if (GET_PFILEPOS(i) == num)
        return i;

  return NULL;
}

void cast_random_spell(struct char_data *ch)
{
} 

void do_nasty_taint(struct char_data *ch) 
{
  int perc,rand;
  char tmp[500];

  char *abils[] = {
    "strength",
    "constitution",
    "charisma",
    "intelligence",
    "wisdom",
    "dexterity"
  };

  perc = (int)((GET_TAINT(ch) / (float)MAX_TAINT) * 200.0);
  perc = MAX(1,MIN(100,perc));
  perc -= number(0,(2 * (GET_INT(ch) + GET_WIS(ch)) / 3));
  perc = MAX(1,perc);
  rand = number(0,1000);

  if (rand > perc) {
    return;
  }
  rand = number(0,5);
  switch(rand) {
    case 0:
      if (1 == GET_RSTR(ch)) {
        return;
      }
      GET_RSTR(ch) -= 1;
      break;
    case 1:
      if (1 == GET_RCON(ch)) {
        return;
      }
      GET_RCON(ch) -= 1;
      break;
    case 2:
      if (1 == GET_RCHA(ch)) {
        return;
      }
      GET_RCHA(ch) -= 1;
      break;
    case 3:
      if (1 == GET_RINT(ch)) {
        return;
      }
      GET_RINT(ch) -= 1;
      break;
    case 4:
      if (1 == GET_RWIS(ch)) {
        return;
      }
      GET_RWIS(ch) -= 1;
      break;
    case 5:
      if (1 == GET_RDEX(ch)) {
        return;
      }
      GET_RDEX(ch) -= 1;
      break;
  }
  sprintf(tmp,"The taint seeps into your body, corrupting you, and you loose\r\n" 
              "1 point of %s.\r\n",abils[rand]);
  send_to_char(tmp,ch);
}

void do_evil_taint(struct char_data *ch) 
{
  int perc,rand;

  perc = (int)((GET_TAINT(ch) / (float)MAX_TAINT) * 200.0);
  perc = MAX(1,MIN(100,perc));
  perc -= number(0,(2 * (GET_INT(ch) + GET_WIS(ch)) / 3));
  perc = MAX(1,perc);

  rand = number(0,150);

  if (rand > perc) {
    return;
  }

  if (!GET_CHAR_GUILDS(ch))
    rand = number(0,1); 
  else
    rand = number(0,3);

  switch(rand) {
    case 0:
/* Normal Skill*/
      break;
    case 1:
/* Normal Spell */
      break;
    case 2:
/*Guildskill*/
      break;
    case 3:
      /*Guildspell*/
      break;
  }
}

void do_crazy_taint(struct char_data *ch)
{
  int perc,rand;

  perc = (int)((GET_TAINT(ch) / (float)MAX_TAINT) * 200.0);
  perc = MAX(1,MIN(100,perc));
  perc -= number(0,(2 * (GET_INT(ch) + GET_WIS(ch)) / 3));
  rand = number(0,100);

  if (rand > perc) {
    return;
  }

  rand = number(0,20);

  if (POS_SLEEPING == GET_POS(ch))
    rand = 20;

  switch(rand) {
    case 1:
      send_to_char("Run away, flee! They're out to get you! Flee before"
           "it's to late!\r\n",ch);
      break;
    case 2:
      send_to_char("Kill them, kill them all! Kill them! Now! Blood! Kill them! Evryone, yes, kill everyone! Now!\r\n",ch);
      break; 
    case 3:
      act("$n suddenly starts to laugh violently, then just as suddenly"
      " he stops...\n",FALSE,ch,0,0,TO_NOTCHAR);
      send_to_char("Still, it feels good to laugh about it...\r\n",ch);
      break;
    case 4:
      send_to_char("You head is suddenly filled with visions of blood and\r\n"
         "destruction. You feel an urge to kill everyone in an endless\r\n"
     "orgy of death and unimaginalbe horrors!\r\n",ch);
      act("$n looks straight at you with an evil grin on his lips..\n",
      FALSE,ch,0,0,TO_NOTCHAR);
      break;
    case 5:
      act("$n clenches his fists and glares around the room, fuming with anger!\n",
          FALSE,ch,0,0,TO_NOTCHAR);
      act("You clenches your fists and glare at all those idiots present in the room...\n",
           FALSE,ch,0,0,TO_CHAR);
      break;
    case 6:
      act("You hear a voice in your head, saying \"Kill them, kill them all\n"
          "before it's to late...They're all out go get you, kill them now!\"\n"
          "over and over again...\n",FALSE,ch,0,0,TO_CHAR);
      act("$n suddenly is rigid with horror, looking with wild eyes at everyone present!\n",
           FALSE,ch,0,0,TO_NOTCHAR);
      break;
    case 7:
      command_interpreter(ch,"growl");
      break;
    case 8:
      command_interpreter(ch,"glare");
      break;
    case 9:
      if (PRF_FLAGGED(ch,PRF_GRASPING)) {
        REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
        act("$n suddenly decides to release the one power...\n",FALSE,ch,0,0,TO_OPUSERSS);
      }
      else {
        if (ROOM_FLAGGED(ch->in_room,ROOM_NOMAGIC)) {
          send_to_char("You feel an incredible rage burst through you!\r\n",ch);
          act("$n scowls violently at the world in general\n",FALSE,ch,0,0,TO_NOTCHAR);
        }
        else {
          SET_BIT(PRF_FLAGS(ch),PRF_GRASPING);
          act("$n suddenly seizes Saidin...\n",FALSE,ch,0,0,TO_OPUSERSS);
          act("You feel a faint tingle in your skin...\n",FALSE,ch,0,0,TO_OPUSERNSS);
        }
      }
      break;
    case 10:
      act("$n stares at the sky for a long time, then snaps back to reality\n", FALSE,ch,NULL,NULL,TO_NOTCHAR);
      break;
    case 11:
      act("$n starts nodding, and mumbling to himself.\n",FALSE,ch,0,0,TO_NOTCHAR); 
      send_to_char("Nice fella to talk to, this Spiney Norman...\r\n",ch);
      break;
    case 12:
      cast_random_spell(ch);
      break;
    case 13:
      command_interpreter(ch,"sing");
      break;
    case 14:
      send_to_char("What was that? A movement, there, in the shadows?\r\n",ch);
      break;
    case 15:
      command_interpreter(ch,": stands up and shouts 'Stop following me your"
                             "BA***RDS!\r\n");
      break;
    case 16:
      command_interpreter(ch,"mutter");
      break;
    case 17:
      command_interpreter(ch,"laugh");
      break;
    case 18:
      command_interpreter(ch,"sundance");
      break;
    case 19:
      send_to_char("You feel someone, or something, watching you...\r\n",ch);
      break;
    case 20:
      send_to_char("A voice suddenly booms: 'YOU CANNOT ESCAPE, MAGGOTS. SOON"
    " I, SHAI'TAN WILL RULE THIS PUNY WORLD! BOW TO YOUR TRUE MASTER NOW!"
    "\r\n",ch);
      break;
    default:
      command_interpreter(ch,"'Argle-bargle, bork bork bork!");
      break;  
  }
}
 
void do_taint_stuff(struct char_data *ch)
{

  if (IS_IMMORTAL(ch) || (get_guild(ch,7)))
    return;

  if (cant_channel(ch))
    return;

  if (!GET_TAINT(ch))
    return;

  if (GET_TAINT(ch) >= ((MAX_TAINT / 10) * 6)) {
    do_nasty_taint(ch);
  }
  else if (GET_TAINT(ch) >= ((MAX_TAINT / 10) * 4)) {
    do_evil_taint(ch);
  }
  else {
    do_crazy_taint(ch);
  }
}

void taint_effects(void)
{
  static struct char_data *i;

  return;

  for (i = character_list; i != NULL; i = i->next) {
    if (!cant_channel(i) && (SEX_MALE == GET_SEX(i))) {
      do_taint_stuff(i);
    }
  }
}

void update_noinc(void)
{
  static struct char_data *i;
  static int j;

  for (i = character_list; i != NULL; i = i->next) {
    if (IS_NPC(i))
      continue;
    for (j = 0; j < MAX_SKILLS+1; j++) {
      if (!GET_INC_TIME(i,j)) {
        continue;
      }
      else if (GET_INC_TIME(i,j)) {
    GET_INC_TIME(i,j) = 0;
        CLEAR_NOINC(i,j);
      }
    }
  }
}

/* affect_update: called from comm.c (causes spells to wear off) */
void affect_update(void)
{
  static struct affected_type *af, *next;
  static struct char_data *i,*ch;
  char tmp[500]; 
  int mana_cost = 0,type;

  for (i = character_list; i; i = i->next)
  {  
    for (af = i->affected; af; af = next) {
      next = af->next;
      if (af->duration > 1) {
        if (!(af->tie_info) && (af->type >= 1)&&(af->type <= MAX_SPELLS)) {
          if (!(ch = get_char_id(af->weaver))) 
            af->duration--; /* somehow we didn't get the weaver! */
          else {
            mana_cost = ((spell_info[af->type].mana + af->mana_add) / af->duration);
            GET_MANA(ch) -= mana_cost;
            sprintf(tmp,"Your maintaining of your %s-weave on %s cost you %d mana.\r\n",spells[af->type],GET_NAME(i),mana_cost);
            if(!PLR_FLAGGED(ch, PLR_MAILING | PLR_WRITING))
              send_to_char(tmp,ch);
          }
        }
        else
      af->duration--;
      }
      else if (af->duration == -1)	/* No action */
    af->duration = -1;	/* GODs only! unlimited */
      else {
    if ((af->type > 0) && (af->type <= MAX_SKILLS))
      if (!af->next || (af->next->type != af->type) ||
          (af->next->duration > 0))
        if (*spell_wear_off_msg[af->type] && !PLR_FLAGGED(i, PLR_MAILING | PLR_WRITING))
            {
          send_to_char(spell_wear_off_msg[af->type], i);
          send_to_char("\r\n", i);
        }
      type = af->type;
          affect_remove(i, af);
      if (SKILL_ARREST == type)
        do_follow(i,GET_NAME(i),0,0);
      }
    }
  }
}

void mana_update(int taint)
{
  static struct char_data *i;

  for (i = character_list; i != NULL; i = i->next) {
    if (!IS_NPC(i)) {
      if (i->player_specials->linked_by)
        continue;
      if (IS_SET(PRF_FLAGS(i),PRF_GRASPING) && GET_SEX(i) == SEX_FEMALE)
        GET_MANA(i) -= 10;
      else if (IS_SET(PRF_FLAGS(i),PRF_GRASPING) && GET_SEX(i) == SEX_MALE) {
        if (taint && IS_SET(PRF_FLAGS(i),PRF_IC)) {
          GET_TAINT(i) += add_taint(i,10);
        }
        GET_MANA(i) -= 15;
      }
      else if (IS_SET(PRF_FLAGS(i),PRF_TGRASP))
        GET_MANA(i) -= 20;
    }
  }
}

void update_timers(void)
{
  struct char_data *i;
  int j;

  for (i = character_list; i != NULL; i = i->next) {
    for (j = 0; j < MAX_CAN; j++) {
      if (GET_TIMES(i,j)) {
        GET_TIMES(i,j) -= PULSE_VIOLENCE;
      }
    }
  }
}

void perform_tie_char(struct char_data *ch,struct char_data *target,int weave)
{
  struct affected_type *af;  
  int found = 0;
  char tmp[500]; 

  if (!(target->affected)) {
    send_to_char("Your target is not affected by any weaves!\r\n",ch);
    return;
  }
  for (af = target->affected; af; af = af->next) {
    if ((af->type == weave) && (af->weaver == GET_IDNUM(ch))) {
      if (!(af->tie_info)) {
        af->tie_info = 1; /* Weave is tied! */
        sprintf(tmp,"You tie off the %s-weave on %s.\r\n",spells[af->type],((ch == target) ? "yourself" : GET_NAME(target)));
        send_to_char(tmp,ch);
        if (target != ch) {
          sprintf(tmp,"%s ties off the %s-weave on you!\r\n",GET_NAME(ch),spells[af->type]);
          send_to_char(tmp,target);
        }
        found = TRUE;
      }
    }
  }
  if (!found) {
    sprintf(tmp,"You can't find that weave (maintained) on %s!\r\n",((ch == target) ? "yourself" : GET_NAME(target)));
    send_to_char(tmp,ch);
  }
}

void perform_tie_room(struct char_data *ch,struct room_data *target,int weave)
{
  struct affected_type *af;  
  int found = 0;
  char tmp[500]; 

  if (!(target->aff)) {
    send_to_char("This room is not affected by any weaves!\r\n",ch);
    return;
  }
  for (af = target->aff; af; af = af->next) {
    if ((af->type == weave) && (af->weaver == GET_IDNUM(ch))) {
      if (!(af->tie_info)) {
        af->tie_info = 1; /* Weave is tied! */
        sprintf(tmp,"You tie off the %s-weave in this room.\r\n",spells[af->type]);
        send_to_char(tmp,ch);
        found = TRUE;
      }
    }
  }
  if (!found) {
    sprintf(tmp,"You can't find that weave (maintained) in this room!\r\n");
    send_to_char(tmp,ch);
  }
}

void perform_unravel_char(struct char_data *ch,struct char_data *target,int weave)
{
  struct affected_type *af;  
  char tmp[500]; 
  int found = FALSE,success = FALSE,defender = 0,user = 0,chance = 0;
  struct char_data *weaver = NULL;
  struct affected_type *tweave = NULL;

  if (NULL == target->affected) {
    send_to_char("Wait a minute. Your target isn't affected by anything!\r\n",ch);
    return;
  }

    
  for (af = target->affected; (af && !found); af = af->next) {
    if (af->type == weave) {
      found = TRUE;
      tweave = af;
    }
  }   
    
  if (!found || (tweave == NULL)) {
    send_to_char("You cant find that weave on your target!\r\n",ch);
    return;
  }
  /* Decision was made to make maintained weaves !unravel */
  if (!(tweave->tie_info)) {
    send_to_char("That weave is being maintained.  You cannot unravel it.\r\n",ch);
    return;
  }
  
  if (tweave->weaver == GET_IDNUM(ch))
    success = TRUE;
  else {
    defender = ((tweave->tie_info) ? tweave->mana_add : (2 * tweave->mana_add));
    user = MIN((GET_MANA(ch)-50),defender);
    chance = 50 + ((defender - user) * 5);
    chance = MIN(100,MAX(chance,5));  
    weaver = get_char_id(tweave->weaver);
    if (weaver && GET_SEX(weaver) != GET_SEX(ch)) 
      chance /= 2;
    if (!(tweave->tie_info))
      chance /= 2;

    if (number(0,100) < chance)
      success = TRUE;
    else
     success = FALSE;
  }
  if (success) {
    sprintf(tmp,"Okay, you unravel the %s-weave woven on %s.\r\n",spells[tweave->type],
                 ((ch == target) ? "yourself" : GET_NAME(target)));
    send_to_char(tmp,ch);
    if (target != ch) {
      sprintf(tmp,"%s unravels the %s-weave on you!\r\n",GET_NAME(ch),spells[tweave->type]);
    send_to_char(tmp,target);
    } 
    send_to_char(spell_wear_off_msg[tweave->type],target);
    send_to_char("\r\n",target);
    sprintf(tmp,"$n weaves flows of Spirit and unravels the %s-weave on $N.",spells[tweave->type]);
    act(tmp,FALSE,ch,0,target,TO_OPUSERSS);
    GET_MANA(ch) -= user;   

    /* Commented out the messages for people whose maintained weaves are unraveled since that is not allowed anymore
    if (weaver) {
        if (!(tweave->tie_info) && (weaver->in_room != ch->in_room) && (weaver != ch)) {
        sprintf(tmp,"Suddenly you feel the %s-weave you've woven on %s unravel!\r\n",spells[tweave->type],GET_NAME(target));
        send_to_char(tmp,weaver);
      }
      else if (!(tweave->tie_info) && (weaver->in_room == ch->in_room) && (weaver != ch)) {
        sprintf(tmp,"You feel %s unraveling the %s-weave you've put on %s!\r\n",GET_NAME(ch),spells[tweave->type],GET_NAME(target));
        send_to_char(tmp,weaver);
      }
    } */          
    affect_remove(target,tweave); 
  }
  else {
    sprintf(tmp,"You fail to unravel the %s-weave woven on %s.\r\n",spells[tweave->type],
                 ((ch == target) ? "yourself" : GET_NAME(target)));
    send_to_char(tmp,ch);
    if (target != ch) {
      sprintf(tmp,"%s tries to unravel the %s-weave on you,but fails!\r\n",GET_NAME(ch),spells[tweave->type]);
      send_to_char(tmp,target);
    } 
    sprintf(tmp,"$n weave flows of Spirit and tries to unravel the %s-weave on $N,but fails!",spells[tweave->type]);
    act(tmp,FALSE,ch,0,target,TO_OPUSERSS);
    GET_MANA(ch) -= user;   

    if (weaver) {
      if (!(tweave->tie_info) && (weaver->in_room != ch->in_room) && (weaver != ch)) {
        sprintf(tmp,"Suddenly you feel someone trying to unravel the %s-weave you've woven on %s.\r\n",spells[tweave->type],GET_NAME(target));
        send_to_char(tmp,weaver);
      }
      else if (!(tweave->tie_info) && (weaver->in_room == ch->in_room) && (weaver != ch)) {
        sprintf(tmp,"You feel %s trying to unravel the %s-weave you've put on %s, but failing utterly!\r\n",GET_NAME(ch),spells[tweave->type],GET_NAME(target));
        send_to_char(tmp,weaver);
      }
    }            
  }
}

void perform_unravel_room(struct char_data *ch,struct room_data *target,int weave)
{
  struct affected_type *af,*tweave = NULL;  
  char tmp[500]; 
  int found = 0, success = 0, user = 0, defender = 0,chance = 0;
  struct char_data *weaver = NULL;

  if (NULL == target->aff) {
    send_to_char("But this room isn't affected by anything!\r\n",ch);
    return;
  }

  for (af = target->aff; af; af = af->next) {
    if (af->type == weave) {
      found = TRUE;
      tweave = af;
    }
  }
   
  if (!found || (tweave == NULL)) {
    send_to_char("You cant find that weave in this room!\r\n",ch);
    return;
  }

  if (tweave->weaver == GET_IDNUM(ch))
    success = TRUE;
  else {
    defender = ((tweave->tie_info) ? tweave->mana_add : (2 * tweave->mana_add));
    user = MIN((GET_MANA(ch)-50),defender);
    chance = 50 + ((defender - user) * 5);
    chance = MIN(100,MAX(chance,5));  
    weaver = get_char_id(tweave->weaver);
    if (weaver && GET_SEX(weaver) != GET_SEX(ch)) 
      chance /= 2;
    if (!(tweave->tie_info))
      chance /= 2;

    if (number(0,100) < chance)
      success = TRUE;
    else
     success = FALSE;
  }
  if (success) {
    sprintf(tmp,"Okay, you unravel the %s-weave woven in this room.\r\n",spells[tweave->type]);
    send_to_char(tmp,ch);

    sprintf(tmp,"$n weave flows of Spirit and unravels the %s-weave in this room.",spells[tweave->type]);
    act(tmp,FALSE,ch,0,target,TO_OPUSERSS);
    GET_MANA(ch) -= user;   

    if (weaver) {
      if (!(tweave->tie_info) && (weaver->in_room != ch->in_room) && (weaver != ch)) {
        sprintf(tmp,"Suddenly you feel the %s-weave you've woven in %s unravel!\r\n",spells[tweave->type],target->name);
        send_to_char(tmp,weaver);
      }
      else if (!(tweave->tie_info) && (weaver->in_room == ch->in_room) && (weaver != ch)) {
        sprintf(tmp,"You feel %s unraveling the %s-weave you've put on this room!\r\n",GET_NAME(ch),spells[tweave->type]);
        send_to_char(tmp,weaver);
      }
    }            
    affect_remove_room(target,tweave);
  }
  else {
    sprintf(tmp,"You fail to unravel the %s-weave woven on this room.\r\n",spells[tweave->type]);
    send_to_char(tmp,ch);

    sprintf(tmp,"$n weave flows of Spirit and tries to unravel the %s-weave in this room,but fails!",spells[tweave->type]);
    act(tmp,FALSE,ch,0,target,TO_OPUSERSS);
    GET_MANA(ch) -= user;   

    if (weaver) {
      if (!(tweave->tie_info) && (weaver->in_room != ch->in_room) && (weaver != ch)) {
        sprintf(tmp,"Suddenly you feel someone trying to unravel the %s-weave you've woven in %s.\r\n",spells[tweave->type],target->name);
        send_to_char(tmp,weaver);
      }
      else if (!(tweave->tie_info) && (weaver->in_room == ch->in_room) && (weaver != ch)) {
        sprintf(tmp,"You feel %s trying to unravel the %s-weave you've put in this room, but failing utterly!\r\n",GET_NAME(ch),spells[tweave->type]);
        send_to_char(tmp,weaver);
      }
    }            
  }
}

void perform_unknot_char(struct char_data *ch,struct char_data *target,int weave)
{
  struct affected_type *af;  
  int found = FALSE;
  char tmp[500]; 

  if (NULL == target->affected) {
    send_to_char("But that person isn't affected by anything!\r\n",ch);
    return;
  }

  for (af = target->affected; af; af = af->next) {
    if ((af->type == weave) && (af->tie_info) && (af->weaver == GET_IDNUM(ch))) {
      found = TRUE;
      af->tie_info = 0;
      sprintf(tmp,"You start to maintain the %s-weave on %s.\r\n",spells[af->type],((ch == target) ? "yourself" : GET_NAME(target)));
      send_to_char(tmp,ch);
      if (target != ch) {
        sprintf(tmp,"%s unknots his %s-weave on you, and starts to maintain it.\r\n",GET_NAME(ch),spells[af->type]);
        send_to_char(tmp,target);
      }
    }
  }
  if (!found) {
    sprintf(tmp,"You haven't tied any %s on %s!\r\n",spells[weave],((ch == target) ? "yourself" : GET_NAME(target)));
    send_to_char(tmp,ch);
  }
}

void perform_unknot_room(struct char_data *ch,struct room_data *target,int weave)
{ 
 struct affected_type *af;  
 int found = FALSE;
 char tmp[500]; 

  if (NULL == target->aff) {
    send_to_char("But this room isn't affected by anything!\r\n",ch);
    return;
  }

  for (af = target->aff; af; af = af->next) {
    if ((af->type == weave) && (af->tie_info) && (af->weaver == GET_IDNUM(ch))) {
      found = TRUE;
      af->tie_info = 0;
      sprintf(tmp,"You start to maintain the %s-weave in this room\r\n",spells[af->type]);
      send_to_char(tmp,ch);
    }
  }
  if (!found) {
    sprintf(tmp,"You haven't tied any %s in this room!\r\n",spells[weave]);
    send_to_char(tmp,ch);
  }
}

ACMD(do_tie)
{
  char *temp1,*temp2;
  int num;
  struct char_data *target = NULL;
  struct room_data *troom = NULL;
  char tmp[500];
  char *action[] = {
   "unknot",
   "unravel",
   "tie"
  };

  if (cant_channel(ch)) {
    send_to_char("You can't channel your moron!\r\n",ch);
    return;
  }

  if (!PRF_FLAGGED(ch,PRF_GRASPING) && !PRF_FLAGGED(ch,PRF_TGRASP)) {
    do_grasp(ch,"",0,0);
  }

  if (!PRF_FLAGGED(ch,PRF_GRASPING) && !PRF_FLAGGED(ch,PRF_TGRASP)) {
    sprintf(tmp,"You must be grasping before you can %s!\r\n",action[subcmd]);
    send_to_char(tmp,ch);
    return;
  }

  if (!*argument) {
    send_to_char("Yeah, right...Perhaps if you tell me what to do?\r\n",ch);
    return;
  }

  temp1 = strtok(argument,"'");
  
  if (NULL == temp1) {
    send_to_char("What weave?\r\n",ch);
    return;
  }
  temp1 = strtok(NULL,"'");

  if (NULL == temp1) {
    sprintf(tmp,"Usage: %s 'weave' <target|room>\r\n",action[subcmd]);
    send_to_char(tmp,ch);
    return;
  }

  temp2 = strtok(NULL,"\0");

  num = find_skill_num(temp1);
   
  if ((num < 1) || (num > MAX_SPELLS)) {
    send_to_char("Yeah, yeah. Very funny. NOT!\r\n",ch);
    return;
  }

  if (NULL != temp2) 
   skip_spaces(&temp2);

  if (!temp2) 
    target = ch;
  else if (!strncasecmp("room",temp2,strlen(temp2)))  
    troom = &world[ch->in_room];     
  else
    target = get_char_room_vis(ch,temp2);

  if ((NULL == target) && (NULL == troom))  {
    send_to_char("Ehr, perhaps if you supply a valid target?\r\n",ch);
    return;
  }
  
  switch (subcmd) {
    case SCMD_TIE:
      ((target != NULL) ? perform_tie_char(ch,target,num) : perform_tie_room(ch,troom,num));
      break;
    case SCMD_UNKNOT:
      ((target != NULL) ? perform_unknot_char(ch,target,num) : perform_unknot_room(ch,troom,num));
      break;
    case SCMD_UNRAVEL:
      ((target != NULL) ? perform_unravel_char(ch,target,num) : perform_unravel_room(ch,troom,num));
  }
}    
