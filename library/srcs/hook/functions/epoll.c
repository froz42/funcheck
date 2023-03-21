#include <sys/epoll.h>
#include <sys/types.h>
#include "../hook.h"

DEFINE_HOOK_FUNCTION(int, epoll_create, EAGAIN, -1, int, size);
DEFINE_HOOK_FUNCTION(int, epoll_create1, EAGAIN, -1, int, flags);
DEFINE_HOOK_FUNCTION(int, epoll_ctl, EAGAIN, -1, int, epfd, int, op, int, fd, struct epoll_event *, event);
DEFINE_HOOK_FUNCTION(int, epoll_wait, EAGAIN, -1, int, epfd, struct epoll_event *, events, int, maxevents, int, timeout);
DEFINE_HOOK_FUNCTION(int, epoll_pwait, EAGAIN, -1, int, epfd, struct epoll_event *, events, int, maxevents, int, timeout, const __sigset_t *, sigmask);
DEFINE_HOOK_FUNCTION(int, epoll_pwait2, EAGAIN, -1, int, epfd, struct epoll_event *, events, int, maxevents, const struct timespec *, timeout, const __sigset_t *, sigmask);
