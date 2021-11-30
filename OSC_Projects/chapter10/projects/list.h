#ifndef __LIST
#define __LIST

#include <stdio.h>
#include <stdlib.h>

typedef int LData;

#define TRUE 1
#define FALSE 0

typedef struct _LNode
{
    struct _LNode* next;
    LData data;
}LNode;

typedef struct _List
{
    LNode* head; // head is pointing dummy node
    LNode* cur;
    LNode* before;
    int size;
}List;

void ListInit(List* list)
{
    list->head = (LNode*)malloc(sizeof(LNode)); // creating a dummy node
    list->head->next=NULL;
    list->size=0;
}

LNode* LNodeInit(LData data)
{
    LNode* newLNode = (LNode*)malloc(sizeof(LNode));
    newLNode->data=data;
    newLNode->next=NULL;
    return newLNode;
}

int LFirst(List *list, LData *pdata)
{
    if(list->head->next==NULL)
    {
        return FALSE;
    }
    list->before=list->head;
    list->cur=list->head->next;

    *pdata=list->cur->data; // store data of first node to pdata
    return TRUE;
}

int LNext(List* list, LData *pdata)
{
    if(list->cur->next==NULL)
        return FALSE;

    list->before=list->cur;
    list->cur = list->cur->next;

    *pdata = list->cur->data;
    return TRUE;
}

void LInsert(List* list,LData data)
{
    LNode* newLNode=LNodeInit(data);
    
    newLNode->next= list->head->next; // 새 노드가 다른 노드를 가리키게 함 
    list->head->next= newLNode;       // 더미 노드가 새 노드를 가리키게 함

    list->size++;
}

LData LRemove(List* list)
{
    LNode* rpos = list->cur;
    LData rdata = rpos->data;

    list->before->next=list->cur->next;
    list->cur=list->before;

    free(rpos);
    list->size--;
    return rdata;
}

int LSize(List* list)
{
    return list->size;
}


#endif
