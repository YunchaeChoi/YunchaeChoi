#ifndef __STACK_H__
#define __STACK_H__

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

    newNode->data = data;
    newNode->next = pstack->head;

    pstack->head = newNode;
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
        if(tempNode->next==NULL)
            break;
        tempNode=tempNode->next;
    }
    // printf("\n");
    return answer;

}

#endif