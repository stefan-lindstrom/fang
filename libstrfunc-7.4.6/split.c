/*-
 * Copyright (c) 1999, 2000, 2001, 2002 Lev Walkin <vlm@spelio.net.ru>.
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
 * $Id: split.c,v 1.29 2002/12/19 03:54:34 vlm Exp $
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>

#ifdef	HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef	HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#ifdef	HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#include "strfunc.h"

/****************/
/* SPLIT STRING */
/****************/

/* split string and add its contents to the svect structure */
/* Split: full version */
int
splitf(svect *s, const char *msg, const char *dlm, int flags) {
	char *p;
	char *w = NULL;
	int tokens = 0;
	int dlen;
	char cp, cd;

	p = (char *)msg;

	if(p == NULL || s == NULL) {
		errno = EINVAL;
		return -1;
	}

	if(dlm == NULL) {
		if(flags & 4)
			flags &= ~4;

		if(flags)
			/* Default to parse /etc/password-like strings */
			dlm = ":";
		else
			/* Default to split on common whitespaces */
			dlm = " \t\n\r";
	}
	cd = *dlm;
	dlen = strlen(dlm);

	/* Regex value */
	if(flags & 4) {
#ifndef	HAVE_REGCOMP
		errno = ENOTSUP;
		return -1;
#else
		char *p;
		sed_t *st;
		svect *sl;

		p = (char *)malloc(dlen + 10);
		if(p == NULL)
			/* ENOMEM? */
			return -1;
		if(*dlm != '/') {
			strcpy(p, "/");
			strcat(p, dlm);
			strcat(p, "/bge");
		} else {
			strcpy(p, dlm);
			strcat(p, "bge");
		}
		dlm = p;

		st = sed_compile(dlm);
		if(st == NULL) {
			/* ENOMEM? */
			return -1;
		}

		if(sed_exec(st, msg) == NULL) {
			sed_free(st);
			if(sadd(s, msg) == -1) {
				/* ENOMEM? */
				return 1;
			}
			return 1;
		}

		sl = sed_border_results(st);
		if(sl && sl->count) {
			int added = 0;
			for(; tokens < sl->count; tokens++) {
				char *tok = sl->list[tokens];
				sl->list[tokens] == NULL;
				if(sadd_attach(s, tok,
					  sl->lens[tokens]) == -1) {
					while(added--)	/* cleanup */
						sdel(s, s->count - 1);
					sed_free(st);
					/* ENOMEM? */
					return -1;
				}
				added++;
			}
			sl->count = 0;
			sl->list[0] = NULL;
		} else {
			if(sadd(s, msg) == -1) {
				sed_free(st);
				/* ENOMEM? */
				return -1;
			}
		}

		sed_free(st);
		return tokens;
#endif	/* HAVE_REGCOMP */
	}

	if(flags & 2) {

		for(; (cp = *p); p++) {
			if((cd == cp) && (strncmp(p, dlm, dlen) == 0)) {
				if(w) {
					if(sadd2(s, w, p-w) == -1) {
						while(tokens--)	/* cleanup */
							sdel(s, s->count - 1);
						/* ENOMEM? */
						return -1;
					}
					w = NULL;
					tokens++;
				} else if(flags & 1) {
					if(sadd2(s, "", 0) == -1) {
						while(tokens--)	/* cleanup */
							sdel(s, s->count - 1);
						/* ENOMEM? */
						return -1;
					}
					tokens++;
				}
				p += dlen - 1;
			} else {
				if(w == NULL)
					w = p;
			}
		}

	} else {

		for(; (cp = *p); p++) {
			if((cd == cp) || memchr(dlm, cp, dlen)) {
				if(w) {
					if(sadd2(s, w, p-w) == -1) {
						while(tokens--)
							sdel(s, s->count - 1);
						/* ENOMEM? */
						return -1;
					}
					w = NULL;
					tokens++;
				} else if(flags & 1) {
					if(sadd2(s, "", 0) == -1) {
						while(tokens--)
							sdel(s, s->count - 1);
						/* ENOMEM? */
						return -1;
					}
					tokens++;
				}
			} else {
				if(w == NULL)
					w = p;
			}
		}

	}

	if(w) {
		if(sadd2(s, w, p-w) == -1) {
			while(tokens--)
				sdel(s, s->count - 1);
			/* ENOMEM? */
			return -1;
		}
		tokens++;
	}

	return tokens;
}

