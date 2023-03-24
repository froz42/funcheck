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
#include <string.h>
#include <pthread.h>
#include "../host.h"
#include "../logs/logs.h"

/**
 * @brief Generate shared memory to be shared with the guest
 *
 * @param name the name of the shared memory
 * @return t_shared_info* the shared memory
 */
t_shared_info *generate_shared_memory(char *name)
{
	int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	if (fd == -1)
		log_fatal("generate_shared_memory: shm_open", true);

	if (ftruncate(fd, sizeof(t_shared_info)) == -1)
		log_fatal("generate_shared_memory: ftruncate", true);

	t_shared_info *shared_info = mmap(NULL, sizeof(t_shared_info), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (shared_info == MAP_FAILED)
		log_fatal("generate_shared_memory: mmap", true);

	bzero(shared_info, sizeof(t_shared_info));
	close(fd);
	return shared_info;
}

/**
 * @brief Free the shared memory
 *
 * @param name the name of the shared memory
 * @param shared_info the shared memory
 */
void free_shared_memory(char *name, t_shared_info *shared_info)
{
	if (munmap(shared_info, sizeof(t_shared_info)) == -1)
		log_fatal("free_shared_memory: munmap", true);
	if (shm_unlink(name) == -1)
		log_fatal("free_shared_memory: shm_unlink", true);
}