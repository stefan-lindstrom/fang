/******************************************************************************
 * $Id: zedit.c,v 1.5 2003/06/17 10:30:05 fang Exp $
 *   
 * File: zedit.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: zedit.c,v 
 *   Revision 1.2  2002/07/28 13:56:05  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.3  2002/05/27 10:52:50  fang
 *   Dmallc enabled.
 *
 *   Revision 1.2  2002/02/22 15:04:29  fang
 *   *** empty log message ***
 *
 *   Revision 1.11  2001/05/22 09:07:09  mud
 *   DG-Scripts for Fang.
 *
 *   Revision 1.10  2000/06/10 16:40:04  mud
 *   Minor requests from purdy fixed...
 *
 *   Revision 1.9  2000/06/09 21:19:52  mud
 *   Level check for builder lists...
 *
 *   Revision 1.8  2000/06/09 21:15:49  mud
 *   Level check for builder lists...
 *
 *   Revision 1.7  2000/06/09 18:40:53  mud
 *   Purdans 'Editor Name List' for zoned added.
 *
 *   Revision 1.6  2000/03/02 13:55:19  mud
 *   New zcmds now supported by zedit.
 *
 *   Revision 1.5  2000/03/02 12:54:52  mud
 *   Zones are now saved/loaded in XML-format.
 *
 *   Revision 1.4  2000/03/01 19:18:11  mud
 *   Support for 'percentage loading' added to zedit.
 *
 *   Revision 1.3  2000/02/22 22:22:22  mud
 *   Changed the minimum level required to set top-of-zone to headbuilder.
 *   The current minimum level of implementor is just silly /Fredrik
 *
 *   Revision 1.2  2000/02/21 14:00:20  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*  _TwyliteMud_ by Rv.                          Based on CircleMud3.0bpl9 *
*    				                                          *
*  OasisOLC - zedit.c 		                                          *
*    				                                          *
*  Copyright 1996 Harvey Gilpin.                                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "constants.h"
#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "comm.h"
#include "utils.h"
#include "db.h"
#include "olc.h"
#include "modify.h"
#include "xmlZones.h"
#include "dg_scripts.h"
 

/*-------------------------------------------------------------------*/
/*. Nasty internal macros to clean up the code .*/

#define ZCMD zone_table[OLC_ZNUM(d)].cmd[subcmd]
#define MYCMD OLC_ZONE(d)->cmd[subcmd]
#define OLC_CMD(d)   (OLC_ZONE(d)->cmd[OLC_VAL(d)])

/*-------------------------------------------------------------------*\
  utility functions 
\*-------------------------------------------------------------------*/

void zedit_setup(struct  descriptor_data *d, int room_num)
{ struct zone_data *zone;
  int subcmd = 0, count = 0, cmd_room = -1;

  /*. Alloc some zone shaped space .*/
  CREATE(zone, struct zone_data, 1);

  /*. Copy in zone header info .*/
  zone->name = str_dup(zone_table[OLC_ZNUM(d)].name);
  zone->lifespan = zone_table[OLC_ZNUM(d)].lifespan;
  zone->top = zone_table[OLC_ZNUM(d)].top;
  zone->reset_mode = zone_table[OLC_ZNUM(d)].reset_mode;
  zone->all = zone_table[OLC_ZNUM(d)].all;

  if (NULL != zone_table[OLC_ZNUM(d)].hfl_Editors && 0 != getNumber(zone_table[OLC_ZNUM(d)].hfl_Editors)) {
    zone->hfl_Editors = createDll();
    first(zone_table[OLC_ZNUM(d)].hfl_Editors);

    do { 
      char *dt = this(zone_table[OLC_ZNUM(d)].hfl_Editors);
      asend_to_char(d->character,"Name: %s\r\n",dt);
      alog("Name: %s",dt);  
      append(zone->hfl_Editors,strdup(dt));
    } while(next(zone_table[OLC_ZNUM(d)].hfl_Editors));
  } 

  /*. The remaining fields are used as a 'has been modified' flag .*/
  zone->number = 0;   	/*. Header info has changed .*/
  zone->age = 0;	/*. Commands have changed   .*/
  
  /*. Start the reset command list with a terminator .*/
  CREATE(zone->cmd, struct reset_com, 1);
  zone->cmd[0].command = 'S';

  /*. Add all entried in zone_table that relate to this room .*/
  while(ZCMD.command != 'S')
  { switch(ZCMD.command)
    { case 'M':
      case 'O':
        cmd_room = ZCMD.arg3;
        break;
      case 'D':
      case 'R':
        cmd_room = ZCMD.arg1;
        break;
      case 'V':
        if (ZCMD.arg1 == WLD_TRIGGER)
         cmd_room = real_room(ZCMD.arg3);
        break;
      default:
        break;
    }
    if(cmd_room == room_num)
    { add_cmd_to_list(&(zone->cmd), &ZCMD, count);
      count++;
    }
    subcmd++;
  }

  OLC_ZONE(d) = zone;
  /*. Display main menu .*/
  zedit_disp_menu(d);
}


/*-------------------------------------------------------------------*/
/*. Create a new zone .*/

