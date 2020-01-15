/******************************************************************************
 * $Id: graph.c,v 1.3 2002/11/07 04:02:02 fang Exp $
 *   
 * File: graph.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: graph.c,v 
 *   Revision 1.2  2002/07/28 13:56:04  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.4  2002/05/27 10:52:49  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.3  2002/03/13 13:38:12  fang
 *   Ashasense added
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.6  2002/01/31 09:14:38  mud
 *   *** empty log message ***
 *
 *   Revision 1.5  2002/01/30 09:20:44  mud
 *   shortgate added. Also made gate not choke on unknown rooms, if distance between rooms are not morre than 10 rooms
 *
 *   Revision 1.4  2001/03/30 15:25:35  mud
 *   *** empty log message ***
 *
 *   Revision 1.3  2001/01/18 12:53:47  mud
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/02/21 14:00:17  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* ************************************************************************
*   File: graph.c                                       Part of CircleMUD *
*  Usage: various graph algorithms                                        *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */


#define TRACK_THROUGH_DOORS

/* You can define or not define TRACK_THOUGH_DOORS, above, depending on
   whether or not you want track to find paths which lead through closed
   or hidden doors.
*/

#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "utils.h"
#include "constants.h"
#include "act.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "fight.h"
#include "spells.h"

struct bfs_queue_struct {
  sh_int room;
  char dir;
  struct bfs_queue_struct *next;
};

static struct bfs_queue_struct *queue_head = 0, *queue_tail = 0;

/* Utility macros */
#define MARK(room) (SET_BIT(ROOM_FLAGS(room), ROOM_BFS_MARK))
#define UNMARK(room) (REMOVE_BIT(ROOM_FLAGS(room), ROOM_BFS_MARK))
#define IS_MARKED(room) (IS_SET(ROOM_FLAGS(room), ROOM_BFS_MARK))
#define TOROOM(x, y) (world[(x)].dir_option[(y)]->to_room)
#define IS_CLOSED(x, y) (IS_SET(world[(x)].dir_option[(y)]->exit_info, EX_CLOSED))

#ifdef TRACK_THROUGH_DOORS
#define VALID_EDGE(x, y) (world[(x)].dir_option[(y)] && \
              (TOROOM(x, y) != NOWHERE) &&	\
              (!ROOM_FLAGGED(TOROOM(x, y), ROOM_NOTRACK)) && \
              (!IS_MARKED(TOROOM(x, y))))

#define B_VALID_EDGE(x,y) (world[(x)].dir_option[(y)] && \
              (TOROOM(x,y) != NOWHERE) && \
              (!IS_MARKED(TOROOM(x,y))))
#else
#define VALID_EDGE(x, y) (world[(x)].dir_option[(y)] && \
              (TOROOM(x, y) != NOWHERE) &&	\
              (!IS_CLOSED(x, y)) &&		\
              (!ROOM_FLAGGED(TOROOM(x, y), ROOM_NOTRACK)) && \
              (!IS_MARKED(TOROOM(x, y))))

#define B_VALID_EDGE(x,y) (world[(x)].dir_option[(y)] && \
              (TOROOM(x,y) != NOWHERE) &&    \
              (!IS_CLOSED(x,y)) &&           \
              (!IS_MARKED(TOROOM(x,y))))
#endif

void bfs_enqueue(sh_int room, int dir)
{
  struct bfs_queue_struct *curr;

  CREATE(curr, struct bfs_queue_struct, 1);
  curr->room = room;
  curr->dir = dir;
  curr->next = 0;

  if (queue_tail) {
    queue_tail->next = curr;
    queue_tail = curr;
  } else
    queue_head = queue_tail = curr;
}


void bfs_dequeue(void)
{
  struct bfs_queue_struct *curr;

  curr = queue_head;

  if (!(queue_head = queue_head->next))
    queue_tail = 0;
  free(curr);
}


void bfs_clear_queue(void)
{
  while (queue_head)
    bfs_dequeue();
}


