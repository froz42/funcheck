#include <errno.h>
#include "../hook.h"
#include "../../utils/export.h"
#include "../../events/event_sender.h"
#include "../../function_search/function_search.h"


void EXPORT *realloc(void *ptr, size_t size)
{
	if (!is_alloc_hooks_enabled())
	{
		typeof(&realloc) original_function = get_original_function();
		return original_function(ptr, size);
	}
	disable_alloc_hooks();
	t_shared_info *shared_memory = get_shared_memory();
	get_backtrace(shared_memory->backtrace);
	if (should_block(shared_memory->backtrace))
	{
		errno = ENOMEM;
		enable_alloc_hooks();
		return NULL;
	}
	void *save_result = ptr;
	send_remove_alloc_event(shared_memory, ptr);
	typeof(&realloc) original_function = get_original_function();
	save_result = original_function(ptr, size);
	send_alloc_event(shared_memory, save_result, size, !is_function_hooks_enabled());
	enable_alloc_hooks();

	return save_result;
}