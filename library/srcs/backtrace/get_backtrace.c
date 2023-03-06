#define _GNU_SOURCE

#include <dlfcn.h>
#include <execinfo.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <linux/limits.h>
#include <unistd.h>
#include "../utils/config.h"
#include "../../../shared/shared.h"
#include "../utils/error.h"

#define FALLBACK_MAIN_POS 2

static void process_adresses(void **backtrace_buffer, int backtrace_size)
{
	int i;
	for (i = 0; i < backtrace_size; i++)
	{
		Dl_info info;
		if (!dladdr(backtrace_buffer[i], &info))
			raise_error("process_adresses: dladdr", true);
		backtrace_buffer[i] = (void *)((size_t)backtrace_buffer[i] - (size_t)info.dli_fbase - 1);
	}
	backtrace_buffer[i] = NULL;
}

void get_backtrace(ptr_address *dest)
{
	void *buffer[MAX_BACKTRACE_DEPTH + 1];
	int backtrace_size = backtrace(buffer, MAX_BACKTRACE_DEPTH);
	if (backtrace_size == -1)
		raise_error("get_backtrace: backtrace", true);
	process_adresses(buffer, backtrace_size);
	int i;
	for (i = 0; i < backtrace_size; i++)
		dest[i] = (ptr_address)buffer[i];
	dest[backtrace_size] = 0x0;
}