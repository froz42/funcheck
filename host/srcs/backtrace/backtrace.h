#ifndef BACKTRACE_H
#define BACKTRACE_H


typedef struct s_address_info
{
    ptr_address address;
    char function_name[256];
    char file_name[256];
    unsigned int line_number;
    unsigned int column_number;
} t_address_info;

#include "../symbolizer/symbolizer.h"

t_address_info *backtrace_process(
    t_address_info *dest,
    t_symbolizer *symbolizer,
    ptr_address *backtrace);
    
void backtrace_print(t_address_info *backtrace);

#endif