#include "conf.h"
#include "sysdep.h"
#include "structs.h"

#define OBJ_SIZE sizeof(struct obj_file_elem)
#define CREATE(result, type, number)  do {\
	if (!((result) = (type *) calloc ((number), sizeof(type))))\
		{ printf("malloc failure\r\n"); abort(); } } while(0)

struct obj_file_elem target;

void txt_to_obj(struct obj_file_elem *src,FILE *fil);

char *fread_string(FILE * fl, char *error)
{
  char buf[MAX_STRING_LENGTH], tmp[MAX_STRING_LENGTH], *rslt;
  register char *point;
  int done = 0, length = 0, templength = 0;

  *buf = '\0';

  do {
    if (!fgets(tmp, 8000, fl)) {
      fprintf(stderr, "SYSERR: fread_string: format error at or near %s\n",
	      error);
      exit(1);
    }
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

    if (length + templength >= MAX_STRING_LENGTH) {
      printf("String too long.\r\n");
      exit(1);
    } else {
      strcat(buf + length, tmp);
      length += templength;
    }
  } while (!done);

  /* allocate space for the new string and copy it */
  if (strlen(buf) > 0) {
    CREATE(rslt, char, length + 1);
    strcpy(rslt, buf);
  } else
    rslt = NULL;

  return rslt;
}

int main(int argc,char **argv)
{
  struct rent_info rent;
  FILE *in,*out;
  char outname[1000], inname[1000];
  int numobjs = 0,i;
  
  sprintf(inname,"%s.objs.txt", argv[1]);  
  if (!(in = fopen(inname,"r")))  {
    printf("Format: txt2obj <plrname>\r\n");  
    return 1;
  }
    
 
  sprintf(outname,"%s.objs", argv[1]);
  if (!(out = fopen(outname,"w"))){
    printf("Error opening outfile.\r\n");
    return 1;
  }
  fscanf(in,"%d\n",&numobjs);
  
  fscanf(in,"%ld %ld %ld %ld %ld %ld\n", &rent.time, &rent.rentcode, 
         &rent.net_cost_per_diem, &rent.gold, &rent.account, &rent.nitems);
  
  fwrite(&rent, sizeof(struct rent_info), 1, out);
  printf("%d objs", numobjs);
  for (i=0; i < numobjs; i++) {
    printf(".");
    txt_to_obj(&target,in);
    fwrite(&target,OBJ_SIZE,1,out);
  }

  fclose(in);
  fclose(out);
  printf("Done.\n");
  return 0;
}

void txt_to_obj(struct obj_file_elem *src,FILE *fil)
{
  struct obj_affected_type *o;
  int i = 0;
  long dum[10];
  char *temp, buf[10000];
  
  fscanf(fil,"%ld %ld %ld %ld %ld\n",&dum[0], &dum[1], &src->value[0],&(src->value[1]),&(src->value[2]));
  src->item_number = (obj_num) dum[0];
  src->locate = (sh_int) dum[1];
  fscanf(fil,"%ld %ld %ld %ld %ld\n",&(src->value[3]),&(src->extra_flags),&(src->mana_add),&(src->weight),&(src->timer));
  fscanf(fil,"%ld\n",&(src->bitvector));
  
  temp = fread_string(fil, buf);
  sprintf(src->name, temp);
  if (!strncasecmp(src->name, "NOTHING", strlen(src->name))) 
    src->name[0] = 0;
  temp = fread_string(fil, buf);
  sprintf(src->s_desc, temp);
  if (!strncasecmp(src->s_desc, "NOTHING", strlen(src->s_desc))) 
    src->s_desc[0] = 0;
  temp = fread_string(fil, buf);
/* Jag skippade a_desc, eftersom den verkar kuka ur. */
  temp = fread_string(fil, buf);
  sprintf(src->desc, temp);
  if (!strncasecmp(src->desc, "NOTHING", strlen(src->desc))) 
    src->desc[0] = 0;  
  for (i = 0; i < MAX_OBJ_AFFECT; i++) {
    o = &(src->affected[i]);
    fscanf(fil,"%ld %ld\n",&dum[0],&dum[1]);
    o->location = (byte) dum[0],
    o->modifier = (sbyte) dum[1];
  }
}
