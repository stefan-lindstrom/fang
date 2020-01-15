/******************************************************************************
 * $Id: class.c,v 1.9 2003/12/02 08:15:36 fang Exp $
 *   
 * File: class.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: class.c,v $
 *   Revision 1.9  2003/12/02 08:15:36  fang
 *   Cappen: Added mycolor to items displayed by the toggle command
 *
 *   Revision 1.8  2003/06/11 03:08:25  fang
 *   Added "wrath" weave.
 *
 *   Revision 1.7  2002/11/07 04:02:01  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.6  2002/10/08 18:23:07  fang
 *   fischer: removed the spark for wolfkin
 *
 *   Revision 1.5  2002/09/22 21:43:49  fischer
 *   Removing mounts and horsetrain from the code
 *
 *   Revision 1.4  2002/09/15 00:50:07  fang
 *   Drath: XLOG update - Categories added to xlog display automatically
 *
 *   Revision 1.3  2002/07/31 04:34:50  fang
 *   Drath: Update to level 2 message
 *
 *   Revision 1.2  2002/07/28 13:56:03  fang
 *   Removed dmlloc...just to test.
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.6  2002/07/16 15:16:16  fang
 *   Drath: Seafolk Guildskills are done.
 *
 *   Revision 1.5  2002/07/06 07:05:12  fang
 *   Drath: IMMLOCK for Fred and 2nd to last XLOG commit
 *
 *   Revision 1.4  2002/05/31 08:57:33  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/05/27 10:52:48  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.48  2002/02/22 14:54:43  mud
 *   *** empty log message ***
 *
 *   Revision 1.47  2002/02/14 13:41:56  mud
 *   *** empty log message ***
 *
 *   Revision 1.46  2002/01/24 21:33:13  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.45.2.1  2002/01/21 01:23:40  mud
 *   Code cleanup. Aiel clans/socities gone. Lot of old guild code gone.
 *
 *   Revision 1.45  2001/11/07 09:21:11  mud
 *
 *
 *   Added SliceWeave as an RP-weave to all non-channie classes so that
 *   those classes, when sparked, can be instructed in the skill.
 *
 *   Revision 1.44  2001/11/07 08:49:33  mud
 *   Titles in who list should close colour even if the titles owner have forgotten.
 *
 *   Revision 1.42  2001/10/04 18:09:07  mud
 *   Added General nobility title.
 *
 *   Revision 1.40  2001/07/31 22:30:26  mud
 *   Appraise made a skill available to all classes. - Illaria
 *
 *   Revision 1.39  2001/07/31 09:29:29  mud
 *   Changed mana and cost on sedaibond and ashabond. -Illaria
 *
 *   Revision 1.38  2001/07/27 14:42:55  mud
 *
 *   changed cost for sedai and ashabonds
 *
 *   Revision 1.37  2001/07/18 17:09:10  mud
 *
 *   bond becomes spells ashabond and sedaibond
 *
 *   Revision 1.33  2000/11/07 12:00:39  mud
 *   Finally. XML Mobiles are done.
 *
 *   Revision 1.31  2000/07/29 11:58:44  mud
 *   added a message in advance_level, telling level2's to got to the trainer
 *
 *   Revision 1.30  2000/07/06 19:14:19  mud
 *   Repair fix.
 *
 *   Revision 1.29  2000/07/06 08:47:42  mud
 *   Changed craft and repair for craftsman class.
 *
 *   Revision 1.28  2000/06/27 05:46:14  mud
 *   Dragonsworn ranks.
 *
 *   Revision 1.27  2000/06/24 06:11:48  mud
 *   Changes to Aiel guild ranks and subranks
 *
 *   Revision 1.23  2000/05/26 09:20:32  mud
 *   Fixed findherb, added a herbident ACMD, IMM commands level fixes.
 *
 *   Revision 1.22  2000/05/25 15:06:48  mud
 *
 *   Talent code changed to utilize char* arrays instead of a million #defines.
 *   Changes made to do_set, do_stat, do_score.
 *   Imm command Prove added.
 *
 *   Revision 1.21  2000/05/24 08:41:12  mud
 *   Aiel endurance changes
 *
 *   Revision 1.20  2000/05/23 22:59:47  mud
 *   Asha'man rank change.
 *
 *   Revision 1.19  2000/05/08 15:20:35  mud
 *   *** empty log message ***
 *
 *   Revision 1.18  2000/05/04 16:21:41  mud
 *   *** empty log message ***
 *
 *   Revision 1.17  2000/04/28 17:09:24  mud
 *   Modified ranks and subranks for Seanchan added new ranks for Tuatha'an
 *
 *   Revision 1.16  2000/04/28 08:48:17  mud
 *   Forsaken rank/subrank
 *
 *   Revision 1.15  2000/04/27 05:17:12  mud
 *   *** empty log message ***
 *
 *   Revision 1.14  2000/04/16 17:48:52  mud
 *   Added "const" to three string arrays, for constistency's sake.
 *
 *   Revision 1.3  2000/03/07 20:31:50  mud
 *   Updated to add new ranks to Band of the Red Hand
 *
 *   Revision 1.2  2000/02/21 14:00:16  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* ************************************************************************
*   File: class.c                                       Part of CircleMUD *
*  Usage: Source file for class-specific code                             *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

/*
 * This file attempts to concentrate most of the code which must be changed
 * in order for new classes to be added.  If you're adding a new class,
 * you should go through this entire file from beginning to end and add
 * the appropriate new special cases for your new class.
 */
#include "class.h"
#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "db.h"
#include "utils.h"
#include "spells.h"
#include "interpreter.h"
#include "comm.h"
#include "skimming.h"

void channeler_setup(void);
void wilder_setup(void);
void soldier_setup(void);
void adventurer_setup(void);
void craftsman_setup(void);
void thief_setup(void);
void wisdom_setup(void);
void init_spells(void);
void init_questandguildskills(void);

/* List of all the implemented spells */
BPTR(spell_list[MAX_SPELLS]) = {
  NULL,	
  armor,
  airsword,
  gate,
  blindness,
  invisibility,
  call_lightning,
  firesword,  wrap,
  gag, 							
  shield,						/* 10 */
  NULL, /*<- Should be link or links, but something keeps fucking up */
  gentle,
  create_wat,
  heal,
  heal_gentling,
  airshield,
  burn,
  airblow,
  ice_arrows,	
  detect_magic, /* 20 */
  meteor,
  light,
  earthquake,
  rolling_ring,
  lightning,
  fireball,
  darkness,
  blindness,
  compulsion,
  NULL, /* 30 */
  NULL,NULL,
  sanctuary,
  sleeper,
  NULL,
  infravision,
  chill,
  earthshield,
  NULL, /* 39 */
  NULL,NULL,
  pain,
  NULL,
  NULL,
  create_food,
  NULL,
  whirlwind,
  delve,
  minor_healing,
  dome_of_air,
  ball_of_light,
  disintegrate,
  ball_of_air,
  open_lock,
  call_fire,
  shaft_of_fire,
  dome_of_fire,
  NULL,
  destroy_weapon,
  drown, /* 60 */
  poison,
  invigorate,
  storm,
  dart_of_air,
  call_frost,
  shaft_of_frost,
  wind_armor,
  confusion,
  slow,
  unbalance,
  explosion,
  dome_of_frost,
  fist_of_air,
  firestorm,
  refresh,
  earth_armor,
  destroy_armor,
  fire_arrows,
  earth_missile,
  kinheal,
  shield_of_saidin,
  cripple,
  deaf,
  heal_II,
  thunderburst, /* 85 */
  stormshield,
  group_restore,
  ward_human,
  identify,
  fog,
  entangle,
  mend,
  frostbite,
  roomseal,
  targate,
  skim,
  sedaibond,
  ashabond,
  watershield,
  stormsfury,	/* 100 */
  wrath,
  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL	/* 200 */
};

/* costs */
struct cost da_costs[3] = {
   {18, 1 },
   {25, 2 },
   {30, 3 },
};

/* The standard AFW-text */

char *std_afw = "Sorry, but this person is AFW!\r\nTry again later.\r\n";

/* The standard AFK-text */

char *std_Flag = "Sorry, but this person is AFK!\r\nTry again later.\r\n";

/* Names first */

#define STD_HOME_ROOM 10228

