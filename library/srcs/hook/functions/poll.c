#include <sys/poll.h>
#include <bits/types/__sigset_t.h>

#include "../hook.h"

DEFINE_HOOK_FUNCTION(int, poll, EAGAIN, -1, struct pollfd *, fds, nfds_t, nfds, int, timeout);
DEFINE_HOOK_FUNCTION(int, ppoll, EAGAIN, -1, struct pollfd *, fds, nfds_t, nfds, const struct timespec *, timeout, const __sigset_t *, sigmask);
