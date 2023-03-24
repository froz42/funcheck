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
#include "../../backtrace/backtrace.h"
#include "../../config/config.h"
#include "../utils/output_utils.h"


/**
 * @brief write a backtrace element
 *
 * @param indent_count indent count
 * @param is_last if the element is the last of the array
 * @param backtrace the backtrace element to write
 */
static void write_backtrace_element(size_t indent_count, bool_t is_last, t_address_info *backtrace)
{
    char buffer[1024];
    snprintf(
        buffer,
        sizeof(buffer),
        "0x%lx",
        backtrace->address);

    if (is_json_output_set())
    {
        json_write_object(indent_count);
        json_write_key_value_string(
            "address",
            buffer,
            indent_count + 1,
            false);
        json_write_key_value_string(
            "function",
            backtrace->function_name,
            indent_count + 1,
            false);
        json_write_key_value_string(
            "file",
            backtrace->file_name,
            indent_count + 1,
            false);
        json_write_key_value_number(
            "line",
            backtrace->line_number,
            indent_count + 1,
            false);
        json_write_key_value_number(
            "column",
            backtrace->column_number,
            indent_count + 1,
            true);
        json_write_object_end(indent_count, is_last);
    }
}

/**
 * @brief write a backtrace
 *
 * @param key
 * @param indent_count indent count
 * @param is_last if the backtrace is the last of the array
 * @param backtrace the backtrace to write
 */
void write_backtrace_json(
    const char *key,
    size_t indent_count,
    bool_t is_last,
    t_address_info *backtrace)
{
    if (is_json_output_set())
    {
        size_t backtrace_size = get_symbolized_backtrace_size(backtrace);
        config_t *config = get_config();
        json_write_key_array(key, indent_count);
        size_t backtrace_count = 0;
        for (size_t i = 0; backtrace[i].address; i++)
        {
            if (is_option_set(COMPLETE_BACKTRACE_MASK, config) ||
                !should_ignore_function(backtrace[i].function_name))
            {
                write_backtrace_element(
                    indent_count + 1,
                    ++backtrace_count == backtrace_size,
                    &backtrace[i]);
            }
        }
        json_write_array_end(indent_count, is_last);
    }
}