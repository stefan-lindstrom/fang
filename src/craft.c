/******************************************************************************
 * $Id: craft.c,v 1.10 2003/12/16 19:26:38 fang Exp $
 *   
 * File: craft.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: craft.c,v $
 *   Revision 1.10  2003/12/16 19:26:38  fang
 *   Tiek: Changed the check in craft to >= instead of >
 *
 *   Revision 1.9  2003/02/02 14:48:49  fang
 *   Extra descs added to tool as well.
 *
 *   Revision 1.8  2003/01/27 17:06:31  fang
 *   All should work with exdescs and craft now.
 *
 *   Revision 1.7  2003/01/25 16:47:14  fang
 *   Extra descs in craft completed. For tool  is yet to be done. The code can be cleaner tho'
 *
 *   Revision 1.6  2002/11/07 04:02:01  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.5  2002/10/08 17:30:36  fang
 *   fischer: set minimum craft cost to 1
 *
 *   Revision 1.4  2002/09/15 00:50:08  fang
 *   Drath: XLOG update - Categories added to xlog display automatically
 *
 *   Revision 1.3  2002/08/30 19:52:28  fang
 *   Drath: New room DG trigger, LEAVE
 *
 *   Revision 1.2  2002/07/28 13:56:04  fang
 *   Removed dmlloc...just to test.
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.9  2002/06/25 19:23:51  fang
 *   Drath: XLOG Commit #2
 *
 *   Revision 1.8  2002/06/18 15:56:01  fang
 *   Maz: Logging for TOOL & CRAFT in Online Syslog.
 *
 *   Revision 1.7  2002/05/27 10:52:48  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.6  2002/03/24 03:01:15  fang
 *   Drathus: Fixed CRAFT so Nose/Ear EQ cannot have stats.
 *
 *   Revision 1.5  2002/03/12 01:01:12  fang
 *   *** empty log message ***
 *
 *   Revision 1.4  2002/02/28 17:42:47  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/02/28 17:11:03  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.14  2002/01/31 14:21:04  mud
 *   Craft can only crate erar/nose eq of type OTHER; and without any affections.
 *
 *   Revision 1.13  2002/01/24 21:33:14  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.12.2.1  2002/01/23 21:49:40  mud
 *   Removed objsave.[c|h] (old objsave stuff).
 *   Removed obj_file_elem and rent structs.
 *   Removed final vestiges of old guild system, except parts used
 *   in guild.[c|h] for backwards compatibility.
 *
 *   Revision 1.12  2002/01/13 20:07:32  mud
 *   In craft when setting container capacity, now prompts for capacity, not AC
 *
 *   Revision 1.11  2002/01/02 01:10:43  mud
 *   Added some logging messages to tooling code.
 *
 *   Revision 1.10  2001/11/07 09:38:27  mud
 *   *** empty log message ***
 *
 *   Revision 1.9  2000/12/03 11:03:41  mud
 *   Some startup work for editable exdescs in craft/tool done
 *
 *   Revision 1.8  2000/12/03 10:26:10  mud
 *   More QEQ tracking, begun some minro mods to have exdescs editable by craft/tool.
 *
 *   Revision 1.7  2000/08/14 19:11:55  mud
 *   The new guild system.
 *
 *   Revision 1.6  2000/07/05 09:52:37  mud
 *   Changed max craft cost to 10k.
 *
 *   Revision 1.5  2000/06/25 16:10:26  mud
 *   Fixed craft limitations.
 *
 *   Revision 1.4  2000/06/25 14:53:43  mud
 *   Gavin: Squished tooling and craft bugs, as well as changing the craft
 *   prices to adhere to current builder prices.
 *
 *   Revision 1.3  2000/03/22 06:57:06  mud
 *   Minor modification to show_tool_obj, increased buffer size for tooling mail.
 *
 *   Revision 1.2  2000/02/21 14:00:16  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#include "handler.h"
#include "constants.h"
#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "interpreter.h"
#include "comm.h"
#include "utils.h"
#include "db.h"
#include "screen.h"
#include "craft.h"
#include "spells.h"
#include "guild.h"
#include "boards.h"

void show_tool_obj(struct char_data *ch);
void parse_tool_command(struct char_data *ch,char *arg);

int allowed_craft_types[10] = {
  ITEM_LIGHT,
  ITEM_WEAPON,
  ITEM_BOW,
  ITEM_ARROW,
  ITEM_ARMOR,
  ITEM_CONTAINER,
  ITEM_NOTE,
  ITEM_PEN,
  ITEM_BOAT,
  ITEM_OTHER
};

int allowed_craft_affects[12] = {
  APPLY_STR,
  APPLY_DEX,
  APPLY_INT,
  APPLY_WIS,
  APPLY_CON,
  APPLY_CHA,
  APPLY_ABS,
  APPLY_HIT,
  APPLY_MOVE,
  APPLY_AC,
  APPLY_HITROLL,
  APPLY_DAMROLL
};

long craft_calc_cost(struct char_data *ch)
{
  struct obj_data *obj = GET_CRAFT_OBJ(ch);
  int i, apply, mod;
  long cost = 0;

  switch (GET_OBJ_TYPE(obj)) {
    case ITEM_ARROW: /* Just so they won't come for free */
      cost +=10;
    break;
  }

  for (i = 0; i < MAX_OBJ_AFFECT; i++) { 
    apply = obj->affected[i].location;         
    mod = obj->affected[i].modifier;
    if (!apply)
      continue;
    
    if (apply <= APPLY_CHA)
      cost += mod*1000;
    else if (apply == APPLY_ABS)
      cost += mod*200;
    else if (apply == APPLY_AC)
      cost += (-mod)*100;
    else if ((apply == APPLY_MANA) || (apply == APPLY_HIT) || (apply == APPLY_MOVE))
      cost += mod*100;
    else if ((apply == APPLY_HITROLL) || (apply == APPLY_DAMROLL))
      cost += mod*750;
  }
  switch (GET_OBJ_TYPE(obj)) {
    case ITEM_ARMOR:
      cost += (100*GET_OBJ_VAL(obj, 0) + 200*GET_OBJ_VAL(obj, 1));
    break;
    case ITEM_BOW:
    case ITEM_WEAPON:
      cost += 100*GET_OBJ_VAL(obj, 1)*GET_OBJ_VAL(obj, 2);
    break;
    case ITEM_CONTAINER:
      cost += 2*GET_OBJ_VAL(obj, 0);
    break;
    case ITEM_LIGHT:
      cost += GET_OBJ_VAL(obj, 2);
    break;
  }
  if( cost<= 0 ) { cost = 1; }
  return cost;
}

