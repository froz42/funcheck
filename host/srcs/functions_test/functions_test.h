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

#ifndef FUNCTIONS_TEST_H
#define FUNCTIONS_TEST_H

#include "../functions_fetch/functions_fetch.h"
#include "../time/time.h"

// 2 iterations are required to test a function that has been called at least twice
#define REQUIRED_ITERATIONS(function_info) (function_info->call_count > 1) ? 2 : 1

typedef struct
{
    size_t nb_total_tests;
    size_t nb_failed_tests;
    msseconds_t time_laps;

} function_tests_result_t;

function_tests_result_t functions_test(
    args_t arg_guest,
    char **envp,
    t_fetch_result *fetch_result,
    t_symbolizer *symbolizer);

#endif