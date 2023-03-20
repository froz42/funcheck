#include <pthread.h>
#include "../hook.h"

typedef void *(*start_routine_t)(void *);

DEFINE_HOOK_FUNCTION(int, pthread_create, EAGAIN, EAGAIN, pthread_t *, thread, const pthread_attr_t *, attr, start_routine_t, start_routine, void *, arg);