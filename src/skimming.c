/******************************************************************************
 * $Id: skimming.c,v 1.5 2002/11/07 04:02:04 fang Exp $
 *   
 * File: skimming.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: skimming.c,v 
 *   Revision 1.3  2002/09/23 05:52:42  fische
 *   fisch: clearing out lua triggers part 
 
 *   Revision 1.2  2002/07/28 13:56:05  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.5  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.4  2002/03/19 02:22:19  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/03/13 12:58:37  fang
 *   TicksInRoom reset when entering gates/tar-gates.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.8  2002/01/24 21:33:20  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.7.2.2  2002/01/24 20:49:37  mud
 *   More cleanup. Removed files:  specialday.c string.c string.h
 *   Removed DONT_TRADE_WITH-stuff from shops.
 *
 *   Revision 1.7.2.1  2002/01/24 03:20:34  mud
 *   Removed some further unescessary files (player.[ch], roomprog.[ch]
 *   old2new.c). Cleaned up some other stuff (guild stuff mostly).
 *
 *   Revision 1.7  2001/11/06 10:02:16  mud
 *   Skim gates too rushes followers through gate.
 *
 *   Revision 1.6  2001/05/19 09:03:32  mud
 *   *** empty log message ***
 *
 *   Revision 1.5  2000/12/11 12:14:18  mud
 *   Removed most references to PFILEPOS() get_char_ppos(), since with the xml code ppos will(may) change every reboot
 *
 *   Revision 1.4  2000/07/08 12:30:19  mud
 *   Lowered the skimming time to half of what it was.
 *
 *   Revision 1.3  2000/04/10 22:19:53  mud
 *   New Talent System
 *
 *   Revision 1.2  2000/02/21 14:00:19  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#include "skimming.h"
#include "structs.h"
#include "utils.h"
#include "db.h"
#include "interpreter.h"
#include "spec_procs.h"
#include "comm.h"
#include "fight.h"
#include "handler.h"
#include "spells.h"
#include "act.h"

/*
 * Todo: Should be finnished by now. :)
 */
static int  __start_at = 32701;  // I made zone 327 for this. :) 
static char __room_taken[99];  // 99 rooms to a zone (No1 will always be taken)

/*
 * Shared strings/ex_descriptions for skimming rooms
 */
static char *__name = "A very dark room";
static char *__desc = "All around is empty, dark, space. A platform of some\r\n"
            "some kind occupies part of the room. Falling of the platform\r\n"
            "seems like a sure path to death.\r\n";
static char *__e_keyw = "platform skimming";
static char *__e_desc[4] = {
  "The platform looks like the foredeck of a sailing barge.\r\n",
  "The platform looks like a white marmor balcony, fit for a kings palace.\r\n",
  "The platform seems to be made up of tiles with the ancient sumbol of the Aes Sedai imprinted on them.\r\n",
  "The platform looks like a tiled floor.\r\n"
};

/*
 * Finds a free room, and return it's VNUM. 
 */
static int find_free_room(void)
{
  int i;
  for (i = 0; i < 100; i++) {
    if (!__room_taken[i])
      return (__start_at + i);
  }
  return -1; 
}

/*
 * Occupies a room. <rnum> is rooms vnum. 
 * Doesn't check if room is free, or within range.
 * Use find_free_room() to obtain a valid room_num. 
 */ 
static void occupy_room(int rnum)
{
  __room_taken[rnum - __start_at] = 1;
}

/*
 * Unoccupies a room. 
 * Same procedure as for occupy_room()
 */
static void unoccupy_room(int rnum)
{
   __room_taken[rnum - __start_at] = 0;
}

/*
 * removes a skimroom <r> from <world>. 
 * All characters in the room at this time'll die. 
 * All objects in the room at this time'll be lost forever
 */
