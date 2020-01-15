/******************************************************************************
 * $Id: comm.h,v 1.3 2003/06/17 10:30:05 fang Exp $
 *   
 * File: comm.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: comm.h,v $
 *   Revision 1.3  2003/06/17 10:30:05  fang
 *
 *   Removed a new set of "externs" from source files. For fucks sake people,
 *   let's keep those externs where they belong, in an apropriate header, ok?
 *
 *   Revision 1.2  2003/02/03 09:04:44  fang
 *   NPC-check in add_to_rplog
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.7  2002/01/24 21:33:14  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.6.2.2  2002/01/22 20:50:58  mud
 *
 *
 *   Removed boats and info system. Obsolete, unused code with no value.
 *   Fixed bug in portal_stone SPECIAL. Now checks that a room exists
 *   before attempting to transf chars there. :)
 *
 *   Revision 1.6.2.1  2002/01/22 03:05:11  mud
 *   More cleanup. Removed extern decls from source files. Protected some DG headers from multiple inclision
 *
 *   Revision 1.6  2001/10/02 13:17:06  mud
 *   *** empty log message ***
 *
 *   Revision 1.5  2001/09/19 05:52:58  mud
 *   Dynamic buffer and select-\>poll mod.
 *
 *   Revision 1.4  2001/03/17 10:40:46  mud
 *   New and Improved TAR.
 *
 *   Revision 1.3  2001/01/23 12:09:45  mud
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/02/21 14:00:16  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* ************************************************************************
*   File: comm.h                                        Part of CircleMUD *
*  Usage: header file: prototypes of public communication functions       *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#ifndef __COMM_H__
#define __COMM_H__

#include "sysdep.h"
#include "structs.h"

#define NUM_RESERVED_DESCS	8

/* comm.c */
void	send_to_all(char *messg);
void    asend_to_all(char *fmt,...);
void	send_to_char(const char *messg, struct char_data *ch);
void    asend_to_char(struct char_data *ch,char *fmt,...);
void	send_to_room(char *messg, int room);
void    asend_to_room(int room,char *fmt,...);
void    asend_to_outdoor(char *fmt,...);
void	send_to_outdoor(char *messg);
void	perform_to_all(char *messg, struct char_data *ch);
void    copy_emote_to_room(char *messg, int room,struct char_data *from);

void	close_socket(struct descriptor_data *d);

void	perform_act(char *orig, struct char_data *ch, struct obj_data *obj,
		    void *vict_obj, struct char_data *to, int to_dreaming);

void	act(char *str, int hide_invisible, struct char_data *ch,
struct obj_data *obj, void *vict_obj, int type);

#define TO_ROOM		1
#define TO_VICT		2
#define TO_NOTVICT	3
#define TO_CHAR		4
#define TO_NOTCHAR	5       /* to all but char! */
#define TO_TPUSER       6       /* To all users of The True Power, exclude vict*/
#define TO_OPUSERNSS    7       /* To user of the OP not same sax as CHAR! Excl vict*/
#define TO_OPUSERSS     8       /* To user of the OP same sax as CHAR! Excl vict*/
#define TO_NOTCHANN	9	/* to all non-channelers in room */
#define TO_VICTCSS	10	/* to vict only if channeler of same sex as ch*/
#define TO_VICTCNS	11	/* to vict only if channeler of opp. sex as ch*/
#define TO_VICTNC	12	/* Tovictr only if no channeler! */
#define TO_NOTTPUSER    13	/* To all in room not using the TP,exclude vict */
#define TO_VICTTP	14	/* To victim if TPuser	*/
#define TO_VICTNOTTP	15	/* To vict only if not TPUser! */
#define TO_NOTVICTSPAM  16	/* To all in but victim and char, exclude !SPAM people */
#define TO_SLEEP	128	/* to char, even if sleeping */
#define TO_DREAMERS     17

int	write_to_descriptor(struct descriptor_data *d, socket_t desc, char *txt);
void	write_to_q(const char *txt, struct txt_q *queue, int aliased);
void	write_to_output(const char *txt, struct descriptor_data *d);
void	page_string(struct descriptor_data *d, char *str, int keep_internal);

#define SEND_TO_Q(messg, desc)  write_to_output((messg), desc)

typedef RETSIGTYPE sigfunc(int);


/* Function prototypes for brain-dead OS's */

#if defined(__COMM_C__) && defined(__GNUC__)

#ifndef accept
   int accept();
#endif

#ifndef bind
   int bind();
#endif

#ifndef chdir
   int chdir();
#endif

#ifndef close
   int close();
#endif

/*#ifndef fcntl
   int fcntl();
#endif*/

#ifndef getpeername
   int getpeername();
#endif

#ifndef getrlimit
   int getrlimit();
#endif

#ifndef getsockname
   int getsockname();
#endif

/* Conflicting types, commented out/ Fredrik
#ifndef htonl
   u_long htonl();
#endif
*/

/*#ifndef htons
   u_short htons();
#endif*/

#ifndef listen
   int listen();
#endif

/* Conflicting types, commented out/ Fredrik
#ifndef ntohl
   u_long ntohl();
#endif
*/

/*#ifndef read
   int read();
#endif*/

#ifndef select
   int select();
#endif

#ifndef setitimer
   int setitimer();
#endif

#ifndef setrlimit
   int setrlimit();
#endif

#ifndef setsockopt
   int setsockopt();
#endif

#ifndef socket
   int socket();
#endif

/*#ifndef write
   int write();
#endif*/

#endif /* __COMM_C__ */

/*
 * Data
 */
extern struct descriptor_data *descriptor_list;
extern int circle_reboot;
extern int circle_shutdown;
extern int buf_largecount;          
extern int buf_overflows;   
extern int buf_switches;
extern int no_specials;
extern unsigned long dg_global_pulse;
extern int headpid;
extern int port_used;
extern int mother_desc;

/*
 * Funcs
 */
extern void echo_off(struct descriptor_data *d);
extern void echo_on(struct descriptor_data *d);
extern int new_descriptor(int s);
extern void flush_queues(struct descriptor_data *d);

#endif /* __COMM_H__ */
