/******************************************************************************
 * $Id: act.guild.c,v 1.31 2004/02/23 07:13:49 fang Exp $
 *   
 * File: act.guild.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: act.guild.c,v $
 *   Revision 1.31  2004/02/23 07:13:49  fang
 *   Changed a typo in do_dazzle so gender displays properly
 *
 *   Revision 1.30  2004/02/23 03:47:22  fang
 *   Fixed reported bug on 'dazzle' skill. Now you cannot dazzle yourself.
 *
 *   Revision 1.29  2003/12/05 05:19:20  fang
 *   Tze: Fixed typo with darkfriend 'summon' skill
 *
 *   Revision 1.28  2003/07/17 02:03:42  fang
 *   the throw gskill (HOUSES) no longer works in peaceful rooms.
 *
 *   Revision 1.27  2003/06/10 12:16:08  fang
 *
 *   Added UNIQUE flag for mobs and objs. A unique obj/mob can only exists one
 *   instance at the time on the MUD, no matter what. A unique obj will not
 *   save to characters obj file either.
 *
 *   Revision 1.26  2003/06/03 07:54:55  fang
 *   Removed the affect from herbident, since it no longer serves a purpose.
 *
 *   Revision 1.25  2003/06/03 07:38:28  fang
 *   Removed the delay on herbident.
 *
 *   Revision 1.24  2003/06/03 07:19:39  fang
 *   Herbident and Findherb can now be improved with use.
 *
 *   Revision 1.23  2003/05/22 02:41:36  fang
 *   Fixed 'dazzle' so that people without the skill always get the 'Huh?!?'
 *   response when they try to use it. -Tze
 *
 *   Revision 1.22  2003/02/04 08:45:23  fang
 *   Fixed bug in deletion of entries in ignore/notify lists. Pound can no longer be used in peaceful rooms
 *
 *   Revision 1.21  2003/02/03 09:04:43  fang
 *   NPC-check in add_to_rplog
 *
 *   Revision 1.20  2003/02/02 15:05:57  fang
 *   Fixed gender-vars in act() in markedmap.
 *
 *   Revision 1.19  2002/11/07 04:02:01  fang
 *   Fisch: more rohbust malloc of memory with error checking
 *
 *   Revision 1.18  2002/09/23 02:57:56  fischer
 *   Fischer: no fortify in peaceful rooms
 *
 *   Revision 1.17  2002/09/21 07:27:57  fang
 *   Typo in the redit xlog string
 *
 *   Revision 1.16  2002/09/16 18:22:56  fang
 *   Put in WC guild retainers and some IC checks for retainers.
 *
 *   Revision 1.15  2002/09/14 05:30:17  fang
 *   fisch: wolfcall - wolf 501 caused problems going N or NW so removed out
 *   of options 1, 2, 7 and 8 in switch/case
 *
 *   Revision 1.14  2002/09/13 04:54:08  fang
 *   Drath: Fixed ravage check and extended bargain and charm to 2 hours
 *
 *   Revision 1.13  2002/09/05 04:50:34  fang
 *   fischer:  changed wolfcall to not load 502 - this causes !look in whatever
 *   room they wolf is in.  Found by Kalenth and an errant wolfkin.
 *
 *   Revision 1.12  2002/08/27 21:29:38  fang
 *   Drath: Player Extra Descriptions
 *
 *   Revision 1.11  2002/08/20 21:03:50  fang
 *   Drath: Hunter GSKILLS
 *
 *   Revision 1.10  2002/08/14 03:09:48  fang
 *   Drath: Minor change to MarkedMap
 *
 *   Revision 1.9  2002/08/13 22:24:20  fang
 *   Drath: MarkedMap hunter GSKILL
 *
 *   Revision 1.8  2002/08/06 21:24:27  fang
 *   Drath: Reinabled all ignore settings
 *
 *   Revision 1.7  2002/08/04 09:39:55  fang
 *   Drath: SOCIALIDEA and SHOWSOCIALIDEA seperate from IDEA
 *
 *   Revision 1.6  2002/07/30 14:39:46  fang
 *   Drath: Another gskill update
 *
 *   Revision 1.5  2002/07/29 22:18:56  fang
 *   AFF3 created and gskill update
 *
 *   Revision 1.4  2002/07/29 01:14:47  fang
 *   Drath: Bug fix, gskill update
 *
 *   Revision 1.3  2002/07/28 13:56:03  fang
 *   Removed dmlloc...just to test.
 *
 *   Revision 1.2  2002/07/26 19:39:52  fang
 *   Dath: MYCOLOR mem leak fixed.  Thanks, Fred!
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.10  2002/07/16 15:16:16  fang
 *   Drath: Seafolk Guildskills are done.
 *
 *   Revision 1.9  2002/07/12 03:37:32  fang
 *   Drath: MYCOLOR
 *
 *   Revision 1.8  2002/06/20 03:07:10  fang
 *   Maz: RPTAG
 *
 *   Revision 1.7  2002/06/01 20:43:16  fang
 *   Maz: LOAD and CLONE log real name of disguised players, and fixed SFX bug.
 *
 *   Revision 1.6  2002/05/27 10:52:48  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.5  2002/04/30 18:54:56  fang
 *   AC Cap - Request per CRE
 *
 *   Revision 1.4  2002/03/19 02:22:19  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/03/11 08:15:26  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2002/02/22 15:04:27  fang
 *   *** empty log message ***
 *
 *   Revision 1.53  2002/02/19 14:01:05  mud
 *   act\(\) fixes and stuff
 *
 *   Revision 1.52  2002/02/17 21:23:37  mud
 *   *** empty log message ***
 *
 *   Revision 1.51  2002/01/30 13:25:27  mud
 *   Bug fix with treesing guildskill...
 *
 *   Revision 1.50  2002/01/24 21:33:11  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.49.2.1  2002/01/18 02:24:50  mud
 *   More cleanup...
 *
 *   Revision 1.49  2001/12/21 15:31:33  mud
 *   Fixed sfx bug.
 *
 *   Revision 1.48  2001/11/06 11:31:39  mud
 *   Arrrestees now stops followinf arrester when unarrested/effect wears off.
 *
 *   Revision 1.47  2001/11/06 10:16:28  mud
 *   Can no longer make camp while fighting
 *
 *   Revision 1.35  2001/06/17 07:09:11  mud
 *   Fixed do_fx so it can only be used when the character is IC. Also
 *   have it make it check that before it goes into rplog of the characters
 *   in the room it checks each person to see if they are mailing or writing
 *   instead of just the character who issues the command.
 *
 *   Revision 1.30  2001/03/17 10:40:45  mud
 *   New and Improved TAR.
 *
 *   Revision 1.26  2000/12/01 12:37:40  mud
 *   changed GET_GUILD()s over to get_guild()s
 *
 *   Revision 1.17  2000/09/27 17:54:51  mud
 *   Updated rage skill to have correct output to players
 *
 *   Revision 1.6  2000/04/13 16:09:34  mud
 *   Fixed a typo in the message that gets echoed to the room when using the
 *   fervor skill.
 *
 *   Revision 1.3  2000/04/10 22:19:52  mud
 *   New Talent System
 *
 *   Revision 1.2  2000/02/21 14:00:15  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/*************************************************************************
*   File: act.guild.c                                Part of Dragons Fang *
*  Usage: Miscellaneous guild skills                                      *
*                                                                         *
*  No copyright at all :)                                                 *
************************************************************************ */

#include "skimming.h"
#include "magic.h"
#include "constants.h"
#include "conf.h"
#include "sysdep.h"
#include "graph.h"
#include "class.h"
#include "config.h"
#include <sys/stat.h>
#include "act.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "spells.h"
#include "screen.h"
#include "mobprog.h"
#include "fight.h"
#include "guild.h"
#include "guild_parser.h"
#include "xmlcharsave.h"
#include "mobact.h"

// #include <dmlloc.h>


/* Added by Adammael 11/13/99 */
ACMD(do_enhance)
{
  struct obj_data *obj;
  int found = 0,result,i,hmod = 0;
  long objt = 0;

  if (!GET_SKILL(ch,SKILL_ENHANCE)) {
    send_to_char("You don't know how!\r\n",ch);
    return;
  }
  
  skip_spaces(&argument);
  if (!*argument) {
    send_to_char("Usage: Enhance <object>\r\nWhere object is of type "
         "\\c01armor\\c00, \\c01bow\\c00.\r\n",ch);
    return;
  }
 
  for (obj = ch->carrying; obj && !found; obj = obj->next_content) {
    if (isname(argument,obj->name)) {
      found = TRUE;
      break;
    }
  }
  if (!found || (NULL == obj)) {
    asend_to_char(ch,"You don't seem to have a \"\\c01%s\\c00\", whatever it is.\r\n",argument);
    return;
  }
  
  if (CAN_WEAR(obj,ITEM_WEAR_NOSE) || CAN_WEAR(obj,ITEM_WEAR_EAR)) {
    asend_to_char(ch,"Ear/Nose EQ can not be enhanced!\r\n");
    return;
  }

  if (IS_SET(OBJ_EXTRA_FLAGS(obj),ITEM_ENHANCED)) {
    asend_to_char(ch,"%s is already enhanced!\r\n",obj->short_description);
    return;
  }
  objt = GET_OBJ_TYPE(obj);

  if ((ITEM_ARROW != objt)&&(ITEM_ARMOR != objt)) {
    asend_to_char(ch,"Hey, you can't enhance %s, it's not the right kind of object!\r\n",obj->short_description);
    return;
  }

  SET_BIT(OBJ_EXTRA_FLAGS(obj),ITEM_ENHANCED);

  result = generic_result_mod(ch,SKILL_ENHANCE,NULL,FALSE,-(10 - GET_INT(ch)));
  if ((TOTAL_FUCKUP == result)||(TOTAL_FAILURE == result)) {
    asend_to_char(ch,"You grab your trusty toolkit and start hammering on %s, and it instantly breaks apart!\r\n",obj->short_description);
    act("$n grabs his trysty Acme Craftsman Toolkit(tm), and starts hammering at $p, and $p instantly breaks into pieces!\r\n",FALSE,ch,obj,0,TO_ROOM);
    do_say(ch,"Whoops!",0,0);
    extract_obj(obj);
    obj = NULL;
    return;
  } else {
    asend_to_char(ch,"As the expert craftsmen you are, you soon enhance %s mightily.\r\n",obj->short_description);
    act("$n grabs his toolkit, and starts hammering away at $p, and soon comes back with it, better than new!",FALSE,ch,obj,0,TO_ROOM);
    
    switch(GET_OBJ_TYPE(obj)) {
      case ITEM_ARROW:
      case ITEM_WEAPON:
      case ITEM_ARMOR:
    GET_OBJ_VAL(obj,0) += hmod;
    GET_OBJ_VAL(obj,1) += (hmod + 4);
        found = FALSE;
        hmod  = number((GET_SKILL(ch,SKILL_ENHANCE)/15),(GET_SKILL(ch,SKILL_ENHANCE)/10));
        for (i = 0; (i < MAX_OBJ_AFFECT) && !found;i++) {
          if ((APPLY_AC == obj->affected[i].location)) {
            obj->affected[i].modifier -= hmod;
            found = TRUE;
          }
          if ((APPLY_ABS == obj->affected[i].location)) {
            obj->affected[i].modifier += hmod;
            found = TRUE;
          }
        }
        if (!found) {
          i = 0;
          while(obj->affected[i].location && (i < MAX_OBJ_AFFECT)) i++;
            if (i < MAX_OBJ_AFFECT) {
              obj->affected[i].modifier = hmod;
              obj->affected[i].location = APPLY_ABS;
            }
        }        
        break;
    }
  }   
  do_save(ch,"",0,0); 
}

ACMD(do_preach)
{
  struct char_data *ppl;
  struct affected_type af;
  af = clear_aff_var(af);

  if (!GET_SKILL(ch, SKILL_PREACH)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }

  if (!can_use_gskill(ch, SKILL_PREACH)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }
  if (affected_by_spell(ch, SKILL_PREACH)) {
    send_to_char("You just did that !\r\n", ch);
    return;
  } 
  act("$n tells you of the Dragon reborn and you feel better!", TRUE, ch, 0, 0, TO_ROOM);
  for (ppl = world[ch->in_room].people; ppl; ppl = ppl->next_in_room)
   GET_HIT(ppl) = (int) (GET_HIT(ppl)*1.3);
  send_to_char("Ok, you preach in front of all these people.\r\n", ch);
  af.type = SKILL_PREACH;
  af.duration = 7;
  af.modifier = 0;
  af.location = APPLY_NONE;
  af.bitvector = 0;
  affect_to_char(ch, &af);
}

ACMD(do_fortify)
{ struct affected_type raf,*hlp2,*hlp3;
  int result;
  struct char_data *hlp = NULL;

  raf = clear_aff_var(raf);

  if (!GET_SKILL(ch,SKILL_FORTIFY)) 
  { send_to_char("Huh?!?\r\n",ch);
    return;
  }

  if (ROOM_FLAGGED(ch->in_room,ROOM_PEACEFUL))
  { send_to_char( "Fortifications are not needed in peaceful rooms!\r\n", ch );
    act( "$n seems to be afraid of the \"outside\".", FALSE, ch, 0, 0, TO_ROOM );
    return;
  }

  if (ROOM_AFFECTED(ch->in_room,RAFF_FORTIFY)) 
  { for (hlp2 = world[ch->in_room].aff; hlp2; hlp2 = hlp3) 
    { hlp3 = hlp2->next;
      if (hlp2->type == SKILL_FORTIFY) 
      { if (hlp2->weaver == GET_IDNUM(ch)) 
	{ send_to_char("Okay, you tear down your fortifications...\r\n",ch);
	  act("$n starts to tear down his fortifications...",FALSE,ch,0,0,TO_ROOM);
	  affect_from_room(&world[ch->in_room],SKILL_FORTIFY);
        } 
	else 
	{ send_to_char("Hey, you can't tear down somebody elses fortifications!\r\n",ch);
          act("Hmm...$n tries to tear down fortifications, but he cannot!",FALSE,ch,0,0,TO_ROOM);
        }
      }
    }
    return;
  }

  result = generic_result(ch,SKILL_FORTIFY,NULL,FALSE);
  if (!RESULT_OK(result)) 
  { send_to_char("You fail.\r\n",ch);
    act("$n starts to build fortifications, but he seems to have major problem.",FALSE,ch,0,0,TO_ROOM);
    return;
  }
  raf.duration  = 50;
  raf.bitvector = RAFF_FORTIFY;
  raf.speed     = RAFF_SLOW;
  raf.type      = SKILL_FORTIFY;
  raf.weaver    = GET_IDNUM(ch);
  affect_to_room(&world[ch->in_room],&raf);
  send_to_char("Okay, you build fortifications in this room....\r\n",ch);
  act("$n starts building -\\c01HUGE\\c00- fortifications right here!",FALSE,ch,0,0,TO_ROOM);

  for (hlp = world[ch->in_room].people; hlp; hlp = hlp->next_in_room) 
  { if ((ch != hlp) && IS_AFFECTED(hlp,AFF_HIDE) && (GET_SKILL(hlp,SKILL_HIDE) < 75)) 
    { REMOVE_BIT(AFF_FLAGS(hlp),AFF_HIDE);
      asend_to_char(hlp,"As %s fortifies the room, you are forced out of hiding!\r\n",GET_NAME(ch));
      act("$n is forced out of hiding as $N fortifies the room.",TRUE,hlp,0,ch,TO_NOTCHAR);
    }
  }
}

ACMD(do_mask)
{
  struct affected_type af;
  int result;

  char *mess_s[] = {
    "You weave flows of spirit to mask your channeling ability.",
    "$n weaves flows of spirit and suddenly you can no longer feel that $n can channel at all!",
    "$n looks mightily concentrated for a while..."
  };
  char *mess_f[] = {
    "You mess up your weaves!",
    "$n weave flows of Spirit, but nothing seems to happen.",
    "$n looks disgruntled, dunno why...."
  };

  af = clear_aff_var(af);
  af.bitvector2 = AFF2_MASKED;
  af.duration = 200;
  af.type = SKILL_MASK;

  if (!GET_SKILL(ch,SKILL_MASK)) {
    send_to_char("You don't know how!\r\n",ch);
    return;
  }

  if (IS_AFFECTED2(ch,AFF2_MASKED)) {
    send_to_char("Okay, you un-mask your channeling ability.\r\n",ch);
    affect_from_char(ch,SKILL_MASK);
    return;
  }
  result = generic_result(ch,SKILL_MASK,NULL,FALSE);
  if (!RESULT_OK(result)) {
    act(mess_f[0],TRUE,ch,0,0,TO_CHAR);
    act(mess_f[1],TRUE,ch,0,0,TO_OPUSERSS);
    act(mess_f[2],TRUE,ch,0,0,TO_OPUSERNSS);
    act(mess_f[2],TRUE,ch,0,0,TO_NOTCHANN);
    return;
  }

  affect_to_char(ch,&af);
  act(mess_s[0],TRUE,ch,0,0,TO_CHAR);
  act(mess_s[1],TRUE,ch,0,0,TO_OPUSERSS);
  act(mess_s[2],TRUE,ch,0,0,TO_OPUSERNSS);
  act(mess_s[2],TRUE,ch,0,0,TO_NOTCHANN);  
}

ACMD(do_fine)
{
  struct char_data *victim = NULL;
  char personstr[1000], amountstr[1000];
  int amount = 0;

  if (IS_NPC(ch) || !GET_SKILL(ch, SKILL_FINE)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }

  skip_spaces(&argument);
  if (!*argument) {
    send_to_char("Syntax: Fine <person> amount\r\n", ch);
    return;
  }
  two_arguments(argument, personstr, amountstr);

  if (0 >= (amount = atoi(amountstr))) {
    send_to_char("Please write a reasonable amount.\r\n", ch);
    return;
  }  
  if (!(victim = get_char_room_vis(ch, personstr))) {
    send_to_char(NOPERSON, ch);
    return;
  }
  if (IS_NPC(victim) || (ch == victim)) {
    send_to_char("Very funny...\r\n", ch);
    return;
  }
  if (GET_GOLD(victim) < amount) {  
    asend_to_char(ch,"%s doesn't carry that much money.\r\n", GET_NAME(victim));
    return;
  }
  asend_to_char(ch, "Okay, you fine %s %d coins for ...something.\r\n", GET_NAME(victim), amount);
  act("$n fines $N an amount of money, which $E pays.", TRUE, ch, 0, victim, TO_ROOM);
  asend_to_char(victim, "%s fines you %d coins!\r\n", GET_NAME(ch), amount);
  GET_GOLD(victim) -= amount;
}

ACMD(do_offend)
{
  struct char_data *victim = NULL;
  struct affected_type af,af2;
  char personstr[1000];
  int result;

  if (!can_use_gskill(ch, SKILL_OFFEND)) 
    {
    send_to_char("Huh?!?\r\n",ch);
    return;
    }

  if (!GET_SKILL(ch, SKILL_OFFEND)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }

  skip_spaces(&argument);
  if (!*argument) {
    send_to_char("Syntax: offend <victim>\r\n", ch);
    return;
  }

  one_argument(argument, personstr);
  if (!(victim = get_char_room_vis(ch, personstr))) {
    send_to_char(NOPERSON, ch);
    return;
  }
  if (ch == victim) {
    send_to_char("You start to hate yourself...\r\n", ch);
    return;
  }
  if (GET_LEVEL(victim) >= LVL_IMMORT) {
    send_to_char("Immortals are used to being offended...\r\n",ch);
    return;
  }

  if (IS_AFFECTED2(victim,AFF2_INSULTED)) {
    asend_to_char(ch, "Give the poor %s a break.\r\n", ((GET_SEX(victim) == SEX_MALE) ? "guy" : "gal" ));
    return;
  }
  af  = clear_aff_var(af);
  af2 = clear_aff_var(af2);

  af.bitvector2 = AFF2_INSULTED;
  af.duration   = MAX(10,20-(GET_LEVEL(victim))/10);
  af.type       = SKILL_OFFEND;
  af.location   = APPLY_AC;
  af.modifier   = MIN((GET_LEVEL(ch)/2),50);

  af2.bitvector2 = af.bitvector2;
  af2.duration   = af.duration;
  af2.type       = af.type;
  af2.location   = APPLY_HITROLL;
  af2.modifier   = -MIN((GET_LEVEL(ch)/2),50);

  result = generic_result_mod(ch,SKILL_OFFEND,NULL,FALSE,(GET_INT(ch) - 25));

  if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
    asend_to_char(ch,"You just fell in love with %s.\r\n",GET_NAME(victim));
    return;
  }
  affect_to_char(victim,&af);
  affect_to_char(victim,&af2);

  asend_to_char(ch, "Okay, you insult %s until %s's ready to go for your throat.\r\n", GET_NAME(victim), ((GET_SEX(victim) == SEX_MALE) ? "he" : "she" ));
  act("$n insults $N till $E seems ready to explode.", TRUE, ch, 0, victim, TO_NOTVICT);
  asend_to_char(victim, "%s insults you in a most infuriating way!\r\n", GET_NAME(ch));
}