ACMD(do_tool)
{
  struct obj_data *obj;

  one_argument(argument,arg);

  if (IS_NPC(ch)) {
    send_to_char("Mobs can't tool, smeghead!\r\n",ch);
    return;
  }
  if (!GET_SKILL(ch,SKILL_TOOL) && !IS_AFFECTED(ch, AFF_DREAMING)) {
    send_to_char("You don't know how!\r\n",ch);
    return;
  }
  if (!*arg) {
    send_to_char("Tool what?\r\n",ch);
    return;
  }
  if (!(obj = get_obj_in_list_vis(ch,arg,ch->carrying))) {
    send_to_char("You ain`t carrying that!\r\n",ch);
    return;
  }
   
  if (IS_OBJ_STAT(obj, ITEM_NOTOOL) && GET_LEVEL(ch)<LVL_GROOVYGL ){
   send_to_char("You're not allowed to tool that item.\r\n", ch);  
    return;
  }
   
  SET_BIT(PLR_FLAGS(ch), PLR_WRITING);
  GET_CRAFT_OBJ(ch)   = obj;
  STATE(ch->desc)     = CON_TOOL;
  GET_CRAFT_STATE(ch) = TOOL_MENU;
  GET_CRAFT_EDESC(ch) = NULL;
  obj->ex_description = NULL; /* Null out exdescs */
  show_tool_obj(ch);
  act("$n starts to tool on $p.\r\n", TRUE, ch, obj, 0, TO_ROOM);
  alog("%s starts to tool on %s [%s].", GET_NAME(ch), obj->short_description, obj->name);
}
 

void parse_tool(struct char_data *ch,char *arg)
{
  struct obj_data *obj = GET_CRAFT_OBJ(ch);
  struct extra_descr_data *desc = GET_CRAFT_EDESC(ch);

  switch (GET_CRAFT_STATE(ch)) {
  case TOOL_PAUSE:
    break;
  case TOOL_QUIT:
    STATE(ch->desc) = CON_PLAYING;
    REMOVE_BIT(PLR_FLAGS(ch), PLR_WRITING);
    save_char(ch,ch->in_room);

    act("$n is done tooling.", TRUE, ch, 0, 0, TO_ROOM);

    sprintf(buf, "%s has tooled %s &g(%ld)", GET_NAME(ch), obj->short_description, GET_OBJ_VNUM(obj));
    xlog(SYS_TOOLCRAFT, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), buf, TRUE);

    break;
  case TOOL_MENU:
    parse_tool_command(ch,arg);
    break;
  case TOOL_NAMELIST:
    obj->name = str_dup(arg);
    REMOVE_BIT(GET_OBJ_EXTRA(obj),ITEM_ADDED);
    show_tool_obj(ch);
    GET_CRAFT_STATE(ch) = TOOL_MENU;
    break;
  case TOOL_DESC:
    obj->description = str_dup(arg);
    show_tool_obj(ch);
    GET_CRAFT_STATE(ch) = TOOL_MENU;
    break;
  case TOOL_SDESC:
    obj->short_description = str_dup(arg);
    show_tool_obj(ch);
    GET_CRAFT_STATE(ch) = TOOL_MENU;
      break;
  case TOOL_ADESC:
    obj->action_description = str_dup(arg);
    show_tool_obj(ch);
    GET_CRAFT_STATE(ch) = TOOL_MENU;
    break;
  case TOOL_EDESC:
    tool_edesc_parse(ch,arg);
    break;
  case TOOL_EDESCK:
      if (desc->keyword)
        free(desc->keyword);
    
      desc->keyword=strdup(arg);
      GET_CRAFT_STATE(ch) = TOOL_EDESC;
      show_tool_edesc_menu(ch,desc);

      break;
 default:
    send_to_char("Not implemented yet, be patient.\r\n",ch);
    GET_CRAFT_STATE(ch) = TOOL_QUIT;
    break;
  }
}

void show_tool_obj(struct char_data *ch)
{
  struct obj_data *tmp = GET_CRAFT_OBJ(ch);
  char tbuf[MAX_STRING_LENGTH];

/*  send_to_char("[H[J", ch); */
  if (!tmp->name)
    sprintf(tbuf,"(\\c02A\\c00) Namelist:\r\n");
  else
    sprintf(tbuf," (\\c02A\\c00) Namelist: %s\r\n",tmp->name);
  send_to_char(tbuf,ch);

  if (!tmp->description)
    sprintf(tbuf,"(\\c02B\\c00) Description:\r\n| \r\n");
  else
    sprintf(tbuf," (\\c02B\\c00) Description:\r\n| %s\r\n",tmp->description);
  send_to_char(tbuf,ch);

  if (!tmp->short_description)
    sprintf(tbuf," (\\c02C\\c00) Short description:\r\n|\r\n");
  else
    sprintf(tbuf," (\\c02C\\c00) Short description:\r\n| %s\r\n\r\n",tmp->short_description);
  send_to_char(tbuf,ch);

  if (!tmp->action_description)
    sprintf(tbuf," (\\c02D\\c00) Action description:\r\n|\r\n\r\n");
  else
    sprintf(tbuf," (\\c02D\\c00) Action description:\r\n| %s\r\n\r\n",tmp->action_description);
  send_to_char(tbuf,ch);

  send_to_char("(\\c02E\\c00) Extra description\r\n\r\n",ch);

  send_to_char("\r\n\\c02Q\\c00 to quit.\r\n Choice: ",ch);
}

