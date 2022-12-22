#include <string.h>
#include "stages.h"
#include "../shared_memory/shared_memory.h"
#include "../env/env.h"

static char *get_library_path(void)
{
    static char path[1024];
    char *exec_path = realpath("/proc/self/exe", NULL);
    if (exec_path == NULL)
    {
        dprintf(2, "Error: could not find the execution path\n");
        exit(1);
    }
    // remove the executable name
    char *last_slash = strrchr(exec_path, '/');
    if (last_slash == NULL)
    {
        dprintf(2, "Error: could not find the library path\n");
        exit(1);
    }
    *last_slash = '\0';
    // add the library path
    snprintf(path, sizeof(path), "%s/../library/libfuncheck.so", exec_path);
    free(exec_path);
    char *real_path = realpath(path, NULL);
    if (real_path == NULL)
    {
        dprintf(2, "Error: could not find the library path\n");
        exit(1);
    }
    return real_path;
}

t_setup_result general_setup(char **envp)
{
    char *memory_name = generate_memory_name();
    char *env_memory_name = generate_env_string(ENV_MEMORY_NAME, memory_name);
    char *lib_path = get_library_path();
    char *env_shared_library = generate_env_string("LD_PRELOAD", lib_path);
    free(lib_path);
    char **new_envp = generate_envp(envp, env_memory_name, env_shared_library);
    t_shared_info *shared_memory = generate_shared_memory(memory_name);

    t_setup_result result = {
        .memory_name = memory_name,
        .env_memory_name = env_memory_name,
        .env_shared_library = env_shared_library,
        .new_envp = new_envp,
        .shared_memory = shared_memory};
    return result;
}

void free_setup_result(t_setup_result result)
{
    free_shared_memory(result.memory_name, result.shared_memory);
    free(result.memory_name);
    free(result.env_memory_name);
    free(result.env_shared_library);
    free(result.new_envp);
}