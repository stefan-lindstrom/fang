#include <time.h>
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "spells.h"
#include "db.h"
#include "handler.h"
#include "xmlRooms.h"
#include "xmlobjsave.h"
#include "retainer.h"
#include "guild_parser.h"
#include "guild.h"

#define GET_RETAINER(ch, i)        ch->player_specials->retainer[i]
#define NUM_RETAINERS   10



struct retainer_template_data retainer_templates[] =
{
    {
        "Peasant",
        10000, 1, 1, 100, 5, 10, 10, 1, 0,
        0,0
    },
    {
        "Assassin",
        32000, 200, 15000, 15000, 15000, 15000, 0, 211, 0,
        0,0
    },
    {
        "Child of the Light",
        0, 20, 1000, 1000, 150, 20, 0, 20, 1,
        MOB_NOSTUN | MOB_NOBLIND | MOB_NOSLEEP,
        0
    },
    {
        "Seanchan channeler",
        0, 20, 500, 1000, 75, 20, 0, 20, 12,
        MOB_CHANNELER,
        0
    },
    {
        "Bloodservant",
        0, 20, 1000, 1000, 150, 20, 0, 20, 12, 
        MOB_NOSTUN | MOB_NOBLIND | MOB_NOSLEEP,
        0
    },

    { "\n",         0, 0, 0, 0, 0, 0, 0, 0, 0,0 }
};


int any_owned_retainer_is_fighting(struct char_data *ch)
{
  int i;
  if (IS_NPC(ch))
    return 0;

  for (i = 0; i < NUM_RETAINERS; i ++)
    if (GET_RETAINER(ch, i) && GET_RETAINER(ch,i)->mob &&
        FIGHTING(GET_RETAINER(ch, i)->mob))
      return 1;
  return 0;
}

int get_num_retainers(struct char_data *ch)
{
  int i = 0;
  if (!ch || IS_NPC(ch))
    return 0;
    
  for (i = 0; i < NUM_RETAINERS; i ++)
    if (!GET_RETAINER(ch, i))
      return i;
    
    return NUM_RETAINERS;
}

int is_retainer(struct char_data *ch)
{
  return (ch && IS_NPC(ch) && 
          ch->mob_specials.retainer &&
          (ch->mob_specials.retainer->mob == ch));
}

struct retainer_template_data *get_template_by_name(char *name)
{
  int i;
  if (!name || !*name)
    return 0;
    
  for (i = 0; *retainer_templates[i].name != '\n'; i ++)
    if (!strncasecmp(retainer_templates[i].name, name, strlen(name)))
      return retainer_templates + i;
  return 0;
}

struct retainer_data *create_retainer_from_template(struct retainer_template_data *tmpl)
{
  struct retainer_data *ret = calloc(1, sizeof(struct retainer_data));
  ret->data = *tmpl;
  ret->hp = ret->data.maxhp;
  ret->move = ret->data.maxmove;
  ret->name = strdup(tmpl->name);
  ret->desc = strdup("A standard retainer hangs around here, looking a bit bored.\r\n");
  ret->doing = strdup("standing here.");
  ret->title = strdup("retainer");
  ret->owner = strdup("nobody");
  ret->last_paid = time(0);
  return ret;
}

void send_to_retainer_owner(const char *mesg, struct char_data *mob)
{
  struct char_data *owner;
  char localbuf[MAX_STRING_LENGTH];

  if (!is_retainer(mob) || 
      !mob->mob_specials.retainer->owner ||
      !*mob->mob_specials.retainer->owner ||
      !mob->mob_specials.retainer->snooped)
    return;
  if (!(owner = get_player(mob->mob_specials.retainer->owner, 0)))
    return;
  if (strstr(mesg, "\n"))
    sprintf(localbuf, "&r>>&n%s", mesg);
  else
    sprintf(localbuf, mesg);
  send_to_char(localbuf, owner);
}

void add_retainer(struct char_data *ch, struct retainer_data *ret)
{
  int index;
  if (!ret || !ch || IS_NPC(ch))
    return;
    
  if ((index = get_num_retainers(ch)) == NUM_RETAINERS)
    return;
    
  if (GET_RETAINER(ch, index)) // ???
    return;
    
  GET_RETAINER(ch, index) = ret;
  if (ret->owner && *ret->owner)
    free(ret->owner);
  ret->owner = strdup(GET_NAME(ch));
  if (ret->title && *ret->title)
    free(ret->title);
  ret->title = strdup("retainer");
}


struct retainer_data *readXmlRetainer(xmlNodePtr node)
{
  xmlNodePtr tmp;
  struct retainer_data *ret;
  char *tempStr;