void parse_tool_command(struct char_data *ch,char *arg)
{
  if (!*arg) {
    GET_CRAFT_STATE(ch) = TOOL_QUIT;
    return;
  }
  switch (arg[0]) {
    case 'A' :
    case 'a' :
      GET_CRAFT_STATE(ch) = TOOL_NAMELIST;
      send_to_char("New namelist: ",ch);
      break;
    case 'B' :
    case 'b' :
      GET_CRAFT_STATE(ch) = TOOL_DESC;
      send_to_char("New description: ", ch);
      break;
    case 'C' :
    case 'c' :
      GET_CRAFT_STATE(ch) = TOOL_SDESC;
      send_to_char("New short description: ", ch);
      break;
    case 'D' :
    case 'd' :
      GET_CRAFT_STATE(ch) = TOOL_ADESC;
      send_to_char("New action description: ", ch);
      break;
    case 'E':
    case 'e':
      GET_CRAFT_STATE(ch) = TOOL_EDESC;
      show_tool_edesc_menu(ch,GET_CRAFT_EDESC(ch));
      break;
    case 'Q':
    case 'q':
   /*
    *  STATE(ch->desc) = CON_PLAYING;
    *  REMOVE_BIT(PLR_FLAGS(ch),PLR_WRITING);
    *  save_char(ch,ch->in_room);
    */
      GET_CRAFT_STATE(ch) = TOOL_QUIT;
      parse_tool(ch,"");
      return;
    default:
      send_to_char("That is not a valid option!\r\n Choice: ", ch);
      break;
  }
}

void show_tool_edesc_menu(struct char_data *ch, struct extra_descr_data *foo)
{
  char keyword[MAX_INPUT_LENGTH], desc[MAX_INPUT_LENGTH], next[MAX_INPUT_LENGTH];
  struct extra_descr_data *xdesc = GET_CRAFT_EDESC(ch);
  
  if (NULL != xdesc) {
    sprintf(keyword, "&y%s&n", xdesc->keyword);
    sprintf(desc, "\r\n&y%s&n", xdesc->description);
    if (xdesc->next != NULL)
      sprintf(next, "&ySet&n");
    else
      sprintf(next, "&yNot Set&n");
  } else {
    GET_CRAFT_EDESC(ch) = calloc(1,sizeof(struct extra_descr_data));
    GET_CRAFT_EDESC(ch)->keyword     = strdup("&y<NONE>&n");
    GET_CRAFT_EDESC(ch)->description = strdup("&y<NONE>&n\r\n");
    GET_CRAFT_EDESC(ch)->next =        NULL;
  
    if (NULL == GET_CRAFT_OBJ(ch)->ex_description)
          GET_CRAFT_OBJ(ch)->ex_description = GET_CRAFT_EDESC(ch);
    
    sprintf(keyword, "&y<NONE>&n");
    sprintf(desc, "\r\n&y<NONE>&n");
    sprintf(next, "&yNot Set&n");  
  } 
    
  sprintf(buf, "&nExtra Description Menu\r\n"
               "\r\n"
               "&g1&w) Keywords: %s\r\n"
               "&g2&w) Description: %s\r\n"
               "&g3&w) Next: %s\r\n"
               "\r\n"
               "&g0&w) Exit\r\n"
               "\r\n"
               "&nEnter selection: "
               , keyword, desc, next);
    
  send_to_char(buf, ch);
}


void tool_edesc_parse(struct char_data *ch,char *buf)
{
  switch(atoi(buf)) {
    case 0:
      GET_CRAFT_STATE(ch) = TOOL_MENU;
      GET_CRAFT_EDESC(ch) = GET_CRAFT_OBJ(ch)->ex_description;
      show_tool_obj(ch);
      break;
    case 1:
      send_to_char("Enter Keywords: ", ch);
      GET_CRAFT_STATE(ch)=TOOL_EDESCK;
      break;
    case 2:
      SEND_TO_Q("Enter extra description: (/s saves /h for help)\r\n\r\n", ch->desc);
      SEND_TO_Q("Please format your desc to less than 75 characters per line.\r\n",ch->desc);
      SEND_TO_Q("This will make your desc easier to read for those with smaller screens\r\n",ch->desc);
      SEND_TO_Q("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n",ch->desc);
      
      ch->desc->backstr = NULL;
      if (GET_CRAFT_EDESC(ch)->description) {
        SEND_TO_Q(GET_CRAFT_EDESC(ch)->description, ch->desc);
        ch->desc->backstr = str_dup(GET_CRAFT_EDESC(ch)->description);
      }
      ch->desc->str        = &GET_CRAFT_EDESC(ch)->description;
      ch->desc->max_str    = MAX_MESSAGE_LENGTH;
      ch->desc->mail_to[0] = 0;
      ch->desc->current_text_line = count_newlines(ch->desc->str) + 1;
      
      GET_CRAFT_STATE(ch)=TOOL_EDESC;
      break;
    case 3:
      if (NULL == GET_CRAFT_EDESC(ch)->next) {
        GET_CRAFT_EDESC(ch)->next = calloc(1,sizeof(struct extra_descr_data));
        GET_CRAFT_EDESC(ch)->next->keyword     = strdup("Not set.");  
        GET_CRAFT_EDESC(ch)->next->description = strdup("Not set.\r\n");
        GET_CRAFT_EDESC(ch)->next->next = NULL;
        if (NULL == GET_CRAFT_OBJ(ch)->ex_description)
          GET_CRAFT_OBJ(ch)->ex_description = GET_CRAFT_EDESC(ch);
      }
      GET_CRAFT_EDESC(ch) = GET_CRAFT_EDESC(ch)->next;
      GET_CRAFT_STATE(ch)=TOOL_EDESC;
      show_tool_edesc_menu(ch,GET_CRAFT_EDESC(ch));
      break;
    default:
     GET_CRAFT_STATE(ch)=CRAFT_EDESC;
     show_tool_edesc_menu(ch,GET_CRAFT_EDESC(ch));
/*
 * We can get here, if entering numbers that are "out of range"
 * so let's just stay in the EDESC menu!
 */
      break;
  }   
}

