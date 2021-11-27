#include "doubly_linked_stack.h"

int main()
{
	Stack* stack;
	stack = (Stack*)malloc(sizeof(Stack));	
	StackInit(stack);

	SPush(stack, 12);
	SPush(stack, 13);
	SPush(stack, 14);
	int value = SPop_middle(stack,13);

	printf("top: %d\n",SPeek(stack));
	printf("middle: %d\n",value);
	printf("size: %d\n",StackSize(stack));
	printf("%d\n",SIsEmpty(stack));


	StackDestroy(stack);
	return 0;
}
	
