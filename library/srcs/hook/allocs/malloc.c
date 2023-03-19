#include <errno.h>
#include "../hook.h"
#include "../../utils/export.h"
#include "../../events/event_sender.h"
#include "../../function_search/function_search.h"
#include <stdio.h>

void EXPORT *malloc(size_t size)
{
	if (!is_alloc_hooks_enabled())
	{
		typeof(&malloc) original_function = get_original_function();
		return original_function(size);
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
	typeof(&malloc) original_function = get_original_function();
	void *save_result = original_function(size);
	send_alloc_event(shared_memory, save_result, size, !is_function_hooks_enabled());
	enable_alloc_hooks();

	return save_result;
}