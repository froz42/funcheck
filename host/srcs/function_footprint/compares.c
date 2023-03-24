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

#include "function_footprint.h"
#include <stdio.h>

/**
 * @brief compare two t_allocation
 *
 * @param a a pointer to a t_allocation
 * @param b a pointer to a t_allocation
 * @return int the difference between the two t_allocation
 */
int cmp_t_allocation(t_allocation *a, t_allocation *b)
{
    size_t a_ptr = (size_t)a->ptr;
    size_t b_ptr = (size_t)b->ptr;

    if (a_ptr < b_ptr)
        return -1;
    else if (a_ptr > b_ptr)
        return 1;
    else
        return 0;
}

/**
 * @brief compare two t_function_call_footprint
 *
 * @param a a pointer to a t_function_call_footprint
 * @param b a pointer to a t_function_call_footprint
 * @return int
 */
int cmp_t_function_call_footprint(t_function_call_footprint *a, t_function_call_footprint *b)
{
    t_address_info *backtrace_a = a->backtrace;
    t_address_info *backtrace_b = b->backtrace;

    size_t i = 0;

    // this act like a strncmp size is MAX_BACKTRACE_DEPTH
    while (i < MAX_BACKTRACE_DEPTH &&
           backtrace_a->address &&
           backtrace_a->address == backtrace_b->address)
    {
        backtrace_a++;
        backtrace_b++;
    }
    if (i == MAX_BACKTRACE_DEPTH)
        return 0;
    if (backtrace_a->address > backtrace_b->address)
        return -1;
    else if (backtrace_a->address < backtrace_b->address)
        return 1;
    else
        return 0;
}