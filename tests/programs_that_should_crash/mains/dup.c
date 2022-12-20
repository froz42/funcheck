#include <unistd.h>
#include <signal.h>

int main(void)
{
    int d = dup(0);
    if (d == -1)
        raise(SIGSEGV);
}