/*************************************************************************
*   File: cards.h                                    Part of Dragons Fang *
*  Usage: Card game structs and stuff                                     *
*                                                                         *
*  No copyright at all :)                                                 *
************************************************************************ */

#ifndef __CARDS_H__
#define __CARDS_H__

#include "structs.h"
#include "utils.h"
#include "interpreter.h" 

ACMD(do_cardgame);

extern void end_game(struct card_game *);
extern void remove_player(struct char_data *);
#endif
