/******************************************************************************
 * $Id: strutil.c,v 1.4 2003/05/10 10:04:20 fang Exp $
 *   
 * File: strutil.c
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: strutil.c,v 
 *   Revision 1.2  2002/07/28 13:56:05  fan
 *   Removed dmlloc...just to test
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.3  2002/05/27 10:52:50  fang
 *   Dmlloc enabled.
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/02/21 14:00:19  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#include "strutil.h"
//#include <mlloc.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include "utils.h"

char *strupper(char *src)
{
  char *tmp = strdup(src),*p;

  p = tmp;

  while (*tmp) {
    *tmp = toupper(*tmp);
    tmp++;
  }
  return p;
}

char *strlower(char *src)
{
  char *tmp = strdup(src),*p;

  p = tmp;

  while (*tmp) {
    *tmp = tolower(*tmp);
    tmp++;
  }
  return p;
}

char *str_replace(char *src,char tkn,char with)
{
  char *tmp = strdup(src),*p;

  p = tmp;

  while (*tmp) {
    if (*tmp == tkn) {
      *tmp = with;
    }
    tmp++;
  }
  return p;
}

char *replace_nocase(char *src,char tkn,char with)
{
  char *tmp = strdup(src),*p;

  p = tmp;

  while (*tmp) {
    if (toupper(*tmp) == toupper(tkn)) {
      *tmp = with;
    }
    tmp++;
  }
  return p;
}

char *reverse(char *src)
{
  char *tmp = strdup(src),*p,*q;

  p = tmp;
  q = &src[(strlen(src) - 1)];

  while(*tmp) {
    *tmp = *q;
    tmp++;
    q--;
  }
  return p;
}

char *rem(char *src, char tkn)
{
  char *tmp = strdup(src),*p;

  p = tmp;

  while (*tmp) {
    if (*tmp == tkn) {
      strcpy(tmp,(tmp+1));
    }
    else {
      tmp++;
    }
  }

  return p;
}

char *rem_nocase(char *src,char tkn)
{
  char *tmp = strdup(src),*p;

  p = tmp;

  while (*tmp) {
    if (toupper(*tmp) == toupper(tkn)) {
      strcpy(tmp,(tmp+1));
    }
    else {
      tmp++;
    }
  }

  return p;
}
                    
char **split(char *str,char tkn,int *num)
{
  int n,i;
  char *tmp = strdup(str),*p;
  char **t,**ptr;

  p = tmp;
  n = count_chars(tmp,tkn);
  *num = (++n);
  CREATE( t, char*, n );
  ptr = t;

  for (i = 0; i < (n - 1); i++) {
    p = index(tmp,tkn);
    *p = '\0';
    p++;
    *ptr = strdup(tmp);
    ptr++;
    tmp = p;
  }
  p = index(tmp,'\0');
  *ptr = strdup(tmp);

  return t;
}

int count_chars(char *s,char t)
{
  int tmp = 0;

  while(*s) {
    if (*s == t) {
      tmp++;
    }
    s++;
  }
  return tmp;
}


