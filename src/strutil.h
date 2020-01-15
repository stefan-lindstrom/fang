/******************************************************************************
 * $Id: strutil.h,v 1.2 2003/05/10 10:04:20 fang Exp $
 *   
 * File: strutil.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: strutil.h,v $
 *   Revision 1.2  2003/05/10 10:04:20  fang
 *   More dynvars...
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.2  2000/02/21 14:00:19  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/*******************************************************************
 *                                                                 *
 * Version        : 1.0                                            *
 * Skapad         : 1997-07-23                                     *
 * Senast „ndrad  : N/A                                            *
 * Nytt sedan sist: N/A                                            *
 *                                                                 *
 *******************************************************************/
                                                                    
#ifndef __STRUTIL_H__
#define __STRUTIL_H__

#ifndef MIN
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

extern char  *strupper(char *src);
extern char  *strlower(char *src);
extern char  *str_replace(char *src,char tkn,char with);
extern char  *replace_nocase(char *src,char tkn,char with);
extern char  *reverse(char *src);
extern char  *rem(char *src, char tkn);
extern char  *rem_nocase(char *src,char tkn);
extern char **split(char *str,char tkn,int *num);
extern int    count_chars(char *s,char t);

#endif
