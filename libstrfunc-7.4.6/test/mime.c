#include <stdio.h>
#include <strfunc.h>

int
main(int ac, char **av) {
	char *string;

	if(ac > 1)
		string = av[1];
	else
		string = "=?koi8-r?b?1MXT1C=?=";

	printf("[%s] = [%s]\n", string, mime_word_decode(string));

	return 0;
}
