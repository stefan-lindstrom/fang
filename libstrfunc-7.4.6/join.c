/*-
 * Copyright (c) 1999, 2000, 2001 Lev Walkin <vlm@spelio.net.ru>.
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
 * $Id: join.c,v 1.10 2002/11/30 10:24:03 vlm Exp $
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>

#ifdef	HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef	HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "strfunc.h"


char *_sf_sjoin_buf = NULL;

char *
sjoin(svect *s, char *fmt) {
	char *buf;
	int flen;
	int k, need;
	char *p;

	if(s == NULL || s->count == 0) {
		if(_sf_sjoin_buf) free(_sf_sjoin_buf);
		return (_sf_sjoin_buf = sf_strdup(""));
	}

	if(fmt) {
		flen = strlen(fmt);
	} else {
		fmt = "";
		flen = 0;
	}

	need = 1;
	for(k = 0; k < s->count; k++)
		need +=
			(s->lens[k] >= 0 ? s->lens[k] : strlen(s->list[k]))
			+ (k?flen:0);

	buf = (char *)sf_malloc(need);
	if(buf == NULL)
		/* ENOMEM? */
		return NULL;

	p=buf;

	for(k = 0; k < s->count; k++) {
		if(k) {
			memcpy(p, fmt, flen);
			p += flen;
		}
		if((need = s->lens[k]) < 0)
			need = strlen(s->list[k]);
		memcpy(p, s->list[k], need);
		p += need;
	}
	*p = 0;

	if(_sf_sjoin_buf) free(_sf_sjoin_buf);
	return (_sf_sjoin_buf=buf);
}

char *_sf_join_buf = NULL;

char *
join(char **vals, char *fmt) {
	char *buf;
	int flen;
	int need;
	char *p;
	char **v;

	if(vals == NULL || *vals == NULL) {
		if(_sf_join_buf)
			free(_sf_join_buf);
		return (_sf_join_buf = sf_strdup(""));
	}

	if(fmt == NULL) {
		fmt = "";
		flen = 0;
	} else {
		flen = strlen(fmt);
	}

	need = 1;
	for(v = vals; *v; v++)
		need += strlen(*v) + ((v==vals)?0:flen);

	buf = (char *)sf_malloc(need);
	if(buf == NULL)
		/* ENOMEM? */
		return NULL;

	p=buf;

	for(v = vals; *v; v++) {
		if(v != vals) {
			memcpy(p, fmt, flen);
			p += flen;
		}
		strcpy(p, *v);
		p += strlen(*v);
	}
	*p = '\0';

	if(_sf_join_buf) free(_sf_join_buf);
	return (_sf_join_buf = buf);
}

