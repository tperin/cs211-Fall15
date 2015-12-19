#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "second.h"

int main (int argc, char **argv) {
	/* Declare vars */
	struct node *ptr;
	char buffer[512];
	FILE *fp;
	struct node *head = NULL;
	
	if (argc < 2) {
		printf("error\n");
		return 0;
	}

	fp = fopen(argv[1],"r");
	if (fp == NULL) {
		printf("error\n");
		return 0;
	}

	while (fgets(buffer,sizeof(buffer),fp)) {
		int num;
		const char *letter = strtok(buffer,"\t");
		char *numchar = strtok(NULL,"\t");
		if (numchar == NULL) {
			printf("error\n");
			return 0;
		}
		num = atoi(numchar);
		
		if (strcmp(letter,"i") == 0) {
			insertNode(&head,num);
		}
		else if (strcmp(letter,"d") == 0) {
			deleteNode(&head,num);
		}
		else {
			printf("error\n");
			return 0;
		}
	}
	ptr = head;
	while (ptr != NULL) {
		printf("%d",ptr->value);
		if (ptr->next != NULL) printf("\t");
		ptr = ptr->next;
	}
	printf("\n");
	return 0;
}

void insertNode(struct node **head, int num) {
	struct node *ptr, *nextptr;
	struct node *newNode = (struct node*) malloc(sizeof(struct node));
	newNode->value = num;
	
	if (*head == NULL) {
		*head = newNode;
		return;
	}
	if ((*head)->value == num) {
		return;
	}

	if (num < (*head)->value) {
		newNode->next = *head;
		*head = newNode;
		return;
	}

	ptr = *head;
	nextptr = (*head)->next;
	
	while (nextptr != NULL) {
		if (num == nextptr->value) {
			return;
		}
		if (num < nextptr->value) {
			newNode->next = nextptr;
			ptr->next = newNode;
			return;
		}
		
		ptr = nextptr;
		nextptr = nextptr->next;
	}
	if (ptr->value == num) {
		return;
	}	
	ptr->next = newNode;
}

void deleteNode(struct node **head, int num) {
	struct node *ptr;
	struct node *nextptr;
	
	if (*head == NULL) {
		return;
	}
	
	if ((*head)->value == num) {
		if ((*head)->next == NULL) {
			*head = NULL;
			return;
		}
		*head = (*head)->next;
	}
	
	ptr = *head;
	nextptr = ptr->next;

	while (nextptr != NULL) {
		if (nextptr->value == num) {
			struct node *temp = nextptr->next;
			ptr->next = temp;
			nextptr = temp;
		}
		
		else {
			ptr = nextptr;
			nextptr = ptr->next;	
		}
	}
}	
