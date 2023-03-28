/* Funcheck - A tool for checking functions calls return protections
 * Copyright (C) 2023  Theo Matis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
#define MAGIC_LIMIT_RELATIVE_ADDRESS (void *)0x1000000

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
		// if we have an address bellow 0x1000000
		// we dont need to calculate the relative address
		// since it's already relative
		// we just need to substract 1 to the address to
		// get the right address
		if (backtrace_buffer[i] < (void *)MAGIC_LIMIT_RELATIVE_ADDRESS)
			backtrace_buffer[i] = (void *)((size_t)backtrace_buffer[i] - 1);
		else
		{
			Dl_info info;
			if (!dladdr(backtrace_buffer[i], &info))
				raise_error("process_addresses: dladdr", true);
			backtrace_buffer[i] = (void *)((size_t)backtrace_buffer[i] - (size_t)info.dli_fbase - 1);
		}
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