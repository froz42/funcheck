#include <stdlib.h>
#include <signal.h>

int main(void)
{
    void *ptr = calloc(1, 1);
    if (!ptr)
        raise(SIGSEGV);
    return 0;
}