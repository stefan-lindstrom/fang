/******************************************************************************
 * $Id: boards.c,v 1.34 2003/08/14 03:53:53 fang Exp $
 *   
 * File: boards.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *
 *   Revision 1.43  2000/12/25 09:09:06  mud
 *   *** empty log message ***
 *
 *   Revision 1.42  2000/12/13 23:05:55  mud
 *   Did some board clean up, since someone made one and never told me about
 *   it.
 *
 *   Revision 1.41  2000/12/13 22:07:43  mud
 *   Added a board object for the Sea Folk.
 *
 *   Revision 1.40  2000/11/17 22:39:09  mud
 *   fixed guildinv
 *
 *   Revision 1.39  2000/11/08 18:46:35  mud
 *   Bug and idea boards restricted to LVL_GOD
 *
 *   Revision 1.38  2000/10/31 01:28:10  mud
 *   added multiple recipients for mudmail. 5 should be enough..
 *
 *   Revision 1.37  2000/10/15 22:29:13  mud
 *   *** empty log message ***
 *
 *   Revision 1.36  2000/10/06 03:00:42  mud
 *   *** empty log message ***
 *
 *   Revision 1.35  2000/10/06 00:29:59  mud
 *   Two Rivers Board fix
 *
 *   Revision 1.34  2000/10/04 18:18:30  mud
 *   Added a board
 *
 *   Revision 1.33  2000/10/04 12:37:13  mud
 *   Added Newbie Helper board
 *
 *   Revision 1.32  2000/09/27 12:20:26  mud
 *   *** empty log message ***
 *
 *   Revision 1.31  2000/08/17 15:26:49  mud
 *   *** empty log message ***
 *
 *   Revision 1.30  2000/08/17 09:07:33  mud
 *   *** empty log message ***
 *
 *   Revision 1.29  2000/08/17 09:05:25  mud
 *   *** empty log message ***
 *
 *   Revision 1.28  2000/08/16 23:52:43  mud
 *   *** empty log message ***
 *
 *   Revision 1.27  2000/08/16 23:29:22  mud
 *   *** empty log message ***
 *
 *   Revision 1.26  2000/08/02 15:30:46  mud
 *   changed Board_load_boards to not bomb if file doesnt exist
 *
 *   Revision 1.25  2000/07/24 12:23:56  mud
 *   *** empty log message ***
 *
 *   Revision 1.24  2000/07/24 06:24:00  mud
 *   *** empty log message ***
 *
 *   Revision 1.23  2000/07/24 06:15:52  mud
 *   Added global board!
 *
 *   Revision 1.22  2000/06/21 21:15:34  mud
 *   Added DL board for Shai.
 *
 *   Revision 1.21  2000/05/13 13:57:57  mud
 *   No name flag for boards, where players names won't show up on boards
 *   with the ITEM_NONAMEBOARD flag.
 *
 *   Revision 1.20  2000/04/30 09:28:46  mud
 *   Changed hunter board back to 390
 *
 *****************************************************************************/

/* ************************************************************************
*   File: boards.c                                      Part of CircleMUD *
*  Usage: handling of multiple bulletin boards                            *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "db.h"
#include "boards.h"
#include "interpreter.h"
#include "handler.h"
#include "constants.h"
#include "guild.h"
#include "guild_parser.h"

int loaded_boards = 0;
MSG_BOARD *top_of_boards;


void write_board_index( void )
{
  FILE	*hFile;
  MSG_BOARD *brd = top_of_boards;
  char	name[MAX_STRING_LENGTH];
  sprintf( name, "%s/index", BRD_PREFIX );
  hFile = fopen( name, "w" );
  while( brd )
  {
    fprintf( hFile, "%s\n", brd->file );
    brd=brd->next;
  }
  fprintf( hFile, "$\n" ); 
  fclose( hFile );
}

/* search the room ch is standing in to find which board he's looking at */
MSG_BOARD* find_board_by_name( char *key )
{
  MSG_BOARD *brd = top_of_boards;

  while( brd )
  {
    if( !strcasecmp( key, brd->key ) )
      return brd;
    brd = brd->next;
  }
  return NULL;
}

MSG_BOARD* find_xml_board( struct char_data * ch )
{
  struct obj_data *obj;
  MSG_BOARD *brd;

  for (obj = world[ch->in_room].contents; obj; obj = obj->next_content)
    for( brd=top_of_boards; brd; brd=brd->next )
      if( brd->rnum == GET_OBJ_RNUM(obj) )
    return brd;

  return NULL;
}

struct obj_data	*find_board_object( MSG_BOARD *brd )
{
  register struct obj_data *k;
  for ( k = object_list; k; k = k->next )
  {
    if( brd->rnum == GET_OBJ_RNUM( k ) )
      return k;
  }

