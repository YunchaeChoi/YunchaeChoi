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
	if(newNode==NULL)
	{
		fprintf(stderr,"malloc failed\n");
		exit(1);
	}
	newNode->data = malloc(sizeof(data)/sizeof(char));
	if(newNode->data==NULL)
	{
		fprintf(stderr,"malloc failed\n");
		exit(1);
	}
	strcpy(newNode->data, data);
	newNode->next = s->top;

	s->top = newNode;
	s->size ++;

}

void SPop(Stack *s,FILE *out)
{
	Node *temp;
	if(SIsEmpty(s))
	{
		exit(1);
	}	
	temp=s->top;
	
	fprintf(out,"%s", s->top->data);

	s->top=temp->next;
	s->size--;

	free(temp);
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
			SPush(&stack, line);

		}
		while(Size(&stack)>0)
			SPop(&stack,stdout);
		free(line);
	}
	if(argc==2)
	{
		fp = fopen(argv[1],"r");
		char path[255];
		
		sprintf(path,"reverse: cannot open file '%s'\n",argv[1]);
		if(fp==NULL)
		{
			fprintf(stderr,"%s",path);
			exit(1);
		}
		while((nread=getline(&line,&len,fp))!=-1)
		{
			SPush(&stack, line);
		}
		while(Size(&stack)>0)
		{
			SPop(&stack, stdout);
		}
		free(line);
		fclose(fp);
	}
	if(argc==3)
	{
		char path[255];
		
		sprintf(path,"reverse: cannot open file '%s'\n",argv[1]);
		fp = fopen(argv[1],"r");
	
	

		if(fp==NULL)
		{
			fprintf(stderr,path);
			exit(1);
		}
		if(strcmp(argv[1],argv[2])==0)
		{
			fprintf(stderr,"%s","reverse: input and output file must differ\n");
			exit(1);
		}
		

		if(strstr(argv[1],"/")!=NULL && strstr(argv[2],"/")!=NULL)
		{
			char* temp1;
			char* temp2;
			temp1 = malloc(sizeof(argv[1])/sizeof(char));
			temp2 = malloc(sizeof(argv[2])/sizeof(char));

			strcpy(temp1,argv[1]);
			strcpy(temp2,argv[2]);
			char* ptr1 = strtok(temp1,"/");
			ptr1=strtok(NULL,"/");
			char* ptr2 = strtok(temp2,"/");
			ptr2 = strtok(NULL,"/");
			

			if(strcmp(ptr1,ptr2)==0)
			{
				free(temp1);
				free(temp2);
				fprintf(stderr,"%s","reverse: input and output file must differ\n");
				exit(1);
			}
			

		}
		FILE* fw;
		fw = fopen(argv[2],"w+");
		while((nread=getline(&line,&len,fp))!=-1)
		{
			SPush(&stack,line);
		}
		while(Size(&stack)>0)
		{
			SPop(&stack,fw);
		}
		fclose(fp);
		fclose(fw);


	}
	if(argc>3)
	{
		fprintf(stderr,"usage: reverse <input> <output>\n");
		exit(1);
	}

	return 0;
}
