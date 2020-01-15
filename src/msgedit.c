/******************************************************************************
 * $Id: msgedit.c,v 1.3 2003/08/18 10:44:24 fang Exp $
 *
 * File: messages.c
 *
 * Created: Fri Jun 20 2003 by Stefan Lindstrom (asmodean@ludd.luth.se)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada,
 * yada...
 *
 * Online editor for spell/fight-messages.
 *
 * $Log: msgedit.c,v $
 * Revision 1.3  2003/08/18 10:44:24  fang
 * Minor functionality to dynvars
 *
 * Revision 1.2  2003/06/20 09:46:21  fang
 *
 * Start code for a msg-editor in. Very basic, skeletal code for the first simple
 * step only.
 *
 * Revision 1.1  2003/06/20 08:03:58  fang
 * First commit
 *
 */
#include "msgedit.h"
#include "comm.h"

static const char *__msgeditUsage = "Usage: msgedit <type> <skill/weave-number>\r\n\r\n"
                                    "where type can be one of:\r\n"
                                    "\tfight: edit a fight message\r\n"
                                    "\top: edit a spell message for the One Power\r\n"
                                    "\ttp: edit a spell message for the True Power\r\n";

/*
 * Search thorugh the apropriate array for a message with skill num <num>
 * If no message for that num is found, return the first unused message pos. 
 */
static int msg_find_message(int type, int num, union message_list_ptr *rc)
{
  register int i;

  if (TYPE_MSG == type) {
    rc->f = fight_messages;

    for (i = 0; i < MAX_MESSAGES; i++) {
      if (rc->f[i].a_type && rc->f[i].a_type == num) {
	(rc->f) += i;
	return i;
      }
    }
  } else if (TYPE_SP_MSG == type) {
    rc->op = spell_messages;

    for (i = 0; i < MAX_SPELL_MESS; i++) {
      if (rc->op[i].a_type && rc->op[i].a_type == num) {
	(rc->op) += i;
	return i;
      }
    }
  } else if (TYPE_TP_MSG == type) {
    rc->tp = tp_messages;

    for (i = 0; i < MAX_SPELL_MESS; i++) {
      if (rc->tp[i].a_type && rc->tp[i].a_type == num) {
        (rc->tp) += i;
        return i;
      }
    }
  } else
    return -1;

  /*
   * No msg with that num existing,  find a new, unedited msg to fiddle with...
   */
  if (TYPE_MSG == type) {
    rc->f = fight_messages;

    for (i = 0; i < MAX_MESSAGES; i++) {
      if (!rc->f[i].a_type) {
	(rc->f) += i;
	return i;
      }
    }
  }  else if (TYPE_SP_MSG == type) {
    rc->op = spell_messages;

    for (i = 0; i < MAX_SPELL_MESS; i++) {
      if (!rc->op[i].a_type) {
        (rc->op) += i;
	return i;
      }
    }
  }else if (TYPE_TP_MSG == type) {
    rc->tp = tp_messages;

    for (i = 0; i < MAX_SPELL_MESS; i++) {
      if (!rc->tp[i].a_type) {
        (rc->tp) += i;
        return i;
      }
    }
  }
  
  return -1;
}

static void copy_tmsg_strings(struct tp_msg_type *msg, const struct tp_msg_type *src)
{
  if (src->weaver) 
    msg->weaver = strdup(src->weaver); 
  else 
    msg->weaver = strdup("-NO MESSAGE-");

  if (src->vict_tpu) 
    msg->vict_tpu = strdup(src->vict_tpu);
  else 
    msg->vict_tpu = strdup("-NO MESSAGE-");

  if (src->vict_ntp) 
    msg->vict_ntp = strdup(src->vict_ntp);
  else 
    msg->vict_ntp = strdup("-NO MESSAGE-");

  if (src->onl_tpu) 
    msg->onl_tpu = strdup(src->onl_tpu);
  else 
    msg->onl_tpu = strdup("-NO MESSAGE-");

  if (src->onl_npu) 
    msg->onl_npu = strdup(src->onl_npu);
  else 
    msg->onl_npu = strdup("-NO MESSAGE-");
}

