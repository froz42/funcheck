#include <stdio.h>
#include "../../functions_test/functions_test.h"
#include "../../utils/color.h"

/**
 * @brief Write the tail of the function tests output when the tests are successful
 *
 * @param functions_tests_result the result of the function tests
 */
static void print_function_test_result_success(function_tests_result_t *functions_tests_result)
{
    printf("%sFunction tests: %s%zu passed%s, %zu in total\n",
           BOLD,
           GREEN,
           functions_tests_result->nb_total_tests - functions_tests_result->nb_failed_tests,
           RESET,
           functions_tests_result->nb_total_tests);
}

/**
 * @brief Write the tail of the function tests output when the tests are failed
 *
 * @param functions_tests_result the result of the function tests
 */
static void print_function_test_result_failed(function_tests_result_t *functions_tests_result)
{
    printf("%sFunction tests: %s%zu failed%s, %s%s%zu passed%s, %zu in total\n",
           BOLD,
           RED,
           functions_tests_result->nb_failed_tests,
           RESET,
           BOLD,
           GREEN,
           functions_tests_result->nb_total_tests - functions_tests_result->nb_failed_tests,
           RESET,
           functions_tests_result->nb_total_tests);
}

/**
 * @brief Write the time output of the function tests
 *
 * @param functions_tests_result the result of the function tests
 */
static void print_function_test_result_time(function_tests_result_t *functions_tests_result)
{
    printf("%sTime:%s           %s\n",
           BOLD,
           RESET,
           get_time_string(functions_tests_result->time_laps));
}

/**
 * @brief Write the success_rate output of the function tests
 *
 * @param functions_tests_result the result of the function tests
 */
static void print_function_test_result_success_rate(function_tests_result_t *functions_tests_result)
{
    if (functions_tests_result->nb_total_tests == 0)
        return;
    size_t success_rate = (functions_tests_result->nb_total_tests - functions_tests_result->nb_failed_tests) * 100 / functions_tests_result->nb_total_tests;
    printf("%sSuccess rate:%s   %zu %%\n",
           BOLD,
           RESET,
           success_rate);
}

void write_tail_function_tests_pretty(function_tests_result_t *functions_tests_result)
{
    printf("\n");
    if (functions_tests_result->nb_total_tests && !functions_tests_result->nb_failed_tests)
        print_function_test_result_success(functions_tests_result);
    else if (functions_tests_result->nb_total_tests)
        print_function_test_result_failed(functions_tests_result);
    print_function_test_result_time(functions_tests_result);
    print_function_test_result_success_rate(functions_tests_result);
    if (functions_tests_result->nb_total_tests == 0)
        printf("%sNo test ran%s\n", GRAY, RESET);
    else
        printf("%sAll tests ran%s\n", GRAY, RESET);
}