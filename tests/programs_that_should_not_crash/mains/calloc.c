#include <stdlib.h>

int main(void)
{
    void *ptr = calloc(1, 1);
    if (!ptr)
        return 1;
    return 0;
}