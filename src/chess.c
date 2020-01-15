/****************************************************************************
* File: chess.c                                   Part of Dragon's Fang Mud *
* Contents: All the functionality needed for a game of chess                *
*           between two players in the MUD. Even though it was made for     *
*           Dragon's Fang, it should work fine with any Circle based MUD,   *
*           but some work with the color coding might be necessary;         *
*           we use the '&x' type of color coding.                           *
*           The code in here is made deliberately simple and "plain" in     *
*           combination with comments, so that it can be understood and     *
*           hopefully improved by others.                                   *
*           The code can fairly easily be changed for quite a bit of exotic *
*           setups, such as larger or differently shaped boards,            *
*           different setup of pieces etc.                                  *
*                                                                           *
* Original author: Fredrik Lännergren, fredrik_lannergren@yahoo.com         *
*                                                                           *
* Please, whoever reads this, keep the original author line intact; I'd     *
* like -some- credit for the effort I put into this...                      *
* The rest of the lines, you can do with as you wish.                       *
*****************************************************************************/

#include "structs.h"
#include "utils.h"
#include "interpreter.h"
#include "comm.h"
#include "db.h"
#include "handler.h"
#include "chess.h"

// Forward declaration needed for movement checks 
int enemy_piece_threatening_king(struct char_data *ch);

// Constants

/* 
 * Note that the indexing of the chess piece types aren't
 * zero based, this is because the type numbers are used
 * for checks later on, and 0 in those cases means 'no piece'.
 */
const char chess_chars[7] = {' ', 'p', 'k', 'b', 'R', 'Q', 'K'};
const char *piece_names[] = 
{
  "    ",
  "pawn",
  "knight",
  "bishop",
  "rook",
  "queen",
  "king"
};

/*
 * [Re]sets a game of chess in its original position.
 * Parameters: The game of chess to reset
 */
void reset_game_of_chess(struct game_of_chess *game)
{
  struct chess_piece_data *piece;
  int i, j;

  game->playing = 0;  
  game->turn = CHESS_WHITE;    
  for (i = 0; i < 8; i ++) // Row
  {
    for (j = 0; j < 8; j++) // Column
    {
      game->board[i][j].color = (i+j)%2;    
      if (game->board[i][j].piece)
      {
         free(game->board[i][j].piece);
      }
      game->board[i][j].x = j;
      game->board[i][j].y = i;
      if ((i > 1) && (i < 6)) // No piece here
      {
         continue;
      }
      piece = (struct chess_piece_data*) calloc(1, sizeof(struct chess_piece_data));
      game->board[i][j].piece = piece;
      piece->color = (i < 3)?CHESS_WHITE:CHESS_BLACK;
      piece->on = &game->board[i][j];

      // Now for position checking to see what type of piece should be placed
      if ((i == 1) || (i == 6))
      {
        piece->type = CHESS_PAWN;
      }
      else 
      {
        if ((j == 0) || (j == 7))
        {
          piece->type = CHESS_ROOK;       
        }
        else if ((j == 1) || (j == 6))
        {
          piece->type = CHESS_KNIGHT;
        }
        else if ((j == 2) || (j == 5))
        {
          piece->type = CHESS_BISHOP;
        }
        else if (j == 4)
        {
          piece->type = CHESS_KING;
        }
        else
        {
          piece->type = CHESS_QUEEN;
        }
      }
    }
  }
}

/*
 * Sets up a new game of chess between two players.
 * Note that it doesn't attach the game to any player.
 * If the white player already has a game of chess, it resets that game.
 * Why white player? Because a game of chess struct is attached to the 
 * player who asks another player for a game, and the asker is automatically
 * the white player.
 *
 * Parameters: White player and black player
 * Returns: The new game of chess
 */
struct game_of_chess *setup_game_of_chess(struct char_data *playerwhite, struct char_data *playerblack)
{
  struct game_of_chess *game;
  if (!(game = playerwhite->player_specials->chess))
  {
    game = (struct game_of_chess*) calloc(1, sizeof(struct game_of_chess));
  }
  reset_game_of_chess(game);
  game->white_player = playerwhite;
  game->black_player = playerblack;
    
  return game;
}

/*
 * Frees a game of chess.
 * This can be called regardless of whether any player actually is connected
 * to the game of chess.
 *
 * Parameters: The game to free
 */
