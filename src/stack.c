#include <stack.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

stacknode_t* new_node(int data){
	stacknode_t* node = (stacknode_t*)malloc(sizeof(struct stacknode_t));
	node->data = data;
	node->next = NULL;
	return node;
}

int isEmpty(stacknode_t* root){
	return !root;
}

void push(stacknode_t** root, int data){
	stacknode_t* node = new_node(data);
	node->next = *root;
	*root = node;
}

int pop(stacknode_t** root){
	if (isEmpty(*root)) {
		return INT_MIN;
	}
	stacknode_t* temp = *root;
	*root = (*root)->next;
	int popped = temp->data;
	free(temp);
	return popped;
}
int peek(stacknode_t* root){
	if (isEmpty(root)) {
		return INT_MIN;
	}
	return root->data;
}

