/******************************************************************************
 * $Id: spell_parser.c,v 1.9 2003/06/11 03:30:30 fang Exp $
 *   
 * File: spell_parser.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: spell_parser.c,v $
 *   Revision 1.9  2003/06/11 03:30:30  fang
 *   Added WRATH weave
 *
 *   Revision 1.8  2002/11/07 04:02:04  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.7  2002/09/22 21:43:50  fischer
 *   Removing mounts and horsetrain from the code
 *
 *   Revision 1.6  2002/09/05 14:06:35  fang
 *   Drath: Skill Name update
 *
 *   Revision 1.5  2002/09/04 15:06:06  fang
 *   Drath: Trimmed skill/weave names
 *
 *   Revision 1.4  2002/09/03 22:19:44  fang
 *   Drath: Fixed skill names
 *
 *   Revision 1.3  2002/07/28 13:56:05  fang
 *   Removed dmlloc...just to test.
 *
 *   Revision 1.2  2002/07/26 19:39:52  fang
 *   Dath: MYCOLOR mem leak fixed.  Thanks, Fred!
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.6  2002/07/16 15:16:17  fang
 *   Drath: Seafolk Guildskills are done.
 *
 *   Revision 1.5  2002/07/14 20:28:31  fang
 *   Drath: Changed the skillname for protege to protege from prodigy
 *
 *   Revision 1.4  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.3  2002/05/15 10:08:49  fang
 *   open lock spell now works on containers in room/worn/in inventory
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.16  2002/01/24 21:33:20  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.15.2.2  2002/01/24 20:49:38  mud
 *   More cleanup. Removed files:  specialday.c string.c string.h
 *   Removed DONT_TRADE_WITH-stuff from shops.
 *
 *   Revision 1.15.2.1  2002/01/22 03:05:15  mud
 *   More cleanup. Removed extern decls from source files. Protected some DG headers from multiple inclision
 *
 *   Revision 1.15  2001/08/20 13:49:47  mud
 *   *** empty log message ***
 *
 *   Revision 1.14  2001/07/20 04:47:59  mud
 *   *** empty log message ***
 *
 *   Revision 1.13  2001/07/18 17:09:11  mud
 *
 *   bond becomes spells ashabond and sedaibond
 *
 *   Revision 1.12  2001/03/24 03:25:46  mud
 *   *** empty log message ***
 *
 *   Revision 1.11  2000/10/03 17:11:47  mud
 *   *** empty log message ***
 *
 *   Revision 1.10  2000/08/14 19:11:56  mud
 *   The new guild system.
 *
 *   Revision 1.9  2000/05/26 09:20:33  mud
 *   Fixed findherb, added a herbident ACMD, IMM commands level fixes.
 *
 *   Revision 1.8  2000/05/13 14:32:30  mud
 *   First changes to bond code in making of bond spell numbers.
 *
 *   Revision 1.7  2000/03/15 13:11:40  mud
 *   Changed from B-tree based skill/weave index to 'something else' :)
 *
 *   Revision 1.6  2000/03/09 04:23:43  mud
 *   Renamed 'enhance' to 'enhance item' due to code confusing enhance and enhanced damage.
 *
 *   Revision 1.5  2000/03/09 01:16:45  mud
 *   *** empty log message ***
 *
 *   Revision 1.4  2000/03/08 15:54:18  mud
 *   More on INFO, now almost 100% searchable.. :)
 *
 *   Revision 1.3  2000/03/06 07:34:47  mud
 *
 *   Changed around the 'enhance' and the 'enhanced damage' skill priorities. Hopefully to remeady the
 *   skillset problems.
 *
 *   XOXOXO
 *   Adammael
 *
 *   Revision 1.2  2000/02/21 14:00:19  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/


/* ************************************************************************
*   File: spell_parser.c                                Part of CircleMUD *
*  Usage: top-level magic routines; outside points of entry to magic sys. *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "act.h"  
#include "shop.h" 
#include "fight.h"
#include "conf.h"
#include "sysdep.h"
#include "class.h"
#include "constants.h"
#include "structs.h"
#include "utils.h"
#include "interpreter.h"
#include "spells.h"
#include "handler.h"
#include "comm.h"
#include "db.h"
#include "skills.h"

struct spell_info_type spell_info[TOP_SPELL_DEFINE + 1];

#define SINFO spell_info[spellnum]

/*
 * This arrangement is pretty stupid, but the number of skills is limited by
 * the playerfile.  We can arbitrarily increase the number of skills by
 * increasing the space in the playerfile. Meanwhile, this should provide
 * ample slots for skills.
 */