svect *
split(const char *msg, const char *delim, int flags) {
	svect *sl;

	sl = sinit();
	if(sl == NULL)
		/* ENOMEM? */
		return NULL;

	if(splitf(sl, msg, delim, flags) == -1) {
		sfree(sl);
		return NULL;
	}

	return sl;
}


#define CHECKRANGE do {	\
	  if(w - buf >= buflen) {	\
		int z = w - buf; \
		buf = (char *)sf_realloc(buf, buflen = buflen << 1);	\
		if(buf == NULL) {	\
			while(tokens--)	/* cleanup */	\
				sdel(s, tokens);	\
			return -1;	\
		} \
		w = buf + z;	\
	  }	\
	} while(0)

/* split string and add its contents to the svect structure */
int
splitquotable(svect *s, const char *msg) {
	char *buf=NULL;
	size_t buflen = 0;
	char *p=(char *)msg;
	char *w;
	int tokens = 0;
	register char cp;

	int insng = 0;	/* Inside the single quotes */
	int indbl = 0;	/* Inside the double quotes */

	if(s == NULL || p == NULL) {
		errno = EINVAL;
		return -1;
	}

	buflen=16;
	buf=(char *)sf_malloc(buflen);
	if(buf == NULL) {
		/* ENOMEM? */
		return -1;
	}

	w=buf;

	while(1) {
		switch((cp=*p)) {
			case '\0':
				if(w-buf) {
					if(sadd2(s, buf, w-buf + 1) == -1) {
						while(tokens--)
							sdel(s, s->count - 1);
						if(buf)
							free(buf);
						return -1;
					}
					s->list[s->count-1][w-buf] = 0;
					if(insng) s->list[s->count-1][w-buf + 1] = 1;
					else if(indbl) s->list[s->count-1][w-buf + 1] = 2;
					s->lens[s->count-1] = w-buf;

					tokens++;
					w=buf;
				}
				if(buf)
					free(buf);
				return tokens;
			case ' ':
			case '\t':
			case '\n':
				if(indbl || insng) {
					*w++=cp;
				} else if(w-buf) {
					if(sadd2(s, buf, w-buf + 1) == -1) {
						while(tokens--)
							sdel(s, s->count - 1);
						if(buf)
							free(buf);
						return -1;
					}
					s->list[s->count-1][w-buf] = 0;
					if(insng) s->list[s->count-1][w-buf + 1] = 1;
					else if(indbl) s->list[s->count-1][w-buf + 1] = 2;
					s->lens[s->count-1] = w-buf;
					tokens++;
					w=buf;
				}
				break;
			case '\'':
				if(indbl) {
					*w++=cp;
				} else {
					if(insng) {
						if(sadd2(s, buf, w-buf + 1) == -1) {
							while(tokens--)
								sdel(s, s->count - 1);
							if(buf)
								free(buf);
							return -1;
						}
						s->list[s->count-1][w-buf] = 0;
						if(insng) s->list[s->count-1][w-buf + 1] = 1;
						else if(indbl) s->list[s->count-1][w-buf + 1] = 2;
						s->lens[s->count-1] = w-buf;
						tokens++;
						w=buf;
						insng = 0;
					} else {
						insng = 1;
					}
				}
				break;
			case '"':
				if(insng) {
					*w++=cp;
				} else {
					if(indbl) {
						if(sadd2(s, buf, w-buf + 1) == -1) {
							while(tokens--)
								sdel(s, s->count - 1);
							if(buf)
								free(buf);
							return -1;
						}
						s->list[s->count-1][w-buf] = 0;
						if(insng) s->list[s->count-1][w-buf + 1] = 1;
						else if(indbl) s->list[s->count-1][w-buf + 1] = 2;
						s->lens[s->count-1] = w-buf;
						tokens++;
						w=buf;
						indbl = 0;
					} else {
						indbl = 1;
					}
				}
				break;
			case '\\':
				cp = *++p;
				if(insng) {
					if(cp == '\'') {
						*w++='\'';
						break;
					}
					*w++='\\';
					CHECKRANGE;
					*w++=cp;
					break;
				}
				if(indbl) {
					switch(cp) {
						case '"': *w++='\''; break;
						case '\'': *w++='\''; break;
						case '\n': break;
						default:
							*w++='\\';
							CHECKRANGE;
							*w++=cp;
					}
					break;
				}
				switch(cp) {
					case '\n': break;
					case 'a': *w++='\a'; break;
					case 'b': if(w-buf) w--; break;
					case 'e': *w++='\e'; break;
					case 'f': *w++='\f'; break;
					case 'r': *w++='\r'; break;
					case 'n': *w++='\n'; break;
					case 't': *w++='\t'; break;
					case 'v': *w++='\v'; break;
					case '\\': *w++='\\'; break;
					default: *w++=cp; break;
				}
				break;
			default:
				*w++=cp;
		} /* switch (*) */
		CHECKRANGE;
		p++;
	}

	if(buf)
		free(buf);
	return tokens;
}