  return NULL;
}

char  *build_heading( MSG *msg, struct obj_data *obj )
{
  char *tmstr, *heading;
  int len;
  char buf[MAX_INPUT_LENGTH], buf2[MAX_INPUT_LENGTH];

  tmstr = (char *) asctime(localtime(&msg->date));
  *(tmstr + strlen(tmstr) - 1) = '\0';
	    
  if (GET_OBJ_TYPE(obj) != ITEM_NONAMEBOARD)
  {
    sprintf(buf2, "(%s)", msg->author_name );
    buf[0] = '\0';
  } 
  else 
  { 
    sprintf(buf2, "(Someone)");
    sprintf(buf, "[%s]", msg->author_name );
  }
  sprintf(buf, "%s%6.10s %-12s :: %s", buf, tmstr, buf2, msg->header );
  len = strlen(buf) + 1;
  CREATE( heading, char, len );
  memcpy( heading, buf, len );
  heading[len - 1] = '\0';
  return heading;
}

void init_boards(void)
{
  FILE *index;
  //char buf[256];

  //load the index file
  sprintf(buf2, "%s/index", BRD_PREFIX );

  if (!(index = fopen(buf2, "r"))) 
  {
    xlog( SYS_ERROR, LVL_IMMORT, "Unable to open board index", TRUE );
    exit( 1);
  }
   
  /* first, count the number of records in the file so we can alloc */
  fscanf( index, "%s\n", buf1 );
   
  while (*buf1 != '$') 
  {
    load_xml_board( buf1 );
    fscanf( index, "%s\n", buf1 );
  }
  loaded_boards = 1;
}


SPECIAL(gen_board)
{
  MSG_BOARD *brd;

  if (!loaded_boards)
    init_boards();
  if (!ch->desc)
    return 0;

  if (cmd != find_command("write") 
    && cmd != find_command("look") 
    && cmd != find_command("examine") 
    && cmd != find_command("read") 
    && cmd != find_command("remove") 
    && cmd != find_command("edit")
    && cmd != find_command("reload")
    )
    return 0;

  /**
   * Prevent people from tirggering boards that are carried. 
   * should stop the (someone)[Name] bug from happening. 
   * as well to allow ppl toremove their old posts, even if they
   * -do- have a board in their inventory. 
   */
  if (((struct obj_data *)me)->carried_by)
    return 0;

  if (IS_AFFECTED(ch,AFF_DREAMING) && *argument) 
  {
    send_to_char("You can't use boards while dreaming!\r\n",ch);
    return 0;
  }

  if( (brd = find_xml_board(ch) ) == NULL ) 
  {
    mlog("SYSERR:  degenerate xml board!  (what the hell...)");
    return 0;
  }

  if (cmd == find_command("write")) {
    Board_write_message( brd, ch, argument, (struct obj_data *)me);
    return 1;
  } else if (cmd == find_command("look") || cmd == find_command("examine"))
    return (Board_show_board( brd, ch, argument, (struct obj_data *)me));
  else if (cmd == find_command("reload"))
    return Board_reload_board( brd, ch, argument );
  else if (cmd == find_command("read"))
    return (Board_display_msg( brd, ch, argument, (struct obj_data *)me));
  else if (cmd == find_command("remove"))
    return (Board_remove_msg( brd, ch, argument));
  else if (cmd == find_command("edit"))
  { Board_edit_message( brd, ch, argument, (struct obj_data *)me );
    return 1;
  }
  else
    return 0;
}

