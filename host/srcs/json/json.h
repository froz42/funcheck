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