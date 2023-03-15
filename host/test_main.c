#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

void wrap(void)
{
	int fd = open("test_main.c", O_RDONLY);
	if (fd == -1)
	{
		printf("Error: %s\n", strerror(errno));
		raise(SIGSEGV);
	}
	else
	{
		printf("Success\n");
		close(fd);
	}
}

int main(void)
{
	wrap();
}