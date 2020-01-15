/******************************************************************************
 * $Id: mail.c,v 1.6 2002/11/24 14:40:28 fang Exp $
 *   
 * File: mail.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: mail.c,v $
 *   Revision 1.6  2002/11/24 14:40:28  fang
 *   Once again messing with the same error, different approach this time, though.
 *
 *   Revision 1.5  2002/11/07 04:02:02  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.4  2002/09/06 16:14:16  fang
 *   Drath: Change to mudmail sdesc and sender added to keywords automatically
 *
 *   Revision 1.3  2002/09/04 01:01:41  fang
 *   Drath: Morts can mail from any room.
 *
 *   Revision 1.2  2002/07/28 13:56:04  fang
 *   Removed dmlloc...just to test.
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.4  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.3  2002/03/08 14:16:35  fang
 *   Added a save in the "recieve" mail command.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.20  2001/09/11 03:46:52  mud
 *   *** empty log message ***
 *
 *   Revision 1.19  2001/09/11 02:56:02  mud
 *   *** empty log message ***
 *
 *   Revision 1.18  2001/08/31 13:56:54  mud
 *   *** empty log message ***
 *
 *   Revision 1.17  2001/08/30 12:36:39  mud
 *   *** empty log message ***
 *
 *   Revision 1.16  2001/08/30 12:16:16  mud
 *   *** empty log message ***
 *
 *   Revision 1.15  2001/08/23 16:45:52  mud
 *   *** empty log message ***
 *
 *   Revision 1.14  2001/01/19 17:51:51  mud
 *   Added line numbers to the editor.
 *
 *   Revision 1.13  2000/12/22 10:01:14  mud
 *   *** empty log message ***
 *
 *   Revision 1.12  2000/11/23 23:27:22  mud
 *   *** empty log message ***
 *
 *   Revision 1.11  2000/11/22 06:08:52  mud
 *   *** empty log message ***
 *
 *   Revision 1.10  2000/11/19 22:43:35  mud
 *   *** empty log message ***
 *
 *   Revision 1.9  2000/11/18 17:13:55  mud
 *   added guildmail command
 *
 *   Revision 1.8  2000/11/02 22:55:42  mud
 *   *** empty log message ***
 *
 *   Revision 1.7  2000/10/31 01:28:10  mud
 *   added multiple recipients for mudmail. 5 should be enough..
 *
 *   Revision 1.6  2000/10/28 16:00:46  mud
 *   *** empty log message ***
 *
 *   Revision 1.5  2000/10/28 15:28:31  mud
 *   added a PLR_NOMAIL flag (so a player can't send mail)
 *
 *   Revision 1.4  2000/10/25 00:53:01  mud
 *   *** empty log message ***
 *
 *   Revision 1.3  2000/10/09 10:29:49  mud
 *   added 'showmail' command. shows the date and sender of mail in your inventory
 *
 *   Revision 1.2  2000/02/21 14:00:17  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* ************************************************************************
*   File: mail.c                                        Part of CircleMUD *
*  Usage: Internal funcs and player spec-procs of mud-mail system         *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

/******* MUD MAIL SYSTEM MAIN FILE ***************************************

Written by Jeremy Elson (jelson@cs.jhu.edu)

*************************************************************************/

#include "conf.h"
#include "sysdep.h"
#include "act.h"

#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "db.h"
#include "interpreter.h"
#include "handler.h"
#include "mail.h"
#include "guild.h"
#include "guild_parser.h"

mail_index_type *mail_index = 0;   /* list of recs in the mail file  */
position_list_type *free_list = 0; /* list of free positions in file */
long file_end_pos = 0;		   /* length of file */


void push_free_list(long pos)
{
  position_list_type *new_pos;

  CREATE(new_pos, position_list_type, 1);
  new_pos->position = pos;
  new_pos->next = free_list;
  free_list = new_pos;
}



long pop_free_list(void)
{
  position_list_type *old_pos;
  long return_value;

  if ((old_pos = free_list) != 0) {
    return_value = free_list->position;
    free_list = old_pos->next;
    free(old_pos);
    return return_value;
  } else
    return file_end_pos;
}



mail_index_type *find_char_in_index(long searchee)
{
  mail_index_type *tmp;

  if (searchee < 0) {
    mlog("SYSERR: Mail system -- non fatal error #1.");
    return 0;
  }
  for (tmp = mail_index; (tmp && tmp->recipient != searchee); tmp = tmp->next);

  return tmp;
}



