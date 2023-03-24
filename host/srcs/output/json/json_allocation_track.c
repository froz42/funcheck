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

#include "../utils/output_utils.h"
#include "../../json/json.h"
#include "json_output.h"

/**
 * @brief Write an element of the allocation track (JSON)
 *
 * @param allocation the allocation to write
 * @param indent_count the number of indent
 * @param is_last if the allocation is the last of the array
 */
static void write_allocation_track_element(
    t_allocation *allocation,
    size_t indent_count,
    bool_t is_last)
{
    char address[64];
    json_write_object(indent_count);
    snprintf(
        address,
        sizeof(address),
        "%p",
        allocation->ptr);
    json_write_key_value_string(
        "address",
        address,
        indent_count + 1,
        false);
    json_write_key_value_number(
        "size",
        allocation->size,
        indent_count + 1,
        true);
    json_write_object_end(indent_count, is_last);
}

/**
 * @brief Write a node of the allocation track (JSON)
 *
 * @param value the node to write
 * @param indent_count the number of indent
 * @param is_last if the node is the last of the array
 */
static void write_allocation_track_node(
    t_function_call_footprint *value,
    size_t indent_count,
    bool_t is_last)
{
    json_write_object(indent_count);
    json_write_key_value_string(
        "function-name",
        value->function_name,
        indent_count + 1,
        false);
    json_write_key_value_number(
        "call-count",
        value->call_count,
        indent_count + 1,
        false);
    write_backtrace_json(
        "backtrace",
        indent_count + 1,
        false,
        value->backtrace);
    json_write_key_array("allocations", indent_count + 1);
    btree_t_allocation_node *allocations = value->allocations;
    size_t size = btree_t_allocation_size(allocations);
    for (size_t i = 0; i < size; i++)
    {
        t_allocation *allocation = btree_t_allocation_get(allocations, i);
        write_allocation_track_element(
            allocation,
            indent_count + 2,
            i == size - 1);
    }
    json_write_array_end(indent_count + 1, true);
    json_write_object_end(indent_count, is_last);
}

/**
 * @brief Write the allocation track (JSON)
 *
 * @param key json key
 * @param function_tree the allocation track
 * @param indent_count the number of indent
 * @param is_last if the allocation track is the last of the array
 */
void write_allocation_track_json(
    const char *key,
    btree_t_function_call_footprint *function_tree,
    size_t indent_count,
    bool_t is_last)
{
    json_write_key_array(key, indent_count);
    size_t tree_size = btree_t_function_call_footprint_size(function_tree);
    size_t allocation_count = count_allocations_functions(function_tree);
    size_t allocation_index = 0;
    for (size_t i = 0; i < tree_size; i++)
    {
        t_function_call_footprint *value = btree_t_function_call_footprint_get(
            function_tree,
            i);
        if (value->allocations)
        {
            write_allocation_track_node(
                value,
                indent_count + 1,
                ++allocation_index == allocation_count);
        }
    }
    json_write_array_end(indent_count, is_last);
}