ACMD(do_prodigy)
{
  struct char_data *vict;
  struct affected_type vicaf, chaf;

  vicaf = clear_aff_var(vicaf);
  chaf = clear_aff_var(chaf);

  one_argument(argument, arg);
  
  if (!GET_SKILL(ch, SKILL_PRODIGY)) {
    send_to_char("Huh? How?\r\n", ch);
    return;
  }

  if (!can_use_gskill(ch, SKILL_PRODIGY)) {
    send_to_char("Huh?!?\r\n",ch);
    return;
  }
 
  if (IS_AFFECTED(ch,AFF_NOTEACH)) {
    send_to_char("You feel too tired right now.\r\n",ch);
    return;
  }

  if (!*arg){
    send_to_char("You have to tell me who to teach as well...\r\n", ch);
    return;
  }
  
  if (!(vict = get_char_room_vis(ch, arg))) {
    send_to_char("Teach who? \r\n", ch);
    return;
  }

  if (IS_NPC(vict)) {
    send_to_char("Very funny...teaching mobs..This attempt to abuse advantages is now logged.\r\n", ch);
    return;
  }

  if (IS_AFFECTED(vict,AFF_NOTEACH)) {
    send_to_char("That person is too exhausted right now.\r\n",ch);
    return;
  }

  if (HPTAUGHT(vict) > 25) {
    act("$N wouldn't gain anything from any more lessons.", TRUE, ch, 0, vict, TO_CHAR);
    return;
  }

  if (vict == ch) {
    send_to_char("Trying to teach yourself!? Yeah right.\r\n", ch);
    return;
  }
  
  if (!can_use_gskill(vict, SKILL_PRODIGY)) {
    send_to_char("You can't.\r\n", ch);
    return;
  }

  GET_MAX_HIT(vict) += 40;
  GET_HIT(vict)     += 40;
  HPTAUGHT(vict) ++;   /* number of lessons */

  vicaf.type = SKILL_PRODIGY;
  vicaf.duration = 10;
  vicaf.location = APPLY_NONE;
  vicaf.modifier = 0;
  vicaf.bitvector = AFF_NOTEACH;
  
  chaf.type = SKILL_PRODIGY;
  chaf.duration = 10;
  chaf.location = APPLY_NONE;
  chaf.modifier = 0;
  chaf.bitvector = AFF_NOTEACH;
  
  affect_to_char(vict, &vicaf);
  affect_to_char(ch, &chaf);
  
  act("$n trains $N skillfully, and $N feels far better now.", TRUE, ch, 0, vict, TO_NOTVICT);
  sprintf(buf,"%s gives you deeper insight in how to survive.\r\n", GET_NAME(ch));
  send_to_char(buf, vict);
  sprintf(buf, "You tell %s a little about life, universe and everything.\r\n", GET_NAME(vict));
  send_to_char(buf, ch);  
}

ACMD(do_shieldwall)
{
  struct affected_type af,af2;
  int result;

  if (!can_use_gskill(ch, SKILL_SHIELDWALL)) 
    {
    send_to_char("You wish you could, don't you?\r\n",ch);
    return;
    }

  if (!GET_SKILL(ch,SKILL_SHIELDWALL)) {
    send_to_char("You honestly don't know how to!\r\n",ch);
    return;
  }

  if (IS_AFFECTED2(ch,AFF2_SHIELDWALL)) {
    send_to_char("You're already safe behind your shield..\r\n",ch);
    return;
  }

  af  = clear_aff_var(af);
  af2 = clear_aff_var(af2);

  af.bitvector2 = AFF2_SHIELDWALL;
  af.duration   = (GET_LEVEL(ch)/2);
  af.type       = SKILL_SHIELDWALL;
  af.location   = APPLY_AC;
  af.modifier   = -20;

  af2.bitvector = af.bitvector;
  af2.duration  = af.duration;
  af2.type      = af.type;
  af2.location  = APPLY_ABS;
  af2.modifier  = 20;

  result = generic_result_mod(ch,SKILL_SHIELDWALL,NULL,FALSE,(GET_INT(ch) - 25));

  if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
    send_to_char("You fail to hide behind a shieldwall.\r\n",ch);
    return;
  }
  affect_to_char(ch,&af);
  affect_to_char(ch,&af2);
  send_to_char("You strap your shield around your arm feeling invincible.\r\n",ch);
  act("$n straps his shield around his arm, looking safer behind it.",FALSE,ch,NULL,NULL,TO_ROOM);
}

/* now for a shameless copy of shieldwall */
ACMD(do_arrogance)
{
  struct affected_type af,af2;
  int result;

  if (!can_use_gskill(ch, SKILL_ARROGANCE)) 
    {
    send_to_char("You wish you could, don't you?\r\n",ch);
    return;
    }

  if (!GET_SKILL(ch,SKILL_ARROGANCE)) {
    send_to_char("You honestly don't know how to!\r\n",ch);
    return;
  }

  if (IS_AFFECTED2(ch,AFF2_ARROGANCE)) {
    send_to_char("Not even you can be as arrogant as that...\r\n",ch);
    return;
  }

  af  = clear_aff_var(af);
  af2 = clear_aff_var(af2);

  af.bitvector2 = AFF2_ARROGANCE;
  af.duration   = (GET_LEVEL(ch)/4);
  af.type       = SKILL_ARROGANCE;
  af.location   = APPLY_AC;
  af.modifier   = -MIN((GET_LEVEL(ch)/2),50);

  af2.bitvector2 = af.bitvector2;
  af2.duration   = af.duration;
  af2.type       = af.type;
  af2.location   = APPLY_ABS;
  af2.modifier   = MIN((GET_LEVEL(ch)/2),50);

  result = generic_result_mod(ch,SKILL_ARROGANCE,NULL,FALSE,(GET_INT(ch) - 25));

  if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
    send_to_char("You are too humble right now.\r\n",ch);
    return;
  }
  affect_to_char(ch,&af);
  affect_to_char(ch,&af2);
  send_to_char("You know that you can conquer the world now.\r\n",ch);
  act("$n looks around the room in an incredibly arrogant way.",FALSE,ch,NULL,NULL,TO_ROOM);
}

ACMD(do_concentrate) {
  struct affected_type af,af2;
  int result;

  if (!can_use_gskill(ch, SKILL_CONCENTRATE)) 
  {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }

  if (!GET_SKILL(ch,SKILL_CONCENTRATE)) {
    send_to_char("You don't know how to find the Void!\r\n",ch);
    return;
  }

  if (IS_AFFECTED2(ch,AFF2_CONCENTRATE)) {
    send_to_char("You're already as concentrated as you can be....\r\n",ch);
    return;
  }

  af  = clear_aff_var(af);
  af2 = clear_aff_var(af2);

  af.bitvector2 = AFF2_CONCENTRATE;
  af.duration   = 10 + (GET_LEVEL(ch)/5) + number((GET_SKILL(ch,SKILL_CONCENTRATE)/5),(GET_SKILL(ch,SKILL_CONCENTRATE)/4));
  af.type       = SKILL_CONCENTRATE;
  af.location   = APPLY_HITROLL;
  af.modifier   = MIN(20,(GET_LEVEL(ch)/10) +
(GET_SKILL(ch,SKILL_CONCENTRATE)/5));

  af2.bitvector = af.bitvector;
  af2.duration  = af.duration;
  af2.type      = af.type;
  af2.location  = APPLY_DAMROLL;
  af2.modifier  = af.modifier;

  result = generic_result_mod(ch,SKILL_CONCENTRATE,NULL,FALSE,(GET_INT(ch) - 25));

  if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
    send_to_char("You fail to find the Void.\r\n",ch);
    if (GET_MANA(ch) > 500)
      SET_MANA(ch, GET_MANA(ch) - 200);
    else
      GET_MOVE(ch) = MAX(0, GET_MOVE(ch) - 100);
    return;
  }
  affect_to_char(ch,&af);
  affect_to_char(ch,&af2);
  send_to_char("You feel invincible as you reach the Void.\r\n",ch);
  act("$n looks more intent.",FALSE,ch,NULL,NULL,TO_ROOM);
}

ACMD(do_fx)
{
  struct char_data * i = NULL;
  skip_spaces(&argument);
  if (!GET_SKILL(ch, SKILL_FX) && !GET_SKILL(ch,SKILL_SFX) ) 
  {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }

  if (!PRF_FLAGGED(ch, PRF_IC))
  {
    send_to_char("You have to be IC.\r\n", ch);
    return;
  }

  if (subcmd == SCMD_FX) {
    if (!can_use_gskill(ch, SKILL_FX)) {
      send_to_char("Go away!  You can't use this skill!\r\n", ch);
      return;
    }
    subcmd = 0;
  } 

  if (!*argument) {
    send_to_char("Yes.. but what?\r\n", ch);
    return;
  } else {
      if (subcmd == SCMD_EMOTE) {
        if (*argument == '\'')
          sprintf(buf, "$n%s", coltext(argument, ch));
        else
          sprintf(buf,"$n %s", coltext(argument, ch));
      }
      else
        strcpy(buf, argument);

     /* Commented this out to replace it with act() TO_CHAR
      *
      * act(buf, FALSE, ch, 0, 0, TO_ROOM);
      */

      sprintf(buf2, "%s (%s)\r\n", coltext(buf, ch), GET_NAME(ch));

      for (i = world[ch->in_room].people; i; i = i->next_in_room) {
    if ( (IS_NPC(i) && (i->desc)) || (!IS_NPC(i))) {
      if ((IS_SET(PRF_FLAGS(i), PRF_IC) || GET_LEVEL(i) > LVL_IMMORT) && AWAKE(i) && !PLR_FLAGGED(i, PLR_WRITING | PLR_MAILING)) {
            if (i==ch)
              continue;
            if (PRF2_FLAGGED(i, PRF2_RPTAG))
              send_to_char("[RP] ", i);
            act(coltext(buf, ch), FALSE, i, 0, 0, TO_CHAR);
            add_to_rplog(i, coltext(buf2, ch),ch);
      }
    }
      }
    }

    if (PRF_FLAGGED(ch, PRF_NOREPEAT))
      send_to_char(OK, ch);
    else {
      if (*argument) {
        if (PRF2_FLAGGED(ch, PRF2_RPTAG)) {
          send_to_char("[RP] ", ch);
	}
      }
      act(coltext(buf, ch), FALSE, ch, 0, 0, TO_CHAR);
    }

    to_eavesdropper(coltext(strcat(buf,"\r\n"), ch), ch->in_room);    
 
}

ACMD(do_obscure)
{
  int fool_level = 0;

  if (IS_NPC(ch) || !is_dark(ch) || !GET_SKILL(ch, SKILL_OBSCURE)) {
    send_to_char("You can't do that!\r\n", ch);
    return;
  }
  one_argument(argument, arg);
  if (!*arg || !(is_number(arg))) {
    send_to_char("What do you want to set your virtual level to?\r\n", ch);
    return;
  }

  if (strlen(arg) < 4)
    fool_level = atoi(arg);
  else
    fool_level = GET_LEVEL(ch);
  if ((fool_level < 150) && (fool_level > 0))
    ch->player_specials->fool_level = fool_level;
  else
    ch->player_specials->fool_level = GET_LEVEL(ch);
  sprintf(buf, "Level shown to other players is now %ld.\r\n",
    ch->player_specials->fool_level);
  send_to_char(buf, ch);
}

ACMD(do_tail)
{
 int result;
 struct affected_type af;

 af = clear_aff_var(af);
 if (!GET_SKILL(ch, SKILL_TAIL))
 {
   send_to_char("Huh?!?\r\n", ch);
   return;
 }

 result = generic_result(ch, SKILL_TAIL, NULL,FALSE);
 if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE))
 {
   send_to_char("You fail utterly.\r\n", ch);
   return;
 }
 SET_BIT(AFF_FLAGS(ch), AFF_TAILING);
 do_follow(ch, argument, 0, 0);
 if (!IS_AFFECTED(ch, AFF_SNEAK))
 {
  af.type = SKILL_SNEAK;
  af.duration = MIN(GET_LEVEL(ch), 20);
  af.modifier = 0;
  af.location = APPLY_NONE;
  SET_BIT(af.bitvector, AFF_SNEAK);
  affect_to_char(ch, &af);
 }
}

ACMD(do_inquisition)
{
  int result;
  struct affected_type af;
  
  af = clear_aff_var(af);
  if ((!GET_SKILL(ch, SKILL_INQUISITION)) && (!GET_SKILL(ch, SKILL_SHADOW)))
    {
      send_to_char("Huh?!?\r\n", ch);
      return;
    }
  if (!GET_SKILL(ch, SKILL_SNEAK))
    {
      send_to_char("You need to be able to sneak to use this skill!!\r\n", ch);
      return;
    }
  if (!GET_SKILL(ch, SKILL_HIDE))
    {
      send_to_char("You need to be able to hide to use this skill!!\r\n", ch);
      return;
    }
  if (!can_use_gskill(ch, SKILL_INQUISITION) && !can_use_gskill(ch, SKILL_SHADOW)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }

  /* repeat after me: I shall never again use two names for one skill... */ 
  if (can_use_gskill(ch, SKILL_INQUISITION))
     result = generic_result(ch, SKILL_INQUISITION, NULL,FALSE);
  else
     result = generic_result(ch, SKILL_SHADOW, NULL,FALSE);
  if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
    send_to_char("You fail utterly.\r\n", ch);
    return;
  }

  if (!IS_AFFECTED(ch, AFF_SNEAK))
    {
      result = generic_result_mod(ch,SKILL_SNEAK,NULL,FALSE,(GET_DEX(ch) - 15));
      if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE))
    {
      send_to_char("You fail utterly.\r\n", ch);
      return;
    }
    }
  if (!IS_AFFECTED(ch, AFF_HIDE))
    {
      result = generic_result_mod(ch,SKILL_HIDE,NULL,FALSE,(GET_INT(ch) - 15));
      if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE))
    {
      send_to_char("You fail utterly.\r\n", ch);
      return;
    }
      SET_BIT(AFF_FLAGS(ch), AFF_HIDE);
      /* hmm, would SET_BIT(af.bitvector, AFF_HIDE); work as well if I do affect_to_char? */
    }

  /* this is so inelegant that it hurts... but we don't want to start him sneaking if hide fails */
  if (!IS_AFFECTED(ch, AFF_SNEAK)) 
    {
      af.type = SKILL_SNEAK;
      af.duration = MIN(GET_LEVEL(ch), 20);
      af.modifier = 0;
      af.location = APPLY_NONE;
      SET_BIT(af.bitvector, AFF_SNEAK);
      affect_to_char(ch, &af);
    }
  
  SET_BIT(AFF_FLAGS(ch), AFF_TAILING); /* do I need aff_inquiring ??? */
  do_follow(ch, argument, 0, 0); /* remember to make sure that theres no message for the victim there */
}
  

ACMD(do_pound)
{
  struct char_data *victim;
  int result,mprog = 0,dam = 0;

  skip_spaces(&argument);

  if (!IS_NPC(ch) && !GET_SKILL(ch,SKILL_POUND)) {
    send_to_char("Huh?!?\r\n",ch);
    return;
  }

  if (ROOM_FLAGGED(ch->in_room,ROOM_PEACEFUL)) {
    send_to_char("Hey! This is a peaceful room!\r\n",ch);
    return;
  }

  if (GET_TIMES(ch,CAN_POUND)) {
    send_to_char("You haven't regained your balance since your last pound.\r\n",ch);
    return;
  }
  if (!*arg || !(victim = get_char_room_vis(ch,argument))) {
    if (!(victim = FIGHTING(ch))) {
      send_to_char("Pound on whom?\r\n",ch);
      return;
    }
  }
  if (ch == victim) {
    send_to_char("You pound yourself into the gorund...You're dead!\r\n",ch);
    send_to_char("Goodbye, muddict...You will come back soon!\r\n",ch);
    send_to_char("Connection closed by foreign host.\r\n\r\n\r\n\r\n\r\n\r\n",ch);
    send_to_char("...Nah, I'm just pulling your leg. Don't try to use these skills on yourself inna future, ok?\r\n",ch);
    asend_to_room(ch->in_room,"%s is trying to pound %s into the ground!\r\n",GET_NAME(ch), ((GET_SEX(ch) == SEX_MALE) ? "himself" : "herself" ));
    return;
  }
  if (!GET_EQ(ch,WEAR_WIELD_R)) {
    send_to_char("Better wield something to pound with, eh?\r\n",ch);
    return;
  }
  
  dam = dice(GET_OBJ_VAL(GET_EQ(ch,WEAR_WIELD_R),1),GET_OBJ_VAL(GET_EQ(ch,WEAR_WIELD_R),2));
  dam += GET_DAMROLL(ch);
  dam *= 2;

  mprog = check_for_mobprogs_and_stops(ch, victim, NULL, MPC_ATTACK, 0, 0);
  if (mprog != 2) {  
    if (IS_NPC(ch))
      if (number(0,100) < number(70,MIN((GET_LEVEL(ch)/30),30)))
        result = SOME_SUCCESS;
      else
        result = TOTAL_FAILURE;
    else   
      result = generic_result_mod(ch,SKILL_POUND,NULL,FALSE,-(GET_AC(ch) / 10));

   if (IS_NPC(victim) && MOB_FLAGGED(victim,MOB_NOSTUN))
    result = TOTAL_FAILURE;

    if ((GET_LEVEL(victim) >= LVL_IMMORT) || (result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
      damage(ch,victim,0,SKILL_POUND);
      SET_TIMES(ch,CAN_POUND, (4 * PULSE_VIOLENCE));
      GET_POS(ch) = POS_SITTING;
      return;
    }
    if (result == SOME_SUCCESS) {
      WAIT_STATE(victim,PULSE_VIOLENCE * 2);
      damage(ch,victim,dam,SKILL_POUND);
      GET_POS(victim) = POS_STUNNED;
      SET_TIMES(ch,CAN_POUND, (3 * PULSE_VIOLENCE));
      update_pos(victim);
      return;
    }
    if (result == TOTAL_SUCCESS) {
      WAIT_STATE(victim,PULSE_VIOLENCE * 3);
      GET_POS(victim) = POS_STUNNED;
      SET_TIMES(ch,CAN_POUND, (2 * PULSE_VIOLENCE));
      damage(ch,victim,2*dam,SKILL_POUND);
      update_pos(victim);
      return;
    }
  }
  if (mprog)
    run_mobprog(ch, 0);
}

ACMD(do_speardance)
{
  int result,dam,prob;
  struct char_data *vict;

  skip_spaces(&argument);

  if (!GET_SKILL(ch,SKILL_SPEARDANCE)) {
    send_to_char("Huh?!?\r\n",ch);
    return;
  }
  if (!*argument||(NULL == (vict = get_char_room_vis(ch,argument)))) {
    send_to_char("Speardance with whom?\r\n",ch);
    return;
  }
  if (ch == vict) {
    act("$n starts to dance the spears with $eself.",TRUE,ch,0,ch,TO_ROOM);
    send_to_char("Fun to fight yerself?\r\n",ch);
  }
  if (ROOM_FLAGGED(ch->in_room,ROOM_PEACEFUL)) {
    send_to_char("Fight? Here? You try hard to get angry, but all you can think of is cute, fluffy bunnies.\r\n",ch);
    return;
  }
  if ((GET_POS(ch) == POS_FIGHTING) || FIGHTING(ch)) {
    send_to_char("You're to busy fighting to use this skill right now.\r\n",ch);
    return;
  }
  if (!GET_EQ(ch,WEAR_WIELD_R)) {
    send_to_char("Better wield something in yer right hand, pal...\r\n",ch);
    return;
  }
  result = generic_result(ch,SKILL_SPEARDANCE,NULL,FALSE);
  prob   = GET_SKILL(ch,SKILL_SPEARDANCE)+GET_HITROLL(ch)+5*MAX(-20,GET_AC(vict)/10);
  if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE) || (prob < number(0,100))) {
    damage(ch,vict,0,SKILL_SPEARDANCE);
    return;
  }
  send_to_char("You prepare for the \\c01dance\\c00 and attack!",ch);
  dam = dice(GET_OBJ_VAL(GET_EQ(ch,WEAR_WIELD_R),1),GET_OBJ_VAL(GET_EQ(ch,WEAR_WIELD_R),2));
  dam += GET_DAMROLL(ch);
  dam *= backstab_mult(GET_LEVEL(ch));
  damage(ch,vict,dam,SKILL_SPEARDANCE);
}

ACMD(do_rage)
{
  int result,mod;
  struct affected_type af,af2,af3;

  af  = clear_aff_var(af);
  af2 = clear_aff_var(af2);
  af3 = clear_aff_var(af3);

  if (!GET_SKILL(ch,SKILL_RAGE)) {
    send_to_char("Huh?!?\r\n",ch);
    return;
  }
  if (IS_AFFECTED2(ch,AFF2_RAGE)) {
    send_to_char("Hey, you can't get more angry that you are right now!\r\n",ch);
    return;
  }
  mod = (GET_LEVEL(ch) / 10)+(GET_STR(ch)/6);
  af3.type = af2.type = af.type = SKILL_RAGE;
  af3.duration = af2.duration = af.duration = 6;
  af3.modifier = af2.modifier  = mod;
  af2.location = APPLY_HITROLL;
  af3.location = APPLY_DAMROLL;
  af.modifier = 0;
  af.bitvector = af2.bitvector = af3.bitvector = 0;
  af.bitvector2 = AFF2_NOFLEE | AFF2_RAGE;
  af2.bitvector2 = af3.bitvector2 = AFF2_RAGE;
  result = generic_result(ch,SKILL_RAGE,NULL,FALSE);

  if ((TOTAL_FUCKUP == result)||(TOTAL_FAILURE == result)) {
    send_to_char("You can only think of fluffy bunnies.\r\n",ch);
    return;
  }
  send_to_char("\\c07You think of infidels who dare defile the name of the \\c08Dragon\\c07 and a powerful rage builds up inside you.\r\n",ch);
  act("$n grits $s teeth and $s eyes glint dangerously.",FALSE,ch,0,0,TO_ROOM);
  affect_to_char(ch,&af);
  affect_to_char(ch,&af2);
  affect_to_char(ch,&af3);
}

