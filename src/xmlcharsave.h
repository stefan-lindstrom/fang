/******************************************************************************
 * $Id: xmlcharsave.h,v 1.1.1.1 2002/07/22 14:40:07 fang Exp $
 *   
 * File: xmlcharsave.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: xmlcharsave.h,v $
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:29  fang
 *   *** empty log message ***
 *
 *   Revision 1.11  2002/01/24 21:33:21  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.10.2.2  2002/01/22 20:51:00  mud
 *
 *
 *   Removed boats and info system. Obsolete, unused code with no value.
 *   Fixed bug in portal_stone SPECIAL. Now checks that a room exists
 *   before attempting to transf chars there. :)
 *
 *   Revision 1.10.2.1  2002/01/21 01:23:42  mud
 *   Code cleanup. Aiel clans/socities gone. Lot of old guild code gone.
 *
 *   Revision 1.10  2000/11/16 15:43:05  mud
 *   *** empty log message ***
 *
 *   Revision 1.9  2000/11/12 14:44:14  mud
 *   More support code for xml transforming, a few bugfixes to xmlchar code, and index_from_array().
 *
 *   Revision 1.8  2000/11/09 14:13:48  mud
 *   Getting closer to switching-dat with xml chars
 *
 *   Revision 1.7  2000/11/08 08:25:40  mud
 *   Moved update_p_tabel() to xmlcharsave.[ch].
 *
 *   Revision 1.6  2000/11/08 07:47:43  mud
 *   *** empty log message ***
 *
 *   Revision 1.5  2000/09/10 06:36:20  mud
 *   Some updates...
 *
 *   Revision 1.4  2000/05/25 15:06:48  mud
 *
 *   Talent code changed to utilize char* arrays instead of a million #defines.
 *   Changes made to do_set, do_stat, do_score.
 *   Imm command Prove added.
 *
 *   Revision 1.3  2000/03/10 18:26:42  mud
 *   Added a game of cards, which includes two new files,
 *   cards.c and cards.h, and modifying of a bunch of others.
 *   Note: xmlcharsve.x shouldn't be here, they just sort of tagged along.
 *
 *   Revision 1.2  2000/02/21 14:00:20  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

#ifndef __XMLCHARSAVE_H__
#define __XMLCHARSAVE_H__

/*#include <gnome-xml/parser.h>
#include <gnome-xml/tree.h>
#include <gnome-xml/entities.h>
#include <gnome-xml/parserInternals.h>*/
#include <libxml2/libxml/parser.h>
#include <errno.h>
#include "structs.h"

#include "structs.h"
#include "interpreter.h"

/* these factors should be unique integers */
/*
 * Functions...
 */
extern ACMD(do_xmlsave);
extern ACMD(do_xmlload);
extern ACMD(do_fred);
extern ACMD(do_testing);

extern struct char_data *xmlload_char(char *name);
extern struct char_data *raw_load(char *name, int old);

extern void xmlsave_char(struct char_data *ch);

extern int load_playertable(void);
extern void update_p_table(struct char_data *ch);

extern int char_exists(char *name);

#endif