/* find_first_step: given a source room and a target room, find the first
   step on the shortest path from the source to the target.

   Intended usage: in mobile_activity, give a mob a dir to go if they're
   tracking another mob or a PC.  Or, a 'track' skill for PCs.
*/
int b_find_first_step(sh_int src,sh_int target)
{
  int curr_dir;
  sh_int curr_room;

  if (src < 0 || src > top_of_world || target < 0 || target > top_of_world) {
    mlog("Illegal value passed to b_find_first_step() (graph.c)");
    return BFS_ERROR;
  }
  if (src == target)
    return BFS_ALREADY_THERE;
  for (curr_room = 0; curr_room <= top_of_world;curr_room++)
    UNMARK(curr_room);

  MARK(src);

  for (curr_dir = 0; curr_dir < NUM_OF_DIRS; curr_dir++)
    if (B_VALID_EDGE(src,curr_dir)) {
      MARK(TOROOM(src,curr_dir));
      bfs_enqueue(TOROOM(src,curr_dir),curr_dir);
    }
  
  while (queue_head) {
    if (queue_head->room == target) {
      curr_dir = queue_head->dir;
      bfs_clear_queue();
      return curr_dir;
    }
    else {
      for(curr_dir = 0; curr_dir < NUM_OF_DIRS; curr_dir++)
        if (B_VALID_EDGE(queue_head->room,curr_dir)) {
          MARK(TOROOM(queue_head->room,curr_dir));
          bfs_enqueue(TOROOM(queue_head->room,curr_dir),queue_head->dir);
        }
      bfs_dequeue();
    }
  }
  return BFS_NO_PATH;
}

int find_first_step(sh_int src, sh_int target)
{
  int curr_dir;
  sh_int curr_room;

  if (src < 0 || src > top_of_world || target < 0 || target > top_of_world) {
    mlog("Illegal value passed to find_first_step (graph.c)");
    return BFS_ERROR;
  }
  if (src == target)
    return BFS_ALREADY_THERE;

  /* clear marks first */
  for (curr_room = 0; curr_room <= top_of_world; curr_room++)
    UNMARK(curr_room);

  MARK(src);

  /* first, enqueue the first steps, saving which direction we're going. */
  for (curr_dir = 0; curr_dir < NUM_OF_DIRS; curr_dir++)
    if (VALID_EDGE(src, curr_dir)) {
      MARK(TOROOM(src, curr_dir));
      bfs_enqueue(TOROOM(src, curr_dir), curr_dir);
    }
  /* now, do the classic BFS. */
  while (queue_head) {
    if (queue_head->room == target) {
      curr_dir = queue_head->dir;
      bfs_clear_queue();
      return curr_dir;
    } else {
      for (curr_dir = 0; curr_dir < NUM_OF_DIRS; curr_dir++)
    if (VALID_EDGE(queue_head->room, curr_dir)) {
      MARK(TOROOM(queue_head->room, curr_dir));
      bfs_enqueue(TOROOM(queue_head->room, curr_dir), queue_head->dir);
    }
      bfs_dequeue();
    }
  }

  return BFS_NO_PATH;
}

/*
 * return -HIGH- number if no path between rooms!
 */
int steps_between(int rroom1, int rroom2) 
{
  int dir;
  int cnt = 0;

  if (-1 == rroom1 || -1 == rroom2) {
    return 0x0FFFFFF;
  }
  
  do {
    dir = find_first_step(rroom1,rroom2);

    switch (dir) {
    case BFS_ERROR:
      return 0x0FFFFFF;
      break;
    case BFS_ALREADY_THERE:
      return cnt;
      break;
    case BFS_NO_PATH:
      return 0x0FFFFFF;
      break;
    default:
      cnt++;
      rroom1 = world[rroom1].dir_option[dir]->to_room;
      break;
    }
  } while(1 && cnt < 10000);

  return 0x0FFFFFF;
}

/************************************************************************
*  Functions and Commands which use the above fns		        *
************************************************************************/

