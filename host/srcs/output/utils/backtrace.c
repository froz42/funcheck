#include "../../function_footprint/function_footprint.h"
#include "../../config/config.h"

/**
 * @brief Get the complete backtrace size object
 * 
 * @param backtrace backtrace to get size
 * @return size_t size of the backtrace
 */
static size_t get_complete_backtrace_size(t_address_info *backtrace)
{
    size_t size_backtrace = 0;

    while (backtrace->address)
        backtrace++;
    return size_backtrace;
}

/**
 * @brief Find the index of the main function in the backtrace
 * 
 * @param backtrace backtrace to search
 * @return ssize_t index of the main function in the backtrace or -1 if not found
 */
static ssize_t find_main_index(t_address_info *backtrace)
{
    size_t i = 0;

    while (backtrace[i].address)
    {
        if (strcmp(backtrace[i].function_name, "main") == 0)
            return i;
        i++;
    }
    return -1;
}

/**
 * @brief Get size of the backtrace to display
 * 
 * @param backtrace original backtrace
 * @param complete_backtrace true if complete backtrace should be displayed
 * @return size_t size of the backtrace to display
 */
static size_t get_backtrace_size(t_address_info *backtrace, bool_t complete_backtrace)
{
    size_t complete_backtrace_size = get_complete_backtrace_size(backtrace);

    if (complete_backtrace)
        return complete_backtrace_size;
    ssize_t main_index = find_main_index(backtrace);
    if (main_index == -1)
        return complete_backtrace_size;

    size_t size_backtrace = 0;
    while (backtrace[main_index].address &&
           !should_ignore_function(backtrace[main_index].function_name))
    {
        main_index--;
        size_backtrace++;
    }
    return size_backtrace;
}

/**
 * @brief Get the backtrace to display
 * 
 * @param backtrace original backtrace
 * @return t_address_info* transformed backtrace
 */
t_address_info *get_transformed_backtrace(t_address_info *backtrace)
{
    bool_t complete_backtrace = is_option_set(COMPLETE_BACKTRACE_MASK, get_config());
    size_t size_backtrace = get_backtrace_size(backtrace, complete_backtrace);
    t_address_info *transformed_backtrace = malloc(sizeof(t_address_info) * (size_backtrace + 1));

    if (complete_backtrace)
    {
        memcpy(transformed_backtrace, backtrace, sizeof(t_address_info) * (size_backtrace + 1));
        return transformed_backtrace;
    }
    ssize_t main_index = find_main_index(backtrace);
    if (main_index == -1)
    {
        memcpy(transformed_backtrace, backtrace, sizeof(t_address_info) * (size_backtrace + 1));
        return transformed_backtrace;
    }
    size_t i = size_backtrace - 1;
    while (backtrace[main_index].address &&
           !should_ignore_function(backtrace[main_index].function_name))
    {
        transformed_backtrace[i] = backtrace[main_index];
        main_index--;
        i--;
    }

    return transformed_backtrace;
}