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