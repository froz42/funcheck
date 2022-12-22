#include <unistd.h>

int main(void)
{
    int d = dup(0);
    if (d == -1)
        return 1;
}