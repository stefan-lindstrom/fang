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
 * $Id: timetostr.c,v 1.7 2002/05/16 22:17:50 vlm Exp $
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef	HAVE_SYSEXITS_H
#include <sysexits.h>
#else	/* HAVE_SYSEXITS_H */
#define	EX_USAGE	64
#endif	/* HAVE_SYSEXITS_H */

#ifdef	HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef	HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef	HAVE_GETOPT_H
#include <getopt.h>
#endif

#include <strfunc.h>

void usage(char *);

int timetostr_mode = 0;

int
main(int ac, char **av) {

	timetostr_mode = (av[0] && av[0][0] == 't');

	if(timetostr_mode) {
		/* timetostr mode */

		int c;
		int flags = 0;

		while((c = getopt(ac, av, "lugrif:")) != -1)
		switch(c) {
			case 'l':
				flags |= TFMT_LOCAL;
				break;
			case 'u':
				flags |= TFMT_UF;
				break;
			case 'r':
				flags |= TFMT_RFC822;
				break;
			case 'i':
				flags |= TFMT_ISO8601;
				break;
			case 'g':
				flags |= TFMT_OLD822GMT;
				break;
			case 'f':
				flags = atoi(optarg);
				break;
			default:
				usage(*av);
		}
		ac -= optind;

		if(ac != 1)
			usage(*av);

		av += optind;

		for(c = 0; c < ac; c++)
			printf("%s\n", timetostr(atol(av[c]), flags));

	} else {
		/* timetostr mode */

		svect *sl;
		time_t tloc;
		int i;

		if(ac < 2)
			usage(*av);

		sl = sinit();

		for(i = 1; i < ac; i++)
			sadd(sl, av[i]);

		tloc = strtotime(sjoin(sl, " "));
		printf("%ld\n", tloc);
		sfree(sl);
	}

	return 0;
}

void usage(char *name) {
	if(timetostr_mode) {
		fprintf(stderr,
		"Usage: %s [-l] [-u] [-g] [-r] [-i] [-f <flags>] <unixtime>\n",
		name);
	} else {
		fprintf(stderr,
		"Usage: %s <date_or_time_string>\n",
		name);
	}
	exit(EX_USAGE);
}