sh_int home_rooms[] = {
  STD_HOME_ROOM,
  STD_HOME_ROOM,
  STD_HOME_ROOM,
  STD_HOME_ROOM,
  STD_HOME_ROOM,
  STD_HOME_ROOM,
  STD_HOME_ROOM,
  26113,		/* Illian */
  STD_HOME_ROOM,
  9614,			/* Seanchan */
  STD_HOME_ROOM,
  STD_HOME_ROOM,
  STD_HOME_ROOM,
  STD_HOME_ROOM,
  STD_HOME_ROOM,
  STD_HOME_ROOM,
  STD_HOME_ROOM,	/* Cour'souvra */
  -1			/* Indicates end of home_rooms! */
};

/* 
 * The list below is the one that determines what rooms save their
 * contents to file, and reload them at reboot.
 * The objects in the rooms are saved once every five minutes, in
 * the directory /lib/plrobjs/A-E/, with name depending on virtual
 * number of the room...."10184.objs" and so forth.
 * The actual saving is done in limits.c, and the loading is done in
 * db.c, at boot time.
 */

int rooms_that_save_objs[] = {
  10184, // Donation room.
  -1  // this line must be here in the end of the list
};

/* Definitions needed to make the cart riding work */
/* Add back in stop 1602 when wld file is back. */
struct cart_data cartzones[] =
{
  {			
    3399,   /* Player must be between room 3207 and 3399 for this */
    3207,   /* list to be activated */
    {
      30562, 3248, 4185, 10699, 3236, 17148, 31000, 2000, 15106,
      29050, 14001,1602,15517,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0
    }
  },
  {
    15518,
    15516, /* Temporary taxi-stop for Elaida. */
    {
      30562, 4185, 3393, 10699, 17148, 3301, 0, 0 ,0 ,0 ,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }
  },
  {
    14199,
    14000,
    {
      14091,14093,14138,14162,14166,14170,14096,14036,14044,14145,
      14023,14021,14052,14174,14068,14017,14013,14006,14002,14038,
      0,0,0,0,0,0,0,0,0,0
    }
  },
  {
    17199,
    17100,
    {
      31000,30562,10688,3207,17399,17281,17114,17057,17234,17218,
      17130,15517,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0
    }
  },
  {
    25099,
    25000,
    {
      25040,25115,25113,25006,30564,25148,3200,17148,10186,10669,
      30683,15517,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0
    }
  },
  {
    31137,
    31137,
     {
       26119,28084,25040,17286,15106,14002,3300,3200,10699,15517,
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0

     }
  },
  {
    0,
    0,
    {
      0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0
    }
  }
};

const char *hometown[] = {
  "Three-fold land",
  "Almoth",
  "Amadicia",
  "Andor",
  "Arad Doman",
  "Borderlands",
  "Cairhien",
  "Illian",
  "Murandy",
  "Seanchan",
  "Tarabon",
  "Tar Valon",
  "Tear",
  "Tremalking",
  "Two Rivers",
  "Ogier",	/* Ogier-race, can ONLY be set by Imms! */
  "Altara",
  "Saldaea",
  "Mayene",
  "\n"
};

/* Below are the racial modifiers on the stats  */
/* Str, Con, Dex, Int, wis,Cha, Move, Mana and Hp   */
const int modifiers[MAX_RACES][MAX_ABILITIES] = {
  {1,1,2,-2,0,-2,0,0,0},
  {0,0,1,-1,-1,1,-10,0,10},
  {0,0,0,1,1,0,-10,0,-10},
  {0,0,1,0,-1,0,0,0,0},
  {-1,-1,1,0,0,0,0,10,0},
  {2,2,0,-1,-2,-2,0,0,10},
  {-1,-1,-1,2,1,0,0,0,0},
  {-1,-2,0,0,2,0,10,0,0},
  {-1,-1,-1,1,2,1,-10,0,0},
  {-1,0,1,0,-1,1,0,0,0},
  {0,0,1,0,1,-1,0,-10,0},
  {-1,-1,0,1,1,0,0,0,0},
  {2,1,-1,0,-1,-1,0,0,0},
  {0,-1,1,-1,1,1,-10,0,0},
  {1,1,0,-1,0,-1,0,0,0},
  {0,0,0,0,0,0,0,0,0},
  {-1,-1,-1,2,2,-1,-10,20,-10},
  {0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0}

};

const char *class_abbrevs[] = {
  "Adv",
  "Sol",
  "Cra",
  "Thi",
  "Wis",
  "Wil",
  "Cha",
  "\n"
};

int exp_needed_per_level[NUM_CLASSES] =
{
 2500,
 2500,
 2300,
 2200,
 2800,
 2900,
 3000
};

int classStr2Num(const char *name) 
{
  register int i = 0; 

  if (NULL == name)
    return i;

  for (; *pc_class_types[i] != '\n'; i++) {
    if (!strcasecmp(name,pc_class_types[i]))
      return i;
  }

  return 0;
}

const char *pc_class_types[NUM_CLASSES+1] = {
  "Adventurer",
  "Soldier",
  "Craftsman",
  "Thief",
  "Wisdom",
  "Wilder",
  "Channeler",
  "\n"
};

const char *wkrank[] = 
{
   "Naught",
   "&RHunted&n",
   "&rShunned&n",
   "&KTwisted&n",
   "&YLost&n",
   "&wConfused&n",
   "&Ga cub&n",
   "&gYoung&n",
   "&bBefriended&n",
   "&BAccepted&n",
   "&cTrusted&n",
   "&CDevoted&n",
   "&yRespected&n",
   "&WHonored&n",
   "Inactive",
   "\n"
};

const char *blademastery[] =
{
   "Naught",
   "Unknown",
   "Prodigy",
   "Respected",
   "Veteran",
   "Blademaster",
   "Book Character",
   "\n"
};
  
const char *ajahs[] = {
   "Blue",
   "Brown",
   "Gray",
   "Green",
   "Red",
   "White",
   "Yellow",
   "NO MORE AJAHS"
};

const char *ajah_colors[] = {
   "&b",
   "&y",
   "&n",
   "&g",
   "&r",
   "&W",
   "&Y",
   "\n"
};


/*
 * The code to interpret three letters -- used in interpreter.c when a
 * new character is selecting a class and by 'set class' in act.wizard.c.
 */

int parse_class(char *arg)
{
  int i=0;

  if ((!*arg)||(strlen(arg) < 3))
    return CLASS_UNDEFINED;
  while ((i < NUM_CLASSES) && (strncasecmp(arg,pc_class_types[i],3)))
    i++;
  if (strncasecmp(arg, pc_class_types[i],3))
    return CLASS_UNDEFINED;
  else
    return i;
}

#define SPELL	0
#define SKILL	1


