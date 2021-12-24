#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node_struct{
	int data;
    struct Node_struct *parent;
	struct Node_struct *left;
	struct Node_struct *right;
}Node;

Node* cur; // global pointer that points the current node
Node* root;

int node_list[100001];

void init_list()
{
    for(int i=0;i<100001;i++)
        node_list[i]=0;
}

void set_cur(Node* node)
{
    cur = node;
}

void show_cur()
{
    printf("current value of cur: %d\n",cur->data);
}

void insert_Node_left(Node* parent_node, int value) // 트리에 값 삽입하기
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data=value;
    newNode->parent = parent_node;
    newNode->left=NULL;
    newNode->right=NULL;
    parent_node->left = newNode; 
    //printf("insert left completed!\n");
}


void insert_Node_right(Node* parent_node, int value) // 트리에 값 삽입하기
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data=value;
    newNode->parent=parent_node;
    newNode->left=NULL;
    newNode->right=NULL;
    parent_node->right = newNode; 
    //printf("inser right completed!\n");
}


int check_preorder(Node* root,int value)  
{                                    
    if(root)
    {
        if(root->data==value)
        {
            return 1;
        }
        if(root->left)
            check_preorder(root->left,value);
        if(root->right)
            check_preorder(root->right,value);
    }
    return 0;
}

void preorder(Node* root,int value)  // returns a node that contains the value.
{                                    // maybe have to use backtracking algorithm. brb
    if(root)
    {
        if(root->data==value)
        {
            //printf("found! : %d\n",root->data);
            set_cur(root);
        }
        if(root->left)
            preorder(root->left,value);
        if(root->right)
            preorder(root->right,value);
    }
}

void parent_preorder(Node* root,int value)  // returns a node that contains the value.
{                                    // maybe have to use backtracking algorithm. brb
    if(root)
    {
        if(root->data==value)
        {
            set_cur(root);
            printf("%d\n",cur->parent->data);
        }
        if(root->left)
            parent_preorder(root->left,value);
        if(root->right)
            parent_preorder(root->right,value);
    }
}

Node* insert_Node(int parent, int child) // 트리에 값 삽입하기
{
    preorder(root,parent);
    if(cur->left==NULL)
    {
        insert_Node_left(cur, child);
    }
    else if(cur->right==NULL)
        insert_Node_right(cur,child);
}

void init_root()
{
    root = (Node*)malloc(sizeof(Node));
    root->data = 1;
    root->parent=NULL;
    root->left=NULL;
    root->right=NULL;
    cur=root;
}

//int return_parent(Node* root,int value)
//{
//    Node* child = preorder(root,value);
//    return child->parent->data;
//}

void node_print_status(Node* target)
{
    printf("hi\n");
    printf("target's value: %d\n",target->data);
    if(target->parent)
        printf("target's parent: %d\n",target->parent->data);
    else
        printf("target has no parent\n");
    if(target->left)
       printf("target's left child: %d\n",target->left->data);
    if(target->right)
        printf("target's right child: %d\n",target->right->data);
    printf("-------------------------------------------------\n");
}

void free_tree()
{
    free(cur);
    free(root);
}

int main()
{
    int n,i;
    scanf("%d",&n);

    init_root();
    init_list();
    node_list[1] =1;

    int is_there;

    int parent,child;
    for(i=0;i<n-1;i++)
    {
        scanf("%d %d",&parent, &child);
        if(node_list[parent]==0)
        {
            insert_Node(child,parent);
            node_list[parent]=1;
        }
        else
        {
            insert_Node(parent,child);
            node_list[child]=1;
        }
       // show_cur();
    }
    for(i=2;i<=n;i++)
    {
        parent_preorder(root,i);
    }
    free_tree();
}
