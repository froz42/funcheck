#include <signal.h> 
#include <sys/types.h>
#include "../hook.h"

DEFINE_HOOK_FUNCTION(__sighandler_t, sysv_signal, EAGAIN, SIG_ERR, int, signum, __sighandler_t, handler);
DEFINE_HOOK_FUNCTION(__sighandler_t, signal, EAGAIN, SIG_ERR, int, signum, __sighandler_t, handler);
DEFINE_HOOK_FUNCTION(__sighandler_t, bsd_signal, EAGAIN, SIG_ERR, int, signum, __sighandler_t, handler);
DEFINE_HOOK_FUNCTION(int, kill, EAGAIN, -1, pid_t, pid, int, sig);
DEFINE_HOOK_FUNCTION(int, killpg, EAGAIN, -1, pid_t, pgrp, int, sig);
DEFINE_HOOK_FUNCTION(int, raise, EAGAIN, -1, int, sig);
DEFINE_HOOK_FUNCTION(__sighandler_t, ssignal, EAGAIN, SIG_ERR, int, signum, __sighandler_t, handler);