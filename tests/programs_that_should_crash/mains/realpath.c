#include <stdlib.h>
#include <signal.h>

int main(void)
{
    char *ptr = realpath("/dev/null", NULL);
    if (!ptr)
        raise(SIGSEGV);
    return 0;
}