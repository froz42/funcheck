#ifndef FUNCTIONS_TEST_H
#define FUNCTIONS_TEST_H

#include "../functions_fetch/functions_fetch.h"

size_t functions_test(
    args_t arg_guest,
    char **envp,
    t_fetch_result *fetch_result,
    t_symbolizer *symbolizer);

#endif