#ifndef __THREADBOARD_H__
#define __THREADBOARD_H__

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/entities.h>
#include <libxml/parserInternals.h>
#include <errno.h>
#include "interpreter.h"

struct threadMessage {
  char *header;
  char *author;
  char *contents;
  char *date;
  int  number;
  struct threadMessage *replies;
  struct threadMessage *replyTo;
  struct threadMessage *next;
  struct threadBoard *board;
};

typedef struct threadMessage thrMsg;

struct threadBoard {
  int vnum;
  int top;
  char *name;
  struct threadMessage *messages;
  struct threadBoard *next;
};

typedef struct threadBoard thrBoard;


extern void readBoards();
extern void saveBoard(thrBoard* aBoard);
extern ACMD(do_thrlist);
extern SPECIAL(threadboard);

#endif
