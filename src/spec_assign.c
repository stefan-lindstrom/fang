/******************************************************************************
 * $Id: spec_assign.c,v 1.9 2004/12/16 04:11:46 fang Exp $
 *   
 * File: spec_assign.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: spec_assign.c,v $
 *   Revision 1.9  2004/12/16 04:11:46  fang
 *   Room 15297 no longer exists.
 *
 *   Revision 1.8  2002/11/07 04:02:04  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.7  2002/10/12 06:30:15  fang
 *   fisch: converted boards to xml and made them dynamic not coded.
 *
 *   Revision 1.6  2002/09/22 21:43:50  fischer
 *   Removing mounts and horsetrain from the code
 *
 *   Revision 1.5  2002/09/22 08:11:16  fang
 *   Put in a delay between backstab and flee, a halving of woven shields
 *   efficiency and a full !Magic protection of mobs.
 *
 *   Revision 1.4  2002/09/13 01:43:16  fang
 *   Drath: New boards added for Houses project
 *
 *   Revision 1.3  2002/08/30 21:23:09  fang
 *   Put in retainers.
 *
 *   Revision 1.2  2002/07/28 13:56:05  fang
 *   Removed dmlloc...just to test.
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.5  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.4  2002/03/17 03:17:43  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/02/28 22:03:15  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.33  2002/02/08 22:51:30  mud
 *   *** empty log message ***
 *
 *   Revision 1.32  2002/02/08 19:06:00  mud
 *   Added burnout board and allowed multi-player board
 *
 *   Revision 1.31  2001/09/28 13:59:27  mud
 *   *** empty log message ***
 *
 *   Revision 1.30  2001/09/04 11:23:55  mud
 *   *** empty log message ***
 *
 *   Revision 1.29  2001/09/04 10:04:44  mud
 *   *** empty log message ***
 *
 *   Revision 1.28  2001/09/04 08:36:22  mud
 *   Vanishing board bug squashed (I think ... :))
 *
 *   Revision 1.27  2001/09/03 16:35:01  mud
 *   *** empty log message ***
 *
 *   Revision 1.26  2001/07/09 08:47:30  mud
 *   *** empty log message ***
 *
 *   Revision 1.25  2001/05/22 09:07:07  mud
 *   DG-Scripts for Fang.
 *
 *   Revision 1.24  2001/03/22 16:50:28  mud
 *   *** empty log message ***
 *
 *   Revision 1.23  2001/03/17 10:55:55  mud
 *   *** empty log message ***
 *
 *   Revision 1.22  2001/03/17 10:40:47  mud
 *   New and Improved TAR.
 *
 *   Revision 1.21  2001/03/13 14:04:28  mud
 *   *** empty log message ***
 *
 *   Revision 1.20  2001/03/07 08:29:54  mud
 *   *** empty log message ***
 *
 *   Revision 1.19  2001/01/28 13:18:45  mud
 *   *** empty log message ***
 *
 *   Revision 1.18  2000/12/25 09:09:06  mud
 *   *** empty log message ***
 *
 *   Revision 1.17  2000/12/13 23:05:55  mud
 *   Did some board clean up, since someone made one and never told me about
 *   it.
 *
 *   Revision 1.16  2000/12/13 22:07:43  mud
 *   Added a board object for the Sea Folk.
 *
 *   Revision 1.15  2000/10/21 22:13:13  mud
 *   *** empty log message ***
 *
 *   Revision 1.14  2000/10/15 22:29:14  mud
 *   *** empty log message ***
 *
 *   Revision 1.13  2000/10/04 18:18:30  mud
 *   Added a board
 *
 *   Revision 1.12  2000/10/04 15:57:41  mud
 *   Added gen_board function for Newbiehelper board
 *
 *   Revision 1.11  2000/09/09 18:59:52  mud
 *   Made the idea board a threadboard, to test reactions to it.
 *
 *   Revision 1.10  2000/07/24 06:15:53  mud
 *   Added global board!
 *
 *   Revision 1.9  2000/06/21 21:15:34  mud
 *   Added DL board for Shai.
 *
 *   Revision 1.8  2000/04/30 11:40:49  mud
 *   *** empty log message ***
 *
 *   Revision 1.7  2000/04/16 04:00:15  mud
 *   Removed two lines from the assignobj list -- boards 17001 and 16161 were
 *   listed twice.  Illy and I thought this might be the cause of the
 *   degenerate board syserr.
 *
 *   Revision 1.6  2000/03/16 23:59:48  mud
 *   Added WK board
 *
 *   Revision 1.5  2000/03/16 23:24:00  mud
 *   Added CotL guild board back in.
 *
 *   Revision 1.4  2000/03/02 12:54:32  mud
 *   Changes to get rid of 'non-existing' messages in special assigns
 *
 *   Revision 1.3  2000/03/01 15:15:46  mud
 *   Named specials added, saved in room XML, and setable in redit
 *
 *   Revision 1.2  2000/02/21 14:00:19  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* ************************************************************************
*   File: spec_assign.c                                 Part of CircleMUD *
*  Usage: Functions to assign function pointers to objs/mobs/rooms        *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "conf.h"
#include "sysdep.h"
#include "spec_procs.h"
#include "structs.h"
#include "db.h"
#include "interpreter.h"
#include "utils.h"
#include "constants.h"
#include "threadboard.h"
#include "boards.h"
#include "retainer.h"
#include "boards.h"

/* functions to perform assignments */

