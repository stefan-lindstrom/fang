/******************************************************************************
 * $Id: handler.c,v 1.18 2003/05/27 09:04:22 fang Exp $
 *   
 * File: handler.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: handler.c,v $
 *   Revision 1.18  2003/05/27 09:04:22  fang
 *   Plugged a memory leak
 *
 *   Revision 1.17  2003/05/14 09:15:05  fang
 *   Added simple regexp codo isname. Might need some tweaking.
 *
 *   Revision 1.16  2003/05/14 07:55:31  fang
 *   Added Improved Isname code --Tzeentch
 *
 *   Revision 1.15  2003/03/25 07:58:50  fang
 *   Some valgrind bugs fixed
 *
 *   Revision 1.14  2002/11/24 14:40:28  fang
 *   Once again messing with the same error, different approach this time, though.
 *
 *   Revision 1.13  2002/11/24 14:10:10  fang
 *   Fixed some spam caused by mailing.
 *
 *   Revision 1.12  2002/11/07 04:02:02  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.11  2002/11/02 05:27:19  fang
 *   fisch: clears afk/afw at exit
 *
 *   Revision 1.10  2002/10/29 03:09:26  fang
 *   fisch: added the command "scene".  Scenes are like doings for rooms.
 *
 *   Revision 1.9  2002/10/15 21:39:19  fang
 *   fisch: more info in the SYSERR about char_in_room for character in room
 *
 *   Revision 1.8  2002/10/03 20:06:22  fischer
 *   fischer: changed tells to only work on PCs and not NPCs
 *
 *   Revision 1.7  2002/09/09 04:26:13  fang
 *   Drath: Final bug fixes (we hope) for AEDIT crashes
 *
 *   Revision 1.6  2002/09/01 14:45:01  fang
 *   Put in a fix for retainers that are purged.
 *
 *   Revision 1.5  2002/08/30 21:23:09  fang
 *   Put in retainers.
 *
 *   Revision 1.4  2002/07/30 14:39:46  fang
 *   Drath: Another gskill update
 *
 *   Revision 1.3  2002/07/28 13:56:04  fang
 *   Removed dmlloc...just to test.
 *
 *   Revision 1.2  2002/07/26 19:39:52  fang
 *   Dath: MYCOLOR mem leak fixed.  Thanks, Fred!
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.7  2002/07/16 15:16:16  fang
 *   Drath: Seafolk Guildskills are done.
 *
 *   Revision 1.6  2002/06/06 13:06:15  fang
 *   *** empty log message ***
 *
 *   Revision 1.5  2002/05/27 10:52:49  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.4  2002/04/30 18:54:56  fang
 *   AC Cap - Request per CRE
 *
 *   Revision 1.3  2002/02/27 02:17:40  fang
 *   Fixed disguise so it saves.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.19  2002/01/24 21:33:17  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.18.2.2  2002/01/24 03:20:30  mud
 *   Removed some further unescessary files (player.[ch], roomprog.[ch]
 *   old2new.c). Cleaned up some other stuff (guild stuff mostly).
 *
 *   Revision 1.18.2.1  2002/01/22 20:50:59  mud
 *   Removed boats and info system. Obsolete, unused code with no value.
 *   Fixed bug in portal_stone SPECIAL. Now checks that a room exists
 *   before attempting to transf chars there. :)
 *
 *   Revision 1.18  2001/08/07 05:50:32  mud
 *   *** empty log message ***
 *
 *   Revision 1.17  2001/07/10 18:47:49  mud
 *   *** empty log message ***
 *
 *   Revision 1.16  2001/07/10 18:39:52  mud
 *   *** empty log message ***
 *
 *   Revision 1.15  2001/05/22 09:07:04  mud
 *   DG-Scripts for Fang.
 *
 *   Revision 1.14  2001/05/17 11:21:09  mud
 *   Final residues of old pfile code should be swept away by now. No cleansing of the structures and such things though
 *
 *   Revision 1.13  2001/03/17 10:40:46  mud
 *   New and Improved TAR.
 *
 *   Revision 1.12  2000/11/28 08:48:24  mud
 *   XML char save/load owrks. As does ptable building. Conversions looks fine and dandy too, although there's a few minor things to polish
 *
 *   Revision 1.11  2000/10/26 01:04:08  mud
 *   fix to extract_obj() for furniture in inventory
 *
 *   Revision 1.10  2000/10/09 10:29:49  mud
 *   added 'showmail' command. shows the date and sender of mail in your inventory
 *
 *   Revision 1.9  2000/08/21 03:27:07  mud
 *   *** empty log message ***
 *
 *   Revision 1.8  2000/07/22 02:12:55  mud
 *   Fixed problem with objs being found -still- by their s-desc of l-desc..isntead of
 *   just the alias list....
 *
 *   Revision 1.7  2000/04/18 00:47:55  mud
 *   Minor fix to club code (do_reject)
 *
 *   Revision 1.6  2000/04/09 20:34:04  mud
 *   Changed player obj saving to XML format.
 *
 *   Revision 1.5  2000/03/31 07:06:10  mud
 *   Fixed 'name_matches' to -not- use a mob's l-desc!
 *
 *   Revision 1.4  2000/03/10 18:26:41  mud
 *   Added a game of cards, which includes two new files,
 *   cards.c and cards.h, and modifying of a bunch of others.
 *   Note: xmlcharsve.x shouldn't be here, they just sort of tagged along.
 *
 *   Revision 1.3  2000/03/02 17:04:02  mud
 *   Furniture bug: When pruging a furn.obj, all its 'content' will be removed, hence ridding of us some possible crashbugs
 *
 *   Revision 1.2  2000/02/21 14:00:17  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/************************************************************************
*   File: handler.c                                     Part of CircleMUD *
*  Usage: internal funcs: moving and finding chars/objs                   *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */
#include <stddef.h>
#include <regex.h>

#include "alias.h"
#include "fight.h"
#include "mobact.h"
#include "conf.h"
#include "sysdep.h"
#include "act.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "db.h"
#include "handler.h"
#include "interpreter.h"
#include "spells.h"
#include "cards.h"
#include "xmlobjsave.h"
#include "dg_scripts.h"
#include "retainer.h"
#include "config.h"

char *fname(char *namelist)
{
  static char holder[30];
  register char *point;

  for (point = holder; isalpha(*namelist); namelist++, point++)
    *point = *namelist;

  *point = '\0';

  return (holder);
}

/*
 * Improved CircleMUD isname() function.
 * written by Juliano Ravasi Ferraz <jferraz@linkway.com.br>.
 *
 * This will allow players to type more than one of the aliases of a mob
 * or an
 * object to reference them. For example, if there are 3 swords on the
 * floor
 * (a long sword, a short sword and a tempered sword). If you type "get
 * sword",
 * your player will get the first one. With this, you can type "get
 * sword,long"
 * to get the long one.
 *
 * Set the following preproc constant to the character that you
 * want as a
 * keyword-separator for the isname() function. The default is a
 * comma.
 *
 * This and get_number()!
 */
#define MULTINAME_CHAR  ','

static int single_isname(const char *str, const char *namelist)
{
  const char *curname, *curstr;

  curname = namelist;
  for (;;) {
    for (curstr = str;; curstr++, curname++) {
      if (!*curstr && (use_abbrevs || !isalpha(*curname)))
        return (1);

      if (!*curname)
        return (0);

      if ((use_abbrevs && !*curstr) || *curname == ' ')
        break;

      if (LOWER(*curstr) != LOWER(*curname))
        break;
    }

    while (isalpha(*curname))
      curname++; 

    if (!*curname)
      return (0);

    curname++;
    }
}

