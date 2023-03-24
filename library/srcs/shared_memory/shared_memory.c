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

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../shared_memory/shared_memory.h"
#include "../hook/hook.h"
#include "../utils/error.h"

static t_shared_info *shared_info = NULL;
static const char *shared_memory_name = NULL;

/**
 * @brief Open the shared memory
 * 
 * @param name the name of the shared memory
 * @return t_shared_info* the shared memory
 */
static t_shared_info *open_shared_memory(const char *name)
{
	int fd = shm_open(name, O_RDWR, 0666);
	if (fd == -1)
		raise_error("open_shared_memory: shm_open", true);
	t_shared_info *shared_info = mmap(NULL, sizeof(t_shared_info), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (shared_info == MAP_FAILED)
		raise_error("open_shared_memory: mmap", true);
	return shared_info;
}

/**
 * @brief Initialize the shared memory
 * 
 * @return t_shared_info* the shared memory
 */
t_shared_info *init_shared_memory(void)
{
	const char *name = getenv(ENV_MEMORY_NAME);
	shared_info = open_shared_memory(name);
	shared_memory_name = name;
	return shared_info;
}

/**
 * @brief Close the shared memory
 * 
 */
void close_shared_memory(void)
{
	if (!shared_info)
		return;
	munmap(shared_info, sizeof(t_shared_info));
	shared_info = NULL;
	shared_memory_name = NULL;
}

/**
 * @brief Get the shared memory
 * 
 * @return t_shared_info* the shared memory
 */
t_shared_info *get_shared_memory()
{
	if (shared_info == NULL)
		raise_error("get_shared_memory: shared_info is not initialized", false);
	return shared_info;
}