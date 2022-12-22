#include <stdlib.h>
#include <signal.h>

int main(void)
{
    void *ptr = malloc(10);
    if (!ptr)
        return 1;
    return 0;
}