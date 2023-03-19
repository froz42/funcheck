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
#include "../record_io/record_io.h"
#include "../config/config.h"
#include "../output/output.h"
#include "../time/time.h"
#include "../logs/logs.h"

static int _argc = 0;
static char **_argv = NULL;
static char **_envp = NULL;
static FILE *_tmpfile_stdin = NULL;
static size_t _allocation_test_count;
static size_t _allocation_test_total_size;
static t_symbolizer *_symbolizer = NULL;
static size_t _should_exit_fail = 0;

static void config_shared_memory_test(
    t_shared_info *shared_infos,
    t_function_call_footprint *allocation_info)
{
    config_t *config = get_config();
    shared_infos->treat_abort_as_crash = is_option_set(ABORT_AS_CRASH_MASK, config);
    shared_infos->runtype = RUNTYPE_TEST;
    sem_destroy(&shared_infos->lock_host);
    sem_destroy(&shared_infos->lock_guest);
    sem_init(&shared_infos->lock_host, 1, 0);
    sem_init(&shared_infos->lock_guest, 1, 0);
    // copy backtrace
    for (size_t i = 0; i < allocation_info->backtrace[i].address; i++)
        shared_infos->backtrace[i] = allocation_info->backtrace[i].address;
}

void test_allocation(t_function_call_footprint *allocation_info)
{
    btree_t_function_call_footprint *function_tree = NULL;
    int stdin_pipe[2];
    int stdout_pipe[2];
    int stderr_pipe[2];

    if (!allocation_info->should_test)
        return;
    _allocation_test_count++;
    timeval_t start_time = get_time();
    const config_t *config = get_config();
    char record_output_enabled =
        !is_option_set(ALL_OUTPUT_MASK, config) || is_option_set(JSON_OUTPUT_MASK, config);
    if (pipe(stdin_pipe) == -1)
        log_fatal("test_allocation: pipe failed", true);
    if (record_output_enabled)
    {
        if (pipe(stdout_pipe) == -1)
            log_fatal("test_allocation: pipe failed", true);
        if (pipe(stderr_pipe) == -1)
            log_fatal("test_allocation: pipe failed", true);
    }
    else
    {
        stdout_pipe[0] = NO_FD;
        stdout_pipe[1] = NO_FD;
        stderr_pipe[0] = NO_FD;
        stderr_pipe[1] = NO_FD;
    }

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

    FILE *output_tmpfile = NULL;

    if (record_output_enabled)
    {
        output_tmpfile = tmpfile();
        if (output_tmpfile == NULL)
            log_fatal("test_allocation: tmpfile failed", true);
    }

    t_record_io record_stdout = {
        .fd_to_read = stdout_pipe[0],
        .fd_to_write = NO_FD,
        .tmp_file_store = output_tmpfile,
    };

    t_record_io record_stderr = {
        .fd_to_read = stderr_pipe[0],
        .fd_to_write = NO_FD,
        .tmp_file_store = output_tmpfile,
    };

    if (record_output_enabled)
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
        if (record_output_enabled)
        {
            stop_record(&record_stdout);
            stop_record(&record_stderr);
        }
        exit(EXIT_FAILURE);
    }
    close(stdin_pipe[0]);
    close(stdout_pipe[0]);
    close(stderr_pipe[0]);
    write_record_to_fd(stdin_pipe[1], _tmpfile_stdin);
    close(stdin_pipe[1]);
    if (record_output_enabled)
    {
        close(stdout_pipe[1]);
        close(stderr_pipe[1]);
    }
    int status = 0;
    if (waitpid(ret, &status, 0) < 0)
        log_fatal("test_allocation: waitpid failed", true);
    stop_handle_events(event_thread, setup_result.shared_memory);
    if (record_output_enabled)
    {
        stop_record(&record_stdout);
        stop_record(&record_stderr);
        rewind(output_tmpfile);
    }

    const char *crash_name = NULL;
    t_address_info *crash_backtrace = NULL;
    if (setup_result.shared_memory->event == CRASH)
    {
        _should_exit_fail = 1;
        crash_name = setup_result.shared_memory->function_name;
        crash_backtrace = backtrace_process(
            NULL,
            _symbolizer,
            setup_result.shared_memory->backtrace);
    }

    t_test_result_display result = {
        .function_name = allocation_info->function_name,
        .crash_name = crash_name,
        .tmpfile_output = output_tmpfile,
        .crash_backtrace = crash_backtrace,
        .function_backtrace = allocation_info->backtrace,
        .function_tree = function_tree,
        .exit_code = WEXITSTATUS(status),
        .time = get_timelapse(start_time),
        .actual_test = _allocation_test_count,
        .total_tests = _allocation_test_total_size,
    };
    write_test_result(&result, _allocation_test_count == _allocation_test_total_size);
    if (record_output_enabled)
        fclose(output_tmpfile);
    clear_functions(&function_tree);
    free_setup_result(setup_result);
}



int allocations_test(
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
    _tmpfile_stdin = fetch_result->tmpfile_stdin;
    _allocation_test_count = 0;
    _allocation_test_total_size = count_testable_functions(fetch_result->function_tree);
    btree_t_function_call_footprint_foreach(fetch_result->function_tree, test_allocation);
    fclose(_tmpfile_stdin);
    return _should_exit_fail;
}