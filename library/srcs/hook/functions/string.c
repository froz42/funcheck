#include <string.h>
#include "../hook.h"

typedef char *char_ptr_t;

DEFINE_HOOK_FUNCTION(char_ptr_t, strdup, ENOMEM, NULL, const char *, s);
DEFINE_HOOK_FUNCTION(char_ptr_t, strndup, ENOMEM, NULL, const char *, s, size_t, n);
DEFINE_HOOK_FUNCTION(char_ptr_t, strsignal, ENOMEM, NULL, int, sig);
DEFINE_HOOK_FUNCTION(char_ptr_t, strerror, ENOMEM, NULL, int, errnum);