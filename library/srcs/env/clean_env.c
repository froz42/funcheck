#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../../shared/shared.h"
#include "../utils/error.h"

static size_t get_env_size(char **envp)
{
	size_t size;

	for (size = 0; envp[size] != NULL; size++);
	return size;
}

char **clean_env(char **envp)
{
	size_t size = get_env_size(envp);
	char **clean_env = malloc(sizeof(char *) * (size + 1));
	if (!clean_env)
		raise_error("clean_env: malloc", true);

	size_t i = 0;
	size_t j = 0;

	for (; i < size; i++) {
		if (strncmp(envp[i], "LD_PRELOAD=", 11) == 0)
			continue;
		if (strncmp(envp[i], ENV_MEMORY_NAME, 22) == 0)
			continue;
		clean_env[j] = envp[i];
		j++;
	}
	clean_env[j] = NULL;
	return clean_env;
}