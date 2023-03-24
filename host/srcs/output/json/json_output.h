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

#ifndef JSON_OUTPUT_H
#define JSON_OUTPUT_H

#include "../../config/config.h"
#include "../output.h"

void write_header_json(args_t args_guest);
void write_function_fetch_result_json(
    t_fetch_result_display *fetch_result_display);
void write_test_result_json(t_test_result_display *result, bool_t is_last);
void write_backtrace_json(
    const char *key,
    size_t indent_count,
    bool_t is_last,
    t_address_info *backtrace);
void write_allocation_track_json(
    const char *key,
    btree_t_function_call_footprint *function_tree,
    size_t indent_count,
    bool_t is_last);

#endif