  ret = calloc(1, sizeof(struct retainer_data));
  for (tmp = node->childs; tmp; tmp = tmp->next)
  {
    // All XML nodes are assumed to be standard <tag></tag>.
    // Meaning, empty tags (<tag/>) are ignored
    if (!tmp->childs) 
      continue;
    if (!strcasecmp(tmp->name, "name"))
      ret->name = xmlToString(tmp->childs);
    else if (!strcasecmp(tmp->name, "template"))
    {
      tempStr = xmlToString(tmp->childs);
      sprintf(ret->data.name, tempStr);
      free(tempStr);
    }
    else if (!strcasecmp(tmp->name, "description"))
      ret->desc = xmlToString(tmp->childs);
    else if (!strcasecmp(tmp->name, "owner"))
      ret->owner = xmlToString(tmp->childs);
    else if (!strcasecmp(tmp->name, "doing"))
      ret->doing = xmlToString(tmp->childs);
    else if (!strcasecmp(tmp->name, "title"))
      ret->title = xmlToString(tmp->childs);
    else if (!strcasecmp(tmp->name, "hp"))
      ret->hp = atoi(tmp->childs->content);
    else if (!strcasecmp(tmp->name, "maxhp"))
      ret->data.maxhp = atoi(tmp->childs->content);
    else if (!strcasecmp(tmp->name, "move"))
      ret->move = atoi(tmp->childs->content);
    else if (!strcasecmp(tmp->name, "maxmove"))
      ret->data.maxmove = atoi(tmp->childs->content);
    else if (!strcasecmp(tmp->name, "lastPaid"))
      ret->last_paid = atol(tmp->childs->content);
    else if (!strcasecmp(tmp->name, "level"))
      ret->data.level = atoi(tmp->childs->content);
    else if (!strcasecmp(tmp->name, "damage"))
      ret->data.damage = atoi(tmp->childs->content);
    else if (!strcasecmp(tmp->name, "hitroll"))
      ret->data.hitroll = atoi(tmp->childs->content);
    else if (!strcasecmp(tmp->name, "rent"))
      ret->data.rent = atoi(tmp->childs->content);
    else if (!strcasecmp(tmp->name, "mobflags"))
      ret->data.mobflags = atol(tmp->childs->content);
    else if (!strcasecmp(tmp->name, "mobskills"))
      ret->data.mobskills = atol(tmp->childs->content);
    else if (!strcasecmp(tmp->name, "playerobjs")) 
      ret->eq = xmlCopyNode(tmp, 1);
  }

  // Error catching here
  if (!ret->desc)
    ret->desc = strdup("Empty.");
  if (!ret->name)
    ret->name = strdup("retainer");
  if (!ret->title)
    ret->title = strdup("retainer");
  if (!ret->owner)
    ret->owner = strdup("nobody");
  if (!ret->doing)
    ret->doing = strdup("stands here");
  if (!*ret->data.name)
    sprintf(ret->data.name, "Unknown");

  return ret;
}

void read_retainers(struct char_data *ch)
{
  FILE* fl;
  char fname[200];
  xmlDocPtr doc;
  xmlNodePtr root, temp;
  int index = 0;
  struct retainer_data *ret;

  if (!ch || IS_NPC(ch))
    return;
  get_filename(GET_NAME(ch), fname, RETAINER_FILE);

  if (!(fl = fopen(fname, "r")))
    return;
  fclose(fl);

  if (!(doc = xmlParseFile(fname))) 
    return;

  if (!(root = doc->root)) {
    xmlFreeDoc(doc);
    return;
  }

  if (strncasecmp("retainers", root->name, strlen(root->name)))  {
    xmlFreeDoc(doc);
    return;
  }

  for (temp = root->childs; temp && index < NUM_RETAINERS; temp =  temp->next)
  {
    if (!strcasecmp(temp->name, "retainer") && (ret = readXmlRetainer(temp)))
    {
      GET_RETAINER(ch, index) =  ret;
      index ++;
    }
  }

  xmlFreeDoc(doc);
}
struct char_data *get_retainer_owner(struct char_data *mob)
{
  if (!is_retainer(mob))
    return 0;

  return get_player(mob->mob_specials.retainer->owner, 0);
}

