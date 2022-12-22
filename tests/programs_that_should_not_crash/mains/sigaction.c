#include <signal.h>

int main(void)
{
    struct sigaction act;
    act.sa_handler = SIG_IGN;
    if (sigaction(SIGSEGV, &act, 0) == -1)
        return 1;
    return 0;
}