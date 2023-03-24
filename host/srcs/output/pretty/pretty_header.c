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

#include "../../utils/color.h"
#include "../utils/output_utils.h"
#include "../../host.h"
#include "../output.h"

#define TOP_RIGHT_CORNER "\u250C"
#define BOTTOM_RIGHT_CORNER "\u2514"
#define HORIZONTAL_LINE "\u2500"
#define VERTICAL_LINE "\u2502"
#define TOP_LEFT_CORNER "\u2510"
#define BOTTOM_LEFT_CORNER "\u2518"

#define MINIMUM_WIDTH 30
#define HORIZONTAL_MARGIN 3

#define PROGRAM_NAME "funcheck"

/**
 * @brief Get the length of a string ignoring the color codes
 *
 * @param str the string to get the length from
 * @return size_t the length of the string
 */
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

/**
 * @brief Write line inside the square
 *
 * @param str the string to write
 * @param width_square the width of the square
 */
static void write_line(const char *str, size_t width_square)
{
    write_spaces(HORIZONTAL_MARGIN);
    printf("%s%s%s", YELLOW, VERTICAL_LINE, RESET);
    write_spaces(HORIZONTAL_MARGIN);
    printf("%s", str);
    write_spaces(width_square - HORIZONTAL_MARGIN - strlen_ignore_colors(str));
    printf("%s%s%s\n", YELLOW, VERTICAL_LINE, RESET);
}

/**
 * @brief Write the top line of the square
 *
 * @param width_square the width of the square
 */
static void write_top_line(size_t width_square)
{
    write_spaces(HORIZONTAL_MARGIN);
    printf("%s%s%s", YELLOW, TOP_RIGHT_CORNER, RESET);
    for (size_t i = 0; i < width_square; i++)
        printf("%s%s%s", YELLOW, HORIZONTAL_LINE, RESET);
    printf("%s%s%s\n", YELLOW, TOP_LEFT_CORNER, RESET);
}

/**
 * @brief Write the bottom line of the square
 *
 * @param width_square the width of the square
 */
static void write_bottom_line(size_t width_square)
{
    write_spaces(HORIZONTAL_MARGIN);
    printf("%s%s%s", YELLOW, BOTTOM_RIGHT_CORNER, RESET);
    for (size_t i = 0; i < width_square; i++)
        printf("%s%s%s", YELLOW, HORIZONTAL_LINE, RESET);
    printf("%s%s%s\n", YELLOW, BOTTOM_LEFT_CORNER, RESET);
}

/**
 * @brief Write arguments of the guest program
 *
 * @param args_guest the arguments of the guest program
 * @param width_square the width of the square
 */
static void write_arguments(args_t args_guest, size_t width_square)
{
    char buffer[1024];
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
}

void write_name_version(size_t width_square)
{
    char buffer[1024];
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
}

/**
 * @brief Write the header of the output
 *
 * @param args_guest the arguments of the guest program
 */
void write_header_pretty(args_t args_guest)
{
    const size_t width_square = MINIMUM_WIDTH + HORIZONTAL_MARGIN;

    char buffer[1024];

    printf("\n");
    write_top_line(width_square);
    write_line("", width_square);
    write_name_version(width_square);
    write_line("", width_square);
    snprintf(
        buffer,
        sizeof(buffer),
        "- program: %s%s%s",
        BOLD,
        args_guest.argv[0],
        RESET);
    write_line(buffer, width_square);
    write_line("", width_square);
    write_arguments(args_guest, width_square);
    write_bottom_line(width_square);
    printf("\n");
}