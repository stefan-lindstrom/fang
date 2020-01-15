/*-
 * Copyright (c) 1999, 2000, 2001, 2002 Lev Walkin <vlm@spelio.net.ru>.
 * Portions copyright (c) 1995 Jef Poskanzer <jef@acme.com>.
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
 * $Id: datetime.c,v 1.19 2002/11/30 10:24:03 vlm Exp $
 */

#ifndef	__EXTENSIONS__
#define	__EXTENSIONS__
#endif

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

#ifdef	HAVE_TIME_H
#include <time.h>
#endif

#ifdef	HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#ifdef	HAVE_ERRNO_H
#include <errno.h>
#else
#ifndef	EINVAL
#define	EINVAL	1
#endif
#endif

#include "param.h"

#ifndef	HAVE_SNPRINTF
#ifdef	HAVE__SNPRINTF
#define	snprintf	_snprintf
#else
#error	"This system has neither snprintf() nor _snprintf()"
#endif
#endif

#include "strfunc.h"

#ifndef	HAVE_TM_ZONE
#ifndef	HAVE_TZNAME
#warning "Assume, there is char *tzname[2]"
#define	HAVE_TZNAME
#warning "Please, please report this situation to vlm@spelio.net.ru!"
#warning "This iz due to my lazyness in checking the unusual states..."
#endif
#endif


#ifndef	HAVE_LOCALTIME_R
/*
 * Emulate localtime_r() using localtime().
 */
static struct tm *
_sf_localtime_r(const time_t *clock, struct tm *result) {
	struct tm *tmp;
	tmp = localtime(clock);
	if(tmp)	/* Assume result is not NULL */
		memcpy(result, tmp, sizeof(struct tm));
	return result;
}
#define	localtime_r	_sf_localtime_r
#endif

#ifndef	HAVE_GMTIME_R
/*
 * Emulate gmtime_r() using gmtime().
 * Used in timetostr().
 */
struct tm *
_sf_gmtime_r(const time_t *clock, struct tm *result) {
	struct tm *tmp;
	tmp = gmtime(clock);
	if(tmp) /* Assume result is not NULL */
		memcpy(result, tmp, sizeof(struct tm));
	return result;
}
#define	gmtime_r	_sf_gmtime_r
#endif


#define	NO_VALUE	0x7fffffff
struct _sf_default_values {
	int year;
	int mon;
	int mday;
	int cur_off;
};
static struct _sf_default_values _sf_def = { NO_VALUE };


static int _sf_chartype_table[256] = {
	0, 0, 0, 0, 0, 0, 0, 0,	/* 07 */
	0, 1, 1, 1, 0, 1, 0, 0,	/* 15 */
	0, 0, 0, 0, 0, 0, 0, 0,	/* 23 */
	0, 0, 0, 0, 0, 0, 0, 0,	/* 31 */
	1, 0, 0, 0, 0, 0, 0, 0,	/* 39 */
	0, 0, 0, 0, 0, 0, 0, 0,	/* 47 */
	2, 2, 2, 2, 2, 2, 2, 2,	/* 55 */
	2, 2, 0, 0, 0, 0, 0, 0,	/* 63 */
};
#define	_sf_isspace(foo)	(_sf_chartype_table[(unsigned)(foo)] & 1)
#define	_sf_isdigit(foo)	(_sf_chartype_table[(unsigned)(foo)] & 2)

struct strint {
	char *s;
	int l;
};


inline int
_sf_monthcmp(char *ap, char *bp) {
	register unsigned int a, b, n;

	for(n = 0; (n < 3) && (a = (unsigned)*ap); n++, ap++, bp++) {
		if(a < 'Z') a += 'a' - 'A';
		b = a - (unsigned)*bp;
		if(b)
			return b;
	}
	if(n == 3)
		return 0;

	return -1;
	
}

