/*-
 * Copyright (c) 1999, 2000, 2001, 2002, 2003 Lev Walkin <vlm@spelio.net.ru>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id: sed.c,v 1.24 2003/01/13 06:42:28 vlm Exp $
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>

#ifdef	HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef	HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "strfunc.h"

#ifndef	HAVE_REGCOMP
#warning	"Compiling without regex support, sed*() functions will not work."
#else
#include "sed-int.h"

sed_t *
sed_compile(const char *expr) {
	char *c;
	char dlm;
	int type_of_operation = 0;
	int rexopt = 0;
	int opt = 0;
	char *strt, *pstrt;
	char *endt, *pendt;
	char *end, *pend;
	svect *sl;
	sed_i *st;

	if(expr == NULL) {
		errno = EINVAL;
		return NULL;
	}

	/* Searching for the Type Of Operation */
	switch(*expr) {
		case 's':
			/* s/// */
			type_of_operation = 1;
			expr++;
			break;
		case 'y':
			/* y/// */
			type_of_operation = 2;
			expr++;
			break;
		default:
			/* // */
			type_of_operation = 3;
			break;
	}

	dlm = *expr++;
	if(dlm == '\0' || *expr == '\0') {
		errno = EINVAL;
		return NULL;
	}

	/* Start */
	pstrt=strt=sf_malloc(strlen(expr) + 1);
	if(strt == NULL) {
		/* ENOMEM? */
		return NULL;
	}
	*pstrt = 0;

	for((const char *)c = expr; *c; c++) {
		if(*c == '\\') {
			if(*++c == dlm) {
				*pstrt++ = dlm;
			} else {
				*pstrt++ = '\\';
				*pstrt++ = *c;
			}
			continue;
		}
		if(*c == dlm)
			break;
		*pstrt++ = *c;
	}
	*pstrt = 0;

	if(*c == '\0') {
		free(strt);
		errno = EINVAL;
		return NULL;
	}

	/* Wrap for exceptions */
	{
		size_t clen;

		clen = strlen(c) + 1;

		if( (pendt=endt=sf_malloc(clen)) == NULL
			|| (pend=end=sf_malloc(clen)) == NULL
		) {
			if(endt)
				free(endt);
			free(strt);
			/* ENOMEM? */
			return NULL;
		}

		*pendt = '\0';
		*pend = '\0';
	}

	sl = sinit();
	if(sl == NULL) {
		free(endt);
		free(end);
		free(strt);
		/* ENOMEM? */
		return NULL;
	}

	if(type_of_operation == 3 && *c == dlm && !strchr(c+1, dlm))
		goto skipsecond;

	for(c++; *c; c++) {
		if(*c == '\\') {
			c++;
			if((*c >= '0' && *c <= '9') || (*c == '@')) {
				*pend = 0;

				if(sadd_attach(sl, end, strlen(end)) == -1) {
					if(end)
						free(end);
					free(endt);
					free(strt);
					sfree(sl);
					/* ENOMEM? */
					return NULL;
				}

				/* Add exactly one character */
				if(sadd2(sl, (void *)c, 1) == -1) {
					free(endt);
					free(strt);
					sfree(sl);
					/* ENOMEM? */
					return NULL;
				}

				pend = end = (char *)sf_malloc(strlen(c) + 1);
				if(pend == NULL) {
					free(endt);
					free(strt);
					sfree(sl);
					/* ENOMEM? */
					return NULL;
				}

				*pend = '\0';
				*pendt++ = *c;
				continue;
			}

			/* Un-escape other symbols */
			switch (*c) {
				case '\\':
					*pend++ = '\\';
					*pendt++ = '\\';
					break;
				case 'n':
					*pend++ = '\n';
					*pendt++ = '\n';
					break;
				case 't':
					*pend++ = '\t';
					*pendt++ = '\t';
					break;
			}
			*pend++ = *c;
			*pendt++ = *c;
			continue;
		}

		if(*c == dlm)
			break;

		if(*c == '&') {
			*pend = 0;

			if(sadd_attach(sl, end, strlen(end)) == -1) {
				if(end)
					free(end);
				free(endt);
				free(strt);
				sfree(sl);
				/* ENOMEM? */
				return NULL;
			}

			/* Add exactly one character */
			if(sadd2(sl, (void *)"0", 1) == -1) {
				free(endt);
				free(strt);
				sfree(sl);
				/* ENOMEM? */
				return NULL;
			}

			pend = end = (char *)sf_malloc(strlen(c) + 1);
			if(pend == NULL) {
				free(endt);
				free(strt);
				sfree(sl);
				/* ENOMEM? */
				return NULL;
			}

			*pend = '\0';
			*pendt++ = *c;
			continue;
		}

		*pend++ = *c;
		*pendt++ = *c;
	}

