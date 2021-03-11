#include "threadboard.h"
#include "structs.h"
#include "interpreter.h"
#include "db.h"
#include "utils.h"
#include "comm.h"

static thrBoard *thr_board_list = 0;

/*
 * Adds a reply to a message, used only when reading the saved messages from file
 * in XML format.
 */
void addReplyToMessage(xmlNodePtr message, thrMsg *parent)
{
  xmlNodePtr tmp;
  thrMsg *lastReply = 0, *thisReply;

  if (!message || !parent)
  {
    return;
  }
  if (strcasecmp(message->name, "message"))
  {
    return;
  }
  for (lastReply = parent->replies; lastReply && lastReply->next; lastReply = lastReply->next);

  thisReply = (thrMsg*) calloc(1, sizeof(thrMsg));
  thisReply->author = strdup("unknown");
  thisReply->header = strdup("no header");
  thisReply->contents = strdup("no contents!\r\n");
  thisReply->date = strdup("?????? ??:??");
  thisReply->board = parent->board;
  thisReply->number = thisReply->board->top;
  thisReply->replyTo = parent;
  thisReply->board->top++;
  
  for (tmp = message->children; tmp; tmp = tmp->next)
  {
    if (!strcasecmp(tmp->name, "author"))
    {
      free(thisReply->author);
      thisReply->author = strdup(tmp->children->content);
    }
    else if (!strcasecmp(tmp->name, "header"))
    {
      free(thisReply->header);
      thisReply->header = strdup(tmp->children->content);
    }
    else if (!strcasecmp(tmp->name, "date"))
    {
      free(thisReply->date);
      thisReply->date = strdup(tmp->children->content);
    }
    else if (!strcasecmp(tmp->name, "contents"))
    {
      free(thisReply->contents);
      thisReply->contents = strdup(tmp->children->content);
    }
    else if (!strcasecmp(tmp->name, "message"))
    {
      addReplyToMessage(tmp, thisReply);
    }
  } // iteration thorugh child nodes
  if (!lastReply)
  {
    parent->replies = thisReply;
  }
  else 
  {
    lastReply->next = thisReply;
  }
}

/*
 * Adds a message to a board, only used when parsing through the saved XML file.
 */
void addMessagesToBoard(xmlNodePtr msgList, thrBoard *board)
{
  xmlNodePtr tmp, message;
  thrMsg *lastMsg = 0, *thisMsg;

  if (!msgList || !board)
  {
    return;
  }
  if (strcasecmp(msgList->name, "messages"))
  {
    return;
  }
  for (message = msgList->children; message; message = message->next)
  {
    if (strcasecmp(message->name, "message"))
    {
      continue;
    }    
    thisMsg = (thrMsg*) calloc(1, sizeof(thrMsg));
    thisMsg->author = strdup("unknown");
    thisMsg->header = strdup("no header");
    thisMsg->date = strdup("?????? ??:??");
    thisMsg->contents = strdup("no contents!\r\n");
    thisMsg->board = board;
    thisMsg->number = board->top;
    board->top ++;
    for (tmp = message->children; tmp; tmp = tmp->next)
    {
      if (!strcasecmp(tmp->name, "author"))
      {
        free(thisMsg->author);
        thisMsg->author = strdup(tmp->children->content);
      }
      else if (!strcasecmp(tmp->name, "header"))
      {
        free(thisMsg->header);
        thisMsg->header = strdup(tmp->children->content);
      }
      else if (!strcasecmp(tmp->name, "date"))
      {
        free(thisMsg->date);
        thisMsg->date = strdup(tmp->children->content);
      }
      else if (!strcasecmp(tmp->name, "contents"))
      {
        free(thisMsg->contents);
	if (tmp->children)
	  thisMsg->contents = strdup(tmp->children->content);
	else 
	  thisMsg->contents = strdup(""); 
      }
      else if (!strcasecmp(tmp->name, "message"))
      {
        addReplyToMessage(tmp, thisMsg);
      }
    } // iteration thorugh child nodes
    if (!lastMsg)
    {
      board->messages = thisMsg;
    }
    else 
    {
      lastMsg->next = thisMsg;
    }

    lastMsg = thisMsg;
  }
}

