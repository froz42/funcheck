#ifndef BACKTRACE_H
#define BACKTRACE_H

#include "../../../shared/shared.h"
#include "../utils/bool.h"
#include "../symbolizer/symbolizer.h"

t_address_info *backtrace_process(
    t_address_info *dest,
    t_symbolizer *symbolizer,
    ptr_address *backtrace);
bool_t should_ignore_function(const char *function_name);

#endif