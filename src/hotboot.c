/******************************************************************************
 * $Id: hotboot.c,v 1.4 2003/06/17 10:30:05 fang Exp $
 *   
 * File: hotboot.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: hotboot.c,v 
 *   Revision 1.2  2002/07/28 13:56:04  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.19  2002/07/16 18:59:12  fang
 *   Drath: Added saving to timed reboots
 *
 *   Revision 1.18  2002/06/11 16:10:26  fang
 *   Maz: New TOGGLE screen.  I was kinda bored.
 *
 *   Revision 1.17  2002/06/10 15:44:42  fang
 *   Maz: AutoReboot warning set to 1 min.
 *
 *   Revision 1.16  2002/06/07 18:24:26  fang
 *   Maz: NoSpam room flag.
 *
 *   Revision 1.15  2002/06/05 12:05:27  fang
 *   Maz: Added 1 min warning for autoreboots
 *
 *   Revision 1.14  2002/06/03 16:25:25  fang
 *   Mazrim: Enabled autoREBOOT on an 8 hour cycle to keep the memory usage of the mud down.
 *
 *   Revision 1.13  2002/05/27 10:52:49  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.12  2002/05/12 23:50:46  fang
 *   Added in IGNORE system, and removed AutoHotboots until the cap can be fixed for good. -Maz
 *
 *   Revision 1.11  2002/04/13 15:53:14  fang
 *   *** empty log message ***
 *
 *   Revision 1.10  2002/04/11 16:50:25  fang
 *   Disabling autohotboot until cap fixed.
 *
 *   Revision 1.9  2002/03/04 14:03:31  fang
 *   Fixed timed hotboot spam.
 *
 *   Revision 1.8  2002/03/03 19:11:13  fang
 *   *** empty log message ***
 *
 *   Revision 1.7  2002/03/03 19:07:52  fang
 *   *** empty log message ***
 *
 *   Revision 1.6  2002/03/03 18:57:06  fang
 *   *** empty log message ***
 *
 *   Revision 1.5  2002/03/03 18:44:52  fang
 *   Hotboot Spam.
 *
 *   Revision 1.4  2002/03/03 13:11:12  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/02/26 10:20:56  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.17  2002/01/24 21:33:17  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.16.2.1  2002/01/22 20:50:59  mud
 *
 *
 *   Removed boats and info system. Obsolete, unused code with no value.
 *   Fixed bug in portal_stone SPECIAL. Now checks that a room exists
 *   before attempting to transf chars there. :)
 *
 *   Revision 1.16  2001/11/07 13:22:49  mud
 *   Changed killing of the parent process after hotboot to SIGHUP instead of SIGKILL... cleaner and more efficient I think...and will hopefully get rid of that nasty memory bug noticed between hotboots.
 *
 *   Revision 1.15  2001/08/10 13:48:36  mud
 *   *** empty log message ***
 *
 *   Revision 1.14  2001/05/17 11:21:09  mud
 *   Final residues of old pfile code should be swept away by now. No cleansing of the structures and such things though
 *
 *   Revision 1.13  2001/03/21 10:43:14  mud
 *   More bugfixes to xml char save code
 *
 *   Revision 1.12  2000/11/16 15:43:05  mud
 *   *** empty log message ***
 *
 *   Revision 1.11  2000/09/04 03:34:39  mud
 *   Warnings removed...
 *
 *   Revision 1.10  2000/08/17 00:13:04  mud
 *   *** empty log message ***
 *
 *   Revision 1.9  2000/08/17 00:02:39  mud
 *   *** empty log message ***
 *
 *   Revision 1.8  2000/08/16 23:52:43  mud
 *   *** empty log message ***
 *
 *   Revision 1.7  2000/08/16 10:51:13  mud
 *   Hotboot changes...
 *
 *   Revision 1.6  2000/08/14 19:11:56  mud
 *   The new guild system.
 *
 *   Revision 1.5  2000/05/27 08:18:00  mud
 *   Typo fix. -Tesla
 *
 *   Revision 1.4  2000/04/29 15:38:15  mud
 *   Fixed disguise so that a darkfriend will still be a darkfriend after
 *   disguise.
 *   Added hotboot check as well.
 *
 *   Revision 1.3  2000/04/09 20:34:04  mud
 *   Changed player obj saving to XML format.
 *
 *   Revision 1.2  2000/02/21 14:00:17  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "comm.h"
#include "db.h"
#include "structs.h"
#include "utils.h"
#include "act.h"
#include "handler.h"
#include "mail.h"
#include "alias.h"
#include "class.h"
#include "config.h"
#include "xmlobjsave.h"
#include "xmlcharsave.h"
#include "hotboot.h"
#include "guild.h"

int recover_from_hotboot(void)
{
  char line[2048],name[1024], host[HOST_LENGTH+1];
  struct descriptor_data *desc = NULL;
  int descr,inroom,temptime, file;
  FILE *rec = fopen(RECOV_FILE,"r");
  struct obj_data *wagon = NULL;
  descriptor_list = NULL; // We shouldn't have any in our list. 

  if (!rec) {
    mlog("Recovering from hotboot failed...exiting.");
    exit(1);
  }

  fscanf(rec, "%d\n", &temptime);
  boot_time = temptime;
  while (fgets(line,2048,rec) && !feof(rec)) {
    sscanf(line,"%d %s %d %s\n",&descr,name,&inroom, host);
    new_descriptor(descr);
    // Now, our descriptor_data should be first, i.e.
    desc = descriptor_list;
    desc->connected = CON_PLAYING;
    
    // Time to load character from file, and set him to the room he/she was
    // In before hotboot...

    if (NULL != (desc->character = xmlload_char(name))) {
      strncpy(desc->host, host, HOST_LENGTH);
      desc->character->desc = desc;
      REMOVE_BIT(PLR_FLAGS(desc->character),PLR_WRITING | PLR_MAILING | PLR_CRYO);
      reset_char(desc->character);

      if (GET_INVSTART(desc->character))
        GET_INVIS_LEV(desc->character) = GET_INVSTART(desc->character);

      if (NOWHERE == real_room(inroom)) 
        inroom = (IS_IMMORTAL(desc->character) ? r_immort_start_room : r_mortal_start_room);
      else 
        inroom = real_room(inroom);

      if (PLR_FLAGGED(desc->character,PLR_FROZEN))
        inroom = r_frozen_start_room;
      
      char_to_room(desc->character,inroom);

      desc->character->next = character_list;
      character_list = desc->character;
      file = (IS_AFFECTED(desc->character, AFF_DREAMING))?DREAM_FILE:CRASH_FILE;
      read_XML_playerfile(desc->character, file);
      save_char(desc->character,inroom);
      save_XML_playerfile(desc->character, file);

      fetch_char_guild_data(desc->character);

      GET_ID(desc->character) = max_id++;

      act("$n has returned to the game.", TRUE, desc->character, 0, 0, TO_ROOM);
      send_to_char("Joy! You were succesfully recovered from hotboot! :)\r\n",desc->character);

      if (GET_WAGON(desc->character)) {
        wagon = read_object(GET_WAGON(desc->character), VIRTUAL);
        obj_to_room(wagon, desc->character->in_room);
        desc->character->pulling = wagon;
        wagon->pulled_by = desc->character;
      }
      STATE(desc) = CON_PLAYING;
     
      if (!GET_LEVEL(desc->character)) {
        do_start(desc->character);
        do_newbie_eq(desc->character);  
        send_to_char(START_MESSG, desc->character);
      }

      look_at_room(desc->character, 0);

      if (has_mail(GET_IDNUM(desc->character)))
        send_to_char("You have mail waiting.\r\n", desc->character);

      desc->prompt_mode = 1;
      read_aliases(desc->character);
      SPEAKING(desc->character) = 0;
      desc->character->master = NULL;

      if (IS_AFFECTED2(desc->character,AFF2_LONGSLEEP)) {
        GET_POS(desc->character) = POS_SLEEPING;
      }
      REMOVE_BIT(PLR_FLAGS(desc->character),PLR_WRITING | PLR_MAILING);
      // Finally, everything should be setup, at last. :)
    }
    else {
      alog("hotboot: failed to restore char %s, exiting...",name);
      exit(1);
    } 
  }
  fclose(rec);
  unlink(RECOV_FILE);
  hotboot = 0; // We're through....
  rec = fopen(CONTROLL,"w");
  fclose(rec);

  kill(headpid,1);

  return 0;
}

int do_hotboot(char *exec)
{
  struct stat sbuf;
  char buf[2048],portbuf[100],arg[100];
  FILE *rec;
  struct descriptor_data *desc,*desc2;
  pid_t me;
  int temptime = 0;

  if (NULL == exec || !*exec) {
    sprintf(buf,"%s/%s",STD_PATH,STD_FILE);
  } else {
    sprintf(buf,"%s/%s",STD_PATH,exec);
  }
  sprintf(portbuf,"%d",port_used);

  sprintf(arg, "-h%d", mother_desc);

  if (stat(buf,&sbuf) < 0) {
    alog("do_hotboot:The specified executable %s doesn't seem to exist!",buf);
    return -1;
  }

  if (NULL == (rec = fopen(RECOV_FILE,"w"))) {
    mlog("do_hotboot: Couldn't open recovery file!");
    return -1;
  }

  temptime = boot_time;
  fprintf(rec, "%d\n", temptime);
  for (desc = descriptor_list; desc; desc = desc2) {
    char *bt = "\r\nHotboot in progress, please be patient.\r\n";

    desc2 = desc->next;
    if (STATE(desc) != CON_PLAYING) {
      char *str = "\r\nWe're hotbooting right now....please come back in a few seconds.\r\n"; 
      write(desc->descriptor,str,strlen(str));
      close_socket(desc);
      continue;
    }
    if (GET_WAS_IN(desc->character) != NOWHERE) {
      if (IN_ROOM(desc->character) != NOWHERE)
        char_from_room(desc->character);
      char_to_room(desc->character, GET_WAS_IN(desc->character));
    }
    fprintf(rec,"%d %s %d %s\n",desc->descriptor,GET_RNAME(desc->character),world[desc->character->in_room].number, desc->host);
    do_save(desc->character,"",0,0);
    write(desc->descriptor,bt,strlen(bt));

//    send_to_char("Hotboot in progress, please be patient.\r\n",desc->character);
/*    flush_queues(desc);*/
  }
  fclose(rec);

  // Everything saved, now for the fun (fork'in) parts. :)
  switch(me = fork()) {
    case -1:
      mlog("do_hotboot: fork() failed....");
      return -1;
      break;
    case 0:
      // We're the child. 
      sprintf(arg,"%s-%d",arg,getppid());
      execl(buf,(NULL == exec ? STD_FILE : exec),arg,"-q",portbuf,NULL);
      // If we return from an execl, we've run into major **** trouble. :)
      kill(getppid(),9); // Try with sigkill! 
      mlog("hotboot: Major f**kup in execl()!");
      exit(-1);
      break;
    default:
      // We're the parent!
      hotboot = 1;
      break;
  }

  return -1;
}

