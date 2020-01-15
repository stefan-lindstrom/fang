/**************************************************************************
*  file:  c2t.c                                 Part of Dragons Fang MUD  *
*  Usage: Converting plain text to circle playerfiles for portability     *
***************************************************************************/

#include "../conf.h"
#include "../sysdep.h"

#include "../structs.h"
#include "../utils.h"


struct char_file_u player;
  
char *fread_string(FILE * fl)
{
  char buf[MAX_STRING_LENGTH], tmp[MAX_STRING_LENGTH], *rslt;
  register char *point;
  int done = 0, length = 0, templength = 0;

  *buf = '\0';
  do {
    if (!fgets(tmp, 512, fl)) 
      exit(1);
    /* If there is a '~', end the string; else put an "\r\n" over the '\n'. */
    if ((point = strchr(tmp, '~')) != NULL) {
      *point = '\0';
      done = 1;
    } else {
      point = tmp + strlen(tmp) - 1;
      *(point++) = '\r';
      *(point++) = '\n';
      *point = '\0';
    } 

    templength = strlen(tmp);

    if (length + templength >= MAX_STRING_LENGTH) 
      exit(1);
    else {
      strcat(buf + length, tmp);
      length += templength;
    }
  } while (!done);

  /* allocate space for the new string and copy it */
  if (strlen(buf) > 0) {
    rslt = malloc(length + 1);
    strcpy(rslt, buf);
  } else
    rslt = NULL;

  return rslt;
}

int get_line(FILE * fl, char *buf)
{
  char temp[MAX_STRING_LENGTH];
  int lines = 0;

  do {
    lines++;
    fgets(temp, 256, fl);
    if (*temp)
      temp[strlen(temp) - 1] = '\0';
  } while (!feof(fl) && (*temp == '*' || !*temp));

  if (feof(fl))
    return 0;
  else {
    strcpy(buf, temp);
    return lines;
  }
}

void fix_the_rest()
{
  struct char_special_data_saved *csave;
  struct player_special_data_saved *psave;
  struct char_ability_data *abil;
  struct char_point_data *pnts;
  
  csave = &player.char_specials_saved;
  psave = &player.player_specials_saved;
  abil = &player.abilities;
  pnts = &player.points;    
  
  player.birth = time(0);
  player.played = 0;
  csave->affected_by = 0;
  csave->affected2 = 0;
  csave->affected3 = 0;
  csave->affected4 = 0;
  psave->conditions[0] = 0;
  psave->conditions[0] = 0;
  psave->conditions[0] = 0;
  abil->str_add = 0;
}