char *spells[] =
{
  "!RESERVED!",			/* 0 - reserved */

  /* SPELLS */

  "Armor",			/* 1 */
  "Airsword",
  "Gate",
  "Blind",
  "Invisibility",
  "Call lightning",
  "Firesword",
  "Wrap",
  "Gag",
  "Shield",		/* 10 */
  "Link (Unused)",
  "Gentle/Still",
  "Create water",
  "Heal",
  "Heal Gent/Still",
  "Airshield",
  "Burn",
  "Airblow",
  "Ice Arrows",
  "Detect Magic",		/* 20 */
  "Meteor",
  "Light",
  "Earthquake",
  "Rolling Ring",
  "Lightning Bolt",
  "Fireball",
  "Darkness",
  "Blindness",
  "Compulsion",
  "Cure Light",		/* 30 */
  "Cure Serious",
  "Cure Critical",
  "Sanctuary",
  "Sleep",
  "Sense Life",
  "Infravision",
  "Chill",
  "Earthshield",
  "Watercascade",
  "Icestorm",		/* 40 */
  "Djinn",
  "Pain",
  "Fly",
  "Block gateway",
  "Create Food",
  "Slice Weave",	
  "Whirlwind",    	/*Green Ajah guild spell */
  "Delve",              /*Yellow Ajah guild spell */
  "Minor Healing",
  "Dome of Air",		/* 50 */
  "Ball of Light",
  "Disintegrate",
  "Ball of Air",
  "Open Lock",
  "Call Fire",
  "Shaft of Fire",
  "Dome of Fire",
  "!UNUSED!",
  "Destroy Weapon",
  "Drown",	/* 60 */
  "Poison",
  "Invigorate",
  "Storm",
  "Dart of Air",
  "Call Frost",	/* 65 */
  "Shaft of Frost",
  "Wind Armor",
  "Confusion",
  "Slow",
  "Unbalance",	/* 70 */
  "Explosion",
  "Dome of Frost",
  "Fist of Air",
  "Firestorm",
  "Refresh",	/* 75 */
  "Earth Armor",
  "Destroy Armor",
  "Fire Arrows",
  "Earth Missile",
  "Kin Heal",	/* 80 */
  "Shield of Saidin",
  "Cripple",
  "Deaf",
  "Advanced Heal",
  "Thunderburst",	/* 85 */
  "Stormshield",
  "Group Restore",
  "Ward Human", 
  "Identify", 
  "Fog",	/* 90 */
  "Entangle", 
  "Mend", 
  "Frostbite",
  "Room Seal", 
  "TAR-Gate",	/* 95 */
  "Skim", 
  "Sedaibond",
  "Ashabond",
  "Watershield",
  "Storms-Fury",	/* 100 */
  "Wrath",
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",	/* 105 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",	/* 110 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",	/* 115 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",	/* 120 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",	/* 125 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",	/* 130 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 140 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 145 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",	/* 150 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 160 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 170 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 180 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",   /* 190 */
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",
  "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!", "!UNUSED!",	/* 200 */


  /* SKILLS */

  "Backstab",			/* 201 */
  "Bash",
  "Hide",
  "Kick",
  "Pick Lock",
  "Punch",
  "Rescue",
  "Sneak",
  "Steal",
  "Track",			/* 210 */
  "First Aid",
  "Dodge",
  "Second Attack",
  "Third Attack",
  "Fourth Attack",
  "Shield Attack",
  "Trip",
  "Stun",
  "!UNUSED!",   /* Used to be enhance */
  "Roll With Blow",	/* 220 */
  "Endurance",
  "March",
  "Fast Healing",
  "Archery",
  "Diagnose",	
  "Berserk",
  "Tool",
  "Herblore",
  "Herbmaster",
  "Fifth Attack",	/* 230 */
  "Grasp",
  "Release",
  "T_Grasp",
  "T_Release",
  "Speardance",	
  "Disarm",
  "Two Weapons",
  "Shield Block",
  "Parry",
  "Sword Dancing",	/* 240 */
  "Dirt Kick",
  "Unarmed Combat",
  "Dreaming",
  "Duck",
  "Play Instrument",	
  "Teach",
  "Portal Stones",
  "Hide Tracks",
  "Sliceweave",
  "Learn",	/* 250 */
  "Privacy",
  "Dreamlist",
  "Dreampull",
  "Breakshield",
  "!UNUSED!", 
  "Craft", 
  "Powertrap", 
  "Jump", 
  "Meditation", 
  "Appraise",	/* 260 */
  "Enhanced Damage",
  "Defense",
  "Mask", 
  "Fortify",
  "Protege",	
  "!UNUSED!", 
  "Locate",
  "Black Arts", "Dreamenter", "Wolfkin Set",	/* 270 */
  "Ttest", 
  "Tinfo", 
  "Wolfsense", "Wolfcall", 
  "Navigate",	
  "Bargain", 
  "Saltbrothers", 
  "Eavesdrop", 
  "Marked Map", 
  "Adrenalin",	/* 280 */
  "Knife Toss", 
  "Awareness", 
  "!UNUSED!", 
  "Charm", 
  "Dazzle",	
  "Privacy" , "!UNUSED!", "!UNUSED!", "!UNUSED!",
  /* Caemlyn Guild Skills */
  "Fine",	/* 290 */
  "Throw", 
  "Armscry",
   "!UNUSED!", "!UNUSED!",

   "Supply",
               /* Tinker Guild Skills */
    
  "Enhance",// item",   // Was 'enhance'... now changed for skillset. -Adam
  "Repair",     // Not any fskickng more, it isn't! :>
  "Camp",
  "Travel",
                      /* 2 Rivers Guild Skills */
  "Survival",	/* 300 */
  "Medicine",
  "Stubborness",

                 /* Darkfriend guild skills */	
  "Codetalk",
  "Summon",
  "Sever",
  "Hunt",
  "Test",
  "Obscure",
  "Lip Reading",
  "Dark Gift",	/* 310 */
              /* Gleeman guild skills */
  "Tail",
  "FX",
  "Rumor",
  "Shieldwall",
  "Patrol",	
              /* BotRH guild skills */
  "Tactics",
  "Flank",
  "Brace",
  "Aim",
  "Strike",	/* 320 */
  "Motivation",
  "Bandmarch",
  "Inquisition",
  /* Cotl reserve spot */
  "Fervor",
            /*	Tower Warder Guild Skills */
  "Void",	/* 325 */
  "Warderhealing",
  "Wardercombat",
  "Wardersense",
  "Forage",
       /* Tower Sedai Guild Skills */
  "Mediate",	/* 330 */
  "Knowledge",
  "Know Health",
  "Quick Mind",
  "Seek",
         /* Army of the Dragon Guild Skills */
  "Repel",	
  "Arrest",
  "Preach", "!UNUSED!", "!UNUSED!",
 /* Forsaken Guild Skills */
  "SFX",	/* 340 */
  "Disguise",

   /* Aiel Guild Skills */

  "Rally",
  "Rage",
  "Pound",
  "Camouflage",	
  "Peace",
  "Herbmastery",
  "Veil",
  "Handtalk",
  "Infravision",	/* 350 */
  "Traction",
  "Retreat",
  "Search",
  "Disengage",
  "Stab",	
  "Speed",
  "Critical Hit",
  "Stalk",
  "Run",
/* CoTL Guildskills */
  "Morale",	                                                /* 360 */
  "Reinforce",
  "Brand",
  "Pure Light",
  "Charge",
/* Seanchan guild skills */
  "Leadership",	
  "Recruit",
  "Ravage", 
  "Response", "!UNUSED!",	
/* Ogier guild skills */
  "Treesinging", 				/* 370 */
  "!UNUSED!",
/* Hunters of the Horn Guildskills */
  "Shadow", 
  "Arrogance",
  "Offend",
  "!UNUSED!",	 
/* QUEST skills */
  "Blademastery",
  "Spearmastery",
  "Employ",
  "!UNUSED!","!UNUSED!",                 	/* 380 */
/* Language skills */
  "Trolloc",
  "Ogier",
  "Old Tongue",
  "!UNUSED!",
/* The Kin guild skills */
  "FindHerb",	  				/*385 */
  "HerbIdent", "!UNUSED!", "!UNUSED!", "!UNUSED!",

/* Asha'man Guildskills */
  "Concentrate", "Enh. Spell Dam", "Target",
  "!UNUSED!", "!UNUSED!",
/* Rebel AS guild skills */
  "Inspire",	
  "Guilddisguise",
  "Intimidation",
  "!UNUSED!",
  "Endure",
  "!UNUSED!",	/* 400 */

  /* OBJECT SPELLS AND NPC SPELLS/SKILLS */

  "Identify",			/* 401 */
  "Fire Breath",
  "Gas Breath",
  "Frost Breath",
  "Acid Breath",
  "P-fighting",
  "P-fighting",
  "Longsleep",
  "\n"				/* the end */
};



