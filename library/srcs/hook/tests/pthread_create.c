#include <pthread.h>
#include <errno.h>
#include "../hook.h"
#include "../../utils/export.h"
#include "../../events/event_sender.h"
#include "../../events/event_sender.h"
#include "../../function_search/function_search.h"


int pthread_create(pthread_t *restrict thread,
                   const pthread_attr_t *restrict attr,
                   void *(*start_routine)(void *),
                   void *restrict arg)
{
    typeof(&pthread_create) original_function = get_original_function();
	if (!is_hooks_enabled())
		return original_function(thread, attr, start_routine, arg);
    
    disable_hooks();
    t_shared_info *shared_memory = get_shared_memory();
    get_backtrace(shared_memory->backtrace);
    if (should_block(shared_memory->backtrace))
    {
        enable_hooks();
        errno = EAGAIN;
        return EAGAIN;
    }
    int save_result = original_function(thread, attr, start_routine, arg);
    send_function_call_event(shared_memory);
    enable_hooks();
    
    return save_result;
}