/* Some initializations for characters, including initial skills */
void do_start(struct char_data * ch)
{
   SET_BIT(PRF_FLAGS(ch),  PRF_AUTOEXIT);
   SET_BIT(PRF_FLAGS(ch),  PRF_NOSPAM);
   SET_BIT(PRF_FLAGS(ch),  PRF_IC);
   SET_BIT(PRF2_FLAGS(ch), PRF2_NOOBSCENE);
   SET_BIT(AUTO_FLAGS(ch), AUTO_LOOT);
   SET_BIT(AUTO_FLAGS(ch), AUTO_GOLD);
   SET_BIT(AUTO_FLAGS(ch), AUTO_JUNK);
   SET_BIT(AUTO_FLAGS(ch), AUTO_SPLIT);
   send_to_char("Autoexit, Autoloot, Autogold, Autojunk, Autosplit all set.\r\n", ch);

   GET_AUTHORIZED(ch)=0;
   (void)strcpy(GET_MYCOLOR(ch), "&n");

  if ((GET_CLASS(ch) == CLASS_CHANNELER)||(GET_CLASS(ch) == CLASS_WILDER))
  {
    if (GET_CLASS(ch) == CLASS_CHANNELER)
      GET_SPARK(ch) = FALSE;             /* Safeguard against double */
    else                                 /* abilities being set at   */
      GET_TEACHABLE(ch) = FALSE;         /* creation                 */
    GET_AIR(ch) = 1;
    GET_EARTH(ch) = 1;
    GET_FIRE(ch) = 1;
    GET_SPIRIT(ch) = 1;
    GET_WATER(ch) = 1;
    if (GET_SEX(ch) == SEX_MALE)
      GET_TAINT(ch) = TRUE;
  }
  GET_LEVEL(ch) = 0;
  GET_EXP(ch) = 0;

  set_title(ch, NULL);

  //  commented out because FANG's current policy is to not have "sparked" 
  //  wolfkin but left in incase some other mud wants to allow it
//  if (number(1,100)<=3 && cant_channel(ch)) // Makse sure only !CHANNIE gets wkin!
//  {
//    GET_WOLFKIN(ch) = TRUE;
//  }

  if (!(GET_CLASS(ch) == CLASS_CHANNELER)&&!(GET_CLASS(ch) == CLASS_WILDER)) {
    if (number(1,100)<=3) {
      GET_SPARK(ch) = TRUE;
      GET_AIR(ch) = GET_FIRE(ch) = GET_SPIRIT(ch) = GET_WATER(ch) = GET_EARTH(ch) = 1;
    }
    else if (number(1,100)<=3) {
      GET_TEACHABLE(ch) = TRUE;
      GET_AIR(ch) = GET_FIRE(ch) = GET_SPIRIT(ch) = GET_WATER(ch) = GET_EARTH(ch) = 1;
    }
  }

  if (number(1,100)<=3) {
      ch->player_specials->saved.taveren = TRUE;
  }

  TAUGHT(ch)    = 0; /* Reset number of lessons received in the OP */
  HPTAUGHT(ch)  = 0; /* Reset number of HP lessons received */
  GET_POWER(ch) = gaussian_var(-28,18,-80,30); /*number(-80,30);	*/
  switch (GET_CLASS(ch)) {
  case CLASS_THIEF:
    SET_SKILL(ch, SKILL_SNEAK   , 1);
    SET_SKILL(ch, SKILL_BACKSTAB, 1);
    SET_SKILL(ch, SKILL_STEAL   , 1);
    break;
  case CLASS_CHANNELER:
    SET_SKILL(ch,SKILL_GRASP        ,1);
    SET_SKILL(ch,SKILL_RELEASE      ,1);
    SET_SKILL(ch,SPELL_EARTH_MISSILE,1);
    GET_POWER(ch) = gaussian_var(-20,18,-80,50); /*number(-80,50);*/
    break;
  case CLASS_WILDER:
    SET_SKILL(ch,SKILL_GRASP  ,1);
    SET_SKILL(ch,SKILL_RELEASE,1);
    SET_SKILL(ch,SPELL_ARMOR  ,1);
    GET_POWER(ch) = gaussian_var(15,18,-50,80); /* number(-50,80); */
    break;
  case CLASS_SOLDIER:
    SET_SKILL(ch,SKILL_DODGE,1);
    SET_SKILL(ch,SKILL_BASH ,1);
    SET_SKILL(ch,SKILL_PARRY,1);
    break;
  case CLASS_ADVENTURER:
    SET_SKILL(ch,SKILL_KICK  ,1);
    SET_SKILL(ch,SKILL_DODGE ,1);
    SET_SKILL(ch,SKILL_BASH  ,1);
    break;
  case CLASS_WISDOM:
    SET_SKILL(ch,SKILL_FINDHERB,1);
    SET_SKILL(ch,SKILL_HERBLORE,1);
    SET_SKILL(ch,SKILL_HERBIDENT,1);
    break;
  case CLASS_CRAFTSMAN:
    SET_SKILL(ch,SKILL_TOOL,1);
    break;
  }
  GET_PRACTICES(ch) = 0;
  

  advance_level(ch);

  GET_HIT(ch) = GET_MAX_HIT(ch);
  GET_MANA(ch) = GET_MAX_MANA(ch);
  GET_MOVE(ch) = GET_MAX_MOVE(ch);
  GET_FRACT_MOVE(ch) = 0;

  GET_COND(ch, THIRST) = 24;
  GET_COND(ch, FULL) = 24;
  GET_COND(ch, DRUNK) = 0;

  ch->player.time.played = 0;
  ch->player.time.logon = time(0);
}



/*
 * This function controls the change to maxmove, maxmana, and maxhp for
 * each class every time they gain a level.
 */

void advance_level(struct char_data * ch)
{
  int add_hp = 0, add_mana = 0, add_move = 0, i,extra;
  int oldtrains = GET_TRAINS(ch),oldpracs = GET_PRACTICES(ch);
   char tmp[2000];

  /* Wont get anything up at those levels. */
  if (GET_LEVEL(ch) >= (LVL_IMMORT - 1)) {
    GET_EXP(ch) = (GET_LEVEL(ch)-1)*GET_LEVEL_EXP(ch);
    return;
  }
  add_hp = number(5, 14) + MAX(0, (int) ((GET_CON(ch)-13)/4));
  add_mana = number(5, 14) + MAX(0, (int) ((GET_WIS(ch)-13)/4));
  add_move = number(3, 5) + MAX(0, (int) ((GET_DEX(ch)-13)/4));

  GET_PRACTICES(ch) += number(3,5) + MAX(0, (int) (GET_WIS(ch)+GET_INT(ch)-32)/4);
  
  extra = 1;
  switch (GET_CLASS(ch))
  {
  case CLASS_SOLDIER:
  case CLASS_ADVENTURER:
    if (cant_channel(ch)) { 
      add_hp += (add_mana / 4) + 14;
      extra = 15;
      add_mana = 0;
    } else {
      add_hp += 14;
      add_mana += 3;
      extra = 0;
    }
    break;
  case CLASS_THIEF:
  case CLASS_CRAFTSMAN:
  case CLASS_WISDOM: 
    if (cant_channel(ch)) {
      add_hp += (add_mana / 4) + 10;
      add_mana = 0;
      extra = 20;
    } else {
      add_hp += 10;
      add_mana += 5;
      extra = 0;
    }
    break;
  case CLASS_CHANNELER:
    add_hp += 4;
    add_mana +=11;
    break;
  case CLASS_WILDER:
    add_hp += 4;
    add_mana +=15;
    break;
  }

  if (extra && number(1,100)< (2*extra)) {
    GET_PRACTICES(ch) += 2;
  } 
  if (GET_LEVEL(ch) >= LVL_IMMORT)
  {
    for (i = 0; i < 3; i++)
      GET_COND(ch, i) = (char) -1;
    SET_BIT(PRF_FLAGS(ch), PRF_HOLYLIGHT);
  }

  GET_MAX_HIT(ch) += add_hp;
  GET_MAX_MANA(ch) += add_mana;
  GET_MAX_MOVE(ch) += add_move;

  if (GET_TRAINS(ch) != (ubyte)0xFF) {
    GET_TRAINS(ch)++;

  if ((GET_TRAINS(ch) != (ubyte)0xFF) && number(1,100) <= (25+extra))
      GET_TRAINS(ch)++;
  } else {
    send_to_char("I'm afraid you have too many trains, if another one is added"
                 " you will loose all trains you currently have!\r\n",ch);
  } 

  if (GET_WOLFKIN(ch) && (GET_LEVEL(ch) >= 79) && !GET_SKILL(ch,SKILL_DREAM)) {
    send_to_char("Congratulations. You may now enter the Wolfdream. To do so\r\n",ch);
    send_to_char("go to sleep, and then type dream.\r\n",ch);
    SET_SKILL(ch,SKILL_DREAM,70);
    SET_SKILL(ch,SKILL_DREAMLIST,70);
  }

  if ((!GET_LEVEL(ch)%10)&&!GET_SPARK(ch)&&!GET_TEACHABLE(ch)&&!GET_CLASS(ch)==CLASS_WILDER&&
        !GET_CLASS(ch)==CLASS_CHANNELER&&(number(1,100)<=3))
  {
    send_to_char("You turn violently ill for a short time, then get better.\r\n", ch);
    send_to_char("You feel strange...very strange.\r\n", ch);
    send_to_char("Something has changed...what can it be?\r\n", ch);

    GET_AIR(ch) = 1;
    GET_EARTH(ch) = 1;
    GET_FIRE(ch) = 1;
    GET_SPIRIT(ch) = 1;
    GET_WATER(ch) = 1;
    GET_TEACHABLE(ch) = TRUE;		/* better if you're teachable than suddenly gettin' the Spark */
    if (GET_SEX(ch) == SEX_MALE)
      GET_TAINT(ch) = TRUE;
  }
  
  GET_LEVEL(ch)++;
  if ((ch->in_room) && (ch->in_room != 1) && (ch->in_room != NOWHERE))
    save_char(ch, ch->in_room);
  else
    save_char(ch, NOWHERE);

  sprintf(tmp,"You've gained %d HP, %d moves and %d mana.\r\n",add_hp,add_move,add_mana);
  sprintf(tmp,"%sYou've also gained %d trains and %ld practices.\r\n",tmp,(GET_TRAINS(ch) - oldtrains),(GET_PRACTICES(ch) - oldpracs));

  if (GET_LEVEL(ch) == 2) 
    sprintf(tmp, "%s&CCongratulations!! Now you should consider using those trains and practices at a Trainer to up your stats and gain new skills!&n\r\n&CAlso, if you're not a Channeler or Wilder class character, or a sparked channeler, you can also use the MANACONV command.&n\r\n", tmp);

  send_to_char(tmp,ch);


  sprintf(buf, "%s advanced to level %ld", GET_NAME(ch), GET_LEVEL(ch));
  xlog(SYS_PLEVEL, LVL_IMMORT, buf, TRUE);
}


