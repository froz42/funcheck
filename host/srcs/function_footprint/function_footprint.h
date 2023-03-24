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