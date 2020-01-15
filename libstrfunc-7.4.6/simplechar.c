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
 * $Id: simplechar.c,v 1.9 2002/11/30 10:24:03 vlm Exp $
 */

#include <string.h>
#include <stdlib.h>

#ifdef	HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef	HAVE_SNPRINTF
#ifdef	HAVE__SNPRINTF
#define	snprintf	_snprintf
#else
#error	"This system has neither snprintf() nor _snprintf()"
#endif
#endif

#include "strfunc.h"

int
countchar(const char *s, char c) {
	int cnt = 0;
	char *p = (char *)s;

	if(s == NULL || *s == '\0')
		return 0;

	while(*p)
		if(*p++ == c)
			cnt++;

	return cnt;
}

int
countchar2(const char *s, const char *c) {
	int cnt = 0;
	char *p = (char *)s;

	if(s == NULL || *s == '\0' || c == NULL)
		return 0;

	while(*p)
		if(strchr(c, *p++))
			cnt++;

	return cnt;
}

int
countstr(const char *b, const char *l) {
	int n;
	int len;

	if(b == NULL || l == NULL)
		return 0;

	if(*b == '\0')
		return 0;

	len = strlen(l);
	if(len == 0)
		return 0;

	if(len == 1)
		return countchar(b, *l);

	for(n = 0; *b; b++) {
		if(
			(*(short *)b == *(short *)l)
			&& !strncmp(b, l, len)
		) {
			b += len - 1;
			n++;
		}
	}

	return n;
}


char *
genstr(int i) {
	static char buf[32];

	snprintf(buf, sizeof(buf), "%d", i);

	return buf;
}


char **
charize(const char *what) {
	char **vp;

	vp=(char **)sf_malloc(sizeof(char *) << 1);
	if(vp == NULL) {
		return NULL;
	}

	*vp = sf_strdup((char *)what?(char *)what:"");
	if(*vp == NULL) {
		free_values(vp);
		return NULL;
	}

	vp[1]=NULL;  

	return vp;
}

size_t
count_values(char **vp) {
	int z;

	if(vp == NULL)
		return 0;

	for(z = 0; *vp; vp++, z++);

	return z;
}
		
void
free_values(char **vp) {
	char **p;

	if( vp == NULL)
		return;

	for(p = vp; *p; p++)
		free(*p);

	free(vp);
}

int
copy_values(char **fp, char ***tp) {
	char **vp;

	if(tp == NULL || fp == NULL)
		return 0;

	vp=*tp=(char **)sf_calloc((count_values(fp) + 4), sizeof(char *));
	if(vp == NULL)
		return 0;

	for(; *fp; vp++, fp++) {
		*vp = sf_strdup(*fp);
		if(*vp == NULL) {
			free_values(vp);
			*tp = NULL;
			return -1;
		}
	}

	return 1;
}


char *
brace(char *s, char c) {
	char f;
	int level = 0;

	if(s == NULL || (f = *s) == '\0')
		return NULL;

	while(*(++s)) {
		if((*s == c) && !(level--))
			return s;
		else if(*s == f)
			level++;
	}
		

	return NULL;
}


char *
bracen(char *s, char c, size_t len) {
	char f;
	int level = 0;

	if(s == NULL || (f = *s) == '\0')
		return NULL;

	while(len--) {
		++s;
		if((*s == c) && !(level--))
			return s;
		else if(*s == f)
			level++;
	}
		

	return NULL;
}

