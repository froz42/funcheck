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
 * @brief Write the tail of the function tests output when the tests are successful
 * 
 * @param functions_tests_result the result of the function tests
 */
static void print_function_test_result_success(function_tests_result_t *functions_tests_result)
{
	printf("%sFunction tests: %s%zu passed%s, %zu in total\n",
		BOLD,
		GREEN,
		functions_tests_result->nb_total_tests - functions_tests_result->nb_failed_tests,
		RESET,
		functions_tests_result->nb_total_tests);
}

/**
 * @brief Write the tail of the function tests output when the tests are failed
 * 
 * @param functions_tests_result the result of the function tests
 */
static void print_function_test_result_failed(function_tests_result_t *functions_tests_result)
{
	printf("%sFunction tests: %s%zu failed%s, %s%s%zu passed%s, %zu in total\n",
		BOLD,
		RED,
		functions_tests_result->nb_failed_tests,
		RESET,
		BOLD,
		GREEN,
		functions_tests_result->nb_total_tests - functions_tests_result->nb_failed_tests,
		RESET,
		functions_tests_result->nb_total_tests);
}

/**
 * @brief Write the time output of the function tests
 * 
 * @param functions_tests_result the result of the function tests
 */
static void print_function_test_result_time(function_tests_result_t *functions_tests_result)
{
	printf("%sTime:%s           %s\n",
	BOLD,
	RESET,
	get_time_string(functions_tests_result->time_laps));
}

/**
 * @brief Write the success_rate output of the function tests
 * 
 * @param functions_tests_result the result of the function tests
 */
static void print_function_test_result_success_rate(function_tests_result_t *functions_tests_result)
{
	size_t success_rate = (functions_tests_result->nb_total_tests - functions_tests_result->nb_failed_tests)
	 * 100 / functions_tests_result->nb_total_tests;
	printf("%sSuccess rate:%s   %zu %%\n",
		BOLD,
		RESET,
		success_rate);
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
    {
		if (functions_tests_result->nb_total_tests == 0)
			printf("No test ran because of a crash in functions fetching\n");
		else if (!functions_tests_result->nb_failed_tests)
			print_function_test_result_success(functions_tests_result);
		else
			print_function_test_result_failed(functions_tests_result);
		print_function_test_result_time(functions_tests_result);
		print_function_test_result_success_rate(functions_tests_result);
		printf("Ran all test\n");
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