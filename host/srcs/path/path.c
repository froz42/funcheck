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
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../utils/bool.h"
#include "../logs/logs.h"

/**
 * @brief Check if a file is executable
 * 
 * @param path The path of the file
 * @return bool_t true if the file is executable, false otherwise
 */
static bool_t is_executable(const char *path)
{
    struct stat sb;
    return stat(path, &sb) == 0 && sb.st_mode & S_IXUSR;
}

/**
 * @brief get the program executable path
 *
 * @param name The name of the program
 * @return char* The path of the program
 */
char *get_program_in_path(const char *name)
{
    // if we have a / in the name, we don't need to search in the path
    if (strchr(name, '/'))
    {
        if (is_executable(name))
            return strdup(name);
        return NULL;
    }

    char *path = getenv("PATH");
    if (!path)
        log_fatal("get_program_in_path: PATH not set", true);
    char *path_copy = strdup(path);
    if (!path_copy)
        log_fatal("get_program_in_path: strdup failed", true);
    char *path_token = strtok(path_copy, ":");
    char *program_path = NULL;

    while (path_token)
    {
        program_path = malloc(strlen(path_token) + strlen(name) + 2);
        if (!program_path)
            log_fatal("get_program_in_path: malloc failed", true);
        sprintf(program_path, "%s/%s", path_token, name);
        if (is_executable(program_path))
        {
            free(path_copy);
            return program_path;
        }
        free(program_path);
        path_token = strtok(NULL, ":");
    }
    free(path_copy);
    return NULL;
}

/**
 * @brief Check if there is a program with correct permissions to execute
 *
 * @param name The name of the program
 * @return int 1 if the program exists and can be executed, 0 otherwise
 */
bool_t is_program_in_path(const char *name)
{
    char *program_path = get_program_in_path(name);
    if (program_path)
    {
        free(program_path);
        return true;
    }
    return false;
}