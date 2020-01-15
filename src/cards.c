#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "handler.h"
#include "cards.h"

char *allowed_while_playing[] =
{
  "card",
  "gdt",
  "help",
  "ooc",
  "score"
};

char *card_names[] =
{
  "Ace",
  "Two",
  "Three",
  "Four",
  "Five",
  "Six",
  "Seven",
  "Eight",
  "Nine",
  "Ten",
  "Jack",
  "Lady",
  "Lord"
};

char *card_color_names[] =
{
  "Clubs",
  "Diamonds",
  "Hearts",
  "Spades"
};

struct card_game *new_game()
{
  int i, j;
  struct card_game *game = (struct card_game *) calloc(1, sizeof(struct card_game));
  
  for (i = 0; i < 4; i ++)
  {
    for (j = 0; j < 13; j ++)
    {
      game->deck.cards[i][j].name = card_names[j];    
      game->deck.cards[i][j].color = card_color_names[i];
    }
  }
    
  return game;
}

void start_game(struct char_data *ch, int max_players)
{
    
   GET_GAME(ch) = new_game();
   GET_GAME(ch)->max_players = max_players;
   GET_GAME(ch)->players = (struct char_data **) calloc(2, sizeof(struct char_data *));
   GET_GAME(ch)->players[0] = ch;
    
   act("$n has started a new game of cards.", TRUE, ch, 0, 0, TO_ROOM);
   send_to_char("You start a new game of cards.\r\n", ch);
}

void end_game(struct card_game *game)
{
  int i;
    
  if (!game)
  {
    return;
  }
    
  for (i = 0; game->players[i] && (i < game->max_players); i ++)
  {
     if (i == 0)
     {
       act("$n has ended the game of cards.", TRUE, game->players[i], 0, 0, TO_ROOM);       
       send_to_char("You have ended the game of cards.", game->players[i]);
     }
     GET_GAME(game->players[i]) = NULL;
     game->players[i] = NULL;
  }
  free(game);
}

void add_player(struct card_game *game, struct char_data *ch)
{
  int current;
    
  if (!game || !ch || IS_NPC(ch))
  {
    return;
  }
  for (current = 0; game->players[current]; current ++);
    
  if (game->max_players <= current)
  {
    send_to_char("You can't join that game of cards, it has reached its max amount of players.\r\n", ch);
    return;
  }
  game->players = realloc(game->players, (current + 2)*sizeof(struct char_data *));
  game->players[current] = ch;
  game->players[current + 1] = NULL;
  GET_GAME(ch) = game;
  act("$n has joined the game of cards held by $N.", TRUE, ch, 0, game->players[0], TO_NOTCHAR);
  asend_to_char(ch, "You join the game of cards held by %s.\r\n", GET_NAME(game->players[0]));
}

void drop_all_cards(struct char_data *ch)
{
  int i, j;
  struct card_game *game;
    
  if (!ch || IS_NPC(ch) || !GET_GAME(ch))
  {
    return;      
  }
  game = GET_GAME(ch);
    
  for (i = 0; i < 4; i ++)
  {
    for (j = 0; j < 13; j ++)
    {
      if (game->deck.cards[i][j].held_by == ch)
      {
        game->deck.cards[i][j].held_by = NULL;
        game->deck.cards[i][j].used = TRUE;
      }
    }
  }  
}

void remove_player(struct char_data *ch)
{
  int i = 0, j = 0, num = 0;
  struct char_data **newArray;
  struct card_game *game;
    
  if (!ch || IS_NPC(ch) || !GET_GAME(ch))
  {
    return;      
  }
  game = GET_GAME(ch);
  if (game->players[0] == ch)
  {
    send_to_char("You cannot leave a game of cards you started; you must end it.", ch);
    return;
  }
    
  act("$n has left the game of cards, dropping the cards $e holds.", TRUE, ch, 0, 0, TO_ROOM);
  send_to_char("You leave the game of cards, dropping the cards.", ch); 
  for (num = 0; game->players[num]; num ++);
  newArray = (struct char_data **) calloc(num, sizeof(struct char_data *));
    
  for (i = 0; game->players[i]; i ++)
  {
    if ((game->players[i] != ch) && char_still_online(game->players[i]))
    {
        newArray[j++] = game->players[i];
    }
  }
  drop_all_cards(ch);
  free(game->players);
  game->players = newArray;    
}

