#include "../utils/bool.h"
#include "../config/config.h"
#include "output.h"

bool_t is_json_output_set(void)
{
    return is_option_set(JSON_OUTPUT_MASK, get_config());
}

void output_file(FILE *file)
{
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), file))
        printf("%s", buffer);
}