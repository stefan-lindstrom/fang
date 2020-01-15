%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strfunc.h"

int _sf_cfg_i_nline = 0;
int yylex();
int yyerror(char *);

extern svect *_sf_cfg_i_a;
extern svect *_sf_cfg_i_v;

%}

/*
 * Token value definition
 */
%union {
	char *tv_char;
	svect *tv_svect;
}

/*
 * Token types returned by scanner
 */
%token	<tv_char>	TOK_STRING
%type	<tv_svect>	Attrs

/*
 * Reserved words
 */
%token				EQ
%token				LB
%token				RB
%token				COMMA
%token				COLON
%token				SEMICOLON

%%
cfg_file:
	| sequence;

sequence: block
	| sequence block
	;

eq:
	| EQ
	;

ps:
	| SEMICOLON
	;

cs:
	| COMMA
	;

block:
	TOK_STRING eq TOK_STRING ps {
		if(sadd_attach(_sf_cfg_i_a, $1, strlen($1)) == -1
		 || sadd_attach(_sf_cfg_i_v, $3, strlen($3)) == -1
		) {
			return -1;
		}
	}
	| TOK_STRING eq LB Attrs RB ps {
		int i;

		for(i = 0; i < ((svect *)$4)->count; i++) {
			if(sadd(_sf_cfg_i_a, $1) == -1
			 || sadd(_sf_cfg_i_v, ((svect *)$4)->list[i]) == -1
			) {
				return -1;
			}
		}
		free($1);
		sfree((svect *)$4);
	}
	;

Attrs:
	TOK_STRING {
		$$ = sinit();
		if($$ == NULL)
			return -1;
		if(sadd_attach($$, $1, strlen($1)) == -1)
			return -1;
	}
	| Attrs cs TOK_STRING {
		if(sadd_attach($1, $3, strlen($3)) == -1)
			return -1;
	}

%%

int
yyerror(char *s) {
	fprintf(stderr, "Config parse error near line %d: %s\n",
		_sf_cfg_i_nline + 1, s);
	return 0;
}
