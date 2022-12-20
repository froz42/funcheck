#include <errno.h>
#include <dlfcn.h>
#include "./hook.h"
#include "../utils/export.h"
#include "../events/event_sender.h"
#include "../function_search/function_search.h"


void EXPORT free(void *ptr)
{
    typeof(&free) original_function = get_original_function();
    if (!is_hooks_enabled())
        return original_function(ptr);

    disable_hooks();
    t_shared_info *shared_memory = get_shared_memory();
    shared_memory->allocation.ptr = ptr;
    send_event(shared_memory, REMOVE_ALLOC);
    original_function(ptr);
    enable_hooks();
}