#ifndef BTREE_H
#define BTREE_H

#include <stdlib.h>

/**
 * @brief This macro defines the types for a btree.
 * 
 */
#define define_btree_types(type, cmp) \
typedef struct btree_##type##_node { \
    type value; \
    struct btree_##type##_node *left; \
    struct btree_##type##_node *right; \
} btree_##type##_node; \
\
btree_##type##_node *btree_##type##_insert(btree_##type##_node **root, type *value); \
btree_##type##_node *btree_##type##_search(btree_##type##_node *root, type *value); \
size_t btree_##type##_size(btree_##type##_node *root); \
void btree_##type##_foreach(btree_##type##_node *root, void (*f)(type*)); \
void btree_##type##_delete(btree_##type##_node **root, type *value); \
void btree_##type##_clear(btree_##type##_node **root, void (*f)(type*)); \
type *btree_##type##_get(btree_##type##_node *root, size_t index); \
size_t btree_##type##_count(btree_##type##_node *root, size_t (*f)(type*)); \
typedef btree_##type##_node btree_##type

/**
 * @brief This macro defines the functions for a btree.
 * 
 */
#define define_btree_functions(type, cmp) \
btree_##type##_node *btree_##type##_insert(btree_##type##_node **root, type *value) \
{ \
    if (*root == NULL) { \
        *root = malloc(sizeof(btree_##type##_node)); \
        (*root)->value = *value; \
        (*root)->left = NULL; \
        (*root)->right = NULL; \
        return *root; \
    } \
    if (cmp(value, &(*root)->value) < 0) \
        return btree_##type##_insert(&(*root)->left, value); \
    else \
        return btree_##type##_insert(&(*root)->right, value); \
    return *root; \
} \
\
btree_##type##_node *btree_##type##_search(btree_##type##_node *root, type *value) \
{ \
    if (root == NULL) \
        return NULL; \
    if (cmp(value, &root->value) < 0) \
        return btree_##type##_search(root->left, value); \
    else if (cmp(value, &root->value) > 0) \
        return btree_##type##_search(root->right, value); \
    return root; \
} \
\
size_t btree_##type##_size(btree_##type##_node *root) \
{ \
    if (root == NULL) \
        return 0; \
    return 1 + btree_##type##_size(root->left) + btree_##type##_size(root->right); \
} \
\
void btree_##type##_foreach(btree_##type##_node *root, void (*f)(type*)) \
{ \
    if (root == NULL) \
        return; \
    btree_##type##_foreach(root->left, f); \
    btree_##type##_foreach(root->right, f); \
    f(&root->value); \
} \
\
void btree_##type##_delete(btree_##type##_node **root, type *value) \
{ \
    if (*root == NULL) \
        return; \
    if (cmp(value, &(*root)->value) < 0) \
        btree_##type##_delete(&(*root)->left, value); \
    else if (cmp(value, &(*root)->value) > 0) \
        btree_##type##_delete(&(*root)->right, value); \
    else { \
        if ((*root)->left == NULL) { \
            btree_##type##_node *tmp = *root; \
            *root = (*root)->right; \
            free(tmp); \
        } else if ((*root)->right == NULL) { \
            btree_##type##_node *tmp = *root; \
            *root = (*root)->left; \
            free(tmp); \
        } else { \
            btree_##type##_node *tmp = (*root)->right; \
            while (tmp->left != NULL) \
                tmp = tmp->left; \
            (*root)->value = tmp->value; \
            btree_##type##_delete(&(*root)->right, &tmp->value); \
        } \
    } \
} \
\
void btree_##type##_clear(btree_##type##_node **root, void (*f)(type*)) \
{ \
    if (*root == NULL) \
        return; \
    btree_##type##_clear(&(*root)->left, f); \
    btree_##type##_clear(&(*root)->right, f); \
    if (f != NULL) \
        f(&(*root)->value); \
    free(*root); \
    *root = NULL; \
} \
\
type *btree_##type##_get(btree_##type##_node *root, size_t index) \
{ \
    if (root == NULL) \
        return NULL; \
    size_t left_size = btree_##type##_size(root->left); \
    if (index == left_size) \
        return &root->value; \
    if (index < left_size) \
        return btree_##type##_get(root->left, index); \
    return btree_##type##_get(root->right, index - left_size - 1); \
} \
\
size_t btree_##type##_count(btree_##type##_node *root, size_t (*f)(type*)) \
{ \
    if (root == NULL) \
        return 0; \
    return \
        f(&root->value) \
        + btree_##type##_count(root->left, f) \
        + btree_##type##_count(root->right, f); \
}

#endif