#include <poll.h>
#include <errno.h>
#include "../hook.h"
#include "../../utils/export.h"
#include "../../events/event_sender.h"
#include "../../events/event_sender.h"
#include "../../function_search/function_search.h"

int EXPORT poll(struct pollfd fds[], nfds_t nfds, int timeout)
{
    typeof(&poll) original_function = get_original_function();
    if (!is_hooks_enabled())
        return original_function(fds, nfds, timeout);

    disable_hooks();
    t_shared_info *shared_memory = get_shared_memory();
    get_backtrace(shared_memory->backtrace);
    if (should_block(shared_memory->backtrace))
    {
        errno = EAGAIN;
        enable_hooks();
        return -1;
    }
    send_function_call_event(shared_memory);
    enable_hooks();

    return original_function(fds, nfds, timeout);
}