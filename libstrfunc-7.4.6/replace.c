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
 * $Id: replace.c,v 1.7 2002/11/30 10:24:03 vlm Exp $
 */

#include <string.h>
#include <stdlib.h>
#include "strfunc.h"

char *_sf_replbuf = NULL;
size_t	_sf_replas = 0;

char *
replace_detach() {
	char *p=_sf_replbuf;

	if(!p)
		p=sf_strdup("");

	_sf_replbuf = NULL;
	return p;
}

char *
replace(char *b, char *w, char *t) {
	char *buf=NULL;
	int wlen;	/* what len */
	int tlen = 0;	/* to len */
	register char cw, cb;
	register size_t blen;
	register char *p, *pe;

	if(!b || !w || !t || !*b || !*w)
		return b;

	wlen = strlen(w);

	if(!(blen = _sf_replas))
		blen = 16;

	p=buf=(char *)sf_malloc( blen );
	if(!p)
		return NULL;
	pe = p + blen;

	for(cw = *w; (cb = *b); b++) {
		if(
			cb != cw
			|| strncmp(b, w, wlen)
		) {
			*p++ = cb;
			if(p >= pe) {
				register long z = p - buf;
				p = (char *)sf_realloc(buf, blen <<= 1);
				if(!p) {
					free(buf);
					return NULL;
				}
				buf = p;
				p += z;
				pe = buf + blen;
			}
			continue;
		}

		if(!tlen)
			tlen = strlen(t);

		if(p + tlen >= pe) {
			register long z = p - buf;
			blen = (tlen + (blen << 1)) & (~(blen-1));
			p = (char *)sf_realloc(buf, blen);
			if(!p) {
				free(buf);
				return NULL;
			}
			buf = p;
			p += z;
			pe = buf + blen;
		}

		memcpy(p, t, tlen);
		p += tlen;
		b += wlen-1;
	}
	*p = '\0';

	if(blen > _sf_replas)
		_sf_replas = blen;

	if(_sf_replbuf)
		free(_sf_replbuf);

	return (_sf_replbuf=buf);
}