void free_game_of_chess(struct game_of_chess *chess)
{
  int i, j;
  
  if (!chess)
  {
    return;
  }
  if (chess->white_player)
  {
    chess->white_player->player_specials->chess = 0;
  }
  if (chess->black_player)
  {
    chess->black_player->player_specials->chess = 0;
  }

  // Get rid of any pieces on the board
  for (i = 0; i < 8; i ++)
  {
    for (j = 0; j < 8; j ++)
    {
      if (chess->board[i][j].piece)
      {
        free(chess->board[i][j].piece);
      }
    }
  }
  free(chess);
}

/*
 * Shows another players game of chess to an onlooker.
 * Shows the current board to the player, "graphically",
 * with the board square and the pieces on them.
 * Params: The game to show and the onlooker.
 */
void show_board_to(struct game_of_chess *chess, struct char_data *ch)
{
  int i,j;
  struct chess_square_data *square;
  struct chess_piece_data *piece;
  char bg[4],fg[4];
  send_to_char("   A  B  C  D  E  F  G  H  \r\n",ch);
  for (i = 7; i >= 0; i --)
  {
    asend_to_char(ch, "&y%c&n ", '1'+i);

    // Now all the squares on this particular row
    for (j = 0; j < 8; j ++)
    {
      square = &(chess->board[i][j]);
      piece = square->piece; 
      sprintf(bg, (square->color)? "&7": "&0");
      if (piece)
      {
        sprintf(fg, (piece->color)? "&r": "&G");
      }
      else
      {
        fg[0] = 0;
      }
      asend_to_char(ch, "%s ", bg);
      if (piece) 
      {
        sprintf(buf, "%s%s%c", fg, bg, chess_chars[piece->type]);
      }
      else
        sprintf(buf, " ");
      send_to_char(buf, ch);
      asend_to_char(ch, "%s ", bg);
    }
    send_to_char("&n", ch);
    if (i == 1)
    {
      asend_to_char(ch, " White player: %s",GET_NAME(chess->white_player));
    }
    else if (i == 3)
    {
      asend_to_char(ch, " It's %ss turn to make a move.", (chess->turn == CHESS_BLACK)?
      GET_NAME(chess->black_player):
      GET_NAME(chess->white_player));
    }
    else if (i == 4)
    {
      if (enemy_piece_threatening_king(chess->black_player))
      {
        asend_to_char(ch, " %s is under check.", GET_NAME(chess->black_player));
      }
      else if (enemy_piece_threatening_king(chess->white_player))
      {
        asend_to_char(ch, " %s is under check.", GET_NAME(chess->white_player));
      }
    }
    else if (i == 6)
    {
      asend_to_char(ch, " Black player: %s",GET_NAME(chess->black_player));
    }
    send_to_char("\r\n", ch);
  }    
}

/*
 * Checks whether there are any pieces in the way between squares a and b.
 * WARNING: Don't use this unless the path is straight!
 *          That means either true horizontal, true vertical or
 *          true diagonal. Paths such as "five steps right and three down"
 *          can't be handled by this function!
 *
 * It doesn't check the end points, only the squares in between.
 * If the move is too short to have any squares in between (= one step), it returns 0.
 *
 * Params: Origin square on the board, target square on the board,
 *         and the board itself (since there are no callback pointers)
 * Returns: 1 if there are pieces "in the way", 0 otherwise
 */
int intervening_pieces_in_path(struct chess_square_data *from, 
                               struct chess_square_data *to,
                               struct game_of_chess *chess)
{
  int x, y, climbing;

  if (!from || !to || !chess)
  {
    return 0;
  }
  if ((from->x == to->x) && (from->y == to->y))
  {
    return 0;
  }

  // Only one step (along one or both axis), can't be intervening pieces
  if ((abs(from->x - to->x) < 2) && (abs(from->y - to->y) < 2))
  {
    return 0;
  }

  if (from->y == to->y) // horizontal move
  {
    for (x = MIN(from->x, to->x) + 1; x < MAX(from->x, to->x); x ++)
    {
      if (chess->board[from->y][x].piece)
      {
        return 1;
      }
    }
    return 0;
  }

  if (from->x == to->x) // vertical move
  {
    for (y = MIN(from->y, to->y) + 1; y < MAX(from->y, to->y); y ++)
    {
      if (chess->board[y][from->x].piece)
      {
        return 1;
      }
    }
    return 0;
  }

