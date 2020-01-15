/******************************************************************************
 * $Id: hotboot.h,v 1.2 2002/07/28 14:12:31 fang Exp $
 *   
 * File: hotboot.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: hotboot.h,v $
 *   Revision 1.2  2002/07/28 14:12:31  fang
 *   Changed reboot times to once a week /Fredrik.
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.10  2002/07/16 18:59:12  fang
 *   Drath: Added saving to timed reboots
 *
 *   Revision 1.9  2002/06/11 09:04:38  fang
 *   *** empty log message ***
 *
 *   Revision 1.8  2002/06/10 15:44:42  fang
 *   Maz: AutoReboot warning set to 1 min.
 *
 *   Revision 1.7  2002/06/05 12:05:30  fang
 *   Maz: Added 1 min warning for autoreboots
 *
 *   Revision 1.6  2002/06/03 16:25:25  fang
 *   Mazrim: Enabled autoREBOOT on an 8 hour cycle to keep the memory usage of the mud down.
 *
 *   Revision 1.5  2002/03/05 03:53:11  fang
 *   *** empty log message ***
 *
 *   Revision 1.4  2002/03/04 14:23:36  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/03/03 13:11:12  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2000/04/29 15:38:15  mud
 *   Fixed disguise so that a darkfriend will still be a darkfriend after
 *   disguise.
 *   Added hotboot check as well.
 *
 *   Revision 1.2  2000/02/21 14:00:17  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#ifndef __HOTBOOT_H__
#define __HOTBOOT_H__

#define STD_PATH   "../bin"
#define STD_FILE   "circle"
#define RECOV_FILE "../.recov"
#define CONTROLL   "../.hotboot"
#define AUTO_BOOT_TIME	(7 * 24 * 60 * 60)
#define AUTO_BOOT_MSG	(AUTO_BOOT_TIME - (1 * 60))

extern int recover_from_hotboot(void);
extern int do_hotboot(char *exec);
extern void hotboot_check(struct char_data *ch);
 extern void timed_hotboot();

#endif

