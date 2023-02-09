#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include "stages.h"
#include "../function_footprint/function_footprint.h"
#include "../run/runner.h"
#include "../events/event_utils.h"
#include "../allocations_summary/allocations_summary.h"
#include "../record_io/record_io.h"
#include "../events/handle_event.h"
#include "../config/config.h"

/**
 * @brief Config the memory to fetch the allocation
 *
 * @param shared_info
 */
static void config_shared_memory_fetch(t_shared_info *shared_info)
{
    config_t *config = get_config();
    shared_info->treat_abort_as_crash = is_option_set(ABORT_AS_CRASH_MASK, config);
    shared_info->runtype = RUNTYPE_FETCH;
    sem_init(&shared_info->lock_host, 1, 0);
    sem_init(&shared_info->lock_guest, 1, 0);
}

t_fetch_result allocations_fetch(
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

    result.tmpfile_stdin = tmpfile();
    if (result.tmpfile_stdin == NULL)
    {
        dprintf(2, "[ERROR] tmpfile failed\n");
        exit(1);
    }

    t_record_io record_stdin = {
        .fd_to_read = STDIN_FILENO,
        .fd_to_write = stdin_pipe[1],
        .tmp_file_store = result.tmpfile_stdin};

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
        stop_record(&record_stdin);
        exit(EXIT_FAILURE);
    }
    if (waitpid(ret, NULL, 0) < 0)
    {
        stop_record(&record_stdin);
        exit(EXIT_FAILURE);
    }
    stop_handle_events(event_thread, setup_result.shared_memory);
    if (setup_result.shared_memory->event == CRASH)
    {
        stop_record(&record_stdin);
        clear_functions(&result.function_tree);
        exit(EXIT_FAILURE);
    }
    stop_record(&record_stdin);
    const config_t *config = get_config();
    if (is_option_set(TRACK_ALLOCATIONS_MASK, config))
        allocations_summary(result.function_tree);
    free_setup_result(setup_result);
    return result;
}

void clear_fetch_result(t_fetch_result *result)
{
    clear_functions(&result->function_tree);
}