  // Diagonal move
  // Tricky line below...Either the move is along a climbing diagonal, either way,
  // or along a falling diagonal, either way.
  climbing = ((from->y > to->y) == (from->x < to->x));
  for (x = MIN(from->x, to->x) + 1; x < MAX(from->x, to->x); x ++)
  {
    y = (climbing)? MAX(from->y, to->y) - (x - MIN(from->x, to->x)):
              MIN(from->y, to->y) + (x - MIN(from->x, to->x));
    if (chess->board[y][x].piece)
    {
      return 1;
    } 
  } 
  return 0;
}

/*
 * Determines whether a pawn can go to a certain square on the chessboard 
 *
 * Params: The piece that wishes to move, the target square it wants to
 *         move to and the board which contains both.
 * Returns: 1 if the move is allowed, 0 otherwise.
 */
int pawn_move_allowed(struct chess_piece_data *piece, 
                      struct chess_square_data *square,
                      struct game_of_chess *chess)
{
  int dir, dx, dy;
  
  if (!piece || !square || (piece->type != CHESS_PAWN))
  {
    return 0; 
  }

  dir = (piece->color == CHESS_WHITE)?1:-1; // used to determine if pawn is going 'forward'
  dx = piece->on->x - square->x;
  dy = dir*(square->y - piece->on->y); 
  // Can't go backwards, more than two steps forward or more than one step to the side, ever
  if ((dy < 0) || (dy > 2) || (dx < -1) || (dx > 1))
  {
    return 0;
  }

  // Special case: first move with pawn is allowed two steps straight ahead
  if (dy == 2)
  {
    if (dx != 0) // No sidestepping.
    {
      return 0;
    }
    // White pawn can't go two steps unless it's in its original position
    if ((piece->color == CHESS_WHITE)  && (piece->on->y != 1))
    {
      return 0;
    }
    // Black pawn can't go two steps unless it's in its original position
    if ((piece->color == CHESS_BLACK)  && (piece->on->y != 6))
    {
      return 0;
    }
    return (!intervening_pieces_in_path(piece->on, square, chess));
  }

  if (abs(dx) == 1) // One step sideways
  {
    if (dy != 1) // Can't go sideways, only one step diagonally forward
    {
      return 0;
    }
    // Can't go one step diagonally unless there's an enemy piece to strike there
    if (!square->piece)
    {
      return 0;
    }
  }
  return 1;
}

/*
 * Determines whether a knight move is allowed.
 * Params: Knight piece, square it wants to move to and the board
 *         containing both.
 * Returns: 1 if it's allowed to move to the square, 0 otherwise.
 */
int knight_move_allowed(struct chess_piece_data *piece, 
                        struct chess_square_data *square,
                        struct game_of_chess *chess)
{
  int dx, dy;
  if (!piece || !square || !chess || (piece->type != CHESS_KNIGHT))
  {
    return 0;
  }
  dx = abs(piece->on->x - square->x);
  dy = abs(piece->on->y - square->y);

  // A knight is allowed to move either one step horizontally and two 
  // vertically, or vice versa, regardless of direction.
  if (((dx == 2) && (dy == 1)) ||
      ((dx == 1) && (dy == 2)))
  {
    return 1;
  }
  return 0;
}

/*
 * Determines whether a bishop move is allowed.
 *
 * Params: Bishop piece, square it wants to move to and the board
 *         containing both.
 * Returns: 1 if it's allowed to move to the square, 0 otherwise. 
 */
int bishop_move_allowed(struct chess_piece_data *piece, 
                      struct chess_square_data *square,
                      struct game_of_chess *chess)
{
  int dx, dy;
  if (!piece || !square || !chess || (piece->type != CHESS_BISHOP))
  {
    return 0;
  }
  dx = abs(piece->on->x - square->x);
  dy = abs(piece->on->y - square->y);

  // For every step that a bishop takes along one axis, it has to take one
  // on the other as well.
  if (dx != dy)
  {
    return 0;
  }
  return (!intervening_pieces_in_path(piece->on, square, chess)); 
}


/*
 * Determines whether a rook move is allowed.
 *
 * Params: Rook piece, square it wants to move to and the board
 *         containing both.
 * Returns: 1 if it's allowed to move to the square, 0 otherwise.
 */
int rook_move_allowed(struct chess_piece_data *piece, 
                      struct chess_square_data *square,
                      struct game_of_chess *chess)
{
  int dx, dy;
  if (!piece || !square || !chess || (piece->type != CHESS_ROOK))
  {
    return 0;
  }
  dx = abs(piece->on->x - square->x);
  dy = abs(piece->on->y - square->y);

