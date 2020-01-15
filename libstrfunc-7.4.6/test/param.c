#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <strfunc.h>

void
check(char *key, char *value) {
	char *p;

	printf("Check(\"%s\", \"%s\")\n", key, value);

	p = param(key);
	if(value == NULL && p == NULL)
		return;

	if(p == NULL && value) {
		printf("Error: param(\"%s\") returned NULL\n", key);
		exit(2);
	}

	if(strcmp(p, value)) {
		printf("Value differ from pattern\n");
		exit(2);
	}

}

int
main() {
	putenv("REQUEST_METHOD=GET");
	putenv("QUERY_STRING=a=b&b=c&c=%20d&attribute=value%20");

	check("a", "b");
	check("b", "c");
	check("c", " d");
	check("attribute", "value ");
}
