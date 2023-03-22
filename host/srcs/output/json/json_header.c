#include "../../json/json.h"
#include "../output.h"

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