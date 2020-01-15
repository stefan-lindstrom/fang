/******************************************************************************
 * $Id: guild_parser.c,v 1.4 2002/11/07 04:02:02 fang Exp $
 *   
 * File: guild_parser.c
 *
 * Created: Sat May 13 2000 by Yohay Etsion (yohay_e@netvision.net.il)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: guild_parser.c,v 
 *   Revision 1.3  2002/09/23 05:52:42  fische
 *   fisch: clearing out lua triggers part 
 
 *   Revision 1.2  2002/07/28 13:56:04  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.7  2002/06/13 10:12:28  fang
 *   More free'ing stuff...
 *
 *   Revision 1.6  2002/05/27 10:52:49  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.5  2002/02/28 13:06:12  fang
 *   *** empty log message ***
 *
 *   Revision 1.4  2002/02/27 06:03:56  fang
 *   Further modifications to disguise saving code.
 *
 *   Revision 1.3  2002/02/22 19:17:19  fang
 *   Fixed entity substitution up for properties.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.4  2001/11/10 11:16:16  mud
 *   Temp fix for object status...
 *
 *   Revision 1.3  2001/10/01 08:32:36  mud
 *   More memory leak fixes.
 *
 *   Revision 1.2  2001/04/21 02:56:06  mud
 *   QPadd and GequipSend, plus greceive
 *
 *   Revision 1.1  2000/08/14 19:19:06  mud
 *   New guild system files.
 *
 *
 *****************************************************************************/

#include <stddef.h>
#include <fnmatch.h>
////#include <mlloc.h>

#include "guild_parser.h"
#include "xmlRooms.h"
#include "utils.h"
#include "handler.h"
#include "db.h"
#include "comm.h"
#include "constants.h"
#include "strutil.h"
#include "namedspec.h"

struct guild_info *guilds_data;

/****** XML savings *******/

static xmlNodePtr makeRankInfo(struct rank_info *r, xmlDocPtr doc)
{
  char buf[1024];
  xmlNodePtr rank = xmlNewNode(NULL,"rank");
  rank->doc = doc;

  xmlNewProp(rank,"name",r->name);
 
  sprintf(buf, "%ld", r->num);
  xmlNewProp(rank, "num", buf);

  return rank;
}

static xmlNodePtr makeSponsorerInfo(struct sponsorer_info *s, xmlDocPtr doc)
{
  char buf[1024];
  xmlNodePtr sponsorer = xmlNewNode(NULL, "sponsorer");
  sponsorer->doc = doc;

  sprintf(buf, "%ld", s->idnum);
  xmlNewProp(sponsorer, "idnum", buf);

  xmlNewProp(sponsorer, "name", s->name);

  return sponsorer;
}

static xmlNodePtr makeQPInfo(struct qp_info *qpinfo, xmlDocPtr doc)
{
  char buf[1024];
  xmlNodePtr qpadd = xmlNewNode(NULL,"qpadd");
  qpadd->doc = doc;

  sprintf(buf, "%ld", qpinfo->num);
  xmlNewProp(qpadd, "num", buf);

  xmlNewProp(qpadd, "name", qpinfo->name);

  return qpadd;
}

static xmlNodePtr makeGequipInfo(struct gequip_info *g, xmlDocPtr doc)
{
  char buf[1024];
  xmlNodePtr gequip = xmlNewNode(NULL,"gequip");
  gequip->doc = doc;

  sprintf(buf, "%d", g->vnum);
  xmlNewProp(gequip,"vnum", buf);

  return gequip;
}

