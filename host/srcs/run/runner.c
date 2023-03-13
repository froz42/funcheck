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
	{
		// log_error("fork failed", true);
		exit(1);
	}

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
			log_fatal("run: execvpe failed", true);
			run_info->shared_info->event = EXIT;
			sem_post(&run_info->shared_info->lock_host);
			exit(1);
		}
	}
	return pid;
}