#include "../function_footprint/function_footprint.h"
#include "../symbolizer/symbolizer.h"

size_t backtrace_get_size(ptr_address *address_list)
{
    size_t i = 0;
    while (address_list[i] != 0)
        i++;
    return i;
}

void backtrace_print(t_address_info *backtrace)
{
    size_t i = 0;
    while (i < MAX_BACKTRACE_DEPTH && backtrace[i].address != 0)
    {
        printf(
            "┗━━ %s in %s:%d (%#zx)\n",
            backtrace[i].function_name,
            backtrace[i].file_name,
            backtrace[i].line_number,
            backtrace[i].address);
        i++;
    }
}

t_address_info *backtrace_process(
    t_address_info *dest,
    t_symbolizer *symbolizer,
    ptr_address *backtrace)
{
    size_t backtrace_size = backtrace_get_size(backtrace);
    if (!dest)
        dest = malloc(sizeof(t_address_info) * (backtrace_size + 1));
    if (!dest)
    {
        fprintf(stderr, "Error: malloc failed in process_backtrace\n");
        exit(1);
    }
    symbolizer_get_address_info(symbolizer, backtrace, dest);
    dest[backtrace_size].address = 0;
    return dest;
}