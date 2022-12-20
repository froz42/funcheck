#include <stdlib.h>
#include <signal.h>

int main(void)
{
    void *ptr = realloc(NULL, 10);
    if (!ptr)
        raise(SIGSEGV);
    return 0;
}