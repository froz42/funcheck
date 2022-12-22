#include <sys/socket.h>
#include <signal.h>

int main(void)
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1)
        return 1;
    return 0;
}