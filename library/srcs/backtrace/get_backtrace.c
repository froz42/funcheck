#define _GNU_SOURCE

#include <dlfcn.h>
#include <execinfo.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <linux/limits.h>
#include <unistd.h>
#include "../utils/config.h"
#include "../../../shared/shared.h"
#include "../utils/error.h"
#include "../hook/hook.h"

#define FALLBACK_MAIN_POS 2

/**
 * @brief Transform the addresses in the backtrace buffer to relative addresses.
 * 
 * @param backtrace_buffer the buffer containing the addresses (also the destination)
 * @param backtrace_size the size of the buffer
 */
static void process_addresses(void **backtrace_buffer, int backtrace_size)
{
	int i;
	for (i = 0; i < backtrace_size; i++)
	{
		Dl_info info;
		if (!dladdr(backtrace_buffer[i], &info))
			raise_error("process_addresses: dladdr", true);
		backtrace_buffer[i] = (void *)((size_t)backtrace_buffer[i] - (size_t)info.dli_fbase - 1);
	}
	backtrace_buffer[i] = NULL;
}

/**
 * @brief Get the backtrace of the current thread.
 * 
 * @param dest the destination buffer
 */
void get_backtrace(ptr_address dest[MAX_BACKTRACE_DEPTH])
{
	void *buffer[MAX_BACKTRACE_DEPTH + 1];

	bool_t is_alloc_hook_enabled = is_alloc_hooks_enabled();
	bool_t is_function_hook_enabled = is_function_hooks_enabled();

	disable_alloc_hooks();
	disable_function_hooks();

	int backtrace_size = backtrace(buffer, MAX_BACKTRACE_DEPTH);
	if (backtrace_size == -1)
		raise_error("get_backtrace: backtrace", true);
	process_addresses(buffer, backtrace_size);
	int i;
	for (i = 0; i < backtrace_size; i++)
		dest[i] = (ptr_address)buffer[i];
	dest[backtrace_size] = 0x0;

	if (is_alloc_hook_enabled)
		enable_alloc_hooks();
	if (is_function_hook_enabled)
		enable_function_hooks();
}