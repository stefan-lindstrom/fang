/******************************************************************************
 * $Id: redit.c,v 1.5 2002/11/07 04:02:04 fang Exp $
 *   
 * File: redit.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: redit.c,v 
 *   Revision 1.4  2002/10/12 06:30:15  fan
 *   fisch: converted boards to xml and made them dynamic not coded
 
 *   Revision 1.3  2002/09/23 05:52:42  fische
 *   fisch: clearing out lua triggers part 
 
 *   Revision 1.2  2002/07/28 13:56:05  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.5  2002/06/17 08:14:15  fang
 *   *** empty log message ***
 *
 *   Revision 1.4  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.3  2002/03/13 14:10:59  fang
 *   rcopy now uses explicit building permissions.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.17  2002/01/24 21:33:17  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.16.2.1  2002/01/24 03:20:33  mud
 *   Removed some further unescessary files (player.[ch], roomprog.[ch]
 *   old2new.c). Cleaned up some other stuff (guild stuff mostly).
 *
 *   Revision 1.16  2001/12/07 16:00:46  mud
 *   *** empty log message ***
 *
 *   Revision 1.15  2001/05/22 09:07:06  mud
 *   DG-Scripts for Fang.
 *
 *   Revision 1.14  2001/01/31 15:44:54  mud
 *   *** empty log message ***
 *
 *   Revision 1.13  2001/01/24 14:23:58  mud
 *   *** empty log message ***
 *
 *   Revision 1.12  2001/01/23 12:09:45  mud
 *   *** empty log message ***
 *
 *   Revision 1.11  2001/01/19 17:51:51  mud
 *   Added line numbers to the editor.
 *
 *   Revision 1.10  2000/12/14 19:55:55  mud
 *   *** empty log message ***
 *
 *   Revision 1.9  2000/12/01 09:17:10  mud
 *   Further safety checks added to sedit/medit/redit
 *
 *   Revision 1.8  2000/10/31 01:28:10  mud
 *   added multiple recipients for mudmail. 5 should be enough..
 *
 *   Revision 1.7  2000/09/04 03:34:40  mud
 *   Warnings removed...
 *
 *   Revision 1.6  2000/03/01 15:15:45  mud
 *   Named specials added, saved in room XML, and setable in redit
 *
 *   Revision 1.5  2000/03/01 12:54:36  mud
 *   General cleanup off stuff
 *
 *   Revision 1.4  2000/03/01 11:48:01  mud
 *   Rcopy function added.
 *
 *   Revision 1.3  2000/02/29 16:41:32  mud
 *   XML saving/parsing should actually work now! :)
 *
 *   Revision 1.2  2000/02/21 14:00:18  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*  _TwyliteMud_ by Rv.                          Based on CircleMud3.0bpl9 *
*    				                                          *
*  OasisOLC - redit.c 		                                          *
*    				                                          *
*  Copyright 1996 Harvey Gilpin.                                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*. Original author: Levork .*/

#include "config.h"
#include "constants.h"
#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "comm.h"
#include "utils.h"
#include "db.h"
#include "olc.h"
#include "xmlRooms.h"
#include "namedspec.h"
#include "shop.h"
#include "dg_olc.h"

/*------------------------------------------------------------------------*/

#define  W_EXIT(room, num) (world[(room)].dir_option[(num)])

/*------------------------------------------------------------------------*\

More utils stuff, rcopy for instance. :)

\*------------------------------------------------------------------------*/

static void copy_exdesc(struct room_data *t, struct room_data *s)
{
  struct extra_descr_data *e,*tmp;

  if (NULL != (e = s->ex_description)) {
    while(e) {
      tmp = calloc(1,sizeof(struct extra_descr_data));
      tmp->keyword = strdup(e->keyword);
      tmp->description = strdup(e->description);
      tmp->next = t->ex_description;
      t->ex_description = tmp;

      e = e->next;
    }
  }
}

static void copy_exits(struct room_data *t, struct room_data *s)
{
  int i;
  struct room_direction_data *d,*tmp;

  for (i = 0; i < NUM_OF_DIRS; i++) {
    if (NULL != (d = s->dir_option[i])) {
      tmp = calloc(1,sizeof(struct room_direction_data));
      tmp->general_description = strdup(d->general_description);
      tmp->keyword = strdup(d->keyword);
      tmp->exit_info = d->exit_info;
      tmp->key = d->key;
      tmp->to_room = d->to_room;
      t->dir_option[i] = tmp;
    } else {
      t->dir_option[i] = NULL;
    }
  }
}

/*
 * A lof of copy-paste from redit_save_internally to be able
 * to insert a totally fresh room in world array without going through 
 * the hassle of putting it through default OLC stuff. 
 */
