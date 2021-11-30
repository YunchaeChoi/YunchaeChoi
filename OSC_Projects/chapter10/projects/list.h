#ifndef __LIST
#define __LIST

#include <stdio.h>

typdef int LData;

typedef struct _Node
{
    struct _Node* next;
}Node;

typedef struct _List
{
    Node* head; // head is pointing dummy node
    int size;
}List;

void ListInit(List* list)
{
    list->head = (Node*)malloc(sizeof(Node));
    list->head->next=NULL;
    list->size=0;
}

Node* NodeInit(LData data)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data=data;
    newNode->next=NULL;
    return newNode;
}

int LIsEmpty(List* list)
{
    if(list->head->next==NULL)
        return 1;
    else
        return 0;
}

int LFirst(List *list, LData *pdata)
{
    if(LIsEmpty(list))
    {
        return 0;
    }
    list->before=list->head;
    list->cur=list->head->next;

    *pdata=list->cur->data; // store data of first node to pdata
    return 1;
}

void LInsert(List* list,LData data)
{
    Node* newNode=NodeInit(data);
    
    newNode->next= list->head->next; // 새 노드가 다른 노드를 가리키게 함 
    list->head->next= newNode;       // 더미 노드가 새 노드를 가리키게 함


    list->size++;
}

LData LRemove(List* list)
{
    Node* rpos = list->cur;
}

#endif
