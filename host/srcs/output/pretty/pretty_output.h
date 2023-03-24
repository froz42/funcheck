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

#ifndef PRETTY_OUTPUT_H
#define PRETTY_OUTPUT_H

#include "../output.h"

void write_allocation_track_pretty(
    btree_t_function_call_footprint *function_tree,
    const char *function_blocked,
    t_address_info *backtrace);
void backtrace_print_pretty(t_address_info *backtrace);
void write_header_pretty(args_t args_guest);
void write_function_fetch_result_pretty(
    t_fetch_result_display *fetch_result_display);
void write_test_result_pretty(t_test_result_display *result);

#endif