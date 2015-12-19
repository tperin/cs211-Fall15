#ifndef _first_h
#define _first_h

/*
 * node struct
 * 
 * Used as nodes for a trie for words
 * Members:
 * 	alpha (int): stores the alpabetic index for the node
 * 	word (char*): string for nodes that are the last of their word, can be used to tell if the node is a full word or just a part of one
 * 	wordcount (int): number of times word shows up in data file
 * 	prefixcount (int): number of times a prefix of this word shows up in data file
 * 	children (struct node[]): array of 26 child nodes, one for each letter of the alphabet
 *
 */
struct node {
        int alpha;
        char *word;
        int wordcount;
        int prefixcount;
        struct node *children[26];
};

/*
 * Function readDict
 *
 * Reads the provided dictionary file and adds all words to a trie
 *
 * Parameters:
 * 	dict_file (FILE*): the file pointer for the dictionary file to read
 *
 * Returns:
 * 	void
 *
 */
void readDict(FILE *dict_file);

/*
 * Function readData
 *
 * Finds all words in provided data file
 *
 * Parameters:
 * 	data_file (FILE*): the file pointer for the data file to read
 *
 * Returns:
 * 	void
 *
 */
void readData(FILE *data_file);

/*
 * Function createNode
 *
 * Creates a node struct with default member values, sets all children to 0
 *
 * Parameters:
 * 	
 * Returns:
 * 	struct node*: pointer to created node
 *
 */
struct node *createNode();

/*
 * Function addDictWord
 *
 * Adds provided word to the trie structure
 *
 * Parameters:
 * 	word (char*): the word to add to the trie
 *
 * Returns:
 * 	void
 *
 */
void addDictWord(char *word);

/*
 * Function matchStr
 *
 * Traverses the trie for provided string and increases count of any matches
 *
 * Parameters:
 * 	str (char*): string to add to trie
 *
 * Returns:
 * 	void
 *
 */
void matchStr(char *str);

/*
 * Function printResult
 *
 * Called to print all words with counts from trie structure
 *
 * Parameters:
 *
 * Returns:
 * 	void
 *
 */
void printResult();

/*
 * Function traverseWrite
 *
 * Called by printResult to recursively traverse trie from leftmost to rightmost, writing all words to file
 *
 * Paramters:
 *	out_file (FILE*): file pointer for the file to write to
 *	ptr (struct node*): pointer to the current head we are traversing
 *
 * Returns:
 * 	void
 *
 */
void traverseWrite(FILE *out_file, struct node *ptr);

/* Function deallocTrie
 *
 * Traverses trie recursively and frees all child nodes. Frees word if applicable, then frees head
 *
 * Paramters:
 * 	ptr (struct node*): pointer to node to be freed
 *
 * Returns:
 * 	void
 *
 */
void deallocTrie(struct node *ptr);

#endif