ACMD(do_track)
{
  struct char_data *vict;
  int dir, num, loops = 0;

  if (!GET_SKILL(ch, SKILL_TRACK)) {
    send_to_char("You have no idea how.\r\n", ch);
    return;
  }
  one_argument(argument, arg);

  if (!*arg) 
    {
    send_to_char("Who are you trying to track?\r\n", ch);
    return;
    }
  
  if (!(vict = get_char_pointer(arg)) || GET_LEVEL(vict)>=LVL_IMMORT) 
      {
      send_to_char("No-one around by that name.\r\n",ch);
      return;
      }

  if ( IS_AFFECTED(vict,AFF_NOTRACK) ) 
      {
      send_to_char("You sense no trail.\r\n", ch);
      return;
      }



  dir = find_first_step(ch->in_room, vict->in_room);

  switch (dir) {
  case BFS_ERROR:
    send_to_char("Hmm.. something seems to be wrong.\r\n", ch);
    break;
  case BFS_ALREADY_THERE:
    send_to_char("That person must already be somewhere in the area.\r\n", ch);
    break;
  case BFS_NO_PATH:
    send_to_char("You can't sense a trail to that person from here.\r\n", ch);
    break;
  default:
    num = generic_result_mod(ch,SKILL_TRACK,NULL,FALSE,(GET_INT(ch) - 15));
    if ((num == TOTAL_FUCKUP) || (num == TOTAL_FAILURE))
      do {
    loops++;
    dir = number(0, NUM_OF_DIRS - 1);
      } while (!CAN_GO(ch, dir) && loops < 100); /* If we don't have a control here 
                          the game would hang up if the player is
                          in a closed room */
    if (loops >= 100)
    {
      sprintf(buf, "You failed to sense the track of that person from this room.\r\n");
      send_to_char(buf, ch);
      break;
    }
    else
    {
      sprintf(buf, "You sense a trail %s from here!\r\n", dirs[dir]);
      send_to_char(buf, ch);
      break;
    }
  }
}


void mob_goto(struct char_data *ch, int to_room)
{
  int dir;
  
  if (!IS_NPC(ch))
    return;
    
  if (ch->in_room == to_room) 
    GET_MOB_GOINGTO(ch) = 0;
  else {  
    dir = find_first_step(ch->in_room, to_room);
    if (dir >= 0)
      perform_move(ch, dir, 1);
    else
      GET_MOB_GOINGTO(ch) = 0;
  }
}

void hunt_victim(struct char_data * ch)
{

  int dir;
  byte found;
  struct char_data *tmp;

  if (!ch || !HUNTING(ch))
    return;

  /* make sure the char still exists */
  for (found = 0, tmp = character_list; tmp && !found; tmp = tmp->next)
    if (HUNTING(ch) == tmp)
      found = 1;

  if (!found) {
    do_say(ch, "Damn!  My prey is gone!!", 0, 0);
    HUNTING(ch) = 0;
    return;
  }
  dir = find_first_step(ch->in_room, HUNTING(ch)->in_room);
  if (dir < 0) {
    sprintf(buf, "Damn!  Lost %s!", HMHR(HUNTING(ch)));
    do_say(ch, buf, 0, 0);
    HUNTING(ch) = 0;
    return;
  } else {
    perform_move(ch, dir, 1);
    if (ch->in_room == HUNTING(ch)->in_room)
      hit(ch, HUNTING(ch), TYPE_UNDEFINED);
    return;
  }
}