int isname(const char *str, const char *namelist)
{
  char token[MAX_INPUT_LENGTH], *tokp, *rxstr;
  char *startrx, *stoprx;
  regex_t regex; 
  int err;

  if (str == NULL || namelist == NULL) {
    alog("SYSERR: NULL str (%p) or namelist (%p) passed to isname().",
      str, namelist);
    return (0);
  }

  rxstr   = strdup(str);
  startrx = strchr(rxstr ,'/');
  stoprx  = strrchr(rxstr,'/');

  if (startrx && stoprx && (startrx != stoprx) && use_regexp) {
    startrx++;
    *stoprx='\0';

    if ((err = regcomp(&regex,startrx, REG_EXTENDED|REG_ICASE|REG_NOSUB))) {
      regerror(err,&regex,token, (MAX_INPUT_LENGTH-1));
      alog("SYSERR: error in regexp /%s/: %s",startrx,token);
      free(rxstr);
      return 0;
    }
    if (!regexec(&regex, namelist, -1, NULL, 0)){

      free(rxstr);
      regfree(&regex);
      return 1;
    }

    free(rxstr);
    regfree(&regex);
    return 0;
  }

  free(rxstr);
  while (*str) {
    while (!isalpha(*str) || *str == MULTINAME_CHAR)
      str++;
    if (!*str)
      break;

    for (tokp = token; *str && *str != MULTINAME_CHAR; str++)
      *tokp++ = *str;
    *tokp = '\0';
    if (!single_isname(token, namelist))
      return (0);
  }
  return (1);
}

 
/* REPLACED WITH ABOVE TWO FUNCTIONS --
int isname(char *str, char *namelist)
{
  register char *curname, *curstr;

  if (!str)
    return (FALSE);

  if (!namelist)
    return (FALSE);

  curname = namelist;
  for (;;) {
    for (curstr = str;; curstr++, curname++) {
      if (!*curstr && !isalpha(*curname))
    return (1);

      if ((*curstr==',')&&((*curname ==' ')||(!*curname))) 
        return (1);
      
      if (!*curstr&&((*curstr-1)!=','))
        return (1);

      if (!*curname)
    return (0);

      if (*curname == ' ')
    break;

      if (tolower(*curstr) != tolower(*curname))
    break;
    }


    for (; isalpha(*curname); curname++);
    if (!*curname)
      return (0);
    curname++;		
  }
}
  */


void affect_modify(struct char_data * ch, byte loc, long mod, long bitv, 
long bitv2, long bitv3, 
              bool add,struct obj_data *obj)
{
//  int maxabil;
  long mana_add;

  if (bitv) {
    if (add) 
      SET_BIT(AFF_FLAGS(ch), bitv);
    else 
      REMOVE_BIT(AFF_FLAGS(ch), bitv);
  }

  if (bitv2) {
    if (add) 
      SET_BIT(AFF2_FLAGS(ch), bitv2);
    else 
      REMOVE_BIT(AFF2_FLAGS(ch), bitv2);
  }

  if (bitv3) {
    if (add) 
      SET_BIT(AFF3_FLAGS(ch), bitv3);
    else 
      REMOVE_BIT(AFF3_FLAGS(ch), bitv3);
  }

  if (!add)
    mod = -mod;

//  maxabil = (IS_NPC(ch) ? 25 : 30);
//  if (GET_LEVEL(ch) >= LVL_IMMORT) maxabil = 50;

  switch (loc) {
  case APPLY_NONE:
    break;

  case APPLY_STR:
    GET_STR(ch) += mod;
    break;
  case APPLY_DEX:
    GET_DEX(ch) += mod;
    break;
  case APPLY_INT:
    GET_INT(ch) += mod;
    break;
  case APPLY_WIS:
    GET_WIS(ch) += mod;
    break;
  case APPLY_CON:
    GET_CON(ch) += mod;
    break;
  case APPLY_CHA:
    GET_CHA(ch) += mod;
    break;
  case APPLY_MANA:
    if (obj == NULL) return;

    if ((obj != NULL) && (IS_SET(OBJ_EXTRA_FLAGS(obj),ITEM_ANGREAL) || IS_SET(OBJ_EXTRA_FLAGS(obj),ITEM_SAANGREAL))) {
      if (((GET_SEX(ch) == SEX_MALE) &&  !IS_SET(OBJ_EXTRA_FLAGS(obj),ITEM_NOMALE)) ||
         ((GET_SEX(ch) == SEX_FEMALE) && !IS_SET(OBJ_EXTRA_FLAGS(obj),ITEM_NOFEMALE))) {
         if (!add) {
           ADD_MAX_MANA(ch, -obj->mana_add);
           obj->mana_add = 0;
         }
         else {
           mana_add = (int)(GET_MAX_MANA(ch) * (mod/100.0));
           if ((GET_MAX_MANA(ch) + mana_add) > 15000)
             mana_add = (15000 - GET_MAX_MANA(ch));
           ADD_MAX_MANA(ch, mana_add);
           obj->mana_add = mana_add;
         }
       }   
     }    
     else {
       if (!add) {
         ADD_MAX_MANA(ch, -obj->mana_add);
         obj->mana_add = 0;
       }
       else {
         if ((GET_MAX_MANA(ch) + mod) > 15000)
            mod = (15000 - GET_MAX_MANA(ch));
         obj->mana_add = mod;
         ADD_MAX_MANA(ch, mod);
       }
    }
    break;	
  case APPLY_HIT:
    GET_MAX_HIT(ch) += mod;
    break;
  case APPLY_MOVE:
    GET_MAX_MOVE(ch) += mod;
    break;

  case APPLY_AC:
    GET_REAL_AC(ch) += mod;
    break;

  case APPLY_HITROLL:
    GET_HITROLL(ch) += mod;
    break;

  case APPLY_DAMROLL:
    GET_DAMROLL(ch) += mod;
    break;
  
  case APPLY_ABS:
    GET_ABS(ch) += mod;
    break;

  case APPLY_XATTACKS:
    GET_XATTACKS(ch) += mod;
    break;

  default:
    if (NULL != obj) {
      sprintf(buf, "SYSERR: Unknown affection attempt in obj %ld.", GET_OBJ_VNUM(obj));
    }
    else {
      sprintf(buf,"SYSERR: Affection attempt on a NULL-object!");
    }
    mlog (buf);
    break;

  } /* switch */
}


