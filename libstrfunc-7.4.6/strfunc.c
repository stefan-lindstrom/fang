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
 * $Id: strfunc.c,v 1.27 2002/11/30 10:24:03 vlm Exp $
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


/*
 * Equality test. Case-sensitive.
 */
int
eq(const char *a, const char *b) {

	/* Can't test equality of NOTHING */
	if(a == NULL || b == NULL)
		return 0;

	return strcmp(a, b)?0:1;
}

/*
 * Equality test. Case-insensitive.
 */
int
ecq(const char *a, const char *b) {
	register const unsigned char
		*us1 = (const unsigned char *)a,
		*us2 = (const unsigned char *)b;

	/* Can't test equality of NOTHING */
	if(us1 == NULL || us2 == NULL)
		return 0;

	while (tolower(*us1) == tolower(*us2++))
		if (*us1++ == '\0')
			return 1;

	return 0;
}

/*
 * Equality test. Case-insensitive. b should be already lower-cased.
 */
int
ecq2(const char *a, const char *b) {
	register const unsigned char
		*us1 = (const unsigned char *)a,
		*us2 = (const unsigned char *)b;

	/* Can't test equality of NOTHING */
	if(us1 == NULL || us2 == NULL)
		return 0;

	while (tolower(*us1) == *us2++)
		if (*us1++ == '\0')
			return (1);

	return 0;
}

/*
 * Create or initialize empty string list
 */
svect *
sinit() {
	svect *sl;

	sl=(svect *)sf_calloc(1, sizeof(svect));
	if(sl == NULL) {
		/* ENOMEM? */
		return NULL;
	}

	sl->listlen = 4;

	/* Place for data chunks */
	sl->list = (char **)sf_malloc(sizeof(char *) * sl->listlen);
	if(sl->list == NULL) {
		free(sl);
		/* ENOMEM? */
		return NULL;
	}

	/* Place for data lengths */
	sl->lens = (size_t *)sf_malloc(sizeof(size_t) * sl->listlen);
	if(sl->lens == NULL) {
		free(sl->list);
		free(sl);
		/* ENOMEM? */
		return NULL;
	}

	sl->list[0] = NULL;
	sl->lens[0] = 0;

	return sl;
}

/*
 * Make exact copy of given string list.
 */
svect *
scopy(svect *sl) {
	svect *sd;
	size_t n;

	if(sl == NULL) {
		errno = EINVAL;
		return NULL;
	}

	sd = sinit();
	if(sd == NULL) {
		/* ENOMEM? */
		return NULL;
	}

	if(sl->count == 0 || sl->list == 0)
		return sd;

	for(n = 0; n < sl->count; n++) {
		if(sadd2(sd, sl->list[n], sl->lens[n]) == -1) {
			sfree(sd);
			return NULL;
		}
	}

	return sd;
}

/*
 * Import **vp into string list.
 * Clears **vp!
 */
int
simport(svect *sl, char **vp) {
	char **p;
	int added = 0;

	if(sl == NULL || vp == NULL) {
		if(vp)
			free_values(vp);
		errno = EINVAL;
		return -1;
	}

	for(p = vp; *p; p++, added++) {
		if(sadd_attach(sl, *p, strlen(*p)) == -1) {
			while(added--)
				sdel(sl, sl->count - 1);
			for(; *p; p++);
				free(*p);
			free(vp);
			/* ENOMEM? */
			return -1;
		}
	}

	free(vp);
	return 0;
}

/*
 * Clear string list.
 */
void
sclear(svect *sl) {
	if(sl == NULL)
		return;

	if(sl->listlen > 0) {
		if(sl->list) {
			while(sl->count--)
				if(sl->list[sl->count])
					free(sl->list[sl->count]);
			*(sl->list) = NULL;
			*(sl->lens) = 0;
		} else {
			sl->listlen = 0;
			if(sl->lens) {
				free(sl->lens);
				sl->lens = 0;
			}
		}
	} else {
		sl->listlen = 0;
		if(sl->list) {
			free(sl->list);
			sl->list = 0;
		}
		if(sl->lens) {
			free(sl->lens);
			sl->lens = 0;
		}
	}

	sl->count = 0;
	sl->maxlen = 0;
}

