#include <sys/epoll.h>
#include <signal.h>

int main(void)
{
    int e = epoll_create(1);
    if (e == -1)
        return 1;
    struct epoll_event ev;
    if (epoll_wait(e, &ev, 1, 0) == -1)
        return 1;
    return 0;
}