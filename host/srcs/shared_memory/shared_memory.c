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