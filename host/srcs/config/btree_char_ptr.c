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