int
split_network(const char *ipmp, unsigned int *ip, unsigned int *mask) {
	char buf[32];
	unsigned long t;
	char *msk = NULL;
	char *ipm;

	if(ip == NULL || mask == NULL) {
		errno = EINVAL;
		return -1;
	}

	if(ipmp == NULL) {
		*ip = 0;
		*mask = 0;
		errno = EINVAL;
		return -1;
	}

	t = strlen(ipmp) + 1;
	if(t >= 32) {	/* Too long address/mask */
		errno = EINVAL;
		return -1;
	}

	memcpy(buf, ipmp, t);
	ipm = buf;

	if((msk=strchr(ipm, '/')) || (msk = strchr(ipm, ' ')))
		*msk++ = 0;

	if(sf_iaton(ipm, ip) == 0) {
		*ip = *mask = 0;
		errno = EINVAL;
		return -1;
	}

	if(msk == NULL) {
		*mask = 0xffffffff;
		return 0;
	}

	t = countchar(msk, '.');
	if(t && t > 3) {
		*ip = *mask = 0;
		errno = EINVAL;
		return -1;
	}
	if(t && !sf_iaton(msk, mask)) {
		*ip = *mask = 0;
		errno = EINVAL;
		return -1;
	}
	if(t == 0) {
		errno = 0;
		if(msk[0] == '0' && msk[1] == 'x') {
			t = strtoul(msk, NULL, 16);
#ifdef  WORDS_BIGENDIAN
			*mask = t;
#else   /* WORDS_BIGENDIAN */
#ifdef	HAVE_HTONL
			*mask = htonl(t);
#else	/* HAVE_HTONL */
			*mask =
				((t & 0xff) << 24)
				| ((t & 0xff00) << 8)
				| ((t & 0xff0000) >> 8)
				| ((t & 0xff000000) >> 24);
#endif	/* HAVE_HTONL */
#endif  /* WORDS_BIGENDIAN */
		} else {
			t = strtoul(msk, NULL, 10);
			if((t > 32) || errno) {
				if(errno == 0)
					errno = EINVAL;
				return -1;
			}
			if(t) t = 0xffffffff << (32 - t);
#ifdef  WORDS_BIGENDIAN
			*mask = t;
#else   /* WORDS_BIGENDIAN */
#ifdef	HAVE_HTONL
			*mask = htonl(t);
#else	/* HAVE_HTONL */
			*mask =
				((t & 0xff) << 24)
				| ((t & 0xff00) << 8)
				| ((t & 0xff0000) >> 8)
				| ((t & 0xff000000) >> 24);
#endif	/* HAVE_HTONL */
#endif  /* WORDS_BIGENDIAN */
		}
	}

	*ip = *ip & *mask;

	return 0;
}

