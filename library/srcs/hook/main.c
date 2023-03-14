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
    disable_hooks();
    char **cleaned_env = clean_env(envp);
    enable_hooks();
    int ret = main_orig(argc, argv, cleaned_env);
    disable_hooks();

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