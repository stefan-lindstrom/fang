/******************************************************************************
 * $Id: skills.c,v 1.3 2002/11/07 04:02:04 fang Exp $
 *   
 * File: skills.c
 *
 * Created: Wed Mar 15 2000 by Stefan Lindstrom (asmodean@ludd.luth.se)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * Yet another encapsulation of skills/spells name->index# mapping. guess we'll
 * drop the old binarytree one, if this porperly allows us to find skills/weaves
 * in an non-erronmous way. :)
 *                                    
 * 
 *   $Log: skills.c,v 
 *   Revision 1.2  2002/07/28 13:56:05  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.4  2002/06/13 10:12:28  fang
 *   More free'ing stuff...
 *
 *   Revision 1.3  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2002/01/24 21:33:20  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.1.2.1  2002/01/24 20:49:37  mud
 *   More cleanup. Removed files:  specialday.c string.c string.h
 *   Removed DONT_TRADE_WITH-stuff from shops.
 *
 *   Revision 1.1  2000/03/15 13:11:17  mud
 *   First commit
 *
 *
 *****************************************************************************/
//#include <mlloc.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>

#include "interpreter.h"
#include "spells.h"
#include "skills.h"
#include "comm.h"
#include "utils.h"


typedef struct {   
  char *name;
  int   indexnum;
  // add more if needed. :)
} spell_t;

static int      numSkills = 0;
static spell_t *skills = NULL;

void free_skill_index(void)
{
  int i;
  
  for (i = 0; i < numSkills; i++) {
    FREEPTR(skills[i].name);
  }
  FREEPTR(skills);
}

static int comp(const void *A,const void *B)
{ 
  const spell_t *a = A, *b = B;

  return strcasecmp(a->name,b->name);
}  

void build_skill_index(void)
{  
  int i, j = 0;

  for (i = 1; i <= MAX_SKILLS;  i++) 
    if (strcasecmp(spells[i],"!UNUSED!")) 
      numSkills++;

  skills = calloc(numSkills,sizeof(spell_t));

  for (i = 1; i <= MAX_SKILLS;  i++)
    if (strcasecmp(spells[i],"!UNUSED!")) {  
      skills[j].name = strdup((spells[i]));
      skills[j].indexnum = i;
      j++;
    }  
  qsort(skills,numSkills,sizeof(spell_t),comp);
} 

int find_skillnum_by_name(char *skill)
{ 
  int i; 
  for (i = 0; i < numSkills; i++) 
    if (!strncasecmp(skills[i].name,skill,strlen(skill)))
      return skills[i].indexnum;
  
  return -1;
}   

   
