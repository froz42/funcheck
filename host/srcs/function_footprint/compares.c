#include "function_footprint.h"
#include <stdio.h>

int cmp_t_allocation(t_allocation *a, t_allocation *b)
{
    size_t a_ptr = (size_t)a->ptr;
    size_t b_ptr = (size_t)b->ptr;

    if (a_ptr < b_ptr)
        return -1;
    else if (a_ptr > b_ptr)
        return 1;
    else
        return 0;
}

int cmp_t_function_call_footprint(t_function_call_footprint *a, t_function_call_footprint *b)
{
    t_address_info *backtrace_a = a->backtrace;
    t_address_info *backtrace_b = b->backtrace;

    // this act like a strncmp size is MAX_BACKTRACE_DEPTH
    while (backtrace_a->address && backtrace_a->address == backtrace_b->address)
    {
        backtrace_a++;
        backtrace_b++;
    }
    if (backtrace_a->address > backtrace_b->address)
        return -1;
    else if (backtrace_a->address < backtrace_b->address)
        return 1;
    else
        return 0;
}