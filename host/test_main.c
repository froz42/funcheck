#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

// int main(void)
// {
// 	char *string = NULL;
// 	size_t size_string = 0;
// 	char buffer[1024];

// 	while (1)
// 	{
// 		int ret = read(0, buffer, sizeof(buffer));
// 		if (ret == -1)
// 			break;
// 		if (ret == 0)
// 			break;
// 		buffer[ret] = '\0';
// 		string = realloc(string, size_string + ret + 1);
// 		memcpy(string + size_string, buffer, ret + 1);
// 		size_string += ret;
// 	}
// 	printf("%s", string);
// 	free(string);
// }

int main(void)
{
	char buffer[1024];
	int fd = open("test_main.c", O_RDONLY);
	if (fd == -1)
		abort();
	while (1)
	{
		int ret = read(fd, buffer, sizeof(buffer));
		if (ret == 0)
			break;
		buffer[ret] = '\0';
		printf("%s", buffer);
	}
}