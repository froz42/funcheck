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

#include <stdio.h>
#include "../utils/output_utils.h"
#include "../../config/config.h"

/**
 * @brief write margin for pretty output
 *
 * @param indent_count the number of margin to write
 */
static void write_margin(size_t indent_count)
{
    for (size_t i = 0; i < indent_count; i++)
        printf(" ");
}

/**
 * @brief Write the backtrace in pretty format
 *
 * @param address_info backtrace element to write
 * @param backtrace_size
 * @param backtrace_count
 */
static void backtrace_print_elem(
    t_address_info *address_info,
    bool_t is_last)
{

    const char *cross_line = "┳";
    if (is_last)
        cross_line = "━";

    printf(
        "┗━%s━━ %s",
        cross_line,
        address_info->function_name);
    if (address_info->file_name[0] != '\0')
    {
        printf(" at %s", address_info->file_name);
        if (address_info->line_number)
        {
            printf(":%u", address_info->line_number);
            if (address_info->column_number)
                printf(":%u", address_info->column_number);
        }
    }
    printf(" (%#lx)\n", address_info->address);
}

/**
 * @brief Write the backtrace in pretty format
 *
 * @param backtrace the backtrace to write
 */
void backtrace_print_pretty(t_address_info *backtrace)
{
    t_address_info *processed_backtrace = get_transformed_backtrace(backtrace);
    size_t backtrace_count = 0;
    for (size_t i = 0; processed_backtrace[i].address != 0; i++)
    {
        t_address_info *address_info = &processed_backtrace[i];
        write_margin(backtrace_count++ * 2);

        backtrace_print_elem(address_info, processed_backtrace[i + 1].address == 0);
    }
    free(processed_backtrace);
}