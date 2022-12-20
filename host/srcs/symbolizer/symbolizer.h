#ifndef SYMBOLIZER_H
#define SYMBOLIZER_H

#include <stdio.h>
#include "../backtrace/backtrace.h"

typedef struct s_symbolizer
{
    int pid_symbolizer;
    FILE *read_file;
    int write_fd;
    void (*parse_output)(FILE *file, t_address_info *address_info_ptr);
} t_symbolizer;

t_symbolizer symbolizer_init(char *program_path);
void symbolizer_stop(t_symbolizer *symbolizer);
void symbolizer_get_address_info(
    t_symbolizer *symbolizer,
    const ptr_address *address_list,
    t_address_info *address_infos);
#endif