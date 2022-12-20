#include <errno.h>
#include "../hook.h"
#include "../../utils/export.h"
#include "../../events/event_sender.h"
#include "../../function_search/function_search.h"


void EXPORT *realloc(void *ptr, size_t size)
{
	typeof(&realloc) original_function = get_original_function();
    if (!is_hooks_enabled())
        return original_function(ptr, size);
	
    disable_hooks();
	t_shared_info *shared_memory = get_shared_memory();
	get_backtrace(shared_memory->backtrace);
	if (should_block(shared_memory->backtrace))
	{
		enable_hooks();
		errno = ENOMEM;
		return NULL;
	}
	void *save_result = ptr;
	send_remove_alloc_event(shared_memory, ptr);
	save_result = original_function(ptr, size);
	send_alloc_event(shared_memory, save_result, size);
	enable_hooks();

	return save_result;
}