void parse_craft(struct descriptor_data *d,char *arg)
{
  struct char_data *ch = d->character;
  struct obj_data *obj = GET_CRAFT_OBJ(ch);
  struct extra_descr_data *desc = GET_CRAFT_EDESC(ch);

  char lbuf[MAX_STRING_LENGTH];
  byte show_menu = TRUE, type = 0;
  int i = 0;

  skip_spaces(&arg);
  sprintf(lbuf, arg);

  if (!obj ||  !ch || IS_NPC(ch)) {
    d->connected = CON_PLAYING;
    return;
  }
  type = GET_OBJ_TYPE(obj);

  switch (GET_CRAFT_STATE(ch)) {
    case CRAFT_MENU:
      if (!craft_parse_menu(lbuf, ch))
        send_to_char("Try again: ", ch);
      show_menu = FALSE;
      break;
    case CRAFT_KEYWORD:
      if (strlen(lbuf) < 2) {
        show_menu = FALSE;        
        send_to_char("Too short keyword list, try again.\r\n Keyword: ", ch);
        return;
      }
      obj->name = strdup(lbuf);
    break;
    case CRAFT_SDESC:
      if (strlen(lbuf) < 2) {
        show_menu = FALSE;        
        send_to_char("-Too- short short description, try again.\r\n Short description: ", ch);
        return;
      }
      obj->short_description = strdup(lbuf);
      break;
    case CRAFT_DESC:
      if (strlen(lbuf) < 2) {
        show_menu = FALSE;        
        send_to_char("Too short description, try again.\r\nDescription: ", ch);
        return;
      }
      obj->description = strdup(lbuf);
    break;
    case CRAFT_TYPE:
      if ((atoi(lbuf) < 0) || (atoi(lbuf) > 9)) {
        show_menu = FALSE;
        GET_OBJ_TYPE(obj) = 0;
        send_to_char("Try again: ", ch);
        return;
      }
      
      if (allowed_craft_types[atoi(lbuf)] != ITEM_OTHER && 
      (CAN_WEAR(obj,ITEM_WEAR_EAR) || CAN_WEAR(obj,ITEM_WEAR_NOSE))) {
    show_menu = FALSE;
        GET_OBJ_TYPE(obj) = 0;
        send_to_char("Ear/Nose EQ can only be of type OTHER!\r\nTry again: ",ch);
    return;
      }

      GET_OBJ_VAL(obj, 0) = 0;  /* To avoid confusions, clear values */
      GET_OBJ_VAL(obj, 1) = 0;
      GET_OBJ_VAL(obj, 2) = 0;
      GET_OBJ_VAL(obj, 3) = 0;

      for (i = 0; i < MAX_OBJ_AFFECT; i++) {
    obj->affected[i].location = 0; 
    obj->affected[i].modifier = 0;
      }

      GET_OBJ_TYPE(obj) = allowed_craft_types[atoi(lbuf)];
    break;
    case CRAFT_WEAR:
      if ((atoi(lbuf) < 0) || (atoi(lbuf) > NUM_ITEM_WEARS)) {
        send_to_char("Illegal wear spot, try again: ", ch);
        return;
      }
      if (!atoi(lbuf))   
        GET_CRAFT_STATE(ch) = CRAFT_MENU;
      else      
      { /* if already set.. remove */
        if (IS_SET(GET_OBJ_WEAR(obj), 1 << (atoi(lbuf) - 1)))
          REMOVE_BIT(GET_OBJ_WEAR(obj), 1 << (atoi(lbuf) - 1));
        else
          SET_BIT(GET_OBJ_WEAR(obj), 1 << (atoi(lbuf) - 1));

    if (CAN_WEAR(obj,ITEM_WEAR_EAR) || CAN_WEAR(obj,ITEM_WEAR_NOSE)) {
      if (GET_OBJ_TYPE(obj) != ITEM_OTHER) {
        GET_OBJ_TYPE(obj) = ITEM_OTHER;
        GET_OBJ_VAL(obj, 0) = 0;  /* To avoid confusions, clear values */
        GET_OBJ_VAL(obj, 1) = 0;
        GET_OBJ_VAL(obj, 2) = 0;
        GET_OBJ_VAL(obj, 3) = 0;
      }
      for (i = 0; i < MAX_OBJ_AFFECT; i++) {
        obj->affected[i].location = 0; 
        obj->affected[i].modifier = 0;
      }
    }
        craft_show_wearbits(ch);
        return;
      }
    break;
    case CRAFT_AFFECT1:
      if ((atoi(lbuf) > 5) || (atoi(lbuf) < 0)) {
        send_to_char("Wrong number, try again: ", ch);
        return;
      }
      GET_CRAFT_TYPE(ch) = atoi(lbuf);
      craft_show_affects(ch);
      GET_CRAFT_STATE(ch) = CRAFT_AFFECT2;
      show_menu = FALSE;
    break;
    case CRAFT_AFFECT2:
      if (!atoi(lbuf))
        break;
      if ((atoi(lbuf) > 12) || (atoi(lbuf) < 1)) {
        send_to_char("That's not a valid affection, try again: ", ch);
        return;
      } 
      for (i = 0; i < MAX_OBJ_AFFECT; i ++) {
        if ( i == GET_CRAFT_TYPE(ch)) 
          continue;
        if (obj->affected[i].location == allowed_craft_affects[atoi(lbuf)-1]) {
          send_to_char("Sorry, only allowed to have one affection of the \r\n", ch);
          send_to_char("same type in the same object.\r\n", ch);
          send_to_char("Try again: ", ch);
          return;
        }
      } 
      obj->affected[GET_CRAFT_TYPE(ch)].location = allowed_craft_affects[atoi(lbuf)-1];
      GET_CRAFT_STATE(ch) = CRAFT_AFFECT3;
      send_to_char("By how much should the object affect the wearer?\r\nAmount: ", ch);
      show_menu = FALSE;
    break;
    case CRAFT_AFFECT3:
      obj->affected[GET_CRAFT_TYPE(ch)].modifier = atoi(lbuf);
      craft_parse_affect3(ch, lbuf);
    break;
    case CRAFT_REMOVE:
      if ((atoi(lbuf) > 6) || (atoi(lbuf) < 1)) {
        send_to_char("Number must be between 1 and 6. Try again: ", ch);
        show_menu = FALSE;
      }
      else {
        obj->affected[atoi(lbuf)-1].location = 0;
        obj->affected[atoi(lbuf)-1].modifier = 0;
      }  
    break;
    case CRAFT_VALUE1:
      GET_OBJ_VAL(obj, 0) = atoi(lbuf);
      limit_objval1(ch);
      if (type == ITEM_ARMOR) {
        send_to_char("Enter desired absorbance of armor: ", ch);
        GET_CRAFT_STATE(ch) = CRAFT_VALUE2;
        return;
      }    
    break;
    case CRAFT_VALUE2:
      GET_OBJ_VAL(obj, 1) = atoi(lbuf);
      limit_objval2(ch);
      if ((type == ITEM_WEAPON) || (type == ITEM_BOW)) {
        GET_CRAFT_STATE(ch) = CRAFT_VALUE3;
        send_to_char("Enter size of damage dice: ", ch);
        return;  
      }
    break;
    case CRAFT_VALUE3:
      GET_OBJ_VAL(obj, 2) = atoi(lbuf);
      limit_objval3(ch);
    break;
    case CRAFT_VALUE4:
      GET_OBJ_VAL(obj, 3) = atoi(lbuf);
      limit_objval4(ch);
      break;
    case CRAFT_EDESC:
//      show_craft_edesc_menu(ch,desc);
      craft_edesc_parse(ch,lbuf);      
      show_menu = FALSE;
      break;
    case CRAFT_EDESCK:
      if (desc->keyword)
        free(desc->keyword);

      desc->keyword=strdup(lbuf);
      GET_CRAFT_STATE(ch) = CRAFT_EDESC;
      show_menu = FALSE;
      show_craft_edesc_menu(ch,desc);

      break;
  }

  if (show_menu) {
    GET_CRAFT_STATE(ch) = CRAFT_MENU;
    craft_disp_menu(ch);
  }
}

