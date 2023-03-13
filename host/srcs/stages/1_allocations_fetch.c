#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include "stages.h"
#include "../function_footprint/function_footprint.h"
#include "../run/runner.h"
#include "../events/event_utils.h"
#include "../record_io/record_io.h"
#include "../events/handle_event.h"
#include "../config/config.h"
#include "../output/output.h"
#include "../backtrace/backtrace.h"
#include "../logs/logs.h"

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
    const config_t *config = get_config();
    const char is_json_output = is_option_set(JSON_OUTPUT_MASK, config);
    t_fetch_result result = {0};
    t_setup_result setup_result = general_setup(envp);
    int stdin_pipe[2];
    int stdout_pipe[2];
    int stderr_pipe[2];

    if (pipe(stdin_pipe) == -1)
        log_fatal("allocations_fetch: pipe failed", true);

    if (is_json_output)
    {
        if (pipe(stdout_pipe) == -1)
            log_fatal("allocations_fetch: pipe failed", true);
        if (pipe(stderr_pipe) == -1)
            log_fatal("allocations_fetch: pipe failed", true);
    }
    else
    {
        stdout_pipe[0] = NO_FD;
        stdout_pipe[1] = NO_FD;
        stderr_pipe[0] = NO_FD;
        stderr_pipe[1] = NO_FD;
    }
    config_shared_memory_fetch(setup_result.shared_memory);

    t_run_info run_infos = {
        .argc = argc,
        .argv = argv,
        .envp = setup_result.new_envp,
        .shared_info = setup_result.shared_memory,
        .pipe_to_stdin = COPY_PIPE(stdin_pipe),
        .pipe_to_stdout = COPY_PIPE(stdout_pipe),
        .pipe_to_stderr = COPY_PIPE(stderr_pipe)};

    FILE *tmpfile_output = NULL;

    if (is_json_output)
    {
        tmpfile_output = tmpfile();
        if (tmpfile_output == NULL)
            log_fatal("allocations_fetch: tmpfile failed", true);
    }

    result.tmpfile_stdin = tmpfile();
    if (result.tmpfile_stdin == NULL)
        log_fatal("allocations_fetch: tmpfile failed", true);

    t_record_io record_stdin = {
        .fd_to_read = STDIN_FILENO,
        .fd_to_write = stdin_pipe[1],
        .tmp_file_store = result.tmpfile_stdin};

    t_record_io record_stdout = {
        .fd_to_read = stdout_pipe[0],
        .fd_to_write = NO_FD,
        .tmp_file_store = tmpfile_output};

    t_record_io record_stderr = {
        .fd_to_read = stderr_pipe[0],
        .fd_to_write = NO_FD,
        .tmp_file_store = tmpfile_output};

    launch_record(&record_stdin);
    if (is_json_output)
    {
        launch_record(&record_stdout);
        launch_record(&record_stderr);
    }

    t_handle_event_params params = {
        .function_tree = &result.function_tree,
        .shared_memory = setup_result.shared_memory,
        .symbolizer = symbolizer,
    };

    pthread_t event_thread = launch_handle_events(&params);

    int ret = run(&run_infos);
    int status = 0;
    if (waitpid(ret, &status, 0) == -1)
        log_fatal("allocations_fetch: waitpid failed", true);
    stop_handle_events(event_thread, setup_result.shared_memory);
    stop_record(&record_stdin);
    if (is_json_output)
    {
        stop_record(&record_stdout);
        stop_record(&record_stderr);
        rewind(tmpfile_output);
    }
    const char *crash_name = NULL;
    t_address_info *backtrace = NULL;
    if (setup_result.shared_memory->event == CRASH)
    {
        crash_name = setup_result.shared_memory->function_name;
        backtrace = backtrace_process(
            NULL,
            symbolizer,
            setup_result.shared_memory->backtrace);
    }
    t_fetch_result_display result_display = {
        .function_tree = result.function_tree,
        .tmpfile_output = tmpfile_output,
        .crash_name = crash_name,
        .backtrace = backtrace,
        .exit_code = WEXITSTATUS(status)};
    write_function_fetch_result(&result_display);
    if (crash_name != NULL)
        exit(EXIT_FAILURE);

    if (is_json_output)
        fclose(tmpfile_output);
    free_setup_result(setup_result);
    return result;
}

void clear_fetch_result(t_fetch_result *result)
{
    clear_functions(&result->function_tree);
}