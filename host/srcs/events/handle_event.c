#include "handle_event.h"
#include "event_utils.h"
#include "../backtrace/backtrace.h"
#include <pthread.h>
#include <stdio.h>
#include "../config/config.h"

static void handle_function_call(
    t_symbolizer *symbolizer,
    btree_t_function_call_footprint **function_tree,
    t_shared_info *shared_info,
    char is_alloc)
{
    const config_t *config = get_config();
    if (config->test_functions)
    {
        char *function_name = shared_info->function_name;
        btree_char_ptr_node *found_node = btree_char_ptr_search(
            config->test_functions,
            &function_name);
        // if the function is not present that mean that we don't want to test it
        if (found_node == NULL)
            return;
    }
    if (config->ignore_functions)
    {
        char *function_name = shared_info->function_name;
        btree_char_ptr_node *found_node = btree_char_ptr_search(
            config->ignore_functions,
            &function_name);
        // if the function is present that mean that we want to ignore it
        if (found_node != NULL)
            return;
    }
    if (is_alloc)
        add_allocation(symbolizer, function_tree, shared_info);
    else
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
        case ALLOC:
        case FUNCTION_CALL:
            handle_function_call(
                params->symbolizer,
                params->function_tree,
                params->shared_memory,
                event == ALLOC);
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