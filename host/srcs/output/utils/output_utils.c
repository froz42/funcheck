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

#include "../../function_footprint/function_footprint.h"
#include "../../config/config.h"
#include "../../utils/color.h"
#include <stdio.h>
#include <unistd.h>

#define SIZE_VERTICAL_BAR 30
#define BAR_STR "─"

/**
 * @brief utils passed to foreach to count the number of allocation nodes
 *
 * @param node the node to count
 * @return size_t the number of allocation nodes
 */
static size_t count_allocation_nodes(t_function_call_footprint *node)
{
    if (node->allocations)
        return 1;
    return 0;
}

/**
 * @brief Count the number allocation function from a function tree
 * @param function_tree the function tree to count the allocations from
 * @return size_t the number of allocations
 */
size_t count_allocations_functions(btree_t_function_call_footprint *function_tree)
{
    return btree_t_function_call_footprint_count(
        function_tree,
        count_allocation_nodes);
}

/**
 * @brief Write a delimiter bar
 *
 */
void write_delim_bar(void)
{
    fprintf(stdout, "%s", YELLOW);
    for (size_t i = 0; i < SIZE_VERTICAL_BAR; i++)
        fprintf(stdout, "%s", BAR_STR);
    fprintf(stdout, "%s\n", RESET);
}

/**
 * @brief Check if the json output is set in the config
 *
 * @return bool_t true if the json output is set
 */
bool_t is_json_output_set(void)
{
    return is_option_set(JSON_OUTPUT_MASK, get_config());
}

/**
 * @brief Output a FILE * to stdout
 *
 * @param file the file pointer to output
 */
void output_file(FILE *file)
{
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), file))
        printf("%s", buffer);
}

/**
 * @brief Write count spaces
 *
 * @param count the number of spaces to write
 */
void write_spaces(int count)
{
    fprintf(stdout, "%*s", (int)count, "");
}

bool_t is_a_tty(void)
{
    static int is_a_tty = -1;
    if (is_a_tty == -1)
        is_a_tty = isatty(fileno(stdout));
    return is_a_tty;
}