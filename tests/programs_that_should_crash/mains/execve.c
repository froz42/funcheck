#include <signal.h>
#include <unistd.h>

int main(void)
{
    char *const argv[] = {"/bin/ls", NULL};
    if (execve("/bin/ls", argv, 0) == -1)
        raise(SIGSEGV);
    return 0;
}