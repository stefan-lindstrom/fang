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
 * $Id: cfglex.c,v 1.11 2002/11/30 10:56:34 vlm Exp $
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include "strfunc.h"

#include "cfg.h"

extern char	*_sf_cfg_i_cf;
extern char	*_sf_cfg_i_cfpos;
extern off_t	_sf_cfg_i_cfsize;
extern int	_sf_cfg_i_nline;

int _sf_cfg_i_brcount = 0;

#define CHECKRANGE	\
	while(w-buf >= buflen) {	\
		int z = w - buf;	\
		buflen = buflen << 1;	\
		buf = (char *)sf_realloc(buf, buflen);	\
		if(buf == NULL)	\
			abort();	\
		w = buf + z;	\
	}

static size_t
_cfg_countok(char *);

int sfcfglex() {
	int n;
	char *p;

	if(_sf_cfg_i_cfpos - _sf_cfg_i_cf > _sf_cfg_i_cfsize)
		return 0;
	switch(*_sf_cfg_i_cfpos) {
		case '\n':
			_sf_cfg_i_nline++;
			break;
		case '\r':
			break;
		case '#':
			_sf_cfg_i_cfpos=strchr(_sf_cfg_i_cfpos, '\n');
			if(!_sf_cfg_i_cfpos) return 0; // End of file reached
			_sf_cfg_i_nline++;
			break;
		case '/':
			switch(_sf_cfg_i_cfpos[1]) {
			case '/':
				if(!(_sf_cfg_i_cfpos=
					strchr(_sf_cfg_i_cfpos, '\n')))
					return 0;
				_sf_cfg_i_nline++;
				break;
			case '*':
			    {
				char *p = _sf_cfg_i_cfpos;
				_sf_cfg_i_cfpos = strstr(p, "*/");
				if(_sf_cfg_i_cfpos == NULL)
					return 0;
				*_sf_cfg_i_cfpos = '\0';
				_sf_cfg_i_cfpos += 2;
				_sf_cfg_i_nline += countchar(p, '\n');
			    }
			}
			break;
		case '{': _sf_cfg_i_cfpos++; _sf_cfg_i_brcount++; return LB;
		case '}': _sf_cfg_i_cfpos++; _sf_cfg_i_brcount--; return RB;
		case '=': _sf_cfg_i_cfpos++; return EQ;
		case ',': _sf_cfg_i_cfpos++; return COMMA;
		case ':': _sf_cfg_i_cfpos++; return COLON;
		case ';': _sf_cfg_i_cfpos++; return SEMICOLON;
		case '*':
			_sf_cfg_i_cfpos++;
			sfcfglval.tv_char=sf_strdup("*");
			return TOK_STRING;
		case '\'':
		case '"':
			{

/* === */

			char *buf = (char *)sf_malloc(32);
			int  buflen = 32;
            char *w = buf;
            int insng = 0;
            int indbl = 0;

            p=_sf_cfg_i_cfpos;
			if(!buf) {
				fprintf(stderr, "Memory allocation error in cfglex.o\n");
				abort();
			}

            while(1) {
            switch(*p) {
                case '\0':
                    if(w-buf) {
                        _sf_cfg_i_cfpos=p;
                        *w = '\0';
			sfcfglval.tv_char=buf;
			return TOK_STRING;
                    }
                    return 0;
                case '\n':
                    _sf_cfg_i_nline++;
                case ' ':
				case '\r':
                case '\t':
                    if(insng || indbl) {
			*w++ = *p;
		    } else if(w-buf) {
                        *w = '\0';
			sfcfglval.tv_char=buf;
			return TOK_STRING;
                    }
                    break;
                case '\'':
                    if(indbl)
			*w++ = *p;
                    else
			insng = insng?0:1;
                    if(!insng && !indbl) {
                        _sf_cfg_i_cfpos = p + 1;
                        *w = '\0';
			sfcfglval.tv_char=buf;
			return TOK_STRING;
                    }
                    break;
                case '"':
                    if(insng) *w++ = *p;
                    else
			indbl=indbl?0:1;
                    if(!insng && !indbl) {
                        _sf_cfg_i_cfpos = p + 1;
                        *w = '\0';
			sfcfglval.tv_char=buf;
			return TOK_STRING;
                    }
                    break;
                case '\\':
                    if(insng) {
			*w++ = *p;
			break;
		    }
                    p++;
		    if(*p == '\r' && p[1] == '\n')
			p++;
                    switch(*p) {
                        case '\n': _sf_cfg_i_nline++; break;
                        case 'a': *w++='\a'; break;
                        case 'b': if(w > buf) w--; break;
                        case 'e': *w++='\e'; break;
                        case 'f': *w++='\f'; break;
                        case 'n': *w++='\n'; break;
                        case 'r': *w++='\r'; break;
                        case 't': *w++='\t'; break;
                        case 'v': *w++='\v'; break;
                        case '\\': *w++='\\'; break;
                        default:
                            *w++ = *p;
                    }
                    break;
                default:
                    *w++ = *p;
            }
                CHECKRANGE;
                p++;
            }

/* === */

			}
	}

	if((n=_cfg_countok(_sf_cfg_i_cfpos)) > 0) {
		p=strndup(_sf_cfg_i_cfpos, n);
		_sf_cfg_i_cfpos+=n;

		sfcfglval.tv_char=p;
		return TOK_STRING;
	}

	_sf_cfg_i_cfpos++;
	return sfcfglex();
}


static size_t
_cfg_countok(char *s) {
	register char *p;

	for(p=s; *p; p++) {
		switch(*p) {
			case '/':
			case '-':
			case '_':
			case '!':
			case '@':
				continue;
		}
		if(*p >= 'a' && *p <= 'z')
			continue;
		if(*p >= 'A' && *p <= 'Z')
			continue;
		if(*p >= '0' && *p <= '9')
			continue;
		break;
	}

	return p - s;
}