void save_retainers(struct char_data *ch)
{
  FILE *fl;
  char fname[200], *tmpStr;
  int index;
  xmlDocPtr doc;
  xmlNodePtr retainers, temp;
  struct retainer_data *ret;

  if (!ch || IS_NPC(ch))
    return;

  get_filename(GET_NAME(ch), fname, RETAINER_FILE);

  if (!get_num_retainers(ch))
  {
    if ((fl = fopen(fname, "r"))) // Get rid of the retainers file
    {
      fclose(fl);
      unlink(fname);    
    }
    return;
  }
  
  if (!(fl = fopen(fname, "w")))
  {
    send_to_char("Unable to save your retainers. Tell a coder.\r\n", ch);
    return;
  }
  fclose(fl);
  doc = xmlNewDoc("1.0");
  retainers = xmlNewDocNode(doc, NULL, "retainers",NULL);
  doc->root = retainers;

  for (index = 0; index < NUM_RETAINERS; index ++)
  {
    if (!(ret = GET_RETAINER(ch, index)))
      continue;

    if (ret->mob) // Use the latest eq that the summoned mob carries
    {      
      if (ret->eq)
        xmlFreeNode(ret->eq);
      ret->eq = xmlNewNode(NULL, "playerobjs");
      eq_to_xml(ret->mob, ret->eq, doc);
    }
    temp = xmlNewNode(NULL, "retainer");
    xmlAddChild(retainers, temp);
    if (ret->eq && ret->eq->childs)
    {
      xmlAddChild(temp, ret->eq);
      // Since the _entire_ document is freed at the end, 
      // the eq listing must be copied, thus the clone here.
      ret->eq = xmlCopyNode(ret->eq, 1);
    }
    tmpStr = xmlEncodeEntitiesReentrant(doc, ret->name);
    xmlNewChild(temp, NULL, "name", tmpStr);
    free(tmpStr);

    tmpStr = xmlEncodeEntitiesReentrant(doc, ret->data.name);
    xmlNewChild(temp, NULL, "template", tmpStr);
    free(tmpStr);

    tmpStr = xmlEncodeEntitiesReentrant(doc, ret->desc);
    xmlNewChild(temp, NULL, "description", tmpStr);
    free(tmpStr);

    tmpStr = xmlEncodeEntitiesReentrant(doc, ret->title);
    xmlNewChild(temp, NULL, "title", tmpStr);
    free(tmpStr);

    tmpStr = xmlEncodeEntitiesReentrant(doc, ret->doing);
    xmlNewChild(temp, NULL, "doing", tmpStr);
    free(tmpStr);

    tmpStr = xmlEncodeEntitiesReentrant(doc, ret->owner);
    xmlNewChild(temp, NULL, "owner", tmpStr);
    free(tmpStr);
  
    sprintf(buf, "%d", ret->data.maxhp);
    xmlNewChild(temp, NULL, "maxhp", buf);
 
    sprintf(buf, "%d", ret->data.maxmove);
    xmlNewChild(temp, NULL, "maxmove", buf);

    sprintf(buf, "%d", ret->data.rent);
    xmlNewChild(temp, NULL, "rent", buf);

    sprintf(buf, "%ld", ret->last_paid);
    xmlNewChild(temp, NULL, "lastPaid", buf);

    sprintf(buf, "%ld", ret->data.mobflags);
    xmlNewChild(temp, NULL, "mobflags", buf);

    sprintf(buf, "%ld", ret->data.mobskills);
    xmlNewChild(temp, NULL, "mobskills", buf);

    sprintf(buf, "%d", ret->data.damage);
    xmlNewChild(temp, NULL, "damage", buf);

    sprintf(buf, "%d", ret->data.hitroll);
    xmlNewChild(temp, NULL, "hitroll", buf);

    sprintf(buf, "%d", ret->data.level);
    xmlNewChild(temp, NULL, "level", buf);

    if (ret->mob) // If it's summoned, take the current numbers
    {    
      sprintf(buf, "%d", GET_HIT(ret->mob));
      xmlNewChild(temp, NULL, "hp", buf);
  
      sprintf(buf, "%d", GET_MOVE(ret->mob));
      xmlNewChild(temp, NULL, "move", buf);
    }
    else // No mob summoned, take last numbers
    {
      sprintf(buf, "%d", ret->hp);
      xmlNewChild(temp, NULL, "hp", buf);

      sprintf(buf, "%d", ret->move);
      xmlNewChild(temp, NULL, "move", buf);
    }
  }
  xmlSaveFile(fname, doc);
  xmlFreeDoc(doc);
}

struct char_data *summon_retainer(struct retainer_data *ret)
{
  struct char_data *mob;
  if (!ret)
    return 0;
    
  mob = read_mobile(2, VIRTUAL); // Or whatever...
  if (!mob)
    return NULL;

  ret->mob = mob;
  mob->mob_specials.retainer = ret;
  GET_MAX_HIT(mob) = ret->data.maxhp;
  GET_HIT(mob) = ret->hp;
  GET_MAX_MOVE(mob) = ret->data.maxmove;
  GET_MOVE(mob) = ret->move;
  mob->mob_specials.damnodice = 1;
  mob->mob_specials.damsizedice = ret->data.damage;
  GET_HITROLL(mob) = ret->data.hitroll;
  GET_LEVEL(mob) = 1;        
  MOB_FLAGS(mob) = ret->data.mobflags | MOB_ISNPC | MOB_SENTINEL;
  MOB_SKILLS(mob) = ret->data.mobskills;
  //PRF_TOG_CHK( mob, PRF_IC ); //set even though player flag so that emotes can be seen
  mob->player.name = strdup(ret->name);
  mob->player.short_descr = strdup(ret->name);
  sprintf(buf, "%s, %s's %s, is %s.\r\n", ret->name, ret->owner, ret->title, ret->doing);
  mob->player.long_descr = strdup(buf);
  mob->player.description = strdup(ret->desc);
  mob->player.title = strdup(ret->title);
  if (ret->eq)
    equip_char_XML(mob, ret->eq);
  GET_POS(mob) = GET_DEFAULT_POS(mob) = POS_STANDING;
  
