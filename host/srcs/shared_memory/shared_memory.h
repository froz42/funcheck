#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include "../host.h"

t_shared_info *generate_shared_memory(char *name);
void free_shared_memory(char *name, t_shared_info *shared_info);

#endif