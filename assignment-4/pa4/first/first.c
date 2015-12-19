#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "first.h"

/**
 * Global Variables:
 *	globals - array that holds the integer values of the provided input
 *	varTable - hash table to store all temporary/global instruction variables in
 *	instructions - array of instructions to be executed
 *	outs - array of all variables that are output variables
 *	instrctr - global number of instructions
 */
char globals[26];
struct hashtable *varTable;
struct instruction *instructions[100];
char outs[26];
int instrctr;

/**
 * Takes in user input of instruction file and input file, then passes them to respetive functions
 *
 * argv[1]: path to instruction file
 * argv[2]: path to input file
 *
 */
int main(int argc, char *argv[]) {
	FILE *fp;
	
	/* Make sure enough files were provided */
	if (argc < 3) {
		printf("error\n");
		return 0;
	}

	/* Open instruction file */
	fp = fopen(argv[1],"r");
	if (fp == NULL) {
		printf("error\n");
		return 0;
	}
	
	/* Create empty instruction table, and pass the file pointer to the readInstructions function */
	create_table(100);
	readInstructions(fp);
	fclose(fp);
	
	/* Open input file */
	fp = fopen(argv[2],"r");
	if (fp == NULL) {
		printf("error\n");
		return 0;
	}
	readInputs(fp);
	
	/* Free memory */
	cleanup();

	return 0;
}

void evalInstruction(int i) {
	struct instruction *ins = instructions[i];
	
	int j;
	int val = 0;
	int jt;
	/* If this stays as 1, we have already resolved any variables this instruction creates */
	int isdone = 1;

	/* Check if we already have the values for this instruction's variables */
	for (j = 0; ins->names[j] != '\0'; j++) {
		if (getVar(ins->names[j]) == -1) {
			isdone = 0;
			break;
		}
	}
	if (isdone) return;
	
	
	jt = ins->numInputs;
	if (ins->op == MULTIPLEXER) {
		jt += ins->numOutputs;
	}
	for (j = 0; j < jt; j++) {
		int jinp;
		if (ins->op == MULTIPLEXER) {
			if (j >= ins->numInputs) jinp = ins->vars[j-ins->numInputs];
			else jinp = ins->inputs[j];
		}
		else jinp = ins->vars[j];
		if (isalpha(jinp)) {
			char jinc = jinp;
			if (getVar(jinc) == -1) {
				int jj;
				for (jj = i+1; jj < instrctr; jj++) {
					struct instruction *tins = instructions[jj];
					int jk;
					int breakout = 0;
					for (jk = 0; tins->names[jk] != '\0'; jk++) {
						if (tins->names[jk] == jinp) {
							evalInstruction(jj);
							breakout = 1;
							break;
						}
					}
					if (breakout) break;
				}
			}
			if (getVar(jinc) == -1) {
				int jj;
				for (jj = i-1; jj >= 0; jj--) {
					struct instruction *tins = instructions[jj];
					int jk;
					int breakout = 0;
					for (jk = 0; tins->names[jk] != '\0'; jk++) {
						if (tins->names[jk] == jinp) {
							evalInstruction(jj);
							breakout = 1;
							break;
						}
					}
					if (breakout) break;
				}
			}
		}
		
	}
	if (ins->op == AND || ins->op == OR || ins->op == NOT) {
		val = ins->vars[0];
		if (val > 1) val = getVar(val);
	}
	if (ins->op == AND) {
		int p;
		for (p = 1; p < ins->numInputs;p++) {
			int tvv = ins->vars[p];
			if (tvv > 1) tvv = getVar(tvv);
			val = val && tvv;
		}
	}
	else if (ins->op == OR) {
		int p;
		for (p = 1; p < ins->numInputs;p++) {
			int tvv = ins->vars[p];
			if (tvv > 1) tvv = getVar(tvv);
			val = val || tvv;
		}
	}
	else if (ins->op == NOT) {
		val = !val;
	}
	else if (ins->op == MULTIPLEXER) {
		int tv = 0;
		int ic;
		int ic_pos = 0;

		for (ic = ins->numOutputs-1; ic >= 0; ic--) {
			int tvv = ins->vars[ic];
			if (isalpha(tvv)) tvv = getVar(tvv);
			else if (tvv > 1) tvv = tvv - '0';
			tv += (1<<ic_pos++)*tvv;
		}
		val = ins->inputs[tv];
		if (isalpha(val)) val = getVar(val);
		if (val > 1) val = val - '0';
	}
	else if (ins->op == DECODER) {
		char *bstr = (char*)malloc(sizeof(char)*ins->numInputs);
		int total = 0;
		int ic;
		char *start;
		for (ic = 0; ic < ins->numInputs; ic++) {
			int tempv = ins->vars[ic];
			if (isalpha(tempv)) tempv = getVar(tempv);
			bstr[ic] = '0' + tempv;
		}
		bstr[ic] = '\0';
		start = &bstr[0];
		while (*start)
		{
		 total *= 2;
		 if (*start++ == '1') total += 1;
		}

		for (ic = 0; ic < ins->numOutputs; ic++) {
			setVar(ins->names[ic],ic==total);
		}
		return;
	}
	setVar(ins->names[0],val);
}

