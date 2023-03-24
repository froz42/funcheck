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

#ifndef FUNCTIONS_FETCH_H
#define FUNCTIONS_FETCH_H

#include <stdio.h>
#include "../function_footprint/function_footprint.h"
#include "../config/config.h"
#include "../record_io/record_io.h"

typedef struct
{
    btree_t_function_call_footprint *function_tree;
    FILE *tmpfile_stdin;
} t_fetch_result;

typedef struct
{
    t_record_io record_stdin;
    t_record_io record_stdout;
    t_record_io record_stderr;
} t_records;

t_fetch_result functions_fetch(
    args_t *guest_args,
    char **envp,
    t_symbolizer *symbolizer);
void clear_fetch_result(t_fetch_result *result);

#endif