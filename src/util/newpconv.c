#include <stdio.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>

#include "../structs.h"

struct new_char_file_u
{
  struct char_file_u old;
  char title[50]; // Imm title
  // add more data as needed;
};

typedef struct char_file_u oldchar_t;
typedef struct new_char_file_u newchar_t;

int main(int argc, char **argv)
{
  FILE *in, *out;
  oldchar_t old;
  newchar_t new;

  if (3 != argc) {
    fprintf(stderr,"Usage: %s <infile> <outfile>\n",argv[0]);
    return 1;
  }

  if (!strcmp(argv[1],argv[2])) {
    fprintf(stderr,"in file and out file has same name!\n");
    return 1;
  }

  if (NULL == (in = fopen(argv[1],"r"))) {
    perror("infile couldn't be opened: ");
    return 1;
  }

  if (NULL == (out = fopen(argv[2],"w"))) {
    perror("outfile couldn't be opened: ");
    fclose(in);
    return 1;
  }

  while(fread(&old,sizeof(oldchar_t),1,in)) {
    memcpy(&new,&old,sizeof(oldchar_t));
    memset(old.title,'\0',50);
    // Clear all other datat here too...
    fwrite(&new,sizeof(newchar_t),1,out);
  }

  fclose(in);
  fclose(out);
  printf("Converting completed....Don't forget to add the new fields to char_file_u in structs.h!\n");

  return 0;
}
 