ACMD(do_craft)
{
  struct obj_data *obj = NULL;
  struct extra_descr_data *xdesc;

  skip_spaces(&argument);

  if (IS_NPC(ch)) {
    send_to_char("Go away. Mobs can't do this.\r\n", ch);
    return;
  }
  if (!GET_SKILL(ch, SKILL_CRAFT)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }
  if ((GET_CLASS(ch) != CLASS_CRAFTSMAN) && !get_guild(ch,9) && (GET_LEVEL(ch) < LVL_IMMORT)) {
    send_to_char("You're no craftsman....how did you get this skill?\r\n", ch);
    return;   
  }
  if (!(obj = read_object(CRAFT_OBJ_VNUM, VIRTUAL))) {
    send_to_char("Sorry, Craft is out of order at the moment.\r\n", ch);
    return;
  }
  act("$n begins crafting a new object.", TRUE, ch, 0, 0, TO_ROOM);
  STATE(ch->desc) = CON_CRAFT;
  GET_CRAFT_STATE(ch) = CRAFT_MENU;
  GET_CRAFT_OBJ(ch) = obj;
  GET_CRAFT_EDESC(ch) = NULL;
  obj->ex_description = NULL;
  xdesc = NULL;

  SET_BIT(PLR_FLAGS(ch), PLR_WRITING);
  craft_disp_menu(ch);
}

