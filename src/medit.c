/******************************************************************************
 * $Id: medit.c,v 1.4 2002/11/07 04:02:02 fang Exp $
 *   
 * File: medit.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: medit.c,v 
 *   Revision 1.3  2002/09/23 05:52:42  fische
 *   fisch: clearing out lua triggers part 
 
 *   Revision 1.2  2002/07/28 13:56:04  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.4  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.3  2002/04/30 18:54:56  fang
 *   AC Cap - Request per CRE
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.12  2002/01/24 21:33:17  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.11.2.2  2002/01/24 03:20:32  mud
 *   Removed some further unescessary files (player.[ch], roomprog.[ch]
 *   old2new.c). Cleaned up some other stuff (guild stuff mostly).
 *
 *   Revision 1.11.2.1  2002/01/23 21:49:42  mud
 *   Removed objsave.[c|h] (old objsave stuff).
 *   Removed obj_file_elem and rent structs.
 *   Removed final vestiges of old guild system, except parts used
 *   in guild.[c|h] for backwards compatibility.
 *
 *   Revision 1.11  2001/10/01 14:30:37  mud
 *   *** empty log message ***
 *
 *   Revision 1.10  2001/05/22 09:07:06  mud
 *   DG-Scripts for Fang.
 *
 *   Revision 1.9  2001/01/19 17:51:51  mud
 *   Added line numbers to the editor.
 *
 *   Revision 1.8  2000/12/14 19:55:55  mud
 *   *** empty log message ***
 *
 *   Revision 1.7  2000/12/01 09:17:10  mud
 *   Further safety checks added to sedit/medit/redit
 *
 *   Revision 1.6  2000/11/07 12:00:39  mud
 *   Finally. XML Mobiles are done.
 *
 *   Revision 1.5  2000/10/31 01:28:10  mud
 *   added multiple recipients for mudmail. 5 should be enough..
 *
 *   Revision 1.4  2000/08/29 05:10:00  mud
 *   Now the 'real' hpdice code should be working. As soon as xml saving/parsing is in properly, the same will be true for mana/move.
 *
 *   Revision 1.3  2000/08/29 04:39:05  mud
 *   fixed support for mana/move/hp dice thingy (xdy+z) for future use
 *
 *   Revision 1.2  2000/02/21 14:00:17  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*  _TwyliteMud_ by Rv.                          Based on CircleMud3.0bpl9 *
*    				                                          *
*  OasisOLC - medit.c 		                                          *
*    				                                          *
*  Copyright 1996 Harvey Gilpin.                                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "interpreter.h"  
#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "comm.h"
#include "spells.h"
#include "utils.h"
#include "db.h"
#include "shop.h"
#include "olc.h"
#include "constants.h"
#include "fight.h"
#include "xmlMobiles.h"
#include "dg_olc.h"

/*-------------------------------------------------------------------*/
/*. Handy  macros .*/

#define GET_NDD(mob) ((mob)->mob_specials.damnodice)
#define GET_SDD(mob) ((mob)->mob_specials.damsizedice)
#define GET_ALIAS(mob) ((mob)->player.name)
#define GET_SDESC(mob) ((mob)->player.short_descr)
#define GET_LDESC(mob) ((mob)->player.long_descr)
#define GET_MDESC(mob) ((mob)->player.description)
#define GET_ATTACK(mob) ((mob)->mob_specials.attack_type)
#define S_KEEPER(shop) ((shop)->keeper)

/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*\
  utility functions 
\*-------------------------------------------------------------------*/
int emotestring_ok(char *string, struct mob_prog_list *mpl)
{
  int i, j, ok = TRUE, after_pause = FALSE;
  char allowed[30], *ptr; 
  struct mob_action_list *mal;

  for (mal = mpl->actions; mal; mal = mal->next)
    if (mal->action == MPA_PAUSE)
      after_pause = TRUE;

  if (!mpl)
    return FALSE;
  sprintf(allowed, "msen");

  i = mpl->main;
  if ((i != MPC_TIME) && (i != MPC_MOBENTRY) && (i != MPC_MOBLEAVE) && 
  (i != MPC_TICK) && (i != MPC_RANDOM) && !after_pause)
    sprintf(allowed,"msenMSEN");
 
  for (ptr = string; *ptr && ok; ptr ++) 
    if (*ptr == '$') {
      ptr++;
      if (!*ptr)
        return FALSE;
      ok = FALSE;
      for (j = 0; allowed[j]; j ++)
        if (*ptr == allowed[j])
          ok = TRUE;
    }
  return ok;
}

void medit_setup_new(struct descriptor_data *d)
{ struct char_data *mob;

  /*. Alloc some mob shaped space .*/
  CREATE(mob, struct char_data, 1);
  init_mobile(mob);
  
  GET_MOB_RNUM(mob) = -1;
  /*. default strings .*/
  GET_ALIAS(mob) = str_dup("mob unfinished");
  GET_SDESC(mob) = str_dup("the unfinished mob");
  GET_LDESC(mob) = str_dup("An unfinished mob stands here.\r\n");
  GET_MDESC(mob) = str_dup("It looks, err, unfinished.\r\n");

  OLC_MOB(d) = mob;
  OLC_VAL(d) = 0;   /*. Has changed flag .*/
  OLC_ITEM_TYPE(d) = MOB_TRIGGER;
  medit_disp_menu(d);
}

/*-------------------------------------------------------------------*/

void medit_setup_existing(struct descriptor_data *d, int rmob_num)
{ struct char_data *mob;

  /*. Alloc some mob shaped space .*/
  CREATE(mob, struct char_data, 1);
  copy_mobile(mob, mob_proto + rmob_num);
  OLC_MOB(d) = mob;
  OLC_ITEM_TYPE(d) = MOB_TRIGGER;
  dg_olc_script_copy(d);
  medit_disp_menu(d);
}

/*-------------------------------------------------------------------*/
/*. Remove one action .*/

void remove_action(struct descriptor_data *d)
{
  struct mob_action_list *temp, *this;
  
  this = GET_ACTLIST(d->character);  
  if (!this) {
    mlog("Error in mob program mediting at removal of actions: Attempt to remove nonexistant action.\r\n");
    exit(1);
  }
  
  REMOVE_FROM_LIST(this, GET_MAINLIST(d->character)->actions, next);  
  if ((this->argument) && *(this->argument))
    free(this->argument);    
  free(this);  
  GET_ACTLIST(d->character) = GET_MAINLIST(d->character)->actions;
}

/*-------------------------------------------------------------------*/
/*. Remove one sideterm .*/

void remove_sideterm(struct descriptor_data *d)
{
  struct side_term_list *temp, *this;
  
  this = GET_SIDELIST(d->character);  
  if (!this) {
    mlog("Error in mob program mediting at removal of actions: Attempt to remove nonexistant sideterm.\r\n");
    exit(1);
  }
  
  REMOVE_FROM_LIST(this, GET_MAINLIST(d->character)->sideterms, next);  
  if ((this->argument) && *(this->argument))
    free(this->argument); 
  GET_SIDELIST(d->character) = GET_MAINLIST(d->character)->sideterms;
  free(this);
}

/*-------------------------------------------------------------------*/
/*. Clear the action list in a mobprogram .*/

void medit_clear_actions(struct descriptor_data *d)
{
  struct mob_action_list *action, *next;
  
  for(action = GET_MAINLIST(d->character)->actions; action; action = next) {
    next = action->next;
    if ((action->argument) && *(action->argument))
      free(action->argument);
    free(action);
  }
  GET_MAINLIST(d->character)->actions = NULL;
  GET_ACTLIST(d->character) = NULL;
}

/*-------------------------------------------------------------------*/
/*. Remove all sideterms from a program .*/

void medit_clear_sideterms(struct descriptor_data *d)
{
  struct side_term_list *sides = NULL, *next = NULL;

  for (sides = GET_MAINLIST(d->character)->sideterms; sides; sides = next) {
    next = sides->next;
    if ((sides->argument) && *(sides->argument))
      free(sides->argument);
    free(sides);
  }
  GET_SIDELIST(d->character) = NULL;
  GET_MAINLIST(d->character)->sideterms = NULL;
}

/*-------------------------------------------------------------------*/
/*. Delete an entire mobprogram .*/

void delete_mobprog(struct mob_prog_list *prog, struct descriptor_data *d)
{
  struct mob_prog_list *temp = NULL;
  
  REMOVE_FROM_LIST(prog, GET_MOBPROG(OLC_MOB(d)), next);
  
              /* Freeing time! */
  medit_clear_actions(d);
  medit_clear_sideterms(d);
  
  if ((prog->argument) && *(prog->argument))
    free(prog->argument);
  free(prog); /* Done! */
}
/*-------------------------------------------------------------------*/
/*. Copy mob program to a new mob .*/

