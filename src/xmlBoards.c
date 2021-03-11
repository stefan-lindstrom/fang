/******************************************************************************
 * $Id: xmlBoards.c,v 1.10 2003/06/17 10:30:05 fang Exp $
 *   
 * File: xmlRooms.c
 *
 * Created: Wed Oct 10 2002 by Darren Benham (fischer@thepics.org)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * Storing/parsing boards and board index in xml format
 * 
 *   $Log: xmlRooms.c,v 
 *
 *****************************************************************************/

#include <stddef.h>
#include <fnmatch.h>

#include "utils.h"
#include "boards.h"
#include "handler.h"
#include "db.h"
#include "comm.h"
#include "constants.h"
#include "strutil.h"
#include "namedspec.h"
#include "dg_olc.h"
#include "xmlRooms.h"


static void dumpBoardToXmlFile( MSG_BOARD *board, char *file )
{
  xmlNodePtr tree, msgs, msg;
  xmlDocPtr doc;
  MSG	*temp;
  char	buf[255];

  doc = xmlNewDoc("1.0");
  
  tree = xmlNewDocNode(doc,NULL,"board",NULL);
  doc->children = tree;

  sprintf( buf, "%li", board->vnum );
  xmlNewChild( tree, NULL, "vnum", buf );
  sprintf( buf, "%li", BRD_READ( board ) );
  xmlNewChild( tree, NULL, "read", buf );
  sprintf( buf, "%li", BRD_WRITE( board ));
  xmlNewChild( tree, NULL, "write", buf );
  sprintf( buf, "%li", BRD_REMOVE( board ));
  xmlNewChild( tree, NULL, "remove", buf );
  sprintf( buf, "%li", board->flags );
  xmlNewChild( tree, NULL, "flags", buf );
  sprintf( buf, "%i", board->guild );
  xmlNewChild( tree, NULL, "guild", buf );
  sprintf( buf, "%s", board->key );
  xmlNewChild( tree, NULL, "key", buf );
  xmlNewChild( tree, NULL, "title", internalXmlString( board->title, doc ) );

  if( board->msgs )
  {
    msgs = xmlNewNode( NULL, "messages" );
    for( temp = board->msgs; temp; temp=temp->next )
    {
      msg = xmlNewNode( NULL, "message" );
      if( temp->header )
	xmlNewChild( msg, NULL, "header", internalXmlString( temp->header, doc ) );
      else
	xmlNewChild( msg, NULL, "heading", internalXmlString( temp->heading, doc ) );
      sprintf( buf, "%i", temp->slot_num );
      xmlNewChild( msg, NULL, "slot", buf );  
      sprintf( buf, "%li", temp->level );
      xmlNewChild( msg, NULL, "level", buf );  
      sprintf( buf, "%li", temp->author );
      xmlNewChild( msg, NULL, "author", buf );  
      sprintf( buf, "%li", temp->date );
      xmlNewChild( msg, NULL, "date", buf );  
      if( temp->author_name) xmlNewChild( msg, NULL, "authorname", internalXmlString( temp->author_name, doc ) );
      if( temp->edit_name) xmlNewChild( msg, NULL, "editname", internalXmlString( temp->edit_name, doc ) );
      xmlNewChild( msg, NULL, "text", internalXmlString( temp->msg, doc ) );
      xmlAddChild( msgs, msg );
    }
    xmlAddChild( tree, msgs );
  }

//  xmlMakeMessages( doc, tree, board );

  sprintf( buf, "%s/%s", BRD_PREFIX, file );
  xmlSaveFile( buf, doc );
  xmlFreeDoc(doc);
}

static MSG*  fetchXmlMessage( xmlNodePtr xmlMsg )
{
  xmlNodePtr temp;
  MSG	*work;

  if( strncasecmp( "message", xmlMsg->name, strlen( xmlMsg->name ) ) ) 
  {
    axlog( SYS_OLC, LVL_IMMORT, TRUE, "XML-leaf (%s) is not a message-object!", xmlMsg->name );
    return NULL;
  }
  CREATE( work, MSG, 1 );
  work->next = NULL;
  
  for (temp = xmlMsg->children; NULL != temp; temp = temp->next) 
  {
      if (!strcasecmp(temp->name,"header")) 
	work->header =xmlToString( temp->children );
      else if (!strcasecmp(temp->name,"heading")) 
	work->heading =xmlToString( temp->children );
      else if (!strcasecmp(temp->name,"level")) 
	work->level =atoi( temp->children->content );
      else if (!strcasecmp(temp->name,"author")) 
	work->author =atoi( temp->children->content );
      else if (!strcasecmp(temp->name,"dater")) 
	work->date =atoi( temp->children->content );
      else if (!strcasecmp(temp->name,"date")) 
	work->date =atoi( temp->children->content );
      else if (!strcasecmp(temp->name,"slot")) 
	work->slot_num =atoi( temp->children->content );
      else if (!strcasecmp(temp->name,"authorname")) 
	work->author_name = xmlToString( temp->children );
      else if (!strcasecmp(temp->name,"editname")) 
	work->edit_name = xmlToString( temp->children );
      else if (!strcasecmp(temp->name,"text")) 
	work->msg = xmlToString( temp->children );
  }
  return work;
}