static int
_sf_strint_search(char *str, struct strint *tab, int tabsize) {
	int i, h, l, r;
	l = 0;
	h = tabsize - 1;
	for(;;) {
		i = (h + l) / 2;
		r = _sf_monthcmp(str, tab[i].s);
		if(r < 0)
			h = i - 1;
		else if(r > 0)
			l = i + 1;
		else
			return tab[i].l;
		if(h < l)
			return -1;
	}
}

inline int
_sf_recognize_month(char *dts) {
	static struct strint mon_tab[] = {
		{ "apr", 3 }, { "aug", 7 },
		{ "dec", 11 },
		{ "feb", 1 },
		{ "jan", 0 }, { "jul", 6 }, { "jun", 5 },
		{ "mar", 2 }, { "may", 4 },
		{ "nov", 10 },
 		{ "oct", 9 },
		{ "sep", 8 },
	};

	return _sf_strint_search(dts, mon_tab,
		sizeof(mon_tab) / sizeof(struct strint));
}


/* Water-proof atoi() */
inline int
_sf_natoui(char *str, int num) {
	int n = 0, na;

	while(num-- > 0) {
		na = *str++;
		if(!_sf_isdigit(na))
			return -1;
		n = (n * 10) + (na - '0');
	}

	return n;
}

inline time_t
_sf_recognize_tz(char *dts) {
	time_t tmp = 0;

	if(*dts == '+' || *dts == '-') {
		if((tmp = _sf_natoui(dts+1, 4)) < 0) return -1;
		tmp = (tmp / 100) * 3600 + (tmp % 100) * 60;
		if(*dts == '-')
			tmp = -tmp;
		dts += 5;
		while(*dts == ' ') dts++;
		return tmp;
	} else if(*dts == '(' || (*dts >= 'A' && *dts <= 'Z')) {

		if(*dts == '(') dts++;

		if(!*dts || !dts[1] || !dts[2]
		|| (dts[3] >= 'A' && dts[3] <= 'Z'))
			return 0;

		/* RFC 822 zones */
		if(!strncmp(dts, "GMT", 3))
			tmp = 0;
		else if(!strncmp(dts, "EST", 3))
			tmp = - 5 * 3600;
		else if(!strncmp(dts, "EDT", 3))
			tmp = - 4 * 3600;
		else if(!strncmp(dts, "CST", 3))
			tmp = - 6 * 3600;
		else if(!strncmp(dts, "CDT", 3))
			tmp = - 5 * 3600;
		else if(!strncmp(dts, "MST", 3))
			tmp = - 7 * 3600;
		else if(!strncmp(dts, "MDT", 3))
			tmp = - 6 * 3600;
		else if(!strncmp(dts, "PST", 3))
			tmp = - 8 * 3600;
		else if(!strncmp(dts, "PDT", 3))
			tmp = - 7 * 3600;
		else return -1;

		return tmp;
	}

	return -1;
}


inline long
_sf_recognize_time(char *dts) {
	long l = 0;
	int tmp;

	if(!*dts)
		return -1;

	if((tmp = _sf_natoui(dts, 2)) < 0 || tmp > 24)
		return -1;

	l |= tmp << 16;
	if(dts[2] != ':' && dts[2] != '-') return -1;
	dts+=3;

	if((tmp = _sf_natoui(dts, 2)) < 0 || tmp > 60)
		return -1;

	l |= tmp << 8;
	if(dts[2] != ':' && dts[2] != '-') {
		l |= 5 << 24;
		return -1;
	}
	dts+=3;

	if((tmp = _sf_natoui(dts, 2)) < 0 || tmp > 60)
		return -1;

	l |= tmp;
	l |= 8 << 24;

	return l;
}

/*
 * Ensures year is within correct range.
 */
inline int
_sf_year_range_gen(int year) {
	if(year > 1000) {
		year -= 1900;
	} else {
		/* Dirty hack to convert 80 to 1980. */
		if(year < 38) {
			year += 100;
			return year;
		}
	}

	if(year < 2 || year > 138)
		return 0;

	return year;
}

