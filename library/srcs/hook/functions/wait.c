#include <sys/wait.h>
#include <sys/types.h>
#include <bits/types/siginfo_t.h>
#include "../hook.h"

DEFINE_HOOK_FUNCTION(__pid_t, wait, EAGAIN, -1, int *, status);
DEFINE_HOOK_FUNCTION(__pid_t, waitpid, EAGAIN, -1, __pid_t, pid, int *, status, int, options);
DEFINE_HOOK_FUNCTION(int, waitid, EAGAIN, -1, idtype_t, idtype, __id_t, id, siginfo_t *, infop, int, options);
DEFINE_HOOK_FUNCTION(__pid_t, wait3, EAGAIN, -1, int *, status, int, options, struct rusage *, rusage);
DEFINE_HOOK_FUNCTION(__pid_t, wait4, EAGAIN, -1, __pid_t, pid, int *, status, int, options, struct rusage *, rusage);