/*
 * This simply calculates the backstab multiplier based on a character's
 * level.  This used to be an array, but was changed to be a function so
 * that it would be easier to add more levels to your MUD.  This doesn't
 * really create a big performance hit because it's not used very often.
 */
int backstab_mult(int level)
{
  if (level <= 0)
    return 1;	  /* level 0 */
  else if (level <= 15)
    return 2;	  /* level 1 - 10 */
  else if (level <= 30)
    return 3;	  /* level 11 - 20 */
  else if (level <= 60)
    return 4;	  /* level 21 - 40 */
  else if (level <= 90)
    return 5;	  /* level 41 - 60 */
  else if (level <= 120)
    return 6;	  /* level 61 - 80 */
  else if (level <= 150)
    return 7;	  /* level 81 - 100 */
  else if (level <= 165)
    return 8;	  /* level 101 - 120 */
  else if (level <= 180)
    return 9;	  /* level 121 - 140 */
  else if (level < LVL_IMMORT)
    return 10;	  /* all remaining mortal levels */
  else
    return 20;	  /* immortals */
}

/*
 * SPELLS AND SKILLS.  This area defines which spells are assigned to
 * which classes, and the minimum level the character must be to use
 * the spell or skill.
 */

void init_spell_levels(void)
{
  int i,j;
  for (i = 0; i < MAX_SKILLS; i++)
    for (j = 0; j < NUM_CLASSES; j ++)
      spell_level(i, j , 200, -1, -1, -1, -1, -1, 50,SKILL_GUILD,0);

  channeler_setup();
  wilder_setup();
  soldier_setup();
  adventurer_setup();
  craftsman_setup();
  thief_setup();
  wisdom_setup();
  init_spells();       		/* This one is independent of classes */
  init_questandguildskills();   /* As is this.			     */
};

void init_questandguildskills()
{
  int i , j;
  int guildskills[] =
{
 SKILL_AIM,		SKILL_ARREST,		SKILL_BANDHEAL,
 SKILL_BANDMARCH,	SKILL_BRACE,		SKILL_BRAND,
 SKILL_CAMOUFLAGE,	SKILL_CRITICAL_HIT,	SKILL_DARKGIFT,
 SKILL_DISENGAGE,	SKILL_DISGUISE,		SKILL_DREAM,
 SKILL_ENHANCED_SPELL,	SKILL_FINDHERB,		SKILL_FINE,
 SKILL_FLANK,		SKILL_FX,		SKILL_HANDTALK,
 SKILL_HASTE,		SKILL_HERBMASTERY,	SKILL_HUNT,
 SKILL_INFRAVISION,	SKILL_INSPIRE,		SKILL_INTIMIDATE,
 SKILL_KNOWHEALTH,	SKILL_KNOWLEDGE,	SKILL_LEADERSHIP,
 SKILL_LIPREAD,		SKILL_MEDIATE,		SKILL_MEDICINE,
 SKILL_MORALE,		SKILL_OBSCURE,		SKILL_PATROL,
 SKILL_PEACE,		SKILL_QUICK_MIND,	SKILL_RAVAGE,
 SKILL_RECRUIT,		SKILL_REINFORCE,	
 SKILL_REPEL,		SKILL_RESPONSE,		SKILL_RETREAT,
 SKILL_RUN,		SKILL_RUMOR,		SKILL_SEARCH,
 SKILL_SEARCH,		SKILL_SEEK,		SKILL_SUMMON,
 SKILL_SEVER,		SKILL_STAB,		SKILL_STALK,
 SKILL_STRIKE,		SKILL_SURVIVAL,		SKILL_TACTICS,
 SKILL_TAIL,		SKILL_TEACH,		SKILL_TEST,
 SKILL_TRACTION,	SKILL_TRAVEL,		SKILL_TREESINGING,
 SKILL_TRUELIGHT,	SKILL_VEIL,		SKILL_WARDERCOMBAT,
 SKILL_WARDERHEAL,	SKILL_WARDERSENSE,	SKILL_VOID,
 SKILL_CONCENTRATE,	SKILL_SHIELDWALL,	SKILL_DREAMLIST,
 SKILL_DREAMPULL,	SKILL_RAGE, 	 	SKILL_CALL,
 SKILL_THROW,		SKILL_ARMSCRY, 		SKILL_MEDITATION,
 SKILL_DTALK,		SKILL_DEFENSE,		SKILL_SFX,
 SKILL_FERVOR,		SKILL_MASK, 		SKILL_FORTIFY,
 SKILL_INQUISITION,     SKILL_SHADOW,           SKILL_ARROGANCE,
 SKILL_OFFEND,          SKILL_PRODIGY,          SKILL_TRAIL,
 SKILL_TERTEST,		SKILL_TERINFO,		SKILL_HERBIDENT,
 -1
};

int questskills[] = 
{
  SKILL_MARTIAL_ARTS,	SKILL_FIFTH_ATTACK,	SKILL_BLADEMASTERY,
  SKILL_SPEARMASTERY,	SKILL_TARGET, 		SKILL_SHOOT,

  -1
};
int questcosts[] =
{
  45,			40,			45,
  45,			100,			75,

  -1
};

  for (i = 0; i < NUM_CLASSES; i++) {
    for (j = 0; guildskills[j] > 0; j++)  
      spell_level(guildskills[j], i,  0, -1, -1, -1, -1, 1, 50,SKILL_GUILD,0);
    for (j = 0; questskills[j] > 0; j++)  
      spell_level(questskills[j],  i,  0, -1, -1, -1, -1, questcosts[j], 
50,SKILL_QUEST,0);
  }
}

