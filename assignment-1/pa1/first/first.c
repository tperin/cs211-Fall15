#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv) {
	int num;
	int x;

	if (argc < 2) {
		printf("error\n");
		return 0;
	}
	
	num = atoi(argv[1]);
	
	if (num <= 1) {
		printf("no\n");
		return 0;
	}
	for (x = 2; x < num; x++) {
		if (num % x == 0) {
			printf("no\n");
			return 0;
		}
	}
	printf("yes\n");
	return 0;
}
