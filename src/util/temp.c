/* ************************************************************************
*  file: temp.c                                         Part of CircleMUD * 
*  Usage: fix new ogiers                                                  *
*  All Rights Reserved                                                    *
*  Copyright (C) 1992, 1993 The Trustees of The Johns Hopkins University  *
************************************************************************* */

#include "../conf.h"
#include "../sysdep.h"

#include "../structs.h"
#include "../utils.h"
#include "../class.h"

void purge(char *filename)
{
  FILE *fl;
  FILE *outfile;
  struct char_file_u player;
  int ogier, guild;

  if (!(fl = fopen(filename, "r+"))) {
    printf("Can't open %s.", filename);
    exit(0);
  }
  outfile = fopen("players.new", "w");
  printf("Changing ogier status: \n");

  for (;;) {
    fread(&player, sizeof(struct char_file_u), 1, fl);
    if (feof(fl)) {
      fclose(fl);
      fclose(outfile);
      puts("Done.");
      exit(0);
    } 
    guild = player.player_specials_saved.guild;
    if (guild == 13) {
      ogier = player.player_specials_saved.guildpos;
      player.player_specials_saved.ints[32] = ogier + 1;
      player.player_specials_saved.guild = 0;
      player.player_specials_saved.guildpos = 0;
      printf("Changing %s to new status: %d\r\n", player.name, ogier);
    }
    else
      player.player_specials_saved.ints[32] = 0;
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