void zedit_new_zone(struct char_data *ch, int vzone_num)
{ FILE *fp;
  struct zone_data *new_table;
  int i, room, found = 0;

  if(vzone_num > 326)
  { send_to_char("326 is the highest zone allowed.\r\n", ch);
    return;
  }
  sprintf(buf, "%s/%i.zon", ZON_PREFIX, vzone_num);

  /*. Check zone does not exist .*/
  room = vzone_num * 100;
  for (i = 0; i <= top_of_zone_table; i++)
    if ((zone_table[i].number * 100 <= room) &&
        (zone_table[i].top >= room))
    { send_to_char("A zone already covers that area.\r\n", ch);
      return;
    }

  /*. Create Zone file .*/
  if(!(fp = fopen(buf, "w"))) {
    mudlog("SYSERR: OLC: Can't write new zone file", BRF, LVL_IMPL, TRUE);
    return;
  }
  fprintf(fp, 
    "#%d\n"
    "New Zone~\n"
    "%d 30 2 1\n" /* 1 -> Initially locked zone */
    "S\n"
    "$\n", 
    vzone_num,
    (vzone_num * 100) + 99
  );
  fclose(fp);

  /*. Create Rooms file .*/
  sprintf(buf, "%s/%d.wld", WLD_PREFIX, vzone_num);
  if(!(fp = fopen(buf, "w"))) 
  { mudlog("SYSERR: OLC: Can't write new world file", BRF, LVL_IMPL, TRUE);
    return;
  }
  fprintf(fp, 
    "#%d\n"
        "The Beginning~\n"
    "Not much here.\n"
    "~\n"
    "%d 0 0\n"
    "S\n"
    "$\n",
    vzone_num * 100,
    vzone_num
  );
  fclose(fp);

  /*. Create Mobiles file .*/
  sprintf(buf, "%s/%i.mob", MOB_PREFIX, vzone_num);
  if(!(fp = fopen(buf, "w")))
  { mudlog("SYSERR: OLC: Can't write new mob file", BRF, LVL_IMPL, TRUE);
    return;
  }
  fprintf(fp, "$\n");
  fclose(fp);

  /*. Create Objects file .*/
  sprintf(buf, "%s/%i.obj", OBJ_PREFIX, vzone_num);
  if(!(fp = fopen(buf, "w"))) 
  { mudlog("SYSERR: OLC: Can't write new obj file", BRF, LVL_IMPL, TRUE);
    return;
  }
  fprintf(fp, "$\n");
  fclose(fp);

  /*. Create Shops file .*/
  sprintf(buf, "%s/%i.shp", SHP_PREFIX, vzone_num);
  if(!(fp = fopen(buf, "w")))
  { mudlog("SYSERR: OLC: Can't write new shop file", BRF, LVL_IMPL, TRUE);
    return;
  }
  fprintf(fp, "$~\n");
  fclose(fp);

 
  /*
   * Create the quest file.
   */
  sprintf(buf, "%s/%d.qst", QST_PREFIX, vzone_num);
  if (!(fp = fopen(buf, "w"))) {
    mudlog("SYSERR: OLC: Can't write new trigger file", BRF, LVL_IMPL, TRUE);
    asend_to_char(ch,"Could not write quest file (reason: %s, file: %s).\r\n", strerror(errno),buf);
    return;
  }
  fprintf(fp, "$~\n");
  fclose(fp);

  /*  Create the trigger file.   */  
  sprintf(buf, "%s/%d.trg", TRG_PREFIX, vzone_num);   
  if (!(fp = fopen(buf, "w"))) {     
   mudlog("SYSERR: OLC: Can't write new trigger file", BRF, LVL_IMPL, TRUE); 
   send_to_char("Could not write trigger file.\r\n", ch);     
   return;   
  }   
  fprintf(fp, "$~\n");   
  fclose(fp);

  /*. Update index files .*/
  zedit_create_index(vzone_num, "zon");
  zedit_create_index(vzone_num, "wld");
  zedit_create_index(vzone_num, "mob");
  zedit_create_index(vzone_num, "obj");
  zedit_create_index(vzone_num, "shp");
  zedit_create_index(vzone_num, "qst");
  zedit_create_index(vzone_num, "trg");

  /*. Make a new zone in memory.*/
  CREATE(new_table, struct zone_data, top_of_zone_table);
  new_table[top_of_zone_table + 1].number = 32000;
  for(i = 0; i <= top_of_zone_table + 1; i++)  
    if(!found)
      if (i > top_of_zone_table || zone_table[i].number > vzone_num)
      { found = 1;
        new_table[i].name = str_dup("New Zone");
        new_table[i].number = vzone_num;
        new_table[i].top = (vzone_num * 100) + 99;
        new_table[i].lifespan = 30;
        new_table[i].age = 0;
        new_table[i].reset_mode = 2;
        new_table[i].lock = 1; /* Zone is always starting out as locked */
    new_table[i].all = 0;
    new_table[i].hfl_Editors = NULL;
        CREATE(new_table[i].cmd, struct reset_com, 1);
        new_table[i].cmd[0].command = 'S';
        if (i <= top_of_zone_table)
          new_table[i+1] = zone_table[i];
      } else 
        new_table[i] = zone_table[i];
    else
      new_table[i+1] = zone_table[i];
  free(zone_table);
  zone_table = new_table;
  top_of_zone_table++;

  sprintf(buf, "OLC: %s creates new zone #%d", GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch), vzone_num);
  mudlog(buf, BRF, LVL_BUILDER, TRUE);
  send_to_char("Zone created.\r\n", ch);
  return;
}

/*-------------------------------------------------------------------*/

void zedit_create_index(int znum, char *type)
{ FILE *newfile, *oldfile;
  char new_name[32], old_name[32], *prefix;
  int num, found = FALSE;

  switch(*type)
  { case 'z':
      prefix = ZON_PREFIX;
      break;
    case 'w':
      prefix = WLD_PREFIX;
      break;
    case 'o':
      prefix = OBJ_PREFIX;
      break;
    case 'm':
      prefix = MOB_PREFIX;
      break;
    case 's':
      prefix = SHP_PREFIX;
      break;
    case 'q':
      prefix = QST_PREFIX;
      break;
    case 't': 
      prefix = TRG_PREFIX;  
      break;
    default:
      /*. Caller messed up .*/
      return;
  }

  sprintf(old_name, "%s/index", prefix);
  sprintf(new_name, "%s/newindex", prefix);

  if(!(oldfile = fopen(old_name, "r")))
  { sprintf(buf, "SYSERR: OLC: Failed to open %s", buf);
    mudlog(buf, BRF, LVL_IMPL, TRUE);
    return;
  }
  if(!(newfile = fopen(new_name, "w")))
  { sprintf(buf, "SYSERR: OLC: Failed to open %s", buf);
    mudlog(buf, BRF, LVL_IMPL, TRUE);
    return;
  }

  /*. Index contents must be in order: search through the old file for
      the right place, insert the new file, then copy the rest over.
      .*/

  sprintf(buf1, "%d.%s", znum, type);
  while(get_line(oldfile, buf))
  { if (*buf == '$')
    { if (!found)
        fprintf(newfile, "%s\n", buf1);
      fprintf(newfile, "$\n");
      break;
    }
    if (!found)
    { sscanf(buf, "%d", &num);
      if (num > znum)
      { found = TRUE;
        fprintf(newfile, "%s\n", buf1);
      }
    }
    fprintf(newfile, "%s\n", buf);
  }
    
  fclose(newfile);
  fclose(oldfile);
  /*. Out with the old, in with the new .*/
  remove(old_name);
  rename(new_name, old_name);
}

/*-------------------------------------------------------------------*/
/*. Save all the information on the players descriptor back into
    the zone table .*/

void zedit_save_internally(struct descriptor_data *d)
{ int subcmd = 0, cmd_room = -2, room_num;

  room_num = OLC_NUM(d);

  /*. Zap all entried in zone_table that relate to this room .*/
  while(ZCMD.command != 'S')
  { switch(ZCMD.command)
    { case 'M':
      case 'O':
        cmd_room = ZCMD.arg3;
        break;
      case 'D':
      case 'R':
        cmd_room = ZCMD.arg1;
        break;
      default:
        break;
    }
    if(cmd_room == room_num)
      remove_cmd_from_list(&(zone_table[OLC_ZNUM(d)].cmd), subcmd);
    else
      subcmd++;
  }

  /*. Now add all the entries in the players descriptor list .*/
  subcmd = 0;
  while(MYCMD.command != 'S')
  { add_cmd_to_list(&(zone_table[OLC_ZNUM(d)].cmd), &MYCMD, subcmd);
    subcmd++;
  }

  /*. Finally, if zone headers have been changed, copy over .*/
  if (OLC_ZONE(d)->number)  { 
    free(zone_table[OLC_ZNUM(d)].name);
    zone_table[OLC_ZNUM(d)].name 	= str_dup(OLC_ZONE(d)->name);
    zone_table[OLC_ZNUM(d)].top 	= OLC_ZONE(d)->top;
    zone_table[OLC_ZNUM(d)].reset_mode 	= OLC_ZONE(d)->reset_mode;
    zone_table[OLC_ZNUM(d)].lifespan 	= OLC_ZONE(d)->lifespan;
    zone_table[OLC_ZNUM(d)].lock        = OLC_ZONE(d)->lock;
    zone_table[OLC_ZNUM(d)].all         = OLC_ZONE(d)->all;

    if (NULL != zone_table[OLC_ZNUM(d)].hfl_Editors || 0 != getNumber( zone_table[OLC_ZNUM(d)].hfl_Editors))
      deleteAll(zone_table[OLC_ZNUM(d)].hfl_Editors,free); // Free original. 

    zone_table[OLC_ZNUM(d)].hfl_Editors = OLC_ZONE(d)->hfl_Editors; // Copy..
  }
  olc_add_to_save_list(zone_table[OLC_ZNUM(d)].number, OLC_SAVE_ZONE);
}


