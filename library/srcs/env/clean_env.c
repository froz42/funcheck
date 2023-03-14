#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../../shared/shared.h"
#include "../utils/error.h"

/**
 * @brief Get the size of envp
 *
 * @param envp the environment variables
 * @return size_t the size of envp
 */
static size_t get_env_size(char **envp)
{
	size_t size;

	for (size = 0; envp[size] != NULL; size++)
		;
	return size;
}

/**
 * @brief Remove LD_PRELOAD and ENV_MEMORY_NAME from envp
 *
 * @param envp the environment variables
 * @return char** the cleaned environment variables
 */
char **clean_env(char **envp)
{
	static const char *to_remove[] = {
		"LD_PRELOAD=",
		ENV_MEMORY_NAME,
		NULL};

	size_t size = get_env_size(envp);
	char **clean_env = malloc(sizeof(char *) * (size + 1));
	if (!clean_env)
		raise_error("clean_env: malloc", true);

	size_t j = 0;

	for (size_t i = 0; i < size; i++)
	{
		for (size_t k = 0; to_remove[k] != NULL; k++)
		{
			if (strncmp(envp[i], to_remove[k], strlen(to_remove[k])) == 0)
				goto skip;
		}
		clean_env[j] = envp[i];
		j++;
	skip:;
	}
	clean_env[j] = NULL;
	return clean_env;
}