/*
 * Central starting point for reading through the vnum specific file for 
 * messages. Refuses to load from file if the board already exists, but returns
 * the existing board instead.
 */
thrBoard *readBoard(int vnum)
{
  xmlDocPtr doc;
  xmlNodePtr root, board, tmp;
  thrBoard *lastBoard = 0, *thisBoard;
  FILE *fl;
  int rnum;

  thr_board_list = 0;
  
  for (thisBoard = thr_board_list; thisBoard; thisBoard = thisBoard->next)
  {
    if (thisBoard->vnum == vnum)
    {
      return thisBoard;
    }
  }
  sprintf(buf, "text/threadboards/%d.board", vnum);
  if (!(fl = fopen(buf, "r"))) // file doesn't exist, create new board, add and return
  {
    rnum = real_object(vnum);
    thisBoard = (thrBoard*) calloc(1, sizeof(thrBoard));
    if (rnum > -1)
    {
      thisBoard->name = strdup(obj_proto[rnum].description);
    }
    else
    {
       thisBoard->name = strdup("threadboard");
    }
    thisBoard->vnum = vnum;
    thisBoard->top = 1;    

    if (!thr_board_list)
    {
      thr_board_list = thisBoard;
    }    
    else
    {  // don't care about vnum order...
      for (lastBoard = thr_board_list; lastBoard && lastBoard->next; lastBoard = lastBoard->next);
      lastBoard->next = thisBoard;
    }
    return thisBoard;
  }

  // file does exist, parse it.
  if (!(doc = xmlParseFile(buf)))
  {
    return NULL;
  }

  if (!(root = doc->children) || strcasecmp(root->name, "threadboards"))
  {    
    xmlFreeDoc(doc);
    return NULL;
  }

  for (board = root->children; board; board = board->next)
  {
    if (strcasecmp(board->name, "board"))
    {
      continue;
    }
    thisBoard = (thrBoard*) calloc(1, sizeof(thrBoard));
    thisBoard->name = strdup("unknown");
    thisBoard->vnum = -1;
    thisBoard->top = 1;    
    for (tmp = board->children; tmp; tmp = tmp->next)
    {
      if (!strcasecmp(tmp->name, "messages"))
      {
        addMessagesToBoard(tmp, thisBoard);
      }
      else if (!strcasecmp(tmp->name, "name"))
      {
        free(thisBoard->name);
        thisBoard->name = strdup(tmp->children->content);
      }
      else if (!strcasecmp(tmp->name, "vnum"))
      {
        thisBoard->vnum = atoi(tmp->children->content);
      } 
    }
    if (!lastBoard)
    {
      thr_board_list = thisBoard;
    }
    else
    {
      lastBoard->next = thisBoard;
    }
    lastBoard = thisBoard;
  }
  xmlFreeDoc(doc);
  return thisBoard;
}

/*
 * Creates an xml node from a message.
 * Used for building the XML document prior to saving to file.
 * This function is recursive, so all the replies to the message
 * will be included as children to the returned node.
 */
xmlNodePtr createXMLMessage(thrMsg *message)
{
   xmlNodePtr msgNode;
   thrMsg *tmp;
   
   if (!message)
   {
     return NULL;
   }
   msgNode = xmlNewNode(NULL, "message");
   if (message->author && *message->author)
   {
     xmlNewChild(msgNode, NULL, "author", message->author);
   }
   else
   {
     xmlNewChild(msgNode, NULL, "author", "Nobody");
   }
   if (message->header && *message->header)
   {
     xmlNewChild(msgNode, NULL, "header", message->header);
   }
   else
   {
     xmlNewChild(msgNode, NULL, "header", "No topic");
   }
   if (message->contents && *message->contents)
   {
     xmlNewChild(msgNode, NULL, "contents", message->contents);
   }
   else
   {
     xmlNewChild(msgNode, NULL, "contents", "No contents!");
   }
   if (message->date && *message->date)
   {
     xmlNewChild(msgNode, NULL, "date", message->date);
   }
   else
   {
     xmlNewChild(msgNode, NULL, "date", "no date");
   }
   
   for (tmp = message->replies; tmp; tmp = tmp->next)
   {
      xmlAddChild(msgNode, createXMLMessage(tmp));
   }
   return msgNode;
}

