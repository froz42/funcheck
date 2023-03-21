#include <semaphore.h>
#include <sys/types.h>
#include "../hook.h"


DEFINE_HOOK_FUNCTION(int, sem_init, EAGAIN, -1, sem_t *, sem, int, pshared, unsigned int, value);
DEFINE_HOOK_FUNCTION(int, sem_destroy, EAGAIN, -1, sem_t *, sem);

sem_t *EXPORT sem_open(const char *name, int oflag, ...)
{
    if (!is_hooks_enabled())
    {
        typeof(&sem_open) sem_open_original = get_original_function();
        va_list ap;
        va_start(ap, oflag);
        mode_t mode = va_arg(ap, mode_t);
        unsigned int value = va_arg(ap, unsigned int);
        sem_t *ret = sem_open_original(name, oflag, mode, value);
        va_end(ap);
        return ret;
    }
    disable_function_hooks();
    set_hooked_function_name(__func__);
    t_shared_info *shared_memory = get_shared_memory();
    get_backtrace(shared_memory->backtrace);
    if (should_block(shared_memory->backtrace))
    {
        errno = EACCES;
        enable_function_hooks();
        return SEM_FAILED;
    }
    typeof(&sem_open) sem_open_original = get_original_function();
    va_list ap;
    va_start(ap, oflag);
    mode_t mode = va_arg(ap, mode_t);
    unsigned int value = va_arg(ap, unsigned int);
    send_function_call_event(shared_memory);
    sem_t *ret = sem_open_original(name, oflag, mode, value);
    va_end(ap);
    set_hooked_function_name(NULL);
    enable_function_hooks();
    return ret;
}

DEFINE_HOOK_FUNCTION(int, sem_close, EAGAIN, -1, sem_t *, sem);
DEFINE_HOOK_FUNCTION(int, sem_unlink, EAGAIN, -1, const char *, name);
DEFINE_HOOK_FUNCTION(int, sem_wait, EAGAIN, -1, sem_t *, sem);
DEFINE_HOOK_FUNCTION(int, sem_trywait, EAGAIN, -1, sem_t *, sem);
DEFINE_HOOK_FUNCTION(int, sem_post, EAGAIN, -1, sem_t *, sem);
DEFINE_HOOK_FUNCTION(int, sem_getvalue, EAGAIN, -1, sem_t *, sem, int *, sval);
