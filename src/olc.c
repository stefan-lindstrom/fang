/******************************************************************************
 * $Id: olc.c,v 1.7 2002/11/07 04:02:03 fang Exp $
 *   
 * File: olc.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: olc.c,v 
 *   Revision 1.2  2002/07/28 13:56:05  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.6  2002/07/06 07:05:13  fang
 *   Drath: IMMLOCK for Fred and 2nd to last XLOG commit
 *
 *   Revision 1.5  2002/06/02 22:14:44  fang
 *   Maz: PEDIT
 *
 *   Revision 1.4  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.3  2002/03/12 15:02:15  fang
 *   Minor fixes to interpreter.c. dig is updated to utilize the new build permission thingies.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.17  2001/08/21 15:44:30  mud
 *   *** empty log message ***
 *
 *   Revision 1.16  2001/05/22 09:07:06  mud
 *   DG-Scripts for Fang.
 *
 *   Revision 1.15  2000/09/04 03:34:40  mud
 *   Warnings removed...
 *
 *   Revision 1.14  2000/08/31 14:26:02  mud
 *   *** empty log message ***
 *
 *   Revision 1.13  2000/08/30 02:39:25  mud
 *   made hedit available to LVL_IMMORT
 *
 *   Revision 1.12  2000/08/10 15:06:29  mud
 *   added hedit (help editor)
 *
 *   Revision 1.11  2000/06/17 20:23:42  mud
 *   New OLC perm system fixes.
 *
 *   Revision 1.10  2000/06/10 16:40:04  mud
 *   Minor requests from purdy fixed...
 *
 *   Revision 1.9  2000/06/09 18:40:53  mud
 *   Purdans 'Editor Name List' for zoned added.
 *
 *   Revision 1.8  2000/06/08 14:49:22  mud
 *   olc name lists saved/parsed in xmlZones. Names and all flag checked in OLC permissions.
 *
 *   Revision 1.7  2000/04/20 17:36:52  mud
 *   Fixed erronous saving of tildes '~' in mobs anything else not saving
 *   xml yet.
 *
 *   Revision 1.6  2000/03/28 09:57:43  mud
 *   Minor bugfix in olc...it's -mlog()- not log() (log() beeing the natural logarithm from the math library :))
 *
 *   Revision 1.5  2000/03/25 16:30:59  mud
 *   Added rlink to F1.
 *
 *   Revision 1.4  2000/03/09 12:47:44  mud
 *   Code cleanup. Compiles without warnings now...
 *
 *   Revision 1.3  2000/03/01 10:44:06  mud
 *   Function for checking multiple edits in OLC added
 *
 *   Revision 1.2  2000/02/21 14:00:18  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*  _TwyliteMud_ by Rv.                          Based on CircleMud3.0bpl9 *
*    				                                          *
*  OasisOLC - olc.c 		                                          *
*    				                                          *
*  Copyright 1996 Harvey Gilpin.                                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define _RV_OLC_

#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "interpreter.h"
#include "comm.h"
#include "utils.h"
#include "db.h"
#include "olc.h"
#include "constants.h"
#include "screen.h"
#include "dg_olc.h"

/*. Internal data .*/

struct olc_scmd_data {
  char *text;
  int con_type;
};

struct olc_scmd_data olc_scmd_info[] =
{
  {"room", 	CON_REDIT},
  {"object", 	CON_OEDIT},
  {"room",	CON_ZEDIT},
  {"mobile", 	CON_MEDIT},
  {"shop", 	CON_SEDIT},
  {"action",    CON_AEDIT},
  {"quest",	CON_QEDIT},
  {"help",	CON_HEDIT},
  {"trigger",   CON_TRIGEDIT},
  {"policy",	CON_PEDIT},
};

int check_lock(int number, int subcmd)
{
  struct descriptor_data *d;
  for (d = descriptor_list; d; d = d->next) {
    if (d->connected == olc_scmd_info[subcmd].con_type) {
      if (d->olc && OLC_NUM(d) == number) {
    return 1;
      }
    }
  }
  return 0;
}

