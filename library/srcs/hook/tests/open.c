#include <fcntl.h>
#include <errno.h>
#include <dlfcn.h>
#include <stdarg.h>
#include "../hook.h"
#include "../../utils/export.h"
#include "../../events/event_sender.h"
#include "../../function_search/function_search.h"

int EXPORT open(const char * pathname, int flags, ...)
{
    va_list args;
    typeof(&open) original_function = get_original_function();
    if (!is_hooks_enabled())
        return original_function(pathname, flags);
    va_start(args, flags);
    mode_t mode = va_arg(args, mode_t);
    va_end(args);

    disable_hooks();
    t_shared_info *shared_memory = get_shared_memory();
    get_backtrace(shared_memory->backtrace);
    if (should_block(shared_memory->backtrace))
    {
        errno = EACCES;
        enable_hooks();
        return -1;
    }
    int save_result = original_function(pathname, flags, mode);
    send_function_call_event(shared_memory);
    enable_hooks();
    return save_result;
}