int mag_manacost(struct char_data * ch, int spellnum)
{
  return SINFO.mana;
}

char *skill_name(int num)
{
  int i = 0;

  if (num <= 0) {
    if (num == -1)
      return "UNUSED";
    else
      return "UNDEFINED";
  }

  while (num && *spells[i] != '\n') {
    num--;
    i++;
  }

  if (*spells[i] != '\n')
    return spells[i];
  else
    return "UNDEFINED";
}

    
int find_skill_num(char *name)
{
  skip_spaces(&name);
  return (find_skillnum_by_name(name));
}

int slice_weave_failed(struct char_data *ch, struct char_data *victim, int mana_add, int spellnum)
{
  int mana, vict_str, ch_str, chance;
  int result;
  

  if (!ch || !victim || spellnum < 1 || spellnum > MAX_SPELLS || IS_NPC(victim))
    return 1;

     /*  Seanchan guildskill, Response */
  if ((ch != victim) && GET_SKILL(victim, SKILL_RESPONSE) && (number(0, 100) < 33)) {
    result = generic_result_mod(victim, SKILL_RESPONSE, NULL, FALSE, -GET_SPIRIT(ch));
    act("$N suddenly makes a strange move.", TRUE, ch, 0, victim, TO_NOTVICTSPAM);
    if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) 
      send_to_char("You try to avoid a weave, but fail!\r\n", victim);
    else {
      send_to_char("You barely avoid a weave you feel coming.\r\n", victim);
      return 0;
    }
  }

  if (GET_SEX(ch) != GET_SEX(victim))
    return 1;
  if (!IS_SET(PRF2_FLAGS(victim), PRF2_SLICEWEAVE))
    return 1;

  if (ch == victim)
    return 1;

  if (GET_POS(ch) < POS_SITTING)
    return 1;

  mana = mana_add + spell_info[spellnum].mana;

  if (GET_MANA(victim) < (mana/2 + 0.1*GET_MAX_MANA(victim)))
    return 1;

  SET_MANA(victim, -(mana/2));

  vict_str = GET_FIRE(victim) + GET_SPIRIT(victim);
  ch_str = GET_FIRE(ch) + GET_SPIRIT(ch);
  chance = (vict_str - ch_str) / 14;
  if (!IS_SET(PRF_FLAGS(victim),PRF_GRASPING) && !IS_SET(PRF_FLAGS(victim),PRF_TGRASP))
    result = TOTAL_FAILURE;
  else
    result = generic_result_mod(victim, SKILL_SLICEWEAVE, NULL, FALSE, chance);

  if (result == TOTAL_FAILURE)
    return 1;

  if (result == TOTAL_FUCKUP) {
    act("You tried to slice through $N's weave, but $E was too strong!", TRUE, victim, 0, ch, TO_CHAR);
    act("$n tried to slice through your weave, but was too weak.", TRUE, victim, 0, ch, TO_VICT);
    return 1;
  }
  act("You slice right through $N's weave, which was aimed at you.", TRUE, victim, 0, ch, TO_CHAR);
  act("$n slices right through your weave!", TRUE, victim, 0, ch, TO_VICT);
  return 0;
}

