#include "../config/config.h"
#include "../utils/bool.h"
#include "../stages/stages.h"
#include "../json/json.h"
#include "../time/time.h"
#include "../utils/color.h"
#include "../logs/logs.h"
#include "loading_bar/loading_bar.h"
#include "output.h"

#define UNDEFINED_TIME (unsigned int)-1

static void write_test_result_crash(t_test_result_display *result)
{
    json_write_key_object("crash", 3);
    json_write_key_value_string(
        "name",
        result->crash_name,
        4,
        false);
    write_backtrace("backtrace", 4, true, result->crash_backtrace);
    json_write_object_end(3, false);
}

void write_test_result_json(t_test_result_display *result, bool_t is_last)
{
    config_t *config = get_config();
    json_write_object(2);
    json_write_key_value_string(
        "function-name",
        result->function_name,
        3,
        false);
    if (is_option_set(ALL_OUTPUT_MASK, config) || result->crash_name)
        json_write_key_value_file(
            "output",
            result->tmpfile_output,
            3,
            false);
    json_write_key_value_number(
        "return-code",
        result->exit_code,
        3,
        false);
    if (result->crash_name)
        write_test_result_crash(result);
    else
    {
        if (is_option_set(TRACK_ALLOCATIONS_MASK, config))
            write_allocation_track(
                "allocations-not-freed",
                result->function_tree,
                3,
                false);
    }
    write_backtrace("function-backtrace", 3, true, result->function_backtrace);
    json_write_object_end(2, is_last);
}

static msseconds_t _mean_run_time = UNDEFINED_TIME;

static void compute_mean_run_time(t_test_result_display *result)
{
    if (_mean_run_time == UNDEFINED_TIME)
        _mean_run_time = result->time;
    else
    {
        size_t actual_test = result->actual_test;
        _mean_run_time = (_mean_run_time * (actual_test) + result->time) / (actual_test + 1);
    }
}

void write_test_result_pretty_crash(t_test_result_display *result, const config_t *config)
{
    erase_line();
    fprintf(
        stdout,
        "┏%s %s%s %s\n",
        B_RED,
        BOLD,
        result->crash_name,
        RESET);
    pretty_backtrace_print(result->crash_backtrace);
    fprintf(
        stdout,
        "┏%s %s%s %s the crash occurred when this function failed\n",
        B_RED,
        BOLD,
        result->function_name,
        RESET);
    pretty_backtrace_print(result->function_backtrace);
    if (!is_option_set(ALL_OUTPUT_MASK, config))
    {
        fprintf(stdout, "%s─────────── OUTPUT ───────────%s\n", YELLOW, RESET);
        output_file(result->tmpfile_output);
        write_delim_bar();
        fprintf(stdout, "\n");
    }
}

void write_test_result_pretty(t_test_result_display *result)
{
    const config_t *config = get_config();
    if (result->crash_name)
        write_test_result_pretty_crash(result, config);
    else if (is_option_set(TRACK_ALLOCATIONS_MASK, config))
    {
        write_pretty_allocation_track(
            result->function_tree,
            result->function_name,
            result->function_backtrace);
        fprintf(stdout, "\n");
    }

    if (!is_option_set(ALL_OUTPUT_MASK, config))
    {
        compute_mean_run_time(result);

        const size_t remaining_test = result->total_tests - result->actual_test;
        const msseconds_t remaining_time = _mean_run_time * remaining_test;
        // if less than 1 second remaining, we don't display the time
        erase_line();
        if (remaining_time > 1000)
            write_loading_bar(
                result->actual_test,
                result->total_tests,
                remaining_time);
    }
    else
        write_delim_bar();
}

void write_test_result(t_test_result_display *result, bool_t is_last)
{
    if (is_json_output_set())
        write_test_result_json(result, is_last);
    else
        write_test_result_pretty(result);
}