/* This updates a character by subtracting everything he is affected by */
/* restoring original abilities, and then affecting all again           */
void affect_total(struct char_data * ch)
{
  struct affected_type *af;
  int i, j;
  
  for (i = 0; i < NUM_WEARS; i++) {
    if (GET_EQ(ch, i))
      for (j = 0; j < MAX_OBJ_AFFECT; j++) 
    affect_modify(ch, GET_EQ(ch, i)->affected[j].location,
          GET_EQ(ch, i)->affected[j].modifier, 0, 0, 0, FALSE,GET_EQ(ch,i));
  } 

  for (af = ch->affected; af; af = af->next) 
    affect_modify(ch, af->location, af->modifier, af->bitvector, af->bitvector2, af->bitvector3, FALSE,NULL);

  ch->aff_abils = ch->real_abils;

  for (i = 0; i < NUM_WEARS; i++) {
    if (GET_EQ(ch, i))
      for (j = 0; j < MAX_OBJ_AFFECT; j++) 
        affect_modify(ch, GET_EQ(ch, i)->affected[j].location,
        GET_EQ(ch, i)->affected[j].modifier, 0, 0, 0, TRUE, GET_EQ(ch,i));
  } 

  for (af = ch->affected; af; af = af->next) 
    affect_modify(ch, af->location, af->modifier, af->bitvector, af->bitvector2, af->bitvector3, TRUE,NULL);
  /* Make certain values are between 0..25, not < 0 and not > 25! */

  i = (IS_NPC(ch) ? 25 : 30);

  if (GET_LEVEL(ch) >= LVL_IMMORT) i = 50;

  GET_DEX(ch) = MAX(0, MIN(GET_DEX(ch), i));
  GET_INT(ch) = MAX(0, MIN(GET_INT(ch), i));
  GET_WIS(ch) = MAX(0, MIN(GET_WIS(ch), i));
  GET_CON(ch) = MAX(0, MIN(GET_CON(ch), i));
  GET_STR(ch) = MAX(0, MIN(GET_STR(ch), i));

  if (IS_AFFECTED2(ch, AFF2_BARGAIN) || IS_AFFECTED2(ch, AFF2_CHARM))
    i=50;
  else
    i = (IS_NPC(ch) ? 25 : 30);
  
  GET_CHA(ch) = MAX(0, MIN(GET_CHA(ch), i));
}

/* Inserts an affect type in a room_data structure*/
void affect_to_room(struct room_data *room,struct affected_type *af)
{
  struct affected_type *affect;
  
  CREATE(affect,struct affected_type,1);
  *affect = *af;
  affect->next = room->aff;
  room->aff    = affect;	
  SET_BIT(room->aff_flags,af->bitvector);
}

void affect_remove_room(struct room_data *room,struct affected_type *af)
{
  struct affected_type *temp;

  assert(room->aff);
  REMOVE_BIT(room->aff_flags,af->bitvector);
  REMOVE_FROM_LIST(af,room->aff,next);
  free(af);
}

/* Remove affection of type "type" from a room */
void affect_from_room(struct room_data *room,int type)
{
  struct affected_type *hjp,*nxt;

  for (hjp = room->aff; hjp != NULL; hjp = nxt) {
    nxt = hjp->next; 
    if (hjp && (hjp->type == type))
      affect_remove_room(room,hjp);
  }
}

/* Insert an affect_type in a char_data structure
   Automatically sets apropriate bits and apply's */
void affect_to_char(struct char_data * ch, struct affected_type * af)
{
  struct affected_type *affect;
// SPELL_TOP_DEFINE was: MAX_SKILLS. 
  if ((af->type < 1) || (af->type > TOP_SPELL_DEFINE)) {
    return;
  }

  if ((af->type > 0) && (af->type <= MAX_SPELLS)) {
    if ((SCMD_TIED != af->tie_info) && (SCMD_MAINTAINED != af->tie_info)) {
      return;
    }
  }

  CREATE(affect, struct affected_type, 1);
  *affect = *af;
  affect->next = ch->affected;
  ch->affected = affect;

  affect_modify(ch, af->location, af->modifier, af->bitvector, af->bitvector2, af->bitvector3, TRUE,NULL);
  affect_total(ch);
}

/*
 * Remove an affected_type structure from a char (called when duration
 * reaches zero). Pointer *af must never be NIL!  Frees mem and calls
 * affect_location_apply
 */
void affect_remove(struct char_data * ch, struct affected_type * af)
{
  struct affected_type *temp;

  assert(ch->affected);

  affect_modify(ch, af->location, af->modifier, af->bitvector, af->bitvector2, af->bitvector3, FALSE,NULL);
  REMOVE_FROM_LIST(af, ch->affected, next);
  free(af);
  affect_total(ch);
}



/* Call affect_remove with every spell of spelltype "skill" */
void affect_from_char(struct char_data * ch, int type)
{
  struct affected_type *hjp, *next;

  for (hjp = ch->affected; hjp; hjp = next) {
    if (!hjp)
      return;

    next = hjp->next;

    if (hjp && (hjp->type == type))
      affect_remove(ch, hjp);
  }
}



/*
 * Return if a char is affected by a spell (SPELL_XXX), NULL indicates
 * not affected
 */
bool room_affected_by_spell(struct room_data *room, int type)
{
  struct affected_type *pek;

  if (!room || !room->aff)
    return FALSE;

  for (pek = room->aff; pek; pek = pek->next)
    if (pek->type == type)
      return TRUE;

  return FALSE;
}

bool affected_by_spell(struct char_data * ch, int type)
{
  struct affected_type *hjp;

  if (!ch || !ch->affected)
    return FALSE;

  for (hjp = ch->affected; hjp; hjp = hjp->next)
    if (hjp->type == type)
      return TRUE;

  return FALSE;
}



void affect_join(struct char_data * ch, struct affected_type * af,
            bool add_dur, bool avg_dur, bool add_mod, bool avg_mod)
{
  struct affected_type *hjp;
  bool found = FALSE;

  
  for (hjp = ch->affected; !found && hjp; hjp = hjp->next) {

    if ((hjp->type == af->type) && (hjp->location == af->location)) {
      if (add_dur)
    af->duration += hjp->duration;
      if (avg_dur)
    af->duration >>= 1;

      if (add_mod)
    af->modifier += hjp->modifier;
      if (avg_mod)
    af->modifier >>= 1;

      affect_remove(ch, hjp);
      affect_to_char(ch, af);
      found = TRUE;
    }
  }
  if (!found)
    affect_to_char(ch, af);
}


/* move a player out of a room */
void char_from_room(struct char_data * ch)
{
  struct char_data *temp;

  if (ch == NULL) {
    mlog("SYSERR: NULL in handler.c, char_from_room");
    exit(1);
  }

  if (NOWHERE == ch->in_room) {
    mlog("SYSERR: NOWHERE in handler.c, char_from_room: Char already removed. Returning.");
    return;
  }

  TICKS_IN_ROOM(ch) = 0;

  if (FIGHTING(ch) != NULL) 
    stop_fighting(ch);

  if (GET_EQ(ch, WEAR_LIGHT) != NULL)
    if (GET_OBJ_TYPE(GET_EQ(ch, WEAR_LIGHT)) == ITEM_LIGHT)
      if (GET_OBJ_VAL(GET_EQ(ch, WEAR_LIGHT), 2))	/* Light is ON */
    world[ch->in_room].light--;

  REMOVE_FROM_LIST(ch, world[ch->in_room].people, next_in_room);
  ch->in_room = NOWHERE;
  ch->next_in_room = NULL;
}