void craft_edesc_parse(struct char_data *ch,char *buf)
{
  switch(atoi(buf)) {
    case 0:
      GET_CRAFT_STATE(ch)=CRAFT_MENU;
      GET_CRAFT_EDESC(ch) = GET_CRAFT_OBJ(ch)->ex_description;
      craft_disp_menu(ch);
      break;
    case 1:
      send_to_char("Enter Keywords: ", ch);
      GET_CRAFT_STATE(ch)=CRAFT_EDESCK;
      break;
    case 2:
      SEND_TO_Q("Enter extra description: (/s saves /h for help)\r\n\r\n", ch->desc);
      SEND_TO_Q("Please format your desc to less than 75 characters per line.\r\n",ch->desc);
      SEND_TO_Q("This will make your desc easier to read for those with smaller screens\r\n",ch->desc);
      SEND_TO_Q("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n",ch->desc);

      ch->desc->backstr = NULL;
      if (GET_CRAFT_EDESC(ch)->description) {
        SEND_TO_Q(GET_CRAFT_EDESC(ch)->description, ch->desc);
        ch->desc->backstr = str_dup(GET_CRAFT_EDESC(ch)->description);
      }
      ch->desc->str        = &GET_CRAFT_EDESC(ch)->description;
      ch->desc->max_str    = MAX_MESSAGE_LENGTH;
      ch->desc->mail_to[0] = 0;
      ch->desc->current_text_line = count_newlines(ch->desc->str) + 1;

      GET_CRAFT_STATE(ch)=CRAFT_EDESC;
      break;
    case 3:
      if (NULL == GET_CRAFT_EDESC(ch)->next) {
        GET_CRAFT_EDESC(ch)->next = calloc(1,sizeof(struct extra_descr_data));
        GET_CRAFT_EDESC(ch)->next->keyword     = strdup("Not set.");
        GET_CRAFT_EDESC(ch)->next->description = strdup("Not set.\r\n");
        GET_CRAFT_EDESC(ch)->next->next = NULL;
        if (NULL == GET_CRAFT_OBJ(ch)->ex_description)
          GET_CRAFT_OBJ(ch)->ex_description = GET_CRAFT_EDESC(ch);
      }
      GET_CRAFT_EDESC(ch) = GET_CRAFT_EDESC(ch)->next;
      GET_CRAFT_STATE(ch)=CRAFT_EDESC;
      show_craft_edesc_menu(ch,GET_CRAFT_EDESC(ch));
      break;
    default:
     GET_CRAFT_STATE(ch)=CRAFT_EDESC;
     show_craft_edesc_menu(ch,GET_CRAFT_EDESC(ch));
/*
 * We can get here, if entering numbers that are "out of range"
 * so let's just stay in the EDESC menu!
 */
      break;
  }
}

void show_craft_edesc_menu(struct char_data *ch, struct extra_descr_data *foo)
{
  char keyword[MAX_INPUT_LENGTH], desc[MAX_INPUT_LENGTH], next[MAX_INPUT_LENGTH];  
  struct extra_descr_data *xdesc = GET_CRAFT_EDESC(ch);

  if (NULL != xdesc) {
    sprintf(keyword, "&y%s&n", xdesc->keyword);
    sprintf(desc, "\r\n&y%s&n", xdesc->description);
    if (xdesc->next != NULL)
      sprintf(next, "&ySet&n");
    else
      sprintf(next, "&yNot Set&n");
  } else {
    GET_CRAFT_EDESC(ch) = calloc(1,sizeof(struct extra_descr_data));
    GET_CRAFT_EDESC(ch)->keyword     = strdup("&y<NONE>&n");
    GET_CRAFT_EDESC(ch)->description = strdup("&y<NONE>&n\r\n");
    GET_CRAFT_EDESC(ch)->next =        NULL;

    if (NULL == GET_CRAFT_OBJ(ch)->ex_description)
          GET_CRAFT_OBJ(ch)->ex_description = GET_CRAFT_EDESC(ch);

    sprintf(keyword, "&y<NONE>&n");
    sprintf(desc, "\r\n&y<NONE>&n");
    sprintf(next, "&yNot Set&n");
  }

  sprintf(buf, "&nExtra Description Menu\r\n"
               "\r\n"
               "&g1&w) Keywords: %s\r\n"
               "&g2&w) Description: %s\r\n"
               "&g3&w) Next: %s\r\n"
               "\r\n"
               "&g0&w) Exit\r\n"
               "\r\n"
               "&nEnter selection: "
               , keyword, desc, next);

  send_to_char(buf, ch);
}

void craft_show_affects(struct char_data *ch)
{
  int i;

  for (i = 0; i < 12; i++) {
    asend_to_char(ch, "%2d) %-20.20s ",i + 1, apply_types[allowed_craft_affects[i]]);
    if (!((i+1) % 2))
      send_to_char("\r\n", ch);
  }
  asend_to_char(ch,  "Enter desired apply type (0 to bail out): ");
}

void craft_show_wearbits(struct char_data *ch)
{
  int i;
  char lbuf[MAX_INPUT_LENGTH];

  struct obj_data *obj = GET_CRAFT_OBJ(ch);

  for (i = 0; i < NUM_ITEM_WEARS; i++) {
    asend_to_char(ch, "%2d) %-20.20s ",i + 1, wear_bits[i]);
    if (!((i+1) % 2))
      send_to_char("\r\n", ch);
  }
  sprintbit(GET_OBJ_WEAR(obj), wear_bits, lbuf);
  asend_to_char(ch,  "\r\nWear flags: %s\r\nEnter wear flag, 0 to quit : ",lbuf);
}

