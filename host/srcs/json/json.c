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

#include <stdio.h>
#include "../utils/bool.h"

#define INDENT_SIZE 2


typedef struct {
    char escape_char;
    char *escape_str;
} t_escape_char;

/**
 * @brief Write a string with escape characters
 * 
 * @param str the string to write
 */
static void escape_write_str(char *str)
{
    static const t_escape_char escape_chars[] = {
        {'\b', "\\b"},
        {'\f', "\\f"},
        {'\n', "\\n"},
        {'\r', "\\r"},
        {'\t', "\\t"},
        {'\"', "\\\""},
        {'\\', "\\\\"},
        {0, NULL}
    };

    for (size_t i = 0; str[i]; i++)
    {
        bool_t is_escape_char = false;
        for (size_t j = 0; escape_chars[j].escape_str; j++)
        {
            if (str[i] == escape_chars[j].escape_char)
            {
                printf("%s", escape_chars[j].escape_str);
                is_escape_char = true;
                break;
            }
        }
        if (!is_escape_char)
            printf("%c", str[i]);
    }
}

/**
 * @brief Write the indent
 * 
 * @param indent_count the number of indent
 */
static void json_write_indent(size_t indent_count)
{
    for (size_t i = 0; i < indent_count; i++)
        printf("%*s", INDENT_SIZE, "");
}

/**
 * @brief Write the end of an object
 * 
 * @param indent_count the number of indent
 */
void json_write_object(size_t indent_count)
{
    json_write_indent(indent_count);
    printf("{\n");
}

/**
 * @brief write a key and start an object
 * 
 * @param key the key
 * @param indent_count the number of indent 
 */
void json_write_key_object(const char *key, size_t indent_count)
{
    json_write_indent(indent_count);
    printf("\"%s\": {\n", key);
}

/**
 * @brief Write a key and a value
 * 
 * @param key the key
 * @param value the value
 * @param indent_count the number of indent
 * @param is_last if the value is the last of the object
 */
void json_write_key_value_string(
    const char *key,
    const char *value,
    size_t indent_count,
    bool_t is_last)
{
    json_write_indent(indent_count);
    printf("\"%s\": \"%s\"%s\n", key, value, is_last ? "" : ",");
}

/**
 * @brief Write a single string value
 * 
 * @param value
 * @param indent_count the number of indent 
 * @param is_last if the value is the last of the object
 */
void json_write_value_string(
    const char *value,
    size_t indent_count,
    bool_t is_last)
{
    json_write_indent(indent_count);
    printf("\"%s\"%s\n", value, is_last ? "" : ",");
}

/**
 * @brief Write a key and a value from a file
 * 
 * @param key
 * @param file_value the file to read 
 * @param indent_count the number of indent
 * @param is_last if the value is the last of the object
 */
void json_write_key_value_file(
    const char *key,
    FILE *file_value,
    size_t indent_count,
    bool_t is_last)
{
    rewind(file_value);
    json_write_indent(indent_count);
    printf("\"%s\": \"", key);

    char buffer[1024];
    size_t read_size = 0;
    while ((read_size = fread(buffer, 1, sizeof(buffer) - 1, file_value)) > 0)
    {
        buffer[read_size] = '\0';
        escape_write_str(buffer);
    }
    if (!is_last)
        printf("\",\n");
}

/**
 * @brief Write a key and a value number
 * 
 * @param key
 * @param value 
 * @param indent_count the number of indent 
 * @param is_last if the value is the last of the object
 */
void json_write_key_value_number(const char *key, size_t value, size_t indent_count, bool_t is_last)
{
    json_write_indent(indent_count);
    printf("\"%s\": %zu%s\n", key, value, is_last ? "" : ",");
}

/**
 * @brief Write a key and a value boolean
 * 
 * @param key
 * @param value 
 * @param indent_count the number of indent 
 * @param is_last if the value is the last of the object
 */
void json_write_key_value_bool(const char *key, bool_t value, size_t indent_count, bool_t is_last)
{
    json_write_indent(indent_count);
    printf("\"%s\": %s%s\n", key, BOOL_STR(value), is_last ? "" : ",");
}

/**
 * @brief Write a key and start an array
 * 
 * @param key 
 * @param indent_count the number of indent
 */
void json_write_key_array(const char *key, size_t indent_count)
{
    json_write_indent(indent_count);
    printf("\"%s\": [\n", key);
}

/**
 * @brief Write the end of an array
 * 
 * @param indent_count the number of indent
 * @param is_last if the value is the last of the object
 */
void json_write_array_end(size_t indent_count, bool_t is_last)
{
    json_write_indent(indent_count);
    printf("]%s\n", is_last ? "" : ",");
}

/**
 * @brief Write the end of an object
 * 
 * @param indent_count the number of indent
 * @param is_last if the value is the last of the object
 */
void json_write_object_end(size_t indent_count, bool_t is_last)
{
    json_write_indent(indent_count);
    printf("}%s\n", is_last ? "" : ",");
}