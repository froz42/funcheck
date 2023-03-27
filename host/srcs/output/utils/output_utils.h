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

#ifndef OUTPUT_UTILS_H
#define OUTPUT_UTILS_H

#include "../../function_footprint/function_footprint.h"

size_t count_allocations(btree_t_function_call_footprint *function_tree);
size_t count_allocations_functions(btree_t_function_call_footprint *function_tree);
void write_delim_bar(void);
bool_t is_json_output_set(void);
void output_file(FILE *file);
void write_spaces(int count);
bool_t is_a_tty(void);
t_address_info *get_transformed_backtrace(t_address_info *backtrace);

#endif