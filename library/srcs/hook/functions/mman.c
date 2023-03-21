#include <sys/mman.h>
#include <sys/types.h>
#include "../hook.h"

typedef void *void_ptr_t;

DEFINE_HOOK_FUNCTION(void_ptr_t, mmap, ENOMEM, MAP_FAILED, void *, addr, size_t, length, int, prot, int, flags, int, fd, off_t, offset);
DEFINE_HOOK_FUNCTION(void_ptr_t, mmap64, ENOMEM, MAP_FAILED, void *, addr, size_t, length, int, prot, int, flags, int, fd, __off64_t, offset);
DEFINE_HOOK_FUNCTION(int, munmap, ENOMEM, -1, void *, addr, size_t, length);
DEFINE_HOOK_FUNCTION(int, mprotect, ENOMEM, -1, void *, addr, size_t, len, int, prot);
DEFINE_HOOK_FUNCTION(int, msync, ENOMEM, -1, void *, addr, size_t, length, int, flags);
DEFINE_HOOK_FUNCTION(int, madvise, ENOMEM, -1, void *, addr, size_t, length, int, advice);
DEFINE_HOOK_FUNCTION(int, posix_madvise, ENOMEM, -1, void *, addr, size_t, length, int, advice);
DEFINE_HOOK_FUNCTION(int, mlock, ENOMEM, -1, const void *, addr, size_t, len);
DEFINE_HOOK_FUNCTION(int, munlock, ENOMEM, -1, const void *, addr, size_t, len);
DEFINE_HOOK_FUNCTION(int, mlockall, ENOMEM, -1, int, flags);
DEFINE_HOOK_FUNCTION(int, munlockall, ENOMEM, -1);
DEFINE_HOOK_FUNCTION(int, mincore, ENOMEM, -1, void *, start, size_t, length, unsigned char *, vec);
DEFINE_HOOK_FUNCTION(int, shm_open, ENOMEM, -1, const char *, name, int, oflag, mode_t, mode);
DEFINE_HOOK_FUNCTION(int, shm_unlink, ENOMEM, -1, const char *, name);