inline int
_sf_year_range_big(int year) {
	year -= 1900;

	if(year < 2 || year > 138)
		return 0;

	return year;
}

inline int
_sf_year_range_small(int year) {

	/* Dirty hack to convert 80 to 1980. */
	if(year < 38) {
		year += 100;
		if(year < 2)
			return 0;
		else
			return year;
	}

	if(year < 2 || year > 138)
		return 0;

	return year;
}

inline int
_sf_is_year_leap(int year) {
    return (year % 400)
	? ((year % 100)
		? ((year % 4)?0:1)
		: 0 )
	: 1;
}

/* Fast version of mktime() */
static time_t
_sf_mktime(struct tm *tm) {
	static int mdshift[] = { 0, 31, 59, 90, 120,
		151, 181, 212, 243, 273, 304, 334 };
	time_t t;

	t  = (tm->tm_year - 70) * 365
		+ ((tm->tm_year - 69) >> 2)
		+ mdshift[tm->tm_mon];

	/* Leap day for this year. */
	if ( tm->tm_mon >= 2
		&& _sf_is_year_leap( tm->tm_year + 1900 ) )
		++t;

	t = ((((((t + (tm->tm_mday - 1)) * 24)
		+ tm->tm_hour) * 60)
		+ tm->tm_min) * 60)
		+ tm->tm_sec;

	return t - _sf_def.cur_off;
}


/*
 * Convert given time into time_t.
 * dts (date/time string) can be in numerous representations,
 * some of them are mentioned in strtotime(3) manual page.
 */

