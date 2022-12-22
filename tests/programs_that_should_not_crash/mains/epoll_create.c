#include <sys/epoll.h>

int main(void)
{
    int e = epoll_create(1);
    if (e == -1)
        return 1;
    return 0;
}