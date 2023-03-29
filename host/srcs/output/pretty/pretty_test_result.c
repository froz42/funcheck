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

#include "../utils/output_utils.h"
#include "../../utils/color.h"
#include "../../time/time.h"
#include "../../config/config.h"
#include "../loading_bar/loading_bar.h"
#include "pretty_output.h"

#define UNDEFINED_TIME (unsigned int)-1

static msseconds_t _mean_run_time = UNDEFINED_TIME;

/**
 * @brief compute the mean run time of the tests
 *
 * @param result the result of the test
 */
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

/**
 * @brief write the result of a test if it crashed
 *
 * @param result the result of the test
 * @param config the config of the program
 */
static void write_test_result_pretty_crash(t_test_result_display *result, const config_t *config)
{
    if (is_a_tty())
        erase_line();
    fprintf(
        stdout,
        "┏%s %s%s %s\n",
        B_RED,
        BOLD,
        result->crash_name,
        RESET);
    backtrace_print_pretty(result->crash_backtrace);
    fprintf(
        stdout,
        "┏%s %s%s %s the crash occurred when this function failed\n",
        B_RED,
        BOLD,
        result->function_name,
        RESET);
    backtrace_print_pretty(result->function_backtrace);
    if (!is_option_set(ALL_OUTPUT_MASK, config))
    {
        fprintf(stdout, "%s─────────── OUTPUT ───────────%s\n", YELLOW, RESET);
        output_file(result->tmpfile_output);
        write_delim_bar();
        fprintf(stdout, "\n");
    }
}

/**
 * @brief write the result of a test
 *
 * @param result the result of the test
 */
void write_test_result_pretty(t_test_result_display *result)
{
    const config_t *config = get_config();
    if (result->crash_name)
        write_test_result_pretty_crash(result, config);
    else if (is_option_set(TRACK_ALLOCATIONS_MASK, config))
    {
        write_allocation_track_pretty(
            result->function_tree,
            result->function_name,
            result->function_backtrace);
    }

    if (is_option_set(SHOW_CURRENT_TEST_MASK, config))
        return;
    if (is_option_set(ALL_OUTPUT_MASK, config))
        return write_delim_bar();

    if (!is_a_tty())
        return;
    compute_mean_run_time(result);

    const size_t remaining_test = result->total_tests - result->actual_test;
    const msseconds_t remaining_time = _mean_run_time * remaining_test;
    erase_line();
    // if it's the last test, we don't need to show the loading bar
    if (result->total_tests - result->actual_test > 1)
        write_loading_bar(
            result->actual_test,
            result->total_tests,
            remaining_time);
}