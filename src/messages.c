/******************************************************************************
 * $Id: messages.c,v 1.3 2003/06/19 13:09:13 fang Exp $
 *
 * File: messages.c
 *
 * Created: Tue Jun 17 2003 by Stefan Lindstrom (asmodean@ludd.luth.se)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * Collecting all messages (message, sp_message, tp_message) under one set of
 * files, enabling xml-storage of messages. 
 *
 * $Log: messages.c,v $
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
#include <stddef.h>
#include <errno.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/entities.h>
#include <libxml/parserInternals.h>
#include <stdio.h>

#include "messages.h"
#include "comm.h"
#include "db.h"
#include "utils.h"

struct message_list fight_messages[MAX_MESSAGES];       /* fighting messages */
struct sp_message_list spell_messages[MAX_SPELL_MESS];
struct tp_message_list tp_messages[MAX_SPELL_MESS];

static void msg_type_to_xml(struct msg_type *mt, xmlNodePtr mtype)
{
  if (mt->attacker_msg)
    xmlNewChild(mtype, NULL, "attacker", mt->attacker_msg);
  if (mt->victim_msg)
    xmlNewChild(mtype, NULL, "victim", mt->victim_msg);
  if (mt->room_msg)
    xmlNewChild(mtype, NULL, "room", mt->room_msg);
}

static void sp_msg_type_to_xml(struct sp_msg_type *mt, xmlNodePtr mtype)
{
  if (mt->weaver)
    xmlNewChild(mtype, NULL, "weaver", mt->weaver);
  if (mt->vict_nc) 
    xmlNewChild(mtype, NULL, "notchann", mt->vict_nc);
  if (mt->vict_cos) 
    xmlNewChild(mtype, NULL, "oppsexchann", mt->vict_cos);       /* Channeler Opposite Sex */
  if (mt->vict_css) 
    xmlNewChild(mtype, NULL, "samesexchann", mt->vict_css);
  if (mt->onl_nc) 
    xmlNewChild(mtype, NULL, "notchannroom", mt->onl_nc);
  if (mt->onl_cos) 
    xmlNewChild(mtype, NULL, "oppsexchannroom", mt->onl_cos);
  if (mt->onl_css) 
    xmlNewChild(mtype, NULL, "samesexchannroom", mt->onl_css);
}

static void tp_msg_type_to_xml(struct tp_msg_type *mt, xmlNodePtr mtype)
{
  
  if (mt->weaver)
    xmlNewChild(mtype, NULL, "weave", mt->weaver);
  if (mt->vict_tpu) 
    xmlNewChild(mtype, NULL, "tpuser", mt->vict_tpu);
  if (mt->vict_ntp) 
    xmlNewChild(mtype, NULL, "nottpuser", mt->vict_ntp);
  if (mt->onl_tpu) 
    xmlNewChild(mtype, NULL, "tpuserroom", mt->onl_tpu);
  if (mt->onl_npu) 
    xmlNewChild(mtype, NULL, "nottpuserroom", mt->onl_npu);
}