  return mob;
}

void free_retainer(struct retainer_data *ret)
{
  if (!ret)
    return;
  if (ret->name)
    free(ret->name);
  if (ret->desc)
    free(ret->desc);
  if (ret->doing)
    free(ret->doing);
  if (ret->title)
    free(ret->title);
  if (ret->eq)
    xmlFreeNode(ret->eq);

  memset(ret, 0, sizeof(struct retainer_data)); // To be on the safe side
  free(ret);
}

void remove_and_free_retainer(struct char_data *ch, struct retainer_data *ret)
{
  int i,j;
    
  if (!ret || !ch || IS_NPC(ch))
    return;
  if (!get_num_retainers(ch))
    return;
  for (i = 0; i < NUM_RETAINERS; i ++)
  {
    if (GET_RETAINER(ch, i) == ret)
    {
      for (j = i; j < NUM_RETAINERS-1; j ++) 
        GET_RETAINER(ch, j) = GET_RETAINER(ch, j + 1);
      GET_RETAINER(ch, j) = 0;
    }
  }
  free_retainer(ret);
}

void charge_for_retainers(struct char_data *ch)
{
  int cost = 0;
  int i = 0;
  time_t now = time(0);
  struct retainer_data *ret;

  if (IS_NPC(ch) || !get_num_retainers(ch))
    return;

  for (i = 0; i < NUM_RETAINERS; i ++)
  {
    ret = GET_RETAINER(ch, i);
    if (ret)
    {
      cost += ret->data.rent * ((now - ret->last_paid)/86400);
    }
  }

  if (!cost)
    return;

  if (cost > (GET_GOLD(ch) + GET_BANK_GOLD(ch))) // Can't afford!
  {
    unsummon_retainers(ch);
    send_to_char("You don't have the money to pay your retainers!\r\n", ch);
    send_to_char("They take what you have and leave you.\r\n", ch);
    for (i = 0; i < NUM_RETAINERS; i++)
    {
      if (GET_RETAINER(ch, i))
        free_retainer(GET_RETAINER(ch, i));
      GET_RETAINER(ch, i) = 0;
    }
    GET_GOLD(ch) = 0;
    GET_BANK_GOLD(ch) = 0;
    save_char(ch, NOWHERE); // To prevent cheating by deliberately crashing
    return;
  }

  // Player can afford

  asend_to_char(ch, "Time to pay your retainers! The rent is %d gold.\r\n", cost);

  if (cost > GET_GOLD(ch))
  { 
    cost -= GET_GOLD(ch);
    GET_GOLD(ch) = 0;
    GET_BANK_GOLD(ch) -= cost;
  }
  else
    GET_GOLD(ch) -= cost;

  for (i = 0; i < NUM_RETAINERS; i ++)
  {
    ret = GET_RETAINER(ch, i);
    // Yes yes, I know. Some retainers that are hired during the last day
    // don't charge for the part of this first day, but that's
    // a minor inconvenience. /Fredrik
    if (ret)
      ret->last_paid = now;
  }
}

void send_death_message_to_retainer_owner(struct char_data *ch)
{
  struct char_data *owner;
  if (!is_retainer(ch))
    return;
  if (!(owner = get_player(ch->mob_specials.retainer->owner, 0)))
    return;
  if (owner->in_room != ch->in_room)
    asend_to_char(owner, "Oh dear. Your retainer, %s, just died.\r\n",
                  GET_NAME(ch));
    
}

void unsummon_retainer(struct char_data *ch)
{
  struct retainer_data *ret;
  xmlDocPtr dummy;

  if (!is_retainer(ch))
    return;

  dummy = xmlNewDoc("1.0");
  ret = ch->mob_specials.retainer;
  if (ret->eq)
  {
    xmlFreeNode(ret->eq);
    ret->eq = xmlNewNode(NULL, "playerobjs");
  }
  
  eq_to_xml(ch, ret->eq, dummy);
  extract_char(ch, 1);
  ret->mob = 0;
  ret->snooped = 0;
  xmlFreeDoc(dummy);
}


void unsummon_retainers(struct char_data *ch)
{
  int i;
  if (!ch || IS_NPC(ch))
    return;
  for (i = 0; i < NUM_RETAINERS && GET_RETAINER(ch, i); i ++)
    if (GET_RETAINER(ch, i) && GET_RETAINER(ch, i)->mob)
    {
      unsummon_retainer(GET_RETAINER(ch, i)->mob);
    }
}