skipsecond:

	*pend = 0;
	*pendt = 0;

	sadd_attach(sl, end, strlen(end));

	if(*c == '\0') {
		free(strt);
		free(endt);
		sfree(sl);
		errno = EINVAL;
		return NULL;
	}

	if(type_of_operation == 3) {
		rexopt |= REG_NOSUB;
	}

	if(sl->count > 1) {
		rexopt &= ~REG_NOSUB;
		rexopt |= REG_EXTENDED;
	}

	for(c++; *c; c++)
	switch(*c) {
		case 'e':
			rexopt |= REG_EXTENDED;
			break;
		case 'g':
			opt |= SF_OPT_GLOBAL;	/* Global matching */
			break;
		case 'i':
			rexopt |= REG_ICASE;	/* Ignore case distinctions */
			break;
		case 'm':
			rexopt |= REG_NEWLINE;	/* Compile for newline-sensitive matching */
			break;
		case 'n':
			opt |= SF_OPT_NOZERO;	/* Ignore 0's match */
			break;
		case 'r':
			rexopt &= ~REG_NOSUB;
			opt |= SF_OPT_RESULTS;	/* Remember results */
			break;
		case 'b':
			rexopt &= ~REG_NOSUB;
			opt |= SF_OPT_BDRRES;	/* Remember border results */
			break;
		default:
			free(strt);
			free(endt);
			sfree(sl);
			errno = EINVAL;
			return NULL;
	}

	st = sf_calloc(1, sizeof(*st));
	if(st == NULL) {
		free(strt);
		free(endt);
		sfree(sl);
		/* ENOMEM? */
		return NULL;
	}
	st->to = sl;
	st->type_of_operation = type_of_operation;
	st->opt = opt;

	st->re = NULL;
	st->sfr = strt;
	st->sfr_len = strlen(strt);
	st->sto = endt;
	st->sto_len = strlen(endt);
	st->rexopt = rexopt;

	st->sb = sbuf_init();
	if(st->sb == NULL) {
		sed_free(st);
		/* ENOMEM? */
		return NULL;
	}

	if(type_of_operation == 1 || type_of_operation == 3) {
		st->re = (regex_t *)sf_malloc(sizeof(regex_t));
		if(st->re == NULL) {
			sed_free(st);
			/* ENOMEM? */
			return NULL;
		}

		if(regcomp(st->re, strt, rexopt)) {
			free(st->re);
			st->re = NULL;	/* Don't allow regfree() on this data */
			sed_free(st);
			/* errno set */
			return NULL;
		}

		/* re_nsub + one for the whole string */
		st->nmatch = st->re->re_nsub + 1;

		st->pmatch = (regmatch_t *)sf_malloc(st->nmatch
			* sizeof(regmatch_t));
		if(st->pmatch == NULL) {
			sed_free(st);
			/* ENOMEM? */
			return NULL;
		}
	} else if(type_of_operation == 2) {
		if(rexopt & REG_ICASE) {
			for(c = st->sfr; *c; c++)
				*c = tolower(*c);
		}
	}

	return st;
}