/* place a character in a room */
void char_to_room(struct char_data * ch, int room)
{
  if (!ch || room < 0 || room > top_of_world) {
    mlog("SYSERR: Illegal value(s) passed to char_to_room");
    return;
  }

  if (NOWHERE != ch->in_room) {
    axlog( SYS_ERROR, LVL_IMMORT, TRUE, "SYSERR: char_to_room called for %s when already in room. Removing...", GET_NAME( ch ) );
    char_from_room(ch);
  }

  if( !world[room].people && world[room].scene )
  {
    if( CHAR_SCENE( world[room].scene->owner ) )
      CLEAR_SCENE( CHAR_SCENE( world[room].scene->owner ) );
    if( ROOM_NO_SCENE( room ) )
      CLEAR_SCENE( ROOM_NO_SCENE( room ) );
  }
  ch->next_in_room = world[room].people;
  world[room].people = ch;
  ch->in_room = room;
  TICKS_IN_ROOM(ch) = 0;

  if (!IS_NPC(ch))   /* Simple assumption: New room, no idling */
    GET_R_IDLE(ch) = 0;
  if (GET_EQ(ch, WEAR_LIGHT))
    if (GET_OBJ_TYPE(GET_EQ(ch, WEAR_LIGHT)) == ITEM_LIGHT) 
      if (GET_OBJ_VAL(GET_EQ(ch, WEAR_LIGHT), 2))	/* Light ON */
     world[room].light++;
}


/* give an object to a char   */
void obj_to_char(struct obj_data * object, struct char_data * ch)
{
  if (object && ch) {
    object->next_content = ch->carrying;
    ch->carrying = object;
    object->carried_by = ch;
    object->in_room = NOWHERE;
    IS_CARRYING_W(ch) += GET_OBJ_WEIGHT(object);
    IS_CARRYING_N(ch)++;

    /* set flag for crash-save system */
    SET_BIT(PLR_FLAGS(ch), PLR_CRASH);
  } else
    mlog("SYSERR: NULL obj or char passed to obj_to_char");
}


/* take an object from a char */
void obj_from_char(struct obj_data * object)
{
  struct obj_data *temp;

  if (object == NULL) {
    mlog("SYSERR: NULL object passed to obj_from_char");
    return;
  }
  REMOVE_FROM_LIST(object, object->carried_by->carrying, next_content);

  /* set flag for crash-save system */
  SET_BIT(PLR_FLAGS(object->carried_by), PLR_CRASH);

  IS_CARRYING_W(object->carried_by) -= GET_OBJ_WEIGHT(object);
  IS_CARRYING_N(object->carried_by)--; 
  if (IS_CARRYING_W(object->carried_by) < 0)
    IS_CARRYING_W(object->carried_by) = 0; /* Bug? */
  object->carried_by = NULL;
  object->next_content = NULL;
}



/* Return the effect of a piece of armor in position eq_pos */
int apply_ac(struct char_data * ch, int eq_pos)
{
  int factor;

  assert(GET_EQ(ch, eq_pos));

  if (!(GET_OBJ_TYPE(GET_EQ(ch, eq_pos)) == ITEM_ARMOR))
    return 0;

  switch (eq_pos) {

  case WEAR_BODY:
    factor = 3;
    break;			/* 30% */
  case WEAR_HEAD:
    factor = 2;
    break;			/* 20% */
  case WEAR_LEGS:
    factor = 2;
    break;			/* 20% */
  default:
    factor = 1;
    break;			/* all others 10% */
  }

  return (factor * GET_OBJ_VAL(GET_EQ(ch, eq_pos), 0));
}



void equip_char(struct char_data * ch, struct obj_data * obj, int pos)
{
  int i, j;

  assert(pos >= 0 && pos < NUM_WEARS);

  if (GET_EQ(ch, pos)) {
    sprintf(buf, "SYSERR: Char is already equipped: %s, %s", GET_NAME(ch),
        obj->short_description);
    mlog(buf);
    return;
  }
  if (obj->carried_by) {
    mlog("SYSERR: EQUIP: Obj is carried_by when equip.");
    return;
  }
  if (obj->in_room != NOWHERE) {
    mlog("SYSERR: EQUIP: Obj is in_room when equip.");
    return;
  }

  if (GET_OBJ_TYPE(obj) == ITEM_WEAPON) { 
    if (CAN_WEAR(obj,ITEM_WEAR_DUALWIELD) && (GET_EQ(ch,WEAR_WIELD_R) || GET_EQ(ch,WEAR_WIELD_L))) {
      sprintf(buf,"SYSERR: EQUIP:  Char is already equipped: %s, %s",GET_NAME(ch),obj->short_description);
      mlog(buf);
      return;
    }
    else if (CAN_WEAR(obj,ITEM_WEAR_DUALWIELD)) {
      GET_EQ(ch,WEAR_WIELD_R) =  obj;
      obj->worn_by = ch;
      obj->worn_on = WEAR_WIELD_R;
    }
    else if (CAN_WEAR(obj,ITEM_WEAR_WIELD) && GET_EQ(ch,WEAR_WIELD_R) && GET_EQ(ch,WEAR_WIELD_L)) {
      sprintf(buf,"SYSERR: EQUIP: Char is already equipped: %s, %s",GET_NAME(ch),obj->short_description);
      mlog(buf);
      return;
    }
    else {
      obj->worn_by = ch;
      if (GET_EQ(ch,WEAR_WIELD_R) && (pos == WEAR_WIELD_R)) {
        GET_EQ(ch,WEAR_WIELD_L) = obj;
        obj->worn_on = WEAR_WIELD_L;
      }
      else if (GET_EQ(ch,WEAR_WIELD_L) && (pos == WEAR_WIELD_L)){
        GET_EQ(ch,WEAR_WIELD_R) = obj;
        obj->worn_on = WEAR_WIELD_R;
      }
      else {
        GET_EQ(ch,pos) = obj;
        obj->worn_on   = pos;
      }
    }
  }
  else {
    GET_EQ(ch, pos) = obj;
    obj->worn_by = ch;
    obj->worn_on = pos;
  }

  if (GET_OBJ_TYPE(obj) == ITEM_ARMOR)
  {
    GET_REAL_AC(ch) -= apply_ac(ch, pos);
    GET_ABS(ch) += GET_OBJ_VAL(GET_EQ(ch, pos), 1);
  }
  if (ch->in_room != NOWHERE) {
    if (pos == WEAR_LIGHT && GET_OBJ_TYPE(obj) == ITEM_LIGHT)
      if (GET_OBJ_VAL(obj, 2))	/* if light is ON */
    world[ch->in_room].light++;
  } else
    mlog("SYSERR: ch->in_room = NOWHERE when equipping char.");

  for (j = 0; j < MAX_OBJ_AFFECT; j++) {
    i = 0;
    while (obj->affected[j].location > i)
      i ++;
    affect_modify(ch, obj->affected[j].location,
          obj->affected[j].modifier, 0, 0, 0,TRUE,obj);
  }
  affect_total(ch);
}



struct obj_data *unequip_char(struct char_data * ch, int pos)
{
  int i, j;
  struct obj_data *obj;

  if (pos < 0 || pos >= NUM_WEARS)
    return NULL;

  assert(pos >= 0 && pos < NUM_WEARS);

/*  assert(GET_EQ(ch, pos)); Ugly as hell! Remove it!*/

  if (!GET_EQ(ch,pos))
    return NULL;

  obj = GET_EQ(ch, pos);
  obj->worn_by = NULL;
  obj->worn_on = -1;

  if (GET_OBJ_TYPE(obj) == ITEM_ARMOR)
  {
    GET_REAL_AC(ch) += apply_ac(ch, pos);
    GET_ABS(ch) -= GET_OBJ_VAL(GET_EQ(ch, pos), 1);    
  }
  if (ch->in_room != NOWHERE) {
    if (pos == WEAR_LIGHT && GET_OBJ_TYPE(obj) == ITEM_LIGHT)
      if (GET_OBJ_VAL(obj, 2))	/* if light is ON */
    world[ch->in_room].light--;
  } else
    mlog("SYSERR: ch->in_room = NOWHERE when equipping char.");

