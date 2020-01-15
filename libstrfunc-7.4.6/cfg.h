#ifndef BISON_Y_TAB_H
# define BISON_Y_TAB_H

#ifndef YYSTYPE
typedef union {
	char *tv_char;
	svect *tv_svect;
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
# define	TOK_STRING	257
# define	EQ	258
# define	LB	259
# define	RB	260
# define	COMMA	261
# define	COLON	262
# define	SEMICOLON	263


extern YYSTYPE sfcfglval;

#endif /* not BISON_Y_TAB_H */
