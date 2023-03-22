#include "json_output.h"
#include "../../json/json.h"

/**
 * @brief Write the crash in json format
 * 
 * @param result the result of the test
 */
static void write_test_result_crash(t_test_result_display *result)
{
    json_write_key_object("crash", 3);
    json_write_key_value_string(
        "name",
        result->crash_name,
        4,
        false);
    write_backtrace_json("backtrace", 4, true, result->crash_backtrace);
    json_write_object_end(3, false);
}

/**
 * @brief Write the test result in json format
 * 
 * @param result the result of the test
 * @param is_last true if it's the last test result
 */
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
            write_allocation_track_json(
                "allocations-not-freed",
                result->function_tree,
                3,
                false);
    }
    write_backtrace_json("function-backtrace", 3, true, result->function_backtrace);
    json_write_object_end(2, is_last);
}