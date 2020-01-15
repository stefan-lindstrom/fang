/*-
 * Copyright (c) 2000, 2001 Lev Walkin <vlm@spelio.net.ru>.
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
 * $Id: format.c,v 1.17 2002/11/30 10:24:03 vlm Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>

#ifdef	HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef	HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "strfunc.h"

#define	CHECK	do { \
		size_t n = 0;	\
		while((p-buf) + len >= buflen)	\
			buflen = buflen << (n=1); \
		if(n) { \
			n = (size_t)(p-buf); \
			p = (char *)sf_realloc(buf, buflen); \
			if(p == NULL) {	\
				free(fmt);	\
				free(buf);	\
				return NULL;	\
			}	\
			buf = p; \
			p += n; \
		}	\
	} while(0)

#define ABORT do { free(fmt); free(buf); return NULL; } while(0)

typedef struct {
	char *buf;
	size_t buflen;
	char *metas;
	char *endmetas;
	char **(**funcs)(char *, void *);
} fmt_base_internal;

static char *sf_nel[]={ "X", NULL };

char *
formatf(fmt_base *fb, char *fmt, void *key) {
	char *f;
	char *buf = NULL, *p;
	size_t buflen = 0;
	char *mp;	/* MetaPointer */
	char *emp;	/* EndMetaPointer */
	char *eb;	/* Ending bracet */
	char *ex;	/* Extracted data */
	char *q, *c;	/* Question and colon in "<a>?<b>:<c>" case */
	int ab, qb, cb;	/* Is there any '$''s present in <a> or <b> or <c>? */
	int es; /* Is there any quoted characters? */
	char **rval;	/* Function result value */
	char **(*func)(char *, void *);
	int len;
	fmt_base_internal *fbd = (fmt_base_internal *)fb;
	int num;
	char *cw;	/* Concatenate with */
	char *eqw;	/* Test equality with */
	int eoq;	/* Equality or not equality test */
	char n;

	if(fbd == NULL) {
		errno = EINVAL;
		return NULL;
	}

	if(fmt == NULL) {
		if(fbd->buflen == 0) {
			char *p = (char *)sf_realloc(fbd->buf, 16);
			if(p == NULL)
				/* ENOMEM? */
				return NULL;
			fbd->buf = p;
			fbd->buflen = 16;
		}
		*(fbd->buf) = (char)0;
		return fbd->buf;
	}

	f=fmt=sf_strdup(fmt);
	if(fmt == NULL) {
		/* ENOMEM? */
		return NULL;
	}

	if(!buflen) {
		buf = (char *)sf_malloc(buflen = 128);
		if(buf == NULL) {
			free(fmt);
			/* ENOMEM? */
			return NULL;
		}
	}

	*(p=buf) = '\0';

while((n=*f)) {

	len = 0;

	/* Found quoting */
	if(n == '\\') {
		CHECK;
		switch((n = *++f)) {
			case 'n': *p++='\n'; break;
			case 't': *p++='\t'; break;
			case 's': *p++=' '; break;
			default: *p++=n;
		}
		f++;
		continue;
	}

	/* Metacharacter isn't found */
	if(n != '$' || fbd->metas == NULL) {
		CHECK;
		*p++=n;
		f++;
		continue;
	}

	n = *++f;

	for(mp = fbd->metas, emp = fbd->endmetas;
			*mp; mp++, emp++) {
		if(*mp == n)
			break;
	}

	if(*mp == '\0') {
		len = 2;
		CHECK;
		*p++ = '$';
		*p++ = n;
		f++;
		continue;
	}

	func = fbd->funcs[mp - fbd->metas];

	if((eb = brace(f++, *emp)) == NULL)
		break;

	*eb = '\0';
	ex = f;

	/*
	 *      ex cw eb
	 *      |   | |
	 * abc${def+, }ghi
	 */

	num = -1;

	/* Is it an "a?b:c" case? */
	ab=qb=cb=es = 0;
	for(f=ex, q=c=NULL; *f; f++) {
		switch(*f) {
			case '\\':
				if(!q && !c) es=1;
				f++;
				break;
			case '?':
				if(q) q=c=NULL;
				else q=f;
				break;
			case ':':
				if(c) q=c=NULL;
				else c=f;
				break;
			case '[':
				if(num == -1) {
					num = atoi(f+1);
					if(num != 0 || *(f+1) == '0')
						*f = '\0';
					else
						num=-1;
				}
				break;
			case '$': {
				char *t;

				if(!q && !c) ab=f-ex+1;
				else if(c) cb=f-ex+1;
				else if(q) qb=f-ex+1;

				f++;

				/* Is there any non-escaped metacharacters? */
				if((t = strchr(fbd->metas, *f)) == NULL)
					break;
				t=fbd->endmetas + (t - fbd->metas);
				f=brace(f, *t);

			  }
			  break;
		}
	}


	if(q == NULL || (c && q > c)) {
		cw=ex; while(*cw && *cw != '+') cw++;
		if(*cw == '+' && cw[-1] != '\\') *cw++ = '\0';
		else cw=NULL;

		rval = func(((ab&qb&cb)|es)
			? formatf(fb, ex, key)
			: ex, key);
		if(rval) {
			if(num < 0) {
				f = join(rval, cw ? cw : ", ");
				if(f == NULL)
					ABORT;
				len = strlen(f);
				CHECK;
				strcpy(p, f);
			} else {
				char **vp = rval;
				while(*vp && num) {
					vp++;
					num--;
				}
				if(num == 0 && *vp) {
					len = strlen(*vp);
					CHECK;
					strcpy(p, *vp);
				}
			}
		}
	} else {
		*q++='\0';
		if(c) *c++='\0';

		eoq = 0;
		eqw = ex-1;
		while(*++eqw) {
			if(*eqw == '!' && eqw[1] == '=') {
				*eqw = '\0';
				eqw+=2;
				eoq=-1;
				break;
			}
			if(*eqw == '=' && eqw[1] == '=') {
				*eqw = '\0';
				eqw+=2;
				eoq=1;
				break;
			}
		}

		if(eoq) {
			if(ab && ex+ab < eqw)
				rval = func(formatf(fb, ex, key), key);
			else if(ab) {
				eqw = formatf(fb, eqw, key);
				rval = func(es?formatf(fb, ex, key):ex, key);
			} else {
				rval = func(es?formatf(fb, ex, key):ex, key);
			}
			if(eoq == 1) {
				/*
				 * Equality test
				 */
				if(rval) {
					if(cfind(rval, eqw) == -1)
						rval = NULL;
				} else {
					if(*eqw == '\0')
						rval = sf_nel;
				}
			} else {
				/*
				 * Inequality test
				 */
				if(rval) {
					if(cfind(rval, eqw) != -1)
						rval = NULL;
				} else {
					if(*ecq)
						rval = sf_nel;
				}
			}
		} else {
			rval = func((ab|es)?formatf(fb, ex, key):ex, key);
		}

		if(rval) {
			char **vp = rval;
			if(num < 0)
				num = 0;
			while(*vp && num) {
				vp++;
				num--;
			}
			if(num == 0 && *vp) {
				len = strlen(f=qb?formatf(fb, q, key):q);
				CHECK;
				strcpy(p, f);
				c = NULL;
			}
		}
		if(c) {
			len = strlen(f=cb?formatf(fb, c, key):c);
			CHECK;
			strcpy(p, f);
		}
	}

	p += len;

	f = eb + 1;
	CHECK;

  } /* while() */

	*p = '\0';

	free(fmt);

	fbd->buflen = p-buf;
	/*
	 * Replace previous output buffer.
	 */
	if(fbd->buf)
		free(fbd->buf);
	return (fbd->buf = buf);
}

