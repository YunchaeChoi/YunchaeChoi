#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define SIZE 15000

typedef char* Data;

typedef struct _node
{
	Data data;
	struct _node *next;
}Node;

typedef struct LinkedListStack
{
	Node *top;
	int size;
}Stack;

int SIsEmpty(Stack *s);

void StackInit(Stack *s)
{
	s->top=NULL;
	s->size=0;
}

void SPush(Stack *s, Data data)
{
	Node *newNode = malloc(sizeof(Node));
	strcpy(newNode->data,data);
	newNode->next = s->top;

	s->top = newNode;
	s->size ++;

}

Data SPop(Stack *s)
{
	Node *temp;
	Data result;
	if(SIsEmpty(s))
	{
		exit(1);
	}	
	temp=s->top;
	strcpy(result,s->top->data);

	s->top=temp->next;
	s->size--;

	free(temp);
	return result;
}

int Size(Stack *s)
{
	return s->size;
}

int SIsEmpty(Stack* s)
{
	return s->top==NULL;
}

Data top(Stack *s)
{
	if(SIsEmpty(s))
	{
		printf("empty top\n");
		exit(1);
	}
	return s->top->data;
}


int main(int argc, char** argv)
{
	FILE *fp;
	char *line =NULL;
	size_t len =0;
	ssize_t nread;
	Stack stack;
	StackInit(&stack);	
	if(argc==1)
	{
		while((nread=getline(&line,&len,stdin)!=-1))
		{
			fwrite(line,sizeof(char),nread,stdout);
		}
		free(line);
	}
	if(argc==2)
	{
		fp = fopen(argv[1],"r");
		while((nread=getline(&line,&len,fp))!=-1)
		{
			SPush(&stack, line);
			printf("%s",top(&stack));
		}
		while(Size(&stack)>0)
		{
			printf("%s\n",SPop(&stack));
		}
		free(line);
		fclose(fp);
	}
}
