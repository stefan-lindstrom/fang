/* ************************************************************************
*  file: aqfix.c                                        Part of CircleMUD * 
*  Usage: Fixes autoquest variable clearing                               *
*  All Rights Reserved                                                    *
*  Copyright (C) 1992, 1993 The Trustees of The Johns Hopkins University  *
************************************************************************* */

#include "../conf.h"
#include "../sysdep.h"

#include "../structs.h"
#include "../utils.h"


void purge(char *filename)
{
  FILE *fl;
  FILE *outfile;
  struct char_file_u player;
  int i;

  if (!(fl = fopen(filename, "r+"))) {
    printf("Can't open %s.", filename);
    exit(0);
  }
  outfile = fopen("players.new", "w");
  printf("Deleting: \n");

  for (;;) {
    fread(&player, sizeof(struct char_file_u), 1, fl);
    if (feof(fl)) {
      fclose(fl);
      fclose(outfile);
      puts("Done.");
      exit(0);
    }
    for (i = 0; i < 10; i ++) 
      player.player_specials_saved.AutoQuest[i] = 0; /* Rensar AutoQuest */

    fwrite(&player, sizeof(struct char_file_u), 1, outfile);
  }
}



int main(int argc, char *argv[])
{
  if (argc != 2)
    printf("Usage: %s playerfile-name\n", argv[0]);
  else
    purge(argv[1]);

  return 0;
}
