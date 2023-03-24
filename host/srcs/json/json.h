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

#ifndef JSON_H
#define JSON_H

#include <stdio.h>
#include "../utils/bool.h"

void json_write_object(size_t indent_count);
void json_write_key_object(const char *key, size_t indent_count);
void json_write_key_value_string(const char *key, const char *value, size_t indent_count, bool_t is_last);
void json_write_value_string(
    const char *value,
    size_t indent_count,
    bool_t is_last);
void json_write_key_value_file(const char *key, FILE *file_value, size_t indent_count, bool_t is_last);
void json_write_key_value_number(const char *key, size_t value, size_t indent_count, bool_t is_last);
void json_write_key_value_bool(const char *key, bool_t value, size_t indent_count, bool_t is_last);
void json_write_key_array(const char *key, size_t indent_count);
void json_write_array_end(size_t indent_count, bool_t is_last);
void json_write_object_end(size_t indent_count, bool_t is_last);


#endif