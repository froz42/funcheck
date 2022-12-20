#ifndef EVENT_UTILS_H
#define EVENT_UTILS_H

#include "../../../shared/shared.h"

const char *get_event_name(t_event event);
t_event wait_for_event(t_shared_info *shared_memory);
void release_event(t_shared_info *shared_memory);

#endif
