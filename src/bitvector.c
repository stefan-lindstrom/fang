/******************************************************************************
 * $Id: bitvector.c,v 1.3 2002/11/07 04:02:01 fang Exp $
 *   
 * File: bitvector.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: bitvector.c,v 
 *   Revision 1.2  2002/07/28 13:56:03  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.3  2002/05/27 10:52:48  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.3  2002/01/14 02:36:26  mud
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/02/21 14:00:16  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

//#include <mlloc.h>

#include "bitvector.h"
#include "utils.h"

struct bitv
{
  unsigned long  bt_Size; 	/* Size in bits       */
  unsigned int   by_Size;	/* Size in bytes      */
  unsigned char *bits;          /* Array to hold bits */
};

/*
 * Creates a bitvector with <size> bits width. 
 */
bv_tp BITVECTOR(unsigned long size)
{
  bv_tp tmp = calloc(1,sizeof(bv_t));
  memset(tmp, 0, sizeof(bv_t)); 
  if (NULL == tmp) {
    alog("SYSERR: BITVECTOR()[bitvector.c]: Failed memory allocation.");
    return NULL;
  }

  tmp->bt_Size = size;
  tmp->by_Size = (size / 8) + ((size % 8) ? 1 : 0);
  tmp->bits    = calloc(tmp->by_Size,sizeof(char));
  if (NULL == tmp->bits) {
    alog("SYSERR: BITVECTOR()[bitvector.c]: Failed memory allocation.");
    free(tmp);
    return NULL;
  }
  return (tmp);
}

/*
 * Set's bit <bit> in the bitvector <bv>. 
 * NOTE: <bit> is bitnumber, i.e 31 to set the MSB in a 32 bit bitvector. 
 */
void set_bit(bv_tp bv ,unsigned long bit)
{
  unsigned bitpos,bitinpos;

  if (NULL == bv) {
    alog("SYSERR: set_bit()[bitvector.c]: got NULL-pointer.");
    return;
  }
  if (bit > bv->bt_Size) {
    alog("SYSERR: set_bit()[bitvector.c]: bit out of range for bitvector!");
    return;
  }
  bitpos = bit / 8;
  bitinpos = (1 << (bit % 8));

  bv->bits[bitpos] |= bitinpos;
}

/*
 * Test if <bit> is set in <bv>. 
 */
int test_bit(bv_tp bv,unsigned long bit)
{
  unsigned bitpos,bitinpos;
  
  if (NULL == bv) {
    alog("SYSERR: test_bit()[bitvector.c]: got NULL-pointer.");
    return 0;
  }
  if (bit > bv->bt_Size) {
    alog("SYSERR: test_bit()[bitvector.c]: bit out of range for bitvector!");
    return 0;
  }
  bitpos = bit / 8;
  bitinpos = (1 << (bit % 8));
 
  return (bv->bits[bitpos] & bitinpos);
}

/*
 * Toggles value of <bit> in <bv>
 */
void toggle_bit(bv_tp bv,unsigned long bit)
{  
  unsigned bitpos,bitinpos;
  
  if (NULL == bv) {
    alog("SYSERR: set_bit()[bitvector.c]: got NULL-pointer.");
    return;
  }
  if (bit > bv->bt_Size) {
    alog("SYSERR: set_bit()[bitvector.c]: bit out of range for bitvector");
    return;
  }
  bitpos = bit / 8;
  bitinpos = (1 << (bit % 8));
  
  bv->bits[bitpos] = (bv->bits[bitpos] ^ bitinpos);
}

/*
 * Clears <bit> in <bv>
 */
void clear_bit(bv_tp bv,unsigned long bit)
{
  unsigned bitpos,bitinpos;
  
  if (NULL == bv) {
    alog("SYSERR: set_bit()[bitvector.c]: got NULL-pointer.");
    return;
  }
  if (bit > bv->bt_Size) {
    alog("SYSERR: set_bit()[bitvector.c]: bit out of range for bitvector!");
    return;
  }
  bitpos = bit / 8;
  bitinpos = (1 << (bit % 8));   
   
  bv->bits[bitpos] &= (~bitinpos);
} 

/*
 * Frees mem (and hence kills) the bitvector <bv>. 
 * Make sure you NULL your <bv> ptr after using this function.
 */
void KILL_BV(bv_tp bv)
{
  if (NULL == bv) {
    alog("SYSERR: KILL_BV()[bitvector.c] got NULL-pointer.");
    return;
  }
  if (NULL != bv->bits) {
    free(bv->bits);
  }
  free(bv);
}