int owns_retainer(struct char_data *ch, struct char_data *mob)
{
  int i = 0;
    
  if (!ch || IS_NPC(ch) || !is_retainer(mob))
    return 0;
    
  for (i = 0; i < NUM_RETAINERS; i ++)
  {
    if (GET_RETAINER(ch, i) && (GET_RETAINER(ch, i)->mob == mob))
      return 1;
  }
  return 0;
}

struct retainer_data *get_retainer_by_name(struct char_data *ch, char *name)
{
  int i = 0;
  if (!ch || IS_NPC(ch) || !name || !*name)
    return 0;
    
  for (i = 0; i < NUM_RETAINERS; i ++)
  {
    if (GET_RETAINER(ch, i) && 
       !strncasecmp(GET_RETAINER(ch, i)->name, name, strlen(name)))
      return GET_RETAINER(ch, i);
  }
  return 0;
}

void send_retainer_syntax(struct char_data *ch)
{
    send_to_char("Syntax:\r\n", ch);
    send_to_char("retainer &ysummon <name>&n\r\n", ch);
    send_to_char("     Summons named retainer to your location.\r\n", ch);
    send_to_char("retainer &ydismiss <name>&n\r\n", ch);
    send_to_char("     Dismisses named retainer to limbo.\r\n", ch);
    send_to_char("retainer &yfetch <name>&n\r\n", ch);
    send_to_char("     Makes a loadedretainer that's \"lost\" go to your location.\r\n", ch);
    send_to_char("retainer &yfire <name>&n\r\n", ch);
    send_to_char("     Permanently gets rid of named retainer.\r\n", ch);
    send_to_char("retainer &nlist&n\r\n", ch);
    send_to_char("     Lists all the retainers you've got.\r\n", ch);
    send_to_char("retainer &yorder <name> <order including args>&n\r\n", ch);
    send_to_char("     Gives the named retainer a specific order.\r\n", ch);
    send_to_char("retainer &ysnoop <name>&n\r\n", ch);
    send_to_char("     Makes you start to see 'through the retainers eyes'.\r\n", ch);
    send_to_char("retainer &yunsnoop <name>&n\r\n", ch);
    send_to_char("     Stops your snooping of the named retainer.\r\n", ch);
    send_to_char("retainer &yname <name> <newname>&n\r\n", ch);
    send_to_char("     Changes the name of the given retainer.\r\n", ch);
    send_to_char("retainer &ydesc <name>&n\r\n", ch);
    send_to_char("     Starts a text editing session, changing the retainers desc.\r\n", ch);
    send_to_char("retainer &ydoing <name> <new doing>&n\r\n", ch);
    send_to_char("     Changes the named retainers doing.\r\n", ch);
    send_to_char("retainer &ytitle <name> <new title>&n\r\n", ch);
    send_to_char("     Changes the named retainers title (\"MM, NN's &y<title>&n\").\r\n", ch);
    send_to_char("retainer &yscore <name>&n\r\n", ch);
    send_to_char("     Display's the named retainers score sheet.\r\n", ch);
}

