#include <unistd.h>

int main(void)
{
    int d = dup2(0, 0);
    if (d == -1)
        return 1;
    return 0;
}