ACMD(do_call)
{
  #define AIEL_FORCE 7027
  struct char_data *reinforc[30];
  struct affected_type af,af2;
  int i,num,result,mod;
  char tmp[500];

  af = clear_aff_var(af);
  af = clear_aff_var(af2);

  if (!can_use_gskill(ch, SKILL_CALL)) {
    send_to_char("You? Getting backup from Aiel!? Don't think so!\r\n",ch);
    return;
  }
  if (!GET_SKILL(ch,SKILL_CALL)) {
    send_to_char("Sorry, you don't have a clue on how.\r\n",ch);
    return;
  }
  
  if (IS_AFFECTED(ch,AFF_NOREINFORCE)) {
    send_to_char("Sorry, you can't use call again for a while.\r\n",ch);
    return;
  }

  af.duration = af2.duration = 15;
  af.duration = af2.duration = 15;
  af.type = af2.type = SKILL_CALL;
  af.bitvector = af2.bitvector = AFF_NOREINFORCE;
  af.bitvector |= AFF_CHARM;
  af.bitvector2 = af2.bitvector2 = 0;
  af.location = af2.location = 0;
  af.modifier = af2.modifier = 0;
   
  num = (GET_LEVEL(ch) / 30);
  if(num>3)
     num=3;
  
  
  mod = (GET_CHA(ch) - 15);
  result =  generic_result_mod(ch,SKILL_CALL,NULL,FALSE,mod);

  if ((result = TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
    affect_to_char(ch,&af2);
    send_to_char("You try to rally your clan, but to no use.\r\n",ch);
    act("$n shouts, \"For the Car'a'carn!\", but nothing happens.",FALSE,ch,0,0,TO_ROOM);
    return;
  }
  else {
    affect_to_char(ch,&af2);
   for (i = 0; i < num; i++) {
      reinforc[i] = read_mobile(AIEL_FORCE,VIRTUAL);
      affect_to_char(reinforc[i],&af);
      char_to_room(reinforc[i],ch->in_room);
      add_follower(reinforc[i],ch);
    }
    sprintf(tmp,"You shout for backup, and %d warriors arrives to your help.\r\n",num);
    send_to_char(tmp,ch);
    sprintf(tmp,"$n shouts, \"For the Car'a'carn!\", and %d Aiel comes to $s help!",num);
    act(tmp,FALSE,ch,0,0,TO_ROOM);
  }
}

ACMD(do_bargain)
{
  struct affected_type af, af2;
  int cha=0;

  af = clear_aff_var(af);
  af2 = clear_aff_var(af2);

  if (!GET_SKILL(ch, SKILL_BARGAIN)) {
    send_to_char("You don't know how!\r\n", ch);
    return;
  }
  if (!can_use_gskill(ch, SKILL_BARGAIN)) {
    send_to_char("I don't think so.\r\n", ch);
    return;
  }
  if (IS_AFFECTED2(ch,AFF2_NOBARGAIN)) {
    send_to_char("Sorry, you can't use Bargain again for a while.\r\n",ch);
    return;
  }

  switch(GET_SKILL(ch, SKILL_BARGAIN)) {
    case 70:
      cha=5;
      break;
    case 80:
      cha=10;
      break;
    case 90:
      cha=15;
      break;
    case 100:
      cha=20;
      break;
    default:
      send_to_char("You're not set with a proper value of the skill.  Tell your GL.\r\n", ch);
      return;
  }

  af.type = SKILL_BARGAIN;
  af.duration = 2;
  af.modifier   = cha;
  af.location   = APPLY_CHA;
  af.bitvector2 = AFF2_BARGAIN;

  af2.type = SKILL_BARGAIN;
  af2.duration = 20;
  af2.location = APPLY_NONE;
  af2.modifier = 0;
  af2.bitvector2 = AFF2_NOBARGAIN;
 
  affect_to_char(ch, &af);
  affect_to_char(ch, &af2);

  send_to_char("You use your Seafolk prowess and get ready to Bargain.\r\n", ch);
}

ACMD(do_charm)
{
  struct affected_type af, af2;
  int cha=0;

  af = clear_aff_var(af);
  af2 = clear_aff_var(af2);

  if (!GET_SKILL(ch, SKILL_CHARM)) {
    send_to_char("You don't know how!\r\n", ch);
    return;
  }
  if (!can_use_gskill(ch, SKILL_CHARM)) {
    send_to_char("I don't think so.\r\n", ch);
    return;
  }
  if (IS_AFFECTED2(ch,AFF2_NOCHARM)) {
    send_to_char("Sorry, you can't use Charm again for a while.\r\n",ch);
    return;
  }

  switch(GET_SKILL(ch, SKILL_CHARM)) {
    case 70:
      cha=2;
      break;
    case 80:
      cha=5;
      break;
    case 90:
      cha=7;
      break;
    case 100:
      cha=10;
      break;
    default:
      send_to_char("You're not set with a proper value of the skill.  Tell your GL.\r\n", ch);
      return;
  }

  af.type = SKILL_CHARM;
  af.duration = 2;
  af.modifier   = cha;
  af.location   = APPLY_CHA;
  af.bitvector2 = AFF2_CHARM;

  af2.type = SKILL_CHARM;
  af2.duration = 20;
  af2.location = APPLY_NONE;
  af2.modifier = 0;
  af2.bitvector2 = AFF2_NOCHARM;
 
  affect_to_char(ch, &af);
  affect_to_char(ch, &af2);

  send_to_char("You use your natural charm and get ready to deal.\r\n", ch);
}

ACMD(do_saltbrothers)
{
  #define SF_FORCE 1100
  struct char_data *reinforc[30];
  struct affected_type af,af2;
  int i,num,result,mod;
  char tmp[500];

  af = clear_aff_var(af);
  af = clear_aff_var(af2);

  if (!can_use_gskill(ch, SKILL_SALTBROTHERS)) {
    send_to_char("You? Getting backup from Seafolk!? Don't think so!\r\n",ch);
    return;
  }
  if (!GET_SKILL(ch,SKILL_SALTBROTHERS)) {
    send_to_char("Sorry, you don't have a clue on how.\r\n",ch);
    return;
  }
  
  if (IS_AFFECTED(ch,AFF_NOREINFORCE)) {
    send_to_char("Sorry, you can't use saltbrothers again for a while.\r\n",ch);
    return;
  }

  af.duration = af2.duration = 15;
  af.duration = af2.duration = 15;
  af.type = af2.type = SKILL_SALTBROTHERS;
  af.bitvector = af2.bitvector = AFF_NOREINFORCE;
  af.bitvector |= AFF_CHARM;
  af.bitvector2 = af2.bitvector2 = 0;
  af.location = af2.location = 0;
  af.modifier = af2.modifier = 0;
   
  num = (GET_LEVEL(ch) / 30);
  if(num>3)
     num=3;
  if(num<1)
     num=1;  
  
  mod = (GET_CHA(ch) - 15);
  result =  generic_result_mod(ch,SKILL_SALTBROTHERS,NULL,FALSE,mod);

  if ((result = TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
    affect_to_char(ch,&af2);
    send_to_char("You try to rally your fellow Atha'an Miere, but to no use.\r\n",ch);
    act("$n shouts, \"For the Coramoor!\", but nothing happens.",FALSE,ch,0,0,TO_ROOM);
    return;
  }
  else {
    affect_to_char(ch,&af2);
   for (i = 0; i < num; i++) {
      reinforc[i] = read_mobile(SF_FORCE,VIRTUAL);
      affect_to_char(reinforc[i],&af);
      char_to_room(reinforc[i],ch->in_room);
      add_follower(reinforc[i],ch);
    }
    sprintf(tmp,"You shout for backup, and %d warriors arrives to your help.\r\n",num);
    send_to_char(tmp,ch);
    sprintf(tmp,"$n shouts, \"For the Coramoor!\", and %d Seafolk come to $s help!",num);
    act(tmp,FALSE,ch,0,0,TO_ROOM);
  }
}

ACMD(do_tract)
{
  int result;
  struct affected_type af;
 
  af = clear_aff_var(af);

  if (!GET_SKILL(ch,SKILL_TRACTION)) {
    send_to_char("Huh?!?\r\n",ch);
    return;
  }
  if (IS_AFFECTED2(ch,AFF2_TRACTION)) {
    send_to_char("You have to wait a while before using this skill again.\r\n",ch);
    return;
  }
  af.type       = SKILL_TRACTION;
  af.duration   = 15;
  af.modifier   = 0;
  af.location   = 0;
  af.bitvector  = 0;
  af.bitvector2 = AFF2_TRACTION;

  result = generic_result(ch,SKILL_TRACTION,NULL,FALSE);
  if ((TOTAL_FUCKUP == result)||(TOTAL_FAILURE == result)) {
    send_to_char("You fail!\r\n",ch);
    return;
  }
  send_to_char("Okay, you succeeded.\r\n",ch);
  affect_to_char(ch,&af);
}

ACMD(do_stalk)
{
 int result;
 struct affected_type af;

 af = clear_aff_var(af);
  if (!GET_SKILL(ch, SKILL_STALK))
  {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }
  if (!can_use_gskill(ch, SKILL_STALK)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }
  skip_spaces(&argument);

  if (!argument || !*argument) {
    if (!ch->master) {
      send_to_char("But you're not following anyone..?\r\n", ch);
      return;
    }
    send_to_char("Okay, you stop stalking your prey.\r\n",ch);
    stop_follower(ch);
    return;
  }
    
  result = generic_result(ch, SKILL_STALK, NULL,FALSE);
  if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
    send_to_char("You fail utterly.\r\n", ch);
    return;
  }
  send_to_char("Ok, you begin to stalk your prey.\r\n", ch);
  SET_BIT(AFF_FLAGS(ch), AFF_TAILING);
  do_follow(ch, argument, 0, 0);

  if (!IS_AFFECTED(ch, AFF_SNEAK))
  {
    af.type = SKILL_SNEAK;
    af.duration = MIN(GET_LEVEL(ch), 20);
    af.modifier = 0;
    af.location = APPLY_NONE;
    SET_BIT(af.bitvector, AFF_SNEAK);
    affect_to_char(ch, &af);
  }
}

ACMD(do_lipreading)
{
  int result;
  struct affected_type af;

  af = clear_aff_var(af);
  if (!GET_SKILL(ch, SKILL_LIPREAD))
  {
    send_to_char("Huh!?\r\n", ch);
    return;
  }
   if (!is_dark(ch) && (GET_LEVEL(ch) < LVL_IMMORT)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }
  if (IS_AFFECTED(ch, AFF_LIPREADING))
  {
    send_to_char("You're already utilizing that skill.\r\n", ch);
    return;
  }
  result = generic_result(ch,SKILL_LIPREAD,NULL,FALSE);
  if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE))
  {
    send_to_char("You can't seem to recall how to read lips right now.\r\n", ch);
    act("$n all of a sudden begins to stare at people's lips, frowning in concentration.",
      FALSE, ch, NULL, NULL, TO_ROOM);
    return;
  }
  af.duration = 7 ;
  af.modifier = 0;
  af.location = 0;
  SET_BIT(af.bitvector, AFF_LIPREADING);
  af.next = NULL;
  af.type = SKILL_LIPREAD;
  affect_to_char(ch, &af);
  send_to_char("You can now read the lips of everyone in your nearest surroundings.\r\n", ch);
}

ACMD(do_rumor)
{
  int result;
  struct affected_type af;

  af = clear_aff_var(af);

  if (!GET_SKILL(ch, SKILL_RUMOR))
  {
    send_to_char("Huh!?\r\n", ch);
    return;
  }

  if (!can_use_gskill(ch, SKILL_RUMOR)) 
    {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
    }

  if (IS_AFFECTED(ch, AFF_LIPREADING))
  {
    send_to_char("You're already utilizing that skill.\r\n", ch);
    return;
  }
  result = generic_result(ch,SKILL_RUMOR,NULL,FALSE);
  if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE))
  {
    send_to_char("You don't hear any rumors.\r\n",
ch);
    act("$n listens quietly to the conversations for a time, then frowns.",
      FALSE, ch, NULL, NULL, TO_ROOM);
    return;
  }
  af.duration = 7 ;
  af.modifier = 0;
  af.location = 0;
  SET_BIT(af.bitvector, AFF_LIPREADING);
  af.next = NULL;
  af.type = SKILL_RUMOR;
  affect_to_char(ch, &af);
  send_to_char("You can now read the lips of everyone in your nearest surroundings.\r\n", ch);
}

ACMD(do_camo)
{
  int result;

  if (!GET_SKILL(ch, SKILL_CAMOUFLAGE))
  {
    send_to_char("You'd better learn it before you try it.\r\n", ch);
    return;
  }
  if (!can_use_gskill(ch, SKILL_CAMOUFLAGE)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }
  if (IS_AFFECTED(ch, AFF_CAMOUFLAGED))
  {
    send_to_char("You are already camouflaged as it is...\r\n", ch);
    return;
  }
  send_to_char("You begin to look for things to camouflage yourself with...wait.\r\n", ch);
  WAIT_STATE(ch, PULSE_VIOLENCE* 3); /* Add a little delay for the attempt. */
  result = generic_result(ch, SKILL_CAMOUFLAGE, NULL, FALSE);
  if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE))
  {
    send_to_char("You fail. Maybe there's too little to hide behind in here?\r\n", ch);
    return;
  }
  send_to_char("Okay, you cunningly use what there is to camouflage yourself with.\r\n", ch);
  SET_BIT(AFF_FLAGS(ch), AFF_CAMOUFLAGED);
}

int ma_skills_ok(struct char_data *ch,int skill)
{
  if ((skill == SKILL_TRIP) || (skill == SKILL_DIRTKICK) || (skill == SKILL_PUNCH) || (skill == SKILL_KICK))
    return 1;
  if (skill == SKILL_BASH) {
    if (!GET_EQ(ch,WEAR_WIELD_R) || !GET_EQ(ch,WEAR_WIELD_L)) {
      return 0;
    }
    else {
      return 1;
    }
  }
  return 0;
}

ACMD(martial_arts)
{
  int i,found = 0,num;
  int skills[] = {
    SKILL_KICK,
    SKILL_PUNCH,
    SKILL_DIRTKICK,
    SKILL_TRIP,
    SKILL_BASH
  };

  int result = generic_result(ch,SKILL_MARTIAL_ARTS,NULL,FALSE);

  if (IS_NPC(ch))
    return;
  if (!FIGHTING(ch))
    return;

  for (i = 0; (i < 5) && !found; i++) {
    if (GET_SKILL(ch,skills[i]))
      found = 1;
  }

  if (!found)
    return;
  do {
    num = number(0,4);
  } while (ma_skills_ok(ch,skills[num]) && !GET_SKILL(ch,skills[num]));

  for (i = 0; i < MAX_CAN; i++)
    SET_TIMES(ch,i, 0);

  if (FIGHTING(ch) && ((result == SOME_SUCCESS)||(result==TOTAL_SUCCESS))) {
    if (skills[num] == SKILL_KICK)
      do_kick(ch,"\0",0,0);
    else if (skills[num] == SKILL_PUNCH)
      do_punch(ch,"\0",0,0);
    else if (skills[num] == SKILL_DIRTKICK)
      do_dirtkick(ch,"\0",0,0);
    else if (skills[num] == SKILL_TRIP)
      do_trip(ch,"\0",0,0);
    else if (skills[num] == SKILL_BASH)
      do_bash(ch,"\0",0,0);

    if (GET_POS(ch) == POS_SITTING) {
      GET_POS(ch) = POS_STANDING;
      send_to_char("You stumble to your feet\r\n",ch);
      act("$n scrambles to $s feet.",FALSE,ch,NULL,ch,TO_NOTVICT);
    }
  }
}

void print_link(struct char_data *ch){

  struct char_data *f;
  struct char_data *master;
  int i = 0;
    
  /* Determine who the master of the link is */
  master = (ch->player_specials->linked_by ? ch->player_specials->linked_by : ch);
    
  /* send the character the master data */
   send_to_char("You are currently linked to:\r\n", ch);
  sprintf(buf2, "   [%5ld]M   [%3ld]%%     %s\r\n", GET_MANA(master),
    master->player_specials->managiven/master->player_specials->max_manalinked, GET_NAME(master));
  act(buf2, FALSE, ch, 0, master, TO_CHAR);
                    
    /* cycle through all the linked players */	
  while(master->player_specials->linking[i]){
    f = master->player_specials->linking[i];
    sprintf(buf2, "              [%3ld]%%     %s\r\n", (100*(f)->player_specials->managiven)/master->player_specials->max_manalinked, GET_NAME(f));
    act(buf2, FALSE, ch, 0, f, TO_CHAR);
    ++i;
  }
}  /* end of print_link */

ACMD(do_link)
{
  struct char_data *vict = NULL;
  one_argument(argument, arg);

  if (IS_NPC(ch)){
    send_to_char("You just don't seem to know how to link.  You're too stupid to figure it out!\r\n", ch);
    return;
  }
  if (cant_channel(ch)){
    send_to_char("How do you propose to link someone when you can't even channel, moron!\r\n", ch);
    return;
  }
    
  if(!(*arg)){
    if((!(ch->player_specials->linked_by)) && (!(ch->player_specials->linking[0]))){
      send_to_char("Syntax:  link {dissolve | <target>}\r\n", ch);
      return;
    }
    else{
      print_link(ch);
      return;
    }
  }

  if(!(strncmp("dissolve", arg, strlen(arg)))){
    if (ch->player_specials->linked_by){
      send_to_char("You do not control this link!\r\n", ch);
      return;
    }
    if(!(ch->player_specials->linking[0])){
      send_to_char("You are not linked to anyone!\r\n", ch);
      return;
    }	
    perform_unlink(ch);
    return;
  }

  if(!(vict = get_char_room_vis(ch, arg))){
    send_to_char("Link whom??\r\n", ch);
    return;
  }
  if(IS_NPC(vict)){
    send_to_char("You can't link a mob, sorry\r\n", ch);
    return;
  }
  if(cant_channel(vict)){
    act("You can't seem to link $N.\r\n", FALSE, ch, 0, vict, TO_CHAR);
    return;
  }
  else{
    add_to_link(ch, vict);
    return;
  }	
}

/* end of do_link */

ACMD(do_brand)
{
  struct char_data *victim;

  if (!GET_SKILL(ch, SKILL_BRAND)) {
    send_to_char("You don't know how. \r\n", ch);
    return;
  }
  if (!can_use_gskill(ch, SKILL_BRAND)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }
  one_argument(argument, arg);
  if (!*arg) {
    send_to_char("You have to give a name too.\r\n", ch);
    return;
  }
  if (!(victim = get_char_room_vis(ch, arg))){
    send_to_char("Brand who as a darkfriend?\r\n", ch);
    return;
  }
  if (IS_NPC(victim)) {
    send_to_char("Sorry, but you can only brand players as darkfriends.\r\n", ch);
    return;
  }

  if (IS_BRANDED(victim)) {
      if (!IS_IMMORTAL(ch)) {
      send_to_char("That foul cur is already branded as a darkfriend.\r\n", ch);
      return;
    }
    else {
      act("Ok, you un-brand $N.", 0, ch, 0, victim, TO_CHAR);
      act("$n has just unbranded you! Health, wealth and happiness!", 0, ch, 0, victim, TO_VICT);
      IS_BRANDED(victim) = 0;
      return;
    } 
  }
  IS_BRANDED(victim) = 300;
  sprintf(buf,"%s picks up a red-hot poker and brands you as a darkfriend!!!\r\n", GET_NAME(ch));
  send_to_char(buf, victim);
  sprintf(buf, "You pick up a red-hot poker and brand the lowlife %s as a darkfriend.\r\n", GET_NAME(victim));
  send_to_char(buf, ch);
  act("$n picks up a red-hot poker and brands $N as a darkfriend!", TRUE, ch, 0, victim, TO_NOTVICT);
}

