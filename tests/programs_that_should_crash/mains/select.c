#include <sys/select.h>
#include <signal.h>
#include <fcntl.h>

#define NULL ((void *)0)

int main(void)
{
    // test with /dev/urandom
    fd_set fds;
    FD_ZERO(&fds);
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1)
        return 1;
    FD_SET(fd, &fds);
    struct timeval tv = {0, 0};
    if (select(1, &fds, NULL, NULL, &tv) == -1)
        raise(SIGSEGV);
    return 0;
}