void write_to_file(void *buf, int size, long filepos)
{
  FILE *mail_file;

  mail_file = fopen(MAIL_FILE, "r+b");

  if (filepos % BLOCK_SIZE) {
    mlog("SYSERR: Mail system -- fatal error #2!!!");
    no_mail = 1;
    fclose(mail_file);
    return;
  }
  fseek(mail_file, filepos, SEEK_SET);
  fwrite(buf, size, 1, mail_file);

  /* find end of file */
  fseek(mail_file, 0L, SEEK_END);
  file_end_pos = ftell(mail_file);
  fclose(mail_file);
  return;
}


void read_from_file(void *buf, int size, long filepos)
{
  FILE *mail_file;

  mail_file = fopen(MAIL_FILE, "r+b");

  if (filepos % BLOCK_SIZE) {
    mlog("SYSERR: Mail system -- fatal error #3!!!");
    no_mail = 1;
    fclose(mail_file);
    return;
  }
  fseek(mail_file, filepos, SEEK_SET);
  fread(buf, size, 1, mail_file);
  fclose(mail_file);
  return;
}




void index_mail(long id_to_index, long pos)
{
  mail_index_type *new_index;
  position_list_type *new_position;

  if (id_to_index < 0) {
    mlog("SYSERR: Mail system -- non-fatal error #4.");
    return;
  }
  if (!(new_index = find_char_in_index(id_to_index))) {
    /* name not already in index.. add it */
    CREATE(new_index, mail_index_type, 1);
    new_index->recipient = id_to_index;
    new_index->list_start = NULL;

    /* add to front of list */
    new_index->next = mail_index;
    mail_index = new_index;
  }
  /* now, add this position to front of position list */
  CREATE(new_position, position_list_type, 1);
  new_position->position = pos;
  new_position->next = new_index->list_start;
  new_index->list_start = new_position;
}


/* SCAN_FILE */
/* scan_file is called once during boot-up.  It scans through the mail file
   and indexes all entries currently in the mail file. */
int scan_file(void)
{
  FILE *mail_file;
  header_block_type next_block;
  int total_messages = 0, block_num = 0;
  char buf[100];

  if (!(mail_file = fopen(MAIL_FILE, "r"))) {
    mlog("Mail file non-existant... creating new file.");
    mail_file = fopen(MAIL_FILE, "w");
    fclose(mail_file);
    return 1;
  }
  while (fread(&next_block, sizeof(header_block_type), 1, mail_file)) {
    if (next_block.block_type == HEADER_BLOCK) {
      index_mail(next_block.header_data.to, block_num * BLOCK_SIZE);
      total_messages++;
    } else if (next_block.block_type == DELETED_BLOCK)
      push_free_list(block_num * BLOCK_SIZE);
    block_num++;
  }

  file_end_pos = ftell(mail_file);
  fclose(mail_file);
  sprintf(buf, "   %ld bytes read.", file_end_pos);
  mlog(buf);
  if (file_end_pos % BLOCK_SIZE) {
    mlog("SYSERR: Error booting mail system -- Mail file corrupt!");
    mlog("SYSERR: Mail disabled!");
    return 0;
  }
  sprintf(buf, "   Mail file read -- %d messages.", total_messages);
  mlog(buf);
  return 1;
}				/* end of scan_file */


/* HAS_MAIL */
/* a simple little function which tells you if the guy has mail or not */
int has_mail(long recipient)
{
  if (find_char_in_index(recipient))
    return 1;
  return 0;
}



/* STORE_MAIL  */
/* call store_mail to store mail.  (hard, huh? :-) )  Pass 3 arguments:
   who the mail is to (long), who it's from (long), and a pointer to the
   actual message text (char *).
*/