void convert()
{
  FILE *fl;
  FILE *outfile;
  char dummy[500],dummy2[500], *pek = NULL;  
  int ints[15], amt = 0, i;
  struct char_special_data_saved *csave;
  struct player_special_data_saved *psave;
  struct char_ability_data *abil;
  struct char_point_data *pnts;
  
  if (!(fl = fopen("players.conv", "r+"))) {
    printf("Can't open players.conv.");
    exit(0);
  }

  if (!(outfile = fopen("players.new", "w"))) {
    printf("Can't create players.new...?");
    exit(0);
  }
  csave = &player.char_specials_saved;
  psave = &player.player_specials_saved;
  abil = &player.abilities;
  pnts = &player.points;  
  
  for (;;) {
    get_line(fl, dummy);  
    if (feof(fl)) {
      fclose(fl);
      fclose(outfile);
      puts("Done.");
      printf("Total amount of players converted: %d\n", amt);
      return;
    }  
    sscanf(dummy,"%s", player.name);
    pek = fread_string(fl);
    strcpy(player.title, (pek)? pek: "the Player");
    pek = fread_string(fl);
    strcpy(player.description, (pek)? pek: "An average-looking person.");
    get_line(fl, dummy);
    sscanf(dummy,"%d %d %d %d %d %d %s", &ints[0], &ints[1], &ints[2], &ints[3], 
    &ints[4], &ints[5], player.pwd); 
    player.sex = (byte) ints[0];
    player.class = (byte) ints[1];
    player.level = (long) ints[2];
    player.hometown = (sh_int) ints[3];
    player.weight = (ubyte) ints[4];
    player.height = (ubyte) ints[5];
    amt ++;
    fgets(dummy, 256, fl);
    sscanf(dummy, "%ld %ld %ld", &csave->alignment, &csave->idnum, &csave->act);
    for (i = 0;  i < MAX_SKILLS; i++) {
      fgets(dummy, 256, fl);
      sscanf(dummy, "%d", &ints[0]);
      psave->skills[i] = (byte) ints[0];
    }

    fgets(dummy, 256, fl);
    sscanf(dummy, "%ld %d %d %d %ld %d", &psave->wimp_level, &ints[0], 
    &ints[1], &ints[2], &psave->pref, &ints[3]);
    psave->freeze_level = (byte) ints[0];
    psave->invis_level = (sh_int) ints[1];
    psave->load_room = (room_num) ints[2];
    psave->bad_pws = (ubyte) ints[3];
    pek = fread_string(fl);
    strcpy(psave->clubname, (pek)? pek: "\0");
    pek = fread_string(fl);
    strcpy(psave->clubrank, (pek)? pek: "\0");
    pek = fread_string(fl);
    strcpy(psave->servant, (pek)? pek: "\0");  
    fgets(dummy, 512, fl);
    sscanf(dummy, "%d %d %d %d %d %d %d %d %d %d %d %d %d",
    &ints[0], &ints[1], &ints[2], &ints[3], &ints[4], &ints[5],
    &ints[6], &ints[7], &ints[8], &ints[9], &ints[10], &ints[11], &ints[12]); 
    psave->air = (ubyte) ints[0];
    psave->earth = (ubyte) ints[1];
    psave->fire = (ubyte) ints[2];
    psave->spirit = (ubyte) ints[3];
    psave->water = (ubyte) ints[4];
    psave->taint = (ubyte) ints[5];
    psave->spark = (ubyte) ints[6];
    psave->teachable = (ubyte) ints[7];
    psave->guild = (ubyte) ints[8];
    psave->guildpos = (ubyte) ints[9];
    psave->darkfriend = (ubyte) ints[10];
    psave->taveren = (ubyte) ints[11];
    psave->true_power = (ubyte) ints[12];
    fgets(dummy, 512, fl);
    sscanf(dummy,  "%d %d %d %d %d %d %d %d %d %d %d %d %d %d",
    &ints[0], &ints[1], &ints[2], &ints[3], &ints[4], &ints[5], &ints[6], 
    &ints[7], &ints[8], &ints[9], &ints[10], &ints[11], &ints[12], &ints[13]);
    psave->wolfkin = (ubyte) ints[0];
    psave->trains = (ubyte) ints[1];
    psave->nobility = (ubyte) ints[2];
//    psave->spare2 = (ubyte) ints[3];
    psave->race = (ubyte) ints[4];
    psave->clan = (ubyte) ints[5];
    psave->society = (ubyte) ints[6];
    psave->drf_flags = (ubyte) ints[7];
//    psave->spare3 = (ubyte) ints[8];
    psave->ajah = (ubyte) ints[9];
    psave->betting = (ubyte) ints[10];
    psave->conditions[0] = (sbyte) ints[11];
    psave->conditions[1] = (sbyte) ints[12];
    psave->conditions[2] = (sbyte) ints[13];
    for (i = 0;  i < 200; i++) {
      fgets(dummy, 256, fl);
      sscanf(dummy, "%d", &ints[0]);
      psave->bytes[i] = (byte) ints[0];
    }
    fgets(dummy, 256, fl);
    sscanf(dummy, "%ld %ld %ld %ld %ld %ld %ld %ld %d ", // %ld %ld ",
    &psave->spells_to_learn, &psave->olc_zone, &psave->bonded, &psave->minion, 
    &psave->powerpoints, &psave->questpoints, &psave->absorbation, &psave->permanent,
    //&psave->mount_vnum, 
    &ints[13] /*, &psave->spare9b, &psave->spare10b*/ );
    for (i = 0;  i < 200; i++) {
      fgets(dummy, 256, fl);
      sscanf(dummy, "%ld", &psave->ints[i]);
    }
    fgets(dummy, 256 , fl);
    sscanf(dummy,    "%d %d %d %d %d %d \n", &ints[0], &ints[1], &ints[2],
    &ints[3], &ints[4], &ints[5]);
    abil->str = (sbyte) ints[0];
    abil->intel = (sbyte) ints[1];
    abil->wis = (sbyte) ints[2];
    abil->dex = (sbyte) ints[3];
    abil->con = (sbyte) ints[4];
    abil->cha = (sbyte) ints[5];
    fgets(dummy, 256, fl);
    sscanf(dummy, "%d %d %d %d %d %d %d %ld %ld %ld %ld %ld",
    &ints[0], &ints[1], &ints[2], &ints[3], &ints[4], &ints[5], &ints[6], 
    &pnts->gold, &pnts->bank_gold, &pnts->exp, &pnts->hitroll, &pnts->damroll);
    pnts->mana = (sh_int) ints[0];
    pnts->max_mana = (sh_int) ints[1];
    pnts->hit = (sh_int) ints[2];
    pnts->max_hit = (sh_int) ints[3];
    pnts->move = (sh_int) ints[4];
    pnts->max_move = (sh_int) ints[5];
    pnts->armor = (sh_int) ints[6];
    fgets(dummy, 256, fl);
    sscanf(dummy, "%ld %s", &player.last_logon, dummy2);   
    strcpy(player.host, (*dummy2)? dummy2: "0.0.0.0");
    fix_the_rest();
    fwrite(&player, sizeof(struct char_file_u), 1, outfile);
    fgets(dummy, 256, fl);
    sscanf(dummy, "%s", dummy2);
    if (*dummy2 != '*') {
       printf("File corrupt (expected line with *, got '%s'), exiting\n", dummy2);
       exit(1);
    } 
  }
}

int main(int argc, char *argv[])
{
  if (argc != 1)
    printf("Usage: %s \n", argv[0]);
  else {
    printf("Beginning conversion........\n");
    convert();
  }
  return 0;
}
