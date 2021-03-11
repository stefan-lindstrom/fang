/* ************************************************************************
*   File: utils.h                                       Part of CircleMUD *
*  Usage: header file: utility macros and prototypes of utility funcs     *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#ifndef __UTILS_H__
#define __UTILS_H__

#include "structs.h"
/* external declarations and prototypes **********************************/

extern struct weather_data weather_info;
extern int exp_needed_per_level[NUM_CLASSES];

#undef log

#ifdef CIRCLE_WINDOWS
#define mlog(x) basic_mud_log(x)
#endif

//#define strdup(string) str_dup(string)

/* public functions in utils.c */
char	*str_dup(const char *source);
int     c_strlen(const char *arg);
int	str_cmp(const char *arg1, const char *arg2);
int	strn_cmp(const char *arg1, const char *arg2, int n);
void	mlog(char *str);
int	touch(char *path);
void    xlog(int type, int level, char *str, byte file);
void axlog( int type, int level, byte file, char *fmt,...);
void	mudlog(char *str, char type, int level, byte file);
int check_and_notify_non_IC_receiver(struct char_data *actor, struct char_data *receiver, char *verb);
void	log_death_trap(struct char_data *ch);
int	number(int from, int to);
int	dice(int number, int size);
void	sprintbit(long vektor, const char *names[], char *result);
void	sprinttype(int type, const char *names[], char *result);
int	get_line(FILE *fl, char *buf);
int	get_filename(char *orig_name, char *filename, int mode);
struct time_info_data age(struct char_data *ch);
int	num_pc_in_room(struct room_data *room);
int     replace_str(char **string, char *pattern, char *replacement, int rep_all, int max_size);
void    format_text(char **ptr_string, int mode, struct descriptor_data *d, int maxlen);
void    alog(char *fmt,...);
int     index_from_array(char *key, const char **array, char *endtag, int numInArray);
long    count_newlines(char **ptr_str);
char	*coltext(char *source, struct char_data * ch);
void    add_eavesdropper(struct char_data *ch, long rnum, int dist);
void    rem_eavesdropper(struct char_data *ch, long rnum);
void    to_eavesdropper(char *tosend, long rnum);
void    rem_playerxdesc(struct player_extradesc *xdesc, struct char_data *ch);
void    add_playerxdesc(struct player_extradesc *xdesc, struct char_data *ch);
struct  player_extradesc *find_playerxdesc(char *keywd, int dnum, struct char_data *ch);
void    nhlog(char *str);

/* undefine MAX and MIN so that our functions are used instead */
#ifdef MAX
#undef MAX
#endif

#ifdef MIN
#undef MIN
#endif

int MAX(int a, int b);
int MIN(int a, int b);

#define SMIN(a,b) MIN(strlen(a),strlen(b))

/* in magic.c */
bool	circle_follow(struct char_data *ch, struct char_data * victim);

/* in act.informative.c */
void	look_at_room(struct char_data *ch, int mode);

/* in act.movmement.c */
int	do_simple_move(struct char_data *ch, int dir, int following);
int	perform_move(struct char_data *ch, int dir, int following);

/* in limits.c */
int	mana_limit(struct char_data *ch);
int	hit_limit(struct char_data *ch);
int	move_limit(struct char_data *ch);
int	mana_gain(struct char_data *ch);
int	hit_gain(struct char_data *ch);
int	move_gain(struct char_data *ch);
void	advance_level(struct char_data *ch);
void	set_title(struct char_data *ch, char *title);
void	gain_exp(struct char_data *ch, int gain,struct char_data *vict);
void	gain_exp_regardless(struct char_data *ch, int gain);
int calc_exp(struct char_data * ch, int gain,struct char_data *vict);
void	gain_condition(struct char_data *ch, int condition, int value);
int	check_idling(struct char_data *ch);
void	point_update(void);
void	update_pos(struct char_data *victim);

extern int dummy_int[MAX_SKILLS+1];

/* various constants *****************************************************/


/* defines for mudlog() */
#define OFF	0
#define BRF	1
#define NRM	2
#define CMP	3

/* get_filename() */
#define CRASH_FILE	 0
#define ETEXT_FILE	 1
#define ALIAS_FILE	 2
#define DREAM_FILE       3
#define old_HORSE_FILE       4 // Redundant now.
#define PLR_FILE	 5
#define HOUSE_FILE	 6
#define KNOW_FILE	 7
#define SCRIPT_VARS_FILE 8
#define OLD_PLR_FILE	 9
#define RETAINER_FILE	10
/* breadth-first searching */
#define BFS_ERROR		-1
#define BFS_ALREADY_THERE	-2
#define BFS_NO_PATH		-3

/* mud-life time */
#define SECS_PER_MUD_HOUR	75
#define SECS_PER_MUD_DAY	(24*SECS_PER_MUD_HOUR)
#define SECS_PER_MUD_MONTH	(35*SECS_PER_MUD_DAY)
#define SECS_PER_MUD_YEAR	(17*SECS_PER_MUD_MONTH)

/* real-life time (remember Real Life?) */
#define SECS_PER_REAL_MIN	60
#define SECS_PER_REAL_HOUR	(60*SECS_PER_REAL_MIN)
#define SECS_PER_REAL_DAY	(24*SECS_PER_REAL_HOUR)
#define SECS_PER_REAL_YEAR	(365*SECS_PER_REAL_DAY)


/* string utils **********************************************************/


#define YESNO(a) ((a) ? "YES" : "NO")
#define ONOFF(a) ((a) ? "ON" : "OFF")

