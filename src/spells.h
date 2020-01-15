/******************************************************************************
 * $Id: spells.h,v 1.4 2003/06/11 03:08:56 fang Exp $
 *   
 * File: spells.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: spells.h,v $
 *   Revision 1.4  2003/06/11 03:08:56  fang
 *   Added WRATH weave.
 *
 *   Revision 1.3  2002/09/22 21:43:50  fischer
 *   Removing mounts and horsetrain from the code
 *
 *   Revision 1.2  2002/07/26 19:39:52  fang
 *   Dath: MYCOLOR mem leak fixed.  Thanks, Fred!
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.3  2002/07/16 15:16:17  fang
 *   Drath: Seafolk Guildskills are done.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.10  2002/01/24 21:33:20  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.9.2.2  2002/01/24 03:20:34  mud
 *   Removed some further unescessary files (player.[ch], roomprog.[ch]
 *   old2new.c). Cleaned up some other stuff (guild stuff mostly).
 *
 *   Revision 1.9.2.1  2002/01/22 03:05:16  mud
 *   More cleanup. Removed extern decls from source files. Protected some DG headers from multiple inclision
 *
 *   Revision 1.9  2001/08/11 17:51:12  mud
 *   *** empty log message ***
 *
 *   Revision 1.8  2001/07/18 17:09:12  mud
 *
 *   bond becomes spells ashabond and sedaibond
 *
 *   Revision 1.7  2000/10/03 17:11:47  mud
 *   *** empty log message ***
 *
 *   Revision 1.6  2000/08/14 19:11:56  mud
 *   The new guild system.
 *
 *   Revision 1.5  2000/05/26 09:20:33  mud
 *   Fixed findherb, added a herbident ACMD, IMM commands level fixes.
 *
 *   Revision 1.4  2000/05/13 14:32:30  mud
 *   First changes to bond code in making of bond spell numbers.
 *
 *   Revision 1.3  2000/03/15 13:11:40  mud
 *   Changed from B-tree based skill/weave index to 'something else' :)
 *
 *   Revision 1.2  2000/02/21 14:00:19  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/*
************************************************************************
*   File: spells.h                                      Part of CircleMUD *
*  Usage: header file: constants and fn prototypes for spell system       *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */
#ifndef __SPELLS_H__
#define __SPELLS_H__

#include "tree.h"
#include "structs.h"

#define IS_AGGRESSIVE   (1 << 0)
#define CHAR_TARGET	(1 << 1)   	/* Need a char as target    		*/
#define OBJ_TARGET	(1 << 2)    	/* Need an object as target		*/
#define ROOM_TARGET	(1 << 3)	/* Need a room as target		*/
#define SPELL_TARGET    (1 << 4)	/* Need a spell as target		*/
#define DOOR_TARGET	(1 << 5)	/* Need a door/direction as target	*/
#define NOT_SELF	(1 << 6)	/* Can't be casted upon self		*/
#define ONLY_SELF	(1 << 7)	/* Can ONLY be cast in self		*/
#define NO_TARGET	(1 << 8)	/* Needs NO target, whatsoever!		*/
#define CHAR_ROOM	(1 << 9)	/* To room char is in.			*/

#define POWERS 5
#define AIR    0
#define EARTH  1
#define FIRE   2
#define SPIRIT 3
#define WATER  4
#define MAX_POWER 100

#define MAX_SHROOMS 25
#define SHROOM_NUM  41	/* For create food! */

#define SKILL_NORMAL  (1 << 0)
#define SPELL_NORMAL  (1 << 1)
#define SKILL_GUILD   (1 << 2)
#define SPELL_GUILD   (1 << 3)
#define SKILL_QUEST   (1 << 4)
#define SPELL_QUEST   (1 << 5)
#define SKILL_SPECIAL (1 << 6)
#define SPELL_SPECIAL (1 << 7)

#define TYPE_UNDEFINED               -1
#define SPELL_RESERVED_DBC            0  /* SKILL NUMBER ZERO -- RESERVED */