/*-------------------------------------------------------------------*/
/*. Save all the zone_table for this zone to disk.  Yes, this could
    automatically comment what it saves out, but why bother when you
    have an OLC as cool as this ? :> .*/

void zedit_save_to_disk(struct descriptor_data *d)
{
  char fname[64],tname[64];
  FILE *zfile;

  sprintf(fname, "%s/%i.zon", ZON_PREFIX, zone_table[OLC_ZNUM(d)].number);
  sprintf(tname, "%s/%i.tmp", ZON_PREFIX,zone_table[OLC_ZNUM(d)].number);

  if(!(zfile = fopen(tname, "w"))) {
    sprintf(buf, "SYSERR: OLC: zedit_save_to_disk:  Can't write zone %d.", 
            zone_table[OLC_ZNUM(d)].number);
    mudlog(buf, BRF, LVL_BUILDER, TRUE);
    return;
  }
  save_xml_zone(zfile,OLC_ZNUM(d));
  fclose(zfile);

  remove(fname);
  rename(tname,fname);

  olc_remove_from_save_list(zone_table[OLC_ZNUM(d)].number, OLC_SAVE_ZONE);
}

/*-------------------------------------------------------------------*/
/*. Adds a new reset command into a list.  Takes a pointer to the list
    so that it may play with the memory locations.*/

void add_cmd_to_list(struct reset_com **list, struct reset_com *newcmd, int pos)
{  int count = 0, i, l;
   struct reset_com *newlist;

   /*. Count number of commands (not including terminator) .*/
   while((*list)[count].command != 'S')
     count++;
   
   CREATE(newlist, struct reset_com, count + 2);

   /*. Tight loop to copy old list and insert new command .*/
   l = 0;
   for(i=0;i<=count;i++)
     if(i==pos)
       newlist[i] = *newcmd; 
     else
       newlist[i] = (*list)[l++];
   
   /*. Add terminator then insert new list .*/
   newlist[count+1].command = 'S';
   free(*list);
   *list = newlist;
}

/*-------------------------------------------------------------------*/
/*. Remove a reset command from a list.  Takes a pointer to the list
    so that it may play with the memory locations.*/

void remove_cmd_from_list(struct reset_com **list, int pos)
{  int count = 0, i, l;
   struct reset_com *newlist;

   /*. Count number of commands (not including terminator) .*/
   while((*list)[count].command != 'S')
     count++;
    
   CREATE(newlist, struct reset_com, count);

   /*. Tight loop to copy old list and skip unwanted command .*/
   l = 0;
   for(i=0;i<count;i++)
     if(i==pos)
       continue;
     else
       newlist[l++] = (*list)[i];
   
   /*. Add terminator then insert new list .*/
   newlist[count-1].command = 'S';
   free(*list);
   *list = newlist;
}

/*-------------------------------------------------------------------*/
/*. Error check user input and then add new (blank) command .*/

int new_command(struct descriptor_data *d, int pos)
{  int subcmd = 0;
   struct reset_com *new_com;

   /*. Error check to ensure users hasn't given too large an index .*/
   while(MYCMD.command != 'S')
     subcmd++;
  
   if ((pos > subcmd) || (pos < 0))
     return 0;
    
   /*. Ok, let's add a new (blank) command.*/
   CREATE(new_com, struct reset_com, 1);
   new_com->command = 'N'; 
   add_cmd_to_list(&OLC_ZONE(d)->cmd, new_com, pos);
   return 1;
}


/*-------------------------------------------------------------------*/
/*. Error check user input and then remove command .*/

void delete_command(struct descriptor_data *d, int pos)
{  int subcmd = 0;

   /*. Error check to ensure users hasn't given too large an index .*/
   while(MYCMD.command != 'S')
     subcmd++;
  
   if ((pos >= subcmd) || (pos < 0))
     return;
    
   /*. Ok, let's zap it .*/
   remove_cmd_from_list(&OLC_ZONE(d)->cmd, pos);
}

/*-------------------------------------------------------------------*/
/*. Error check user input and then setup change .*/

int start_change_command(struct descriptor_data *d, int pos)
{  int subcmd = 0;

   /*. Error check to ensure users hasn't given too large an index .*/
   while(MYCMD.command != 'S')
     subcmd++;
  
   if ((pos >= subcmd) || (pos < 0))
     return 0;
    
   /*. Ok, let's get editing .*/
   OLC_VAL(d) = pos;
   return 1;
}

/**************************************************************************
 Menu functions 
 **************************************************************************/

static char* print_builder_list(struct zone_data *z, char *bufp)
{ 
  if (z->all) { 
    strcpy(bufp,"Any builder.");
    return bufp;
  } else if (NULL != z->hfl_Editors && 0 != getNumber(z->hfl_Editors)) {   
    first(z->hfl_Editors);
    *bufp = '\0';
    do {  
      char *t = this(z->hfl_Editors);
      sprintf(bufp,"%s%s ",bufp,t);
    } while(next(z->hfl_Editors));
    return bufp;
  } 
  strcpy(bufp,"None set.");
  return bufp;
} 

