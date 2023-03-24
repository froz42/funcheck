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

#include <errno.h>
#include "../hook.h"
#include "../../utils/export.h"
#include "../../events/event_sender.h"
#include "../../function_search/function_search.h"

void EXPORT *calloc(size_t nmemb, size_t size)
{
	if (!is_alloc_hooks_enabled())
	{
		typeof(&calloc) original_function = get_original_function();
		return original_function(nmemb, size);
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
	typeof(&calloc) original_function = get_original_function();
	void *save_result = original_function(nmemb, size);
	send_alloc_event(shared_memory, save_result, size * nmemb, !is_function_hooks_enabled());
	enable_alloc_hooks();

	return save_result;
}
