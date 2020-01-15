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
 * $Id: svectpair.c,v 1.3 2002/11/30 10:24:03 vlm Exp $
 */

#include <string.h>
#include "strfunc.h"

char *
sgetp(svectpair *slp, const char *what) {
	if(slp == NULL)
		return NULL;

	return sget2(slp->key, what, slp->val);
}

char *
scgetp(svectpair *slp, const char *what) {
	if(slp == NULL)
		return NULL;

	return scget2(slp->key, what, slp->val);
}

int
saddp(svectpair *sp, char *key, char *val, int flags) {
	if(sp == NULL || key == NULL || val == NULL)
		return -1;

	if(sp->key == NULL)
		if((sp->key = sinit()) == NULL)
			return -1;

	if(sp->val == NULL)
		if((sp->val = sinit()) == NULL)
			return -1;

	if(flags & 1) {
		if(sadd_attach(sp->key, key, strlen(key)) == -1)
			return -1;
		if(sadd_attach(sp->val, val, strlen(val)) == -1) {
			sdel(sp->key, sp->key->count - 1);
			return -1;
		}
	} else {
		if(sadd(sp->key, key) == -1)
			return -1;
		if(sadd(sp->val, val) == -1) {
			sdel(sp->key, sp->key->count - 1);
			return -1;
		}
	}

	return sp->key->count;
}

