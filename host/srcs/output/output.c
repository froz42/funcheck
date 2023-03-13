#include "../config/config.h"
#include "../utils/bool.h"
#include "../json/json.h"
#include "../host.h"
#include "../stages/stages.h"
#include "../backtrace/backtrace.h"
#include "../logs/logs.h"
#include "../utils/color.h"
#include "output.h"
#include <stdio.h>

#define SIZE_VERTICAL_BAR 30
#define BAR_STR "─"

void write_tail(void)
{
    if (is_json_output_set())
        json_write_object_end(0, true);
}

void write_delim_bar(void)
{
    fprintf(stdout, "%s", YELLOW);
    for (size_t i = 0; i < SIZE_VERTICAL_BAR; i++)
        fprintf(stdout, "%s", BAR_STR);
    fprintf(stdout, "%s\n", RESET);
}

void write_head_function_fetch(void)
{
    if (is_json_output_set())
        json_write_key_object("function-fetch", 1);
    else
    {
        log_info("If your program wait for stdin input, you can type here or pipe it.");
        fprintf(stdout, "%s───────────── TTY ────────────%s\n", YELLOW, RESET);
    }
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

void write_tail_function_tests(bool_t success)
{
    if (is_json_output_set())
        json_write_array_end(1, true);
    else
    {
        if (success)
            log_success("all tested functions passed");
        else
            log_error("some tested functions failed");
    }
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