#include <stdio.h>

int errMsg(const char* message)
{

	char wait[258];
	fprintf(stderr, "ERROR: %s", message);
	scanf("%s", wait);
	printf("\e[2A\e[J");
	return 0;

}