void init_spells()
{
  int i;
  for (i = 0; i < NUM_CLASSES; i++)
  {

/*
Just so we don't have to hunt it down
spell_level(int spell, int class, int air, int earth, int fire, int spirit, int water,int cost,int mana_apply,byte type,int target_type) 
*/
    spell_level(SPELL_AIRBLOW,	 i,  8,  0,  0,  0,  0,  1, 50,SPELL_NORMAL,CHAR_TARGET|NOT_SELF|IS_AGGRESSIVE);
    spell_level(SPELL_AIRSHIELD, 	 i, 20,  0,  0,  5,  0,  3,200,SPELL_NORMAL,NO_TARGET);
    spell_level(SPELL_AIRSWORD, 	 i, 27,  0,  0,  0,  0,  8,200,SPELL_NORMAL,NO_TARGET);
    spell_level(SPELL_ARMOR, 		 i, 10, 10,  0,  0,  0,  1,100,SPELL_NORMAL,ONLY_SELF);
    spell_level(SPELL_BALLOFAIR,	 i, 25,  0,  0, 10,  0,  2,140,SPELL_NORMAL,CHAR_TARGET|NOT_SELF|IS_AGGRESSIVE);
    spell_level(SPELL_BALLOFLIGHT,	 i, 10,  0,  10, 0,  0,  2,150,SPELL_NORMAL,NO_TARGET);
    spell_level(SPELL_BLINDNESS, 	 i, 14,  0,  0, 16,  0,  3, 50,SPELL_NORMAL,CHAR_TARGET|NOT_SELF);
    spell_level(SPELL_BURN, 		 i,  0,  0, 12,  0,  0,  1, 75,SPELL_NORMAL,CHAR_TARGET|IS_AGGRESSIVE);
    spell_level(SPELL_CALL_FIRE,	 i, 10,  0, 30, 10,  0,  200,700,SPELL_NORMAL,NO_TARGET| IS_AGGRESSIVE);
    spell_level(SPELL_CALL_FROST,	 i, 10,  0, 10, 10, 30,  200,700,SPELL_NORMAL,NO_TARGET| IS_AGGRESSIVE);
    spell_level(SPELL_CALL_LIGHTNING,i, 20,  0, 20,  0,  0,  200,500,SPELL_NORMAL,NO_TARGET|IS_AGGRESSIVE);
    spell_level(SPELL_CHILL, 		 i,  7,  0,  0,  0,  7,  1, 50,SPELL_NORMAL,CHAR_TARGET|IS_AGGRESSIVE);
    spell_level(SPELL_CONFUSION,	 i, 20, 20, 20, 23, 20,  3,200,SPELL_NORMAL,CHAR_TARGET|IS_AGGRESSIVE);
    spell_level(SPELL_CREATE_FOOD,   i,  3,  2,  0,  0,  3,  1,100,SPELL_NORMAL,NO_TARGET);
    spell_level(SPELL_CREATE_WATER,	 i,  0,  0,  0,  0,  5,  1, 50,SPELL_NORMAL,NO_TARGET);
    spell_level(SPELL_CRIPPLE,       i,  8,  9, 10, 10,  8,  2,150,SPELL_NORMAL,CHAR_TARGET|IS_AGGRESSIVE);
    spell_level(SPELL_DARKNESS, 	 i,  7,  0,  0,  0,  0,  1,150,SPELL_NORMAL,CHAR_ROOM);
    spell_level(SPELL_DARTOFAIR, 	 i,  5,  0,  0,  0,  0,  1, 50,SPELL_NORMAL,CHAR_TARGET | IS_AGGRESSIVE);
    spell_level(SPELL_DEAF,		 i,  9, 10,  5, 10, 10,  2,150,SPELL_NORMAL,CHAR_TARGET| IS_AGGRESSIVE);
    spell_level(SPELL_DELVE,         i,  3,  3,  3, 20,  3,  3,120,SPELL_NORMAL,CHAR_TARGET|NOT_SELF);
    spell_level(SPELL_DESTROY_ARMOR, i,  9, 23, 18,  0,  0,  2,200,SPELL_NORMAL,CHAR_TARGET|IS_AGGRESSIVE);
    spell_level(SPELL_DESTROY_WEAPON,i,  9, 23, 18,  0,  0,  2,200,SPELL_NORMAL,CHAR_TARGET|IS_AGGRESSIVE);
    spell_level(SPELL_DISINTEGRATE,  i,  0, 15, 15, 10,  0,  200,350,SPELL_NORMAL,CHAR_TARGET|NOT_SELF|IS_AGGRESSIVE);
    spell_level(SPELL_DOMEOFAIR,     i, 30,  0,  0, 15,  0,  4,150,SPELL_NORMAL,CHAR_ROOM);
    spell_level(SPELL_DOMEOFFIRE,    i, 20,  0, 28, 17,  0,  4,500,SPELL_NORMAL,CHAR_ROOM);
    spell_level(SPELL_DOMEOFFROST,   i, 20,  0,  0, 17, 28,  4,500,SPELL_NORMAL,CHAR_ROOM);
    spell_level(SPELL_DROWN,	     i,  5,  0,  0,  0, 35,  4,400,SPELL_NORMAL,CHAR_TARGET| IS_AGGRESSIVE);
    spell_level(SPELL_EARTH_MISSILE, i,  0,  3,  0,  2,  0,  1, 50,SPELL_NORMAL,CHAR_TARGET|IS_AGGRESSIVE);
    spell_level(SPELL_EARTHQUAKE,    i,  0, 30,  0,  0,  0,  2,500,SPELL_NORMAL,NO_TARGET);
    spell_level(SPELL_EARTHSHIELD,   i,  0, 20,  0,  5,  0,  3,200,SPELL_NORMAL,NO_TARGET);
    spell_level(SPELL_ENTANGLE,	     i,  0, 21,  5,  8, 21,  4,300,SPELL_NORMAL,CHAR_TARGET);
    spell_level(SPELL_EXPLOSION,     i,  5, 30, 40, 10, 10,200,400,SPELL_NORMAL,NO_TARGET|IS_AGGRESSIVE);
    spell_level(SPELL_FIRE_ARROWS,   i,  5,  0,  5,  0,  0,  1, 50,SPELL_NORMAL,CHAR_TARGET|IS_AGGRESSIVE);
    spell_level(SPELL_FIREBALL,      i,  3,  0, 15,  0,  0,  2,150,SPELL_NORMAL,CHAR_TARGET|IS_AGGRESSIVE);
    spell_level(SPELL_FIRESTORM,     i,  0,  0, 50,  5,  0,200,400,SPELL_NORMAL,NO_TARGET|IS_AGGRESSIVE);
    spell_level(SPELL_FIRESWORD,     i,  0,  0, 27,  0,  0,  8,200,SPELL_NORMAL,NO_TARGET);
    spell_level(SPELL_FISTOFAIR,     i,  3,  0,  0,  0,  0,  1, 50,SPELL_NORMAL,CHAR_TARGET|IS_AGGRESSIVE|NOT_SELF);
    spell_level(SPELL_FOG,	     i, 15,  0,  5,  5, 15,  3,375,SPELL_NORMAL, CHAR_ROOM);
    spell_level(SPELL_FROSTBITE,     i, 12,  0, 13,  0, 10,  4,275,SPELL_NORMAL, CHAR_ROOM);
    spell_level(SPELL_GAG,	     i, 18,  0,  0,  0,  0,  2, 50,SPELL_NORMAL,NOT_SELF|CHAR_TARGET| IS_AGGRESSIVE);
    spell_level(SPELL_GATE, 	     i, 35, 35, 35, 35, 35,200,250,SPELL_NORMAL,ROOM_TARGET);
    spell_level(SPELL_SKIMMING,      i, 35, 35, 35, 35, 35,200,250,SPELL_NORMAL,ROOM_TARGET);
    spell_level(SPELL_GENTLE, 	     i,  0,  0, 25, 25,  0,200,5000,SPELL_NORMAL,NOT_SELF|CHAR_TARGET| IS_AGGRESSIVE);
    spell_level(SPELL_HEAL, 	     i,  5,  5,  5,  5,  5,  3,450,SPELL_NORMAL,NOT_SELF|CHAR_TARGET);
    spell_level(SPELL_HEAL_II, 	     i, 25, 25, 25, 25, 25,200,800,SPELL_NORMAL,NOT_SELF|CHAR_TARGET);
    spell_level(SPELL_HEAL_GENTLING, i, 40, 40, 40, 40, 40,200,350,SPELL_NORMAL,NOT_SELF|CHAR_TARGET);
    spell_level(SPELL_ICE_ARROWS,    i,  5,  0,  0,  0, 10,  1, 50,SPELL_NORMAL,NOT_SELF|CHAR_TARGET|IS_AGGRESSIVE);
    spell_level(SPELL_INFRAVISION,   i,  0,  0,  0, 10,  0,  1, 50,SPELL_NORMAL,ONLY_SELF);
    spell_level(SPELL_INVIGORATE,    i,  8,  8,  8,  8,  8,  1, 50,SPELL_NORMAL,CHAR_TARGET|NOT_SELF);
    spell_level(SPELL_INVISIBILITY,  i, 20,  0,  0,  0,  0,200,100,SPELL_NORMAL,ONLY_SELF);
    spell_level(SPELL_LIGHT, 	     i,  2,  0,  2,  0,  0,  1, 50,SPELL_NORMAL,CHAR_ROOM);
    spell_level(SPELL_LIGHTNING,     i,  4,  0, 29,  0,  0,  3,250,SPELL_NORMAL,CHAR_TARGET|IS_AGGRESSIVE);
/*    spell_level(SPELL_LINK, 	     i,  0,  0,  0, 15,  0,  1, 50,SPELL_NORMAL,0); */
    spell_level(SPELL_MEND, 	     i, 16, 25,  0,  0,  0,  5,100,SPELL_NORMAL,OBJ_TARGET);
    spell_level(SPELL_METEOR, 	     i,  9, 20, 16,  0,  0,  5,450,SPELL_NORMAL,NO_TARGET|IS_AGGRESSIVE);
    spell_level(SPELL_MINOR_HEAL,    i,  2,  2,  2,  4,  2,  1, 20,SPELL_NORMAL,NOT_SELF|CHAR_TARGET);
    spell_level(SPELL_OPEN_LOCK,     i, 10, 10,  0,  0,  0,  2, 80,SPELL_NORMAL,DOOR_TARGET);
    spell_level(SPELL_POISON,	     i, 15, 15, 15, 15, 15,  2,200,SPELL_NORMAL,NOT_SELF|CHAR_TARGET| IS_AGGRESSIVE);
    spell_level(SPELL_REFRESH,       i,  2,  1,  2,  2,  2,  1,100,SPELL_NORMAL,NOT_SELF|CHAR_TARGET);
    spell_level(SPELL_SANCTUARY,     i, 20, 20, 20, 37, 20,  4,250,SPELL_NORMAL,CHAR_ROOM);
    spell_level(SPELL_SHAFTOFFIRE,   i,  0,  0, 20,  5,  0,  3,210,SPELL_NORMAL,NOT_SELF|CHAR_TARGET| IS_AGGRESSIVE);
    spell_level(SPELL_SHAFTOFFROST,  i,  0,  0,  0,  5, 20,  3,210,SPELL_NORMAL,NOT_SELF|CHAR_TARGET| IS_AGGRESSIVE);
    spell_level(SPELL_SHIELD, 	     i,  0,  0,  0, 19,  0,  3,250,SPELL_NORMAL,NOT_SELF|CHAR_TARGET| IS_AGGRESSIVE);
    spell_level(SPELL_SLEEP,	     i, 13, 10,  0, 21, 10,  3,200,SPELL_NORMAL,CHAR_TARGET| IS_AGGRESSIVE);
    spell_level(SPELL_SLOW,	     i, 17, 16, 17, 21, 15,  3,200,SPELL_NORMAL,CHAR_TARGET| IS_AGGRESSIVE);
    spell_level(SPELL_STORM,	     i, 15,  0,  0,  0, 15,  4,300,SPELL_NORMAL,CHAR_ROOM);
    spell_level(SPELL_UNBALANCE,     i, 10, 10, 10, 10, 10,  2,150,SPELL_NORMAL,CHAR_TARGET| IS_AGGRESSIVE);
    spell_level(SPELL_WIND_ARMOR,    i, 18,  0,  0,  0,  0,  2,100,SPELL_NORMAL,CHAR_TARGET);
    spell_level(SPELL_EARTH_ARMOR,   i,  0, 18,  0,  0,  0,  2,100,SPELL_NORMAL,CHAR_TARGET);
    spell_level(SPELL_WRAP, 	     i, 37,  0,  0,  0,  0,  1, 50,SPELL_NORMAL,NOT_SELF|CHAR_TARGET| IS_AGGRESSIVE);

    spell_level(SKILL_GRASP,		 i,  1,  1,  1,  1,  1,  3,  0,SPELL_NORMAL,0);
    spell_level(SKILL_RELEASE,	 i,  1,  1,  1,  1,  1,  3,  0,SPELL_NORMAL,0);
    spell_level(SKILL_PORTAL_STONES, i, 20, 20, 20, 20, 20,  5,  0,SPELL_NORMAL,0);
    spell_level(SKILL_BREAKSHIELD  , i, 0 ,  0, 20,  0,  0,  8,  0,SPELL_NORMAL, 0);
    spell_level(SPELL_WARDHUMAN  ,   i,  0,  0,  0,  20,   0,  6, 500, SPELL_NORMAL,CHAR_ROOM);
/*   spell_level(SPELL_WARDSHADOWSPAWN,i,  0,  0,  0,  30,   0, 10, 500, SPELL_NORMAL,CHAR_ROOM);
*/
    spell_level(SPELL_GROUP_RESTORE, i,  15,  0,  10,  20,  5,  1,1000, SPELL_GUILD, NO_TARGET);
    spell_level(SPELL_ROLLING_RING,  i,  5,  30, 30,  0,  0,  1, 600,  SPELL_GUILD,NO_TARGET|IS_AGGRESSIVE);
    spell_level(SPELL_WHIRLWIND, 	 i,  30,  0,  0,  0,  30,  1, 600,  SPELL_GUILD,NO_TARGET|IS_AGGRESSIVE);
    spell_level(SPELL_KINHEAL,       i,  1,  10,  10,  10,  10,  10,   1,  SPELL_GUILD,NO_TARGET);
    spell_level(SPELL_SHIELD_OF_SAIDIN,  i,  30,  10,  0,  15,  0,  1, 200,  SPELL_GUILD, CHAR_TARGET);
    spell_level(SPELL_THUNDERBURST,  i,  1,  20,  20,  20,  0,  0, 750, SPELL_GUILD, NO_TARGET|IS_AGGRESSIVE);
    spell_level(SPELL_STORMSHIELD,   i,  1,  30,  10,  0,  15,  0, 100, SPELL_GUILD, CHAR_TARGET);
    spell_level(SPELL_PAIN,	     i, 15, 15, 15, 15, 15,  3,185,SPELL_GUILD,NOT_SELF|CHAR_TARGET| IS_AGGRESSIVE);
    spell_level(SPELL_COMPULSION,    i, 35, 35, 35, 35, 35,  3,185,SPELL_GUILD,NOT_SELF|CHAR_TARGET| IS_AGGRESSIVE);
    spell_level(SPELL_ROOMSEAL,      i, 30,  0,  0, 15,  0,  4,150,SPELL_GUILD,CHAR_ROOM);
    spell_level(SPELL_TARGATE,       i, 35, 35, 35, 35, 35,200,200,SPELL_GUILD,CHAR_ROOM);
    spell_level(SPELL_SEDAIBOND,     i, 0,  0,  0,  30, 0, 0, 50, SPELL_GUILD, NOT_SELF | CHAR_TARGET);
    spell_level(SPELL_ASHABOND,      i, 0,  0,  0,  30, 0, 0, 50, SPELL_GUILD, NOT_SELF | CHAR_TARGET);
    spell_level(SPELL_WATERSHIELD,   i, 1,   30,  10,  0,  15,  0, 100, SPELL_GUILD, CHAR_TARGET);
    spell_level(SPELL_STORMSFURY,    i,  1,  20,  20,  20,  0,  0, 750, SPELL_GUILD, NO_TARGET|IS_AGGRESSIVE);
    spell_level(SPELL_WRATH,         i, 15,   0,  15,  15, 15,  0, 750, SPELL_GUILD, NO_TARGET|IS_AGGRESSIVE);
  }
}