void Board_write_message( MSG_BOARD *brd, struct char_data * ch, char *arg, struct obj_data *board)
{
  int len;
  MSG *msg, *work;

  if( GET_LEVEL(ch) < BRD_WRITE( brd ) )
  {
    send_to_char("You are not holy enough to write on this board.\r\n", ch);
    return;
  }

  if( PLR_FLAGGED( ch, PLR_CANTPOST ) )
  {
    send_to_char( "A guard shows up and confiscates your post.\r\n", ch );
    return;
  }
  
  if( brd->guild && GET_LEVEL( ch ) < LVL_IMMORT )
  {
    struct char_guild_element  *guild;
    guild = GET_CHAR_GUILDS(ch);
    while ( guild ) 
    {
      if( guild->guild->id == brd->guild 
	&& STATUS_FLAGGED(guild->guildie, STATUS_MEMBER)) 
	break;
      guild = guild->next;
    }
    if( !guild )
    {
      send_to_char( "You aren't a member of that guild!\r\n", ch );
      return;
    }
  }

  if( brd->num_of_msgs >= MAX_BOARD_MESSAGES) 
  {
    send_to_char("The board is full.\r\n", ch);
    return;
  }

  /* skip blanks */
  skip_spaces(&arg);
  delete_doubledollar(arg);

  /* JE 27 Oct 95 - Truncate headline at 80 chars if it's longer than that */
  arg[81] = '\0';

  if (!*arg) {
    send_to_char("We must have a headline!\r\n", ch);
    return;
  }

  CREATE( msg, MSG, 1 );
  CREATE( msg->msg, char, MAX_MESSAGE_LENGTH );
  
  len = strlen( arg ) + 1;
  CREATE( msg->header, char, len );
  memcpy( msg->header, arg, len );
  msg->header[len - 1] = '\0';
  msg->level = GET_LEVEL(ch);
  msg->author = ch->id;
  msg->author_name = strdup( GET_NAME( ch ) );
  msg->heading = NULL;
  msg->edit_name = NULL;
  msg->date = time( NULL );

  send_to_char("Write your message.  (/s saves /h for help)\r\n\r\n", ch);
  send_to_char("Please format your post to less than 75 characters per line.\r\n", ch);
  send_to_char("This will make your post easier to read for those with smaller screens\r\n",ch);
  send_to_char("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n",ch);

  act("$n starts to write a message.", TRUE, ch, 0, 0, TO_ROOM);

  if (!IS_NPC(ch))
    SET_BIT(PLR_FLAGS(ch), PLR_WRITING);

  ch->desc->str = &(msg->msg);
  ch->desc->max_str = MAX_MESSAGE_LENGTH;
  ch->desc->mail_to[0] = brd->rnum + BOARD_MAGIC;
  ch->desc->mail_to[1] = (long)brd;
  ch->desc->current_text_line = 1;

  brd->num_of_msgs++;
  msg->slot_num = brd->num_of_msgs;
  msg->next = NULL;
  if( brd->msgs )
  {
    for( work=brd->msgs; work; work=work->next )
    {
      if( work->next == NULL )
      {
	work->next = msg;
	work = work->next;
      }
    }
  }
  else
    brd->msgs = msg;
  brd->stamp = time( NULL );
}


void Board_edit_message( MSG_BOARD *brd, struct char_data * ch, char *arg, struct obj_data *board)
{
  char *tmpstr;
  
  char number[MAX_STRING_LENGTH];
  int msg;
  MSG *pmsg;

  /* skip blanks */
  one_argument(arg, number);

  if( !board )
    board = find_board_object( brd );
  if (!*number || !board )
  {
    send_to_char( "Edit which message?!\r\n", ch );
    return;
  }
  if (!isdigit(*number) || (!(msg = atoi(number))))
  {
    send_to_char( "Are you trying to be funny?  That's no message!\r\n", ch );
    return;
  }

  if (msg < 1 || msg > brd->num_of_msgs) 
  {
    send_to_char("That message exists only in your imagination.\r\n", ch);
    return;
  }

  if( brd->guild && GET_LEVEL( ch ) < LVL_IMMORT )
  {
    struct char_guild_element  *guild;
    guild = GET_CHAR_GUILDS(ch);
    while ( guild ) 
    {
      if( guild->guild->id == brd->guild 
	&& STATUS_FLAGGED(guild->guildie, STATUS_MEMBER)) 
	break;
      guild = guild->next;
    }
    if( !guild )
    {
      send_to_char( "You aren't a member of that guild!\r\n", ch );
      return;
    }
  }

  for( pmsg=brd->msgs; pmsg; pmsg=pmsg->next )
    if( pmsg->slot_num ==  msg )
      break;

  if( !pmsg )
  {
    send_to_char("Sorry, the board is not working.\r\n", ch);
    mlog("SYSERR: Board is screwed up.");
    return;
  }
  
  if( !pmsg->heading )
    pmsg->heading = build_heading( pmsg, board );

  sprintf(buf, "(%s)", GET_NAME(ch));
  if( GET_LEVEL( ch )<LVL_GRGOD && !(strstr( pmsg->heading, buf )) ) 
  {
    send_to_char("You are not holy enough to edit other people's messages.\r\n", ch);
    return;
  }
  else
    if( GET_LEVEL( ch )>=LVL_GRGOD )
      pmsg->edit_name = strdup( GET_NAME( ch ) );

  if( GET_LEVEL(ch) < pmsg->level )
  {
    send_to_char("You can't edit a message holier than yourself.\r\n", ch);
    return;
  }

  axlog( SYS_TOOLCRAFT, LVL_IMMORT, TRUE, "%s is editing message %i from the %s board", ch->player.name, msg, brd->key );
  CREATE( tmpstr, char, MAX_MESSAGE_LENGTH );
  strcpy( tmpstr, pmsg->msg );
  free( pmsg->msg );
  pmsg->msg = tmpstr;
  
  send_to_char("Write your message.  (/s saves /h for help)\r\n\r\n", ch);
  send_to_char("Please format your post to less than 75 characters per line.\r\n", ch);
  send_to_char("This will make your post easier to read for those with smaller screens\r\n",ch);
  send_to_char("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n",ch);

  act("$n starts to write a message.", TRUE, ch, 0, 0, TO_ROOM);

  if (!IS_NPC(ch))
    SET_BIT(PLR_FLAGS(ch), PLR_WRITING);

  ch->desc->str = &(pmsg->msg);
  ch->desc->backstr = str_dup( pmsg->msg );
  ch->desc->max_str = MAX_MESSAGE_LENGTH;
  ch->desc->mail_to[0] = brd->rnum + BOARD_MAGIC;
  ch->desc->mail_to[1] = (long)brd;
  ch->desc->current_text_line = count_newlines( &(pmsg->msg) ) + 1;

  pmsg->date = time( NULL );
  brd->stamp = time( NULL );
}


