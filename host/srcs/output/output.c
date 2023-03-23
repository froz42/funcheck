#include "utils/output_utils.h"
#include "../config/config.h"
#include "../utils/color.h"
#include "../utils/bool.h"
#include "../json/json.h"
#include "../logs/logs.h"
#include "json/json_output.h"
#include "pretty/pretty_output.h"

/**
 * @brief Write the header of the output
 * 
 * @param args_guest the arguments of the guest program
 */
void write_header(args_t args_guest)
{
    if (is_json_output_set())
        write_header_json(args_guest);
    else
        write_header_pretty(args_guest);
}

/**
 * @brief Write the head of the function fetch output
 * 
 */
void write_head_function_fetch(void)
{
    if (is_json_output_set())
        json_write_key_object("function-fetch", 1);
    else
    {
        log_info("If your program wait for stdin input, you can type here or pipe it.");
        fprintf(stdout, "%s───────────── TTY ────────────%s\n", YELLOW, RESET);
    }
}

/**
 * @brief Write the tail of the function fetch output
 * 
 */
void write_tail_function_fetch(void)
{
    if (is_json_output_set())
        json_write_object_end(1, false);
}

/**
 * @brief Write the head of the function tests output
 * 
 */
void write_head_function_tests(void)
{
    if (is_json_output_set())
        json_write_key_array("function-tests", 1);
}

/**
 * @brief Write the tail of the function tests output
 * 
 * @param success true if all tests passed, false otherwise
 */
void write_tail_function_tests(bool_t success)
{
    if (is_json_output_set())
        json_write_array_end(1, true);
    else
    {
        if (success)
            log_success("all tested functions passed");
        else
            log_error("some tested functions failed");
    }
}

/**
 * @brief Write the function fetch result
 * 
 * @param fetch_result_display the fetch result to write
 */
void write_function_fetch_result(
    t_fetch_result_display *fetch_result_display)
{
    if (is_json_output_set())
        write_function_fetch_result_json(fetch_result_display);
    else
        write_function_fetch_result_pretty(fetch_result_display);
}

/**
 * @brief Write the test result
 * 
 * @param result the test result to write
 * @param is_last true if it is the last test result, false otherwise
 */
void write_test_result(t_test_result_display *result, bool_t is_last)
{
    if (is_json_output_set())
        write_test_result_json(result, is_last);
    else
        write_test_result_pretty(result);
}

/**
 * @brief Write the tail of the output
 * 
 */
void write_tail(void)
{
    if (is_json_output_set())
        json_write_object_end(0, true);
}