static void insert_fresh_room(struct room_data *new) 
{
  int i, j, room_num = 0, found = 0;
  struct room_data *new_world;
  struct char_data *temp_ch;
  struct obj_data *temp_obj;

  // Since this is used -only- to insert 'fresh' rooms, there's no 
  // need to check if the room already exists. :)>
  CREATE(new_world, struct room_data, top_of_world + 2);

  /* count thru world tables */
  for (i = 0; i <= top_of_world; i++) {
    if (!found) {
      if (world[i].number > new->number) {
    found = 1;
    new_world[i] = *new;
    room_num = i;
    
    /* copy from world to new_world + 1 */
    new_world[i + 1] = world[i];

    /* people in this room must have their numbers moved */
    for (temp_ch = world[i].people; temp_ch; temp_ch = temp_ch->next_in_room)
      if (temp_ch->in_room != -1)
        temp_ch->in_room = i + 1;

    /* move objects */
    for (temp_obj = world[i].contents; temp_obj; temp_obj = temp_obj->next_content)
      if (temp_obj->in_room != -1)
        temp_obj->in_room = i + 1;
    
      } else  { /*.   Not yet placed, copy straight over .*/
    new_world[i] = world[i];
      }
    } else { /*. Already been found  .*/
      /* people in this room must have their in_rooms moved */
      for (temp_ch = world[i].people; temp_ch; temp_ch = temp_ch->next_in_room)
    if (temp_ch->in_room != -1)
      temp_ch->in_room = i + 1;

      /* move objects */
      for (temp_obj = world[i].contents; temp_obj; temp_obj = temp_obj->next_content)
    if (temp_obj->in_room != -1)
      temp_obj->in_room = i + 1;

      new_world[i + 1] = world[i];
    }
  }
  if (!found) { /*. Still not found, insert at top of table .*/
    new_world[i] = *new;
    room_num  = i;
  }

  /* copy world table over */
  free(world);
  world = new_world;
  top_of_world++;

  if (room_num <= r_mortal_start_room)
    r_mortal_start_room++;
  if (room_num <= r_immort_start_room)
    r_immort_start_room++;
  if (room_num <= r_frozen_start_room)
    r_frozen_start_room++;
  
  /* Update world exits */
  for (i = 0; i < top_of_world + 1; i++)
    for (j = 0; j < NUM_OF_DIRS; j++)
      if (W_EXIT(i, j))
    if (W_EXIT(i, j)->to_room >= room_num)
      W_EXIT(i, j)->to_room++;

  olc_add_to_save_list(zone_table[real_zone(new->number)].number, OLC_SAVE_ROOM);
}

/** Usage rcopy <flags> <src> <target> **/
/** flags: -x copy ex-descs, -e copy exits, only one flag  **/
/** arg though, so for both exits and exdescs, use -xe or -ex **/
ACMD(do_rcopy)
{
  int copyexit = 0, copydesc = 0,i;
  char arg1[1000],arg2[1000],arg3[1000],*tmp;
  int targ, src,rtarg,rsrc;
  struct room_data *srm,*trm;

  skip_spaces(&argument);
  if (!*argument) {
    send_to_char("Usage: rcopy [-<x|e>] <srcvnum> <targvnum>\r\n",ch);
    return;
  }

  tmp = two_arguments(argument,arg1,arg2);
  one_argument(tmp,arg3);

  if (!*arg1 || !*arg2 || ('-' == *arg1 && (!*arg3 || !*arg2))) {
    send_to_char("Usage: rcopy [-<x|e>] <srcvnum> <targvnum>\r\n",ch);
    return;
  }

  if ('-' == *arg1) {
    for (i = 1; arg1[i] ; i++) {
      switch(arg1[i]) {
      case 'X':
      case 'x':
    copydesc = 1;
    break;
      case 'E':
      case 'e':
    copyexit = 1;
    break;
      default:
    asend_to_char(ch,"%c is not a valid flag!\r\n",arg1[i]);
    return;
      } // Switch
    } // For
    targ = atoi(arg3);
    src  = atoi(arg2);
  } else {
    targ = atoi(arg2);
    src  = atoi(arg1);
  }

  rtarg = real_room(targ);
  rsrc   = real_room(src);

  if (-1 == rsrc) {
    asend_to_char(ch,"Src room with vnum %d doesn't exists.\r\n",src);
    return;
  }
  if (-1 != rtarg) {
    asend_to_char(ch,"Targ room with vnum %d already exists.\r\n",targ);
    return;
  }

  if (!allowed_to_edit_room(ch,targ)) {
    send_to_char("You do not have permission to edit this zone.\r\n", ch);
    return;
  }

  if (check_lock(targ,SCMD_OLC_REDIT)) {
    send_to_char("That room is currently being edited!\r\n", ch);
    return;
  }
  srm = (world+rsrc);
  trm = calloc(1,sizeof(struct room_data));
  *trm = *srm;
  trm->number = targ;
  trm->zone = zone_table[real_zone(targ)].number;
  trm->name = strdup(srm->name);
  trm->description = strdup(srm->description);
  trm->ex_description = 0;

  for (i = 0; i < NUM_OF_DIRS; i++)
    trm->dir_option[i] = NULL;

  trm->wagon = NULL;
  trm->light = 0;
  trm->contents = NULL;
  trm->people = NULL;
  trm->aff = NULL;
  trm->aff_flags = 0;
  trm->skimmer = NULL;
  trm->travel_to = 0;
  trm->eta = 0;
  trm->skstat = 0;
  trm->skim_ppos = 0;

  if (copydesc) {
    copy_exdesc(trm,srm);
  }
  if (copyexit) {
    copy_exits(trm,srm);
  }

  insert_fresh_room(trm);
  send_to_char("Room copied ok! Don't forget to save now!\r\n",ch);
  sprintf(buf, "OLC: %s copied room %d to room %d",  GET_NAME(ch), src, targ);
  mudlog(buf, CMP, LVL_BUILDER, TRUE);
}

/*------------------------------------------------------------------------*\
  Utils and exported functions.
\*------------------------------------------------------------------------*/