int allowed_to_edit_room(struct char_data *ch,int vroom) 
{
  int t1,t2,t3,t4;

  if ((GET_LEVEL(ch) < LVL_BUILDERGL) && 
      !IS_SET((ch)->player_specials->saved.command_groups, CG_BUILDER)) { 
    t1 = (GET_OLC_ZONE(ch) == zone_table[real_zone(world[ch->in_room].number)].number);
    t2 = (GET_OLC_ZONE(ch) == zone_table[real_zone(vroom)].number);
    t3 = builder_named(ch,real_zone(world[ch->in_room].number));
    t4 = builder_named(ch,real_zone(vroom));

    return ((t1 && t2)||(t3 && t4));
  }

  return 1;
}

int builder_named(struct char_data *ch,int number)
{  
  struct zone_data *z = (zone_table+number);

  if (z->all)
    return 1; /* Anyone with olc access may edit... */

  if (NULL == z->hfl_Editors || 0 == getNumber(z->hfl_Editors))
    return 0; /* Can't be named, since the name list is empty...*/

  first(z->hfl_Editors);
  do { 
    char *name = this(z->hfl_Editors);

    if (!strcasecmp(name,GET_RNAME(ch)))
      return 1;

  } while(next(z->hfl_Editors));
  return 0;
} 

/*------------------------------------------------------------*\
 Eported ACMD do_olc function

 This function is the OLC interface.  It deals with all the 
 generic OLC stuff, then passes control to the sub-olc sections.
\*------------------------------------------------------------*/

