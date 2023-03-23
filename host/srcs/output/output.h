#ifndef OUTPUT_H
#define OUTPUT_H

#include "../config/config.h"
#include "../utils/bool.h"
#include "../time/time.h"
#include "../function_footprint/function_footprint.h"
typedef struct
{
    const char *crash_name;
    btree_t_function_call_footprint *function_tree;
    FILE *tmpfile_output;
    t_address_info *backtrace;
    int exit_code;
} t_fetch_result_display;

typedef struct
{
    const char *function_name;
    const char *crash_name;
    FILE *tmpfile_output;
    t_address_info *function_backtrace;
    t_address_info *crash_backtrace;
    btree_t_function_call_footprint *function_tree;
    int exit_code;
    msseconds_t time;
    size_t actual_test;
    size_t total_tests;
} t_test_result_display;

void write_header(args_t args_guest);
void write_head_function_fetch(void);
void write_tail_function_fetch(void);
void write_head_function_tests(void);
void write_tail_function_tests(bool_t success);
void write_function_fetch_result(
    t_fetch_result_display *fetch_result_display);
void write_test_result(t_test_result_display *result, bool_t is_last);
void write_tail(void);

#endif