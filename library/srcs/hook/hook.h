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

#ifndef HOOK_H
#define HOOK_H

#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "../backtrace/backtrace.h"
#include "../utils/bool.h"
#include "../events/event_sender.h"
#include "../function_search/function_search.h"
#include "../utils/export.h"
#include "../utils/macro.h"


void set_backtrace_to_block(ptr_address *backtrace, size_t iteration_to_block);

void disable_function_hooks(void);
void enable_function_hooks(void);
void disable_alloc_hooks(void);
void enable_alloc_hooks(void);
void disable_function_and_alloc_hooks(void);
void enable_function_and_alloc_hooks(void);

bool_t is_function_hooks_enabled(void);
bool_t is_alloc_hooks_enabled(void);

bool_t is_hooks_enabled(void);

bool_t should_block(ptr_address *backtrace);

void setup_exit_hook(void);
void setup_signals(void);

void setup_hooks(void);

void set_hooked_function_name(const char *function_name);

const char *get_hooked_function_name(void);

/**
 * @brief The hook function definition
 *
 * @param type the return type of the function
 * @param name the name of the function
 * @param error_no the error number to set
 * @param error_return the value to return in case of error
 * @param ... the arguments of the function
 *
 * @example DEFINE_HOOK_FUNCTION(int, open, EACCES, -1, char*, pathname, int, flags, mode_t, mode)
 */
#define DEFINE_HOOK_FUNCTION(type, name, error_no, error_return, ...)            \
    type EXPORT name(APPLYTWOJOIN(ARGS, (, ), (void), ##__VA_ARGS__))            \
    {                                                                            \
        if (!is_hooks_enabled())                                                 \
        {                                                                        \
            typeof(&name) name##_original = get_original_function();             \
            return name##_original(APPLYTWOJOIN(PASS, (, ), (), ##__VA_ARGS__)); \
        }                                                                        \
        disable_function_hooks();                                                \
        set_hooked_function_name(#name);                                         \
        t_shared_info *shared_memory = get_shared_memory();                      \
        get_backtrace(shared_memory->backtrace);                                 \
        if (should_block(shared_memory->backtrace))                              \
        {                                                                        \
            errno = error_no;                                                    \
            enable_function_hooks();                                             \
            return error_return;                                                 \
        }                                                                        \
        typeof(&name) name##_original = get_original_function();                 \
        send_function_call_event(shared_memory);                                 \
        type ret = name##_original(APPLYTWOJOIN(PASS, (, ), (), ##__VA_ARGS__)); \
        set_hooked_function_name(NULL);                                          \
        enable_function_hooks();                                                 \
        return ret;                                                              \
    }

#endif