void redit_setup_new(struct descriptor_data *d)
{ 
  CREATE(OLC_ROOM(d), struct room_data, 1);
  OLC_ROOM(d)->name = str_dup("An unfinished room");
  OLC_ROOM(d)->description = str_dup("You are in an unfinished room.\r\n");
  OLC_ITEM_TYPE(d) = WLD_TRIGGER;
  redit_disp_menu(d);
  OLC_VAL(d) = 0;
}

/*------------------------------------------------------------------------*/

void redit_setup_existing(struct descriptor_data *d, int real_num)
{ 
  struct room_data *room;
  struct trig_proto_list *proto, *fproto;
  int counter;
  
  /*. Build a copy of the room .*/
  CREATE (room, struct room_data, 1);
  *room = world[real_num];
  /* allocate space for all strings  */
  if (world[real_num].name)
    room->name = str_dup (world[real_num].name);
  if (world[real_num].description)
    room->description = str_dup (world[real_num].description);

  /* exits - alloc only if necessary */
  for (counter = 0; counter < NUM_OF_DIRS; counter++)
  { if (world[real_num].dir_option[counter])
    { CREATE(room->dir_option[counter], struct room_direction_data, 1);
      /* copy numbers over */
      *room->dir_option[counter] = *world[real_num].dir_option[counter];
      /* mlloc strings */
      if (world[real_num].dir_option[counter]->general_description)
        room->dir_option[counter]->general_description =
          str_dup(world[real_num].dir_option[counter]->general_description);
      if (world[real_num].dir_option[counter]->keyword)
        room->dir_option[counter]->keyword =
          str_dup(world[real_num].dir_option[counter]->keyword);
    }
  }
  
  /*. Extra descriptions if necessary .*/ 
  if (world[real_num].ex_description) 
  { struct extra_descr_data *this, *temp, *temp2;
    CREATE (temp, struct extra_descr_data, 1);
    room->ex_description = temp;
    for (this = world[real_num].ex_description; this; this = this->next)
    { if (this->keyword)
        temp->keyword = str_dup (this->keyword);
      if (this->description)
        temp->description = str_dup (this->description);
      if (this->next)
      { CREATE (temp2, struct extra_descr_data, 1);
    temp->next = temp2;
    temp = temp2;
      } else
        temp->next = NULL;
    }
  }
  
  if (SCRIPT(&world[real_num]))
   script_copy(room, &world[real_num], WLD_TRIGGER);
  proto = world[real_num].proto_script;
  while (proto) {
   CREATE(fproto, struct trig_proto_list, 1);
   fproto->vnum = proto->vnum;
   if (room->proto_script==NULL)
    room->proto_script = fproto;
   proto = proto->next;
   fproto = fproto->next; /* NULL */
  }

  /*. Attatch room copy to players descriptor .*/
  OLC_ROOM(d) = room;
  OLC_VAL(d) = 0;
  OLC_ITEM_TYPE(d) = WLD_TRIGGER;
  dg_olc_script_copy(d);
  redit_disp_menu(d);
}

/*------------------------------------------------------------------------*/
      
#define ZCMD (zone_table[zone].cmd[cmd_no])

void redit_save_internally(struct descriptor_data *d)
{ int i, j, room_num, found = 0;
  struct room_data *new_world;
  struct char_data *temp_ch;
  struct obj_data *temp_obj;
   
  room_num = real_room(OLC_NUM(d));
  OLC_ROOM(d)->zone = real_zone(OLC_NUM(d));
  if (room_num > 0) 
  { /*. Room exits: move contents over then free and replace it .*/
    OLC_ROOM(d)->contents = world[room_num].contents;
    OLC_ROOM(d)->people = world[room_num].people;
    free_room(world + room_num);
    world[room_num] = *OLC_ROOM(d);
    world[room_num].proto_script = OLC_SCRIPT(d);
  } else 
  { /*. Room doesn't exist, hafta add it .*/

    CREATE(new_world, struct room_data, top_of_world + 2);

    /* count thru world tables */
    for (i = 0; i <= top_of_world; i++) 
    { if (!found) {
        /*. Is this the place? .*/
        if (world[i].number > OLC_NUM(d)) 
    { found = 1;

      new_world[i] = *(OLC_ROOM(d));
      new_world[i].number = OLC_NUM(d);
      new_world[i].func = NULL;
          new_world[i].proto_script = OLC_SCRIPT(d);
          room_num  = i;
    
      /* copy from world to new_world + 1 */
          new_world[i + 1] = world[i];
          /* people in this room must have their numbers moved */
      for (temp_ch = world[i].people; temp_ch; temp_ch = temp_ch->next_in_room)
        if (temp_ch->in_room != -1)
          temp_ch->in_room = i + 1;

      /* move objects */
      for (temp_obj = world[i].contents; temp_obj; temp_obj = temp_obj->next_content)
        if (temp_obj->in_room != -1)
          temp_obj->in_room = i + 1;
        } else 
        { /*.   Not yet placed, copy straight over .*/
      new_world[i] = world[i];
        }
      } else 
      { /*. Already been found  .*/
 
        /* people in this room must have their in_rooms moved */
        for (temp_ch = world[i].people; temp_ch; temp_ch = temp_ch->next_in_room)
      if (temp_ch->in_room != -1)
        temp_ch->in_room = i + 1;

        /* move objects */
        for (temp_obj = world[i].contents; temp_obj; temp_obj = temp_obj->next_content)
      if (temp_obj->in_room != -1)
        temp_obj->in_room = i + 1;

        new_world[i + 1] = world[i];
      }
    }
    if (!found)
    { /*. Still not found, insert at top of table .*/
      new_world[i] = *(OLC_ROOM(d));
      new_world[i].number = OLC_NUM(d);
      new_world[i].func = NULL;
      new_world[i].number = OLC_NUM(d);
      room_num  = i;
    }

    /* copy world table over */
    free(world);
    world = new_world;
    top_of_world++;

/* 
  Not needed anymore, since we're dealing with vnums in
   reset tables now, not rnums /Fredrik 990421

    for (zone = 0; zone <= top_of_zone_table; zone++)
      for (cmd_no = 0; ZCMD.command != 'S'; cmd_no++)
        switch (ZCMD.command)
        {
          case 'M':
          case 'O':
            if (ZCMD.arg3 >= room_num)
              ZCMD.arg3++;
        break;
          case 'D':
          case 'R':
            if (ZCMD.arg1 >= room_num)
              ZCMD.arg1++;
          case 'G':
          case 'P':
          case 'E':
          case '*':
            break;
          default:
            alog("SYSERR: OLC: redit_save_internally: Unknown command in zone %d.", zone);
        }
*/
    /* update load rooms, to fix creeping load room problem */
    if (room_num <= r_mortal_start_room)
      r_mortal_start_room++;
    if (room_num <= r_immort_start_room)
      r_immort_start_room++;
    if (room_num <= r_frozen_start_room)
      r_frozen_start_room++;

    /*. Update world exits .*/
    for (i = 0; i < top_of_world + 1; i++)
      for (j = 0; j < NUM_OF_DIRS; j++)
        if (W_EXIT(i, j))
          if (W_EXIT(i, j)->to_room >= room_num)
        W_EXIT(i, j)->to_room++;

  }
  update_shopflags( room_num );
  assign_triggers(&world[room_num], WLD_TRIGGER);
  olc_add_to_save_list(zone_table[OLC_ZNUM(d)].number, OLC_SAVE_ROOM);
}


