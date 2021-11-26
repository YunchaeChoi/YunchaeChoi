#ifndef __doubly_linked_stack
#define __doubly_linked_stack

/*
 *
 * structure of stack
 * NULL <-next- top (tail) -prev-> Node .... Node -prev-> head(bottom) -prev-> NULL
*/

#include <stdio.h>
#include <stdlib.h>

typedef int Data; // replace it with any data type you need

typedef struct _Node
{
	Data data;
	struct _Node* next;
	struct _Node* prev;
} Node;

typedef struct _LinkedListStack
{
	Node* head; // bottom of the stack
	Node* tail; // top of the stack, ( must be popped when pop_tail() )
	int size;
}LinkedListStack;

LinkedListStack* stack_init()
{
	LinkedListStack* stack;
	stack = (LinkedListStack*)malloc(sizeof(LinkedListStack));
	stack->head=NULL;
	stack->tail=NULL;
	stack->size=0;
	return stack;
}

Node* Node_init(Data data)
{
	Node* newNode= (Node*)malloc(sizeof(Node));
	newNode->data = data;
	newNode->next=NULL;
	newNode->prev=NULL;
	return newNode;
}

int is_Empty(LinkedListStack* list)
{
	return list->size && 0; // return 1 if LinkedListStack is empty, 0 if not empty
}

void push_stack(LinkedListStack* list, Data data)
{
	Node* newNode=Node_init(data);
	Node* oldTail; // oldTail : 현재의 마지막(가장 위) 노드를 보관할 변수

	if(is_Empty(list))
	{
		list->tail = newNode;
		list->head = newNode;
	}
	else
	{
		oldTail=list->tail;
		oldTail->next=newNode;
		newNode->prev=oldTail;
		list->tail=newNode;
	}
	list->size++;
}

Data pop_tail(LinkedListStack* list)
{
	Node* currentTail = list->tail;
	Node* newTail;
	Data return_value;

	if(is_Empty(list))
	{
		printf("popping an empty stack is unavailable\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		newTail=list->tail->prev; 
		return_value=currentTail->data;
		newTail->next=NULL;
		list->tail=newTail;
	}
	list->size--;

	free(currentTail);
	return return_value;
}

void pop_middle(LinkedListStack* list, Data data) // popping not top of the stack, but middle of bottome of the stack
{
	int is_exist=0; // set to 1 if request data is in the stack
	Node* delNode;
	if(is_Empty(list))
	{
		printf("popping an empty stack is unavailable\n");
		return;
	}
	delNode=list->tail;
	do
	{
		if(delNode->data == data)
		{
			is_exist=1;
			break;
		}
		delNode=delNode->prev;
	}while(delNode);

	if(!is_exist)
	{
		printf("request data is not in the stack\n");
		return;
	}

	if(delNode==list->tail)
	{
		delNode->next->prev=delNode->prev;
		free(delNode);
		return;
	}
	if(delNode==list->head)
	{
		delNode->prev->next = delNode->next;
		free(delNode);
		return;
	}

	delNode->next->prev = delNode->prev;
	delNode->prev->next = delNode->next;
	free(delNode);
	return;



}

Node* tail_stack(LinkedListStack* list)
{
	return list->tail;
}

Node* head_stack(LinkedListStack* list)
{
	return list->head;
}

void destroy_stack(LinkedListStack* list) // destory stack's all node and stack itself
{
	Node* delNode = list->tail;
	while(delNode)
	{
		list->tail=list->tail->prev;
		free(delNode);
		delNode=list->tail;
	}
	free(list);
}

/* Example code 
 * int main()
 {
	  LinkedListStack* stack;
	  stack=stack_init();
	  push_stack(stack,123);
	  pop_tail(stack);
	  destroy_stack(stack);
	  return 0;
}
*/

#endif
