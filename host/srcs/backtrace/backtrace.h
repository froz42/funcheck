/* Funcheck - A tool for checking functions calls return protections
 * Copyright (C) 2023  Theo Matis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BACKTRACE_H
#define BACKTRACE_H

#include "../../../shared/shared.h"
#include "../utils/bool.h"
#include "../symbolizer/symbolizer.h"

t_address_info *backtrace_process(
    t_address_info *dest,
    t_symbolizer *symbolizer,
    ptr_address *backtrace);
bool_t should_ignore_function(const char *function_name);

#endif