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

#ifndef OUTPUT_H
#define OUTPUT_H

#include "../config/config.h"
#include "../utils/bool.h"
#include "../time/time.h"
#include "../function_footprint/function_footprint.h"
#include "../functions_test/functions_test.h"

typedef struct
{
    const char *crash_name;
    btree_t_function_call_footprint *function_tree;
    FILE *tmpfile_output;
    t_address_info *backtrace;
    int exit_code;
} t_fetch_result_display;

typedef struct
{
    const char *function_name;
    const char *crash_name;
    FILE *tmpfile_output;
    t_address_info *function_backtrace;
    t_address_info *crash_backtrace;
    btree_t_function_call_footprint *function_tree;
    int exit_code;
    msseconds_t time;
    size_t actual_test;
    size_t total_tests;
} t_test_result_display;

void write_header(args_t args_guest);
void write_head_function_fetch(void);
void write_tail_function_fetch(void);
void write_head_function_tests(void);
void write_tail_function_tests(function_tests_result_t *functions_tests_result);
void write_function_fetch_result(
    t_fetch_result_display *fetch_result_display);
void write_test_result(t_test_result_display *result, bool_t is_last);
void write_tail(void);

#endif