/* PLAYER SPELLS -- Numbered from 1 to MAX_SPELLS */

#define SPELL_ARMOR                   1
#define SPELL_AIRSWORD		      2
#define SPELL_GATE    		      3
#define SPELL_BLIND                   4
#define SPELL_INVISIBILITY	      5
#define SPELL_CALL_LIGHTNING          6
#define SPELL_FIRESWORD	 	      7
#define SPELL_WRAP		      8
#define SPELL_GAG		      9
#define SPELL_SHIELD		     10
#define SPELL_LINK		     11
#define SPELL_GENTLE		     12
#define SPELL_CREATE_WATER           13
#define SPELL_HEAL		     14
#define SPELL_HEAL_GENTLING	     15
#define SPELL_AIRSHIELD		     16
#define SPELL_BURN		     17
#define SPELL_AIRBLOW		     18
#define SPELL_ICE_ARROWS	     19
#define SPELL_DETECT_MAGIC           20
#define SPELL_METEOR		     21
#define SPELL_LIGHT                  22
#define SPELL_EARTHQUAKE             23
#define SPELL_ROLLING_RING	     24
#define SPELL_LIGHTNING              25
#define SPELL_FIREBALL               26
#define SPELL_DARKNESS		     27
#define SPELL_BLINDNESS              28
#define SPELL_COMPULSION	     29
#define SPELL_CURE_LIGHT             30
#define SPELL_CURE_SERIOUS           31
#define SPELL_CURE_CRITICAL          32
#define SPELL_SANCTUARY              33
#define SPELL_SLEEP                  34
#define SPELL_SENSE_LIFE             35
#define SPELL_INFRAVISION	     36
#define SPELL_CHILL		     37
#define SPELL_EARTHSHIELD	     38
#define SPELL_WATERCASCADE	     39
#define SPELL_ICESTORM	  	     40
#define SPELL_DJINN		     41
#define SPELL_PAIN		     42
#define SPELL_FLY		     43
#define SPELL_BLOCK_GATEWAY	     44
#define SPELL_CREATE_FOOD	     45
#define SPELL_SLICEWEAVE	     46
#define SPELL_WHIRLWIND		     47
#define SPELL_DELVE                  48
#define SPELL_MINOR_HEAL	     49
#define SPELL_DOMEOFAIR		     50
#define SPELL_BALLOFLIGHT	     51
#define SPELL_DISINTEGRATE	     52
#define SPELL_BALLOFAIR		     53
#define SPELL_OPEN_LOCK		     54
#define SPELL_CALL_FIRE		     55
#define SPELL_SHAFTOFFIRE	     56
#define SPELL_DOMEOFFIRE	     57
#define SPELL_DEVASTATION	     58
#define SPELL_DESTROY_WEAPON	     59
#define SPELL_DROWN		     60
#define SPELL_POISON		     61
#define SPELL_INVIGORATE	     62
#define SPELL_STORM		     63
#define SPELL_DARTOFAIR		     64
#define SPELL_CALL_FROST	     65
#define SPELL_SHAFTOFFROST	     66
#define SPELL_WIND_ARMOR	     67
#define SPELL_CONFUSION		     68
#define SPELL_SLOW		     69
#define SPELL_UNBALANCE		     70
#define SPELL_EXPLOSION		     71
#define SPELL_DOMEOFFROST	     72
#define SPELL_FISTOFAIR		     73
#define SPELL_FIRESTORM		     74
#define SPELL_REFRESH		     75
#define SPELL_EARTH_ARMOR	     76
#define SPELL_DESTROY_ARMOR	     77
#define SPELL_FIRE_ARROWS   	     78
#define SPELL_EARTH_MISSILE	     79
#define SPELL_KINHEAL		     80
#define SPELL_SHIELD_OF_SAIDIN       81
#define SPELL_CRIPPLE		     82
#define SPELL_DEAF		     83
#define SPELL_HEAL_II		     84
#define SPELL_THUNDERBURST	     85
#define SPELL_STORMSHIELD	     86
#define SPELL_GROUP_RESTORE          87
#define SPELL_WARDHUMAN		     88
#define SPELL_IDENTIFY		     89
#define SPELL_FOG		     90
#define SPELL_ENTANGLE		     91
#define SPELL_MEND		     92
#define SPELL_FROSTBITE		     93
#define SPELL_ROOMSEAL		     94
#define SPELL_TARGATE		     95
#define SPELL_SKIMMING		     96
#define SPELL_SEDAIBOND              97
#define SPELL_ASHABOND               98
#define SPELL_WATERSHIELD	     99
#define SPELL_STORMSFURY	     100
#define SPELL_WRATH                  101
/* Insert new spells here, up to MAX_SPELLS */

