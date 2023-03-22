#ifndef OUTPUT_UTILS_H
#define OUTPUT_UTILS_H

#include "../../function_footprint/function_footprint.h"

size_t count_allocations(btree_t_function_call_footprint *function_tree);
size_t count_allocations_functions(btree_t_function_call_footprint *function_tree);
size_t get_symbolized_backtrace_size(t_address_info *backtrace);
void write_delim_bar(void);
bool_t is_json_output_set(void);
void output_file(FILE *file);
void write_spaces(int count);
#endif