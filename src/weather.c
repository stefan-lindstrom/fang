/******************************************************************************
 * $Id: weather.c,v 1.7 2003/08/08 03:37:20 fang Exp $
 *   
 * File: weather.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: weather.c,v 
 *   Revision 1.2  2002/07/28 13:56:05  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.3  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/02/21 14:00:20  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* ************************************************************************
*   File: weather.c                                     Part of CircleMUD *
*  Usage: functions handling time and the weather                         *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "conf.h"
#include "sysdep.h"

#include "weather.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "handler.h"
#include "interpreter.h"
#include "db.h"


void weather_and_time(int mode)
{
  another_hour(mode);
  if (mode)
    weather_change();
}


void another_hour(int mode)
{
  struct time_info_data sanity_check;

  time_info.hours++;

  if (mode) {
    switch (time_info.hours) {
    case 5:
      weather_info.sunlight = SUN_RISE;
      if (!time_restrict)
       send_to_outdoor("The sun rises, casting an amber glow down from the Spine of the World.\r\n");
      break;
    case 6:
      weather_info.sunlight = SUN_LIGHT;
      if (!time_restrict) 
       send_to_outdoor("The Creator brushes away the stars to make the day.\r\n");
      break;
    case 21:
      weather_info.sunlight = SUN_SET;
      if (!time_restrict)
       send_to_outdoor("The sun slowly sinks below the watery horizon of the Aryth Ocean.\r\n");
      break;
    case 22:
      weather_info.sunlight = SUN_DARK;
      if (!time_restrict)
       send_to_outdoor("The Creator pulls the blanket of night across the sky.\r\n");
      break;
    default:
      break;
    }
  }

  /* Redone by Elkins.. lets see if this fixes it */
  /* New changes by Fredrik to Randify the calendar */
  if (time_info.hours > 23)
  {
    time_info.hours = 0;
    time_info.day++;
  }
  if (time_info.day > 27) // 28 days in each Rand month
  {
    time_info.day = 0;
    time_info.month++;
  }
  if (time_info.month > 12) // 13 months in each Rand year
  {
    time_info.month = 0;
    time_info.year++;
  }

  sanity_check=mud_time_passed(time(0),650336715);
  if (abs(time_info.hours-sanity_check.hours) > 1 ||
      abs(time_info.month-sanity_check.month) > 1 ||
      abs(time_info.year-sanity_check.year) > 1)
  {
    reset_time();
  }
}


void weather_change(void)
{
  int diff, change;
  if ((time_info.month > 8) && (time_info.month < 17))
    diff = (weather_info.pressure > 985 ? -2 : 2);
  else
    diff = (weather_info.pressure > 1015 ? -2 : 2);

  weather_info.change += (dice(1, 4) * diff + dice(2, 6) - dice(2, 6));

  weather_info.change = MIN(weather_info.change, 12);
  weather_info.change = MAX(weather_info.change, -12);

  weather_info.pressure += weather_info.change;

  weather_info.pressure = MIN(weather_info.pressure, 1040);
  weather_info.pressure = MAX(weather_info.pressure, 960);

  change = 0;

  switch (weather_info.sky) {
  case SKY_CLOUDLESS:
    if (weather_info.pressure < 990)
      change = 1;
    else if (weather_info.pressure < 1010)
      if (dice(1, 4) == 1)
    change = 1;
    break;
  case SKY_CLOUDY:
    if (weather_info.pressure < 970)
      change = 2;
    else if (weather_info.pressure < 990)
      if (dice(1, 4) == 1)
    change = 2;
      else
    change = 0;
    else if (weather_info.pressure > 1030)
      if (dice(1, 4) == 1)
    change = 3;

    break;
  case SKY_RAINING:
    if (weather_info.pressure < 970)
      if (dice(1, 4) == 1)
    change = 4;
      else
    change = 0;
    else if (weather_info.pressure > 1030)
      change = 5;
    else if (weather_info.pressure > 1010)
      if (dice(1, 4) == 1)
    change = 5;

    break;
  case SKY_LIGHTNING:
    if (weather_info.pressure > 1010)
      change = 6;
    else if (weather_info.pressure > 990)
      if (dice(1, 4) == 1)
    change = 6;

    break;
  default:
    change = 0;
    weather_info.sky = SKY_CLOUDLESS;
    break;
  }

  switch (change) {
  case 0:
    break;
  case 1:
    send_to_outdoor("The sky starts to get cloudy.\r\n");
    weather_info.sky = SKY_CLOUDY;
    break;
  case 2:
    send_to_outdoor("It starts to snow.\r\n");
    weather_info.sky = SKY_RAINING;
    break;
  case 3:
    send_to_outdoor("The clouds disappear.\r\n");
    weather_info.sky = SKY_CLOUDLESS;
    break;
  case 4:
    send_to_outdoor("Lightning starts to show in the sky.\r\n");
    weather_info.sky = SKY_LIGHTNING;
    break;
  case 5:
    send_to_outdoor("The snow stops.\r\n");
    weather_info.sky = SKY_CLOUDY;
    break;
  case 6:
    send_to_outdoor("The lightning stops.\r\n");
    weather_info.sky = SKY_RAINING;
    break;
  default:
    break;
  }
}