static void remove_from_world(struct room_data *r)
{  
  struct room_data *new_world;
  struct char_data *temp_ch,*hlp;
  struct obj_data  *temp_obj,*hlp2;
  int i,rn = real_room(r->number);

  if (-1 == rn) {
    alog("SYSERR: remove_from_world()[skimming.c]: Room is not in world!\r\n");
    return;
  }
  if (r->number < __start_at || r->number > (__start_at + 99)) {
    alog("SYSERR: remove_from_world()[skimming.c]: Room is not a skim-room! (num: %d)",r->number);
    return;
  }
  /*
   * Kill off all chars in room!
   */
  for (temp_ch = r->people; NULL != temp_ch; temp_ch = hlp) {
    hlp = temp_ch->next_in_room;
    if (IS_IMMORTAL(temp_ch)) {
      send_to_char("The platform vanishes, normally you be dead, but since you're a nifty"
                   " imm person, you'll be tranfered to the Imm-zone instead. :)\r\n",temp_ch);
      char_from_room(temp_ch);
      char_to_room(temp_ch,real_room(32600));
    } else {
      send_to_char("Suddenly the platform vanishes beneath your feet, and you"
           " fall through darkness for Ages, until death comes to "
           "end your suffering!\r\n",temp_ch);
      raw_kill(temp_ch,temp_ch);
    }
  }
  /*
   * Remove all objects from room
   */
  for (temp_obj = r->contents; NULL != temp_obj; temp_obj = hlp2) {
    hlp2 = temp_obj->next_content;
    extract_obj(temp_obj);
  }
  CREATE(new_world,struct room_data,top_of_world);

  /*
   * copy over all rooms, from 0 -> (rn - 1) 
   */
  for (i = 0; i < rn; i++) {
    new_world[i] = world[i];
  }
  /*
   * Copy over (rn + 1) -> top_of_world
   */
  for (i = rn+1; i <= top_of_world; i++) {
    new_world[i-1] = world[i];
    for (temp_ch = world[i].people; NULL != temp_ch; temp_ch = temp_ch->next_in_room) {
      if (NOWHERE != temp_ch->in_room)
        temp_ch->in_room--;
    }
    for (temp_obj = world[i].contents; NULL != temp_obj; temp_obj = temp_obj->next_content) {
      if (NOWHERE != temp_obj->in_room)
    temp_obj->in_room--;
    }
  }
  free(r->ex_description); // Need to free this here. 
  free(world);
  world = new_world;
  top_of_world--;

  /*
   * Should be no need of updating exits/zcmds, since we're on the
   * top of the world! :)
   */
}

/*
 * frees a skim room. 
 * Don't do any sanitychecking stuff, like killing off all chars inna room, 
 * should've been done in other funcs. :)
 */
void free_skimroom(struct room_data *r)
{
  int number;

  if (NULL == r || NULL == r->ex_description) {
    alog("SYSERR: free_skimroom()[skimming.c]: NULL-pointer room!");
    return;
  }
  number = r->number;

  remove_from_world(r);
  unoccupy_room(number);
}  

/*
 * Adds a skimroom to the world. Shameless theft from olc. :>
 */
static void add_to_world(struct room_data *r)
{
  struct room_data *new_world;
  struct char_data *temp_ch;
  struct obj_data  *temp_obj;
  int i,found = 0,room_num = 0;
  
  if (NULL == r || NULL == r->ex_description) {
    alog("SYSERR: add_to_world()[skimming.c]: NULL-pointer room!");
    return;
  }

  CREATE(new_world, struct room_data, top_of_world + 2);
  for (i = 0; i <= top_of_world; i++) {
    if (!found) {
      if (world[i].number > r->number) {
        found = 1;
    
        new_world[i] = *r;
        room_num  = i;
      
        new_world[i + 1] = world[i];
        for (temp_ch = world[i].people; temp_ch; temp_ch = temp_ch->next_in_room) 
          if (temp_ch->in_room != -1)  
            temp_ch->in_room = i + 1;

          for (temp_obj = world[i].contents; temp_obj; temp_obj = temp_obj->next_content)
            if (temp_obj->in_room != -1)
              temp_obj->in_room = i + 1;
      } else {
        new_world[i] = world[i];
      }
    } else {
       for (temp_ch = world[i].people; temp_ch; temp_ch = temp_ch->next_in_room)
         if (temp_ch->in_room != -1)
           temp_ch->in_room = i + 1;   
              
        /* move objects */
       for (temp_obj = world[i].contents; temp_obj; temp_obj = temp_obj->next_content)
         if (temp_obj->in_room != -1)
           temp_obj->in_room = i + 1;
      
       new_world[i+1] = world[i];
    }
  }
  if (!found) {
    new_world[i] = *r;    
    room_num  = i;
  }
  free(world);
  world = new_world;
  top_of_world++;

  // SHould, once again, be no need of fiddling with
  // exits/zone-cmds/start_rooms. We're onna top, remember? :)
}
 
