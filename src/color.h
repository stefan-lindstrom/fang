/******************************************************************************
 * $Id: color.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: color.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: color.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.4  2001/10/03 14:34:19  mud
 *   *** empty log message ***
 *
 *   Revision 1.3  2001/09/19 05:52:58  mud
 *   Dynamic buffer and select-\>poll mod.
 *
 *   Revision 1.2  2000/02/21 14:00:16  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#ifndef __COLOR_H__
#define __COLOR_H__

#include "interpreter.h"
/*
 * Funcs
 */
extern GString *proc_color(GString *inbuf, int colour);
int htmlize(FILE *infile, FILE *outfile);
void htmlize_string(char *, char* );
ACMD(do_htmlize);
#endif


