/*-
 * Copyright (c) 2000, 2001 Lev Walkin <vlm@spelio.net.ru>.
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
 * $Id: posdump.c,v 1.5 2001/12/07 21:46:44 vlm Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#ifdef	HAVE_SYSEXITS_H
#include <sysexits.h>
#else	/* HAVE_SYSEXITS_H */
#define	EX_USAGE	64
#define	EX_DATAERR	65
#define	EX_NOINPUT	66
#define	EX_OSERR	71
#endif	/* HAVE_SYSEXITS_H */

#include <strfunc.h>

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

#ifndef F_TLOCK
# ifdef _LK_NBLCK
#  define F_TLOCK _LK_NBLCK
# endif
#endif

#ifdef	HAVE_GETOPT_H
#include <getopt.h>
#endif

#if !defined(HAVE_LOCKF) && defined(HAVE__LOCKING)
# define lockf _locking
# define HAVE_LOCKF 1
#endif

#if !defined(HAVE_FTRUNCATE) && defined(HAVE__CHSIZE)
# define ftruncate _chsize
# define HAVE_FTRUNCATE 1
#endif

void usage(char *name);

typedef struct {
	ino_t	inode __attribute__ ((packed));
	off_t	offset __attribute__ ((packed));
	time_t	tstamp __attribute__ ((packed));
} pos_t;