int Board_show_board(MSG_BOARD *brd, struct char_data * ch, char *arg, struct obj_data *board)
{
  struct board_stamp  *bstamp;
  char	tmp[MAX_STRING_LENGTH], buf[MAX_STRING_LENGTH], *tmp_ptr = NULL;
  MSG	*msg;

  if (!ch->desc)
    return 0;

  one_argument(arg, tmp);

  //board object must be found to know about NONAME flag
  if( !board )
    board = find_board_object( brd );
  if ( !board )
  {
    send_to_char( "That board doesn't seem to be loaded at this time.  Maybe it's in someones inventory.", ch );
    return 1;
  }

  if( !*tmp || !isname(tmp, board->name))
    return 0;

  if (GET_LEVEL(ch) < BRD_READ( brd ) )
  {
    send_to_char("You try but fail to understand the holy words.\r\n", ch);
    return 1;
  }

  if( brd->guild && GET_LEVEL( ch ) < LVL_IMMORT )
  {
    struct char_guild_element  *guild;
    guild = GET_CHAR_GUILDS(ch);
    while ( guild ) 
    {
      if( guild->guild->id == brd->guild 
	&& STATUS_FLAGGED(guild->guildie, STATUS_MEMBER)) 
	break;
      guild = guild->next;
    }
    if( !guild )
    {
      send_to_char( "You aren't a member of that guild!\r\n", ch );
      return 1;
    }
  }

  act("$n studies the board.", TRUE, ch, 0, 0, TO_ROOM);

  strcpy( buf,
     "This is a bulletin board.  Usage: READ/REMOVE <messg #>, WRITE <header>.\r\n"
     "You will need to look at the board to save your message.\r\n");
  if( !brd->num_of_msgs )
    strcat(buf, "The board is empty.\r\n");
  else 
  {
    sprintf(buf + strlen(buf), "There are %d messages on the board.\r\n",brd->num_of_msgs );
    for( msg = brd->msgs; msg; msg=msg->next )
    {
      if( !msg->heading )
	msg->heading = build_heading( msg, board );

      if( msg->heading )
      {
	strcpy(tmp, msg->heading );
	if (GET_OBJ_TYPE(board) == ITEM_NONAMEBOARD && (tmp_ptr = strchr(tmp, ']')) && GET_LEVEL(ch) < LVL_IMMORT) 
	{
	  strcpy(buf2, tmp_ptr+1);
	  strcpy(tmp, buf2);
	}
	sprintf(buf + strlen(buf), "%-2d : %s\r\n", msg->slot_num, tmp);
      }
      else 
      {
	mlog("SYSERR: The board is fubar.");
	send_to_char("Sorry, the board isn't working.\r\n", ch);
	return 1;
      }
    }
  }
  bstamp = ch->brd_stamp;
  while( bstamp )
  {
    if( bstamp->vnum == brd->vnum )
      break;
    bstamp = bstamp->next;
  }
  if( !bstamp )
  {
    CREATE( bstamp, struct board_stamp, 1 );
    bstamp->vnum = brd->vnum;
    bstamp->next = ch->brd_stamp;
    ch->brd_stamp = bstamp;
  }
  bstamp->stamp = time( NULL );

  page_string(ch->desc, buf, 1);

  return 1;
}


