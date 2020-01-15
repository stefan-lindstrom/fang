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
 * $Id: sbuf.c,v 1.22 2002/12/13 11:15:20 vlm Exp $
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <errno.h>

#ifdef	HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef	HAVE_VSNPRINTF
#ifdef	HAVE__VSNPRINTF
#define	vsnprintf	_vsnprintf
#else
#error	"This system has neither vsnprintf() nor _vsnprintf()"
#endif
#endif

#include "strfunc.h"

#define	INITIAL_SBUF_SIZE	16

sbuf *
sbuf_init() {
	sbuf *sb;
	sb = (sbuf *)sf_calloc(1, sizeof(sbuf));
	if(sb == NULL)
		/* ENOMEM? */
		return NULL;

	/* Initial fill */
	if(sbuf_clear(sb) == -1) {
		free(sb);
		return NULL;
	}

	return sb;
}

int
sbuf_clear(sbuf *sb) {
	char *c;

	if(sb == NULL) {
		errno = EINVAL;
		return -1;
	}

	c = (char *)sf_realloc(sb->buf, INITIAL_SBUF_SIZE);
	if(c) {
		sb->buf = c;
		sb->size = INITIAL_SBUF_SIZE;
	} else {
		if(sb->buf == NULL || sb->size == 0) {
			/* ENOMEM? */
			return -1;
		}
	}

	sb->len = 0;
	sb->off = 0;
	*sb->buf = 0;

	return 0;
}

int
sbuf_zero(sbuf *sb) {

	if(sb == NULL) {
		errno = EINVAL;
		return -1;
	}

	sb->len = 0;
	sb->off = 0;

	if(sb->buf) {
		*sb->buf = 0;
	} else {
		return sbuf_clear(sb);
	}

	return 0;
}

void
sbuf_free(sbuf *sb) {
	if(sb == NULL)
		return;

	if(sb->buf) {
		free(sb->buf);
		sb->buf = NULL;
	}

	free(sb);
}

/*
 * Detach the meaningful buffer.
 */
char *
sbuf_detach(sbuf *sb, size_t *optLength, size_t *optSize) {
	char *ret_buf;

	if(sb == NULL) {
		errno = EINVAL;
		return NULL;
	}

	if(sb->buf == NULL) {
		if(sbuf_clear(sb) == -1)
			/* ENOMEM? */
			return NULL;
	}

	if(sb->off == 0) {
		ret_buf = sb->buf;

		if(optLength)
			*optLength= sb->len;
		if(optSize)
			*optSize = sb->size;
	} else {
		int meaningful_len = (sb->len - sb->off);

		if(meaningful_len <= 0) {
			ret_buf = sb->buf;
			*ret_buf = 0;

			if(optLength)
				*optLength = 0;
			if(optSize)
				*optSize = sb->size;
		} else {
			ret_buf = sb->buf;
			memmove(sb->buf, sb->buf + sb->off, meaningful_len);
			sb->buf[meaningful_len] = 0;

			if(optLength)
				*optLength = meaningful_len;
			if(optSize)
				*optSize = meaningful_len + 1;
		}
	}

	/* Reinitialize buffer */
	sb->buf = NULL;
	(void)sbuf_clear(sb);

	return ret_buf;
}

#define	sizeround(foo,prevsize)	((foo)+((prevsize)-1))&(~((prevsize)-1))

ssize_t
sbuf_extend(sbuf *sb, size_t newsize) {
	char *c;

	if(sb == NULL) {
		errno = EINVAL;
		return -1;
	}

	if(newsize < sb->size)
		return sb->size;

	newsize = sizeround(newsize + 1, sb->size);

	c = (char *)sf_realloc(sb->buf, newsize);
	if(c == NULL) {
		/* ENOMEM? */
		return -1;
	}

	sb->buf = c;
	sb->size = newsize;

	/* Clear the rest of the buffer */
	c = sb->buf + sb->len;
	newsize = sb->size - sb->len;
	memset(c, 0, newsize);

	return sb->size;
}

ssize_t
sbuf_add(sbuf *sb, const char *str) {
	register size_t size;

	if(str == NULL) {
		errno = EINVAL;
		return -1;
	}

	size = strlen(str);

	if( sbuf_add2(sb, (void *)str, size) == -1 )
		return -1;

	return size;
}

ssize_t
sbuf_add2(sbuf *sb, void *data, size_t nbytes) {
	size_t expect;

	if(sb == NULL || data == NULL || nbytes == (size_t)-1) {
		errno = EINVAL;
		return -1;
	}

	if(nbytes == 0)
		return 0;

	expect = sb->len + nbytes;

	if(expect >= sb->size) {
		char *p;

		if(sb->off >= nbytes) {
			memmove(sb->buf, sb->buf + sb->off, sb->len - sb->off);
			sb->len -= sb->off;
			sb->off = 0;
		} else {
			size_t newsize = sizeround(expect + 1, sb->size);

			p = (char *)sf_realloc(sb->buf, newsize);
			if(p == NULL)
				return -1;

			sb->buf = p;
			sb->size = newsize;
		}
	}

	memcpy(sb->buf + sb->len, data, nbytes);

	sb->len += nbytes;
	sb->buf[sb->len] = '\0';

	return sb->len;
}


