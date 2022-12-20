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

static t_shared_info *shared_info = NULL;
static const char *shared_memory_name = NULL;

static t_shared_info *open_shared_memory(const char *name)
{
	int fd = shm_open(name, O_RDWR, 0666);
	if (fd == -1)
	{
		dprintf(2, "client: shm_open: %s\n", strerror(errno));
		exit(1);
	}
	t_shared_info *shared_info = mmap(NULL, sizeof(t_shared_info), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (shared_info == MAP_FAILED)
	{
		dprintf(2, "client: mmap: %s\n", strerror(errno));
		exit(1);
	}
	return shared_info;
}

t_shared_info *init_shared_memory(void)
{
	const char *name = getenv(ENV_MEMORY_NAME);
	shared_info = open_shared_memory(name);
	shared_memory_name = name;
	return shared_info;
}

void close_shared_memory(void)
{
	if (!shared_info)
		return;
	if (munmap(shared_info, sizeof(t_shared_info)) == -1)
		dprintf(2, "client: munmap: %s\n", strerror(errno));
	shared_info = NULL;
	shared_memory_name = NULL;
}

t_shared_info *get_shared_memory()
{
	if (shared_info == NULL)
	{
		dprintf(2, "client: shared memory not initialized\n");
		exit(1);
	}
	return shared_info;
}