/* the main menu */
void zedit_disp_menu(struct descriptor_data * d)
{ 
  int subcmd = 0, room, counter = 0;
  char tbuf[5*1024];
  get_char_cols(d->character);
  room = real_room(OLC_NUM(d));

  /*. Menu header .*/
  sprintf(buf,
    "[H[J"
    "Room number: %s%ld%s		Room zone: %s%d\r\n"
        "%sZ%s) Zone name    : %s%s\r\n"
        "%sL%s) Lifespan     : %s%d minutes\r\n"
        "%sT%s) Top of zone  : %s%d\r\n"
        "%sR%s) Reset Mode   : %s%s%s\r\n"
        "%sB%s) Builder list : %s%s%s\r\n"
        "[Command list]\r\n",

    cyn, OLC_NUM(d), nrm,
    cyn, zone_table[OLC_ZNUM(d)].number,
        grn, nrm, yel, OLC_ZONE(d)->name ? OLC_ZONE(d)->name : "<NONE!>",
        grn, nrm, yel, OLC_ZONE(d)->lifespan,
        grn, nrm, yel, OLC_ZONE(d)->top,
        grn, nrm, yel, OLC_ZONE(d)->reset_mode ? 
          ((OLC_ZONE(d)->reset_mode == 1) ?
          "Reset when no players are in zone." : 
          "Normal reset.") :
          "Never reset", nrm, 
    grn, nrm, yel, print_builder_list(OLC_ZONE(d),tbuf),nrm
  );

  /*. Print the commands for this room into display buffer .*/
  while(MYCMD.command != 'S')
  { /*. Translate what the command means .*/
    switch(MYCMD.command)
    { 
      case 'L':
    sprintf(buf2,"%sLoad %s [%s%ld%s] and put as sleeping on obj %s [%s%ld%s], Max : %ld, Chance: %d%%",
        MYCMD.if_flag ? " then " : "", 
        mob_proto[real_mobile(MYCMD.arg1)].player.short_descr,
        cyn , MYCMD.arg1, yel,
                obj_proto[real_object(MYCMD.arg3)].short_description,
                cyn, MYCMD.arg3, yel,
        MYCMD.arg2,
        MYCMD.percentage
        );
    break;
      case 'T':
        sprintf(buf2,"%sLoad %s [%s%ld%s] and put as sitting on obj %s [%s%ld%s], Max : %ld, Chance: %d%%",
                MYCMD.if_flag ? " then " : "",
                mob_proto[real_mobile(MYCMD.arg1)].player.short_descr,
                cyn , MYCMD.arg1, yel,
                obj_proto[real_object(MYCMD.arg3)].short_description,
                cyn, MYCMD.arg3, yel,
                MYCMD.arg2,
                MYCMD.percentage
                );
        break;
      case 'U':
        sprintf(buf2,"%sLoad %s [%s%ld%s] and put on obj %s [%s%ld%s], Max : %ld, Chance: %d%%",
                MYCMD.if_flag ? " then " : "",
        obj_proto[real_object(MYCMD.arg1)].short_description,
                cyn , MYCMD.arg1, yel,
        obj_proto[real_object(MYCMD.arg3)].short_description,
        cyn, MYCMD.arg3, yel,
                MYCMD.arg2,
                MYCMD.percentage
                );
        break;
      case'M':
        sprintf(buf2, "%sLoad %s [%s%ld%s], Max : %ld, Chance: %d%%",
                MYCMD.if_flag ? " then " : "",
        mob_proto[real_mobile(MYCMD.arg1)].player.short_descr,
                cyn, MYCMD.arg1, yel,
        MYCMD.arg2,
        MYCMD.percentage
        );
        break;
      case'G':
        sprintf(buf2, "%sGive it %s [%s%ld%s], Max : %ld, Chance: %d%%",
        MYCMD.if_flag ? " then " : "",
        obj_proto[real_object(MYCMD.arg1)].short_description,
                cyn,MYCMD.arg1, yel,
        MYCMD.arg2,
        MYCMD.percentage
        );
        break;
      case'O':
        sprintf(buf2, "%sLoad %s [%s%ld%s], Max : %ld, Chance: %d%%",
        MYCMD.if_flag ? " then " : "",
        obj_proto[real_object(MYCMD.arg1)].short_description,
                cyn, obj_index[real_object(MYCMD.arg1)].virtual, yel,
        MYCMD.arg2,
        MYCMD.percentage
        );
        break;
      case'E':
        sprintf(buf2, "%sEquip with %s [%s%ld%s], %s, Max : %ld, Chance: %d%%",
        MYCMD.if_flag ? " then " : "",
        obj_proto[real_object(MYCMD.arg1)].short_description,
                cyn, MYCMD.arg1, yel,
        equipment_types[MYCMD.arg3],
        MYCMD.arg2,
        MYCMD.percentage
        );
        break;
      case'P':
        sprintf(buf2, "%sPut %s [%s%ld%s] in %s [%s%ld%s], Max : %ld, Chance: %d%%",
        MYCMD.if_flag ? " then " : "",
        obj_proto[real_object(MYCMD.arg1)].short_description,
                cyn, MYCMD.arg1, yel,
        obj_proto[real_object(MYCMD.arg3)].short_description,
                cyn, MYCMD.arg3, yel,
        MYCMD.arg2,
        MYCMD.percentage
        );
        break;
      case'R':
        sprintf(buf2, "%sRemove %s [%s%ld%s] from room.(Chance: %d%%)",
        MYCMD.if_flag ? " then " : "",
        obj_proto[real_object(MYCMD.arg2)].short_description,
                cyn, MYCMD.arg2, yel, MYCMD.percentage
        );
        break;
      case'D':
        sprintf(buf2, "%sSet door %s as %s(Chance: %d%%).",
        MYCMD.if_flag ? " then " : "",
        dirs[MYCMD.arg2],
        MYCMD.arg3 ? ((MYCMD.arg3 == 1) ? 
                  "closed" : "locked") : "open",
        MYCMD.percentage
        );
        break;
      case 'A':
        sprintf(buf2, "%sAttach trigger %s%s%s [%s%ld%s] to %s",
                MYCMD.if_flag ? " then " : "",
                cyn, trig_index[real_trigger(MYCMD.arg2)]->proto->name, yel,
                cyn, MYCMD.arg2, yel,
                ((MYCMD.arg1 == MOB_TRIGGER) ? "mobile" :
                ((MYCMD.arg1 == OBJ_TRIGGER) ? "object" :
                ((MYCMD.arg1 == WLD_TRIGGER)? "room" : "????"))));
        break;
      case 'V':
        sprintf(buf2, "%sAssign global %s:%s to %s with context %ld",
                MYCMD.if_flag ? " then " : "",
                MYCMD.sarg1, MYCMD.sarg2,
                ((MYCMD.arg1 == MOB_TRIGGER) ? "mobile" :
                ((MYCMD.arg1 == OBJ_TRIGGER) ? "object" :
                ((MYCMD.arg1 == WLD_TRIGGER)? "room" : "????"))), MYCMD.arg2);
        break;
      default:
        strcpy(buf2, "<Unknown Command>");
        break;
    }
    /*. Build the display buffer for this command .*/
    sprintf(buf1, "%s%d - %s%s\r\n",
        nrm, counter++, yel,
        buf2
    );
    strcat(buf, buf1);
    subcmd++;
  }
  /*. Finish off menu .*/
  sprintf(buf1, 
        "%s%d - <END OF LIST>\r\n"
        "%sN%s) New command.\r\n"
        "%sE%s) Edit a command.\r\n"
        "%sD%s) Delete a command.\r\n"
        "%sQ%s) Quit\r\nEnter your choice : ",
        nrm, counter,
                grn, nrm, grn, nrm, grn, nrm, grn, nrm
  );

  strcat(buf, buf1);
  send_to_char(buf, d->character);

  OLC_MODE(d) = ZEDIT_MAIN_MENU;
}


/*-------------------------------------------------------------------*/
/*. Print the command type menu and setup response catch. */

void zedit_disp_comtype(struct descriptor_data *d)
{ get_char_cols(d->character);
  sprintf(buf, "[H[J"
    "%sM%s) Load Mobile to room             %sO%s) Load Object to room\r\n"
    "%sE%s) Equip mobile with object        %sG%s) Give an object to a mobile\r\n"
    "%sP%s) Put object in another object    %sD%s) Open/Close/Lock a Door\r\n"
    "%sR%s) Remove an object from the room  %sL%s) Load mob on obj (sleeping)\r\n"
    "%sT%s) Load mob on obj (sitting)       %sU%s) Load obj ON obj\r\n"
    "%sA%s) Assign a Trigger                %sV%s) Set a global variable\r\n"
    "What sort of command will this be? : ",
    grn, nrm, grn, nrm, grn, nrm, grn, nrm, grn, nrm,
    grn, nrm, grn, nrm ,grn, nrm, grn, nrm, grn, nrm, grn, nrm, grn, nrm
  );
  send_to_char(buf, d->character);
  OLC_MODE(d)=ZEDIT_COMMAND_TYPE;
}


/*-------------------------------------------------------------------*/
/*. Print the appropriate message for the command type for arg1 and set
    up the input catch clause .*/

void zedit_disp_arg1(struct descriptor_data *d)
{
  switch(OLC_CMD(d).command){ 
    case 'M':
    case 'L': // NEW: Put mobile (sleeping) on obj
    case 'T': // NEW: Put mobile (sitting) on obj
      send_to_char("Input mob's vnum : ", d->character);
      OLC_MODE(d) = ZEDIT_ARG1;
      break;
    case 'O':
    case 'U': // NEW: PutObj-ON-Obj
    case 'E':
    case 'P':
    case 'G':
      send_to_char("Input object vnum : ", d->character);
      OLC_MODE(d) = ZEDIT_ARG1;
      break;
    case 'D':
    case 'R':
      /*. Arg1 for these is the room number, skip to arg2 .*/
      OLC_CMD(d).arg1 = OLC_NUM(d);
      zedit_disp_arg2(d);
      break;
    case 'A':
    case 'V':
      if (!IS_SET((d->character)->player_specials->saved.command_groups, CG_DGS))
      {
       send_to_char("You don't have permission to do that. Talk to the HB.\r\n", d->character);
       cleanup_olc(d, CLEANUP_ALL);
       return;
      }
      send_to_char("Input trigger type (0:mob, 1:obj, 2:room) :", d->character);
      OLC_MODE(d) = ZEDIT_ARG1;
      break;
    default:
      /*. We should never get here .*/
      cleanup_olc(d, CLEANUP_ALL);
      mudlog("SYSERR: OLC: zedit_disp_arg1(): Help!", BRF, LVL_BUILDER, TRUE);
      return;
  }
}

    