void copy_mobprog(struct char_data *from, struct char_data *to)
{
  struct mob_prog_list *mprogfrom, *mprogto;
  struct side_term_list *stlfrom, *stlto = NULL;
  struct mob_action_list *malfrom, *malto = NULL;
 
  if (GET_MOBPROG(from)) 
  {
    CREATE(GET_MOBPROG(to), struct mob_prog_list, 1);
  }
  else {
    GET_MOBPROG(to) = NULL;
    return;
  }
  
  for (mprogfrom = GET_MOBPROG(from), mprogto = GET_MOBPROG(to);
       mprogfrom; mprogfrom = mprogfrom->next) {
    mprogto->main = mprogfrom->main;
    mprogto->intarg = mprogfrom->intarg;
    if (mprogfrom->argument)
      mprogto->argument = strdup(mprogfrom->argument);
    else 
      mprogto->argument = NULL;
    
    if (mprogfrom->sideterms) {
      CREATE(mprogto->sideterms, struct side_term_list, 1);
      stlto = mprogto->sideterms;
    }
    else
      mprogto->sideterms = NULL;
      
    for (stlfrom = mprogfrom->sideterms; stlfrom; stlfrom = stlfrom->next) {
      stlto->number = stlfrom->number;
      stlto->not = stlfrom->not;
      stlto->int_arg = stlfrom->int_arg;
      if (stlfrom->argument)
        stlto->argument = strdup(stlfrom->argument);
      else
        stlto->argument = NULL;
      if (stlfrom->next) {
        CREATE(stlto->next, struct side_term_list, 1);
        stlto = stlto->next; 
      }
      else
        stlto->next = NULL;
    }
    
    if (mprogfrom->actions) {
      CREATE(mprogto->actions, struct mob_action_list, 1);
      malto = mprogto->actions;
    }
    else
      mprogto->actions = NULL;
      
    for (malfrom = mprogfrom->actions; malfrom; malfrom = malfrom->next) {
      malto->action = malfrom->action;
      malto->intarg = malfrom->intarg;
      if (malfrom->argument)
        malto->argument = strdup(malfrom->argument);
      else
        malto->argument = NULL;
      if (malfrom->next) {
        CREATE(malto->next, struct mob_action_list, 1);
        malto = malto->next;
      }
      else
        malto->next = NULL;
    }
    
    if (mprogfrom->next) {
      CREATE(mprogto->next, struct mob_prog_list, 1);
      mprogto = mprogto->next;
    }
    else
      mprogto->next = NULL;
  }
}

/*-------------------------------------------------------------------*/
/*. Copy one mob struct to another .*/

void copy_mobile(struct char_data *tmob, struct char_data *fmob)
{
  struct trig_proto_list *proto, *fproto;

  /*. Free up any used strings .*/
  if (GET_ALIAS(tmob))
    free(GET_ALIAS(tmob));
  if (GET_SDESC(tmob))
    free(GET_SDESC(tmob));
  if (GET_LDESC(tmob))
    free(GET_LDESC(tmob));
  if (GET_MDESC(tmob))
    free(GET_MDESC(tmob));
 
  proto = tmob->proto_script;
  while (proto) {
   fproto = proto;
   proto = proto->next;
   free(fproto);
  }
 
  /*.Copy mob .*/
/*  *tmob = *fmob;*/
  memcpy(tmob,fmob,sizeof(struct char_data));
  
  if (GET_MOBPROG(fmob))
    copy_mobprog(fmob, tmob);
    
  /*. Realloc strings .*/
  if (GET_ALIAS(fmob))
    GET_ALIAS(tmob) = str_dup(GET_ALIAS(fmob));

  if (GET_SDESC(fmob))
    GET_SDESC(tmob) = str_dup(GET_SDESC(fmob));

  if (GET_LDESC(fmob))
    GET_LDESC(tmob) = str_dup(GET_LDESC(fmob));

  if (GET_MDESC(fmob))
    GET_MDESC(tmob) = str_dup(GET_MDESC(fmob));

  /* copy the new script list */
  if (fmob->proto_script) {
   fproto = fmob->proto_script;
   CREATE(proto, struct trig_proto_list, 1);
   tmob->proto_script = proto;
   do {       
    proto->vnum = fproto->vnum;       
    fproto = fproto->next;      
    if (fproto) {         
     CREATE(proto->next, struct trig_proto_list, 1);         
     proto = proto->next;
    }     
   } while (fproto);  
  }
}


/*-------------------------------------------------------------------*/
/*. Ideally, this function should be in db.c, but I'll put it here for
    portability.*/

void init_mobile(struct char_data *mob)
{
  clear_char(mob);

  SET_BIT(MOB_FLAGS(mob), MOB_ISNPC);
  GET_HIT(mob) = 1;
  GET_MANA(mob) = 1;
  GET_MAX_MANA(mob) = 100;
  GET_MAX_MOVE(mob) = 100;
  GET_NDD(mob) = 1;
  GET_SDD(mob) = 1;
  GET_WEIGHT(mob) = 200;
  GET_HEIGHT(mob) = 198;

  mob->real_abils.str = 11;
  mob->real_abils.intel = 11;
  mob->real_abils.wis = 11;
  mob->real_abils.dex = 11;
  mob->real_abils.con = 11;
  mob->real_abils.cha = 11;
  mob->aff_abils = mob->real_abils;
  mob->affected = NULL;
  mob->player_specials = &dummy_mob;
  CREATE(mob->mob_specials.skills[0], int, 2);
  mob->mob_specials.skills[0] = NULL;

  mob->mob_specials.hpnodice   = 1;
  mob->mob_specials.hpsizedice = 1;
  mob->mob_specials.hpbonus    = 1;

  mob->mob_specials.mananodice   = 4;
  mob->mob_specials.manasizedice = 10;
  mob->mob_specials.manabonus    = 10;

  mob->mob_specials.movenodice   = 10;
  mob->mob_specials.movesizedice = 20;
  mob->mob_specials.movebonus    = 100;
}

/*-------------------------------------------------------------------*/
/*. Save new/edited mob to memory .*/

#define ZCMD zone_table[zone].cmd[cmd_no]

void medit_save_internally(struct descriptor_data *d)
{ int rmob_num, found = 0, new_mob_num = 0, shop;
  struct char_data *new_proto;
  struct index_data *new_index;
  struct char_data *live_mob;
  struct descriptor_data *dsc;
  
  /* put the script into proper position */   
  OLC_MOB(d)->proto_script = OLC_SCRIPT(d);

  rmob_num = real_mobile(OLC_NUM(d));


  /*. Mob exists? Just update it .*/
  if (rmob_num != -1) { 
    /*
     * First check if mob is a shop keeper, if so, set these flags, before updating live mobs and mob proto!
     */
    for(shop = 0; shop <= top_shop; shop++) {
      if (rmob_num == SHOP_KEEPER(shop)) 
    SET_BIT(MOB_FLAGS(&(mob_proto[new_mob_num])),MOB_NOCHARM | MOB_NOMAGIC | MOB_NOSLEEP | MOB_NOSTUN | MOB_NOTRIP | MOB_NOKILL);
    }

    OLC_MOB(d)->proto_script = OLC_SCRIPT(d);
    copy_mobile((mob_proto + rmob_num), OLC_MOB(d));

    /*. Update live mobiles .*/
    for(live_mob = character_list; live_mob; live_mob = live_mob->next)
      if(IS_MOB(live_mob) && GET_MOB_RNUM(live_mob) == rmob_num)
      { /*. Only really need update the strings, since these can cause
            protection faults.  The rest can wait till a reset/reboot .*/
        GET_ALIAS(live_mob) = GET_ALIAS(mob_proto + rmob_num);
        GET_SDESC(live_mob) = GET_SDESC(mob_proto + rmob_num);
        GET_LDESC(live_mob) = GET_LDESC(mob_proto + rmob_num);
        GET_MDESC(live_mob) = GET_MDESC(mob_proto + rmob_num);
      }
  } 
  /*. Mob does not exist, hafta add it .*/
  else
  { 
    CREATE(new_proto, struct char_data, top_of_mobt + 2);
    CREATE(new_index, struct index_data, top_of_mobt + 2);
   
    for (rmob_num = 0; rmob_num <= top_of_mobt; rmob_num++)
    { if (!found)
      { /*. Is this the place?  .*/
        if ((rmob_num > top_of_mobt) ||
            (mob_index[rmob_num].virtual > OLC_NUM(d)))
        { /*. Yep, stick it here .*/
          found = 1;
          new_index[rmob_num].virtual = OLC_NUM(d);
          new_index[rmob_num].number = 0;
          new_index[rmob_num].func = NULL;
          new_mob_num = rmob_num;
          GET_MOB_RNUM(OLC_MOB(d)) = rmob_num;
          copy_mobile((new_proto + rmob_num), OLC_MOB(d));
          /*. Copy the mob that should be here on top .*/
          new_index[rmob_num + 1] = mob_index[rmob_num];
          new_proto[rmob_num + 1] = mob_proto[rmob_num];
          GET_MOB_RNUM(new_proto + rmob_num + 1) = rmob_num + 1;
        } else
        { /*. Nope, copy over as normal.*/
          new_index[rmob_num] = mob_index[rmob_num];
          new_proto[rmob_num] = mob_proto[rmob_num];
        }
      } else
      { /*. We've already found it, copy the rest over .*/
        new_index[rmob_num + 1] = mob_index[rmob_num];
        new_proto[rmob_num + 1] = mob_proto[rmob_num];
        GET_MOB_RNUM(new_proto + rmob_num + 1) = rmob_num + 1;
      }
    }
    if (!found)
    { /*. Still not found, must add it to the top of the table .*/
      new_index[rmob_num].virtual = OLC_NUM(d);
      new_index[rmob_num].number = 0;
      new_index[rmob_num].func = NULL;
      new_mob_num = rmob_num;
      GET_MOB_RNUM(OLC_MOB(d)) = rmob_num;
      copy_mobile((new_proto + rmob_num), OLC_MOB(d));
    }

    /*. Replace tables .*/
    free(mob_proto);
    mob_proto = new_proto;
    // replace_index (mob_index, new_index);
    // *sigh* trying another approach, then...
    memcpy(mob_index, new_index, sizeof(struct  index_data)*(top_of_mobt +2));
    free(new_index);
    top_of_mobt++;

    /*. Update live mobile rnums .*/
    for(live_mob = character_list; live_mob; live_mob = live_mob->next)
      if(GET_MOB_RNUM(live_mob) >= new_mob_num)
        GET_MOB_RNUM(live_mob)++;
    
    /*. Update shop keepers .*/
    for(shop = 0; shop <= top_shop; shop++) {
      if(SHOP_KEEPER(shop) >= new_mob_num)
        SHOP_KEEPER(shop)++;
    }
    /*. Update keepers in shops being edited .*/
    for(dsc = descriptor_list; dsc; dsc = dsc->next)
      if(dsc->connected == CON_SEDIT)
        if(S_KEEPER(OLC_SHOP(dsc)) >= new_mob_num)
          S_KEEPER(OLC_SHOP(dsc))++;
  }
  olc_add_to_save_list(zone_table[OLC_ZNUM(d)].number, OLC_SAVE_MOB);
}