void send_to_card_players(char *message, struct card_game *game)
{
  int i,j;
  
  if (!message || !game)
  {
    return;      
  }
    
  for (i = 0; game->players[i]; i ++)
  {
    if (char_still_online(game->players[i]))
      send_to_char(message, game->players[i]);
    else // remove lost player.
    {    
      for (j = i+1; game->players[j]; j ++)
        game->players[j-1] = game->players[j];
      game->players[j-1] = NULL;
    }
  }
}


int drop_one_card(struct char_data *ch, int num)
{
  int i = 0, j = 0, count = 0;
  struct card_game *game;
  char mybuf[100];

  if (!ch || IS_NPC(ch) || !GET_GAME(ch))
  {
    return FALSE;      
  }
  game = GET_GAME(ch);
    
  for (i = 0; i < 4; i ++)
  {
    for (j = 0; j < 13; j ++)
    {
      if (game->deck.cards[i][j].held_by == ch)
      {
        count ++;
        if (count == num)
        {
          game->deck.cards[i][j].held_by = NULL;
          game->deck.cards[i][j].used = TRUE;
          asend_to_char(ch, "You drop %s of %s.\r\n", game->deck.cards[i][j].name, game->deck.cards[i][j].color);
          sprintf(mybuf, "%s drops a card.\r\n", GET_NAME(ch));
          send_to_card_players(mybuf, game);
          return TRUE;
        }
      }
    }
  }
    
  asend_to_char(ch, "You only have %d card%s in your hand, and can't drop #%d.\r\n", count, (count == 1)?"":"s", num);
  return FALSE;
}

int any_cards_left_in_deck(struct card_game *game)
{
  int i, j;
    
  for (i = 0; i < 4; i ++)
    for (j = 0; j < 13; j ++)
      if (!game->deck.cards[i][j].held_by && !game->deck.cards[i][j].used)
         return TRUE;
            
  return FALSE;
}

void get_one_card(struct char_data *ch)
{
  int i = 0, j = 0, found = 0;
  struct card_game *game;
  char mybuf[100];

  if (!ch || IS_NPC(ch) || !GET_GAME(ch))
  {
    return;      
  }
  game = GET_GAME(ch);
    
  if (!any_cards_left_in_deck(game))
  {
    send_to_char("There are no cards left in the deck...\r\n", ch);
    return;        
  }
    
  while (!found)
  {
     i = number(0,3);
     j = number(0, 12);
     if (!game->deck.cards[i][j].held_by && !game->deck.cards[i][j].used)
       found = TRUE;
  }
  asend_to_char(ch, "You got %s of %s.\r\n", game->deck.cards[i][j].name,game->deck.cards[i][j].color);
  sprintf(mybuf, "%s gets a card.\r\n", GET_NAME(ch));
  send_to_card_players(mybuf, game);
  game->deck.cards[i][j].held_by = ch;
}

void remove_one_card(struct char_data *ch, char *value, char *color)
{
  int i, j;
  char mybuf[100];
  struct card_game *game;
  struct card_data *card;

  if (!ch || IS_NPC(ch) || !GET_GAME(ch) || !color || !value)
  {
    return;
  }
  game = GET_GAME(ch);

  for (i = 0; i < 4; i ++)
    for (j = 0; j < 13; j ++)
    {
      card = &game->deck.cards[i][j];
      if (!strncasecmp(card->color, color, strlen(color)) &&
          !strncasecmp(card->name, value, strlen(value)))
      {
        if (card->held_by)
        {
          asend_to_char(ch, "That card is already held by %s.\r\n", GET_NAME(card->held_by));
          return;
        }
        if (card->used)
        {
          send_to_char("That card is already used and out of the deck.\r\n", ch);
          return;
        }
        card->used = TRUE;
        sprintf(mybuf, "%s picks out %s of %s from the deck, putting it on the discarded pile.\r\n",
                GET_NAME(ch), card->name, card->color);
        send_to_card_players(mybuf, game);
        return;
      }
    }
  asend_to_char(ch, "There is no card in the deck named '%s' of '%s'.\r\n", value, color);
}

