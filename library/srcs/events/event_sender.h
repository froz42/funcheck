#ifndef EVENT_SENDER_H
#define EVENT_SENDER_H

#include "../shared_memory/shared_memory.h"

void send_event(t_shared_info *shared_memory, t_event event);
void send_event_nonblocking(t_shared_info *shared_memory, t_event event);

void _send_alloc_event(
    t_shared_info *shared_memory,
    void *ptr,
    size_t size,
    const char *function_name);
void _send_remove_alloc_event(
    t_shared_info *shared_memory,
    void *ptr,
    const char *function_name);
void _send_function_call_event(
	t_shared_info *shared_memory,
	const char *function_name);

#define send_alloc_event(shared_memory, ptr, size) \
    _send_alloc_event(shared_memory, ptr, size, __func__)
#define send_remove_alloc_event(shared_memory, ptr) \
    _send_remove_alloc_event(shared_memory, ptr, __func__)
#define send_function_call_event(shared_memory) \
    _send_function_call_event(shared_memory, __func__)

#endif