/*-
 * Copyright (c) 2000, 2001, 2002 Lev Walkin <vlm@spelio.net.ru>.
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
 * $Id: filepos.c,v 1.11 2002/11/30 10:24:03 vlm Exp $
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

#ifdef	HAVE_TIME_H
#include <time.h>
#endif

#ifdef	HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#ifdef	HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef	HAVE_SYS_LOCKING_H
#include <sys/locking.h>
#endif

#if !defined(HAVE_FCNTL) && !defined(HAVE_LOCKF) && defined(HAVE__LOCKING)
# define lockf _locking
# define HAVE_LOCKF 1
#endif

#ifndef F_TLOCK
# ifdef _LK_NBLCK
#  define F_TLOCK _LK_NBLCK
# endif
#endif

#include "strfunc.h"

int
initfdpos(const char *filename, int stream, const char *ext) {
	char *pos_fname;
	int posfd;
	off_t ofs = 0;
	ino_t ino = 0;
	time_t tloc;
	struct stat sb;
	int flen, elen;
#ifdef	HAVE_FCNTL
	struct flock flk;
#endif	/* HAVE_FCNTL */


	if(filename == NULL || *filename == '\0'
		|| ext == NULL || *ext == '\0') {
		errno = EINVAL;
		return -1;
	}

	flen = strlen(filename);
	elen = strlen(ext);

	pos_fname = (char *)alloca(flen + elen + 2);

	/* filename.ext */
	memcpy(pos_fname, filename, flen);
	pos_fname[flen] = '.';
	memcpy(pos_fname + flen + 1, ext, elen + 1);
	/* it will be naturally NUL-terminated because of elen + 1 */

	posfd = open(pos_fname, O_RDWR | O_CREAT, 0644);
	if(posfd == -1)
		return -1;

#ifdef	HAVE_FCNTL
	memset(&flk, 0, sizeof(flk));
	flk.l_type = F_WRLCK;
	flk.l_whence = SEEK_SET;

	if(fcntl(posfd, F_SETLK, &flk) == -1) {
		close(posfd);
		return -1;
	}
#elif defined(HAVE_LOCKF)
	if(lockf(posfd, F_TLOCK, sizeof(ino_t) + sizeof(off_t)) != 0) {
		close(posfd);
		return -1;
	}
#else
#warning	"File locking unavailable. filepos(3) may malfunction"
#endif	/* HAVE_FCNTL */

	if(fstat(posfd, &sb)) {
		close(posfd);
		return -1;
	}

	if((sb.st_mode & S_IFMT) != S_IFREG) {
		close(posfd);
		return -1;
	}

	time(&tloc);

	lseek(posfd, 0, SEEK_SET);

	if(  (sb.st_size == 0)
	  ||  ( (sb.st_size != sizeof(ino_t) + sizeof(off_t))
	  && (sb.st_size % (sizeof(ino_t) + sizeof(off_t) + sizeof(time_t))) )
	) {

		fstat(stream, &sb);
		ino = sb.st_ino;
		ofs = 0;

		write(posfd, &ino, sizeof(ino_t));
		write(posfd, &ofs, sizeof(off_t));
		write(posfd, &tloc, sizeof(time_t));

	} else {
		/* If could not read basic info */
		if(read(posfd, &ino, sizeof(ino_t)) != sizeof(ino_t)
		|| read(posfd, &ofs, sizeof(off_t)) != sizeof(off_t))
			return -1;

		fstat(stream, &sb);

		if((ino != sb.st_ino) || (ofs > sb.st_size)) {
			ino = sb.st_ino;
			ofs = 0;
			lseek(posfd, 0, SEEK_SET);
			write(posfd, &ino, sizeof(ino_t));
			write(posfd, &ofs, sizeof(off_t));
			write(posfd, &tloc, sizeof(time_t));
		}
	}

	lseek(stream, ofs, SEEK_SET);

	return posfd;
}

int
_sf_writeposfd(off_t offset, int posfd, int doSync) {

	if(offset < 0 || posfd == -1) {
		errno = EINVAL;
		return -1;
	}

	if(lseek(posfd, sizeof(ino_t), SEEK_SET) == -1)
		return -1;

	if(write(posfd, &offset, sizeof(off_t)) != sizeof(off_t))
		return -1;

	if(doSync) {
		time_t tloc;
		time(&tloc);

		write(posfd, &tloc, sizeof(time_t));

#ifdef	HAVE_FSYNC
		fsync(posfd);
#endif
	}

	return 0;
}

int
adjfdpos(int stream, int posfd, int doSync) {

	/* Determine current offset and write it to the position file */

	return _sf_writeposfd(
		lseek(stream, 0, SEEK_CUR),
		posfd, doSync);

	return 0;
}

int
initFILEpos(const char *filename, FILE *stream, const char *ext) {
	int posfd;
	off_t ofs;
	off_t ols;

	if(stream == NULL) {
		errno = EINVAL;
		return -1;
	}

	ols = lseek(fileno(stream), 0, SEEK_CUR);

	posfd=initfdpos(filename, fileno(stream), ext);
	if(posfd == -1)
		return -1;

	ofs = lseek(fileno(stream), 0, SEEK_CUR);
	if(ofs == -1)
		return -1;

	/* Rewind it where it should be. For transparency. */
	lseek(fileno(stream), ols, SEEK_SET);

	/* Advance a FILE position */
	if(fseek(stream, ofs, SEEK_SET) == -1)
		return -1;

	return posfd;
}

int
adjFILEpos(FILE *stream, int posfd, int doSync) {
	off_t ofs;
	off_t newofs;
	long fofs;

	if(stream == NULL) {
		errno = EINVAL;
		return  -1;
	}

	ofs = lseek(fileno(stream), 0, SEEK_CUR);
	fofs = ftell(stream);

	lseek(fileno(stream), fofs, SEEK_SET);

	if(adjfdpos(fileno(stream), posfd, doSync) == -1)
		return -1;

	newofs = lseek(fileno(stream), 0, SEEK_CUR);

	/* Rewind it where it should be. For transparency. */
	lseek(fileno(stream), ofs, SEEK_SET);

	if(fseek(stream, newofs, SEEK_SET) == -1)
		return -1;

	return 0;
}