ACMD(do_summon)
{
  struct char_data *reinforc[30];
  struct affected_type af,af2;
  int i,num,result,mod;
  char tmp[500];

  af = clear_aff_var(af);
  af = clear_aff_var(af2);

  if (!is_dark(ch) && (GET_LEVEL(ch) < LVL_IMMORT)) {
    send_to_char("You? Reinforce!? Don't think so!\r\n",ch);
    return;
  }
 if (!GET_SKILL(ch,SKILL_SUMMON)) {
    send_to_char("Sorry, you don't have a clue on how.\r\n",ch);
    return;
  }
  
  if (IS_AFFECTED(ch,AFF_NOREINFORCE)) {
    send_to_char("Sorry, you can't reinforce again for a while.\r\n",ch);
    return;
  }
  
  af.duration = af2.duration = 15;
  af.duration = af2.duration = 15;
  af.type = af2.type = SKILL_SUMMON;
  af.bitvector = af2.bitvector = AFF_NOREINFORCE;
  af.bitvector |= AFF_CHARM;
  af.bitvector2 = af2.bitvector2 = 0;
  af.location = af2.location = 0;
  af.modifier = af2.modifier = 0; 

  num = (GET_LEVEL(ch) / 30);
  mod = (GET_CHA(ch) - 15);
  result =  generic_result_mod(ch,SKILL_SUMMON,NULL,FALSE,mod);

  if ((result = TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
    affect_to_char(ch,&af2);    
    send_to_char("The Great Lord refuses to hear yoo!\r\n",ch);
    act("$n mumbles a short prayer to the Dark One, but nothing happens.",FALSE,ch,0,0,TO_ROOM);
    return;
  }
  else {
    affect_to_char(ch,&af2);
   for (i = 0; i < num; i++) {
      reinforc[i] = read_mobile(6020,VIRTUAL);
      affect_to_char(reinforc[i],&af);
      char_to_room(reinforc[i],ch->in_room);
      add_follower(reinforc[i],ch);
    }
    sprintf(tmp,"The Great Lord hears your prayers, and gives you %d trollocs.\r\n",num);
    send_to_char(tmp,ch);   
    sprintf(tmp,"$n mumbles a prayer to the Dark One and %d trolocs comes to $s aid.",num);
    act(tmp,FALSE,ch,0,0,TO_ROOM);
  }
}

ACMD(do_reinforce)
{
  if (!can_use_gskill(ch, SKILL_REINFORCE)) {
    send_to_char("You? Reinforce!? Don't think so!\r\n",ch);
    return;
  }
  send_to_char("Reinforce mob call skill has been replaced with retainers.\r\n", ch);
  send_to_char("Go to the retainer shop and buy them, they are for free!\r\n", ch);
}

ACMD(do_patrol)
{
  struct char_data *reinforc[30];
  struct affected_type af,af2;   
  int i,num,result,mod;
  char tmp[500];
  
  af = clear_aff_var(af);  
  af = clear_aff_var(af2);
  af2=af;
  if (!can_use_gskill(ch, SKILL_PATROL)) {
    send_to_char("You don't have the authority to call for guardsmen!\r\n",ch); 
    return;
  }
    
  if (!GET_SKILL(ch,SKILL_PATROL)) {
    send_to_char("If only you could.\r\n",ch);
    return;
  }
  
  if (IS_AFFECTED2(ch,AFF2_NOPATROL)) {
    send_to_char("Unfortunately, you can't call for new guards yet.\r\n",ch);
    return;
  }
      
  af.duration = af2.duration = 15;
  af.bitvector = AFF_CHARM;
  af.type = af2.type = SKILL_PATROL;
  af2.bitvector = 0;
  af.bitvector2 = af2.bitvector2 = AFF2_NOPATROL;
  af.location = af2.location = 0;
  af.modifier = af2.modifier = 0;

  num = (GET_LEVEL(ch) / 40);
  mod = (GET_CHA(ch) - 15);
  result =  generic_result_mod(ch,SKILL_PATROL,NULL,FALSE,mod);
  
  if ((result = TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
    affect_to_char(ch,&af2);
    send_to_char("You call, but there are no guardsmen nearby!\r\n",ch);
    act("$n calls for backup, but nobody arrives",FALSE,ch,0,0,TO_ROOM);
    return;
  }
  else {
    affect_to_char(ch,&af2);
   
    for (i = 0; i < num; i++) {
      reinforc[i] = read_mobile(29810,VIRTUAL);
      affect_to_char(reinforc[i],&af);
      char_to_room(reinforc[i],ch->in_room);
      add_follower(reinforc[i],ch);
    }
    sprintf(tmp,"Ah, %d new guardsmen come to back you up.\r\n",num);
    send_to_char(tmp,ch);
    sprintf(tmp,"As $n hollers for reinforcements, %d new soldiers comes to $s help.",num);
    act(tmp,FALSE,ch,0,0,TO_ROOM);
  }
}



ACMD(do_recruit)
{
  struct affected_type af;
  struct char_data *mob;
  int temp;

  if (!GET_SKILL(ch, SKILL_RECRUIT)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }
  if (!can_use_gskill(ch, SKILL_RECRUIT)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }
  if (RECRUIT_WAIT(ch)) {
    send_to_char("You can't do that yet. Wait a little. \r\n", ch);
    return;
  }
  send_to_char("Ok, you recruit a soldier. \r\n", ch);
  act("$n recruits a fellow to follow $m.", TRUE, ch, 0, 0, TO_ROOM);
  if (!(mob = read_mobile(9600, VIRTUAL))) {
    send_to_char("This guildskill has fucked up. Tell an Imm.\r\n", ch);
    return;
  }
  char_to_room(mob, ch->in_room);
  GET_TIMER(mob) = MAX(1, GET_LEVEL(ch)/15);
  RECRUIT_WAIT(ch) = MAX(1, (GET_LEVEL(ch)*2)/15);
  temp = GET_LEVEL(ch);
  GET_LEVEL(mob) = temp;
  GET_HIT(mob) = GET_MAX_HIT(mob) = GET_LEVEL(mob) * 20;
  GET_REAL_AC(mob) = 100 - GET_LEVEL(mob);
  GET_HITROLL(mob) = GET_LEVEL(mob) / 5;
  GET_DAMROLL(mob) = GET_LEVEL(mob) / 20;
  mob->mob_specials.damnodice = 2;
  mob->mob_specials.damsizedice = MAX(1, GET_LEVEL(mob)/3);
  af = clear_aff_var(af);
  af.duration = 100;
  af.bitvector = AFF_CHARM;
  af.type = SKILL_RECRUIT;
  af.location = 0;
  af.modifier = 0;
  affect_to_char(mob, &af);
  add_follower(mob, ch);
}

ACMD(do_morale)
{
  struct affected_type af,af2,af3;
  struct char_data *k;
  struct follow_type *f;
  int result,mod,attacks=0,hit=0,dam=0;

  af = clear_aff_var(af);
  af2 = clear_aff_var(af2);
  af3 = clear_aff_var(af3);

  if (!can_use_gskill(ch, SKILL_MORALE)) {
    send_to_char(" You don't have a clue on how!\r\n",ch);
    return;
  }
  if (!GET_SKILL(ch,SKILL_MORALE)) {
    send_to_char("You know about as much of this as a block of stone!\r\n",ch);
    return;
  }
  if (IS_AFFECTED(ch,AFF_MORALE)) {
    send_to_char("Your morale is as high as it can get!\r\n",ch);
    return;
  }
  if (!IS_AFFECTED(ch,AFF_GROUP)) {
    send_to_char("You don't even have a group to do that with!\r\n",ch);
    return;
  }
  mod = (GET_CHA(ch) - 15);
  result = generic_result_mod(ch,SKILL_MORALE,NULL,FALSE,mod);
  af.duration = 20;
  af.modifier = 0;
  af.location = 0;
  af.type     = SKILL_MORALE;
  af.bitvector= AFF_MORALE;

  if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
    affect_to_char(ch,&af);
    send_to_char("Despite your nice speech, you can't raise the spirit among your men.\r\n",ch);
    act("$n tries to raise the spirit among his men, but in vain.",FALSE,ch,0,0,TO_ROOM);
    return;
  }

  k = (ch->master ? ch->master : ch);

  for (f = k->followers; f != NULL; f = f->next) {
    if (!IS_AFFECTED(f->follower,AFF_GROUP))
      continue;
    if (!get_guild(f->follower, 1))
      continue;

    /*
     * This needs re-coding! previously based on guildranks, but sicne guild_pos 
     * macros is no longer....
     */
    hit += 3;
    dam += 3;
  }
  if (get_guild(k, 1)) {
    hit += 3;
    dam += 3;
  }

  attacks = MIN(attacks,6);	/* Max 6 attacks! */
  hit     = MIN(hit,30);
  dam     = MIN(dam,30);	/* Hit n' dam max at 30! */

  af.modifier = attacks;
  af.location = APPLY_XATTACKS;
  af2.modifier = hit;
  af2.location = APPLY_HITROLL;
  af3.modifier = dam;
  af3.location = APPLY_DAMROLL;
  af2.duration = af3.duration = af.duration;
  af2.type = af3.type = af.type;
  af2.bitvector = af3.bitvector = af.bitvector;
  SET_BIT(af2.bitvector, AFF_MORALE);
  SET_BIT(af3.bitvector, AFF_MORALE);

  affect_to_char(k,&af);
  affect_to_char(k,&af2);
  affect_to_char(k,&af3);

  for (f = k->followers; f != NULL; f = f->next) {
    if (!IS_AFFECTED(f->follower,AFF_GROUP))
      continue;
    if (!get_guild(f->follower, 1))
      continue;
    if (!IS_AFFECTED(f->follower,AFF_MORALE)) {
      affect_to_char(f->follower,&af);
      affect_to_char(f->follower,&af2);
      affect_to_char(f->follower,&af3);
    }
  }
  send_to_char("You raise the spirit among your men. For the Light!\r\n",ch);
  act("$n raises the spirit among $s men, beware!",FALSE,ch,0,0,TO_ROOM);
}

ACMD(do_employ)
{
  struct affected_type af;
  struct char_data *mob;
  int temp,num;

  num = *arg;

  if (!GET_SKILL(ch, SKILL_EMPLOY)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }
  if (RECRUIT_WAIT(ch)) {
    send_to_char("You can't do that yet. Wait a little. \r\n", ch);
    return;
  }
  switch (num){
   case 1:{
    send_to_char("Ok, you hire a rough mercenary. \r\n", ch);
    act("$n hires a fellow to follow $m.", TRUE, ch, 0, 0, TO_ROOM);
    GET_GOLD(ch) -= 5000;
    mob = read_mobile(30, VIRTUAL);
    char_to_room(mob, ch->in_room);
    GET_TIMER(mob) = MAX(1, GET_LEVEL(ch)/15);
    RECRUIT_WAIT(ch) = MAX(1, (GET_LEVEL(ch)*2)/15);
    af.duration = 100;
    af.bitvector = AFF_CHARM;
    af.type = SKILL_EMPLOY;
    af.location = 0;
    af.modifier = 0;
    affect_to_char(mob, &af);
    add_follower(mob, ch);
    break;
   }
   case 2:{
    for (temp = 0; temp < 2; temp++){
     send_to_char("Ok, you hire a rough mercenary. \r\n", ch);
     act("$n hires a fellow to follow $m.", TRUE, ch, 0, 0, TO_ROOM);
     GET_GOLD(ch) -= 6000;
     mob = read_mobile(30, VIRTUAL);
     char_to_room(mob, ch->in_room);
     GET_TIMER(mob) = MAX(1, GET_LEVEL(ch)/15);
     RECRUIT_WAIT(ch) = MAX(1, (GET_LEVEL(ch)*2)/15);
     af.duration = 100;
     af.bitvector = AFF_CHARM;
     af.type = SKILL_EMPLOY;
     af.location = 0;
     af.modifier = 0;
     affect_to_char(mob, &af);
     add_follower(mob, ch);
     break;
    }
   }
   case 3:{
    for (temp = 0; temp < 3; temp++){
     send_to_char("Ok, you hire a rough mercenary. \r\n", ch);
     act("$n hires a fellow to follow $m.", TRUE, ch, 0, 0, TO_ROOM);
     GET_GOLD(ch) -= 7000;
     mob = read_mobile(30, VIRTUAL);
     char_to_room(mob, ch->in_room);
     GET_TIMER(mob) = MAX(1, GET_LEVEL(ch)/15);
     RECRUIT_WAIT(ch) = MAX(1, (GET_LEVEL(ch)*2)/15);
     af.duration = 100;
     af.bitvector = AFF_CHARM;
     af.type = SKILL_EMPLOY;
     af.location = 0;
     af.modifier = 0;
     affect_to_char(mob, &af);
     add_follower(mob, ch);
     break;
    }
   }
   case 4:{
    for (temp = 0; temp < 4; temp++){
     send_to_char("Ok, you hire a rough mercenary. \r\n", ch);
     act("$n hires a fellow to follow $m.", TRUE, ch, 0, 0, TO_ROOM);
     GET_GOLD(ch) -= 8000;
     mob = read_mobile(30, VIRTUAL);
     char_to_room(mob, ch->in_room);
     GET_TIMER(mob) = MAX(1, GET_LEVEL(ch)/15);
     RECRUIT_WAIT(ch) = MAX(1, (GET_LEVEL(ch)*2)/15);
     af.duration = 100;
     af.bitvector = AFF_CHARM;
     af.type = SKILL_EMPLOY;
     af.location = 0;
     af.modifier = 0;
     affect_to_char(mob, &af);
     add_follower(mob, ch);
     break;
    }
   }
   case 5:{
    for (temp = 0; temp < 5; temp++){
     send_to_char("Ok, you hire a rough mercenary. \r\n", ch);
     act("$n hires a fellow to follow $m.", TRUE, ch, 0, 0, TO_ROOM);
     GET_GOLD(ch) -= 9000;
     mob = read_mobile(30, VIRTUAL);
     char_to_room(mob, ch->in_room);
     GET_TIMER(mob) = MAX(1, GET_LEVEL(ch)/15);
     RECRUIT_WAIT(ch) = MAX(1, (GET_LEVEL(ch)*2)/15);
     af.duration = 100;
     af.bitvector = AFF_CHARM;
     af.type = SKILL_EMPLOY;
     af.location = 0;
     af.modifier = 0;
     affect_to_char(mob, &af);
     add_follower(mob, ch);
     break;
    }
   }
   default:
    send_to_char("Try that again\r\n", ch);
  }
}

ACMD(do_arrest)
{
  struct char_data *victim;
  struct affected_type af;
  if (!GET_SKILL(ch, SKILL_ARREST)) {
    send_to_char("You don't have that authority to do that.\r\n", ch);
    return;
  }
  if (!can_use_gskill(ch, SKILL_ARREST)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }
  one_argument(argument, arg);

  if (!(victim = get_char_room_vis(ch, arg))) {
    send_to_char("Arrest who?\r\n", ch);
    return;
  }

  if (IS_AFFECTED2(victim, AFF2_ARRESTED)) {
    affect_from_char(victim, SKILL_ARREST);
    REMOVE_BIT(AFF_FLAGS(victim), AFF2_ARRESTED);
    send_to_char("You release your prisoner from custody.\r\n", ch);
    act("You watch as $n releases his prisoner from custody.",FALSE,ch,NULL,NULL,TO_ROOM);
    act("You have been released by $n!", TRUE, ch, 0, victim, TO_VICT);
    do_follow(victim,GET_NAME(victim),0,0);
    return;
  }

  if (GET_LEVEL(victim) >= LVL_IMMORT) {
    send_to_char("Nice try...Why not go arrest some mortals instead?\r\n", ch);
    return;
  }

 if ( IS_NPC(victim) ) {
    send_to_char("Oh come on.. it's just an innocent mob, you don't want to do that!\r\n", ch);
    return;
  }

  act("$n arrests $N!\r\n", TRUE, ch, 0, victim, TO_NOTVICT);
  act("You quickly arrest $N and reads $M $S rights.\r\n", TRUE, ch, 0, victim, TO_CHAR);
  act("You have been arrested by $n!", TRUE, ch, 0, victim, TO_VICT);

  af = clear_aff_var(af);

  af.type = SKILL_ARREST;
  af.duration = 3 + GET_LEVEL(ch)/20;
  af.bitvector2 = AFF2_ARRESTED;

  affect_to_char(victim,&af);
  do_follow(victim,GET_NAME(ch), 0, 0);
}

/* The kin guild skills */

ACMD(do_findherb)
{
  int herbrnums[100], amt = 0, i = 0;
  struct obj_data *herb = NULL;
  struct affected_type af;

  if (!GET_SKILL(ch, SKILL_FINDHERB)) {
    send_to_char("You didn't find any herbs this time either...\r\n", ch);
    send_to_char("Could it be because you're a complete incompetent at this?\r\n", ch);
    return;
  }
  if (IS_AFFECTED2(ch, AFF2_NOFINDHERB)) {
    send_to_char("You have to wait until you can use this skill again.\r\n", ch);
    return;
  }

  if ((SECT(ch->in_room) < SECT_FIELD) || (SECT(ch->in_room) > SECT_MOUNTAIN)) {
    send_to_char("There are no herbs to be found here.\r\n", ch);
    return;
  }

  act("$n looks around on the ground for something.", TRUE, ch, 0, 0, TO_NOTCHAR);

  if (generic_result_mod(ch,SKILL_FINDHERB,NULL,FALSE,GET_WIS(ch) - 25) < SOME_SUCCESS) {
    send_to_char("You find no herbs right now.\r\n", ch);
    return;
  }
  for (i = 0; (i <= top_of_objt) && (amt < 100); i ++)
    if (obj_proto[i].obj_flags.type_flag == ITEM_HERB) {
      herbrnums[amt] = i;
      amt ++;
    }

  if (!amt) {
    send_to_char("There seems to be no herbs in the entire world right now...\r\n", ch);
    return;
  }
  herb = read_object(herbrnums[number(0, amt - 1)], REAL);
  if (!herb) {
    send_to_char("You seem unable to find any herbs!\r\n",ch);
    return;
  }
  sprintf(buf,"$n finds %s in the terrain.\r\n", herb->short_description);
  act(buf, TRUE, ch, 0, 0, TO_NOTCHAR);
  sprintf(buf,"You find %s in the terrain.\r\n", herb->short_description);
  send_to_char(buf, ch);
  obj_to_char(herb, ch);

  af = clear_aff_var(af);
  af.type = SKILL_FINDHERB;
  af.location = 0;
  af.modifier = 0;
  af.duration = 4;
  af.bitvector2 = AFF2_NOFINDHERB;
  affect_to_char(ch, &af);
}

ACMD(do_herbident)
{
  struct obj_data *herb = NULL;
/*  struct affected_type af; */
  char arg1[MAX_INPUT_LENGTH];

  one_argument(argument, arg1);

  if (!*arg1) {
    send_to_char("Identify which herb ? Use 'herbident <herb>'.\r\n", ch);
    return;
  }

  if (!(herb = get_obj_in_list_vis(ch, arg1, ch->carrying))) {
    sprintf(buf, "You don't seem to have %s %s.\r\n", AN(arg1), arg1);
    send_to_char(buf, ch);
    return;
  }

  if (GET_OBJ_TYPE(herb) != ITEM_HERB) {
    send_to_char("This is not an herb !\r\n", ch);
    return;
  }

  if (!GET_SKILL(ch, SKILL_HERBIDENT)) {
    send_to_char("Oh, come on ! Like you know how to do that !\r\n", ch);
    return;
  }
/*
  if (IS_AFFECTED2(ch, AFF2_NOHERBIDENT)) {
    send_to_char("You have to wait until you can use this skill again.\r\n", ch);
    return;
  } -- removed delay on herbident-- */

  if (generic_result_mod(ch,SKILL_HERBIDENT,NULL,FALSE,GET_WIS(ch) - 25) < SOME_SUCCESS) {
    send_to_char("You try to identify this herb, but fail.\r\n", ch);
/*    af = clear_aff_var(af);
    af.type = SKILL_HERBIDENT;
    af.location = 0;
    af.modifier = 0;
    af.duration = 3;
    af.bitvector2 = AFF2_NOHERBIDENT;
    affect_to_char(ch, &af); */
    return;
  }

  if (!GET_OBJ_VAL(herb, 0)) {
    send_to_char("This herb has no special affect.\r\n", ch);
    return;
  }
  
  sprintf(buf, "This is a %s herb that %s %ld ", 
      (GET_OBJ_VAL(herb, 0) == 1 ? "&rharming&n" : "&Whealing&n"),
      (GET_OBJ_VAL(herb, 0) == 1 ? "harms" : "heals"),
      GET_OBJ_VAL(herb, 1));

  switch (GET_OBJ_VAL(herb, 2)) {
    case 1:
      sprintf(buf, "%s%s points.\r\n",buf, "hit");
    break;
    case 2:
      sprintf(buf, "%s%s points.\r\n",buf, "mana");
    break;
    case 3:
      sprintf(buf, "%s%s points.\r\n",buf, "movement");
    break;
  }
  
  send_to_char(buf, ch);
/*
  af = clear_aff_var(af);
  af.type = SKILL_HERBIDENT;
  af.location = 0;
  af.modifier = 0;
  af.duration = 2;
  af.bitvector2 = AFF2_NOHERBIDENT;
  affect_to_char(ch, &af);
*/
  return;
}

ACMD(do_treesing)
{
  struct obj_data *obj = NULL;
  byte found = FALSE,found2 = 0;
  int i;
  skip_spaces(&argument);

  if (!GET_SKILL(ch, SKILL_TREESINGING)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }

  if (!can_use_gskill(ch, SKILL_TREESINGING)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }

  if (!get_guild(ch,13) && (GET_LEVEL(ch) < LVL_IMMORT)){
    send_to_char("Go away, you're not an Ogier!\r\n",ch);
    return;
  }

  if (!*argument) {
    send_to_char("What item do you want to sing?\r\n", ch);
    return;
  }

  for (obj = ch->carrying; obj && !found ; obj = obj->next_content)
    if (isname(argument, obj->name)) {
      found = TRUE;
      break;
    }
  if (!found || !obj) {
    send_to_char("You don't seem to be carrying such an object.\r\n", ch);
    return;
  }

  if (IS_SET(OBJ_EXTRA_FLAGS(obj),ITEM_SUNG)) {
    sprintf(buf, "%s is already sung to.\r\n", obj->short_description);
    send_to_char(buf, ch);
    return;
  }

  if (CAN_WEAR(obj,ITEM_WEAR_EAR) || CAN_WEAR(obj,ITEM_WEAR_NOSE)) {
    asend_to_char(ch,"Ear/Nose EQ can not be sung!\r\n");
    return;
  }

  if ((GET_OBJ_TYPE(obj) != ITEM_ARROW) && (GET_OBJ_TYPE(obj) != ITEM_WEAPON) &&
  (GET_OBJ_TYPE(obj) != ITEM_ARMOR)) {
    act("$p cannot be sung, wrong type.", FALSE, ch, obj, 0, TO_CHAR);
    return;
  }
  found = FALSE;
  act("$n sings a strange, melodious song to $p.", TRUE, ch, obj, 0, TO_ROOM);
  send_to_char("You begin to sing at your item.\r\n", ch);

  if (GET_SKILL(ch, SKILL_TREESINGING) > number(0, 100)) {
    send_to_char("You smile in satisfaction as the item responds.\r\n", ch);
    sprintf(buf, "%s (sung)", obj->short_description);
    SET_BIT(OBJ_EXTRA_FLAGS(obj),ITEM_NOBREAK);
    obj->short_description = strdup(buf);
    switch (GET_OBJ_TYPE(obj)) {
      case ITEM_ARROW:
      case ITEM_WEAPON:
        GET_OBJ_VAL(obj, 0) += 8;
    found = found2 = 0;
        for (i = 0; ((i < MAX_OBJ_AFFECT) && (!found || !found2)); i++) {
          if ((obj->affected[i].location == APPLY_HITROLL) && !found) {
             obj->affected[i].modifier += 8;
             found = 1;
          }
          if ((APPLY_DAMROLL == obj->affected[i].location) && !found2) {
         obj->affected[i].modifier += 8;
         found2 = 1;
      }
        }
        if (!found) {
          i = 0;
          while (obj->affected[i].location && (i < MAX_OBJ_AFFECT))
            i++;
          if (i < MAX_OBJ_AFFECT) {
            obj->affected[i].location = APPLY_HITROLL;
            obj->affected[i].modifier = 8;
          }
        }
    if (!found2) {
          i = 0;
      while((i < MAX_OBJ_AFFECT) && obj->affected[i].location) 
        i++;
          if (i < MAX_OBJ_AFFECT) {
        obj->affected[i].location = APPLY_DAMROLL;
        obj->affected[i].modifier = 8;
          }
        } 
      break;
      case ITEM_ARMOR:
        GET_OBJ_VAL(obj, 0) += 5;
        GET_OBJ_VAL(obj, 1) += 10;
        for (i = 0, found = 0;(i < MAX_OBJ_AFFECT) &&!found; i++) {
          if (obj->affected[i].location == APPLY_ABS) {
         found = 1;
         obj->affected[i].modifier += 10;
          }
        } 
    if (!found) {
      i = 0;
          while((i < MAX_OBJ_AFFECT) && obj->affected[i].location) 
        i++;
          if (i < MAX_OBJ_AFFECT) {
        obj->affected[i].location = APPLY_ABS;
        obj->affected[i].modifier = 10;
          }
        }
      break;
    }
  } else
    send_to_char("Oh, no! You made a mistake. This item can -never- be sung now.\r\n", ch);
  SET_BIT(OBJ_EXTRA_FLAGS(obj),ITEM_SUNG);
}

static void move_as_bond(struct char_data *from, struct char_data *to, struct char_data *whom)
{
  int temp1,temp2;

  temp1 = GET_BONDED(from);
  REMOVE_BIT(temp1,(1 << 30));
  temp2 = GET_BONDED(whom);
  REMOVE_BIT(temp2,(1 << 30));

  if (!GET_BONDED(from) || !GET_BONDED(whom) || (temp1 != temp2)) {
    act("*sigh* This command can't be completed, since the targets are either unbonded, or not bonded with each other!",0,to,0,0,TO_ROOM);
    return;
  }
  asend_to_char(whom,"%s passes your bond on to %s!\r\n",GET_NAME(from),GET_NAME(to));
  act("You pass your Warder on to $N!",0,from,0,to,TO_CHAR);
  act("$N passes her Warder on to you!",0,to,0,from,TO_CHAR);

  if (!GET_BONDED(to)) {
    top_bond_id++;
    GET_BONDED(to) = top_bond_id;
    SET_BIT(GET_BONDED(to),(1 << 30));
  }
  GET_BONDED(whom) = GET_BONDED(to);
  REMOVE_BIT(GET_BONDED(whom),(1 << 30));
  update_p_table(to);
  update_p_table(whom);
  // All done. 
}

static void move_asha_bond(struct char_data *from, struct char_data *to, struct char_data *whom)
{
 int temp1,temp2;

  temp1 = GET_ASHABONDED(from);
  REMOVE_BIT(temp1,(1 << 30));
  temp2 = GET_ASHABONDED(whom);
  REMOVE_BIT(temp2,(1 << 30));

  if (!GET_ASHABONDED(from) || !GET_ASHABONDED(whom) || (temp1 != temp2)) {
   act("*sigh* This command can't be completed, since the targets are either unbonded, or not bonded with each other!",0,to,0,0,TO_ROOM);
   return;
  }
  asend_to_char(whom,"%s passes your bond on to %s!\r\n",GET_NAME(from),GET_NAME(to));
  act("You pass your Warder on to $N!",0,from,0,to,TO_CHAR);
  act("$N passes her Warder on to you!",0,to,0,from,TO_CHAR);

  if (!GET_ASHABONDED(to)) {
    top_ashabond_id++;
    GET_ASHABONDED(to) = top_ashabond_id;
    SET_BIT(GET_ASHABONDED(to),(1 << 30));
  }
  GET_ASHABONDED(whom) = GET_ASHABONDED(to);
  REMOVE_BIT(GET_ASHABONDED(whom),(1 << 30));
  update_p_table(to);
  update_p_table(whom);
  // All done.
}

static void move_fs_bond(struct char_data *from, struct char_data *to, struct char_data *whom)
{
 int temp1,temp2;

  temp1 = GET_MINION(from);
  REMOVE_BIT(temp1,(1 << 30));
  temp2 = GET_MINION(whom);
  REMOVE_BIT(temp2,(1 << 30));

  if (!GET_MINION(from) || !GET_MINION(whom) || (temp1 != temp2)) {
    act("*sigh* This command can't be completed, since the targets are either unbonded, or not bonded with each other!",0,to,0,0,TO_ROOM);
    return;
  }
  asend_to_char(whom,"%s passes your bond on to %s!\r\n",GET_NAME(from),GET_NAME(to));
  act("You pass your Warder on to $N!",0,from,0,to,TO_CHAR);
  act("$N passes her Warder on to you!",0,to,0,from,TO_CHAR);

  if (!GET_MINION(to)) {
    top_minion_id++;
    GET_MINION(to) = top_minion_id;
    SET_BIT(GET_MINION(to),(1 << 30));
  }
  GET_MINION(whom) = GET_MINION(to);
  REMOVE_BIT(GET_MINION(whom),(1 << 30));
  update_p_table(to);
  update_p_table(whom);
  // All done.
}

ACMD(do_passbond) 
{
  struct char_data *to, *from, *whom; 
/*
 * from - the holder of the bond (Ex: Moiraine)
 * to   - the person to pass the bond to (Ex: Myrelle(sp?))
 * whom - Which bondee to pass on (Ex: Lan). 
 */
 char arg1[MAX_INPUT_LENGTH],*rest;

  skip_spaces(&argument);
  if (!IS_IMMORTAL(ch)) {
    send_to_char("Only Imms can pass Bonds at this time...\r\n",ch);
    return;
  }
  if (!argument || !*argument) {
    send_to_char("Usage: passbond <from> <to> <bondee>\r\n",ch);
    return;
  }
  rest = one_argument(argument,arg1);
  if (!(from = get_char_room_vis(ch,arg1))) {
    asend_to_char(ch,"The person %s is not here!\r\n",arg1);
    return;
  }
  rest = one_argument(rest,arg1);
  if (!(to = get_char_room_vis(ch,arg1))) {
    asend_to_char(ch,"The person %s is not here!\r\n",arg1);
    return;
  }
  rest = one_argument(rest,arg1);
  if (!(whom = get_char_room_vis(ch,arg1))) {
    asend_to_char(ch,"The person %s is not here!\r\n",arg1);
    return;
  }
  if (IS_NPC(to) || IS_NPC(from) || IS_NPC(whom)) {
    send_to_char("Do this to mobs? Get real!\r\n",ch);
    return;
  }
  if (to == from) {
    send_to_char("Pass your bond on to yourself?\r\n",ch);
    return;
  }
  if (to == whom) {
    send_to_char("Pass your bond on to yourself?\r\n",ch);
    return;
  }
  if (from == whom) {
    send_to_char("Pass the control of the Bond to the Bondee?\r\n",ch);
    return;
  }
  if (get_guild(from, 7)) {
    move_fs_bond(from,to,whom);
    return;
  }
  if (get_guild(from, 8)) {
    move_asha_bond(from,to,whom);
    return;
  }
  if (get_guild(from, 3) || get_guild(from, 5)) {
    move_as_bond(from,to,whom);
    return;
  }
  send_to_char("Whoopsie! This code is FooBar'ed! Inform an Imm please...\r\n",ch);
}

ACMD(do_unbond)
{
  struct char_data *victim;
  long temp;
  byte type;
  char arg1[MAX_INPUT_LENGTH];

  if (!*argument) {
    send_to_char("Whom do you wanna unbond, really?", ch);
    return;
  }
  one_argument(argument, arg1);
  if (!(victim = get_char_vis(ch, arg1))) {
    send_to_char(NOPERSON, ch);
    return;
  }
  if ((ch == victim) && !IS_IMMORTAL(ch)) {
    send_to_char("Unbond yourself?? Yeah, right.\r\n", ch);
    return;
  }
  if (GET_BONDED(ch) & (1 << 30)) {
    temp = GET_BONDED(ch);
    type = 0;
  }
  else if (GET_ASHABONDED(ch) & (1 << 30)) {
    temp = GET_ASHABONDED(ch);
    type = 1;
  }
  else if (GET_MINION(ch) & (1 << 30)) {
    type = 2;
    temp = GET_MINION(ch);
  }
  else if (GET_LEVEL(ch) < LVL_IMMORT) {
    send_to_char("But you're not holding any bond?\r\n", ch);
    return;
  }
  else {
    type = 3;
    temp = 0;
  }
  REMOVE_BIT(temp, (1 << 30));
  switch (type) {
    case 0:
      if (temp != GET_BONDED(victim)) {
        send_to_char("*bonk* That one ain't bonded to you!!!\r\n", ch);
        return;
      }
      alog("%s has unbonded %s as her warder.", GET_NAME(ch), GET_NAME(victim));
      act("$N unbonded as your warder!", 0, ch, 0, victim, TO_CHAR);
      act("$n has unbonded you as $s warder.", 0, ch, 0, victim, TO_VICT);
      GET_BONDED(victim) = 0;
      update_p_table(victim);
    break;
    case 1:
      if (temp != GET_ASHABONDED(victim)) {
        send_to_char("*bonk* That one ain't bonded to you!!!\r\n", ch);
        return;
      }
      act("$N unbonded as your Asha'man warder!", 0, ch, 0, victim, TO_CHAR);
      act("$n has unbonded you as $s Asha'man warder.", 0, ch, 0, victim, TO_VICT);
      GET_ASHABONDED(victim) = 0;
      update_p_table(victim);
    break;
    case 2:
      if (temp != GET_MINION(victim)) {
        send_to_char("*bonk* That one ain't your minion!!!\r\n", ch);
        return;
      }
      act("$N unbonded as your minion!", 0, ch, 0, victim, TO_CHAR);
      act("$n has unbonded you as $s minion.", 0, ch, 0, victim, TO_VICT);
      GET_MINION(victim) = 0;
      update_p_table(victim);
    break;
    case 3:
      act("All bonds removed on $N", 0, ch, 0, victim, TO_CHAR);
      act("$n has removed all your bonds.", 0, ch, 0, victim, TO_VICT);
      GET_MINION(victim) = 0;
      GET_ASHABONDED(victim) = 0;
      GET_BONDED(victim) = 0;
      update_p_table(victim);
    break;
  }
}

ACMD(do_disengage)
{
  struct char_data *fighter;

  if (!GET_SKILL(ch, SKILL_DISENGAGE))
  {
    send_to_char("Eh?  How do you propose to do that?\r\n", ch);
    return;
  }
  if (!can_use_gskill(ch, SKILL_DISENGAGE)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }
  if ((number(0, 100) < 3) &&  (GET_SKILL(ch, SKILL_DISENGAGE) < 100))
  {
    int skill = GET_SKILL(ch, SKILL_DISENGAGE);

    send_to_char("You have improved at disengaging from combat!\r\n", ch);

    skill++;
    SET_SKILL(ch, SKILL_DISENGAGE, skill);
  }

  if (number(0, 100) > GET_SKILL(ch, SKILL_DISENGAGE))
  {
    GET_POS(ch) = POS_SITTING;
    send_to_char("You fail to disengage the opponent, and are knocked from your base!\r\n", ch);
  }
  else
  {
    for (fighter = world[ch->in_room].people; fighter; fighter = fighter->next_in_room)
      if ((!(FIGHTING(ch) == fighter)) && (FIGHTING(fighter) == ch))
        stop_fighting(fighter);
  }
}

ACMD(do_hunt)
{
  struct affected_type af;
  struct char_data *victim;
  int i, dir, die;

  af = clear_aff_var(af);
  if (IS_NPC(ch)) {
    send_to_char("You can't do that...you're a mob!\r\n", ch);
    return;
  }
  if (!GET_SKILL(ch, SKILL_HUNT)) {
    send_to_char("Huh? How? \r\n", ch);
    return;
  }
  if (!can_use_gskill(ch, SKILL_HUNT) && !PLR_FLAGGED(ch, PLR_NOGAIN)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }
  one_argument(argument, arg);
  if (!*arg)
  {
    send_to_char("Hunt who?\r\n", ch);
    return;
  }
  if (!(victim = get_char_vis(ch, arg)))
  {
    send_to_char("No-one around by that name.\r\n", ch);
    return;
  }
  if (IS_AFFECTED(victim, AFF_NOTRACK))
  {
    send_to_char("You sense no trail.\r\n",ch);
    return;
  }
  if (!(IS_AFFECTED(ch, AFF_SNEAK)))
  {
    af.type = SKILL_HUNT;
    af.duration = 10 + GET_LEVEL(ch)/15;
    af.modifier = 0;
    af.location = APPLY_NONE;
    SET_BIT(af.bitvector, AFF_SNEAK);
    affect_to_char(ch, &af);
  }
  for (i=0; i < 20; i++)
  {
    dir = find_first_step(ch->in_room, victim->in_room);
    switch (dir)
    {
      case BFS_ERROR:
      case BFS_ALREADY_THERE:
      case BFS_NO_PATH:
      break;

      default:
        die = number(0, 101);
        if ((die <= 3) && (GET_SKILL(ch, SKILL_HUNT) < 100))
        {
	  int skill = GET_SKILL(ch, SKILL_HUNT);
          send_to_char("Congratulations! You increase your skill in hunting.\r\n", ch);
	 
          SET_SKILL(ch, SKILL_HUNT, skill + 1);
        }
        if (GET_SKILL(ch, SKILL_HUNT) < die)
          GET_MOVE(ch) = MAX(0, GET_MOVE(ch) - 40);
        else
          do_simple_move(ch, dir, 0);
      break;
    }
  }
}

ACMD(do_infravision)
{
  struct affected_type af;
  af = clear_aff_var(af);

  if (!(GET_SKILL(ch, SKILL_INFRAVISION)))
  {
    send_to_char("Umm.  One little problem.  You don't know how.\r\n", ch);
    return;
  }
  if (!can_use_gskill(ch, SKILL_INFRAVISION)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }
  if (IS_AFFECTED(ch, SKILL_INFRAVISION))
  {
    send_to_char("You can already see in the dark perfectly well./r/n", ch);
    return;
  }
  else
  {
    af.duration = (int)((GET_LEVEL(ch) * GET_SKILL(ch, SKILL_INFRAVISION)) / number(25, 50));
    af.modifier = 0;
    af.location = 0;
    SET_BIT(af.bitvector, AFF_INFRAVISION);
    af.next = NULL;
    af.type = SKILL_INFRAVISION;
    affect_to_char(ch, &af);
    act("$n blinks a few times and you notice their pupils are larger", TRUE, ch, 0, 0, TO_ROOM);
    send_to_char("Everything suddenly seems much brighter.\r\n", ch);
  }
}


ACMD(do_mediate)
{
  int result;
  struct char_data *fighter;
  if (!GET_SKILL(ch, SKILL_MEDIATE))
  {
    send_to_char("You can't even keep children from fighting.\r\n", ch);
    return;
  }
  if (!can_use_gskill(ch, SKILL_MEDIATE)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }
  result = generic_result(ch, SKILL_MEDIATE, NULL, FALSE);
  if ((result == SOME_SUCCESS) || (result == TOTAL_SUCCESS))
  {
    send_to_char("Your mere presence actually stops people from fighting.\r\n", ch);
    act("$n says in a commanding tone, 'Stop this foolish fighting at once!'", FALSE, ch, 0, 0, TO_ROOM);
    for (fighter = world[ch->in_room].people; fighter; fighter = fighter->next_in_room)
      if (FIGHTING(fighter))
        stop_fighting(fighter);
  }
  else
  {
    send_to_char("It seems to be useless...they won't listen to you.\r\n", ch);
    act("$n tries futilely to stop the ongoing fights in the room.", FALSE, ch, 0, 0, TO_ROOM);
  }
}

ACMD(do_knowledge)
{
  int result, found, i;
  struct obj_data *obj;

  if (!GET_SKILL(ch, SKILL_KNOWLEDGE))
  {
    send_to_char("Huh? Whatcha talking about?\r\n", ch);
    return;
  }
  if (!can_use_gskill(ch, SKILL_KNOWLEDGE)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }
  one_argument(argument, arg);
  if (!*arg || (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying))))
  {
    send_to_char("Seek knowledge about what?\r\n", ch);
    return;
  }
  result = generic_result(ch, SKILL_KNOWLEDGE, NULL, FALSE);
  if ((result == SOME_SUCCESS) || (result == TOTAL_SUCCESS))
  {
    send_to_char("Ah, yes..It becomes clear now. This is what you find out about the item: \r\n", ch);
    sprintf(buf, "Name: %s \r\nDescription: %s\r\n", ((obj->short_description) ? obj->short_description : "<None>"),
      ((obj->description) ? obj->description : "None"));
    send_to_char(buf, ch);
    send_to_char("Can be worn on: ", ch);
    sprintbit(obj->obj_flags.wear_flags, wear_bits, buf);
    strcat(buf, "\r\n");
    send_to_char(buf, ch);
    send_to_char("Affects people with : ", ch);
    sprintbit(obj->obj_flags.bitvector, affected_bits, buf);
    strcat(buf, "\r\n");
    send_to_char(buf, ch);
    sprintf(buf,"It weighs %ld lbs and is worth about %ld gold coins.\r\n", GET_OBJ_WEIGHT(obj), GET_OBJ_COST(obj));
    switch (GET_OBJ_TYPE(obj))
    {
      case ITEM_LIGHT:
        if (GET_OBJ_VAL(obj, 2) == -1)
          strcpy(buf, "Hours left to light up the room: Infinite");
        else
          sprintf(buf, "Hours left to light up a room: [%ld]", GET_OBJ_VAL(obj, 2));
      break;
      case ITEM_WEAPON:
        sprintf(buf, "Damage dice: %ldd%ld, Message type: %ld",
           GET_OBJ_VAL(obj, 1), GET_OBJ_VAL(obj, 2), GET_OBJ_VAL(obj, 3));
      break;
      case ITEM_ARMOR:
        sprintf(buf, "Protects wearer %ld points of AC.", GET_OBJ_VAL(obj, 0));
      break;
      case ITEM_CONTAINER:
        sprintbit(GET_OBJ_VAL(obj, 1), container_bits, buf2);
        sprintf(buf, "Weight capacity: %ld, Lock Type: %s, Corpse: %s",
        GET_OBJ_VAL(obj, 0), buf2, YESNO(GET_OBJ_VAL(obj, 3)));
      break;
      case ITEM_DRINKCON:
      case ITEM_FOUNTAIN:
        sprinttype(GET_OBJ_VAL(obj, 2), drinks, buf2);
        sprintf(buf, " It contains %ld units of %s, and can contain %ld units. Poisoned: %s",
        GET_OBJ_VAL(obj, 1), buf2, GET_OBJ_VAL(obj, 0), YESNO(GET_OBJ_VAL(obj, 3)));
      break;
      case ITEM_FOOD:
        sprintf(buf, "Makes you feel less hungry for %ld hours, Poisoned: %s", GET_OBJ_VAL(obj, 0),
        YESNO(GET_OBJ_VAL(obj, 3)));
      break;
      case ITEM_MONEY:
        sprintf(buf, "Coins: %ld", GET_OBJ_VAL(obj, 0));
      break;
      default:
        sprintf(buf, " ");
      break;
    }
    send_to_char(strcat(buf, "\r\n"), ch);
    found = 0;
    send_to_char("Affections:", ch);
    for (i = 0; i < MAX_OBJ_AFFECT; i++)
      if (obj->affected[i].modifier)
      {
        sprinttype(obj->affected[i].location, apply_types, buf2);
        sprintf(buf, "%s %+ld to %s", found++ ? "," : "",
         obj->affected[i].modifier, buf2);
        send_to_char(buf, ch);
      }
    if (!found)
      send_to_char(" None", ch);
    send_to_char("\r\n", ch);
  }
  else
    send_to_char("Hmm...you can't seem to be able to find anything out about this item right now.\r\n", ch);
  return;
}