/*
** sed_exec(): Executes compiled expression against the specified string.
*/

char *
sed_exec(sed_t *sp, const char *string) {
	register sed_i *se=(sed_i *)sp;
	int slen = 0;

	if(string == NULL || se == NULL) {
		errno = EINVAL;
		return NULL;
	}

	slen = strlen(string);
	sbuf_zero(se->sb);

	if((se->type_of_operation == 1) || (se->type_of_operation == 3)) {
		size_t nm = se->nmatch;
		regmatch_t *pm = se->pmatch;
		svect *results = se->Results;
		svect *bdrres = se->BorderResults;
		int notstart = 0;
		int i;
		char *c;
		int foundsmth = 0;	/* Is anything matched (for ///) */
		int shifted = 0;

		pm[0].rm_so = 0;
		pm[0].rm_eo = slen;

		/* We'll save results (()'s) here. */
		if(results) {
			sclear(results);
		} else {
			if((se->opt & SF_OPT_RESULTS)) {
				results = se->Results = sinit();
				if(results == NULL)
					/* ENOMEM? */
					return NULL;
			}
		}

		if(bdrres) {
			sclear(bdrres);
		} else {
			if((se->opt & SF_OPT_BDRRES)) {
				bdrres = se->BorderResults = sinit();
				if(bdrres == NULL)
					/* ENOMEM? */
					return NULL;
			}
		}

here1:

		/* Try to evaluate expression against the given string */
		if(regexec(se->re, string, nm, pm, notstart)) {
			if(sbuf_add(se->sb, string) == -1)
				/* ENOMEM? */
				return NULL;
			if(bdrres)
				if(sadd(bdrres, string) == -1)
					/* ENOMEM? */
					return NULL;
			if((se->type_of_operation == 3) && foundsmth == 0) {
				errno = ESRCH;
				return NULL;
			}
			return se->sb->buf;
		} else {
			foundsmth++;
			shifted--;
		}

		/*
		 * Fillout results array, result buffer and
		 * border buffer.
		 */
		if(results) {
			if(nm == 1) {	/* If no braces detected in expression */
				if(pm[0].rm_so > -1)
					if(sadd2(results,
					  (void *)(string + pm[0].rm_so),
					    pm[0].rm_eo - pm[0].rm_so) == -1)
						/* ENOMEM? */
						return NULL;
			} else  {
				if((se->opt & SF_OPT_NOZERO))
					i = 1;
				else
					i = 0;
				for(; i < nm && pm[i].rm_so > -1; i++) {
					if(sadd2(results,
					  (void *)(string + pm[i].rm_so),
					    pm[i].rm_eo - pm[i].rm_so ) == -1)
						/* ENOMEM? */
						return NULL;
				}
			}
		}

		/* Add characters before matched substring */
		if(pm[0].rm_so != -1) {
			if( sbuf_add2(se->sb, (void *)string, pm[0].rm_so) == -1)
				/* ENOMEM? */
				return NULL;
			if(bdrres) {
				if(shifted == 1) {
					if(sadd2(bdrres, (void *)(string - 1),
					    pm[0].rm_so + 1) == -1)
						/* ENOMEM? */
						return NULL;
				} else if(pm[0].rm_so) {
					if(sadd2(bdrres, (void *)string,
					    pm[0].rm_so) == -1)
						/* ENOMEM? */
						return NULL;
				}
			}
		}

		/* Add to the TO buffer */
		if( sbuf_add(se->sb, se->to->list[0]) == -1)
			/* ENOMEM? */
			return NULL;

		for(i = 1; i < se->to->count; i+=2) {
			int t;
			c = se->to->list[i];
			if((*c == '@') && se->ResHandler) {
				if(sbuf_add(se->sb,
						se->ResHandler(results)) == -1)
					/* ENOMEM? */
					return NULL;
				if(sbuf_add(se->sb, se->to->list[i+1]) == -1)
					/* ENOMEM? */
					return NULL;
				continue;
			}
			t = atoi(c);
			if(
				(t < 0)
				|| (t >= nm)
				|| (pm[t].rm_eo == -1)
			) continue;

			if(sbuf_add2(se->sb,
				(void *)(string + pm[t].rm_so),
				pm[t].rm_eo - pm[t].rm_so) == -1)
				/* ENOMEM? */
				return NULL;
			if(sbuf_add(se->sb, se->to->list[i+1]) == -1)
				/* ENOMEM? */
				return NULL;
		}

		if(se->opt & SF_OPT_GLOBAL) {
			notstart |= REG_NOTBOL;
			if(pm[0].rm_eo == 0) {
				if(*string) {
					string++;
					shifted = 2;
					goto here1;
				} else {
					return se->sb->buf;
				}
			}

			string += pm[0].rm_eo;
			goto here1;
		}

		/* Add the rest of the string */
		if(sbuf_add(se->sb, string + pm[0].rm_eo) == -1)
			/* ENOMEM? */
			return NULL;

		if(bdrres)
			if(sadd(bdrres, string + pm[0].rm_eo) == -1)
				/* ENOMEM? */
				return NULL;

		return se->sb->buf;

	} else if(se->type_of_operation == 2) {

		char *s, *c, *b;
		char *sfr;
		char cs;
		int i;

		sbuf_extend(se->sb, slen + 1);
		b = se->sb->buf;
		*b = 0;

		sfr = se->sfr;

		if(se->rexopt & REG_ICASE) {

			for((const char *)s = string; (cs=*s); s++) {
				cs = tolower(cs);
				for(c = sfr; *c; c++)
					if(cs == *c)
						break;
				if(*c == '\0') {
					*b++ = *s;
					continue;
				}
				i = c - sfr;
				if(i > se->sto_len)
					break;
				*b++ = se->sto[i];
			}

		} else {

			for((const char *)s = string; (cs=*s); s++) {
				for(c = sfr; *c; c++)
					if(cs == *c)
						break;
				if(*c == '\0') {
					*b++ = *s;
					continue;
				}
				i = c - sfr;
				if(i > se->sto_len)
					break;
				*b++ = se->sto[i];
			}

		}	/* if(se->rexopt & REG_ICASE) */

		*b = 0;

		return se->sb->buf;

	}

	errno = ESRCH;
	return NULL;
}


