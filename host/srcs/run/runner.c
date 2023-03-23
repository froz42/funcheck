#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include "../shared_memory/shared_memory.h"
#include "../run/runner.h"
#include "../logs/logs.h"
#include "../utils/bool.h"
#include "../env/env.h"

#define RELATIVE_LIBRARY_PATH "../library/libfuncheck.so"

/**
 * @brief Get the absolute path of the library
 * 
 * @return char* the absolute path of the library
 */
static char *get_library_path(void)
{
    static char path[1024];
    if (path[0] != '\0')
        return path;
    char *exec_path = realpath("/proc/self/exe", NULL);
    if (exec_path == NULL)
        log_fatal("Could not find the executable path", true);
    // remove the executable name
    char *last_slash = strrchr(exec_path, '/');
    if (last_slash == NULL)
        log_fatal("Could not find the executable path", true);
    *last_slash = '\0';
    // add the library path
    snprintf(path, sizeof(path), "%s/%s", exec_path, RELATIVE_LIBRARY_PATH);
    free(exec_path);
    char *real_path = realpath(path, NULL);
    if (real_path == NULL)
        log_fatal("Could not find the library path", true);
    strncpy(path, real_path, sizeof(path));
    free(real_path);
    return path;
}

t_runner_setup setup_runner(char **envp)
{
    char *memory_name = generate_memory_name();
    char *env_memory_name = generate_env_string(ENV_MEMORY_NAME, memory_name);
    char *lib_path = get_library_path();
    char *env_shared_library = generate_env_string("LD_PRELOAD", lib_path);
    char **new_envp = generate_envp(envp, env_memory_name, env_shared_library);
    t_shared_info *shared_memory = generate_shared_memory(memory_name);

    t_runner_setup result = {
        .memory_name = memory_name,
        .env_memory_name = env_memory_name,
        .env_shared_library = env_shared_library,
        .new_envp = new_envp,
        .shared_memory = shared_memory};
    return result;
}

void free_runner_setup(t_runner_setup *result)
{
    free_shared_memory(result->memory_name, result->shared_memory);
    free(result->memory_name);
    free(result->env_memory_name);
    free(result->env_shared_library);
    free(result->new_envp);
}

/**
 * @brief Run the program
 * 
 * @param run_info the run infos
 * @return int the return code of the program
 */
int run(t_run_info *run_info)
{
	pid_t pid = fork();

	if (pid < 0)
		log_fatal("run: fork failed", true);

	if (pid == 0)
	{
		// dup the end of the pipe to stdin
		if (run_info->pipe_to_stdin[0] != -1)
		{
			close(run_info->pipe_to_stdin[1]);
			dup2(run_info->pipe_to_stdin[0], STDIN_FILENO);
			close(run_info->pipe_to_stdin[0]);
		}
		if (run_info->pipe_to_stdout[1] != -1)
		{
			close(run_info->pipe_to_stdout[0]);
			dup2(run_info->pipe_to_stdout[1], STDOUT_FILENO);
			close(run_info->pipe_to_stdout[1]);
		}
		if (run_info->pipe_to_stderr[1] != -1)
		{
			close(run_info->pipe_to_stderr[0]);
			dup2(run_info->pipe_to_stderr[1], STDERR_FILENO);
			close(run_info->pipe_to_stderr[1]);
		}
		int ret = execvpe(run_info->argv[0], run_info->argv, run_info->envp);
		if (ret < 0)
		{
			run_info->shared_info->event = EXIT;
			sem_post(&run_info->shared_info->lock_host);
			log_fatal("run: execvpe failed", true);
		}
	}
	return pid;
}