int craft_parse_menu(char *arg, struct char_data *ch)
{
  int i;
  struct obj_data *obj = GET_CRAFT_OBJ(ch);

  switch (atoi(arg)) {
    case 1:
      send_to_char("Enter keywords :", ch);
      GET_CRAFT_STATE(ch) = CRAFT_KEYWORD;
    break;
    case 2: 
      send_to_char("Enter short description :", ch);
      GET_CRAFT_STATE(ch) = CRAFT_SDESC;
    break;   
    case 3: 
      send_to_char("Enter description :", ch);
      GET_CRAFT_STATE(ch) = CRAFT_DESC;
    break;   
    case 4:
      for (i = 0; i < 10; i ++) {
        asend_to_char(ch, "%2d) %-20.20s", i, item_types[allowed_craft_types[i]]);
        if ((i+1)%2)
          send_to_char("\r\n", ch);
      }      
      send_to_char("\r\nEnter object type : ", ch);
      GET_CRAFT_STATE(ch) = CRAFT_TYPE;
    break;
    case 5:
      craft_show_wearbits(ch);
      GET_CRAFT_STATE(ch) = CRAFT_WEAR;
    break;
    case 6:
      send_to_char("Enter number (0-5) of affect to change: ", ch);
      GET_CRAFT_STATE(ch) = CRAFT_AFFECT1;
    break;
    case 7:
      GET_OBJ_VAL(obj, 0) = 0;
      GET_OBJ_VAL(obj, 1) = 0;
      GET_OBJ_VAL(obj, 2) = 0;
      GET_OBJ_VAL(obj, 3) = 0;
      
      if ((GET_OBJ_TYPE(obj) == ITEM_WEAPON)||(GET_OBJ_TYPE(obj) == ITEM_BOW)) {
        GET_CRAFT_STATE(ch) = CRAFT_VALUE2;
        send_to_char("Enter number of damage dice: ", ch);
      }
      else if (GET_OBJ_TYPE(obj) == ITEM_LIGHT) {
        send_to_char("Enter required numbers of hours to burn: ", ch);
        GET_CRAFT_STATE(ch) = CRAFT_VALUE3;
      }
      else if (GET_OBJ_TYPE(obj) == ITEM_ARMOR) {
        send_to_char("Enter desired AC modifier: ", ch);
        GET_CRAFT_STATE(ch) = CRAFT_VALUE1;
      }
      else if (GET_OBJ_TYPE(obj) == ITEM_CONTAINER) {
        send_to_char("Enter desired capacity of container: ", ch);
        GET_CRAFT_STATE(ch) = CRAFT_VALUE1;
      }
    break;
    case 8:
      if (craft_check_obj(ch)) {
	 if (GET_GOLD(ch) >= craft_calc_cost(ch)) {
        
        /* CRAFT isn't checking to see if there are stats for
             * NOSE/EAR eq upon saving, just when the wear flag is
             * set.  So this code is added here to remove any stats
             * before it's saved.
             */

        if (CAN_WEAR(obj,ITEM_WEAR_EAR) || CAN_WEAR(obj,ITEM_WEAR_NOSE)) {
          if (GET_OBJ_TYPE(obj) != ITEM_OTHER) {
            GET_OBJ_TYPE(obj) = ITEM_OTHER;
            GET_OBJ_VAL(obj, 0) = 0;  /* To avoid confusions, clear values */
            GET_OBJ_VAL(obj, 1) = 0;
            GET_OBJ_VAL(obj, 2) = 0;
            GET_OBJ_VAL(obj, 3) = 0;
          }
          for (i = 0; i < MAX_OBJ_AFFECT; i++) {
            obj->affected[i].location = 0; 
            obj->affected[i].modifier = 0;
          }
        }


          obj_to_char(obj, ch);
          GET_GOLD(ch) -= craft_calc_cost(ch);
        }
        else {
          asend_to_char(ch, "You couldn't afford the %ld gold it cost to craft the object.\r\n", craft_calc_cost(ch));
          extract_obj(obj);
        }
        act("$n has finished crafting. Wonder what $e made?", TRUE, ch, 0, 0, TO_ROOM);
        ch->desc->connected = CON_PLAYING;
        GET_CRAFT_STATE(ch) = 0;
        REMOVE_BIT(PLR_FLAGS(ch), PLR_WRITING); 
    GET_CRAFT_OBJ(ch) = NULL;
        sprintf(buf, "%s has created %s &g(%ld)", GET_NAME(ch), obj->short_description, GET_OBJ_VNUM(obj));
        xlog(SYS_TOOLCRAFT, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), buf, TRUE);
        return 1; 
      } else {
        send_to_char("The object is too expensive. Please reduce some stats on it.\r\n", ch);
        GET_CRAFT_STATE(ch) = CRAFT_MENU;  
      }
    break;
    case 10:
      //Adding Extra Descs to Craft... DRATHUS.
      //send_to_char("Hey! Din't you read onna menu, not implemented yet!\r\n",ch);
      //craft_disp_menu(ch);
      //GET_CRAFT_STATE(ch) = CRAFT_MENU;
      obj = GET_CRAFT_OBJ(ch);
      show_craft_edesc_menu(ch,GET_CRAFT_EDESC(ch));
      GET_CRAFT_STATE(ch) = CRAFT_EDESC;
      break;
    case 9:
      extract_obj(GET_CRAFT_OBJ(ch));
      GET_CRAFT_STATE(ch) = 0;
      ch->desc->connected = CON_PLAYING;
      send_to_char("Ok, you abort your crafting.\r\n", ch);
      REMOVE_BIT(PLR_FLAGS(ch), PLR_WRITING);
      GET_CRAFT_OBJ(ch) = NULL;
      return 0;
    break; 
    default:  
      return 0;
    break;
  }
 return 1;
}

int craft_parse_type(struct char_data *ch)
{
  int i;

  for (i = 0; i < 10; i++)
    if (GET_OBJ_TYPE(GET_CRAFT_OBJ(ch)) == allowed_craft_types[i])
      return 1;
  return 0;
}

