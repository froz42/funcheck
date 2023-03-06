#include "../shared_memory/shared_memory.h"
#include "../events/event_sender.h"
#include "../shared_memory/shared_memory.h"
#include <stdlib.h>
#include <stdio.h>

static void exit_hook(void)
{
	close_shared_memory();
}

void setup_exit_hook(void)
{
	atexit(exit_hook);
}