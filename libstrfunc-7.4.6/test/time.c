#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/time.h>
#include <assert.h>

#include <strfunc.h>

const	int cycles = 1000000;

int rec_speed(char *time_string, int zinfo_caching);
int check(char *parse, char *test);

void fail(char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	exit(1);
}

int
main(int ac, char **av) {
	char buf[256];
	char *time_string = NULL;
	struct tm tm;
	time_t tloc;

	if(ac >= 2)
		time_string = av[1];

	time(&tloc);
	localtime_r(&tloc, &tm);
	sprintf(buf, "%04d-%02d-%02dT12:23:00",
		tm.tm_year + 1900,
		tm.tm_mon + 1,
		tm.tm_mday
	);

	/* HH:MM */
	assert(check("12:23", buf));

	/* HH:MM:SS */
	assert(check("12:23:00", buf));

	/* YYYYMMDD */
	assert(check("20020516", "2002-05-16T00:00:00"));

	/* MM.DD.YYYY */
	assert(check("27.08.1980", "1980-08-27T00:00:00"));

	/* MM.DD.YY */
	assert(check("27.08.80", "1980-08-27T00:00:00"));

	/* DD/MM/YYYY */
	assert(check("27/08/1980", "1980-08-27T00:00:00"));

	assert(check("Sat, 30 Sep 2000 17:24:58 +0400",
		"2000-09-30T13:24:58Z"));

	assert(check("Sat, 30 Sep 2000 17:24:58 PST",
		"2000-10-01T01:24:58Z"));

	assert(check("20000930T172458+0400",
		"2000-09-30T13:24:58Z"));

	assert(check("2000-09-30T17:24:58+0400",
		"2000-09-30T13:24:58Z"));

	assert(check("2000-09-30T21:24:58Z",
		"2000-09-30T21:24:58Z"));

	assert(check(
		"20031226T151515Z",
		"2003-12-26T15:15:15Z"));

	assert(check(
		"Wed, 24 Dec 2001 16:00:01 -0800 (PST)",
		"2001-12-25T00:00:01Z"));

	assert(check("Sat Sep 30 17:24:58 2000",
		"2000-09-30T17:24:58"));

	assert(check("1997/06/23 13:22:33",
		"1997-06-23T13:22:33"));

	{
	char buf[64];
	sprintf(buf, "%d-03-23T19:07:43", tm.tm_year + 1900);
	assert(check("Mar 23 19:07:43", buf));
	}

	assert(check("Thursday, 10-Jun-93 01:29:59 GMT",
		"1993-06-10T01:29:59Z"));

	assert(check("Thu, 10 Jan 1993 01:29:59 GMT",
		"1993-01-10T01:29:59Z"));

	assert(check("Wed Jun  9 01:29:59 1993 GMT",
		"1993-06-09T01:29:59Z"));

	assert(rec_speed(time_string, 0) == 0);
	assert(rec_speed(time_string, 1) == 0);

	return 0;
}

int
check(char *parse, char *test) {
	time_t t;
	char *p;
	static int count = 0;
	int flags;

	printf("Test %d\n", ++count);

	printf("  Got: [%s]\n", parse);

	t = strtotime(parse);
	if(t <= 0) {
		printf("  UNKNOWN DATE TYPE SUPPLIED\n");
		return 0;
	}

	printf("  S2T: %ld\n", (long)t);

	/* User-friendly ISO8601 time output, GMT */
	flags = TFMT_ISO8601 | TFMT_UF;

	p = timetostr(t, flags);

	printf("  T2S: [%s]\n", p);
	printf("  Tst: [%s]%s\n", test, test ? "" : " Not checking for validity.");

	if(!p || (test && strtotime(test) != t)) {
		printf("strtotime(%s)=%ld, strtotime(%s)=%ld\n", parse, t, test, strtotime(test));
		printf("  NOT EQUAL TO SUPPLIED!\n");
		return 0;
	}

	return 1;
}

int
rec_speed(char *time_string, int zinfo_caching) {
	register int i;
	time_t t;
	struct timeval oldtv, tv;
	double ms;
	char *p;

	if(!time_string)
		time_string = "Wed, 24 Dec 2001 16:00:01 -0800 (PST)";

	if(strfunc_ctl(SF_SET_STRTOTIME_ZINFO_CACHING, &zinfo_caching))
		return -1;

	printf("\nMeasuring speed of recognition %s ZINFO_CACHING\n",
		zinfo_caching ? "with" : "without");
	printf("\tPattern: [%s]\n", time_string);

	gettimeofday(&oldtv, NULL);
	for(i = 0; i < cycles; i++)
		t = strtotime(time_string);
	gettimeofday(&tv, NULL);

	p = timetostr(t, TFMT_LOCAL | TFMT_CTIME);
	if(p == NULL) return -1;
	printf("\tRecognized: [%s]\n", p);

	tv.tv_sec -= oldtv.tv_sec;
	tv.tv_usec -= oldtv.tv_usec;

	ms = (double)tv.tv_sec * 1000 + (double)tv.tv_usec / 1000;
	printf("\tstrtotime(): %d recognitions per %0.1f ms.\n",
		cycles, ms);

	return 0;
}