  // Can only move along one axis at a time with a rook
  if (dx && dy) 
  {
    return 0;
  }
  // Since either dx or dy is 0 here, we know it's going along a straight line,
  // therefore it's safe to call the intervening function
  return (!intervening_pieces_in_path(piece->on, square, chess));  
}


/*
 * Determines whether a queen move is allowed.
 *
 * Params: Queen piece, square it wants to move to and the board
 *         containing both.
 * Returns: 1 if it's allowed to move to the square, 0 otherwise.
 */
int queen_move_allowed(struct chess_piece_data *piece, 
                      struct chess_square_data *square,
                      struct game_of_chess *chess)
{
  int dx, dy;

  if (!piece || !square || !chess || (piece->type != CHESS_QUEEN))
  {
    return 0;
  }

  dx = abs(piece->on->x - square->x);
  dy = abs(piece->on->y - square->y);
  
  // A queen must either go like a bishop or a rook.
  if ((dx != dy) && dx && dy)
  {
    return 0;
  }
  return (!intervening_pieces_in_path(piece->on, square, chess)); 
}

/*
 * Determines whether a king move is allowed.
 * Note that this only checks whether the basic move is allowed, 
 * not whether the king would put itself in a threatened square.
 *
 * Params: King piece, square it wants to move to and the board
 *         containing both.
 * Returns: 1 if it's allowed to move to the square, 0 otherwise.
 */
int king_move_allowed(struct chess_piece_data *piece, 
                      struct chess_square_data *square,
                      struct game_of_chess *chess)
{
  int dx, dy;
  if (!piece || !square || !chess || (piece->type != CHESS_KING))
  {
    return 0;
  }

  dx = abs(piece->on->x - square->x);
  dy = abs(piece->on->y - square->y);
  // Only allowed to move one step in either direction
  if ((dx > 1) || (dy > 1))
  {
    return 0;
  }
  return 1;
}

/*
 * Entry point to check whether a piece on the board is allowed to move
 * to a certain square.
 *
 * Params: Piece, square it wants to move to and the board
 *         containing both.
 * Returns: 1 if it's allowed to move to the square, 0 otherwise.
 */
int chess_move_allowed(struct chess_piece_data *piece, 
                       struct chess_square_data *square,
                       struct game_of_chess *chess)
{
  switch (piece->type)
  {
    case CHESS_PAWN:
    return pawn_move_allowed(piece, square, chess);
    case CHESS_KNIGHT:
    return knight_move_allowed(piece, square, chess);
    case CHESS_BISHOP:
    return bishop_move_allowed(piece, square, chess);
    case CHESS_ROOK:
    return rook_move_allowed(piece, square, chess);
    case CHESS_QUEEN:
    return queen_move_allowed(piece, square, chess);    
    case CHESS_KING: 
    return king_move_allowed(piece, square, chess);
  }
  return 0;
}

/*
 * Determines whether a certain square on the chess board can be walked
 * to by any of the opponents pieces in the next move.
 * This is a helper function used to see whether the king is threatened.
 *
 * Params: The player who wants to check for opponents 'coverage',
 *         the square to check
 * Returns: 1 if there is an enemy piece which can reach the square
 *          in the next move.
 */
int enemy_piece_covering_square(struct char_data *ch, struct chess_square_data *square)
{
  struct game_of_chess *chess;
  struct chess_piece_data *piece;
  int opp_color, x, y;

  if (!ch || !square || !ch->player_specials->chess)
  {
    return 1;
  }

  chess = ch->player_specials->chess;
  opp_color = (ch == chess->white_player)?CHESS_BLACK:CHESS_WHITE;
  for (y = 0; y < 8; y ++)
  {
    for (x = 0;x < 8; x++)
    {
      piece = chess->board[y][x].piece;
      if (piece && (piece->color == opp_color))
      {
        if (chess_move_allowed(piece, square, chess))
        {
          return piece->type;
        }
      }
    }
  }
  return 0;
}

/*
 * Makes a short castling
 *
 * Params: The player wanting the move.
 * Returns: 1 if the move was successful, 0 otherwise (not used anywhere)
 */