static char *__sun[] = { "sunrise", "sunup", "sunset", "sundown" };
ACMD(do_tset)
{
   one_argument(argument, arg);
   if (!*arg) {
     asend_to_char(ch,"Usage: tset { %s | %s | %s |%s }\r\n", *__sun,*(__sun+1),
							      *( __sun+2), *(__sun+3));
     return;
  }

  if (!strcasecmp(arg,*__sun)) {    
    time_info.hours = 5;
    weather_info.sunlight = SUN_RISE;
  } else if (!strcasecmp(arg,*(__sun+1))) {
    time_info.hours = 6;
    weather_info.sunlight = SUN_LIGHT;
  } else if (!strcasecmp(arg,*(__sun+2))) {
    time_info.hours = 21;
    weather_info.sunlight = SUN_SET;
  } else if (!strcasecmp(arg,*(__sun+3))) {
    time_info.hours = 22;
    weather_info.sunlight = SUN_DARK;
  } else {
    asend_to_char(ch,"Usage: tset { %s | %s | %s |%s }\r\n", *__sun,*(__sun+1),*( __sun+2), *(__sun+3));
    return;
  }
  asend_to_char(ch,"You have set the time to %d hrs\r\n",time_info.hours);
  axlog(SYS_GOD, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)),TRUE, "%s has set mud hour to %d",GET_RNAME(ch),time_info.hours);
}

ACMD(do_wset)
{
  static char *weather_type[] = {
    "clear",
    "cloudy",
    "raining",
    "lightning" };
  int weather;
  const char *usage = "Usage: wset { clear | cloudy | raining | lightning }\r\n"
                      "\"raining\" results in precipitation appropriate for the temperature.\r\n";
  
  one_argument(argument, arg);

  if (!*arg) {
    send_to_char(usage, ch);
    return;
  }

  if (is_abbrev(arg, "clear"))
    weather = 0;
  else if (is_abbrev(arg, "cloudy"))
    weather = 1;
  else if (is_abbrev(arg, "raining"))
    weather = 2;
  else if (is_abbrev(arg, "lightning"))
    weather = 3;
  else weather = 4;

  if (weather == weather_info.sky) {
    sprintf(buf, "But it is already %s.\r\n", weather_type[weather]);
    send_to_char(buf, ch);
    return;
  }

  switch (weather) {
    case 0:
      weather_info.sky = SKY_CLOUDLESS;
      weather_info.pressure = 1040;
      break;
    case 1:
      weather_info.sky = SKY_CLOUDY;
      weather_info.pressure = 1002;
      break;
    case 2:
      weather_info.sky = SKY_RAINING;
      weather_info.pressure = 980;
      break;
    case 3:
      weather_info.sky = SKY_LIGHTNING;
      weather_info.pressure = 970;
      break;
    default:
      break;
  }

  if (weather == 4) {
    send_to_char(usage, ch);
    return;
  } else {
    weather_change();
    sprintf(buf, "%s has changed the weather to '%s'", GET_NAME(ch),
      weather_type[weather]);
    xlog(SYS_GOD, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), buf, TRUE);
    send_to_char("Done.\r\n", ch);
  }
}

