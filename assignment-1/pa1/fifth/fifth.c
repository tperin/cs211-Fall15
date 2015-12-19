#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fifth.h"

int main(int argc, char **argv) {
	FILE *fp;
	int rows1,cols1,rows2,cols2,r_itr,c_itr,prows,pcols;
	char buffer[512];
	int **product;
	int **matrix1;
	int **matrix2;

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
	rows1 = atoi(strtok(buffer,"\t"));
	cols1 = atoi(strtok(NULL,"\t"));

	if (rows1 == 0 || cols1 == 0) {
		printf("error\n");
		return 0;
	}

	matrix1 = create(rows1,cols1);
	
	r_itr = 0;
	while (fgets(buffer,sizeof(buffer),fp)) {
		char *input;
	
		if (strcmp(buffer,"\n") == 0) {
			break;
		}
		
		c_itr = 0;
		input = strtok(buffer,"\t");
		while (input != NULL) {
			matrix1[r_itr][c_itr] = atoi(input);
			input = strtok(NULL,"\t");
			c_itr += 1;
		}
		r_itr += 1;
	}

	fgets(buffer,sizeof(buffer),fp);
	rows2 = atoi(strtok(buffer,"\t"));
	cols2 = atoi(strtok(NULL,"\t"));

	if (rows2 == 0 || cols2 == 0 || rows2 != cols1) {
		printf("error\n");
		return 0;
	}

	matrix2 = create(rows2,cols2);
	
	r_itr = 0;
	while (fgets(buffer,sizeof(buffer),fp)) {
		char *input;

		c_itr = 0;
		input = strtok(buffer,"\t");
		while (input != NULL) {
			matrix2[r_itr][c_itr] = atoi(input);
			input = strtok(NULL,"\t");
			c_itr += 1;
		}
		r_itr += 1;
	}

	if (rows1 < rows2) {
		prows = rows2;
		pcols = cols1;
	}
	else {
		prows = rows1;
		pcols = cols2;
	}
	product = create(prows,pcols);
	
	r_itr = 0;
	while (r_itr < rows1) {
		int c = 0;
		while (c < cols2) {
			int sum = 0;
			c_itr = 0;
			while (c_itr < rows2) {
				sum += matrix1[r_itr][c_itr] * matrix2[c_itr][c];
				c_itr += 1;
			}
			product[r_itr][c] = sum;
			c += 1;
		}
		r_itr += 1;
	}

	r_itr = 0;
	while (r_itr < rows1) {
		c_itr = 0;
		while (c_itr < cols2) {
			printf("%d\t",product[r_itr][c_itr]);
			c_itr += 1;
		}
		printf("\n");
		r_itr += 1;
	}
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