int
main(int ac, char **av) {
	int pfd;
	struct stat sb;
	pos_t pos;
	ssize_t size;
	int c;
	char *crea = NULL;
	int trunc = 0;
	int save = 0;
	int ret = 0;

	while((c = getopt(ac, av, "c:tsr:")) != -1)
	switch(c) {
		case 'c':
			if(crea)
				usage(*av);
			crea = optarg;
			break;
		case 't':
			trunc = 1;
			break;
		case 's':
			save = 1;
			break;
		case 'r':
			ret = atoi(optarg);
			if(ret < 1) {
				fprintf(stderr, "Invalid -r value.\n");
				usage(*av);
			};
			break;
		default:
			usage(*av);
	};

	if(crea && trunc && save && ret)
		usage(*av);

	ac -= optind;
	av += optind;

	if(ac != 1)
		usage(*(av-optind));

	if(crea) {
		FILE *fp = fopen(crea, "r");
		if(!fp) {
			fprintf(stderr, "Can't open source file.\n");
			exit(EX_NOINPUT);
		};
		if((pfd=initFILEpos(crea, fp, *av)) == -1) {
			fprintf(stderr, "Can't create position file.\n");
			exit(EX_OSERR);
		};
		printf("Position file for %s initialized.\n", crea);

		lseek(pfd, 0L, SEEK_SET);

	} else {

		pfd = open(*av, O_RDWR, 0644);
		if(!pfd) {
			fprintf(stderr, "No such file: %s\n", *av);
			exit(EX_NOINPUT);
		};

		printf("File:\t%s\n", *av);
	};

	if(fstat(pfd, &sb)) {
		fprintf(stderr, "Can't fstat() file.\n");
		exit(EX_OSERR);
	};
	if((sb.st_mode & S_IFMT) != S_IFREG) {
		fprintf(stderr, "It is not a regular file.\n");
		exit(EX_DATAERR);
	};

	if(  (sb.st_size == 0)
	  ||  ( (sb.st_size != sizeof(ino_t) + sizeof(off_t))
	  && (sb.st_size % sizeof(pos_t)) )
	) {
		fprintf(stderr, "Incorrect file size %ld, must be %d or %d.\n",
			(long)sb.st_size, sizeof(ino_t) + sizeof(off_t), sizeof(pos_t));
		exit(EX_DATAERR);
	};

	if(lockf(pfd, F_TLOCK, sb.st_size)) {
		fprintf(stderr, "Note: File is locked by anyone else.\n");
	}

	if(trunc) {
		if(ftruncate(pfd, sizeof(pos_t)) == -1) {
			fprintf(stderr, "Can't truncate file.\n");
			close(pfd);
			exit(EX_OSERR);
		};
	};

	if(save) {
		size = read(pfd, &pos, sizeof(pos));
		if(size != sizeof(pos)) {
			fprintf(stderr, "Incorrect read count: %d out of %d\n",
				size, sizeof(pos));
			close(pfd);
			exit(EX_OSERR);
		};

		if(lseek(pfd, 0L, SEEK_END) == -1) {
			fprintf(stderr, "Can't seek() to the end of file.\n");
			exit(EX_OSERR);
		};

		if(write(pfd, &pos, sizeof(pos)) != sizeof(pos)) {
			fprintf(stderr, "Error writing %d bytes!\n", sizeof(pos));
			exit(EX_OSERR);
		};

		if(lseek(pfd, 0L, SEEK_SET) == -1) {
			fprintf(stderr, "Can't seek() to the start.\n");
			exit(EX_OSERR);
		};
	};

	if(ret) {
		pos_t savedpos;
		size = read(pfd, &pos, sizeof(pos));
		if(size != sizeof(pos)) {
			fprintf(stderr, "Incorrect read count: %d out of %d\n",
				size, sizeof(pos));
			close(pfd);
			exit(EX_OSERR);
		};

		if(lseek(pfd, ret * sizeof(pos_t), SEEK_SET) == -1) {
			fprintf(stderr, "Can't seek().\n");
			exit(EX_OSERR);
		};

		size = read(pfd, &savedpos, sizeof(savedpos));
		if(size != sizeof(savedpos)) {
			if(size)
				fprintf(stderr, "Incorrect read count: %d out of %d\n",
					size, sizeof(savedpos));
			else
				fprintf(stderr, "No saved data found at #%d\n", ret);
			close(pfd);
			exit(EX_OSERR);
		};

		if(pos.inode != savedpos.inode) {
			fprintf(stderr, "Can't swap current and saved position indicators: inodes are different.\n");
			exit(EX_DATAERR);
		};

		if(lseek(pfd, ret * sizeof(pos_t), SEEK_SET) == -1) {
			fprintf(stderr, "Can't seek().\n");
			exit(EX_OSERR);
		};

		time(&pos.tstamp);

		if((write(pfd, &pos, sizeof(pos))) != sizeof(pos)) {
			fprintf(stderr, "Error writing %d bytes!\n", sizeof(pos));
			exit(EX_OSERR);
		};

		if(lseek(pfd, 0L, SEEK_SET) == -1) {
			fprintf(stderr, "Can't seek().\n");
			exit(EX_OSERR);
		};

		time(&savedpos.tstamp);

		if(write(pfd, &savedpos, sizeof(savedpos)) != sizeof(savedpos)) {
			fprintf(stderr, "Error writing %d bytes!\n", sizeof(savedpos));
			exit(EX_OSERR);
		};

		if(lseek(pfd, 0L, SEEK_SET) == -1) {
			fprintf(stderr, "Can't seek().\n");
			exit(EX_OSERR);
		};

	};

	c = 0;

  while((size = read(pfd, &pos, sizeof(pos))) >= 0) {

	if(!size)
		break;

	if(size != sizeof(pos)) {
		fprintf(stderr, "Incorrect read count: %d out of %d\n",
			size, sizeof(pos));
		close(pfd);
		exit(EX_OSERR);
	};

	if(c)
		printf("Saved data found: #%d\n", c);
	c++;

	printf("\tInode:\t%ld\n", (long)pos.inode);
	printf("\tOffset:\t%ld\n", (long)pos.offset);
	printf("\tTime:\t%s\n",
		timetostr(pos.tstamp, TFMT_RFC822 | TFMT_UF | TFMT_LOCAL));

  };

	close(pfd);

	return 0;
};


void
usage(char *name) {
	fprintf(stderr,
	"Position file dumper.\n"
	"Usage: %s [{-c <file>|-s|-t|-r <n>}] <file.pos>\n"
	"\t-c:\tCreate position file for <file> with <file.pos> extension\n"
	"\t-s:\tSave current position\n"
	"\t-r:\tReturn n'th position by swapping with default\n"
	"\t-t:\tClear (truncate) saved positions\n"
	, name
	);
	exit(EX_USAGE);
};