static int save_fight_msgs(void)
{
  xmlNodePtr tree, msg, mc, mtype;
  xmlDocPtr doc;
  FILE *fl;
  int i, j;
  char buf[500];
  struct message_type *sp;
  struct msg_type *mt;

  if (NULL == (fl = fopen(MESS_FILE_XML,"w")))
    return 2;

  doc = xmlNewDoc("1.0");
  
  tree = xmlNewDocNode(doc,NULL,"messages",NULL);
  doc->children = tree;

  for (i = 0; i < MAX_MESSAGES; i++) {
    if (fight_messages[i].a_type) { // type == 0 -> no message!
      msg = xmlNewNode(NULL, "message");
      msg->doc = doc;

      sprintf(buf, "%ld", fight_messages[i].a_type);
      xmlNewProp(msg,"type", buf);

      sprintf(buf,"%ld", fight_messages[i].number_of_attacks);
      xmlNewProp(msg,"nummsg", buf);

      sp = fight_messages[i].msg;
      for (j = 0; j < fight_messages[i].number_of_attacks; j++) {
	mc = xmlNewNode(NULL, "msg");
	mc->doc = doc;
       
	mtype = xmlNewNode(NULL, "die");
	mtype->doc = doc;
	mt = &sp->die_msg;
	msg_type_to_xml(mt, mtype);
	xmlAddChild(mc, mtype);

	mtype = xmlNewNode(NULL, "miss");
	mtype->doc = doc;
        mt = &sp->miss_msg;
        msg_type_to_xml(mt, mtype);
        xmlAddChild(mc, mtype);

        mtype = xmlNewNode(NULL, "hit");
	mtype->doc = doc;
        mt = &sp->hit_msg;
        msg_type_to_xml(mt, mtype);
        xmlAddChild(mc, mtype);

        mtype = xmlNewNode(NULL, "god");
	mtype->doc = doc;
        mt = &sp->god_msg;
        msg_type_to_xml(mt, mtype);
        xmlAddChild(mc, mtype);

        xmlAddChild(msg,mc);	
	sp = sp->next;
      }
      xmlAddChild(tree, msg);
    }
  }

  xmlDocDump(fl,doc);
  xmlFreeDoc(doc);
  fclose(fl);

  return 0;
}

static int save_sp_msgs(void)
{
  xmlNodePtr tree, msg, mc, mtype;
  xmlDocPtr doc;
  FILE *fl;
  int i, j;
  char buf[500];
  struct sp_message_type *sp;
  struct sp_msg_type *mt;

  if (NULL == (fl = fopen(SPELL_FILE_XML,"w")))
    return 3;

  doc = xmlNewDoc("1.0");

  tree = xmlNewDocNode(doc,NULL,"spmessages",NULL);
  doc->children = tree;

  for (i = 0; i < MAX_SPELL_MESS; i++) {
    if (spell_messages[i].a_type) { // type == 0 -> no message!
      msg = xmlNewNode(NULL, "message");
      msg->doc = doc;

      sprintf(buf, "%ld", spell_messages[i].a_type);
      xmlNewProp(msg,"type", buf);

      sprintf(buf,"%ld", spell_messages[i].number_of_attacks);
      xmlNewProp(msg,"nummsg", buf);

      sp = spell_messages[i].msg;
      for (j = 0; j < spell_messages[i].number_of_attacks; j++) {
        mc = xmlNewNode(NULL, "msg");
        mc->doc = doc;

        mtype = xmlNewNode(NULL, "die");
        mtype->doc = doc;
        mt = &sp->die_msg;
        sp_msg_type_to_xml(mt, mtype);
        xmlAddChild(mc, mtype);

        mtype = xmlNewNode(NULL, "miss");
        mtype->doc = doc;
        mt = &sp->miss_msg;
        sp_msg_type_to_xml(mt, mtype);
        xmlAddChild(mc, mtype);

        mtype = xmlNewNode(NULL, "hit");
        mtype->doc = doc;
        mt = &sp->hit_msg;
        sp_msg_type_to_xml(mt, mtype);
        xmlAddChild(mc, mtype);

	mtype = xmlNewNode(NULL, "god");
        mtype->doc = doc;
        mt = &sp->god_msg;
        sp_msg_type_to_xml(mt, mtype);
        xmlAddChild(mc, mtype);

        xmlAddChild(msg,mc);
        sp = sp->next;
      }
      xmlAddChild(tree, msg);
    }
  }

  xmlDocDump(fl,doc);
  xmlFreeDoc(doc);
  fclose(fl);

  return 0;
}