#define LOWER(c)   (((c)>='A'  && (c) <= 'Z') ? ((c)+('a'-'A')) : (c))
#define UPPER(c)   (((c)>='a'  && (c) <= 'z') ? ((c)+('A'-'a')) : (c) )

#define ISNEWL(ch) ((ch) == '\n' || (ch) == '\r') 
#define IF_STR(st) ((st) ? (st) : "\0")
#define CAP(st)  (*(st) = UPPER(*(st)), st)

#define AN(string) (strchr("aeiouAEIOU", *string) ? "an" : "a")


/* memory utils **********************************************************/


#define CREATE(result, type, number)				      \
  { (result) = (type *)calloc( (number), sizeof(type));		      \
    if( !(result) )						      \
    { xlog( SYS_ERROR, LVL_GOD, "CREATE failure", TRUE ); abort(); }  \
  }

//maybe add "strerror( errno )" into the messages

#define RECREATE(result,type,number)					\
  { (result) = (type *)realloc( (result), sizeof(type) * (number));	\
   if ( !(result) )							\
    { xlog( SYS_ERROR, LVL_GOD, "RECREATE failure", TRUE ); abort(); }	\
  }

#define FREEPTR(ptr) { if (ptr) { free(ptr); ptr = NULL; } }

/*
 * the source previously used the same code in many places to remove an item
 * from a list: if it's the list head, change the head, else traverse the
 * list looking for the item before the one to be removed.  Now, we have a
 * macro to do this.  To use, just make sure that there is a variable 'temp'
 * declared as the same type as the list to be manipulated.  BTW, this is
 * a great application for C++ templates but, alas, this is not C++.  Maybe
 * CircleMUD 4.0 will be...
 */
#define REMOVE_FROM_LIST(item, head, next)	\
   if ((item) == (head))		\
      head = (item)->next;		\
   else {				\
      temp = head;			\
      while (temp && (temp->next != (item))) \
	 temp = temp->next;		\
      if (temp)				\
         temp->next = (item)->next;	\
   }					\


/* basic bitvector utils *************************************************/

#define IS_SET(flag,bit)  ((flag) & (bit))
#define SET_BIT(var,bit)  ((var) |= (bit))
#define REMOVE_BIT(var,bit)  ((var) &= ~(bit))
#define TOGGLE_BIT(var,bit) ((var) = (var) ^ (bit))

#define MOB_FLAGS(ch)  ((ch)->char_specials.saved.act)
#define MOB_SKILLS(ch) ((ch)->mob_specials.skillvector)
#define PLR_FLAGS(ch)  ((ch)->char_specials.saved.plr)
#define PRF_FLAGS(ch)  ((ch)->player_specials->saved.pref)
#define PRF2_FLAGS(ch) ((ch)->player_specials->saved.pref2)
#define AFF_FLAGS(ch)  ((ch)->char_specials.saved.affected_by)
#define AFF2_FLAGS(ch) ((ch)->char_specials.saved.affected2)
#define AFF3_FLAGS(ch) ((ch)->char_specials.saved.affected3)
#define AUTO_FLAGS(ch) ((ch)->player_specials->saved.AutoFlags)
#define SESS_FLAGS(ch) ((ch)->player_specials->saved.sess)

#define EXTENDED_SYSLOG(ch)	((ch)->player_specials->saved.syslog)
#define GOT_SYSLOG(ch, flag)	(!IS_NPC(ch) && IS_SET(EXTENDED_SYSLOG(ch), (flag)))
#define SYSL_TOG_CHK(ch, flag)	((TOGGLE_BIT(EXTENDED_SYSLOG(ch), (flag))) & (flag))

#define ROOM_FLAGS(loc)      (world[(loc)].room_flags)
#define ROOM_SECTOR(loc)     (world[(loc)].sector_type)
#define ROOM_TEMP_FLAGS(loc) (world[(loc)].temp_flags)
#define ROOM_AFF(room)       (world[(room)].aff_flags)
#define OBJ_EXTRA_FLAGS(obj) ((obj)->obj_flags.extra_flags)

#define IS_NPC(ch)  (IS_SET(MOB_FLAGS(ch), MOB_ISNPC))
#define IS_MOB(ch)  (IS_NPC(ch) && ((ch)->nr >-1))

#define MOB_FLAGGED(ch, flag)   (IS_NPC(ch) && IS_SET(MOB_FLAGS(ch), (flag)))
#define PLR_FLAGGED(ch, flag)   (!IS_NPC(ch) && IS_SET(PLR_FLAGS(ch), (flag)))
#define AFF_FLAGGED(ch, flag)   (IS_SET(AFF_FLAGS(ch), (flag)))
#define AFF2_FLAGGED(ch, flag)  (IS_SET(AFF2_FLAGS(ch), (flag)))
#define AFF3_FLAGGED(ch, flag)  (IS_SET(AFF3_FLAGS(ch), (flag)))
#define PRF_FLAGGED(ch, flag)   (IS_SET(PRF_FLAGS(ch), (flag)))
#define PRF2_FLAGGED(ch, flag)  (IS_SET(PRF2_FLAGS(ch), (flag)))
#define AUT_FLAGGED(ch, flag)   (IS_SET(AUTO_FLAGS(ch),(flag)))
#define ROOM_FLAGGED(loc, flag) (IS_SET(ROOM_FLAGS(loc), (flag)) || IS_SET(ROOM_TEMP_FLAGS(loc), (flag)))
#define SESS_FLAGGED(ch, flag)  (IS_SET(SESS_FLAGS(ch), (flag)))