/*-------------------------------------------------------------------*/
/*. Save ALL mobiles for a zone to their .mob file, mobs are all 
    saved in Extended format, regardless of whether they have any
    extended fields.  Thanks to Samedi for ideas on this bit of code.*/

void medit_save_to_disk(struct descriptor_data *d)
{ 
  int  zone, top;
  FILE *mob_file;
  char fname[64],tname[64];
  zone = zone_table[OLC_ZNUM(d)].number; 
  top = zone_table[OLC_ZNUM(d)].top; 

  sprintf(tname, "%s/%i.tmp", MOB_PREFIX, zone);
  sprintf(fname, "%s/%i.mob", MOB_PREFIX, zone);

  if(!(mob_file = fopen(tname, "w"))) { 
    mudlog("SYSERR: OLC: Cannot open mob file!", BRF, LVL_BUILDER, TRUE);
    return;
  }

  mobiles_save_zone_to_file((zone_table[OLC_ZNUM(d)].number * 100),zone_table[OLC_ZNUM(d)].top,mob_file);
  fclose(mob_file);

  remove(fname);        /* Remove old file */
  rename(tname,fname);  /* Rename tmp-file as mob file */

  olc_remove_from_save_list(zone_table[OLC_ZNUM(d)].number, OLC_SAVE_MOB);
}

/**************************************************************************
 Menu functions 
 **************************************************************************/

/*. Display actions that can be added. .*/
void medit_disp_addaction(struct descriptor_data *d)
{
  int i, j;
  char temp[MAX_INPUT_LENGTH];
  int after_pause = 0;
  struct mob_action_list *dum;


  j = GET_MAINLIST(d->character)->main;
  

  for (dum = GET_MAINLIST(d->character)->actions; dum; dum = dum->next)
    if (dum->action == MPA_PAUSE) {
      after_pause = TRUE;
      break;
    }

  send_to_char("[H[J", d->character);
  sprintf(buf,"         Select action:\r\n=========================================\r\n");
  
  for (i = 1; *actions[i] != '*'; i ++){
    if (((j == MPC_TIME) || (j == MPC_MOBENTRY) || (j == MPC_RANDOM) ||
    (j == MPC_MOBLEAVE) || (j == MPC_TICK) || (after_pause)) &&
    ((i == MPA_SAYTO) || (i == MPA_ACTIONTO) || (i == MPA_DOING) ||
    (i == MPA_FORCE) || (i == MPA_GIVE) || (i == MPA_CAST)))
      sprintf(temp, "Not available.");
    else
      sprintf(temp, actions[i]);
    sprintf(buf,"%s\\c02%2d\\c00) %-32s", buf, i, temp);
    if (!(i%2))
      sprintf(buf,"%s\r\n", buf);
  }
  sprintf(buf,"%s\r\n\r\nEnter number: ", buf);
  send_to_char(buf, d->character);
}

/*-------------------------------------------------------------------*/
/*. Display side terms that can be added. .*/
void medit_disp_addside(struct descriptor_data *d)
{
  int i, j;
  char temp[MAX_INPUT_LENGTH];
  j = GET_MAINLIST(d->character)->main;
  
  send_to_char("[H[J", d->character);
  sprintf(buf,"         Select side term:\r\n=========================================\r\n");
  for (i = 1; *sideterms[i] != '*'; i ++){
    if (((j == MPC_TIME) || (j == MPC_MOBENTRY) || (j == MPC_RANDOM) ||
    (j == MPC_MOBLEAVE) || (j == MPC_TICK)) && (i != MPC_MODE))
      sprintf(temp,"\\c06Not available.\\c00");
    else
      sprintf(temp, sideterms[i]);
    sprintf(buf,"%s\\c02%2d\\c00) %-32s", buf, i, temp);
    if (!(i%2))
      sprintf(buf,"%s\r\n", buf);
  }
  sprintf(buf,"%s\r\n\r\nEnter number: ", buf);
  send_to_char(buf, d->character);
}

/*-------------------------------------------------------------------*/
/*. Display current sideterm list .*/

void medit_disp_sideterms(struct descriptor_data *d)
{
  struct side_term_list *list;
  char temp[MAX_INPUT_LENGTH], temp2[15];
  int i = 1;
  
  send_to_char("[H[J", d->character);
  send_to_char("Current side terms in the list:\r\n", d->character);
  send_to_char("===============================\r\n", d->character);
  if (!GET_MAINLIST(d->character)->sideterms)
    send_to_char(" <none>", d->character);
  else
  for (list = GET_MAINLIST(d->character)->sideterms; list; list = list->next, i++)
  {
    if (list->not)
      sprintf(temp2, " \\c01(Not)\\c00 ");
    else
      sprintf(temp2, " ");
    if ((list->number == MPC_SEX) || (list->number == MPC_LEVEL) ||
        (list->number == MPC_RACE) || (list->number == MPC_GUILD) ||
        (list->number == MPC_WEARINGN) || (list->number == MPC_CARRYINGN) ||
    (MPC_GRANKIS == list->number) || (MPC_GRANKLESS == list->number) || 
    (MPC_GRANKMORE == list->number))
      sprintf(temp," \\c02%2d\\c00)%s %s\"%s\"\r\n", i, temp2, sideterms[list->number], list->argument);
    else if ((list->number == MPC_CARRYING) || (list->number == MPC_WEARING) 
             || (list->number == MPC_MODE) || (list->number == MPC_QUESTED))
      sprintf(temp," \\c02%2d\\c00)%s%s %d\r\n", i, temp2, sideterms[list->number], list->int_arg);
    else 
      sprintf(temp, " \\c02%2d\\c00)%s%s\r\n", i, temp2, sideterms[list->number]);
    send_to_char(temp, d->character);
  }   
  send_to_char("\r\n\r\n\r\n(\\c03A\\c00)dd, (\\c03R\\c00)emove, (\\c03E\\c00)xit, (\\c03C\\c00)lear list\r\nEnter your choice:", d->character);
}

/*-------------------------------------------------------------------*/
/*. Display current actionlist .*/
void medit_disp_actions(struct descriptor_data *d)
{
  struct mob_action_list *list;
  char temp[MAX_INPUT_LENGTH];
  int i = 1;
  send_to_char("[H[J", d->character);
  sprintf(buf,"Current actions are in the list:\r\n================================\r\n");
  GET_ACTLIST(d->character) = GET_MAINLIST(d->character)->actions;
  if (GET_ACTLIST(d->character)) {
    for (list = GET_ACTLIST(d->character);  list; list = list->next, i++) {
      if ((list->action == MPA_GIVE) || (list->action == MPA_MOVETO) || 
          (list->action == MPA_MODE) || (list->action == MPA_PAUSE)) 
        sprintf(temp, "%d", list->intarg);
      else if ((list->action != MPA_STOP))  
        sprintf(temp, "\"%s\"", list->argument);
      else
        sprintf(temp, " ");
      sprintf(buf, "%s \\c02%2d\\c00 - %s %s\r\n", buf, i, actions[list->action], temp);
    }
    send_to_char(buf, d->character);
  }
  else {
    send_to_char(buf, d->character);
    send_to_char("     <none>\r\n", d->character);
  }
  send_to_char("\r\n\r\n\r\n(\\c03A\\c00)dd, (\\c03R\\c00)emove, (\\c03E\\c00)xit, (\\c03C\\c00)lear list\r\nEnter your choice:", d->character);
}

void medit_disp_mainterms(struct descriptor_data *d)
{
  int i;
  
  send_to_char("[H[J", d->character);
  sprintf(buf,"         Select main term:\r\n=========================================\r\n");
  for (i = 1; *mainterms[i] != '*'; i ++)
    sprintf(buf,"%s\\c02%2d\\c00) %s\r\n", buf, i, mainterms[i]);
  sprintf(buf,"%s\r\n\r\nEnter number:", buf);
  send_to_char(buf, d->character);
}

