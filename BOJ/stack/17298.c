#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef int Data;

typedef struct _node
{
    Data data;
    struct _node *next;
}Node;

typedef struct _liststack
{
    Node * head;
}ListStack;

typedef ListStack Stack;

void StackInit(Stack *pstack)
{
    pstack->head=NULL;
    pstack->head->next=NULL;
}

int SIsEmpty(Stack *pstack)
{
    if(pstack->head == NULL)
        return TRUE;
    else
        return FALSE;
}

void SPush(Stack *pstack, Data data)
{
    Node *newNode = (Node*)malloc(sizeof(Node));
    if(SIsEmpty(pstack)==TRUE)
    {
        //pstack->head=newNode;
        newNode->data=data;
        newNode->next=NULL;
    }
    else
    {
        newNode->data = data;
        newNode->next = pstack->head;
    }
    pstack->head=newNode;
}

Data SPop(Stack *pstack)
{
    Data rdata;
    Node * rnode;

    if(SIsEmpty(pstack))
    {
        printf("Stack Memory Error!\n");
        exit(-1);
    }
    else
    {
        rdata = pstack->head->data;
        rnode = pstack -> head;

        pstack -> head = pstack->head->next;
        free(rnode);

        return rdata;
    }
}

Data SPeek(Stack *pstack)
{
    if(SIsEmpty(pstack))
    {
        return -1;
    }

    return pstack->head->data;
}

Data SBottom(Stack *pstack)
{
    Node *tempNode = pstack->head;
    while(tempNode->next)
    {
        tempNode=tempNode->next;
    }
    return tempNode->data;
}

Data STop(Stack *pstack)
{
    return pstack->head->data;
}

Data SSearch(Stack *pstack, int i) // Ai
{
    Node *standardNode = pstack->head;
    Node *tempNode = pstack->head;
    for(int j=0;j<i-1;j++)
    {
        if(!standardNode->next)
        {
            printf("null\n");
            break;
        }
        standardNode=standardNode->next;
    }
// printf("standard: %d\n",standardNode->data);
    int answer=-1;
    for(int j=0;j<i-1;j++)
    {
    // printf("%d ",tempNode->data);
        if(tempNode->data > standardNode->data)
        {
            answer = tempNode->data;
        }
        if(!tempNode->next)
        {
            printf("null\n");
            break;
        }
        tempNode=tempNode->next;
    }
//   printf("\n");
    return answer;

}

void SPrint(Stack* pstack)
{
    Node* temp = pstack->head;
    
    while(temp)
    {
        printf("%d ",temp->data);
        if(!temp->next)
        {
            printf("null\n");
            break;
        }
        temp=temp->next;
    }
}

int main()
{
    Stack *stack;
    StackInit(stack);
    int n;
    scanf("%d",&n);
    int i;
    int an;
    for(i=0;i<n;i++)
    {
        scanf("%d",&an);
        SPush(stack,an);
    }


    for(i=n;i>0;i--)
    {
        int ans= SSearch(stack,i);
        printf("%d",ans);
        if(i>1) printf(" ");
    }
    printf("\n");
    

}