/* IS_AFFECTED for backwards compatibility */
#define IS_AFFECTED(ch, skill)  (AFF_FLAGGED((ch), (skill)))
#define IS_AFFECTED2(ch, skill) (AFF2_FLAGGED((ch), (skill)))
#define IS_AFFECTED3(ch, skill) (AFF3_FLAGGED((ch), (skill)))
#define ROOM_AFFECTED(room,type) (IS_SET(ROOM_AFF(room),type))

#define PLR_TOG_CHK(ch,flag) (TOGGLE_BIT(PLR_FLAGS(ch), (flag)) & (flag))
#define PRF2_TOG_CHK(ch,flag) (TOGGLE_BIT(PRF2_FLAGS(ch), (flag)) & (flag))
#define PRF_TOG_CHK(ch,flag) (TOGGLE_BIT(PRF_FLAGS(ch), (flag)) & (flag))
#define AUT_TOG_CHK(ch,flag) (TOGGLE_BIT(AUTO_FLAGS(ch), (flag)) & (flag))
#define SESS_TOG_CHK(ch, flag) (TOGGLE_BIT(SESS_FLAGS(ch), (flag)) & (flag))

#define NORMAL_DREAMER(ch) (IS_AFFECTED((ch),AFF_DREAMING) && !IS_AFFECTED2((ch),AFF2_TAR_GATE))
#define FLESH_DREAMER(ch)  (IS_AFFECTED((ch),AFF_DREAMING) && IS_AFFECTED2((ch),AFF2_TAR_GATE))

/* room utils ************************************************************/


#define SECT(room)	(world[(room)].sector_type)

#define IS_DARK(room)  ( ROOM_AFFECTED(room,RAFF_DARKNESS) || \
                         (!world[room].light && \
                         (ROOM_FLAGGED(room, ROOM_DARK) || \
                          ( ( SECT(room) != SECT_INSIDE && \
                              SECT(room) != SECT_CITY ) && \
                            (weather_info.sunlight == SUN_SET || \
			     weather_info.sunlight == SUN_DARK)) ) ))

#define IS_LIGHT(room)  (ROOM_AFFECTED(room,RAFF_LIGHT) || \
                           !IS_DARK(room))

#define GET_ROOM_SPEC(room) ((room) >= 0 ? world[(room)].func : NULL)

/* char utils ************************************************************/

#define GET_NAME(ch)	 (IS_NPC(ch) ? (ch)->player.short_descr : \
                         (GET_DISGUISED(ch) ? GET_DNAME(ch)  : GET_RNAME(ch)))
#define GET_KWS_CH(ch)   (IS_NPC(ch) ? (ch)->player.name : GET_NAME(ch))
#define GET_DESC(ch) 	 (GET_RDESC(ch))  
#define GET_TITLE(ch)    (GET_DISGUISED(ch) ? GET_DTITLE(ch) : GET_RTITLE(ch))
#define GET_CPROMPT(ch)  ((ch)->player.custom_prompt)
#define GET_NOBILITY(ch) (GET_DISGUISED(ch) ? GET_DNOBILITY(ch): GET_RNOBILITY(ch))

#define IN_ROOM(ch)	   ((ch)->in_room)
#define GET_WAS_IN(ch)	   ((ch)->was_in_room)
#define GET_MOBPROG(ch)    ((ch)->mprog)
#define GET_OBJPROG(obj)   ((obj)->oprog)
#define GET_ROOMPROG(room) (world[(room)].rprog)

#define GET_AGE(ch)     (age(ch).year)

#define GET_RNAME(ch)     (IS_NPC(ch) ? \
			    (ch)->player.short_descr : (ch)->player.name)
#define GET_RTITLE(ch)      ((ch)->player.title)
#define GET_LEVEL(ch)       ((ch)->player.level)
#define IS_NEWBIE(ch)       (GET_LEVEL((ch)) <= 20 ? 1 : 0)
#define GET_CURRENTDESC(ch) ((ch)->player.current_desc)
#define GET_DESCNUM(ch,num) ((ch)->player.descriptions[(num)])
#define GET_RDESC(ch)       ((ch)->player.descriptions[GET_CURRENTDESC((ch))])
#define GET_PLAYERXDESC(ch) ((ch)->player.xdesc)
#define GET_CONTACTINFO(ch) ((ch)->player.contactinfo)
#define GET_BACKGROUND(ch)  ((ch)->player.background)
#define GET_PRETITLE(ch)    ((ch)->player.pretitle)
#define GET_MYCOLOR(ch)     ((ch)->player.mycolor ? (ch)->player.mycolor : "&n")
#define GET_PASSWD(ch)	    ((ch)->player.passwd)
#define GET_PFILEPOS(ch)    ((ch)->pfilepos)
#define GET_QUEST_TYPE(ch)  (aquest_table[real_quest((int)GET_QUEST((ch)))].type)
#define GET_QUEST(ch)	    ((ch)->player_specials->saved.current_quest) 

#define GET_RNOBILITY(ch) ((ch)->player.nobility)
#define GET_BM(ch)        ((ch)->player_specials->saved.blademastery)
#define GET_SUBDUE(ch)    ((ch)->player_specials->saved.subdue)
#define CHAR_SCENE(ch)	  ((ch)->player_specials->scene)
#define ROOM_SCENE(r)	  ((r)->scene)
#define ROOM_NO_SCENE(r)  (world[(r)].scene)
#define CLEAR_SCENE(b)	  { SCENE_DATA *c=(b);			    \
			    if(c->text) free(c->text);		    \
			    if(c->owner) CHAR_SCENE(c->owner)=NULL; \
			    if(c->room) ROOM_SCENE(c->room)=NULL;   \
			    free(c);}