static ACMD(ashasense) 
{
  struct char_data *bonded = NULL,*tmp;
  int dir,found;
  long vid = 0, hid = 0;

  if (!GET_ASHABONDED(ch)) {
    send_to_char("Hey! You aren't bonded!\r\n",ch);
    return;
  }

  vid = GET_ASHABONDED(ch);
  hid = GET_ASHABONDED(ch);
  SET_BIT(hid, (1 << 30));
  REMOVE_BIT(vid, (1 << 30));
  if (!IS_SET(GET_ASHABONDED(ch), (1 << 30))) {
    for(found = 0, tmp = character_list; tmp && !found; tmp = tmp->next) {
      if (GET_ASHABONDED(tmp) == hid) {
        found = TRUE;
        bonded = tmp;
      }
    }
    if (!found) {
      send_to_char("Sorry, your bond holder isn't on.\r\n",ch);
      return;
    }
  }
  else { /* The Sedai wanna track one of her warder(s) we need an argument here! */
    if (!*arg) {
      send_to_char("Sense who?\r\n",ch);
      return;
    }
    if (!(bonded = get_player(arg, 0))) {
      send_to_char("Ehh? That person isn't on you know.\r\n",ch);
      return;
    }
    if (bonded == ch) {
      send_to_char("Not much use in sensing yourself stoopid!\r\n",ch);
      return;
    }
    if (GET_ASHABONDED(bonded) != vid) {
      send_to_char("Hey, you can only sense your own bonded!\r\n",ch);
      return;
    }
  }

  /* We should have the person to track in bonded now! */
  dir = b_find_first_step(ch->in_room,bonded->in_room);

  switch (dir) {
  case BFS_ERROR:
    send_to_char("Something seems to have gone wrong!\r\n",ch);
    break;
  case BFS_ALREADY_THERE:
    send_to_char("You are in the same room right now!\r\n",ch);
    break;
  case BFS_NO_PATH:
    send_to_char("There is no way to get to that person from here!\r\n",ch);
    break;
  default:
    sprintf(buf,"You can sense %s to the %s.\r\n",GET_NAME(bonded),dirs[dir]);
    send_to_char(buf,ch);
    break;
  }
}

/* When finnished should allow a sedais/warder to sense  */
/* the direction to their counterpart, nomatter if there */
/* is a NOTRACK room in the way.			 */
ACMD(bondsense)
{
  struct char_data *bonded = NULL,*tmp;
  int dir,found;
  long vid = 0, hid = 0;

  one_argument(argument,arg);

  if (SCMD_ASHASENSE == subcmd) {
    ashasense(ch,argument,cmd,subcmd);
    return;
  }

  if (!GET_BONDED(ch)) {
    send_to_char("Hey! You aren't bonded!\r\n",ch);
    return;
  }
  vid = GET_BONDED(ch);
  hid = GET_BONDED(ch);
  SET_BIT(hid, (1 << 30));
  REMOVE_BIT(vid, (1 << 30));
  if (!IS_SET(GET_BONDED(ch), (1 << 30))) {
    for(found = 0, tmp = character_list; tmp && !found; tmp = tmp->next)
      if (GET_BONDED(tmp) == hid) {
        found = TRUE;
        bonded = tmp;
      }
    
    if (!found) {
      send_to_char("Sorry, your bond holder isn't on.\r\n",ch);
      return;
    }
  }
  else { /* The Sedai wanna track one of her warder(s) we need an argument here! */
    if (!*arg) {
      send_to_char("Sense who?\r\n",ch);
      return;
    }
    if (!(bonded = get_player(arg, 0))) {
      send_to_char("Ehh? That person isn't on you know.\r\n",ch);
      return;
    }
    if (bonded == ch) {
      send_to_char("Not much use in sensing yourself stoopid!\r\n",ch);
      return;
    }
    if (GET_BONDED(bonded) != vid) {
      send_to_char("Hey, you can only sense your own bonded!\r\n",ch);
      return;
    }
  }

  /* We should have the person to track in bonded now! */
  dir = b_find_first_step(ch->in_room,bonded->in_room);

  switch (dir) {
    case BFS_ERROR:
      send_to_char("Something seems to have gone wrong!\r\n",ch);
      break;
    case BFS_ALREADY_THERE:
      send_to_char("You are in the same room right now!\r\n",ch);
      break;
    case BFS_NO_PATH:
      send_to_char("There is no way to get to that person from here!\r\n",ch);
      break;
     default:
       sprintf(buf,"You can sense %s to the %s.\r\n",GET_NAME(bonded),dirs[dir]);
       send_to_char(buf,ch);
       break;
  }
}
