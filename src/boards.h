/******************************************************************************
 * $Id: boards.h,v 1.13 2003/06/17 10:30:05 fang Exp $
 *   
 * File: boards.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: boards.h,v $
 *   Revision 1.13  2003/06/17 10:30:05  fang
 *
 *   Removed a new set of "externs" from source files. For fucks sake people,
 *   let's keep those externs where they belong, in an apropriate header, ok?
 *
 *   Revision 1.12  2002/10/29 01:34:17  fang
 *   Fischer: release reload to other functions when performed in a room with
 *   a board in it.
 *
 *   Revision 1.11  2002/10/23 18:29:58  fang
 *   fisch: activated the "GLOBAL" flag to mark boards global readable or not
 *
 *   Revision 1.10  2002/10/23 04:17:19  fang
 *   fisch: "list board" to list boards with new messages added
 *
 *   Revision 1.9  2002/10/19 04:32:13  fang
 *   fisch: added fields saved for flags, keywords and guilds
 *
 *   Revision 1.8  2002/10/15 21:37:24  fang
 *   fisch: added "reload board" command
 *
 *   Revision 1.7  2002/10/14 22:23:03  fang
 *   fisch: added "edit by" tag in gawd awful red and white colors
 *
 *   Revision 1.6  2002/10/14 03:15:50  fang
 *   fisch: added "edit" for the message board messages
 *
 *   Revision 1.5  2002/10/13 15:35:46  fang
 *   fischer: removed three old entertainer boards
 *
 *   Revision 1.4  2002/10/12 06:30:15  fang
 *   fisch: converted boards to xml and made them dynamic not coded.
 *
 *   Revision 1.3  2002/09/13 01:43:16  fang
 *   Drath: New boards added for Houses project
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.4  2002/03/17 03:17:42  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/02/28 22:03:15  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.38  2002/02/08 19:05:01  mud
 *   Adding 2 boards
 *
 *   Revision 1.37  2001/09/04 11:23:55  mud
 *   *** empty log message ***
 *
 *   Revision 1.36  2001/09/03 16:13:49  mud
 *   *** empty log message ***
 *
 *   Revision 1.35  2001/07/09 08:47:30  mud
 *   *** empty log message ***
 *
 *   Revision 1.34  2001/03/13 04:53:04  mud
 *   Fixed some typos
 *   !Save objects won't get saved (any !save containes will delete everything it contains)
 *
 *   Revision 1.33  2001/01/28 13:18:45  mud
 *   *** empty log message ***
 *
 *   Revision 1.32  2001/01/27 08:16:35  mud
 *   *** empty log message ***
 *
 *   Revision 1.31  2000/12/25 09:09:06  mud
 *   *** empty log message ***
 *
 *   Revision 1.30  2000/12/13 23:05:55  mud
 *   Did some board clean up, since someone made one and never told me about
 *   it.
 *
 *   Revision 1.29  2000/12/13 22:07:43  mud
 *   Added a board object for the Sea Folk.
 *
 *   Revision 1.28  2000/12/11 16:21:13  mud
 *   Some minor bugfixes, and some parts of qstrip done
 *
 *   Revision 1.27  2000/12/11 12:14:18  mud
 *   Removed most references to PFILEPOS() get_char_ppos(), since with the xml code ppos will(may) change every reboot
 *
 *   Revision 1.26  2000/12/10 11:01:58  mud
 *   *** empty log message ***
 *
 *   Revision 1.25  2000/12/10 09:32:13  mud
 *   Fixed train overflow when raising mana/move/hp
 *
 *   Revision 1.24  2000/12/03 10:26:56  mud
 *   *** empty log message ***
 *
 *   Revision 1.23  2000/12/03 10:26:10  mud
 *   More QEQ tracking, begun some minro mods to have exdescs editable by craft/tool.
 *
 *   Revision 1.22  2000/11/17 22:39:09  mud
 *   fixed guildinv
 *
 *   Revision 1.21  2000/11/16 15:43:42  mud
 *   *** empty log message ***
 *
 *   Revision 1.20  2000/11/16 15:43:04  mud
 *   *** empty log message ***
 *
 *   Revision 1.19  2000/10/15 22:29:14  mud
 *   *** empty log message ***
 *
 *   Revision 1.18  2000/10/06 03:00:42  mud
 *   *** empty log message ***
 *
 *   Revision 1.17  2000/10/04 18:18:30  mud
 *   Added a board
 *
 *   Revision 1.16  2000/08/17 15:26:49  mud
 *   *** empty log message ***
 *
 *   Revision 1.15  2000/07/24 06:15:52  mud
 *   Added global board!
 *
 *   Revision 1.14  2000/06/21 21:15:34  mud
 *   Added DL board for Shai.
 *
 *   Revision 1.13  2000/05/13 13:57:57  mud
 *   No name flag for boards, where players names won't show up on boards
 *   with the ITEM_NONAMEBOARD flag.
 *
 *   Revision 1.12  2000/04/29 21:48:25  mud
 *   Hunters board.
 *
 *   Revision 1.11  2000/04/15 03:43:16  mud
 *   Added df list back in for the cotl.
 *
 *   Revision 1.10  2000/03/31 23:36:57  mud
 *   Added ice board back into the game.
 *
 *   Revision 1.9  2000/03/23 03:06:13  mud
 *   Updated NUM_OF_BOARDS to reflect correct number of boards in boards.c
 *
 *   Revision 1.8  2000/03/23 02:41:14  mud
 *   Upped the max number of boards
 *
 *   Revision 1.7  2000/03/17 09:10:55  mud
 *   Put in the Ogier board...obj 24000...
 *
 *   Revision 1.6  2000/03/16 23:59:48  mud
 *   Added WK board
 *
 *   Revision 1.5  2000/03/16 23:47:27  mud
 *   Added 2 aiel, 1 tw board
 *
 *   Revision 1.4  2000/03/16 23:23:59  mud
 *   Added CotL guild board back in.
 *
 *   Revision 1.3  2000/03/13 19:38:00  mud
 *   Changed the list of boards as specified in Edin's mail to fang_coders.
 *
 *   Revision 1.2  2000/02/21 14:00:16  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/


/* ************************************************************************
*   File: boards.h                                      Part of CircleMUD *
*  Usage: header file for bulletin boards                                 *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#ifndef __BOARDS_H__
#define __BOARDS_H__
/* Note: If you add or remove a board, make sure that NUM_OF_BOARDS 
 * acutally reflect the real number of boards. That is also true when
 * you just comment out a line in boards.c..This number -must- equal
 * the number of boards defined.
 * /Fredrik 990313
 */
