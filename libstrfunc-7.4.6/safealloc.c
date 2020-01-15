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
 * $Id: safealloc.c,v 1.10 2003/01/13 00:31:05 vlm Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>

#ifdef  HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef  HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "strfunc.h"
#include "param.h"

#ifdef	HAVE_SLEEP
# if  defined(__MINGW32__) && defined(HAVE__SLEEP)
  /* 06-dec-2001 [als] sleep() seems to be in lib, but there is no prototype */
#  define	sleep(foo)	_sleep(foo)
# endif
#else
#ifdef	HAVE__SLEEP
#define	sleep(foo)	_sleep(foo)
#else
#define	sleep(foo)	(void)(foo)
#endif
#endif

int
_sf_sa_retry(int retries) {
	static char z = 0;

	if(_sf_param_mfb_cb) {
		int tmp_errno = errno;
		_sf_param_mfb_cb();
		errno = tmp_errno;
	}

	switch(_sf_param_mfb) {
		case SF_ARG_MFB_ABORT:
			abort();
			return 0;	/* Who knows? */

		case SF_ARG_MFB_ENOMEM:
			if(errno != ENOMEM) {
				abort();
				errno = ENOMEM;
			}
			return 0;

		case SF_ARG_MFB_TRY_NOFAIL:
			sleep(5);
			return 1;

		case SF_ARG_MFB_TRY_ABORT:
			if(retries > _sf_param_mf_tries) {
				abort();
				return 0;
			}
			sleep(5);
			return 1;

		case SF_ARG_MFB_TRY_ENOMEM:
			if(retries > _sf_param_mf_tries) {
				errno = ENOMEM;
				return 0;
			}
			sleep(5);
			return 1;

		default:
			abort();
			return 0;
	}

	return 0;
}

void *
sf_malloc(size_t size) {
	void *p;
	int retry = 0;

	if(size < 1) {
		fprintf(stderr,
			"STRFUNC: Invalid argument to sf_malloc()\n");
		abort();
	}

	do {
		p = (void *)malloc(size);
	} while(p == NULL && _sf_sa_retry(++retry));

	return p;
}

void *
sf_calloc(size_t num, size_t size) {
	void *p;
	int retry = 0;

	if(size < 1) {
		fprintf(stderr,
			"STRFUNC: Invalid argument to sf_calloc()\n");
		abort();
	}


	do {
		p = (void *)calloc(num, size);
	} while(p == NULL && _sf_sa_retry(++retry));

	return p;
}

void *
sf_realloc(void *ptr, size_t size) {
	void *p;
	int retry = 0;

	if(size < 1) {
		fprintf(stderr,
			"STRFUNC: Invalid argument to sf_realloc()\n");
		abort();
	}

	do {
		p = (void *)realloc(ptr, size);
	} while(p == NULL && _sf_sa_retry(++retry));

	return p;
}

char *
sf_strdup(const char *from) {
	char *p;
	int lenZ;

	if(from == NULL) {
		fprintf(stderr,
			"STRFUNC: NULL pointer passed to sf_strdup()\n");
		abort();
	}

	lenZ = strlen(from) + 1;

	p = (char *)sf_malloc(lenZ);
	if(p == NULL)
		return NULL;

	return memcpy(p, from, lenZ);
}

/*
 * Duplicate only n first characters.
 */
char *
strndup(const char *from, size_t n) {
	char *buf;

	if(from == NULL) {
		fprintf(stderr,
			"STRFUNC: NULL pointer passed to strndup()\n");
		abort();
	}

	buf = (char *)sf_malloc(n + 1);
	if(buf == NULL)
		return NULL;

	strncpy(buf, from, n);
	buf[n] = '\0';

	return buf;
}