#define POS(value)   	  ((value) / 32) /* Gives pos in array */
#define BITPOS(value)	  (1 << ((value) % 32)) /* Gives bit in pos   */

#define LIMIT(var, min, max)    (MAX(min, MIN(var, max)))

/*
 * I wonder if this definition of GET_REAL_LEVEL should be the definition
 * of GET_LEVEL?  JE
 */
#define GET_REAL_LEVEL(ch) \
   (ch->desc && ch->desc->original ? GET_LEVEL(ch->desc->original) : \
    GET_LEVEL(ch))

#define GET_CLASS(ch)   ((ch)->player.class)
#define GET_HOME(ch)	((ch)->player.hometown)
#define GET_HEIGHT(ch)	((ch)->player.height)
#define GET_WEIGHT(ch)	((ch)->player.weight)
#define GET_SEX(ch)	((ch)->player.sex)

#define GET_LEVEL_EXP(ch) (exp_needed_per_level[(int) GET_CLASS(ch)])

#define GET_STR(ch)     ((ch)->aff_abils.str)
#define GET_DEX(ch)     ((ch)->aff_abils.dex)
#define GET_INT(ch)     ((ch)->aff_abils.intel)
#define GET_WIS(ch)     ((ch)->aff_abils.wis)
#define GET_CON(ch)     ((ch)->aff_abils.con)
#define GET_CHA(ch)     ((ch)->aff_abils.cha)

#define GET_EXP(ch)	  ((ch)->points.exp)
#define GET_REAL_AC(ch)   ((ch)->points.armor)
#define GET_AC(ch) (LIMIT((ch)->points.armor, -600, 100))
#define GET_HIT(ch)	  ((ch)->points.hit)
#define GET_MAX_HIT(ch)	  ((ch)->points.max_hit)
#define GET_FRACT_MOVE(ch) ((ch)->char_specials.fract_move)
#define GET_MOVE(ch)	  ((ch)->points.move)
#define GET_MAX_MOVE(ch)  ((ch)->points.max_move)

extern void SET_MANA(struct char_data *ch, long val);
extern long GET_MANA(struct char_data *ch);
extern void ADD_MANA(struct char_data *ch, long val);

#define GET_MAX_MANA(ch)  (!IS_NPC(ch) ? ((ch)->player_specials->linking[0] ? \
                             (ch)->player_specials->max_manalinked : \
                             (ch)->points.max_mana): (ch)->points.mana)
#define GET_GOLD(ch)	  ((ch)->points.gold)      
#define GET_BANK_GOLD(ch) ((ch)->points.bank_gold) 
#define GET_HITROLL(ch)	  ((ch)->points.hitroll)
#define GET_DAMROLL(ch)   ((ch)->points.damroll)

#define GET_POS(ch)	  ((ch)->char_specials.position)
#define GET_SEAT(ch)      ((ch)->char_specials.sitting_on)
#define GET_IDNUM(ch)	  ((ch)->char_specials.saved.idnum)
#define IS_CARRYING_W(ch) ((ch)->char_specials.carry_weight)
#define IS_CARRYING_N(ch) ((ch)->char_specials.carry_items)

#define FIGHTING(ch)	  ((ch)->char_specials.fighting)
#define HUNTING(ch)	  ((ch)->char_specials.hunting)
#define TALKING(ch)  	  ((ch)->char_specials.talking)

#define GET_COND(ch, i)		((ch)->player_specials->saved.conditions[(i)])
#define GET_LOADROOM(ch)	((ch)->player_specials->saved.load_room)
#define GET_PRACTICES(ch)	((ch)->player_specials->saved.spells_to_learn)
#define GET_INVIS_LEV(ch)	((ch)->player_specials->saved.invis_level)
#define GET_GUILD_INVIS(ch)     ((ch)->player_specials->saved.ginvis)
#define GET_WIMP_LEV(ch)	((ch)->player_specials->saved.wimp_level)
#define GET_FREEZE_LEV(ch)	((ch)->player_specials->saved.freeze_level)
#define GET_BAD_PWS(ch)		((ch)->player_specials->saved.bad_pws)
#define GET_TALK(ch, i)		((ch)->player_specials->saved.talks[i])
#define POOFIN(ch)		((ch)->player_specials->poofin)
#define POOFOUT(ch)		((ch)->player_specials->poofout)
#define GET_LAST_OLC_TARG(ch)	((ch)->player_specials->last_olc_targ)
#define GET_LAST_OLC_MODE(ch)	((ch)->player_specials->last_olc_mode)
#define GET_ALIASES(ch)		((ch)->player_specials->aliases)
#define GET_LAST_TELL(ch)	((ch)->player_specials->last_tell)
#define GET_REPLYTO(ch)         ((ch)->player_specials->saved.replyto)
#define GET_EAVESDROPROOM(ch)   ((ch)->player_specials->saved.eavesroom)
#define GET_NUMMAPPED(ch)       ((ch)->player_specials->saved.nummapped)
#define GET_MAPPED(ch, i)       ((ch)->player_specials->saved.mapped[i])
#define GET_SLEEPROOM(ch)	((ch)->player_specials->sleeproom)
#define GET_SLEEPMOB(ch)        ((ch)->player_specials->sleepmob)
#define GET_DREAMPULLED(ch) ((ch)->player_specials->dreampulled)
#define GET_DREAMENTERED(ch) ((ch)->player_specials->dreamentered)
#define GET_KNOW(ch)		((ch)->player_specials->known_rooms)