#define MAX_SPELLS		    200

/* PLAYER SKILLS - Numbered from MAX_SPELLS+1 to MAX_SKILLS */
#define SKILL_BACKSTAB              201
#define SKILL_BASH                  202
#define SKILL_HIDE                  203
#define SKILL_KICK                  204
#define SKILL_PICK_LOCK             205
#define SKILL_PUNCH                 206
#define SKILL_RESCUE                207
#define SKILL_SNEAK                 208
#define SKILL_STEAL                 209
#define SKILL_TRACK		    210
#define SKILL_FIRST_AID		    211
#define SKILL_DODGE		    212
#define SKILL_SECOND_ATTACK	    213
#define SKILL_THIRD_ATTACK	    214
#define SKILL_FOURTH_ATTACK	    215
#define SKILL_SHIELD_ATTACK	    216
#define SKILL_TRIP		    217
#define SKILL_STUN		    218
#define SKILL_ROLL_WITH_BLOW        220
#define SKILL_ENDURANCE	            221
#define SKILL_MARCH		    222
#define SKILL_FAST_HEALING	    223
#define SKILL_SHOOT		    224
#define SKILL_DIAGNOSE		    225
#define SKILL_BERSERK		    226
#define SKILL_TOOL		    227
#define SKILL_HERBLORE		    228
#define SKILL_HERBMASTER	    229
#define SKILL_FIFTH_ATTACK	    230
#define SKILL_GRASP		    231
#define SKILL_RELEASE		    232
#define SKILL_TGRASP		    233
#define SKILL_TRELEASE		    234
#define SKILL_SPEARDANCE	    235
#define SKILL_DISARM		    236
#define SKILL_TWO_WEAPONS	    237
#define SKILL_SHIELD_BLOCK	    238
#define SKILL_PARRY		    239
#define SKILL_SWORDDANCE	    240
#define SKILL_DIRTKICK              241
#define SKILL_MARTIAL_ARTS	    242
#define SKILL_DREAM		    243
#define SKILL_DUCK		    244
#define SKILL_PLAY		    245
#define SKILL_TEACH		    246
#define SKILL_PORTAL_STONES	    247
#define SKILL_HIDE_TRACKS           248
#define SKILL_SLICEWEAVE	    249
#define SKILL_LEARN		    250
#define SKILL_PRIVACY		    251
#define SKILL_DREAMLIST             252
#define SKILL_DREAMPULL             253
#define SKILL_BREAKSHIELD	    254
#define old_SKILL_HORSETRAIN   	    255
#define SKILL_CRAFT		    256
#define SKILL_POWERTRAP		    257
#define SKILL_JUMP		    258
#define SKILL_MEDITATION	    259 /* "Mostly" for DF */
#define SKILL_APPRAISE		    260 /* Craftsman */
#define SKILL_ENHANCED_DAMAGE       261 
#define SKILL_DEFENSE		    262 /* Two Rivers G-skill */
#define SKILL_MASK		    263 /* Mask channeling ability */
#define SKILL_FORTIFY		    264 /* CoTL G-skill */
#define SKILL_PRODIGY               265 /* Rebel Warders (where are they??) */
#define SKILL_TRAIL                 266 /* Someone needed to define this (M) */
#define SKILL_LOCATE                267
#define SKILL_BLACK_ARTS	    268  /* DF SKILL */
#define SKILL_DREAMENTER            269  /* Aiel Skills */
#define SKILL_WKSET                 270  /* It's a skill so that morts can do it */
#define SKILL_TERTEST		    271
#define SKILL_TERINFO		    272
#define SKILL_WOLFSENSE             273
#define SKILL_WOLFCALL              274

