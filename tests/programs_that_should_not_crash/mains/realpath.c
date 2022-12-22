#include <stdlib.h>

int main(void)
{
    char *ptr = realpath("/dev/null", NULL);
    if (!ptr)
        return 1;
    return 0;
}