/******************************************************************************
 * $Id: newbiegoto.c,v 1.3 2002/10/31 02:58:48 fang Exp $
 *   
 * File: newbiegoto.c
 *
 * Patched: Tue Oct 15 2002 by Darren Benham (fischer@thepics.org)
 * Author: Drathus
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 *   $Log: newbiegoto.c,v $
 *   Revision 1.3  2002/10/31 02:58:48  fang
 *   fisch: added "newbiegoto <newbie name>" for newbie helpers to the new
 *   newbiegoto command.
 *
 *   Revision 1.2  2002/10/16 17:05:05  fang
 *   fischer: added to mud
 *
 *
 */

#include  "act.h"
#include  "comm.h"
#include  "structs.h"
#include  "interpreter.h"
#include  "utils.h"
#include  "handler.h"
#include  "db.h"
#include  "magic.h"


ACMD(do_newbiegoto)
{
  sh_int rnm;
  int count=0, found=0;
  char *ch_mss[]={"neutral","menacing","golden"}, tpbuf[200];

  skip_spaces(&argument);
  
  if ((GET_LEVEL(ch) > 20) && (GET_LEVEL(ch) < LVL_IMMORT) &&
    !IS_SET(ch->player_specials->saved.command_groups, CG_NEWBIE))
  {
    send_to_char("Sorry, you can't use this command.\r\n", ch);
    return;
  }

  if ((GET_POS(ch) == POS_FIGHTING) || FIGHTING(ch))
  {
    send_to_char("Not while fighting!", ch);
    return;
  }

  if (!*argument) {
    send_to_char("&cYou are able to travel to the following locations:&n\r\n", ch);
    for (rnm=0;rnm<=top_of_world;rnm++) {
      if (ROOM_FLAGGED(rnm, ROOM_NEWBIEGOTO)) {
        count++;
        sprintf(buf, "&c%3d&n) %-30s\r\n", count, world[rnm].name);
        send_to_char(buf, ch);
      }
    }
    return;
  } 
  else
  {
    if (!is_number(argument)) 
    {
      struct char_data	*victim;
      sh_int  location;
      if( !IS_SET(ch->player_specials->saved.command_groups, CG_NEWBIE)
	&& !IS_IMMORTAL( ch ) )
      {
	send_to_char("Enter the number of the location to travel to.\r\n", ch);
	return;
      }
      if( !(victim = get_char_vis( ch, argument ) ) )
      {
	send_to_char( "No such newbie.\r\n", ch );
	return;
      }
      if( GET_LEVEL( victim ) > 20 )
      {
	send_to_char( "Your target is not a newbie.\r\n", ch );
	return;
      }
      if( (location=find_target_room( ch, argument ) ) < 0 )
      {
	send_to_char( "A strong bug occured.  Alert a WEAVER, please.\r\n", ch );
	return;
      } 
      if(ROOM_FLAGGED(location,ROOM_PRIVATE)) 
      { 
	if(GET_REAL_LEVEL(ch)<LVL_GRGOD) 
	{ 
	  if(number_of_players_in_room(location)>1) 
	  { 
	    send_to_char("That room is private right now.\r\n", ch); 
	    return; 
	  } 
	} 
      } 

      if( POOFOUT(ch) ) sprintf(buf, "%s", POOFOUT(ch)); 
      else strcpy(buf, "$n disappears in a puff of smoke."); 
      act(buf, TRUE, ch, 0, 0, TO_ROOM); 
      char_from_room(ch); 

      char_to_room(ch, location); 
      if( POOFIN(ch) ) sprintf(buf, "%s", POOFIN(ch)); 
      else strcpy(buf, "$n appears with an ear-splitting bang."); 
      act(buf, TRUE, ch, 0, 0, TO_ROOM); 
      
      GET_POS(ch) = POS_STANDING;
      look_at_room(ch, 0);
    } 
    else 
    {
      for (rnm=0;rnm<=top_of_world;rnm++) 
      {
        if (ROOM_FLAGGED(rnm, ROOM_NEWBIEGOTO))
	{
          count++;
          if (count==atoi(argument))
	  {
            found=1;
            break;
          }
        }
      }
      if (found==0) {
        send_to_char("Enter the number of the location to travel to.\r\n", ch);
      } else {
        if (SESS_FLAGGED(ch, SESS_EAVESDROP)) {
          send_to_char("You move away from where you were and can no longer hear soft voices.\r\n", ch);
          (void)SESS_TOG_CHK(ch, SESS_EAVESDROP);
          rem_eavesdropper(ch, GET_EAVESDROPROOM(ch));
          GET_EAVESDROPROOM(ch) = 0;
        }
        act("$n goes elsewhere.", FALSE, ch, 0, 0, TO_ROOM);
        send_to_char("You make your way somewhere else.\r\n", ch);
        char_from_room(ch);
        char_to_room(ch, rnm);
        look_at_room(ch, 0);
        if(ROOM_FLAGGED(ch->in_room,ROOM_NOMAGIC) && IS_SET(PRF_FLAGS(ch),PRF_GRASPING))
        {
          send_to_char("Suddenly you lose your grip of the One Power! Sorry! You cannot channel in this room!\r\n",ch);
          sprintf(tpbuf,"The %s aura around $n suddenly vanishes!",ch_mss[(int)GET_SEX(ch)]);
          act(tpbuf,FALSE,ch,NULL,NULL,TO_OPUSERSS);
          remove_char_maintained(ch);
          remove_room_maintained(ch);
          REMOVE_BIT(PRF_FLAGS(ch),PRF_GRASPING);
        }
        act("$n has arrived.", FALSE, ch, 0, 0, TO_ROOM);
      }
      return;
    }
  }
}