/*-------------------------------------------------------------------*/
/*. Print the appropriate message for the command type for arg2 and set
    up the input catch clause .*/

void zedit_disp_arg2(struct descriptor_data *d)
{ int i = 0;
  switch(OLC_CMD(d).command)
  { case 'M':
    case 'O':
    case 'E':
    case 'P':
    case 'G':
    case 'L':
    case 'T':
    case 'U':
      send_to_char("Input the maximum number that can exist on the mud : ", d->character);
      break;
    case 'D':
      while(*dirs[i] != '\n')
      { sprintf(buf, "%d) Exit %s.\r\n", i, dirs[i]);
        send_to_char(buf, d->character);
        i++;
      }
      send_to_char("Enter exit number for door : ", d->character);
      break;
    case 'R':
      send_to_char("Input object's vnum : ", d->character);
      break;
    case 'A':
      send_to_char("Enter the trigger VNum : ", d->character);
      break;
    case 'V':
      send_to_char("Global's context (0 for none) : ", d->character);
      break;
    default:
      /*. We should never get here .*/
      cleanup_olc(d, CLEANUP_ALL);
      mudlog("SYSERR: OLC: zedit_disp_arg2(): Help!", BRF, LVL_BUILDER, TRUE);
      return;
  }
  OLC_MODE(d) = ZEDIT_ARG2;
}


/*-------------------------------------------------------------------*/
/*. Print the appropriate message for the command type for arg3 and set
    up the input catch clause .*/

void zedit_disp_arg3(struct descriptor_data *d)
{ int i = 0;
  switch(OLC_CMD(d).command)
  { 
    case 'E':
      while(*equipment_types[i] !=  '\n')
      { sprintf(buf, "%2d) %26.26s %2d) %26.26s\r\n", i, 
         equipment_types[i], i+1, (*equipment_types[i+1] != '\n') ?
         equipment_types[i+1] : "");
        send_to_char(buf, d->character);
        if(*equipment_types[i+1] != '\n')
          i+=2;
        else
          break;
      }
      send_to_char("Input location to equip : ", d->character);
      break;
    case 'L':
    case 'T':
    case 'U':
      send_to_char("Input the vnum of the furniture; ",d->character);
      break;
    case 'P':
      send_to_char("Input the vnum of the container : ", d->character);
      break;
    case 'D':
      send_to_char("0)  Door open\r\n"
                   "1)  Door closed\r\n"
                   "2)  Door locked\r\n" 
                   "Enter state of the door : ", d->character);
      break;
    case 'V':
      send_to_char("Enter the global name: ", d->character);
      OLC_MODE(d) = ZEDIT_SARG1;
      return;
    case 'M':
    case 'O':
    case 'R':
    case 'G':
    default:
      /*. We should never get here .*/
      cleanup_olc(d, CLEANUP_ALL);
      mudlog("SYSERR: OLC: zedit_disp_arg3(): Help!", BRF, LVL_BUILDER, TRUE);
      return;
  }
  OLC_MODE(d) = ZEDIT_ARG3;
}

void zedit_disp_percentage(struct descriptor_data *d)
{
  asend_to_char(d->character,"Enter chance (in percent) of this command to be "
        "executed (current: %d): ",OLC_CMD(d).percentage);
  OLC_MODE(d) = ZEDIT_PERC;
} 

static void display_blist(struct descriptor_data *d)
{ 
  int i;
  asend_to_char(d->character,"Edit builder permissions of zone:\r\n\r\n");
  asend_to_char(d->character,"Currently assigned builders: \r\n");
  if (OLC_ZONE(d)->all)
    asend_to_char(d->character,"All builders.\r\n\r\n");
  else if (NULL == OLC_ZONE(d)->hfl_Editors || 0 == getNumber(OLC_ZONE(d)->hfl_Editors))
    asend_to_char(d->character,"No specific permissions given.\r\n");
  else {
    i = 1;
    first(OLC_ZONE(d)->hfl_Editors);
    do {  
      asend_to_char(d->character,"&g%d&n) &c%s&n      ",i,this(OLC_ZONE(d)->hfl_Editors));
      if (!(i % 2))
    asend_to_char(d->character,"\r\n");
      ++i;
    } while(next(OLC_ZONE(d)->hfl_Editors));
     asend_to_char(d->character,"\r\n");
  } 
  asend_to_char(d->character,"&gA&n) Add a new builder to list\r\n");
  asend_to_char(d->character,"&gD&n) Delete one builder from list\r\n");
  asend_to_char(d->character,"&gP&n) Purge all builders from list\r\n");
  asend_to_char(d->character,"&gF&n) Allow all builders to access zone\r\n");
  asend_to_char(d->character,"Any other key to exit to main menu.\r\n\r\n");
  asend_to_char(d->character,"Your command: ");
} 

static int name_in_list(struct descriptor_data *d, char *name) 
{
  struct zone_data *z = OLC_ZONE(d);
  if (NULL == d || NULL == name || !*name || NULL == z->hfl_Editors || 0 == getNumber(z->hfl_Editors)) 
    return 0;

  first(z->hfl_Editors);
  do { 
    char *tn = this(z->hfl_Editors);
    if (!strcasecmp(name,tn))
      return 1;
  } while(next(z->hfl_Editors));

  return 0;
} 

static int delete_name_nr(struct descriptor_data *d,int nr)
{ 
  int i = 1;
  struct zone_data *z = OLC_ZONE(d);

  if (NULL == z->hfl_Editors || 0 == getNumber(z->hfl_Editors) || getNumber(z->hfl_Editors) < nr)
    return 0;

  first(z->hfl_Editors);
  do { 
    if (i == nr) {     
      deleteCurrentElement(z->hfl_Editors,free);
      return 1;
    } 
    ++i;
  } while(next(z->hfl_Editors));
  return 0;
} 

static int delete_name_ch(struct descriptor_data *d,char *name)
{
  struct zone_data *z = OLC_ZONE(d);

  if (NULL == z->hfl_Editors || 0 == getNumber(z->hfl_Editors))
    return 0;   

  first(z->hfl_Editors);
  do { 
    if (!strcasecmp(name,this(z->hfl_Editors))) {  
      deleteCurrentElement(z->hfl_Editors,free);
      return 1;
    } 
  } while(next(z->hfl_Editors)); 

  return 0;
}  

static void purge_all_builders(struct descriptor_data *d)
{ 
  struct zone_data *z = OLC_ZONE(d);

  if (z->all)
    z->all = 0;

  if (NULL != z->hfl_Editors)
    deleteAll(z->hfl_Editors,free);

  z->hfl_Editors = NULL;
} 
    
/**************************************************************************
  The GARGANTUAN event handler
 **************************************************************************/