  GET_EQ(ch, pos) = NULL;

  for (j = 0; j < MAX_OBJ_AFFECT; j++) {
    i = 0;
    while (obj->affected[j].location > i)
      i ++;
    affect_modify(ch, obj->affected[j].location,
          obj->affected[j].modifier,
          obj->obj_flags.bitvector, 0, 0, FALSE,obj);
  }
  affect_total(ch);

  return (obj);
}

int get_number(char **name)
{
  int i;
  char number[MAX_INPUT_LENGTH], *ppos;

  /*
   * These lines was added to make possible the dot sign to be used as
   * a magic
   * char on isname() function.
   */
  if (!isdigit(**name))
    return (1);

  if ((ppos = strchr(*name, '.')) != NULL) {
    *ppos++ = '\0';
    strcpy(number, *name);
    strcpy(*name, ppos);

    for (i = 0; *(number + i); i++)
      if (!isdigit(*(number + i)))
        return (0);

    return (atoi(number));
  }
  return (1);
}


/* REPLACED WITH ABOVE
int get_number(char **name)
{
  int i;
  char *ppos;
  char number[MAX_INPUT_LENGTH];

  *number = '\0';

  if ((ppos = strchr(*name, '.'))) {
    *ppos++ = '\0';
    strcpy(number, *name);
    strcpy(*name, ppos);

    for (i = 0; *(number + i); i++)
      if (!isdigit(*(number + i)))
    return 0;

    return (atoi(number));
  }
  return 1;
}
 */



/* Search a given list for an object number, and return a ptr to that obj */
struct obj_data *get_obj_in_list_num(int num, struct obj_data * list)
{
  struct obj_data *i;

  for (i = list; i; i = i->next_content)
    if (GET_OBJ_RNUM(i) == num)
      return i;

  return NULL;
}



/* search the entire world for an object number, and return a pointer  */
struct obj_data *get_obj_num(int nr)
{
  struct obj_data *i;

  for (i = object_list; i; i = i->next)
    if (GET_OBJ_RNUM(i) == nr)
      return i;

  return NULL;
}



/* search a room for a char, and return a pointer if found..  */
struct char_data *get_char_room(char *name, int room)
{
  struct char_data *i;
  int j = 0, number;
  char tmpname[MAX_INPUT_LENGTH];
  char *tmp = tmpname;

  strcpy(tmp, name);
  if (!(number = get_number(&tmp))) 
    return NULL;

  for (i = world[room].people; i && (j <= number); i = i->next_in_room)
    if (isname(tmp, GET_NAME(i)) || isname(tmp,GET_KWS_CH(i)))
      if (++j == number)
    return i;

  return NULL;
}



/* search all over the world for a char num, and return a pointer if found */
struct char_data *get_char_num(int nr)
{
  struct char_data *i;

  for (i = character_list; i; i = i->next)
    if (GET_MOB_RNUM(i) == nr)
      return i;

  return NULL;
}



/* put an object in a room */
void obj_to_room(struct obj_data * object, int room)
{
  if (!object || room < 0 || room > top_of_world)
    mlog("SYSERR: Illegal value(s) passed to obj_to_room");
  else {
    object->next_content = world[room].contents;
    world[room].contents = object;
    object->in_room = room;
    object->carried_by = NULL;
  }
}


/* Take an object from a room */
void obj_from_room(struct obj_data * object)
{
  struct obj_data *temp;

  if (!object || object->in_room == NOWHERE) {
    mlog("SYSERR: NULL object or obj not in a room passed to obj_from_room");
    return;
  }

  if (object->in_room < 0 || object->in_room > top_of_world || NULL == world[object->in_room].contents) {
    mlog("SYSERR: NULL contents or NULL room in obj_from_room()");
    return;
  }

  REMOVE_FROM_LIST(object, world[object->in_room].contents, next_content);

  object->in_room = NOWHERE;
  object->next_content = NULL;
}


/* put an object in an object (quaint)  */
void obj_to_obj(struct obj_data * obj, struct obj_data * obj_to)
{
  struct obj_data *tmp_obj;

  if (!obj || !obj_to || obj == obj_to) {
    mlog("SYSERR: NULL object or same source and target obj passed to obj_to_obj");
    return;
  }

  obj->next_content = obj_to->contains;
  obj_to->contains = obj;
  obj->in_obj = obj_to;

  for (tmp_obj = obj->in_obj; tmp_obj->in_obj; tmp_obj = tmp_obj->in_obj)
    GET_OBJ_WEIGHT(tmp_obj) += GET_OBJ_WEIGHT(obj);

  /* top level object.  Subtract weight from inventory if necessary. */
  GET_OBJ_WEIGHT(tmp_obj) += GET_OBJ_WEIGHT(obj);
  if (tmp_obj->carried_by)
    IS_CARRYING_W(tmp_obj->carried_by) += GET_OBJ_WEIGHT(obj);
}


/* remove an object from an object */
void obj_from_obj(struct obj_data * obj)
{
  struct obj_data *temp, *obj_from;

  if (obj->in_obj == NULL) {
    mlog("error (handler.c): trying to illegally extract obj from obj");
    return;
  }
  obj_from = obj->in_obj;
  REMOVE_FROM_LIST(obj, obj_from->contains, next_content);

  /* Subtract weight from containers container */
  for (temp = obj->in_obj; temp->in_obj; temp = temp->in_obj)
    GET_OBJ_WEIGHT(temp) -= GET_OBJ_WEIGHT(obj);

  /* Subtract weight from char that carries the object */
  GET_OBJ_WEIGHT(temp) -= GET_OBJ_WEIGHT(obj);
  if (temp->carried_by)
    IS_CARRYING_W(temp->carried_by) -= GET_OBJ_WEIGHT(obj);

  obj->in_obj = NULL;
  obj->next_content = NULL;
}


/* Set all carried_by to point to new owner */
void object_list_new_owner(struct obj_data * list, struct char_data * ch)
{
  if (list) {
    object_list_new_owner(list->contains, ch);
    object_list_new_owner(list->next_content, ch);
    list->carried_by = ch;
  }
}


/* Extract an object from the world */
void extract_obj(struct obj_data * obj)
{
  struct obj_data *temp;
  struct obj_data *fo;
  struct char_data *fc;

  if (NULL == obj) {
    mlog("SYSERR: extract_obj() called with NULL-pointer arg.");
    return;
  }

  if (obj->worn_by != NULL)
    if (unequip_char(obj->worn_by, obj->worn_on) != obj)
      mlog("SYSERR: Inconsistent worn_by and worn_on pointers!!");

  // Remove all stuff from obj if obj is furniture...
  if (GET_OBJ_TYPE(obj) == ITEM_FURNITURE) {
    for (fc = character_list; fc; fc = fc->next) {
      if (GET_SEAT(fc) == obj) {
        GET_SEAT(fc) = NULL;
      }
    }
    for (fo = object_list; fo; fo = fo->next) {
      if (fo->on_obj == obj)
        fo->on_obj = NULL;
    }
  }

  if (obj->in_room != NOWHERE)
    obj_from_room(obj);
  else if (obj->carried_by)
    obj_from_char(obj);
  else if (obj->in_obj)
    obj_from_obj(obj);
  if (obj->pulled_by)
    obj->pulled_by->pulling = NULL;
  if (GET_OBJ_RNUM(obj)>0 &&
      world[GET_OBJ_RNUM(obj)].wagon)
    world[GET_OBJ_RNUM(obj)].wagon = NULL;
  /* Get rid of the contents of the object, as well. */

  while (obj->contains)
    extract_obj(obj->contains);

  REMOVE_FROM_LIST(obj, object_list, next);

  if (GET_OBJ_RNUM(obj) >= 0)
    (obj_index[GET_OBJ_RNUM(obj)].number)--;

  if (SCRIPT(obj))
   extract_script(SCRIPT(obj));

  free_obj(obj);
  obj = NULL;
}



