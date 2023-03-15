#include "../function_footprint/function_footprint.h"
#include "../symbolizer/symbolizer.h"
#include "../utils/bool.h"
#include <string.h>

static size_t backtrace_get_size(ptr_address *address_list)
{
    size_t i = 0;
    while (address_list[i] != 0)
        i++;
    return i;
}

bool_t should_ignore_function(const char *function_name)
{
    static const char *functions_to_ignores[] = {
        "_fini",
        "_end",
        "_start",
        "__GNU_EH_FRAME_HDR",
        "__FRAME_END__",
        "??"
    };
    for (size_t i = 0; i < sizeof(functions_to_ignores) / sizeof(char *); i++)
    {
        if (strcmp(function_name, functions_to_ignores[i]) == 0)
            return true;
    }
    return false;
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
    return dest;
}