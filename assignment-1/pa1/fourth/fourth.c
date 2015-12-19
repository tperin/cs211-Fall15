#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fourth.h"

int main(int argc, char **argv) {
	FILE *fp;
	int rows,cols,r_itr,c_itr;
	char buffer[512];
	int **sum;

	if (argc < 2) {
		printf("error\n");
		return 0;
	}

	fp = fopen(argv[1],"r");
	if (fp == NULL) {
		printf("error\n");
		return 0;
	}
	
	fgets(buffer,sizeof(buffer),fp);
	rows = atoi(strtok(buffer,"\t"));
	cols = atoi(strtok(NULL,"\t"));

	if (rows == 0 || cols == 0) {
		printf("error\n");
		return 0;
	}	
	
	sum = create(rows,cols);
	
	r_itr = 0;
	while (fgets(buffer,sizeof(buffer),fp)) {
		char *input;
	
		if (strcmp(buffer,"\n") == 0) {
			r_itr = 0;
			continue;
		}

		c_itr = 0;
		input = strtok(buffer,"\t");
		while (input != NULL) {
			sum[r_itr][c_itr] += atoi(input);
			input = strtok(NULL,"\t");
			c_itr += 1;
		}

		r_itr += 1;
	}
	
	print(sum,rows,cols);
	return 0;
}

int **create(int m, int n) {
	int **matrix;
	int x;

	matrix = (int**) malloc(m * sizeof(int*));
	for (x = 0; x < m; x++) {
		matrix[x] = (int*) malloc(n * sizeof(int));
	}
	
	return matrix;
}

void print(int **matrix, int m, int n) {
	int x,y;
	
	for (x = 0; x < m; x++) {
		for (y = 0; y < n; y++) {
			printf("%d",matrix[x][y]);
			if (y < (n - 1)) {
				printf("\t");
			}	
		}
			
		printf("\n");
	}
}
