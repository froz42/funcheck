#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include "stages.h"
#include "../function_footprint/function_footprint.h"
#include "../run/runner.h"
#include "../events/event_utils.h"
#include "../events/handle_event.h"
#include "../leak_check/leak_check.h"
#include "../record_io/record_io.h"

static int _argc = 0;
static char **_argv = NULL;
static char **_envp = NULL;
static char *_stdin_record = NULL;
static size_t _alloction_test_count;
static size_t _alloction_test_total_size;
static t_symbolizer *_symbolizer = NULL;
static size_t _should_exit_fail = 0;

#define RECORD_OUTPUT 1

static void config_shared_memory_test(
    t_shared_info *shared_infos,
    t_function_call_footprint *allocation_info)
{
    shared_infos->runtype = RUNTYPE_TEST;
    sem_destroy(&shared_infos->lock_host);
    sem_destroy(&shared_infos->lock_guest);
    sem_init(&shared_infos->lock_host, 1, 0);
    sem_init(&shared_infos->lock_guest, 1, 0);
    // copy backtrace
    for (size_t i = 0; i < allocation_info->backtrace[i].address; i++)
        shared_infos->backtrace[i] = allocation_info->backtrace[i].address;
}

/**
 * @brief Write the provided str to the fd, line by line with a maximum buffer of 2048
 *
 * @param fd the fd to write to
 * @param str the str to write
 */
static void write_to_fd(int fd, char *str)
{
    // we need to send the string line by line
    if (str == NULL)
        return;
    char *line = strtok(str, "\n");
    while (line)
    {
        size_t len = strlen(line);
        while (len > 0)
        {
            size_t to_write = len > 2048 ? 2048 : len;
            write(fd, line, to_write);
            line += to_write;
            len -= to_write;
        }
        line = strtok(NULL, "\n");
    }
}

void test_allocation(t_function_call_footprint *allocation_info)
{
    btree_t_function_call_footprint *function_tree = NULL;
    int stdin_pipe[2];
    int stdout_pipe[2];
    int stderr_pipe[2];

    if (pipe(stdin_pipe) == -1)
    {
        dprintf(2, "[ERROR] pipe failed: %s\n", strerror(errno));
        exit(1);
    }
    if (RECORD_OUTPUT)
    {
        if (pipe(stdout_pipe) == -1)
        {
            dprintf(2, "[ERROR] pipe failed: %s\n", strerror(errno));
            exit(1);
        }
        if (pipe(stderr_pipe) == -1)
        {
            dprintf(2, "[ERROR] pipe failed: %s\n", strerror(errno));
            exit(1);
        }
    }
    else
    {
        stdout_pipe[0] = NO_FD;
        stdout_pipe[1] = NO_FD;
        stderr_pipe[0] = NO_FD;
        stderr_pipe[1] = NO_FD;
    }
    printf(
        "test_function: %s: %zi/%zi\n",
        allocation_info->function_name,
        ++_alloction_test_count,
        _alloction_test_total_size);
    t_setup_result setup_result = general_setup(_envp);
    config_shared_memory_test(setup_result.shared_memory, allocation_info);
    t_run_info run_infos = {
        .argc = _argc,
        .argv = _argv,
        .envp = setup_result.new_envp,
        .shared_info = setup_result.shared_memory,
        .pipe_to_stdin = COPY_PIPE(stdin_pipe),
        .pipe_to_stdout = COPY_PIPE(stdout_pipe),
        .pipe_to_stderr = COPY_PIPE(stderr_pipe),
    };
    t_record_io record_stdout = {
        .fd_to_read = stdout_pipe[0],
        .fd_to_write = NO_FD,
        .record = NULL,
    };

    t_record_io record_stderr = {
        .fd_to_read = stderr_pipe[0],
        .fd_to_write = NO_FD,
        .record = NULL,
    };

    if (RECORD_OUTPUT)
    {
        launch_record(&record_stdout);
        launch_record(&record_stderr);
    }

    t_handle_event_params params = {
        .function_tree = &function_tree,
        .shared_memory = setup_result.shared_memory,
        .symbolizer = _symbolizer,
    };
    pthread_t event_thread = launch_handle_events(&params);

    int ret = run(&run_infos);
    if (ret < 0)
    {
        if (RECORD_OUTPUT)
        {
            force_stop_record(&record_stdout);
            force_stop_record(&record_stderr);
            free(record_stdout.record);
            free(record_stderr.record);
        }
        exit(EXIT_FAILURE);
    }
    close(stdin_pipe[0]);
    close(stdout_pipe[0]);
    close(stderr_pipe[0]);
    write_to_fd(stdin_pipe[1], _stdin_record);
    close(stdin_pipe[1]);
    if (RECORD_OUTPUT)
    {
        close(stdout_pipe[1]);
        close(stderr_pipe[1]);
    }
    if (waitpid(ret, NULL, 0) < 0)
    {
        if (RECORD_OUTPUT)
        {
            force_stop_record(&record_stdout);
            force_stop_record(&record_stderr);
            free(record_stdout.record);
            free(record_stderr.record);
        }
        clear_functions(&function_tree);
        exit(EXIT_FAILURE);
    }
    stop_handle_events(event_thread, setup_result.shared_memory);
    if (setup_result.shared_memory->event == CRASH)
    {
        if (RECORD_OUTPUT)
        {
            force_stop_record(&record_stdout);
            force_stop_record(&record_stderr);
            printf("stderr: \n'\n%s'\n", record_stderr.record);
            printf("stdout: \n'\n%s'\n", record_stdout.record);
            free(record_stdout.record);
            free(record_stderr.record);
            _should_exit_fail = 1;
        }
        clear_functions(&function_tree);
        free_setup_result(setup_result);
        return;
    }
    if (RECORD_OUTPUT)
    {
        force_stop_record(&record_stdout);
        force_stop_record(&record_stderr);
        free(record_stdout.record);
        free(record_stderr.record);
    }
    check_leaks(function_tree);
    clear_functions(&function_tree);
    free_setup_result(setup_result);
}

void allocation_test(
    int argc,
    char **argv,
    char **envp,
    t_fetch_result *fetch_result,
    t_symbolizer *symbolizer)
{
    _argc = argc;
    _argv = argv;
    _envp = envp;
    _symbolizer = symbolizer;
    _stdin_record = fetch_result->stdin_record;
    _alloction_test_count = 0;
    _alloction_test_total_size = btree_t_function_call_footprint_size(fetch_result->function_tree);
    btree_t_function_call_footprint_foreach(fetch_result->function_tree, test_allocation);
    if (_should_exit_fail)
        exit(EXIT_FAILURE);
}