ACMD(do_olc)
{
  int number = -1, save = 0, real_num;
  struct descriptor_data *d;

  if (IS_NPC(ch))
    /*. No screwing arround .*/
    return;

  if (subcmd == SCMD_OLC_SAVEINFO)
  { olc_saveinfo(ch);
    return;
  }

  /*. Parse any arguments .*/
  two_arguments(argument, buf1, buf2);
  if (!*buf1)
  { /* No argument given .*/
    switch(subcmd)
    { case SCMD_OLC_ZEDIT:
      case SCMD_OLC_REDIT:
        number = world[IN_ROOM(ch)].number;
        break;
      case SCMD_OLC_OEDIT:
      case SCMD_OLC_MEDIT:
      case SCMD_OLC_SEDIT:
      case SCMD_OLC_QEDIT:
      case SCMD_OLC_TRIGEDIT:
        sprintf(buf, "Specify a %s VNUM to edit.\r\n", olc_scmd_info[subcmd].text);
        send_to_char (buf, ch);
        return;
      case SCMD_OLC_HEDIT:
        sprintf(buf, "Specify a %s entry to edit.\r\n", olc_scmd_info[subcmd].text);
        send_to_char(buf, ch);
        return;
      case SCMD_OLC_PEDIT:
        sprintf(buf, "Specify a %s entry to edit.\r\n", olc_scmd_info[subcmd].text);
        send_to_char(buf, ch);
        return;
      case SCMD_OLC_AEDIT:
        send_to_char("Specify an action to edit.\r\n", ch);
        return;
    }
  } else if (!isdigit (*buf1))
  {
    if (strncmp("save", buf1, 4) == 0) {
        if (subcmd == SCMD_OLC_HEDIT || subcmd == SCMD_OLC_AEDIT) {
            save = 1;  
            number = 0;
        } else if (subcmd == SCMD_OLC_PEDIT) {
            save = 1;
            number = 0;
        } else if (!*buf2) {
        send_to_char("Save which zone?\r\n", ch);
        return;
      } else 
      { save = 1;
        number = atoi(buf2) * 100;
      }
    } else if (subcmd == SCMD_OLC_PEDIT) {
      number = 0;
    } else if (subcmd == SCMD_OLC_HEDIT || subcmd == SCMD_OLC_AEDIT )
      number = 0;
    else if (subcmd == SCMD_OLC_ZEDIT && GET_LEVEL(ch) >= (LVL_GRGOD-1))
    { if ((strncmp("new", buf1, 3) == 0) && *buf2)
        zedit_new_zone(ch, atoi(buf2));
      else
        send_to_char("Specify a new zone number.\r\n", ch);
      return;
    } else
    { send_to_char ("Yikes!  Stop that, someone will get hurt!\r\n", ch);
      return;
    }
  }

  /*. If a numeric argument was given, get it .*/
  if (number == -1)
    number = atoi(buf1);

  /*. Check whatever it is isn't already being edited .*/
  for (d = descriptor_list; d; d = d->next)
    if (d->connected == olc_scmd_info[subcmd].con_type)
      if (d->olc && OLC_NUM(d) == number) {
        if (subcmd == SCMD_OLC_HEDIT)
          sprintf(buf, "Help files are already being editted by %s.\r\n",
                  (CAN_SEE(ch, d->character) ? GET_NAME(d->character) : "someone"));
        else if (subcmd == SCMD_OLC_PEDIT)
          sprintf(buf, "Policy files are already being edited by %s.\r\n",
                  (CAN_SEE(ch, d->character) ? GET_NAME(d->character) : "someone"));
        else if (subcmd == SCMD_OLC_AEDIT)
          sprintf(buf, "Actions are already being editted by %s.\r\n",
                  (CAN_SEE(ch, d->character) ? GET_NAME(d->character) : "someone"));
        else
          sprintf(buf, "That %s is currently being edited by %s.\r\n",
                  olc_scmd_info[subcmd].text, GET_NAME(d->character));
        send_to_char(buf, ch);
        return;
      }

  d = ch->desc; 

  /*. Give descriptor an OLC struct .*/
  CREATE(d->olc, struct olc_data, 1);

  /*. Find the zone (or help rnum).*/
  if (subcmd == SCMD_OLC_AEDIT && !save)
    ;
  else if (subcmd == SCMD_OLC_HEDIT && !save)
    OLC_ZNUM(d) = find_help_rnum(buf1);
  else if (subcmd == SCMD_OLC_PEDIT && !save)
    OLC_ZNUM(d) = find_poli_rnum(buf1);
  else if ((OLC_ZNUM(d) = real_zone(number)) == -1) {
    send_to_char ("Sorry, there is no zone for that number!\r\n", ch); 
    free(d->olc);
    return;
  }

  /*. Everyone but IMPLs can only edit zones they have been assigned .*/
  if (subcmd != SCMD_OLC_HEDIT) {
    if ((GET_LEVEL(ch) < LVL_BUILDERGL) && !IS_SET((ch)->player_specials->saved.command_groups, CG_BUILDER)) {
      if ((zone_table[OLC_ZNUM(d)].number != GET_OLC_ZONE(ch)) && !builder_named(ch,OLC_ZNUM(d)))
      { send_to_char("You do not have permission to edit this zone.\r\n", ch); 
        free(d->olc);
        return;
      }
    } else if(( !(GET_LEVEL(ch) < LVL_IMPL)) &&
      (!ch->player_specials->saved.olc_zone == AEDIT_PERMISSION)) {
    send_to_char( "You do not have permission to edit actions.\r\n", ch);
    free(d->olc);
    return;
    }
  }
    
 
  if(save)
  { 
        switch(subcmd)
    { case SCMD_OLC_REDIT: 
        send_to_char("Saving all rooms in zone.\r\n", ch);
        sprintf(buf, "%s saves rooms for zone %d",
         GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch),
         zone_table[OLC_ZNUM(d)].number);
        xlog(SYS_OLC, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), buf, TRUE);
        redit_save_to_disk(d); 
        break;
      case SCMD_OLC_ZEDIT:
        send_to_char("Saving all zone information.\r\n", ch);
        sprintf(buf, "%s saves zone info for zone %d",
         GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch),
         zone_table[OLC_ZNUM(d)].number);
        xlog(SYS_OLC, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), buf, TRUE);
        zedit_save_to_disk(d); 
        break;
      case SCMD_OLC_OEDIT:
        send_to_char("Saving all objects in zone.\r\n", ch);
        sprintf(buf, "%s saves objects for zone %d",
         GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch),
         zone_table[OLC_ZNUM(d)].number);
        xlog(SYS_OLC, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), buf, TRUE);
        oedit_save_to_disk(d); 
        break;
      case SCMD_OLC_MEDIT:
        send_to_char("Saving all mobiles in zone.\r\n", ch);
        sprintf(buf, "%s saves mobs for zone %d",
         GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch),
         zone_table[OLC_ZNUM(d)].number);
        xlog(SYS_OLC, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), buf, TRUE);
        medit_save_to_disk(d); 
        break;
      case SCMD_OLC_SEDIT:
        send_to_char("Saving all shops in zone.\r\n", ch);
        sprintf(buf, "%s saves shops for zone %d",
         GET_DISGUISED(ch) ? GET_RNAME(ch) : GET_NAME(ch),
         zone_table[OLC_ZNUM(d)].number);
        xlog(SYS_OLC, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), buf, TRUE);
        sedit_save_to_disk(d); 
        break;
      case SCMD_OLC_HEDIT:
        /* shouldn't get here.. should be at do_heditutil in hedit.c */
        mlog("SYSERR:: Reached case SCMD_OLC_HEDIT in olc.c::do_olc <if save>");
        break;
      case SCMD_OLC_AEDIT:
        send_to_char("Saving all actions.\r\n", ch);
	sprintf(buf, "%s saves all actions", GET_NAME(ch));
        xlog(SYS_OLC, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), buf, TRUE);
	aedit_save_to_disk(d);
	break;
    }
    free(d->olc);
    return;
  }
 
  if( subcmd != SCMD_OLC_AEDIT) OLC_NUM(d) = number;
  else {
    OLC_NUM(d) = 0;
    OLC_STORAGE(d) = str_dup(buf1);
    for (OLC_ZNUM(d) = 0; (OLC_ZNUM(d) <= top_of_socialt); OLC_ZNUM(d)++)  {
      if (is_abbrev(OLC_STORAGE(d), soc_mess_list[OLC_ZNUM(d)].command))
      break;
    }
    if (OLC_ZNUM(d) > top_of_socialt)  {
      if (find_command(OLC_STORAGE(d)) > NOTHING)  {
	cleanup_olc(d, CLEANUP_ALL);
	send_to_char("That command already exists.\r\n", ch);
	return;
      }
      sprintf(buf, "Do you wish to add the '%s' action? ", OLC_STORAGE(d));
      send_to_char(buf, ch);
       OLC_MODE(d) = AEDIT_CONFIRM_ADD;
    }
    else  {
      sprintf(buf, "Do you wish to edit the '%s' action? ", soc_mess_list[OLC_ZNUM(d)].command);
      send_to_char(buf, ch);
      OLC_MODE(d) = AEDIT_CONFIRM_EDIT;
    }
  }

  /*. Steal players descriptor start up subcommands .*/
  switch(subcmd) {
    case SCMD_OLC_TRIGEDIT:
      if ((real_num = real_trigger(number)) >= 0)
       trigedit_setup_existing(d, real_num);
      else
       trigedit_setup_new(d);
      STATE(d) = CON_TRIGEDIT;
      break;
    case SCMD_OLC_HEDIT:
      if (OLC_ZNUM(d) < 0)
        hedit_setup_new(d, buf1);
      else
        hedit_setup_existing(d, OLC_ZNUM(d));
      STATE(d) = CON_HEDIT;
      break;
    case SCMD_OLC_PEDIT:
      if (OLC_ZNUM(d) < 0)
        pedit_setup_new(d, buf1);
      else
        pedit_setup_existing(d, OLC_ZNUM(d));
      STATE(d) = CON_PEDIT;
      break;
    case SCMD_OLC_REDIT:
      real_num = real_room(number);
      if (real_num >= 0)
        redit_setup_existing(d, real_num);
      else
        redit_setup_new(d);
      STATE(d) = CON_REDIT;
      break;
    case SCMD_OLC_ZEDIT:
      real_num = real_room(number);
      if (real_num < 0)
      {  send_to_char("That room does not exist.\r\n", ch); 
         free(d->olc);
         return;
      }
      zedit_setup(d, number);
      STATE(d) = CON_ZEDIT;
      break;
    case SCMD_OLC_MEDIT:
      real_num = real_mobile(number);
      if (real_num < 0)
        medit_setup_new(d);
      else
        medit_setup_existing(d, real_num);
      STATE(d) = CON_MEDIT;
      break;
    case SCMD_OLC_OEDIT:
      real_num = real_object(number);
      if (real_num >= 0)
        oedit_setup_existing(d, real_num);
      else
        oedit_setup_new(d);
      STATE(d) = CON_OEDIT;
      break;
    case SCMD_OLC_SEDIT:
      real_num = real_shop(number);
      if (real_num >= 0)
        sedit_setup_existing(d, real_num);
      else
        sedit_setup_new(d);
      STATE(d) = CON_SEDIT;
      break;
    case SCMD_OLC_AEDIT:
      STATE(d) = CON_AEDIT;
      break;
  }
  act("$n starts using OLC.", TRUE, d->character, 0, 0, TO_ROOM);
  SET_BIT(PLR_FLAGS (ch), PLR_WRITING);
}
/*------------------------------------------------------------*\
 Internal utlities 
\*------------------------------------------------------------*/