void channeler_setup(void)
{
  spell_level(SKILL_BACKSTAB,		CLASS_CHANNELER,200, -1, -1, -1, -1, 14, 50,SKILL_NORMAL,0);
  spell_level(SKILL_BASH,		CLASS_CHANNELER, 35, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DIRTKICK,		CLASS_CHANNELER, 60, -1, -1, -1, -1,  8, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DISARM,		CLASS_CHANNELER, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DODGE,		CLASS_CHANNELER, 20, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DUCK,		CLASS_CHANNELER, 40, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_FOURTH_ATTACK,	CLASS_CHANNELER, 200, -1, -1, -1, -1, 10, 50,SKILL_NORMAL,0);
  spell_level(SKILL_HASTE,		CLASS_CHANNELER,120, -1, -1, -1, -1, 10, 50,SKILL_NORMAL,0);
  spell_level(SKILL_HIDE,		CLASS_CHANNELER, 200, -1, -1, -1, -1, 10, 50,SKILL_NORMAL,0);
  spell_level(SKILL_LOCATE,             CLASS_CHANNELER, 80, -1, -1, -1, -1, 5, 50, SKILL_NORMAL,0);
  spell_level(SKILL_HIDE_TRACKS,	CLASS_CHANNELER, 200, -1, -1, -1, -1,  4, 50,SKILL_NORMAL,0);
  spell_level(SKILL_KICK,		CLASS_CHANNELER, 12, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_PARRY,		CLASS_CHANNELER, 15, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_PICK_LOCK,		CLASS_CHANNELER, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_POWERTRAP,		CLASS_CHANNELER, 200, -1, -1, -1, -1,  11, 50,SKILL_NORMAL,0);
  spell_level(SKILL_PUNCH,		CLASS_CHANNELER, 20, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_RESCUE,		CLASS_CHANNELER, 200, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_SECOND_ATTACK,	CLASS_CHANNELER, 15, -1, -1, -1, -1,  4, 50,SKILL_NORMAL,0);
  spell_level(SKILL_SHIELD_BLOCK,	CLASS_CHANNELER, 40, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_SLICEWEAVE,	        CLASS_CHANNELER, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_SNEAK,		CLASS_CHANNELER, 200, -1, -1, -1, -1, 10, 50,SKILL_NORMAL,0);
  spell_level(SKILL_STUN,		CLASS_CHANNELER,200, -1, -1, -1, -1,  8, 50,SKILL_NORMAL,0);
  spell_level(SKILL_THIRD_ATTACK,	CLASS_CHANNELER, 200, -1, -1, -1, -1,  6, 50,SKILL_NORMAL,0);
  spell_level(SKILL_TRACK,		CLASS_CHANNELER,200, -1, -1, -1, -1,  7, 50,SKILL_NORMAL,0);
  spell_level(SKILL_TRIP,		CLASS_CHANNELER, 35, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_JUMP,		CLASS_CHANNELER, 75, -1, -1, -1, -1,  4,50, SKILL_NORMAL,0);
  spell_level(SKILL_APPRAISE,		CLASS_CHANNELER, 30 , -1, -1, -1, -1, 4,50,SKILL_NORMAL,0);
}

