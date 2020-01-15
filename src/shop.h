/******************************************************************************
 * $Id: shop.h,v 1.2 2003/06/17 10:30:05 fang Exp $
 *   
 * File: shop.h
 *
 * Created: Mon Feb 21 2000 by Unknown Fella (unknown@unknown)
 *
 * This is part of Dragons Fang, which is based on CircleMUD, etc, etc, yada, 
 * yada...
 *
 * A nice description of the files general purpose would be nice here... :)
 * 
 *   $Log: shop.h,v $
 *   Revision 1.2  2003/06/17 10:30:05  fang
 *
 *   Removed a new set of "externs" from source files. For fucks sake people,
 *   let's keep those externs where they belong, in an apropriate header, ok?
 *
 *   Revision 1.1.1.1  2002/07/22 14:40:07  fang
 *   Resetup Fang CVS with current source
 *
 *   Revision 1.2  2002/02/22 15:04:28  fang
 *   *** empty log message ***
 *
 *   Revision 1.4  2002/01/24 21:33:19  mud
 *   Merged from branch: Cleanup-2002_01_18-cleanup_branch.
 *
 *   Revision 1.3.2.1  2002/01/24 20:49:37  mud
 *   More cleanup. Removed files:  specialday.c string.c string.h
 *   Removed DONT_TRADE_WITH-stuff from shops.
 *
 *   Revision 1.3  2000/12/01 09:17:11  mud
 *   Further safety checks added to sedit/medit/redit
 *
 *   Revision 1.2  2000/02/21 14:00:19  mud
 *   CVS-logging headers added to all files.
 *
 *
 *****************************************************************************/

/* ************************************************************************
*   File: shop.h                                        Part of CircleMUD *
*  Usage: shop file definitions, structures, constants                    *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#ifndef __SHOP_H__
#define __SHOP_H__

#include "structs.h"

struct shop_buy_data {
   int type;
   char *keywords;
} ;

#define BUY_TYPE(i)		((i).type)
#define BUY_WORD(i)		((i).keywords)


struct shop_data {
   long	 virtual;		/* Virtual number of this shop		*/
   long	 *producing;		/* Which item to produce (virtual)	*/
   float profit_buy;		/* Factor to multiply cost with		*/
   float profit_sell;		/* Factor to multiply cost with		*/
   struct shop_buy_data *type;	/* Which items to trade			*/
   char	*no_such_item1;		/* Message if keeper hasn't got an item	*/
   char	*no_such_item2;		/* Message if player hasn't got an item	*/
   char	*missing_cash1;		/* Message if keeper hasn't got cash	*/
   char	*missing_cash2;		/* Message if player hasn't got cash	*/
   char	*do_not_buy;		/* If keeper dosn't buy such things	*/
   char	*message_buy;		/* Message when player buys item	*/
   char	*message_sell;		/* Message when player sells item	*/
   long	 temper1;		/* How does keeper react if no money	*/
   long	 bitvector;		/* Can attack? Use bank? Cast here?	*/
   long	 keeper;		/* The mobil who owns the shop (virtual)*/
   long	 with_who;		/* Who does the shop trade with?	*/
   long	*in_room;		/* Where is the shop?			*/
   long	 open1, open2;		/* When does the shop open?		*/
   long	 close1, close2;	/* When does the shop close?		*/
   long	 bankAccount;		/* kljsdhrgelkwrtghwseoiyghzaw4e5oy     */
   long	 lastsort;		/* How many items are sorted in inven?	*/
   SPECIAL (*func);		/* Secondary spec_proc for shopkeeper	*/
};


#define MAX_TRADE	5	/* List maximums for compatibility	*/
#define MAX_PROD	5	/*	with shops before v3.0		*/
#define VERSION3_TAG	"v3.0"	/* The file has v3.0 shops in it!	*/
#define MAX_SHOP_OBJ	100	/* "Soft" maximum for list maximums	*/


/* Pretty general macros that could be used elsewhere */
#define IS_GOD(ch)		(!IS_NPC(ch) && (GET_LEVEL(ch) >= LVL_GOD))
#define GET_OBJ_NUM(obj)	(obj->item_number)
#define END_OF(buffer)		((buffer) + strlen((buffer)))