/*------------------------------------------------------------------------*/

void redit_save_to_disk(struct descriptor_data *d)
{
  FILE *fp;
  char tname[100],fname[100];

  sprintf(fname, "%s/%d.wld", WLD_PREFIX, zone_table[OLC_ZNUM(d)].number);
  sprintf(tname,"%s/%d.tmp",WLD_PREFIX, zone_table[OLC_ZNUM(d)].number);

  if (!(fp = fopen(tname, "w+")))
  { mudlog("SYSERR: OLC: Cannot open room file!", BRF, LVL_BUILDER, TRUE);
    return;
  }

  room_save_zone_to_file((zone_table[OLC_ZNUM(d)].number * 100),zone_table[OLC_ZNUM(d)].top,fp);
  fclose(fp);

  remove(fname);
  rename(tname,fname);

  olc_remove_from_save_list(zone_table[OLC_ZNUM(d)].number, OLC_SAVE_ROOM);
}

/*------------------------------------------------------------------------*/

void free_room(struct room_data *room)
{ int i;
  struct extra_descr_data *this, *next;

  if (room->name)
    free(room->name);
  if (room->description)
    free(room->description);

  /*. Free exits .*/
  for (i = 0; i < NUM_OF_DIRS; i++) { 
    if (room->dir_option[i]) { 
      if (room->dir_option[i]->general_description)
        free(room->dir_option[i]->general_description);
      if (room->dir_option[i]->keyword)
        free(room->dir_option[i]->keyword);
      free(room->dir_option[i]);
    }
  }

  /*. Free extra descriptions .*/
  for (this = room->ex_description; this; this = next) { 
    next = this->next;

    if (this->keyword)
      free(this->keyword);
    if (this->description)
      free(this->description);
    free(this);
  }
}

/**************************************************************************
 Menu functions 
 **************************************************************************/

/* For specials */
void redit_display_specials(struct descriptor_data * d)
{
  makeSpecialList(d->character);
  send_to_char("\r\n",d->character);

  OLC_MODE(d) = REDIT_NAMED_SPEC;
}

/* For extra descriptions */
void redit_disp_extradesc_menu(struct descriptor_data * d)
{
  struct extra_descr_data *extra_desc = OLC_DESC(d);
  
  sprintf(buf, "[H[J"
    "%s1%s) Keyword: %s%s\r\n"
    "%s2%s) Description:\r\n%s%s\r\n"
        "%s3%s) Goto next description: ",

    grn, nrm, yel, extra_desc->keyword ? extra_desc->keyword : "<NONE>",
    grn, nrm, yel, extra_desc->description ?  extra_desc->description : "<NONE>",
        grn, nrm
  );
  
  if (!extra_desc->next)
    strcat(buf, "<NOT SET>\r\n");
  else
    strcat(buf, "Set.\r\n");
  strcat(buf, "Enter choice (0 to quit) : ");
  send_to_char(buf, d->character);
  OLC_MODE(d) = REDIT_EXTRADESC_MENU;
}

