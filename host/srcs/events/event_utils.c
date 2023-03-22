#include "../../../shared/shared.h"

/**
 * @brief Get event name
 * 
 * @param event the event
 * @return const char* the name of the event
 */
const char *get_event_name(t_event event)
{
    static const char *events_name[] =
        {
            "NONE",
            "alloc",
            "remove alloc",
            "function call",
            "crash",
            "exit"};
    if (event > sizeof(events_name) / sizeof(char *))
        return "UNKNOWN";
    return events_name[event];
}

/**
 * @brief wait for an event from the guest
 * 
 * @param shared_memory the shared memory
 * @return t_event the event
 */
t_event wait_for_event(t_shared_info *shared_memory)
{
    sem_wait(&shared_memory->lock_host);
    return shared_memory->event;
}

/**
 * @brief release the event to the guest
 * 
 * @param shared_memory the shared memory
 */
void release_event(t_shared_info *shared_memory)
{
    sem_post(&shared_memory->lock_guest);
}