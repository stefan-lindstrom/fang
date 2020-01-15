/******************************************************************************
 * $Id: guild_parser.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: guild_parser.h
 *
 * Created: Sat May 13 2000 by Yohay Etsion (yohay_e@netvision.net.il)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: guild_parser.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.3  2002/06/13 10:12:28  fang
 *   More free'ing stuff...
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2001/04/21 02:56:06  mud
 *   QPadd and GequipSend, plus greceive
 *
 *   Revision 1.1  2000/08/14 19:19:06  mud
 *   New guild system files.
 *
 *
 *****************************************************************************/

#ifndef __GUILDPARSER_H__
#define __GUILDPARSER_H__

#include <stddef.h>
#include <errno.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/entities.h>
#include <libxml/parserInternals.h>
#include <stdio.h>

#include "structs.h"
#include "interpreter.h"

/* Struct defines */

struct rank_info  {
	char *name;
	long num;
        struct rank_info *next;
};

struct sponsorer_info {
	long idnum;
	char *name;
     	struct sponsorer_info *next;
};

struct qp_info {
	char *name; /* Name of person awarding */
	long num;   /* Number of QPs awarded */
	struct qp_info *next;
};

struct guildie_info {
	char *name;
	long idnum;
	long rank_num;
        struct rank_info *rank;
	char *subrank;
	long perm;
	long status;
        long deposited;
        long withdrew;
	struct char_data *ch;
     	struct sponsorer_info *sponsorers;
        struct qp_info *qpadd;
	struct gequip_info *gequipsent;
	struct guildie_info *next;
};

struct gskill_info {
	int skill;
	int maximum_set;
	struct gskill_info *next;
};

struct gequip_info {
	int vnum;
	struct gequip_info *next;
};

struct gzone_info {
	int zone;
	struct gzone_info *next;
};

struct ghelp_info {
  char *keyword;
  char *entry;
  struct ghelp_info *next;
 };

struct guild_info {
  int id;
  int type;
  char *name;
  char *description;
  char *requirements;
  char *gossip;
  char *gl_title;
  char *guildie_titles;
  char *guild_filename;
  char *gossip_name;
  char *gchan_name;
  char *gchan_color;
  int gchan_type;
  struct rank_info *ranks;
  struct guildie_info *guildies;
  struct gskill_info *gskills;
  struct gzone_info *gzones;
  struct gequip_info *gequip;
  struct ghelp_info *ghelp;
  long gflags;
  int guildwalk_room;
  int gold;
  struct guild_info *next;
 };

struct char_guild_element {
	struct guild_info *guild;
	struct guildie_info *guildie;
	struct char_guild_element *next;
};

/* Functions and stuff */

extern void GuildToXml(struct guild_info *g, FILE *f);
extern void XmlToGuild(char *file);
extern struct guild_info *guilds_data;
extern void free_guild_data(void);

#endif