void craft_parse_affect3(struct char_data *ch,char *buf)
{
  int nr = atoi(buf);

  struct obj_data *obj = GET_CRAFT_OBJ(ch);

  switch (obj->affected[GET_CRAFT_TYPE(ch)].location) {
    case APPLY_STR:
    case APPLY_DEX:
    case APPLY_INT:
    case APPLY_WIS:
    case APPLY_CON:
    case APPLY_CHA:
      obj->affected[GET_CRAFT_TYPE(ch)].modifier = MAX(0, MIN(2, nr));
    break;
    case APPLY_HIT:
    case APPLY_MANA:
    case APPLY_MOVE:
      obj->affected[GET_CRAFT_TYPE(ch)].modifier = MAX(0, MIN(50, nr));
    break;
    case APPLY_AC:
      if (nr > 0)
    send_to_char("Armour applies should be negative!\r\n",ch);

      obj->affected[GET_CRAFT_TYPE(ch)].modifier = MIN(0, MAX(-50, nr));
    break;
    case APPLY_ABS:
      obj->affected[GET_CRAFT_TYPE(ch)].modifier = MAX(0, MIN(50, nr));
    break;
    case APPLY_HITROLL:
    case APPLY_DAMROLL:
      obj->affected[GET_CRAFT_TYPE(ch)].modifier = MAX(0, MIN(25, nr));
    break;    
    default:
      obj->affected[GET_CRAFT_TYPE(ch)].modifier = 0;
    break;
  }
}

int craft_check_obj(struct char_data *ch)
{
  if (craft_calc_cost(ch) <= 10000)
    return 1;
  return 0;
}

void craft_disp_menu(struct char_data *ch)
{
  struct obj_data  *obj = NULL;
  char lbuf[MAX_STRING_LENGTH];
  long v[4], i;

  if (!ch || IS_NPC(ch) || !GET_CRAFT_OBJ(ch)) {
    GET_CRAFT_STATE(ch) = 0;
    ch->desc->connected = CON_PLAYING;
    GET_CRAFT_OBJ(ch) = NULL;
    return;
  }
  obj = GET_CRAFT_OBJ(ch);

  for (i = 0; i < 4; i++)
    v[i] = GET_OBJ_VAL(obj, i);
 
  asend_to_char(ch, "1) Keywords: \\c02%s\\c00\r\n",obj->name);
  asend_to_char(ch, "2) Short description: \\c02%s\\c00\r\n", obj->short_description);
  asend_to_char(ch, "3) Description: \\c02%s\\c00\r\n", obj->description);
  sprinttype(GET_OBJ_TYPE(obj), item_types, lbuf);
  asend_to_char(ch, "4) Type: \\c02%s\\c00\r\n", lbuf);
  sprintbit(obj->obj_flags.wear_flags, wear_bits, lbuf);
  asend_to_char(ch, "5) Wear flags: \\c02%s\\c00\r\n", lbuf);
  send_to_char("6) Affection when worn:\r\n", ch);
  for (i = 0; i < MAX_OBJ_AFFECT; i++) {
    sprinttype(obj->affected[i].location, apply_types, lbuf);
    asend_to_char(ch, "\\c02  %d) %+ld to %s\\c00\r\n", i, obj->affected[i].modifier, lbuf);
  }
  asend_to_char(ch, "Cost to craft: \\c02%d coins\\c00\r\n", craft_calc_cost(ch));
  if ((GET_OBJ_TYPE(obj) == ITEM_WEAPON) || (GET_OBJ_TYPE(obj) == ITEM_BOW))
    asend_to_char(ch, "7) Weapon damage: \\c02%ldd%ld\\c00\r\n", v[1], v[2]);
  if (GET_OBJ_TYPE(obj) == ITEM_LIGHT)
    asend_to_char(ch, "7) Hours left to burn: \\c02%ld\\c00\r\n", v[2]);
  if (GET_OBJ_TYPE(obj) == ITEM_ARMOR)
    asend_to_char(ch, "7) AC modifier: \\c02%+ld\\c00 Absorbance: \\c02%ld\\c00\r\n", v[0], v[1]);
  if (GET_OBJ_TYPE(obj) == ITEM_CONTAINER)
    asend_to_char(ch, "7) Containers capacity: \\c02%ld\\c00\r\n", v[0]);

  asend_to_char(ch,"10) Add extra description.\r\n");
 
  send_to_char("8) Exit and create object.\r\n", ch);
  send_to_char("9) Exit without creating object.\r\n\r\n", ch);
  send_to_char("Enter number: ", ch);
}

void limit_objval1(struct char_data *ch)
{
  struct obj_data *obj = GET_CRAFT_OBJ(ch);
  int val = GET_OBJ_VAL(obj, 0), type = GET_OBJ_TYPE(obj);
  
  if (type == ITEM_CONTAINER)  /* Can't contain more than 50 pounds */
    GET_OBJ_VAL(obj, 0) = MAX(0, MIN(50, val));
  if (type == ITEM_ARMOR) /* Can't give more than 15 AC */
    GET_OBJ_VAL(obj, 0) = MAX(0, MIN(15, val));
}

void limit_objval2(struct char_data *ch)
{
  struct obj_data *obj = GET_CRAFT_OBJ(ch);
  int val = GET_OBJ_VAL(obj, 1), type = GET_OBJ_TYPE(obj);

  if (type == ITEM_ARMOR)   /* Can't have higher than 15 abs */
    GET_OBJ_VAL(obj, 1) = MAX(0, MIN(15, val));
  if ((type == ITEM_WEAPON) || (type == ITEM_BOW))
    GET_OBJ_VAL(obj, 1) = MAX(0, MIN(6, val));
}

void limit_objval3(struct char_data *ch)
{
  struct obj_data *obj = GET_CRAFT_OBJ(ch);
  int val = GET_OBJ_VAL(obj, 2), type = GET_OBJ_TYPE(obj);

  if (type == ITEM_LIGHT)   /* Can't burn longer than 400 hrs */
    GET_OBJ_VAL(obj, 2) = MAX(0, MIN(400, val));
  if ((type == ITEM_WEAPON) || (type == ITEM_BOW))
    GET_OBJ_VAL(obj, 2) = MAX(0, MIN(6, val));
}

void limit_objval4(struct char_data *ch)
{
   /* Nothing in here yet */
}