void olc_saveinfo(struct char_data *ch)
{ 
  struct olc_save_info *entry;
  static char *save_info_msg[8] = { "Rooms", "Objects", "Zone info", "Mobiles", "Shops", "Help", "Policy", "Actions" };

  if (olc_save_list)
    send_to_char("The following OLC components need saving:-\r\n", ch);
  else
    send_to_char("The database is up to date.\r\n", ch);

  for (entry = olc_save_list; entry; entry = entry->next) {
    if ((int)entry->type == OLC_SAVE_HELP)
      sprintf(buf, " - Help Entries.\r\n");
    else if ((int)entry->type == OLC_SAVE_ACTION)
      sprintf(buf, " - Actions.\r\n" );
    else if ((int)entry->type == OLC_SAVE_POLI)
      sprintf(buf, " - Policy Entries.\r\n");
    else
      sprintf(buf, " - %s for zone %ld.\r\n", 
    save_info_msg[(int)entry->type], entry->zone);
    send_to_char(buf, ch);
  }
}


int real_zone(int number)
{ int counter;
  for (counter = 0; counter <= top_of_zone_table; counter++)
    if ((number >= (zone_table[counter].number * 100)) &&
        (number <= (zone_table[counter].top)))
      return counter;

  return -1;
}

/*------------------------------------------------------------*\
 Exported utlities 
\*------------------------------------------------------------*/