static int save_tp_msgs(void)
{
  xmlNodePtr tree, msg, mc, mtype;
  xmlDocPtr doc;
  FILE *fl;
  int i, j;
  char buf[500];  
  struct tp_message_type *sp;
  struct tp_msg_type *mt;
  
  if (NULL == (fl = fopen(TP_FILE_XML,"w")))
    return 4;

  doc = xmlNewDoc("1.0");
  
  tree = xmlNewDocNode(doc,NULL,"tpmessages",NULL);
  doc->children = tree;
  
  for (i = 0; i < MAX_SPELL_MESS; i++) {
    if (tp_messages[i].a_type) { // type == 0 -> no message!
      msg = xmlNewNode(NULL, "message");
      msg->doc = doc;
  
      sprintf(buf, "%ld", tp_messages[i].a_type);
      xmlNewProp(msg,"type", buf);
  
      sprintf(buf,"%ld", tp_messages[i].number_of_attacks);
      xmlNewProp(msg,"nummsg", buf);
  
      sp = tp_messages[i].msg;
      for (j = 0; j < tp_messages[i].number_of_attacks; j++) {
	mc = xmlNewNode(NULL, "msg");
        mc->doc = doc;
      
        mtype = xmlNewNode(NULL, "die");
        mtype->doc = doc;
        mt = &sp->die_msg;
        tp_msg_type_to_xml(mt, mtype);
        xmlAddChild(mc, mtype);

	mtype = xmlNewNode(NULL, "miss");
        mtype->doc = doc;
        mt = &sp->miss_msg;
        tp_msg_type_to_xml(mt, mtype);
        xmlAddChild(mc, mtype);

	mtype = xmlNewNode(NULL, "hit");
        mtype->doc = doc;
        mt = &sp->hit_msg;
        tp_msg_type_to_xml(mt, mtype);
        xmlAddChild(mc, mtype);

	xmlAddChild(msg,mc);
        sp = sp->next;   
      }
      xmlAddChild(tree, msg);
    }
  }   
        
  xmlDocDump(fl,doc);
  xmlFreeDoc(doc);

  fclose(fl);

  return 0;
}

int save_messages_xml(int type)
{
  if (TYPE_MSG == type) 
    return save_fight_msgs();
  else if(TYPE_SP_MSG == type)
    return save_sp_msgs();
  else if (TYPE_TP_MSG == type)
    return save_tp_msgs();

  return 1;
}

static const char *__errors[] = 
{
  "No error", 
  "Unknown msg type",
  "Unable to open fight message file",
  "Unable to open op spell message file",
  "Unable to open tp spell message file",
  "No root node in XML document", 
  "Root node of XML fight message file is not \"messages\"",
  "Root node of XML op message file is not \"spmessages\"",
  "Root node of XML tp message file is not \"tpmessages\"",
};

const char *msg_error(int error)
{
  return __errors[error];
}

static void fillInOpMsg(xmlNodePtr msg, struct sp_msg_type *m)
{
  xmlNodePtr tmp;

  for (tmp = msg->children; tmp != NULL; tmp = tmp->next) {
    if (!strcasecmp(tmp->name, "weaver")) {
      if (tmp->children && tmp->children->content)
	m->weaver = strdup(tmp->children->content);
      else
	m->weaver = strdup("<EMPTY>");
    }
    if (!strcasecmp(tmp->name,"notchann")) { 
      if (tmp->children && tmp->children->content)
	m->vict_nc = strdup(tmp->children->content); 
      else
        m->vict_nc = strdup("<EMPTY>");
    }
    if (!strcasecmp(tmp->name,"oppsexchann")) { 
      if (tmp->children && tmp->children->content)
	m->vict_cos = strdup(tmp->children->content); 
      else
        m->vict_cos = strdup("<EMPTY>");
    }
    if (!strcasecmp(tmp->name,"samesexchann")) { 
      if (tmp->children && tmp->children->content)
	m->vict_css = strdup(tmp->children->content); 
     else
       m->vict_css = strdup("<EMPTY>");
    }
    if (!strcasecmp(tmp->name,"notchannroom")) { 
      if (tmp->children && tmp->children->content)
	m->onl_nc = strdup(tmp->children->content); 
      else
        m->onl_nc = strdup("<EMPTY>");
    }
    if (!strcasecmp(tmp->name,"oppsexchannroom")) { 
      if (tmp->children && tmp->children->content)
	m->onl_cos = strdup(tmp->children->content); 
      else
        m->onl_cos = strdup("<EMPTY>");
    }
    if (!strcasecmp(tmp->name,"samesexchannrroom")) { 
      if (tmp->children && tmp->children->content)
	m->onl_css = strdup(tmp->children->content); 
      else
        m->onl_css = strdup("<EMPTY>");
    }
  }
}

