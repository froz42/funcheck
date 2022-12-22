#include <sys/mman.h>
#include <errno.h>
#include "../hook.h"
#include "../../utils/export.h"
#include "../../events/event_sender.h"
#include "../../function_search/function_search.h"


void EXPORT *mmap(void *addr, size_t len, int prot, int flags,
           int fildes, off_t off)
{
    typeof(&mmap) original_function = get_original_function();
    if (!is_hooks_enabled())
        return original_function(addr, len, prot, flags, fildes, off);

    disable_hooks();
    t_shared_info *shared_memory = get_shared_memory();
    get_backtrace(shared_memory->backtrace);
    if (should_block(shared_memory->backtrace))
    {
        errno = ENOMEM;
        enable_hooks();
        return MAP_FAILED;
    }
    send_function_call_event(shared_memory);
    enable_hooks();

    return original_function(addr, len, prot, flags, fildes, off);
}