#define GET_LONGING(ch)         ((ch)->player_specials->not_in_stedding)
#define LONGING_NOGAIN          1200 /* 10 hrs outside stedding, slow gain */

#define SPEAKING(ch)		((ch)->player_specials->speaking)
#define DICING(ch)		((ch)->player_specials->dicer)
#define DICEBET(ch)		((ch)->player_specials->bet)
#define GET_DEST(ch)		((ch)->char_specials.goingto)
#define GET_TRAVEL_TO(ch)	((ch)->player_specials->travellingto)
#define GET_MAINLIST(ch)	((ch)->player_specials->mob_prog_list)
#define GET_SIDELIST(ch)	((ch)->player_specials->mob_st_list)
#define GET_ACTLIST(ch)		((ch)->player_specials->mob_act_list)
#define GET_OBJ_MAINLIST(ch)    ((ch)->player_specials->opl)
#define GET_OBJ_ACTLIST(ch)     ((ch)->player_specials->oal)
#define GET_DOING(ch)		((ch)->player_specials->doing)
#define GET_LEAVE(ch)		((ch)->player_specials->leave)
#define GET_ARRIVE(ch)		((ch)->player_specials->arrive)
#define GET_PREFIX(ch)		((ch)->player_specials->prefix)
#define GET_INC_TIME(ch,skill)	((ch)->player_specials->timeincrease[(skill)])
#define GET_GAME(ch)		((ch)->player_specials->game)
#define GET_PRIVATE(ch)		((ch)->player_specials->private)
#define GET_NO_INC(ch,skill)	((ch)->player_specials->noincrease[POS(skill)])
#define SET_NOINC(ch,skill)     SET_BIT(GET_NO_INC((ch),(skill)),BITPOS(skill))
#define CLEAR_NOINC(ch,skill)   REMOVE_BIT(GET_NO_INC((ch),(skill)),BITPOS(skill))
#define NOINC_SET(ch,skill)     IS_SET(GET_NO_INC((ch),(skill)),BITPOS(skill))
#define GET_R_IDLE(ch)          ((ch)->player_specials->room_idle)
#define GET_LAST_EMOTE(ch)      ((ch)->player_specials->saved.last_emote_counter)
#define GET_AUTHORIZED(ch)      ((ch)->player_specials->saved.unauthorized)
#define TICKS_IN_ROOM(ch)       ((ch)->player_specials->ticks_in_room)
#define GET_NO_LOCATE(ch)       ((ch)->player_specials->saved.nolocate)
#define GET_WHEEL_BET(ch)	((ch)->player_specials->wheel_bet)
#define GET_WHEEL_NR(ch)	((ch)->player_specials->wheel_nr)
#define GET_TRAINS(ch)    	((ch)->player_specials->saved.trains)
#define GET_QP(ch) 		((ch)->player_specials->saved.questpoints)
#define GET_AIR(ch)             ((ch)->player_specials->saved.air)
#define GET_EARTH(ch)		((ch)->player_specials->saved.earth)
#define GET_FIRE(ch)		((ch)->player_specials->saved.fire)
#define GET_SPIRIT(ch)		((ch)->player_specials->saved.spirit)
#define GET_WATER(ch)		((ch)->player_specials->saved.water)
#define GET_TEACHABLE(ch)	((ch)->player_specials->saved.teachable)
#define GET_SPARK(ch)		((ch)->player_specials->saved.spark)
#define GET_POWER(ch) 		((ch)->player_specials->saved.powerpoints)
#define GET_TAINT(ch)		((ch)->player_specials->saved.taint)
#define GET_TAVEREN(ch)		((ch)->player_specials->saved.taveren)
#define GET_WOLFKIN(ch)		((ch)->player_specials->saved.wolfkin)
#define BETTING(ch)		((ch)->player_specials->saved.betting)
#define GET_BONDED(ch) 		((ch)->player_specials->saved.bonded)
#define GET_MINION(ch) 		((ch)->player_specials->saved.minion)
#define GET_ASHABONDED(ch)      ((ch)->player_specials->saved.ashabonded)
#define GET_AUTOQUEST(ch) 	((ch)->player_specials->saved.AutoQuest[0])
#define GET_AFW(ch)		((ch)->player_specials->afk_String)
#define GET_AFK(ch)		((ch)->player_specials->afk_String)

extern long GET_TIMES(struct char_data *ch, int skill);
extern void SET_TIMES(struct char_data *ch, int skill, long value);

#define GET_RACE(ch)            ((ch)->player_specials->saved.race)
#define GET_AJAH(ch)            ((ch)->player_specials->saved.ajah)
#define old_GET_MOUNTS(ch)		((ch)->player_specials->mounts)
#define GET_RIDING(ch)		((ch)->player_specials->riding)
#define GET_SERVANT(ch)		((ch)->player_specials->saved.servant)
#define GET_COMPULSION(ch)      ((ch)->player_specials->saved.compulsion)

#define IS_PLAYING(ch)		((ch)->player_specials->saved.playing)
#define RECRUIT_WAIT(ch)	((ch)->player_specials->saved.recruit_wait)
#define TAUGHT(ch)		((ch)->player_specials->saved.taught)
#define GET_NOHIDE(ch)		((ch)->player_specials->saved.nohide)
#define GET_LINES(ch)		((ch)->player_specials->saved.lines)
#define HPTAUGHT(ch)            ((ch)->player_specials->saved.hptaught)

