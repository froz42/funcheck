#include "../utils/bool.h"
#include "../backtrace/backtrace.h"
#include "../json/json.h"
#include "output.h"
#include <stdio.h>

void write_backtrace_element(size_t indent_count, bool_t is_last, t_address_info *backtrace)
{
    char buffer[1024];
    snprintf(
        buffer,
        sizeof(buffer),
        "0x%lx",
        backtrace->address);

    if (is_json_output_set())
    {
        json_write_object(indent_count);
        json_write_key_value_string(
            "address",
            buffer,
            indent_count + 1,
            false);
        json_write_key_value_string(
            "function",
            backtrace->function_name,
            indent_count + 1,
            false);
        json_write_key_value_string(
            "file",
            backtrace->file_name,
            indent_count + 1,
            false);
        json_write_key_value_number(
            "line",
            backtrace->line_number,
            indent_count + 1,
            false);
        json_write_key_value_number(
            "column",
            backtrace->column_number,
            indent_count + 1,
            true);
        json_write_object_end(indent_count, is_last);
    }
}

size_t get_backtrace_size(t_address_info *backtrace)
{
    size_t size_backtrace = 0;
    config_t *config = get_config();
    for (size_t i = 0; backtrace[i].address; i++)
    {
        if (is_option_set(COMPLETE_BACKTRACE_MASK, config) ||
            !should_ignore_function(backtrace[i].function_name))
        {
            size_backtrace++;
        }
    }
    return size_backtrace;
}

void write_backtrace(const char *key, size_t indent_count, bool_t is_last, t_address_info *backtrace)
{
    if (is_json_output_set())
    {
        size_t backtrace_size = get_backtrace_size(backtrace);
        config_t *config = get_config();
        json_write_key_array(key, indent_count);
        size_t backtrace_count = 0;
        for (size_t i = 0; backtrace[i].address; i++)
        {
            if (is_option_set(COMPLETE_BACKTRACE_MASK, config) ||
                !should_ignore_function(backtrace[i].function_name))
            {
                write_backtrace_element(
                    indent_count + 1,
                    ++backtrace_count == backtrace_size,
                    &backtrace[i]);
            }
        }
        json_write_array_end(indent_count, is_last);
    }
}