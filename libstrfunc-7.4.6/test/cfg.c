#include <stdio.h>
#include <strfunc.h>

int
main() {
	char *value1;
	svect *values2;
	svect *values3;
	int r;

	if((r=cfgread("test.cfg")) != 0) {
		if(r > 0) {
			printf("Wrong file format");
		} else {
			printf("File access failed");
		};
		return 1;
	};

	value1  = cfgget("key1");
	values2 = cfgget2("key2");
	values3 = cfgget2("key3");

	printf("key1=\"%s\"\n", value1);
	printf("key2={\"%s\"}\n", sjoin(values2, "\", \""));
	printf("key3={\"%s\"}\n", sjoin(values3, "\", \""));

	/* Free allocated structures */
	sfree(values2);
	sfree(values3);

	return 0;
};
