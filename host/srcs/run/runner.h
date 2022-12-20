#ifndef RUNNER_H
#define RUNNER_H

#include "../shared_memory/shared_memory.h"

typedef struct s_run_info
{
	int argc;
	char **argv;
	char **envp;
	t_shared_info *shared_info;
	int pipe_to_stdin[2];
	int pipe_to_stdout[2];
	int pipe_to_stderr[2];
} t_run_info;

# define NO_PIPE_FD -1
# define NO_PIPE {NO_PIPE_FD, NO_PIPE_FD}
# define COPY_PIPE(pipe) {pipe[0], pipe[1]}

int run(t_run_info *run_info);

#endif