#ifndef CONFIG_H
#define CONFIG_H

#include <string.h>

#include "../../../shared/templates/btree.h"

#define TRACK_LEAK_MASK 0b1
#define ALL_STDOUT_MASK 0b10
#define SIMPLE_OUTPUT_MASK 0b100
#define JSON_OUTPUT_MASK 0b1000
#define VERSION_MASK 0b10000
#define HELP_MASK 0b100000

typedef char *char_ptr;

define_btree_types(char_ptr, strcmp);

typedef struct
{
    char options;
    btree_char_ptr *test_functions;
    btree_char_ptr *ignore_functions;
} config_t;

typedef struct
{
    int argc;
    char **argv;
} args_t;

args_t parse_args(int argc, char **argv);
config_t *get_config();
char is_option_set(int mask, const config_t *config);
void display_help(void);

#endif