/*-------------------------------------------------------------------*/
/*. Display current mob program .*/

void medit_disp_mob_program(struct descriptor_data *d)
{
  char temp[MAX_INPUT_LENGTH], temp2[15];
  int imain = 0, side = 0, action = 0, not = 0, side_int_arg = 0, 
  main_int_arg = 0, act_int_arg = 0;
  char *sidearg = NULL, *mainarg = NULL, *actarg = NULL;
  struct side_term_list *this_side = NULL;
  struct mob_prog_list *mpl =  NULL, *list = NULL;
  struct mob_action_list  *this_act = NULL;
  
  list = GET_MAINLIST(d->character);
  
  if (list) {    
    mpl = list;
    imain = mpl->main;
    mainarg = mpl->argument;
    main_int_arg = mpl->intarg;
    if ((imain == MPC_SAY) || (imain == MPC_SAYTO) || (imain == MPC_ACTION) ||
    (imain == MPC_ACTIONTO) || (imain == MPC_TELL))
      sprintf(temp,"\"%s\"", mainarg);
    else if ((imain == MPC_GIVE) || (imain == MPC_TIME) || (imain == MPC_RANDOM))
      sprintf(temp,"%d",main_int_arg);
    else 
      sprintf(temp," ");   
   
  }
  else {
    imain = -1;
    strcpy(temp,"<no arg>");
  }
  send_to_char("[H[J", d->character);
  
  sprintf(buf, "\\c02Main term:\\c00 %s%s\r\n",(mpl)? mainterms[imain]: "\\c01<none>\\c00 ", temp);
  sprintf(buf, "%s \\c02-----Sideterms-----\\c00\r\n", buf);
  if (mpl->sideterms)
    for (this_side = mpl->sideterms; this_side; this_side = this_side->next) {
      side = this_side->number;
      sidearg = this_side->argument;
      not = this_side->not;
      side_int_arg = this_side->int_arg;
      if (not)
        sprintf(temp2, " (Not) ");
      else 
        sprintf(temp2, " ");
      if ((side == MPC_SEX) || (side == MPC_RACE) || (side == MPC_GUILD) ||
      (side == MPC_LEVEL) || (side == MPC_WEARINGN) || (side == MPC_CARRYINGN)) 
        sprintf(temp, "\"%s\"", sidearg);
      else if ((side == MPC_CARRYING) || (side == MPC_WEARING) || (side == MPC_MODE)) 
        sprintf(temp, "%d", side_int_arg);
      else sprintf(temp, " ");
      sprintf(buf, "%s%s%s%s\r\n", buf, temp2, sideterms[side], temp);
    }
  else
    sprintf(buf, "%s   <none>\r\n", buf);
    
  sprintf(buf, "%s\r\n \\c02-----Actions-----\\c00 \r\n", buf);
  if (mpl->actions)
    for (this_act = mpl->actions; this_act; this_act = this_act->next) {
      action = this_act->action;
      actarg = this_act->argument;
      act_int_arg = this_act->intarg;
      if ((action == MPA_GIVE) || (action == MPA_MOVETO) || 
          (action == MPA_MODE) || (action == MPA_PAUSE)) 
        sprintf(temp, " %d", act_int_arg);
      else if ((action != MPA_STOP))  
        sprintf(temp,  "\"%s\"", actarg);
      else
        sprintf(temp, " ");
        
      sprintf(buf, "%s %s %s\r\n", buf, actions[action], temp);
    }
  else
    sprintf(buf, "%s   \\c08<none>\\c00\r\n", buf);
  send_to_char(buf, d->character);
  
  send_to_char("(\\c03D\\c00)elete (\\c03E\\c00)xit (\\c03M\\c00)ainterms (\\c03S\\c00)ideterms (\\c03A\\c00)ctions (\\c03N\\c00)ew ", d->character);
  if (mpl->next)
    send_to_char(" Ne(\\c03x\\c00)t", d->character);
  if (mpl != GET_MOBPROG(OLC_MOB(d)))
    send_to_char(" (\\c03P\\c00)revious", d->character);
  send_to_char("\r\nEnter choice:", d->character);   
}

/*-------------------------------------------------------------------*/
/*. Display positions (sitting, standing etc) .*/

void medit_disp_positions(struct descriptor_data *d)
{ int i;

  get_char_cols(d->character);

  send_to_char("[H[J", d->character);
  for (i = 0; *position_types[i] != '\n'; i++)
  {  sprintf(buf, "%s\\c02%2d\\c00%s) %s\r\n", grn, i, nrm, position_types[i]);
     send_to_char(buf, d->character);
  }
  send_to_char("Enter position number : ", d->character);
}


/*-------------------------------------------------------------------*/
/* Display skills */

void medit_disp_skill_flags(struct descriptor_data *d)
{ int i, columns = 0;
  
  get_char_cols(d->character);
  send_to_char("[H[J", d->character);
  for (i = 0; i < NUM_MOB_SKILLS; i++)
  {  sprintf(buf, "%s%2d%s) %-20.20s  ",
    grn, i+1, nrm, mobskill_bits[i]
     );
     if(!(++columns % 2))
       strcat(buf, "\r\n");
     send_to_char(buf, d->character);
  }
  sprintbit(MOB_SKILLS(OLC_MOB(d)), mobskill_bits, buf1);
  sprintf(buf, "\r\n"
    "Current skills : %s%s%s\r\n"
    "Enter mob skills (0 to quit) : ",
        cyn, buf1, nrm
  );
  send_to_char(buf, d->character);
}



/*-------------------------------------------------------------------*/
/*. Display sex (Oooh-err).*/

void medit_disp_sex(struct descriptor_data *d)
{ int i;

  get_char_cols(d->character);

  send_to_char("[H[J", d->character);
  for (i = 0; i < NUM_GENDERS; i++)
  {  sprintf(buf, "%s%2d%s) %s\r\n", grn, i, nrm, genders[i]);
     send_to_char(buf, d->character);
  }
  send_to_char("Enter gender number : ", d->character);
}

/*-------------------------------------------------------------------*/
/*. Display attack types menu .*/

void medit_disp_attack_types(struct descriptor_data *d)
{ int i;
  
  get_char_cols(d->character);
  send_to_char("[H[J", d->character);
  for (i = 0; i < NUM_ATTACK_TYPES; i++)
  {  sprintf(buf, "%s%2d%s) %s\r\n", 
    grn, i, nrm, attack_hit_text[i].singular
     );
     send_to_char(buf, d->character);
  }
  send_to_char("Enter attack type : ", d->character);
}
 

/*-------------------------------------------------------------------*/
/*. Display mob-flags menu .*/

void medit_disp_mob_flags(struct descriptor_data *d)
{ int i, columns = 0;
  
  get_char_cols(d->character);
  send_to_char("[H[J", d->character);
  for (i = 0; i < NUM_MOB_FLAGS; i++)
  {  sprintf(buf, "%s%2d%s) %-20.20s  ",
    grn, i+1, nrm, action_bits[i]
     );
     if(!(++columns % 2))
       strcat(buf, "\r\n");
     send_to_char(buf, d->character);
  }
  sprintbit(MOB_FLAGS(OLC_MOB(d)), action_bits, buf1);
  sprintf(buf, "\r\n"
    "Current flags : %s%s%s\r\n"
    "Enter mob flags (0 to quit) : ",
        cyn, buf1, nrm
  );
  send_to_char(buf, d->character);
}

/*-------------------------------------------------------------------*/
/*. Display aff-flags menu .*/

void medit_disp_aff_flags(struct descriptor_data *d)
{ int i, columns = 0;
  
  get_char_cols(d->character);
  send_to_char("[H[J", d->character);
  for (i = 0; i < NUM_AFF_FLAGS; i++)
  {  sprintf(buf, "%s%2d%s) %-20.20s  ", 
    grn, i+1, nrm, affected_bits[i]
     );
     if(!(++columns % 2))
       strcat(buf, "\r\n");
     send_to_char(buf, d->character);
  }
  sprintbit(AFF_FLAGS(OLC_MOB(d)), affected_bits, buf1);
  sprintf(buf, "\r\n"
    "Current flags   : %s%s%s\r\n"
    "Enter aff flags (0 to quit) : ",
        cyn, buf1, nrm
  );
  send_to_char(buf, d->character);
}
  
/*-------------------------------------------------------------------*/
/*. Display main menu .*/

