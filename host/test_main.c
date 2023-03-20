#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

int main(void)
{
    int fd = open("test_main.c", O_RDONLY);
    if (fd == -1)
    {
        raise(SIGSEGV);
    }
}