/*
 * Destroy string list.
 */
void
sfree(svect *sl) {
	if(sl == NULL)
		return;

	if(sl->list) {
		if(sl->count > 0) {
			void *p;
			while(sl->count--) {
				p = sl->list[sl->count];
				if(p) free(p);
			}
		}
		free(sl->list);
	}

	if(sl->lens)
		free(sl->lens);

	free(sl);
}

/* Basic adding function */
int
_sf_add_internal(svect *s, void *msg, size_t len) {

	if(s->count + 1 >= s->listlen) {
		void *z;
		size_t newlistlen;

		if((newlistlen = s->listlen << 2) == 0)
			newlistlen = 4;

		z = sf_realloc(s->list, sizeof(char *) * newlistlen);
		if(z)
			s->list = (char **)z;
		else
			/* ENOMEM? */
			return -1;

		z = sf_realloc(s->lens, sizeof(size_t) * newlistlen);
		if(z)
			s->lens = (size_t *)z;
		else
			/* ENOMEM? */
			return -1;
		s->listlen = newlistlen;
	}

	s->list[s->count] = msg;
	s->lens[s->count] = len;

	if(len > s->maxlen)
		s->maxlen = len;

	s->count++;
	s->list[s->count] = NULL;
	s->lens[s->count] = 0;

	return 0;
}


/* Attach previously malloc'ed memory */
int
sadd_attach(svect *s, void *msg, size_t len) {
	char *p;

	if(s == NULL || msg == NULL) {
		if(msg)
			free(msg);
		errno = EINVAL;
		return -1;
	}

	/* Does not relocate msg in most cases */
	p = (char *)sf_realloc(msg, len + 1);
	if(p == NULL) {
		free(msg);
		/* ENOMEM? */
		return -1;
	}

	p[len] = 0;

	if(_sf_add_internal(s, p, len) == -1) {
		free(p);
		/* ENOMEM? */
		return -1;
	}

	return 0;
}

/* Add a string to the end of the svect structure */
int
sadd2(svect *s, void *msg, size_t len) {
	void *tmp;

	if(s == NULL || msg == NULL) {
		errno = EINVAL;
		return -1;
	}

	tmp = sf_malloc(len+1);
	if(tmp == NULL)
		/* ENOMEM? */
		return -1;

	memcpy(tmp, msg, len);
	((char *)tmp)[len] = 0;

	if(_sf_add_internal(s, tmp, len) == -1) {
		free(tmp);
		return -1;
	}

	return 0;
}

int
sadd(svect *s, const char *msg) {
	if(s == NULL || msg == NULL) {
		errno = EINVAL;
		return -1;
	}

	return sadd2(s, (void *)msg, strlen(msg));
}


/* delete i'th string from the list */
int
sdel(svect *s, size_t n) {
	if(s == NULL) {
		errno = EINVAL;
		return -1;
	}

	if(n >= s->count)
		return s->count;

	free(s->list[n]);

	for(s->count--; n <= s->count; n++) {
		s->list[n] = s->list[n+1];
		s->lens[n] = s->lens[n+1];
	}

	return s->count;
}

/* Insert element */
ssize_t
sins(svect *s, const char *el, size_t n) {
	int i;

	if(s == NULL || el == NULL) {
		errno = EINVAL;
		return -1;
	}

	if(n > s->count) {
		errno = ESRCH;
		return -1;
	}

	if(sadd(s, el) == -1)
		/* ENOMEM? */
		return -1;

	for(i = s->count; i > n; i--) {
		s->list[i] = s->list[i-1];
		s->lens[i] = s->lens[i-1];
	}
	s->list[n] = s->list[s->count];
	s->lens[n] = s->lens[s->count];

	s->list[s->count] = NULL;
	s->lens[s->count] = 0;

	return n;
}