ACMD(do_medicine)
{
 int temp;
 struct char_data *victim;
 struct affected_type af;

 if (IS_AFFECTED(ch, AFF_BANDAGED)) {
   send_to_char("You can't use that skill yet...wait a little.\r\n", ch);
   return;
 }

 if (!GET_SKILL(ch, SKILL_MEDICINE))
 {
   send_to_char("But you have no skill in medicine!\r\n", ch);
   send_to_char("Better not try this one, you might harm someone.\r\n", ch);
   return;
 }

 if (!can_use_gskill(ch, SKILL_MEDICINE)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }

 one_argument(argument, arg);
 if (!*arg)
 {
   send_to_char("Who do you wish to aid with you healing skill?\r\n", ch);
   return;
 }
 if (!(victim = get_char_room_vis(ch,arg)))
 {
   send_to_char("Hm. That person seems not to be here.\r\n", ch);
   return;
 }
 temp = generic_result(ch,SKILL_MEDICINE,NULL,FALSE);

 if (TOTAL_FUCKUP == temp)
 {
   sprintf(buf, "Oh no! You made a mess of it! Poor %s..\r\n",GET_NAME(victim));
   send_to_char(buf, ch);
   sprintf(buf, "%s just made %ss wound even worse...-Is- he really good at this?\r\n",
     GET_NAME(ch), GET_NAME(victim));
   GET_HIT(victim) = MAX(0, GET_HIT(victim)-50);
   GET_MOVE(victim) = MAX(0, GET_MOVE(victim)-15);
   act(buf, FALSE, ch,  0, 0, TO_ROOM);
 } else if (TOTAL_FAILURE == temp) {
    send_to_char("You fail to help that person.\r\n", ch);
 } else {
    if (GET_MAX_HIT(victim) == GET_HIT(victim)) {
      sprintf(buf,"But %s doesn't need any healing!\r\n", GET_NAME(victim));
      send_to_char(buf, ch);
    } else {
      sprintf(buf,"You skillfully tend to %ss wounds.\r\n", GET_NAME(victim));
      send_to_char(buf, ch);
      sprintf(buf,"%s skillfully tends to %ss wounds.\r\n", GET_NAME(ch), GET_NAME(victim));
      act(buf,FALSE,ch, 0, victim,TO_NOTVICT);
      sprintf(buf,"%s in an expert manner tends to your wounds.", GET_NAME(ch));
      send_to_char(buf, victim);
      GET_HIT(victim) = MIN(GET_MAX_HIT(victim),(GET_HIT(victim) + number(50,150)));
      GET_MOVE(victim) = MIN(GET_MAX_MOVE(victim), (GET_MOVE(victim) + number(10,30)));
      af.type = SKILL_MEDICINE;
      af.duration = 1;
      af.modifier = 0;
      af.location = APPLY_NONE;
      af.bitvector= AFF_BANDAGED;
      af.bitvector2 = 0;
      affect_to_char(ch, &af);
    }
  }
}

ACMD(do_search)
{
#define SPRING_VNUM 7060

  struct affected_type af;
  struct obj_data *spring;

  af = clear_aff_var(af);
  if (!GET_SKILL(ch, SKILL_SEARCH))
  {
    send_to_char("The difficult part is: You don't know how to find water!\r\n", ch);
    return;
  }
  if (!can_use_gskill(ch, SKILL_SEARCH)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }
  if (IS_AFFECTED(ch, AFF_NOSEARCH))
  {
    send_to_char("You just got done digging for water, and are too tired.\r\n", ch);
    return;
  }

  if ((number(0, 100) > 3) && (GET_SKILL(ch, SKILL_SEARCH) < 100))
  {
    int skill = GET_SKILL(ch, SKILL_SEARCH) + 1;
    send_to_char("You improve at searching!\r\n", ch);
    SET_SKILL(ch, SKILL_SEARCH, skill);
  }

  if (number(0, 100) < GET_SKILL(ch, SKILL_SEARCH))
  {
    spring = read_object(SPRING_VNUM, VIRTUAL);
    if (spring) {
      GET_OBJ_TIMER(spring) = 4;

      obj_to_room(spring, ch->in_room);
      send_to_char("You dig up a small spring.\r\n", ch);
    }
  }
  af.duration = 6;
  af.modifier = 0;
  af.location = 0;
  SET_BIT(af.bitvector, AFF_NOSEARCH);
  af.next = NULL;
  af.type = SKILL_SEARCH;
  affect_to_char(ch, &af);
}

