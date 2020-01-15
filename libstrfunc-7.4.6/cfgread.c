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
 * $Id: cfgread.c,v 1.11 2002/11/30 10:56:34 vlm Exp $
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#ifdef	HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef	HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef	HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef	HAVE_MMAP
#include <sys/mman.h>
#endif

#include "strfunc.h"

char *_sf_cfg_i_cf;
char *_sf_cfg_i_cfpos;
off_t _sf_cfg_i_cfsize;

int sfcfgparse();

extern int _sf_cfg_i_nline;
extern int _sf_cfg_i_brcount;

/*
 * Place for configuration data.
 */
svect *_sf_cfg_i_a;
svect *_sf_cfg_i_v;

int
cfgread(const char *cfname) {
	int fd;
	struct stat sb;
	int ret = -1;
#ifndef	HAVE_MMAP
	ssize_t rd, rds;
	char *buf;
#endif

	if(cfname == NULL) {
		fprintf(stderr, "-ERR: filename required.\n");
		return -1;
	}

	if((fd=open(cfname, O_RDONLY)) == -1 || fstat(fd, &sb)) {
		fprintf(stderr, "-ERR: Can't open config file.\n");
		return -1;
	}

	_sf_cfg_i_cfsize = sb.st_size + 1;
	
#ifdef	HAVE_MMAP
	_sf_cfg_i_cfpos = _sf_cfg_i_cf = (char *)mmap(NULL,
			_sf_cfg_i_cfsize, PROT_READ | PROT_WRITE, MAP_PRIVATE,
				fd, 0);
	if(_sf_cfg_i_cf == MAP_FAILED) {
		close(fd);
		fprintf(stderr, "-ERR: Can't mmap() config file.\n");
		return -1;
	}
#else	/* HAVE_MMAP */
	buf = (char *)sf_malloc(_sf_cfg_i_cfsize);
	if(buf == NULL) {
		close(fd);
		return -1;
	}
	rds = 0;
	while(rds < sb.st_size) {

		rd = read(fd, buf + rds, sb.st_size - rds);
		if(rd == -1) {
			free(buf);
			fprintf(stderr, "-ERR: Can't correctly read() config file.\n");
			return -1;
		} else if(rd == 0) {
			/* Correct the config size. */
			_sf_cfg_i_cfsize = rds;
			break;
		}
		rds += rd;
	}

	buf[rds] = 0;

	_sf_cfg_i_cfpos = _sf_cfg_i_cf = buf;
#endif	/* HAVE_MMAP */

	_sf_cfg_i_nline = 0;
	_sf_cfg_i_brcount = 0;

	/*
	 * Initialize configuration storage.
	 */
	if(_sf_cfg_i_a) {
		sclear(_sf_cfg_i_a);
	} else {
		_sf_cfg_i_a = sinit();
		if(_sf_cfg_i_a == NULL)
			goto finish;
	}
	if(_sf_cfg_i_v) {
		sclear(_sf_cfg_i_v);
	} else {
		_sf_cfg_i_v = sinit();
		if(_sf_cfg_i_v == NULL)
			goto finish;
	}

	/*
	 * Parse prepared buffer.
	 */
	ret = 0;
	if(sfcfgparse())
		ret = 1;

finish:

	close(fd);
#ifdef	HAVE_MMAP
	munmap(_sf_cfg_i_cf, _sf_cfg_i_cfsize);
#else
	free(_sf_cfg_i_cf);
#endif

	return ret;
}

char *cfgget(char *at) {
	return sget2(_sf_cfg_i_a, at, _sf_cfg_i_v);
}

svect *
cfgget2(char *at) {
	svect *sl = NULL;
	int i;

	if(_sf_cfg_i_a == NULL) {
		errno = EINVAL;
		return NULL;
	}

	sl = sinit();
	if(sl == NULL) {
		/* Errno will be set by sinit() */
		return NULL;
	}

	if(at == NULL)
		return sl;

	for(i = 0; i< _sf_cfg_i_a->count; i++)
		if(ecq(_sf_cfg_i_a->list[i], at)) {
			if(sadd(sl, _sf_cfg_i_v->list[i]) == -1) {
				sfree(sl);
				/* ENOMEM? */
				return NULL;
			}
		}

	return sl;
}

