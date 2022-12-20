#include <unistd.h>
#include <signal.h>

int main(void)
{
    if (getpid() == -1)
        raise(SIGSEGV);
    return 0;
}