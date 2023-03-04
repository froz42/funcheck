#include "../config/config.h"
#include "../utils/bool.h"
#include "../json/json.h"
#include "../host.h"
#include "../stages/stages.h"
#include "../backtrace/backtrace.h"
#include "output.h"

void write_header(args_t args_guest)
{
    if (is_json_output_set())
    {
        json_write_object(0);
        json_write_key_value_string(
            "funcheck-version",
            VERSION,
            1,
            false);
        json_write_key_value_string(
            "program",
            args_guest.argv[0],
            1,
            false);

        // write arguments
        json_write_key_array("arguments", 1);
        for (int i = 1; i < args_guest.argc; i++)
        {
            json_write_value_string(
                args_guest.argv[i],
                2,
                i == args_guest.argc - 1);
        }
        json_write_array_end(1, false);
    }
}

void write_tail(void)
{
    if (is_json_output_set())
        json_write_object_end(0, true);
}

void write_head_function_fetch(void)
{
    if (is_json_output_set())
        json_write_key_object("function-fetch", 1);
}

void write_tail_function_fetch(void)
{
    if (is_json_output_set())
        json_write_object_end(1, false);
}

void write_head_function_tests(void)
{
    if (is_json_output_set())
        json_write_key_array("function-tests", 1);
}

void write_tail_function_tests(void)
{
    if (is_json_output_set())
        json_write_array_end(1, true);
}

void write_head_function_test_header(void)
{
    if (is_json_output_set())
        json_write_key_array("functions-tests", 1);
}

void write_tail_function_test_header(void)
{
    if (is_json_output_set())
        json_write_array_end(1, true);
}