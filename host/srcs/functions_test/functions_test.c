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

#include <unistd.h>
#include <sys/wait.h>
#include "../function_footprint/function_footprint.h"
#include "../functions_fetch/functions_fetch.h"
#include "../run/runner.h"
#include "../logs/logs.h"
#include "../events/handle_event.h"
#include "../run/runner.h"
#include "../output/output.h"
#include "../time/time.h"
#include "functions_test.h"

static args_t _arg_guest;
static char **_envp;
static t_symbolizer *_symbolizer;
static FILE *_tmpfile_stdin;
static size_t _function_test_count;
static size_t _function_test_total_size;
static bool_t _function_test_fail;

/**
 * @brief Config the memory to test the functions
 *
 * @param shared_infos the shared memory between host and lib
 * @param backtrace the backtrace of the function to test
 * @param config the config of the program
 */
static void config_shared_memory_test(
    t_shared_info *shared_infos,
    t_address_info *backtrace,
    const config_t *config)
{
    shared_infos->treat_abort_as_crash = is_option_set(ABORT_AS_CRASH_MASK, config);
    shared_infos->runtype = RUNTYPE_TEST;
    sem_destroy(&shared_infos->lock_host);
    sem_destroy(&shared_infos->lock_guest);
    sem_init(&shared_infos->lock_host, 1, 0);
    sem_init(&shared_infos->lock_guest, 1, 0);
    // copy backtrace
    for (size_t i = 0; i < backtrace[i].address; i++)
        shared_infos->backtrace[i] = backtrace[i].address;
}

/**
 * @brief Set the up the record io objects for the test
 *
 * @param pipes the pipes
 * @param record_output_enabled if the output should be recorded
 * @return t_records the record io objects
 */
static t_records setup_record_io(t_pipes *pipes, bool_t record_output_enabled)
{
    t_records records;

    records.record_stdout = init_record_io(pipes->stdout_pipe[0], NO_FD);
    records.record_stderr = init_record_io(pipes->stderr_pipe[0], NO_FD);

    FILE *tmpfile_output = NULL;

    if (record_output_enabled)
    {
        tmpfile_output = tmpfile();
        if (tmpfile_output == NULL)
            log_fatal("setup_record_io: tmpfile failed", true);
    }
    records.record_stdout.tmp_file_store = tmpfile_output;
    records.record_stderr.tmp_file_store = tmpfile_output;
    return records;
}

/**
 * @brief Set the up pipes for the test
 *
 * @param record_output_enabled if the output should be recorded
 * @return t_pipes the pipes
 */
static t_pipes setup_pipes(bool_t record_output_enabled)
{
    t_pipes pipes_fetch;
    if (pipe(pipes_fetch.stdin_pipe) == -1)
        log_fatal("functions_fetch: pipe failed", true);

    if (record_output_enabled)
    {
        if (pipe(pipes_fetch.stdout_pipe) == -1)
            log_fatal("functions_fetch: pipe failed", true);
        if (pipe(pipes_fetch.stderr_pipe) == -1)
            log_fatal("functions_fetch: pipe failed", true);
    }
    else
    {
        pipes_fetch.stdout_pipe[0] = NO_FD;
        pipes_fetch.stdout_pipe[1] = NO_FD;
        pipes_fetch.stderr_pipe[0] = NO_FD;
        pipes_fetch.stderr_pipe[1] = NO_FD;
    }
    return pipes_fetch;
}

/**
 * @brief Launch the record of fds
 *
 * @param records the record io objects
 * @param record_output_enabled if the output should be recorded
 */
static void launch_records(t_records *records, const bool_t record_output_enabled)
{
    if (record_output_enabled)
    {
        launch_record(&records->record_stdout);
        launch_record(&records->record_stderr);
    }
}

/**
 * @brief Stop the record of fds
 *
 * @param records the record io objects
 * @param record_output_enabled if the output should be recorded
 */
static void stop_records(t_records *records, const bool_t record_output_enabled)
{
    if (record_output_enabled)
    {
        stop_record(&records->record_stdout);
        stop_record(&records->record_stderr);
    }
}

/**
 * @brief Close the unused pipes by the parent
 *
 * @param pipes the pipes
 * @param record_output_enabled if the output should be recorded
 */
static void close_unused_pipes(t_pipes *pipes, bool_t record_output_enabled)
{
    close(pipes->stdin_pipe[0]);
    if (record_output_enabled)
    {
        close(pipes->stdout_pipe[0]);
        close(pipes->stderr_pipe[0]);
    }
}

/**
 * @brief Once the test is done, close the remaining pipes
 *
 * @param pipes the pipes
 * @param record_output_enabled if the output should be recorded
 */
static void close_remaining_pipes(t_pipes *pipes, bool_t record_output_enabled)
{
    if (record_output_enabled)
    {
        close(pipes->stdout_pipe[1]);
        close(pipes->stderr_pipe[1]);
    }
}

/**
 * @brief Run the function test
 *
 * @param params Handle event params
 * @param records the record io objects
 * @param run_infos the run infos
 * @param pipes the pipes
 * @param record_output_enabled if the output should be recorded
 * @return int the status of the test
 */