void zedit_parse(struct descriptor_data * d, char *arg)
{ int pos, i = 0;

  switch (OLC_MODE(d)) 
  {
/*-------------------------------------------------------------------*/
  case ZEDIT_CONFIRM_SAVESTRING:
    switch (*arg) {
    case 'y':
    case 'Y':
      /*. Save the zone in memory .*/
      send_to_char("Saving zone info in memory.\r\n", d->character);
      zedit_save_internally(d);
      sprintf(buf, "OLC: %s edits zone info for room %ld", GET_DISGUISED(d->character) ? GET_RNAME(d->character) : GET_NAME(d->character),
              OLC_NUM(d));	      
      mudlog(buf, CMP, LVL_BUILDER, TRUE);
      cleanup_olc(d, CLEANUP_ALL);
      break;
    case 'n':
    case 'N':
      cleanup_olc(d, CLEANUP_ALL);
      break;
    default:
      send_to_char("Invalid choice!\r\n", d->character);
      send_to_char("Do you wish to save the zone info? : ", d->character);
      break;
    }
    break; /* end of ZEDIT_CONFIRM_SAVESTRING */

/*-------------------------------------------------------------------*/
  case ZEDIT_MAIN_MENU:
    switch (*arg) 
    { case 'q':
      case 'Q':
        if(OLC_ZONE(d)->age || OLC_ZONE(d)->number)
        { send_to_char("Do you wish to save the changes to the zone info? (y/n) : ", d->character);
          OLC_MODE(d) = ZEDIT_CONFIRM_SAVESTRING;
        } else
        { send_to_char("No changes made.\r\n", d->character);
          cleanup_olc(d, CLEANUP_ALL);
        }
        break;
      case 'n':
      case 'N':
        /*. New entry .*/
        send_to_char("What number in the list should the new command be? : ", d->character);
        OLC_MODE(d) = ZEDIT_NEW_ENTRY;
        break;
      case 'e':
      case 'E':
        /*. Change an entry .*/
        send_to_char("Which command do you wish to change? : ", d->character);
        OLC_MODE(d) = ZEDIT_CHANGE_ENTRY;
        break;
      case 'd':
      case 'D':
        /*. Delete an entry .*/
        send_to_char("Which command do you wish to delete? : ", d->character);
        OLC_MODE(d) = ZEDIT_DELETE_ENTRY;
        break;
      case 'z':
      case 'Z':
        /*. Edit zone name .*/
        send_to_char("Enter new zone name : ", d->character);
        OLC_MODE(d) = ZEDIT_ZONE_NAME;
        break;
      case 't':
      case 'T':
        /*. Edit zone top .*/
        if(GET_LEVEL(d->character) < LVL_HBUILDER)
          zedit_disp_menu(d);
        else
        { send_to_char("Enter new top of zone : ", d->character);
          OLC_MODE(d) = ZEDIT_ZONE_TOP;
        }
        break;
      case 'l':
      case 'L':
        /*. Edit zone lifespan .*/
        send_to_char("Enter new zone lifespan : ", d->character);
        OLC_MODE(d) = ZEDIT_ZONE_LIFE;
        break;
      case 'r':
      case 'R':
        /*. Edit zone reset mode .*/
        send_to_char("\r\n"
                     "0) Never reset\r\n"
                     "1) Reset only when no players in zone\r\n"
                     "2) Normal reset\r\n"
                     "Enter new zone reset type : ", d->character);
        OLC_MODE(d) = ZEDIT_ZONE_RESET;
        break;
    case 'B':
    case 'b':
      if (GET_LEVEL(d->character) <  LVL_GRGOD) {
    send_to_char("You're not allowed to do this! Try another command: ",d->character);
      } else {
    display_blist(d);
    OLC_MODE(d) = ZEDIT_ENTER_BUILD;
      }
      break;
      default:
        zedit_disp_menu(d);
        break;
    }
    break; /*. End ZEDIT_MAIN_MENU .*/
/*-------------------------------------------------------------------*/ 
  case ZEDIT_ADD_BUILDER:
    if (!*arg) { 
      display_blist(d);
      OLC_MODE(d) = ZEDIT_ENTER_BUILD;
      return;    
    }  
  
    if (!ptable_lookup(arg)) {  
      send_to_char("No such player exists, try again: ",d->character);
      return;
    } 
    if (name_in_list(d,arg)) {  
      send_to_char("That player is already permitted to edit this zone, try again: ",d->character);
      return;
    }   

    if (NULL == OLC_ZONE(d)->hfl_Editors)
      OLC_ZONE(d)->hfl_Editors = createDll();

    append(OLC_ZONE(d)->hfl_Editors,strdup(arg));
    OLC_ZONE(d)->all = 0; // Reset eventual all flag if individually named editors...
    OLC_ZONE(d)->age = 1; 
    OLC_ZONE(d)->number = 1;
    display_blist(d);
    OLC_MODE(d) = ZEDIT_ENTER_BUILD;
    break;
/*-------------------------------------------------------------------*/
  case ZEDIT_DELETE_BUILDER:
    if (!*arg || OLC_ZONE(d)->all || NULL ==  OLC_ZONE(d)->hfl_Editors || 0 == getNumber(OLC_ZONE(d)->hfl_Editors)) {  
      display_blist(d);
      OLC_MODE(d) = ZEDIT_ENTER_BUILD;
      return;
    }  
    if (!atoi(arg)) {  
      if (!name_in_list(d,arg)) { 
    send_to_char("No such player in list, try again: ",d->character);
    return;
      } else { // okay to delete...
    delete_name_ch(d,arg);

    OLC_ZONE(d)->all = 0; // Reset eventual all flag if individually named editors...
    OLC_ZONE(d)->age = 1;
    OLC_ZONE(d)->number = 1;
    display_blist(d);
    OLC_MODE(d) = ZEDIT_ENTER_BUILD;     
      } 
    } else { // numerical argument, try to delete...
      if (!delete_name_nr(d,atoi(arg))) {
    send_to_char("Invalid number, no such builder in list, try again: ",d->character);
    return;
      } 
      OLC_ZONE(d)->all = 0; // Reset eventual all flag if individually named editors...
      OLC_ZONE(d)->age = 1;
      OLC_ZONE(d)->number = 1;
      display_blist(d);
      OLC_MODE(d) = ZEDIT_ENTER_BUILD; 
    } 

    break;
/*-------------------------------------------------------------------*/   
  case ZEDIT_ENTER_BUILD:
    switch (*arg) {  
    case 'A': // add a new builder
    case 'a': 
      send_to_char("Enter name of builder to add: ",d->character);
      OLC_MODE(d) = ZEDIT_ADD_BUILDER;
      break;

    case 'D': // Delete one builder
    case 'd':
      send_to_char("Enter number or name of builder to delete: ",d->character);
      OLC_MODE(d) = ZEDIT_DELETE_BUILDER;
      break;

    case 'p': // Purge all builders
    case 'P':
      purge_all_builders(d);
      OLC_ZONE(d)->all = 0; // Reset eventual all flag if individually named editors...
      OLC_ZONE(d)->age = 1;
      OLC_ZONE(d)->number = 1;
      display_blist(d);
      OLC_MODE(d) = ZEDIT_ENTER_BUILD;         
      break;

    case 'F': // Set full permissions for all
    case 'f':
      purge_all_builders(d); // No need for individual builders if anyone may edit :)
      OLC_ZONE(d)->all = 1;  // Allow anyone with correct CG to edit.
      OLC_ZONE(d)->age = 1;
      OLC_ZONE(d)->number = 1;
      display_blist(d);
      OLC_MODE(d) = ZEDIT_ENTER_BUILD;       
      break;

    default: // Assume we're done, and exit to main menu...
      zedit_disp_menu(d);
      OLC_MODE(d) = ZEDIT_MAIN_MENU;
      break;
    } 
    break;
/*-------------------------------------------------------------------*/
  case ZEDIT_NEW_ENTRY:
    /*. Get the line number and insert the new line .*/
    pos = atoi(arg);
    if (isdigit(*arg) && new_command(d, pos))
    {  if (start_change_command(d, pos))
       { zedit_disp_comtype(d);
         OLC_ZONE(d)->age = 1;
       }
    } else
      zedit_disp_menu(d);
    break;

/*-------------------------------------------------------------------*/
  case ZEDIT_DELETE_ENTRY:
    /*. Get the line number and delete the line .*/
    pos = atoi(arg);
    if(isdigit(*arg))
    { delete_command(d, pos);
      OLC_ZONE(d)->age = 1;
    }
    zedit_disp_menu(d);
    break;

/*-------------------------------------------------------------------*/
  case ZEDIT_CHANGE_ENTRY:
    /*. Parse the input for which line to edit, and goto next quiz .*/
    pos = atoi(arg);
    if(isdigit(*arg) && start_change_command(d, pos))
    { zedit_disp_comtype(d);
      OLC_ZONE(d)->age = 1;
    } else
      zedit_disp_menu(d);
    break;

/*-------------------------------------------------------------------*/
  case ZEDIT_COMMAND_TYPE:
    /*. Parse the input for which type of command this is, 
        and goto next quiz .*/
    OLC_CMD(d).command = toupper(*arg);
    if (!OLC_CMD(d).command || (strchr("MOPEDGRLTUAV", OLC_CMD(d).command) == NULL))
    { send_to_char("Invalid choice, try again : ", d->character);
    } else
    { if (OLC_VAL(d))
      { /*. If there was a previous command .*/
      if (OLC_CMD(d).command == 'T' || OLC_CMD(d).command == 'V') {
       OLC_CMD(d).if_flag = 1;
       zedit_disp_arg1(d);
      } else {
         send_to_char("Is this command dependent on the success of the previous one? (y/n)\r\n", 
                      d->character);
         OLC_MODE(d) = ZEDIT_IF_FLAG;
      }
      } else
      { /*. 'if-flag' not appropriate .*/
        OLC_CMD(d).if_flag = 0;
        zedit_disp_arg1(d);
      }
    }
    break;

/*-------------------------------------------------------------------*/
  case ZEDIT_IF_FLAG:
    /*. Parse the input for the if flag, and goto next quiz .*/
    switch(*arg)
    { case 'y':
      case 'Y':
        OLC_CMD(d).if_flag = 1;
        break;
      case 'n':
      case 'N':
        OLC_CMD(d).if_flag = 0;
        break;
      default:
        send_to_char("Try again : ", d->character);
        return;
    }
    zedit_disp_arg1(d);
    break;


/*-------------------------------------------------------------------*/
  case ZEDIT_ARG1:
    /*. Parse the input for arg1, and goto next quiz .*/
    if  (!isdigit(*arg))
    { send_to_char("Must be a numeric value, try again : ", d->character);
      return;
    }
    switch(OLC_CMD(d).command)
    { 
      case 'M':
      case 'L':
      case 'T':
        pos = atoi(arg);
        if (real_mobile(pos) >= 0)
        { OLC_CMD(d).arg1 = pos;
          zedit_disp_arg2(d);
        } else
          send_to_char("That mobile does not exist, try again : ", d->character);
        break;
      case 'U':
      case 'O':
      case 'P':
      case 'E':
      case 'G':
        pos = atoi(arg);
        if (real_object(pos) >= 0)
        { OLC_CMD(d).arg1 = pos;
          zedit_disp_arg2(d);
        } else
          send_to_char("That object does not exist, try again : ", d->character);
        break;
      case 'A':
      case 'V':
        if (atoi(arg)<MOB_TRIGGER || atoi(arg)>WLD_TRIGGER)
         send_to_char("Invalid input.", d->character);
        else {
         OLC_CMD(d).arg1 = atoi(arg);
         zedit_disp_arg2(d);
        }
        break;
      case 'D':
      case 'R':
      default:
        /*. We should never get here .*/
        cleanup_olc(d, CLEANUP_ALL);
        mudlog("SYSERR: OLC: zedit_parse(): case ARG1: Ack!", BRF, LVL_BUILDER, TRUE);
        break;
    }
    break;


/*-------------------------------------------------------------------*/
  case ZEDIT_ARG2:
    /*. Parse the input for arg2, and goto next quiz .*/
    if  (!isdigit(*arg))
    { send_to_char("Must be a numeric value, try again : ", d->character);
      return;
    }
    switch(OLC_CMD(d).command)
    { case 'M':
      case 'O':
        OLC_CMD(d).arg2 = atoi(arg);
        OLC_CMD(d).arg3 = OLC_NUM(d);
        zedit_disp_percentage(d);
        break;
      case 'G':
        OLC_CMD(d).arg2 = atoi(arg);
        zedit_disp_percentage(d);
        break; 
      case 'P':
      case 'L':
      case 'T':
      case 'U':
      case 'E':
        OLC_CMD(d).arg2 = atoi(arg);
        zedit_disp_arg3(d);
        break;
      case 'V':
        OLC_CMD(d).arg2 = atoi(arg);
        if (OLC_CMD(d).arg1 == WLD_TRIGGER)
         OLC_CMD(d).arg3 = OLC_NUM(d);
        send_to_char("Enter the global name : ", d->character);
        OLC_MODE(d) = ZEDIT_SARG1;
        break;
      case 'D':
        pos = atoi(arg);
        /*. Count dirs .*/
        while(*dirs[i] != '\n')
          i++;
        if ((pos < 0) || (pos > i))
          send_to_char("Try again : ", d->character);
        else
        { OLC_CMD(d).arg2 = pos;
          zedit_disp_arg3(d);
        }
        break;
      case 'R':
        pos = atoi(arg);
        if (real_object(pos) >= 0)
        { OLC_CMD(d).arg2 = pos;
          zedit_disp_percentage(d);
        } else
          send_to_char("That object does not exist, try again : ", d->character);
        break;
      case 'A':
        if (real_trigger(atoi(arg)) >= 0) {
          OLC_CMD(d).arg2 = atoi(arg);
          zedit_disp_menu(d);
        } else
          send_to_char("That trigger does not exist, try again : ", d->character);
        break;
      default:
        /*. We should never get here .*/
        cleanup_olc(d, CLEANUP_ALL);
        mudlog("SYSERR: OLC: zedit_parse(): case ARG2: Ack!", BRF, LVL_BUILDER, TRUE);
        break;
    }
    break;

/*-------------------------------------------------------------------*/
  case ZEDIT_PERC:
    if (!isdigit(*arg)) { 
      send_to_char("Must be a numeric value, try again : ", d->character);
      return;
    }

    OLC_CMD(d).percentage = atoi(arg);
    zedit_disp_menu(d);
    break;

  case ZEDIT_ARG3:
    /*. Parse the input for arg3, and go back to main menu.*/
    if (!isdigit(*arg) && OLC_CMD(d).command != 'V') 
    { send_to_char("Must be a numeric value, try again : ", d->character);
      return;
    }
    switch(OLC_CMD(d).command)
    { case 'E':
        pos = atoi(arg);
        /*. Count number of wear positions (could use NUM_WEARS,
            this is more reliable) .*/
        while(*equipment_types[i] != '\n')
          i++;
        if ((pos < 0) || (pos > i))
          send_to_char("Try again : ", d->character);
        else
        { OLC_CMD(d).arg3 = pos;
          zedit_disp_percentage(d);
        }
        break;
      case 'L':
      case 'U':
      case 'T':
      case 'P':
        pos = atoi(arg);
        if (real_object(pos) >= 0)
        { OLC_CMD(d).arg3 = pos;
          zedit_disp_percentage(d);
        } else
          send_to_char("That object does not exist, try again : ", d->character);
        break;
      case 'D':
        pos = atoi(arg);
        if ((pos < 0) || (pos > 2))
          send_to_char("Try again : ", d->character);
        else
        { OLC_CMD(d).arg3 = pos;
          zedit_disp_percentage(d);
        }
        break;
      case 'A':
      case 'V':
      case 'M':
      case 'O':
      case 'G':
      case 'R':
      default:
        /*. We should never get here .*/
        cleanup_olc(d, CLEANUP_ALL);
        mudlog("SYSERR: OLC: zedit_parse(): case ARG3: Ack!", BRF, LVL_BUILDER, TRUE);
        break;
    }
    break;
  
/*-------------------------------------------------------------------*/   
  case ZEDIT_SARG1:     
   if (strlen(arg)) {       
    OLC_CMD(d).sarg1 = str_dup(arg);       
    OLC_MODE(d) = ZEDIT_SARG2;       
    send_to_char("Enter the global value : ", d->character);      
   } else       
     send_to_char("Must have some name to assign : ", d->character);     
   break;
/*-------------------------------------------------------------------*/   
  case ZEDIT_SARG2:     
   if (strlen(arg)) {       
    OLC_CMD(d).sarg2 = str_dup(arg);       
    zedit_disp_menu(d);     
   } else       
      send_to_char("Must have some value to set it to :", d->character);     
   break; 
/*-------------------------------------------------------------------*/
  case ZEDIT_ZONE_NAME:
    /*. Add new name and return to main menu .*/
    free(OLC_ZONE(d)->name);
    OLC_ZONE(d)->name = str_dup(arg);
    OLC_ZONE(d)->number = 1;
    zedit_disp_menu(d);
    break;

/*-------------------------------------------------------------------*/
  case ZEDIT_ZONE_RESET:
    /*. Parse and add new reset_mode and return to main menu .*/
    pos = atoi(arg);
    if (!isdigit(*arg) || (pos <  0) || (pos > 2))
      send_to_char("Try again (0-2) : ", d->character);
    else
    { OLC_ZONE(d)->reset_mode = pos;
      OLC_ZONE(d)->number = 1;
      zedit_disp_menu(d);
    }
    break; 

/*-------------------------------------------------------------------*/
  case ZEDIT_ZONE_LIFE:
    /*. Parse and add new lifespan and return to main menu .*/
    pos = atoi(arg);
    if (!isdigit(*arg) || (pos <  0) || (pos > 240))
      send_to_char("Try again (0-240) : ", d->character);
    else
    { OLC_ZONE(d)->lifespan = pos;
      OLC_ZONE(d)->number = 1;
      zedit_disp_menu(d);
    }
    break; 

/*-------------------------------------------------------------------*/
  case ZEDIT_ZONE_TOP:
    /*. Parse and add new top room in zone and return to main menu .*/
    if (OLC_ZNUM(d) == top_of_zone_table)
      OLC_ZONE(d)->top = MAX(OLC_ZNUM(d) * 100, MIN(32000, atoi(arg)));
    else
      OLC_ZONE(d)->top = MAX(OLC_ZNUM(d) * 100, MIN(zone_table[OLC_ZNUM(d) +1].number * 100, atoi(arg)));
    zedit_disp_menu(d);
    break; 

/*-------------------------------------------------------------------*/
  default:
    /*. We should never get here .*/
    cleanup_olc(d, CLEANUP_ALL);
    mudlog("SYSERR: OLC: zedit_parse(): Reached default case!",BRF,LVL_BUILDER,TRUE);
    break;
  }
}
/*. End of parse_zedit() .*/