void store_mail(long to, long from, char *message_pointer)
{
  header_block_type header;
  data_block_type data;
  long last_address, target_address;
  char *msg_txt = message_pointer;
  int bytes_written = 0;
  int total_length = strlen(message_pointer);

  assert(sizeof(header_block_type) == sizeof(data_block_type));
  assert(sizeof(header_block_type) == BLOCK_SIZE);

  if (from < 0 || to < 0 || !*message_pointer) {
    mlog("SYSERR: Mail system -- non-fatal error #5.");
    return;
  }
  memset((char *) &header, 0, sizeof(header));	/* clear the record */
  header.block_type = HEADER_BLOCK;
  header.header_data.next_block = LAST_BLOCK;
  header.header_data.from = from;
  header.header_data.to = to;
  header.header_data.mail_time = time(0);
  strncpy(header.txt, msg_txt, HEADER_BLOCK_DATASIZE);
  header.txt[HEADER_BLOCK_DATASIZE] = '\0';

  target_address = pop_free_list();	/* find next free block */
  index_mail(to, target_address);	/* add it to mail index in memory */
  write_to_file(&header, BLOCK_SIZE, target_address);

  if (strlen(msg_txt) <= HEADER_BLOCK_DATASIZE)
    return;			/* that was the whole message */

  bytes_written = HEADER_BLOCK_DATASIZE;
  msg_txt += HEADER_BLOCK_DATASIZE;	/* move pointer to next bit of text */

  /*
   * find the next block address, then rewrite the header to reflect where
   * the next block is.
   */
  last_address = target_address;
  target_address = pop_free_list();
  header.header_data.next_block = target_address;
  write_to_file(&header, BLOCK_SIZE, last_address);

  /* now write the current data block */
  memset((char *) &data, 0, sizeof(data));	/* clear the record */
  data.block_type = LAST_BLOCK;
  strncpy(data.txt, msg_txt, DATA_BLOCK_DATASIZE);
  data.txt[DATA_BLOCK_DATASIZE] = '\0';
  write_to_file(&data, BLOCK_SIZE, target_address);
  bytes_written += strlen(data.txt);
  msg_txt += strlen(data.txt);

  /*
   * if, after 1 header block and 1 data block there is STILL part of the
   * message left to write to the file, keep writing the new data blocks and
   * rewriting the old data blocks to reflect where the next block is.  Yes,
   * this is kind of a hack, but if the block size is big enough it won't
   * matter anyway.  Hopefully, MUD players won't pour their life stories out
   * into the Mud Mail System anyway.
   * 
   * Note that the block_type data field in data blocks is either a number >=0,
   * meaning a link to the next block, or LAST_BLOCK flag (-2) meaning the
   * last block in the current message.  This works much like DOS' FAT.
   */

  while (bytes_written < total_length) {
    last_address = target_address;
    target_address = pop_free_list();

    /* rewrite the previous block to link it to the next */
    data.block_type = target_address;
    write_to_file(&data, BLOCK_SIZE, last_address);

    /* now write the next block, assuming it's the last.  */
    data.block_type = LAST_BLOCK;
    strncpy(data.txt, msg_txt, DATA_BLOCK_DATASIZE);
    data.txt[DATA_BLOCK_DATASIZE] = '\0';
    write_to_file(&data, BLOCK_SIZE, target_address);

    bytes_written += strlen(data.txt);
    msg_txt += strlen(data.txt);
  }
}				/* store mail */




/* READ_DELETE */
/* read_delete takes 1 char pointer to the name of the person whose mail
you're retrieving.  It returns to you a char pointer to the message text.
The mail is then discarded from the file and the mail index. */

