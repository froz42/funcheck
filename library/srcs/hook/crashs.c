#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../shared_memory/shared_memory.h"
#include "../events/event_sender.h"
#include "../backtrace/backtrace.h"
#include "./hook.h"

static void sig_handler(int signo)
{
	disable_hooks();
	t_shared_info *shared_memory = get_shared_memory();
	t_event event = NONE;

	switch (signo)
	{
	case SIGSEGV:
	case SIGBUS:
	case SIGILL:
	case SIGFPE:
	case SIGABRT:
		event = CRASH;
		break;
	default:
		break;
	}
	if (event != NONE)
	{
		get_backtrace(shared_memory->backtrace);
		send_event(shared_memory, event);
	}
	exit(0);
}

void setup_signals(void)
{
	// handle all signals that quit the program
	static const int signals[] = {
		SIGSEGV,
		SIGBUS,
		SIGILL,
		SIGFPE,
		SIGABRT,
		SIGTERM,
		SIGPIPE,
		SIGINT,
		SIGQUIT,
		SIGKILL,
		SIGSTOP,
		SIGTSTP,
	};

	for (size_t i = 0; i < sizeof(signals) / sizeof(int); i++)
		signal(signals[i], sig_handler);
}
