#include <signal.h>

void func(int n)
{
    (void)n;
}

int main(void)
{
    if (signal(SIGSEGV, func) == SIG_ERR)
        return 1;
    return 0;
}