static void copy_smsg_strings(struct sp_msg_type *msg, const struct sp_msg_type *src)
{
  if (src->weaver) 
    msg->weaver = strdup(src->weaver); 
  else 
    msg->weaver = strdup("-NO MESSAGE-");

  if (src->vict_nc) 
    msg->vict_nc = strdup(src->vict_nc); 
  else 
    msg->vict_nc = strdup("-NO MESSAGE-");

  if (src->vict_cos) 
    msg->vict_cos = strdup(src->vict_cos); 
  else 
    msg->vict_cos = strdup("-NO MESSAGE-");

  if (src->vict_css) 
    msg->vict_css = strdup(src->vict_css); 
  else 
    msg->vict_css = strdup("-NO MESSAGE-");

  if (src->onl_nc) 
    msg->onl_nc = strdup(src->onl_nc); 
  else 
    msg->onl_nc = strdup("-NO MESSAGE-");

  if (src->onl_cos) 
    msg->onl_cos = strdup(src->onl_cos); 
  else 
    msg->onl_cos = strdup("-NO MESSAGE-");

  if (src->onl_css) 
    msg->onl_css = strdup(src->onl_css); 
  else 
    msg->onl_css = strdup("-NO MESSAGE-");
}

static void copy_msg_strings(struct msg_type *msg, const struct msg_type *src)
{
  if (src->attacker_msg)
    msg->attacker_msg = strdup(src->attacker_msg);
  else
    msg->attacker_msg = strdup("-NO MESSAGE-");

  if (src->victim_msg)
    msg->victim_msg = strdup(src->victim_msg);
  else
    msg->victim_msg = strdup("-NO MESSAGE-");

  if (src->room_msg)
    msg->room_msg = strdup(src->room_msg);
  else
    msg->room_msg = strdup("-NO MESSAGE-");
}

static void copy_msgs(int type, union message_list_ptr *dest, union message_list_ptr *src)
{
  union message_list_ptr *t = src, d;

  if (TYPE_MSG == type) {
    for (; t->f->msg; t->f->msg = t->f->msg = t->f->msg->next) {
      d.f = dest->f;
      d.f->msg = calloc(1,sizeof(struct message_type));
      copy_msg_strings(&d.f->msg->die_msg,&t->f->msg->die_msg);
      copy_msg_strings(&d.f->msg->hit_msg,&t->f->msg->hit_msg);
      copy_msg_strings(&d.f->msg->miss_msg,&t->f->msg->miss_msg);
      copy_msg_strings(&d.f->msg->god_msg,&t->f->msg->god_msg);
      d.f->msg->next = dest->f->msg;
      dest->f->msg = d.f->msg;
    }
  }
  if (TYPE_SP_MSG == type) {
    for (; t->op->msg; t->op->msg = t->op->msg = t->op->msg->next) {
      d.op = dest->op;
      d.op->msg = calloc(1,sizeof(struct sp_message_type));
      copy_smsg_strings(&d.op->msg->die_msg,&t->op->msg->die_msg);
      copy_smsg_strings(&d.op->msg->hit_msg,&t->op->msg->hit_msg);
      copy_smsg_strings(&d.op->msg->miss_msg,&t->op->msg->miss_msg);
      copy_smsg_strings(&d.op->msg->god_msg,&t->op->msg->god_msg);
      d.op->msg->next = dest->op->msg;
      dest->op->msg = d.op->msg;
    }
  }
  if (TYPE_TP_MSG == type) {
    for (; t->tp->msg; t->tp->msg = t->tp->msg = t->tp->msg->next) {
      d.tp = dest->tp;
      d.tp->msg = calloc(1,sizeof(struct tp_message_type));
      copy_tmsg_strings(&d.tp->msg->die_msg,&t->tp->msg->die_msg);
      copy_tmsg_strings(&d.tp->msg->hit_msg,&t->tp->msg->hit_msg);
      copy_tmsg_strings(&d.tp->msg->miss_msg,&t->tp->msg->miss_msg);
      d.tp->msg->next = dest->tp->msg;
      dest->tp->msg = d.tp->msg;
    }
  }
}