void medit_disp_menu(struct descriptor_data * d)
{ struct char_data *mob;
  char buf3[MAX_INPUT_LENGTH];

  mob = OLC_MOB(d);
  get_char_cols(d->character);

  sprintf(buf, "[H[J"
    "-- Mob Number:  [%s%ld%s]\r\n"
    "%s1%s) Sex: %s%-7.7s%s	         %s2%s) Alias: %s%s\r\n"
        "%s3%s) S-Desc: %s%s\r\n"
    "%s4%s) L-Desc:-\r\n%s%s" 
    "%s5%s) D-Desc:-\r\n%s%s"
    "%s6%s) Level:        [%s%4ld%s]\r\n" 
        "%s8%s) Hitroll:      [%s%4ld%s],   %s9%s) Damroll:      [%s%4ld%s]\r\n"
        "%sA%s) NumDamDice:   [%s%4d%s],   %sB%s) SizeDamDice:  [%s%4d%s]\r\n"
    "%sC%s) Num HP Dice:  [%s%4d%s],   %sD%s) Size HP Dice: [%s%4d%s],  %sE%s) HP Bonus: [%s%5d%s]\r\n"
    "%sF%s) Num MANA dice [%s%4d%s],   %sG%s) Size MANA dice: [%s%4d%s], %sH%s) MANA bonus: [%s%5d%s]\r\n"
    "%sI%s) Num MOVE dice [%s%4d%s],   %sJ%s) Size MOVE dice: [%s%4d%s], %sK%s) MOVE bonus: [%s%5d%s]\r\n"
    "%sL%s) Armor Class:  [%s%4d%s],   %sM%s) Exp:     [%s%9ld%s],  %sN%s) Money:  [%s%8ld%s]\r\n",
    cyn, OLC_NUM(d), nrm,
    grn, nrm, yel, genders[(int)GET_SEX(mob)], nrm,
    grn, nrm, yel, GET_ALIAS(mob),
    grn, nrm, yel, GET_SDESC(mob),
    grn, nrm, yel, GET_LDESC(mob),
    grn, nrm, yel, GET_MDESC(mob),
    grn, nrm, cyn, GET_LEVEL(mob), nrm,
    grn, nrm, cyn, GET_HITROLL(mob), nrm,
    grn, nrm, cyn, GET_DAMROLL(mob), nrm,
    grn, nrm, cyn, GET_NDD(mob), nrm,
    grn, nrm, cyn, GET_SDD(mob), nrm,

    grn, nrm, cyn, mob->mob_specials.hpnodice, nrm,
    grn, nrm, cyn, mob->mob_specials.hpsizedice, nrm,
    grn, nrm, cyn, mob->mob_specials.hpbonus, nrm,

    grn, nrm, cyn, mob->mob_specials.mananodice, nrm,
    grn, nrm, cyn, mob->mob_specials.manasizedice, nrm,
    grn, nrm, cyn, mob->mob_specials.manabonus, nrm,

    grn, nrm, cyn, mob->mob_specials.movenodice, nrm,
    grn, nrm, cyn, mob->mob_specials.movesizedice, nrm,
    grn, nrm, cyn, mob->mob_specials.movebonus, nrm,

    grn, nrm, cyn, GET_AC(mob), nrm, 
        /*. Gold & Exp are longs in my mud, ignore any warnings .*/
    grn, nrm, cyn, (long)GET_EXP(mob), nrm,
    grn, nrm, cyn, (long)GET_GOLD(mob), nrm
  );
  send_to_char(buf, d->character);

  sprintbit(MOB_FLAGS(mob), action_bits, buf1);
  sprintbit(AFF_FLAGS(mob), affected_bits, buf2);
  sprintbit(MOB_SKILLS(mob), mobskill_bits, buf3);
  sprintf(buf,
    "%sO%s) Position   : %s%s\r\n"
        "%sP%s) Default    : %s%s\r\n"
        "%sR%s) Attack     : %s%s\r\n"
        "%sS%s) NPC Flags  : %s%s\r\n"
        "%sT%s) AFF Flags  : %s%s\r\n"
        "%sU%s) SKILL Flags: %s%s\r\n"
        "%sV%s) Mob programs\r\n"
        "%sW%s) Mob timer\r\n"
        "%sX%s) Script     : %s%s\r\n"
        "%sQ%s) Quit\r\n"
        "Enter choice : ",

    grn, nrm, yel, position_types[(int)GET_POS(mob)],
    grn, nrm, yel, position_types[(int)GET_DEFAULT_POS(mob)],
        grn, nrm, yel, attack_hit_text[GET_ATTACK(mob)].singular,
    grn, nrm, cyn, buf1, 
    grn, nrm, cyn, buf2,
    grn, nrm, cyn, buf3,
    grn, nrm, 
        grn, nrm,
        grn, nrm, cyn, mob->proto_script?"Set.":"Not Set.",
        grn, nrm
  );
  send_to_char(buf, d->character);

  OLC_MODE(d) = MEDIT_MAIN_MENU;
}

/**************************************************************************
  The GARGANTAUN event handler
 **************************************************************************/