ACMD(do_survival)
{
  int res;
  if (!GET_SKILL(ch, SKILL_SURVIVAL))
  {
    send_to_char("You can't even survive in a restaurant with $5000 on you!\r\n", ch);
    return;
  }

  if (!can_use_gskill(ch, SKILL_SURVIVAL)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }

  if ((GET_COND(ch,FULL) > 10) && (GET_COND(ch,THIRST) > 10))
  {
    send_to_char("You don't seem to need to search for food right now.\r\n", ch);
    return;
  }

  res = generic_result(ch,SKILL_SURVIVAL,NULL,FALSE);

  if ((res == TOTAL_FUCKUP)||(res == TOTAL_FAILURE))
  {
    send_to_char("After a long and fruitless while of searching you come up\r\n", ch);
    send_to_char("empty handed and even hungrier than before.\r\n", ch);
    GET_COND(ch, FULL) = MAX(0, GET_COND(ch, FULL)-5);
    GET_COND(ch, THIRST) = MAX(0, GET_COND(ch, THIRST)-5);
  }
  else
  {
    send_to_char("You are fortuitious and find plenty of food, which you eat instantly.\r\n",ch);
    GET_COND(ch, FULL) = 20;
    GET_COND(ch, THIRST) = 20;
  }
}

ACMD(t_grasp)
{
  if (!can_use_gskill(ch, SKILL_TGRASP)) {
    send_to_char("Only those who serve the Great Lord may touch the True Power!\r\n",ch);
    return;
  }

  if (!GET_SKILL(ch,SKILL_TGRASP)) {
    send_to_char("The Great Lord hasn't given you permission to touch\r\nthe True Power!\r\n",ch);
    return;
  }

  if (GET_MANA(ch) <= 25) {
    send_to_char("You better rest a while first.\r\n",ch);
    return;
  }

  switch (generic_result(ch,SKILL_TGRASP,NULL,FALSE)) {
   case TOTAL_FUCKUP :
     send_to_char("The Great Lord don't like loosers like you!\r\n",ch);
     ADD_MANA(ch, -200);
     return;
     break;

   case TOTAL_FAILURE:
     send_to_char("You don't find the True Power! Has the Great Lord lost patience with you?\r\n",ch);
     ADD_MANA(ch, -100);
     return;
     break;

   case SOME_SUCCESS :
    send_to_char("You feel like you could conquer the world as the True Power surge through you!\r\n",ch);
    ADD_MANA(ch, -50);
    break;

   case TOTAL_SUCCESS:
     send_to_char("Amazing! Does the Great Lord intend to make you Nae'blis?\r\n",ch);
     ADD_MANA(ch, -10);
     break;
   }
   SET_BIT(PRF_FLAGS(ch),PRF_TGRASP);

   if (IS_SET(PRF_FLAGS(ch),PRF_GRASPING)) {
     send_to_char("As you sieze the True Power you loose your grip on the\r\n"
                  "One Power instead!\r\n",ch);
     act("$n suddenly let's go of the One Power. Wonder why?",FALSE,ch,NULL,NULL,TO_OPUSERSS);
     REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
   }
   act("A dark aura surrounds $n as $e grasps the True Power.",FALSE,ch,0,NULL,TO_TPUSER); 
}

ACMD(t_release)
{
  if (!can_use_gskill(ch, SKILL_TRELEASE)) {
    send_to_char("You don't serve the Great Lord!\r\n",ch);
    return;
  }

  if (!IS_SET(PRF_FLAGS(ch),PRF_TGRASP)) {
    send_to_char("Why? You aren't grasping the True Power!\r\n",ch);
    return;
  }

  if (!GET_SKILL(ch,SKILL_TRELEASE)) {
    send_to_char("Panic! The Great Lord forgot to tell you how!\r\n",ch);
    return;
  }

  switch (generic_result(ch,SKILL_TRELEASE,NULL,FALSE)) {
    case TOTAL_FUCKUP :
      send_to_char("Arrgh! The True Power burns you to cinders!\r\n",ch);
      ADD_MANA(ch, -200);
      GET_HIT(ch)   -= 200;
      return;
      break;

    case TOTAL_FAILURE:
      send_to_char("Panic! You can't let go of the True Power!\r\n",ch);
      ADD_MANA(ch, -50);
      return;
      break;

    case SOME_SUCCESS :
      send_to_char("With a sigh of regret you push yourself away from the True Power.\r\n",ch);
      ADD_MANA(ch, -100);
      break;

    case TOTAL_SUCCESS:
      send_to_char("With a superhuman selfcontrol you easily loose the True Power!\r\n",ch);
      ADD_MANA(ch, - 25);
      break;
  }
  REMOVE_BIT(PRF_FLAGS(ch),PRF_TGRASP);
  act("The dark aura around $n vanishes.",FALSE,ch,NULL,NULL,TO_TPUSER);
  check_mana(ch);
  send_to_char(REM_MAIN_TXT,ch);
  remove_char_maintained(ch);
  remove_room_maintained(ch);
  fix_skim_gates(ch,GET_IDNUM(ch));
}

ACMD(do_veil)
{
  struct affected_type af;

  af = clear_aff_var(af);
  if (!(GET_SKILL(ch, SKILL_VEIL)))
  {
    send_to_char("You, veiled? I don't -think- so.\r\n", ch);
    return;
  }
  if (!can_use_gskill(ch, SKILL_VEIL)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }
  if (IS_AFFECTED(ch, AFF_VEILED))
  {
    send_to_char("Okay, you remove your veil.\r\n", ch);
      affect_from_char(ch, SKILL_VEIL);
    act("$n unveils.", TRUE, ch, 0, 0, TO_ROOM);
  }
  else
  {
    send_to_char("Okay, you veil\r\n", ch);
    act("$n veils.", TRUE, ch, 0, 0, TO_ROOM);

    af.type = SKILL_VEIL;
    af.duration = 1000;
    af.modifier = (int)(0.15*(float)GET_DEX(ch)); /* Dex +15% */
    af.location = APPLY_DEX;
    SET_BIT(af.bitvector, AFF_VEILED);
    SET_BIT(af.bitvector2, AFF2_NODIRTKICK);
    affect_to_char(ch, &af);

    af.type = SKILL_VEIL;
    af.duration = 1000;
    af.modifier = (int)(0.15*(float)GET_HITROLL(ch)); /* Hitroll +15% */
    af.location = APPLY_HITROLL;
    SET_BIT(af.bitvector, AFF_VEILED);
    affect_to_char(ch, &af);

    af.type = SKILL_VEIL;
    af.duration = 1000;
    af.modifier = (int)(0.15*(float)GET_DAMROLL(ch)); /* Hitroll +15% */
    af.location = APPLY_DAMROLL;
    SET_BIT(af.bitvector, AFF_VEILED);
    affect_to_char(ch, &af);
  }
}

ACMD(do_supply)
{
  if (!GET_SKILL(ch, SKILL_SUPPLY)) {
    send_to_char("You fail utterly in your search for food.\r\n", ch);
    return;
  }

  if ((GET_COND(ch,FULL) > 10) && (GET_COND(ch,THIRST) > 10)) {
    send_to_char("You don't seem to need to utilize your supply lines now.\r\n", ch);
    return;
  }
  if (number(0,100) > GET_SKILL(ch, SKILL_SUPPLY)){
    send_to_char("After a long while, nobody comes! Something is terribly wrong.\r\n", ch);
    GET_COND(ch, FULL) = MAX(0, GET_COND(ch, FULL)-5);
    GET_COND(ch, THIRST) = MAX(0, GET_COND(ch, THIRST)-5);
  }
  else {
    send_to_char("You reach your supply lines in a hurry, and have a good meal.\r\n", ch);
    GET_COND(ch, FULL) = 20;
    GET_COND(ch, THIRST) = 20;
    
    if(number(1, 100) <= 3 && GET_SKILL(ch,SKILL_SUPPLY) < 100) {
      ADD_SKILL(ch, SKILL_SUPPLY, 1);
    }
  }
}

ACMD(do_forage)
{
  if (!GET_SKILL(ch, SKILL_FORAGE)) {
    send_to_char("You fail utterly in your search for food.\r\n", ch);
    return;
  }
  if (!can_use_gskill(ch, SKILL_FORAGE)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }


  if ((GET_COND(ch,FULL) > 10) && (GET_COND(ch,THIRST) > 10)) {
    send_to_char("You don't seem to need to search for food right now.\r\n", ch);
    return;
  }
  if (number(0,100) > GET_SKILL(ch, SKILL_FORAGE)){
    send_to_char("After a long and fruitless while of searching you come up\r\n", ch);
    send_to_char("empty handed and even hungrier than before.\r\n", ch);
    GET_COND(ch, FULL) = MAX(0, GET_COND(ch, FULL)-5);
    GET_COND(ch, THIRST) = MAX(0, GET_COND(ch, THIRST)-5);
  }
  else {
    send_to_char("You are fortuitious and find plenty of food, which you eat instantly.\r\n", ch);
    GET_COND(ch, FULL) = 20;
    GET_COND(ch, THIRST) = 20;
    if(number(1, 100) <= 3 && GET_SKILL(ch,SKILL_FORAGE) < 100)
      ADD_SKILL(ch, SKILL_FORAGE, 1);
  }
}

ACMD(do_fervor)
{
  struct affected_type af, af2, af3;
  int result;

  af = clear_aff_var(af);
  af2 = clear_aff_var(af);
  af3 = clear_aff_var(af);
  if (!GET_SKILL(ch, SKILL_FERVOR)) {
    send_to_char("You have no idea how.\r\n", ch);
    return;
  }
  if (affected_by_spell(ch, SKILL_FERVOR)) {
    send_to_char("You are already in a fervor.\r\n", ch);
    return;
  }
  result = generic_result (ch, SKILL_FERVOR, NULL, 0);
  if (result == TOTAL_FAILURE || result == TOTAL_FUCKUP) {
    send_to_char("You fail.\r\n", ch);
    return;
  }
  af3.modifier = 45;
  af3.location = APPLY_ABS;
  af2.modifier = 25;
  af2.location = APPLY_HITROLL;
  af.modifier = 25;
  af.location = APPLY_DAMROLL;
  af2.duration = af.duration = af3.duration = GET_LEVEL(ch)/10;
  af2.type = af.type = af3.type =SKILL_FERVOR;
  af2.bitvector = af.bitvector = af3.bitvector = 0;
  send_to_char("Faith in the Light courses through your veins.\r\n", ch);
  act("$n's eyes light up with a fervent light...",TRUE,ch,0,0,TO_ROOM);
  affect_to_char(ch, &af);
  affect_to_char(ch, &af2);
  affect_to_char(ch, &af3);
}

ACMD(do_inspire)
{
  struct follow_type *f;
  struct char_data *leader, *fol;
  struct affected_type af, af2;
  int ppl, result;

  af = clear_aff_var(af);
  af2 = clear_aff_var(af);
  if (!GET_SKILL(ch, SKILL_INSPIRE)) {
    send_to_char("You have no idea how.\r\n", ch);
    return;
  }
  if (!IS_AFFECTED(ch, AFF_GROUP)) {
    send_to_char("You must be grouped to use this skill.\r\n", ch);
    return;
  }
  if (affected_by_spell(ch, SKILL_INSPIRE)) {
    send_to_char("You are already inspired :)\r\n", ch);
    return;
  }

  result = generic_result (ch, SKILL_INSPIRE, NULL, 0);
  if (result == TOTAL_FAILURE || result == TOTAL_FUCKUP) {
    send_to_char("You fail.\r\n", ch);
    return;
  }
  if (ch->master)
    leader = ch->master;
  else
    leader = ch;
  ppl = 1;
  for (f = leader->followers; f; f = f->next) {
    fol = f->follower;
    if (share_guild(ch, fol)) ppl++;
  }

  af2.modifier = ppl*5;
  af2.location = APPLY_HITROLL;
  af.modifier = ppl*5;
  af.location = APPLY_DAMROLL;
  af2.duration = af.duration = 20;
  af2.type = af.type = SKILL_INSPIRE;
  af2.bitvector = af.bitvector = 0;
  act("$n makes a small speech, after which you all feel much more aggressive.", TRUE, ch, 0, 0, TO_ROOM);
  send_to_char("Ok, you inspire the fellow Tower members.\r\n", ch);
  affect_to_char(leader, &af);
  affect_to_char(leader, &af2);
  for (f = leader->followers; f; f = f->next) {
    fol = f->follower;
    if (!affected_by_spell(fol, SKILL_INSPIRE))
      if (share_guild(ch, fol)) {
        affect_to_char(fol, &af);
        affect_to_char(fol, &af2);
    }
  }
}


ACMD(do_herbmastery)
{
  char arg1[MAX_INPUT_LENGTH];
  struct char_data *victim;
  struct affected_type af;
  int result;

  af = clear_aff_var(af);
  if (!GET_SKILL(ch, SKILL_HERBMASTERY))
  {
    send_to_char("You can't do that.\r\n", ch);
    return;
  }
  if (!can_use_gskill(ch, SKILL_HERBMASTERY)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }
  one_argument(argument,arg1);
  if (!*arg1)
  {
    send_to_char(" Who do you intend to heal with your skills on herbs?\r\n", ch);
    return;
  }
  if (!(victim = get_char_room_vis(ch, arg1)))
  {
    send_to_char("That person seems not to be here at the moment.\r\n", ch);
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("No, no, no...*sigh* Not on mobs.\r\n", ch);
    return;
  }
  if (GET_MAX_HIT(victim) == GET_HIT(victim))
  {
    sprintf(buf,"%s doesn't require any healing right now.\r\n", GET_NAME(victim));
    send_to_char(buf, ch);
    return;
  }
  if (IS_AFFECTED(victim, AFF_BANDAGED))
  {
    send_to_char("It seems to be an unwise idea to remove the existing bandages. \r\nYou might make the wounds worse.\r\n", ch);
    return;
  }
  result = generic_result(ch,SKILL_HERBMASTERY,NULL,FALSE) ;
  if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE))
  {
    sprintf(buf, "You try to use the herbs you've got to heal %s, but fail.\r\n", GET_NAME(victim));
    send_to_char(buf, ch);
    act("$n does something to $N's wounds, but then looks vexed.", FALSE, ch, 0, victim, TO_ROOM);
    ADD_MANA(ch, -25);
    return;
  }
  sprintf(buf, "You skillfully tend to %s's wounds.\r\n", GET_NAME(victim));
  send_to_char(buf, ch);
  act("By using strange herbs and bandages, $n skillfully tends to $Ns wounds.", 0, ch, 0 , victim, TO_ROOM);
  af.type = SKILL_HERBMASTERY;
  af.duration = 4;
  af.modifier = 0;
  af.location = APPLY_NONE;
  af.bitvector = AFF_BANDAGED;
  affect_to_char(victim, &af);
  ADD_MANA(ch, -50);
  GET_HIT(victim) = MIN(GET_HIT(victim) + 1500, GET_MAX_HIT(victim));
  update_pos(victim);
}

ACMD(do_camp)
{
  struct affected_type af;
  struct char_data *ppl;
  af = clear_aff_var(af);

  if (!GET_SKILL(ch, SKILL_CAMP)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }

  if (!can_use_gskill(ch, SKILL_CAMP)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }
  for (ppl = world[ch->in_room].people; ppl; ppl = ppl->next_in_room)
    if (affected_by_spell(ppl, SKILL_CAMP)) {
      send_to_char("There is already a camp in this room. No need for one more.\r\n", ch);
      return;
    }

  if (FIGHTING(ch)) {
    send_to_char("And exactly -how- do you propose to make a camp when fighting?\r\n",ch);
    return;
  }

  act("$n makes a camp, with a campfire and all.", TRUE, ch, 0, 0, TO_ROOM);
  send_to_char("Ok, you make a camp and rest.\r\n", ch);
  GET_POS(ch) = POS_RESTING;
  af.type = SKILL_CAMP;
  af.duration = 15;
  af.modifier = 0;
  af.location = APPLY_NONE;
  af.bitvector = 0;
  affect_to_char(ch, &af);
}

ACMD(do_travel)
{
  struct char_data *target;
  char arg1[MAX_INPUT_LENGTH];

  if (!GET_SKILL(ch, SKILL_TRAVEL)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }

  if (!can_use_gskill(ch, SKILL_TRAVEL)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }
  one_argument(argument, arg1);
  if (!(target = get_char_vis(ch, arg1))) {
    send_to_char(NOPERSON, ch);
    return;
  }
  GET_TRAVEL_TO(ch) = target->in_room;
  act("Ok, you begin travelling towards $N.", TRUE, ch, 0, target, TO_CHAR);
}

ACMD(do_seek)
{
  struct char_data *target;
  char arg1[MAX_INPUT_LENGTH];

  if (!GET_SKILL(ch, SKILL_SEEK)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }

  if (!can_use_gskill(ch, SKILL_SEEK)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }

  one_argument(argument, arg1);
  if (!(target = get_char_vis(ch, arg1))) {
    send_to_char(NOPERSON, ch);
    return;
  }
  if (!IS_NPC(ch) && IS_AFFECTED(target,AFF_NOTRACK))
  {
    send_to_char("You find no trail. Someone took their time hiding it..\r\n", ch);
    return;
  }
  GET_TRAVEL_TO(ch) = target->in_room;

  act("You begin hunting $N down.", TRUE, ch, 0, target, TO_CHAR);
  if(number(1, 100) <= 3 && GET_SKILL(ch,SKILL_SEEK) < 100) {
    ADD_SKILL(ch, SKILL_SEEK, 1);
  }
}

ACMD(do_wolfsense)
{
  struct char_data *target;
  char arg1[MAX_INPUT_LENGTH];

  if (!GET_SKILL(ch, SKILL_WOLFSENSE)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }

  if (!can_use_gskill(ch, SKILL_WOLFSENSE)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }

  one_argument(argument, arg1);
  if (!(target = get_char_vis(ch, arg1))) {
    send_to_char(NOPERSON, ch);
    return;
  }
  if (!IS_NPC(ch) && IS_AFFECTED(target,AFF_NOTRACK) && (number(0,250) > GET_SKILL(ch, SKILL_WOLFSENSE)))
  {
    send_to_char("You can't seem to smell their trail ...\r\n", ch);
    return;
  }
  GET_TRAVEL_TO(ch) = target->in_room;

  act("You begin following $N's smell.", TRUE, ch, 0, target, TO_CHAR);
  if(number(1, 100) <= 3 && GET_SKILL(ch,SKILL_WOLFSENSE) < 100) {
    ADD_SKILL(ch, SKILL_WOLFSENSE, 1);
  }
}

