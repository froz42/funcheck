#include "config.h"

/**
 * @brief wrap strcmp to be used with btree
 * 
 * @param a pointer to string
 * @param b pointer to string
 * @return int the result of strcmp
 */
int strcmp_btree_wrapper(char **a, char **b)
{
    return strcmp(*a, *b);
}

define_btree_functions(char_ptr, strcmp_btree_wrapper);