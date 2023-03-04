#ifndef stages_h
#define stages_h

#include "../../../shared/shared.h"
#include "../function_footprint/function_footprint.h"
#include "../run/runner.h"
#include "../symbolizer/symbolizer.h"
#include "../utils/bool.h"

typedef struct
{
    char *memory_name;
    char *env_memory_name;
    char *env_shared_library;
    char **new_envp;
    t_shared_info *shared_memory;
} t_setup_result;

typedef struct
{
    btree_t_function_call_footprint *function_tree;
    FILE *tmpfile_stdin;
} t_fetch_result;

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
} t_test_result_display;

t_setup_result general_setup(char **envp);
void free_setup_result(t_setup_result result);
t_fetch_result allocations_fetch(
    int argc,
    char **argv,
    char **envp,
    t_symbolizer *symbolizer);
void clear_fetch_result(t_fetch_result *result);
int allocations_test(
    int argc,
    char **argv,
    char **envp,
    t_fetch_result *fetch_result,
    t_symbolizer *symbolizer);
#endif