int Board_display_msg( MSG_BOARD *brd, struct char_data * ch, char *arg, struct obj_data *board)
{
  char number[MAX_STRING_LENGTH], buffer[MAX_STRING_LENGTH], tmp[MAX_STRING_LENGTH], *tmp_ptr = NULL;
  int msg;
  MSG *pmsg;

  one_argument(arg, number);
  if( !board )
    board = find_board_object( brd );
  if (!*number || !board )
    return 0;
  if (isname(number, board->name))	/* so "read board" works */
    return (Board_show_board( brd, ch, arg, board));
  if (!isdigit(*number) || (!(msg = atoi(number))))
    return 0;

  if (GET_LEVEL(ch) < BRD_READ( brd ) )
  {
    send_to_char("You try but fail to understand the holy words.\r\n", ch);
    return 1;
  }

  if( brd->guild && GET_LEVEL( ch ) < LVL_IMMORT )
  {
    struct char_guild_element  *guild;
    guild = GET_CHAR_GUILDS(ch);
    while ( guild ) 
    {
      if( guild->guild->id == brd->guild 
	&& STATUS_FLAGGED(guild->guildie, STATUS_MEMBER)) 
	break;
      guild = guild->next;
    }
    if( !guild )
    {
      send_to_char( "You aren't a member of that guild!\r\n", ch );
      return 1;
    }
  }

  if( !brd->num_of_msgs ) 
  {
    send_to_char("The board is empty!\r\n", ch);
    return (1);
  }
  if (msg < 1 || msg > brd->num_of_msgs) {
    send_to_char("That message exists only in your imagination.\r\n", ch);
    return (1);
  }

  for( pmsg=brd->msgs; pmsg; pmsg=pmsg->next )
    if( pmsg->slot_num ==  msg )
      break;

  if( !pmsg )
  {
    send_to_char("Sorry, the board is not working.\r\n", ch);
    mlog("SYSERR: Board is screwed up.");
    return 1;
  }
  if( !pmsg->heading )
    pmsg->heading = build_heading( pmsg, board );

  if( !pmsg->heading )
  {
    send_to_char("That message appears to be screwed up.\r\n", ch);
    return 1;
  }
  if( !pmsg->msg ) 
  {
    send_to_char("That message seems to be empty.\r\n", ch);
    return 1;
  }
  strcpy(tmp, pmsg->heading );
  if (GET_OBJ_TYPE(board) == ITEM_NONAMEBOARD && (tmp_ptr = strchr(tmp, ']'))) 
  {
    strcpy(buf2, tmp_ptr+1);
    strcpy(tmp, buf2);
  }
  if( pmsg->edit_name )
    sprintf( tmp, "%s\r\n&1&WEdited by %s&n", tmp, pmsg->edit_name );
  
  sprintf(buffer, "Message %d : %s\r\n\r\n%s\r\n", msg, tmp,
      pmsg->msg );

  page_string(ch->desc, buffer, 1);

  return 1;
}


int Board_remove_msg( MSG_BOARD *brd, struct char_data * ch, char *arg)
{
  int  msg, slot_num;
  char number[MAX_INPUT_LENGTH], buf[MAX_INPUT_LENGTH];
  MSG *pmsg, *work=NULL;
  struct descriptor_data *d;

  one_argument(arg, number);

  if (!*number || !isdigit(*number))
    return 0;
  if (!(msg = atoi(number)))
    return (0);

  if( brd->guild && GET_LEVEL( ch ) < LVL_IMMORT )
  {
    struct char_guild_element  *guild;
    guild = GET_CHAR_GUILDS(ch);
    while ( guild ) 
    {
      if( guild->guild->id == brd->guild 
	&& STATUS_FLAGGED(guild->guildie, STATUS_MEMBER)) 
	break;
      guild = guild->next;
    }
    if( !guild )
    {
      send_to_char( "You aren't a member of that guild!\r\n", ch );
      return 1;
    }
  }

  if( !brd->num_of_msgs )
  {
    send_to_char("The board is empty!\r\n", ch);
    return 1;
  }
  if (msg < 1 || msg > brd->num_of_msgs ) 
  {
    send_to_char("That message exists only in your imagination.\r\n", ch);
    return 1;
  }

  for( pmsg=brd->msgs; pmsg; pmsg=pmsg->next )
  {
    if( pmsg->slot_num == msg )
      break;
    work = pmsg;
  }

  if( !pmsg )
  {
    send_to_char("Sorry, the board is not working.\r\n", ch);
    mlog("SYSERR: Board is screwed up.");
    return 1;
  }
  if( !pmsg->heading )
  {
    if( GET_LEVEL(ch) < BRD_REMOVE( brd ) && strcmp( GET_NAME(ch), pmsg->author_name ) )
    {
      send_to_char("You are not holy enough to remove other people's messages.\r\n", ch);
      return 1;
    }
  }
  else
  {
    sprintf(buf, "(%s)", GET_NAME(ch));
    if( GET_LEVEL(ch) < BRD_REMOVE( brd ) && !(strstr( pmsg->heading, buf )) ) 
    {
      send_to_char("You are not holy enough to remove other people's messages.\r\n", ch);
      return 1;
    }
  }

  if( GET_LEVEL(ch) < pmsg->level )
  {
    send_to_char("You can't remove a message holier than yourself.\r\n", ch);
    return 1;
  }
  for (d = descriptor_list; d; d = d->next)
    if (!d->connected && d->str == &(pmsg->msg) ) 
    {
      send_to_char("At least wait until the author is finished before removing it!\r\n", ch);
      return 1;
    }

  if( pmsg->msg )
    free( pmsg->msg );
 
  if( pmsg->heading )
    free( pmsg->heading );

  if( work )
  {
    work->next = pmsg->next;
    slot_num = pmsg->slot_num;
  }
  else
  {
    brd->msgs = pmsg->next;
    slot_num = 2;
  }
  
  free( pmsg );

  if( brd->msgs )
    for( work=brd->msgs; work; work=work->next )
    {
      if( work->slot_num >= slot_num )
	work->slot_num--;
    }
  brd->num_of_msgs--;
  send_to_char("Message removed.\r\n", ch);
  sprintf(buf, "$n just removed message %d.", msg);
  act(buf, FALSE, ch, 0, 0, TO_ROOM);
  Board_save_board( brd );

  return 1;
}


