#ifndef LOGGER_H
#define LOGGER_H

# define COLOR_RED     "\x1b[31m"
# define COLOR_GREEN   "\x1b[32m"
# define COLOR_YELLOW  "\x1b[33m"
# define COLOR_BLUE    "\x1b[34m"
# define COLOR_MAGENTA "\x1b[35m"
# define COLOR_CYAN    "\x1b[36m"
# define COLOR_RESET   "\x1b[0m"

# define bool char
# define true 1
# define false 0

void log_info(const char *message);
void log_error(const char *msg, bool show_errno);
void log_warning(const char *message);
void log_success(const char *message);
void log_debug(const char *message);

#endif