void ASSIGNMOB(int mob, SPECIAL(fname))
{
  // NULL check so that named specials isn't 'over written' by assign. 
  if (real_mobile(mob) >= 0) {
    if (mob_index[real_mobile(mob)].func == NULL) {
      mob_index[real_mobile(mob)].func = fname;
    }
  }
  else if (!mini_mud) {
    sprintf(buf, "SYSERR: Attempt to assign spec to non-existant mob #%d",
        mob);
    mlog(buf);
  }
}

void ASSIGNOBJ(int obj, SPECIAL(fname))
{
  if (real_object(obj) >= 0) 
  {
    if (fname == gen_board) 
    {
      MSG_BOARD	*brd;

      for( brd=top_of_boards; brd != NULL; brd=brd->next )
      {
	if( brd->vnum == obj) // I don't think rnums have been set up properly at this point!
	{ 
	  obj_index[real_object(obj)].func = fname;
	  return;
	}
      }

      alog("SYSERR: Erroneous attempt to assign gen_board on wrong board #%d", obj);
      return;
    } // fname == gen_board

    if (obj_index[real_object(obj)].func == NULL)
      obj_index[real_object(obj)].func = fname;
  }
  else if (!mini_mud) {
    sprintf(buf, "SYSERR: Attempt to assign spec to non-existant obj #%d",
        obj);
    mlog(buf);
  }
}

void ASSIGNROOM(int room, SPECIAL(fname))
{
  if (real_room(room) >= 0) {
    if (world[real_room(room)].func == NULL) {
      world[real_room(room)].func = fname;
    }
  }
  else if (!mini_mud) {
    sprintf(buf, "SYSERR: Attempt to assign spec to non-existant rm. #%d",
        room);
    mlog(buf);
  }
}


/* ********************************************************************
*  Assignments                                                        *
******************************************************************** */

