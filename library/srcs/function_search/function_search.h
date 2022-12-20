#ifndef FUNCTION_SEARCH_H
# define FUNCTION_SEARCH_H

#include "../../../shared/templates/btree.h"

typedef struct s_function_search
{
    const char *function_name;
    void *function;
} t_function_search;

int function_search_cmp(t_function_search *a, t_function_search *b);

define_btree_types(t_function_search, function_search_cmp);

void *function_search_get_function_address(const char *function_name);

# define get_original_function() \
    function_search_get_function_address(__func__)

#endif