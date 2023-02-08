#include "handle_event.h"
#include "event_utils.h"
#include "../backtrace/backtrace.h"
#include <pthread.h>
#include <stdio.h>

static void handle_crash(t_symbolizer *symbolizer, t_shared_info *shared_info)
{
    printf("[CRASH] %s\n", shared_info->function_name);
    t_address_info *processed_backtrace =
        backtrace_process(
            NULL,
            symbolizer,
            shared_info->backtrace);
    backtrace_print(processed_backtrace);
    free(processed_backtrace);
}

static void handle_alloc(
    t_symbolizer *symbolizer,
    btree_t_function_call_footprint **function_tree,
    t_shared_info *shared_info)
{
    add_allocation(symbolizer, function_tree, shared_info);
}

static void handle_function_call(
    t_symbolizer *symbolizer,
    btree_t_function_call_footprint **function_tree,
    t_shared_info *shared_info)
{
    add_function_call(symbolizer, function_tree, shared_info);
}

static void handle_free(btree_t_function_call_footprint **function_tree, t_shared_info *shared_info)
{
    remove_allocation(function_tree, shared_info->allocation.ptr);
}

static void *handle_events_routine(t_handle_event_params *params)
{
    while (1)
    {
        t_event event = wait_for_event(params->shared_memory);
        switch (event)
        {
        case EXIT:
            return (NULL);
        case CRASH:
            handle_crash(
                params->symbolizer,
                params->shared_memory);
            release_event(params->shared_memory);
            return (NULL);
        case FUNCTION_CALL:
            handle_function_call(
                params->symbolizer,
                params->function_tree,
                params->shared_memory);
            break;
        case ALLOC:
            handle_alloc(
                params->symbolizer,
                params->function_tree,
                params->shared_memory);
            break;
        case REMOVE_ALLOC:
            handle_free(
                params->function_tree,
                params->shared_memory);
            break;
        default:
            break;
        }
        release_event(params->shared_memory);
    }
    return (NULL);
}

pthread_t launch_handle_events(t_handle_event_params *params)
{
    pthread_t thread;
    if (pthread_create(&thread, NULL, (void *(*)(void *))handle_events_routine, params) != 0)
    {
        printf("Error: pthread_create failed\n");
        exit(1);
    }
    return (thread);
}

void stop_handle_events(pthread_t thread, t_shared_info *shared_memory)
{
    t_event saved_event = shared_memory->event;
    shared_memory->event = EXIT;
    sem_post(&shared_memory->lock_host);
    pthread_join(thread, NULL);
    shared_memory->event = saved_event;
}