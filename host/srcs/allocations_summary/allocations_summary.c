#include "../function_footprint/function_footprint.h"
#include <stdio.h>

static size_t total_size_allocations(btree_t_allocation *tree, size_t *count_allocation)
{
    if (tree == NULL)
        return 0;
    size_t size = tree->value.size;
    size += total_size_allocations(tree->left, count_allocation);
    size += total_size_allocations(tree->right, count_allocation);
    if (count_allocation != NULL)
        (*count_allocation)++;
    return size;
}

const char *str_or_questionmark(const char *str)
{
    if (str == NULL)
        return "??";
    if (str[0] == '\0')
        return "??";
    return str;
}

static void print_backtrace(t_address_info *backtrace)
{
    size_t i = 0;
    while (i < MAX_BACKTRACE_DEPTH && backtrace[i].address != 0)
    {
        printf(
            "┗━━ %s in %s:%d (%#zx) %s:%d\n",
            str_or_questionmark(backtrace[i].function_name),
            str_or_questionmark(backtrace[i].file_name),
            backtrace[i].line_number,
            backtrace[i].address,
            str_or_questionmark(backtrace[i].file_name),
            backtrace[i].line_number);
        i++;
    }
}

static void allocation_summary(t_function_call_footprint *info)
{
    if (info->allocations != NULL)
    {
        size_t count_allocations = 0;
        size_t size = total_size_allocations(info->allocations, &count_allocations);
        printf("%zu bytes not freed in %zu allocations\n", size, count_allocations);
        printf("Allocated by %s\n", info->function_name);
        print_backtrace(info->backtrace);
    }
}

void allocations_summary(btree_t_function_call_footprint *tree)
{
    if (tree == NULL)
        return;
    btree_t_function_call_footprint_foreach(tree, allocation_summary);
}