char *read_delete(long recipient)
/* recipient is the name as it appears in the index.
   recipient_formatted is the name as it should appear on the mail
   header (i.e. the text handed to the player) */
{
  header_block_type header;
  data_block_type data;
  mail_index_type *mail_pointer, *prev_mail;
  position_list_type *position_pointer;
  long mail_address, following_block;
  char *message, *tmstr, buf[200];
  size_t string_size;

  if (recipient < 0) {
    mlog("SYSERR: Mail system -- non-fatal error #6.");
    return 0;
  }
  if (!(mail_pointer = find_char_in_index(recipient))) {
    mlog("SYSERR: Mail system -- post office spec_proc error?  Error #7.");
    return 0;
  }
  if (!(position_pointer = mail_pointer->list_start)) {
    mlog("SYSERR: Mail system -- non-fatal error #8.");
    return 0;
  }
  if (!(position_pointer->next)) {	/* just 1 entry in list. */
    mail_address = position_pointer->position;
    free(position_pointer);

    /* now free up the actual name entry */
    if (mail_index == mail_pointer) {	/* name is 1st in list */
      mail_index = mail_pointer->next;
      free(mail_pointer);
    } else {
      /* find entry before the one we're going to del */
      for (prev_mail = mail_index;
       prev_mail->next != mail_pointer;
       prev_mail = prev_mail->next);
      prev_mail->next = mail_pointer->next;
      free(mail_pointer);
    }
  } else {
    /* move to next-to-last record */
    while (position_pointer->next->next)
      position_pointer = position_pointer->next;
    mail_address = position_pointer->next->position;
    free(position_pointer->next);
    position_pointer->next = 0;
  }

  /* ok, now lets do some readin'! */
  read_from_file(&header, BLOCK_SIZE, mail_address);

  if (header.block_type != HEADER_BLOCK) {
    mlog("SYSERR: Oh dear.");
    no_mail = 1;
    mlog("SYSERR: Mail system disabled!  -- Error #9.");
    return 0;
  }
  tmstr = asctime(localtime(&header.header_data.mail_time));
  *(tmstr + strlen(tmstr) - 1) = '\0';

  sprintf(buf, " -.o0o.-  The royal Caemlyn mail system  -.o0o.-\r\n"
      "Date: %s\r\n"
      "  To: %s\r\n"
      "From: %s\r\n\r\n", tmstr, get_name_by_id(recipient),
      get_name_by_id(header.header_data.from));

  string_size = (sizeof(char) * (strlen(buf) + strlen(header.txt) + 1));
  CREATE(message, char, string_size);
  strcpy(message, buf);
  strcat(message, header.txt);
  message[string_size - 1] = '\0';
  following_block = header.header_data.next_block;

  /* mark the block as deleted */
  header.block_type = DELETED_BLOCK;
  write_to_file(&header, BLOCK_SIZE, mail_address);
  push_free_list(mail_address);

  while (following_block != LAST_BLOCK) {
    read_from_file(&data, BLOCK_SIZE, following_block);

    string_size = (sizeof(char) * (strlen(message) + strlen(data.txt) + 1));
    RECREATE(message, char, string_size);
    strcat(message, data.txt);
    message[string_size - 1] = '\0';
    mail_address = following_block;
    following_block = data.block_type;
    data.block_type = DELETED_BLOCK;
    write_to_file(&data, BLOCK_SIZE, mail_address);
    push_free_list(mail_address);
  }

  return message;
}

#define HARD_CODED_POSTMASTER 1 // Use Puff as postmaster :)
ACMD(do_receive) 
{
  struct char_data *pm = read_mobile(HARD_CODED_POSTMASTER,VIRTUAL);

  if (!ch->desc || IS_NPC(ch) || NULL == pm)
    return; 

  if (no_mail) {
    send_to_char("Sorry, the mail system is having technical difficulties.\r\n",ch);
    return;
  }

  skip_spaces(&argument);
  char_to_room(pm,ch->in_room);
  postmaster_receive_mail(ch, pm, cmd, argument);

  save_char(ch, NOWHERE);
  
  extract_char(pm,TRUE);
}

ACMD(do_check)
{
  struct char_data *pm = read_mobile(HARD_CODED_POSTMASTER,VIRTUAL);

  if (!ch->desc || IS_NPC(ch) || NULL == pm)
    return;
    
  if (no_mail) {
    send_to_char("Sorry, the mail system is having technical difficulties.\r\n",ch);
    return;
  }
    
  skip_spaces(&argument);
  char_to_room(pm,ch->in_room);
  postmaster_check_mail(ch, pm, cmd, argument);
  extract_char(pm,TRUE);
}