void wilder_setup(void)
{
  spell_level(SKILL_BACKSTAB,		CLASS_WILDER, 200, -1, -1, -1, -1, 10, 50,SKILL_NORMAL,0);
  spell_level(SKILL_BASH,		CLASS_WILDER, 25, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DIRTKICK,		CLASS_WILDER, 50, -1, -1, -1, -1,  8, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DISARM,		CLASS_WILDER, 200, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DODGE,		CLASS_WILDER, 12, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DUCK,		CLASS_WILDER, 30, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_FOURTH_ATTACK,	CLASS_WILDER, 200, -1, -1, -1, -1,  8, 50,SKILL_NORMAL,0);
  spell_level(SKILL_HASTE,		CLASS_WILDER,100, -1, -1, -1, -1, 10, 50,SKILL_NORMAL,0);
  spell_level(SKILL_HIDE,		CLASS_WILDER, 200, -1, -1, -1, -1,  7, 50,SKILL_NORMAL,0);
  spell_level(SKILL_LOCATE,             CLASS_WILDER, 60, -1, -1, -1, -1, 5, 50, SKILL_NORMAL,0);
  spell_level(SKILL_HIDE_TRACKS,	CLASS_WILDER, 200, -1, -1, -1, -1,  4, 50,SKILL_NORMAL,0);
  spell_level(SKILL_KICK,		CLASS_WILDER,  9, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_PARRY,		CLASS_WILDER, 10, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_PICK_LOCK,		CLASS_WILDER, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_POWERTRAP,		CLASS_WILDER, 200, -1, -1, -1, -1,  12, 50,SKILL_NORMAL,0);
  spell_level(SKILL_PUNCH,		CLASS_WILDER, 20, -1, -1, -1, -1,  4, 50,SKILL_NORMAL,0);
  spell_level(SKILL_RESCUE,		CLASS_WILDER, 200, -1, -1, -1, -1,  1, 50,SKILL_NORMAL,0);
  spell_level(SKILL_SECOND_ATTACK,	CLASS_WILDER, 15, -1, -1, -1, -1,  4, 50,SKILL_NORMAL,0);
  spell_level(SKILL_SHIELD_BLOCK,	CLASS_WILDER, 35, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_SLICEWEAVE,	        CLASS_WILDER, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_SNEAK, 		CLASS_WILDER, 200, -1, -1, -1, -1,  7, 50,SKILL_NORMAL,0);
  spell_level(SKILL_STUN,		CLASS_WILDER, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_THIRD_ATTACK,	CLASS_WILDER, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_TRACK,		CLASS_WILDER, 200, -1, -1, -1, -1,  7, 50,SKILL_NORMAL,0);
  spell_level(SKILL_TRIP,		CLASS_WILDER, 30, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_JUMP,		CLASS_WILDER, 65, -1, -1, -1, -1,  4, 50, SKILL_NORMAL,0);
  spell_level(SKILL_APPRAISE,		CLASS_WILDER, 30 , -1, -1, -1, -1, 4,50,SKILL_NORMAL,0);
}

void adventurer_setup(void)
{
  spell_level(SKILL_SLICEWEAVE,	        CLASS_ADVENTURER, 200, 20, 20, 20, 20,  200, 200,SKILL_NORMAL,0);

  spell_level(SKILL_BASH,		CLASS_ADVENTURER,  6, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DODGE,		CLASS_ADVENTURER,  7, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_KICK,		CLASS_ADVENTURER,  3, -1, -1, -1, -1,  1, 50,SKILL_NORMAL,0);
  spell_level(SKILL_RESCUE,		CLASS_ADVENTURER, 200, -1, -1, -1, -1,  1, 50,SKILL_NORMAL,0);
  spell_level(SKILL_SECOND_ATTACK,	CLASS_ADVENTURER,  5, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_THIRD_ATTACK,	CLASS_ADVENTURER, 20, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_FOURTH_ATTACK,	CLASS_ADVENTURER, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_TRIP,		CLASS_ADVENTURER, 200, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_PUNCH,		CLASS_ADVENTURER, 13, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DIRTKICK,		CLASS_ADVENTURER, 40, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DISARM,		CLASS_ADVENTURER, 200, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_PICK_LOCK,		CLASS_ADVENTURER, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_PARRY,		CLASS_ADVENTURER,  5, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_SHIELD_BLOCK,	CLASS_ADVENTURER, 25, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DUCK,		CLASS_ADVENTURER, 25, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_HASTE,		CLASS_ADVENTURER, 80, -1, -1, -1, -1,  8, 50,SKILL_NORMAL,0);
  spell_level(SKILL_STUN,		CLASS_ADVENTURER, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_BACKSTAB,		CLASS_ADVENTURER, 200, -1, -1, -1, -1,  8, 50,SKILL_NORMAL,0);
  spell_level(SKILL_SNEAK,		CLASS_ADVENTURER, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_HIDE,		CLASS_ADVENTURER, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_LOCATE,             CLASS_ADVENTURER, 40, -1, -1, -1, -1, 5, 50, SKILL_NORMAL,0);
  spell_level(SKILL_TRACK,		CLASS_ADVENTURER, 200, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_HIDE_TRACKS,	CLASS_ADVENTURER, 200, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_JUMP, 		CLASS_ADVENTURER, 50, -1, -1, -1, -1, 2, 50, SKILL_NORMAL,0);
  spell_level(SKILL_APPRAISE,		CLASS_ADVENTURER, 30 , -1, -1, -1, -1, 4,50,SKILL_NORMAL,0);
}

void soldier_setup(void)
{
  spell_level(SKILL_SLICEWEAVE,	        CLASS_SOLDIER,200, 20, 20, 20, 20,  200, 200,SKILL_NORMAL,0);

  spell_level(SKILL_BASH,		CLASS_SOLDIER,  5, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DODGE,		CLASS_SOLDIER,  5, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_KICK,		CLASS_SOLDIER,  2, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_RESCUE,		CLASS_SOLDIER, 200, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_SECOND_ATTACK,	CLASS_SOLDIER,  4, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_THIRD_ATTACK,	CLASS_SOLDIER, 18, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_FOURTH_ATTACK,	CLASS_SOLDIER, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_TRIP,		CLASS_SOLDIER, 22, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_PUNCH,		CLASS_SOLDIER, 15, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DIRTKICK,		CLASS_SOLDIER, 40, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DISARM,		CLASS_SOLDIER, 200, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_PICK_LOCK,		CLASS_SOLDIER, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_PARRY,		CLASS_SOLDIER,  3, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_LOCATE,             CLASS_SOLDIER, 40, -1, -1, -1, -1, 5, 50, SKILL_NORMAL,0);
  spell_level(SKILL_SHIELD_BLOCK,	CLASS_SOLDIER, 25, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DUCK,		CLASS_SOLDIER, 25, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_HASTE,		CLASS_SOLDIER, 80, -1, -1, -1, -1,  7, 50,SKILL_NORMAL,0);
  spell_level(SKILL_STUN,		CLASS_SOLDIER, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_BACKSTAB,		CLASS_SOLDIER, 200, -1, -1, -1, -1, 10, 50,SKILL_NORMAL,0);
  spell_level(SKILL_HIDE,		CLASS_SOLDIER, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_SNEAK,		CLASS_SOLDIER, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_TRACK,		CLASS_SOLDIER, 200, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_HIDE_TRACKS,	CLASS_SOLDIER, 200, -1, -1, -1, -1,  4, 50,SKILL_NORMAL,0);
  spell_level(SKILL_JUMP, 		CLASS_SOLDIER,  45, -1, -1, -1, -1,  2, 50, SKILL_NORMAL,0); 
  spell_level(SKILL_APPRAISE,		CLASS_SOLDIER, 30 , -1, -1, -1, -1, 4,50,SKILL_NORMAL,0);
}

