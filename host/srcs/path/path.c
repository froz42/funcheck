#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
        return strdup(name);

    char *path = getenv("PATH");
    char *path_copy = strdup(path);
    char *path_token = strtok(path_copy, ":");
    char *program_path = NULL;

    while (path_token)
    {
        program_path = malloc(strlen(path_token) + strlen(name) + 2);
        sprintf(program_path, "%s/%s", path_token, name);
        if (access(program_path, X_OK) == 0)
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
int is_program_in_path(const char *name)
{
    char *program_path = get_program_in_path(name);
    if (program_path)
    {
        if (access(program_path, X_OK) == 0)
        {
            free(program_path);
            return 1;
        }
    }
    free(program_path);
    return 0;
}