static void fillInTpMsg(xmlNodePtr msg, struct tp_msg_type *m)
{  
  xmlNodePtr tmp;

  for (tmp = msg->children; tmp != NULL; tmp = tmp->next) {
    if (!strcasecmp(tmp->name, "weave")) { 
      if (tmp->children && tmp->children->content)
	m->weaver = strdup(tmp->children->content);
      else
        m->weaver  = strdup("<EMPTY>");
    }
    if (!strcasecmp(tmp->name, "tpuser")) { 
      if (tmp->children && tmp->children->content)
	m->vict_tpu = strdup(tmp->children->content);
      else
        m->vict_tpu = strdup("<EMPTY>");
    }
    if (!strcasecmp(tmp->name, "nottpuser")) { 
      if (tmp->children && tmp->children->content)
	m->vict_ntp = strdup(tmp->children->content);
      else
        m->vict_ntp = strdup("<EMPTY>");
    }
    if (!strcasecmp(tmp->name, "tpuserroom")) { 
      if (tmp->children && tmp->children->content)
	m->onl_tpu = strdup(tmp->children->content);
      else
        m->onl_tpu = strdup("<EMPTY>");
    }
    if (!strcasecmp(tmp->name, "nottpuserroom")) { 
      if (tmp->children && tmp->children->content)
	m->onl_npu = strdup(tmp->children->content);
      else
        m->onl_npu = strdup("<EMPTY>");
    }
  }
}

static void fillInFightMsg(xmlNodePtr msg, struct msg_type *m)
{
  xmlNodePtr tmp;
 
  for (tmp = msg->children; tmp != NULL; tmp = tmp->next) {
    if (!strcasecmp(tmp->name, "attacker")) {
      if (tmp->children && tmp->children->content)
	m->attacker_msg = strdup(tmp->children->content);
      else
        m->attacker_msg = strdup("<EMPTY>");
    }
    if (!strcasecmp(tmp->name,"victim")) {
      if (tmp->children && tmp->children->content)
	m->victim_msg   = strdup(tmp->children->content);
      else
        m->victim_msg = strdup("<EMPTY>");
    }
    if (!strcasecmp(tmp->name,"room")) {
      if (tmp->children && tmp->children->content)
	m->room_msg     = strdup(tmp->children->content);
      else
        m->room_msg = strdup("<EMPTY>");
    }
  }
}

static int load_fight_msgs(void)
{
  xmlNodePtr root,temp, temp2, temp3;
  xmlDocPtr  doc;
  register int i;
  int j;
  struct message_type *m;

  if (!(doc = xmlParseFile(MESS_FILE_XML))) {
    return 2;
  }
  if (!(root = doc->children)) {
    return 5;
  }
  if (strcasecmp("messages",root->name)) {
    return 6;
  }

  for (i = 0; i < MAX_MESSAGES; i++) {
    fight_messages[i].a_type = 0;
    fight_messages[i].number_of_attacks = 0;
    fight_messages[i].msg = 0;
  }

  i = 0; 
  for (temp = root->children; NULL != temp; temp = temp->next) {
    if (!strcasecmp(temp->name, "message")) {
      fight_messages[i].a_type = xmlAtol(xmlGetProp(temp,"type"));
      fight_messages[i].number_of_attacks = xmlAtol(xmlGetProp(temp,"nummsg"));

      j = 0; 
      for (temp2 = temp->children; NULL != temp2; temp2 = temp2->next) {
	if (!strcasecmp(temp2->name, "msg")) {
	  m = calloc(1, sizeof(struct message_type));
          m->next = fight_messages[i].msg;
          fight_messages[i].msg = m;
	  j++;

	  for (temp3 = temp2->children; NULL != temp3; temp3 = temp3->next) {
	    if (!strcasecmp(temp3->name, "die")) {
	      fillInFightMsg(temp3, &m->die_msg);
	    }
	    if (!strcasecmp(temp3->name, "miss")) {
	      fillInFightMsg(temp3, &m->miss_msg);
	    }
	    if (!strcasecmp(temp3->name, "hit")) {
	      fillInFightMsg(temp3, &m->hit_msg);
	    }
	    if (!strcasecmp(temp3->name,"god")) {
	      fillInFightMsg(temp3, &m->god_msg);
	    }
	  }
	}
      }

      if (fight_messages[i].number_of_attacks != j) {
	alog("WARNING: #msg-property in fight message %d for message xmlfile does not "
	     " match the number actually found! Expected %ld got %d", 
	     i, fight_messages[i].number_of_attacks, j);
        fight_messages[i].number_of_attacks = j;
      }
      i++;
    }
  }
  xmlFreeDoc(doc);

  return 0;
}