/*
 * 'Creates' a skimmroom, and adds it to the room_index thingy. 
 * returns 0 if failed, room_num otherwise. 
 */
static int create_skimroom(struct char_data *ch,int tto)
{
  int i;
  struct room_data *r = calloc(1,sizeof(struct room_data));

  if (NULL == r) {
     alog("SYSERR: create_skimroom()[skimming.c]: Couldn't allocate memory for skimroom!");
     return 0;
  }
  if (NOWHERE == (r->number = find_free_room())) {
    alog("SYSERR: create_skimroom()[skimming.c]: No free room for skimming!");
    free(r);
    return 0;
  }
  occupy_room(r->number);
  r->zone = (__start_at / 100);
  r->sector_type = SECT_INSIDE;

  r->name = __name;
  r->description = __desc;
  r->ex_description = calloc(1,sizeof(struct extra_descr_data));
  if (NULL == r->ex_description) {
    alog("SYSERR: create_skimroom()[skimming.c]: Couldn't allocate memory for extra description!");
    free(r);
    return 0;
  }
  r->ex_description->keyword     = __e_keyw;
  r->ex_description->description = __e_desc[number(0,3)];
  r->ex_description->next        = NULL;

  for (i = 0; i < NUM_OF_DIRS; i++) {
    r->dir_option[i] = NULL;
  }
  r->room_flags = ROOM_VERY_DARK | ROOM_DARK | ROOM_NOGATE | ROOM_NOTRACK | ROOM_NORECALL | ROOM_SOUNDPROOF | ROOM_INDOORS;
  r->wagon = NULL;
  r->light = 0;
  r->func  = 0;
  r->contents =  NULL;
  r->people =    NULL;
  r->aff =       NULL;
  r->aff_flags = 0;
  for (i = 0; i < 10; i++) {
    r->status[i] = 0;
  }
  r->skimmer   = ch;
  r->travel_to = tto;
  r->eta       = number(1,3); 		// 1 to 10 minutes skim-time
  r->skstat    = HASNT_STARTED;
  r->skim_ppos = GET_IDNUM(ch);
  add_to_world(r);
  i = r->number;
/*
 * Whe're copying the contents of <r>, so we need to free it to stop
 * mem-leaks. 
 */
  free(r);
  return (i);
}

static void enter_skim_gate(struct char_data *ch, int room)
{
  TICKS_IN_ROOM(ch) = 0;

  act("$n enters the Gate and disappears.",TRUE,ch,NULL,NULL,TO_ROOM);
  act("You enter a Gate and steps out at another place.",TRUE,ch,NULL,NULL,TO_CHAR);
  char_from_room(ch);   
  char_to_room(ch,room);
  act("$n steps out of a Gate.",TRUE,ch,NULL,NULL,TO_ROOM);
  act("You step out of a Gate at....",TRUE,ch,NULL,NULL,TO_CHAR);
  do_look(ch,"\0",0,SCMD_LOOK);
}

