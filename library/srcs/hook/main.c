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

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include "../shared_memory/shared_memory.h"
#include "../utils/export.h"
#include "../env/clean_env.h"
#include "./hook.h"
#include "../utils/error.h"

static int (*main_orig)(int, char **, char **);

/**
 * @brief main function called by __libc_start_main
 * 
 * @param argc argc
 * @param argv argv
 * @param envp envp
 * @return int the return value of the original main
 */
int main_hook(int argc, char **argv, char **envp)
{
    disable_function_and_alloc_hooks();
    char **cleaned_env = clean_env(envp);
    enable_function_and_alloc_hooks();
    int ret = main_orig(argc, argv, cleaned_env);
    disable_function_and_alloc_hooks();

    free(cleaned_env);
    return ret;
}

/**
 * @brief Hook the __libc_start_main function
 * 
 * @param main the main function
 * @param argc arg count
 * @param argv arg values
 * @param init 
 * @param fini 
 * @param rtld_fini 
 * @param stack_end 
 * @return int 
 */
int EXPORT __libc_start_main(
    int (*main)(int, char **, char **),
    int argc,
    char **argv,
    int (*init)(int, char **, char **),
    void (*fini)(void),
    void (*rtld_fini)(void),
    void *stack_end)
{
    main_orig = main;

    typeof(&__libc_start_main) orig = dlsym(RTLD_NEXT, "__libc_start_main");
    if (!orig)
        raise_error("main_hook: dlsym: could not find __libc_start_main", true);

    return orig(main_hook, argc, argv, init, fini, rtld_fini, stack_end);
}