ssize_t
sfind(svect *sl, const char *what) {
	int n;
	int wlen;
	short sw;
	int slen;

	if(sl == NULL || sl->count == NULL || what == NULL) {
		errno = EINVAL;
		return -1;
	}

	wlen = strlen(what);
	if(wlen)
		sw = *(short *)what;
	else
		sw = 0;	/* will never be used */

	for(n = 0; n < sl->count; n++) {
		slen = sl->lens[n];

		if(slen >= 0) {
			if(slen != wlen)
				continue;

			if(wlen) {
				if(
					(*(short *)sl->list[n] == sw)
					&& !strcmp(sl->list[n], what)
				) return n;
			} else {
				return n;
			}

		} else {
			/*
			 * Don't rely on sl->lens[n]'s idea of string length.
			 */
			if(strcmp(sl->list[n], what) == 0)
				return n;
		}

	}

	errno = ESRCH;
	return -1;
}

ssize_t
scfind(svect *sl, const char *what) {
	size_t n;
	int wlen;
	int slen;

	if(sl == NULL || sl->count == NULL || what == NULL) {
		errno = EINVAL;
		return -1;
	}

	wlen = strlen(what);

	for(n = 0; n < sl->count; n++) {
		slen = sl->lens[n];

		if(slen >= 0 && slen != wlen)
			continue;

		if(wlen) {
			if(ecq(sl->list[n], what))
				return n;
		} else {
			return n;
		}
	}

	errno = ESRCH;
	return -1;
}

char *
sget2(svect *sl, const char *what, svect *sl2) {
	int n;

	if((n=sfind(sl, what)) == -1)
		/* ESRCH? */
		return NULL;

	if(n >= sl2->count) {
		errno = ESRCH;
		return NULL;
	}

	return sl2->list[n];
}

char *
scget2(svect *sl, const char *what, svect *sl2) {
	int n;

	if((n=scfind(sl, what)) == -1)
		/* ESRCH? */
		return NULL;

	if(n >= sl2->count) {
		errno = ESRCH;
		return NULL;
	}

	return sl2->list[n];
}


char **
sarray(svect *sl, size_t start) {

	if(sl == NULL)
		return NULL;

	if(start > sl->count)
		start=sl->count;

	return &(sl->list[start]);
}

char **
mkarray(svect *sl, size_t start) {
	size_t n;
	char **v;
	char **vp;

	if(sl == NULL) {
		errno = EINVAL;
		return NULL;
	}

	n=sizeof(char *) * (sl->count - start + 1);
	vp=v=(char **)sf_malloc(n);
	if(v == NULL) {
		/* ENOMEM? */
		return NULL;
	}

	for( n=start; n < sl->count; n++) {
		*vp=sf_strdup(sl->list[n]);
		if(*vp == NULL) {
			free_values(v);
			/* ENOMEM? */
			return NULL;
		}
		vp++;
	}

	*vp=NULL;

	return v;
}


/* ==================================================================== */

ssize_t
find(char **vp, const char *what) {
	int z;

	if(vp == NULL || what == NULL) {
		errno = EINVAL;
		return -1;
	}

	for(z = 0; *vp; vp++, z++) {
		if(strcmp(*vp, what) == 0)
			return z;
	}

	errno = ESRCH;
	return -1;
}

ssize_t
cfind(char **vp, const char *what) {
	int z;

	if(vp == NULL || what == NULL) {
		errno = EINVAL;
		return -1;
	}

	for(z = 0; *vp; vp++, z++) {
		if(ecq(*vp, what))
			return z;
	}

	errno = ESRCH;
	return -1;
}


/*
 * Code for people
 
Сидят в баре звери. Вдруг заяц открывает дверь пинком,
подходит ко льву и говорит:
- А ну, пошли.
Лев:
- Пошли.
Через некоторое время лев возвращается - весь в крови, с мордой
исцарапанной. Удивленные звери спрашивают:
- Ну что?
- Да кто ж знал, что он по крыжовнику побежит?

 */


