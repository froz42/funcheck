#include "config.h"

int strcmp_btree_wrapper(char **a, char **b)
{
    return strcmp(*a, *b);
}

define_btree_functions(char_ptr, strcmp_btree_wrapper);