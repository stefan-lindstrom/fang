#include "conf.h"
#include "sysdep.h"
#include "structs.h"

#define OBJ_SIZE sizeof(struct obj_file_elem)

struct obj_file_elem target;

void obj_to_txt(struct obj_file_elem *src,FILE *fil);

int main(int argc,char **argv)
{
  struct rent_info rent;
  FILE *in,*out;
  char outname[1000];
  int numobjs = 0;
  
  sprintf(argv[1],"%s.objs",argv[1]);
  
  if (!(in = fopen(argv[1],"r"))) {
    printf("Format: obj2txt <plrname>\r\n");
    return 1;
  }
  strcpy(outname,argv[1]);
  strcat(outname,".txt");

  if (!(out = fopen(outname,"w")))
    return 1;
  
  fread(&rent, sizeof(struct rent_info), 1, in);
  
  while(fread(&target,OBJ_SIZE,1,in))
    numobjs++;

  fprintf(out,"%d\n",numobjs);

  fseek(in,0L,SEEK_SET);
  
  if (fread(&rent, sizeof(struct rent_info), 1,in))
    fprintf(out,"0 %ld 0 %ld %ld %ld\n", rent.rentcode, rent.gold, rent.account, rent.nitems);
  else {
    printf("Unexpected end in file %s in middle of rent code.\r\nExiting.", argv[1]);
    exit(1);
  }
  while(fread(&target,OBJ_SIZE,1,in))
    obj_to_txt(&target,out);

  fclose(in);
  fclose(out);
  printf("Done.\r\n");
  return 0;
}

void obj_to_txt(struct obj_file_elem *src,FILE *fil)
{
  struct obj_affected_type *o;
  int i;

  fprintf(fil,"%d %d %ld %ld %ld\n",src->item_number,src->locate,src->value[0],src->value[1],src->value[2]);
  fprintf(fil,"%ld %ld %ld %ld %ld\n",src->value[3],src->extra_flags,src->mana_add,src->weight,src->timer);
  fprintf(fil,"%ld\n",src->bitvector);
  
  if (*src->name) 
    fprintf(fil,"%s~\n", src->name);
  else 
    fprintf(fil, "NOTHING~\n");
    
  if (*src->s_desc)  
    fprintf(fil,"%s~\n",src->s_desc);
  else 
    fprintf(fil, "NOTHING~\n");    
  if (*src->a_desc)    
    fprintf(fil,"%s~\n",src->a_desc);
  else 
    fprintf(fil, "NOTHING~\n");    
  if (*src->desc)    
    fprintf(fil,"%s~\n",src->desc);
  else 
    fprintf(fil, "NOTHING~\n");
    
  for (i = 0; i < MAX_OBJ_AFFECT; i++) {
    o = &(src->affected[i]);
    fprintf(fil,"%d %ld\n",o->location,o->modifier);
  }
}