time_t
strtotime(char *dts) {
	static time_t lcupd;	/* Last year/zone cache update */
	static time_t hour_end_in;
	struct tm tm;
	time_t tmp = 0;
	int form;

	/*
	 * Cache current year, time offset.
	 */
	if(_sf_def.year == NO_VALUE) {
		time(&tmp);
		if(localtime_r(&tmp, &tm) == NULL)
			goto fail;
		_sf_def.year = tm.tm_year;
		_sf_def.mon = tm.tm_mon;
		_sf_def.mday = tm.tm_mday;
		_sf_def.cur_off =
#ifdef	HAVE_TM_ZONE
			tm.tm_gmtoff;
#else	/* HAVE_TM_ZONE */
			- timezone;
#endif	/* HAVE_TM_ZONE */
		hour_end_in = 3600 - (60 * tm.tm_min + tm.tm_sec);
		lcupd = tmp;
	} else if(_sf_param_stt_zcache == 0) {
		time(&tmp);
		if((tmp - lcupd) > hour_end_in) {
			_sf_def.year = NO_VALUE;
			return strtotime(dts);
		}
	}

	if(!dts) goto fail;

	/* That's a good start. */
	while(*dts == ' ')
		dts++;

	/*
	** Forms:
	** form = 0: something like error
	** form = 1: ? RFC 822
	** form = 2: ? ISO 8601
	** form = 3: ? ctime()
	** form = 4: ? syslogd format (Mar 23 18:15:00)
	*/

	form = 0;
	if(*dts >= 'A') {
		form = 1;
	} else if(_sf_isdigit(*dts)) {
		int n = 0;
		while(_sf_isdigit(dts[n])) n++;
		if(n != 8	/* YYYYMMDD */
			&& (!dts[n]
				||
			   (_sf_isspace(dts[n]) && !dts[n+1]))) {
			if((unsigned long)atol(dts) < 0x7fffffff)
				return (time_t)atol(dts);
		}
		form = 2;
	} else {
		/*
		 * Unknown date/time representation,
		 * started neither from digital,
		 * nor from alphabetic character.
		 */
		goto fail;
	}


	/* Initialize temporary tm structure. */
	memset(&tm, 0, sizeof(struct tm));
	tm.tm_isdst = -1;
	tm.tm_year = _sf_def.year;
	tm.tm_mon = _sf_def.mon;
	tm.tm_mday = _sf_def.mday;

if(form == 1) {
	char *savedts=dts;

	while(*dts && *dts != ',' && *dts != ' ')
		dts++;

	if(*dts != ',') {
		/* Maybe, it is just ctime()? */
		dts = savedts;
		form = 3;
		goto f3;
	}

	dts++;
	while(*dts == ' ') dts++;
	if(!*dts) goto fail;

	if(*dts && dts[1] == ' ') {
		tmp = _sf_natoui(dts++, 1);
	} else {
		tmp = _sf_natoui(dts, 2);
		dts+=2;
	}

	if(tmp < 1)
		goto fail;
	else
		tm.tm_mday = tmp;

	while(*dts == ' ' || *dts == '-') dts++;
	if(!*dts) goto fail;

	tmp = _sf_recognize_month(dts);
	if(tmp < 0)
		goto fail;
	else
		tm.tm_mon = tmp;

	while(*dts >= 'A' || *dts == ' ' || *dts == '-') dts++;
	if(!*dts) goto fail;

year:

	if((tmp = _sf_natoui(dts, 4)) == -1) {
		if((tmp = _sf_natoui(dts, 2)) < 0)
			goto fail;
		if((tm.tm_year = _sf_year_range_small(tmp)) == 0)
			goto fail;
		dts+=2;
	} else {
		if((tm.tm_year = _sf_year_range_big(tmp)) == 0)
			goto fail;
		dts+=4;
	}

	while(*dts == ' ') dts++;

	if((tmp = _sf_recognize_time(dts)) == -1)
		goto fail;

	dts += ((unsigned int)tmp) >> 24;
	tm.tm_hour = (tmp >> 16) & 0xff;
	tm.tm_min = (tmp >> 8) & 0xff;
	tm.tm_sec = tmp & 0xff;
	
	while(*dts == ' ') dts++;

	tmp = _sf_recognize_tz(dts);

	goto ex;

} /* NOT (form == 1) */

if(form == 2) {

	/* Year */
	if((tmp = _sf_natoui(dts, 4)) == -1) {
		/* hh:mm... form */
		if(strlen(dts) >= 5) {
			if(dts[2] == ':') {
				/* HH:MM */
				goto tme;
			} else if(dts[2] == '/' || dts[2] == '.') {
				/* DD.MM */
				goto dte;
			} else if(dts[2] == '-' || dts[2] == ' ') {
				if(_sf_isdigit(dts[3]))
					goto dte;
				tm.tm_mday = _sf_natoui(dts, 2);
				tm.tm_mon = _sf_recognize_month(dts + 3);
				if((tm.tm_mday & tm.tm_mon) == -1) goto fail;
				dts += 6;
				if(*dts != '-' && *dts != ' ') goto fail;
				dts++;
				goto year;
			}
		}
		goto fail;
	} else {
		if((tm.tm_year = _sf_year_range_big(tmp)) == 0)
			goto fail;
	}

	dts+=4;
	if(!*dts) goto fail;

	if(*dts == '-' || *dts == '/') dts++;
	else {
		if(*dts == '+' || *dts == 'Z') goto zone;
		if(*dts == 'T') {
			dts++;
			goto tme;
		}
	}

dte:

	/* Month */
	if((tmp = _sf_natoui(dts, 2)) == -1) {
		if(dts[-1] == '-') goto fail;
		if(!_sf_isspace(*dts)) goto fail;
		goto ex;
	} else {
		if((--tmp) < 0)
			goto fail;
		else
			tm.tm_mon = tmp;
	}

	dts+=2;
	if(!*dts) goto ex;

	if(*dts == '-' || *dts == '/' || *dts == '.') {
		dts++;
	} else {
		if(*dts == '+' || *dts == 'Z') goto zone;
		if(*dts == 'T') {
			dts++;
			goto tme;
		}
	}

	/* Month Day */
	if((tmp = _sf_natoui(dts, 2)) == -1) {
		if(dts[-1] == '-') goto fail;
		if(!_sf_isspace(*dts)) goto fail;
		goto ex;
	} else {
		if(!tmp)
			goto fail;
		else
			tm.tm_mday = tmp;
	}

	dts+=2;
	if(dts[-3] == '.' || (dts[-3] == '/' && dts[0] == '/')) {
		int tmp = tm.tm_mday;
		tm.tm_mday = tm.tm_mon + 1;
		tm.tm_mon = tmp - 1;

	}

	if(*dts == '.' || *dts == '/') {
		if((tmp = _sf_natoui(dts + 1, 4)) == -1) {
			if((tmp = _sf_natoui(dts + 1, 2)) == -1)
				goto fail;
			if(_sf_isdigit(dts[3]))
				goto fail;
			if((tm.tm_year = _sf_year_range_small(tmp)) == 0)
				goto fail;
			dts += 3;
		} else {
			if((tm.tm_year = _sf_year_range_big(tmp)) == 0)
				goto fail;
			dts += 5;
		}
	}

lp:
	if(!*dts) {
		tmp = -1;
		goto ex;
	}
	if(*dts == 'T') {
		dts++;
		goto tme;
	}
	if(_sf_isdigit(*dts)) goto tme;
	if(*dts == '+') goto zone;
	if(*dts == 'Z') goto ex;
	if(*dts++ == ' ')
		goto lp;
	goto ex;

tme:

	if((tmp = _sf_natoui(dts, 2)) == -1)
		goto fail;
	else
		tm.tm_hour = tmp;

	dts+=2;

	if(!*dts) goto ex;
	if(*dts == ':') {
		dts++;
	} else {
		if(*dts == '+' || *dts == '-') goto zone;
		if(*dts == 'Z') goto ex;
	}

	if((tmp = _sf_natoui(dts, 2)) == -1) {
		if(dts[-1] == ':') goto fail;
		if(!_sf_isspace(*dts)) goto fail;
		goto ex;
	} else {
		tm.tm_min = tmp;
	}

	dts+=2;

	if(!*dts) goto zone;
	if(*dts == ':') dts++;
	else {
		if(*dts == '+' || *dts == '-') goto zone;
		if(*dts == 'Z') goto ex;
	}

	if((tmp = _sf_natoui(dts, 2)) == -1) {
		if(dts[-1] == ':') goto fail;
		if(!_sf_isspace(*dts)) goto fail;
		goto ex;
	} else {
		tm.tm_sec = tmp;
	}

	dts+=2;

	if(!*dts) {
		/*
		 * Time in this form without the GMT offset
		 * is the LOCAL time
		 */
		tmp = -1;
		goto ex;
	}
	while(*dts == ' ') dts++;

zone:

	if(!*dts) {
		/*
		 * Time in this form without the GMT offset
		 * is the LOCAL time
		 */
		tmp = -1;
		goto ex;
	}

	if(*dts == 'Z') {
		tmp = 0;
		goto ex;
	}
	if(*dts != '+' && *dts != '-') {
		if(_sf_isspace(*dts)) goto ex;
		goto fail;
	}

	tmp = _sf_recognize_tz(dts);

	goto ex;
}

if(form == 3) {
f3:

	if((tmp = _sf_recognize_month(dts)) != -1) {
		tm.tm_mon = tmp;
		form = 4;
		goto f4;
	}

	while(*dts && *dts != ' ')
		dts++;

	if(!*dts)
		goto fail;

	dts++;

	if((tmp = _sf_recognize_month(dts)) < 0)
		goto fail;
	else
		tm.tm_mon = tmp;

	while(*dts >= 'A' || *dts == ' ') dts++;

	if(*dts && dts[1] == ' ') {
		tmp = _sf_natoui(dts++, 1);
	} else {
		tmp = _sf_natoui(dts, 2);
		dts+=2;
	}

	if(tmp < 1)
		goto fail;
	else
		tm.tm_mday = tmp;

	while(*dts && *dts == ' ') dts++;


	if((tmp = _sf_recognize_time(dts)) == -1)
		goto fail;

	dts += ((unsigned int)tmp) >> 24;
	tm.tm_hour = (tmp >> 16) & 0xff;
	tm.tm_min = (tmp >> 8) & 0xff;
	tm.tm_sec = tmp & 0xff;

	while(*dts == ' ') dts++;
	if(!*dts) goto fail;

	if((tmp = _sf_natoui(dts, 4)) < 0) {
		if((tmp = _sf_natoui(dts, 2)) < 0)
			goto fail;
		if((tm.tm_year = _sf_year_range_small(tmp)) == 0)
			goto fail;
		dts+=2;
	} else {
		if((tm.tm_year = _sf_year_range_big(tmp)) == 0)
			goto fail;
		dts+=4;
	}

	while(*dts && *dts == ' ')
		dts++;

	tmp = _sf_recognize_tz(dts);

	goto ex;

}

/* syslogd format (Mar 23 18:15:00) */
if(form == 4) {
f4:

	while(_sf_isspace(*dts) == 0)	/* Skip month */
		dts++;
	while(_sf_isspace(*dts))	/* Skip space before the date */
		dts++;

	if(!*dts)
		goto fail;

	if(
		((tmp = _sf_natoui(dts, 2)) == -1)
		&& ((tmp = _sf_natoui(dts, 1)) == -1)
	)
		goto fail;
	else
		tm.tm_mday = tmp;

	while(*dts && *dts != ' ')	/* Skip date */
		dts++;

	while(*dts && *dts == ' ')	/* Skip space before the time */
		dts++;

	if(!*dts)
		goto fail;

	if((tmp = _sf_recognize_time(dts)) == -1)
		goto fail;

	/*
	**	We don't care about dts now
	dts += ((unsigned int)tmp) >> 24;
	*/
	tm.tm_hour = (tmp >> 16) & 0xff;
	tm.tm_min = (tmp >> 8) & 0xff;
	tm.tm_sec = tmp & 0xff;

	tmp = -1;
	goto ex;
}

ex:

	if(tmp == -1) {
		/*
		 * Time in this form without the GMT offset
		 * is the LOCAL time
		 */
		tmp = 0;
	} else {
		tmp -= _sf_def.cur_off;
	}

	return _sf_mktime(&tm) - tmp;

fail:

	errno = EINVAL;

	return 0;
}





