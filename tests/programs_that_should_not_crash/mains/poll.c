#include <poll.h>
#include <signal.h>

int main(void)
{
    struct pollfd fds[1];
    fds[0].fd = 0;
    int ret = poll(fds, 1, 0);
    if (ret == -1)
        return 1;
    return 0;
}