static xmlNodePtr makeGuildieInfo(struct guildie_info *g, xmlDocPtr doc)
{
  char buf[1024];
  xmlNodePtr guildie = xmlNewNode(NULL,"guildie");
  struct sponsorer_info *s;
  struct qp_info *qp;
  struct gequip_info *gequip;
  guildie->doc = doc;

  sprintf(buf, "%ld", g->idnum);
  xmlNewProp(guildie,"idnum", buf);

  xmlNewProp(guildie,"name",g->name);
 
  sprintf(buf, "%ld", g->rank_num);
  xmlNewProp(guildie,"rank_num", buf);

  xmlNewProp(guildie,"subrank",g->subrank);

  sprintf(buf, "%ld", g->perm);
  xmlNewProp(guildie,"perm", buf);

  sprintf(buf, "%ld", g->status);
  xmlNewProp(guildie,"status", buf);

  sprintf(buf, "%ld", g->deposited);
  xmlNewProp(guildie,"deposited", buf);

  sprintf(buf, "%ld", g->withdrew);
  xmlNewProp(guildie,"withdrew", buf);

  s = g->sponsorers;

  while (s) {
   xmlAddChild(guildie, makeSponsorerInfo(s, doc));
   s = s->next;
  }

  qp = g->qpadd;
  while (qp) {
   xmlAddChild(guildie, makeQPInfo(qp, doc));
   qp = qp->next;
  }

  gequip = g->gequipsent;
  while (gequip) {
   xmlAddChild(guildie, makeGequipInfo(gequip, doc));
   gequip = gequip->next;
  }

  return guildie;
}

static xmlNodePtr makeGskillInfo(struct gskill_info *g, xmlDocPtr doc)
{
  char buf[1024];
  xmlNodePtr gskill = xmlNewNode(NULL,"gskill");
  gskill->doc = doc;

  sprintf(buf, "%d", g->skill);
  xmlNewProp(gskill,"skill", buf);

  sprintf(buf, "%d", g->maximum_set);
  xmlNewProp(gskill,"maximum_set", buf);
  
  return gskill;
}

static xmlNodePtr makeGzoneInfo(struct gzone_info *g, xmlDocPtr doc)
{
  char buf[1024];
  xmlNodePtr gzone = xmlNewNode(NULL,"gzone");
  gzone->doc = doc;

  sprintf(buf, "%d", g->zone);
  xmlNewProp(gzone,"zone", buf);
  
  return gzone;
}

static xmlNodePtr makeGhelpInfo(struct ghelp_info *g, xmlDocPtr doc)
{
  char *p;
  xmlNodePtr ghelp = xmlNewNode(NULL, "ghelp");
  ghelp->doc = doc;

  xmlNewProp(ghelp, "keyword", g->keyword);

  xmlNewChild(ghelp,NULL,"entry",(p = internalXmlString(g->entry,doc)));
  free(p);

  return ghelp;
}

static xmlNodePtr makeGuildBasic(struct guild_info *g,xmlDocPtr doc)
{
  char buf[1024],*p;
  xmlNodePtr basic = xmlNewNode(NULL,"guildbasic");
  basic->doc = doc;

  xmlNewProp(basic,"name",g->name);
  xmlNewProp(basic,"guildfile",g->guild_filename);
  xmlNewProp(basic,"gossip_name",g->gossip_name);

  sprintf(buf, "%d", g->type);
  xmlNewProp(basic,"type", buf);

  sprintf(buf, "%d", g->id);
  xmlNewProp(basic,"id", buf);

  xmlNewProp(basic, "gltitle", g->gl_title);

  xmlNewProp(basic, "guildietitles", g->guildie_titles);

  sprintf(buf, "%ld", g->gflags);
  xmlNewProp(basic,"gflags", buf);
  
  sprintf(buf, "%d", g->guildwalk_room);
  xmlNewProp(basic,"guildwalk_room", buf);

  xmlNewChild(basic,NULL,"desc",(p = internalXmlString(g->description,doc)));
  free(p);

  xmlNewChild(basic,NULL,"reqs",(p = internalXmlString(g->requirements,doc)));
  free(p);

  xmlNewChild(basic,NULL,"gossip",(p = internalXmlString(g->gossip,doc)));
  free(p);

  xmlNewChild(basic,NULL,"gchan",(p = internalXmlString(g->gchan_name,doc)));
  free(p);
  
  xmlNewProp(basic->childs->next->next->next, "gchan_color", g->gchan_color);

  sprintf(buf, "%d", g->gchan_type);
  xmlNewProp(basic->childs->next->next->next, "gchan_type", buf);

  sprintf(buf, "%d", g->gold);
  xmlNewProp(basic, "gold", buf);

  return basic;
}

