#ifndef RUNNER_H
#define RUNNER_H

#include "../shared_memory/shared_memory.h"
#include "../config/config.h"
#include "../backtrace/backtrace.h"

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

typedef struct
{
    char *memory_name;
    char *env_memory_name;
    char *env_shared_library;
    char **new_envp;
    t_shared_info *shared_memory;
} t_runner_setup;

typedef struct
{
    int stdin_pipe[2];
    int stdout_pipe[2];
    int stderr_pipe[2];
} t_pipes;

typedef struct
{
    const char *crash_name;
    t_address_info *backtrace;
} t_crash_info;

# define NO_FD -1
# define NO_PIPE {NO_FD, NO_FD}
# define COPY_PIPE(pipe) {pipe[0], pipe[1]}

t_runner_setup setup_runner(char **envp);
void free_runner_setup(t_runner_setup *result);
int run(t_run_info *run_info);
t_crash_info get_crash_infos(
    t_shared_info *shared_memory,
    t_symbolizer *symbolizer);

#endif