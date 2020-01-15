/*-
 * Copyright (c) 1999, 2000, 2001, 2002 Lev Walkin <vlm@spelio.net.ru>.
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
 * $Id: param.c,v 1.24 2003/01/13 00:31:04 vlm Exp $
 */

#include <errno.h>
#include "strfunc.h"

#include "param.h"

static void
_sf_memory_failure_callback() {
	static char z = 0;
	if(z++) return;

	fprintf(stderr,
		"STRFUNC: Memory allocation error. "
		"Possible, more will follow. "
		"Suppressing further messages.\n");
}

int	_sf_param_mfb = 0;
void	(*_sf_param_mfb_cb)(void) = _sf_memory_failure_callback;
int	_sf_param_mf_tries = 10;
int	_sf_param_stt_zcache = 0;
/* Reserved for future use
int	_sf_param_split_regex_learn = 1;
*/

int
strfunc_ctl(int rq, int *arg) {

	if(arg == NULL) {
		errno = EINVAL;
		return -1;
	}

	switch(rq) {

		case SF_GET_MEMORY_FAILURE_BEHAVIOR:
			*arg = _sf_param_mfb;
			return 0;

		case SF_SET_MEMORY_FAILURE_BEHAVIOR:
			if(*arg < SF_ARG_MFB_ABORT
			    || *arg > SF_ARG_MFB_TRY_NOFAIL) {
				errno = EINVAL;
				return -1;
			}
			_sf_param_mfb = *arg;
			return 0;

		case SF_GET_MEMORY_FAILURE_TRIES:
			*arg = _sf_param_mf_tries;
			return 0;

		case SF_SET_MEMORY_FAILURE_TRIES:
			if(*arg < 1) {
				errno = EINVAL;
				return -1;
			}
			_sf_param_mf_tries = *arg;
			return 0;

		case SF_GET_STRTOTIME_ZINFO_CACHING:
			*arg = _sf_param_stt_zcache;
			return 0;

		case SF_SET_STRTOTIME_ZINFO_CACHING:
			_sf_param_stt_zcache = *arg;
			return 0;

		default:
			errno = EINVAL;
			return -1;
	}

	errno = EINVAL;
	return -1;
}

void
strfunc_memory_failure_callback(void (*cb)(void)) {
	_sf_param_mfb_cb = cb;
}


/* Return run-time version of the library */
int
strfunc_version() {
	return 746;
}

