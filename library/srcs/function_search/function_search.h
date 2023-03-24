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