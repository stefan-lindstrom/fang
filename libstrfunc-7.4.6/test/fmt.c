#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifdef  HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef  HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <strfunc.h>

char **func(char *found, void *optKeyPassed) {
	static char *tmp[2] = {NULL};

	(void)optKeyPassed;

	if(*found == '1') {
		*tmp = "value mapped from 1";
		return tmp;
	}

	if(*found == '2') {
		*tmp = "value mapped from 2";
		return tmp;
	}

	if(*found == '3') {
		static char *zzz[] = { "3:one", "3:two", "3:three", NULL };
		return zzz;
	}

	return NULL;
}

int
main() {

	fmt_base *fb;
	char *p;

	fb = format_init();

	if(!fb) {
		perror("format_init() failed");
		exit(1);
	}

	format_metarule(fb, '{', '}', func);

	assert(strcmp(formatf(fb, "${3?YES:NO}", NULL), "YES") == 0);
	assert(strcmp(formatf(fb, "${3[2]?YES:NO}", NULL), "YES") == 0);
	assert(strcmp(formatf(fb, "${3[3]?YES:NO}", NULL), "NO") == 0);

	printf("%s",
		formatf(fb, "${1}\n${2}\n${1?${3+\n\t}}\n", NULL)
	);

	p = formatf(fb, "${3[1]?YES:NO}", NULL);
	assert(strcmp(p, "YES") == 0);
	p = formatf(fb, "${3[3]?YES:NO}", NULL);
	assert(strcmp(p, "NO") == 0);

	format_free(fb);

	return 0;
}
