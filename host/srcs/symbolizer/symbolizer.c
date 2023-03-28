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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <signal.h>
#include "../function_footprint/function_footprint.h"
#include "../path/path.h"
#include "parse_output/parse_output.h"
#include "symbolizer.h"
#include "../logs/logs.h"

typedef struct s_symbolizer_config
{
    const char *symbolizer_name;
    const char *symbolizer_options;
    void (*parse_output)(FILE *file, t_address_info *address_info_ptr);
} t_symbolizer_config;

/**
 * @brief This function will return the best symbolizer available on the system.
 *
 * @return a symbolizer config
 */
static const t_symbolizer_config *get_symbolizer_config(void)
{
    static const t_symbolizer_config symbolizer_config[] = {
        {"llvm-symbolizer", "-f -e %s --relativenames", parse_llvm_symbolizer_output},
        {"llvm-symbolizer-12", "-f -e %s --relativenames", parse_llvm_symbolizer_output},
        {"addr2line", "-f -e %s", parse_addr2line_output},
        {NULL, NULL, NULL},
    };

    for (int i = 0; symbolizer_config[i].symbolizer_name; i++)
    {
        if (is_program_in_path(symbolizer_config[i].symbolizer_name))
            return &symbolizer_config[i];
    }
    log_fatal("No symbolizer found", false);
    return NULL; /* never reached */
}

/**
 * @brief This function will generate the argv array for the symbolizer.
 * 
 * @param program_name symbolizer name
 * @param options symbolizer options
 * @return char** the argv array (must be freed)
 */
static char **generate_argv(const char *program_name, char *options)
{
    // tokenize options
    size_t count = 0;
    char *options_ptr;

    options_ptr = options;
    while ((options_ptr = strchr(options_ptr, ' ')) != NULL)
    {
        count++;
        options_ptr++;
    }
    char **argv = malloc(sizeof(char *) * (count + 3));
    if (argv == NULL)
        log_fatal("generate_argv: malloc failed", true);
        

    char *copy_options = strdup(options);

    size_t i = 0;
    argv[i++] = strdup(program_name);
    char *token = strtok(copy_options, " ");
    while (token)
    {
        argv[i] = strdup(token);
        token = strtok(NULL, " ");
        i++;
    }
    argv[i] = NULL;
    return argv;
}

/**
 * @brief This function will initialize the symbolizer.
 * 
 * @param program_path the path of the program to symbolize
 * @return t_symbolizer 
 */
t_symbolizer symbolizer_init(char *program_path)
{
    char options[1024] = {0};
    const t_symbolizer_config *symbolizer_config = get_symbolizer_config();

    snprintf(
        options,
        sizeof(options),
        symbolizer_config->symbolizer_options,
        program_path);

    int pipe_stdin[2];
    int pipe_stdout[2];

    if (pipe(pipe_stdin) == -1)
        log_fatal("symbolizer_init: pipe", true);
    if (pipe(pipe_stdout) == -1)
        log_fatal("symbolizer_init: pipe", true);

    pid_t pid = fork();

    if (pid == -1)
        log_fatal("symbolizer_init: fork", true);
    else if (pid == 0)
    {
        char **argv = generate_argv(symbolizer_config->symbolizer_name, options);

        close(pipe_stdin[1]);
        close(pipe_stdout[0]);
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        dup2(pipe_stdin[0], STDIN_FILENO);
        dup2(pipe_stdout[1], STDOUT_FILENO);
        close(pipe_stdin[0]);
        close(pipe_stdout[1]);

        execvp(
            symbolizer_config->symbolizer_name,
            argv);
        log_fatal("symbolizer_init: execvp", true);
    }
    close(pipe_stdin[0]);
    close(pipe_stdout[1]);

    t_symbolizer new_symbolizer = {
        .pid_symbolizer = pid,
        .read_file = fdopen(pipe_stdout[0], "r"),
        .write_fd = pipe_stdin[1],
        .parse_output = symbolizer_config->parse_output,
    };
    if (new_symbolizer.read_file == NULL)
        log_fatal("symbolizer_init: fdopen", true);
    return new_symbolizer;
}

/**
 * @brief Stop the symbolizer.
 * 
 * @param symbolizer the symbolizer to stop
 */
void symbolizer_stop(t_symbolizer *symbolizer)
{
    fclose(symbolizer->read_file);
    close(symbolizer->write_fd);
    waitpid(symbolizer->pid_symbolizer, NULL, 0);
}


/**
 * @brief This function will get the address info from the symbolizer.
 * 
 * @param symbolizer the symbolizer
 * @param address_list the address list to symbolize
 * @param address_infos the symbolized address list
 */
void symbolizer_get_address_info(
    t_symbolizer *symbolizer,
    const ptr_address *address_list,
    t_address_info *address_infos)
{
    size_t i = 0;
    for (; address_list[i]; i++)
    {
        dprintf(symbolizer->write_fd, "%#zx\n", address_list[i]);
        t_address_info *address_info_ptr = &address_infos[i];
        address_info_ptr->address = (unsigned long)address_list[i];
        symbolizer->parse_output(symbolizer->read_file, address_info_ptr);
    }
    address_infos[i].address = 0x0;
}