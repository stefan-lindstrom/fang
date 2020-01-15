/******************************************************************************
 * $Id: config.c,v 1.5 2003/05/14 09:15:05 fang Exp $
 *   
 * File: config.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: config.c,v 
 *   Revision 1.2  2002/07/28 13:56:04  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.4  2002/05/27 10:52:48  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.3  2002/03/19 04:06:37  fang
 *   Moved the greeting into an actual file.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.20  2002/01/14 00:15:47  mud
 *   Score should no longer show negative amount of objs until you hit some 2*10^9 objs. :)
 *
 *   Revision 1.19  2001/10/28 13:19:49  mud
 *   *** empty log message ***
 *
 *   Revision 1.18  2001/09/30 23:07:05  mud
 *   *** empty log message ***
 *
 *   Revision 1.17  2001/09/30 20:45:03  mud
 *   *** empty log message ***
 *
 *   Revision 1.16  2001/09/08 15:03:45  mud
 *   *** empty log message ***
 *
 *   Revision 1.15  2001/09/03 11:12:30  mud
 *   Examine should work for threaded boards now
 *
 *   Revision 1.14  2001/08/20 11:02:50  mud
 *   *** empty log message ***
 *
 *   Revision 1.13  2001/04/23 04:27:48  mud
 *   *** empty log message ***
 *
 *   Revision 1.12  2001/03/24 03:38:56  mud
 *   *** empty log message ***
 *
 *   Revision 1.11  2001/03/24 03:25:45  mud
 *   *** empty log message ***
 *
 *   Revision 1.10  2001/03/17 10:55:55  mud
 *   *** empty log message ***
 *
 *   Revision 1.9  2001/03/17 10:40:46  mud
 *   New and Improved TAR.
 *
 *   Revision 1.8  2001/03/13 04:53:04  mud
 *   Fixed some typos
 *   !Save objects won't get saved (any !save containes will delete everything it contains)
 *
 *   Revision 1.7  2001/03/11 09:47:48  mud
 *   *** empty log message ***
 *
 *   Revision 1.6  2001/01/31 12:27:18  mud
 *   Changed the exp loss when dying to 0 for level <= 20 (newbies) and 1000
 *   for level > 20
 *
 *   Revision 1.5  2001/01/28 13:18:45  mud
 *   *** empty log message ***
 *
 *   Revision 1.4  2001/01/24 18:28:13  mud
 *   Added a way to get to the newbie zone (forgotten in the last commit)
 *
 *   Revision 1.3  2000/04/07 15:04:07  mud
 *
 *
 *   The new Grant System
 *
 *   Revision 1.2  2000/02/21 14:00:16  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* ************************************************************************
*   File: config.c                                      Part of CircleMUD *
*  Usage: Configuration of various aspects of CircleMUD operation         *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#define __CONFIG_C__

#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "config.h"

#ifndef TRUE
#define TRUE	1
#endif

#define YES	1

#ifndef FALSE
#define FALSE	0
#endif

#define NO	0

/*
 * Below are several constants which you can change to alter certain aspects
 * of the way CircleMUD acts.  Since this is a .c file, all you have to do
 * to change one of the constants (assuming you keep your object files around)
 * is change the constant in this file and type 'make'.  Make will recompile
 * this file and relink; you don't have to wait for the whole thing to
 * recompile as you do if you change a header file.
 *
 * I realize that it would be slightly more efficient to have lots of
 * #defines strewn about, so that, for example, the autowiz code isn't
 * compiled at all if you don't want to use autowiz.  However, the actual
 * code for the various options is quite small, as is the computational time
 * in checking the option you've selected at run-time, so I've decided the
 * convenience of having all your options in this one file outweighs the
 * efficency of doing it the other way.
 *
 */

/****************************************************************************/
/****************************************************************************/

/* GAME PLAY OPTIONS */

/*
 * pk_allowed sets the tone of the entire game.  If pk_allowed is set to
 * NO, then players will not be allowed to kill, summon, charm, or sleep
 * other players, as well as a variety of other "asshole player" protections.
 * However, if you decide you want to have an all-out knock-down drag-out
 * PK Mud, just set pk_allowed to YES - and anything goes.
 */
int pk_allowed = YES;

/* is playerthieving allowed? */
int pt_allowed = YES;

/* minimum level a player must be to shout/holler/gossip/auction */
int level_can_shout = 1;

/* number of movement points it costs to holler */
int holler_move_cost = 20;

/* exp change limits */
int max_exp_gain = 1000;	/* max gainable per kill */
int max_exp_loss = 1000;	/* max losable per death */

/* number of tics (usually 75 seconds) before PC/NPC corpses decompose */
int max_npc_corpse_time = 5;
int max_pc_corpse_time = 10;

/* should items in death traps automatically be junked? */
int dts_are_dumps = YES;

