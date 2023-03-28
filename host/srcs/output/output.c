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

#include "utils/output_utils.h"
#include "../config/config.h"
#include "../utils/color.h"
#include "../utils/bool.h"
#include "../json/json.h"
#include "../logs/logs.h"
#include "json/json_output.h"
#include "pretty/pretty_output.h"
#include "output.h"

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
 * @param function_tests_result_t the function tests result
 */
void write_tail_function_tests(function_tests_result_t *functions_tests_result)
{
    if (is_json_output_set())
        json_write_array_end(1, true);
    else
		write_tail_function_tests_pretty(functions_tests_result);
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

/**
 * @brief Write the current test
 * 
 * @param function_name the name of the tested function
 * @param iteration the iteration of the test
 * @param function_backtrace the backtrace of the tested function
 */
void write_current_test(
    char *function_name,
    size_t iteration,
    t_address_info *function_backtrace)
{
    if (!is_json_output_set())
        write_current_test_pretty(function_name, iteration, function_backtrace);
}