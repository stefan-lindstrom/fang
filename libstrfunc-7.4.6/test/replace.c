#include <stdio.h>
#include <strfunc.h>

int
main() {
/*
	char *body;

	body = "\r\n-- \r\nLev Walkin\r\nvlm@spelio.net.ru\r\n";

	printf("%s", replace(body, "\r\n", "\n"));
*/
char *body = "The answer is obvious, because you can't beat it. \r\n"
"<?noformat\r\n"
"   denna\r\n"
" text\r\n"
"skall ej\r\n"
" autoformatas!\r\n"
"?>\r\n"
"Nu skall vi testa <?noformat something completely different?> Hur\r\n"
"Kommer <?noformatdetta att funka?>?\r\n";
    char *s1, *s2;

    replace_detach();
    s1 = replace(body,"<?noformat","");
    replace_detach();
    s2 = replace(s1  , "?>", "");
    replace_detach();
   
    printf(s1);
    printf(s2);

	return 0;
}
