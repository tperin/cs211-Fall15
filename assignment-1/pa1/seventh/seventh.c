#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "seventh.h"

int main(int argc, char **argv) {
	int x;

	for (x = 1; x < argc; x++) {
		char *word = argv[x];
		int ctr = 0;
		while (*(word + ctr) != '\0') {
			ctr += 1;
		}
		printf("%c",word[ctr-1]);
	}
	printf("\n");
	return 0;
}