/******************/
/* TIME TO STRING */
/******************/

char *
timetostr(time_t value, int flags) {
#define	SF_INT_TS_SIZE	64
	static char ts[SF_INT_TS_SIZE];
	static struct tm tm;
	static char *wd[]={"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
	static char *mn[]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
		"Aug", "Sep", "Oct", "Nov", "Dec" };
	int uf = 0;
	int ogmt = 0;


	/* Splitting the value */
	if((flags & TFMT_LOCAL) && !(flags & TFMT_OLD822GMT))
		(void)localtime_r(&value, &tm);
	else {
		(void)gmtime_r(&value, &tm);
	}

	if(flags & TFMT_UF)
		uf=1;
	if(flags & TFMT_OLD822GMT)
		ogmt=1;

	flags = flags & (int)0xff;

	if(flags == TFMT_RFC822) {
		char *fmts;

#ifdef	HAVE_TM_ZONE
		if(!tm.tm_zone)
			uf = 0;
#else
#ifndef	HAVE_TZNAME
			uf = 0;
#endif
#endif

		if(ogmt)
			fmts = "%s, %02d %s %4d %02d:%02d:%02d GMT";
		else
			fmts = "%s, %02d %s %4d %02d:%02d:%02d";

		fmts = ts + snprintf(ts, SF_INT_TS_SIZE, fmts,
		wd[tm.tm_wday], tm.tm_mday, mn[tm.tm_mon],
		tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

		if(!ogmt) {
			fmts += snprintf(fmts, SF_INT_TS_SIZE - (fmts - ts),
				" %c%02ld%02ld",

#ifdef	HAVE_TM_ZONE
				(tm.tm_gmtoff<0)?'-':'+',
				(long)( (labs(tm.tm_gmtoff) % 86400)  / 3600 ),
				(long)( (labs(tm.tm_gmtoff) % 3600) / 60 ));
			if(uf)
			fmts += snprintf(fmts, SF_INT_TS_SIZE - (fmts - ts),
				" (%s)", tm.tm_zone);
#else
#ifdef	HAVE_TZNAME
				(timezone>0)?'-':'+',
				(long)( (labs(timezone) % 86400)  / 3600 ),
				(long)( (labs(timezone) % 3600) / 60 ));
			if(uf)
			fmts += snprintf(fmts, SF_INT_TS_SIZE - (fmts - ts),
				" (%s)", tzname[0]);
#else
				0, 0);
#endif
#endif /* HAVE_TM_ZONE */

		}

	} else if(flags == TFMT_ISO8601 || flags == TFMT_X208) {
		char *fmts;

		if(flags == TFMT_ISO8601) {
			if(uf)
				fmts="%04d-%02d-%02dT%02d:%02d:%02d";
			else
				fmts="%04d%02d%02dT%02d%02d%02d";
		} else {
			fmts="%04d%02d%02d%02d%02d%02d";
		}

		fmts = ts + snprintf(ts, SF_INT_TS_SIZE, fmts,
				tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
				tm.tm_hour, tm.tm_min, tm.tm_sec);

#ifdef	HAVE_TM_ZONE
		if(tm.tm_gmtoff)
			snprintf(fmts, SF_INT_TS_SIZE - (fmts - ts), "%c%02ld%02ld",
				(tm.tm_gmtoff<0)?'-':'+',
				(long)( (labs(tm.tm_gmtoff) % 86400)  / 3600 ),
				(long)( (labs(tm.tm_gmtoff) % 3600) / 60 ));
		else
			snprintf(fmts, SF_INT_TS_SIZE - (fmts - ts), "Z");
#else
		if(timezone)
			snprintf(fmts, SF_INT_TS_SIZE - (fmts - ts), "%c%02ld%02ld",
				(timezone>0)?'-':'+',
				(long)( (labs(timezone) % 86400)  / 3600 ),
				(long)( (labs(timezone) % 3600) / 60 ));
		else
			snprintf(fmts, SF_INT_TS_SIZE - (fmts - ts), "Z");
#endif /* HAVE_TM_ZONE */

	} else if(flags == TFMT_CTIME) {
		char *fmts;
		fmts = ts + snprintf(ts, SF_INT_TS_SIZE,
			"%s %s %02d %02d:%02d:%02d %04d",
			wd[tm.tm_wday], mn[tm.tm_mon], tm.tm_mday,
			tm.tm_hour, tm.tm_min, tm.tm_sec, tm.tm_year + 1900);

#ifdef	HAVE_TM_ZONE
		if(uf)
			snprintf(fmts, SF_INT_TS_SIZE - (fmts - ts), " %c%02ld%02ld",
				(tm.tm_gmtoff<0)?'-':'+',
				(long)( (tm.tm_gmtoff % 86400)  / 3600 ),
				(long)( (tm.tm_gmtoff % 3600) / 60 ));
#else
		if(uf)
			snprintf(fmts, SF_INT_TS_SIZE - (fmts - ts), " %c%02ld%02ld",
				(timezone>0)?'-':'+',
				(long)( (timezone % 86400)  / 3600 ),
				(long)( (timezone % 3600) / 60 ));
#endif /* HAVE_TM_ZONE */
	} else {
		sprintf(ts, "%ld", value);
	}

	return ts;
}