long mana_check(long mana, struct char_data *ch)
{
  if (!mana) {
    return 0;
  }

  if (mana > (2*GET_MAX_MANA(ch))) {
    if (GET_LEVEL(ch) < 10) 
      mana = GET_MAX_MANA(ch);
    else 
      mana = (2*GET_MAX_MANA(ch));
  }
  else if (mana < 0) {
    mana = GET_MAX_MANA(ch);
  }
  else if ((GET_MANA(ch) - mana) >= GET_MANA(ch)) {
    mana = (GET_MANA(ch) - 100);
  }
  return mana;
}
 

double calc_flows(struct char_data *ch,int spell) 
{
  double req = 0.0, user = 0.0;
  int i;
  ubyte *fp = &(GET_AIR(ch));

  for (i = 0; i < 5; i++) {
    if ((0 != spell_info[spell].min_level[(int)GET_CLASS(ch)][i])) {
      req  += (double) spell_info[spell].min_level[(int)GET_CLASS(ch)][i];
      user += (double) fp[i]; 
    }
  }
  if (req == 0) { /* something went wrong, just return 1 */
    return 1.0;
  }
  return (user/req);    
} 

/*
 * do_cast is the entry point for PC-casted spells.  It parses the arguments,
 * determines the spell number and finds a target, throws the die to see if
 * the spell can be cast, checks for sufficient mana and subtracts it, and
 * passes control to cast_spell().
 */
