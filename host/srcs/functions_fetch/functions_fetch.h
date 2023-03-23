#ifndef FUNCTIONS_FETCH_H
#define FUNCTIONS_FETCH_H

#include <stdio.h>
#include "../function_footprint/function_footprint.h"
#include "../config/config.h"

typedef struct
{
    btree_t_function_call_footprint *function_tree;
    FILE *tmpfile_stdin;
} t_fetch_result;

t_fetch_result functions_fetch(
    args_t *guest_args,
    char **envp,
    t_symbolizer *symbolizer);
void clear_fetch_result(t_fetch_result *result);

#endif