void thief_setup(void)
{
  spell_level(SKILL_SLICEWEAVE,	        CLASS_THIEF,200, 20, 20, 20, 20,  200, 200,SKILL_NORMAL,0);

  spell_level(SKILL_BASH,		CLASS_THIEF, 15, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DODGE,		CLASS_THIEF, 10, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_KICK,		CLASS_THIEF,  7, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_LOCATE,             CLASS_THIEF, 20, -1, -1, -1, -1, 5, 50, SKILL_NORMAL,0);
  spell_level(SKILL_RESCUE,		CLASS_THIEF, 200, -1, -1, -1, -1,  1, 50,SKILL_NORMAL,0);
  spell_level(SKILL_SECOND_ATTACK,	CLASS_THIEF, 10, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_THIRD_ATTACK,	CLASS_THIEF, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_FOURTH_ATTACK,	CLASS_THIEF, 200, -1, -1, -1, -1,  8, 50,SKILL_NORMAL,0);
  spell_level(SKILL_TRIP,		CLASS_THIEF, 25, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_PUNCH,		CLASS_THIEF, 15, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DIRTKICK,		CLASS_THIEF, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DISARM,		CLASS_THIEF, 200, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_PICK_LOCK,		CLASS_THIEF, 20 , -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_PARRY,		CLASS_THIEF, 20, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_SHIELD_BLOCK,	CLASS_THIEF, 50, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DUCK,		CLASS_THIEF, 45, -1, -1, -1, -1,  1, 50,SKILL_NORMAL,0);
  spell_level(SKILL_HASTE,		CLASS_THIEF,100, -1, -1, -1, -1,  4, 50,SKILL_NORMAL,0);
  spell_level(SKILL_STUN,		CLASS_THIEF,200, -1, -1, -1, -1, 10, 50,SKILL_NORMAL,0);
  spell_level(SKILL_BACKSTAB,		CLASS_THIEF, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_HIDE,		CLASS_THIEF, 200, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_SNEAK,		CLASS_THIEF, 200, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_TRACK,		CLASS_THIEF, 200, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_HIDE_TRACKS,	CLASS_THIEF, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_STEAL,		CLASS_THIEF, 25 , -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_JUMP, 		CLASS_THIEF, 50 , -1, -1, -1, -1,  3, 50, SKILL_NORMAL,0);
  spell_level(SKILL_APPRAISE,		CLASS_THIEF, 30 , -1, -1, -1, -1, 4,50,SKILL_NORMAL,0);
}

void wisdom_setup(void)
{
  spell_level(SKILL_SLICEWEAVE,	        CLASS_WISDOM,200, 20, 20, 20, 20,  200, 200,SKILL_NORMAL,0);

  spell_level(SKILL_BASH,		CLASS_WISDOM, 25, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DODGE,		CLASS_WISDOM, 20, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_KICK,		CLASS_WISDOM, 15, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_RESCUE,		CLASS_WISDOM, 200, -1, -1, -1, -1,  1, 50,SKILL_NORMAL,0);
  spell_level(SKILL_SECOND_ATTACK,	CLASS_WISDOM, 20, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_THIRD_ATTACK,	CLASS_WISDOM, 200, -1, -1, -1, -1,  8, 50,SKILL_NORMAL,0);
  spell_level(SKILL_FOURTH_ATTACK,	CLASS_WISDOM, 200, -1, -1, -1, -1, 10, 50,SKILL_NORMAL,0);
  spell_level(SKILL_TRIP,		CLASS_WISDOM, 30, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_PUNCH,		CLASS_WISDOM, 25, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DIRTKICK,		CLASS_WISDOM, 55, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DISARM,		CLASS_WISDOM, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_PICK_LOCK,		CLASS_WISDOM, 200, -1, -1, -1, -1, 10, 50,SKILL_NORMAL,0);
  spell_level(SKILL_PARRY,		CLASS_WISDOM, 30, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_SHIELD_BLOCK,	CLASS_WISDOM, 60, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_LOCATE,             CLASS_WISDOM, 50, -1, -1, -1, -1, 5, 50, SKILL_NORMAL,0);
  spell_level(SKILL_DUCK,		CLASS_WISDOM, 55, -1, -1, -1, -1,  1, 50,SKILL_NORMAL,0);
  spell_level(SKILL_HASTE,		CLASS_WISDOM,100, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_STUN,		CLASS_WISDOM,200, -1, -1, -1, -1, 10, 50,SKILL_NORMAL,0);
  spell_level(SKILL_BACKSTAB,		CLASS_WISDOM, 200, -1, -1, -1, -1, 10, 50,SKILL_NORMAL,0);
  spell_level(SKILL_HIDE,		CLASS_WISDOM, 200, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_HERBLORE,		CLASS_WISDOM, 10, -1, -1, -1, -1,  1, 50,SKILL_NORMAL,0);
  spell_level(SKILL_SNEAK,		CLASS_WISDOM, 200, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_TRACK,		CLASS_WISDOM, 200, -1, -1, -1, -1,  2, 50,SKILL_NORMAL,0);
  spell_level(SKILL_HIDE_TRACKS,	CLASS_WISDOM, 200, -1, -1, -1, -1,  5, 50,SKILL_NORMAL,0);
  spell_level(SKILL_JUMP, 		CLASS_WISDOM, 90 , -1, -1, -1, -1,  5, 50, SKILL_NORMAL,0);
  spell_level(SKILL_APPRAISE,		CLASS_WISDOM, 30 , -1, -1, -1, -1, 4,50,SKILL_NORMAL,0);
}

void craftsman_setup(void)
{
  spell_level(SKILL_SLICEWEAVE,	        CLASS_CRAFTSMAN,200, 20, 20, 20, 20,  200, 200,SKILL_NORMAL,0);

  spell_level(SKILL_BASH,		CLASS_CRAFTSMAN, 20, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_CRAFT,		CLASS_CRAFTSMAN, 85, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);  
  spell_level(SKILL_DODGE,		CLASS_CRAFTSMAN, 15, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_KICK,		CLASS_CRAFTSMAN,  8, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_LOCATE,             CLASS_CRAFTSMAN, 50, -1, -1, -1, -1, 5, 50, SKILL_NORMAL,0);
  spell_level(SKILL_RESCUE,		CLASS_CRAFTSMAN, 200, -1, -1, -1, -1,  1, 50,SKILL_NORMAL,0);
  spell_level(SKILL_SECOND_ATTACK,	CLASS_CRAFTSMAN, 15, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_THIRD_ATTACK,	CLASS_CRAFTSMAN, 200, -1, -1, -1, -1,  8, 50,SKILL_NORMAL,0);
  spell_level(SKILL_FOURTH_ATTACK,	CLASS_CRAFTSMAN, 200, -1, -1, -1, -1, 10, 50,SKILL_NORMAL,0);
  spell_level(SKILL_TRIP,		CLASS_CRAFTSMAN, 27, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_PUNCH,		CLASS_CRAFTSMAN, 25, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DIRTKICK,		CLASS_CRAFTSMAN, 40, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DISARM,		CLASS_CRAFTSMAN, 200, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_PICK_LOCK,		CLASS_CRAFTSMAN, 200, -1, -1, -1, -1,  8, 50,SKILL_NORMAL,0);
  spell_level(SKILL_PARRY,		CLASS_CRAFTSMAN, 25, -1, -1, -1, -1,  1, 50,SKILL_NORMAL,0);
  spell_level(SKILL_SHIELD_BLOCK,	CLASS_CRAFTSMAN, 70, -1, -1, -1, -1,  1, 50,SKILL_NORMAL,0);
  spell_level(SKILL_DUCK,		CLASS_CRAFTSMAN, 40, -1, -1, -1, -1,  1, 50,SKILL_NORMAL,0);
  spell_level(SKILL_HASTE,		CLASS_CRAFTSMAN,100, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_STUN,		CLASS_CRAFTSMAN,200, -1, -1, -1, -1,  8, 50,SKILL_NORMAL,0);
  spell_level(SKILL_BACKSTAB,		CLASS_CRAFTSMAN, 200, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_HIDE,		CLASS_CRAFTSMAN, 200, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_SNEAK,		CLASS_CRAFTSMAN, 200, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_TOOL,		CLASS_CRAFTSMAN,  40, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_TRACK,		CLASS_CRAFTSMAN, 200, -1, -1, -1, -1,  3, 50,SKILL_NORMAL,0);
  spell_level(SKILL_HIDE_TRACKS,	CLASS_CRAFTSMAN, 200, -1, -1, -1, -1,  7, 50,SKILL_NORMAL,0);
  spell_level(SKILL_JUMP, 		CLASS_CRAFTSMAN, 85 , -1, -1, -1, -1, 5, 50, SKILL_NORMAL,0);
  spell_level(SKILL_APPRAISE,		CLASS_CRAFTSMAN, 30 , -1, -1, -1, -1, 4,50,SKILL_NORMAL,0);
  spell_level(SKILL_ENHANCE, 		CLASS_CRAFTSMAN,100 , -1, -1, -1, -1, 8, 50, SKILL_NORMAL,0); 
  spell_level(SKILL_REPAIR,		CLASS_CRAFTSMAN,65  , -1, -1, -1, -1, 3, 50, SKILL_NORMAL, 0);
}



/*talent stuff goes here*/
