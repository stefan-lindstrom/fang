/******************************************************************************
 * $Id: alias.c,v 1.5 2003/05/06 08:26:42 fang Exp $
 *   
 * File: alias.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: alias.c,v 
 *   Revision 1.2  2002/07/28 13:56:03  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.3  2002/05/27 10:52:48  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2000/04/24 03:56:16  mud
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/02/21 14:00:16  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* ************************************************************************
*   File: alias.c				   A utility to CircleMUD *
*  Usage: writing/reading player's aliases                                *
*                                                                         *
*  Code done by Jeremy Hess and Chad Thompson				  *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "utils.h"
#include "interpreter.h"
#include "comm.h"
#include "alias.h"

void write_aliases(struct char_data *ch)
{
  FILE *file;
  char fn[127],*buf,fn2[200],tmp[500];
  struct alias *temp;
  int length;

  get_filename(GET_RNAME(ch),fn,ALIAS_FILE);

  unlink(fn);
  if( !GET_ALIASES(ch) )
    return;
  strcpy(fn2,fn);
  strcat(fn2,".tmp");

  file = fopen(fn2,"wt");

  if (NULL == file) {
    sprintf(tmp,"SYSERR: Error writing %s's aliases!\n",GET_RNAME(ch));
    sprintf(tmp,"%s\tReason given: %s\n",tmp,strerror(errno));
    mlog(tmp);
    send_to_char("Error saving aliases. Please try again later, or inform an Imm\r\n",ch);
    return;
  }

  temp = GET_ALIASES(ch);
  
  while( temp )
  {
    length = strlen(temp->alias);
    fprintf(file,"%d\n",length); 
    fprintf(file,"%s\n",temp->alias);
    buf = strdup(temp->replacement); 
    while( *++buf == ' ' );
    length = strlen(buf);
    fprintf(file,"%d\n",length); 
    fprintf(file,"%s\n",buf);
    fprintf(file,"%ld\n",temp->type);
    temp = temp->next;
  }
  
  fclose(file);
  remove(fn);
  rename(fn2,fn);
}

void read_aliases(struct char_data *ch)
{   
  FILE *file;
  char fn[255];
  struct alias *t2;
  int length;
  char temp_buf[2*4096],buf[2*4096];

  get_filename(GET_RNAME(ch),fn,ALIAS_FILE);
  file = fopen(fn,"r");

  if( !file ) {
    return;
  }
 
  CREATE(GET_ALIASES(ch),struct alias,1);
  t2 = GET_ALIASES(ch); 
  do 
  { 
    fscanf(file,"%d\n",&length);
    fgets(buf,length+1,file);
    t2->alias=strdup(buf);
    fscanf(file,"%d\n",&length);
    fgets(buf,length+1,file);
    strcpy(temp_buf," ");
    strcat(temp_buf,buf); 
    t2->replacement=strdup(temp_buf); 
    fscanf(file,"%d\n",&length);
    t2->type = length; 
    if( !feof(file) ){
      CREATE(t2->next,struct alias,1);
      t2 = t2->next;
    } 
  } while( !feof(file) ); 
  
  fclose(file);
} 