/*
 * Writes the contents of given threadboard to file.
 */
void saveThreadBoard(thrBoard *board)
{
  thrMsg *msg;   
  xmlDocPtr doc;
  xmlNodePtr root, boardNode, messagesNode;
  FILE *file;

  sprintf(buf, "text/threadboards/%d.board", board->vnum);
  if (!(file = fopen(buf, "w")))
  {
    alog("SYSERR: Couldn't open %s. All new messages will be lost at reboot.", buf);
    return;
  }
  doc = xmlNewDoc("1.0");
  root = xmlNewDocNode(doc, NULL, "threadboards", NULL);
  doc->children = root;
  boardNode = xmlNewChild(root, NULL, "board", NULL);
  xmlNewChild(boardNode, NULL, "name", board->name);
  sprintf(buf, "%d", board->vnum);
  xmlNewChild(boardNode, NULL, "vnum", buf);
  messagesNode = xmlNewChild(boardNode, NULL, "messages", NULL);
  for (msg = board->messages; msg; msg = msg->next)
  {
    xmlAddChild(messagesNode, createXMLMessage(msg));
  }
  xmlDocDump(file, doc);
  xmlFreeDoc(doc);
  fclose(file);
}

/*
 * Lists all the top level messages (thread 'starters' so to speak) on
 * a single board to a player.
 */
void listMessagesOnBoard(thrBoard *board, struct char_data *ch)
{
  thrMsg *message, *replies;
  int numReplies = 0, numMessages = 0;

  if (!board || !ch)
  {
    return;
  }
  for (message = board->messages; message; message = message->next)
  {
    numMessages ++;
    numReplies = 0;
    for (replies = message->replies; replies; replies = replies->next)
    {
      numReplies ++;
    }
    asend_to_char(ch, "[%3d] &g(&y%s&g):&Y %s &g[&r%d&g]&n\r\n", message->number, 
                  message->author, message->header, numReplies);
  }  
  asend_to_char(ch, "%d messages.\r\n", numMessages);
}
/*
 * Shows all the messages within on thread with the given starting points
 * to a player. The 'depth' variable is to indicate indent, nothing else.
 * Note that you don't have to pass the top level messages on a threaded
 * board as parameters, you can give any message on that board. 
 * This method is called recursively to build the message 'tree'.
 */
void showThreadOnBoard(thrMsg *message, struct char_data *ch, int depth)
{
  thrMsg *temp;
  int i;
  char filler[depth+1];

  if (!message || !ch)
  {
    return;
  }
  for (i = 0; i < depth; i ++)
  {
    filler[i] = ' ';
  }
  filler[depth] = 0;
  asend_to_char(ch, "%s[%d] <%s>&g(&y%s&g):&Y %s&n\r\n", filler, message->number, 
                     message->date, message->author, message->header);
  for (temp = message->replies; temp; temp = temp->next)
  {
      showThreadOnBoard(temp, ch, depth+2);
  }
}
/*
 * A utilitarian command that lists all the existing threadboards along with
 * the expected vnum for each.
 */
ACMD(do_thrlist)
{
  thrBoard *board;
  thrMsg *msg;

  int numBoards = 0, numMessages = 0;

  if (!thr_board_list)
  {
    send_to_char("No threaded boards currently active in game.\r\n", ch);
    send_to_char("Note: Boards are dynamically loaded when used. It may be that no board\r\n", ch);
    send_to_char("      has yet been used.\r\n", ch);
    return;
  }
  send_to_char("These threaded boards currently exist:\r\n", ch);

  for (board = thr_board_list; board; board = board->next)
  {
    numMessages = 0;
    for (msg = board->messages; msg; msg = msg->next)
    {
      numMessages ++;
    }
    asend_to_char(ch, " Vnum &g%d&y, %s &g[&y%d&g]&n\r\n", board->vnum, board->name, 
                  numMessages);
    numBoards ++;
  }   
  asend_to_char(ch, "All in all %d boards _used_. There may be more not yet loaded.\r\n", numBoards);
}

/*
 * Does a recursive search in the tree with the given message as root
 * for a message with a certain number.
 */
