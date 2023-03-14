#include "bool.h"
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

void raise_error(const char *message, bool_t show_errno)
{
    if (show_errno)
        dprintf(STDERR_FILENO, "[FUNCHECK LIBRARY] [FATAL ERROR] %s: %s", message, strerror(errno));
    else
        dprintf(STDERR_FILENO, "[FUNCHECK LIBRARY] [FATAL ERROR] %s", message);
    abort();
}