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

	if (!(!shared_memory->treat_abort_as_crash && signo == SIGABRT))
	{
		strncpy(
			shared_memory->function_name,
			strsignal(signo),
			sizeof(shared_memory->function_name));
		get_backtrace(shared_memory->backtrace);
		shared_memory->event = CRASH;
	}
	exit(signo + 128);
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
	};

	for (size_t i = 0; i < sizeof(signals) / sizeof(signals[0]); i++)
	{
		if (signal(signals[i], sig_handler) == SIG_ERR)
		{
			dprintf(2, "[ERROR] signal failed\n");
			exit(1);
		}
	}
}
