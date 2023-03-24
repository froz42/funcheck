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

#include <string.h>
#include <stdio.h>
#include "../shared_memory/shared_memory.h"
#include "../utils/error.h"
#include "../hook/hook.h"

/**
 * @brief Send an event to host
 *
 * @param shared_memory the shared memory with host
 * @param event the event to send
 */
void send_event(t_shared_info *shared_memory, t_event event)
{
	bool_t function_hooks_enabled = is_function_hooks_enabled();
	disable_function_hooks();
	shared_memory->event = event;
	if (sem_post(&shared_memory->lock_host) == -1)
		raise_error("send_event: sem_post", true);
	if (sem_wait(&shared_memory->lock_guest) == -1)
		raise_error("send_event: sem_wait", true);
	if (function_hooks_enabled)
		enable_function_hooks();
}

/**
 * @brief Send an event to host without waiting for response
 *
 * @param shared_memory the shared memory with host
 * @param event the event to send
 */
void send_event_nonblocking(t_shared_info *shared_memory, t_event event)
{
	bool_t function_hooks_enabled = is_function_hooks_enabled();
	shared_memory->event = event;
	if (sem_post(&shared_memory->lock_host) == -1)
		raise_error("send_event: sem_post", true);
	if (function_hooks_enabled)
		enable_function_hooks();
}

/**
 * @brief Send an allocation event to host
 *
 * @param shared_memory the shared memory with host
 * @param ptr the pointer to the allocated memory
 * @param size the size of the allocated memory
 * @param function_name the name of the function that allocated the memory
 */
void _send_alloc_event(
	t_shared_info *shared_memory,
	void *ptr,
	size_t size,
	bool_t is_child,
	const char *function_name)
{
	shared_memory->allocation.ptr = ptr;
	shared_memory->allocation.size = size;
	shared_memory->should_test = !is_child;

	if (is_child)
	{
		const char *parent_function_name = get_hooked_function_name();
		snprintf(
			shared_memory->function_name,
			sizeof(shared_memory->function_name),
			"%s (%s)",
			parent_function_name,
			function_name);
	}
	else
		strncpy(
			shared_memory->function_name,
			function_name,
			sizeof(shared_memory->function_name));
	send_event(shared_memory, ALLOC);
}

/**
 * @brief Send a remove allocation event to host
 *
 * @param shared_memory the shared memory with host
 * @param ptr the pointer to the allocated memory
 * @param function_name the name of the function that allocated the memory
 */
void _send_remove_alloc_event(
	t_shared_info *shared_memory,
	void *ptr,
	const char *function_name)
{
	;
	shared_memory->allocation.ptr = ptr;
	strncpy(
		shared_memory->function_name,
		function_name,
		sizeof(shared_memory->function_name));
	send_event(shared_memory, REMOVE_ALLOC);
}

/**
 * @brief Send a function call event to host
 *
 * @param shared_memory the shared memory with host
 * @param function_name the name of the function that was called
 */
void _send_function_call_event(
	t_shared_info *shared_memory,
	const char *function_name)
{
	shared_memory->should_test = true;
	strncpy(
		shared_memory->function_name,
		function_name,
		sizeof(shared_memory->function_name));
	send_event(shared_memory, FUNCTION_CALL);
}