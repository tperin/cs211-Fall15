#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"

struct hashtable *create_table(int size) {
	int x;

	struct hashtable *table = (struct hashtable*) malloc(sizeof(struct hashtable));
	table->size = size;
	table->table = malloc(sizeof(struct node*) * size);
	
	for (x = 0; x < size; x++) {
		table->table[x] = NULL;
	}
	return table;
}

int insert(struct hashtable *table, int value) {
	int key;
	struct node *ptr;

	struct node *insert_node = (struct node*) malloc(sizeof(struct node));
	insert_node->value = value;
	insert_node->next = NULL;

	if (value < 0) {
		key = (value * -1) % table->size;
	}
	else {	
		key = value % table->size;
	}

	if (table->table[key] == NULL) {
		table->table[key] = insert_node;
		return 0;
	}
	if (table->table[key]->value == value) {
		return 1;
	}
	ptr = table->table[key];
	while (ptr->next != NULL) {
		ptr = ptr->next;
		if (ptr->value == value) {
			return 1;
		}
	}
	ptr->next = insert_node;
	return 0;	
}

int search(struct hashtable *table, int value) {
	int key;
	struct node *ptr;
	struct node *head;
	if (value < 0) {
		key = (value * -1) % table->size;
	}
	else {
		key = value % table->size;
	}
	
	head = table->table[key];
	if (head == NULL) {
		return 0;
	}
	if (head->value == value) {
		return 1;
	}
	ptr = head->next;
	while (ptr != NULL) {
		if (ptr->value == value) {
			return 1;
		}
		ptr = ptr->next;
	}
	return 0;
}
