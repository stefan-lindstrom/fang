/****************************************************************************
* File: chess.h                                   Part of Dragon's Fang Mud *
* Contents: All the functionality needed for a game of chess.               *
*                                                                           *
* Original author: Fredrik Lännergren, fredrik_lannergren@yahoo.com         *
*                                                                           *
* Please, whoever reads this, keep the original author line intact; I'd     *
* like -some- credit for the effort I put into this...                      *
*****************************************************************************/

#ifndef __CHESS_H__
#define __CHESS_H__

#include "structs.h"
#include "interpreter.h"

#define CHESS_PAWN      1
#define CHESS_KNIGHT    2
#define CHESS_BISHOP    3
#define CHESS_ROOK      4
#define CHESS_QUEEN     5
#define CHESS_KING      6

#define CHESS_BLACK 0
#define CHESS_WHITE 1

#define CHESS_FLAG_PAWN_MOVED_TWO_STEPS	1
#define CHESS_FLAG_KING_MOVED		2
#define CHESS_FLAG_ROOK_MOVED		4

struct chess_square_data
{
  int color; // The color of the square, black or white
  int x;     // The x-index of this square on the board. Stupid, I know, but...
  int y;     // The y-index of this square on the board. ---------""----------
  struct chess_piece_data *piece;  // The piece occupying this square
};

struct chess_piece_data
{  
  int type;  // The type; pawn, knight...
  int color; // The color, black or white
  int flags; // bitvector used for setting movement flags
  struct chess_square_data *on; // The square this piece is on currently
};

struct game_of_chess
{
   struct char_data *white_player;  
   struct char_data *black_player;
   struct chess_square_data board[8][8]; // A chessboard always contains 64 squares
   int turn;    // The color of whose turn it is, black or white
   int playing; // Flag to determine whether the board currently is used in a game
};

extern ACMD(do_chess);
extern void free_game_of_chess(struct game_of_chess *);
#endif 
