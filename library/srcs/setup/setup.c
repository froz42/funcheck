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

#include <stdlib.h>
#include <stdio.h>
#include "../shared_memory/shared_memory.h"
#include "../hook/hook.h"

# define BEFORE_MAIN __attribute__((constructor))

/**
 * @brief Setup the library
 * 
 * @return t_shared_info* the shared memory with host
 */
t_shared_info *setup()
{
    disable_function_and_alloc_hooks();
    t_shared_info *shared_memory = init_shared_memory();
    setup_hooks();
    if (shared_memory->runtype == RUNTYPE_TEST)
        set_backtrace_to_block(
            shared_memory->backtrace,
            shared_memory->iteration_to_test);
    setvbuf(stdout, NULL, _IONBF, 0);
    unsetenv("LD_PRELOAD");
    unsetenv(ENV_MEMORY_NAME);
    enable_function_and_alloc_hooks();
    return shared_memory;
}

/**
 * @brief setup the hooks (called before main)
 * 
 */
void BEFORE_MAIN setup_hook()
{
    setup();
}