ACMD(do_cast)
{
  char *temp1,*temp2,*temp3,*temp4,*p;
  int spell_num;
  char arg1[200],arg2[200];
  BPTR(temp);
  struct char_data *victim;
  struct obj_data *obj;
  sh_int room_num;
  int found = 0,num2,door,tmpmana,rc;
  double mod = 0.0;

  if (GET_TIMES(ch,CAN_WEAVE)) 
  {
    send_to_char("\\c01You can't weave again so soon!\\c00",ch);
    return;
  }

  memset(arg1,'\0',200);
  memset(arg2,'\0',200);
  if(ch->player_specials->linked_by)
  {
    send_to_char("You cannot control the One Power right now.\r\n", ch);
    return;
  }

  if (!IS_SET(PRF_FLAGS(ch),PRF_GRASPING) && !IS_SET(PRF_FLAGS(ch),PRF_TGRASP)) {
    do_grasp(ch,"",0,0);
  }

  if (!IS_SET(PRF_FLAGS(ch),PRF_GRASPING) && !IS_SET(PRF_FLAGS(ch),PRF_TGRASP)) {
    if (GET_SKILL(ch,SKILL_TGRASP))
      send_to_char("You must grasp the One/True Power before weaving any weaves!\r\n",ch);
    else
      send_to_char("You must grasp the One Power before weaving any weaves!\r\n",ch);
    return;
  }
 

  temp1 = strtok(argument,"'");

  if (NULL == temp1) {
    send_to_char("Weave what where?\r\n", ch);
    return;
  }

  temp1 = strtok(NULL,"'");

  if (NULL == temp1) {
    send_to_char("Weave names must be enclosed in the Symbols chosen by the Creator: '\r\n", ch);
    return;
  }

  temp2 = strtok(NULL,"\0");

  spell_num = find_skill_num(temp1);

  if ((spell_num < 1) || (spell_num > MAX_SPELLS)) {
    send_to_char("Weave what?!\r\n", ch);
    return;
  }

  mod = calc_flows(ch,spell_num);
  if (mod <= 0.0) mod = 1.0;		/* Sanity check! */

  if (!GET_SKILL(ch,spell_num)) {
    send_to_char("And how are you supposed to weave that?\r\nYou don't know how, woolhead!\r\n",ch);
    return;
  }

  if (spell_list[spell_num] == NULL) {
    send_to_char("Sorry, this weave isn't implemented yet.\r\nComplain to the Imms.\r\n",ch);
    return;
  }

  if (IS_SET(spell_info[spell_num].targets,IS_AGGRESSIVE) && ROOM_FLAGGED(ch->in_room,ROOM_PEACEFUL)) {
    send_to_char("You feel calm, and uncapable of violence.\r\n",ch);
    return;
  }
  temp = spell_list[spell_num];

  if (IS_SET(spell_info[spell_num].targets,NO_TARGET)) {
    if (IS_SET(spell_info[spell_num].targets,IS_AGGRESSIVE)) { /* Rest of string should be mana-add!*/
      for (victim = world[ch->in_room].people; victim && !found; victim = victim->next_in_room) {
        if (not_same_dreamer(ch,victim)) 
      continue;
        if (victim != ch)
          found = 1;
      }
      if (!found) {
        send_to_char("Bloody useless, you're alone in this room!\r\n",ch);
        return;
      }
    }
    if (!temp2)
      temp(ch,NULL,NULL,NULL,NULL,-1,-1,0,subcmd,mod);
    else {
      tmpmana = atoi(temp2);
      tmpmana = mana_check(tmpmana,ch);
      temp(ch,NULL,NULL,NULL,NULL,-1,-1,tmpmana,subcmd,mod);
    }
    if (GET_MANA(ch) < 0)
      check_mana(ch);
    return;
  }
  if (IS_SET(spell_info[spell_num].targets,ONLY_SELF)) {   /* Spell can ONLY be cat upon self */
   if (!temp2)
      temp(ch,ch,NULL,NULL,NULL,-1,-1,0,subcmd,mod);      /* Next in argument-string should be */
    else {					      /* mana-add! */
      tmpmana = atoi(temp2);
      tmpmana = mana_check(tmpmana,ch);
      temp(ch,ch,NULL,NULL,NULL,-1,-1,tmpmana,subcmd,mod);
    }
    if (GET_MANA(ch) < 0)
      check_mana(ch);
    return;
  }	

  if (IS_SET(spell_info[spell_num].targets,CHAR_TARGET)) {
    if (!temp2) {
      if (IS_SET(spell_info[spell_num].targets,IS_AGGRESSIVE)) {
        if (!FIGHTING(ch)) {
          send_to_char("Weave that weave on who?\r\n",ch);
          return;
        }
        if (MOB_FLAGGED(FIGHTING(ch), MOB_NOMAGIC)) {
          act("$N is immune to weaves!", TRUE, ch, 0, FIGHTING(ch), TO_CHAR);
          return;
        }
        if (IS_GHOLAM(FIGHTING(ch)) && IS_SET(PRF_FLAGS(ch),PRF_GRASPING)) {
          act("Your flows simply vanishes as they reach $N.",FALSE,ch,0,FIGHTING(ch),TO_CHAR);
          act("$ns flows vanishes as they reach $N",FALSE,ch,0,FIGHTING(ch),TO_OPUSERSS);
          act("$n looks at $N with fear.",FALSE,ch,0,FIGHTING(ch),TO_OPUSERNSS);
          act("$n looks at $N with fear.",FALSE,ch,0,FIGHTING(ch),TO_NOTCHANN);
          return;
        }
        if (slice_weave_failed(ch, FIGHTING(ch), 0, spell_num))
          temp(ch,FIGHTING(ch),NULL,NULL,NULL,-1,-1,0,subcmd,mod);
        if (GET_MANA(ch) < 0)
          check_mana(ch);
    return;
      }
      send_to_char("Weave that on who?\r\n",ch);
      return;
    }
    two_arguments(temp2,arg1,arg2);

    if (!(victim = get_char_room_vis(ch,arg1))) {
      if (IS_SET(spell_info[spell_num].targets,IS_AGGRESSIVE)) {
        if (!FIGHTING(ch)) {
          send_to_char("Weave that weave on who?\r\n",ch);
          return;
        }
        if (MOB_FLAGGED(FIGHTING(ch), MOB_NOMAGIC)) {
          act("$N is immune to weaves!", TRUE, ch, 0, FIGHTING(ch), TO_CHAR);
          return;
        }

        if (IS_GHOLAM(FIGHTING(ch)) && IS_SET(PRF_FLAGS(ch),PRF_GRASPING)) {
          act("Your flows simply vanishes as they reach $N.",FALSE,ch,0,FIGHTING(ch),TO_CHAR);
          act("$ns flows vanishes as they reach $N",FALSE,ch,0,FIGHTING(ch),TO_OPUSERSS);
          act("$n looks at $N with fear.",FALSE,ch,0,FIGHTING(ch),TO_OPUSERNSS);
          act("$n looks at $N with fear.",FALSE,ch,0,FIGHTING(ch),TO_NOTCHANN);
          return;
        }


        if (!*arg2) {
          tmpmana = atoi(arg1);
          tmpmana = mana_check(tmpmana,ch);
          if (slice_weave_failed(ch, FIGHTING(ch), tmpmana, spell_num))
            temp(ch,FIGHTING(ch),NULL,NULL,NULL,-1,-1,tmpmana,subcmd,mod);
        }
        else {
          tmpmana = atoi(arg2);
          tmpmana = mana_check(tmpmana,ch);
            if (slice_weave_failed(ch, FIGHTING(ch), tmpmana, spell_num))
              temp(ch,FIGHTING(ch),NULL,NULL,NULL,-1,-1,tmpmana,subcmd,mod);
        }
        if (GET_MANA(ch) < 0)
          check_mana(ch);
        return;
      }
      send_to_char("Weave what on who?\r\n",ch);
      return;
    }
    /* victim found, now some basic sanity-tests! */
    if (IS_SET(spell_info[spell_num].targets,IS_AGGRESSIVE) && !ok_damage_shopkeeper(ch,victim)) {
      return;
    }
    if ((victim == ch) && IS_SET(spell_info[spell_num].targets,IS_AGGRESSIVE)) {
      send_to_char("Nooo! Your mother would be SO sad!\r\n",ch);
      return;
    }
    if ((victim == ch) && IS_SET(spell_info[spell_num].targets,NOT_SELF)) {
      send_to_char("You can't weave this on yourself bonehead!\r\n",ch);
      return;
    }
    if ((GET_LEVEL(ch) < LVL_IMMORT) && (GET_LEVEL(victim) >= LVL_IMMORT)) {
      send_to_char("Nope, it's bad policy to weave thingys on Imms!\r\n",ch);
      return;
    }
    if (MOB_FLAGGED(victim, MOB_NOMAGIC)) {
      act("$N is immune to weaves!", TRUE, ch, 0, victim, TO_CHAR);
      return;
    }

    if (IS_GHOLAM(victim) && IS_SET(PRF_FLAGS(ch),PRF_GRASPING)) {
      act("Your flows simply vanishes as they reach $N.",FALSE,ch,0,victim,TO_CHAR);
      act("$ns flows vanishes as they reach $N",FALSE,ch,0,victim,TO_OPUSERSS);
      act("$n looks at $N with fear.",FALSE,ch,0,victim,TO_OPUSERNSS);
      act("$n looks at $N with fear.",FALSE,ch,0,victim,TO_NOTCHANN);
      return;
    }
    tmpmana = atoi(arg2);
    tmpmana = mana_check(tmpmana,ch);
    if (slice_weave_failed(ch,victim, tmpmana, spell_num))
      temp(ch,victim,NULL,NULL,NULL,-1,-1,tmpmana,subcmd,mod);
    if (GET_MANA(ch) < 0)
      check_mana(ch);
    return;
  }

  if (IS_SET(spell_info[spell_num].targets,ROOM_TARGET)) {
    if (!temp2) {
      send_to_char("You must supply a room-name, bummer!\r\n",ch);
      return;
    }
    skip_spaces(&temp2);
    if ('\'' == *temp2) { // Advanced keyword search...
      temp2++;
      if (NULL == (p = index(temp2,'\''))) {
        send_to_char("If you're going to use advanced keyword options in weaves, you \\c01MUST\\c00 enclose the keywords within '",ch);
    return;
      }
                  // Make sure keuwords ends up in arg1, and rest in arg2! 
      *p = '\0'; p++;
      strcpy(arg1,temp2);
      strcpy(arg2,p);
    } else {
      two_arguments(temp2,arg1,arg2);
    }
    if (!(room_num = find_room_by_name(arg1))) {
      send_to_char("You must supply a room-name, bummer!\r\n",ch);
      return;
    }
    if (ROOM_FLAGGED(room_num,ROOM_NOGATE)||ROOM_FLAGGED(room_num,ROOM_NOMAGIC)) {
      send_to_char("Sorry, this isn't allowed in that room!\r\n",ch);
      return;
    }
    
    tmpmana = ((arg2) ? atoi(arg2) : 0);
    tmpmana = mana_check(tmpmana,ch);
  temp(ch,NULL,NULL,&world[room_num],NULL,-1,-1,tmpmana,subcmd,mod);
    if (GET_MANA(ch) < 0)
      check_mana(ch);
    return;
  }
  if (IS_SET(spell_info[spell_num].targets,CHAR_ROOM)) {
    if (!temp2)
      temp(ch,NULL,NULL,NULL,NULL,-1,-1,0,subcmd,mod);
    else {
      one_argument(temp2,arg1);
      tmpmana = ((arg1) ? atoi(arg1) : 0);
      tmpmana = mana_check(tmpmana,ch);
      temp(ch,NULL,NULL,NULL,NULL,-1,-1,tmpmana,subcmd,mod);
      if (GET_MANA(ch) < 0)
        check_mana(ch);
    }
    return;
  }
  if (IS_SET(spell_info[spell_num].targets,SPELL_TARGET)) {
    if (!temp2) {
      send_to_char("You must send a weavename as an argument!\r\n",ch);
      return;
    }
    temp3 = strtok(temp2,"'");
    if (temp3 == NULL) {
      send_to_char("You must send a weavename as an argument!\r\n",ch);
      return;
    }
    temp3 = strtok(temp2,"'");
    if (temp3 == NULL) {
      send_to_char("Weave names must be enclosed in the symbols chosen by the Creator: '\r\n",ch);
      return;
    }
    temp4 = strtok(NULL,'\0');
    num2 = find_skill_num(temp3);
    if ((num2 < 1) || (num2 > MAX_SPELLS)) {
      send_to_char("Hah! Very funny, NOT!\r\n",ch);
      return;
    }
    if (!GET_SKILL(ch,num2)) {
      send_to_char("You don't know that weave!\r\n",ch);
      return;
    }
    if (spell_list[num2] == NULL) {
      send_to_char("Sorry, that weave isn't implemented yet.\r\n",ch);
      return;
    }
    if (!temp4)
      temp(ch,NULL,NULL,NULL,NULL,-1,num2,0,subcmd,mod);
    else {
      tmpmana = atoi(temp4);
      tmpmana = mana_check(tmpmana,ch);
      temp(ch,NULL,NULL,NULL,NULL,-1,num2,tmpmana,subcmd,mod);
    }
    return;
  }
  if (IS_SET(spell_info[spell_num].targets,DOOR_TARGET)) {
    if (!temp2) {
      send_to_char("You must supply a direction for this weave!\r\n",ch);
      return;
    }
    two_arguments(temp2,arg1,arg2);
    obj = NULL;
    door = search_block(arg1,(const char **)dirs,FALSE);
    rc = generic_find(arg1,FIND_OBJ_INV|FIND_OBJ_ROOM|FIND_OBJ_EQUIP, ch, &victim, &obj);

    if (door == -1 && !rc) {
      send_to_char("That is neither a direction, nor a container, smeghead!\r\n",ch);
      return;
    }

    tmpmana = atoi(arg2);
    tmpmana = mana_check(tmpmana,ch);
    temp(ch,NULL,obj,&world[ch->in_room],NULL,door,-1,tmpmana,subcmd,mod);

    if (GET_MANA(ch) < 0)
      check_mana(ch);
    return;
  }
  if (IS_SET(spell_info[spell_num].targets,OBJ_TARGET)) {
    if (!temp2) { 
      send_to_char("You must supply a name of the object as well.\r\n", ch);
      return;
    }
    two_arguments(temp2, arg1, arg2);
    if (!(obj = get_obj_in_list_vis(ch, arg1, ch->carrying))) {
      send_to_char("There is no such thing in your inventory.\r\n", ch);
      return;
    }
    tmpmana = ((arg2) ? atoi(arg2) : 0);
    tmpmana = mana_check(tmpmana,ch);
    temp(ch, NULL, obj, NULL, NULL, 0, 0,tmpmana, subcmd, mod);
    return; 
  }
  send_to_char("This spell has not been properly initiated, please inform an Imm.\r\n",ch);
  return;
}