thrMsg *getReply(int number, thrMsg *parent)
{
  thrMsg *tmp, *tmpSub;

  for (tmp = parent->replies; tmp; tmp = tmp->next)
  {
    if (tmp->number == number)
    {
      return tmp;
    }
    else if ((tmpSub = getReply(number, tmp)))
    {
      return tmpSub;
    }
  } 
  return 0;
}

/* 
 * Searches an entire board for a message with a certain number.
 */
thrMsg *getMessage(int number, thrBoard *board)
{
   thrMsg *tmp, *tmpSub;

   for (tmp = board->messages; tmp; tmp = tmp->next)
   {
     if (tmp->number == number)
     {
       return tmp;
     }
     else if ((tmpSub = getReply(number, tmp)))
     {
       return tmpSub;
     }
   }
   return 0;
}

/*
 * Gets a board by vnum. Vnum in this case means the obj to which the board is attached.
 */
thrBoard *getBoard(int vnum, struct char_data *ch)
{
  thrBoard *board;

  for (board = thr_board_list; board; board = board->next)
  {
    if (board->vnum == vnum)
    {
      break;
    }
  }
  if (!board)
  {
    board = readBoard(vnum);
  }
  if (!board)
  {
    send_to_char("&rError:&n A threadboard special was assigned to an object, but no file \r\n", ch);
    send_to_char("for it could be created or read. File may be corrupted.\r\n", ch);
    asend_to_char(ch, "Please tell a coder about this, the vnum is %d.\r\n", vnum);
    return 0;
  }
  return board;
}

/*
 * Shows a message in its entirety to a player. 
 * Usually used when player types "read #"
 */
void displayMessage(thrMsg *message, struct char_data *ch)
{
  thrMsg *replies;
  int numReplies = 0;

  if (!message || !ch)
  {
    return;
  } 
  asend_to_char(ch, "&g <%s> (&y%s&g):&Y %s&n\r\n", message->date, message->author, message->header);
  asend_to_char(ch, "Message:\r\n%s\r\n", message->contents);
  for (replies = message->replies; replies; replies = replies->next)
  {
    numReplies ++;
  }  
  if (numReplies > 1)
    asend_to_char(ch, "There are &g%d&n replies to this message.\r\n", numReplies);
  else if (numReplies == 1)
    send_to_char("There is one reply to this message.\r\n", ch);
}

/*
 * Removes a message and all its replies, recursively.
 * It also frees the resources allocated by them.
 */
void removeMessage(thrMsg *message)
{
  thrMsg *thisMsg, *temp, *nextMsg;

  if (!message)
  {
    return;
  }

  if (message->replyTo)
  {
    REMOVE_FROM_LIST(message, message->replyTo->replies, next);
  }
  else
  {
    REMOVE_FROM_LIST(message, message->board->messages, next);
  }

  for (thisMsg = message->replies; thisMsg; thisMsg = nextMsg)
  {
    nextMsg = thisMsg->next;
    removeMessage(thisMsg);
  }


  free(message->header);
  free(message->author);
  free(message->date);
  free(message->contents);
  free(message);
}

/*
 * The special function which the obj has to have as SPECIAL to
 * function as a threaded board. Note that setting the SPECIAL
 * on an obj isn't enough; a skeleton <board>...</board> section
 * has to be added manually to the lib/text/threadboards file.
 */
