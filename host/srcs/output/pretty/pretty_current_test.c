#include "pretty_output.h"
#include "../../utils/color.h"

/**
 * @brief Write the current test
 * 
 * @param function_name the name of the tested function
 * @param iteration The current iteration tested
 * @param function_backtrace the backtrace of the tested function
 */
void write_current_test_pretty(
    char *function_name,
    size_t iteration,
    t_address_info *function_backtrace)
{
     fprintf(
        stdout,
        "┏%s %s%s %s is currently being tested",
        B_CYAN,
        BOLD,
        function_name,
        RESET);

    if (iteration != 0)
        fprintf(stdout, ": iteration %s%zu%s", BOLD, iteration, RESET);
    fprintf(stdout, "\n");
    backtrace_print_pretty(function_backtrace);
    fprintf(stdout, "\n");
}