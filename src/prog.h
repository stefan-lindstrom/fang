/******************************************************************************
 * $Id: prog.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: prog.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: prog.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/02/21 14:00:18  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/*************************************************************************
*   File: prog.h                                    Part of Dragons Fang *
*  Usage: header file for all kinds of programs                          *
************************************************************************ */


#ifndef __PROG_H__
#define __PROG_H__

#include <stdio.h>
#include <stddef.h>


struct side_term_list
{
  int                    number;
  int                    not;
  int                    int_arg;
  char                  *argument;
  struct side_term_list *next;
} typedef side_term_list;

struct obj_action_list {
  int action, intarg;
  char *argument;
  struct obj_action_list *next;
};

struct obj_prog_list {
  int main, intarg;
  char *argument;
  struct side_term_list *sideterms;
  struct obj_action_list *actions;
  struct obj_prog_list *next;
};


struct room_action_list  
{
  int                      action;
  int                      intarg;
  char                    *stringarg;
  struct room_action_list *next;
} typedef room_action_list;

struct room_prog_list
{
  int                    main;
  int                    intarg;
  char                  *stringarg;
  side_term_list        *sideterms;
  room_action_list      *actions;  
  struct room_prog_list *next;     
} typedef room_prog_list;


struct mob_action_list {
  int action, intarg;
  char *argument;
  struct mob_action_list *next;
};
   
struct mob_prog_list {
  int main, intarg;
  char *argument;
  struct side_term_list *sideterms;
  struct mob_action_list *actions;
  struct mob_prog_list *next;
}; 

#endif