static int run_function_test(
    t_handle_event_params *params,
    t_records *records,
    t_run_info *run_infos,
    t_pipes *pipes,
    bool_t record_output_enabled)
{
    pthread_t event_thread = launch_handle_events(params);
    launch_records(records, record_output_enabled);

    int status = 0;
    pid_t pid = run(run_infos);
    close_unused_pipes(pipes, record_output_enabled);
    write_record_to_fd(pipes->stdin_pipe[1], _tmpfile_stdin);
    close(pipes->stdin_pipe[1]);
    if (waitpid(pid, &status, 0) < 0)
        log_fatal("run_function_test: waitpid failed", true);
    stop_handle_events(event_thread, params->shared_memory);
    stop_records(records, record_output_enabled);
    close_remaining_pipes(pipes, record_output_enabled);
    return WEXITSTATUS(status);
}

/**
 * @brief Clear resources used by the function test
 *
 * @param output_tmpfile the tmpfile used to store the output
 * @param function_tree the function tree
 * @param runner_setup the runner setup
 * @param crash_info the crash info
 */
static void clear_function_test(
    FILE *output_tmpfile,
    btree_t_function_call_footprint **function_tree,
    t_runner_setup *runner_setup,
    t_crash_info *crash_info)
{
    if (output_tmpfile)
        fclose(output_tmpfile);
    clear_functions(function_tree);
    free_runner_setup(runner_setup);
    free(crash_info->backtrace);
}

/**
 * @brief Test a function
 * 
 * @param function_info the function info
 */
static void function_test(t_function_call_footprint *function_info)
{
    if (!function_info->should_test)
        return;
    _function_test_count++;
    timeval_t start_time = get_time();
    const config_t *config = get_config();
    btree_t_function_call_footprint *function_tree = NULL;
    bool_t record_output_enabled =
        !is_option_set(ALL_OUTPUT_MASK, config) || is_option_set(JSON_OUTPUT_MASK, config);
    t_pipes pipes = setup_pipes(record_output_enabled);
    t_records records = setup_record_io(&pipes, record_output_enabled);
    t_runner_setup runner_setup = setup_runner(_envp);
    config_shared_memory_test(
        runner_setup.shared_memory,
        function_info->backtrace, config);
    t_handle_event_params params = {
        .function_tree = &function_tree,
        .shared_memory = runner_setup.shared_memory,
        .symbolizer = _symbolizer,
    };
    t_run_info run_infos = {
        .argc = _arg_guest.argc,
        .argv = _arg_guest.argv,
        .envp = runner_setup.new_envp,
        .pipe_to_stdin = COPY_PIPE(pipes.stdin_pipe),
        .pipe_to_stdout = COPY_PIPE(pipes.stdout_pipe),
        .pipe_to_stderr = COPY_PIPE(pipes.stderr_pipe),
        .shared_info = runner_setup.shared_memory,
    };
    int status = run_function_test(
        &params,
        &records,
        &run_infos,
        &pipes,
        record_output_enabled);
    t_crash_info crash_infos = get_crash_infos(runner_setup.shared_memory, _symbolizer);
    if (crash_infos.crash_name)
        _function_test_fail++;
    t_test_result_display result_display = {
        .function_name = function_info->function_name,
        .crash_name = crash_infos.crash_name,
        .tmpfile_output = records.record_stdout.tmp_file_store,
        .crash_backtrace = crash_infos.backtrace,
        .function_backtrace = function_info->backtrace,
        .function_tree = function_tree,
        .exit_code = status,
        .time = get_timelapse(start_time),
        .actual_test = _function_test_count,
        .total_tests = _function_test_total_size,
    };
    write_test_result(&result_display, _function_test_count == _function_test_total_size);
    clear_function_test(
        records.record_stdout.tmp_file_store,
        &function_tree,
        &runner_setup,
        &crash_infos);
}

/**
 * @brief Test all functions
 * 
 * @param arg_guest the arguments of the guest program
 * @param envp the environment of the guest program
 * @param fetch_result the fetch result
 * @param symbolizer the symbolizer
 * @return function_tests_result_t the number of failed tests, the number
 *  of total tests and the time
 */
function_tests_result_t functions_test(
    args_t arg_guest,
    char **envp,
    t_fetch_result *fetch_result,
    t_symbolizer *symbolizer)
{
    timeval_t start;
    msseconds_t _lapse;
    start = get_time();
    _arg_guest = arg_guest;
    _envp = envp;
    _symbolizer = symbolizer;
    _tmpfile_stdin = fetch_result->tmpfile_stdin;
    _function_test_count = 0;
    _function_test_fail = 0;
    _function_test_total_size = count_testable_functions(fetch_result->function_tree);
    btree_t_function_call_footprint_foreach(fetch_result->function_tree, function_test);
    _lapse = get_timelapse(start);
    function_tests_result_t result = {
        .nb_total_tests = _function_test_total_size,
        .nb_failed_tests = _function_test_fail,
        .time_laps = _lapse,
        };
    return result;
}