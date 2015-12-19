#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bst.h"
#include "ninth.h"

int main (int argc, char **argv) {
	FILE *fp;
	char buffer[512];

	struct node *head = NULL;	
	
	if (argc < 2) {
		printf("error\n");
		return 0;
	}
	
	fp = fopen(argv[1],"r");
	if (fp == NULL) {
		printf("error\n");
		return 0;
	}

	while (fgets(buffer,sizeof(buffer),fp)) {
		char *letter = strtok(buffer,"\t");
		int number = atoi(strtok(NULL,"\t"));
		
		if (strcmp(letter,"i") == 0) {
			int lvl = insert(&head,number);
			if (lvl < 0) {
				printf("duplicate\n");
			}
			else {
				printf("inserted %d\n",(lvl+1));
			}	
		}
		else if (strcmp(letter,"s") == 0) {
			int level = search(head,number,0);
			if (level == -1) {
				printf("absent\n");
			}
			else {
				printf("present %d\n",level);
			}
		}
		else if (strcmp(letter,"d") == 0) {
			int success = del(&head,number);
			printf("%s\n",(success == 1 ? "success" : "fail"));
		}
		else {
			printf("error\n");
			return 0;
		}
	}
	printf("\n");
	return 0;	
}
