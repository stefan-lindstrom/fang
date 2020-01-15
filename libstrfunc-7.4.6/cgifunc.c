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
 * $Id: cgifunc.c,v 1.32 2003/01/13 02:12:51 vlm Exp $
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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

#include <errno.h>

#include "strfunc.h"
#include "b64.h"

char *_sf_urld_buf = NULL;
ssize_t _sf_urld_buflen = 0;

extern int _sf_add_internal(svect *, void *msg, size_t len);

char *
url_decode(char *str) {
	char *buf;
	char *p;
	int n;
	unsigned char c;

	n = (str ? strlen(str) : 0) + 1;
	if(n < _sf_urld_buflen)
		n = _sf_urld_buflen;

	p = buf = (char *)sf_malloc(n);
	if(p == NULL)
		return NULL;

	/*
	 * If string is NULL, then return empty string.
	 */
	if(str == NULL) {
		if(_sf_urld_buf)
			free(_sf_urld_buf);
		_sf_urld_buflen = n;
		*p = '\0';
		return (_sf_urld_buf = buf);
	}

	for(p; (c = *str); str++, p++) {
		if(c == '%') {
			char a, b;

			if( !(a = str[1]) || !(b = str[2]) ) {
				/* Incomplete, but valid */
				*p = '%';
				continue;
			}

			if(a >= 'a') a -= 32;
			if(b >= 'a') b -= 32;

			if(a >= '0' && a <= '9')
				c = a - '0';
			else if( a >= 'A' && a <= 'F')
				c = 10 + a - 'A';
			else {
				/* Strange, but valid */
				*p = '%';
				continue;
			}

			c *= 16;

			if(b >= '0' && b <= '9')
				c += b - '0';
			else if( b >= 'A' && b <= 'F')
				c += 10 + b - 'A';
			else {
				*p = '%';
				/* Strange, but valid */
				continue;
			}

			str+= 2;

		} else if(c == '+') {
			*p = ' ';
			continue;
		}

		*p = c;
	}


	*p = '\0';

	if(_sf_urld_buf)
		free(_sf_urld_buf);
	_sf_urld_buflen = n;
	return (_sf_urld_buf = buf);
}

char *_sf_urle_buf = NULL;
int _sf_urle_buflen = 0;

char *
url_encode(char *str) {
	char *buf;
	char *p;
	static char *table="0123456789ABCDEF";
	int n;
	unsigned char c;

	n = strlen(str?str:"") + 1;
	if(_sf_urle_buflen > n)
		n = _sf_urle_buflen;

	p = buf = (char *)sf_malloc((n + 2) * 3);
	if(p == NULL)
		/* ENOMEM? */
		return NULL;

	if(str) {
		for(; (c=*str); str++) {
			if(
				(c >= 'A' && c <= 'Z') ||
				(c >= 'a' && c <= 'z') ||
				(c >= '0' && c <= '9') ||
				strchr("@._", *str)
			) {
				*p++ = *str;
				continue;
			}
			if(c == ' ') {
				*p++ = '+';
				continue;
			}
			*p++ = '%';
			*p++ = table[ (unsigned char)(c & 0xF0) >> 4];
			*p++ = table[ (unsigned char)(c & 0x0F) ];
		} /* for */
	} /* if(str) */

	*p = '\0';

	if(_sf_urle_buf)
		free(_sf_urle_buf);
	_sf_urle_buflen = n;
    return (_sf_urle_buf = buf);
}

svect *_sf_attr = NULL;
svect *_sf_vals = NULL;
svect *_sf_unmv = NULL; /* Unmodified value */
svect *_sf_type = NULL; /* Content-type */

#define	__VECT_INIT(var) do {	\
		if(var) {	\
			sclear(var);	\
		} else {	\
			var = sinit();	\
			if((var) == NULL) {	\
				/* ENOMEM? */	\
				return -1;	\
			}	\
		}	\
	} while(0);

int _sf_cgi_parse_multipart(char *data, size_t len);

