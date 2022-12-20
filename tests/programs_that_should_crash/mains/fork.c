#include <unistd.h>
#include <signal.h>

int main(void)
{
    int pid = fork();
    if (pid == -1)
        raise(SIGSEGV);
    return 0;
}