void GuildToXml(struct guild_info *g, FILE *f)
{
  xmlNodePtr guild;
  xmlDocPtr doc;
  struct gskill_info *gskills;
  struct guildie_info *guildies;
  struct gzone_info *gzones;
  struct rank_info *ranks;
  struct gequip_info *gequip;
  struct ghelp_info *ghelp;
  char *tmp = NULL;
  int pos = -1;

  doc = xmlNewDoc("1.0");
  guild = xmlNewDocNode(doc, NULL, "guildfile", NULL);
  doc->root = guild;

  xmlAddChild(guild, makeGuildBasic(g, doc));

  gskills = g->gskills;
  guildies = g->guildies;
  gzones = g->gzones;
  ranks = g->ranks;
  gequip = g->gequip;
  ghelp = g->ghelp;

  while (ranks) {
      xmlAddChild(guild, makeRankInfo(ranks, doc));
      ranks = ranks->next;
  }

  while (gskills) {
      xmlAddChild(guild, makeGskillInfo(gskills, doc));
      gskills = gskills->next;
  }

  while (gzones) {
      xmlAddChild(guild, makeGzoneInfo(gzones, doc));
      gzones = gzones->next;
  }

  while (guildies) {
          tmp = strlower(guildies->name);
      if ((pos = find_name(tmp)) != NOTHING)
        xmlAddChild(guild, makeGuildieInfo(guildies,doc));
          free(tmp);
      guildies = guildies->next;
  }

  while (gequip) {
      xmlAddChild(guild, makeGequipInfo(gequip, doc));
      gequip = gequip->next;
  }

  while (ghelp) {
      xmlAddChild(guild, makeGhelpInfo(ghelp, doc));
      ghelp = ghelp->next;
  }

  xmlDocDump(f, doc);
  xmlFreeDoc(doc); 
}


/******** XML loadings *********/

static void fetchRankInfo(xmlNodePtr rank, struct rank_info *r)
{
  r->num = xmlAtoi(xmlGetProp(rank, "num"));
  r->name = xmlGetProp(rank, "name");
}

static void fetchSponsorerInfo(xmlNodePtr sponsorer, struct sponsorer_info *s)
{
  s->idnum = xmlAtoi(xmlGetProp(sponsorer, "idnum"));
  s->name = xmlGetProp(sponsorer, "name");
}

static void fetchQPInfo(xmlNodePtr qpadd, struct qp_info *qpinfo)
{
  qpinfo->name = xmlGetProp(qpadd, "name");
  qpinfo->num = xmlAtoi(xmlGetProp(qpadd, "num"));
}

static void fetchGequipInfo(xmlNodePtr gequip, struct gequip_info *g)
{
  g->vnum = xmlAtoi(xmlGetProp(gequip, "vnum"));
}

static void fetchGuildieInfo(xmlNodePtr guildie, struct guildie_info *g)
{
  struct sponsorer_info *s;
  struct qp_info *qp;
  struct gequip_info *gequip;
  xmlNodePtr temp;

  g->idnum = xmlAtoi(xmlGetProp(guildie, "idnum"));
  g->name = xmlGetProp(guildie, "name");
  g->rank_num = xmlAtoi(xmlGetProp(guildie, "rank_num"));
  g->subrank = xmlGetProp(guildie, "subrank");
  g->perm = xmlAtoi(xmlGetProp(guildie, "perm"));
  g->status = xmlAtoi(xmlGetProp(guildie, "status"));
  g->deposited = xmlAtoi(xmlGetProp(guildie, "deposited"));
  g->withdrew = xmlAtoi(xmlGetProp(guildie, "withdrew"));

  for (temp = guildie->childs; NULL != temp; temp = temp->next) {
   if (!strcasecmp(temp->name, "SPONSORER")) {
    s = calloc(1, sizeof(struct sponsorer_info));
    s->next = g->sponsorers;
    g->sponsorers = s;
    fetchSponsorerInfo(temp, s);
   }
   if (!strcasecmp(temp->name, "QPADD")) {
    qp = calloc(1, sizeof(struct qp_info));
    qp->next = g->qpadd;
    g->qpadd = qp;
    fetchQPInfo(temp, qp);
   }
   if (!strcasecmp(temp->name, "GEQUIP")) {
    gequip = calloc(1, sizeof(struct gequip_info));
    gequip->next = g->gequipsent;
    g->gequipsent = gequip;
    fetchGequipInfo(temp, gequip);
   } 
  }    
}

