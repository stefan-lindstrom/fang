/* ************************************************************************
*  file:  showplay.c                                  Part of CircleMud   *
*  Usage: list a diku playerfile                                          *
*  Copyright (C) 1990, 1991 - see 'license.doc' for complete information. *
*  All Rights Reserved                                                    *
************************************************************************* */

#include "../conf.h"
#include "../sysdep.h"

#include "../structs.h"


void show(char *filename)
{
  char sexname;
  char classname[10];
  FILE *fl;
  struct char_file_u player;
  int num = 0;

  if (!(fl = fopen(filename, "r+"))) {
    perror("error opening playerfile");
    exit(1);
  }
  for (;;) {
    fread(&player, sizeof(struct char_file_u), 1, fl);
    if (feof(fl)) {
      fclose(fl);
      exit(1);
    }
    switch (player.class) {
    case CLASS_ADVENTURER:
      strcpy(classname, "Adv");
      break;
    case CLASS_SOLDIER:
      strcpy(classname, "Sol");
      break;
    case CLASS_CHANNELER:
      strcpy(classname, "Cha");
      break;
    case CLASS_WILDER:
      strcpy(classname, "Wil");
      break;
    case CLASS_THIEF:
      strcpy(classname, "Thi");
      break;
    case CLASS_CRAFTSMAN:
      strcpy(classname, "Cra");
      break;
    case CLASS_WISDOM:
      strcpy(classname, "Wis");
    default:
      strcpy(classname, "--");
      break;
    }

    switch (player.sex) {
    case SEX_FEMALE:
      sexname = 'F';
      break;
    case SEX_MALE:
      sexname = 'M';
      break;
    case SEX_NEUTRAL:
      sexname = 'N';
      break;
    default:
      sexname = '-';
      break;
    }

    printf("%5d. ID: %5ld (%c) [%2ld %s] %-16s %9ldg %9ldb\n", ++num,
	   player.char_specials_saved.idnum, sexname, player.level,
	   classname, player.name, player.points.gold,
	   player.points.bank_gold);
  }
}


int main(int argc, char **argv)
{
  if (argc != 2)
    printf("Usage: %s playerfile-name\n", argv[0]);
  else
    show(argv[1]);

  return 0;
}