int
parse_form() {
	static int parsed = 0;
	int n;
	char *af, *as;
	svect *sl = NULL;
	char *s;

	if(parsed)
		return 0;

	__VECT_INIT(_sf_attr)
	__VECT_INIT(_sf_vals);
	__VECT_INIT(_sf_unmv);
	__VECT_INIT(_sf_type);

	if(!(s=getenv("REQUEST_METHOD")) ||
		(strcmp(s, "GET") && strcmp(s, "HEAD") && strcmp(s, "POST"))) {
			/* Invalid input */
			errno = EINVAL;
			return -1;
	}

	if(strcmp(s, "POST")) {
		if(!(s=getenv("QUERY_STRING"))) {
			errno = EINVAL;
			return -1;
		}
		sl = split(s, "&", 0);
		if(sl == NULL) {
			/* ENOMEM? */
			return -1;
		}
	} else {
		char *buf;
		ssize_t buflen, bl, tmp;
		int known_content_length;

		if(!(s=getenv("CONTENT_LENGTH")) || (buflen=atoi(s)) < 0) {
			buflen = 4091; /* Probably, one page */
			known_content_length = 0;
		} else {
			known_content_length = 1;
		}

		buf=(char *)sf_malloc(buflen+1);
		if(buf == NULL)
			/* ENOMEM? */
			return -1;

		/* Read into previoulsy allocated buffer */
		for(bl = 0; bl < buflen; ) {
			tmp = read(0, buf+bl, buflen - bl);

			if(tmp == -1) {
				if(errno == EINTR)
					continue;
				free(buf);
				/* read() will return value */
				return -1;
			}

			if(tmp == 0) {
				if(known_content_length)
					return -1;
				break;
			}

			bl+=tmp;
		}

		/* Terminating with zero */
		buf[buflen=bl] = '\0';

		/* Parse differently, if it is multipart form. */
		if((s=getenv("CONTENT_TYPE")) &&
			!strncasecmp(s, "multipart/form-data",
				sizeof("multipart/form-data")-1)) {
			tmp = _sf_cgi_parse_multipart(buf, buflen);
			free(buf);
			/* Invalid data supplied */
			errno = EINVAL;
			return tmp;
		}

		sl = split(buf, "&", 0);
		free(buf);
	}


	if(sl->count == 0) {
		sfree(sl);
		return 0;
	}

	for(n = 0; n < sl->count; n++) {
		af = sl->list[n];
		if((as=strchr(af, '=')))
			*as++ = '\0';
		if(sadd(_sf_attr, url_decode(af)) == -1
		  || sadd(_sf_unmv, as?as:"") == -1	/* Unmodified value */
		  || sadd(_sf_vals, url_decode(as)) == -1	/* Decoded value */
		  || sadd(_sf_type, "text/unknown") == -1
		) {
			sfree(sl);
			/* ENOMEM? */
			return -1;
		}
	}

	sfree(sl);

	parsed = 1;
	return 0;
}

char *
param(char *field) {
	if(_sf_attr == NULL) {
		if(parse_form() == -1)
			return NULL;
	}

	return scget2(_sf_attr, field, _sf_vals);
}

svect *
param2(char *field, int flag) {
	static svect *sl = NULL;
	svect *vals;
	int n;

	if(_sf_attr == NULL) {
		if(parse_form() == -1)
			return NULL;
	}

	if(sl == NULL) {
		sl = sinit();
		if(sl == NULL)
			return NULL;
	} else {
		sl->count = 0;
		sl->maxlen = 0;
		sl->list[0] = NULL;
		sl->lens[0] = 0;
	}

	if(field == NULL)
		return sl;

	switch(flag) {
		case 0:
			vals = _sf_vals;
			break;
		case 1:
			vals = _sf_unmv;
			break;
		case 2:
			vals = _sf_type;
			break;
		default:
			vals = _sf_vals;
	}

	for(n = 0; n < _sf_attr->count; n++)
		if(ecq(_sf_attr->list[n], field))
			_sf_add_internal(sl, vals->list[n], vals->lens[n]);

	return sl;
}

/*
 * Returns an array of passed CGI attributes.
 */
svect *
params() {

	if(_sf_attr == NULL) {
		if(parse_form() == -1)
			return NULL;
	}

	return _sf_attr;
}

/*
 * html_quote storage.
 */
static char *_sf_hq_buf = NULL;

char *
html_quote_detach() {
	char *buf = _sf_hq_buf;
	_sf_hq_buf = NULL;
	return buf;
}

