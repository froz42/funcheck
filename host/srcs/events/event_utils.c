#include "../../../shared/shared.h"

const char *get_event_name(t_event event)
{
    static const char *events_name[] =
        {
            "NONE",
            "alloc",
            "remove alloc",
            "crash",
            "exit"};
    return events_name[event];
}

t_event wait_for_event(t_shared_info *shared_memory)
{
    sem_wait(&shared_memory->lock_host);
    return shared_memory->event;
}

void release_event(t_shared_info *shared_memory)
{
    sem_post(&shared_memory->lock_guest);
}