void spell_level(int spell, int class, int air, int earth, int fire, int spirit, int water,int cost,int mana_apply,byte type,int target_type)
{
  char buf[256];
  int bad = 0;

  if (spell < 0 || spell > TOP_SPELL_DEFINE) {
    sprintf(buf, "SYSERR: attempting assign to illegal spellnum %d", spell);
    mlog(buf);
    return;
  }

  if (class < 0 || class >= NUM_CLASSES) {
    sprintf(buf, "SYSERR: assigning '%s' to illegal class %d",
        skill_name(spell), class);
    mlog(buf);
    bad = 1;
  }

  if (!bad) {
    spell_info[spell].min_level[class][AIR]     = air;
    spell_info[spell].min_level[class][EARTH]   = earth;
    spell_info[spell].min_level[class][FIRE]    = fire;
    spell_info[spell].min_level[class][SPIRIT]  = spirit;
    spell_info[spell].min_level[class][WATER]   = water;
    spell_info[spell].cost[class] 		= cost;
    spell_info[spell].mana 	    		= mana_apply;
    spell_info[spell].mana_add			= mana_apply;
    spell_info[spell].type        		= type;
    spell_info[spell].targets     		= target_type;
  }



}


/* Assign the spells on boot up */
void spello(int spl, int max_mana, int min_mana, int mana_change, int minpos,
          int targets, int violent, int routines)
{
  int i,j;

  for (i = 0; i < NUM_CLASSES; i++)
    for(j = 0; j < POWERS; j++)
      spell_info[spl].min_level[i][j] = MAX_POWER;
  spell_info[spl].mana = max_mana;
  spell_info[spl].min_position = minpos;
  spell_info[spl].targets = targets;
  spell_info[spl].violent = violent;
  spell_info[spl].routines = routines;
}


