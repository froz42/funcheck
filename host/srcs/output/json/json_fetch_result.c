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

#include "../../json/json.h"
#include "json_output.h"
#include "../output.h"

/**
 * @brief Close the json object if a crash occurred
 *
 */
static void write_crash_end()
{
    json_write_object_end(1, false);
    write_head_function_tests();
    write_tail_function_tests(NULL);
    json_write_object_end(0, true);
}

/**
 * @brief write the crash infos
 * 
 * @param fetch_result_display the fetch result to write
 */
static void write_crash_infos(t_fetch_result_display *fetch_result_display)
{
    json_write_key_object("crash", 2);
    json_write_key_value_string(
        "name",
        fetch_result_display->crash_name,
        3,
        false);
    write_backtrace_json("backtrace", 3, true, fetch_result_display->backtrace);
    json_write_object_end(2, false);
}

/**
 * @brief write the fetch result in json format
 *
 * @param fetch_result_display the fetch result to write
 */
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
        write_crash_infos(fetch_result_display);
    if (is_option_set(TRACK_ALLOCATIONS_MASK, config))
        write_allocation_track_json(
            "allocations-not-freed",
            fetch_result_display->function_tree,
            2,
            false);
    json_write_key_value_number(
        "function-detected-count",
        count_testable_functions(fetch_result_display->function_tree),
        2,
        true);

    if (fetch_result_display->crash_name)
        write_crash_end();
}