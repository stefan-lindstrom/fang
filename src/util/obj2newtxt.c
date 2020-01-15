#include "conf.h"
#include "sysdep.h"
#include "structs.h"

#define OBJ_SIZE sizeof(struct obj_file_elem)

struct obj_file_elem target;

int get_line(FILE * fl, char *buf)
{
  char temp[200];
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

void convert_one_file(char *name) { 
  FILE *fl;
  char text[200000];
  int i;
  struct obj_file_elem src;
  struct rent_info rent;

  if (!(fl = fopen(name,"r"))) {
    printf("Couldn't open file %s.\r\n", name);
    return;
  }
  fread(&rent, sizeof(struct rent_info), 1, fl);
  sprintf(text,"0 %ld 0 %ld %ld %ld\n", rent.rentcode, rent.gold, rent.account, rent.nitems);
  fread(&src, OBJ_SIZE,1,fl);
  while(!feof(fl)) { 
    sprintf(text,"%s%d %d %ld %ld %ld 100\n",text, 
src.item_number,src.locate,src.value[0],src.value[1],src.value[2]);
    sprintf(text,"%s%ld %ld %ld %ld %ld\n",text, src.value[3],src.extra_flags,src.mana_add,src.weight,src.timer);
    sprintf(text,"%s%ld\n",text, src.bitvector);
    src.name[499] = 0;
    src.s_desc[499] = 0;
    src.a_desc[499] = 0;
    src.desc[499] = 0;
    sprintf(text,"%s%s~\n", text, ((src.name) && (*src.name))? 
src.name:"NOTHING");   
    sprintf(text,"%s%s~\n", text, ((src.s_desc) && 
(*src.s_desc))?src.s_desc:"NOTHING");
    sprintf(text,"%s%s~\n", text, ((src.a_desc) && (*(src.a_desc))) ?
            src.a_desc:"NOTHING");
    sprintf(text,"%s%s~\n", text, ((src.desc) && 
(src.desc))?src.desc:"NOTHING");
    
    for (i = 0; i < MAX_OBJ_AFFECT; i++) 
      sprintf(text,"%s%d %ld\n", text, src.affected[i].location, src.affected[i].modifier);
    fread(&src, OBJ_SIZE, 1, fl);
  }
  fclose(fl);
  if (unlink(name)) {
    printf("Error: Couldn't delete old file %s.\r\n", name);
    return;
  }
  if (!(fl = fopen(name,"w"))) {
    printf("Error: couldn't create new file after old file's been deleted!\r\n");
    return;
  }
  fprintf(fl, text);
  fclose(fl);
  printf("%s converted.\r\n", name);
  free(text);
}

void convert_files(FILE *fl)
{
  
  char *list[100], line[255], *dum;
  int i = 0,j;
  while (!feof(fl)) {
    get_line(fl, line);
    if (!feof(fl)) {
      list[i]= malloc(100);
      list[i+1]= malloc(100);
      list[i+2]= malloc(100);
      list[i+3]= malloc(100);
      list[i+4]= malloc(100);
      sscanf(line, "%s %s %s %s %s",list[i], list[i+1], list[i+2], 
list[i+3], list[i+4]);
      i += 5;
    }
  }
  for (j = 0; j < i; j ++) {
    if (!list[j])
      continue;
    dum = list[j];
    while (*dum && (*dum != '.'))
      dum ++;
    if (!strncmp(dum, ".objs", 4)){
      printf("Converting %s.\r\n", list[j]);
      convert_one_file(list[j]);
    }
  }
}

int main(int argc,char **argv)
{
  FILE *in;
  if (!(in = fopen("plrlist","r"))) {
    printf("Error: Can't find the file plrlist.\r\n");
    return 1;
  }
  printf("Beginning conversion.\r\n");
  convert_files(in); 
  fclose(in);
  return 0;
}     