#define old_NUM_OF_BOARDS           57     /* change if needed! */
#define MAX_BOARD_MESSAGES 	120      /* arbitrary -- change if needed */
#define MAX_MESSAGE_LENGTH	4096	/* arbitrary -- change if needed */

#define INDEX_SIZE	   ((old_NUM_OF_BOARDS*MAX_BOARD_MESSAGES) + 5)

#define BRD_FLAG_GLOCK	    (1<<0)    /*guild locked*/
#define BRD_FLAG_NONAME	    (1<<1)    /*names not printed*/
#define BRD_FLAG_GLOBAL	    (1<<2)    /*not available for global read*/

#define SET_BRD_FLAG(b,f)   ((b)->flags = ((b)->flags | (f)))
#define CLEAR_BRD_FLAG(b,f) ((b)->flags = ((b)->flags & (~(f))))
#define CHK_BRD_FLAG(b,f)   ((b)->flags & (f))

#define BRD_READ(b)   (((b)->levels)&255)
#define BRD_WRITE(b)   ((((b)->levels>>8))&255)
#define BRD_REMOVE(b)   ((((b)->levels>>16))&255)

#define BRD_SET_READ(b, l)  ((b)->levels=(((b)->levels)&0xFFFFFF00)+l)
#define BRD_SET_WRITE(b, l)  ((b)->levels=(((b)->levels)&0xFFFF00FF)+(((u_long)l)<<8))
#define BRD_SET_REMOVE(b, l)  ((b)->levels=(((b)->levels)&0xFF00FFFF)+(((u_long)l)<<16))

#define BOARD_MAGIC	1048575	/* arbitrary number - see modify.c */

typedef struct tagMSG_BOARD MSG_BOARD;
typedef struct tagMSG MSG;

struct tagMSG_BOARD
{
  long	      vnum;
  long	      rnum;
  u_long      levels;
  long	      flags;
  int	      guild;
  time_t      stamp;
  char	      key[50];
  char	      file[50];
  byte	      num_of_msgs;
  char	      *title;
  MSG	      *msgs;
  MSG_BOARD   *next;
};

struct tagMSG
{
  byte slot_num;
  long	  level;
  long	  author;
  time_t  date;
  char	  *heading;   //can be removed when no more <heading> tags in board files
  char	  *header;    //replacing <heading> tags with <header> tags
  char	  *author_name;
  char	  *edit_name;
  char	  *msg;
  MSG	  *next;
};

int	Board_display_msg(MSG_BOARD *brd, struct char_data *ch, char *arg, struct obj_data *board);
int	Board_show_board(MSG_BOARD *brd, struct char_data *ch, char *arg, struct obj_data *board);
int	Board_remove_msg(MSG_BOARD *brd, struct char_data *ch, char *arg);
int	Board_reload_board( MSG_BOARD *brd, struct char_data *ch, char *arg );
void	Board_write_message(MSG_BOARD *brd, struct char_data *ch, char *arg, struct obj_data *board);
void	Board_edit_message(MSG_BOARD *brd, struct char_data *ch, char *arg, struct obj_data *board);
void	Board_save_board(MSG_BOARD *brd);
void	Board_load_board( int i );

MSG_BOARD* find_board_by_name( char *key );

extern void load_xml_board( char *f );
extern void save_xml_board( MSG_BOARD *board );
extern void init_boards( void );
extern void  do_board_set( struct char_data *ch, char *arg );

/*
 * Data
 */

extern MSG_BOARD  *top_of_boards;
#endif
