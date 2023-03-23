#include "functions_fetch.h"
#include "../../../shared/shared.h"
#include "../config/config.h"
#include "../run/runner.h"
#include "../record_io/record_io.h"
#include "../events/handle_event.h"
#include "../output/output.h"
#include "../logs/logs.h"
#include <sys/wait.h>
#include <unistd.h>

typedef struct
{
    int stdin_pipe[2];
    int stdout_pipe[2];
    int stderr_pipe[2];
} t_pipes_fetch;

typedef struct
{
    t_record_io record_stdin;
    t_record_io record_stdout;
    t_record_io record_stderr;
} t_records_fetch;

typedef struct
{
    const char *crash_name;
    t_address_info *backtrace;
} t_crash_info;

/**
 * @brief Config the memory to fetch the functions
 *
 * @param shared_info the shared memory between host and guest
 */
static void config_shared_memory_fetch(t_shared_info *shared_info, const config_t *config)
{
    shared_info->treat_abort_as_crash = is_option_set(ABORT_AS_CRASH_MASK, config);
    shared_info->runtype = RUNTYPE_FETCH;
    sem_init(&shared_info->lock_host, 1, 0);
    sem_init(&shared_info->lock_guest, 1, 0);
}

static t_pipes_fetch setup_pipes(const config_t *config)
{
    t_pipes_fetch pipes_fetch;
    if (pipe(pipes_fetch.stdin_pipe) == -1)
        log_fatal("functions_fetch: pipe failed", true);

    if (is_option_set(JSON_OUTPUT_MASK, config))
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

void close_unused_pipes(const t_pipes_fetch *pipes, const config_t *config)
{
    close(pipes->stdin_pipe[0]);

    if (is_option_set(JSON_OUTPUT_MASK, config))
    {
        close(pipes->stdout_pipe[0]);
        close(pipes->stderr_pipe[0]);
    }
}

void close_remaining_pipes(const t_pipes_fetch *pipes, const config_t *config)
{
    close(pipes->stdin_pipe[1]);

    if (is_option_set(JSON_OUTPUT_MASK, config))
    {
        close(pipes->stdout_pipe[1]);
        close(pipes->stderr_pipe[1]);
    }
}

static void setup_tmp_files(FILE **tmpfile_output, FILE **tmpfile_stdin, const config_t *config)
{
    if (is_option_set(JSON_OUTPUT_MASK, config))
    {
        *tmpfile_output = tmpfile();
        if (*tmpfile_output == NULL)
            log_fatal("functions_fetch: tmpfile failed", true);
    }

    *tmpfile_stdin = tmpfile();
    if (*tmpfile_stdin == NULL)
        log_fatal("functions_fetch: tmpfile failed", true);
}

static t_records_fetch setup_record_io(t_pipes_fetch *pipes, const config_t *config)
{
    t_records_fetch records;

    records.record_stdin = init_record_io(STDIN_FILENO, pipes->stdin_pipe[1]);
    records.record_stdout = init_record_io(pipes->stdout_pipe[0], NO_FD);
    records.record_stderr = init_record_io(pipes->stderr_pipe[0], NO_FD);

    FILE *tmpfile_output = NULL;
    setup_tmp_files(&tmpfile_output, &records.record_stdin.tmp_file_store, config);
    records.record_stdout.tmp_file_store = tmpfile_output;
    records.record_stderr.tmp_file_store = tmpfile_output;
    return records;
}

static void launch_records(t_records_fetch *records, const config_t *config)
{
    launch_record(&records->record_stdin);
    if (is_option_set(JSON_OUTPUT_MASK, config))
    {
        launch_record(&records->record_stdout);
        launch_record(&records->record_stderr);
    }
}

static void stop_records(t_records_fetch *records, const config_t *config)
{
    stop_record(&records->record_stdin);
    if (is_option_set(JSON_OUTPUT_MASK, config))
    {
        stop_record(&records->record_stdout);
        stop_record(&records->record_stderr);
    }
}

static int run_function_fetch(
    t_handle_event_params *params,
    t_records_fetch *records,
    t_run_info *run_infos,
    t_pipes_fetch *pipes,
    const config_t *config)
{
    pthread_t event_thread = launch_handle_events(params);
    launch_records(records, config);

    int status = 0;
    int pid = run(run_infos);
    close_unused_pipes(pipes, config);
    if (waitpid(pid, &status, 0) == -1)
        log_fatal("allocations_fetch: waitpid failed", true);
    stop_handle_events(event_thread, run_infos->shared_info);
    stop_records(records, config);
    close_remaining_pipes(pipes, config);
    return WEXITSTATUS(status);
}

t_crash_info get_crash_infos(
    t_runner_setup *runner_setup,
    t_symbolizer *symbolizer)
{
    t_crash_info crash_infos = {0};
    if (runner_setup->shared_memory->event == CRASH)
    {
        crash_infos.crash_name = runner_setup->shared_memory->function_name;
        crash_infos.backtrace = backtrace_process(
            NULL,
            symbolizer,
            runner_setup->shared_memory->backtrace);
    }
    return crash_infos;
}

t_fetch_result functions_fetch(
    args_t *guest_args,
    char **envp,
    t_symbolizer *symbolizer)
{
    const config_t *config = get_config();

    t_pipes_fetch pipes = setup_pipes(config);

    t_fetch_result fetch_result = {0};
    t_records_fetch records = setup_record_io(&pipes, config);

    fetch_result.tmpfile_stdin = records.record_stdin.tmp_file_store;

    t_runner_setup runner_setup = setup_runner(envp);
    config_shared_memory_fetch(runner_setup.shared_memory, config);

    t_handle_event_params params = {
        .function_tree = &fetch_result.function_tree,
        .shared_memory = runner_setup.shared_memory,
        .symbolizer = symbolizer,
    };

    t_run_info run_infos = {
        .argc = guest_args->argc,
        .argv = guest_args->argv,
        .envp = runner_setup.new_envp,
        .pipe_to_stdin = COPY_PIPE(pipes.stdin_pipe),
        .pipe_to_stdout = COPY_PIPE(pipes.stdout_pipe),
        .pipe_to_stderr = COPY_PIPE(pipes.stderr_pipe),
        .shared_info = runner_setup.shared_memory,
    };
    int status = run_function_fetch(&params, &records, &run_infos, &pipes, config);

    t_crash_info crash_infos = get_crash_infos(&runner_setup, symbolizer);
    t_fetch_result_display result_display = {
        .function_tree = fetch_result.function_tree,
        .tmpfile_output = records.record_stdout.tmp_file_store,
        .crash_name = crash_infos.crash_name,
        .backtrace = crash_infos.backtrace,
        .exit_code = status,
    };
    write_function_fetch_result(&result_display);
    if (crash_infos.crash_name != NULL)
        exit(EXIT_FAILURE);
    if (is_option_set(JSON_OUTPUT_MASK, config))
        fclose(records.record_stdout.tmp_file_store);

    free_runner_setup(&runner_setup);

    return fetch_result;
}

void clear_fetch_result(t_fetch_result *result)
{
    fclose(result->tmpfile_stdin);
    clear_functions(&result->function_tree);
}