/*
 * Writes the bitvector <bv> to the file <fl>. 
 * Doesn't close the file or anything though. 
 */
int to_file(bv_tp bv,FILE *fl)
{
  int lines = 0, ill = 0,i;

  if (NULL == bv || NULL == fl) {
    alog("SYSERR: to_file()[bitvector.c] got NULL-pointer(s).");
    return -1;
  }
  
  lines = bv->by_Size / 10 + ((bv->by_Size % 10) ? 1 : 0);
  ill   = (bv->by_Size % 10);

  fprintf(fl,"BV 1.0\n%lu %u\n%u %u %u\n",bv->bt_Size,bv->by_Size,lines,10,ill);
  for (i = 0; i < (lines - 1); i++) {
    fprintf(fl,"%u %u %u %u %u %u %u %u %u %u\n",bv->bits[i * 10],bv->bits[1+(i * 10)],
    bv->bits[2+(i * 10)],bv->bits[3+(i * 10)],bv->bits[4+(i * 10)],bv->bits[5+(i * 10)],
    bv->bits[6+(i * 10)],bv->bits[7+(i * 10)],bv->bits[8+(i * 10)],bv->bits[9+(i * 10)]);
  }
  for (i = 0; i < ill; i++) {
    fprintf(fl,"%u ",bv->bits[i+((lines-1)*10)]);
  }
  if (ill) {
    fprintf(fl,"\n*EOBV*\n");
  } else {
    fprintf(fl,"*EOBV*\n");
  }

  return 1;
}

/*
 * Reads from <fl> a bitvector to place in <bv>. If <bv> is smaller than the
 * bitvector in <fl>, then a new <bv> will be created to fit the size from 
 * <fl> and <bv> killed. 
 * if <bv> is larger than the bitvector in <fl> it'll simply keep it's size
 * and the <fl> bitvector'll be read into it. 
 *
 * In any case, the resulting, read bitvector will be RETURNED, not passed
 * in <bv>!
 */
bv_tp from_file(bv_tp bv,FILE *fl)
{
  int lines = 0, ill = 0, i,j,t;
  int bytes, bits;
  bv_tp tmp = NULL;
  unsigned buf[10];

  if (NULL == bv || NULL == fl) {
    alog("SYSERR: from_file()[bitvector.c]: Got NULL-pointer.");
    return NULL;
  }

  if ((t = fscanf(fl,"BV %d.%d\n%d %d\n%d 10 %d\n",&i,&j,&bits,&bytes,&lines,&ill)) < 6) {
    alog("SYSERR: from_file()[bitvector.c]: Not a valid bitvector in file.(scanned items: %d, expected 6)",t);
    return NULL;
  }

  if (bv->bt_Size < bits) { // Bitvector in file too large, must make new
    if (NULL == (tmp = BITVECTOR(bits))) {
      alog("SYSERR: from_file()[bitvector.c]: Failed to create new bitvector.");
      return NULL;
    }    
    bv = tmp;
  }
  else {
    bv->bt_Size = bits;
    bv->by_Size = bytes;
  }

  for (i = 0; i < (lines - 1); i++) {
    fscanf(fl,"%u %u %u %u %u %u %u %u %u %u\n",buf, buf+1,buf+2,buf+3,buf+4,buf+5,buf+6,buf+7,buf+8,buf+9);
    bv->bits[0+(i * 10)] = (char)buf[0];
    bv->bits[1+(i * 10)] = (char)buf[1];  
    bv->bits[2+(i * 10)] = (char)buf[2];  
    bv->bits[3+(i * 10)] = (char)buf[3];
    bv->bits[4+(i * 10)] = (char)buf[4];  
    bv->bits[5+(i * 10)] = (char)buf[5];
    bv->bits[6+(i * 10)] = (char)buf[6];
    bv->bits[7+(i * 10)] = (char)buf[7];
    bv->bits[8+(i * 10)] = (char)buf[8];
    bv->bits[9+(i * 10)] = (char)buf[9];

  }
  for (i = 0; i < ill; i++) {
    fscanf(fl,"%u ",buf);
    bv->bits[i+((lines - 1) * 10)] = (char)buf[0];
  }
  fscanf(fl,"\n*EOBV*\n");

  return bv;
}

char *bv_toString(bv_tp bv,char **str)
{
  char buf[8096];
  int i;

  buf[0] = '\0';

  for (i = 0; NULL != str[i]; i++) {
    if (test_bit(bv,(unsigned long)i)) {
      strcat(buf,str[i]);
      strcat(buf," ");
    }
  }
  return(strdup(buf));
}