static void fetchGskillInfo(xmlNodePtr gskill, struct gskill_info *g)
{
  g->skill = xmlAtoi(xmlGetProp(gskill, "skill"));
  g->maximum_set = xmlAtoi(xmlGetProp(gskill, "maximum_set"));
}

static void fetchGzoneInfo(xmlNodePtr gzone, struct gzone_info *g)
{
  g->zone = xmlAtoi(xmlGetProp(gzone, "zone"));
}

static void fetchGhelpInfo(xmlNodePtr ghelp, struct ghelp_info *g)
{
  g->keyword = xmlGetProp(ghelp, "keyword");
  g->entry = xmlToString(ghelp->childs->childs);
}

static void fetchGuildBasics(xmlNodePtr basic, struct guild_info *g)
{
  xmlNodePtr temp;

  g->name = xmlGetProp(basic, "name");
  g->guild_filename = xmlGetProp(basic, "guildfile");
  g->gossip_name = xmlGetProp(basic, "gossip_name");
  g->gl_title = xmlGetProp(basic, "gltitle");
  g->guildie_titles = xmlGetProp(basic, "guildietitles");
  g->type = xmlAtoi(xmlGetProp(basic, "type"));
  g->id = xmlAtoi(xmlGetProp(basic, "id"));
  g->gflags = xmlAtoi(xmlGetProp(basic, "gflags"));
  g->guildwalk_room = xmlAtoi(xmlGetProp(basic, "guildwalk_room"));
  g->gold = xmlAtoi(xmlGetProp(basic, "gold"));

  for (temp = basic->childs; temp != NULL; temp = temp->next) {
    if (!strcasecmp(temp->name, "desc"))
     g->description = xmlToString(temp->childs);

    else if (!strcasecmp(temp->name, "reqs"))
     g->requirements = xmlToString(temp->childs);

    else if (!strcasecmp(temp->name, "gossip"))
     g->gossip = xmlToString(temp->childs);

    else if (!strcasecmp(temp->name, "gchan")) {
     g->gchan_name = xmlToString(temp->childs);
     g->gchan_color = xmlGetProp(temp, "gchan_color");
     g->gchan_type = xmlAtoi(xmlGetProp(temp, "gchan_type"));
    }

    else 
     continue;
  }
}

