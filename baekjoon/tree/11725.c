#include <stdio.h>
#include <stdlib.h>

typedef struct node_struct{
	int data;
	struct node_struct *left;
	struct node_struct *right;
}node;

node* insert_node(node* root, int value) // 트리에 값 삽입하기
{
	if(root==NULL)
	{
		root=(node *)malloc(sizeof(node));
		root->left = root->right = NULL;
		root->data = value;
	}
	else 
	{
		if(root->data > value) root->left = insert_node(root->left, value); 
        // 부모노드보다 값이 크면 왼쪽으로 보낸디
		else root->right = insert_node(root->right, value);
	}
	return root;
}






int main()
{
    int n;
    scanf("%d",&n);

    node *root = NULL;
    
}