void medit_parse(struct descriptor_data * d, char *arg)
{ 
  int i, j, after_pause = 0;
  struct mob_prog_list *present = NULL, *present_next = NULL;
  struct mob_action_list *dum = NULL;
  char temp[MAX_INPUT_LENGTH], *strng;
  
  if (OLC_MODE(d) > MEDIT_NUMERICAL_RESPONSE)
  { if(!*arg || (!isdigit(arg[0]) && ((*arg == '-') && (!isdigit(arg[1])))))
    { send_to_char("Field must be numerical, try again : ", d->character);
      return;
    }
  }

  switch (OLC_MODE(d)) 
  {
/*-------------------------------------------------------------------*/
  case MEDIT_CONFIRM_SAVESTRING:
    /*. Ensure mob has MOB_ISNPC set or things will go pair shaped .*/
    SET_BIT(MOB_FLAGS(OLC_MOB(d)), MOB_ISNPC);
    switch (*arg) {
    case 'y':
    case 'Y':
      /*. Save the mob in memory and to disk  .*/
      send_to_char("Saving mobile to memory.\r\n", d->character);
      medit_save_internally(d);
      sprintf(buf, "OLC: %s edits mob %ld", GET_DISGUISED(d->character) ?
        GET_RNAME(d->character) : GET_NAME(d->character),
               OLC_NUM(d));	      
      mudlog(buf, CMP, LVL_BUILDER, TRUE);
      cleanup_olc(d, CLEANUP_ALL);
      return;
    case 'n':
    case 'N':
      cleanup_olc(d, CLEANUP_ALL);
      return;
    default:
      send_to_char("Invalid choice!\r\n", d->character);
      send_to_char("Do you wish to save the mobile? : ", d->character);
      return;
    }
    break;

/*-------------------------------------------------------------------*/
  case MEDIT_MAIN_MENU:
    i = 0;
    switch (*arg) 
    { case 'q':
      case 'Q':
        if (OLC_VAL(d)) /*. Anything been changed? .*/
        { send_to_char("Do you wish to save the changes to the mobile? (y/n) : ", d->character);
          OLC_MODE(d) = MEDIT_CONFIRM_SAVESTRING;
        } else
          cleanup_olc(d, CLEANUP_ALL);
        return;
      case '1':
        OLC_MODE(d) = MEDIT_SEX;
        medit_disp_sex(d);
        return;
      case '2':
        OLC_MODE(d) = MEDIT_ALIAS;
        i--;
        break;
      case '3':
        OLC_MODE(d) = MEDIT_S_DESC;
        i--;
        break;
      case '4':
        OLC_MODE(d) = MEDIT_L_DESC;
        i--;
        break;
      case '5':
        send_to_char("Enter new mob description:(/s saves /h for help)\r\n", d->character);
send_to_char("Please format your desc to less than 75 characters per line.\r\n", d->character);
send_to_char("This will make your desc easier to read for those with smaller screens\r\n",d->character);
send_to_char("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n",d->character);

        /*. Pass control to modify.c .*/
        OLC_MODE(d) = MEDIT_D_DESC;
         d->backstr = NULL;
         if (OLC_MOB(d)->player.description) {
            SEND_TO_Q(OLC_MOB(d)->player.description, d);
            d->backstr = str_dup(OLC_MOB(d)->player.description);
         }
        d->str = &OLC_MOB(d)->player.description;
        d->max_str = MAX_MOB_DESC;
        d->mail_to[0] = 0;
    d->character->desc->current_text_line = count_newlines(d->str) + 1;
        OLC_VAL(d) = 1;
        return;
      case '6':
        OLC_MODE(d) = MEDIT_LEVEL;
        i++;
        break;
      case '7':
    medit_disp_menu(d);
        break;
      case '8':
        OLC_MODE(d) = MEDIT_HITROLL;
        i++;
        break;
      case '9':
        OLC_MODE(d) = MEDIT_DAMROLL;
        i++;
        break;
      case 'a':
      case 'A':
        OLC_MODE(d) = MEDIT_NDD;
        i++;
        break;
      case 'b':
      case 'B':
        OLC_MODE(d) = MEDIT_SDD;
        i++;
        break;
      case 'c':
      case 'C':
        OLC_MODE(d) = MEDIT_NUM_HP_DICE;
        i++;
        break;
      case 'd':
      case 'D':
        OLC_MODE(d) = MEDIT_SIZE_HP_DICE;
        i++;
        break;
      case 'e':
      case 'E':
        OLC_MODE(d) = MEDIT_ADD_HP;
        i++;
        break;
      case 'f':
      case 'F':
        OLC_MODE(d) = MEDIT_NUM_MANA_DICE;
        i++;
        break;
      case 'g':
      case 'G':
        OLC_MODE(d) = MEDIT_SIZE_MANA_DICE;
        i++;
        break;
      case 'h':
      case 'H':
        OLC_MODE(d) = MEDIT_ADD_MANA;
        i++;
        break;
      case 'i':
      case 'I':
        OLC_MODE(d) = MEDIT_NUM_MOVE_DICE;
        i++;
        break;
      case 'j':
      case 'J':
        OLC_MODE(d) = MEDIT_SIZE_MOVE_DICE;
        i++;
        break;
      case 'k':
      case 'K':
        OLC_MODE(d) = MEDIT_ADD_MOVE;
        i++;
        break;
      case 'l':
      case 'L':
        OLC_MODE(d) = MEDIT_AC;
        i++;
        break;
      case 'm':
      case 'M':
        OLC_MODE(d) = MEDIT_EXP;
        i++;
        break;
      case 'n':
      case 'N':
        OLC_MODE(d) = MEDIT_GOLD;
        i++;
        break;
      case 'o':
      case 'O':
        OLC_MODE(d) = MEDIT_POS;
        medit_disp_positions(d);
        return;
      case 'p':
      case 'P':
        OLC_MODE(d) = MEDIT_DEFAULT_POS;
        medit_disp_positions(d);
        return;
      case 'r':
      case 'R':
        OLC_MODE(d) = MEDIT_ATTACK;
        medit_disp_attack_types(d);
        return;
      case 's':
      case 'S':
        OLC_MODE(d) = MEDIT_NPC_FLAGS;
        medit_disp_mob_flags(d);
        return;
      case 't':
      case 'T':
        OLC_MODE(d) = MEDIT_AFF_FLAGS;
        medit_disp_aff_flags(d);
        return;
      case 'u':
      case 'U':
        OLC_MODE(d) = MEDIT_SKILLS;
        medit_disp_skill_flags(d);
        return;
      case 'v':
      case 'V':
        OLC_MODE(d) = MEDIT_MOBPROG;
        if (!GET_MOBPROG(OLC_MOB(d)))
          CREATE(GET_MOBPROG(OLC_MOB(d)), struct mob_prog_list, 1);
        GET_MAINLIST(d->character) = GET_MOBPROG(OLC_MOB(d));
        medit_disp_mob_program(d);
        return;
      case 'w':         
      case 'W':
        OLC_MODE(d) = MEDIT_TIMER;
        i++;
      break;
      case 'x':
      case 'X':
        OLC_SCRIPT_EDIT_MODE(d) = SCRIPT_MAIN_MENU;
        dg_script_menu(d);
        return;
      default:
        medit_disp_menu(d);
    return;
    }
    if (i==1)
    {  send_to_char("\r\nEnter new value : ", d->character);
       return;
    }
    if (i==-1)
    {  send_to_char("\r\nEnter new text :\r\n| ", d->character);
       return;
    }
    break; 

/*-------------------------------------------------------------------*/
  case OLC_SCRIPT_EDIT:
    if (dg_script_edit_parse(d, arg)) return;
    break;
/*-------------------------------------------------------------------*/
  case MEDIT_ALIAS:
    if(GET_ALIAS(OLC_MOB(d)))
      free(GET_ALIAS(OLC_MOB(d)));
    GET_ALIAS(OLC_MOB(d)) = str_dup(arg); 
    break;
/*-------------------------------------------------------------------*/
  case MEDIT_S_DESC:
    if(GET_SDESC(OLC_MOB(d)))
      free(GET_SDESC(OLC_MOB(d)));
    GET_SDESC(OLC_MOB(d)) = str_dup(arg); 
    break;
/*-------------------------------------------------------------------*/
  case MEDIT_L_DESC:
    if(GET_LDESC(OLC_MOB(d)))
      free(GET_LDESC(OLC_MOB(d)));
    strcpy(buf, arg);
    strcat(buf, "\r\n");
    GET_LDESC(OLC_MOB(d)) = str_dup(buf); 
    break;
/*-------------------------------------------------------------------*/
  case MEDIT_D_DESC:
    /*. We should never get here .*/
    cleanup_olc(d, CLEANUP_ALL);
    mudlog("SYSERR: OLC: medit_parse(): Reached D_DESC case!",BRF,LVL_BUILDER,TRUE);
    break;
/*-------------------------------------------------------------------*/
  case MEDIT_NPC_FLAGS:
    i = atoi(arg);
    if (i==0)
      break;
    if (!((i < 0) || (i > NUM_MOB_FLAGS)))
    { i = 1 << (i - 1);
      if (IS_SET(MOB_FLAGS(OLC_MOB(d)), i))
        REMOVE_BIT(MOB_FLAGS(OLC_MOB(d)), i);
      else
        SET_BIT(MOB_FLAGS(OLC_MOB(d)), i);
    }
    medit_disp_mob_flags(d);
    return;
/*-------------------------------------------------------------------*/
  case MEDIT_AFF_FLAGS:
    i = atoi(arg);
    if (i==0)
      break;
    if (!((i < 0) || (i > NUM_AFF_FLAGS)))
    { i = 1 << (i - 1);
      if (IS_SET(AFF_FLAGS(OLC_MOB(d)), i))
        REMOVE_BIT(AFF_FLAGS(OLC_MOB(d)), i);
      else
        SET_BIT(AFF_FLAGS(OLC_MOB(d)), i);
    }
    medit_disp_aff_flags(d);
    return;
/*-------------------------------------------------------------------*/
  case MEDIT_SKILLS:
       i = atoi(arg);
    if (i==0)
      break;
    if (!((i < 0) || (i > NUM_MOB_SKILLS)))
    { i = 1 << (i - 1);
      if (IS_SET(MOB_SKILLS(OLC_MOB(d)), i))
        REMOVE_BIT(MOB_SKILLS(OLC_MOB(d)), i);
      else
        SET_BIT(MOB_SKILLS(OLC_MOB(d)), i);
    }
    medit_disp_skill_flags(d);
    return;
/*-------------------------------------------------------------------*/
  case MEDIT_MOBPROG:
    switch (*arg) {
      case 'a':
      case 'A':
        OLC_MODE(d) = MEDIT_MP_ACTIONS;
        GET_ACTLIST(d->character) = GET_MAINLIST(d->character)->actions;        
        medit_disp_actions(d);
        return;
      case 'd':
      case 'D':
        delete_mobprog(GET_MAINLIST(d->character), d);
        if (!GET_MOBPROG(OLC_MOB(d))) {         /* Must never have an empty mobproglist */
          CREATE(GET_MOBPROG(OLC_MOB(d)), struct mob_prog_list, 1);
          GET_MOBPROG(OLC_MOB(d))->next = NULL;
          GET_MOBPROG(OLC_MOB(d))->actions = NULL;
          GET_MOBPROG(OLC_MOB(d))->sideterms = NULL;
        }
        GET_MAINLIST(d->character) = GET_MOBPROG(OLC_MOB(d));
        medit_disp_mob_program(d);
        return;
      case 'e':
      case 'E':
        for (present = GET_MOBPROG(OLC_MOB(d)); present; present = present_next) {
          present_next = present->next;
          if (!present->actions || !present->main) 
            delete_mobprog(present, d);
        }
        OLC_MODE(d) = MEDIT_MAIN_MENU;
        medit_disp_menu(d);
        GET_MAINLIST(d->character) = NULL;
        GET_ACTLIST(d->character) = NULL;
        GET_SIDELIST(d->character) = NULL;
        OLC_VAL(d) = 1;
        break;  
      case 'm':
      case 'M':
        OLC_MODE(d) = MEDIT_MP_MAINTERMS;
        medit_clear_actions(d);
        medit_clear_sideterms(d);
        medit_disp_mainterms(d);
        return;
      case 'n':
      case 'N':
        GET_MAINLIST(d->character) = GET_MOBPROG(OLC_MOB(d));
        i = 0;
        while (GET_MAINLIST(d->character)->next) {  /* Step to last main program */
          GET_MAINLIST(d->character) = GET_MAINLIST(d->character)->next;  
          i++;
          if (i >= 29) {
             send_to_char("Program list is full, sorry.\r\n", d->character);
             return;
          }
        }
        CREATE(GET_MAINLIST(d->character)->next, struct mob_prog_list, 1); /* Create new last */
        GET_MAINLIST(d->character) = GET_MAINLIST(d->character)->next; /* Go to new */
        GET_MAINLIST(d->character)->actions = NULL;
        GET_MAINLIST(d->character)->sideterms = NULL;
        medit_disp_mob_program(d);
        return;
      case 'p':
      case 'P':
        if (GET_MOBPROG(OLC_MOB(d)) != GET_MAINLIST(d->character)) {
          for (present = GET_MOBPROG(OLC_MOB(d)); 
            present->next != GET_MAINLIST(d->character);
            present = present->next);
          GET_MAINLIST(d->character) = present;
        } 
        else 
          send_to_char("Already at beginning of list.\r\n", d->character);
        medit_disp_mob_program(d);        
        return;
      case 's':
      case 'S':
        i = GET_MAINLIST(d->character)->main;
        GET_SIDELIST(d->character) = GET_MAINLIST(d->character)->sideterms;
        OLC_MODE(d) = MEDIT_MP_SIDETERMS;
        medit_disp_sideterms(d);
        return;
      case 'x':
      case 'X':
        if (GET_MAINLIST(d->character)->next) {
          GET_MAINLIST(d->character) = GET_MAINLIST(d->character)->next;
          GET_SIDELIST(d->character) = GET_MAINLIST(d->character)->sideterms;
          GET_ACTLIST(d->character) = GET_MAINLIST(d->character)->actions;
          medit_disp_mob_program(d);
        } 
        else
          send_to_char("Already at end of list.\r\n", d->character);
      return;
      default:
        send_to_char("Wrong command, try again: ", d->character);
      return;      
    }     
  return;        
/*-------------------------------------------------------------------*/
  case MEDIT_MP_MAINTERMS:
    i = atoi(arg);
    if (!i) {
      send_to_char("Invalid number, try again: ", d->character);
      return;
    }
    GET_MAINLIST(d->character)->main = MAX(1, MIN(NUM_MAINTERMS, i));
    if ((i == MPC_TIME) || (i == MPC_SAY) || (i == MPC_SAYTO) ||
        (i == MPC_ACTION) || (i == MPC_ACTIONTO) || (i == MPC_GIVE) ||
        (i == MPC_RANDOM) || (i == MPC_TELL)) {
      OLC_MODE(d) = MEDIT_MP_MAINTERMS2;
      send_to_char("Enter argument for selected main term: ", d->character);
    } else {
      GET_MAINLIST(d->character)->argument = NULL;
      OLC_MODE(d) = MEDIT_MOBPROG;
      medit_disp_mob_program(d);
    }
    return;
/*-------------------------------------------------------------------*/
  case MEDIT_MP_MAINTERMS2:
    i = GET_MAINLIST(d->character)->main;
      
    if ((i == MPC_TIME) || (i == MPC_GIVE) || (i == MPC_RANDOM)) {
      j = atoi(arg);
      if (!j) {
        send_to_char("Invalid number for this main term, please try again: ", d->character);
        return;
      }
      GET_MAINLIST(d->character)->intarg = j;
      GET_MAINLIST(d->character)->argument = strdup(arg);
    } else {
      if (!*arg)  {
        send_to_char("You must have an argument for this main term, try again: ", d->character);
        return;
      }
      GET_MAINLIST(d->character)->intarg = 0;
      GET_MAINLIST(d->character)->argument = strdup(arg);
    }
    OLC_MODE(d) = MEDIT_MOBPROG;
    medit_disp_mob_program(d);
    return;
/*-------------------------------------------------------------------*/
  case MEDIT_MP_ACTIONS:
    switch (*arg) {
      case 'a':
      case 'A':
        OLC_MODE(d) = MEDIT_MP_ADDACTION1;
        GET_ACTLIST(d->character) = GET_MAINLIST(d->character)->actions;
        if (GET_ACTLIST(d->character)) {
          while (GET_ACTLIST(d->character)->next)
            GET_ACTLIST(d->character) = GET_ACTLIST(d->character)->next;
          CREATE(GET_ACTLIST(d->character)->next, struct mob_action_list, 1);
          GET_ACTLIST(d->character) = GET_ACTLIST(d->character)->next;
        } else {
          CREATE(GET_MAINLIST(d->character)->actions, struct mob_action_list, 1);
          GET_ACTLIST(d->character) = GET_MAINLIST(d->character)->actions;
          GET_ACTLIST(d->character)->next = NULL;
        }
        medit_disp_addaction(d);
      break;
      case 'c':
      case 'C':
        medit_clear_actions(d);
        medit_disp_actions(d);
      break;
      case 'e':
      case 'E':
        if (!GET_ACTLIST(d->character)) {
          send_to_char("The list must contain at least one action.\r\n", d->character);
          return;
        }
        OLC_MODE(d) = MEDIT_MOBPROG;
        GET_ACTLIST(d->character) = GET_MOBPROG(OLC_MOB(d))->actions;        
        medit_disp_mob_program(d);
      break;
      case 'r':
      case 'R':
        if (!GET_ACTLIST(d->character)) 
          send_to_char("There are no actions to delete. \r\nEnter command: ", d->character);
        else  {
          one_argument(++arg,temp);
          if (!(i = atoi(temp))) 
            send_to_char("Remove which action?\r\nEnter command: ", d->character);
          else {
            for (GET_ACTLIST(d->character) = GET_MAINLIST(d->character)->actions, j = 1; 
            (j < i) && GET_ACTLIST(d->character); 
            j ++, GET_ACTLIST(d->character) = GET_ACTLIST(d->character)->next);
            if ((i != j) || !GET_ACTLIST(d->character))
              send_to_char("Wrong number entered. \r\nEnter command: ", d->character);
            else {
              remove_action(d);
              medit_disp_actions(d);              
            }
          }
        }
        GET_ACTLIST(d->character) = GET_MAINLIST(d->character)->actions;        
        return;
      default:
        send_to_char("Wrong command, try again: ", d->character);
      return;       
    }
    return;
/*--------------------------------------------------------------------------*/
  case MEDIT_MP_SIDETERMS:
    switch (*arg) {
      case 'a':
      case 'A':
        OLC_MODE(d) = MEDIT_MP_ADDSIDE1;
        GET_SIDELIST(d->character) = GET_MAINLIST(d->character)->sideterms;
        if (GET_SIDELIST(d->character)) {
          while (GET_SIDELIST(d->character)->next)
            GET_SIDELIST(d->character) = GET_SIDELIST(d->character)->next;
          CREATE(GET_SIDELIST(d->character)->next, struct side_term_list, 1);
          GET_SIDELIST(d->character) = GET_SIDELIST(d->character)->next;
        } else {
          CREATE(GET_MAINLIST(d->character)->sideterms, struct side_term_list, 1);
          GET_SIDELIST(d->character) = GET_MAINLIST(d->character)->sideterms;
        }
        medit_disp_addside(d);
        return;
      
      case 'c':
      case 'C':
        medit_clear_sideterms(d);
        medit_disp_sideterms(d);
        return;  
      case 'e':
      case 'E':
        OLC_MODE(d) = MEDIT_MOBPROG;
        GET_SIDELIST(d->character) = GET_MAINLIST(d->character)->sideterms;
        medit_disp_mob_program(d);
        return;
      case 'r':
      case 'R':
        if (!GET_SIDELIST(d->character)) 
          send_to_char("There are no side terms to delete.\r\n Enter command: ", d->character);
        else  {
          one_argument(++arg,temp);
          if (!(i = atoi(temp))) 
            send_to_char("Remove which side term? \r\nEnter command: ", d->character);
          else {
            for (GET_SIDELIST(d->character) = GET_MAINLIST(d->character)->sideterms, j = 1; 
            (j < i) && GET_SIDELIST(d->character); 
            j ++, GET_SIDELIST(d->character) = GET_SIDELIST(d->character)->next);
            if ((i != j) || !GET_SIDELIST(d->character)) 
              send_to_char("Wrong number entered. \r\nEnter command: ", d->character);
            else {
              remove_sideterm(d);
              medit_disp_sideterms(d);              
            }
          }
        }
        GET_SIDELIST(d->character) = GET_MAINLIST(d->character)->sideterms;        
        return;
      default:
        send_to_char("Wrong command, try again: ", d->character);
      return; 
    }
    return;
/*-------------------------------------------------------------------*/
  case MEDIT_MP_ADDACTION1:
    GET_ACTLIST(d->character)->argument = NULL;
    GET_ACTLIST(d->character)->next = NULL;
    i = atoi(arg);
    if (!i || (i > NUM_MOBACTIONS)) {
      send_to_char("Invalid choice, try again:", d->character);
      return;
    }
    for (dum = GET_MAINLIST(d->character)->actions; dum; dum = dum->next)
      if (dum->action == MPA_PAUSE)
        after_pause = TRUE;

    j = GET_MAINLIST(d->character)->main;
    if  (((j == MPC_TIME) || (j == MPC_MOBENTRY) || (j == MPC_MOBLEAVE) ||
        (j == MPC_TICK) || (after_pause)) && 
        ((i == MPA_SAYTO) || (i == MPA_ACTIONTO) || (i == MPA_DOING) ||
        (i == MPA_FORCE) || (i == MPA_GIVE) || (i == MPA_CAST) || (i == MPA_STOP))) {
      send_to_char("That action is not allowed for that main term, try again: ", d->character);
      return;
    }    
    GET_ACTLIST(d->character)->action = i;
    if ((i != MPA_STOP)) {
      OLC_MODE(d) = MEDIT_MP_ADDACTION2;
      send_to_char("Enter argument for selected action:", d->character);
    } else {
      OLC_MODE(d) = MEDIT_MP_ACTIONS;
      medit_disp_actions(d);
    }
    return;
/*-------------------------------------------------------------------*/
  case MEDIT_MP_ADDACTION2:
    if (((GET_ACTLIST(d->character)->action == MPA_MOVETO) ||
        (GET_ACTLIST(d->character)->action == MPA_MODE) ||
        (GET_ACTLIST(d->character)->action == MPA_GIVE) ||
        (GET_ACTLIST(d->character)->action == MPA_PAUSE)) && !atoi(arg)) {
       send_to_char("Argument must be a nonzero number, try again: ", d->character);
      return;
    }
    if ((GET_ACTLIST(d->character)->action != MPA_STOP) && (!*arg)) {
      send_to_char("Argument can't be empty, try again: ", d->character);
      return;
    }   
    if (GET_ACTLIST(d->character)->action != MPA_STOP) {
      if ((GET_ACTLIST(d->character)->action == MPA_MOVETO) ||
         (GET_ACTLIST(d->character)->action == MPA_MODE) ||
         (GET_ACTLIST(d->character)->action == MPA_PAUSE) ||
         (GET_ACTLIST(d->character)->action == MPA_GIVE)) {
        GET_ACTLIST(d->character)->argument = NULL;
        GET_ACTLIST(d->character)->intarg = atoi(arg);
      } else {
        strng = delete_doubledollar(arg);
        if (!emotestring_ok(strng, GET_MAINLIST(d->character))) {
           send_to_char("Arguments in string invalid for emote. Try again.: ", d->character);
          return;
        }
        GET_ACTLIST(d->character)->argument = strdup(strng);
      }
    } else
      GET_ACTLIST(d->character)->argument = NULL;
    GET_ACTLIST(d->character) = GET_MAINLIST(d->character)->actions;
    OLC_MODE(d) = MEDIT_MP_ACTIONS;
    medit_disp_actions(d);
    return;
/*-------------------------------------------------------------------*/
  case MEDIT_MP_ADDSIDE1:
    GET_SIDELIST(d->character)->argument = NULL;
    GET_SIDELIST(d->character)->next = NULL;
    i = atoi(arg);
    if ((i < 1) || (i > NUM_SIDETERMS)) {
      send_to_char("Invalid choice, try again:", d->character);
      return;
    }
    j = GET_MAINLIST(d->character)->main;
    if (((j == MPC_TIME) || (j == MPC_MOBENTRY) || (j == MPC_MOBLEAVE) ||
        (j == MPC_TICK)) && (i != MPC_MODE)) {
      send_to_char("That side term can't be combined with the current main term, try again.\r\nEnter choice: ", d->character);
      return;
    }
    GET_SIDELIST(d->character)->number = i;
    if ((i == MPC_SEX) || (i == MPC_LEVEL) || (i == MPC_CARRYING) || 
        (i == MPC_WEARING) || (i == MPC_RACE) || (i == MPC_GUILD) ||
        (i == MPC_QUESTED) ||
        (i == MPC_MODE) || (i == MPC_WEARINGN) || (i == MPC_CARRYINGN) ||
    (MPC_GRANKIS == i) || (MPC_GRANKLESS == i) || (MPC_GRANKMORE == i)) {     
      OLC_MODE(d) = MEDIT_MP_ADDSIDE2;
      send_to_char("Enter argument for selected side term:", d->character);
    } else {
      OLC_MODE(d) = MEDIT_MP_ADDNOT;
      send_to_char("Is the result true if the test of this term fails? (y/n):", d->character);
    }
    return;
/*-------------------------------------------------------------------*/
  case MEDIT_MP_ADDSIDE2:
    i = GET_SIDELIST(d->character)->number;
    if ((i == MPC_CARRYING) || (i == MPC_WEARING) || 
        (i == MPC_MODE) || (i == MPC_QUESTED)) {
      if (!atoi(arg)) {
        send_to_char("Argument must be a nonzero number, try again: ", d->character);
        return;
      }
      GET_SIDELIST(d->character)->int_arg = atoi(arg);
      GET_SIDELIST(d->character)->argument = strdup(arg);
    } 
    else if ((i == MPC_SEX) || (i == MPC_RACE) || (i == MPC_GUILD) ||
         (i == MPC_WEARINGN) || (i == MPC_LEVEL) || (i == MPC_CARRYINGN) ||
     (MPC_GRANKIS == i) || (MPC_GRANKLESS == i) || (MPC_GRANKMORE == i)) {
      if (!*arg) {
        send_to_char("Argument can't be empty, try again: ", d->character);
        return;
      }
      GET_SIDELIST(d->character)->argument = strdup(arg);
      GET_SIDELIST(d->character)->int_arg = 0;
    }  else {
      GET_SIDELIST(d->character)->argument = NULL;
      GET_SIDELIST(d->character)->int_arg = 0;      
    }
    OLC_MODE(d) = MEDIT_MP_ADDNOT;
    send_to_char("Is the result true if the test of this term fails? (y/n): ", d->character);    
    return;
/*-------------------------------------------------------------------*/
  case MEDIT_MP_ADDNOT:
    switch (*arg) {
      case 'y':
      case 'Y':
        GET_SIDELIST(d->character)->not = TRUE;
        break;
      case 'n':
      case 'N':
        GET_SIDELIST(d->character)->not = FALSE;
        break;
      default:
        send_to_char("Invalid answer, try again: ", d->character);
        return;
      break;
    }
    OLC_MODE(d) = MEDIT_MP_SIDETERMS;
    medit_disp_sideterms(d);
    return;
/*-------------------------------------------------------------------*/
/*. Numerical responses .*/

  case MEDIT_SEX:
    GET_SEX(OLC_MOB(d)) = MAX(0, MIN(NUM_GENDERS -1, atoi(arg)));
    break;

  case MEDIT_HITROLL:
    GET_HITROLL(OLC_MOB(d)) = MAX(0, MIN(200, atoi(arg)));
    break;

  case MEDIT_DAMROLL:
    GET_DAMROLL(OLC_MOB(d)) = MAX(0, MIN(50, atoi(arg)));
    break;

  case MEDIT_NDD:
    GET_NDD(OLC_MOB(d)) = MAX(0, MIN(30, atoi(arg)));
    break;

  case MEDIT_SDD:
    GET_SDD(OLC_MOB(d)) = MAX(0, MIN(127, atoi(arg)));
    break;

  case MEDIT_NUM_HP_DICE:
    /*
    GET_HIT(OLC_MOB(d)) = MAX(0, MIN(30, atoi(arg)));
    */
    OLC_MOB(d)->mob_specials.hpnodice = MAX(0, MIN(30, atoi(arg))); 
    break;

  case MEDIT_SIZE_HP_DICE:
    /* 
    GET_MANA(OLC_MOB(d)) = MAX(0, MIN(1000, atoi(arg)));
    */
    OLC_MOB(d)->mob_specials.hpsizedice = MAX(0, MIN(1000, atoi(arg)));
    break;

  case MEDIT_ADD_HP:
    /* 
    GET_MOVE(OLC_MOB(d)) = MAX(0, MIN(30000, atoi(arg)));
    */
    OLC_MOB(d)->mob_specials.hpbonus = MAX(0, MIN(30000, atoi(arg)));
    break;

  case MEDIT_NUM_MANA_DICE:
    OLC_MOB(d)->mob_specials.mananodice = MAX(0, MIN(30, atoi(arg)));
    break;

  case MEDIT_SIZE_MANA_DICE:
    OLC_MOB(d)->mob_specials.manasizedice = MAX(0, MIN(1000, atoi(arg)));
    break;
  
  case MEDIT_ADD_MANA:
    OLC_MOB(d)->mob_specials.manabonus = MAX(0, MIN(30000, atoi(arg)));
    break;

  case MEDIT_NUM_MOVE_DICE:
    OLC_MOB(d)->mob_specials.movenodice = MAX(0, MIN(30, atoi(arg)));
    break;
  
  case MEDIT_SIZE_MOVE_DICE:
    OLC_MOB(d)->mob_specials.movesizedice = MAX(0, MIN(1000, atoi(arg)));
    break;
    
  case MEDIT_ADD_MOVE:
    OLC_MOB(d)->mob_specials.movebonus = MAX(0, MIN(30000, atoi(arg)));
    break;

  case MEDIT_AC:
    GET_REAL_AC(OLC_MOB(d)) = MAX(-200, MIN(200, atoi(arg)));
    break;

  case MEDIT_EXP:
    GET_EXP(OLC_MOB(d)) = MAX(0, atol(arg));
    break;

  case MEDIT_GOLD:
    GET_GOLD(OLC_MOB(d)) = MAX(0, atol(arg));
    break;

  case MEDIT_POS:
    GET_POS(OLC_MOB(d)) = MAX(0, MIN(NUM_POSITIONS-1, atoi(arg)));
    break;

  case MEDIT_DEFAULT_POS:
    GET_DEFAULT_POS(OLC_MOB(d)) = MAX(0, MIN(NUM_POSITIONS-1, atoi(arg)));
    break;

  case MEDIT_ATTACK:
    GET_ATTACK(OLC_MOB(d)) = MAX(0, MIN(NUM_ATTACK_TYPES-1, atoi(arg)));
    break;

  case MEDIT_LEVEL:
    GET_LEVEL(OLC_MOB(d)) = MAX(1, MIN(200, atoi(arg)));
    break;

  case MEDIT_ALIGNMENT:
    break;

  case MEDIT_TIMER:
    GET_TIMER(OLC_MOB(d)) = MAX(0, MIN(1000, atoi(arg)));
    break;
/*-------------------------------------------------------------------*/
  default:
    /*. We should never get here .*/
    cleanup_olc(d, CLEANUP_ALL);
    mudlog("SYSERR: OLC: medit_parse(): Reached default case!",BRF,LVL_BUILDER,TRUE);
    break;
  }
/*-------------------------------------------------------------------*/
/*. END OF CASE 
    If we get here, we have probably changed something, and now want to
    return to main menu.  Use OLC_VAL as a 'has changed' flag .*/

  OLC_VAL(d) = 1;
  medit_disp_menu(d);
}
/*. End of medit_parse() .*/

