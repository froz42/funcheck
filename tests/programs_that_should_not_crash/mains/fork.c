#include <unistd.h>

int main(void)
{
    int pid = fork();
    if (pid == -1)
        return 1;
    return 0;
}