#ifndef stages_h
#define stages_h

#include "../../../shared/shared.h"
#include "../function_footprint/function_footprint.h"
#include "../run/runner.h"
#include "../symbolizer/symbolizer.h"

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

t_setup_result general_setup(char **envp);
void free_setup_result(t_setup_result result);
t_fetch_result allocations_fetch(
    int argc,
    char **argv,
    char **envp,
    t_symbolizer *symbolizer);
void clear_fetch_result(t_fetch_result *result);
void allocations_test(
    int argc,
    char **argv,
    char **envp,
    t_fetch_result *fetch_result,
    t_symbolizer *symbolizer);
#endif