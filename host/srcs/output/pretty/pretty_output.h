#ifndef PRETTY_OUTPUT_H
#define PRETTY_OUTPUT_H

void write_allocation_track_pretty(
    btree_t_function_call_footprint *function_tree,
    const char *function_blocked,
    t_address_info *backtrace);
void backtrace_print_pretty(t_address_info *backtrace);
void write_header_pretty(args_t args_guest);
void write_function_fetch_result_pretty(
    t_fetch_result_display *fetch_result_display);
void write_test_result_pretty(t_test_result_display *result);

#endif