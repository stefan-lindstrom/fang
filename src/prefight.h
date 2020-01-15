#ifndef __PREFIGHT_H__
#define __PREFIGHT_H__

#include "structs.h"

extern int chance_to_hit(struct char_data * ch, struct char_data * victim, int attack_skill, int type);
extern int avg_damage( struct char_data *ch,struct char_data *victim, int type );

#endif