/*Seafolk guild skills */
#define SKILL_NAVIGATE		    275
#define SKILL_BARGAIN		    276
#define SKILL_SALTBROTHERS	    277

/*Hunter guild skills */
#define SKILL_EAVESDROP	            278
#define SKILL_MARKEDMAP		    279
#define SKILL_ADRENALIN		    280
#define SKILL_KNIFETOSS		    281
#define SKILL_AWARENESS             282
#define SKILL_MINORDISGUISE	    283
#define SKILL_CHARM                 284
#define SKILL_DAZZLE		    285

/* Caemlyn guild skills */
#define SKILL_FINE	290
#define SKILL_THROW     291
#define SKILL_ARMSCRY   292

/* Extended WC skills - blame Rhadam and Pedron */
#define SKILL_SUPPLY    295



/* Tinkers Guild skills */
#define SKILL_ENHANCE 296
#define SKILL_REPAIR  297
#define SKILL_CAMP    298
#define SKILL_TRAVEL  299

/* 2 Rivers Guild Skills */
#define SKILL_SURVIVAL 300
#define SKILL_MEDICINE 301
#define SKILL_STUBBORNESS 302

/* Darkfriend Guild Skills */
#define SKILL_DTALK    303
#define SKILL_SUMMON   304
#define SKILL_SEVER    305
#define SKILL_HUNT     306
#define SKILL_TEST     307
#define SKILL_OBSCURE  308
#define SKILL_LIPREAD  309
#define SKILL_DARKGIFT 310

/* Gleeman Guild Skills */

#define SKILL_TAIL     311
#define SKILL_FX       312
#define SKILL_RUMOR    313
#define SKILL_SHIELDWALL 314
#define SKILL_PATROL 315

/* BotRH Guild Skills */
#define SKILL_TACTICS  316
#define SKILL_FLANK    317
#define SKILL_BRACE    318
#define SKILL_AIM      319
#define SKILL_STRIKE   320
#define SKILL_BANDHEAL 321
#define SKILL_BANDMARCH 322

/* Cotl Reserve spot */
#define SKILL_INQUISITION  323
#define SKILL_FERVOR	   324

/* Tower Warder guild skills */
#define SKILL_VOID         325
#define SKILL_WARDERHEAL   326 /*No longer in use*/
#define SKILL_WARDERCOMBAT 327
#define SKILL_WARDERSENSE  328 /*No longer in use*/
#define SKILL_FORAGE       329

/* Tower Sedai guild skills */
#define SKILL_MEDIATE 	   330
#define SKILL_KNOWLEDGE    331
#define SKILL_KNOWHEALTH   332
#define SKILL_QUICK_MIND   333

/* Another Tower Warder Skill */
#define SKILL_SEEK         334

/* AotD Guild skills */
#define SKILL_REPEL	   335
#define SKILL_ARREST	   336

/* DS Guild Skills */
#define SKILL_PREACH       337

/* Forsaken Guildskills */
#define SKILL_SFX	   340
#define SKILL_DISGUISE	   341

/* Aiel Guild Skills */
#define SKILL_CALL	   342
#define SKILL_RAGE	   343
#define SKILL_POUND	   344
#define SKILL_CAMOUFLAGE   345
#define SKILL_PEACE	   346
#define SKILL_HERBMASTERY  347
#define SKILL_VEIL	   348
#define SKILL_HANDTALK     349
#define SKILL_INFRAVISION  350
#define SKILL_TRACTION     351
#define SKILL_RETREAT      352
#define SKILL_SEARCH       353
#define SKILL_DISENGAGE    354
#define SKILL_STAB         355
#define SKILL_HASTE        356
#define SKILL_CRITICAL_HIT 357
#define SKILL_STALK        358
#define SKILL_RUN          359

