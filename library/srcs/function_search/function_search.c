#define _GNU_SOURCE

#include <dlfcn.h>
#include <string.h>
#include <stdio.h>
#include "function_search.h"
#include "../hook/hook.h"
#include "../utils/error.h"

static btree_t_function_search *_function_tree = NULL;

define_btree_functions(t_function_search, function_search_cmp);

/**
 * @brief This function is used to compare two t_function_search.
 * 
 * @param a the first element
 * @param b the second element
 * @return int the difference between the two elements
 */
int function_search_cmp(t_function_search *a, t_function_search *b)
{
    return strcmp(a->function_name, b->function_name);
}

/**
 * @brief This function is used to insert a new element
 * 
 * @param root the root of the tree
 * @param value the value to insert
 * @param custom_alloc the custom allocator to use
 * @return btree_t_function_search* the node containing the value
 */
btree_t_function_search *btree_t_function_search_custom_insert(
    btree_t_function_search **root,
    t_function_search *value,
    void *(custom_alloc)(size_t))
{
    if (*root == NULL)
    {
        *root = custom_alloc(sizeof(btree_t_function_search));
        (*root)->value = *value;
        (*root)->left = NULL;
        (*root)->right = NULL;
        return *root;
    }
    if (function_search_cmp(value, &(*root)->value) < 0)
        return btree_t_function_search_custom_insert(
            &(*root)->left,
            value,
            custom_alloc);
    else
        return btree_t_function_search_custom_insert(
            &(*root)->right,
            value,
            custom_alloc);
}

/**
 * @brief This function is used to search for a function address.
 * 
 * @param function_name the name of the function
 * @return void* the address of the function
 */
void *function_search_get_function_address(const char *function_name)
{
    bool_t is_hook_enabled = is_hooks_enabled();
    disable_hooks();
    t_function_search function_search = {.function_name = function_name};
    btree_t_function_search *node = btree_t_function_search_search(
        _function_tree,
        &function_search);
    if (node == NULL)
    {
        void *function = dlsym(RTLD_NEXT, function_name);
        if (function == NULL)
            raise_error("function_search_get_function_address: dlsym", true);

        t_function_search new_function_search = {
            .function_name = function_name,
            .function = function};
        // if we are in a malloc, we can't use malloc
        if (strcmp(function_name, "malloc") == 0)
            btree_t_function_search_custom_insert(
                &_function_tree,
                &new_function_search,
                function);
        else
            btree_t_function_search_insert(
                &_function_tree,
                &new_function_search);
        if (is_hook_enabled)
            enable_hooks();
        return function;
    }
    if (is_hook_enabled)
        enable_hooks();
    return node->value.function;
}