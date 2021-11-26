#include "doubly_linked_stack.h"

int main()
{
	LinkedListStack* stack;
	stack=stack_init();
	push_stack(stack, 12);
	push_stack(stack, 13);

	printf("%d\n",pop_tail);

	destroy_stack(stack);
	return 0;
}
	
