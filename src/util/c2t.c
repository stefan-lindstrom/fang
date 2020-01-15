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
  struct char_special_data_saved *csave;
  struct player_special_data_saved *psave;
  struct char_ability_data *abil;
  struct char_point_data *pnts;
  int i;
  
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
    csave = &player.char_specials_saved;
    psave = &player.player_specials_saved;
    abil = &player.abilities;
    pnts = &player.points;

    fprintf(outfile,"%s \n%s~ \n%s~ \n%d %d %ld %d %d %d %s\n",
    player.name, player.title, player.description, player.sex, player.class,
    player.level, player.hometown, player.weight, player.height, player.pwd);
    
     /* The three variables below are the only ones interesting to convert */
    fprintf(outfile,"%ld %ld %ld\n", csave->alignment, csave->idnum, csave->act);
    
    /* Next struct - player_special_data_saved */
    
    for (i = 0; i < MAX_SKILLS; i ++)
      fprintf(outfile, "%d\n", psave->skills[i]);
    fprintf(outfile, "%ld %d %d %d %ld %d \n%s~ \n%s~ \n%s~ \n",
    psave->wimp_level, psave->freeze_level, psave->invis_level, psave->load_room,
    psave->pref, psave->bad_pws, psave->clubname, psave->clubrank, psave->servant);
    
    fprintf(outfile, "%d %d %d %d %d %ld %d %d %d %d %d %d %d \n",
    psave->air, psave->earth, psave->fire, psave->spirit, psave->water, psave->taint,
    psave->spark, psave->teachable, psave->guild, psave->guildpos, psave->darkfriend,
    psave->taveren, psave->true_power); 

    /*    
    fprintf(outfile, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
    psave->wolfkin, psave->trains, psave->nobility, psave->spare2, psave->race,
    psave->clan, psave->society, psave->drf_flags, psave->spare3, psave->ajah, psave->betting,
    psave->conditions[0], psave->conditions[1], psave->conditions[2]);
*/
    fprintf(outfile, "%d %d %d %d %d %d %d %d %d %d %d %d\n",
    psave->wolfkin, psave->trains, psave->nobility, psave->race,
    psave->clan, psave->society, psave->drf_flags, psave->ajah, psave->betting,
    psave->conditions[0], psave->conditions[1], psave->conditions[2]);
    
    for (i = 0; i < 200 ; i ++)
      fprintf(outfile, "%d\n", psave->bytes[i]);

/*    
    fprintf(outfile, "%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld \n",
    psave->spells_to_learn, psave->olc_zone, psave->bonded, psave->minion, 
    psave->powerpoints, psave->questpoints, psave->absorbation, psave->permanent,
    psave->mount_vnum, psave->spare9b, psave->spare10b);
*/
    fprintf(outfile, "%ld %ld %ld %ld %ld %ld %ld %ld \n",
    psave->spells_to_learn, psave->olc_zone, psave->bonded, psave->minion, 
    psave->powerpoints, psave->questpoints, psave->absorbation, psave->permanent);
    
    for (i = 0; i < 200; i ++)
      fprintf(outfile, "%ld \n", psave->ints[i]);
    
    fprintf(outfile, "%d %d %d %d %d %d \n", abil->str, abil->intel, abil->wis,
    abil->dex, abil->con, abil->cha);
    
    fprintf(outfile, "%d %d %d %d %d %d %d %ld %ld %ld %ld %ld \n",
    pnts->mana, pnts->max_mana, pnts->hit, pnts->max_hit, pnts->move, pnts->max_move,
    pnts->armor, pnts->gold, pnts->bank_gold, pnts->exp, pnts->hitroll, pnts->damroll);
    
    fprintf(outfile, "%ld %s \n* \n", player.last_logon, player.host);   
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