/* For exits */
void redit_disp_exit_menu(struct descriptor_data * d)
{
  /* if exit doesn't exist, alloc/create it */
  if(NULL == OLC_EXIT(d))
    CREATE(OLC_EXIT(d), struct room_direction_data, 1);

  /* weird door handling! */
  if ((NULL != OLC_EXIT(d)) && IS_SET(OLC_EXIT(d)->exit_info, EX_ISDOOR)) 
  {
    if (IS_SET(OLC_EXIT(d)->exit_info, EX_PICKPROOF))
      strcpy(buf2, "Pickproof");
    else
      strcpy(buf2, "Is a door");
    if (IS_SET(OLC_EXIT(d)->exit_info, EX_HIDDEN))
      strcat(buf2, " and hidden");
  } else
    strcpy(buf2, "No door");

  get_char_cols(d->character);
  sprintf(buf, "[H[J"
    "%s1%s) Exit to     : %s%d\r\n"
    "%s2%s) Description :-\r\n%s%s\r\n"
    "%s3%s) Door name   : %s%s\r\n"
    "%s4%s) Key         : %s%d\r\n"
    "%s5%s) Door flags  : %s%s\r\n"
    "%s6%s) Purge exit.\r\n"
    "Enter choice, 0 to quit : ",

    grn, nrm, cyn, world[OLC_EXIT(d)->to_room].number,
        grn, nrm, yel, OLC_EXIT(d)->general_description ? OLC_EXIT(d)->general_description : "<NONE>",
    grn, nrm, yel, OLC_EXIT(d)->keyword ? OLC_EXIT(d)->keyword : "<NONE>",
    grn, nrm, cyn, OLC_EXIT(d)->key,
    grn, nrm, cyn, buf2, grn, nrm
  );

  send_to_char(buf, d->character);
  OLC_MODE(d) = REDIT_EXIT_MENU;
}

/* For exit flags */
void redit_disp_exit_flag_menu(struct descriptor_data * d)
{
  get_char_cols(d->character);
  sprintf(buf,  "%s0%s) No door\r\n"
        "%s1%s) Closeable door\r\n"
        "%s2%s) Pickproof\r\n"
        "%s3%s) Closeable and hidden while closed\r\n"
        "%s4%s) Pickproof and hidden while closed\r\n"
        "Enter choice : ",
        grn, nrm, grn, nrm, grn, nrm, grn, nrm, grn, nrm
  );
  send_to_char(buf, d->character);
}

/* For room flags */
void redit_disp_flag_menu(struct descriptor_data * d)
{ int counter, columns = 0;

  get_char_cols(d->character);
  send_to_char("[H[J", d->character);
  for (counter = 0; counter < NUM_ROOM_FLAGS; counter++) 
  { sprintf(buf, "%s%2d%s) %-20.20s ",
        grn, counter + 1, nrm, room_bits[counter]);
    if(!(++columns % 2))
      strcat(buf, "\r\n");
    send_to_char(buf, d->character);
  }
  sprintbit(OLC_ROOM(d)->room_flags, room_bits, buf1);
  sprintf(buf, 
    "\r\nRoom flags: %s%s%s\r\n"
    "Enter room flags, 0 to quit : ",
    cyn, buf1, nrm
  );
  send_to_char(buf, d->character);
  OLC_MODE(d) = REDIT_FLAGS;
}

/* for sector type */
void redit_disp_sector_menu(struct descriptor_data * d)
{ int counter, columns = 0;

  send_to_char("[H[J", d->character);
  for (counter = 0; counter < NUM_ROOM_SECTORS; counter++) {
    sprintf(buf, "%s%2d%s) %-20.20s ",
        grn, counter, nrm, sector_types[counter]);
    if(!(++columns % 2))
      strcat(buf, "\r\n");
    send_to_char(buf, d->character);
  }
  send_to_char("\r\nEnter sector type : ", d->character);
  OLC_MODE(d) = REDIT_SECTOR;
}