int shortcastle(struct char_data *ch)
{
  struct chess_piece_data *rook, *king;
  struct game_of_chess *chess;
  struct char_data *opp;
  int y, color;
   
  if (!ch || !ch->player_specials->chess)
  {
    return 0;
  }
  chess = ch->player_specials->chess;
 
  y = (chess->white_player == ch)? 0: 7; 
  color = (chess->white_player == ch)? CHESS_WHITE: CHESS_BLACK;
  rook = chess->board[y][7].piece;
  king = chess->board[y][4].piece;
  if (!rook || (rook->type != CHESS_ROOK) || (rook->color != color))
  {
    send_to_char("You can't, the rook is no longer in its original place.\r\n", ch);
    return 0;
  }
  if (!king || (king->type != CHESS_KING) || (king->color != color))
  {
    send_to_char("You can't, the king is no longer in its original place.\r\n", ch);
    return 0;
  }
  if (king->flags & CHESS_FLAG_KING_MOVED)
  {
    send_to_char("You can't, the king isn't allowed castling after moving.\r\n", ch);
    return 0;
  }
  if (rook->flags & CHESS_FLAG_ROOK_MOVED)
  {
    send_to_char("You can't, the rook isn't allowed castling after moving.\r\n", ch);
    return 0;  
  }
  if (enemy_piece_threatening_king(ch))
  {
    send_to_char("You can't, you're under check.\r\n", ch);
    return 0;
  }
  if (chess->board[y][6].piece || chess->board[y][5].piece)
  {
    send_to_char("You can't, there are pieces in the way.\r\n", ch);
    return 0;
  }
  if (enemy_piece_covering_square(ch, &chess->board[y][5]) ||
      enemy_piece_covering_square(ch, &chess->board[y][6]))
  {
    send_to_char("You can't, your opponent is covering the squares you intend to use.\r\n", ch);
    return 0;
  }
  opp = (ch == chess->white_player)? chess->black_player: chess->white_player;

  send_to_char("Ok, you make a short castling.\r\n", ch);
  asend_to_char(opp, "%s makes a short castling.\r\n", GET_NAME(ch));

  rook->on->piece = 0;
  rook->on = &chess->board[y][5];
  rook->flags = CHESS_FLAG_ROOK_MOVED;
  chess->board[y][5].piece = rook;

  king->on->piece = 0;
  king->on = &chess->board[y][6];
  king->flags = CHESS_FLAG_KING_MOVED;
  chess->board[y][6].piece = king;
  
  chess->turn = (chess->turn == CHESS_BLACK)?CHESS_WHITE:CHESS_BLACK;
  return 1;
}

/*
 * Makes a long castling
 *
 * Params: The player wanting the move.
 * Returns: 1 if the move was successful, 0 otherwise (not used anywhere)
 */
int longcastle(struct char_data *ch)
{
  struct chess_piece_data *rook, *king;
  struct game_of_chess *chess;
  struct char_data *opp;
  int y, color;
   
  if (!ch || !ch->player_specials->chess)
  {
    return 0;
  }
  chess = ch->player_specials->chess;
 
  y = (chess->white_player == ch)? 0: 7; 
  color = (chess->white_player == ch)? CHESS_WHITE: CHESS_BLACK;
  rook = chess->board[y][0].piece;
  king = chess->board[y][4].piece;
  if (!rook || (rook->type != CHESS_ROOK) || (rook->color != color))
  {
    send_to_char("You can't, the rook is no longer in its original place.\r\n", ch);
    return 0;
  }
  if (!king || (king->type != CHESS_KING) || (king->color != color))
  {
    send_to_char("You can't, the king is no longer in its original place.\r\n", ch);
    return 0;
  }
  if (king->flags & CHESS_FLAG_KING_MOVED)
  {
    send_to_char("You can't, the king isn't allowed castling after moving.\r\n", ch);
    return 0;
  }
  if (rook->flags & CHESS_FLAG_ROOK_MOVED)
  {
    send_to_char("You can't, the rook isn't allowed castling after moving.\r\n", ch);
    return 0;  
  }
  if (chess->board[y][1].piece || chess->board[y][2].piece || 
      chess->board[y][3].piece)
  {
    send_to_char("You can't, there are pieces in the way.\r\n", ch);
    return 0;
  }
  if (enemy_piece_covering_square(ch, &chess->board[y][2]) ||
      enemy_piece_covering_square(ch, &chess->board[y][3]))
  {
    send_to_char("You can't, your opponent is covering the squares you intend to use.\r\n", ch);
    return 0;
  }
  opp = (ch == chess->white_player)? chess->black_player: chess->white_player;

  send_to_char("Ok, you make a long castling.\r\n", ch);
  asend_to_char(opp, "%s makes a long castling.\r\n", GET_NAME(ch));

  rook->on->piece = 0;
  rook->on = &chess->board[y][3];
  rook->flags = CHESS_FLAG_ROOK_MOVED;
  chess->board[y][3].piece = rook;

  king->on->piece = 0;
  king->on = &chess->board[y][2];
  king->flags = CHESS_FLAG_KING_MOVED;
  chess->board[y][2].piece = king;
  
  chess->turn = (chess->turn == CHESS_BLACK)?CHESS_WHITE:CHESS_BLACK;
  return 1;
}