ssize_t
sbuf_sprintf(sbuf *sb, char *fmt, ...) {
	size_t avail;	/* Available buffer size */
	int size;
	va_list ap;

	if(sb == NULL || fmt == NULL) {
		errno = EINVAL;
		return -1;
	}

	avail = sb->size - sb->len;
	if(avail < 2) {
		if(sbuf_extend(sb, sb->size + 2) == -1)
			return -1;
		avail = sb->size - sb->len;
	}

	va_start(ap, fmt);

	while(
		(size = vsnprintf(sb->buf + sb->len, avail, fmt, ap)) < 0
		|| size >= avail)
	{
		if(size <= 0)
			size = sb->size;
		if(sbuf_extend(sb, sb->size + size + 1) == -1) {
			/* Clear the tails... */
			sb->buf[sb->len] = 0;
			return -1;
		}
		avail = sb->size - sb->len;
	}

	va_end(ap);

	sb->len += size;
	sb->buf[sb->len] = 0;

	return size;
}

char *
sbuf_fgets(sbuf *sb, FILE *fp) {
	size_t avail = 0;	/* Available buffer size */
	ssize_t saved=-1;
	char *p;

	if(sb == NULL || fp == NULL) {
		errno = EINVAL;
		return NULL;
	}

  while(1) {

	avail = sb->size - sb->len;
	if(avail < 2) {
		if(sbuf_extend(sb, sb->size + 2) == -1)
			/* ENOMEM? */
			return NULL;
		avail = sb->size - sb->len;
	}

	p = fgets(sb->buf + sb->len, avail, fp);
	if(p == NULL) {
		/* Something still in the buffer */
		if(saved != -1)
			return (sb->buf + saved);
		errno = 0;
		return NULL;
	}

	if(saved == -1)
		saved = sb->len;

	if(*p == '\0')
		continue;

	sb->len += strlen(p);
	if( sb->buf[sb->len - 1] == '\n' )
		return sb->buf + saved;

  }	/* while(1) */

	errno = 0;
	return NULL;
}

/*
 * Fetch specified number of characters from smart buffer.
 */

char *
sbuf_fetch(sbuf *sb, size_t len, char *optDelimiter, size_t *optRLen, int flags) {
	char *ret_buf = NULL;
	char dlen;

	if(sb == NULL || sb->buf == NULL) {
		errno = EINVAL;
		return NULL;
	}

	if((sb->len - sb->off) <= 0) {
		errno = ESRCH;	/* No error */
		return NULL;
	}

	if(len == 0 && optDelimiter == NULL) {
		errno = EINVAL;
		return NULL;
	}

	if(len) {
		/* If request length more than avaliable, return the whole */
		if(len >= (sb->len - sb->off))
			return sbuf_detach(sb, optRLen, NULL);

		ret_buf = (char *)sf_malloc(len + 1);
		if(ret_buf == NULL)
			/* ENOMEM */
			return NULL;

		memcpy(ret_buf, sb->buf + sb->off, len);
		sb->off += len;
		ret_buf[len] = 0;

		if(optRLen)
			*optRLen = len;

	} else {
		char *p = sb->buf + sb->off;
		size_t k;
		int start = 1;
		size_t skip = 0;

		len = sb->len - sb->off;
		dlen = strlen(optDelimiter);

		if(flags & 2) {
			char ds = *optDelimiter;

			/* Split by string set */
			for(k = 0; k < len; k++, p++) {
				if(*p == ds	/* Pre-check to not to invoke function */
					&& strncmp(p, optDelimiter, dlen) == 0) {
					if(flags & start) {
						skip += dlen;
						k += dlen - 1;
						p += dlen - 1;
						continue;
					} else {
						break;
					}
				}
				start = 0;
			}

			if(skip) {
				len -= skip;
				k -= skip;
				sb->off += skip;
			}

		} else {

			/* Split by character set */
			for(k = 0; k < len; k++, p++) {
				if(*p	/* Don't mind intermittent NUL's */
					&& memchr(optDelimiter, *p, dlen)) {
					if(flags & start) {
						skip++;
						continue;
					} else {
						break;
					}
				}
				start = 0;
			}

			if(skip) {
				len -= skip;
				k -= skip;
				sb->off += skip;
			}

		}

		/* Buffer does not contain delimiters */
		if(k == len) {
			errno = EAGAIN;
			return NULL;
		}

		ret_buf = (char *)sf_malloc(k + 1);
		if(ret_buf == NULL) {
			/* ENOMEM? */
			return NULL;
		}
		memcpy(ret_buf, sb->buf + sb->off, k);
		ret_buf[k] = 0;
		sb->off += k;
		if(optRLen)
			*optRLen = k;

		if(flags & 2) {
			if(!k && !(flags & 1) && len && *p) {
				if(strncmp(p, optDelimiter, dlen) == 0) {
					sb->off += dlen;
				}
			}
		} else {
			if(!k && !(flags & 1)) {
				sb->off++;
				/* Skip trailing delimiter characters */
				for(k++, p++; k < len; k++, p++) {
					if(!*p || strchr(optDelimiter, *p)
							== NULL)
						break;
					sb->off++;
				}
			}
		}

	}

	return ret_buf;
}