void XmlToGuild(char *file)
{
   xmlNodePtr root, temp;
   xmlDocPtr doc;
   struct guild_info *g;
   struct gequip_info *gequip;
   struct gskill_info *gskill;
   struct guildie_info *guildie;
   struct gzone_info *gzone;
   struct rank_info *rank, *rank2;
   struct ghelp_info *ghelp;
 
   if (!(doc = xmlParseFile(file))) {
    alog("XmlToGuild:[guild_parser.c]: Couldn't parse XML-file %s",file);
    return;
   }
   if (!(root = doc->root)) {
    alog("XmlToGuild:[guild_parser.c]: No XML-root in document %s",file);
    xmlFreeDoc(doc);
    return;
   }
   if (strncasecmp("GUILDFILE",root->name,strlen(root->name))) {
    alog("XmlToGuild:[guild_parser.c]: XML-root (%s in file %s) is not a room-object!",root->name,file);
    xmlFreeDoc(doc);
    return;
   }

   g = calloc(1, sizeof(struct guild_info));
   g->next = guilds_data;
   guilds_data = g;

   for (temp = root->childs; NULL != temp; temp = temp->next) {

    if (!strcasecmp(temp->name, "GUILDBASIC")) 
    fetchGuildBasics(temp, g);

    else if (!strcasecmp(temp->name, "RANK")) {
          rank = calloc(1, sizeof(struct rank_info));
      if (!g->ranks) {
       g->ranks = rank;
      }
      else {
       rank2 = g->ranks;
       while (rank2->next != NULL)
        rank2 = rank2->next;
       rank2->next = rank;
      }
      fetchRankInfo(temp, rank);
    }	

    else if (!strcasecmp(temp->name, "GUILDIE")) { 
          guildie = calloc(1, sizeof(struct guildie_info));
      guildie->next = g->guildies;
      g->guildies = guildie;
      fetchGuildieInfo(temp, guildie);
          rank = g->ranks;
          while (rank) 
            if (guildie->rank_num == rank->num) {
             guildie->rank = rank;
             rank = 0;
            }
            else rank = rank->next;
    }

    else if (!strcasecmp(temp->name, "GSKILL"))  {
      gskill = calloc(1, sizeof(struct gskill_info));
      gskill->next = g->gskills;
      g->gskills = gskill;
      fetchGskillInfo(temp, gskill);
    }

    else if (!strcasecmp(temp->name, "GZONE"))   {
      gzone = calloc(1, sizeof(struct gzone_info));
      gzone->next = g->gzones;
      g->gzones = gzone;
      fetchGzoneInfo(temp, gzone);
    }

    else if (!strcasecmp(temp->name, "GEQUIP"))  {
      gequip = calloc(1, sizeof(struct gequip_info));
      gequip->next = g->gequip;
      g->gequip = gequip;
      fetchGequipInfo(temp, gequip);
    }
 
    else if (!strcasecmp(temp->name, "GHELP"))  {
      ghelp = calloc(1, sizeof(struct ghelp_info));
      ghelp->next = g->ghelp;
      g->ghelp = ghelp;
      fetchGhelpInfo(temp, ghelp);
    }
   }
   xmlFreeDoc(doc);
}

void free_guild_data(void)
{
  struct guild_info     *gld,   *gnext;
  struct ghelp_info     *ghlp,  *ghnext;
  struct gzone_info     *zon,   *znext;
  struct gequip_info    *eq,    *enext;
  struct gskill_info    *skill, *snext;
  struct guildie_info   *member,*mnext;
  struct rank_info      *rank,  *rnext;
  struct sponsorer_info *spons, *spnext;
  struct qp_info        *qp,    *qnext;

  for (gld = guilds_data; NULL != gld; gld = gnext) {
    gnext = gld->next;

    FREEPTR(gld->name);
    FREEPTR(gld->description);
    FREEPTR(gld->requirements);
    FREEPTR(gld->gossip);
    FREEPTR(gld->gl_title);
    FREEPTR(gld->guildie_titles);
    FREEPTR(gld->guild_filename);
    FREEPTR(gld->gossip_name);
    FREEPTR(gld->gchan_name);
    FREEPTR(gld->gchan_color);

    /*structs---
      struct rank_info *ranks;
    */

    for (rank = gld->ranks; NULL != rank; rank = rnext) {
      rnext = rank->next;
      FREEPTR(rank->name);
      free(rank);
    }

    for (member = gld->guildies; NULL != member; member = mnext) {
      mnext = member->next;
      
      FREEPTR(member->name);
      FREEPTR(member->subrank);

      for (spons = member->sponsorers; NULL != spons; spons = spnext) {
    spnext = spons->next;

    FREEPTR(spons->name);
    free(spons);
      }

      for (qp = member->qpadd; NULL != qp; qp = qnext) {
    qnext = qp->next;
    FREEPTR(qp->name);
    free(qp);
      }

      for (eq = member->gequipsent; NULL != eq; eq = enext) {
    enext = eq->next;

    free(eq);
      }
      free(member);
    }

    for (skill = gld->gskills; NULL != skill; skill = snext) {
      snext = skill->next;
      free(skill);
    }

    for (ghlp = gld->ghelp; NULL != ghlp; ghlp = ghnext) {
      ghnext = ghlp->next;

      FREEPTR(ghlp->keyword);
      FREEPTR(ghlp->entry);
      free(ghlp);
    }
    
    for (zon = gld->gzones; NULL != zon; zon = znext) {
      znext = zon->next;
      free(zon);
    }

    for (eq = gld->gequip; NULL != eq; eq = enext) {
      enext = eq->next;
      free(eq);
    }

    free(gld);
  }
  guilds_data = NULL;
}
