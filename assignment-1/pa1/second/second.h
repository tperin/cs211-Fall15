#ifndef _second_h
#define _second_h

typedef struct node {
	int value;
	struct node *next;
} node;

void insertNode(struct node **head, int);

void deleteNode(struct node **head, int);

#endif
