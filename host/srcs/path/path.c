#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../utils/bool.h"
#include "../logs/logs.h"

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
    return get_program_in_path(name) != NULL;
}