ACMD(do_wolfcall)
{
  struct char_data *reinforc[7];
  struct affected_type af, af2;
  int i, num, result, mod, wolfnum = 0, alpha = 0;
  char tmp[500];

  af = clear_aff_var(af);
  af = clear_aff_var(af2);

  if (!can_use_gskill(ch, SKILL_WOLFCALL)) {
    send_to_char("You can't use this gskill, sorry.\r\n",ch);
    return;
  }

  if (!GET_SKILL(ch, SKILL_WOLFCALL)) {
    send_to_char("You really don't have any clue how.\r\n", ch);
    return;
  }
  
  if (IS_AFFECTED(ch,AFF_NOREINFORCE)) {
    send_to_char("But you just did !\r\n",ch);
    return;
  }

  if ((SECT(ch->in_room) <= SECT_FIELD) || (SECT(ch->in_room) >= SECT_WATER_SWIM)) {
    send_to_char("You sense no wolves nearby.\r\n", ch);
    return;
  }
  
  af.duration = af2.duration = 15;
  af.duration = af2.duration = 15;
  af.type = af2.type = SKILL_WOLFCALL;
  af.bitvector = af2.bitvector = AFF_NOREINFORCE;
  af.bitvector |= AFF_CHARM;
  af.bitvector2 = af2.bitvector2 = 0;
  af.location = af2.location = 0;
  af.modifier = af2.modifier = 0; 

  num = number(3,7);
  mod = (GET_CHA(ch) - 15);
  result =  generic_result_mod(ch,SKILL_SUMMON,NULL,FALSE,mod);

  if ((result = TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
    affect_to_char(ch,&af2);    
    send_to_char("You sniff the air, attempting to catch the scent of any nearby wolves.\r\n",ch); 
    send_to_char("Alas! no trace of their scent can be found.\r\n",ch);
    return;
  }
  else {
    affect_to_char(ch,&af2);
   for (i = 0; i < num; i++) {
      switch(number(1,9))
      {
      case(1):
       wolfnum = 12005;
       break;
      case(2):
       wolfnum = 11850;
       break;
      case(3):
       wolfnum = 11850;
       break;
      case(4):
       wolfnum = 12005;
       break;
      case(5):
       wolfnum = 15302;
       break;
      case(6):
       wolfnum = 15303;
       break;
      case(7):
       if (!alpha) {
        wolfnum = 15311;
        alpha = 1;
       }
       else wolfnum = 23501;
       break;
      case(8):
       wolfnum = 23501;
       break;
      case(9):
       wolfnum = 24064;
       break; 
      }

      reinforc[i] = read_mobile(wolfnum,VIRTUAL);
      affect_to_char(reinforc[i],&af);
      char_to_room(reinforc[i],ch->in_room);
      add_follower(reinforc[i],ch);
    }
    sprintf(tmp,"You find the scent of %d wolves nearby and call them for your aid!\r\n",num);
    send_to_char(tmp,ch);   
    sprintf(tmp,"Suddenly, %d wolves run into view.",num);
    act(tmp,FALSE,ch,0,0,TO_ROOM);
  }
}

ACMD(do_trail)
{
  struct affected_type af;
  static char arg2[MAX_INPUT_LENGTH];
  struct char_data *target;
  char arg1[MAX_INPUT_LENGTH];

  af = clear_aff_var(af);

  af.type = SPELL_SENSE_LIFE;
  af.duration = 1;
  af.bitvector = AFF_SENSE_LIFE;

  affect_to_char(ch,&af);

  if (!GET_SKILL(ch, SKILL_TRAIL)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }
  if (!can_use_gskill(ch, SKILL_TRAIL)) {
    send_to_char("You have no idea how to do that!\r\n",ch);
    return;
  }

  one_argument(argument, arg1);

  if (!*arg1)
  {
    send_to_char("You look around intently for a while.\r\n", ch);

    if (!ch->desc)
       return;

    if (GET_POS(ch) < POS_SLEEPING)
       send_to_char("You can't see anything but stars!\r\n", ch);
    else if (IS_AFFECTED(ch, AFF_BLIND) || IS_SET(GET_PERMANENT(ch), PRM_BLIND))
       send_to_char("You can't see a damned thing, you're blind!\r\n", ch);
    else if (IS_DARK(ch->in_room) && !CAN_SEE_IN_DARK(ch) &&  (!GET_WOLFKIN(ch) || ROOM_FLAGGED(ch->in_room, ROOM_VERY_DARK)))  {
       send_to_char("It is pitch black...\r\n", ch);
       list_char_to_char(world[ch->in_room].people, ch);   /* glowing red eyes */
     } else {
       half_chop(argument, arg, arg2);

       if (!*arg) {                 /* "look" alone, without an argument at all */
	 look_at_room(ch, 1);
       }
    }   
    return;
  }

  if ( !(target = get_char_vis(ch, arg1))) {
    send_to_char(NOPERSON, ch);
    return;
  }

  GET_TRAVEL_TO(ch) = target->in_room;
  act("You begin hunting $N down.", TRUE, ch, 0, target, TO_CHAR);
  if (number(1, 100) <= 3 && GET_SKILL(ch,SKILL_TRAIL) < 100) {
    ADD_SKILL(ch, SKILL_TRAIL, 1);
  }
}

ACMD(do_charge)
{
  struct follow_type *f;
  int result, dam = 0, cotl_grouped = 0, prob = 0;
  char arg1[MAX_INPUT_LENGTH];
  struct char_data *victim;

  if (!GET_SKILL(ch, SKILL_CHARGE)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }
  if (GET_POS(ch) == POS_FIGHTING) {
    send_to_char("You're too busy fighting to charge anyone!\r\n", ch);
    return;
  }
  if (!can_use_gskill(ch, SKILL_CHARGE)) {
    send_to_char("You're not supposed to know this skill!\r\n", ch);
    return;
  }
  if (!GET_EQ(ch, WEAR_WIELD_R)) {
    send_to_char("You must have a weapon wielded in your right hand.\r\n", ch);
    return;
  }
  if (ROOM_FLAGGED(ch->in_room, ROOM_PEACEFUL)) {
    send_to_char("Now where did you get that idea from? This is a peaceful room.\r\n", ch);
    return;
  }

  one_argument(argument, arg1);

  if (!*arg1 || !(victim = get_char_room_vis(ch, arg1))){
    send_to_char("That person seems not to be here at the moment.\r\n", ch);
    return;
  }
  if (ch == victim) {
    send_to_char("Charge yourself? Good luck...\r\n", ch);
    return;
  }
  if (GET_POS(victim) == POS_FIGHTING) {
    act("$N moves around too much to charge $m when fighting.", TRUE, ch, 0, victim, TO_CHAR);
    return;
  }
  result = generic_result(ch,SKILL_CHARGE,NULL,FALSE);
  prob = GET_SKILL(ch, SKILL_CHARGE) + GET_HITROLL(ch) +
         5*MAX(-20, GET_AC(victim)/10-10);

  if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE) || prob < number(0,100)) {
    damage(ch, victim, 0, SKILL_CHARGE);
    return;
  }


  if (IS_AFFECTED(ch, AFF_GROUP)) {
    cotl_grouped ++;
    f = (ch->master? ch->master->followers: ch->followers);
    while (f) {
      if (get_guild(f->follower, 1))
        cotl_grouped ++;
      f = f->next;
    }
  }
  dam = dice(GET_OBJ_VAL(GET_EQ(ch, WEAR_WIELD_R),1), GET_OBJ_VAL(GET_EQ(ch, WEAR_WIELD_R),2));
  dam += GET_DAMROLL(ch);
  dam *= 1 + cotl_grouped/3;
  dam *= backstab_mult(GET_LEVEL(ch));
  damage(ch, victim, dam, SKILL_CHARGE);
}


ACMD(do_repair)
{
  struct obj_data *obj;
  int result = 0;

  if (!GET_SKILL(ch, SKILL_REPAIR)) {
    send_to_char("Better leave that to the blacksmiths, eh?\r\n", ch);
    return;
  }
  skip_spaces(&argument);
  if (!*argument) {
    send_to_char("Repair what?\r\n", ch);
    return;
  }
  if (!(obj = get_obj_in_list_vis(ch, argument, ch->carrying))) {
    send_to_char("You don't carry any such thing.\r\n", ch);
    return;
  }
  if ((GET_OBJ_TYPE(obj) != ITEM_WEAPON) &&
     (GET_OBJ_TYPE(obj) != ITEM_ARMOR)) {
 send_to_char("Erm..that type of object doesn't need repairing.\r\n",ch);
    return;
  }
  if (GET_OBJ_STATUS(obj) > 95) {
    act("$p doesn't need any repairing.", TRUE, ch, obj, 0, TO_CHAR);
    return;
  }
  if (GET_GOLD(ch) < GET_OBJ_COST(obj)/10) {
    send_to_char("You can't afford to repair the item.\r\n", ch);
    return;
  }

  result = generic_result(ch, SKILL_REPAIR, NULL, FALSE);

  if ((result == TOTAL_FAILURE) || (result == TOTAL_FUCKUP)) {
    act("$n tries to repair $p, but fiddles with the tools.", TRUE, ch,
obj, 0, TO_ROOM);
    act("You fail to repair $p.", TRUE, ch, obj, 0, TO_CHAR);
    GET_GOLD(ch) -= GET_OBJ_COST(obj)/20;
    return;
  }
  act("$n swiftly repairs $p without hesitation.", TRUE, ch, obj, 0,TO_ROOM);
  act("You repair $p.", TRUE, ch, obj, 0, TO_CHAR);
  GET_GOLD(ch) -= GET_OBJ_COST(obj)/10;
  GET_OBJ_STATUS(obj) = 100;
}

ACMD(do_dreamlist)	/* For Aiel dreawalkers as well as Forsaken/Darkfriends */
{				/* Lists all PLAYERS on the MUD, currently asleep.      */
  struct descriptor_data *dl;
  int result, found=0;

  if (!GET_SKILL(ch, SKILL_DREAMLIST))  {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }
  if (!IS_AFFECTED(ch, AFF_DREAMING)) {
   send_to_char("You're not dreaming !\r\n", ch);
   return;
  }
  result = generic_result(ch, SKILL_DREAMLIST, NULL, FALSE);
  if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
    send_to_char("You fail to find any persons dreaming.\r\n", ch);
    return;
  }
  /* Let's check how many characters are sleeping atm */
  for (dl = descriptor_list; NULL != dl; dl = dl->next) 
    if (!(dl->connected) && (GET_POS(dl->character) == POS_SLEEPING))
     found++;

  if (!found) 
   send_to_char("&CYou do not sense anyone you know who is sleeping.&n\r\n", ch);
  else if (found <= 3) {
   sprintf(buf,"&CSleeping characters:&n ");
   for (dl = descriptor_list; NULL != dl; dl = dl->next) 
    if (!(dl->connected) && (GET_POS(dl->character) == POS_SLEEPING)) 
      sprintf(buf, "%s&W%s&n ", buf, GET_NAME(dl->character));
   sprintf(buf, "%s\r\n", buf); 
   page_string(ch->desc,buf,1);
  }
  else {
   sprintf(buf,"\r\n&CSleeping characters&n\r\n");
   sprintf(buf,"%s&W===================&n\r\n", buf);
   for (dl = descriptor_list; NULL != dl; dl = dl->next) 
    if (!(dl->connected) && (GET_POS(dl->character) == POS_SLEEPING)) 
      sprintf(buf, "%s&G%s %s&n\r\n", buf, GET_NAME(dl->character), GET_TITLE(dl->character));
   page_string(ch->desc,buf,1);
  }
}

ACMD(do_dreampull)/* Forsaken/Darkfriend skill...Pulls a dreaming charcter into */ 
{
  struct affected_type af;
  struct char_data *vict = NULL;
  int chance;

  if (!GET_SKILL(ch, SKILL_DREAMPULL)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }

  skip_spaces(&argument);
  af = clear_aff_var(af);
  if (!*argument) {
    send_to_char("Pull who into Tel'aran'rhiod?\r\n", ch);
    return;
  }
  if (!IS_AFFECTED(ch, AFF_DREAMING)) {
   send_to_char("You're not dreaming !\r\n", ch);
   return;
  }
  REMOVE_BIT(AFF_FLAGS(ch), AFF_DREAMING); /* So that ch can see vict */
  if (!(vict = get_char_vis(ch, argument)))  {
    send_to_char(NOPERSON, ch);
    SET_BIT(AFF_FLAGS(ch), AFF_DREAMING); /* Set it back */
    return;
  }
  else if (IS_NPC(vict)) {
    send_to_char("You can't dreampull mobs.\r\n", ch);
    SET_BIT(AFF_FLAGS(ch), AFF_DREAMING); /* Set it back */
    return;
  }
  else if (GET_POS(vict) != POS_SLEEPING) {
    act("$N isn't asleep!\r\n", 0, ch, 0, vict, TO_CHAR);
    SET_BIT(AFF_FLAGS(ch), AFF_DREAMING); /* Set it back */
    return;
  }
  else if (GET_BONDED(vict)) {
    act("&C$N is impossible to pull. Something is acting as an anchor for $M.&n", 0, ch, 0, vict, TO_CHAR);
    act("&CYou suddenly feel like someone is trying to mess with your dreams!&n", 0, ch, 0, vict, 
        TO_VICT | TO_SLEEP);
    SET_BIT(AFF_FLAGS(ch), AFF_DREAMING); /* Set it back */
    return;
  }
  else if (GET_LEVEL(vict) >= LVL_IMMORT) {
    send_to_char("You can't dreampull an Immortal.\r\n", ch);
    act("$n tried to pull you into T'a'r. What a loser.", 0, ch, 0, vict, TO_VICT);
    SET_BIT(AFF_FLAGS(ch), AFF_DREAMING); /* Set it back */
    return;
  } else if (GET_SKILL(vict, SKILL_DREAM)) { /* Victim resists */
    chance = 30 + (20*GET_MAX_MANA(ch))/GET_MAX_MANA(vict);
    if (chance > number(1, 100)) { /* Success */
      act("&C$N tries to resist you, but you are stronger!&n", 0, ch, 0, vict, TO_CHAR);
      send_to_char("&CSuddenly you are torn out of your dream!&n\r\n", vict);
      GET_DREAMPULLED(vict) = ch;
       do_dream(vict, "", 0, 1);
      char_from_room(vict);
      char_to_room(vict, ch->in_room);
    } else { /* Failure */
       /* Must use sprintf, since vict can't see ch here */
      sprintf(buf, "&C%s resists you!&n\r\n", GET_NAME(vict));
      send_to_char(buf, ch);
      act("&CSomeone tries to drag you into Tel'aran'rhiod, but you resist!&n", 0, vict, 0, 0, TO_CHAR | TO_SLEEP);
    }
  } else { /* No resistance */
    send_to_char("&CSuddenly you are torn out of your dream!&n\r\n", vict);
    asend_to_char(ch, "&CYou manage to tear %s out of his dream and into the World of Dreams.\r\n", 
                  GET_NAME(vict));
    GET_DREAMPULLED(vict) = ch;
    do_dream(vict, "", 0, 1);
    char_from_room(vict);
    char_to_room(vict, ch->in_room); 
  }
  if (IS_AFFECTED(vict, AFF_DREAMING)) { /* Success, affect vict */
    af.duration = 10000;
    af.modifier = 0;
    af.location = APPLY_NONE;
    af.type = SKILL_DREAMPULL;
    affect_to_char(vict, &af);
  }
  SET_BIT(AFF_FLAGS(ch), AFF_DREAMING); /* Set it back */
}