void shuffle_deck(struct card_game *game)
{
  int i, j;
    
  for (i = 0; i < 4; i ++)
    for (j = 0; j < 13; j ++)
      if (!game->deck.cards[i][j].held_by)
        game->deck.cards[i][j].used = FALSE;
}

void replace_one_card(struct char_data *ch, int num)
{
  struct card_game *game;
  if (!ch || IS_NPC(ch) || !GET_GAME(ch))
  {
    return;      
  }
  game = GET_GAME(ch);
  if (!any_cards_left_in_deck(game))
  {
    send_to_char("There are no cards left in the deck...\r\n", ch);
    return;
  }
    
  if (drop_one_card(ch, num))
  {
    get_one_card(ch);      
  }
}

void show_all_cards(struct char_data *ch, int to_room)
{
  struct card_game *game;
  char mybuf[15000];
  int i, j, count = 0;
    
  if (!ch || IS_NPC(ch) || !GET_GAME(ch))
  {
    return;
  }
  game = GET_GAME(ch);
  if (to_room)
  {
    sprintf(mybuf, "%s shows his cards, which are: ", GET_NAME(ch));
  }  
  else
  {
    sprintf(mybuf, "You currently have the following cards: \r\n");     
  }

  for (i = 0; i < 4; i ++)
  {
    for (j = 0; j < 13; j++)
    {
      if (game->deck.cards[i][j].held_by == ch)
      {
        if (to_room)
        {
          sprintf(mybuf, "%s %s of %s, ", mybuf, game->deck.cards[i][j].name, game->deck.cards[i][j].color);
          game->deck.cards[i][j].held_by = NULL;
          game->deck.cards[i][j].used = TRUE;
        }
        else 
        {
          count ++;
          sprintf(mybuf, "%s %2d: %s of %s\r\n", mybuf, count, game->deck.cards[i][j].name, game->deck.cards[i][j].color);
        }  
      }
    }
  }
  if (to_room)
  {
    send_to_card_players(mybuf, game);
  }
  else
  {
    send_to_char(mybuf, ch);
  }
}

void play_one_card(struct char_data *ch, int num)
{
  struct card_game *game;
  char mybuf[500];
  int i, j, count = 0;
          
  if (!ch || IS_NPC(ch) || !GET_GAME(ch))
  {
    return;
  }
  game = GET_GAME(ch);
  for (i = 0; i < 4; i ++)  
  {
    for (j = 0; j < 13; j++)
    {
      if (game->deck.cards[i][j].held_by == ch)  
      {
        count++;
        if (count == num)
        {
          sprintf(mybuf, "%s plays %s of %s.\r\n", GET_NAME(ch),game->deck.cards[i][j].name, game->deck.cards[i][j].color);
          game->deck.cards[i][j].held_by = NULL;
          game->deck.cards[i][j].used = TRUE;
          send_to_card_players(mybuf, game);
          return;
        }
      }
    }
  }

  if (count < num)
  {
    send_to_char("Can't play it, you don't have that many cards!\r\n", ch);
  }
}

