#include <sys/epoll.h>
#include <signal.h>

int main(void)
{
    int e = epoll_create(1);
    if (e == -1)
        raise(SIGSEGV);
    return 0;
}