#define IS_BRANDED(ch)		 ((ch)->player_specials->saved.branded)
#define GET_WAGON(ch)		((ch)->player_specials->saved.wagon)
#define GET_HOUSE(ch, i)        ((ch)->player_specials->saved.houses[(i)])
#define GET_ISWOLFKIN(ch)       ((ch)->player_specials->saved.wolfkinT)
#define GET_WKRANK(ch)          ((ch)->player_specials->saved.wkrank)
#define GET_INVSTART(ch)        ((ch)->player_specials->saved.invstart)
#define GET_OGIER(ch)		((ch)->player_specials->saved.ogier)
#define GET_PRIMTALENTS1(ch)      ((ch)->player_specials->saved.primtal1)
#define GET_PRIMTALENTS2(ch)      ((ch)->player_specials->saved.primtal2)
#define GET_SECTALENTS1(ch)       ((ch)->player_specials->saved.spectal1)
#define GET_SECTALENTS2(ch)       ((ch)->player_specials->saved.spectal2)
#define GET_SPSKILLS1(ch)         ((ch)->player_specials->saved.specskills1)
#define GET_SPSKILLS2(ch)         ((ch)->player_specials->saved.specskills2)

#define GET_RSTR(ch) 		((ch)->real_abils.str)
#define GET_RCON(ch) 		((ch)->real_abils.con)
#define GET_RDEX(ch) 		((ch)->real_abils.dex)
#define GET_RINT(ch) 		((ch)->real_abils.intel)
#define GET_RWIS(ch) 		((ch)->real_abils.wis)
#define GET_RCHA(ch) 		((ch)->real_abils.cha)
#define GET_ABS(ch)		((ch)->player_specials->saved.absorbation)
#define GET_PERMANENT(ch)	((ch)->player_specials->saved.permanent)
#define GET_DFINTR(ch)          ((ch)->player_specials->saved.drf_flags)
#define GET_DISGUISED(ch)	((ch)->player_specials->is_disguised)
#define GET_DNAME(ch)		((ch)->player_specials->real_name)
#define GET_DDESC(ch)		((ch)->player_specials->real_desc)
#define GET_DTITLE(ch)		((ch)->player_specials->real_title)
#define GET_DNOBILITY(ch)	((ch)->player_specials->real_nobility)
#define GET_CRAFT_OBJ(ch)       ((ch)->player_specials->craft)
#define GET_CRAFT_STATE(ch)	((ch)->player_specials->craft_state)
#define GET_CRAFT_TYPE(ch)      ((ch)->player_specials->obj_type)
#define GET_CRAFT_EDESC(ch)     ((ch)->player_specials->tl)
#define GET_XATTACKS(ch)	((ch)->player_specials->xattacks)

extern byte GET_SKILL(struct char_data *ch, int skill);
extern void SET_SKILL(struct char_data *ch, int skill, byte value);
extern void ADD_SKILL(struct char_data *ch, int skill, byte toAdd);

#define GET_LASTEMOTESTR(ch)	((ch)->player_specials->lastemotestr)
#define GET_LASTEMOTETYPE(ch)	((ch)->player_specials->lastemotetype)
#define GET_EQ(ch, i)		((ch)->equipment[i])

#define GET_MOB_SPEC(ch) (IS_MOB(ch) ? (mob_index[(ch->nr)].func) : NULL)
#define GET_MOB_RNUM(mob)	((mob)->nr)
#define GET_MOB_VNUM(mob)	(IS_MOB(mob) ? \
				 mob_index[GET_MOB_RNUM(mob)].virtual : -1)

#define GET_MOB_WAIT(ch)	((ch)->mob_specials.wait_state)
#define GET_DEFAULT_POS(ch)	((ch)->mob_specials.default_pos)
#define MEMORY(ch)		((ch)->mob_specials.memory)
#define GET_DREAMER(ch)       	((ch)->mob_specials.dreamer)
#define GET_TIMER(ch)		((ch)->mob_specials.timer)
#define GET_MOB_MODE(ch)	((ch)->mob_specials.mode)
#define GET_MP_ACTIVE(ch)   	((ch)->mob_specials.prog_activated)
#define GET_MOB_GOINGTO(ch)	((ch)->mob_specials.target_room)

#define CAN_CARRY_W(ch) (200+10*GET_LEVEL(ch))
#define CAN_CARRY_N(ch) (5 + (GET_DEX(ch) >> 1) + (GET_LEVEL(ch) >> 1))
#define AWAKE(ch) (GET_POS(ch) > POS_SLEEPING)
#define CAN_SEE_IN_DARK(ch) \
   (AFF_FLAGGED(ch, AFF_INFRAVISION) || \
    PRF_FLAGGED(ch, PRF_HOLYLIGHT) || \
    GET_WOLFKIN(ch))

#define IS_IMMORTAL(ch)         (GET_LEVEL(ch) >= LVL_IMMORT)

#define GET_MSTATUS(m,num)	 ((m)->mob_specials.status[(num)])
#define SET_MSTATUS(m,num,val)   (((m)->mob_specials.status[(num)]) = (val))
#define GET_OSTATUS(obj,num)	 ((obj)->status[(num)])
#define SET_OSTATUS(obj,num,val) (((obj)->status[(num)]) = (val))
#define GET_RSTATUS(r,num)	 (world[(r)].status[(num)])
#define SET_RSTATUS(r,val,num)   (world[(r)].status[(num)] = (val))

#define GET_ID(x)         ((x)->id) /* For DGS */

/* descriptor-based utils ************************************************/


#define WAIT_STATE(ch, cycle) { \
	if ((ch)->desc) (ch)->desc->wait = (cycle); \
	else if (IS_NPC(ch)) GET_MOB_WAIT(ch) = (cycle); }

