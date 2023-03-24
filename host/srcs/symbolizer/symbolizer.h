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

#ifndef SYMBOLIZER_H
#define SYMBOLIZER_H

#include <stdio.h>

typedef struct s_address_info
{
    ptr_address address;
    char function_name[256];
    char file_name[256];
    unsigned int line_number;
    unsigned int column_number;
} t_address_info;

typedef struct s_symbolizer
{
    int pid_symbolizer;
    FILE *read_file;
    int write_fd;
    void (*parse_output)(FILE *file, t_address_info *address_info_ptr);
} t_symbolizer;

t_symbolizer symbolizer_init(char *program_path);
void symbolizer_stop(t_symbolizer *symbolizer);
void symbolizer_get_address_info(
    t_symbolizer *symbolizer,
    const ptr_address *address_list,
    t_address_info *address_infos);
#endif