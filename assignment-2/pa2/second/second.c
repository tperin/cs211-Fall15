#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "second.h"

#define MAX_WORD_SIZE 512

/* 
 * Head will be used to store head of trie, LINE_COUNTER stores which line of the map file is being parsed 
 */
struct node *head;
int map_line_counter = 1;

int main(int argc, char **argv) {
	FILE *map_file;
	char linebuffer[512];

	if (argc < 2 || !(map_file = fopen(argv[1],"r"))) {
		printf("invalid input\n");
		return 0;
	}
	while (fgets(linebuffer,sizeof(linebuffer),map_file)) {
		FILE *dict_file;
		FILE *data_file;

		/* Pull out file names from line */
		char *dictfn = strtok(linebuffer," ");
		char *datafn = strtok(NULL," \n");

		head = NULL;

		/* Read dict file, add words to trie, close dict file */
		dict_file = fopen(dictfn,"r");
		if (!dict_file) {
			printf("invalid input\n");
			return 0;
		}
		readDict(dict_file);
		fclose(dict_file);
		
		/* Read data file, which will find all words and increment trie accordingly */
		data_file = fopen(datafn,"r");
		if (!data_file) {
			printf("invalid input\n");
			return 0;
		}
		readData(data_file);
		fclose(data_file);
		
		/* Print trie to file and free it so we can continue to next file pair */	
		printResult();
		deallocTrie(head);
		
		map_line_counter += 1;
	}
	fclose(map_file);
	return 0;
}

void readDict(FILE *dict_file) {
	char c;
	size_t cindex = 0;
	char buffer[MAX_WORD_SIZE];

	/* Loop through every char in file */
	for (;;) {
		if ( (c = fgetc(dict_file)) == EOF || !isalpha(c) ) {
			/* We have a non-alpha char, check if we have a word stored in the buffer, if so call function to add it to trie */
			if (cindex > 0) {
				buffer[cindex] = '\0';
				addDictWord(buffer);
				cindex = 0;
			}
			/* If we reach EOF we can break, the last word will have already been added */
			if (c == EOF) break;
		}
		else {
			/* We have alphabetic char, get its lowercase val and add it to word buffer */
			buffer[cindex++] = tolower(c);	
		}
	}
}

void readData(FILE *data_file) {
	char c;
	size_t cindex = 0;
	char buffer[MAX_WORD_SIZE];

	/* Same thing as readDict, except this time we call matchStr instead of addDictWord */
	for (;;) {
		if ( (c = fgetc(data_file)) == EOF || !isalpha(c) ) {
			if (cindex > 0) {
				buffer[cindex] = '\0';
				matchStr(buffer);
				cindex = 0;
			}
			if (c == EOF) break;
		}
		else {
			buffer[cindex++] = tolower(c);
		}	
	}
}

struct node *createNode(void) {
	struct node *nNode = (struct node*) malloc(sizeof(struct node));
	int x;

	nNode->alpha = -1;
	nNode->wordcount = 0;
	nNode->word = NULL;

	/* Set all children to 0, so we can check if they don't exist without having to malloc space for 26 new nodes every time we have a new node */
	for (x = 0; x < 26; x++) {
		nNode->children[x] = 0;
	}
	return nNode;
}

void addDictWord(char *word) {
	int x;
	struct node *ptr;
	int wordc = strlen(word);

	if (!head) head = createNode();

	/* Traverse tree, adding nodes if needed, until we finish the word */
	ptr = head;
	for (x = 0; x < wordc; x++) {
		int curc_a = tolower(word[x]) - 'a';

		if (!ptr->children[curc_a]) {
			ptr->children[curc_a] = createNode();
			ptr->children[curc_a]->alpha = curc_a;
		}
		ptr = ptr->children[curc_a];
	}
	/* Set word member for the final node */
	ptr->word = (char*) malloc(sizeof(char) * (wordc+1));
	for (x = 0; x < wordc; x++) {
		ptr->word[x] = word[x];
	}
	ptr->word[wordc] = '\0';
}

void matchStr(char *str) {
	int x;
	struct node *ptr;
	int strc = strlen(str);

	if (!head) return;

	ptr = head;

	for (x = 0; x < strc; x++) {
		int curc_a = tolower(str[x]) - 'a';
		if (!(ptr = ptr->children[curc_a])) return;
	}
	ptr->wordcount += 1;
}

void printResult() {
	FILE *out_file;
	char out_filename[64];
	
	sprintf(out_filename, "out%d.txt", map_line_counter);
	out_file = fopen(out_filename,"w");
	if (!out_file) {
		printf("error\n");
		return;
	}

	traverseWrite(out_file,head,0);
	fclose(out_file);
}

void traverseWrite(FILE *out_file, struct node *ptr, int parent_words) {
	int i;

	if (!ptr) return;

	if (ptr->word) fprintf(out_file,"%s %d %d\n",ptr->word,ptr->wordcount,parent_words);
	
	parent_words += ptr->wordcount;

	for (i = 0; i < 26; i++) {
		traverseWrite(out_file,ptr->children[i],parent_words);
	}
}

void deallocTrie(struct node *ptr) {
	int x;

	if (!ptr) return;

	for (x = 0; x < 26; x++) {
		deallocTrie(ptr->children[x]);
	}
	if (ptr->word) free(ptr->word);

	free(ptr);
}