static SPECIAL(sk_gate)
{
  char arg1[2000];
  struct obj_data *gate1,*gate2 = (struct obj_data *) me;
  sh_int room;
  int made_by,weaver_exit = 0,was_room = -1;
  struct follow_type *k, *next;
  int was_in = ch->in_room;

  if (!CMD_IS("enter") && !CMD_IS("look") && !CMD_IS("examine") && !CMD_IS("weave") && !CMD_IS("tieweave"))
    return 0;

  argument = one_argument(argument,arg1);

  if (CMD_IS("weave") || CMD_IS("tieweave")) {
    char *tmp = strdup(arg1);
    tmp++;
    skip_spaces(&tmp);
    if (*arg1 && (!strncasecmp(tmp,"gate",strlen(tmp)) || !strncasecmp(tmp,"tar-gate",strlen(tmp)) || !strncasecmp(tmp,"skim",strlen(tmp)))) {
      send_to_char("You can't use those weaves while Skimming!\r\n",ch);
      free(tmp);
      return 1;
    }
    return 0;
  }
  if (!(gate1 = get_obj_in_list_vis(ch,arg1,world[ch->in_room].contents))) {
    return 0;
  }
  
  if (gate1 != gate2)
    return 0; 

  if (!*arg1)
    return 0;

   if (CMD_IS("enter")) {
  
    room    = GET_OBJ_VAL(gate2,2);
    made_by = GET_OBJ_VAL(gate2,1);
    gate1 = gate2->linked;

    if (GET_OBJ_VAL(gate2,1) == GET_IDNUM(ch)) {
      // must be done first if weaver is master. 
      k = ch->followers;
      while (k) {
    next = k->next;
    if (k->follower->in_room == was_in && GET_POS(k->follower) == POS_STANDING) {
      act("You jump through the gate ahead of $N.\r\n", FALSE, k->follower, 0, ch, TO_CHAR);
      enter_skim_gate(k->follower,room);
    }
    k = next;
      }

      k = ch->tailed;
      while (k) {
    next = k->next;
    if (k->follower->in_room == was_in && GET_POS(k->follower) == POS_STANDING) {
      act("Unfortunately you can't tail $N through this gate. You must eneter before $N!\r\n", 
          FALSE, k->follower, 0, ch, TO_CHAR);
    }
    k = next;
      }

      act("$n enters the Gate, and the Gate vanishes in a flash of light",TRUE,ch,NULL,NULL,TO_ROOM);
      act("You enter the Gate and it dissappears!",TRUE,ch,NULL,NULL,TO_CHAR);
      obj_from_room(gate2);
      extract_obj(gate2);
      if ((world[ch->in_room].number / 100) == (__start_at/100) && world[ch->in_room].skstat == TARGET_REACHED) {        
        weaver_exit = 1;
        was_room = ch->in_room;
      } else if ((world[room].number / 100) == (__start_at/100) && world[room].skstat == HASNT_STARTED) {
        world[room].skstat = IS_TRAVELING;        
      } 
      TICKS_IN_ROOM(ch) = 0;
      char_from_room(ch);   
      char_to_room(ch,room);
    }
    else {
      enter_skim_gate(ch,room);
    }

    if ((world[room].number / 100) == (__start_at / 100) && world[room].skstat == IS_TRAVELING)
      send_to_room("The platform starts moving through the darknes...\r\n",room);
    if (weaver_exit) 
       free_skimroom(&world[was_room]);

    if (GET_OBJ_VAL(gate1,1) == GET_IDNUM(ch)) {
      act("$n steps out of a Gate that disappears in a a flash of light!",
           TRUE,ch,NULL,NULL,TO_ROOM);
      act("You step out of a Gate that instantly disappears.",TRUE,ch,NULL,NULL,TO_CHAR);
      obj_from_room(gate1);
      extract_obj(gate1);
      do_look(ch,"\0",0,SCMD_LOOK);
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

#define SKGATE 43
void update_skims(void)
{
  int i = __start_at,time = 10,rnum;
  struct room_data *r;
  struct obj_data *gate1 = NULL,*gate2 = NULL;
 
  for (; i < __start_at+99; i++) {
    rnum = real_room(i);
    if (NOWHERE != rnum) {
      r = &world[rnum];
      if (IS_TRAVELING == r->skstat) {
        r->eta--;
        if (0 == r->eta) {
      send_to_room("The platform slows down....\r\n",real_room(i));
      r->skstat = TARGET_REACHED;
      gate1 = read_object(SKGATE,VIRTUAL);
      gate2 = read_object(SKGATE,VIRTUAL);	  
      gate1->linked = gate2;
      gate2->linked = gate1;
          GET_OBJ_VAL(gate1,0) = 4;
          GET_OBJ_VAL(gate1,1) = GET_IDNUM(r->skimmer);
          GET_OBJ_VAL(gate1,2) = r->travel_to;
          GET_OBJ_TIMER(gate1) = time;
          obj_to_room(gate1,real_room(r->number));
     
          GET_OBJ_VAL(gate2,0) = 4;
          GET_OBJ_VAL(gate2,1) = GET_IDNUM(r->skimmer);
          GET_OBJ_VAL(gate2,2) = real_room(r->number);
          GET_OBJ_TIMER(gate2)  = time;
          obj_to_room(gate2,r->travel_to);

      act("With a huge bang, a bar of light suddenly appears in the room,\r\n"
             "turning into a Gate!",TRUE,NULL,gate1,NULL,TO_ROOM);
      act("With a huge bang, a bar of light suddenly appears in the room,\r\n"  
             "turning into a Gate!",TRUE,NULL,gate2,NULL,TO_ROOM);
        } else {
          send_to_room("The platfroms speeds on through the endless darkness...\r\n",real_room(i));
        } 
      } // else: Is traveling 
    } // else: No real_room found. 
  } // for 
}

/* No 96 */
BSPELL(skim)
{
  struct obj_data *gate1 = NULL,*gate2 = NULL;
  int success,mana,time,bonus;
  int room_num,tmp1;

  if (IS_AFFECTED(ch,AFF_DREAMING)) {
    send_to_char("Sorry. No Gates in Tel'Aran'Rhiod.\r\n",ch);
    return;
  }
 
  time      = 10;
  room_num = real_room(rm->number);
  bonus    = IntBonus(ch);
  mana     = spell_info[SPELL_SKIMMING].mana + mana_add;
  success  = normal_spell_test(SPELL_SKIMMING, ch, NULL, mana, bonus);

  if (zone_table[world[room_num].zone].lock && !IS_IMMORTAL(ch)) {
    send_to_char("Sorry.  That Zone is locked!\r\n", ch);
    success = 0;
  }

  if (success) {
    if (0 == (tmp1 = create_skimroom(ch,room_num))) {
      send_to_char("Sorry. Seems the Skimming Space Time Continuum is out of sync at the moment. Try again later.\r\n",ch);
      return;
    }

    if (!IS_IMMORTAL(ch) && !test_bit(GET_KNOW(ch),world[real_room(tmp1)].number) && IS_SET(PRF_FLAGS(ch),PRF_IC)) {
      send_to_char("You don't know your destination well enough to skim there!\r\n",ch);
      free_skimroom(&world[real_room(tmp1)]);
      return;
    }
    if (NULL == (gate1 = read_object(SKGATE,VIRTUAL))) {
      send_to_char("Weird. Seems your Gate couldn't be created. Try again later, or tell an IMM if the problem remains.\r\n",ch);
      free_skimroom(&world[real_room(tmp1)]);
      return;
    }
    if (NULL == (gate2 = read_object(SKGATE,VIRTUAL))) {
      send_to_char("Weird. Seems your Gate couldn't be created. Try again later, or tell an IMM, should the problem remain!\r\n",ch);
      free_skimroom(&world[real_room(tmp1)]);
      return;
    }

    GET_OBJ_VAL(gate1,0) = 4;
    GET_OBJ_VAL(gate1,1) = GET_IDNUM(ch);
    GET_OBJ_VAL(gate1,2) = real_room(tmp1);
    GET_OBJ_TIMER(gate1) = time;
    obj_to_room(gate1,ch->in_room);

    GET_OBJ_VAL(gate2,0) = 4;              
    GET_OBJ_VAL(gate2,1) = GET_IDNUM(ch);
    GET_OBJ_VAL(gate2,2) = ch->in_room;    
    GET_OBJ_TIMER(gate2)  = time;  
    obj_to_room(gate2,real_room(tmp1));

    if (world[real_room(tmp1)].people != NULL) {
      act("With a huge bang, a bar of light suddenly appears in the room,\r\n"
          "turning into a Gate!",TRUE,world[real_room(tmp1)].people,
           NULL,NULL,TO_ROOM);
      act("With a huge bang, a bar of light suddenly appears in the room,\r\n"
        "turning into a Gate!",TRUE,world[real_room(tmp1)].people,NULL,NULL,TO_CHAR);
    }
    gate1->linked = gate2;
    gate2->linked = gate1;
    ASSIGNOBJ(SKGATE,sk_gate);

    gen_message(SPELL_SKIMMING,ch,vict,FALSE,TRUE);
    check_if_learned(ch,SPELL_SKIMMING);
  }
  GET_TAINT(ch) += add_taint(ch,(int)(mana * 0.4));

#undef SKGATE 
}

void fix_skim_gates(struct char_data *ch,long ppos)
{
  int i = __start_at;

  if (NULL == ch && ppos < 0) { 
    return;
  }

  for (; i < __start_at+99; i++) {
    if (NOWHERE != real_room(i)) {
      if ((NULL != ch && world[real_room(i)].skimmer == ch) || (ppos >= 0 && world[real_room(i)].skim_ppos == ppos))
        free_skimroom(&world[real_room(i)]);
    }
  }
}