SPECIAL(threadboard)
{
  struct obj_data *obj = (struct obj_data *) me;
  thrBoard *board;
  thrMsg *message, *last, *replyTo = 0;
  int msgNum;
  char *header, arg1[200];
  struct tm *now;
  time_t t;

  if (obj->in_room < 0)
  {
    return 0;
  }
  if (!CMD_IS("look") && !CMD_IS("list") && !CMD_IS("read") && !CMD_IS("remove") &&
      !CMD_IS("write") && !CMD_IS("reply") && !CMD_IS("examine"))
  {
    return 0;
  }

  skip_spaces(&argument);

  if (!(board = getBoard(GET_OBJ_VNUM(obj), ch))) // Also tries to load from file
  {
    return 1;
  }
  
  if (CMD_IS("look") || CMD_IS("examine"))
  {
    if (!*argument)
    {
      return 0;
    }
    if (!strncasecmp("board", argument, strlen(argument)))
    {
      listMessagesOnBoard(board, ch);
      return 1;
    }
    return 0;
  }
  else if (CMD_IS("list") || CMD_IS("read"))
  {    
    saveThreadBoard(board);
    if ((msgNum = atoi(argument)) < 1)
    {
      send_to_char("Give a reasonable number, please.\r\n", ch);
    }
    else if (!(message = getMessage(msgNum, board)))
    {
      asend_to_char(ch, "There is no message with number %d on this board.\r\n", msgNum);
    }
    else
    {
      if (CMD_IS("list"))
      {
        showThreadOnBoard(message, ch, 0);
      }
      else
      {
        displayMessage(message, ch);
      }
    }
  }
  else if (CMD_IS("remove"))
  {    
    if ((msgNum = atoi(argument)) < 1)
    {
      send_to_char("Give a reasonable number, please.\r\n", ch);
    }
    else if (!(message = getMessage(msgNum, board)))
    {
      asend_to_char(ch, "There is no message with number %d on this board.\r\n", msgNum);
    }
    else
    {
      if (message->replies && (GET_LEVEL(ch) < LVL_IMMORT))
      {
        send_to_char("You can't remove that message, there are replies to it.\r\n", ch);
        return 1;
      }
      if (strcasecmp(GET_NAME(ch), message->author) && (GET_LEVEL(ch) < LVL_IMMORT))
      {
        send_to_char("You can't; you didn't write that message.\r\n", ch);
        return 1;
      }
      asend_to_char(ch, "Message %d %sremoved.\r\n", msgNum, 
                   (message->replies)?"and all its replies ":"");
      removeMessage(message);
    }     
  } //remove
  else if (CMD_IS("write") || CMD_IS("reply"))
  {
    if (CMD_IS("write"))
    {
      header = argument;
    }
    else
    {
      header = one_argument(argument, arg1);
      msgNum = atoi(arg1);
      if (!(replyTo = getMessage(msgNum, board)))
      {
        send_to_char("There is no message with that number on this board.\r\n", ch);
        send_to_char("Hint: The command 'reply' in a room with a threadboard is used for\r\n", ch);
        send_to_char("writing replies to notes on that board.\r\n", ch);
        send_to_char("If you want to reply to a tell, step away from this room.\r\n", ch);
        return 1; 
      }
    }
    if (!*header  && !replyTo)
    {
      send_to_char("You must provide a headline too.\r\n", ch);
    }    
    else
    {      
      if (!*header) // no header provided, defaults to re: <original header>
      {
        sprintf(buf, "re: %s", replyTo->header);
        header = buf;
      }
      else
      {
        skip_spaces(&header); // must get rid of first space
      }
      send_to_char("Write your message.  (/s saves /h for help)\r\n\r\n", ch);
      send_to_char("Please format your post to less than 75 characters per line.\r\n", ch);
      send_to_char("This will make your post easier to read for those with smaller screens\r\n",ch);
      send_to_char("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n",ch);
      act("$n starts to write a message.", TRUE, ch, 0, 0, TO_ROOM);
      if (!IS_NPC(ch))
        SET_BIT(PLR_FLAGS(ch), PLR_WRITING);
      message = (thrMsg *) calloc(1, sizeof(thrMsg));      
      message->header = strdup(header);
      message->author = strdup(GET_NAME(ch));
      t = time(0);
      now = localtime(&t);
      sprintf(buf, "%02d%02d%02d %02d:%02d", now->tm_year%100, now->tm_mon, now->tm_mday, 
              now->tm_hour, now->tm_min);
  
      message->date = strdup(buf);
      message->replyTo = replyTo;
      message->board = board;
      message->number = board->top;      
      board->top ++;

      if (replyTo)
      {
        for (last = replyTo->replies; last && last->next; last = last->next);
        if (!last)
        {
          replyTo->replies = message;
        }      
        else
        {
          last->next = message;
        }
      }
      else
      {
        for (last = board->messages; last && last->next; last = last->next);
        if (!last)
        {
          board->messages = message;
        }      
        else
        {
          last->next = message;
        }
      }
      ch->desc->str = &(message->contents);
      ch->desc->max_str = 4096;
    }
  }
  return 1;  
}
