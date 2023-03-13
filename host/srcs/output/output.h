#ifndef OUTPUT_H
#define OUTPUT_H

#include "../config/config.h"
#include "../utils/bool.h"
#include "../stages/stages.h"
#include "output.h"

void write_header(args_t args_guest);
void write_tail(void);
void write_head_function_tests(void);
void write_tail_function_tests(bool_t success);
void write_head_function_fetch(void);
void write_tail_function_fetch(void);
void write_function_fetch_result(
    t_fetch_result_display *fetch_result_display);
void write_backtrace(const char *key, size_t indent_count, bool_t is_last, t_address_info *backtrace);
void write_test_result(t_test_result_display *result, bool_t is_last);
void write_allocation_track(
    const char *key,
    btree_t_function_call_footprint *function_tree,
    size_t indent_count,
    bool_t is_last);
bool_t is_json_output_set(void);
void write_head_function_test_header(void);
void write_tail_function_test_header(void);
void write_delim_bar(void);
void write_pretty_allocation_track(
    btree_t_function_call_footprint *function_tree,
    const char *function_blocked,
    t_address_info *backtrace);
void output_file(FILE *file);
#endif