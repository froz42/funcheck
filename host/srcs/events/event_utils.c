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