void Board_save_board( MSG_BOARD *brd )
{
  save_xml_board( brd );
}


int	Board_reload_board( MSG_BOARD *brd, struct char_data *ch, char *arg )
{
  MSG_BOARD *curbrd;
  MSG *msg, *next;
  char	tmp[MAX_STRING_LENGTH];

  one_argument( arg, tmp );
  if( !*tmp || (!isname(tmp, brd->key) && strcasecmp( tmp, "board" )) )
    return 0;

  load_xml_board( brd->file );
  if( brd == top_of_boards )
    top_of_boards = brd->next;
  else
  {
    for( curbrd = top_of_boards; curbrd; curbrd = curbrd->next )
      if( curbrd->next == brd )
	break;
    if( curbrd->next )
      curbrd->next = brd->next;
    else
    {
      xlog( SYS_ERROR, LVL_IMMORT, "SYSERR: Trying to remove a board that's not there", TRUE );
      xlog( SYS_ERROR, LVL_IMMORT, "SYSERR: Reboot/hotboot is probably necessary", TRUE );
      send_to_char( "An error has occured.  Get someone to reboot the system.\r\n", ch );
      return 1;
    }
  }
  
  for( msg = brd->msgs; msg; msg=next )
  {
    next = msg->next;
    if( msg->header ) free( msg->header );
    if( msg->heading ) free( msg->heading );
    if( msg->author_name ) free( msg->author_name );
    if( msg->edit_name ) free( msg->edit_name );
    if( msg->msg ) free( msg->msg );
    free( msg );
  }
  free( brd );

  send_to_char( "Okay!\r\n", ch );
  return 1;
}

ACMD( do_edit ) 
{
  send_to_char( "There is no board here to edit.\r\n", ch );
}

ACMD( do_list ) 
{
  int	go;
  char	*arg;
  char	item[MAX_STRING_LENGTH];
  struct board_stamp  *bstamp;
  MSG_BOARD *brd;
  struct obj_data *board;

  arg = one_argument( argument, item );

  if( !*item )
  {
    send_to_char( "List what?\r\n", ch );
    return;
  }

  if( !strncasecmp( item, "boards", strlen( item ) ) )
  {
    brd = top_of_boards;
    if( GET_LEVEL( ch ) >= LVL_GOD )
      send_to_char( "\r\nnew key             at    file            rea wri rem gd title\r\n--- --------------- ----- --------------- --- --- --- -- -----\r\n", ch );
    else
      send_to_char( "\r\nnew title\r\n--- --- --------------- -----\r\n", ch );

    while( brd ) 
    {
      board = find_board_object( brd );
      bstamp = ch->brd_stamp;
      go = 1;
      if( GET_LEVEL( ch ) < BRD_READ( brd ) )
	go = 0;
    
      if( brd->guild && GET_LEVEL( ch ) < LVL_IMMORT && go )
      {
        struct char_guild_element  *guild;
	guild = GET_CHAR_GUILDS(ch);
        while ( guild ) 
        {
	  if( guild->guild->id == brd->guild 
	    && STATUS_FLAGGED(guild->guildie, STATUS_MEMBER)) 
	    break;
	  guild = guild->next;
	}
	if( !guild )
	  go = 0;
      }
      if( go )
      {
	while( bstamp )
	{
	  if( bstamp->vnum == brd->vnum )
	    break;
	  bstamp=bstamp->next;
	}
	if( bstamp && bstamp->stamp > brd->stamp )
	  strcpy( item, "[ ]" );
	else
	  strcpy( item, "[*]" );

	if( GET_LEVEL( ch ) >= LVL_GOD )
	{
	  sprintf( item, "%s %-15s", item, brd->key );
          if( board && board->in_room > 0 )
	    sprintf( item, "%s %-5d", item, world[board->in_room].number );
          else
            sprintf( item, "%s %-15s", item, "(not loaded)" );
	  sprintf( item, "%s %-15s %3ld %3ld %3ld %2d", 
	    item, brd->file,
	    BRD_READ( brd ), BRD_WRITE( brd), BRD_REMOVE( brd ),
	    brd->guild );
	}

	sprintf( item, "%s %s\r\n", item, brd->title );
	send_to_char( item, ch );
      }
      brd = brd->next;
    }
  }
}

