#include <stdio.h>
#include <unistd.h>
#include "logger.h"
#include <string.h>
#include <errno.h>

/**
 * @brief Log info message
 * 
 * @param msg the message to log
 */
void log_info(const char *msg)
{
	dprintf(1, "[%sINFO%s] %s\n", COLOR_BLUE, COLOR_RESET, msg);
}

/**
 * @brief Log error message
 * 
 * @param msg the message to log
 * @param show_errno if true, strerror will be printed
 */
void log_error(const char *msg, bool show_errno)
{
	if (show_errno)
		dprintf(2, "[%sERROR%s] %s: %s\n", COLOR_RED, COLOR_RESET, msg, strerror(errno));
	else
		dprintf(2, "[%sERROR%s] %s\n", COLOR_RED, COLOR_RESET, msg);
}

/**
 * @brief log a warning message
 * 
 * @param msg the message to log
 */
void log_warning(const char *msg)
{
	dprintf(2, "[%sWARNING%s] %s", COLOR_YELLOW, COLOR_RESET, msg);
}

/**
 * @brief log a success message
 * 
 * @param msg the message to log
 */
void log_success(const char *msg)
{
	dprintf(1, "[%sSUCCESS%s] %s", COLOR_GREEN, COLOR_RESET, msg);
}

/**
 * @brief Log a debug message
 * 
 * @param msg the message to log
 */
void log_debug(const char *msg)
{
	dprintf(1, "[%sDEBUG%s] %s", COLOR_MAGENTA, COLOR_RESET, msg);
}