/*. Add an entry to the 'to be saved' list .*/

void olc_add_to_save_list(int zone, byte type)
{ struct olc_save_info *new;

  /*. Return if it's already in the list .*/
  for(new = olc_save_list; new; new = new->next)
    if ((new->zone == zone) && (new->type == type))
      return;

  CREATE(new, struct olc_save_info, 1);
  new->zone = zone;
  new->type = type;
  new->next = olc_save_list;
  olc_save_list = new;
}

/*. Remove an entry from the 'to be saved' list .*/

void olc_remove_from_save_list(int zone, byte type)
{ struct olc_save_info **entry;
  struct olc_save_info *temp;

  for(entry = &olc_save_list; *entry; entry = &(*entry)->next)
    if (((*entry)->zone == zone) && ((*entry)->type == type))
    { temp = *entry;
      *entry = temp->next;
      free(temp);
      return;
    }
}

/*. Set the colour string pointers for that which this char will
    see at color level NRM.  Changing the entries here will change 
    the colour scheme throught the OLC.*/

void get_char_cols(struct char_data *ch)
{
/* These are now #defined */
/*
  nrm = CCNRM(ch, C_NRM);
  grn = CCGRN(ch, C_NRM);
  cyn = CCCYN(ch, C_NRM);
  yel = CCYEL(ch, C_NRM);
*/
}


/*. This procedure removes the '\r\n' from a string so that it may be
    saved to a file.  Use it only on buffers, not on the oringinal
    strings.*/

void strip_string(char *buffer)
{ register char *ptr, *str;

  ptr = buffer;
  str = ptr;

  while((*str = *ptr))
  { str++;
    ptr++;
    if (*ptr == '\r')
      ptr++;
    if (*ptr == '~') // Just in case.
      *ptr = ' '; /* Just set it to space, that shouldn't be able to fsck anything up! */
  }
}