/*Defines to make the next function more readable*/
#define TO_LOWER( a ) { int b=0; while((a)[b]){ (a)[b]=tolower((a)[b]);b++;}}
#define OPT_KEY	    0
#define OPT_TITLE   1
#define OPT_READ    2
#define OPT_WRITE   3
#define OPT_REMOVE  4
#define OPT_GUILD   5
#define OPT_FILE    6
#define OPT_NEW     7
#define OPT_DELETE  8
#define OPT_GLOBAL  9
#define OPT_NONAME  10
void  do_board_set( struct char_data *ch, char *arg )
{
  int	opt;
  char	log[MAX_STRING_LENGTH];
  char	buf[MAX_STRING_LENGTH];
  char	str1[MAX_STRING_LENGTH];
  char	*usage = "Set brd <key> <token> <value>\r\ntoken = 'key', 'title', 'read', 'write', 'remove', 'guild', 'file', 'global', 'noname'\r\n'new' or 'delete'\r\n";
  char	str2[MAX_STRING_LENGTH];
  char	*options[] = 
    { "key", "title", "read", "write", "remove", "guild", "file", "new", "delete", "global", "noname", 0 };
  MSG_BOARD *brd;

  //parse the command string
  half_chop( arg, str1, buf );
  half_chop( buf, str2, buf );
  for( opt = 0; options[opt]; opt++ )
  {
    if( !strncasecmp( options[opt], str2, strlen( str2 ) ) )
      break;
  }
  if( !*str1 || !*str2 || !*buf || !options[opt] )
  {
    send_to_char( usage, ch );
    return;
  }

  sprintf( log, "%s 'set brd", GET_NAME( ch ) );
  brd = find_board_by_name( str1 ); 
  if( !brd && opt !=OPT_NEW )
  {
    sprintf( buf, "The board, '%s', doesn't exist\r\n", str1 );
    send_to_char( buf, ch );
    return;
  }

  if( brd )
    sprintf( log, "%s %s", log, brd->key );
  else
    sprintf( log, "%s %s", log, str1 );


  switch( opt )
  {
    case OPT_KEY:
    {
      half_chop( buf, str1, buf );
      TO_LOWER( str1 );
      if( strlen( str1 ) > 49 )
	str1[49] = 0;
      strcpy( brd->key, str1 );
      sprintf( buf, "Board key set to '%s'.\r\n", brd->key );
      sprintf( log, "%s key %s.'", log, brd->key );
      break;
    }
    case OPT_TITLE:
    {
      if( brd->title )
	free( brd->title );
      CREATE( brd->title, char, strlen( buf ) + 1 );
      strcpy( brd->title, buf );
      sprintf( buf, "Board '%s' title set to '%s'.\r\n", brd->key, brd->title );
      sprintf( log, "%s title %s.'", log, brd->title );
      break;
    }
    case OPT_READ:
    {
      half_chop( buf, str1, buf );
      BRD_SET_READ( brd, atol( str1 ) );
      sprintf( buf, "Board '%s' read level set to '%ld'.\r\n", brd->key, BRD_READ( brd ) );
      sprintf( log, "%s read %ld.'", log, BRD_READ( brd ) );
      break;
    }
    case OPT_WRITE:
    {
      half_chop( buf, str1, buf );
      BRD_SET_WRITE( brd, atol( str1 ) );
      sprintf( buf, "Board '%s' write level set to '%ld'.\r\n", brd->key, BRD_WRITE( brd ) );
      sprintf( log, "%s write %ld.'", log, BRD_WRITE( brd ) );
      break;
    }
    case OPT_REMOVE:
    {
      half_chop( buf, str1, buf );
      BRD_SET_REMOVE( brd, atol( str1 ) );
      sprintf( buf, "Board '%s' remove level set to '%ld'.\r\n", brd->key, BRD_REMOVE( brd ) );
      sprintf( log, "%s remove %ld.'", log, BRD_REMOVE( brd ) );
      break;
    }
    case OPT_GUILD:
    {
      half_chop( buf, str1, buf );
      brd->guild = atoi( str1 );
      sprintf( buf, "Board '%s' guild set to '%d'.\r\n", brd->key, brd->guild );
      sprintf( log, "%s guild %d.'", log, brd->guild );
      break;
    }
    case OPT_FILE:
    {
      half_chop( buf, str1, buf );
      TO_LOWER( str1 );
      if( strlen( str1 ) > 49 )
	str1[49] = 0;
      sprintf( buf, "%s/%s", BRD_PREFIX, brd->file );
      unlink( buf );
      strcpy( brd->file, str1 );
      write_board_index();
      sprintf( buf, "Board '%s' filename set to '%s'.\r\n", brd->key, brd->file );
      sprintf( log, "%s file %s.'", log, brd->file );
      break;
    }
    case OPT_GLOBAL:
      half_chop( buf, str1, buf );
      if( !strcasecmp( str1, "on" ) )
	SET_BRD_FLAG( brd, BRD_FLAG_GLOBAL );
      else if( !strcasecmp( str1, "off" ) )
	CLEAR_BRD_FLAG( brd, BRD_FLAG_GLOBAL );
      else
      {
	send_to_char( "Value must be 'on' or 'off'.\r\n", ch );
	return;
      }
      sprintf( buf, "Board '%s' global set to '%s'.\r\n", brd->key, str1 );
      sprintf( log, "%s global %s.'", log, str1 );
      break;
    case OPT_DELETE:
      if( !strcmp( buf, "yes delete" ) )
      {
	MSG_BOARD	*brd2 = top_of_boards;
	if( brd == top_of_boards )
	  top_of_boards = brd->next;
	else
	{
	  while( brd2 )
	  {
	    if( brd2->next == brd )
	      break;
	    brd2=brd2->next;
	  }
	  if( brd2 )
	    brd2->next = brd->next;
	}
	write_board_index();
	sprintf( buf, "%s/%s", BRD_PREFIX, brd->file );
	unlink( buf );
      }
      else
      {
	send_to_char( "Deleting a board is not reversable.\r\n"
		      "Please use 'set brd <key> delete yes delete' to confirm.\r\n", ch );

	axlog( SYS_DEBUG, MAX(LVL_GOD, GET_INVIS_LEV(ch)), FALSE, "Trying to delete with '%s'", buf );
	return;
      }
      sprintf( buf, "Board '%s' has been deleted.\r\n", brd->key );
      sprintf( log, "%s delete.'", log );
      break;
    case OPT_NEW:
      CREATE( brd, MSG_BOARD, 1 );
      half_chop( buf, str2, buf );
      half_chop( buf, brd->file, buf );
      if( !*buf )
      {
	send_to_char( "Creating a board requires the following format:\r\n"
		      "set brd <new key> new <vnum> <file> <title>\r\n", ch );
	free( brd );
	return;
      }
      brd->vnum = atol( str2 );
      brd->rnum = real_object( brd->vnum );
      if( brd->rnum == -1 )
      {
	send_to_char( "That vnum does not exist.  Cannot create the board.\r\n", ch );
	free( brd );
	return;
      }
      strcpy( brd->key, str1 );
      CREATE( brd->title, char, strlen( buf ) + 1 );
      strcpy( brd->title, buf );
      brd->next = top_of_boards;
      top_of_boards = brd;
      sprintf( buf, "Board '%s' has been created.\r\n", brd->key );
      sprintf( log, "%s new %ld %s %s.'", log, brd->vnum, brd->file, brd->title );
      Board_save_board( brd );
      write_board_index();
      break;
    case OPT_NONAME:
      sprintf( buf, "set board %s %s has not been implemented yet.\r\n", brd->key, str1 );
      send_to_char( buf, ch );
      break;
    default:
      send_to_char( "You just did the impossible!\r\n", ch );
      axlog( SYS_ERROR, LVL_GOD, TRUE, "SYSERR: %s tried to execute set board %s %s!", GET_NAME( ch ), brd->key, str1 );
  }
  send_to_char( buf, ch );
  xlog( SYS_SET, MAX( LVL_GOD, GET_INVIS_LEV( ch ) ), log, TRUE );
  save_xml_board( brd );
}