static int load_sp_msgs(void)
{
  xmlNodePtr root,temp, temp2, temp3;
  xmlDocPtr  doc;
  register int i;
  int j;
  struct sp_message_type *m;

  if (!(doc = xmlParseFile(SPELL_FILE_XML))) {
    return 3;
  }
  if (!(root = doc->children)) {
    return 5;
  }
  if (strcasecmp("spmessages",root->name)) {
    return 7;
  }

  for (i = 0; i < MAX_SPELL_MESS; i++) {
    spell_messages[i].a_type = 0;
    spell_messages[i].number_of_attacks = 0;
    spell_messages[i].msg = 0;
  }

  i = 0;
  for (temp = root->children; NULL != temp; temp = temp->next) {
    if (!strcasecmp(temp->name, "message")) {
      spell_messages[i].a_type = xmlAtol(xmlGetProp(temp,"type"));
      spell_messages[i].number_of_attacks = xmlAtol(xmlGetProp(temp,"nummsg"));

      j = 0;
      for (temp2 = temp->children; NULL != temp2; temp2 = temp2->next) {
        if (!strcasecmp(temp2->name, "msg")) {
          m = calloc(1, sizeof(struct sp_message_type));
          m->next = spell_messages[i].msg;
          spell_messages[i].msg = m;
          j++;

          for (temp3 = temp2->children; NULL != temp3; temp3 = temp3->next) {
            if (!strcasecmp(temp3->name, "die")) {
              fillInOpMsg(temp3, &m->die_msg);
            }
            if (!strcasecmp(temp3->name, "miss")) {
              fillInOpMsg(temp3, &m->miss_msg);
            }
            if (!strcasecmp(temp3->name, "hit")) {
              fillInOpMsg(temp3, &m->hit_msg);
            }
	    if (!strcasecmp(temp3->name, "god")) {
              fillInOpMsg(temp3, &m->god_msg);
            }
          }
        }
      }
      if (spell_messages[i].number_of_attacks != j) {
        alog("WARNING: #msg-property in spell message %d for message xmlfile does not "
             " match the number actually found! Expected %ld got %d",
             i, spell_messages[i].number_of_attacks, j);
        spell_messages[i].number_of_attacks = j;
      }
      i++;
    }
  }
  xmlFreeDoc(doc);

  return 0;
}

