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
#include "../config/config.h"
#include "../symbolizer/symbolizer.h"
#include "runner.h"

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

/**
 * @brief Generate resources for the runner
 * 
 * @param envp the environment variables
 * @return t_runner_setup the runner setup
 */
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

/**
 * @brief Free the resources of the runner
 * 
 * @param result the runner setup
 */
void free_runner_setup(t_runner_setup *result)
{
    free_shared_memory(result->memory_name, result->shared_memory);
    free(result->memory_name);
    free(result->env_memory_name);
    free(result->env_shared_library);
    free(result->new_envp);
}

/**
 * @brief Get the crash infos from the shared memory
 * 
 * @param shared_memory the shared memory between the host and the library 
 * @param symbolizer the symbolizer
 * @return t_crash_info the crash infos
 */
t_crash_info get_crash_infos(
    t_shared_info *shared_memory,
    t_symbolizer *symbolizer)
{
    t_crash_info crash_infos = {0};
    if (shared_memory->event == CRASH)
    {
        crash_infos.crash_name = shared_memory->function_name;
        crash_infos.backtrace = backtrace_process(
            NULL,
            symbolizer,
            shared_memory->backtrace);
    }
    return crash_infos;
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
		if (run_info->pipe_to_stdin[0] != NO_FD)
		{
			close(run_info->pipe_to_stdin[1]);
			dup2(run_info->pipe_to_stdin[0], STDIN_FILENO);
			close(run_info->pipe_to_stdin[0]);
		}
		if (run_info->pipe_to_stdout[1] != NO_FD)
		{
			close(run_info->pipe_to_stdout[0]);
			dup2(run_info->pipe_to_stdout[1], STDOUT_FILENO);
			close(run_info->pipe_to_stdout[1]);
		}
		if (run_info->pipe_to_stderr[1] != NO_FD)
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