#define CHECK_WAIT(ch)	(((ch)->desc) ? ((ch)->desc->wait > 1) : 0)
#define STATE(d)	((d)->connected)


/* object utils **********************************************************/


#define GET_OBJ_TYPE(obj)	((obj)->obj_flags.type_flag)
#define GET_OBJ_COST(obj)	((obj)->obj_flags.cost)
#define GET_OBJ_QCOST(obj)	((obj)->obj_flags.questcost)
#define IS_QUEST_ITEM(obj)   	((obj)->obj_flags.questcost)
#define GET_OBJ_EXTRA(obj)	((obj)->obj_flags.extra_flags)
#define GET_OBJ_WEAR(obj)	((obj)->obj_flags.wear_flags)
#define GET_OBJ_VAL(obj, val)	((obj)->obj_flags.value[(val)])
#define GET_OBJ_WEIGHT(obj)	((obj)->obj_flags.weight)
#define GET_OBJ_TIMER(obj)	((obj)->obj_flags.timer)
#define GET_OBJ_RNUM(obj)	((obj)->item_number)
#define GET_OBJ_STATUS(obj)     ((obj)->obj_status)
#define GET_OP_ACTIVE(obj)	((obj)->op_activated)
#define GET_OBJ_VNUM(obj)	(GET_OBJ_RNUM(obj) >= 0 ? \
				 obj_index[GET_OBJ_RNUM(obj)].virtual : -1)
#define IS_OBJ_STAT(obj,stat)	(IS_SET((obj)->obj_flags.extra_flags,stat))

#define GET_OBJ_SPEC(obj) ((obj)->item_number >= 0 ? \
	(obj_index[(obj)->item_number].func) : NULL)

#define CAN_WEAR(obj, part) (IS_SET((obj)->obj_flags.wear_flags, (part)))


/* compound utilities and other macros **********************************/


#define HSHR(ch) (GET_SEX(ch) ? (GET_SEX(ch)==SEX_MALE ? "his":"her") :"its")
#define HSSH(ch) (GET_SEX(ch) ? (GET_SEX(ch)==SEX_MALE ? "he" :"she") : "it")
#define HMHR(ch) (GET_SEX(ch) ? (GET_SEX(ch)==SEX_MALE ? "him":"her") : "it")

#define ANA(obj) (strchr("aeiouyAEIOUY", *(obj)->name) ? "An" : "A")
#define SANA(obj) (strchr("aeiouyAEIOUY", *(obj)->name) ? "an" : "a")


/* Various macros building up to CAN_SEE */

#define LIGHT_OK(ch)	(!IS_AFFECTED((ch), AFF_BLIND) && \
(IS_LIGHT((ch)->in_room) || CAN_SEE_IN_DARK(ch)))

#define IS_GREYMAN(ch) (!IS_NPC(ch) && IS_SET(GET_DFINTR(ch),DRF_GREYMAN))
#define IS_GHOLAM(ch)  (!IS_NPC(ch) && IS_SET(GET_DFINTR(ch),DRF_GHOLAM))

#define INVIS_OK(sub, obj) ((!IS_AFFECTED((obj), AFF_INVISIBLE) || IS_AFFECTED(sub, AFF_DETECT_INVIS)) 

extern int share_guild(struct char_data *ch1, struct char_data *ch2); /* guild.c */
#define GUILD_INVIS_OK(sub, obj) (!( GET_GUILD_INVIS(obj) && !share_guild(sub, obj) ) ) 

#define SEE_HIDDEN_OK(sub, obj)  ((!IS_AFFECTED((obj), AFF_HIDE) && \
  !IS_AFFECTED((obj), AFF_CAMOUFLAGED)) || IS_AFFECTED(sub, AFF_SENSE_LIFE)) && !IS_GREYMAN(obj))
  
#define SEE_DREAMING_OK(sub, obj) ((!IS_AFFECTED(sub, AFF_DREAMING) && !IS_AFFECTED((obj), AFF_DREAMING)) || \
 (IS_AFFECTED(sub, AFF_DREAMING) && IS_AFFECTED((obj), AFF_DREAMING)))

#define SEE_DREAMING_OK_OBJ(sub,obj) ((!IS_AFFECTED(sub, AFF_DREAMING) && !IS_SET(GET_OBJ_EXTRA(obj),ITEM_TAR)) || \
	(IS_AFFECTED(sub, AFF_DREAMING) && IS_SET(GET_OBJ_EXTRA(obj),ITEM_TAR)))

#define MORT_CAN_SEE(sub, obj) (GUILD_INVIS_OK(sub, obj) && LIGHT_OK(sub) && INVIS_OK(sub, obj) && SEE_HIDDEN_OK(sub, obj) && SEE_DREAMING_OK(sub, obj))

#define MORT_CAN_SEE_INCLUDING_TAR(sub, obj) (GUILD_INVIS_OK(sub, obj) && LIGHT_OK(sub) && \
INVIS_OK(sub, obj) && SEE_HIDDEN_OK(sub, obj))

#define IMM_CAN_SEE(sub, obj) \
   (MORT_CAN_SEE(sub, obj) || PRF_FLAGGED(sub, PRF_HOLYLIGHT))

#define IMM_CAN_SEE_INCLUDING_TAR(sub, obj) \
   (MORT_CAN_SEE_INCLUDING_TAR(sub, obj) || PRF_FLAGGED(sub, PRF_HOLYLIGHT))

#define SELF(sub, obj)  ((sub) == (obj))

