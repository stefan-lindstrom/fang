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
 * $Id: sed-int.h,v 1.3 2002/02/15 03:19:11 vlm Exp $
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

#ifndef	HAVE_REGEX_LIB
#  error "No regex library found, sf_sed(3) functions unavailable"
#else
# ifdef	HAVE_REGEX_H
#  include <regex.h>
# elif	defined(HAVE_RXPOSIX_H)
#  include <rxposix.h>
# elif	defined(HAVE_PCREPOSIX_H)
#  include <pcreposix.h>
# else
   /* eh? how did we get here?? */
#  error "Please, please report this situation to vlm@spelio.net.ru!"
#  error "This iz due to my lazyness in checking the unusual states..."
# endif /* regexp variants */
#endif /* HAVE_REGEX_LIB */


#include "strfunc.h"

#define	SF_OPT_GLOBAL	1	/* Global matching */
#define	SF_OPT_RESULTS	2	/* Store results */
#define	SF_OPT_NOZERO	4	/* Disclude zero argument from results */
#define	SF_OPT_BDRRES	8	/* Border results */

typedef struct {
	int type_of_operation;	/* 1 - s///, 2 - y///, 3 - // */
	int rexopt;
	int opt;	/* 2 - remember results, 1 - global matching, 0 - off. */
	regex_t *re;
	int nmatch;
	regmatch_t *pmatch;

	svect *to;
	sbuf  *sb;

	char *sfr;
	size_t sfr_len;

	char *sto;
	size_t sto_len;

	char *(*ResHandler)(svect *);
	svect *Results;

	svect *BorderResults;
} sed_i;
