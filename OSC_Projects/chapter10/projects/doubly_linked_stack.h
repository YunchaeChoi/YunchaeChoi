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
	struct _Node* next; // to the bottom of the stack ( head )
	struct _Node* prev; // going 'prev' side is going upward the stack ( to the top of the stack ) ( tail )
}Node;

typedef struct _LinkedListStack
{
	Node* head; // bottom of the stack
	Node* tail; // top of the stack, ( must be popped when pop_tail() )
	int size;
}LinkedListStack;

typedef LinkedListStack Stack;

Stack* MakeStack()
{
	Stack* return_stack = (Stack*)malloc(sizeof(Stack));
	return return_stack;
}

void StackInit(Stack* stack) // initializes the stack
{
	stack->head=NULL;
	stack->tail=NULL;
	stack->size=0;
}

Node* NodeInit(Data data)
{
	Node* newNode= (Node*)malloc(sizeof(Node));
	newNode->data = data;
	newNode->next=NULL;
	newNode->prev=NULL;
	return newNode;
}

int SIsEmpty(Stack* list)
{
	return list->size && 0; // return 1 if Stack is empty, 0 if not empty
}

void SPush(Stack* list, Data data)
{
	Node* newNode=Node_init(data);
	Node* oldTail; // oldTail : 현재의 마지막(가장 위) 노드를 보관할 변수

	if(SIsEmpty(list))
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

Data SPop_tail(Stack* list)
{
	Node* currentTail = list->tail;
	Node* newTail;
	Data return_value;

	if(SIsEmpty(list))
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

void SPop_middle(Stack* list, Data data) // popping not top of the stack, but middle of bottome of the stack
{
	int is_exist=0; // set to 1 if request data is in the stack
	Node* delNode;
	if(SIsEmpty(list))
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

Node* SPeek(Stack* list) // show top of the stack ( a.k.a. tail )
{
	return list->tail;
}

Node* SBottom(Stack* list) // show bottom of the stack ( a.k.a. head )
{
	return list->head;
}

void StackShow(Stack* list)
{
	if(SIsEmpty(list))
	{
		printf("stack is empty. nothing to show\n");
	}
	Node* showNode=list->tail;
	while(showNode)
	{
		printf("%d\n",showNode);
		showNode=showNode->next;
	}
	return;
}

void StackDestroy(Stack* list) // destory stack's all node and stack itself
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
	  Stack* stack;
	  stack=stack_init();
	  push_stack(stack,123);
	  pop_tail(stack);
	  destroy_stack(stack);
	  return 0;
}
*/

#endif
