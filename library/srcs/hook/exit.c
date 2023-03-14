#include "../shared_memory/shared_memory.h"
#include "../events/event_sender.h"
#include "../shared_memory/shared_memory.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Close the shared memory on exit
 * 
 */
static void exit_hook(void)
{
	close_shared_memory();
}

/**
 * @brief Setup the exit hook
 * 
 */
void setup_exit_hook(void)
{
	atexit(exit_hook);
}