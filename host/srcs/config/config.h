/* Funcheck - A tool for checking functions calls return protections
 * Copyright (C) 2023  Theo Matis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <string.h>

#include "../../../shared/templates/btree.h"

#define TRACK_ALLOCATIONS_MASK 1 << 0
#define ALL_OUTPUT_MASK 1 << 1
#define JSON_OUTPUT_MASK 1 << 2
#define VERSION_MASK 1 << 3
#define HELP_MASK 1 << 4
#define ABORT_AS_CRASH_MASK 1 << 5
#define COMPLETE_BACKTRACE_MASK 1 << 6
#define SHOW_CURRENT_TEST_MASK 1 << 7

typedef char *char_ptr;

define_btree_types(char_ptr, strcmp);

typedef struct
{
    unsigned long int options;
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
char is_option_set(unsigned long int mask, const config_t *config);
void display_help(void);

#endif