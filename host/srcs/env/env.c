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

#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief This function generate a unique memory name to be used by shm_open
 *
 * @return char* the name of the memory
 */
char *generate_memory_name(void)
{
	static int i = 0;
	// the format will be funcheck_<pid>_<timestamp>_<iteration>
	char *name = malloc(sizeof(char) * 64);
	if (name == NULL)
		return NULL;
	snprintf(name, 64, "/funcheck_%d_%ld_%i", getpid(), time(NULL), i++);
	return name;
}

/**
 * @brief Generate a env variable key=value
 *
 * @param key the key of the env variable
 * @param value the value of the env variable
 * @return char* the env variable
 */
char *generate_env_string(char *key, char *value)
{
	char *env = malloc(sizeof(char) * (strlen(key) + strlen(value) + 2));
	if (env == NULL)
		return NULL;
	sprintf(env, "%s=%s", key, value);
	return env;
}

/**
 * @brief This function generate envp for the child process
 *
 * @param base_envp the base envp passed to the main function
 * @param to_add the env variable to add
 * @return char** the new envp
 */
char **generate_envp(char **base_envp, char *to_add, char *to_add2)
{
	int i = 0;
	while (base_envp[i] != NULL)
		i++;
	char **envp = malloc(sizeof(char *) * (i + 3));
	if (envp == NULL)
		return NULL;
	i = 0;
	while (base_envp[i] != NULL)
	{
		envp[i] = base_envp[i];
		i++;
	}
	envp[i] = to_add;
	envp[i + 1] = to_add2;
	envp[i + 2] = NULL;
	return envp;
}