static MSG*  fetchXmlMessages( xmlNodePtr xmlMsgs )
{
  MSG	*top, *work, *curr;
  xmlNodePtr temp;

  top = work = curr = NULL;

  if( strncasecmp( "messages", xmlMsgs->name, strlen( xmlMsgs->name ) ) )
  {
    axlog( SYS_OLC, LVL_IMMORT, TRUE, "XML-leaf (%s) is not a messages-object!", xmlMsgs->name );
    return NULL;
  }

  for( temp = xmlMsgs->children; temp; temp = temp->next )
  {
    if( !strcasecmp( temp->name, "message" ) )
      work = fetchXmlMessage( temp );
    if( work )
    {
      if( !top )
	curr = top = work;
      else
      {
	curr->next = work;
	curr = work;
      }
    }
  }
  return top;
}

static MSG_BOARD* readBoardFromXml(char *file)
{
  int	      i;
  u_long  read_lvl, write_lvl, remove_lvl;
  MSG	      *msg;
  xmlNodePtr root,temp;
  char	    filename[255];
  xmlDocPtr  doc;
  MSG_BOARD  *bi;

  sprintf( filename, "%s/%s", BRD_PREFIX, file );
  if (!(doc = xmlParseFile(filename))) 
  {
    axlog( SYS_OLC, LVL_IMMORT, TRUE, "Couldn't parse XML-file %s", filename );
    return NULL;
  }
  if (!(root = doc->children)) 
  {
    axlog( SYS_OLC, LVL_IMMORT, TRUE, "No XML-root in board document %s", file );
    xmlFreeDoc(doc);
    return NULL;
  }
  if (strncasecmp("BOARD",root->name,strlen(root->name))) 
  {
    axlog( SYS_OLC, LVL_IMMORT, TRUE, "XML-root (%s in file %s) is not a board-object!", root->name, filename );
    xmlFreeDoc(doc);
    return NULL;
  }

  CREATE( bi, MSG_BOARD, 1 );
  read_lvl = write_lvl = remove_lvl = 0;
  strcpy( bi->file, file );
  strcpy( bi->key, file );
  
  for (temp = root->children; NULL != temp; temp = temp->next) 
  {
      if (!strcasecmp(temp->name,"vnum")) 
	bi->vnum =atoi( temp->children->content );
      else if (!strcasecmp(temp->name,"read")) 
	read_lvl =atoi( temp->children->content );
      else if (!strcasecmp(temp->name,"write")) 
	write_lvl =atoi( temp->children->content );
      else if (!strcasecmp(temp->name,"remove")) 
	remove_lvl =atoi( temp->children->content );
      else if (!strcasecmp(temp->name,"flags")) 
	bi->flags =atol( temp->children->content );
      else if (!strcasecmp(temp->name,"guild")) 
	bi->guild =atoi( temp->children->content );
      else if (!strcasecmp(temp->name,"key")) 
        strcpy( bi->key, temp->children->content );
      else if (!strcasecmp(temp->name,"title")) 
	bi->title = xmlToString( temp->children );
      else if (!strcasecmp(temp->name,"messages")) 
        bi->msgs = fetchXmlMessages( temp );
  }
  i=0;
  for( msg=bi->msgs; msg; msg=msg->next ) i++;
  bi->num_of_msgs = i;
  BRD_SET_READ( bi, read_lvl );
  BRD_SET_WRITE( bi, write_lvl );
  BRD_SET_REMOVE( bi, remove_lvl );
  xmlFreeDoc(doc);
  return bi;
}

/*
 * Ex:
 *
 * load_xml_board("./etc/board.ooc" );
 */
void load_xml_board(char *f)
{
  MSG_BOARD  *bi;

  bi = readBoardFromXml(f);
  if( bi )
  {
    bi->next = top_of_boards;
    top_of_boards = bi;
    if( (bi->rnum = real_object( bi->vnum )) == -1 ) 
    {
      sprintf(buf, "SYSERR: Fatal board error: board vnum %ld does not exist!", bi->vnum );
      mlog(buf);
      alog("EXITING!!!");
      exit(1);
    }
  }
}

void save_xml_board( MSG_BOARD *board )
{
  // Attempt to dump this room-file as xml as well...
  dumpBoardToXmlFile( board, board->file );

  return;
}

