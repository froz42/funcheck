#include <unistd.h>
#include <signal.h>

int main(void)
{
    if (getpid() == -1)
        return 1;
    return 0;
}