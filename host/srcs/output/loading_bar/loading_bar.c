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
#include "../../time/time.h"
#include "../../utils/color.h"

/**
 * @brief Erase the current line and move the cursor to the beginning of the line
 * 
 */
void erase_line(void)
{
    printf("\x1b[2K"); // Clear entire line
    printf("\x1b[0G"); // Move to the beginning of the line
    fflush(stdout);
}

/**
 * @brief Write a loading bar with the current time remaining
 * 
 * @param current current progress
 * @param total total progress
 * @param time_remaining time remaining in ms
 */
void write_loading_bar(
    size_t current,
    size_t total,
    msseconds_t time_remaining)
{
    size_t i = 0;
    size_t percent = (current * 100) / total;
    size_t bar_size = 15;
    size_t bar_percent = (percent * bar_size) / 100;
    printf("%s", YELLOW);
    while (i < bar_percent)
    {
        printf("▰ ");
        i++;
    }
    printf("%s", RESET);
    printf("%s", RED);
    while (i < bar_size)
    {
        printf("▱ ");
        i++;
    }
    printf("%s", RESET);
    const char *time_remaining_str = get_time_string(time_remaining * 2);
    printf(
        " %s%s%s (%s%zu%s/%s%zu%s)",
        BOLD,
        time_remaining_str,
        RESET,
        BOLD,
        current,
        RESET,
        BOLD,
        total,
        RESET);
    fflush(stdout);
}