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
	struct _Node* next; // to the top of the stack ( tail )
	struct _Node* prev; // going 'prev' side is going downward the stack ( to the bottom of the stack ) ( head )
}Node;

typedef struct _LinkedListStack
{
	Node* head; // bottom of the stack
	Node* tail; // top of the stack, ( must be popped when pop_tail() )
	int size;
}LinkedListStack;

typedef LinkedListStack Stack;
/*
Stack* MakeStack()
{
	Stack* return_stack = (Stack*)malloc(sizeof(Stack));
	return return_stack;
}
*/

void StackInit(Stack* stack);

Node* NodeInit(Data data);

int SIsEmpty(Stack* stack);

void SPush(Stack* stack, Data data);

Data SPop(Stack* stack);

Data SPop_middle(Stack* stack, Data data);

Data SPeek(Stack* stack);

Data SBottom(Stack* stack);

int StackSize(Stack* stack);

void StackShow(Stack* stack);

void StackDestroy(Stack* stack);


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

int SIsEmpty(Stack* stack)
{
	return !stack->size; // return 1 if Stack is empty, 0 if not empty
}

void SPush(Stack* stack, Data data)
{
	Node* newNode=NodeInit(data);
	Node* oldTail; // oldTail : 현재의 마지막(가장 위) 노드를 보관할 변수

	if(SIsEmpty(stack))
	{
		stack->tail = newNode;
		stack->head = newNode;
	}
	else
	{
		oldTail=stack->tail;
		oldTail->next=newNode;
		newNode->prev=oldTail;
		stack->tail=newNode;
	}
	stack->size++;
}

Data SPop(Stack* stack)
{
	Node* currentTail = stack->tail;
	Node* newTail;
	Data return_value;

	if(SIsEmpty(stack))
	{
		printf("popping an empty stack is unavailable\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		newTail=stack->tail->prev; 
		return_value=currentTail->data;
		newTail->next=NULL;
		stack->tail=newTail;
	}
	stack->size--;

	free(currentTail);
	return return_value;
}

Data SPop_middle(Stack* stack, Data data) // popping not top of the stack, but middle of bottome of the stack
{
	int is_exist=0; // set to 1 if request data is in the stack
	Node* delNode;
	Data value; // to be returned
	if(SIsEmpty(stack))
	{
		printf("popping an empty stack is unavailable\n");
		exit(EXIT_FAILURE);
	}
	delNode=stack->tail;
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
		exit(EXIT_FAILURE);
	}

	value=delNode->data;

	if(delNode==stack->tail)
	{
		delNode->prev->next=delNode->next;
		stack->tail= delNode->prev;
		free(delNode);
		return value;
	}
	if(delNode==stack->head)
	{
		delNode->next->prev = delNode->prev;
		stack->head = delNode->next;
		free(delNode);
		return value;
	}
	delNode->next->prev = delNode->prev;
	delNode->prev->next = delNode->next;
	free(delNode);
	stack->size--;
	return value;
}

Data SPop_Bottom(Stack* stack)
{
	Node *delNode= stack->head;
	Data value= stack->head->value;
	delNode->next->prev = delNode->prev;
	stack->head = delNode->next;
	free(delNode);
	return value;
}

Data SPeek(Stack* stack) // show top of the stack ( a.k.a. tail )
{
	return stack->tail->data;
}

Data SBottom(Stack* stack) // show bottom of the stack ( a.k.a. head )
{
	return stack->head->data;
}



int StackSize(Stack* stack)
{
	return stack->size;
}

void StackShow(Stack* stack)
{
	if(SIsEmpty(stack))
	{
		printf("stack is empty. nothing to show\n");
	}
	Node* showNode=stack->tail;
	while(showNode)
	{
		printf("%d\n",showNode->data);
		showNode=showNode->next;
	}
	return;
}

void StackDestroy(Stack* stack) // destory stack's all node and stack itself
{
	Node* delNode = stack->tail;
	while(delNode)
	{
		stack->tail=stack->tail->prev;
		free(delNode);
		delNode=stack->tail;
	}
	free(stack);
}

/* Example code 
-----------------------
int main()
{
	return 0;
}
-----------------------
*/

#endif