/* Can subject see character "obj"? */
#define CAN_SEE_CODE(sub, obj) (SELF(sub, obj) || \
   ((GET_REAL_LEVEL(sub) >= GET_INVIS_LEV(obj)) ))

#define CAN_SEE(sub, obj) (SELF(sub, obj) || \
   ((GET_REAL_LEVEL(sub) >= GET_INVIS_LEV(obj)) && IMM_CAN_SEE(sub, obj)))

#define CAN_SEE_INCLUDING_TAR(sub, obj) (SELF(sub, obj) || \
   ((GET_REAL_LEVEL(sub) >= GET_INVIS_LEV(obj)) && IMM_CAN_SEE_INCLUDING_TAR(sub, obj)))

/* End of CAN_SEE */


#define INVIS_OK_OBJ(sub, obj) \
  (!IS_OBJ_STAT((obj), ITEM_INVISIBLE) || IS_AFFECTED((sub), AFF_DETECT_INVIS))

#define MORT_CAN_SEE_OBJ(sub, obj) (LIGHT_OK(sub) && INVIS_OK_OBJ(sub, obj))

#define TAR_OK(sub, obj) ((IS_OBJ_STAT(obj, ITEM_TAR) && IS_AFFECTED((sub), AFF_DREAMING)) || \
 			 (!IS_OBJ_STAT(obj, ITEM_TAR) && !IS_AFFECTED((sub), AFF_DREAMING)))

#define CAN_SEE_OBJ(sub, obj) \
   ((MORT_CAN_SEE_OBJ(sub, obj) || PRF_FLAGGED((sub), PRF_HOLYLIGHT)) \
    && TAR_OK(sub, obj))

#define CAN_CARRY_OBJ(ch,obj)  \
   (((IS_CARRYING_W(ch) + GET_OBJ_WEIGHT(obj)) <= CAN_CARRY_W(ch)) &&   \
    ((IS_CARRYING_N(ch) + 1) <= CAN_CARRY_N(ch)))

#define CAN_GET_OBJ(ch, obj)   \
   (CAN_WEAR((obj), ITEM_WEAR_TAKE) && CAN_CARRY_OBJ((ch),(obj)) && \
    CAN_SEE_OBJ((ch),(obj)))


#define PERS(ch, vict)   (CAN_SEE(vict, ch) ? GET_NAME(ch) : "someone")
#define PERS_INCLUDING_TAR(ch, vict)   (CAN_SEE_INCLUDING_TAR(vict, ch) ? GET_NAME(ch) : "someone")

#define OBJS(obj, vict) (CAN_SEE_OBJ((vict), (obj)) ? \
	(obj)->short_description  : "something")

#define OBJN(obj, vict) (CAN_SEE_OBJ((vict), (obj)) ? \
	fname((obj)->name) : "something")


#define EXIT(ch, door)  (world[(ch)->in_room].dir_option[door])

#define CAN_GO(ch, door) (EXIT(ch,door) && \
			 (EXIT(ch,door)->to_room != NOWHERE) && \
			 !IS_SET(EXIT(ch, door)->exit_info, EX_CLOSED))


#define CLASS_ABBR(ch) (IS_NPC(ch) ? "---" : class_abbrevs[(int)GET_CLASS(ch)])

#define OUTSIDE(ch) ( !ROOM_FLAGGED((ch)->in_room, ROOM_INDOORS) || \
                     (SECT_INSIDE != ROOM_SECTOR((ch)->in_room)) )

#define SENDOK(ch)      (((ch)->desc || SCRIPT_CHECK((ch), MTRIG_ACT)) && \
                        (AWAKE(ch) || sleep) && \
                        !PLR_FLAGGED((ch), PLR_WRITING | PLR_MAILING))
/* for success! */
#define TOTAL_FUCKUP  0
#define TOTAL_FAILURE 1
#define SOME_SUCCESS  2
#define TOTAL_SUCCESS 3

#define RESULT_OK(r) ((TOTAL_SUCCESS == (r)) || (SOME_SUCCESS == (r)))

/* OS compatibility ******************************************************/


/* there could be some strange OS which doesn't have NULL... */
#ifndef NULL
#define NULL (void *)0
#endif

#if !defined(FALSE)
#define FALSE 0
#endif

#if !defined(TRUE)
#define TRUE  (!FALSE)
#endif

/* defines for fseek */
#ifndef SEEK_SET
#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2
#endif

#if defined(NOCRYPT) || !defined(HAVE_CRYPT)
#define CRYPT(a,b) (a)
#else
#define CRYPT(a,b) ((char *) crypt((a),(b)))
#endif

/*
 * Funcs
 */
extern void die_follower(struct char_data * ch, int quit);
extern int cant_channel(struct char_data *ch);
extern int can_channel_same_sex(struct char_data *ch,struct char_data *list);
extern int can_channel_not_same_sex(struct char_data *ch,struct char_data *list);
extern void add_follower(struct char_data * ch, struct char_data *leader);
extern int generic_result(struct char_data *ch,int skill_num,char *string,byte use_gen);
extern int generic_result_mod(struct char_data *ch,int skill_num,char *string,byte use_gen,int mod);
extern sh_int find_room_by_name(char *arg);
extern int number_of_players_in_room( int room_number );
extern void stop_follower(struct char_data * ch);
extern struct time_info_data mud_time_passed(time_t t2, time_t t1);
extern int xmlAtoi(char *string);
extern long xmlAtol(char *string);
extern double gaussrand(void);
extern unsigned int randPoisson(double dMean);
extern int gaussian_var(int med,int d,int l,int h);
extern struct time_info_data real_time_passed(time_t t2, time_t t1);

#endif