ACMD(do_msgedit)
{
  char type[2048], number[2048];
  int num, itype = -1, pos;
  union message_list_ptr search;

  if (!*argument) {
    send_to_char(__msgeditUsage, ch);
    return;
  }

  two_arguments(argument, type, number);
  
  if (!*type || !*number) {
    send_to_char(__msgeditUsage, ch);
    return;
  }

  num = atoi(number);

  if (!strcasecmp(type,"fight"))
    itype = TYPE_MSG;
  else if (!strcasecmp(type,"op"))
    itype = TYPE_SP_MSG;
  else if (!strcasecmp(type,"tp"))
    itype = TYPE_TP_MSG;
  else {
    send_to_char(__msgeditUsage, ch);
    return;
  }

  pos = msg_find_message(itype,num, &search);
  if (-1 == pos) {
    asend_to_char(ch, "Could neither find a message for skill/weave %d to edit, nor an "
		  "empty position to use!\r\n", num);
    return;
  }

  ch->desc->msged = calloc(1,sizeof(struct msg_edit_data));
  ch->desc->msged->mode = MSG_MENU;
  ch->desc->msged->type = itype;
  ch->desc->msged->num  = pos;
  ch->desc->msged->list = search;
  if (TYPE_MSG == itype) {
    ch->desc->msged->list.f = calloc(1,sizeof(struct message_list));
    *(ch->desc->msged->list.f) = *(search.f);
    if (!ch->desc->msged->list.f->a_type || 
	!ch->desc->msged->list.f->number_of_attacks ||
	!ch->desc->msged->list.f->msg) {
      ch->desc->msged->message.f = search.f->msg;
    } else {
      copy_msgs(itype, &ch->desc->msged->list, &search);
    }
  }
  if (TYPE_SP_MSG == itype) {
    ch->desc->msged->list.op = calloc(1,sizeof(struct sp_message_list));
    *(ch->desc->msged->list.op) = *(search.op);
    ch->desc->msged->message.op = search.op->msg;
  }
  if (TYPE_TP_MSG == itype) {
    ch->desc->msged->list.tp = calloc(1,sizeof(struct tp_message_list));
    *(ch->desc->msged->list.tp) = *(search.tp);
    ch->desc->msged->message.tp = search.tp->msg;
  }

  msg_display_menu(ch);
  /* GET_STATE(ch) = CON_MSGEDIT; */
}

static void display_f(struct char_data *ch)
{
  asend_to_char(ch,"%c[H%c[J",(char)27, (char)27);
  asend_to_char(ch,"Type: Fight, Skill: %d, Num Msgs: #%d\r\n", 
		ch->desc->msged->list.f->a_type, ch->desc->msged->list.f->number_of_attacks);

  asend_to_char(ch,"1) Die message      2) Miss message    3) Hit message\r\n");
  asend_to_char(ch,"4) God message      5) Goto next message for skill (%s)\r\n",
                (ch->desc->msged->message.tp->next ? "SET" : "NOT SET"));
 asend_to_char(ch,"6) Quit\r\n\r\n");
}

static void display_op(struct char_data *ch)
{
  asend_to_char(ch,"%c[H%c[J",(char)27, (char)27);
  asend_to_char(ch,"Type: OP-Spell, Skill: %d, Num Msgs: #%d\r\n",
                ch->desc->msged->list.op->a_type, ch->desc->msged->list.op->number_of_attacks);

  asend_to_char(ch,"1) Die message      2) Miss message    3) Hit message\r\n");
  asend_to_char(ch,"4) God message      5) Goto next message for weave (%s)\r\n",
                (ch->desc->msged->message.op->next ? "SET" : "NOT SET"));
  asend_to_char(ch,"6) Quit\r\n\r\n");
}

static void display_tp(struct char_data *ch)
{
  asend_to_char(ch,"%c[H%c[J",(char)27, (char)27);
  asend_to_char(ch,"Type: TP-Spell, Skill: %d, Num Msgs: #%d\r\n",
                ch->desc->msged->list.tp->a_type, ch->desc->msged->list.tp->number_of_attacks);

  asend_to_char(ch,"1) Die message      2) Miss message    3) Hit message\r\n");
  asend_to_char(ch,"5) Goto next message for weave (%s)\r\n",
		(ch->desc->msged->message.tp->next ? "SET" : "NOT SET"));
  asend_to_char(ch,"6) Quit\r\n\r\n");
}

void msg_display_menu(struct char_data *ch)
{
  if (TYPE_MSG == ch->desc->msged->type) 
    display_f(ch);
  else if (TYPE_SP_MSG == ch->desc->msged->type)
    display_op(ch);
  else if (TYPE_TP_MSG == ch->desc->msged->type)
    display_tp(ch);

  asend_to_char(ch, "Enter your choice: ");
}

