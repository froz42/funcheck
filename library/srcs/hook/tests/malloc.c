#include <errno.h>
#include <dlfcn.h>
#include "../hook.h"
#include "../../utils/export.h"
#include "../../events/event_sender.h"
#include "../../events/event_sender.h"
#include "../../function_search/function_search.h"

void EXPORT *malloc(size_t size)
{
	typeof(&malloc) original_function = get_original_function();
	if (!is_hooks_enabled())
		return original_function(size);

	disable_hooks();
	t_shared_info *shared_memory = get_shared_memory();
	get_backtrace(shared_memory->backtrace);
	if (should_block(shared_memory->backtrace))
	{
		errno = ENOMEM;
		enable_hooks();
		return NULL;
	}
	void *save_result = original_function(size);
	send_alloc_event(shared_memory, save_result, size);
	enable_hooks();

	return save_result;
}