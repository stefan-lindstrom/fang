/******************************************************************************
 * $Id: guild.h,v 1.3 2003/01/22 09:03:17 fang Exp $
 *   
 * File: guild.h
 *
 * Created: Sun May 14 2000 by Yohay Etsion (yohay_e@netvision.net.il)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: guild.h,v $
 *   Revision 1.3  2003/01/22 09:03:17  fang
 *   Added defs for OOC/IC clubs. OOC Clubs are defined as Clubs (15), old def kept for backwards compatibility.
 *
 *   Revision 1.2  2003/01/21 20:33:42  fang
 *   Added guild type Subguild, as well as OOC/IC clubs. New constant RMAX_GUILD_TYPES.
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.4  2002/05/27 10:52:49  fang
 *   Dmalloc enabled.
 *
 *   Revision 1.3  2002/05/12 23:50:46  fang
 *   Added in IGNORE system, and removed AutoHotboots until the cap can be fixed for good. -Maz
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.12  2002/01/28 08:52:57  mud
 *   Changed delete so that is a character has qps (raw, skills or eq)
 *   the normal delete proicedure will not work, and a warning is printed.
 *   Instead of "delete yes delete me" (qhich still works for no-qp chars)
 *   the qp-chars will have to type "delete yes delete me and my qps!".
 *
 *   Revision 1.11  2002/01/24 21:33:17  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.10.2.2  2002/01/23 21:49:41  mud
 *   Removed objsave.[c|h] (old objsave stuff).
 *   Removed obj_file_elem and rent structs.
 *   Removed final vestiges of old guild system, except parts used
 *   in guild.[c|h] for backwards compatibility.
 *
 *   Revision 1.10.2.1  2002/01/22 20:50:58  mud
 *
 *
 *   Removed boats and info system. Obsolete, unused code with no value.
 *   Fixed bug in portal_stone SPECIAL. Now checks that a room exists
 *   before attempting to transf chars there. :)
 *
 *   Revision 1.10  2002/01/15 19:17:26  mud
 *   Warning free...let's keep it that way...
 *
 *   Revision 1.5  2000/10/19 20:24:50  mud
 *   modified 'set name' to keep guild info
 *
 *   Revision 1.4  2000/09/09 10:50:15  mud
 *   fixed do_ajahset to work with new guild system
 *
 *   Revision 1.3  2000/09/04 03:34:39  mud
 *   Warnings removed...
 *
 *   Revision 1.2  2000/09/04 02:39:02  mud
 *   PLEASE! ADD FRIKKIN' FUNC. DECS YOU'RE PLANNING ON EXPORTING 
 *   AND USING IN OTHER SOURCE FILE IN THE FRIKKIN' HEADER!!
 *
 *   Revision 1.1  2000/08/14 19:19:06  mud
 *   New guild system files.
 *
 *
 *****************************************************************************/

#ifndef __GUILD_H__
#define __GUILD_H__

#include "structs.h"

/* Guild Types */

#define GUILD_UNDEFINED    0
#define GUILD_ORGANIZATION 1
#define GUILD_GROUP        2
#define GUILD_HOMELAND     3
#define GUILD_DARK         4
#define GUILD_MISC         5
#define GUILD_SUBGUILD     6

#define MAX_GUILD_TYPES    6
#define GUILD_IC_CLUB     14
#define GUILD_CLUB        15 /* keep this for backwards compatibility */
#define GUILD_OOC_CLUB    GUILD_CLUB

#define RMAX_GUILD_TYPES  15

/**
 * Some crap left for support of old guilds/guild code
 */
#define GLD_OGIER      13 
#define GLD_WOLFKIN    20
#define GLD_DARKFRIEND 21
#define MAXGUILDS      22

/* Guild flags */

#define GLD_EXCLUSIVE	    (1 << 0) /* A person can join only 1 of those */
#define GLD_IC		    (1 << 1) /* An IC guild, not OOC */
#define GLD_GLTITLE         (1 << 2) /* The GL has an IMM title */
#define GLD_GUILDIESTITLE   (1 << 3) /* Guildies have an IMM title (Ogier) */
#define GLD_GOSSIP          (1 << 4) /* Guild has a gossip file */
#define GLD_NOGCHAN         (1 << 5) /* Guild has no guild channel */
#define GLD_SECRETIVE       (1 << 6) /* Secretive guild */
#define GLD_NOGLIST         (1 << 7) /* Guildies can't use glist */
#define GLD_NOGWHO          (1 << 8) /* Guildies can't use gwho */
#define GLD_NOGUILDWALK     (1 << 9) /* No guildwalk for this guild */
#define GLD_NOBANK          (1 << 10) /* Guild has no bank account */
#define GLD_DARK	    (1 << 11) /* Dark guild */
#define GLD_DISGUISE_STAY   (1 << 12) /* Guild carries through disguise */

/* Guildie's status flags */

#define STATUS_SEEKING	   (1 << 0) /* Seeker */
#define STATUS_AUTHORIZED  (1 << 1) /* BG approved */
#define STATUS_MEMBER      (1 << 2) /* Guilded */
#define STATUS_GL          (1 << 3) /* Guildleader */
#define STATUS_CHAN_ON     (1 << 4) /* Guildie has gchan on */
#define STATUS_SUBRANK     (1 << 5) /* Guildie has a subrank */
#define STATUS_WARDER      (1 << 6) /* For old system support */
#define STATUS_GINVIS	   (1 << 7) /* Guild invis flag */
#define STATUS_TAG	   (1 << 8) /* Guildie sees a tag in the gchannel */
#define STATUS_SPONSORLIST (1 << 9) /* Guildie is listed on sponsorlist */

/* Guildie's perm flags */

#define PERM_GUILD	    (1 << 0) /* Guildie can guild others */
#define PERM_WITHDRAW	    (1 << 1) /* Guildie can withdraw gold from the guild */
#define PERM_GOSSIP_WRITE   (1 << 2) /* Guildie can add to the guild's gossip */
#define PERM_SPONSOR        (1 << 3) /* Guildie can sponsor seekers */
#define PERM_AUTHORIZE	    (1 << 4) /* Guildie can approve seekers' bgs */
#define PERM_GSKILLSET	    (1 << 5) /* Guildie can gskillset */

/* MACROs */

#define GLD_FLAGGED(gld, flag)		(IS_SET(gld->gflags, flag))
#define STATUS_FLAGGED(guildie, flag)   (IS_SET(guildie->status, flag))
#define PERM_FLAGGED(guildie, flag)	(IS_SET(guildie->perm, flag))
#define GLD_TYPE(gld)                   (gld->type)
#define GET_CHAR_GUILDS(ch)		((ch)->char_specials.guilds_data)

/* Functions */
extern int is_dark(struct char_data *ch);
extern int get_guild(struct char_data *ch, int num);
extern int can_use_gskill(struct char_data *ch, int skill);
extern void do_guild_channel(struct char_data *ch, struct char_guild_element *element, char *argument);
extern void do_guild_gossip(struct char_data *ch, struct char_guild_element *element, char *argument);
extern void fetch_char_guild_data(struct char_data *ch);
extern void remove_char_guild_data(struct char_data *ch);
extern int get_guild_by_room(struct char_data *ch, int room);
extern void guildie_rename(struct char_data *ch, char *newname, int offline);
extern void do_guild_walk(struct char_data *ch, struct char_guild_element *element);
extern int share_guild(struct char_data *, struct char_data *);
extern void delete_char_guilds(struct char_data *ch);
extern int is_ignoring(struct char_data *ch, struct char_data *vict);
extern int get_guild_by_name(struct char_data *ch, char *name);

#endif