/* "okay" etc. */
char *OK = "Okay.\r\n";
char *NOPERSON = "No-one by that name here.\r\n";
char *NOEFFECT = "Nothing seems to happen.\r\n";

/****************************************************************************/
/****************************************************************************/


/* RENT/CRASHSAVE OPTIONS */

/*
 * Should the MUD allow you to 'rent' for free?  (i.e. if you just quit,
 * your objects are saved at no cost, as in Merc-type MUDs.)
 */
int free_rent = YES;

/* maximum number of items players are allowed to rent */
int max_obj_save = 256;

/* receptionist's surcharge on top of item costs */
int min_rent_cost = 100;

/*
 * Should the game automatically save people?  (i.e., save player data
 * every 4 kills (on average), and Crash-save as defined below.
 */
int auto_save = YES;

/*
 * if auto_save (above) is yes, how often (in minutes) should the MUD
 * Crash-save people's objects?   
 */
int autosave_time = 5;

/* Lifetime of crashfiles and forced-rent (idlesave) files in days */
int crash_file_timeout = 10;

/* Lifetime of normal rent files in days */
int rent_file_timeout = 30;


/****************************************************************************/
/****************************************************************************/


/* ROOM NUMBERS */

/* virtual number of room that mortals should enter at */
sh_int mortal_start_room = 10186;//1900;

/* virtual number of room that immorts should enter at by default */
sh_int immort_start_room =32601;

/* virtual number of room that frozen players should enter at */
sh_int frozen_start_room =  10188;

sh_int death_room = 10188;

sh_int recall_room = 10228;
sh_int newbie_room = 32500;

/*
 * virtual numbers of donation rooms.  note: you must change code in
 * do_drop of act.item.c if you change the number of non-NOWHERE
 * donation rooms.
 */
sh_int donation_room_1 = 10184;
sh_int donation_room_2 = NOWHERE;	/* unused - room for expansion */
sh_int donation_room_3 = NOWHERE;	/* unused - room for expansion */


/****************************************************************************/
/****************************************************************************/


/* GAME OPERATION OPTIONS */

/*
 * This is the default port the game should run on if no port is given on
 * the command-line.  NOTE WELL: If you're using the 'autorun' script, the
 * port number there will override this setting.  Change the PORT= line in
 * instead of (or in addition to) changing this.
 */
int DFLT_PORT = 5050;

/* default directory to use as data directory */
char *DFLT_DIR = "lib";

/* maximum number of players allowed before game starts to turn people away */
int MAX_PLAYERS = 300;

/* maximum size of bug, typo and idea files in bytes (to prevent bombing) */
int max_filesize = 50000;

/* maximum number of password attempts before disconnection */
int max_bad_pws = 3;

/*
 * Some nameservers are very slow and cause the game to lag terribly every 
 * time someone logs in.  The lag is caused by the gethostbyaddr() function
 * which is responsible for resolving numeric IP addresses to alphabetic names.
 * Sometimes, nameservers can be so slow that the incredible lag caused by
 * gethostbyaddr() isn't worth the luxury of having names instead of numbers
 * for players' sitenames.
 *
 * If your nameserver is fast, set the variable below to NO.  If your
 * nameserver is slow, of it you would simply prefer to have numbers
 * instead of names for some other reason, set the variable to YES.
 *
 * You can experiment with the setting of nameserver_is_slow on-line using
 * the SLOWNS command from within the MUD.
 */

int nameserver_is_slow = NO; /* YES; must test the DNS-caching! */

   /*
    * Many players appreciate typing only the beginning of the name of
    * things
    * (mobs, objects, players, descriptions, etc) on the MUD to reference
    * them.
    *
    * If you want this feature enabled on your MUD, set the variable
    * below to YES,
    * and your players will be able to type commands like "kill cre" in
    * the place
    * of "kill creppy". If you don't want this feature, leave this
    * variable set to
    * NO.
    *
    * BE AWARE that a lot of mistakes can be caused by setting this
    * to YES. Lazy
    * people typing only one or two letters of a mob name can,
    * e.g., cast a spell
    * on another player in the same room.
    */

int use_abbrevs = YES;

char *WELC_MESSG =
"\r\n"
"Welcome to the lands of Dragon's Fang!  May your visit here be... Interesting."
"\r\n\r\n";

char *START_MESSG =
"Welcome.  This is your new Dragon's Fang character!  You can now earn gold,\r\n"
"gain experience, find weapons and equipment, and much more -- while\r\n"
"meeting people from around the world!\r\n";

/****************************************************************************/
/****************************************************************************/


/* AUTOWIZ OPTIONS */

/* Should the game automatically create a new wizlist/immlist every time
   someone immorts, or is promoted to a higher (or lower) god level? */
int use_autowiz = YES;

/* If yes, what is the lowest level which should be on the wizlist?  (All
   immort levels below the level you specify will go on the immlist instead.) */
int min_wizlist_lev = LVL_GOD;

int use_regexp = YES;

