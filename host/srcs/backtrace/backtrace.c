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

#include "../function_footprint/function_footprint.h"
#include "../symbolizer/symbolizer.h"
#include "../utils/bool.h"
#include <string.h>

/**
 * @brief This function is used to get the size of the backtrace
 * 
 * @param address_list the backtrace
 * @return size_t the size of the backtrace
 */
static size_t backtrace_get_size(ptr_address *address_list)
{
    size_t i = 0;
    while (address_list[i] != 0)
        i++;
    return i;
}


/**
 * @brief This function is used to check if a function should be ignored
 * 
 * @param function_name the name of the function
 * @return true if the function should be ignored
*/
bool_t should_ignore_function(const char *function_name)
{
    static const char *functions_to_ignores[] = {
        "_fini",
        "_end",
        "_start",
        "__GNU_EH_FRAME_HDR",
        "__FRAME_END__",
        "??"
    };
    for (size_t i = 0; i < sizeof(functions_to_ignores) / sizeof(char *); i++)
    {
        if (strcmp(function_name, functions_to_ignores[i]) == 0)
            return true;
    }
    return false;
}

/**
 * @brief This function is used to process the backtrace to get information about the functions
 * 
 * @param dest the destination of the backtrace (can be NULL)
 * @param symbolizer the symbolizer
 * @param backtrace the backtrace
 * @return t_address_info* the backtrace (malloced if dest is NULL)
 */
t_address_info *backtrace_process(
    t_address_info *dest,
    t_symbolizer *symbolizer,
    ptr_address *backtrace)
{
    size_t backtrace_size = backtrace_get_size(backtrace);
    if (!dest)
        dest = malloc(sizeof(t_address_info) * (backtrace_size + 1));
    if (!dest)
    {
        fprintf(stderr, "Error: malloc failed in process_backtrace\n");
        exit(1);
    }
    symbolizer_get_address_info(symbolizer, backtrace, dest);
    return dest;
}