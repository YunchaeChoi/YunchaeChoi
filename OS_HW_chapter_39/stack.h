#ifndef __STACK__H
#define __STACK__H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 4096

int *stack;
int size=0;

typedef char* Data; // changable

typedef struct _stackNode {
	Data data;
	struct _stackNode *next;
}stackNode;

stackNode* top; // pointer to the top node


void push(Data data)  { // size: length of the line
	stackNode* temp = (stackNode*) malloc(sizeof(stackNode));
//	temp->data = (Data) malloc(sizeof(char) * (size+1));
	if(!temp) {
		perror("malloc failed\n");
		exit(EXIT_FAILURE);
	}
	temp->data = data;
	temp->next = top;
	top = temp;
	if(top)
		printf("top: %s", top->data);
	size ++;
}

Data pop() {
	Data item;
	stackNode* temp = top;

	if(top == NULL) {
		perror("stack is empty\n");
		return NULL;
	}
	else {
		item = temp->data;
		top = temp->next;
		free(temp->data);
		free(temp);
		size --;
		return item;
	}
}

int isEmpty() {
	if(top == NULL) {
		return 1; //empty
	}
	return 0; // not empty
}

void delAllNode() {
	stackNode* temp = top;
	if(!isEmpty()) {
		temp = top;
		top = temp->next;
		free(temp);
	}
}

void printStack() {
	stackNode* p = top;
	printf("\n----------print stack---------\n");
	while(p) {
		printf("%s\n", p->data);
		p = p->next;
	}
	printf("done.\n");
}

int sizeStack() {
	return size;
}






#endif
