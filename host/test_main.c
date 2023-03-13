#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

void wrap(void)
{
	char *str = strdup("Hello world\nhello world\n");
	if (!str)
	{
		fprintf(stderr, "fake crashing: strdup failed: %s\n", strerror(errno));
		raise(SIGSEGV);
	}

	char *str2 = strdup("Hello world\nhello world\n");
	if (!str2)
		return;
	printf("%s", str);
	free(str);
	free(str2);
}

int main(void)
{
	wrap();
}