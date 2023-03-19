#ifndef ALLOCATIONS_H
#define ALLOCATIONS_H

#include "../../../shared/shared.h"
#include "../../../shared/templates/btree.h"
#include "../backtrace/backtrace.h"

/**
 * @brief This struct contain the size and the ptr of the allocation.
 */
typedef struct s_allocation
{
    size_t size;
    void *ptr;
} t_allocation;
int cmp_t_allocation(t_allocation *a, t_allocation *b);

define_btree_types(t_allocation, cmp_t_allocation);


/**
 * @brief This struct contain the backtrace and a btree of allocations.
 */
typedef struct s_function_call_footprint
{
    t_address_info backtrace[MAX_BACKTRACE_DEPTH];
    size_t call_count;
    btree_t_allocation_node *allocations;
    char *function_name;
    bool_t should_test;
} t_function_call_footprint;

int cmp_t_function_call_footprint(t_function_call_footprint *a, t_function_call_footprint *b);

define_btree_types(t_function_call_footprint, cmp_t_function_call_footprint);

#include "../symbolizer/symbolizer.h"

btree_t_function_call_footprint *add_function_call(
    t_symbolizer *symbolizer,
    btree_t_function_call_footprint **function_tree,
    t_shared_info *shared_infos);
void add_allocation(
    t_symbolizer *symbolizer,
    btree_t_function_call_footprint **function_tree,
    t_shared_info *shared_infos);
void clear_functions(btree_t_function_call_footprint **function_tree);
void remove_allocation(btree_t_function_call_footprint **function_tree, void *ptr);
size_t count_testable_functions(btree_t_function_call_footprint *function_tree);
#endif