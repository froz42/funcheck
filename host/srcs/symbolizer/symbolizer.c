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
        {"llvm-symbolizer-14", "-f -e %s --relativenames", parse_llvm_symbolizer_output},
        {"llvm-symbolizer-13", "-f -e %s --relativenames", parse_llvm_symbolizer_output},
        {"llvm-symbolizer-12", "-f -e %s --relativenames", parse_llvm_symbolizer_output},
        {"llvm-symbolizer-11", "-f -e %s --relativenames", parse_llvm_symbolizer_output},
        {"llvm-symbolizer-10", "-f -e %s --relativenames", parse_llvm_symbolizer_output},
        {"llvm-symbolizer-9", "-f -e %s --relativenames", parse_llvm_symbolizer_output},
        {"llvm-symbolizer-8", "-f -e %s --relativenames", parse_llvm_symbolizer_output},
        {"addr2line", "-f -e %s", parse_addr2line_output},
        {NULL, NULL, NULL},
    };

    for (int i = 0; symbolizer_config[i].symbolizer_name; i++)
    {
        if (is_program_in_path(symbolizer_config[i].symbolizer_name))
            return (t_symbolizer_config *)&symbolizer_config[i];
    }
    exit(EXIT_FAILURE);
}

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
    {
        dprintf(STDERR_FILENO, "Malloc failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

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
    {
        dprintf(2, "error: pipe: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (pipe(pipe_stdout) == -1)
    {
        dprintf(2, "error: pipe: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1)
    {
        dprintf(2, "error: fork: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
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
        for (size_t i = 0; argv[i]; i++)
            free(argv[i]);
        free(argv);
        dprintf(2, "error: execvp: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    close(pipe_stdin[0]);
    close(pipe_stdout[1]);

    t_symbolizer new_symbolizer = {
        .pid_symbolizer = pid,
        .read_file = fdopen(pipe_stdout[0], "r"),
        .write_fd = pipe_stdin[1],
        .parse_output = symbolizer_config->parse_output,
    };
    return new_symbolizer;
}

void symbolizer_stop(t_symbolizer *symbolizer)
{
    fclose(symbolizer->read_file);
    close(symbolizer->write_fd);
    waitpid(symbolizer->pid_symbolizer, NULL, 0);
}

void symbolizer_get_address_info(
    t_symbolizer *symbolizer,
    const ptr_address *address_list,
    t_address_info *address_infos)
{
    for (size_t i = 0; address_list[i]; i++)
    {
        dprintf(symbolizer->write_fd, "%#zx\n", address_list[i]);
        t_address_info *address_info_ptr = &address_infos[i];
        address_info_ptr->address = (unsigned long)address_list[i];
        symbolizer->parse_output(symbolizer->read_file, address_info_ptr);
    }
}