void update_object(struct obj_data * obj, int use)
{
  /* dont update objects with a timer trigger */
  if (!SCRIPT_CHECK(obj, OTRIG_TIMER) && (GET_OBJ_TIMER(obj) > 0)) 
    GET_OBJ_TIMER(obj) -= use;
  if (obj->contains)
    update_object(obj->contains, use);
  if (obj->next_content)
    update_object(obj->next_content, use);
}


void update_char_objects(struct char_data * ch)
{
  int i;

  if (GET_EQ(ch, WEAR_LIGHT) != NULL)
    if (GET_OBJ_TYPE(GET_EQ(ch, WEAR_LIGHT)) == ITEM_LIGHT)
      if (GET_OBJ_VAL(GET_EQ(ch, WEAR_LIGHT), 2) > 0) {
    i = --GET_OBJ_VAL(GET_EQ(ch, WEAR_LIGHT), 2);
    if (i == 1) {
      act("Your light begins to flicker and fade.", FALSE, ch, 0, 0, TO_CHAR);
      act("$n's light begins to flicker and fade.", FALSE, ch, 0, 0, TO_ROOM);
    } else if (i == 0) {
      act("Your light sputters out and dies.", FALSE, ch, 0, 0, TO_CHAR);
      act("$n's light sputters out and dies.", FALSE, ch, 0, 0, TO_ROOM);
      world[ch->in_room].light--;
    }
      }

  for (i = 0; i < NUM_WEARS; i++)
    if (GET_EQ(ch, i))
      update_object(GET_EQ(ch, i), 1);

  if (ch->carrying)
    update_object(ch->carrying, 1);
}



/* Extract a ch completely from the world, and leave his stuff behind */
void extract_char(struct char_data * ch, int delete)
{
  struct char_data *k, *temp;
  struct descriptor_data *t_desc;
  struct obj_data *obj;
  int i, freed = 0;
  void remove_char_guild_data(struct char_data *ch);

  if (!IS_NPC(ch) && !ch->desc) {
    for (t_desc = descriptor_list; t_desc; t_desc = t_desc->next)
      if (t_desc->original == ch)
    do_return(t_desc->character, "", 0, 0);
  }
  if (ch->in_room == NOWHERE) {
    mlog("SYSERR: NOWHERE extracting char. (handler.c, extract_char)");
    exit(1);
  }
  /* Forget snooping, if applicable */
  if (ch->desc) {
    if (ch->desc->snooping) {
      ch->desc->snooping->snoop_by = NULL;
      ch->desc->snooping = NULL;
    }
    if (ch->desc->snoop_by) {
      SEND_TO_Q("Your victim is no longer among us.\r\n",
        ch->desc->snoop_by);
      ch->desc->snoop_by->snooping = NULL;
      ch->desc->snoop_by = NULL;
    }
  }

  die_follower(ch, delete);
  /* transfer objects to room, if any */
  while (ch->carrying) {
    obj = ch->carrying;
    obj_from_char(obj);
    if (delete) 
      extract_obj(obj);
    else
      obj_to_room(obj, ch->in_room);
  }
  /* transfer equipment to room, if any */
  for (i = 0; i < NUM_WEARS; i++)
    if (GET_EQ(ch, i)) {
      if (delete)
        extract_obj(unequip_char(ch, i));
      else
        obj_to_room(unequip_char(ch, i), ch->in_room);
    }

  if (FIGHTING(ch))
    stop_fighting(ch);
  if (ch->pulling)
    ch->pulling->pulled_by = NULL;
  ch->pulling = NULL;
  for (k = combat_list; k; k = temp) {
    temp = k->next_fighting;
    if (FIGHTING(k) == ch)
      stop_fighting(k);
  }

  if (is_retainer(ch)) // Unlink connection from retainer struct
  {
    ch->mob_specials.retainer->mob = (struct char_data *) 0;
  }

  if (!IS_NPC(ch)) {
    unsummon_retainers(ch);
    if(GET_AFW(ch) != std_afw && GET_AFK(ch) != std_Flag )
      free(GET_AFW(ch));
    GET_AFW(ch) = NULL;

    if( IS_SET(PRF_FLAGS(ch),PRF_AFK)) 
      REMOVE_BIT(PRF_FLAGS(ch),PRF_AFK);
    if( IS_SET(PRF2_FLAGS(ch),PRF2_AFW)) 
      REMOVE_BIT(PRF2_FLAGS(ch),PRF2_AFW);


    if ((ch->in_room) && (ch->in_room != 1) && (ch->in_room != NOWHERE))
      save_char(ch, ch->in_room);
    else
      save_char(ch, NOWHERE);
  }
  /*
   * can't remove guilds from char -before- we have saved!
   */
  remove_char_guild_data(ch);

  if (!IS_NPC(ch) && DICING(ch) && char_still_online(DICING(ch))) {
    DICING(DICING(ch)) = NULL;
    DICING(ch) = NULL;
  } 

  if (!IS_NPC(ch) && GET_GAME(ch))
  {
    if (GET_GAME(ch)->players[0] == ch)
      end_game(GET_GAME(ch));
    else
      remove_player(ch);
  }

  char_from_room(ch);

  /* pull the char from the list */
  REMOVE_FROM_LIST(ch, character_list, next);

  if (ch->desc && ch->desc->original)
    do_return(ch, "", 0, 0);
  if (!IS_NPC(ch)) {
    if (delete)   {
      void do_newdisguise(struct char_data *ch, char *arg, int cmd, int subcmd);
      if (GET_DISGUISED(ch))
    do_newdisguise(ch, "off", 0, 0);
      GET_ALIASES(ch) = 0;
      write_aliases(ch);
      delete_playerfile(GET_NAME(ch));
    }
  } else {
    if (GET_MOB_RNUM(ch) > -1)		/* if mobile */
      mob_index[GET_MOB_RNUM(ch)].number--;
    clearMemory(ch);		/* Only NPC's can have memory */
    if (SCRIPT(ch))
     extract_script(SCRIPT(ch));
    if (SCRIPT_MEM(ch))
      extract_script_mem(SCRIPT_MEM(ch));
    SCRIPT_MEM(ch) = NULL;
    free_char(ch);
    freed = 1;
  }

  if(!freed && ch->player_specials && CHAR_SCENE( ch ) )
    CHAR_SCENE( ch )->owner = NULL;

  if (!freed && ch->desc != NULL) {
    STATE(ch->desc) = CON_CLOSE;
  } else {  /* if a player gets purged from within the game */
    if (!freed)
      free_char(ch);
  }
}



/* ***********************************************************************
* Here follows high-level versions of some earlier routines, ie functions*
* which incorporate the actual player-data                               *.
*********************************************************************** */

int name_matches(char *name, struct char_data *ch)
{
  if (IS_NPC(ch)) {
    if (isname(name, GET_KWS_CH(ch)))
      return TRUE;
  } else {
     if (!strncasecmp(name, GET_NAME(ch), strlen(name)))
       return TRUE;
  }
  return FALSE;
}

