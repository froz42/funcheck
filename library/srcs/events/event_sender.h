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

#ifndef EVENT_SENDER_H
#define EVENT_SENDER_H

#include "../shared_memory/shared_memory.h"

void send_event(t_shared_info *shared_memory, t_event event);
void send_event_nonblocking(t_shared_info *shared_memory, t_event event);

void _send_alloc_event(
    t_shared_info *shared_memory,
    void *ptr,
    size_t size,
    bool_t is_child,
    const char *function_name);

void _send_remove_alloc_event(
    t_shared_info *shared_memory,
    void *ptr,
    const char *function_name);
void _send_function_call_event(
    t_shared_info *shared_memory,
    const char *function_name);

/**
 * @brief Send an allocation event to host
 *
 * @param shared_memory the shared memory with host
 * @param ptr the pointer to the allocated memory
 * @param size the size of the allocated memory
 * @param is_child whether the allocation is a child of another function
 */
#define send_alloc_event(shared_memory, ptr, size, is_child) \
    _send_alloc_event(shared_memory, ptr, size, is_child, __func__)

/**
 * @brief Send a remove allocation event to host
 *
 * @param shared_memory the shared memory with host
 * @param ptr the pointer to the allocated memory
 */
#define send_remove_alloc_event(shared_memory, ptr) \
    _send_remove_alloc_event(shared_memory, ptr, __func__)

/**
 * @brief Send a function call event to host
 *
 * @param shared_memory the shared memory with host
 */
#define send_function_call_event(shared_memory) \
    _send_function_call_event(shared_memory, __func__)

#endif