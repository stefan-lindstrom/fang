/******************************************************************************
 * $Id: messages.h,v 1.4 2003/06/20 07:49:55 fang Exp $
 *
 * File: messages.h
 *
 * Created: Tue Jun 17 2003 by Stefan Lindstrom (asmodean@ludd.luth.se)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * Collecting all messages (message, sp_message, tp_message) under one set of
 * files, enabling xml-storage of messages. 
 *
 * $Log: messages.h,v $
 * Revision 1.4  2003/06/20 07:49:55  fang
 *
 * db.c now loads xml-messages instead of old message files. Old code for message
 * loading have been removed.
 *
 * Revision 1.3  2003/06/19 13:09:13  fang
 *
 * Code for loading xml-messages added. Also, reload options to reload messages
 * from xml.-files added. Soon time to do the switch.
 *
 * Revision 1.2  2003/06/17 13:02:37  fang
 *
 * Added a single file for messages. Code for now contains only functionalty to
 * save messages (fight/spell op/spell tp) as XML. All structs, and data, and
 * defines pertaining to messages have been moved to the new header.
 *
 * Revision 1.1  2003/06/17 11:00:00  fang
 * first commit
 *
 */
#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include "interpreter.h"

#define MAX_MESSAGES            100
#define MAX_SPELL_MESS          100  /* number of spell-messages */

#define MESS_FILE_XML   "misc/messages.xml"    /* damage messages           */
#define SPELL_FILE_XML  "misc/sp_messages.xml" /*spell messages             */
#define TP_FILE_XML     "misc/tp_messages.xml" /* TP - messages!            */

#define TYPE_MSG    0
#define TYPE_SP_MSG 1
#define TYPE_TP_MSG 2

/* Structs for TP-messages! */
struct tp_msg_type
{
  char *weaver;
  char *vict_tpu;
  char *vict_ntp;
  char *onl_tpu;
  char *onl_npu;
};

struct tp_message_type
{  
  struct tp_msg_type die_msg; 
  struct tp_msg_type miss_msg;
  struct tp_msg_type hit_msg;
  struct tp_message_type *next;
};

struct tp_message_list
{
  long a_type;
  long number_of_attacks;
  struct tp_message_type *msg;
}; 

struct sp_msg_type
{ 
  char *weaver;
  char *vict_nc;
  char *vict_cos;       /* Channeler Opposite Sex */
  char *vict_css;
  char *onl_nc;
  char *onl_cos;
  char *onl_css;
};

struct sp_message_type {
  struct sp_msg_type die_msg;  /* messages when death                  */
  struct sp_msg_type miss_msg; /* messages when miss                   */
  struct sp_msg_type hit_msg;  /* messages when hit                    */
  struct sp_msg_type god_msg;  /* messages when hit on god             */
  struct sp_message_type *next;        /* to next messages of this kind.       */
};
  
struct sp_message_list {
  long a_type;                 /* Attack type                          */
  long number_of_attacks;      /* How many attack messages to chose from. */
  struct sp_message_type *msg; /* List of messages.                    */
};
   
struct msg_type {
  char *attacker_msg;  /* message to attacker */
  char *victim_msg;    /* message to victim   */
  char *room_msg;      /* message to room     */
};

struct message_type {
  struct msg_type die_msg;     /* messages when death                  */
  struct msg_type miss_msg;    /* messages when miss                   */
  struct msg_type hit_msg;     /* messages when hit                    */
  struct msg_type god_msg;     /* messages when hit on god             */
  struct message_type *next;   /* to next messages of this kind.       */
};

struct message_list {
  long a_type;                 /* Attack type                          */
  long number_of_attacks;      /* How many attack messages to chose from. */
  struct message_type *msg;    /* List of messages.                    */
};

extern struct message_list fight_messages[MAX_MESSAGES];       /* fighting messages */
extern struct sp_message_list spell_messages[MAX_SPELL_MESS];
extern struct tp_message_list tp_messages[MAX_SPELL_MESS];

extern int save_messages_xml(int type);
extern int load_messages_xml(int type);
extern const char *msg_error(int error);
extern void free_fight_msg(struct msg_type *m);
extern void free_sp_msg(struct sp_msg_type *m);
extern void free_tp_msg(struct tp_msg_type *m);

extern ACMD(save_msgs);

#endif