ACMD(do_cardgame)
{
  char argOne[100], argTwo[100], argThree[100], argFour[100];
  int maxNum= 0, i;
  struct char_data *holder;
    
  // two_arguments(argument, argOne, argTwo);
  sprintf(argTwo, one_argument(argument, argOne));
  if (!GET_GAME(ch) && strncasecmp(argOne, "start", strlen(argOne)) && strncasecmp(argOne, "join", strlen(argOne)))
  {
    send_to_char("You must either 'join' or 'start' a game of cards first...", ch);
    return;
  }
  
  if (!strncasecmp(argOne, "start", strlen(argOne)))
  {
    if (GET_GAME(ch))
    {
      send_to_char("You are already in a game of cards. Stop playing that one first.\r\n", ch);
      return;
    }
    if ((maxNum = atoi(argTwo)) < 2)
    {
      send_to_char("You can't have less than two players in a game of cards!\r\n", ch);
      return;
    }
    start_game(ch, maxNum);
    return;
  }
    
  if (!strncasecmp(argOne, "join", strlen(argOne)))
  {
    if (GET_GAME(ch))
    {
      send_to_char("You are already in a game of cards. Stop playing that one first.\r\n", ch);
      return;
    }
    if (!(holder = get_char_room_vis(ch, argTwo)))
    {
      send_to_char("That person seems to not be here at the moment...\r\n", ch);
      return;
    }
    if (!GET_GAME(holder))
    {
      asend_to_char(ch, "%s isn't playing cards at the moment...\r\n", GET_NAME(holder));
    }
    add_player(GET_GAME(holder), ch);
    return;
  }
  
  if (!strncasecmp(argOne, "leave", strlen(argOne)))
  {
    remove_player(ch);
    return;
  }
    
  if (!strncasecmp(argOne, "end", strlen(argOne)))
  {
    if (GET_GAME(ch)->players[0] != ch)
    {
      asend_to_char(ch, "Only %s can end the game, since %s started it.\r\n", GET_NAME(GET_GAME(ch)->players[0]), HSSH(GET_GAME(ch)->players[0]));
      send_to_char("If you want to stop playing, just type 'cardgame leave' instead.\r\n", ch);
      return;
    }
    send_to_char("Card game ended.\r\n",ch);
    end_game(GET_GAME(ch));    
    return;
  }
    
  if (!strncasecmp(argOne, "get", strlen(argOne)))
  {
    if ((maxNum = atoi(argTwo)) < 1)
    {
      send_to_char("Try getting a reasonable number of cards...\r\n", ch);
      return;
    }
    asend_to_char(ch, "You get %d card[s] from the deck.\r\n", maxNum);
    for (i = 0; i < maxNum; i ++)
    {
      get_one_card(ch);
    }
    return;
  }
  
  if (!strncasecmp(argOne, "replace", strlen(argOne)))
  {
    if ((i = atoi(argTwo)) < 1)
    {
      send_to_char("Try replacing a card # you possibly -can- have instead...\r\n", ch);
    }
    replace_one_card(ch, i);
    return;      
  }
    
  if (!strncasecmp(argOne, "drop", strlen(argOne)))
  {
    if (!strncasecmp(argTwo, "all", strlen(argTwo)))
    {
        act("$n drops all $s cards.", TRUE, ch, 0, 0, TO_ROOM);
        send_to_char("You drop all the cards you hold.\r\n", ch);
        drop_all_cards(ch);
        return;
    }
    if ((i = atoi(argTwo)) < 1)
    {
      send_to_char("Try dropping a card # you possibly -can- have instead...\r\n", ch);
    }
    drop_one_card(ch, i);
    return;
  }

  if (!strncasecmp(argOne, "list", strlen(argOne)))
  {
    show_all_cards(ch, FALSE);
    return;
  }
   
  if (!strncasecmp(argOne, "show", strlen(argOne)))
  {
    if ((i = atoi(argTwo)) > 0)
    {
      play_one_card(ch, i);
    }
    else
    {
      show_all_cards(ch, TRUE);
    }
    return;
  }
    
  if (!strncasecmp(argOne, "shuffle", strlen(argOne)))
  {
    act("$n shuffles the deck of cards.", TRUE, ch,  0, 0, TO_ROOM);
    send_to_char("You shuffle the cards not held by anyone.\r\n", ch);
    shuffle_deck(GET_GAME(ch));  
    return;
  }

  if (!strncasecmp(argOne, "players", strlen(argOne)))
  {
    send_to_char("Players currently in the game of cards:\r\n", ch);
    for (i = 0; GET_GAME(ch)->players[i]; i ++)
    {
      asend_to_char(ch, "%s\r\n", GET_NAME(GET_GAME(ch)->players[i]));
    }
    return;
  }
  if (!strncasecmp(argOne, "remove", strlen(argOne)))
  {
    two_arguments(argTwo, argThree, argFour);
    if (!*argThree || !*argFour)
    {
      send_to_char("Remove what card from the deck?\r\n", ch);
      return;
    }
    remove_one_card(ch, argThree, argFour);    
    return;
  }
  send_to_char("Hmm...no such command as argument to 'cardgame'...See the help text.\r\n", ch);
}