void send_guildmail(struct char_data * ch, struct char_data *mailman, char *arg) {
  char buf[256];
  struct char_guild_element *element;
  int num = 0;

  if (PLR_FLAGGED(ch, PLR_NOMAIL)) {
    send_to_char("You are not allowed to send mail at the moment. Speak with an immortal.\r\n", ch);
    return;
  }
   
  one_argument(arg, buf);
  if (*buf) {
   num = atoi(buf);
   element = GET_CHAR_GUILDS(ch);
   while (element) {
     if (((element->guild->id == num) || (!strncasecmp(element->guild->name, buf, strlen(buf))))
         && STATUS_FLAGGED(element->guildie, STATUS_GL)) break;
     element = element->next;
   }
  } else {
    send_to_char("Usage: guildmail <guild no/name>\r\n", ch);
    return;
  }
  
  if (!element) {
    send_to_char("You don't lead the guild.\r\n", ch);
    return;
  }

  ch->desc->mail_to[0] = -1;
  ch->desc->mail_to[1] = GET_IDNUM(ch);
  ch->desc->mail_to[2] = element->guild->id;

  act("$n starts to write some mail.", TRUE, ch, 0, 0, TO_ROOM);
  sprintf(buf, "&r$n tells you, 'Write your message.(/s saves /h for help)'&n");
  act(buf, FALSE, mailman, 0, ch, TO_VICT);

send_to_char("Please format your note to less than 75 characters per line.\r\n", ch);
send_to_char("This will make your note easier to read for those with smaller screens\r\n",ch);
send_to_char("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n",ch);

  SET_BIT(PLR_FLAGS(ch), PLR_MAILING);
  SET_BIT(PLR_FLAGS(ch), PLR_WRITING);

  CREATE( ch->desc->str, char*, 1 );
  sprintf(buf, "&mGuildmail&n: %s&n\r\n\r\n", element->guild->name);
  CREATE( *(ch->desc->str), char, strlen( buf ) + 1 );
  strcpy(*(ch->desc->str), buf);
  send_to_char("\r\n", ch);
  send_to_char(buf, ch);
  ch->desc->current_text_line = count_newlines(ch->desc->str) + 1;
  ch->desc->max_str = MAX_MAIL_SIZE;
}


/*
 * Allow immortlas to mail from anywhere, but morts
 * still need to find a postmaster
*/
ACMD(do_mail)
{
  struct char_data *pm = read_mobile(HARD_CODED_POSTMASTER,VIRTUAL);
  
  if (!ch->desc || IS_NPC(ch) || NULL == pm)
    return;

  char_to_room(pm,ch->in_room);

//  if (!IS_IMMORTAL(ch) && (subcmd != SCMD_MAIL_GUILD)) {
//    extract_char(pm,TRUE);
//    do_not_here(ch,argument,0,0);
//    return;
//  }
    
  if (no_mail) {
    send_to_char("Sorry, the mail system is having technical difficulties.\r\n",ch);
    extract_char(pm,TRUE);
    return;
  }
  
  skip_spaces(&argument);
//  char_to_room(pm,ch->in_room);
  if (subcmd == SCMD_MAIL_GUILD) 
    send_guildmail(ch, pm, argument);
  else 
    postmaster_send_mail(ch, pm, cmd, argument);
  extract_char(pm,TRUE);
}
#undef HARD_CODED_POSTMASTER

/*****************************************************************
** Below is the spec_proc for a postmaster using the above       **
** routines.  Written by Jeremy Elson (jelson@server.cs.jhu.edu) **
*****************************************************************/

SPECIAL(postmaster)
{
  if (!ch->desc || IS_NPC(ch))
    return 0;			/* so mobs don't get caught here */
   
  if (!(CMD_IS("mail") || CMD_IS("check") || CMD_IS("receive")))
    return 0;

  if (no_mail) {
    send_to_char("Sorry, the mail system is having technical difficulties.\r\n", ch);
    return 0;
  }

  /* Check if the room has the postmaster flag set */
  if (GET_ROOM_SPEC(ch->in_room) == postmaster) {
     return 0;
  }
   
  if (CMD_IS("mail")) {
    postmaster_send_mail(ch, me, cmd, argument);
    return 1;
  } else if (CMD_IS("check")) {
    postmaster_check_mail(ch, me, cmd, argument);
    return 1;
  } else if (CMD_IS("receive")) {
    postmaster_receive_mail(ch, me, cmd, argument);
    save_char(ch, NOWHERE);
    return 1;
  } 
  else
    return 0;
}


