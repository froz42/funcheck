#include "../stages/stages.h"
#include "../config/config.h"
#include "../json/json.h"
#include "output.h"
#include "../logs/logs.h"
#include "../utils/color.h"

void write_function_fetch_result_json(
    t_fetch_result_display *fetch_result_display)
{
    const config_t *config = get_config();
    json_write_key_value_file(
        "output",
        fetch_result_display->tmpfile_output,
        2,
        false);
    json_write_key_value_number(
        "return-code",
        fetch_result_display->exit_code,
        2,
        false);
    if (fetch_result_display->crash_name)
    {
        json_write_key_object("crash", 2);
        json_write_key_value_string(
            "name",
            fetch_result_display->crash_name,
            3,
            false);
        write_backtrace("backtrace", 3, true, fetch_result_display->backtrace);
        json_write_object_end(2, false);
    }
    if (is_option_set(TRACK_ALLOCATIONS_MASK, config))
        write_allocation_track(
            "allocations-not-freed",
            fetch_result_display->function_tree,
            2,
            false);
    json_write_key_value_number(
        "function-detected-count",
        btree_t_function_call_footprint_size(fetch_result_display->function_tree),
        2,
        true);

    if (fetch_result_display->crash_name)
    {
        // if it's a crash we need to close the json
        json_write_object_end(1, false);
        write_head_function_test_header();
        write_tail_function_test_header();
        json_write_object_end(0, true);
    }
}

void write_function_fetch_result_pretty(
    t_fetch_result_display *fetch_result_display)
{
    t_task_end task_end = fetch_result_display->crash_name
                              ? TASK_END_CRASH
                              : TASK_END_SUCCESS;
    write_delim_bar();
    log_task("Testable functions fetching", task_end);
    char buffer[1024];

    if (fetch_result_display->crash_name)
    {
        fprintf(stdout, "\n");
        fprintf(
            stdout,
            "┏%s %s%s %s\n",
            B_RED,
            BOLD,
            fetch_result_display->crash_name,
            RESET);
        pretty_backtrace_print(fetch_result_display->backtrace);
    }
    else
    {
        snprintf(
            buffer,
            sizeof(buffer),
            "Functions detected count: %s%zi%s",
            BOLD,
            btree_t_function_call_footprint_size(fetch_result_display->function_tree),
            RESET);
        log_info(buffer);
        if (is_option_set(TRACK_ALLOCATIONS_MASK, get_config()))
            write_pretty_allocation_track(
                fetch_result_display->function_tree, 
                NULL,
                NULL);
        fprintf(stdout, "\n");
    }
}

void write_function_fetch_result(
    t_fetch_result_display *fetch_result_display)
{
    if (is_json_output_set())
        write_function_fetch_result_json(fetch_result_display);
    else
        write_function_fetch_result_pretty(fetch_result_display);
}