/* assign special procedures to mobiles */
void assign_mobiles(void)
{
  ASSIGNMOB(1, puff);
  /* CAEMLYN  */
  ASSIGNMOB(33, taunt);
/*  ASSIGNMOB(390, postmaster); */
  ASSIGNMOB(1003, postmaster);
  ASSIGNMOB(1300, guild);
  ASSIGNMOB(1301, trainer);
  ASSIGNMOB(1302, postmaster);
  ASSIGNMOB(1315, bank);
  ASSIGNMOB(1720, postmaster);
  ASSIGNMOB(1949, newbiehealer);
  ASSIGNMOB(1952, postmaster);
  ASSIGNMOB(1960, trainer);
  ASSIGNMOB(3593, bank);
  ASSIGNMOB(4099, postmaster);
  ASSIGNMOB(5002, laras);
  ASSIGNMOB(6000, postmaster);
  ASSIGNMOB(6700, trainer);
  ASSIGNMOB(6701, guild);
  ASSIGNMOB(7027, guild);
  ASSIGNMOB(7028, trainer);
  ASSIGNMOB(7400, postmaster);
  ASSIGNMOB(7401, guild);
  ASSIGNMOB(7402, trainer);
  ASSIGNMOB(7403, bank);
  ASSIGNMOB(8803, postmaster);
  ASSIGNMOB(8804, guild);
  ASSIGNMOB(8805, trainer);
  ASSIGNMOB(10001, postmaster);
  ASSIGNMOB(10019, guild);
  ASSIGNMOB(10029, trainer);
  ASSIGNMOB(10044, newbiehealer);
  ASSIGNMOB(13127, guild);	/* Henre Haslin */
  ASSIGNMOB(13126, trainer);    /* MAzrims stat-raiser. :) */
  ASSIGNMOB(13125, postmaster);
  ASSIGNMOB(14100, postmaster);
  ASSIGNMOB(14404, postmaster);//Eraana's postmaster
  ASSIGNMOB(14490, bank);
  ASSIGNMOB(15206, guild);
  ASSIGNMOB(15207, trainer);
  ASSIGNMOB(15208, postmaster);
  ASSIGNMOB(15377, trainer);
  ASSIGNMOB(15378, guild);
  ASSIGNMOB(16000, postmaster);
  ASSIGNMOB(17016, guild);
  ASSIGNMOB(17017, trainer);
  ASSIGNMOB(17079, postmaster);
  ASSIGNMOB(17080, bank);
/*  ASSIGNMOB(17401, postmaster); */
  ASSIGNMOB(25004, bank);
  ASSIGNMOB(25005, postmaster);
  ASSIGNMOB(29507, postmaster);
  ASSIGNMOB(29996, postmaster);
  ASSIGNMOB(30113, bank);
  ASSIGNMOB(32615, postmaster);
  ASSIGNMOB(6718, postmaster);  /* Shai'tan's postman */
  ASSIGNMOB(32617, postmaster);
  ASSIGNMOB(18131, postmaster);  /*Valtara's fat man postperson */
}



/* assign special procedures to objects */
void assign_objects(void)
{
  MSG_BOARD *brd;

  for( brd=top_of_boards; brd; brd=brd->next )
    ASSIGNOBJ( brd->vnum, gen_board );

  ASSIGNOBJ(83, strong_dream_obj);
  ASSIGNOBJ(84, weak_dream_obj);
  ASSIGNOBJ(28023, herb);
  ASSIGNOBJ(28024, herb);
  ASSIGNOBJ(15200, pigeon);     /* Tower Warders pigeons */
  ASSIGNOBJ(6700 , pigeon);      /* DF Pigeon */
  ASSIGNOBJ(3607 , warder_cloak); /* Rebel Warder Cloak */
  ASSIGNOBJ(3520 , pigeon);     /* A piece of parchment */
  ASSIGNOBJ(1007, warder_cloak); /* Tower Warder Cloak */
  ASSIGNOBJ(39,dream_gate);       /* T'A'R Gate */
  ASSIGNOBJ(40,sp_gate);          /* A Shimmering Gateway */
}



/* assign special procedures to rooms  */
void assign_rooms(void)
{
  ASSIGNROOM(3292, portal_stones);
/*  ASSIGNROOM(4928, portal_stones); */
  ASSIGNROOM(6791, dreamroom);   /* Df meeting place in T'a'r */
  ASSIGNROOM(8939, portal_stones);
/*  ASSIGNROOM(9411, portal_stones); */
/*  ASSIGNROOM(9472, skillshop); */   /* Questshop in Seandar 	*/
  ASSIGNROOM(10001, repair_shop);
  ASSIGNROOM(30696, retainer_shop);
  ASSIGNROOM(10390, skillshop);  /* Quest shop for skills/items in Caemlyn */
  ASSIGNROOM(14076, bank);
  ASSIGNROOM(14141, bank);
  ASSIGNROOM(14047, hospital);
/*  ASSIGNROOM(15297, hospital); */
  ASSIGNROOM(15585, bank);
  ASSIGNROOM(15718, bank);
  ASSIGNROOM(17320, portal_stones);
  ASSIGNROOM(23335, portal_stones);
  ASSIGNROOM(24048, hospital);
  ASSIGNROOM(29942, hospital);
/*  ASSIGNROOM(31212, portal_stones); */
  ASSIGNROOM(32500, button_room); /* The button in the mortal start_room! */
  ASSIGNROOM(32699,portal_stones);
  ASSIGNROOM(32601,portal_stones);
  ASSIGNROOM(7031, portal_stones);
  ASSIGNROOM(28093, skillshop);
}
