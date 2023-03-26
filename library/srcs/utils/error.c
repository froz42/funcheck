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

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include "bool.h"

void raise_error(const char *message, bool_t show_errno)
{
    if (show_errno)
        dprintf(STDERR_FILENO, "[FUNCHECK LIBRARY] [FATAL ERROR] %s: %s\n", message, strerror(errno));
    else
        dprintf(STDERR_FILENO, "[FUNCHECK LIBRARY] [FATAL ERROR] %s\n", message);
    exit(EXIT_FAILURE);
}