fmt_base *
format_init() {

	return (fmt_base *)sf_calloc(1, sizeof(fmt_base_internal));

}

void
format_free(fmt_base *fb) {
	fmt_base_internal *fbd = (fmt_base_internal *)fb;

	if(fbd == NULL)
		return;

	if(fbd->buf) {
		free(fbd->buf);
	}
	if(fbd->metas) {
		free(fbd->metas);
		free(fbd->endmetas);
		free(fbd->funcs);
	}

	free(fbd);
}

int
format_metarule(fmt_base *fb, char sw, char ew, char **(*func)(char *, void *)) {
	fmt_base_internal *fbd=(fmt_base_internal *)fb;
	int len = 0;
	void *p;

	if(fbd == NULL || sw == '\0' || ew == '\0') {
		errno = EINVAL;
		return -1;
	}
	
	if(fbd->metas)
		len = strlen(fbd->metas);

	len++;

	p = sf_realloc(fbd->metas, sizeof(char) * (len+1));
	if(p == NULL)
		/* ENOMEM? */
		return -1;
	fbd->metas=(char *)p;

	p = sf_realloc(fbd->endmetas, sizeof(char) * len);
	if(p == NULL)
		/* ENOMEM? */
		return -1;
	fbd->endmetas=(char *)p;

	p = (char **(**)(char *, void *))
		sf_realloc(fbd->funcs,
			sizeof(char **(*)(char *, void *)) * len);
	if(p == NULL)
		/* ENOMEM? */
		return -1;
	fbd->funcs=(char **(**)(char *, void *))p;

	len--;
	fbd->metas[len]=sw;
	fbd->metas[len+1]='\0';
	fbd->endmetas[len]=ew;
	fbd->funcs[len]=func;

	return 0;
}

char *
format_lastresult(fmt_base *fbp, size_t *optSize) {
	fmt_base_internal *fbi = (fmt_base_internal *)fbp;

	if(fbi == NULL) {
		if(optSize)
			*optSize = 0;
		return "";
	}

	if(fbi->buf) {
		*optSize = fbi->buflen;
		return fbi->buf;
	}

	*optSize = 0;

	return "";
}

size_t
format_lastsize(fmt_base *fbp) {
	fmt_base_internal *fbi = (fmt_base_internal *)fbp;

	if(fbi && fbi->buf)
		return fbi->buflen;

	return 0;
}

char *
format_detach(fmt_base *fbp, size_t *optSize) {
	fmt_base_internal *fbi = (fmt_base_internal *)fbp;
	char *s;

	if(fbi == NULL) {
		if(optSize)
			*optSize = 0;
		return sf_strdup("");
	}

	s = fbi->buf;

	if(s == NULL) {
		s = sf_strdup("");
		if(optSize)
			*optSize = 0;
	} else {
		free(fbi->buf);
		if(optSize)
			*optSize = fbi->buflen;
	}

	fbi->buf = NULL;
	fbi->buflen = 0;

	return s;
}

