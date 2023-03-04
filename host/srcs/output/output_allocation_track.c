#include "../config/config.h"
#include "../utils/bool.h"
#include "../json/json.h"
#include "../host.h"
#include "../stages/stages.h"
#include "../backtrace/backtrace.h"
#include "output.h"

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
    write_backtrace(
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

static size_t count_allocation_nodes(t_function_call_footprint *node)
{
    if (node->allocations)
        return 1;
    return 0;
}

void write_allocation_track(
    const char *key,
    btree_t_function_call_footprint *function_tree,
    size_t indent_count,
    bool_t is_last)
{
    if (is_json_output_set())
    {
        json_write_key_array(key, indent_count);
        size_t tree_size = btree_t_function_call_footprint_size(function_tree);
        size_t allocation_count = btree_t_function_call_footprint_count(
            function_tree,
            count_allocation_nodes);
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
}