#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

void wrap(void)
{
	printf("Hello world\nhello world\n");
	char *str = strdup("Hello world\nhello world\n");
	if (!str)
		raise(SIGSEGV);
}

int main(void)
{
	wrap();
}