#ifndef _bst_h
#define _bst_h

typedef struct node {
	int value;
	struct node *left;
	struct node *right;
} node;

extern int temp_level_holder;

struct node *create_node(int value);

int insert(struct node **head, int value);

struct node *insert_helper(struct node **head, int value, int level);

int del(struct node **head, int value);

struct node *del_helper(struct node *head, int value);

int search(struct node *head, int value,int level);

#endif