/* CoTL Guild Skills */
#define SKILL_MORALE	   360
#define SKILL_REINFORCE    361
#define SKILL_BRAND	   362
#define SKILL_TRUELIGHT    363
#define SKILL_CHARGE       364

/* Seanchan Guild Skills */
#define SKILL_LEADERSHIP   365
#define SKILL_RECRUIT	   366
#define SKILL_RAVAGE	   367
#define SKILL_RESPONSE	   368

/* Ogier Guild skills */
#define SKILL_TREESINGING  370

/* Hunters of the Horn Guild skills */
#define SKILL_SHADOW       372
#define SKILL_ARROGANCE    373
#define SKILL_OFFEND       374

/* QUEST skills */
#define SKILL_BLADEMASTERY 376
#define SKILL_SPEARMASTERY 377
#define SKILL_EMPLOY       378

/* Language skills */
#define SKILL_TROLLOC	   381
#define SKILL_OGIER	   382
#define SKILL_OLDTONGUE    383

/* The Kin guild skills */
#define SKILL_FINDHERB	   385
#define SKILL_HERBIDENT    386

/* Asha'man GUILD_SKILLS */
#define SKILL_CONCENTRATE  390
#define SKILL_ENHANCED_SPELL 391
#define SKILL_TARGET	   392 /* Actually a quest skill */

/* Rebel Sedai guild skills */
#define SKILL_INSPIRE      395
#define SKILL_G_DISGUISE   396
#define SKILL_INTIMIDATE   397

/* Some other guild skill */
#define SKILL_ENDURE       399

/* New skills may be added here up to MAX_SKILLS (400) */


/*
 *  NON-PLAYER AND OBJECT SPELLS AND SKILLS
 *  The practice levels for the spells and skills below are _not_ recorded
 *  in the playerfile; therefore, the intended use is for spells and skills
 *  associated with objects (such as SPELL_IDENTIFY used with scrolls of
 *  identify) or non-players (such as NPC-only spells).
 */

#define NOQUIT_TEXT		     407
#define LONGSLEEP_TEXT		     408

#define TOP_SPELL_DEFINE	     499
/* NEW NPC/OBJECT SPELLS can be inserted here up to 299 */


/* WEAPON ATTACK TYPES */

#define TYPE_HIT                     500
#define TYPE_STING                   501
#define TYPE_WHIP                    502
#define TYPE_SLASH                   503
#define TYPE_BITE                    504
#define TYPE_BLUDGEON                505
#define TYPE_CRUSH                   506
#define TYPE_POUND                   507
#define TYPE_CLAW                    508
#define TYPE_MAUL                    509
#define TYPE_THRASH                  510
#define TYPE_PIERCE                  511
#define TYPE_BLAST		     512
#define TYPE_PUNCH		     513
#define TYPE_STAB		     514
#define TYPE_WHACK		     515

/* new attack types can be added here - up to TYPE_SUFFERING */
#define TYPE_SUFFERING		     599

struct spell_info_type {
   byte min_position;	 /* Position for caster	  */
   sh_int type;		 /* guild-skil? Normal-skill? */
   long mana;		 /* Mana used by spell    */
   long mana_add;	 /* How much mana to add for doubling of effect */

   long cost[NUM_CLASSES];/* Cost in trains to GAIN*/

   long min_level[NUM_CLASSES][5];
   long routines;
   byte violent;
   long targets; 	/* What targets? Also, AGGRESSIVE, and such */
};


#define SPELL_TYPE_SPELL   0
#define SPELL_TYPE_POTION  1


/* Attacktypes with grammar */

struct attack_hit_type {
   char	*singular;
   char	*plural;
};

#define BSPELL(spellname) \
void spellname(struct char_data *ch,struct char_data*vict, struct obj_data *obj,struct room_data *rm,\
               char *arg,long direction,long spell_num,long mana_add,sbyte tied,double mod) 
#define BPTR(spellname) \
void (*spellname)(struct char_data *ch,struct char_data *vict,struct obj_data *obj,\
                   struct room_data *rm,char *arg,long direction,long spell_num, long mana_add,sbyte tied,double mod)


