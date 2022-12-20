#include <sys/mman.h>
#include <signal.h>
#include <stdlib.h>

int main(void)
{
    void *ptr = mmap(NULL, 10, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED)
        raise(SIGSEGV);
    return 0;
}