void unused_spell(int spl)
{
  int i,j;

  for (i = 0; i < NUM_CLASSES; i++)
    for (j = 0; j < POWERS; j++)
      spell_info[spl].min_level[i][j] = LVL_IMPL + 1;
  spell_info[spl].mana = 0;
  spell_info[spl].min_position = 0;
  spell_info[spl].targets = 0;
  spell_info[spl].violent = 0;
  spell_info[spl].routines = 0;
}

#define skillo(skill) spello(skill, 0, 0, 0, 0, 0, 0, 0);


/*
 * Arguments for spello calls:
 *
 * spellnum, maxmana, minmana, manachng, minpos, targets, violent?, routines.
 *
 * spellnum:  Number of the spell.  Usually the symbolic name as defined in
 * spells.h (such as SPELL_HEAL).
 *
 * maxmana :  The maximum mana this spell will take (i.e., the mana it
 * will take when the player first gets the spell).
 *
 * minmana :  The minimum mana this spell will take, no matter how high
 * level the caster is.
 *
 * manachng:  The change in mana for the spell from level to level.  This
 * number should be positive, but represents the reduction in mana cost as
 * the caster's level increases.
 *
 * minpos  :  Minimum position the caster must be in for the spell to work
 * (usually fighting or standing). targets :  A "list" of the valid targets
 * for the spell, joined with bitwise OR ('|').
 *
 * violent :  TRUE or FALSE, depending on if this is considered a violent
 * spell and should not be cast in PEACEFUL rooms or on yourself.  Should be
 * set on any spell that inflicts damage, is considered aggressive (i.e.
 * charm, curse), or is otherwise nasty.
 *
 * routines:  A list of magic routines which are associated with this spell
 * if the spell uses spell templates.  Also joined with bitwise OR ('|').
 *
 * See the CircleMUD documentation for a more detailed description of these
 * fields.
 */

/*
 * NOTE: SPELL LEVELS ARE NO LONGER ASSIGNED HERE AS OF Circle 3.0 bpl9.
 * In order to make this cleaner, as well as to make adding new classes
 * much easier, spell levels are now assigned in class.c.  You only need
 * a spello() call to define a new spell; to decide who gets to use a spell
 * or skill, look in class.c.  -JE 5 Feb 1996
 */

void mag_assign_spells(void)
{
  int i;

  /* Do not change the loop below */
  for (i = 1; i <= TOP_SPELL_DEFINE; i++)
    unused_spell(i);


  /*
   * Declaration of skills - this actually doesn't do anything except
   * set it up so that immortals can use these skills by default.  The
   * min level to use the skill for other classes is set up in class.c.
   */

  skillo(SKILL_BACKSTAB);
  skillo(SKILL_BASH);
  skillo(SKILL_HIDE);
  skillo(SKILL_KICK);
  skillo(SKILL_PICK_LOCK);
  skillo(SKILL_PUNCH);
  skillo(SKILL_RESCUE);
  skillo(SKILL_SNEAK);
  skillo(SKILL_STEAL);
  skillo(SKILL_TRACK);
  skillo(SKILL_SEEK);
}

