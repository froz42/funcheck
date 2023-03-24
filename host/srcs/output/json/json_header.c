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

#include "../../json/json.h"
#include "../output.h"
#include "../../host.h"

/**
 * @brief Write the header in json format
 * 
 * @param args_guest the arguments of the guest program
 */
void write_header_json(args_t args_guest)
{
    json_write_object(0);
    json_write_key_value_string(
        "funcheck-version",
        VERSION,
        1,
        false);
    json_write_key_value_string(
        "program",
        args_guest.argv[0],
        1,
        false);

    // write arguments
    json_write_key_array("arguments", 1);
    for (int i = 1; i < args_guest.argc; i++)
    {
        json_write_value_string(
            args_guest.argv[i],
            2,
            i == args_guest.argc - 1);
    }
    json_write_array_end(1, false);
}