void postmaster_send_mail(struct char_data * ch, struct char_data *mailman,
              int cmd, char *arg)
{
  char buf[256], buf2[256], names[(MAX_NAME_LENGTH * 5) + 25];
  int num_recipients = 0;

  if (GET_LEVEL(ch) < MIN_MAIL_LEVEL && !GET_AUTHORIZED(ch)) {
    sprintf(buf, "$n tells you, 'Sorry, you have to be level %d to send mail!'",
        MIN_MAIL_LEVEL);
    act(buf, FALSE, mailman, 0, ch, TO_VICT);
    return;
  }

  if (PLR_FLAGGED(ch, PLR_NOMAIL)) {
    send_to_char("You are not allowed to send mail at the moment. Speak with an immortal.\r\n", ch);
    return;
  }
   

  for (num_recipients = 0;num_recipients < 5;num_recipients++) ch->desc->mail_to[num_recipients] = 0;

  num_recipients = 0;

  sprintf(names, "&mAll Recipients&n:");
  while (arg && *arg && (num_recipients < 5)) {
    arg = one_argument(arg, buf);

    if ((ch->desc->mail_to[num_recipients] = get_id_by_name(buf)) < 0) {
      sprintf(buf2, "&r$n tells you, 'No one by the name of %s is registered here!'&n", buf);
      act(buf2, FALSE, mailman, 0, ch, TO_VICT);
    } else {
      strcat(names, " ");
      strcat(names, get_name_by_id(ch->desc->mail_to[num_recipients]));
      num_recipients++;
    }
  }
  strcat(names, "\r\n\r\n");
  
  if (!num_recipients) {			/* you'll get no argument from me! */
    act("$n tells you, 'You need to specify an addressee!'",
    FALSE, mailman, 0, ch, TO_VICT);
    for (num_recipients = 0;num_recipients < 5;num_recipients++) ch->desc->mail_to[num_recipients] = 0;
    return;
  }
  if (GET_LEVEL(ch) < LVL_IMMORT && GET_GOLD(ch) < (STAMP_PRICE * num_recipients)) {
    sprintf(buf, "$n tells you, '%d stamp%s costs %d coins.'\r\n"
        "$n tells you, '...which I see you can't afford.'", num_recipients, 
        num_recipients == 1 ? "" : "s", STAMP_PRICE * num_recipients);
    act(buf, FALSE, mailman, 0, ch, TO_VICT);
    for (num_recipients = 0;num_recipients < 5;num_recipients++) ch->desc->mail_to[num_recipients] = 0;
    return;
  }

  if (GET_LEVEL(ch) < LVL_IMMORT)
    GET_GOLD(ch) -= STAMP_PRICE * num_recipients;

  act("$n starts to write some mail.", TRUE, ch, 0, 0, TO_ROOM);
  sprintf(buf, "$n tells you, 'I'll take %d coins for the stamp%s.'\r\n"
       "$n tells you, 'Write your message.(/s saves /h for help)'",
      (GET_LEVEL(ch) < LVL_IMMORT ? (STAMP_PRICE * num_recipients) : 0), num_recipients == 1 ? "" : "s");
  act(buf, FALSE, mailman, 0, ch, TO_VICT);

send_to_char("Please format your note to less than 75 characters per line.\r\n", ch);
send_to_char("This will make your note easier to read for those with smaller screens\r\n",ch);
send_to_char("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n",ch);

  SET_BIT(PLR_FLAGS(ch), PLR_MAILING);
  SET_BIT(PLR_FLAGS(ch), PLR_WRITING);

  CREATE( ch->desc->str, char*, 1 );
  *(ch->desc->str) = NULL;
  if (num_recipients > 1) {
    CREATE( *(ch->desc->str), char, strlen( buf ) + 1 );
    strcpy(*(ch->desc->str), names);
    send_to_char("\r\n", ch);
    send_to_char(names, ch);
  }
  ch->desc->current_text_line = count_newlines(ch->desc->str) + 1;
  ch->desc->max_str = MAX_MAIL_SIZE;
}


void postmaster_check_mail(struct char_data * ch, struct char_data *mailman,
              int cmd, char *arg)
{
  char buf[256];

  if (has_mail(GET_IDNUM(ch)))
    sprintf(buf, "$n tells you, 'You have mail waiting.'");
  else
    sprintf(buf, "$n tells you, 'Sorry, you don't have any mail waiting.'");
  act(buf, FALSE, mailman, 0, ch, TO_VICT);
}