char *
html_quote(char *s) {
	char *buf;
	char *p;
	int n;
	int nc;
	char sc;

	if(s == NULL)
		s = "";

	nc = countchar2(s, "\"&<>");
	n = strlen(s) + sizeof("&quot;") * nc + 1;

	buf = (char *)sf_malloc(n);
	if(buf == NULL)
		/* ENOMEM? */
		return NULL;

	if(nc == 0) {
		memcpy(buf, s, n);

		if(_sf_hq_buf)
			free(_sf_hq_buf);

		return (_sf_hq_buf = buf);
	}

	for(p = buf; (sc=*s); s++) {
		switch(sc) {
			case '"':
				strcpy(p, "&quot;"); p+=6; break;
			case '<':
				strcpy(p, "&lt;"); p+=4; break;
			case '>':
				strcpy(p, "&gt;"); p+=4; break;
			case '&':
				strcpy(p, "&amp;"); p+=5; break;
			default:
				*p++ = sc; continue;
			}
	}
	*p = 0;

	if(_sf_hq_buf)
		free(_sf_hq_buf);

	return (_sf_hq_buf = buf);
}

char *_sfqpbuf = NULL;
int _sf_in_mime_words;

char *_sf_b64_buf = NULL;
size_t _sf_b64_len = 0;

void *
base64_detach(size_t *size) {
	void *buf = _sf_b64_buf;
	_sf_b64_buf = NULL;
	if(size)
		*size = _sf_b64_len;
	return (void *)buf;
}

char *
quoted_printable_decode(char *p) {
	char *buf;
	char *s;
	register int n;

	if(p == NULL) {
		/*
		 * Return empty string.
		 */
		if(_sfqpbuf)
			free(_sfqpbuf);
		_sfqpbuf=sf_strdup("");
		return _sfqpbuf;
	}

	/*
	 * Allocate sufficient space to hold decoded string.
	 */
	s=buf=(char *)sf_malloc(strlen(p) + 1);
	if(buf == NULL)
		/* ENOMEM? */
		return NULL;

	for(; *p; p++) {

		if(_sf_in_mime_words && (*p == '_')) {
			*s++ = ' ';
			continue;
		}

		if(*p == '=') {
			*s = '\0';
			n = *++p; 
			if(n == '\0') {
				p--;
				break;
			}

			if(p[1] == '\0')
				break;

			if( n >= '0' && n <= '9') {
				*s = n-'0';
			} else if( (n >= 'A' && n <= 'F')
				|| (n >= 'a' && n <= 'f') ) {
				*s=10+n-((n>='a')?'a':'A');
			} else {
				if(n == '\n' || n == '\r')
					continue;
				*s++ = '=';
				*s++ = n;
				continue;
			}

			*s *= 16;
			n = *++p;

			if( n >= '0' && n <= '9') {
				*s += n-'0';
			} else if( (n >= 'A' && n <= 'F')
					|| (n >= 'a' && n <= 'f') ) {
				*s += 10+n-((n>='a')?'a':'A');
			} else {
				if(n == '\n' || n == '\r') {
					*s++ = n;
					continue;
				}
				*s = ' ';
				continue;
			}

			s++;
			continue;
		}
		*s++ = *p;
	}
	*s = '\0';

	if(_sfqpbuf)
		free(_sfqpbuf);
	return (_sfqpbuf=buf);
}


char *
_sf_handler(svect *sl) {
	char *p;

	if(sl->count < 4) {
		sclear(sl);
		return NULL;
	}

	p = sl->list[3];
	if(ecq(sl->list[2], "Q")) {
		_sf_in_mime_words++;
		p = quoted_printable_decode(p);
		_sf_in_mime_words--;
	} else if(ecq(sl->list[2], "B")) {
		p = (char *)base64_decode(p, NULL);
	}

	if(ecq(sl->list[1], "windows-1251")) {
		p = windows_1251_to_koi8_r(p);
	} else if(ecq(sl->list[1], "iso-8859-5")) {
		p = iso_8859_5_to_koi8_r(p);
	} else if(ecq(sl->list[1], "cp-866")) {
		p = cp_866_to_koi8_r(p);
	}

	sclear(sl);

	return p;
}

