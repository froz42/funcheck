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

#include "../output.h"
#include "../../utils/color.h"
#include "../utils/output_utils.h"
#include "../../logs/logs.h"
#include "pretty_output.h"
#include "../loading_bar/loading_bar.h"

/**
 * @brief utils passed to foreach to count the number of allocation nodes
 * 
 * @param node the node to count
 * @return size_t the number of allocation nodes
 */
static size_t count_byte_allocated(t_allocation *node)
{
    return node->size;
}

/**
 * @brief Count the number of bytes allocated in a context
 * 
 * @param node the context
 * @return size_t the number of bytes allocated
 */
static size_t count_byte_allocated_in_context(t_function_call_footprint *node)
{
    return btree_t_allocation_count(node->allocations, count_byte_allocated);
}

/**
 * @brief Count the number of allocations in a context
 * 
 * @param node the context
 * @return size_t the number of allocations
 */
static size_t count_allocation_in_context(t_function_call_footprint *node)
{
    return btree_t_allocation_size(node->allocations);
}

/**
 * @brief print the allocation track of a element
 * 
 * @param value the allocation track to print
 */
static void write_allocation_track_element(
    t_function_call_footprint *value)
{
    if (!value->allocations)
        return;
    size_t byte_allocated = count_byte_allocated_in_context(value);
    size_t allocation_count = count_allocation_in_context(value);
    fprintf(
        stdout,
        "┏%s %s%s %s %s%zu%s bytes allocated in %s%zu%s allocations is not freed\n",
        B_YELLOW,
        BOLD,
        value->function_name,
        RESET,
        YELLOW,
        byte_allocated,
        RESET,
        YELLOW,
        allocation_count,
        RESET);
    backtrace_print_pretty(value->backtrace);
}

/**
 * @brief print the allocation track of a node
 * 
 * @param function_tree the tree to print
 * @param function_blocked the function that is blocked
 * @param backtrace the backtrace of the function that is blocked
 */
void write_allocation_track_pretty(
    btree_t_function_call_footprint *function_tree,
    const char *function_blocked,
    t_address_info *backtrace)
{
    char buffer[512];
    size_t allocation_context_count = count_allocations_functions(function_tree);
    if (allocation_context_count == 0)
        return;
    if (!function_blocked)
    {
        snprintf(
            buffer,
            sizeof(buffer),
            "Allocations are not freed in %s%zu%s contexts\n",
            YELLOW,
            allocation_context_count,
            RESET);
        log_warn(buffer);
    }
    else
    {
        if (is_a_tty())
            erase_line();
        fprintf(
            stdout,
            "┏%s %s%s %s when this function is failing, allocations are not freed in %s%zu%s contexts \n",
            B_GREEN,
            BOLD,
            function_blocked,
            RESET,
            YELLOW,
            allocation_context_count,
            RESET);
        backtrace_print_pretty(backtrace);
    }
    btree_t_function_call_footprint_foreach(function_tree, write_allocation_track_element);
    printf("\n");
}