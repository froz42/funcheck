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

#include <string.h>
#include "../hook.h"

typedef char *char_ptr_t;

DEFINE_HOOK_FUNCTION(char_ptr_t, strdup, ENOMEM, NULL, const char *, s);
DEFINE_HOOK_FUNCTION(char_ptr_t, strndup, ENOMEM, NULL, const char *, s, size_t, n);
DEFINE_HOOK_FUNCTION(char_ptr_t, strsignal, ENOMEM, NULL, int, sig);
DEFINE_HOOK_FUNCTION(char_ptr_t, strerror, ENOMEM, NULL, int, errnum);