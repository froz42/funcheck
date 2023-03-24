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
#include <dlfcn.h>
#include "./hook.h"
#include "../utils/export.h"
#include "../events/event_sender.h"
#include "../function_search/function_search.h"

/**
 * @brief The free function hook.
 * 
 * @param ptr the pointer to the allocated memory
 */
void EXPORT free(void *ptr)
{
    typeof(&free) original_function = get_original_function();
    t_shared_info *shared_memory = get_shared_memory();
    shared_memory->allocation.ptr = ptr;
    send_event(shared_memory, REMOVE_ALLOC);
    original_function(ptr);
}