static int load_tp_msgs(void)
{
  xmlNodePtr root,temp, temp2, temp3;
  xmlDocPtr  doc;
  register int i;
  int j;
  struct tp_message_type *m;

  if (!(doc = xmlParseFile(TP_FILE_XML))) {
    return 4;
  }
  if (!(root = doc->children)) {
    return 5;
  }
  if (strcasecmp("tpmessages",root->name)) {
    return 8;
  }

  for (i = 0; i < MAX_SPELL_MESS; i++) {
    tp_messages[i].a_type = 0;
    tp_messages[i].number_of_attacks = 0;
    tp_messages[i].msg = 0;
  }

  i = 0;
  for (temp = root->children; NULL != temp; temp = temp->next) {
    if (!strcasecmp(temp->name, "message")) {
      tp_messages[i].a_type = xmlAtol(xmlGetProp(temp,"type"));
      tp_messages[i].number_of_attacks = xmlAtol(xmlGetProp(temp,"nummsg"));

      j = 0;
      for (temp2 = temp->children; NULL != temp2; temp2 = temp2->next) {
        if (!strcasecmp(temp2->name, "msg")) {
          m = calloc(1, sizeof(struct tp_message_type));
          m->next = tp_messages[i].msg;
          tp_messages[i].msg = m;
          j++;

          for (temp3 = temp2->children; NULL != temp3; temp3 = temp3->next) {
            if (!strcasecmp(temp3->name, "die")) {
              fillInTpMsg(temp3, &m->die_msg);
            }
            if (!strcasecmp(temp3->name, "miss")) {
              fillInTpMsg(temp3, &m->miss_msg);
            }
            if (!strcasecmp(temp3->name, "hit")) {
              fillInTpMsg(temp3, &m->hit_msg);
            }
          }
        }
      }
      if (tp_messages[i].number_of_attacks != j) {
        alog("WARNING: #msg-property in tp message %d for message xmlfile does not "
             " match the number actually found! Expected %ld got %d",
             i, tp_messages[i].number_of_attacks, j);
        tp_messages[i].number_of_attacks = j;
      }
      i++;
    }
  }
  xmlFreeDoc(doc);

  return 0;


  return 0;
}

void free_fight_msg(struct msg_type *m)
{
  if (m->attacker_msg)
    free(m->attacker_msg);
  if (m->victim_msg)
    free(m->victim_msg);
  if (m->room_msg)
    free(m->room_msg);
}

void free_tp_msg(struct tp_msg_type *m)
{
  if (m->weaver) 
    free(m->weaver);
  if (m->vict_tpu) 
    free(m->vict_tpu);
  if (m->vict_ntp) 
    free(m->vict_ntp);
  if (m->onl_tpu) 
    free(m->onl_tpu);
  if (m->onl_npu) 
    free(m->onl_npu);
}

void free_sp_msg(struct sp_msg_type *m)
{
  if (m->weaver) 
    free(m->weaver);
  if (m->vict_nc) 
    free(m->vict_nc);
  if (m->vict_cos) 
    free(m->vict_cos);
  if (m->vict_css) 
    free(m->vict_css);
  if (m->onl_nc) 
    free(m->onl_nc);
  if (m->onl_cos) 
    free(m->onl_cos);
  if (m->onl_css) 
    free(m->onl_css);
}

int load_messages_xml(int type)
{
  if (TYPE_MSG == type)
    return load_fight_msgs();
  else if(TYPE_SP_MSG == type)
    return load_sp_msgs();
  else if (TYPE_TP_MSG == type)
    return load_tp_msgs();

  return 1;
}

ACMD(save_msgs)
{
  int rc = 0;

  if (!*argument) {
    send_to_char("Usage: msave {fight|op|tp|all}\r\n", ch);
    return;
  }

  one_argument(argument,buf); 

  if (!strcmp(buf, "fight"))
    rc = save_messages_xml(TYPE_MSG);
  else if (!strcmp(buf, "op"))
    rc = save_messages_xml(TYPE_SP_MSG);
  else if (!strcmp(buf, "tp")) 
    rc = save_messages_xml(TYPE_TP_MSG);
  else if (!strcmp(buf, "all")) {
    rc = save_messages_xml(TYPE_MSG);
    if (rc) {
      asend_to_char(ch, "Failure saving \"fight\" (all): %s\r\n", msg_error(rc));
    }

    rc = save_messages_xml(TYPE_SP_MSG);
    if (rc) {
      asend_to_char(ch, "Failure saving \"op\" (all): %s\r\n", msg_error(rc));
    }

    rc = save_messages_xml(TYPE_TP_MSG);
    if (rc) {
      asend_to_char(ch, "Failure saving \"tp\" (all): %s\r\n", msg_error(rc));
    }
    rc = 0;
  } else {
    send_to_char("Usage: msave {fight|op|tp|all}\r\n", ch);
    return;
  }

  if (rc) {
    asend_to_char(ch, "Failure saving \"%s\": %s\r\n", buf, msg_error(rc));
  } else {
    asend_to_char(ch, "Saving \"%s\" successful.\r\n", buf);
  } 
}
