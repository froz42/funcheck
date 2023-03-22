#include "function_footprint.h"
#include <stdio.h>

/**
 * @brief compare two t_allocation
 * 
 * @param a a pointer to a t_allocation
 * @param b a pointer to a t_allocation
 * @return int the difference between the two t_allocation
 */
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

/**
 * @brief compare two t_function_call_footprint
 * 
 * @param a a pointer to a t_function_call_footprint
 * @param b a pointer to a t_function_call_footprint
 * @return int 
 */
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