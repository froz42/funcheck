#ifndef LOADING_BAR_H
#define LOADING_BAR_H

#include <stdlib.h>
#include "../../time/time.h"

void erase_line(void);
void write_loading_bar(size_t current, size_t total, msseconds_t time_remaining);

#endif