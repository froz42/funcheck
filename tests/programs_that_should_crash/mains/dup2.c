#include <unistd.h>
#include <signal.h>

int main(void)
{
    int d = dup2(0, 0);
    if (d == -1)
        raise(SIGSEGV);
    return 0;
}