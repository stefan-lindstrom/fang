/**************************************************************************
*  file:  c2t.c                                 Part of Dragons Fang MUD  *
*  Usage: Converting circle playerfiles to plain text ones for portability*
***************************************************************************/

#include "../conf.h"
#include "../sysdep.h"

#include "../structs.h"
#include "../utils.h"

void convert(char *filename)
{
  FILE *fl;
  FILE *outfile;
  struct char_file_u player;
  
  if (!(fl = fopen(filename, "r+"))) {
    printf("Can't open %s.", filename);
    exit(0);
  }
  outfile = fopen("players.conv", "w");  
  for (;;) {
    fread(&player, sizeof(struct char_file_u), 1, fl);
    if (feof(fl)) {
      fclose(fl);
      fclose(outfile);
      puts("Done.");
      exit(0);
    }  
    player.player_specials_saved.bonded = 0;
    player.player_specials_saved.minion = 0;
    fwrite(&player, sizeof(struct char_file_u), 1, outfile);
  }
}

int main(int argc, char *argv[])
{
  if (argc != 2)
    printf("Usage: %s playerfile-name\n", argv[0]);
  else
    convert(argv[1]);
  return 0;
}