#define ZCMDS zone_table[zone_num].cmd[subcmd]
ACMD(do_zcmd)
{
  int zone_num;
  one_argument(argument, arg);
  if (IS_NPC(ch))
    return;
  if (GET_LEVEL(ch) < LVL_GRGOD)
    return;
  if( !arg || !*arg )
    return;
  if( is_number( arg ) ) 
  {
    int i, j;
    for (j = atoi(arg), i = 0; zone_table[i].number != j && i <= top_of_zone_table; i++);
      if( i > top_of_zone_table )
      {
	send_to_char("That is not a valid zone.\r\n", ch);
	return;
      }
      else
	zone_num = i;
  }
  else
  {
    send_to_char("That is not even a number!\r\n", ch);
    return;
  }
  send_to_char(" Zone        Arg1  Arg2  Arg3  Arg4\r\n", ch);
  subcmd = 0;
  while (zone_table[zone_num].cmd[subcmd].command != 'S') 
  {
     switch (zone_table[zone_num].cmd[subcmd].command) 
     {
       case 'M':
         sprintf(buf, "%5d  %c %d %5ld %5ld %5ld %5d %ld\r\n", zone_num,
             ZCMDS.command, ZCMDS.if_flag, ZCMDS.arg1, ZCMDS.arg2,
             //ZCMDS.command, ZCMDS.if_flag, mob_index[ZCMDS.arg1].virtual, ZCMDS.arg2,
             //world[ZCMDS.arg3].number, ZCMDS.percentage);
             ZCMDS.arg3, ZCMDS.percentage, ZCMDS.line);
         break;
      case 'O':
         sprintf(buf, "%5d  %c %d %5ld %5ld %5ld %5d %ld\r\n", zone_num,
             ZCMDS.command, ZCMDS.if_flag, ZCMDS.arg1, ZCMDS.arg2,
             //ZCMDS.command, ZCMDS.if_flag, obj_index[ZCMDS.arg1].virtual, ZCMDS.arg2,
             //world[ZCMDS.arg3].number, ZCMDS.percentage);
             ZCMDS.arg3, ZCMDS.percentage, ZCMDS.line);
         break;
      case 'P':
         sprintf(buf, "%5d  %c %d %5ld %5ld %5ld %5d %ld\r\n", zone_num,
             ZCMDS.command, ZCMDS.if_flag, ZCMDS.arg1, ZCMDS.arg2,
             //ZCMDS.command, ZCMDS.if_flag, obj_index[ZCMDS.arg1].virtual, ZCMDS.arg2,
             //obj_index[ZCMDS.arg3].virtual, ZCMDS.percentage);
             ZCMDS.arg3, ZCMDS.percentage, ZCMDS.line);
         break;
      case 'G':
         sprintf(buf, "%5d  %c %d %5ld %5ld %5ld %5d %ld\r\n", zone_num,
             ZCMDS.command, ZCMDS.if_flag, ZCMDS.arg1, ZCMDS.arg2,
             //ZCMDS.command, ZCMDS.if_flag, obj_index[ZCMDS.arg1].virtual, ZCMDS.arg2,
             ZCMDS.arg3, ZCMDS.percentage, ZCMDS.line);
         break;
      case 'D':
         sprintf(buf, "%5d  %c %d %5ld %5ld %5ld %5d %ld\r\n", zone_num,
             ZCMDS.command, ZCMDS.if_flag, ZCMDS.arg1, ZCMDS.arg2,
             ZCMDS.arg3, ZCMDS.percentage, ZCMDS.line);
         break;
      default:
         sprintf(buf, "%5d  %c %d %5ld %5ld %5ld %5d %ld\r\n", zone_num,
             ZCMDS.command, ZCMDS.if_flag, ZCMDS.arg1, ZCMDS.arg2,
             ZCMDS.arg3, ZCMDS.percentage, ZCMDS.line);
         break;
    }
    send_to_char(buf, ch);
    subcmd++;
  }
}