char *
mime_word_decode(char *str) {
#ifndef	HAVE_REGCOMP
#warning	"mime_word_decode() will not function without regex."
	errno = EINVAL;
	return NULL;
#else
	static sed_t *se;
	static sbuf *sbs;
	sbuf *sb;

	if(se == NULL) {
		se = sed_compile(
			"s/=\\?([a-z0-9._-]+)\\?(.)\\?"
			"([^[:space:]\n\r\t?]+)"
			"\\?=[ \n\r\t]*/\\@/gei");
		if(se == NULL) {
			errno = EINVAL;
			return NULL;
		}
		sed_handler(se, _sf_handler);
	}

	if(sbs == NULL) {
		sbs = sbuf_init();
		if(sbs == NULL)
			/* ENOMEM? */
			return NULL;
	}

	/* Dynamic allocation */

	sb = sbuf_init();
	if(sb == NULL)
		/* ENOMEM? */
		return NULL;

	if(str == NULL) {
		if(sbuf_add(sb, "") == -1) {
			sbuf_free(sb);
			/* ENOMEM? */
			return NULL;
		}
		goto mwde;
	}

	if(countchar2(str, "=?") < 4) {
		sbuf_add(sb, str);
		goto mwde;
	}


	if(sbuf_add(sb, sed_exec(se, str)) == -1) {
		sbuf_free(sb);
		/* ENOMEM? */
		return NULL;
	}

mwde:
	if(sbs)
		sbuf_free(sbs);
	sbs = sb;
	return sbs->buf;

#endif	/* HAVE_REGCOMP */
}