/*. This procdure frees up the strings and/or the structures
    attatched to a descriptor, sets all flags back to how they
    should be .*/

void cleanup_olc(struct descriptor_data *d, byte cleanup_type)
{ 
  if (d->olc)
  {
    /*
     * Check for help.
     */ 
    if (OLC_STORAGE(d)) free(OLC_STORAGE(d));
    if (OLC_HELP(d)) {
      switch (cleanup_type) {
      case CLEANUP_ALL:       free_help(OLC_HELP(d)); break;
      case CLEANUP_STRUCTS:  free(OLC_HELP(d));       break;
      default: /* The caller has screwed up. */       break;  
      }
    }
    if (OLC_POLI(d)) {
      switch (cleanup_type) {
      case CLEANUP_ALL:       free_poli(OLC_POLI(d)); break;
      case CLEANUP_STRUCTS:  free(OLC_POLI(d));       break;
      default: /* The caller has screwed up. */       break;  
      }
    }
   
    /*. Check for room .*/
    if(OLC_ROOM(d))
    { /*. free_room performs no sanity checks, must be carefull here .*/
      switch(cleanup_type)
      { case CLEANUP_ALL:
          free_room(OLC_ROOM(d));
          break;
        case CLEANUP_STRUCTS:
          free(OLC_ROOM(d));
          break;
        default:
          /*. Caller has screwed up .*/
          break;
      }
    }

    /*. Check for object .*/
    if(OLC_OBJ(d))
    { /*. free_obj checks strings arn't part of proto .*/
      free_obj(OLC_OBJ(d));
    }

    /*. Check for mob .*/
    if(OLC_MOB(d))
    { /*. free_char checks strings arn't part of proto .*/
      free_char(OLC_MOB(d));
    }
  
    /*. Check for zone .*/
    if(OLC_ZONE(d))
    { /*. cleanup_type is irrelivent here, free everything .*/

      free(OLC_ZONE(d)->name);
      free(OLC_ZONE(d)->cmd);
      free(OLC_ZONE(d));
    }

    /*. Check for shop .*/
    if(OLC_SHOP(d))
    { /*. free_shop performs no sanity checks, must be carefull here .*/
      switch(cleanup_type)
      { case CLEANUP_ALL:
          free_shop(OLC_SHOP(d));
          break;
        case CLEANUP_STRUCTS:
          free(OLC_SHOP(d));
          break;
        default:
          /*. Caller has screwed up .*/
          break;
      }
    }

    if (OLC_ACTION(d)) {
      switch(cleanup_type) {
	case CLEANUP_ALL:
	  free_action( OLC_ACTION(d));
	  break;
	case CLEANUP_STRUCTS:
	  free(OLC_ACTION(d));
	  break;
	default:
	  break;
      }
    }

    /*. Restore desciptor playing status .*/
    if (d->character)
    { REMOVE_BIT(PLR_FLAGS(d->character), PLR_WRITING);
      STATE(d)=CON_PLAYING;
      act("$n stops using OLC.", TRUE, d->character, 0, 0, TO_ROOM);
    }
    free(d->olc);
  }
}

/* Create an exit in a room (vnum!) in this direction.  (No target) */
int create_dir(int room, int dir)
{
  struct room_data *rm;
  int rnum = real_room(room);
  struct room_direction_data *exit;

  if (rnum <= 0) {
      mlog("create_dir(): tried to create invalid door (room doesn't exists!)");
      return FALSE;
   }
   rm = &world[rnum];
   if (rm->dir_option[dir])
     return FALSE;

   exit = (struct room_direction_data *) calloc(1, sizeof(struct room_direction_data));
   exit->to_room = NOWHERE;
   exit->general_description = strdup("You see nothing special.\r\n");
   exit->key = -1;
   rm->dir_option[dir] = exit;

   return TRUE;

}