/*
 * Determines whether the players king is threatened by any enemy piece
 * on the chessboard.
 * Prerequisites: ch must be a player currently playing chess with someone
 * If the players king is no longer on the board, it returns 0...
 * Nobody said this code is complete ;)
 *
 * Otherwise, if a piece is threatening the king, it returns
 * the type of the piece threatening that area.
 */
int enemy_piece_threatening_king(struct char_data *ch)
{
  struct game_of_chess *chess;
  int color, x, y;
  
  if (!ch || !ch->player_specials->chess)
  {
    return 0;
  }
  chess = ch->player_specials->chess;
  color = (ch == chess->white_player)?CHESS_WHITE:CHESS_BLACK;

  // First find the king
  for (y = 0; y < 8; y ++)
  {
    for (x = 0; x < 8; x ++)
    {
      if (chess->board[y][x].piece && 
         (chess->board[y][x].piece->type == CHESS_KING) &&
         (chess->board[y][x].piece->color == color))
      {  
        return enemy_piece_covering_square(ch, &chess->board[y][x]);
      }
    }
  }  
  return 0;
}

/*
 * The actual command called. It lets the players ask for a game of
 * chess, accept a request for a game of chess, move pieces, 
 * pass, resign and see the board.
 */
ACMD(do_chess)
{
  char arg1[100], arg2[100], *arg3;
  struct game_of_chess *chess;
  struct chess_piece_data *piece, *taken_piece;
  struct chess_square_data *square, *last_square;
  struct char_data *opp = NULL;
  int row, column, type;

  if (IS_NPC(ch))
  {
    send_to_char("You fuckup! You're a mob! You can't play chess!\r\n", ch);
    return;
  }

  skip_spaces(&argument);
  arg3 = two_arguments(argument, arg1, arg2);
  chess = ch->player_specials->chess;

  if (chess)
  {
    opp = (chess->white_player == ch)?chess->black_player:chess->white_player;
  }

  if (!*arg1)
  {
    send_to_char("Valid syntax is:\r\n", ch);
    send_to_char("chess watch <player>\r\n", ch);
    send_to_char("chess ask <opponent>\r\n", ch);
    send_to_char("chess accept <opponent>\r\n", ch);
    send_to_char("chess show\r\n", ch);
    send_to_char("chess resign\r\n", ch);
    send_to_char("chess pass\r\n", ch);
    send_to_char("chess move <pos1> <pos2>\r\n", ch);
    send_to_char("chess move longcastle\r\n", ch);
    send_to_char("chess move shortcastle\r\n", ch);
    return;
  }
   
  if (!strncasecmp(arg1, "watch", strlen(arg1)))
  {
    opp = get_player_vis(ch, arg2, ch->in_room);
    if (!opp)
    {
      send_to_char("No player by that name here.\r\n", ch);
      return;
    }
    if (IS_NPC(opp) || !opp->player_specials->chess || 
        !opp->player_specials->chess->playing)
    {
      asend_to_char(ch, "%s isn't playing chess.\r\n", GET_NAME(opp));
      return;
    }
    show_board_to(opp->player_specials->chess, ch);
    return;
  }

  if (!strncasecmp(arg1, "ask", strlen(arg1)))
  {
    // A heap of sanity checks below...
    if (chess && chess->playing)
    {
      asend_to_char(ch, "You're already playing chess with %s.\r\n", GET_NAME(opp));
      return;
    }
    opp = get_player_vis(ch, arg2, ch->in_room);
    if (!opp)
    {
      send_to_char("No player by that name here.\r\n", ch);
      return;
    }
    // You can skip this check, I just thought it silly that anyone
    // would want to play chess with himself...
    if (opp == ch)
    {
      send_to_char("Sorry, not allowed to play solo.\r\n", ch);
      return;
    }
    if (opp->player_specials->chess && (opp->player_specials->chess->playing))
    {
      asend_to_char(ch, "%s is already playing a game of chess with someone else.\r\n", GET_NAME(opp));
      return;
    } 
    asend_to_char(ch, "You ask to play chess with %s.\r\n", GET_NAME(opp));
    asend_to_char(opp, "%s asks to play chess with you.\r\n", GET_NAME(ch));
    if (chess)
    {
      reset_game_of_chess(chess);
      chess->white_player = ch; 
      chess->black_player = opp;
    }
    else
    {
      ch->player_specials->chess = setup_game_of_chess(ch, opp);
      chess = ch->player_specials->chess;
    }
    chess->playing = 0;
    return;
  } // "ask"

  if (!strncasecmp(arg1, "accept", strlen(arg1)))
  {
    // Another heap of sanity checks...
    if (ch->player_specials->chess && ch->player_specials->chess->playing)
    {
      asend_to_char(ch, "You are already playing chess with %s.\r\n", opp);
      return;
    }
    opp = get_player_vis(ch, arg2, ch->in_room);
    if (!opp)
    {
      send_to_char("No player by that name here.\r\n", ch);
      return;
    }
    // You can skip this check, I just thought it silly that anyone
    // would want to play chess with himself...
    if (opp == ch)
    {
      send_to_char("Sorry, not allowed to play solo.\r\n", ch);
      return;
    }
    chess = opp->player_specials->chess;
    if (!chess || (chess->black_player != ch))
    {
      asend_to_char(ch, "But %s has not asked to play chess with you...", GET_NAME(opp));
      return;
    }
    if (chess->playing)
    {
      asend_to_char(ch, "%s is already playing a game of chess with someone else.\r\n", GET_NAME(opp));
      return;
    }
    asend_to_char(ch, "You accept %ss offer to play chess.\r\n", GET_NAME(opp));
    asend_to_char(opp, "%s accepts your offer to play chess.\r\n", GET_NAME(ch));
    if (ch->player_specials->chess)
    {
      free_game_of_chess(ch->player_specials->chess);
    }
    ch->player_specials->chess = chess;
    chess->playing = 1;
    return;
  } // accept

  if (!strncasecmp(arg1, "resign", strlen(arg1)))
  {
    if (!chess || !chess->playing)
    {
      send_to_char("Resign from what? Your job? You are not playing any chess.\r\n", ch);
      return;
    }
    send_to_char("You resign this game of chess.\r\n", ch);
    asend_to_char(opp, "%s resigns this game of chess.\r\n", GET_NAME(ch));
    free_game_of_chess(chess);
    return;
  } // resign
   
  if (!strncasecmp(arg1, "show", strlen(arg1)))
  {
    if (chess && chess->playing)
    {
      show_board_to(chess, ch);
    }
    else
    {
      send_to_char("Show what? You're not playing any chess at the moment.\r\n", ch);
    }
    return;
  } // show
   
  if (!strncasecmp(arg1, "move", strlen(arg1)))
  {
    if (!chess || !chess->playing)
    {
      send_to_char("You should start a game of chess before moving pieces around...\r\n", ch);
      return;
    }
    // First a turn check...
    if ((chess->turn == CHESS_BLACK && chess->black_player != ch) ||
        (chess->turn == CHESS_WHITE && chess->white_player != ch))
    {
      send_to_char("It's not your turn to move.\r\n", ch);
      return;
    }
    // Now a basic syntax check...
    if ((strlen(arg2) > 1) && !strncasecmp(arg2, "shortcastling", strlen(arg2)))
    {
      shortcastle(ch);
      return;
    }
    if ((strlen(arg2) > 1) && !strncasecmp(arg2, "longcastling", strlen(arg2)))
    {
      longcastle(ch);
      return;
    }
    if (!*arg2 || !arg3 || !*arg3 || strlen(arg2) < 2 || strlen(arg3) < 2)
    {
      send_to_char("Valid move syntax is 'move <pos1> <pos2>'\r\n", ch);
      send_to_char("                  or 'move shortcastle'\r\n", ch);
      send_to_char("                  or 'move longcastle'\r\n", ch);
      send_to_char("Example: chess move a2 a4\r\n", ch);
      send_to_char("Only lowercase letters for the positions allowed.\r\n", ch);
      return;
    }
    column = arg2[0]-'a';
    row = arg2[1]-'1';
    // Now a sanity check for positions on the board...
    if ((row < 0) || (row > 7) || (column < 0) || (column > 7))
    {
      asend_to_char(ch, "'%s' is an invalid position on the board.\r\n", arg2);
      return;
    }
    piece = chess->board[row][column].piece;

    // Now a sanity check for actual pieces on the selected square...
    if (!piece || 
        (piece->color == CHESS_WHITE && chess->white_player != ch) ||
        (piece->color == CHESS_BLACK && chess->black_player != ch))
    {
      asend_to_char(ch, "You don't have any piece on square %s.\r\n", arg2);
      return;
    }
    skip_spaces(&arg3);
    column = arg3[0]-'a';
    row = arg3[1]-'1';
    if ((row < 0) || (row > 7) || (column < 0) || (column > 7))
    {
      asend_to_char(ch, "'%s' is an invalid target position on the board.\r\n", arg3);
      return;
    }
    square = &(chess->board[row][column]);

    // Now a collision sanity check for target square...
    if (square->piece && square->piece->color == piece->color)
    {
      asend_to_char(ch, "You can't, you already have a piece on %s.\r\n", arg3);
      return;
    }

    // Now a more complex movement verification check...
    if (chess_move_allowed(piece, square, chess))
    {

      // We must check whether the players king would [still] be under threat
      // after the move. If that happens, the move is still invalid, even though
      // the basic movement rules allow for the move.

      taken_piece = square->piece;
      last_square = piece->on;
      piece->on->piece = 0;
      piece->on = square;
      square->piece = piece;

      if ((type = enemy_piece_threatening_king(ch))) // Move back and notify player
      {
         send_to_char("That move isn't allowed, since your king would\r\n", ch);
         asend_to_char(ch, "be under threat from %s's %s after the move.\r\n", GET_NAME(opp), 
                       piece_names[type]);
         square->piece = taken_piece; 
         last_square->piece = piece;
         piece->on = last_square;
         return;
      }
      asend_to_char(ch, "You move your %s from %s to %s.\r\n", piece_names[piece->type], arg2, arg3);
      asend_to_char(opp, "%s moves %s from %s to %s.\r\n", GET_NAME(ch),
                    piece_names[piece->type], arg2, arg3);
      if (taken_piece)
      {
        asend_to_char(ch, "You take %s's %s.\r\n", GET_NAME(opp), piece_names[taken_piece->type]);
        asend_to_char(opp, "%s takes your %s.\r\n", GET_NAME(ch), piece_names[taken_piece->type]);
        free(taken_piece);
      }
      // These flags are for checking future castling
      if (piece->type == CHESS_KING)
      {
        piece->flags = CHESS_FLAG_KING_MOVED;
      }
      else if (piece->type == CHESS_ROOK)
      {
        piece->flags = CHESS_FLAG_ROOK_MOVED;
      }
      else if (piece->type == CHESS_PAWN) // pawn promotion check
      {
        if (((piece->color == CHESS_BLACK) && (piece->on->y == 0)) ||
            ((piece->color == CHESS_WHITE) && (piece->on->y == 7)))
        {
          piece->type = CHESS_QUEEN;
          asend_to_char(ch, "Pawn is promoted to queen.\r\n");
          asend_to_char(opp, "Pawn is promoted to queen.\r\n");
        }
      }
      if (enemy_piece_threatening_king(opp))
      {
        send_to_char("Check!\r\n", ch);
        send_to_char("Check!\r\n", opp);
      }
      chess->turn = (chess->turn == CHESS_BLACK)?CHESS_WHITE:CHESS_BLACK;
    }
    else
    {
      send_to_char("That move is not allowed.\r\n", ch);
    }
    return;
  } // move 
  else if (!strncasecmp(arg1, "pass", strlen(arg1)))
  {
    if ((chess->white_player == ch) == (chess->turn == CHESS_BLACK))
    {
      send_to_char("It's not your turn...\r\n", ch);
      return;
    }
    send_to_char("You pass your turn.\r\n", ch);
    asend_to_char(opp, "%s passes.\r\n", GET_NAME(ch));
    chess->turn = (chess->turn == CHESS_BLACK)?CHESS_WHITE:CHESS_BLACK;
    return;
  }
  // No valid argument to "chess", show syntax...

  send_to_char("Valid syntax is:\r\n", ch);
  send_to_char("chess watch <player>\r\n", ch);
  send_to_char("chess ask <opponent>\r\n", ch);
  send_to_char("chess accept <opponent>\r\n", ch);
  send_to_char("chess show\r\n", ch);
  send_to_char("chess resign\r\n", ch);
  send_to_char("chess pass\r\n", ch);
  send_to_char("chess move <pos1> <pos2>\r\n", ch);
  send_to_char("chess move longcastling\r\n", ch); 
  send_to_char("chess move shorcastling\r\n", ch);
}
