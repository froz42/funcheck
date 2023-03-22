#include <stdio.h>
#include "../utils/output_utils.h"
#include "../../config/config.h"

/**
 * @brief write margin for pretty output
 *
 * @param indent_count the number of margin to write
 */
static void write_margin(size_t indent_count)
{
    for (size_t i = 0; i < indent_count; i++)
        printf(" ");
}

/**
 * @brief Write the backtrace in pretty format
 * 
 * @param address_info backtrace element to write
 * @param backtrace_size 
 * @param backtrace_count 
 */
static void backtrace_print_elem(
    t_address_info *address_info,
    size_t backtrace_size,
    size_t backtrace_count)
{

    const char *cross_line = "┳";
    if (backtrace_count == backtrace_size)
        cross_line = "━";

    printf(
        "┗━%s━━ %s",
        cross_line,
        address_info->function_name);
    if (address_info->file_name[0] != '\0')
    {
        printf(" at %s", address_info->file_name);
        if (address_info->line_number)
        {
            printf(":%u", address_info->line_number);
            if (address_info->column_number)
                printf(":%u", address_info->column_number);
        }
    }
    printf(" (%#lx)\n", address_info->address);
}

/**
 * @brief Write the backtrace in pretty format
 *
 * @param backtrace the backtrace to write
 */
void backtrace_print_pretty(t_address_info *backtrace)
{
    config_t *config = get_config();
    size_t backtrace_size = get_symbolized_backtrace_size(backtrace);
    size_t backtrace_count = 0;
    for (size_t i = 0; backtrace[i].address != 0; i++)
    {
        t_address_info *address_info = &backtrace[i];
        if (!is_option_set(COMPLETE_BACKTRACE_MASK, config) &&
            should_ignore_function(address_info->function_name))
            continue;
        write_margin(backtrace_count++ * 2);

        backtrace_print_elem(address_info, backtrace_size, backtrace_count);
    }
}