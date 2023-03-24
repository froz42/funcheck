#ifndef FUNCTIONS_FETCH_H
#define FUNCTIONS_FETCH_H

#include <stdio.h>
#include "../function_footprint/function_footprint.h"
#include "../config/config.h"
#include "../record_io/record_io.h"

typedef struct
{
    btree_t_function_call_footprint *function_tree;
    FILE *tmpfile_stdin;
} t_fetch_result;

typedef struct
{
    t_record_io record_stdin;
    t_record_io record_stdout;
    t_record_io record_stderr;
} t_records;

t_fetch_result functions_fetch(
    args_t *guest_args,
    char **envp,
    t_symbolizer *symbolizer);
void clear_fetch_result(t_fetch_result *result);

#endif