/* Possible states for objects trying to be sold */
#define OBJECT_DEAD		0
#define OBJECT_NOTOK		1
#define OBJECT_OK		2


/* Types of lists to read */
#define LIST_PRODUCE		0
#define LIST_TRADE		1
#define LIST_ROOM		2

struct stack_data {
   long data[100];
   long len;
} ;

#define S_DATA(stack, index)	((stack)->data[(index)])
#define S_LEN(stack)		((stack)->len)


/* Which expression type we are now parsing */
#define OPER_OPEN_PAREN		0
#define OPER_CLOSE_PAREN	1
#define OPER_OR			2
#define OPER_AND		3
#define OPER_NOT		4
#define MAX_OPER		4

extern const char *operator_str[];

#define SHOP_NUM(i)		(shop_index[(i)].virtual)
#define SHOP_KEEPER(i)		(shop_index[(i)].keeper)
#define SHOP_OPEN1(i)		(shop_index[(i)].open1)
#define SHOP_CLOSE1(i)		(shop_index[(i)].close1)
#define SHOP_OPEN2(i)		(shop_index[(i)].open2)
#define SHOP_CLOSE2(i)		(shop_index[(i)].close2)
#define SHOP_ROOM(i, num)	(shop_index[(i)].in_room[(num)])
#define SHOP_BUYTYPE(i, num)	(BUY_TYPE(shop_index[(i)].type[(num)]))
#define SHOP_BUYWORD(i, num)	(BUY_WORD(shop_index[(i)].type[(num)]))
#define SHOP_PRODUCT(i, num)	(shop_index[(i)].producing[(num)] )
#define SHOP_BANK(i)		(shop_index[(i)].bankAccount)
#define SHOP_BROKE_TEMPER(i)	(shop_index[(i)].temper1)
#define SHOP_BITVECTOR(i)	(shop_index[(i)].bitvector)
#define SHOP_TRADE_WITH(i)	(shop_index[(i)].with_who)
#define SHOP_SORT(i)		(shop_index[(i)].lastsort)
#define SHOP_BUYPROFIT(i)	(shop_index[(i)].profit_buy)
#define SHOP_SELLPROFIT(i)	(shop_index[(i)].profit_sell)
#define SHOP_FUNC(i)		(shop_index[(i)].func)

#define	WILL_START_FIGHT	1
#define WILL_BANK_MONEY		2

#define SHOP_KILL_CHARS(i)	(IS_SET(SHOP_BITVECTOR(i), WILL_START_FIGHT))
#define SHOP_USES_BANK(i)	(IS_SET(SHOP_BITVECTOR(i), WILL_BANK_MONEY))

extern const char *shop_bits[];

#define MIN_OUTSIDE_BANK	5000
#define MAX_OUTSIDE_BANK	15000

#define MSG_NOT_OPEN_YET	"Come back later!"
#define MSG_NOT_REOPEN_YET	"Sorry, we have closed, but come back later."
#define MSG_CLOSED_FOR_DAY	"Sorry, come back tomorrow."
#define MSG_NO_STEAL_HERE	"$n is a bloody thief!"
#define MSG_NO_SEE_CHAR		"I don't trade with someone I can't see!"
#define MSG_NO_SELL_ALIGN	"Get out of here before I call the guards!"
#define MSG_NO_SELL_CLASS	"We don't serve your kind here!"
#define MSG_NO_USED_WANDSTAFF	"I don't buy used up wands or staves!"
#define MSG_CANT_KILL_KEEPER	"Get out of here before I call the guards!"

/*
 * Funcs
 */
extern int ok_damage_shopkeeper(struct char_data * ch, struct char_data * victim);
extern void show_shops(struct char_data * ch, char *arg);
extern void boot_the_shops(FILE * shop_f, char *filename, int rec_count);
extern void sort_keeper_objs(struct char_data * keeper, int shop_nr);
extern void update_shopflags(int roomnr);
extern char *customer_string(int shop_nr, int detailed);

/*
 * Data
 */
extern int top_shop;
extern struct shop_data *shop_index;

#endif
