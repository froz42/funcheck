#include <sys/select.h>
#include <sys/types.h>
#include "../hook.h"

DEFINE_HOOK_FUNCTION(int, select, EAGAIN, -1, int, nfds, fd_set *, readfds, fd_set *, writefds, fd_set *, exceptfds, struct timeval *, timeout);
DEFINE_HOOK_FUNCTION(int, pselect, EAGAIN, -1, int, nfds, fd_set *, readfds, fd_set *, writefds, fd_set *, exceptfds, const struct timespec *, timeout, const __sigset_t *, sigmask);