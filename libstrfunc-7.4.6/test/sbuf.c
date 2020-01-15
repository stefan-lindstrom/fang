#include <stdio.h>
#include <strfunc.h>

int
main() {
	sbuf *sb;
	size_t len;

	sb = sbuf_init();

	printf("=== 1\n");

	sbuf_add(sb, "\r\nONE\r\n\r\nTWO\r\n\r\nTHREE\r\n");
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 3));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 3));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 3));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 3));
		printf("%d\n", len);
	printf("[%s]\n", sbuf_detach(sb, NULL, NULL));
	sbuf_zero(sb);

	printf("=== 2\n");

	sbuf_add(sb, "\r\nONE\r\n\r\nTWO\r\n\r\nTHREE\r\n");
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 2));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 2));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 2));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 2));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 2));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 2));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 2));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 2));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 2));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 2));
		printf("%d\n", len);
	printf("[%s]\n", sbuf_detach(sb, NULL, NULL));
	sbuf_zero(sb);

	printf("=== 3\n");

	sbuf_add(sb, "\r\nONE\r\n\r\nTWO\r\n\r\nTHREE\r\n");
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 1));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 1));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 1));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 1));
		printf("%d\n", len);
	printf("[%s]\n", sbuf_detach(sb, NULL, NULL));
	sbuf_zero(sb);

	printf("=== 4\n");

	sbuf_add(sb, "\r\nONE\r\n\r\nTWO\r\n\r\nTHREE\r\n");
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 0));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 0));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 0));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 0));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 0));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 0));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 0));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 0));
		printf("%d\n", len);
	printf("[%s]: ", sbuf_fetch(sb, 0, "\r\n", &len, 0));
		printf("%d\n", len);
	printf("[%s]\n", sbuf_detach(sb, NULL, NULL));
	sbuf_zero(sb);

	return 0;
}
