#include <unistd.h>

int main(void)
{
    char *const argv[] = {"/bin/ls", NULL};
    if (execve("/bin/ls", argv, 0) == -1)
        return 1;
    return 0;
}