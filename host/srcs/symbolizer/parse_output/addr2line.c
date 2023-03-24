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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "../../function_footprint/function_footprint.h"

/**
 * @brief Parse the output of addr2line.
 * 
 * @param file the file to parse
 * @param address_info_ptr the address info to fill
 */
void parse_addr2line_output(FILE *file, t_address_info *address_info_ptr)
{
    // we parse address two lines by two lines
    // <function_name>\n
    // <file_name>:<line_number>\n
    char *line = NULL;
    size_t size_line = 0;
    ssize_t ret = 0;

    // read the function name
    ret = getline(&line, &size_line, file);
    if (ret == -1)
        return;
    if (ret == 0)
        return;
    line[ret - 1] = '\0';
    strncpy(
        address_info_ptr->function_name,
        line,
        sizeof(address_info_ptr->function_name) - 1);

    // read the file name and line number
    ret = getline(&line, &size_line, file);
    if (ret == -1)
        return;
    if (ret == 0)
        return;
    line[ret - 1] = '\0';
    char *file_name = strtok(line, ":");
    char *line_number = strtok(NULL, ":");
    strncpy(
        address_info_ptr->file_name,
        file_name,
        sizeof(address_info_ptr->file_name) - 1);
    if (line_number)
        address_info_ptr->line_number = strtoul(line_number, NULL, 10);
    else
        address_info_ptr->line_number = 0;
    address_info_ptr->column_number = 0;
    free(line);
}