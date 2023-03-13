#include "output.h"
#include "../utils/max.h"
#include "../json/json.h"
#include "../utils/color.h"

#define TOP_RIGHT_CORNER "\u250C"
#define BOTTOM_RIGHT_CORNER "\u2514"
#define HORIZONTAL_LINE "\u2500"
#define VERTICAL_LINE "\u2502"
#define TOP_LEFT_CORNER "\u2510"
#define BOTTOM_LEFT_CORNER "\u2518"

#define MINIMUM_WIDTH 30
#define HORIZONTAL_MARGIN 3

#define PROGRAM_NAME "funcheck"

static void write_header_json(args_t args_guest)
{
    json_write_object(0);
    json_write_key_value_string(
        "funcheck-version",
        VERSION,
        1,
        false);
    json_write_key_value_string(
        "program",
        args_guest.argv[0],
        1,
        false);

    // write arguments
    json_write_key_array("arguments", 1);
    for (int i = 1; i < args_guest.argc; i++)
    {
        json_write_value_string(
            args_guest.argv[i],
            2,
            i == args_guest.argc - 1);
    }
    json_write_array_end(1, false);
}

static void write_spaces(int width_margin)
{
    printf("%*s", width_margin, "");
}

static size_t strlen_ignore_colors(const char *str)
{
    size_t size = 0;
    for (size_t i = 0; str[i]; i++)
    {
        char c = str[i];
        if (c == '\033')
        {
            while (str[i] != 'm')
                i++;
        }
        else
            size++;
    }
    return size;
}

static void write_line(const char *str, size_t width_square)
{
    write_spaces(HORIZONTAL_MARGIN);
    printf("%s%s%s", YELLOW, VERTICAL_LINE, RESET);
    write_spaces(HORIZONTAL_MARGIN);
    printf("%s", str);
    write_spaces(width_square - HORIZONTAL_MARGIN - strlen_ignore_colors(str));
    printf("%s%s%s\n", YELLOW, VERTICAL_LINE, RESET);
}

static void write_top_line(size_t width_square)
{
    write_spaces(HORIZONTAL_MARGIN);
    printf("%s%s%s", YELLOW, TOP_RIGHT_CORNER, RESET);
    for (size_t i = 0; i < width_square; i++)
        printf("%s%s%s", YELLOW, HORIZONTAL_LINE, RESET);
    printf("%s%s%s\n", YELLOW, TOP_LEFT_CORNER, RESET);
}

static void write_bottom_line(size_t width_square)
{
    write_spaces(HORIZONTAL_MARGIN);
    printf("%s%s%s", YELLOW, BOTTOM_RIGHT_CORNER, RESET);
    for (size_t i = 0; i < width_square; i++)
        printf("%s%s%s", YELLOW, HORIZONTAL_LINE, RESET);
    printf("%s%s%s\n", YELLOW, BOTTOM_LEFT_CORNER, RESET);
}

static void write_header_pretty(args_t args_guest)
{

    const size_t width_square = MINIMUM_WIDTH + HORIZONTAL_MARGIN;

    char buffer[1024];

    printf("\n");
    write_top_line(width_square);

    write_line("", width_square);
    snprintf(
        buffer,
        sizeof(buffer),
        "%s%s%s -- %s%s%s",
        YELLOW,
        PROGRAM_NAME,
        RESET,
        BOLD,
        VERSION,
        RESET);
    write_line(buffer, width_square);
    write_line("", width_square);

    snprintf(
        buffer,
        sizeof(buffer),
        "- program: %s%s%s",
        BOLD,
        args_guest.argv[0],
        RESET);
    write_line(buffer, width_square);
    snprintf(buffer, sizeof(buffer), "- arguments:");
    write_line(buffer, width_square);
    for (int i = 1; i < args_guest.argc; i++)
    {
        snprintf(
            buffer,
            sizeof(buffer), "    * %s%s%s",
            BOLD,
            args_guest.argv[i],
            RESET);
        write_line(buffer, width_square);
    }
    write_line("", width_square);
    write_bottom_line(width_square);
    printf("\n");
}

void write_header(args_t args_guest)
{
    if (is_json_output_set())
        write_header_json(args_guest);
    else
        write_header_pretty(args_guest);
}