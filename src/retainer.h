#ifndef __RETAINER_H__
#define __RETAINER_H__

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/entities.h>
#include <libxml/parserInternals.h>
#include <errno.h>

struct retainer_template_data
{
    char name[100];
    int cost, level, maxhp, maxmove, damage, hitroll, rent, minlevel;
    int guildrequired;
    long mobflags, mobskills;
};

struct retainer_data
{
  struct char_data *mob;
  char *name, *desc, *doing, *title, *owner;
  struct retainer_template_data data;
  int hp, move;
  time_t last_paid;
  xmlNodePtr eq;
  int snooped; // Not saved
};

extern SPECIAL(retainer_shop);
extern int is_retainer(struct char_data *ch);
extern void send_to_retainer_owner(const char *mesg, struct char_data *mob);
extern void save_retainers(struct char_data *ch);
extern void read_retainers(struct char_data *ch);
extern void unsummon_retainers(struct char_data *ch);
extern int any_owned_retainer_is_fighting(struct char_data *ch);
extern struct char_data *get_retainer_owner(struct char_data *mob);
extern void send_death_message_to_retainer_owner(struct char_data *ch);
extern void remove_and_free_retainer(struct char_data *ch, struct retainer_data *ret);
extern void charge_for_retainers(struct char_data *ch);
extern void print_score(struct char_data *ch,struct char_data *tch);
extern int Valid_Name(char *newname);
extern int char_exists(char *name);
ACMD(do_retainer);

#endif
