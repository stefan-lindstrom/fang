#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef  HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef  HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <strfunc.h>

void test(const char *str, const char *dlm, int i);

int
main() {
	int i;
	svect *sl;
	char *orig = "z\\b\"one two three\" and 'this\\' \"' is \"the '";
	char *s;

	sl = sinit();

	splitquotable(sl, orig);

	printf("Original string: %s\n", orig);
	printf("[%s]\n", sjoin(sl, "], ["));

	printf("Lengths: ");
	for(i = 0; i < sl->count; i++) {
		if(i)
			printf(", ");
		printf("[%d]", sl->list[i][sl->lens[i]+1]);
	}
	printf("\n");

	sclear(sl);
	splitf(sl, "one, two,three, , four five, six", ", ", 3);
	printf("[%s]\n", sjoin(sl, "], ["));

	sclear(sl);
	splitf(sl, "one, two,three, , four five, six", ", ", 2);
	printf("[%s]\n", sjoin(sl, "], ["));

	sclear(sl);
	splitf(sl, "one two three, , four five		six", "[ \t,]+", 4);
	printf("[%s]\n", sjoin(sl, "], ["));

	sclear(sl);
	splitf(sl, "one two three, , four five		six", "[ \t,]", 4);
	printf("[%s]\n", sjoin(sl, "], ["));

	test(strdup("where a and b"), "[ ]+(or|and)[ ]+", 4);

	test(strdup(" one two three"), " ", 4);
	test(strdup(" one two three"), "( )*", 4);

	test(strdup("one two three"), " ", 4);
	test(strdup("one two three"), "( )*", 4);

	test(strdup("oneword"), " ", 4);
	test(strdup("oneword"), "( )*", 4);

	return 0;
}


void
test(const char *str, const char *dlm, int i) {
	static slist *sl = NULL;
	int ret;

	if(sl)
		sclear(sl);
	else
		sl = sinit();


	ret = splitf(sl, str, dlm, i);
	printf("\nsplitf(..., \"%s\", \"%s\", %d) = %d\n", str, dlm, i, ret);
	if(sl->count)
		printf(" results: [%s]\n", sjoin(sl, "], ["));
	else
		printf(" results: <none>\n");
}
