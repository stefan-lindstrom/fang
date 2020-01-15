/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*  _TwyliteMud_ by Rv.                          Based on CircleMud3.0bpl9 *
*    				                                          *
*  OasisOLC - oedit.c 		                                          *
*    				                                          *
*  Copyright 1996 Harvey Gilpin.                                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*. Original author: Levork .*/
#include "fight.h"
#include "interpreter.h"
#include "boards.h"
#include "constants.h"
#include "config.h"
#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "comm.h"
#include "spells.h"
#include "utils.h"
#include "db.h"
#include "boards.h"
#include "shop.h"
#include "olc.h"
#include "xmlObjects.h"
#include "dg_olc.h"

/*------------------------------------------------------------------------*/
/*. Macros .*/

#define S_PRODUCT(s, i) ((s)->producing[ i ] )

/*------------------------------------------------------------------------
  Utility and exported functions
\*------------------------------------------------------------------------*/
void remove_obj_sideterms(struct descriptor_data *d)
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

int obj_emotestring_ok(char *strng, struct obj_prog_list *opl)
{
  int i,j, ok = TRUE, after_pause = FALSE;
  char allowed[30], *ptr;
  struct obj_action_list *oal;

  for (oal = opl->actions; oal; oal = oal->next)
    if (oal->action == OPA_PAUSE)      
      after_pause = TRUE;

  if (!opl)
    return FALSE;
  sprintf(allowed, "p");

  i = opl->main;
  if ((i != OPC_TIME) && (i != OPC_TICK) && (i != OPC_RANDOM) && !after_pause)
    sprintf(allowed,"pMSEN");    

  for (ptr = strng; *ptr && ok; ptr ++)
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
/*-------------------------------------------------------------------*/
/*. Copy obj program to a new obj .*/

void copy_objprog(struct obj_data *from, struct obj_data *to)
{
  struct obj_prog_list *oprogfrom, *oprogto;
  struct side_term_list *stlfrom, *stlto = NULL;
  struct obj_action_list *oalfrom, *oalto = NULL;
 
  if (GET_OBJPROG(from)) 
  {
    CREATE(GET_OBJPROG(to), struct obj_prog_list, 1);
  }
  else {
    GET_OBJPROG(to) = NULL;
    return;
  }
  
  for (oprogfrom = GET_OBJPROG(from), oprogto = GET_OBJPROG(to);
       oprogfrom; oprogfrom = oprogfrom->next) {
    if (!oprogfrom->main || !oprogfrom->actions) /* Don't copy faulty structs */
      continue;
    oprogto->main = oprogfrom->main;
    oprogto->intarg = oprogfrom->intarg;
    if (oprogfrom->argument)
      oprogto->argument = strdup(oprogfrom->argument);
    else 
      oprogto->argument = NULL;
    
    if (oprogfrom->sideterms) {
      CREATE(oprogto->sideterms, struct side_term_list, 1);
      stlto = oprogto->sideterms;
    }
    else
      oprogto->sideterms = NULL;
      
    for (stlfrom = oprogfrom->sideterms; stlfrom; stlfrom = stlfrom->next) {
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
    
    if (oprogfrom->actions) {
      CREATE(oprogto->actions, struct obj_action_list, 1);
      oalto = oprogto->actions;
    }
    else
      oprogto->actions = NULL;
      
    for (oalfrom = oprogfrom->actions; oalfrom; oalfrom = oalfrom->next) {
      oalto->action = oalfrom->action;
      oalto->intarg = oalfrom->intarg;
      if (oalfrom->argument)
        oalto->argument = strdup(oalfrom->argument);
      else
        oalto->argument = NULL;
      if (oalfrom->next) {
        CREATE(oalto->next, struct obj_action_list, 1);
        oalto = oalto->next;
      }
      else
        oalto->next = NULL;
    }
    
    if (oprogfrom->next) {
      CREATE(oprogto->next, struct obj_prog_list, 1);
      oprogto = oprogto->next;
    }
    else
      oprogto->next = NULL;
  }
}

void oedit_setup_new(struct descriptor_data *d)
{
  CREATE (OLC_OBJ(d), struct obj_data, 1);
  clear_object(OLC_OBJ(d));
  OLC_OBJ(d)->name = str_dup("unfinished object");
  OLC_OBJ(d)->description = str_dup("An unfinished object is lying here.");
  OLC_OBJ(d)->short_description = str_dup("an unfinished object");
  GET_OBJ_WEAR(OLC_OBJ(d)) = ITEM_WEAR_TAKE;
  OLC_VAL(d) = 0;
  OLC_ITEM_TYPE(d) = OBJ_TRIGGER;
  oedit_disp_menu(d);
}
/*------------------------------------------------------------------------*/

void oedit_setup_existing(struct descriptor_data *d, int real_num)
{ struct extra_descr_data *this, *temp, *temp2;
  struct obj_data *obj;

  /* allocate object */
  CREATE (obj, struct obj_data, 1);
  clear_object (obj);
  *obj = obj_proto[real_num];
 
  /* copy all strings over */
  if (obj_proto[real_num].name)
    obj->name = str_dup (obj_proto[real_num].name);
  if (obj_proto[real_num].short_description)
    obj->short_description = str_dup (obj_proto[real_num].short_description);
  if (obj_proto[real_num].description)
    obj->description = str_dup (obj_proto[real_num].description);
  if (obj_proto[real_num].action_description)
    obj->action_description = str_dup (obj_proto[real_num].action_description);

  /*. Extra descriptions if necessary .*/
  if (obj_proto[real_num].ex_description)
  { /* temp is for obj being edited */
    CREATE (temp, struct extra_descr_data, 1);
    obj->ex_description = temp;
    for (this = obj_proto[real_num].ex_description; this; this = this->next)
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
  if (obj_proto[real_num].oprog)
    copy_objprog(&obj_proto[real_num], obj);

  /* DGS stuff */
  if (SCRIPT(obj))
   script_copy(obj, &obj_proto[real_num], OBJ_TRIGGER);

  /*. Attatch new obj to players descriptor .*/
  OLC_OBJ(d) = obj;
  OLC_VAL(d) = 0;
  OLC_ITEM_TYPE(d) = OBJ_TRIGGER;
  dg_olc_script_copy(d);
  oedit_disp_menu(d);
}
/*-------------------------------------------------------------------*/
/*. Remove one action .*/

void remove_obj_action(struct descriptor_data *d)
{
  struct obj_action_list *temp, *this;
  
  this = GET_OBJ_ACTLIST(d->character);  
  if (!this) {
    mlog("Error in obj program oediting at removal of actions: Attempt to remove nonexistant action.\r\n");
    exit(1);
  }
  
  REMOVE_FROM_LIST(this, GET_OBJ_MAINLIST(d->character)->actions, next);  
  if (this->argument)
    free(this->argument);    
  free(this);  
  GET_OBJ_ACTLIST(d->character) = GET_OBJ_MAINLIST(d->character)->actions;
}

/*-------------------------------------------------------------------*/
/*. Remove one sideterm .*/

void remove_obj_sideterm(struct descriptor_data *d)
{
  struct side_term_list *temp, *this;
  
  this = GET_SIDELIST(d->character);  
  if (!this) {
    mlog("Error in obj program oediting at removal of actions: Attempt to remove nonexistant sideterm.\r\n");
    exit(1);
  }
  
  REMOVE_FROM_LIST(this, GET_OBJ_MAINLIST(d->character)->sideterms, next);  
  if (this->argument)
    free(this->argument); 
  GET_SIDELIST(d->character) = GET_OBJ_MAINLIST(d->character)->sideterms;
  free(this);
}

/*-------------------------------------------------------------------*/
/*. Clear the action list in a objprogram .*/

void oedit_clear_actions(struct descriptor_data *d)
{
  struct obj_action_list *action, *next;
  
  for(action = GET_OBJ_MAINLIST(d->character)->actions; action; action = next) {
    next = action->next;
    if (action->argument)
      free(action->argument);
    free(action);
  }
  GET_OBJ_MAINLIST(d->character)->actions = NULL;
  GET_OBJ_ACTLIST(d->character) = NULL;
}

/*-------------------------------------------------------------------*/
/*. Remove all sideterms from a program .*/

void oedit_clear_sideterms(struct descriptor_data *d)
{
  struct side_term_list *sides = NULL, *next = NULL;

  for (sides = GET_OBJ_MAINLIST(d->character)->sideterms; sides; sides = next) {
    next = sides->next;
    if (sides->argument)
      free(sides->argument);
    free(sides);
  }
  GET_SIDELIST(d->character) = NULL;
  GET_OBJ_MAINLIST(d->character)->sideterms = NULL;
}

/*-------------------------------------------------------------------*/
/*. Delete an entire objprogram .*/

void delete_objprog(struct obj_prog_list *prog, struct descriptor_data *d)
{
  struct obj_prog_list *temp = NULL;
  
  REMOVE_FROM_LIST(prog, GET_OBJPROG(OLC_OBJ(d)), next);
  
              /* Freeing time! */
  oedit_clear_actions(d);
  oedit_clear_sideterms(d);
  
  if (prog->argument)
    free(prog->argument);
  free(prog); /* Done! */
}
/*-------------------------------------------------------------------*/



/*-------------------------------------------------------------------*/

#define ZCMD zone_table[zone].cmd[cmd_no]

void oedit_save_internally(struct descriptor_data *d)
{ int i, shop, robj_num, found = FALSE;
  struct extra_descr_data *this, *next_one;
  struct obj_data *obj, *swap, *new_obj_proto;
  struct index_data *new_obj_index;
  struct descriptor_data *dsc;

  axlog( SYS_DEBUG, MAX(LVL_GOD, GET_INVIS_LEV(d->character)), TRUE, "oedit saving obj_index: 0x%lX", obj_index );

  /* write to internal tables */
  robj_num = real_object(OLC_NUM(d));
  if (robj_num > 0) {
    /* we need to run through each and every object currently in the
     * game to see which ones are pointing to this prototype */
  
    /* if object is pointing to this prototype, then we need to replace
     * with the new one */
    CREATE(swap, struct obj_data, 1);
    for (obj = object_list; obj; obj = obj->next) {
      if (obj->item_number == robj_num) {
        *swap = *obj;
        *obj = *OLC_OBJ(d);
        /* copy game-time dependent vars over */
        obj->in_room = swap->in_room;
        obj->item_number = robj_num;
        obj->carried_by = swap->carried_by;
        obj->worn_by = swap->worn_by;
        obj->worn_on = swap->worn_on;
        obj->in_obj = swap->in_obj;
        obj->contains = swap->contains;
        obj->next_content = swap->next_content;
        obj->next = swap->next;
        copy_objprog(swap, obj);
      }
    }
    free_obj(swap);
    /* now safe to free old proto and write over */
    if (obj_proto[robj_num].name)
    free(obj_proto[robj_num].name);
    if (obj_proto[robj_num].description)
      free(obj_proto[robj_num].description);
    if (obj_proto[robj_num].short_description)
      free(obj_proto[robj_num].short_description);
    if (obj_proto[robj_num].action_description)
      free(obj_proto[robj_num].action_description);
    if (obj_proto[robj_num].ex_description)
      for (this = obj_proto[robj_num].ex_description; this; this = next_one) { 
        next_one = this->next;
        if (this->keyword)
      free(this->keyword);
        if (this->description)
          free(this->description);
        free(this);
      }
    obj_proto[robj_num] = *OLC_OBJ(d);
    obj_proto[robj_num].item_number = robj_num;
    obj_proto[robj_num].proto_script = OLC_SCRIPT(d);
  } else {
    MSG_BOARD *brd;
    /*. It's a new object, we must build new tables to contain it .*/

    CREATE(new_obj_index, struct index_data, top_of_objt + 2);
    CREATE(new_obj_proto, struct obj_data, top_of_objt + 2);
    /* start counting through both tables */
    for (i = 0; i <= top_of_objt; i++) {
      /* if we haven't found it */
      if (!found) {
        /* check if current virtual is bigger than our virtual */
        if (obj_index[i].virtual > OLC_NUM(d)) 
        { found = TRUE;
          robj_num = i;
      OLC_OBJ(d)->item_number = robj_num;
      new_obj_index[robj_num].virtual = OLC_NUM(d);
      new_obj_index[robj_num].number = 0;
      new_obj_index[robj_num].func = NULL;
      new_obj_proto[robj_num] = *(OLC_OBJ(d));
          new_obj_proto[robj_num].proto_script = OLC_SCRIPT(d);
      new_obj_proto[robj_num].in_room = NOWHERE;
          /*. Copy over the obj that should be here .*/
          new_obj_index[robj_num + 1] = obj_index[robj_num];
      new_obj_proto[robj_num + 1] = obj_proto[robj_num];
      new_obj_proto[robj_num + 1].item_number = robj_num + 1;
        } else {
      /* just copy from old to new, no num change */
      new_obj_proto[i] = obj_proto[i];
      new_obj_index[i] = obj_index[i];
        }
      } else {
        /* we HAVE already found it.. therefore copy to object + 1 */
        new_obj_index[i + 1] = obj_index[i];
        new_obj_proto[i + 1] = obj_proto[i];
        new_obj_proto[i + 1].item_number = i + 1;
      }
    }
    if (!found)
    { robj_num = i;
      OLC_OBJ(d)->item_number = robj_num;
      new_obj_index[robj_num].virtual = OLC_NUM(d);
      new_obj_index[robj_num].number = 0;
      new_obj_index[robj_num].func = NULL;
      new_obj_proto[robj_num] = *(OLC_OBJ(d));
      new_obj_proto[robj_num].proto_script = OLC_SCRIPT(d);
      new_obj_proto[robj_num].in_room = NOWHERE;
    }

    /* free and replace old tables */
  axlog( SYS_DEBUG, 0, TRUE, "Freeing proto: 0x%lX and index: 0x%lX", obj_proto, obj_index );
    free (obj_proto);
    free (obj_index);
    obj_proto = new_obj_proto;
    obj_index = new_obj_index;
    top_of_objt++;

    /*. Renumber live objects .*/
    for (obj = object_list; obj; obj = obj->next)
      if (GET_OBJ_RNUM (obj) >= robj_num)
        GET_OBJ_RNUM (obj)++;

    /*. Renumber notice boards */
    for( brd=top_of_boards; brd; brd=brd->next )
      if( brd->rnum >= robj_num)
	brd->rnum++;

    /*. Renumber shop produce .*/
    for(shop = 0; shop < top_shop; shop++)
      for(i = 0; SHOP_PRODUCT(shop, i) != -1; i++)
        if (SHOP_PRODUCT(shop, i) >= robj_num)
          SHOP_PRODUCT(shop, i)++;

    /*. Renumber produce in shops being edited .*/
    for(dsc = descriptor_list; dsc; dsc = dsc->next)
      if(dsc->connected == CON_SEDIT)
        for(i = 0; S_PRODUCT(OLC_SHOP(dsc), i) != -1; i++)
          if (S_PRODUCT(OLC_SHOP(dsc), i) >= robj_num)
            S_PRODUCT(OLC_SHOP(dsc), i)++;
      
  } 
  olc_add_to_save_list(zone_table[OLC_ZNUM(d)].number, OLC_SAVE_OBJ);
}
/*------------------------------------------------------------------------*/

void oedit_save_to_disk(struct descriptor_data *d)
{ 
  FILE *fp;
  char tname[1024],fname[1024];

  sprintf(fname, "%s/%d.obj", OBJ_PREFIX, zone_table[OLC_ZNUM(d)].number);
  sprintf(tname,"%s/%d.tmp",OBJ_PREFIX, zone_table[OLC_ZNUM(d)].number);

  if (!(fp = fopen(tname, "w+")))
  { mudlog("SYSERR: OLC: Cannot open objects file!", BRF, LVL_BUILDER, TRUE);
    return;
  }

  objects_save_zone_to_file((zone_table[OLC_ZNUM(d)].number * 100),zone_table[OLC_ZNUM(d)].top,fp);
  fclose(fp);

  remove(fname);
  rename(tname,fname);

  olc_remove_from_save_list(zone_table[OLC_ZNUM(d)].number, OLC_SAVE_OBJ);
}

/**************************************************************************
 Menu functions 
 **************************************************************************/


/*. Display actions that can be added. .*/
void oedit_disp_addaction(struct descriptor_data *d)
{
  int i, j;
  char temp[MAX_INPUT_LENGTH];
  int after_pause = 0;
  struct obj_action_list *dum;


  j = GET_OBJ_MAINLIST(d->character)->main;
  

  for (dum = GET_OBJ_MAINLIST(d->character)->actions; dum; dum = dum->next)
    if (dum->action == OPA_PAUSE) {
      after_pause = TRUE;
      break;
    }

  send_to_char("[H[J", d->character);
  sprintf(buf,"         Select action:\r\n=========================================\r\n");
  
  for (i = 1; *obj_actions[i] != '*'; i ++){
    if (((j == OPC_TIME) || (j == OPC_RANDOM) || (j == OPC_TICK) || (after_pause)) && 
    ((i == OPA_FORCE) || (i == OPA_STOP) || (i == OPA_EMOTETO)))
      sprintf(temp, "Not available.");
    else
      sprintf(temp, obj_actions[i]);
    sprintf(buf,"%s\\c02%2d\\c00) %-32s", buf, i, temp);
    if (!(i%2))
      sprintf(buf,"%s\r\n", buf);
  }
  sprintf(buf,"%s\r\n\r\nEnter number: ", buf);
  send_to_char(buf, d->character);
}

/*-------------------------------------------------------------------*/
/*. Display side terms that can be added. .*/
void oedit_disp_addside(struct descriptor_data *d)
{
  int i, j;
  char temp[MAX_INPUT_LENGTH];
  j = GET_OBJ_MAINLIST(d->character)->main;
  
  send_to_char("[H[J", d->character);
  sprintf(buf,"         Select side term:\r\n=========================================\r\n");
  for (i = 1; *obj_sideterms[i] != '*'; i ++){
    if (((j == OPC_TIME) || (j == OPC_RANDOM) ||
       (j == OPC_TICK)) && (i != OPC_STATUS))
      sprintf(temp,"Not available.");
    else
      sprintf(temp, obj_sideterms[i]);
    sprintf(buf,"%s\\c02%2d\\c00) %-32s", buf, i, temp);
    if (!(i%2))
      sprintf(buf,"%s\r\n", buf);
  }
  sprintf(buf,"%s\r\n\r\nEnter number: ", buf);
  send_to_char(buf, d->character);
}

/*-------------------------------------------------------------------*/
/*. Display current sideterm list .*/

void oedit_disp_sideterms(struct descriptor_data *d)
{
  struct side_term_list *list;
  char temp[MAX_INPUT_LENGTH], temp2[15];
  int i = 1;
  
  send_to_char("[H[J", d->character);
  send_to_char("Current side terms in the list:\r\n", d->character);
  send_to_char("===============================\r\n", d->character);
  if (!GET_OBJ_MAINLIST(d->character)->sideterms)
    send_to_char(" <none>", d->character);
  else
  for (list = GET_OBJ_MAINLIST(d->character)->sideterms; list; list = list->next, i++)
  {
    if (list->not)
      sprintf(temp2, " \\c01(Not)\\c00 ");
    else
      sprintf(temp2, " ");
    if ((list->number == OPC_SEX) || (list->number == OPC_LEVEL) ||
        (list->number == OPC_RACE) || (list->number == OPC_GUILD) ||
        (list->number == OPC_WEARINGN) || (list->number == OPC_CARRYINGN)||
    (OPC_GRANKIS == list->number) || (OPC_GRANKLESS == list->number) ||
    (OPC_GRANKMORE == list->number))
      sprintf(temp," \\c02%2d\\c00)%s %s\"%s\"\r\n", i, temp2, obj_sideterms[list->number], list->argument);
    else if ((list->number == OPC_CARRYING) || (list->number == OPC_WEARING) || 
             (list->number == OPC_STATUS))
      sprintf(temp," \\c02%2d\\c00)%s%s %d\r\n", i, temp2, obj_sideterms[list->number], list->int_arg);
    else 
      sprintf(temp, " \\c02%2d\\c00)%s%s\r\n", i, temp2, obj_sideterms[list->number]);
    send_to_char(temp, d->character);
  }   
  send_to_char("\r\n\r\n\r\n(\\c03A\\c00)dd, (\\c03R\\c00)emove, (\\c03E\\c00)xit, (\\c03C\\c00)lear list\r\nEnter your choice:", d->character);
}

/*-------------------------------------------------------------------*/
/*. Display current actionlist .*/
void oedit_disp_actions(struct descriptor_data *d)
{
  struct obj_action_list *list;
  char temp[MAX_INPUT_LENGTH];
  int i = 1;
  send_to_char("[H[J", d->character);
  sprintf(buf,"Current actions are in the list:\r\n================================\r\n");
  GET_OBJ_ACTLIST(d->character) = GET_OBJ_MAINLIST(d->character)->actions;
  if (GET_OBJ_ACTLIST(d->character)) {
    for (list = GET_OBJ_ACTLIST(d->character);  list; list = list->next, i++) {
      if ((list->action == OPA_GOTO) || (list->action == OPA_WEARDOWN) ||
          (list->action == OPA_PAUSE)) 
        sprintf(temp, "%d", list->intarg);
      else if ((list->action != OPA_STOP))  
        sprintf(temp, "\"%s\"", list->argument);
      else
        sprintf(temp, " ");
      sprintf(buf, "%s \\c02%2d\\c00 - %s %s\r\n", buf, i, obj_actions[list->action], temp);
    }
    send_to_char(buf, d->character);
  }
  else {
    send_to_char(buf, d->character);
    send_to_char("     <none>\r\n", d->character);
  }
  send_to_char("\r\n\r\n\r\n(\\c03A\\c00)dd, (\\c03R\\c00)emove, (\\c03E\\c00)xit, (\\c03C\\c00)lear list\r\nEnter your choice:", d->character);
}

void oedit_disp_mainterms(struct descriptor_data *d)
{
  int i;
  
  send_to_char("[H[J", d->character);
  sprintf(buf,"         Select main term:\r\n=========================================\r\n");
  for (i = 1; *obj_mainterms[i] != '*'; i ++)
    sprintf(buf,"%s\\c02%2d\\c00) %s\r\n", buf, i, obj_mainterms[i]);
  sprintf(buf,"%s\r\n\r\nEnter number:", buf);
  send_to_char(buf, d->character);
}

/*-------------------------------------------------------------------*/
/*. Display current obj program .*/

void oedit_disp_obj_program(struct descriptor_data *d)
{
  char temp[MAX_INPUT_LENGTH], temp2[15];
  int imain = 0, side = 0, action = 0, not = 0, side_int_arg = 0, 
  main_int_arg = 0, act_int_arg = 0;
  char *sidearg = NULL, *mainarg = NULL, *actarg = NULL;
  struct side_term_list *this_side = NULL;
  struct obj_prog_list *opl =  NULL, *list = NULL;
  struct obj_action_list  *this_act = NULL;
  
  list = GET_OBJ_MAINLIST(d->character);
  
  if (list) {    
    opl = list;
    imain = opl->main;
    mainarg = opl->argument;
    main_int_arg = opl->intarg;
    if ((imain == OPC_ACTIONROOM) || (imain == OPC_ACTIONTOROOM) ||
        (imain == OPC_ACTIONWEAR) || (imain == OPC_ACTIONTOWEAR) ||
        (imain == OPC_ACTIONCARRY) || (imain == OPC_ACTIONTOCARRY) ||
        (imain == OPC_SAYROOM) || (imain == OPC_SAYWEAR) ||
        (imain == OPC_SAYCARRY)) /* Arg is a string */
      sprintf(temp,"\"%s\"", mainarg);
    else if ((imain == OPC_TIME) || (imain == OPC_RANDOM)) 
      sprintf(temp, "%d", main_int_arg);
    else 
      sprintf(temp, " ");   
  }
  else {
    imain = -1;
    strcpy(temp,"<no arg>");
  }
  send_to_char("[H[J", d->character);
  
  sprintf(buf, "\\c02Main term:\\c00 %s%s\r\n",(opl)? obj_mainterms[imain]: "\\c01<none>\\c00 ", temp);
  sprintf(buf, "%s \\c02-----Sideterms-----\\c00\r\n", buf);
  if (opl->sideterms)
    for (this_side = opl->sideterms; this_side; this_side = this_side->next) {
      side = this_side->number;
      sidearg = this_side->argument;
      not = this_side->not;
      side_int_arg = this_side->int_arg;
      if (not)
        sprintf(temp2, " (Not) ");
      else 
        sprintf(temp2, " ");
      if ((side == OPC_SEX) || (side == OPC_RACE) || (side == OPC_GUILD) ||
      (side == OPC_LEVEL) || (side == OPC_WEARINGN) || (side == OPC_CARRYINGN)) 
        sprintf(temp, "\"%s\"", sidearg);
      else if ((side == OPC_CARRYING) || (side == OPC_WEARING) || (side == OPC_STATUS)) 
        sprintf(temp, "%d", side_int_arg);
      else sprintf(temp, " ");
      sprintf(buf, "%s%s%s%s\r\n", buf, temp2, obj_sideterms[side], temp);
    }
  else
    sprintf(buf, "%s   <none>\r\n", buf);
    
  sprintf(buf, "%s\r\n \\c02-----Actions-----\\c00 \r\n", buf);
  if (opl->actions)
    for (this_act = opl->actions; this_act; this_act = this_act->next) {
      action = this_act->action;
      actarg = this_act->argument;
      act_int_arg = this_act->intarg;
      if ((action == OPA_GOTO) || (action == OPA_WEARDOWN) || (action == OPA_PAUSE)) 
        sprintf(temp, " %d", act_int_arg);
      else if ((action != OPA_STOP) && (action != OPA_DESTROY))  
        sprintf(temp,  "\"%s\"", actarg);
      else
        sprintf(temp, " ");
        
      sprintf(buf, "%s %s %s\r\n", buf, obj_actions[action], temp);
    }
  else
    sprintf(buf, "%s   \\c08<none>\\c00\r\n", buf);
  send_to_char(buf, d->character);
  
  send_to_char("(\\c03D\\c00)elete (\\c03E\\c00)xit (\\c03M\\c00)ainterms (\\c03S\\c00)ideterms (\\c03A\\c00)ctions (\\c03N\\c00)ew ", d->character);
  if (opl->next)
    send_to_char(" Ne(\\c03x\\c00)t", d->character);
  if (opl != GET_OBJPROG(OLC_OBJ(d)))
    send_to_char(" (\\c03P\\c00)revious", d->character);
  send_to_char("\r\nEnter choice:", d->character);   
}

/* For container flags */
void oedit_disp_container_flags_menu(struct descriptor_data * d)
{ get_char_cols(d->character);
  sprintbit(GET_OBJ_VAL(OLC_OBJ(d), 1), container_bits, buf1);
  send_to_char("[H[J", d->character);
  sprintf(buf,
    "%s1%s) CLOSEABLE\r\n"
    "%s2%s) PICKPROOF\r\n"
    "%s3%s) CLOSED\r\n"
    "%s4%s) LOCKED\r\n"
    "Container flags: %s%s%s\r\n"
        "Enter flag, 0 to quit : ",
        grn, nrm, grn, nrm, grn, nrm, grn, nrm, cyn, buf1, nrm
  );
  send_to_char(buf, d->character);
}

/* For extra descriptions */
void oedit_disp_extradesc_menu(struct descriptor_data * d)
{
  struct extra_descr_data *extra_desc = OLC_DESC(d);
  
  if (!extra_desc->next)
    strcpy(buf1, "<Not set>\r\n");
  else
    strcpy(buf1, "Set.");

  get_char_cols(d->character);
  send_to_char("[H[J", d->character);
  sprintf(buf, 
    "Extra desc menu\r\n"
    "%s1%s) Keyword: %s%s\r\n"
    "%s2%s) Description:\r\n%s%s\r\n"
        "%s3%s) Goto next description: %s\r\n"
    "%s0%s) Quit\r\n"
        "Enter choice : ",

    grn, nrm, yel, extra_desc->keyword ? extra_desc->keyword : "<NONE>",
    grn, nrm, yel, extra_desc->description ? extra_desc->description : "<NONE>",
        grn, nrm, buf1,
        grn, nrm
  );
  send_to_char(buf, d->character);
  OLC_MODE(d) = OEDIT_EXTRADESC_MENU;
}

/* Ask for *which* apply to edit */
void oedit_disp_prompt_apply_menu(struct descriptor_data * d)
{
  int counter;

  get_char_cols(d->character);
  send_to_char("[H[J", d->character);
  for (counter = 0; counter < MAX_OBJ_AFFECT; counter++) {
    if (OLC_OBJ(d)->affected[counter].modifier) {
      sprinttype(OLC_OBJ(d)->affected[counter].location, apply_types, buf2);
      sprintf(buf, " %s%d%s) %+ld to %s\r\n", 
            grn, counter + 1, nrm,
        OLC_OBJ(d)->affected[counter].modifier, buf2
      );
      send_to_char(buf, d->character);
    } else {
      sprintf(buf, " %s%d%s) None.\r\n", grn, counter + 1, nrm);
      send_to_char(buf, d->character);
    }
  }
  send_to_char("\r\nEnter affection to modify (0 to quit) : ", d->character);
  OLC_MODE(d) = OEDIT_PROMPT_APPLY;
}

/*. Ask for liquid type .*/
void oedit_liquid_type(struct descriptor_data * d)
{ int counter, columns = 0;

  get_char_cols(d->character);
  send_to_char("[H[J", d->character);
  for (counter = 0; counter < NUM_LIQ_TYPES; counter++) 
  { sprintf(buf, " %s%2d%s) %s%-20.20s ", 
            grn, counter, nrm, yel,
        drinks[counter]
    );
    if (!(++columns % 2))
      strcat(buf, "\r\n");
    send_to_char(buf, d->character);
  }
  sprintf(buf, "\r\n%sEnter drink type : ", nrm);
  send_to_char(buf, d->character);
  OLC_MODE(d) = OEDIT_VALUE_3;
}

/* The actual apply to set */
void oedit_disp_apply_menu(struct descriptor_data * d)
{
  int counter, columns = 0;

  get_char_cols(d->character);
  send_to_char("[H[J", d->character);
  for (counter = 0; counter < NUM_APPLIES; counter++) 
  { sprintf(buf, "%s%2d%s) %-20.20s ",
        grn, counter, nrm, apply_types[counter]
    );
    if (!(++columns % 2))
      strcat(buf, "\r\n");
    send_to_char(buf, d->character);
  }
  send_to_char("\r\nEnter apply type (0 is no apply) : ", d->character);
  OLC_MODE(d) = OEDIT_APPLY;
}


/* weapon type */
void oedit_disp_weapon_menu(struct descriptor_data * d)
{ int counter, columns = 0;

  get_char_cols(d->character);
  send_to_char("[H[J", d->character);
  for (counter = 0; counter < NUM_ATTACK_TYPES; counter++) 
  { sprintf(buf, "%s%2d%s) %-20.20s ",
        grn, counter, nrm, attack_hit_text[counter].singular
    );
    if(!(++columns % 2))
      strcat(buf, "\r\n");
    send_to_char(buf, d->character);
  }
  send_to_char("\r\nEnter weapon type : ", d->character);
}

/* spell type */
void oedit_disp_spells_menu(struct descriptor_data * d)
{
  int counter, columns = 0;

  get_char_cols(d->character);
  send_to_char("[H[J", d->character);
  for (counter = 0; counter < NUM_SPELLS; counter++) {
    sprintf(buf, "%s%2d%s) %s%-20.20s ",
        grn, counter, nrm, yel, spells[counter]
    );
    if (!(++columns % 3))
      strcat(buf, "\r\n");
    send_to_char(buf, d->character);
  }
  sprintf(buf, "\r\n%sEnter spell choice (0 for none) : ", nrm);
  send_to_char(buf, d->character);
}

void furniture_type_menu(struct descriptor_data *d)
{
  int counter, columns = 0;
  
  get_char_cols(d->character);
#if defined(CLEAR_SCREEN)
  send_to_char("^[[H^[[J", d->character);
#endif
  send_to_char("\r\n", d->character);
  for (counter = 0; counter < NUM_FURNITURE_TYPES; counter++) {   
    sprintf(buf, "%s%2d%s) %s%-20.20s %s", grn, counter, nrm, yel,
                furniture_types[counter], !(++columns % 3) ? "\r\n" : "");
    send_to_char(buf, d->character);
  }
  sprintf(buf, "\r\n\r\n%sEnter type choice : ", nrm);
  send_to_char(buf, d->character);
}  

void furniture_usage_type(struct descriptor_data *d)
{
  int counter, columns = 0;
  
  get_char_cols(d->character);
#if defined(CLEAR_SCREEN)
  send_to_char("^[[H^[[J", d->character);
#endif
  send_to_char("\r\n", d->character);
  for (counter = 0; counter < NUM_FURNITURE_USAGE_TYPES; counter++) {
    sprintf(buf, "%s%2d%s) %s%-20.20s %s", grn, counter, nrm, yel,
                furniture_usages[counter], !(++columns % 3) ? "\r\n" : "");
    send_to_char(buf, d->character);
  }
  sprintf(buf, "\r\n%sEnter usage type choice : ", nrm);
  send_to_char(buf, d->character);
}

/* object value 1 */
void oedit_disp_val1_menu(struct descriptor_data * d)
{
  OLC_MODE(d) = OEDIT_VALUE_1;
  switch (GET_OBJ_TYPE(OLC_OBJ(d))) {
  case ITEM_LIGHT:
    /* values 0 and 1 are unused.. jump to 2 */
    oedit_disp_val3_menu(d);
    break;
  case ITEM_OTHER:
    if (IS_SET(GET_OBJ_EXTRA(OLC_OBJ(d)),ITEM_TERANGREAL)) {
      send_to_char("Enter the number of the spell for this ter'angreal(1-200): ",d->character);
    } else {
      oedit_disp_menu(d);
    }
    break;
  case ITEM_INSTRUMENT:
  case ITEM_HERB:
    send_to_char("No effect/Harming/Healing (0/1/2) : ", d->character);
    break;
  case ITEM_POTION:
    send_to_char("Spell level : ", d->character);
    break; 
  case ITEM_BOW:
  case ITEM_ARROW:
  case ITEM_WEAPON:
    /* this seems to be a circleism.. not part of normal diku? */
    send_to_char("Modifier to Hitroll : ", d->character);
    break;
  case ITEM_ARMOR:
    send_to_char("Apply to AC : ", d->character);
    break;
  case ITEM_CONTAINER:
    send_to_char("Max weight to contain : ", d->character);
    break;
  case ITEM_WAGON:
    oedit_disp_val2_menu(d);
    break;
  case ITEM_DRINKCON:
  case ITEM_FOUNTAIN:
    send_to_char("Max drink units : ", d->character);
    break;
  case ITEM_FOOD:
    send_to_char("Hours to fill stomach : ", d->character);
    break;
    
  case ITEM_MONEY:
    send_to_char("Number of gold coins : ", d->character);
    break;
  case ITEM_HEALING:
    send_to_char("Heal what? \\c020\\c00: Hitpoints, \\c021\\c00: Mana, \\c022\\c00 Move : ", d->character);
    break;
  case ITEM_FURNITURE:
    furniture_type_menu(d);
    break;
  case ITEM_DAGGER:
    send_to_char("Enter Damage Ammount: ", d->character);
  case ITEM_NOTE:
    /* this is supposed to be language, but it's unused */
    break;
  default:
    oedit_disp_menu(d);
  }
}

/* object value 2 */
void oedit_disp_val2_menu(struct descriptor_data * d)
{
  OLC_MODE(d) = OEDIT_VALUE_2;
  switch (GET_OBJ_TYPE(OLC_OBJ(d))) {
  case ITEM_OTHER:
    if (IS_SET(GET_OBJ_EXTRA(OLC_OBJ(d)),ITEM_TERANGREAL)) {
      send_to_char("Enter lvl of \"spell-caster\" for this ter'angreal(1-199): ",d->character);
    } else {
      oedit_disp_menu(d);
    }
    break;
  case ITEM_INSTRUMENT:
  case ITEM_HERB:
    send_to_char("Strength of affection in points: ", d->character);
    break;
  case ITEM_POTION:
    oedit_disp_spells_menu(d);
    break; 
  case ITEM_BOW:
  case ITEM_ARROW:
  case ITEM_WEAPON:
    send_to_char("Number of damage dice : ", d->character);
    break;
  case ITEM_FOOD:
    /* values 2 and 3 are unused, jump to 4. how odd */
    oedit_disp_val4_menu(d);
    break;
  case ITEM_WAGON:
  case ITEM_CONTAINER:
    /* these are flags, needs a bit of special handling */
    oedit_disp_container_flags_menu(d);
    break;
  case ITEM_DRINKCON:
  case ITEM_FOUNTAIN:
    send_to_char("Initial drink units : ", d->character);
    break;
  case ITEM_ARMOR:
    send_to_char("Apply to absorbation: ", d->character);
    break;
  case ITEM_FURNITURE:
    furniture_usage_type(d);
    break;
  case ITEM_HEALING:
    send_to_char("\\c020\\c00 - absolute healing, \\c021\\c00 - percentual healing :", d->character);
    break;
  default:
    oedit_disp_menu(d);
  }
}

/* object value 3 */
void oedit_disp_val3_menu(struct descriptor_data * d)
{
  OLC_MODE(d) = OEDIT_VALUE_3;
  switch (GET_OBJ_TYPE(OLC_OBJ(d))) {
  case ITEM_OTHER:
    if (IS_SET(GET_OBJ_EXTRA(OLC_OBJ(d)),ITEM_TERANGREAL)) {
      send_to_char("Mana add this Ter'angreal will use(0-12000): ",d->character);
    } else {
      oedit_disp_menu(d);
    }
    break;
  case ITEM_LIGHT:
    send_to_char("Number of hours (0 = burnt, -1 is infinite) : ", d->character);
    break;
  case ITEM_HERB:
    send_to_char("Stat the herb affects (1 = HP, 2 = Mana, 3 = Move) :", d->character);
    break;
  case ITEM_INSTRUMENT:
    send_to_char("Stat the instrument affects (1 = HP, 2 = Mana, 3 = Move) :", d->character);
    break;
  case ITEM_POTION:
    oedit_disp_spells_menu(d);
    break; 
  case ITEM_BOW:
  case ITEM_ARROW:
  case ITEM_WEAPON:
    send_to_char("Size of damage dice : ", d->character);
    break;
  case ITEM_WAGON:
  case ITEM_CONTAINER:
    send_to_char("Vnum of key to open container (-1 for no key) : ", d->character);
    break;
  case ITEM_DRINKCON:
  case ITEM_FOUNTAIN:
    oedit_liquid_type(d);
    break;
  case ITEM_FURNITURE:
    send_to_char("Max users : ", d->character);
    break;
  case ITEM_HEALING:
    send_to_char("Heal how much? (Not over 100 if percentual) :", d->character);
    break;
  default:
    oedit_disp_menu(d);
  }
}

/* object value 4 */
void oedit_disp_val4_menu(struct descriptor_data * d)
{
  OLC_MODE(d) = OEDIT_VALUE_4;
  switch (GET_OBJ_TYPE(OLC_OBJ(d))) {
  case ITEM_INSTRUMENT:
    send_to_char("Delay before musician can play again, in ticks : ", d->character);
  break;
  case ITEM_POTION:
    oedit_disp_spells_menu(d);
    break;
  case ITEM_WEAPON:
    oedit_disp_weapon_menu(d);
    break;
  case ITEM_DRINKCON:
  case ITEM_FOUNTAIN:
  case ITEM_FOOD:
    send_to_char("Poisoned (0 = not poison) : ", d->character);
    break;
  default:
    oedit_disp_menu(d);
  }
}

/* object type */
void oedit_disp_type_menu(struct descriptor_data * d)
{ int counter, columns = 0;

  get_char_cols(d->character);
  send_to_char("[H[J", d->character);
  for (counter = 0; counter < NUM_ITEM_TYPES; counter++) {
    sprintf(buf, "%s%2d%s) %-20.20s ",
        grn, counter, nrm, item_types[counter]
    );
    if (!(++columns % 2))
      strcat(buf, "\r\n");
    send_to_char(buf, d->character);
  }
  send_to_char("\r\nEnter object type : ", d->character);
}

/* object extra flags */
void oedit_disp_extra_menu(struct descriptor_data * d)
{
  int counter, columns = 0;

  get_char_cols(d->character);
  send_to_char("[H[J", d->character);
  for (counter = 0; counter < NUM_ITEM_FLAGS; counter++) 
  { sprintf(buf, "%s%2d%s) %-20.20s ",
        grn, counter + 1, nrm, extra_bits[counter]
    );
    if (!(++columns % 2))
      strcat(buf, "\r\n");
    send_to_char(buf, d->character);
  }
  sprintbit(GET_OBJ_EXTRA(OLC_OBJ(d)), extra_bits, buf1);
  sprintf(buf,  "\r\nObject flags: %s%s%s\r\n"
        "Enter object extra flag (0 to quit) : ", 
        cyn, buf1, nrm
  );
  send_to_char(buf, d->character);
}

/* object wear flags */
void
oedit_disp_wear_menu(struct descriptor_data * d)
{
  int counter, columns = 0;

  get_char_cols(d->character);
  send_to_char("[H[J", d->character);
  for (counter = 0; counter < NUM_ITEM_WEARS; counter++) {
    sprintf(buf, "%s%2d%s) %-20.20s ",
        grn, counter + 1, nrm, wear_bits[counter]
    );
    if (!(++columns % 2))
      strcat(buf, "\r\n");
    send_to_char(buf, d->character);
  }
  sprintbit(GET_OBJ_WEAR(OLC_OBJ(d)), wear_bits, buf1);
  sprintf(buf,  "\r\nWear flags: %s%s%s\r\n"
        "Enter wear flag, 0 to quit : ",
        cyn, buf1, nrm
  );
  send_to_char(buf, d->character);
}


/* display main menu */
void
oedit_disp_menu(struct descriptor_data * d)
{ struct obj_data *obj;

  obj = OLC_OBJ(d);
  get_char_cols(d->character);

  /*. Build buffers for first part of menu .*/
  sprinttype(GET_OBJ_TYPE(obj), item_types, buf1);
  sprintbit(GET_OBJ_EXTRA(obj), extra_bits, buf2);

  /*. Build first hallf of menu .*/
  sprintf(buf, "[H[J"
    "-- Item number : [%s%ld%s]\r\n"
    "%s1%s) Namelist : %s%s\r\n"
    "%s2%s) S-Desc   : %s%s\r\n"
    "%s3%s) L-Desc   :-\r\n%s%s\r\n"
    "%s4%s) A-Desc   :-\r\n%s%s"
    "%s5%s) Type        : %s%s\r\n"
    "%s6%s) Extra flags : %s%s\r\n",

    cyn, OLC_NUM(d), nrm, 
    grn, nrm, yel, obj->name,
    grn, nrm, yel, obj->short_description,
    grn, nrm, yel, obj->description,
    grn, nrm, yel, obj->action_description ?  obj->action_description : "<not set>\r\n",
    grn, nrm, cyn, buf1,
    grn, nrm, cyn, buf2
  );
  /*. Send first half .*/
  send_to_char(buf, d->character);

  /*. Build second half of menu .*/
  sprintbit(GET_OBJ_WEAR(obj), wear_bits, buf1);
  sprintf(buf,
    "%s7%s) Wear flags  : %s%s\r\n"
    "%s8%s) Weight      : %s%ld\r\n"
    "%s9%s) Cost        : %s%ld\r\n"
    "%sA%s) Cost in QP  : %s%ld\r\n"
    "%sB%s) Timer       : %s%ld\r\n"
/*.  	"%sC%s) Level       : %s%d\r\n" -- Object level .*/
    "%sD%s) Values      : %s%ld %ld %ld %ld\r\n"
    "%sE%s) Applies menu\r\n"
    "%sF%s) Extra descriptions menu\r\n"
        "%sO%s) Object programs\r\n"
        "%sS%s) Script      : %s%s\r\n"
    "%sQ%s) Quit\r\n"
    "Enter choice : ",

    grn, nrm, cyn, buf1,
    grn, nrm, cyn, GET_OBJ_WEIGHT(obj),
    grn, nrm, cyn, GET_OBJ_COST(obj),
    grn, nrm, cyn, GET_OBJ_QCOST(obj),
    grn, nrm, cyn, GET_OBJ_TIMER(obj),
/*. 	grn, nrm, cyn, GET_OBJ_LEVEL(obj), -- Object level .*/
    grn, nrm, cyn, GET_OBJ_VAL(obj, 0), 
                GET_OBJ_VAL(obj, 1), 
            GET_OBJ_VAL(obj, 2),
                GET_OBJ_VAL(obj, 3),
        grn, nrm, grn, nrm, grn, nrm, 
        grn, nrm, cyn, (obj->proto_script ? "Set." : "Not Set."), 
        grn, nrm
  );
  send_to_char(buf, d->character);
  OLC_MODE(d) = OEDIT_MAIN_MENU;
}

/***************************************************************************
 main loop (of sorts).. basically interpreter throws all input to here
 ***************************************************************************/


void oedit_parse(struct descriptor_data * d, char *arg)
{
  int number, max_val = 0, min_val = 0, i = 0, j = 0, after_pause = 0;
  struct obj_prog_list *present, *present_next = NULL;
  struct obj_action_list *dum;
  char temp[100], *strng;
  switch (OLC_MODE(d)) {

  case OEDIT_CONFIRM_SAVESTRING:
    switch (*arg) {
    case 'y':
    case 'Y':
      send_to_char("Saving object to memory.\r\n", d->character);
      oedit_save_internally(d);
      sprintf(buf, "OLC: %s edits obj %ld",  GET_DISGUISED(d->character) ? GET_RNAME(d->character) : GET_NAME(d->character), OLC_NUM(d));
      mudlog(buf, CMP, LVL_BUILDER, TRUE);
      cleanup_olc(d, CLEANUP_STRUCTS);
      return;
    case 'n':
    case 'N':
      /*. Cleanup all .*/
      cleanup_olc(d, CLEANUP_ALL);
      return;
    default:
      send_to_char("Invalid choice!\r\n", d->character);
      send_to_char("Do you wish to save this object internally?\r\n", d->character);
      return;
    }

  case OEDIT_MAIN_MENU:
    /* throw us out to whichever edit mode based on user input */
    switch (*arg) {
    case 'q':
    case 'Q':
      if (OLC_VAL(d)) 
      { /*. Something has been modified .*/
        send_to_char("Do you wish to save this object internally? : ", d->character);
        OLC_MODE(d) = OEDIT_CONFIRM_SAVESTRING;
      } else 
        cleanup_olc(d, CLEANUP_ALL);
      return;
    case '1':
      send_to_char("Enter namelist : ", d->character);
      OLC_MODE(d) = OEDIT_EDIT_NAMELIST;
      break;
    case '2':
      send_to_char("Enter short desc : ", d->character);
      OLC_MODE(d) = OEDIT_SHORTDESC;
      break;
    case '3':
      send_to_char("Enter long desc :-\r\n| ", d->character);
      OLC_MODE(d) = OEDIT_LONGDESC;
      break;
    case '4':
      OLC_MODE(d) = OEDIT_ACTDESC;
      SEND_TO_Q("Enter action description: (/s saves /h for help)\r\n\r\n", d);
SEND_TO_Q("Please format your desc to less than 75 characters per line.\r\n", d);
SEND_TO_Q("This will make your desc easier to read for those with smaller screens\r\n",d);
SEND_TO_Q("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n",d);

      d->backstr = NULL;
      if (OLC_OBJ(d)->action_description) {
       SEND_TO_Q(OLC_OBJ(d)->action_description, d);
       d->backstr = str_dup(OLC_OBJ(d)->action_description);
      }
      d->str = &OLC_OBJ(d)->action_description;
      d->max_str = MAX_MESSAGE_LENGTH;
      d->mail_to[0] = 0;
      OLC_VAL(d) = 1; 
      d->character->desc->current_text_line = count_newlines(d->str) + 1;
      break;
    case '5':
      oedit_disp_type_menu(d);
      OLC_MODE(d) = OEDIT_TYPE;
      break;
    case '6':
      oedit_disp_extra_menu(d);
      OLC_MODE(d) = OEDIT_EXTRAS;
      break;
    case '7':
      oedit_disp_wear_menu(d);
      OLC_MODE(d) = OEDIT_WEAR;
      break;
    case '8':
      send_to_char("Enter weight : ", d->character);
      OLC_MODE(d) = OEDIT_WEIGHT;
      break;
    case '9':
      send_to_char("Enter cost : ", d->character);
      OLC_MODE(d) = OEDIT_COST;
      break;
    case 'a':
    case 'A':
      send_to_char("Enter cost per day : ", d->character);
      OLC_MODE(d) = OEDIT_COSTPERDAY;
      break;
    case 'b':
    case 'B':
      send_to_char("Enter timer : ", d->character);
      OLC_MODE(d) = OEDIT_TIMER;
      break;
    case 'c':
    case 'C':
      oedit_disp_menu(d);
      /*. Object level flags in my mud...
      send_to_char("Enter level : ", d->character);
      OLC_MODE(d) = OEDIT_LEVEL;
      .*/
      break;
    case 'd':
    case 'D':
      /*. Clear any old values .*/
      GET_OBJ_VAL(OLC_OBJ(d), 0) = 0;
      GET_OBJ_VAL(OLC_OBJ(d), 1) = 0;
      GET_OBJ_VAL(OLC_OBJ(d), 2) = 0;
      GET_OBJ_VAL(OLC_OBJ(d), 3) = 0;
      OLC_VAL(d) = 1;
      oedit_disp_val1_menu(d);
      break;
    case 'e':
    case 'E':
      oedit_disp_prompt_apply_menu(d);
      break;
    case 'f':
    case 'F':
      /* if extra desc doesn't exist . */
      if (!OLC_OBJ(d)->ex_description) {
    CREATE(OLC_OBJ(d)->ex_description, struct extra_descr_data, 1);
    OLC_OBJ(d)->ex_description->next = NULL;
      }
      OLC_DESC(d) = OLC_OBJ(d)->ex_description;
      oedit_disp_extradesc_menu(d);
      break;
    case 'o':
    case 'O':
        OLC_MODE(d) = OEDIT_OBJPROG;
        if (!GET_OBJPROG(OLC_OBJ(d)))
          CREATE(GET_OBJPROG(OLC_OBJ(d)), struct obj_prog_list, 1);
        GET_OBJ_MAINLIST(d->character) = GET_OBJPROG(OLC_OBJ(d));
        oedit_disp_obj_program(d);
        return;
    break;
    case 's':
    case 'S':
        OLC_SCRIPT_EDIT_MODE(d) = SCRIPT_MAIN_MENU;
        dg_script_menu(d);
        return;
    default:
      oedit_disp_menu(d);
      break;
    }
    return;			/* end of OEDIT_MAIN_MENU */

  case OLC_SCRIPT_EDIT:
    if(dg_script_edit_parse(d, arg)) return;
    break;
  case OEDIT_EDIT_NAMELIST:
    if (OLC_OBJ(d)->name)
      free(OLC_OBJ(d)->name);
    OLC_OBJ(d)->name = str_dup(arg);
    break;

  case OEDIT_SHORTDESC:
    if (OLC_OBJ(d)->short_description)
      free(OLC_OBJ(d)->short_description);
    OLC_OBJ(d)->short_description = str_dup(arg);
    break;

  case OEDIT_LONGDESC:
    if (OLC_OBJ(d)->description)
      free(OLC_OBJ(d)->description);
    OLC_OBJ(d)->description = str_dup(arg);
    break;

  case OEDIT_TYPE:
    number = atoi(arg);
    if ((number < 1) || (number >= NUM_ITEM_TYPES))
    { send_to_char("Invalid choice, try again : ", d->character);
      return;
    } else 
    { GET_OBJ_TYPE(OLC_OBJ(d)) = number;
    }
    break;

  case OEDIT_EXTRAS:
    number = atoi(arg);
    if ((number < 0) || (number > NUM_ITEM_FLAGS))
    { oedit_disp_extra_menu(d);
      return;
    } else {
      /* if 0, quit */
      if (number == 0)
    break;
      else  { /* if already set.. remove */
	if (IS_SET(GET_OBJ_EXTRA(OLC_OBJ(d)), 1 << (number - 1)))  
          REMOVE_BIT(GET_OBJ_EXTRA(OLC_OBJ(d)), 1 << (number - 1));
	else 
	  /* set */
	  SET_BIT(GET_OBJ_EXTRA(OLC_OBJ(d)), 1 << (number - 1));

	/*
	 * No matter what, is UNIQUE is set, ensure NOSAVE is set as well!
	 */
	if (IS_SET(GET_OBJ_EXTRA(OLC_OBJ(d)), ITEM_UNIQUE))
	  SET_BIT(GET_OBJ_EXTRA(OLC_OBJ(d)), ITEM_NOSAVE);

	oedit_disp_extra_menu(d);
        return;
      }
    }

  case OEDIT_WEAR:
    number = atoi(arg);
    if ((number < 0) || (number > NUM_ITEM_WEARS)) {
      send_to_char("That's not a valid choice!\r\n", d->character);
      oedit_disp_wear_menu(d);
      return;
    } else 
    { /* if 0, quit */
      if (number == 0)
    break;
      else 
      { /* if already set.. remove */
    if (IS_SET(GET_OBJ_WEAR(OLC_OBJ(d)), 1 << (number - 1)))
      REMOVE_BIT(GET_OBJ_WEAR(OLC_OBJ(d)), 1 << (number - 1));
    else
      SET_BIT(GET_OBJ_WEAR(OLC_OBJ(d)), 1 << (number - 1));
    oedit_disp_wear_menu(d);
        return;
      }
    }

  case OEDIT_WEIGHT:
    number = atoi(arg);
    GET_OBJ_WEIGHT(OLC_OBJ(d)) = number;
    break;

  case OEDIT_COST:
    number = atoi(arg);
    GET_OBJ_COST(OLC_OBJ(d)) = number;
    break;

  case OEDIT_COSTPERDAY:
    number = atoi(arg);
    GET_OBJ_QCOST(OLC_OBJ(d)) = number;
    break;

  case OEDIT_TIMER:
    number = atoi(arg);
    GET_OBJ_TIMER(OLC_OBJ(d)) = number;
    break;

  case OEDIT_LEVEL:
    /*. Object level flags on my mud...
    number = atoi(arg);
    GET_OBJ_LEVEL(OLC_OBJ(d)) = number;
    .*/
    break;

  case OEDIT_VALUE_1:
    number = atoi(arg);
/*
 * Need some sanity check for ter'angries here...
 */
    if (IS_SET(GET_OBJ_EXTRA(OLC_OBJ(d)),ITEM_TERANGREAL)) {
      if (number <= 0 || number >= MAX_SPELLS) {
        oedit_disp_val1_menu(d);
        return;
      }
    }

    switch (GET_OBJ_TYPE(OLC_OBJ(d))) {

    case ITEM_FURNITURE:
    if (number < 0 || number >= NUM_FURNITURE_TYPES) {
        furniture_type_menu(d);
        return;
      }
      else {
        GET_OBJ_VAL(OLC_OBJ(d), 0) = number;
        OLC_VAL(d) = 1;
        oedit_disp_val2_menu(d); 
        return;
      }
      break;

    default:
      GET_OBJ_VAL(OLC_OBJ(d), 0) = atoi(arg);
      oedit_disp_val2_menu(d);
    }

    oedit_disp_val2_menu(d);
    return;
  case OEDIT_VALUE_2:
    /* here, I do need to check for outofrange values */
    number = atoi(arg);
    switch (GET_OBJ_TYPE(OLC_OBJ(d))) 
    { 
      case ITEM_FURNITURE:
      if (number < 0 || number >= NUM_FURNITURE_USAGE_TYPES) {
       furniture_usage_type(d); 
       return;
      }
      else {
        GET_OBJ_VAL(OLC_OBJ(d), 1) = number;
        oedit_disp_val3_menu(d);
      }
      break;
      case ITEM_OTHER:
        if (IS_SET(GET_OBJ_EXTRA(OLC_OBJ(d)),ITEM_TERANGREAL)) {
          if (number < 1 || number > 199) {
            oedit_disp_val2_menu(d);
        return;
     
          } else {
            GET_OBJ_VAL(OLC_OBJ(d), 1) = number;
            oedit_disp_val3_menu(d);
            return;
          }
        }
        break;
      case ITEM_POTION:
    if (number < 0 || number >= NUM_SPELLS)
      oedit_disp_val2_menu(d);
    else {
      GET_OBJ_VAL(OLC_OBJ(d), 1) = number;
      oedit_disp_val3_menu(d);
    }
    break;
      case ITEM_WAGON:
      case ITEM_CONTAINER:
    /* needs some special handling since we are dealing with flag values
     * here */
    number = atoi(arg);
    if (number < 0 || number > 4)
      oedit_disp_container_flags_menu(d);
    else 
        { /* if 0, quit */
      if (number != 0)
      { number =  1 << (number - 1);
            if (IS_SET(GET_OBJ_VAL(OLC_OBJ(d), 1), number))
              REMOVE_BIT(GET_OBJ_VAL(OLC_OBJ(d), 1), number);
        else
              SET_BIT(GET_OBJ_VAL(OLC_OBJ(d), 1), number);
        oedit_disp_val2_menu(d);
      } else
        oedit_disp_val3_menu(d);
    }
    break;
      default:
    GET_OBJ_VAL(OLC_OBJ(d), 1) = number;
    oedit_disp_val3_menu(d);
    }
    return;

  case OEDIT_VALUE_3:
    number = atoi(arg);
    /*. Quick'n'easy error checking .*/
    switch (GET_OBJ_TYPE(OLC_OBJ(d))) 
    { 
      case ITEM_OTHER:
        if (IS_SET(GET_OBJ_EXTRA(OLC_OBJ(d)),ITEM_TERANGREAL)) {
          if (number < 0 || number > 12000) {
            oedit_disp_val3_menu(d);
        return;
      }
          else {
            GET_OBJ_VAL(OLC_OBJ(d),2) = number;
        oedit_disp_val4_menu(d);
        return;
          }
        }
        break;
      case ITEM_HEALING:
        min_val = 0;
        max_val = (GET_OBJ_VAL(OLC_OBJ(d), 1))? 100: 10000;
        break;
      case ITEM_WEAPON:
        min_val = 1;
        max_val = 50;
        break;
      case ITEM_FURNITURE:
      min_val = 1;
      max_val = 5;
      break;
      case ITEM_DRINKCON:
      case ITEM_FOUNTAIN:
        min_val = 0;
        max_val = NUM_LIQ_TYPES -1;
        break;
      default:
        min_val = -32000;
        max_val = 32000;
    }
    GET_OBJ_VAL(OLC_OBJ(d), 2) = MAX(min_val, MIN(number, max_val));
    oedit_disp_val4_menu(d);
    return;

  case OEDIT_VALUE_4:
    number = atoi(arg);
    switch (GET_OBJ_TYPE(OLC_OBJ(d))) {
    case ITEM_POTION:
      min_val = 0;
      max_val = NUM_SPELLS -1;
      break;
    case ITEM_WEAPON:
      min_val = 0;
      max_val = NUM_ATTACK_TYPES -1;
      break;
    default:
      min_val = -32000;
      max_val = 32000;
      break;
    }
    GET_OBJ_VAL(OLC_OBJ(d), 3) = MAX(min_val, MIN(number, max_val));
    break;

  case OEDIT_PROMPT_APPLY:
    number = atoi(arg);
    if (number == 0)
      break;
    else if (number < 0 || number > MAX_OBJ_AFFECT) 
    { oedit_disp_prompt_apply_menu(d);
      return;
    }
    OLC_VAL(d) = number - 1;
    OLC_MODE(d) = OEDIT_APPLY;
    oedit_disp_apply_menu(d);
    return;

  case OEDIT_APPLY:
    number = atoi(arg);
    if (number == 0) 
    { OLC_OBJ(d)->affected[OLC_VAL(d)].location = 0;
      OLC_OBJ(d)->affected[OLC_VAL(d)].modifier = 0;
      oedit_disp_prompt_apply_menu(d);
    } else if (number < 0 || number >= NUM_APPLIES) 
      oedit_disp_apply_menu(d);
    else 
    { OLC_OBJ(d)->affected[OLC_VAL(d)].location = number;
      send_to_char("Modifier : ", d->character);
      OLC_MODE(d) = OEDIT_APPLYMOD;
    }
    return;

  case OEDIT_APPLYMOD:
    number = atoi(arg);
    OLC_OBJ(d)->affected[OLC_VAL(d)].modifier = number;
    oedit_disp_prompt_apply_menu(d);
    return;

  case OEDIT_EXTRADESC_KEY:
    if (OLC_DESC(d)->keyword)
      free(OLC_DESC(d)->keyword);
    OLC_DESC(d)->keyword = str_dup(arg);
    oedit_disp_extradesc_menu(d);
    return;

/*-------------------------------------------------------------------*/
  case OEDIT_OBJPROG:
    switch (*arg) {
      case 'a':
      case 'A':
        OLC_MODE(d) = OEDIT_OP_ACTIONS;
        GET_OBJ_ACTLIST(d->character) = GET_OBJ_MAINLIST(d->character)->actions;        
        oedit_disp_actions(d);
        return;
      case 'd':
      case 'D':
        delete_objprog(GET_OBJ_MAINLIST(d->character), d);
        if (!GET_OBJPROG(OLC_OBJ(d))) {         /* Must never have an empty objproglist */
          CREATE(GET_OBJPROG(OLC_OBJ(d)), struct obj_prog_list, 1);
          GET_OBJPROG(OLC_OBJ(d))->next = NULL;
          GET_OBJPROG(OLC_OBJ(d))->actions = NULL;
          GET_OBJPROG(OLC_OBJ(d))->sideterms = NULL;
        }
        GET_OBJ_MAINLIST(d->character) = GET_OBJPROG(OLC_OBJ(d));
        oedit_disp_obj_program(d);
        return;
      case 'e':
      case 'E':
        for (present = GET_OBJPROG(OLC_OBJ(d)); present; present = present_next) {
          present_next = present->next;
          if (!present->actions || !present->main) 
            delete_objprog(present, d);
         }
        OLC_MODE(d) = OEDIT_MAIN_MENU;
        oedit_disp_menu(d);
        GET_OBJ_MAINLIST(d->character) = NULL;
        GET_OBJ_ACTLIST(d->character) = NULL;
        GET_SIDELIST(d->character) = NULL;
        OLC_VAL(d) = 1;
        break;  
      case 'm':
      case 'M':
        OLC_MODE(d) = OEDIT_OP_MAINTERMS;
        oedit_clear_actions(d);
        oedit_clear_sideterms(d);
        oedit_disp_mainterms(d);
        return;
      case 'n':
      case 'N':
        GET_OBJ_MAINLIST(d->character) = GET_OBJPROG(OLC_OBJ(d));
        i = 0;
        while (GET_OBJ_MAINLIST(d->character)->next) {  /* Step to last main program */
          GET_OBJ_MAINLIST(d->character) = GET_OBJ_MAINLIST(d->character)->next;  
          i++;
          if (i >= 29) {
             send_to_char("Program list is full, sorry.\r\n", d->character);
             return;
          }
        }
        CREATE(GET_OBJ_MAINLIST(d->character)->next, struct obj_prog_list, 1); /* Create new last */
        GET_OBJ_MAINLIST(d->character) = GET_OBJ_MAINLIST(d->character)->next; /* Go to new */
        GET_OBJ_MAINLIST(d->character)->actions = NULL;
        GET_OBJ_MAINLIST(d->character)->sideterms = NULL;
        oedit_disp_obj_program(d);
        return;
      case 'p':
      case 'P':
        if (GET_OBJPROG(OLC_OBJ(d)) != GET_OBJ_MAINLIST(d->character)) {
          for (present = GET_OBJPROG(OLC_OBJ(d)); 
            present->next != GET_OBJ_MAINLIST(d->character);
            present = present->next);
          GET_OBJ_MAINLIST(d->character) = present;
        } 
        else 
          send_to_char("Already at beginning of list.\r\n", d->character);
        oedit_disp_obj_program(d);        
        return;
      case 's':
      case 'S':
        i = GET_OBJ_MAINLIST(d->character)->main;
        GET_SIDELIST(d->character) = GET_OBJ_MAINLIST(d->character)->sideterms;
        OLC_MODE(d) = OEDIT_OP_SIDETERMS;
        oedit_disp_sideterms(d);
        return;
      case 'x':
      case 'X':
        if (GET_OBJ_MAINLIST(d->character)->next) {
          GET_OBJ_MAINLIST(d->character) = GET_OBJ_MAINLIST(d->character)->next;
          GET_SIDELIST(d->character) = GET_OBJ_MAINLIST(d->character)->sideterms;
          GET_OBJ_ACTLIST(d->character) = GET_OBJ_MAINLIST(d->character)->actions;
          oedit_disp_obj_program(d);
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
  case OEDIT_OP_MAINTERMS:
    i = atoi(arg);
    if (!i) {
      send_to_char("Invalid number, try again: ", d->character);
      return;
    }
    GET_OBJ_MAINLIST(d->character)->main = MAX(1, MIN(NUM_OBJ_MAINTERMS, i));
    if ((i == OPC_TIME) || (i == OPC_RANDOM) ||
        (i == OPC_ACTIONROOM) || (i == OPC_ACTIONTOROOM) ||
        (i == OPC_ACTIONWEAR) || (i == OPC_ACTIONTOWEAR) ||
        (i == OPC_ACTIONCARRY) || (i == OPC_ACTIONTOCARRY) ||
        (i == OPC_SAYROOM) || (i == OPC_SAYWEAR) ||
        (i == OPC_SAYCARRY)) {
      OLC_MODE(d) = OEDIT_OP_MAINTERMS2;
      send_to_char("Enter argument for selected main term: ", d->character);
    } else {
      GET_OBJ_MAINLIST(d->character)->argument = NULL;
      OLC_MODE(d) = OEDIT_OBJPROG;
      oedit_disp_obj_program(d);
    }
    return;
/*-------------------------------------------------------------------*/
  case OEDIT_OP_MAINTERMS2:
    i = GET_OBJ_MAINLIST(d->character)->main;
      
    if ((i == OPC_TIME) || (i == OPC_RANDOM)) {
      j = atoi(arg);
      if (!j) {
        send_to_char("Invalid number for this main term, please try again: ", d->character);
        return;
      }
      GET_OBJ_MAINLIST(d->character)->intarg = j;
      GET_OBJ_MAINLIST(d->character)->argument = strdup(arg);
    } else {
      if (!*arg)  {
        send_to_char("You must have an argument for this main term, try again: ", d->character);
        return;
      }
      GET_OBJ_MAINLIST(d->character)->intarg = 0;
      GET_OBJ_MAINLIST(d->character)->argument = strdup(arg);
    }
    OLC_MODE(d) = OEDIT_OBJPROG;
    oedit_disp_obj_program(d);
    return;
/*-------------------------------------------------------------------*/
  case OEDIT_OP_ACTIONS:
    switch (*arg) {
      case 'a':
      case 'A':
        OLC_MODE(d) = OEDIT_OP_ADDACTION1;
        GET_OBJ_ACTLIST(d->character) = GET_OBJ_MAINLIST(d->character)->actions;
        if (GET_OBJ_ACTLIST(d->character)) {
          while (GET_OBJ_ACTLIST(d->character)->next)
            GET_OBJ_ACTLIST(d->character) = GET_OBJ_ACTLIST(d->character)->next;
          CREATE(GET_OBJ_ACTLIST(d->character)->next, struct obj_action_list, 1);
          GET_OBJ_ACTLIST(d->character) = GET_OBJ_ACTLIST(d->character)->next;
        } else {
          CREATE(GET_OBJ_MAINLIST(d->character)->actions, struct obj_action_list, 1);
          GET_OBJ_ACTLIST(d->character) = GET_OBJ_MAINLIST(d->character)->actions;
          GET_OBJ_ACTLIST(d->character)->next = NULL;
        }
        oedit_disp_addaction(d);
      break;
      case 'c':
      case 'C':
        oedit_clear_actions(d);
        oedit_disp_actions(d);
      break;
      case 'e':
      case 'E':
        if (!GET_OBJ_ACTLIST(d->character)) {
          send_to_char("The list must contain at least one action.\r\n", d->character);
          return;
        }
        OLC_MODE(d) = OEDIT_OBJPROG;
        GET_OBJ_ACTLIST(d->character) = GET_OBJPROG(OLC_OBJ(d))->actions;        
        oedit_disp_obj_program(d);
      break;
      case 'r':
      case 'R':
        if (!GET_OBJ_ACTLIST(d->character)) 
          send_to_char("There are no actions to delete. \r\nEnter command: ", d->character);
        else  {
          one_argument(++arg,temp);
          if (!(i = atoi(temp))) 
            send_to_char("Remove which action?\r\nEnter command: ", d->character);
          else {
            for (GET_OBJ_ACTLIST(d->character) = GET_OBJ_MAINLIST(d->character)->actions, j = 1; 
            (j < i) && GET_OBJ_ACTLIST(d->character); 
            j ++, GET_OBJ_ACTLIST(d->character) = GET_OBJ_ACTLIST(d->character)->next);
            if ((i != j) || !GET_OBJ_ACTLIST(d->character))
              send_to_char("Wrong number entered. \r\nEnter command: ", d->character);
            else {
              remove_obj_action(d);
              oedit_disp_actions(d);              
            }
          }
        }
        GET_OBJ_ACTLIST(d->character) = GET_OBJ_MAINLIST(d->character)->actions;        
        return;
      default:
        send_to_char("Wrong command, try again: ", d->character);
      return;       
    }
    return;
/*--------------------------------------------------------------------------*/
  case OEDIT_OP_SIDETERMS:
    switch (*arg) {
      case 'a':
      case 'A':
        OLC_MODE(d) = OEDIT_OP_ADDSIDE1;
        GET_SIDELIST(d->character) = GET_OBJ_MAINLIST(d->character)->sideterms;
        if (GET_SIDELIST(d->character)) {
          while (GET_SIDELIST(d->character)->next)
            GET_SIDELIST(d->character) = GET_SIDELIST(d->character)->next;
          CREATE(GET_SIDELIST(d->character)->next, struct side_term_list, 1);
          GET_SIDELIST(d->character) = GET_SIDELIST(d->character)->next;
        } else {
          CREATE(GET_OBJ_MAINLIST(d->character)->sideterms, struct side_term_list, 1);
          GET_SIDELIST(d->character) = GET_OBJ_MAINLIST(d->character)->sideterms;
        }
        oedit_disp_addside(d);
        return;
      
      case 'c':
      case 'C':
        oedit_clear_sideterms(d);
        oedit_disp_sideterms(d);
        return;  
      case 'e':
      case 'E':
        OLC_MODE(d) = OEDIT_OBJPROG;
        GET_SIDELIST(d->character) = GET_OBJ_MAINLIST(d->character)->sideterms;
        oedit_disp_obj_program(d);
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
            for (GET_SIDELIST(d->character) = GET_OBJ_MAINLIST(d->character)->sideterms, j = 1; 
            (j < i) && GET_SIDELIST(d->character); 
            j ++, GET_SIDELIST(d->character) = GET_SIDELIST(d->character)->next);
            if ((i != j) || !GET_SIDELIST(d->character)) 
              send_to_char("Wrong number entered. \r\nEnter command: ", d->character);
            else {
              remove_obj_sideterm(d);
              oedit_disp_sideterms(d);              
            }
          }
        }
        GET_SIDELIST(d->character) = GET_OBJ_MAINLIST(d->character)->sideterms;        
        return;
      default:
        send_to_char("Wrong command, try again: ", d->character);
      return; 
    }
    return;
/*-------------------------------------------------------------------*/
  case OEDIT_OP_ADDACTION1:
    GET_OBJ_ACTLIST(d->character)->argument = NULL;
    GET_OBJ_ACTLIST(d->character)->next = NULL;
    i = atoi(arg);
    if ((i < 1) || (i > NUM_OBJ_ACTIONS)) {
      send_to_char("Invalid choice, try again:", d->character);
      return;
    }
    for (dum = GET_OBJ_MAINLIST(d->character)->actions; dum; dum = dum->next)
      if (dum->action == OPA_PAUSE)
        after_pause = TRUE;

    j = GET_OBJ_MAINLIST(d->character)->main;
    if  (((j == OPC_TIME) || (j == OPC_TICK) || (j == OPC_RANDOM) ||
          (after_pause)) &&  ((i == OPA_FORCE) || (i == OPA_STOP) || 
          (i == OPA_EMOTETO))) {
      send_to_char("That action is not allowed for that main term, try again: ", d->character);
      return;
    }    
    GET_OBJ_ACTLIST(d->character)->action = i;
    if ((i != OPA_STOP) && (i != OPA_DESTROY)) {
      OLC_MODE(d) = OEDIT_OP_ADDACTION2;
      send_to_char("Enter argument for selected action:", d->character);
    } else {
      OLC_MODE(d) = OEDIT_OP_ACTIONS;
      oedit_disp_actions(d);
    }
    return;
/*-------------------------------------------------------------------*/
  case OEDIT_OP_ADDACTION2:
    if (((GET_OBJ_ACTLIST(d->character)->action == OPA_GOTO) ||
         (GET_OBJ_ACTLIST(d->character)->action == OPA_WEARDOWN) ||
        (GET_OBJ_ACTLIST(d->character)->action == OPA_PAUSE)) && !atoi(arg)) {
       send_to_char("Argument must be a nonzero number, try again: ", d->character);
      return;
    }
    if (((GET_OBJ_ACTLIST(d->character)->action != OPA_DESTROY) &&
         (GET_OBJ_ACTLIST(d->character)->action != OPA_STOP)) && (!*arg)) {
      send_to_char("Argument can't be empty, try again: ", d->character);
      return;
    }   
    if ((GET_OBJ_ACTLIST(d->character)->action != OPA_STOP) && 
         (GET_OBJ_ACTLIST(d->character)->action != OPA_DESTROY)) {
      if ((GET_OBJ_ACTLIST(d->character)->action == OPA_GOTO) ||
         (GET_OBJ_ACTLIST(d->character)->action == OPA_WEARDOWN) ||
         (GET_OBJ_ACTLIST(d->character)->action == OPA_PAUSE)) {
        GET_OBJ_ACTLIST(d->character)->argument = NULL;
        GET_OBJ_ACTLIST(d->character)->intarg = atoi(arg);
      } else {
        strng = delete_doubledollar(arg);
        if (!obj_emotestring_ok(strng, GET_OBJ_MAINLIST(d->character))) {
           send_to_char("Arguments in string invalid for emote. Try again.: ", d->character);
          return;
        }
        GET_OBJ_ACTLIST(d->character)->argument = strdup(strng);
      }
    } else
      GET_OBJ_ACTLIST(d->character)->argument = NULL;
    GET_OBJ_ACTLIST(d->character) = GET_OBJ_MAINLIST(d->character)->actions;
    OLC_MODE(d) = OEDIT_OP_ACTIONS;
    oedit_disp_actions(d);
    return;
/*-------------------------------------------------------------------*/
  case OEDIT_OP_ADDSIDE1:
    GET_SIDELIST(d->character)->argument = NULL;
    GET_SIDELIST(d->character)->next = NULL;
    i = atoi(arg);
    if ((i < 1) || (i > NUM_OBJ_SIDETERMS)) {
      send_to_char("Invalid choice, try again:", d->character);
      return;
    }
    j = GET_OBJ_MAINLIST(d->character)->main;
    if (((j == OPC_TIME) || (j == OPC_TICK) || (j == OPC_RANDOM)) && 
        (i != OPC_STATUS)) {
      send_to_char("That side term can't be combined with the current main term, try again.\r\nEnter choice: ", d->character);
      return;
    }
    GET_SIDELIST(d->character)->number = i;
    if ((i == OPC_SEX) || (i == OPC_LEVEL) || (i == OPC_CARRYING) || 
        (i == OPC_WEARING) || (i == OPC_RACE) || (i == OPC_GUILD) ||
        (i == OPC_QUESTED) ||
        (i == OPC_STATUS) || (i == OPC_WEARINGN) || (i == OPC_CARRYINGN) ||
    (OPC_GRANKIS == i) || (OPC_GRANKLESS == i) || (OPC_GRANKMORE == i)) {     
      OLC_MODE(d) = OEDIT_OP_ADDSIDE2;
      send_to_char("Enter argument for selected side term:", d->character);
    } else {
      OLC_MODE(d) = OEDIT_OP_ADDNOT;
      send_to_char("Is the result true if the test of this term fails? (y/n):", d->character);
    }
    return;
/*-------------------------------------------------------------------*/
  case OEDIT_OP_ADDSIDE2:
    i = GET_SIDELIST(d->character)->number;
    if ((i == OPC_CARRYING) || (i == OPC_WEARING) || 
        (i == OPC_QUESTED) || (i == OPC_STATUS)) {
      if (!atoi(arg)) {
        send_to_char("Argument must be a nonzero number, try again: ", d->character);
        return;
      }
      GET_SIDELIST(d->character)->int_arg = atoi(arg);
      GET_SIDELIST(d->character)->argument = strdup(arg);
    } 
    else if ((i == OPC_SEX) || (i == OPC_RACE) || (i == OPC_GUILD) ||
         (i == OPC_WEARINGN) || (i == OPC_LEVEL) || (i == OPC_CARRYINGN) ||
     (OPC_GRANKIS == i) || (OPC_GRANKLESS == i) || (OPC_GRANKMORE == i)) {
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
    OLC_MODE(d) = OEDIT_OP_ADDNOT;
    send_to_char("Is the result true if the test of this term fails? (y/n): ", d->character);    
    return;
/*-------------------------------------------------------------------*/
  case OEDIT_OP_ADDNOT:
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
    OLC_MODE(d) = OEDIT_OP_SIDETERMS;
    oedit_disp_sideterms(d);
    return;
/*-------------------------------------------------------------------*/
  case OEDIT_EXTRADESC_MENU:
    number = atoi(arg);
    switch (number) {
    case 0:
      { /* if something got left out */
    if (!OLC_DESC(d)->keyword || !OLC_DESC(d)->description) 
        { struct extra_descr_data **tmp_desc;

      if (OLC_DESC(d)->keyword)
        free(OLC_DESC(d)->keyword);
      if (OLC_DESC(d)->description)
        free(OLC_DESC(d)->description);

          /*. Clean up pointers .*/
      for(tmp_desc = &(OLC_OBJ(d)->ex_description); *tmp_desc;
          tmp_desc = &((*tmp_desc)->next))
          { if (*tmp_desc == OLC_DESC(d))
            { *tmp_desc = NULL;
              break;
            }
          }
      free(OLC_DESC(d));
    }
      }
      break;
    case 1:
      OLC_MODE(d) = OEDIT_EXTRADESC_KEY;
      send_to_char("Enter keywords, separated by spaces :-\r\n| ", d->character);
      return;

    case 2:
      OLC_MODE(d) = OEDIT_EXTRADESC_DESCRIPTION;
      SEND_TO_Q("Enter the extra description: (/s saves /h for help)\r\n\r\n", d);
      d->backstr = NULL;
      if (OLC_DESC(d)->description) {
        SEND_TO_Q(OLC_DESC(d)->description, d);
        d->backstr = str_dup(OLC_DESC(d)->description);
      }
      d->str = &OLC_DESC(d)->description;
      d->max_str = MAX_MESSAGE_LENGTH;
      d->mail_to[0] = 0;
      OLC_VAL(d) = 1;
      d->character->desc->current_text_line = count_newlines(d->str) + 1;
      return;

    case 3:
      /*. Only go to the next descr if this one is finished .*/
      if (OLC_DESC(d)->keyword && OLC_DESC(d)->description)
      { struct extra_descr_data *new_extra;

    if (OLC_DESC(d)->next)
      OLC_DESC(d) = OLC_DESC(d)->next;
    else 
        { /* make new extra, attach at end */
      CREATE(new_extra, struct extra_descr_data, 1);

      OLC_DESC(d)->next = new_extra;
      OLC_DESC(d) = OLC_DESC(d)->next;
    }
      }
      /*. No break - drop into default case .*/
    default:
      oedit_disp_extradesc_menu(d);
      return;
    }
    break;
  default:
    mudlog("SYSERR: OLC: Reached default case in oedit_parse()!", BRF, LVL_BUILDER, TRUE);
    break;
  }

  /*. If we get here, we have changed something .*/
  OLC_VAL(d) = 1; /*. Has changed flag .*/
  oedit_disp_menu(d);
}
