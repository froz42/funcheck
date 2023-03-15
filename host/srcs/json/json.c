#include <stdio.h>
#include "../utils/bool.h"

#define INDENT_SIZE 2


typedef struct {
    char escape_char;
    char *escape_str;
} t_escape_char;

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

static void json_write_indent(size_t indent_count)
{
    for (size_t i = 0; i < indent_count; i++)
        printf("%*s", INDENT_SIZE, "");
}

void json_write_object(size_t indent_count)
{
    json_write_indent(indent_count);
    printf("{\n");
}

void json_write_key_object(const char *key, size_t indent_count)
{
    json_write_indent(indent_count);
    printf("\"%s\": {\n", key);
}

void json_write_key_value_string(
    const char *key,
    const char *value,
    size_t indent_count,
    bool_t is_last)
{
    json_write_indent(indent_count);
    printf("\"%s\": \"%s\"%s\n", key, value, is_last ? "" : ",");
}

void json_write_value_string(
    const char *value,
    size_t indent_count,
    bool_t is_last)
{
    json_write_indent(indent_count);
    printf("\"%s\"%s\n", value, is_last ? "" : ",");
}

void json_write_key_value_file(
    const char *key,
    FILE *file_value,
    size_t indent_count,
    bool_t is_last)
{
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

void json_write_key_value_number(const char *key, size_t value, size_t indent_count, bool_t is_last)
{
    json_write_indent(indent_count);
    printf("\"%s\": %zu%s\n", key, value, is_last ? "" : ",");
}

void json_write_key_value_bool(const char *key, bool_t value, size_t indent_count, bool_t is_last)
{
    json_write_indent(indent_count);
    printf("\"%s\": %s%s\n", key, BOOL_STR(value), is_last ? "" : ",");
}

void json_write_key_array(const char *key, size_t indent_count)
{
    json_write_indent(indent_count);
    printf("\"%s\": [\n", key);
}

void json_write_array_end(size_t indent_count, bool_t is_last)
{
    json_write_indent(indent_count);
    printf("]%s\n", is_last ? "" : ",");
}

void json_write_object_end(size_t indent_count, bool_t is_last)
{
    json_write_indent(indent_count);
    printf("}%s\n", is_last ? "" : ",");
}