/* Comparing the name to the full name of the character, this is
 * in case another char for example Mathias is before Mat in the
 * list */
int full_name_matches(char *name, struct char_data *ch)
{
  if (IS_NPC(ch)) {
    if (isname(name, GET_KWS_CH(ch)))
      return TRUE;
  } else {
    if (!strcasecmp(GET_NAME(ch), name))
      return TRUE;
  }
  return FALSE;
}

struct char_data *get_player_vis(struct char_data * ch, char *name, int inroom)
{
  struct char_data *i;

  for (i = character_list; i; i = i->next) 
  {
    if( !IS_NPC(i) 
	&& (!inroom || i->in_room == ch->in_room) 
	&& full_name_matches(name, i) 
	&& CAN_SEE(ch, i))
      return i;
  }
   
  for (i = character_list; i; i = i->next) 
  {
    if ( !IS_NPC(i) 
      && (!inroom || i->in_room == ch->in_room) 
      && name_matches(name, i) 
      && CAN_SEE(ch, i))
      return i;
  }

  return NULL;
}


struct char_data *get_player(char *name, int inroom)
{
  struct char_data *i;
    
  for(i = character_list; i; i = i->next){
    if(!IS_NPC(i) && (!inroom) && name_matches(name, i))
      return(i);
  }
  return(NULL);
}
    

struct char_data *get_char_room_vis(struct char_data * ch, char *name)
{
  struct char_data *i;
  int j = 0, number;
  char tmpname[MAX_INPUT_LENGTH];
  char *tmp = tmpname;

  /* JE 7/18/94 :-) :-) */
  if (!str_cmp(name, "self") || !str_cmp(name, "me"))
    return ch;

  /* 0.<name> means PC with name */
  strcpy(tmp, name);
  if (!(number = get_number(&tmp)))
    return get_player_vis(ch, tmp, 1);

  for (i = world[ch->in_room].people; i && j <= number; i = i->next_in_room)
    if (name_matches(tmp, i))
      if (CAN_SEE(ch, i))
    if (++j == number)
      return i;

  return NULL;
}


struct char_data *get_char_vis(struct char_data * ch, char *name)
{
  struct char_data *i;
  int j = 0, number;
  char tmpname[MAX_INPUT_LENGTH];
  char *tmp = tmpname;

  /* check the room first */
  if ((i = get_char_room_vis(ch, name)) != NULL)
    return i;

  strcpy(tmp, name);
  if (!(number = get_number(&tmp)))
    return get_player_vis(ch, tmp, 0);

  for (i = character_list; i && (j <= number); i = i->next)
    if (full_name_matches(tmp, i) && CAN_SEE(ch, i))
      if (++j == number)
    return i;

  j = 0;
   
  for (i = character_list; i && (j <= number); i = i->next)
    if (name_matches(tmp, i) && CAN_SEE(ch, i))
      if (++j == number)
    return i;
  
   
  return NULL;
}

struct char_data *get_char_vis_including_tar(struct char_data * ch, char *name)
{
  struct char_data *i;
  int j = 0, number;
  char tmpname[MAX_INPUT_LENGTH];
  char *tmp = tmpname;

  /* check the room first */
  if ((i = get_char_room_vis(ch, name)) != NULL)
    return i;

  strcpy(tmp, name);
  if (!(number = get_number(&tmp)))
    return get_player_vis(ch, tmp, 0);

  return get_player_vis( ch, name, 0 );
  for (i = character_list; i && (j <= number); i = i->next)
    if (full_name_matches(tmp, i) && CAN_SEE_INCLUDING_TAR(ch, i))
      if (++j == number)
        return i;
   
  j = 0;

  for (i = character_list; i && (j <= number); i = i->next)
    if (name_matches(tmp, i) && CAN_SEE_INCLUDING_TAR(ch, i))
      if (++j == number)
        return i;
   
   
  return NULL;
}

int char_still_online(struct char_data *ch)
{
  struct char_data *i;

  for (i = character_list; NULL != i; i = i->next) {
    if (i == ch) 
      return 1;
  }
  return 0;
}

struct char_data *get_char_id(int id) {
  struct char_data *i;

  for (i = character_list; i; i = i->next) {
    if (GET_IDNUM(i) == id) return i;
  }

  return NULL;
}

struct char_data *get_char_pointer(char* name)
{

  struct char_data *i;
  int j=0, number;
  char tmpname[MAX_INPUT_LENGTH];
  char *tmp = tmpname;
  
  strcpy(tmp, name);
  if(!(number = get_number(&tmp)))
    return get_player(tmp, 0);
    
  for (i = character_list; i && (j <= number); i = i->next)
    if (name_matches(tmp, i))
      if (++j == number)
        return(i);
    
  return(NULL);
}

int is_inv_mail(struct obj_data *obj, char *keyword) {
  char *tmp;
  
  if (obj->item_number != 1) return 0;
  tmp = strstr(obj->action_description, "From: ");
  if (!tmp) return 0;
  tmp += 6;
  if (!strncasecmp(tmp, keyword, strlen(keyword))) return 1;
  return 0;
}

struct obj_data *get_obj_in_list_vis(struct char_data * ch, char *name,
                              struct obj_data * list)
{
  struct obj_data *i;
  int j = 0, number;
  char tmpname[MAX_INPUT_LENGTH];
  char tmpbuf[MAX_INPUT_LENGTH];
  char *tmp = tmpname;

  strcpy(tmp, name);
  if (!(number = get_number(&tmp)))
    return NULL;

  for (i = list; i && (j <= number); i = i->next_content)
    if (isname(tmp, i->name) || ((i->carried_by != NULL) && is_inv_mail(i, tmp)))
      if (CAN_SEE_OBJ(ch, i) || (ITEM_LIGHT == GET_OBJ_TYPE(i)))
    if (++j == number) 
      return i;

  number = number - j;
  sprintf(tmpbuf,"%d.%s",number,tmp);
  strcpy(name,tmpbuf);

  return NULL;
}

struct obj_data *get_obj_worn_vis(struct char_data *ch,char *name)
{
  int j=0,i,number;
  char tmpname[MAX_INPUT_LENGTH];
  char tmpbuf[MAX_INPUT_LENGTH];
  char *tmp = tmpname;  

  strcpy(tmp, name); /* Changed from "strcpy(tmpname, name);" Error? */
  if (!(number = get_number(&tmp)))
    return NULL;

  for (i = 0; (j <= number) && i < NUM_WEARS; i++) {
    if (GET_EQ(ch,i))
      if (isname(tmpname,(GET_EQ(ch,i))->name))
        if (CAN_SEE_OBJ(ch,GET_EQ(ch,i)))
          if (++j == number)
            return GET_EQ(ch,i);
  }
  number = number - j;
  sprintf(tmpbuf,"%d.%s",number,tmpname);  
  strcpy(name,tmpbuf);

  return NULL;
}



/* search the entire world for an object, and return a pointer  */
struct obj_data *get_obj_vis(struct char_data * ch, char *name)
{
  struct obj_data *i;
  int j = 0, number;
  char tmpname[MAX_INPUT_LENGTH];
  char *tmp = tmpname;

  /* scan items carried */
  if ((i = get_obj_in_list_vis(ch, name, ch->carrying)))
    return i;

  /* scan room */
  if ((i = get_obj_in_list_vis(ch, name, world[ch->in_room].contents)))
    return i;

