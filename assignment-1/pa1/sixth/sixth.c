#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sixth.h"

int main(int argc, char **argv) {
	int x;

	if (argc < 2) {
		printf("error\n");
		return 0;
	}
	
	for (x = 1; x < argc; x++) {
		printf("%c",argv[x][0]);
	}
	printf("\n");
	
	return 0;
}
