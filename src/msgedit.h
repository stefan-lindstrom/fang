/******************************************************************************
 * $Id: msgedit.h,v 1.2 2003/06/20 09:46:21 fang Exp $
 *
 * File: messages.h
 *
 * Created: Fri Jun 20 2003 by Stefan Lindstrom (asmodean@ludd.luth.se)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * Online editor for spell/fight-messages.
 *
 * $Log: msgedit.h,v $
 * Revision 1.2  2003/06/20 09:46:21  fang
 *
 * Start code for a msg-editor in. Very basic, skeletal code for the first simple
 * step only.
 *
 * Revision 1.1  2003/06/20 08:03:58  fang
 * First commit
 *
 */
#ifndef __MSGEDIT_H__
#define __MSGEDIT_H__

#include "messages.h"
#include "interpreter.h"
#include "structs.h"

#define MSG_MENU 0

union msg_ptr {
  struct msg_type    *f;
  struct sp_msg_type *op;
  struct tp_msg_type *tp;
};

union message_ptr {
  struct message_type    *f;
  struct sp_message_type *op;
  struct tp_message_type *tp;
};

union message_list_ptr {
  struct message_list    *f;
  struct sp_message_list *op;
  struct tp_message_list *tp;
};

struct msg_edit_data {
  int mode;
  int type; /* type of msg edited, fight, op or tp */
  int num;  /* numer (pos) in message array currently edited, if nescessary */
  union message_list_ptr list;
  union message_ptr      message;
  union msg_ptr          msg;
};


extern ACMD(do_msgedit);

extern void msg_display_menu(struct char_data *ch);

#endif