/* the main menu */
void redit_disp_menu(struct descriptor_data * d)
{ struct room_data *room;

  get_char_cols(d->character);
  room = OLC_ROOM(d);

  sprintbit((long) room->room_flags, room_bits, buf1);
  sprinttype(room->sector_type, sector_types, buf2);
  sprintf(buf,
    "[H[J"
    "-- Room number : [%s%ld%s]  	Room zone: [%s%d%s]\r\n"
    "%s1%s) Name           : %s%s\r\n"
    "%s2%s) Description    :\r\n%s%s"
    "%s3%s) Room flags     : %s%s\r\n"
    "%s4%s) Sector type    : %s%s\r\n"
    "%s5%s) Exit north     : %s%d\r\n"
    "%s6%s) Exit east      : %s%d\r\n"
    "%s7%s) Exit south     : %s%d\r\n"
    "%s8%s) Exit west      : %s%d\r\n"
    "%s9%s) Exit up        : %s%d\r\n"
    "%sA%s) Exit down      : %s%d\r\n"
    "%sB%s) Exit northeast : %s%d\r\n"
    "%sC%s) Exit southeast : %s%d\r\n"
    "%sD%s) Exit southwest : %s%d\r\n"
    "%sE%s) Exit northwest : %s%d\r\n"  	  	  	  	
    "%sF%s) Extra descriptions menu\r\n"
    "%sH%s) Special        : %s%s\r\n"
        "%sS%s) Script         : %s%s\r\n"
    "%sQ%s) Quit\r\n"
    "Enter choice : ",

    cyn, OLC_NUM(d), nrm,
    cyn, zone_table[OLC_ZNUM(d)].number, nrm,
    grn, nrm, yel, room->name,
    grn, nrm, yel, room->description,
    grn, nrm, cyn, buf1,
    grn, nrm, cyn, buf2,
    grn, nrm, cyn, room->dir_option[NORTH] ?
          world[room->dir_option[NORTH]->to_room].number : -1,
    grn, nrm, cyn, room->dir_option[EAST] ?
          world[room->dir_option[EAST]->to_room].number : -1,
    grn, nrm, cyn, room->dir_option[SOUTH] ? 
          world[room->dir_option[SOUTH]->to_room].number : -1,
    grn, nrm, cyn, room->dir_option[WEST] ? 
          world[room->dir_option[WEST]->to_room].number : -1,
    grn, nrm, cyn, room->dir_option[UP] ? 
          world[room->dir_option[UP]->to_room].number : -1,
    grn, nrm, cyn, room->dir_option[DOWN] ? 
          world[room->dir_option[DOWN]->to_room].number : -1,
    grn, nrm, cyn, room->dir_option[NORTHEAST] ? 
          world[room->dir_option[NORTHEAST]->to_room].number : -1,
    grn, nrm, cyn, room->dir_option[SOUTHEAST] ? 
          world[room->dir_option[SOUTHEAST]->to_room].number : -1,
    grn, nrm, cyn, room->dir_option[SOUTHWEST] ? 
          world[room->dir_option[SOUTHWEST]->to_room].number : -1,
    grn, nrm, cyn, room->dir_option[NORTHWEST] ? 
          world[room->dir_option[NORTHWEST]->to_room].number : -1,
    grn, nrm,                                         
      grn, nrm, cyn, (NULL == OLC_ROOM(d)->func ? "NONE" : (char *)getSpecialPointer(OLC_ROOM(d)->func)),
        grn, nrm, cyn, room->proto_script?"Set.":"Not Set.",
      grn, nrm
  );
  send_to_char(buf, d->character);

  OLC_MODE(d) = REDIT_MAIN_MENU;
}

/**************************************************************************
  The main loop
 **************************************************************************/

