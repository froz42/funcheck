#ifndef BOOL_H
#define BOOL_H

typedef enum bool_e {
    false = 0,
    true = 1
} bool_t;

# define BOOL_STR(x) ((x) ? "true" : "false")

#endif