int free_dir(int room, int dir)
{
  struct room_data *rm;
  int rnum = real_room(room);
  
  if (rnum < 1) {
    mlog("free_dir(): tried to free invalid door (room doesn't exists!)");
    return FALSE;
  }
  rm = &world[rnum];
  if ((dir < 0) || (dir >= NUM_OF_DIRS)) {
     mlog("free_dir(): tried to free invalid door(Invalid direction)");
     return FALSE;
  }
  free(rm->dir_option[dir]);
  rm->dir_option[dir] = NULL;

  return TRUE;
}

#define RLINK_FORMAT    "Usage:  rlink <dir> <connect|disconnect> <1|2> [target]\r\n"
    
/* The big baby. */
ACMD(do_rlink)
{
  char direction[10], command[20], type[10], target[10];
  long vnum_base = 0, vnum_targ = 0, rnum_base = 0, rnum_targ = 0;
  int dir = 0, k = 0, type_int = 0, top_room = 0;
  long save_zone_1 = 0, save_zone_2 = 0, create_new_room = FALSE;
  struct room_data *targ = NULL,*base = NULL;
  struct room_direction_data *bdir = NULL;
   
  argument = two_arguments(argument, direction, command);  
  two_arguments(argument, type, target);   

  if (!*direction || !*command || !*type) {
    send_to_char(RLINK_FORMAT, ch);
    return;
  } else if (!is_number(type)) {
    send_to_char(RLINK_FORMAT, ch);
    return;
  }
  
  type_int = atoi(type);
  
  if (type_int != 1 && type_int != 2) {
    send_to_char(RLINK_FORMAT, ch);
    return;
  }

  rnum_base = ch->in_room;
  vnum_base = world[ch->in_room].number;
  base = &world[ch->in_room];

  if (!*target && !is_abbrev(command, "disconnect")) {
    create_new_room = TRUE;
  } else {
    if (!is_number(target)) {
      send_to_char(RLINK_FORMAT, ch);
      send_to_char("Hint: Type in a valid target room number.\r\n", ch);
      return;
    }
    vnum_targ = atoi(target);
    rnum_targ = real_room(vnum_targ);
    if (rnum_targ < 1)
    {
      send_to_char(RLINK_FORMAT, ch);
      return;
    }
    targ = &world[rnum_targ]; 
  }
  
  switch (*direction) {
    case 'n':
    case 'N':
      {
      if ((*(direction+1)) == 'e') dir = NORTHEAST;
      else if ((*(direction+1)) == 'w') dir = NORTHWEST;
      else dir = NORTH;
      break;
      }
    case 'e':
    case 'E':
      dir = EAST;
      break;
    case 's':
    case 'S':
      {
      if ((*(direction+1)) == 'e') dir = SOUTHEAST;
      else if ((*(direction+1)) == 'w') dir = SOUTHWEST;
      else dir = SOUTH;
      break; 
      }
    case 'w':
    case 'W':
      dir = WEST;
      break;
    case 'u':
    case 'U':
      dir = UP;
      break;
    case 'd':
    case 'D':
      dir = DOWN;
      break;
    default:
      send_to_char("No such direction!\r\n", ch);
      return;
  }
       
  bdir = base->dir_option[dir];
  if (bdir && !strncasecmp(command, "connect", strlen(command))) {
    send_to_char("An exit in that direction already exists! Remove it first\r\n", ch);
    return; 
  }
  if (!bdir && !strncasecmp(command, "disconnect", strlen(command))) {
    send_to_char("No exit in that direction exists! \r\n", ch);
    return;
  }
      
  save_zone_1 = base->zone;
  if (zone_table[save_zone_1].number != GET_OLC_ZONE(ch) && !IS_SET((ch)->player_specials->saved.command_groups, CG_BUILDER)
      && (GET_LEVEL(ch) < LVL_BUILDERGL) && !builder_named(ch,save_zone_1)) { 
    send_to_char("You cannot create exits in this zone.\r\n", ch);
    return; 
  }
    
  if (!create_new_room) {
    save_zone_2 = targ->zone;
  } else {
    top_room = (zone_table[save_zone_1].top);
    
    for (k = zone_table[save_zone_1].number*100; k <= top_room; k++) {
      if (real_room(k) < 1) {
        CREATE(ch->desc->olc, struct olc_data, 1);
        OLC_ROOM(ch->desc) = (struct room_data *) calloc(1, sizeof(struct room_data));
  
        OLC_ZNUM(ch->desc) = base->zone;
        OLC_NUM(ch->desc) = k;
        OLC_ROOM(ch->desc)->number = k;
        OLC_ROOM(ch->desc)->zone = save_zone_1;
        OLC_ROOM(ch->desc)->name = strdup("An unfinished room");
        OLC_ROOM(ch->desc)->description = strdup("You're in an unfinished  room.\r\n");
        OLC_VAL(ch->desc) = 0;
        vnum_targ = k;
         
      
        redit_save_internally(ch->desc);
        cleanup_olc(ch->desc, CLEANUP_STRUCTS);
        save_zone_2 = save_zone_1;
        sprintf(buf, "You have created new room #%ld.\r\n", vnum_targ);
        send_to_char(buf, ch);
        rnum_targ = real_room(k);  // Must re-get, since array is changed
        rnum_base = real_room(vnum_base); // Same here
        base = &world[rnum_base];
        targ = &world[rnum_targ];
        break;
      }
    }
    if (k > top_room) {
      send_to_char("Cannot create a new room in this zone!\r\n", ch);
      return;
    }
  }
  if (!create_new_room && !builder_named(ch,save_zone_2) &&
     (zone_table[save_zone_2].number != GET_OLC_ZONE(ch))
     && (GET_LEVEL(ch) < LVL_BUILDER)  && (type_int == 2)) {
    send_to_char("You cannot create exits in the target zone.\r\n", ch);
    return;
  }
        
  if (!strncasecmp(command, "connect", strlen(command))) {
    struct room_direction_data *exit = base->dir_option[dir];
        
    if (!exit)
    {
      create_dir(vnum_base, dir);
      exit = base->dir_option[dir];
    }
    exit->to_room = rnum_targ;
      
    if (type_int == 2) {
      targ = &world[real_room(vnum_targ)];
      exit = targ->dir_option[rev_dir[dir]];  
      if (!exit)
      {
        create_dir(vnum_targ, rev_dir[dir]);
        exit = targ->dir_option[rev_dir[dir]];  
      }
      exit->to_room = rnum_base;
      exit = NULL;
      if (!save_zone_2) 
        save_zone_2 = targ->zone;
    }
  }
        
  else if (!strncasecmp(command, "disconnect", strlen(command))) {
    struct room_direction_data *exit;
    if (type_int == 2) {
      exit = base->dir_option[dir];
      if (exit && (exit->to_room >= 0)) {
        free_dir(vnum_targ, rev_dir[dir]);
        if (exit)
          free_dir(vnum_base, dir);
        else {
          send_to_char("No such exit!\r\n", ch);
          return;
        }
        save_zone_2 = targ->zone;
      } else {
        send_to_char("There is no reciprocol exit to remove.\r\n", ch);
        if (exit && (exit->to_room >= 0)) {
          free_dir(ch->in_room, dir);
        } else {
          send_to_char("No such exit!\r\n", ch);
          return;
        }
      
      }
    } else if (type_int == 1) {
      exit = base->dir_option[dir];
      if (!exit || (exit->to_room < 0)) {
        send_to_char("No such exit!\r\n", ch);
        return;  
      } else {
        free_dir(vnum_base, dir);
      }
    } else {
      send_to_char("Invalid disconnect type.\r\n", ch);
      return;
    }
  } else {
    send_to_char("Invalid command type.  Valid choices are connect and disconnect.\r\n", ch);
    return;
  }
         
  if (!strncasecmp(command, "connect", strlen(command))) {
    sprintf(buf, "You make an exit %s to room %ld.\r\n", dirs[dir], vnum_targ);
    send_to_char(buf, ch);
  } else
    send_to_char("Exit deleted.\r\n", ch);
        
  olc_add_to_save_list(zone_table[save_zone_1].number, OLC_SAVE_ROOM);
  if (save_zone_2 && save_zone_2 != save_zone_1)
    olc_add_to_save_list(zone_table[save_zone_2].number, OLC_SAVE_ROOM);
  return;
}

