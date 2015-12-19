#include <stdlib.h>
#include <stdio.h>
#include "bst.h"

int temp_level_holder;

struct node *create_node(int value) {
	struct node *node = (struct node*) malloc(sizeof(struct node));
	node->value = value;
	node->left = NULL;
	node->right = NULL;
	return node;
}

int insert(struct node **head, int value) {
	temp_level_holder = 0;
	insert_helper(head,value,0);
	return temp_level_holder;
}

struct node *insert_helper(struct node **head, int value, int level) {
	temp_level_holder = level;
	if (*head == NULL) {
		*head = create_node(value);
		return *head;
	}
	if (value < (*head)->value) {
		(*head)->left = insert_helper(&(*head)->left,value,level+1);	
	}
	else if (value > (*head)->value) {
		(*head)->right = insert_helper(&(*head)->right, value, level+1);
	}
	else {
		temp_level_holder = -1;
	}
	return *head;
}
int del(struct node **head, int value) {
	if (*head == NULL) {
		return 0;
	}
	if (search(*head,value,0) == -1) {
		return 0;
	}
	*head = del_helper(*head,value);
	return 1;
}
struct node *del_helper(struct node *head, int value) {
	if (head == NULL) {
		return NULL;
	}
	if (value < head->value) {
		head->left = del_helper(head->left,value);
	}
	else if (value > head->value) {
		head->right = del_helper(head->right,value);
	}
	else {
		struct node *min_right;
		if (head->left == NULL) {
			return head->right;
		}
		if (head->right == NULL) {
			return head->left;
		}
		min_right = head->right;
		while (min_right->left != NULL) {
			min_right = min_right->left;
		}
		head->value = min_right->value;
		head->right = del_helper(head->right,min_right->value);	
	}
	return head;
}

void recadd(struct node **master, struct node *head, int ignore) {
	if (head != NULL) {
		if (head->value != ignore) {
			insert(master,head->value);
		}
		recadd(master,head->left,ignore);
		recadd(master,head->right,ignore);
	}	
}

int search(struct node *head, int value,int level) {
	int left,right;

	level += 1;
	if (head == NULL) {
		return -1;
	}
	if (head->value == value) {
		return level;
	}
	left = search(head->left,value,level);
	if (left != -1) {
		return left;
	}
	right = search(head->right,value,level);
	if (right != -1) {
		return right;
	}
	return -1;
}





