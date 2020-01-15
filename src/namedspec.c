/*****************************************************************************
 * $Id: namedspec.c,v 1.3 2002/11/07 04:02:03 fang Exp $
 *
 * File: namedspec.c
 *
 * Created: Wed 2 Mar 2000 by Stefan Lindstrom (asmodean@ludd.luth.se)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada,
 * yada...
 *
 * Code for named specials. Specials will (at least) be saved by name in
 * rooms, and will be setable via OLC.
 *
 *   $Log: namedspec.c,v 
 *   Revision 1.2  2002/07/28 13:56:04  fan
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
 *   Revision 1.5  2002/01/24 21:33:17  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.4.2.1  2002/01/24 20:49:37  mud
 *   More cleanup. Removed files:  specialday.c string.c string.h
 *   Removed DONT_TRADE_WITH-stuff from shops.
 *
 *   Revision 1.4  2001/09/28 13:59:27  mud
 *   *** empty log message ***
 *
 *   Revision 1.3  2001/03/17 10:40:47  mud
 *   New and Improved TAR.
 *
 *   Revision 1.2  2000/03/01 15:15:45  mud
 *   Named specials added, saved in room XML, and setable in redit
 *
 *   Revision 1.1  2000/03/01 12:54:19  mud
 *   First commit
 *
 *
 *****************************************************************************/

//#include <mlloc.h>
#include <stddef.h>

#include "namedspec.h"
#include "dll.h"
#include "structs.h"
#include "spec_procs.h"
#include "interpreter.h"
#include "comm.h"


struct spec_data
{
  char *name;
  SPECIAL(*fn);
};

typedef struct spec_data spec_t, *spec_tp;

static DLL __named;

static void free_named_e(ELEMENT e) 
{
  spec_t *n = (spec_t *)e;
  FREEPTR(n->name);
  free(n);
}

void de_boot_namedspecials(void)
{
  deleteAll(__named,free_named_e);
  __named = NULL;
}

static int strMatch(ELEMENT a, ELEMENT b)
{
  char *str = (char *)b;

  return (!strcasecmp(str,((spec_tp)a)->name));
}

spec_p getSpecialByNum(int numbertotry)
{
  spec_tp p = elementAt(__named,(numbertotry));

  return (NULL == p ? NULL : p->fn);
}

void  makeSpecialList(struct char_data *ch)
{
  int i = 1,smax = 0;
  spec_tp p;

  first(__named);
  do {
    p = this(__named);
    smax=MAX(smax,strlen(p->name));
  } while(next(__named));

  first(__named);
  do {
    p = this(__named);
    asend_to_char(ch,"%3d) %s %*s",i,p->name,(smax - strlen(p->name)),"");
    if (!(i % 2))
      send_to_char("\r\n",ch);
    i++;
  } while(next(__named));
}

void *getSpecialName(char *name)
{
  if (NULL == __named)
    return NULL;
   
  if (!search(__named,name,strMatch))
    return ((spec_tp)this(__named))->fn;
  return NULL;
}

static int ptrMatch(ELEMENT a, ELEMENT b)
{
  if (((spec_tp)a)->fn == b)
    return 1;
  return 0;
}

void *getSpecialPointer(SPECIAL(*fn))
{
  if (NULL == __named)
    return NULL;

  if (!search(__named,fn,ptrMatch))
    return ((spec_tp)this(__named))->name;

  return NULL;
}

void registerSpecial(char *name, SPECIAL(*fn))
{
  spec_tp s;

  if (NULL == __named)
    __named = createDll();

  if (!getSpecialName(name)) {
    s = calloc(1,sizeof(spec_t));
    s->name = strdup(name);
    s->fn   = fn;
    append(__named,s);
  }
}

void bootNamedSpecials(void)
{
  registerSpecial("Dump",dump);
  registerSpecial("PostMaster",postmaster);
  registerSpecial("Puff",puff);
  registerSpecial("Fido",fido);
  registerSpecial("Janitor",janitor);
  registerSpecial("Bank",bank);
  registerSpecial("Taunt",taunt);  
  registerSpecial("Guild",guild);
  registerSpecial("Puff",puff);
  registerSpecial("Fido",fido);
  registerSpecial("Thief",thief);
  registerSpecial("Trainer",trainer);
  registerSpecial("Laras",laras);
  registerSpecial("HeadGear",headgear);
  registerSpecial("Pigeon",pigeon);   
  registerSpecial("Herb",herb);  
  registerSpecial("Aelfinn",aelfinn);
  registerSpecial("RecallCharm",recall_charm);
  registerSpecial("WeakDreamObj",weak_dream_obj);   
  registerSpecial("StrongDreamObj",strong_dream_obj);
  registerSpecial("WarderCloak",warder_cloak); 
  registerSpecial("Customs",customs);
  registerSpecial("Hospital",hospital);
  registerSpecial("SkillShop",skillshop);
  registerSpecial("PortalStones",portal_stones);
  registerSpecial("DreamRoom",dreamroom);
  registerSpecial("RepairShop",repair_shop);
  registerSpecial("NewbieHealer",newbiehealer);

  //  registerSpecial("GenBoard",gen_board); 
  // Don't 'boot' board-spec until better board code is available. 
}

ACMD(test_ns)
{
  send_to_char("Doesn't do anything right now :)\r\n",ch);
}