void redit_parse(struct descriptor_data * d, char *arg)
{ 
  int number;
  SPECIAL(*fn);

  switch (OLC_MODE(d)) {
  case REDIT_NAMED_SPEC:
    //    OLC_MODE(d) = REDIT_PICK_SPEC;
    //    return;
  case REDIT_PICK_SPEC:
    fn = getSpecialByNum(atoi(arg));
    OLC_ROOM(d)->func = fn;
    OLC_MODE(d) = REDIT_MAIN_MENU;
    OLC_VAL(d) = 42;
    redit_disp_menu(d);
    return;
  case REDIT_CONFIRM_SAVESTRING:
    switch (*arg) {
    case 'y':
    case 'Y':
      redit_save_internally(d);
      sprintf(buf, "OLC: %s edits room %ld",  GET_DISGUISED(d->character) ? GET_RNAME(d->character) : GET_NAME(d->character), OLC_NUM(d));
      mudlog(buf, CMP, LVL_BUILDER, TRUE);
      /*. Do NOT free strings! just the room structure .*/
      cleanup_olc(d, CLEANUP_STRUCTS);
      send_to_char("Room saved to memory.\r\n", d->character);
      break;
    case 'n':
    case 'N':
      /* free everything up, including strings etc */
      cleanup_olc(d, CLEANUP_ALL);
      break;
    default:
      send_to_char("Invalid choice!\r\n", d->character);
      send_to_char("Do you wish to save this room internally? : ", d->character);
      break;
    }
    return;

  case REDIT_MAIN_MENU:
    switch (*arg) {
    case 'q':
    case 'Q':
      if (OLC_VAL(d))
      { /*. Something has been modified .*/
        send_to_char("Do you wish to save this room internally? : ", d->character);
        OLC_MODE(d) = REDIT_CONFIRM_SAVESTRING;
      } else
        cleanup_olc(d, CLEANUP_ALL);
      return;
    case '1':
      send_to_char("Enter room name:-\r\n| ", d->character);
      OLC_MODE(d) = REDIT_NAME;
      break;
    case '2':
      send_to_char("Enter room description:-\r\n", d->character);
      OLC_MODE(d) = REDIT_DESC;

      SEND_TO_Q("\x1B[H\x1B[J", d);
      SEND_TO_Q("Enter room description: (/s saves /h for help)\r\n\r\n", d);
SEND_TO_Q("Please format your desc to less than 75 characters per line.\r\n", d);
SEND_TO_Q("This will make your desc easier to read for those with smaller screens\r\n",d);
SEND_TO_Q("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n",d);
      d->backstr = NULL;
      if (OLC_ROOM(d)->description) {
         SEND_TO_Q(OLC_ROOM(d)->description, d);
         d->backstr = str_dup(OLC_ROOM(d)->description);
      }
      d->str = &OLC_ROOM(d)->description;
      d->max_str = MAX_ROOM_DESC;
      d->mail_to[0] = 0;
      OLC_VAL(d) = 1;
      d->character->desc->current_text_line = count_newlines(d->str) + 1;
      break;
    case '3':
      redit_disp_flag_menu(d);
      break;
    case '4':
      redit_disp_sector_menu(d);
      break;
    case '5':
      OLC_VAL(d) = NORTH;
      redit_disp_exit_menu(d);
      break;
    case '6':
      OLC_VAL(d) = EAST;
      redit_disp_exit_menu(d);
      break;
    case '7':
      OLC_VAL(d) = SOUTH;
      redit_disp_exit_menu(d);
      break;
    case '8':
      OLC_VAL(d) = WEST;
      redit_disp_exit_menu(d);
      break;
    case '9':
      OLC_VAL(d) = UP;
      redit_disp_exit_menu(d);
      break;
    case 'a':
    case 'A':
      OLC_VAL(d) = DOWN;
      redit_disp_exit_menu(d);
      break;
    case 'b':
    case 'B':
      OLC_VAL(d) = NORTHEAST;
      redit_disp_exit_menu(d);
      break;
    case 'c':
    case 'C':
      OLC_VAL(d) = SOUTHEAST;
      redit_disp_exit_menu(d);
      break;
    case 'd':
    case 'D':
      OLC_VAL(d) = SOUTHWEST;
      redit_disp_exit_menu(d);
      break;
    case 'e':
    case 'E':
      OLC_VAL(d) = NORTHWEST;
      redit_disp_exit_menu(d);
      break;                        
    case 'f':
    case 'F':
      /* if extra desc doesn't exist . */
      if (!OLC_ROOM(d)->ex_description) {
    CREATE(OLC_ROOM(d)->ex_description, struct extra_descr_data, 1);
    OLC_ROOM(d)->ex_description->next = NULL;
      }
      OLC_DESC(d) = OLC_ROOM(d)->ex_description;
      redit_disp_extradesc_menu(d);
      break;
    case 'g' :
    case 'G' : 
      redit_disp_menu(d);
      break;  
    case 'H':
    case 'h':
      redit_display_specials(d);
      break;
    case 's':
    case 'S':
      OLC_SCRIPT_EDIT_MODE(d) = SCRIPT_MAIN_MENU;
      dg_script_menu(d);
      return;
    default:
      send_to_char("Invalid choice!", d->character);
      redit_disp_menu(d);
      break;
    }
    return;

  case OLC_SCRIPT_EDIT:
    if (dg_script_edit_parse(d, arg)) return;
    break;

  case REDIT_NAME:
    if (OLC_ROOM(d)->name)
      free(OLC_ROOM(d)->name);
    if (strlen(arg) > MAX_ROOM_NAME)
      arg[MAX_ROOM_NAME -1] = 0;
    OLC_ROOM(d)->name = str_dup(arg);
    break;
  case REDIT_DESC:
    /* we will NEVER get here */
    mudlog("SYSERR: Reached REDIT_DESC case in parse_redit",BRF,LVL_BUILDER,TRUE);
    break;

  case REDIT_FLAGS:
    number = atoi(arg);
    if ((number < 0) || (number > NUM_ROOM_FLAGS)) {
      send_to_char("That's not a valid choice!\r\n", d->character);
      redit_disp_flag_menu(d);
    } else {
      if (number == 0)
        break;
      else {
    /* toggle bits */
    if (IS_SET(OLC_ROOM(d)->room_flags, 1 << (number - 1)))
      REMOVE_BIT(OLC_ROOM(d)->room_flags, 1 << (number - 1));
    else
      SET_BIT(OLC_ROOM(d)->room_flags, 1 << (number - 1));
    redit_disp_flag_menu(d);
      }
    }
    return;

  case REDIT_SECTOR:
    number = atoi(arg);
    if (number < 0 || number >= NUM_ROOM_SECTORS) {
      send_to_char("Invalid choice!", d->character);
      redit_disp_sector_menu(d);
      return;
    } else 
      OLC_ROOM(d)->sector_type = number;
    break;

  case REDIT_EXIT_MENU:
    switch (*arg) {
    case '0':
      break;
    case '1':
      OLC_MODE(d) = REDIT_EXIT_NUMBER;
      send_to_char("Exit to room number : ", d->character);
      return;
    case '2':
      OLC_MODE(d) = REDIT_EXIT_DESCRIPTION;
      SEND_TO_Q("Enter exit description: (/s saves /h for help)\r\n\r\n", d);
SEND_TO_Q("Please format your desc to less than 75 characters per line.\r\n", d);
SEND_TO_Q("This will make your desc easier to read for those with smaller screens\r\n",d);
SEND_TO_Q("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n",d);
      d->backstr = NULL;
      if (OLC_EXIT(d)->general_description) {
       SEND_TO_Q(OLC_EXIT(d)->general_description, d);
       d->backstr = str_dup(OLC_EXIT(d)->general_description);
      }
      d->str = &OLC_EXIT(d)->general_description;
      d->max_str = MAX_EXIT_DESC;
      d->mail_to[0] = 0;
      send_to_char("Enter exit description:-\r\n", d->character);
      d->character->desc->current_text_line = count_newlines(d->str) + 1;
      return;
    case '3':
      OLC_MODE(d) = REDIT_EXIT_KEYWORD;
      send_to_char("Enter keywords : ", d->character);
      return;
    case '4':
      OLC_MODE(d) = REDIT_EXIT_KEY;
      send_to_char("Enter key number : ", d->character);
      return;
    case '5':
      redit_disp_exit_flag_menu(d);
      OLC_MODE(d) = REDIT_EXIT_DOORFLAGS;
      return;
    case '6':
      /* delete exit */
      if (OLC_EXIT(d)->keyword)
    free(OLC_EXIT(d)->keyword);
      if (OLC_EXIT(d)->general_description)
    free(OLC_EXIT(d)->general_description);
      free(OLC_EXIT(d));
      OLC_EXIT(d) = NULL;
      break;
    default:
      send_to_char("Try again : ", d->character);
      return;
    }
    break;

  case REDIT_EXIT_NUMBER:
    number = (atoi(arg));
    if (number != -1)
    { number = real_room(number);
      if (number < 0)
      { send_to_char("That room does not exist, try again : ", d->character);
        return;
      }
    }
    OLC_EXIT(d)->to_room = number;
    redit_disp_exit_menu(d);
    return;

  case REDIT_EXIT_DESCRIPTION:
    /* we should NEVER get here */
    mudlog("SYSERR: Reached REDIT_EXIT_DESC case in parse_redit",BRF,LVL_BUILDER,TRUE);
    break;

  case REDIT_EXIT_KEYWORD:
    if (OLC_EXIT(d)->keyword)
      free(OLC_EXIT(d)->keyword);
    OLC_EXIT(d)->keyword = str_dup(arg);
    redit_disp_exit_menu(d);
    return;

  case REDIT_EXIT_KEY:
    number = atoi(arg);
    OLC_EXIT(d)->key = number;
    redit_disp_exit_menu(d);
    return;

  case REDIT_EXIT_DOORFLAGS:
    number = atoi(arg);
    if ((number < 0) || (number > 4)) {
      send_to_char("That's not a valid choice!\r\n", d->character);
      redit_disp_exit_flag_menu(d);
    } else {
      /* doors are a bit idiotic, don't you think? :) */
      if (number == 0)
    OLC_EXIT(d)->exit_info = 0;
      else if (number == 1)
    OLC_EXIT(d)->exit_info = EX_ISDOOR;
      else if (number == 2)
    OLC_EXIT(d)->exit_info = EX_ISDOOR | EX_PICKPROOF;
      else if (number == 3)
        OLC_EXIT(d)->exit_info = EX_ISDOOR | EX_HIDDEN;
      else if (number == 4)
        OLC_EXIT(d)->exit_info = EX_ISDOOR | EX_PICKPROOF | EX_HIDDEN;
      /* jump out to menu */
      redit_disp_exit_menu(d);
    }
    return;

  case REDIT_EXTRADESC_KEY:
    OLC_DESC(d)->keyword = str_dup(arg);
    redit_disp_extradesc_menu(d);
    return;

  case REDIT_EXTRADESC_MENU:
    number = atoi(arg);
    switch (number) {
    case 0:
      {
    /* if something got left out, delete the extra desc
     when backing out to menu */
    if (!OLC_DESC(d)->keyword || !OLC_DESC(d)->description) 
        { struct extra_descr_data **tmp_desc;

      if (OLC_DESC(d)->keyword)
        free(OLC_DESC(d)->keyword);
      if (OLC_DESC(d)->description)
        free(OLC_DESC(d)->description);

      /*. Clean up pointers .*/
      for(tmp_desc = &(OLC_ROOM(d)->ex_description); *tmp_desc;
          tmp_desc = &((*tmp_desc)->next))
          { if(*tmp_desc == OLC_DESC(d))
        { *tmp_desc = NULL;
              break;
        }
      }
      free(OLC_DESC(d));
    }
      }
      break;
    case 1:
      OLC_MODE(d) = REDIT_EXTRADESC_KEY;
      send_to_char("Enter keywords, separated by spaces : ", d->character);
      return;
    case 2:
      OLC_MODE(d) = REDIT_EXTRADESC_DESCRIPTION;
      send_to_char("Enter extra description:-\r\n", d->character);
      /* send out to modify.c */
      SEND_TO_Q("Enter extra description: (/s saves /h for help)\r\n\r\n", d);
SEND_TO_Q("Please format your desc to less than 75 characters per line.\r\n", d);
SEND_TO_Q("This will make your desc easier to read for those with smaller screens\r\n",d);
SEND_TO_Q("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n",d);
      d->backstr = NULL;
      if (OLC_DESC(d)->description) {
         SEND_TO_Q(OLC_DESC(d)->description, d);
         d->backstr = str_dup(OLC_DESC(d)->description);
      }
      d->str = &OLC_DESC(d)->description;
      d->max_str = MAX_EXTRA_DESC;
      d->mail_to[0] = 0;
      d->character->desc->current_text_line = count_newlines(d->str) + 1;
      return;
    case 3:
      if (!OLC_DESC(d)->keyword || !OLC_DESC(d)->description) {
    send_to_char("You can't edit the next extra desc without completing this one.\r\n", d->character);
    redit_disp_extradesc_menu(d);
      } else {
    struct extra_descr_data *new_extra;

    if (OLC_DESC(d)->next)
      OLC_DESC(d) = OLC_DESC(d)->next;
    else {
      /* make new extra, attach at end */
      CREATE(new_extra, struct extra_descr_data, 1);
      OLC_DESC(d)->next = new_extra;
      OLC_DESC(d) = new_extra;
    }
    redit_disp_extradesc_menu(d);
      }
      return;
    }
    break;

  default:
    /* we should never get here */
    mudlog("SYSERR: Reached default case in parse_redit",BRF,LVL_BUILDER,TRUE);
    break;
  }
  /*. If we get this far, something has be changed .*/
  OLC_VAL(d) = 1;
  redit_disp_menu(d);
}