void postmaster_receive_mail(struct char_data * ch, struct char_data *mailman,
              int cmd, char *arg)
{
  char buf[256];
  char date[MAX_INPUT_LENGTH], from[MAX_INPUT_LENGTH], *tmp = NULL, sdesc[MAX_INPUT_LENGTH];
  struct obj_data *obj;

  if (!has_mail(GET_IDNUM(ch))) {
    sprintf(buf, "$n tells you, 'Sorry, you don't have any mail waiting.'");
    act(buf, FALSE, mailman, 0, ch, TO_VICT);
    return;
  }
  while (has_mail(GET_IDNUM(ch))) {
    obj = create_obj();
    obj->item_number = 1;
    obj->name = str_dup("mail paper letter");
    obj->short_description = str_dup("a piece of mail");
    obj->description = str_dup("Someone has left a piece of mail here.");

    GET_OBJ_TYPE(obj) = ITEM_NOTE;
    GET_OBJ_WEAR(obj) = ITEM_WEAR_TAKE | ITEM_WEAR_HOLD;
    GET_OBJ_WEIGHT(obj) = 1;
    GET_OBJ_COST(obj) = 30;
    obj->action_description = read_delete(GET_IDNUM(ch));

    if (obj->action_description == NULL)
      obj->action_description =	str_dup("Mail system error - please report.  Error #11.\r\n");

    if (!(tmp = strstr(obj->action_description, "Date:"))) {
    } else {
      strncpy(date, tmp+6, MAX_INPUT_LENGTH-1);
      if (!(tmp = strchr(date, '\r'))) {
        strcpy(date, "<Unknown>");
      }
    }
    *tmp='\0';
    if (!(tmp = strstr(obj->action_description, "From:"))) {
    } else {
      strncpy(from, tmp+6, MAX_INPUT_LENGTH-1);
      if (!(tmp = strchr(from, '\r'))) {
        strcpy(from, "<Unknown>");
      }
    }
    *tmp='\0';
    sprintf(sdesc, "a mudmail from %s%s&n sent on %s%s&n", GET_MYCOLOR(ch), from, GET_MYCOLOR(ch), date);
    obj->short_description = str_dup(sdesc);
    sprintf(sdesc, "mail paper letter %s", from);
    obj->name = str_dup(sdesc);

    obj_to_char(obj, ch);

    act("$n gives you a piece of mail.", FALSE, mailman, 0, ch, TO_VICT);
    act("$N gives $n a piece of mail.", FALSE, ch, 0, mailman, TO_ROOM);
  }
}

ACMD(do_showmail) {
  int num_mails = 0, bits;
  char date[MAX_INPUT_LENGTH], from[MAX_INPUT_LENGTH], *tmp = NULL;
  struct obj_data *obj = NULL;
  struct char_data *dummy = NULL;
  
  if (IS_NPC(ch) || !ch->desc) {
    send_to_char("Mobs don't get any mail..\r\n", ch);
    return;
  }
  
  if (!argument || !*argument) {
    obj = ch->carrying;
    sprintf(buf, "Mail in your inventory: \r\n");
  } else {
    one_argument(argument, arg);
    if (!(bits = generic_find(arg, FIND_OBJ_INV | FIND_OBJ_EQUIP, ch, &dummy, &obj))) {
      sprintf(buf, "There doesn't seem to be %s %s here.\r\n", AN(arg), arg);
      send_to_char(buf, ch);
      return;
    } else if (GET_OBJ_TYPE(obj) != ITEM_CONTAINER) {
      send_to_char("There's no mail inside that!\r\n", ch);
      return;
    } else {
      if (IS_SET(GET_OBJ_VAL(obj, 1), CONT_CLOSED))
        send_to_char("It is closed.\r\n", ch);
      else {
        sprintf(buf, "Mail carried in %s", obj->short_description);
        switch (bits) {
          case (FIND_OBJ_INV):
        strcat(buf, " (carried): \r\n");
        break;
      case (FIND_OBJ_EQUIP):
        strcat(buf, " (used): \r\n");
        break;
      }
        obj = obj->contains;
      }
    }
  }

  for (;obj;obj = obj->next_content) {
    if (obj->item_number == 1) {
      if (!(tmp = strstr(obj->action_description, "Date:")))
        continue;
      strncpy(date, tmp, MAX_INPUT_LENGTH-1);
      if (!(tmp = strchr(date, '\r')))
        continue;
      *tmp = '\0';
      if (!(tmp = strstr(obj->action_description, "From:")))
        continue;
      strncpy(from, tmp, MAX_INPUT_LENGTH-1);
      if (!(tmp = strchr(from, '\r')))
        continue;
      *tmp = '\0';
      num_mails++;
      sprintf(buf, "%s%4d. %s, %s\r\n", buf, num_mails, date, from);
    }
  }
  
  strcat(buf, num_mails ? "\r\n" : "  None.\r\n");
  page_string(ch->desc, buf, 1);
}
