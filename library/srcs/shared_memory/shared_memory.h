#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

# include "../../../shared/shared.h"

t_shared_info *init_shared_memory(void);
t_shared_info *get_shared_memory();
void close_shared_memory(void);

#endif