ACMD(do_retainer)
{
  char arg1[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH], *arg3, tmpname[100];
  struct retainer_data *ret = 0;
  struct char_data *mob, *tch = 0;
  int i;
  if (IS_NPC(ch))
  {
    send_to_char("You're a mob. Go away.\r\n", ch);
    return;
  }

  if (!get_num_retainers(ch))
  {
    send_to_char("You have no retainers. Go buy some first.\r\n", ch);
    return;
  }
  arg3 = two_arguments(argument, arg1, arg2);
  skip_spaces(&arg3);

  if (!*arg1)
  {
    send_retainer_syntax(ch);
    return;
  }

  if (!strncasecmp(arg1, "summon", strlen(arg1)))
  {
    if (!(ret = get_retainer_by_name(ch, arg2)))
    {
      send_to_char("You have no retainer by that name.\r\n", ch);
    }   
    else if (ret->mob)
    {
      send_to_char("That retainer is already summoned.\r\n", ch);
      asend_to_char(ch, "If you've lost it, try with \"retainer fetch %s\" instead.\r\n",
                    ret->name);
    }
    else
    {
      if (GET_SEX(ch)==SEX_MALE)
       act("$n summons a retainer of $s.\r\n", 0, ch, 0, 0, TO_ROOM);
      else
       act("$n summons a retainer of $ss.\r\n", 0, ch, 0, 0, TO_ROOM);

      asend_to_char(ch, "Ok, you summon %s.\r\n", ret->name);
      if ((mob = summon_retainer(ret)))
      {
        char_to_room(mob, ch->in_room);
        add_follower(mob, ch);
      } 
    }
  }
  else if (!strncasecmp(arg1, "dismiss", strlen(arg1)))
  {
    if (!(ret = get_retainer_by_name(ch, arg2)))
    {
      send_to_char("You have no retainer by that name.\r\n", ch);
    }
    else if (!ret->mob)
    {
      send_to_char("That retainer isn't summoned...\r\n", ch);
    }
    else if (ret->mob->in_room != ch->in_room)
    {
      asend_to_char(ch, "%s doesn't appear to be here.\r\n", GET_NAME(ret->mob));
    }
    else if (FIGHTING(ret->mob))
    {
      asend_to_char(ch, "%s appears to be busy fighting...\r\n", GET_NAME(ret->mob));
    }
    else
    {
      act("$n dismisses $s retainer $N.\r\n", 0, ch, 0, ret->mob, TO_ROOM);
      asend_to_char(ch, "Ok, you dismiss %s.\r\n", ret->name);
      unsummon_retainer(ret->mob);
    }
  }
  else if (!strncasecmp(arg1, "fetch", strlen(arg1)))
  {
    if (!(ret = get_retainer_by_name(ch, arg2)))
    {
      send_to_char("You have no retainer by that name.\r\n", ch);
    }
    else if (!ret->mob)
    {
      send_to_char("That retainer isn't summoned...\r\n", ch);
    }
    else if (ret->mob->in_room == ch->in_room)
    {
      asend_to_char(ch, "%s is already here.\r\n", GET_NAME(ret->mob));
    }
    else if (FIGHTING(ret->mob))
    {
      asend_to_char(ch, "%s appears to be busy fighting...\r\n", GET_NAME(ret->mob));
    }
    else
    {
      asend_to_char(ch, "You fetch %s.\r\n", ret->name);
      act("$n is suddenly summoned elsewhere by $s owner.", 1, ret->mob, 0, 0, TO_ROOM);
      char_from_room(ret->mob);
      char_to_room(ret->mob, ch->in_room);
    }
  }
  else if (!strncasecmp(arg1, "fire", strlen(arg1)))
  {
    if (!(ret = get_retainer_by_name(ch, arg2)))
    {
      send_to_char("You have no retainer by that name.\r\n", ch);
    }
    else if (ret->mob)
    {
      send_to_char("You can't fire summoned retainerss. Dismiss it first.\r\n", ch);
    }
    else
    {
      asend_to_char(ch, "Ok, you fire %s.\r\n", ret->name);
      if (ret->mob)
        extract_char(ret->mob, 0);
      remove_and_free_retainer(ch, ret);
    }
  }
  else if (!strncasecmp(arg1, "list", strlen(arg1)))
  {
    send_to_char("These are the retainers you currently have:\r\n", ch);
    
    for (i = 0; i < NUM_RETAINERS && GET_RETAINER(ch, i); i++)
    {
      ret = GET_RETAINER(ch, i);
      ctime_r(&ret->last_paid, buf2);
      asend_to_char(ch, "&y%s&n, Rent: &y%d&n coins per RL day, last paid &y%s&n\r\n", 
                    ret->name, ret->data.rent, buf2);
    }
  }
  else if (!strncasecmp(arg1, "order", strlen(arg1)))
  {
    if (!(ret = get_retainer_by_name(ch, arg2)))
    {
      send_to_char("You have no retainer by that name.\r\n", ch);
    }
    else if (!ret->mob)
    {
      asend_to_char(ch,"%s isn't summoned...\r\n", ret->name);
    }
    else if (!*arg3)
    {
      asend_to_char(ch, "Order %s to do what?\r\n", ret->name);
    }
    else
    {
      asend_to_char(ch, "You order %s to '%s'.\r\n", ret->name, arg3);
      command_interpreter(ret->mob, arg3);
    }
  }
  else if (!strncasecmp(arg1, "snoop", strlen(arg1)))
  {
    if (!(ret = get_retainer_by_name(ch, arg2)))
    {
      send_to_char("You have no retainer by that name.\r\n", ch);
    }
    else if (!ret->mob)
    {
      asend_to_char(ch,"%s isn't summoned...\r\n", ret->name);
    } 
    else if (ret->snooped)
    {
      asend_to_char(ch, "You're already snooping %s.\r\n", ret->name);
    }
    else
    {
      asend_to_char(ch, "Snoop on for %s.\r\n", ret->name);
      ret->snooped = 1;
    }
  }
  else if (!strncasecmp(arg1, "unsnoop", strlen(arg1)))
  {
    if (!(ret = get_retainer_by_name(ch, arg2)))
    {
      send_to_char("You have no retainer by that name.\r\n", ch);
    }
    else if (!ret->mob)
    {
      asend_to_char(ch,"%s isn't summoned...\r\n", ret->name);
    }
    else if (!ret->snooped)
    {
      asend_to_char(ch, "You're not snooping %s.\r\n", ret->name);
    }
    else
    {
      asend_to_char(ch, "Snoop off for %s.\r\n", ret->name);
      ret->snooped = 0;
    }
  }
  else if (!strncasecmp(arg1, "name", strlen(arg1)))
  {
    if (!(ret = get_retainer_by_name(ch, arg2)))
    {
      send_to_char("You have no retainer by that name.\r\n", ch);
    }
    else if (ret->mob)
    {
      send_to_char("You can't rename summoned retainers. Dismiss it first.\r\n", ch);
    }
    else if (!*arg3)
    {
      send_to_char("Name it as what?\r\n", ch);
    }
    else if (!Valid_Name(arg3) || char_exists(arg3) || 
      _parse_name(arg3, tmpname) || strlen(tmpname) < 2 ||
      strlen(tmpname) > MAX_NAME_LENGTH ||
      fill_word(strcpy(buf, tmpname)) || reserved_word(buf)) 
    {
      send_to_char("Illegal new name.\r\n", ch);
    }
    else
    {
      char buf[255];
      asend_to_char(ch, "Ok, you rename %s to %s.\r\n", ret->name, tmpname);
      sprintf( buf, "%s renamed %s retainer from %s to %s",
		      GET_NAME(ch), (GET_SEX(ch) == SEX_MALE) ? "his" : "her",
		      ret->name, tmpname );
      xlog( SYS_RETAINER, LVL_IMMORT, buf, TRUE);
      free(ret->name);
      ret->name = strdup(tmpname);
    }
  }
  else if (!strncasecmp(arg1, "desc", strlen(arg1)))
  {
    if (!(ret = get_retainer_by_name(ch, arg2)))
    {
      send_to_char("You have no retainer by that name.\r\n", ch);
    }
    else if (ret->mob)
    {
      send_to_char("You cannot change descriptions of summoned retainers. Dismiss it first.\r\n", ch);
    }
    else
    {
      send_to_char("(/s saves /h for help)\r\n", ch);
      send_to_char("Please format your desc to less than 75 characters per line.\r\n", ch);
      send_to_char("This will make your desc easier to read for those with smaller screens\r\n",ch);
      send_to_char("|----|----|----|----This line is the appropriate length----|----|----|----|\r\n",ch);
      if (ret->desc)  {
        send_to_char("Old retainer description:\r\n", ch);
        send_to_char(ret->desc, ch);
      }

      ch->desc->max_str = 3*(EXDSCR_LENGTH-24);
      ch->desc->backstr = ((NULL == ret->desc) ? str_dup("An average retainer\r\n") : strdup(ret->desc));
      ch->desc->str = &ret->desc;
      ch->desc->current_text_line = count_newlines(ch->desc->str) + 1;
      SET_BIT(PLR_FLAGS(ch), PLR_WRITING);
    }
  }
  else if (!strncasecmp(arg1, "doing", strlen(arg1)))
  {
    if (!(ret = get_retainer_by_name(ch, arg2)))
    {
      send_to_char("You have no retainer by that name.\r\n", ch);
    }
    else if (!*arg3)
    {
      send_to_char("Set its doing as what?\r\n", ch);
    }
    else
    {
      asend_to_char(ch, "Ok, you change %s's doing to 'is %s'.\r\n", ret->name, arg3);
      if (ret->doing)
        free(ret->doing);
      ret->doing = strdup(arg3);
      if (ret->mob)
      {
        free(ret->mob->player.long_descr);
        sprintf(buf, "%s, %s's %s, is %s.\r\n", ret->name, ret->owner, ret->title, ret->doing);        
        ret->mob->player.long_descr = strdup(buf);
      }
    }
  }
  else if (!strncasecmp(arg1, "title", strlen(arg1)))
  {
    if (!(ret = get_retainer_by_name(ch, arg2)))
    {
      send_to_char("You have no retainer by that name.\r\n", ch);
    }
    else if (!*arg3)
    {
      send_to_char("Set its title as what?\r\n", ch);
    }
    else
    {
      char buf[255];
      asend_to_char(ch, "Ok, you change %s's title to '%s'.\r\n", ret->name, arg3);
      sprintf( buf, "%s changed the title for %s retainer, %s, to %s",
		      ch->player.name,
		      (GET_SEX(ch) == SEX_MALE) ? "his" : "her", ret->name,
		      arg3 );
      xlog( SYS_RETAINER, LVL_IMMORT, buf, TRUE);
      if (ret->title)
        free(ret->title);
      ret->title = strdup(arg3);
      if (ret->mob)
      {
        free(ret->mob->player.long_descr);
        sprintf(buf, "%s, %s's %s, is %s.\r\n", ret->name, ret->owner, ret->title, ret->doing);        
        ret->mob->player.long_descr = strdup(buf);
      }
    }
  }
  else if (!strncasecmp(arg1, "score", strlen(arg1)))
  {
    if (!(ret = get_retainer_by_name(ch, arg2)))
    {
      send_to_char("You have no retainer by that name.\r\n", ch);
    }
    else if (!ret->mob)
    {
      send_to_char("You cannot score an unsummoned retainer.\r\n", ch);
    }
    else
    {
      if (!(tch = get_char_vis(ch,arg2))) 
        send_to_char("Unable to find a retainer to score!\r\n", ch);
      else
        print_score(ch, tch);
    }
  }
  else
  {
    send_retainer_syntax(ch);
  }
}

