#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifdef	HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef	HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <strfunc.h>

void check(char *, char *, char *shouldbe);
void stress_testing(void);

int main(int ac, char **av) {

	if(ac > 1) {
		if(strcmp(av[1], "stress") == 0)
			stress_testing();
	}

	check("s/(word)/S\\1/i", "He bought a word and said a word.",
		"[He bought a Sword and said a word.]");
	check("s/(word)/S\\1/ig", "He bought a word and put this word.",
		"[He bought a Sword and put this Sword.]");
	check("s/(tree) (apple)/\\2 \\1/igr", "Tree Apple goose",
		"[Apple Tree goose]");
	check("s/tree apple/sdf/i", "Tree Apple mixed", "[sdf mixed]");
	check("y/abc/AbC/i", "aBcabc", "[AbCAbC]" );
	check("/apple/i", "another apple tree", "[]");
	check("/aple/i", "another apple tree", "<null>");
	check("/apple/yes!/i", "another apple tree", "[yes!]");
	check("/tree$/yes!/i", "another apple tree", "[yes!]");
	check("/t(re)e/yes!/i", "another apple tree", "<null> # No /e");
	check("/([0-9])/i", "celeron-400", "<null>");
	check("/([0-9])/i", "celeron", "<null>");
	check("/([0-9])/ie", "celeron-400", "[]");
	check("/([0-9])/ie", "celeron", "<null>");
	check("/([0-9])/\\1/i", "celeron-400", "[celeron-400]");
	check("/-([0-9]+)/\\1/ier", "celeron-400", "[celeron400]");
	check("/^[;:])([0-9]*)[@]*$/rei", ":)2", "[]");
	check("/ab /reig", "xxx Ab xxx aB xxx", NULL);
	check("/ab /reign", "xxx Ab xxx aB xxx", NULL);
	check("/(ab) /reig", "xxx Ab xxx aB xxx", NULL);
	check("/(ab) /reign", "xxx Ab xxx aB xxx", NULL);
	check("s/tree apple/[\\0]/i", "Tree Apple mixed", "[[Tree Apple] mixed]");
	check("s/=([0-9])=/=z=/ge", "=0= =2= =3=", "[=z= =z= =z=]");
	check("s/(=[0-9]= )+/=z=/re", "=0= =2= =3=", "[=z==3=]");

	check("s/([ ]+)([a-z]{2})/_/rgbe", "quick brown   fox", NULL);
	check("s/^([0-9]{6})$/78422\\1/re", "497035", NULL);
	check("s/Ducap/<\\/p><p>/g", "John Ducap uses only one p in his name.", NULL);
	check("/([_a-z0-9.-]+@[a-z0-9-]+\\.[a-z0-9.-]+)/rgben", "some, vlm@domain.com, another@email.com, whatever", NULL);
	check("/[_a-z0-9.-]+@[a-z0-9-]+\\.[a-z0-9.-]+/beginr", "vlm@domain.com, another@email.com, whatever", NULL);
	check("s/A(.+)B(.+)/1:[\\1], 2:[\\2]/beginr", "A here B there", NULL);
	check("s/A(.+)B(.+)?/1:[\\1], 2:[\\2]/beginr", "A here B there b other", NULL);


	return 0;
};

void
check(char *expr, char *ag, char *sb) {
	sed_t *se;
	svect *sl;
	char *r;

	printf("\n");
	printf("Checking expression:\n");
	printf("[%s] against [%s]\n", expr, ag);

	se=sed_compile(expr);

	if(!se) {
		printf("Compile FAILED!\n");
		return;
	};

	r = sed_exec(se, ag);
	if(r)
		printf("Result: [%s]\n", r);
	else
		printf("Result: <null>\n");
	if(sb)
		printf("Should: %s\n", sb);

	if((sl = sed_results(se)))
		printf("sed_results: [%s]\n", sjoin(sl, "], ["));

	if((sl = sed_border_results(se)))
		printf("sed_border_results: [%s]\n", sjoin(sl, "], ["));

	sed_free(se);
};

void
stress_testing() {
	int z = 10000;

	while(z--) {
		sed_t *se;
		printf("\nCompile\n");
		se = sed_compile("/(0|1|on|off|yes|no|true|false)/ebi");
		assert(se != NULL);
		sed_exec(se, "blah");
		printf("\nDestroy\n");
		sed_free(se);
	}
}




