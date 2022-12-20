#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include "stages.h"
#include "../function_footprint/function_footprint.h"
#include "../run/runner.h"
#include "../events/event_utils.h"
#include "../leak_check/leak_check.h"
#include "../record_io/record_io.h"
#include "../events/handle_event.h"

/**
 * @brief Config the memory to fetch the allocation
 *
 * @param shared_info
 */
static void config_shared_memory_fetch(t_shared_info *shared_info)
{
    shared_info->runtype = RUNTYPE_FETCH;
    sem_init(&shared_info->lock_host, 1, 0);
    sem_init(&shared_info->lock_guest, 1, 0);
}

t_fetch_result allocation_fetch(
    int argc,
    char **argv,
    char **envp,
    t_symbolizer *symbolizer)
{
    t_fetch_result result = {0};
    t_setup_result setup_result = general_setup(envp);
    int stdin_pipe[2];

    if (pipe(stdin_pipe) == -1)
    {
        dprintf(2, "[ERROR] pipe failed\n");
        exit(1);
    }
    config_shared_memory_fetch(setup_result.shared_memory);

    t_run_info run_infos = {
        .argc = argc,
        .argv = argv,
        .envp = setup_result.new_envp,
        .shared_info = setup_result.shared_memory,
        .pipe_to_stdin = COPY_PIPE(stdin_pipe),
        .pipe_to_stdout = NO_PIPE,
        .pipe_to_stderr = NO_PIPE,
    };

    t_record_io record_stdin = {
        .fd_to_read = STDIN_FILENO,
        .fd_to_write = stdin_pipe[1],
        .record = NULL,
    };

    launch_record(&record_stdin);

    t_handle_event_params params = {
        .function_tree = &result.function_tree,
        .shared_memory = setup_result.shared_memory,
        .symbolizer = symbolizer,
    };

    pthread_t event_thread = launch_handle_events(&params);

    int ret = run(&run_infos);
    close(stdin_pipe[0]);
    if (ret < 0)
    {
        force_stop_record(&record_stdin);
        free(record_stdin.record);
        exit(EXIT_FAILURE);
    }
    if (waitpid(ret, NULL, 0) < 0)
    {
        force_stop_record(&record_stdin);
        free(record_stdin.record);
        exit(EXIT_FAILURE);
    }
    stop_handle_events(event_thread, setup_result.shared_memory);
    if (setup_result.shared_memory->event == CRASH)
    {
        force_stop_record(&record_stdin);
        free(record_stdin.record);
        clear_functions(&result.function_tree);
        exit(EXIT_FAILURE);
    }
    force_stop_record(&record_stdin);
    result.stdin_record = record_stdin.record;
    check_leaks(result.function_tree);
    free_setup_result(setup_result);
    return result;
}

void clear_fetch_result(t_fetch_result *result)
{
    clear_functions(&result->function_tree);
    free(result->stdin_record);
}