/*
** Destroy sed_t matching structure.
*/

void
sed_free(sed_t *sp) {
	register sed_i *st=(sed_i *)sp;

	if(st == NULL)
		return;

	if(st->re) {
		regfree(st->re);
		free(st->re);
	}

	if(st->pmatch) {
		free(st->pmatch);
	}

	if(st->to) {
		sfree(st->to);
	}

	if(st->sb) {
		sbuf_free(st->sb);
	}

	if(st->sfr) {
		free(st->sfr);
	}

	if(st->sto) {
		free(st->sto);
	}

	if(st->Results) {
		sfree(st->Results);
	}

	if(st->BorderResults) {
		sfree(st->BorderResults);
	}

	free(st);
}

int
sed_handler(sed_t *sp, char *(*ResHandler)(svect *)) {
	register sed_i *se=(sed_i *)sp;
	if(se == NULL)
		return -1;

	se->opt |= SF_OPT_RESULTS;
	se->ResHandler = ResHandler;

	return 0;
}

svect *
sed_results(sed_t *sp) {
	register sed_i *se=(sed_i *)sp;

	if(se == NULL) {
		errno = EINVAL;
		return NULL;
	}

	return se->Results;

}


svect *
sed_border_results(sed_t *sp) {
	register sed_i *se=(sed_i *)sp;

	if(se == NULL) {
		errno = EINVAL;
		return NULL;
	}

	return se->BorderResults;
}

#endif
