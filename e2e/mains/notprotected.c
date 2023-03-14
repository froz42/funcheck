#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int main(void)
{
    char *str = strdup("Hello world\n");
    if (!str)
        raise(SIGSEGV);
    printf("%s", str);
    free(str);
    return EXIT_SUCCESS;
}