/* basic magic calling functions */

int find_skill_num(char *name);

/* other prototypes */
void spell_level(int spell, int class, int air, int earth, int fire, int spirit, int water,int cost,int mana_apply,byte type,int target_type);
void init_spell_levels(void);
char *skill_name(int num);

BSPELL(armor);
BSPELL(airsword);
BSPELL(gate);
BSPELL(targate);
BSPELL(blindness);
BSPELL(invisibility);
BSPELL(infravision);
BSPELL(call_lightning);
BSPELL(firesword);
BSPELL(minor_healing);
BSPELL(wrap);
BSPELL(gag);
BSPELL(shield);
BSPELL(gentle);
BSPELL(heal_gentling);
BSPELL(earthquake);
BSPELL(sliceweave);
BSPELL(whirlwind);
BSPELL(fireball);
BSPELL(disintegrate);
BSPELL(ball_of_air);
BSPELL(open_lock);
BSPELL(call_fire);
BSPELL(shaft_of_fire);
BSPELL(earthshield);
BSPELL(fly);
BSPELL(sleeper);
BSPELL(ball_of_light);
BSPELL(create_wat);
BSPELL(heal);
BSPELL(airshield);
BSPELL(burn);
BSPELL(airblow);
BSPELL(ice_arrows);
BSPELL(detect_magic);
BSPELL(meteor);
BSPELL(lightning);
BSPELL(rolling_ring); /* Asha'man GuildSpell! */
BSPELL(light);
BSPELL(darkness);
BSPELL(destroy_weapon);
BSPELL(dome_of_air);
BSPELL(dome_of_fire);
BSPELL(delve);        /*  Yellow Sedai guild Spell  */
BSPELL(chill);
BSPELL(drown);
BSPELL(poison);
BSPELL(invigorate);
BSPELL(storm);
BSPELL(dart_of_air);
BSPELL(call_frost);
BSPELL(shaft_of_frost);
BSPELL(wind_armor);
BSPELL(confusion);
BSPELL(slow);
BSPELL(unbalance);
BSPELL(create_food);
BSPELL(explosion);
BSPELL(dome_of_frost);
BSPELL(fist_of_air);
BSPELL(firestorm);
BSPELL(refresh);
BSPELL(earth_armor);
BSPELL(destroy_armor);
BSPELL(fire_arrows);
BSPELL(earth_missile);
BSPELL(kinheal);
BSPELL(shield_of_saidin);
BSPELL(cripple);
BSPELL(deaf);
BSPELL(heal_II);
BSPELL(compulsion);
BSPELL(stormshield);
BSPELL(watershield);
BSPELL(stormsfury);
BSPELL(thunderburst);
BSPELL(group_restore);
BSPELL(ward_human);
BSPELL(fog);
BSPELL(identify);
BSPELL(entangle);
BSPELL(mend);
BSPELL(frostbite);
BSPELL(pain);
BSPELL(sanctuary);
BSPELL(roomseal);
BSPELL(sedaibond);
BSPELL(ashabond);
BSPELL(wrath);

/*
 * Funcs
 */
extern int gen_message(int type,struct char_data *ch,struct char_data *vict,int target,int success);
extern int slice_weave_failed(struct char_data *ch, struct char_data *victim, int mana_add, int spellnum);
extern void perform_unlink(struct char_data *ch);
extern void add_to_link(struct char_data *ch, struct char_data *vict);
extern unsigned long add_taint(struct char_data *ch, long amount);
extern void mag_assign_spells(void);
extern int IntBonus(struct char_data *ch);
extern int normal_spell_test(int type, struct char_data *ch, struct char_data *victim,int mana, int bonus);
extern int calc_weave_bonus(struct char_data *ch,struct char_data *vict);
extern int item_to_destroy(struct char_data *ch,int *dam_mult);


/*
 * Data
 */
extern char *spells[];
extern struct spell_info_type spell_info[TOP_SPELL_DEFINE + 1];

extern void check_if_learned(struct char_data *ch,int spell);

#endif
