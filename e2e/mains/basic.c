#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char *str = strdup("Hello world\n");
    if (!str)
        return EXIT_FAILURE;
    printf("%s", str);
    free(str);
    return EXIT_SUCCESS;
}