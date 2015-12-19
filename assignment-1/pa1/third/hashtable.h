#ifndef _hashtable_h
#define _hashtable_h

typedef struct node {
	int value;
	struct node *next;
} node;

typedef struct hashtable {
	int size;
	struct node **table;
} hashtable;

struct hashtable *create_table(int size);

/* Inserts value into hashtable. If it is a duplicate, it adds to end of linked list. Returns 1 if duplicate, 0 if not */
int insert(struct hashtable *table, int value);

/* Search hashtable for value. Returns 1 if exists, 0 if not */
int search(struct hashtable *table, int value);

#endif