int int2gray(int n) {
	return n ^ (n >> 1);
}

void runInputs() {
	int i;
	for (i = 0; i < instrctr; i++) evalInstruction(i);
}

void readInputs(FILE *fp) {
	char c;
	int i;
	int ctr = 0;

	for (;;) {
		if ( (c = fgetc(fp)) == '\n' ) {
			runInputs();
			for (i = 0; outs[i] != '\0'; i++) {
				printf("%d ",getVar(outs[i]));
			}
			printf("\n");
			destroy_table(varTable);
			create_table(100);
			
			ctr = 0;
		}
		else {
			if (c == EOF) break;
			if (!isdigit(c)) continue;
			setVar(globals[ctr++],c-'0');
		}
	}
}

void readInstructions(FILE *fp) {
	char buffer[512];
	instrctr = 0;
	
	for (;;) {
		if (fscanf(fp, "%s", buffer) != EOF) {
			struct instruction *ins;
			
			if (strcmp(buffer,"INPUTVAR") == 0) {
				int numInputs,ictr;
				fscanf(fp, "%s", buffer);
				numInputs = atoi(buffer);
				
				for (ictr = 0; ictr < numInputs; ictr++) {
					fscanf(fp, "%s", buffer);
					globals[ictr] = buffer[0];
				}
			}
			else if (strcmp(buffer,"OUTPUTVAR") == 0) {
				int numOutputs,ictr;
				fscanf(fp, "%s", buffer);
				numOutputs = atoi(buffer);
				
				for (ictr = 0; ictr < numOutputs; ictr++) {
					fscanf(fp, "%s", buffer);
					outs[ictr] = buffer[0];
				}
				outs[ictr+1] = '\0';
			}
			else if (strcmp(buffer,"MULTIPLEXER") == 0) {
				int citr = 0;

				ins = create_instruction();
				ins->op = MULTIPLEXER;
				fscanf(fp, "%s", buffer);
				ins->numInputs = atoi(buffer);
				ins->numOutputs = log10(ins->numInputs)/log10(2);
				ins->inputs = (int*)malloc(sizeof(int)*ins->numInputs);
				ins->vars = (int*)malloc(sizeof(int)*ins->numOutputs);
							
				for (citr = 0; citr < ins->numInputs; citr++) {
					fscanf(fp, "%s", buffer);
					ins->inputs[int2gray(citr)] = buffer[0];
				}
				
				for (citr = 0; citr < ins->numOutputs; citr++) {
					fscanf(fp, "%s", buffer);
					ins->vars[citr] = buffer[0];
				}
				fscanf(fp, "%s", buffer);
				ins->names = (char*)malloc(sizeof(char));
				ins->names[0] = buffer[0];
				instructions[instrctr++] = ins;
				
			}
			else if (strcmp(buffer,"DECODER") == 0) {
				int citr = 0;
				
				ins = create_instruction();
				ins->op = DECODER;
				
				fscanf(fp, "%s", buffer);
				ins->numInputs = atoi(buffer);
				ins->numOutputs = pow(2,ins->numInputs);
				
				ins->vars = (int*)malloc(sizeof(int)*ins->numInputs);
				ins->names = (char*)malloc(sizeof(char)*ins->numOutputs);
				
				for (citr = 0; citr < ins->numInputs; citr++) {
					fscanf(fp, "%s", buffer);
					ins->vars[citr] = buffer[0];
				}
				
				for (citr = 0; citr < ins->numOutputs; citr++) {
					fscanf(fp, "%s", buffer);
					ins->names[int2gray(citr)] = buffer[0];
				}
				instructions[instrctr++] = ins;
				
			}
			else if (strcmp(buffer,"AND") == 0 || strcmp(buffer,"NOT") == 0 || strcmp(buffer,"OR") == 0) {
				char c;
				int citr = 0;
				
				ins = create_instruction();
				if (buffer[0] == 'A') ins->op = AND;
				else if (buffer[0] == 'N') ins->op = NOT;
				else if (buffer[0] == 'O') ins->op = OR;
				
				ins->vars = (int*)malloc(sizeof(int)*26);
				
				while ((c = fgetc(fp)) != '\n') {
					if (c == ' ') continue;
					ins->vars[citr++] = c;
				}
				ins->numInputs = --citr;
				ins->names = (char*)malloc(sizeof(char));
				ins->names[0] = ins->vars[citr];
				instructions[instrctr++] = ins;
			}
		}
		else break;
	}
}