void hotboot_check(struct char_data *ch) {
  struct descriptor_data *d;
  int count = 0;

  sprintf(buf, "The following people are not in a position for a hotboot:\r\n");
  for (d = descriptor_list; d; d = d->next) {
    if (STATE(d) != CON_PLAYING || !d->character ||
    PLR_FLAGGED(d->character, PLR_MAILING) ||
    PLR_FLAGGED(d->character, PLR_WRITING)) {
      sprintf(buf, "%s%s\r\n", buf, (d->character ? GET_NAME(d->character) :
      "<unknown>"));
      count++;
    }
  }
  if (!count)
    sprintf(buf, "%sEveryone is ready to go!!!\r\n", buf);
  page_string(ch->desc, buf, 1);
}

void timed_hotboot() {
 
  double tmp;
  static int hotboot_processing = FALSE;
  struct descriptor_data *tch;
 
  if ((tmp = difftime(time(0), hotboot_time)) >= AUTO_BOOT_MSG && tmp < AUTO_BOOT_TIME && !hotboot_processing) {
    send_to_all("&R[&rAutomated System warning: &cThe mud will be automatically rebooted in exactly one minute!&R]&n\r\n");
    send_to_all("&R[&rAutomated System warning: &cYou WILL be saved prior to the reboot.&R]&n\r\n");
    hotboot_processing = TRUE;
  }
  else if (tmp > AUTO_BOOT_TIME && circle_shutdown != 1) {
    for (tch=descriptor_list; tch; tch=tch->next)
      if (!IS_NPC(tch->character)) {
        send_to_char("&WAutosaving ...", tch->character);
        do_save((tch->character), "", 0, 0);
        send_to_char("&WDone!&n\r\n", tch->character);
        send_to_char("&R[&CThe mud is being automatically rebooted! Be back soon...&R]&n\r\n", (tch->character));
      }

    touch("../.fastboot");
    circle_shutdown = circle_reboot = 1;

 /*   if (!stat(STD_PATH"/new.circle", &sbuf))
  *     do_hotboot("new.circle");
  *   else
  *     do_hotboot(NULL);
  */

   }
}