  strcpy(tmp, name);
  if (!(number = get_number(&tmp)))
    return NULL;

  /* ok.. no luck yet. scan the entire obj list   */
  for (i = object_list; i && (j <= number); i = i->next)
    if (isname(tmp, i->name))
      if (CAN_SEE_OBJ(ch, i))
    if (++j == number)
      return i;

  return NULL;
}



struct obj_data *get_object_in_equip_vis(struct char_data * ch,
                 char *arg, struct obj_data * equipment[], int *j)
{
  for ((*j) = 0; (*j) < NUM_WEARS; (*j)++)
    if (equipment[(*j)])
      if (CAN_SEE_OBJ(ch, equipment[(*j)]))
    if (isname(arg, equipment[(*j)]->name))
      return (equipment[(*j)]);

  return NULL;
}


char *money_desc(int amount)
{
  static char buf[128];

  if (amount <= 0) {
    mlog("SYSERR: Try to create negative or 0 money.");
    return NULL;
  }
  if (amount == 1)
    strcpy(buf, "a gold coin");
  else if (amount <= 10)
    strcpy(buf, "a tiny pile of gold coins");
  else if (amount <= 20)
    strcpy(buf, "a handful of gold coins");
  else if (amount <= 75)
    strcpy(buf, "a little pile of gold coins");
  else if (amount <= 200)
    strcpy(buf, "a small pile of gold coins");
  else if (amount <= 1000)
    strcpy(buf, "a pile of gold coins");
  else if (amount <= 5000)
    strcpy(buf, "a big pile of gold coins");
  else if (amount <= 10000)
    strcpy(buf, "a large heap of gold coins");
  else if (amount <= 20000)
    strcpy(buf, "a huge mound of gold coins");
  else if (amount <= 75000)
    strcpy(buf, "an enormous mound of gold coins");
  else if (amount <= 150000)
    strcpy(buf, "a small mountain of gold coins");
  else if (amount <= 250000)
    strcpy(buf, "a mountain of gold coins");
  else if (amount <= 500000)
    strcpy(buf, "a huge mountain of gold coins");
  else if (amount <= 1000000)
    strcpy(buf, "an enormous mountain of gold coins");
  else
    strcpy(buf, "an absolutely colossal mountain of gold coins");

  return buf;
}


struct obj_data *create_money(int amount)
{
  struct obj_data *obj;
  struct extra_descr_data *new_descr;
  char buf[2048];

  if (amount <= 0) {
    mlog("SYSERR: Try to create negative or 0 money.");
    return NULL;
  }

  if (amount > 100000000) {
     mlog("SYSERR: More than 10^8 coins in create_money().");
     return NULL;
  }

  obj = create_obj();
  CREATE(new_descr, struct extra_descr_data, 1);

  if (amount == 1) {
    obj->name = str_dup("coin gold");
    obj->short_description = str_dup("a gold coin");
    obj->description = str_dup("One miserable gold coin is lying here.");
    new_descr->keyword = str_dup("coin gold");
    new_descr->description = str_dup("It's just one miserable little gold coin.");
  } else {
    obj->name = str_dup("coins gold");
    obj->short_description = str_dup(money_desc(amount));
    sprintf(buf, "%s is lying here.", money_desc(amount));
    obj->description = str_dup(CAP(buf));

    new_descr->keyword = str_dup("coins gold");
    if (amount < 10) {
      sprintf(buf, "There are %d coins.", amount);
      new_descr->description = str_dup(buf);
    } else if (amount < 100) {
      sprintf(buf, "There are about %d coins.", 10 * (amount / 10));
      new_descr->description = str_dup(buf);
    } else if (amount < 1000) {
      sprintf(buf, "It looks to be about %d coins.", 100 * (amount / 100));
      new_descr->description = str_dup(buf);
    } else if (amount < 100000) {
      sprintf(buf, "You guess there are, maybe, %d coins.",
          1000 * ((amount / 1000) + number(0, (amount / 1000))));
      new_descr->description = str_dup(buf);
    } else
      new_descr->description = str_dup("There are a LOT of coins.");
  }

  new_descr->next = NULL;
  obj->ex_description = new_descr;

  GET_OBJ_TYPE(obj) = ITEM_MONEY;
  GET_OBJ_WEAR(obj) = ITEM_WEAR_TAKE;
  GET_OBJ_VAL(obj, 0) = amount;
  GET_OBJ_COST(obj) = amount;
  obj->item_number = NOTHING;

  return obj;
}


/* Generic Find, designed to find any object/character                    */
/* Calling :                                                              */
/*  *arg     is the sting containing the string to be searched for.       */
/*           This string doesn't have to be a single word, the routine    */
/*           extracts the next word itself.                               */
/*  bitv..   All those bits that you want to "search through".            */
/*           Bit found will be result of the function                     */
/*  *ch      This is the person that is trying to "find"                  */
/*  **tar_ch Will be NULL if no character was found, otherwise points     */
/* **tar_obj Will be NULL if no object was found, otherwise points        */
/*                                                                        */
/* The routine returns a pointer to the next word in *arg (just like the  */
/* one_argument routine).                                                 */

int generic_find(char *arg, int bitvector, struct char_data * ch,
             struct char_data ** tar_ch, struct obj_data ** tar_obj)
{
  int i, found;
  char name[MAX_INPUT_LENGTH];

  one_argument(arg, name);
  if (!name || !*name)
    return (0);

  if (NULL == tar_ch)  return 0;
  if (NULL == tar_obj) return 0;

  *tar_ch = NULL;
  *tar_obj = NULL;

  if (IS_SET(bitvector, FIND_CHAR_ROOM)) {	/* Find person in room */
    if ((*tar_ch = get_char_room_vis(ch, name))) {
      return (FIND_CHAR_ROOM);
    }
  }
  if (IS_SET(bitvector, FIND_CHAR_WORLD)) {
    if ((*tar_ch = get_char_vis(ch, name))) {
      return (FIND_CHAR_WORLD);
    }
  }
  if (IS_SET(bitvector, FIND_OBJ_EQUIP)) {
    for (found = FALSE, i = 0; i < NUM_WEARS && !found; i++)
      if (GET_EQ(ch, i) && isname(name, GET_EQ(ch, i)->name) == 1) {
    *tar_obj = GET_EQ(ch, i);
    found = TRUE;
      }
    if (found) {
      return (FIND_OBJ_EQUIP);
    }
  }
  if (IS_SET(bitvector, FIND_OBJ_INV)) {
    if ((*tar_obj = get_obj_in_list_vis(ch, name, ch->carrying))) {
      return (FIND_OBJ_INV);
    }
  }
  if (IS_SET(bitvector, FIND_OBJ_ROOM)) {
    if ((*tar_obj = get_obj_in_list_vis(ch, name, world[ch->in_room].contents))) {
      return (FIND_OBJ_ROOM);
    }
  }
  if (IS_SET(bitvector, FIND_OBJ_WORLD)) {
    if ((*tar_obj = get_obj_vis(ch, name))) {
      return (FIND_OBJ_WORLD);
    }
  }
  return (0);
}


/* a function to scan for "all" or "all.x" */
int find_all_dots(char *arg)
{
  if (!strcmp(arg, "all"))
    return FIND_ALL;
  else if (!strncmp(arg, "all.", 4)) {
    strcpy(arg, arg + 4);
    return FIND_ALLDOT;
  } else
    return FIND_INDIV;
}
