#ifndef JSON_OUTPUT_H
#define JSON_OUTPUT_H

#include "../../config/config.h"
#include "../../stages/stages.h"

void write_header_json(args_t args_guest);
void write_function_fetch_result_json(
    t_fetch_result_display *fetch_result_display);
void write_test_result_json(t_test_result_display *result, bool_t is_last);
void write_backtrace_json(
    const char *key,
    size_t indent_count,
    bool_t is_last,
    t_address_info *backtrace);
void write_allocation_track_json(
    const char *key,
    btree_t_function_call_footprint *function_tree,
    size_t indent_count,
    bool_t is_last);

#endif