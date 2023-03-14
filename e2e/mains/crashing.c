#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int main(void)
{
    raise(SIGSEGV);
    return 0;
}