#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

int main(void)
{
    char buffer[1];
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1)
        return 1;
    int ret = read(fd, buffer, sizeof(buffer));
    if (ret == -1)
        raise(SIGSEGV);
    return 0;
}