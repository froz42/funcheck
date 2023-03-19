#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	long long unsigned int *p = (void *)0xdeadbeef;
    printf("zu: %zu\n", p);
    return EXIT_SUCCESS;
}