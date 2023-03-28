
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *malloc_function(void)
{
	return (malloc(1000));
}

int main(void)
{
	void *ptr = malloc_function();
	if (ptr == NULL)
		return (1);
	free(ptr);

	void *array[10];

	for (int i = 0; i < 10; i++)
	{
		array[i] = malloc(1000);
		if (array[i] == NULL)
		{
			printf("it fail %i\n", i);
			return (0);
		}
		else {
			printf("it works %i\n", i);
		}
		memset(array[i], 0, 1000);
	}
	
	for (int i = 0; i < 10; i++)
		free(array[i]);

	ptr = malloc_function();
	memset(ptr, 0, 1000);
	free(ptr);
	printf("it works\n");
}

