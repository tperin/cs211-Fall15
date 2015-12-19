#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

int main (int argc, char *argv[]) {
	FILE *fp;
	char buffer[512];

	struct hashtable *table = NULL;

	if (argc < 2) {
		printf("error\n");
		return 0;
	}
	
	fp = fopen(argv[1],"r");
	if (fp == NULL) {
		printf("error\n");
		return 0;
	}
	
	table = create_table(10000);
	
	while (fgets(buffer,sizeof(buffer),fp)) {
		char *letter = strtok(buffer,"\t");
		int number = atoi(strtok(NULL,"\t"));

		if (strcmp(letter,"i") == 0) {
			int is_duplicate = insert(table,number);
			printf("%s\n",(is_duplicate == 0 ? "inserted" : "duplicate"));
		}
		else if (strcmp(letter,"s") == 0) {
			int is_found = search(table,number);
			printf("%s\n",(is_found == 0 ? "absent" : "present"));
		}
		else {
			printf("error\n");
		}
	}

	return 0;
}