void cleanup() {
	int i;
	for (i = 0; instructions[i] != NULL; i++) {
		struct instruction *ins = instructions[i];
		/*
		if (ins->inputs != NULL) {
			free(ins->inputs);
			ins->inputs = NULL;
		}
		if (ins->names != NULL){
			free(ins->names);
			ins->names = NULL;
		}
		if (ins->vars != NULL) {
			free(ins->vars);
			ins->vars = NULL;
		}
		*/
		free(ins);
		ins = NULL;
	}
}

struct instruction *create_instruction() {
	struct instruction *ins = (struct instruction*)malloc(sizeof(struct instruction));
	return ins;
}

void destroy_table() {
	int i;

	for (i = 0; i < varTable->size; i++) {
		destroy_node(varTable->table[i]);
	}
	free(varTable->table);
	varTable->table = NULL;
	free(varTable);
	varTable = NULL;
}

void destroy_node(struct node *ptr) {
	if (ptr == NULL) return;
	if (ptr->next != NULL) {
		destroy_node(ptr->next);
	}
	free(ptr);
	ptr = NULL;
}

void create_table(int size) {
	int x;

	varTable = (struct hashtable*) malloc(sizeof(struct hashtable));
	varTable->size = size;
	varTable->table = malloc(sizeof(struct node*) * size);

	for (x = 0; x < size; x++) {
		varTable->table[x] = NULL;
	}
}

void setVar(char letter, int value) {
	int key;
	struct node *ptr;
	struct node *set_node;

	key = letter % varTable->size;
	if (varTable->table[key] != NULL && varTable->table[key]->letter == letter) {
		varTable->table[key]->value = value;
		return;
	}

	set_node = (struct node*) malloc(sizeof(struct node));
	set_node->value = value;
	set_node->letter = letter;
	set_node->next = NULL;

	if (varTable->table[key] == NULL) {
		varTable->table[key] = set_node;
		return;
	}
	ptr = varTable->table[key];
	while (ptr->next != NULL) {
		ptr = ptr->next;
	}
	ptr->next = set_node;
}

int getVar(char letter) {
	int key = letter % varTable->size;

	struct node *head = varTable->table[key];
	if (head == NULL) {
		return -1;
	}
	if (head->letter == letter) return head->value;
	
	while (head != NULL && head->letter != letter) head = head->next;
	if (head != NULL && head->letter == letter) return head->value;
	return -1;
}
