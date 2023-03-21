#include <sys/stat.h>
#include <sys/types.h>
#include "../hook.h"

DEFINE_HOOK_FUNCTION(int, stat, EAGAIN, -1, const char *, path, struct stat *, buf);
DEFINE_HOOK_FUNCTION(int, fstat, EAGAIN, -1, int, fd, struct stat *, buf);
DEFINE_HOOK_FUNCTION(int, stat64, EAGAIN, -1, const char *, path, struct stat *, buf);
DEFINE_HOOK_FUNCTION(int, fstat64, EAGAIN, -1, int, fd, struct stat *, buf);
DEFINE_HOOK_FUNCTION(int, lstat, EAGAIN, -1, const char *, path, struct stat *, buf);
DEFINE_HOOK_FUNCTION(int, lstat64, EAGAIN, -1, const char *, path, struct stat *, buf);
DEFINE_HOOK_FUNCTION(int, fstatat, EAGAIN, -1, int, fd, const char *, path, struct stat *, buf, int, flag);
DEFINE_HOOK_FUNCTION(int, fstatat64, EAGAIN, -1, int, fd, const char *, path, struct stat *, buf, int, flag);
DEFINE_HOOK_FUNCTION(int, chmod, EAGAIN, -1, const char *, path, mode_t, mode);
DEFINE_HOOK_FUNCTION(int, fchmod, EAGAIN, -1, int, fd, mode_t, mode);
DEFINE_HOOK_FUNCTION(int, lchmod, EAGAIN, -1, const char *, path, mode_t, mode);
DEFINE_HOOK_FUNCTION(int, fchmodat, EAGAIN, -1, int, fd, const char *, path, mode_t, mode, int, flag);
DEFINE_HOOK_FUNCTION(__mode_t, umask, EAGAIN, -1, mode_t, mask);
DEFINE_HOOK_FUNCTION(int, mkdir, EAGAIN, -1, const char *, path, mode_t, mode);
DEFINE_HOOK_FUNCTION(int, mkdirat, EAGAIN, -1, int, fd, const char *, path, mode_t, mode);
DEFINE_HOOK_FUNCTION(int, mknod, EAGAIN, -1, const char *, path, mode_t, mode, dev_t, dev);
DEFINE_HOOK_FUNCTION(int, mknodat, EAGAIN, -1, int, fd, const char *, path, mode_t, mode, dev_t, dev);
DEFINE_HOOK_FUNCTION(int, mkfifo, EAGAIN, -1, const char *, path, mode_t, mode);
DEFINE_HOOK_FUNCTION(int, mkfifoat, EAGAIN, -1, int, fd, const char *, path, mode_t, mode);
DEFINE_HOOK_FUNCTION(int, utimensat, EAGAIN, -1, int, fd, const char *, path, const struct timespec *, times, int, flag);
DEFINE_HOOK_FUNCTION(int, futimens, EAGAIN, -1, int, fd, const struct timespec *, times);