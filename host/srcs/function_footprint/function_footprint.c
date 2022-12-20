#include "function_footprint.h"
#include <string.h>
#include <stdio.h>
#include "../events/event_utils.h"
#include "../backtrace/backtrace.h"

define_btree_functions(t_allocation, cmp_t_allocation);
define_btree_functions(t_function_call_footprint, cmp_t_function_call_footprint);

static void clear_btree_t_function_call_footprint(t_function_call_footprint *info)
{
    free(info->function_name);
    btree_t_allocation_clear(&info->allocations, NULL);
}

btree_t_function_call_footprint *add_function_call(
    t_symbolizer *symbolizer,
    btree_t_function_call_footprint **function_tree,
    t_shared_info *shared_infos)
{
     t_function_call_footprint allocation_info = {
        .allocations = NULL,
        .call_count = 0,
        .function_name = strdup(shared_infos->function_name),
    };

    backtrace_process(allocation_info.backtrace, symbolizer, shared_infos->backtrace);

    btree_t_function_call_footprint *node = btree_t_function_call_footprint_search(*function_tree, &allocation_info);
    if (node == NULL)
        node = btree_t_function_call_footprint_insert(function_tree, &allocation_info);
    else
        free(allocation_info.function_name);
    node->value.call_count++;   
    return node;
}

void add_allocation(
    t_symbolizer *symbolizer,
    btree_t_function_call_footprint **function_tree,
    t_shared_info *shared_infos)
{
    btree_t_function_call_footprint *node = add_function_call(
        symbolizer,
        function_tree,
        shared_infos);
    t_allocation allocation = {
        .size = shared_infos->allocation.size,
        .ptr = shared_infos->allocation.ptr};
    btree_t_allocation_insert(&node->value.allocations, &allocation);
}

void clear_functions(btree_t_function_call_footprint **function_tree)
{
    btree_t_function_call_footprint_clear(function_tree, clear_btree_t_function_call_footprint);
}

static void *_ptr_to_free = NULL;

static void remove_allocation_from_tree(t_function_call_footprint *info)
{
    t_allocation allocation = {.ptr = _ptr_to_free};
    btree_t_allocation_delete(&info->allocations, &allocation);
}

void remove_allocation(btree_t_function_call_footprint **function_tree, void *ptr)
{
    _ptr_to_free = ptr;
    btree_t_function_call_footprint_foreach(*function_tree, remove_allocation_from_tree);
}
