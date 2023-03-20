#include <stdlib.h>
#include "../hook.h"

typedef char *char_ptr_t;
typedef void *void_ptr_t;

DEFINE_HOOK_FUNCTION(char_ptr_t, initstate, ENOSYS, NULL, unsigned int, seed, char *, state, size_t, size);
DEFINE_HOOK_FUNCTION(char_ptr_t, setstate, ENOSYS, NULL, char *, state);

DEFINE_HOOK_FUNCTION(int, random_r, ENOSYS, -1, struct random_data *, buf, int32_t *, result);
DEFINE_HOOK_FUNCTION(int, srandom_r, ENOSYS, -1, unsigned int, seed, struct random_data *, buf);
DEFINE_HOOK_FUNCTION(int, initstate_r, ENOSYS, -1, unsigned int, seed, char *, state, size_t, size, struct random_data *, buf);
DEFINE_HOOK_FUNCTION(int, setstate_r, ENOSYS, -1, char *, state, struct random_data *, buf);

DEFINE_HOOK_FUNCTION(void_ptr_t, valloc, ENOMEM, NULL, size_t, size);

typedef void (*atexit_function_t)(void);

typedef void (*on_exit_function_t)(int, void *);

DEFINE_HOOK_FUNCTION(int, on_exit, ENOMEM, -1, on_exit_function_t, function, void *, arg);
DEFINE_HOOK_FUNCTION(int, at_quick_exit, ENOMEM, -1, atexit_function_t, function);

typedef void (*on_quick_exit_function_t)(void);

DEFINE_HOOK_FUNCTION(int, on_quick_exit, ENOMEM, -1, on_quick_exit_function_t, function);

DEFINE_HOOK_FUNCTION(char_ptr_t, getenv, ENOMEM, NULL, const char *, name);
DEFINE_HOOK_FUNCTION(int, putenv, ENOMEM, -1, char *, string);
DEFINE_HOOK_FUNCTION(int, setenv, ENOMEM, -1, const char *, name, const char *, value, int, overwrite);
DEFINE_HOOK_FUNCTION(int, unsetenv, ENOMEM, -1, const char *, name);

DEFINE_HOOK_FUNCTION(char_ptr_t, mktemp, ENOMEM, NULL, char *, template);
DEFINE_HOOK_FUNCTION(int, mkstemp, ENOMEM, -1, char *, template);
DEFINE_HOOK_FUNCTION(int, mkstemps, ENOMEM, -1, char *, template, int, suffixlen);
DEFINE_HOOK_FUNCTION(int, mkostemp, ENOMEM, -1, char *, template, int, flags);
DEFINE_HOOK_FUNCTION(int, mkostemps, ENOMEM, -1, char *, template, int, suffixlen, int, flags);

DEFINE_HOOK_FUNCTION(int, system, ENOMEM, -1, const char *, command);

DEFINE_HOOK_FUNCTION(char_ptr_t, realpath, ENOMEM, NULL, const char *, path, char *, resolved_path);

DEFINE_HOOK_FUNCTION(int, posix_openpt, ENOMEM, -1, int, flags);
DEFINE_HOOK_FUNCTION(int, grantpt, ENOMEM, -1, int, fd);
DEFINE_HOOK_FUNCTION(int, unlockpt, ENOMEM, -1, int, fd);
DEFINE_HOOK_FUNCTION(char_ptr_t, ptsname, ENOMEM, NULL, int, fd);
DEFINE_HOOK_FUNCTION(int, ptsname_r, ENOMEM, -1, int, fd, char *, buf, size_t, buflen);
DEFINE_HOOK_FUNCTION(int, getpt, ENOMEM, -1);