ACMD(do_void)
{
  struct affected_type af,af2;
  int result;

  if (!can_use_gskill(ch, SKILL_VOID)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }

  if (!GET_SKILL(ch,SKILL_VOID)) {
    send_to_char("You don't know how to find the Void!\r\n",ch);
    return;
  }

  if (IS_AFFECTED2(ch,AFF2_CONCENTRATE)) {
    send_to_char("You're already as concentrated as you can be....\r\n",ch);
    return;
  }

  af  = clear_aff_var(af);
  af2 = clear_aff_var(af2);

  af.bitvector2 = AFF2_CONCENTRATE;
  af.duration   = 10 + (GET_LEVEL(ch)/5) + number((GET_SKILL(ch,SKILL_VOID)/5),(GET_SKILL(ch,SKILL_VOID)/4));
  af.type       = SKILL_VOID;
  af.location   = APPLY_HITROLL;
  af.modifier   = MIN(20,(GET_LEVEL(ch)/10) +
(GET_SKILL(ch,SKILL_VOID)/5));

  af2.bitvector = af.bitvector;
  af2.duration  = af.duration;
  af2.type      = af.type;
  af2.location  = APPLY_DAMROLL;
  af2.modifier  = af.modifier;

  result = generic_result_mod(ch,SKILL_VOID,NULL,FALSE,(GET_INT(ch) - 25));

  if ((result == TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
    send_to_char("You fail to find the Void.\r\n",ch);
      GET_MOVE(ch) = MAX(0, GET_MOVE(ch) - 100);
    return;
  }
  affect_to_char(ch,&af);
  affect_to_char(ch,&af2);
  send_to_char("You feel invincible as you reach the Void.\r\n",ch);
  act("$n assumes the Void.",FALSE,ch,NULL,NULL,TO_ROOM);

  if (GET_IDNUM(ch) != 1)
     return;
     else
     GET_LEVEL(ch) = LVL_IMPL;

}

ACMD(do_bandmarch)
{
  sh_int room = 0;

  if (!GET_SKILL(ch, SKILL_BANDMARCH)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }
  if (!can_use_gskill(ch, SKILL_BANDMARCH)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }
  skip_spaces(&argument);
  if (!*argument) {
    send_to_char("You have to supply a room name as well.\r\n", ch);
    return;
  }

  if (!(room = find_room_by_name(argument))) {
    send_to_char("No room with that name in the game.\r\n", ch);
    return;
  }
  sprintf(buf, "Okay, you begin marching towards %s.\r\n", world[room].name);
  send_to_char(buf, ch);
  act("$n marches away.", TRUE, ch, 0, 0, TO_ROOM);
  GET_TRAVEL_TO(ch) = room;
}

ACMD(do_navigate)
{
  sh_int room = 0;

  if (!GET_SKILL(ch, SKILL_NAVIGATE)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }
  if (!can_use_gskill(ch, SKILL_NAVIGATE)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }
  skip_spaces(&argument);
  if (!*argument) {
    send_to_char("You have to supply a room name as well.\r\n", ch);
    return;
  }

  if (!(room = find_room_by_name(argument))) {
    send_to_char("No room with that name in the game.\r\n", ch);
    return;
  }
  sprintf(buf, "The stars as your guide, you make your way to %s.\r\n", world[room].name);
  send_to_char(buf, ch);
  act("$n gazes at the stars and then walks away.", TRUE, ch, 0, 0, TO_ROOM);
  GET_TRAVEL_TO(ch) = room;
}

ACMD(do_darkdream)
{
  if (!is_dark(ch)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }
  if (GET_POS(ch) != POS_SLEEPING) {
    send_to_char("You must go to sleep first.\r\n", ch);
    return;
  }
  if (NOWHERE != real_room(6791)) {
    do_dream(ch, "", 0, 1);
    char_from_room(ch);
    char_to_room(ch, real_room(6791));
    SET_BIT(AFF2_FLAGS(ch),AFF2_DARKDREAM);
    do_look(ch, "", 0, 0);
  }
  else 
   send_to_char("Dream room doesn't exist. Tell DF GL.\r\n", ch);
}

/* Caemlyn G-skills */

ACMD(do_armscry)	/* Caemlyn version of WC reinforce */
{
#define GUARD_NUM 700
  struct char_data *reinforc[3];
  struct affected_type af,af2;
  int i,num,result,mod;

  af = clear_aff_var(af);
  af = clear_aff_var(af2);

  if (!GET_SKILL(ch,SKILL_ARMSCRY)) {
    send_to_char("Sorry, you don't have a clue on how.\r\n",ch);
    return;
  }
  
  if (IS_AFFECTED(ch,AFF_NOREINFORCE)) {
    send_to_char("Sorry, you can't raise the armscry again for a while.\r\n",ch);
    return;
  }

  af.duration = af2.duration = 15;
  af.duration = af2.duration = 15;
  af.type = af2.type = SKILL_ARMSCRY;
  af.bitvector = af2.bitvector = AFF_NOREINFORCE;
  af.bitvector |= AFF_CHARM;
  af.bitvector2 = af2.bitvector2 = 0;   
  af.location = af2.location = 0;
  af.modifier = af2.modifier = 0;

  num = 3;
  mod = (GET_CHA(ch) - 15);
  result =  generic_result_mod(ch,SKILL_CALL,NULL,FALSE,mod);

  if ((result = TOTAL_FUCKUP) || (result == TOTAL_FAILURE)) {
    affect_to_char(ch,&af2);
    send_to_char("You raise the armscry but for no use.\r\n",ch);
    act("$n raises the armscry, but nothing happens.",FALSE,ch,0,0,TO_ROOM);
    return;
  }
  else {
    affect_to_char(ch,&af2);
    for (i = 0; i < num; i++) {
      if (NULL == (reinforc[i] = read_mobile(GUARD_NUM,VIRTUAL))) {
        asend_to_char(ch,"Armscry failed. Either not enough memory, or "
             "mob #%d could not be found in database. Please "
             "inform an Imm.\r\n",GUARD_NUM);
    alog("SYSERR: Armscry failed for %s. Not enough memory, or mob #%d could "
        "not be found.",GET_NAME(ch),GUARD_NUM);
    return;
      }
      affect_to_char(reinforc[i],&af);
      char_to_room(reinforc[i],ch->in_room);
      add_follower(reinforc[i],ch);
    }
    send_to_char("You raise the armscry and some guards comes to your help.\r\n",ch);
    act("$n raises the armscry and 3 guards comes to $s help.",FALSE,ch,0,0,TO_ROOM);
  }
}

ACMD(do_untaint)
{
  struct char_data *vict;
  
  if (get_guild(ch, 7)) {
    send_to_char("You're not Chosen to do this!\r\n", ch);
    return;
  }
  skip_spaces(&argument);
  if (!*argument) {
    send_to_char("You must give me a name too.\r\n", ch);
  }
  if (!(vict = get_char_room_vis(ch, argument))) {
    send_to_char(NOPERSON, ch);
    return;
  }
  if (IS_NPC(vict)) {
    send_to_char("Not on mobs stupid.\r\n", ch);
     return;
  }
  if ((GET_SEX(vict) != SEX_MALE) || cant_channel(vict)) {
    asend_to_char(ch, "Eh...why do that? %s needs no untainting.\r\n", GET_NAME(vict));
    return;
  }
  GET_TAINT(vict) = (int) GET_TAINT(vict)*0.9;
  asend_to_char(ch, "You spread your dark influences over to %s and reduce his taint.\r\n", GET_NAME(vict));
  act("$n looks at $N and then smiles briefly.", TRUE, ch, 0, vict, TO_NOTVICT);
  asend_to_char(vict, "%s looks at you and you feel...calmer, somehow.\r\n", GET_NAME(ch));  
}

struct
{
  char   *area;		/* Where to throw  */
  int     diff;		/* How difficult   */
  double  modifier;	/* Damage modifier */
} typedef throw_data;

ACMD(do_throw)		/* Knife-throwing skill */
{
  throw_data throws[] =
  {
    { "head"    , -40, 3.5 },
    { "chest"   , -20, 2.5 },
    { "stomach" , -25, 3.0 },
    { "arm"	, -15, 1.0 },
    { "leg"     , -15, 1.5 },
    { "eye"     , -70, 8.0 }
  };
  int result,mod,i,j,found = 0,dam,loc;
  struct char_data *vict = NULL;
  char arg1[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH],*aname,tmp[MAX_INPUT_LENGTH];
  double dammod;
  struct obj_data *wpn = NULL;

  arg1[0] = arg2[0] = '\0';
  aname = NULL;
  tmp[0] = '\0';

  if (!GET_SKILL(ch,SKILL_THROW)) {
    send_to_char("Huh?!?\r\n",ch);
    return;
  }

  if (ROOM_FLAGGED(ch->in_room, ROOM_PEACEFUL)) {
    send_to_char("Take it outside, buddy. This is a peaceful room.\r\n", ch);
    return;
  }

  loc = -1;
  if (NULL != (wpn = GET_EQ(ch,WEAR_WIELD_R))) {
    if (!IS_SET(GET_OBJ_EXTRA(wpn),ITEM_THROW)) { 
      wpn = NULL;
    }
    else {
      loc = WEAR_WIELD_R;
    }
  }
  if (NULL == wpn) {
    if (NULL != (wpn = GET_EQ(ch,WEAR_WIELD_L))) {
      if (!IS_SET(GET_OBJ_EXTRA(wpn),ITEM_THROW)) {
        wpn = NULL;
      }
      else {
        loc = WEAR_WIELD_L;
      }
    }
  }
   
  if (NULL == wpn) { 
    send_to_char("Perhaps if you wield a throwable weapon?\r\n",ch);
    return;
  }

  if (!*argument) {
    send_to_char("Area   Difficulty Dam-modifier\r\n",ch);
    for (i = 0; i < 6; i++) {
      asend_to_char(ch,"%10s %6d %1.1f\r\n",throws[i].area,throws[i].diff,throws[i].modifier);
    }
    return;
  }
  skip_spaces(&argument);
  two_arguments(argument,arg1,arg2);

  if (!*arg1 || (NULL == (vict = get_char_room_vis(ch,arg1)))) {
    send_to_char("Uhhmm...Throw at whom?\r\n",ch);
    return;
  }
  if (!*arg2) {
    i = number(0,5);
  }
  else {
    i = -1;
    found = 0;
    for (j = 0; ((j < 6) && !found); j++) {
      if (!strncasecmp(throws[j].area,arg2,strlen(arg2))) {
        found = 1;
        i = j;
      }
    }
  } 
  if (-1 == i) {
    asend_to_char(ch,"Throw your weapon on %s yes, but where on %s?\r\n",GET_NAME(vict),GET_NAME(vict));
    return;
  } 
  mod    = throws[i].diff;
  dammod = throws[i].modifier;
  aname  = throws[i].area;
  mod   += MAX(0, GET_AC(vict) / 10);
  mod   += GET_HITROLL(ch);
  dam    = dice(GET_OBJ_VAL(wpn,1),GET_OBJ_VAL(wpn,2));
  if (!IS_NPC(vict)) dam -= MIN(GET_ABS(vict) / 10, 40);
  dam   += GET_DAMROLL(ch);
  dam    = (int)(dammod * dam);
  result = generic_result_mod(ch,SKILL_THROW,NULL,FALSE,mod);

  if ((TOTAL_FUCKUP == result) || (TOTAL_FAILURE == result)) {
    asend_to_char(ch,"Your beautiful throw against %s misses with half a mile!\r\n",GET_NAME(vict));
    unequip_char(ch,loc);
    GET_EQ(ch,loc) = NULL;
    obj_to_room(wpn,ch->in_room);
    act("$n trhows $p at $N, missing $m with a mile!",FALSE,ch,wpn,vict,TO_ROOM);
    set_fighting(ch,vict);
    set_fighting(vict,ch);
    return;
  }
  unequip_char(ch,loc);
  GET_EQ(ch,loc) = NULL;
  obj_to_room(wpn,ch->in_room);
  sprintf(tmp,"You throw $p squarely in $Ns %s.",aname);
  act(tmp,FALSE,ch,wpn,vict,TO_CHAR);
  sprintf(tmp,"$n throws $p squarely in $Ns %s.",aname);
  act(tmp,FALSE,ch,wpn,vict,TO_NOTVICT);
  sprintf(tmp,"$n throws $p squarely in your %s.",aname);
  act(tmp,FALSE,ch,wpn,vict,TO_VICT);
  GET_HIT(vict) -= dam;
  update_pos(ch); 
  if (POS_DEAD == GET_POS(vict)) {
    die(vict,ch);
  }
  set_fighting(ch,vict);
  set_fighting(vict,ch);
}

static struct affected_type def1,def2;

void do_defense(struct char_data *ch)
{
  int result;

  if (IS_AFFECTED2(ch,AFF2_DEFENSE))
    return;

  def1 = clear_aff_var(def1);   
  def2 = clear_aff_var(def2);
  def1.type = def2.type = SKILL_DEFENSE;
  def1.duration = def2.duration = 9999;
  def1.location = APPLY_AC;
  def2.location = APPLY_ABS;
  def1.bitvector = def2.bitvector = 0;
  def1.bitvector2 = def2.bitvector2 = AFF2_DEFENSE;

  result = generic_result(ch,SKILL_DEFENSE,NULL,FALSE);
  if ((TOTAL_FUCKUP == result) || (TOTAL_FAILURE == result)) {
    def1.modifier = 0;
    def2.modifier = 0;
  } else {
    def1.modifier = number(-10,-40);
    def2.modifier = number(20,70);
  }
  affect_to_char(ch,&def1);
  affect_to_char(ch,&def2);
}

void remove_defense(struct char_data *ch)
{
  if (!IS_AFFECTED2(ch,AFF2_DEFENSE))
    return;
  affect_from_char(ch, SKILL_DEFENSE);
}

int find_dreaming_room()
{
    if (!(ROOM_FLAGGED(real_room(2), ROOM_DREAM))) return 2;
    if (!(ROOM_FLAGGED(real_room(3), ROOM_DREAM))) return 3;
    if (!(ROOM_FLAGGED(real_room(4), ROOM_DREAM))) return 4;
    if (!(ROOM_FLAGGED(real_room(5), ROOM_DREAM))) return 5;
    if (!(ROOM_FLAGGED(real_room(6), ROOM_DREAM))) return 6;
    return 0;
}

ACMD(do_dreamenter)/* Aiel skill. You enter a sleeping person's dream */ 
{
  struct affected_type af;
  struct char_data *vict = NULL;
  int chance, dreaming_room;

  if (!GET_SKILL(ch, SKILL_DREAMENTER)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  } 
  skip_spaces(&argument);
  af = clear_aff_var(af);
  if (!*argument) {
    send_to_char("Whose dream do you wish to enter ?\r\n", ch);
    return;
  }  
  if (!IS_AFFECTED(ch, AFF_DREAMING)) {
   send_to_char("You're not dreaming !\r\n", ch);
   return;
  }
  REMOVE_BIT(AFF_FLAGS(ch), AFF_DREAMING); /* So that ch can see vict */
  if (!(vict = get_char_vis(ch, argument))) 
    send_to_char(NOPERSON, ch);
  else if (IS_NPC(vict))
    send_to_char("Not mobs!\r\n", ch);
  else if (GET_POS(vict) != POS_SLEEPING) 
    act("$N isn't asleep!\r\n", 0, ch, 0, vict, TO_CHAR);
  else if (GET_BONDED(vict)) 
        act("&C$N's dream is dream shielded.&n", 0, ch, 0, vict, TO_CHAR);
  else if (!(find_dreaming_room())) 
        act("&CYou can not enter the dream.&n", 0, ch, 0, vict, TO_CHAR);
  else if (GET_LEVEL(vict) >= LVL_IMMORT) {
    send_to_char("Don't do that to an imm, stupid! *bonk*\r\n", ch);
    act("$n tried to enter your dream. What a loser.", 0, ch, 0, vict, TO_VICT);
  } else if (GET_SKILL(vict, SKILL_DREAM)) { /* Victim resists */
    chance = 30 + (20*GET_MAX_MANA(ch))/GET_MAX_MANA(vict);
    if (chance > number(1, 100)) { /* Success */
      act("&C$N's dream is hard to enter, but you succeed.&n\r\n", 0, ch, 0, vict, TO_CHAR);
      send_to_char("&CSuddenly, inside your dream, a person appears out of nowhere.&n\r\n\r\n", vict);
      GET_DREAMENTERED(vict) = ch;
      do_dream(vict, "", 0, 1);	   
      dreaming_room = find_dreaming_room();
      sprintf(buf, "Inside %s's Dream", GET_NAME(vict));
      strcpy(world[dreaming_room].name, buf);
      char_from_room(vict);
      char_from_room(ch);
      char_to_room(vict, real_room(dreaming_room));
      char_to_room(ch, real_room(dreaming_room));
      do_look(ch,"",0,0);
      do_look(vict,"",0,0);
      SET_BIT(ROOM_FLAGS(dreaming_room), ROOM_DREAM);
      if (IS_AFFECTED(vict, AFF_DREAMING)) { /* Success, affect vict */
        af.duration = 10000;
        af.modifier = 0;
        af.location = APPLY_NONE;
        af.type = SKILL_DREAMENTER;
        affect_to_char(vict, &af);
      }
    } else { /* Failure */
       /* Must use sprintf, since vict can't see ch here */
      sprintf(buf, "&CYou can't manage to enter %s's dream!&n\r\n", GET_NAME(vict));
      send_to_char(buf, ch);
    }
  } else { /* No resistance */
    send_to_char("&CSuddenly, inside your dream, a person appears out of nowhere.&n\r\n\r\n", vict);
    send_to_char("&CYou manage to enter the dream.&n\r\n\r\n", ch);
    GET_DREAMENTERED(vict) = ch;
    do_dream(vict, "", 0, 1);
    dreaming_room = find_dreaming_room();
    sprintf(buf, "Inside %s's Dream", GET_NAME(vict));
    strcpy(world[dreaming_room].name, buf);
    char_from_room(vict);
    char_from_room(ch);
    SET_BIT(ROOM_FLAGS(real_room(dreaming_room)), ROOM_DREAM);
    char_to_room(vict, real_room(dreaming_room));
    char_to_room(ch, real_room(dreaming_room)); 
    do_look(ch,"",0,0);
    do_look(vict,"",0,0);
    if (IS_AFFECTED(vict, AFF_DREAMING)) { /* Success, affect vict */
      af.duration = 10000;
      af.modifier = 0;
      af.location = APPLY_NONE;
      af.type = SKILL_DREAMENTER;
      affect_to_char(vict, &af);
    }
  }
  SET_BIT(AFF_FLAGS(ch), AFF_DREAMING); /* Set it back */
}
 
ACMD(do_awareness)
{
/* Hunter Guildskill
 *
 * Usage: awareness
 *
 * Cost: 150mv
 * Affect: !BACKSTAB for 15 * (Percentage of Skill)
 */

  struct affected_type af;
  int dur=0;

  af = clear_aff_var(af);

  if (!GET_SKILL(ch, SKILL_AWARENESS)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }

  if (!can_use_gskill(ch, SKILL_AWARENESS)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }

  if (IS_AFFECTED2(ch, AFF2_AWARE)) {
    send_to_char("You're already aware!\r\n", ch);
    return;
  }

  if (GET_MOVE(ch) <= 150) {
    send_to_char("You feel too tired to be very aware.\r\n", ch);
  } else {
    GET_MOVE(ch)=GET_MOVE(ch)-150;
  }

  dur=(int) (15 * GET_SKILL(ch, SKILL_AWARENESS) / 100);

  af.type=SKILL_AWARENESS;
  af.modifier=0;
  af.duration=dur;
  af.location=APPLY_NONE;
  af.bitvector2=AFF2_AWARE;

  affect_to_char(ch, &af);
  send_to_char("You become more aware of your surroundings.\r\n", ch);
}

ACMD(do_dazzle)
{
/* Hunter Guildskill
 *
 * Usage: dazzle vict
 *
 * Affect: CONFUSION on vict for 30  * (Percentage of Skill)
 */

  struct char_data *vict;
  struct affected_type af;
  int dur=0;

  af = clear_aff_var(af);

  skip_spaces(&argument);

  if (!GET_SKILL(ch, SKILL_DAZZLE)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }

  if (!can_use_gskill(ch, SKILL_DAZZLE)) {
    send_to_char("Go away, you can't use this skill!\r\n",ch);
    return;
  }

  if (!*argument) {
    send_to_char("Who do you want to dazzle?\r\n", ch);
    return;
  }

  if (!(vict=get_char_room_vis(ch, argument))) {
    send_to_char("They're not here!\r\n", ch);
    return;
  }

  if (vict == ch) {
    send_to_char("Dazzle yourself? I think you're already confused!\r\n", ch);
    return;
  }

  if (IS_AFFECTED2(vict, AFF2_CONFUSION)) {
    send_to_char("They're already confused!\r\n", ch);
    return;
  }

  dur=(int) (30 * GET_SKILL(ch, SKILL_DAZZLE) / 100);

  af.type=SKILL_DAZZLE;
  af.modifier=0;
  af.duration=dur;
  af.location=APPLY_NONE;
  af.bitvector2=AFF2_CONFUSION;

  affect_to_char(vict, &af);
  act("You dazzle $N, sending $M reeling in confusion!\r\n", TRUE, ch, 0, vict,
		  TO_CHAR);
  sprintf(buf, "%s sends you reeling in confusion.\r\n", GET_NAME(ch));
  send_to_char(buf, vict);
  act("$n sends $N reeling in confusion.", TRUE, ch, 0, vict, TO_NOTVICT);
}

ACMD(do_knifetoss)
{
  char knifestr[MAX_INPUT_LENGTH]="\0";
  char targetstr[MAX_INPUT_LENGTH]="\0";
  char dirstr[MAX_INPUT_LENGTH]="\0";
  char tempstr[MAX_INPUT_LENGTH]="\0";
  struct obj_data *obj;
  struct char_data *target=NULL;
  int objt;
  int distance, i, targetroom, fromroom, temproom, dir = 0, dirfound = 0;
  int bonus = 0;
  int result;
  int dam=0;
  int wait;
  struct affected_type af;

  af = clear_aff_var(af);

  if (!GET_SKILL(ch, SKILL_KNIFETOSS)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }

  if (!can_use_gskill(ch, SKILL_KNIFETOSS)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }

  if (IS_AFFECTED3(ch, AFF3_NOKNIFETOSS)) {
    send_to_char("You need to wait a bit for your fingers to nimble up again.\r\n", ch);
    return;
  }

  if (!*argument) {
    send_to_char("Usage:\r\nKNIFETOSS <object> <target> <direction>\r\n", ch);
    return;
  }

  skip_spaces(&argument);
  half_chop(argument, knifestr, tempstr);
  half_chop(tempstr, targetstr, dirstr);

  if (!*knifestr || !*targetstr || !*dirstr) {
    send_to_char("Usage:\r\nKNIFETOSS <object> <target> <direction>\r\n", ch);
    return;
  }

  if (!(obj = get_obj_in_list_vis(ch,knifestr,ch->carrying))) {
    if (!(obj = get_object_in_equip_vis(ch, knifestr, ch->equipment, &objt))) {
      asend_to_char(ch,"You don't seem to have a \"%s\"...\r\n",knifestr);
      return;
    }
  }

  if (GET_OBJ_TYPE(obj) != ITEM_DAGGER) {
    send_to_char("That's not a tossable weapon!\r\n", ch);
    return;
  }
  
  if (GET_WOLFKIN(ch))
    bonus = 1;
  
  if (GET_BONDED(ch) && !(GET_BONDED(ch) & (1 << 30)))
    bonus = 2;

  for (i = 0; i < NUM_OF_DIRS; i++) /* Check if shooter typed in a correct dir */
    if(!strcasecmp(dirs[i], dirstr)) {
      dir = i;
      dirfound = TRUE;
    }

  if (dirfound){
    distance = 0;
    fromroom = ch->in_room;
    for (i=1; i < 4 + bonus; i++) {
      if (!target)
        if (CAN_GO(ch, dir)){
          temproom = ch->in_room;
          char_from_room(ch);               /* Ugly, I know. But easy :) */
          char_to_room(ch, world[temproom].dir_option[dir]->to_room);
          target = get_char_room_vis(ch, targetstr);
          distance++;
        }
    }

    targetroom = ch->in_room;
    char_from_room(ch);
    char_to_room(ch, fromroom);
    if (!target)     {
      send_to_char("Sorry, can't find that person.\r\n", ch);
      return;
    }
  } else {
    send_to_char("Which way is this unfortunate target, really? \r\n", ch);
    return;
  }

  if (ROOM_FLAGGED(target->in_room, ROOM_PEACEFUL)) {
    send_to_char("Tsk-Tsk.  No tossing daggers into Peaceful rooms!\r\n", ch);
    return;
  }

  result = generic_result_mod(ch,SKILL_KNIFETOSS,NULL,FALSE,0);
      
  if ((TOTAL_FUCKUP == result) || (TOTAL_FAILURE == result)) {
    damage(ch, target, 0, SKILL_KNIFETOSS);
    sprintf(buf, "Your dagger ends up in the dirt near %s.\r\n", GET_NAME(target));
    send_to_char(buf, ch);
    remember(target, ch);
    obj_from_char(obj);
    obj_to_room(obj,target->in_room);
  } else {
    dam = GET_OBJ_VAL(obj, 0);
    remember(target, ch);
    obj_from_char(obj);
    damage(ch, target, dam, SKILL_KNIFETOSS);
    update_pos(ch);
  }

  if (dam<3000) {
    wait=1;
  } else {
    if (dam<5000) {
      wait=2;
    } else {
      if (dam < 7000) {
        wait=3;
      } else {
        if (dam < 15000) {
          wait=4;
        } else {
          wait=5;
        }
      }
    }
  }  

  af.type = SKILL_KNIFETOSS;
  af.duration = wait;
  af.modifier = 0;
  af.location = APPLY_NONE;
  af.bitvector3 = AFF3_NOKNIFETOSS;
  affect_to_char(ch, &af);

}

ACMD(do_markedmap)
{
  char arg1[100], arg2[100];
  int num, i;

  if (!GET_SKILL(ch, SKILL_MARKEDMAP)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }

  if (!can_use_gskill(ch, SKILL_MARKEDMAP)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }

  skip_spaces(&argument);
  two_arguments(argument, arg1, arg2);
  if (!*arg1) {
    send_to_char("USAGE:\r\n\r\n"
                 "  MARKEDMAP #        - Go to room in slot #\r\n"
                 "  MARKEDMAP SHOW     - View your marked rooms\r\n"
                 "  MARKEDMAP ADD #    - Add current room to slot #\r\n"
                 "  MARKEDMAP REMOVE # - Remove slot #\r\n", ch);
    return;
  }
  if (!strcasecmp("show", arg1)) {
    for (i=0;i<10;i++) {
      if (GET_MAPPED(ch, i)) {
          sprintf(buf, "&W%2d) &n%s&n\r\n", i+1, world[real_room(GET_MAPPED(ch, i))].name);
      } else {
        sprintf(buf, "&W%2d) &nNot Set&n\r\n", i+1);
      }
      send_to_char(buf, ch);
    }        
    return;
  } else if (!strcasecmp("add", arg1)) {
      if (!is_number(arg2)) {
        send_to_char("Which position do you wish to add the current room to?\r\n", ch);
        return;
      } else {
        num=atoi(arg2);
        if (num<1 || num > 10) {
          send_to_char("Enter a valid number, 1-10.\r\n", ch);
          return;
        } else {
          if (!ROOM_FLAGGED(ch->in_room, ROOM_GODROOM)) {
            sprintf(buf, "You scribe on your map, making note of your position for future reference.\r\n");
            send_to_char(buf, ch);
            GET_MAPPED(ch, num-1)=world[ch->in_room].number;
            return;
          } else {
            send_to_char("Sorry, You can't map your way back to an IMM room.\r\n", ch);
            return;
          }
        }
      }

  } else if (!strcasecmp("remove", arg1)) {
      if (!is_number(arg2)) {
        send_to_char("Which entry do you want to remove?\r\n", ch);
        return;
      } else {
        num=atoi(arg2);
        if (num<1 || num > 10) {
          send_to_char("Enter a valid number, 1-10.\r\n", ch);
          return;
        } else {
          if (GET_MAPPED(ch, num-1)) {
            GET_MAPPED(ch, num-1)=0;
            sprintf(buf, "Entry #%d removed from your map.\r\n", num);
          } else {
            sprintf(buf, "Entry #%d was already empty!\r\n", num);
          }
          send_to_char(buf, ch);
          return;
        }
      }
  } else {
      if (!is_number(arg1)) {
        send_to_char("Unknown option.\r\n", ch);
        return;
      } else {
        num=atoi(arg1);
        if (num<1 || num > 10) {
          send_to_char("Enter a valid number, 1-10.\r\n", ch);
          return;
        } else {
          if (GET_MAPPED(ch, num-1)) {

            if (SESS_FLAGGED(ch, SESS_EAVESDROP)) {
              send_to_char("You move away from where you were and can no longer hear soft voices.\r\n", ch);
              (void)SESS_TOG_CHK(ch, SESS_EAVESDROP);
              rem_eavesdropper(ch, GET_EAVESDROPROOM(ch));
              GET_EAVESDROPROOM(ch) = 0;
            }

            send_to_char("You pull out your map and make your way to your destination.\r\n", ch);
            sprintf(buf, "$n pulls out a detailed map and makes $s way to $s destination.");
            act(buf, TRUE, ch, 0, 0, TO_ROOM);
            char_from_room(ch);
            char_to_room(ch, real_room(GET_MAPPED(ch, num-1)));
            sprintf(buf, "$n arrives and then rolls up a map before storing it in a wooden case.");
            act(buf, TRUE, ch, 0, 0, TO_ROOM);
            GET_POS(ch) = POS_STANDING;
            look_at_room(ch, 0);
          } else {
            send_to_char("You don't have a destination set there!\r\n", ch);
          }
          return;
        }
      }
  }
}

ACMD(do_eavesdrop)
{
  char dirstr[MAX_INPUT_LENGTH]="\0";
  struct room_data *rm;
  //struct affected_type af;
  int i, dir = 0, dirfound = 0;
  long temproom;

  skip_spaces(&argument);
  one_argument(argument, dirstr);

  if (!GET_SKILL(ch, SKILL_EAVESDROP)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }

  if (!can_use_gskill(ch, SKILL_EAVESDROP)) {
    send_to_char("Huh?!?\r\n", ch);
    return;
  }

  if (SESS_FLAGGED(ch, SESS_EAVESDROP)) {
    send_to_char("You move away from where you were and can no longer hear soft voices.\r\n", ch);
    (void)SESS_TOG_CHK(ch, SESS_EAVESDROP);
    rem_eavesdropper(ch, GET_EAVESDROPROOM(ch));
    GET_EAVESDROPROOM(ch) = 0;
    return;
  } 

  if (!*dirstr) {
    send_to_char("USAGE:\r\n\r\n  EAVESDROP <direction>\r\n", ch);
    return;
  }

  for (i = 0; i < NUM_OF_DIRS; i++)
    if(!strcasecmp(dirs[i], dirstr)) {
      dir = i;
      dirfound = TRUE;
    }

  if (dirfound){
        if (CAN_GO(ch, dir)){
          temproom = ch->in_room;
          char_from_room(ch);               /* Ugly, I know. But easy :) */

          if( ROOM_FLAGGED( world[temproom].dir_option[dir]->to_room, ROOM_PRIVATE ) ) {
            if( GET_REAL_LEVEL( ch ) < LVL_GRGOD ) {
              if( number_of_players_in_room( world[temproom].dir_option[dir]->to_room ) > 1 ) {
                send_to_char( "That room is private right now.\r\n", ch );
                return;
              }
            }
          }

          char_to_room(ch, world[temproom].dir_option[dir]->to_room);
          rm = &world[temproom];

          add_eavesdropper(ch, ch->in_room, 1);
          GET_EAVESDROPROOM(ch) = (ch->in_room);
          (void)SESS_TOG_CHK(ch, SESS_EAVESDROP);
          
          char_from_room(ch);
          char_to_room(ch, temproom);
          send_to_char("You position yourself so you can listen in.\r\n", ch);
          return;
        } else {
          send_to_char("You're unable to Eavesdrop there!\r\n", ch);
          return;
        }
  } else {
    send_to_char("Where?\r\n", ch);
    return;
  }
}