static unsigned char _sf_uc_ib[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/==";

char *
base64_encode(void *data, size_t size) {
	char *output;
	char *ou;
	unsigned char *p=(unsigned char *)data;
	void *dte=data + size;
	int nc = 0;

	if(data == NULL) {
		errno = EINVAL;
		return NULL;
	}

	ou=output=(char *)sf_malloc( (4 * size / 3) + (size / 50) + 1);
	if(output == NULL) {
		/* ENOMEM? */
		return NULL;
	}

	while((char *)dte - (char *)p >= 3) {
		*ou = _sf_uc_ib[ *p >> 2 ];
		ou[1] = _sf_uc_ib[ ((*p & 0x03) << 4) | (p[1] >> 4) ];
		ou[2] = _sf_uc_ib[ ((p[1] & 0x0F) << 2) | (p[2] >> 6) ];
		ou[3] = _sf_uc_ib[ p[2] & 0x3F ];

		p+=3;
		ou+=4;

		nc+=4;
		if((nc % 76) == 0) *ou++='\n';
	}
	if((char *)dte - (char *)p == 2) {
		*ou++ = _sf_uc_ib[ *p >> 2 ];
		*ou++ = _sf_uc_ib[ ((*p & 0x03) << 4) | (p[1] >> 4) ];
		*ou++ = _sf_uc_ib[ ((p[1] & 0x0F) << 2) ];
		*ou++ = '=';
	} else if((char *)dte - (char *)p == 1) {
		*ou++ = _sf_uc_ib[ *p >> 2 ];
		*ou++ = _sf_uc_ib[ ((*p & 0x03) << 4) ];
		*ou++ = '=';
		*ou++ = '=';
	}

	*ou = '\0';

	_sf_b64_len = (ou - output);

	if(_sf_b64_buf)
		free(_sf_b64_buf);
	_sf_b64_buf=output;

	return output;
}

void *
base64_decode(char *str, size_t *size) {
	void *output, *ou;

	unsigned int pr[6];
	int n;
	int doit;

	if(size)
		*size = 0;
	if(str == NULL)
		str = "";

	ou = output = (void *)sf_malloc(strlen(str) + 1);
	if(output == NULL)
		/* ENOMEM? */
		return NULL;

	doit = 1;

	do {
		n = 0;
		while(n < 4) {
			unsigned char ch;
			ch = _sf_uc_bi[*(unsigned char *)str];
			if(ch < B64_UNUSED) {
				pr[n] = ch;
				n++;
				str++;
			} else if(ch == B64_UNUSED) {
				str++;
			} else {
				doit = 0;
				break;
			}
		}

		if(doit == 0 && n < 4) {
			pr[n+2] = pr[n+1] = pr[n]='\0';
		}

		*(char *)ou = (pr[0] << 2) | (pr[1] >> 4);
		*(char *)(ou+1) = (pr[1] << 4) | (pr[2] >> 2);
		*(char *)(ou+2) = (pr[2] << 6) | pr[3];

		ou += (n * 3) >> 2;
		
	} while(doit);

	*(char *)ou = '\0';	/* NUL-terminate */

	_sf_b64_len = (ou - output);

	if(size)
		*size = _sf_b64_len;

	/*
	 * Replace the old buffer.
	 */
	if(_sf_b64_buf)
		free(_sf_b64_buf);
	return (_sf_b64_buf=output);
}

int
_sf_cgi_parse_multipart(char *data, size_t len) {
	char *bnd;
	int bndlen;
	char *dp = data;
	char *sd = NULL;	/* start data */
	char *fname = NULL;
	char *name = NULL;
	char *ct = NULL;
	char *tmp;
	svect *sl1, *sl2;
	int i, z;

	if(dp == NULL)
		return 0;
	if((bnd=getenv("CONTENT_TYPE")) == NULL)
		return 0;
	if((bnd=strstr(bnd, "boundary=")) == NULL)
		return 0;
	bnd+=sizeof("boundary")-2;

	bnd=sf_strdup(bnd);
	if(bnd == NULL)
		return 0;
	*bnd = bnd[1] = '-';
	bndlen = strlen(bnd);

	sl1 = sinit();
	if(sl1 == NULL)
		return -1;
	sl2 = sinit();
	if(sl2 == NULL)
		return -1;

	while(dp-data < len) {
		if(strncmp(dp, bnd, bndlen)) { dp++; continue; }

		*(dp-1) = *(dp-2) = '\0';

		if(sd) {
			if(	sadd(_sf_attr, name?name:"UNKNOWN") == -1
				|| sadd(_sf_type, ct?ct:"") == -1
			) {
				sfree(sl1);
				sfree(sl2);
				return -1;
			}
			if(fname) {
				if(	sadd(_sf_vals, fname) == -1
					|| sadd2(_sf_unmv, sd, dp-sd-2) == -1
				) {
					sfree(sl1);
					sfree(sl2);
					return -1;
				}
			} else {
				if( sadd2(_sf_vals, sd, dp-sd-2) == -1
					|| sadd2(_sf_unmv, sd, dp-sd-2) == -1
				) {
					sfree(sl1);
					sfree(sl2);
					return -1;
				}
			}
			fname=name=sd=ct = NULL;
		}

		dp+=bndlen;
		if(strncmp(dp, "--\r\n", 4) == 0)
			break;
		dp+=2;

		/* Part started */

		tmp = strstr(dp, "\r\n\r\n");
		tmp += 2;
		*tmp = '\0';
		sd = tmp + 2;

		for(tmp = dp; *tmp; tmp++) {
			if(*tmp == ';')
				*tmp = ' ';
		}

		sclear(sl1);
		splitf(sl1, dp, "\r\n", 0);
		if(sl1->count == 0)
			continue;

		for(i = 0; i<sl1->count; i++) {
			if(!strncasecmp(sl1->list[i], "Content-Disposition:",
				sizeof("Content-Disposition:")-1)) {
				sclear(sl2);
				splitquotable(sl2, sl1->list[i]);
				for(z = 0; z < sl2->count; z++) {
					if(!strncasecmp(sl2->list[z], "name=", 5))
						name=sl2->list[z]+5;
					else
					if(!strncasecmp(sl2->list[z], "filename=", 9))
						fname=sl2->list[z]+9;
				}
			} else if(!strncasecmp(sl1->list[i], "Content-Type:",
				sizeof("Content-Type:")-1)) {
				ct=sl1->list[i] + sizeof("Content-Type:") - 1;
				while(*ct == ' ') ct++;
			}
		}

	}

	sfree(sl1);
	sfree(sl2);

	return 1;
}

int
set_cookie(char *name, char *value, char *optDomain, char *optPath, time_t optMaxAge, char *optComment, int optSecure) {

	if(value == NULL || name == NULL || *name == '\0') {
		errno = EINVAL;
		return -1;
	}

	printf("Set-Cookie: %s=\"%s\"", name, url_encode(value));
	if(optDomain)
		printf("; Domain=%s", optDomain);
	if(optPath)
		printf("; path=\"%s\"", optPath);
	if(optMaxAge >= 0)
		printf("; Max-Age=%ld", optMaxAge);
	if(optComment)
		printf("; Comment=\"%s\"", html_quote(optComment));
	if(optSecure)
		printf("; Secure");
	printf("\n");

	return 0;
}

svect *sf_cgi_csla = NULL;
svect *sf_cgi_cslv = NULL;

svect *
cookies() {
	if(sf_cgi_csla == NULL)
		cookie("");
	return sf_cgi_csla;
}

char *
cookie(char *name) {

	if(name == NULL) {
		errno = EINVAL;
		return NULL;
	}

	if(sf_cgi_csla == NULL) {
		char *p, *s, *pt;

		pt=getenv("HTTP_COOKIE");
		if(pt == NULL || *pt == '\0') {
			/* No cookies */
			errno = ESRCH;
			return NULL;
		}
	
		p = (char *)alloca(strlen(pt) + 1);
		if(p == NULL) {
			errno = EINVAL;
			return NULL;
		}
	
		strcpy(p, pt);
	
		sf_cgi_csla = sinit();
		if(sf_cgi_csla == NULL)
			/* ENOMEM? */
			return NULL;
	
		sf_cgi_cslv = sinit();
		if(sf_cgi_cslv == NULL)
			/* ENOMEM? */
			return NULL;
	
		for(s=p; *p; p++) {
			if(*p == '=') {
				*p++='\0';
				if(sadd(sf_cgi_csla, s) == -1) {
					sfree(sf_cgi_csla);
					sfree(sf_cgi_cslv);
					sf_cgi_csla = NULL;
					sf_cgi_cslv = NULL;
					/* ENOMEM? */
					return NULL;
				}
				s=p;
				if(*s == '"')
					p=strchr(++s, '"');
				else
					p=strchr(s, ';');
				if(p == NULL)
					p=strrchr(s, '\0');
				*p++ = 0;
				if( sadd(sf_cgi_cslv, url_decode(s)) == -1 ) {
					sfree(sf_cgi_csla);
					sfree(sf_cgi_cslv);
					sf_cgi_csla = NULL;
					sf_cgi_cslv = NULL;
					/* ENOMEM? */
					return NULL;
				}
				if(*p == '"') p++;
				if(*p == ';') p++;
				while(*p == ' ') p++;
				s=p;
			
			}
		}
	}
	
	if(sf_cgi_csla->count == 0) {
		errno = ESRCH;
		return NULL;
	}

	return scget2(sf_cgi_csla, name, sf_cgi_cslv);
}


char *
limittextwidth(char *txt, size_t width) {
	register int n;
	register char *c, *tc;

	if(txt == NULL) {
		errno = EINVAL;
		return NULL;
	}
	if(*txt == '\0' || (ssize_t)width < 1)
		return txt;

	n = 0;
	c = txt;

	while(*++c) {
		if(*c == '\n') {
			n = 0;
			continue;
		}
		if(++n <= width)
			continue;

		tc=c;
		while((c > txt) && (*c != '\n')) {
			if(*--c == ' ') {
				*c = '\n';
				tc = NULL;
				n = 0;
				break;
			}
		}

		if(tc == NULL) {
			n = 0;
			continue;
		}

		while(*++c && *c != ' ');
		n = 0;
		if(*c)
			*c = '\n';
		else
			return txt;
	}

	return txt;
}


svect *
getlanguageprefs() {
	static svect *sl = NULL;
	char *p;
	int i;
	register char *t;

	if(sl)	/* Already parsed */
		return sl;

	p = getenv("HTTP_ACCEPT_LANGUAGE");
	if(p == NULL) {
		errno = ESRCH;
		return NULL;
	}

	sl = split(p, ", ", 0);
	if(sl == NULL)
		/* ENOMEM? */
		return NULL;

	for(i = 0; i < sl->count; i++) {
		p = strchr(sl->list[i], ';');
		if(p) {
			*p = '\0';
			if((p - sl->list[i]) == 0) {
				sdel(sl, i--);
				continue;
			}
		}

		for(t = sl->list[i]; *t; t++) {
			if(*t == '-')
				continue;
			if(*t >= 'a' && *t <= 'z')
				continue;
			if(*t >= '0' && *t <= '9')
				continue;
			if(*t >= 'A' && *t <= 'Z')
				continue;
			break;
		}
		if(*t)	/* Not reached the end */
			sdel(sl, i--);
	}

	if(sl->count == 0) {
		sfree(sl);
		sl = NULL;
	}

	return sl;
}


