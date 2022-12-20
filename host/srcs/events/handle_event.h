#ifndef HANDLE_EVENT_H
#define HANDLE_EVENT_H

#include "../function_footprint/function_footprint.h"
#include "../symbolizer/symbolizer.h"
#include <pthread.h>

typedef struct {
    btree_t_function_call_footprint **allocation_tree;
    t_shared_info *shared_memory;
    t_symbolizer *symbolizer;
} t_handle_event_params;

pthread_t launch_handle_events(t_handle_event_params *params);
void stop_handle_events(pthread_t thread, t_shared_info *shared_memory);

#endif