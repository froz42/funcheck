#include <stdlib.h>
#include <stdio.h>
#include "../shared_memory/shared_memory.h"
#include "../hook/hook.h"

# define BEFORE_MAIN __attribute__((constructor))

t_shared_info *setup()
{
    disable_hooks();
    t_shared_info *shared_memory = init_shared_memory();
    setup_hooks();
    if (shared_memory->runtype == RUNTYPE_TEST)
        set_backtrace_to_block(shared_memory->backtrace, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    unsetenv("LD_PRELOAD");
    unsetenv(ENV_MEMORY_NAME);
    enable_hooks();
    return shared_memory;
}

void BEFORE_MAIN setup_hook()
{
    setup();
}