int   check_retainer_guild( struct char_data *ch, struct retainer_template_data *templ )
{
  struct char_guild_element *element = NULL;

  if( templ->guildrequired )
  {
    element = GET_CHAR_GUILDS(ch);
    while( element )
    {
      if (element->guild->id == templ->guildrequired ) 
        break;
      element = element->next;
    }
    
    //enable if guildleaders can buy other guilds retainers
    //if (!element)
    //{
    //  element = GET_CHAR_GUILDS(ch);
    //  while (element)
    //  {
    //    if (STATUS_FLAGGED(element->guildie, STATUS_GL)) 
    //      break;
    //    element = element->next;
    //  }
    //}

    //You have to be a member and not anything else to buy
    if( element )
    {
      if( !STATUS_FLAGGED(element->guildie, STATUS_MEMBER))
        element = NULL;
    } 
    if( element )
      return TRUE;
    else
      return FALSE;
  } 
  return TRUE;
}

SPECIAL(retainer_shop)
{
  int i = 0, j;
  struct retainer_template_data *templ;

  if (CMD_IS("list"))
  {
    if (IS_NPC(ch))
    {
      send_to_char("You're a mob! Get lost!\r\n", ch);
      return 1;
    }

    send_to_char("The following retainers are available for hire:\r\n", ch);
    asend_to_char(ch, "%20s %6s %5s %5s %6s %7s %5s %10s\r\n", 
                  "Name", "Cost", "Hp", "Move", "Damage", "HR", "Rent", "Lev To Buy");
    for (i = 0; *retainer_templates[i].name != '\n'; i ++)
    {
      if( (GET_LEVEL(ch) >= retainer_templates[i].minlevel) 
        && check_retainer_guild( ch, retainer_templates+i ) )
      {
        asend_to_char(ch, "%s%20s %6d %5d %5d 1d%-6d %5d %5d %-10d&n\r\n", 
                      (retainer_templates[i].guildrequired)?"&y":"",
                      retainer_templates[i].name,
                      retainer_templates[i].cost,
                      retainer_templates[i].maxhp,
                      retainer_templates[i].maxmove,
                      retainer_templates[i].damage,
                      retainer_templates[i].hitroll,
                      retainer_templates[i].rent,
                      retainer_templates[i].minlevel);
      }
    }
    return 1;
  }
  else if (CMD_IS("buy"))
  {
    int cost = 0;
    int num = 0;
    if (IS_NPC(ch))
    {
      send_to_char("You're a mob! Get lost!\r\n", ch);
      return 1;
    }

    if (get_num_retainers(ch) >= NUM_RETAINERS)
    {
      send_to_char("You can't buy any more retainers; you've filled your quota.\r\n", ch);
      return 1;
    }
    skip_spaces(&argument);
    if (!(templ = get_template_by_name(argument)))
    {
      send_to_char("There is no retainer by that name to hire.\r\n", ch);
      return 1;
    }
    if (GET_LEVEL(ch) < templ->minlevel)
    {
      send_to_char("There is no retainer by that name to hire.\r\n", ch);
      return 1;
    }

    if (templ->guildrequired)
    {
      if( !check_retainer_guild(ch, templ ))
      {
        send_to_char("There is no retainer by that name to hire.\r\n", ch);
        return 1;
      }
  
      for (j = 0, num = 0; j < NUM_RETAINERS; j ++)
        if (GET_RETAINER(ch, j) && 
          !strcasecmp(GET_RETAINER(ch, j)->data.name, 
                      templ->name))
          num ++;
      if (num >= 4)
      {
        asend_to_char(ch, "You have %d '%s' type retainers, max allowed is 4.\r\n", num, templ->name);
        send_to_char("Meaning, you can't get any more.\r\n", ch);
        return 1;
      }        
    } 
    
    cost = templ->cost;
    if (templ->cost > (GET_GOLD(ch) + GET_BANK_GOLD(ch)))
    {
      send_to_char("You can't afford that retainer!\r\n", ch);
      return 1;
    }

    send_to_char("You buy yourself a new retainer! Remember to rename it...\r\n", ch);
    act("$n buys $mself a new retainer.", 0, ch, 0, 0, TO_ROOM);
    add_retainer(ch, create_retainer_from_template(templ)); 
    if (GET_GOLD(ch) < cost)
    {
      cost -= GET_GOLD(ch);
      GET_GOLD(ch) = 0;
      GET_BANK_GOLD(ch) -= cost;
    }
    else
      GET_GOLD(ch) -= cost;

    return 1;
  }
  return 0;
}
