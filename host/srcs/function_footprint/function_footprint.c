#include "function_footprint.h"
#include <string.h>
#include <stdio.h>
#include "../events/event_utils.h"
#include "../backtrace/backtrace.h"

define_btree_functions(t_allocation, cmp_t_allocation);
define_btree_functions(t_function_call_footprint, cmp_t_function_call_footprint);

/**
 * @brief clear the t_function_call_footprint
 * 
 * @param info the t_function_call_footprint to clear
 */
static void clear_btree_t_function_call_footprint(t_function_call_footprint *info)
{
    free(info->function_name);
    btree_t_allocation_clear(&info->allocations, NULL);
}

/**
 * @brief add a function call to the function tree
 * 
 * @param symbolizer the symbolizer used to symbolize the backtrace
 * @param function_tree the function tree used to store the function call footprint
 * @param shared_infos the shared infos between the host and the guest
 * @return btree_t_function_call_footprint* the function call node
 */
btree_t_function_call_footprint *add_function_call(
    t_symbolizer *symbolizer,
    btree_t_function_call_footprint **function_tree,
    t_shared_info *shared_infos)
{
     t_function_call_footprint allocation_info = {
        .allocations = NULL,
        .call_count = 0,
        .function_name = strdup(shared_infos->function_name),
        .should_test = shared_infos->should_test
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

/**
 * @brief add an allocation to the function tree
 * 
 * @param symbolizer the symbolizer used to symbolize the backtrace
 * @param function_tree the function tree used to store the function call footprint
 * @param shared_infos the shared infos between the host and the guest
 */
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

/**
 * @brief clear the function tree
 * 
 * @param function_tree the function tree to clear
 */
void clear_functions(btree_t_function_call_footprint **function_tree)
{
    btree_t_function_call_footprint_clear(function_tree, clear_btree_t_function_call_footprint);
}

static void *_ptr_to_free = NULL;

/**
 * @brief remove an allocation from the function tree
 * 
 * @param info the function call node
 */
static void remove_allocation_from_tree(t_function_call_footprint *info)
{
    t_allocation allocation = {.ptr = _ptr_to_free};
    btree_t_allocation_delete(&info->allocations, &allocation);
}

/**
 * @brief remove an allocation from the function tree
 * 
 * @param function_tree the function tree to remove the allocation from
 * @param ptr the pointer to the allocation to remove
 */
void remove_allocation(btree_t_function_call_footprint **function_tree, void *ptr)
{
    _ptr_to_free = ptr;
    btree_t_function_call_footprint_foreach(*function_tree, remove_allocation_from_tree);
}

/**
 * @brief count the number of tests in the function tree
 * 
 * @param test_elem the function call node
 * @return size_t the number of tests
 */
static size_t count_tests(t_function_call_footprint *test_elem)
{
    if (test_elem->should_test)
        return 1;
    return 0;
}

/**
 * @brief count the number of testable functions in the function tree
 * 
 * @param function_tree the function tree to count the testable functions from
 * @return size_t the number of testable functions
 */
size_t count_testable_functions(btree_t_function_call_footprint *function_tree)
{
    return btree_t_function_call_footprint_count(function_tree, count_tests);
}
